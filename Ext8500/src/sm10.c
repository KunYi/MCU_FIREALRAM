
//工作紀要
// 需要實作 Resend 機制
// Tx/Rx Timeout 機制
// #define DEBUG_RECEIVE_PORT1
#define _SM10_C_
#include "common.h"          /* common typer define                        */ 
#include "regw77e58.h"
#include "ptimer.h"          /* physical timer0 interrupt                  */
#include "stimer.h"          /* soft timer                                 */
#include "callsofttimer.h"
#include "uart.h"
#include "display.h"         /* L.E.D. Display                             */
#include "key.h"
//#include "relay.h"                                                                                     
#include "eeprom.h"
#include "sensor.h"
//#include "speech.h"
#include "bitio.h"
#include "P1Layer3.h"
#include "P2Layer3.h"
#include "P1L3CMD.h"
#include "sm10.h"
#include "lcd.h"
#include "P1L3CMD.h"
#include "misc.h"



//#include "gfifo.h"
ExportQueFunction(KeyFIFO)

CreateQue(P1FIFOIn)
CreateQue(P1FIFOOut)
ImplementQue(P1FIFOIn)
ImplementQue(P1FIFOOut)

CreateQue(P2FIFOIn)
CreateQue(P2FIFOOut)
ImplementQue(P2FIFOIn)
ImplementQue(P2FIFOOut)



#define  GetKeyTime 14
                                   
//const CODE_DATA BYTE Password[] = {  5, 3, 7, 6, 0};
#include "CopyRight.C"
static volatile CallBack LEDRefresh = { Refresh, 200, 0 };
static volatile CallBack KeyGet = { PHYSICAL_GetKey, GetKeyTime, 0};
static volatile CallBack SoftTimerRefresh = { CallSoftTimer, 10, 0};


SYS_STATUS sysStatus;


bit PreMicPhonePressState;
// -- declaration Global variable
 
 
 EXTERN_DATA WORD PreEEPROMWriteTimer;
   
// BYTE MyID; // Communication ID
 BYTE Item;
 BYTE KeyCode;
 BYTE ReceID;
 BYTE Status1;
 BYTE Status2;

// End Declaration globel variable

#ifdef DEBUG_RECEIVE_PORT1
// implement follow function in uart_v101.c
void DEBUG_PORT1(void);
#endif

void main (void)
{
	PMR |= 0x01; // Enable W77E58 on chip 1K XRAM
	
	P_CallBack[0] = &SoftTimerRefresh;
	P_CallBack[1] = &KeyGet;
	P_CallBack[2] = &LEDRefresh;
	P_CallBack[3] = 0;
	
	IsStartSoftTimer1      =   OFF;
	IsStartSoftTimer2      =   OFF;
	IsStartSetupSoftTimer  =   OFF;
	IsStartSystemSoftTimer =   OFF;
	
	WatchDog = ~WatchDog;	// Reset hardware watch dog timer
	
	RESET_REGISTER(EA);		// Disable Interrupt
	DEVICE_Enable = 1;		// DEVICE Disable
	
	P_CallBack[0] = &SoftTimerRefresh;
	P_CallBack[1] = &KeyGet;
	P_CallBack[2] = &LEDRefresh;
	P_CallBack[3] = 0;
	
	ClrAllLED();
	ClrFlash(SOUND_LED);
	ClrLED(SOUND_LED);
	Refresh();
	
	DEVICE_Enable = 0;		// DEVICE Enable
	
	// initial EmgTel
	InitialSensor();

	InitUART0();
	InitUART1();
	
	InitialQue(P1FIFOIn);
	InitialQue(P1FIFOOut);
	InitialQue(P2FIFOIn);
	InitialQue(P2FIFOOut);
	InitialQue(KeyFIFO);
	
	WatchDog = ~WatchDog;  // Reset hardware watch dog timer
	
	// Initial MyId
	MyID = GetCommID();
	MyID = GetCommID();
	WatchDog = ~WatchDog;  // Reset hardware watch dog timer
	MyID = GetCommID();
	
	Init_Physical_Timer0();	// Init Timer0
	Start_Timer0();			// Star Timer0
	Enable_Timer0_Interrupt(); // Enable Timer 0 Interrupt
	SET_REGISTER(EA);		// Enable Interrupt
	
	InitialSoftTimer(SoftTimer1);
	InitialSoftTimer(SystemSoftTimer);
	
	IsStartSoftTimer1 = ON;
	IsStartSystemSoftTimer = ON;
	PreMicPhonePressState  = IsMicPhonePress();
	InitialSensor();
	
	WatchDog = ~WatchDog;		// Reset hardware watch dog timer
	LcdScreen(LCD_EN_AUTOOFF);	// Enable LCD auto off
	LCDDelay();
	WatchDog = ~WatchDog;		// Reset hardware watch dog timer
	LCDDelay();	
	WatchDog = ~WatchDog;		// Reset hardware watch dog timer
	LcdScreen(LCD_GW_TURN);     // trun around for GW MARK
	LCDDelay();
	WatchDog = ~WatchDog;		// Reset hardware watch dog timer
	LCDDelay();	
	WatchDog = ~WatchDog;		// Reset hardware watch dog timer
	LcdScreen(LCD_GW_TURN);     // trun around for GW MARK
	WatchDog = ~WatchDog;		// Reset hardware watch dog timer
	LCDDelay();
	WatchDog = ~WatchDog;		// Reset hardware watch dog timer

#ifdef DEBUG_RECEIVE_PORT1
      DEBUG_PORT1();
#endif
	while (TRUE)
	{
		WatchDog = ~WatchDog;		// Reset hardware watch dog timer
		
		// process of layer 2
		RxP1();
		RxP2();
		
		// process of layer 3
		if (P1PackageReceive == TRUE)
		{
			P1PackageReceive = FALSE;
			P1Process();
		}
		
		if (P2PackageReceive == TRUE)
		{
			P2PackageReceive = FALSE;
			P2Process();
		}
		
		#include "P1APPLICATION.C"
		if ((PreMicPhonePressState != IsMicPhonePress()))
		{
			// store state
			LcdScreen(LCD_BL_ON);
			PreMicPhonePressState = IsMicPhonePress();
			if (PreMicPhonePressState) {
				// set MicPhonePress
				P1TxCMD(RM_MIC_PRESS, 0x00, 0xFF);
				SelectAudioSource(MIC_AUDIO);
			} else
			{
				// clear MicPhonePress
				P1TxCMD(RM_MIC_PRESS, 0x00, 0x00);
				SelectAudioSource(ALLCLOSE_AUDIO);
			}
		} else if (!IsEmptyOfQue(KeyFIFO))
		{
			// get input key
			// press a key
			KeyCode = GetKey();
			//      sync, leng, GMASTERID, Type,  MyID, TagetID,  KeyCode, CheckSum
			// send 0x15, 0x07,      0x80, 0x01,  0x00,    0x00,     0x01,     0x77
			// send 0x15, 0x07,      0x80, 0x01,  0x00,    0x00,     0x0a,     0x6E
			// send 0x15, 0x07,      0x80, 0x01,  0x00,    0x00,     0x27,     0x51
			// send 0x15, 0x07,      0x80, 0x01,  0x00,    0x01,     0x03,     0x74
			// send 0x15, 0x07,      0x80, 0x01,  0x00,    0x02      0x05,     0x71
			if (KeyCode != 0xFF)
			{
				LcdScreen(LCD_BL_ON);
				P1TxCMD(RM_KEY, 0x00, KeyCode);
			}
		} else if (GetFromQue(P2FIFOIn, &Item))
		{
			switch(Item) {
			case P2_RMEU_KEY:
				GetFromQue(P2FIFOIn, &ReceID);
				GetFromQue(P2FIFOIn, &Status1);
				P1TxCMD(RM_KEY, ReceID, Status1);
				break;
			}
		} else
		{
			// check TelDiff
			CheckTel();
			if (IsTelDiff())
			{
				// send Group function
				P1TxCMD(RM_GROUP, (TelSensor[0]), (TelSensor[1]));
			}
		}
		
		UART0Drive();
		UART1Drive();
		
		// added for self-refresh LCD
		ReLCDScreen();
	} /* while (TRUE) end */
} /* main() end */

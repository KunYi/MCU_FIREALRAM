// 
// File Name   : P1Layer3.C
// Auther      : K'un I Chen
// Create      : 07/26/2003
// version     :
// Objective   :
// Description : 連接 GMaster 
// History     :

#include "P1Layer3.h"
#include "P1L3CMD.h"
#include "Display.h"
#include "lcd.h"
#include "eeprom.h"
#include "sm10.h"

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80


extern BYTE MyID;


CODE_DATA const BYTE CONVBIT[] = { BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7 };
#define LEDTable CONVBITd P1ResponseACK(BYTE ID, BYTE Cmd);

static EXTERN_DATA BYTE P1TXBUFF[10];
#define GMASTER_ID  0x80

//#define ExportQueFunction(x)  extern _PutTo##x(BYTE Val);             
//                              extern _GetFrom##x(PBYTE Val);                    

ExportQueFunction(P1FIFOIn)
ExportQueFunction(P1FIFOOut)
ExportQueFunction(P2FIFOIn)
ExportQueFunction(P2FIFOOut)
//#include "Gfifo.h"

  //  系統端傳送到延伸副機
  //  4       SubID    0xF1                                        測試是否有這個 ID
  //  12      0xF0+ID  0x01, ID,   RED LED,          ( 2 BYTE)
  //  13      0xF0+ID  0x02, ID,   Green LED,        ( 2 BYTE)
  //  14      0xF0+ID  0x03, ID,   Control LED,      ( 2 BYTE)
  //  15      0xF0+ID  0x04, ID,   Group LED,        ( 2 BYTE)
  //  12      0xF0+ID  0x05, ID,   Flash RED LED,    ( 2 BYTE)
  //  13      0xF0+ID  0x06, ID,   Flash Green LED,  ( 2 BYTE)
  //  14      0xF0+ID  0x07, ID,   Flash Cnt LED,    ( 2 BYTE)
  //  15      0xF0+ID  0x08, ID,   Flash Group LED,  ( 2 BYTE)
  //  16      0xF0+ID  0x09, LCD Status
  //  17      0xF0+ID  0x0A                                            RESET COMMAND
  //  3       SubID    0xF0, Cmd                                       回應 ACK 給 延伸副機

void P1Process(void) {
  switch(P1RxBuff[1]) {
  case CMD_UNIT_RESET:
    break; 
  // entry mode
  #include "P1APP\E_MANU.C"
  #include "P1APP\E_GROUP.C"
  #include "P1APP\E_BGM.C"
  // #include "P1APP\E_PREALARM.C"
  #include "P1APP\E_EMGTEL.C"

  // quit mode
  #include "P1APP\Q_MANU.C"
  #include "P1APP\Q_GROUP.C"
  #include "P1APP\Q_BGM.C"
  
  // active mode
  #include "P1APP\A_GROUP.C"
  #include "P1APP\A_MANU.C"
  
  // key mode
  #include "P1APP\KEY.C"

  // PTT push mode
  #include "P1APP\PTT.C"
  // EMG PROC
  #include "P1APP\EMG.C"
  #include "P1APP\SPC.C"
  #include "P1APP\SHORT.C"
  #include "P1APP\DISPLAY.C"

  case RESET_CMD:
    // reset command
    P1ResponseACK(P1RxBuff[0], P1RxBuff[1]);
    PutToQue(P1FIFOIn, P1RxBuff[1]);
    break;

  case QUERY_ID_STATUS: // QuerID Syn, 4, ID, Query(0xF0), Check
    P1ResponseACK(P1RxBuff[0], P1RxBuff[1]);
    break;

  default:
    break;
  }
} // terminal of P1Process(void)


void TranslateP1(register BYTE Item)
{
  switch (Item)
  {
    case  1:
        break;

    case  2:
        break;

    case 0xF0:
        break;

    default:
        break;
  }
} // terminal of TranslateP1(register BYTE Item)

  // 必須實作 且已經實作
  // P2 term
  // 延伸副機傳到系統端
  // No.   Address   content
  //  1       0x80   0x01, SubID, ID, nn Key              延伸副機發出
  //  2       0x80   0x02, SubID, ExtendID, nn Key        延伸副機發出 //press
  //  3       0x80   0x03, SubID, ID,  Set/Clear Key      MicPhone State
  //  11      0x80   0xF0, SubID                          延伸副機回應 ACK   

void P1TxCMD(BYTE CMD, BYTE Value, BYTE Value1)
{
  BYTE CheckSum;

  P1Port_Put(P1Package_SYNC);

  CheckSum = 0x07 + GMASTER_ID;

  P1Port_Put(0x07);           // Len
  P1Port_Put(GMASTER_ID); 

  CheckSum += CMD;
  P1Port_Put(CMD);

  CheckSum += MyID;
  P1Port_Put(MyID);  

  CheckSum += Value;
  P1Port_Put(Value);

  CheckSum += Value1;
  P1Port_Put(Value1);

  P1Port_Put((~CheckSum+1));  
// Add Buffer re-send use
  P1TXBUFF[0] = 0x07;
  P1TXBUFF[1] = GMASTER_ID;
  P1TXBUFF[2] = CMD;
  P1TXBUFF[3] = MyID;
  P1TXBUFF[4] = Value;
  P1TXBUFF[5] = Value1;
  P1TXBUFF[6] = ~CheckSum+1;
}


// transmitter a ACK packet to GMASTER controller
// Sync, packet_len, ACK(0xF0), ID, Cmd
void P1ResponseACK(BYTE ID, BYTE Cmd)
{
  BYTE CheckSum; 

  if (ID > 0xF0) 
  {
    if ((ID - 0xF0) != MyID)
    {
      return;
    } 
    else 
    { 
      ID = MyID; 
    }
  }

  
  P1Port_Put(P1Package_SYNC);

  P1Port_Put(ACK_Len);

  P1Port_Put(GMASTER_ID);

  CheckSum = ACK_Len + GMASTER_ID;

  CheckSum += 0xF0;     // ACK
  P1Port_Put(0xF0);

  CheckSum += ID;
  P1Port_Put(ID);
     
  CheckSum += Cmd;
  P1Port_Put(Cmd);
  
  P1Port_Put((~CheckSum+1));

}


#include "stimer.h"          /* soft timer                                 */
#include "communication.h"   /* Extern Unit and Call Station communication */
#include "key.h"
#include "relay.h"                                                                                     
#include "eeprom.h"
#include "sensor.h"
#include "speech.h"


#define _DISABLE_MAXVOLUME_  1  // added disable Group/Handle Vol. Feb, 16, 2005
#define _FOR_TRTS_ 1
//#define __DEBUG_FIRE_ALRAM__
//#define _DEBUG_VERSION_
#ifdef DEBUG_RECEIVE_PORT1
void DEBUG_PORT1(void);
#endif

ExportQueFunction(KeyFIFO)
//_PutToKeyFIFO(BYTE Value);

#define  GetKeyTime 14

void CallSoftTimer(void);
void SetFireRelationZone(void);
                                   
//const CODE_DATA BYTE Password[] = {  5, 3, 7, 6, 0};
// Revesion
#include ".\CB\rev.c"
// encode looktable for input/output mapping
#include ".\CB\encode.c"
// copy right magic number
#include ".\CB\copyright.c"    

                             
static CallBack LEDRefresh = { Refresh, 200, 0 };
static CallBack KeyGet = { PHYSICAL_GetKey, GetKeyTime, 0};
static CallBack SoftTimerRefresh = { CallSoftTimer, 10, 0};
static CallBack RelayRefresh = { RefreshRelay, 400, 0 };

SoftTimer idata SoftTimer1;
SoftTimer idata SoftTimer2;
SoftTimer idata SoftTimer3;
//static P_INTERNAL_DATA SoftTimer TxStatusTiemr;
SoftTimer idata SetupSoftTimer;
SoftTimer idata SystemSoftTimer;

// static P_INTERNAL_DATA BYTE TxStatusTimer;

bit IsStartSoftTimer1;
bit IsStartSoftTimer2; 
bit IsStartSoftTimer3;
bit IsStartSetupSoftTimer; 
bit IsStartSystemSoftTimer;


// #define DEBUG_TIMER

#ifdef DEBUG_TIMER
bit IsDebugTimer;
static P_INTERNAL_DATA SoftTimer DebugTimer;
#endif
// END DEBUG_TIMER

// @@@@@@@@@@@@@@@  Declaration Global variable  @@@@@@@@@@@@@@
 Status                     ProcState = InitialState;
 Status                     CallStationProcState = InitialCallStationState;
 P_INTERNAL_DATA Status     OnlyEmergencyUseTempState;
 P_INTERNAL_DATA Status     StorageStatus;
 Status                     CommunicationProcState;
 Status                     CallStationState;
 volatile BYTE idata     GenCount1;
 volatile BYTE idata     GenCount2;
 volatile BYTE idata     GenCount3;
 volatile BYTE idata     GenCount4; // for EEPROM
          BYTE xdata     SFireCount; 
 
 EXTERN_DATA BYTE CallStationBuffer[4];
 //EXTERN_DATA BYTE SetupBuffer[20];
 EXTERN_DATA BYTE SetupBuffer[31];		//by andy
 EXTERN_DATA BYTE FireZoneBuffer[(((MaxExtendUnit+1)*2)+10)];
 EXTERN_DATA BYTE LoopShortBuffer[2];
 EXTERN_DATA BYTE FireRelationMask[2];
 EXTERN_DATA int  FireZoneCount;
 EXTERN_DATA WORD PreEEPROMWriteTimer;
 EXTERN_DATA EMG_KEYSET[2];
 
 BYTE KeyCode;
 BYTE Chime;
 bit PreDelay;

BYTE Delay1;
WORD Delay2;
  
 bit IsSpeechPlayer = FALSE;
 bit IsFireActiveFlag = FALSE;
 bit IsLoopShortFlag = FALSE;
 bit IsBGMMode, IsPasswordOK, IsDisFireAlarm;
// bit IsPreEmergencyMode, IsEmergencyMode;
 bit IsAllZoneEmergencyMode, IsNoFireMode;
 bit IsNoFireCancelMode;
 
// **************  End Declaration globel variable ****************

extern bit PackageReceive;

bit IsPreEmergencyMode, IsEmergencyMode;
static bit IsAllCall = 0;


void main (void) {

//  bit IsTwoFireZone;
 /* test var */
 
 PMR |= 0x01; /* Enable W77E58 on chip 1K XRAM */

 P_CallBack[0] = &SoftTimerRefresh;
 P_CallBack[1] = &KeyGet;
 P_CallBack[2] = &LEDRefresh;
 P_CallBack[3] = 0;
  
 IsStartSoftTimer1      =   OFF;
 IsStartSoftTimer2      =   OFF;
 IsStartSetupSoftTimer      =   OFF;
 IsStartSystemSoftTimer =   OFF;
 IsPreEmergencyMode     = FALSE;
 IsEmergencyMode        = FALSE;
 IsAllZoneEmergencyMode = FALSE;
 IsNoFireMode           = FALSE;
 IsNoFireCancelMode     = FALSE;
 IsFireActiveFlag       = FALSE;
 IsLoopShortFlag        = FALSE;
 IsDisFireAlarm         =  TRUE;
 
 ClrRemoteMicPress();  // clear remote micphone button flag

 InitialQue(KeyFIFO);

 GenCount1     = 0;
 GenCount2     = 0;
 GenCount3     = 0;
 GenCount4     = 0;
 SFireCount    = 0;
 PreDelay = 0;
 FireZoneCount = 0;
 Chime = Chime1; 
 ProcState  = InitialState;

#ifdef __DEBUG_FIRE_ALRAM__
 // debug
 FireZoneBuffer[1] = 0;
 FireZoneBuffer[0] = 0;
#else
 // release version
 FireZoneBuffer[1] =  ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireStatusStorage, 0, 0));
 FireZoneBuffer[0] =  ~Read_BYTE_EEPROM((CalcEEPROMOffset(FireStatusStorage, 0, 0)+1));
#endif

 StorageStatus     =  Read_BYTE_EEPROM((CalcEEPROMOffset(FireStatusStorage, 0, 0)+2));

 //-----------------------------Delay 4 sec-------------------------------------------------------------------------------------
/*
 for(Delay1=0;Delay1<40;Delay1++)
 {
  for(Delay2=0;Delay2<20000;Delay2++)
	 	{	 WatchDog = ~WatchDog;  }  
	 }
*/
 //-----------------------------Delay 4 sec-------------------------------------------------------------------------------------
 //---------------------------------------------bY andy----------------------------	
 voice_bank_select=0x02;                       //OKI_RESET   &enable 74HC139
 voice_bank_select=0x82;                       //oki working at oki_reset Pin High
 //--------------------------------------------------------------------------------
  while (TRUE) {
    // check idle state in setup mode, 
    // over ten minute then exit setup mode
    if ((SetupSoftTimer.min >= 10 ) && (IsStartSetupSoftTimer == ON)) {
      IsStartSetupSoftTimer = OFF;
      ProcState = ExitSetupModeState;
    }
    if ((PackageReceive == TRUE )&&(IsStartSetupSoftTimer == ON)) {
      InitialSoftTimer(SetupSoftTimer);
    }
    WatchDog = ~WatchDog;  /* Reset hardware watch dog timer */
   
    /* Relay refresh */
    if ((SystemSoftTimer.m10sec % 30) == 0) {  
      RefreshRelay();  
    }
     
    /* fire and loop shor state refresh */
    if ((SystemSoftTimer.m10sec % 50) == 0) { 
      if (!IsDisFireAlarm) { 
        CheckFire();
      } 
      CheckTel(); // for Group function
      CheckShort();
    }
   
    /* process extend unit packect*/
    CommunicationProcState = ProcExtendUnit(ProcState, &SetupBuffer[0], &FireZoneBuffer[0]);
   

    //========================================================================

    /* process call station packect */
    CallStationState = ProcCallStation(CallStationState, &CallStationBuffer[0]);
    /* receive extend unit */
    ExtendUnit_Receive();
    /* receive callstation */
    CallStation_Receive();
  
    /* MSM6550 Speech FIFO player*/ 
    SpeechPlayer();
    WatchDog = ~WatchDog;  /* Reset hardware watch dog timer */
   
    if (!IsDisFireAlarm) {
      // Check extend unit status
      // Is fire alram or emergency tel?
      if ((CommunicationProcState == ExtendUnitFireActiveState) || (CommunicationProcState == EmergencyTelState)) {
        
        P_INTERNAL_DATA BYTE TempCount, Value1, Value2;
        P_INTERNAL_DATA BYTE Count, Temp;
      
        CallStationFire(); //by andy
        //--------------------------add by andy at 2002/1/18--------------------//
        // Check while clr all led ???
        // Check while clr all led ???
        if ((ProcState==CallStationBGMPlayState) || 
            (ProcState==CallStationSelectZoneState)||
            (ProcState==CallStationModeState)) {
          //if((IsPreEmergencyMode !=TRUE) ||(IsEmergencyMode !=TRUE))
          ClrAllLED();			//by andy
        } 
        //----------------------------------------------------------------------//
       
        // active emergency tel hook 
        if (CommunicationProcState == EmergencyTelState) {
          if (!IsEmergencyMode) {
            ProcState = InitialConfirmFireEmergencyState;
          }
        } else {
          if (!IsPreEmergencyMode) { 
            ProcState = InitialPreFireEmergencyState; 
          }
        }
        GenCount2 = 0;
       
       
        // Check current mode 
        // if not emergency in advance or emgergency or all zone emergency
        if (!IsPreEmergencyMode && !IsEmergencyMode && !IsAllZoneEmergencyMode)  {
          GenCount1 = 0;
          GenCount3 = 0;
          GenCount4 = 0;
          SetupBuffer[0] = 0;
          SelectAudioSource(ALLCLOSE_AUDIO);
          ClrFlash(SOUND_LED);
          ClrLED(SOUND_LED);
          while(IsSpeechBusy());
          SpeechStop();
          ClearSpeechFIFO();
          SelectAudioSource(DIGITAL_AUDIO);
          ClrAllLED();
          ClrAllRelay();
          SetFlash(SOUND_LED);
  //      DisCallStation();
          IsStartSoftTimer1      = OFF;
          IsStartSoftTimer2      = OFF;
        }

        Value2 = FireZoneBuffer[((MaxExtendUnit+1)*2)+0];
        Value1 = FireZoneBuffer[(((MaxExtendUnit+1)*2)+2)];
       
        for (TempCount = 0; TempCount < 10; TempCount++) {
          if (TempCount == 8 ) {
            Value1 = FireZoneBuffer[(((MaxExtendUnit+1)*2)+1)];
          }
             
          if ((Value1 & EnCode1[TempCount]) == EnCode1[TempCount])  { 
            FireZoneCount++;   
            Temp = ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireRelationStorage, Value2, TempCount));
            for (Count = 0; Count < 10; Count++) {
              if (Count == 8) {
                Temp = ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireRelationStorage, Value2, TempCount)+1);
              }
                    
              if ((Temp & EnCode1[Count]) == EnCode1[Count])  {
                SetLED(Count+ZONE_ACTIVE_LED);
                if (Count < 8) { 
                  FireRelationMask[0] |= EnCode1[Count]; 
                } else { 
                  FireRelationMask[1] |= EnCode1[Count];
                }
                
                SetRelay(Count);
              }
            } /* for (Count) end */
          } /* if (Value1) end */
        } /* for (TempCount) end */

        // refresh LED & Relay state
        Refresh();
        RefreshRelay();
        
        if (!IsEmergencyMode) {
          if (FireZoneCount >= 2) {
		  	if (CheckIsAlarmHoldState != ProcState || 
				InitialCheckIsAlarmHoldState != ProcState) {
            	ProcState = InitialConfirmFireEmergencyState;
		  	}
            GenCount1 = 0;
            GenCount2 = 0;
            GenCount3 = 0;
            GenCount4 = 0;
          }
        }
      }    /* if (CommunicationProcState) end */
    }      /* if (!IsDisFireAlarm) end */

#undef TempCount
#undef Value1
#undef Value2
#undef Count
#undef Temp

  UART0Drive();
  UART1Drive();

  KeyCode = GetKey();
  WatchDog = ~WatchDog;  /* Reset hardware watch dog timer */
/* Process Routine */
  switch (ProcState) {
/* ************************************************************************* */
/* ************************************************************************* */
/* System initial state                                                      */
/* ************************************************************************* */
/* ************************************************************************* */
#define InitialStateCount  GenCount2

  case InitialState:
    #include ".\PS\InitialState.C"
    break;      

  // Check Prev Power off status
  case CheckPrevStatus:
    #include ".\PS\CheckPrevStatus.c"
    break;
  
  // System normal state
  case NormalState:
    #include ".\PS\NormalState.c"
    break;

  // Entry Setup State : check Input 4 digital number password
  case EntryCheckPasswordState:
    #include ".\PS\EChkPW.C"
	break;

#define SetupCount GenCount2
  case InitialEntrySetupState:
    #include ".\PS\InitEntrySetupState.c"
    break;
      
 case ExitEntrySetupState:
   IsStartSetupSoftTimer = OFF;
   GenCount2 = 0;
   ProcState = InitialState;
   break;
// Entry setup state process end

// Setup Mode Select State : Select
 case SetupModeSelectState:
   #include ".\PS\SetModeSel.c"
   break;
 
 case SetupModeSelectSpecialState:
   #include ".\PS\SetModeSS.c"
   break;
 
  case ExitSetupModeState:
    #include ".\PS\ExtSetMode.c"
	break;
#undef SetupCount 
// Setup mode select process end

/* ************************************************************************* */
/* ************************************************************************* */
/*   Setup fire alarm zone relation State :                                  */
/* ************************************************************************* */
/* ************************************************************************* */
#define SetupFireCount GenCount2
 case InitialSetupFireRelationState: { // for UEStudio Folding function
  if (SetupFireCount == 0) { 
      EntryFireRelationSetup();
      SetupFireCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
  }
       
  if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      EntryFireRelationSetup();
      SetupFireCount++;
      InitialSoftTimer(SoftTimer1);
  }

  if ((SetupFireCount == 3) || 
      (CommunicationProcState == InitialSetupFireRelationState)) {
    //  SetLED(HUMAN_HANDLING_LED);
      SetupFireCount = 0;
      ProcState = SetupFireRelationZoneState;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
  } else { IsStartSoftTimer1 = ON; }
  } // for UEStudio Folding function
  break;
 
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */ 
 case SetupFireRelationZoneState: { // // for UEStudio Folding function
   switch (KeyCode) {

   case LCD4_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = ExitSetupFireRelationState;
     LcdScreen(LCD_QUIT_SETUP);
     break;
   case BACKLIGHT_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     LcdScreen(LCD_BACKLIGHT);
     break;
   default:
     if (KeyCode <= ZONE_KEY10) {    
       InitialSoftTimer(SetupSoftTimer);
       SetupBuffer[0] = 0;
       SetupBuffer[1] = KeyCode;
       ProcState = FireRelationZoneAssigenState;
       LcdScreen(LCD_SETUP_FIRE_ZONE);
     }
     break;
   }

   if (CommunicationProcState == SetupFireRelationZoneState) { 
     InitialSoftTimer(SetupSoftTimer);
     ProcState = ReadFireRelationSetupState;
     LcdScreen(LCD_SETUP_FIRE_ZONE);
   }
   } // for UEStudio Folding function
   break;
 
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */   
  case FireRelationZoneAssigenState: { // for UEStudio Folding function
    if (SetupFireCount == 0) { 
      FireRelationSetupZoneAssigen(SetupBuffer[1]+1);
      SetupFireCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      FireRelationSetupZoneAssigen(SetupBuffer[1]+1);
      SetupFireCount++;
      InitialSoftTimer(SoftTimer1);
    }      
     
    if ((SetupFireCount == 3) ||
        (CommunicationProcState == FireRelationZoneAssigenState)) {
      InitialSoftTimer(SetupSoftTimer);
      SetLED(SetupBuffer[1]);
      SetupFireCount = 0;
      ProcState = ReadFireRelationSetupState;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    } else  { 
     IsStartSoftTimer1 = ON; 
    }
    } // for UEStudio Folding function
    break;
  
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */ 
 case ReadFireRelationSetupState: { // for UEStudio Folding function
   SetupBuffer[2] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireRelationStorage, SetupBuffer[0], SetupBuffer[1]));
   SetupBuffer[3] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(FireRelationStorage, SetupBuffer[0],SetupBuffer[1])+1));

   for (SetupFireCount = 0; SetupFireCount < 8; SetupFireCount++) {
     if ((SetupBuffer[2] & EnCode1[SetupFireCount]) == EnCode1[SetupFireCount])
     { 
       SetLED(SetupFireCount+ZONE_ACTIVE_LED); 
     }
   }

   for (SetupFireCount = 0; SetupFireCount < 2; SetupFireCount++)
   {
      if ((SetupBuffer[3] & EnCode1[SetupFireCount]) == EnCode1[SetupFireCount])
      { 
        SetLED(SetupFireCount+ZONE_ACTIVE_LED+8); 
      }
   }
   SetupFireCount = 0;
   ProcState = SetupFireRelationState;
   } // for UEStudio Folding function
   break;
   
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */ 
  case SetupFireRelationState: { // for UEStudio Folding function
    switch (KeyCode) {
    case ZONE_KEY01: case ZONE_KEY02: case ZONE_KEY03:
    case ZONE_KEY04: case ZONE_KEY05: case ZONE_KEY06:
    case ZONE_KEY07: case ZONE_KEY08: case ZONE_KEY09:
    case ZONE_KEY10:
      InitialSoftTimer(SetupSoftTimer);
      if (IsSetLED(KeyCode+ZONE_ACTIVE_LED)) { 
        ProcState = ResetFireRelationZoneState; 
      } else { 
        ProcState = SetFireRelationZoneState;   
      }
      SetupBuffer[8] = KeyCode;
      break;

    //   case HUMAN_HANDLING_KEY: 
    case LCD4_KEY: 
      InitialSoftTimer(SetupSoftTimer);
      ProcState = ExitSetupFireRelationState;
      LcdScreen(LCD_QUIT_SETUP);
      break;
    case BACKLIGHT_KEY: 
      InitialSoftTimer(SetupSoftTimer);
      LcdScreen(LCD_BACKLIGHT);
      break;
    //   case CONFIRM_KEY: 
    case LCD1_KEY: 
      InitialSoftTimer(SetupSoftTimer);
      ProcState = SaveFireRelationZoneState;
      break;
    default: 
      break;
    }
    }// for UEStudio Folding function
    break;
 
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */
/* ************************************************************************* */ 
  case SetFireRelationZoneState: { // for UEStudio Folding function
    if (SetupFireCount == 0) { 
      KeyIsSet(SetupBuffer[8]+1);
      SetupFireCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      KeyIsSet(SetupBuffer[8]+1);
      SetupFireCount++;
      InitialSoftTimer(SoftTimer1);
    }

    if ((SetupFireCount == 3) ||
        (CommunicationProcState == SetFireRelationZoneState)) {
      SetLED(SetupBuffer[8]+ZONE_ACTIVE_LED);
      if (SetupBuffer[8] < 8) {
        SetupBuffer[2] |= EnCode1[SetupBuffer[8]];
      } else {
        SetupBuffer[3] |= EnCode1[SetupBuffer[8]];
      }
      SetupFireCount = 0;
      ProcState = SetupFireRelationState;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    } else { 
      IsStartSoftTimer1 = ON; 
    }
    } // for UEStudio Folding function
    break;

/* ************************************************************************* */
/* ************************************************************************* */
/* fire alarm zone relation cancel                                           */
/* ************************************************************************* */
/* ************************************************************************* */    
 case ResetFireRelationZoneState: {
 	if (SetupFireCount == 0)
 	{
 		KeyIsReset(SetupBuffer[8]+1);
 		SetupFireCount++;
 		InitialSoftTimer(SoftTimer1);
 		IsStartSoftTimer1 = ON;
    }
    
    if (SoftTimer1.m10sec == 3 )
    {
    	IsStartSoftTimer1 = OFF;
    	KeyIsReset(SetupBuffer[8]+1);
    	SetupFireCount++;
    	InitialSoftTimer(SoftTimer1);
    }
    
    if ((SetupFireCount == 3) ||
    	(CommunicationProcState == ResetFireRelationZoneState))
    {
    	ClrLED(SetupBuffer[8]+ZONE_ACTIVE_LED);
    	
    	if (SetupBuffer[8] < 8)
    	{
    		SetupBuffer[2] &= ~EnCode1[SetupBuffer[8]];
    	}
    	else
    	{
    		SetupBuffer[3] &= ~EnCode1[SetupBuffer[8]];
        }
        SetupFireCount = 0;
        ProcState = SetupFireRelationState;
        IsStartSoftTimer1 = OFF;
        InitialSoftTimer(SoftTimer1);
    }
    else 
    { 
    	IsStartSoftTimer1 = ON;
    }
  }	// end case ResetFireRelationZoneState
  break;
   
/* ************************************************************************* */
/* ************************************************************************* */
/*  Save fire alarm relation setup value                                     */
/* ************************************************************************* */
/* ************************************************************************* */
 case SaveFireRelationZoneState: {
 	if (SetupFireCount == 0)
    {
    	SaveFireRelationSetup();
    	SetupFireCount++;
    	InitialSoftTimer(SoftTimer1);
		// SetFlash(CONFIRM_LED);
		IsStartSoftTimer1 = ON;
		Write_BYTE_EEPROM((CalcEEPROMOffset(FireRelationStorage, SetupBuffer[0],SetupBuffer[1])),
			~SetupBuffer[2]);
		SetupBuffer[9] = TRUE;      
    }
    
    if (CommunicationProcState == SaveFireRelationZoneState)
    {
    	SetupBuffer[9] = FALSE;
    }
    
    if (SoftTimer1.m10sec == 3 )
    {
    	IsStartSoftTimer1 = OFF;
    	
    	if (SetupFireCount < 3 && SetupBuffer[9])
        {
        	SaveFireRelationSetup();
        }
        SetupFireCount++;
        InitialSoftTimer(SoftTimer1);
        IsStartSoftTimer1 = ON;
    }
    
    if (SetupFireCount == 20)
    {
    	Write_BYTE_EEPROM((CalcEEPROMOffset(FireRelationStorage, SetupBuffer[0], SetupBuffer[1])+1),
    		~SetupBuffer[3]);
    	
    	for (SetupFireCount = 0; SetupFireCount < 10; SetupFireCount++)
    	{
    		ClrLED(SetupFireCount+ZONE_ACTIVE_LED);
    	}
    	
    	if (SetupBuffer[0] == 0)
    	{
    		ClrLED(SetupBuffer[1]);
    	}
    	
    	SetupFireCount = 0;
    	//	ClrFlash(CONFIRM_LED);
    	ProcState =InitialSetupFireRelationState;
    	LcdScreen(LCD_SETUP_FIRE);
    	IsStartSoftTimer1 = OFF;
    	InitialSoftTimer(SoftTimer1);
    }
 }	// end case SaveFireRelationZoneState
 break;

 case ExitSetupFireRelationState: {
 	if (SetupFireCount == 0)
    {
    	ExitFireRelationSetup();
    	SetupFireCount++;
    	InitialSoftTimer(SoftTimer1);
    	IsStartSoftTimer1 = ON;
    }
    
    if (SoftTimer1.m10sec == 3 )
    {
    	IsStartSoftTimer1 = OFF;
    	ExitFireRelationSetup();
    	SetupFireCount++;
    	InitialSoftTimer(SoftTimer1);
    }
    
    if ((SetupFireCount == 3) ||
    	(CommunicationProcState == ExitSetupFireRelationState))
    {
    	ClrAllLED();
    	//	SetLED(SETUP_LED);
    	SetupFireCount = 0;
    	ProcState = SetupModeSelectState;
    	IsStartSoftTimer1 = OFF;
    	InitialSoftTimer(SoftTimer1);
    }
    else 
    {
    	IsStartSoftTimer1 = ON;
    }
 }	// end case ExitSetupFireRelationState
 break;
 
#undef SetupFireCount 
/* Setup fire relation state end                                             */


/* ************************************************************************* */
/* ************************************************************************* */
/*   Setup group public address zone State :                                 */
/* ************************************************************************* */
/* ************************************************************************* */
#define SetupGroupCount GenCount2
 case InitialSetupGroupState:
 	#include "./PS/InitSetGrp.c"
 break;
 
 case SetupGroupZoneState:
   switch (KeyCode) {
   case GROUP_KEY01: case GROUP_KEY02: case GROUP_KEY03: case GROUP_KEY04:
   case GROUP_KEY05: case GROUP_KEY06: case GROUP_KEY07: case GROUP_KEY08:
   case GROUP_KEY09: case GROUP_KEY10:
     InitialSoftTimer(SetupSoftTimer);
     SetupBuffer[0] = 0;
     SetupBuffer[1] = KeyCode-30;
     ProcState = SetupGroupAssigenState;
     LcdScreen(LCD_SETUP_GROUP_ZONE);
   break;
 
//   case GROUP_KEY: 
   case LCD4_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     ProcState = ExitSetupGroupState;
     LcdScreen(LCD_QUIT_SETUP);
     break;
   case BACKLIGHT_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     LcdScreen(LCD_BACKLIGHT);
     break;
   }
 break;

 case SetupGroupAssigenState:
   if (SetupGroupCount == 0)
     {
      GroupSetupAssign(SetupBuffer[1]+1);
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
     }
       
   if (SoftTimer1.m10sec == 3 )
     { 
      IsStartSoftTimer1 = OFF; 
      GroupSetupAssign(SetupBuffer[1]+1);
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
     }      
     
   if ((SetupGroupCount == 3) ||
      (CommunicationProcState == SetupGroupAssigenState))
    {
      SetLED(SetupBuffer[1]+GROUP_LED);
      SetupGroupCount = 0;
      ProcState = ReadGroupSetupState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
   else { IsStartSoftTimer1 = ON; }
   
 break;

 case ReadGroupSetupState:
   SetupBuffer[2] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(GroupStorage, SetupBuffer[0], SetupBuffer[1]));
   SetupBuffer[3] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(GroupStorage, SetupBuffer[0], SetupBuffer[1])+1));

   for (SetupGroupCount = 0; SetupGroupCount < 8; SetupGroupCount++)
      {
       if ((SetupBuffer[2] & EnCode1[SetupGroupCount]) >> SetupGroupCount)
         { SetLED(SetupGroupCount+ZONE_ACTIVE_LED); }
      }
   for (SetupGroupCount = 0; SetupGroupCount < 2; SetupGroupCount++)
      {
       if ((SetupBuffer[3] & EnCode1[SetupGroupCount]) >> SetupGroupCount)
         { SetLED(SetupGroupCount+ZONE_ACTIVE_LED+8); }
      }
   SetupGroupCount = 0;
   ProcState = SetupGroupState;
 break;

 case SetupGroupState: 
   switch (KeyCode) {
   case ZONE_KEY01: case ZONE_KEY02: case ZONE_KEY03:
   case ZONE_KEY04: case ZONE_KEY05: case ZONE_KEY06:
   case ZONE_KEY07: case ZONE_KEY08: case ZONE_KEY09:
   case ZONE_KEY10:
     InitialSoftTimer(SetupSoftTimer);
     SetupBuffer[8] = KeyCode;
     if (IsSetLED(SetupBuffer[8]+ZONE_ACTIVE_LED))
       { ProcState = ResetGroupZoneState; }
     else
       { ProcState = SetGroupZoneState;   }
     break;

//   case GROUP_KEY:
   case LCD4_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = ExitSetupGroupState;
     LcdScreen(LCD_QUIT_SETUP);
     break;
//   case ALARM_CANCEL_KEY: break;
//   case CONFIRM_KEY: 
   case LCD1_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     ProcState = SaveGroupZoneState;
     break;
   case BACKLIGHT_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     LcdScreen(LCD_BACKLIGHT);
     break;
   default: break;
   }
 break;
 
 case SetGroupZoneState:

  if (SetupGroupCount == 0)
    { 
      KeyIsSet(SetupBuffer[8]+1);
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      KeyIsSet(SetupBuffer[8]+1);
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
    }

  if ((SetupGroupCount == 3) ||
    (CommunicationProcState == SetGroupZoneState))
   {
    SetLED(SetupBuffer[8]+ZONE_ACTIVE_LED);
    if (SetupBuffer[8] < 8)
      {
        SetupBuffer[2] |= EnCode1[SetupBuffer[8]];
      }
    else
      {
        SetupBuffer[3] |= EnCode1[SetupBuffer[8]];
      }
    SetupGroupCount = 0;
    ProcState = SetupGroupState;
    IsStartSoftTimer1 = OFF; 
    InitialSoftTimer(SoftTimer1);
   }
   else { IsStartSoftTimer1 = ON; }
 break;

 case ResetGroupZoneState:
  if (SetupGroupCount == 0)
    { 
      KeyIsReset(SetupBuffer[8]+1);
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      KeyIsReset(SetupBuffer[8]+1);
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
    }        
    
  if ((SetupGroupCount == 3) ||
      (CommunicationProcState == ResetGroupZoneState))
    {
       ClrLED(SetupBuffer[8]+ZONE_ACTIVE_LED);
    if (SetupBuffer[8] < 8)
      {
        SetupBuffer[2] &= ~EnCode1[SetupBuffer[8]];
      }
    else
      {
        SetupBuffer[3] &= ~EnCode1[SetupBuffer[8]];
      }
       SetupGroupCount = 0;
       ProcState = SetupGroupState;
       IsStartSoftTimer1 = OFF; 
       InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }
 break;
 
 case SaveGroupZoneState:
  if (SetupGroupCount == 0)
    { 
      SaveGroupSetup();
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
//      SetFlash(CONFIRM_LED);
      IsStartSoftTimer1 = ON;
      Write_BYTE_EEPROM(CalcEEPROMOffset(GroupStorage, SetupBuffer[0], SetupBuffer[1]),
	                    ~SetupBuffer[2]);
						
      SetupBuffer[9] = TRUE;
    }

  if (CommunicationProcState == SaveGroupZoneState)
    {
      SetupBuffer[9] = FALSE;
    }
     
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      if (SetupGroupCount < 3 && SetupBuffer[9])
        {
         SaveGroupSetup();
        }
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }        

  
  if (SetupGroupCount == 20)
    {
       Write_BYTE_EEPROM((CalcEEPROMOffset(GroupStorage, SetupBuffer[0], SetupBuffer[1])+1),
	                     ~SetupBuffer[3]);
						 
       for (SetupGroupCount = 0; SetupGroupCount < 10; SetupGroupCount++)
          {       
            ClrLED(SetupGroupCount+ZONE_ACTIVE_LED);
            ClrLED(SetupGroupCount+GROUP_LED);
          }          
       if (SetupBuffer[0] == 0)
         { ClrLED(SetupBuffer[1]); }         
       SetupGroupCount = 0;
  //     ClrFlash(CONFIRM_LED);
       ProcState = InitialSetupGroupState;
       LcdScreen(LCD_GROUP_SETUP);
       IsStartSoftTimer1 = OFF; 
       InitialSoftTimer(SoftTimer1);
    }
 break;

 case ExitSetupGroupState:
  if (SetupGroupCount == 0)
    { 
      ExitGroupSetup();
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }    

  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      ExitGroupSetup();
      SetupGroupCount++;
      InitialSoftTimer(SoftTimer1);
    }        
    
  if ((SetupGroupCount == 3) ||
      (CommunicationProcState == ExitSetupGroupState))
    {
       ClrAllLED();
//       SetLED(SETUP_LED);
       SetupGroupCount = 0;
       ProcState = SetupModeSelectState;
       IsStartSoftTimer1 = OFF; 
       InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }
 break;
#undef SetupGroupCount
/* Setup group zone state end                                                */


/* ************************************************************************* */
/* ************************************************************************* */
/*   Setup background music player zone State :                              */
/* ************************************************************************* */
/* ************************************************************************* */
#define SetupBGMCount GenCount2
 case InitialSetupBGMState:
  if (SetupBGMCount == 0)
    { 
      EntryBackgroundMusicSetup();
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      EntryBackgroundMusicSetup();
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
    }     
  if ((SetupBGMCount == 3) || 
      (CommunicationProcState == InitialSetupGroupState) )
    {
//      SetLED(BACKGROUND_MUSIC_LED);
      SetupBGMCount = 0;
      ProcState = ReadBGMSetupState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
   else { IsStartSoftTimer1 = ON; }
 break;

 case ReadBGMSetupState:
   SetupBuffer[2] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(BackGroundMusicStorage,
                                      SetupBuffer[0], SetupBuffer[1]));
   SetupBuffer[3] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(BackGroundMusicStorage,
                                       SetupBuffer[0], SetupBuffer[1])+1));

   for (SetupBGMCount = 0; SetupBGMCount < 8; SetupBGMCount++)
      {
       if ((SetupBuffer[2] & EnCode1[SetupBGMCount]) == EnCode1[SetupBGMCount])
         { SetLED(SetupBGMCount+ZONE_ACTIVE_LED); }
      }
   for (SetupBGMCount = 0; SetupBGMCount < 2; SetupBGMCount++)
      {
       if ((SetupBuffer[3] & EnCode1[SetupBGMCount]) == EnCode1[SetupBGMCount])
         { SetLED(SetupBGMCount+ZONE_ACTIVE_LED+8); }
      }
   SetupBGMCount = 0;
   ProcState = SetupBGMZoneState;
   
 break;
 
 case SetupBGMZoneState: 
   switch (KeyCode) {
   case ZONE_KEY01: case ZONE_KEY02: case ZONE_KEY03:
   case ZONE_KEY04: case ZONE_KEY05: case ZONE_KEY06:
   case ZONE_KEY07: case ZONE_KEY08: case ZONE_KEY09:
   case ZONE_KEY10:
     InitialSoftTimer(SetupSoftTimer);
     SetupBuffer[8] = KeyCode;
     if (IsSetLED(SetupBuffer[8]+ZONE_ACTIVE_LED))
       { ProcState = ResetBGMZoneState; }
     else
       { ProcState = SetBGMZoneState;   }
     break;

//   case BACKGROUND_MUSIC_KEY: 
   case LCD4_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     ProcState = ExitSetupBGMState;
     LcdScreen(LCD_QUIT_SETUP);
     break;
   case BACKLIGHT_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     LcdScreen(LCD_BACKLIGHT );
     break;
//   case CONFIRM_KEY: 
   case LCD1_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     ProcState = SaveBGMZoneState;
     break;
   default: break;
   }
 break;

 case SetBGMZoneState:
  if (SetupBGMCount == 0)
    { 
      KeyIsSet(SetupBuffer[8]+1);
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      KeyIsSet(SetupBuffer[8]+1);
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
    }

  if ((SetupBGMCount >= 3) ||
    (CommunicationProcState == SetBGMZoneState))
   {
    SetLED(SetupBuffer[8]+ZONE_ACTIVE_LED);
    if (SetupBuffer[8] < 8)
      {
        SetupBuffer[2] |= EnCode1[SetupBuffer[8]];
      }
    else
      {
        SetupBuffer[3] |= EnCode1[SetupBuffer[8]];
      }
    SetupBGMCount = 0;
    ProcState = SetupBGMZoneState;
    IsStartSoftTimer1 = OFF; 
    InitialSoftTimer(SoftTimer1);
   }
   else { IsStartSoftTimer1 = ON; }
 break;

 case ResetBGMZoneState:
  if (SetupBGMCount == 0)
    { 
      KeyIsReset(SetupBuffer[8]+1);
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      KeyIsReset(SetupBuffer[8]+1);
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
    }        
    
  if ((SetupBGMCount >= 3) ||
      (CommunicationProcState == ResetBGMZoneState))
    {
    ClrLED(SetupBuffer[8]+ZONE_ACTIVE_LED);

    if (SetupBuffer[8] < 8)
      {
        SetupBuffer[2] &= ~EnCode1[SetupBuffer[8]];
      }
    else
      {
        SetupBuffer[3] &= ~EnCode1[SetupBuffer[8]];
      }
    SetupBGMCount = 0;
    ProcState = SetupBGMZoneState;
    IsStartSoftTimer1 = OFF; 
    InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }
 break;
 
 case SaveBGMZoneState:
  if (SetupBGMCount == 0)
    { 
      SaveBackgroundMusicSetup();
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
//      SetFlash(CONFIRM_LED);
      IsStartSoftTimer1 = ON;
      Write_BYTE_EEPROM(CalcEEPROMOffset(BackGroundMusicStorage,
                        SetupBuffer[0], SetupBuffer[1]), ~SetupBuffer[2]);
      SetupBuffer[9] = TRUE;
    }

  if (CommunicationProcState == SaveBGMZoneState)
    {
      SetupBuffer[9] = FALSE;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      if (SetupBGMCount < 3 && SetupBuffer[9])
        {
         SaveBackgroundMusicSetup();
        }
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }        

  
  if (SetupBGMCount >= 20)
    {
       Write_BYTE_EEPROM((CalcEEPROMOffset(BackGroundMusicStorage, 
                          SetupBuffer[0], SetupBuffer[1])+1), ~SetupBuffer[3]);
       SetupBGMCount = 0;
       IsStartSoftTimer1 = OFF;
//       ClrFlash(CONFIRM_LED);
       ProcState = ExitSetupBGMState;
       IsStartSoftTimer1 = OFF; 
       InitialSoftTimer(SoftTimer1);
    }
 break;

 case ExitSetupBGMState:
  if (SetupBGMCount == 0)
    { 
      ExitBackgroundMusicSetup();
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      ExitBackgroundMusicSetup();
      SetupBGMCount++;
      InitialSoftTimer(SoftTimer1);
    }        
    
  if ((SetupBGMCount >= 3) ||
      (CommunicationProcState == ExitSetupBGMState))
    {
       ClrAllLED();
//       SetLED(SETUP_LED);
       SetupBGMCount = 0;
       ProcState = SetupModeSelectState;
       LcdScreen(LCD_QUIT_SETUP);
       IsStartSoftTimer1 = OFF; 
       InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }
 break;
#undef SetupBGMCount
/* Setup background music player zone state end                              */


/* ************************************************************************* */
/* ************************************************************************* */
/*   Setup zone digital voice message state :                                */
/* ************************************************************************* */
/* ************************************************************************* */
#define SetupVoiceCount GenCount2
 case InitialSetupVoiceState:
  if (SetupVoiceCount == 0)
    { 
      EntryVoiceMessageSetup();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      EntryVoiceMessageSetup();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
    }     
  if ((SetupVoiceCount >= 3) || 
      (CommunicationProcState == InitialSetupVoiceState) )
    {
//      SetLED(ALARM_CANCEL_LED);         
      SetupVoiceCount = 0;
      ProcState = SetupVoiceZoneState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
   else {  
         if ((COPYRIGTH[0] != 0x50) && (COPYRIGTH[120] != 0x01))
           { return; }                             
           IsStartSoftTimer1 = ON; 
        }
 break;
 
 case SetupVoiceZoneState:
   switch (KeyCode) {
   case ZONE_KEY01: case ZONE_KEY02: case ZONE_KEY03: case ZONE_KEY04:
   case ZONE_KEY05: case ZONE_KEY06: case ZONE_KEY07: case ZONE_KEY08:
   case ZONE_KEY09: case ZONE_KEY10:
     InitialSoftTimer(SetupSoftTimer);
     SetupBuffer[0] = 0;
     SetupBuffer[1] = KeyCode;
     ProcState = SetupVoiceZoneAssigenState;
     LcdScreen(LCD_VOICE_SETUP);
   break;

//   case ALARM_CANCEL_KEY:
   case LCD4_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = ExitVoiceMessageSetupState;
     LcdScreen(LCD_QUIT_SETUP);
     break;
   case BACKLIGHT_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     LcdScreen(LCD_BACKLIGHT);
     break;
   }

   if (CommunicationProcState == SetupVoiceZoneState)
   {
      InitialSoftTimer(SetupSoftTimer);
     ProcState = SetupVoiceMessageState; 
     LcdScreen(LCD_VOICE_SETUP);
   }
 break;

 
 case SetupVoiceZoneAssigenState:
    if (SetupVoiceCount == 0)
     { 
      VoiceMessageSetupZoneAssigen(SetupBuffer[1]+1);
      
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
     }
       
   if (SoftTimer1.m10sec == 3 )
     { 
      IsStartSoftTimer1 = OFF; 
      VoiceMessageSetupZoneAssigen(SetupBuffer[1]+1);
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
     }      
     
   if ((SetupVoiceCount >= 3) ||
      (CommunicationProcState == SetupVoiceZoneAssigenState))
    {
      SetLED(SetupBuffer[1]);
      SetupVoiceCount = 0;
      ProcState = SetupVoiceMessageState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
   else { IsStartSoftTimer1 = ON; }
 break;
 
  case SetupVoiceMessageState:
    if (SetupVoiceCount == 0) {
      SetupVoiceCount++;
    }
    
    switch (KeyCode) {
    case ZONE_KEY01: case ZONE_KEY02: case ZONE_KEY03: case ZONE_KEY04:
    case ZONE_KEY05: case ZONE_KEY06: case ZONE_KEY07: case ZONE_KEY08:
    case ZONE_KEY09: case ZONE_KEY10:
      InitialSoftTimer(SetupSoftTimer);

      if (SetupVoiceCount <= 5) {
        
        if (KeyCode <= ZONE_KEY09) {      
          LcdScreen(LCD_ZONENUMBER0+KeyCode+1);
        } else { 
          LcdScreen(LCD_ZONENUMBER0);
        }
        
        switch(SetupVoiceCount) {
        case 1:
          if (KeyCode == ZONE_KEY10) { 
            KeyCode = TitleName1;
          } else {
            KeyCode += TitleName1;
            KeyCode++;
          }
          break;
        case 2: // Hundred
          SetupBuffer[7] = 0;
          if ( KeyCode == ZONE_KEY01) { 
            KeyCode = SpeechHundred;
            SetupBuffer[7] = 100;
          } else { 
            KeyCode = 0; 
          }
          break;
        case 3: // Ten
          if (KeyCode != ZONE_KEY10) { 
            SetupBuffer[7] += ((KeyCode+1) * 10);        
            KeyCode += SpeechTen;
            if (SetupBuffer[3] == SpeechHundred) { 
              if (KeyCode == SpeechTen) { 
                KeyCode = SpeechTen1; 
              }
            }
          } else { 
            if (SetupBuffer[3] == SpeechHundred) { 
              KeyCode = 1; 
            } else { 
              KeyCode = 0; 
            }
          }
          break;
        case 4: // human
          if (KeyCode != ZONE_KEY10) {
            SetupBuffer[7] += (KeyCode+1); 
          }

          KeyCode += 2;
          
          if (KeyCode == (ZONE_KEY10+2)) { 
            KeyCode = 0;
          }
          break;
        case 5:
          if (KeyCode <= ZONE_KEY09) {
            KeyCode += UnitName1;
          } else { 
            KeyCode = 0;
          }
          break;
        }
        SetupBuffer[SetupVoiceCount+2-1] = KeyCode;    
        SetupVoiceCount++;
        break;    
//      LcdScreen(0x80+SetupVoiceCount-1);
      }
      break;

//   case CONFIRM_KEY:
    case LCD1_KEY:
      InitialSoftTimer(SetupSoftTimer);
      if (SetupVoiceCount == 6) { 
        ProcState = SaveVoiceMessageSetupState;
        SetupVoiceCount = 0;
      }
      break;
//   case ALARM_CANCEL_KEY:
    case LCD4_KEY:
      InitialSoftTimer(SetupSoftTimer);
      ProcState = ExitVoiceMessageSetupState;
      SetupVoiceCount = 0;
      LcdScreen(LCD_QUIT_SETUP );
      break;
    case BACKLIGHT_KEY: 
      InitialSoftTimer(SetupSoftTimer);
      LcdScreen(LCD_BACKLIGHT);
      break;
    }
    break;
 
 case SaveVoiceMessageSetupState:
  if (SetupVoiceCount == 0)
    { 
      SaveVoiceMessageSetup();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
//      SetFlash(CONFIRM_LED);
      IsStartSoftTimer1 = ON;  	   
      Write_BYTE_EEPROM(CalcEEPROMOffset(VoiceMessageStorage, SetupBuffer[0],
                                        SetupBuffer[1]), SetupBuffer[2]);                  
      SetupBuffer[9] = TRUE;
      SelectAudioSource(DIGITAL_AUDIO);
      SetFlash(SOUND_LED);
      if ((SetupBuffer[7] <= 156) && (SetupBuffer[7] >= 131))
        {
          SetupBuffer[3] = 0;
          SetupBuffer[4] = 0;
          SetupBuffer[5] = (SetupBuffer[7] - 131 + SpeechA);
        }
      Speech_Put(SetupBuffer[2]);     
      Speech_Put(SetupBuffer[3]);
      Speech_Put(SetupBuffer[4]);
      Speech_Put(SetupBuffer[5]);
      Speech_Put(SetupBuffer[6]);
      SetupVoiceCount++;
    }

  if (CommunicationProcState == SaveVoiceMessageSetupState)
    {
      SetupBuffer[9] = FALSE;
    }
     
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      if (SetupVoiceCount < 3 && SetupBuffer[9])
        {
         SaveVoiceMessageSetup();
        }
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }        
  
  if (SetupVoiceCount == 4)
    {
     Write_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage, SetupBuffer[0],
                                        SetupBuffer[1])+1), SetupBuffer[3]);
    }

  if (SetupVoiceCount == 6)
    {
     Write_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage, SetupBuffer[0],
                                        SetupBuffer[1])+2), SetupBuffer[4]);
    }

  if (SetupVoiceCount == 8)
    {
     Write_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage, SetupBuffer[0],
                                        SetupBuffer[1])+3), SetupBuffer[5]);
    }
        
  if (SetupVoiceCount == 10)
    {
     Write_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage, SetupBuffer[0],
                                        SetupBuffer[1])+4),SetupBuffer[6]);
    }
    
  if (SetupVoiceCount >= 21)
    {
      if (!IsSpeechPlay())
      {
         SetupVoiceCount = 0;         
         ProcState =InitialSetupVoiceState;
            LcdScreen(LCD_NONAME1);
         ClrFlash(SOUND_LED);
         ClrLED(SOUND_LED);
         if (SetupBuffer[0] == 0)
         { ClrLED(SetupBuffer[1]); }
           
      }
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
   break;
 
 case ExitVoiceMessageSetupState:
   if (SetupVoiceCount == 0)
    { 
      ExitVoiceMessageSetup();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      ExitVoiceMessageSetup();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
    }        
    
  if ((SetupVoiceCount == 3) ||
      (CommunicationProcState == ExitVoiceMessageSetupState))
    {
     if (SetupBuffer[0] == 0)
       { ClrLED(SetupBuffer[1]); }
//       ClrFlash(CONFIRM_LED);
       ClrAllLED();
//       SetLED(SETUP_LED);
       SetupVoiceCount = 0;
       ProcState = SetupModeSelectState;
        IsStartSoftTimer1 = OFF; 
       InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }

 break;
#undef SetupVoiceCount
/* Setup zone digital speech message end                                     */

/* ************************************************************************* */
/* ************************************************************************* */
/*   zone digital voice message test play state :                            */
/* ************************************************************************* */
/* ************************************************************************* */
#define SetupVoiceCount GenCount2
 case InitialVoiceTestPlayState:
  if (SetupVoiceCount == 0)
    { 
      EntryVoiceTestPlay();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(DIGITAL_AUDIO);
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      EntryVoiceTestPlay();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
    }     

  if ((SetupVoiceCount == 3) || 
      (CommunicationProcState == InitialVoiceTestPlayState) )
    {
//      SetLED(TESTPLAY_LED);
	  #ifndef _DISABLE_MAXVOLUME_
      SetEmergencyRelay();
	  #endif
      SetupVoiceCount = 0;
      ProcState = VoiceTestPlayState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
   else { IsStartSoftTimer1 = ON; }

 break;
 
 case VoiceTestPlayState: 
   switch (KeyCode) {
   case ZONE_KEY01: case ZONE_KEY02: case ZONE_KEY03: case ZONE_KEY04:
   case ZONE_KEY05: case ZONE_KEY06: case ZONE_KEY07: case ZONE_KEY08:
   case ZONE_KEY09: case ZONE_KEY10:
     InitialSoftTimer(SetupSoftTimer);
     SetupBuffer[0] = 0;
     SetupBuffer[1] = KeyCode;
     ProcState = VoiceTestPlayZoneAssigenState;
   break;
   case LCD4_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = ExitVoiceTestPlayState;
     LcdScreen(LCD_QUIT_SETUP);
     break;
   case BACKLIGHT_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     LcdScreen(LCD_BACKLIGHT);
     break;
   }
    if (CommunicationProcState == VoiceTestPlayState) { 
      InitialSoftTimer(SetupSoftTimer);
      ProcState = VoiceMessageTestPlayState; 
    }
    break;

 case VoiceTestPlayZoneAssigenState:
     if (SetupVoiceCount == 0)
     { 
//    SetRelay(SetupBuffer[1]);
      VoiceTestPlayZoneAssigen(SetupBuffer[1]+1);
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
     }
       
   if (SoftTimer1.m10sec == 3 )
     { 
      IsStartSoftTimer1 = OFF; 
      VoiceTestPlayZoneAssigen(SetupBuffer[1]+1);
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
     }      
     
   if ((SetupVoiceCount == 3) ||
      (CommunicationProcState == VoiceTestPlayZoneAssigenState))
    {
      SetLED(SetupBuffer[1]+ZONE_ACTIVE_LED);
      SetupVoiceCount = 0;
      ProcState = VoiceMessageTestPlayState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
   else { IsStartSoftTimer1 = ON; }
 break;
 
 case VoiceMessageTestPlayState:

   if (SetupVoiceCount == 0)
     { 
      InitialSoftTimer(SoftTimer1);
//      SetFlash(TESTPLAY_LED);
      SetFlash(SOUND_LED);
      SetupVoiceCount++;
      IsStartSoftTimer1 = ON;
     }
     
   if (SetupVoiceCount == 4)
     {
       Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage
                                   , SetupBuffer[0], SetupBuffer[1])+0)));
       Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage
                                   , SetupBuffer[0], SetupBuffer[1])+1)));
       Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage
                                 , SetupBuffer[0], SetupBuffer[1])+2)));
       Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage
                                , SetupBuffer[0], SetupBuffer[1])+3)));
       Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage
                                , SetupBuffer[0], SetupBuffer[1])+4)));                                                    
       SetupVoiceCount++;
       IsStartSoftTimer1 = OFF;
       InitialSoftTimer(SoftTimer1);
     }
     
   if (IsSpeechFIFOEmpty() && (SetupVoiceCount == 5))
     {
      IsStartSoftTimer1 = ON;
     }

   if (SoftTimer1.m10sec == 3 )
     {
      IsStartSoftTimer1 = OFF; 
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
     }
           
   if (SetupVoiceCount >= 10)
     {
      if (!IsSpeechPlay())
        {
         SetupVoiceCount = 0;
         ProcState = VoiceTestPlayStopState;
        }
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
     }
          
 break;

 case VoiceTestPlayStopState:
     if (SetupVoiceCount == 0)
     { 
      VoiceTestPlayStop();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
     }
       
   if (SoftTimer1.m10sec == 3 )
     { 
      IsStartSoftTimer1 = OFF; 
      VoiceTestPlayStop();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
     }      
     
   if ((SetupVoiceCount == 3) ||
      (CommunicationProcState == VoiceTestPlayStopState))
    {
     if (SetupBuffer[0] == 0)
       {
         ClrLED(SetupBuffer[1]+ZONE_ACTIVE_LED);
  //       ClrRelay(SetupBuffer[1]);
       }

//     ClrFlash(CONFIRM_LED);
     ClrFlash(SOUND_LED);
//     ClrFlash(TESTPLAY_LED);
     SetupVoiceCount = 0;
     ProcState = InitialVoiceTestPlayState;
     IsStartSoftTimer1 = OFF;
     InitialSoftTimer(SoftTimer1);
    }
   else { IsStartSoftTimer1 = ON; }
 break;
  
 case ExitVoiceTestPlayState:
   if (SetupVoiceCount == 0)
     { 
      ExitVoiceTestPlay();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
     }
       
   if (SoftTimer1.m10sec == 3 )
     { 
      IsStartSoftTimer1 = OFF; 
      ExitVoiceTestPlay();
      SetupVoiceCount++;
      InitialSoftTimer(SoftTimer1);
     }      
     
   if ((SetupVoiceCount == 3) ||
      (CommunicationProcState == ExitVoiceTestPlayState))
    {
       ClrAllLED();
//       SetLED(SETUP_LED);
       ClrEmergencyRelay();
       SetupVoiceCount = 0;
       ProcState = SetupModeSelectState;
        IsStartSoftTimer1 = OFF;
       InitialSoftTimer(SoftTimer1);
    }
   else { IsStartSoftTimer1 = ON; }
 break;
#undef SetupVoiceCount
/* zone digital speech message test play end                                 */

/* ************************************************************************* */
/* ************************************************************************* */
/*                 Player background music state                             */
/* ************************************************************************* */
/* ************************************************************************* */
#define BGMPlayCount   GenCount2
  case InitialBGMPlayState:
    if (BGMPlayCount == 0) {
      LcdScreen(LCD_ENTRY_BGM);
      EntryBGMPlay();
      CallStationMPUBGMON();
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO); 
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      SetupBuffer[2] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(BackGroundMusicStorage,
                                         SetupBuffer[0], SetupBuffer[1]));
      SetupBuffer[3] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(BackGroundMusicStorage,
                                          SetupBuffer[0], SetupBuffer[1])+1));

      for (BGMPlayCount = 0; BGMPlayCount < 10; BGMPlayCount++) {
        if (BGMPlayCount < 8) { 
          SetupBuffer[5] = BGMPlayCount; 
        } else { 
          SetupBuffer[5] = BGMPlayCount - 8; 
          if (BGMPlayCount == 8 ) { 
            SetupBuffer[2] = SetupBuffer[3];
          }
        }
             
        SetupBuffer[4] = EnCode1[SetupBuffer[5]];
         
        if ((SetupBuffer[2] & SetupBuffer[4]) == SetupBuffer[4]) { 
          if (!IsLoopShort(BGMPlayCount+ZONE_RELAY)) {
            SetLED(BGMPlayCount+ZONE_ACTIVE_LED);
            SetRelay(BGMPlayCount+ZONE_RELAY);
			// disable the function due to call it in SetRelay(), July 24 2008
            // ClrFlash(BGMPlayCount+ZONE_ACTIVE_LED);
          } else { 
            SetFlash(BGMPlayCount+ZONE_ACTIVE_LED); 
          }
        }
      } /* for (BGMPlay) end */        
      SelectAudioSource(BACKGROUND_AUDIO);      
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      BGMPlayCount = 1;
      Refresh();
      RefreshRelay();      
    } /* if (BGMPlay) end */
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      EntryBGMPlay();
//    CallStationMPUBGMON();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
    if ((BGMPlayCount == 3) || 
        (CommunicationProcState == InitialBGMPlayState)||(CallStationState == CallStationMPUBGMONState)) {
//    SetLED(BACKGROUND_MUSIC_LED);
      IsBGMMode = ON;
      BGMPlayCount = 0;
      SelectAudioSource(BACKGROUND_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ProcState = BGMPlayState;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    }
    else { IsStartSoftTimer1 = ON; }
    
    break;
 
  case BGMPlayState: 
    switch (KeyCode) {
//   case HUMAN_HANDLING_KEY:   /* Push HUNMAN_HANDLING key */
    case LCD3_KEY:   /* Push HUNMAN_HANDLING key */
                              /* then exit background music mode
                                 entry human handling public address */
        //ProcState = ChangeHumanHandlingState;		//del by andy
      ProcState = InitialHumanHandlingState;		//add by andy
      LcdScreen(LCD_ENTRY_MANU);		        //add by andy			
      CallStationState = CallStationChangTalkState;
      break;
//   case BACKGROUND_MUSIC_KEY: /* Push BGM Key */
    case LCD4_KEY: /* Push BGM Key */
    case RESET_ALARM_KEY:
        /* Exit background music mode */
      ProcState = ExitBGMPlayState;
      break;
//   case GROUP_KEY:   /* Push GROUP key */
    case LCD2_KEY:   /* Push GROUP key */
                     /* then exit background music mode
                        entry group public address */
      ProcState = ChangeGroupState;
      CallStationState = CallStationChangTalkState;
      break;
    case ALL_ALARM_KEY: 
      ProcState = InitialAllZoneEmergencyState;
      break;
    case BACKLIGHT_KEY: 
      LcdScreen(LCD_BACKLIGHT);
      break;
    default: break;
    }
    
    switch(CallStationState) {
    case CallStationActiveState:
      ProcState = InitialCallStationModeState;
      LcdScreen(LCD_ENTRY_CS);
      break;
    case CallStationChimeUpState:
      Chime = Chime1;
      break;
    case CallStationChimeDownState:
      Chime = Chime2;
      break;
    case CallStationZoneState:
      ProcState = InitialCallStationZoneState;
      LcdScreen(LCD_ENTRY_CS);
      break;
    case CallStationBGMOFFState:
    case CallStationNormalState:
      ProcState = ExitBGMPlayState;
      break;
    }
    GenCount2 = 0;
    break;
 
 case ChangeHumanHandlingState:
  if (BGMPlayCount == 0)
    { 
      ExitBGMPlay();
      CallStationMPUBGMOFF();
      CallStationMPUTalk();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrAllRelay();
//      DisCallStation();
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      ExitBGMPlay();
//		CallStationMPUBGMOFF();
//		CallStationMPUTalk();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
  if ((BGMPlayCount == 3)||
      (CommunicationProcState == ChangeHumanHandlingState)||
      (CallStationState == CallStationChangTalkState)||
      (CallStationState == CallStationMPUBGMONState))
    {
      ClrAllLED();
      BGMPlayCount = 0;
      ProcState = InitialHumanHandlingState;
      LcdScreen(LCD_ENTRY_MANU);
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; } 
 break;
 
 case ChangeGroupState:
  if (BGMPlayCount == 0)
    { 
      ExitBGMPlay();
      CallStationMPUBGMOFF();
      CallStationMPUTalk();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrAllRelay();
//      DisCallStation();
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      ExitBGMPlay();
//    CallStationMPUBGMOFF();
//    CallStationMPUTalk();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
  if ((BGMPlayCount == 3)||
      (CommunicationProcState == ChangeGroupState)||(CallStationState == CallStationMPUBGMOFFState))
    {
      ClrAllLED();
      BGMPlayCount = 0;
      ProcState = InitialGroupState;
      LcdScreen(LCD_ENTRY_GROUP);
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }
 break;
 
 case ChangeCallStationModeState:
  if (BGMPlayCount == 0)
    { 
      ExitBGMPlay();
	  CallStationBGMON();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrAllRelay();
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      ExitBGMPlay();
//		CallStationBGMON();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
    }

  if ((BGMPlayCount == 3)||
      (CommunicationProcState == ChangeCallStationModeState)||
      (CallStationState == CallStationBGMONState))
    {
      ClrAllLED();
      BGMPlayCount = 0;
      ProcState = InitialCallStationModeState;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }
 break;

  case ExitBGMPlayState:
    if (BGMPlayCount == 0) { 
      ExitBGMPlay();
      CallStationMPUBGMOFF();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrAllRelay();
    }
       
    if (SoftTimer1.m10sec == 3 )  { 
      IsStartSoftTimer1 = OFF; 
      ExitBGMPlay();
//		CallStationMPUBGMOFF();
      BGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
    }

    if ((BGMPlayCount == 3)||
        (CommunicationProcState == ExitBGMPlayState)||
        (CallStationState == CallStationBGMOFFState)||
        (CallStationState == CallStationChangTalkState)) {
      ClrAllLED();
      BGMPlayCount = 0;
      IsBGMMode = OFF;
      ProcState = EntryNormalState;
      GenCount2 = 0;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    } else { IsStartSoftTimer1 = ON; }
    break;
#undef BGMPlayCount
/* Background playing state end                                              */

/* ************************************************************************* */
/* ************************************************************************* */
/*                 Callstation Player background music state                 */
/* ************************************************************************* */
/* ************************************************************************* */


#define CallstationBGMPlayCount   GenCount2

 case InitialCallStationBGMPlayState:
  if (CallstationBGMPlayCount == 0)
    { 
      EntryBGMPlay();
//    CallStationMPUBGMON();
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO); 
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      SetupBuffer[2] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(BackGroundMusicStorage,
                                         SetupBuffer[0], SetupBuffer[1]));
      SetupBuffer[3] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(BackGroundMusicStorage,
                                         SetupBuffer[0], SetupBuffer[1])+1));

      for (CallstationBGMPlayCount = 0; CallstationBGMPlayCount < 10; CallstationBGMPlayCount++)
         {
           if (CallstationBGMPlayCount < 8) 
             { SetupBuffer[5] = CallstationBGMPlayCount; }
           else
             { 
               SetupBuffer[5] = CallstationBGMPlayCount - 8; 
               if (CallstationBGMPlayCount == 8 ) 
                 { SetupBuffer[2] = SetupBuffer[3]; }
             }
             
           SetupBuffer[4] = EnCode1[SetupBuffer[5]];
         
           if ((SetupBuffer[2] & SetupBuffer[4]) == SetupBuffer[4])
             { 
               if (!IsLoopShort(CallstationBGMPlayCount+ZONE_RELAY))               
                 {
                  SetLED(CallstationBGMPlayCount+ZONE_ACTIVE_LED);
                  SetRelay(CallstationBGMPlayCount+ZONE_RELAY);
				  // disable the function, due to call it in the SetRelay() July 24 2008
                  // ClrFlash(CallstationBGMPlayCount+ZONE_ACTIVE_LED);
                 }
               else
                 { SetFlash(CallstationBGMPlayCount+ZONE_ACTIVE_LED); }                
             }
         } /* for (BGMPlay) end */
        
      SelectAudioSource(CALLSTATION_AUDIO);      
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      CallstationBGMPlayCount = 1;
    } // if (BGMPlay) end 
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      EntryBGMPlay();
//		CallStationMPUBGMON();
      CallstationBGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
 if ((CallstationBGMPlayCount == 3) || 
     (CommunicationProcState == InitialBGMPlayState))
   {
//     SetLED(BACKGROUND_MUSIC_LED);
     //IsBGMMode = ON;			//delled by andy
     CallstationBGMPlayCount = 0;
     SelectAudioSource(CALLSTATION_AUDIO);
     ClrFlash(SOUND_LED);
     ClrLED(SOUND_LED);
     ProcState = CallStationBGMPlayState;
     IsStartSoftTimer1 = OFF;
     InitialSoftTimer(SoftTimer1);
   }
 else { IsStartSoftTimer1 = ON; }
    
 break;
 
 case CallStationBGMPlayState: 
   switch (KeyCode) {
   case LCD1_KEY:
     CallstationBGMPlayCount=0;
     ProcState=InitialBGMPlayState;
     break;
   case LCD2_KEY: // Push GROUP_KEY 
     CallstationBGMPlayCount= 0;
     ProcState = ChangeGroupState;
     LcdScreen(LCD_ENTRY_GROUP);
     ClrAllLED();
     ClrAllRelay();
     CallStationMPUTalk();
     break;

   case LCD3_KEY: // Push HUMAN_HANDLING_KEY 
     CallstationBGMPlayCount= 0;
     ProcState = ChangeHumanHandlingState;
     LcdScreen(LCD_ENTRY_MANU);
     ClrAllLED();
     ClrAllRelay();
     CallStationMPUTalk();
     //DisCallStation();
     break;   

   case ALL_ALARM_KEY: 
     ProcState = InitialAllZoneEmergencyState;
     GenCount2 = 0;
     break;
   case BACKLIGHT_KEY: 
     LcdScreen(LCD_BACKLIGHT);
     break;
   default: break;
   }
   if (CallStationState == CallStationActiveState)
     {
       ProcState = InitialCallStationModeState;
       LcdScreen(LCD_ENTRY_CS);
     }
   if (CallStationState == CallStationChimeUpState)
     {
       Chime = Chime1;
     }
   if (CallStationState == CallStationChimeDownState)
     {
       Chime = Chime2;
     }
   if (CallStationState == CallStationZoneState)
     {
       ExitGroup();		//by andy
       //CallStationMPUTalkOk();  //by andy
       ClrAllLED();		//by andy
       ClrAllRelay();		//by andy
       ProcState = InitialCallStationZoneState;
       LcdScreen(LCD_ENTRY_CS);
     }
   if (CallStationState == CallStationBGMOFFState) {
     ProcState = ExitCallStationBGMPlayState;
     LcdScreen(LCD_NORMAL);
   }   
 break;
 
 case ExitCallStationBGMPlayState:
  if (CallstationBGMPlayCount == 0)
    {       
      ExitBGMPlay();
//CallStationMPUBGMOFF();
      CallstationBGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrAllRelay();
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      ExitBGMPlay();
//CallStationMPUBGMOFF();
      CallstationBGMPlayCount++;
      InitialSoftTimer(SoftTimer1);
    }

  if ((CallstationBGMPlayCount == 3)||
      (CommunicationProcState == ExitBGMPlayState))
      {
      ClrAllLED();
      CallstationBGMPlayCount = 0;
      IsBGMMode = OFF;
      ProcState = EntryNormalState;
      GenCount2 = 0;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }
  
 break;
 
#undef CallstationBGMPlayCount
/* Callstation Background playing state end                                              */

/* ************************************************************************* */
/* ************************************************************************* */
/*                   Group public address  state                             */
/* ************************************************************************* */
/* ************************************************************************* */
#define GroupCount GenCount2
 case InitialGroupState:
 if (GroupCount == 0)
    { 
      EntryGroup();
      CallStationMPUTalk();
      GroupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
//      DisCallStation();
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF;
      EntryGroup(); 
      CallStationMPUTalk();
      GroupCount++;
      InitialSoftTimer(SoftTimer1);
    }

  if ((GroupCount >= 3) ||
      (CommunicationProcState == InitialGroupState)||
      (CallStationState == CallStationChangTalkState)||
      (CallStationState == CallStationMPUTalkState))
    {
//      SetLED(GROUP_LED);
	  #ifndef _DISABLE_MAXVOLUME_
      SetEmergencyRelay();
	  #endif
      SelectAudioSource(MIC_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);

      for (GroupCount = 0 ; GroupCount < 10 ; GroupCount++)
         {
          SetupBuffer[GroupCount] = 0;
         }
         
      GroupCount = 0;
      ProcState = SelectGroupState;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    }
  else { IsStartSoftTimer1 = ON; }
    
 break; 

 case SelectGroupState:
   if (IsMicPhonePress()) {
     ProcState = GroupActionState;
     LcdScreen(LCD_MIC);
   }

   switch (KeyCode) {
   case GROUP_KEY01: case GROUP_KEY02: case GROUP_KEY03:
   case GROUP_KEY04: case GROUP_KEY05: case GROUP_KEY06:
   case GROUP_KEY07: case GROUP_KEY08: case GROUP_KEY09:
   case GROUP_KEY10:
     SetupBuffer[8] = KeyCode - GROUP_KEY;

     if (IsSetLED(SetupBuffer[8]+GROUP_LED))
       { ProcState = ResetGroupState; }
     else
       { ProcState = SetGroupState;   }
     break;
//   case GROUP_KEY:   /* Push GROUP key */
   case LCD4_KEY:   /* Push GROUP key */
   case RESET_ALARM_KEY:
                     /* then exit group mode */
        ProcState = ExitGroupState;
        break;
   case ALL_ALARM_KEY: 
     ProcState = InitialAllZoneEmergencyState;
     GenCount2 = 0;
     break;
   case BACKLIGHT_KEY: 
        LcdScreen(LCD_BACKLIGHT);
        break;
   default: break;
  }
  
 break;
 
 case SetGroupState:
  if (GroupCount == 0)
    { 
      KeyIsSet(SetupBuffer[8]+1);
      GroupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      KeyIsSet(SetupBuffer[8]+1);
      GroupCount++;
      InitialSoftTimer(SoftTimer1);
    }

  if ((GroupCount >= 3) ||
    (CommunicationProcState == SetGroupState))
   {
    SetLED(SetupBuffer[8]+GROUP_LED);
    if (SetupBuffer[8] < 8)
      {
        SetupBuffer[0] |= EnCode1[SetupBuffer[8]];
      }
    else
      {
        SetupBuffer[1] |= EnCode1[SetupBuffer[8]];
      }
    GroupCount = 0;
    ProcState = SelectGroupState;
    IsStartSoftTimer1 = OFF; 
    InitialSoftTimer(SoftTimer1);
   }
   else { IsStartSoftTimer1 = ON; }
 break;
 
 case ResetGroupState:
  if (GroupCount == 0)
    { 
      KeyIsReset(SetupBuffer[8]+1);
      GroupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
  if (SoftTimer1.m10sec == 3 )
    { 
      IsStartSoftTimer1 = OFF; 
      KeyIsReset(SetupBuffer[8]+1);
      GroupCount++;
      InitialSoftTimer(SoftTimer1);
    }

  if ((GroupCount >= 3) ||
    (CommunicationProcState == ResetGroupState))
   {
    ClrLED(SetupBuffer[8]+GROUP_LED);
    if (SetupBuffer[8] < 8)
      {
        SetupBuffer[0] &= ~EnCode1[SetupBuffer[8]];
      }
    else
      {
        SetupBuffer[1] &= ~EnCode1[SetupBuffer[8]];
      }
    GroupCount = 0;
    ProcState = SelectGroupState;
    IsStartSoftTimer1 = OFF; 
    InitialSoftTimer(SoftTimer1);
   }
   else { IsStartSoftTimer1 = ON; }      
 break;

 case GroupState:

   if (!IsMicPhonePress()) {
      ProcState = ExitGroupState;
   }
     
   if (KeyCode == ALL_ALARM_KEY) {
     ProcState = InitialAllZoneEmergencyState;
     GenCount2 = 0; 
   }
     
 break;

#define TempBuffer  SetupBuffer
#define TempCount1  GenCount3
#define TempCount2  GenCount4

 case GroupActionState:

   if (GroupCount == 0)
   { 
      GroupActive(SetupBuffer[1], SetupBuffer[0]);
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      
      for (GroupCount = 0; GroupCount < 10; GroupCount++)
      {
           ClrLED(GroupCount+ZONE_ACTIVE_LED);
      }
      
      GroupCount = 1;
    }
       
  if (SoftTimer1.m10sec == 3 )
  { 
    IsStartSoftTimer1 = OFF; 
    GroupActive(SetupBuffer[1], SetupBuffer[0]);
    GroupCount++;
    InitialSoftTimer(SoftTimer1);
  }     

  if ((GroupCount >= 3) || 
      (CommunicationProcState == GroupActionState))
  {
    for (TempCount1 = 0; TempCount1 < 10; TempCount1++)
    {           
      if (TempCount1 == 8)
      {
        TempBuffer[0] = TempBuffer[1];
      }
    
      if ((TempBuffer[0] & EnCode1[TempCount1]) == EnCode1[TempCount1])
      { 
            
        TempBuffer[2] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(GroupStorage, 0, TempCount1));
        TempBuffer[3] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(GroupStorage, 0, TempCount1)+1));
            
        for (TempCount2 = 0; TempCount2 < 8; TempCount2++)
        {
          if ((TempBuffer[2] & EnCode1[TempCount2]) == EnCode1[TempCount2])
          { 
            SetLED(TempCount2+ZONE_ACTIVE_LED); 
            SetRelay(TempCount2+ZONE_RELAY);
          }
        }
         
        for (TempCount2 = 0; TempCount2 < 2; TempCount2++)
        {
          if ((TempBuffer[3] & EnCode1[TempCount2]) == EnCode1[TempCount2])
          { 
            SetLED(TempCount2+ZONE_ACTIVE_LED+8); 
            SetRelay(TempCount2+ZONE_RELAY+8); 
          }
        }
      } /* if (Tempbuffer[2]) end */
    } /* for (TempCount1) end */

    GroupCount = 0;
    TempCount1 = 0;
    TempCount2 = 0;
    SelectAudioSource(MIC_AUDIO);
    ClrFlash(SOUND_LED);
    ClrLED(SOUND_LED);
    ProcState = GroupState;
    IsStartSoftTimer1 = OFF; 
    InitialSoftTimer(SoftTimer1);
  }  /* if (GroupCount) end*/
  else { IsStartSoftTimer1 = ON; }
  break;

#undef TempBuffer
#undef TempCount1
#undef TempCount2

  case ExitGroupState:
    if (GroupCount == 0) { 
      ExitGroup();
      CallStationMPUTalkOk();
      GroupCount++;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrAllRelay();
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF;
      ExitGroup();
// CallStationMPUTalkOk();
      GroupCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
    if (( GroupCount == 3 ) ||
        (CommunicationProcState == ExitGroupState)||
        (CallStationState == CallStationMPUTalkState)||
        (CallStationState == CallStationBGMONState))  {
      ClrAllLED();
	  #ifndef _DISABLE_MAXVOLUME_
	  	ClrEmergencyRelay();
	  #endif
      GroupCount = 0;
      if (IsBGMMode) { 
        ProcState = InitialBGMPlayState;
      } else { 
        ProcState = EntryNormalState;
      }
//    ResetCallStationBus();
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    } else { IsStartSoftTimer1 = ON; }   
    break;
/* Group public address state                                                */

/* ************************************************************************* */
/* ************************************************************************* */
/*               Human handling  public address  state                       */
/* ************************************************************************* */
/* ************************************************************************* */
#define HumanHandlingCount GenCount2
  case InitialHumanHandlingState:
    if (HumanHandlingCount == 0) { 
      EntryHumanHandling();
      CallStationMPUTalk();
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      ClrAllLED();	//by andy
      ClrAllRelay();
    }

    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF;
      // EntryHumanHandling();
      // CallStationMPUTalk();
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
    if (( HumanHandlingCount >= 3 )) {
//        ( CommunicationProcState == InitialHumanHandlingState)) ||
//        (CallStationState == CallStationMPUTalkState)) {
  	  #ifndef _DISABLE_MAXVOLUME_
      SetEmergencyRelay();
	  #endif
      SelectAudioSource(MIC_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ProcState = SelectHumanHandlingState;
      for (HumanHandlingCount = 0; HumanHandlingCount < 10; HumanHandlingCount++) {
        SetupBuffer[HumanHandlingCount] = 0;
      }
      HumanHandlingCount = 0;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    } else { 
      IsStartSoftTimer1 = ON;
    }
    break;
 
  case SelectHumanHandlingState:
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (IsAllCall == 1) {	//All Call
      BYTE nCount;
      for (nCount = 0;nCount < 10; nCount++) {
        SetRelay(nCount);
        SetLED(nCount+ZONE_ACTIVE_LED);
      } 
 //-----------------------------Delay 100 msec-------------------------------------------------------------------------------------
      for (Delay1=0;Delay1<20;Delay1++)	{
        for (Delay2=0;Delay2<1000;Delay2++) {
          WatchDog = ~WatchDog;
        }
      }
 //-----------------------------Delay 100 msec-------------------------------------------------------------------------------------
      // EntryBGMPlay();		//通知副機All Active_Led ON !!!
      AllCallZone();	
      IsAllCall = 0;
    }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    if (IsMicPhonePress()) {
      ProcState = HumanHandlingActionState;
      LcdScreen(LCD_MANU_MIC);
    }
    
    switch (KeyCode) {
    case LCD4_KEY:
    case RESET_ALARM_KEY:
      ProcState = ExitHumanHandlingState;
      GenCount2 = 0;
      break;
    case ALL_ALARM_KEY: 
      ProcState = InitialAllZoneEmergencyState;
      GenCount2 = 0;
      break;
    case BACKLIGHT_KEY: 
      LcdScreen(LCD_BACKLIGHT);
      break;
    case ALL_ZONE_KEY:
      IsAllCall = 1;
      break;
    default: 
      if ((KeyCode <= ZONE_KEY10)) {
        SetupBuffer[8] = KeyCode;
         if (IsSetLED(KeyCode+ZONE_ACTIVE_LED)) { 
           ProcState = ResetHumanHandlingZoneState;  
         } else { 
           ProcState = SetHumanHandlingZoneState;  }
         } /* if (KeyCode) end */
      break;
    }
    break;
 
  case SetHumanHandlingZoneState:
    if (HumanHandlingCount == 0) { 
      KeyIsSet(SetupBuffer[8]+1);
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      KeyIsSet(SetupBuffer[8]+1);
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
    }

    if ((HumanHandlingCount >= 3) ||
        (CommunicationProcState == SetHumanHandlingZoneState)) {
      SetRelay(SetupBuffer[8]);
      SetLED(SetupBuffer[8]+ZONE_ACTIVE_LED);
      HumanHandlingCount = 0;
      ProcState = SelectHumanHandlingState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    } else { 
      IsStartSoftTimer1 = ON; 
    }
    break;
 
  case ResetHumanHandlingZoneState:
    if (HumanHandlingCount == 0) { 
      KeyIsReset(SetupBuffer[8]+1);
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      KeyIsReset(SetupBuffer[8]+1);
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
    }

    if ((HumanHandlingCount >= 3) ||
       (CommunicationProcState == SetHumanHandlingZoneState)) {
      ClrRelay(SetupBuffer[8]);
      ClrLED(SetupBuffer[8]+ZONE_ACTIVE_LED);
      HumanHandlingCount = 0;
      ProcState = SelectHumanHandlingState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    } else { 
      IsStartSoftTimer1 = ON; 
    }
    break;
 
  case HumanHandlingActionState:
    if (HumanHandlingCount == 0) { 
      HumanHandlingCount++;
      MicPhonePress();
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    }

    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      MicPhonePress();
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
    }

    if ((HumanHandlingCount >= 3) ||
        (CommunicationProcState == HumanHandlingActionState)) {
      HumanHandlingCount = 0;
      ProcState = HumanHandlingState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    } else { 
      IsStartSoftTimer1 = ON; 
    }
    break;
 
  case HumanHandlingState:
    if (!IsMicPhonePress()) {
      ProcState = ExitHumanHandlingState;
    }
    if (KeyCode == ALL_ALARM_KEY) {
      ProcState = InitialAllZoneEmergencyState;
      GenCount2 = 0;
    }
    break;

  case ExitHumanHandlingState:
    if (HumanHandlingCount == 0) { 
      ExitHumanHandling();
      CallStationMPUTalkOk();
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      ClrAllRelay();
    }
       
  if (SoftTimer1.m10sec == 3)
    { 
      IsStartSoftTimer1 = OFF;
      ExitHumanHandling();
//    CallStationMPUTalkOk();
      HumanHandlingCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
  if ((HumanHandlingCount == 3) ||
      (CommunicationProcState == ExitHumanHandlingState)||(CallStationState == CallStationNormalState))
    {
     ClrAllLED();
	 #ifndef _DISABLE_MAXVOLUME_
	 	ClrEmergencyRelay();
	 #endif
     HumanHandlingCount = 0;
     SelectAudioSource(ALLCLOSE_AUDIO);
     ClrFlash(SOUND_LED);
     ClrLED(SOUND_LED);
     if (IsBGMMode) { 
       ProcState = InitialBGMPlayState;
     } else { 
       ProcState = EntryNormalState;
     }
//     ResetCallStationBus();
     IsStartSoftTimer1 = OFF;
     InitialSoftTimer(SoftTimer1);
    }
   else { IsStartSoftTimer1 = ON; }
   break;
 
 
/* ************************************************************************* */
/* ************************************************************************* */
/*         CallStation Zone Select  state       		             */
/* ************************************************************************* */
/* ************************************************************************* */

#define CallstationZoneCount GenCount2
 
 case InitialCallStationZoneState:
      CallstationZoneCount  = 0 ;
      ClrAllRelay();
      ClrAllLED();		//by andy
      //SetEmergencyRelay();	//del by andy
      //SelectAudioSource(CALLSTATION_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ProcState = CallStationSelectZoneState;		
      /*for (CallstationZoneCount = 0; CallstationZoneCount < 10; CallstationZoneCount++)
         {
           SetupBuffer[HumanHandlingCount] = 0;
         }*/
//---------------------- by andy --------------------
      //PHYSICAL_RED_LED=~CallStationBuffer[2];	//test CallStationBuffer[1]=??
      //SetLED(CallStationBuffer[2]+ZONE_FIRE_LED);
      
      if (CallStationBuffer[1]< 1 )
         {
         SetLED(CallStationBuffer[2]+ZONE_ACTIVE_LED);
    	 SetRelay(CallStationBuffer[2]); 
         }
      else
        {
        CallStationSetZone(CallStationBuffer[1],CallStationBuffer[2]);
        ProcState = CallStationSelectZoneState;	          //add by andy
        }
//--------------------------------------------------- 
    
 break;
 
  case CallStationSelectZoneState:
    switch (KeyCode) {
    case LCD2_KEY:
      ProcState=ChangeGroupState;
      LcdScreen(LCD_ENTRY_GROUP);
      CallstationZoneCount=0;
      ClrAllLED();
      ClrAllRelay();
      CallStationMPUTalk();
      break;
    case LCD3_KEY:
      ProcState=ChangeHumanHandlingState;
      LcdScreen(LCD_ENTRY_MANU);
      SelectAudioSource(ALLCLOSE_AUDIO);
      CallstationZoneCount=0;
      ClrAllLED();
      ClrAllRelay();
      CallStationMPUTalk();
      break;       
    case ALL_ALARM_KEY: 
    /* All zone emergency public address */
      ProcState = InitialAllZoneEmergencyState;
      GenCount2 = 0;
      break;
    case BACKLIGHT_KEY: 
      LcdScreen(LCD_BACKLIGHT);
      break;
    }             
    switch(CallStationState) {
    case CallStationZoneTalkState:
      ProcState = CallStationZoneActionState;		
      break;
    case CallStationZoneState:
      CallStationState = CallStationZoneState;
      ProcState = SetCallStationSelectZoneState;
      break;
    case CallStationNormalState:
      ProcState = ExitCallStationZoneState;
      break;
    }

    CallstationZoneCount++;	//by andy
    if (!IsSpeechPlay() && CallstationZoneCount < 50) {
      CallstationZoneCount++;
      if (CallstationZoneCount > 48) { 
        SelectAudioSource(CALLSTATION_AUDIO);//by andy
        ClrFlash(SOUND_LED);
        ClrLED(SOUND_LED);
      }
    }    
    break;
    
  case SetCallStationSelectZoneState:
      
  //if (CallstationZoneCount  == 0)
    // {
      SetupBuffer[8] = CallStationBuffer[1];	       
      SetupBuffer[9] = CallStationBuffer[2];  
/********** by andy **********/      
/* callstation送完zone_select protocol後,再送一個[0x15,0x00,0x02,0x00,0x11]
   的protocol給主機,方便主機GEC8500判斷音源開啟時機 */
  
     if(SetupBuffer[9]==0x11)
        {
           SelectAudioSource(DIGITAL_AUDIO);
           SetFlash(SOUND_LED);
           Speech_Put(Chime);	
        }     
/******************************/
      if (SetupBuffer[8]==0 )
          {
  	   SetRelay(SetupBuffer[9]); 
           SetLED(SetupBuffer[9]+ZONE_ACTIVE_LED);        
           //KeyIsSet(SetupBuffer[9]+1);	
           //CallstationZoneCount++;
           //InitialSoftTimer(SoftTimer1);
           //IsStartSoftTimer1 = ON;
           ProcState = CallStationSelectZoneState;  //add by andy
          }
      
 /*        if (SoftTimer1.m10sec == 3 )
            { 
             IsStartSoftTimer1 = OFF; 
             KeyIsSet(SetupBuffer[9]+1);
             CallstationZoneCount++;
             InitialSoftTimer(SoftTimer1);
             }					*/
/*
  if ((CallstationZoneCount >= 3) ||
      (CommunicationProcState == SetCallStationSelectZoneState))
   {
    //SetRelay(SetupBuffer[9]);
    //SetLED(SetupBuffer[9]+ZONE_ACTIVE_LED);
    //LcdScreen(0x19);
    //CallstationZoneCount = 0;
    ProcState = CallStationSelectZoneState;
    IsStartSoftTimer1 = OFF; 
    InitialSoftTimer(SoftTimer1);
   }						
   else { IsStartSoftTimer1 = ON; }	*/
   
    // }
    else
        {
         //CallstationZoneCount++;
         //InitialSoftTimer(SoftTimer1);
        // IsStartSoftTimer1 = ON;
         CallStationSetZone(CallStationBuffer[1],CallStationBuffer[2]);
         ProcState = CallStationSelectZoneState;	//add by andy
         //if (SoftTimer1.m10sec == 3 )
           // { 
            // IsStartSoftTimer1 = OFF; 
       //    CallStationSetZone(CallStationBuffer[1],CallStationBuffer[2]); //***
            // CallstationZoneCount++;
             //InitialSoftTimer(SoftTimer1);
           //  }

     /*     if ((CallstationZoneCount  == 3) ||
             (CommunicationProcState == SetCallStationSelectZoneState))
             {
              CallstationZoneCount = 0;
              ProcState = CallStationSelectZoneState;
              IsStartSoftTimer1 = OFF; 
              InitialSoftTimer(SoftTimer1);
              }
              else { IsStartSoftTimer1 = ON; }      */
      }
      break;
 
// case ResetCallStationSelectZoneState:
// break;
  case CallStationZoneActionState:
 
    if (CallStationState == CallStationNormalState) {
      ProcState = ExitCallStationZoneState;
    }
     
    SelectAudioSource(DIGITAL_AUDIO);
    SetFlash(SOUND_LED);
    Speech_Put(Chime);
    CallstationZoneCount++;
    InitialSoftTimer(SoftTimer1);
    IsStartSoftTimer1 = ON;

    if (!IsSpeechPlay() && CallstationZoneCount < 50) {
      CallstationZoneCount++;
      if (CallstationZoneCount > 48) { 
        SelectAudioSource(CALLSTATION_AUDIO);//by andy
        ClrFlash(SOUND_LED);
        ClrLED(SOUND_LED);
      }
    }
/*
 if (!IsMicPhonePress())
    {
      ProcState = ExitCallStationZoneState;
    }
*/
    switch (KeyCode) {
    case ALL_ALARM_KEY: 
    /* All zone emergency public address */
      ProcState = InitialAllZoneEmergencyState;
      GenCount2 = 0;
      break;
    case LCD4_KEY: 
    case RESET_ALARM_KEY:
      ProcState = ExitCallStationZoneState;
      GenCount2 = 0;
      break;   

    case BACKLIGHT_KEY: 
      LcdScreen(LCD_BACKLIGHT);
      break;
    }              

  break;

 case ExitCallStationZoneState:
     ExitHumanHandling();		//added by andy
     CallStationMPUTalkOk();		//added by andy	
     ClrAllRelay();
     ClrAllLED();
     ClrEmergencyRelay();
     SelectAudioSource(ALLCLOSE_AUDIO);
     ClrFlash(SOUND_LED);
     ClrLED(SOUND_LED);

     if (IsBGMMode) { 
       ProcState = InitialBGMPlayState;
       CallStationState=CallStationBGMONState;
     } else { 
       ProcState = EntryNormalState;
     }
     GenCount2 = 0; 
 break;

#define CallStationCount GenCount2
#define TempCount1       GenCount1
#define TempCount2       GenCount3
#define TempBuffer       SetupBuffer

 case InitialCallStationModeState:
  if (CallStationCount == 0)
    { 
      ClrAllLED();
      ClrAllRelay();
      CallStationActive(CallStationBuffer[2], CallStationBuffer[1]);
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClearSpeechFIFO();
      while (IsSpeechBusy());
      SpeechStop();
      
      
    if ((CallStationBuffer[1] == 0xff) &&(CallStationBuffer[2] == 0xff))		//All Call
      {
        for (TempCount1 = 0; TempCount1 < 10; TempCount1++)
           {
            if (!IsLoopShort(TempCount1+ZONE_RELAY))
              {
               SetLED(TempCount1+ZONE_ACTIVE_LED);
               SetRelay(TempCount1+ZONE_RELAY);
			   // disalbe the function, due to call it in the SetRelay() July 24 2008
               // ClrFlash(TempCount1+ZONE_RELAY);
              }
            else
              { SetFlash(TempCount1+ZONE_RELAY); }
           }
      }
    else																																								//Group
      {
      TempBuffer[2] = CallStationBuffer[1];
      for (TempCount1 = 0; TempCount1 < 10; TempCount1++)
         {           
         if (TempCount1 == 8)
           {
            TempBuffer[2] = CallStationBuffer[2];
           }
    
         if ((TempBuffer[2] & EnCode1[TempCount1]) == EnCode1[TempCount1])
           { 
             
            TempBuffer[0] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(GroupStorage, 0, TempCount1));
            TempBuffer[1] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(GroupStorage, 0, TempCount1)+1));

             SetLED(TempCount1+GROUP_LED); 		// Set Group LED
            
            for (TempCount2 = 0; TempCount2 < 8; TempCount2++)
               {
               if ((TempBuffer[0] & EnCode1[TempCount2]) == EnCode1[TempCount2])
                 { 
                  SetLED(TempCount2+ZONE_ACTIVE_LED); 
                  SetRelay(TempCount2+ZONE_RELAY);
                 }
               }
         
            for (TempCount2 = 0; TempCount2 < 2; TempCount2++)
               {
               if ((TempBuffer[1] & EnCode1[TempCount2]) == EnCode1[TempCount2])
                 { 
                  SetLED(TempCount2+ZONE_ACTIVE_LED+8); 
                  SetRelay(TempCount2+ZONE_RELAY+8); 
                 }
               }
		     } // if (Tempbuffer[2]) end 
	      } // for (TempCount1) end 
      } // else end 

      SelectAudioSource(DIGITAL_AUDIO);
      SetFlash(SOUND_LED);
      Speech_Put(Chime);
      CallStationCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
  } // if (CallStationCount) end 
       
  if (SoftTimer1.m10sec == 4 )
    { 
      IsStartSoftTimer1 = OFF; 
      CallStationActive(CallStationBuffer[2], CallStationBuffer[1]);
      CallStationCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
  if ((CallStationCount == 3) || 
      (CommunicationProcState == InitialCallStationModeState))
    {
         IsStartSoftTimer1 = OFF;
         CallStationCount = 0;
         TempCount1 = 0;
         TempCount2 = 0;
         ProcState = CallStationModeState;
       }
  else { IsStartSoftTimer1 = ON; }
   
  break;

  case CallStationModeState:
    if (!IsSpeechPlay() && CallStationCount < 50){
      CallStationCount++;
      if (CallStationCount > 48) {
        SelectAudioSource(CALLSTATION_AUDIO); 
	ClrFlash(SOUND_LED);
   	ClrLED(SOUND_LED);
      }
    }
    
    switch (CallStationState) {
    case CallStationActiveState:
      ProcState = InitialCallStationModeState;
      CallStationCount = 0;
      break;
    case CallStationNormalState:
      ProcState = ExitCallStationModeState;
      CallStationCount = 0;
      break;
    }

    switch (KeyCode) {   
    case LCD3_KEY: //Push HUMAN_HANDLING_KEY 
      //Entry numan handling public address mode 
      ProcState = ChangeHumanHandlingState;
      LcdScreen(LCD_ENTRY_MANU);
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrAllLED();
      ClrAllRelay();
      CallStationMPUTalk();
//    DisCallStation();
      CallStationCount = 0;
      break;   
    case LCD2_KEY: // Push GROUP_KEY 
      // Entry group public address state 
      ProcState = ChangeGroupState;
      LcdScreen(LCD_ENTRY_GROUP);
      CallStationCount = 0;
      ClrAllLED();
      ClrAllRelay();
      CallStationMPUTalk();
//    DisCallStation();
      break;
    case ALL_ALARM_KEY: 
      // All zone emergency public address 
      ProcState = InitialAllZoneEmergencyState;
      GenCount2 = 0;
      CallStationCount = 0;
      break;
    case BACKLIGHT_KEY: 
      CallStationCount = 0;
      LcdScreen(LCD_BACKLIGHT);
      break;
    }
    break;
 
  case ExitCallStationModeState:
    if (CallStationCount == 0) { 
      ExitGroup();
      CallStationMPUTalkOk();
      CallStationCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);			//by andy
      ClrLED(SOUND_LED);			//by andy
      SpeechPlay(0);
      ClrAllRelay();
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF;
      ExitGroup();
//    CallStationMPUTalkOk();
      CallStationCount++;
      InitialSoftTimer(SoftTimer1);
  
      if ( CallStationCount == 3 ) {
        ClrAllLED();
        CallStationCount = 0;
        if (IsBGMMode) { 
          ProcState = InitialBGMPlayState;
       	  CallStationState = CallStationBGMONState;
        } else { 
          ProcState = EntryNormalState;
     	}
        }
      else { IsStartSoftTimer1 = ON; }
    } 
    break;

#undef CallStationCount
#undef TempCount1
#undef TempCount2
#undef TempBuffer
 
 case InitialPreFireEmergencyState:
// DisCallStation();
// CallStationFire();
// CallStationProcState = CallStationFirePlayState;
#define NowUnit		   SetupBuffer[0]	
    if (GenCount2 == 0) {
      ClrAllLED();
      ClrAllRelay();
      SPECIAL_CMD(SPC_ENTRY_FIRE, FireZoneBuffer[0], FireZoneBuffer[1]);
      LcdScreen(LCD_BL_ON);
      LCDStatus();
      SelectAudioSource(DIGITAL_AUDIO);
      SetFlash(ALL_ALARM_LED);
      SetFlash(FIRE_LED);
      SetFlash(NO_FIRE_LED);
      SetLED(NO_FIRE_LED);
	  SetEmergencyRelay();
      SetFireStatusRelay();
      IsPreEmergencyMode = TRUE;
      GenCount2 = 1;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      WatchDog = ~WatchDog;  /* Reset hardware watch dog timer */
      SetFireRelationZone();
      LcdScreen(LCD_PRE_ALARM);
    } else if (SoftTimer1.m10sec == 3 ) { 
      SPECIAL_CMD(SPC_ENTRY_FIRE, FireZoneBuffer[0], FireZoneBuffer[1]);
      IsStartSoftTimer1 = OFF; 
      GenCount2++;
      InitialSoftTimer(SoftTimer1);
    }
    
    if (( GenCount2 == 3 )) {
     // ||(CommunicationProcState == InitialEntrySetupState )||(CallStationState == CallStationMPUTalkState)) 
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
      NowUnit=0;		
      GenCount2=0;				
      SetFlash(SOUND_LED);
      ClearSpeechFIFO();
      while(IsSpeechBusy());
      LcdScreen(LCD_BL_AUTO_OFF);
      SpeechStop();
      InitialSoftTimer(SoftTimer2);
      IsStartSoftTimer2 = ON;
      ProcState = PreFireEmergencyState;
      StorageStatus = PreFireEmergencyStorageStatus;
      PreDelay = 0;
    } else { 
      IsStartSoftTimer1 = ON; 
    }
    break;

 break;
 
  case PreFireEmergencyState:
    switch(KeyCode) {
    case NO_FIRE_KEY:
      ProcState = InitialConfirmNoFireEmergencyState;
      break;
//  case ALARM_CANCEL_KEY:				//by andy
//  case RESET_ALARM_KEY:
       //OnlyEmergencyUseTempState = ProcState;
      //GenCount2 = 0;
       //ProcState = InitialCheckIsAlarmHoldState;
       //IsStartSoftTimer2 = OFF;       
       //InitialSoftTimer(SoftTimer2);
       //break;
//  case CONFIRM_KEY:
    case FIRE_KEY:
      ProcState = InitialConfirmFireEmergencyState;
      break;
    case ALL_ALARM_KEY: 
      ProcState = InitialAllZoneEmergencyState;
      break;
    case BACKLIGHT_KEY: 
      LcdScreen(LCD_BACKLIGHT);
      break;
    default:
      if (KeyCode <= ZONE_KEY10) {
        BYTE i;  
        i = (KeyCode >= ZONE_KEY09) ? 1 : 0;
        
        if ((FireRelationMask[i] & EnCode1[KeyCode]) == 0) {  
          EMG_KEYSET[0] = KeyCode;
          if (!IsSetLED(KeyCode+ZONE_ACTIVE_LED)) {
            SetLED(KeyCode+ZONE_ACTIVE_LED);
            SetRelay(KeyCode);
            EMG_KEYSET[1] = 0xFF;
          } else {
            ClrLED(KeyCode+ZONE_ACTIVE_LED);
            ClrRelay(KeyCode);
            EMG_KEYSET[1] = 0x00;
          }
        }
        OnlyEmergencyUseTempState = ProcState;
        ProcState = ZoneSelectProc;        
      } /* if (KeyCode) end */      
      break;   
    } /* KeyCode switch - case end */ 
    GenCount2 = 0;

    if (IsMicPhonePress())  { 
      SelectAudioSource(MIC_AUDIO);
      //ClrFlash(SOUND_LED);
      //ClrLED(SOUND_LED);
    } else  {
      SelectAudioSource(DIGITAL_AUDIO);
      //SetFlash(SOUND_LED);
    }

    if (PreDelay ==0) {
 //-----------------------------Delay 700 msec-------------------------------------------------------------------------------------
      for (Delay1=0;Delay1<7;Delay1++) {
        for (Delay2=0;Delay2<20000;Delay2++) {
	WatchDog = ~WatchDog;  
        }  
      }
 //-----------------------------Delay 700 msec-------------------------------------------------------------------------------------
      PreDelay = 1;
    }
//----------------------------------------- by andy ------------------------   
    if (IsSpeechFIFOEmpty() && PreDelay == 1) {
      Speech_Put(Alarm1);
      Speech_Put(Alarm1);
      Speech_Put(Alarm1);
      Speech_Put(Care_Care);
#define NowProcZone SetupBuffer[1]
#define Temp        SetupBuffer[2]
#define ZoneCount   SetupBuffer[3]
#define NowUnit     SetupBuffer[0]

      for (NowUnit = 0;NowUnit < MaxExtendUnit;NowUnit++) {
        Temp = FireZoneBuffer[(NowUnit*2)+1];

        for (ZoneCount = 0; ZoneCount < 10; ZoneCount++)  {
          if (ZoneCount == 8) {
            Temp = FireZoneBuffer[(NowUnit*2)];
          }
            
          if (Temp != 0) {
            if ((Temp & EnCode1[ZoneCount]) == EnCode1[ZoneCount])  {
        
              Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                                           NowUnit, ZoneCount)+0)));		
              Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                                           NowUnit, ZoneCount)+1)));
              Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                                           NowUnit, ZoneCount)+2)));       
              Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                                           NowUnit, ZoneCount)+3)));
              Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                                           NowUnit, ZoneCount)+4)));

            } /* if (Temp) end */
          } /* if (Temp) end */
        } /* for (ZoneCount) end */
      }//for(NowUnit)end
#undef NowUnit
#undef NowProcZone
#undef Temp
#undef ZoneCount
   	  
      Speech_Put(SensorAction);
      Speech_Put(Alarm1);
      Speech_Put(Alarm1);
      Speech_Put(Alarm1);
      Speech_Put(EngCare_Care);
      Speech_Put(EngSensorAction);      
    } //end if(IsSpeechFIFOEmpty()&&PreDelay==1)

    switch (GetPreAlarmTimerSet()) {
    case   0:
      if (SoftTimer2.sec == 30) {
        goto ConfirmDelayTimer;
      }
     break;
    case   1:
      if (SoftTimer2.sec == 40) {
        goto ConfirmDelayTimer;
      }      
      break;
    case   2:
      if (SoftTimer2.sec == 50) {
        goto ConfirmDelayTimer;
      }
      break;      
    case   3:
      if (SoftTimer2.min == 1) {
        goto ConfirmDelayTimer;
      }
      break;
    case   4:
      if ((SoftTimer2.min == 1) && (SoftTimer2.sec == 10))  {
        goto ConfirmDelayTimer;
      }
      break;
    case   5:
      if ((SoftTimer2.min == 1) && (SoftTimer2.sec == 20))  {
        goto ConfirmDelayTimer;
      }
      break;
    case   6:
      if ((SoftTimer2.min == 1) && (SoftTimer2.sec == 30))  {
        goto ConfirmDelayTimer;
      }
      break;
    case   7:
      if ((SoftTimer2.min == 1) && (SoftTimer2.sec == 40))  {
        goto ConfirmDelayTimer;
      }
      break;
    case   8:
      if ((SoftTimer2.min == 1) && (SoftTimer2.sec == 50))  {
        goto ConfirmDelayTimer;
      }
      break;
    case   9:
      if (SoftTimer2.min == 2) {
        goto ConfirmDelayTimer;
      }
      break;
    case  10:
      if ((SoftTimer2.min == 2) && (SoftTimer2.sec == 30))  {
        goto ConfirmDelayTimer;
      }
      break;
    case  11:
      if (SoftTimer2.min == 3)  {
        goto ConfirmDelayTimer;
      }
      break;
    case  12:
      if ((SoftTimer2.min == 3) && (SoftTimer2.sec == 30))  {
        goto ConfirmDelayTimer;
      }
      break;
    case  13:
      if (SoftTimer2.min == 4) {
        goto ConfirmDelayTimer;
      }
      break;
    case  14:
      if ((SoftTimer2.min == 4) && (SoftTimer2.sec == 30))  {
        goto ConfirmDelayTimer;
      }
      break;
    case  15:
      if (SoftTimer2.min == 5) {
        goto ConfirmDelayTimer;
      }
      break;

    default:
      if (SoftTimer2.min == 2) {
        goto ConfirmDelayTimer;
      }
      break;
    } /* GetPreAlarmTimerSet() switch - case end */
    break;
 
ConfirmDelayTimer:
    GenCount2 = 0;
    ProcState = InitialConfirmFireEmergencyState;
    break;
 
  case ExitPreFireEmergencyState:
	if (IsBGMMode)
	{
		ProcState = InitialBGMPlayState;
	}
	else
	{
		ProcState = EntryNormalState;
	}  
    GenCount2 = 0;
    break;

 case InitialConfirmNoFireEmergencyState:
    if (GenCount2 == 0) {
      LcdScreen(LCD_NO_FIRE);
      SPECIAL_CMD(SPC_NOT_FIRE, FireZoneBuffer[0], FireZoneBuffer[1]);
      IsStartSoftTimer2 = OFF;
      InitialSoftTimer(SoftTimer2);
      IsPreEmergencyMode = TRUE;
      IsNoFireMode       = TRUE;
      SelectAudioSource(ALLCLOSE_AUDIO);

      ClrFireStatusRelay();
      IsNoFireMode = TRUE;
      
      ClrFlash(ALL_ALARM_LED);
      // ClrFlash(SOUND_LED);
      ClrFlash(FIRE_LED);
      ClrFlash(NO_FIRE_LED);
      ClrLED(FIRE_LED);
      ClrLED(SOUND_LED);
      SetLED(NO_FIRE_LED);
      
      GenCount2 = 1;
      InitialSoftTimer(SoftTimer1);
    } else if (SoftTimer1.m10sec == 3 ) { 
      SPECIAL_CMD(SPC_NOT_FIRE, FireZoneBuffer[0], FireZoneBuffer[1]);
      GenCount2++;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
    
    if (( GenCount2 == 3 )) {
     // ||(CommunicationProcState == InitialEntrySetupState )||(CallStationState == CallStationMPUTalkState)) 
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
      GenCount2=0;
      ClearSpeechFIFO();
      while(IsSpeechBusy());
      SpeechStop();
      ProcState = ConfirmNoFireEmergencyState;
      StorageStatus = FireEmergencyStorageStatus;
      LcdScreen(LCD_BL_AUTO_OFF);
    } else { 
      IsStartSoftTimer1 = ON; 
    }
    break;   
 break;

  case ConfirmNoFireEmergencyState:
    switch(KeyCode) {
    case RESET_ALARM_KEY:
    case LCD4_KEY:
      IsNoFireCancelMode = TRUE;
      OnlyEmergencyUseTempState = ConfirmNoFireEmergencyState;
      ProcState = InitialCheckIsAlarmHoldState;
      break;
    case ALL_ALARM_KEY:
      ProcState = InitialAllZoneEmergencyState;
      break;  
    case BACKLIGHT_KEY: 
      LcdScreen(LCD_BACKLIGHT);
      break;
    default:
      if (KeyCode <= ZONE_KEY10) {
        BYTE i;  
        i = (KeyCode >= ZONE_KEY09) ? 1 : 0;
        
        if ((FireRelationMask[i] & EnCode1[KeyCode]) == 0) {  
          EMG_KEYSET[0] = KeyCode;
          if (!IsSetLED(KeyCode+ZONE_ACTIVE_LED)) {
            SetLED(KeyCode+ZONE_ACTIVE_LED);
            SetRelay(KeyCode);
            EMG_KEYSET[1] = 0xFF;
          } else {
            ClrLED(KeyCode+ZONE_ACTIVE_LED);
            ClrRelay(KeyCode);
            EMG_KEYSET[1] = 0x00;
          }
        }
        OnlyEmergencyUseTempState = ProcState;
        ProcState = ZoneSelectProc;          
      }  /* if (KeyCode) end */
      break;   
    } /* KeyCode end */
    GenCount2 = 0;
   
 if (IsMicPhonePress())  { 
   SelectAudioSource(MIC_AUDIO);
   //ClrFlash(SOUND_LED);
   //ClrLED(SOUND_LED);
 } else  {
   SelectAudioSource(DIGITAL_AUDIO);
   //SetFlash(SOUND_LED);
 }
 
 if (!IsNoFireCancelMode) {
   if (IsSpeechFIFOEmpty()) {
     ClearSpeechFIFO();
     Speech_Put(Alarm2);
     Speech_Put(Alarm2);
     Speech_Put(Alarm2);
     Speech_Put(Care_Care);
     Speech_Put(NoFireAction);
     Speech_Put(Alarm2);
     Speech_Put(Alarm2);
     Speech_Put(Alarm2);
     Speech_Put(EngCare_Care);
     Speech_Put(EngNoFireAction);
   }
 }
 break;

#define CheckCount GenCount2 
  	// Clean all alram state
  case InitialCheckIsAlarmHoldState:
    LcdScreen(LCD_CHK_ALARM);				//by andy

    if (CheckCount == 0) {
      // send CheckIsAlarm command to each extend unit
      CheckIsAlarm();
      
      //CallStationNoFire();
      ClrAllLED();
      ClrAllRelay();
      SelectAudioSource(ALLCLOSE_AUDIO);      
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClearSpeechFIFO();
      while(IsSpeechBusy());
      SpeechStop();

      for (CheckCount = 0; CheckCount < (MaxExtendUnit+1); CheckCount++) {
        FireZoneBuffer[CheckCount*2] = 0;
        FireZoneBuffer[(CheckCount*2)+1] = 0;
      }
	  FireZoneCount = 0;
	  
      FireRelationMask[0] = 0;
      FireRelationMask[1] = 0;
      ClrFireSensorState();

      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      CheckCount = 1;
    }
       
    if (SoftTimer1.m10sec == 3 )  { 
      IsStartSoftTimer1 = OFF; 
      // send CheckIsAlarm command to each extend unit
      CheckIsAlarm();
      //  CallStationNoFire();
      CheckCount++;
      InitialSoftTimer(SoftTimer1);
    }
 
    if (( CheckCount == 3 ) || 
        (CommunicationProcState == InitialCheckIsAlarmHoldState)) {
      CheckCount = 0;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
      ProcState = CheckIsAlarmHoldState;
    } else { 
      IsStartSoftTimer1 = ON; 
    }
 
 break;
 
 case CheckIsAlarmHoldState:
 
   if (CheckCount == 0)  {                        
     InitialSoftTimer(SoftTimer1);
     IsStartSoftTimer1 = ON;
     CheckCount++;
   }	/* if (CheckCount) */
   
   if (SoftTimer1.m10sec == 3 ) { 
     IsStartSoftTimer1 = OFF; 
     CheckCount++;
     InitialSoftTimer(SoftTimer1);
   }

// Still Fire
   if (((CommunicationProcState == ExtendUnitFireActiveState) ||
       (CommunicationProcState == EmergencyTelState)         ||
        IsAnyFireAction()) && CheckCount > 10 ) {

     //LcdScreen(LCD_FIRE);
     //SelectAudioSource(DIGITAL_AUDIO);
     //SetFlash(SOUND_LED);
     
     if (OnlyEmergencyUseTempState == AllZoneEmergencyState) {
       ProcState =InitialConfirmFireEmergencyState;  //by andy
     } else  {
       ProcState = OnlyEmergencyUseTempState;
     }


      switch (ProcState) {
      case PreFireEmergencyState:        
        //SetFlash(ALL_ALARM_LED);
		ProcState = InitialPreFireEmergencyState;
        break;
        
      case ConfirmFireEmergencyState:
        //SetLED(ALL_ALARM_LED);
		//SetLED(FIRE_LED);
		ProcState = InitialConfirmFireEmergencyState;
        break;
        
      case ConfirmNoFireEmergencyState:
	  	ProcState = InitialConfirmNoFireEmergencyState;
	  	IsNoFireCancelMode = FALSE;
        break;
      default: break;
      }
      
      //Fire is still on
      // send to extend unit
      FireAlarmAction(FireZoneBuffer[0], FireZoneBuffer[1]);

	  CheckCount = 0;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
	  
	  // check fire alarm, function in sensor.c
	  
      if (IsAnyFireAction()) { 
        IsFireActiveFlag = TRUE;
		SFireCount = 0;
      }
   }
     
   if (CheckCount  >= 100)  { // *30ms
     ClearSpeechFIFO();
     while(IsSpeechBusy());
     SpeechStop();
     CheckCount = 0;
	
	if (IsBGMMode)
	{
		ProcState = InitialBGMPlayState;
	}
	else
	{
		ProcState = EntryNormalState;
	}		
     GenCount2 = 0;
     ExitAllZoneEmergency();		//Exit
     CallStationState = CallStationNormalState;
     ClrFlash(ALL_ALARM_LED);
     ClrFlash(FIRE_LED);
     ClrFlash(NO_FIRE_LED);
     ClrAllLED();
     ClrFireStatusRelay();
     ClrAllRelay();
     CallStationNoFire();
     CallStationNoFire();
     CallStationNoFire();
 //  ResetCallStationBus();
     ClrEmergencyRelay();
     IsStartSoftTimer1 = OFF;
     InitialSoftTimer(SoftTimer1);
     //IsBGMMode          = OFF;
     IsEmergencyMode    = FALSE;
     IsPreEmergencyMode = FALSE;
     IsNoFireMode       = FALSE;
     IsNoFireCancelMode = FALSE;
     FireZoneCount = 0;
     StorageStatus = NormalStorageStatus;
   } else  { /* if (CheckCount) end */
       IsStartSoftTimer1 = ON; 
   }

 break;
#undef CheckCount
  
  case InitialConfirmFireEmergencyState:
    if (GenCount2 == 0) {
      LcdScreen(LCD_FIRE);

      SPECIAL_CMD(SPC_CONFIRM_FIRE, FireZoneBuffer[0], FireZoneBuffer[1]);

      IsStartSoftTimer1      = OFF;
      IsStartSoftTimer2      = OFF;
      InitialSoftTimer(SoftTimer2);
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
	  SetEmergencyRelay();
      SetFireStatusRelay();

      IsPreEmergencyMode = TRUE;
      IsEmergencyMode    = TRUE;

      ClrFlash(ALL_ALARM_LED);
      ClrFlash(FIRE_LED);
      ClrFlash(NO_FIRE_LED);
      //ClrFlash(SOUND_LED);

      ClrLED(SOUND_LED);
      ClrLED(NO_FIRE_LED);

      SetLED(FIRE_LED);
      SetLED(ALL_ALARM_LED);

      SelectAudioSource(ALLCLOSE_AUDIO);

      ClearSpeechFIFO();
      while(IsSpeechBusy());
      SpeechStop();

      SetFireRelationZone();
      GenCount2 = 1;
    } else if (SoftTimer1.m10sec == 3 ) { 
      SPECIAL_CMD(SPC_CONFIRM_FIRE, FireZoneBuffer[0], FireZoneBuffer[1]);
      IsStartSoftTimer1 = OFF; 
      GenCount2++;
      InitialSoftTimer(SoftTimer1);
    }
    
    if (( GenCount2 == 3 )) {
     // ||(CommunicationProcState == InitialEntrySetupState )||(CallStationState == CallStationMPUTalkState)) 
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
      // LcdScreen(LCD_BL_AUTO_OFF);
      SetupBuffer[0] = 0; // NowUnit
      GenCount2 = 0;
      ProcState = ConfirmFireEmergencyState;
      PreDelay = 0;
    } else { 
      IsStartSoftTimer1 = ON; 
    }
    break;
 
  case ConfirmFireEmergencyState:
    switch(KeyCode) {
    case RESET_ALARM_KEY:
    case LCD4_KEY:
      OnlyEmergencyUseTempState = ProcState;
      ProcState = InitialCheckIsAlarmHoldState;
      break;
    case ALL_ALARM_KEY:
      ProcState = InitialAllZoneEmergencyState;
      break;  
    case BACKLIGHT_KEY: 
      LcdScreen(LCD_BACKLIGHT);
      break;
    default:
      if (KeyCode <= ZONE_KEY10) {
        BYTE i;  
        i = (KeyCode >= ZONE_KEY09) ? 1 : 0;
        
        if ((FireRelationMask[i] & EnCode1[KeyCode]) == 0) {  
          EMG_KEYSET[0] = KeyCode;
          if (!IsSetLED(KeyCode+ZONE_ACTIVE_LED)) {
            SetLED(KeyCode+ZONE_ACTIVE_LED);
            SetRelay(KeyCode);
            EMG_KEYSET[1] = 0xFF;
          } else {
            ClrLED(KeyCode+ZONE_ACTIVE_LED);
            ClrRelay(KeyCode);
            EMG_KEYSET[1] = 0x00;
          }
        }
        OnlyEmergencyUseTempState = ProcState;
        ProcState = ZoneSelectProc;            
      }  /* if (KeyCode) end */
   break;   
 } /* KeyCode end */
 GenCount2 = 0;
 
 /* Is user micphone press talk button */ 
    if (IsMicPhonePress()) { 
      SelectAudioSource(MIC_AUDIO);
      //ClrFlash(SOUND_LED);
      //ClrLED(SOUND_LED);
    } else {
      SelectAudioSource(DIGITAL_AUDIO);
      //SetFlash(SOUND_LED);
    }
   
 /* Is speech FIFO empty */
 //**********************************add by andy********************
    if (IsSpeechFIFOEmpty()) {
      Speech_Put(Alarm3);
      Speech_Put(Alarm3);
      Speech_Put(Alarm3);
      Speech_Put(Fire_Fire);

#define NowProcZone SetupBuffer[1]
#define Temp        SetupBuffer[2]
#define ZoneCount   SetupBuffer[3]
#define NowUnit     SetupBuffer[0]

   for (NowUnit = 0;NowUnit < MaxExtendUnit;NowUnit++)
   {
      Temp = FireZoneBuffer[(NowUnit*2)+1];

       for (ZoneCount = 0; ZoneCount < 10; ZoneCount++)
       {
         if (ZoneCount == 8)
         {
           Temp = FireZoneBuffer[(NowUnit*2)];
         }

         if (Temp != 0)
         {
           if ((Temp & EnCode1[ZoneCount]) == EnCode1[ZoneCount])
           {   
             Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                        NowUnit, ZoneCount)+0)));              			
             Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                        NowUnit, ZoneCount)+1)));
             Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                        NowUnit, ZoneCount)+2)));
             Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                        NowUnit, ZoneCount)+3)));
             Speech_Put(Read_BYTE_EEPROM((CalcEEPROMOffset(VoiceMessageStorage,
                        NowUnit, ZoneCount)+4)));
           } /* if (Temp) end */
         } /* if (Temp) end */
       } /* for (ZoneCount) end */
   }//for(NowUnit)end
#undef NowUnit
#undef NowProcZone
#undef Temp
#undef ZoneCount

      Speech_Put(Fire_Action);
      Speech_Put(EmergencyExit);
      Speech_Put(Alarm3);
      Speech_Put(Alarm3);
      Speech_Put(Alarm3);
      Speech_Put(EngFire_Fire);
      Speech_Put(EngFire_Action);
      Speech_Put(EngEmergencyExit);
    } /* end if(IsSpeechFIFOEmpty*/
    break;   
       
  case EntryFireAlarmConfirmState:  
    ProcState = EntryNormalState;
    GenCount2 = 0;
    break;
 
 #define AllZoneCount  GenCount2
  case InitialAllZoneEmergencyState:
    if (AllZoneCount == 0) { 
      LcdScreen(LCD_ALLEMG);
      EntryAllZoneEmergency();
      CallStationFire();
      SetEmergencyRelay();
      SetFireStatusRelay();
      AllZoneCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;

      IsAllZoneEmergencyMode = TRUE;
      IsPreEmergencyMode = TRUE;
      IsEmergencyMode    = TRUE;

      ClrAllLED();
      ClrAllRelay();
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClearSpeechFIFO();
      // correctly LED status.      
      SetLED(ALL_ALARM_LED);
      ClrFlash(ALL_ALARM_LED);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrFlash(FIRE_LED);
      ClrFlash(NO_FIRE_LED);
      ClrLED(NO_FIRE_LED);
//    DisCallStation();
//	CallStationProcState = CallStationFirePlayState;
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF;
      EntryAllZoneEmergency();
//    CallStationFire();
      AllZoneCount++;
      InitialSoftTimer(SoftTimer1);
    }
      
    if (( AllZoneCount == 3 ) ||
        (CommunicationProcState == InitialAllZoneEmergencyState)||
        (CallStationState == CallStationFirePlayState)) {
      SetLED(ALL_ALARM_LED);
      for (AllZoneCount = 0; AllZoneCount < 10; AllZoneCount++) { 
        SetLED(AllZoneCount+ZONE_ACTIVE_LED);
        if (!IsLoopShort(AllZoneCount+ZONE_RELAY)) { 
          SetLED(AllZoneCount+ZONE_FIRE_LED);
          SetRelay(AllZoneCount+ZONE_RELAY);
		  // disable the function due to call it in the SetRelay() at July 24 2008
          // ClrFlash(AllZoneCount+ZONE_ACTIVE_LED);
        } else {
          SetLED(AllZoneCount+ZONE_FIRE_LED);		
          SetFlash(AllZoneCount+ZONE_ACTIVE_LED);
        }                
      }
      while(IsSpeechBusy());
      SpeechStop();
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      SetEmergencyRelay();
      SetFireStatusRelay();
	  SelectAudioSource(DIGITAL_AUDIO);
      SetFlash(SOUND_LED);
      AllZoneCount = 0;
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
      ProcState = AllZoneEmergencyState;
      CallStationProcState = CallStationFirePlayState;
      LcdScreen(LCD_BL_AUTO_OFF);
      StorageStatus = AllZoneEmergencyStorageStatus;
    }
   else { IsStartSoftTimer1 = ON; }
   break;
 
  case AllZoneEmergencyState:
    if (KeyCode == RESET_ALARM_KEY) { 
      OnlyEmergencyUseTempState = ProcState;
      AllZoneCount = 0;
      ProcState = InitialCheckIsAlarmHoldState;
    }
    
    if (IsMicPhonePress()) { 
      SelectAudioSource(MIC_AUDIO);
      //ClrFlash(SOUND_LED);
      //ClrLED(SOUND_LED);
    } else {
      SelectAudioSource(DIGITAL_AUDIO);
      //SetFlash(SOUND_LED);
    }
    
    if (IsSpeechFIFOEmpty()) {
      Speech_Put(Alarm3);
      Speech_Put(Alarm3);
      Speech_Put(Alarm3);
      Speech_Put(Fire_Fire);
      Speech_Put(EmergencyExit);
      Speech_Put(Alarm3);
      Speech_Put(Alarm3);
      Speech_Put(Alarm3);
      Speech_Put(EngFire_Fire);
      Speech_Put(EngEmergencyExit);
    } 
    break;
 
  case ExitAllZoneEmergencyState:

    if (AllZoneCount == 0) { 
      ExitAllZoneEmergency();
      CallStationNoFire();
      AllZoneCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrAllRelay();
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF;
      ExitAllZoneEmergency();
//		CallStationNoFire();
      AllZoneCount++;
      InitialSoftTimer(SoftTimer1);
    }

    if ((AllZoneCount == 3 ) ||
        (CommunicationProcState == ExitAllZoneEmergencyState )||
        (CallStationState == CallStationNormalState)) {
      ClrAllLED();
      ClrFlash(ALL_ALARM_LED);
      ClrFlash(FIRE_LED);
      ClrFlash(NO_FIRE_LED);
      AllZoneCount = 0;
      IsAllZoneEmergencyMode = FALSE;
      ClrFireSensorState();

      if (!IsEmergencyMode) {
		if (IsBGMMode)
		{
			ProcState = InitialBGMPlayState;
		}
		else
		{
			ProcState = EntryNormalState;
		}
        GenCount2 = 0;
      } else {
        OnlyEmergencyUseTempState = ConfirmFireEmergencyState;
        ProcState = CheckIsAlarmHoldState;
      }

      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);
    } else { IsStartSoftTimer1 = ON; } 
    break;

 #undef AllZoneCount

#define ExGroupCount  GenCount2
#define TempCount1    GenCount3
#define TempCount2    GenCount4
#define TempBuffer    SetupBuffer
 // @@@@@@@@@@@@@@@@@@@@@
 // new added for extra group
  case EntryExGroup:
   
    if ( 0 == ExGroupCount) {
      LcdScreen(LCD_EXGROUP);
      EntryETGroup(PreTelStatus[0], PreTelStatus[1]);
      ExGroupCount++;
      InitialSoftTimer(SoftTimer1);   
      // get bit of 1 ~ 8 group state
      TempBuffer[0] = PreTelStatus[0];
      for (TempCount1 = 0; TempCount1 < 10; TempCount1++) {           
        if (TempCount1 == 8) {
          // get bit of 8 ~ 10 group state
          TempBuffer[0] = PreTelStatus[1];
        }
    
        if ((TempBuffer[0] & EnCode1[TempCount1]) == EnCode1[TempCount1]) { 
            
          TempBuffer[2] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(GroupStorage, 0, TempCount1));
          TempBuffer[3] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(GroupStorage, 0, TempCount1)+1));
            
          for (TempCount2 = 0; TempCount2 < 8; TempCount2++) {
            if ((TempBuffer[2] & EnCode1[TempCount2]) == EnCode1[TempCount2]) { 
              SetLED(TempCount2+ZONE_ACTIVE_LED); 
              SetRelay(TempCount2+ZONE_RELAY);
            }
          }
         
          for (TempCount2 = 0; TempCount2 < 2; TempCount2++) {
            if ((TempBuffer[3] & EnCode1[TempCount2]) == EnCode1[TempCount2]) { 
              SetLED(TempCount2+ZONE_ACTIVE_LED+8); 
              SetRelay(TempCount2+ZONE_RELAY+8); 
            }
          }
          TempCount1 = 0;
          TempCount2 = 0;
        } /* if (Tempbuffer[2]) end */
      } /* for (TempCount1) end */
    }

    if (SoftTimer1.m10sec == 3 )  { 
      EntryETGroup(PreTelStatus[0], PreTelStatus[1]);
      IsStartSoftTimer1 = OFF;
      ExGroupCount++;
      InitialSoftTimer(SoftTimer1);
    }

  if (( ExGroupCount == 3 ) ||
      ( CommunicationProcState == SetGroupState)) {
    ExGroupCount = 0;
    TempCount1 = 0;
    TempCount2 = 0;
    IsStartSoftTimer1 = OFF;
    InitialSoftTimer(SoftTimer1);
    Refresh();
    RefreshRelay();
    ProcState = ExGroup;
  } else { IsStartSoftTimer1 = ON; }
    
 break;


  case ExGroup:
    if (( 0 == PreTelStatus[0]) &&
        ( 0 == PreTelStatus[1]))
    {
      ProcState = ExitExGroup;
      ExGroupCount = 0;
    }
     
    if (KeyCode == ALL_ALARM_KEY) {
      ProcState = InitialAllZoneEmergencyState;
      GenCount2 = 0;
    }
    break;

  case ExitExGroup:
  /* Original version
  // restore prev extra group state state
  RestoreLCDState();
  RestoreRelayState();
  RestoreLEDState();
  ProcState = StoreProcState;
  CallStationState = StoreCallStationState;
  // release callstation bus
  CallStationMPUTalkOk();
  Refresh();
  // RefreshRelay();
  RefreshRelay();
  */
    if (ExGroupCount == 0) { 
      ExitGroup();
      CallStationMPUTalkOk();
      ExGroupCount++;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      ClrAllRelay();
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF;
      ExitGroup();
  //  CallStationMPUTalkOk();
      ExGroupCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
    if ((ExGroupCount == 3 ) ||
        (CommunicationProcState == ExitGroupState)||
        (CallStationState == CallStationMPUTalkState)||
        (CallStationState == CallStationBGMONState)) {

      ClrAllLED();
	  #ifndef _DISABLE_MAXVOLUME_
	  	ClrEmergencyRelay();
	  #endif

      if (IsBGMMode) { 
        ProcState = InitialBGMPlayState;
      } else { 
        ProcState = EntryNormalState;
      }
      GenCount2 = 0;
      //  ResetCallStationBus();
      IsStartSoftTimer1 = OFF;
      InitialSoftTimer(SoftTimer1);  
    } else {
      IsStartSoftTimer1 = ON;
    }
    break;
#undef ExGroupCount
#undef TempCount1
#undef TempCount2
#undef TempBuffer

// new state for remote panel
#define InitCount  GenCount2
  case EntryNormalState:
    if (InitCount == 0) {
      SPECIAL_CMD(SPC_NORMAL, 0, 0);
      WatchDog = ~WatchDog;  /* Reset hardware watch dog timer */
      LcdScreen(LCD_BL_ON);
      LCDStatus();
      CallStationMPUTalk();
      InitCount = 1;      
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      LCDDelay();
      LcdScreen(LCD_NORMAL);
      WatchDog = ~WatchDog;  /* Reset hardware watch dog timer */
    } else if (SoftTimer1.m10sec == 3 ) { 
      SPECIAL_CMD(SPC_NORMAL, 0, 0);
      IsStartSoftTimer1 = OFF; 
      InitCount++;
      InitialSoftTimer(SoftTimer1);
    }
    if (( InitCount == 3 )) {
     // ||(CommunicationProcState == InitialEntrySetupState )||(CallStationState == CallStationMPUTalkState)) 
      LcdScreen(LCD_NORMAL);
      InitCount = 0;
	  Delay1 = 0; // using send LCD Status in NormalState  
	  Delay2 = 0; // using send LCD Status in NormalState
      ProcState = NormalState;
      CallStationMPUTalkOk();
      CallStationState=CallStationNormalState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    } else { 
      IsStartSoftTimer1 = ON; 
    }
    break;
#undef InitCount
  // process alarm
  case ZoneSelectProc:
    if (GenCount2 == 0) {
      if (EMG_KEYSET[1] == 0xFF) {
        KeyIsSet(EMG_KEYSET[0]+1);
      } else {
        KeyIsReset(EMG_KEYSET[0]+1);
      }
      GenCount2 = 1;      
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
    } else if (SoftTimer1.m10sec == 3 ) { 
      if (EMG_KEYSET[1] == 0xFF) {
        KeyIsSet(EMG_KEYSET[0]+1);
      } else {
        KeyIsReset(EMG_KEYSET[0]+1);
      }
      IsStartSoftTimer1 = OFF; 
      GenCount2++;
      InitialSoftTimer(SoftTimer1);
    }
    
    if (( GenCount2 == 3 )) {
     // ||(CommunicationProcState == InitialEntrySetupState )||(CallStationState == CallStationMPUTalkState)) 
      GenCount2 = 0;
      ProcState = OnlyEmergencyUseTempState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    } else { 
      IsStartSoftTimer1 = ON; 
    }
 } /* ProcState switch - case end */


 if (IsFireDiffer()) {
   if (IsAnyFireAction()) {
     BYTE Temp4[2],Temp3[2];

     GetFireStatus(&Temp4[0]);
     FireZoneBuffer[0] |= Temp4[1];
     FireZoneBuffer[1] |= Temp4[0];

     if (!IsPreEmergencyMode) {
       GenCount1 = 0;
       GenCount2 = 0;
       GenCount3 = 0;
       SetupBuffer[0] = 0;
       IsStartSoftTimer1      = OFF;
       IsStartSoftTimer2      = OFF;
//       DisCallStation();

	   	if (InitialCheckIsAlarmHoldState != ProcState) {
       		ProcState = InitialPreFireEmergencyState;
	   	}
     }

	 SetFireRelationZone();
	 
     if (!IsEmergencyMode) {
       if (FireZoneCount >= 2) {
	   	 if ((InitialCheckIsAlarmHoldState != ProcState) ||
		 	(CheckIsAlarmHoldState != ProcState)) {
	   	 	ProcState = InitialConfirmFireEmergencyState;
	   	 }
         GenCount1 = 0;
         GenCount2 = 0;
         GenCount3 = 0;
       }
     }
     IsFireActiveFlag       = TRUE;
   }
 } // if (IsFireDiff()) end
   
 if (IsShortDiffer())
 {
   BYTE count;    
   GetShortStatus(&LoopShortBuffer[0]);

   for ( count = 0; count < 10; count++)
   {
      if (IsLoopShort(count))
      {
		ClrLED(count+ZONE_ACTIVE_LED);
        ClrRelay(count);
        SetFlash(count+ZONE_ACTIVE_LED);
      }
	  else
	  {
        ClrFlash(count+ZONE_ACTIVE_LED);
	  }
   }
   		IsLoopShortFlag = TRUE;
 } // if (IsShortDiffer()) end 
   
 if (IsFireActiveFlag) {
   if (SFireCount == 0) { 
     FireAlarmAction(FireZoneBuffer[0], FireZoneBuffer[1]);
     CallStationFire();
     SFireCount++;
     InitialSoftTimer(SoftTimer3);
     IsStartSoftTimer3 = ON;
   }

   if (SoftTimer3.m10sec == 3 ) { 
     FireAlarmAction(FireZoneBuffer[0], FireZoneBuffer[1]);
     //     CallStationFire();
     SFireCount++;
     IsStartSoftTimer3 = OFF; 
     InitialSoftTimer(SoftTimer3);
     IsStartSoftTimer3 = ON;
   }
 
   if (( SFireCount == 3 ) ||
       ( CommunicationProcState == FireAlarmActiveState)||
       ( CallStationState == CallStationFirePlayState)) {
     SFireCount = 0;
     IsFireActiveFlag = FALSE;
     IsStartSoftTimer3 = OFF; 
     InitialSoftTimer(SoftTimer3);
   } else { 
     IsStartSoftTimer3 = ON; 
   }
 } else { // if (IsFireActiveFlag) end
   if (IsLoopShortFlag) {
     if (SFireCount == 0) {
       ZoneLoopShortAction(LoopShortBuffer[1], LoopShortBuffer[0]);
       SFireCount++;
       InitialSoftTimer(SoftTimer3);
       IsStartSoftTimer3 = ON;
     }
    
     if (SoftTimer3.m10sec == 3 ) { 
       ZoneLoopShortAction(LoopShortBuffer[1], LoopShortBuffer[0]);
       SFireCount++;
       IsStartSoftTimer3 = OFF; 
       InitialSoftTimer(SoftTimer3);
       IsStartSoftTimer3 = ON; 
     }
 
     if (( SFireCount == 3 ) ||
         ( CommunicationProcState == LoopShortActiveState))
     {
       SFireCount = 0;
       IsLoopShortFlag = FALSE;
       IsStartSoftTimer3 = OFF; 
       InitialSoftTimer(SoftTimer3);
     } else { 
       IsStartSoftTimer3 = ON; 
     }        
   }  // if (IsLoopShortFlag) end
 } // else end

 if (StorageStatus != Read_BYTE_EEPROM(CalcEEPROMOffset(FireStatusStorage, 0, 0)+2)) {
   if (GetSystemTimer() != PreEEPROMWriteTimer) {
     Write_BYTE_EEPROM(CalcEEPROMOffset(FireStatusStorage, 0, 0)+2, StorageStatus);
     PreEEPROMWriteTimer = GetSystemTimer();        
   }     
 } else {
   if ((Read_BYTE_EEPROM(CalcEEPROMOffset(FireStatusStorage, 0, 0)) != (~FireZoneBuffer[1]))) {
     if (GetSystemTimer() != PreEEPROMWriteTimer) {
       Write_BYTE_EEPROM((CalcEEPROMOffset(FireStatusStorage, 0, 0)), (~FireZoneBuffer[1]));
       PreEEPROMWriteTimer = GetSystemTimer();
     }
   } else { 
     if ((Read_BYTE_EEPROM(CalcEEPROMOffset(FireStatusStorage, 0, 0)+1)) != (~FireZoneBuffer[0])) {
       if (GetSystemTimer() != PreEEPROMWriteTimer) {
         Write_BYTE_EEPROM((CalcEEPROMOffset(FireStatusStorage, 0, 0)+1), (~FireZoneBuffer[0]));
         PreEEPROMWriteTimer = GetSystemTimer();
       }
     }
   }
 }

 //========================================================================
 // added for extra group mode (using EMG. Tel trigger)
 // check Emg. Tel status
    if (IsExGroupDiffer()) {
      if ((0 != PreTelStatus[0]) ||  
          (0 != PreTelStatus[1])) {
        //================================================
        // check current state
        if ((ProcState == NormalState)                ||
            (ProcState == BGMPlayState)               ||
            (ProcState == CallStationBGMPlayState)    ||
            (ProcState == CallStationSelectZoneState) ||
            (ProcState == CallStationModeState)) {
          // not send exit BGM mode
          if (IsBGMMode) {
            CallStationMPUBGMOFF();
            SelectAudioSource(ALLCLOSE_AUDIO);
          }

          GenCount2 = 0;
          GenCount3 = 0;
          GenCount4 = 0;
          ProcState = EntryExGroup;
     
          ClrAllLED();
          ClrAllRelay();    

          CallStationMPUTalk();
        } else if (ExGroup == ProcState) {
        // entry EntryExGroup again refresh group state
          GenCount2 = 0;
          GenCount3 = 0;
          GenCount4 = 0;
          ProcState = EntryExGroup;
          ClrAllLED();
          ClrAllRelay(); 
        }
      }      // end 
    } // end if (IsExGroupDiffer())
  } /* while (TRUE) end */
} /* main() end */

void SetFireRelationZone(void) {
  BYTE Temp[2];
  BYTE count1;
  BYTE count2;
        
  // bit7 ...bit 0
  Temp[0] = FireZoneBuffer[1];
  for (count1 = 0; count1 < 10; count1++) {
    if (count1 == 8) {  
    // bit9, bit8
      Temp[0] = FireZoneBuffer[0]; 
    }
    
    if (Temp[0] & 0x01) {
      SetLED(count1); // Fire active LED light 
      FireZoneCount++;     
      Temp[1] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireRelationStorage, 0, count1));
      for (count2 = 0; count2 < 10; count2++) {
        if (count2 == 8) { 
          Temp[1] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireRelationStorage, 0, count1)+1); 
        }

        if (Temp[1] & 0x01) {
          SetLED(count2+ZONE_ACTIVE_LED);
          SetRelay(count2);
          if ( count2 < 8) {
            FireRelationMask[0] |= EnCode1[count2];
          } else { 
            FireRelationMask[1] |= EnCode1[count2];
          }
        }  // if (Temp[1])
        Temp[1] = Temp[1] >> 1;    
      } // for (count2) end
    } // if (Temp[0]) end
    Temp[0] = Temp[0] >> 1;
  } // for (count1) end
}

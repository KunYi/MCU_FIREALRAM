#if ! defined( _COMMUNICATION_H_ )
#define _COMMUNICATION_H_
#include "regw77e58.h"
#include "common.h"
#include "ptimer.h"
#include "uart.h"
#include "sm10.h"
#define ExtendPort_Put(x)                  UART0_Put(x)
#define ExtendPort_Get(x)                  UART0_Get(x)
#define ExtendPort_Receive_Length()        UART0_R_Length
#define ExtendPort_Transmit_Length()       UART0_T_Length
#define IsExtendUnit_Recevice()            SyncState
#define CallStationPort_Put(x)             UART1_Put(x)
#define CallStationPort_Get(x)             UART1_Get(x)
#define CallStationPort_Receive_Length()   UART1_R_Length
#define CallStationPort_Transmit_Length()  UART1_T_Length
#define FireSetupPackage                   0
#define EntrySetupPackage                  3
#define ExitSetupPackage                   4
#define EntryFireRelationSetupPackage      5
#define FireRelationSetupZoneAssignPackage 17
#define SaveFireRelationSetupPackage       6
#define ExitFireRelationSetupPackage       7
#define EntryBackgroundMusicSetupPackage   8
#define SaveBackgroundMusicSetupPackage    9
#define ExitBackgroundMusicSetupPackage    10
#define EntryGroupSetupPackage             11
#define GroupSetupAssignPackage            17
#define SaveGroupSetupPackage              12
#define ExitGroupSetupPackage              13
#define EntryVoiceMessageSetupPackage      14
#define VoiceMessageSetupZoneAssignPackage 17
#define SaveVoiceMessageSetupPackage       15
#define ExitVoiceMessageSetupPackage       16
#define EntryVoiceTestPlayPackage          28
#define VoiceTestPlayZoneAssignPackage     17
#define VoiceTestPlayStopPackage           30
#define ExitVoiceTestPlayPackage           31
#define IsExistPackage                     18
#define EntryHumanHandlingPackage          19
#define ExitHumanHandlingPackage           20
#define EntryBGMPlayPackage                21
#define ExitBGMPlayPackage                 22
#define EntryGroupPackage                  23
#define ExitGroupPackage                   24
#define KeyIsSetPackage                    25
#define KeyIsResetPackage                  26
#define GroupActivePackage                 32
#define CallStationActivePackage           32
#define ExitCallStationModePackage         13
#define ReplyKeySetPackage                 33
#define ReplyKeyResetPackage               34
#define EntryAllZoneEmergencyPackage       35
#define ExitAllZoneEmergencyPackage        36
#define ZoneLoopShortPackage               37
#define ReplyShortPackage                  38
#define FireAlarmPackage                   39
#define ReplyFireAlarmPackage              40
#define ReplyEmergencyTelPackage           41
#define ReplyExtendZoneActivePackage       42
#define CheckIsAlarmPackage                43
#define MicPhonePressPackage               44
#define CallStationSetZonePackage          48
#define CallStationResetZonePackage        49
#define AllCallPackage                     50

#define C_NoFirePackage			   0
#define C_FirePackage			   1
#define C_CallStationTalkPackage	   2
#define C_TalkingPackage		   3
#define C_TalkOKPackage		       	   4
#define C_ZoneSelectPackage		   5
#define C_GroupSelectPackage		   6
#define C_AllCallPackage		   7
#define C_ChimeUPPackage		   8
#define C_ChimeDownPackage		   9
#define C_MPUBGMONPackage		  10
#define C_MPUBGMOFFPackage		  11
#define C_BGMONPackage			  12
#define C_BGMOFFPackage			  13
#define C_PowerOnPackage		  14
#define C_PowerOnErrorPackage		  15
#define C_MPUTalkPackage	          16
#define C_MPUTalkOkPackage		  17

#define CallStationNoFire() \
        CallStation_Transmit(C_NoFirePackage, 0)
#define CallStationFire() \
        CallStation_Transmit(C_FirePackage, 0)
#define CallStationTalk(ID) \
        CallStation_Transmit(C_CallStationTalkPackage, ID)
#define CallStationTalking() \
        CallStation_Transmit(C_TalkingPackage, 0)
#define CallStationTalkOk() \
        CallStation_Transmit(C_TalkOKPackage, 0)
#define CallStationZoneSelect() \
        CallStation_Transmit(C_ZoneSelectPackage, 0)
#define CallStationGroupSelect() \
        CallStation_Transmit(C_GroupSelectPackage, 0)
#define CallStationAllCall() \
        CallStation_Transmit(C_AllCallPackage, 0)
#define CallStationChimeUP() \
        CallStation_Transmit(C_ChimeUPPackage, 0)
#define CallStationChimeDown() \
        CallStation_Transmit(C_ChimeDownPackage, 0)
#define CallStationMPUBGMON() \
        CallStation_Transmit(C_MPUBGMONPackage, 0)
#define CallStationMPUBGMOFF() \
        CallStation_Transmit(C_MPUBGMOFFPackage, 0)
#define CallStationBGMON() \
        CallStation_Transmit(C_BGMONPackage, 0)
#define CallStationBGMOFF(ID) \
        CallStation_Transmit(C_BGMOFFPackage, ID)
#define CallStationPowerOnOk() \
        CallStation_Transmit(C_PowerOnPackage, 0)
#define CallStationPowerOnError(ID) \
        CallStation_Transmit(C_PowerOnErrorPackage, ID)
#define CallStationMPUTalk() \
        CallStation_Transmit(C_MPUTalkPackage, 0)
#define CallStationMPUTalkOk() \
        CallStation_Transmit(C_MPUTalkOkPackage, 0)



#define IsExtendUnitExist(ID) \
        ExtendUnit_Transmit(IsExistPackage, 0, 0, 0, 0, ID)
#define EntrySetupMode() \
        ExtendUnit_Transmit(EntrySetupPackage, 0, 0, 0, 0, 0)
#define ExitSetupMode()  \
        ExtendUnit_Transmit(ExitSetupPackage, 0, 0, 0, 0, 0)
#define EntryFireRelationSetup() \
        ExtendUnit_Transmit(EntryFireRelationSetupPackage, 0, 0, 0, 0, 0)
#define FireRelationSetupZoneAssigen(Zone) \
        ExtendUnit_Transmit(FireRelationSetupZoneAssignPackage, (Zone), 0, 0, 0, 0)
#define SaveFireRelationSetup() \
        ExtendUnit_Transmit(SaveFireRelationSetupPackage, 0, 0, 0, 0, 0)
#define ExitFireRelationSetup() \
        ExtendUnit_Transmit(ExitFireRelationSetupPackage, 0, 0, 0, 0, 0)
#define EntryBackgroundMusicSetup() \
        ExtendUnit_Transmit(EntryBackgroundMusicSetupPackage, 0, 0, 0, 0, 0)
#define SaveBackgroundMusicSetup() \
        ExtendUnit_Transmit(SaveBackgroundMusicSetupPackage, 0, 0, 0, 0, 0)
#define ExitBackgroundMusicSetup() \
        ExtendUnit_Transmit(ExitBackgroundMusicSetupPackage, 0, 0, 0, 0, 0)
#define EntryGroupSetup() \
        ExtendUnit_Transmit(EntryGroupSetupPackage, 0, 0, 0, 0, 0)
#define GroupSetupAssign(GroupNo) \
        ExtendUnit_Transmit(GroupSetupAssignPackage, (GroupNo), 0, 0, 0, 0)
#define SaveGroupSetup() \
        ExtendUnit_Transmit(SaveGroupSetupPackage, 0, 0, 0, 0, 0)
#define ExitGroupSetup() \
        ExtendUnit_Transmit(ExitGroupSetupPackage, 0, 0, 0, 0, 0)
#define EntryVoiceMessageSetup() \
        ExtendUnit_Transmit(EntryVoiceMessageSetupPackage, 0, 0, 0, 0, 0)
#define VoiceMessageSetupZoneAssigen(Zone) \
        ExtendUnit_Transmit(VoiceMessageSetupZoneAssignPackage, (Zone), 0, 0, 0, 0)
#define SaveVoiceMessageSetup() \
        ExtendUnit_Transmit(SaveVoiceMessageSetupPackage, 0, 0, 0, 0, 0)
#define ExitVoiceMessageSetup() \
        ExtendUnit_Transmit(ExitVoiceMessageSetupPackage, 0, 0, 0, 0, 0)
#define EntryVoiceTestPlay() \
        ExtendUnit_Transmit(EntryVoiceTestPlayPackage, 0, 0, 0, 0, 0)
#define VoiceTestPlayZoneAssigen(Zone) \
        ExtendUnit_Transmit(VoiceTestPlayZoneAssignPackage, (Zone), 0, 0, 0, 0)
#define VoiceTestPlayStop() \
        ExtendUnit_Transmit(VoiceTestPlayStopPackage, 0, 0, 0, 0, 0)
#define ExitVoiceTestPlay() \
        ExtendUnit_Transmit(ExitVoiceTestPlayPackage, 0, 0, 0, 0, 0)
#define EntryHumanHandling() \
        ExtendUnit_Transmit(EntryHumanHandlingPackage, 0, 0, 0, 0, 0)
#define ExitHumanHandling() \
        ExtendUnit_Transmit(ExitHumanHandlingPackage, 0, 0, 0, 0, 0)
#define EntryBGMPlay() \
        ExtendUnit_Transmit(EntryBGMPlayPackage, 0, 0, 0, 0, 0)
#define ExitBGMPlay() \
        ExtendUnit_Transmit(ExitBGMPlayPackage, 0, 0, 0, 0, 0)
#define EntryGroup() \
        ExtendUnit_Transmit(EntryGroupPackage, 0, 0, 0, 0, 0)
#define GroupActive(State1, State2) \
        ExtendUnit_Transmit(GroupActivePackage, (State1), (State2), 0, 0, 0)
#define ExitGroup() \
        ExtendUnit_Transmit(ExitGroupPackage, 0, 0, 0, 0, 0)
#define KeyIsSet(Key) \
        ExtendUnit_Transmit(KeyIsSetPackage, (Key), 0, 0, 0, 0)
#define KeyIsReset(Key) \
        ExtendUnit_Transmit(KeyIsResetPackage, (Key), 0, 0, 0, 0)
#define CallStationActive(State1,State2) \
        ExtendUnit_Transmit(CallStationActivePackage, (State1), (State2), 0, 0, 0)
#define ExitCallStation() \
        ExtendUnit_Transmit(ExitCallStationPackage, 0, 0, 0, 0, 0)
#define ReplyKeySet(ExtendID,KeyNo) \
        ExtendUnit_Transmit(ReplyKeySetPackage, (ExtendID), (KeyNo), 0, 0, 0) 
#define ReplyKeyReset(ExtendID,KeyNo) \
        ExtendUnit_Transmit(ReplyKeyResetPackage, (ExtendID), (KeyNo), 0, 0, 0)
#define EntryAllZoneEmergency() \
        ExtendUnit_Transmit(EntryAllZoneEmergencyPackage, 0, 0, 0, 0, 0)
#define ExitAllZoneEmergency() \
        ExtendUnit_Transmit(ExitAllZoneEmergencyPackage, 0, 0, 0, 0, 0)
#define FireAlarmAction(Value1, Value2) \
        ExtendUnit_Transmit(FireAlarmPackage, (Value1), (Value2), 0, 0, 0)
#define ZoneLoopShortAction(Value1, Value2) \
        ExtendUnit_Transmit(ZoneLoopShortPackage, (Value1), (Value2), 0, 0, 0)
#define ReplyShort(Value1, Value2) \
        ExtendUnit_Transmit(ReplyShortPackage, (Value1), (Value2), 0, 0, 0)
#define ReplyFireAlram(Value1,Value2) \
        ExtendUnit_Transmit(ReplyFireAlarmPackage, (Value1), (Value2), 0, 0, 0)
#define ReplyEmergencyTel(Value1, Value2) \
        ExtendUnit_Transmit(ReplyEmergencyTelPackage, (Value1), (Value2), 0, 0, 0)
#define ReplyExtendZoneActive(Value1) \
        ExtendUnit_Transmit(ReplyExtendZoneActivePackage, (Value1), 0xff, 0, 0, 0)
#define CheckIsAlarm() \
        ExtendUnit_Transmit(CheckIsAlarmPackage, 0, 0, 0, 0, 0)
#define MicPhonePress() \
        ExtendUnit_Transmit(MicPhonePressPackage, 0, 0, 0, 0, 0)
#define CallStationSetZone(State1,State2) \
        ExtendUnit_Transmit(CallStationSetZonePackage, (State1), (State2), 0, 0, 0)
#define CallStationResetZone(State1,State2) \
        ExtendUnit_Transmit(CallStationResetZonePackage, (State1), (State2), 0, 0, 0)
#define AllCallZone() \
        ExtendUnit_Transmit(AllCallPackage, 0, 0, 0, 0, 0)/*主機按下全區廣播
                                                            鍵,送出此封包給
	                                                    副機*/
                             
 
#if defined ( _PHYSCICAL_COMM_ )
#define Package_SYNC            0x15
#define Package_Length          0x09
/* ID */
#define Single_Extend  		0x01
#define All_Extend              0x02
#define Single_CallStation      0x01
#define All_CallStation		0x02
#define CallStation_Single_Extend     0x01
#define CallStation_All_Extend	      0x02

/* Parameter typer */
#define DataFixed         	0x20
#define DataFixed1         	0x21
#define DataFixed2         	0x22
#define ParameterFixed          0x00
#define ParameterField1         0x01
#define ParameterField2         0x02
#define ParameterField3         0x03
#define ParameterField4         0x04
#define ParameterField5         0x05
#define ParameterField6         0x06

#define Remote_Control          0x03
#define CallStation_Remote      0x04
#define All_Code                0x20
#define Remote_Code             0x40
#define CallStation_Remote_Code 0x60
/* 主機回答CallStation */
const CODE_DATA BYTE CallStation_Package[][5] = { 
	{ All_CallStation, 0x00, DataFixed,'F', 0x00},					//NO. 0
	//	No Fire
	{ All_CallStation, 0x00, DataFixed,'F', 0xff},						//			1
	//	 Fire
	{ Single_CallStation, 0x01, DataFixed,'T', 'A'},				//			2
	//	 CallStation Talking
	{ All_CallStation, 0x01, DataFixed,'T', 0xff},						//			3
	//	 Talking Now
	{ All_CallStation, 0x01, DataFixed,'T', 0x00},					//			4
	//	 Talking Over
	{ Single_CallStation, 0x02, DataFixed,'Z', 'A'},				//			5
	//	 Zone Select
	{ Single_CallStation, 0x03, DataFixed,'G', 'A'},				//			6
	//	 Group Select
	{ Single_CallStation, 0x04, DataFixed, 'A', 0x00},		//			7
	//	 All Call
	{ Single_CallStation, 0x05, DataFixed, 'C', 'A'},				//			8
	//	 Chime	UP
	{ Single_CallStation, 0x05, DataFixed, 'C', 'D'},			//			9
	//	 Chime DOWN
	{ All_CallStation, 0x06, DataFixed, 'B', 0xff},					//			10
	//	 MPU BackGround Music Select
	{ All_CallStation, 0x06, DataFixed, 'B', 0x00},					//			11
	//	 MPU BackGround Music DisSelect
	{ Single_CallStation, 0x07, DataFixed, 'B', 'A'},				//			12
	//	 BackGround Music Select
	{ Single_CallStation, 0x07, DataFixed, 'B', 'C'},				//			13
	//	 BackGround Music DisSelect
	{ Single_CallStation, 0x0a, DataFixed,'N', 0x00},			//			14
	//	Power ON the Call station OK
	{ Single_CallStation, 0x00, DataFixed,'N', 0xee},			//			15
	//	Power ON the Call station Error
	{ All_CallStation, 0x08, DataFixed,'T', 0xff},				//			16
	//	MPU Talking
	{ All_CallStation, 0x08, DataFixed,'T', 0x00}};			//			17
	//	MPU Talking Ok



const CODE_DATA BYTE Extend_Package[][8] = { 
  {   All_Extend, 'F', 'M',  ParameterFixed,   0,   0,   0,   0 },/* NO.  0 */
  {   All_Extend, 'F', 'R',  ParameterFixed,   0,   0,   0,   0 },/*      1 */
  {   All_Extend, 'F', 'A',  ParameterFixed,   0,   0,   0,   0 },/*      2 */
  
  /* ---------------------------------------------------------------------- */
  /*                 setup control communication package                    */
  /* ______________________________________________________________________ */
  /*  Setup mode select package */
  {   All_Extend, 'M', 'S',  ParameterFixed, 'M', 'O', 'D',  'E'},/*      3 */
  
  /*  Exit setup mode package */
  {   All_Extend, 'M', 'S',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*      4 */
  
  /* ====================================================================== */  
  /*  Setup fire relation zone package */
  {   All_Extend, 'M', 'C',  ParameterFixed, 'M', 'O', 'D',  'E'},/*      5 */
  /*  Save fire relation zone package */
  {   All_Extend, 'M', 'C',  ParameterFixed, 'S', 'A', 'V',  'E'},/*      6 */
  /*  Exit Setup fire relation zone package */
  {   All_Extend, 'M', 'C',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*      7 */
  /* ====================================================================== */  

  /* ====================================================================== */  
  /*  Setup background music zone package */
  {   All_Extend, 'M', 'B',  ParameterFixed, 'M', 'O', 'D',  'E'},/*      8 */
  /*  Save background music zone package */
  {   All_Extend, 'M', 'B',  ParameterFixed, 'S', 'A', 'V',  'E'},/*      9 */  
  /*  Exit Setup background music zone package */
  {   All_Extend, 'M', 'B',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*     10 */
  /* ====================================================================== */  

  /* ====================================================================== */  
  /*  Setup group zone package */
  {   All_Extend, 'M', 'G',  ParameterFixed, 'M', 'O', 'D',  'E'},/*     11 */
  /*  Save group zone package */
  {   All_Extend, 'M', 'G',  ParameterFixed, 'S', 'A', 'V',  'E'},/*     12 */  
  /*  Exit Setup group zone package */
  {   All_Extend, 'M', 'G',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*     13 */
  /* ====================================================================== */  

  /* ====================================================================== */  
  /*  Setup zone voice message package */
  {   All_Extend, 'M', 'V',  ParameterFixed, 'M', 'O', 'D',  'E'},/*     14 */
  /*  Save zone voice message package */
  {   All_Extend, 'M', 'V',  ParameterFixed, 'S', 'A', 'V',  'E'},/*     15 */  
  /*  Exit Setup zone voice message package */
  {   All_Extend, 'M', 'V',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*     16 */
  /* ====================================================================== */  
  
  /* Setup zone package */
  {   All_Extend, 'M', 'F',  ParameterField1, 0,    0,   0, 0xff},/*     17 */
  
  /* ______________________________________________________________________ */
  /*           setup control communication package define end               */
  /* ---------------------------------------------------------------------- */
  
  /*  Is exist extend unit */
  {   All_Extend, 'M', 'M',  ParameterFixed, 'O', 'K',   0,   0 },/*     18 */
  
  /* ====================================================================== */  
  /*  Entry human handling mode */
  {   All_Extend, 'H', 'M',  ParameterFixed, 'M', 'O', 'D',  'E'},/*     19 */
  /*  Exit human handling mode */
  {   All_Extend, 'H', 'Q',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*     20 */
  /* ====================================================================== */  
  
  /* ====================================================================== */  
  /*  Entry background music play mode */
  {   All_Extend, 'B', 'M',  ParameterFixed, 'M', 'O', 'D',  'E'},/*    21 */   
  /*  Exit background music play mode */
  {   All_Extend, 'B', 'Q',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*    22 */   
  /* ====================================================================== */  

 
  /* ====================================================================== */  
  /*  Entry group public address mode */
  {   All_Extend, 'G', 'M',  ParameterFixed, 'M', 'O', 'D',  'E'},/*     23 */   
  /*  exit group public address mode */
  {   All_Extend, 'G', 'Q',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*     24 */
  /* ====================================================================== */  

  /* ====================================================================== */  
  /*  Main controllr zone key is set  */
  {   All_Extend, 'K', 'A',  ParameterField1, 0,   0,   0,  0xff},/*     25 */   
  /*  Main controllr zone key is reset  */
  {   All_Extend, 'K', 'A',  ParameterField1, 0,   0,   0,     0},/*     26 */
  /* ====================================================================== */  

  {   All_Extend, 'G', 'A',  ParameterField2, 0,   0,   0,     0},/*     27 */

  /* ====================================================================== */  
  /*  Main controllr zone key is set  */
  {   All_Extend, 'M', 'X',  ParameterFixed, 'M', 'O', 'D',  'E'},/*     28 */   
  /*  Main controllr zone key is reset  */
  {   All_Extend, 'K', 'A',  ParameterField1, 0,   0,   0,     0},/*     29 */
  /*  Main controllr zone key is set  */
  {   All_Extend, 'M', 'X',  ParameterFixed, 'S', 'T', 'O',  'P'},/*     30 */   
  /*  Main controllr zone key is reset  */
  {   All_Extend, 'M', 'X',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*     31 */
  /* ====================================================================== */
    
  /* ====================================================================== */
  /* Call station group public address mode */
  {   All_Extend, 'G', 'A', ParameterField3, 'G', 'A',   0,    0},/*     32 */
  /* ====================================================================== */
  
  /* ====================================================================== */
  /* Reply extend unit any key set */
  {Single_Extend, 'K', 'R', ParameterField4,   0,   0,   0, 0xff},/*     33 */
  /* Replay extend unit any key reset */
  {Single_Extend, 'K', 'R', ParameterField4,   0,   0,   0,    0},/*     34 */
  /* ====================================================================== */

  /* ====================================================================== */  
  /*  Entry all zone emergency public address  mode */
  {   All_Extend, 'E', 'M',  ParameterFixed, 'M', 'O', 'D',  'E'},/*     35 */   
  /*  Exit all zone emergency public address mode */
  {   All_Extend, 'E', 'Q',  ParameterFixed, 'Q', 'U', 'I',  'T'},/*     36 */
  /* ====================================================================== */
  
  /* ====================================================================== */
  /* Reply extend unit loop short */
  {   All_Extend, 'S', 'A', ParameterField3,   0,   0,   0,    0},/*     37 */
  /* Reply extend unit loop short */
  {Single_Extend, 'S', 'R', ParameterField5,   0,   0,   0,    0},/*     38 */
  /* ====================================================================== */  

  /* ====================================================================== */
  /* Fire alarm active */
  {   All_Extend, 'F', 'A', ParameterField3,   0,   0,   0,    0},/*     39 */
  /* Reply extend unit active */
  {Single_Extend, 'F', 'R', ParameterField5,   0,   0,   0,    0},/*     40 */
  /* ====================================================================== */  
  {Single_Extend, 'T', 'R', ParameterField5,   0,   0,   0,    0},/*     41 */
  
  {Single_Extend, 'M', 'R', ParameterField5,   0,   0,   0, 0xff},/*     42 */

  {   All_Extend, 'C', 'A',  ParameterFixed, 'P', 'U', 'S',  'H'},/*     43 */
  
  {   All_Extend, 'P', 'A',  ParameterFixed, 'P', 'U', 'S',  'H'},/*     44 */
  /*  Entry group public address mode */
  {Single_Extend, 'K', 'A',  ParameterFixed,   0,   0,   0,   0 }, /*			45*/  
  /*  exit group public address mode */
  {Single_Extend, 'K', 'A',  ParameterField1,   0,   0,   0,   0 },/*			46*/
  
  {Single_Extend, 0x55, 0x55, ParameterFixed, 0x55, 0x55, 0x55, 0x55},/*			47*/
  /*  CallStation Set Zone */
 // {CallStation_Single_Extend, 'C', 'A',  ParameterField4,   0,   0,   0,   0xff }, /*			48*/  
 //{CallStation_Single_Extend, 'Z', 'S',  ParameterField4,   0,   0,   0,   0xff }, /*			48*/  //by andy
 {ParameterField4, 'Z', 'S',  ParameterField4,   0,   0,   0,   0xff }, /*			48*/  //by andy
  /*  CallStation Reset Zone */
  {CallStation_Single_Extend, 'C', 'A',  ParameterField4,   0,   0,   0,   0 }, /*			49*/  
  /*  All Call Zone */
  {CallStation_Single_Extend, 'H', 'A',  ParameterFixed,  'A',   'L',  'L',   'C'}}; /*			50*/  

BOOLEAN ExtendUnit_Transmit(const BYTE, const BYTE, const BYTE, const BYTE, const BYTE, const BYTE);
BOOLEAN CallStation_Transmit(const BYTE , const BYTE );

#else
extern bit SyncState;
#endif
BOOLEAN IsReceiveExtendUnit(void);
BOOLEAN IsReceiveCallStation(void);
void InitialCommunication(void);                                       
void ExtendUnit_Receive(void);
void CallStation_Receive(void);
Status ProcExtendUnit(const Status,BYTE *,BYTE *);
Status ProcCallStation(const Status, BYTE *);

/* first  BYTE ==> Extend ID
   second BYTE ==> Transmit Package Number
   ...         ==> Package need Para */
// BOOLEAN ExtendUnit_Transmit(BYTE, BYTE, ...);
BOOLEAN ExtendUnit_Transmit(const BYTE, const BYTE, const BYTE, const BYTE, const BYTE, const BYTE);
BOOLEAN CallStation_Transmit(const BYTE, const BYTE);

void    DisCallStation(void);
void    ResetCallStationBus(void);
#endif
/* COMMUNICATION.H FILE END */

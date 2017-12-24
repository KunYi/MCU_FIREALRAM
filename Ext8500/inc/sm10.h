#if ! defined ( _SM10_H_ )
#define _SM10_H_

//#define  MaxExtendUnit 12
#define  MaxExtendUnit 16	//by andy(16)

#define ZONE_ACTIVE_LED   10
#define ZONE_FIRE_LED      0
#define GROUP_LED      30
#define ZONE_CONTROL_LED  20
#define ZONE_RELAY        0

#define MAX_ZONE          10
//#define ZONE_FIRE_SENSOR  0
//#define ZONE_TEL_SENSOR  0

/* define ZONE Active Green LED */
#define ZONE01_GLED       (ZONE_ACTIVE_LED+0)
#define ZONE02_GLED       (ZONE_ACTIVE_LED+1)
#define ZONE03_GLED       (ZONE_ACTIVE_LED+2)
#define ZONE04_GLED       (ZONE_ACTIVE_LED+3)
#define ZONE05_GLED       (ZONE_ACTIVE_LED+4)
#define ZONE06_GLED       (ZONE_ACTIVE_LED+5)
#define ZONE07_GLED       (ZONE_ACTIVE_LED+6)
#define ZONE08_GLED       (ZONE_ACTIVE_LED+7)
#define ZONE09_GLED       (ZONE_ACTIVE_LED+8)
#define ZONE10_GLED       (ZONE_ACTIVE_LED+9)

/* define ZONE Fire Red LED */
#define ZONE01_RLED       (ZONE_FIRE_LED+0)
#define ZONE02_RLED       (ZONE_FIRE_LED+1)
#define ZONE03_RLED       (ZONE_FIRE_LED+2)
#define ZONE04_RLED       (ZONE_FIRE_LED+3)
#define ZONE05_RLED       (ZONE_FIRE_LED+4)
#define ZONE06_RLED       (ZONE_FIRE_LED+5)
#define ZONE07_RLED       (ZONE_FIRE_LED+6)
#define ZONE08_RLED       (ZONE_FIRE_LED+7)
#define ZONE09_RLED       (ZONE_FIRE_LED+8)
#define ZONE10_RLED       (ZONE_FIRE_LED+9)

/* define GROUP Green LED */
#define GROUP01_GLED       (GROUP_LED+0)
#define GROUP02_GLED       (GROUP_LED+1)
#define GROUP03_GLED       (GROUP_LED+2)
#define GROUP04_GLED       (GROUP_LED+3)
#define GROUP05_GLED       (GROUP_LED+4)
#define GROUP06_GLED       (GROUP_LED+5)
#define GROUP07_GLED       (GROUP_LED+6)
#define GROUP08_GLED       (GROUP_LED+7)
#define GROUP09_GLED       (GROUP_LED+8)
#define GROUP10_GLED       (GROUP_LED+9)

/* define control indicate LED */
#define CONTROL01_LED     (ZONE_CONTROL_LED+0)
#define CONTROL02_LED     (ZONE_CONTROL_LED+1)
#define CONTROL03_LED     (ZONE_CONTROL_LED+2)
#define CONTROL04_LED     (ZONE_CONTROL_LED+3)
//#define CONTROL05_LED     (ZONE_CONTROL_LED+4)
//#define CONTROL06_LED     (ZONE_CONTROL_LED+5)
//#define CONTROL07_LED     (ZONE_CONTROL_LED+6)
//#define CONTROL08_LED     (ZONE_CONTROL_LED+7)
//#define CONTROL09_LED     (ZONE_CONTROL_LED+8)

/* define ZONE Relay */
#define ZONE01_RELAY      (ZONE_RELAY+0)
#define ZONE02_RELAY      (ZONE_RELAY+1)
#define ZONE03_RELAY      (ZONE_RELAY+2)
#define ZONE04_RELAY      (ZONE_RELAY+3)
#define ZONE05_RELAY      (ZONE_RELAY+4)
#define ZONE06_RELAY      (ZONE_RELAY+5)
#define ZONE07_RELAY      (ZONE_RELAY+6)
#define ZONE08_RELAY      (ZONE_RELAY+7)
#define ZONE09_RELAY      (ZONE_RELAY+8)
#define ZONE10_RELAY      (ZONE_RELAY+9)
 
/* define ZONE Fire Sensor */
/*
#define ZONE01_SENSOR     (ZONE_FIRE_SENSOR+0)
#define ZONE02_SENSOR     (ZONE_FIRE_SENSOR+1)
#define ZONE03_SENSOR     (ZONE_FIRE_SENSOR+2)
#define ZONE04_SENSOR     (ZONE_FIRE_SENSOR+3)
#define ZONE05_SENSOR     (ZONE_FIRE_SENSOR+4)
#define ZONE06_SENSOR     (ZONE_FIRE_SENSOR+5)
#define ZONE07_SENSOR     (ZONE_FIRE_SENSOR+6)
#define ZONE08_SENSOR     (ZONE_FIRE_SENSOR+7)
#define ZONE09_SENSOR     (ZONE_FIRE_SENSOR+8)
#define ZONE10_SENSOR     (ZONE_FIRE_SENSOR+9)
*/
/* define ZONE Tel Sensor */
/*
#define EMGZONE01_SENSOR     (ZONE_TEL_SENSOR+0)
#define EMGZONE02_SENSOR     (ZONE_TEL_SENSOR+1)
#define EMGZONE03_SENSOR     (ZONE_TEL_SENSOR+2)
#define EMGZONE04_SENSOR     (ZONE_TEL_SENSOR+3)
#define EMGZONE05_SENSOR     (ZONE_TEL_SENSOR+4)
#define EMGZONE06_SENSOR     (ZONE_TEL_SENSOR+5)
#define EMGZONE07_SENSOR     (ZONE_TEL_SENSOR+6)
#define EMGZONE08_SENSOR     (ZONE_TEL_SENSOR+7)
#define EMGZONE09_SENSOR     (ZONE_TEL_SENSOR+8)
#define EMGZONE10_SENSOR     (ZONE_TEL_SENSOR+9)
*/
/* Input key define */
#define  ZONE_KEY                       0


#define  CONTROL_KEY             16
#define  GROUP_KEY                  30
#define  ZONE_KEY01              (ZONE_KEY+0)
#define  ZONE_KEY02              (ZONE_KEY+1)
#define  ZONE_KEY03              (ZONE_KEY+2)
#define  ZONE_KEY04              (ZONE_KEY+3)
#define  ZONE_KEY05              (ZONE_KEY+4)
#define  ZONE_KEY06              (ZONE_KEY+5)
#define  ZONE_KEY07              (ZONE_KEY+6)
#define  ZONE_KEY08              (ZONE_KEY+7)
#define  ZONE_KEY09              (ZONE_KEY+8)
#define  ZONE_KEY10              (ZONE_KEY+9)
#define  GROUP_KEY01					(GROUP_KEY+0)
#define  GROUP_KEY02					(GROUP_KEY+1)
#define  GROUP_KEY03					(GROUP_KEY+2)
#define  GROUP_KEY04					(GROUP_KEY+3)
#define  GROUP_KEY05					(GROUP_KEY+4)
#define  GROUP_KEY06					(GROUP_KEY+5)
#define  GROUP_KEY07					(GROUP_KEY+6)
#define  GROUP_KEY08					(GROUP_KEY+7)
#define  GROUP_KEY09					(GROUP_KEY+8)
#define  GROUP_KEY10					(GROUP_KEY+9)
#define  CONTROL_KEY01           (CONTROL_KEY+0)
#define  CONTROL_KEY02           (CONTROL_KEY+1)
#define  CONTROL_KEY03           (CONTROL_KEY+2)
#define  CONTROL_KEY04           (CONTROL_KEY+3)
#define  CONTROL_KEY05           (CONTROL_KEY+4)
#define  CONTROL_KEY06           (CONTROL_KEY+5)
#define  CONTROL_KEY07           (CONTROL_KEY+6)
#define  CONTROL_KEY08           (CONTROL_KEY+7)
#define  CONTROL_KEY09           (CONTROL_KEY+8)
#define  CONTROL_KEY10           (CONTROL_KEY+9)

/* define SM10 control  LED and key alias */
/*
#define  SETUP_KEY               CONTROL_KEY06
#define  SETUP_LED               CONTROL06_LED
#define  NO_FIRE_KEY             CONTROL_KEY01
#define  NO_FIRE_LED             CONTROL01_LED
#define  HUMAN_HANDLING_KEY      CONTROL_KEY05
#define  HUMAN_HANDLING_LED      CONTROL05_LED
#define  BACKGROUND_MUSIC_KEY    CONTROL_KEY07
#define  BACKGROUND_MUSIC_LED    CONTROL07_LED
#define  TESTPLAY_KEY            CONTROL_KEY04
#define  TESTPLAY_LED            CONTROL04_LED
#define  GROUP_KEY               CONTROL_KEY08
#define  GROUP_LED               CONTROL08_LED
#define  ALARM_CANCEL_KEY        CONTROL_KEY03
#define  ALARM_CANCEL_LED        CONTROL03_LED
#define  CONFIRM_KEY             CONTROL_KEY02
#define  CONFIRM_LED             CONTROL02_LED
#define  ALL_ALARM_KEY           CONTROL_KEY09
#define  ALL_ALARM_LED           CONTROL09_LED
*/
#define  LCD1_KEY    			        CONTROL_KEY01
#define  LCD2_KEY    			        CONTROL_KEY02
#define  LCD3_KEY    			        CONTROL_KEY03
#define  LCD4_KEY    			        CONTROL_KEY04
#define  FIRE_KEY                     CONTROL_KEY05
#define  NO_FIRE_KEY             CONTROL_KEY06
#define  ALL_ZONE_KEY          CONTROL_KEY07
#define  BACKLIGHT_KEY       CONTROL_KEY08
#define  ALL_ALARM_KEY      CONTROL_KEY09
#define  RESET_ALARM_KEY  CONTROL_KEY10

#define  ALL_ALARM_LED       CONTROL01_LED
#define  FIRE_LED        			     CONTROL02_LED
#define  NO_FIRE_LED             CONTROL03_LED
#define  SOUND_LED           	  CONTROL04_LED

/* define Process state */
#define  InitialState                                      0
#define  NormalState                        (InitialState+ 1)
#define  EntryCheckPasswordState            (InitialState+ 2)
#define  InitialEntrySetupState             (InitialState+ 3)
#define  ExitEntrySetupState                (InitialState+ 4)
#define  InitialSetupModeSelectState        (InitialState+ 5)
#define  SetupModeSelectState               (InitialState+ 6)
#define  ExitSetupModeState                 (InitialState+ 7)
#define  InitialSetupFireRelationState      (InitialState+ 8)
#define  SetupFireRelationState             (InitialState+ 9)
#define  SetupFireRelationZoneState         (InitialState+10)
#define  FireRelationZoneAssigenState       (InitialState+11)
#define  SetFireRelationZoneState           (InitialState+12)
#define  ResetFireRelationZoneState         (InitialState+13)
#define  ReadFireRelationSetupState         (InitialState+14)
#define  SaveFireRelationZoneState          (InitialState+15)
#define  ExitSetupFireRelationState         (InitialState+16)
#define  InitialSetupGroupState             (InitialState+17)
#define  SetupGroupState                    (InitialState+18)
#define  SetupGroupZoneState                (InitialState+19)
#define  SetupGroupAssigenState             (InitialState+20)
#define  SetGroupZoneState                  (InitialState+21)
#define  ResetGroupZoneState                (InitialState+22)
#define  ReadGroupSetupState                (InitialState+23)
#define  SaveGroupZoneState                 (InitialState+24)
#define  ExitSetupGroupState                (InitialState+25)
#define  InitialSetupBGMState               (InitialState+26)
#define  ReadBGMSetupState                  (InitialState+27)
#define  SetupBGMZoneState                  (InitialState+28)
#define  SetBGMZoneState                    (InitialState+29)
#define  ResetBGMZoneState                  (InitialState+30)
#define  SaveBGMZoneState                   (InitialState+31)
#define  ExitSetupBGMState                  (InitialState+32)
#define  InitialSetupVoiceState             (InitialState+33)
#define  SetupVoiceZoneState                (InitialState+34)
#define  SetupVoiceZoneAssigenState         (InitialState+35)
#define  SetupVoiceMessageState             (InitialState+36)
#define  SaveVoiceMessageSetupState         (InitialState+37)
#define  ExitVoiceMessageSetupState         (InitialState+38)
#define  InitialVoiceTestPlayState          (InitialState+39)
#define  VoiceTestPlayState                 (InitialState+40)
#define  VoiceTestPlayZoneAssigenState      (InitialState+41)
#define  VoiceMessageTestPlayState          (InitialState+42)
#define  VoiceTestPlayStopState             (InitialState+43)
#define  ExitVoiceTestPlayState             (InitialState+44)
#define  InitialBGMPlayState                (InitialState+45)
#define  BGMPlayState                       (InitialState+46)
#define  ChangeHumanHandlingState           (InitialState+47)
#define  ChangeGroupState                   (InitialState+48)
#define  ChangeCallStationModeState         (InitialState+49)
#define  ExitBGMPlayState                   (InitialState+50)
#define  InitialHumanHandlingState          (InitialState+51)
#define  HumanHandlingState                 (InitialState+52)
#define  ExitHumanHandlingState             (InitialState+53)
#define  InitialConfirmFireEmergencyState   (InitialState+54)
#define  ConfirmFireEmergencyState          (InitialState+55)
#define  ExitConfirmFireEmergencyState      (InitialState+56)
#define  InitialPreFireEmergencyState       (InitialState+57)
#define  PreFireEmergencyState              (InitialState+58)
#define  ExitPreFireEmergencyState          (InitialState+59)
#define  InitialConfirmNoFireEmergencyState (InitialState+60)
#define  ConfirmNoFireEmergencyState        (InitialState+61)
#define  ExitConFirmNoFireEmergencyState    (InitialState+62)
#define  InitialGroupState                  (InitialState+63)
#define  GroupState                         (InitialState+64)
#define  GroupActionState                   (InitialState+65)
#define  ExitGroupState                     (InitialState+66)
#define  InitialCallStationModeState        (InitialState+67)
#define  CallStationModeState               (InitialState+68)
#define  ExitCallStationModeState           (InitialState+69)
#define  CallStationActiveState             (InitialState+70)
#define  InitialAllZoneEmergencyState       (InitialState+71)
#define  AllZoneEmergencyState              (InitialState+72)
#define  ExitAllZoneEmergencyState          (InitialState+73)
#define  InitialEmergencyTelState           (InitialState+74)
#define  EmergencyTelState                  (InitialState+75)
#define  ExitEmergencyTelState              (InitialState+76)
#define  InitialFireAlarmState              (InitialState+77)
#define  FireAlarmState                     (InitialState+78)
#define  ExitFireAlarmState                 (InitialState+79)
#define  EntryFireAlarmConfirmState         (InitialState+80)
#define  EntryShortActionState              (InitialState+81)
#define  ExtendUnitFireActiveState          (InitialState+82)
#define  InitialCheckIsAlarmHoldState       (InitialState+83)
#define  CheckIsAlarmHoldState              (InitialState+84)
#define  FireAlarmActiveState               (InitialState+85)
#define  LoopShortActiveState               (InitialState+86)
#define  SelectGroupState                   (InitialState+87)
#define  SetGroupState                      (InitialState+88)
#define  ResetGroupState                    (InitialState+89)
#define  SelectHumanHandlingState           (InitialState+90)
#define  SetHumanHandlingZoneState          (InitialState+91)
#define  ResetHumanHandlingZoneState        (InitialState+92)
#define  HumanHandlingActionState           (InitialState+93)
#define  CheckPrevStatus                    (InitialState+94)
#define  SetupModeSelectSpecialState				(InitialState+95)
#define  InitialCallStationZoneState				(InitialState+96)
#define  CallStationSelectZoneState				(InitialState+97)
#define  SetCallStationSelectZoneState						(InitialState+98)
#define  ResetCallStationSelectZoneState				(InitialState+99)
#define  CallStationZoneActionState							(InitialState+100)
#define  ExitCallStationZoneState										(InitialState+101)
#define  InitialCallStationBGMPlayState                (InitialState+102)
#define  CallStationBGMPlayState                       (InitialState+103)
#define  ExitCallStationBGMPlayState                   (InitialState+104)
// CallStation State
#define	InitialCallStationState			100
#define  CallStationNormalState							(InitialCallStationState+1)
#define  CallStationFirePlayState							(InitialCallStationState+2)
#define  CallStationExitBGMPlayState				(InitialCallStationState+3)
#define  CallStationErrorState									(InitialCallStationState+4)
#define  CallStationTalkState									(InitialCallStationState+5)
#define  CallStationBGMState									(InitialCallStationState+6)
#define  CallStationZoneState									(InitialCallStationState+7)
#define  CallStationGroupState								(InitialCallStationState+8)
#define  CallStationAllCallState								(InitialCallStationState+9)
#define  CallStationChimeUpState						(InitialCallStationState+10)
#define  CallStationChimeDownState				(InitialCallStationState+11)
#define  CallStationMPUBGMONState				(InitialCallStationState+12)
#define  CallStationMPUBGMOFFState			(InitialCallStationState+13)
#define  CallStationBGMONState							(InitialCallStationState+14)
#define  CallStationBGMOFFState						(InitialCallStationState+15)
#define  CallStationMPUTalkState							(InitialCallStationState+16)
#define  CallStationChangTalkState					(InitialCallStationState+17)
#define  CallStationNoFireState								(InitialCallStationState+18)
#define  CallStationPowerONState						(InitialCallStationState+19)
#define  CallStationZoneTalkState						(InitialCallStationState+19)
#define  CallStationZoneExitState						(InitialCallStationState+19)
#define  CallStationZone1State									(InitialCallStationState+20)


typedef enum { SYS_NORMAL = 0, 
               SYS_GROUP, 
               SYS_MANU,
               SYS_BGM,
               SYS_PREALARM,
               SYS_EMGTEL,
               SYS_FIRE,
               SYS_NOT_FIRE
             } DEF_STATUS;

typedef struct { 
                 BOOLEAN IsBGM;
                 DEF_STATUS Proc;
               } SYS_STATUS;

#ifndef _SM10_C_
extern SYS_STATUS sysStatus;
#endif

#endif
/* SM10.H END */

#ifndef _GW_MASTER_H_
#define _GW_MASTER_H_

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
#define  SetupModeSelectSpecialState		(InitialState+95)
#define  InitialCallStationZoneState		(InitialState+96)
#define  CallStationSelectZoneState			(InitialState+97)
#define  SetCallStationSelectZoneState		(InitialState+98)
#define  ResetCallStationSelectZoneState	(InitialState+99)
#define  CallStationZoneActionState			(InitialState+100)
#define  ExitCallStationZoneState			(InitialState+101)
#define  InitialCallStationBGMPlayState     (InitialState+102)
#define  CallStationBGMPlayState            (InitialState+103)
#define  ExitCallStationBGMPlayState        (InitialState+104)


#define SEND_KEY_CMD          1
#define MIC_PRESS_STATE_CMD   3
#define GROUP_CMD             4
#define EXIT_GROUP_CMD        6
#define SEND_ACK_CMD          5


#define RLED_STATUS           1
#define GLED_STATUS           2
#define CNTLED_STATUS         3
#define GRPLED_STATUS         4
#define FRLED_STATUS          5
#define FGLED_STATUS          6
#define FCNTLED_STATUS        7
#define FGRPLED_STATUS        8

#endif

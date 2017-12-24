#ifndef _P1LAYER2_H_
#define _P1LAYER2_H_
// File Name   : P1Layer2.h
// Auther      : K'un I Chen
// Create      : 07/21/2003
// version     :
// Objective   :
// Description : P1 Port 負責原生系統的溝通與連絡
// History     :
#include "common.h"
#include "uart.h"
#include "ptimer.h"


#define P1Port_Put(x)                      UART0_Put(x)
#define P1Port_Get(x)                      UART0_Get(x)
#define P1Port_Receive_Length()            UART0_R_Length
#define P1Port_Transmit_Length()           UART0_T_Length

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

#define P1Package_SYNC                   0x15
#define P1RxBuffLength                      5
//@@@@@
#define P1Package_Length                    7

#ifndef P1LAYER2_IMPLEMENT
extern bit P1PackageReceive; // bit of complete package indacate
extern bit P1SyncState;
extern idata BYTE P1RxBuff[P1RxBuffLength];
#endif

#define P1_IsReceive()                    P1SyncState

void RxP1(void);
void TxP1(char* packet, BYTE count);

#endif
#if ! defined  ( _SENSOR_H_)
#define _SENSOR_H_
#include "regw77e58.h"
#include "common.h"
#include "sm10.h"
#include "sm10io.h"

#define PHYSICAL_FIRESENSOR0    PORT0_RW
#define PHYSICAL_FIRESENSOR1    PORT1_RW   /* only use bit 0, 1 */

#define PHYSICAL_TELSENSOR0     PORT3_RW
#define PHYSICAL_TELSENSOR1     PORT1_RW   /* only use bit 4, 5 */

#define PHYSICAL_SHORTSENSOR0   PORT4_RW
#define PHYSICAL_SHORTSENSOR1   PORT1_RW   /* only use bit 6, 7 */
#define PHYSICAL_SYSTEMSET      PORT5_RW   /* PreAlram Time use bit 0 - 4, NC or NO use bit 5, High is NO */

// #define IsMicPhonePress()       ~MicPhonePTT

//sbit    MicPhonePTT  = P3 ^ 2;

void    InitialSensor(void);
void    CheckTel(void);
bit     IsTelDiff(void);
bit     IsTelNormalClose(void);
BYTE    GetPreAlarmTime(void);
BYTE    GetPreAlarmTimerSet(void);
bit     IsMicPhonePress(void);

//void    CheckFire(void);
//void    CheckShort(void);
//BOOLEAN IsAnyFireAction(void);
//BOOLEAN IsAnyLoopShort(void);
//BOOLEAN IsLoopShort(const BYTE);
//BOOLEAN IsShortDiffer(void);
//BOOLEAN IsZoneFireAction(const BYTE);
//BOOLEAN IsFireDiffer(void);
//BOOLEAN IsFireNormalClose(void);
//BOOLEAN IsTelNormalClose(void);
//void    GetFireStatus(BYTE *);
//void    GetTelStatus(BYTE *);
//void    GetShortStatus(BYTE *);
//void    ClrFireSensorState(void);
//void    ClrShortSensorState(void);
//void    ClrShortState(const BYTE);

#ifndef __SENSOR_IMPLEMENT__
extern EXTERN_DATA BYTE TelSensor[2];
extern EXTERN_DATA BYTE TelStatus[2];
#endif
/*
BOOLEAN IsMicPhonePress(void);
*/
#endif 
/* _SENSOR_H FILE END */

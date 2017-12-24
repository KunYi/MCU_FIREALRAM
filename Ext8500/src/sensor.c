#define __SENSOR_IMPLEMENT__

#include "sensor.h"
#define  RelayTable LEDTable

extern CODE_DATA BYTE LEDTable[];
volatile BYTE  EXTERN_DATA TelSensor[2];
volatile BYTE  EXTERN_DATA TelStatus[2];
static EXTERN_DATA volatile  BYTE PreTelSensor[2];
static volatile BYTE DebounceCounter;
static BYTE PPTCounter;

EXTERN_DATA BYTE PreAlramTime;

// before use the module must call the function
void InitialSensor(void)
{
  GetPreAlarmTime();
  TelSensor[0] = TelSensor[1] = 0;
  TelStatus[0] = TelStatus[1] = 0;
  PreTelSensor[0] = PreTelSensor[1] = 0;
  DebounceCounter = 0;
  PPTCounter = 0;

  CheckTel();
  CheckTel();
  CheckTel();
  CheckTel();
  IsTelDiff();

  CheckTel();
  CheckTel();
  CheckTel();
  CheckTel();
  IsTelDiff();

  CheckTel();
  CheckTel();
  CheckTel();
  CheckTel();
  IsTelDiff();

  CheckTel();
  CheckTel();
  CheckTel();
  CheckTel();
  IsTelDiff();
}

void CheckTel(void)
{
 BYTE Temp1, Temp2;

 if (IsTelNormalClose()) // sensor normal close  high level
 { 
   Temp1 = ~PHYSICAL_TELSENSOR0;
   Temp2 = ~PHYSICAL_TELSENSOR1;
 }
 else // sensor relay normal open with low level
 {
   Temp1 = PHYSICAL_TELSENSOR0;
   Temp2 = PHYSICAL_TELSENSOR1;
 }
 Temp2 = Temp2 >> 4;
 Temp2 &= 0x03;


 if ((PreTelSensor[0] != Temp1) ||
     (PreTelSensor[1] != Temp2))
 { 
   PreTelSensor[0]  = Temp1;
   PreTelSensor[1]  = Temp2;
   DebounceCounter  = 0;
 }
 else
 { 
   DebounceCounter++;
 }
 
 if (DebounceCounter > 64)
 {
     DebounceCounter = 0;
     TelSensor[0] = Temp1;
     TelSensor[1] = Temp2;
 }

}

bit IsTelDiff(void)
{
 if (( TelSensor[0] != TelStatus[0] ) || ( TelSensor[1] != TelStatus[1] ))
 { 
     TelStatus[0] = TelSensor[0];
     TelStatus[1] = TelSensor[1];
     return TRUE;
 }

 return FALSE;
}


bit IsTelNormalClose(void)
{
 return ((PreAlramTime & 0x20) == 0x20) ? TRUE : FALSE;
}

BYTE GetPreAlarmTime(void)
{
 PreAlramTime = ~PHYSICAL_SYSTEMSET;
 return (PreAlramTime & 0x0f);
}

BYTE GetPreAlarmTimerSet(void)
{
 return (PreAlramTime & 0x0f);
}


bit IsMicPhonePress(void) {
  BYTE temp;
  
  temp = PORT9_RW;
  temp &= 0x01;

  if (temp == 0x01)
  {
    PPTCounter++;

    if (PPTCounter > 20)
	  return TRUE;
  }
  else 
  { 
    PPTCounter = 0;
    return FALSE;
  }
}


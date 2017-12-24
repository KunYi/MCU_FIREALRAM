#define __IMPLEMENT_SENSOR__
#include "sensor.h"
#define  RelayTable LEDTable

extern BYTE code LEDTable[];
//extern bit IsPreEmergencyMode,IsEmergencyMode;
//extern  Status  ProcState;

static BYTE xdata FireSensor[2];
static BYTE xdata PreFireSensor[2];
static BYTE xdata PreFireStatus[2];

static BYTE xdata TelSensor[2];
       BYTE idata PreTelStatus[2];
static BYTE xdata PreTelSensor[2];

static BYTE xdata ShortSensor[2];
static BYTE xdata PreShortSensor[2];
static BYTE xdata PreShortStatus[2];

static BYTE xdata FireDebounce;
static BYTE xdata TelDebounce;
static BYTE xdata ShortDebounce;
       BYTE xdata PreAlramTime;
static bit IsRemoteMicPress;
//static bit IsShortDiff;

void InitialSensor(void)
{
  FireSensor[0] = FireSensor[1] = 0;
  TelSensor[0] = TelSensor[1] = 0;
  PreFireStatus[0] = PreFireStatus[1] = 0;
  PreTelStatus[0] = PreTelStatus[1] = 0;
  PreFireSensor[0] = PreFireSensor[1] = 0;
  PreTelSensor[0] = PreTelSensor[1] = 0;
  ShortSensor[0] = ShortSensor[1] = 0;
  PreShortStatus[0] = PreShortStatus[1] = 0;
  FireDebounce = ShortDebounce = TelDebounce = 0;
//  IsPreEmergencyMode = FALSE;
//  IsShortDiff = FALSE;
  GetPreAlarmTime();
}

BOOLEAN IsAnyFireAction(void)
{

 // Modify TelSensor
 if ((FireSensor[0] != 0) || (FireSensor[1] != 0))
 { 
   return TRUE; 
 }
 else 
 { 
   return FALSE; 
 }
}


void CheckFire(void)
{
  BYTE Temp1, Temp2;

  //scan fire state  
  if (IsFireNormalClose()) // sensor relay normal close  high level
  { 
     Temp1 = ~PHYSICAL_FIRESENSOR0;
     Temp2 = ~PHYSICAL_FIRESENSOR1;
  }
  else // sensor relay normal open with low level
  {
     Temp1 = PHYSICAL_FIRESENSOR0;
     Temp2 = PHYSICAL_FIRESENSOR1;
  }

  Temp2 &= 0x03;

//fire debounce 
  if ((PreFireSensor[0] != Temp1) ||
      (PreFireSensor[1] != Temp2))
  { 
    PreFireSensor[0] = Temp1;
    PreFireSensor[1] = Temp2;
    FireDebounce = 0;
  }
  else
  { 
    FireDebounce++; 
  }
  // release version
  if (FireDebounce > 3)
  {
     FireDebounce = 0;
     FireSensor[0] = Temp1;
     FireSensor[1] = Temp2;
//	 IsPreEmergencyMode = TRUE;
  }
}

#define COUNT_TEL_DEBOUNCE  10
void CheckTel(void)
{
 BYTE Temp1, Temp2;
//scan tel state
 if (IsTelNormalClose()) /* sensor relay normal close  high level */
 { 
     Temp1 = ~PHYSICAL_TELSENSOR0;
     Temp2 = ~PHYSICAL_TELSENSOR1;
 }
 else /* sensor relay normal open with low level */
 {
     Temp1 = PHYSICAL_TELSENSOR0;
     Temp2 = PHYSICAL_TELSENSOR1;
 }
 

 Temp2 = Temp2 >> 4;
 Temp2 &= 0x03;

//tel debounce
 if ((PreTelSensor[0] != Temp1) ||
     (PreTelSensor[1] != Temp2))
 { 
   TelDebounce = 0;
   PreTelSensor[0] = Temp1;
   PreTelSensor[1] = Temp2;
 }
 else
 { 
   if (TelDebounce <= COUNT_TEL_DEBOUNCE)
   {
       TelDebounce++; 
   }
 }

  // release version
 if (TelDebounce >= COUNT_TEL_DEBOUNCE)
 {
  //   TelDebounce = 0;
    TelSensor[0] = Temp1;
    TelSensor[1] = Temp2;
 }

}

void CheckShort(void)
{
	BYTE Temp1, Temp2;
	
	Temp1 = ~PHYSICAL_SHORTSENSOR0;
	Temp2 = ~PHYSICAL_SHORTSENSOR1;
	
	Temp2 = Temp2 >> 6;
	Temp2 &= 0x03;
	
	if ((PreShortSensor[0] != Temp1) || (PreShortSensor[1] != Temp2))
	{
		ShortDebounce = 0;
		PreShortSensor[0] = Temp1;
		PreShortSensor[1] = Temp2;
	}
	else
	{
		ShortDebounce++;
	}
 
	if (ShortDebounce > 3)
	{
		ShortDebounce = 0;
		ShortSensor[0] = Temp1  | PreShortStatus[0];
		ShortSensor[1] = Temp2  | PreShortStatus[1];
	}
}

BOOLEAN IsLoopShort(const BYTE RelayNumber)
{
  BYTE Temp1, Temp2;
  
  if (RelayNumber <= ZONE10_RELAY )
    {
      if (RelayNumber <= ZONE08_RELAY) 
        { Temp1 = 0; Temp2 = RelayNumber; }
      else 
        { Temp1 = 1; Temp2 = (RelayNumber - 8); }
      
      if ((PreShortStatus[Temp1] & RelayTable[Temp2]) == RelayTable[Temp2])
        { return TRUE; }
      else { return FALSE; }
    }
  else { return FALSE; }
}

BOOLEAN IsFireDiffer(void)
{ 
  if (( FireSensor[0] != PreFireStatus[0] ) ||( FireSensor[1] != PreFireStatus[1] ))
  { 
    PreFireStatus[0] = FireSensor[0];
    PreFireStatus[1] = FireSensor[1];
    return TRUE;
  }
  return FALSE;
}

// Added new function at 04/27/2004 by Kunyi Chen
// because change function of from EmgTel sensor to Extend Group
BOOLEAN IsExGroupDiffer(void)
{
  if (( TelSensor[0] != PreTelStatus[0] ) ||( TelSensor[1] != PreTelStatus[1] ))
  { 
    PreTelStatus[0] = TelSensor[0];
    PreTelStatus[1] = TelSensor[1];
    return TRUE;
  }
  return FALSE;
}

BOOLEAN IsShortDiffer(void)
{
// if ( IsShortDiff || ( ShortSensor[0] != PreShortStatus[0] ) ||
 if (( ShortSensor[0] != PreShortStatus[0] ) ||
     ( ShortSensor[1] != PreShortStatus[1] ))
   { 
     PreShortStatus[0] = ShortSensor[0];
     PreShortStatus[1] = ShortSensor[1];
	//  IsShortDiff = FALSE;
     return TRUE;
   }
 return FALSE;
}

BOOLEAN IsFireNormalClose(void)
{
 return ((PreAlramTime & 0x10) == 0x10) ? TRUE : FALSE;
}

BOOLEAN IsTelNormalClose(void)
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

void GetFireStatus(BYTE *Buffer)
{
 Buffer[0] = PreFireStatus[0];
 Buffer[1] = PreFireStatus[1];
}

void GetTelStatus(BYTE *Buffer0)
{
 Buffer0[0] = PreTelStatus[0];
 Buffer0[1] = PreTelStatus[1];
}

void GetShortStatus(BYTE *Buffer2)
{
 Buffer2[0] = PreShortStatus[0];
 Buffer2[1] = PreShortStatus[1];
}

void ClrFireSensorState(void)
{
 PreFireStatus[0] = 0;
 PreFireStatus[1] = 0; 
// PreTelStatus[0] = 0;
// PreTelStatus[1] = 0; 
}

void ClrShortSensorState(void)
{
 PreShortStatus[0] = 0;
 PreShortStatus[1] = 0; 
}

void ClrShortState(const BYTE Number)
{
  BYTE Temp1, Temp2;
  
  if (Number <= ZONE10_RELAY )
    {
      if (Number <= ZONE08_RELAY) 
        { Temp1 = 0; Temp2 = Number; }
      else 
        { Temp1 = 1; Temp2 = (Number - 8); }
	
	  Temp2 = ~RelayTable[Temp2];
	  ShortSensor[Temp1] &= Temp2;
	  PreShortSensor[0] = 0;
	  PreShortSensor[1] = 0;
	  ShortDebounce = 0;
    }

}

bit IsMicPhonePress(void)
{
  BYTE temp;
  temp = PORT9_RW;
	temp &= 0x01;

  
	if ((temp == 0x01) || (IsRemoteMicPress == TRUE))
    {
	  return TRUE;
    }
	else
    {
      return FALSE;
    }
}

void SetRemoteMicPress(void)
{
  IsRemoteMicPress = TRUE;
}

void ClrRemoteMicPress(void)
{
  IsRemoteMicPress = OFF;
}


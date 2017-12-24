#include "relay.h"
#include "sensor.h"
// #include "sm10.h"
#include "display.h"
#define  RelayTable LEDTable
extern CODE_DATA BYTE LEDTable[];

static EXTERN_DATA  BYTE RELAY_BUFFER[2] = { 0, 0 };
static EXTERN_DATA  StoreRelay[2];

// static BYTE SHORT_BUFFER[2] = { 0, 0 };

void RefreshRelay(void)
{
 PHYSICAL_RELAY0 = ~RELAY_BUFFER[0];
 PHYSICAL_RELAY1 = ~RELAY_BUFFER[1];
}

void SetRelay(BYTE RelayNumber)
{
	BYTE temp1, temp2;  
	if ( RelayNumber <= ZONE10_RELAY )
	{
		ClrShortState(RelayNumber);
		ClrFlash(RelayNumber+ZONE_ACTIVE_LED);
		if ( RelayNumber <= ZONE08_RELAY ) 
		{ 
			temp1 = 0; temp2 = RelayNumber; 
		}
		else 
		{ 
			temp1 = 1; temp2 = RelayNumber - 8; 
		}

	RELAY_BUFFER[temp1] |= RelayTable[temp2];
  }
}

void ClrRelay(BYTE RelayNumber)
{
  BYTE temp1, temp2;
  
  if ( RelayNumber <= ZONE10_RELAY )
    { 
      if ( RelayNumber <= ZONE08_RELAY ) 
        { temp1 = 0; temp2 = RelayNumber; }
      else 
        { temp1 = 1; temp2 = RelayNumber - 8; }
  
      RELAY_BUFFER[temp1] &= ~RelayTable[temp2];
   }
}

void SetEmergencyRelay(void)
{
  RELAY_BUFFER[1] |= 0x08;
}

void ClrEmergencyRelay(void)
{
 RELAY_BUFFER[1] &= 0xF7;
}

void SetFireStatusRelay(void)
{
 RELAY_BUFFER[1] |= 0x80;
}

void ClrFireStatusRelay(void)
{
 RELAY_BUFFER[1] &= 0x7F;
}

/* the function not use
BOOLEAN IsSetRelay(const BYTE RelayNumber)
{
 if (RelayNumber <= ZONE10_RELAY )
   { return ((RELAY_BUFFER[RelayNumber/8] & RelayTable[RelayNumber%8]) == RelayTable[RelayNumber%8]) ? TRUE : FALSE; }
 return FALSE;
}
*/
void SetRelayBuffer(const BYTE relay1, const BYTE relay2)
{
   RELAY_BUFFER[0] = relay1;
   RELAY_BUFFER[1] = relay2;  
}

// 
void StoreRelayState(void)
{
  StoreRelay[0] = RELAY_BUFFER[0];
  StoreRelay[1] = RELAY_BUFFER[1];
}

void RestoreRelayState(void)
{
  RELAY_BUFFER[0] = StoreRelay[0];
  RELAY_BUFFER[1] = StoreRelay[1];
}

/*
BOOLEAN IsShort(const BYTE RelayNumber)
{
  BYTE Temp1, Temp2;
  
  SHORT_BUFFER[0] = ~PHYSICAL_FUSE0;
  SHORT_BUFFER[1] = (~(PHYSICAL_FUSE1 | 0x3f) >> 6);

  if (RelayNumber <= ZONE10_RELAY )
    {
      if (RelayNumber <= ZONE08_RELAY) 
        { Temp1 = 0; Temp2 = RelayNumber; }
      else 
        { Temp1 = 1; Temp2 = (RelayNumber - 8); }
      
      if (SHORT_BUFFER[Temp1] & RelayTable[Temp2] == RelayTable[Temp2])
        { return TRUE; }
      else { return FALSE; }
    }
  else { return FALSE; }
}

BOOLEAN IsAnyLoopShort(void)
{ 
  BYTE temp1, temp2;
  temp1 = ~PHYSICAL_FUSE0;
  temp2 = (~(PHYSICAL_FUSE1 | 0x3f) >> 6);
  if (temp2 > 0 ) 
    { 
       SHORT_BUFFER[1] = temp2;
       SHORT_BUFFER[0] = temp1; 
       return TRUE; 
    }

  if (temp1 > 0 ) 
    { 
       SHORT_BUFFER[0] = temp1; 
       SHORT_BUFFER[1] = temp2;
       return TRUE; 
    }

  return FALSE;  
}
*/


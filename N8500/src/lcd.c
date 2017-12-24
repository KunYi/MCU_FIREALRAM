#include "lcd.h"

#ifdef _SS_COMMAND_
extern bit IsLCDChange;
#endif

EXTERN_DATA BYTE mLCDStatus;
static EXTERN_DATA BYTE  StoreLCD;


void LcdScreen(BYTE sc)
{
/*
BYTE LcdErrorCount;
BYTE LcdCount,LcdCount1; 

LcdCount = 0;
LcdCount1 = 0;
*/
#ifdef _SS_COMMAND_
IsLCDChange = TRUE;
#endif
PHYSICAL_LCD=~sc;
mLCDStatus = sc;
/*
for(LcdCount = 0;LcdCount < 10;LcdCount++){
for(LcdCount1 = 0;LcdCount1 < 200;LcdCount1++){
		}}
*/
/*
if(LcdErrorCount>=3){
	LcdErrorCount=0;
 	PHYSICAL_LCD=~LCDERR;
 	}
*/
}

/*
void StoreLCDState(void)
{
  StoreLCD = mLCDStatus;
}

void RestoreLCDState(void){
  mLCDStatus = StoreLCD;
  PHYSICAL_LCD=~mLCDStatus;
#ifdef _SS_COMMAND_
  IsLCDChange = TRUE;
#endif
}

BOOLEAN IsSlaveRead(void) {
  BYTE TEMP;
  TEMP = PHYSICAL_SLAVE_CPU_STATE & 0x04;
  if (TEMP==0x04) {  
    return FALSE; 
  } else { 
    return TRUE; 
  }
}

*/
void LCDDelay(void) {
  BYTE LcdCount,LcdCount1; 

  LcdCount = 0;
  LcdCount1 = 0;
 
  for (LcdCount = 0;LcdCount < 10;LcdCount++){
    for(LcdCount1 = 0;LcdCount1 < 200;LcdCount1++){
	}
  }
}

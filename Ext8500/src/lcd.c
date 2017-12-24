#include "lcd.h"
#define  _LCD_STOP_REFRESH  0xFFFE
#define  LCD_REF_COUNT  2000
BYTE LcdErrorCount;
EXTERN_DATA BYTE LcdCount,LcdCount1;
static EXTERN_DATA unsigned int  _recount = 0;
static EXTERN_DATA BYTE _LCDcnt = 0;
static EXTERN_DATA BYTE _Screen = 0;

void LcdScreen(BYTE sc)
{
	if (sc != _Screen)
	{
		PHYSICAL_LCD=~sc;
		LcdCount = 0;
		LcdCount1 = 0;
	
		if (sc <= 0x3F) {
			_Screen = sc;
			_LCDcnt = 0;
			_recount = 0;
		}
		
		for (LcdCount = 0;LcdCount < 10;LcdCount++) 
		{
			for(LcdCount1 = 0;LcdCount1 < 200;LcdCount1++)
			{
			}
		}
	}
}

void ReLCDScreen(void)
{	
	if (_recount == _LCD_STOP_REFRESH)
		return;

	if (_recount > LCD_REF_COUNT)
	{
		_recount = 0;
		if (_LCDcnt++ < 3)
		{
			PHYSICAL_LCD = ~_Screen;
		} else
		{
			_recount = _LCD_STOP_REFRESH;
		}
	} else 
	{
		_recount++;
	}
}


BOOLEAN IsSlaveRead(void)
{
BYTE TEMP;
TEMP = PHYSICAL_SLAVE_CPU_STATE & 0x04;
if(TEMP==0x04)
   {  return FALSE; }
 else { return TRUE; }
}
 
void LCDDelay(void) {
  BYTE LcdCount,LcdCount1; 

  LcdCount = 0;
  LcdCount1 = 0;
 
  for (LcdCount = 0;LcdCount < 10;LcdCount++) {
    for(LcdCount1 = 0;LcdCount1 < 200;LcdCount1++){
	}
  }
}















































































































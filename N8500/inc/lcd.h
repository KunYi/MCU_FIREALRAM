#if ! defined  ( _LCD_H_)
#define _LCD_H_
#include "regw77e58.h"
#include "common.h"
#include "sm10.h"
#include "sm10io.h"
extern EXTERN_DATA BYTE mLCDStatus;

#define PHYSICAL_LCD    PORT9_RW
#define PHYSICAL_SLAVE_CPU_STATE   PORT9_RW   /* only use bit 2 */
#define LCDERR 0xee
void LcdScreen(BYTE);
BOOLEAN IsSlaveRead(void);
void LCDDelay(void);
void StoreLCDState(void);
void RestoreLCDState(void);

#define LCD_QUITGROUP
#define LCD_NORMAL       0x01
#define LCD_QUIT_SETUP   0x02
#define LCD_GROUP_SETUP  0x03
#define LCD_SETUP_GROUP_ZONE  0x04
#define LCD_BGM_SETUP    0x05
#define LCD_CHK_PASSWORD 0x06
#define LCD_SETUP_SMODE  0x08
#define LCD_SETUP_FIRE   0x09
#define LCD_SETUP_FIRE_ZONE    0x0A
#define LCD_NONAME1      0x0B
#define LCD_VOICE_SETUP  0x0C
#define LCD_ENTRY_GROUP  0x0D
#define LCD_MIC          0x0E
#define LCD_ENTRY_MANU   0x0F
#define LCD_MANU_MIC     0x10
#define LCD_ENTRY_BGM    0x11
#define LCD_ENTRY_CS     0x12 // Callstation
#define LCD_ENTRY_CSBGM  0x13
#define LCD_PRE_ALARM    0x14
#define LCD_FIRE         0x15
#define LCD_NO_FIRE      0x16
#define LCD_ALLEMG       0x18
#define LCD_TESTVOICE    0x19
#define LCD_CHK_ALARM    0x1A
#define LCD_EXGROUP      0x1B

#define LCD_ZONENUMBER0  0x80
#define LCD_ZONENUMBER1  0x81
#define LCD_ZONENUMBER2  0x82
#define LCD_ZONENUMBER3  0x83
#define LCD_ZONENUMBER4  0x84
#define LCD_ZONENUMBER5  0x85
#define LCD_ZONENUMBER6  0x86
#define LCD_ZONENUMBER7  0x87
#define LCD_ZONENUMBER8  0x88
#define LCD_ZONENUMBER9  0x89
#define LCD_SHOWSTAR     0x8B

#define LCD_BACKLIGHT    0xC1

#define LCD_BL_AUTO_OFF   0xD2
#define LCD_QUIT_SETUP_SMODE  0xD3
#define LCD_BL_ON        0xD3

#endif
/* _LCD_H FILE END */ 

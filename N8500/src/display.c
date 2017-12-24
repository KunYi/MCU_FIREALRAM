#define DISPLAY_IMPLMENT
#include "display.h"

P_INTERNAL_DATA BYTE FlashRAM[5] = { 0, 0, 0, 0, 0 };
P_INTERNAL_DATA BYTE VRAM[5] = { 0, 0, 0, 0, 0 };

EXTERN_DATA BYTE StoreFlash[5];
EXTERN_DATA BYTE StoreVRAM[5];

// static P_INTERNAL_DATA BYTE RealVRAM[5];
static bit FlashFlag = 1;
CODE_DATA const BYTE LEDTable[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
CODE_DATA const BYTE ConverTable[] = {  0,  1,  2,  3,  4,  5,  6,  7, 24, 25, 
                                        8,  9, 10, 11, 12, 13, 14, 15, 26, 27,
                                       16, 17, 18, 19, 20, 21, 22, 23, 28, 29,
                                       32, 33, 34, 35, 36, 37, 38, 39, 30, 31};
// Global
#ifdef __SS_COMMAND_
extern bit IsRLEDChange;  
extern bit IsGLEDChange;
extern bit IsCLEDChange;
extern bit IsGRLEDChange;                                     
extern bit IsFRLEDChange;  
extern bit IsFGLEDChange;
extern bit IsFCLEDChange;
extern bit IsFGRLEDChange;                                     
#endif

void Refresh(void)
{
         if (FlashFlag = ~FlashFlag)
         {
             PHYSICAL_RED_LED      = ~VRAM[0];
             PHYSICAL_GREEN_LED    = ~VRAM[1];
             PHYSICAL_CONTROL_LED  = ~VRAM[2];
             PHYSICAL_MIXER_LED    = ~VRAM[3];
             PHYSICAL_GROUP_LED    = ~VRAM[4];
         }
         else
         { 
             PHYSICAL_RED_LED      = ~(VRAM[0] ^ FlashRAM[0]);
             PHYSICAL_GREEN_LED    = ~(VRAM[1] ^ FlashRAM[1]);
             PHYSICAL_GROUP_LED    = ~(VRAM[4] ^ FlashRAM[4]);
             PHYSICAL_CONTROL_LED  = ~(VRAM[2] ^ FlashRAM[2]);
             PHYSICAL_MIXER_LED    = ~(VRAM[3] ^ FlashRAM[3]);
         }
}

void SetLED(BYTE LEDNumber) { 
  BYTE temp;

  if (LEDNumber <40) {
    temp = ConverTable[LEDNumber];
    if (!((VRAM[temp/8] & LEDTable[temp%8]) == LEDTable[temp%8])) {  
      VRAM[temp/8] |= LEDTable[temp%8];

#ifdef __SS_COMMAND_
      if (LEDNumber < 10) { // RED LED Setting
        IsRLEDChange = TRUE;
      } else if (LEDNumber < 20) { // GREEN LED Setting
        IsGLEDChange = TRUE;
      } else if (LEDNumber < 30) { // CONTROL LED
        IsCLEDChange = TRUE;
      } else { // GROUP LED
        IsGRLEDChange = TRUE;
      }
#endif
    }
  }  
}

void ClrLED(BYTE LEDNumber) { 
  BYTE temp;

  if (LEDNumber < 40) {
    temp = ConverTable[LEDNumber];

    if (((VRAM[temp/8] & LEDTable[temp%8]) == LEDTable[temp%8])) {
      VRAM[temp/8] &= ~LEDTable[temp%8];
#ifdef __SS_COMMAND_
      if (LEDNumber < 10) { // RED LED Setting
        IsRLEDChange = TRUE;
      } else if (LEDNumber < 20) { // GREEN LED Setting
        IsGLEDChange = TRUE;
      } else if (LEDNumber < 30) { // CONTROL LED
        IsCLEDChange = TRUE;
      } else { // GROUP LED
        IsGRLEDChange = TRUE;
      }
#endif
    }
  }
}


BOOLEAN IsSetLED(BYTE LEDNumber) { 
  BYTE temp;
  if (LEDNumber < 40) {
    temp = ConverTable[LEDNumber];
    return ((VRAM[temp/8] & LEDTable[temp%8]) == LEDTable[temp%8]) ? TRUE : FALSE;
  }
  return FALSE;
}

void SetVRAM(BYTE RED, BYTE GREEN, BYTE Control, BYTE MIXER, BYTE GROUP) {
  VRAM[0] = RED;
  VRAM[1] = GREEN;
  VRAM[2] = Control;
  VRAM[3] = GROUP;
  VRAM[4] = MIXER;
#ifdef __SS_COMMAND_
  IsRLEDChange = TRUE;
  IsGLEDChange = TRUE;
  IsCLEDChange = TRUE;
  IsGRLEDChange = TRUE;
#endif
}

void SetFlash(BYTE LEDNumber) { 
  BYTE temp;
  if (LEDNumber < 40) {
    temp = ConverTable[LEDNumber];
    if (!((FlashRAM[temp/8] & LEDTable[temp%8]) == LEDTable[temp%8])) {
      FlashRAM[temp/8] |= LEDTable[temp%8]; 

#ifdef __SS_COMMAND_
      if (LEDNumber < 10) { // RED LED Setting
        IsFRLEDChange = TRUE;
      } else if (LEDNumber < 20) { // GREEN LED Setting
        IsFGLEDChange = TRUE;
      } else if (LEDNumber < 30) { // CONTROL LED
        IsFCLEDChange = TRUE;
      } else { // GROUP LED
        IsFGRLEDChange = TRUE;
      }
#endif
    }     
  }
}

void ClrFlash(BYTE LEDNumber) { 
  BYTE temp;
  if (LEDNumber < 40) {
    temp = ConverTable[LEDNumber];
    if ((FlashRAM[temp/8] & LEDTable[temp%8]) == LEDTable[temp%8]) {
      FlashRAM[temp/8] &= ~LEDTable[temp%8];

#ifdef __SS_COMMAND_
      if (LEDNumber < 10) { // RED LED Setting
        IsFRLEDChange = TRUE;
      } else if (LEDNumber < 20) { // GREEN LED Setting
        IsFGLEDChange = TRUE;
      } else if (LEDNumber < 30) { // CONTROL LED
        IsFCLEDChange = TRUE;
      } else { // GROUP LED
        IsFGRLEDChange = TRUE;
      }
#endif
    }
  }
}

BOOLEAN IsSetFlash(BYTE LEDNumber) { 
  BYTE temp;
  if (LEDNumber < 40) {
    temp = ConverTable[LEDNumber];
    return ((FlashRAM[temp/8] & LEDTable[temp%8]) == LEDTable[temp%8]) ? TRUE : FALSE; 
  }
  return FALSE;
}

void GetGFLEDStatus(BYTE *Buff)
{
	Buff[0] = FlashRAM[1];
	Buff[1] = ((FlashRAM[3] >> 2) & 0x03);
}

#ifdef __SS_COMMAND_
// new added for remote panel 
void StoreLEDState(void)
{
   StoreFlash[0] = FlashRAM[0];
   StoreFlash[1] = FlashRAM[1];
   StoreFlash[2] = FlashRAM[2];
   StoreFlash[3] = FlashRAM[3];
   StoreFlash[4] = FlashRAM[4];

   StoreVRAM[0]  = VRAM[0];
   StoreVRAM[1]  = VRAM[1];
   StoreVRAM[2]  = VRAM[2];
   StoreVRAM[3]  = VRAM[3];
   StoreVRAM[4]  = VRAM[4];
}

// new added for remote panel 
void RestoreLEDState(void)
{
   FlashRAM[0] = StoreFlash[0];
   FlashRAM[1] = StoreFlash[1];
   FlashRAM[2] = StoreFlash[2];
   FlashRAM[3] = StoreFlash[3];
   FlashRAM[4] = StoreFlash[4];

   VRAM[0]     = StoreVRAM[0];
   VRAM[1]     = StoreVRAM[1];
   VRAM[2]     = StoreVRAM[2];
   VRAM[3]     = StoreVRAM[3];
   VRAM[4]     = StoreVRAM[4];
 
   IsRLEDChange = TRUE;
   IsGLEDChange = TRUE;
   IsCLEDChange = TRUE;
   IsGRLEDChange = TRUE;
   IsFRLEDChange = TRUE;  
   IsFGLEDChange = TRUE;
   IsFCLEDChange = TRUE;
   IsFGRLEDChange = TRUE;    
}
#endif



















































































































#include "display.h"

static P_INTERNAL_DATA BYTE FlashRAM[5] = { 0, 0, 0, 0, 0 };
static P_INTERNAL_DATA BYTE VRAM[5] = { 0, 0, 0, 0, 0 };
static P_INTERNAL_DATA BYTE RealVRAM[5];
static bit FlashFlag = 1;
CODE_DATA const BYTE LEDTable[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

// bit 0, 1 RLED
// bit 2, 3 GLED
// bit 4, 5 CONTROL
// bit 6, 7 Group

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

CODE_DATA const BYTE ConverTable[] = {  0,  1,  2,  3,  4,  5,  6,  7, 24, 25,  // RED
                                        8,  9, 10, 11, 12, 13, 14, 15, 26, 27,  // GREED
                                       16, 17, 18, 19, 20, 21, 22, 23, 28, 29,  // CONTROL
                                       32, 33, 34, 35, 36, 37, 38, 39, 30, 31}; // GROUP
                                     
void Refresh(void)
{
   if (FlashFlag = ~FlashFlag)
   {
     PHYSICAL_RED_LED      = ~VRAM[0];
     PHYSICAL_GREEN_LED    = ~VRAM[1];
     PHYSICAL_GROUP_LED    = ~VRAM[4];
     PHYSICAL_CONTROL_LED  = ~VRAM[2];
     PHYSICAL_MIXER_LED    = ~VRAM[3];
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

void SetLED(BYTE LEDNumber)
{ 
  register temp;
  if (LEDNumber <40)
    {
     temp = ConverTable[LEDNumber];
     VRAM[temp/8] |= LEDTable[temp%8];
    }
}

void ClrLED(BYTE LEDNumber)
{ 
  register temp;
  if (LEDNumber < 40)
    {
     temp = ConverTable[LEDNumber];
     VRAM[temp/8] &= ~LEDTable[temp%8];
    }
}


BOOLEAN IsSetLED(BYTE LEDNumber)
{ 
  register temp;
  if (LEDNumber < 40)
    {
     temp = ConverTable[LEDNumber];
     return ((VRAM[temp/8] & LEDTable[temp%8]) == LEDTable[temp%8]) ? TRUE : FALSE;
    }
  return FALSE;
}

void SetVRAM(BYTE RED, BYTE GREEN, BYTE Control, BYTE MIXER, BYTE GROUP)
{
  VRAM[0] = RED;
  VRAM[1] = GREEN;
  VRAM[2] = Control;
  VRAM[3] = MIXER;  
  VRAM[4] = GROUP;
}

void SetRLED_RAM(BYTE RED, BYTE RED1)
{
  VRAM[0]  = RED;
  VRAM[3]  = RED1;
}

void SetGLED_RAM(BYTE GREEN, BYTE GREEN1)
{
  VRAM[1]  = GREEN;
  VRAM[3]  = GREEN1;
}

void OR_GLED(BYTE GREEN, BYTE GREEN1) {
  VRAM[1] |= GREEN;
  VRAM[3] |= (GREEN1 & (BIT2 | BIT3));
}

void ClearGLED(void) {
  // Group  Bit 6, 7
  // Green  Bit 2, 3
  VRAM[1] = 0;
  VRAM[3] &= ~0xCC;
  VRAM[4] = 0;
}

void SetCNTLED_RAM(BYTE CNT, BYTE CNT1)
{
  VRAM[2]  = CNT;
  VRAM[3]  = CNT1;
}

void SetGROUPLED_RAM(BYTE GROUP, BYTE GROUP1)
{
  VRAM[4]  = GROUP;
  VRAM[3]  = GROUP1;
}

void SetFRLED_RAM(BYTE RED, BYTE RED1)
{
  FlashRAM[0]  = RED;
  FlashRAM[3]  = RED1;
}

void SetFGLED_RAM(BYTE GREEN, BYTE GREEN1)
{
  FlashRAM[1]  =  GREEN;
  FlashRAM[3]  &= ~0x0C;
  FlashRAM[3]  |= (GREEN1 << 2);
}

void SetFCLED_RAM(BYTE CNT, BYTE CNT1)
{
  FlashRAM[2]  = CNT;
  FlashRAM[3]  = CNT1;
}

void SetFGPLED_RAM(BYTE GROUP, BYTE GROUP1)
{
  FlashRAM[4]  = GROUP;
  FlashRAM[3]  = GROUP1;
}

void SetFlash(BYTE LEDNumber)
{ 
  register temp;
  if (LEDNumber < 40)
  {
     temp = ConverTable[LEDNumber];
     FlashRAM[temp/8] |= LEDTable[temp%8]; 
  }
}

void ClrFlash(BYTE LEDNumber)
{ 
  register temp;
  if (LEDNumber < 40)
    {
     temp = ConverTable[LEDNumber];
     FlashRAM[temp/8] &= ~LEDTable[temp%8]; 
    }
}

BOOLEAN IsSetFlash(BYTE LEDNumber)
{ 
  register temp;
  if (LEDNumber < 40)
    {
     temp = ConverTable[LEDNumber];
     return ((FlashRAM[temp/8] & LEDTable[temp%8]) == LEDTable[temp%8]) ? TRUE : FALSE; 
    }
  return FALSE;
}





















































































































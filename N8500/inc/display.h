#if ! defined ( _DISPLAY_H_ )
#define _DISPLAY_H_
#include "common.h"
#include "sm10io.h"
//#define MaxLED 24

#define ClrAllLED() SetVRAM((BYTE)0,(BYTE)0,(BYTE)0, (BYTE)0, (BYTE)0)

#ifdef _DEBUG_CANCEL_WRITE_IO_
  #define  PHYSICAL_RED_LED      CANCEL_IO
  #define  PHYSICAL_MIXER_LED    CANCEL_IO
  #define  PHYSICAL_GREEN_LED    CANCEL_IO
  #define  PHYSICAL_GROUP_LED    CANCEL_IO
  #define  PHYSICAL_CONTROL_LED  CANCEL_IO
#else
  #define  PHYSICAL_RED_LED      PORT0_RW
  #define  PHYSICAL_MIXER_LED    PORT1_RW
  #define  PHYSICAL_GREEN_LED    PORT2_RW
  #define  PHYSICAL_GROUP_LED    PORT7_RW
  #define  PHYSICAL_CONTROL_LED  PORT8_RW
#endif

void SetLED(BYTE);
void ClrLED(BYTE);
BOOLEAN IsSetLED(BYTE);
void SetFlash(BYTE);
void ClrFlash(BYTE);
BOOLEAN IsFlash(BYTE);
void SetVRAM(BYTE, BYTE, BYTE, BYTE, BYTE);
void SetFlashRAM(DWORD);
void Refresh(void);
void StoreLEDState(void);
void RestoreLEDState(void);
void GetGFLEDStatus(BYTE *Buff);

// export variable
#ifndef DISPLAY_IMPLMENT

extern P_INTERNAL_DATA BYTE FlashRAM[5];
extern P_INTERNAL_DATA BYTE VRAM[5];

#define RED_LED1   (VRAM[0])
#define RED_LED2   (VRAM[3])
#define GREEN_LED1 (VRAM[1])
#define GREEN_LED2 (VRAM[3])
#define CNT_LED1   (VRAM[2])
#define CNT_LED2   (VRAM[3])
#define GROUP_LED1 (VRAM[4])
#define GROUP_LED2 (VRAM[3])

#define FRED_LED1   (FlashRAM[0])
#define FRED_LED2   (FlashRAM[3])
#define FGREEN_LED1 (FlashRAM[1])
#define FGREEN_LED2 (FlashRAM[3])
#define FCNT_LED1   (FlashRAM[2])
#define FCNT_LED2   (FlashRAM[3])
#define FGROUP_LED1 (FlashRAM[4])
#define FGROUP_LED2 (FlashRAM[3])


#endif

#endif
/* DISPLAY.H FILE END */ 

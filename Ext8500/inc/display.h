#ifndef  _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"
#include "sm10io.h"

#define ClrAllLED() SetVRAM((BYTE)0,(BYTE)0,(BYTE)0, (BYTE)0, (BYTE)0)

#define  PHYSICAL_RED_LED      PORT0_RW
#define  PHYSICAL_MIXER_LED    PORT1_RW
#define  PHYSICAL_GREEN_LED    PORT2_RW
#define  PHYSICAL_GROUP_LED    PORT7_RW
#define  PHYSICAL_CONTROL_LED  PORT8_RW

void SetLED(BYTE);
void ClrLED(BYTE);
BOOLEAN IsSetLED(BYTE);
void SetFlash(BYTE);
void ClrFlash(BYTE);
BOOLEAN IsFlash(BYTE);
void SetVRAM(BYTE, BYTE, BYTE, BYTE, BYTE);
void SetFlashRAM(DWORD);
void Refresh(void);

// Extend function
void SetRLED_RAM(BYTE RED, BYTE RED1);
void SetGLED_RAM(BYTE GREEN, BYTE GREEN1);
void OR_GLED(BYTE GREEN, BYTE GREEN1);

void ClearGLED(void);

void SetCNTLED_RAM(BYTE CNT, BYTE CNT1);
void SetGROUPLED_RAM(BYTE GROUP, BYTE GROUP1);

void SetFRLED_RAM(BYTE RED, BYTE RED1);
void SetFGLED_RAM(BYTE GREEN, BYTE GREEN1);

void SetFCLED_RAM(BYTE CNT, BYTE CNT1);
void SetFGPLED_RAM(BYTE GROUP, BYTE GROUP1);


#endif
/* DISPLAY.H FILE END */ 

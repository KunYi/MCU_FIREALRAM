#ifndef _MISC_H_
#define _MISC_H_

#include "common.h"
#include "sm10io.h"

#define PHYSICAL_SYSTEMSET      PORT5_RW

// for audio select
#define PHYSICAL_AUDIO_SOURCE_SELECT   PORT5_RW
#define DIGITAL_AUDIO       1
#define CALLSTATION_AUDIO   2
#define MIC_AUDIO           3
#define BACKGROUND_AUDIO    4
#define MUTE_AUDIO          5
#define ALLCLOSE_AUDIO      6

// Get Communication ID
BYTE GetCommID(void);
void SelectAudioSource(const BYTE);

  #ifdef _IMP_MISC_VAR_
    BYTE MyID;
  #else
    extern BYTE MyID;
  #endif

#define GMASTER_ID  0x80

#define CHK_ID_TIME   32
#endif // end of file
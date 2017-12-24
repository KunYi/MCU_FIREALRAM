#define _IMP_MISC_VAR_

#include "misc.h"
#include "bitio.h"

// Get Communication ID 
BYTE GetCommID(void) {
  BYTE ID, PreID, i;

  for (i= 0; i < CHK_ID_TIME; i++) {
    PreID = ~PHYSICAL_SYSTEMSET & 0x0F;
    if (ID != PreID) { 
      i = 0; ID = PreID;
    }
  }
  return ID;
}


// added 1/13, 2006 implement audio select
void SelectAudioSource(const BYTE item)
{
   BYTE temp;
   switch(item) {
   case  DIGITAL_AUDIO:
        temp = ~0x11;
        break;
   case  CALLSTATION_AUDIO:
        temp = ~0x12;
        break;
   case  MIC_AUDIO:
        temp = ~0x14;
        break;
   case  BACKGROUND_AUDIO:
        temp = ~0x18;
        break;        
   case  MUTE_AUDIO:
        temp = 0xEF;
        break;
   case  ALLCLOSE_AUDIO:
   default:
        temp = 0xEF;
        break;
  }  
  PHYSICAL_AUDIO_SOURCE_SELECT = temp;
}
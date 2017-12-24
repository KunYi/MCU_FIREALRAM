#include "speech.h"
#include "sm10.h"

static EXTERN_DATA BYTE PlayLength = 0;
static EXTERN_DATA BYTE PlayItem = 0;
static EXTERN_DATA BYTE PlayerDebounce;
static EXTERN_DATA BYTE VoiceMessagePlayFIFO[PlayFIFOMaxLength];

void SpeechPlay(const BYTE SpeechNo)
{
  MSM6550_ST = HI;
  CONTROL_PORT_MSM6550 = (SpeechNo | 0x80);
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = LOW;
  MSM6550_ST = LOW;
  MSM6550_ST = LOW;
  MSM6550_ST = HI;
}

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

void SpeechCmd(const BYTE COMMAND)
{
  MSM6550_ST = HI;
  CONTROL_PORT_MSM6550 = (COMMAND & 0x7f);
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = HI;
  MSM6550_ST = LOW;
  MSM6550_ST = LOW;
  MSM6550_ST = LOW;
  MSM6550_ST = HI;
}

BOOLEAN Speech_Put(const BYTE Context)
{  
  if (PlayLength >= PlayItem)
    {   
     if (!((PlayLength - PlayItem) < PlayFIFOMaxLength))
       { return FALSE; }     
    }
  else 
    { 
     if (!((PlayItem - PlayLength) < PlayFIFOMaxLength))
       { return FALSE; }    
    }

  PlayLength++;
  if (PlayLength == PlayFIFOMaxLength)
    { PlayLength = 0; }
  VoiceMessagePlayFIFO[PlayLength] = Context;
  return TRUE;
        
}

BOOLEAN Speech_Get(BYTE *Context)
{
  if (PlayLength != PlayItem)
    {
      PlayItem++;
      if (PlayItem == PlayFIFOMaxLength)
        { PlayItem = 0; }
     *(Context) = VoiceMessagePlayFIFO[PlayItem];
      return TRUE;
    } 
  return FALSE;
}

BOOLEAN IsSpeechFIFOEmpty(void)
{
  return (PlayLength == PlayItem) ? TRUE : FALSE;
}

void ClearSpeechFIFO(void)
{
  PlayLength = PlayItem = 0;
}

BOOLEAN IsSpeechPlay(void)
{
  if (MSM6550_BUY)
    {
      if (PlayerDebounce <= 20)
        {
          PlayerDebounce++;
          return TRUE;
        }
      else 
        { 
          return FALSE; 
        }  
    }
  else
    {
      PlayerDebounce = 0;
      return TRUE;
    }    
}

void SpeechPlayer(void)
{
  BYTE temp;
  if (!IsSpeechPlay() && !IsSpeechBusy())
    {
     if (Speech_Get(&temp))
       {        
        if ((temp > 0) && (temp < 128))
          {        
           SpeechPlay(temp);
          }
       }
   }
}


































































































































































































































#if ! defined( _SPEECH_H_ )
#define _SPEECH_H_
#include "regw77e58.h"
#include "common.h"
#include "bitio.h"
#include "sm10io.h"

#define SpeechNull        0
#define SpeechTen        11
#define SpeechFifth      15
#define SpeechHundred    20
#define EngSpeechHundred 93
#define EngSpeechTweenty 77
#define SpeechTen1       21
#define SpeechA          22
#define TitleName1       35
/*
#define TitleName2       49
#define TitleName3       50
#define TitleName4       51
#define TitleName5       52
#define TitleName6       53
#define TitleName7       54
#define TitleName8       55
#define TitleName9       56
#define TitleName10      57
*/
#define UnitName1        38//58
/*
#define UnitName2        59
#define UnitName3        60
#define UnitName4        61
#define UnitName5        62
#define UnitName6        63
#define UnitName7        64
#define UnitName8        65
#define UnitName9        66
#define UnitName10       67
*/
#define EngFire_Fire     99
#define Fire_Fire        42//68
#define EngCare_Care	 101
#define Care_Care        43//69
#define EngFire_Action	 100
#define EngFire_Fire     99
#define Fire_Action      44//70
#define EmergencyExit    45//71
#define EngEmergencyExit 102
#define SensorAction     46//72
#define EngSensorAction	 103
#define NoFireAction     47//73
#define EngNoFireAction  104
#define Chime1           48//74
#define Chime2           49//75
#define Chime3           50//76
#define Chime4           51//77
#define Alarm1           52//78
#define Alarm2           53//79
#define Alarm3           54//80
#define Alarm4           55//81

#define PHYSICAL_AUDIO_SOURCE_SELECT   PORT5_RW
#define DIGITAL_AUDIO       1
#define CALLSTATION_AUDIO   2
#define MIC_AUDIO           3
#define BACKGROUND_AUDIO    4
#define MUTE_AUDIO          5
#define ALLCLOSE_AUDIO      6

#define CONTROL_PORT_MSM6550           PORT6_RW
#define InitialSpeechChip() SpeechCmd(0)
                            
// #define IsSpeechPlay() (!MSM6550_BUY)
#define IsSpeechBusy() (!MSM6550_NAR)
#define SpeechStop()   SpeechPlay(0)
#define PlayFIFOMaxLength          64
void    SpeechPlay(const BYTE);
BOOLEAN Speech_Put(const BYTE);
BOOLEAN Speech_Get(BYTE *);
BOOLEAN IsSpeechFIFOEmpty(void);
BOOLEAN IsSpeechPlay(void);
void    ClearSpeechFIFO(void);
void    SpeechPlayer(void);
void    SpeechCmd(const BYTE);
void    SelectAudioSource(const BYTE);
#endif 
/* SPEECH.H FILE END */
 

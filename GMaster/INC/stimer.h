#if ! defined (_STIMER_H_)
#define _STIMER_H_
#include "common.h"
#define INC_TMSec(SoftTimer) (++SoftTimer.m10sec)
#define INC_Sec(SoftTimer) (++SoftTimer.sec)
#define INC_Min(SoftTimer) (++SoftTimer.min)
#define CLR_TMSec(SoftTimer) (SoftTimer.m10sec = 0)
#define CLR_Sec(SoftTimer) (SoftTimer.sec = 0)
#define CLR_Min(SoftTimer) (SoftTimer.min = 0)
#define InitialSoftTimer(SoftTimer) CLR_TMSec(SoftTimer); \
                                    CLR_Sec(SoftTimer);   \
                                    CLR_Min(SoftTimer)
typedef struct {
                 BYTE  m10sec;
                 BYTE     sec;
                 BYTE     min;
               } SoftTimer;
/*               
void INC_Sec(SoftTimer *);
void INC_Min(SoftTimer *);
void INC_TMSec(SoftTimer *);
*/
#endif
/* STIMER.H END */

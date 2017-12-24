#ifndef _SYS_L1_H_
#define _SYS_L1_H_

#define SystemTimerProc()                             \
{                                                     \
   /* process of Timer */                             \
  if (Timer1Flag) { /* run once of pre-microsecond */ \
    OSTimer.MicroSecond++;                            \
    Timer1Flag = false;                               \
                                                      \
    if (++m10Tick > 9) {                              \
      m10Tick = 0;                                    \
      INC_TMSec(softtimer1);                          \
      INC_TMSec(systimer);                            \
                                                      \
      /* 1000 milli second */                         \
      if (softtimer1.m10sec > 99) {                   \
        CLR_TMSec(softtimer1);                        \
        INC_Sec(softtimer1);                          \
                                                      \
        /* 60 second */                               \
        if (softtimer1.sec > 59)  {                   \
          CLR_Sec(softtimer1);                        \
          INC_Min(softtimer1);                        \
           /* 60 minute */                            \
           if (softtimer1.min > 59) {                 \
             CLR_Min(softtimer1);                     \
           }                                          \
        }                                             \
      }                                               \
                                                      \
      /* 1000 milli second */                         \
      if (systimer.m10sec > 99) {                     \
        CLR_TMSec(systimer);                          \
        INC_Sec(systimer);                            \
                                                      \
        /* 60 second */                               \
        if (systimer.sec > 59) {                      \
          CLR_Sec(systimer);                          \
          INC_Min(systimer);                          \
                                                      \
          /* 60 minute */                             \
          if (systimer.min > 59) {                    \
             CLR_Min(systimer);                       \
          }                                           \
        }                                             \
      }                                               \
    }                                                 \
  }                                                   \
}

#endif

#if ! defined  (PHYSICAL_TIMER_H)
#define PHYSICAL_TIMER_H
#include "common.h"
#include "regw77e58.h"
/*    use this file timer0 object need
      define Clock Frequency
      define Period
      define Max CallBack                      */
      
/* define time characteristics (Clock FREQUENCY in MHz) */
#define FREQUENCY  22.118
/* is 22.118MHz one tick 0.54254us */
/* 185 tick = 100us                */
/* 1843 tick = 1 ms                */
#define PERIOD        1838L
#define Max_CallBack_Count 5
#if ! defined(PERIOD)
#error "define PERIOD value,Please"
#else
#define Init_Physical_Timer0()             TH0 = ((65536L - PERIOD) / 256); TL0 = ((65536L - PERIOD) % 256); TMOD |= 0x01
#define Start_Timer0()                     TR0 = 1
#define Enable_Timer0_Interrupt()          ET0 = 1
#define ReloadTimer0()                     TH0 = ((65536L - PERIOD) / 256); TL0 = ((65536L - PERIOD) % 256)
#endif
void Physical_Timer_Interrupt(void);

#define CreaterCallBack(Tick,FunName,NextNode)  CallBack WaitCall = {(FunName), (Tick), (&NextNode)}; \
                                                OSTimer->wait_call = &WaitCall

#define GetSystemTimer()                   OSTimer.MicroSecond
                                                
typedef struct CallNode {
                   void (*Funtion)(void);
                   WORD RunTick;
                   WORD tick;
                 } CallBack ;
                 
typedef struct PHYSICAL_Timer {
                                 WORD MicroSecond;
                              } PTimer;
#if ! defined( _P_CallBack_ )                              
// extern CallBack *P_CallBack[Max_CallBack_Count];
extern bit Timer1Flag;
extern PTimer OSTimer;
#endif

#endif
/* PTIMER.H FILE END */

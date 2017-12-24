#ifndef __CALL_SOFT_TIMER_H_
#define __CALL_SOFT_TIMER_H_
#include "common.h"
#include "stimer.h"

#ifndef __IMPLEMENT_CALL_SOFT_TIMER_
extern bit IsStartSoftTimer1;
extern bit IsStartSoftTimer2;
extern bit IsStartSoftTimer3;
extern bit IsStartSetupSoftTimer;
extern bit IsStartSystemSoftTimer;

extern P_INTERNAL_DATA SoftTimer SoftTimer1;
extern P_INTERNAL_DATA SoftTimer SoftTimer2;
extern P_INTERNAL_DATA SoftTimer SoftTimer3;
extern P_INTERNAL_DATA SoftTimer SetupSoftTimer;
extern P_INTERNAL_DATA SoftTimer SystemSoftTimer;
#endif

void CallSoftTimer(void);
#endif

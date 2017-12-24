#if ! defined (_BIT_IO_H_)
#define _BIT_IO_H_
#include "regw77e58.h"
sbit WatchDog = P3 ^ 5;
sbit DEVICE_Enable = P1 ^ 7;
sbit MSM6550_ST = P1 ^ 6;
sbit MSM6550_BUY = P1 ^ 5;
sbit MSM6550_NAR = P1 ^ 4;
#endif 

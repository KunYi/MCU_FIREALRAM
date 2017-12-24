#define  _P_CallBack_
#include "ptimer.h"
 
CallBack *P_CallBack[Max_CallBack_Count];
PTimer OSTimer = {0};
void Physical_Timer_Interrupt(void) interrupt 1 
{
 int i = 0;
 if (++OSTimer.MicroSecond == 65535)
   { OSTimer.MicroSecond = 0; }
   
 while ( P_CallBack[i] != 0)
    {      
       if (++(P_CallBack[i]->tick) == P_CallBack[i]->RunTick)
       {   
         P_CallBack[i]->tick = 0; 
         P_CallBack[i]->Funtion();
       }
       i++;   
    }
    ReloadTimer0();
}
                 

/*
void Test1(void);
void Test2(void);
void Test3(void);
sbit WatchDog = P3 ^ 5;
sbit DEVICE_Enable = P1 ^ 7;
   CallBack CallFun = { Test2, 250, 0};
   CallBack InitCall = { Test1,150, 0};
*/   
/*
void main (void)
{
//   int i;
   CallBack Test;
   WatchDog = ~WatchDog;
   Test.Funtion = Test3;
   Test.RunTick = 500;
   Test.tick = 0;
   P_CallBack[0] =  &InitCall;
   P_CallBack[1] =  &CallFun;
   P_CallBack[2] =  &Test;
   P_CallBack[3] = 0;
   DIRECT_BYTE_IO(0x2000) = (BYTE)0xff;
   DIRECT_BYTE_IO(0x2020) = (BYTE)0xff;
   DIRECT_BYTE_IO(0x2040) = (BYTE)0xff;
   DIRECT_BYTE_IO(0x20E0) = (BYTE)0xff;
   WatchDog = ~WatchDog;   
   DEVICE_Enable = 0;
   WatchDog = ~WatchDog;
                                         */
   /* set timer period, select mode 1 */
  	// Init_Physical_Timer0;                 
	/* start timer 0 */
	// Start_Timer0;
	/* enable timer 0 interrupt */
	// Enable_Timer0_Interrupt;
	/* global interrupt enable */
	// EA = 1;
/*	
 while(TRUE)
 {
   WatchDog = ~WatchDog;
 }
}
*/
/*
void Test1(void)
{
 static bit TestFlag = 0;
 if ( TestFlag)
    { DIRECT_BYTE_IO(0x2000) = 0x55; }
 else
    { DIRECT_BYTE_IO(0x2000) = 0xAA; }
    
 TestFlag = ~TestFlag;     
}

void Test2(void)
{
 static bit TestFlag = 0; 
 if ( TestFlag)
    { DIRECT_BYTE_IO(0x2040) = 0xAA; }
 else
    { DIRECT_BYTE_IO(0x2040) = 0x55; }
    
 TestFlag = ~TestFlag;     

} 

void Test3(void)
{
 static bit TestFlag = 0;
 if (TestFlag)
    { DIRECT_BYTE_IO(0x20E0) = 0x55; }
 else
    { DIRECT_BYTE_IO(0x20E0) = 0xAA; }
    
 TestFlag = ~TestFlag;     
}
 */

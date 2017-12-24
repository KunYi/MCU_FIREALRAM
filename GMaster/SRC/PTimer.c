#define  _P_CallBack_
#include "ptimer.h"

// CallBack *P_CallBack[Max_CallBack_Count];
PTimer OSTimer = {0};
bit  Timer1Flag = false;
void Physical_Timer_Interrupt(void) interrupt 1 
{
    //OSTimer.MicroSecond++;
    Timer1Flag = true;
    
    
    ReloadTimer0();
}

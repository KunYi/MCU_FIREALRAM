#define __IMPLEMENT_CALL_SOFT_TIMER_
#include "CallSoftTimer.h"
bit IsStartSoftTimer1;
bit IsStartSoftTimer2;
bit IsStartSoftTimer3;
bit IsStartSetupSoftTimer;
bit IsStartSystemSoftTimer;

P_INTERNAL_DATA SoftTimer SoftTimer1;
P_INTERNAL_DATA SoftTimer SoftTimer2;
P_INTERNAL_DATA SoftTimer SoftTimer3;
P_INTERNAL_DATA SoftTimer SetupSoftTimer;
P_INTERNAL_DATA SoftTimer SystemSoftTimer;


void CallSoftTimer(void) {  
  if (IsStartSoftTimer1) {
    INC_TMSec(SoftTimer1);
    if ((SoftTimer1.m10sec == 100))  { 
      CLR_TMSec(SoftTimer1); INC_Sec(SoftTimer1); 
      if ((SoftTimer1.sec == 60)) {
        CLR_Sec(SoftTimer1);
        INC_Min(SoftTimer1);
        if ( (SoftTimer1.min == 30) ) { CLR_Min(SoftTimer1); }
        }    
      }
    }
     
  if (IsStartSoftTimer2)
    { 
     INC_TMSec(SoftTimer2);
     if ( (SoftTimer2.m10sec == 100) ) 
       { 
        CLR_TMSec(SoftTimer2); INC_Sec(SoftTimer2);
        if ((SoftTimer2.sec == 60))
          { CLR_Sec(SoftTimer2);
            INC_Min(SoftTimer2);
            if ( (SoftTimer2.min == 30) ) { CLR_Min(SoftTimer2); }
          }    
       }
     }

  if (IsStartSoftTimer3)
    { 
     INC_TMSec(SoftTimer3);
     if ( (SoftTimer3.m10sec == 100) ) 
       { 
        CLR_TMSec(SoftTimer3); INC_Sec(SoftTimer3);
        if ((SoftTimer3.sec == 60))
          { CLR_Sec(SoftTimer3);
            INC_Min(SoftTimer3);
            if ( (SoftTimer3.min == 30) ) { CLR_Min(SoftTimer3); }
          }    
       }
     }

  if (IsStartSetupSoftTimer)
    { 
     INC_TMSec(SetupSoftTimer);
     if ( (SetupSoftTimer.m10sec == 100) ) 
       { 
        CLR_TMSec(SetupSoftTimer); INC_Sec(SetupSoftTimer);
        if ((SetupSoftTimer.sec == 60))
          { CLR_Sec(SetupSoftTimer);
            INC_Min(SetupSoftTimer);
            if ( (SetupSoftTimer.min == 30) ) { CLR_Min(SetupSoftTimer); }
          }    
       }
     }

  if (IsStartSystemSoftTimer)
    { 
     INC_TMSec(SystemSoftTimer);
     if ( (SystemSoftTimer.m10sec > 100) ) 
       { 
        CLR_TMSec(SystemSoftTimer); 
       }
     }
} /* CallSoftTimer() function end */
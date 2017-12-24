#include "stimer.h"

extern bit IsStartSoftTimer1;
extern bit IsStartSoftTimer2; 
extern bit IsStartSoftTimer3;
extern bit IsStartSetupSoftTimer; 
extern bit IsStartSystemSoftTimer;

extern P_INTERNAL_DATA SoftTimer SoftTimer1;
extern P_INTERNAL_DATA SoftTimer SoftTimer2;
extern P_INTERNAL_DATA SoftTimer SoftTimer3;
//static P_INTERNAL_DATA SoftTimer TxStatusTiemr;
extern P_INTERNAL_DATA SoftTimer SetupSoftTimer;
extern P_INTERNAL_DATA SoftTimer SystemSoftTimer;

void CallSoftTimer(void) {  
  if (IsStartSoftTimer1) {
    INC_TMSec(SoftTimer1);
    if ((SoftTimer1.m10sec == 100)) { 
      CLR_TMSec(SoftTimer1); INC_Sec(SoftTimer1); 
      if ((SoftTimer1.sec == 60)) { 
        CLR_Sec(SoftTimer1);
        INC_Min(SoftTimer1);
        if ( (SoftTimer1.min == 30) ) { 
          CLR_Min(SoftTimer1); 
        }
      }    
    }
  }
     
  if (IsStartSoftTimer2)  { 
    INC_TMSec(SoftTimer2);
    if ( (SoftTimer2.m10sec == 100) ) { 
      CLR_TMSec(SoftTimer2); 
      INC_Sec(SoftTimer2);
      if ((SoftTimer2.sec == 60)) { 
        CLR_Sec(SoftTimer2);
        INC_Min(SoftTimer2);
        if ( (SoftTimer2.min == 30) ) { 
          CLR_Min(SoftTimer2); 
        }
      }    
    }
  }

  if (IsStartSoftTimer3) { 
    INC_TMSec(SoftTimer3);
    if ( (SoftTimer3.m10sec == 100) )  { 
      CLR_TMSec(SoftTimer3); INC_Sec(SoftTimer3);
      if ((SoftTimer3.sec == 60)) { 
        CLR_Sec(SoftTimer3);
        INC_Min(SoftTimer3);
        if ( (SoftTimer3.min == 30) ) { 
          CLR_Min(SoftTimer3); 
        }
      }    
    }
  }

  if (IsStartSetupSoftTimer) { 
    INC_TMSec(SetupSoftTimer);
    if ( (SetupSoftTimer.m10sec == 100) ) { 
      CLR_TMSec(SetupSoftTimer); 
      INC_Sec(SetupSoftTimer);
      if ((SetupSoftTimer.sec == 60)) { 
        CLR_Sec(SetupSoftTimer);
        INC_Min(SetupSoftTimer);
        if ( (SetupSoftTimer.min == 30) ) { 
          CLR_Min(SetupSoftTimer); 
        }
      }    
    }
  }
  
  if (IsStartSystemSoftTimer) { 
    INC_TMSec(SystemSoftTimer);
    if ( (SystemSoftTimer.m10sec > 100) ) { 
      CLR_TMSec(SystemSoftTimer); 
    }
  }

#ifdef DEBUG_TIMER
  if (IsDebugTimer) { 
    INC_TMSec(DebugTimer);
    if ( (DebugTimer.m10sec > 100) ) { 
      CLR_TMSec(DebugTimer); 
      INC_Sec(DebugTimer);

      if ((DebugTimer.sec == 60)) {
        CLR_Sec(DebugTimer);
        INC_Min(DebugTimer);
          
        if (DebugTimer.min == 30) {
          CLR_Min(DebugTimer);
        }
      }
    }     
  }
#endif
} // CallSoftTimer() function end
// Action / Confirm & Autoalarm 
  if (P1RxBuff[2] == 'A')  {
    if ((P1RxBuff[3] == 'W') && (P1RxBuff[4] == 'H') &&  (P1RxBuff[5] == 'A') && (P1RxBuff[6] == 'T')) {
      // re-send
      // No. 1
      // No. 2        
    }
    else if ((P1RxBuff[3] == 0) && (P1RxBuff[4] == 0) && (P1RxBuff[5] == 0) && (P1RxBuff[6] == 0))  {
    // System initial
    // No. 03
    // main unit broadcast
    // implement process
    // initial
      PutToQue(P1FIFOIn, SYSTEM_RESET);
    }
  }  
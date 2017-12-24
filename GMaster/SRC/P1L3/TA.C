  // Action / Confirm & Autoalarm
  if (P1RxBuff[2] == 'A') {  
    if (P1RxBuff[4] == 0) {   
      if (P1RxBuff[3] < 16) {      
        PutToQue(P1FIFOIn, EMGTEL_PROC);  //
        PutToQue(P1FIFOIn, P1RxBuff[3]); // ID
        PutToQue(P1FIFOIn, P1RxBuff[5]); // 10 bits Flash data
        PutToQue(P1FIFOIn, P1RxBuff[6]);         
      }
    }
  }

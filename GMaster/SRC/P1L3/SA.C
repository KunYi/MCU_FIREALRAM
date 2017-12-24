  // Action / Confirm & Autoalarm
  if (P1RxBuff[2] == 'A') {  
    if (P1RxBuff[4] == 0)  { 
      if (P1RxBuff[3] < 16) {
        PutToQue(P1FIFOIn, SHORT_PROC);  //
        PutToQue(P1FIFOIn, P1RxBuff[3]); // ID
        PutToQue(P1FIFOIn, P1RxBuff[5]); // 10 bits Flash data
        PutToQue(P1FIFOIn, P1RxBuff[6]); 
      }
    }
  } else if (P1RxBuff[2] == 'R') {
     if (P1RxBuff[0] == 0x40) {
       PutToQue(P1FIFOIn, XFUNC_PROC);
       PutToQue(P1FIFOIn, P1RxBuff[4]);  // Type
       PutToQue(P1FIFOIn, P1RxBuff[3]);  // ReponseID
       PutToQue(P1FIFOIn, P1RxBuff[6]);  // Key Value 
     }
  } else if (P1RxBuff[2] == 'S') {
     switch(P1RxBuff[4]) {
     case 1:  // red LED status
     case 2:  // green LED status
     case 3:  // control LED status
     case 4:  // group LED status
     case 5:  // flash red LED status
     case 6:  // flash green LED status
     case 7:  // flash control LED status
     case 8:  // flash control LED status
     case 9:  // LCD Status
       if (P1RxBuff[3] != 0) { 
         // Master not response
         ResponseP1ACK(P1RxBuff[3], P1RxBuff[4]);
       }
       PutToQue(P1FIFOIn, DISPLAY_PROC);
       PutToQue(P1FIFOIn, P1RxBuff[3]);  // ID
       PutToQue(P1FIFOIn, P1RxBuff[4]);  // Type
       PutToQue(P1FIFOIn, P1RxBuff[5]);  // byte 1
       PutToQue(P1FIFOIn, P1RxBuff[6]);  // byte 2
       break;
     }
  }

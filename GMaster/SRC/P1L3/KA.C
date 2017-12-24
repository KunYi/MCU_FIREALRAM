   // Action / Confirm & Autoalarm
   if (P1RxBuff[2] == 'A') {
     if (P1RxBuff[4] == 0) {
       // Set extend unit ID */
       if ((P1RxBuff[3] < 16) && ((P1RxBuff[5] <= 10) && (P1RxBuff[5] > 0))) { 
         PutToQue(P1FIFOIn, KEY_PROC);

         PutToQue(P1FIFOIn, P1RxBuff[3]); // send unit
         PutToQue(P1FIFOIn, P1RxBuff[5]); // Key

         if (P1RxBuff[6] == 0) {
           PutToQue(P1FIFOIn, 0);
         } else {
           PutToQue(P1FIFOIn, 0xff);
         }
       }
     }
   }

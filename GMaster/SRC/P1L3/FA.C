  // Extend unit action fire alarm
  if (P1RxBuff[2] == 'A') {       
    if (P1RxBuff[4] == 0) {  // Get fire alarm action extend unit ID 
      // Fire_alarm_ID = P1RxBuff[3]     
      // Get extend unit fire alarm state     
      // Fire_alarm_state = P1RxBuff[5]  
      // only use bit 1, 0  (Zone 10, 9)
      // Fire_alarm_state = P1RxBuff[6]   
      // use bit 7...0 (ZONE8...ZONE1)        
      // send a package to remote operator plane
      // 通知遠端控制面版進入火災警報模式
        PutToQue(P1FIFOIn, FIRE_PROC);  //
        PutToQue(P1FIFOIn, P1RxBuff[3]); // ID
        PutToQue(P1FIFOIn, P1RxBuff[4]); // State, N8500 not support
        PutToQue(P1FIFOIn, P1RxBuff[5]); // 10 bits Flash data
        PutToQue(P1FIFOIn, P1RxBuff[6]);                
     }
  }

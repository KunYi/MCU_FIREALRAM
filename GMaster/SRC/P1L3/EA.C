  if (P1RxBuff[2] == 'M') { 
    if (( P1RxBuff[3] == 'M') && ( P1RxBuff[4] == 'O') && ( P1RxBuff[5] == 'D') && ( P1RxBuff[6] == 'E')) { 
   	       // send a package to remote operator plane
           // 通知遠端控制面版進入緊急全區警報模式
      PutToQue(P1FIFOIn, EMG_PROC);
      PutToQue(P1FIFOIn, EMG_SUB_ENTRY);
    }
  } else if (P1RxBuff[2] == 'Q') {  
    if (( P1RxBuff[3] == 'Q') && ( P1RxBuff[4] == 'U') && ( P1RxBuff[5] == 'I') && ( P1RxBuff[6] == 'T')) {
   	       // send a package to remote operator plane
           // 通知遠端控制面版離開緊急全區警報模式
      PutToQue(P1FIFOIn, EMG_PROC);
      PutToQue(P1FIFOIn, EMG_SUB_QUIT);  
    }
  }
  break;

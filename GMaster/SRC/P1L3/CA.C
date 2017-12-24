  if (P1RxBuff[2] == 'A') {
    if ((P1RxBuff[3] == 'P') && (P1RxBuff[4] == 'U') && (P1RxBuff[5] == 'S') && (P1RxBuff[6] == 'H')) { 
    // send a package to remote operator plane
    // 通知遠端控制面版離開警報模式
       PutToQue(P1FIFOIn,QUITALARM_PROC);
    }
  } 

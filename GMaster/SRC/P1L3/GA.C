  if (P1RxBuff[2] == 'A') {
    if ((P1RxBuff[3] == 0) && (P1RxBuff[4] == 0) && (P1RxBuff[5] == 0)) {
      // send a package to remote operator plane
      // 通知遠端控制面版選擇群
    } else if ((P1RxBuff[3] == 'G') && (P1RxBuff[4] == 'A')) {
      PutToQue(P1FIFOIn, GROUP_PROC);
      PutToQue(P1FIFOIn, GROUP_SUB_ACTION1);
      PutToQue(P1FIFOIn, P1RxBuff[5]); // 10 bits status
      PutToQue(P1FIFOIn, P1RxBuff[6]);
    }
  } else if (P1RxBuff[2] == 'M') {
    if ((P1RxBuff[3] == 'M') && (P1RxBuff[4] == 'O') && (P1RxBuff[5] == 'D') && (P1RxBuff[6] == 'E'))     {
      // send a package to remote operator plane
      // 通知遠端控制面版進入群組狀態
      PutToQue(P1FIFOIn, GROUP_PROC);
      PutToQue(P1FIFOIn, GROUP_SUB_ENTRY);
    }
  } else if (P1RxBuff[2] == 'Q') {
    if ((P1RxBuff[3] == 'Q') && (P1RxBuff[4] == 'U') && (P1RxBuff[5] == 'I') && (P1RxBuff[6] == 'T')) {
      PutToQue(P1FIFOIn, GROUP_PROC);
      PutToQue(P1FIFOIn, GROUP_SUB_QUIT);
    }
  } else if (P1RxBuff[2] == 'E') {
      PutToQue(P1FIFOIn, GROUP_PROC);
      PutToQue(P1FIFOIn, GROUP_SUB_EXGROUP);
      PutToQue(P1FIFOIn, P1RxBuff[5]);
      PutToQue(P1FIFOIn, P1RxBuff[6]);
  }
   
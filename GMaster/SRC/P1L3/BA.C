  switch(P1RxBuff[2]) {
  case 'M':
    if (( P1RxBuff[3] == 'M') && ( P1RxBuff[4] == 'O') && ( P1RxBuff[5] == 'D') && ( P1RxBuff[6] == 'E')) {
      PutToQue(P1FIFOIn,BGM_PROC);
      PutToQue(P1FIFOIn,BGM_SUB_ENTRY);
    }
    break;
  case 'Q':               
    if ((P1RxBuff[3] == 'Q') && (P1RxBuff[4] == 'U') && (P1RxBuff[5] == 'I') && (P1RxBuff[6] == 'T')) {
      PutToQue(P1FIFOIn,BGM_PROC);
      PutToQue(P1FIFOIn,BGM_SUB_QUIT);
    }
    break;
  }

  if (P1RxBuff[2] == 'A') {
    if ((P1RxBuff[3] == 'P') && (P1RxBuff[4] == 'U') &&  (P1RxBuff[5] == 'S') && (P1RxBuff[6] == 'H')) {
      PutToQue(P1FIFOIn, PTT_PROC);
    }
  }

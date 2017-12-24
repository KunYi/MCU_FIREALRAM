
  PutToQue(P1FIFOIn,SPC_PROC);

  switch(P1RxBuff[2]) {
  case SPC_NORMAL:
    PutToQue(P1FIFOIn,SC_SUB_NORMAL);
    break;
  case SPC_ENTRY_FIRE:
    PutToQue(P1FIFOIn,SC_SUB_ENTRY_FIRE);
    goto SPC_COMM_FIRE;
  case SPC_CONFIRM_FIRE:
    PutToQue(P1FIFOIn,SC_SUB_CONFIRM_FIRE);
    goto SPC_COMM_FIRE;
  case SPC_NOT_FIRE:
    PutToQue(P1FIFOIn,SC_SUB_NOT_FIRE);
  SPC_COMM_FIRE:
    PutToQue(P1FIFOIn,P1RxBuff[3]);
    PutToQue(P1FIFOIn,P1RxBuff[4]);
    break;
  }

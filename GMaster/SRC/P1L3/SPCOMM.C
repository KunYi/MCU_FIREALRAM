
  switch(P1RxBuff[2]) {
  case SPC_NORMAL:
    P2TxCMD(AckID, 0, CMD_ENTRY_NORMAL, 0, 0);
    break;
  case SPC_ENTRY_FIRE:
    break;
  case SPC_CONFIRM_FIRE:
    break;
  case SPC_NOT_FIRE:
    break;
  }

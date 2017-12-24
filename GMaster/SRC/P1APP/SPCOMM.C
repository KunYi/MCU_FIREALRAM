  GetFromQue(P1FIFOIn, &Type);  // SUB_ITEM
  switch(Type) {
  case SC_SUB_NORMAL:
    P2TxCMD(AckID, 0, CMD_ENTRY_NORMAL, 0, 0);
    break;
  case SC_SUB_ENTRY_FIRE:
  case SC_SUB_CONFIRM_FIRE:
  case SC_SUB_NOT_FIRE:
    GetFromQue(P1FIFOIn, &status1); // KEY
    GetFromQue(P1FIFOIn, &status2); // Key Status
    break;
  }

  switch(Type) {
  case SC_SUB_ENTRY_FIRE:
    P2TxCMD(AckID, 0, CMD_ENTRY_PREALARM, status1, status2);
    break;
  case SC_SUB_CONFIRM_FIRE:
    P2TxCMD(AckID, 0, CMD_CONFIRM_FIRE, status1, status2);
    break;
  case SC_SUB_NOT_FIRE:
    P2TxCMD(AckID, 0, CMD_NOT_FIRE, status1, status2);
    break;
  }

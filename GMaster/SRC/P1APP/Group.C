  GetFromQue(P1FIFOIn, &Type); // Sub Item
  switch(Type) {
  case GROUP_SUB_QUIT:
    P2TxCMD(AckID, 0, CMD_QUIT_GROUP, 0, 0);
    break;
  case GROUP_SUB_ACTION1:
    GetFromQue(P1FIFOIn, &status1);
    GetFromQue(P1FIFOIn, &status2);
    P2TxCMD(AckID, 0, CMD_ACTIVE_GROUP, status1, status2);
    break;
  case GROUP_SUB_EXGROUP:
    GetFromQue(P1FIFOIn, &status1);
    GetFromQue(P1FIFOIn, &status2);
    P2TxCMD(AckID, 0, CMD_EXTEND_GROUP, status1, status2);
    break;
  case GROUP_SUB_ENTRY:
    P2TxCMD(AckID, 0, CMD_ENTRY_GROUP, 0, 0);
    break;
  }

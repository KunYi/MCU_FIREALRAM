  GetFromQue(P1FIFOIn, &Type);  // SUB_ITEM
  
  switch(Type) {
  case BGM_SUB_ENTRY:
    // polling system
    P2TxCMD(AckID, 0, CMD_ENTRY_BGM, 0, 0);
    break;
  case BGM_SUB_QUIT:
    P2TxCMD(AckID, 0, CMD_QUIT_BGM, 0, 0);
    break;
  }
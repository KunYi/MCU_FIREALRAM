  GetFromQue(P1FIFOIn, &Type);  // SUB_ITEM
  
  switch(Type) {
  case MANU_SUB_ENTRY:
    P2TxCMD(AckID, 0, CMD_ENTRY_MANU, 0, 0);
    break;
  case MANU_SUB_QUIT:
    P2TxCMD(AckID, 0, CMD_QUIT_MANU, 0, 0);
    break;
  case MANU_SUB_ALLCALL:
    P2TxCMD(AckID, 0, CMD_ALLCALL_MANU, 0, 0);
    break;
  }
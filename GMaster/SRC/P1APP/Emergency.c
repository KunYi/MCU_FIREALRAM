  GetFromQue(P1FIFOIn, &Type); // Sub Item
  switch(Type) {
  case EMG_SUB_ENTRY:
    // all RED LED, all GREEN LED, FA
    P2TxCMD(AckID, 0, CMD_ALL_EMG, 0, 0);
    break;
  case EMG_SUB_QUIT:
    // System reset
    P2TxCMD(AckID, 0, CMD_QUIT_EMG, 0, 0);
    break;
  }
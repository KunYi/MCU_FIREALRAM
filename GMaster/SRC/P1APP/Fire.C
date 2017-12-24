  GetFromQue(P1FIFOIn, &ReceID);   // ID
  GetFromQue(P1FIFOIn, &Type);     // Type, N8500 not support type
  GetFromQue(P1FIFOIn, &status1);  // 10 bits
  GetFromQue(P1FIFOIn, &status2);  //
  
  if (ReceID == 0) {
    break;
  } else {
    P2TxCMD(AckID, ReceID, CMD_EU_FIRE, status1, status2);
  }

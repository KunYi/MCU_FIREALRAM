  GetFromQue(P1FIFOIn, &ReceID);   // ID
  GetFromQue(P1FIFOIn, &status1);  // 10 bits
  GetFromQue(P1FIFOIn, &status2);  //
  
  P2TxCMD(AckID, ReceID, CMD_EMG_TEL, status1, status2);
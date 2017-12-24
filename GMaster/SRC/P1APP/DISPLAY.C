
  GetFromQue(P1FIFOIn, &ReceID);  // ID
  GetFromQue(P1FIFOIn, &Type);
  GetFromQue(P1FIFOIn, &status1);
  GetFromQue(P1FIFOIn, &status2);

  switch(Type) {
  case 1: // RED LED
    P2TxCMD(AckID, ReceID, CMD_RLED, status1, status2);
    break; 
  case 2: // GREEN LED
    P2TxCMD(AckID, ReceID, CMD_GLED, status1, status2);
    break; 
  case 3: // CONTROL LED
    P2TxCMD(AckID, ReceID, CMD_CLED, status1, status2);
    break; 
  case 4: // GROUP LED
    P2TxCMD(AckID, ReceID, CMD_GPLED, status1, status2);
    break; 
  case 5: // Flash RED
    P2TxCMD(AckID, ReceID, CMD_FRLED, status1, status2);
    break; 
  case 6: // Flash GREEN
    P2TxCMD(AckID, ReceID, CMD_FGLED, status1, status2);
    break; 
  case 7: // Flash Control
    P2TxCMD(AckID, ReceID, CMD_FCLED, status1, status2);
    break; 
  case 8: // Flash Group
    P2TxCMD(AckID, ReceID, CMD_FGPLED, status1, status2);
    break; 
  case 9: // LCD
    P2TxCMD(AckID, ReceID, CMD_LCD, status1, status2);
    break; 
  }

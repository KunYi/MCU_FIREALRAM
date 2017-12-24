  GetFromQue(P1FIFOIn, &Type);   // Type
  GetFromQue(P1FIFOIn, &status1); // ResponseID
  GetFromQue(P1FIFOIn, &status2); // Reply KeyValue

  if (P2SendID == 0x00) {
    switch (Type) {
    case XFUNC_SUB_ACCEPTKEY: // Key
      if ((status2 == P2Value2)) {
        CompleteP1Tx();           
      }
      break;
    case XFUNC_SUB_MICSTATUS: // Mic
      if (0xFF == status2) {
        SET_PORTA(PRESS_REMOTE_MIC);
      } else if ( 0x00 == status2) {
        SET_PORTA(RELEASE_REMOTE_MIC);
      }
      CompleteP1Tx();
      break;
    case  XFUNC_SUB_XGROUP:
      if (0x00 == status2) {
      } else if (0xFF == status2) {
        // setting PORT_A
        SET_PORTA(ENTRY_EXGROUP);
        bSetPORTA = 1;
      }
      CompleteP1Tx();
      break;

    case XFUNC_SUB_DGROUP: // exit ex group
      if (bSetPORTA) {
        // clean PORT_A
        SET_PORTA(EXIT_EXGROUP);
        bSetPORTA = 0;
      }
      CompleteP1Tx();    
      break;
    } // end switch
  }
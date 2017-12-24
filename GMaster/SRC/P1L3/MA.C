  switch (P1RxBuff[2]) {
  default:
    if ((P1RxBuff[3] == 'Q') && (P1RxBuff[4] == 'U') && (P1RxBuff[5] == 'I') && (P1RxBuff[6] == 'T')) {
      // Exit relation setup
      PutToQue(P1FIFOIn, SETUP_PROC);
      switch (P1RxBuff[2]) {
      case 'B':
        PutToQue(P1FIFOIn, SETUP_SUB_QBGM);
        break;
      case 'C':
        PutToQue(P1FIFOIn, SETUP_SUB_QFIRE);
        break;
      case 'G':
        PutToQue(P1FIFOIn, SETUP_SUB_QGROUP);
        break;
      case 'V':
        PutToQue(P1FIFOIn, SETUP_SUB_QVOICE);
        break;        
      case 'S':
        PutToQue(P1FIFOIn, SETUP_SUB_QSETUP);
        break;                
      case 'X':
        PutToQue(P1FIFOIn, SETUP_SUB_QTRYPLAY);
        break;
      }
    } else if ((P1RxBuff[3] == 'M') && (P1RxBuff[5] == 'O') && (P1RxBuff[5] == 'D') && (P1RxBuff[6] == 'E')) {
      // Entry relation setup
      PutToQue(P1FIFOIn, SETUP_PROC);
      switch (P1RxBuff[2]) {
      case 'B':
        PutToQue(P1FIFOIn, SETUP_SUB_EBGM);
        break;
      case 'C':
        PutToQue(P1FIFOIn, SETUP_SUB_EFIRE);
        break;
      case 'G':
        PutToQue(P1FIFOIn, SETUP_SUB_EGROUP);
        break;
      case 'V':
        PutToQue(P1FIFOIn, SETUP_SUB_EVOICE);
        break;        
      case 'S':
        PutToQue(P1FIFOIn, SETUP_SUB_ESETUP);
        break;                
      case 'X':
        PutToQue(P1FIFOIn, SETUP_SUB_ETRYPLAY);
        break;
      }
    }

  case  'F': // Fire zone
    if ((P1RxBuff[4] == 0) && (P1RxBuff[6] == 0xff )) { 
      if ((P1RxBuff[3] <= 0x0f) && (P1RxBuff[5] <= 0x0A)) {        
        PutToQue(P1FIFOIn, SETUP_PROC);
        PutToQue(P1FIFOIn, SETUP_SUB_ZONE);
        PutToQue(P1FIFOIn, P1RxBuff[3]); // ID
        PutToQue(P1FIFOIn, P1RxBuff[5]); //Zone
      }
    }
    break;
          
  case  'R': // Receive package
    if ((P1RxBuff[3] == 'O') && (P1RxBuff[4] == 'K') && (P1RxBuff[5] ==  0 )) { 
      PutToQue(P1FIFOIn, EXIST_PROC);  //
      PutToQue(P1FIFOIn, P1RxBuff[6]); // ID
    }
    break;                      
            
           

       
  } // terminal of switch(P1RxBuffer[2])

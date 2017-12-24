
  GetFromQue(P1FIFOIn, &Type);  // SUB_ITEM

  switch(Type) {
  case SETUP_SUB_EBGM:
    break;
  case SETUP_SUB_EFIRE:
    break;
  case SETUP_SUB_EGROUP:
    break;
  case SETUP_SUB_EVOICE:
    break;
  case SETUP_SUB_ESETUP:
    break;
  case SETUP_SUB_ETRYPLAY:
    break;
  case SETUP_SUB_QBGM:
    break;
  case SETUP_SUB_QFIRE:
    break;
  case SETUP_SUB_QGROUP:
    break;
  case SETUP_SUB_QVOICE:
    break;
  case SETUP_SUB_QSETUP:
    break;
  case SETUP_SUB_QTRYPLAY:
    break;
  case SETUP_SUB_ZONE:
    GetFromQue(P1FIFOIn, &ReceID);  // ID
    GetFromQue(P1FIFOIn, &status1); // Zone
    break;
  }
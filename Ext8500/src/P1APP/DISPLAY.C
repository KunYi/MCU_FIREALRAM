  // P1RxBuff[3] // bit 9 .. bit8
  // P1RxBuff[4] // bit 7 .. bit0
  case  CMD_RLED:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_SELF_RLED);
    } else {
      // other
      PutToQue(P1FIFOIn, P1_EXUNIT_RLED);
    }   
    goto DISPLAY_COMM;
  case  CMD_GLED:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_SELF_GLED);
    } else {
      // other
      PutToQue(P1FIFOIn, P1_EXUNIT_GLED);
    }   
    goto DISPLAY_COMM;
  case  CMD_CLED:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_SELF_CLED);
    } else {
      // other
      PutToQue(P1FIFOIn, P1_EXUNIT_CLED);
    }   
    goto DISPLAY_COMM;
  case  CMD_GPLED:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_SELF_GPLED);
    } else {
      // other
      PutToQue(P1FIFOIn, P1_EXUNIT_GPLED);
    }   
    goto DISPLAY_COMM;
  case  CMD_FRLED:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_SELF_FRLED);
    } else {
      // other
      PutToQue(P1FIFOIn, P1_EXUNIT_FRLED);
    }   
    goto DISPLAY_COMM;
  case  CMD_FGLED:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_SELF_FGLED);
    } else {
      // other
      PutToQue(P1FIFOIn, P1_EXUNIT_FGLED);
    }   
    goto DISPLAY_COMM;
  case  CMD_FCLED:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_SELF_FCLED);
    } else {
      // other
      PutToQue(P1FIFOIn, P1_EXUNIT_FCLED);
    }   
    goto DISPLAY_COMM;
  case  CMD_FGPLED:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_SELF_FGPLED);
    } else {
      // other
      PutToQue(P1FIFOIn, P1_EXUNIT_FGPLED);
    }   
    goto DISPLAY_COMM;
  case  CMD_LCD:
    if (P1RxBuff[2] == 0) {
      // main unit
      PutToQue(P1FIFOIn, P1_LCD_STATE);
    }

DISPLAY_COMM:
  PutToQue(P1FIFOIn, P1RxBuff[2]); // ID
  PutToQue(P1FIFOIn, P1RxBuff[3]); // status1-- bit9 ... bit8
  PutToQue(P1FIFOIn, P1RxBuff[4]); // status2-- bit7 ... bit0
  break;

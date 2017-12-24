  case CMD_KEY:
    // Original ID == P1RxBuff[2]
    // Key number  == P1RxBuff[3]
    // Key Status  == P1RxBuff[4]
    if (P1RxBuff[2] == 0) {
      if (sysStatus.Proc == SYS_GROUP) {
        // group
        if (P1RxBuff[4] == 0) {
          ClrLED(GROUP_LED + P1RxBuff[3] - 1);
        } else {        
          SetLED(GROUP_LED + P1RxBuff[3] - 1);
        } 
      } else {
        // zone
        if (P1RxBuff[4] == 0) {
          ClrLED(ZONE_ACTIVE_LED + P1RxBuff[3] - 1);
        } else {        
          SetLED(ZONE_ACTIVE_LED + P1RxBuff[3] - 1);
        } 
      }
    } else {
        // other unit
    }
    break;

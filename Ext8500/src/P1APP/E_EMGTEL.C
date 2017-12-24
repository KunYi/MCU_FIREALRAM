  case CMD_EMG_TEL:
    // Original ID == P1RxBuff[2]
    // Status1  == P1RxBuff[3]
    // Status2  == P1RxBuff[4]

    LcdScreen(LCD_FIRE);
    if (0 == P1RxBuff[2]) {
      // GW8500
      
    } else {
      // GW8310
      
    }
    break;

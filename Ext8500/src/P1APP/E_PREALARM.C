  case CMD_ENTRY_PREALARM:
    LcdScreen(LCD_PRE_ALARM);
    SetFlash(ALL_ALARM_LED);
    SetFlash(FIRE_LED);
    SetFlash(NO_FIRE_LED);
    SetLED(NO_FIRE_LED);
    break;
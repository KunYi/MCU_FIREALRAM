  case CMD_ENTRY_MANU:
    ClrAllLED();
    LcdScreen(LCD_ENTRY_MANU);
    sysStatus.Proc = SYS_MANU;
    break;
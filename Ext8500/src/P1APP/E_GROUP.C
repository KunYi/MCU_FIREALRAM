  case CMD_ENTRY_GROUP:
    ClearGLED();
    LcdScreen(LCD_ENTRY_GROUP);
    sysStatus.Proc = SYS_GROUP;
    break;
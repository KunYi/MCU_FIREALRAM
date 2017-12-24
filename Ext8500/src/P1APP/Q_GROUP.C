  case CMD_QUIT_GROUP:
    ClearGLED();
    LcdScreen(LCD_NORMAL); 
    sysStatus.Proc = SYS_NORMAL;
    break;
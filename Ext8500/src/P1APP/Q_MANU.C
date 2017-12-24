  case CMD_QUIT_MANU:
    ClearGLED();
    LcdScreen(LCD_NORMAL); 
    sysStatus.Proc = SYS_NORMAL;
    break;
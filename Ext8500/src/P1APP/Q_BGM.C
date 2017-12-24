  case CMD_QUIT_BGM:
    //sysStatus.IsBGM = FALSE;
    ClearGLED();
    LcdScreen(LCD_NORMAL); 
    sysStatus.Proc = SYS_NORMAL;

    break;
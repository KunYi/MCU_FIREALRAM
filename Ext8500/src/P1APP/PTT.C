  case CMD_PTT_PROC:
    switch(sysStatus.Proc) {
    case SYS_NORMAL:
      break;
    case SYS_MANU:
      LcdScreen(LCD_MANU_MIC);
      break;
    case SYS_GROUP:
    case SYS_BGM:
      // nothing
      break;
    }
    break;

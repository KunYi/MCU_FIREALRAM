  case CMD_ENTRY_BGM:
    LcdScreen(LCD_ENTRY_BGM);
    ClearGLED();
    // sysStatus.IsBGM = TRUE;
    {
      BYTE Temp[2];
      
      Temp[0] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(BackGroundMusicStorage, 0, 0));
      Temp[1] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(BackGroundMusicStorage,0, 0)+1));
      // Set LED
      Temp[1] = Temp[1] << 2; // because Green 9, 10 relattionship BIT2, BIT3
      OR_GLED(Temp[0], Temp[1]);
    }        
    sysStatus.Proc = SYS_BGM;

    break;
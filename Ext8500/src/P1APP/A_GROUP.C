  case CMD_ACTIVE_GROUP:
    LcdScreen(LCD_MIC);
    {
      BYTE Temp[2];
      BYTE GroupCount;
      BYTE Group;

      // P1RxBuff[3]  LSB Bit8, Bit9
      // P1RxBuff[4]  LSB Bit0, Bit1 ....... Bit7 MSB      
      // ------xx xxxxxxxx

      Group = P1RxBuff[4];

      for (GroupCount = 0;  GroupCount < 10; GroupCount++) {
        if (GroupCount == 8) {
          Group = P1RxBuff[3];
        }
        
        if (Group & 0x01) {
          Temp[0] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(GroupStorage, 0, GroupCount));
          Temp[1] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(GroupStorage, 0, GroupCount)+1));
          Temp[1] = Temp[1] << 2; // because Green 9, 10 relattionship BIT2, BIT3
          OR_GLED(Temp[0], Temp[1]);
        }
        // Set LED
        Group = Group >> 1;
      } 
    }
    break;
  case CMD_EXTEND_GROUP:
    ClearGLED();
    sysStatus.Proc = SYS_GROUP;
    LcdScreen(LCD_EXGROUP);
    {
      BYTE Temp[2];
      BYTE GroupCount;
      BYTE Group;

      // P1RxBuff[3]  LSB Bit8, Bit9
      // P1RxBuff[4]  LSB Bit0, Bit1 ....... Bit7 MSB      
      // ------xx xxxxxxxx

      Group = P1RxBuff[3];
      for (GroupCount = 0;  GroupCount < 10; GroupCount++) {
        if (GroupCount == 8) {
          Group = P1RxBuff[4];
        }
        
        if (Group & 0x01) {
          Temp[0] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(GroupStorage, 0, GroupCount));
          Temp[1] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(GroupStorage, 0, GroupCount)+1));
          Temp[1] = Temp[1] << 2; // because Green 9, 10 relattionship BIT2, BIT3
          OR_GLED(Temp[0], Temp[1]);
        }
        // Set LED
        Group = Group >> 1;
      } 
    }
    break;

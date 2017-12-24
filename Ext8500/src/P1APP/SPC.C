  case CMD_ENTRY_NORMAL:
    sysStatus.Proc = SYS_NORMAL;
    ClearGLED();
	// correctly LED state @ Aug. 30, 2008
	ClrLED(FIRE_LED);
	ClrLED(ALL_ALARM_LED);
    LcdScreen(LCD_NORMAL);
    break;

  case CMD_CONFIRM_FIRE:
    sysStatus.Proc = SYS_FIRE;
    LcdScreen(LCD_FIRE);
    ClrFlash(ALL_ALARM_LED);
    ClrFlash(FIRE_LED);
    ClrFlash(NO_FIRE_LED);
    ClrLED(SOUND_LED);
    ClrLED(NO_FIRE_LED);
    SetLED(FIRE_LED);
    SetLED(ALL_ALARM_LED);
    SetFlash(SOUND_LED);

    goto FireRelation;

  case CMD_NOT_FIRE:
    sysStatus.Proc = SYS_NOT_FIRE;
    LcdScreen(LCD_NO_FIRE);
    ClrFlash(ALL_ALARM_LED);
    ClrFlash(FIRE_LED);
    ClrFlash(NO_FIRE_LED);
    ClrLED(FIRE_LED);
    ClrLED(SOUND_LED);
    SetLED(NO_FIRE_LED);
    SetFlash(SOUND_LED);

    goto FireRelation;
  case CMD_ENTRY_PREALARM:
    sysStatus.Proc = SYS_PREALARM;
    LcdScreen(LCD_PRE_ALARM);

    ClrAllLED();
    SetFlash(ALL_ALARM_LED);
    SetFlash(FIRE_LED);
    SetFlash(NO_FIRE_LED);
    SetFlash(SOUND_LED);
    SetLED(NO_FIRE_LED);
    // because cann't dectect digital sound
    // SetFlash(SOUND_LED);

FireRelation:
  {
    BYTE Temp[2];
    BYTE FireCount;
    BYTE Fire;

    // P1RxBuff[3]  LSB Bit8, Bit9
    // P1RxBuff[4]  LSB Bit0, Bit1 ....... Bit7 MSB      
    // ------xx xxxxxxxx

    Fire = P1RxBuff[4];

    for (FireCount = 0;  FireCount < 10; FireCount++) {
      if (FireCount == 8) {
        Fire = P1RxBuff[3];
      }
        
      if (Fire & 0x01) {
        SetLED(FireCount); // setting Fire LED
        Temp[0] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireRelationStorage, 0, FireCount));
        Temp[1] = ~Read_BYTE_EEPROM((CalcEEPROMOffset(FireRelationStorage, 0, FireCount)+1));
        Temp[1] = Temp[1] << 2; // because Green 9, 10 relattionship BIT2, BIT3
        OR_GLED(Temp[0], Temp[1]);
        // Set LED
      }
      Fire = Fire >> 1;
    } 
  }

    break;

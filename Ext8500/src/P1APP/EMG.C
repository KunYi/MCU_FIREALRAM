  case CMD_ALL_EMG:
    LcdScreen(LCD_ALLEMG);
    SetLED(ALL_ALARM_LED);
    ClrFlash(ALL_ALARM_LED);
    ClrFlash(SOUND_LED);
    ClrFlash(FIRE_LED);
    ClrFlash(NO_FIRE_LED);
    ClrLED(NO_FIRE_LED);
    OR_GLED(0xFF,0x0C); // Turn On Green
    {
      BYTE nCount;

      for (nCount = 0; nCount < 10; nCount++) {
        SetLED(nCount+ZONE_FIRE_LED);

      }
    }

    SetFlash(SOUND_LED);
    // Refresh(); // has a bug don't refresh led state
    break;

  case CMD_QUIT_EMG:
    // check alarm
    LcdScreen(LCD_CHK_ALARM);
    ClrLED(ALL_ALARM_LED);
    ClrFlash(ALL_ALARM_LED);
    ClrFlash(SOUND_LED);
    ClrFlash(FIRE_LED);
	ClrLED(FIRE_LED);
    ClrFlash(NO_FIRE_LED);
    ClrLED(NO_FIRE_LED);
    ClearGLED();
    {
      BYTE nCount;
      for (nCount = 0; nCount < 10; nCount++) {
        ClrLED(nCount+ZONE_FIRE_LED);

      }
    }
    break;

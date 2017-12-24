// System normal state
    if (IsMicPhonePress()) {
      IsAllCall = 1;
      ProcState = InitialHumanHandlingState;
    }
    
    switch (KeyCode) {   
    // Entry human handling public address mode
    case LCD3_KEY:
      GenCount2 = 0;
      ProcState = InitialHumanHandlingState;
      LcdScreen(LCD_ENTRY_MANU);
      //  CallStationMPUTalk();
      //  DisCallStation();
      break;   
   	// Push BACKGROUND_MUSIC_KEY
    case LCD4_KEY: 
      // Entry BGM Player state
      GenCount2 = 0;
      ProcState = InitialBGMPlayState;
      break;        
    // Push GROUP_KEY
    case LCD2_KEY:
      // Entry group public address state
      GenCount2 = 0;
      ProcState = InitialGroupState;
      LcdScreen(LCD_ENTRY_GROUP);
      CallStationMPUTalk();
      break;
    // All zone emergency public address
    case ALL_ALARM_KEY: 
      ProcState = InitialAllZoneEmergencyState;
      GenCount2 = 0;
      break;
    case ALL_ZONE_KEY:
      GenCount2 = 0;
      IsAllCall = 1;
      ProcState = InitialHumanHandlingState;
      LcdScreen(LCD_ENTRY_MANU);
      break;
    case BACKLIGHT_KEY: 
      LcdScreen(LCD_BACKLIGHT);
      break;
    } // switch (KeyCode) end
   
    if (GetPushKey() == LCD1_KEY)  {
      if ( PushCount() >= 100)  { 
        ProcState = EntryCheckPasswordState;
        LcdScreen(LCD_CHK_PASSWORD); 
        GenCount2 = 0;
	      CallStationMPUTalk();
        // DisCallStation();
      }
    }
    
    switch(CallStationState) {
    case CallStationActiveState:
      ProcState = InitialCallStationModeState;
      LcdScreen(LCD_ENTRY_CS);
      break;
    case CallStationChimeUpState:
      Chime = Chime1;
      break;
    case CallStationChimeDownState:
      Chime = Chime2;
      break;
    case CallStationZoneState:
      ProcState =InitialCallStationZoneState;
      LcdScreen(LCD_ENTRY_CS);  
      break;
    case CallStationBGMONState:
      ProcState = InitialCallStationBGMPlayState;
      LcdScreen(LCD_ENTRY_CSBGM);
      break;
    }
            
    IsDisFireAlarm = OFF;
    IsEmergencyMode = FALSE;
    IsPreEmergencyMode = FALSE;
	  // Add for Sync Display issus
    // Idle 
   	if (Delay2 > 20000) {
      SPECIAL_CMD(SPC_NORMAL, 0, 0);
	  Delay2 = 0;
	} else { Delay2++; }
  // Normal state process end

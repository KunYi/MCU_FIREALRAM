    if (SetupCount == 0) { 
      ExitSetupMode();
      CallStationMPUTalkOk();
      SetupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      LcdScreen(LCD_BL_ON);
      IsStartSetupSoftTimer = OFF;
    }
       
	if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      ExitSetupMode();
      // CallStationMPUTalkOk();
      SetupCount++;
      InitialSoftTimer(SoftTimer1);
    }
    
	if ((SetupCount == 3) || 
      (CommunicationProcState == ExitSetupModeState)||
      (CallStationState = CallStationNormalState)) {
     ClrAllLED();
     SetupCount = 0;
     ProcState = InitialState;
     CallStationState = CallStationNormalState;
     IsDisFireAlarm = OFF;
     // ResetCallStationBus();
     IsStartSoftTimer1 = OFF;
     InitialSoftTimer(SoftTimer1);
  	} else { IsStartSoftTimer1 = ON; }

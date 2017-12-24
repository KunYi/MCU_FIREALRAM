    if (SetupCount == 0) { 
      IsDisFireAlarm = ON;
      EntrySetupMode();
      CallStationMPUTalk();
      SetupCount++;
      InitialSoftTimer(SoftTimer1);
      IsStartSoftTimer1 = ON;
      IsStartSetupSoftTimer = ON;
      InitialSoftTimer(SetupSoftTimer);
    }
       
    if (SoftTimer1.m10sec == 3 ) { 
      IsStartSoftTimer1 = OFF; 
      EntrySetupMode();
      // CallStationMPUTalk();
      SetupCount++;
      InitialSoftTimer(SoftTimer1);
    }
 
    if (( SetupCount == 3 ) ||
        (CommunicationProcState == InitialEntrySetupState )||(CallStationState == CallStationMPUTalkState)) {
      // SetLED(SETUP_LED);
      SetupCount = 0;
      ProcState = SetupModeSelectState;
      IsStartSoftTimer1 = OFF; 
      InitialSoftTimer(SoftTimer1);
    }
    else { IsStartSoftTimer1 = ON; }
 
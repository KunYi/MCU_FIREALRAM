    if (InitialStateCount == 0) {
      PMR |= 0x01;                 /* Enable W77E58 on chip 1K XRAM */
      RESET_REGISTER(EA);          /* Disable Interrupt */
      DEVICE_Enable = 1;           /* DEVICE Disable */
      P_CallBack[0] = &SoftTimerRefresh;
      P_CallBack[1] = &KeyGet;
      P_CallBack[2] = &LEDRefresh;
      P_CallBack[3] = 0;

      IsStartSoftTimer1      = OFF;
      IsStartSoftTimer2      = OFF;
      IsStartSystemSoftTimer = OFF;
      IsPreEmergencyMode     = FALSE;
      IsEmergencyMode        = FALSE;
      IsAllZoneEmergencyMode = FALSE;
      IsNoFireMode           = FALSE;
      IsNoFireCancelMode     = FALSE;
      IsBGMMode              = FALSE;
      IsFireActiveFlag       = FALSE;
      IsLoopShortFlag        = FALSE;
      IsDisFireAlarm         = ON;

      GenCount1     = 0;
      GenCount2     = 0;
      GenCount3     = 0;
      GenCount4     = 0;
//      GenCount5     = 0;
      SFireCount    = 0;
      FireZoneCount = 0;
 
      ClrAllLED();
      ClrAllRelay();
      SelectAudioSource(ALLCLOSE_AUDIO);
      ClrFlash(SOUND_LED);
      ClrLED(SOUND_LED);
      Refresh();
      DEVICE_Enable = 0;         /* DEVICE Enable */
      InitUART0();
      InitUART1();
      while(IsSpeechBusy());
      InitialSpeechChip();
      while(IsSpeechBusy());
      SpeechCmd(0x60);
      Init_Physical_Timer0();    /* Init Timer0 */
      Start_Timer0();            /* Star Timer0 */
      Enable_Timer0_Interrupt(); /* Enable Timer 0 Interrupt */
      SET_REGISTER(EA);          /* Enable Interrupt */
      InitialSoftTimer(SoftTimer1);
      InitialSoftTimer(SystemSoftTimer);
      InitialStateCount = 1;
      IsStartSoftTimer1 = ON;
      IsStartSystemSoftTimer = ON;
      InitialSensor();
      ClearSpeechFIFO();
    }
     
    if (SoftTimer1.m10sec == 4) { 
      IsStartSoftTimer1 = OFF;
      IsExtendUnitExist(MaxExtendUnit+1-InitialStateCount);
      InitialSoftTimer(SoftTimer1);

         // complete initial process 
      if (InitialStateCount == MaxExtendUnit+1) { 
        for (InitialStateCount = 1; InitialStateCount < MaxExtendUnit+1; InitialStateCount++)  {
          FireZoneBuffer[(InitialStateCount*2)]   = 0;
          FireZoneBuffer[(InitialStateCount*2)+1] = 0;
        }
                
        for (InitialStateCount = 0; InitialStateCount < 2; InitialStateCount++) {
          LoopShortBuffer[InitialStateCount]  = 0;
          FireRelationMask[InitialStateCount] = 0;
        }
 
        InitialSoftTimer(SoftTimer1);
        ProcState = CheckPrevStatus;
        CallStationPowerOnOk();
//      ResetCallStationBus();
        InitialStateCount = 0;
        IsDisFireAlarm = OFF;
        IsSpeechPlayer = TRUE;
      } else { 
        InitialStateCount++; 
        IsStartSoftTimer1 = ON; 
      }         
    } /* if (SoftTimer1.m10sec) */

#undef InitialStateCount
    

{ //  case InitialSetupGroupState
  if (SetupGroupCount == 0)
  {
  	EntryGroupSetup();
  	SetupGroupCount++;
  	InitialSoftTimer(SoftTimer1);
  	IsStartSoftTimer1 = ON;
  }
       
  if (SoftTimer1.m10sec == 3 )
  {
  	IsStartSoftTimer1 = OFF;
  	EntryGroupSetup();
  	SetupGroupCount++;
  	InitialSoftTimer(SoftTimer1);
  }
  
  if ((SetupGroupCount == 3) || 
      (CommunicationProcState == InitialSetupGroupState) )
  {
  	//      SetLED(GROUP_LED);
  	SetupGroupCount = 0;
  	ProcState = SetupGroupZoneState;
  	IsStartSoftTimer1 = OFF;
  	InitialSoftTimer(SoftTimer1);
  }
  else 
  {
  	IsStartSoftTimer1 = ON;
  }
} // end case InitialSetupGroupState
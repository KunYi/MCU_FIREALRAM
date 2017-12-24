 /* Setup Mode Select 
    Fire alarm zone relation define
    Digital voice message define */
   switch (KeyCode) {
   case LCD4_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     //  ProcState = ExitSetupModeState;	//by andy
     ProcState =ExitGroupState;		//by andy
   break;   

   // Entry fire relation setup
   case LCD1_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = InitialSetupFireRelationState;
     LcdScreen(LCD_SETUP_FIRE);
     //  SetLED(HUMAN_HANDLING_LED); 
     break;

   case LCD3_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     ProcState = InitialVoiceTestPlayState;
     LcdScreen(LCD_TESTVOICE);
     //  SetLED(TESTPLAY_LED);
     break;
   
   // Entry digital voice message setup
   case LCD2_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = InitialSetupVoiceState;
     LcdScreen(LCD_NONAME1);
     break;
   case BACKLIGHT_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     LcdScreen(LCD_BACKLIGHT);
     break;     
   }
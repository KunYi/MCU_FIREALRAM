 /* Setup Mode Select 
    Fire alarm zone relation define
    Group zone define
    Background music zone define
    Digital voice message define */
   switch (KeyCode) {
   case LCD4_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     //ProcState = ExitSetupModeState;	//by andy
     ProcState =ExitGroupState;		//by andy
   break;        
   case LCD2_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = InitialSetupBGMState;
     LcdScreen(LCD_BGM_SETUP);
     //  SetLED(BACKGROUND_MUSIC_LED);
     break;
   case LCD1_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = InitialSetupGroupState;
     LcdScreen(LCD_GROUP_SETUP);
     //  SetLED(GROUP_LED);
     break;

   case LCD3_KEY:
     InitialSoftTimer(SetupSoftTimer);
     ProcState = SetupModeSelectSpecialState;
     LcdScreen(LCD_SETUP_SMODE);
     break;
   
   case BACKLIGHT_KEY: 
     InitialSoftTimer(SetupSoftTimer);
     LcdScreen(LCD_BACKLIGHT);
     break;
   }    
// Entry Setup State : check Input 4 digital number password
#define PasswordCount GenCount2
switch (KeyCode) {
	case LCD4_KEY:
		IsPasswordOK = FALSE;
		PasswordCount = 5;
		break;
		default:
			if (KeyCode <= ZONE_KEY10) {
				if (KeyCode < ZONE_KEY10) {
					if ( Read_BYTE_EEPROM(0x670+(PasswordCount-1)) != KeyCode+1 ) {
						IsPasswordOK = FALSE;
					}
				} else {
					if ( Read_BYTE_EEPROM(0x670+(PasswordCount-1)) != 0 ) {
						IsPasswordOK = FALSE;
					}
				}
				PasswordCount++;
				LcdScreen(LCD_SHOWSTAR);
				IsStartSoftTimer1 = OFF;
				InitialSoftTimer(SoftTimer1);
				IsStartSoftTimer1 = ON;
			} /* if (KeyCode <= ZONE_KEY10) end */
			break;
} // switch (KeyCode) end

			if (PasswordCount == 0) {
				// SetFlash(SETUP_LED);
				IsPasswordOK = TRUE;
				InitialSoftTimer(SoftTimer1);
				IsStartSoftTimer1 = ON;
				PasswordCount++;
			}

			if (PasswordCount >= 5) {
				IsStartSoftTimer1 = OFF;
				//       ClrFlash(SETUP_LED);
				PasswordCount = 0;
				InitialSoftTimer(SoftTimer1);
				if (IsPasswordOK) {
					ProcState = InitialEntrySetupState;
					LcdScreen(LCD_BL_AUTO_OFF);
					LCDDelay();
					LcdScreen(LCD_QUIT_SETUP);
				} else  {
					ProcState = ExitEntrySetupState;
				}
			} // if (PasswordCount) end

			if (SoftTimer1.sec >= 4) {
				IsStartSoftTimer1 = OFF;
				// ClrFlash(SETUP_LED);
				ProcState = ExitEntrySetupState;
				PasswordCount = 0;
				InitialSoftTimer(SoftTimer1);
			}
			#undef PasswordCount

  if (GetFromQue(P1FIFOIn, &Item)) {
    switch(Item) {
    case SYSTEM_RESET: // main  send initial
      break;
    case KEY_PROC:
      #include "Key.C"
      break;
    case GROUP_PROC:
      #include "Group.C"
      break;
    case EMG_PROC:
      #include "Emergency.C"
      break;
    case SHORT_PROC:
      #include "Short.C"
    case XFUNC_PROC:
      #include "XFunc.C"
      break;
    case DISPLAY_PROC:
      #include "DISPLAY.C"
      // Response ACK to P1 Bus
      break;
    case EMGTEL_PROC:
      #include "EmergencyTel.C"
      break;
    case FIRE_PROC:
      #include "Fire.C"
      break;
    case SETUP_PROC:
      #include "Setup.C"
      break;
    case EXIST_PROC:
      #include "Exist.C"
      break;
    case BGM_PROC:
      #include "BGM.C"
      break;
    case QUITALARM_PROC:
      #include "QuitAlarm.C"
      break;
    case MANU_PROC:
      #include "MANU.C"
      break;
    case PTT_PROC:
      #include "PTT.C"
      break;
    case SPC_PROC:
      #include "SPCOMM.C"
      break;
    }  // end switch(Item)
  }
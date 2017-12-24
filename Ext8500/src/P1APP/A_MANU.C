  case CMD_ALLCALL_MANU:
    {
      BYTE nCount;
      for (nCount = 0; nCount < MAX_ZONE; nCount++) {
        SetLED(ZONE_ACTIVE_LED + nCount);
      }
    }
    break;

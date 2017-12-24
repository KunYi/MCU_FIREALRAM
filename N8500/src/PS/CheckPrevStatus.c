// Check Prev Power off status
    
    switch(StorageStatus) {  
    case PreFireEmergencyStorageStatus: /* Pre emergency */
      ProcState = InitialPreFireEmergencyState;
      IsPreEmergencyMode = TRUE;
      goto ProcessEmergencyStatus;
    case FireEmergencyStorageStatus:   /* Fire emergency */
      ProcState = InitialConfirmFireEmergencyState;
      goto ProcessEmergencyStatus;
    case NoFireEmergencyStorageStatus: /* No fire emergency */
      ProcState = InitialConfirmNoFireEmergencyState;
      goto ProcessEmergencyStatus;
    case AllZoneEmergencyStorageStatus: /* All zone emergency */
      ProcState = InitialAllZoneEmergencyState;
      goto ProcessEmergencyStatus;
//------------------------------------------------------------------      
ProcessEmergencyStatus: {   
  FireZoneBuffer[1] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireStatusStorage, 0, 0));
  FireZoneBuffer[0] = ~Read_BYTE_EEPROM(CalcEEPROMOffset(FireStatusStorage, 0, 0)+1);
  
  SetFireRelationZone();  
} /* ProcessEmergencyStatus */
//------------------------------------------------------------------      
      break;
    default:   /* Normal emergency */
      ProcState = EntryNormalState;
      StorageStatus = NormalStorageStatus;
      break;
    }
  GenCount2 = 0;

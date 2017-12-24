#define _PHYSCICAL_COMM_                  
#include "communication.h"
#include "sensor.h"

//extern BOOLEAN _PutToKeyFIFO(BYTE Val);

#define ExtendBufferLength 7
#define CallstationBfferLength 6

bit SyncState      = FALSE;


static P_INTERNAL_DATA BYTE ExtendBuffer[ExtendBufferLength];
static P_INTERNAL_DATA BYTE CallStationBuffer[CallstationBfferLength];
static P_INTERNAL_DATA BYTE PreID = 0;
static P_INTERNAL_DATA BYTE PreCallStationID = 0;
static P_INTERNAL_DATA BYTE StartCount = 0;
bit PackageReceive = FALSE;
static bit CallStationReceive = FALSE;

void ExtendUnit_Receive(void) {
  static P_INTERNAL_DATA WORD wait_timer;
  static P_INTERNAL_DATA BYTE ReceiveLength;
  static P_INTERNAL_DATA BYTE PackageCheckSum;
  static bit LengthState    = FALSE;
  static bit LengthOK       = FALSE;
  BYTE Length;
  BYTE ReceiveTemp;
 
  /* ============================================================= */ 
  Length = ExtendPort_Receive_Length();
  while (Length) {
    if (ExtendPort_Get(&ReceiveTemp)) { /* Is get byte success  */
      wait_timer = GetSystemTimer();
      if (SyncState) {                  /* SyncState is true    */
        if (LengthState) {             /* LengthState is true  */
          PackageCheckSum += ReceiveTemp;
          ReceiveLength--;
          //if (LengthOK)               /* LengthOK is true     */
          //  { 
          if ( ReceiveLength == 0 ) {
            SyncState   = FALSE;
            LengthOK    = FALSE;
            LengthState = FALSE;
            if (PackageCheckSum == 0) { 
              PackageReceive = TRUE; 
            }
          } else { 
           ExtendBuffer[ExtendBufferLength-ReceiveLength] = ReceiveTemp; 
          }
          break; 
          //}
          //else 
          // { 
          // if (ReceiveLength == 0 ) { break; } 
          // }  
        } else {                         /* LengthState is flase */
          LengthState = TRUE;
          ReceiveLength = ReceiveTemp;
          if ( Package_Length == ReceiveLength ) {/* Package length is  right */
            LengthOK = TRUE; 
            PackageCheckSum = ReceiveLength; 
          }
          ReceiveLength--;  
        }    
      } else  {  /* SyncState is flase   */
        if (Package_SYNC == ReceiveTemp) { SyncState = TRUE; }
      }
    }
    Length--;  
  }
  /* one package be byte to byte timer > 3 ms then package fail */
  if (SyncState) {
    if ((GetSystemTimer() - wait_timer) > 3) { 
      SyncState   = FALSE; 
      LengthOK    = FALSE;
      LengthState = FALSE;
    }
  }
}    

void CallStation_Receive(void)
{
 static P_INTERNAL_DATA WORD wait_timer;
// static bit PackageReceive = FALSE;

 BYTE ReceiveTemp;
 BYTE Length;
 Length = CallStationPort_Receive_Length();

 while (Length)
      {
        if (CallStationPort_Get(&ReceiveTemp))
          { 
           CallStationBuffer[StartCount++] = ReceiveTemp;
           if (StartCount >= 6) { StartCount = 0; CallStationReceive = TRUE; }
           wait_timer = GetSystemTimer();
          }
        Length--;         
      }
    /* one package be byte to byte timer > 200 ms then package fail */
   if ((GetSystemTimer() - wait_timer) > 200)
     {
       if (StartCount > 0 )
         {  //ResetCallStationBus();
          StartCount = 0; }
     }
}

Status ProcCallStation(const Status CallStationState, BYTE *CBuffer)
{
  if (CallStationReceive)
    {
     CallStationReceive = FALSE;     
/* function byte */       
     switch (CallStationBuffer[2]) 
      {
       case 0x00:  
       	if((CallStationBuffer[3] == 'F')&&(CallStationBuffer[4]&0xf0 == 'A')){						//	CallStation Ans Fire
//       		CallStationState = CallStationFirePlayState;
       		if(CallStationState == CallStationFirePlayState){return (CallStationNormalState);}
       		else {return (CallStationFirePlayState);}
       		}
	    break;
       case 0x01:  
      
       	if(CallStationBuffer[4]>= 0x80){		//	CallStation Talk
       		PreCallStationID = CallStationBuffer[4] & 0x0f;
      		CallStationTalk(PreCallStationID);
//       		CallStationState = CallStationTalkState;
//       		if(CallStationState == CallStationZoneState){return (CallStationZoneTalkState);}
       		return (CallStationTalkState);
       		}
       	if(CallStationBuffer[4]<= 0x80){		//	CallStation Talk Ok
       		PreCallStationID = CallStationBuffer[4] & 0x0f;
       		CallStationTalk(PreCallStationID);
//       		CallStationState = CallStationTalkState;
//       		if(CallStationState == CallStationZoneTalkState){return (CallStationZoneExitState);}
//       		if(CallStationState == CallStationModeState){return (ExitCallStationModeState);}

       		   return (CallStationNormalState);
       		}
       break;
//       break;

       case 0x02:  
//       	if(CallStationBuffer[3] == 'Z'){																											//	CallStation Zone Select
 //    		CallStationZoneSelect();
           *(CBuffer+1) = CallStationBuffer[3];
           *(CBuffer+2) = CallStationBuffer[4];
//       		if(CallStationState == CallStationZoneState){return (CallStationZoneState);}
        		return (CallStationZoneState);
//      		}
       break;
       case 0x03:  
//       	if(CallStationBuffer[3] == 'G'){																											//	CallStation Group Select
			  CallStationGroupSelect();
           *(CBuffer+2) = CallStationBuffer[3] & 0x03 ;
           *(CBuffer+1) = CallStationBuffer[4] & 0xff;
       return (CallStationActiveState);

//       		}
       break;
       case 0x04:  
       	if((CallStationBuffer[3] == 'A')&&(CallStationBuffer[4] == 0xff)){						//	CallStation All Call
       		CallStationAllCall();
           *(CBuffer+1) = 0xff;
           *(CBuffer+2) = 0xff;
           return (CallStationActiveState);
       		}
       break;
       case 0x05:  
       	if((CallStationBuffer[3] == 'C')&&(CallStationBuffer[4] == 0xff)){						//	CallStation Chime UP
       		CallStationChimeUP();
//       		CallStationState = CallStationTalkState;
       		return (CallStationChimeUpState);
       		}
       	if((CallStationBuffer[3] == 'C')&&(CallStationBuffer[4] == 0x00)){					//	CallStation Chime Dowm
       		CallStationChimeDown();
       		return (CallStationChimeDownState);
       		}
       break;

       case 0x06:  
       	if((CallStationBuffer[3] == 'B')&&(CallStationBuffer[4] == 0xff)){						//	CallStation Ans MPU BGM Play
       		CallStationMPUBGMON();
//       		if(CallStationState == CallStationNormalState){return (CallStationBGMONState);}
       		return (CallStationNormalState);
       		}
       	if((CallStationBuffer[3] == 'B')&&(CallStationBuffer[4] == 0x00)){					//	CallStation Ans MPU BGM Play OK
       		CallStationMPUBGMOFF();
//       		return (CallStationBGMOFFState);
       		return (CallStationNormalState);
       		}
       break;

		 case 0x07:
       	if(CallStationBuffer[4] >= 0x80){		//	CallStation BGM Play
	     		PreCallStationID = CallStationBuffer[4]&0x0f;
	     		CallStationBGMON();
       		return (CallStationBGMONState);
       		}
      	if(CallStationBuffer[4] <= 0x80){		//	CallStation BGM Play OK
	     		PreCallStationID = CallStationBuffer[4]&0x0f;
	     		CallStationBGMOFF(PreCallStationID);
//       		CallStationState = CallStationTalkState;
//       		if(CallStationState == CallStationChangTalkState){return (CallStationMPUBGMOFFState);}
//       		if(CallStationState == CallStationBGMONState){return (CallStationBGMOFFState);}
				return (CallStationBGMOFFState);
       		}
       break;
		 case 0x08:
       	if((CallStationBuffer[3] == 'T')&&(CallStationBuffer[4] == 'A')){							//	CallStation Ans MPU Talk
       	if(CallStationState == CallStationNormalState){return (CallStationMPUTalkState);}
       	else 	if(CallStationState == CallStationMPUTalkState){return (CallStationNormalState);}
       	else 	if(CallStationState == CallStationChangTalkState){return (CallStationBGMONState);}
       	else 	if(CallStationState == CallStationBGMONState){return (CallStationChangTalkState);}
       		}
       break;
		 case 0x09:
       	if(CallStationBuffer[4] == 0xff){			//	CallStation Power On
       		PreCallStationID = CallStationBuffer[3]&0x0f;
//       		CallStationState = CallStationNormalPlayState;
//       		CallStationPowerOnOk(PreCallStationID);
       		return (CallStationPowerONState);
       		}
       	if(CallStationBuffer[4] == 0x88){			//	CallStation Power On Error
	     		PreCallStationID = CallStationBuffer[3]&0x0f;
//       		CallStationState = CallStationErrorState;
//       		CallStationPowerOnError(PreCallStationID);
//       		return (CallStationErrorState);
       		return (CallStationNormalState);
       		}
	        break;
		 case 0x0a:
       	if((CallStationBuffer[3]& 0xf0 == 'N')&&(CallStationBuffer[4] == 'A')){			//	CallStation Ans Power On Ok
//       		CallStationPowerOnError(PreCallStationID);
//       		return (CallStationErrorState);
       		return (CallStationNormalState);
       		}
       break;
		 case 0x11:

       	if(CallStationBuffer[4]<= 0x80){		//	CallStation Talk Ok
       		PreCallStationID = CallStationBuffer[4] & 0x0f;
       		CallStationTalk(PreCallStationID);
//       		CallStationState = CallStationTalkState;
//       		if(CallStationState == CallStationZoneTalkState){return (CallStationZoneExitState);}
//       		if(CallStationState == CallStationModeState){return (ExitCallStationModeState);}

       		   return (CallStationNormalState);
       		}
       break;
/*
       		if(CallStationState == CallStationBGMState){
	       		return (CallStationBGMState);
	       		}
       break;
*/
		 case 0x17:
      	if(CallStationBuffer[4] <= 0x80){		//	CallStation BGM Play OK
	     		PreCallStationID = CallStationBuffer[4]&0x0f;
	     		CallStationBGMOFF(PreCallStationID);
//       		CallStationState = CallStationTalkState;
//       		if(CallStationState == CallStationChangTalkState){return (CallStationMPUBGMOFFState);}
//       		if(CallStationState == CallStationBGMONState){return (CallStationBGMOFFState);}
				return (CallStationBGMOFFState);
		      break;
       		}

       default: /* Transmit package error */
	       return EOF;
       break;
      }
     
    }
  return EOF;
}

#define SPC_COMMAND        (0xAA)
#define SPC_NORMAL          (255)
#define SPC_ENTRY_FIRE      (254)
#define SPC_CONFIRM_FIRE    (253)
#define SPC_NOT_FIRE        (252)
#define GWMASTER_ID        (0x40)

void SPECIAL_CMD(BYTE CMD, BYTE parameter1, BYTE parameter2) {
  BYTE Chksum;

    
  ExtendPort_Put(Package_SYNC);
  switch(CMD) {
  case SPC_NORMAL:
    ExtendPort_Put(Chksum = 0x05); // length
    break;
  case SPC_ENTRY_FIRE:    
  case SPC_CONFIRM_FIRE:
  case SPC_NOT_FIRE:
    ExtendPort_Put(Chksum = 0x07); // length
    break;
  }

  ExtendPort_Put(GWMASTER_ID);   // receid
  Chksum += GWMASTER_ID;
  
  ExtendPort_Put(SPC_COMMAND);          // Special command;
  Chksum += SPC_COMMAND;
  
  ExtendPort_Put(CMD);           // sub command
  Chksum += CMD;

  switch(CMD) {
  case SPC_ENTRY_FIRE:    
  case SPC_CONFIRM_FIRE:
  case SPC_NOT_FIRE:
    
    Chksum += parameter1; 
    ExtendPort_Put(parameter1);

    Chksum += parameter2;       
    ExtendPort_Put(parameter2);
    break;
  }
  ExtendPort_Put(~Chksum+1);       
}

BOOLEAN ExtendUnit_Transmit(const BYTE Number,
                            const BYTE parameter1, const BYTE parameter2, 
                            const BYTE parameter3, const BYTE parameter4, const BYTE ID) {
  int     i = 0;
  BYTE    Chksum;
  
  //BYTE    SendID;
  //BYTE    SendCmd1;
  //BYTE    SendCmd2;

  ExtendPort_Put(Package_SYNC);
  ExtendPort_Put(Chksum = Package_Length);
  
  if ( ID > 0 ) {
    if (Extend_Package[Number][i++] == All_Extend) { 
      ExtendPort_Put(ID+0x20); 
      Chksum += (ID+0x20); 
    } else { 
      ExtendPort_Put(ID); 
      Chksum += ID; 
    }
  } else {
    // if ID == 0
    if (Number == IsExistPackage) { 
      ExtendPort_Put(0x20);
      Chksum += 0x20; 
      i++; 
    } else {  
      if (Extend_Package[Number][i++] == All_Extend) { 
        if (PreID < 0x20) { 
          ExtendPort_Put(PreID+0x20); 
          Chksum += (PreID+0x20); 
        } else if (PreID == 0x40) {
          ExtendPort_Put(0x40);
          Chksum += 0x40;
        }
      } else { 
        ExtendPort_Put(PreID); 
        Chksum += PreID; 
      }
      // if (PreID > 0x0f) { return FALSE; }
    }
  }
     
  Chksum += Extend_Package[Number][i]; 
  ExtendPort_Put(Extend_Package[Number][i++]);
  Chksum += Extend_Package[Number][i]; 
  ExtendPort_Put(Extend_Package[Number][i++]);
   
  switch (Extend_Package[Number][i++]) {
  case  ParameterFixed:  
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);
       
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);
       
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);

    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i]);
       
    ExtendPort_Put((~Chksum)+1);     
    break;
  case ParameterField1 :
    // fixed paramter of ahead two and fourth 
    // third is a variable
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);
       
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);

    Chksum += parameter1; 
    ExtendPort_Put(parameter1);
    i++;
        
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i]);
       
    ExtendPort_Put(~Chksum+1);     
    break;

  case ParameterField2 :    
    // fixed parameter of ahead three
    // fourth parameter is variable
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);
       
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);
     
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i]);

    Chksum += parameter1; 
    ExtendPort_Put(parameter1);
       
    ExtendPort_Put(~Chksum+1);
    break;

  case ParameterField3:
    // fixed parameter ahead pre-two
    // 3'ed and 4'th is variable
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);
       
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);
       
    Chksum += parameter1; 
    ExtendPort_Put(parameter1);

    Chksum += parameter2;       
    ExtendPort_Put(parameter2);
    ExtendPort_Put(~Chksum+1);       
    break;

  case ParameterField4 :    
    Chksum += parameter1; 
    ExtendPort_Put(parameter1);
    i++;
	   
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);

    Chksum += parameter2;       
    ExtendPort_Put(parameter2);
    i++;
	   
    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i++]);
       
    ExtendPort_Put(~Chksum+1);       	 
    break;
     
  case ParameterField5 :
    Chksum += PreID;
    ExtendPort_Put(PreID);
    i++;

    Chksum += Extend_Package[Number][i]; 
    ExtendPort_Put(Extend_Package[Number][i]);
   
    Chksum += parameter1; 
    ExtendPort_Put(parameter1);
          
    Chksum += parameter2;       
    ExtendPort_Put(parameter2);

    ExtendPort_Put(~Chksum+1);     
    break;

  case ParameterField6 :    
    Chksum += parameter1; 
    ExtendPort_Put(parameter1);
 
    Chksum += parameter2; 
    ExtendPort_Put(parameter2);
 
    Chksum += parameter3; 
    ExtendPort_Put(parameter3);
 
    Chksum += parameter4; 
    ExtendPort_Put(parameter4);
 
    ExtendPort_Put(~Chksum+1);     
    break;
  }  
  return TRUE;
}

BOOLEAN CallStation_Transmit(const BYTE Number, const BYTE ID)
{
  int     i = 0;
  BYTE    Chksum = 0;

  CallStationPort_Put(Package_SYNC);
  
  if ( ID > 0 )
    {
    if (CallStation_Package[Number][i++] == All_CallStation)
      { CallStationPort_Put(ID+0x10); 
        Chksum += (ID+0x10); }
    else
      { CallStationPort_Put(ID); 
        Chksum += ID; }
    }
 else
    {
       if (CallStation_Package[Number][i++] == All_CallStation)
         { CallStationPort_Put(PreCallStationID+0x10); 
           Chksum += (PreCallStationID+0x10); }
       else
         { CallStationPort_Put(PreCallStationID); 
           Chksum += PreCallStationID; }
   if (PreCallStationID > 0x05) { return FALSE; }
     
     }
  Chksum += CallStation_Package[Number][i]; 																		// Func Byte
  CallStationPort_Put(CallStation_Package[Number][i++]);

  switch (CallStation_Package[Number][i++])
  {  case  DataFixed:  
       Chksum += CallStation_Package[Number][i]; 
       CallStationPort_Put(CallStation_Package[Number][i++]);
       Chksum += CallStation_Package[Number][i]; 
       CallStationPort_Put(CallStation_Package[Number][i++]);
       
       CallStationPort_Put((~Chksum)+1);
       return CallStationActiveState;
     break;
       
		case  DataFixed1:  																								// Last Data + UseID
       Chksum += CallStation_Package[Number][i]; 
       CallStationPort_Put(CallStation_Package[Number][i++]);
       Chksum += (CallStation_Package[Number][i]+PreCallStationID); 
       CallStationPort_Put((CallStation_Package[Number][i++]+PreCallStationID));
       
       CallStationPort_Put((~Chksum)+1);
       return CallStationActiveState;
     break;

		case  DataFixed2:  																								// Last Data + UseID
       Chksum += CallStation_Package[Number][i]; 
       CallStationPort_Put(CallStation_Package[Number][i++]);
       Chksum += (CallStation_Package[Number][i]+PreCallStationID); 
       CallStationPort_Put((CallStation_Package[Number][i++]+PreCallStationID));
       
       CallStationPort_Put((~Chksum)+1);
       return CallStationActiveState;;
     break;
     case 3 :    break;
     case 4 :    break;
     case 5 :    break;
   }  
  return EOF;
}

#define ProcState state
Status ProcExtendUnit(const Status state,BYTE *SetupBuffer, BYTE *FireZoneBuffer)  {
  if (PackageReceive)  {
    PackageReceive = FALSE;
    switch (ExtendBuffer[1]) {
    case 0xAA: /* SS Command Response*/
      if (ExtendBuffer[2] == 0x55) {
        // GW Master ACK
        break; 
      }  
    case 'B': /* Background music */
      /* Receive package          */
      if (ExtendBuffer[2] == 'R')  {
        /* Extend Unit receive entry background music mode package ok    */
        if ( ExtendBuffer[3] == 'M')  {
          if ((ExtendBuffer[4] == 'O') &&
              (ExtendBuffer[5] == 'D') &&
              (ExtendBuffer[6] == 'E') &&
              (InitialBGMPlayState == ProcState))  {
            return InitialBGMPlayState;
          }
        }
      } else if (ExtendBuffer[3] == 'Q')  {
        /* Extend Unit receive quit background music mode package ok     */
        if ((ExtendBuffer[4] == 'U') &&
            (ExtendBuffer[5] == 'I') &&
            (ExtendBuffer[6] == 'T')) {
          switch(ProcState) {
          case ExitBGMPlayState:
            return ExitBGMPlayState;
          case ChangeGroupState:
            return ChangeGroupState;
          case ChangeHumanHandlingState:
            return ChangeHumanHandlingState;
          case ChangeCallStationModeState: 
            return ChangeCallStationModeState;
          }
        }
      }  
      break;
    case  'C':  /* Alarm remove */
      /* Receive package                */
      if (ExtendBuffer[2] == 'R') {
        if ((ExtendBuffer[3] == 'P') &&
            (ExtendBuffer[4] == 'U') &&
            (ExtendBuffer[5] == 'S') &&
            (ExtendBuffer[6] == 'H') &&
            (InitialCheckIsAlarmHoldState == ProcState)) { 
          return InitialCheckIsAlarmHoldState; 
        }
      } else if (CallStationBuffer[2] == 'R')  {
        if (ExtendBuffer[6] == 0xff) {
          /*  Set zone key   */
          return SetCallStationSelectZoneState;
        } else if ( ExtendBuffer[6] == 0x00) {
          /* Reset zone key */
          return ResetCallStationSelectZoneState;
        }
      }
      break;
    case 'E': /* Emergency       */
     /* Receive package                */ 
      if (ExtendBuffer[2] == 'R')  {
        /* Extend unit receive entry emergency public address package ok */
        if ( ExtendBuffer[3] == 'M')  { 
          if ((ExtendBuffer[4] == 'O') && 
              (ExtendBuffer[5] == 'D') &&
              (ExtendBuffer[6] == 'E') &&
              (InitialAllZoneEmergencyState == ProcState)) { 
            return InitialAllZoneEmergencyState; 
          } 
        } else if (ExtendBuffer[3] == 'Q')  {  
        /* Extend unit receive quit emergency public address package ok  */
          if ((ExtendBuffer[4] == 'U') && 
              (ExtendBuffer[5] == 'I') &&
              (ExtendBuffer[6] == 'T') &&
              (ExitAllZoneEmergencyState == ProcState)) { 
            return ExitAllZoneEmergencyState; 
          } 
        }
      }
      break;
    case 'F': /* Fire alarm      */
      /* Extend unit action fire alarm  */
      if (ExtendBuffer[2] == 'A') {
        if (ExtendBuffer[4] == 0) { 
        /* Get fire alarm action extend unit ID */
        /* Fire_alarm_ID = ExtendBuffer[3]      */
        /* Get extend unit fire alarm state     */
        /* Fire_alarm_state = ExtendBuffer[5]  
           only use bit 1, 0  (Zone 10, 9)
           Fire_alarm_state = ExtendBuffer[6]   
           use bit 7...0 (ZONE8...ZONE1)        */
          if ((ExtendBuffer[3] > 0) &&
   	      (ExtendBuffer[3] < (MaxExtendUnit+1))) {
            PreID = ExtendBuffer[3];
//	    			     ExtendBuffer[5] &= 0x03;
            FireZoneBuffer[PreID*2] |= ExtendBuffer[5];
            FireZoneBuffer[(PreID*2)+1] |= ExtendBuffer[6];                            
            FireZoneBuffer[((MaxExtendUnit+1)*2)+0] = PreID;
            FireZoneBuffer[((MaxExtendUnit+1)*2)+1] = ExtendBuffer[5];
            FireZoneBuffer[((MaxExtendUnit+1)*2)+2] = ExtendBuffer[6];
	    				          
            for (ExtendBuffer[4] = 0; ExtendBuffer[4] < 150;) { 
              ExtendBuffer[4]++; 
            }
                        
            ReplyFireAlram(ExtendBuffer[5],ExtendBuffer[6]);

            if ((ExtendBuffer[5] != 0) || (ExtendBuffer[6] != 0)) {
              return ExtendUnitFireActiveState;
            } else { 
              return EOF; 
            }
          }  
        }
      } else if (ExtendBuffer[2] == 'R') {
      /* Receive package                            */
        if ((ExtendBuffer[3] == 0) &&
            (ExtendBuffer[4] == 0)) {
          /* Extend unit receive action fire alarm package ok                */
          /* compare state */
          /* Trigger fire alarm    */
          return FireAlarmActiveState;
        }             
      }
      break;
    case 'G': /* Group           */
    /* Group receive                          */
      if ( 'R' == ExtendBuffer[2])  {
        if ( ExtendBuffer[3] == 'M') {  
      /* Extend unit receive group public address package ok         */
          if ((ExtendBuffer[4] == 'O') &&
              (ExtendBuffer[5] == 'D') &&
              (ExtendBuffer[6] == 'E') &&
              (InitialGroupState == ProcState)) {
            return InitialGroupState; 
          }
        } else if (ExtendBuffer[3] == 'Q')  {  
          if ((ExtendBuffer[4] == 'U') &&
              (ExtendBuffer[5] == 'I') &&
              (ExtendBuffer[6] == 'T') &&
              (ExitGroupState == ProcState)) {
              return ExitGroupState; 
          }                       
        } else if ((ExtendBuffer[3] == 'G') &&
                   (ExtendBuffer[4] == 'R') &&
                   (InitialCallStationModeState == ProcState))  {
          return InitialCallStationModeState; 
        } else if (ExtendBuffer[3] == 0) {  
        /* Extend unit receive choice group number package ok          */ 
          if ((ExtendBuffer[4] == 0) &&
              (ExtendBuffer[5] == 0) &&
              (SetGroupState == ProcState))  { 
            return SetGroupState; 
          }
        }
      }
      break;
    case 'H': /* Man handling    */
      /* Receive package                         */
      if (ExtendBuffer[2] == 'R') {
        if (ExtendBuffer[3] == 'M') {
          /* Extend unit receive man handling mode package               */
          if ((ExtendBuffer[4] == 'O') &&
              (ExtendBuffer[5] == 'D') &&
              (ExtendBuffer[6] == 'E')) { 
          }
        } else if (ExtendBuffer[3] == 'Q')  {
          /* Extend unit receive quit man handling mode package ok       */
          if ((ExtendBuffer[4] == 'U') &&
              (ExtendBuffer[5] == 'I') &&
              (ExtendBuffer[6] == 'T'))  { 
          }
        }
      }
      break;
    case 'K': /* Key             */
      /* Action / Confirm & Autoalarm   */
      if (ExtendBuffer[2] == 'A') {
        if (ExtendBuffer[4] == 0) {
        /* Set extend unit ID */
      	  if ((ExtendBuffer[3] > 0) &&
              (ExtendBuffer[3] < 16)) { 
   	        PreID = ExtendBuffer[3]; 
   	      }
					   
          if (ExtendBuffer[6] == 0)  {
            /* Extend unit set zone key   */
            for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)  {
              ExtendBuffer[6]++;
            }
            ReplyKeySet(ExtendBuffer[3],ExtendBuffer[5]);
            ExtendBuffer[6] = 0;
          } else if ( ExtendBuffer[6] == 0xff) {
            /* Extend unit reset zone key */
            for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;) {
              ExtendBuffer[6]++; 
            }
            ReplyKeyReset(ExtendBuffer[3],ExtendBuffer[5]);
     	    ExtendBuffer[6] = 0xff;
          }
        }
      } else if (ExtendBuffer[2] == 'R') {
        /* Receive package              */
        if ((ExtendBuffer[3] == 0) && 
            (ExtendBuffer[4] == 0)) {
          if (ExtendBuffer[6] == 0) {
            switch(ProcState) {
            case ResetFireRelationZoneState:
              return ResetFireRelationZoneState; 
            case ResetGroupZoneState:
              return ResetGroupZoneState; 
            case ResetBGMZoneState:
              return ResetBGMZoneState; 
            case ResetHumanHandlingZoneState:
              return ResetHumanHandlingZoneState; 
            case ResetGroupState:
              return ResetGroupState; 
            }
          } else if (ExtendBuffer[6] == 0xff) {
            switch(ProcState) {
            case SetFireRelationZoneState:
              return SetFireRelationZoneState; 
            case SetGroupZoneState:
              return SetGroupZoneState; 
            case SetBGMZoneState:
              return SetBGMZoneState;  
            case SetHumanHandlingZoneState:
              return SetHumanHandlingZoneState; 
            case SetGroupState:
              return SetGroupState; 
            }
          }
        }
      }
      break;
    case 'L': /* Circuit control */
      /* Action / Confirm & Autoalarm */
      if (ExtendBuffer[2] == 'A') {
      } else if (ExtendBuffer[2] == 'R') {
      }
      break;
    case 'M': /* Setup           */
      switch (ExtendBuffer[2]) {
      case  'C': /* Exit fire relation setup */
        if ((ExtendBuffer[3] == 'Q') &&
            (ExtendBuffer[4] == 'U') && 
            (ExtendBuffer[5] == 'I') && 
            (ExtendBuffer[6] == 'T') &&
            (ExitSetupFireRelationState == ProcState))  {  
          return ExitSetupFireRelationState; 
        }
        break;
      case  'F': /* Fire zone                    */
        if ((ExtendBuffer[4] == 0) &&
            (ExtendBuffer[6] == 0xff ))  { 
          if ((ExtendBuffer[3] <= 0x0f) && (ExtendBuffer[5] <= 0x0A))  {
            switch (ProcState) {
            case SetupFireRelationZoneState:
              /* Fire relation setup state Extend unit assigned zone action */
              *(SetupBuffer)   = ExtendBuffer[3];
              *(SetupBuffer+1) = ExtendBuffer[5] - 1;
              for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)  { 
                 ExtendBuffer[6]++; 
              }
              ReplyExtendZoneActive(ExtendBuffer[5]);
              return SetupFireRelationZoneState;
            case SetupVoiceZoneState:
              /* Voice message setup state Extend unit assigned zone action */
              *(SetupBuffer)   = ExtendBuffer[3];
              *(SetupBuffer+1) = ExtendBuffer[5] - 1;
              for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)  { 
                ExtendBuffer[6]++;
              }
              ReplyExtendZoneActive(ExtendBuffer[5]);
              return SetupVoiceZoneState;
            case VoiceTestPlayState:
              /* Voice message test play state Extend unit assigned zone action */
              *(SetupBuffer)   = ExtendBuffer[3];
              *(SetupBuffer+1) = ExtendBuffer[5] - 1;
              for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)  { 
                ExtendBuffer[6]++;
              }
              ReplyExtendZoneActive(ExtendBuffer[5]);
              return VoiceTestPlayState;
            }
          }
       }  
       break;
     case  'R': /* Receive package              */ 
       switch (ExtendBuffer[3]) {
       case 0: 
         if ((ExtendBuffer[4] == 0) &&
             (ExtendBuffer[6] == 0xff))  { 
           return ProcState; 
         }
         break;
       case 'M':
         if ((ExtendBuffer[4] == 'O') &&
             (ExtendBuffer[5] == 'D') &&
             (ExtendBuffer[6] == 'E'))  { 
           switch (ProcState) {
           case InitialSetupFireRelationState:
             return InitialSetupFireRelationState;
           case InitialSetupGroupState:
             return InitialSetupGroupState;
           case InitialSetupBGMState:
             return InitialSetupBGMState;
           case InitialSetupVoiceState:
             return  InitialSetupVoiceState;
           case InitialVoiceTestPlayState:
             return  InitialVoiceTestPlayState;
           }
         }
         break;
       case 'O':
         if ((ExtendBuffer[4] == 'K') &&
             (ExtendBuffer[5] ==  0 ) &&
             (InitialState == ProcState) &&
             ( ExtendBuffer[6] <= 0x0f )) { 
           PreID = ExtendBuffer[6]; 
           return InitialState; 
         } 
         break;                      
       case 'Q':
         if ((ExtendBuffer[4] == 'U') &&
             (ExtendBuffer[5] == 'I') &&
             (ExtendBuffer[6] == 'T')) { 
         }
         break;
       case 'S': /* Save setup value */
         if ((ExtendBuffer[4] == 'A') &&
             (ExtendBuffer[5] == 'V') &&
             (ExtendBuffer[6] == 'E'))  {
           switch (ProcState) {
           case InitialSetupFireRelationState:
             return InitialSetupFireRelationState; 
           case InitialSetupGroupState:
             return InitialSetupGroupState; 
           case InitialSetupBGMState:
             return InitialSetupBGMState; 
           case InitialSetupVoiceState:
             return  InitialSetupVoiceState; 
           }
         }
         break;
                    case  'X': /* Try Play                     */
                      if ((ExtendBuffer[3] == 'M') &&
                          (ExtendBuffer[4] == 'O') &&
                          (ExtendBuffer[5] == 'D') &&
                          (ExtendBuffer[6] == 'E'))
                        { }
                        break;
                     }
                   break;
                  }
                  break;
        case 'N': /* None            */
                  break;
        case 'O': /* None            */
                  break;
        case 'P': /* Micphone press push talk */
                  if (ExtendBuffer[2] == 'A')
                    {
                     if ((ExtendBuffer[3] == 'P') &&
                         (ExtendBuffer[4] == 'U') &&
                         (ExtendBuffer[5] == 'S') &&
                         (ExtendBuffer[6] == 'H'))
                       {
                         if (ProcState == HumanHandlingActionState)
                           { return HumanHandlingActionState; }
                       }                        
                    }
                  break;
        case 'Q': /* None            */
                  break;
        case 'R': /* Resetup         */
                  /* Action / Confirm & Autoalarm */
                  if (ExtendBuffer[2] == 'A')
                    {
                    }
                 /* Receive package              */ 
                  else if (ExtendBuffer[2] == 'R') 
                    {  if ((ExtendBuffer[3] == 0) &&
                           (ExtendBuffer[4] == 0) &&
                           (ExtendBuffer[5] == 0) &&
                           (ExtendBuffer[6] == 0))
                         {   }
                    }
                  break;
        case 'S': /* Circuit short   */
            /* Action / Confirm & Autoalarm */
            switch (ExtendBuffer[2])
            {
            default:
                /* if not 'A', 'R' or 'S' Sub Command */
                break;

            case 'A':       
               if (ExtendBuffer[4] == 0)
               { 
                 if ((ExtendBuffer[3] > 0) &&
   			         (ExtendBuffer[3] < 16))
                 { 
                   PreID = ExtendBuffer[3]; 
                 }
                 
                 for (ExtendBuffer[4] = 0; ExtendBuffer[4] < 150;)
                 { 
                    ExtendBuffer[4]++; 
                 }
                   ReplyShort(ExtendBuffer[5], ExtendBuffer[6]);
               }
               break;

            /* Receive package              */
            case 'R':
               if ((ExtendBuffer[3] == 0) &&
                   (ExtendBuffer[4] == 0))
               {
                 return  LoopShortActiveState;
               }
               break;

            /* Remote Push key */
            case 'S':
               // check ID is main unit?
               if ((0 == ExtendBuffer[0]) || (0x20 == ExtendBuffer[0]))
               {
                 if (ExtendBuffer[3] == 0x40)
                 {
                   // Cancel PreID = 0x40 at 12/25/2004
                   // PreID = 0x40;
                   
                   // remote controller tx key value
                   switch (ExtendBuffer[4]) 
                   {
                   case 0x00: // Key
                       PutToQue(KeyFIFO, (ExtendBuffer[6]));
                       ReplyRemoteActionKey(ExtendBuffer[6]);
                       break;

                   case 0x01: // MicPhone Press
                       ReplyRemoteMicPressKey(ExtendBuffer[6]);

                       if (ExtendBuffer[6] == 0xFF)
                       {
                         SetRemoteMicPress();
                       }
                       else if (ExtendBuffer[6] == 0x00)
                       {
                         ClrRemoteMicPress();
                       }
                       break;
                   }
                 }
               }
               break;
            }
            break;
        case 'T': /* Emergency telephone */
                  /* Action / Confirm & Autoalarm */
                  if (ExtendBuffer[2] == 'A') {  
                    if (ExtendBuffer[4] == 0) {   
                      if ((ExtendBuffer[3] > 0) &&
   					      (ExtendBuffer[3] < 16)) {
                        PreID = ExtendBuffer[3];
                        FireZoneBuffer[PreID*2] |= ExtendBuffer[5];
                        FireZoneBuffer[(PreID*2)+1] |= ExtendBuffer[6];                            
                        FireZoneBuffer[((MaxExtendUnit+1)*2)+0] = PreID;
                        FireZoneBuffer[((MaxExtendUnit+1)*2)+1] = ExtendBuffer[5];
                        FireZoneBuffer[((MaxExtendUnit+1)*2)+2] = ExtendBuffer[6];
                        for (ExtendBuffer[4] = 0; ExtendBuffer[4] < 150;) { 
                          ExtendBuffer[4]++; }
                        ReplyEmergencyTel(ExtendBuffer[5], ExtendBuffer[6]);
                        return EmergencyTelState;
                      }
                    }
                  }
                  /* Receive package              */
                  else if (ExtendBuffer[2] == 'R')  {  
                    if (ExtendBuffer[3] == 0)  {
                    }
                  }
                  break;
      
       }
     }
     return EOF;
}
/*
void DisCallStation(void)
{
//    PreCallStationID = 6;
    CallStationPort_Put(0x60);
    CallStationPort_Put(0x02);
    CallStationPort_Put(0xff);
    CallStationPort_Put(0xff);
}

void ResetCallStationBus(void)  
{
     CallStationPort_Put(0); 
     CallStationPort_Put(0);
     CallStationPort_Put(0);
     CallStationPort_Put(0);
     if (PreCallStationID == 6)
       { PreCallStationID = 0; }
}
*/
BOOLEAN IsReceiveExtendUnit(void)
{
  if (ExtendPort_Receive_Length() > 0 ) { return TRUE; }
  if (SyncState) { return TRUE; }
  return FALSE;
}

BOOLEAN IsReceiveCallStation(void)
{
  if (CallStationPort_Receive_Length() > 0 ) { return TRUE; }
  if (StartCount > 0) { return TRUE; }
  return FALSE;
}

void InitialCommunication(void)
{
 PreID = 0;
 PreCallStationID = 0;
 StartCount = 0;
 PackageReceive = FALSE;
 CallStationReceive = FALSE;
}






















































































































#define _PHYSCICAL_COMM_                  
#include "communication.h"
#define ExtendBufferLength 7
#define CallstationBfferLength 6

bit SyncState      = FLASE;

static P_INTERNAL_DATA BYTE ExtendBuffer[ExtendBufferLength];
static P_INTERNAL_DATA BYTE CallStationBuffer[CallstationBfferLength];
static P_INTERNAL_DATA BYTE PreID = 0;
static P_INTERNAL_DATA BYTE PreCallStationID = 0;
static P_INTERNAL_DATA BYTE StartCount = 0;
bit PackageReceive = FLASE;
static bit CallStationReceive = FLASE;

typedef void (*fun_ptr)(void);               


void ExtendUnit_Receive(void)
{
 static P_INTERNAL_DATA WORD wait_timer;
 static P_INTERNAL_DATA BYTE ReceiveLength;
 static P_INTERNAL_DATA BYTE PackageCheckSum;
 static bit LengthState    = FLASE;
 static bit LengthOK       = FLASE;
 BYTE Length;
 BYTE ReceiveTemp;
 
 /* ============================================================= */ 
 Length = ExtendPort_Receive_Length();
 while (Length)
      {
        if (ExtendPort_Get(&ReceiveTemp)) /* Is get byte success  */
          {
          wait_timer = GetSystemTimer();
          if (SyncState)                  /* SyncState is true    */
            { 
            if (LengthState)              /* LengthState is true  */
              {
                   PackageCheckSum += ReceiveTemp;
                   ReceiveLength--;
                 if (LengthOK)               /* LengthOK is true     */
                   { 
                    if ( ReceiveLength == 0 ) 
                      { SyncState   = FLASE;
                        LengthOK    = FLASE;
                        LengthState = FLASE;
                       if (PackageCheckSum == 0)
                         { 
                           PackageReceive = TRUE; 
                         }
                      }
                    else { ExtendBuffer[ExtendBufferLength-ReceiveLength] = ReceiveTemp; }

                     break; 
                   }
                  else 
                   { 
                   if (ReceiveLength == 0 ) { break; } 
                   }  
              }
            else                          /* LengthState is flase */
              {
               LengthState = TRUE;
               ReceiveLength = ReceiveTemp;
               if ( Package_Length == ReceiveLength ) /* Package length is  right */
                 { LengthOK = TRUE; PackageCheckSum = ReceiveLength; }
               ReceiveLength--;  
              }    
            }
          else                            /* SyncState is flase   */
            { 
             if (Package_SYNC == ReceiveTemp) { SyncState = TRUE; }
            }
          }
        Length--;  
      }
     /* one package be byte to byte timer > 3 ms then package fail */
     if (SyncState)
       {
        if ((GetSystemTimer() - wait_timer) > 3)
          { 
           SyncState   = FLASE; 
           LengthOK    = FLASE;
           LengthState = FLASE;
          }
       }
}    

void CallStation_Receive(void)
{
 static P_INTERNAL_DATA WORD wait_timer;
// static bit PackageReceive = FLASE;

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
     CallStationReceive = FLASE;     
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

BOOLEAN ExtendUnit_Transmit(const BYTE Number,
                            const BYTE parameter1, const BYTE parameter2, 
                            const BYTE parameter3, const BYTE parameter4, const BYTE ID)
{
  int     i = 0;
  BYTE    Chksum;



  ExtendPort_Put(Package_SYNC);
  ExtendPort_Put(Chksum = Package_Length);
  
  if ( ID > 0 )
    {
    if (Extend_Package[Number][i++] == All_Extend)
      { ExtendPort_Put(ID+0x20); 
        Chksum += (ID+0x20); }
    else
      { ExtendPort_Put(ID); 
        Chksum += ID; }
    }
 else
    {
    if (Number == IsExistPackage)
      { 
        ExtendPort_Put(0x20);
        Chksum += 0x20; 
        i++; }
    else
      {  
       if (Extend_Package[Number][i++] == All_Extend)
         { ExtendPort_Put(PreID+0x20); 
           Chksum += (PreID+0x20); }
       else
         { ExtendPort_Put(PreID); 
           Chksum += PreID; }
       if (PreID > 0x0f) { return FLASE; }
      }
     }
     
  Chksum += Extend_Package[Number][i]; 
  ExtendPort_Put(Extend_Package[Number][i++]);
  Chksum += Extend_Package[Number][i]; 
  ExtendPort_Put(Extend_Package[Number][i++]);
   
  switch (Extend_Package[Number][i++])
  {  case  ParameterFixed:  
       
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
       Chksum += Extend_Package[Number][i]; 
       ExtendPort_Put(Extend_Package[Number][i++]);
       
       Chksum += Extend_Package[Number][i]; 
       ExtendPort_Put(Extend_Package[Number][i++]);
     
       Chksum += Extend_Package[Number][i]; 
       ExtendPort_Put(Extend_Package[Number][i++]);

       Chksum += parameter1; 
       ExtendPort_Put(parameter1);
       
       ExtendPort_Put(~Chksum+1);
     break;

     case ParameterField3:    
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
     case 6 :    break;
     case 7 :    break;
     case 8 :    break;
     case 9 :
      Chksum += parameter3;       
      Chksum += parameter4;       
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
   if (PreCallStationID > 0x05) { return FLASE; }
     
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
Status ProcExtendUnit(const Status state,BYTE *SetupBuffer, BYTE *FireZoneBuffer)
    {
     if (PackageReceive)
       {
         PackageReceive = FLASE;
        switch (ExtendBuffer[1]) {
        case 'B': /* Brackground music */
                  /* Receive package                */
                  if (ExtendBuffer[2] == 'R')
                    {
                    /* Extend Unit receive 
                       entry background music mode package ok    */
                    if ( ExtendBuffer[3] == 'M')
                      {  if ((ExtendBuffer[4] == 'O') &&
                             (ExtendBuffer[5] == 'D') &&
                             (ExtendBuffer[6] == 'E'))
                           { 
                            if (ProcState == InitialBGMPlayState) 
                              {
                                return InitialBGMPlayState;
                              }
                           }
                      }
                    /* Extend Unit receive
                       quit background music mode package ok     */
                    else if (ExtendBuffer[3] == 'Q')
                      {  if ((ExtendBuffer[4] == 'U') &&
                             (ExtendBuffer[5] == 'I') &&
                             (ExtendBuffer[6] == 'T'))
                           { 
                            if (ProcState == ExitBGMPlayState) 
                              { return ExitBGMPlayState
                              ;  }
                            else if (ProcState == ChangeGroupState)
                              { return ChangeGroupState; }
                            else if (ProcState == ChangeHumanHandlingState)
                              { return ChangeHumanHandlingState; }
                            else if (ProcState == ChangeCallStationModeState)
                              { return ChangeCallStationModeState; }
                           }
                      }
                    else
                      { /* no define */
                      }
                    }  
                  break;
        case  'C':  /* Alarm remove */
                  /* Receive package                */
                  if (ExtendBuffer[2] == 'R')
                    {
                    if ((ExtendBuffer[3] == 'P') &&
                        (ExtendBuffer[4] == 'U') &&
                        (ExtendBuffer[5] == 'S') &&
                        (ExtendBuffer[6] == 'H'))
                      { 
                        if (ProcState == InitialCheckIsAlarmHoldState)
                          { return InitialCheckIsAlarmHoldState; }
                      }
                    }


                  else if (CallStationBuffer[2] == 'R')
                    {
                      /*  Set zone key   */
                      if (ExtendBuffer[6] == 0xff)
                        {
       			return SetCallStationSelectZoneState;
                        }
                      /* Reset zone key */
                      else if ( ExtendBuffer[6] == 0x00)
                        {
                        return ResetCallStationSelectZoneState;
                        }
                      }

                  break;
        case 'D': /* None            */
                  break;
        case 'E': /* Emergency       */
                  /* Receive package                */ 
                  if (ExtendBuffer[2] == 'R')
                    {
                  /* Extend unit receive
                     entry emergency public address package ok */
                    if ( ExtendBuffer[3] == 'M')
                      { if ((ExtendBuffer[4] == 'O') && 
                            (ExtendBuffer[5] == 'D') &&
                            (ExtendBuffer[6] == 'E'))
                          { 
                           if (ProcState == InitialAllZoneEmergencyState)
                             { return InitialAllZoneEmergencyState; } 
                          }
                      }
                  /* Extend unit receive
                     quit emergency public address package ok  */
                    else if (ExtendBuffer[3] == 'Q')
                      {  if ((ExtendBuffer[4] == 'U') && 
                             (ExtendBuffer[5] == 'I') &&
                             (ExtendBuffer[6] == 'T'))                        
                           {
                           if (ProcState == ExitAllZoneEmergencyState)
                             { return ExitAllZoneEmergencyState; } 
                           }
                      }
                    else
                      { /* no define */ }
                    }
                  break;
        case 'F': /* Fire alarm      */
                  /* Extend unit action fire alarm  */
                  if (ExtendBuffer[2] == 'A') 
                    {
                    if (ExtendBuffer[4] == 0)
                      {  /* Get fire alarm action extend unit ID */
                         /* Fire_alarm_ID = ExtendBuffer[3]      */
                         /* Get extend unit fire alarm state     */
                         /* Fire_alarm_state = ExtendBuffer[5]  
                            only use bit 1, 0  (Zone 10, 9)
                            Fire_alarm_state = ExtendBuffer[6]   
                            use bit 7...0 (ZONE8...ZONE1)        */
                        if ((ExtendBuffer[3] > 0) &&
   					         (ExtendBuffer[3] < (MaxExtendUnit+1)))
	    				        { 
	    				          PreID = ExtendBuffer[3];
//	    				          ExtendBuffer[5] &= 0x03;
	    				          FireZoneBuffer[PreID*2] |= ExtendBuffer[5];
	    				          FireZoneBuffer[(PreID*2)+1] |= ExtendBuffer[6];                            
                            FireZoneBuffer[((MaxExtendUnit+1)*2)+0] = PreID;
                            FireZoneBuffer[((MaxExtendUnit+1)*2)+1] = ExtendBuffer[5];
                            FireZoneBuffer[((MaxExtendUnit+1)*2)+2] = ExtendBuffer[6];
	    				          
                            for (ExtendBuffer[4] = 0; ExtendBuffer[4] < 150;)
                               { ExtendBuffer[4]++; }
                            ReplyFireAlram(ExtendBuffer[5],ExtendBuffer[6]);

                            if ((ExtendBuffer[5] != 0) || (ExtendBuffer[6] != 0))
                              {
                               return ExtendUnitFireActiveState;
                              }
                            else { return EOF; }
                          }
                        
                      }
                    }
                   /* Receive package                            */
                  else if (ExtendBuffer[2] == 'R') 
                    {
                  /* Extend unit receive 
                     action fire alarm package ok                */
                    if ((ExtendBuffer[3] == 0) &&
                        (ExtendBuffer[4] == 0))
                       {
                       /* compare state */
                       /* fire alarm    */
                       return FireAlarmActiveState;
                       }
                   
                    }
                  break;
        case 'G': /* Group           */
                  /* Group receive                          */
                  if (ExtendBuffer[2] == 'R')
                    {
                  /* Extend unit receive
                     group public address package ok         */
                    if ( ExtendBuffer[3] == 'M')
                      {  if ((ExtendBuffer[4] == 'O') &&
                             (ExtendBuffer[5] == 'D') &&
                             (ExtendBuffer[6] == 'E'))
                           {  
                            if ( ProcState == InitialGroupState )
                              { return InitialGroupState; }
                           }
                      }
                    else if (ExtendBuffer[3] == 'Q')
                      {  if ((ExtendBuffer[4] == 'U') &&
                             (ExtendBuffer[5] == 'I') &&
                             (ExtendBuffer[6] == 'T'))
                           {
                            if ( ProcState == ExitGroupState )
                              { return ExitGroupState; }
                           }                       
                      }
                    else if ((ExtendBuffer[3] == 'G') &&
                             (ExtendBuffer[4] == 'R'))
                      {
                         if (ProcState == InitialCallStationModeState)
                           { return InitialCallStationModeState; }
                      }
                  /* Extend unit receive
                     choice group number package ok          */ 
                    else if (ExtendBuffer[3] == 0)
                      {  
                         if ((ExtendBuffer[4] == 0) &&
                             (ExtendBuffer[5] == 0))
                           { 
                             if (ProcState == SetGroupState)
                               { return SetGroupState; }
                           }
                      }
                    else 
                         { /* no define */ }
                    }     
                    break;
        case 'H': /* Man handling    */
                  /* Receive package                         */
                  if (ExtendBuffer[2] == 'R')
                    {
                  /* Extend unit receive
                     man handling mode package               */
                    if (ExtendBuffer[3] == 'M')
                      {  if ((ExtendBuffer[4] == 'O') &&
                             (ExtendBuffer[5] == 'D') &&
                             (ExtendBuffer[6] == 'E'))
                           { }
                      }
                  /* Extend unit receive
                     quit man handling mode package ok       */
                    else if (ExtendBuffer[3] == 'Q')
                      {  if ((ExtendBuffer[4] == 'U') &&
                             (ExtendBuffer[5] == 'I') &&
                             (ExtendBuffer[6] == 'T'))
                           { }
                      }
                    else
                    { /* no define */  }
                    }
                  break;
        case 'I': /* None            */
                  break;
        case 'J': /* None            */
                  break;
        case 'K': /* Key             */
                  /* Action / Confirm & Autoalarm   */
                  if (ExtendBuffer[2] == 'A')
                    {
                    if (ExtendBuffer[4] == 0)
                      {
                      /* Set extend unit ID */
      			       if ((ExtendBuffer[3] > 0) &&
   					        (ExtendBuffer[3] < 16))
	    				      { PreID = ExtendBuffer[3]; }
					   
                      /* Extend unit set zone key   */
                      if (ExtendBuffer[6] == 0)
                        {
                         for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)
                            { ExtendBuffer[6]++; }
						       ReplyKeySet(ExtendBuffer[3],ExtendBuffer[5]);
					          ExtendBuffer[6] = 0;
                        }
                      /* Extend unit reset zone key */
                      else if ( ExtendBuffer[6] == 0xff)
                        {
                         for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)
                            { ExtendBuffer[6]++; }
   						    ReplyKeyReset(ExtendBuffer[3],ExtendBuffer[5]);
					          ExtendBuffer[6] = 0;
                        }
                      }
                    }
                  /* Receive package              */
                  else if (ExtendBuffer[2] == 'R')
                    {
                     if ((ExtendBuffer[3] == 0) && 
                         (ExtendBuffer[4] == 0))
                       {
                        if (ExtendBuffer[6] == 0)
                          {
                           if (ProcState == ResetFireRelationZoneState)
                             { return ResetFireRelationZoneState; }
						        else if (ProcState == ResetGroupZoneState)
                             { return ResetGroupZoneState; }
						        else if (ProcState == ResetBGMZoneState)
						           { return ResetBGMZoneState; }
						        else if (ProcState == ResetHumanHandlingZoneState)
						           { return ResetHumanHandlingZoneState; }
						        else if (ProcState == ResetGroupState)
						           { return ResetGroupState; }
                          }
                        if (ExtendBuffer[6] == 0xff)
                          {
                           if (ProcState == SetFireRelationZoneState)
                             { return SetFireRelationZoneState; }
                           else if (ProcState == SetGroupZoneState)
                             { return SetGroupZoneState; }
                           else if (ProcState == SetBGMZoneState)
     	                       { return SetBGMZoneState;  }
						         else if (ProcState == SetHumanHandlingZoneState)
						           { return SetHumanHandlingZoneState; }
						         else if (ProcState == SetGroupState)
						           { return SetGroupState; }
     	                    }
                        }
                    }
                  break;
        case 'L': /* Circuit control */
                  /* Action / Confirm & Autoalarm */
                  if (ExtendBuffer[2] == 'A')
                    {
                    }
                  else if (ExtendBuffer[2] == 'R')
                    {
                    }
                  break;
        case 'M': /* Setup           */
                  switch (ExtendBuffer[2]){
                  case  'C': /* Exit fire relation setup */
                    if ((ExtendBuffer[3] == 'Q') &&
                        (ExtendBuffer[4] == 'U') && 
                        (ExtendBuffer[5] == 'I') && 
                        (ExtendBuffer[6] == 'T'))
                      {
                       if (ProcState == ExitSetupFireRelationState)
                         {  return ExitSetupFireRelationState; }
                      }
                      break;
                  case  'F': /* Fire zone                    */
                    if ((ExtendBuffer[4] == 0) &&
                        (ExtendBuffer[6] == 0xff ))
                      { 
                       if ((ExtendBuffer[3] <= 0x0f) && (ExtendBuffer[5] <= 0x0A))
                         {
                          /* Fire relation setup state 
                             Extend unit assigned zone action */
                          if (ProcState == SetupFireRelationZoneState)
                            {
                              *(SetupBuffer)   = ExtendBuffer[3];
                              *(SetupBuffer+1) = ExtendBuffer[5] - 1;
                              for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)
                                 { ExtendBuffer[6]++; }
                              ReplyExtendZoneActive(ExtendBuffer[5]);
                              return SetupFireRelationZoneState;
                            } 
                            
                          /* Voice message setup state 
                             Extend unit assigned zone action */
                          if (ProcState == SetupVoiceZoneState)
                            {
                             *(SetupBuffer)   = ExtendBuffer[3];
                             *(SetupBuffer+1) = ExtendBuffer[5] - 1;
                             for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)
                                { ExtendBuffer[6]++; }                       
                            ReplyExtendZoneActive(ExtendBuffer[5]);
                            return SetupVoiceZoneState;
                            }
                            
                          /* Voice message test play state 
                             Extend unit assigned zone action */
                          if (ProcState == VoiceTestPlayState)
                            {
                             *(SetupBuffer)   = ExtendBuffer[3];
                             *(SetupBuffer+1) = ExtendBuffer[5] - 1;
                             for (ExtendBuffer[6] = 0; ExtendBuffer[6] < 150;)
                                { ExtendBuffer[6]++; }
                              ReplyExtendZoneActive(ExtendBuffer[5]);
                              return VoiceTestPlayState;
                            }
                         }
                      }
                        break;
                  case  'R': /* Receive package              */ 
                    switch (ExtendBuffer[3]) {
                    case   0: 
                      if ((ExtendBuffer[4] == 0) &&
                          (ExtendBuffer[6] == 0xff))
                        { return ProcState; }
                        break;
                    case 'M':
                      if ((ExtendBuffer[4] == 'O') &&
                          (ExtendBuffer[5] == 'D') &&
                          (ExtendBuffer[6] == 'E'))
                        { 
                          if (ProcState == InitialSetupFireRelationState)
                            { return InitialSetupFireRelationState; }
                            
                          if (ProcState == InitialSetupGroupState)
                            { return InitialSetupGroupState; }
                            
                          if (ProcState == InitialSetupBGMState)
                            { return InitialSetupBGMState; }
                            
                          if (ProcState == InitialSetupVoiceState)
                            { return  InitialSetupVoiceState; }                         

                          if (ProcState == InitialVoiceTestPlayState)
                            { return  InitialVoiceTestPlayState; }
                        }
                      break;
                    case 'O':
                      if ((ExtendBuffer[4] == 'K') &&
                          (ExtendBuffer[5] ==  0 ))
                        { if (ProcState == InitialState)
                            { 
                            if ( ExtendBuffer[6] <= 0x0f ) 
                              { 
                                PreID = ExtendBuffer[6]; 
                                return InitialState; 
                              } 
                            }
                        }
                        break;                      
                    case 'Q':
                      if ((ExtendBuffer[4] == 'U') &&
                          (ExtendBuffer[5] == 'I') &&
                          (ExtendBuffer[6] == 'T'))
                        { }
                        break;
                    case 'S': /* Save setup value */
                      if ((ExtendBuffer[4] == 'A') &&
                          (ExtendBuffer[5] == 'V') &&
                          (ExtendBuffer[6] == 'E'))
                        {
                          if (ProcState == InitialSetupFireRelationState)
                            { return InitialSetupFireRelationState; }
                            
                          if (ProcState == InitialSetupGroupState)
                            { return InitialSetupGroupState; }
                            
                          if (ProcState == InitialSetupBGMState)
                            { return InitialSetupBGMState; }
                            
                          if (ProcState == InitialSetupVoiceState)
                            { return  InitialSetupVoiceState; }                         
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
                  if (ExtendBuffer[2] == 'A')
                    {  if (ExtendBuffer[4] == 0)
                         { 
                          if ((ExtendBuffer[3] > 0) &&
   					            (ExtendBuffer[3] < 16))
                            { PreID = ExtendBuffer[3]; }
                          for (ExtendBuffer[4] = 0; ExtendBuffer[4] < 150;)
                            { ExtendBuffer[4]++; }
                          ReplyShort(ExtendBuffer[5], ExtendBuffer[6]);
                         }
                    }
                  /* Receive package              */
                  else if (ExtendBuffer[2] == 'R')
                    {  if ((ExtendBuffer[3] == 0) &&
                           (ExtendBuffer[4] == 0))
                         {
                          return  LoopShortActiveState;
                         }
                    }
                  break;
        case 'T': /* Emergency telephone */
                  /* Action / Confirm & Autoalarm */
                  if (ExtendBuffer[2] == 'A')
                    {  if (ExtendBuffer[4] == 0)
                         {   
                          if ((ExtendBuffer[3] > 0) &&
   					            (ExtendBuffer[3] < 16))
                            { PreID = ExtendBuffer[3];

                             PreID = ExtendBuffer[3];
                             FireZoneBuffer[PreID*2] |= ExtendBuffer[5];
                             FireZoneBuffer[(PreID*2)+1] |= ExtendBuffer[6];                            
                             FireZoneBuffer[((MaxExtendUnit+1)*2)+0] = PreID;
                             FireZoneBuffer[((MaxExtendUnit+1)*2)+1] = ExtendBuffer[5];
                             FireZoneBuffer[((MaxExtendUnit+1)*2)+2] = ExtendBuffer[6];
                              for (ExtendBuffer[4] = 0; ExtendBuffer[4] < 150;)
                                 { ExtendBuffer[4]++; }
                              ReplyEmergencyTel(ExtendBuffer[5], ExtendBuffer[6]);
                              return EmergencyTelState;
                            }
                         }
                    }
                  /* Receive package              */
                  else if (ExtendBuffer[2] == 'R')
                    {  if (ExtendBuffer[3] == 0)
                         {
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
  return FLASE;
}

BOOLEAN IsReceiveCallStation(void)
{
  if (CallStationPort_Receive_Length() > 0 ) { return TRUE; }
  if (StartCount > 0) { return TRUE; }
  return FLASE;
}

void InitialCommunication(void)
{
 PreID = 0;
 PreCallStationID = 0;
 StartCount = 0;
 PackageReceive = FLASE;
 CallStationReceive = FLASE;
}






















































































































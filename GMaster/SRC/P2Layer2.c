// 
// File Name   : P2Layer2.C
// Auther      : K'un I Chen
// Create      : 07/21/2003
// version     :
// Objective   :
// Description : P2 Port 負責遠端操作面盤的通訊
// History     :

// L2 : SyncState, Len, to Address, Data, Chksum
// Chksum = Len
// current only address to 0x80
#define _P2LAYER2_IMPLEMENT_
#include "stimer.h"
#include "P2layer2.h"

// object implement in GMMAIN.C file
extern SoftTimer systimer;

// Gobal Variable
bit P2PackageReceive = false;
bit P2SyncState = false;
idata BYTE P2RxBuff[P2RxBuffLength];
P2_SEND_STRUCT p2cnt;

void P2InitSend(BYTE SendCount) {
  p2cnt.ReCounter = SendCount;
  p2cnt.Len       = 0;
  p2cnt.Chksum    = 0;
  p2cnt.TimeOut = 0;
}

void P2PutPacket(BYTE d) {
  p2cnt.Buff[p2cnt.Len++] = d;
  p2cnt.Chksum += d;
}

void P2Send(void) {
  BYTE iCount;

  P2Port_Put(P2Package_SYNC);
 
  iCount = 0;

  do {
    P2Port_Put(p2cnt.Buff[iCount++]);
  } while(iCount < p2cnt.Len);

  P2Port_Put(p2cnt.Chksum);

  if (p2cnt.ReCounter != 0) {
    p2cnt.ReCounter--;
    p2cnt.TimeOut = systimer.m10sec + P2_TIMEOUT_SPACE;
    if (p2cnt.TimeOut > 99) {
      p2cnt.TimeOut -= 99;
    }
  } 
}

void RxP2(void) {
 static idata WORD wait_timer;
 static idata BYTE ReceiveLength;
 static idata BYTE PackageCheckSum;
 static idata BYTE BuffCount;
 static bit LengthState    = FALSE;
 static bit LengthOK       = FALSE;
 BYTE Length;
 BYTE ReceiveTemp;
 
 /* ============================================================= */ 
 Length = P2Port_Receive_Length();
 while (Length) {
   if (P2Port_Get(&ReceiveTemp)) { /* Is get byte success  */
     wait_timer = GetSystemTimer();
     if (P2SyncState) {              /* SyncState is true    */
       if (LengthState) {            /* LengthState is true  */
         PackageCheckSum += ReceiveTemp;
         ReceiveLength--;
         if (LengthOK) {             /* LengthOK is true     */
            if ( ReceiveLength == 0 ) { 
           	  P2SyncState = FALSE;
              LengthOK    = FALSE;
              LengthState = FALSE;
              if (PackageCheckSum == 0) { 
                P2PackageReceive = TRUE; 
              }
            } else { 
              P2RxBuff[BuffCount++] = ReceiveTemp; 
            }
            break; 
         } else { 
           if (ReceiveLength == 0 ) { 
             break; 
           } 
         }  
       } else { /* LengthState is flase */
         LengthState = TRUE;
         ReceiveLength = ReceiveTemp;
         LengthOK = true; 
         PackageCheckSum = ReceiveLength; 
         ReceiveLength--;
         BuffCount = 0;
       }    
     } else { /* SyncState is flase   */
       if (P2Package_SYNC == ReceiveTemp) { 
      	 P2SyncState = true; 
       }
     } // terminal of else // SyscState is false
   } // terminal of if (P2Port_Get(&ReceiveTemp))
  Length--;  
 } // while end
 
 /* one package be byte to byte timer > 3 ms then package fail */
 if (P2SyncState) {
   if ((GetSystemTimer() - wait_timer) > 3) { /*  maybe operation error (carray)*/
     P2SyncState = false; 
     LengthOK    = false;
     LengthState = false;
   }
 }
}



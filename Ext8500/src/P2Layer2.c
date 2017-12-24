// 
// File Name   : P2Layer2.C
// Auther      : K'un I Chen
// Create      : 07/21/2003
// version     :
// Objective   :
// Description : P2 Port 負責遠端延伸操作面盤的通訊
// History     :

// L2 : SyncState, Len, to Address, Data, Chksum
// Chksum = Len
// current only address to 0x80
#define P2LAYER2_IMPLEMENT
#include "P2layer2.h"
bit P2PackageReceive = FALSE;
bit P2SyncState = FALSE;

BYTE idata P2RxBuff[P2RxBuffLength];

void RxP2(void) {
  static idata WORD wait_timer;
  static idata BYTE ReceiveLength;
  static idata BYTE PackageCheckSum;
  static idata BYTE BuffCount;
  static bit LengthState    = FALSE;
  static bit LengthOK       = FALSE;
  BYTE Length;
  BYTE ReceiveTemp;
 
  RESET_REGISTER(EA);                         /* Enable Interrupt                        */
  Length = P2Port_Receive_Length();
  SET_REGISTER(EA);                           /* Enable Interrupt                        */

  while (Length) {
    if (P2Port_Get(&ReceiveTemp))  {
      wait_timer = GetSystemTimer();
      if (P2SyncState) {
        if (LengthState) {
          PackageCheckSum += ReceiveTemp;
          ReceiveLength--;
          if (LengthOK) {
            if ( ReceiveLength == 0 ) {
              P2SyncState = FALSE;
              LengthOK    = FALSE;
              LengthState = FALSE;
              if (PackageCheckSum == 0) { 
                P2PackageReceive = TRUE; 
              }
              break;
            } else { 
              if (BuffCount < P2RxBuffLength) {
                P2RxBuff[BuffCount++] = ReceiveTemp; 
              } else {
                P2SyncState = FALSE;
                LengthOK    = FALSE;
                LengthState = FALSE;
              }              
            }
          } else { 
            if (ReceiveLength == 0 ) { 
              break; 
            } 
          }   
        } else {
           LengthState = TRUE;
           ReceiveLength = ReceiveTemp;
           LengthOK = TRUE; 
           PackageCheckSum = ReceiveLength; 
           ReceiveLength--;
           BuffCount = 0;
        }    
      } else {
        if (P2Package_SYNC == ReceiveTemp) { 
          P2SyncState = TRUE; 
        }
      }
    }
    Length--;  
  }
 
  // one package be byte to byte timer > 3 ms then package fail
  if (P2SyncState) {
    if ((GetSystemTimer() - wait_timer) > 3) { //  maybe operation error (carray)
      P2SyncState = FALSE; 
      LengthOK    = FALSE;
      LengthState = FALSE;
    }
  }
}

BOOLEAN IsP2Receive(void) {
  if (P2Port_Receive_Length() > 0 ) { return TRUE; }
  if (P2SyncState) { return TRUE; }
  return FALSE;
}
/*
void TxP2(char* packet, BYTE count)
{
}
*/


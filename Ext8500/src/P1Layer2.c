// 
// File Name   : P1Layer2.C
// Auther      : K'un I Chen
// Create      : 07/21/2003
// version     :
// Objective   :
// Description : P1 Port 負責與 GMASTER 的溝通與連絡
// History     :
#define P1LAYER2_IMPLEMENT
#include "P1layer2.h"

// test pattern
// Sync, Len,  ID,   Type,   ID,   status,  status,   Checksum
// 0x15, 0x07, 0xff, 0x01, 0x00,     0x55,    0xAA,    0xFA
// 0x15, 0x07, 0xff, 0x02, 0x00,     0x55,    0xAA,    0xF9

bit P1PackageReceive = FALSE;
bit P1SyncState = FALSE;

idata BYTE P1RxBuff[P1RxBuffLength];

void RxP1(void) {
  static idata WORD wait_timer;
  static idata BYTE ReceiveLength;
  static idata BYTE PackageCheckSum;
  static idata BYTE BuffCount;
  static bit LengthState    = FALSE;
  static bit LengthOK       = FALSE;
  BYTE Length;
  BYTE ReceiveTemp;
 
 /* ============================================================= */ 
  RESET_REGISTER(EA);                       /* Enable Interrupt                        */
  Length = P1Port_Receive_Length();
  SET_REGISTER(EA);                         /* Enable Interrupt                        */
  while (Length) {
    if (P1Port_Get(&ReceiveTemp))           /* Is get byte success  */ {
      wait_timer = GetSystemTimer();
      if (P1SyncState) {                    /* SyncState is true    */
        if (LengthState) {                  /* LengthState is true  */
          PackageCheckSum += ReceiveTemp;
          ReceiveLength--;
          if (LengthOK) {                   /* LengthOK is true     */
            if ( ReceiveLength == 0 ) {
              P1SyncState = FALSE;
              LengthOK    = FALSE;
              LengthState = FALSE;
              if (PackageCheckSum == 0) { 
                // complete a package
                P1PackageReceive = TRUE; 
              } 
              break;
            } else { 
              if (BuffCount < P1RxBuffLength) {
                P1RxBuff[BuffCount++] = ReceiveTemp; 
              } else {
             	P1SyncState = FALSE;
                LengthOK    = FALSE;
                LengthState = FALSE;
              }
            }
             
          } else { 
            if (ReceiveLength == 0 ) { 
              break; 
            } 
          }  
        } else { // LengthState is flase
          LengthState = TRUE;
          ReceiveLength = ReceiveTemp;
          if ( P1Package_Length == ReceiveLength ) {// Package length is  right
          	LengthOK = TRUE; 
          	PackageCheckSum = ReceiveLength; 
          }
          ReceiveLength--;  
          BuffCount = 0;
        }    
      } else { // SyncState is flase
        if (P1Package_SYNC == ReceiveTemp) {
          P1SyncState = TRUE; 
        }
      }
    }
    Length--;  
  }
     /* one package be byte to byte timer > 3 ms then package fail */
     if (P1SyncState)
     {
        if ((GetSystemTimer() - wait_timer) > 3) /*  maybe operation error (carray)*/
          { 
           P1SyncState = FALSE; 
           LengthOK    = FALSE;
           LengthState = FALSE;
          }
     }
}

BOOLEAN IsReceive(void) {
  if (P1Port_Receive_Length() > 0 ) { return TRUE; }
  if (P1SyncState) { return TRUE; }
  return FALSE;
}
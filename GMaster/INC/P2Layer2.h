#ifndef _P2LAYER2_H_
#define _P2LAYER2_H_
// File Name   : P1Layer2.h
// Auther      : K'un I Chen
// Create      : 07/21/2003
// version     :
// Objective   :
// Description : P2 Port 負責擴展系統的溝通與連絡
// History     :
#include "common.h"
#include "uart.h"
#include "ptimer.h"

#define P2Port_Put(x)                      UART1_Put(x)
#define P2Port_Get(x)                      UART1_Get(x)
#define P2Port_Receive_Length()            UART1_R_Length
#define P2Port_Transmit_Length()           UART1_T_Length

#define P2Package_SYNC                   0x15
#define P2RxBuffLength                      7
//@@@@@
#define P2Package_Length                    7

#define P2_IsReceive()  P2SyncState

void RxP2(void);


#define P2_RESEND_COUNT     3
#define P2_TIMEOUT_SPACE   80
#define P2_REG_LEN          8

typedef struct {  
  BYTE TimeOut;
  BYTE ReCounter;
  BYTE Len;
  BYTE Buff[P2_REG_LEN+1];
  BYTE Chksum;
} P2_SEND_STRUCT;

void P2InitSend(BYTE SendCount);
void P2PutPacket(BYTE d);
void P2Send(void);

// P2 Layer Export Variable
#ifndef _P2LAYER2_IMPLEMENT_
  extern bit P2PackageReceive; // bit of complete package indacate
  extern bit P2SyncState;
  extern idata BYTE P2RxBuff[P2RxBuffLength];
  extern P2_SEND_STRUCT p2cnt;
#endif

#endif
#ifndef _P1LAYER3_H_
#define _P1LAYER3_H_
// File Name   : P1Layer3.h
// Auther      : K'un I Chen
// Create      : 07/26/2003
// version     :
// Objective   :
// Description : 負責原生系統的溝通與連絡
// History     :

#include "P1Layer2.h"
#include "gfifo.h"

ExportQueFunction(P1FIFOIn)
ExportQueFunction(P1FIFOOut)
ExportQueFunction(P2FIFOIn)
ExportQueFunction(P2FIFOOut)

#define P1Sync 0x15

void P1Process(void);
void TranslateP1(register BYTE Item);
void P1TxCMD(BYTE CMD, BYTE ID, BYTE Value1, BYTE Value2);
void P1QueryState(BYTE QC);
void ResponseP1ACK(BYTE ID,BYTE ACKCMD);
void P1Run(void);
void P1Stop(void);

#define P1_REG_LEN  8                          
                         
//void P1InitSend(void);
void P1PutPacket(BYTE Val);
void P1FinalPacket(void);
void P1Send(void);

#define P1_RESEND_COUNT     3
#define P1_TIMEOUT_SPACE   80

//BOOLEAN IsCompleteP1Tx(void);
#define IsCompleteP1Tx()  ((p1cnt.ReCounter != 0) ? FALSE : TRUE)
typedef struct {  
  BYTE TimeOut;
  BYTE ReCounter;
  BYTE Len;
  BYTE Buff[P1_REG_LEN+1];
  BYTE Chksum;
} P1_SEND_STRUCT;
           
#ifndef IMPLEMENT_P1L3_
extern P1_SEND_STRUCT p1cnt;
#endif

void P1InitSend(BYTE SendCount);
void P1PutPacket(BYTE date);
void P1Send(void);
#define P1ReSend() {                                                        \
  if ((p1cnt.ReCounter != 0) && (0 == (p1cnt.TimeOut - systimer.m10sec))) { \
    P1Send();                                                               \
  }                                                                         \
}



#define P1_KEY_CMD        0x00
#define P1_MIC_PRESS_CMD  0x01
#define P1_GROUP_CMD      0x02
#define P1_EXIT_GROUP_CMD 0x03


#define P1_EXCMD    0xCF
#define P1_EXSTOP   0x81
#define P1_EXSTART  0x82
#define P1_EXKEY    0x83
#define P1_EXQUERY  0x84

#define SPC_COMMAND        (0xAA)
#define SPC_NORMAL          (255)
#define SPC_ENTRY_FIRE      (254)
#define SPC_CONFIRM_FIRE    (253)
#define SPC_NOT_FIRE        (252)

#endif
// end of _P1LAYER3_H_

#ifndef _P2LAYER3_H_
#define _P2LAYER3_H_
// File Name   : P2Layer3.h
// Auther      : K'un I Chen
// Create      : 07/26/2003
// version     :
// Objective   :
// Description : 負責遠端系統的溝通與連絡
// History     :

#include "P2Layer2.h"
#include "gfifo.h"
typedef enum { P2_WAIT = 1, 
               P2_RNACK, 
               P2_RACK,
               P2_KEY,
               P2_RESEND
             } P2L3_STATUS;

#ifndef P2LAYER3_IMPLEMENT
extern data P2L3_STATUS  P2Status;
#endif

#define P2ACK_Len     5
#define P2CMD_LEN     6
#define P2QueryID_LEN 4

#define P2CMD_RMKEY    0xAA
#define P2CMD_ACK      0x55

void P2Process(void);
void P2ResponseACK(BYTE ID,BYTE CMD);
void P2TxCMD(BYTE ID, BYTE CMD, BYTE S1, BYTE S2);


#define P2_RMEU_KEY    0x01

#endif
// end of _P2LAYER3_H_

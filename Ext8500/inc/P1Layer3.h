#ifndef _P1LAYER3_H_
#define _P1LAYER3_H_
// File Name   : P1Layer3.h
// Auther      : K'un I Chen
// Create      : 07/26/2003
// version     :
// Objective   :
// Description : 負責系統的溝通與連絡
// History     :

#include "P1Layer2.h"
#include "gfifo.h"
#define ACK_Len 6

void P1Process(void);
void P1TxCMD(BYTE CMD, BYTE Value, BYTE Value1);
void TranslateP1(register BYTE Item);
void P1ResponseACK(BYTE ID, BYTE Cmd);

#define RM_KEY              0x01
#define RM_MIC_PRESS        0x03
#define RM_GROUP            0x04
#define RM_ACK              0xF0

#endif
// end of _P1LAYER3_H_

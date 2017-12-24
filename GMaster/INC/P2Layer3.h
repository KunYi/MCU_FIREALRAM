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
extern data BYTE P2SendCMD;
#endif

#define ACK_Len 4
#define P2CMD_LEN  7
#define P2QueryID_LEN 4
#define CMD_QUERY_ID  0xF0


ExportQueFunction(P2FIFOIn)
ExportQueFunction(P2FIFOOut)
ExportQueFunction(P1FIFOOut)
ExportQueFunction(P1FIFOIn)

void P2Process(void);
void TranslateP2(register BYTE Item);
void P2ResponseACK(BYTE ID);

void P2TxCMD(BYTE bAckID, BYTE ID, BYTE CMD, BYTE S1, BYTE S2);
void P2QueryID(BYTE ID);
void P2Stop(void);
void P2Run(void);


#define P2_KEY              0x01
#define P2_MIC_PRESS        0x03
#define P2_GROUP            0x04
#define P2_ACK              0xF0

#define SEND_GREEN          0x40
#define SEND_GROUP          0x41
#define SEND_LCD            0x09

#define P2_EXCMD    0xCF
#define P2_EXSTOP   0x81
#define P2_EXSTART  0x82
#define P2_EXKEY    0x83
#define P2_EXQUERY  0x84

#define CMD_SYS_START       0x10
#define CMD_UNIT_RESET      (CMD_SYS_START +  0)
#define CMD_ENTRY_GROUP     (CMD_SYS_START +  1)
#define CMD_ACTIVE_GROUP    (CMD_SYS_START +  2)
#define CMD_QUIT_GROUP      (CMD_SYS_START +  3)
#define CMD_ENTRY_MANU      (CMD_SYS_START +  4)
#define CMD_QUIT_MANU       (CMD_SYS_START +  5)
#define CMD_ENTRY_BGM       (CMD_SYS_START +  6)
#define CMD_QUIT_BGM        (CMD_SYS_START +  7)
#define CMD_ENTRY_PREALARM  (CMD_SYS_START +  8)
#define CMD_FIRE            (CMD_SYS_START +  9)
#define CMD_CANCEL_ALRAM    (CMD_SYS_START + 10)
#define CMD_EMG_ALL         (CMD_SYS_START + 11)
#define CMD_SHORT           (CMD_SYS_START + 12)
#define CMD_EMG_TEL         (CMD_SYS_START + 13)
#define CMD_KEY             (CMD_SYS_START + 14)
#define CMD_PTT_PROC        (CMD_SYS_START + 15)
#define CMD_ALLCALL_MANU    (CMD_SYS_START + 16)
#define CMD_ALL_EMG         (CMD_SYS_START + 17)
#define CMD_QUIT_EMG        (CMD_SYS_START + 18)
#define CMD_EXTEND_GROUP    (CMD_SYS_START + 19)
#define CMD_ENTRY_NORMAL    (CMD_SYS_START + 20)
#define CMD_CONFIRM_FIRE    (CMD_SYS_START + 21)
#define CMD_NOT_FIRE        (CMD_SYS_START + 22)
#define CMD_SHORT_STATE     (CMD_SYS_START + 23)
#define CMD_RLED            (CMD_SYS_START + 24)
#define CMD_GLED            (CMD_SYS_START + 25)
#define CMD_CLED            (CMD_SYS_START + 26)
#define CMD_GPLED           (CMD_SYS_START + 27)
#define CMD_FRLED           (CMD_SYS_START + 28)
#define CMD_FGLED           (CMD_SYS_START + 29)
#define CMD_FCLED           (CMD_SYS_START + 30)
#define CMD_FGPLED          (CMD_SYS_START + 31)
#define CMD_LCD             (CMD_SYS_START + 32)
#define CMD_EU_FIRE         (CMD_SYS_START + 33)
#endif
// end of _P2LAYER3_H_

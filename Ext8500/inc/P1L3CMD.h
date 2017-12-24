#ifndef _P1L3CMD_H_
#define _P1L3CMD_H_

#define RLED_STATUS           1
#define GLED_STATUS           2
#define CNTLED_STATUS         3
#define GRPLED_STATUS         4
#define FRLED_STATUS          5
#define FGLED_STATUS          6
#define FCNTLED_STATUS        7
#define FGRPLED_STATUS        8
#define MAINLCD_STATUS        9
#define RESET_CMD            10

#define CMD_P1L3_START      0x10
#define CMD_UNIT_RESET      (CMD_P1L3_START +  0)
#define CMD_ENTRY_GROUP     (CMD_P1L3_START +  1)
#define CMD_ACTIVE_GROUP    (CMD_P1L3_START +  2)
#define CMD_QUIT_GROUP      (CMD_P1L3_START +  3)
#define CMD_ENTRY_MANU      (CMD_P1L3_START +  4)
#define CMD_QUIT_MANU       (CMD_P1L3_START +  5)
#define CMD_ENTRY_BGM       (CMD_P1L3_START +  6)
#define CMD_QUIT_BGM        (CMD_P1L3_START +  7)
#define CMD_ENTRY_PREALARM  (CMD_P1L3_START +  8) // 0x18
#define CMD_FIRE            (CMD_P1L3_START +  9)
#define CMD_CANCEL_ALRAM    (CMD_P1L3_START + 10)
#define CMD_EMG_ALL         (CMD_P1L3_START + 11)
#define CMD_SHORT           (CMD_P1L3_START + 12)
#define CMD_EMG_TEL         (CMD_P1L3_START + 13)
#define CMD_KEY             (CMD_P1L3_START + 14)
#define CMD_PTT_PROC        (CMD_P1L3_START + 15)
#define CMD_ALLCALL_MANU    (CMD_P1L3_START + 16) // 0x20
#define CMD_ALL_EMG         (CMD_P1L3_START + 17) // 0x21
#define CMD_QUIT_EMG        (CMD_P1L3_START + 18) // 0x22
#define CMD_EXTEND_GROUP    (CMD_P1L3_START + 19) // 0x23
#define CMD_ENTRY_NORMAL    (CMD_P1L3_START + 20) // 0x24
#define CMD_CONFIRM_FIRE    (CMD_P1L3_START + 21) // 0x25
#define CMD_NOT_FIRE        (CMD_P1L3_START + 22) // 0x26
#define CMD_SHORT_STATE     (CMD_P1L3_START + 23) // 0x27
#define CMD_RLED            (CMD_P1L3_START + 24) 
#define CMD_GLED            (CMD_P1L3_START + 25)
#define CMD_CLED            (CMD_P1L3_START + 26) // 0x2A
#define CMD_GPLED           (CMD_P1L3_START + 27) 
#define CMD_FRLED           (CMD_P1L3_START + 28)
#define CMD_FGLED           (CMD_P1L3_START + 29)
#define CMD_FCLED           (CMD_P1L3_START + 30)
#define CMD_FGPLED          (CMD_P1L3_START + 31) // 0x2F
#define CMD_LCD             (CMD_P1L3_START + 32) // 0x30
#define CMD_EU_FIRE         (CMD_P1L3_START + 33) // 0x31

#define QUERY_ID_STATUS    0xF0

#define P1_BASE_COMMAND      250 // 0xFA
#define P1_SELF_RLED         (P1_BASE_COMMAND -  0)  // 0xFA,250
#define P1_SELF_GLED         (P1_BASE_COMMAND -  1)  
#define P1_SELF_CLED         (P1_BASE_COMMAND -  2)
#define P1_SELF_GPLED        (P1_BASE_COMMAND -  3)  // 0xF7,247
#define P1_EXUNIT_RLED       (P1_BASE_COMMAND -  4)
#define P1_EXUNIT_GLED       (P1_BASE_COMMAND -  5)  // 0xF5,245
#define P1_EXUNIT_CLED       (P1_BASE_COMMAND -  6)
#define P1_EXUNIT_GPLED      (P1_BASE_COMMAND -  7)  // 0xF3,243
#define P1_SELF_FRLED        (P1_BASE_COMMAND -  8)
#define P1_SELF_FGLED        (P1_BASE_COMMAND -  9)
#define P1_SELF_FCLED        (P1_BASE_COMMAND - 10)  // 0xF0,240
#define P1_SELF_FGPLED       (P1_BASE_COMMAND - 11)
#define P1_EXUNIT_FRLED      (P1_BASE_COMMAND - 12)  
#define P1_EXUNIT_FGLED      (P1_BASE_COMMAND - 13)
#define P1_EXUNIT_FCLED      (P1_BASE_COMMAND - 14)
#define P1_EXUNIT_FGPLED     (P1_BASE_COMMAND - 15)
#define P1_LCD_STATE         (P1_BASE_COMMAND - 16)  // 0xEA, 234


#endif
// End P1L3CMD.H

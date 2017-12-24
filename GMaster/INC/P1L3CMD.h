#ifndef _P1L3CMD_H_
#define _P1L3CMD_H_

#define SYSTEM_RESET          1
#define KEY_PROC              2
#define GROUP_PROC            3
#define EMG_PROC              4
#define SHORT_PROC            5
#define XFUNC_PROC            6
#define DISPLAY_PROC          7
#define EMGTEL_PROC           8
#define FIRE_PROC             9
#define SETUP_PROC           10
#define EXIST_PROC           11
#define BGM_PROC             12
#define QUITALARM_PROC       13
#define MANU_PROC            14
#define PTT_PROC             15
#define SPC_PROC             16


// GROUP_PROC Sub Item
#define GROUP_SUB_QUIT        253
#define GROUP_SUB_ENTRY       252
#define GROUP_SUB_ACTION1     251
#define GROUP_SUB_ACTION2     250
#define GROUP_SUB_EXGROUP     249
// EMG_PROC Sub Item
#define EMG_SUB_ENTRY         0xE0
#define EMG_SUB_QUIT          0xDF
//XFUNC_PROC Sub Item
// Don't change below define
// Because is depend
#define XFUNC_SUB_ACCEPTKEY   0
#define XFUNC_SUB_MICSTATUS   1
#define XFUNC_SUB_XGROUP      2
#define XFUNC_SUB_DGROUP      3
// SETUP_PROC Sub Item
#define SETUP_SUB_EBGM        0xA0
#define SETUP_SUB_EFIRE       0x9F
#define SETUP_SUB_EGROUP      0x9E
#define SETUP_SUB_EVOICE      0x9D
#define SETUP_SUB_ESETUP      0x9C
#define SETUP_SUB_ETRYPLAY    0x9B
#define SETUP_SUB_QBGM        0x9A
#define SETUP_SUB_QFIRE       0x99
#define SETUP_SUB_QGROUP      0x98
#define SETUP_SUB_QVOICE      0x97
#define SETUP_SUB_QSETUP      0x96
#define SETUP_SUB_QTRYPLAY    0x95
#define SETUP_SUB_ZONE        0x94
// BGM_PROC Sub Item
#define BGM_SUB_ENTRY         0x80
#define BGM_SUB_QUIT          0x79
// MANU_PROC Sub Item
#define MANU_SUB_ENTRY        0x70
#define MANU_SUB_QUIT         0x6F
#define MANU_SUB_ALLCALL      0x6E

#define SC_SUB_NORMAL         0x67
#define SC_SUB_ENTRY_FIRE     0x66
#define SC_SUB_CONFIRM_FIRE   0x65
#define SC_SUB_NOT_FIRE       0x64

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
#define QUERY_ID_STATUS    0xF0
#endif

// End P1L3CMD.H

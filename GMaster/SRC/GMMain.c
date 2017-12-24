// 
// File Name   : GMMain.C
// Auther      : K'un I Chen
// Create      : 07/20/2003
// version     :
// Objective   :
// Description :
// History     :

// #define DEBUG_RECEIVE_PORT1
// #define DEBUG_RECEIVE_PORT2
#ifdef DEBUG_RECEIVE_PORT1
  void DEBUG_PORT1(void);
#endif 

#ifdef DEBUG_RECEIVE_PORT2
  void DEBUG_PORT2(void);
#endif 

/*
#define CompleteP1Tx()  __P1RSCounter = 0; \
                        Type = 0x55; \
                        status1 = 0; \
                        status2 = 0
  */                      

#define CompleteP1Tx()  \
                        Type = 0x55; \
                        status1 = 0; \
                        status2 = 0

#define  PORT_A  DIRECT_BYTE_IO(0x2000)
#define  PORT_B  DIRECT_BYTE_IO(0x2200)
#define  PORT_C  DIRECT_BYTE_IO(0x2400)

#define  SET_PORTA(x)  (PORT_A = (x));
#define  SET_PORTB(x)  (PORT_B = (x));
#define  SET_PORTC(x)  (PORT_C = (x));

#define  PRESS_REMOTE_MIC      0x01
#define  RELEASE_REMOTE_MIC    0x00
#define  ENTRY_EXGROUP         0x02
#define  EXIT_EXGROUP          0x00



#include <regw77e58.h>
#include "ptimer.h"
#include "stimer.h"
#include "uart.h"
#include "P1Layer3.h"
#include "P2Layer3.h"
#include "L1.h"
#include "GMMain.h"
#include "Gfifo.h"
#include "P1L3CMD.h"
#include "LCDDef.h"
#include "QUERY.h"


#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define P2TimeoutSpare   30

data SoftTimer softtimer1;
data SoftTimer systimer;
data BYTE m10Tick;

typedef enum { SYS_INIT = 0, SYS_NORMAL, SYS_GROUP, SYS_MANULLY, SYS_BGM } PROCSTATE;
typedef struct {
          PROCSTATE Status;  
        } MAIN_UNIT;
        
enum { SYS_IDLE = 0, P_MASTER, R1_MASTER, R2_MASTER, R3_MASTER, R4_MASTER } MASTER_RING;
data BYTE MasterRing;

enum { STAT_INIT = 0, STAT_IDLE, STAT_QP1, STAT_QR1, STAT_QR2, STAT_QR3, STAT_QR4 } SYS_STATUS;
data BYTE MainStatus;


// Create FIFO & Operator function
CreateQue(P1FIFOIn)
CreateQue(P1FIFOOut)
ImplementQue(P1FIFOIn)
ImplementQue(P1FIFOOut)


CreateQue(P2FIFOIn)
CreateQue(P2FIFOOut)
ImplementQue(P2FIFOIn)
ImplementQue(P2FIFOOut)

#define REDLED          0
#define GREENLED        1
#define CONTROLLED      2
#define MIXERLED        3
#define GROUPLED        4

#define EREDLED         0
#define EGREENLED       1
#define EMIXERLED       2

data MAIN_UNIT MainUnit;

CODE_DATA const BYTE CONVBIT[] = { BIT0, BIT1, BIT2, BIT3, BIT4, BIT5, BIT6, BIT7 };
#define LEDTable CONVBIT

data BYTE M_VRAM[5];         // main 
data BYTE M_FlashRAM[5];     // main flash

pdata BYTE E_VRAM[16][3];     // extend
pdata BYTE E_FlashRAM[16][3]; // extend flash

data BYTE Item;
data BYTE ReceID;
data BYTE Type;
data BYTE status1;
data BYTE status2;

data BYTE P2PrevTimer;
data BYTE P2TxCount;
data BYTE P2ACK_ID;
data BYTE AckID;
data BYTE Count;
data BYTE P2AckCMD;
data BYTE P2AckID;

data BYTE P2CMD;
data BYTE P2SendID;
data BYTE P2Value1;
data BYTE P2Value2;
bit bInitial;
bit bSetPORTA;
bit bCompleteP1Tx;
bit bOverTimeQuery;

// bit bWaitP2Ack;

sbit WatchDog = P1 ^ 5;

void main(void) {

 #include "Initial.C"

#ifdef DEBUG_RECEIVE_PORT1
  DEBUG_PORT1();
#endif

#ifdef DEBUG_RECEIVE_PORT2
  DEBUG_PORT2();
#endif 


 while(true) { // main loop
   // driver layer   
   WatchDog = 0;
   // L1 System timer process routine
   SystemTimerProc();
   // #include "SysTimerProc.c"
   WatchDog = 1;
  
   // process of layer 1
   // if not receiver then transmitter
   #ifdef _DRIVER_FAST_
     #include "Driver\UartDriver.c"
   #else
     UART0Drive(); // if not receive then transmiter
     UART1Drive(); // if not receive then transmiter
   #endif

   // process of layer 2
   #ifdef _DRIVER_FAST_
     #include "Driver\P1L2.c"
     #include "Driver\P2L2.c"
   #else
     RxP1();
     RxP2();
   #endif
   P1ReSend();
   // process of layer 3
   if (P1PackageReceive == true) {
     P1PackageReceive = false;
     P1Process();
   }
     
   if (P2PackageReceive == true) {
     P2PackageReceive = false;
     P2Process();
   }

   if (bInitial == TRUE) {       
     if (Count < 5) {
       if ( P2PrevTimer == systimer.m10sec) {
         P2QueryID(Count++);
               
         P2PrevTimer = systimer.m10sec + P2TimeoutSpare; // add Timer
         if (P2PrevTimer > 99) {
           P2PrevTimer -= 99;
         }
       }
     } else  { 
       bInitial = FALSE;
     }
   } 
   
   // Application
   #include ".\P1APP\P1Application.C"
   #include "Polling.C"
   
   if (TRUE == IsCompleteP1Tx()) {
     if (GetFromQue(P2FIFOIn, &Item)) {
       switch(Item) {
       case SEND_KEY_CMD: // send a Key
         GetFromQue(P2FIFOIn, &P2SendID);
         GetFromQue(P2FIFOIn, &P2Value2);
         // Sync,  Len, TagetID, CMD1, CMD2, SendID,  CMD,  Res,  Key,  Chk
         // 0x15, 0x09,    0x00, 0x53, 0x53,   0x40, 0x00, 0x00, 0x01, 0x10
         // 0x15, 0x09,    0x01, 0x53, 0x53,   0x40, 0x00, 0x00, 0x03, 0x0D
         P2Value1 = 0;

         P1TxCMD(P1_KEY_CMD, P2SendID, P2Value1, P2Value2);
         break;
       case 0x02:
         break;

       case MIC_PRESS_STATE_CMD: // send a MicPress
         GetFromQue(P2FIFOIn, &P2Value2);

         // Sync,  Len, TagetID, CMD1, CMD2, SendID,  CMD,  Res,  Key,  Chk
         // 0x15, 0x09,    0x00, 0x53, 0x53,   0x40, 0x01, 0x00, 0x00, 0x10  clear
         // 0x15, 0x09,    0x00, 0x53, 0x53,   0x40, 0x01, 0x00, 0xFF, ?     set
         P2Value1 = 0;

         P1TxCMD(P1_MIC_PRESS_CMD, 0, P2Value1, P2Value2);
         break;

       case GROUP_CMD:
         GetFromQue(P2FIFOIn, &P2Value1);
         GetFromQue(P2FIFOIn, &P2Value2);

         if ((0 != P2Value1) || (0 != P2Value2)) {
           P1TxCMD( P1_GROUP_CMD, 0, P2Value1, P2Value2);
         } else {
           P1TxCMD( P1_EXIT_GROUP_CMD, 0, 0, 0);
         }
         break;

       case SEND_ACK_CMD: // Reponse ACK Command         
         GetFromQue(P2FIFOIn, &P2AckID);
         GetFromQue(P2FIFOIn, &P2AckCMD);

         if (P2AckCMD == P2SendCMD) {
           if (bInitial == TRUE) {
             if (P2AckID == Count) {
               bInitial = FALSE;
             }
           } else {
               // if Ack P2TxCount
             P2TxCount = 0;
           }
         }
         break;
       } // end switch(Item)
     } // end if (GetFromQue(P2FIFOIn, &Item))
   } 

   if (P2TxCount != 0) {
     if (systimer.m10sec == P2PrevTimer) {
       P2TxCount--;
       P2TxCMD(AckID, ReceID, Type, status1, status2);

       P2PrevTimer = systimer.m10sec + P2TimeoutSpare;           // add Timer
       if (P2PrevTimer > 99) {
         P2PrevTimer -= 99;
       }
     }
   }   
 }; // terminal of main loop
 
}


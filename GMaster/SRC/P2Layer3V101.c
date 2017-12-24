//
// File Name   : P2Layer3.C
// Auther      : K'un I Chen
// Create      : 07/26/2003
// version     :
// Objective   :
// Description : 
// History     :
#include "gmmain.h"
#include "P2Layer3.h"
#include "Gfifo.h"

data P2L3_STATUS  P2Status;
data BYTE P2SendCMD;

// Create FIFO & Operator function

extern BYTE P2ACK_ID;

void P2Process(void) {  
  // 必須實作 且已經實作
  // P2 term
  // 延伸副機傳到系統端
  // No.   Address   content
  //  1       0x80   0x01, SubID, nn Key                        延伸副機發出
  //  2       0x80   0x02, SubID, ExtendID, nn Key              延伸副機發出
  //  11      0x80   0xF0, SubID                                延伸副機回應 ACK
  //  GMASTERID, Type,  MyID, TagetID,  KeyCode,
  //       0x80, 0x01,  0x00,    0x00,     0x01,     0x77
  //       0x80, 0x01,  0x00,    0x00,     0x0a,     0x6E
  //       0x80, 0x01,  0x00,    0x00,     0x27,     0x51
  //       0x80, 0x01,  0x00,    0x01,     0x03,     0x74
  //       0x80, 0x01,  0x00,    0x02      0x05,     0x71

  if (P2RxBuff[0] != 0x80) { // Address
    // failed packet
    return;
  }

  if (P2RxBuff[1] == P2_ACK) {
      // receive ACK
      PutToQue(P2FIFOIn, SEND_ACK_CMD);        // ACK
      PutToQue(P2FIFOIn, P2RxBuff[2]); // ID
      PutToQue(P2FIFOIn, P2RxBuff[3]); // AckCMD;      
  } else {
    switch(P2RxBuff[1]) {
    case P2_KEY:
      PutToQue(P2FIFOIn, SEND_KEY_CMD);
  //      PutToQue(P2FFIOIn, P2RxBuff[2]); // SubRootID
      PutToQue(P2FIFOIn, P2RxBuff[3]); // OriginalID
      PutToQue(P2FIFOIn, P2RxBuff[4]); // nn Key
      break;     
    case 0x02:
  //      PutToQue(P2FIFOIn, 0x02);
  //      PutToQue(P2FIFOIn, P2RxBuff[2]); // SubRootID
  //      PutToQue(P2FIFOIn, P2RxBuff[3]); // ExtendID
  //      PutToQue(P2FIFOIn, P2RxBuff[4]); // nn Key

  //      P2ResponseACK(P2RxBuff[2]);
      break;
    case P2_MIC_PRESS:
      PutToQue(P2FIFOIn, MIC_PRESS_STATE_CMD);
      PutToQue(P2FIFOIn, P2RxBuff[4]);   
      break;
    case P2_GROUP:
      PutToQue(P2FIFOIn, GROUP_CMD);
      PutToQue(P2FIFOIn, P2RxBuff[3]);  // Group  8 ~ 1
      PutToQue(P2FIFOIn, P2RxBuff[4]);  // Group 10 ~ 9
      break;

    default:
      // not define packet
      return; 
      break;
    }
    P2ACK_ID = P2RxBuff[2];
    P2ResponseACK(P2ACK_ID);
  }

}


  //  系統端傳送到延伸副機
  //  4       SubID  0xF1                                       測試是否有這個 ID
  //  12      0xFF   0x01, ID, RED LED, ACKID         ( 2 BYTE)
  //  13      0xFF   0x02, ID, Green LED, ACKID       ( 2 BYTE)
  //  14      0xFF   0x03, ID, Flash RED LED, ACKID   ( 2 BYTE)
  //  15      0xFF   0x04, ID, Flash GREEN LED, ACKID ( 2 BYTE)
  //  16      0xFF   0x05                                       RESET COMMAND
  //  3       SubID  0xF0                                       回應 ACK 給 延伸副機

   
void P2ResponseACK(BYTE ID) {
  BYTE CheckSum = ACK_Len;

  P2Port_Put(P2Package_SYNC);
  P2Port_Put(ACK_Len);

  CheckSum += ID;
  P2Port_Put(ID);

  CheckSum += 0xF0;     // ACK
  P2Port_Put(0xF0);

  
  P2Port_Put((~CheckSum+1));

  P2SendCMD = 0;
}

//#define P2TxRLEDStatus(ID, RLED1, RLED2)     P2TxCMD((ID), CMD_RLEDSTATUS, (RLED1), (RLED2))
//#define P2TxGLEDStatus(ID, GLED1, GLED2)     P2TxCMD((ID), CMD_GLEDSTATUS, (GLED1), (GLED2))
//#define P2TxGLEDStatus(ID, GLED1, GLED2)     P2TxCMD((ID), CMD_CLEDSTATUS, (CLED1), (CLED2))
//#define P2TxGLEDStatus(ID, GLED1, GLED2)     P2TxCMD((ID), CMD_GRLEDSTATUS, (GLED1), (GLED2))

//#define P2TxFRLEDStatus(ID, FRLED1, FRLED2)  P2TxCMD((ID), CMD_FRLEDSTATUS, (FRLED1), (FRLED2))
//#define P2TxFGLEDStatus(ID, FGLED1, FGLED2)  P2TxCMD((ID), CMD_FRLEDSTATUS, (FGLED1), (FGLED2))
//#define P2TxOLEDStatus(ID, OLED1, OLED2)  P2TxCMD((ID), CMD_OLEDSTATUS, (OLED1), (OLED2))

void P2TxCMD(BYTE bAckID, BYTE ID, BYTE CMD, BYTE S1, BYTE S2) {
  BYTE CheckSum;

  P2Port_Put(P2Package_SYNC);  // the instruction should change
  P2Port_Put(P2CMD_LEN);       // if (!P2Port_Put(x)) return;

  CheckSum = 0xFF + P2CMD_LEN;
  // + bAckID;    // board cast AckID
  P2Port_Put(0xFF);
  

  CheckSum += CMD;   // command
  P2Port_Put(CMD);

  CheckSum += ID;    // Original ID
  P2Port_Put(ID);

  CheckSum += S1;    // byte 1 for status
  P2Port_Put(S1);
  
  CheckSum += S2;    // byte 2 for status
  P2Port_Put(S2);

   
  P2Port_Put((~CheckSum+1));  

  P2SendCMD = CMD;
}

void P2QueryID(BYTE ID) {
  BYTE CheckSum = P2QueryID_LEN;

  P2Port_Put(P2Package_SYNC);
  P2Port_Put(P2QueryID_LEN);
  
  CheckSum += ID;    // Query ID
  P2Port_Put(ID);
  
  CheckSum += CMD_QUERY_ID;
  P2Port_Put(CMD_QUERY_ID);
  
  P2Port_Put((~CheckSum+1));

  P2SendCMD = CMD_QUERY_ID;
}

void P2Run(void) {
  BYTE CheckSum;

  P2Port_Put(P2Package_SYNC);
  P2Port_Put(0x04);   // len
   
  P2Port_Put(0x20); // ID
  CheckSum = 0x24;
 
  P2Port_Put(P2_EXSTART);
  CheckSum += P2_EXSTART;
  
  P2Port_Put(CheckSum); 
}

// 0x15, 0x05, ID, ExCMD, ExSTART, CHKSUM
void P2Stop(void) {
  BYTE CheckSum;

  P2Port_Put(P2Package_SYNC);
  P2Port_Put(0x05);   // len
  CheckSum = 0x05;
  
  P2Port_Put(P2_EXCMD);
  CheckSum += P2_EXCMD;
  
  P2Port_Put(P2_EXSTOP);
  CheckSum += P2_EXSTOP;
  
  P2Port_Put(CheckSum);
}

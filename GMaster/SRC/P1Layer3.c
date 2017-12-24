// 
// File Name   : P1Layer3.C
// Auther      : K'un I Chen
// Create      : 07/26/2003
// version     :
// Objective   :
// Description : 
// History     :

#define IMPLEMENT_P1L3_
#include "stimer.h"
#include "P1Layer3.h"
#include "Gfifo.h"
#include "P1L3CMD.h"


// object implement in GMMAIN.C file
extern SoftTimer systimer;


P1_SEND_STRUCT p1cnt;


void P1InitSend(BYTE SendCount) {
  p1cnt.ReCounter = SendCount;
  p1cnt.Len       = 0;
  p1cnt.Chksum    = 0;
  p1cnt.TimeOut = 0;
}

void P1PutPacket(BYTE d) {
  p1cnt.Buff[p1cnt.Len++] = d;
  p1cnt.Chksum += d;
}

void P1Send(void) {
  BYTE iCount;

  P1Port_Put(P1Sync);
 
  iCount = 0;

  do {
    P1Port_Put(p1cnt.Buff[iCount++]);
  } while(iCount < p1cnt.Len);

  P1Port_Put(p1cnt.Chksum);

  // p1cnt.Len = p1cnt.Buff[P1_REG_LEN];
  if (p1cnt.ReCounter != 0) {
    p1cnt.ReCounter--;
    p1cnt.TimeOut = systimer.m10sec + P1_TIMEOUT_SPACE;
    if (p1cnt.TimeOut > 99) {
      p1cnt.TimeOut -= 99;
    }
  } 
}

  //  系統端傳送到延伸副機
  //  4       SubID  0xF1                                       測試是否有這個 ID
  //  12      0xFF   0x01, ID, RED LED, ACKID         ( 2 BYTE)
  //  13      0xFF   0x02, ID, Green LED, ACKID       ( 2 BYTE)
  //  14      0xFF   0x03, ID, Flash RED LED, ACKID   ( 2 BYTE)
  //  15      0xFF   0x04, ID, Flash GREEN LED, ACKID ( 2 BYTE)
void P1Process(void)  {
  // in the function process packect
  switch(P1RxBuff[1]) {
  case 'R': // Resetup
    #include ".\P1L3\RA.C"
    break;
  case 'K': // Key
    #include ".\P1L3\KA.C" 
    break;
  case 'G': // Group
    #include ".\P1L3\GA.C"
    break;
  case 'E': // Emergency
    #include ".\P1L3\EA.C"
    break;    
  case 'S': // Circuit short
    #include ".\P1L3\SA.C"
    break;
  case 'T': // Emergency telephone
    #include ".\P1L3\TA.C"
    break;
  case 'F': // Fire alarm
    #include ".\P1L3\FA.C"
    break;
  case 'M': // Setup
    #include ".\P1L3\MA.C"
    break;
  case 'B': // Background music
    #include ".\P1L3\BA.C"
    break; 
  case  'C':  // Alarm remove
    #include ".\P1L3\CA.C"
    break;   
  case 'H': // Man handling
    #include ".\P1L3\HA.C"
    break;
  case 'P': // Micphone press push talk
    #include ".\P1L3\PA.C"
    break;
  case SPC_COMMAND:
    #include ".\P1L3\SC.C"
    break;
  }
} // terminal of P1Process(void)


void P1TxCMD(BYTE CMD, BYTE ID, BYTE Value1, BYTE Value2)
{
  BYTE CheckSum;

  P1Port_Put(P1Sync);
  P1Port_Put(0x09);   // len
  CheckSum = 0x09;

  P1Port_Put(ID);
  CheckSum += ID;
  
  P1Port_Put('S');
  CheckSum += 'S';

  P1Port_Put('S');
  CheckSum += 'S';

  P1Port_Put(0x40);
  CheckSum += 0x40;

  P1Port_Put(CMD);
  CheckSum += CMD;

  P1Port_Put(Value1);
  CheckSum += Value1;

  P1Port_Put(Value2);
  CheckSum += Value2;

  P1Port_Put(~CheckSum+1);

}

// 0x15, 0x05, ID, ExCMD, ExSTOP, CHKSUM
void P1Stop() {
  BYTE CheckSum;

  P1Port_Put(P1Sync);
  P1Port_Put(0x05);   // len
  CheckSum = 0x05;
  
  P1Port_Put(P1_EXCMD);
  CheckSum += P1_EXCMD;
  
  P1Port_Put(P1_EXSTOP);
  CheckSum += P1_EXSTOP;
  
  P1Port_Put(CheckSum);
}

// 0x15, 0x05, ID, ExCMD, ExSTART, CHKSUM
void P1Run(void) {
  BYTE CheckSum;

  P1Port_Put(P1Sync);
  P1Port_Put(0x04);   // len
   
  P1Port_Put(0x20); // ID
  CheckSum = 0x24;
 
  P1Port_Put(P1_EXSTART);
  CheckSum += P1_EXSTART;
  
  P1Port_Put(CheckSum);  
}


void P1SendKEY(BYTE ID, BYTE KEY) {
  BYTE CheckSum;
  
  P1Port_Put(P1Sync);
  P1Port_Put(0x05);   // len
  CheckSum = 0x05;
  
  P1Port_Put(ID);
  CheckSum += ID;
  
  P1Port_Put(P1_EXKEY);
  CheckSum += P1_EXKEY;
  
  P1Port_Put(KEY);
  CheckSum += KEY;
  
  P1Port_Put(CheckSum);
}

// 0x15, 0x05, ID, ExCMD, ExQuery, CHKSUM
void P1QueryState(BYTE QC) {
  BYTE CheckSum;

  P1Port_Put(P1Sync);
  P1Port_Put(0x05);   // len
  CheckSum = 0x05;
  
  // ID
  P1Port_Put(QC);
  CheckSum += QC;
  
  // COMMAND
  P1Port_Put(P1_EXCMD);
  CheckSum += P1_EXCMD;
  
  P1Port_Put(P1_EXQUERY);
  CheckSum += P1_EXQUERY;
  
  P1Port_Put(CheckSum);
}

void ResponseP1ACK(BYTE ID, BYTE ACKCMD)
{
  BYTE CheckSum;

  P1Port_Put(P1Sync);
  P1Port_Put(0x06);   // len
  CheckSum = 0x06;

  P1Port_Put(ID);
  CheckSum += ID;

  P1Port_Put(0xAA);
  CheckSum += 0xAA;

  P1Port_Put(0x55);
  CheckSum += 0x55;

  P1Port_Put(ACKCMD);
  CheckSum += ACKCMD;

  P1Port_Put(~CheckSum+1);
}

//----------------------------------------------------------------------------
// Function Name: IsCompleteP1Tx(void)
// Parameter: None
// Result: if TRUE is Complete a packet transmit
//            FALSE then re-send packet

/*
BOOLEAN IsCompleteP1Tx(void)
{
  return (p1cnt.ReCounter != 0) ? FALSE : TRUE;
}
*/

/*
 P1InitSend(3);
 P1PutPacket(0x05);
 P1PutPacket(0x04);
 P1PutPacket(0x02);
 P1PutPacket(0x01);
 P1Send();
*/


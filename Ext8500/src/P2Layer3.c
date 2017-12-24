// 
// File Name   : P2Layer3.C
// Auther      : K'un I Chen
// Create      : 07/26/2003
// version     :
// Objective   :
// Description : 
// History     :

#include "P2Layer3.h"
#include "Gfifo.h"

data P2L3_STATUS  P2Status;

// Create FIFO & Operator function



void P2Process(void)
{
  BYTE temp;
  
  
  if (P2RxBuff[0] != 0x80)
  {
    return;
  }
  
  switch(P2RxBuff[1])
  {
    case 0x01:
      // ACK response
      PutToQue(P2FIFO,0x01);
      break;

    case 0x02:
      // Send a key

      break;

    case 0x03:
      // Request System status

      break;

    case 0x04:
      // NACK response;

      break;

    case 0x05:
      // Request Master ring

      break;

    case 0x06:
      // Release Master ring

      break;

    case 0x07:
      // Send Green LED status
      break;

    case 0x08:
      // Send RED LED status
      break;

    case 0x09:
      // Request Green LED status
      break;

    case 0x0A:
      // Request RED LED status
      break;

    case 0x0B:
      // Send System status
      break;

    default:
      break;
  }
}


void TranslateP2(register BYTE Item)
{
   BYTE temp;
   switch(Item)
   {
     case  3: // system initial
     case  4:
          break;
     case  6: // extend key setting
          GetFromQue(P1FIFO, &temp); // MachineNo.
          GetFromQue(P1FIFO, &temp); // Key
          break;
     case  8: // extend key relaese
          GetFromQue(P1FIFO, &temp); // MachineNo.
          GetFromQue(P1FIFO, &temp); // Key
          break;
     case 10: // main unit key setting
          GetFromQue(P1FIFO, &temp); // Key
          break;
     case 12: // main unit key setting
          GetFromQue(P1FIFO, &temp); // Key 
     case 14: // quit Gropu mode
          break;
     case 16: // entry Group mode
          break;
     case 18: // active group 
          GetFromQue(P1FIFO, &temp); // NN
          break;
     case 20: // receive Group state mode
          GetFromQue(P1FIFO, &temp); // MSB 
          GetFromQue(P1FIFO, &temp); // LSB
          break;
     case 22: // entry Emergency mode
          break;
     case 24: // quit Emergency mode
          break;
     case 26: // extend & main unit loop short 
          GetFromQue(P1FIFO, &temp); // MachineNo.
          GetFromQue(P1FIFO, &temp); // MSB 
          GetFromQue(P1FIFO, &temp); // LSB
          break;
     case 30: // extend telephone alram
          GetFromQue(P1FIFO, &temp); // MachineNo.
          GetFromQue(P1FIFO, &temp); // MSB 
          GetFromQue(P1FIFO, &temp); // LSB            
          break;
     case 32: // main & extend fire alram No. 34 same
          GetFromQue(P1FIFO, &temp); // MachineNo.
          GetFromQue(P1FIFO, &temp); // MSB 
          GetFromQue(P1FIFO, &temp); // LSB
          break;
     case 63: // entry BGM mode
          break;
     case 65: // quit  BGM mode
          break;
     case 67: // quit alram mode
          break;
     case 69: // quit manual mode
          break;
     case 71: // entry manual mode
          break;
     case 73: // entry push talk 
          break;
     case 0x90: // Send LED Status
          break;
     default:
          break;
   }
}

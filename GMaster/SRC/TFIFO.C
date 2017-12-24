#include "common.h"
#include "regw77e58.h"
#define MaxTQueLen 32
typedef struct {
                 BYTE  QueT;
                 BYTE  QueH;
                 BYTE  QueLen;
                 PBYTE  ptrData;
                } TFIFO;

typedef TFIFO* PTFIFO;

#define InitialTQue(x)   ((x).QueH) = ((x).QueT) = (x).QueLen = 0; \
                         (x).ptrData = x##data

#define IsEmptyOfTQue(x) ((*x).QueLen == 0) ? TRUE:FALSE
#define IsFullOfTQue(x)  (((*x).QueLen) >= MaxTQueLen) ? TRUE:FALSE
#define GetTQueLen(x)    ((x).QueLen)

#define CreateTFIFO(x)   TFIFO (x); \
                         BYTE xdata x##data[MaxTQueLen]



Boolean PutToFIFO(PTFIFO fifo, Val)
{
   EA = FALSE;
   
   if (IsFullOfTQue(fifo))
   { 
     EA = TRUE; 
     return FALSE; 
   }

   fifo->QueLen++;
   fifo->ptrData[fifo->QueT++] = Val;
							                                                  \
   if(fifo->QueT >= MaxTQueLen) 
      fifo->QueT = 0;
   EA = TRUE;
   return TRUE;
}

Boolean GetFromFIFO(PTFIFO fifo, PBYTE Val)
{
  EA = FALSE;

  if (IsEmptyOfTQue(fifo))
  { 
    EA = TRUE; 
    return FALSE; 
  }
  
  *Val = fifo->ptrData[fifo->QueH++]; 
  fifo->QueLen--;
 
  if (fifo->QueH >= MaxTQueLen) 
    fifo->QueH = 0;

  EA = TRUE;
  return TRUE;
}                                                     


void main(void)
{
  BYTE temp;
  BYTE xdata Temp1[MaxTQueLen+2];
  CreateTFIFO(test);

  InitialTQue(test);

  for(temp = 0; temp < 32; temp++)
     Temp1[temp] = temp+1;

  for(temp = 0; temp < MaxTQueLen+2; temp++)
     PutToFIFO(&test, Temp1[temp]);

  for(temp = 0; temp < MaxTQueLen+2; temp++)
     GetFromFIFO(&test, &temp);

  while(1);
}

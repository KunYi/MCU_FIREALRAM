#if !defined ( _UART_H_ )
#define _UART_H_
#define MaxLength 32

#define UART0_RS485_Adapter
#define UART0_RS485_DelayTime 5
#define UART1_RS485_Adapter
#define UART1_RS485_DelayTime 5


#define UART0_R_Length   RLength0
#define UART0_T_Length   TLength0
#define UART0_R_IsEmpty  ((RLength0 == 0 ) ? TRUE : FLASE)
#define UART0_R_IsFull   ((RLength0 == MaxLength) ? TRUE : FLASE )
#define UART0_T_IsEmpty  ((TLength0 == 0 ) ? TRUE : FLASE)
#define UART0_T_IsFull   ((TLength0 == MaxLength) ? TRUE : FLASE )
#define UART1_R_Length   RLength1
#define UART1_T_Length   TLength1
#define UART1_R_IsEmpty  ((RLength1 == 0 ) ? TRUE : FLASE)
#define UART1_R_IsFull   ((RLength1 == MaxLength) ? TRUE : FLASE )
#define UART1_T_IsEmpty  ((TLength1 == 0 ) ? TRUE : FLASE)
#define UART1_T_IsFull   ((TLength1 == MaxLength) ? TRUE : FLASE )

#include "regw77e58.h"

#if ! defined ( _PYHSICAL_UART_ )
   extern BYTE RLength0;
   extern BYTE TLength0;
   extern BYTE RLength1;
   extern BYTE TLength1;

   extern bit __uart0;
   extern bit __uart1;
#endif 

#if defined ( UART0_RS485_Adapter )
sbit __DIR0 = P1 ^ 0;
   #if !defined( TRANSMIT )
      #define TRANSMIT 1
      #define RECEIVE  0
   #endif 
#endif
 
#if defined ( UART1_RS485_Adapter )
sbit __DIR1 = P1 ^ 1;
    #if !defined( TRANSMIT )
        #define TRANSMIT 1
        #define RECEIVE  0
    #endif 
#endif 

void UART0_Interrupt(void);
void UART1_Interrupt(void);
void InitUART0(void);
void InitUART1(void);
BOOLEAN UART0_Put(BYTE);
BOOLEAN UART0_Get(BYTE *);
void UART0Drive(void);
void UART1Drive(void);
BOOLEAN UART1_Put(BYTE);
BOOLEAN UART1_Get(BYTE *);

#endif
/* UART.H FILE END */

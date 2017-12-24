#ifndef _COMMON_H_
#define _COMMON_H_ 1

    #ifndef EOF
        #define EOF 0xffff;
    #endif 

typedef unsigned char  BYTE;
typedef unsigned int   WORD;
typedef unsigned long DWORD;

typedef unsigned char* PBYTE;
typedef unsigned int* PWORD;
typedef WORD         Status;


typedef enum { FALSE = 0, OFF = 0 , NO = 0, DISABLE = 0, TRUE = 1, ON = 1, YES = 1, ENABLE = 1 } BOOLEAN;
typedef enum { LOW = 0, HI = 1 } IO_STATE;
typedef struct {
                 BYTE *head;
                 BYTE *tail;
//                 BYTE *Buffer;
               } QUEUE;


#define CODE_DATA           code
#define P_INTERNAL_DATA    idata
#define INTERNAL_DATA       data
#define BIT_DATA           bdata
#define EXTERN_DATA        xdata
#define P_EXTERN_DATA      pdata
#define DIRECT_BYTE_IO(PORT)  (*((unsigned char  volatile xdata *) PORT))
#define DIRECT_WORD_IO(PORT)  (*((unsigned int  volatile xdata *) PORT))
#define SET_REGISTER(reg) reg = ENABLE
#define RESET_REGISTER(reg) reg = DISABLE

// #define __SS_COMMAND_ 1

#endif 
/* COMMON.H FILE END */

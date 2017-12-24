#if ! defined ( _COMMON_H_ )
#define _COMMON_H_
/* ===================================================================
   Please defined use compile, Thanks
   is Franklin PRO_VIEW ==> #define _PRO_VIEW_
   or Keil              ==> #define _Keil_
   or AD2500 C          ==> #define _AD2500_
   or IAR ICC8051       ==> #define _IAR_
  ================================================================= */
#define _PRO_VIEW_       /* defined use Compile */

#if ! defined ( EOF )
#define EOF 0xffff;
#endif 
typedef unsigned char  BYTE;
typedef unsigned int   WORD;
typedef unsigned long DWORD;

typedef unsigned char* PBYTE;
typedef unsigned int* PWORD;
typedef WORD         Status;


typedef enum { FALSE = 0, OFF = 0 , NO = 0, DISABLE = 0, false = 0, TRUE = 1, ON = 1, YES = 1, ENABLE = 1, true = 1 } BOOLEAN;
typedef bit  Boolean;
typedef enum { LOW = 0, HI = 1 } IO_STATE;


#if defined (_PRO_VIEW_)

// use Franklin
#define CODE_DATA           code
#define P_INTERNAL_DATA    idata
#define INTERNAL_DATA       data
#define BIT_DATA           bdata
#define EXTERN_DATA        xdata
#define P_EXTERN_DATA      pdata
#define DIRECT_BYTE_IO(Port)  (((volatile BYTE*) 0x20000L)[Port])
#define DIRECT_WORD_IO(Port)  (((volatile WORD*) 0x20000L)[Port])
#define SET_REGISTER(reg) reg = ENABLE
#define RESET_REGISTER(reg) reg = DISABLE
// end

#elif defined (_Kile_)
// use Kile
#define CODE_DATA           code
#define P_INTERNAL_DATA    idata
#define INTERNAL_DATA       data
#define BIT_DATA           bdata
#define EXTERN_DATA        xdata
#define P_EXTERN_DATA      pdata
#define DIRECT_BYTE_IO(Port)   (*(volatile BYTE*) Port)
#define DIRECT_WORD_IO(Port)   (*(volatile WORD*) Port)

#elif defined (_AD2500_)
// use AD2500
#define CODE_DATA           code
#define P_INTERNAL_DATA    idata
#define INTERNAL_DATA       data
#define BIT_DATA           bdata
#define EXTERN_DATA        xdata
#define P_EXTERN_DATA      pdata
#define DIRECT_BYTE_IO(Port)   (*(volatile BYTE*) Port)
#define DIRECT_WORD_IO(Port)   (*(volatile WORD*) Port)

#elif defined (_IAR_)
//use IAR 
#define CODE_DATA           code
#define P_INTERNAL_DATA    idata
#define INTERNAL_DATA       data
#define BIT_DATA           bdata
#define EXTERN_DATA        xdata
#define P_EXTERN_DATA      pdata
#define Locate(Address)       at Address
#define DIRECT_BYTE_IO(Port)   (*(volatile BYTE*) Port)
#define DIRECT_WORD_IO(Port)   (*(volatile WORD*) Port)

#elif defined (_PCSIM_)
#define CODE_DATA
#define P_INTERNAL_DATA
#define INTERNAL_DATA
#define BIT_DATA
#define EXTERN_DATA
#define P_EXTERN_DATA
#define Locate(Address)
#define DIRECT_BYTE_IO(Port)   (*(volatile BYTE*) Port)
#define DIRECT_WORD_IO(Port)   (*(volatile WORD*) Port)
#define sfr   unsigned char
#define sfr16 short unsigned int
#define sbit  unsigned char
#define	bit   unsigned char

#else
//no defined 
#define CODE_DATA          
#define P_INTERNAL_DATA    
#define INTERNAL_DATA      
#define BIT_DATA           
#define EXTERN_DATA        
#define P_EXTERN_DATA
#define DIRECT_BYTE_IO(Port)
#define DIRECT_WORD_IO(Port)
#error "No Predefine use Compile, Please. Reference file: common.h Line:4"
#endif

#endif 
/* COMMON.H FILE END */

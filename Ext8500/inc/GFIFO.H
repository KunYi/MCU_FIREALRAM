#ifndef _GFIFO_H_
#define _GFIFO_H_
#include "regw77e58.h"
typedef struct {  BYTE QueH; 
                  BYTE QueT;
				  BYTE QueLen; } QueCtrl;

/* data struct operation macro define               */
#define MaxQueLen  (48)

#define CreateQue(x)        BYTE xdata x[MaxQueLen];                          \
                            QueCtrl data x##C;                                \
							BOOLEAN _PutTo##x(BYTE Val);                      \
							BOOLEAN _GetFrom##x(PBYTE Val);

#define ExportQueFunction(x)   extern  _PutTo##x(BYTE Val);                   \
                               extern  _GetFrom##x(PBYTE Val);                \
                               extern  QueCtrl data x##C;


#define ImplementQue(x)     BOOLEAN _PutTo##x(BYTE Val)                       \
                            {                                                 \
							 EA = FALSE;                                      \
                             if (IsFullOfQue(x))                              \
							 { EA = TRUE; return FALSE; }                     \
							 x##C.QueLen++;                                   \
                             x[x##C.QueT++] = Val;                            \
							                                                  \
                             if(x##C.QueT >= MaxQueLen) x##C.QueT = 0;        \
							 EA = TRUE;                                       \
                             return TRUE;                                     \
                            }                                                 \
                                                                              \
                            BOOLEAN _GetFrom##x(PBYTE Val)                    \
                            {                                                 \
							 EA = FALSE;                                      \
							 if (IsEmptyOfQue(x))                             \
						       { EA = TRUE; return FALSE; }                   \
							 *Val = x[x##C.QueH++];                           \
					         x##C.QueLen--;                                   \
							 if(x##C.QueH >= MaxQueLen) x##C.QueH = 0;        \
							 EA = TRUE;                                       \
							 return TRUE;                                     \
							}                                                     
							                                                       
#define InitialQue(x)   x##C.QueH = x##C.QueT = x##C.QueLen = 0
#define IsEmptyOfQue(x) (x##C.QueLen == 0) ? TRUE:FALSE
#define IsFullOfQue(x)  (x##C.QueLen >= MaxQueLen) ? TRUE:FALSE
#define GetQueLen(x)    x##C.QueLen
#define PutToQue(x, value) _PutTo##x((value))
#define GetFromQue(x, value) _GetFrom##x(value)

#endif

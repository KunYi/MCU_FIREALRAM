#ifndef _QUERY_H_
#define _QUERY_H_
#include "common.h"

typedef enum { IDLE_QUERY = 0, WAIT_QUERY, INIT_QUERY, MAIN_QUERY, STOP_QUERY } QUERY_STATE;
#define START_QUERY()   QueryState = INIT_QUERY

#define  MAX_QUERY      17

void InitialQuery(void);
void SetExist(BYTE qc);
bit  IsQuery(BYTE qc);


#ifdef _IMPLEMENT_QUERY_
data BYTE Exist[2];
data QUERY_STATE QueryState;
data BYTE QueryCount;
//bit bOverTimeQuery;
extern CODE_DATA const BYTE CONVBIT[];
#else
extern data BYTE Exist[2];
extern data QUERY_STATE QueryState;
extern BYTE QueryCount;
//extern bOverTimeQuery;
#endif

#endif
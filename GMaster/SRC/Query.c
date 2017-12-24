#define _IMPLEMENT_QUERY_
#include "Query.h"

void SetExist(BYTE qc) {
  if (qc == 0) {
    return;
  } else {
   if (qc < 8)  {
     Exist[0] |= CONVBIT[qc];
   } else if (qc < 16) {
     Exist[1] |= CONVBIT[qc - 8];
   }
  }
}

bit IsQuery(BYTE qc) {
   if (qc == 0) {
     return 1;
   } else {
     if (Exist < 8) {
       return (Exist[0] & CONVBIT[qc]) ? 1 : 0;
     } else if (Exist < 16) {
       return (Exist[1] & CONVBIT[qc-8]) ? 1 : 0;
     } else {
       return 0;
     }
   }
}

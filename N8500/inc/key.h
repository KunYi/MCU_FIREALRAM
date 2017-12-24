#if ! defined ( _KEY_H_ )
#define _KEY_H_

#include "common.h"
#include "sm10io.h"
#include "gfifo.h"
//#define MaxKey  32

#define PHYSICAL_KeyM1 PORT2_RW
#define PHYSICAL_KeyM2 PORT1_RW
#define PHYSICAL_KeyM3 PORT6_RW
#define PHYSICAL_KeyM4 PORT7_RW
#define PHYSICAL_KeyM5 PORT8_RW

BYTE GetKey(void);
BYTE GetPushKey(void);
void PHYSICAL_GetKey(void);
WORD PushCount(void);
#endif
/* KEY.H FILE END */

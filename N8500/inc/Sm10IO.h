#if ! defined ( _SM10_IO_H_ )
#define _SM10_IO_H_

#include "common.h"

#define  CANCEL_IO      DIRECT_BYTE_IO(0xFFFF)

#define  IO_BASE       0x5000
#define  PORT0_RW      DIRECT_BYTE_IO(IO_BASE)
#define  PORT1_RW      DIRECT_BYTE_IO(IO_BASE+0x40)
#define  PORT2_RW      DIRECT_BYTE_IO(IO_BASE+0x80)
#define  PORT3_RW      DIRECT_BYTE_IO(IO_BASE+0xc0)
#define  PORT4_RW      DIRECT_BYTE_IO(IO_BASE+0x100)
#define  PORT5_RW      DIRECT_BYTE_IO(IO_BASE+0x140)
#define  PORT6_RW      DIRECT_BYTE_IO(IO_BASE+0x180)
#define  PORT7_RW      DIRECT_BYTE_IO(IO_BASE+0x1c0)
#define  PORT8_RW      DIRECT_BYTE_IO(IO_BASE+0x200)
#define  PORT9_RW      DIRECT_BYTE_IO(IO_BASE+0x240)
#define  voice_bank_select    DIRECT_BYTE_IO(IO_BASE+0x280)
//#define  ExtendByteIO  DIRECT_BYTE_IO(0x0000)

#endif
/* SM10.H FILE END */

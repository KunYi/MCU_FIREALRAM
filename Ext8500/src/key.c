#include "key.h"


#define KeyTable LEDTable
extern CODE_DATA BYTE LEDTable[];

//static P_INTERNAL_DATA BYTE KeyBuffer[10];
static P_INTERNAL_DATA BYTE PreKey;
static P_INTERNAL_DATA BYTE KeyCode;
static P_INTERNAL_DATA WORD LockCount;
//static P_INTERNAL_DATA QUEUE KeyQueue = { &KFIFO[0], &KFIFO[0]};
// static P_INTERNAL_DATA BOOLEAN KeyTrue;
static bit KeyTrue = FALSE;
static bit KeyPush = FALSE;
static bit IsKey   = FALSE;


CODE_DATA  const BYTE ConverKEYTable[] = {  0,  1,  2,  3,  4,  5,  6,  7,
									 	    0,  0,  8,  9,  0,  0,  0,  0, 
										   16, 17, 18, 19, 20, 21, 22, 23,
										   24, 25, 38, 39,  0,  0,  0,  0, 
                                           30, 31, 32, 33, 34, 35, 36, 37};

CreateQue(KeyFIFO)
ImplementQue(KeyFIFO)

void PHYSICAL_GetKey(void) {
  BYTE KeyM1, KeyM2, KeyM3, KeyM4, KeyM5, i; 
  BYTE KeyValue;
  
  KeyM1 = PHYSICAL_KeyM1;
  KeyM2 = PHYSICAL_KeyM2;
  KeyM3 = PHYSICAL_KeyM3;
  KeyM4 = PHYSICAL_KeyM4;
  KeyM5 = PHYSICAL_KeyM5;
      
  /* Key moudle 2 only bit 2, 3 is key button */
  KeyM2 |= 0xf3;
  /* Key moudle 4 only bit 0,1,2,3 is key button */
  KeyM4 |= 0xf0;
      
  KeyValue = 0xff;
      
  if (KeyM1 != 0xff) { 
    if ((KeyM2 == 0xff) && (KeyM3 == 0xff) && (KeyM4 == 0xff) && (KeyM5 == 0xff)) {
      for ( i = 0; i < 8; i++) {
        if (KeyM1== ~KeyTable[i]) {
          KeyValue = i; break; 
        } 
      } 
    } else { 
      KeyTrue = KeyPush = FALSE;
      return; 
    }
  } else { 
    if (KeyM2 != 0xff) { 
      if ((KeyM3 == 0xff) && (KeyM4 == 0xff && (KeyM5 == 0xff))) {
        for ( i = 0; i < 8; i++) {  
          if (KeyM2 == ~KeyTable[i]) {
            KeyValue = i+8; 
            break; 
          } 
        } 
      } else { 
        KeyTrue = KeyPush = FALSE; 
        return; 
      }
    } else { 
      if (KeyM3 != 0xff) {
        if ((KeyM4 == 0xff) && (KeyM5 == 0xff)) {
          for ( i = 0; i < 8; i++) {
            if (KeyM3 == ~KeyTable[i]) { 
              KeyValue = i+16;
              break; 
            } 
          } 
        } else {
          KeyTrue = KeyPush = FALSE;
          return; 
        }
      } else { 
        if (KeyM4 != 0xff) {
          if (KeyM5 == 0xff) {
            for ( i = 0; i < 8; i++) { 
              if (KeyM4 == ~KeyTable[i]) { 
                KeyValue = i+24; 
                break; 
               } 
            }
          } else { 
            KeyTrue = KeyPush = FALSE;
            return; 
          }
        } else {
          if (KeyM5 != 0xff) {
            for ( i = 0; i < 8; i++) {
              if (KeyM5 == ~KeyTable[i]) {
                KeyValue = i+32;
                break;
              } 
            } 
          } else {
            KeyTrue = KeyPush = FALSE;
            return; 
          }
        }
      }
    }
  }

  if (KeyValue  == 0xFF) { 
    KeyTrue = KeyPush = FALSE;  
    PreKey = 0xff;
    return;
  }

  KeyValue = ConverKEYTable[KeyValue];
  /*  Is Key Debound       */
  if (PreKey == KeyValue) {  
    if (KeyPush) { 
      LockCount++; 
    } else { 
      if (KeyTrue) { 
        KeyPush = TRUE; 
        KeyTrue = FALSE; 
      } else { 
        KeyTrue = IsKey = TRUE;
        PutToQue(KeyFIFO, KeyValue);
        LockCount = 0;
        KeyCode = KeyValue;
      }
    }
  } else {
    PreKey = KeyValue;
    KeyTrue = KeyPush = FALSE;
  } 
}

  
BYTE GetKey(void) {
  char KeyTemp;

  if (!IsEmptyOfQue(KeyFIFO)) {
    GetFromQue(KeyFIFO, &KeyTemp);
    return KeyTemp;
  }
  return (BYTE) EOF; 
}

BYTE GetPushKey() {
  if (KeyPush) { 
    return PreKey; 
  }
  return (BYTE) EOF;
}

WORD PushCount(void) {
 return LockCount;
}






















































































































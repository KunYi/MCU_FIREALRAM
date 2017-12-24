#if !defined( _RELAY_H_ )
#define _RELAY_H_
#include "common.h"
#include "sm10.h"
#include "sm10IO.h"

#ifdef _DEBUG_CANCEL_WRITE_IO_
  #define PHYSICAL_RELAY0    CANCEL_IO
  #define PHYSICAL_RELAY1    CANCEL_IO
  #define PHYSICAL_FUSE0     CANCEL_IO
  #define PHYSICAL_FUSE1     CANCEL_IO  
#else
  #define PHYSICAL_RELAY0    PORT3_RW
  #define PHYSICAL_RELAY1    PORT4_RW
  #define PHYSICAL_FUSE0     PORT4_RW
  #define PHYSICAL_FUSE1     PORT1_RW     /* use bit 6, 7 */
#endif

void RefreshRelay(void);
void SetRelay(BYTE);
void ClrRelay(BYTE);
void SetEmergencyRelay(void);
void ClrEmergencyRelay(void);
void SetFireStatusRelay(void);
void ClrFireStatusRelay(void);
void SetRelayBuffer(const BYTE, const BYTE);
//BOOLEAN IsSetRelay(const BYTE);
BOOLEAN IsShort(const BYTE);
BOOLEAN IsAnyLoopShort(void);

void StoreRelayState(void);
void RestoreRelayState(void);

#define ClrAllRelay() SetRelayBuffer(0,0)
#endif 
/* _RELAY_H_ FILE END */

#if ! defined ( _EEPROM_H_ )
#define _EEPROM_H_
#include "common.h"
#define Read_BYTE_EEPROM(Address) DIRECT_BYTE_IO((0x8000+(Address)))

// by Simon.
//#define Write_BYTE_EEPROM(Address, value) DIRECT_BYTE_IO((0x8000+(Address))) = (value)
#define dWrite_BYTE_EEPROM(Address, value) DIRECT_BYTE_IO((0x8000+(Address))) = (value)
// end of by Simon.

#define FireRelationStorage        1
#define GroupStorage               2
#define BackGroundMusicStorage     3
#define VoiceMessageStorage        4
#define FireStatusStorage          5
#define NormalStorageStatus               0x00
#define PreFireEmergencyStorageStatus     0x01
#define FireEmergencyStorageStatus        0x02
#define NoFireEmergencyStorageStatus      0x03
#define AllZoneEmergencyStorageStatus     0x04
WORD	CalcEEPROMOffset(BYTE para1, BYTE papa2, BYTE para3);
// by Simon.
void	Write_BYTE_EEPROM(WORD address,BYTE data1);
void	disable_28c64b_protection(void);
// end fo by Simon.
#endif 
/* _EEPROM_H_ HANDLE FILE END */

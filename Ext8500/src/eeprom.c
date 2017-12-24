#include "eeprom.h"


// by Simon
// this modification will check the write done status to ensure the proper write has been done.
// refer to the datasheet of any 28C64B and 28C64 for the detail

void
Write_BYTE_EEPROM(unsigned short address, unsigned char the_data)
{
	BYTE d1,d2;
	dWrite_BYTE_EEPROM(address, the_data);
	// wait for complete
	do{
		d1=Read_BYTE_EEPROM(address);
		d1 &= 0x40;
		d2=Read_BYTE_EEPROM(address);
		d2 &= 0x40;
		if(d1==d2){
			d1=0;
		}else{
			d1=1;
		}
	}while(d1==1);
}

// end of by simon.

WORD CalcEEPROMOffset(BYTE TYPE, BYTE ID, BYTE ZONE)
{
  switch (TYPE)
  {
   case  FireRelationStorage:
         return ((ID*20)+(ZONE*2));
   case  GroupStorage:
         return ((ZONE*2)+0x140);
   case  BackGroundMusicStorage:
         return (0x160);
   case  VoiceMessageStorage:
         return ((ID*80)+(ZONE*8)+0x170);
   case  FireStatusStorage:
         return (0x700);
         /* 0x700  -> zone 1 -  8 neg status
          * 0x701  -> zone 9 - 10 neg status 
          * 0x702  -> 0x00        = normal
          *           0x01        = pre fire emergency
          *           0x02        = fire emergency
          *           0x03        = no fire emergency
          *           0x04        = allzone
          *           0x05 - 0xff = normal
          */
   default: return 0xfff0;
  }
}

//by Simon
// to disable the 28C54B type EEPROM's software protection
// refer to the datasheet of any 28C64B for the detail of disable the software protection

void
disable_28c64b_protection(void)
{
	unsigned char b1,b2;
	//store original data	//for compatiable to the 28C64
	b1=Read_BYTE_EEPROM(0x1555);
	b2=Read_BYTE_EEPROM(0x0AAA);
	
	//start the disable procedure
	dWrite_BYTE_EEPROM(0x1555, 0xAA);
	dWrite_BYTE_EEPROM(0x0AAA, 0x55);
	dWrite_BYTE_EEPROM(0x1555, 0x80);
	dWrite_BYTE_EEPROM(0x1555, 0xAA);
	dWrite_BYTE_EEPROM(0x0AAA, 0x55);
	dWrite_BYTE_EEPROM(0x1555, 0x20);
	//end of the procedure
	
	//restore original data
	Write_BYTE_EEPROM(0x1555,b1);
	Write_BYTE_EEPROM(0x0AAA,b2);
}

// end of by simon.


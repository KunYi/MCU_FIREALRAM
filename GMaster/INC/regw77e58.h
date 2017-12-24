#ifndef REG_W77E58_H
#define REG_W77E58_H 
/* Copyright GOOD WILL INSTRUMENT Co.,Ltd. 1999 
* REGW77E58.H
* Last Modified By K'un I Chen
* First Modified 05/04/1999
* Last Modified 05/05/1999
* Modified for Version 1.00
* Notes:
* Winbone W77E58 Processor Declarations
*/
/*  BYTE Registers  */
sfr P0     = 0x80;
sfr P1     = 0x90;
sfr P2     = 0xA0;
sfr P3     = 0xB0;
sfr PSW    = 0xD0;
sfr ACC    = 0xE0;
sfr B      = 0xF0;
sfr SP     = 0x81;
sfr DPL    = 0x82;
sfr DPH    = 0x83;
sfr PCON   = 0x87;
sfr TCON   = 0x88;
sfr TMOD   = 0x89;
sfr TL0    = 0x8A;
sfr TL1    = 0x8B;
sfr TH0    = 0x8C;
sfr TH1    = 0x8D;
sfr IE     = 0xA8;
sfr IP     = 0xB8;
sfr SCON0  = 0x98;
sfr SCON   = 0x98;
sfr SBUF0  = 0x99;
sfr SBUF   = 0x99;

/*  W77E58 Extensions  */
sfr DPL1   = 0x84;
sfr DPH1   = 0x85;
sfr DPS    = 0x86;
sfr CKCON  = 0x8E;
sfr EXIF   = 0x91;
sfr SADDR0 = 0xA9;
sfr SADDR1 = 0xAA;
sfr SADEN0 = 0xB9;
sfr SADEN1 = 0xBA;
sfr SCON1  = 0xC0;
sfr SBUF1  = 0xC1;
sfr PMR    = 0xC4;
sfr STATUS = 0xC5;
sfr TA     = 0xC7;
sfr T2CON  = 0xC8;
sfr T2MOD  = 0xC9;
sfr RCAP2L = 0xCA;
sfr RCAP2H = 0xCB;
sfr TL2    = 0xCC;
sfr TH2    = 0xCD;
sfr WDCON  = 0xD8;
sfr EIE    = 0xE8;
sfr EIP    = 0xF8;
sfr16 RCAP2 = 0xCA;

/*  BIT Registers  */
/*  PSW  */
sbit CY    = 0xD7;
sbit AC    = 0xD6;
sbit F0    = 0xD5;
sbit RS1   = 0xD4;
sbit RS0   = 0xD3;
sbit OV    = 0xD2;
sbit FL    = 0xD1;
sbit P     = 0xD0;

/*  TCON  */
sbit TF1   = 0x8F;
sbit TR1   = 0x8E;
sbit TF0   = 0x8D;
sbit TR0   = 0x8C;
sbit IE1   = 0x8B;
sbit IT1   = 0x8A;
sbit IE0   = 0x89;
sbit IT0   = 0x88;

/*  IE  */
sbit EA    = 0xAF;
sbit ES1   = 0xAE;
sbit ET2   = 0xAD;
sbit ES0   = 0xAC;
sbit ET1   = 0xAB;
sbit EX1   = 0xAA;
sbit ET0   = 0xA9;
sbit EX0   = 0xA8;

/*  IP  */
sbit PS1   = 0xBE;
sbit PT2   = 0xBD;
sbit PS    = 0xBC;
sbit PS0   = 0xBC;
sbit PT1   = 0xBB;
sbit PX1   = 0xBA;
sbit PT0   = 0xB9;
sbit PX0   = 0xB8;

/*  P1  */
sbit INT5  = 0x97;
sbit INT4  = 0x96;
sbit INT3  = 0x95;
sbit INT2  = 0x94;
sbit TXD1  = 0x93;
sbit RXD1  = 0x92;
sbit T2EX  = 0x91;
sbit T2    = 0x90;

/*  P3  */
sbit RD    = 0xB7;
sbit WR    = 0xB6;
sbit T1    = 0xB5;
sbit T0    = 0xB4;
sbit INT1  = 0xB3;
sbit INT0  = 0xB2;
sbit TXD0  = 0xB1;
sbit RXD0  = 0xB0;

/*  SCON0  */
sbit SM0   = 0x9F;
sbit SM1   = 0x9E;
sbit SM2   = 0x9D;
sbit REN   = 0x9C;
sbit TB8   = 0x9B;
sbit RB8   = 0x9A;
sbit TI    = 0x99;
sbit RI    = 0x98;

/*  SCON1  */
sbit SM01  = 0xC7;
sbit SM11  = 0xC6;
sbit SM21  = 0xC5;
sbit REN1  = 0xC4;
sbit TB81  = 0xC3;
sbit RB81  = 0xC2;
sbit TI1   = 0xC1;
sbit RI1   = 0xC0;

/*  T2CON  */
sbit TF2   = 0xCF;
sbit EXF2  = 0xCE;
sbit RCLK  = 0xCD;
sbit TCLK  = 0xCC;
sbit EXEN2 = 0xCB;
sbit TR2   = 0xCA;
sbit C_T2  = 0xC9;
sbit CP_RL2= 0xC8;

/*  WDCON  */
sbit SMOD  = 0xDF;
sbit POR   = 0xDE;
sbit EPFI  = 0xDD;
sbit PFI   = 0xDC;
sbit WDIF  = 0xDB;
sbit WDRF  = 0xDA;
sbit EWT   = 0xD9;
sbit RWT   = 0xD8;

/*  EIE  */
sbit EWDI  = 0xEC;
sbit EX5   = 0xEB;
sbit EX4   = 0xEA;
sbit EX3   = 0xE9;
sbit EX2   = 0xE8;

/*  EIP  */
sbit PWDI  = 0xEC;
sbit PX5   = 0xEB;
sbit PX4   = 0xEA;
sbit PX3   = 0xE9;
sbit PX2   = 0xE8;



#endif 
// end of REG_W77E58_H

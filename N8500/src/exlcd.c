#include "reg51.h"
char	lcdnum[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D',
                'E','F','G','H','I','J','K','L','M','N'};
char  lcdeng[]={'O','P','Q','R','S','T','U','V','W','X','Y','Z'};             
void 	set_lcd();
void 	cursor_home();
void	put_ir(int cntl_word);
void	put_dr(int data_word);
void 	check_busy();
void	delay(int z);

void main(void)
  {
	int key=0,i;
	set_lcd();
	while(key==0)
	{
             cursor_home();
	     for(i=0;i<26;i++)
	     {
	         put_dr(lcdnum[i]);
	         delay(30000);
	      }
	         // cursor_home();
	         put_ir(0xc0);	   //Begin Display at 40H(Display Data RAM)
	     for(i=0;i<12;i++)
	      {
	         put_dr(lcdeng[i]);
	         delay(30000);
	      }
	 }

}
void set_lcd()
{
	put_ir(0x38);            //Function Set
	put_ir(0x06);            //EntryMode
	put_ir(0x0e);            //Display On/Off
}
void cursor_home()
{
	put_ir(0x01);            //Display Clear
	put_ir(0x02);            //Home
} 
void put_ir(int cntl_word)
{
	check_busy();
	P1=cntl_word;
	P2=0x00;
	P2=0x04;
	delay(100);
	P2=0x00;
}
void put_dr(int data_word)
{
	check_busy();
	P1=data_word;
	P2=0x01;
	P2=0x05;
	delay(100);
	P2=0x01;
}
void check_busy()
{
	int  busy_flag;
	P2=0x02;
	P2=0x06;
	delay(100);
READ_AGAIN:
        busy_flag=P1;
        busy_flag&=0x80;
        if(busy_flag==0x80)
        {
         goto READ_AGAIN;
         }
         else
         {
         P2=0x02;
         }
 }
  
void delay(int z)
{
 	int m;
 	for(m=0;m<z;m++);
}
 
	
	

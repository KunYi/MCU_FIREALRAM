#include "reg51.h"
char	DisplayData1[]={"Welcome to GoodWill!"};
char	DisplayData2[]={"Happy New Year"};
char	DisplayData3[]={"I Am Andy"};
void 	set_lcd();
void 	cursor_home();
void	put_ir(int cntl_word);
void	put_dr(int data_word);
void 	check_busy();
void	delay(int z);
void    DelayX10ms(int count);
void    main(void)
{
 int i,j;
 set_lcd();
 while(1)
  {
  put_ir(0x01); 
  put_ir(0x84);
  for(i=0; i<9; i++)
     {
     put_dr(DisplayData3[i]);
     }
  DelayX10ms(1000);
  for(j=0; j<5; j++)
  {
     put_ir(0x82);
     for(i=0; i<20; i++)
        {
          put_dr(DisplayData1[i]);
        }
     put_ir(0xc4);                  //Begin Display at 44H(Display Data RAM) 
     for(i=0; i<14; i++)
        {
         put_dr(DisplayData2[i]);
        }
     DelayX10ms(400);
     put_ir(0x82);                 //Begin Display at 42H(Display Data RAM) 
     for(i=0;i<20;i++)             
        {
         put_dr(0x20);             //put in space char
        }
     DelayX10ms(400);   
     } //end for(i=0; i<5;i++ )
  } //end while(1)
} // end main()
void set_lcd()
{
	put_ir(0x38);            //Function Set
	put_ir(0x06);            //Entry Mode
	put_ir(0x0c);            //Display On/Off          
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
void DelayX10ms(int count)
{
    int i,j,k;
    
    for(i=0; i<count; i++)
       for(j=0; j<10; j++)
          for(k=0; k<120; k++)
          ;
}

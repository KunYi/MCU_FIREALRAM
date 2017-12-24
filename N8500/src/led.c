#include "reg51.h"
 void  delay(int x); 
 int key=0,i,m,n,ledc[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
 int ledcc[6]={0x40,0x20,0x10,0x08,0x04,0x02};
 main()
 {
         while(key==0)
         {
              
              for(i=0;i<8;i++)
              {
                 P1=0x00;
                 P0=ledc[i];                                                                                                                                                            
                 delay(10000);
               }
               for(i=0;i<6;i++)
               {

                   P1=0x00;
                   P0=ledcc[i];
                   delay(10000);
               }
         }
}
void delay(int x)
{
      for(m=0;m<x;m++);
        for(n=0;n<100;n++);
}



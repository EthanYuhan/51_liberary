
#include "stc15f2k60s2.h"
#include <intrins.h>
#include"lcd12864.h"

/* 
12864液晶实验
*/

unsigned char code CharCode[]="你好123456789abcdefghijk";


//针对 STC15W4K56S4 系列 IO口初始化
//io口初始化 P0 P1 P2 P3 P4等 为准双向IO口   

void IO_init(void)
{
	P0M0 = 0X00;
	P0M1 = 0X00;

	P1M0 = 0X00;
	P1M1 = 0X00;

	P2M0 = 0X00;
	P2M1 = 0X00;

	P3M0 = 0X00;
	P3M1 = 0X00;

	P4M0 = 0X00;
	P4M1 = 0X00;  
	
	P5M0 = 0X00;
	P5M1 = 0X00;  
	
	P6M0 = 0X00;
	P6M1 = 0X00;  
	
	P7M0 = 0X00;
	P7M1 = 0X00;  
}





main()
{	
	  uchar i;
	
	 IO_init();				   //针对 STC15W4K56S4  IO口初始化
	
  //设置STC 单片机的P12 P13 P14为推挽输出  针对12864LCD设置    --P20 P21 22
	P2M1 &= ~(1<<2),P2M0 |=  (1<<2); 
	P2M1 &= ~(1<<1),P2M0 |=  (1<<1); 
	P2M1 &= ~(1<<0),P2M0 |=  (1<<0); 	
	
//	P0 =0xFF ;	
//	LCD12864_RS  =  0;             //
//	LCD12864_RW  =  0;             //
//	LCD12864_EN  =  0;
//	while(1);

	
//	while(1)
//	{
//	LCD12864_EN = !LCD12864_EN;
//	P0 =0x80 ;	
//	LCD12864_Delay1ms(1);
//	P0 =0 ;	
//	LCD12864_Delay1ms(1);		
//	}
//	
	
  LCD12864_Init();
	while(1)
	{
		LCD12864_SetWindow(0, 0);
		while(CharCode[i]!='\0')
		{
			LCD12864_WriteData(CharCode[i]);
			i++;
			if(i==16)  //第一行满写第三行
			{
				LCD12864_SetWindow(1,0);
			}

			if(i==32)  //第三行满写第四行
			{
				LCD12864_SetWindow(2,0);
			}
			if(i==48)  //第三行满写第四行
			{
				LCD12864_SetWindow(3,0);
			}
			
		}
		while(1);	
	}	
	
	
	
}


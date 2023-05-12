
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "24cxx.h"

/*			
	*实际用24C256 待验证程序是否兼容
	*待增加矩阵键盘程序
	
	
			
I2C对24C02读 写操作 
     说明 此次实验主要加了 P1.5 P1.6 IO口模拟I2C总线  
	 
	 

   
*/

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



//主函数
void main(void)
{ 
	u8 pd;
	u8 buf[10] ;

	IO_init(); //IO口初始化	
	LCD_Init();	//液晶屏初始化
	LCD_Clear(WHITE);//清屏白底	
	Gui_StrCenter(0,16,RED,BLUE,"24C02 read and write测试程序",16,1);//居中显示	
	

	pd=AT24C_Rcvone(0);    //选择要读取的地址  然后将读取的数据存入变量pd中
							//1读出1个字节到存入的指定变量 
	sprintf((char *)buf,"AT24C: %c",pd);	
	Show_Str(109,56,BLUE,YELLOW,buf,16,0); 
	
	pd='F';		    //将预存储Count1中的值给变量pd
	AT24C_Sendone(0,pd);	//将pd的值存入  0地址位置 存入量为1字节
	
	
	
	while(1); //防止程序跑飞
	

		
	
}

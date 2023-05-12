
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "ds1302.h"

/*			
*DS1302引脚  
*CE-P5^4  I/O-P3^6  SCLK-P3^5  	

DS1302 时钟芯片测试
     说明 板载了DS1302芯片 使用的引脚为	  
	      
		  关于DS1302的初始化	Set_RTC();  这个函数是初始化DS1302的函数
		  说白了就是DS1302在重上电的时候一定要 重新设置一次时间 否则读取时 将不会读到数据
		  这也是它的一个缺点吧 
		  如果下一次程序 写进时间 然后下次不想改时间 那么就要把	Set_RTC();屏蔽了 并保持DS1302持续供电
		  在后面的综合程序里 我们将用一种读初步设置方法 让其设置一次 以后就不用设置了 这个后面在介绍吧

		  DS1302的具体工作原理  大家就请结合原理图 和DS1302中文手册 来看  这里不多做介绍

   
*/

void IO_init(void)
{
	P0M0 = 0X00;	P0M1 = 0X00;

	P1M0 = 0X00;  P1M1 = 0X00;

	P2M0 = 0X00;	P2M1 = 0X00;

	P3M0 = 0X00;	P3M1 = 0X00;

	P4M0 = 0X00;	P4M1 = 0X00;  
	
	P5M0 = 0X00;	P5M1 = 0X00;  
	
	P6M0 = 0X00;	P6M1 = 0X00;  
	
	P7M0 = 0X00;	P7M1 = 0X00; 	
}



//主函数
void main(void)
{ 

	IO_init(); //IO口初始化	
	LCD_Init();	//液晶屏初始化
	LCD_Clear(WHITE);//清屏白底	
	Gui_StrCenter(0,0,RED,BLUE,"DS1302 TIME测试程序",16,1);//居中显示	

	Set_RTC();				   //DS1302时钟初始化   
	while(1)
	{
	 ds1302_scan(50,60,RED,WHITE);		//更新显示
    }					 
	
	
}






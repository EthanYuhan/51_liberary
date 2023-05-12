
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include <stdio.h>
#include "string.h"
#include  "sd.h"
#include  "spi.h"
#include  "pff.h"				  //文件系统调用.h加载
#include  "sram.h"
#include  "ov7670.h"


/*			
*引脚和原理图一直  程序可直接验证 


				   //LCD_RESET 改成 P3^3 ！！！！！！！！ 
				   //LCD_RESET 直接接3.3V VCC
				   //P1口为仅输入状态,要减少P1口的干扰，问题解决
					 
					 
					 //移植于历史版本 测试！！！
	  	                                           		    	  
*/





FATFS fatfs;	//文件系统结构体定义
u8 tbuf[512];			   //512字节 SD卡数据缓存空间


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
	SP=0X80;				    //调整堆栈指向  

	IO_init(); 					//IO口初始化	
	LCD_Init();					//液晶屏初始化
	Init_SPI(); 				//SPI初始化 
	
	LCD_Clear(WHITE);   //清屏白底		

  SD_Init();			     //SD卡初始化
  pf_mount(&fatfs);	   //初始化petit FATFS文件系统  并提取tf卡相应数据
											 //这句非常重要，它是使用所有Petit Fatfs文件系统功能的前提

	
	get_font_sector();		  //提取TF卡中  GBK点阵码  首扇区
													//利用tf字库进行汉字显示

  ov7670_test();			  //ov7670测试函数
  while(1);
	

	
	
}







/*			
*引脚和原理图一直  程序可直接验证 
//现在128M tf卡读到的容量是120，待换新的小容量tf卡验证
//number函数功能可通过配合sprintf函数实现，建议下次验证时去掉这个函数
 	

TF卡初始化 
     说明 主要初始化TF卡  判断卡类型 是容量还是大容量	小容量SD 0-2g  大容量 SDHC 2-32g
	      读取卡的容量 并将其显示
		  整个操作都在sd.c底层里已经写好  如果有不懂的地方请仔细阅读资料
   
*/


#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "sd.h"
#include  "spi.h"

extern u8 SD_Type;			  //SD卡类型

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
	u32 sd_size; //临时变量

	IO_init(); //IO口初始化	
	LCD_Init();	//液晶屏初始化
	Init_SPI(); //SPI初始化  
	LCD_Clear(WHITE);//清屏白底	
	
	Show_Str(0,0,BLACK,WHITE,"TF Card Init!",16,0); 
	Show_Str(0,40,BLUE,WHITE,"TF Card Type:",16,0); //卡类型
	Show_Str(0,60,BLUE,WHITE,"TF Card Size:      Mb",16,0); //卡容量
	

	if(SD_Init()==0) Show_Str(0,20,BLACK,WHITE,"TF Card  ok",16,0);	  //初始化TF卡 
	 else Show_Str(0,20,RED,WHITE,"TF Card Error",16,0);

	 sd_size=SD_GetCapacity();	  //计算SD卡容量

	 if(SD_Type==0x04) Show_Str(104,40,BLUE,WHITE,"SD_TYPE_V2",16,0);	   //0-2g
	 if(SD_Type==0x06) Show_Str(104,40,BLUE,WHITE,"SD_TYPE_V2HC",16,0);  //2-32g

	 number(104,60,sd_size>>20,RED,WHITE);   //显示卡容量  1M=1024KB  1KB=1024B  1B=8bit	   2的20次方为1024x1024 也就是M
	 
	
	 while(1);  					 
	
	
}






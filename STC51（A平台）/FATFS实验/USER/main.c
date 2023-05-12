
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "sd.h"
#include  "spi.h"
#include  "pff.h"				  //文件系统调用.h加载


/*			

		 
		  
*/


FATFS fatfs;	//文件系统结构体定义
u8 tbuf[512];			   //512字节 SD卡数据缓存空间

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
	FRESULT res;
	u8 buf[40];

	IO_init(); //IO口初始化	
	LCD_Init();	//液晶屏初始化
	Init_SPI(); //SPI初始化 
	LCD_Clear(WHITE);//清屏白底		

    SD_Init();			      //SD卡初始化
    //pf_mount(&fatfs);	         //初始化petit FATFS文件系统  并提取tf卡相应数据
	
	
	res=pf_mount(&fatfs);	
	

	
if(res == FR_OK) Show_Str(0, 146, BLACK,WHITE, "FAFS  ok",16,1);
else if(res == FR_DISK_ERR) Show_Str(0, 146, RED,WHITE, "FR_DISK_ERR  err",16,1);
else if(res == FR_NOT_READY)Show_Str(0, 146, RED,WHITE, "FR_NOT_READY  err",16,1);
else if(res == FR_NO_FILE) Show_Str(0, 146, RED,WHITE, "FR_NO_FILE  err",16,1);
else if(res == FR_NOT_OPENED) Show_Str(0, 146, RED,WHITE, "FR_NOT_OPENED  err",16,1);
else if(res == FR_NOT_ENABLED) Show_Str(0, 146, RED,WHITE, "FR_NOT_ENABLED  err",16,1);
else if(res == FR_NO_FILESYSTEM) Show_Str(0, 146, RED,WHITE, "FR_NO_FILESYSTEM  err",16,1);


	while(1);			 
	
	
}






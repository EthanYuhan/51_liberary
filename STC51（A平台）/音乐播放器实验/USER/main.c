
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
#include  "vs1053.h"
#include  "mp3player.h"


/*			
音乐播放器实验
详见README文档说明		  	                                           		    	  
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
 u16 br;
 u32 cnt=0;					  //音乐进度递增变量
 u8 mp3in;
	
	SP=0X80;				      //调整堆栈指向   手册286页 详解

	IO_init(); //IO口初始化	
	LCD_Init();	//液晶屏初始化
	Init_SPI(); //SPI初始化 
	LCD_Clear(WHITE);//清屏白底		

  SD_Init();			      //SD卡初始化
  pf_mount(&fatfs);	         //初始化petit FATFS文件系统  并提取tf卡相应数据
								 ////这句非常重要，它是使用所有Petit Fatfs文件系统功能的前提

	
  GUI_sprintf_hzstr16x(0,0,"VS1053--TEST",BLACK,WHITE);

  mp3in=Mp3Player_Init(); 
  if(mp3in==1)GUI_sprintf_hzstr16x(0,30,"MP3 Init OK",RED,WHITE);
  else GUI_sprintf_hzstr16x(0,30,"MP3 Init Error",RED,WHITE);

  res=pf_open("/MUSIC/这条街.mp3");			//打开指定路径下的音乐文件名

  if(res == FR_OK)GUI_sprintf_hzstr16x(0,80,"The music is already open!",BLUE,WHITE);


  VS_Restart_Play();  					    // 重启播放 
  VS_Set_All();        					    // 设置音量等信息 			 
  VS_Reset_DecodeTime();					// 复位解码时间 
  VS_Set_Vol(220);                          // 设置音量
  VS_SPI_SpeedHigh();	                    // SPI设置为高速

 // SetBands(); 							// 频谱补丁初始化

  GUI_sprintf_hzstr16x(0,110,"Play music...",BLUE,WHITE);
	
	
  while(1)
   {
	res=pf_read(tbuf,512,&br);				//通过文件系统读取指定文件夹下的一音乐数据

	if((res!=FR_OK))
	{
	 // led=0;
		while(1);
	}
     cnt=0;

	 do{
	   	if(VS_Send_MusicData(tbuf+cnt)==0)	  //一次送32个字节音乐数据
		{
		cnt+=32;	
		}
		else {
		//led=0;
		}

	   }while(cnt<512);

   }

    if(br!=512)                              // 文件结束
    {
      while(1);
    }	 
	
	
}






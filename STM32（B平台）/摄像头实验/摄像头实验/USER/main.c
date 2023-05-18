#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "sram.h"
#include "malloc.h"
#include "string.h"
#include "text.h"
#include "usmart.h"
#include "ov7670.h"
#include "timer.h"
#include "exti.h"


//--------------------------
//摄像头实验  
//详见README文档说明
//-------------------------- 
 
 
 
extern u8 ov_sta;	//在exit.c里面定义
extern u8 ov_frame;	//在timer.c里面定义	   
//更新LCD显示
void camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta==2)
	{
		LCD_Scan_Dir(L2R_U2D);		//L2R_U2D
		LCD_SetCursor(0x00,0x0000);	//设置光标位置 				
		LCD_WriteRAM_Prepare();     //开始写入GRAM	
		OV7670_RRST=0;				//开始复位读指针 
		OV7670_RCK=0;
		OV7670_RCK=1;
		OV7670_RCK=0;
		OV7670_RRST=1;				//复位读指针结束 
		OV7670_RCK=1;  
		for(j=0;j<76800;j++)
		{
			OV7670_RCK=0;
			color=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK=1; 
			color<<=8;  
			OV7670_RCK=0;
			color|=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK=1; 
			LCD->LCD_RAM=color;    
		}   							 
		EXTI_ClearITPendingBit(EXTI_Line8);  //清除EXTI8线路挂起位	
		ov_sta=0;					//开始下一次采集
 		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//恢复默认扫描方向 
	} 
}


	 
 int main(void)
 {	 
					 
	u8 *pname;			//带路径的文件名 
	u8 lightmode=0,saturation=2,brightness=2,contrast=2;  //
	u8 effect=0;

 
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 	//串口初始化为9600

	LCD_Init();	
	if(lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X9488)	//强制设置屏幕分辨率为320*240.以支持3.5/4.3寸大屏
	{
		lcddev.width=240;
		lcddev.height=320; 
	}


 	mem_init(SRAMIN);	//初始化内部内存池	
	usmart_dev.init(72);//usmart初始化	
 	//exfuns_init();					//为fatfs相关变量申请内存  


	//piclib_init();					//初始化画图
		 
	POINT_COLOR=RED;      
 	 
 	Show_Str(60,50,200,16,"STM32",16,0);				    	 
	Show_Str(60,70,200,16,"CAMERA",16,0);				    	 		    	 
	Show_Str(60,90,200,16,"20201114",16,0);
									   						    
 	pname=mymalloc(SRAMIN,30);	//为带路径的文件名分配30个字节的内存		    
 	while(pname==NULL)			//内存分配出错
 	{	    
		Show_Str(60,190,240,16,"SRAM Fail!",16,0);
		delay_ms(200);				  
		LCD_Fill(60,190,240,146,WHITE);//清除显示	     
		delay_ms(200);				  
	}   											  
	while(OV7670_Init())//初始化OV7670
	{
		Show_Str(60,190,240,16,"OV7670 err!",16,0);
		delay_ms(200);
	    LCD_Fill(60,190,239,186,WHITE);
		delay_ms(200);
	}
	
 	Show_Str(60,190,200,16,"OV7670  ok!",16,0);
	delay_ms(1500);	 

	OV7670_Light_Mode(lightmode);
	OV7670_Color_Saturation(saturation);
	OV7670_Brightness(brightness);
	OV7670_Contrast(contrast);
 	OV7670_Special_Effects(effect);	
				  
	EXTI8_Init();										//使能定时器捕获
	OV7670_Window_Set(10,174,240,320);	//设置窗口	  
  OV7670_CS=0;		
	
 	while(1)
	{		
	
 		camera_refresh();//更新显示
		
		
	}	   										    			    
}





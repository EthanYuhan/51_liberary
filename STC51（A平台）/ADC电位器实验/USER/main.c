
#include "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include <stdio.h>
#include "string.h"
#include "ad.h"


/*
ADC电位器实验
详见README文档
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
	u16 ad;
	u8 buf[10] ;
	
	IO_init(); //IO口初始化

	//液晶屏初始化
	LCD_Init();	
	LCD_Clear(WHITE);//清屏白底	
	Gui_StrCenter(0,16,RED,BLUE,"ADC 测试程序",16,1);//居中显示	
	Show_Str(20,56,BLUE,YELLOW,"Voltage",16,0);
	Show_Str(101,56,BLUE,YELLOW,": .  V",16,0);
	
	
	//循环
	while(1)
	{
		
	delay_ms(500);
	
	ad=GetADCResult(0);			 //读取电压数量值  P1.0口  采集AD电压值
		
    ad=(float)ad*3.3*100/1024;      //3.3v！！！
								//5/1024 5v电压份1024份 值为每一份的值 ad为采集的份数  与其相乘为采集的电压值 *100小数点后两位 
                                 //10位AD采集 即2的10次方 满值为1024 这里用1024表示5伏的电压 
					             //那么用采集到的数量值 除以1024 在乘以5 得到的值就是采集的电压数值

	//以 3.3v 电压为基准，计算采集值的电压并将其显示出来 ！！！
		
	sprintf((char *)buf,"%d",(ad/100%10));		
    Show_Str(109,56,BLUE,YELLOW,buf,16,0);  //显示电压值
	sprintf((char *)buf,"%d",(ad/10%10));	
	Show_Str(125,56,BLUE,YELLOW,buf,16,0);  //显示电压值
	sprintf((char *)buf,"%d",(ad%10));	
	Show_Str(133,56,BLUE,YELLOW,buf,16,0);  //显示电压值
		
		
	delay_ms(500);			
		
		
	}   
}

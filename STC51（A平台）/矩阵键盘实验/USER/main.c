
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include "uart.h"
#include <intrins.h>


u8	KeyCode;	//给用户使用的键码, 17~32有效


u8 IO_KeyState, IO_KeyState1, IO_KeyHoldCnt;	//行列键盘变量


/*****************************************************
	行列键扫描程序
	使用XY查找4x4键的方法，只能单键，速度快

   Y     P74      P75      P76      P77
          |        |        |        |
X         |        |        |        |
P70 ---- K00 ---- K01 ---- K02 ---- K03 ----
          |        |        |        |
P71 ---- K04 ---- K05 ---- K06 ---- K07 ----
          |        |        |        |
P72 ---- K08 ---- K09 ---- K10 ---- K11 ----
          |        |        |        |
P73 ---- K12 ---- K13 ---- K14 ---- K15 ----
          |        |        |        |
******************************************************/


u8 code T_KeyTable[16] = {0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0};

void IO_KeyDelay(void)
{
	u8 i;
	i = 60;
	while(--i)	;
}

void	IO_KeyScan(void)	
{
	u8	j;

	j = IO_KeyState1;	//保存上一次状态

	P7 = 0xf0;	//X低，读Y
	IO_KeyDelay();
	IO_KeyState1 = P7 & 0xf0;

	P7 = 0x0f;	//Y低，读X
	IO_KeyDelay();
	
	IO_KeyState1 |= (P7 & 0x0f);
	
	IO_KeyState1 ^= 0xff;	//取反
	
	if(j == IO_KeyState1)	//连续两次读相等
	{
		j = IO_KeyState;
		IO_KeyState = IO_KeyState1;
		if(IO_KeyState != 0)	//有键按下
		{
			F0 = 0;
			if(j == 0)	F0 = 1;	//第一次按下
			else if(j == IO_KeyState)
			{
				if(++IO_KeyHoldCnt >= 20)	//1秒后重键
				{
					IO_KeyHoldCnt = 18;
					F0 = 1;
				}
			}
			if(F0)
			{
				j = T_KeyTable[IO_KeyState >> 4];
				if((j != 0) && (T_KeyTable[IO_KeyState& 0x0f] != 0)) 
				{
					KeyCode = (u8)(((j - 1) * 4 )+ (T_KeyTable[IO_KeyState & 0x0f]) );	//计算键码，17~32  + 16
				}
					
			}
		}
		else	IO_KeyHoldCnt = 0;
	}
	P7 = 0xff;
}






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
	
	u8 buf[40];
	
	IO_init(); //IO口初始化		
	LCD_Init();//液晶屏初始化
	delay1ms(10);	
	LCD_Clear(WHITE);//清屏白底	
	
	UartInit();                 //串口初始化	
	UartSendStr("按键测试\r\n");
	
	
	KeyCode = 0;	//给用户使用的键码, 1~16有效
	IO_KeyState = 0;
	IO_KeyState1 = 0;
	IO_KeyHoldCnt = 0;

	Gui_StrCenter(0,16,RED,BLUE,"KEY 测试程序",16,1);//居中显示	
	Show_Str(20,56,BLUE,YELLOW,"KeyCode:",16,0);

	

	//循环进行各项测试	
	while(1)
	{
		IO_KeyScan();
		
		if(KeyCode>0)
		{
			
		UartSendStr("pressed");		
						
		LCD_ShowNum(100,56,KeyCode,2,16);
			
		}
		
		if(KeyCode == 1)	
		{
		UartSendStr("1");
		}
		if(KeyCode == 2)	
		{
		UartSendStr("2");
		}
		if(KeyCode == 3)	
		{
		UartSendStr("3");
		}
		if(KeyCode == 4)	
		{
		UartSendStr("4");
		}
		
		KeyCode=0;

		delay1ms(100);
		


	}   
}

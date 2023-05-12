
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include "uart.h"
#include <intrins.h>


u8	KeyCode;	//���û�ʹ�õļ���, 17~32��Ч


u8 IO_KeyState, IO_KeyState1, IO_KeyHoldCnt;	//���м��̱���


/*****************************************************
	���м�ɨ�����
	ʹ��XY����4x4���ķ�����ֻ�ܵ������ٶȿ�

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

	j = IO_KeyState1;	//������һ��״̬

	P7 = 0xf0;	//X�ͣ���Y
	IO_KeyDelay();
	IO_KeyState1 = P7 & 0xf0;

	P7 = 0x0f;	//Y�ͣ���X
	IO_KeyDelay();
	
	IO_KeyState1 |= (P7 & 0x0f);
	
	IO_KeyState1 ^= 0xff;	//ȡ��
	
	if(j == IO_KeyState1)	//�������ζ����
	{
		j = IO_KeyState;
		IO_KeyState = IO_KeyState1;
		if(IO_KeyState != 0)	//�м�����
		{
			F0 = 0;
			if(j == 0)	F0 = 1;	//��һ�ΰ���
			else if(j == IO_KeyState)
			{
				if(++IO_KeyHoldCnt >= 20)	//1����ؼ�
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
					KeyCode = (u8)(((j - 1) * 4 )+ (T_KeyTable[IO_KeyState & 0x0f]) );	//������룬17~32  + 16
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






//������
void main(void)
{ 
	
	u8 buf[40];
	
	IO_init(); //IO�ڳ�ʼ��		
	LCD_Init();//Һ������ʼ��
	delay1ms(10);	
	LCD_Clear(WHITE);//�����׵�	
	
	UartInit();                 //���ڳ�ʼ��	
	UartSendStr("��������\r\n");
	
	
	KeyCode = 0;	//���û�ʹ�õļ���, 1~16��Ч
	IO_KeyState = 0;
	IO_KeyState1 = 0;
	IO_KeyHoldCnt = 0;

	Gui_StrCenter(0,16,RED,BLUE,"KEY ���Գ���",16,1);//������ʾ	
	Show_Str(20,56,BLUE,YELLOW,"KeyCode:",16,0);

	

	//ѭ�����и������	
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

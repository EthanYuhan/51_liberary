#include "key.h"
#include "sys.h"


u8 KeyCode=0;	//给用户使用的键码, 17~32有效
u8 IO_KeyState=0;
u8 IO_KeyState1=0;
u8 IO_KeyHoldCnt=0;	//行列键盘变量
u8 code T_KeyTable[16] = {0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0};



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




#include "key.h"
#include "sys.h"


u8 KeyCode=0;	//���û�ʹ�õļ���, 17~32��Ч
u8 IO_KeyState=0;
u8 IO_KeyState1=0;
u8 IO_KeyHoldCnt=0;	//���м��̱���
u8 code T_KeyTable[16] = {0,1,2,0,3,0,0,0,4,0,0,0,0,0,0,0};



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




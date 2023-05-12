
#include	"config.h"
#include	"PWM.h"
#include  "sys.h"

/***************************************
PWM����ʵ��
���README�ĵ�
******************************************/




//========================================================================
// ����: void	PWM_config(void)
// ����: PWM���ú�����
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2014-8-15
// ��ע: 
//========================================================================
void	PWM_config(void)
{
	PWMx_InitDefine		PWMx_InitStructure;					//�ṹ����
	
//	PWMx_InitStructure.PWMx_IO_Select       = PWM2_P37;		//PWM���IOѡ��. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//����PWM����˿ڵĳ�ʼ��ƽ, 0��1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//�ж�����,   			ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//��һ����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//�ڶ�����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//��һ����ת����, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//�ڶ�����ת����, 1~32767
//	PWMx_Configuration(PWM2_ID, &PWMx_InitStructure);		//��ʼ��PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P3n_standard(1<<7);										//IO��ʼ��, �ϵ�ʱΪ����

//	PWMx_InitStructure.PWMx_IO_Select       = PWM3_P21;		//PWM���IOѡ��. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//����PWM����˿ڵĳ�ʼ��ƽ, 0��1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//�ж�����,   			ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//��һ����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//�ڶ�����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//��һ����ת����, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//�ڶ�����ת����, 1~32767
//	PWMx_Configuration(PWM3_ID, &PWMx_InitStructure);		//��ʼ��PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P2n_standard(1<<1);										//IO��ʼ��, �ϵ�ʱΪ����

//	PWMx_InitStructure.PWMx_IO_Select       = PWM4_P22;		//PWM���IOѡ��. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//����PWM����˿ڵĳ�ʼ��ƽ, 0��1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//�ж�����,   			ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//��һ����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//�ڶ�����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//��һ����ת����, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//�ڶ�����ת����, 1~32767
//	PWMx_Configuration(PWM4_ID, &PWMx_InitStructure);		//��ʼ��PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P2n_standard(1<<2);										//IO��ʼ��, �ϵ�ʱΪ����

//	PWMx_InitStructure.PWMx_IO_Select       = PWM5_P23;		//PWM���IOѡ��. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//����PWM����˿ڵĳ�ʼ��ƽ, 0��1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//�ж�����,   			ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//��һ����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//�ڶ�����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//��һ����ת����, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//�ڶ�����ת����, 1~32767
//	PWMx_Configuration(PWM5_ID, &PWMx_InitStructure);		//��ʼ��PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P2n_standard(1<<3);										//IO��ʼ��, �ϵ�ʱΪ����

//	PWMx_InitStructure.PWMx_IO_Select       = PWM6_P07;		//PWM���IOѡ��. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//����PWM����˿ڵĳ�ʼ��ƽ, 0��1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//�ж�����,   			ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//��һ����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//�ڶ�����ת�ж�����,   ENABLE��DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//��һ����ת����, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//�ڶ�����ת����, 1~32767
//	PWMx_Configuration(PWM6_ID, &PWMx_InitStructure);		//��ʼ��PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P0n_standard(1<<7);										//IO��ʼ��, �ϵ�ʱΪ����

	PWMx_InitStructure.PWMx_IO_Select       = PWM7_P17;		//PWM���IOѡ��. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
	PWMx_InitStructure.Start_IO_Level       = 0;			//����PWM����˿ڵĳ�ʼ��ƽ, 0��1
	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//�ж�����,   			ENABLE��DISABLE
	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//��һ����ת�ж�����,   ENABLE��DISABLE
	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//�ڶ�����ת�ж�����,   ENABLE��DISABLE
	PWMx_InitStructure.FirstEdge            = 64;			//��һ����ת����, 1~32767
	PWMx_InitStructure.SecondEdge           = 192;			//�ڶ�����ת����, 1~32767
	PWMx_Configuration(PWM7_ID, &PWMx_InitStructure);		//��ʼ��PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
	P1n_standard(1<<7);										//IO��ʼ��, �ϵ�ʱΪ����

	TH2 = (u8)((65536UL - MAIN_Fosc / 500000UL) / 256);	//������ʹ��Timer2
	TL2 = (u8)((65536UL - MAIN_Fosc / 500000UL) % 256);
	AUXR = 0x14;		//Timer2 set As Timer, Timer2 set as 1T mode;
	PWM_SourceClk_Duty(PwmClk_T2, 256);	//ʱ��Դ: PwmClk_1T,PwmClk_2T, ... PwmClk_16T, PwmClk_T2,   PWM����: 1~32767
	
	PWMCR |= ENPWM;		// ʹ��PWM���η�������PWM��������ʼ����
	PWMCR &= ~ECBI;		//��ֹPWM�����������ж�
//	PWMCR |=  ECBI;		//����PWM�����������ж�

//	PWMFDCR = ENFD | FLTFLIO | FDIO;	//PWMʧЧ�жϿ���,  ENFD | FLTFLIO | EFDI | FDCMP | FDIO
}


/**********************************************/
void main(void)
{
	u8 led0pwmval=0;
	u8  dir=1;	
	
	PWM_config();
	
	PWMx_SetPwmWide(PWM7_ID, 190, 192);  //PWM7 ��P17���Ƹ���LED����
	
	while(1)
	{
		delay_ms(50);	 
		if(dir)led0pwmval++;
		else led0pwmval--;

 		if(led0pwmval>191)dir=0;
		if(led0pwmval==0)dir=1;	
	
	  PWMx_SetPwmWide(PWM7_ID, led0pwmval, 192);  //PWM7 ��P17���Ƹ���LED����
	
	}

}




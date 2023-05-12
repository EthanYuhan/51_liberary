
#include	"config.h"
#include	"PWM.h"
#include  "sys.h"

/***************************************
PWM调光实验
详见README文档
******************************************/




//========================================================================
// 函数: void	PWM_config(void)
// 描述: PWM配置函数。
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-8-15
// 备注: 
//========================================================================
void	PWM_config(void)
{
	PWMx_InitDefine		PWMx_InitStructure;					//结构定义
	
//	PWMx_InitStructure.PWMx_IO_Select       = PWM2_P37;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
//	PWMx_Configuration(PWM2_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P3n_standard(1<<7);										//IO初始化, 上电时为高阻

//	PWMx_InitStructure.PWMx_IO_Select       = PWM3_P21;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
//	PWMx_Configuration(PWM3_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P2n_standard(1<<1);										//IO初始化, 上电时为高阻

//	PWMx_InitStructure.PWMx_IO_Select       = PWM4_P22;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
//	PWMx_Configuration(PWM4_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P2n_standard(1<<2);										//IO初始化, 上电时为高阻

//	PWMx_InitStructure.PWMx_IO_Select       = PWM5_P23;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
//	PWMx_Configuration(PWM5_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P2n_standard(1<<3);										//IO初始化, 上电时为高阻

//	PWMx_InitStructure.PWMx_IO_Select       = PWM6_P07;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
//	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
//	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
//	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
//	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
//	PWMx_Configuration(PWM6_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
//	P0n_standard(1<<7);										//IO初始化, 上电时为高阻

	PWMx_InitStructure.PWMx_IO_Select       = PWM7_P17;		//PWM输出IO选择. PWM2_P37,PWM2_P27,PWM3_P21,PWM3_P45,PWM4_P22,PWM4_P44,PWM5_P23,PWM5_P42,PWM6_P16,PWM6_P07,PWM7_P17,PWM7_P06
	PWMx_InitStructure.Start_IO_Level       = 0;			//设置PWM输出端口的初始电平, 0或1
	PWMx_InitStructure.PWMx_Interrupt       = DISABLE;		//中断允许,   			ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge_Interrupt  = DISABLE;		//第一个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.SecondEdge_Interrupt = DISABLE;		//第二个翻转中断允许,   ENABLE或DISABLE
	PWMx_InitStructure.FirstEdge            = 64;			//第一个翻转计数, 1~32767
	PWMx_InitStructure.SecondEdge           = 192;			//第二个翻转计数, 1~32767
	PWMx_Configuration(PWM7_ID, &PWMx_InitStructure);		//初始化PWM, 	PWM2_ID,PWM3_ID,PWM4_ID,PWM5_ID,PWM6_ID,PWM7_ID
	P1n_standard(1<<7);										//IO初始化, 上电时为高阻

	TH2 = (u8)((65536UL - MAIN_Fosc / 500000UL) / 256);	//波特率使用Timer2
	TL2 = (u8)((65536UL - MAIN_Fosc / 500000UL) % 256);
	AUXR = 0x14;		//Timer2 set As Timer, Timer2 set as 1T mode;
	PWM_SourceClk_Duty(PwmClk_T2, 256);	//时钟源: PwmClk_1T,PwmClk_2T, ... PwmClk_16T, PwmClk_T2,   PWM周期: 1~32767
	
	PWMCR |= ENPWM;		// 使能PWM波形发生器，PWM计数器开始计数
	PWMCR &= ~ECBI;		//禁止PWM计数器归零中断
//	PWMCR |=  ECBI;		//允许PWM计数器归零中断

//	PWMFDCR = ENFD | FLTFLIO | FDIO;	//PWM失效中断控制,  ENFD | FLTFLIO | EFDI | FDCMP | FDIO
}


/**********************************************/
void main(void)
{
	u8 led0pwmval=0;
	u8  dir=1;	
	
	PWM_config();
	
	PWMx_SetPwmWide(PWM7_ID, 190, 192);  //PWM7 的P17控制高亮LED亮度
	
	while(1)
	{
		delay_ms(50);	 
		if(dir)led0pwmval++;
		else led0pwmval--;

 		if(led0pwmval>191)dir=0;
		if(led0pwmval==0)dir=1;	
	
	  PWMx_SetPwmWide(PWM7_ID, led0pwmval, 192);  //PWM7 的P17控制高亮LED亮度
	
	}

}




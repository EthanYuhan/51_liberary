#include "stm32f10x.h"
#include "infrared.h"
#include "sys.h" 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	
//红外对管测速驱动代码	   								  
//////////////////////////////////////////////////////////////////////////////////  
								    
//按键初始化函数  PB13!!
void INFRARED_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);//使能PORTB 时钟



	//初始化 红外对管-->GPIOB.13   上拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //上拉输入//GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.13 

}


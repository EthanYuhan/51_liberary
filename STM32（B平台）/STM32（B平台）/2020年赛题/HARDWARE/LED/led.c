#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 

//LED驱动代码	   
								  
////////////////////////////////////////////////////////////////////////////////// 

//初始化PA15 输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PA端口时钟	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	//PA15	底板右上角独立LED
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//PA15 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA

	

}
 

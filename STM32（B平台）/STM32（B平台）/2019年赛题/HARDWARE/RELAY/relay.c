#include "relay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//继电器驱动代码								  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PD7为输出口.并使能这个口的时钟		    
//继电器初始化
void RELAY_Init(void)
{
	 
 GPIO_InitTypeDef  GPIO_InitStructure; 
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //使能GPIOD端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PD7端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);	 //根据参数初始化GPIOD.7
	

 
 GPIO_SetBits(GPIOD,GPIO_Pin_7);//输出1，关闭继电器输出

}

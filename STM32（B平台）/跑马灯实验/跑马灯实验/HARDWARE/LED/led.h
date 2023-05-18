#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//LED驱动代码	   
									  
////////////////////////////////////////////////////////////////////////////////// 
//PD14 15  PD0 1   PE789 10
#define LED0 PDout(14)// 
#define LED1 PDout(15)// 
#define LED2 PDout(0)// 
#define LED3 PDout(1)// 
#define LED4 PEout(7)// 
#define LED5 PEout(8)// 
#define LED6 PEout(9)// 
#define LED7 PEout(10)// 

void LED_Init(void);//初始化

		 				    
#endif

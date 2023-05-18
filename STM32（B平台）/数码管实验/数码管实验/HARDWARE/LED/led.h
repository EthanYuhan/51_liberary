#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

//定义数码管数据端口  
#define DX0 PDout(14)// 
#define DX1 PDout(15)// 
#define DX2 PDout(0)// 
#define DX3 PDout(1)// 
#define DX4 PEout(7)// 
#define DX5 PEout(8)// 
#define DX6 PEout(9)// 
#define DX7 PEout(10)// 


#define LSA PEout(11)     //控制脚
#define LSB PEout(12)     //控制脚
#define LSC PEout(13)     //控制脚



void LED_Init(void);
void Display(u8 index);


		 				    
#endif

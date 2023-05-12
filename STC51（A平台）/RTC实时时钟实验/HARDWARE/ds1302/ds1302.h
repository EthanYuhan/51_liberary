

#ifndef  __DS1302_H
#define  __DS1302_H
#include "sys.h"

sbit SCK=P3^5;	//时钟	
sbit SDA=P3^6;	//数据	
sbit CE=P5^4;// DS1302复位


void Set_RTC(void); //set RTC  初始化
void ds1302_scan(u8 x,u16 y,u16 dcolor,u16 bgcolor);  //时间显示


#endif





#ifndef  __DS1302_H
#define  __DS1302_H
#include "sys.h"

sbit SCK=P3^5;	//时钟	
sbit SDA=P3^6;	//数据	
sbit CE=P5^4;	//复位


//void Set_RTC(void);    //set RTC 
void ds1302_scan(void);  //时间显示

void Write_Ds1302(u8 address,u8 dat);
u8 bcd_read(u8 add);
u8 Read_Ds1302 ( u8 address );
void Set_RTC();

#endif





#ifndef  __DS1302_H
#define  __DS1302_H
#include "sys.h"

sbit SCK=P4^5;	//ʱ��	
sbit SDA=P3^4;	//����	
sbit CE=P1^2;// DS1302��λ


void Set_RTC(void); //set RTC  ��ʼ��
void ds1302_scan(u8 x,u16 y,u16 dcolor,u16 bgcolor);  //ʱ����ʾ


#endif



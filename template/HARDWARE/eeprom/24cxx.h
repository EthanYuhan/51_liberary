



#ifndef Z24C02_H
#define Z24C02_H
#include "sys.h"

//void c02();										  //24c测试函数
void AT24C_Sendone(u16 Addr,u8 Data);	          //写一个字节
u8 AT24C_Rcvone(u16 Addr);						  //读一个字节
void AT24C_SendLenByte(u16 Addr,u8 *Data,u8 Len);  //写多个字节
void AT24C_RcvLenByte(u16 Addr,u8 *temp,u8 Len);	  //读多个字节


#endif






#ifndef Z24C02_H
#define Z24C02_H
#include "sys.h"

//void c02();										  //24c���Ժ���
void AT24C_Sendone(u16 Addr,u8 Data);	          //дһ���ֽ�
u8 AT24C_Rcvone(u16 Addr);						  //��һ���ֽ�
void AT24C_SendLenByte(u16 Addr,u8 *Data,u8 Len);  //д����ֽ�
void AT24C_RcvLenByte(u16 Addr,u8 *temp,u8 Len);	  //������ֽ�


#endif


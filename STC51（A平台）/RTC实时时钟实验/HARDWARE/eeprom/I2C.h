

#ifndef I2C_H
#define I2C_H
#include "sys.h"

void Start_I2c();		  //��������
void Stop_I2c();		  //ֹͣ����
void SendByte(u8 c);	  //�ֽ����ݷ��ͺ���
u8  RcvByte();			  //�ֽ����ݽ��պ��� 
void Ack_I2c(bit a);	    

u8 I2c_wait_ack(void);	  //�ȴ�Ӧ���źŵ���


#endif

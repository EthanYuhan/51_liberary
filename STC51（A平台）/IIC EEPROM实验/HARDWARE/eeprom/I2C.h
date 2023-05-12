

#ifndef I2C_H
#define I2C_H
#include "sys.h"

void Start_I2c();		  //启动总线
void Stop_I2c();		  //停止总线
void SendByte(u8 c);	  //字节数据发送函数
u8  RcvByte();			  //字节数据接收函数 
void Ack_I2c(bit a);	    

u8 I2c_wait_ack(void);	  //等待应答信号到来


#endif

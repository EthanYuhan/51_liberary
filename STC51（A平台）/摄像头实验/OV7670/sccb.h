#ifndef _SCCB_H_
#define _SCCB_H_
#include "sys.h"


sbit SCCB_SIC=P1^0;		  //ʱ��
sbit SCCB_SID=P1^1;		  //����


#define DELAYTIME 200

void startSCCB(void);
void stopSCCB(void);
void noAck(void);
unsigned char getAck();
unsigned char SCCBwriteByte(unsigned char dat);
unsigned char SCCBreadByte(void);

#endif



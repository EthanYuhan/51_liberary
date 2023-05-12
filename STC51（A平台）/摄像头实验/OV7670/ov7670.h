#ifndef _OV7670_H_
#define _OV7670_H_
#include "sys.h"


#define OV7670_REG_NUM  114

 //===========FIFO PIN============
sbit FIFO_WEN =P1^7;		   //д��FIFOʹ��
sbit FIFO_RCLK=P1^6;		   //������ʱ��
sbit FIFO_WRST=P1^5;		   //дָ�븴λ
sbit FIFO_RRST=P1^3;		   //��ָ�븴λ
sbit FIFO_OE  =P1^4;		   //Ƭѡ�ź�(OE)


extern u8 cur_status;						 //֡��־λ  ��interrupt.c�����е���


u8 wr_Sensor_Reg(u8 regID, u8 regDat);
u8 rd_Sensor_Reg(u8 regID, u8 *regDat);

u8 Ov7670_init(void);						 //OV7670��ʼ��
u8 ov7670_test(void);						 //OV7670�򵥰���Ժ���

void OV7670_Window_Set(unsigned int sx,unsigned int sy,unsigned int width,unsigned int height);	   //ov7670��������
void OV7670_Light_Mode(unsigned char mode);
void OV7670_Special_Effects(unsigned char eft);
void OV7670_Contrast(unsigned char contrast);
void OV7670_Brightness(unsigned char bright);
void OV7670_Color_Saturation(unsigned char sat);

#endif




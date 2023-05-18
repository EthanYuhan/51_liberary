#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 

//SCCB ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/14
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
//		OV7670ģ�� ----------- STM32������
//		 OV_D0~D7  -----------   PC0~7 ����
//		 OV_SCL    ------------  PD3   ����
//		 OV_SDA    ------------  PG13 -->PD6
//		 OV_VSYNC  ------------  PA8   ����
//		 FIFO_RRST -----------   PG14  ����
//		 FIFO_OE   -----------   PG15  ����
//		 FIFO_WRST ------------  PD6  -->PG13
//		 FIFO_WEN  ------------  PB3   ����
//		 FIFO_RCLK ------------  PB4   ����	
//
//#define SCCB_SDA_IN()  {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=0X00800000;}    //-->PD6    ÿ��ĸ߰�λ
//#define SCCB_SDA_OUT() {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=0X00300000;}
#define SCCB_SDA_IN()  {GPIOD->CRL&=0XF0FFFFFF;GPIOD->CRL|=0X08000000;}    //-->PD6     ÿ��ĵͰ�λ
#define SCCB_SDA_OUT() {GPIOD->CRL&=0XF0FFFFFF;GPIOD->CRL|=0X03000000;}

//IO��������	 
#define SCCB_SCL    		PDout(3)	 	//SCL
#define SCCB_SDA    		PDout(6) 		//SDA  -->PD6	 

#define SCCB_READ_SDA    	PDin(6)  		//����SDA  -->PD6   
#define SCCB_ID   			0X42  			//OV7670��ID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
u8 SCCB_WR_Byte(u8 dat);
u8 SCCB_RD_Byte(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif














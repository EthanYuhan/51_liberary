#ifndef __SCCB_H
#define __SCCB_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 

//SCCB 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/14
//版本：V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
//		OV7670模块 ----------- STM32开发板
//		 OV_D0~D7  -----------   PC0~7 不变
//		 OV_SCL    ------------  PD3   不变
//		 OV_SDA    ------------  PG13 -->PD6
//		 OV_VSYNC  ------------  PA8   不变
//		 FIFO_RRST -----------   PG14  不变
//		 FIFO_OE   -----------   PG15  不变
//		 FIFO_WRST ------------  PD6  -->PG13
//		 FIFO_WEN  ------------  PB3   不变
//		 FIFO_RCLK ------------  PB4   不变	
//
//#define SCCB_SDA_IN()  {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=0X00800000;}    //-->PD6    每组的高八位
//#define SCCB_SDA_OUT() {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=0X00300000;}
#define SCCB_SDA_IN()  {GPIOD->CRL&=0XF0FFFFFF;GPIOD->CRL|=0X08000000;}    //-->PD6     每组的低八位
#define SCCB_SDA_OUT() {GPIOD->CRL&=0XF0FFFFFF;GPIOD->CRL|=0X03000000;}

//IO操作函数	 
#define SCCB_SCL    		PDout(3)	 	//SCL
#define SCCB_SDA    		PDout(6) 		//SDA  -->PD6	 

#define SCCB_READ_SDA    	PDin(6)  		//输入SDA  -->PD6   
#define SCCB_ID   			0X42  			//OV7670的ID

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














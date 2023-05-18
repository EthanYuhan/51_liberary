#ifndef __GT911_DRIVER_H
#define __GT911_DRIVER_H

//#include "stm32f10x_exti.h"
#include "ctiic.h"
#include "delay.h"
#include "sys.h"
#include "touch.h"
#include "lcd.h"

#define TOUCH_MAX 5

#define RST_OUT()	{	GPIOF->CRH&=0XFFFF0FFF;GPIOF->CRH|=0X00003000;}		//set RSSET pin to output
#define INT_OUT()	{	GPIOF->CRH&=0XFFFFF0FF;GPIOF->CRH|=0X00000300;}		//set RSSET pin to output
#define INT_IN()	{	GPIOF->CRH&=0XFFFFFFFF;GPIOF->CRH|=0X00000400;}		//set RSSET pin to output
//#define INT_OUT()	{	GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0X00000003;}		//set INT pin to output
//#define INT_IN()	{	GPIOB->CRH&=0XFFFFFFF0;GPIOB->CRH|=0X00000004;}		//set INT pin to input

//RST--PF11
//INT--PF10
#define RST_CTRL   		PFout(11)	//GT911 RESET pin out high or low
#define INT_CTRL   		PFout(10) 	//GT911 INT pin out high or low
#define INT_GET   		PFin(10) 	//Get GT911 INT pin status


#define GT9XX_IIC_RADDR 0xBB	//IIC read address, should be 0x29
#define GT9XX_IIC_WADDR 0xBA	//IIC write address, should be 0x28

#define GT9XX_READ_ADDR 0x814E	//touch point information
#define GT9XX_ID_ADDR 0x8140		//ID of touch IC


void GT911_int_sync(u16 ms);
void GT911_reset_guitar(u8 addr);
void GT911_gpio_init(void);
u8 GT9XX_WriteHandle (u16 addr);
u8 GT9XX_WriteData (u16 addr,u8 value);
u8 GT9XX_ReadData (u16 addr, u8 cnt, u8 *value);
u8 GT911_Init(void);
u8 Touch_Get_Count(void);
u8 GT911_Scan(u8 mode);
void GT9xx_send_config(void);
void GT9xx_Eint_Init(void);


#endif


//====================================TFT液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口
//液晶屏模块             C51单片机
// DB0~DB7       接       P00~P07        //数据低8位（8位模式：DB0~DB7接P00~P07）
// DB8~DB15      接       P20~P27        //数据高8位
//=======================================液晶屏控制线接线==========================================//
//液晶屏模块             C51单片机
//   CS          接        P54          //片选控制信号
//   RST         接        P35           //复位信号
//   RS          接        P55           //数据/命令选择控制信号
//   WR          接        P42           //写控制信号
//   RD          接        P44           //读控制信号
//   BL          接        VCC           //背光控制信号   51核心板TFT屏幕背光常亮 
//=========================================触摸屏接线==============================================//
//触摸屏使用的数据总线类型为IIC，51单片机采用模拟IIC总线通信
//触摸屏模块            C51单片机
//  SDA         接        P16           //电容触摸屏IIC总线数据引脚
//  SCL         接        P15           //电容触摸屏IIC总线时钟引脚
//  RST         接        P13           //电容触摸屏IC复位控制引脚（低电平复位）
//  INT         接        P17           //电容触摸屏中断检测引脚（发生触摸时为低电平）
//**************************************************************************************************/	

//*********此引脚说明放置lcd.h GT911.h gtiic.h三个文件中********************************************/

#ifndef __GT911_DRIVER_H
#define __GT911_DRIVER_H


#include "gtiic.h"
#include "sys.h"
#include "touch.h"
#include "lcd.h"
#include  "stc15f2k60s2.h"

#define TOUCH_MAX 5

//引脚定义

sbit RST_CTRL = P1^3;	//GT911 RESET pin out high or low
sbit INT_CTRL = P1^7; //GT911 INT pin out high or low

#define GT9XX_IIC_RADDR 0xBB	//IIC read address
#define GT9XX_IIC_WADDR 0xBA	//IIC write address

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
u8 GT911_Scan(void);
void GT9xx_send_config(void);
void GT9xx_Eint_Init(void);


#endif

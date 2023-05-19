#ifndef __LCD12864_H
#define __LCD12864_H

//---包含头文件---//
#include "stc15f2k60s2.h"
#include "sys.h"

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---定义使用的IO口---//
#define LCD12864_DATAPORT P0	  //数据IO口

sbit LCD12864_RS  =  P2^0;             //（数据命令）寄存器选择输入    TFT_D10  A8 
sbit LCD12864_RW  =  P2^1;             //液晶读/写控制                 TFT_D8   A9
sbit LCD12864_EN  =  P2^2;             //液晶使能控制									 TFT_D9   A10



//---声明全局函数---//
void LCD12864_Delay1ms(uint c);
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init();
void LCD12864_ClearScreen(void);
void LCD12864_SetWindow(uchar x, uchar y);
void LCD12864_VerticalRoll(uchar N_Pixel); //卷动显示
void LCD12864_Roll(void);
void LCD12864_pixel(unsigned char x, unsigned char y, unsigned char attr);
void LCD_line_h(unsigned char y, unsigned char attr);

#endif

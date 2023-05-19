#ifndef __LCD12864_H
#define __LCD12864_H

//---����ͷ�ļ�---//
#include "stc15f2k60s2.h"
#include "sys.h"

//---�ض���ؼ���---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

//---����ʹ�õ�IO��---//
#define LCD12864_DATAPORT P0	  //����IO��

sbit LCD12864_RS  =  P2^0;             //����������Ĵ���ѡ������    TFT_D10  A8 
sbit LCD12864_RW  =  P2^1;             //Һ����/д����                 TFT_D8   A9
sbit LCD12864_EN  =  P2^2;             //Һ��ʹ�ܿ���									 TFT_D9   A10



//---����ȫ�ֺ���---//
void LCD12864_Delay1ms(uint c);
void LCD12864_WriteCmd(uchar cmd);
void LCD12864_WriteData(uchar dat);
void LCD12864_Init();
void LCD12864_ClearScreen(void);
void LCD12864_SetWindow(uchar x, uchar y);
void LCD12864_VerticalRoll(uchar N_Pixel); //����ʾ
void LCD12864_Roll(void);
void LCD12864_pixel(unsigned char x, unsigned char y, unsigned char attr);
void LCD_line_h(unsigned char y, unsigned char attr);

#endif

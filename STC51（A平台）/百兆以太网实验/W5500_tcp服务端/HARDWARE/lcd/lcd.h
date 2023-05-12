
//====================================TFTҺ���������߽���==========================================//
//��ģ��Ĭ��������������Ϊ16λ����
//Һ����ģ��             C51��Ƭ��
// DB0~DB7       ��       P00~P07        //���ݵ�8λ��8λģʽ��DB0~DB7��P00~P07��
// DB8~DB15      ��       P20~P27        //���ݸ�8λ
//=======================================Һ���������߽���==========================================//
//Һ����ģ��             C51��Ƭ��
//   CS          ��        P54          //Ƭѡ�����ź�
//   RST         ��        P35           //��λ�ź�
//   RS          ��        P55           //����/����ѡ������ź�
//   WR          ��        P42           //д�����ź�
//   RD          ��        P44           //�������ź�
//   BL          ��        VCC           //��������ź�   51���İ�TFT��Ļ���ⳣ�� 
//=========================================����������==============================================//
//������ʹ�õ�������������ΪIIC��51��Ƭ������ģ��IIC����ͨ��
//������ģ��            C51��Ƭ��
//  SDA         ��        P16           //���ݴ�����IIC������������
//  SCL         ��        P15           //���ݴ�����IIC����ʱ������
//  RST         ��        P13           //���ݴ�����IC��λ�������ţ��͵�ƽ��λ��
//  INT         ��        P17           //���ݴ������жϼ�����ţ���������ʱΪ�͵�ƽ��
//**************************************************************************************************/	

//*********������˵������lcd.h GT911.h gtiic.h�����ļ���********************************************/	

#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"
#include  "stc15f2k60s2.h"



/////////////////////////////////////�û�������///////////////////////////////////	 
//֧�ֺ��������ٶ����л�
#define USE_HORIZONTAL  	  0 //����Һ����˳ʱ����ת���� 	0-0����ת��1-90����ת��2-180����ת��3-270����ת
#define LCD_USE8BIT_MODEL   0 	//�������������Ƿ�ʹ��8λģʽ 0,ʹ��16λģʽ.1,ʹ��8λģʽ
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////	  
//����LCD�ĳߴ�
#define LCD_W 320
#define LCD_H 480

//IO����
#define  LCD_DataPortH P2     //��8λ���ݿ�,8λģʽ��ֻʹ�ø�8λ 
#define  LCD_DataPortL P0     //��8λ���ݿ�	,8λģʽ�µ�8λ���Բ�����,��ȷ��P0���Ѿ�����10K����,����̫С����С4.7K,�Ƽ�10K.
sbit LCD_RS = P5^5;  		 //����/�����л�
sbit LCD_WR = P4^2;		  //д����
sbit LCD_RD = P4^4;		     //������
sbit LCD_CS = P5^4;		//Ƭѡ	
sbit LCD_RESET = P3^5;	      //��λ   
//sbit LCD_BL=P4^7;		//������ƣ��������Ҫ���ƣ���3.3V   51���İ�tft��Ļ���ⳣ��

//LCD�Ļ�����ɫ�ͱ���ɫ	   
extern u16  POINT_COLOR;//Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ
//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	 wramcmd;		//��ʼдgramָ��
	u16  rramcmd;   //��ʼ��gramָ��
	u16  setxcmd;		//����x����ָ��
	u16  setycmd;		//����y����ָ��	 
}_lcd_dev; 	

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����
void LCD_Init(void); 
void LCD_Clear(u16 Color);
void LCD_write(u8 HVAL,u8 LVAL);
u16 LCD_read(void);
void LCD_WR_DATA(u16 Data);
u16 LCD_RD_DATA(void);
void LCD_WR_REG(u16 Reg);
void LCD_SetCursor(u16 Xpos, u16 Ypos);//���ù��λ��
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);//������ʾ����
void LCD_DrawPoint(u16 x,u16 y);//����
u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_WriteRAM_Prepare(void);
void LCD_ReadRAM_Prepare(void);
void LCD_direction(u8 direction );
void LCD_WR_DATA_16Bit(u16 Data);
u16 Lcd_RD_DATA_16Bit(void);
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
void LCD_ReadReg(u16 LCD_Reg,u8 *Rval,int n);
u16 Color_To_565(u8 r, u8 g, u8 b);
u16 LCD_Read_ID(void);


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)


					  		 
#endif  
	 
	 




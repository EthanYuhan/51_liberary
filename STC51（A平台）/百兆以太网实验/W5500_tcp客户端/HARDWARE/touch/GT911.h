
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

#ifndef __GT911_DRIVER_H
#define __GT911_DRIVER_H


#include "gtiic.h"
#include "sys.h"
#include "touch.h"
#include "lcd.h"
#include  "stc15f2k60s2.h"

#define TOUCH_MAX 5

//���Ŷ���

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

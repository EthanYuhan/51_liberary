#ifndef __DS18B20_H
#define __DS18B20_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 

//DS18B20��������	   
								  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
#define DS18B20_IO_IN()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0X08000000;} //PG11 -->PB14
#define DS18B20_IO_OUT() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=0X03000000;}
////IO��������											   
#define	DS18B20_DQ_OUT PBout(14) //���ݶ˿�	PG11 -->PB14
#define	DS18B20_DQ_IN  PBin(14)  //���ݶ˿�	PG11 
   	
u8 DS18B20_Init(void);//��ʼ��DS18B20
short DS18B20_Get_Temp(void);//��ȡ�¶�
void DS18B20_Start(void);//��ʼ�¶�ת��
void DS18B20_Write_Byte(u8 dat);//д��һ���ֽ�
u8 DS18B20_Read_Byte(void);//����һ���ֽ�
u8 DS18B20_Read_Bit(void);//����һ��λ
u8 DS18B20_Check(void);//����Ƿ����DS18B20
void DS18B20_Rst(void);//��λDS18B20    
#endif
















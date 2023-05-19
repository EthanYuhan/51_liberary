#include "stc15f2k60s2.h"
#include <intrins.h>
#include <stdio.h>
#include "string.h"
#include "sys.h"
#include "lcd12864.h"
#include "key.h"
#include "timer.h"
#include "ad.h"
#include "ds1302.h"
#include "dht11.h"

u8 code CharCodeLine1[]="CZCJJQ";       //��һ����ʾ�ַ�
u8 code CharCodeLine2[]="0302";         //�ڶ�����ʾ�ַ�
u8 code MileValue[]="mile��       km";  //��ʾ��ʻ���
u8 code CostValue[]="cost��     yuan";  //��ʾӦ��ȡ����
u8 code WaiteTimeValue[]="delay ��    min"; //��ʾӦ��ȡ����

u8  StartDrive = 0 ;  //0-ͣ��������� 1-��ʼ����Ƽ��� 2-��ֹ�ȴ�״̬
u16 MileNum = 0; 	  //��̼���
u16 CostNum = 0;      //Ӧ�շ��ü���
u16 WaiteTimeNum = 0; //�ȴ�ʱ��
u16 StartPrice = 6;   //�𲽼� Ĭ�� 6
u8  TimeNum = 1;      //ʱ��1/2 Ĭ��Ϊ1
u16 ad;               //ADC����ֵ
u8  MoterSpeed =8;    //ͨ����ʱ��2���Ʋ������ת��
u8  AdcFlag = 0 ;     //adc������־
u16 InterruptCount = 0 ;  //�����жϼ���

extern u8 KeyCode;      //������ֵ
extern u16 TimerCount ; //��ʱ������
extern u8 PulseNum;     //�������
extern u16 WaiteTimeCount; //��ֹ״̬�ȴ�ʱ�����

sbit people_dec = P0^1;

void IO_init(void){  //IO�ڳ�ʼ��
	//IO����Ϊ׼˫��IO�� 
	P0M0 = 0X00;	P0M1 = 0X00;  
	P1M0 = 0X00;	P1M1 = 0X00;
	P2M0 = 0X00;	P2M1 = 0X00;
	P3M0 = 0X00;	P3M1 = 0X00;
	P4M0 = 0X00;	P4M1 = 0X00;  	
	P5M0 = 0X00;	P5M1 = 0X00;  	
	P6M0 = 0X00;	P6M1 = 0X00;  	
	P7M0 = 0X00;	P7M1 = 0X00;  
	
	//�����������
	P2M1 &= ~(1<<2),P2M0 |=  (1<<2);//12864LCD����
	P2M1 &= ~(1<<1),P2M0 |=  (1<<1); 
	P2M1 &= ~(1<<0),P2M0 |=  (1<<0); 
	P1M1 &= ~(1<<3),P1M0 |=  (1<<3);//�������
	P1M1 &= ~(1<<4),P1M0 |=  (1<<4); 
	P1M1 &= ~(1<<1),P1M0 |=  (1<<1); 
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
	P5M1 &= ~(1<<5),P5M0 |=  (1<<5);//������
	P5M1 &= ~(1<<2),P5M0 |=  (1<<2);//�̵���
}


void main(void){  //������
	u8 i;	
	u8 temp;
	IO_init();				 //IO�ڳ�ʼ��		
	LCD12864_Init();         //12864Һ����ʼ?
	LCD12864_WriteCmd(0x34);
	LCD12864_ClearScreen();	
	LCD12864_Roll();
	
	while(1){  //��ѭ��
		if(people_dec==0){
			P53=0;
		}else{
			P53=1;
		}	
	}
}




void exint0()  interrupt 2 //�ⲿ�ж�1�ж�
{
	InterruptCount ++ ;    //����һȦ20��
}






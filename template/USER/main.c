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
	P2M1 &= ~(1<<2),P2M0 |=  (1<<2); //12864LCD����
	P2M1 &= ~(1<<1),P2M0 |=  (1<<1); 
	P2M1 &= ~(1<<0),P2M0 |=  (1<<0); 
	P1M1 &= ~(1<<3),P1M0 |=  (1<<3); //�������
	P1M1 &= ~(1<<4),P1M0 |=  (1<<4); 
	P1M1 &= ~(1<<1),P1M0 |=  (1<<1); 
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
	P5M1 &= ~(1<<5),P5M0 |=  (1<<5); //������
	P5M1 &= ~(1<<2),P5M0 |=  (1<<2); //�̵���
}


void main(void){  //������
	u8 i;	
	u8 temp;
	
	UpdateTemp();//��ȡ��ʪ��

	IO_init();				 //IO�ڳ�ʼ��		
    LCD12864_Init();         //12864Һ����ʼ��	
	Give(0x00); 			 //ֹͣ�������
	
	Roll();          	     //����ʾ������	
	Delay100ms();	
	
	LCD12864_WriteCmd(0x01); //���LCD12864����ʾ����	
	LCD12864_SetWindow(0,0); //��1����ʾ��ʻ���
	while(MileValue[i]!='\0'){
		LCD12864_WriteData(MileValue[i]);
		i++;
	}
	i=0;	
	LCD12864_SetWindow(1,0); //��2����ʾӦ��ȡ����
	while(CostValue[i]!='\0'){
		LCD12864_WriteData(CostValue[i]);
		i++;
	}
	i=0;
  	
	while(1){  //��ѭ��
		IO_KeyScan();   //����ɨ�� ��ⰴ�� 
		
		ds1302_scan();  //RTCʱ����ʾ ʱ��
		delay1ms(10);   //��ʱ
	   
		if(KeyCode ==1)	//S1���£�������̵�һ����
		{
			KeyCode=0;				
			if(StartDrive==0)  //0-�������״̬(ͣ��)
			{
				StartDrive=1;
				PulseNum =0 ;
				TimerCount = 0;				
				InterruptCount = 0; //��̼�������
				WaiteTimeCount = 0; //��ֹ�ȴ�ʱ������
				MileNum = 0; 		//��̼���
				CostNum = 0; 		//Ӧ�շ��ü���
				WaiteTimeNum = 0;   //�ȴ�ʱ��
				
				Timer2Init(); //��ʱ��2��ʼ��				
				IE2 |=0x04;	  //����ʱ��2�ж�--��ʱ�����Ʋ������ת��
				IT1 =1;		  //����INT1���ж����ͣ�1�����½��� 0�������ػ��½��أ�
				EX1 =1;       //ʹ��INT1�ж� 		
				EA  =1;       //�����ж� 
			}
			else if(StartDrive==1) //1-��ʼ����Ƽ���״̬
			{
				StartDrive=0;//ֹͣ����ֹͣ�Ƽ�				
				IE2 &=~0x04; //�ض�ʱ��2�ж�
				EA  =0;	     //�����ж�						
				Give(0x00);  //ֹͣ�������		
			}		
		}
		else if(KeyCode ==2) //S2����
		{
			KeyCode=0;			
			if(StartDrive==1) 
			{
				StartDrive=2; //����������״̬(ͣ��)			
				IE2 &=~0x04;  //�ض�ʱ��2�ж�								
				Give(0x00);   //ֹͣ�������	
					
				Timer0Init(); //��ʱ��0��ʼ��
				ET0 =1;		  //ʹ�ܶ�ʱ���ж�	
				TR0 =1;		  //��ʱ��0��ʼ��ʱ--����ȴ�ʱ��
				EA  =1;	      //�����ж�				

				LCD12864_SetWindow(2,0); //��3����ʾ�ȴ�ʱ��
				while(WaiteTimeValue[i]!='\0')  
				{
					LCD12864_WriteData(WaiteTimeValue[i]);
					i++;
				}
				i=0;			
			}
			else if(StartDrive==2) 
			{
				StartDrive=1;	//�˳���ֹ״̬				
				ET0 =0;			//�� ��ʱ���ж�	
			    TR0 =0;			//�� ��ʱ��0��ʼ��ʱ 				
				IE2 |=0x04;	    //����ʱ��2�ж�--��ʱ�����Ʋ������ת��				

				LCD12864_SetWindow(2,4);//��3��----��ʾ�ȴ�ʱ��
				WaiteTimeNum=WaiteTimeCount/20; //����������1�����5���� 
				LCD12864_WriteData(WaiteTimeNum/1000%10+0x30);//ǧλ
				LCD12864_WriteData(WaiteTimeNum/100%10+0x30);				
				LCD12864_WriteData(WaiteTimeNum/10%10+0x30);
				LCD12864_WriteData(WaiteTimeNum%10+0x30);
			}
		}
		else if(KeyCode ==3) //S3�����л�ʱ��
		{
			KeyCode=0;			
			if(TimeNum==1)
			{
				TimeNum=2;
				StartPrice = 7;
			}
			else
			{
				TimeNum=1;
				StartPrice = 6;
			}
		}
		
		else if(KeyCode ==13) //RTCʱ�����  ʱ��һ
		{
			KeyCode=0;	
	        temp = bcd_read(0x85);//ʱ				
			if(temp==23) temp=0;
			else temp=temp+1;
			temp = temp/10*16+temp%10;
			Write_Ds1302(0x8E,0X00);   //дʹ��	
			Write_Ds1302(0x84,temp);	
			Write_Ds1302(0x8E,0x80);   //д��ֹ
		}
		else if(KeyCode ==14) //RTCʱ����� ʱ��һ
		{
			KeyCode=0;
			temp = bcd_read(0x85);//ʱ			
			if(temp==0) temp=23;
			else temp=temp-1;
			temp = temp/10*16+temp%10;
			Write_Ds1302(0x8E,0X00);   //дʹ��	
			Write_Ds1302(0x84,temp);	
			Write_Ds1302(0x8E,0x80);   //д��ֹ
		}
		else if(KeyCode ==15) //RTCʱ�����  �ּ�һ
		{
			KeyCode=0;
			temp = bcd_read(0x83);//��				
			if(temp==59) temp=0;
			else temp=temp+1;
			temp = temp/10*16+temp%10;
			Write_Ds1302(0x8E,0X00);   //дʹ��	
			Write_Ds1302(0x82,temp);	
			Write_Ds1302(0x8E,0x80);   //д��ֹ			
		}
		else if(KeyCode ==16) //RTCʱ����� �ּ�һ
		{
			KeyCode=0;
			temp = bcd_read(0x83);//��			
			if(temp==0) temp=59;
			else temp=temp-1;
			temp = temp/10*16+temp%10;
			Write_Ds1302(0x8E,0X00);   //дʹ��	
			Write_Ds1302(0x82,temp);	
			Write_Ds1302(0x8E,0x80);   //д��ֹ
		}
		
		
		
		if(StartDrive==1)  //��ʼ�Ƽ� ����ʾ
		{
			
			if(AdcFlag > 0) //ADC����һ��
			{
				AdcFlag = 0;
				ad=GetADCResult(0);			   //�ɼ�AD ֵΪ300-1020			
			    MoterSpeed =(float)ad*0.05-13; //4-40��Χ
			}
				

#if DRIVING_SIMULATION	
			
			MileNum=PulseNum*4;     //��ʱ���������1����ʻ4km
			LCD12864_SetWindow(0,3);//��1��	��ʾ���
			LCD12864_WriteData(MileNum/10000%10+0x30);
			LCD12864_WriteData(MileNum/1000%10+0x30); //ǧλ
			LCD12864_WriteData(MileNum/100%10+0x30);			
			LCD12864_WriteData(MileNum/10%10+0x30);			
			LCD12864_WriteData(MileNum%10+0x30);
			
			CostNum=(MileNum-3)*12+60;  //��Ϊ1�뼴����4km���Ѿ������𲽼ۣ��˼۸�Ϊ10����ʵ�۸�
			LCD12864_SetWindow(1,3);    //��2��	��ʾӦ��ȡ����	
			LCD12864_WriteData(CostNum/1000%10+0x30);//ǧλ
			LCD12864_WriteData(CostNum/100%10+0x30);			
			LCD12864_WriteData(CostNum/10%10+0x30);	
			LCD12864_WriteData('.');
			LCD12864_WriteData(CostNum%10+0x30);
					
#else								  //����һȦ20��1Ȧ1�ף�Ϊ��ʾ�����1000������1Ȧ1km 1��0.05km
			MileNum=InterruptCount*5; //����������̼��㣬Ϊ��ʾС���� ��100�� 									  
			LCD12864_SetWindow(0,3);  //��1����ʾ���
			LCD12864_WriteData(MileNum/10000%10+0x30);
			LCD12864_WriteData(MileNum/1000%10+0x30);//ǧλ
			LCD12864_WriteData(MileNum/100%10+0x30);	
			LCD12864_WriteData('.');                 //��λС����
			LCD12864_WriteData(MileNum/10%10+0x30);			
			LCD12864_WriteData(MileNum%10+0x30);
			
			if(MileNum<= 300)  //��100�����𲽼�����3km
			{
				CostNum = (StartPrice*100) +(WaiteTimeNum*20) ; //��ʵ�۸�100��
			}
			else              //�����𲽼�����
			{
				              //��ʵ�۸�100��
				CostNum =(float)((MileNum-300)*1.2)+(StartPrice*100)+(WaiteTimeNum*20) ;
			}			
			LCD12864_SetWindow(1,3);//��2��	��ʾӦ��ȡ����   	
			LCD12864_WriteData(CostNum/10000%10+0x30);
			LCD12864_WriteData(CostNum/1000%10+0x30);//ǧλ
			LCD12864_WriteData(CostNum/100%10+0x30);	
			LCD12864_WriteData('.');			
			LCD12864_WriteData(CostNum/10%10+0x30);	
#endif

		}
		else if(StartDrive==2) //��ֹ�ȴ�״̬ ѭ����ʾ�ȴ�ʱ��
		{
			LCD12864_SetWindow(2,4);        //��3����ʾ�ȴ�ʱ��
			WaiteTimeNum=WaiteTimeCount/20; //��1�����5����
			LCD12864_WriteData(WaiteTimeNum/1000%10+0x30);//ǧλ
			LCD12864_WriteData(WaiteTimeNum/100%10+0x30);				
			LCD12864_WriteData(WaiteTimeNum/10%10+0x30);
			LCD12864_WriteData(WaiteTimeNum%10+0x30);
			
			
			//��ʾ�ܽ��
			if(MileNum<= 300)  //��100��  �𲽼�����3km
			{
				CostNum = (StartPrice*100) +(WaiteTimeNum*20) ; //��ʵ�۸�100��
			}
			else              //�����𲽼�����
			{
				              //��ʵ�۸�100��
				CostNum =(float)((MileNum-300)*1.2)+(StartPrice*100)+(WaiteTimeNum*20) ;
			}			
			LCD12864_SetWindow(1,3);//��2��	��ʾӦ��ȡ����   	
			LCD12864_WriteData(CostNum/10000%10+0x30);
			LCD12864_WriteData(CostNum/1000%10+0x30);//ǧλ
			LCD12864_WriteData(CostNum/100%10+0x30);	
			LCD12864_WriteData('.');			
			LCD12864_WriteData(CostNum/10%10+0x30);	
		}
	}
}



void exint0()  interrupt 2 //�ⲿ�ж�1�ж�
{
	InterruptCount ++ ;    //����һȦ20��
}






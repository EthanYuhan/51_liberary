#include "stc15f2k60s2.h"
#include <intrins.h>
#include "sys.h"
#include "lcd12864.h"
#include "key.h"
#include "motor.h"
#include "timer.h"

u8 code CharCodeLine1[]="ZNDTKZQ";  //��һ����ʾ�ַ�
u8 code CharCodeLine2[]="0208";     //�ڶ�����ʾ�ַ�
u8 code FloorNum[]="Floor:5";       //Ĭ����5¥
u8 code PRESSFloorNum[]="1x2x3x4x5x6x7x8x"; //��������ʾ��¥��� 
u8 code PRESS9FloorNum[]="9x";

										    // 8 7 6 5 4 3 2 1   ¥
u8 PressFloorValue = 0; // 0 0 0 0 0 0 0 0      0δ���£�1����    //�����µ�¥��
u8 Floor9Value = 0;    //�Ƿ����˰���¥
u8 CurrentFloor = 1 ;  //��ǰ¥��ţ�Ĭ����5¥
u8 LiftDirection = 2;  //0ֹͣ 1���� 2�½�
u8 DesFloor = 0;  //Ŀ�ĵ�¥�� 0Ϊ��Чֵ

extern u8 KeyCode;  //������ֵ
extern u8 speed;    //��������ٶ�
extern u16 TimerCount ; //��ʱ������
extern u8 GoTo5FloorFlag;  //�����Ƿ����ȥ5¥��־λ


sbit beep=P5^5;		// ����������
sbit led=P5^3;	
sbit relay=P5^2;



void IO_init(void)  //IO�ڳ�ʼ��
{
	P0M0 = 0X00;	P0M1 = 0X00;  //IO����Ϊ׼˫��IO�� 
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


void Roll(void)  //����ʾ������
{	
	uchar i;
	
	//Ϊ��ʾ�ϰ�����һ���ַ���׼������ַ0xa0  ��ϸ�ο����£�https://wenku.baidu.com/view/375ec764cc22bcd127ff0c9a.html	
	LCD12864_WriteCmd(0xa0);	
	while(CharCodeLine1[i]!='\0')
	{
		LCD12864_WriteData(CharCodeLine1[i]);
		i++;
	}
	i=0;
  //Ϊ��ʾ�ϰ����ڶ����ַ���׼������ַ0xb0  	
	LCD12864_WriteCmd(0xb0);    
	while(CharCodeLine2[i]!='\0')
	{
		LCD12864_WriteData(CharCodeLine2[i]);
		i++;
	}
	
		for(i=0;i<33;i++)   //�ϰ�������ʾ
	{
			LCD12864_VerticalRoll(i);
			Delay100ms();	   //ÿ�и�16�����أ�����32���أ�0.1���1���أ���������ʾ��������Ҫ3.2��
	}
	
	LCD12864_WriteCmd(0x30);	 //�ָ�����ָ�

}

void KeySetLiftUpDownValue(void)  //����PressFloorValue����ʾ�����µ�¥�����Ϣ
{
	if(KeyCode >0)
	{
		TimerCount = 0;   //�а��������£���ʱ�������¿�ʼ����
	}	
	
	if(KeyCode ==1)	
		{			
			PressFloorValue|=0x01;	
      LCD12864_WriteCmd(0x88);  
			LCD12864_WriteData('1');
		  LCD12864_WriteData('o');
		
		}
		else if(KeyCode ==2)	
		{
			PressFloorValue|=0x02;	
			LCD12864_WriteCmd(0x89);  
			LCD12864_WriteData('2');
		  LCD12864_WriteData('o');
			
		}
		else if(KeyCode ==3)	
		{
			PressFloorValue|=0x04;	
			LCD12864_WriteCmd(0x8a);  
			LCD12864_WriteData('3');
		  LCD12864_WriteData('o');
		}
		else if(KeyCode ==4)	
		{
		  PressFloorValue|=0x08;	
			LCD12864_WriteCmd(0x8b);  
			LCD12864_WriteData('4');
		  LCD12864_WriteData('o');
		}
		else if(KeyCode ==5)	
		{
			PressFloorValue|=0x10;
      LCD12864_WriteCmd(0x8c);  
			LCD12864_WriteData('5');
		  LCD12864_WriteData('o');			
		}
		else if(KeyCode ==6)	
		{
			PressFloorValue|=0x20;	
			LCD12864_WriteCmd(0x8d);  
			LCD12864_WriteData('6');
		  LCD12864_WriteData('o');
		}
		else if(KeyCode ==7)	
		{
			
			PressFloorValue|=0x40;
			LCD12864_WriteCmd(0x8e);  
			LCD12864_WriteData('7');
		  LCD12864_WriteData('o');
		}
		else if(KeyCode ==8)	
		{
			PressFloorValue|=0x80;
			LCD12864_WriteCmd(0x8f);  
			LCD12864_WriteData('8');
		  LCD12864_WriteData('o');
		}
		else if(KeyCode ==9)	//���˰���¥
		{
			Floor9Value = 1;	
			LCD12864_WriteCmd(0x98);  
			LCD12864_WriteData('9');
		  LCD12864_WriteData('o');
		}
	
		KeyCode=0;

}

void GoToFloor(u8 num)   //�ӵ�ǰ¥��ִ�numָ��¥��
{
  u8 i,n;	
	
	beep=0;   //������ʾ  //���е�������ʾ��ÿ������ʱ����ʾ��
	led=0;
  Delay100ms();
	beep=1;   
	led=1;	
	   		

	n=CurrentFloor;		
	
	if(num<CurrentFloor)
	{			
			for(i=0;i<(n-num);i++)
			{
				MotorCCWRound(1);
				LCD12864_WriteCmd(0x93);  //��ʾ��ǰ¥����  
				CurrentFloor--;
		    LCD12864_WriteData(CurrentFloor+0x30);  
			}
	}
	else 
	{
		for(i=0;i<(num-n);i++)
			{
				MotorCWRound(1);
				LCD12864_WriteCmd(0x93);  //��ʾ��ǰ¥����  
				CurrentFloor++;
		    LCD12864_WriteData(CurrentFloor+0x30);  
			}			
	
	}						

	  PressFloorValue&=(~(0x01<<(num-1))); //���Ŀ�ĵ�¥�㱻������Ϣ  	
	  LCD12864_SetWindow(0,0);
	  LCD12864_WriteData(PressFloorValue+0x30); //������Ϣ
	  LCD12864_WriteCmd(0x87+num);       //��������ʾ
		LCD12864_WriteData(num+0x30);       
		LCD12864_WriteData('x');
	
	
		CurrentFloor = num; //���µ�ǰ¥��
	  //LiftDirection=0;  //���ﲻ������Ϊֹͣ״̬��ֻ��һ�����򵽵ײ�û��¥�㱻���µ�ʱ��

}


void main(void)  //������
{	
	u8 i,n,j;	

	IO_init();				 //IO�ڳ�ʼ��		
  LCD12864_Init();   //12864Һ����ʼ��		
	MotorStop(); 			 //ֹͣ�������
	beep=1;   				 //�ط�����			
	relay=1;           //����
	led=1;             //�ص�
	
	Roll();            //����ʾ������
	Delay100ms();	
	LCD12864_WriteCmd(0x01); //���LCD12864����ʾ����	
	
	LCD12864_SetWindow(1,0); //�ڶ�����ʾ��ǰ¥���
	while(FloorNum[i]!='\0')
	{
		LCD12864_WriteData(FloorNum[i]);
		i++;
	}
	i=0;	
	
	LCD12864_WriteCmd(0x88); //��������ʾ�����µ�¥��� ��ʼֵ��ʾ
	while(PRESSFloorNum[i]!='\0')  
	{
		LCD12864_WriteData(PRESSFloorNum[i]);
		i++;
	}
	i=0;
	LCD12864_WriteCmd(0x98); //��������ʾ��¥�Ƿ񱻰��µ�¥���  
	LCD12864_WriteData('9');
	LCD12864_WriteData('x');
	
	Timer0Init();   //��ʱ����ʼ���������ж�    

  LCD12864_WriteCmd(0x93);  //��ʾ��ǰ¥����  	   
	LCD12864_WriteData(CurrentFloor+0x30);  
	
  	
	while(1)  
	{

			for (i=0;i<16;i++)    //ֱ�Ӽ��16�ΰ���
			{
				IO_KeyScan();		         //����ɨ�� ��ⰴ�� 	
		   		KeySetLiftUpDownValue(); //����PressFloorValue����ʾ�����µ�¥�����Ϣ		xo��ʾ
			}	
		
		// if((GoTo5FloorFlag==1)&&(CurrentFloor!=5)&&((LiftDirection==0)) ) //�����ݿ���ʱ���е�5¥
		// {
		// 	if(CurrentFloor>5)
		// 	{
		// 		LiftDirection=2; //��������
		// 	}
		// 	else
		// 	{
		// 		LiftDirection=1; //��������
		// 	}
			
		// 	GoTo5FloorFlag = 0; 
		// 	DesFloor = 5 ; 				//Ŀ��¥�㸳ֵ5	
		// 	LCD12864_SetWindow(0,0);//������Ϣ				
		// 	LCD12864_WriteData(DesFloor+0x30);//��һ����ʾ ������Ϣ Ŀ��¥���				
		// 	GoToFloor(DesFloor);  //�������е�5¥
			
		// }		
		
		
		// //-------------------���ݿ����߼�������-----------------------
		
		// if((PressFloorValue>0)||(Floor9Value>0) )  //¥�㰴���б�����    LiftDirection��ֵ����     
		// {			
			
		// 	if((LiftDirection==0)&&(PressFloorValue<(0x01<<(CurrentFloor-1)))&&(PressFloorValue>0))    //��ǰֹͣ״̬ ��ǰ¥�������µ�¥���б����� ��������  0001 0000
		// 	{
		// 	LiftDirection = 2;  //����
		// 	LCD12864_SetWindow(0,0);//������Ϣ
		// 	LCD12864_WriteData('D');	
		// 	}
		// 	else if((LiftDirection==0)&&(PressFloorValue>(0x01<<(CurrentFloor-1)))&&(PressFloorValue>0))
		// 	{
		// 	LiftDirection = 1;  //����
		// 	LCD12864_SetWindow(0,0);//������Ϣ
		// 	LCD12864_WriteData('U');
		// 	}
				
		// }		
		
		// 	if((LiftDirection==2)&&(PressFloorValue>0) ) //��ǰ����״̬    DesFloor��ֵ����
		// 	{	
		// 		for(j=1;j<CurrentFloor;j++)
		// 		//for(j=CurrentFloor;j>0;j--)
		// 		{
		// 			if(((0x01<<(CurrentFloor-j-1))&PressFloorValue)> 0)
		// 			{
		// 				 break; 
		// 			}
		// 		}
       			
		// 		if(j<CurrentFloor)
		// 		{	
		// 			if(DesFloor>0) //�����ʱDesFloor��Ϊ0 �����豸����������
		// 			{
		// 				if(DesFloor < CurrentFloor-j	)//����������˵
		// 				{
		// 					DesFloor = CurrentFloor-j;  //����DesFloor
		// 				}
						
		// 			}
		// 			else DesFloor = CurrentFloor-j;	
					
		// 		LCD12864_SetWindow(0,0);//������Ϣ				
		// 	  LCD12864_WriteData(DesFloor+0x30);//��һ����ʾ ������Ϣ Ŀ��¥���
				
		// 		//GoToFloor(DesFloor);   //�ó�ȥ ����һ��һ��¥����
		// 		}
		// 		else   //if j==CurrentFloor �Ĵ����֧		
		// 		{					
		// 		 LiftDirection=0; //����ֹͣ 
		// 		 DesFloor = 0 ; //Ŀ��¥�㸳ֵ0 				
		// 		}
		// 	}			
		// 	else if ((LiftDirection==1)&&(PressFloorValue>0))  //��ǰ����״̬  DesFloor��ֵ����
		// 	{
		// 		for(j=0;j<(8-CurrentFloor+1);j++)
		// 		//for(j=CurrentFloor-1;j>0;j--)
		// 		{
		// 			if(((0x01<<(CurrentFloor+j-1))&PressFloorValue)> 0)
		// 			{
		// 				 break; 
		// 			}
		// 		}
				
		// 		if(j<(8-CurrentFloor+1))
		// 		{	
		// 			if(DesFloor>0) //�����ʱDesFloor��Ϊ0 �����豸����������
		// 			{
		// 				if(DesFloor > CurrentFloor+j	)//����������˵
		// 				{
		// 					DesFloor = CurrentFloor+j;  //����DesFloor
		// 				}
						
		// 			}
		// 			else DesFloor = CurrentFloor+j;	
					
		// 		LCD12864_SetWindow(0,0);//������Ϣ				
		// 	  LCD12864_WriteData(DesFloor+0x30);//��һ����ʾ ������Ϣ Ŀ��¥���
				
		// 		}
		// 		else   //if j==CurrentFloor �Ĵ����֧		
		// 		{					
		// 		 LiftDirection=0; //����ֹͣ   
		// 		 DesFloor = 0 ; //Ŀ��¥�㸳ֵ0 				
		// 		}

		// 	}			
			
			
		// 	//�������еĿ���
		// 	if(DesFloor>0)  
		// 	{
		// 		if(DesFloor<CurrentFloor)
		// 		{
		// 			GoToFloor(CurrentFloor-1); //һ��һ�����
		// 		}
		// 		else if(DesFloor>CurrentFloor)
		// 		{
		// 			GoToFloor(CurrentFloor+1); //һ��һ�����				
		// 		}
		// 		else  //����
		// 		{					
		// 			LCD12864_WriteCmd(0x94);  
		// 			LCD12864_WriteData('=');  //��ʾ����ͣ
					
		// 			//LiftDirection=0; //����ֹͣ   
		// 			DesFloor = 0 ; //Ŀ��¥�㸳ֵ0 

		// 			beep=0;   //������ʾ
		// 			led=0;
		// 			Delay100ms();
		// 			beep=1;   
		// 			led=1;			
		// 			relay=0;  //���ſ���
		// 			led=0;
					
		// 			delay_ms(1000); //����ά��1sʱ��
		// 			delay_ms(1000); //����ά��1sʱ��
							
		// 			relay=1; //���Źص�
		// 			led=1;				
		// 		}
		// 	}

		// //---------------------------------------------
		

	
	}
	
}


#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "motor.h"
#include "timer.h"
#include "beep.h" 
#include "relay.h"

 
/************************************************
2019�����⸴��
************************************************/



u8 FloorNum[]="Floor:1";       //Ĭ����1¥
u8 PRESSFloorNum[]="1x2x3x4x5x6x7x8x9x"; //��������ʾ��¥��� 

										    // 8 7 6 5 4 3 2 1   ¥
u8 PressFloorValue = 0; // 0 0 0 0 0 0 0 0      0δ���£�1����    //�����µ�¥��
u8 Floor9Value = 0;    //�Ƿ����˰���¥
u8 CurrentFloor = 1 ;  //��ǰ¥��ţ�Ĭ����5¥
u8 LiftDirection = 0;  //0ֹͣ 1���� 2�½�
u8 DesFloor = 0;  //Ŀ�ĵ�¥�� 0Ϊ��Чֵ
u8 KeyCode=0;	//����ֵ

extern u16 TimerCount ; //��ʱ������
extern u8 GoTo5FloorFlag;  //�����Ƿ����ȥ5¥��־λ



void KeySetLiftUpDownValue(void)  //����PressFloorValue����ʾ�����µ�¥�����Ϣ
{
	//u8 temp[]="0";	
	if(KeyCode >0)
	{
		TimerCount = 0;   //�а��������£���ʱ�������¿�ʼ����
	}	
	
	if(KeyCode ==1)	
		{			
			PressFloorValue|=0x00000001;	
      LCD_ShowString(30+12,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��			
//			sprintf((char*)temp,"PressFloorValue is:%d",PressFloorValue);
//			LCD_ShowString(30,120+10,240,24,24,temp);
		}
		else if(KeyCode ==2)	
		{
			PressFloorValue|=0x00000002;		
      LCD_ShowString(30+12*3,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��			
		}
		else if(KeyCode ==3)	
		{
			PressFloorValue|=0x00000004;	
		  LCD_ShowString(30+12*5,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��	
		}
		else if(KeyCode ==4)	
		{
		  PressFloorValue|=0x00000008;	
			LCD_ShowString(30+12*7,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��	
		}
		else if(KeyCode ==5)	
		{
			PressFloorValue|=0x00000010;
    	LCD_ShowString(30+12*9,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��		
		}
		else if(KeyCode ==6)	
		{
			PressFloorValue|=0x00000020;	
			LCD_ShowString(30+12*11,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��	
		}
		else if(KeyCode ==7)	
		{			
			PressFloorValue|=0x00000040;
		  LCD_ShowString(30+12*13,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��	
		}
		else if(KeyCode ==8)	
		{
			PressFloorValue|=0x00000080;
	    LCD_ShowString(30+12*15,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��	
		}
		else if(KeyCode ==9)	//���˰���¥
		{
			PressFloorValue|=0x00000100;
		  LCD_ShowString(30+12*17,90+10,240,24,24,"o"); //ÿ���ַ�ռ12�Ŀ��	
		}
	
		KeyCode=0;

}


void GoToFloor(u8 num)   //�ӵ�ǰ¥��ִ�numָ��¥��
{
  u8 i,n;	
	
	BEEP=0;    //������ʾ  //���е�������ʾ��ÿ������ʱ����ʾ��
	LED0=0;
  delay_ms(100);
	BEEP=1;  // �ط�����
	LED0=1;  //�ص�	
	
	n=CurrentFloor;		
	
	if(num<CurrentFloor)
	{			
			for(i=0;i<(n-num);i++)
			{
				MotorCCWRound(1);
				CurrentFloor--;
				LCD_ShowNum(102,70,CurrentFloor,1,24);
			}
	}
	else 
	{
		for(i=0;i<(num-n);i++)
			{
				MotorCWRound(1);
				CurrentFloor++;
		    LCD_ShowNum(102,70,CurrentFloor,1,24); 
			}			
	
	}						

	  PressFloorValue&=(~(0x00000001<<(num-1))); //���Ŀ�ĵ�¥�㱻������Ϣ	
	  LCD_ShowString(18+num*24,90+10,240,24,24,"x");	
	
		CurrentFloor = num; //���µ�ǰ¥��
	  //LiftDirection=0;  //���ﲻ������Ϊֹͣ״̬��ֻ��һ�����򵽵ײ�û��¥�㱻���µ�ʱ��

}


 	
 int main(void)
 {	 
 	u8 x=0;
	u8 y,i,j,n;          //tft��ʾ������
	u8 temp[]="0";
	 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	KEY_Init();          //���󰴼���ʼ��   
	MOTOR_Init();  //�������IO��ʼ��
  RELAY_Init();  //�̵�����ʼ��
  BEEP_Init();   //��������ʼ��	 
	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	 
	MotorStop(); //ֹͣ�������ת�� 
	BEEP=1;  // �ط�����
	LED0=1;  //�ص�
	RELAY=1; //����
	 
	POINT_COLOR=RED;	 
	//sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣							
		
	 
	for( y=30;y>0;y-- )  //�ڶ�����ʾ��Ϣ��������ʾ�ܺ�ʱ30*0.1s
	{
			LCD_Clear(WHITE);  //��Ļ�ֱ���320*480
			LCD_ShowString(30,y*10,210,24,24,"ZNDTKZQ"); 
			LCD_ShowString(30,y*10+30,200,24,24,"0208");			
			delay_ms(100);				
	}
	delay_ms(1000);	  
	LCD_Clear(WHITE); //����	
   
	
	LCD_ShowString(30,90+10,240,24,24,"1x2x3x4x5x6x7x8x9x");//������ ��¼���µ�¥����Ϣ
	sprintf((char*)FloorNum,"Floor:%d",CurrentFloor);
	LCD_ShowString(30,60+10,200,24,24,FloorNum);     //�ڶ�����ʾ��ǰ¥���
		
  while(1) 
	{
		KeyCode =  keyhandle();  //����ɨ��		
		if(KeyCode > 0)
		{
			KeySetLiftUpDownValue();

		}
		
		if((GoTo5FloorFlag==1)&&(CurrentFloor!=5)&&((LiftDirection==0)) ) //�����ݿ���ʱ���е�5¥
		{
			if(CurrentFloor>5)
			{
				LiftDirection=2; //��������
			}
			else
			{
				LiftDirection=1; //��������
			}
			
			GoTo5FloorFlag = 0; 
			DesFloor = 5 ; 				//Ŀ��¥�㸳ֵ5	
			LCD_ShowNum(30,10,DesFloor,1,24);
			GoToFloor(DesFloor);  //�������е�5¥
			
		}	
		
		//-------------------���ݿ����߼�������-----------------------
		
		if((PressFloorValue>0))  //¥�㰴���б�����    LiftDirection��ֵ����     
		{			
			
			if((LiftDirection==0)&&(PressFloorValue<(0x00000001<<(CurrentFloor-1)))&&(PressFloorValue>0))    //��ǰֹͣ״̬ ��ǰ¥�������µ�¥���б����� ��������  0001 0000
			{
			LiftDirection = 2;  //����
			//LCD_ShowString(30,22,240,24,24,"Goingdown"); 
			}
			else if((LiftDirection==0)&&(PressFloorValue>(0x00000001<<(CurrentFloor-1)))&&(PressFloorValue>0))
			{
			LiftDirection = 1;  //����
			//LCD_ShowString(30,22,240,24,24,"GoingUp"); 
			}
				
		}		
		
		if((LiftDirection==2)&&(PressFloorValue>0) ) //��ǰ����״̬    DesFloor��ֵ����
			{	
				for(j=1;j<CurrentFloor;j++)
				//for(j=CurrentFloor;j>0;j--)
				{
					if(((0x00000001<<(CurrentFloor-j-1))&PressFloorValue)> 0)
					{
						 break; 
					}
				}
       			
				if(j<CurrentFloor)
				{	
					if(DesFloor>0) //�����ʱDesFloor��Ϊ0 �����豸����������
					{
						if(DesFloor < CurrentFloor-j	)//����������˵
						{
							DesFloor = CurrentFloor-j;  //����DesFloor
						}
						
					}
					else DesFloor = CurrentFloor-j;	
		
			  LCD_ShowNum(30,10,DesFloor,1,24);//��һ����ʾ ������Ϣ Ŀ��¥���
				

				}
				else   //if j==CurrentFloor �Ĵ����֧		
				{					
				 LiftDirection=0; //����ֹͣ 
				 DesFloor = 0 ; //Ŀ��¥�㸳ֵ0 				
				}
			}			
			else if ((LiftDirection==1)&&(PressFloorValue>0))  //��ǰ����״̬  DesFloor��ֵ����
			{
				for(j=0;j<(8-CurrentFloor+1);j++)
				//for(j=CurrentFloor-1;j>0;j--)
				{
					if(((0x00000001<<(CurrentFloor+j-1))&PressFloorValue)> 0)
					{
						 break; 
					}
				}
				
				if(j<(8-CurrentFloor+1))
				{	
					if(DesFloor>0) //�����ʱDesFloor��Ϊ0 �����豸����������
					{
						if(DesFloor > CurrentFloor+j	)//����������˵
						{
							DesFloor = CurrentFloor+j;  //����DesFloor
						}
						
					}
					else DesFloor = CurrentFloor+j;	
					
			
			  LCD_ShowNum(30,10,DesFloor,1,24);//��һ����ʾ ������Ϣ Ŀ��¥���
				
				}
				else   //if j==CurrentFloor �Ĵ����֧		
				{					
				 LiftDirection=0; //����ֹͣ   
				 DesFloor = 0 ; //Ŀ��¥�㸳ֵ0 				
				}

			}			
			
			
			//�������еĿ���
			if(DesFloor>0)  
			{
				if(DesFloor<CurrentFloor)
				{
					GoToFloor(CurrentFloor-1); //һ��һ�����
				}
				else if(DesFloor>CurrentFloor)
				{
					GoToFloor(CurrentFloor+1); //һ��һ�����				
				}
				else  //����
				{					
 
					LCD_ShowString(114,70,240,24,24,"="); //��ʾ����ͣ
					
					//LiftDirection=0; //����ֹͣ   
					DesFloor = 0 ; //Ŀ��¥�㸳ֵ0 

						BEEP=0;    //������ʾ   
						LED0=0;
						delay_ms(100);
						BEEP=1;  // �ط�����
						LED0=1;  //�ص�	
					
					RELAY=0;  //���ſ���
					LED0=0;
					
					delay_ms(1000); //����ά��1sʱ��
					delay_ms(1000); //����ά��1sʱ��
							
					RELAY=1; //���Źص�
					LED0=1;				
				}
			}

		//---------------------------------------------
		
		
		
	  		
		
	} 
	
	
	
	
	
	
	
}

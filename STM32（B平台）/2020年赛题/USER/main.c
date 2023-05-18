#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "motor.h"
#include "timer.h"
#include "exti.h"
#include "infrared.h"
#include "adc.h"
#include "rtc.h" 

 
/************************************************
2020�����⸴��
************************************************/


u8 CharCodeLine1[]="CZCJJQ";      //��һ����ʾ�ַ�
u8 CharCodeLine2[]="0302";        //�ڶ�����ʾ�ַ�
u8 MileValue[]="mile: km";  	  //��ʾ��ʻ���
u8 CostValue[]="cost: yuan";  	  //��ʾӦ��ȡ����
u8 WaiteTimeValue[]="delay: min"; //��ʾ�ȴ�ʱ��


float MileNum = 0; 	  //��̼���
float CostNum = 0;    //Ӧ�շ��ü���
u16 WaiteTimeNum = 0; //�ȴ�ʱ��
u16 StartPrice = 6;   //�𲽼� Ĭ�� 6
u8  TimeNum = 1;      //ʱ��1/2 Ĭ��Ϊ1
u16 ad;               //ADC����ֵ
u8  MoterSpeed =6;    //ͨ����ʱ��2���Ʋ������ת��
u8  AdcFlag = 0 ;     //adc������־
u8 KeyCode=0;	      //����ֵ
u16 InterruptCount = 0 ;   //�����жϼ���

extern u16 TimerCount ;    //��ʱ������
extern u8  PulseNum;       //�������
extern u16 WaiteTimeCount; //��ֹ״̬�ȴ�ʱ�����

 	
 int main(void)
 {	 
	u8 temp[]="0";
	u8 y,t_min=0;	
    u8 t_hour=0;
    u8 min=0;	
    u8 hour=0;	 
	static  u8  StartDrive = 0 ;  //0-ͣ��������� 1-��ʼ����Ƽ��� 2-��ֹ�ȴ�״̬
	 
	delay_init();	    	      //��ʱ������ʼ��	
    //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 
	uart_init(115200);	 	      //���ڳ�ʼ��Ϊ115200

	LCD_Init();
	KEY_Init();         //���󰴼���ʼ��   
	MOTOR_Init();  		//�������IO��ʼ��
	INFRARED_Init();   	//��ʼ������Թܵ�Ӳ���ӿ�	 
	Adc_Init();		    //ADC��ʼ��
	RTC_Init();	  	    //RTC��ʼ��
	MotorStop(); 		//ֹͣ�������ת�� 	 
	 
	POINT_COLOR=RED;	 	
	 
	for( y=30;y>0;y-- )        //������ʾ���ܺ�ʱ30*0.1s
	{
			LCD_Clear(WHITE);  //��Ļ�ֱ���320*480
			LCD_ShowString(30,y*10,210,24,24,"CZCJJQ"); 
			LCD_ShowString(30,y*10+30,200,24,24,"0302");			
			delay_ms(100);				
	}
	delay_ms(200);	  
	LCD_Clear(WHITE);         
  		
	LCD_ShowString(30,30+10,480,24,24,MileValue);  //��һ�� ��ʾ��ʻ���
	LCD_ShowString(30,60+10,480,24,24,CostValue);  //�ڶ��� ��ʾӦ��ȡ����


  while(1) 
	{
		KeyCode =  keyhandle(); //����ɨ��	
		delay_ms(10);
		
		if(KeyCode == 1) 		//S1����
		{
			KeyCode=0;
			if(StartDrive==0)   //0-�������״̬(ͣ��)
			{
				StartDrive=1;
				PulseNum =0 ;
				TimerCount = 0;				
				InterruptCount = 0; //��̼�������
				WaiteTimeCount = 0; //��ֹ�ȴ�ʱ������
				MileNum = 0; 		//��̼���
				CostNum = 0; 		//Ӧ�շ��ü���
				WaiteTimeNum = 0;   //�ȴ�ʱ��
				
				TIM2_Int_Init(24,719);//Tout= ((24+1)*( 719+1))/72=250us	
									  //����ʱ��2�ж�--��ʱ�����Ʋ������ת��
				EXTIX_Init();		  //�ⲿ�жϳ�ʼ��
			}
			else if(StartDrive==1)    //1-��ʼ����Ƽ���״̬
			{
				StartDrive=0;		  //ֹͣ����ֹͣ�Ƽ�				
				
				TIM_Cmd(TIM2, DISABLE);  //�ض�ʱ��2		
				MotorStop(); 		     //ֹͣ�������ת�� 		
			}
		}
		else if(KeyCode ==2)    //S2����
		{
			KeyCode=0;			
			if(StartDrive==1) 
			{
				StartDrive = 2; //����������״̬(ͣ��)			
				TIM_Cmd(TIM2, DISABLE);  //�ض�ʱ��2		
				MotorStop(); 		     //ֹͣ�������ת�� 
				
				TIM3_Int_Init(999,719);  //Tout= ((99+1)*( 7199+1))/72=10000us=10ms
									     //��ʱ��3��ʼ��
				
				LCD_ShowString(30,90+10,480,24,24,WaiteTimeValue);  //��3����ʾ�ȴ�ʱ��	
			}
			else if(StartDrive==2) 
			{
				StartDrive=1;	         //�˳���ֹ״̬				
				TIM_Cmd(TIM3, DISABLE);  //�ض�ʱ��3	
				delay_ms(10);	
				TIM2_Int_Init(24,719);   //Tout= ((24+1)*( 719+1))/72=250us		
			
				WaiteTimeNum=WaiteTimeCount/20; //����������1�����5���� 
			    sprintf((char*)WaiteTimeValue,"delay: %dmin",WaiteTimeNum);
				LCD_ShowString(30,90+10,480,24,24,WaiteTimeValue);  //��3����ʾ�ȴ�ʱ��	
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
		else if(KeyCode ==13)  //ʱ������ ʱ��һ
		{
			t_hour = calendar.hour;
			if(t_hour==23) t_hour = 0;
			else t_hour = calendar.hour+1;			
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,t_hour,calendar.min,calendar.sec);
			sprintf((char*)temp,"%d:%d  ",t_hour,calendar.min);
			LCD_ShowString(30,180+10,480,24,24,temp);			
		}
		else if(KeyCode ==14)  //ʱ������ ʱ��һ
		{
			t_hour = calendar.hour;
			if(t_hour==0) t_hour = 23;
			else t_hour = calendar.hour-1;			
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,t_hour,calendar.min,calendar.sec);
			sprintf((char*)temp,"%d:%d  ",t_hour,calendar.min);
			LCD_ShowString(30,180+10,480,24,24,temp);			
		}
		else if(KeyCode ==15)   //ʱ������ �ּ�һ
		{
			t_min = calendar.min;
			if(t_min==59) t_min = 0;
			else t_min = calendar.min+1;			
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,t_min,calendar.sec);
			sprintf((char*)temp,"%d:%d  ",calendar.hour,t_min);
			LCD_ShowString(30,180+10,480,24,24,temp);			
		}
		else if(KeyCode ==16)   //ʱ������ �ּ�һ
		{
			t_min = calendar.min;
			if(t_min==0) t_min = 59;
			else t_min = calendar.min-1;			
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,t_min,calendar.sec);
			sprintf((char*)temp,"%d:%d  ",calendar.hour,t_min);
			LCD_ShowString(30,180+10,480,24,24,temp);			
		}
		

		if(StartDrive==1)  //��ʼ�Ƽ� ����ʾ
			{			
				if(AdcFlag > 0) //ADC����һ��
				{
					AdcFlag = 0;
					ad=Get_Adc_Average(ADC_Channel_8,10);//�ɼ�AD ֵΪ(300-1020)*4  		
					MoterSpeed =(float)ad*0.01-7; //6-40��Χ
				}
				

#if DRIVING_SIMULATION	
			
			MileNum=PulseNum*4;     //��ʱ���������1����ʻ4km				
			sprintf((char*)MileValue,"mile: %04dkm",MileNum);	
			LCD_ShowString(30,30+10,480,24,24,MileValue);  //��һ�� ��ʾ��ʻ���
			
			CostNum=(float)(MileNum-3)*1.2+6;  //��Ϊ1�뼴����4km���Ѿ������𲽼�
			sprintf((char*)CostValue,"cost: %.2fyuan",CostNum);
			LCD_ShowString(30,60+10,480,24,24,CostValue);  //�ڶ��� ��ʾӦ��ȡ����	
		
					
#else								     //����һȦ20��1Ȧ1�ף�Ϊ��ʾ�����1000������1Ȧ1km 1��0.05km
			MileNum=InterruptCount*0.05; //����������̼��㣬Ϊ��ʾС����
			sprintf((char*)MileValue,"mile: %.2fkm",MileNum);		
			LCD_ShowString(30,30+10,480,24,24,MileValue);  //��һ�� ��ʾ��ʻ���
				
			if(MileNum<= 3)  //�𲽼�����3km
			{
				CostNum = StartPrice +(WaiteTimeNum*0.2) ; //��ʵ�۸�
			}
			else              //�����𲽼�����
			{
				              //��ʵ�۸�
				CostNum =(float)((MileNum-3)*1.2)+StartPrice+(WaiteTimeNum*0.2) ;
			}
			sprintf((char*)CostValue,"cost: %.2fyuan",CostNum);
			LCD_ShowString(30,60+10,480,24,24,CostValue);  //�ڶ��� ��ʾӦ��ȡ����		
		
#endif
			}
			else if(StartDrive==2) //��ֹ�ȴ�״̬ ѭ����ʾ�ȴ�ʱ��
			{
				WaiteTimeNum=WaiteTimeCount/20; //��1�����5����
				sprintf((char*)WaiteTimeValue,"delay: %dmin",WaiteTimeNum);
				LCD_ShowString(30,90+10,480,24,24,WaiteTimeValue);  //��3����ʾ�ȴ�ʱ��	
				
				if(MileNum<= 3)    //�𲽼�����3km
				{
					CostNum = StartPrice +(WaiteTimeNum*0.2) ;      //��ʵ�۸�
				}
				else              //�����𲽼�����
				{				  //��ʵ�۸�
					CostNum =(float)((MileNum-3)*1.2)+StartPrice+(WaiteTimeNum*0.2) ;
				}
				sprintf((char*)CostValue,"cost: %.2fyuan",CostNum);
				LCD_ShowString(30,60+10,480,24,24,CostValue);      //�ڶ��� ��ʾӦ��ȡ����	
	
			}
			
			
		if((min!=calendar.min)||(hour!=calendar.hour))             //ˢ����ʾʱ��
		{
			min=calendar.min;
			hour=calendar.hour;
			sprintf((char*)temp,"%d:%d  ",calendar.hour,calendar.min);
			LCD_ShowString(30,180+10,480,24,24,temp);
		}	
	} 	
}
 




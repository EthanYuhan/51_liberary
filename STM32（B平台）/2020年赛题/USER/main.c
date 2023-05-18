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
2020年赛题复现
************************************************/


u8 CharCodeLine1[]="CZCJJQ";      //第一行显示字符
u8 CharCodeLine2[]="0302";        //第二行显示字符
u8 MileValue[]="mile: km";  	  //显示行驶里程
u8 CostValue[]="cost: yuan";  	  //显示应收取费用
u8 WaiteTimeValue[]="delay: min"; //显示等待时间


float MileNum = 0; 	  //里程计数
float CostNum = 0;    //应收费用计数
u16 WaiteTimeNum = 0; //等待时间
u16 StartPrice = 6;   //起步价 默认 6
u8  TimeNum = 1;      //时段1/2 默认为1
u16 ad;               //ADC采样值
u8  MoterSpeed =6;    //通过定时器2控制步进电机转速
u8  AdcFlag = 0 ;     //adc采样标志
u8 KeyCode=0;	      //按键值
u16 InterruptCount = 0 ;   //码盘中断计数

extern u16 TimerCount ;    //定时器计数
extern u8  PulseNum;       //脉冲计数
extern u16 WaiteTimeCount; //静止状态等待时间计数

 	
 int main(void)
 {	 
	u8 temp[]="0";
	u8 y,t_min=0;	
    u8 t_hour=0;
    u8 min=0;	
    u8 hour=0;	 
	static  u8  StartDrive = 0 ;  //0-停车服务结束 1-开始服务计价中 2-静止等待状态
	 
	delay_init();	    	      //延时函数初始化	
    //设置NVIC中断分组2:2位抢占优先级，2位响应优先级	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 
	uart_init(115200);	 	      //串口初始化为115200

	LCD_Init();
	KEY_Init();         //矩阵按键初始化   
	MOTOR_Init();  		//步进电机IO初始化
	INFRARED_Init();   	//初始化红外对管的硬件接口	 
	Adc_Init();		    //ADC初始化
	RTC_Init();	  	    //RTC初始化
	MotorStop(); 		//停止步进电机转动 	 
	 
	POINT_COLOR=RED;	 	
	 
	for( y=30;y>0;y-- )        //滚动显示，总耗时30*0.1s
	{
			LCD_Clear(WHITE);  //屏幕分辨率320*480
			LCD_ShowString(30,y*10,210,24,24,"CZCJJQ"); 
			LCD_ShowString(30,y*10+30,200,24,24,"0302");			
			delay_ms(100);				
	}
	delay_ms(200);	  
	LCD_Clear(WHITE);         
  		
	LCD_ShowString(30,30+10,480,24,24,MileValue);  //第一行 显示行驶里程
	LCD_ShowString(30,60+10,480,24,24,CostValue);  //第二行 显示应收取费用


  while(1) 
	{
		KeyCode =  keyhandle(); //按键扫描	
		delay_ms(10);
		
		if(KeyCode == 1) 		//S1按下
		{
			KeyCode=0;
			if(StartDrive==0)   //0-服务结束状态(停车)
			{
				StartDrive=1;
				PulseNum =0 ;
				TimerCount = 0;				
				InterruptCount = 0; //里程计数清零
				WaiteTimeCount = 0; //静止等待时间清零
				MileNum = 0; 		//里程计数
				CostNum = 0; 		//应收费用计数
				WaiteTimeNum = 0;   //等待时间
				
				TIM2_Int_Init(24,719);//Tout= ((24+1)*( 719+1))/72=250us	
									  //开定时器2中断--定时器控制步进电机转动
				EXTIX_Init();		  //外部中断初始化
			}
			else if(StartDrive==1)    //1-开始服务计价中状态
			{
				StartDrive=0;		  //停止服务停止计价				
				
				TIM_Cmd(TIM2, DISABLE);  //关定时器2		
				MotorStop(); 		     //停止步进电机转动 		
			}
		}
		else if(KeyCode ==2)    //S2按下
		{
			KeyCode=0;			
			if(StartDrive==1) 
			{
				StartDrive = 2; //进入服务结束状态(停车)			
				TIM_Cmd(TIM2, DISABLE);  //关定时器2		
				MotorStop(); 		     //停止步进电机转动 
				
				TIM3_Int_Init(999,719);  //Tout= ((99+1)*( 7199+1))/72=10000us=10ms
									     //定时器3初始化
				
				LCD_ShowString(30,90+10,480,24,24,WaiteTimeValue);  //第3行显示等待时间	
			}
			else if(StartDrive==2) 
			{
				StartDrive=1;	         //退出静止状态				
				TIM_Cmd(TIM3, DISABLE);  //关定时器3	
				delay_ms(10);	
				TIM2_Int_Init(24,719);   //Tout= ((24+1)*( 719+1))/72=250us		
			
				WaiteTimeNum=WaiteTimeCount/20; //分钟数，用1秒代替5分钟 
			    sprintf((char*)WaiteTimeValue,"delay: %dmin",WaiteTimeNum);
				LCD_ShowString(30,90+10,480,24,24,WaiteTimeValue);  //第3行显示等待时间	
			}
			
		}
		else if(KeyCode ==3) //S3按下切换时段
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
		else if(KeyCode ==13)  //时钟设置 时加一
		{
			t_hour = calendar.hour;
			if(t_hour==23) t_hour = 0;
			else t_hour = calendar.hour+1;			
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,t_hour,calendar.min,calendar.sec);
			sprintf((char*)temp,"%d:%d  ",t_hour,calendar.min);
			LCD_ShowString(30,180+10,480,24,24,temp);			
		}
		else if(KeyCode ==14)  //时钟设置 时减一
		{
			t_hour = calendar.hour;
			if(t_hour==0) t_hour = 23;
			else t_hour = calendar.hour-1;			
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,t_hour,calendar.min,calendar.sec);
			sprintf((char*)temp,"%d:%d  ",t_hour,calendar.min);
			LCD_ShowString(30,180+10,480,24,24,temp);			
		}
		else if(KeyCode ==15)   //时钟设置 分加一
		{
			t_min = calendar.min;
			if(t_min==59) t_min = 0;
			else t_min = calendar.min+1;			
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,t_min,calendar.sec);
			sprintf((char*)temp,"%d:%d  ",calendar.hour,t_min);
			LCD_ShowString(30,180+10,480,24,24,temp);			
		}
		else if(KeyCode ==16)   //时钟设置 分减一
		{
			t_min = calendar.min;
			if(t_min==0) t_min = 59;
			else t_min = calendar.min-1;			
			RTC_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,t_min,calendar.sec);
			sprintf((char*)temp,"%d:%d  ",calendar.hour,t_min);
			LCD_ShowString(30,180+10,480,24,24,temp);			
		}
		

		if(StartDrive==1)  //开始计价 并显示
			{			
				if(AdcFlag > 0) //ADC采样一次
				{
					AdcFlag = 0;
					ad=Get_Adc_Average(ADC_Channel_8,10);//采集AD 值为(300-1020)*4  		
					MoterSpeed =(float)ad*0.01-7; //6-40范围
				}
				

#if DRIVING_SIMULATION	
			
			MileNum=PulseNum*4;     //定时脉冲情况下1秒行驶4km				
			sprintf((char*)MileValue,"mile: %04dkm",MileNum);	
			LCD_ShowString(30,30+10,480,24,24,MileValue);  //第一行 显示行驶里程
			
			CostNum=(float)(MileNum-3)*1.2+6;  //因为1秒即运行4km，已经超过起步价
			sprintf((char*)CostValue,"cost: %.2fyuan",CostNum);
			LCD_ShowString(30,60+10,480,24,24,CostValue);  //第二行 显示应收取费用	
		
					
#else								     //码盘一圈20格，1圈1米，为演示方便乘1000倍，即1圈1km 1格0.05km
			MileNum=InterruptCount*0.05; //步进电机码盘计算，为显示小数点
			sprintf((char*)MileValue,"mile: %.2fkm",MileNum);		
			LCD_ShowString(30,30+10,480,24,24,MileValue);  //第一行 显示行驶里程
				
			if(MileNum<= 3)  //起步价区间3km
			{
				CostNum = StartPrice +(WaiteTimeNum*0.2) ; //真实价格
			}
			else              //超出起步价区间
			{
				              //真实价格
				CostNum =(float)((MileNum-3)*1.2)+StartPrice+(WaiteTimeNum*0.2) ;
			}
			sprintf((char*)CostValue,"cost: %.2fyuan",CostNum);
			LCD_ShowString(30,60+10,480,24,24,CostValue);  //第二行 显示应收取费用		
		
#endif
			}
			else if(StartDrive==2) //静止等待状态 循环显示等待时间
			{
				WaiteTimeNum=WaiteTimeCount/20; //用1秒代替5分钟
				sprintf((char*)WaiteTimeValue,"delay: %dmin",WaiteTimeNum);
				LCD_ShowString(30,90+10,480,24,24,WaiteTimeValue);  //第3行显示等待时间	
				
				if(MileNum<= 3)    //起步价区间3km
				{
					CostNum = StartPrice +(WaiteTimeNum*0.2) ;      //真实价格
				}
				else              //超出起步价区间
				{				  //真实价格
					CostNum =(float)((MileNum-3)*1.2)+StartPrice+(WaiteTimeNum*0.2) ;
				}
				sprintf((char*)CostValue,"cost: %.2fyuan",CostNum);
				LCD_ShowString(30,60+10,480,24,24,CostValue);      //第二行 显示应收取费用	
	
			}
			
			
		if((min!=calendar.min)||(hour!=calendar.hour))             //刷新显示时间
		{
			min=calendar.min;
			hour=calendar.hour;
			sprintf((char*)temp,"%d:%d  ",calendar.hour,calendar.min);
			LCD_ShowString(30,180+10,480,24,24,temp);
		}	
	} 	
}
 




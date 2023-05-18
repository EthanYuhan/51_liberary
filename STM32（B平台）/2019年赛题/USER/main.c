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
2019年赛题复现
************************************************/



u8 FloorNum[]="Floor:1";       //默认在1楼
u8 PRESSFloorNum[]="1x2x3x4x5x6x7x8x9x"; //被按下显示的楼层号 

										    // 8 7 6 5 4 3 2 1   楼
u8 PressFloorValue = 0; // 0 0 0 0 0 0 0 0      0未按下，1按下    //被按下的楼号
u8 Floor9Value = 0;    //是否有人按九楼
u8 CurrentFloor = 1 ;  //当前楼层号，默认在5楼
u8 LiftDirection = 0;  //0停止 1上升 2下降
u8 DesFloor = 0;  //目的地楼层 0为无效值
u8 KeyCode=0;	//按键值

extern u16 TimerCount ; //定时器计数
extern u8 GoTo5FloorFlag;  //电梯是否空闲去5楼标志位



void KeySetLiftUpDownValue(void)  //更新PressFloorValue并显示被按下的楼层号信息
{
	//u8 temp[]="0";	
	if(KeyCode >0)
	{
		TimerCount = 0;   //有按键被按下，定时器就重新开始计数
	}	
	
	if(KeyCode ==1)	
		{			
			PressFloorValue|=0x00000001;	
      LCD_ShowString(30+12,90+10,240,24,24,"o"); //每个字符占12的宽度			
//			sprintf((char*)temp,"PressFloorValue is:%d",PressFloorValue);
//			LCD_ShowString(30,120+10,240,24,24,temp);
		}
		else if(KeyCode ==2)	
		{
			PressFloorValue|=0x00000002;		
      LCD_ShowString(30+12*3,90+10,240,24,24,"o"); //每个字符占12的宽度			
		}
		else if(KeyCode ==3)	
		{
			PressFloorValue|=0x00000004;	
		  LCD_ShowString(30+12*5,90+10,240,24,24,"o"); //每个字符占12的宽度	
		}
		else if(KeyCode ==4)	
		{
		  PressFloorValue|=0x00000008;	
			LCD_ShowString(30+12*7,90+10,240,24,24,"o"); //每个字符占12的宽度	
		}
		else if(KeyCode ==5)	
		{
			PressFloorValue|=0x00000010;
    	LCD_ShowString(30+12*9,90+10,240,24,24,"o"); //每个字符占12的宽度		
		}
		else if(KeyCode ==6)	
		{
			PressFloorValue|=0x00000020;	
			LCD_ShowString(30+12*11,90+10,240,24,24,"o"); //每个字符占12的宽度	
		}
		else if(KeyCode ==7)	
		{			
			PressFloorValue|=0x00000040;
		  LCD_ShowString(30+12*13,90+10,240,24,24,"o"); //每个字符占12的宽度	
		}
		else if(KeyCode ==8)	
		{
			PressFloorValue|=0x00000080;
	    LCD_ShowString(30+12*15,90+10,240,24,24,"o"); //每个字符占12的宽度	
		}
		else if(KeyCode ==9)	//有人按九楼
		{
			PressFloorValue|=0x00000100;
		  LCD_ShowString(30+12*17,90+10,240,24,24,"o"); //每个字符占12的宽度	
		}
	
		KeyCode=0;

}


void GoToFloor(u8 num)   //从当前楼层抵达num指定楼层
{
  u8 i,n;	
	
	BEEP=0;    //声光提示  //运行的声光提示，每层运行时都提示！
	LED0=0;
  delay_ms(100);
	BEEP=1;  // 关蜂鸣器
	LED0=1;  //关灯	
	
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

	  PressFloorValue&=(~(0x00000001<<(num-1))); //清除目的地楼层被按下信息	
	  LCD_ShowString(18+num*24,90+10,240,24,24,"x");	
	
		CurrentFloor = num; //更新当前楼层
	  //LiftDirection=0;  //这里不能设置为停止状态，只有一个方向到底并没有楼层被按下的时候

}


 	
 int main(void)
 {	 
 	u8 x=0;
	u8 y,i,j,n;          //tft显示竖坐标
	u8 temp[]="0";
	 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	KEY_Init();          //矩阵按键初始化   
	MOTOR_Init();  //步进电机IO初始化
  RELAY_Init();  //继电器初始化
  BEEP_Init();   //蜂鸣器初始化	 
	TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
	 
	MotorStop(); //停止步进电机转动 
	BEEP=1;  // 关蜂鸣器
	LED0=1;  //关灯
	RELAY=1; //关门
	 
	POINT_COLOR=RED;	 
	//sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。							
		
	 
	for( y=30;y>0;y-- )  //第二行显示信息，滚动显示总耗时30*0.1s
	{
			LCD_Clear(WHITE);  //屏幕分辨率320*480
			LCD_ShowString(30,y*10,210,24,24,"ZNDTKZQ"); 
			LCD_ShowString(30,y*10+30,200,24,24,"0208");			
			delay_ms(100);				
	}
	delay_ms(1000);	  
	LCD_Clear(WHITE); //清屏	
   
	
	LCD_ShowString(30,90+10,240,24,24,"1x2x3x4x5x6x7x8x9x");//第三行 记录按下的楼层信息
	sprintf((char*)FloorNum,"Floor:%d",CurrentFloor);
	LCD_ShowString(30,60+10,200,24,24,FloorNum);     //第二行显示当前楼层号
		
  while(1) 
	{
		KeyCode =  keyhandle();  //按键扫描		
		if(KeyCode > 0)
		{
			KeySetLiftUpDownValue();

		}
		
		if((GoTo5FloorFlag==1)&&(CurrentFloor!=5)&&((LiftDirection==0)) ) //当电梯空闲时运行到5楼
		{
			if(CurrentFloor>5)
			{
				LiftDirection=2; //电梯下行
			}
			else
			{
				LiftDirection=1; //电梯上行
			}
			
			GoTo5FloorFlag = 0; 
			DesFloor = 5 ; 				//目标楼层赋值5	
			LCD_ShowNum(30,10,DesFloor,1,24);
			GoToFloor(DesFloor);  //电梯运行到5楼
			
		}	
		
		//-------------------电梯控制逻辑处理部分-----------------------
		
		if((PressFloorValue>0))  //楼层按键有被按下    LiftDirection赋值策略     
		{			
			
			if((LiftDirection==0)&&(PressFloorValue<(0x00000001<<(CurrentFloor-1)))&&(PressFloorValue>0))    //当前停止状态 当前楼层数以下的楼层有被按下 优先下行  0001 0000
			{
			LiftDirection = 2;  //下行
			//LCD_ShowString(30,22,240,24,24,"Goingdown"); 
			}
			else if((LiftDirection==0)&&(PressFloorValue>(0x00000001<<(CurrentFloor-1)))&&(PressFloorValue>0))
			{
			LiftDirection = 1;  //上行
			//LCD_ShowString(30,22,240,24,24,"GoingUp"); 
			}
				
		}		
		
		if((LiftDirection==2)&&(PressFloorValue>0) ) //当前下行状态    DesFloor赋值策略
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
					if(DesFloor>0) //如果此时DesFloor不为0 代表设备正在运行中
					{
						if(DesFloor < CurrentFloor-j	)//对于下行来说
						{
							DesFloor = CurrentFloor-j;  //更新DesFloor
						}
						
					}
					else DesFloor = CurrentFloor-j;	
		
			  LCD_ShowNum(30,10,DesFloor,1,24);//第一行显示 调试信息 目的楼层号
				

				}
				else   //if j==CurrentFloor 的处理分支		
				{					
				 LiftDirection=0; //电梯停止 
				 DesFloor = 0 ; //目标楼层赋值0 				
				}
			}			
			else if ((LiftDirection==1)&&(PressFloorValue>0))  //当前上行状态  DesFloor赋值策略
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
					if(DesFloor>0) //如果此时DesFloor不为0 代表设备正在运行中
					{
						if(DesFloor > CurrentFloor+j	)//对于下行来说
						{
							DesFloor = CurrentFloor+j;  //更新DesFloor
						}
						
					}
					else DesFloor = CurrentFloor+j;	
					
			
			  LCD_ShowNum(30,10,DesFloor,1,24);//第一行显示 调试信息 目的楼层号
				
				}
				else   //if j==CurrentFloor 的处理分支		
				{					
				 LiftDirection=0; //电梯停止   
				 DesFloor = 0 ; //目标楼层赋值0 				
				}

			}			
			
			
			//电梯运行的控制
			if(DesFloor>0)  
			{
				if(DesFloor<CurrentFloor)
				{
					GoToFloor(CurrentFloor-1); //一层一层控制
				}
				else if(DesFloor>CurrentFloor)
				{
					GoToFloor(CurrentFloor+1); //一层一层控制				
				}
				else  //到达
				{					
 
					LCD_ShowString(114,70,240,24,24,"="); //提示电梯停
					
					//LiftDirection=0; //电梯停止   
					DesFloor = 0 ; //目标楼层赋值0 

						BEEP=0;    //声光提示   
						LED0=0;
						delay_ms(100);
						BEEP=1;  // 关蜂鸣器
						LED0=1;  //关灯	
					
					RELAY=0;  //开门开灯
					LED0=0;
					
					delay_ms(1000); //开门维持1s时间
					delay_ms(1000); //开门维持1s时间
							
					RELAY=1; //关门关灯
					LED0=1;				
				}
			}

		//---------------------------------------------
		
		
		
	  		
		
	} 
	
	
	
	
	
	
	
}

#include "stc15f2k60s2.h"
#include <intrins.h>
#include "sys.h"
#include "lcd12864.h"
#include "key.h"
#include "motor.h"
#include "timer.h"

u8 code CharCodeLine1[]="ZNDTKZQ";  //第一行显示字符
u8 code CharCodeLine2[]="0208";     //第二行显示字符
u8 code FloorNum[]="Floor:5";       //默认在5楼
u8 code PRESSFloorNum[]="1x2x3x4x5x6x7x8x"; //被按下显示的楼层号 
u8 code PRESS9FloorNum[]="9x";

										    // 8 7 6 5 4 3 2 1   楼
u8 PressFloorValue = 0; // 0 0 0 0 0 0 0 0      0未按下，1按下    //被按下的楼号
u8 Floor9Value = 0;    //是否有人按九楼
u8 CurrentFloor = 1 ;  //当前楼层号，默认在5楼
u8 LiftDirection = 2;  //0停止 1上升 2下降
u8 DesFloor = 0;  //目的地楼层 0为无效值

extern u8 KeyCode;  //按键键值
extern u8 speed;    //步进电机速度
extern u16 TimerCount ; //定时器计数
extern u8 GoTo5FloorFlag;  //电梯是否空闲去5楼标志位


sbit beep=P5^5;		// 蜂鸣器定义
sbit led=P5^3;	
sbit relay=P5^2;



void IO_init(void)  //IO口初始化
{
	P0M0 = 0X00;	P0M1 = 0X00;  //IO配置为准双向IO口 
	P1M0 = 0X00;	P1M1 = 0X00;
	P2M0 = 0X00;	P2M1 = 0X00;
	P3M0 = 0X00;	P3M1 = 0X00;
	P4M0 = 0X00;	P4M1 = 0X00;  	
	P5M0 = 0X00;	P5M1 = 0X00;  	
	P6M0 = 0X00;	P6M1 = 0X00;  	
	P7M0 = 0X00;	P7M1 = 0X00;  
	
	//设置推挽输出
	P2M1 &= ~(1<<2),P2M0 |=  (1<<2); //12864LCD设置
	P2M1 &= ~(1<<1),P2M0 |=  (1<<1); 
	P2M1 &= ~(1<<0),P2M0 |=  (1<<0); 
	P1M1 &= ~(1<<3),P1M0 |=  (1<<3); //步进电机
	P1M1 &= ~(1<<4),P1M0 |=  (1<<4); 
	P1M1 &= ~(1<<1),P1M0 |=  (1<<1); 
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
  P5M1 &= ~(1<<5),P5M0 |=  (1<<5); //蜂鸣器
	P5M1 &= ~(1<<2),P5M0 |=  (1<<2); //继电器
}


void Roll(void)  //卷动显示两行字
{	
	uchar i;
	
	//为显示上半屏第一行字符做准备，地址0xa0  详细参考文章：https://wenku.baidu.com/view/375ec764cc22bcd127ff0c9a.html	
	LCD12864_WriteCmd(0xa0);	
	while(CharCodeLine1[i]!='\0')
	{
		LCD12864_WriteData(CharCodeLine1[i]);
		i++;
	}
	i=0;
  //为显示上半屏第二行字符做准备，地址0xb0  	
	LCD12864_WriteCmd(0xb0);    
	while(CharCodeLine2[i]!='\0')
	{
		LCD12864_WriteData(CharCodeLine2[i]);
		i++;
	}
	
		for(i=0;i<33;i++)   //上半屏卷动显示
	{
			LCD12864_VerticalRoll(i);
			Delay100ms();	   //每行高16个像素，两行32像素，0.1秒卷动1像素，两行字显示结束共需要3.2秒
	}
	
	LCD12864_WriteCmd(0x30);	 //恢复基本指令集

}

void KeySetLiftUpDownValue(void)  //更新PressFloorValue并显示被按下的楼层号信息
{
	if(KeyCode >0)
	{
		TimerCount = 0;   //有按键被按下，定时器就重新开始计数
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
		else if(KeyCode ==9)	//有人按九楼
		{
			Floor9Value = 1;	
			LCD12864_WriteCmd(0x98);  
			LCD12864_WriteData('9');
		  LCD12864_WriteData('o');
		}
	
		KeyCode=0;

}

void GoToFloor(u8 num)   //从当前楼层抵达num指定楼层
{
  u8 i,n;	
	
	beep=0;   //声光提示  //运行的声光提示，每层运行时都提示！
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
				LCD12864_WriteCmd(0x93);  //显示当前楼层数  
				CurrentFloor--;
		    LCD12864_WriteData(CurrentFloor+0x30);  
			}
	}
	else 
	{
		for(i=0;i<(num-n);i++)
			{
				MotorCWRound(1);
				LCD12864_WriteCmd(0x93);  //显示当前楼层数  
				CurrentFloor++;
		    LCD12864_WriteData(CurrentFloor+0x30);  
			}			
	
	}						

	  PressFloorValue&=(~(0x01<<(num-1))); //清除目的地楼层被按下信息  	
	  LCD12864_SetWindow(0,0);
	  LCD12864_WriteData(PressFloorValue+0x30); //调试信息
	  LCD12864_WriteCmd(0x87+num);       //并更新显示
		LCD12864_WriteData(num+0x30);       
		LCD12864_WriteData('x');
	
	
		CurrentFloor = num; //更新当前楼层
	  //LiftDirection=0;  //这里不能设置为停止状态，只有一个方向到底并没有楼层被按下的时候

}


void main(void)  //主函数
{	
	u8 i,n,j;	

	IO_init();				 //IO口初始化		
  LCD12864_Init();   //12864液晶初始化		
	MotorStop(); 			 //停止步进电机
	beep=1;   				 //关蜂鸣器			
	relay=1;           //关门
	led=1;             //关灯
	
	Roll();            //卷动显示两行字
	Delay100ms();	
	LCD12864_WriteCmd(0x01); //清除LCD12864的显示内容	
	
	LCD12864_SetWindow(1,0); //第二行显示当前楼层号
	while(FloorNum[i]!='\0')
	{
		LCD12864_WriteData(FloorNum[i]);
		i++;
	}
	i=0;	
	
	LCD12864_WriteCmd(0x88); //第三行显示被按下的楼层号 初始值显示
	while(PRESSFloorNum[i]!='\0')  
	{
		LCD12864_WriteData(PRESSFloorNum[i]);
		i++;
	}
	i=0;
	LCD12864_WriteCmd(0x98); //第四行显示九楼是否被按下的楼层号  
	LCD12864_WriteData('9');
	LCD12864_WriteData('x');
	
	Timer0Init();   //定时器初始化并开启中断    

  LCD12864_WriteCmd(0x93);  //显示当前楼层数  	   
	LCD12864_WriteData(CurrentFloor+0x30);  
	
  	
	while(1)  
	{

			for (i=0;i<16;i++)    //直接检测16次按键
			{
				IO_KeyScan();		         //按键扫描 检测按键 	
		   		KeySetLiftUpDownValue(); //更新PressFloorValue并显示被按下的楼层号信息		xo显示
			}	
		
		// if((GoTo5FloorFlag==1)&&(CurrentFloor!=5)&&((LiftDirection==0)) ) //当电梯空闲时运行到5楼
		// {
		// 	if(CurrentFloor>5)
		// 	{
		// 		LiftDirection=2; //电梯下行
		// 	}
		// 	else
		// 	{
		// 		LiftDirection=1; //电梯上行
		// 	}
			
		// 	GoTo5FloorFlag = 0; 
		// 	DesFloor = 5 ; 				//目标楼层赋值5	
		// 	LCD12864_SetWindow(0,0);//调试信息				
		// 	LCD12864_WriteData(DesFloor+0x30);//第一行显示 调试信息 目的楼层号				
		// 	GoToFloor(DesFloor);  //电梯运行到5楼
			
		// }		
		
		
		// //-------------------电梯控制逻辑处理部分-----------------------
		
		// if((PressFloorValue>0)||(Floor9Value>0) )  //楼层按键有被按下    LiftDirection赋值策略     
		// {			
			
		// 	if((LiftDirection==0)&&(PressFloorValue<(0x01<<(CurrentFloor-1)))&&(PressFloorValue>0))    //当前停止状态 当前楼层数以下的楼层有被按下 优先下行  0001 0000
		// 	{
		// 	LiftDirection = 2;  //下行
		// 	LCD12864_SetWindow(0,0);//调试信息
		// 	LCD12864_WriteData('D');	
		// 	}
		// 	else if((LiftDirection==0)&&(PressFloorValue>(0x01<<(CurrentFloor-1)))&&(PressFloorValue>0))
		// 	{
		// 	LiftDirection = 1;  //上行
		// 	LCD12864_SetWindow(0,0);//调试信息
		// 	LCD12864_WriteData('U');
		// 	}
				
		// }		
		
		// 	if((LiftDirection==2)&&(PressFloorValue>0) ) //当前下行状态    DesFloor赋值策略
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
		// 			if(DesFloor>0) //如果此时DesFloor不为0 代表设备正在运行中
		// 			{
		// 				if(DesFloor < CurrentFloor-j	)//对于下行来说
		// 				{
		// 					DesFloor = CurrentFloor-j;  //更新DesFloor
		// 				}
						
		// 			}
		// 			else DesFloor = CurrentFloor-j;	
					
		// 		LCD12864_SetWindow(0,0);//调试信息				
		// 	  LCD12864_WriteData(DesFloor+0x30);//第一行显示 调试信息 目的楼层号
				
		// 		//GoToFloor(DesFloor);   //拿出去 单独一层一层楼控制
		// 		}
		// 		else   //if j==CurrentFloor 的处理分支		
		// 		{					
		// 		 LiftDirection=0; //电梯停止 
		// 		 DesFloor = 0 ; //目标楼层赋值0 				
		// 		}
		// 	}			
		// 	else if ((LiftDirection==1)&&(PressFloorValue>0))  //当前上行状态  DesFloor赋值策略
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
		// 			if(DesFloor>0) //如果此时DesFloor不为0 代表设备正在运行中
		// 			{
		// 				if(DesFloor > CurrentFloor+j	)//对于下行来说
		// 				{
		// 					DesFloor = CurrentFloor+j;  //更新DesFloor
		// 				}
						
		// 			}
		// 			else DesFloor = CurrentFloor+j;	
					
		// 		LCD12864_SetWindow(0,0);//调试信息				
		// 	  LCD12864_WriteData(DesFloor+0x30);//第一行显示 调试信息 目的楼层号
				
		// 		}
		// 		else   //if j==CurrentFloor 的处理分支		
		// 		{					
		// 		 LiftDirection=0; //电梯停止   
		// 		 DesFloor = 0 ; //目标楼层赋值0 				
		// 		}

		// 	}			
			
			
		// 	//电梯运行的控制
		// 	if(DesFloor>0)  
		// 	{
		// 		if(DesFloor<CurrentFloor)
		// 		{
		// 			GoToFloor(CurrentFloor-1); //一层一层控制
		// 		}
		// 		else if(DesFloor>CurrentFloor)
		// 		{
		// 			GoToFloor(CurrentFloor+1); //一层一层控制				
		// 		}
		// 		else  //到达
		// 		{					
		// 			LCD12864_WriteCmd(0x94);  
		// 			LCD12864_WriteData('=');  //提示电梯停
					
		// 			//LiftDirection=0; //电梯停止   
		// 			DesFloor = 0 ; //目标楼层赋值0 

		// 			beep=0;   //声光提示
		// 			led=0;
		// 			Delay100ms();
		// 			beep=1;   
		// 			led=1;			
		// 			relay=0;  //开门开灯
		// 			led=0;
					
		// 			delay_ms(1000); //开门维持1s时间
		// 			delay_ms(1000); //开门维持1s时间
							
		// 			relay=1; //关门关灯
		// 			led=1;				
		// 		}
		// 	}

		// //---------------------------------------------
		

	
	}
	
}


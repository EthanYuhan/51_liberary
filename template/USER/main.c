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

u8 code CharCodeLine1[]="CZCJJQ";       //第一行显示字符
u8 code CharCodeLine2[]="0302";         //第二行显示字符
u8 code MileValue[]="mile：       km";  //显示行驶里程
u8 code CostValue[]="cost：     yuan";  //显示应收取费用
u8 code WaiteTimeValue[]="delay ：    min"; //显示应收取费用

u8  StartDrive = 0 ;  //0-停车服务结束 1-开始服务计价中 2-静止等待状态
u16 MileNum = 0; 	  //里程计数
u16 CostNum = 0;      //应收费用计数
u16 WaiteTimeNum = 0; //等待时间
u16 StartPrice = 6;   //起步价 默认 6
u8  TimeNum = 1;      //时段1/2 默认为1
u16 ad;               //ADC采样值
u8  MoterSpeed =8;    //通过定时器2控制步进电机转速
u8  AdcFlag = 0 ;     //adc采样标志
u16 InterruptCount = 0 ;  //码盘中断计数

extern u8 KeyCode;      //按键键值
extern u16 TimerCount ; //定时器计数
extern u8 PulseNum;     //脉冲计数
extern u16 WaiteTimeCount; //静止状态等待时间计数

void IO_init(void){  //IO口初始化
	//IO配置为准双向IO口 
	P0M0 = 0X00;	P0M1 = 0X00;  
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


void main(void){  //主函数
	u8 i;	
	u8 temp;
	
	UpdateTemp();//获取温湿度

	IO_init();				 //IO口初始化		
    LCD12864_Init();         //12864液晶初始化	
	Give(0x00); 			 //停止步进电机
	
	Roll();          	     //卷动显示两行字	
	Delay100ms();	
	
	LCD12864_WriteCmd(0x01); //清除LCD12864的显示内容	
	LCD12864_SetWindow(0,0); //第1行显示行驶里程
	while(MileValue[i]!='\0'){
		LCD12864_WriteData(MileValue[i]);
		i++;
	}
	i=0;	
	LCD12864_SetWindow(1,0); //第2行显示应收取费用
	while(CostValue[i]!='\0'){
		LCD12864_WriteData(CostValue[i]);
		i++;
	}
	i=0;
  	
	while(1){  //主循环
		IO_KeyScan();   //按键扫描 检测按键 
		
		ds1302_scan();  //RTC时间显示 时分
		delay1ms(10);   //延时
	   
		if(KeyCode ==1)	//S1按下（矩阵键盘第一键）
		{
			KeyCode=0;				
			if(StartDrive==0)  //0-服务结束状态(停车)
			{
				StartDrive=1;
				PulseNum =0 ;
				TimerCount = 0;				
				InterruptCount = 0; //里程计数清零
				WaiteTimeCount = 0; //静止等待时间清零
				MileNum = 0; 		//里程计数
				CostNum = 0; 		//应收费用计数
				WaiteTimeNum = 0;   //等待时间
				
				Timer2Init(); //定时器2初始化				
				IE2 |=0x04;	  //开定时器2中断--定时器控制步进电机转动
				IT1 =1;		  //设置INT1的中断类型（1：仅下降沿 0：上升沿货下降沿）
				EX1 =1;       //使能INT1中断 		
				EA  =1;       //开总中断 
			}
			else if(StartDrive==1) //1-开始服务计价中状态
			{
				StartDrive=0;//停止服务停止计价				
				IE2 &=~0x04; //关定时器2中断
				EA  =0;	     //关总中断						
				Give(0x00);  //停止步进电机		
			}		
		}
		else if(KeyCode ==2) //S2按下
		{
			KeyCode=0;			
			if(StartDrive==1) 
			{
				StartDrive=2; //进入服务结束状态(停车)			
				IE2 &=~0x04;  //关定时器2中断								
				Give(0x00);   //停止步进电机	
					
				Timer0Init(); //定时器0初始化
				ET0 =1;		  //使能定时器中断	
				TR0 =1;		  //定时器0开始计时--计算等待时间
				EA  =1;	      //开总中断				

				LCD12864_SetWindow(2,0); //第3行显示等待时间
				while(WaiteTimeValue[i]!='\0')  
				{
					LCD12864_WriteData(WaiteTimeValue[i]);
					i++;
				}
				i=0;			
			}
			else if(StartDrive==2) 
			{
				StartDrive=1;	//退出静止状态				
				ET0 =0;			//关 定时器中断	
			    TR0 =0;			//关 定时器0开始计时 				
				IE2 |=0x04;	    //开定时器2中断--定时器控制步进电机转动				

				LCD12864_SetWindow(2,4);//第3行----显示等待时间
				WaiteTimeNum=WaiteTimeCount/20; //分钟数，用1秒代替5分钟 
				LCD12864_WriteData(WaiteTimeNum/1000%10+0x30);//千位
				LCD12864_WriteData(WaiteTimeNum/100%10+0x30);				
				LCD12864_WriteData(WaiteTimeNum/10%10+0x30);
				LCD12864_WriteData(WaiteTimeNum%10+0x30);
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
		
		else if(KeyCode ==13) //RTC时间调整  时加一
		{
			KeyCode=0;	
	        temp = bcd_read(0x85);//时				
			if(temp==23) temp=0;
			else temp=temp+1;
			temp = temp/10*16+temp%10;
			Write_Ds1302(0x8E,0X00);   //写使能	
			Write_Ds1302(0x84,temp);	
			Write_Ds1302(0x8E,0x80);   //写禁止
		}
		else if(KeyCode ==14) //RTC时间调整 时减一
		{
			KeyCode=0;
			temp = bcd_read(0x85);//时			
			if(temp==0) temp=23;
			else temp=temp-1;
			temp = temp/10*16+temp%10;
			Write_Ds1302(0x8E,0X00);   //写使能	
			Write_Ds1302(0x84,temp);	
			Write_Ds1302(0x8E,0x80);   //写禁止
		}
		else if(KeyCode ==15) //RTC时间调整  分加一
		{
			KeyCode=0;
			temp = bcd_read(0x83);//分				
			if(temp==59) temp=0;
			else temp=temp+1;
			temp = temp/10*16+temp%10;
			Write_Ds1302(0x8E,0X00);   //写使能	
			Write_Ds1302(0x82,temp);	
			Write_Ds1302(0x8E,0x80);   //写禁止			
		}
		else if(KeyCode ==16) //RTC时间调整 分减一
		{
			KeyCode=0;
			temp = bcd_read(0x83);//分			
			if(temp==0) temp=59;
			else temp=temp-1;
			temp = temp/10*16+temp%10;
			Write_Ds1302(0x8E,0X00);   //写使能	
			Write_Ds1302(0x82,temp);	
			Write_Ds1302(0x8E,0x80);   //写禁止
		}
		
		
		
		if(StartDrive==1)  //开始计价 并显示
		{
			
			if(AdcFlag > 0) //ADC采样一次
			{
				AdcFlag = 0;
				ad=GetADCResult(0);			   //采集AD 值为300-1020			
			    MoterSpeed =(float)ad*0.05-13; //4-40范围
			}
				

#if DRIVING_SIMULATION	
			
			MileNum=PulseNum*4;     //定时脉冲情况下1秒行驶4km
			LCD12864_SetWindow(0,3);//第1行	显示里程
			LCD12864_WriteData(MileNum/10000%10+0x30);
			LCD12864_WriteData(MileNum/1000%10+0x30); //千位
			LCD12864_WriteData(MileNum/100%10+0x30);			
			LCD12864_WriteData(MileNum/10%10+0x30);			
			LCD12864_WriteData(MileNum%10+0x30);
			
			CostNum=(MileNum-3)*12+60;  //因为1秒即运行4km，已经超过起步价，此价格为10倍真实价格
			LCD12864_SetWindow(1,3);    //第2行	显示应收取费用	
			LCD12864_WriteData(CostNum/1000%10+0x30);//千位
			LCD12864_WriteData(CostNum/100%10+0x30);			
			LCD12864_WriteData(CostNum/10%10+0x30);	
			LCD12864_WriteData('.');
			LCD12864_WriteData(CostNum%10+0x30);
					
#else								  //码盘一圈20格，1圈1米，为演示方便乘1000倍，即1圈1km 1格0.05km
			MileNum=InterruptCount*5; //步进电机码盘计算，为显示小数点 乘100倍 									  
			LCD12864_SetWindow(0,3);  //第1行显示里程
			LCD12864_WriteData(MileNum/10000%10+0x30);
			LCD12864_WriteData(MileNum/1000%10+0x30);//千位
			LCD12864_WriteData(MileNum/100%10+0x30);	
			LCD12864_WriteData('.');                 //两位小数点
			LCD12864_WriteData(MileNum/10%10+0x30);			
			LCD12864_WriteData(MileNum%10+0x30);
			
			if(MileNum<= 300)  //乘100倍，起步价区间3km
			{
				CostNum = (StartPrice*100) +(WaiteTimeNum*20) ; //真实价格100倍
			}
			else              //超出起步价区间
			{
				              //真实价格100倍
				CostNum =(float)((MileNum-300)*1.2)+(StartPrice*100)+(WaiteTimeNum*20) ;
			}			
			LCD12864_SetWindow(1,3);//第2行	显示应收取费用   	
			LCD12864_WriteData(CostNum/10000%10+0x30);
			LCD12864_WriteData(CostNum/1000%10+0x30);//千位
			LCD12864_WriteData(CostNum/100%10+0x30);	
			LCD12864_WriteData('.');			
			LCD12864_WriteData(CostNum/10%10+0x30);	
#endif

		}
		else if(StartDrive==2) //静止等待状态 循环显示等待时间
		{
			LCD12864_SetWindow(2,4);        //第3行显示等待时间
			WaiteTimeNum=WaiteTimeCount/20; //用1秒代替5分钟
			LCD12864_WriteData(WaiteTimeNum/1000%10+0x30);//千位
			LCD12864_WriteData(WaiteTimeNum/100%10+0x30);				
			LCD12864_WriteData(WaiteTimeNum/10%10+0x30);
			LCD12864_WriteData(WaiteTimeNum%10+0x30);
			
			
			//显示总金额
			if(MileNum<= 300)  //乘100倍  起步价区间3km
			{
				CostNum = (StartPrice*100) +(WaiteTimeNum*20) ; //真实价格100倍
			}
			else              //超出起步价区间
			{
				              //真实价格100倍
				CostNum =(float)((MileNum-300)*1.2)+(StartPrice*100)+(WaiteTimeNum*20) ;
			}			
			LCD12864_SetWindow(1,3);//第2行	显示应收取费用   	
			LCD12864_WriteData(CostNum/10000%10+0x30);
			LCD12864_WriteData(CostNum/1000%10+0x30);//千位
			LCD12864_WriteData(CostNum/100%10+0x30);	
			LCD12864_WriteData('.');			
			LCD12864_WriteData(CostNum/10%10+0x30);	
		}
	}
}



void exint0()  interrupt 2 //外部中断1中断
{
	InterruptCount ++ ;    //码盘一圈20格
}






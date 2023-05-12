#include "stc15f2k60s2.h"																	 



/* 
步进电机实验
详见README文件
*/



#define uchar unsigned char
#define uint  unsigned int

#define MotorData P1   
//步进电机控制接口定义



//         D C B A
//   7 6 5 4 3 2 1 0    P1口
//   0 0 0 1 0 0 0 0
uchar phasecw[4] ={0x10,0x08,0x04,0x02};//正转 电机导通相序 D-C-B-A
uchar phaseccw[4]={0x02,0x04,0x08,0x10};//反转 电机导通相序 A-B-C-D


uchar speed;



//ms延时函数
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<224;j++);
}


//逆时针转动
void MotorCCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   Delay_xms(speed);//转速调节
  }
}

//顺时针转动
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   Delay_xms(speed);//转速调节
  }
}



//停止转动
void MotorStop(void)
{
 MotorData=0x00;
}


//针对 STC15W4K56S4 系列 IO口初始化
void IO_init(void)
{
  P0M0 = 0X00;	P0M1 = 0X00;

	P1M0 = 0X00;  P1M1 = 0X00;

	P2M0 = 0X00;	P2M1 = 0X00;

	P3M0 = 0X00;	P3M1 = 0X00;

	P4M0 = 0X00;	P4M1 = 0X00;  
	
	P5M0 = 0X00;	P5M1 = 0X00;  
	
	P6M0 = 0X00;	P6M1 = 0X00;  
	
	P7M0 = 0X00;	P7M1 = 0X00;  	
}



main()
{
	uint i;
	
	IO_init();				   //针对 STC15W4K56S4  IO口初始化	
	
	
	//	//设置STC单片机的 为推挽输出
	P1M1 &= ~(1<<3),P1M0 |=  (1<<3); 
	P1M1 &= ~(1<<4),P1M0 |=  (1<<4); 
	P1M1 &= ~(1<<1),P1M0 |=  (1<<1); 
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
	
	
	Delay_xms(50);//等待系统稳定
	
	
    speed=25; 	//---------------------------顺时针转动
	 while(1)
	 {
	 for(i=0;i<10;i++)
	  {
	   MotorCW();  //顺时针转动
	  }  
	  speed--;     //减速 
	  
	  if(speed<4)  
	  {
	   speed=25;    //重新开始减速运动
	   MotorStop();
	   Delay_xms(500);
	  }  
	 }


//	speed=4;       //--------------------------逆时针转动
//	 while(1)
//	 {
//	 for(i=0;i<10;i++)
//	  {
//	   MotorCCW();  //逆时针时针转动
//	  }  
//	  speed++;     //减速 
//	  if(speed>25)  
//	  {
//	   speed=4;    //重新开始减速运动
//	   MotorStop();
//	   Delay_xms(500);
//	  }  
//	 }


	
	
	
	
	

}


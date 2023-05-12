#include "motor.h"
#include "sys.h"
#include "lcd12864.h"


#define MotorData P1   
//步进电机控制接口定义

u8 speed = 15;   //步进电机速度设置,数值越小速度越大，最小设置为15

//         D C B A
//   7 6 5 4 3 2 1 0    P1口
//   0 0 0 1 0 0 0 0
u8 phasecw[4] ={0x10,0x08,0x04,0x02};//正转 电机导通相序 D-C-B-A
u8 phaseccw[4]={0x02,0x04,0x08,0x10};//反转 电机导通相序 A-B-C-D




//ms延时函数
void Delay_xms(u16 x)
{
 u16 i,j;
 for(i=0;i<x;i++)
  for(j=0;j<224;j++);
}


//逆时针转动
void MotorCCW(void)  //参考文章：https://blog.csdn.net/weixin_43915898/article/details/102591966
{
 u8 i,j;
	for(j=0;j<8;j++) //电机内齿轮转360度
	{
		for(i=0;i<4;i++)//旋转45度
		{
		 MotorData=phaseccw[i];
		 Delay_xms(speed);//转速调节
		}
	}
 
}

//顺时针转动
void MotorCW(void)   //参考文章：https://blog.csdn.net/weixin_43915898/article/details/102591966
{
 u8 i,j;
	
	for(j=0;j<8;j++)  //电机内齿轮转360度
	{
		for(i=0;i<4;i++)  //旋转45度
		{
		 MotorData=phasecw[i];
		 Delay_xms(speed);//转速调节
		}	
	}
 
}

void MotorCWRound(u8 n)  //顺时针转圈
{
	u8 i,j;
	
	//电机转动同时屏幕提示   0x94地址	
	LCD12864_WriteCmd(0x94);  
	LCD12864_WriteData('^');  //提示正在上升
	
	for(j=0;j<n;j++)
	{
		for(i=0;i<64;i++)  //减速比64，码盘顺时针旋转1周
	  {
	   MotorCW();  //顺时针转动
	  }
	}	
}

void MotorCCWRound(u8 n)  //逆时针转圈
{
	u8 i,j;
	
	//电机转动同时屏幕提示   0x94地址	
	LCD12864_WriteCmd(0x94);  
	LCD12864_WriteData('v');  //提示正在下降
	
	for(j=0;j<n;j++)
	{
		for(i=0;i<64;i++)  //减速比64，码盘逆时针旋转1周
	  {
	   MotorCCW();  //逆时针转动
	  }
	}	
//	LCD12864_WriteCmd(0x94);  
//	LCD12864_WriteData('=');  //提示电梯停
}

  



//停止转动
void MotorStop(void)
{
 MotorData=0x00;
}

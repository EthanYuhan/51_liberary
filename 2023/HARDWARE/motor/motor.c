#include "motor.h"
#include "sys.h"

//         D C B A
//   7 6 5 4 3 2 1 0    P1口
//   0 0 0 1 0 0 0 0
uchar phasecw[4] ={0x10,0x08,0x04,0x02};//正转 电机导通相序 D-C-B-A
uchar phaseccw[4]={0x02,0x04,0x08,0x10};//反转 电机导通相序 A-B-C-D
uchar speed;
	
//逆时针转动
void MotorCCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   delay_ms(speed);//转速调节
  }
}

//顺时针转动
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   delay_ms(speed);//转速调节
  }
}


//停止转动
void MotorStop(void)
{
 MotorData=0x00;
}

//针对 STC15W4K56S4 系列 IO口初始化
//io口初始化 P0 P1 P2 P3 P4 为准双向IO口   
//注意: STC15W4K32S4系列的芯片,上电后所有与PWM相关的IO口均为
//      高阻态,需将这些口设置为准双向口或强推挽模式方可正常使用
//相关IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
//        P2.3/P2.7/P3.7/P4.2/P4.4/P4.5

void motor_init(){
    //设置STC 单片机的P15 P16为推挽输出
	P1M1 &= ~(1<<5),P1M0 |=  (1<<5); 
	P1M1 &= ~(1<<6),P1M0 |=  (1<<6); 
}

void dc_motor_stop(){
    moto1=0;
    moto2=0;
}

void dc_motor_run(){
    moto1=1;
    moto2=1;
}


#ifndef  MOTOR_H
#define  MOTOR_H

#include "stc15f2k60s2.h"

//---重定义关键词---//
#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint 
#define uint unsigned int
#endif

#ifndef ushort 
#define ushort unsigned short
#endif


sbit moto1=P1^5;	
sbit moto2=P1^6;

#define MotorData P1   
//步进电机控制接口定义

void motor_init();
void dc_motor_stop();
void dc_motor_run();
void MotorCCW(void);
void MotorCW(void);
void MotorStop(void);

#endif

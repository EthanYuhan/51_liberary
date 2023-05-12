#ifndef __MOTOR_H
#define __MOTOR_H

#include "stc15f2k60s2.h"
#include "sys.h"


void Delay_xms(u16 x);
void MotorCCW(void);
void MotorCW(void);
void MotorStop(void);

void MotorCWRound(u8 n) ; //˳ʱ��תȦ
void MotorCCWRound(u8 n) ; //��ʱ��תȦ





#endif
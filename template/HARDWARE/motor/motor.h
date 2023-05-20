#ifndef  MOTOR_H
#define  MOTOR_H

#include "stc15f2k60s2.h"

sbit moto1=P1^5;	
sbit moto2=P1^6;

void motor_init();
void dc_motor_stop();
void dc_motor_run();

#endif

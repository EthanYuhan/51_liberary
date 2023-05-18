#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//���������������	   
									  
////////////////////////////////////////////////////////////////////////////////// 


enum dir{Pos,Neg};



//IN1-PG6
#define IN1_GPIO_PORT     GPIOG
#define IN1_GPIO_CLK      RCC_APB2Periph_GPIOG
#define IN1_GPIO_PIN      GPIO_Pin_6

//IN2-PG7
#define IN2_GPIO_PORT     GPIOG
#define IN2_GPIO_CLK      RCC_APB2Periph_GPIOG
#define IN2_GPIO_PIN      GPIO_Pin_7

//IN3-PG8
#define IN3_GPIO_PORT     GPIOG
#define IN3_GPIO_CLK      RCC_APB2Periph_GPIOG
#define IN3_GPIO_PIN      GPIO_Pin_8

//IN1-PG9
#define IN4_GPIO_PORT     GPIOG
#define IN4_GPIO_CLK      RCC_APB2Periph_GPIOG
#define IN4_GPIO_PIN      GPIO_Pin_9



/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //���Ϊ�ߵ�ƽ		
#define digitalLo(p,i)		 {p->BRR=i;}	 //����͵�ƽ

/* �������IO�ĺ� */
#define IN1_HIGH        digitalHi(IN1_GPIO_PORT,IN1_GPIO_PIN)
#define IN1_LOW         digitalLo(IN1_GPIO_PORT,IN1_GPIO_PIN)

#define IN2_HIGH        digitalHi(IN2_GPIO_PORT,IN2_GPIO_PIN)
#define IN2_LOW         digitalLo(IN2_GPIO_PORT,IN2_GPIO_PIN)

#define IN3_HIGH        digitalHi(IN3_GPIO_PORT,IN3_GPIO_PIN)
#define IN3_LOW         digitalLo(IN3_GPIO_PORT,IN3_GPIO_PIN)

#define IN4_HIGH        digitalHi(IN4_GPIO_PORT,IN4_GPIO_PIN)
#define IN4_LOW         digitalLo(IN4_GPIO_PORT,IN4_GPIO_PIN)






void MOTOR_Init(void);//��ʼ��

void stepper(uint8_t dir,int speed); //��������

		 				    
#endif

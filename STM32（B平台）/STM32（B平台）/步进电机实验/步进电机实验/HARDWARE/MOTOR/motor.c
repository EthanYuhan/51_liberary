#include "motor.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 

//���������������	   
								  
////////////////////////////////////////////////////////////////////////////////// 

     
//������� IO��ʼ��
void MOTOR_Init(void)
{
	
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����������ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( IN1_GPIO_CLK | IN2_GPIO_CLK | IN3_GPIO_CLK | IN4_GPIO_CLK , ENABLE);
		
	 /*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IN1_GPIO_PIN;	
		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(IN1_GPIO_PORT, &GPIO_InitStructure);	
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IN2_GPIO_PIN;
		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(IN2_GPIO_PORT, &GPIO_InitStructure);
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IN3_GPIO_PIN;
		/*���ÿ⺯������ʼ��GPIOF*/
		GPIO_Init(IN3_GPIO_PORT, &GPIO_InitStructure);
		
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = IN4_GPIO_PIN;
		/*���ÿ⺯������ʼ��GPIOF*/
		GPIO_Init(IN4_GPIO_PORT, &GPIO_InitStructure);
		
		//�������ų�ʼ��Ϊ�͵�ƽ
    IN1_LOW;
    IN2_LOW;
    IN3_LOW;
    IN4_LOW;		

}



void stepper(uint8_t dir,int speed)
{
	if(dir == Pos)
	{
		IN1_HIGH;
		delay_ms(speed);
		IN1_LOW;
		IN2_HIGH;
		delay_ms(speed);
		IN2_LOW;
		IN3_HIGH;
		delay_ms(speed);
		IN3_LOW;
		IN4_HIGH;
		delay_ms(speed);
		IN4_LOW;
	}
	else
	{
		IN1_HIGH;
		delay_ms(speed);
		IN1_LOW;
		IN4_HIGH;
		delay_ms(speed);
		IN4_LOW;
		IN3_HIGH;
		delay_ms(speed);
		IN3_LOW;
		IN2_HIGH;
		delay_ms(speed);
		IN2_LOW;
	}
}




 

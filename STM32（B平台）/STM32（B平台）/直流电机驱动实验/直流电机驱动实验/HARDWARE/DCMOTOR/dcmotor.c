#include "dcmotor.h"
//////////////////////////////////////////////////////////////////////////////////	 

//ֱ�������������
								  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB15��PG11Ϊ�����.��ʹ������ڵ�ʱ��		    
//ֱ�������ʼ��
void DCMOTOR_Init(void)
{
	 
 GPIO_InitTypeDef  GPIO_InitStructure; 
	
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��GPIOG�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PG11 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOG, &GPIO_InitStructure);	 //���ݲ�����ʼ��PG11
	
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PB15 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //���ݲ�����ʼ��PB15

	
 GPIO_ResetBits(GPIOB,GPIO_Pin_15);//
 GPIO_ResetBits(GPIOG,GPIO_Pin_11);//

}

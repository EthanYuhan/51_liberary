#include "stm32f10x.h"
#include "infrared.h"
#include "sys.h" 
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	
//����Թܲ�����������	   								  
//////////////////////////////////////////////////////////////////////////////////  
								    
//������ʼ������  PB13!!
void INFRARED_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB ,ENABLE);//ʹ��PORTB ʱ��



	//��ʼ�� ����Թ�-->GPIOB.13   ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������//GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.13 

}


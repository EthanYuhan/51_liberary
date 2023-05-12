#include "stc15f2k60s2.h"																	 



/* 
�������ʵ��
���README�ļ�
*/



#define uchar unsigned char
#define uint  unsigned int

#define MotorData P1   
//����������ƽӿڶ���



//         D C B A
//   7 6 5 4 3 2 1 0    P1��
//   0 0 0 1 0 0 0 0
uchar phasecw[4] ={0x10,0x08,0x04,0x02};//��ת �����ͨ���� D-C-B-A
uchar phaseccw[4]={0x02,0x04,0x08,0x10};//��ת �����ͨ���� A-B-C-D


uchar speed;



//ms��ʱ����
void Delay_xms(uint x)
{
 uint i,j;
 for(i=0;i<x;i++)
  for(j=0;j<224;j++);
}


//��ʱ��ת��
void MotorCCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phaseccw[i];
   Delay_xms(speed);//ת�ٵ���
  }
}

//˳ʱ��ת��
void MotorCW(void)
{
 uchar i;
 for(i=0;i<4;i++)
  {
   MotorData=phasecw[i];
   Delay_xms(speed);//ת�ٵ���
  }
}



//ֹͣת��
void MotorStop(void)
{
 MotorData=0x00;
}


//��� STC15W4K56S4 ϵ�� IO�ڳ�ʼ��
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
	
	IO_init();				   //��� STC15W4K56S4  IO�ڳ�ʼ��	
	
	
	//	//����STC��Ƭ���� Ϊ�������
	P1M1 &= ~(1<<3),P1M0 |=  (1<<3); 
	P1M1 &= ~(1<<4),P1M0 |=  (1<<4); 
	P1M1 &= ~(1<<1),P1M0 |=  (1<<1); 
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
	
	
	Delay_xms(50);//�ȴ�ϵͳ�ȶ�
	
	
    speed=25; 	//---------------------------˳ʱ��ת��
	 while(1)
	 {
	 for(i=0;i<10;i++)
	  {
	   MotorCW();  //˳ʱ��ת��
	  }  
	  speed--;     //���� 
	  
	  if(speed<4)  
	  {
	   speed=25;    //���¿�ʼ�����˶�
	   MotorStop();
	   Delay_xms(500);
	  }  
	 }


//	speed=4;       //--------------------------��ʱ��ת��
//	 while(1)
//	 {
//	 for(i=0;i<10;i++)
//	  {
//	   MotorCCW();  //��ʱ��ʱ��ת��
//	  }  
//	  speed++;     //���� 
//	  if(speed>25)  
//	  {
//	   speed=4;    //���¿�ʼ�����˶�
//	   MotorStop();
//	   Delay_xms(500);
//	  }  
//	 }


	
	
	
	
	

}


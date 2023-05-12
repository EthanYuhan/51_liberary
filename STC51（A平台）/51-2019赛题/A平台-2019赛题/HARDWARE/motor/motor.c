#include "motor.h"
#include "sys.h"
#include "lcd12864.h"


#define MotorData P1   
//����������ƽӿڶ���

u8 speed = 15;   //��������ٶ�����,��ֵԽС�ٶ�Խ����С����Ϊ15

//         D C B A
//   7 6 5 4 3 2 1 0    P1��
//   0 0 0 1 0 0 0 0
u8 phasecw[4] ={0x10,0x08,0x04,0x02};//��ת �����ͨ���� D-C-B-A
u8 phaseccw[4]={0x02,0x04,0x08,0x10};//��ת �����ͨ���� A-B-C-D




//ms��ʱ����
void Delay_xms(u16 x)
{
 u16 i,j;
 for(i=0;i<x;i++)
  for(j=0;j<224;j++);
}


//��ʱ��ת��
void MotorCCW(void)  //�ο����£�https://blog.csdn.net/weixin_43915898/article/details/102591966
{
 u8 i,j;
	for(j=0;j<8;j++) //����ڳ���ת360��
	{
		for(i=0;i<4;i++)//��ת45��
		{
		 MotorData=phaseccw[i];
		 Delay_xms(speed);//ת�ٵ���
		}
	}
 
}

//˳ʱ��ת��
void MotorCW(void)   //�ο����£�https://blog.csdn.net/weixin_43915898/article/details/102591966
{
 u8 i,j;
	
	for(j=0;j<8;j++)  //����ڳ���ת360��
	{
		for(i=0;i<4;i++)  //��ת45��
		{
		 MotorData=phasecw[i];
		 Delay_xms(speed);//ת�ٵ���
		}	
	}
 
}

void MotorCWRound(u8 n)  //˳ʱ��תȦ
{
	u8 i,j;
	
	//���ת��ͬʱ��Ļ��ʾ   0x94��ַ	
	LCD12864_WriteCmd(0x94);  
	LCD12864_WriteData('^');  //��ʾ��������
	
	for(j=0;j<n;j++)
	{
		for(i=0;i<64;i++)  //���ٱ�64������˳ʱ����ת1��
	  {
	   MotorCW();  //˳ʱ��ת��
	  }
	}	
}

void MotorCCWRound(u8 n)  //��ʱ��תȦ
{
	u8 i,j;
	
	//���ת��ͬʱ��Ļ��ʾ   0x94��ַ	
	LCD12864_WriteCmd(0x94);  
	LCD12864_WriteData('v');  //��ʾ�����½�
	
	for(j=0;j<n;j++)
	{
		for(i=0;i<64;i++)  //���ٱ�64��������ʱ����ת1��
	  {
	   MotorCCW();  //��ʱ��ת��
	  }
	}	
//	LCD12864_WriteCmd(0x94);  
//	LCD12864_WriteData('=');  //��ʾ����ͣ
}

  



//ֹͣת��
void MotorStop(void)
{
 MotorData=0x00;
}

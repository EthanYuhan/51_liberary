#include "stc15f2k60s2.h"


/*
���ڲ���ʵ��
���README�ļ�

*/


char  code  MESSAGE[]= "�Ұ���Ƭ�� ";	//���嵽����ռ���



//��� IAP15W4K61S4 ϵ�� IO�ڳ�ʼ��
//io�ڳ�ʼ�� P0 P1 P2 P3 P4 Ϊ׼˫��IO��   

void IO_init(void)
{
  P0M0 = 0X00;  P0M1 = 0X00;

  P1M0 = 0X00;  P1M1 = 0X00;

  P2M0 = 0X00;  P2M1 = 0X00;

  P3M0 = 0X00;  P3M1 = 0X00;

  P4M0 = 0X00;  P4M1 = 0X00;  
}




void ISP_Check(unsigned char tmp)		//�����յ����ݴ�����������
{
  unsigned char a;
    if(tmp==0x15) 						//�յ�������Ϊ0x15
     {
   
     ES= 0;								//�رմ����ж�
	  a=0;
	   while(MESSAGE[a]!= '\0')
   	   {
		SBUF = MESSAGE[a];	        //SUBF����/���ͻ�����(�ֽд���ͨ�����⹦�ܼĴ���)
         while(!TI);				// �������ݴ���	(TI�����жϱ�־)
		 TI = 0;					// ������ݴ��ͱ�־
		
		a++;					    // ��һ���ַ�
       } 
 
     ES= 1;								 //���������ж�
     }

  }   




void main() 			 //������
{
	   IO_init();				   //��� IAP15W4K61S4  IO�ڳ�ʼ��
       SCON=0x50;        //REN=1�����н���״̬�����ڹ���ģʽ2     	   
	   TMOD= 0x00;       //��ʱ��1Ϊģʽ0��16λ�Զ����أ�                
	   AUXR=0X40;		 //����1Tģʽ
       TL1=(65535-(24000000/4/9600));    //���ò�������װֵ
	   TH1=(65535-  (24000000/4/9600))>>8;

                        // ���в�������� STC15�ֲ�������ϸ˵�� 	 619ҳ
      
	   TR1  = 1;        //������ʱ��1  
	   ES   = 1;        //�������ж�                                                                     
	   EA   = 1;        // �����ж� 
	   while(1);
}



void uart_int(void) interrupt 4 using 1
{ 
unsigned char tmp;
    
	if (RI) 
    {

      tmp = SBUF;

	  ISP_Check(tmp); 

      RI = 0;
    }
}  


 




#include  "stc15f2k60s2.h"
#include  "gui.h"
#include  "tft.h"
#include  "ov7670.h"
#include  "sys.h"
#include  "sccb.h"
#include  "h1838.h"
#include  "interrupt.h"


u8 sign_18_ov=0;     //�жϺ���ѡ���־

//�жϺ���
//˵�� h1838 ��ov7670  ���õ���P3.2���ⲿ�жϹ��� 
void IR_IN() interrupt 0   
{

 if(sign_18_ov==0)		   //��������жϵ��ú���
 {
   u8 j,k;
   u16 N=0;
   u8 IRCOM[4];
   EX0 = 0;   		       //�����жϺ�ر��ж� ���н���
   delaylow(112);		   //24M��27M�´�Լ7ms��ʱ	9ms�͵�ƽǰ ���P32״̬
   if (IRIN==1) 		   //�����0 ˵��������� ����
   { EX0 =1;
	   return;
    } 
                            //ȷ��IR�źų���
   while(!IRIN);            //��IR��Ϊ�ߵ�ƽ������9ms��ǰ���͵�ƽ�źš�   

   for (j=0;j<4;j++)         //�ռ���������
   { 
    for (k=0;k<8;k++)        //ÿ��������8λ
    {
     while (IRIN);           //�� IR ��Ϊ�͵�ƽ������ǵ�һ�βɼ�����4.5ms�ߵ�ƽ�źš�

     while (!IRIN);          //�� IR ��Ϊ�ߵ�ƽ
     
     while (IRIN)            //����IR�ߵ�ƽʱ��  ��ԥ��Ƭ�������� �����ż���ʱ��
     {					    
       delaylow(5);         //27M�´�Լ300+us 
       N++;           
       if(N>=30)
	    { EX0=1;		     //���ж�
	     return;}            //0.14ms���������Զ��뿪��

     }                       //�ߵ�ƽ�������                 
     IRCOM[j]=IRCOM[j] >> 1;      //�������λ����0��
     if (N>=6) 
     {IRCOM[j]=IRCOM[j] | 0x80;}  //�������λ����1��
     N=0;
    }
   } 
   if (IRCOM[2]!=~IRCOM[3])	//������ ���Ʒ�����бȶ� �����������
   { EX0=1;return; }

   ma[1]=IRCOM[2] & 0x0F;      //ȡ����ĵ���λ
   ma[0]=IRCOM[2] >> 4;        //����4λ ȡ����λֵ

   if(ma[1]>9)
     ma[1]=ma[1]+0x37;		    //A ��65��ʼ ʮ������Ϊ0x41
   else
	 ma[1]=ma[1]+0x30;

   if(ma[0]>9)
     ma[0]=ma[0]+0x37;
   else
	 ma[0]=ma[0]+0x30;
   hong=1;                //�򿪱�־λ
   EX0 = 1; 			  //���ж�
  }

 else if(sign_18_ov==1)	   //����ͷ�жϵ��ú���
  {
   if(cur_status==0) //�����ʱ״̬Ϊ0����˵����һ��ͼ��Ŀ�ʼ����ʼ��FIFO��������
    {
     FIFO_WRST=0;	 //д��λ
     FIFO_WRST=1;
     FIFO_WEN=1;	 //дFIFOʹ��
     cur_status=1;   //���Ϊ1
    }
   else 
    if(cur_status==1)	//˵���˴�Ϊͼ��Ľ������༴��һͼ��Ŀ�ʼ
     {
       FIFO_WEN=0;		//дFIFO����
       FIFO_WRST=0;		//д��λ
       FIFO_WRST=1;
       cur_status=2;	//���Ϊ2  ��ʱ˵�����Զ�ȡFIFO�еĻ�������
	   EX0 = 0; 		//�ر��ж�
     }
  
  }

}




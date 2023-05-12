

#include "stc15f2k60s2.h"
#include <stdio.h>
#include "spi.h"
#include "sys.h"	
#include "uart.h"
#include "ld3320.h"
#include "ld3320_test.h"




u8   direct=0,process=0,flag=0; /*process����ʶ��ģ�鹤��״̬,flag�жϱ�־λ,direct����ʶ��ָ����Ϣ*/


/**
  * @brief  LD3320����
  * @param  ��
  * @retval ��
  */
void   LD3320_Task(void)
{ 
	u8   i=0;
	u8   j=0;
	u16  LD_ASR_Candi_Num; //���жϴ����ù�����

	UartSendStr("LD3320_Task...\r\n");
	delay1ms(1);	
	
	LD_WriteReg(0x29,0) ;   /*�ر��ж�*/
  LD_WriteReg(0x02,0) ;  
	
 
  while(1)
  {
    switch(process)
    { 
      case  0:         /*ld3320���ڿ���״̬*/
      i++;
      process=LD_Run();  
      if(i>=4)
      {
        UartSendStr("����ʶ��ģ���ڲ����ϣ�����Ӳ������ \r\n");
        return;
      }
      break;

      case  1:          /*ld3320����ʶ��ɹ�*/

      if(flag)          //�ж�ʶ���Ƿ�ɹ�
      {          
        switch(direct)  /*�Խ��ִ����ز���*/         
        {
          case 1:       /*�����ˮ�ơ�*/
          UartSendStr(" ��ˮ�� ָ��ʶ��ɹ�\r\n");
          break;

          case 2:	      /*�����˸��*/
          UartSendStr(" ��˸ ָ��ʶ��ɹ�\r\n"); 
          break;

          case 3:	      /*�������������*/
          UartSendStr(" ����/���  ָ��ʶ��ɹ�\r\n"); 
          break;

          case 4:		    /*���ȫ��*/
          UartSendStr(" ȫ�� ָ��ʶ��ɹ�\r\n");
          break;

          default:     
          break;
        } 
        process=flag=0;        
      }
      break;
      
      case  2:         /*ld3320����ʶ��ʧ��*/
			i=0;
      if(flag)         
      {
        UartSendStr("δʶ����Ч��Ϣ��û������\r\n");
        process=flag=0;            
      } 
      break;      
                     
      default:
      break;  
    }
		
		if((process==1)&&(LD_ReadReg(0xB2)==0x21) && (LD_ReadReg(0xbf)==0x35))	
	  {
		  UartSendStr("��⵽����\r\n");	
		  
		  flag=1;     
		  
				LD_ASR_Candi_Num = LD_ReadReg(0xba); 
				if(LD_ASR_Candi_Num>0 && LD_ASR_Candi_Num<=4) 
				{ 
					direct = LD_ReadReg(0xc5);					
					process=1;    //ʶ��ɹ�    
					 
				}else
				process=2;  //ʶ��ʧ��
	  
	  /*���һ������ʶ����������üĴ���*/
		LD_WriteReg(0x2b,0);
		LD_WriteReg(0x1C,0);//д0:ADC������
		LD_WriteReg(0x29,0);
		LD_WriteReg(0x02,0);
		LD_WriteReg(0x2B,0);
		LD_WriteReg(0xBA,0);	
		LD_WriteReg(0xBC,0);	
		LD_WriteReg(0x08,1);//���FIFO_DATA
		LD_WriteReg(0x08,0);//���FIFO_DATA�� �ٴ�д0   	
	  }
		
		
    LD_Test();
   }
}
   
  

void  LD_Test(void)
{
  
  switch(direct)  //�Խ��ִ����ز���
	{
		case 1:  //�����ˮ�ơ� 
			Glide_LED();
		break;
    
		case 2:	  //�����˸��
			Flicker_LED();
		break;
    
		case 3:	//�������������  
			Open_LED();
		break;
    
		case 4:		//���ȫ��
			Off_LED();
		break;
	  
		default:
    break;
	}	
}


/**
  * @brief  ��ˮ��Ч��
  * @param  ��
  * @retval ��
  */
 void Glide_LED(void)
 {
  P00 = 0;
	delay_ms(20);
  P00 = 1;
	delay_ms(20);	 
  P01 = 0;
	delay_ms(20);
  P01 = 1;
	delay_ms(20);
	P02 = 0;
	delay_ms(20);
  P02 = 1;
	delay_ms(20);
	P03 = 0;
	delay_ms(20);
  P03 = 1;
	delay_ms(20);
	P04 = 0;
	delay_ms(20);
  P04 = 1;
	delay_ms(20);	 
  P05 = 0;
	delay_ms(20);
  P05 = 1;
	delay_ms(20);
	P06 = 0;
	delay_ms(20);
  P06 = 1;
	delay_ms(20);
	P07 = 0;
	delay_ms(20);
  P07 = 1;
	delay_ms(20);
  
 }

/**
  * @brief  ����˸
  * @param  ��
  * @retval ��
  */
void Flicker_LED(void)

{ 
  P00 = 0;
	delay_ms(200);
  P00 = 1;
	delay_ms(200);	
}

/**
  * @brief  ����
  * @param  ��
  * @retval ��
  */
void Open_LED(void)
{
  P00 = 0;
}

/**
  * @brief  ��ȫ��Ϩ��
  * @param  ��
  * @retval ��
  */
void  Off_LED(void)
{

  P00 = 1;
	P01 = 1;
	P02 = 1;
	P03 = 1;
	P04 = 1;
	P05 = 1;
	P06 = 1;
	P07 = 1;

} 










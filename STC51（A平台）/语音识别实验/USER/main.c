
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include <stdio.h>
#include "string.h"
#include  "spi.h"
#include  "uart.h"
#include  "LD3320.h"
#include "ld3320_test.h"


/*			
 ����ʶ��ʵ��
 ���README�ĵ�
*/


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
	

	
	
	//rst 		   P13 
	//cs(NSS)    P12

	//�������
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
	P5M1 &= ~(1<<2),P5M0 |=  (1<<2); 


}


void main()
{  
  SP=0X80;				      //������ջָ��  
  IO_init();				    // IO�ڳ�ʼ��
	
	
  //LD_SPIS_L();        //����WR/SPIS �͵�ƽ	

	
  Init_SPI(); 			      //SPI��ʼ��
  //SPI_Speed(3);               //spi����ٶ�	
	
  UartInit();                 //���ڳ�ʼ��
	
	delay1ms(1);
	
	UartSendStr("��ʼ����LD3320���Գ���\r\n");
	
	delay1ms(1);	
		
	/*����ʶ��ģ�����*/
	LD3320_Task();
	
	while(1);
	

}



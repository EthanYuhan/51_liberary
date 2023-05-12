
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include <stdio.h>
#include "string.h"
#include  "spi.h"
#include  "uart.h"
#include  "LD3320.h"
#include "ld3320_test.h"


/*			
 语音识别实验
 详见README文档
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

	//推挽输出
	P1M1 &= ~(1<<2),P1M0 |=  (1<<2); 
	P5M1 &= ~(1<<2),P5M0 |=  (1<<2); 


}


void main()
{  
  SP=0X80;				      //调整堆栈指向  
  IO_init();				    // IO口初始化
	
	
  //LD_SPIS_L();        //设置WR/SPIS 低电平	

	
  Init_SPI(); 			      //SPI初始化
  //SPI_Speed(3);               //spi最低速度	
	
  UartInit();                 //串口初始化
	
	delay1ms(1);
	
	UartSendStr("开始运行LD3320测试程序\r\n");
	
	delay1ms(1);	
		
	/*语音识别模块测试*/
	LD3320_Task();
	
	while(1);
	

}




#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "sd.h"
#include  "spi.h"
#include  "pff.h"				  //�ļ�ϵͳ����.h����


/*			

		 
		  
*/


FATFS fatfs;	//�ļ�ϵͳ�ṹ�嶨��
u8 tbuf[512];			   //512�ֽ� SD�����ݻ���ռ�

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



//������
void main(void)
{ 
	FRESULT res;
	u8 buf[40];

	IO_init(); //IO�ڳ�ʼ��	
	LCD_Init();	//Һ������ʼ��
	Init_SPI(); //SPI��ʼ�� 
	LCD_Clear(WHITE);//�����׵�		

    SD_Init();			      //SD����ʼ��
    //pf_mount(&fatfs);	         //��ʼ��petit FATFS�ļ�ϵͳ  ����ȡtf����Ӧ����
	
	
	res=pf_mount(&fatfs);	
	

	
if(res == FR_OK) Show_Str(0, 146, BLACK,WHITE, "FAFS  ok",16,1);
else if(res == FR_DISK_ERR) Show_Str(0, 146, RED,WHITE, "FR_DISK_ERR  err",16,1);
else if(res == FR_NOT_READY)Show_Str(0, 146, RED,WHITE, "FR_NOT_READY  err",16,1);
else if(res == FR_NO_FILE) Show_Str(0, 146, RED,WHITE, "FR_NO_FILE  err",16,1);
else if(res == FR_NOT_OPENED) Show_Str(0, 146, RED,WHITE, "FR_NOT_OPENED  err",16,1);
else if(res == FR_NOT_ENABLED) Show_Str(0, 146, RED,WHITE, "FR_NOT_ENABLED  err",16,1);
else if(res == FR_NO_FILESYSTEM) Show_Str(0, 146, RED,WHITE, "FR_NO_FILESYSTEM  err",16,1);


	while(1);			 
	
	
}






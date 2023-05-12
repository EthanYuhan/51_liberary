
/*			
*���ź�ԭ��ͼһֱ  �����ֱ����֤ 
//����128M tf��������������120�������µ�С����tf����֤
//number�������ܿ�ͨ�����sprintf����ʵ�֣������´���֤ʱȥ���������
 	

TF����ʼ�� 
     ˵�� ��Ҫ��ʼ��TF��  �жϿ����� ���������Ǵ�����	С����SD 0-2g  ������ SDHC 2-32g
	      ��ȡ�������� ��������ʾ
		  ������������sd.c�ײ����Ѿ�д��  ����в����ĵط�����ϸ�Ķ�����
   
*/


#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "sd.h"
#include  "spi.h"

extern u8 SD_Type;			  //SD������

void IO_init(void)
{
	P0M0 = 0X00;
	P0M1 = 0X00;

	P1M0 = 0X00;
  P1M1 = 0X00;

	P2M0 = 0X00;
	P2M1 = 0X00;

	P3M0 = 0X00;
	P3M1 = 0X00;

	P4M0 = 0X00;
	P4M1 = 0X00;  
	
	P5M0 = 0X00;
	P5M1 = 0X00;  
	
	P6M0 = 0X00;
	P6M1 = 0X00;  
	
	P7M0 = 0X00;
	P7M1 = 0X00; 	
}



//������
void main(void)
{ 
	u32 sd_size; //��ʱ����

	IO_init(); //IO�ڳ�ʼ��	
	LCD_Init();	//Һ������ʼ��
	Init_SPI(); //SPI��ʼ��  
	LCD_Clear(WHITE);//�����׵�	
	
	Show_Str(0,0,BLACK,WHITE,"TF Card Init!",16,0); 
	Show_Str(0,40,BLUE,WHITE,"TF Card Type:",16,0); //������
	Show_Str(0,60,BLUE,WHITE,"TF Card Size:      Mb",16,0); //������
	

	if(SD_Init()==0) Show_Str(0,20,BLACK,WHITE,"TF Card  ok",16,0);	  //��ʼ��TF�� 
	 else Show_Str(0,20,RED,WHITE,"TF Card Error",16,0);

	 sd_size=SD_GetCapacity();	  //����SD������

	 if(SD_Type==0x04) Show_Str(104,40,BLUE,WHITE,"SD_TYPE_V2",16,0);	   //0-2g
	 if(SD_Type==0x06) Show_Str(104,40,BLUE,WHITE,"SD_TYPE_V2HC",16,0);  //2-32g

	 number(104,60,sd_size>>20,RED,WHITE);   //��ʾ������  1M=1024KB  1KB=1024B  1B=8bit	   2��20�η�Ϊ1024x1024 Ҳ����M
	 
	
	 while(1);  					 
	
	
}






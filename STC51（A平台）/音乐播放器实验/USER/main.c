
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
#include  "vs1053.h"
#include  "mp3player.h"


/*			
���ֲ�����ʵ��
���README�ĵ�˵��		  	                                           		    	  
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
 u16 br;
 u32 cnt=0;					  //���ֽ��ȵ�������
 u8 mp3in;
	
	SP=0X80;				      //������ջָ��   �ֲ�286ҳ ���

	IO_init(); //IO�ڳ�ʼ��	
	LCD_Init();	//Һ������ʼ��
	Init_SPI(); //SPI��ʼ�� 
	LCD_Clear(WHITE);//�����׵�		

  SD_Init();			      //SD����ʼ��
  pf_mount(&fatfs);	         //��ʼ��petit FATFS�ļ�ϵͳ  ����ȡtf����Ӧ����
								 ////���ǳ���Ҫ������ʹ������Petit Fatfs�ļ�ϵͳ���ܵ�ǰ��

	
  GUI_sprintf_hzstr16x(0,0,"VS1053--TEST",BLACK,WHITE);

  mp3in=Mp3Player_Init(); 
  if(mp3in==1)GUI_sprintf_hzstr16x(0,30,"MP3 Init OK",RED,WHITE);
  else GUI_sprintf_hzstr16x(0,30,"MP3 Init Error",RED,WHITE);

  res=pf_open("/MUSIC/������.mp3");			//��ָ��·���µ������ļ���

  if(res == FR_OK)GUI_sprintf_hzstr16x(0,80,"The music is already open!",BLUE,WHITE);


  VS_Restart_Play();  					    // �������� 
  VS_Set_All();        					    // ������������Ϣ 			 
  VS_Reset_DecodeTime();					// ��λ����ʱ�� 
  VS_Set_Vol(220);                          // ��������
  VS_SPI_SpeedHigh();	                    // SPI����Ϊ����

 // SetBands(); 							// Ƶ�ײ�����ʼ��

  GUI_sprintf_hzstr16x(0,110,"Play music...",BLUE,WHITE);
	
	
  while(1)
   {
	res=pf_read(tbuf,512,&br);				//ͨ���ļ�ϵͳ��ȡָ���ļ����µ�һ��������

	if((res!=FR_OK))
	{
	 // led=0;
		while(1);
	}
     cnt=0;

	 do{
	   	if(VS_Send_MusicData(tbuf+cnt)==0)	  //һ����32���ֽ���������
		{
		cnt+=32;	
		}
		else {
		//led=0;
		}

	   }while(cnt<512);

   }

    if(br!=512)                              // �ļ�����
    {
      while(1);
    }	 
	
	
}






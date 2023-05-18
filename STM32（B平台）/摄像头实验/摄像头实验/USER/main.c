#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "sram.h"
#include "malloc.h"
#include "string.h"
#include "text.h"
#include "usmart.h"
#include "ov7670.h"
#include "timer.h"
#include "exti.h"


//--------------------------
//����ͷʵ��  
//���README�ĵ�˵��
//-------------------------- 
 
 
 
extern u8 ov_sta;	//��exit.c���涨��
extern u8 ov_frame;	//��timer.c���涨��	   
//����LCD��ʾ
void camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta==2)
	{
		LCD_Scan_Dir(L2R_U2D);		//L2R_U2D
		LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 				
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK=0;
		OV7670_RCK=1;
		OV7670_RCK=0;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK=1;  
		for(j=0;j<76800;j++)
		{
			OV7670_RCK=0;
			color=GPIOC->IDR&0XFF;	//������
			OV7670_RCK=1; 
			color<<=8;  
			OV7670_RCK=0;
			color|=GPIOC->IDR&0XFF;	//������
			OV7670_RCK=1; 
			LCD->LCD_RAM=color;    
		}   							 
		EXTI_ClearITPendingBit(EXTI_Line8);  //���EXTI8��·����λ	
		ov_sta=0;					//��ʼ��һ�βɼ�
 		ov_frame++; 
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽�� 
	} 
}


	 
 int main(void)
 {	 
					 
	u8 *pname;			//��·�����ļ��� 
	u8 lightmode=0,saturation=2,brightness=2,contrast=2;  //
	u8 effect=0;

 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ9600

	LCD_Init();	
	if(lcddev.id==0X6804||lcddev.id==0X5310||lcddev.id==0X5510||lcddev.id==0X9488)	//ǿ��������Ļ�ֱ���Ϊ320*240.��֧��3.5/4.3�����
	{
		lcddev.width=240;
		lcddev.height=320; 
	}


 	mem_init(SRAMIN);	//��ʼ���ڲ��ڴ��	
	usmart_dev.init(72);//usmart��ʼ��	
 	//exfuns_init();					//Ϊfatfs��ر��������ڴ�  


	//piclib_init();					//��ʼ����ͼ
		 
	POINT_COLOR=RED;      
 	 
 	Show_Str(60,50,200,16,"STM32",16,0);				    	 
	Show_Str(60,70,200,16,"CAMERA",16,0);				    	 		    	 
	Show_Str(60,90,200,16,"20201114",16,0);
									   						    
 	pname=mymalloc(SRAMIN,30);	//Ϊ��·�����ļ�������30���ֽڵ��ڴ�		    
 	while(pname==NULL)			//�ڴ�������
 	{	    
		Show_Str(60,190,240,16,"SRAM Fail!",16,0);
		delay_ms(200);				  
		LCD_Fill(60,190,240,146,WHITE);//�����ʾ	     
		delay_ms(200);				  
	}   											  
	while(OV7670_Init())//��ʼ��OV7670
	{
		Show_Str(60,190,240,16,"OV7670 err!",16,0);
		delay_ms(200);
	    LCD_Fill(60,190,239,186,WHITE);
		delay_ms(200);
	}
	
 	Show_Str(60,190,200,16,"OV7670  ok!",16,0);
	delay_ms(1500);	 

	OV7670_Light_Mode(lightmode);
	OV7670_Color_Saturation(saturation);
	OV7670_Brightness(brightness);
	OV7670_Contrast(contrast);
 	OV7670_Special_Effects(effect);	
				  
	EXTI8_Init();										//ʹ�ܶ�ʱ������
	OV7670_Window_Set(10,174,240,320);	//���ô���	  
  OV7670_CS=0;		
	
 	while(1)
	{		
	
 		camera_refresh();//������ʾ
		
		
	}	   										    			    
}





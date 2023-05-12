
#include <stdio.h>
#include "lcd.h"
#include "string.h"
#include "sys.h"
#include "gui.h"
#include "test.h"
#include "touch.h"
#include "pic.h"

#define OTT_MAX_TOUCH  5   //5  		//������֧�ֵĵ���,�̶�Ϊ5��

//========================variable==========================//
u16 ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//������ɫ����
const u16 POINT_COLOR_TBL[OTT_MAX_TOUCH]={RED,GREEN,BLUE,BROWN,GRED}; //���ݴ���5����ɫ����
u16 ColornTab[8]={RED,MAGENTA,GREEN,DARKBLUE,BLUE,BLACK,LIGHTGREEN};
//=====================end of variable======================//

/*****************************************************************************
 * @name       :void DrawTestPage(u8 *str)
 * @date       :2018-08-09 
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/ 
void DrawTestPage(u8 *str)
{
//���ƹ̶���up
LCD_Clear(WHITE);
LCD_Fill(0,0,lcddev.width,20,BLUE);
//���ƹ̶���down
LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
POINT_COLOR=WHITE;
Gui_StrCenter(0,2,WHITE,BLUE,str,16,1);//������ʾ
Gui_StrCenter(0,lcddev.height-18,WHITE,BLUE,"12345678910",16,1);//������ʾ
//���Ʋ�������
//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
}

/*****************************************************************************
 * @name       :void main_test(void)
 * @date       :2018-08-09 
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void main_test(void)
{
	DrawTestPage("�ۺϲ��Գ���");
	
	Gui_StrCenter(0,30,RED,BLUE,"����",16,1);//������ʾ
	Gui_StrCenter(0,60,RED,BLUE,"�ۺϲ��Գ���",16,1);//������ʾ	
	Gui_StrCenter(0,90,MAGENTA,BLUE,"3.5\" ILI9488 320X480",16,1);//������ʾ
	Gui_StrCenter(0,120,BLUE,BLUE," 2019-06-20",16,1);//������ʾ
	delay_ms(1500);		
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-09 
 * @function   :Color fill test(white,black,red,green,blue)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
	//DrawTestPage("����1:��ɫ������");
	LCD_Fill(0,0,lcddev.width,lcddev.height,WHITE);
	Show_Str(20,30,BLUE,YELLOW,"BL Test",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width,lcddev.height,RED);
	Show_Str(20,30,BLUE,YELLOW,"RED ",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width,lcddev.height,GREEN);
	Show_Str(20,30,BLUE,YELLOW,"GREEN ",16,1);delay_ms(800);
	LCD_Fill(0,0,lcddev.width,lcddev.height,BLUE);
	Show_Str(20,30,RED,YELLOW,"BLUE ",16,1);delay_ms(800);
}

/*****************************************************************************
 * @name       :void Test_FillRec(void)
 * @date       :2018-08-09 
 * @function   :Rectangular display and fill test
								Display red,green,blue,yellow,pink rectangular boxes in turn,
								1500 milliseconds later,
								Fill the rectangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("����3:GUI����������");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width-1,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawFillRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 		
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-09 
 * @function   :circular display and fill test
								Display red,green,blue,yellow,pink circular boxes in turn,
								1500 milliseconds later,
								Fill the circular in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("����4:GUI��Բ������");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for (i=0; i<5; i++)  
		gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,0);
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width-1,lcddev.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(lcddev.width/2-80+(i*25),lcddev.height/2-50+(i*25),ColorTab[i],30,1);
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-09 
 * @function   :triangle display and fill test
								Display red,green,blue,yellow,pink triangle boxes in turn,
								1500 milliseconds later,
								Fill the triangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
	u8 i=0;
	DrawTestPage("����5:GUI Triangle������");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Draw_Triangel(lcddev.width/2-80+(i*20),lcddev.height/2-20+(i*15),lcddev.width/2-50-1+(i*20),lcddev.height/2-20-52-1+(i*15),lcddev.width/2-20-1+(i*20),lcddev.height/2-20+(i*15));
	}
	delay_ms(1500);	
	LCD_Fill(0,20,lcddev.width-1,lcddev.height-20,WHITE); 
	for(i=0;i<5;i++)
	{
		POINT_COLOR=ColorTab[i];
		Fill_Triangel(lcddev.width/2-80+(i*20),lcddev.height/2-20+(i*15),lcddev.width/2-50-1+(i*20),lcddev.height/2-20-52-1+(i*15),lcddev.width/2-20-1+(i*20),lcddev.height/2-20+(i*15));
	}
	delay_ms(1500);
}

/*****************************************************************************
 * @name       :void English_Font_test(void)
 * @date       :2018-08-09 
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void English_Font_test(void)
{
	DrawTestPage("����6:Ӣ����ʾ����");
	POINT_COLOR=RED;
	BACK_COLOR=BLUE;
	LCD_ShowString(10,30,12,"6X12:abcdefghijklmnopqrstuvwxyz",0);
	LCD_ShowString(10,45,12,"6X12:ABCDEFGHIJKLMNOP0123456789",1);
	LCD_ShowString(10,60,12,"6X12:~!@#$%^&*()_+{}:<>?/|-+.",0);
	LCD_ShowString(10,80,16,"8X16:abcdefghijklmnopqrstuvw",0);
	LCD_ShowString(10,100,16,"8X16:ABCDEFGHIJKLMN01234567",1);
	LCD_ShowString(10,120,16,"8X16:~!@#$%^&*()_+{}:<>?/|",0); 
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Chinese_Font_test(void)
 * @date       :2018-08-09 
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Chinese_Font_test(void)
{	
	DrawTestPage("����7:������ʾ����");
	Show_Str(10,30,BLUE,YELLOW,"16X16:��ӭ��",16,0);
	Show_Str(10,50,BLUE,YELLOW,"16X16:Welcome�Ƽ�",16,0);
	Show_Str(10,70,BLUE,YELLOW,"24X24:���Ĳ���",24,1);
	Show_Str(10,100,BLUE,YELLOW,"32X32:�������",32,1);
	delay_ms(1200);
}

/*****************************************************************************
 * @name       :void Pic_test(void)
 * @date       :2018-08-09 
 * @function   :picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Pic_test(void)
{
	DrawTestPage("����8:ͼƬ��ʾ����");
//	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
//	Gui_Drawbmp16(30,30,gImage_qq);
//	Show_Str(30+12,75,BLUE,YELLOW,"Linux",16,1);
//	Gui_Drawbmp16(90,30,gImage_qq);
//	Show_Str(90+12,75,BLUE,YELLOW,"Linux",16,1);
//	Gui_Drawbmp16(150,30,gImage_qq);
//	Show_Str(150+12,75,BLUE,YELLOW,"Linux",16,1);
//	delay_ms(1200);
	
	Gui_Drawbmp16(0,50,gImage_qq);
	Gui_Drawbmp16(50,50,gImage_qq);
	Gui_Drawbmp16(100,50,gImage_qq);
	Gui_Drawbmp16(150,50,gImage_qq);
	Gui_Drawbmp16(200,50,gImage_qq);
	Gui_Drawbmp16(250,50,gImage_qq);
	Gui_Drawbmp16(300,50,gImage_qq);
	Gui_Drawbmp16(350,50,gImage_qq);
	Gui_Drawbmp16(400,50,gImage_qq);

	
	Gui_Drawbmp16(0,100,gImage_qq);
	Gui_Drawbmp16(50,100,gImage_qq);
	Gui_Drawbmp16(100,100,gImage_qq);
	Gui_Drawbmp16(150,100,gImage_qq);
	Gui_Drawbmp16(200,100,gImage_qq);
	Gui_Drawbmp16(250,100,gImage_qq);
	Gui_Drawbmp16(300,100,gImage_qq);
	Gui_Drawbmp16(350,100,gImage_qq);
	Gui_Drawbmp16(400,100,gImage_qq);


	Gui_Drawbmp16(0,150,gImage_qq);
	Gui_Drawbmp16(50,150,gImage_qq);
	Gui_Drawbmp16(100,150,gImage_qq);
	Gui_Drawbmp16(150,150,gImage_qq);
	Gui_Drawbmp16(200,150,gImage_qq);
	Gui_Drawbmp16(250,150,gImage_qq);
	Gui_Drawbmp16(300,150,gImage_qq);
	Gui_Drawbmp16(350,150,gImage_qq);
	Gui_Drawbmp16(400,150,gImage_qq);

	Gui_Drawbmp16(0,200,gImage_qq);
	Gui_Drawbmp16(50,200,gImage_qq);
	Gui_Drawbmp16(100,200,gImage_qq);
	Gui_Drawbmp16(150,200,gImage_qq);
	Gui_Drawbmp16(200,200,gImage_qq);
	Gui_Drawbmp16(250,200,gImage_qq);
	Gui_Drawbmp16(300,200,gImage_qq);
	Gui_Drawbmp16(350,200,gImage_qq);
	Gui_Drawbmp16(400,200,gImage_qq);
	
		Gui_Drawbmp16(0,250,gImage_qq);
	Gui_Drawbmp16(50,250,gImage_qq);
	Gui_Drawbmp16(100,250,gImage_qq);
	Gui_Drawbmp16(150,250,gImage_qq);
	Gui_Drawbmp16(200,250,gImage_qq);
	Gui_Drawbmp16(250,250,gImage_qq);
	Gui_Drawbmp16(300,250,gImage_qq);
	Gui_Drawbmp16(350,250,gImage_qq);
	Gui_Drawbmp16(400,250,gImage_qq);

	
	
	
}

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09 
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Rotate_Test(void)
{
	u8 i=0;
	u8 *Direction[4]={"Rotation:0","Rotation:90","Rotation:180","Rotation:270"};
	
	for(i=0;i<4;i++)
	{
	LCD_direction(i);
	DrawTestPage("����9:��Ļ�ǶȲ���");
	Show_Str(20,30,BLUE,YELLOW,Direction[i],16,1);
	Gui_Drawbmp16(30,50,gImage_qq);
	delay_ms(1000);delay_ms(1000);
	}
	LCD_direction(USE_HORIZONTAL);
}

/*****************************************************************************
 * @name       :void Touch_Test(void)
 * @date       :2018-08-09 
 * @function   :touch test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Touch_Test(void)
{ 
	u8 t=0;
	u8 i=0,j=0;	 
 	u16 lastpos[5][2];		//���һ�ε����� 
	
	DrawTestPage("����10:����Touch����(֧��5�㴥��)  ");
	
	TP_Init();	
	
	//LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
	POINT_COLOR=RED;//���û�����ɫ //���
	
	while(1)
	{
		j++;
		TP_Scan();
		for(t=0;t<OTT_MAX_TOUCH;t++)//���5�㴥��
		{
			if((tp_dev.sta)&(1<<t))//�ж��Ƿ��е㴥����
			{
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)//��LCD��Χ��
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<16)
					{
							if(j>1)
							{
								continue;
							}
							DrawTestPage("����10:����Touch����(֧��5�㴥��)  ");
							LCD_ShowString(lcddev.width-24,0,16,"RST",1);//��ʾ��������
							POINT_COLOR=RED;//���û�����ɫ //���
					}
					else
					{
							LCD_DrawLine2(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//����

					}
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
//		delay_ms(1); //��֤�Է�Ӧ��Ӱ��  20
		if(j>4)
		{
			j=0;
		}
	}  
}

/*****************************************************************************
 * @name       :void Test_Read(void)
 * @date       :2018-11-13 
 * @function   :read test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Read(void)
{
	u16 lcd_id,color;
	u8 buf[10] = {0},i;
	u8 cbuf[20] = {0};
	DrawTestPage("����2:��ID����ɫֵ����");
	LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	lcd_id = LCD_Read_ID();
	sprintf((char *)buf,"ID:0x%x",lcd_id);
	Show_Str(50,25,BLUE,YELLOW,buf,16,0);
	
	for (i=0; i<7; i++) 
	{
		POINT_COLOR=ColornTab[i];
		//LCD_DrawFillRectangle(40-10,55+i*25-10,40+10,55+i*25+10);
		LCD_DrawFillRectangle(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15),lcddev.width/2-80+(i*15)+60,lcddev.height/2-80+(i*15)+60); 
		color = LCD_ReadPoint(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15));

		sprintf((char *)cbuf,"read color:0x%x",color);
		Show_Str(lcddev.width/2-80+(i*15),lcddev.height/2-80+(i*15)-8,POINT_COLOR,YELLOW,cbuf,16,0);

		delay_ms(1000);
	}
	
	
	
	while(1);
	
}
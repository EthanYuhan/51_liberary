
/*			
ͼƬ��ʾʵ��
���README�ĵ�
		  	                                           		    	  
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
#include  "pff.h"				  //�ļ�ϵͳ����.h����

FATFS fatfs;	//�ļ�ϵͳ�ṹ�嶨��
u8 tbuf[512];			   //512�ֽ� SD�����ݻ���ռ�

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

//��ʾBIN�ļ�  bin�ļ���СΪ240x320
//path  bin�ļ���
//����0 ��ʾ��� 1  ��ȡʧ��
u8 play_bin(const char  *path)
{

FRESULT res; 
u16 br,i,y=0;
u8 x=0;
	
res=pf_open(path);		             //��ָ��·���ļ�	 ��һ�����Դ��κ�·���µ��ļ� ע�����Ĺ��ܾ��Ǵ��ļ��������ļ���
                                     //���ļ������ݵ�ǰ���Ǳ���Ҫ������ļ�

if(res == FR_OK){

  LCD_SetWindows(0,0,239,319);      // ��ɨ���� ��ɨ����	   Ҫ����BIN ���ɷ�ʽ �����������
 
  while(1)
  {
     pf_read(tbuf, 512, &br); //512�ĳ�256 ���У�
									//���� res=pf_open(path);�򿪵���ָ��·���µ��ļ�������Ϳ���ֱ�Ӷ�ȡ�ļ�������
	                                //�ӳ�ʼλ�ö�ȡ512���ֽڵ�������  �ٴζ�ȡ����ƫ�� �Զ�������512�ֽ�
									//pf_read���ص㣺�������Զ�ȡ·���µ��ļ����ݣ������㲻�ı�·����ǰ�������Զ��ӵ�ַ��ȡ���������
									//�ӳ�����Կ��� �����ڲ�ͣ��ѭ��pf_read(tbuf, 512, &br);��������ȡ512���ֽڣ�
									//��ô���ڲ��Ĺ������ǴӸ���·����ʼ��ȥ512���ֽ�Ȼ���Զ�ָ���512�ֽڵ�·������ȡ��512���ֽ�ֱ��ͼƬ��ʾ���


	  for(i=0;i<256;i++)            //Ȼ��д��Һ������������ʾ256�����أ�ÿ������16λ��2���ֽ� Ҳ����ÿ�����ֽ�����ʾһ�����ص� 
	{   
   	   LCD_WR_DATA(((u16)tbuf[i*2+1]<<8)+tbuf[i*2]);	 //��ʾ��ɫ
	   x++;
	   if(x==240)                        
	     {
	     y++;
	     x=0;
	     if(y==320)
	       return 0;
	     }
    }
  
	}

   }

 return 1;  
 }  

 
 //��ʾ16λɫBMPͼƬ
//ͼƬ��ʾ�� ��Ļ������λ��
//����bmp�������� ǰ4���ֽ�Ϊ Ϊ�ļ���С  18-21��  22-25 ��  28���أ���ֵΪ1��4��8��16��24����32 
//*path  bmpͼƬ�ļ���
//���� 0  ��ʾ���  1 ��ʾ�������� 
u8 play_bmp(const char *path)
{
FRESULT res; 

u16 br,y=0,zy,height,	  //width,heightͼƬ�ĳ�ʼ���
     y1,i1,tmp;		              //tmp 16λ�������
u8 x=0,zx,width,x1,
   rgb=0, 
   Bmpcolor;

res=pf_open(path);		             //��ָ��·���ļ�	 ��һ�����Դ��κ�·���µ��ļ� ע�����Ĺ��ܾ��Ǵ��ļ��������ļ���
                                     //���ļ������ݵ�ǰ���Ǳ���Ҫ������ļ�

if(res == FR_OK){


    pf_read(tbuf, 54, &br);		 //ȡǰ54�ֽ�  ǰ54�ֽں���bmp �ļ���С �ļ�����߶� ����ֵ ��������Ϣ   
	if(br!=54) return 1;		 //��ȡ����

											//ʵ�ʿ�͸߶�����4���ֽڱ�ʾ�ģ���������ͼƬ�Ĵ�С���ܳ������ĳߴ�
											//����ֻ��һ���ֽڱ�ʾ���,�����ֽڱ�ʾ�߶�
	width= tbuf[18];				        //�����ļ����	 
	height= (tbuf[23]<<8)+tbuf[22];	//�����ļ��߶�

	Bmpcolor=(u16)(tbuf[28]/8);					//��ȡ��ɫ��� һ����16λ 24λ 32λ  
	
//	sprintf(buf,"w%ld  B%ld",(u32)width,(u32)Bmpcolor);
//	Show_Str(0,0,BLUE,YELLOW,buf,32,0); 
	
	
	//��С����Ļ�ߴ��ͼƬ�ŵ���Ļ���м���ʾ
	if(width<239)   zx=(240-width)/2;         else zx=0;  //
	if(height<319)	zy=(320-height)/2;        else zy=0;

	x1=zx; y1=zy;			   //��ֵ������ֵ

					 //BMPͼƬ�����ɨ�跽ʽΪ ���µ��� ������
	LCD_Scan_Dir(4); //ɨ�跽ʽΪ ������   ���µ��� 		

    LCD_SetWindows(x1,y1,x1+width-1,y1+height-1);         //������ʾ��Χ ��ɨ���� ��ɨ����
	LCD_RS=1;    //д����������	 	  Ϊ���д���ٶ� ��ѭ��ǰ����
	
	while(1)                   //һֱ�����һ��
	{
		  	  
	 pf_read(tbuf, 512, &br);		 //��54�ֽں�λ�ö�ȡ512���ֽڵ�������  
	     		
	 for(i1=0;i1<512;i1++)
	 {
	  	if(Bmpcolor==2)				 //16λBMP
		{
         switch(rgb)				 //555ת565��ʽ
		 {
		  case 0:
		     tmp=((u16)tbuf[i1]&0x1f);			 //R
			 tmp+=(((u16)tbuf[i1])&0xe0)<<1;	 //G
			  break;
		  case 1:
             tmp+=(u16)tbuf[i1]<<9;				 //B
			 break;		     		 
		 }	                                                
		}
		else if(Bmpcolor==3)		//24λBMP���� RGB�ֱ�ռ8���ֽ�
		{
		  switch(rgb)
		  {
		    case 0:
			  tmp=tbuf[i1]>>3;					 //B
			  break;
			case 1:
			  tmp+=((u16)tbuf[i1]<<3)&0x07e0;	 //G
			  break;
			case 2:
			  tmp+=((u16)tbuf[i1]<<8)&0xf800;	 //R
			  break;	  
		  }
		
		}
		rgb++;

		if(rgb==Bmpcolor)
		{	  
			LCD_WR_DATA(tmp);  //��ʼд��
			tmp=0;
			rgb=0;			
	
	        x1++;							  //�����Լ� ����һ�� �������� �����Լ� ֱ��ɨ������ͼƬ
	        if(x1==width+zx)                  
	        {	 
	         y1--;
	         x1=zx;
	         if(y1==zy-height)
			  {	
									//�ָ�����ɨ�跽ʽ
									//LCD��ʾ��������    				  
				  LCD_Scan_Dir(1);  //  ��>��  ��>�� 
				  
				 return 0;    //��ʾ���	
			  }
				 						     
	        }
		 }

	  } 

	}
	 
   }
 return 1;   //����
}
 



//������
void main(void)
{ 

	IO_init(); //IO�ڳ�ʼ��	
	LCD_Init();	//Һ������ʼ��
	Init_SPI(); //SPI��ʼ�� 
	LCD_Clear(WHITE);//�����׵�		

    SD_Init();			      //SD����ʼ��
    pf_mount(&fatfs);	         //��ʼ��petit FATFS�ļ�ϵͳ  ����ȡtf����Ӧ����

   
	play_bmp("/24bmp/2.bmp");	  //��ʾָ��·���µ�bmp�ļ�	   
							   	//����Ҫ��֪����TF�����·�� "/����è.bmp"������TF����Ŀ¼���ļ���Ϊ����è.bmp�ļ�
							     //�����ļ����ļ��� �����ļ����µĻ���è.bmp ��ô��Ҫд"/����/����è.bmp"
 
	
	
	while(1);		 
	
	
}






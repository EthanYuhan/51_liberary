#include "sys.h" 
//#include "fontupd.h"
//#include "w25qxx.h"
#include "lcd.h"
#include "text.h"	
#include "string.h"												    
#include "usart.h"	
#include "sdio_sdcard.h"
#include "ff.h"
//////////////////////////////////////////////////////////////////////////////////	 

//������ʾ ��������	   
								  
////////////////////////////////////////////////////////////////////////////////// 
static FRESULT res_sd;
static FIL fnew;													/* file objects */
extern UINT br;
extern FATFS *fs[_VOLUMES];  

 
//code �ַ�ָ�뿪ʼ
//���ֿ��в��ҳ���ģ
//code �ַ����Ŀ�ʼ��ַ,GBK��
//mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
//size:�����С
//ʾ�� Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ������� 
u8 Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{		    
	unsigned char qh,ql;
	unsigned char i;					  
	unsigned long foffset; 		
	static u8 everRead = 0;

	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	qh=*code;
	ql=*(++code);
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�� ���ú���
	{   		    
	    for(i=0;i<csize;i++)*mat++=0x00;//�������
	    return 2; //��������
	}   	
	if(ql<0x7f)ql-=0x40;//ע��!
	else ql-=0x41;
	qh-=0x81;   
	
	foffset=((unsigned long)190*qh+ql)*csize;	//�õ��ֿ��е��ֽ�ƫ����  	
	

		/*��һ��ʹ�ã������ļ�ϵͳ����ʼ��sd*/
	if(everRead == 0)
	{
	res_sd = f_mount(fs[0],"0:",1);
	everRead = 1;
	}	

	 switch(size)	
	{
		case 12:
		res_sd = f_open(&fnew , "0:/SYSTEM/FONT/GBK12.FON", FA_OPEN_EXISTING | FA_READ);
		break;
		case 16:
		res_sd = f_open(&fnew , "0:/SYSTEM/FONT/GBK16.FON", FA_OPEN_EXISTING | FA_READ);
		break;
		case 24:
		res_sd = f_open(&fnew , "0:/SYSTEM/FONT/GBK24.FON", FA_OPEN_EXISTING | FA_READ);
		break;
	}

    if ( res_sd == FR_OK ) 
    {
        f_lseek (&fnew, foffset);		//ָ��ƫ��				
		res_sd = f_read( &fnew, mat, csize, &br );	        
		f_close(&fnew);	      
        return 0;  
    }    
    else
        return 1;  


}  




//��ʾһ��ָ����С�ĺ���
//x,y :���ֵ�����
//font:����GBK��
//size:�����С
//mode:0,������ʾ,1,������ʾ	
// ʾ�� Show_Font(30+132,220,fontx,24,0);	
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,t,t1;
	u16 y0=y;
	u8 dzk[72];   
	 	
	
	u8 csize=(size/8+((size%8)?1:0))*(size);//�õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	if(size!=12&&size!=16&&size!=24)return;	//��֧�ֵ�size
	
	Get_HzMat(font,dzk,size);	//�õ���Ӧ��С�ĵ�������  ��Ӵ�tf�����ֿ�
	
	for(t=0;t<csize;t++)
	{   												   
		temp=dzk[t];			//�õ���������                          
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR); 
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	} 

}
//��ָ��λ�ÿ�ʼ��ʾһ���ַ���	    
//֧���Զ�����
//(x,y):��ʼ����
//width,height:����
//str  :�ַ���
//size :�����С
//mode:0,�ǵ��ӷ�ʽ;1,���ӷ�ʽ    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //�ַ���������  	    				    				  	  
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//���� 
	        else              //�ַ�
	        {      
                if(x>(x0+width-size/2))//����
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//Խ�緵��      
		        if(*str==13)//���з���
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//��Ч����д�� 
				str++; 
		        x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
	        }
        }else//���� 
        {     
            bHz=0;//�к��ֿ�    
            if(x>(x0+width-size))//����
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//Խ�緵��  						     
	        Show_Font(x,y,str,size,mode); //��ʾ�������,������ʾ 
	        str+=2; 
	        x+=size;//��һ������ƫ��	    
        }						 
    }   
}  			 		 
//��ָ����ȵ��м���ʾ�ַ���
//����ַ����ȳ�����len,����Show_Str��ʾ
//len:ָ��Ҫ��ʾ�Ŀ��			  
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len)
{
	u16 strlenth=0;
   	strlenth=strlen((const char*)str);
	strlenth*=size/2;
	if(strlenth>len)Show_Str(x,y,lcddev.width,lcddev.height,str,size,1);
	else
	{
		strlenth=(len-strlenth)/2;
	    Show_Str(strlenth+x,y,lcddev.width,lcddev.height,str,size,1);
	}
}   

























		  







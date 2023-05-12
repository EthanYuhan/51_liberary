
//DS1302����  
//���� ��ʾ ʱ�� ���� ���� ���� 
//�ɲ�ͣ������ʱ��

#include "stc15f2k60s2.h" 
#include "ds1302.h"
#include "intrins.h"
#include "sys.h"
#include "lcd12864.h"


//code u8 write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //���ʱ�������� ���λ��дλ
//code u8 read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  
//u8 ds1302tmp[7]={50,59,23,20,4,7,20};//���ʱ��������		 2020�� 4�� 20�� 8��20�� ������  ��������Լ��趨��



//дһ���ֽ� 
//temp Ҫд����ֽ�	 ����ַ�����ݣ�
void Write_Ds1302_Byte(u8 temp) 
{
 u8 i;
 for (i=0;i<8;i++)     	//ѭ��8�� д������
  { 
     SCK=0;
     SDA=temp&0x01;     //ÿ�δ�����ֽ� 
     temp>>=1;  		//����һλ
     SCK=1;
   }
}   

//д��DS1302����
//address д��ĵ�ַ
//dat     д�������
void Write_Ds1302( u8 address,u8 dat )     
{
 	CE=0;
	_nop_();
 	SCK=0;
	_nop_();
 	CE=1;						//����
   	_nop_();                    
 	Write_Ds1302_Byte(address);	//���͵�ַ
 	Write_Ds1302_Byte(dat);		//��������
 	CE=0;  		            //�ָ�
}

//����DS1302����
//address ��ȡ���ݵĵ�ַ
//���� ��ȡ������ֵ
u8 Read_Ds1302 ( u8 address )
{
 	u8 i,temp=0x00;
 	CE=0;
	_nop_();
	_nop_();
 	SCK=0;
	_nop_();
	_nop_();
 	CE=1;
	_nop_();
	_nop_();

 	Write_Ds1302_Byte(address);

 	for (i=0;i<8;i++) 		//ѭ��8�� ��ȡ����
 	{		
 		if(SDA)
 		temp|=0x80;			//ÿ�δ�����ֽ�
		SCK=0;
		temp>>=1;			//����һλ
		_nop_();
	   _nop_();
	   _nop_();
 		SCK=1;
	} 

 	CE=0;
	_nop_();	          	//����ΪDS1302��λ���ȶ�ʱ��
 	_nop_();
	CE=0;
	SCK=0;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	SCK=1;
	_nop_();
	_nop_();
	SDA=0;
	_nop_();
	_nop_();
	SDA=1;
	_nop_();
	_nop_();
	return (temp);			//����
}


//д��ʱ DCBת��	
//DS1302 ֻ���� DCB�� ����4λ��10λֵ ��4λ�Ÿ�λֵ
//add Ҫת���ĵ�ַ
//���� ת���������
u8 bcd_read(u8 add)
{
  u8 fla,fla2;
  fla=Read_Ds1302(add);
  fla2=((fla/16)*10)+(fla&0x0f);
  return fla2;	
}



/*

//�趨ʱ������
//��Ҫ����ʱ��оƬ��� ���ʱ��������
void Set_RTC(void)		       //�趨 ����
{
	u8 i;

	for(i=0;i<7;i++)         //BCD����
	 ds1302tmp[i]=ds1302tmp[i]/10*16+ds1302tmp[i]%10;  //ds1302tmp[7]={0,20,8,20,4,7,20};
	 
 	Write_Ds1302(0x8E,0X00);   //дʹ��
			
 	for(i=0;i<7;i++)		   //7��д�� ���ʱ��������
 	Write_Ds1302(write_rtc_address[i],ds1302tmp[i]);
	
    Write_Ds1302(0x8E,0x80);   //д��ֹ

}

*/




//DS1302  ʱ�� ��ʾ����
//�˺�����Ҫѭ�� ���и��� �����Զ�����
//��20:25
void ds1302_scan(void)
{
    u8 temp;	
	
	temp = bcd_read(0x85);//ʱ

	LCD12864_SetWindow(3,0);    //��4��	��ʾ			
	LCD12864_WriteData(temp/10%10+0x30);			
	LCD12864_WriteData(temp%10+0x30);
	LCD12864_WriteData(':');
	
	temp = bcd_read(0x83);//��

	LCD12864_WriteData(temp/10%10+0x30);			
	LCD12864_WriteData(temp%10+0x30);	
	
}




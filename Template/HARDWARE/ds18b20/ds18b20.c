
#include "stc15f2k60s2.h"
#include "sys.h"
#include "ds18b20.h"
#include "gui.h"


sbit DQ=P1^7;       //ds18b20 �˿�


//DS18B20��λ����
void ow_reset(void)
{

    DQ=1;                   //�Ӹ�������
    DQ=0;
    delay4us(125);             //>480 us	  �͵�ƽ��λ�ź�
    DQ=1;
    delay4us(4);              //>15us ��������	 15-60us�ߵ�ƽ��  ��60-240us ��Ӧ���ź�
}



//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
u8 ds18B20_check(void) 	   
{   
	u8 retry=0;	 		//���������
    while (DQ&&retry<50)
	{
		retry++;
		delay4us(1);		//���4us
	};
	if(retry>=100)return 1;
	else retry=0;	 
    while (!DQ&&retry<60)		 //����240us����ʱ ��������ĸ�λ����
	{
		retry++;
		delay4us(1);		//���4us
	};

	if(retry>=60)return 1;	    //û�нӵ�DS18B20Ӧ��
	return 0;					//�ӵ�Ӧ��
}


//��ʼ��DS18B20  ͬʱ���DS�Ĵ���
//����1:������
//����0:����   
u8 ds18b20_init(void)
{
  u8 m;
  ow_reset();         //��λ����
  m=ds18B20_check();  //�ȵ�dsӦ��
  return m;
}


//DS18B20д�����
//��1-WIRE ������д1���ֽ�
//u8 val Ҫд���ֽ�
//DS18B20�ֲ���������ʱ��ͼ
void write_byte(u8 val)
{
 u8 i,testb;
 for(i=8;i>0;i--)
 { 
   testb=val&0x01;            //���λ�Ƴ�
   val=val>>1;
   if(testb)				  //д1
   {
   	 DQ=0;
	 delay4us(1);			  //4us
	 DQ=1;
	 delay4us(15);			  //60us
   }
   else						  //д0
   {
     DQ=0;
	 delay4us(15);			  //60us
	 DQ=1;
	 delay4us(1);			  //4us
   }
 }
}

//DS18B20��1�ֽں���
//��������ȡ1���ֽ�
//����ֵΪ��ȡ�ֽ�ֵ
//˵��  һ��1bit�Ķ�ȡ������Ҫ60us	  ���ζ�ȡ֮����Ҫ����1us�Ļָ�ʱ��
//    ���ζ�ȡ1bit  �������Ͳ��ܳ���15us Ȼ����������
u8 read_byte(void)
{
 u8 i;
 u8 value=0;
 for(i=8;i>0;i--)
 {
  DQ=1;
  value>>=1;   //value=value>>1
  DQ=0;                                  
  delay4us(2);							 //����4us 
  DQ=1;                                  //����
  delay4us(3);							 //����10us	 ׼���������ߵ�ǰ����
  if(DQ)value|=0x80;					 //����ǰ����ֵ������ʱ����
  delay4us(13);                          //50 us��ʱ  ���һ�ζ�ȡ����ʱ��һ�ζ�ȡ����60us������1us�Ļָ�ʱ��
 }
 DQ=1;
 return(value);
}



//�����¶Ⱥ���
//����Ϊ�¶�ֵ  �¶�ֵΪshort���� ������
short read_temp()	  //short���Ա�ʾ-32768~+32767
{
  u8 TL,TH;
  u8 temp;
  short t;
  ow_reset();                  //���߸�λ
  ds18B20_check();			   //�ȴ�DSӦ��
  write_byte(0xcc);            //������
  write_byte(0x44);            //��ת������
  ow_reset();				   //��λ
  ds18B20_check();			   //�ȴ�DSӦ��
  write_byte(0xcc);            //������
  write_byte(0xbe);			   //���Ͷ��¶�����
  TL=read_byte();    //���¶�ֵ�ĵ��ֽ�
  TH=read_byte();    //���¶�ֵ�ĸ��ֽ�
  t=TH;

  if(TH>7)
  {
    TH=~TH;
    TL=~TL; 
    temp=0;//�¶�Ϊ��   
  }else temp=1;
  t<<=8;             
  t+=TL;             // ���ֽںϳ�һ�����ͱ�����
  t=(float)t*0.625;  //0.0625Ϊ12λ�¶Ȳɼ��ķֱ���	   tΪ�ɼ�����ֵ  ��������10����ȡС�����һλ
  if(temp)return t;
  else return -t;
  }


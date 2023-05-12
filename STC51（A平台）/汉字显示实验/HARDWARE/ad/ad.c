
#include "stc15f2k60s2.h"
#include "sys.h"
#include "intrins.h" 
#include "ad.h"
#include "gui.h"


                                   

/*Define ADC operation const for ADC_CONTR*/ 
#define ADC_POWER   0x80            //ADC power control bit 
#define ADC_FLAG    0x10            //ADC complete flag ģ��ת��������־λ 
#define ADC_START   0x08            //ADC start control bit  ģ��ת����������λ 


//ת���ٶȿ���λSPEED0��SPEED1��������״̬����Ӧ����ת���ٶ� 
#define ADC_SPEEDLL 0x00            //540 clocks  
#define ADC_SPEEDL  0x20            //360 clocks 
#define ADC_SPEEDH  0x40            //180 clocks 
#define ADC_SPEEDHH 0x60            //90 clocks 



/*---------------------------- 
Get ADC result 
----------------------------*/ 
u16 GetADCResult(u8 ch)                             //�������в��� ����ϸ������
{ 
    u16 Vo;
    P1ASF = 0x01;                                        //ѡ��P1�ڵ���һ�� ����Ŀں�chҪ��Ӧ���ܴﵽѡ��ÿ�
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;//0x00|0x00|ch|0x08:ѡ��A/D����ͨ������ʼA/Dת�� 
                                                         // ��ô��������Ҫԭ����ǲ���λѰַ
                                                         //   ͨ��ѡ���ں�3λ����ֱ����һ��������ʾch
                                                         //����ch=6 ��ô��Ӧ�ĺ���λ����110           
    _nop_();                             //Must wait before inquiry �� 
    _nop_();                             //����ADC_CONTR�Ĵ��������4��CPUʱ�����ڵ���ʱ�����ܱ�ֵ֤��д��ADC_CONTR�Ĵ��� 
    _nop_(); 
    _nop_(); 
    while (!(ADC_CONTR & ADC_FLAG));     //Wait complete flag 

   ADC_CONTR &= ~ADC_FLAG;               //Close ADC ����־λ����ȴ��´�Ӳ����1
                                         //Ҳ����д�� ADC_CONTR= ADC_CONTR & ( ~ADC_FLAG)
   Vo=(ADC_RES<<2)+ADC_RESL; 			 //��10λAD�ɼ�����   �����8λAD Vo=ADC_RESL ����
                                         //10λAD�ɼ� ��2��10�η� ��ֵΪ1024 ������1024��ʾ5���ĵ�ѹ 
										 //8λAD�ɼ� �� 2��8�η�  ��ֵΪ256  ��256��ʾ5��
    return Vo; 
} 

/*************************************************************************************/


//
//void adshow()
//{
// struct TFT_Pointer Tp_pix;  //���崥�������ṹ��
// u16 ad;
// GUI_Clear(White);	            //����
// tian(1);    //�����ɫ
// GUI_sprintf_hzstr16x(0,0,"AD test",White,1);
//
// GUI_sprintf_hzstr16x(20,56,"��ǰ��ѹֵ",Black,1);
// GUI_sprintf_hzstr16x(101,56,": .  V",Black,White);
// GUI_sprintf_hzstr16x(20,72,"�ɼ�������:",Blue,1);
//
// GUI_arcrectangle(18,122,200,60,6,Red,Red);	    //˵���ֱ߿�����
// GUI_arcrectangle(20,124,196,56,6,Black,Black);
//
// GUI_sprintf_hzstr16x(22,126,"P1.0����ΪAD��ѹ�ɼ���",Yellow,1);
// GUI_sprintf_hzstr16x(22,142,"�ɼ���ѹ��Χ0-5V",Yellow,1);
// GUI_sprintf_hzstr16x(22,158,"AD����10λ�ֱ���",Yellow,1);
//
// while(1)
// {
//  Tp_pix=TFT_Cm();	                            // ɨ�败����
//  if(Tp_pix.flag==1)                           //�Ƿ��д����¼� ���� 
//  {      			   
//   if(Tp_pix.x>200&&Tp_pix.x<239&&Tp_pix.y>295&&Tp_pix.y<320)
//   {
//     Return(); //���ض�̬Ч��
//	 break;	   //�������˵�
//   }	  
//  }
//
//  delay1ms(500);
//  ad=GetADCResult(0);			 //��ȡ��ѹ����ֵ  P1.0�ڲɼ�AD��ѹֵ
//  GUI_box( 109,72,160,88,White);									//���ϴ���ʾ
//  number(109,72,ad,Red,White) ;								    //��ʾ�ɼ�������
//
//			
//  ad=(float)ad*5*100/1024;       //5/1024 5v��ѹ��1024�� ֵΪÿһ�ݵ�ֵ adΪ�ɼ��ķ���  �������Ϊ�ɼ��ĵ�ѹֵ *100С�������λ 
//                                 //10λAD�ɼ� ��2��10�η� ��ֵΪ1024 ������1024��ʾ5���ĵ�ѹ 
//					             //��ô�òɼ���������ֵ ����1024 �ڳ���5 �õ���ֵ���ǲɼ��ĵ�ѹ��ֵ
//
//
//  GUI_sprintf_char(109,56,(ad/100%10)+'0',Red,White,0);	            //��ʾ��ѹֵ     
//  GUI_sprintf_char(125,56,(ad/10%10)+'0',Red,White,0);
//  GUI_sprintf_char(133,56,(ad%10)+'0',Red,White,0);
//
// }	 
//}



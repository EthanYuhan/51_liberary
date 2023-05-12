#include "stc15f2k60s2.h"
#include "sys.h"
#include "intrins.h" 
#include "ad.h"
 
                                   

/*Define ADC operation const for ADC_CONTR*/ 
#define ADC_POWER   0x80            //ADC power control bit 
#define ADC_FLAG    0x10            //ADC complete flag      ģ��ת��������־λ 
#define ADC_START   0x08            //ADC start control bit  ģ��ת����������λ 


//ת���ٶȿ���λSPEED0��SPEED1��������״̬����Ӧ����ת���ٶ� 
#define ADC_SPEEDLL 0x00            //540 clocks  
#define ADC_SPEEDL  0x20            //360 clocks 
#define ADC_SPEEDH  0x40            //180 clocks 
#define ADC_SPEEDHH 0x60            //90 clocks 



/*---------------------------- 
Get ADC result 
----------------------------*/ 
u16 GetADCResult(u8 ch)                                // 
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



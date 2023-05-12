
#include "stc15f2k60s2.h"
#include "sys.h"
#include "intrins.h" 
#include "ad.h"
#include "gui.h"


                                   

/*Define ADC operation const for ADC_CONTR*/ 
#define ADC_POWER   0x80            //ADC power control bit 
#define ADC_FLAG    0x10            //ADC complete flag 模数转换结束标志位 
#define ADC_START   0x08            //ADC start control bit  模数转换启动控制位 


//转换速度控制位SPEED0和SPEED1，共四种状态，对应四种转换速度 
#define ADC_SPEEDLL 0x00            //540 clocks  
#define ADC_SPEEDL  0x20            //360 clocks 
#define ADC_SPEEDH  0x40            //180 clocks 
#define ADC_SPEEDHH 0x60            //90 clocks 



/*---------------------------- 
Get ADC result 
----------------------------*/ 
u16 GetADCResult(u8 ch)                             //这里如有不懂 请仔细看资料
{ 
    u16 Vo;
    P1ASF = 0x01;                                        //选择P1口的哪一口 这里的口和ch要对应才能达到选择该口
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ch | ADC_START;//0x00|0x00|ch|0x08:选择A/D输入通道，开始A/D转换 
                                                         // 这么用语句的主要原因就是不能位寻址
                                                         //   通道选择在后3位所以直接用一个整数表示ch
                                                         //例如ch=6 那么对应的后三位就是110           
    _nop_();                             //Must wait before inquiry ， 
    _nop_();                             //设置ADC_CONTR寄存器后需加4个CPU时钟周期的延时，才能保证值被写入ADC_CONTR寄存器 
    _nop_(); 
    _nop_(); 
    while (!(ADC_CONTR & ADC_FLAG));     //Wait complete flag 

   ADC_CONTR &= ~ADC_FLAG;               //Close ADC 将标志位清零等待下次硬件置1
                                         //也可以写成 ADC_CONTR= ADC_CONTR & ( ~ADC_FLAG)
   Vo=(ADC_RES<<2)+ADC_RESL; 			 //打开10位AD采集功能   如果用8位AD Vo=ADC_RESL 即可
                                         //10位AD采集 即2的10次方 满值为1024 这里用1024表示5伏的电压 
										 //8位AD采集 即 2的8次方  满值为256  用256表示5伏
    return Vo; 
} 

/*************************************************************************************/


//
//void adshow()
//{
// struct TFT_Pointer Tp_pix;  //定义触摸变量结构体
// u16 ad;
// GUI_Clear(White);	            //清屏
// tian(1);    //下填充色
// GUI_sprintf_hzstr16x(0,0,"AD test",White,1);
//
// GUI_sprintf_hzstr16x(20,56,"当前电压值",Black,1);
// GUI_sprintf_hzstr16x(101,56,": .  V",Black,White);
// GUI_sprintf_hzstr16x(20,72,"采集数量基:",Blue,1);
//
// GUI_arcrectangle(18,122,200,60,6,Red,Red);	    //说明字边框设置
// GUI_arcrectangle(20,124,196,56,6,Black,Black);
//
// GUI_sprintf_hzstr16x(22,126,"P1.0口做为AD电压采集口",Yellow,1);
// GUI_sprintf_hzstr16x(22,142,"采集电压范围0-5V",Yellow,1);
// GUI_sprintf_hzstr16x(22,158,"AD采用10位分辨率",Yellow,1);
//
// while(1)
// {
//  Tp_pix=TFT_Cm();	                            // 扫描触摸屏
//  if(Tp_pix.flag==1)                           //是否有触摸事件 发生 
//  {      			   
//   if(Tp_pix.x>200&&Tp_pix.x<239&&Tp_pix.y>295&&Tp_pix.y<320)
//   {
//     Return(); //返回动态效果
//	 break;	   //返回主菜单
//   }	  
//  }
//
//  delay1ms(500);
//  ad=GetADCResult(0);			 //读取电压数量值  P1.0口采集AD电压值
//  GUI_box( 109,72,160,88,White);									//请上次显示
//  number(109,72,ad,Red,White) ;								    //显示采集数量基
//
//			
//  ad=(float)ad*5*100/1024;       //5/1024 5v电压份1024份 值为每一份的值 ad为采集的份数  与其相乘为采集的电压值 *100小数点后两位 
//                                 //10位AD采集 即2的10次方 满值为1024 这里用1024表示5伏的电压 
//					             //那么用采集到的数量值 除以1024 在乘以5 得到的值就是采集的电压数值
//
//
//  GUI_sprintf_char(109,56,(ad/100%10)+'0',Red,White,0);	            //显示电压值     
//  GUI_sprintf_char(125,56,(ad/10%10)+'0',Red,White,0);
//  GUI_sprintf_char(133,56,(ad%10)+'0',Red,White,0);
//
// }	 
//}



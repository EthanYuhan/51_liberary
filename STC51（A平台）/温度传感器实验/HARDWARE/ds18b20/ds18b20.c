
#include "stc15f2k60s2.h"
#include "sys.h"
#include "ds18b20.h"
#include "gui.h"


sbit DQ=P1^7;       //ds18b20 端口


//DS18B20复位函数
void ow_reset(void)
{

    DQ=1;                   //从高拉倒低
    DQ=0;
    delay4us(125);             //>480 us	  低电平复位信号
    DQ=1;
    delay4us(4);              //>15us 的上升沿	 15-60us高电平后  是60-240us 的应答信号
}



//等待DS18B20的回应
//返回1:未检测到DS18B20的存在
//返回0:存在
u8 ds18B20_check(void) 	   
{   
	u8 retry=0;	 		//检测计算变量
    while (DQ&&retry<50)
	{
		retry++;
		delay4us(1);		//大概4us
	};
	if(retry>=100)return 1;
	else retry=0;	 
    while (!DQ&&retry<60)		 //保持240us的延时 完成整个的复位过程
	{
		retry++;
		delay4us(1);		//大概4us
	};

	if(retry>=60)return 1;	    //没有接到DS18B20应答
	return 0;					//接到应答
}


//初始化DS18B20  同时检测DS的存在
//返回1:不存在
//返回0:存在   
u8 ds18b20_init(void)
{
  u8 m;
  ow_reset();         //复位总线
  m=ds18B20_check();  //等等ds应答
  return m;
}


//DS18B20写命令函数
//向1-WIRE 总线上写1个字节
//u8 val 要写入字节
//DS18B20手册最下面有时序图
void write_byte(u8 val)
{
 u8 i,testb;
 for(i=8;i>0;i--)
 { 
   testb=val&0x01;            //最低位移出
   val=val>>1;
   if(testb)				  //写1
   {
   	 DQ=0;
	 delay4us(1);			  //4us
	 DQ=1;
	 delay4us(15);			  //60us
   }
   else						  //写0
   {
     DQ=0;
	 delay4us(15);			  //60us
	 DQ=1;
	 delay4us(1);			  //4us
   }
 }
}

//DS18B20读1字节函数
//从总线上取1个字节
//返回值为读取字节值
//说明  一次1bit的读取最少需要60us	  两次读取之间需要至少1us的恢复时间
//    单次读取1bit  总线拉低不能超过15us 然后马上拉高
u8 read_byte(void)
{
 u8 i;
 u8 value=0;
 for(i=8;i>0;i--)
 {
  DQ=1;
  value>>=1;   //value=value>>1
  DQ=0;                                  
  delay4us(2);							 //拉低4us 
  DQ=1;                                  //拉高
  delay4us(3);							 //拉高10us	 准备接收总线当前数据
  if(DQ)value|=0x80;					 //将当前数据值存入临时变量
  delay4us(13);                          //50 us延时  完成一次读取的延时（一次读取最少60us）跳过1us的恢复时间
 }
 DQ=1;
 return(value);
}



//读出温度函数
//返回为温度值  温度值为short变量 有正负
short read_temp()	  //short可以表示-32768~+32767
{
  u8 TL,TH;
  u8 temp;
  short t;
  ow_reset();                  //总线复位
  ds18B20_check();			   //等待DS应答
  write_byte(0xcc);            //发命令
  write_byte(0x44);            //发转换命令
  ow_reset();				   //复位
  ds18B20_check();			   //等待DS应答
  write_byte(0xcc);            //发命令
  write_byte(0xbe);			   //发送读温度命令
  TL=read_byte();    //读温度值的低字节
  TH=read_byte();    //读温度值的高字节
  t=TH;

  if(TH>7)
  {
    TH=~TH;
    TL=~TL; 
    temp=0;//温度为负   
  }else temp=1;
  t<<=8;             
  t+=TL;             // 两字节合成一个整型变量。
  t=(float)t*0.625;  //0.0625为12位温度采集的分辨率	   t为采集的数值  这里扩大10倍提取小数点后一位
  if(temp)return t;
  else return -t;
  }



//DS1302部分
//整体 显示 时间 日期 年月 星期 
//可不停更新秒时间
//可进行触摸设置
#include "stc15f2k60s2.h" //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义
#include "ds1302.h"
#include "intrins.h"
#include "gui.h"
#include "sys.h"


code u8 write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //秒分时日月周年 最低位读写位
code u8 read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  

u8 ds1302tmp[7]={0,20,8,20,4,7,20};//秒分时日月周年		 2014年 4月 20日 8点20分 星期日  这个数是自己设定的


//写一个字节 
//temp 要写入的字节	 （地址或数据）
void Write_Ds1302_Byte(u8 temp) 
{
 u8 i;
 for (i=0;i<8;i++)     	//循环8次 写入数据
  { 
     SCK=0;
     SDA=temp&0x01;     //每次传输低字节 
     temp>>=1;  		//右移一位
     SCK=1;
   }
}   

//写入DS1302数据
//address 写入的地址
//dat     写入的数据
void Write_Ds1302( u8 address,u8 dat )     
{
 	CE=0;
	_nop_();
 	SCK=0;
	_nop_();
 	CE=1;						//启动
   	_nop_();                    
 	Write_Ds1302_Byte(address);	//发送地址
 	Write_Ds1302_Byte(dat);		//发送数据
 	CE=0;  		            //恢复
}

//读出DS1302数据
//address 读取数据的地址
//返回 读取的数据值
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

 	for (i=0;i<8;i++) 		//循环8次 读取数据
 	{		
 		if(SDA)
 		temp|=0x80;			//每次传输低字节
		SCK=0;
		temp>>=1;			//右移一位
		_nop_();
	   _nop_();
	   _nop_();
 		SCK=1;
	} 

 	CE=0;
	_nop_();	          	//以下为DS1302复位的稳定时间
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
	return (temp);			//返回
}


//写入时 DCB转换	
//DS1302 只接收 DCB码 即高4位放10位值 低4位放个位值
//add 要转换的地址
//返回 转换后的数据
u8 bcd_read(u8 add)
{
  u8 fla,fla2;
  fla=Read_Ds1302(add);
  fla2=((fla/16)*10)+(fla&0x0f);
  return fla2;	
}


//设定时钟数据
//主要设置时钟芯片里的 秒分时日月周年
void Set_RTC(void)		       //设定 日历
{
	u8 i;

	for(i=0;i<7;i++)         //BCD处理
	 ds1302tmp[i]=ds1302tmp[i]/10*16+ds1302tmp[i]%10;
	 
 	Write_Ds1302(0x8E,0X00);   //写使能
			
 	for(i=0;i<7;i++)		   //7次写入 秒分时日月周年
 	Write_Ds1302(write_rtc_address[i],ds1302tmp[i]);
	
    Write_Ds1302(0x8E,0x80);   //写禁止

}







//DS1302  时间 年月日 星期 整体显示函数
//此函数需要循环 进行更新 不能自动更新
//x y  显示的时间的初始坐标  星期在其上面显示  年月日 在星期上面显示
//如
//2013-12-12
//Friday
//20:25:32
//dcolor 字体颜色	  bgcolor  背景颜色
void ds1302_scan(u8 x,u16 y,u16 dcolor,u16 bgcolor)
{
    u8 i;
	//GUI_sprintf_hzstr16x(x+16,y,":  :",dcolor,bgcolor);  //显示秒 分 时 之间的 “：”	
	 Show_Str(x+16,y,dcolor,bgcolor,":  :",16,0);

	for(i=0;i<3;i++)						  //秒  分  时
	 number10(x+24*i,y,bcd_read(0x85-i*2),dcolor,bgcolor);
						 
				 
		switch(bcd_read(0x8b))					  //星期
		{
		 case 1:
		    //GUI_sprintf_hzstr16x(x,y-16,"Sunday",dcolor,bgcolor);
		    Show_Str(x,y-16,dcolor,bgcolor,"Sunday",16,0);
			break;
		 case 2:
		    //GUI_sprintf_hzstr16x(x,y-16,"Monday",dcolor,bgcolor);
		    Show_Str(x,y-16,dcolor,bgcolor,"Monday",16,0);
			break;
		 case 3:
		    //GUI_sprintf_hzstr16x(x,y-16,"Tuesday",dcolor,bgcolor);
			Show_Str(x,y-16,dcolor,bgcolor,"Tuesday",16,0);			
			break;
		 case 4:
		    //GUI_sprintf_hzstr16x(x,y-16,"Wednesday",dcolor,bgcolor);
			Show_Str(x,y-16,dcolor,bgcolor,"Wednesday",16,0);
			break;
		 case 5:
		    //GUI_sprintf_hzstr16x(x,y-16,"Thursday",dcolor,bgcolor);
			Show_Str(x,y-16,dcolor,bgcolor,"Thursday",16,0);
			break;
		 case 6:
		    //GUI_sprintf_hzstr16x(x,y-16,"Friday",dcolor,bgcolor);
			Show_Str(x,y-16,dcolor,bgcolor,"Friday",16,0);
			break;
		 case 7:
		    //GUI_sprintf_hzstr16x(x,y-16,"Saturday",dcolor,bgcolor);
			Show_Str(x,y-16,dcolor,bgcolor,"Saturday",16,0);
			break;
		}
		
		//GUI_sprintf_hzstr16x(x,y-32,"20  -  -",dcolor,bgcolor);	//显示日 月 年 之间的 “-”
		Show_Str(x,y-32,dcolor,bgcolor,"20  -  -",16,0);

		 
		 number10(x+16,y-32,bcd_read(0x8d),dcolor,bgcolor);		 //年
		 number10(x+16+24,y-32,bcd_read(0x89),dcolor,bgcolor);	 //月
		 number10(x+16+48,y-32,bcd_read(0x87),dcolor,bgcolor);	 //日								                           	
}




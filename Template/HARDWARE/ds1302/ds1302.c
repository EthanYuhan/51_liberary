
//DS1302部分  
//整体 显示 时间 日期 年月 星期 
//可不停更新秒时间

#include "stc15f2k60s2.h" 
#include "ds1302.h"
#include "intrins.h"
#include "sys.h"
#include "lcd12864.h"


//code u8 write_rtc_address[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c}; //秒分时日月周年 最低位读写位
//code u8 read_rtc_address[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};  
//u8 ds1302tmp[7]={50,59,23,20,4,7,20};//秒分时日月周年		 2020年 4月 20日 8点20分 星期日  这个数是自己设定的



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



/*

//设定时钟数据
//主要设置时钟芯片里的 秒分时日月周年
void Set_RTC(void)		       //设定 日历
{
	u8 i;

	for(i=0;i<7;i++)         //BCD处理
	 ds1302tmp[i]=ds1302tmp[i]/10*16+ds1302tmp[i]%10;  //ds1302tmp[7]={0,20,8,20,4,7,20};
	 
 	Write_Ds1302(0x8E,0X00);   //写使能
			
 	for(i=0;i<7;i++)		   //7次写入 秒分时日月周年
 	Write_Ds1302(write_rtc_address[i],ds1302tmp[i]);
	
    Write_Ds1302(0x8E,0x80);   //写禁止

}

*/




//DS1302  时间 显示函数
//此函数需要循环 进行更新 不能自动更新
//例20:25
void ds1302_scan(void)
{
    u8 temp;	
	
	temp = bcd_read(0x85);//时

	LCD12864_SetWindow(3,0);    //第4行	显示			
	LCD12864_WriteData(temp/10%10+0x30);			
	LCD12864_WriteData(temp%10+0x30);
	LCD12864_WriteData(':');
	
	temp = bcd_read(0x83);//分

	LCD12864_WriteData(temp/10%10+0x30);			
	LCD12864_WriteData(temp%10+0x30);	
	
}




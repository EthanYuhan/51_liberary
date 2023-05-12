#include  "stc15f2k60s2.h"
#include  "gui.h"
#include  "tft.h"
#include  "ov7670.h"
#include  "sys.h"
#include  "sccb.h"
#include  "h1838.h"
#include  "interrupt.h"


u8 sign_18_ov=0;     //中断函数选择标志

//中断函数
//说明 h1838 和ov7670  都用到了P3.2口外部中断功能 
void IR_IN() interrupt 0   
{

 if(sign_18_ov==0)		   //红外解码中断调用函数
 {
   u8 j,k;
   u16 N=0;
   u8 IRCOM[4];
   EX0 = 0;   		       //进入中断后关闭中断 进行解码
   delaylow(112);		   //24M或27M下大约7ms延时	9ms低电平前 检测P32状态
   if (IRIN==1) 		   //如果非0 说明红外错误 返回
   { EX0 =1;
	   return;
    } 
                            //确认IR信号出现
   while(!IRIN);            //等IR变为高电平，跳过9ms的前导低电平信号。   

   for (j=0;j<4;j++)         //收集四组数据
   { 
    for (k=0;k<8;k++)        //每组数据有8位
    {
     while (IRIN);           //等 IR 变为低电平，如果是第一次采集跳过4.5ms高电平信号。

     while (!IRIN);          //等 IR 变为高电平
     
     while (IRIN)            //计算IR高电平时长  犹豫单片机的运行 存在着计算时间
     {					    
       delaylow(5);         //27M下大约300+us 
       N++;           
       if(N>=30)
	    { EX0=1;		     //开中断
	     return;}            //0.14ms计数过长自动离开。

     }                       //高电平计数完毕                 
     IRCOM[j]=IRCOM[j] >> 1;      //数据最高位补“0”
     if (N>=6) 
     {IRCOM[j]=IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
    }
   } 
   if (IRCOM[2]!=~IRCOM[3])	//控制码 控制反码进行比对 如果错误跳出
   { EX0=1;return; }

   ma[1]=IRCOM[2] & 0x0F;      //取键码的低四位
   ma[0]=IRCOM[2] >> 4;        //右移4位 取高四位值

   if(ma[1]>9)
     ma[1]=ma[1]+0x37;		    //A 从65开始 十六进制为0x41
   else
	 ma[1]=ma[1]+0x30;

   if(ma[0]>9)
     ma[0]=ma[0]+0x37;
   else
	 ma[0]=ma[0]+0x30;
   hong=1;                //打开标志位
   EX0 = 1; 			  //开中断
  }

 else if(sign_18_ov==1)	   //摄像头中断调用函数
  {
   if(cur_status==0) //如果此时状态为0，则说明是一个图像的开始，开始向FIFO罐入数据
    {
     FIFO_WRST=0;	 //写复位
     FIFO_WRST=1;
     FIFO_WEN=1;	 //写FIFO使能
     cur_status=1;   //标记为1
    }
   else 
    if(cur_status==1)	//说明此处为图像的结束，亦即下一图像的开始
     {
       FIFO_WEN=0;		//写FIFO禁能
       FIFO_WRST=0;		//写复位
       FIFO_WRST=1;
       cur_status=2;	//标记为2  此时说明可以读取FIFO中的缓存数据
	   EX0 = 0; 		//关闭中断
     }
  
  }

}




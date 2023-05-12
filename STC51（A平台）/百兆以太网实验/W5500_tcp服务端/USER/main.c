
#include  "stc15f2k60s2.h"	
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include  "sd.h"
#include  "spi.h"
#include  "uart.h"
#include  "w5500.h"


/*			
百兆以太网实验-w5500网络模块TCP服务器模式  

详见README文档说明
*/



extern u8 SD_Type;			  //SD卡类型

u32 W5500_Send_Delay=0; //W5500发送延时计数变量(ms)



void IO_init(void)
{
	P0M0 = 0X00;	P0M1 = 0X00;

	P1M0 = 0X00;  P1M1 = 0X00;

	P2M0 = 0X00;	P2M1 = 0X00;

	P3M0 = 0X00;	P3M1 = 0X00;

	P4M0 = 0X00;	P4M1 = 0X00;  
	
	P5M0 = 0X00;	P5M1 = 0X00;  
	
	P6M0 = 0X00;	P6M1 = 0X00;  
	
	P7M0 = 0X00;	P7M1 = 0X00;   

}



//装载网络参数
//说明: 网关、子网掩码、物理地址、本机IP地址、本机端口号、目的IP地址、目的端口号、端口工作模式
void Load_Net_Parameters(void)
{
	Gateway_IP[0] = 192;//加载网关参数
	Gateway_IP[1] = 168;
	Gateway_IP[2] = 1;
	Gateway_IP[3] = 1;

	Sub_Mask[0]=255;//加载子网掩码
	Sub_Mask[1]=255;
	Sub_Mask[2]=255;
	Sub_Mask[3]=0;

	Phy_Addr[0]=0x0c;//加载物理地址
	Phy_Addr[1]=0x29;
	Phy_Addr[2]=0xab;
	Phy_Addr[3]=0x7c;
	Phy_Addr[4]=0x00;
	Phy_Addr[5]=0x02;

	IP_Addr[0]=192;//加载本机IP地址
	IP_Addr[1]=168;
	IP_Addr[2]=1;
	IP_Addr[3]=246;

	S0_Port[0] = 0x13;//加载端口0的端口号5000 
	S0_Port[1] = 0x88;

//	S0_DIP[0]=192;//加载端口0的目的IP地址
//	S0_DIP[1]=168;
//	S0_DIP[2]=1;
//	S0_DIP[3]=210;
//	
//	S0_DPort[0] = 0x17;//加载端口0的目的端口号6000
//	S0_DPort[1] = 0x70;

	S0_Mode=TCP_SERVER;//加载端口0的工作模式,TCP服务器模式
}


//W5500初始化配置
void W5500_Initialization(void)
{
	W5500_Init();		//初始化W5500寄存器
	Detect_Gateway();	//检查网关服务器 
	Socket_Init(0);		//指定Socket(0~7)初始化,初始化端口0
}




//描述    : W5500端口初始化配置
//说明 : 分别设置4个端口,根据端口工作模式,将端口置于TCP服务器、TCP客户端或UDP模式.
//		 从端口状态字节Socket_State可以判断端口的工作情况
void W5500_Socket_Set(void)
{
	if(S0_State==0)//端口0初始化配置
	{
		if(S0_Mode==TCP_SERVER)//TCP服务器模式 
		{
			if(Socket_Listen(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else if(S0_Mode==TCP_CLIENT)//TCP客户端模式 
		{
			if(Socket_Connect(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else//UDP模式 
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}
}




//W5500接收并发送接收到的数据
//s:端口号
//说明 : 本过程先调用S_rx_process()从W5500的端口接收数据缓冲区读取数据,
//		 然后将读取的数据从Rx_Buffer拷贝到Temp_Buffer缓冲区进行处理。
//		 处理完毕，将数据从Temp_Buffer拷贝到Tx_Buffer缓冲区。调用S_tx_process()
//		 发送数据。
void Process_Socket_Data(SOCKET s)
{
	u16 size;
	size=Read_SOCK_Data_Buffer(s, Rx_Buffer);
	memcpy(Tx_Buffer, Rx_Buffer, size);			
	Write_SOCK_Data_Buffer(s, Tx_Buffer, size);
}




void main()
{  
  SP=0X80;				      //调整堆栈指向   手册286页 详解
  IO_init();				   //真对 IAP15W4K61S4  IO口初始化

  Init_SPI(); 			      //SPI初始化
  UartInit();                 //串口初始化
	
	UartSendStr("W5500模块 TCP服务器模式\r\n");
	delay1ms(2000);
	Load_Net_Parameters();		//装载网络参数	
	W5500_Hardware_Reset();		//硬件复位W5500
	W5500_Initialization();		//W5500初始化配置

 
 	while(1)
	{

		W5500_Socket_Set();//W5500端口初始化配置

		W5500_Interrupt_Process();//W5500中断处理程序框架

		if((S0_Data & S_RECEIVE) == S_RECEIVE)//如果Socket0接收到数据
		{
			S0_Data&=~S_RECEIVE;
			Process_Socket_Data(0);//W5500接收并发送接收到的数据
		}		
		else if(W5500_Send_Delay>=20000)//定时发送字符串
		{
			if(S0_State == (S_INIT|S_CONN))
			{
				S0_Data&=~S_TRANSMITOK;
				memcpy(Tx_Buffer, "\r\nW5500服务器TEST\r\n", 30);	
				Write_SOCK_Data_Buffer(0, Tx_Buffer, 19);//指定Socket(0~7)发送数据处理,端口0发送30字节数据
			}
			W5500_Send_Delay=0;
		}	

		W5500_Send_Delay++;

	}

}



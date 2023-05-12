
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
������̫��ʵ��-w5500����ģ��TCP������ģʽ  

���README�ĵ�˵��
*/



extern u8 SD_Type;			  //SD������

u32 W5500_Send_Delay=0; //W5500������ʱ��������(ms)



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



//װ���������
//˵��: ���ء��������롢�����ַ������IP��ַ�������˿ںš�Ŀ��IP��ַ��Ŀ�Ķ˿ںš��˿ڹ���ģʽ
void Load_Net_Parameters(void)
{
	Gateway_IP[0] = 192;//�������ز���
	Gateway_IP[1] = 168;
	Gateway_IP[2] = 1;
	Gateway_IP[3] = 1;

	Sub_Mask[0]=255;//������������
	Sub_Mask[1]=255;
	Sub_Mask[2]=255;
	Sub_Mask[3]=0;

	Phy_Addr[0]=0x0c;//���������ַ
	Phy_Addr[1]=0x29;
	Phy_Addr[2]=0xab;
	Phy_Addr[3]=0x7c;
	Phy_Addr[4]=0x00;
	Phy_Addr[5]=0x02;

	IP_Addr[0]=192;//���ر���IP��ַ
	IP_Addr[1]=168;
	IP_Addr[2]=1;
	IP_Addr[3]=246;

	S0_Port[0] = 0x13;//���ض˿�0�Ķ˿ں�5000 
	S0_Port[1] = 0x88;

//	S0_DIP[0]=192;//���ض˿�0��Ŀ��IP��ַ
//	S0_DIP[1]=168;
//	S0_DIP[2]=1;
//	S0_DIP[3]=210;
//	
//	S0_DPort[0] = 0x17;//���ض˿�0��Ŀ�Ķ˿ں�6000
//	S0_DPort[1] = 0x70;

	S0_Mode=TCP_SERVER;//���ض˿�0�Ĺ���ģʽ,TCP������ģʽ
}


//W5500��ʼ������
void W5500_Initialization(void)
{
	W5500_Init();		//��ʼ��W5500�Ĵ���
	Detect_Gateway();	//������ط����� 
	Socket_Init(0);		//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
}




//����    : W5500�˿ڳ�ʼ������
//˵�� : �ֱ�����4���˿�,���ݶ˿ڹ���ģʽ,���˿�����TCP��������TCP�ͻ��˻�UDPģʽ.
//		 �Ӷ˿�״̬�ֽ�Socket_State�����ж϶˿ڵĹ������
void W5500_Socket_Set(void)
{
	if(S0_State==0)//�˿�0��ʼ������
	{
		if(S0_Mode==TCP_SERVER)//TCP������ģʽ 
		{
			if(Socket_Listen(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else if(S0_Mode==TCP_CLIENT)//TCP�ͻ���ģʽ 
		{
			if(Socket_Connect(0)==TRUE)
				S0_State=S_INIT;
			else
				S0_State=0;
		}
		else//UDPģʽ 
		{
			if(Socket_UDP(0)==TRUE)
				S0_State=S_INIT|S_CONN;
			else
				S0_State=0;
		}
	}
}




//W5500���ղ����ͽ��յ�������
//s:�˿ں�
//˵�� : �������ȵ���S_rx_process()��W5500�Ķ˿ڽ������ݻ�������ȡ����,
//		 Ȼ�󽫶�ȡ�����ݴ�Rx_Buffer������Temp_Buffer���������д���
//		 ������ϣ������ݴ�Temp_Buffer������Tx_Buffer������������S_tx_process()
//		 �������ݡ�
void Process_Socket_Data(SOCKET s)
{
	u16 size;
	size=Read_SOCK_Data_Buffer(s, Rx_Buffer);
	memcpy(Tx_Buffer, Rx_Buffer, size);			
	Write_SOCK_Data_Buffer(s, Tx_Buffer, size);
}




void main()
{  
  SP=0X80;				      //������ջָ��   �ֲ�286ҳ ���
  IO_init();				   //��� IAP15W4K61S4  IO�ڳ�ʼ��

  Init_SPI(); 			      //SPI��ʼ��
  UartInit();                 //���ڳ�ʼ��
	
	UartSendStr("W5500ģ�� TCP������ģʽ\r\n");
	delay1ms(2000);
	Load_Net_Parameters();		//װ���������	
	W5500_Hardware_Reset();		//Ӳ����λW5500
	W5500_Initialization();		//W5500��ʼ������

 
 	while(1)
	{

		W5500_Socket_Set();//W5500�˿ڳ�ʼ������

		W5500_Interrupt_Process();//W5500�жϴ��������

		if((S0_Data & S_RECEIVE) == S_RECEIVE)//���Socket0���յ�����
		{
			S0_Data&=~S_RECEIVE;
			Process_Socket_Data(0);//W5500���ղ����ͽ��յ�������
		}		
		else if(W5500_Send_Delay>=20000)//��ʱ�����ַ���
		{
			if(S0_State == (S_INIT|S_CONN))
			{
				S0_Data&=~S_TRANSMITOK;
				memcpy(Tx_Buffer, "\r\nW5500������TEST\r\n", 30);	
				Write_SOCK_Data_Buffer(0, Tx_Buffer, 19);//ָ��Socket(0~7)�������ݴ���,�˿�0����30�ֽ�����
			}
			W5500_Send_Delay=0;
		}	

		W5500_Send_Delay++;

	}

}



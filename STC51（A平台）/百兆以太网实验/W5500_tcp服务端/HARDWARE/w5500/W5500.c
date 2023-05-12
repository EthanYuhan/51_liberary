/**********************************************************************************
W5500����ģ�� ������

**********************************************************************************/

#include "stc15f2k60s2.h"
#include "spi.h"				
#include "W5500.h"
#include "sys.h"	
#include "uart.h"




/***************----- W5500 GPIO���� -----***************/
sbit W5500_RST = P3^4;	//����W5500��RST����
//sbit W5500_INT = P3^3;	//����W5500��INT���� �ж�δ�� ����ɨ��Ĵ�����ʽ 
sbit W5500_SCS = P1^2;	//����W5500��CS����	 
	
//SPI	   STC15 SPI��ӦIO����     Ӳ��SPI
//W5500_SCLK 	W5500��SCLK����		   P43
//W5500_MISO 	W5500��MISO���� 	   P41
//W5500_MOSI 	W5500��MOSI����	 	   P40



//��������������� 
u8 Gateway_IP[4];//����IP��ַ 
u8 Sub_Mask[4];	//�������� 
u8 Phy_Addr[6];	//�����ַ(MAC) 
u8 IP_Addr[4];	//����IP��ַ 

u8 S0_Port[2];	//�˿�0�Ķ˿ں�(5000) 
u8 S0_DIP[4];	//�˿�0Ŀ��IP��ַ 
u8 S0_DPort[2];	//�˿�0Ŀ�Ķ˿ں�(6000) 

u8 UDP_DIPR[4];	    //UDP(�㲥)ģʽ,Ŀ������IP��ַ
u8 UDP_DPORT[2];	//UDP(�㲥)ģʽ,Ŀ�������˿ں�


//�˿ڵ�����ģʽ
u8 S0_Mode =3;	            //�˿�0������ģʽ,0:TCP������ģʽ,1:TCP�ͻ���ģʽ,2:UDP(�㲥)ģʽ
#define TCP_SERVER	0x00	//TCP������ģʽ
#define TCP_CLIENT	0x01	//TCP�ͻ���ģʽ 
#define UDP_MODE	0x02	//UDP(�㲥)ģʽ 

//�˿ڵ�����״̬
u8 S0_State =0;	            //�˿�0״̬��¼,1:�˿���ɳ�ʼ��,2�˿��������(����������������) 
#define S_INIT		0x01	//�˿���ɳ�ʼ�� 
#define S_CONN		0x02	//�˿��������,���������������� 

//�˿��շ����ݵ�״̬
u8 S0_Data;	               	//�˿�0���պͷ������ݵ�״̬,1:�˿ڽ��յ�����,2:�˿ڷ���������� 
#define S_RECEIVE	 0x01	//�˿ڽ��յ�һ�����ݰ� 
#define S_TRANSMITOK 0x02	//�˿ڷ���һ�����ݰ���� 

//�˿����ݻ����� 
u8 Rx_Buffer[40];	//�˿ڽ������ݻ����� 
u8 Tx_Buffer[40];	//�˿ڷ������ݻ����� 

//u8 W5500_Interrupt;	//W5500�жϱ�־(0:���ж�,1:���ж�)


//ͨ��SPI1����һ���ֽ�
//dat ���͵��ֽ�
//�޷���
u8 SPI1_Send_Byte(u8 dat)
{
    return SPI_SendByte(dat);	//д1���ֽ�����
}


//PI1����2���ֽ�����(16λ)
//dat:�����͵�16λ����
void SPI1_Send_Short(u16 dat)
{
	SPI_SendByte(dat/256);//д���ݸ�λ   �൱��dat>>8
	SPI_SendByte(dat);	  //д���ݵ�λ
}


//ͨ��SPI1��W5500ָ����ַ�Ĵ���д1���ֽ�����
// reg:16λ�Ĵ�����ַ,dat:��д�������
void Write_W5500_1Byte(u16 reg, u8 dat)
{

	W5500_SCS=0; //CSƬѡW5500

	SPI1_Send_Short(reg);   //ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM1|RWB_WRITE|COMMON_R); //ͨ��SPI1д�����ֽ�,1���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	SPI1_Send_Byte(dat);  //д1���ֽ�����

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
}



//ͨ��SPI1��ָ����ַ�Ĵ���д2���ֽ�����
//reg:16λ�Ĵ�����ַ,dat:16λ��д�������(2���ֽ�)
void Write_W5500_2Byte(u16 reg, u16 dat)
{
	W5500_SCS=0; //CSƬѡW5500
		
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM2|RWB_WRITE|COMMON_R);//ͨ��SPI1д�����ֽ�,2���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���
	SPI1_Send_Short(dat);//д16λ����

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
}


//ͨ��SPI1��ָ����ַ�Ĵ���дn���ֽ�����
//reg:16λ�Ĵ�����ַ,*dat_ptr:��д�����ݻ�����ָ��,size:��д������ݳ���
void Write_W5500_nByte(u16 reg, u8 *dat_ptr, u16 size)
{
	u16 i;
	W5500_SCS=0; //CSƬѡW5500
		
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(VDM|RWB_WRITE|COMMON_R);//ͨ��SPI1д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��ͨ�üĴ���

	for(i=0;i<size;i++)//ѭ������������size���ֽ�����д��W5500
	{
		SPI1_Send_Byte(*dat_ptr++);//дһ���ֽ�����
	}

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
}



//ͨ��SPI1��ָ���˿ڼĴ���д1���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ,dat:��д�������
void Write_W5500_SOCK_1Byte(SOCKET s, u16 reg, u8 dat)
{
	W5500_SCS=0; //CSƬѡW5500
		
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM1|RWB_WRITE|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,1���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	SPI1_Send_Byte(dat);//д1���ֽ�����

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
}



//ͨ��SPI1��ָ���˿ڼĴ���д2���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ,dat:16λ��д�������(2���ֽ�)
void Write_W5500_SOCK_2Byte(SOCKET s, u16 reg, u16 dat)
{
	W5500_SCS=0; //CSƬѡW5500
			
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM2|RWB_WRITE|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,2���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���
	SPI1_Send_Short(dat);//д16λ����

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
}



//ͨ��SPI1��ָ���˿ڼĴ���д4���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ,*dat_ptr:��д���4���ֽڻ�����ָ��
void Write_W5500_SOCK_4Byte(SOCKET s, u16 reg, u8 *dat_ptr)
{
	W5500_SCS=0; //CSƬѡW5500
			
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM4|RWB_WRITE|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,4���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

	SPI1_Send_Byte(*dat_ptr++);//д��1���ֽ�����
	SPI1_Send_Byte(*dat_ptr++);//д��2���ֽ�����
	SPI1_Send_Byte(*dat_ptr++);//д��3���ֽ�����
	SPI1_Send_Byte(*dat_ptr++);//д��4���ֽ�����

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
}


//��ȡW5500ָ����ַ�Ĵ�����1���ֽ�����
//reg:16λ�Ĵ�����ַ
//����:��ȡ���Ĵ�����1���ֽ�����
u8 Read_W5500_1Byte(u16 reg)
{
	u8 i;

	W5500_SCS=0; //CSƬѡW5500
			
	SPI1_Send_Short(reg);  //ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM1|RWB_READ|COMMON_R);//ͨ��SPI1д�����ֽ�,1���ֽ����ݳ���,������,ѡ��ͨ�üĴ���

	i=SPI1_Send_Byte(0x00);//���Ϳ����� �õ��������ݸ�i

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
	return i;//���ض�ȡ���ļĴ�������
}



//��W5500ָ���˿ڼĴ�����1���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ
//����ֵ:��ȡ���Ĵ�����1���ֽ�����
u8 Read_W5500_SOCK_1Byte(SOCKET s, u16 reg)
{
	u8 i;

	W5500_SCS=0; //CSƬѡW5500
			
	SPI1_Send_Short(reg);   //ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM1|RWB_READ|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,1���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

	i=SPI1_Send_Byte(0x00);   //���Ϳ����� �õ��������ݸ�i

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
	return i;//���ض�ȡ���ļĴ�������
}



//��W5500ָ���˿ڼĴ�����2���ֽ�����
//s:�˿ں�,reg:16λ�Ĵ�����ַ
//����ֵ:��ȡ���Ĵ�����2���ֽ�����(16λ)
u16 Read_W5500_SOCK_2Byte(SOCKET s, u16 reg)
{
	u16 i;

	W5500_SCS=0; //CSƬѡW5500
			
	SPI1_Send_Short(reg);//ͨ��SPI1д16λ�Ĵ�����ַ
	SPI1_Send_Byte(FDM2|RWB_READ|(s*0x20+0x08));//ͨ��SPI1д�����ֽ�,2���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

	i=SPI1_Send_Byte(0x00);//���Ϳ����� 	��ȡ���ֽ�����
	i=i<<8;				   //ƫ�Ƹ��ֽ�����	
	i+=SPI1_Send_Byte(0x00);//���Ϳ�����    ��ȡ���ֽ�����

	W5500_SCS=1; //����CS  ȡ��Ƭѡ
	return i;//���ض�ȡ���ļĴ�������
}




//��W5500�������ݻ������ж�ȡ����
//s:�˿ں�,*dat_ptr:���ݱ��滺����ָ��
//����ֵ:��ȡ�������ݳ���,rx_size���ֽ�
u16 Read_SOCK_Data_Buffer(SOCKET s, u8 *dat_ptr)
{
	u16 rx_size;
	u16 offset, offset1;
	u16 i;
	u8 j;

	rx_size=Read_W5500_SOCK_2Byte(s,Sn_RX_RSR);
	if(rx_size==0) return 0;  //û���յ������򷵻�
	if(rx_size>1460) rx_size=1460;

	offset=Read_W5500_SOCK_2Byte(s,Sn_RX_RD);
	offset1=offset;
	offset&=(S_RX_SIZE-1);    //����ʵ�ʵ������ַ

	W5500_SCS=0; //CSƬѡW5500

	SPI1_Send_Short(offset);//д16λ��ַ
	j=SPI1_Send_Byte(VDM|RWB_READ|(s*0x20+0x18));//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���
	
	if((offset+rx_size)<S_RX_SIZE)//�������ַδ����W5500���ջ������Ĵ���������ַ
	{
		for(i=0;i<rx_size;i++)//ѭ����ȡrx_size���ֽ�����
		{
			j=SPI1_Send_Byte(0x00);//����һ��������
			//j=SPI_I2S_ReceiveData(SPI1);//��ȡ1���ֽ�����
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
	}
	else//�������ַ����W5500���ջ������Ĵ���������ַ
	{
		offset=S_RX_SIZE-offset;
		for(i=0;i<offset;i++)//ѭ����ȡ��ǰoffset���ֽ�����
		{
			j=SPI1_Send_Byte(0x00);//���Ϳ�����	  ��ȡ1���ֽ����ݸ�j
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
		W5500_SCS=1; //����CS  ȡ��Ƭѡ

		W5500_SCS=0; //CSƬѡW5500

		SPI1_Send_Short(0x00);//д16λ��ַ
		j=SPI1_Send_Byte(VDM|RWB_READ|(s*0x20+0x18));//д�����ֽ�,N���ֽ����ݳ���,������,ѡ��˿�s�ļĴ���

		for(;i<rx_size;i++)//ѭ����ȡ��rx_size-offset���ֽ�����
		{
			j=SPI1_Send_Byte(0x00);//���Ϳ�����	  ��ȡ1���ֽ����ݸ�j
			*dat_ptr=j;//����ȡ�������ݱ��浽���ݱ��滺����
			dat_ptr++;//���ݱ��滺����ָ���ַ����1
		}
	}
	W5500_SCS=1; //����CS  ȡ��Ƭѡ

	offset1+=rx_size;//����ʵ�������ַ,���´ζ�ȡ���յ������ݵ���ʼ��ַ
	Write_W5500_SOCK_2Byte(s, Sn_RX_RD, offset1);
	Write_W5500_SOCK_1Byte(s, Sn_CR, RECV);//����������������
	return rx_size;//���ؽ��յ����ݵĳ���
}



//������д��W5500�����ݷ��ͻ�����
//s:�˿ں�,*dat_ptr:���ݱ��滺����ָ��,size:��д�����ݵĳ���
void Write_SOCK_Data_Buffer(SOCKET s, u8 *dat_ptr, u16 size)
{
	u16 offset,offset1;
	u16 i;

	//�����UDPģʽ,�����ڴ�����Ŀ��������IP�Ͷ˿ں�
	if((Read_W5500_SOCK_1Byte(s,Sn_MR)&0x0f) != SOCK_UDP)//���Socket��ʧ��
	{		
		Write_W5500_SOCK_4Byte(s, Sn_DIPR, UDP_DIPR);//����Ŀ������IP  		
		Write_W5500_SOCK_2Byte(s, Sn_DPORTR, UDP_DPORT[0]*256+UDP_DPORT[1]);//����Ŀ�������˿ں�				
	}

	offset=Read_W5500_SOCK_2Byte(s,Sn_TX_WR);
	offset1=offset;
	offset&=(S_TX_SIZE-1);//����ʵ�ʵ������ַ

	W5500_SCS=0; //CSƬѡW5500

	SPI1_Send_Short(offset);//д16λ��ַ
	SPI1_Send_Byte(VDM|RWB_WRITE|(s*0x20+0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

	if((offset+size)<S_TX_SIZE)//�������ַδ����W5500���ͻ������Ĵ���������ַ
	{
		for(i=0;i<size;i++)//ѭ��д��size���ֽ�����
		{
			SPI1_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����		
		}
	}
	else//�������ַ����W5500���ͻ������Ĵ���������ַ
	{
		offset=S_TX_SIZE-offset;
		for(i=0;i<offset;i++)//ѭ��д��ǰoffset���ֽ�����
		{
			SPI1_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����
		}
		W5500_SCS=1; //����CS  ȡ��Ƭѡ

		W5500_SCS=0; //CSƬѡW5500

		SPI1_Send_Short(0x00);//д16λ��ַ
		SPI1_Send_Byte(VDM|RWB_WRITE|(s*0x20+0x10));//д�����ֽ�,N���ֽ����ݳ���,д����,ѡ��˿�s�ļĴ���

		for(;i<size;i++)//ѭ��д��size-offset���ֽ�����
		{
			SPI1_Send_Byte(*dat_ptr++);//д��һ���ֽڵ�����
		}
	}
	W5500_SCS=1; //����CS  ȡ��Ƭѡ

	offset1+=size;//����ʵ�������ַ,���´�д���������ݵ��������ݻ���������ʼ��ַ
	Write_W5500_SOCK_2Byte(s, Sn_TX_WR, offset1);
	Write_W5500_SOCK_1Byte(s, Sn_CR, SEND);//����������������				
}



//W5500Ӳ����λ
//˵����W5500�ĸ�λ���ű��ֵ͵�ƽ����500us����,����ʹW5500���븴λ״̬
void W5500_Hardware_Reset(void)
{
	u16 xun;
	W5500_RST=0;    //��λ��������
	delay1ms(50);
	W5500_RST=1;    //��λ��������
	delay1ms(200);
	while((Read_W5500_1Byte(PHYCFGR)&LINK)==0) //��ѭ���ȴ���̫���������	����Ҫ���������ӵ�·�ɺ�ģ���� �����޷�ͨ��
	{
	  xun++;
	  if(xun==40000)
	  {
	    UartSendStr("��̫������ʧ�ܣ�����δ�������ߣ�\r\n");
		xun=0;
	  }
	}
	UartSendStr("��̫�����ӳɹ���׼������ͨѶ״̬��\r\n");
	delay1ms(2000);

}




//��ʼ��W5500��Ӧ�Ĵ���
//˵����Ӳ����ʼ����Ҫ���������Ӧ�����ʼ��
void W5500_Init(void)
{
	u8 i=0;

	Write_W5500_1Byte(MR, RST);  //�����λW5500,��1��Ч,��λ���Զ���0
	delay1ms(10);                //��ʱ10ms

	//��������(Gateway)��IP��ַ,Gateway_IPΪ4�ֽ�
	//ʹ�����ؿ���ʹͨ��ͻ�������ľ��ޣ�ͨ�����ؿ��Է��ʵ��������������Internet
	Write_W5500_nByte(GAR, Gateway_IP, 4);
			
	//������������(MASK)ֵ,SUB_MASKΪ4�ֽ�
	//��������������������
	Write_W5500_nByte(SUBR,Sub_Mask,4);		
	
	//���������ַ,PHY_ADDRΪ6�ֽ�,����Ψһ��ʶ�����豸�������ֵַ
	//�õ�ֵַ��Ҫ��IEEE���룬����OUI�Ĺ涨��ǰ3���ֽ�Ϊ���̴��룬�������ֽ�Ϊ��Ʒ���
	//����Լ����������ַ��ע���һ���ֽڱ���Ϊż��
	Write_W5500_nByte(SHAR,Phy_Addr,6);		

	//���ñ�����IP��ַ,IP_ADDRΪ4�ֽ�
	//ע�⣬����IP�����뱾��IP����ͬһ�����������򱾻����޷��ҵ�����
	Write_W5500_nByte(SIPR,IP_Addr,4);		
	
	//���÷��ͻ������ͽ��ջ������Ĵ�С���ο�W5500�����ֲ� ��ʼ���ֱ�����Ϊ2k �ܹ�Ϊ16k
	for(i=0;i<8;i++)
	{
		Write_W5500_SOCK_1Byte(i,Sn_RXBUF_SIZE, 0x02);//Socket Rx memory size=2k
		Write_W5500_SOCK_1Byte(i,Sn_TXBUF_SIZE, 0x02);//Socket Tx mempry size=2k
	}

	//��������ʱ�䣬Ĭ��Ϊ2000(200ms) 
	//ÿһ��λ��ֵΪ100΢��,��ʼ��ʱֵ��Ϊ2000(0x07D0),����200����
	Write_W5500_2Byte(RTR, 0x07d0);

	//�������Դ�����Ĭ��Ϊ8�� 
	//����ط��Ĵ��������趨ֵ,�������ʱ�ж�(��صĶ˿��жϼĴ����е�Sn_IR ��ʱλ(TIMEOUT)�á�1��)
	Write_W5500_1Byte(RCR,8);

//	//�����жϣ��ο�W5500�����ֲ�ȷ���Լ���Ҫ���ж�����
//	//IMR_CONFLICT��IP��ַ��ͻ�쳣�ж�,IMR_UNREACH��UDPͨ��ʱ����ַ�޷�������쳣�ж�
//	//������Socket�¼��жϣ�������Ҫ���
//	Write_W5500_1Byte(IMR,IM_IR7 | IM_IR6);	//�ж�����Ĵ���
//	Write_W5500_1Byte(SIMR,S0_IMR);			//socket�ж�״̬0-7 �Ĵ���������S0��
//	Write_W5500_SOCK_1Byte(0, Sn_IMR, IMR_SENDOK | IMR_TIMEOUT | IMR_RECV | IMR_DISCON | IMR_CON);	//S0 �жϹ���ȫ����
}




//������ط�����
//����ģ��һ�˿ڵ���Ӧ�������ж�W5500ģʽ�Ƿ�������ͨѶ״̬��
//����ֵ:�ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
u8 Detect_Gateway(void)
{
	u8 ip_adde[4];
	ip_adde[0]=IP_Addr[0]+1;
	ip_adde[1]=IP_Addr[1]+1;
	ip_adde[2]=IP_Addr[2]+1;
	ip_adde[3]=IP_Addr[3]+1;

	//������ؼ���ȡ���ص������ַ
	Write_W5500_SOCK_4Byte(0,Sn_DIPR,ip_adde);//��Ŀ�ĵ�ַ�Ĵ���д���뱾��IP��ͬ��IPֵ
	Write_W5500_SOCK_1Byte(0,Sn_MR,MR_TCP);   //����socketΪTCPģʽ
	Write_W5500_SOCK_1Byte(0,Sn_CR,OPEN);     //��Socket	
	delay1ms(5);//��ʱ5ms 	
	
	if(Read_W5500_SOCK_1Byte(0,Sn_SR) != SOCK_INIT)//���socket��ʧ��	 ������TCPģʽʧ��
	{
		Write_W5500_SOCK_1Byte(0,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}

	Write_W5500_SOCK_1Byte(0,Sn_CR,CONNECT);//����SocketΪConnectģʽ	TCP��Ϊ�ͻ���					

	do
	{
		u8 j=0;
		j=Read_W5500_SOCK_1Byte(0,Sn_IR);//��ȡSocket0�жϱ�־�Ĵ���
		if(j!=0)						 //������ж���ʾ ��д1���Ӧ�ж�
		Write_W5500_SOCK_1Byte(0,Sn_IR,j);
		delay1ms(5);//��ʱ5ms 
		if((j&IR_TIMEOUT) == IR_TIMEOUT) //��ʱ
		{
			return FALSE;	
		}
		else if(Read_W5500_SOCK_1Byte(0,Sn_DHAR) != 0xff)	 //ֻ�ж�ĩλMAC �Ƿ�ΪFF ����ɹ���������Ӧ��ΪFF
		{
			Write_W5500_SOCK_1Byte(0,Sn_CR,CLOSE);//�ر�Socket	   ˵�����Գɹ�
			return TRUE;							
		}
	}while(1);
}




//ָ��Socket(0~7)��ʼ��
//s:����ʼ���Ķ˿�
void Socket_Init(SOCKET s)
{
	//���÷�Ƭ���ȣ��ο�W5500�����ֲᣬ��ֵ���Բ��޸�	
	Write_W5500_SOCK_2Byte(0, Sn_MSSR, 1460);//����Ƭ�ֽ���=1460(0x5b4)
	//����ָ���˿�
	switch(s)
	{
		case 0:
			//���ö˿�0�Ķ˿ں�
			Write_W5500_SOCK_2Byte(0, Sn_PORT, S0_Port[0]*256+S0_Port[1]);
			//���ö˿�0Ŀ��(Զ��)�˿ں�
			Write_W5500_SOCK_2Byte(0, Sn_DPORTR, S0_DPort[0]*256+S0_DPort[1]);
			//���ö˿�0Ŀ��(Զ��)IP��ַ
			Write_W5500_SOCK_4Byte(0, Sn_DIPR, S0_DIP);			
			
			break;

		case 1:
			break;

		case 2:
			break;

		case 3:
			break;

		case 4:
			break;

		case 5:
			break;

		case 6:
			break;

		case 7:
			break;

		default:
			break;
	}
}



//����ָ��Socket(0~7)Ϊ�ͻ�����Զ�̷���������
//s:���趨�Ķ˿�
//����ֵ:�ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
//˵��  :������Socket�����ڿͻ���ģʽʱ,���øó���,��Զ�̷�������������
//	     ����������Ӻ���ֳ�ʱ�жϣ��������������ʧ��,��Ҫ���µ��øó�������
//	     �ó���ÿ����һ��,�������������һ������
u8 Socket_Connect(SOCKET s)
{
	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_TCP);//����socketΪTCPģʽ
	Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket
	delay1ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_INIT)//���socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}
	Write_W5500_SOCK_1Byte(s,Sn_CR,CONNECT);//����SocketΪConnectģʽ
	return TRUE;//����TRUE,���óɹ�
}




//����ָ��Socket(0~7)��Ϊ�������ȴ�Զ������������
//s:���趨�Ķ˿�
//����ֵ:�ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
//˵��  :������Socket�����ڷ�����ģʽʱ,���øó���,�ȵ�Զ������������
//		 �ó���ֻ����һ��,��ʹW5500����Ϊ������ģʽ
u8 Socket_Listen(SOCKET s)
{
	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_TCP);//����socketΪTCPģʽ 
	Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket	
	delay1ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_INIT)//���socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}	
	Write_W5500_SOCK_1Byte(s,Sn_CR,LISTEN);//����SocketΪ����ģʽ	
	delay1ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_LISTEN)//���socket����ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//���ò��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}

	return TRUE;

	//���������Socket�Ĵ򿪺�������������,����Զ�̿ͻ����Ƿ�������������,����Ҫ�ȴ�Socket�жϣ�
	//���ж�Socket�������Ƿ�ɹ����ο�W5500�����ֲ��Socket�ж�״̬
	//�ڷ���������ģʽ����Ҫ����Ŀ��IP��Ŀ�Ķ˿ں�
}





//����ָ��Socket(0~7)ΪUDPģʽ
//s:���趨�Ķ˿�
//����ֵ:�ɹ�����TRUE(0xFF),ʧ�ܷ���FALSE(0x00)
//˵��  :���Socket������UDPģʽ,���øó���,��UDPģʽ��,Socketͨ�Ų���Ҫ��������
//		 �ó���ֻ����һ�Σ���ʹW5500����ΪUDPģʽ
u8 Socket_UDP(SOCKET s)
{
	Write_W5500_SOCK_1Byte(s,Sn_MR,MR_UDP);//����SocketΪUDPģʽ*/
	Write_W5500_SOCK_1Byte(s,Sn_CR,OPEN);//��Socket*/
	delay1ms(5);//��ʱ5ms
	if(Read_W5500_SOCK_1Byte(s,Sn_SR)!=SOCK_UDP)//���Socket��ʧ��
	{
		Write_W5500_SOCK_1Byte(s,Sn_CR,CLOSE);//�򿪲��ɹ�,�ر�Socket
		return FALSE;//����FALSE(0x00)
	}
	else
		return TRUE;

	//���������Socket�Ĵ򿪺�UDPģʽ����,������ģʽ��������Ҫ��Զ��������������
	//��ΪSocket����Ҫ��������,�����ڷ�������ǰ����������Ŀ������IP��Ŀ��Socket�Ķ˿ں�
	//���Ŀ������IP��Ŀ��Socket�Ķ˿ں��ǹ̶���,�����й�����û�иı�,��ôҲ��������������
}





//������:W5500_Interrupt_Process
//����  :W5500�жϴ��������
void W5500_Interrupt_Process(void)
{
	u8 i,j;

IntDispose:
//	W5500_Interrupt=0;//�����жϱ�־
//	i = Read_W5500_1Byte(IR);//��ȡ�жϱ�־�Ĵ���
//	Write_W5500_1Byte(IR, (i&0xf0));//��д����жϱ�־

//	if((i & CONFLICT) == CONFLICT)//IP��ַ��ͻ�쳣����
//	{
//		 //�Լ���Ӵ���
//	}
//
//	if((i & UNREACH) == UNREACH)//UDPģʽ�µ�ַ�޷������쳣����
//	{
//		//�Լ���Ӵ���
//	}

	i=Read_W5500_1Byte(SIR);//��ȡ�˿��жϱ�־�Ĵ���	
	if((i & S0_INT) == S0_INT)//Socket0�¼����� 
	{
		j=Read_W5500_SOCK_1Byte(0,Sn_IR); //��ȡSocket0�жϱ�־�Ĵ���
		Write_W5500_SOCK_1Byte(0,Sn_IR,j);//��д����жϱ�־

		if(j&IR_CON)//��TCPģʽ��,Socket0�ɹ����� 
		{
			S0_State|=S_CONN;//��������״̬0x02,�˿�������ӣ�����������������
		}
		if(j&IR_DISCON)//��TCPģʽ��Socket�Ͽ����Ӵ���
		{
			Write_W5500_SOCK_1Byte(0,Sn_CR,CLOSE);//�رն˿�,�ȴ����´����� 
			Socket_Init(0);		//ָ��Socket(0~7)��ʼ��,��ʼ���˿�0
			S0_State=0;//��������״̬0x00,�˿�����ʧ��
		}
		if(j&IR_SEND_OK)//Socket0���ݷ������,�����ٴ�����S_tx_process()������������ 
		{
			S0_Data|=S_TRANSMITOK;//�˿ڷ���һ�����ݰ���� 
		}
		if(j&IR_RECV)//Socket���յ�����,��������S_rx_process()���� 
		{
			S0_Data|=S_RECEIVE;//�˿ڽ��յ�һ�����ݰ�
		}
		if(j&IR_TIMEOUT)//Socket���ӻ����ݴ��䳬ʱ���� 
		{
			Write_W5500_SOCK_1Byte(0,Sn_CR,CLOSE);// �رն˿�,�ȴ����´����� 
			S0_State=0;//��������״̬0x00,�˿�����ʧ��
		}
	}

	if(Read_W5500_1Byte(SIR) != 0) 
		goto IntDispose;
}


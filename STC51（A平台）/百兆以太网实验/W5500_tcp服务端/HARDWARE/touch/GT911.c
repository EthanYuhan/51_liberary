
#include <stdio.h>
#include "GT911.h"
#include "gui.h"

/*****************************************************************************
 * @name       :void GT911_int_sync(u16 ms)
 * @date       :2019-06-20 
 * @function   :GT911 initialization synchronization
 * @parameters :ms:Synchronization delay
 * @retvalue   :None
******************************************************************************/
void GT911_int_sync(u16 ms)
{
	INT_CTRL=0;
	delay_ms(ms);
	
	
	
	P1M1 |=(1<<7);				  //����INTΪ����  1  0
	P1M0 &=~(1<<7);
	
	
}	

/*****************************************************************************
 * @name       :void GT911_reset_guitar(u8 addr)
 * @date       :2019-06-20 
 * @function   :GT911 reset
 * @parameters :add:IIC bus address
 * @retvalue   :None
******************************************************************************/
void GT911_reset_guitar(u8 addr)
{
	INT_CTRL=1;
	RST_CTRL=1;
	delay_ms(20);
	RST_CTRL=0;
	INT_CTRL=0;
	delay_ms(20);
	INT_CTRL=(addr == 0x28);
	delay_ms(20);
	RST_CTRL=1;
	delay_ms(20);
}

/*****************************************************************************
 * @name       :void GT911_reset(void)
 * @date       :2019-06-20 
 * @function   :GT911 reset
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void GT911_reset(void)
{
	RST_CTRL=0;
	delay_ms(10);   //10ms
	RST_CTRL=1;
	delay_ms(10);
}

/*****************************************************************************
 * @name       :void GT911_gpio_init(void)
 * @date       :2019-06-20 
 * @function   :GT911 IC gpio initialization
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void GT911_gpio_init(void)
{			
	GT_IIC_Init();
//	delay_us(2);
	GT911_reset();
}

/*****************************************************************************
 * @name       :u8 GT9XX_WriteHandle (u16 addr)
 * @date       :2019-06-20 
 * @function   :GT911 IC write handle
 * @parameters :addr:register address
 * @retvalue   :the status of writen
******************************************************************************/
u8 GT9XX_WriteHandle (u16 addr)
{
	u8 status;

	GT_IIC_Start();
	GT_IIC_Send_Byte(GT9XX_IIC_WADDR); //д����ָ��
	GT_IIC_Send_Byte((u8)(addr >> 8)); //д��16λ��ַ
	GT_IIC_Send_Byte((u8)addr);
	status = 1;
	return status;	
}

/*****************************************************************************
 * @name       :u8 GT9XX_WriteData (u16 addr,u8 value)
 * @date       :2019-06-20 
 * @function   :GT911 write data to register
 * @parameters :addr:register address
                value:the value for writen
 * @retvalue   :the status of writen
******************************************************************************/
u8 GT9XX_WriteData (u16 addr,u8 value)
{
	u8 status;
	GT_IIC_Start();

	GT9XX_WriteHandle(addr);
	GT_IIC_Send_Byte(value);
	GT_IIC_Stop();	
	status = 1;
	return status;
}

/*****************************************************************************
 * @name       :u8 GT9XX_ReadData (u16 addr, u8 cnt, u8 *value)
 * @date       :2019-06-20 
 * @function   :GT911 read data to register
 * @parameters :addr:register address
                cnt:the number of data which is read
                value:the point of read data
 * @retvalue   :the status of writen
******************************************************************************/
u8 GT9XX_ReadData (u16 addr, u8 cnt, u8 *value) 
{
	u8 status;
	u8 i;

	status = 0;
	GT_IIC_Start();
	GT9XX_WriteHandle(addr);
	GT_IIC_Start();
	delay_us(20);
	GT_IIC_Send_Byte(GT9XX_IIC_RADDR);
	//GT_IIC_Wait_Ack();     //������
	for(i = 0 ; i < cnt; i++)
	{
		if (i == (cnt - 1))
		{
			value[i] = GT_IIC_Read_Byte(0);
		}
		else
		{
			value[i] = GT_IIC_Read_Byte(1);
		}
	}					
	GT_IIC_Stop();
	status = 1;
	return (status);	
}

/*****************************************************************************
 * @name       :u8 GT911_Init(void)
 * @date       :2019-06-20 
 * @function   :GT911 IC initialization
 * @parameters :None
 * @retvalue   :the status of initialization
******************************************************************************/
u8 GT911_Init(void)
{
	u8 touchIC_ID[4]=0;	
	char str[16];
	GT911_gpio_init();
//	GT9xx_Eint_Init();
	GT911_reset_guitar(GT9XX_IIC_WADDR);
	GT911_int_sync(50);
	GT9XX_ReadData (GT9XX_ID_ADDR,4,touchIC_ID);
	
	
	if( touchIC_ID[0] == '9' )
	{
		sprintf(str,"ID:%s\r\n",touchIC_ID);		
        LCD_ShowString(lcddev.width-56,0,16,str,1);		
		//GT9xx_send_config();
		return 1;
	}
	else 
	{
		sprintf(str,"ID:%s\r\n",touchIC_ID);				
		LCD_ShowString(lcddev.width-56,0,16,"ERR",1);		
		return 0;
	}
	
}

/*****************************************************************************
 * @name       :u8 Touch_Get_Count(void)
 * @date       :2019-06-20 
 * @function   :get the count of read data
 * @parameters :None
 * @retvalue   :the count of read data
******************************************************************************/
u8 Touch_Get_Count(void)
{
	u8 count[1] = {0};
	GT9XX_ReadData (GT9XX_READ_ADDR,1,count);	//read touch data
	return (count[0]&0x0f);
}

const u16 TPX[] = {0x8150,0x8158,0x8160,0x8168,0x8170}; //���������������ݵ�ַ��1~5��

/*****************************************************************************
 * @name       :u8 GT911_Scan(void)
 * @date       :2019-06-20 
 * @function   :Query GT911 trigger status
 * @parameters :None
 * @retvalue   :the status of GT911 trigger
******************************************************************************/
//u8 GT911_Scan(void)  //��stm32��ֲ������
//{
//	u8 str[30];//����
//	u8 buf[42]; 
//	u8 i=0;
//	u8 res=0;
//	u8 temp;
//	u8 tempsta;
//	
////	u8 mode=0;
// 	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
//	t++;
//	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����   //�ĳ�20��������
//	{ 
//		GT9XX_ReadData(GT9XX_READ_ADDR, 42, buf);
//		if((buf[0]&0X80)&&((buf[0]&0XF)<6))
//		{	
//			temp=0;	
//			GT9XX_WriteData(GT9XX_READ_ADDR,temp);
//		}		
//		if((buf[0]&0XF)&&((buf[0]&0XF)<6))
//		{
//			for(i=0;i<(buf[0]&0XF);i++)
//			{
//				switch(buf[1+i*8])
//				{
//					case 4:
//					{
//						temp |= 1<<4;
//						break;
//					}
//					case 3:
//					{
//						temp |= 1<<3;
//						break;
//					}
//					case 2:
//					{
//						temp |= 1<<2;
//						break;
//					}
//					case 1:
//					{
//						temp |= 1<<1;
//						break;
//					}
//					case 0:
//					{	
//						temp |= 1<<0;
//						break;
//					}
//					default:
//					break;				
//				}
//			}
////			temp=0XFF<<(buf[0]&0XF);		//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
//			tempsta=tp_dev.sta;			//���浱ǰ��tp_dev.staֵ
//			tp_dev.sta=temp|TP_PRES_DOWN|TP_CATH_PRES; 
//			tp_dev.x[4]=tp_dev.x[0];	//���津��0������
//			tp_dev.y[4]=tp_dev.y[0];
//			for(i=0;i<5;i++)
//			{                                                                                                                                                                         
//				if(tp_dev.sta&(1<<i))	//������Ч?
//				{
//					//GT9XX_ReadData(TPX[i],4,buf);	//��ȡXY����ֵ  
//					if(lcddev.dir==0)
//					{
//						tp_dev.x[i]=((u16)buf[3+i*8]<<8)+buf[2+i*8];
//						tp_dev.y[i]=((u16)buf[5+i*8]<<8)+buf[4+i*8];
//					}
//					else if(lcddev.dir==1)
//					{
//						tp_dev.y[i]=lcddev.height-(((u16)buf[3+i*8]<<8)+buf[2+i*8]);
//						tp_dev.x[i]=((u16)buf[5+i*8]<<8)+buf[4+i*8];
//					}
//					else if(lcddev.dir==2)
//					{
//						tp_dev.x[i]=lcddev.width-(((u16)buf[3+i*8]<<8)+buf[2+i*8]);
//						tp_dev.y[i]=lcddev.height-(((u16)buf[5+i*8]<<8)+buf[4+i*8]);
//					}
//					else if(lcddev.dir==3)
//					{
//						tp_dev.y[i]=((u16)buf[3+i*8]<<8)+buf[2+i*8];
//						tp_dev.x[i]=lcddev.width-(((u16)buf[5+i*8]<<8)+buf[4+i*8]);						
//					}  
//					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
//					sprintf(str,"x[%d]:%i,y[%d]:%i ",i,(tp_dev.x[i]),i,(tp_dev.y[i]));		   //����	 
//					Show_Str(0, 32, BLACK,WHITE, str,16,0);
//				}
//			
//			}
//			res=1;
//			if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height)//�Ƿ�����(���곬����)
//			{ 
//				if((buf[0]&0XF)>1)		//��������������,�򸴵ڶ�����������ݵ���һ������.
//				{
//					tp_dev.x[0]=tp_dev.x[1];
//					tp_dev.y[0]=tp_dev.y[1];
//					t=0;				//����һ��,��������������10��,�Ӷ����������
//				}else					//�Ƿ�����,����Դ˴�����(��ԭԭ����)  
//				{
//					tp_dev.x[0]=tp_dev.x[4];
//					tp_dev.y[0]=tp_dev.y[4];
//					buf[0]=0X80;		
//					tp_dev.sta=tempsta;	//�ָ�tp_dev.sta
//				}
//			}else t=0;					//����һ��,��������������10��,�Ӷ����������
//		} 		
//	}	
//	if((buf[0]&0X8F)==0X80)//�޴����㰴��
//	{ 
//		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
//		{
//			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
//		}else						//֮ǰ��û�б�����
//		{ 
//			tp_dev.x[0]=0xffff;
//			tp_dev.y[0]=0xffff;
//			tp_dev.sta&=0XE0;	//�������Ч���	
//		}	
//	}
//	if(t>240)t=10;//���´�10��ʼ����
//	return res;


//}


u8 GT911_Scan(void)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	u8 tempsta;
	u8 mode=0;
 	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����   
	t++;
	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{ 
		GT9XX_ReadData(GT9XX_READ_ADDR, 1, &mode);
		if((mode&0X80)&&((mode&0XF)<6))
		{	
			temp=0;	
			GT9XX_WriteData(GT9XX_READ_ADDR,temp);
		}		
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFF<<(mode&0XF);		//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� 
			tempsta=tp_dev.sta;			//���浱ǰ��tp_dev.staֵ
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES; 
			tp_dev.x[4]=tp_dev.x[0];	//���津��0������
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//������Ч?
				{
					GT9XX_ReadData(TPX[i],4,buf);	//��ȡXY����ֵ
					if(lcddev.dir==0)
					{
						tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
						tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
					}
					else if(lcddev.dir==1)
					{
						tp_dev.y[i]=lcddev.height-(((u16)buf[1]<<8)+buf[0]);
						tp_dev.x[i]=((u16)buf[3]<<8)+buf[2];
					}
					else if(lcddev.dir==2)
					{
						tp_dev.x[i]=lcddev.width-(((u16)buf[1]<<8)+buf[0]);
						tp_dev.y[i]=lcddev.height-(((u16)buf[3]<<8)+buf[2]);
					}
					else if(lcddev.dir==3)
					{
						tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
						tp_dev.x[i]=lcddev.width-(((u16)buf[3]<<8)+buf[2]);						
					}  
					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}
			} 
			res=1;
			if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height)//�Ƿ�����(���곬����)
			{ 
				if((mode&0XF)>1)		//��������������,�򸴵ڶ�����������ݵ���һ������.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//����һ��,��������������10��,�Ӷ����������
				}else					//�Ƿ�����,����Դ˴�����(��ԭԭ����)  
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;		
					tp_dev.sta=tempsta;	//�ָ�tp_dev.sta
				}
			}else t=0;					//����һ��,��������������10��,�Ӷ����������
		}
	}
	if((mode&0X8F)==0X80)//�޴����㰴��
	{ 
		if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
		{
			tp_dev.sta&=~(1<<7);	//��ǰ����ɿ�
		}else						//֮ǰ��û�б�����
		{ 
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE0;	//�������Ч���	
		}	
	}
	if(t>240)t=10;//���´�10��ʼ����
	return res;
}



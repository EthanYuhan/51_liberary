
#include "gtiic.h" 

/*****************************************************************************
 * @name       :void GT_Delay(void)
 * @date       :2019-06-20 
 * @function   :Delay in controlling I2C speed
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void GT_Delay(void)
{
	//_nop_();_nop_();_nop_();_nop_();
	//delay_us(6);  //2 --> 6
	
	//delay2us(1);
	delay_us(1);
} 




//SDA�������
//  SDA         ��        P64           //���ݴ�����IIC������������
void GT_IO_SDA(u8 m)
{
 if(m==1)
 {
  P6M1 |=(1<<4);				  //����SDAΪ����  1  0
  P6M0 &=~(1<<4);
 }
 else
 {
  P6M1 &=~(1<<4);			 //�ָ�SDA˫��io     0 0
  P6M0 &=~(1<<4);
 }
}


/*****************************************************************************
 * @name       :void GT_IIC_Init(void)
 * @date       :2019-06-20 
 * @function   :Capacitive touch chip IIC interface initialization
 * @parameters :None
 * @retvalue   :None
******************************************************************************/	
void GT_IIC_Init(void)
{
	GT_IIC_SCL=1;
//	GT_Delay();
	GT_IIC_SDA=1;	
//	GT_Delay();	
}

/*****************************************************************************
 * @name       :void GT_IIC_Start(void)
 * @date       :2019-06-20  
 * @function   :Generating IIC starting signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void GT_IIC_Start(void)
{
	GT_IO_SDA(0);   //���
	GT_IIC_SDA=1;	
//	GT_Delay();	
	GT_IIC_SCL=1;
	GT_Delay();
 	GT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	GT_Delay();
	GT_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  

/*****************************************************************************
 * @name       :void GT_IIC_Stop(void)
 * @date       :2019-06-20 
 * @function   :Generating IIC stop signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void GT_IIC_Stop(void)
{ 
	GT_IO_SDA(0);   //���
	GT_IIC_SCL=0;
//	GT_Delay();
	GT_IIC_SDA=0;
	GT_Delay();
	GT_IIC_SCL=1;
	GT_Delay();
	GT_IIC_SDA=1;//STOP:when CLK is high DATA change form low to high 
	GT_Delay();
}

/*****************************************************************************
 * @name       :u8 GT_IIC_Wait_Ack(void)
 * @date       :2019-06-20 
 * @function   :Wait for the response signal
 * @parameters :None
 * @retvalue   :0-receive response signal successfully
								1-receive response signal unsuccessfully
******************************************************************************/ 
u8 GT_IIC_Wait_Ack(void)
{
	u8 ucErrTime=0; 
	GT_IO_SDA(1);			  //����SDAΪ����
	GT_IIC_SDA=1;GT_Delay();	   
	GT_IIC_SCL=1;GT_Delay();	 
	while(GT_IIC_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			GT_IIC_Stop();
			GT_IO_SDA(0);	 //�ָ�SDA˫��io
			return 1;
		} 
	}
	GT_IIC_SCL=0;//ʱ�����0 

	GT_IO_SDA(0);	 //�ָ�SDA˫��io	
	return 0;  
} 

/*****************************************************************************
 * @name       :void GT_IIC_Ack(void)
 * @date       :2019-06-20 
 * @function   :Generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 
void GT_IIC_Ack(void)
{
	GT_IIC_SCL=0;
//	GT_Delay();
	GT_IO_SDA(0);
	GT_IIC_SDA=0;
	GT_Delay();
	GT_IIC_SCL=1;
	GT_Delay();
	GT_IIC_SCL=0;
}

/*****************************************************************************
 * @name       :void GT_IIC_NAck(void)
 * @date       :2019-06-20 
 * @function   :Don't generate ACK response signal
 * @parameters :None
 * @retvalue   :None
******************************************************************************/ 	    
void GT_IIC_NAck(void)
{
	GT_IIC_SCL=0;
//	GT_Delay();
	GT_IO_SDA(0);
	GT_IIC_SDA=1;
	GT_Delay();
	GT_IIC_SCL=1;
	GT_Delay();
	GT_IIC_SCL=0;
}		
			 				     
/*****************************************************************************
 * @name       :void GT_IIC_Send_Byte(u8 txd)
 * @date       :2019-06-20  
 * @function   :send a byte data by IIC bus
 * @parameters :txd:Data to be sent
 * @retvalue   :None
******************************************************************************/			  
void GT_IIC_Send_Byte(u8 txd)
{                        
    u8 t; 
//		GT_Delay();
	GT_IO_SDA(0);  //����Ϊ���
    GT_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {  
      GT_IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 
			//GT_Delay();
			GT_IIC_SCL=1;//����ʱ�ӿ�ʼ���ݴ���
			GT_Delay();
	//		GT_IIC_SDA=CY;
	//		if((txd&0x80)>>7)
	//			GT_IIC_SDA=1;
	//		else
	//			GT_IIC_SDA=0;	
	//		GT_Delay();
	//		GT_IIC_SCL=1;
//			GT_Delay();
			GT_IIC_SCL=0;	
			GT_Delay();
//			txd<<=1; 
    }	
//		GT_IIC_SCL=0;
//		GT_Delay();
//		GT_IIC_SDA=1;
//		GT_Delay();
		GT_IIC_Wait_Ack();		
} 
	    
/*****************************************************************************
 * @name       :u8 GT_IIC_Read_Byte(unsigned char ack)
 * @date       :2019-06-20  
 * @function   :read a byte data by IIC bus
 * @parameters :ack:0-send nACK
									  1-send ACK
 * @retvalue   :Data to be read
******************************************************************************/	  
u8 GT_IIC_Read_Byte(unsigned char ack)
{
	u8 i,receive=0;
//	GT_IIC_SCL=0;
//	GT_Delay();
//	GT_IIC_SDA=1;
	
  GT_IO_SDA(1);					//����SDAΪ����
	
  for(i=0;i<8;i++)
	{
    GT_IIC_SCL=0; 	    	   
		delay4us(1); 
		GT_IIC_SCL=1;
		receive<<=1;
		if(GT_IIC_SDA)
			receive++;
//		else
//			receive|=0x00;
//		GT_IIC_SCL=0; 	    	   
//		GT_Delay();  
	}	  				 
	if (!ack)GT_IIC_NAck();//����nACK
	else GT_IIC_Ack(); //����ACK   
	
	
	GT_IO_SDA(0);					//�ָ�SDA˫��io
 	return receive;
}





























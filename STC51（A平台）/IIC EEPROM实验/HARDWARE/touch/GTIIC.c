
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




//SDA方向控制
//  SDA         接        P64           //电容触摸屏IIC总线数据引脚
void GT_IO_SDA(u8 m)
{
 if(m==1)
 {
  P6M1 |=(1<<4);				  //配置SDA为输入  1  0
  P6M0 &=~(1<<4);
 }
 else
 {
  P6M1 &=~(1<<4);			 //恢复SDA双向io     0 0
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
	GT_IO_SDA(0);   //输出
	GT_IIC_SDA=1;	
//	GT_Delay();	
	GT_IIC_SCL=1;
	GT_Delay();
 	GT_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	GT_Delay();
	GT_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
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
	GT_IO_SDA(0);   //输出
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
	GT_IO_SDA(1);			  //配置SDA为输入
	GT_IIC_SDA=1;GT_Delay();	   
	GT_IIC_SCL=1;GT_Delay();	 
	while(GT_IIC_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			GT_IIC_Stop();
			GT_IO_SDA(0);	 //恢复SDA双向io
			return 1;
		} 
	}
	GT_IIC_SCL=0;//时钟输出0 

	GT_IO_SDA(0);	 //恢复SDA双向io	
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
	GT_IO_SDA(0);  //设置为输出
    GT_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {  
      GT_IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 
			//GT_Delay();
			GT_IIC_SCL=1;//拉低时钟开始数据传输
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
	
  GT_IO_SDA(1);					//配置SDA为输入
	
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
	if (!ack)GT_IIC_NAck();//发送nACK
	else GT_IIC_Ack(); //发送ACK   
	
	
	GT_IO_SDA(0);					//恢复SDA双向io
 	return receive;
}





























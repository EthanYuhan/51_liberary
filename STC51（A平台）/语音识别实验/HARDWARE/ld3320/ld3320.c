
#include "stc15f2k60s2.h"
#include <stdio.h>
#include "spi.h"
#include "sys.h"	
#include "uart.h"
#include "ld3320.h"
#include "ld3320_test.h"





u8 nLD_Mode = LD_MODE_IDLE;//用来记录当前是在进行ASR识别还是在播放MP3


/**
  * @brief  写指令
  * @param  无
  * @retval 无
  */
void LD_WriteReg(u8 data1,u8 data2)
{
	LD_CS_L();
	//LD_SPIS_L();
	
  
	SPI_SendByte(0x04);
	SPI_SendByte(data1);
	SPI_SendByte(data2);
  
	LD_CS_H();
}

/**
  * @brief  读指令
  * @param  无
  * @retval 返回数据
  */
 u8 LD_ReadReg(u8 reg_add)
{
	u8 i;
  
	LD_CS_L();
	//LD_SPIS_L();

  
	SPI_SendByte(0x05);
	SPI_SendByte(reg_add);
	i=SPI_SendByte(0x00);
  
	LD_CS_H();
  
	return(i);
}


/**
  * @brief  语音识别模块语音识别功能与MP3播放功能共同配置
  * @param  无
  * @retval 无
  */
void LD_Init_Common(void)    
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	delay_us(10);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	delay_us(10);
	LD_WriteReg(0xCF, 0x43);   
	delay_us(10);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00); 
		LD_WriteReg(0x19, LD_PLL_MP3_19);   
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);   
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	  LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	delay_us(1);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c); 
	delay_us(10);
  
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}


/**
  * @brief  ld3320语音识别功能初始化
  * @param  无
  * @retval 无
  */
 void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);	
	delay_us(1);
  
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8);
  LD_WriteReg(0xB3,0x22 ); 
	delay_us(1);
}


/**
  * @brief  运行LD3320
  * @param  无
  * @retval 无
  */
u8 LD_Run(void)
{  
    LD_reset();
    delay_us(1);
  
		LD_AsrStart();			     //初始化ASR
		delay_us(1);
  
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
    {
      return 0;
    }
		
		delay_us(1);
		if (LD_AsrRun() == 0)
		{
			return 0;
		}
    UartSendStr("模块启动OK \r\n");
    
    return 1;    
}

/**
  * @brief  LD3320复位
  * @param  无
  * @retval 无
  */
 void LD_reset(void)
{
	LD_RST_H();
	delay2us(1);  //改成毫秒  
	LD_RST_L();
	delay2us(1);  
	LD_RST_H();
	delay2us(1);
  
	LD_CS_L();
	delay2us(1);  
	LD_CS_H();		
	delay2us(1);
}


/**
  * @brief  启动语音识别功能
  * @param  无
  * @retval 无
  */
 void LD_AsrStart(void)
{
	LD_Init_ASR();
}

/**
  * @brief  检查B2寄存器运行状态位
  * @param  无
  * @retval 无
  */
u8 LD_Check_ASRBusyFlag_b2(void)
{
	u8 j;
	u8 flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		delay_us(100);		
	}
	return flag;
}


///**
//  * @brief  ld3320寄存器操作
//  * @param  无
//  * @retval 无
//  */
// u8 spi_send_byte(u8 byte)
//{
//	while (SPI_I2S_GetFlagStatus(LD3320_SPI, SPI_I2S_FLAG_TXE) == RESET);
//	SPI_I2S_SendData(LD3320_SPI,byte);
//  
//	while (SPI_I2S_GetFlagStatus(LD3320_SPI,SPI_I2S_FLAG_RXNE) == RESET);
//	return SPI_I2S_ReceiveData(LD3320_SPI);
//}


/**
  * @brief  ld3320语音识别功能配置
  * @param  无
  * @retval 无
  */
 u8 LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	delay_us( 1);
	LD_WriteReg(0x08, 0x00);
	delay_us( 1);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);	
	LD_WriteReg(0x37, 0x06);
	delay_us(5);
  
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);
	LD_WriteReg(0xBD, 0x00);   
  
	return 1;

}

/**
  * @brief  写入识别列表
  * @param  无
  * @retval 1：写入列表成功，0：写入列表失败
  */
 u8 LD_AsrAddFixed(void)
{
	u8 k, flag;
	u8 nAsrAddLength;
	#define DATE_A 5   //数组二维数值
	#define DATE_B 20		//数组一维数值
	//添加关键词，用户修改
	u8  sRecog[DATE_A][DATE_B] = {
	 			"liu shui deng",\
				"shan shuo",\
				"dian deng",\
				"quan mie",\
				"kai deng "\
		
																	};	
	u8  pCode[DATE_A] = {
	 															1,	\
															  2,	\
																3,  \
																4,	\
																3		\
															};	//添加识别码，用户修改
  
	flag = 1;
                              
	for (k=0; k<DATE_A; k++)
	{			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}

		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0);
		LD_WriteReg(0x08, 0x04);
		delay_us(1);
    
		LD_WriteReg(0x08, 0x00);
		delay_us(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
      
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
	}	 
  UartSendStr("写入识别列表成功\r\n");
  
	return flag;
}




/**
  * @brief  ld3320配置
  * @param  无
  * @retval 无
  */
void LD3320_Config(void)
{

	

}


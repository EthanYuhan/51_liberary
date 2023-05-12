

#include "stc15f2k60s2.h"
#include <stdio.h>
#include "spi.h"
#include "sys.h"	
#include "uart.h"
#include "ld3320.h"
#include "ld3320_test.h"




u8   direct=0,process=0,flag=0; /*process语音识别模块工作状态,flag中断标志位,direct语音识别指令信息*/


/**
  * @brief  LD3320测试
  * @param  无
  * @retval 无
  */
void   LD3320_Task(void)
{ 
	u8   i=0;
	u8   j=0;
	u16  LD_ASR_Candi_Num; //从中断代码拿过来的

	UartSendStr("LD3320_Task...\r\n");
	delay1ms(1);	
	
	LD_WriteReg(0x29,0) ;   /*关闭中断*/
  LD_WriteReg(0x02,0) ;  
	
 
  while(1)
  {
    switch(process)
    { 
      case  0:         /*ld3320处于空闲状态*/
      i++;
      process=LD_Run();  
      if(i>=4)
      {
        UartSendStr("语音识别模块内部故障，请检查硬件连接 \r\n");
        return;
      }
      break;

      case  1:          /*ld3320语音识别成功*/

      if(flag)          //判断识别是否成功
      {          
        switch(direct)  /*对结果执行相关操作*/         
        {
          case 1:       /*命令“流水灯”*/
          UartSendStr(" 流水灯 指令识别成功\r\n");
          break;

          case 2:	      /*命令“闪烁”*/
          UartSendStr(" 闪烁 指令识别成功\r\n"); 
          break;

          case 3:	      /*命令“按键触发”*/
          UartSendStr(" 开灯/点灯  指令识别成功\r\n"); 
          break;

          case 4:		    /*命令“全灭”*/
          UartSendStr(" 全灭 指令识别成功\r\n");
          break;

          default:     
          break;
        } 
        process=flag=0;        
      }
      break;
      
      case  2:         /*ld3320语音识别失败*/
			i=0;
      if(flag)         
      {
        UartSendStr("未识别到有效信息或没有声音\r\n");
        process=flag=0;            
      } 
      break;      
                     
      default:
      break;  
    }
		
		if((process==1)&&(LD_ReadReg(0xB2)==0x21) && (LD_ReadReg(0xbf)==0x35))	
	  {
		  UartSendStr("检测到声音\r\n");	
		  
		  flag=1;     
		  
				LD_ASR_Candi_Num = LD_ReadReg(0xba); 
				if(LD_ASR_Candi_Num>0 && LD_ASR_Candi_Num<=4) 
				{ 
					direct = LD_ReadReg(0xc5);					
					process=1;    //识别成功    
					 
				}else
				process=2;  //识别失败
	  
	  /*完成一次语音识别后重新配置寄存器*/
		LD_WriteReg(0x2b,0);
		LD_WriteReg(0x1C,0);//写0:ADC不可用
		LD_WriteReg(0x29,0);
		LD_WriteReg(0x02,0);
		LD_WriteReg(0x2B,0);
		LD_WriteReg(0xBA,0);	
		LD_WriteReg(0xBC,0);	
		LD_WriteReg(0x08,1);//清除FIFO_DATA
		LD_WriteReg(0x08,0);//清除FIFO_DATA后 再次写0   	
	  }
		
		
    LD_Test();
   }
}
   
  

void  LD_Test(void)
{
  
  switch(direct)  //对结果执行相关操作
	{
		case 1:  //命令“流水灯” 
			Glide_LED();
		break;
    
		case 2:	  //命令“闪烁”
			Flicker_LED();
		break;
    
		case 3:	//命令“按键触发”  
			Open_LED();
		break;
    
		case 4:		//命令“全灭”
			Off_LED();
		break;
	  
		default:
    break;
	}	
}


/**
  * @brief  流水灯效果
  * @param  无
  * @retval 无
  */
 void Glide_LED(void)
 {
  P00 = 0;
	delay_ms(20);
  P00 = 1;
	delay_ms(20);	 
  P01 = 0;
	delay_ms(20);
  P01 = 1;
	delay_ms(20);
	P02 = 0;
	delay_ms(20);
  P02 = 1;
	delay_ms(20);
	P03 = 0;
	delay_ms(20);
  P03 = 1;
	delay_ms(20);
	P04 = 0;
	delay_ms(20);
  P04 = 1;
	delay_ms(20);	 
  P05 = 0;
	delay_ms(20);
  P05 = 1;
	delay_ms(20);
	P06 = 0;
	delay_ms(20);
  P06 = 1;
	delay_ms(20);
	P07 = 0;
	delay_ms(20);
  P07 = 1;
	delay_ms(20);
  
 }

/**
  * @brief  灯闪烁
  * @param  无
  * @retval 无
  */
void Flicker_LED(void)

{ 
  P00 = 0;
	delay_ms(200);
  P00 = 1;
	delay_ms(200);	
}

/**
  * @brief  开灯
  * @param  无
  * @retval 无
  */
void Open_LED(void)
{
  P00 = 0;
}

/**
  * @brief  等全部熄灭
  * @param  无
  * @retval 无
  */
void  Off_LED(void)
{

  P00 = 1;
	P01 = 1;
	P02 = 1;
	P03 = 1;
	P04 = 1;
	P05 = 1;
	P06 = 1;
	P07 = 1;

} 










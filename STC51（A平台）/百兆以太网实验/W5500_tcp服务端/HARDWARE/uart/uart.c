

#include "uart.h"


void UartInit(void)		
{
       SCON=0x50;        
	   TMOD= 0x00;                      
	   AUXR=0X40;		 
       TL1=(65535-(24000000/4/9600));     //24M ÆµÂÊ
	   TH1=(65535-(24000000/4/9600))>>8;
	   TR1  = 1;        
	   ES   = 1;                                                                            
	   EA   = 1;        	
	
}

void UartSendByte(u8 dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

void UartSendStr(u8 *str)
{
	while(*str)
	{
		UartSendByte(*str);
		str++;
	}
}

void UartSend(u8 *str ,u8 len)
{
	while(len--)
	{
		UartSendByte(*str);
		str++;
	}
}



/*****************************************************************************/
		 


void UART0_Interrupt(void) interrupt 4 using 1
{
	u8 SbufTemp;
	
	if(RI)
	{
		RI = 0;
		SbufTemp = SBUF;
	}
}

 



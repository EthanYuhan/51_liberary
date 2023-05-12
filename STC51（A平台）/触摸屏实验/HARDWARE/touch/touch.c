#include "sys.h"
#include "touch.h"
#include "lcd.h"
#include "gui.h"

_m_tp_dev tp_dev=
{
	0,
	0,
 	0,	 		
};					
//Ĭ��Ϊtouchtype=0������.
 
/*****************************************************************************
 * @name       :u8 TP_Init(void)
 * @date       :2018-08-09 
 * @function   :Initialization touch screen
 * @parameters :None
 * @retvalue   :0-no calibration
								1-Has been calibrated
******************************************************************************/  
u8 TP_Init(void)
{			    		   
	GT911_Init();
	return 0;
}

u8 TP_Scan(void)
{
	return GT911_Scan();
}
	    	

#ifndef __LD3320_TEST_H
#define __LD3320_TEST_H



//sck 		 P43  
//miso 		 P41  
//mosi     P40 
//IRQ      没接
//rst 		   P13 
//cs(NSS)    P12
//WR(SPIS)   接地



//与电容屏触摸 引脚控制引脚重合  ???

/***************----- LD3320 GPIO定义 -----***************/
sbit LD3320_RST = P1^3;	//定义LD3320的RST引脚
//sbit LD3320_IRQ = P3^2;	//定义LD3320的IRQ引脚
sbit LD3320_CS  = P1^2;	//定义LD3320的CS引脚	
sbit LD3320_SPIS = P1^2;	//定义LD3320的WR(SPIS)引脚


sbit SD_CS     = P4^6;  
//sbit W5500_CS  = P3^4;  //跟ld cs复用！

///LD3320引脚相关定义
#define LD_RST_H() 		LD3320_RST = 1 			
#define LD_RST_L() 		LD3320_RST = 0		

#define LD_CS_H()		LD3320_CS = 1 					
#define LD_CS_L()		LD3320_CS = 0		

//WR引脚   接地！！！
		     





#define CLK_IN   					20  /* user need modify this value according to clock in */
#define LD_PLL_11					(u8)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19			0x0f
#define LD_PLL_MP3_1B			0x18
#define LD_PLL_MP3_1D   	(u8)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(u8)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

#define MIC_VOL 0x20



void  LD3320_Task(void);
void  EXTI_Hander(void);
void  LD_Test(void);
void  Glide_LED(void);
void  Flicker_LED(void);
void  Open_LED(void); 
void  Off_LED(void);


#endif /* __LD3320_TEST_H */

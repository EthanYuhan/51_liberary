
//====================================TFT液晶屏数据线接线==========================================//
//本模块默认数据总线类型为16位并口
//液晶屏模块             C51单片机
// DB0~DB7       接       P00~P07        //数据低8位（8位模式：DB0~DB7接P00~P07）
// DB8~DB15      接       P20~P27        //数据高8位
//=======================================液晶屏控制线接线==========================================//
//液晶屏模块             C51单片机
//   CS          接        P54          //片选控制信号
//   RST         接        P35           //复位信号
//   RS          接        P55           //数据/命令选择控制信号
//   WR          接        P42           //写控制信号
//   RD          接        P44           //读控制信号
//   BL          接        VCC           //背光控制信号   51核心板TFT屏幕背光常亮 
//=========================================触摸屏接线==============================================//
//触摸屏使用的数据总线类型为IIC，51单片机采用模拟IIC总线通信
//触摸屏模块            C51单片机
//  SDA         接        P16           //电容触摸屏IIC总线数据引脚
//  SCL         接        P15           //电容触摸屏IIC总线时钟引脚
//  RST         接        P13           //电容触摸屏IC复位控制引脚（低电平复位）
//  INT         接        P17           //电容触摸屏中断检测引脚（发生触摸时为低电平）
//**************************************************************************************************/	

//*********此引脚说明放置lcd.h GT911.h gtiic.h三个文件中********************************************/	

#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"
#include  "stc15f2k60s2.h"



/////////////////////////////////////用户配置区///////////////////////////////////	 
//支持横竖屏快速定义切换
#define USE_HORIZONTAL  	  0 //定义液晶屏顺时针旋转方向 	0-0度旋转，1-90度旋转，2-180度旋转，3-270度旋转
#define LCD_USE8BIT_MODEL   0 	//定义数据总线是否使用8位模式 0,使用16位模式.1,使用8位模式
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////	  
//定义LCD的尺寸
#define LCD_W 320
#define LCD_H 480

//IO连接
#define  LCD_DataPortH P2     //高8位数据口,8位模式下只使用高8位 
#define  LCD_DataPortL P0     //低8位数据口	,8位模式下低8位可以不接线,请确认P0口已经上拉10K电阻,不宜太小，最小4.7K,推荐10K.
sbit LCD_RS = P5^5;  		 //数据/命令切换
sbit LCD_WR = P4^2;		  //写控制
sbit LCD_RD = P4^4;		     //读控制
sbit LCD_CS = P5^4;		//片选	
sbit LCD_RESET = P3^5;	      //复位   
//sbit LCD_BL=P4^7;		//背光控制，如果不需要控制，接3.3V   51核心板tft屏幕背光常亮

//LCD的画笔颜色和背景色	   
extern u16  POINT_COLOR;//默认红色    
extern u16  BACK_COLOR; //背景颜色.默认为白色
//LCD重要参数集
typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	 wramcmd;		//开始写gram指令
	u16  rramcmd;   //开始读gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令	 
}_lcd_dev; 	

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数
void LCD_Init(void); 
void LCD_Clear(u16 Color);
void LCD_write(u8 HVAL,u8 LVAL);
u16 LCD_read(void);
void LCD_WR_DATA(u16 Data);
u16 LCD_RD_DATA(void);
void LCD_WR_REG(u16 Reg);
void LCD_SetCursor(u16 Xpos, u16 Ypos);//设置光标位置
void LCD_SetWindows(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd);//设置显示窗口
void LCD_DrawPoint(u16 x,u16 y);//画点
u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_WriteRAM_Prepare(void);
void LCD_ReadRAM_Prepare(void);
void LCD_direction(u8 direction );
void LCD_WR_DATA_16Bit(u16 Data);
u16 Lcd_RD_DATA_16Bit(void);
void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
void LCD_ReadReg(u16 LCD_Reg,u8 *Rval,int n);
u16 Color_To_565(u8 r, u8 g, u8 b);
u16 LCD_Read_ID(void);


//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


					  		 
#endif  
	 
	 




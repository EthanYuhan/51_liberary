
/*			

		  	                                           		    	  
*/

#include "stc15f2k60s2.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "touch.h"
#include <stdio.h>
#include "string.h"
#include "sd.h"
#include "spi.h"
#include "pff.h" //文件系统调用.h加载

FATFS fatfs;  //文件系统结构体定义
u8 tbuf[512]; //512字节 SD卡数据缓存空间

void IO_init(void)
{
	P0M0 = 0X00;
	P0M1 = 0X00;

	P1M0 = 0X00;
  P1M1 = 0X00;

	P2M0 = 0X00;
	P2M1 = 0X00;

	P3M0 = 0X00;
	P3M1 = 0X00;

	P4M0 = 0X00;
	P4M1 = 0X00;  
	
	P5M0 = 0X00;
	P5M1 = 0X00;  
	
	P6M0 = 0X00;
	P6M1 = 0X00;  
	
	P7M0 = 0X00;
	P7M1 = 0X00; 
}

//显示BIN文件  bin文件大小为240x320
//path  bin文件名
//返回0 显示完成 1  读取失败
u8 play_bin(const char *path)
{

	FRESULT res;
	u16 br, i, y = 0;
	u8 x = 0;

	res = pf_open(path); //打开指定路径文件	 这一步可以打开任何路径下的文件 注意它的功能就是打开文件，不是文件夹
						 //读文件内数据的前提是必须要打开这个文件

	if (res == FR_OK)
	{

		LCD_SetWindows(0, 0, 239, 319); // 先扫横行 在扫纵行	   要符合BIN 生成方式 否则产生乱码

		while (1)
		{
			pf_read(tbuf, 512, &br); //512改成256 不行！
									 //上面 res=pf_open(path);打开的是指定路径下的文件，这里就可以直接读取文件的数据
									 //从初始位置读取512个字节到缓存区  再次读取无需偏移 自动跳到下512字节
									 //pf_read的特点：不仅可以读取路径下的文件数据，而在你不改变路径的前提它会自动加地址读取下面的数据
									 //从程序可以看出 程序在不停的循环pf_read(tbuf, 512, &br);函数来读取512个字节，
									 //那么它内部的工作就是从给的路径开始读去512个字节然后自动指向加512字节的路径，读取下512个字节直到图片显示完成

			for (i = 0; i < 256; i++) //然后写到液晶屏，可以显示256个像素，每个像素16位即2个字节 也就是每两个字节能显示一个像素点
			{
				LCD_WR_DATA(((u16)tbuf[i * 2 + 1] << 8) + tbuf[i * 2]); //显示颜色
				x++;
				if (x == 240)
				{
					y++;
					x = 0;
					if (y == 320)
						return 0;
				}
			}
		}
	}

	return 1;
}

//显示16位色BMP图片
//图片显示在 屏幕的正中位置
//进入bmp首扇区后 前4个字节为 为文件大小  18-21宽  22-25 高  28象素，其值为1、4、8、16、24、或32
//*path  bmp图片文件名
//返回 0  显示完成  1 显示出现问题
u8 play_bmp(const char *path)
{
	FRESULT res;

	u16 br, y = 0, zy, height, //width,height图片的初始左边
		y1, i1, tmp;		   //tmp 16位解码变量
	u8 x = 0, zx, width, x1,
	   rgb = 0,
	   Bmpcolor;

	res = pf_open(path); //打开指定路径文件	 这一步可以打开任何路径下的文件 注意它的功能就是打开文件，不是文件夹
						 //读文件内数据的前提是必须要打开这个文件

	if (res == FR_OK)
	{

		pf_read(tbuf, 54, &br); //取前54字节  前54字节含有bmp 文件大小 文件长宽尺度 像素值 等数据信息
		if (br != 54)
			return 1; //提取出错

		//实际宽和高都是用4个字节表示的，但是由于图片的大小不能超过屏的尺寸
		//这里只用一个字节表示宽度,两个字节表示高度
		width = tbuf[18];					 //计算文件宽度
		height = (tbuf[23] << 8) + tbuf[22]; //计算文件高度

		Bmpcolor = (u16)(tbuf[28] / 8); //获取颜色深度 一般是16位 24位 32位

		//将小于屏幕尺寸的图片放到屏幕正中间显示
		if (width < 239)
			zx = (240 - width) / 2;
		else
			zx = 0; //
		if (height < 319)
			zy = (320 - height) / 2;
		else
			zy = 0;

		x1 = zx;
		y1 = zy; //赋值计算后的值

		//BMP图片解码的扫描方式为 从下到上 从左到右
		LCD_Scan_Dir(4); //扫描方式为 从左到右   从下到上

		LCD_SetWindows(x1, y1, x1 + width - 1, y1 + height - 1); //设置显示范围 先扫横行 在扫纵行
		LCD_RS = 1;												 //写数据线拉高	 	  为提高写入速度 主循环前拉高

		while (1) //一直到最后一簇
		{

			pf_read(tbuf, 512, &br); //从54字节后位置读取512个字节到缓存区

			for (i1 = 0; i1 < 512; i1++)
			{
				if (Bmpcolor == 2) //16位BMP
				{
					switch (rgb) //555转565格式
					{
					case 0:
						tmp = ((u16)tbuf[i1] & 0x1f);		  //R
						tmp += (((u16)tbuf[i1]) & 0xe0) << 1; //G
						break;
					case 1:
						tmp += (u16)tbuf[i1] << 9; //B
						break;
					}
				}
				else if (Bmpcolor == 3) //24位BMP解码 RGB分别占8个字节
				{
					switch (rgb)
					{
					case 0:
						tmp = tbuf[i1] >> 3; //B
						break;
					case 1:
						tmp += ((u16)tbuf[i1] << 3) & 0x07e0; //G
						break;
					case 2:
						tmp += ((u16)tbuf[i1] << 8) & 0xf800; //R
						break;
					}
				}
				rgb++;

				if (rgb == Bmpcolor)
				{
					LCD_WR_DATA(tmp); //开始写入
					tmp = 0;
					rgb = 0;

					x1++; //横向自加 加满一行 横向清零 纵向自加 直到扫完整体图片
					if (x1 == width + zx)
					{
						y1--;
						x1 = zx;
						if (y1 == zy - height)
						{
							//恢复正常扫描方式
							//LCD显示方向设置
							LCD_Scan_Dir(1); //  左>右  上>下

							return 0; //显示完成
						}
					}
				}
			}
		}
	}
	return 1; //出错
}

//主函数
void main(void)
{
	SP = 0X80; //调整堆栈指向   手册286页 详解

	IO_init();	//IO口初始化
	LCD_Init(); //液晶屏初始化
	Init_SPI(); //SPI初始化

	LCD_direction(1); //横屏
	LCD_Clear(WHITE); //清屏白底

	SD_Init();		  //SD卡初始化
	pf_mount(&fatfs); //初始化petit FATFS文件系统  并提取tf卡相应数据

	get_font_sector(); //提取TF卡中  GBK点阵码  首扇区

	//利用tf字库进行汉字显示
	Show_Str(200, 0, BLUE, WHITE, "将进酒", 16, 0);
	Show_Str(0, 20, BLUE, WHITE, "君不见，黄河之水天上来，奔流到海不复回。", 16, 0);
	Show_Str(0, 40, BLUE, WHITE, "君不见，高堂明镜悲白发，朝如青丝暮成雪。", 16, 0);
	Show_Str(0, 60, BLUE, WHITE, "人生得意须尽欢，莫使金樽空对月。", 16, 0);
	Show_Str(0, 80, BLUE, WHITE, "天生我材必有用，千金散尽还复来。", 16, 0);
	Show_Str(0, 100, BLUE, WHITE, "烹羊宰牛且为乐，会须一饮三百杯。", 16, 0);
	Show_Str(0, 120, BLUE, WHITE, "岑夫子，丹丘生，将进酒，杯莫停。", 16, 0);
	Show_Str(0, 140, BLUE, WHITE, "与君歌一曲，请君为我倾耳听。", 16, 0);
	Show_Str(0, 160, BLUE, WHITE, "钟鼓馔玉不足贵，但愿长醉不复醒。", 16, 0);
	Show_Str(0, 180, BLUE, WHITE, "古来圣贤皆寂寞，惟有饮者留其名。", 16, 0);
	Show_Str(0, 200, BLUE, WHITE, "陈王昔时宴平乐，斗酒十千恣欢谑。", 16, 0);
	Show_Str(0, 220, BLUE, WHITE, "主人何为言少钱，径须沽取对君酌。", 16, 0);
	Show_Str(0, 240, BLUE, WHITE, "五花马、千金裘，", 16, 0);
	Show_Str(0, 260, BLUE, WHITE, "呼儿将出换美酒，与尔同销万古愁！", 16, 0);
	

	while (1)
		;
}

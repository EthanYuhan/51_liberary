#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "display12864.h"


/************************************************
 12864液晶实验
 详见README文档说明
************************************************/


int main(void)
{
    delay_init();	    //延时函数初始化
    delay_ms(100); 

    InitDis();
    DisStr("    你好    ");

    lcd_locate(2,1);
    DisStr("123456789");
    lcd_DisStr(4,2,"abcd");

    while(1);
}



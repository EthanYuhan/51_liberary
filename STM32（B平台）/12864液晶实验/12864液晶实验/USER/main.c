#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "display12864.h"


/************************************************
 12864Һ��ʵ��
 ���README�ĵ�˵��
************************************************/


int main(void)
{
    delay_init();	    //��ʱ������ʼ��
    delay_ms(100); 

    InitDis();
    DisStr("    ���    ");

    lcd_locate(2,1);
    DisStr("123456789");
    lcd_DisStr(4,2,"abcd");

    while(1);
}



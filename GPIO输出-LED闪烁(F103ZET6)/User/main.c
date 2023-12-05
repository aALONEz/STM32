#include "stm32f10x.h"                  // Device header
#include "LED.h"//引入LED的头文件
#include "Delay.h"//引入延时头文件

int main(void)
{
	LED_Init();
	while(1)
	{
		LED_Switch(ON);//LED点亮
		Delay_ms(200);//延时
		LED_Switch(OFF);//LED关闭
		Delay_ms(200);//延时
	}
}

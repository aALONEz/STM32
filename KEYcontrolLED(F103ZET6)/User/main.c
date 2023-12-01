#include "stm32f10x.h"                  // Device header
#include "LED.h"//包含LED模块的头文件
#include "KEY.h"//包含KEY头文件

int main(void)
{
	//调用LED初始化函数,初始化LED对应的引脚
	LED_init(RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_1);
	//调用KEY初始化函数,初始化KEY对应的引脚
	KEY_init(RCC_APB2Periph_GPIOE, GPIOE, GPIO_Pin_4);
	while(1)
	{
		if(get_KEY(GPIOE, GPIO_Pin_4) == 1)
		{
			//使用GPIO_ReadOutputDataBit获得LED引脚的电平状态
			if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0)//此时就是点亮的,因为低电平点亮
			{
				set_LED(GPIOA, GPIO_Pin_1, Bit_SET);
			}else
			{
				set_LED(GPIOA, GPIO_Pin_1, Bit_RESET);
			}
		}
	}
}

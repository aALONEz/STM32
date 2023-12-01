#include "stm32f10x.h"                  // Device header
#include "Delay.h"//引入Delay延时头文件

int main(void)
{
	//1.使能LED引脚对应端口的对应时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//2.初始化全部GPIO引脚
		//2.1创建GPIO_InitTypeDef结构体变量,并赋值后将其传递给GPIO_Init函数
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;//使用|符号将8个LED对应的引脚全部初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	while(1)
	{
		//3.让LED像流水一样闪烁
		for(int i = 0; i < 8; i++)
		{
			GPIO_Write(GPIOA, ~(1 << i));//由于是低电平点亮所以这里的结果要按位取反一下
			Delay_ms(50);//延时500ms
		}
	}
}

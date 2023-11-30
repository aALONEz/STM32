#include "stm32f10x.h"                  // Device header

int main(void)
{
	//1.使能对应GPIO组
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//2.设置GPIO模式等信息,并初始化GPIO。此处和蜂鸣器模块相连的引脚是PB12
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//3.控制IO口输出低电平,低电平导通三极管
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	//GPIO_SetBits(GPIOB, GPIO_Pin_12);//设置为高电平,如果你的蜂鸣器模块是高电平激活请解除这个注释,然后注释上面的.
	while(1)
	{
		
	}
}

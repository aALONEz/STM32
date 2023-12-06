#include "stm32f10x.h"                  // Device header

/*
* 函数名:ABZ_Init
* 功能:有源蜂鸣器初始化
* 参数:无
* 返回值:无
* 备注:有源蜂鸣器控制引脚连接PB8.
*/
void ABZ_Init(void)
{
	//有源蜂鸣器 PB8
	//1.使能对应GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//引脚编号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平变化速度50MHz
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//3.控制PB8输出低电平(高电平蜂鸣)
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);//低电平
}

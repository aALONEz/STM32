#include "stm32f10x.h"                  // Device header

void BEEP_Init(void)
{
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;//有源蜂鸣器连接引脚编号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO电平变化速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//3.设置默认不蜂鸣(高电平蜂鸣)
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);//低电平
}

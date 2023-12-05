#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	//LED 链接PB5 	低电平点亮	
	//1.使能GPIO时钟,GPIOB
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB时钟
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;//定义GPIO初始化结构体
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;//初始化的引脚编号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//GPIO速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//3.设置默认为高电平,不点亮
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

#include "stm32f10x.h"                  // Device header

void LED_Init(void)
{
	//LED PB5
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//GPIOB
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;//LED引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO模式,推完输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平变化速度,50MHz
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//3.控制LED引脚电平状态为高电平.(低电平点亮)
	GPIO_SetBits(GPIOB, GPIO_Pin_5);//高电平
}

uint8_t Get_LED_State(void)
{
	uint8_t LED_State = 0;
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == Bit_SET)//高电平就是没有点亮
	{
		LED_State = 0;//0表示没有点亮
	}
	else
	{
		LED_State = 1;//1表示点亮
	}
	return LED_State;
}

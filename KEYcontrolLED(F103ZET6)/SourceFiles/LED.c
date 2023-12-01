#include "stm32f10x.h"                  // Device header
#include "LED.h"

void LED_init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t LED_GPIO_Pin)
{
	//初始化外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);
	
	//初始化GPIO结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = LED_GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化结构体
	GPIO_Init(GPIOx, &GPIO_InitStruct);
	
	//设置LED对应引脚为高电平
	GPIO_WriteBit(GPIOx, LED_GPIO_Pin, Bit_SET);
}

void set_LED(GPIO_TypeDef* GPIOx, uint16_t LED_GPIO_Pin, BitAction BitVal)
{
	GPIO_WriteBit(GPIOx, LED_GPIO_Pin, BitVal);
}

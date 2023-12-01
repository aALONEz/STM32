#include "stm32f10x.h"                  // Device header
#include "BEEP.h"

//beep链接引脚PA5
void beep_init(void)
{
	//使能beep对应GPIO的时钟
	RCC_APB2PeriphClockCmd(beep_rcc, ENABLE);
	
	//初始化GPIO为输出模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = beep_gpio_pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(beep_gpio, &GPIO_InitStruct);
	
	//初始化beep默认电气属性,电平设置为低(beep高电平鸣叫)
	GPIO_WriteBit(beep_gpio, beep_gpio_pin, beep_off);
}

void set_beep(BitAction BitVal)
{
	GPIO_WriteBit(beep_gpio, beep_gpio_pin, BitVal);
}

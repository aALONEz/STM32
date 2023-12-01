#include "stm32f10x.h"                  // Device header
#include "LDR.h"

void ldr_init(void)
{
	//使能时钟
	RCC_APB2PeriphClockCmd(ldr_rcc, ENABLE);
	
	//初始化GPIO,配置为输入模式
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//如果模块一直安装着可以设置为浮空输入,光敏电阻传感器模块被遮挡时是高电平
	GPIO_InitStruct.GPIO_Pin = ldr_gpio_pin_digital;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式下这个参数无效
	
	GPIO_Init(ldr_gpio, &GPIO_InitStruct);
}

uint8_t get_ldr_digital(void)
{
	return GPIO_ReadInputDataBit(ldr_gpio, ldr_gpio_pin_digital);
}

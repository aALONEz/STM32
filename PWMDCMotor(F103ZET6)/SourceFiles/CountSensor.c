#include "stm32f10x.h"                  // Device header
#include "CountSensor.h"


void countsensor_init(void)
{
	//使能对应GPIO组外设时钟
	RCC_APB2PeriphClockCmd(countsensor_rcc, ENABLE);
	//使能AFIO外设时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//NVIC是内核外设，它是属于STM32内部的，所以不需要使能它的时钟。
	//EXTI的时钟，外部中断的触发并不依赖于EXTI模块的时钟。外部中断信号通过引脚直接连接到中断控制器（NVIC）中，
	//而不需要经过EXTI模块。当外部中断信号触发时，中断控制器会自动将控制权转移到相应的中断处理函数。
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = countsensor_gpio_pin_digital;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//此项在输入模式下无效
	
	GPIO_Init(countsensor_gpio, &GPIO_InitStruct);
	
	//配置AFIO
	GPIO_EXTILineConfig(countsensor_gpio_exti, countsensor_gpio_pin_exti);
	
	//配置EXTI
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = countsensor_exti_line;//使用那个中断通道
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//中断通道的状态,开启还是关闭
	EXTI_InitStruct.EXTI_Mode = countsensor_exti_mode;//中断响应方式,中断响应和事件响应
	EXTI_InitStruct.EXTI_Trigger = countsensor_exti_trigger;//中断触发方式
	
	EXTI_Init(&EXTI_InitStruct);
	
	//配置NVIC
		//NVIC分组设置已经放在主函数因为只需要设置一次
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = countsensor_nvic_irq;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = countsensor_nvic_irq_pp;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = countsensor_nvic_irq_sp;
	
	NVIC_Init(&NVIC_InitStruct);
}

uint8_t countsensor_get_digital(void)
{
	return GPIO_ReadInputDataBit(countsensor_gpio, countsensor_gpio_pin_digital);
}

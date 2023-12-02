#include "stm32f10x.h"                  // Device header
#include "RotaryEncoder.h"

void rotary_encoder_init(void)
{
	//使能对应外设时钟
	RCC_APB2PeriphClockCmd(rotary_encoder_rcc, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = rotary_encoder_gpio_pin_a | rotary_encoder_gpio_pin_b;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式无效
	
	GPIO_Init(rotary_encoder_gpio, &GPIO_InitStruct);
	
	//配置AFIO
	GPIO_EXTILineConfig(rotary_encoder_afio_gpio, rotary_encoder_afio_gpio_pin_a);
	GPIO_EXTILineConfig(rotary_encoder_afio_gpio, rotary_encoder_afio_gpio_pin_b);
	
	//配置EXTI
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = rotary_encoder_exti_line_a | rotary_encoder_exti_line_b;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = rotary_encoder_exti_mode;
	EXTI_InitStruct.EXTI_Trigger = rotary_encoder_exti_trigger;
	
	EXTI_Init(&EXTI_InitStruct);
	
	//配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = rotary_encoder_nvic_irq;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = rotary_encoder_nvic_irq_pp;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = rotary_encoder_nvic_irq_sp;
	
	NVIC_Init(&NVIC_InitStruct);
}

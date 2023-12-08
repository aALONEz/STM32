#include "stm32f10x.h"                  // Device header

/*
* 函数名:Encoder_Init
* 功能: 旋钮编码器 外部中断 初始化
* 参数: 无
* 返回值: 无
* 备注: A相连接PA0 B相连接PA1
*/
void Encoder_Init(void)
{
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA
	//初始化GPIO,为浮空输入模式
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式下无效
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinLockConfig(GPIOA, GPIO_Pin_0 | GPIO_Pin_1);
	
	//使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//配置AFIO连接EXTI
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//PA0
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//PA1
	//初始化EXTI
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line1 | EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStruct);
	
	
	//初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	//PA0
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	//PA1
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
}

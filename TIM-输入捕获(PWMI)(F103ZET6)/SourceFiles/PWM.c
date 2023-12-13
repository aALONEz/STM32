#include "stm32f10x.h"                  // Device header

/*
* 函数名:PWM_Init
* 功能:PWM输出初始化
* 参数:无
* 返回值:无
* 备注:PWM波形输出引脚PA0
*/
void PWM_Init(void)
{
	//1.使能GPIO时钟和TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//3.设置TIM2为时钟源
	TIM_InternalClockConfig(TIM2);//TIM2使用内部时钟源
	//4.初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟因子,一分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;//ARR自动重装载寄存器值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;//PSC预分频器值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//高级定时器重复计数器
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	//5.初始化输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM1模式
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//高电平
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//使能
	TIM_OCInitStruct.TIM_Pulse = 50;
	
	TIM_OC1Init(TIM2, &TIM_OCInitStruct);
	//6.使能TIM2定时器
	TIM_Cmd(TIM2, ENABLE);//TIM2使能
}

/*
* 函数名:PWM_SetPulse
* 功能:设置PWM输出脉冲值
* 参数:Pulse 脉冲值 0~65535
* 返回值:无
* 备注:无
*/
void PWM_SetPulse(uint16_t Pulse)
{
	TIM_SetCompare1(TIM2, Pulse);
}

/*
* 函数名:PWM_SetPeriod
* 功能:设置PWM自动重装器的值
* 参数:Period 脉冲值 0~65535
* 返回值:无
* 备注:无
*/
void PWM_SetPeriod(uint16_t Period)
{
	TIM_SetAutoreload(TIM2, Period);
}

/*
* 函数名:PWM_SetPrescaler
* 功能:设置PWM预分频器的值
* 参数:Prescaler 脉冲值 0~65535
* 返回值:无
* 备注:无
*/
void PWM_SetPrescaler(uint16_t Prescaler, uint16_t TIM_PSCReloadMode)
{
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode);
}

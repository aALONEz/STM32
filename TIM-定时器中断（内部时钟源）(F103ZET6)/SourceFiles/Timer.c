#include "stm32f10x.h"                  // Device header

/*
* 函数名：Timer_Init
* 功能：初始化定时器中断
* 参数：无
* 返回值：无
* 备注：（通用定时器）定时器2向上计数，更新中断
*/
void Timer_Init(void)
{
	//1.使能对应外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能TIM2时钟
	
	//2.设置定时器2的时基单元时钟源选择内部时钟
	TIM_InternalClockConfig(TIM2);
	
	//3.时基单元初始化
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频，用于将输入的时钟源进一步分频，与预分频不一样
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;//计数周期的值，自动重装载的值，0-65535.实际值是填入值加1
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;//预分频器的值，0-65535.实际值是填入值加1
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//重复计数器的值，高级定时器才有的功能，通用和基本定时器没有
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	//7.TIM_TimeBaseInit函数的实现在最后加上了手动生成更新事件（TIM_IT_Update）以重新加载预分频器和重复计数器。调用TIM_ClearFlag函数清除一下中断标志位
	TIM_ClearFlag(TIM2, TIM_IT_Update);
	
	//4.开启定时器中断，配置中断输出控制
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//更新中断 输出使能
	
	//5.配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//中断通道状态
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级/主占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;//响应优先级/从占优先级
	
	NVIC_Init(&NVIC_InitStruct);
	
	//6.开启定时器
	TIM_Cmd(TIM2, ENABLE);//定时器2 使能
}

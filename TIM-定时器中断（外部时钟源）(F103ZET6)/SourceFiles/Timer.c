#include "stm32f10x.h"                  // Device header

void Timer_Init(void)
{
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//GPIOE
	//2.使能TIM4时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//TIM4
	//3.初始化GPIO，设置为浮空输入模式，具体设置什么模式请参考STM32中文参考手册“外设的GPIO配置”章节。
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//引脚编号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平翻转速度，输入模式无效，但是这个成员也必须要赋值。
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	//4.设置外部时钟线路2为TIM4的时钟源。
	TIM_ETRClockMode2Config(TIM4, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);//TIM4时钟源设置为外部时钟源线路2，外部时钟源不分频，不翻转极性（极性：高低电平），滤波系数0
	//5.初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频，时钟因子，对外部时钟进行滤波的，//不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 10 - 1;//自动重装载寄存器的值，ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;//预分频器的值，PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//重复计数器的值，高级定时器的功能
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除初始化手动更新事件的标志位。
	//6.开启定时器中断
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	//8.配置NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd =ENABLE;//使能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级分组
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;//响应优先级分组
	
	NVIC_Init(&NVIC_InitStruct);
	//9.使能定时器
	TIM_Cmd(TIM4, ENABLE);//使能定时器4
}

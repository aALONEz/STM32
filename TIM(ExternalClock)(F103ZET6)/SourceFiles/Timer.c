#include "stm32f10x.h"                  // Device header
#include "Timer.h"

void timer_init(void)
{
	//1.使能RCC外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//2.配置时钟源
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);//参数2:外部分频系数,这里选择不分频   参数3:电平极性,就是高电平有效还是低电平有效
	//参数4.滤波系数,这里不使用滤波
	//3.配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频因子,用于滤波的和时基单元关系不大,这里默认填写一分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//定时器计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 10000 - 1;//目标值,计数器的目标,也就是设置自动重装载寄存器.取值范围0x0000 到 0xFFFF 十进制0~65535 16位寄存器。
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200 - 1;//预分频器PSC的值,取值范围0x0000 到 0xFFFF,也就是0~65535
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0X00;//重复计数器的值，高级定时器才有的功能，值必须在0X00~0XFF之间，TIM2是通用定时器，所以这不用，写0即可
	//上面这个结构体并没有计数器的设置，可以用涉及函数里面讲到的两个函数来操作计数器。计数器默认是0
	
	//计数器溢出频率：CK_CNT_OV = CK_CNT / (ARR + 1) = CK_PSC / (PSC + 1) / (ARR + 1)
	//对于STM32F103系列也就是 72M(内部时钟源 CK_CNT) / (PSC + 1) / (ARR + 1) 假设定时1S,也就是1000000us = 72000000000000 / 7200(PSC + 1) / 10000(ARR + 1)
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	//3.5由于上面TIM_TimeBaseInit函数在上电的时候就会立马执行一次更新事件（这个更新事件是为了让预分频器的值立马起作用而手动置中断标志位了）,
	//更新事件和更新中断是同时发生,所以就导致其实上电就会执行一次中断,这肯定是不行的
	//那么我们就在开启中断通道前面把这个中断标志位给清除了。
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//手动清除更新时间中断标志位,这样就可以防止刚初始化完就立马进一次中断.
	
	//4.使能中断,开启更新中断到NVIC的线路
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//参数2是中断的类型,详细请看涉及函数中对该函数详解.
	
	//6.NVIC初始化
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;//中断通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能/失能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;//响应优先级
	
	NVIC_Init(&NVIC_InitStruct);
	
	//7.启动定时器
	TIM_Cmd(TIM2, ENABLE);
}

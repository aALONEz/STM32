#include "stm32f10x.h"                  // Device header

void pwm_init(void)
{
	//1.使能对应时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能TIM3时钟
	//2.设置时基单元时钟源
	TIM_InternalClockConfig(TIM3);//设置TIM3定时器3为内部时钟源
	//3.配置时基单元,使用TIM_TimeBaseInit函数来初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//定时器时钟分频因子.
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;//自动重装载值,ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 36 - 1;//PSC,这里现在是20KHz,是不在人耳听到的范围内
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//高级定时器功能,重复计数值
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);//时基单元初始化
	
	//4.初始化输出比较单元,初始化第一个输出比较通道
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);//给上面结构体局部变量赋值初始值以免下面不给一些成员赋值导致错误
	//这里只赋值我们需要的结构体成员就可以了.
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//输出比较的模式,设置为PWM1
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//设置输出比较的极性。
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//设置输出使能,注意还有一个N使能那个是高级定时器的
	TIM_OCInitStruct.TIM_Pulse = 0;//输出比较通道脉冲宽度,也就是设置CCR
	
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	
	
	
	//5.使能定时器
	TIM_Cmd(TIM3, ENABLE);
}

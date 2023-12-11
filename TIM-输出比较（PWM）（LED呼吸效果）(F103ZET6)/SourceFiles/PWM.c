#include "stm32f10x.h"                  // Device header

/*
* 函数名：PWM_Init
* 功能 ：PWM初始化 此PWM用于控制LED
* 参数： 无
* 返回值：无
* 备注：LED正极链接PA1（TIM2_CH2），另一端连接GND
*/
void PWM_Init(void)
{
	//1.使能GPIO时钟与TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能TIM2时钟
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//这里要配置为复用推挽输出否则，片上外设定时器没有办法输出到PA1引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平反转速度
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);//PA1
	//3.配置时钟源
	TIM_InternalClockConfig(TIM2);//TIM2使用内部时钟源
	//4.初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;//自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;//预分频值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//高级定时器的重复计数器值
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	//5.配置输出比较,这里是TIM2_CH2引脚所以要调用TIM_OC2Init函数。
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);//输出比较初始化结构体，赋值初始值。
	//TIM_OCInitTypeDef结构体，如果使用的是通用定时器有很多成员是不需要填写的，请先调用上面的结构体初始化函数，再选择自己需要配置的成员配置即可。不需要全部成员都配置。
	
	TIM_OCInitStruct.TIM_OCIdleState = TIM_OCNIdleState_Reset;//输出空闲状态  低电平
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//输出比较模式    PWM1模式
	TIM_OCInitStruct.TIM_OCNIdleState = TIM_OCNIdleState_Reset;//互补输出空闲电平状态
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_Low;//高级定时器有的功能 互补输出极性 低电平
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性，粗略的理解为高电平还是低电平有效    高电平
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Disable;//高级定时器有的功能  N极性输出（互补输出）使能还是失能  失能
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//输出使能还是失能  使能
	TIM_OCInitStruct.TIM_Pulse = 0;//脉冲值可以理解为占空比 这里就是高电平占多久
	
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);
	//6.开启定时器
	TIM_Cmd(TIM2, ENABLE);//TIM2 使能
}

//7.自定义函数，在程序运行中
void Set_Pulse(uint16_t Pulse)
{
	TIM_SetCompare2(TIM2, Pulse);
}

#include "stm32f10x.h"                  // Device header

/*
* 函数名:Servo_Init
* 功能:舵机初始化,PWM初始化
* 参数:无
* 返回值:无
* 备注:舵机信号线链接PA6,使用定时器3_CH1口
*/
void Servo_Init(void)
{
	//1.使能GPIO时钟和TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能TIM3
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;//PA6
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平反转速度
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//3.设置TIM3时钟源
	TIM_InternalClockConfig(TIM3);//TIM3使用内部时钟源
	//4.初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//初始化TIM_TimeBaseInitStruct结构体成员
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 20000 - 1;//ARR自动重装载器值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;//PSC 预分频器值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//高级定时器的重复计数器的值
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	//5.配置输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);//初始化结构体成员值
	
	//下面是结构体成员只赋值我们需要用的.
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM1模式!
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性，高电平
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//输出状态,使能
	TIM_OCInitStruct.TIM_Pulse = 500;//脉冲值CCR
	
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	//6.使能定时器
	TIM_Cmd(TIM3, ENABLE);//使能定时器3
}

/*
* 函数名:Set_Servo_Angle
* 功能:设置舵机角度
* 参数:Angle角度值 0-180
* 返回值:无
* 备注:无
*/
void Set_Servo_Angle(uint16_t Angle)
{
	TIM_SetCompare1(TIM3, 500 + ((Angle / 180.0f) * 2000.0f));
}

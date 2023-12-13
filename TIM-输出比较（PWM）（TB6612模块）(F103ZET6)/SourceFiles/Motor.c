#include "stm32f10x.h"                  // Device header

//PWMA连接PA1 AIN2连接PE0 AIN1链接PE1 按键连接PE4
void Motor_Init(void)
{
	//1.使能GPIO和TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOE, ENABLE);//使能GPIOA(PWMA(PA1))和GPIOE(AIN1(PE1) AIN2(PE0))
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//使能TIM2时钟
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;//定义GPIO_InitTypeDef结构体变量
	
	//AIN1 AIN2
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//引脚电平反转速度
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//PWM
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平反转速度
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//3.设置TIM2时钟源
	TIM_InternalClockConfig(TIM2);//TIM2时钟源为内部时钟源
	
	//4.初始化时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟因子 一分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 100 - 1;//ARR
	TIM_TimeBaseInitStruct.TIM_Prescaler = 720 - 1;//PSC
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//高级定时器重复计数寄存器
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	
	//5.初始化输出比较
	TIM_OCInitTypeDef TIM_OCInitStruct;
	TIM_OCStructInit(&TIM_OCInitStruct);//结构体成员初始化赋值
	
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;//PWM1模式
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性,高电平
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;//输出使能
	TIM_OCInitStruct.TIM_Pulse = 0;//脉冲,高电平持续时间
	
	TIM_OC2Init(TIM2, &TIM_OCInitStruct);
	
	//6.使能TIM2
	TIM_Cmd(TIM2, ENABLE);//使能TIM2
}

/*
* 函数名: Set_Motor_Speed
* 功能:设置电机速度,-100~100
* 参数: Speed 速度值 -100~100
* 返回值:无
* 备注:设置电机速度,电机速度0对应点击停止.正数正转 负数反转
*/
void Set_Motor_Speed(int8_t Speed)
{
	if(Speed >= 0)//正转
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_1);//AIN1高电平
		GPIO_ResetBits(GPIOE, GPIO_Pin_0);//AIN2低电平
		TIM_SetCompare2(TIM2, Speed);
	}
	else//反转
	{
		GPIO_SetBits(GPIOE, GPIO_Pin_0);//AIN2高电平
		GPIO_ResetBits(GPIOE, GPIO_Pin_1);//AIN1低电平
		TIM_SetCompare2(TIM2, -Speed);
	}
}


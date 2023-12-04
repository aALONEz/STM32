#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"
#include "OLED.h"

void motor_set_speed(signed char speed);
signed char speed;//定义速度变量

int main(void)
{
	OLED_Init();
	pwm_init();
	
	//初始化GPIOA Pin6引脚为输出模式作为PWM输出引脚。
	//初始化GPIOE Pin4 Pin3引脚作为按键引脚
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能GPIOE时钟。
	//初始化GPIO,PWM输出引脚
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//这里作为PWM输出引脚要设置为复用推挽输出模式,而不是推挽输出模式.具体是因为只有复用开漏或者推挽输出模式下IO口的输出才能交给片上外设
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;//PWM信号线连接引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出电平翻转速率
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//初始化GPIO,按键引脚
	//GPIO_InitTypeDef GPIO_InitStruct;不需要再次定义结构体直接使用上面的结构体变量。
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;//按键连接引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式无效
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//初始化GPIO,TB6612的AIN1和AIN2引脚
	//GPIO_InitTypeDef GPIO_InitStruct;不需要再次定义结构体直接使用上面的结构体变量。
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;//AIN1和AIN2连接引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//引脚翻转速率
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
	while(1)
	{
		OLED_ShowSignedNum(1, 7, speed, 3);
		
		/*
		* 按下pin4引脚按键速度减,按下pin3引脚按键速度加,速度为负数时反转,为正数时正转
		* 速度范围-100~100,当大于100就自动归零,小于-100自动归零
		*/
		//速度减按键
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)//是低电平是代表按键按下
		{
			Delay_ms(20);//延时20ms,消抖
			if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)//消抖之后还是按下状态说明是按键按下
			{
				while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET);//如果不松开按键就会一直进入循环,松开不进入循环。
				Delay_ms(20);//延时20ms,松手消抖
				speed -= 10;//按键按下就让速度减10;
			}
		}
		//速度加按键
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)//是低电平是代表按键按下
		{
			Delay_ms(20);//延时20ms,消抖
			if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)//消抖之后还是按下状态说明是按键按下
			{
				while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET);//如果不松开按键就会一直进入循环,松开不进入循环。
				Delay_ms(20);//延时20ms,松手消抖
				speed += 10;//按键按下就让速度加10;
			}
		}
		if(speed > 100 || speed < -100)//判断速度越界情况
        {
			speed = 0;
        }
		motor_set_speed(speed);
	}
}

void motor_set_speed(signed char speed)//speed -100~100范围,负数为反转,正数正转
{
	if(speed >= 0)//正转
	{
		//调用GPIO_SetBits函数设置AIN1引脚为高电平
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		//调用GPIO_ResetBits函数设置AIN2引脚为低电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		//调用TIM_SetCompare1函数设置PWM的占空比以调整其速度
		TIM_SetCompare1(TIM3, speed);
	}
	else
	{
		//调用GPIO_SetBits函数设置AIN2引脚为高电平
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		//调用GPIO_ResetBits函数设置AIN1引脚为低电平
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		//调用TIM_SetCompare1函数设置PWM的占空比以调整其速度
		TIM_SetCompare1(TIM3, -speed);//负数这里要加一个负号
	}
}

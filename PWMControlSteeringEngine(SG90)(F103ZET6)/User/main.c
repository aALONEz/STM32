#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"
#include "OLED.h"

void servo_set_angle(float angle);
float angle = 0;//角度变量

int main(void)
{
	OLED_Init();
	pwm_init();
	
	//初始化GPIOA Pin0引脚为输出模式作为PWM输出引脚。
	//初始化GPIOE Pin4引脚作为按键引脚
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA时钟。
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能GPIOE时钟。
	//初始化GPIO,PWM输出引脚
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//这里作为PWM输出引脚要设置为复用推挽输出模式,而不是推挽输出模式.具体是因为只有复用开漏或者推挽输出模式下IO口的输出才能交给片上外设
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;//舵机信号线连接引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输出电平翻转速率
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//初始化GPIO,PWM输出引脚
	//GPIO_InitTypeDef GPIO_InitStruct;不需要再次定义结构体直接使用上面的结构体变量。
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;//按键连接引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式无效
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	//调用TIM_SetCompare1函数
	TIM_SetCompare1(TIM2, 500);
	
	while(1)
	{
		OLED_ShowNum(1, 5, angle, 5);
		/*
		* 每按键一次就让舵机角度加10,到180度自动归零
		*/
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)//Bit_RESET 低电平
		{
			Delay_ms(20);//延时20毫米消抖
			if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)//延时消抖之后再次判断是不是还是按键按下的状态
			{
				Delay_ms(20);
				while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET);//按键放下后才结束循环,以免按键按住不放.如果你想实现按键按住不放可以连续增长可以取消的循环
				angle += 10;
				if(angle > 180)
				{
					angle = 0;
				}
				servo_set_angle(angle);
			}
		}
		
	}
}

void servo_set_angle(const float angle)
{
	TIM_SetCompare1(TIM2, 500 + ((angle / 180) * 2000));//让角度除舵机的最大角度,就得到角度在最大角度的比例,然后这个比例再乘上2500-500的差2000,就是这个比例在0-2000的值,再加500就是500-2500的比例
}

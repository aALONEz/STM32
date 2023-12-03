#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"

uint8_t i = 0;

uint32_t countsensor_count = 0;//中断触发变量

int main(void)
{
	pwm_init();
	
	//初始化GPIOA Pin1引脚为输出模式作为PWM输出引脚,这里使用LED检测定时器输出PWM,正常的话LED会有呼吸效果
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//这里作为PWM输出引脚要设置为复用推挽输出模式,而不是推挽输出模式.具体是因为只有复用开漏或者推挽输出模式下IO口的输出才能交给片上外设
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	while(1)
	{
		//调用TIM_SetCompare2函数在循环中不断更改CCR的值,以达到呼吸灯的效果
		for(i = 0; i <= 100; i++)
		{
			TIM_SetCompare2(TIM2, i);
			Delay_ms(10);//延时10ms以保证看到效果
		}
		for(i = 0; i <= 100; i++)
		{
			TIM_SetCompare2(TIM2, 100 - i);
			Delay_ms(10);//延时10ms以保证看到效果
		}
	}
}

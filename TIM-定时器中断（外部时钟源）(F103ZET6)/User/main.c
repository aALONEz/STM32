#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Timer.h"
#include "Delay.h"

uint16_t Count = 0;

int main(void)
{
	//7.中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//分组2
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1, 1, "Count:");
	while(1)
	{
		OLED_ShowNum(1, 7, Count, 5);
		OLED_ShowNum(2, 7, TIM_GetCounter(TIM4), 5);
	}
}

//10.编写中断服务函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)//判断标志位
	{
		Count++;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//清除标志位
	}
}

#include "stm32f10x.h"                  // Device header
#include "OLED.h"//OLED头文件
#include "Timer.h"

uint16_t Count;

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级分组
	OLED_Init();
	Timer_Init();
	OLED_ShowString(1, 1, "Count:");
	while(1)
	{
		OLED_ShowNum(1, 7, Count, 5);
	}
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)//TIM_GetITStatus中断标志位检查函数
	{
		Count++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//TIM_ClearITPendingBit清除中断标志位函数
	}
	
}

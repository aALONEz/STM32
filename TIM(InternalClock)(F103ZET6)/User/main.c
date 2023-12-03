#include "stm32f10x.h" // Device header
#include "OLED.h"
#include "Timer.h"

uint16_t it_count = 0;//创建一个变量用来存储进入中断的次数

int main(void)
{
	//5.NVIC分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	timer_init();
	OLED_ShowString(1, 1, "ITCount:");
	while(1)
	{
		OLED_ShowNum(1, 9, it_count, 5);//调用OLED_ShowNum展示进入定时器中断的次数.
	}
}

void TIM2_IRQHandler(void)
{
	//检查中断标志位,不管是不是单独占用中断通道都要检查一下中断标志位
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)//检查 TIM2 更新中断 的标志位
	{
		it_count++;
		
		//清除中断标志位,一定不要忘记了
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//清除 TIM2 更新中断 的标志位
	}
}

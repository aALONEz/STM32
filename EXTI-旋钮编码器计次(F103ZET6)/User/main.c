#include "stm32f10x.h"                  // Device header
#include "OLED.h"//引入OLED头文件
#include "Encoder.h"//引入旋钮编码器头文件
#include "Delay.h"

int16_t Count;

void EXTI1_IRQHandler(void)//PA1
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)
	{
		Delay_ms(25);//消抖,基本上机械开关都要消抖
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)//消抖判断
		{
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 1)
			{
				Count--;
			}
		}
		
		
		EXTI_ClearITPendingBit(EXTI_Line1);
		
	}
}

void EXTI0_IRQHandler(void)//PA0
{
	if(EXTI_GetITStatus(EXTI_Line0) == SET)
	{
		Delay_ms(25);//消抖,基本上机械开关都要消抖
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == 0)//消抖判断
		{
			if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 1)
			{
				Count++;
			}
		}
		EXTI_ClearITPendingBit(EXTI_Line0);
		
	}
}


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	OLED_Init();
	OLED_ShowString(1, 1, "Count:");
	Encoder_Init();
	__enable_irq();
	while(1)
	{
		OLED_ShowSignedNum(1, 7, Count, 5);
	}
}



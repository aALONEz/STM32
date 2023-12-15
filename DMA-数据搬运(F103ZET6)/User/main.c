#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MyDMA.h"
uint8_t Data1[] = {1, 2, 3, 4};//源数组
uint8_t Data2[] = {0, 0, 0, 0};//目标数组

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "Data1:");
	OLED_ShowString(3, 1, "Data2:");
	MyDMA_Init((uint32_t)Data1, (uint32_t)Data2, 4);
	while(1)
	{
		OLED_ShowNum(2, 1, ++Data1[0], 3);
		OLED_ShowNum(2, 5, ++Data1[1], 3);
		OLED_ShowNum(2, 9, ++Data1[2], 3);
		OLED_ShowNum(2, 13, ++Data1[3], 3);
		Delay_ms(1000);
		Set_MyDMA_DataCounter(4);
		OLED_ShowNum(4, 1, Data2[0], 3);
		OLED_ShowNum(4, 5, Data2[1], 3);
		OLED_ShowNum(4, 9, Data2[2], 3);
		OLED_ShowNum(4, 13, Data2[3], 3);
		Delay_ms(1000);
	}
}

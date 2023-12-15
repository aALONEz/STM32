#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"
#include "Delay.h"


int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowString(1, 1, "X_Value:");
	OLED_ShowString(3, 1, "Y_Value:");
	while(1)
	{
		OLED_ShowNum(2, 1, ADC_Value[0], 5);
		OLED_ShowNum(4, 1, ADC_Value[1], 5);
		Delay_ms(100);
	}
}

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "SYSCLK:");
	OLED_ShowNum(1, 8, SystemCoreClock, 8);
	while(1)
	{
		OLED_ShowString(2, 1, "Runing");
		Delay_ms(500);
		OLED_ShowString(2, 1, "      ");
		Delay_ms(500);
		//如果是72MHz的主频就是一秒闪烁一次,36MHz的就是两秒闪烁一次,其他的频率以此类推
	}
}

#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "OLED.h"
#include "IC.h"

int main(void)
{
	OLED_Init();
	PWM_Init();
	IC_Init();
	OLED_ShowString(1, 1, "PSC:");
	OLED_ShowString(2, 1, "Pulse:");
	PWM_SetPrescaler(720 - 1, TIM_PSCReloadMode_Immediate);//PSC
	PWM_SetPulse(80);
	while(1)
	{
		OLED_ShowNum(1, 5, IC_GetFreq(), 6);
		OLED_ShowNum(2, 7, IC_GetDuty(), 5);
	}
}

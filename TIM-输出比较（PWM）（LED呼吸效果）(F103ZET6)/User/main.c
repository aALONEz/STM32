#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"


int main(void)
{
	PWM_Init();
	while(1)
	{
		for(int i = 0; i < 100; i++)//逐渐亮
		{
			Set_Pulse(i);
			Delay_ms(20);
		}
		for(int i = 0; i < 100; i++)//逐渐灭
		{
			Set_Pulse(100 - i);
			Delay_ms(20);
		}
	}
}

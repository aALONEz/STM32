#include "stm32f10x.h"                  // Device header
#include "Servo.h"
#include "Delay.h"
#include "OLED.h"

uint16_t Angle;

int main(void)
{
	Servo_Init();
	OLED_Init();
	OLED_ShowString(1, 1, "Angle:");
	while(1)
	{
		for(Angle = 0; Angle <= 180; Angle++)
		{
			Set_Servo_Angle(Angle);
			Delay_ms(50);
			OLED_ShowNum(1, 7, Angle, 5);
		}
		for(Angle = 0; Angle <= 180; Angle++)
		{
			Set_Servo_Angle(180 - Angle);
			Delay_ms(50);
			OLED_ShowNum(1, 7, (180 - Angle), 5);
		}
	}
}

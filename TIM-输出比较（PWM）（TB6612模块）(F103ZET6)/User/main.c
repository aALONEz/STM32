#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "KEY.h"
#include "OLED.h"

int8_t Speed = 0;

int main(void)
{
	OLED_Init();//OLED初始化
	KEY_Init();//按键初始化
	Motor_Init();//电机初始化
	OLED_ShowString(1, 1, "Speed:");
	
	Set_Motor_Speed(50);
	while(1)
	{
		if(Get_KEY_State() == 1)
		{
			Speed += 10;
			if(Speed > 100)
			{
				Speed = -100;
			}
		}
		OLED_ShowSignedNum(1, 7, Speed, 3);
		Set_Motor_Speed(Speed);
	}
}



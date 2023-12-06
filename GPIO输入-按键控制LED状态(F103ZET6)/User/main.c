#include "stm32f10x.h"                  // Device header
#include "KEY.h"//按键头文件
#include "LED.h"//LED头文件

int main(void)
{
	Key_Init();//按键初始化函数
	LED_Init();//LED初始化
	while(1)
	{
		if(Get_Key_State() == 1)
		{
			if(Get_LED_State() == 1)
			{
				LED_Switch(OFF);
			}
			else
			{
				LED_Switch(ON);
			}
		}
	}
}

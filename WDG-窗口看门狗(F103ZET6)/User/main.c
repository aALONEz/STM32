#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"

int main(void)
{
	OLED_Init();
	//窗口看门狗需要开启WWDG时钟,位于APB1总线上
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	//设置预分频器
	WWDG_SetPrescaler(WWDG_Prescaler_8);//8分频
	//设置窗口值
	WWDG_SetWindowValue(21 | 0X40);//30ms
	//设置计数器值,喂狗一次,每个看门狗都建议使能之前喂狗一次
	WWDG_SetCounter(54 | 0X40);
	//如果需要中断之类的请在使能前配置中断相关
	//使能窗口看门狗
	WWDG_Enable(54 | 0X40);
	while(1)
	{
		OLED_ShowString(1, 1, "Wait.....");
		Delay_ms(40);
		//要在30ms~50ms期间喂狗
		WWDG_SetCounter(54 | 0X40);
	}
}

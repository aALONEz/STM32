#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{
	OLED_Init();
	//独立看门狗不需要单独开启LSI时钟
	//向IWDG键寄存器写入0X5555解除写保护
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//配置分频器和重装载值
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2499);
	//喂狗一次
	IWDG_ReloadCounter();
	//使能独立看门狗,此时使能之后就不需要再关闭写使能,因为已经顺便帮我们修改了里面的值
	IWDG_Enable();
	while(1)
	{
		OLED_ShowString(1, 1, "Wait.....");
	}
}

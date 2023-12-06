#include "stm32f10x.h"                  // Device header
#include "ABZ.h"//有源蜂鸣器头文件
#include "LDR.h"//光敏电阻传感器头文件

int main(void)
{
	LDR_Init();
	ABZ_Init();
	
	while(1)
	{
		if(Get_LDR_State() == 1)
		{
			ABZ_Switch(ON);
		}
		else
		{
			ABZ_Switch(OFF);
		}
	}
}

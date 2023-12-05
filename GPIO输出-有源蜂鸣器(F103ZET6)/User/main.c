#include "stm32f10x.h"                  // Device header
#include "BEEP.h"

int main(void)
{
	//调用BEEP_Init初始化BEEP
	BEEP_Init();
	BEEP_Switch(ON);
	while(1)
	{
		
	}
}

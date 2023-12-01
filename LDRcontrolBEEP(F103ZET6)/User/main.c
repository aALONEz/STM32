#include "stm32f10x.h"                  // Device header
#include "BEEP.h"
#include "LDR.h"

int main(void)
{
	//调用beep初始化函数
	beep_init();
	//调用ldr初始化函数
	ldr_init();
	while(1)
	{
		if(get_ldr_digital() == Bit_SET)
		{
			set_beep(beep_on);
		}
		else
		{
			set_beep(beep_off);
		}
	}
}

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "BKP.h"

int main(void)
{
	OLED_Init();
	BKP_DeInit();//BKP寄存器的值恢复原来的值
	BKP_Init();
	
	//BKP_WriteData(BKP_DR1, 0X3568);
	OLED_ShowHexNum(1, 1, BKP_ReadData(BKP_DR1), 4);
	
	while(1)
	{
		
	}
}

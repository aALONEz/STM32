#include "stm32f10x.h"                  // Device header
#include "OLED.h"

int main(void)
{
	OLED_Init();
	
	OLED_ShowString(1, 1, "FlashSize:   KB");//11
	OLED_ShowString(2, 1, "UID:   ");//11
	
	//显示芯片Flash容量
	OLED_ShowNum(1, 11, *((__IO uint16_t*)(0X1FFFF7E0)), 3);
	//显示芯片UID
	OLED_ShowHexNum(2, 5, *((__IO uint16_t*)(0X1FFFF7E8)), 5);
	OLED_ShowHexNum(2, 9, *((__IO uint16_t*)(0X1FFFF7E8 + 0X02)), 5);
	OLED_ShowHexNum(3, 1, *((__IO uint32_t*)(0X1FFFF7E8 + 0X04)), 8);
	OLED_ShowHexNum(3, 9, *((__IO uint32_t*)(0X1FFFF7E8 + 0X08)), 8);
	while(1)
	{
		
	}
}

#include "stm32f10x.h"                  // Device header
#include "W25Q128.h"
#include "OLED.h"

uint8_t WriteArray[4] = {0X01, 0X02, 0X03, 0X99};
uint8_t ReadArray[4] = {0};
int main(void)
{
	OLED_Init();
	W25Q128_Init();
	//W25Q128_SectorErase(0X000000);
	//W25Q128_SectorErase(0X000100);
	//W25Q128_PageProgram(0X000000, WriteArray, 4);
	W25Q128_ReadData(0X000000, ReadArray, 4);
	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");
	
	OLED_ShowHexNum(1, 3, WriteArray[0], 2);
	OLED_ShowHexNum(1, 6, WriteArray[1], 2);
	OLED_ShowHexNum(1, 9, WriteArray[2], 2);
	OLED_ShowHexNum(1, 12, WriteArray[3], 2);
	
	OLED_ShowHexNum(2, 3, ReadArray[0], 2);
	OLED_ShowHexNum(2, 6, ReadArray[1], 2);
	OLED_ShowHexNum(2, 9, ReadArray[2], 2);
	OLED_ShowHexNum(2, 12, ReadArray[3], 2);
	while(1)
	{
		
	}
}

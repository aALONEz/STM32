#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Store.h"
#include "MyFlash.h"

int main(void)
{
	OLED_Init();
	Store_Init();
	
	OLED_ShowHexNum(1, 1, 0X000000007, 1);
	
	Store_Clear();
	
	Store_WriteByte(0x0807FFFF, 0X99);
	Store_WriteHalfWord(0x0807FFFE, 0X7D4D);
	Store_WriteWord(0x0807FFFF, 0XFFFFFFFF);
	Store_Save();
	
	OLED_ShowHexNum(1, 1, MyFlash_ReadByte(0x0807FFFF), 2);
	OLED_ShowHexNum(1, 9, Store_ReadByte(0x0807FFFF), 2);
	
	OLED_ShowHexNum(2, 1, MyFlash_ReadHalfWord(0x0807FFFE), 4);
	OLED_ShowHexNum(2, 9, Store_ReadHalfWord(0x0807FFFE), 4);
	
	OLED_ShowHexNum(3, 1, MyFlash_ReadWord(0x0807FFFC), 8);	
	OLED_ShowHexNum(3, 9, Store_ReadWord(0x0807FFFC), 8);
	
	
	

	
	
//	OLED_ShowHexNum(1, 1, 0XFFFFFFFF, 8);
//	OLED_ShowHexNum(2, 1, 0XFFFFFFFF, 8);
//	OLED_ShowHexNum(3, 1, 0XFFFFFFFF, 8);
//	OLED_ShowHexNum(4, 1, 0XFFFFFFFF, 8);
//	
//	OLED_ShowHexNum(1, 9, 0X00000000, 8);
//	OLED_ShowHexNum(2, 9, 0X00000000, 8);
//	OLED_ShowHexNum(3, 9, 0X00000000, 8);
//	OLED_ShowHexNum(4, 9, 0X00000000, 8);
	while(1)
	{
		
	}
}

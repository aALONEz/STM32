#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"

int main(void)
{
	OLED_Init();
	MyRTC_Init();
	
	OLED_ShowString(2, 1, "Data:XXXX-XX-XX");
	OLED_ShowString(3, 1, "Time:XX:XX:XX");
	OLED_ShowString(1, 1, "CNT:");
	
	while(1)
	{
		MyRTC_GetTime();
		OLED_ShowNum(1, 5, RTC_GetCounter(), 10);//时间戳
		OLED_ShowNum(2, 6, TimeArray[0], 4);//年
		OLED_ShowNum(2, 11, TimeArray[1], 2);//月
		OLED_ShowNum(2, 14, TimeArray[2], 2);//日
		
		OLED_ShowNum(3, 6, TimeArray[3], 2);//时
		OLED_ShowNum(3, 9, TimeArray[4], 2);//分
		OLED_ShowNum(3, 12, TimeArray[5], 2);//秒
	}
}

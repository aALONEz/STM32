#include "stm32f10x.h"                  // Device header
#include "MyRTC.h"
#include "OLED.h"
#include "Delay.h"

int main(void)
{
	OLED_Init();
	MyRTC_Init();
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	//设置闹钟值,让闹钟唤醒待机模式
//	uint32_t AlarmNum = RTC_GetCounter() + 10;
//	RTC_SetAlarm(AlarmNum);//让闹钟的值是当前代码执行的时间加10秒
	
	//开启WakeUP引脚使能
	PWR_WakeUpPinCmd(ENABLE);//调用此函数不需要将对应GPIO初始化配置
	
	OLED_ShowString(1, 1, "Data:XXXX-XX-XX");
	OLED_ShowString(2, 1, "Time:XX:XX:XX");
	
	while(1)
	{
		MyRTC_GetTime();
		OLED_ShowNum(1, 6, TimeArray[0], 4);//年
		OLED_ShowNum(1, 11, TimeArray[1], 2);//月
		OLED_ShowNum(1, 14, TimeArray[2], 2);//日
		
		OLED_ShowNum(2, 6, TimeArray[3], 2);//时
		OLED_ShowNum(2, 9, TimeArray[4], 2);//分
		OLED_ShowNum(2, 12, TimeArray[5], 2);//秒
		
		OLED_ShowString(4, 1, "Runing");
		Delay_ms(100);
		OLED_ShowString(4, 1, "      ");
		Delay_ms(100);
		
		OLED_Clear();
		
		//调用开启待机模式前要先清除唤醒标志（WUF）位,如果不停止这个标志位,他就会一直进入待机模式,然后瞬间离开待机模式
		PWR_ClearFlag(PWR_FLAG_WU);
		//开启待机模式
		PWR_EnterSTANDBYMode();
	}
}

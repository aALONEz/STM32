#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "AD.h"
#include "Delay.h"

float Voltage;

int main(void)
{
	OLED_Init();
	AD_Init();
	OLED_ShowString(1, 1, "Value:");
	OLED_ShowString(2, 1, "Voltage:");
	while(1)
	{
		OLED_ShowNum(1, 7, Get_ADValue(), 5);
		Voltage = (float)Get_ADValue() / 4095 * 3.3;//转换成3.3V电压
		OLED_ShowNum(2, 9, Voltage, 1);//显示Voltage整数部分
		OLED_ShowString(2, 10, ".");//显示Voltage小数点
		OLED_ShowNum(2, 11, (uint16_t)(Voltage * 100) % 100, 2);//显示Voltage小数部分
		Delay_ms(100);
	}
}

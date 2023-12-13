#include "stm32f10x.h"                  // Device header
#include "OLED.h"//OLED头文件
#include "Encoder.h"//编码器头文件
#include "Delay.h"//延时头文件

int main(void)
{
	OLED_Init();
	OLED_ShowString(1, 1, "Encoder:");
	Encoder_Init();
	while(1)
	{
		OLED_ShowSignedNum(1, 9, Get_Encoder(), 5);
		Delay_ms(1000);//延时1秒用做测频法的闸门时间,如果是点击的编码器,可以将演示降低到10ms,甚至1ms,你也可以使用另一个定时器来定时这个闸门时间
		//比如说使用定时器定时1秒或者1毫秒,定时中断一下然后在中断内将一个全局变量的值赋值为Get_Encoder()的值
	} 
}

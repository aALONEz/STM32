#include "stm32f10x.h"                  // Device header
#include "OLED.h"//引入OLED头文件
#include "IR.h"//引入对射式红外传感器 外部中断 头文件

uint16_t Count;//全局变量默认初始化为0

int main(void)
{
	//5.5设置中断优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断优先级分组2
	OLED_Init();//OLED初始化,使用的模拟I2C
	IR_Init();//对射式红外传感器 外部中断 初始化
	OLED_ShowString(1, 1, "Count:");///在1行1列显示字符串
	while(1)
	{
		OLED_ShowNum(1, 7, Count, 5);///在1行7列显示Count,显示最多5位
	}
}

/*
* 通道一的中断函数
*/
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1) == SET)//判断中断线路一的标志位
	{
		Count++;
		//清除中断标志位
		EXTI_ClearITPendingBit(EXTI_Line1);//最后执行清除中断标志位
	}
}

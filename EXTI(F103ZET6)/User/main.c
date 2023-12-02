#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "CountSensor.h"

//为了方便显示实验现象,引入了OLED模块,但是目前先不讲解OLED模块的视频
//OLED采用4线模拟I2C,只需要将OLED.c里面标注要修改的地方修改了,此OLED驱动代码来源B站UP"江协科技"

uint32_t countsensor_count = 0;//中断触发变量

int main(void)
{
	OLED_Init();
	//NVIC分组设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//一定要在分组后调用这个函数,对射式红外传感器模块中断方式初始化函数
	countsensor_init();
	
	OLED_ShowString(1, 1, "CountNum:");//在第一行第一列开始显示CountNum:
	while(1)
	{
		OLED_ShowNum(1, 10, countsensor_count, 6);//在第一行第十列开始显示countsensor_count,占用6格空间
	}
}

/*
* 中断函数不需要声明它是自动执行的
*/
void EXTI1_IRQHandler(void)
{
	//判断是不是我们想要的中断通道
	if(EXTI_GetITStatus(countsensor_exti_line) == SET)
	{
		countsensor_count++;
		//中断程序结束前一定要调用EXTI_ClearITPendingBit清除中断标志位,且最好在判断是这个中断之后再清除
		EXTI_ClearITPendingBit(countsensor_exti_line);
	}
	
}

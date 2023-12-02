#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "RotaryEncoder.h"

//为了方便显示实验现象,引入了OLED模块,但是目前先不讲解OLED模块的视频
//OLED采用4线模拟I2C,只需要将OLED.c里面标注要修改的地方修改了,此OLED驱动代码来源B站UP"江协科技"

int16_t countsensor_count = 0;//中断触发变量

int main(void)
{
	OLED_Init();
	//NVIC分组设置
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//调用RotaryEncoder初始化函数
	rotary_encoder_init();
	
	OLED_ShowString(1, 1, "CountNum:");//在第一行第一列开始显示CountNum:
	while(1)
	{
		OLED_ShowNum(1, 10, countsensor_count, 6);//在第一行第十列开始显示countsensor_count,占用6格空间
	}
}

/*
* 中断函数不需要声明它是自动执行的
* 目前是外部中断5~9的中断函数,对应的是GPIO_Pin_5~9
*/
void EXTI9_5_IRQHandler(void)
{
	//判断连接模块A端口的引脚
	if(EXTI_GetITStatus(rotary_encoder_exti_line_a) == SET)//判断中断标志位
	{
		//判断另一个引脚,也就是连接模块B端口的引脚电平状态,这里判断是因为旋转编码器有数值变化的时候是输出两个方波,每个方波相差90度
		if(GPIO_ReadInputDataBit(rotary_encoder_gpio, rotary_encoder_gpio_pin_b) == 0)
		{
			countsensor_count--;//反转
		}
		
		//清除中断标志位
		EXTI_ClearITPendingBit(rotary_encoder_exti_line_a);
	}
	//判断连接模块B端口的引脚
	if(EXTI_GetITStatus(rotary_encoder_exti_line_b) == SET)//判断中断标志位
	{
		//判断另一个引脚,也就是连接模块A端口的引脚电平状态
		if(GPIO_ReadInputDataBit(rotary_encoder_gpio, rotary_encoder_gpio_pin_a) == 0)
		{
			countsensor_count++;//正转
		}
		
		//清除中断标志位
		EXTI_ClearITPendingBit(rotary_encoder_exti_line_b);
	}
}

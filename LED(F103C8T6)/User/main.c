#include "stm32f10x.h"                  // Device header
#include "Delay.h"//引入Delay延时头文件

int main(void)
{
	//1.使能LED引脚对应端口的对应时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//2.初始化全部GPIO引脚
		//2.1创建GPIO_InitTypeDef结构体变量,并赋值后将其传递给GPIO_Init函数
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	while(1)
	{
		//3.让LED闪烁起来
		//想要让LED闪烁起来就需要让LED亮起一段时间然后再熄灭一段时间而且这是一个循环
		//你使用其他控制GPIO引脚电平的函数也是可以的
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);//LED低电平点亮
		Delay_ms(500);//延时500ms
		GPIO_SetBits(GPIOA, GPIO_Pin_0);//LED高电平熄灭
		Delay_ms(500);//延时500ms
	}
}

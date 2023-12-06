#include "stm32f10x.h"                  // Device header
#include "Delay.h"


void Key_Init(void)
{
	//按键 PE4
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能GPIOE
	//初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	//调用GPIO_StructInit函数先将结构体赋值一个默认值然后再修改.
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;//按键引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//GPIO模式,上拉输入模式
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

uint8_t Get_Key_State(void)
{
	uint8_t Key_State = 0;//0未按下,1按下
	//按下返回低电平(Bit_RESET) 未按下返回高电平(Bit_SET)
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
	{
		Delay_ms(20);//按下消抖
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)//消抖之后再次判断是不是按下
		{
			while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET);//松手停止循环
			Delay_ms(20);//松手消抖
			Key_State = 1;
		}
	}
	return Key_State;
}

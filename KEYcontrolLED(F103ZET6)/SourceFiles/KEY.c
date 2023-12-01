#include "stm32f10x.h"                  // Device header
#include "KEY.h"
#include "Delay.h"

void KEY_init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
	//使能KEY对饮引脚GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);
	
	//初始化KEY对应GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_x;//KEY对应引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//此项在输入模式无用
	GPIO_Init(GPIOx, &GPIO_InitStruct);
}

uint8_t get_KEY(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x)
{
	uint8_t key_num = 0;//没有按键按下默认为0,1为按下。
	
	if(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_x) == 0)//按键为一端接GND一端接STM32GPIO引脚所以低电平(0)算是按键按下
	{
		Delay_ms(20);//延时10毫秒消抖,后面会定时器后将使用定时器消抖
		while(GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_x) == 0);//只有松手才会停止循环,判断按键松开
		Delay_ms(10);
		key_num = 1;
	}
	return key_num;
}

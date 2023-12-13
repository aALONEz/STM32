#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/*
* 函数名:KEY_Init
* 功能:按键初始化
* 参数:无
* 返回值:无
* 备注:按键连接PE4
*/
void KEY_Init(void)
{
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//上拉输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/*
* 函数名:Get_KEY_State
* 功能:获取按键状态
* 参数:无
* 返回值:1表示按键按下,0表示按键未按下
* 备注:按键连接PE4接口
*/
uint8_t Get_KEY_State(void)
{
	uint8_t KEY_State = 0;
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
	{
		Delay_ms(20);//消抖
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
		{
			while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET);//松开按键停止循环
			KEY_State = 1;
		}
	}
	return KEY_State;
}

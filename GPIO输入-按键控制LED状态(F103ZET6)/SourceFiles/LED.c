#include "stm32f10x.h"                  // Device header

/*
* 函数名:LED_Init
* 功能:LED初始化
* 参数:无
* 返回值:无
* 备注:LED0 PB5		LED1 PE5	
*/
void LED_Init(void)
{
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);//GPIOB GPIOE
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	//PB5
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;//LED引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO模式,推完输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平变化速度,50MHz
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//PE5
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;//LED引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//GPIO模式,推完输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平变化速度,50MHz
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	//3.控制LED引脚电平状态为高电平.(低电平点亮)
	GPIO_SetBits(GPIOB, GPIO_Pin_5);//高电平
	GPIO_SetBits(GPIOE, GPIO_Pin_5);//高电平
}

/*
* 函数名:Get_LED0_State
* 功能:LED0状态
* 参数:无
* 返回值:uint8_t 1表示点亮状态 0表示熄灭状态
* 备注:LED0 PB5	
*/
uint8_t Get_LED0_State(void)
{
	uint8_t LED0_State = 0;
	if(GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_5) == Bit_SET)//高电平就是没有点亮
	{
		LED0_State = 0;//0表示没有点亮
	}
	else
	{
		LED0_State = 1;//1表示点亮
	}
	return LED0_State;
}

/*
* 函数名:Get_LED1_State
* 功能:LED1状态
* 参数:无
* 返回值:uint8_t 1表示点亮状态 0表示熄灭状态
* 备注:LED1 PE5	
*/
uint8_t Get_LED1_State(void)
{
	uint8_t LED1_State = 0;
	if(GPIO_ReadOutputDataBit(GPIOE, GPIO_Pin_5) == Bit_SET)//高电平就是没有点亮
	{
		LED1_State = 0;//0表示没有点亮
	}
	else
	{
		LED1_State = 1;//1表示点亮
	}
	return LED1_State;
}


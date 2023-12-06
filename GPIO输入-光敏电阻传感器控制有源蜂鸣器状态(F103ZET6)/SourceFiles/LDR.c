#include "stm32f10x.h"                  // Device header

/*
* 函数名:LDR_Init
* 功能:初始化光敏电阻传感器数字输出
* 参数:无
* 返回值:无
* 备注:光敏电阻传感器数字输出引脚连接PA0
*/
void LDR_Init(void)
{
	//光敏电阻传感器数字输出引脚连接 PA0
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);//结构体复制初始值
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入,可选下拉输入.高电平表示被遮挡
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//3.获取光敏电阻传感器引脚电平状态
/*
* 函数名:Get_LDR_State
* 功能:获取光敏电阻传感器数字输出引脚电平状态
* 参数:无
* 返回值:1表示被遮挡 0表示未被遮挡
* 备注:光敏电阻传感器数字输出引脚连接PA0
*/
uint8_t Get_LDR_State(void)
{
	uint8_t LDR_State = 0;//0未被遮挡
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_RESET)//未被遮挡
	{
		LDR_State = 0;//0表示未被遮挡
	}
	else
	{
		LDR_State = 1;//1表示被遮挡
	}
	return LDR_State;
}

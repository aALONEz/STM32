#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define KEYEXTI_GPIO 		GPIO_PortSourceGPIOE
#define KEYEXTI_Pin 		GPIO_PinSource3
#define KEYEXTI_Line 		EXTI_Line3
#define KEYEXTI_IRQChannel 	EXTI3_IRQn

/*
* 函数名：Key_Init
* 功能：按键初始化
* 参数：无
* 返回值：无
* 备注：KEY0 PE4	KEY1 PE3
*/
void Key_Init(void)
{
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);//使能GPIOE
	//初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	//调用GPIO_StructInit函数先将结构体赋值一个默认值然后再修改.
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_3;//按键引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;//GPIO模式,上拉输入模式
	
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/*
* 函数名：KeyEXTI_Init
* 功能：按键1中断初始化,需要先调用按键初始化
* 参数：无
* 返回值：无
* 备注：KEY1 PE3
*/
void Key1EXTI_Init(void)
{
	//使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//连接EXTI线路
	GPIO_EXTILineConfig(KEYEXTI_GPIO, KEYEXTI_Pin);
	//EXTI初始化
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = KEYEXTI_Line;//通道号
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断响应
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStruct);
	//NVIC初始化
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = KEYEXTI_IRQChannel;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStruct);
}

/*
* 函数名：Get_Key0_State
* 功能：获取按键0的状态
* 参数：无
* 返回值：uint8_t 1表示按下 0表示未按下
* 备注：KEY0 PE4
*/
uint8_t Get_Key0_State(void)
{
	uint8_t Key_State = 0;//0未按下,1按下
	//按下返回低电平(Bit_RESET) 未按下返回高电平(Bit_SET)
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)
	{
		Delay_ms(10);//按下消抖
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET)//消抖之后再次判断是不是按下
		{
			while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_RESET);//松手停止循环
			Delay_ms(10);//松手消抖
			Key_State = 1;
		}
	}
	return Key_State;
}

/*
* 函数名：Get_Key1_State
* 功能：获取按键1的状态
* 参数：无
* 返回值：uint8_t 1表示按下 0表示未按下
* 备注：KEY1 PE3
*/
uint8_t Get_Key1_State(void)
{
	uint8_t Key_State = 0;//0未按下,1按下
	//按下返回低电平(Bit_RESET) 未按下返回高电平(Bit_SET)
	if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)
	{
		Delay_ms(10);//按下消抖
		if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET)//消抖之后再次判断是不是按下
		{
			while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_RESET);//松手停止循环
			Delay_ms(10);//松手消抖
			Key_State = 1;
		}
	}
	return Key_State;
}

void EXTI3_IRQHandler(void)//KEY中断服务函数
{
	if(EXTI_GetITStatus(KEYEXTI_Line) == SET)//判断是不是EXTI3中断
	{
		
		EXTI_ClearITPendingBit(KEYEXTI_Line);//清除中断标志位
	}
}

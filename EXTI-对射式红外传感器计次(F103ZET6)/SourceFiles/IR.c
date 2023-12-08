#include "stm32f10x.h"                  // Device header

/*
* 函数名:IR_Init
* 功能: 对射式红外传感器,外部中断初始化
* 参数: 无
* 返回值: 无
* 备注: 数字输出引脚连接PA1,被遮挡时高电平,不被遮挡时低电平
*/
void IR_Init(void)
{
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);//结构体赋值默认值
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;//引脚编号
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//具体GPIO模式请参考芯片参考手册的外设GPIO配置
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_PinLockConfig(GPIOA, GPIO_Pin_1);//锁定GPIO引脚配置函数,防止后面意外更改
	//3.使能AFIO时钟,要通过AFIO将1号引脚的中断接在EXTI上
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//使能AFIO时钟
	//4.调用GPIO_EXTILineConfig函数设置外部中断线路,引脚1外部中断导通.
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//PA1开启外部中断
	//5.初始化EXTI,EXTI不需要开启时钟EXTI和NVIC都是默认打开时钟的
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_StructInit(&EXTI_InitStruct);//结构体赋值默认值
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;//线路1,PA1就是线路1,PB1也是线路1,这就是为什么不可以同一引脚编号同时中断。PB2就是线路2
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断响应
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发,遮挡的时候触发		下降沿触发,拿走遮挡物的时候触发
	
	EXTI_Init(&EXTI_InitStruct);
	//6.初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;//通道号与上面的EXTI_Line线路对应
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;//使能或者失能
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级 1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;//响应优先级 2
	
	NVIC_Init(&NVIC_InitStruct);
}

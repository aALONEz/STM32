#include "stm32f10x.h"                  // Device header

/*
* 函数名:ADC_DMA_Init
* 功能:ADC使用的DMA初始化 硬件触发 半字大小
* 参数:PeripheralBaseAddr 外设寄存器地址,ADC数据寄存器地址    MemoryBaseAddr 存储器地址
* 返回值:无
* 备注:无
*/
void ADC_DMA_Init(uint32_t PeripheralBaseAddr, uint32_t MemoryBaseAddr, uint16_t Number)
{
	//1.使能DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA1使能
	//2.初始化DMA
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_BufferSize = Number;//传输次数,传输计数器的值
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向 外设寄存器作为源头
	DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;//关闭软件触发
	DMA_InitStruct.DMA_MemoryBaseAddr = MemoryBaseAddr;//存储器地址
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据大小 半字 uint16_t
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器是否自增,是
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;//传输模式 循环模式,也就是自动重装模式
	DMA_InitStruct.DMA_PeripheralBaseAddr = PeripheralBaseAddr;//外设寄存器地址,也可以是存储器地址
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设寄存器数据大小 半字 uint16_t
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设寄存器地址是否自增 否
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;//优先级 中等
	
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);//这里必须要是通道1,因为ADC1的硬件触发DMA在通道1上
	//3.使能DMA
	DMA_Cmd(DMA1_Channel1, ENABLE);//DMA1通道1使能
}

/*
* 函数名:Set_ADC_DMA_DataCounter
* 功能:ADC使用的DMA传输计数器值设置
* 参数:Number 要设置传输计数器的值
* 	   void ADC_SoftwareStartConvCmd(ADC_TypeDef*, FunctionalState) ADC库函数ADC_SoftwareStartConvCmd的函数指针
* 返回值:无
* 备注:此函数只有在DMA正常模式以及ADC连续转换模式关闭的情况下才可以使用
*/
void Set_ADC_DMA_DataCounter(void ADC_SoftwareStartConvCmd(ADC_TypeDef*, FunctionalState), uint16_t Number)
{
	//设置传输计数器的值之前要先关闭DMA
	DMA_Cmd(DMA1_Channel1, DISABLE);//关闭DMA1通道1,通道对应触发信号来自那个通道
	DMA_SetCurrDataCounter(DMA1_Channel1, Number);//设置传输计数器的值
	DMA_Cmd(DMA1_Channel1, ENABLE);//使能DMA1通道1,通道对应触发信号来自那个通道
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//开启ADC1转换
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//检查传输完成标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);//清除传输完成标志位
}

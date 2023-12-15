#include "stm32f10x.h"                  // Device header

/*
* 函数名:MyDMA_Init
* 功能:初始化DMA,源与目的地址全部都是自增模式,软件触发,DMA方向外设寄存器到存储器,以字节(8位)大小传输
* 参数: SourceArrayAddress 源地址 DestinationArrayAddress 目的地地址 Number 传输的次数
* 返回值:无
* 备注:只用于手动在存储器或者外设到存储器的手动MDA调用
*/
void MyDMA_Init(uint32_t SourceArrayAddress, uint32_t DestinationArrayAddress, uint16_t Number)
{
	//1.使能DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//DMA是AHB总线外设
	
	//2.初始化DMA
	DMA_InitTypeDef DMA_InitStruct;
	
	DMA_InitStruct.DMA_MemoryBaseAddr = DestinationArrayAddress;//存储器基地址,需要一个32位的地址变量
	DMA_InitStruct.DMA_PeripheralBaseAddr = SourceArrayAddress;//外设基地址,可以填写存储器基地址和外设基地址两种,需要一个32位的地址变量
	DMA_InitStruct.DMA_BufferSize = Number;//数据传输次数
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据大小,Byte
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据大小,Byte
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Enable;// 外设地址增量模式,就是地址是否自增,开启
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;// 存储器地址增量模式,就是地址是否自增,开启
	DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;//数据传输模式,就是是否自动重装,正常模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;// 传输优先级,中等优先级
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;// 数据传输方向,外设站点作为源头的传输方向
	DMA_InitStruct.DMA_M2M = DMA_M2M_Enable;// 存储器到存储器传输,就是是否使用软件触发,关闭
	
	DMA_Init(DMA1_Channel1, &DMA_InitStruct);
	
	//3.使能DMA
	DMA_Cmd(DMA1_Channel1, DISABLE);
}

/*
* 函数名:MyDMA_Transfer
* 功能:给传输计数器赋值
* 参数: Number 传输的次数
* 返回值:无
* 备注:无
*/
void Set_MyDMA_DataCounter(uint16_t Number)
{
	//给传输计数器赋值的时候必须要将DMA先关闭再赋值
	DMA_Cmd(DMA1_Channel1, DISABLE);//失能
	DMA_SetCurrDataCounter(DMA1_Channel1, Number);//传输次数
	DMA_Cmd(DMA1_Channel1, ENABLE);//使能
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);//转运完成判断
	DMA_ClearFlag(DMA1_FLAG_TC1);
}

#include "stm32f10x.h"                  // Device header
#include "ADC_DMA.h"

#define NbrOfChannel 2

uint16_t ADC_Value[2];


/*
* 函数名:AD_Init
* 功能:AD模拟到数字转换初始化,ADC1初始化
* 参数:无
* 返回值:无
* 备注:待测模拟量请链接PA0
*/
void AD_Init(void)
{
	//1.使能GPIO时钟和ADC1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);//使能GPIOA和ADC1
	
	//2.配置ADC时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//6分频,12MHz.ADC最大时钟频率为14MHz
	
	//3.配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式无效,但是也要赋值
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//4.配置ADC规则通道转换顺序
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);//ADC1,通道0配置到ADC顺序1的位置(顺序1-16),对于模拟信号采样1.5个周期
	//要设置多个通道只需要再次调用ADC_RegularChannelConfig函数配置即可
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_1Cycles5);//ADC1,通道5配置到ADC顺序2的位置(顺序1-16),对于模拟信号采样1.5个周期
	//5.初始化配置ADC
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;//是否使能连续转换模式 是
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐模式 右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发源选择 无
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//ADC工作模式 独立模式
	ADC_InitStruct.ADC_NbrOfChannel = NbrOfChannel;//需要转换的通道数量 2
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;//是否使能扫描模式 是
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//6.DMA初始化
	ADC_DMA_Init((uint32_t)&ADC1->DR, (uint32_t)ADC_Value, NbrOfChannel);//第一个参数 源头地址,第二个参数 目的地地址,第三个参数 单次循环搬运次数
	//7.如果需要模拟看门狗或者开启中断(两个可同时开启),请在该步骤编写.
	
	//8.开启ADC的DMA触发信号
	ADC_DMACmd(ADC1, ENABLE);
	
	//9.使能ADC.
	ADC_Cmd(ADC1, ENABLE);
	
	//10.ADC校准
	ADC_ResetCalibration(ADC1);//复位校准,每次校准前都需要先复位
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//如果复位校准一直没有完成就会一直在此循环
	ADC_StartCalibration(ADC1);//开始校准ADC
	while(ADC_GetCalibrationStatus(ADC1) == SET);//如果校准一直未完成就是一直在此循环.
	
	//11.ADC转换开启
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

/*
* 函数名:Get_ADValue
* 功能:获取ADC_Value数组的结果,也就是ADC转换后的数值
* 参数:无
* 返回值:无
* 备注:此函数只有在DMA正常模式以及ADC连续转换模式开启的情况下才可以使用
*/
void Get_ADValue(void)
{
	Set_ADC_DMA_DataCounter(&ADC_SoftwareStartConvCmd, NbrOfChannel);
}

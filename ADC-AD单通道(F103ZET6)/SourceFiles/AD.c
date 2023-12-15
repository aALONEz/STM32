#include "stm32f10x.h"                  // Device header

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
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式无效,但是也要赋值
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//4.配置ADC规则通道转换顺序
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);//ADC1,通道0配置到ADC顺序1的位置(顺序1-16),对于模拟信号采样1.5个周期
	//要设置多个通道只需要再次调用ADC_RegularChannelConfig函数配置即可
	
	//5.初始化配置ADC
	ADC_InitTypeDef ADC_InitStruct;
	
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//是否使能连续转换模式 否
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐模式 右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发源选择 无
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//ADC工作模式 独立模式
	ADC_InitStruct.ADC_NbrOfChannel = 1;//需要转换的通道数量 1
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//是否使能扫描模式 否
	
	ADC_Init(ADC1, &ADC_InitStruct);
	
	//6.如果需要模拟看门狗或者开启中断(两个可同时开启),请在该步骤编写.
	
	//7.使能ADC.
	ADC_Cmd(ADC1, ENABLE);
	
	//8.ADC校准
	ADC_ResetCalibration(ADC1);//复位校准,每次校准前都需要先复位
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);//如果复位校准一直没有完成就会一直在此循环
	ADC_StartCalibration(ADC1);//开始校准ADC
	while(ADC_GetCalibrationStatus(ADC1) == SET);//如果校准一直未完成就是一直在此循环.
}

/*
* 函数名:Get_ADValue
* 功能:获取AD转换后的数值
* 参数:无
* 返回值:ADValue 转换完成后拿去ADC_DR寄存器的值,上面配置的是数据右对齐所以可以直接读取不需要转换
* 备注:数据右对齐
*/
uint16_t Get_ADValue(void)
{
	uint16_t ADValue;
	//调用软件触发ADC转换
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	//判断EOC标志位
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//如果转换未完成就一直在这里循环
	
	//转换完成拿到寄存器的值
	ADValue = ADC_GetConversionValue(ADC1);//拿到ADC_DR寄存器的数值之后,硬件会自动将ADC_FLAG_EOC标志位自动置零,所以不需要我们手动置零
	return ADValue;
}

#include "stm32f10x.h"                  // Device header

/*
* 函数名:IC_Init
* 功能:输入捕获初始化
* 参数:无
* 返回值:无
* 备注:测量PA6(TIM3_CH1)引脚的方波频率
*/
void IC_Init(void)
{
	//1.使能TIM3时钟和GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//TIM3时钟
	//2.初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式无效,但是此成员要赋值一个默认值,不可以不赋值初始化GPIO,会产生BUG
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//3.设置TIM3为内部时钟源
	TIM_InternalClockConfig(TIM3);//TIM3内部时钟源
	//4.设置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;//时钟因子 一分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;//ARR,自动重装载计数器的值设置为最大
	TIM_TimeBaseInitStruct.TIM_Prescaler = 72 - 1;//PSC,//72M / (PSC + 1)就是标准频率的值
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;//高级定时器的重复计数模式
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	//5.配置输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStruct;
	
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//待测方波频率输入引脚,也就是上面初始化的引脚,是定时器那个通道的,举例:PA6就是TIM3_CH1,CH1就是通道一
	TIM_ICInitStruct.TIM_ICFilter = 0;//滤波阈值,0-15数字越大滤波效果越强
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿触发
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;//捕获分频
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;//直接输入
	
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	
	TIM_PWMIConfig(TIM3, &TIM_ICInitStruct);//将上面结构体直接配置成PMWI模式,就是与现有结构体的值取反,PWMI模式就是一个上升沿触发一个下降沿触发,一个直连输入一个交叉输入
	//6.配置TRGI的触发源
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	//7.配置从模式为CNT复位模式
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	//8.使能定时器
	TIM_Cmd(TIM3, ENABLE);//使能定时器3
}

/*
* 函数名:IC_GetFreq
* 功能:获取输入捕获的频率
* 参数:无
* 返回值:Freq 被测方波的频率
* 备注:Freq = 时钟源 / (PSC + 1) / (CCR + 1)//CCR+1是为了剔除正负一误差
*/
uint32_t IC_GetFreq(void)
{
	uint32_t Freq;
	Freq = (72000000 / (TIM_GetPrescaler(TIM3) + 1)) / (TIM_GetCapture1(TIM3) + 1);//CCR+1是为了剔除正负一误差
	return Freq;
}

/*
* 函数名:IC_GetDuty
* 功能:获取输入捕获的占空比率
* 参数:无
* 返回值:Duty 被测方波的占空比
* 备注:Duty = (CCR_2 + 1) * 100 / (CCR_1 + 1)//CCR_1+1 CCR_2+1是为了剔除正负一误差
*/
uint32_t IC_GetDuty(void)
{
	uint32_t Duty;
	Duty = (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);//CCR_1+1 CCR_2+1是为了剔除正负一误差,*100是因为CCR_2 / CCR_1肯定是小于1的数,也就是个百分比数,这个时候乘100转换为对应整数
	return Duty;
}

#include "stm32f10x.h"                  // Device header

/*
* 函数名:Encoder_Init
* 功能:编码器接口初始化
* 参数:无
* 返回值:无
* 备注:编码器接口PA6,PA7
*/
void Encoder_Init(void)
{
	//1.使能GPIOA时钟和TIM3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//TIM3时钟
	//2.初始化配置GPIO
	GPIO_InitTypeDef GPIO_InitStrcut;
	
	GPIO_InitStrcut.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStrcut.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStrcut.GPIO_Speed = GPIO_Speed_50MHz;//输入模式无效,但是该成员需要赋值,不可以是随机值
	
	GPIO_Init(GPIOA, &GPIO_InitStrcut);//PA6,PA7
	//3.配置时基单元
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);//结构体赋值初始值
	
	TIM_TimeBaseInitStruct.TIM_Period = 65536 - 1;//ARR,自动重装载寄存器值//一般设置为最大
	TIM_TimeBaseInitStruct.TIM_Prescaler = 1 - 1;//PSC,预分配器的值,0就是不分频.也就是一分频
	//这里不需要配置其他成员,因为在编码器模式下,其他成员无效只要有个初始值就可以了
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	//4.配置输入捕获单元
	TIM_ICInitTypeDef TIM_ICInitStruct;
	TIM_ICStructInit(&TIM_ICInitStruct);//结构体赋值初始值
	//PA6
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;//通道
	TIM_ICInitStruct.TIM_ICFilter = 0;
	//这里不需要配置其他成员,因为在编码器模式下,其他成员无效只要有个初始值就可以了
	
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	//PA7
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;//通道
	TIM_ICInitStruct.TIM_ICFilter = 0;
	
	TIM_ICInit(TIM3, &TIM_ICInitStruct);
	//5.配置编码器接口
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//上升沿,这里上升沿并不是代表上升沿有效,编码器模式上升沿下降沿都有效,这里上升沿表示引脚电平不反相,反之下降沿就是反相
	//6.使能定时器
	TIM_Cmd(TIM3, ENABLE);
}

/*
* 函数名:Get_Encoder
* 功能:返回编码器状态,速度和方向
* 参数:无
* 返回值:Encoder 正数代表正转,负数代表反转,绝对值表示速度
* 备注:编码器接口PA6,PA7
*/
int16_t Get_Encoder(void)
{
	int16_t Encoder;
	Encoder = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);//计数器归零
	
	return Encoder;
}

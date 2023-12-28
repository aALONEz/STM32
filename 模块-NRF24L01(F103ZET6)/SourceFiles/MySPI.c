#include "stm32f10x.h"                  // Device header
#include "Delay.h"

//CS引脚为PB12，SCK引脚为PB13，MISO引脚为PB14，MOSI引脚为PB15
//CS
#define CS_RCC 			RCC_APB2Periph_GPIOB
#define CS_GPIO 		GPIOG
#define CS_Pin 			GPIO_Pin_7
//SCK
#define SCK_RCC 		RCC_APB2Periph_GPIOB
#define SCK_GPIO 		GPIOB
#define SCK_Pin 		GPIO_Pin_13
//MISO
#define MISO_RCC 		RCC_APB2Periph_GPIOB
#define MISO_GPIO 		GPIOB
#define MISO_Pin 		GPIO_Pin_14
//MOSI
#define MOSI_RCC 		RCC_APB2Periph_GPIOB
#define MOSI_GPIO 		GPIOB
#define MOSI_Pin 		GPIO_Pin_15

/*
* 函数名:MySPI_W_CS
* 功能:指定CS引脚电平状态控制函数
* 参数:BitValue 1高电平 0低电平
* 返回值:无
* 备注:无
*/
static void MySPI_W_CS(uint8_t BitValue)//如果SPI总线有多个设备请多次定义CS片选引脚函数
{
	GPIO_WriteBit(CS_GPIO, CS_Pin, (BitAction)BitValue);
	//Delay_us(10);
}

/*
* 函数名:MySPI_Init
* 功能:硬件SPI初始化
* 参数:无
* 返回值:无
* 备注:无
*/
void MySPI_Init(void)
{
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(CS_RCC | SCK_RCC | MISO_RCC | MOSI_RCC, ENABLE);
	//2.初始化配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	//CS
	GPIO_InitStruct.GPIO_Pin = CS_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平反转速度
	GPIO_Init(CS_GPIO, &GPIO_InitStruct);
	
	//SCK
	GPIO_InitStruct.GPIO_Pin = SCK_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平反转速度
	GPIO_Init(SCK_GPIO, &GPIO_InitStruct);
	
	//MISO
	GPIO_InitStruct.GPIO_Pin = MISO_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平反转速度
	GPIO_Init(MISO_GPIO, &GPIO_InitStruct);
	
	//MISO
	GPIO_InitStruct.GPIO_Pin = MOSI_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平反转速度
	GPIO_Init(MOSI_GPIO, &GPIO_InitStruct);
	//3.使能SPI外设时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	//4.初始化配置SPI外设
	SPI_InitTypeDef SPI_InitStruct;
	SPI_StructInit(&SPI_InitStruct);
	
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//SPI时钟分频  4分频 9Mhz
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;//时钟相位, 第一个边沿采样 主要配合下面CPOL来选择使用SPI模式几的,这里设置为模式0
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;//时钟极性,默认电平状态
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;//数据帧格式8位
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//数据传输方向  全双工模式
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;//高位先行
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;//SPI模式  主机模式
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;//软件管理NSS还是硬件管理NSS 软件管理NSS
	SPI_InitStruct.SPI_CRCPolynomial = 7;//CRC多项式的值
	
	SPI_Init(SPI2, &SPI_InitStruct);
	//5.使能SPI
	SPI_Cmd(SPI2, ENABLE);
	//6.设置CS引脚默认高电平
	MySPI_W_CS(1);
}

/*
* 函数名:MySPI_Start
* 功能:硬件SPI起始位时序
* 参数:无
* 返回值:无
* 备注:无
*/
void MySPI_Start(void)
{
	MySPI_W_CS(0);
}

/*
* 函数名:MySPI_Start
* 功能:硬件SPI停止位时序
* 参数:无
* 返回值:无
* 备注:无
*/
void MySPI_Stop(void)
{
	MySPI_W_CS(1);
}

/*
* 函数名:MySPI_SwapByte
* 功能:硬件SPI交换一个字节时序
* 参数:SendData 发送的字节
* 返回值:交换接收的字节
* 备注:无
*/
uint16_t MySPI_SwapByte(uint16_t SendData)//模式0
{
	uint16_t ReceiveData = 0X00;
	
	//判断发送缓冲器空,1为空 0为非空
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);//不需要再手动清除标志位,当有数据进入的时候会自动清0
	//将要发送的数据放到TxE,发送缓冲器中
	SPI_I2S_SendData(SPI2, SendData);
	//判断接收缓冲器的状态 为1非空,为0空
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);//不需要再手动清除标志位,当没有数据的时候会自动清0
	//读取传送来的数据
	ReceiveData = SPI_I2S_ReceiveData(SPI2);
	
	return ReceiveData;
}

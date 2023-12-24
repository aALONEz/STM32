#include "stm32f10x.h"                  // Device header
#include <stdio.h>//引入printf头文件
#include <stdarg.h>
#include "SerialPort.h"

uint8_t SerialPort_RxPacket[4];//接收
uint8_t SerialPort_TxPacket[4];//发送
uint8_t SerialPort_RxFlag;//接收标志位


void SerialPort_Init(void)
{
	//开启GPIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//开启USART3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//PB10
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//IO口电平反转速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//PB11
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//输入模式无效 IO口电平反转速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//USART初始化
	USART_InitTypeDef USART_InitStruct;
	
	USART_InitStruct.USART_BaudRate = 9600;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//USART模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//校验位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//数据宽度
	
	USART_Init(USART3, &USART_InitStruct);
	//开启中断
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//开启优先级分组,整个工程只需要开启一次
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC初始化配置
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;//USART3通道
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	
	NVIC_Init(&NVIC_InitStruct);
	//USART3使能
	USART_Cmd(USART3, ENABLE);
	
}

/*
* 函数名:SerialPort_SendByte
* 功能:USART3 发送一字节数据
* 参数:Data 要发送的数据
* 返回值:无
* 备注:USART3_TX (PB10)
*/
void SerialPort_SendByte(uint8_t Data)
{
	USART_SendData(USART3, Data);
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);//判断发送寄存器是否为空.
	//不需要清除USART_FLAG_TXE标志位状态,因为在写入发送寄存器的时候会自动将这个标志位清零.
}


void SerialPort_SendPacket(void)
{
	//发送包头
	SerialPort_SendByte(0XFF);
	//发送数据段
	SerialPort_SendArray(SerialPort_TxPacket, sizeof(SerialPort_TxPacket) / sizeof(SerialPort_TxPacket[0]));
	//发送包尾
	SerialPort_SendByte(0XFE);
}

/*
* 函数名:SerialPort_SendArray
* 功能:USART3 发送一字节数据
* 参数:DataArray 要发送的数组  Length 数组的长度
* 返回值:无
* 备注:USART3_TX (PB10)
*/
void SerialPort_SendArray(uint8_t* DataArray, uint16_t Length)
{
	for(int i = 0; i < Length; i++)
	{
		USART_SendData(USART3, DataArray[i]);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);//判断发送寄存器是否为空.
		//不需要清除USART_FLAG_TXE标志位状态,因为在写入发送寄存器的时候会自动将这个标志位清零.
	}
	
}

/*
* 函数名:SerialPort_SendString
* 功能:USART3 发送字符串
* 参数:Data 要发送的字符串首字符指针
* 返回值:无
* 备注:USART3_TX (PB10)
*/
void SerialPort_SendString(char* Data)
{
	while(*Data)
	{
		USART_SendData(USART3, *Data);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);//判断发送寄存器是否为空.
		//不需要清除USART_FLAG_TXE标志位状态,因为在写入发送寄存器的时候会自动将这个标志位清零.
		Data++;
	}
}

static uint32_t Pow(const uint32_t Number, uint8_t Length)
{
	uint32_t temp = 1;
	for(int i = 0; i < Length; i++)
	{
		temp *= Number;
	}
	return temp;
}

/*
* 函数名:SerialPort_SendNumber
* 功能:USART3 发送数字
* 参数:Number 要发送的数字
* 返回值:无
* 备注:USART3_TX (PB10)    只能发送正整数
*/
void SerialPort_SendNumber(uint32_t Number)
{
	uint8_t Length = 0;
	uint32_t TempNumber = Number;
	//先求出数字位数
	while(TempNumber != 0)
	{
		TempNumber /= 10;
		Length++;
	}
	//显示成数字字符的时候肯定是先发送数字的高位,所以这里要先算出高位数字发出
	for(int i = 0; i < Length; i++)
	{
		TempNumber = Number / Pow(10, Length - i - 1) + '0';
		SerialPort_SendByte((uint8_t)TempNumber);
		Number %= Pow(10, Length - i - 1);
	}
	if(Length == 0)//数字长度为0时,发送字符零
	{
		SerialPort_SendByte('0');
	}
}

/*
* 函数名:fputc
* 功能:移植printf需要,将fputc重定向到我们自己的字节发送函数.printf底层使用的就是fputc打印到屏幕上的
*/
int fputc(int ch, FILE* f)
{
	SerialPort_SendByte(ch);
	return ch;
}

/*
* 函数名:SerialPort_Printf
* 功能:封装sprintf实现printf打印到串口
*/
void SerialPort_Printf(char* format, ...)
{
	char String[100] = {0};
	va_list arg;
	va_start(arg, format);
	vsprintf(String, format, arg);
	va_end(arg);
	SerialPort_SendString(String);
}

/*
* 函数名:SerialPort_GetRxFlag
* 功能:判断与修改SerialPort_RxFlag的值,自定义标志位
* 参数:无
* 返回值:1 代表此时SerialPort_GetRxData已经接收到新的数据了,0表示没有接收到新的数据
* 备注:无
*/
uint8_t SerialPort_GetRxFlag(void)
{
	if(SerialPort_RxFlag == 1)
	{
		SerialPort_RxFlag = 0;
		return 1;
	}
	return 0;
}

	//配置中断服务函数模板
void USART3_IRQHandler(void)
{
	static uint8_t RxState = 0;
	static uint8_t DataFlag = 0;
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		//获取数据
		uint8_t RxData = USART_ReceiveData(USART3);
		
		if(RxState == 0)//等待包头
		{
			if(RxData == 0XFF)
			{
				RxState = 1;//将状态切换为等待接收数据状态
				DataFlag = 0;
			}
			
		}else if(RxState == 1)//等待数据
		{
			SerialPort_RxPacket[DataFlag++] = RxData;
			if(DataFlag >= 4)
			{
				DataFlag = 0;
				RxState = 2;
			}
			
		}else if(RxState == 2)//等待包尾
		{
			if(RxData == 0XFE)
			{
				RxState = 0;//将状态切换为等待接收包头状态
				SerialPort_RxFlag = 1;
			}
		}
		//清除中断标志位,虽然会自动清除但是手动也清除一下,不清除也可以
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}


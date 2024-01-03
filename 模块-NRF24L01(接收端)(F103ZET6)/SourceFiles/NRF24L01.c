#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "NRF24L01.h"

uint8_t TxBuf[32] = {0X00};
uint8_t RxBuf[32] = {0X00};

uint8_t ByteCount_Q = 1;//接收多少个有效自己的数据

static void NRF24L01_W_CE(uint8_t BitVal)
{
	GPIO_WriteBit(NRF24L01_CE_GPIO, NRF24L01_CE_Pin, (BitAction)BitVal);
}

static uint8_t NRF24L01_R_IRQ(void)
{
	return GPIO_ReadInputDataBit(NRF24L01_IRQ_GPIO, NRF24L01_IRQ_Pin);
}

/*
* 函数名:NRF24L01_Init
* 功能:NRF24L01初始化,初始化为待机模式
* 参数:无
* 返回值:无
* 备注:CS引脚初始化在MySPI_Init()函数中
*/
void NRF24L01_Init(void)
{
	//1.使能GPIO时钟
	RCC_APB2PeriphClockCmd(NRF24L01_IRQ_RCC | NRF24L01_CE_RCC, ENABLE);
	//初始化配置GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	//IRQ
	GPIO_InitStruct.GPIO_Pin = NRF24L01_IRQ_Pin;//NRF24L01中断信号接收引脚
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平翻转速度,输入模式无效,但是要赋值
	GPIO_Init(NRF24L01_IRQ_GPIO, &GPIO_InitStruct);
	//CE
	GPIO_InitStruct.GPIO_Pin = NRF24L01_CE_Pin;//NRF24L01使能引脚,在 CS(片选)为低的情况下，CE 协同CONFIG 寄存器共同决定NRF24L01 的状态
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平翻转速度
	GPIO_Init(NRF24L01_CE_GPIO, &GPIO_InitStruct);
	//2.调用MySPI
	MySPI_Init();
	//4.设置SPI时钟速率为9Mhz(NRF24L01最大速率是10Mhz)
	
	//3.NRF24L01待机
	GPIO_ResetBits(NRF24L01_CE_GPIO, NRF24L01_CE_Pin);//低电平
	//4.片选取消
	MySPI_Stop();//不选中
}

/*
* 函数名:NRF24L01_Write_Reg
* 功能:NRF24L01指定地址写一个字节
* 参数:Reg 要写入的寄存器地址
*      Value 要写入的值(一个字节)
* 返回值:无
* 备注:只需填写寄存器地址,无需填写指令代码
*/
void NRF24L01_Write_Reg(uint8_t Reg, uint8_t Value)
{
	//1.拉低NRF24L01的片选,也就是选中
	MySPI_Start();//选中
	//2.发送写指令或上寄存器地址,再接收发送回来的数据
	MySPI_SwapByte(NRF24L01_Write | Reg);
	//3.发送要写入的数据
	MySPI_SwapByte(Value);
	//4.释放片选,也就是不选中
	MySPI_Stop();//不选中
}

/*
* 函数名:NRF24L01_Read_Reg
* 功能:NRF24L01指定地址读一个字节
* 参数:Reg 要读取的寄存器地址
* 返回值:uint8_t 读取的值(一个字节)
* 备注:只需填写寄存器地址,无需填写指令代码
*/
uint8_t NRF24L01_Read_Reg(uint8_t Reg)
{
	uint8_t Data = 0X00;
	
	//1.拉低NRF24L01的片选,也就是选中
	MySPI_Start();//选中
	//2.发送写指令或上寄存器地址,再接收发送回来的数据
	MySPI_SwapByte(NRF24L01_Read | Reg);
	//3.发送空的数据,换回有用的数据
	Data = MySPI_SwapByte(NRF24L01_NOP);
	//4.释放片选,也就是不选中
	MySPI_Stop();//不选中
	
	return Data;
}

/*
* 函数名:NRF24L01_Read_Buf
* 功能:NRF24L01指定地址读1~32个字节
* 参数:Reg 要读取的寄存器地址
*      Buf 保存读取的值的数组指针(uint8_t*)
*      Count 要读取多少个字节
* 返回值:无
* 备注:只需填写寄存器地址,无需填写指令代码
*/
void NRF24L01_Read_Buf(uint8_t Reg, uint8_t* Buf, uint8_t Count)
{
	uint8_t i;
	//1.拉低NRF24L01的片选,也就是选中
	MySPI_Start();//选中
	//2.发送要读取的指令加寄存器地址
	MySPI_SwapByte(NRF24L01_Read | Reg);
	//3.读取接收到的地址
	for(i = 0; i < Count; i++)
	{
		Buf[i] = MySPI_SwapByte(NRF24L01_NOP);
	}
	//4.释放片选,也就是不选中
	MySPI_Stop();//不选中
}

/*
* 函数名:NRF24L01_Write_Buf
* 功能:NRF24L01指定地址写1~32个字节
* 参数:Reg 要写入的寄存器地址
*      Buf 写入的值的数组指针(uint8_t*)
*      Count 要写入多少个字节
* 返回值:无
* 备注:只需填写寄存器地址,无需填写指令代码
*/
void NRF24L01_Write_Buf(uint8_t Reg, uint8_t* Buf, uint8_t Count)
{
	uint8_t i;
	//1.拉低NRF24L01的片选,也就是选中
	MySPI_Start();//选中
	//2.发送要写入的指令加寄存器地址
	MySPI_SwapByte(NRF24L01_Write | Reg);
	//3.读取接收到的地址
	for(i = 0; i < Count; i++)
	{
		 MySPI_SwapByte(Buf[i]);
	}
	//4.释放片选,也就是不选中
	MySPI_Stop();//不选中
}

/*
* 函数名:NRF24L01_Tx_Mode_Init
* 功能:NRF24L01发送模式初始化,切记必须要先初始化NRF24L01,也就是要先调用NRF24L01_Init()
* 参数:无
* 返回值:无
* 备注:无
*/
void NRF24L01_Tx_Mode_Init(void)
{
	//1.配置TxADDR(发送地址)
	//CE低电平,待机模式
	NRF24L01_W_CE(0);
	//写Tx_ADDR寄存器,编写发送地址
	TxBuf[0] = (uint8_t)(Tx_Address >> 32);
	TxBuf[1] = (uint8_t)(Tx_Address >> 24);
	TxBuf[2] = (uint8_t)(Tx_Address >> 16);
	TxBuf[3] = (uint8_t)(Tx_Address >> 8);
	TxBuf[4] = (uint8_t)(Tx_Address >> 0);
	//写Tx_ADDR寄存器
	NRF24L01_Write_Buf(NRF24L01_Tx_ADDR, TxBuf, 5);
	
	//2.写Rx_ADDR_P0(为了使能Auto Ack)
	NRF24L01_Write_Buf(NRF24L01_Rx_ADDR_P0, TxBuf, 5);
	
	//3.使能AutoAck,配置EN_AA寄存器
	NRF24L01_Write_Reg(NRF24L01_EN_AA, 0X01);//使能通道0的自动应答
	
	//4.使能PIPE 0 配置EN_RXADDR
	NRF24L01_Write_Reg(NRF24L01_EN_RXADDR, 0X01);//使能通道0的接收地址允许
	
	//5.配置自动重发次数,配置SETUP_RETR寄存器
	NRF24L01_Write_Reg(NRF24L01_SETUP_RETR, 0X1A);//设置自动重发次数为10次,自动重发延时为586us
	
	//6.配置通信频率,配置RF_CH寄存器
	NRF24L01_Write_Reg(NRF24L01_RF_CH, NRF24L01_RATE);//配置通信频率2400 + 119Mhz
	
	//7.配置发射参数,配置RF_SETUP寄存器
	NRF24L01_Write_Reg(NRF24L01_RF_SETUP, 0X0F);//配置发射功率最大 数据传输速率2Mbps 开启低噪声放大器增益
	
	//8.配置NRF24L01的基本参数和工作模式,配置CONFIG寄存器
	NRF24L01_Write_Reg(NRF24L01_CONFIG, 0X0E);//IRQ引脚不显示所有中断,CRC使能,16位CRC校验 ,上电,发送模式
	
	//9.CE高电平,PWR_UP为1,PRIM_RX0,发送模式
	NRF24L01_W_CE(1);//低电平关闭发送,高电平持续10us后立马发送
}

/*
* 函数名:NRF24L01_TxPacket
* 功能:NRF24L01发送数据
* 参数:Buf 要发送数据数组的指针(uint8*)
*      Count 要发送多少个字节
* 返回值:uint8_t 发送状态错误码,详细看头文件错误码定义
* 备注:无
*/
uint8_t NRF24L01_TxPacket(uint8_t* Buf, uint8_t Count)
{
	uint8_t State;
	//1.CE低电平,待机模式
	NRF24L01_W_CE(0);
	//2.将数据发送到NRF24L01的发送缓冲空间
	NRF24L01_Write_Buf(NRF24L01_Write_TX, Buf, Count);
	//3.开启发送
	NRF24L01_W_CE(1);
	//下面是判断发送完成的代码
	//4.检测中断引脚电平
	while(NRF24L01_R_IRQ() != 0);//此处不会死循环,因为如果达到最大重发次数也会将这个引脚电平拉低
	//5.读取NRF24L01状态寄存器
	State = NRF24L01_Read_Reg(NRF24L01_STATUS);
	//6.清除中断标志位
	NRF24L01_Write_Reg(NRF24L01_STATUS, State);
	if(((State & 0X10) >> 4) == 1)//判断是不是最大重发次数导致中断
	{
		NRF24L01_Write_Reg(NRF24L01_Clear_TX, NRF24L01_NOP);//清除NRF24L01的发送寄存器(Tx_FIFO).
		return NRF24L01_Tx_ERROR_MAXRT;
	}
	if(((State & 0X20) >> 5) == 1)
	{
		return NRF24L01_Tx_OK;//返回发送完成状态
	}
	return NRF24L01_Tx_UNKNOWN;//返回未知错误状态
}

void NRF24L01_Rx_EXTI_Init(void)
{
	//1.使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//使能AFIO时钟。只有使能AFIO才可以连接GPIO到EXTI的线路
	//2.GPIO链接EXTI.
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);//PA1
	//3.初始化EXTI
	EXTI_InitTypeDef EXTI_InitStruct;
	
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;//通道6
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;//使能
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//EXTI模式,中断
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿激活中断
	
	EXTI_Init(&EXTI_InitStruct);
	//4.初始化中断分组,整个芯片只需要初始化一次
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//5.初始化NVIC
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;
	
	NVIC_Init(&NVIC_InitStruct);
}

/*
* 函数名:NRF24L01_Rx_Mode_Init
* 功能:NRF24L01接收模式初始化,切记必须要先初始化NRF24L01,也就是要先调用NRF24L01_Init()
* 参数:无
* 返回值:无
* 备注:利用外部中断检测是否接受到数据
*/
void NRF24L01_Rx_Mode_Init(uint8_t ByteCount)
{
	ByteCount_Q = ByteCount;
	//1.配置RX_ADDR_P0(接收通道0地址)
	//CE低电平,待机模式
	NRF24L01_W_CE(0);
	//写RX_ADDR_P0寄存器,编写接收通道0地址,一般与发送端的发送地址保持一致
	TxBuf[0] = (uint8_t)(Tx_Address >> 32);
	TxBuf[1] = (uint8_t)(Tx_Address >> 24);
	TxBuf[2] = (uint8_t)(Tx_Address >> 16);
	TxBuf[3] = (uint8_t)(Tx_Address >> 8);
	TxBuf[4] = (uint8_t)(Tx_Address >> 0);
	//写RX_ADDR_P0寄存器
	NRF24L01_Write_Buf(NRF24L01_Rx_ADDR_P0, TxBuf, 5);
	
	//2.使能AutoAck,配置EN_AA寄存器
	NRF24L01_Write_Reg(NRF24L01_EN_AA, 0X01);//使能通道0的自动应答
	
	//3.使能PIPE 0寄存器,配置EN_RXADDR寄存器
	NRF24L01_Write_Reg(NRF24L01_EN_RXADDR, 0X01);//使能通道0的接收地址允许
	
	//4.配置通信频率,配置RF_CH寄存器,与发送端保持一致
	NRF24L01_Write_Reg(NRF24L01_RF_CH, NRF24L01_RATE);//配置通信频率2400 + 119Mhz
	
	//5.配置通道零有效数据宽度,配置Rx_Pw_P0寄存器.
	NRF24L01_Write_Reg(NRF24L01_Rx_Pw_P0, ByteCount);//配置的有效数据宽度要与发送方发送的字节个数匹配上
	
	//6.配置发射参数,配置RF_SETUP寄存器
	NRF24L01_Write_Reg(NRF24L01_RF_SETUP, 0X0F);
	
	//7.配置NRF24L01的基本参数和工作模式,配置CONFIG寄存器
	NRF24L01_Write_Reg(NRF24L01_CONFIG, 0X0F);//开启所有中断对引脚的电平控制,接收模式
	
	//8.开启外部中断,接收中断,接收完成就会置标志位
	NRF24L01_Rx_EXTI_Init();
	
	//9.CE高电平,PWR_UP为1,PRIM_RX1,接收模式
	NRF24L01_W_CE(1);
}

/*
* 函数名:NRF24L01_RxPacket
* 功能:NRF24L01接收模式接收数据
* 参数:无
* 返回值:无
* 备注:利用外部中断检测是否接受到数据,收到数据就接收数据,最大接收32字节
*/
void NRF24L01_RxPacket(uint8_t ByteCount_Q)
{
	uint8_t State;
	//读取状态寄存器
	State = NRF24L01_Read_Reg(NRF24L01_STATUS);
	//清除状态寄存器标志位
	NRF24L01_Write_Reg(NRF24L01_STATUS, State);
	//读取接收到的数据
	NRF24L01_Read_Buf(NRF24L01_Read_RX, RxBuf, ByteCount_Q);//读取多少个接收的字节,一般与发送方的字节数一致
	//清除接收寄存器
	NRF24L01_Write_Reg(NRF24L01_Clear_RX, NRF24L01_NOP);
}

/*
* 函数名:EXTI9_5_IRQHandler
* 功能:EXTI9_5_IRQHandler 中断服务函数
* 参数:无
* 返回值:无
* 备注:利用外部中断检测是否接受到数据,收到数据就调用接收数据的函数
*/
void EXTI1_IRQHandler(void)
{
	//判断什么标志位导致的中断
	if(EXTI_GetITStatus(EXTI_Line1) == SET)//判断是不是通道6导致的中断
	{
		NRF24L01_RxPacket(ByteCount_Q);
		//清除中断服务函数标志位
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

/*
* 函数名:NRF24L01_Check
* 功能:检测NRF24L01是否存在SPI2上
* 参数:无
* 返回值:uint8_t NRF24L01_ABSENT 不存在  NRF24L01_EXIST存在
* 备注:利用外部中断检测是否接受到数据,收到数据就调用接收数据的函数
*/
uint8_t NRF24L01_Check(void)
{
	uint8_t Buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	//修改发送地址
	NRF24L01_Write_Buf(NRF24L01_Tx_ADDR, Buf, 5);
	NRF24L01_Read_Buf(NRF24L01_Tx_ADDR, Buf, 5);
	for(uint8_t i = 0; i < 5; i++)
	{
		if(Buf[i] != 0XA5)
		{
			return NRF24L01_ABSENT;//不存在
		}
	}
	return NRF24L01_EXIST;//存在
}


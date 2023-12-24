#include "stm32f10x.h"                  // Device header
#include "Delay.h"



//SCL
#define SCL_RCC		RCC_APB2Periph_GPIOE
#define SCL_GPIO 	GPIOE
#define SCL_Pin 	GPIO_Pin_1

//SDA
#define SDA_RCC		RCC_APB2Periph_GPIOE
#define SDA_GPIO 	GPIOE
#define SDA_Pin 	GPIO_Pin_0

/*
* 函数名:MyI2C_W_SCL
* 功能:软件模拟I2C,设置SCL引脚电平状态
* 参数:BitValue 要设置的值,1高电平,0低电平
* 返回值:无
* 备注:无
*/
static void MyI2C_W_SCL(uint8_t BitValue)
{
	GPIO_WriteBit(SCL_GPIO, SCL_Pin, (BitAction)BitValue);
	Delay_us(10);
}

/*
* 函数名:MyI2C_W_SDA
* 功能:软件模拟I2C,设置SDA引脚电平状态
* 参数:BitValue 要设置的值,1高电平,0低电平
* 返回值:无
* 备注:无
*/
static void MyI2C_W_SDA(uint8_t BitValue)
{
	GPIO_WriteBit(SDA_GPIO, SDA_Pin, (BitAction)BitValue);
	Delay_us(10);
}

/*
* 函数名:MyI2C_R_SDA
* 功能:软件模拟I2C,读取SDA引脚电平状态
* 参数:无
* 返回值:BitValue 1高电平,0低电平
* 备注:无
*/
static uint8_t MyI2C_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = GPIO_ReadInputDataBit(SDA_GPIO, SDA_Pin);
	Delay_us(10);
	return BitValue;
}

/*
* 函数名:MyI2C_Init
* 功能:软件模拟I2C,初始化
* 参数:无
* 返回值:无
* 备注:SCL连接PE1,SDA连接PE0.如果需要修改引脚,请修改上面宏定义内容
*/
void MyI2C_Init(void)
{
	//使能GPIO时钟
	RCC_APB2PeriphClockCmd(SCL_RCC, ENABLE);//GPIOE
	//GPIO初始化,设置为开漏输出模式
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = SDA_Pin | SCL_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(SDA_GPIO, &GPIO_InitStruct);
	//设置两个引脚为高电平
	GPIO_SetBits(SCL_GPIO, SCL_Pin);//SCL
	GPIO_SetBits(SDA_GPIO, SDA_Pin);//SDA
}

/*
* 函数名:MyI2C_Start
* 功能:软件模拟I2C,起始位
* 参数:无
* 返回值:无
* 备注:无
*/
void MyI2C_Start(void)
{
	//为了确保稳定,这里要先将SCL和SDA全部是高电平,也就是全部释放
	//设置两个引脚为高电平,这里建议先释放SDA因为这里此函数需要兼容在时序中插入起始位,兼容重复起始条件
	MyI2C_W_SDA(1);//SDA 1
	MyI2C_W_SCL(1);//SCL 1
	//接着将SDA拉低
	MyI2C_W_SDA(0);//SDA 0
	//将SCL电平拉低
	MyI2C_W_SCL(0);//SCL 0
}

/*
* 函数名:MyI2C_Stop
* 功能:软件模拟I2C,停止位
* 参数:无
* 返回值:无
* 备注:无
*/
void MyI2C_Stop(void)
{
	//在停止位时序开始前不清楚SDA的时序情况,所以这里需要先把SDA拉低然后再将SCL拉高,再将SDA拉高
	MyI2C_W_SDA(0);//SDA 0
	MyI2C_W_SCL(1);//SCL 1
	MyI2C_W_SDA(1);//SDA 1
}

/*
* 函数名:MyI2C_SendByte
* 功能:软件模拟I2C,发送一个字节
* 参数:Byte 要发送的字节
* 返回值:无
* 备注:无
*/
void MyI2C_SendByte(uint8_t Byte)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		//每次准备发送数据的时候SCL都是低电平所以这里不需要再将SCL置低电平.(只有SCL低电平才可以放数据)
		//放入对应位的数据,高位先行
		MyI2C_W_SDA(Byte & (0X80 >> i));//&0X80确认Byte最高位是0或者1,非零就是1(高电平),0就是低电平
		//SCL产生一个时钟脉冲
		MyI2C_W_SCL(1);//SCL 1
		MyI2C_W_SCL(0);//SCL 0
	}
}

/*
* 函数名:MyI2C_ReceiveByte
* 功能:软件模拟I2C,接收一个字节
* 参数:无
* 返回值:Byte 要接收的字节
* 备注:无
*/
uint8_t MyI2C_ReceiveByte(void)
{
	uint8_t Byte = 0;
	MyI2C_W_SDA(1);//开始接收之前主机要先释放SDA,不释放SDA每次接收的就都是0
	for(uint8_t i = 0; i < 8; i++)
	{
		//SCL低电平从机放数据到SDA,SCL高电平主机读取SDA电平
		MyI2C_W_SCL(1);//SCL 1
		Byte |= MyI2C_R_SDA() << (8 - i - 1);
		MyI2C_W_SCL(0);//SCL 0
	}
	return Byte;
}

/*
* 函数名:MyI2C_SendAck
* 功能:软件模拟I2C,发送应答,发给从机的应答位
* 参数:AckBit 要发送的应答位,1为应答,0为不应答
* 返回值:无
* 备注:无
*/
void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(!(AckBit));//这里将AckBit逻辑取反,符合一下正常逻辑,此时AckBit的值为1就是表示应答了,0表示没有应答
	MyI2C_W_SCL(1);//SCL 1
	MyI2C_W_SCL(0);//SCL 0
}

/*
* 函数名:MyI2C_ReceiveAck
* 功能:软件模拟I2C,接收应答,等待从机的应答位
* 参数:无
* 返回值:AckBit 从机应答位,AckBit的值为1就是表示应答了,0表示没有应答
* 备注:无
*/
uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);//SCL 1
	AckBit = MyI2C_R_SDA();
	AckBit = !AckBit;//这里将结果逻辑取反,符合一下正常逻辑,此时AckBit的值为1就是表示应答了,0表示没有应答
	MyI2C_W_SCL(0);//SCL 0
	return AckBit;
}

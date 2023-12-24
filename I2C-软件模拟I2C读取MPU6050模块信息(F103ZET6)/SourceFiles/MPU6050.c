#include "stm32f10x.h"                  // Device header
#include "MyI2C.h"
#include "MPU6050.h"

#define MPU6050_Write 	0XD0//1101 000 0
#define MPU6050_Read  	0XD1//1101 000 1


/*
* 函数名:MPU6050_WriteReg
* 功能:MPU6050指定地址写一个Byte
* 参数:RegAddress 指定写入的地址
*      Data 指定写入的数据
* 返回值:无
* 备注:如果未找到从机将会直接返回,没有做没有找到从机的处理
*/
void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	MyI2C_Start();//起始位
	MyI2C_SendByte(MPU6050_Write);//发送从机地址加写入位
	if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	MyI2C_SendByte(RegAddress);//发送要写入的寄存器地址
	if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	MyI2C_SendByte(Data);//发送要写入的数据
	if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	MyI2C_Stop();//停止位
}

/*
* 函数名:MPU6050_WriteMultipleData
* 功能:MPU6050指定地址连续写多个Byte
* 参数:RegAddress 指定起始写入的地址
*      Data 指定写入的数据的数组指针
*	   Length 数组长度
* 返回值:无
* 备注:每字节写入之后会自动跳到下个地址写下一个字节
*/
void MPU6050_WriteMultipleData(uint8_t RegAddress, uint8_t* Data, uint8_t Length)
{
	MyI2C_Start();//起始位
	MyI2C_SendByte(MPU6050_Write);//发送从机地址加写入位
	if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	MyI2C_SendByte(RegAddress);//发送要写入的寄存器地址
	if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	for(uint8_t i = 0; i < Length; i++)//循环发送数组内的内容`
	{
		MyI2C_SendByte(*(Data + i));//发送要写入的数据
		if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	}
	MyI2C_Stop();//停止位
}

/*
* 函数名:MPU6050_ReadReg
* 功能:MPU6050指定地址读一个Byte
* 参数:RegAddress 指定读取的地址
* 返回值:uint8_t Data 读取到的一字节数据
* 备注:无
*/
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data = 0;
	//设置MPU6050寄存器内置指针,指向要读的地方
	MyI2C_Start();//起始位
	MyI2C_SendByte(MPU6050_Write);//发送从机地址加写入位
	if(MyI2C_ReceiveAck() == 0) return 0;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	MyI2C_SendByte(RegAddress);//发送要写入的寄存器地址
	if(MyI2C_ReceiveAck() == 0) return 0;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	//读取
	MyI2C_Start();//起始位
	MyI2C_SendByte(MPU6050_Read);//发送从机地址加读取位
	if(MyI2C_ReceiveAck() == 0) return 0;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	Data = MyI2C_ReceiveByte();
	MyI2C_SendAck(0);//发送应答位 不应答
	MyI2C_Stop();//停止位
	return Data;
}

/*
* 函数名:MPU6050_ReadMultipleData
* 功能:MPU6050指定地址连续读多个Byte
* 参数:Data 接收数据的数组指针
*	   RegAddress 指定起始读取的地址
*	   Length 数组长度,等于要接收多少个字节
* 返回值:无
* 备注:每字节写入之后会自动跳到下个地址写下一个字节
*/
void MPU6050_ReadMultipleData(uint8_t* Data, uint8_t RegAddress, uint8_t Length)
{
	//设置MPU6050寄存器内置指针,指向要读的地方
	MyI2C_Start();//起始位
	MyI2C_SendByte(MPU6050_Write);//发送从机地址加写入位
	if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	MyI2C_SendByte(RegAddress);//发送要写入的寄存器地址
	if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	//连续读取
	MyI2C_Start();//起始位
	MyI2C_SendByte(MPU6050_Read);//发送从机地址加读取位
	if(MyI2C_ReceiveAck() == 0) return;//没有应答就直接返回函数,或者可以没有应答就弹出一个提示之类的
	for(uint8_t i = 0; i < Length; i++)
	{
		*(Data + i) = MyI2C_ReceiveByte();//接收一个字节
		i >= (Length - 1) ? MyI2C_SendAck(0)/*发送应答位 不应答*/ : MyI2C_SendAck(1)/*发送应答位 应答*/;//i小于(Length - 1)就继续给应答,当接收到最后一个字节数据的时候要不给应答
	}
	MyI2C_Stop();//停止位
}

/*
* 函数名:MPU6050_Init
* 功能:MPU6050初始化,主要是检测有没有这个从机地址的设备
* 参数:无
* 返回值:char 成功找到从机返回1 未找到返回 0
* 备注:成功找到从机返回1 未找到返回 0
*/
uint8_t MPU6050_Init(void)
{
	MyI2C_Init();//初始化软件I2C
	MyI2C_Start();		 //7位地址  W (W(0)写入   R(1)读取)
	MyI2C_SendByte(MPU6050_Write);
	uint8_t Ack = MyI2C_ReceiveAck();
	MyI2C_Stop();
	if(Ack == 0)
	{
		return 0;//未找到MPU6050
	}
	//复位MPU6050所有寄存器
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0X80);//0X80 1 0 0 0 0 000
	//解除MPU6050睡眠模式和使用陀螺仪时钟
	uint8_t Power_State = MPU6050_ReadReg(MPU6050_PWR_MGMT_1);
	Power_State &= ~(1 << 6);//关闭睡眠
	Power_State |= 1;//使用内部陀螺仪时钟
	//此时Power_State 0 0 0 0 0 001
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, Power_State);//0X6B寄存器位6的值是1是开启睡眠模式,为0是关闭睡眠模式
	//设置电源管理器2寄存器,6轴全部不待机
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0X00);
	//设置MPU6050采样分频
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0X09);//值越小数据输出越快
	//配置MPU6050配置寄存器
	MPU6050_WriteReg(MPU6050_CONFIG, 0X06);//00 000 110
	//配置陀螺仪寄存器
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0X18);//XYZ轴自检失能 量程选择最大
	//配置加速度计寄存器
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0X18);//XYZ轴自检失能 量程选择最大 高通滤波器(高八位)不开启
	return 1;//找到MPU6050
}

/*
* 函数名:MPU6050_Get_ACCEL_X
* 功能:MPU6050获取加速度计X轴的数据
* 参数:无
* 返回值:int16_t X_Value 寄存器0x3B和0X3C组成的16位有符号值
* 备注:无
*/
int16_t MPU6050_Get_ACCEL_X(void)
{
	int16_t X_Value = 0;
	uint8_t ACCEL_X_Data[2] = {0};
	MPU6050_ReadMultipleData(ACCEL_X_Data, MPU6050_ACCEL_XOUT_H, (sizeof(ACCEL_X_Data) / sizeof(ACCEL_X_Data[0])));
	X_Value = (((uint16_t)ACCEL_X_Data[0]) << 8);
	X_Value |= ((uint16_t)ACCEL_X_Data[1]);
	return X_Value;
}

/*
* 函数名:MPU6050_Get_ACCEL_Y
* 功能:MPU6050获取加速度计Y轴的数据
* 参数:无
* 返回值:int16_t Y_Value 寄存器0x3D和0X3E组成的16位有符号值
* 备注:无
*/
int16_t MPU6050_Get_ACCEL_Y(void)
{
	int16_t Y_Value = 0;
	uint8_t ACCEL_Y_Data[2] = {0};
	MPU6050_ReadMultipleData(ACCEL_Y_Data, MPU6050_ACCEL_YOUT_H, (sizeof(ACCEL_Y_Data) / sizeof(ACCEL_Y_Data[0])));
	Y_Value = (((uint16_t)ACCEL_Y_Data[0]) << 8);
	Y_Value |= ((uint16_t)ACCEL_Y_Data[1]);
	return Y_Value;
}

/*
* 函数名:MPU6050_Get_ACCEL_Z
* 功能:MPU6050获取加速度计Z轴的数据
* 参数:无
* 返回值:int16_t Z_Value 寄存器0x3F和0X40组成的16位有符号值
* 备注:无
*/
int16_t MPU6050_Get_ACCEL_Z(void)
{
	int16_t Z_Value = 0;
	uint8_t ACCEL_Z_Data[2] = {0};
	MPU6050_ReadMultipleData(ACCEL_Z_Data, MPU6050_ACCEL_ZOUT_H, (sizeof(ACCEL_Z_Data) / sizeof(ACCEL_Z_Data[0])));
	Z_Value = (((uint16_t)ACCEL_Z_Data[0]) << 8);
	Z_Value |= ((uint16_t)ACCEL_Z_Data[1]);
	return Z_Value;
}

/*
* 函数名:MPU6050_Get_TEMP
* 功能:MPU6050获取温度计的数据
* 参数:无
* 返回值:int16_t Temp_Value 寄存器0x41和0X42组成的16位有符号值
* 备注:无
*/
int16_t MPU6050_Get_TEMP(void)
{
	int16_t Temp_Value = 0;
	uint8_t TEMP_Data[2] = {0};
	MPU6050_ReadMultipleData(TEMP_Data, MPU6050_TEMP_OUT_H, (sizeof(TEMP_Data) / sizeof(TEMP_Data[0])));
	Temp_Value = (((uint16_t)TEMP_Data[0]) << 8);
	Temp_Value |= ((uint16_t)TEMP_Data[1]);
	return Temp_Value;
}

/*
* 函数名:MPU6050_Get_GYRO_X
* 功能:MPU6050获取陀螺仪X轴的数据
* 参数:无
* 返回值:int16_t X_Value 寄存器0x43和0X44组成的16位有符号值
* 备注:无
*/
int16_t MPU6050_Get_GYRO_X(void)
{
	int16_t X_Value = 0;
	uint8_t X_Data[2] = {0};
	MPU6050_ReadMultipleData(X_Data, MPU6050_GYRO_XOUT_H, (sizeof(X_Data) / sizeof(X_Data[0])));
	X_Value = (((uint16_t)X_Data[0]) << 8);
	X_Value |= ((uint16_t)X_Data[1]);
	return X_Value;
}

/*
* 函数名:MPU6050_Get_GYRO_Y
* 功能:MPU6050获取陀螺仪Y轴的数据
* 参数:无
* 返回值:int16_t Y_Value 寄存器0x45和0X46组成的16位有符号值
* 备注:无
*/
int16_t MPU6050_Get_GYRO_Y(void)
{
	int16_t Y_Value = 0;
	uint8_t Y_Data[2] = {0};
	MPU6050_ReadMultipleData(Y_Data, MPU6050_GYRO_YOUT_H, (sizeof(Y_Data) / sizeof(Y_Data[0])));
	Y_Value = (((uint16_t)Y_Data[0]) << 8);
	Y_Value |= ((uint16_t)Y_Data[1]);
	return Y_Value;
}

/*
* 函数名:MPU6050_Get_GYRO_Z
* 功能:MPU6050获取陀螺仪Z轴的数据
* 参数:无
* 返回值:int16_t Z_Value 寄存器0x47和0X48组成的16位有符号值
* 备注:无
*/
int16_t MPU6050_Get_GYRO_Z(void)
{
	int16_t Z_Value = 0;
	uint8_t Z_Data[2] = {0};
	MPU6050_ReadMultipleData(Z_Data, MPU6050_GYRO_ZOUT_H, (sizeof(Z_Data) / sizeof(Z_Data[0])));
	Z_Value = (((uint16_t)Z_Data[0]) << 8);
	Z_Value |= ((uint16_t)Z_Data[1]);
	return Z_Value;
}

/*
* 函数名:MPU6050_Get_ID
* 功能:MPU6050获取MPU6050的设备ID
* 参数:无
* 返回值:int16_t Z_Value 寄存器0x47和0X48组成的16位有符号值
* 备注:无
*/
uint8_t MPU6050_Get_DeviceID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

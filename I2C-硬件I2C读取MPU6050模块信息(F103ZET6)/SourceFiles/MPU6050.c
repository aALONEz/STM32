#include "stm32f10x.h"                  // Device header
#include "MPU6050.h"

/*
* 函数名:Time_Out_Check
* 功能:状态检测超时函数,检测指定EVx事件,超时还未检测到就停止检测
* 参数:I2Cx 指定的I2C外设
*      I2C_EVENT 指定的EVx事件
* 返回值:无
* 备注:无
*/
static void Time_Out_Check(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Time_Out = 10000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Time_Out--;
		if(Time_Out <= 1)
		{
			/*可以添加错误提示,比如向串口发送错误信息*/
			break;
		}
	}
}

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
	//1.发送起始位
	I2C_GenerateSTART(I2C2, ENABLE);
	//2.等待相关EV5事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	//3.发送从机地址加操作位
	I2C_Send7bitAddress(I2C2, 0XD0, I2C_Direction_Transmitter);
	//4.等待相关EV6事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	//5.发送要写入的寄存器地址
	I2C_SendData(I2C2, RegAddress);
	//6.等待相关EV8事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	//7.发送要写入的数据
	I2C_SendData(I2C2, Data);
	//8.等待相关EV8_2事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	//9.发送停止位
	I2C_GenerateSTOP(I2C2, ENABLE);
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
	//1.发送起始位
	I2C_GenerateSTART(I2C2, ENABLE);
	//2.等待相关EV5事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	//3.发送从机地址加操作位
	I2C_Send7bitAddress(I2C2, 0XD0, I2C_Direction_Transmitter);
	//4.等待相关EV6事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	//5.发送要写入的寄存器地址
	I2C_SendData(I2C2, RegAddress);
	//6.等待相关EV8事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
	for(uint8_t i = 0; i < Length; i++)
	{
		//7.发送要写入的数据
		I2C_SendData(I2C2, *(Data + 1));
		if(i >= Length - 1)//当是要发送的最后一个字节的时候要等待EV8_2事件而不是EV8事件
		{
			//8.等待相关EV8_2事件标志位
			Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
		}
		else
		{
			//8.等待相关EV8事件标志位
			Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING);
		}
	}
	//9.发送停止位
	I2C_GenerateSTOP(I2C2, ENABLE);
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
	//指定地址读,指定地址操作
	//1.发送起始位
	I2C_GenerateSTART(I2C2, ENABLE);
	//2.等待相关EV5事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	//3.发送从机地址加操作位
	I2C_Send7bitAddress(I2C2, 0XD0, I2C_Direction_Transmitter);//写
	
	//4.等待相关EV6事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	//5.发送要写入的寄存器地址
	I2C_SendData(I2C2, RegAddress);
	//6.等待相关EV8_2事件标志位,这里等待EV8事件也可以
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	//指定地址读,读操作
	//7.发送起始位
	I2C_GenerateSTART(I2C2, ENABLE);
	//8.等待相关EV5事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	//3.发送从机地址加操作位
	I2C_Send7bitAddress(I2C2, 0XD0, I2C_Direction_Receiver);//读
	//4.等待相关EV6事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	//5.应答位设置为非应答,这里一定要在最后一个字节之前设置为非应答
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	//6.发送停止位,此时停止位不会立即发送,会等待接收最后一个字节后再发送
	I2C_GenerateSTOP(I2C2, ENABLE);
	//7.等待EV7事件,等待一个字节接收完的事件
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
	//8.接收读的数据
	Data = I2C_ReceiveData(I2C2);
	//9.接收完数据之后将应答位开启
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
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
	//指定地址读,指定地址操作
	//1.发送起始位
	I2C_GenerateSTART(I2C2, ENABLE);
	//2.等待相关EV5事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	//3.发送从机地址加操作位
	I2C_Send7bitAddress(I2C2, 0XD0, I2C_Direction_Transmitter);//写
	//4.等待相关EV6事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);
	//5.发送要写入的寄存器地址
	I2C_SendData(I2C2, RegAddress);
	//6.等待相关EV8_2事件标志位,这里等待EV8事件也可以
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED);
	
	//指定地址读,读操作
	//7.发送起始位
	I2C_GenerateSTART(I2C2, ENABLE);
	//8.等待相关EV5事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_MODE_SELECT);
	//3.发送从机地址加操作位
	I2C_Send7bitAddress(I2C2, 0XD0, I2C_Direction_Receiver);//读
	//4.等待相关EV6事件标志位
	Time_Out_Check(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);
	for(uint8_t i = 0; i < Length; i++)
	{
		if(i >= Length - 1)
		{
			//5.应答位设置为非应答,这里一定要在最后一个字节之前设置为非应答
			I2C_AcknowledgeConfig(I2C2, DISABLE);
			//6.发送停止位,此时停止位不会立即发送,会等待接收最后一个字节后再发送
			I2C_GenerateSTOP(I2C2, ENABLE);
		}
		//7.等待EV7事件,等待一个字节接收完的事件
		Time_Out_Check(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED);
		//8.接收读的数据
		*(Data + i) = I2C_ReceiveData(I2C2);
	}
	//9.接收完数据之后将应答位开启
	I2C_AcknowledgeConfig(I2C2, ENABLE);
}

/*
* 函数名:MPU6050_Init
* 功能:MPU6050初始化,主要是检测有没有这个从机地址的设备
* 参数:无
* 返回值:无
* 备注:无
*/
void MPU6050_Init(void)
{
	//1.使能I2C外设时钟和GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);//I2C2外设时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//GPIOB外设时钟使能
	//2.设置PB10与PB11为复用开漏输出
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;//Pin10和Pin11
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;//复用开漏输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//电平反转速度
	
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	//3.初始化配置I2C
	I2C_InitTypeDef I2C_InitStruct;
	
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;//使能 应答位
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//从机模式下是7位地址还是10位地址
	I2C_InitStruct.I2C_ClockSpeed = 50000;//50KHz
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;//占空比 2(低电平) : 1(高电平)
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;//I2C模式
	I2C_InitStruct.I2C_OwnAddress1 = 0X00;//从机模式的地址
	
	I2C_Init(I2C2, &I2C_InitStruct);
	//4.使能I2C
	I2C_Cmd(I2C2, ENABLE);

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

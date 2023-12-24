#ifndef __MPU6050_H
#define __MPU6050_H

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
void MPU6050_WriteMultipleData(uint8_t RegAddress, uint8_t* Data, uint8_t Length);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);
void MPU6050_ReadMultipleData(uint8_t* Data, uint8_t RegAddress, uint8_t Length);
uint8_t MPU6050_Init(void);

int16_t MPU6050_Get_ACCEL_X(void);
int16_t MPU6050_Get_ACCEL_Y(void);
int16_t MPU6050_Get_ACCEL_Z(void);
int16_t MPU6050_Get_TEMP(void);
int16_t MPU6050_Get_GYRO_X(void);
int16_t MPU6050_Get_GYRO_Y(void);
int16_t MPU6050_Get_GYRO_Z(void);


//MPU6050关键寄存器地址
enum MPU6050_Reg
{
	MPU6050_SMPLRT_DIV 		= (uint8_t)0X19,//MPU6050采样分频
	MPU6050_CONFIG 			= (uint8_t)0X1A,//MPU6050配置
	MPU6050_GYRO_CONFIG 	= (uint8_t)0X1B,//MPU6050陀螺仪配置
	MPU6050_ACCEL_CONFIG 	= (uint8_t)0X1C,//MPU6050加速度计配置
	
	MPU6050_ACCEL_XOUT_H 	= (uint8_t)0x3B,//MPU6050加速度计X轴高八位
	MPU6050_ACCEL_XOUT_L 	= (uint8_t)0x3C,//MPU6050加速度计X轴低八位
	MPU6050_ACCEL_YOUT_H 	= (uint8_t)0x3D,//MPU6050加速度计Y轴高八位
	MPU6050_ACCEL_YOUT_L 	= (uint8_t)0x3E,//MPU6050加速度计Y轴低八位
	MPU6050_ACCEL_ZOUT_H 	= (uint8_t)0x3F,//MPU6050加速度计Z轴高八位
	MPU6050_ACCEL_ZOUT_L 	= (uint8_t)0x40,//MPU6050加速度计Z轴低八位
	MPU6050_TEMP_OUT_H 		= (uint8_t)0x41,//MPU6050温度计高八位
	MPU6050_TEMP_OUT_L 		= (uint8_t)0x42,//MPU6050温度计低八位
	MPU6050_GYRO_XOUT_H 	= (uint8_t)0x43,//MPU6050陀螺仪X轴高八位
	MPU6050_GYRO_XOUT_L 	= (uint8_t)0x44,//MPU6050陀螺仪X轴低八位
	MPU6050_GYRO_YOUT_H 	= (uint8_t)0x45,//MPU6050陀螺仪Y轴高八位
	MPU6050_GYRO_YOUT_L 	= (uint8_t)0x46,//MPU6050陀螺仪Y轴低八位
	MPU6050_GYRO_ZOUT_H 	= (uint8_t)0x47,//MPU6050陀螺仪Z轴高八位
	MPU6050_GYRO_ZOUT_L 	= (uint8_t)0x48,//MPU6050陀螺仪Z轴低八位
	
	MPU6050_PWR_MGMT_1 		= (uint8_t)0x6B,//电源管理寄存器1
	MPU6050_PWR_MGMT_2 		= (uint8_t)0x6C,//电源管理寄存器2
	MPU6050_WHO_AM_I 		= (uint8_t)0x75,//I2C ID
};

#endif

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"


int16_t AX, AY, AZ, TEMP, GX, GY, GZ;

int main(void)
{
	MPU6050_Init();
	OLED_Init();//这里要把软件模拟的OLED放在硬件下面初始化
	while(1)
	{
		AX = MPU6050_Get_ACCEL_X();
		AY = MPU6050_Get_ACCEL_Y();
		AZ = MPU6050_Get_ACCEL_Z();
		TEMP = MPU6050_Get_TEMP();
		GX = MPU6050_Get_GYRO_X();
		GY = MPU6050_Get_GYRO_Y();
		GZ = MPU6050_Get_GYRO_Z();
		OLED_ShowSignedNum(1, 8, TEMP, 5);
		OLED_ShowSignedNum(2, 1, AX, 5);
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
	}
}

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"

int16_t AX, AY, AZ, TEMP, GX, GY, GZ;

int main(void)
{
	OLED_Init();
	if(MPU6050_Init() == 0)
	{
		OLED_ShowString(1, 1, "ERROR");
	}
	else
	{
		OLED_ShowString(1, 1, "OK");
	}
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

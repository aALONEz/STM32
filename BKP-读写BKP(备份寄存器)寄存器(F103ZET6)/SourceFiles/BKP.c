#include "stm32f10x.h"                  // Device header

/*
* 函数名：BKP_Init
* 功能：BKP（备份寄存器）初始化
* 参数：无
* 返回值：无
* 备注：无
*/
void BKP_Init(void)
{
	//1.使能PWR和BKP的时钟,使能RTC,PWR,BKP其中的任意一个或者多个,都需要开启PWR和BKP的时钟,RTC不在APB1总线上
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	//2.配置PWR_CR寄存器的DBP位,使能对BKP和RTC的访问,此函数在PWR头文件中声明
	PWR_BackupAccessCmd(ENABLE);
}

/*
* 函数名：BKP_WriteData
* 功能：向BKP写入数据，BKP（备份寄存器）
* 参数：BKP_DRx x可以是1-42（互联和大容量，中容量一下的是1-10）
*       Data 要写入的数据16位
* 返回值：无
* 备注：无
*/
void BKP_WriteData(uint16_t BKP_DRx, uint16_t Data)
{
	BKP_WriteBackupRegister(BKP_DRx, Data);
}

/*
* 函数名：BKP_ReadData
* 功能：读取BKP数据，BKP（备份寄存器）
* 参数：BKP_DRx x可以是1-42（互联和大容量，中容量一下的是1-10）
* 返回值：读取到的数据16位
* 备注：无
*/
uint16_t BKP_ReadData(uint16_t BKP_DRx)
{
	BKP_ReadBackupRegister(BKP_DRx);
}

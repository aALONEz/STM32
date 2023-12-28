#include "stm32f10x.h"                  // Device header

/*
* 函数名:MyFlash_Init
* 功能:MyFlash初始化
* 参数:无
* 返回值:无
* 备注:无
*/
void MyFlash_Init(void)
{
	
}

/*
* 函数名:MyFlash_ReadWord
* 功能:MyFlash指定地址读取字(32位)
* 参数:Address 指定的地址
* 返回值:uint32_t 指定地址读取的值
* 备注:无
*/
uint32_t MyFlash_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t*)Address);
}

/*
* 函数名:MyFlash_ReadHalfWord
* 功能:MyFlash指定地址读取半字(16位)
* 参数:Address 指定的地址
* 返回值:uint16_t 指定地址读取的值
* 备注:无
*/
uint16_t MyFlash_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t*)Address);
}

/*
* 函数名:MyFlash_ReadByte
* 功能:MyFlash指定地址读取字节(8位)
* 参数:Address 指定的地址
* 返回值:uint8_t 指定地址读取的值
* 备注:无
*/
uint8_t MyFlash_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t*)Address);
}

/*
* 函数名:MyFlash_EraseAllPages
* 功能:MyFlash全部擦除
* 参数:无
* 返回值:无
* 备注:无
*/
void MyFlash_EraseAllPages(void)
{
	//解锁Flash
	FLASH_Unlock();
	//Flash全擦除,库函数中没有包括加解锁的步骤,所以需要额外加解锁
	FLASH_EraseAllPages();
	//上锁Flash
	FLASH_Lock();
}

/*
* 函数名:MyFlash_ErasePage
* 功能:MyFlash页擦除
* 参数:PageAddress 指定的页头地址
* 返回值:无
* 备注:无
*/
void MyFlash_ErasePage(uint32_t PageAddress)
{
	//解锁Flash
	FLASH_Unlock();
	//Flash页擦除,库函数中没有包括加解锁的步骤,所以需要额外加解锁
	FLASH_ErasePage(PageAddress);
	//上锁Flash
	FLASH_Lock();
}

/*
* 函数名:MyFlash_ProgramWord
* 功能:MyFlash页编程指定地址写入字,32位
* 参数:Address 指定的地址    Data 要写入的数据
* 返回值:无
* 备注:无
*/
void MyFlash_ProgramWord(uint32_t Address, uint32_t Data)
{
	//解锁Flash
	FLASH_Unlock();
	//调用FLASH_ProgramWord函数,写入数据,切记在写入数据之前这个地址空间要是擦除后未写入任何数据的状态,或者你写入的数据是0X00000000
	FLASH_ProgramWord(Address, Data);
	//上锁Flash
	FLASH_Lock();
}

/*
* 函数名:MyFlash_ProgramHalfWord
* 功能:MyFlash页编程指定地址写入字,16位
* 参数:Address 指定的地址    Data 要写入的数据
* 返回值:无
* 备注:无
*/
void MyFlash_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
	//解锁Flash
	FLASH_Unlock();
	//调用FLASH_ProgramWord函数,写入数据,切记在写入数据之前这个地址空间要是擦除后未写入任何数据的状态,或者你写入的数据是0X0000
	FLASH_ProgramHalfWord(Address, Data);
	//上锁Flash
	FLASH_Lock();
}

//不可以页编程字节,也就是不可以单独写入字节

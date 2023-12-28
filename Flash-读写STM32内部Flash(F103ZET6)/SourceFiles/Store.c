#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"

/*最后一页的页头地址*/
#define EndPageAddress 0x0807F800
/*单页大小,单位字节*/
#define EndPageSize 2048

/*
* 最后一页的缓冲数组
*/
uint16_t Store_Data[EndPageSize / 2];

/*
* 函数名:Store_Init
* 功能:Store初始化,先判断系统是不是第一次使用,如果是那就擦除最后一页并且设置第一个半字是标志位,然后将除标志位半字以外所有的值清除为0
* 参数:无
* 返回值:无
* 备注:仅适用于STM32F103大容量,其他产品请在上面修改内置Flash最后一页的页头地址
*/
void Store_Init(void)
{
	if(MyFlash_ReadHalfWord(EndPageAddress) != 0X4D4D)//判断是不是第一次使用
	{
		//如果是第一次使用那么就先擦除最后一页
		MyFlash_ErasePage(EndPageAddress);
		MyFlash_ProgramHalfWord(EndPageAddress, 0X4D4D);
		for(uint32_t i = 1; i < (sizeof(Store_Data) / sizeof(Store_Data[0])); i++)//从1开始是因为前面半字是标志位
		{
			MyFlash_ProgramHalfWord(EndPageAddress + i * 2, 0X0000);//每次是写入16位,两个字节,所以这里地址要每次跳过2
		}
	}
	for(uint32_t i = 0; i < (sizeof(Store_Data) / sizeof(Store_Data[0])); i++)//从1开始是因为前面半字是标志位
	{
		Store_Data[i] = MyFlash_ReadHalfWord(EndPageAddress + i * 2);//将Flash里面的内容保存到临时数组里面
	}
}

/*
* 函数名:Store_Save
* 功能:Store缓冲器保存到内置Flash
* 参数:无
* 返回值:无
* 备注:无
*/
void Store_Save(void)
{
	//页擦除
	MyFlash_ErasePage(EndPageAddress);
	for(uint32_t i = 0; i < (sizeof(Store_Data) / sizeof(Store_Data[0])); i++)//从1开始是因为前面半字是标志位
	{
		MyFlash_ProgramHalfWord(EndPageAddress + i * 2, Store_Data[i]);//将临时数组保存到Flash里面
	}
}

/*
* 函数名:Store_WriteByte
* 功能:Store指定地址写一个字节,地址范围是(EndPageAddress + 2) ~ (EndPageAddress + EndPageSize)
* 参数:Address 指定的内置Flash地址    Data 要写入的数据(8位)
* 返回值:无
* 备注:无
*/
void Store_WriteByte(uint32_t Address, uint8_t Data)
{
	if((Address - EndPageAddress <= 1) || (Address > (EndPageAddress + EndPageSize)))//禁止写入标志位,以及禁止越界写入
	{
		/*可以返回错误信息,或者弹出警告之类的*/
		return;
	}
	//使用指针来操作写入,使用指针强转
	uint8_t* TempStore_Data = (uint8_t*)Store_Data;
	*(TempStore_Data + (Address - EndPageAddress)) = Data;
}

/*
* 函数名:Store_WriteHalfWord
* 功能:Store指定地址写一个半字,地址范围是(EndPageAddress + 2) ~ (EndPageAddress + EndPageSize)
* 参数:Address 指定的内置Flash地址    Data 要写入的数据(16位)
* 返回值:无
* 备注:无
*/
void Store_WriteHalfWord(uint32_t Address, uint16_t Data)
{
	if((Address - EndPageAddress <= 1) || (Address > (EndPageAddress + EndPageSize - 2)))//禁止写入标志位,以及禁止越界写入
	{
		/*可以返回错误信息,或者弹出警告之类的*/
		return;
	}
	/*此处STM32是小端字节序,所以低位放在低地址处*/
	//使用指针来操作写入,使用指针强转
	uint8_t* TempStore_Data = (uint8_t*)Store_Data;
	*(TempStore_Data + (Address - EndPageAddress + 1)) = Data >> 8;
	*(TempStore_Data + (Address - EndPageAddress)) = Data;
}

/*
* 函数名:Store_WriteWord
* 功能:Store指定地址写一个字,地址范围是(EndPageAddress + 2) ~ (EndPageAddress + EndPageSize)
* 参数:Address 指定的内置Flash地址    Data 要写入的数据(32位)
* 返回值:无
* 备注:无
*/
void Store_WriteWord(uint32_t Address, uint32_t Data)
{
	if((Address - EndPageAddress <= 1) || (Address > (EndPageAddress + EndPageSize - 4)))//禁止写入标志位,以及禁止越界写入
	{
		/*可以返回错误信息,或者弹出警告之类的*/
		return;
	}
	/*此处STM32是小端字节序,所以低位放在低地址处*/
	//使用指针来操作写入,使用指针强转
	uint8_t* TempStore_Data = (uint8_t*)Store_Data;
	*(TempStore_Data + (Address - EndPageAddress + 3)) = Data >> 24;
	*(TempStore_Data + (Address - EndPageAddress + 2)) = Data >> 16;
	*(TempStore_Data + (Address - EndPageAddress + 1)) = Data >> 8;
	*(TempStore_Data + (Address - EndPageAddress + 0)) = Data;
}

/*
* 函数名:Store_Clear
* 功能:Store清除所有数据,不清除标志位
* 参数:无
* 返回值:无
* 备注:无
*/
void Store_Clear(void)
{
	for(uint32_t i = 1; i < (sizeof(Store_Data) / sizeof(Store_Data[0])); i++)//从1开始是因为前面半字是标志位
	{
		Store_Data[i] = 0X0000;
	}
	Store_Save();
}

/*
* 函数名:Store_ClearFlag
* 功能:Store清除标志位,注意一旦清除标志位,那么下次开机将会清除所有保存的数据
* 参数:无
* 返回值:无
* 备注:无
*/
void Store_ClearFlag(void)
{
	Store_Data[0] = 0X0000;
	Store_Save();
}


/*
* 函数名:Store_ReadByte
* 功能:Store指定地址读取一个字节
* 参数:Address 指定的地址
* 返回值:uint8_t 读取到的值
* 备注:无
*/
uint8_t Store_ReadByte(uint32_t Address)
{
	if((Address > (EndPageAddress + EndPageSize)))//禁止越界访问
	{
		/*可以返回错误信息,或者弹出警告之类的*/
		return 0;
	}
	//使用指针来操作读取,使用指针强转
	uint8_t* TempStore_Data = (uint8_t*)Store_Data;
	return TempStore_Data[Address - EndPageAddress];
}

/*
* 函数名:Store_ReadHalfWord
* 功能:Store指定地址读取一个半字
* 参数:Address 指定的地址
* 返回值:uint16_t 读取到的值
* 备注:无
*/
uint16_t Store_ReadHalfWord(uint32_t Address)
{
	if((Address > (EndPageAddress + EndPageSize - 2)))//禁止越界访问
	{
		/*可以返回错误信息,或者弹出警告之类的*/
		return 0;
	}
	uint16_t RetData = 0X0000;
	/*此处STM32是小端字节序,所以低位放在低地址处*/
	//使用指针来操作读取,使用指针强转
	uint8_t* TempStore_Data = (uint8_t*)Store_Data;
	RetData = *(TempStore_Data + (Address - EndPageAddress + 1));
	RetData <<= 8;
	RetData |= *(TempStore_Data + (Address - EndPageAddress));
	return RetData;
}

/*
* 函数名:Store_ReadWord
* 功能:Store指定地址读取一个字
* 参数:Address 指定的地址
* 返回值:uint32_t 读取到的值
* 备注:无
*/
uint32_t Store_ReadWord(uint32_t Address)
{
	if((Address > (EndPageAddress + EndPageSize - 4)))//禁止越界访问
	{
		/*可以返回错误信息,或者弹出警告之类的*/
		return 0;
	}
	uint32_t RetData = 0X00000000;
	/*此处STM32是小端字节序,所以低位放在低地址处*/
	//使用指针来操作读取,使用指针强转
	uint8_t* TempStore_Data = (uint8_t*)Store_Data;
	RetData |= ((uint32_t)(*(TempStore_Data + (Address - EndPageAddress + 3)))) << 24;
	RetData |= ((uint32_t)(*(TempStore_Data + (Address - EndPageAddress + 2)))) << 16;
	RetData |= ((uint32_t)(*(TempStore_Data + (Address - EndPageAddress + 1)))) << 8;
	RetData |= ((uint32_t)(*(TempStore_Data + (Address - EndPageAddress + 0))));
	return RetData;
}

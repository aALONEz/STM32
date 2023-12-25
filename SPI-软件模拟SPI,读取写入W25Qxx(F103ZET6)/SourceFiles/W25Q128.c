#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q128.h"

/*
* 函数名:W25Q128_Init
* 功能:W25Q128初始化
* 参数:无
* 返回值:无
* 备注:无
*/
void W25Q128_Init(void)
{
	MySPI_Init();
}

/*
* 函数名:W25Q128_WriteEnable
* 功能:W25Q128写入使能
* 参数:无
* 返回值:无
* 备注:无
*/
void W25Q128_WriteEnable(void)
{
	//发送起始位
	MySPI_Start();
	//交换一个字节数据
	MySPI_SwapByte(W25Q128_WRITE_ENABLE);
	//发送停止位
	MySPI_Stop();
}

/*
* 函数名:W25Q128_Get_MID_DID
* 功能:W25Q128获取厂商ID和设备ID
* 参数:MID 接收厂商ID变量的指针
*      DID 接收设备ID变量的指针
* 返回值:无
* 备注:无
*/
void W25Q128_Get_MID_DID(uint8_t* MID, uint16_t* DID)
{
	//发送起始位
	MySPI_Start();
	//交换一个字节数据,发送指令
	MySPI_SwapByte(W25Q128_JEDEC_ID);
	//交换一个字节数据,接收厂商ID
	(*MID) = MySPI_SwapByte(W25Q128_DUMMY_BYTE);
	//交换一个字节数据,接收设备ID高8位
	(*DID) = MySPI_SwapByte(W25Q128_DUMMY_BYTE);
	//左移8位,给剩下的低八位留位置
	(*DID) <<= 8;
	//交换一个字节数据,接收设备ID低8位
	(*DID) |= MySPI_SwapByte(W25Q128_DUMMY_BYTE);
	//发送停止位
	MySPI_Stop();
}

/*
* 函数名:W25Q128_WaitBusy
* 功能:W25Q128芯片忙状态等待
* 参数:无
* 返回值:无
* 备注:无
*/
void W25Q128_WaitBusy(void)
{
	int TimeOut = 100000;
	//发送起始位
	MySPI_Start();
	//交换一个字节数据,发送指令
	MySPI_SwapByte(W25Q128_READ_STATUS_REGISTER_1);
	while((MySPI_SwapByte(W25Q128_DUMMY_BYTE) & 0X01))
	{
		TimeOut--;
		if(TimeOut <= 0)
		{
			break;
		}
	}
	//发送停止位
	MySPI_Stop();
}

/*
* 函数名:W25Q128_PageProgram
* 功能:W25Q128页编程,页写入
* 参数:Address 要指定从那个地址开始写
*      DataArray 存放写入数据的数组指针
*      Count 写入几个字节,小于等于数组的长度
* 返回值:无
* 备注:多写入256个字节,这里使用uint8_t 限制为最多写入255个字节,即使填入的地址是页尾或者写入的字节个数会超过页尾,会将本页写完后自动跳到下一页的起始位置继续写入
*/
void W25Q128_PageProgram(uint32_t Address, uint8_t* DataArray, uint8_t Count)//最多写入256个字节,这里使用uint8_t 限制为最多写入255个字节
{
	//检测忙等待
	W25Q128_WaitBusy();
	uint8_t i = 0;
	//写入使能
	W25Q128_WriteEnable();
	//发送起始位
	MySPI_Start();
	//交换一个字节数据,发送指令
	MySPI_SwapByte(W25Q128_PAGE_PROGRAM);
	//交换一个字节数据,发送地址高8位
	MySPI_SwapByte(Address >> 16);
	//交换一个字节数据,发送地址中8位
	MySPI_SwapByte(Address >> 8);
	//交换一个字节数据,发送地址低8位
	MySPI_SwapByte(Address);
	//连续交换一个字节数据,发送要写入的数据
	for(i = 0; i < Count; i++)
	{
		//下面是对于,写入的长度已经超过了页尾的处理方法
		uint32_t temp = (Address + i) & 0X000000FF;//拿到十六进制最低的两位的数据
		MySPI_SwapByte(*(DataArray + i));
		if((0XFF - temp) == 0)//如果最低的两位数据是FF，那么就说明是页尾了
		{
			break;//跳出循环
		}
	}
	//发送停止位
	MySPI_Stop();
	if(i < Count)//当没有遇到页尾此时i是等于Count的，所以这里判断i小于Count就是遇到页尾跳出循环了，就需要再次调用这个函数，传递现在的地址+1（FF的地址已经写入），传入当前数组（DataArray + i）加1（FF的地址已经写入，所以数组加1），数量要减i，因为有i个已经写入，而且还要减1，因为FF这个地址已经写入
	{
		W25Q128_PageProgram((Address + i + 1), (DataArray + i + 1), (Count - i - 1));
	}
}

/*
* 函数名:W25Q128_SectorErase
* 功能:W25Q128扇区擦除
* 参数:Address 要擦除的扇区地址,可以填写任意段的扇区地址,会自动擦除所在扇区
* 返回值:无
* 备注:无
*/
void W25Q128_SectorErase(uint32_t Address)
{
	Address &= ~(0X00000FFF);//非扇区头地址,处理方法
	//检测忙等待
	W25Q128_WaitBusy();
	//写入使能
	W25Q128_WriteEnable();
	//发送起始位
	MySPI_Start();
	//交换一个字节数据,发送指令
	MySPI_SwapByte(W25Q128_SECTOR_ERASE_4KB);
	//交换一个字节数据,发送地址高8位
	MySPI_SwapByte(Address >> 16);
	//交换一个字节数据,发送地址中8位
	MySPI_SwapByte(Address >> 8);
	//交换一个字节数据,发送地址低8位
	MySPI_SwapByte(Address);
	//发送停止位
	MySPI_Stop();
}

/*
* 函数名:W25Q128_ReadData
* 功能:W25Q128读取数据
* 参数:Address 要擦除的起始地址
*      DataArray 接收数据的数组指针
*      Count 要接收多少个字节,小于等于数组长度
* 返回值:无
* 备注:无
*/
void W25Q128_ReadData(uint32_t Address, uint8_t* DataArray, uint32_t Count)//读取不受256字节限制
{
	//检测忙等待,忙时读取也不行
	W25Q128_WaitBusy();
	//发送起始位
	MySPI_Start();
	//交换一个字节数据,发送指令
	MySPI_SwapByte(W25Q128_READ_DATA);
	//交换一个字节数据,发送地址高8位
	MySPI_SwapByte(Address >> 16);
	//交换一个字节数据,发送地址中8位
	MySPI_SwapByte(Address >> 8);
	//交换一个字节数据,发送地址低8位
	MySPI_SwapByte(Address);
	//循环接收数据
	for(uint32_t i = 0; i < Count; i++)
	{
		*(DataArray + i) = MySPI_SwapByte(W25Q128_DUMMY_BYTE);
	}
	//发送停止位
	MySPI_Stop();
}

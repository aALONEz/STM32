#include "stm32f10x.h"                  // Device header
#include "SerialPort.h"//串口头文件
#include "OLED.h"

int main(void)
{
	OLED_Init();
	uint8_t DataArray[4] = {0X41, 0X42, 0X43, 0X44};
	uint16_t Length = sizeof(DataArray) / sizeof(DataArray[0]);
	
	SerialPort_Init();
	SerialPort_SendByte(0X42);
	SerialPort_SendArray(DataArray, Length);
	SerialPort_SendString("HelloWorld!\n");
	SerialPort_SendNumber(00);
	SerialPort_SendString("\n");
	printf("Number = %d\n", 666);
	
	char TempString[100] = {0};//数组要尽量足够大
	sprintf(TempString, "Number = %d\n", 777);
	SerialPort_SendString(TempString);
	
	SerialPort_Printf("Number = %d\n", 888);
	
	SerialPort_Printf("你好世界!");
	while(1)
	{
		if(SerialPort_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(1, 1, SerialPort_GetRxData(), 2);
		}
	}
}

#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "SerialPort.h"

int main(void)
{
	OLED_Init();
	SerialPort_Init();
	
	OLED_ShowString(3, 1, "RxData:");
	OLED_ShowString(1, 1, "TxData:");
	
	SerialPort_TxPacket[0] = 0XF1;
	SerialPort_TxPacket[1] = 0XF2;
	SerialPort_TxPacket[2] = 0XF3;
	SerialPort_TxPacket[3] = 0XF4;
	
	SerialPort_SendPacket();
	
	OLED_ShowHexNum(2, 1, SerialPort_TxPacket[0], 2);
	OLED_ShowHexNum(2, 4, SerialPort_TxPacket[1], 2);
	OLED_ShowHexNum(2, 7, SerialPort_TxPacket[2], 2);
	OLED_ShowHexNum(2, 10, SerialPort_TxPacket[3], 2);
	while(1)
	{
		if(SerialPort_GetRxFlag() == 1)
		{
			OLED_ShowHexNum(4, 1, SerialPort_RxPacket[0], 2);
			OLED_ShowHexNum(4, 4, SerialPort_RxPacket[1], 2);
			OLED_ShowHexNum(4, 7, SerialPort_RxPacket[2], 2);
			OLED_ShowHexNum(4, 10, SerialPort_RxPacket[3], 2);
		}
	}
}

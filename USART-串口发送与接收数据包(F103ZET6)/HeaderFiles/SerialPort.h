#ifndef __SERIAL_PORT_H
#define __SERIAL_PORT_H

extern uint8_t SerialPort_RxPacket[4];//接收
extern uint8_t SerialPort_TxPacket[4];//发送

void SerialPort_SendByte(uint8_t Data);
void SerialPort_SendPacket(void);
void SerialPort_Init(void);
void SerialPort_SendArray(uint8_t* DataArray, uint16_t Length);
void SerialPort_SendString(char* Data);
void SerialPort_SendNumber(uint32_t Number);
void SerialPort_Printf(char* format, ...);
uint8_t SerialPort_GetRxFlag(void);


#endif

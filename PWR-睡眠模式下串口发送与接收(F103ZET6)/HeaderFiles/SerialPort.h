#ifndef __SERIALPORT_H
#define __SERIALPORT_H

#include <stdio.h>

void SerialPort_Init(void);
void SerialPort_SendByte(uint8_t Data);
void SerialPort_SendArray(uint8_t* DataArray, uint16_t Length);
void SerialPort_SendString(char* Data);
void SerialPort_SendNumber(uint32_t Number);
void SerialPort_Printf(char* format, ...);
uint8_t SerialPort_GetRxData(void);
uint8_t SerialPort_GetRxFlag(void);

#endif

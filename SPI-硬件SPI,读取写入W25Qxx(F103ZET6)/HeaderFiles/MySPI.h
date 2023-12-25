#ifndef __MY_SPI_H
#define __MY_SPI_H

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint8_t MySPI_SwapByte(uint8_t SendData);

#endif

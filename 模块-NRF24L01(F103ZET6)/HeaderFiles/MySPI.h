#ifndef __MY_SPI_H
#define __MY_SPI_H

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint16_t MySPI_SwapByte(uint16_t SendData);

#endif

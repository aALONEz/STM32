#ifndef __MY_SPI_H
#define __MY_SPI_H

//CS引脚为PG7，SCK引脚为PB13，MISO引脚为PB14，MOSI引脚为PB15
//CS
#define CS_RCC 			RCC_APB2Periph_GPIOG
#define CS_GPIO 		GPIOG
#define CS_Pin 			GPIO_Pin_7
//SCK
#define SCK_RCC 		RCC_APB2Periph_GPIOB
#define SCK_GPIO 		GPIOB
#define SCK_Pin 		GPIO_Pin_13
//MISO
#define MISO_RCC 		RCC_APB2Periph_GPIOB
#define MISO_GPIO 		GPIOB
#define MISO_Pin 		GPIO_Pin_14
//MOSI
#define MOSI_RCC 		RCC_APB2Periph_GPIOB
#define MOSI_GPIO 		GPIOB
#define MOSI_Pin 		GPIO_Pin_15

void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);
uint16_t MySPI_SwapByte(uint16_t SendData);

#endif

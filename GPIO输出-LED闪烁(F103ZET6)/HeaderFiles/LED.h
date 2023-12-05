#ifndef __LED_H
#define __LED_H

#define ON 	Bit_RESET
#define OFF Bit_SET

#define LED_Switch(x) GPIO_WriteBit(GPIOB, GPIO_Pin_5, x)//使用宏定义封装一个LED状态切换

void LED_Init(void);

#endif

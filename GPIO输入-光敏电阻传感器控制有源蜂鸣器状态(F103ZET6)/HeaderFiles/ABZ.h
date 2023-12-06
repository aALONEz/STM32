#ifndef __ABZ_H
#define __ABZ_H

//4.蜂鸣器状态切换
#define OFF Bit_RESET
#define ON 	Bit_SET
#define ABZ_Switch(x) GPIO_WriteBit(GPIOB, GPIO_Pin_8, x)

void ABZ_Init(void);

#endif

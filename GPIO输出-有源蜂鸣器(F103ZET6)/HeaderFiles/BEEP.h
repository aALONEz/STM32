#ifndef __BEEP_H
#define __BEEP_H

//宏定义开关状态
#define ON 	Bit_SET
#define OFF Bit_RESET

//宏定义GPIO_WriteBit,使用BEEP_Switch()传递上面开关状态控制BEEP
#define BEEP_Switch(x) GPIO_WriteBit(GPIOB, GPIO_Pin_8, x)

/*
* 初始化有源蜂鸣器,对应引脚PB8
*/
void BEEP_Init(void);

#endif

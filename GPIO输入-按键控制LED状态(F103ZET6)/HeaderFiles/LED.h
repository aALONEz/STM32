#ifndef __LED_H
#define __LED_H

/*
* 调用LED_Switch()传递ON表示打开LED,传递OFF表示关闭LED,记得带有';'举例:LED_Switch(ON);  打开LED
*/
#define ON Bit_RESET
#define OFF Bit_SET
#define LED_Switch(x) GPIO_WriteBit(GPIOB, GPIO_Pin_5, x)

void LED_Init(void);
uint8_t Get_LED_State(void);


#endif

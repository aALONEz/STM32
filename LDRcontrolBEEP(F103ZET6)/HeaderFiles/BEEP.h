#ifndef __BEEP_H
#define __BEEP_H

#define beep_rcc 		RCC_APB2Periph_GPIOA//beep(有源蜂鸣器)对应的GPIO组时钟
#define beep_gpio 		GPIOA//beep(有源蜂鸣器)对应的GPIO组
#define beep_gpio_pin 	GPIO_Pin_5//beep(有源蜂鸣器)对应的引脚
#define beep_on 		Bit_SET//beep(有源蜂鸣器)鸣叫时的电平高低    Bit_SET高电平
#define beep_off 		Bit_RESET//beep(有源蜂鸣器)鸣叫时的电平高低  Bit_RESET低电平

/*
* 功能:beep(有源蜂鸣器)初始化,默认初始化为低电平
* 参数: 空 修改beep对应的引脚和电气属性请修改上方的宏定义
* 返回值:空
*/
void beep_init(void);

/*
* 功能:设置beep(有源蜂鸣器)的电气属性,鸣叫还是不鸣叫
* 参数: BitVal 两个取值范围 beep_on 蜂鸣器鸣叫    beep_off蜂鸣器不鸣叫
* 返回值:空
*/
void set_beep(BitAction BitVal);


#endif

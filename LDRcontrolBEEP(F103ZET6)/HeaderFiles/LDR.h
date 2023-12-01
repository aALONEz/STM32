#ifndef __LDR_H
#define __LDR_H

#define ldr_rcc 						RCC_APB2Periph_GPIOA//光敏电阻传感器模块 链接的GPIO组时钟
#define ldr_gpio 						GPIOA//光敏电阻传感器模块 链接的GPIO组
#define ldr_gpio_pin_digital 			GPIO_Pin_7//光敏电阻传感器模块 数字引脚
#define ldr_gpio_pin_analog 			GPIO_Pin_1//光敏电阻传感器模块 模拟引脚

/*
* 功能:光敏电阻传感器模块初始化,默认为浮空输入
* 参数: 空 修改光敏电阻传感器模块对应的引脚和电气属性请修改上方的宏定义
* 返回值:空
*/
void ldr_init(void);

/*
* 功能:获得光敏电阻传感器模块数字接口电平高低
* 参数: 空 修改光敏电阻传感器模块对应的引脚和电气属性请修改上方的宏定义
* 返回值:Bit_SET 高电平
* 		 Bit_RESET 低电平
*/
uint8_t get_ldr_digital(void);

#endif

#ifndef __LED_H
#define __LED_H

//LED初始化函数
/*
* 功能:初始化LED,初始化之后LED对应引脚默认被设置为高电平
* 参数: RCC_APB2Periph LED链接的GPIO组外设时钟
*		GPIO_TypeDef LED链接的GPIO组
*		LED_GPIO_Pin LED链接的GPIO引脚
* 返回值: 空
*/
void LED_init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t LED_GPIO_Pin);

/*
* 功能:设置LED对应引脚电平状态
* 参数: GPIOx LED链接的GPIO组外设时钟
*		GPIO_TypeDef LED链接的GPIO组
*		LED_GPIO_Pin LED链接的GPIO引脚
* 返回值: 空
*/
void set_LED(GPIO_TypeDef* GPIOx, uint16_t LED_GPIO_Pin, BitAction BitVal);


#endif

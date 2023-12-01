#ifndef __KEY_H
#define __KEY_H

/*
* 功能:KEY初始化,初始化为默认上拉输入
* 参数: RCC_APB2Periph KEY对应的GPIO组时钟
*		GPIOx KEY连接的对应GPIO组
*		GPIO_Pin_x KEY连接的GPIO引脚
* 返回值:空
*/
void KEY_init(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);

/*
* 功能:获取按键状态,0为没有按下 1为按下
* 参数: GPIOx KEY连接的对应GPIO组
*		GPIO_Pin_x KEY连接的GPIO引脚
* 返回值:按键当前状态0为没有按下 1为按下
*/
uint8_t get_KEY(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_x);



#endif

#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H

/*请在main.c文件内定义中断函数*/
/*该模块代码是以中断方式初始化的*/

#define countsensor_rcc 				RCC_APB2Periph_GPIOA//对射式红外传感器,链接引脚的对应GPIO组时钟
#define countsensor_gpio 				GPIOA//对射式红外传感器,链接引脚的对应GPIO组
#define countsensor_gpio_pin_digital 	GPIO_Pin_1////对射式红外传感器,数字引脚链接引脚的对应GPIO引脚
#define countsensor_gpio_pin_analog 	GPIO_Pin_7////对射式红外传感器,模拟引脚链接引脚的对应GPIO引脚
#define countsensor_gpio_exti 			GPIO_PortSourceGPIOA//中断对应GPIO组,一般是和对应接收信号的引脚保持一致
#define countsensor_gpio_pin_exti 		GPIO_PinSource1//中断对应GPIO引脚,一般是和对应接收信号的引脚保持一致
#define countsensor_exti_line 			EXTI_Line1//要配置的中断通道与Pin引脚编号对应
#define countsensor_exti_mode 			EXTI_Mode_Interrupt////中断响应方式,中断响应(EXTI_Mode_Interrupt)和事件响应(EXTI_Mode_Event).默认中断响应
#define countsensor_exti_trigger 		EXTI_Trigger_Falling//中断触发方式,上升沿触发(EXTI_Trigger_Rising),下降沿触发(EXTI_Trigger_Falling),双边沿触发(EXTI_Trigger_Rising_Falling).默认下降沿触发
#define countsensor_nvic_irq 			EXTI1_IRQn//nvic中断线路选择,与引脚相同
#define countsensor_nvic_irq_pp 		1//抢占优先级请搜索NVIC_Priority_Table参考这个表
#define countsensor_nvic_irq_sp 		1//响应优先级请搜索NVIC_Priority_Table参考这个表

/*
* 功能：对射式红外传感器初始化，默认会浮空输入，如需修改对应引脚请修改上面宏定义,使用前请记得主函数调用NVIC分组进行分组
* 参数：无
* 返回值：无
*/
void countsensor_init(void);

/*
* 功能：对射式红外传感器获取数字信号
* 参数：无
* 返回值：对应电平状态0表示低电平,1表示高电平
*/
uint8_t countsensor_get_digital(void);


#endif

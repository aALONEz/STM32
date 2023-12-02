#ifndef __ROTARY_ENCODER_H
#define __ROTARY_ENCODER_H

#define rotary_encoder_rcc 				RCC_APB2Periph_GPIOA
#define rotary_encoder_gpio 			GPIOA
#define rotary_encoder_gpio_pin_a 		GPIO_Pin_7
#define rotary_encoder_gpio_pin_b 		GPIO_Pin_5
#define rotary_encoder_afio_gpio 		GPIO_PortSourceGPIOA
#define rotary_encoder_afio_gpio_pin_a 	GPIO_PinSource7
#define rotary_encoder_afio_gpio_pin_b 	GPIO_PinSource7
#define rotary_encoder_exti_line_a 		EXTI_Line7//一般对应连接模块A引脚的引脚号
#define rotary_encoder_exti_line_b 		EXTI_Line5//一般对应连接模块B引脚的引脚号
#define rotary_encoder_exti_mode 		EXTI_Mode_Interrupt//中断响应模式,中断函数/事件响应,默认中断函数
#define rotary_encoder_exti_trigger 	EXTI_Trigger_Falling//中断触发方式,默认下降沿触发
#define rotary_encoder_nvic_irq 		EXTI9_5_IRQn//中断通道
#define rotary_encoder_nvic_irq_pp 		1//抢占优先级
#define rotary_encoder_nvic_irq_sp 		2//响应优先级

/*
* 功能：初始化旋转编码器，需要修改对应引脚和模式请在上面宏定义修改，没有要修改的也可以去源文件修改
* 参数：无
* 返回值：无
*/
void rotary_encoder_init(void);

#endif

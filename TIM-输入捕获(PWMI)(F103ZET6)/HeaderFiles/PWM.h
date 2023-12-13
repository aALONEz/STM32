#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void);
void PWM_SetPulse(uint16_t Pulse);
void PWM_SetPeriod(uint16_t Period);
void PWM_SetPrescaler(uint16_t Prescaler, uint16_t TIM_PSCReloadMode);

#endif

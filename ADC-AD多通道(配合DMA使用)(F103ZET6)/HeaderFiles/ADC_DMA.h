#ifndef __ADC_DMA_H
#define __ADC_DMA_H

void ADC_DMA_Init(uint32_t PeripheralBaseAddr, uint32_t MemoryBaseAddr, uint16_t Number);
void Set_ADC_DMA_DataCounter(void ADC_SoftwareStartConvCmd(ADC_TypeDef*, FunctionalState), uint16_t Number);
#endif

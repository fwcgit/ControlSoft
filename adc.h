#ifndef _ADC_H
#define _ADC_H
#include "stm32f10x.h"
void Config_GPIO_adc1(void);
void Conifg_ADC_DMA(void);
void Config_ADC_MODE(void);
void Start_Convet_ADC(void);
void display_value(void);
#endif


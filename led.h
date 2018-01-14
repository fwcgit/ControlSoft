#ifndef _LED_H_
#define _LED_H_
#include<stm32f10x.h>
#define OFF 1
#define ON  0
#define GPIOD_Pin_2(a) if(a) GPIOD->BSRR = 1 << 2; else GPIOD->BRR = 1 << 2
#define GPIOA_Pin_8(a) if(a) GPIOA->BSRR = 1 << 8; else GPIOA->BRR = 1 << 8
void GPIO_Led_Init(void);
#endif

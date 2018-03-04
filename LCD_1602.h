#ifndef _LCD_1602_H
#define _LCD_1602_H
#include<stm32f10x.h>

#define LCD_RS(a) if(a) GPIOB->BSRR = 1 <<6 ; else GPIOB->BRR = 1<<6
#define LCD_RW(a) if(a) GPIOB->BSRR = 1 <<8 ; else GPIOB->BRR = 1<<8
#define LCD_E(a) if(a) 	GPIOB->BSRR = 1 <<7 ; else GPIOB->BRR = 1<<7
	
void Config_GPIO_LCD(void);
void init_LCD(void);
void wirte_data_LCD(u8 data);
void wirte_cmd_LCD(u8 cmd);
void display_LCD(u8 cmd,u8 data);
#endif

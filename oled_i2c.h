#ifndef _OLED_I2C_H
#define _OLED_I2C_H
#include<stm32f10x.h>

void OLED_Config(void);
void OLED_Init(void);
void OLED_Write_cmd(u8 cmd);
void OLED_Write_data(u8 data);
void I2C_Write_Byte(uint8_t addr,uint8_t data);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_Fill(unsigned char fill_Data);//È«ÆÁÌî³ä
void OLED_XY(u8 x,u8 y);
#endif

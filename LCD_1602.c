#include "LCD_1602.h"
#include "delay.h"

void Config_GPIO_LCD(void)
{
	GPIO_InitTypeDef GPIO_Init_Type_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_Init_Type_Structure.GPIO_Pin 		= GPIO_Pin_0  |  GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init_Type_Structure.GPIO_Mode 		= GPIO_Mode_Out_PP;
	GPIO_Init_Type_Structure.GPIO_Speed  	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
	
	GPIO_Init_Type_Structure.GPIO_Pin 		= GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_Init(GPIOB,&GPIO_Init_Type_Structure);
}

void wirte_data_LCD(u8 data)
{
	LCD_RS(1);
	LCD_RW(0);
	GPIOA->ODR = data;
	delay_ms(5);
	LCD_E(1);
	LCD_E(0);
}

void wirte_cmd_LCD(u8 cmd)
{
	LCD_RS(0);
	LCD_RW(0);
	GPIOA->ODR = cmd;
	delay_ms(5);
	LCD_E(1);
	LCD_E(0);
}

void display_LCD(u8 cmd,u8 data)
{
	wirte_cmd_LCD(cmd);
	wirte_data_LCD(data);
}

void init_LCD(void)
{

	delay_ms(10);
	
	wirte_cmd_LCD(0x38);
	wirte_cmd_LCD(0x0f);
	wirte_cmd_LCD(0x06);
	wirte_cmd_LCD(0x01);
	wirte_cmd_LCD(0x80+0x10);
	
	wirte_cmd_LCD(0x80);
	wirte_data_LCD('T');
	wirte_cmd_LCD(0x81);
	wirte_data_LCD('e');
	wirte_cmd_LCD(0x82);
	wirte_data_LCD('m');
	wirte_cmd_LCD(0x83);
	wirte_data_LCD('p');
	wirte_cmd_LCD(0x84);
	wirte_data_LCD(':');
}

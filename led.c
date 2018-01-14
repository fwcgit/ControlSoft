#include"led.h"

void GPIO_Led_Init(void)
{
	
	GPIO_InitTypeDef GPIO_Init_Type_Structure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD,ENABLE);
	
	GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init_Type_Structure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_Init_Type_Structure.GPIO_Speed= GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
	
	GPIO_Init_Type_Structure.GPIO_Pin = GPIO_Pin_2;
	
	GPIO_Init(GPIOD,&GPIO_Init_Type_Structure);

}

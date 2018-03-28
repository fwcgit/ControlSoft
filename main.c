#include<stm32f10x.h>
#include<stdio.h>
#include"delay.h"
#include"led.h"
#include"usart.h"
#include "LCD_1602.h"
#include "adc.h"
#include "oled_i2c.h"
#include<math.h>

u8 fwc[] = 
{
0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00,//?0
0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,
0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00,//?1
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//:2

};

char *tips = "system to start up \r\n";
char *strTable = "0123456789";

void EXTI_PC1_CONFIG(void);
void NVIC_Configuration(void);
extern __IO uint16_t ADCConvertVale; 
u16 ADC_ConvertedValueLocal; 
void initToStr2(u16 temp);
void initToStr2(u16 temp)
{
	
	u16 w = temp /10000;
	u16 q = temp /1000%10;
	u16 b = temp /100%10;
	u16 s = temp %100/10;
	u16 g = temp %10;

	display_LCD(0x85,*(strTable+w));
	display_LCD(0x86,*(strTable+q));
	display_LCD(0x87,*(strTable+b));
	display_LCD(0x88,*(strTable+s));
	display_LCD(0x89,*(strTable+g));
	
	display_LCD(0x89,*(strTable+g));
	
}
int main(void)
{
	
	int i;
	unsigned char wm=0;
	unsigned int  adder=32*16;
	
	delay_init(72);

	GPIO_Led_Init();
		
	EXTI_PC1_CONFIG();
	
	USART1_CONFIG();
	
	OLED_Config();
	OLED_Init();
	
	//Config_GPIO_LCD();
	
	init_LCD();
	
	//Start_Convet_ADC();
	
	OLED_Fill(0x00);
	
		for(i=0;i<3;i++)
		{
				adder=32*i;
				OLED_XY(i*16 , 0);
				for(wm = 0;wm < 16;wm++)
				{
					OLED_Write_data(fwc[adder]);
					adder += 1;
				}
				OLED_XY(i*16 , 1);
				for(wm = 0;wm < 16;wm++)
				{
					OLED_Write_data(fwc[adder]);
					adder += 1;
				}
		}
	
	


	
	
	
	while(1)
	{

			//OLED_Fill(0xff);
			GPIOD_Pin_2(OFF);
			delay_ms(500);
			GPIOD_Pin_2(ON);
			//OLED_Fill(0x00);
			delay_ms(500);
			//display_value();
		//ADC_ConvertedValueLocal = ADC1->DR;
			printf("%s","sssssss");
//initToStr2(ADC_ConvertedValueLocal);
	}
}


void EXTI_PC1_CONFIG(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitTypeDef GPIO_Init_Type;
	GPIO_Init_Type.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init_Type.GPIO_Pin	 = GPIO_Pin_1;
	GPIO_Init(GPIOC,&GPIO_Init_Type);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource1);
	
	EXTI_InitTypeDef EXTI_Init_Type;
	EXTI_Init_Type.EXTI_Line 		= EXTI_Line1;
	EXTI_Init_Type.EXTI_Mode 		= EXTI_Mode_Interrupt;
	EXTI_Init_Type.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init_Type.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&EXTI_Init_Type);
	
	NVIC_Configuration();
	
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_Init_Type_Structure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_Init_Type_Structure.NVIC_IRQChannel 										= EXTI1_IRQn;
	NVIC_Init_Type_Structure.NVIC_IRQChannelPreemptionPriority 	= 0;
	NVIC_Init_Type_Structure.NVIC_IRQChannelSubPriority 				= 0;
	NVIC_Init_Type_Structure.NVIC_IRQChannelCmd									= ENABLE;
	
	NVIC_Init(&NVIC_Init_Type_Structure);
	
}

int fputc(int ch,FILE *file)
{
	
	USART_SendData(USART1,(unsigned char)ch);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
	return ch;
	
}

void EXTI1_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line1) != RESET){
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) == 0){
			
				
			GPIOA_Pin_8 (OFF);
			
		}else{
			
			GPIOA_Pin_8 (ON);
			
		}
		
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
	
}


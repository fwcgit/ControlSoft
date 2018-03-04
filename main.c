#include<stm32f10x.h>
#include<stdio.h>
#include"delay.h"
#include"led.h"
#include"usart.h"
#include "LCD_1602.h"
#include "adc.h"
#include<math.h>

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
	
	delay_init(72);

	GPIO_Led_Init();
		
	EXTI_PC1_CONFIG();
	
	USART1_CONFIG();
	
	Config_GPIO_LCD();
	
	init_LCD();
	
	Start_Convet_ADC();
	
	while(1)
	{


			GPIOD_Pin_2(OFF);
			delay_ms(500);
			GPIOD_Pin_2(ON);
			delay_ms(500);
			display_value();
		ADC_ConvertedValueLocal = ADC1->DR;
			printf("%d\r\n",ADC_ConvertedValueLocal);
			initToStr2(ADC_ConvertedValueLocal);
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


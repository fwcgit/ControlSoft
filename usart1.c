#include "usart.h"
#include "led.h"

#define MAX_BUFF 100

u8 	buff[MAX_BUFF];
u16 buff_pos = 0;
u8 	is_start = 0;
u8  is_stop  = 0;

void USART1_CONFIG(void)
{
		GPIO_InitTypeDef 	GPIO_Init_Type_Structure;
		USART_InitTypeDef USART_Init_Type_Structure;
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
	
		GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init_Type_Structure.GPIO_Pin	 = GPIO_Pin_9;
		GPIO_Init_Type_Structure.GPIO_Speed= GPIO_Speed_50MHz;
		
		GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
		GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init_Type_Structure.GPIO_Pin	 = GPIO_Pin_10;

	
		GPIO_Init(GPIOA,&GPIO_Init_Type_Structure);
	
	  USART_Init_Type_Structure.USART_BaudRate 						=115200;
		USART_Init_Type_Structure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_Init_Type_Structure.USART_Mode 								= USART_Mode_Tx | USART_Mode_Rx;
		USART_Init_Type_Structure.USART_Parity 							= USART_Parity_No;
		USART_Init_Type_Structure.USART_StopBits 						= USART_StopBits_1;
		USART_Init_Type_Structure.USART_WordLength					= USART_WordLength_8b;
		
		USART_Init(USART1,&USART_Init_Type_Structure);
		
		USART_ClearFlag(USART1,USART_FLAG_TC);
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
		
		USART_Cmd(USART1,ENABLE);
		
		NVIC_USART1_Configuration();
}


void NVIC_USART1_Configuration(void)
{
		NVIC_InitTypeDef NVIC_Init_Type_Structure;
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
		NVIC_Init_Type_Structure.NVIC_IRQChannel 										= USART1_IRQn;
		NVIC_Init_Type_Structure.NVIC_IRQChannelPreemptionPriority 	= 1;
		NVIC_Init_Type_Structure.NVIC_IRQChannelSubPriority 				= 1;
		NVIC_Init_Type_Structure.NVIC_IRQChannelCmd									= ENABLE;
	
		NVIC_Init(&NVIC_Init_Type_Structure);
}

void USART1_IRQHandler(void)
{
	u8 res;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){
		
		res = USART_ReceiveData(USART1);
		
		if(res == 0x3b){
			
			is_start = 1;
			is_stop  = 0;
			buff_pos = 0;
			
		}else if(res == 0x0d){
			is_stop = 1;
		}

		if(buff_pos >= MAX_BUFF) buff_pos = 0;		
		
		if(is_start){
			
			if(buff_pos < MAX_BUFF){
			
				buff[buff_pos++] = res;
				
			}
		}
		
		if(is_stop){
			
			is_start = 0;
			is_stop  = 0;
			
			if(buff[0] == 0x3b){
				if(buff[1] == 0x01){
						GPIOA_Pin_8(ON);
				}else{
						GPIOA_Pin_8(OFF);
				}
			}
			
		}
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}

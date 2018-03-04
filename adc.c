#include "adc.h"
#include <math.h>
#include "LCD_1602.h"
#define ADC1_DR_Address    ((u32)0x40012400+0x4c)
void initToStr(u16 temp);
u8 get_ntc_temp(u16 ntc);
u8 temp;
volatile  u16 ADCConvertVale;

void Config_GPIO_adc1(void)
{
	GPIO_InitTypeDef GPIO_Init_Type_Structure;
	RCC_APB2PeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_ADC1,ENABLE);
	
	GPIO_Init_Type_Structure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init_Type_Structure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init_Type_Structure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Init_Type_Structure);
}

void Conifg_ADC_DMA(void)
{
		DMA_InitTypeDef DMA_Init_Type_Structurn;
		DMA_DeInit(DMA1_Channel1);
		DMA_Init_Type_Structurn.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;
		DMA_Init_Type_Structurn.DMA_MemoryBaseAddr = (u32)&ADCConvertVale;
		DMA_Init_Type_Structurn.DMA_DIR = DMA_DIR_PeripheralSRC;
		DMA_Init_Type_Structurn.DMA_BufferSize = 1;
		DMA_Init_Type_Structurn.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		DMA_Init_Type_Structurn.DMA_MemoryInc= DMA_MemoryInc_Disable;
		DMA_Init_Type_Structurn.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		DMA_Init_Type_Structurn.DMA_MemoryDataSize= DMA_MemoryDataSize_HalfWord;
		DMA_Init_Type_Structurn.DMA_Mode = DMA_Mode_Circular;
		DMA_Init_Type_Structurn.DMA_Priority = DMA_Priority_High;
		DMA_Init_Type_Structurn.DMA_M2M = DMA_M2M_Disable;
		DMA_Init(DMA1_Channel1,&DMA_Init_Type_Structurn);
		DMA_Cmd(DMA1_Channel1,ENABLE);
}	

void Config_ADC_MODE(void)
{
	ADC_InitTypeDef ADC_Init_Type_Structure;
	ADC_Init_Type_Structure.ADC_Mode = ADC_Mode_Independent;
	ADC_Init_Type_Structure.ADC_ScanConvMode = DISABLE;
	ADC_Init_Type_Structure.ADC_ContinuousConvMode = ENABLE;
	ADC_Init_Type_Structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Init_Type_Structure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init_Type_Structure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1,&ADC_Init_Type_Structure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1,ADC_SampleTime_55Cycles5);
	
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

void Start_Convet_ADC(void)
{
	Config_GPIO_adc1();
	Conifg_ADC_DMA();
	Config_ADC_MODE();

}

void display_value(void)
{
		initToStr(ADCConvertVale);
}

u8 get_ntc_temp(u16 ntc)
{
	float temp;
	float t2 ;
	float ntc_r;
	float t_25;
	float ntc_ln;
	float ln ;
	float res_v;
	
	res_v = ntc * 330.0 / 3.3 -3.3;
	ntc_r = (3.3 - res_v/100.0) / (res_v/100.0/10.0);
	
	t_25 = log(10);
	ntc_ln = log(ntc_r);                                                                                                                                                                                                                                                                                            
	ln = t_25 - ntc_ln;
	
	ln = ln / 3950.0;
	t2 = 1.0/298.15;
	t2 = t2-ln;
	temp = 1.0/t2;
	temp = temp - 273.15;
	
	return (u8)temp;
}

void initToStr(u16 temp)
{
	u8 bs,ss,gs;
	temp = temp & 0xff;
	int b = temp /100;
	int s = temp %100/10;
	int g = temp / 10%10;
		switch(b)
	{
		case 0:
			bs = '0';
			break;
		case 1:
			bs = '1';
			break;
		case 2:
			bs = '2';
			break;
		case 3:
			bs = '3';
			break;
		case 4:
			bs = '4';
			break;
		case 5:
			bs = '5';
			break;
		case 6:
			bs = '6';
			break;
		case 7:
			bs = '7';
			break;
		case 8:
			bs = '8';
			break;
		case 9:
			bs = '9';
			break;
		default:
			bs = '0';
			break;
	}
	switch(s)
	{
		case 0:
			ss = '0';
			break;
		case 1:
			ss = '1';
			break;
		case 2:
			ss = '2';
			break;
		case 3:
			ss = '3';
			break;
		case 4:
			ss = '4';
			break;
		case 5:
			ss = '5';
			break;
		case 6:
			ss = '6';
			break;
		case 7:
			ss = '7';
			break;
		case 8:
			ss = '8';
			break;
		case 9:
			ss = '9';
			break;
		default:
			ss = '0';
			break;
	}
	
		switch(g)
	{
		case 0:
			gs = '0';
			break;
		case 1:
			gs = '1';
			break;
		case 2:
			gs = '2';
			break;
		case 3:
			gs = '3';
			break;
		case 4:
			gs = '4';
			break;
		case 5:
			gs = '5';
			break;
		case 6:
			gs = '6';
			break;
		case 7:
			gs = '7';
			break;
		case 8:
			gs = '8';
			break;
		case 9:
			gs = '9';
			break;
		default:
			gs = '0';
			break;
	}

	display_LCD(0x85,bs);
	display_LCD(0x86,ss);
	display_LCD(0x87,gs);
	
}


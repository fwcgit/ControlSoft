#include "oled_i2c.h"
#include "delay.h"
#define OLED_ADDRESS 0x78


void OLED_Init(void)
{
	delay_ms(100); //这里的延时很重要
	
	OLED_Write_cmd(0xAE); //display off
	OLED_Write_cmd(0x20);	//Set Memory Addressing Mode	
	OLED_Write_cmd(0x10);	//00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
	OLED_Write_cmd(0xb0);	//Set Page Start Address for Page Addressing Mode,0-7
	OLED_Write_cmd(0xc8);	//Set COM Output Scan Direction
	OLED_Write_cmd(0x00); //---set low column address
	OLED_Write_cmd(0x10); //---set high column address
	OLED_Write_cmd(0x40); //--set start line address
	OLED_Write_cmd(0x81); //--set contrast control register
	OLED_Write_cmd(0xff); //亮度调节 0x00~0xff
	OLED_Write_cmd(0xa1); //--set segment re-map 0 to 127
	OLED_Write_cmd(0xa6); //--set normal display
	OLED_Write_cmd(0xa8); //--set multiplex ratio(1 to 64)
	OLED_Write_cmd(0x3F); //
	OLED_Write_cmd(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
	OLED_Write_cmd(0xd3); //-set display offset
	OLED_Write_cmd(0x00); //-not offset
	OLED_Write_cmd(0xd5); //--set display clock divide ratio/oscillator frequency
	OLED_Write_cmd(0xf0); //--set divide ratio
	OLED_Write_cmd(0xd9); //--set pre-charge period
	OLED_Write_cmd(0x22); //
	OLED_Write_cmd(0xda); //--set com pins hardware configuration
	OLED_Write_cmd(0x12);
	OLED_Write_cmd(0xdb); //--set vcomh
	OLED_Write_cmd(0x20); //0x20,0.77xVcc
	OLED_Write_cmd(0x8d); //--set DC-DC enable
	OLED_Write_cmd(0x14); //
	OLED_Write_cmd(0xaf); //--turn on oled panel
}

void OLED_Config()
{
	GPIO_InitTypeDef GPIO_Init_Type_Structurn;
	I2C_InitTypeDef	 I2C_Init_Type_Structurn;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_Init_Type_Structurn.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Init_Type_Structurn.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init_Type_Structurn.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_Init_Type_Structurn);
	
	GPIO_Init_Type_Structurn.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init_Type_Structurn.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init_Type_Structurn.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_Init_Type_Structurn);
	
	I2C_Init_Type_Structurn.I2C_Mode = I2C_Mode_I2C;
	I2C_Init_Type_Structurn.I2C_OwnAddress1 = 0x30;
	I2C_Init_Type_Structurn.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_Init_Type_Structurn.I2C_Ack = I2C_Ack_Enable;
	I2C_Init_Type_Structurn.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init_Type_Structurn.I2C_ClockSpeed = 400000;
	
	I2C_Cmd(I2C2,ENABLE);
	I2C_Init(I2C2,&I2C_Init_Type_Structurn);
	
	GPIOB->BSRR = 1<<2;
	
}

void OLED_Write_cmd(u8 cmd)
{
	I2C_Write_Byte(0x00,cmd);
}
void OLED_Write_data(u8 data)
{
	I2C_Write_Byte(0x40,data);
}
void I2C_Write_Byte(uint8_t addr,uint8_t data)
{
	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C2,ENABLE);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	
	I2C_Send7bitAddress(I2C2,OLED_ADDRESS,I2C_Direction_Transmitter);
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	
	I2C_SendData(I2C2, addr);//寄存器地址
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C2, data);//发送数据
	while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	I2C_GenerateSTOP(I2C2, ENABLE);//关闭I2C2总线
}

void OLED_ON(void)
{
	OLED_Write_cmd(0X8D);  //设置电荷泵
	OLED_Write_cmd(0X14);  //开启电荷泵
	OLED_Write_cmd(0XAF);  //OLED唤醒
}

//--------------------------------------------------------------
// Prototype      : void OLED_OFF(void)
// Calls          : 
// Parameters     : none
// Description    : 让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
//--------------------------------------------------------------
void OLED_OFF(void)
{
	OLED_Write_cmd(0X8D);  //设置电荷泵
	OLED_Write_cmd(0X10);  //关闭电荷泵
	OLED_Write_cmd(0XAE);  //OLED休眠
}

void OLED_XY(u8 x,u8 y)
{
	OLED_Write_cmd(0xb0+y);
	OLED_Write_cmd(((x&0xf0)>>4)|0x10);
	OLED_Write_cmd((x&0x0f)|0x01);
}


void OLED_Fill(unsigned char fill_Data)//全屏填充
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
		OLED_Write_cmd(0xb0+m);		//page0-page1
		OLED_Write_cmd(0x00);		//low column start address
		OLED_Write_cmd(0x10);		//high column start address
		for(n=0;n<128;n++)
			{
				OLED_Write_data(fill_Data);
			}
	}
}

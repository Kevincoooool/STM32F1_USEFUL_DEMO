/**
  ******************************************************************************
  * @file    bsp_BS81XX.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   CRC计算相关
  ******************************************************************************
  */
#include "include.h"
#include "debug.h"

uint8_t Key_Val; //key值全局变量

void BS81XXWait(void)
{
	u32 t = 100;
	while (t--)
		;
}

void BS81XXI2CStart(void)
{			   //在SCL高电平时，SDA从高电平跳变到低电平为起始条件
	IIC_SCL_L; //SCL = 0;
	BS81XXWait();
	IIC_SDA_H; //SDA = 1;
	BS81XXWait();
	IIC_SCL_H; //SCL = 1;
	BS81XXWait();
	IIC_SDA_H; //SDA = 1;
	BS81XXWait();
	IIC_SDA_L; //SDA = 0;
	BS81XXWait();
}

void BS81XXI2CStop(void)
{			   //在SCL高电平时，SDA从低电平跳变到高电平为停止条件
	IIC_SCL_H; //SCL = 1;
	IIC_SDA_L; //SDA = 0;
	BS81XXWait();
	IIC_SDA_H; //SDA = 1;
	BS81XXWait();
}

u8 BS81XXWACK(void)
{ //等待从机ACK,等待slave把sda拉回low
	u16 i = 0;
	IIC_SCL_L; //SCL = 0;
	BS81XXWait();
	IIC_SCL_H;				  //SCL = 1;
	while (IIC_READ_SDA == 1) //SDAIN == 1)
	{
		if (i++ > 3000)
		{
			return 1; //超时
		}
	}
	//IIC_SCL_L;
	return 0;
}

void BS81XXACK(void)
{			   //ACK,SCL低电平时，SDA改变数据
	IIC_SCL_L; //SCL = 0;
	BS81XXWait();
	IIC_SDA_L; //SDA = 0;
	BS81XXWait();
	IIC_SCL_H; //SCL = 1;
	BS81XXWait();
}

void BS81XXNoACK(void)
{			   //NACK
	IIC_SCL_L; //SCL = 0;
	BS81XXWait();
	IIC_SDA_H; //SDA = 1;
	BS81XXWait();
	IIC_SCL_H; //SCL = 1;
	BS81XXWait();
}

void BS81XXI2CSend(u8 dat)
{
	u8 i;
	for (i = 0; i < 8; i++)
	{
		IIC_SCL_L; //SCL = 0;
		BS81XXWait();
		if ((dat & 0x80))
		{
			IIC_SDA_H; //SDA = 1;
		}
		else
		{
			IIC_SDA_L; //SDA = 0;
		}
		BS81XXWait();
		dat = dat << 1;
		IIC_SCL_H; //SCL = 1;
		BS81XXWait();
	}
}

u8 BS81XXI2CRec(void)
{
	u8 i, k = 0;
	BS81XXWait();
	for (i = 8; i > 0; i--)
	{
		BS81XXWait();
		IIC_SCL_L; //SCL = 0;
		BS81XXWait();
		IIC_SCL_H; //SCL = 1;
		BS81XXWait();
		k = (k << 1) | (IIC_READ_SDA); //SDAIN;
	}
	return k;
}

void Key_Init(void)
{
	//uint8_t status;
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //先使能外设IO 时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  //根据设定参数初始化GPIO
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);

	/*--------------------------BS81XX按键中断配置-----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* 选择EXTI的信号源 */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	/* EXTI为中断模式 */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	/* 使能中断 */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}



u8 GetKey(u8 dat)
{
	u8 t = RETRY;
	u8 a;

	while (t--)
	{
		BS81XXI2CStart();
		BS81XXI2CSend(0xA0); //写
		if (BS81XXWACK())
			continue; //此时SCL被从机拉低，等待高电平再继续
		BS81XXWait();
		while (IIC_READ_SCL == 0)
			;				//while(SCLIN == 0);
		BS81XXI2CSend(dat); //读取
		if (BS81XXWACK())
			continue;
		BS81XXWait();
		while (IIC_READ_SCL == 0)
			; //while(SCLIN == 0);
		BS81XXI2CStart();
		BS81XXI2CSend(0xA1); //读取
		if (BS81XXWACK())
			continue;
		BS81XXWait();
		while (IIC_READ_SCL == 0)
			; //SCLIN == 0);
		a = BS81XXI2CRec();
		BS81XXNoACK();
		BS81XXI2CStop();
		return a;
	}
	return 0;
}

uint8_t TK_SetReg(uint8_t addr, uint8_t reg, uint8_t len, const uint8_t *buf)
{
	uint8_t i;
	BS81XXI2CStart();
	BS81XXI2CSend(addr); //发送器件地址+写命令
	if (BS81XXWACK())	 //等待应答
	{
		BS81XXI2CStop();
		return 1;
	}
	BS81XXI2CSend(reg); //写寄存器地址
	BS81XXWACK();		//等待应答
	for (i = 0; i < len; i++)
	{
		BS81XXI2CSend(buf[i]); //发送数据
		if (BS81XXWACK())	   //等待ACK
		{
			BS81XXI2CStop();
			return 1;
		}
	}
	BS81XXI2CStop();
	return 0;
}

//uint16_t TK_Scan(void)
//{
//	uint16_t Key_Val = 0;
//	uint16_t  Key_ValH = 0; //高位键值
//	uint16_t  Key_ValL = 0; //地位键值
//
//	Key_ValH	|=	GetKey(0x09);
//	Key_ValL	|=	GetKey(0x08);
//
//	Key_Val = (Key_ValH<<8)|Key_ValL;
//
//	return Key_Val;
//}
uint8_t Read_Key(uint8_t value)
{
	return Touch_Key_Analysis()==value?1:0;
}
uint8_t Read_Key1(void)
{
	return Read_Key(1);
}
uint8_t Read_Key2(void)
{
	return Read_Key(2);
}
uint8_t Touch_Key_Analysis(void)
{

	static uint16_t Key_Val;
	static uint16_t Key_Val_H;
	static uint16_t Key_Val_L;

	Key_Val_H = GetKey(0x09);
	Key_Val_L = GetKey(0x08);
	Key_Val = (Key_Val_H << 8) | Key_Val_L;
	switch (Key_Val)
	{
	case Key1_Pres:
		//printf("\r\n 4");
		return 1;
		//break;
	case Key2_Pres:
		//printf("\r\n 1");
		return 2;
		//break;
	case Key3_Pres:
		//printf("\r\n 2");
		return 3;
		//break;
	case Key4_Pres:
		//printf("\r\n 7");
		return 4;
		//break;
	case Key5_Pres:
		//printf("\r\n 10");
		return 5;
		//break;
	case Key6_Pres:
		//printf("\r\n 11");
		return 6;
		//break;
	case Key7_Pres:
		//printf("\r\n 8");

		return 7;
		//break;
	case Key8_Pres:
		//printf("\r\n 5");

		return 8;
		//break;
	case Key9_Pres:
		//printf("\r\n 3");

		return 9;
		//break;
	case Key10_Pres:
		//printf("\r\n 6");

		return 10;
		//break;
	case Key11_Pres:
		//printf("\r\n 9");
		return 11;
		//break;
	case Key12_Pres:
		//printf("\r\n 12");
		return 12; 
	case Key13_Pres:
		//printf("\r\n 12");
		return 13;
	case Key14_Pres:
		//printf("\r\n 12");
		return 14;
	case Key15_Pres:
		//printf("\r\n 12");
		return 15;
	case Key16_Pres:
		//printf("\r\n 12");
		return 16;
	
	default:
		return 0;
	}
}
void EXTI0_IRQHandler(void)
{
	//确保是否产生了EXTI Line中断
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{

		Key_Val = Touch_Key_Analysis(); //读取Key 并返回
		PRINTF_INFO("Exti!!: %d\r\n", Key_Val);
		// LED1 取反
		LED1_TOGGLE;
		//清除中断标志位
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

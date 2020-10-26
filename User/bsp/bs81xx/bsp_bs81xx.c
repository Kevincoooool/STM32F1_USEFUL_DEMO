/**
  ******************************************************************************
  * @file    bsp_BS81XX.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   CRC�������
  ******************************************************************************
  */
#include "include.h"
#include "debug.h"

uint8_t Key_Val; //keyֵȫ�ֱ���

void BS81XXWait(void)
{
	u32 t = 100;
	while (t--)
		;
}

void BS81XXI2CStart(void)
{			   //��SCL�ߵ�ƽʱ��SDA�Ӹߵ�ƽ���䵽�͵�ƽΪ��ʼ����
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
{			   //��SCL�ߵ�ƽʱ��SDA�ӵ͵�ƽ���䵽�ߵ�ƽΪֹͣ����
	IIC_SCL_H; //SCL = 1;
	IIC_SDA_L; //SDA = 0;
	BS81XXWait();
	IIC_SDA_H; //SDA = 1;
	BS81XXWait();
}

u8 BS81XXWACK(void)
{ //�ȴ��ӻ�ACK,�ȴ�slave��sda����low
	u16 i = 0;
	IIC_SCL_L; //SCL = 0;
	BS81XXWait();
	IIC_SCL_H;				  //SCL = 1;
	while (IIC_READ_SDA == 1) //SDAIN == 1)
	{
		if (i++ > 3000)
		{
			return 1; //��ʱ
		}
	}
	//IIC_SCL_L;
	return 0;
}

void BS81XXACK(void)
{			   //ACK,SCL�͵�ƽʱ��SDA�ı�����
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

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //��ʹ������IO ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;	   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);			  //�����趨������ʼ��GPIO
	GPIO_SetBits(GPIOB, GPIO_Pin_6 | GPIO_Pin_7);

	/*--------------------------BS81XX�����ж�����-----------------------------*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/* ѡ��EXTI���ź�Դ */
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	/* EXTIΪ�ж�ģʽ */
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �½����ж� */
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	/* ʹ���ж� */
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
		BS81XXI2CSend(0xA0); //д
		if (BS81XXWACK())
			continue; //��ʱSCL���ӻ����ͣ��ȴ��ߵ�ƽ�ټ���
		BS81XXWait();
		while (IIC_READ_SCL == 0)
			;				//while(SCLIN == 0);
		BS81XXI2CSend(dat); //��ȡ
		if (BS81XXWACK())
			continue;
		BS81XXWait();
		while (IIC_READ_SCL == 0)
			; //while(SCLIN == 0);
		BS81XXI2CStart();
		BS81XXI2CSend(0xA1); //��ȡ
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
	BS81XXI2CSend(addr); //����������ַ+д����
	if (BS81XXWACK())	 //�ȴ�Ӧ��
	{
		BS81XXI2CStop();
		return 1;
	}
	BS81XXI2CSend(reg); //д�Ĵ�����ַ
	BS81XXWACK();		//�ȴ�Ӧ��
	for (i = 0; i < len; i++)
	{
		BS81XXI2CSend(buf[i]); //��������
		if (BS81XXWACK())	   //�ȴ�ACK
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
//	uint16_t  Key_ValH = 0; //��λ��ֵ
//	uint16_t  Key_ValL = 0; //��λ��ֵ
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
	//ȷ���Ƿ������EXTI Line�ж�
	if (EXTI_GetITStatus(EXTI_Line0) != RESET)
	{

		Key_Val = Touch_Key_Analysis(); //��ȡKey ������
		PRINTF_INFO("Exti!!: %d\r\n", Key_Val);
		// LED1 ȡ��
		LED1_TOGGLE;
		//����жϱ�־λ
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

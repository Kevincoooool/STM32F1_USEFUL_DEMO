/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @author  jiejei
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  */

#include "include.h"

/**
  ******************************************************************
														��������
  ******************************************************************
  */
#if USE_USART_DMA_RX
static void USARTx_DMA_Rx_Config(void);
#endif

#if USE_USART_DMA_TX
static void USARTx_DMA_Tx_Config(void);
#endif

/**
  ******************************************************************
														��������
  ******************************************************************
  */
uint8_t Usart_Rx_Buf[USART_RX_BUFF_SIZE];

/************************************************************
  * @brief   NVIC_Configuration
  * @param   NULL
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ���ô��ڵ�NVIC һ������Ϊ4λ��ռ���ȼ�����
  ***********************************************************/
static void NVIC_Configuration(void);
/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Ƕ�������жϿ�������ѡ�� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* ����USARTΪ�ж�Դ */
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	/* �������ȼ�*/
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	/* �����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	/* ��ʼ������NVIC */
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  USART GPIO ����,������������
  * @param  ��
  * @retval ��
  */
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);

	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

	// ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl =
		USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStructure);
	// �����ж����ȼ�����
	NVIC_Configuration();

#if USE_USART_DMA_RX
	// ���� ���ڿ���IDEL �ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_IDLE, ENABLE);
	// ��������DMA����
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE);
	/* ʹ�ܴ���DMA */
	USARTx_DMA_Rx_Config();
#else
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
#endif

#if USE_USART_DMA_TX
	// ��������DMA����
	//	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);
	USARTx_DMA_Tx_Config();
#endif

	// ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);
}

#if USE_USART_DMA_RX

static void USARTx_DMA_Rx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// ����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// ����DMAԴ��ַ���������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART_DR_ADDRESS;
	// �ڴ��ַ(Ҫ����ı�����ָ��)
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart_Rx_Buf;
	// ���򣺴��ڴ浽����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	// �����С
	DMA_InitStructure.DMA_BufferSize = USART_RX_BUFF_SIZE;
	// �����ַ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// �ڴ��ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// �������ݵ�λ
	DMA_InitStructure.DMA_PeripheralDataSize =
		DMA_PeripheralDataSize_Byte;
	// �ڴ����ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	// DMAģʽ��һ�λ���ѭ��ģʽ
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	// ���ȼ�����
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	// ��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// ����DMAͨ��
	DMA_Init(USART_RX_DMA_CHANNEL, &DMA_InitStructure);
	// ���DMA���б�־
	DMA_ClearFlag(DMA1_FLAG_TC5);
	DMA_ITConfig(USART_RX_DMA_CHANNEL, DMA_IT_TE, ENABLE);
	// ʹ��DMA
	DMA_Cmd(USART_RX_DMA_CHANNEL, ENABLE);
}
#endif

#if USE_USART_DMA_TX

uint8_t Usart_Tx_Buf[USART_TX_BUFF_SIZE];

static void USARTx_DMA_Tx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// ����DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	// ����DMAԴ��ַ���������ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART_DR_ADDRESS;
	// �ڴ��ַ(Ҫ����ı�����ָ��)
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart_Tx_Buf;
	// ���򣺴��ڴ浽����
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	// �����С
	DMA_InitStructure.DMA_BufferSize = USART_TX_BUFF_SIZE;
	// �����ַ����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	// �ڴ��ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	// �������ݵ�λ
	DMA_InitStructure.DMA_PeripheralDataSize =
		DMA_PeripheralDataSize_Byte;
	// �ڴ����ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	// DMAģʽ��һ�λ���ѭ��ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	//		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	// ���ȼ�����
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	// ��ֹ�ڴ浽�ڴ�Ĵ���
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	// ����DMAͨ��
	DMA_Init(USART_TX_DMA_CHANNEL, &DMA_InitStructure);
	// ʹ��DMA
	DMA_Cmd(USART_TX_DMA_CHANNEL, ENABLE);
}
#endif

/**
  ******************************************************************
  * @brief   ����һ��DMA����
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  * @param   len: ���ݴ�����(����)
  * @return  NULL
  ******************************************************************
  */

/*****************  ����һ���ֽ� **********************/
void Usart_SendByte(USART_TypeDef *pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx, ch);

	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

/****************** ����8λ������ ************************/
void Usart_SendArray(USART_TypeDef *pUSARTx, uint8_t *array, uint16_t num)
{
	uint8_t i;

	for (i = 0; i < num; i++)
	{
		/* ����һ���ֽ����ݵ�USART */
		Usart_SendByte(pUSARTx, array[i]);
	}
	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
		;
}

/*****************  �����ַ��� **********************/
void Usart_SendString(USART_TypeDef *pUSARTx, char *str)
{
	unsigned int k = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str + k));
		k++;
	} while (*(str + k) != '\0');

	/* �ȴ�������� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET)
	{
	}
}

/*****************  ����һ��16λ�� **********************/
void Usart_SendHalfWord(USART_TypeDef *pUSARTx, uint16_t ch)
{
	uint8_t temp_h, temp_l;

	/* ȡ���߰�λ */
	temp_h = (ch & 0XFF00) >> 8;
	/* ȡ���Ͱ�λ */
	temp_l = ch & 0XFF;

	/* ���͸߰�λ */
	USART_SendData(pUSARTx, temp_h);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;

	/* ���͵Ͱ�λ */
	USART_SendData(pUSARTx, temp_l);
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
		;
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(DEBUG_USARTx, (uint8_t)ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET)
		;

	return (ch);
}

int my_putchar(int c)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(DEBUG_USARTx, (uint8_t)c);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET)
		;

	return (c);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
	/* �ȴ������������� */
	while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET)
		;

	return (int)USART_ReceiveData(DEBUG_USARTx);
}

/**
  ******************************************************************
  * @brief   �����жϷ�����
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */
void DEBUG_USART_IRQHandler(void)
{
#if USE_USART_DMA_RX
	/* ʹ�ô���DMA */
	if (USART_GetITStatus(DEBUG_USARTx, USART_IT_IDLE) != RESET)
	{
		/* �������� */
		Receive_DataPack();
		// ��������жϱ�־λ
		USART_ReceiveData(DEBUG_USARTx);
	}
#else
	/* �����ж� */
	if (USART_GetITStatus(DEBUG_USARTx, USART_IT_RXNE) != RESET)
	{
		Receive_DataPack();
	}
#endif
}

/********************************END OF FILE***************************************/

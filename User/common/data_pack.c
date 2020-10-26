/* ͷ�ļ� */

#include "include.h"
/**
  ******************************************************************
  * @brief   ����֡�����ʽ
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  * 
	* --------------------------------------------------------------------
	* | ��ʼ֡ | ���ݳ���|  �豸id  | �豸���� | ��Ч���� | У�� | ����֡ |
	* --------------------------------------------------------------------
	* |  0x02  | length  |system id |   cmd    |   buff   | crc  |  0x03  |
	* --------------------------------------------------------------------
	* |  uint8 | uint8   |  uint8   |  uint8   |   buff   |uint32|  uint8 |
	* -------------------------------------------------------------------- 
	* |  1�ֽ� | 1�ֽ�   |  1�ֽ�   |  1�ֽ�   |   buff   |4�ֽ� |  1�ֽ� |
	* --------------------------------------------------------------------
	******************************************************************
  */

/**                    Usart_Rx_Sta 
	* ----------------------------------------------------------
	* | 1  1  0  0 | 0  0  0  0 | 0  0  0  0 | 0  0  0  0|
	* -----------------------------------------------------------
  * �����λ���ڱ��������Ƿ�������   0��δ��� 1�����
  * �θ�λ���ڱ����Ƿ��յ�����֡ͷ     0��δ��� 1�����
  * ����λ���ڱ������ݰ�����,���ݰ���󳤶�Ϊ(255)���ֽ�
*/
/* ����״̬��� */
uint16_t Usart_Rx_Sta = 0;

/**
  ******************************************************************
														�ڲ����ú�������
  ******************************************************************
  */

/**
  ******************************************************************
  * @brief   �������ݣ������ݰ���ʽ
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  * @param   buff:������ʼ��ַ
  * @return  0�����ͳɹ���������ʾʧ��
  ******************************************************************
  */
int32_t Send_DataPack(void *buff,
					  uint8_t data_len
#if USE_SYSTEM_ID
					  ,
					  uint8_t sys_id
#endif
#if USE_SYSTEM_CMD
					  ,
					  uint8_t sys_cmd
#endif
)
{
#if USE_USART_DMA_TX
	uint8_t *pTxBuf = Usart_Tx_Buf;
#else
	/* ʹ����ͨ���ڷ��� */
	int32_t res;
	uint8_t Usart_Tx_Buf[data_len + 9];
	uint8_t *pTxBuf = Usart_Tx_Buf;
#endif

#if USE_DATA_CRC
	__IO uint32_t CRCValue;
#endif
	/* buff ��Ч	*/
	if (NULL == buff)
	{
		PRINTF_ERR("data Is Null \n");
		ASSERT(ASSERT_ERR);
		return -1;
	}
	/* ����֡ͷ */
	*pTxBuf++ = (uint8_t)(DATA_HEAD);

	/* ���ݳ��� */
	*pTxBuf++ = (uint8_t)(data_len);

	/* �豸id */
#if USE_SYSTEM_ID
	*pTxBuf++ = (uint8_t)(sys_id);
#endif
	/* �豸���� */
#if USE_SYSTEM_ID
	*pTxBuf++ = (uint8_t)(sys_id);
#endif

	/* ��Ч���� */
	if (0 != data_len)
	{
		memcpy(pTxBuf, buff, data_len);
	}
	/* ����ƫ�� */
	pTxBuf = (pTxBuf + data_len);

#if USE_DATA_CRC
	/* ʹ��CRCУ�� */
	CRCValue = CRC_CalcBlockCRC((uint32_t *)buff, data_len);
	/* У��ֵ */
	*pTxBuf++ = (uint8_t)(CRCValue >> 24);
	*pTxBuf++ = (uint8_t)(CRCValue >> 16);
	*pTxBuf++ = (uint8_t)(CRCValue >> 8);
	*pTxBuf++ = (uint8_t)(CRCValue);
#endif
	/* ����֡β */
	*pTxBuf++ = (uint8_t)(DATA_TAIL);

#if USE_USART_DMA_TX
	DMA_Send_Data((pTxBuf - Usart_Tx_Buf));
#else
	res = Usart_Send_Data(Usart_Tx_Buf, (pTxBuf - Usart_Tx_Buf));
	if (res != ERR_OK)
	{
		PRINTF_ERR("uart write error %d \n", res);
		ASSERT(ASSERT_ERR);
		return res;
	}
#endif

	return ERR_OK;
}

#if USE_USART_DMA_TX
/************************************************************
  * @brief   DMA_Send_Data
	* @param   len : ���͵����ݳ��� ���ֽڣ�
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    
  ***********************************************************/
void DMA_Send_Data(uint16_t len)
{

	DMA_Cmd(USART_TX_DMA_CHANNEL, DISABLE);			   //�ر�DMA����
													   //
													   ////	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}	//ȷ��DMA���Ա�����
													   //
	DMA_SetCurrDataCounter(USART_TX_DMA_CHANNEL, len); //�������ݴ�����
													   //
	DMA_Cmd(USART_TX_DMA_CHANNEL, ENABLE);
	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE); //����DMA����
}
#else

err_t Usart_Send_Data(uint8_t *buf, uint16_t len)
{
	uint16_t i = 0;
	/* �ж� buff �ǿ� */
	if ((NULL == buf) || (len == 0))
	{
		PRINTF_ERR("send data is null!");
		return ERR_NULL;
	}
	/* ѭ�����ͣ�һ���ֽ� */
	for (i = 0; i < len; i++)
	{
		Usart_SendByte(DEBUG_USARTx, buf[i]);
	}
	PRINTF_DEBUG("send data length  is %d!", len);
	return ERR_OK;
}

#endif

/************************************************************
  * @brief   Uart_DMA_Rx_Data
  * @param   NULL
  * @return  NULL
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ʹ�ô��� DMA ����ʱ���õĺ���
  ***********************************************************/
#if USE_USART_DMA_RX
void Receive_DataPack(void)
{
	/* ���յ����ݳ��� */
	uint32_t buff_length;

	/* �ر�DMA ����ֹ���� */
	DMA_Cmd(USART_RX_DMA_CHANNEL, DISABLE); /* ��ʱ�ر�dma��������δ���� */

	/* ��ȡ���յ������ݳ��� ��λΪ�ֽ�*/
	buff_length = USART_RX_BUFF_SIZE - DMA_GetCurrDataCounter(USART_RX_DMA_CHANNEL);

	/* ��ȡ���ݳ��� */
	Usart_Rx_Sta = buff_length;

	PRINTF_DEBUG("buff_length = %d\r\n", buff_length);

	/* ��DMA��־λ */
	DMA_ClearFlag(DMA1_FLAG_TC5);

	/* ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ */
	USART_RX_DMA_CHANNEL->CNDTR = USART_RX_BUFF_SIZE;

	/* �˴�Ӧ���ڴ����������ٴ򿪣����� DataPack_Process() ��*/
	DMA_Cmd(USART_RX_DMA_CHANNEL, ENABLE);

	/* �����ź� �����ͽ��յ������ݱ�־����ǰ̨�����ѯ */

	/* ��ǽ�����ɣ��� DataPack_Handle ����*/
	Usart_Rx_Sta |= 0xC000;

	/* 
	DMA �������ȴ����ݡ�ע�⣬����жϷ�������֡�����ʺܿ죬MCU����������˴ν��յ������ݣ�
	�ж��ַ������ݵĻ������ﲻ�ܿ������������ݻᱻ���ǡ���2�ַ�ʽ�����

  1. �����¿�������DMAͨ��֮ǰ����Rx_Buf��������������ݸ��Ƶ�����һ�������У�
  Ȼ���ٿ���DMA��Ȼ�����ϴ����Ƴ��������ݡ�

  2. ����˫���壬��������DMA_MemoryBaseAddr�Ļ�������ַ����ô�´ν��յ������ݾͻ�
  ���浽�µĻ������У������ڱ����ǡ�
	*/
}
#else
void Receive_DataPack(void)
{
	uint8_t res;
	/* ��ȡ���ݻ�����жϱ�־λ */
	res = USART_ReceiveData(DEBUG_USARTx);

	if ((Usart_Rx_Sta & 0x8000) == 0) //����δ���
	{
		if (Usart_Rx_Sta & 0x4000) //���յ���DATA_HEAD
		{
			if (res != DATA_TAIL) /* �յ��Ĳ�������֡β */
			{
				/* ������������ */
				Usart_Rx_Buf[Usart_Rx_Sta & 0XFF] = res;
				Usart_Rx_Sta++;
			}
			else
			{
				if ((Usart_Rx_Sta & 0XFF) <= DATA_TAIL)
				{
					/* ���ݳ���С�ڵ���3���ֽڣ�
          ˵���յ������ݲ�������֡β��
          �ܿ��������ݳ��� 0x03��
          ��Ҫ��������*/
					/* ������������ */
					Usart_Rx_Buf[Usart_Rx_Sta & 0XFF] = res;
					Usart_Rx_Sta++;
				}
				else
				{
					/* ������֡βҲ���� */
					Usart_Rx_Buf[Usart_Rx_Sta & 0XFF] = res;
					Usart_Rx_Sta++;
					Usart_Rx_Sta |= 0x8000; /* ��������� */
					PRINTF_DEBUG("receive ok!");
					PRINTF_DEBUG("buff_length = %d", Usart_Rx_Sta & 0XFF);
				}
			}
		}
		else /* ��û�յ�DATA_HEAD */
		{
			if (res == DATA_HEAD) /* �յ�����֡ͷ */
			{
				Usart_Rx_Buf[Usart_Rx_Sta & 0XFF] = res;
				Usart_Rx_Sta++;
				Usart_Rx_Sta |= 0x4000; /* ��ǽ��յ�֡ͷ */
			}
			else /* ���մ��� */
			{
				Usart_Rx_Sta = 0;
				PRINTF_ERR("receive fail!");
			}
		}
	}
}

#endif

/************************************************************
  * @brief   DataPack_Process
	* @param   buff:���ݱ������ʼ��ַ��datapack:������Ϣ����Ľṹ��ָ��
  * @return  ����0������ճɹ�������������� ��
  * @author  jiejie
  * @github  https://github.com/jiejieTop
  * @date    2018-xx-xx
  * @version v1.0
  * @note    ���ݰ�������������
  ***********************************************************/
err_t DataPack_Process(uint8_t *buff, DataPack_t *datapack)
{
	uint16_t data_len;
	uint16_t check_data_len;
	uint8_t *pbuff = Usart_Rx_Buf;

	if ((NULL == buff) || (NULL == datapack))
	{
		PRINTF_ERR("buff or len is NULL\n");
		ASSERT(ASSERT_ERR);
		return ERR_NULL;
	}
	/* ������� */
	if (Usart_Rx_Sta & 0x8000)
	{
		/* ��ȡ���ݳ��� */
		data_len = Usart_Rx_Sta & 0xffff;
		/* ���������ɱ�־λ */
		Usart_Rx_Sta = 0;
		if (data_len < 4)
		{
			PRINTF_ERR("datapack is mar!");
			return ERR_UNUSE;
		}
		else
		{
			datapack->data_length = *(pbuff + 1) << 8 | *(pbuff + 2);
		}
#if USE_DATA_CRC
		check_data_len = data_len - 8;
		PRINTF_DEBUG("check_data_len = %d", check_data_len);
#else
		check_data_len = data_len - 4;
		PRINTF_DEBUG("check_data_len = %d", check_data_len);
#endif
		/* У�����ݰ��Ƿ����� */
		if (check_data_len == datapack->data_length)
		{
			memcpy(buff, pbuff + 3, datapack->data_length);
			memset(Usart_Rx_Buf, 0, data_len);
			PRINTF_DEBUG("data_length = %d", datapack->data_length);
			PRINTF_DEBUG("data = %s", buff);
			PRINTF_DEBUG("data handle ok��");
		}
		else
		{
			PRINTF_DEBUG("data_length = %d", datapack->data_length);
			PRINTF_ERR("data length is not equal!");
			memset(Usart_Rx_Buf, 0, data_len);
			buff = NULL;
			datapack->data_length = 0;
		}
#if USE_USART_DMA_RX
		/* ��DMA�����Խ�����һ�ν��� */
		DMA_Cmd(USART_RX_DMA_CHANNEL, ENABLE);
#endif
		return ERR_OK;
	}
	return ERR_NULL;
}

/********************************** ���� ********************************************/
///*
//				uint8_t res[50];

//				DataPack_t datapack;
//				int32_t err;

//				err = DataPack_Process(res,&datapack);
//				if(ERR_OK == err)
//				{
//					Send_DataPack(res,datapack.data_length);
//				}
//*/

//					void DEBUG_USART_IRQHandler(void)
//					{
//					#if USE_USART_DMA_RX
//						/* ʹ�ô���DMA */
//						if(USART_GetITStatus(DEBUG_USARTx,USART_IT_IDLE)!=RESET)
//						{
//							/* �������� */
//							Uart_DMA_Rx_Data();
//							// ��������жϱ�־λ
//							USART_ReceiveData( DEBUG_USARTx );
//						}
//					#else
//						/* �����ж� */
//						if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
//						{
//							Receive_DataPack();
//						}
//					#endif
//					}

/********************************** ���� ********************************************/

/********************************END OF FILE***************************************/

#ifndef __DATA_PACK_H
#define __DATA_PACK_H

#include "include.h"


/**
  ******************************************************************
													   �궨��
  ******************************************************************
  */ 

#define USE_SYSTEM_ID   0
#define USE_SYSTEM_CMD  0
#define USE_DATA_CRC    0

/* ����֡ͷ */
#define DATA_HEAD   0x02

/* ����֡β */
#define DATA_TAIL   0x03





/**
  ******************************************************************
													   ��������
  ******************************************************************
  */ 
typedef struct datapack
{
	uint8_t data_head; //����ͷ
	uint8_t data_tail; //����β
//	uint8_t data_type; 			//��������
	uint16_t data_length; // ���ݳ���
	uint32_t data_crc; // ����У��
}DataPack_t;




/**
  ******************************************************************
														��������
  ******************************************************************
  */ 
int32_t Send_DataPack(void *buff,
                      uint8_t data_len
#if USE_SYSTEM_ID
                      ,uint8_t sys_id
#endif
#if USE_SYSTEM_CMD
                      ,uint8_t sys_cmd
#endif
);

err_t DataPack_Process(uint8_t* buff,DataPack_t* datapack);

#if USE_USART_DMA_RX
void Receive_DataPack(void);
#else
void Receive_DataPack(void);
#endif

#if USE_USART_DMA_TX
void DMA_Send_Data(uint16_t len);
#else
err_t Usart_Send_Data(uint8_t *buf, uint16_t len);
#endif




#endif /* __DATA_PACK_H */

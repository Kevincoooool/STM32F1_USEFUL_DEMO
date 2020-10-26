/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  jiejei
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ledӦ�ú����ӿ�
  ******************************************************************************
  */
#include "include.h"

/**
  * @brief  ��ʼ������LED��IO
  * @param  ��
  * @retval ��
  */
void LED_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����LED��ص�GPIO����ʱ��*/
	RCC_APB2PeriphClockCmd(LED1_GPIO_CLK, ENABLE);
	/*ѡ��Ҫ���Ƶ�GPIO����*/
	GPIO_InitStructure.GPIO_Pin = LED1_GPIO_PIN;

	/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;

	/*������������Ϊ50MHz */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*���ÿ⺯������ʼ��GPIO*/
	GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

	//		/*ѡ��Ҫ���Ƶ�GPIO����*/
	//		GPIO_InitStructure.GPIO_Pin = LED2_GPIO_PIN;

	//		/*���ÿ⺯������ʼ��GPIO*/
	//		GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStructure);
	//
	//		/*ѡ��Ҫ���Ƶ�GPIO����*/
	//		GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;

	//		/*���ÿ⺯������ʼ��GPIOF*/
	//		GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

	/* �ر�����led��	*/
	GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);

	/* �ر�����led��	*/
	//		GPIO_SetBits(LED2_GPIO_PORT, LED2_GPIO_PIN);
	//
	//		/* �ر�����led��	*/
	//		GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}

/*********************************************END OF FILE**********************/

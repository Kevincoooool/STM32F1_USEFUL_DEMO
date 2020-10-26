/**
  ******************************************************************************
  * @file    main.c
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   main
  ******************************************************************************
  * @attention ��demo����RTOS�汾����ҪRTOS��demo���ע�ܽܵ�GitHub��
  *	GitHub��https://github.com/jiejieTop
  *
  * ʵ��ƽ̨:Ұ�� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "include.h"

/**
  ******************************************************************
													   ��������
  ******************************************************************
  */

Button_t Button1;
Button_t Button2;

/**
  ******************************************************************
														��������
  ******************************************************************
  */
static void BSP_Init(void);

void Btn1_Dowm_CallBack(void *btn)
{
	PRINTF_INFO("Button1 ����!\r\n");
}

void Btn1_Double_CallBack(void *btn)
{
	PRINTF_INFO("Button1 ˫��!\r\n");
}

void Btn1_Long_CallBack(void *btn)
{
	PRINTF_INFO("Button1 ����!\r\n");
}
void Btn1_Long_Free_CallBack(void *btn)
{
	PRINTF_INFO("Button1 �����ɿ�!\r\n");
}
void Btn1_Continuos_CallBack(void *btn)
{
	PRINTF_INFO("Button1 ����!\r\n");
}
void Btn1_ContinuosFree_CallBack(void *btn)
{
	PRINTF_INFO("Button1 �����ͷ�!\r\n");
}

void Btn2_Dowm_CallBack(void *btn)
{
	PRINTF_INFO("Button2 ����!\r\n");
}

void Btn2_Double_CallBack(void *btn)
{
	PRINTF_INFO("Button2 ˫��!\r\n");
}

void Btn2_Long_CallBack(void *btn)
{
	PRINTF_INFO("Button2 ����!\r\n");
}
void Btn2_Long_Free_CallBack(void *btn)
{
	PRINTF_INFO("Button2 �����ɿ�!\r\n");
}
void Btn2_Continuos_CallBack(void *btn)
{
	PRINTF_INFO("Button2 ����!\r\n");
}
void Btn2_ContinuosFree_CallBack(void *btn)
{
	PRINTF_INFO("Button2 �����ͷ�!\r\n");
}

/**
  ******************************************************************
  * @brief   ������
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */
int main(void)
{
	//  BSP_Init();
	CPU_TS_TmrInit();
	USART_Config();
	LED_GPIO_Config();
	Key_Init();
	Button_Create("Button1",
				  &Button1,
				  Read_Key1,
				  KEY_ON);
	Button_Attach(&Button1, BUTTON_DOWN, Btn1_Dowm_CallBack);			//����
	Button_Attach(&Button1, BUTTON_DOUBLE, Btn1_Double_CallBack);		//˫��
	Button_Attach(&Button1, BUTTON_LONG, Btn1_Long_CallBack);			//����
	Button_Attach(&Button1, BUTTON_LONG_FREE, Btn1_Long_Free_CallBack); //�����ɿ�

	Button_Create("Button2",
				  &Button2,
				  Read_Key2,
				  KEY_ON);
	Button_Attach(&Button2, BUTTON_DOWN, Btn2_Dowm_CallBack);	  //����
	Button_Attach(&Button2, BUTTON_DOUBLE, Btn2_Double_CallBack); //˫��
	Button_Attach(&Button2, BUTTON_LONG, Btn2_Long_CallBack);	  //����
	Button_Attach(&Button2, BUTTON_LONG_FREE, Btn2_Long_Free_CallBack);
	Get_Button_Event(&Button1);
	Get_Button_Event(&Button2);
	PRINTF_INFO("welcome to learn stm32 library!\r\n");
	while (1)
	{

		//PRINTF_INFO("key value :%d!\r\n",Touch_Key_Analysis());
		Button_Process(); //��Ҫ���ڵ��ð���������
		Delay_ms(10);
		LED1_TOGGLE;
	}
}

/**
  ******************************************************************
  * @brief   BSP_Init���������а弶��ʼ��
  * @author  jiejie
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************
  */
//static void BSP_Init(void)
//{
//	/* LED ��ʼ�� */
//	LED_GPIO_Config();

//#if USE_DWT_DELAY
//	/* �ں˾�ȷ��ʱ����ʼ�� */
//	CPU_TS_TmrInit();
//#else
//	/* �δ�ʱ����ʼ�� */
//	SysTick_Init();
//#endif

//	/* ���ڳ�ʼ�� */
//	USART_Config();

//	/* ������ʼ�� */
//	Key_GPIO_Config();

//	//	/* �ⲿ�жϳ�ʼ�� */
//	//	EXTI_Key_Config();

//	CRC_Config();

//	/* ��ӡ��Ϣ */
//	PRINTF_INFO("welcome to learn stm32 library!\r\n");
//}

/********************************END OF FILE***************************************/

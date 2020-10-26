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
static void BSP_Init(void);

void Btn1_Dowm_CallBack(void *btn)
{
  PRINT_INFO("1��������!");
}

void Btn1_Double_CallBack(void *btn)
{
  PRINT_INFO("1����˫��!");
}

void Btn1_Long_CallBack(void *btn)
{
  PRINT_INFO("1��������!");
}

void Btn2_Dowm_CallBack(void *btn)
{
  PRINT_INFO("2��������!");
}

void Btn2_Double_CallBack(void *btn)
{
  PRINT_INFO("2����˫��!");
}

void Btn2_Long_CallBack(void *btn)
{
  PRINT_INFO("2��������!");
}

/**
  ******************************************************************
													   ��������
  ******************************************************************
  */ 
	

Button_t test_button;
Button_t test2_button; 
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

	BSP_Init();
  
//  PRINT_DEBUG("��ǰ��ƽ��%d",Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN));
  
  Button_Create("test_button",
            &test_button, 
            Read_KEY1_Level, 
            KEY_ON);
  Button_Attach(&test_button,BUTTON_DOWM,Btn1_Dowm_CallBack);
  Button_Attach(&test_button,BUTTON_ALL_RIGGER,Btn1_Double_CallBack);
  Button_Attach(&test_button,BUTTON_LONG,Btn1_Long_CallBack);
  
  Button_Create("test2_button",
            &test2_button, 
            Read_KEY2_Level, 
            KEY_ON);
  Button_Attach(&test2_button,BUTTON_DOWM,Btn2_Dowm_CallBack);
  Button_Attach(&test2_button,BUTTON_ALL_RIGGER,Btn2_Double_CallBack);
  Button_Attach(&test2_button,BUTTON_LONG,Btn2_Long_CallBack);
  
  
	while(1)                            
	{
    Button_Cycle_Process(&test_button);
    Button_Cycle_Process(&test2_button);
		Delay_ms(20);
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
static void BSP_Init(void)
{
	/* LED ��ʼ�� */
	LED_GPIO_Config();
	
#if USE_DWT_DELAY
	/* �ں˾�ȷ��ʱ����ʼ�� */
	CPU_TS_TmrInit();
#else
	/* �δ�ʱ����ʼ�� */
	SysTick_Init();
#endif
	
	/* ���ڳ�ʼ�� */
	USART_Config();
  
	/* ������ʼ�� */
  Key_GPIO_Config();
  
//	/* �ⲿ�жϳ�ʼ�� */
//	EXTI_Key_Config(); 
	
	CRC_Config();
	
	/* ��ӡ��Ϣ */
	PRINT_INFO("welcome to learn jiejie stm32 library!\n");
	
}




/********************************END OF FILE***************************************/

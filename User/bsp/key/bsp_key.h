/*
 * @Descripttion : 
 * @version      : 
 * @Author       : Kevincoooool
 * @Date         : 2020-06-01 17:53:39
 * @LastEditors  : Kevincoooool
 * @LastEditTime : 2020-10-26 16:45:01
 * @FilePath     : \stm32f103demo\User\bsp\key\bsp_key.h
 */
#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"

//  ���Ŷ���
#define KEY1_GPIO_CLK RCC_APB2Periph_GPIOA
#define KEY1_GPIO_PORT GPIOA
#define KEY1_GPIO_PIN GPIO_Pin_0

#define KEY2_GPIO_CLK RCC_APB2Periph_GPIOC
#define KEY2_GPIO_PORT GPIOC
#define KEY2_GPIO_PIN GPIO_Pin_13

/** �������±��ú�
	*  ��������Ϊ�ߵ�ƽ������ KEY_ON=1�� KEY_OFF=0
	*  ����������Ϊ�͵�ƽ���Ѻ����ó�KEY_ON=0 ��KEY_OFF=1 ����
	*/


void Key_GPIO_Config(void);
uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

uint8_t Read_KEY1_Level(void);
uint8_t Read_KEY2_Level(void);

#endif /* __KEY_H */

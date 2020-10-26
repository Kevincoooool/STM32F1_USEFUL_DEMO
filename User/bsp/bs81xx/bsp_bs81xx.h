/*
 * @Descripttion : 
 * @version      : 
 * @Author       : Kevincoooool
 * @Date         : 2020-10-26 10:45:53
 * @LastEditors  : Kevincoooool
 * @LastEditTime : 2020-10-26 10:47:56
 * @FilePath     : \stm32f103demo\User\bsp\bs81xx\bsp_bs81xx.h
 */
#ifndef __BS81XX_H
#define __BS81XX_H

#include "stm32f10x.h"

#define IIC_READ_SDA GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) //读取输入SDA 数据
#define IIC_READ_SCL GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) //读取输入SCL数据

#define IIC_SDA_H GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define IIC_SDA_L GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define IIC_SCL_H GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define IIC_SCL_L GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define RETRY 50

#define Key1_Pres 0x0001
#define Key2_Pres 0x0002
#define Key3_Pres 0x0004
#define Key4_Pres 0x0008
#define Key5_Pres 0x0010
#define Key6_Pres 0x0020
#define Key7_Pres 0x0040
#define Key8_Pres 0x0080
#define Key9_Pres 0x0100
#define Key10_Pres 0x0200
#define Key11_Pres 0x0400
#define Key12_Pres 0x0800
#define Key13_Pres 0x1000
#define Key14_Pres 0x2000
#define Key15_Pres 0x4000
#define Key16_Pres 0x8000

#define TK_RegVal 0x0f //保留8,7位；门坎值<0x3f  0--63
#define Slave_ADDR 0x50

void IIC_SDA_IN(void);
void IIC_SDA_OUT(void);

void IIC_SCL_IN(void);
void IIC_SCL_OUT(void);

void Key_Init(void);
u8 GetKey(u8 dat);
void Check_Sum(void);
uint8_t TK_SetReg(uint8_t addr, uint8_t reg, uint8_t len, const uint8_t *buf);
uint16_t TK_Scan(void);
uint8_t TK_Analyze(void);
uint8_t Touch_Key_Analysis(void);
void CRC_Config(void);
uint8_t Read_Key(uint8_t value);
uint8_t Read_Key1(void);
uint8_t Read_Key2(void);

#endif /* __CRC_H */

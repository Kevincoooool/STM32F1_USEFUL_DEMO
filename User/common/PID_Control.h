/**
  ******************************************************************************
  * @file    PID_Control.h
  * @author  willieon
  * @version V0.1
  * @date    January-2015
  * @brief   PID�����㷨ͷ�ļ�
  *                        ����ṹ�������Լ���������
  *                        #define IF_THE_INTEGRAL_SEPARATION  0/1  Ϊ���ַ����־
  ******************************************************************************
  **/
 
#ifndef __PID_CONTROL_H__
#define __PID_CONTROL_H__
 
 
 
 
#define IF_THE_INTEGRAL_SEPARATION  0    
//#define IF_THE_INTEGRAL_SEPARATION  1   //�Ƿ���ַ���  0-�����룬1 -����
 
typedef struct 
{
        double SetPoint; // �趨Ŀ�� Desired Value   
        double Proportion; // �������� Proportional Const 
        double Integral; // ���ֳ��� Integral Const 
        double Derivative; // ΢�ֳ��� Derivative Const   
        double LastError; // Error[-1] 
        double PrevError; // Error[-2] 
        double SumError; // Sums of Errors  
}PID;
 
#if IF_THE_INTEGRAL_SEPARATION            //�Ƿ���ַ���Ԥ���뿪ʼ
 
double PIDCalc(double NextPoint ,double SepLimit, PID *pp);   //�����ַ����PID����
 
#else
 
double PIDCalc( double NextPoint, PID *pp);     //�������ַ����PID����
 
#endif        //�Ƿ���ַ���Ԥ�������
 
void PIDInit (double SetPoint, double Proportion, double Integral, double Derivative, PID *pp);
 
#endif

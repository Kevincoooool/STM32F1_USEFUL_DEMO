/**
  ******************************************************************************
  * @file    PID_Control.c
  * @author  willieon
  * @version V0.1
  * @date    January-2015
  * @brief   PID�����㷨��������
  *        
  *
  ******************************************************************************
  **/

#include "PID_Control.h"
#include "math.h"

/*************************************************************************************
*        ��    �ƣ� double PIDCalc( PID *pp, double NextPoint ,double SepLimit)
*        ��    �ܣ� PID��������
*        ��ڲ����� PID *pp  - �����������������Ľṹ��
*                           NextPoint - ����������ֵ
*                           SepLimit  - ���ַ�������
*        ���ڲ����� ����PID������
*        ˵    ���� Ĭ�ϲ����л��ַ��룬����û���Ҫʹ�û��ַ��룬����PID_Control.h��
*                                �� #define IF_THE_INTEGRAL_SEPARATION  0  ��Ϊ 
*                            #define IF_THE_INTEGRAL_SEPARATION  1
*        ���÷����� ���л��ַ���ʱ��ڲ���Ϊ3�������巽�����£�
*                                PID PIDControlStruct ;   //����PID����ṹ��
*                                PIDInit(50, 0.24, 0.04, 0.2, &PIDControlStruct);//�ṹ���ʼ����ע��&���Ų���ʡ
*                                ControlData = PIDCalc(ReadData, 200, &PIDControlStruct);   //������ = PIDCalc(����ֵ,���ַ�������,PID����ṹ��)
*
***************************************************************************************
*/

#if IF_THE_INTEGRAL_SEPARATION

double PIDCalc(double NextPoint, double SepLimit, PID *pp)
{
	double dError, Error, Flag;
	Error = pp->SetPoint - NextPoint; // ƫ��
	if (abs(Error) > SepLimit)		  //��ƫ����ڷ������޻��ַ���
	{
		Flag = 0;
	}
	else //��ƫ��С�ڷ������ޣ����������
	{
		Flag = 1;
		pp->SumError += Error; // ����
	}
	dError = pp->LastError - pp->PrevError; // ��ǰ΢��
	pp->PrevError = pp->LastError;
	pp->LastError = Error;
	return (
		pp->Proportion * Error				 // ������
		+ Flag * pp->Integral * pp->SumError // ������
		+ pp->Derivative * dError			 // ΢����
	);
}

#else

double PIDCalc(double NextPoint, PID *pp)
{
	double dError, Error;
	Error = pp->SetPoint - NextPoint;		// ƫ��
	pp->SumError += Error;					// ����
	dError = pp->LastError - pp->PrevError; // ��ǰ΢��
	pp->PrevError = pp->LastError;
	pp->LastError = Error;
	return (pp->Proportion * Error		  // ������
			+ pp->Integral * pp->SumError // ������
			+ pp->Derivative * dError	  // ΢����
	);
}

#endif

/*************************************************************************************
*        ��    �ƣ� double PIDCalc( PID *pp, double NextPoint ,double SepLimit)
*        ��    �ܣ� PID��ʼ���趨
*        ��ڲ����� PID *pp  - �����������������Ľṹ��
*                           SetPoint - �趨��Ŀ��ֵ
*                           Proportion��Integral ��Derivative - P,I,Dϵ��
*        ���ڲ����� ��
*        ˵    ����        
*        ���÷�����  PID PIDControlStruct ;   //����PID����ṹ��
*                                PIDInit(50, 0.24, 0.04, 0.2, &PIDControlStruct);//�ṹ���ʼ����ע��&���Ų���ʡ
*                                ��Ϊ������Ҫ����һ��ָ�룬��Ҫ�Խṹ��ȡ�׵�ַ����ָ��
*
***************************************************************************************
*/

void PIDInit(double SetPoint, double Proportion, double Integral, double Derivative, PID *pp)
{
	pp->SetPoint = SetPoint;	 // �趨Ŀ�� Desired Value
	pp->Proportion = Proportion; // �������� Proportional Const
	pp->Integral = Integral;	 // ���ֳ��� Integral Const
	pp->Derivative = Derivative; // ΢�ֳ��� Derivative Const
	pp->LastError = 0;			 // Error[-1]
	pp->PrevError = 0;			 // Error[-2]
	pp->SumError = 0;			 // Sums of Errors

	//memset ( pp,0,sizeof(struct PID));   //need include "string.h"
}

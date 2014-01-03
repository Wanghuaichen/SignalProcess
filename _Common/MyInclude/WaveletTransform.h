#pragma once
#include <math.h>
#include "Peak.h"

//DB4С���ĳ߶Ⱥ���ϵ��
const double h0 = (1.0 + sqrtf(3.0)) / (4 * sqrtf(2.0));
const double h1 = (3.0 + sqrtf(3.0)) / (4 * sqrtf(2.0));
const double h2 = (3.0 - sqrtf(3.0)) / (4 * sqrtf(2.0));
const double h3 = (1.0 - sqrtf(3.0)) / (4 * sqrtf(2.0));

//DB4С����С������ϵ��
const double g0 = h3;
const double g1 = -h2;
const double g2 = h1;
const double g3 = h0;


// CWaveletTransform

class CWaveletTransform : public CWnd
{
	DECLARE_DYNAMIC(CWaveletTransform)

public:
	CWaveletTransform();
	virtual ~CWaveletTransform();
	void WatchDate(double TimeData[], double VoltageData[], int n); //��������
	void Reshape(double TimeData[],double VoltageData[], int n); //�Եõ�����������
	void MovingAverage(double TimeData[], double VoltageData[], int n, int Avgtimes); //���ƶ�ƽ�������˲�
	void DB4DWT(double Data[], int n); // DB4С���任
	void DB4LiftDWT(double Data[], int n); // DB4С���任�����㷨
	void HaarDWT(double Src_val[]/*ԭʼ����*/,double** C_val/*��������*/,int n_total,int levl); //��ɢС���ֽ�

//�������ع��ķ���
	double Reshape(std::vector<std::pair<double,double> > &ReshapeData);  //�Եõ�����������
	double Reshape_v2(std::vector<CSema> &ReshapeData); //�Եõ�����������
	void MovingAverage(std::vector<std::pair<double,double> >&ReshapeData, int Avgtimes); //���ƶ�ƽ�������˲�

public:
	double set_D(double a,double b);
	double set_A(double a,double b);
	

protected:
	DECLARE_MESSAGE_MAP()
};



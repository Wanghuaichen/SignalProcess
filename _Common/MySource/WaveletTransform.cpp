// WaveletTransform.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "SignalProcess.h"
#include "WaveletTransform.h"
#include <time.h>
#include <algorithm>
#include <utility>
#include <vector>


// CWaveletTransform

IMPLEMENT_DYNAMIC(CWaveletTransform, CWnd)

CWaveletTransform::CWaveletTransform()
{

}

CWaveletTransform::~CWaveletTransform()
{
}

void CWaveletTransform::MovingAverage(double TimeData[], double VoltageData[], int n, int Avgtimes) //���ƶ�ƽ�������˲�
{
	for (int i=0; i<n-Avgtimes; i++)
	{
		for (int j=1; j<Avgtimes; j++)
		{
			VoltageData[i] = VoltageData[i] + VoltageData[i+j];
		}
		VoltageData[i] = VoltageData[i]/Avgtimes;
	}
	for (int i=n-Avgtimes; i<n; i++)
	{
		TimeData[i] = TimeData[n-Avgtimes];
		VoltageData[i] = VoltageData[n-Avgtimes];
	}
}
void CWaveletTransform::WatchDate(double TimeData[], double VoltageData[], int n) //��������
{
	std::pair<double,double> DataPair;
	std::vector< std::pair<double,double> > MyData; //δ�����������
	for (int index=0; index<n; index++)
	{
		DataPair = std::make_pair(TimeData[index],VoltageData[index]);
		MyData.push_back(DataPair);
		if (VoltageData[index]==NULL)
		{
			break;
		}
		
	}
	//int i;
}
void CWaveletTransform::Reshape(double TimeData[], double VoltageData[], int n) //�Եõ�����������
{
	// �����TimeData��������Ԫ��֮��
	double dSum = 0.0;
	for (int index=0; index<n; index++)
	{
		dSum += TimeData[index];
	}
	// ����
	std::pair<double,double> DataPair;
	std::vector< std::pair<double,double> > MyData; //δ�����������
	for (int index=0; index<n; index++)
	{
		DataPair = std::make_pair(TimeData[index],VoltageData[index]);
		MyData.push_back(DataPair);
	}
	std::vector< std::pair<double,double> >::iterator begin = MyData.begin();
	std::vector< std::pair<double,double> >::iterator end = MyData.end();
	std::sort(begin,end);
	for (int index=0; index<n; index++)
	{
		TimeData[index] = begin->first;
		VoltageData[index] = begin->second;
		if (index!=n-1) begin++;
	}
	//���ʱ���������
	double dCycle = (2*dSum-2*(begin->first))/(n*(n-1));  //ʱ������(�Ȳ����м��)
	//if (dCycle >= 10.0)
	
	double d45Cycle = (int)(dCycle*10+5)/10;  //�Լ����������
	const int nReshapeDataSize = static_cast<int>(((DataPair.first)-(MyData.begin()->first))/d45Cycle+1); 
	double *ReshapeTimeData = new double[nReshapeDataSize]; 
	double *ReshapeVoltageData = new double[nReshapeDataSize];
	ReshapeTimeData[0] = TimeData[0];
	ReshapeVoltageData[0] = VoltageData[0];
	for (int i=1; i<nReshapeDataSize; i++)
	{
		ReshapeTimeData[i] = ReshapeTimeData[i-1] + d45Cycle; //TimeData������
		int start = 0;
		int last = 0;
		bool bInsertVal = true;
		for (int j=i; ;j++) //������Բ�ֵ�ķ�Χ
		{
			if (TimeData[j] == ReshapeTimeData[i])
			{
				ReshapeVoltageData[i] = VoltageData[j];
				bInsertVal = false;
				break;
			}
			if (TimeData[j] > ReshapeTimeData[i])
			{
				last = j;
				start = j - 1;
				break;
			}
		}
		if (bInsertVal)
		{
			double k = (VoltageData[last]-VoltageData[start])/d45Cycle; //����б��
			double b = VoltageData[start] - k*TimeData[start];
			ReshapeVoltageData[i] = k*ReshapeTimeData[i] + b;
		}
	}
	//std::vector< std::pair<double,double> > MyReshapeData; //�����������
	for (int index=0; index<n; index++)
	{
		if (index > nReshapeDataSize-1)
		{
			TimeData[index] = ReshapeTimeData[nReshapeDataSize-1];
			VoltageData[index] = ReshapeVoltageData[nReshapeDataSize-1];
		}
		else
		{
			TimeData[index] = ReshapeTimeData[index];
			VoltageData[index] = ReshapeVoltageData[index];
		}
		//DataPair = std::make_pair(TimeData[index], VoltageData[index]);
		//MyReshapeData.push_back(DataPair);
	}
	
	//DB4DWT(ReshapeTimeData, nReshapeDataSize); //С���任�㷨�����㷨����Ⱦ�
	//DB4LiftDWT(ReshapeTimeData,nReshapeDataSize); //С���任�����㷨�����㷨����Ⱦ�


	delete []ReshapeTimeData;
	delete []ReshapeVoltageData;
}




void CWaveletTransform::DB4DWT(double Data[], int n) // DB4С���任
{
	if ( n>=4 )
	{
		int i=0, j=0;
		int half = n >> 1;
		double *tmp = new double[n];
		for (j=0; j<half; j++)
		{
			tmp[j] = Data[(2*j)%n]*h0+
				Data[(2*j+1)%n]*h1+
				Data[(2*j+2)%n]*h2+	
				Data[(2*j+3)%n]*h3;
		
			tmp[j+half] = Data[(2*j)%n]*g0+
						  Data[(2*j+1)%n]*g1+
						  Data[(2*j+2)%n]*g2+
						  Data[(2*j+3)%n]*g3;
		}
		for (i=0; i<n; i++)
		{
			Data[i] = tmp[i];
		}
		delete []tmp;
	}
}

void CWaveletTransform::DB4LiftDWT(double Data[], int n)
{
	int half = n>>1;
	double *pS = new double[half];	//��ʱ�������ƽ��ϵ��
	double *pD = new double[half];	//��ʱ�������ϸ��ϵ��
	for (int i=0; i<half; i++)
	{
		pS[i] = Data[2*i]; //even
		pD[i] = Data[2*i+1]; //odd
	}
	for (int i=0; i<half; i++) //DB4�任Update1
	{
		pS[i] = pS[i] + pD[i] * sqrt(3.0);
	}
	for (int i=0; i<half; i++) //DB4��predict
	{
		pD[i] = pD[i] - sqrt(3.0)*pS[i]/4
			- (sqrt(3.0)-2)*pS[(i-1)>0 ? (i-1) : (half+i-1)]/4;  //�߽������������
	}
	for (int i = 0; i<half; i++) //DB4��updata2
	{
		pS[i] = pS[i] - pD[(i+1)%half];  //�߽������������
	}
	for (int i = 0; i<half; i++)
	{
		pS[i] = (sqrt(3.0)-1)*pS[i]/sqrt(2.0); //����ϵ��
		pS[i] = (sqrt(3.0)+1)*pD[i]/sqrt(2.0);
	}
	for (int i=0; i<half; i++)
	{
		Data[i] = pS[i]; //��ƽ��ϵ���Ż�ԭ����
	}
	for (int i=half; i<n; i++)
	{
		Data[i] = pD[i-half]; //��ϸ��ϵ���Ż�ԭ����
	}

	delete []pD;
	delete []pS;
}




void CWaveletTransform::HaarDWT(double Src_val[]/*ԭʼ����*/,double** ppC_val/*��������*/,int n_total,int levl) //��ɢС���ֽ�
{
	double* C_val = new double[n_total];
	* ppC_val = C_val;

	if(levl==0)
	{
		for(int i=0;i<n_total;i++)
			C_val[i] = Src_val[i];
		
	}
	else if(levl>log10((double)n_total)/log10(2.0))
	{
		CString str("���������� 0 �� ");
		CString tmp;
		tmp.Format(_T("%d"),(int)(log10((double)n_total)/log10(2.0)));
		str+=tmp;
		str+="֮��";
		AfxMessageBox(str);
		return;
	}
	else
	{
		int row=(int)(log10((double)n_total)/log10(2.0)+1); 
		int col=n_total/2;
		int i;
		double **A_val;
		double **D_val;
		
		A_val=new double*[row];
		D_val=new double*[row];
		
		for(i=0;i<row;i++)
		{
			A_val[i]=new double[col];
			D_val[i]=new double[col];
		} 
		//////////////////////////////////////////////////////////////////////
		for(int init=0;init<col;init++)//initialize the matrix,as the 1 level encode,this length is half of the n_total;
		{
			A_val[1][init]=set_A(Src_val[2*init],Src_val[2*init+1]);
			D_val[1][init]=set_D(Src_val[2*init],Src_val[2*init+1]);
		}
		int N=n_total/2;
		row=2;
		for(row=2;row<=levl;row++)//DWT,the level row will been filled;
		{	
			N=N/2;
			for(col=0;col<N;col++)
			{
				A_val[row][col]=set_A(A_val[row-1][2*col],A_val[row-1][2*col+1]);
				D_val[row][col]=set_D(A_val[row-1][2*col],A_val[row-1][2*col+1]);
			}
		}
		//load  the result from the A matrix and D matrix to C matrix;
		int len=(int)(n_total/pow(2.0,levl));//length for the ENcode level;
		int cindex=0;
		for(cindex=0;cindex<len;cindex++)//load the Ak from the A matrix's level row;
			C_val[cindex]=A_val[levl][cindex];
		
			/*for(row=levl;row>=1;row-- )//this part store the A and D;
			{
			len=(int)(n_total/pow(2,row));
			for(col=0;col<len;col++)//load all the Dk from the D matrix;
			C_val[cindex++]=D_val[row][col];
			}*/
		
		for(col=cindex;col<n_total;col++)//this part only store the A;
			C_val[col]=0;
		///////////////////////////////////////////////////////////////////////////////////
		row=(int)(log10((double)n_total)/log10(2.0)+1);

		for(int ii=0;ii<row;ii++) 
		{
			delete []A_val[ii]; 
			delete []D_val[ii];
		}
		delete []A_val; 
		delete []D_val;
	}
	//WatchDate(Src_val,C_val,n_total);
	
}

double CWaveletTransform::set_A(double a, double b)
{
	double REFVAL=pow(2,0.5);
	return (a+b)/REFVAL;
}

double CWaveletTransform::set_D(double a, double b)
{
	double REFVAL=pow(2,0.5);
	return (a-b)/REFVAL;
}


BEGIN_MESSAGE_MAP(CWaveletTransform, CWnd)
END_MESSAGE_MAP()



// CWaveletTransform ��Ϣ�������








/*
	2010.05.20
   ���¶����޸İ汾��ʹ��vector����ʵ������
*/

double CWaveletTransform::Reshape(std::vector<std::pair<double,double> > &ReshapeData) //�Եõ�����������
{
	// 1.��Ҫ���д�����ź�������������
	std::vector< std::pair<double,double> >::iterator begin = ReshapeData.begin();
	std::vector< std::pair<double,double> >::iterator end   = ReshapeData.end();
	std::sort(begin,end);

	// 2.��ƽ������
	size_t n = ReshapeData.size(); 
	double avgDiff = 0.0; 
	for (size_t i=1; i<n; i++)
		avgDiff += ReshapeData[i].first - ReshapeData[i-1].first;
	avgDiff = avgDiff/n;
	


	//���ʱ���������
	//double interval = (2*avgDiff-2*(begin->first))/(n*(n-1));  //ʱ������(�Ȳ����м��)
	//if (dCycle >= 10.0)

	//double Interval = (int)(interval*10+5)/10;  //�Լ����������
	 
	//size_t maxSize = ((ReshapeData.back().first)-(ReshapeData.front().first))/interval+1;
	unsigned rSize = static_cast<unsigned>(((ReshapeData[n-1].first)-(ReshapeData[0].first))/avgDiff);

	if (rSize > n)
		rSize = n;
	std::vector< std::pair<double,double> > TempData(ReshapeData);
	for (unsigned i=1; i<rSize; ++i)
	{
		ReshapeData[i].first = ReshapeData[i-1].first + avgDiff; //TimeData������
		int start = 0;
		int last = 0;
		bool bInsertVal = true;
		for (int j=i; ;j++) //������Բ�ֵ�ķ�Χ
		{
			if (TempData[j].first == ReshapeData[i].first)
			{
				ReshapeData[i].second = TempData[j].second;
				bInsertVal = false;
				break;
			}
			if (TempData[j].first > ReshapeData[i].first)
			{
				last = j;
				start = j - 1;
				break;
			}
		}
		if (bInsertVal)
		{
			double k = (ReshapeData[last].second - ReshapeData[start].second) / avgDiff; //����б��
			double b = ReshapeData[start].second - k * ReshapeData[start].first;
			ReshapeData[i].second = k * ReshapeData[i].first + b;
		}
	}
	return avgDiff;
}
//---------------------------------------------------------------------------------------------------------
//	2010.05.20���޸İ汾
//---------------------------------------------------------------------------------------------------------
double CWaveletTransform::Reshape_v2(std::vector<CSema> &ReshapeData) //�Եõ�����������
{
	// 1.��Ҫ���д�����ź�������������
	std::vector<CSema>::iterator begin = ReshapeData.begin();
	std::vector<CSema>::iterator end   = ReshapeData.end();
	std::sort(begin,end);

	// 2.��ƽ������
	size_t n = ReshapeData.size(); 
	double avgDiff = 0.0; 
	for (size_t i=1; i<n; i++)
		avgDiff += ReshapeData[i].x - ReshapeData[i-1].x;
	avgDiff = avgDiff/n;

	unsigned rSize = static_cast<unsigned>(((ReshapeData[n-1].x)-(ReshapeData[0].x))/avgDiff);

	if (rSize > n)
		rSize = n;
	std::vector<CSema> TempData(ReshapeData);
	for (unsigned i=1; i<rSize; ++i)
	{
		ReshapeData[i].x = ReshapeData[i-1].x + avgDiff; //TimeData������
		int start = 0;
		int last = 0;
		bool bInsertVal = true;
		for (int j=i; ;j++) //������Բ�ֵ�ķ�Χ
		{
			if (TempData[j].x == ReshapeData[i].x)
			{
				ReshapeData[i].y=TempData[j].y;
				bInsertVal = false;
				break;
			}
			if (TempData[j].x> ReshapeData[i].x)
			{
				last = j;
				start = j-1;
				break;
			}
		}
		if (bInsertVal)
		{
			double k = (ReshapeData[last].y - ReshapeData[start].y) / avgDiff; //����б��
			double b = ReshapeData[start].y - k * ReshapeData[start].x;
			ReshapeData[i].y= k * ReshapeData[i].x + b;
		}
	}
	return avgDiff;
}

void CWaveletTransform::MovingAverage(std::vector<std::pair<double,double> >&ReshapeData, int Avgtimes) //���ƶ�ƽ�������˲�
{
	std::vector<std::pair<double,double> >temp(ReshapeData);
	int n = ReshapeData.size();
	std::vector<std::pair<double,double> >::iterator first = ReshapeData.begin();
	std::vector<std::pair<double,double> >::iterator last = ReshapeData.end();
	for (int i=0; i<n - Avgtimes; i++)
	{
		for (int j=1; j<Avgtimes; j++)
		{
			ReshapeData[i].second = ReshapeData[i].second + ReshapeData[i+j].second;
		}
		ReshapeData[i].second = ReshapeData[i].second / Avgtimes;
		first++;
	}
	
	ReshapeData.erase(first, last);
	/*for (int i=n-Avgtimes; i<n; i++)
	{
		TimeData[i] = TimeData[n-Avgtimes];
		VoltageData[i] = VoltageData[n-Avgtimes];
	}*/
}
#include "StdAfx.h"
#include "Peak.h"
#include <cmath>
#include <algorithm>
#include <deque>

 
CSema::CSema():idx(0),x(0.0),y(0.0)
{  
}   
CSema::CSema(int index, double time, double val):idx(index),x(time),y(val)
{
}
bool CSema::operator<(const CSema &s)
{
	return (this->x)<(s.x);
}  
bool CSema::operator==(const CSema &rhs)
{
	return ((this->idx)==rhs.idx);
		//&& ((this->x)-rhs.x==0.00000000) 
		//&& ((this->y)-rhs.y==0.00000000);
}
bool CSema::operator!=(const CSema &rhs)
{
	return !(*this==rhs);
		/*((this->idx)!=rhs.idx)
		|| ((this->x)-rhs.x!=0.00000000) 
		|| ((this->y)-rhs.y!=0.00000000);*/
}
CSema::~CSema()
{
}
CRealTimeSema::CRealTimeSema(int index, double time, double val)
	:CSema(index, time, val)
{ pts = UNKNOWN; }

CRealTimeSema::CRealTimeSema(int index, double time, double val, RealTimeStates rts)
	:CSema(index, time, val)
{ pts = rts; }

CRealTimeSema::CRealTimeSema(const CSema& sema)
	:CSema(sema)
{            }

CPeak::CPeak(void):Type(0),dType(PEAKVALLEY),StartSema(),PeakSema(),FinSema(),HalfPeakWidth(0.0),
	PeakWidth(0.0),PeakArea(0.0),Disfactor(0.0),Tailing(0.0),PeakAreaPercent(0.0),PeakHighPercent(0.0),
	ColumnPlate(0.0),Resolution(0.0)
{
} 
CPeak::CPeak(CSema startSema, CSema peakSema, CSema finSema)
	:StartSema(startSema),PeakSema(peakSema),FinSema(finSema)
{
	Type = 0;
	dType = PEAKVALLEY;
	HalfPeakWidth=0.0;
	PeakWidth=0.0;
	PeakHeight=0.0;
	PeakArea=0.0;
	Disfactor=0.0;
	Tailing=0.0;
	// add
	PeakAreaPercent = 0.0;  //�����%
	PeakHighPercent = 0.0;  //���%
	ColumnPlate = 0.0;      //����������
	Resolution = 0.0;       //�����
}

CPeak::CPeak(int sgn, CSema startSema, CSema peakSema, CSema finSema)
	:Type(sgn),dType(PEAKVALLEY),StartSema(startSema),PeakSema(peakSema),FinSema(finSema)
{
	HalfPeakWidth=0.0;
	PeakWidth=0.0;
	PeakHeight=0.0;
	PeakArea=0.0;
	Disfactor=0.0;
	Tailing=0.0;
	// add
	PeakAreaPercent = 0.0;  //�����%
	PeakHighPercent = 0.0;  //���%
	ColumnPlate = 0.0;      //����������
	Resolution = 0.0;       //�����
}

CPeak::CPeak(int sgn, CSema startSema, CSema peakSema, CSema finSema, double pw, double ph,
			 CSema MstartSema, CSema MpeakSema, CSema MfinSema,  double hpw,  
			 double pa, double df, double tg):
Type(sgn),dType(PEAKVALLEY),StartSema(startSema),PeakSema(peakSema),FinSema(finSema),PeakWidth(pw),PeakHeight(ph),
MidStartSema(MstartSema),MidPeakSema(MpeakSema),MidFinSema(MfinSema),HalfPeakWidth(hpw),
PeakArea(pa),Disfactor(df),Tailing(tg)
{ 
	// add
	PeakAreaPercent = 0.0;  //�����%
	PeakHighPercent = 0.0;  //���%
	ColumnPlate = 0.0;      //����������
	Resolution = 0.0;       //�����
}

bool CPeak::operator==(const CPeak &rhs)
{
	return (this->Type==rhs.Type)  
		&& (this->dType==rhs.dType)  
		&& (this->StartSema==rhs.StartSema)  
		&& (this->PeakSema==rhs. PeakSema)    
		&& (this->FinSema==rhs.FinSema)       
		&& (this->PeakWidth-rhs.PeakWidth==0.00000000)  
		&& (this->PeakHeight-rhs.PeakHeight==0.00000000)      
		&& (this->MidStartSema==rhs.MidStartSema)  
		&& (this->MidPeakSema==rhs.MidPeakSema)  
		&& (this->MidFinSema==rhs.MidFinSema)    
		&& (this->HalfPeakWidth-rhs.HalfPeakWidth==0.00000000)   
		&& (this->PeakArea-rhs.PeakArea==0.00000000)      
		&& (this->Disfactor-rhs.Disfactor==0.00000000)       
		&& (this->Tailing-rhs.Tailing==0.00000000)// add
		&& (this->PeakAreaPercent-rhs.PeakAreaPercent==0.00000000)
		&& (this->PeakHighPercent-rhs.PeakHighPercent==0.00000000)
		&& (this->ColumnPlate-rhs.ColumnPlate==0.00000000)
		&& (this->Resolution-rhs.Resolution==0.00000000);      
}
bool CPeak::operator==(int value)
{
	return (this->Type==value); 
}
bool CPeak::operator!=(const CPeak &rhs)
{
	return !(*this==rhs);
}
bool CPeak::operator< (const CPeak &rhs) const
{
	return this->PeakSema.x < rhs.PeakSema.x;
}
CPeak&  CPeak::operator= (const CPeak& rhs)
{ 
	if (*this!=rhs)
	{
		Type = rhs.Type;          //������,1=����; -1=����; >1�ص�����; <-1�ص�����
		dType = rhs.dType;
		StartSema= rhs.StartSema;     //�����
		PeakSema= rhs.PeakSema;      //�壬��
		FinSema= rhs.FinSema;       //�����
		PeakWidth= rhs.PeakWidth;     //���
		PeakHeight= rhs.PeakHeight;    //���
		MidStartSema= rhs.MidStartSema;  //������
		MidPeakSema= rhs.MidPeakSema;   //��壬��
		MidFinSema= rhs.MidFinSema;    //������
		HalfPeakWidth= rhs.HalfPeakWidth; //����
		PeakArea= rhs.PeakArea;      //�����
		Disfactor= rhs.Disfactor;     //��������
		Tailing= rhs.Tailing;       //��β����
		// add
		PeakAreaPercent= rhs.PeakAreaPercent;  //�����%
		PeakHighPercent= rhs.PeakHighPercent;  //���%
		ColumnPlate= rhs.ColumnPlate;      //����������
		Resolution= rhs.Resolution;       //�����
	}
	return *this;
}
CPeak::~CPeak(void)
{
}

//��÷����͵���������
CString CPeak::CPeakType::ConvertToTypeName(void)
{
	CString strName(_T(""));
	if (negative==1) strName += _T("����/");
	if (single == 1) 
	{
		strName += _T("����/");
		goto CPeakStandard_CPeakType_Lable;
	}
	if (overlap_pl == 1) 
	{
		strName += _T("�ص���/");
		goto CPeakStandard_CPeakType_Lable;
	}
	if (overlap_ls == 1) 
	{
		strName += _T("�ص���(���)/");
		goto CPeakStandard_CPeakType_Lable;
	} 
	if (overlap_rs == 1) 
	{
		strName += _T("�ص���(�Ҽ�)/");
		goto CPeakStandard_CPeakType_Lable;
	}
	if (overlap_vs == 1) 
	{
		strName += _T("�ص���(��ֱ�ַ�)/");
		goto CPeakStandard_CPeakType_Lable;
	}

CPeakStandard_CPeakType_Lable:
	if (tail == 1) 
	{
		strName += _T("��β");
	}
	 
	if (strName.IsEmpty()) //�����ܳ��ֵ����
	{
		CString str(_T(""));
		return str;
	}

	//���û����β������֡�����/�ص���(��ֱ�ַ�)/��ʱ�������һ��/ɾ��
	if (strName[strName.GetLength()-1]=='/') 
	{
		strName.Delete(strName.GetLength()-1);
	}

	return strName;
}

void CPeak::CPeakType::GetFromTypeName(CString cStrName)
{
	cStrName += _T("/");
	//�����ַ���
	while (!cStrName.IsEmpty())
	{
		int iMark = cStrName.Find(_T("/"));
		CString szToken = cStrName.Left(iMark);
		if (szToken==_T("����")) negative=1;
		if (szToken==_T("����")) single=1;
		if (szToken==_T("�ص���")) overlap_pl=1;
		if (szToken==_T("�ص���(���)")) overlap_ls=1;
		if (szToken==_T("�ص���(�Ҽ�)")) overlap_rs=1;
		if (szToken==_T("�ص���(��ֱ�ַ�)")) overlap_vs=1;
		if (szToken==_T("��β")) tail=1;
		cStrName = cStrName.Right(cStrName.GetLength() - iMark -1);
	}
}

//ȫ�ֺ���
//-----------------------------------------------------------------------------
//���ܣ��������ź�������ƽ����
//���������
//			1.vector<CSema> &handleSema ��������ź���
//			2.avg ƽ������
//���������vector<CSema> &avgData ƽ�����õ��ź���
//-----------------------------------------------------------------------------			
void new_Averaging(const std::vector<CSema> &handleSema, //��������ź���
				   std::vector<CSema> &avgData,		     //ƽ�����õ��ź���
				   unsigned avg )					//ƽ������
{//step1: ����avgΪ1���ƽ�����ߣ�ʣ���������Ϊ1��
	if (avg==0)
	{
		avgData = handleSema;
		return;
	}
	const unsigned handleSemaSize = handleSema.size();
	for (unsigned i=0; i<handleSemaSize; ++i)
	{
		double avgTime = 0.0;
		double avgSema = 0.0;
		if (i % avg == 0 && i+avg < handleSemaSize) 
		{
			for (unsigned j=i; j< i+avg; j++)
			{
				avgTime += handleSema[j].x;
				avgSema += handleSema[j].y;
			}
			avgTime = avgTime / avg;
			avgSema = avgSema / avg;
			CSema OneSema(i, avgTime, avgSema);
			avgData.push_back(OneSema);
		}
		if ((handleSemaSize-avg*avgData.size())/avg<1) //ʣ��
		{
			for (unsigned j=avg*avgData.size(); j<handleSemaSize; ++j)
			{ 
				avgTime += handleSema[j].x;    
				avgSema += handleSema[j].y;  
			}
			avgTime = avgTime/(handleSemaSize-avg*avgData.size()+1);
			avgSema = avgSema/(handleSemaSize-avg*avgData.size()+1);
			CSema OneSema(avg*avgData.size(), avgTime, avgSema);
			avgData.push_back(OneSema);
			break;
		}
	}
}
//-----------------------------------------------------------------------------
//���ܣ���������ź���������֮��б��
//���������
//			1.vector<CSema> &avgDataƽ�����õ��ź���
//���������
//			1.vector<double> &firstOrder б�ʴ������
//			2.maxSlope ƽ�����õ��ź��������б��
//-----------------------------------------------------------------------------
void new_GetAvgDataSlope(const std::vector<CSema> &avgData, //ƽ�����õ��ź���
						 std::vector<double> &firstOrder,	//б�ʴ������
						 double &maxSlope)					//���б��            
{//step2: ���ƽ��������ÿһ���б��
	//�������N�����ݣ�����������N-1�����
	//���һ������û������б��
	const unsigned avgDataSize = avgData.size()-1;
	double *k = new double[avgDataSize];
	for (unsigned i=0; i<avgDataSize; ++i)
	{	
		double ydiff = avgData[i+1].y - avgData[i].y;
		double xdiff = avgData[i+1].x - avgData[i].x;
		k[i] = (ydiff) / (xdiff);
		if (k[i]>=maxSlope)
			maxSlope = k[i];
		firstOrder.push_back(k[i]);
	}
	delete [] k;
	k = NULL;
}
//-----------------------------------------------------------------------------
//���ܣ���������ź�������㲨���յ�
//���������
//			1.vector<CSema> &avgDataƽ�����õ��ź���
//			2.vector<double> &firstOrder ƽ�����õ��ź�����б��
//			3.slopeBased ����б��
//			4.hasNagetiveSpike �������
//			5.isTwiceJudge �����ж�
//���������
//			1.vector<CPeak> &Peak �ź�������㲨���յ�
//-----------------------------------------------------------------------------
void new_GetpeakSearch(const std::vector<CSema> &avgData,   //ƽ�����õ��ź���
					   const std::vector<double> &firstOrder,//ƽ�����õ��ź�����б��
					   std::vector<CPeak> &Peak,    //(���)�������
					   double slopeBased, //����б��
					   bool hasNagetiveSpike = true,//��������������Ƿ��и������
					   bool isTwiceJudge = true)   
					    
{// step3: ������������˳��Ѱ��
	enum enumState {
		STATELESS,			//��״̬
		UPCOMING_WAVE,		//���в�
		DOWNGOING_WAVE,		//���в�
		HANDLE_ACCOMPLISH   //�������
	} hState;				//��������еı��

	const unsigned avgDataSize = avgData.size();
	unsigned i=0, j=0, k=0;
	unsigned getStartPoint=0, getpeakPoint=0, getFinishPoint=0;
	int mark = 1; //�����ͱ��: 1=����; -1=����; 2,3,4,...Ϊ�ص�����; -2,-3,-4...Ϊ�ص�����
	bool isNagetiveSpike = false;
	hState = STATELESS;
	while (hState != HANDLE_ACCOMPLISH)
	{
		switch (hState)
		{
		case STATELESS:  //��״̬
			for ( ; i<avgDataSize-1; ++i)
			{
				if (i==avgDataSize-2)
				{
					hState = HANDLE_ACCOMPLISH;
					break;
				}
				 
				if (firstOrder[i]<0) //���忼�����
				{
					if (hasNagetiveSpike)
					{
						if (fabs(firstOrder[i])<slopeBased)
							continue;
						if (fabs(firstOrder[i])>=slopeBased)
						{
							if (!isTwiceJudge) //����������2���ж�
							{
								isNagetiveSpike = true;
								getStartPoint = i;
								hState = DOWNGOING_WAVE;
								break;
							}
							if (fabs(firstOrder[i+1])<slopeBased)
								continue;
							if (fabs(firstOrder[i+1])>=slopeBased)
							{
								isNagetiveSpike = true;
								getStartPoint = i;
								hState = DOWNGOING_WAVE;
								break;
							}
						}
					}
					else
						continue;
				}	

				if (firstOrder[i]>=0) //�Ǹ��忼�����
				{
					if (firstOrder[i]<slopeBased)
						continue;
					if (firstOrder[i]>slopeBased)
					{
						if (!isTwiceJudge) //����������2���ж�
						{
							getStartPoint = i;
							hState = UPCOMING_WAVE;
							break;
						}
						if (firstOrder[i+1]<slopeBased)
							continue;
						if (firstOrder[i+1]>slopeBased)
						{
							getStartPoint = i;
							hState = UPCOMING_WAVE;
							break;
						}
					}
				}
			}
			break;

		case UPCOMING_WAVE: //���в�
			if (!isNagetiveSpike)
			{
				for (j=i+1; j<avgDataSize-1; ++j)
				{
					if (j==avgDataSize-2)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[j]<0)
						continue;
					if (firstOrder[j]>=0)
					{
						if (firstOrder[j+1]>0)
							continue;
						if (firstOrder[j+1]<0)
						{
							getpeakPoint = j+1;
							hState = DOWNGOING_WAVE;
							break;
						}
					}
				}
			}
			else
			{
				for (j=k+1; j<avgDataSize-1; ++j)
				{
					if (j==avgDataSize-2)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[j]>=0)
					{
						if (firstOrder[j]>slopeBased)
							continue;
						if (firstOrder[j]<slopeBased ) 
						{
							if (!isTwiceJudge) //����������2���ж�
							{
								getFinishPoint = j+1;
								hState = STATELESS;
								i = j;
								break;
							}
							if (firstOrder[j+1]<slopeBased)
							{
								getFinishPoint = j+1;
								hState = STATELESS;
								i = j;
								break;
							}
						}
					}
					if (firstOrder[j]<0)
					{
						getFinishPoint = j;
						hState = STATELESS;
						i = j-1;
						break;
					}

				}
				if (hState==STATELESS)
				{
					CPeak OnePeak(avgData[getStartPoint],avgData[getpeakPoint],avgData[getFinishPoint]);
					Peak.push_back(OnePeak);

					unsigned t = Peak.size()-1;
					if (t>1 && (Peak[t].StartSema.x-Peak[t-1].FinSema.x)==0.000000)
					{
						Peak[t-1].Type = -mark;// ֮ǰ�ı�־��Ϊ��������
						Peak[t].Type = -mark;  //��������
					}
					else
					{
						Peak[t].Type = -1; //��������
						mark++;
					}


					isNagetiveSpike = false;

				}
			}
			break;

		case DOWNGOING_WAVE: //���в�
			if (!isNagetiveSpike)
			{
				for (k = j+1; k<avgDataSize-1; ++k)
				{
					if (k==avgDataSize-2)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if ( firstOrder[k]<0 ) 
					{
						if (fabs(firstOrder[k])>slopeBased)
							continue;

						if ( fabs(firstOrder[k])<slopeBased )
						{
							if (!isTwiceJudge) //����������2���ж�
							{
								getFinishPoint = k+1;
								hState = STATELESS;
								i = k;
								break;
							}
							if ( fabs(firstOrder[k+1])<slopeBased/* && firstOrder[k]<0*/)
							{
								getFinishPoint = k+1;
								hState = STATELESS;
								i = k;
								break;
							}
							if (fabs(firstOrder[k+1])>slopeBased)
								continue;

						}
					}
					if ( firstOrder[k]>0 )
					{
						getFinishPoint = k;
						hState = STATELESS;
						i = k;
						break;
					}
				}
				if (hState==STATELESS)
				{
					CPeak OnePeak(avgData[getStartPoint],avgData[getpeakPoint],avgData[getFinishPoint]);
					Peak.push_back(OnePeak);

					unsigned t = Peak.size()-1;

					if (t>1 && (Peak[t].StartSema.x-Peak[t-1].FinSema.x)==0.000000)
					{
						Peak[t-1].Type = mark;	// ֮ǰ�ı�־��Ϊ��������
						Peak[t].Type = mark;	//��������
					}
					else
					{
						Peak[t].Type = 1; //��������
						mark++;
					}


				}
			}
			else
			{
				for (k = i+1; k<avgDataSize-1; ++k)
				{
					if (k==avgDataSize-2)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[k]>0)
						continue;
					if (firstOrder[k]<=0)
					{
						if (firstOrder[k+1]<0)
							continue;
						if (firstOrder[k+1]>0)
						{
							getpeakPoint = k+1;
							hState = UPCOMING_WAVE;
							break;
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
}


//-----------------------------------------------------------------------------
//���ܣ�����������ź�������㲨���յ㻹ԭ��Ԫ������
//���������
//			1.vector<CSema> &handleSema Ԫ�����ź���
//			2.avg ƽ������
//			3.slopeBased ����б��
//���������
//			1.vector<CPeak> &Peak ��ԭ���ź�������㲨���յ�
//-----------------------------------------------------------------------------
void new_RestorePeakData(const std::vector<CSema> &handleSema, //����ԭ���ź���
						 std::vector<CPeak> &Peak,
						 const int avg,
						 const double slopeBased)  

{// step4: ������ĵ㻹ԭ��Ԫ����
	const unsigned handleSemaSize = handleSema.size();
	const unsigned peakSemaSize = Peak.size();

	for (unsigned i=0; i<peakSemaSize; ++i)  //����ÿһ����
	{
		for (unsigned j=Peak[i].StartSema.idx; j<Peak[i].StartSema.idx+avg && j<handleSemaSize-1; j++) //�Ȼ�ԭ���
		{
			double k=(handleSema[j+1].y-handleSema[j].y)/(handleSema[j+1].x-handleSema[j].x);
			if (fabs(k)>slopeBased)
			{
				if (j==Peak[i].StartSema.idx+avg-1)
				{
					Peak[i].StartSema = handleSema[j];
					break;
				}
			}
			else
			{
				Peak[i].StartSema = handleSema[j];
				break;
			}
		}

		for (unsigned j=Peak[i].FinSema.idx; j<Peak[i].FinSema.idx+avg && j<handleSemaSize-1; j++) //�ٻ�ԭ���
		{
			double k=(handleSema[j+1].y-handleSema[j].y)/(handleSema[j+1].x-handleSema[j].x);
			if (fabs(k)>slopeBased)
			{
				if (j==Peak[i].FinSema.idx+avg-1)
				{
					Peak[i].FinSema = handleSema[j];
					break;
				}
			}
			else
			{
				Peak[i].FinSema = handleSema[j];
				break;
			}
		}

		//���ԭ������߲��ȵ�
		double maxValue = -INT_MAX;
		double minValue = INT_MAX;
		if (Peak[i].Type>=1) //��ԭ����
		{
			for (unsigned j=Peak[i].StartSema.idx; j<Peak[i].FinSema.idx; ++j)
			{
				if (maxValue <= handleSema[j].y)
				{
					maxValue = handleSema[j].y;
					Peak[i].PeakSema = handleSema[j];
				}
			}
		}
		else //��ԭ����
		{
			for (unsigned j=Peak[i].StartSema.idx; j<Peak[i].FinSema.idx; ++j)
			{
				if (minValue >= handleSema[j].y)
				{
					minValue = handleSema[j].y;
					Peak[i].PeakSema = handleSema[j];	
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------
//���ܣ���ֱ�ַ��㷨
//���������
//			1.vector<CPeak> &Peak �Զ����Peak���ͱ���
//���������
//			1.vector<CPeak> &Peak ��ֱ�ַ��Ĳ�
//-----------------------------------------------------------------------------
void new_DividePeak(std::vector<CPeak> &Peak)          
{// ��ֱ�ַ�
	//std::vector<std::pair<double, double> > TempStart;    //�������
	//std::vector<std::pair<double, double> > TempFinish;   //�������     

	const unsigned PeakSize = Peak.size();
	unsigned bg=0;
	unsigned ed=bg;
	bool getIndex = false; //��ü����±�
	for (; bg<PeakSize; ++bg)
	{
		if (Peak[bg].Type==1 || Peak[bg].Type==-1) //�������, �������κθı�
		{
			continue;
		}
		else //����������±�
		{
			ed = bg+1;
			int mark = Peak[bg].Type;
			for (; ed<PeakSize; ++ed)
			{
				if (ed+1==PeakSize && Peak[PeakSize-1].Type==mark)
				{
					getIndex = true;
					break;
				}
				if (Peak[ed].Type!=mark)
				{
					ed--;
					getIndex = true;
					break;
				}
			}
		}

		if (getIndex) //���������
		{
			double k = (Peak[ed].FinSema.y-Peak[bg].StartSema.y)/(Peak[ed].FinSema.x-Peak[bg].StartSema.x);
			double c = Peak[bg].StartSema.y-k*Peak[bg].StartSema.x;
			double tmp=Peak[bg].StartSema.y;
			for (;bg!=ed; ++bg)
			{
				double _y = Peak[bg].FinSema.x*k+c;
				if ((Peak[bg].Type>1 && _y > Peak[bg].FinSema.y) || (Peak[bg].Type<-1 && _y < Peak[bg].FinSema.y))
				{
					//Peak[bg].StartSema.y = tmp;//
					//Peak[bg+1].Type = Peak[bg+1].Type/abs(Peak[bg+1].Type);

					if (bg+1!=ed)
						++bg;
				}
				else
				{
					Peak[bg].FinSema.y = _y;
					tmp = Peak[bg+1].StartSema.y;
					Peak[bg+1].StartSema.y = _y;
				}
			}
			getIndex = false;
		}	
	}
}
//-----------------------------------------------------------------------------
//���ܣ���ֱ�ַ�����У��
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.hasNagetiveSpike �������
//���������
//			1.vector<CPeak> &Peak ����У����Ĳ�
//-----------------------------------------------------------------------------
void new_BaselineAdjust(const std::vector<CSema> &handleSema,
					   std::vector<CPeak> &Peak,
					   bool hasNagetiveSpike = true) //��������������Ƿ��и������
{
	//��ֱ�ַ����У��
	const unsigned PeakSize = Peak.size();
	unsigned bg=0;
	unsigned ed=bg;
	for (; bg<PeakSize; ++bg)
	{
		if (abs(Peak[bg].Type)==1) //�������
		{
			ed = bg;
		}
		else				  //�ط����
		{
			ed = bg+1;
			int mark = Peak[bg].Type;
			for (; ed<PeakSize; ++ed)
			{
				if (ed+1==PeakSize)
				{
					break;
				}
				if (Peak[ed].Type!=mark)
				{
					ed--;
					break;
				}
			}
		}
		unsigned s = Peak[bg].StartSema.idx;
		unsigned f = Peak[ed].FinSema.idx;
		//���У��
		bool isfail = true;
		for (; s<=Peak[bg].PeakSema.idx && isfail; ++s)
		{
			double k = (handleSema[f].y-handleSema[s].y)/(handleSema[f].x-handleSema[s].x);
			double c = handleSema[s].y-k*handleSema[s].x;
			for (unsigned m=s; m<=Peak[bg].PeakSema.idx; ++m) 
			{
				double _y = k * handleSema[m].x+c;
				if (hasNagetiveSpike) //���Ǹ������
				{
					if (fabs(_y) > fabs(handleSema[m].y))
					{
						isfail = true;
						break;
					}
				}
				else
				{
					if (fabs(_y) > fabs(handleSema[m].y) && _y>0)
					{
						isfail = true;
						break;
					}
					else if(fabs(_y) < fabs(handleSema[m].y) && _y<=0)
					{
						isfail = true;
						break;
					}
				}
				isfail = false;
			}
			if (!isfail)
			{
				//�������
				Peak[bg].StartSema = handleSema[s];
				isfail = false;
			}
		}isfail = true;	//���У��
		for (; f>=Peak[ed].PeakSema.idx && isfail; --f)
		{
			double k = (handleSema[f].y-handleSema[s].y)/(handleSema[f].x-handleSema[s].x);
			double c = handleSema[f].y-k*handleSema[f].x;
			for (unsigned n=f; n>=Peak[ed].PeakSema.idx; --n)
			{
				double _y = k * handleSema[n].x + c;
				if (hasNagetiveSpike) //���Ǹ������
				{
					if (fabs(_y) > fabs(handleSema[n].y))
					{
						isfail = true;
						break;
					}
				}
				else
				{
					if (fabs(_y) > fabs(handleSema[n].y) && _y>0)
					{
						isfail = true;
						break;
					}
					else if(fabs(_y) < fabs(handleSema[n].y) && _y<=0)
					{
						isfail = true;
						break;
					}
				}
				isfail = false;				
			}
			if (!isfail)
			{
				//�������
				Peak[ed].FinSema = handleSema[f];
				isfail = false;
			}
		}
	bg = ed;
	}
}
//-----------------------------------------------------------------------------
//���ܣ��Ǵ�ֱ�ַ�����У��
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.hasNagetiveSpike �������
//���������
//			1.vector<CPeak> &Peak ����У����Ĳ�
//-----------------------------------------------------------------------------
void new_BaselineCheak(const std::vector<CSema> &handleSema,
					   std::vector<CPeak> &Peak,
					   bool hasNagetiveSpike = true) //��������������Ƿ��и������
{
	// �Ǵ�ֱ�ַ�
	const unsigned PeakSize = Peak.size();
	unsigned bg=0;
	unsigned ed=bg;
	bool getIndex = false; //��ü����±�

	for (; bg<PeakSize; ++bg)
	{
		if (Peak[bg].Type==1 || Peak[bg].Type==-1) //�������
		{
			ed = bg;
			getIndex = true;
		}

		else //�ص�����
		{
			ed = bg+1;
			int mark = Peak[bg].Type;
			for (; ed<PeakSize; ++ed)
			{
				if (ed+1==PeakSize)
				{
					getIndex = true;
					break;
				}
				if (Peak[ed].Type!=mark)
				{
					ed--;
					getIndex = true;
					break;
				}
			}
		}
		if (getIndex) 
		{
			unsigned i = bg;
			unsigned o = ed;
			unsigned j = Peak[i].StartSema.idx;
			do 
			{
				bool isfail = true;
				for (j=Peak[i].StartSema.idx; j<=Peak[i].PeakSema.idx && isfail; ++j)	
				{
					//---------------------------------------------------------
					//if (j==Peak[i].PeakSema.idx)
					//{
					//	//startSema[i] = (handleSema[sIndex[i]]); //����һ��
					//	Peak[i].StartSema = handleSema[Peak[i].StartSema.idx];
					//	isfail = false;
					//	break;
					//}
					//---------------------------------------------------------
					double k = (handleSema[Peak[i].FinSema.idx].y-handleSema[j].y)/(handleSema[Peak[i].FinSema.idx].x-handleSema[j].x);
					//double k = (handleSema[fIndex[i]].second - handleSema[j].second)/(handleSema[fIndex[i]].first - handleSema[j].first);
					double c = handleSema[j].y-k*handleSema[j].x;
					//double c = handleSema[j].second - k * handleSema[j].first;
					for (unsigned m=j; m<=Peak[i].PeakSema.idx; ++m) 
					{
						double _y = k*handleSema[m].x+c;
						if (hasNagetiveSpike) //���Ǹ������
						{
							if (getIndex && fabs(_y) > fabs(handleSema[m].y))
							{
								isfail = true;
								break;
							}
						}
						else
						{
							if (getIndex && fabs(_y) > fabs(handleSema[m].y) && _y>0)
							{
								isfail = true;
								break;
							}
							else if(getIndex && fabs(_y) < fabs(handleSema[m].y) && _y<=0)
							{
								isfail = true;
								break;
							}
						}
						isfail = false;
					}
					if (!isfail)
					{
						//startSema[i] = handleSema[j]; //�������
						//sIndex[i] = j;
						Peak[i].StartSema = handleSema[j];
						isfail = false;
					}
				} isfail = true;
				for (unsigned l=Peak[i].FinSema.idx; l>=Peak[i].PeakSema.idx && isfail; --l)
				{
					//---------------------------------------------------------
					//if (l==Peak[i].PeakSema.idx)
					//{
					//	//finishSema[i]=handleSema[fIndex[i]]; //����һ��
					//	Peak[i].FinSema = handleSema[Peak[i].FinSema.idx];
					//	break;
					//}
					//---------------------------------------------------------
					double k = (handleSema[l].y-handleSema[j].y)/(handleSema[l].x-handleSema[j].x);
					//double k = (handleSema[l].second-handleSema[j].second)/(handleSema[l].first-handleSema[j].first);
					double c = handleSema[l].y-k*handleSema[l].x;
					//double b = handleSema[l].second - k * handleSema[l].first;
					for (unsigned n=l; n>=Peak[i].PeakSema.idx; --n)
					{
						double _y = k * handleSema[n].x + c;
						if (hasNagetiveSpike) //���Ǹ������
						{
							if (getIndex && fabs(_y) > fabs(handleSema[n].y))//
							{
								isfail = true;
								break;
							}
						}
						else
						{
							if (getIndex && fabs(_y) > fabs(handleSema[n].y) && _y>0)//
							{
								isfail = true;
								break;
							}
							else if(getIndex && fabs(_y) < fabs(handleSema[n].y) && _y<=0)//
							{
								isfail = true;
								break;
							}
						}
						isfail = false;				
					}
					if (!isfail)
					{
						//finishSema[i]=handleSema[l]; //�������
						//fIndex[i]=l;
						Peak[i].FinSema = handleSema[l];
						isfail = false;
					}
				}
				i++;	
			} while (i!=o+1);
			getIndex = false;
		}
		bg = ed;
	}
	//if (startSema.size()!=peakSema.size()||finishSema.size()!=peakSema.size())
	//{	
	//	::AfxMessageBox(_T("������󣬴��ڸ���������빴ѡ���Ǹ������"));
	//	ASSERT(0);
	//}
}
//-----------------------------------------------------------------------------
//���ܣ�����У����Բ����͵ļ��
//���������
//			1.vector<CPeak> &Peak ����鲨���͵Ĳ�
//���������
//			1.vector<CPeak> &Peak �����͵ļ���Ĳ�
//-----------------------------------------------------------------------------
void new_PeakTypeChange(std::vector<CPeak> &Peak)
{
	const unsigned PeakSize = Peak.size();
	if (PeakSize==0) return;
	for (unsigned i=0; i<PeakSize-1; ++i)
	{
		if (  (Peak[i].dType == CPeak::PLUSTANGET)
			||(Peak[i].dType == CPeak::MINUSTANGET))
		{
			continue;
		}

		if (abs(Peak[i].Type)!=1) 
		{
			if (Peak[i].FinSema==Peak[i+1].StartSema)
			{
				continue;
			}
			else /*if (Peak[i].FinSema!=Peak[i+1].StartSema)*/
			{
				if (i!=0)
				{
					if (Peak[i].StartSema==Peak[i-1].FinSema)
					{
						continue;
					}
					if (Peak[i].Type==0)
						continue;

					Peak[i].Type = Peak[i].Type/abs(Peak[i].Type);
				}
			}
		}
	}
}
void new_GetPeakWidth(const std::vector<CSema> &handleSema,
					   std::vector<CPeak> &Peak,
					   bool isDividePeak)
{
	const unsigned PeakSize = Peak.size();
	const int factor = 1; //Y��ͳһ����
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = Peak[i].StartSema.x;
		double y1 = Peak[i].StartSema.y;
		double x2 = Peak[i].FinSema.x;
		double y2 = Peak[i].FinSema.y;

		double peakWidth = sqrt((y2/factor-y1/factor)*(y2/factor-y1/factor)+(x2-x1)*(x2-x1));
		Peak[i].PeakWidth = peakWidth;  //���

		double tana = (y2-y1)/(x2-x1);
		double c = y1 - x1 * tana;

		double py = Peak[i].PeakSema.y; 
		double px = Peak[i].PeakSema.x;
		double ty = px*tana+c;

		double peakHeight = fabs(fabs(ty)-fabs(py));
		Peak[i].PeakHeight = peakHeight; //���

		double PeakWidthLeft =  sqrt((ty/factor-y1/factor)*(ty/factor-y1/factor)+(px-x1)*(px-x1));  
		double PeakWidthRight = sqrt((y2/factor-ty/factor)*(y2/factor-ty/factor)+(x2-px)*(x2-px));
		double lr = PeakWidthLeft/PeakWidthRight;
		Peak[i].Tailing = lr; //��β����

		double hx = px;
		double hy = (py-ty)/2 + ty;
		double _c = hy-hx*tana;


		bool getHalfStart = false;
		bool getHalfFin = false;
		if (isDividePeak)
		{
			unsigned StartIdx = Peak[i].StartSema.idx;
			unsigned StartIdxNext = Peak[i].StartSema.idx+1;
			double gapPre_start = fabs(handleSema[StartIdx].y);
			double gapNext_start = fabs(handleSema[StartIdxNext].y);
			double gap_start = fabs(handleSema[StartIdx].x * tana + _c);
			if (gap_start>=gapPre_start && gap_start<=gapNext_start) //��������ڷַ�����
			{
				CSema temp(StartIdx,Peak[i].StartSema.x, gap_start); //������	
				Peak[i].MidStartSema = temp;
				getHalfStart = true;
			}

			unsigned FinIdx = Peak[i].FinSema.idx-1;
			unsigned FinIdxNext = Peak[i].StartSema.idx;
			double gapPre_fin = fabs(handleSema[FinIdx].y);
			double gapNext_fin = fabs(handleSema[FinIdxNext].y);
			double gap_fin = fabs(handleSema[FinIdx].x * tana + _c);
			if (gap_fin>=gapPre_fin && gap_fin<=gapNext_fin) //��������ڷַ�����
			{
				CSema temp(FinIdxNext, Peak[i].FinSema.x, gap_fin); //������	
				Peak[i].MidFinSema = temp;
				getHalfStart = true;
			}
		}

		double diff_y = INT_MAX;
		unsigned idxl, idxr;
		if (!getHalfStart)
		{
			for (unsigned l=Peak[i].StartSema.idx; l<=Peak[i].PeakSema.idx; ++l) //���У��ƽ��е�
			{
				double ly = handleSema[l].x * tana + _c;
				if (diff_y >= fabs(handleSema[l].y - ly))
				{
					diff_y = fabs(handleSema[l].y - ly);
					idxl = l;
				}
			}
			CSema tmp(handleSema[idxl]); //������
			Peak[i].MidStartSema = tmp;
		}
		diff_y = INT_MAX;
		if (!getHalfFin)
		{
			for (unsigned r=Peak[i].PeakSema.idx; r<=Peak[i].FinSema.idx; ++r) //���У��ƽ��е�
			{
				double ry = handleSema[r].x * tana + _c;
			
				if (diff_y >= fabs(handleSema[r].y - ry))
				{
					diff_y = fabs(handleSema[r].y - ry);
					idxr = r;
				}
			}
			CSema tmp(handleSema[idxr]);   //������
			Peak[i].MidFinSema = tmp;
		}
		double hk = (Peak[i].MidFinSema.y-Peak[i].MidStartSema.y)/(Peak[i].MidFinSema.x-Peak[i].MidStartSema.x);
		double hc = Peak[i].MidStartSema.y - Peak[i].MidStartSema.x * hk;
		double rey = hx*hk+hc;
		CSema HalfPeak(Peak[i].PeakSema.idx, hx, rey); // ����е�
		Peak[i].MidPeakSema = HalfPeak;
		//CPeak OneHalf(HalfStart, HalfPeak, HalfFin);
		//Half.push_back(OneHalf);  //�������
	}

	//���Ӱ���ֱ�ߣ��벨��Ĵ����ཻ��һ�㡣���������ǰ�󳤶�
	//const unsigned HalfSize = Half.size();
	//ASSERT(HalfSize==PeakSize);
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = Peak[i].MidStartSema.x; 
		double y1 = Peak[i].MidStartSema.y;
		double x2 = Peak[i].MidFinSema.x; 
		double y2 = Peak[i].MidFinSema.y;
		double halfPeakWidth = sqrt((y2/factor-y1/factor)*(y2/factor-y1/factor)+(x2-x1)*(x2-x1));
		Peak[i].HalfPeakWidth = halfPeakWidth;  //�����

		double x0 = Peak[i].MidPeakSema.x;      //�����ǰ�󳤶�
		double y0 = Peak[i].MidPeakSema.y;
		double halfPeakWidthLeft =  sqrt((y0/factor-y1/factor)*(y0/factor-y1/factor)+(x0-x1)*(x0-x1));
		double halfPeakWidthRight = sqrt((y2/factor-y0/factor)*(y2/factor-y0/factor)+(x2-x0)*(x2-x0));
		double disfactor = halfPeakWidthLeft/halfPeakWidthRight;
		Peak[i].Disfactor = disfactor;  //���������
	}
}

//-----------------------------------------------------------------------------
//���ܣ���ò��ķ��������ߡ��������ӡ���β���ӵȸ�������
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak Ҫ��ø��������Ĳ�
//			3.isDividePeak �Ƿ�ֱ�ַ�
//���������
//			1.vector<CPeak> &Peak ��ø���������Ĳ�
//-----------------------------------------------------------------------------
void new_GetPeakWidth_v2(const std::vector<CSema> &handleSema,
						 std::vector<CPeak> &Peak,
						 bool isDividePeak)
{
	const unsigned PeakSize = Peak.size();
	//const int factor = 1; //Y��ͳһ����
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = Peak[i].StartSema.x;
		double y1 = Peak[i].StartSema.y;
		double x2 = Peak[i].FinSema.x;
		double y2 = Peak[i].FinSema.y;

		//double peakWidth = sqrt((y2/factor-y1/factor)*(y2/factor-y1/factor)+(x2-x1)*(x2-x1));
		double peakWidth = fabs(x2-x1);
		Peak[i].PeakWidth = peakWidth;  //���

		double tana = (y2-y1)/(x2-x1);
		double c = y1 - x1 * tana;

		double py = Peak[i].PeakSema.y; 
		double px = Peak[i].PeakSema.x;
		double ty = px*tana+c;

		double peakHeight = fabs(fabs(ty)-fabs(py));
		Peak[i].PeakHeight = peakHeight; //���

		//double PeakWidthLeft =  sqrt((ty/factor-y1/factor)*(ty/factor-y1/factor)+(px-x1)*(px-x1));  
		//double PeakWidthRight = sqrt((y2/factor-ty/factor)*(y2/factor-ty/factor)+(x2-px)*(x2-px));
		double PeakWidthLeft  = fabs(px-x1);
		double PeakWidthRight = fabs(x2-px);
		double lr = PeakWidthLeft/PeakWidthRight;
		Peak[i].Tailing = lr; //��β����

		double hx = px;
		double hy = (py-ty)/2 + ty;
		double _c = hy-hx*tana;


		bool getHalfStart = false;
		bool getHalfFin = false;
		if (isDividePeak)
		{
			unsigned StartIdx = Peak[i].StartSema.idx;
			unsigned StartIdxNext = Peak[i].StartSema.idx+1;
			double gapPre_start = fabs(handleSema[StartIdx].y);
			double gapNext_start = fabs(handleSema[StartIdxNext].y);
			double gap_start = fabs(handleSema[StartIdx].x * tana + _c);
			if (gap_start>=gapPre_start && gap_start<=gapNext_start) //��������ڷַ�����
			{
				CSema temp(StartIdx,Peak[i].StartSema.x, gap_start); //������	
				Peak[i].MidStartSema = temp;
				getHalfStart = true;
			}

			unsigned FinIdx = Peak[i].FinSema.idx-1;
			unsigned FinIdxNext = Peak[i].StartSema.idx;
			double gapPre_fin = fabs(handleSema[FinIdx].y);
			double gapNext_fin = fabs(handleSema[FinIdxNext].y);
			double gap_fin = fabs(handleSema[FinIdx].x * tana + _c);
			if (gap_fin>=gapPre_fin && gap_fin<=gapNext_fin) //��������ڷַ�����
			{
				CSema temp(FinIdxNext, Peak[i].FinSema.x, gap_fin); //������	
				Peak[i].MidFinSema = temp;
				getHalfStart = true;
			}
		}

		double diff_y = INT_MAX;
		unsigned idxl, idxr;
		if (!getHalfStart)
		{
			for (unsigned l=Peak[i].StartSema.idx; l<=Peak[i].PeakSema.idx; ++l) //���У��ƽ��е�
			{
				double ly = handleSema[l].x * tana + _c;
				if (diff_y >= fabs(handleSema[l].y - ly))
				{
					diff_y = fabs(handleSema[l].y - ly);
					idxl = l;
				}
			}
			CSema tmp(handleSema[idxl]); //������
			Peak[i].MidStartSema = tmp;
		}
		diff_y = INT_MAX;
		if (!getHalfFin)
		{
			for (unsigned r=Peak[i].PeakSema.idx; r<=Peak[i].FinSema.idx; ++r) //���У��ƽ��е�
			{
				double ry = handleSema[r].x * tana + _c;

				if (diff_y >= fabs(handleSema[r].y - ry))
				{
					diff_y = fabs(handleSema[r].y - ry);
					idxr = r;
				}
			}
			CSema tmp(handleSema[idxr]);   //������
			Peak[i].MidFinSema = tmp;
		}
		double hk = (Peak[i].MidFinSema.y-Peak[i].MidStartSema.y)/(Peak[i].MidFinSema.x-Peak[i].MidStartSema.x);
		double hc = Peak[i].MidStartSema.y - Peak[i].MidStartSema.x * hk;
		double rey = hx*hk+hc;
		CSema HalfPeak(Peak[i].PeakSema.idx, hx, rey); // ����е�
		Peak[i].MidPeakSema = HalfPeak;
		//CPeak OneHalf(HalfStart, HalfPeak, HalfFin);
		//Half.push_back(OneHalf);  //�������
	}

	//���Ӱ���ֱ�ߣ��벨��Ĵ����ཻ��һ�㡣���������ǰ�󳤶�
	//const unsigned HalfSize = Half.size();
	//ASSERT(HalfSize==PeakSize);
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = Peak[i].MidStartSema.x; 
		double y1 = Peak[i].MidStartSema.y;
		double x2 = Peak[i].MidFinSema.x; 
		double y2 = Peak[i].MidFinSema.y;
		//double halfPeakWidth = sqrt((y2/factor-y1/factor)*(y2/factor-y1/factor)+(x2-x1)*(x2-x1));
		double halfPeakWidth = fabs(x2-x1);
		Peak[i].HalfPeakWidth = halfPeakWidth;  //�����

		double x0 = Peak[i].MidPeakSema.x;      //�����ǰ�󳤶�
		double y0 = Peak[i].MidPeakSema.y;
		//double halfPeakWidthLeft =  sqrt((y0/factor-y1/factor)*(y0/factor-y1/factor)+(x0-x1)*(x0-x1));
		//double halfPeakWidthRight = sqrt((y2/factor-y0/factor)*(y2/factor-y0/factor)+(x2-x0)*(x2-x0));
		double halfPeakWidthLeft =  fabs(x0-x1);
		double halfPeakWidthRight = fabs(x2-x0);
		double disfactor = halfPeakWidthLeft/halfPeakWidthRight;
		Peak[i].Disfactor = disfactor;  //���������
	}
}
//-----------------------------------------------------------------------------
//���ܣ���ò��ķ����
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak Ҫ��÷�����Ĳ�		
//���������
//			1.vector<CPeak> &Peak ��÷������Ĳ�
//-----------------------------------------------------------------------------
void new_GetPeakArea(const std::vector<CSema> &handleSema,
					 std::vector<CPeak> &Peak)
{
	const unsigned PeakSize = Peak.size();
	const unsigned handleSemaSize = handleSema.size();
	unsigned j = 0;
	for (unsigned i=0; i<PeakSize; ++i)  
	{
		double x2 = Peak[i].FinSema.x;           
		double y2 = Peak[i].FinSema.y;
		double x1 = Peak[i].StartSema.x;
		double y1 = Peak[i].StartSema.y;
		double k = (y2-y1)/(x2-x1);
		double c = y1-k*x1;

		double area = 0.0;
		for (; j<handleSemaSize; ++j)
		{
			if (handleSema[j].x-Peak[i].StartSema.x >= 0.000000) //�ҵ����
			{	
				do 
				{
					double x4 = handleSema[j+1].x;
					double y4 = handleSema[j+1].y;
					double x3 = handleSema[j].x;
					double y3 = handleSema[j].y;

					double h = x4 - x3;
					double up = fabs(fabs(y3)-fabs(x3*k+c));
					double down = fabs(fabs(y4)-fabs(x4*k+c));
					area += (up+down)*h/2;
					j++;
				} while (handleSema[j].x-Peak[i].FinSema.x <= 0.000000); //�ҵ��յ�
				Peak[i].PeakArea = area;
				break;
			}
		}
	}
}
//-----------------------------------------------------------------------------
//���ܣ�ɾ���ȸ����������С�Ĳ���
//���������		
//			1.vector<CPeak> &Peak ������Ĳ�
//			2.halfPeakWidth �����������
//���������
//			1.vector<CPeak> &Peak ɾ���ȸ����������С�Ĳ����Ĳ�
//-----------------------------------------------------------------------------
void new_ErasePeakByHalfWidth(std::vector<CPeak> &Peak,
							  const double halfPeakWidth)
{
	const unsigned PeakSize = Peak.size();
	std::vector<CPeak> Temp; //��ʱ����
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (Peak[i].HalfPeakWidth - halfPeakWidth >= 0.000000)
		{
			Temp.push_back(Peak[i]);
		}
	}
	Peak.swap(Temp);
	//ɾ���ı������
	const unsigned SwapPeakSize = Peak.size();
	typedef std::vector<CPeak>::iterator CI;
	for (CI it=Peak.begin(); it!=Peak.end(); ++it)
	{
		if (abs(it->Type)!=1)
		{
			int times = count(Peak.begin(), Peak.end(), *it);
			if (times==1)
			{
				it->Type = (it->Type)/abs(it->Type); //�����ͱ�Ϊ����
			}
		}
	}
	
}
//-----------------------------------------------------------------------------
//���ܣ�ɾ���ȸ������������С�Ĳ���
//���������		
//			1.vector<CPeak> &Peak ������Ĳ�
//			2.halfPeakWidth �������������
//���������
//			1.vector<CPeak> &Peak ɾ���ȸ������������С�Ĳ����Ĳ�
//-----------------------------------------------------------------------------
void new_ErasePeakByPeakArea(std::vector<CPeak> &Peak,
							 const double peakArea)
{
	const unsigned PeakSize = Peak.size();
	std::vector<CPeak> Temp; //��ʱ����
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (Peak[i].PeakArea >= peakArea)
		{
			Temp.push_back(Peak[i]);
		}
	}
	Peak.swap(Temp);
	//ɾ���ı������
	const unsigned SwapPeakSize = Peak.size();
	typedef std::vector<CPeak>::iterator CI;
	for (CI it=Peak.begin(); it!=Peak.end(); ++it)
	{
		if (abs(it->Type)!=1)
		{
			//int PeakType = it->Type;
			//int times = count(Peak.begin(), Peak.end(), it->Type);
			int times=0;
			for (unsigned i=0; i<SwapPeakSize; ++i)
			{
				if (Peak[i].Type==(it->Type))
					times++;
			}
			if (times==1)
			{
				it->Type = (it->Type)/abs(it->Type); //�����ͱ�Ϊ����
			}
		}
	}
}
//-----------------------------------------------------------------------------
//���ܣ���û��ߵı�ǵ�
//���������		
//			1.vector<CPeak> &Peak �������ߵĲ�
//���������
//			1.vector<CSema> &TagPoint ��û��ߵı�ǵ������
//-----------------------------------------------------------------------------
void new_GetTagPoint(const std::vector<CPeak> &Peak,
					 std::vector<CSema> &TagPoint)
{
	if (Peak.empty())
	{
		return;
	}
	const unsigned PeakSize = Peak.size();
	double diff_h=fabs(Peak[0].PeakHeight); //��ǵ��
	for (unsigned i=0; i<PeakSize; ++i) //����С���
	{
		if (diff_h>=fabs(Peak[i].PeakHeight))
		{
			diff_h=fabs(Peak[i].PeakHeight);
		}
	}
	diff_h = diff_h*0.15; //�����Ϊ���30%
	for (unsigned i=0; i<PeakSize; ++i)
	{
		//diff_h = Peak[i].PeakHeight*0.05;  //�����Ϊ���10%
		CSema maeue(Peak[i].StartSema.idx, Peak[i].StartSema.x, Peak[i].StartSema.y+diff_h);
		TagPoint.push_back(maeue);
		CSema maeshita(Peak[i].StartSema.idx, Peak[i].StartSema.x, Peak[i].StartSema.y-diff_h);
		TagPoint.push_back(maeshita);
		CSema ushiroue(Peak[i].FinSema.idx, Peak[i].FinSema.x, Peak[i].FinSema.y+diff_h);
		TagPoint.push_back(ushiroue);
		CSema ushiroshita(Peak[i].FinSema.idx, Peak[i].FinSema.x, Peak[i].FinSema.y-diff_h);
		TagPoint.push_back(ushiroshita);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//  �����õ��㷨
//----------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------
VOID test_GetDividePeakPoint(std::vector<CPeak> &Peak, unsigned bg, unsigned ed)
{
	if (bg==ed)
	{
		return;
	}

	//��Ǵ�ֱ�ַ�
	for (unsigned i=bg; i<=ed; ++i)
		Peak[i].dType = CPeak::VERTDIVIDE;

	double k = (Peak[ed].FinSema.y-Peak[bg].StartSema.y)/(Peak[ed].FinSema.x-Peak[bg].StartSema.x);
	double c = Peak[bg].StartSema.y-k*Peak[bg].StartSema.x;
	double tmp=Peak[bg].StartSema.y;
	for (;bg!=ed; ++bg)
	{
		

		double _y = Peak[bg].FinSema.x*k+c;
		//if ((Peak[bg].Type>1 && _y > Peak[bg].FinSema.y) || (Peak[bg].Type<-1 && _y < Peak[bg].FinSema.y))
		//{
		//	//Peak[bg].StartSema.y = tmp;//
		//	//Peak[bg+1].Type = Peak[bg+1].Type/abs(Peak[bg+1].Type);

		//	if (bg+1!=ed)
		//		++bg;
		//}
		//else
		//{
			Peak[bg].FinSema.y = _y;
			tmp = Peak[bg+1].StartSema.y;
			Peak[bg+1].StartSema.y = _y;
		//}
	}
}
//-----------------------------------------------------------------------------
//���ܣ���ֱ�ַ��㷨
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak �Զ����Peak���ͱ���
//���������
//			1.vector<CPeak> &Peak ��ֱ�ַ��Ĳ�
//-----------------------------------------------------------------------------
VOID test_DividePeak(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak)  //��д��ֱ�ַ��㷨        
{// ��ֱ�ַ�    

	const unsigned PeakSize = Peak.size();
	unsigned bg=0;
	unsigned ed=bg;
	bool getIndex = false; //��ü����±�
	for (; bg<PeakSize; ++bg)
	{
		if (Peak[bg].Type==1 || Peak[bg].Type==-1) //�������, �������κθı�
		{
			continue; //֮���㷨���л���У��
		}
		else //����������±�
		{
			ed = bg+1;
			int mark = Peak[bg].Type;
			for (; ed<PeakSize; ++ed)
			{
				if (ed+1==PeakSize && Peak[PeakSize-1].Type==mark)
				{
					getIndex = true;
					break;
				}
				if (Peak[ed].Type!=mark)
				{
					ed--;
					getIndex = true;
					break;
				}
			}
		}

		if (getIndex) //���������
		{
			unsigned bgPeak = bg;
			unsigned edPeak = ed;
			bool finish = false;
loop:
			while ((ed!=bg) || (!finish))
			{
				if (bg==ed)
				{
					Peak[bg].Type = 1;
				}
				
				double k = (Peak[ed].FinSema.y-Peak[bg].StartSema.y)/(Peak[ed].FinSema.x-Peak[bg].StartSema.x);
				double c = Peak[bg].StartSema.y-k*Peak[bg].StartSema.x;
				for (unsigned i=Peak[bg].StartSema.idx; i<Peak[ed].FinSema.idx; ++i)
				{
					double line_y = handleSema[i].x*k+c;
					if (line_y> handleSema[i].y && (bg!=ed)) //��ǰ֮�����������
					{
						ed--;
						break; //�����ϵ���������ϵĵ㣬�������һ����
					}
					if (i==Peak[ed].FinSema.idx-2 /*|| (bg+1==ed)*/ || (bg==ed))
					{
						test_GetDividePeakPoint(Peak, bg, ed);//��ǰ������Ļ���
						if (edPeak==ed)
						{
							//������ϣ�
							finish = true;
							bg = ed;
							break;
						}
						else
						{
							bg = ed+1;
							ed = edPeak;
							goto loop;
						}
					}
				}
			}
			getIndex = false;
		}	
	}
}
//-----------------------------------------------------------------------------
//���ܣ�ɾ������2��������ĳɷ�
//���������
//			1.vector<CPeak> &Peak �Զ����Peak���ͱ���
//���������
//			1.vector<CPeak> &Peak ɾ������2��������ĳɷ��Ĳ�
//-----------------------------------------------------------------------------
VOID test_ErasePeakByNextto(std::vector<CPeak> &Peak)
{
	
	std::vector<CPeak> Tmp;
	const unsigned PeakSize = Peak.size();
	if (PeakSize==0) return;
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (  (Peak[i].StartSema.idx==Peak[i].PeakSema.idx)
			||(Peak[i].StartSema.idx+1==Peak[i].PeakSema.idx)
			||(Peak[i].StartSema.idx+2==Peak[i].PeakSema.idx)
			||(Peak[i].PeakSema.idx==Peak[i].FinSema.idx)
			||(Peak[i].PeakSema.idx+1==Peak[i].FinSema.idx)
			||(Peak[i].PeakSema.idx+2==Peak[i].FinSema.idx))
		{
			continue;
		}
		else
			Tmp.push_back(Peak[i]);
	}
	Peak.swap(Tmp);
	//ɾ���ı������
	const unsigned SwapPeakSize = Peak.size();
	typedef std::vector<CPeak>::iterator CI;
	for (CI it=Peak.begin(); it!=Peak.end(); ++it)
	{
		if (abs(it->Type)!=1)
		{
			//int PeakType = it->Type;
			int times = count(Peak.begin(), Peak.end(), it->Type);
			//int times=0;
			//for (unsigned i=0; i<SwapPeakSize; ++i)
			//{
			//	if (Peak[i].Type==(it->Type))
			//		times++;
			//}
			if (times==1)
			{
				it->Type = (it->Type)/abs(it->Type); //�����ͱ�Ϊ����
			}
		}
	}
}


VOID test_GetpeakSearch(const std::vector<CSema> &avgData,   //ƽ�����õ��ź���
					   const std::vector<double> &firstOrder,//ƽ�����õ��ź�����б��
					   std::vector<CPeak> &Peak,    //(���)�������
					   double slopeBased, //����б��
					   bool hasNagetiveSpike = true,//��������������Ƿ��и������
					   bool isTwiceJudge = true)   

{// step3: ������������˳��Ѱ��
	enum enumState {
		STATELESS,			//��״̬
		UPCOMING_WAVE,		//���в�
		DOWNGOING_WAVE,		//���в�
		HANDLE_ACCOMPLISH   //�������
	} hState;				//��������еı��

	const unsigned avgDataSize = avgData.size();
	unsigned i=0, j=0, k=0;
	unsigned getStartPoint=0, getpeakPoint=0, getFinishPoint=0;
	int mark = 1; //�����ͱ��: 1=����; -1=����; 2,3,4,...Ϊ�ص�����; -2,-3,-4...Ϊ�ص�����
	bool isNagetiveSpike = false;
	bool processed = false;
	hState = STATELESS;
	const int series = 2;
	while (hState != HANDLE_ACCOMPLISH)
	{
		switch (hState)
		{
		case STATELESS:  //��״̬
			for ( ; i<avgDataSize-series; ++i)  //
			{
				if (i==avgDataSize-series-1) //
				{
					hState = HANDLE_ACCOMPLISH;
					break;
				}

				if (firstOrder[i]<0) //���忼�����
				{
					if (hasNagetiveSpike)
					{
						if (fabs(firstOrder[i])<slopeBased)
							continue;
						if (fabs(firstOrder[i])>=slopeBased)
						{
							if (!isTwiceJudge) //����������2���ж�
							{
								isNagetiveSpike = true;
								getStartPoint = i;
								hState = DOWNGOING_WAVE;
								break;
							}
							if (fabs(firstOrder[i+1])<slopeBased)
								continue;
							if (fabs(firstOrder[i+1])>=slopeBased)
							{
								isNagetiveSpike = true;
								getStartPoint = i;
								hState = DOWNGOING_WAVE;
								break;
							}
						}
					}
					else
						continue;
				}	

				if (firstOrder[i]>=0) //�Ǹ��忼�����
				{
					if (firstOrder[i]<slopeBased)
						continue;
					if (firstOrder[i]>slopeBased)
					{
						if (!isTwiceJudge) //����������2���ж�
						{
							getStartPoint = i;
							hState = UPCOMING_WAVE;
							break;
						}
						if (firstOrder[i+1]<slopeBased)
							continue;
						if (firstOrder[i+1]>slopeBased)
						{
							getStartPoint = i;
							hState = UPCOMING_WAVE;
							break;
						}
					}
				}
			}
			break;

		case UPCOMING_WAVE: //���в�
			if (!isNagetiveSpike)
			{
				for (j=i+1; j<avgDataSize-1; ++j)
				{
					if (j==avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[j]<0)
						continue;
					if (firstOrder[j]>=0)
					{
						if (firstOrder[j+1]>0)
							continue;
						if (firstOrder[j+1]<0 && firstOrder[j+2]<0) //2010.08.25 ��ӶԲ������Ĵ���
						{
							getpeakPoint = j+1;
							hState = DOWNGOING_WAVE;
							break;
						}
					}
				}
			}
			else
			{
				for (j=k+1; j<avgDataSize-1; ++j)
				{
					if (j==avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[j]>=0)
					{
						if (firstOrder[j]>slopeBased)  //2010.08.25 ���ƽͷ�崦������
						{	
							processed = true;
							continue;
						}
						if (processed && firstOrder[j]<slopeBased ) 
						{
							if (!isTwiceJudge) //����������2���ж�
							{
								getFinishPoint = j+1;
								hState = STATELESS;
								i = j;
								break;
							}
							if (firstOrder[j+1]<slopeBased)
							{
								getFinishPoint = j+1;
								hState = STATELESS;
								i = j;
								break;
							}
						}
					}
					if (firstOrder[j]<0)
					{
						getFinishPoint = j;
						hState = STATELESS;
						i = j-1;
						break;
					}

				}
				if (hState==STATELESS)
				{
					processed = false;

					CPeak OnePeak(avgData[getStartPoint],avgData[getpeakPoint],avgData[getFinishPoint]);
					Peak.push_back(OnePeak);

					unsigned t = Peak.size()-1;
					if (t>1 && (Peak[t].StartSema.x-Peak[t-1].FinSema.x)==0.000000)
					{
						Peak[t-1].Type = -mark;// ֮ǰ�ı�־��Ϊ��������
						Peak[t].Type = -mark;  //��������
					}
					else
					{
						Peak[t].Type = -1; //��������
						mark++;
					}


					isNagetiveSpike = false;

				}
			}
			break;

		case DOWNGOING_WAVE: //���в�
			if (!isNagetiveSpike)
			{
				for (k = j+1; k<avgDataSize-1; ++k)
				{
					if (k==avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if ( firstOrder[k]<0 ) 
					{
						if (fabs(firstOrder[k])>slopeBased)  //2010.08.24 ���ƽͷ�崦������
						{
							processed = true;
							continue;
						}

						if ( processed && (fabs(firstOrder[k]) < slopeBased) && (firstOrder[k]!=0.000000))
						{
							if (!isTwiceJudge) //����������2���ж�
							{
								getFinishPoint = k+1;
								hState = STATELESS;
								i = k;
								break;
							}
							if ( fabs(firstOrder[k+1])<slopeBased && (firstOrder[k+1]!=0.000000)/* && firstOrder[k]<0*/)
							{
								getFinishPoint = k+1;
								hState = STATELESS;
								i = k;
								break;
							}
							if (fabs(firstOrder[k+1])>slopeBased)
								continue;

						}
					}
					if ( firstOrder[k]>0 )
					{
						getFinishPoint = k;
						hState = STATELESS;
						i = k;
						break;
					}
				}
				if (hState==STATELESS)
				{
					processed = false;
					CPeak OnePeak(avgData[getStartPoint],avgData[getpeakPoint],avgData[getFinishPoint]);
					Peak.push_back(OnePeak);

					unsigned t = Peak.size()-1;

					if (t>1 && (Peak[t].StartSema.x-Peak[t-1].FinSema.x)==0.000000)
					{
						Peak[t-1].Type = mark;	// ֮ǰ�ı�־��Ϊ��������
						Peak[t].Type = mark;	//��������
					}
					else
					{
						Peak[t].Type = 1; //��������
						mark++;
					}
				}
			}
			else
			{
				for (k = i+1; k<avgDataSize-1; ++k)
				{
					if (k==avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[k]>0)
						continue;
					if (firstOrder[k]<=0)
					{
						if (firstOrder[k+1]<0)
							continue;
						if (firstOrder[k+1]>0)
						{
							getpeakPoint = k+1;
							hState = UPCOMING_WAVE;
							break;
						}
					}
				}
			}
			break;
		default:
			break;
		}
	}
}



//-----------------------------------------------------------------------------
//���ܣ���������ź�������㲨���յ���ڲ��������ж�
//���������
//			1.vector<CSema> &avgDataƽ�����õ��ź���
//			2.vector<double> &firstOrder ƽ�����õ��ź�����б��
//			3.slopeBased ����б��
//			4.hasNagetiveSpike �������
//			5.series �ڲ�����N���ж�
//���������
//			1.vector<CPeak> &Peak �ź�������㲨���յ�
//-----------------------------------------------------------------------------
VOID test_GetpeakSearch_Series(const std::vector<CSema> &avgData,   //ƽ�����õ��ź���
								const std::vector<double> &firstOrder,//ƽ�����õ��ź�����б��
								std::vector<CPeak> &Peak,    //(���)�������
								double slopeBased, //����б��
								bool hasNagetiveSpike,//��������������Ƿ��и������
								const int series = 2 )   

{// step3: ������������˳��Ѱ��
	enum enumState {
		STATELESS,			//��״̬
		UPCOMING_WAVE,		//���в�
		DOWNGOING_WAVE,		//���в�
		HANDLE_ACCOMPLISH   //�������
	} hState;				//��������еı��

	const unsigned avgDataSize = avgData.size();
	unsigned i=0, j=0, k=0;
	unsigned getStartPoint=0, getpeakPoint=0, getFinishPoint=0;
	int mark = 1; //�����ͱ��: 1=����; -1=����; 2,3,4,...Ϊ�ص�����; -2,-3,-4...Ϊ�ص�����
	bool isNagetiveSpike = false;
	bool processed = false;
	hState = STATELESS;
	while (hState != HANDLE_ACCOMPLISH)
	{
		switch (hState)
		{
		case STATELESS:  //��״̬
			for ( ; i<avgDataSize-series && hState==STATELESS; ++i)  //
			{
				if (i>=avgDataSize-series-1) //
				{
					hState = HANDLE_ACCOMPLISH;
					break;
				}

				if (firstOrder[i]<=0)
				{//�������
					if (hasNagetiveSpike) //���忼�����
					{
						if (fabs(firstOrder[i])<slopeBased)
							continue;
						if (fabs(firstOrder[i])>=slopeBased)
						{
							for (int s=1; s<=series; ++s)
							{
								if (fabs(firstOrder[i+s])<slopeBased)
									break;
								if (fabs(firstOrder[i+s])>slopeBased)
								{
									if (s==series)
									{
										isNagetiveSpike = true;
										getStartPoint = i;
										hState = DOWNGOING_WAVE;
										break;
									}
								}
							}
						}
					}
				else
					continue;
				}	
			
				if (firstOrder[i]>=0) //�Ǹ��忼�����
				{
					if (firstOrder[i]>slopeBased)
					{
						for (int s=1; s<=series; ++s)
						{
							if (fabs(firstOrder[i+s])<slopeBased)
								break;
							if (fabs(firstOrder[i+s])>=slopeBased)
							{
								if (s==series)
								{
									getStartPoint = i;
									hState = UPCOMING_WAVE;
									break;
								}
							}
						}
					}
				}
			}
		break;

		case UPCOMING_WAVE: //���в�
			if (!isNagetiveSpike)
			{
				for (j=i/*-1*/; j<avgDataSize-1 && hState==UPCOMING_WAVE; ++j)
				{
					if (j>=avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}

					if (firstOrder[j]<0)
						continue;
					if (firstOrder[j]>=0)
					{
						for (int s=1; s<=series; ++s)
						{
							if (firstOrder[j+s]>0)//
								break;
							if (firstOrder[j+s]<0)
							{
								if (s==series)
								{
									getpeakPoint = j+s;
									hState = DOWNGOING_WAVE;
									break;
								}
							}
						}
					}
				}
			}
			else
			{//�������
				for (j=k+1; j<avgDataSize-1 && hState==UPCOMING_WAVE; ++j)
				{
					if (j>=avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[j]>=0)
					{
						if (firstOrder[j]>slopeBased)  //2010.08.25 ���ƽͷ�崦������
						{	
							processed = true;
							continue;
						}
						if (processed && firstOrder[j]<slopeBased && (firstOrder[j]!=0.000000)) 
						{
							for (int s=1; s<=series; ++s)
							{
								if (firstOrder[j+s]>slopeBased)
									break;
								if (firstOrder[j+s]<slopeBased  && (firstOrder[j+s]!=0.000000))
								{
									if (s==series)
									{
										getFinishPoint = j+1;
										hState = STATELESS;
										i = j;
										break;
									}
								}
							}
						}
					}
					if (firstOrder[j]<0)
					{
						getFinishPoint = j;
						hState = STATELESS;
						i = j;  // modify i=j-1;
						break;
					}

				}
				if (hState==STATELESS)
				{
					processed = false;

					CPeak OnePeak(avgData[getStartPoint],avgData[getpeakPoint],avgData[getFinishPoint]);
					Peak.push_back(OnePeak); //����

					unsigned t = Peak.size()-1;
					if (t>1 && (Peak[t].StartSema.x-Peak[t-1].FinSema.x)==0.000000)
					{
						Peak[t-1].Type = -mark;// ֮ǰ�ı�־��Ϊ��������
						Peak[t].Type = -mark;  //��������
					}
					else
					{
						Peak[t].Type = -1; //��������
						mark++;
					}
					isNagetiveSpike = false;
				}
			}
			break;

		case DOWNGOING_WAVE: //���в�
			if (!isNagetiveSpike)
			{
				for (k=j/*+1*/; k<avgDataSize-1 && hState==DOWNGOING_WAVE; ++k)
				{
					if (k>=avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					
					if ( firstOrder[k]<0 ) 
					{
						if (fabs(firstOrder[k])>slopeBased)  //2010.08.24 ���ƽͷ�崦������
						{
							processed = true;
							continue;
						}

						if (processed && (fabs(firstOrder[k]) < slopeBased) && (firstOrder[k]!=0.000000))
						{
							for (int s=1; s<=series; ++s)
							{
								if (fabs(firstOrder[k+s])>slopeBased)
									break;
								if (fabs(firstOrder[k+s])<slopeBased && (firstOrder[k+s]!=0.000000))
								{
									if (s==series)
									{
										getFinishPoint = k+1;
										hState = STATELESS;
										i = k;
										break;
									}
								}
							}
							
						}
					}
					if ( firstOrder[k]>0 )
					{
						getFinishPoint = k;
						hState = STATELESS;
						i = k;
						break;
					}
				}
				if (hState==STATELESS)
				{
					processed = false;
					CPeak OnePeak(avgData[getStartPoint],avgData[getpeakPoint],avgData[getFinishPoint]);
					Peak.push_back(OnePeak);  //����

					unsigned t = Peak.size()-1;

					if (t>1 && (Peak[t].StartSema.x-Peak[t-1].FinSema.x)==0.000000)
					{
						Peak[t-1].Type = mark;	// ֮ǰ�ı�־��Ϊ��������
						Peak[t].Type = mark;	//��������
					}
					else
					{
						Peak[t].Type = 1; //��������
						mark++;
					}
				}
			}
			else
			{//���岨��
				for (k=i+1; k<avgDataSize-1 && hState==DOWNGOING_WAVE; ++k)
				{
					if (k>=avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[k]>0)
						continue;

					if (firstOrder[k]<=0)
					{
						for (int s=1; s<=series; ++s)
						{
							if (firstOrder[k+s]<0)
								break;
							if (firstOrder[k+s]>0)
							{
								if (s==series)
								{
									getpeakPoint = k+1;
									hState = UPCOMING_WAVE;
									break;
								}
							}
						}
					}
				}
			}
		break;

		default:
			break;
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------
// ׷��3���¹���
//----------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//���ܣ��Ե�������л���У��
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak �Զ����Peak���ͱ���
//			3.PeakNo �����
//			4.hasNagetiveSpike �������
//���������
//			1.vector<CPeak> &Peak �Ե�������л���У����Ĳ�
//-----------------------------------------------------------------------------
VOID one_BaselineCheak(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, const int PeakNo, bool hasNagetiveSpike = true)
{
	bool isfail = true;
	unsigned i = PeakNo;
	Peak[i].Type = 1;
	unsigned j = 0;
	for (unsigned j=Peak[i].StartSema.idx; j<=Peak[i].PeakSema.idx && isfail; ++j)	
	{
		double k = (handleSema[Peak[i].FinSema.idx].y-handleSema[j].y)/(handleSema[Peak[i].FinSema.idx].x-handleSema[j].x);
		double c = handleSema[j].y-k*handleSema[j].x;
		for (unsigned m=j; m<=Peak[i].PeakSema.idx; ++m) 
		{
			double _y = k*handleSema[m].x+c;
			if (hasNagetiveSpike) //���Ǹ������
			{
				if (/*getIndex && */fabs(_y) > fabs(handleSema[m].y))
				{
					isfail = true;
					break;
				}
			}
			else
			{
				if (/*getIndex && */fabs(_y) > fabs(handleSema[m].y) && _y>0)
				{
					isfail = true;
					break;
				}
				else if(/*getIndex && */fabs(_y) < fabs(handleSema[m].y) && _y<=0)
				{
					isfail = true;
					break;
				}
			}
			isfail = false;
		}
		if (!isfail)
		{
			//�������
			Peak[i].StartSema = handleSema[j];
			isfail = false;
		}
	} isfail = true;
	for (unsigned l=Peak[i].FinSema.idx; l>=Peak[i].PeakSema.idx && isfail; --l)
	{
		double k = (handleSema[l].y-handleSema[j].y)/(handleSema[l].x-handleSema[j].x);
		double c = handleSema[l].y-k*handleSema[l].x;
		for (unsigned n=l; n>=Peak[i].PeakSema.idx; --n)
		{
			double _y = k * handleSema[n].x + c;
			if (hasNagetiveSpike) //���Ǹ������
			{
				if (/*getIndex && */fabs(_y) > fabs(handleSema[n].y))//
				{
					isfail = true;
					break;
				}
			}
			else
			{
				if (/*getIndex && */fabs(_y) > fabs(handleSema[n].y) && _y>0)//
				{
					isfail = true;
					break;
				}
				else if(/*getIndex && */fabs(_y) < fabs(handleSema[n].y) && _y<=0)//
				{
					isfail = true;
					break;
				}
			}
			isfail = false;				
		}
		if (!isfail)
		{
			//�������
			Peak[i].FinSema = handleSema[l];
		}
	}
};
//-----------------------------------------------------------------------------
//���ܣ��Ե������÷��
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak �Զ����Peak���ͱ���
//			3.PeakNo �����
//			4.isDividePeak ��ֱ�ַ�
//���������
//			1.vector<CPeak> &Peak �Ե������÷���Ĳ�
//-----------------------------------------------------------------------------
int one_GetPeakWidth( const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, const int PeakNo, bool isDividePeak=false)
{
	//const unsigned PeakSize = Peak.size();
	////const int factor = 1; //Y��ͳһ����
	//for (unsigned i=0; i<PeakSize; ++i)
	//{
		unsigned i = PeakNo;

		double x1 = Peak[i].StartSema.x;
		double y1 = Peak[i].StartSema.y;
		double x2 = Peak[i].FinSema.x;
		double y2 = Peak[i].FinSema.y;
		double peakWidth = fabs(x2-x1);
		Peak[i].PeakWidth = peakWidth;  //���
		double tana = (y2-y1)/(x2-x1);
		double c = y1 - x1 * tana;
		double py = Peak[i].PeakSema.y; 
		double px = Peak[i].PeakSema.x;
		double ty = px*tana+c;
		double peakHeight = fabs(fabs(ty)-fabs(py));
		Peak[i].PeakHeight = peakHeight; //���
		double PeakWidthLeft  = fabs(px-x1);
		double PeakWidthRight = fabs(x2-px);
		double lr = PeakWidthLeft/PeakWidthRight;
		Peak[i].Tailing = lr; //��β����
		double hx = px;
		double hy = (py-ty)/2 + ty;
		double _c = hy-hx*tana;
		bool getHalfStart = false;
		bool getHalfFin = false;
		if (isDividePeak)
		{
			unsigned StartIdx = Peak[i].StartSema.idx;
			unsigned StartIdxNext = Peak[i].StartSema.idx+1;
			double gapPre_start = fabs(handleSema[StartIdx].y);
			double gapNext_start = fabs(handleSema[StartIdxNext].y);
			double gap_start = fabs(handleSema[StartIdx].x * tana + _c);
			if (gap_start>=gapPre_start && gap_start<=gapNext_start) //��������ڷַ�����
			{
				CSema temp(StartIdx,Peak[i].StartSema.x, gap_start); //������	
				Peak[i].MidStartSema = temp;
				getHalfStart = true;
			}

			unsigned FinIdx = Peak[i].FinSema.idx-1;
			unsigned FinIdxNext = Peak[i].StartSema.idx;
			double gapPre_fin = fabs(handleSema[FinIdx].y);
			double gapNext_fin = fabs(handleSema[FinIdxNext].y);
			double gap_fin = fabs(handleSema[FinIdx].x * tana + _c);
			if (gap_fin>=gapPre_fin && gap_fin<=gapNext_fin) //��������ڷַ�����
			{
				CSema temp(FinIdxNext, Peak[i].FinSema.x, gap_fin); //������	
				Peak[i].MidFinSema = temp;
				getHalfStart = true;
			}
		}

		double diff_y = INT_MAX;
		unsigned idxl, idxr;
		bool getidxl = false;
		bool getidxr = false;
		if (!getHalfStart)
		{
			for (unsigned l=Peak[i].StartSema.idx; l<=Peak[i].PeakSema.idx; ++l) //���У��ƽ��е�
			{
				double ly = handleSema[l].x * tana + _c;
				if (diff_y >= fabs(handleSema[l].y - ly))
				{
					diff_y = fabs(handleSema[l].y - ly);
					idxl = l;
					getidxl = true;
				}
			}
			if (getidxl)
			{
				CSema tmp(handleSema[idxl]); //������
				Peak[i].MidStartSema = tmp;
			}
		}
		diff_y = INT_MAX;
		if (!getHalfFin)
		{
			
			for (unsigned r=Peak[i].PeakSema.idx; r<=Peak[i].FinSema.idx; ++r) //���У��ƽ��е�
			{
				double ry = handleSema[r].x * tana + _c;

				if (diff_y >= fabs(handleSema[r].y - ry))
				{
					diff_y = fabs(handleSema[r].y - ry);
					idxr = r;
					getidxr = true;
				}
			}
			if (getidxr)
			{
				CSema tmp(handleSema[idxr]);   //������
				Peak[i].MidFinSema = tmp;
			}
		}
		if (!getidxl && !getidxr)
		{
			::AfxMessageBox(_T("������������������������"));
			return -1;
		}


		double hk = (Peak[i].MidFinSema.y-Peak[i].MidStartSema.y)/(Peak[i].MidFinSema.x-Peak[i].MidStartSema.x);
		double hc = Peak[i].MidStartSema.y - Peak[i].MidStartSema.x * hk;
		double rey = hx*hk+hc;
		CSema HalfPeak(Peak[i].PeakSema.idx, hx, rey); // ����е�
		Peak[i].MidPeakSema = HalfPeak;
		//CPeak OneHalf(HalfStart, HalfPeak, HalfFin);
		//Half.push_back(OneHalf);  //�������
	

	//���Ӱ���ֱ�ߣ��벨��Ĵ����ཻ��һ�㡣���������ǰ�󳤶�
	//const unsigned HalfSize = Half.size();
	//ASSERT(HalfSize==PeakSize);
	//for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = Peak[i].MidStartSema.x; 
		double y1 = Peak[i].MidStartSema.y;
		double x2 = Peak[i].MidFinSema.x; 
		double y2 = Peak[i].MidFinSema.y;
		//double halfPeakWidth = sqrt((y2/factor-y1/factor)*(y2/factor-y1/factor)+(x2-x1)*(x2-x1));
		double halfPeakWidth = fabs(x2-x1);
		Peak[i].HalfPeakWidth = halfPeakWidth;  //�����

		double x0 = Peak[i].MidPeakSema.x;      //�����ǰ�󳤶�
		double y0 = Peak[i].MidPeakSema.y;
		//double halfPeakWidthLeft =  sqrt((y0/factor-y1/factor)*(y0/factor-y1/factor)+(x0-x1)*(x0-x1));
		//double halfPeakWidthRight = sqrt((y2/factor-y0/factor)*(y2/factor-y0/factor)+(x2-x0)*(x2-x0));
		double halfPeakWidthLeft =  fabs(x0-x1);
		double halfPeakWidthRight = fabs(x2-x0);
		double disfactor = halfPeakWidthLeft/halfPeakWidthRight;
		Peak[i].Disfactor = disfactor;  //���������
	}
	return 0;
}
//-----------------------------------------------------------------------------
//���ܣ��Ե������÷����
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak �Զ����Peak���ͱ���
//			3.PeakNo �����
//���������
//			1.vector<CPeak> &Peak �Ե������÷������Ĳ�
//-----------------------------------------------------------------------------
VOID one_GetPeakArea(const std::vector<CSema> &handleSema,std::vector<CPeak> &Peak, const int PeakNo)
{
	const unsigned PeakSize = Peak.size();
	const unsigned handleSemaSize = handleSema.size();
	unsigned j = 0;
	/*for (unsigned i=0; i<PeakSize; ++i)  
	{*/
	unsigned i = PeakNo;
	double x2 = Peak[i].FinSema.x;           
	double y2 = Peak[i].FinSema.y;
	double x1 = Peak[i].StartSema.x;
	double y1 = Peak[i].StartSema.y;
	double k = (y2-y1)/(x2-x1);
	double c = y1-k*x1;

	double area = 0.0;
	for (; j<handleSemaSize; ++j)
	{
		if (handleSema[j].x-Peak[i].StartSema.x >= 0.000000) //�ҵ����
		{	
			do 
			{
				double x4 = handleSema[j+1].x;
				double y4 = handleSema[j+1].y;
				double x3 = handleSema[j].x;
				double y3 = handleSema[j].y;

				double h = x4 - x3;
				double up = fabs(fabs(y3)-fabs(x3*k+c));
				double down = fabs(fabs(y4)-fabs(x4*k+c));
				area += (up+down)*h/2;
				j++;
			} while (handleSema[j].x-Peak[i].FinSema.x <= 0.000000); //�ҵ��յ�
			Peak[i].PeakArea = area;
			break;
		}
	}
	//}
}
//-----------------------------------------------------------------------------
//���ܣ��ֶ��Ե�������л���У��
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak �Զ����Peak���ͱ���
//			3.Para& p �������
//���������
//			1.vector<CPeak> &Peak �ֶ��Ե�������л���У����Ĳ�
//-----------------------------------------------------------------------------
VOID neo_BaseLineAdj(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, Para& p) // 1. ���ߵ���
{
	std::vector<CPeak> PeakCopy(Peak); 
	const unsigned SemaSize = handleSema.size();
	const int PeakSize = Peak.size();
	const int PeakNo = p.pn-1;
	if ((PeakNo>PeakSize-1)||(PeakNo<0)||PeakSize==0 ) //�����Ĳ�������
		return;
	if (p.st > p.ft)
	{
		double tmp = p.ft;
		p.ft = p.st;
		p.st = tmp;
	}
	//�ȵ������
	unsigned ml = Peak[PeakNo].StartSema.idx; //�����±�
	unsigned mr = Peak[PeakNo].StartSema.idx; //�����±�
	if (Peak[PeakNo].StartSema.x>=p.st)
	{
		while (ml>1)
		{
			if (p.st<=handleSema[ml].x && p.st>=handleSema[ml-1].x)
			{
				Peak[PeakNo].StartSema = handleSema[ml];
				break;
			}
			ml--;
		}
	}
	else
	{
		while (mr<SemaSize-1)
		{
			if (handleSema[mr].x<=p.st && handleSema[mr+1].x>=p.st)
			{
				Peak[PeakNo].StartSema = handleSema[mr];
				break;
			}
			mr++;
		}
	}
	//�������
	ml = Peak[PeakNo].FinSema.idx; //�����±�
	mr = Peak[PeakNo].FinSema.idx; //�����±�
	if (Peak[PeakNo].FinSema.x>=p.ft)
	{
		while (ml>1)
		{
			if (p.ft<=handleSema[ml].x && p.ft>=handleSema[ml-1].x)
			{
				Peak[PeakNo].FinSema = handleSema[ml];
				break;
			}
			ml--;
		}
	}
	else
	{
		while (mr<SemaSize-1)
		{
			if (handleSema[mr].x<=p.ft && handleSema[mr+1].x>=p.ft)
			{
				Peak[PeakNo].FinSema = handleSema[mr];
				break;
			}
			mr++;
		}
	}
	one_BaselineCheak(handleSema, Peak, PeakNo, false); //��������
	//new_PeakTypeChange(m_Peak); //�����ͷ����ı�
	int ret = one_GetPeakWidth(handleSema, Peak, PeakNo, false); //����������
	if (ret == -1)
	{
		Peak.swap(PeakCopy);
		return;
	}
	one_GetPeakArea(handleSema, Peak, PeakNo); //�����
	new_PeakTypeChange(Peak);
}

int BinarySearch(const std::vector<CSema> &handleSema, double x) 
{ 
	int n = handleSema.size();
	int left=0;
	int right=n-1;
	while(left<=right)
	{ 
		int middle=(left+right)/2;
		if (x<=handleSema[middle].x && x>=handleSema[middle-1].x) return middle;
		if (x>handleSema[middle].x) left=middle+1;
		else right=middle-1;
	} 
	return -1;
}

//-----------------------------------------------------------------------------
//���ܣ��ֶ�����һ����
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak �Զ����Peak���ͱ���
//			3.Para& p �������
//���������
//			1.vector<CPeak> &Peak �ֶ�����һ�����Ĳ�
//-----------------------------------------------------------------------------
VOID neo_AddNewPeak(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, Para& p)	// 3. �����������ӷ�
{
	const unsigned SemaSize = handleSema.size();
	const unsigned PeakSize = Peak.size();
	if (SemaSize==0) //�����Ĳ�������
		return;
	if (p.st > p.ft)
	{
		double tmp = p.ft;
		p.ft = p.st;
		p.st = tmp;
	}
	int ml = BinarySearch(handleSema, p.st); //���ֲ����������
	int mr = BinarySearch(handleSema, p.ft); //���ֲ����յ�����

	double peak = 0.0;
	int mm = 0; 
	for (int i=ml; i<=mr; ++i) //˳����Ҳ�(��)
	{
		if (handleSema[i].y>=peak)
		{
			peak = handleSema[i].y;
			mm = i; 
		}
	}
	//������Ҫ�Բ�����˹�������
	CPeak newPeak(handleSema[ml], handleSema[mm], handleSema[mr]);
	//std::vector<CPeak> PeakCopy;
	int PeakNo = 0;
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (handleSema[mm].x < Peak[i].PeakSema.x)
		{
			Peak.insert(Peak.begin()+i, newPeak);
			PeakNo = i;
			break;
		}
	}
	if (handleSema[mm].x > Peak[PeakSize-1].PeakSema.x)
	{
		Peak.push_back(newPeak);
		PeakNo = PeakSize;
	}


	//Peak.push_back(newPeak);
	one_BaselineCheak(handleSema, Peak, PeakNo, false); //��������
	int ret = one_GetPeakWidth(handleSema, Peak, PeakNo, false);
	if (ret == -1)
	{
		//Peak.swap(PeakCopy);
		return;
	}
	one_GetPeakArea(handleSema, Peak, PeakNo);

}
//-----------------------------------------------------------------------------
//���ܣ��ֶ�ɾ��һ����
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak �Զ����Peak���ͱ���
//			3.Para& p �������
//���������
//			1.vector<CPeak> &Peak �ֶ�ɾ��һ�����Ĳ�
//-----------------------------------------------------------------------------
VOID neo_DelOnePeak(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, Para& p)
{
	std::vector<CPeak> NewPeak;

	if (p.st > p.ft)
	{
		double tmp = p.ft;
		p.ft = p.st;
		p.st = tmp;
	}
	//int ml = BinarySearch(handleSema, p.st); //���ֲ����������
	//int mr = BinarySearch(handleSema, p.ft); //���ֲ����յ�����
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (Peak[i].PeakSema.x<p.st || Peak[i].PeakSema.x>p.ft)
		{
			CPeak OnePeak(Peak[i]);
			NewPeak.push_back(OnePeak);
		}
	}
	Peak.swap(NewPeak);
	new_PeakTypeChange(Peak);
}



//----------------------------------------------------------------------------------------------------------------------
// 2010.12.09 - 2010.12.13 
// ����Ѱ��ӿڣ�����Ϊ��ʵʱ�������״̬�ж�

static std::deque<CRealTimeSema> dequetmpSema;// ȫ�־�̬������Ϊƽ��������
static std::deque<CRealTimeSema> dequeBufSema;// ȫ�־�̬������ΪѰ�建��

void RealTime_Averaging(const std::vector<CRealTimeSema> &InputSema, //����:���ź���
						std::vector<CRealTimeSema> &OutputSema,		 //����:ƽ�����õ��ź���
						std::vector<CRealTimeSema> &TotalAvgSema,    //�ܵ�ƽ�����õ��ź���
						std::vector<CRealTimeSema> &TotalSema,       //�ܵ��ź���
						unsigned avg )	//ƽ������
{//step1: ����avgΪ1���ƽ�����ߣ�ʣ���������Ϊ1��
	if (avg==0)
	{
		//MessageBox(_T("����ƽ����"));
		return;
	}
	const unsigned TotalNumber = TotalSema.size();
	const unsigned InputNum = InputSema.size(); //�����ź�������
	const unsigned TmpNum = dequetmpSema.size();  //��������ź�������
	const unsigned CurrNum = InputNum + TmpNum;
	if (CurrNum > avg) //�Ӷ�����ȡ��ƽ����
	{
		dequetmpSema.insert(dequetmpSema.end(), InputSema.begin(), InputSema.end());
		for (unsigned i=0; i<=CurrNum; ++i)
		{
			double avgTime = 0.0;
			double avgSema = 0.0;
			if (i % avg == 0 && i+avg < CurrNum) 
			{
				for (unsigned j=i; j< i+avg; j++)
				{
					avgTime += dequetmpSema[0].x;
					avgSema += dequetmpSema[0].y;
					TotalSema.push_back(dequetmpSema[0]);
					dequetmpSema.pop_front(); //����
				}
				avgTime = avgTime / avg;
				avgSema = avgSema / avg;

				CRealTimeSema OneRTSema(i+TotalNumber, avgTime, avgSema);
				OutputSema.push_back(OneRTSema);
				//TotalAvgSema.push_back(OneRTSema);�ŵ�RealTime_GetAvgDataSlope���������
			}
			if (dequetmpSema.size()<avg) //ʣ��
			{
				break;
			}
		}
	}
	else //ֱ�ӷ��ڶ�����
	{
		dequetmpSema.insert(dequetmpSema.end(), InputSema.begin(), InputSema.end());
	}
}


void RealTime_GetAvgDataSlope(std::vector<CRealTimeSema> &InputSema, //ƽ�����õ��ź���
							  std::vector<CRealTimeSema> &TotalAvgSema,
							  std::vector<double> &firstOrder,	//б�ʴ������
							  double &maxSlope)					//���б��            
{//step2: ���ƽ��������ÿһ���б��

	//������Ҫ�Ż�
	if (!firstOrder.empty())
	{
		const unsigned totsize = TotalAvgSema.size()-1;
		double ydiff = InputSema[0].y - TotalAvgSema[totsize].y;
		double xdiff = InputSema[0].x - TotalAvgSema[totsize].x;
		double first_k = ydiff / xdiff;
		firstOrder.push_back(first_k);
		if (first_k>=maxSlope)
			maxSlope =first_k;
	}
	
	const unsigned avgDataSize = InputSema.size()-1;
	double *k = new double[avgDataSize];
	for (unsigned i=0; i<avgDataSize; ++i)
	{	
		double ydiff = InputSema[i+1].y - InputSema[i].y;
		double xdiff = InputSema[i+1].x - InputSema[i].x;
		k[i] = (ydiff) / (xdiff);
		if (k[i]>=maxSlope)
			maxSlope = k[i];
		firstOrder.push_back(k[i]);
	}
	delete [] k;
	k = NULL;

	for (unsigned i=0; i<InputSema.size(); ++i)
	{
		
		TotalAvgSema.push_back(InputSema[i]);
	}

	//InputSema.clear(); //����֮��ɾ��
	
	//dequetmpSema.clear();//����֮��ɾ��
	

}

void RealTime_GetpeakSearch(std::vector<CRealTimeSema> &InputSema, //ƽ�����õ��ź���
							const std::vector<double>  &firstOrder,		//ƽ�����õ��ź�����б��
							std::vector<CRealTimeSema> &TotAvgSema,
							std::vector<CRealTimeSema> &RealTimeSema,   //(���)�������
							const double slopeBased, bool hasNagetiveSpike, bool isTwiceJudge)
{
	// step3: ������������˳��Ѱ��
	enum enumState {
		STATELESS,			//��״̬
		//����
		PLUS_UPCOMING_WAVE,	 //���в�
		PLUS_DOWNGOING_WAVE, //���в�
		//����
		MINUS_DOWNGOING_WAVE,	//���в�
		MINUS_UPCOMING_WAVE,	//���в�
		
		HANDLE_ACCOMPLISH   //�������
	} hState;				//��������еı��


		////δ֪״̬
		//UNKNOWN,		  //��ʼ��
		//HANDLED,          //�Ѵ������޷��жϴ��ں���״̬
		////�������
		//PLUS_START_POINT, //���
		//PLUS_RAISE_POINT, //����
		//PLUS_PEAK_POINT,  //�����
		//PLUS_DROP_POINT,  //�½�
		//PLUS_FINISH_POINT, //�յ�
		////�������
		//MINUS_START_POINT, //���
		//MINUS_DROP_POINT,  //�½�
		//MINUS_TROUGH_POINT,//���ȵ�
		//MINUS_RAISE_POINT, //����
		//MINUS_FINISH_POINT //�յ�

	//����ǰ�����ݳ�ʼ��
	static enumState lastState = STATELESS; //��һ��δ������ϵ�״̬
	static unsigned id = 0;//��̬��firstOrder�±꣩
	id -= dequeBufSema.size(); //���������±�
	const unsigned DataSize = InputSema.size()+dequeBufSema.size(); //��ν��д�������ݸ���
	dequeBufSema.insert(dequeBufSema.end(),InputSema.begin(),InputSema.end()); //������źŶ��ŵ�����ĩ��
	InputSema.clear(); //�����ź����,�ⲽ���Էŵ���һ������������
	hState = lastState;
	unsigned i=0, j=0, k=0;

	//unsigned getStartPoint=0, getpeakPoint=0, getFinishPoint=0;
	//int mark = 1; //�����ͱ��: 1=����; -1=����; 2,3,4,...Ϊ�ص�����; -2,-3,-4...Ϊ�ص�����
	//bool isNagetiveSpike = false;
	
	while (hState != HANDLE_ACCOMPLISH)
	{
		switch (hState)
		{
		case STATELESS:  //��״̬
			lastState = STATELESS;
			for ( ; i<=DataSize-1; ++i, ++id)
			{
				if (i>=DataSize-1) 
				{
					hState = HANDLE_ACCOMPLISH;
					break;
				}
				if (firstOrder[id]>=0) //�Ǹ��忼�����
				{
					if (firstOrder[id]<slopeBased)
					{
						//InputSema[i].pts = CRealTimeSema::HANDLED; //����Ѵ���
						continue;
					}
					if (firstOrder[id]>slopeBased)
					{
						if (firstOrder[id+1]<slopeBased)
						{
							//InputSema[i+1].pts = CRealTimeSema::HANDLED; //����Ѵ���
							continue;
						}
						if (firstOrder[id+1]>=slopeBased)
						{
							//isNagetiveSpike = true;
							//getStartPoint = i;
							RealTimeSema.push_back(TotAvgSema[id]); //�ҵ����
							hState = PLUS_UPCOMING_WAVE; //�������в�
							break;
						}
					}
				}
			}
			break;


		case PLUS_UPCOMING_WAVE://�������в�
			lastState = PLUS_UPCOMING_WAVE;
			for (j=i/*+1*/; j<=DataSize-1; ++j, ++id)
			{
				if (j>=DataSize-1) 
				{
					hState = HANDLE_ACCOMPLISH;
					break;
				}
				if (firstOrder[id]<0)
				{
					//InputSema[i].pts = CRealTimeSema::PLUS_RAISE_POINT; //�������
					continue;
				}
				if (firstOrder[id]>=0)
				{
					if (firstOrder[id+1]>0)
					{
						//InputSema[i].pts = CRealTimeSema::PLUS_RAISE_POINT; //�������
						continue;
					}
					if (firstOrder[id+1]<0/* && firstOrder[j+2]<0*/) //2010.08.25 ��ӶԲ������Ĵ���
					{
						//getpeakPoint = j+1;
						RealTimeSema.push_back(TotAvgSema[id+1]);//�ҵ�����
						hState = PLUS_DOWNGOING_WAVE; //�������в�
						break;
					}
				}
			}
			break;



		case PLUS_DOWNGOING_WAVE://�������в�
			lastState = PLUS_DOWNGOING_WAVE;
			for (k = j/*+1*/; k<=DataSize-1; ++k, ++id)
			{
				if (k>=DataSize-1) 
				{
					hState = HANDLE_ACCOMPLISH;
					break;
				}

				if ( firstOrder[id]<0 ) 
				{
	
					if ((fabs(firstOrder[id]) < slopeBased) && (firstOrder[id]!=0.000000))
					{
					
						if ( fabs(firstOrder[id+1])<slopeBased && (firstOrder[id+1]!=0.000000))
						{
							//getFinishPoint = k+1;
							RealTimeSema.push_back(TotAvgSema[id+1]); //�ҵ��յ�
							hState = STATELESS;
							i = k;
							break;
						}
						if (fabs(firstOrder[id+1])>slopeBased)
							continue;

					}
				}
				if ( firstOrder[id+1]>0 )
				{
					//getFinishPoint = k;
					RealTimeSema.push_back(TotAvgSema[id]);
					hState = STATELESS;
					i = k;
					break;
				}
			}
			break;


		case MINUS_DOWNGOING_WAVE://���� ���в�
			break;
		case MINUS_UPCOMING_WAVE://���� ���в�
			break;
		case HANDLE_ACCOMPLISH://�������
			break;
		default:
			break;
}}}

void RealTime_GetpeakSearch_v2(std::vector<CRealTimeSema> &InputSema, //ƽ�����õ��ź���
								const std::vector<double>  &firstOrder,		//ƽ�����õ��ź�����б��
								std::vector<CRealTimeSema> &TotAvgSema,
								std::vector<CRealTimeSema> &RealTimeSema,   //(���)�������
								const double slopeBased, bool hasMinusPeak, bool isTwiceJudge,
								const int series = 2)
{
	// step3: ������������˳��Ѱ��
	enum enumState {
		STATELESS,			//��״̬
		//����
		PLUS_UPCOMING_WAVE,	 //���в�
		PLUS_DOWNGOING_WAVE, //���в�
		//����
		MINUS_DOWNGOING_WAVE,	//���в�
		MINUS_UPCOMING_WAVE,	//���в�

		HANDLE_POSTPROCESS,   //����
		HANDLE_ACCOMPLISH     //�������
	} hState;				//��������еı��

	
	//δ֪״̬
	//UNKNOWN,		  //��ʼ��
	//HANDLED,          //�Ѵ������޷��жϴ��ں���״̬
	//�������
	//PLUS_START_POINT, //���
	//PLUS_RAISE_POINT, //����
	//PLUS_PEAK_POINT,  //�����
	//PLUS_DROP_POINT,  //�½�
	//PLUS_FINISH_POINT, //�յ�
	//PLUS_CONTINUOUS_WAVE, //������
	//�������
	//MINUS_START_POINT, //���
	//MINUS_DROP_POINT,  //�½�
	//MINUS_TROUGH_POINT,//���ȵ�
	//MINUS_RAISE_POINT, //����
	//MINUS_FINISH_POINT //�յ�
	
	//����ǰ�����ݳ�ʼ��
	static enumState lastState = STATELESS; //��һ��δ������ϵ�״̬
	static unsigned id = 0;//��̬��firstOrder�±꣩
	static bool OnlyOnce = true;
	const unsigned firstOrderSize = firstOrder.size();
	unsigned DataSize = 0;
	if (OnlyOnce)
	{
		DataSize = InputSema.size() + dequeBufSema.size()-1; //��ν��д�������ݸ���
		OnlyOnce = !OnlyOnce;
	} 
	else
		DataSize = InputSema.size() + dequeBufSema.size();
	dequeBufSema.insert(dequeBufSema.end(),InputSema.begin(),InputSema.end());//������źŶ��ŵ�����ĩ��
	InputSema.clear(); //�����ź����,�ⲽ���Էŵ���һ������������
	hState = lastState;
	unsigned i=0, j=0, k=0;
	unsigned dequeId=0; //���б��
	bool isContinuousWave = false; //�����崦����
	bool isFlatPeakProcessed = false; //ƽͷ�崦����
	
	//unsigned getStartPoint=0, getpeakPoint=0, getFinishPoint=0;
	//int mark = 1; //�����ͱ��: 1=����; -1=����; 2,3,4,...Ϊ�ص�����; -2,-3,-4...Ϊ�ص�����
	//bool isNagetiveSpike = false;

	while (hState != HANDLE_ACCOMPLISH)
	{
		switch (hState)
		{
		case STATELESS:  //��״̬
			lastState = STATELESS;
			for (; i<DataSize-series && hState==STATELESS; ++i, ++id)
			{
				if (i>=DataSize-series-1 || id>=firstOrderSize-series) 
				{
					++id;
					hState = HANDLE_POSTPROCESS;
					break;
				}

				if (firstOrder[id]<=0)
				{//�������
					if (hasMinusPeak) //���Ǹ������
					{
						if (fabs(firstOrder[id])<slopeBased)
							continue;
						if (fabs(firstOrder[id])>=slopeBased)
						{
							for (int s=1; s<=series; ++s)
							{
								if (fabs(firstOrder[id+s])<slopeBased)
								{
									isContinuousWave = false; 
									break;
								}
								if (fabs(firstOrder[id+s])>slopeBased)
								{
									if (s==series)
									{
										if ((!RealTimeSema.empty())
											&& (RealTimeSema[RealTimeSema.size()-1].pts == CRealTimeSema::MINUS_CONTINUOUS_WAVE)
											&& isContinuousWave)
										{
											//::AfxMessageBox(_T("dequeBufSema.empty()"));
											RealTimeSema.push_back(RealTimeSema[RealTimeSema.size()-1]); //�ҵ�(����)���
										}
										//�����������
										else
										{
											dequeBufSema[i].pts = CRealTimeSema::MINUS_START_POINT; //���Ϊ�������
											RealTimeSema.push_back(dequeBufSema[i]); //�ҵ����

										}
										for (unsigned x=dequeId; x<i; ++x)
											dequeBufSema[x].pts = CRealTimeSema::HANDLED; //����Ѵ���
										dequeId = i+1;
										hState = MINUS_DOWNGOING_WAVE; //�������в�
										break;
									}
								}
							}
						}
					}
					else
						continue;
				}
				if (firstOrder[id]>=0) //�Ǹ��忼�����
				{
					if (firstOrder[id]>slopeBased)
					{
						for (int s=1; s<=series; ++s)
						{
							if (firstOrder[id+s]<slopeBased)
							{
								isContinuousWave = false; 
								break;
							}
							if (firstOrder[id+s]>=slopeBased)
							{
								if (s==series)
								{
									if ((!RealTimeSema.empty())
										&& (RealTimeSema[RealTimeSema.size()-1].pts == CRealTimeSema::PLUS_CONTINUOUS_WAVE)
										&& isContinuousWave)
									{
										//::AfxMessageBox(_T("dequeBufSema.empty()"));
										RealTimeSema.push_back(RealTimeSema[RealTimeSema.size()-1]); //�ҵ�(����)���
									}
									//�����������
									else
									{
										dequeBufSema[i].pts = CRealTimeSema::PLUS_START_POINT; //���Ϊ���
										RealTimeSema.push_back(dequeBufSema[i]); //�ҵ����

									}
									for (unsigned x=dequeId; x<i; ++x)
										dequeBufSema[x].pts = CRealTimeSema::HANDLED; //����Ѵ���
									dequeId = i+1;
									hState = PLUS_UPCOMING_WAVE; //�������в�
									break;
								}
							}
						}
					}
				}
			}
			break;

		case PLUS_UPCOMING_WAVE://�������в�
			lastState = PLUS_UPCOMING_WAVE;
			for (j=i; j<DataSize-series && hState == PLUS_UPCOMING_WAVE; ++j, ++id)
			{

				if (j>=DataSize-series-1 || id>=firstOrderSize-series) 
				{
					++id;
					hState = HANDLE_POSTPROCESS;
					break;
				}

				if (firstOrder[id]<0)
					continue;
				if (firstOrder[id]>=0)
				{
					for (int s=1; s<=series; ++s)
					{
						if (firstOrder[id+s]>0)
							break;
						if (firstOrder[id+s]<0)
						{
							if (s==series)
							{
								dequeBufSema[j+s].pts = CRealTimeSema::PLUS_PEAK_POINT; //��ǲ���
								RealTimeSema.push_back(dequeBufSema[j+s]); //�ҵ�����

								for (unsigned x=dequeId; x<j+s; ++x)
									dequeBufSema[x].pts = CRealTimeSema::PLUS_RAISE_POINT; //���������
								dequeId = j+s+1;
								hState = PLUS_DOWNGOING_WAVE; //�������в�
								break;
							}
						}
					}
				}
			}
			break;

		case PLUS_DOWNGOING_WAVE://�������в�
			lastState = PLUS_DOWNGOING_WAVE;
			for (k = j; k<DataSize-series && hState == PLUS_DOWNGOING_WAVE; ++k, ++id)
			{
				if (k>=DataSize-series-1 || id>=firstOrderSize-series) 
				{
					++id;
					hState = HANDLE_POSTPROCESS;
					break;
				}

				if ( firstOrder[id]<0 ) 
				{
					if (fabs(firstOrder[id])>slopeBased)  //2010.08.24 ���ƽͷ�崦������
					{
						isFlatPeakProcessed = true;
						continue;
					}

					if (isFlatPeakProcessed && (fabs(firstOrder[id]) < slopeBased) && (firstOrder[id]!=0.000000))
					{
						for (int s=1; s<=series; ++s)
						{
							if (fabs(firstOrder[id+s])>slopeBased)
								break;
							if (fabs(firstOrder[id+s])<slopeBased && (firstOrder[id+s]!=0.000000))
							{
								if (s==series)
								{
									//getFinishPoint = k+1;
									dequeBufSema[k+1].pts = CRealTimeSema::PLUS_FINISH_POINT; //����յ�
									RealTimeSema.push_back(dequeBufSema[k+1]); //�ҵ��յ�
									for (unsigned x=dequeId; x<k+1; ++x)
										dequeBufSema[x].pts = CRealTimeSema::PLUS_DROP_POINT; //����½���
									dequeId = k+2;
									hState = STATELESS;
									i = k;
									break;
								}
							}
						}

					}
				}
				if ( firstOrder[id]>0 )
				{
					//getFinishPoint = k;
					dequeBufSema[k+1].pts = CRealTimeSema::PLUS_CONTINUOUS_WAVE; //��ǣ��������յ�
					RealTimeSema.push_back(dequeBufSema[k+1]); //�ҵ��յ�
					for (unsigned x=dequeId; x<k+1; ++x)
						dequeBufSema[x].pts = CRealTimeSema::PLUS_DROP_POINT; //����½���
					dequeId = k+2; //
					i = k+1;
					isContinuousWave = true; //
					hState = STATELESS; //
					break;
				}
			}
			break;
			
		case MINUS_DOWNGOING_WAVE://���� ���в�
			lastState = MINUS_DOWNGOING_WAVE;
			for (j=i; j<DataSize-series && hState==MINUS_DOWNGOING_WAVE; ++j,++id)
			{
				if (j>=DataSize-series-1 || id >= firstOrderSize-series) 
				{
					++id;
					hState = HANDLE_POSTPROCESS;
					break;
				}

				if (firstOrder[id]>0)
					continue;
				if (firstOrder[id]<=0)
				{
					for (int s=1; s<=series; ++s)
					{
						if (firstOrder[id+s]<0)
							break;
						if (firstOrder[id+s]>0)
						{
							if (s==series)
							{
								dequeBufSema[j+s].pts = CRealTimeSema::MINUS_TROUGH_POINT; //��ǲ���
								RealTimeSema.push_back(dequeBufSema[j+s]); //�ҵ�����

								for (unsigned x=dequeId; x<j+s; ++x)
									dequeBufSema[x].pts = CRealTimeSema::MINUS_DROP_POINT; //����½���
								dequeId = j+s+1;
								hState = MINUS_UPCOMING_WAVE; //�������в�
								break;
							}
						}
					}
				}
			}
			break;
		case MINUS_UPCOMING_WAVE://���� ���в�
			lastState = MINUS_UPCOMING_WAVE;
			for (k=j; k<DataSize-series && hState==MINUS_UPCOMING_WAVE; ++k,++id)
			{
				if (k>=DataSize-series-1 || id >= firstOrderSize-series)
				{
					++id;
					hState = HANDLE_POSTPROCESS;
					break;
				}
				if (firstOrder[id]>=0)
				{
					if (firstOrder[id]>slopeBased)  //2010.08.25 ���ƽͷ�崦������
					{	
						isFlatPeakProcessed = true;
						continue;
					}
					if (isFlatPeakProcessed && firstOrder[id]<slopeBased && (firstOrder[id]!=0.000000)) 
					{
						for (int s=1; s<=series; ++s)
						{
							if (firstOrder[id+s]>slopeBased)
								break;
							if (firstOrder[id+s]<slopeBased  && (firstOrder[id+s]!=0.000000))
							{
								if (s==series)
								{
									//getFinishPoint = k+1;
									dequeBufSema[k+1].pts = CRealTimeSema::MINUS_FINISH_POINT; //��Ǹ����յ�
									RealTimeSema.push_back(dequeBufSema[k+1]); //�ҵ������յ�
									for (unsigned x=dequeId; x<k+1; ++x)
										dequeBufSema[x].pts = CRealTimeSema::MINUS_RAISE_POINT; //��Ǹ���������
									dequeId = k+2;
									hState = STATELESS;
									i = k;
									break;
								}
							}
						}
					}
				}
				if (firstOrder[id]<0)
				{
					//getFinishPoint = k;
					dequeBufSema[k+1].pts = CRealTimeSema::MINUS_CONTINUOUS_WAVE; //��ǣ��������յ�
					RealTimeSema.push_back(dequeBufSema[k+1]); //�ҵ��յ�
					for (unsigned x=dequeId; x<k+1; ++x)
						dequeBufSema[x].pts = CRealTimeSema::MINUS_RAISE_POINT; //��Ǹ���������
					dequeId = k+2; //
					i = k+1;
					isContinuousWave = true; //
					hState = STATELESS; //
					break;
				}
			}
			break;

		case HANDLE_POSTPROCESS://���к�������
			isFlatPeakProcessed = false;
			if (dequeId>=dequeBufSema.size())
				::AfxMessageBox(_T("dequeId>=dequeBufSema.size()"));

			id -= dequeBufSema.size()-dequeId-series-1; //�����±���ʼ��

			for (unsigned x=0; x<dequeId; ++x)
				dequeBufSema.pop_front();

			hState = HANDLE_ACCOMPLISH;
			break;

		default:
			break;
		}
	}
}

  
void RealTime_RestorePeakData(std::vector<CRealTimeSema> &InputSema, //���������ź������������
								std::vector<CRealTimeSema> &TotalSema,      //�ܵ��ź���
								std::vector<CRealTimeSema> &RealTimeSema,   //��㲨�岨���յ�����
								const int avg,
								const double slopeBased)  
{// step4: ������ĵ㻹ԭ��Ԫ����
	const unsigned currentTotalSemaSize = TotalSema.size();
	const unsigned peakSemaSize = RealTimeSema.size();
	static unsigned i=0; //������㣬���壬����3���±�
	static unsigned p=1; //���Ƽ�ʱ��ԭ��������±� d
	// ��ʱ��ԭ�������
	for ( ; p<peakSemaSize-3 && peakSemaSize>2; p+=3)
	{
		double maxValue = -INT_MAX;
		for (unsigned x = RealTimeSema[p-1].idx; x<RealTimeSema[p].idx+avg && RealTimeSema[p].idx+avg < currentTotalSemaSize; ++x)
		{
			if (maxValue <= TotalSema[x].y)
			{
				maxValue = TotalSema[x].y;
				RealTimeSema[p] = TotalSema[x];
			}
		}
	}
	//�ȵ���㣬���壬����3������֮���ٻ�ԭ
	for ( ; i<peakSemaSize-3 && peakSemaSize>2 /*&& peakSemaSize%3==0*/; i+=3)  //���
	{
		for (unsigned j=RealTimeSema[i].idx; j<RealTimeSema[i].idx+avg && j<currentTotalSemaSize-1; j++) //�Ȼ�ԭ���
		{
			double k=(TotalSema[j+1].y-TotalSema[j].y)/(TotalSema[j+1].x-TotalSema[j].x);
			if (fabs(k)>slopeBased)
			{
				if (j==RealTimeSema[i].idx+avg-1)
				{
					RealTimeSema[i] = TotalSema[j];
					break;
				}
			}
			else
			{
				RealTimeSema[i] = TotalSema[j];
				break;
			}
		}
		for (unsigned j=RealTimeSema[i+2].idx; j<RealTimeSema[i+2].idx+avg && j<currentTotalSemaSize-1; j++) //�ٻ�ԭ���
		{
			double k=(TotalSema[j+1].y-TotalSema[j].y)/(TotalSema[j+1].x-TotalSema[j].x);
			if (fabs(k)>slopeBased)
			{
				if (j==RealTimeSema[i+2].idx+avg-1)
				{
					RealTimeSema[i+2] = TotalSema[j];
					break;
				}
			}
			else
			{
				RealTimeSema[i+2] = TotalSema[j];
				break;
			}
		}

		//���ԭ������߲��ȵ�
		double maxValue = -INT_MAX;
		double minValue = INT_MAX;
		//if (Peak[i].Type>=1) //��ԭ����
	//	{
			for (unsigned j=RealTimeSema[i].idx; j<RealTimeSema[i+2].idx; ++j)
			{
				if (maxValue <= TotalSema[j].y)
				{
					maxValue = TotalSema[j].y;
					RealTimeSema[i+1] = TotalSema[j];
				}
			}
			//Sleep(2);
		//}
		//else //��ԭ����
		//{
		//	for (unsigned j=Peak[i].StartSema.idx; j<Peak[i].FinSema.idx; ++j)
		//	{
		//		if (minValue >= handleSema[j].y)
		//		{
		//			minValue = handleSema[j].y;
		//			Peak[i].PeakSema = handleSema[j];	
		//		}
		//	} 
		//}
	}
}




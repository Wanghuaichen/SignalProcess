#include "StdAfx.h"
#include "Peak.h"
#include <cmath>
#include <algorithm>
#include <deque>
#include "ConfigArgument.h"

const double TAILING = 1.000000; //��β���ӣ��б�������Ƿ��/��β


VOID CA_GetMinMaxSlope(const std::vector<CSema> &avgData,
	const std::vector<double> &firstOrder,
	double &MinSlope, 
	double &MaxSlope,
	int N)
{
	if (N<1 || static_cast<size_t>(N)>firstOrder.size())  return;

	//��ʼ��б����ֵ
	MinSlope = (double)INT_MAX;
	MaxSlope = 0.0;

	//б�ʱȽϻ���
	deque<double> tmpSlope;


	for (size_t i=0; i<firstOrder.size(); i+=N)
	{
		for (int j=0; j<N; ++j)
		{
			if (firstOrder[i]<=0) //С�ڵ���0��б�ʲ�Ҫ
				continue;
			tmpSlope.push_back(firstOrder[i]);
		}
		if (tmpSlope.empty()) continue;
		double maxS = tmpSlope[0];
		double minS = tmpSlope[0];
		while (!tmpSlope.empty()) //�Ƚϻ��棬�õ����ֵ����Сֵ
		{
			if (tmpSlope[0]>maxS) maxS = tmpSlope[0];
			if (tmpSlope[0]<minS) minS = tmpSlope[0];
			tmpSlope.pop_front();
		}
	
		if(maxS < MinSlope) MinSlope = maxS;
		if(minS > MaxSlope) MaxSlope = minS;
		
	}
}



VOID CA_GetpeakSearch_Series(const std::vector<CSema> &avgData,   //ƽ�����õ��ź���
	const std::vector<double> &firstOrder,//ƽ�����õ��ź�����б��
	std::vector<CPeak> &Peak,    //(���)�������
	std::vector<double> &slopeBased, //������б������
	std::deque<bool> &hasNagetiveSpike,//!�����������
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
			for ( ; i<avgDataSize-series && hState==STATELESS; ++i)  
			{
				if (i>=avgDataSize-series-1) 
				{
					hState = HANDLE_ACCOMPLISH;
					break;
				}

				if (firstOrder[i]<=0)
				{//�������
					if (hasNagetiveSpike[i]) //���忼����� 
					{
						if (fabs(firstOrder[i])<slopeBased[i])
							continue;
						if (fabs(firstOrder[i])>=slopeBased[i])
						{
							for (int s=1; s<=series; ++s)
							{
								if (fabs(firstOrder[i+s])<slopeBased[i])
									break;
								if (fabs(firstOrder[i+s])>slopeBased[i])
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
					if (firstOrder[i]>slopeBased[i])
					{
						for (int s=1; s<=series; ++s)
						{
							if (fabs(firstOrder[i+s])<slopeBased[i+s])
								break;
							if (fabs(firstOrder[i+s])>=slopeBased[i+s])
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
						if (firstOrder[j]>slopeBased[j])  //2010.08.25 ���ƽͷ�崦������
						{	
							processed = true;
							continue;
						}
						if (processed && firstOrder[j]<slopeBased[j] && (firstOrder[j]!=0.000000)) 
						{
							for (int s=1; s<=series; ++s)
							{
								if (firstOrder[j+s]>slopeBased[j+s])
									break;
								if (firstOrder[j+s]<slopeBased[j+s]  && (firstOrder[j+s]!=0.000000))
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
					if (t>0 && (Peak[t].StartSema.x-Peak[t-1].FinSema.x)==0.000000)
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
				for (k=j; k<avgDataSize-1 && hState==DOWNGOING_WAVE; ++k)
				{
					if (k>=avgDataSize-series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}

					if ( firstOrder[k]<0 ) 
					{
						if (fabs(firstOrder[k])>slopeBased[k])  //2010.08.24 ���ƽͷ�崦������
						{
							processed = true;
							continue;
						}

						if (processed && (fabs(firstOrder[k]) < slopeBased[k]) && (firstOrder[k]!=0.000000))
						{
							for (int s=1; s<=series; ++s)
							{
								if (fabs(firstOrder[k+s])>slopeBased[k+s])
									break;
								if (fabs(firstOrder[k+s])<slopeBased[k+s] && (firstOrder[k+s]!=0.000000))
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

					if (t>0 && (Peak[t].StartSema.x-Peak[t-1].FinSema.x)==0.000000)
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







//-----------------------------------------------------------------------------
//���ܣ�����������ź�������㲨���յ㻹ԭ��Ԫ������
//���������
//			1.vector<CSema> &handleSema Ԫ�����ź���
//			2.avg ƽ������
//			3.std::vector<double> &slopeBased, //������б������
//���������
//			1.vector<CPeak> &Peak ��ԭ���ź�������㲨���յ�
//-----------------------------------------------------------------------------
VOID CA_RestorePeakData(const std::vector<CSema> &handleSema, //����ԭ���ź���
	std::vector<CPeak> &Peak,
	const int avg,
	const std::vector<double> &slopeBased) //������б������)  

{// step4: ������ĵ㻹ԭ��Ԫ����
	const unsigned handleSemaSize = handleSema.size();
	const unsigned peakSemaSize = Peak.size();

	for (unsigned i=0; i<peakSemaSize; ++i)  //����ÿһ����
	{
		for (unsigned j=Peak[i].StartSema.idx; j<Peak[i].StartSema.idx+avg && j<handleSemaSize-1; j++) //�Ȼ�ԭ���
		{
			double k=(handleSema[j+1].y-handleSema[j].y)/(handleSema[j+1].x-handleSema[j].x);
			if (fabs(k)>slopeBased[j])
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
			if (fabs(k)>slopeBased[j])
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
//���ܣ����߷ַ�
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ
//			2.deque<pair<unsigned, unsigned> > &dequeIdx ���߷ַ�����Ĳ���
//			
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_TangentDividPeak(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	vector<AnalysisConfigArguments::PeakSepaArg> &PeakSepaArgs)
{
	if (PeakSepaArgs.empty())
	{
		return;
	}
	const unsigned ArgSize = PeakSepaArgs.size();
	for (unsigned index = 0; index<ArgSize; ++index)
	//while (!PeakSepaArgs.empty())
	{   
		// ����
		if (PeakSepaArgs[index].SepaType == 2)
		{
			unsigned start = PeakSepaArgs[index].StartPeakNo; //���������
			unsigned fin = PeakSepaArgs[index].FinishPeakNo;  //�������յ�
		
			for ( ; PeakSepaArgs[index].StartPeakNo!=PeakSepaArgs[index].FinishPeakNo; ++(PeakSepaArgs[index].StartPeakNo))
			{
				unsigned sp = Peak[start].StartSema.idx;
				unsigned ep = Peak[fin].FinSema.idx;
				double _k = (handleSema[ep].y-handleSema[sp].y)/(handleSema[ep].x-handleSema[sp].x);
				double _c = handleSema[sp].y- _k*handleSema[sp].x;
				for (unsigned i=Peak[start].PeakSema.idx; i<=Peak[fin].PeakSema.idx; ++i)
				{
					double _y = _k * handleSema[i].x + _c;
					if (Peak[start].Type < 0) //���Ǹ������
					{
						if ((fabs(_y) < fabs(handleSema[i].y) && _y>0)
						   ||(fabs(_y) > fabs(handleSema[i].y) && _y>=0))
						{
							start++;
							break;
						}
					}
					else
					{
						if ((fabs(_y) > fabs(handleSema[i].y) && _y>0)//������������������ڻ�����
							||(fabs(_y) < fabs(handleSema[i].y) && _y<=0))
						{
							start++;
							break;
						}
					}
				}
			}

			Peak[start].FinSema = Peak[fin].FinSema;
			//��Ƿַ�����//
			for (unsigned i=start; i<=fin; ++i)
				Peak[i].dType = CPeak::PLUSTANGET;
			//////////////

			for ( ; start!=fin+1; ++start)
			{
		
				//У�����������
				unsigned s = Peak[start].StartSema.idx;
				unsigned f = Peak[fin].FinSema.idx;
				//���У��
				bool isfail = true;
				for (; s<=Peak[start].PeakSema.idx && isfail; ++s)
				{
					double k = (handleSema[f].y-handleSema[s].y)/(handleSema[f].x-handleSema[s].x);
					double c = handleSema[s].y-k*handleSema[s].x;
					for (unsigned m=s; m<=Peak[start].PeakSema.idx; ++m) 
					{
						double _y = k * handleSema[m].x+c;
						if (Peak[start].Type < 0) //���Ǹ������
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
						Peak[start].StartSema = handleSema[s];
						isfail = false;
					}
				}isfail = true;	//�յ�У��
				for (; f>=Peak[fin].PeakSema.idx && isfail; --f)
				{
					double k = (handleSema[f].y-handleSema[s].y)/(handleSema[f].x-handleSema[s].x);
					double c = handleSema[f].y-k*handleSema[f].x;
					for (unsigned n=f; n>=Peak[fin].PeakSema.idx; --n)
					{
						double _y = k * handleSema[n].x + c;
						if (Peak[start].Type < 0) //���Ǹ������
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
						Peak[fin].FinSema = handleSema[f];
						isfail = false;
					}
				}
			}
		}	
		else if (PeakSepaArgs[index].SepaType == 3)
		{
			//����
			//���е�����
			unsigned start = PeakSepaArgs[index].FinishPeakNo; //���������
			unsigned fin = PeakSepaArgs[index].StartPeakNo;  //�������յ�

			for ( ; PeakSepaArgs[index].StartPeakNo!=PeakSepaArgs[index].FinishPeakNo; --(PeakSepaArgs[index].FinishPeakNo))
			{
				unsigned sp = Peak[start].FinSema.idx;
				unsigned ep = Peak[fin].StartSema.idx;
				double _k = (handleSema[ep].y-handleSema[sp].y)/(handleSema[ep].x-handleSema[sp].x);
				double _c = handleSema[sp].y- _k*handleSema[sp].x;
				for (unsigned i=Peak[start].PeakSema.idx; i>=Peak[fin].PeakSema.idx; --i)
				{
					double _y = _k * handleSema[i].x + _c;
					if (Peak[start].Type < 0) //���Ǹ������
					{
						if ((fabs(_y) < fabs(handleSema[i].y) && _y>0)
							||(fabs(_y) > fabs(handleSema[i].y) && _y>=0))
						{
							//::AfxMessageBox(_T("�������δʵ�֣�����"));
							//return;
							start++;
							break;
						}
					}
					else
					{
						if ((fabs(_y) > fabs(handleSema[i].y) && _y>0)//������������������ڻ�����
							||(fabs(_y) < fabs(handleSema[i].y) && _y<=0))
						{
							start--;
							break;
						}
					}
				}
			}

			Peak[start].StartSema = Peak[fin].StartSema;
			//��Ƿַ�����//
			for (unsigned i=start; i>=fin; --i)
				Peak[i].dType = CPeak::MINUSTANGET;
			//////////////

			for ( ; start!=fin-1; --start)
			{
		
				//У�����������
				unsigned s = Peak[start].FinSema.idx;
				unsigned f = Peak[fin].StartSema.idx;
				//���У��
				bool isfail = true;
				for (; s>=Peak[start].PeakSema.idx && isfail; --s)
				{
					double k = (handleSema[f].y-handleSema[s].y)/(handleSema[f].x-handleSema[s].x);
					double c = handleSema[s].y-k*handleSema[s].x;
					for (unsigned m=s; m>=Peak[start].PeakSema.idx; --m) 
					{
						double _y = k * handleSema[m].x+c;
						if (Peak[start].Type < 0) //���Ǹ������
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
						Peak[start].FinSema = handleSema[s];
						isfail = false;
					}
				}isfail = true;	//�յ�У��
				for (; f<=Peak[fin].PeakSema.idx && isfail; ++f)
				{
					double k = (handleSema[f].y-handleSema[s].y)/(handleSema[f].x-handleSema[s].x);
					double c = handleSema[f].y-k*handleSema[f].x;
					for (unsigned n=f; n<=Peak[fin].PeakSema.idx; ++n)
					{
						double _y = k * handleSema[n].x + c;
						if (Peak[start].Type < 0) //���Ǹ������
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
						Peak[fin].StartSema = handleSema[f];
						isfail = false;
					}
				}
			}
		}
	}
}


//-----------------------------------------------------------------------------
//���ܣ���ֱ�ַ��㷨(�ɰ汾)
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak �Զ����Peak���ͱ���
//���������
//			1.vector<CPeak> &Peak ��ֱ�ַ��Ĳ�
//-----------------------------------------------------------------------------
VOID CA_DividePeak_legacy(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<AnalysisConfigArguments::PeakSepaArg> &PeakSepaArgs)        
{// ��ֱ�ַ�    
	if (PeakSepaArgs.empty())
	{
		return;
	}
	const unsigned PeakSize = Peak.size();
	
	const unsigned ArgSize = PeakSepaArgs.size();
	for (unsigned index = 0; index<ArgSize; ++index)
	{
		if (PeakSepaArgs[index].SepaType == 1) //ȷ���ַ������Ǵ�ֱ�ַ�
		{
			unsigned bg = PeakSepaArgs[index].StartPeakNo;
			unsigned ed = PeakSepaArgs[index].FinishPeakNo;
			unsigned bgPeak = PeakSepaArgs[index].StartPeakNo;
			unsigned edPeak = PeakSepaArgs[index].FinishPeakNo;
			bool finish = false;
			bool isPlus = true; //�Ƿ�����
			if (Peak[bg].Type > 0)
			{
				isPlus = true;
			}
			else
			{
				isPlus = false;
			}
			{//����������ص����������Ƿ�ȫ��������򸺷�
				unsigned tmp_bg = bg;
				unsigned tmp_ed = ed;
				for (unsigned tmp=tmp_bg; tmp<=tmp_ed; ++tmp)
				{
					if (isPlus)
					{
						if (Peak[tmp].Type < 0)
							ASSERT(TRUE);
					}
					else
					{
						if (Peak[tmp].Type > 0)
							ASSERT(TRUE);
					}
				}
			}

loop:
			while ((ed!=bg) || (!finish))
			{
				if (bg==ed) //��������������⣬û�п��Ǹ���
				{
					Peak[bg].Type = abs(Peak[bg].Type)/Peak[bg].Type;
				}

				double k = (Peak[ed].FinSema.y-Peak[bg].StartSema.y)/(Peak[ed].FinSema.x-Peak[bg].StartSema.x);
				double c = Peak[bg].StartSema.y-k*Peak[bg].StartSema.x;
				for (unsigned i=Peak[bg].StartSema.idx; i<Peak[ed].FinSema.idx; ++i)
				{
					double line_y = handleSema[i].x*k+c;
					if (isPlus && (bg!=ed))    //�����������
					{
						if ( (line_y>0 && fabs(line_y) > fabs(handleSema[i].y) ) ||
							 (line_y<0 && fabs(line_y) < fabs(handleSema[i].y) ) )
						{
							ed--;
							break; //�����ϵ���������ϵĵ㣬�������һ����
						}
					}
					if ((!isPlus) && fabs(line_y) > fabs(handleSema[i].y) && (bg!=ed)) //���Ǹ������
					{
						if ( (line_y<0 && fabs(line_y) > fabs(handleSema[i].y) ) ||
							(line_y>0 && fabs(line_y) < fabs(handleSema[i].y) ) )
						{
							ed--;
							break; //�����ϵ�С�������ϵĵ㣬�������һ����
						}
						
					}
					if (i==Peak[ed].FinSema.idx-2 || (bg==ed))
					{
						test_GetDividePeakPoint(Peak, bg, ed);//��ǰ������Ļ���,[bg,ed]������ȷ��Ϊ��ֱ�ַ�
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

		}
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
VOID CA_DividePeak(const std::vector<CSema> &OrigPeak, 
	std::vector<CPeak> &Peak,
	vector<AnalysisConfigArguments::PeakSepaArg> &SepaArgs)        
{// ��ֱ�ַ�    
	if (SepaArgs.empty()) return;
	//�Ȳ��Ե�һ������
	const unsigned SepaArgsSize = SepaArgs.size();
	for (unsigned ss=0; ss<SepaArgsSize; ++ss)
	{
		if (SepaArgs[ss].SepaType != 1) continue;
		
		unsigned sPNo = SepaArgs[ss].StartPeakNo;
		unsigned ePNo = SepaArgs[ss].FinishPeakNo;

		bool isplus = true;
		if (Peak[sPNo].Type > 0) isplus = true;
		else isplus = false;
		

	loop: //gotoѭ��
		for ( ; sPNo <= ePNo; --ePNo)
		{
			//�����յ������
			double _k = (Peak[ePNo].FinSema.y-Peak[sPNo].StartSema.y)/(Peak[ePNo].FinSema.x-Peak[sPNo].StartSema.x);
			double _b = Peak[sPNo].StartSema.y - _k*Peak[sPNo].StartSema.x;
			for (unsigned i=Peak[sPNo].StartSema.idx; i<Peak[ePNo].FinSema.idx; ++i)
			{ 
				double Y0 = _k * OrigPeak[i].x + _b; //YO>y��ֱ���ϣ�YO<y��ֱ���·���=y����ֱ����
				if (isplus) //�������
				{
					if ( (Y0>0 && fabs(Y0) > fabs(OrigPeak[i].y) ) || 
						(Y0<0 && fabs(Y0) < fabs(OrigPeak[i].y) ) ) 
					{
						if (sPNo==ePNo)
						{
							sPNo++;
							ePNo=SepaArgs[0].FinishPeakNo;
							goto loop;
						}
						ePNo--;
						goto loop;
					}
				}
				else
				{	//�������
					if (fabs(Y0) > fabs(OrigPeak[i].y)) 
					{
						if ( (Y0<0 && fabs(Y0) > fabs(OrigPeak[i].y) ) ||
							(Y0>0 && fabs(Y0) < fabs(OrigPeak[i].y) ) )
						{
							if (sPNo==ePNo)
							{
								sPNo++;
								ePNo=SepaArgs[0].FinishPeakNo;
								goto loop;
							}
							ePNo--;
							goto loop;
						}
					}
				}
			}
			
			//��ȫ����ֱ���ϣ�˵����η��������ֱ����
			unsigned s=sPNo;
			unsigned e=ePNo;
			//���[s,e]�ڵķ����ͱ��
			int typeLab;
			for (unsigned i=s; i<=e; ++i)
			{
				if (abs(Peak[i].Type) != 1)
				{
					typeLab = Peak[i].Type;
					break;
				}
			}
			for (; s<e; ++s)
			{
				Peak[s].dType = CPeak::VERTDIVIDE;
				Peak[s+1].dType = CPeak::VERTDIVIDE;
				Peak[s].Type = typeLab;
				Peak[s+1].Type = typeLab;
				Peak[s].FinSema.y = _k*Peak[s].FinSema.x+_b;
				Peak[s+1].StartSema = Peak[s].FinSema;
			
			}
			if (sPNo==ePNo)
			{
				sPNo++;
				ePNo=SepaArgs[ss].FinishPeakNo;
			}
			else
			{
				sPNo=ePNo;
				ePNo=SepaArgs[ss].FinishPeakNo;
			}
		}
	}
}


//-----------------------------------------------------------------------------
//���ܣ���ֹ����
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ
//			2.vector<unsigned> &BanPeakNo ��ֹ���ַ��
//			
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_BanPeak(std::vector<CPeak> &Peak,
	vector<unsigned> &BanPeakNo)
{
	vector<CPeak> buffPeak;
	const unsigned PeakSize = Peak.size();
	const unsigned BanPeakNoSize = BanPeakNo.size();
	for (unsigned i=0; i<PeakSize; ++i)
	{
		bool flag = true; //��ǵ�
		for (unsigned j=0; j<BanPeakNoSize; ++j)
		{
			if (i == BanPeakNo[j])
			{
				flag = false; //���ɾ�����Ŵ��ڣ��򲻷����������൱��ɾ������
			}
		}
		if (flag)
		{
			buffPeak.push_back(Peak[i]); 
		}
	}

	Peak.swap(buffPeak);
}



//-----------------------------------------------------------------------------
//���ܣ�ɾ��֮ǰ�����еİ���ֵ����Ա����m_cArrayHPW
//-----------------------------------------------------------------------------
VOID CA_GetAllHPW(std::vector<CPeak> &Peak, CArray<double> &cArrayHPW)
{
	if (cArrayHPW.GetSize()!=0)
		cArrayHPW.RemoveAll();
	
	const size_t PeakSize = Peak.size();
	for (size_t i=0; i<PeakSize; ++i)
	{
		cArrayHPW.Add(Peak[i].HalfPeakWidth);
	}
}


//-----------------------------------------------------------------------------
//���ܣ��ֶ���С����ɾ����Ҫ��
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ
//			2.vector<double> &MinHalfWidth ÿ�����Ӧ�ֶ���С����
//			
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_EraseByHalfPeakWidth(std::vector<CPeak> &Peak,
	vector<double> &MinHalfWidth)
{
	const unsigned PeakSize = Peak.size();
	const unsigned MinHalfWidthSize = MinHalfWidth.size();
	if (PeakSize!=MinHalfWidthSize)	ASSERT(TRUE);

	std::vector<CPeak> buffPeak; //��ʱ��������
	
	//Ѱ��һ��������
	int lastType = 0; //��һ�β�������

	//����������������
	for (unsigned i=0; i<PeakSize; ++i)
	{
		bool flag = false; //������ص��壬ֻ����һ���������С�������еĶ���ȥ��
		unsigned StartMarkPoint = i; //���������Ŀ�ʼ�±�
		unsigned FinishMarkPoint = i; //���������Ľ����±�

		if (abs(Peak[i].Type)==1) //����
		{
			if (Peak[i].HalfPeakWidth - MinHalfWidth[i] >= 0.000000)//������С����
			{
				buffPeak.push_back(Peak[i]);
			}
		}
		else //������
		{
			lastType = Peak[i].Type; //��¼��������ʼ����
			while ( lastType == Peak[i].Type && i<PeakSize) //ֱ�����������
			{
				if (Peak[i].HalfPeakWidth - MinHalfWidth[i] >= 0.000000) //������С����
				{
					flag = true;
				}
				FinishMarkPoint = i; //�ı�������Ľ����±�
				i++;
				if (i==PeakSize) //��ֹԽ��
				{
					FinishMarkPoint = i-1;
					break;
				}
			}
			if (flag) //�����������ķ�װ����ʱ����
			{
				for (unsigned j = StartMarkPoint; j<=FinishMarkPoint; ++j)
				{
					buffPeak.push_back(Peak[j]);
				}
				flag = !flag;
			}
			i = FinishMarkPoint;  //���¶�λi���±�
		}
	}
	Peak.swap(buffPeak);
}



//-----------------------------------------------------------------------------
//���ܣ������ȫ���Ե��������
//���������
//			1.std::vector<CSema> &handleSema Ԫ�����ź���
//			2.std::vector<CPeak> &Peak		 ����Ϣ
//			3.
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_GetHalfPeakWidth(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak)
{
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = Peak[i].StartSema.x;
		double y1 = Peak[i].StartSema.y;
		double x2 = Peak[i].FinSema.x;
		double y2 = Peak[i].FinSema.y;

		double peakWidth = fabs(x2-x1);

		double tana = (y2-y1)/(x2-x1);
		double c = y1 - x1 * tana;

		double py = Peak[i].PeakSema.y; 
		double px = Peak[i].PeakSema.x;
		double ty = px*tana+c;

		double hx = px;
		double hy = (py-ty)/2 + ty;
		double _c = hy-hx*tana;		

		double diff_y = INT_MAX;
		unsigned idxl, idxr;

			for (unsigned l=Peak[i].StartSema.idx; l<=Peak[i].PeakSema.idx; ++l) //���У��ƽ��е�
			{
				double ly = handleSema[l].x * tana + _c;
				if (diff_y >= fabs(handleSema[l].y - ly))
				{
					diff_y = fabs(handleSema[l].y - ly);
					idxl = l;
				}
			}
			CSema tmpFront(handleSema[idxl]); //������
			Peak[i].MidStartSema = tmpFront;
		
		diff_y = INT_MAX;

			for (unsigned r=Peak[i].PeakSema.idx; r<=Peak[i].FinSema.idx; ++r) //���У��ƽ��е�
			{
				double ry = handleSema[r].x * tana + _c;

				if (diff_y >= fabs(handleSema[r].y - ry))
				{
					diff_y = fabs(handleSema[r].y - ry);
					idxr = r;
				}
			}
			CSema tmpLast(handleSema[idxr]);   //������
			Peak[i].MidFinSema = tmpLast;
		
		double hk = (Peak[i].MidFinSema.y-Peak[i].MidStartSema.y)/(Peak[i].MidFinSema.x-Peak[i].MidStartSema.x);
		double hc = Peak[i].MidStartSema.y - Peak[i].MidStartSema.x * hk;
		double rey = hx*hk+hc;
		CSema HalfPeak(Peak[i].PeakSema.idx, hx, rey); // ����е�
		Peak[i].MidPeakSema = HalfPeak;
	}

	//���Ӱ���ֱ�ߣ��벨��Ĵ����ཻ��һ�㡣���������ǰ�󳤶�
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = Peak[i].MidStartSema.x; 
		double y1 = Peak[i].MidStartSema.y;
		double x2 = Peak[i].MidFinSema.x; 
		double y2 = Peak[i].MidFinSema.y;
		
		double halfPeakWidth = fabs(x2-x1);
		Peak[i].HalfPeakWidth = halfPeakWidth;  //�����
	}
}


//-----------------------------------------------------------------------------
//���ܣ��ֹ��ַ�
//���������
//			1.std::vector<CSema> &handleSema Ԫ�����ź���
//			2.std::vector<CPeak> &Peak		 ����Ϣ
//			3.const int index,               �ַ��±�
//			4.const double Spra_t            �ַ�ʱ��
//���������
//			1.�ֹ��ַ��Ƿ�ɹ�
//			2.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
bool CA_HandPeakSpration(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	const int index,     
	const double Spra_t)
{
	//�ҵ��ַ�ʱ����ԭ�����ϵĵ�
	if (index < 0 || index >= (int)Peak.size())
		return false;
	unsigned SpIndex = INT_MAX; //�ַ���ԭ�����ϵĵ���±�
	double diff = (double)INT_MAX;
	for (unsigned i=Peak[index].StartSema.idx; i<Peak[index].FinSema.idx; ++i)
	{
		if (fabs(handleSema[i].x - Spra_t) <= diff)
		{
			diff = fabs(handleSema[i].x - Spra_t);
			SpIndex = i;
		}
	}

	if (SpIndex==INT_MAX) return false; //δ�ҵ��±�

	//����
	if (Peak[index].Type > 0)
	{
		bool bFindLeft = false;
		bool bFindRight = false;
		CSema leftSema; //��ߵķ�
		double leftMax = static_cast<double>(-INT_MAX);
		for (unsigned l=SpIndex; l>Peak[index].StartSema.idx; --l)
		{//������ߣ��ҵ�y��仯���ơ����ȫ���½�����Ѱ��ʧ��
			if (handleSema[l].y <= handleSema[l-1].y)
			{
				//leftSema = handleSema[l-1];
				bFindLeft = true;
			}
			if (bFindLeft && fabs(handleSema[l].y) >= leftMax) //�б仯���ƺ�ſ�ʼ�����ߵ�
			{
				leftMax = fabs(handleSema[l].y);
				leftSema = handleSema[l];
			}
		}

		CSema rightSema; //�ұߵķ�
		double rightMax = static_cast<double>(-INT_MAX);
		for (unsigned r=SpIndex; r<Peak[index].FinSema.idx-1; ++r)
		{
			if (handleSema[r+1].y >= handleSema[r].y)
			{
				//rightSema = handleSema[r+1];
				bFindRight = true;
			}
			if (bFindRight && fabs(handleSema[r].y) >= rightMax)
			{
				rightMax = fabs(handleSema[r].y);
				rightSema = handleSema[r];
			}
		}
		if (bFindLeft && bFindRight) //���Էַ�
		{
			//unsigned leftSemaIdx;
			//unsigned rightSemaIdx;
			////�ж������ҷ���һ���Ǳ����Ĳ��壬����һ�����µķַ�
			//if (leftMax - Peak[index].PeakSema.y = 0.0 ) //�����Ԫ��
			//{
			//	leftSemaIdx = 
			//}
			//���ֹ��ַ��ʱ����У��
			diff = (double)INT_MAX;
			for (unsigned i=leftSema.idx; i<rightSema.idx; ++i)
			{
				if (fabs(handleSema[i].y)<=diff)
				{
					diff = fabs(handleSema[i].y);
					SpIndex = i; //�õ���ȷ�ķַ�ʱ�䣬ԭ���ߵ��±�
				}
			}
			//�Է������ķ������У��
			int pType = 1;
			std::vector<CPeak> buffVect;
			for (unsigned i=0; i<(unsigned)index; ++i)
			{	if (Peak[i].Type!=1)
					pType = Peak[i].Type;
				buffVect.push_back(Peak[i]); //ǰ�벿��
			}
		
			CPeak addOne(pType+1, handleSema[SpIndex], rightSema, Peak[index].FinSema); //tmp
			Peak[index].FinSema = handleSema[SpIndex];
			Peak[index].PeakSema = leftSema;
			Peak[index].Type = pType+1;
			//�����߷ַ�ֱ����Ϊ��ֱ�ַ�
			double x1 = Peak[index].StartSema.x;
			double y1 = Peak[index].StartSema.y;
			double x2 = addOne.FinSema.x;
			double y2 = addOne.FinSema.y;
			double k = (y2-y1)/(x2-x1);
			double c = y1 - k * x1;
			double old_x = Peak[index].FinSema.x;
			double new_y = old_x * k + c;
			Peak[index].FinSema.y = new_y;
			addOne.StartSema.y = new_y;
			//������߷ַ�Ϊ��ֱ�ַ�
			Peak[index].dType = CPeak::VERTDIVIDE;
			addOne.dType = CPeak::VERTDIVIDE;

			buffVect.push_back(Peak[index]);
			buffVect.push_back(addOne);

			for (unsigned i=index+1; i<Peak.size(); ++i)
			{
				if (Peak[i].Type!=1)
					Peak[i].Type += 1;
				buffVect.push_back(Peak[i]); //��벿��
			}
			Peak.swap(buffVect);
			return true;
		}
	}
	else //����
	{
		bool bFindLeft = false;
		bool bFindRight = false;
		CSema leftSema; //��ߵķ�
		double leftMax = static_cast<double>(-INT_MAX);
		for (unsigned l=SpIndex; l>Peak[index].StartSema.idx; --l)
		{//������ߣ��ҵ�y��仯���ơ����ȫ���½�����Ѱ��ʧ��
			if (handleSema[l].y >= handleSema[l-1].y)
			{
				//leftSema = handleSema[l-1];
				bFindLeft = true;
			}
			if (bFindLeft && fabs(handleSema[l].y) >= leftMax) //�б仯���ƺ�ſ�ʼ�����ߵ�
			{
				leftMax = fabs(handleSema[l].y);
				leftSema = handleSema[l];
			}
		}

		CSema rightSema; //�ұߵķ�
		double rightMax = static_cast<double>(-INT_MAX);
		for (unsigned r=SpIndex; r<Peak[index].FinSema.idx-1; ++r)
		{
			if (handleSema[r+1].y <= handleSema[r].y)
			{
				//rightSema = handleSema[r+1];
				bFindRight = true;
			}
		}
		if (bFindLeft && bFindRight) //���Էַ�
		{
			//���ֹ��ַ��ʱ����У��
			diff = static_cast<double>(-INT_MAX);
			for (unsigned i=leftSema.idx; i<rightSema.idx; ++i)
			{
				if (handleSema[i].y>=diff)
				{
					diff = handleSema[i].y;
					SpIndex = i; //�õ���ȷ�ķַ�ʱ�䣬ԭ���ߵ��±�
				}
			}
			//�Է������ķ������У��
			int pType = -1;
			std::vector<CPeak> buffVect;
			for (unsigned i=0; i<(unsigned)index; ++i)
			{	
				if (Peak[i].Type!=-1)
					pType = Peak[i].Type;
				buffVect.push_back(Peak[i]); //ǰ�벿��
			}

			CPeak addOne(pType-1, handleSema[SpIndex], rightSema, Peak[index].FinSema); //tmp
			Peak[index].FinSema = handleSema[SpIndex];
			Peak[index].PeakSema = leftSema;
			Peak[index].Type = pType-1;
			buffVect.push_back(Peak[index]);
			buffVect.push_back(addOne);

			for (unsigned i=index+1; i<Peak.size(); ++i)
			{
				if (Peak[i].Type!=-1)
					Peak[i].Type -= 1;
				buffVect.push_back(Peak[i]); //��벿��
			}
			Peak.swap(buffVect);
			return true;
		}
	}
	return false;
}







//-----------------------------------------------------------------------------
//���ܣ��ֹ����壨��㣩 
//���������
//			1.std::vector<CSema> &handleSema Ԫ�����ź���
//			2.std::vector<CPeak> &Peak		 ����Ϣ
//			3.vector<double> &Arg            �ֹ����壨��㣩����
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_HandAdjustStart(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<double> &Arg)
{
	const unsigned ArgSize =  Arg.size();
	const unsigned PeakSize = Peak.size();
	const unsigned SemaSize = handleSema.size();
	vector<double> buffVect; //��ʱ����������Arg�����ݶԻ�
	
	for (unsigned ai=0; ai<ArgSize; ++ai)
	{
		unsigned Ano = UINT_MAX; //�������ķ��
		double diff = (double)INT_MAX;
		//�������������������ҵ�������Ǹ���ʼ�㣬ȷ�����
		for (unsigned i=0; i<PeakSize; ++i)
		{
			double CurrentDiff = fabs(Arg[ai]-Peak[i].StartSema.x);
			if (CurrentDiff - diff == 0.00000000)
			{//����������
				::AfxMessageBox(_T("�����ĵ����ǰ��2����㳤����ͬ"));
				return;
			}
			if (CurrentDiff < diff)
			{
				diff = CurrentDiff;
				Ano = i;
			}
		}
		//�Ѿ��ҵ����,�Է�Ž����ж��Ƿ��������������
		bool clear = false; //�Ƿ������������
		if (Ano!=UINT_MAX)
		{
			if (Ano == 0)//��һ��������
			{
				if (Peak[Ano].PeakSema.x > Arg[ai])
					clear = true;
			}
			else //�ڶ������Ժ�����
			{
				if (abs(Peak[Ano].Type)==1) //����
				{
					if ((Peak[Ano].PeakSema.x > Arg[ai])
						&&(Peak[Ano-1].FinSema.x <= Arg[ai]))
					{
						clear = true;
					}
				}
				else
				{
					if ((Peak[Ano].PeakSema.x > Arg[ai])
						&&(Peak[Ano-1].PeakSema.x <= Arg[ai]))
					{
						clear = true;
					}
				}
				
			}
		}
		if (clear) //�ֹ�������Խ���
		{
			unsigned left = 0;
			unsigned right = 0;
			if (Ano == 0)
				right = Peak[Ano].PeakSema.idx;
			else
			{
				left = Peak[Ano-1].PeakSema.idx;
				right = Peak[Ano].PeakSema.idx;
			}
		
			//�ҵ����������Ӧԭ�����ϵĵ�
			for (unsigned index = left; index < right; ++index)
			{
				if ((handleSema[index].x <= Arg[ai])
				  &&(handleSema[index+1].x >= Arg[ai]))
				{
					buffVect.push_back(handleSema[index].x); //��ǰ���������ȷ����У��
					//�ֹ�����
					if (abs(Peak[Ano].Type)==1) //����
						Peak[Ano].StartSema = handleSema[index];
					else //������
					{
						Peak[Ano].StartSema = handleSema[index]; //���ı�
						Peak[Ano-1].FinSema = Peak[Ano].StartSema; //��һ��������յ�ı�
					}
				}
			}
		}
	}
	Arg.swap(buffVect);
}







//-----------------------------------------------------------------------------
//���ܣ��ֹ����壨�յ㣩 
//���������
//			1.std::vector<CSema> &handleSema Ԫ�����ź���
//			2.std::vector<CPeak> &Peak		 ����Ϣ
//			3.vector<double> &Arg            �ֹ����壨��㣩����
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_HandAdjustFinish(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<double> &Arg)
{
	const unsigned ArgSize =  Arg.size();
	const unsigned PeakSize = Peak.size();
	const unsigned SemaSize = handleSema.size();
	vector<double> buffVect; //��ʱ����������Arg�����ݶԻ�

	for (unsigned ai=0; ai<ArgSize; ++ai)
	{
		unsigned Ano = UINT_MAX; //�������ķ��
		double diff = (double)INT_MAX;
		//�������������������ҵ�������Ǹ���ֹ�㣬��ȷ�����
		for (unsigned i=0; i<PeakSize; ++i)
		{
			double CurrentDiff = fabs(Arg[ai]-Peak[i].FinSema.x); 
			if (CurrentDiff - diff == 0.00000000)
			{//����������
				::AfxMessageBox(_T("�����ĵ����ǰ��2����㳤����ͬ"));
				return;
			}
			if (CurrentDiff < diff)
			{
				diff = CurrentDiff;
				Ano = i;
			}
		}
		//�Ѿ��ҵ����,�Է�Ž����ж��Ƿ��������������
		bool clear = false; //�Ƿ������������
		if (Ano!=UINT_MAX)
		{
			if (Ano == PeakSize-1)//���һ��������
			{
				if (Peak[Ano].PeakSema.x < Arg[ai])
					clear = true;
			}
			else //��һ���嵽���һ����֮������
			{
				if (abs(Peak[Ano].Type)==1) //����
				{
					if ((Peak[Ano].PeakSema.x < Arg[ai])
						&&(Peak[Ano+1].StartSema.x >= Arg[ai]))
					{
						clear = true;
					}
				}
				else
				{
					if ((Peak[Ano].PeakSema.x < Arg[ai])
						&&(Peak[Ano+1].PeakSema.x >= Arg[ai]))
					{
						clear = true;
					}
				}

			}
		}
		if (clear) //�ֹ�������Խ���
		{
			unsigned left = 0;
			unsigned right = 0;
			if (Ano == PeakSize-1)
			{
				left = Peak[Ano].PeakSema.idx;
				right = Peak[Ano].FinSema.idx;
			}
			else
			{
				left = Peak[Ano].PeakSema.idx;
				right = Peak[Ano+1].PeakSema.idx;
			}

			//�ҵ����������Ӧԭ�����ϵĵ�
			for (unsigned index = left; index < right; ++index)
			{
				if ((handleSema[index].x <= Arg[ai])
					&&(handleSema[index+1].x >= Arg[ai]))
				{
					buffVect.push_back(handleSema[index].x); //��ǰ���������ȷ����У��
					//�ֹ�����
					if (abs(Peak[Ano].Type)==1) //����
					{
						Peak[Ano].FinSema = handleSema[index];
						break;
					}
					else //������
					{
						Peak[Ano].FinSema = handleSema[index]; //���ı�
						Peak[Ano+1].StartSema = Peak[Ano].FinSema; //��һ��������յ�ı�
						break;
					}
				}
			}
		}
	}
	Arg.swap(buffVect);
}







//-----------------------------------------------------------------------------
//���ܣ��ֹ����壨��ȣ� 
//���������
//			1.std::vector<CSema> &handleSema Ԫ�����ź���
//			2.std::vector<CPeak> &Peak		 ����Ϣ
//			3.vector<double> &Arg            �ֹ����壨��ȣ�����
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_HandAdjustPeak(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<double> &Arg)
{
	const unsigned ArgSize =  Arg.size();
	const unsigned PeakSize = Peak.size();
	const unsigned SemaSize = handleSema.size();
	//���ֹ����壨��ȣ��������б���
	for (unsigned ArgIndex = 0; ArgIndex<ArgSize; ++ArgIndex)
	{
		for (unsigned i=0; i<PeakSize; ++i) //�ҵ���Ҫ�����ķ��λ���ĸ�����
		{
			if((Arg[ArgIndex] > Peak[i].StartSema.x)
				&&(Arg[ArgIndex] < Peak[i].FinSema.x)) //�ҵ�
			{
				//��ԭ��Ԫ������
				unsigned left = Peak[i].StartSema.idx;
				unsigned right = Peak[i].FinSema.idx;
				for (unsigned j=left; j<right-1; ++j)
				{
					if ((Arg[ArgIndex] >= handleSema[j].x)
					&&(Arg[ArgIndex] <= handleSema[j+1].x))//��ԭ
					{//�Ƿ�Ҫ��׼������һ���㣬�Աȸ��������һ��
						Peak[i].PeakSema = handleSema[j];
						Arg[ArgIndex] = handleSema[j].x; //У�����ֹ����壨��ȣ�
					}
				}
			}
		}
	}
}







//-----------------------------------------------------------------------------
//���ܣ��ϲ��� 
//���������
//			1.std::vector<CSema> &handleSema     Ԫ�����ź���
//			2.std::vector<CPeak> &Peak		     ����Ϣ
//			3.vector<pair<double,double> > &Arg  �ֹ����壨��ȣ�����
//			5.AnalysisConfigArguments* pCA       ���ò����ṹָ��
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_MergePeak(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<pair<double,double> > &Arg,
	AnalysisConfigArguments* pCA)
{//�Ժ���������ϲ�����������ƣ��Ժ��衾�Ż���
	const unsigned ArgsSize = Arg.size();
	const unsigned PeakSize = Peak.size();
	const unsigned SemaSize = handleSema.size();

	for (unsigned ai=0; ai<ArgsSize; ++ai)
	{
		CSema startSema, peakSema, finishSeam;

		double start = Arg[ai].first;   //���
		double end =   Arg[ai].second;  //�յ�
		unsigned sNo = INT_MAX; //�ϲ�������±�
		unsigned eNo = INT_MAX; //�ϲ����յ��±�
		//���������ȷ�����ڵ�N����֮ǰ
		for (unsigned pi=0; pi<PeakSize; ++pi)
		{
			if (Peak[pi].PeakSema.x >= start) //�ж�������ֻҪ����������
			{
				sNo = pi;
				break;
			}
		}
		//�������յ�ȷ�����ڵ�N����֮��
		for (unsigned pi=PeakSize-1; pi>=0; --pi)
		{
			if (Peak[pi].PeakSema.x <= end)
			{
				eNo = pi;
				break;
			}
		}
		//�Ժϲ����±��ж�
		if (sNo!=INT_MAX && eNo!=INT_MAX)
		{
			unsigned diff = eNo - sNo;
			if (diff >= 1)//�ϲ�������������������Ժϲ�
			{
				//����㻹ԭ��ԭ����
				unsigned left = INT_MAX;  //�ϲ�������±�
				unsigned right = INT_MAX; //�ϲ����յ��±�
				unsigned startIndex = 0;
				if (sNo!=0)
					startIndex = Peak[sNo-1].PeakSema.idx;
				for (unsigned si=startIndex; si<SemaSize-1; ++si)
				{
					if ((start>=handleSema[si].x)
					&&(start<=handleSema[si+1].x))
					{
						start = handleSema[si].x; //���У��
						startSema = handleSema[si];
						left = si;
						break;
					}
				}
				//���յ㻹ԭ��ԭ����
				unsigned endIndex = handleSema[SemaSize-1].idx;
				if (eNo!=PeakSize-1)
					endIndex = Peak[eNo+1].PeakSema.idx;
				for (unsigned si=endIndex; si>0; --si)
				{
					if ((end<=handleSema[si].x)
					&& (end>=handleSema[si-1].x))
					{
						end = handleSema[si].x; //�յ�У��
						finishSeam = handleSema[si];
						right = si;
						break;
					}
				}
				//�ҳ������ڵ����ֵ��Ϊ���
				pair<double,unsigned> maxVal = std::make_pair(static_cast<double>(-INT_MAX),0);
				pair<double,unsigned> minVal = std::make_pair(static_cast<double>( INT_MAX),0);
				//unsigned minusIndex = 0; //��¼��Сֵ���±꣨���壩
				double point = 0.0;
				for (unsigned i=left; i<right; ++i)
				{
					if (handleSema[i].y >= maxVal.first)
					{
						maxVal.first = handleSema[i].y; //�ҵ��������ڵ����ֵ
						maxVal.second = i;
						continue;
					}

					if (handleSema[i].y <= minVal.first)
					{
						minVal.first = handleSema[i].y; //�ҵ��������ڵ���Сֵ
						minVal.second = i;
						point = handleSema[i].x;
						continue;
					}
				}
				//���廹�ǲ��ȵ�ȡ��
				//double realVal = 0.0; //����������ȷ���ķ�ֵ
				unsigned realIndex = 0; //����������ȷ���ķ�ֵ���±�
				if (maxVal.first > fabs(minVal.first))
				{
					realIndex = maxVal.second;//�����ھ����Ҳ���
				}
				else if (maxVal.first < fabs(minVal.first))
				{
					//�жϸ���Сֵ�Ƿ��ڸ���������
					//�ǣ���Сֵ��Ϊ����
					//����Ȼ�����ֵ��Ϊ����
					if (pCA->GetMinusCheakStatus(point))
						realIndex = minVal.second;
					else
						realIndex = maxVal.second;
				}
				else
				{//����������
					::AfxMessageBox(_T("�ϲ�����������Сֵ�����ֵ��ֵ��ͬ"));
					return;
				}



				//�����еĲ���������������
				vector<CPeak> buffPeak;
				for (unsigned pi=0; pi!=sNo; ++pi)
				{
					buffPeak.push_back(Peak[pi]);
				}
				if (realIndex > SemaSize)
				{
					TRACE(_T("(realIndex > SemaSize)"));
					return;
				}
				
				CPeak peak(1, startSema, handleSema[realIndex], finishSeam);
				buffPeak.push_back(peak);

				for (unsigned pi=eNo+1; pi<PeakSize; ++pi)
				{
					buffPeak.push_back(Peak[pi]);
				}
				Peak.swap(buffPeak);
			}
		}
	}
}


//-----------------------------------------------------------------------------
//���ܣ����ӷ� 
//���������������ڵ�һ�������ҳ���ֵ���γɲ��塣3������������ϡ�
//���ʣ��Ƿ���Ҫ�����ӷ������ߵ�����
//      �Ƿ���Ҫ��Ƿ�ķ����ͣ������ֹ����壬�ֹ�����
//-----------------------------------------------------------------------------
VOID CA_AddPeak(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	deque<ConfigArgument> &AddArgs)
{
	const unsigned OrigSemaSize = handleSema.size();
	while (!AddArgs.empty())
	{
		//1,�ҵ�ԭ�����ϵ���ʼ��
		bool bfStart = false;
		CSema StartPoint;
		double starttime = AddArgs[0].BeginTime;
		for (unsigned i=0; i<OrigSemaSize-1; ++i)
		{
			if (handleSema[i].x <= starttime && handleSema[i+1].x >= starttime)
			{
				//starttime = handleSema[i+1].x;
				bfStart = true;
				StartPoint = handleSema[i+1];
				break;
			}
		}

		//2,�ҵ�ԭ�����ϵ���ֹ��
		bool bfEnd = false;
		CSema EndPoint;
		double endtime = AddArgs[0].EndTime;
		for (unsigned i=0; i<OrigSemaSize-1; ++i)
		{
			if (handleSema[i].x <= endtime && handleSema[i+1].x >= endtime)
			{
				//endtime = handleSema[i];
				bfEnd = true;
				EndPoint = handleSema[i];
				break;
			}
		}

		//3,��Ԫ���ߵĲ���򲨹�
		CSema PeakPoint;
		bool bFindPeak = false;
		double dPeakType = AddArgs[0].InputValue;
		if (bfStart && bfEnd)
		{
			
			if (dPeakType - 1 == 0.000000) //�Ҳ���
			{
				
				for (unsigned i = StartPoint.idx; i<EndPoint.idx; ++i)
				{
					if (handleSema[i].y >= PeakPoint.y)
					{
						PeakPoint = handleSema[i];
						bFindPeak = true;
					}
				}
			}
			else if (dPeakType + 1 == 0.000000)
			//�Ҳ���
			{
				PeakPoint.y = (double)INT_MAX;
				for (unsigned i = StartPoint.idx; i<EndPoint.idx; ++i)
				{
					if (handleSema[i].y <= PeakPoint.y)
					{
						PeakPoint = handleSema[i];
						bFindPeak = true;
					}
				}
			}
		}

		//4,��һ��3�����������������
		if (bFindPeak)
		{
			CPeak peak(StartPoint, PeakPoint, EndPoint);
			//��֮���ӷ�������Ϊ���壬��ʱ�������ڳ�Ѫ˫�����û����и���
			if (dPeakType - 1 == 0.000000)
			{
				peak.Type = 1;
			}
			else if (dPeakType + 1 == 0.000000)
			{
				peak.Type = -1;
			}
			Peak.push_back(peak);

			CA_PeakValleyBaseLineCheck(handleSema, Peak, 
				Peak.size()-1,
				Peak.size()-1); //��Ȼ���У��
		}
	

		AddArgs.pop_front();
	}
	//�Ƿ���Ҫ�Խ����������
	
}





VOID CA_NewPeakValleyBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end) 
{
}





VOID CA_PeakValleyBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end) 
{// ��Ȼ���У���㷨
	if (begin > end)
	{
		ASSERT(_T("(begin > end)"));
		return;
	}

	const unsigned PeakSize = Peak.size();
	unsigned bg = begin;
	unsigned ed = end;
	bool getIndex = false; 

	deque<bool> MinusCheck(PeakSize, false); //�ֶθ�����
	for (unsigned i=begin; i<=end; ++i)
	{
		if (Peak[i].Type < 0)
		{
			MinusCheck[i] = true;
		}
	}

	for (; begin<=end; ++begin)
	{  
		bg = begin;
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
				if (Peak[ed].Type!=mark || Peak[ed].dType!=CPeak::PEAKVALLEY)
				{
					ed--;
					getIndex = true;
					break;
				}
				if (ed+1==PeakSize)
				{
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
					double k = (handleSema[Peak[i].FinSema.idx].y-handleSema[j].y)/(handleSema[Peak[i].FinSema.idx].x-handleSema[j].x);
					double c = handleSema[j].y - k * handleSema[j].x;

					for (unsigned m=j; m<=Peak[i].PeakSema.idx; ++m) 
					{
						double _y = k * handleSema[m].x + c;
						if (MinusCheck[i]) //���Ǹ������
						{
							if ( (_y <= 0 && (fabs(handleSema[m].y) < fabs(_y))) ||
							   	 (_y > 0  && (fabs(handleSema[m].y) > fabs(_y))) )
							{
								isfail = true;
								break;
							}
						}
						else 
						{
							if ( (_y <= 0 && (fabs(handleSema[m].y) > fabs(_y))) ||
								 (_y > 0  && (fabs(handleSema[m].y) < fabs(_y))) )
							{
								isfail = true;
								break;
							}
						}
						isfail = false;
					}
					if (!isfail)
					{
						Peak[i].StartSema = handleSema[j];
						isfail = false;
					}
				} isfail = true;

				for (unsigned l=Peak[i].FinSema.idx; l>=Peak[i].PeakSema.idx && isfail; --l)
				{
					//double k = (handleSema[Peak[i].FinSema.idx].y-handleSema[j].y)/(handleSema[Peak[i].FinSema.idx].x-handleSema[j].x);
					double k = (handleSema[l].y-handleSema[j].y)/(handleSema[l].x-handleSema[j].x);
					double c = handleSema[l].y-k*handleSema[l].x;
					
					for (unsigned n=l; n>=Peak[i].PeakSema.idx; --n)
					{
						double _y = k * handleSema[n].x + c;
						if (MinusCheck[i]) //���Ǹ������
						{
							
							if ((handleSema[n].y <= 0 && (fabs(handleSema[n].y) < fabs(_y))) ||
								(handleSema[n].y > 0  && (fabs(handleSema[n].y) > fabs(_y))) )
							{
								isfail = true;
								break;
							}
						}
						else  
						{
							if ( (_y <= 0 && (fabs(handleSema[n].y) > fabs(_y))) ||
								(_y > 0  && (fabs(handleSema[n].y) < fabs(_y))) )
							{
								isfail = true;
								break;
							}
						}
						isfail = false;				
					}
					if (!isfail)
					{
						Peak[i].FinSema = handleSema[l];
						isfail = false;
					}
				}
				i++;	
			} while (i!=o+1);
			getIndex = false;
		}
		begin = ed;
	}
}







//-----------------------------------------------------------------------------
//���ܣ���ֱ�ַ�����У��
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak		������
//          3.unsigned begin			��ʼ����
//          4.unsigned end				��������
//���������
//			1.vector<CPeak> &Peak ����У����Ĳ�
//-----------------------------------------------------------------------------
VOID CA_VertDivideBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end)
{
	//��ֱ�ַ����У��
	if (begin > end)
	{
		ASSERT(_T("(begin > end)"));
		return;
	}

	const unsigned PeakSize = Peak.size();
	unsigned bg = begin;
	unsigned ed = end;
	bool getIndex = false; 
	deque<bool> MinusCheck(PeakSize, false); //�ֶθ�����
	for (unsigned i=begin; i<=end; ++i)
	{
		if (Peak[i].Type < 0)
		{
			MinusCheck[i] = true;
		}
	}
	for (; begin<=end; ++begin)
	{
		bg = begin;

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
		
				if (Peak[ed].Type != mark || Peak[ed].dType != CPeak::VERTDIVIDE)
				{
					ed--;
					break;
				}
				if (ed == PeakSize) //modify if (ed+1 == PeakSize)
				{
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
				if (MinusCheck[bg]) //���Ǹ������
				{
					//if (fabs(_y) > fabs(handleSema[m].y))
					if ((fabs(_y) > fabs(handleSema[m].y) && _y<=0) ||
						(fabs(_y) < fabs(handleSema[m].y) && _y>0 ))
					{
						isfail = true;
						break;
					}
				}
				else
				{
					if ((fabs(_y) > fabs(handleSema[m].y) && _y>0) ||
						(fabs(_y) < fabs(handleSema[m].y) && _y<=0))
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
				if (MinusCheck[ed]) //���Ǹ������
				{
					if ((fabs(_y) > fabs(handleSema[n].y) && _y<=0) ||
						(fabs(_y) < fabs(handleSema[n].y) && _y>0 ))
					{
						isfail = true;
						break;
					}
				}
				else
				{
					if ((fabs(_y) > fabs(handleSema[n].y) && _y>0)||
						(fabs(_y) < fabs(handleSema[n].y) && _y<=0))
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
		begin = ed;
	}
}






//-----------------------------------------------------------------------------
//���ܣ�[��]���߷ַ�����У��
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak		������
//          3.unsigned begin			��ʼ����
//          4.unsigned end				��������
//���������
//			1.vector<CPeak> &Peak ����У����Ĳ�
//-----------------------------------------------------------------------------
VOID CA_PlusTangetBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end)
{
	for (unsigned i=begin; i<=end; ++i)//�Ը����������ű���
	{
		for (unsigned j=end; j>=i; --j)
		{
			if (Peak[i].FinSema.x >= Peak[j].StartSema.x ) //�ҵ�һ�����������зַ�
			{
				CA_PeakValleyBaseLineCheck(handleSema, Peak, i+1, j);
				//�߶�һ��
				double x0 = Peak[i].StartSema.x;
				double y0 = Peak[i].StartSema.y;
				double x1 = Peak[i].FinSema.x;
				double y1 = Peak[i].FinSema.y;
				//�߶ζ���
				double x2 = Peak[j].StartSema.x;
				double y2 = Peak[j].StartSema.y;
				double x3 = Peak[j].FinSema.x;
				double y3 = Peak[j].FinSema.y;
				//�ж������߶��Ƿ��ཻ���������������Ƿ����ص���
				if (TwoLineIsIntersect(x0,y0,x1,y1,x2,y2,x3,y3))
				{
					//::AfxMessageBox(_T("�߶��ཻ"));
					Peak[i].FinSema = Peak[j].FinSema;
				}
				i = j;
				break;
			}
		}
	}
}



//-----------------------------------------------------------------------------
//���ܣ�[��]���߷ַ�����У��
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak		������
//          3.unsigned begin			��ʼ����
//          4.unsigned end				��������
//���������
//			1.vector<CPeak> &Peak ����У����Ĳ�
//-----------------------------------------------------------------------------
VOID CA_MinusTangetBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end)
{
	for (unsigned i=end; i>begin; --i)//�Ը����������ű���
	{
		for (unsigned j=begin; j<=i; ++j)
		{
			if (Peak[j].FinSema.x >= Peak[i].StartSema.x ) //�ҵ�һ�������ķ��зַ�
			{
				CA_PeakValleyBaseLineCheck(handleSema, Peak, j, i-1);
				//�߶�һ��
				double x0 = Peak[i].StartSema.x;
				double y0 = Peak[i].StartSema.y;
				double x1 = Peak[i].FinSema.x;
				double y1 = Peak[i].FinSema.y;

				//�߶ζ���
				double x2 = Peak[j].StartSema.x;
				double y2 = Peak[j].StartSema.y;
				double x3 = Peak[j].FinSema.x;
				double y3 = Peak[j].FinSema.y;
				//�ж������߶��Ƿ��ཻ���������������Ƿ����ص���
				if (TwoLineIsIntersect(x0,y0,x1,y1,x2,y2,x3,y3))
				{
					//::AfxMessageBox(_T("�߶��ཻ"));
					Peak[i].StartSema = Peak[j].StartSema;
				}
				i = j;
				break;
			}
		}
	}
}

//-----------------------------------------------------------------------------
//���ܣ������ȱȲ������Զ��жϷַ����ͣ�����ַ崦����������
//-----------------------------------------------------------------------------

VOID CA_CheckPeakToValley(std::vector<CPeak> &Peak, const double p2vRatio,
	deque<pair<unsigned, unsigned> > &DividePeakId)
{
	if (DividePeakId.empty()) return;
	
	deque<pair<unsigned, unsigned> > tmpDeque; //����һ����ʱ���������������������Ի�
		
	while (!DividePeakId.empty())
	{
		unsigned bgId = DividePeakId[0].first;
		unsigned edId = DividePeakId[0].second;

		//���Ϊ����
		if (bgId >= edId) 
		{
			DividePeakId.pop_front();
			continue;
		}

		// ������
		unsigned PEAKh_Size = edId - bgId + 1;
		unsigned P2VRatio_Size = edId - bgId;
		if (PEAKh_Size > 100000) return; //�����Ǵ���100000������������  

		vector<double> pPEAKh; //��ʱ���
		vector<double> pP2VRatio; //��ʱ��ȱȽ��
		vector<CSema> secPoint; //����������ϵĵ㣬�����������岨������1

		//�������������
		double X1 = Peak[bgId].StartSema.x;
		double Y1 = Peak[bgId].StartSema.y;
		double X2 = Peak[edId].FinSema.x;
		double Y2 = Peak[edId].FinSema.y;
		double K = (Y2-Y1)/(X2-X1);
		double C = Y2 - K * X2;
		for (unsigned i=bgId; i<=edId; ++i)
		{
			double XH = Peak[i].PeakSema.x;
			double YH = Peak[i].PeakSema.y;

			double XR = Peak[i].FinSema.x;
			double YR = Peak[i].FinSema.y;

			double H = K * XH + C;
			double R = K * XR + C;

			double PeakH = fabs(fabs(YH)-fabs(H));
			double P2VRatio = fabs(fabs(YR)-fabs(R));

			
			pPEAKh.push_back(PeakH);
			pP2VRatio.push_back(P2VRatio);

			CSema sema(Peak[i].FinSema);
			sema.y = R;
			secPoint.push_back(sema);
		}

		//bool bSucceed = false; //�Ƿ������������ô�ֱ�ַ�
		for (unsigned i=0; i<pPEAKh.size()-1; i++)
		{
			double H1 = pPEAKh[i];
			double H2 = pPEAKh[i+1];
			double HV = (H1 >= H2) ? H2 : H1;
			
			pP2VRatio[i] = HV / pP2VRatio[i];

			if (pP2VRatio[i] <= p2vRatio) //��ȱȵ����û�ָ��ֵ����ֱ�ַ�
			{
				//bSucceed = true;
				//break;
				Peak[bgId+i].dType = CPeak::VERTDIVIDE;
				Peak[bgId+i+1].dType = CPeak::VERTDIVIDE;
				Peak[bgId+i].FinSema  = secPoint[i];
				Peak[bgId+i+1].StartSema  = secPoint[i];
			}
			else //��ȱȸ����û�ָ��ֵ���������
			{
				Peak[bgId+i].dType = CPeak::PEAKVALLEY;
			}
		}

		DividePeakId.pop_front();
	}

	tmpDeque.swap(DividePeakId);
}



//-----------------------------------------------------------------------------
// ���ܣ���û���У������
// ���������
//			0.CPeak::DivideType divType             �ַ�����
//			1.const std::vector<CPeak> &Peak		������
//          2.pair<unsigned, unsigned> &Arg			��ֱ�ַ����У������
// ���������
//			2.pair<unsigned, unsigned> &Arg	        
//-----------------------------------------------------------------------------
VOID CA_GetBaseLineArgs(CPeak::DivideType divType,
	const std::vector<CPeak> &Peak,
	deque<pair<unsigned, unsigned> > &Arg)
{
	const unsigned ResultSize = Peak.size();
	//�ȱ���һ�飬�ѷ�����������Ŷ�����ͬһ������
	vector<unsigned> pIndex;
	for (unsigned i=0; i<ResultSize; ++i)
	{
		if (Peak[i].dType == divType)
			pIndex.push_back(i);
	}

	const unsigned IdextSize = pIndex.size();
	if (IdextSize==0) return;

	//����˫�˶���deque<unsigned> rIndex
	deque<unsigned> rIndex;

	rIndex.push_front(pIndex[0]);
	for (unsigned i=1; i<IdextSize; ++i)
	{
		if (pIndex[i] - rIndex.back() == 1)
		{
			rIndex.push_back(pIndex[i]);
		}
		else
		{
			pair<unsigned, unsigned> p = std::make_pair(rIndex.front(), rIndex.back());
			Arg.push_back(p);
			rIndex.clear();
			rIndex.push_back(pIndex[i]);
		}
	}
	if (!rIndex.empty())
	{
		pair<unsigned, unsigned> p = std::make_pair(rIndex.front(), rIndex.back());
		Arg.push_back(p);
	}
}


//-----------------------------------------------------------------------------
// ɾ��֮ǰ�����еķ������ֵ����Ա����m_cArrayMPA
//-----------------------------------------------------------------------------
VOID CA_GetAllMPA(std::vector<CPeak> &Peak, CArray<double> &cArrayMPA)
{
	if (cArrayMPA.GetSize()!=0)
		cArrayMPA.RemoveAll();

	const size_t PeakSize = Peak.size();
	for (size_t i=0; i<PeakSize; ++i)
	{
		cArrayMPA.Add(Peak[i].PeakArea);
	}
}



//-----------------------------------------------------------------------------
// ɾ��֮ǰ�����еķ�߸�ֵ����Ա����m_cArrayMH
//-----------------------------------------------------------------------------
VOID CA_GetAllMH(std::vector<CPeak> &Peak, CArray<double> &cArrayMH)
{
	cArrayMH.RemoveAll();

	const size_t PeakSize = Peak.size();
	for (size_t i=0; i<PeakSize; ++i)
	{
		cArrayMH.Add(Peak[i].PeakHeight);
	}
}


//-----------------------------------------------------------------------------
//���ܣ���÷����
//���������
//			1.vector<CSema> &handleSema ��ʼ�ź���
//			2.vector<CPeak> &Peak Ҫ��÷�����Ĳ�		
//���������    
//			1.vector<CPeak> &Peak ��÷������Ĳ�
//-----------------------------------------------------------------------------
VOID CA_GetPeakArea(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak)
{
	const unsigned PeakSize = Peak.size();
	const unsigned SemaSize = handleSema.size();
	//unsigned j = 0;
	for (unsigned i=0; i<PeakSize; ++i)  
	{
		double x2 = Peak[i].FinSema.x;           
		double y2 = Peak[i].FinSema.y;
		double x1 = Peak[i].StartSema.x;
		double y1 = Peak[i].StartSema.y;
		double k = (y2-y1)/(x2-x1);
		double c = y1-k*x1;

		double area = 0.0;
		for (unsigned j = 0; j<SemaSize; ++j)
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
					double front = fabs(fabs(y3)-fabs(x3*k+c));
					double behind = fabs(fabs(y4)-fabs(x4*k+c));
					area += (front+behind)*h/2;
					j++;
				} while (handleSema[j].x-Peak[i].FinSema.x <= 0.000000); //�ҵ��յ�
				Peak[i].PeakArea = area;
				break;
			}
		}
	}
	//�������ÿ���������������������߷ַ�����Ҫ��ȥ�ظ��������һ����
	deque<pair<unsigned, unsigned> > PlusTangetArgs;  //���зַ�
	deque<pair<unsigned, unsigned> > MinusTangetArgs; //���зַ�
	CA_GetBaseLineArgs(CPeak::PLUSTANGET, Peak, PlusTangetArgs);
	while (!PlusTangetArgs.empty())
	{
		deque<pair<unsigned, unsigned> > dequBuff;
		unsigned begin = PlusTangetArgs[0].first;
		unsigned end =  PlusTangetArgs[0].second;
		for (unsigned i = begin; i<=end; ++i)
		{
			if (Peak[i].FinSema.x < Peak[i+1].StartSema.x)
			{
				std::pair<unsigned, unsigned> p = std::make_pair(begin, i);
				dequBuff.push_back(p);
				begin = i+1;
			}
			if (i==end-1)
			{
				std::pair<unsigned, unsigned> p = std::make_pair(begin, end);
				dequBuff.push_back(p);
				break;
			}
		}
		while (!dequBuff.empty())
		{
			unsigned mainPeakNo = dequBuff[0].first; //������
			unsigned subFirstPeakNo = dequBuff[0].first+1;   //���ĵ�һ�����
			unsigned subLastPeakNo = dequBuff[0].second; //�������һ�����
			for (unsigned i=subFirstPeakNo; i<=subLastPeakNo; ++i)
			{
				if (Peak[mainPeakNo].PeakArea < Peak[i].PeakArea)
				{
					ASSERT(_T("CPeak::PLUSTANGET (Peak[mainPeakNo].PeakArea < Peak[i].PeakArea)"));
				}
				Peak[mainPeakNo].PeakArea -= Peak[i].PeakArea;
			}
			dequBuff.pop_front();
		}
		PlusTangetArgs.pop_front();
	}

	CA_GetBaseLineArgs(CPeak::MINUSTANGET, Peak, MinusTangetArgs);
	while (!MinusTangetArgs.empty())
	{
		deque<pair<unsigned, unsigned> > dequBuff;
		unsigned begin = MinusTangetArgs[0].second;
		unsigned end =  MinusTangetArgs[0].first;
		for (unsigned i = begin; i>=end; --i)
		{
			if (Peak[i].StartSema.x > Peak[i-1].FinSema.x)
			{
				std::pair<unsigned, unsigned> p = std::make_pair(i, begin);
				dequBuff.push_back(p);
				begin = i-1;
			}
			if (i==end+1)
			{
				std::pair<unsigned, unsigned> p = std::make_pair(end, begin);
				dequBuff.push_back(p);
				break;
			}
		}
		while (!dequBuff.empty())
		{
			unsigned mainPeakNo = dequBuff[0].second; //������
			unsigned subFirstPeakNo = dequBuff[0].second-1;   //���ĵ�һ�����
			unsigned subLastPeakNo = dequBuff[0].first;		//�������һ�����
			for (unsigned i=subFirstPeakNo; i>=subLastPeakNo; --i)
			{
				if (Peak[mainPeakNo].PeakArea < Peak[i].PeakArea)
				{
					ASSERT(_T("CPeak::MINUSTANGET (Peak[mainPeakNo].PeakArea < Peak[i].PeakArea)"));
				}
				Peak[mainPeakNo].PeakArea -= Peak[i].PeakArea;
			}
			dequBuff.pop_front();
		}
		MinusTangetArgs.pop_front();
	}
}

VOID CA_EraseByPeakArea(std::vector<CPeak> &Peak,
	vector<double> &PeakArea)
{//��ɾ���壬δ�Է������������
	vector<CPeak> buffPeak;
	const unsigned PeakSize = Peak.size();
	const unsigned PeakAreaSize = PeakArea.size();
	if (PeakSize!=PeakAreaSize)
	{
		TRACE(_T("(PeakSize!=PeakAreaSize)"));
		ASSERT(TRUE);
	}
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (fabs(Peak[i].PeakArea) > PeakArea[i])
		{
			buffPeak.push_back(Peak[i]);
		}
	}
	Peak.swap(buffPeak);
}

VOID CA_EraseByPeakHeight(std::vector<CPeak> &Peak,
	vector<double> &PeakHeight)
{
	vector<CPeak> buffPeak;
	const unsigned PeakSize = Peak.size();
	const unsigned PeakHeightSize = PeakHeight.size();
	if (PeakSize!=PeakHeightSize) ASSERT(TRUE);
	
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (fabs(Peak[i].PeakHeight) > PeakHeight[i])
			buffPeak.push_back(Peak[i]);
	}
	Peak.swap(buffPeak);
}







//-----------------------------------------------------------------------------
//���ܣ����ķ����% �� ���%
//���������
//			1.std::vector<CPeak> &Peak		 ����Ϣ
//			
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_GetPeakPercent(std::vector<CPeak> &Peak)
{
	// ���ķ����% �� ���%
	const unsigned PeakSize = Peak.size();

	double totPeakArea=0.0, totPeakHeigh=0.0;
	for (unsigned i=0; i<PeakSize; ++i)
	{
		totPeakArea += Peak[i].PeakArea;
		totPeakHeigh += Peak[i].PeakHeight;
	}
	if (totPeakArea==0.0 || totPeakHeigh==0.0)
	{
		ASSERT(totPeakArea==0.0 || totPeakHeigh==0.0);
	}
	for (unsigned i=0; i<PeakSize; ++i)
	{
		Peak[i].PeakAreaPercent = Peak[i].PeakArea / totPeakArea * 100.0f;
		Peak[i].PeakHighPercent = Peak[i].PeakHeight / totPeakHeigh * 100.0f;
	}
}






//-----------------------------------------------------------------------------
//���ܣ�����������������ߣ��������ӣ���β���ӵȣ�
//���������
//			1.std::vector<CSema> &handleSema Ԫ�����ź���
//			2.std::vector<CPeak> &Peak		 ����Ϣ
//			3.
//���������
//			1.std::vector<CPeak> &Peak ����Ϣ���޸�
//-----------------------------------------------------------------------------
VOID CA_GetOtherPeakArgs(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak)
{
	const unsigned PeakSize = Peak.size();
	double factor = 1.0; //��������
	for (unsigned i=0; i<PeakSize; ++i)
	{
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

		//double PeakWidthLeft =  sqrt((ty/factor-y1/factor)*(ty/factor-y1/factor)+(px-x1)*(px-x1));  
		//double PeakWidthRight = sqrt((y2/factor-ty/factor)*(y2/factor-ty/factor)+(x2-px)*(x2-px));
		double PeakWidthLeft  = fabs(px-x1);
		double PeakWidthRight = fabs(x2-px);
		double lr = PeakWidthLeft/PeakWidthRight;
		Peak[i].Tailing = lr; //��β����

		double hx = px;
		double hy = (py-ty)/2 + ty;
		double _c = hy-hx*tana;		

		double diff_y = INT_MAX;
		unsigned idxl, idxr;

		for (unsigned l=Peak[i].StartSema.idx; l<=Peak[i].PeakSema.idx; ++l) //���У��ƽ��е�
		{
			double ly = handleSema[l].x * tana + _c;
			if (diff_y >= fabs(handleSema[l].y - ly))
			{
				diff_y = fabs(handleSema[l].y - ly);
				idxl = l;
			}
		}
		CSema tmpFront(handleSema[idxl]); //������
		Peak[i].MidStartSema = tmpFront;

		diff_y = INT_MAX;

		for (unsigned r=Peak[i].PeakSema.idx; r<=Peak[i].FinSema.idx; ++r) //���У��ƽ��е�
		{
			double ry = handleSema[r].x * tana + _c;

			if (diff_y >= fabs(handleSema[r].y - ry))
			{
				diff_y = fabs(handleSema[r].y - ry);
				idxr = r;
			}
		}
		CSema tmpLast(handleSema[idxr]);   //������
		Peak[i].MidFinSema = tmpLast;

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
//���ܣ����ֲ��������Ӱ����У��
//���������
//			1.vector<CPeak> &Peak ����鲨���͵Ĳ�
//���������
//			1.vector<CPeak> &Peak �����͵ļ���Ĳ�
//-----------------------------------------------------------------------------
VOID CA_PeakTypeChange(std::vector<CPeak> &Peak)
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
			if (Peak[i].FinSema.idx == Peak[i+1].StartSema.idx)
			{
				continue;
			}
			else /*if (Peak[i].FinSema!=Peak[i+1].StartSema)*/
			{
				if (i!=0)
				{
					if (Peak[i].StartSema.idx == Peak[i-1].FinSema.idx)
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







//-----------------------------------------------------------------------------
//���ܣ��ж������߶��Ƿ��ཻ
//���������
//			1.double x0, double y0, double x1, double y1 �߶�һ����������
//			2.double x2, double y2,	double x3, double y3 �߶ζ�����������
//���������
//			�жϽ��
//-----------------------------------------------------------------------------
bool TwoLineIsIntersect(double x0, double y0, 
						double x1, double y1, 
						double x2, double y2, 
						double x3, double y3)
{ //�����߶��Ƿ��ཻX0X1 AND X1X2
	double x, y;
	double Minx01 = min(x0, x1);
	double Miny01 = min(y0, y1);
	double Minx23 = min(x2, x3);
	double Miny23 = min(y2, y3);
	double Maxx01 = max(x0, x1);
	double Maxy01 = max(y0, y1);
	double Maxx23 = max(x2, x3);
	double Maxy23 = max(y2, y3);

	if(x1!=x0 && x2!=x3)
	{
		double k1 = (y1-y0)/(x1-x0);
		double k2 = (y3-y2)/(x3-x2);
		double Den = (y1-y0)*(x3-x2) - (y3-y2)*(x1-x0);
		if(k1==k2)
		{ //ƽ�в��ཻ
			double d1 = abs(y0*(x1-x0)-x0*(y1-y0)-y2*(x3-x2)+x2*(y3-y2)); //���빫ʽd = abs(c1-c2) / sqrt(a*a+b*b)
			if(d1==0)
			{//ֱ���غ�
				if((x2>Minx01 && x2<Maxy01 && y2>Miny01 && y2<Maxy01) || (x3>Minx01 && x3<Maxy01 && y3>Miny01 && y3<Maxy01)
					|| (x0>Minx23 && x0<Maxy23 && y0>Miny23 && y0<Maxy23) || (x1>Minx23 && x1<Maxy23 && y1>Miny23 && y1<Maxy23))
				{  //ʵ����ײ�����߶��غ���Ϊ�ཻ��
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}   
		}
		x = ((y2-y0)*(x1-x0)*(x3-x2)+(y1-y0)*(x3-x2)*x0-(y3-y2)*(x1-x0)*x2)/Den;
		y = ((y1-y0)*(x-x0))/(x1-x0) + y0;

		if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 && Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
		{
			//InterX = x;
			//InterY = y;
			return true;
		}
	}
	else if(x1==x0 && x2!=x3)
	{
		x = x0;
		y = ((y3-y2)*(x0-x2))/(x3-x2) + y2;
		if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 && Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
		{
			//InterX = x;
			//InterY = y;
			return true;
		}
	}
	else if(x1!=x0 && x2==x3)
	{
		x = x2;
		y = ((y1-y0)*(x2-x0))/(x1-x0) + y0;
		if(Minx01<=x && x<=Maxx01 && Miny01<=y && y<=Maxy01 && Minx23<=x && x<=Maxx23 && Miny23<=y && y<=Maxy23)
		{
			//InterX = x;
			//InterY = y;
			return true;
		}       
	}
	return false;
}






//-----------------------------------------------------------------------------
//���ܣ��жϷ����ͣ�Ϊ��ƥ��CPeakStandard���е�CPeakType
//-----------------------------------------------------------------------------
VOID CA_SetPeakType(std::vector<CPeak> &Peak)
{
	const size_t PeakSize = Peak.size();
	for (size_t i=0; i<PeakSize; ++i)
	{
		//unsigned short negative:				1;	//����
		//unsigned short single :				1;	//����
		//unsigned short overlap_pl:			1;	//�ص���
		//unsigned short overlap_ls:			1;	//�ص���(���)
		//unsigned short overlap_rs:			1;	//�ص���(�Ҽ�)
		//unsigned short overlap_vs:			1;	//�ص���(��ֱ�ַ�)
		//unsigned short tail:					1;	//��β

		//  PEAKVALLEY, //��Ȼ��ߣ���ʼ����
		//	VERTDIVIDE, //��ֱ�ַ�
		//	PLUSTANGET, //���зַ�
		//	MINUSTANGET //���зַ�

		//int    Type;          //������,1=����; -1=����; >1�ص�����; <-1�ص�����
		if (Peak[i].Type < 0) Peak[i].PeakType.negative = 1;
		if (abs(Peak[i].Type) == 1) Peak[i].PeakType.single = 1;
		if (Peak[i].dType == CPeak::PEAKVALLEY)   Peak[i].PeakType.overlap_pl = 1; 
		if (Peak[i].dType == CPeak::MINUSTANGET)  Peak[i].PeakType.overlap_ls = 1; 
		if (Peak[i].dType == CPeak::PLUSTANGET)   Peak[i].PeakType.overlap_rs = 1; 
		if (Peak[i].dType == CPeak::VERTDIVIDE)   Peak[i].PeakType.overlap_vs = 1; 
		if (Peak[i].Tailing > TAILING) Peak[i].PeakType.tail = 1;
	}
	//���������߷ַ�ķǼ�������������
	for (size_t i=0; i<PeakSize; ++i)
	{
		if (Peak[i].dType == CPeak::PLUSTANGET)//���ǵ�һ����Ϊ������
		{
			unsigned bg = i;
			double endx = Peak[i].FinSema.x;
			for ( ; i<PeakSize-1; ++i)
			{
				if (endx < Peak[i+1].StartSema.x)
				{
					break;
					i--;
				}
			}
			Peak[bg].PeakType.overlap_pl = 1; 
		} 
	}

	//��//��������ַ�ķǼ�������������
	for (int i=PeakSize-1; i>=0; --i)
	{
		if (Peak[i].dType == CPeak::MINUSTANGET)
		{
			unsigned bg = i;
			double startx = Peak[i].StartSema.x;
			for ( ; i>0; --i)
			{
				if (startx > Peak[i-1].StartSema.x)
				{
					break;
					i++;
				}
			}
			Peak[bg].PeakType.overlap_pl = 1; 
		} 
	}
}






//-----------------------------------------------------------------------------
//���ܣ�add ��ô�ֱ�ַ��ϵĴ��ߣ�����Ԫ�������ݺͽ�����ݣ����ش�ֱ�ַ����ߵĽ������
//-----------------------------------------------------------------------------
VOID CA_GetValleyLine(const CCurveData &curveData, const CPeakList &peakList,
	CArrayPALine &PALineResult)
{
	const int peakSize = peakList.m_cArrayData.GetSize();  //��������
	const int curveSize = curveData.m_arrayData.GetSize(); //Ԫ�������ݸ���

	//�ж�Ԫ���������Ƿ�����
	for (int i=1; i< curveSize; ++i) 
	{
		if (curveData.m_arrayData[i].dX - curveData.m_arrayData[i-1].dX < 0.000000)
		{
			return; //Ԫ���������򷵻�
		}
	}

	static int j = 0; //����Ԫ�������±�

	//ֻ�Է��������ж�
	for (int i=0; i<peakSize; ++i)
	{
		double compareX = peakList.m_cArrayData[i].m_dataEnd.dX;
		for ( ; j< curveSize; ++j) //����Ԫ���ߣ��Ա�ÿһ��x����
		{
			if (curveData.m_arrayData[j].dX - compareX == 0.0000) //�ҵ���ԭ�����ϵĵ�
			{ //��ʼ�Ա�Y��������
				double compareY = peakList.m_cArrayData[i].m_dataEnd.dY;
				if (curveData.m_arrayData[j].dY - compareY != 0.0000) //�ҵ���ֱ�ַ��ϴ���
				{
					CCurveDataSingle oneSingle(curveData.m_arrayData[j].dX, curveData.m_arrayData[j].dY); 
					CCurveDataSingle twoSingle(compareX, compareY);
					CPALine PALine;
					PALine.m_point[0] = oneSingle;
					PALine.m_point[1] = twoSingle;
					PALineResult.Add(PALine);
				}
			}
		}
	}
}
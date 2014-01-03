#include "StdAfx.h"
#include "PeakSeak.h"
#include "WaveletTransform.h"
#include "RealTime.h"
#include <fstream>



CRealTime::CRealTime(void)
{
	m_InputNums = 0;
	m_prevprocess_peak_nums = 0;
	m_id = 0;
	m_isRealTimeParametersSet = false;

	counter=0;

	m_pDrawCurve = NULL;
	m_pLinePeakSeek = NULL;
	m_pPoint = NULL; 
	m_pPeak  = NULL; 
	m_BaseLineColor=RGB(210, 0, 0);
}

CRealTime::CRealTime(CSuperChartCtrl* p)
{
	m_BaseLineColor=RGB(210, 0, 0);

	if (p!=NULL)
	{
		m_pCtrl=p;

//		m_pLinePeakSeek = m_pCtrl->RealDraw(GROUPID_FIRST);
		m_pPoint = m_pCtrl->CreatePointsSerie(); //��������յ���
		m_pPeak  = m_pCtrl->CreatePointsSerie(); //����������
//		m_pPoint->SetGroupID(GROUPID_SECOND);
//		m_pPeak->SetGroupID(GROUPID_SECOND);


	}

}
CRealTime::~CRealTime(void)
{

}


VOID CRealTime::GetPeakResult(std::vector<CPeak> &Peak)
{
	Peak = m_Peak;
}
VOID CRealTime::SetSemaDate(CString filePathName)
{
	CSheet csvSheet;  
	std::ifstream file(filePathName);  //��csv�ļ�  
	csvSheet.loadFrom(file);  //������CSheet  //MessageBox(_T("��ȡ��1��2������"));

	//��ȡʱ������
	const int dataSize = csvSheet.getRowCount();
	double *pX = new double[dataSize];
	double *pY = new double[dataSize];

	std::vector<double> time, val; //�ݴ��ʱ����ź���
	for (int i=0; i<csvSheet.getRowCount(); ++i)  
	{  
		CRow row = csvSheet.getRow(i);  //��ȡָ����  
		CString s = row.getColumn(0);	//��ȡָ����  
		double t = wcstod(s, NULL);
		time.push_back(t);
		pX[i] = t; 
	}  

	for (int i=0; i<csvSheet.getRowCount(); ++i)  
	{  
		CRow row = csvSheet.getRow(i);  //��ȡָ����
		CString s = row.getColumn(1); //��ȡָ����
		double v = wcstod(s, NULL);
		val.push_back(v);
		pY[i] = v;
	}  


	if (m_OrigSema.size()!=0) //�����ݷ���������
	{
		m_OrigSema.clear();
	}
	if (time.size()==val.size())
	{
		dXMax = time[0];
		dXMin = time[0];
		dYMax = val[0];
		dYMin = val[0];
		for (size_t i=0; i<time.size(); i++)
		{
			CSema OneSema(i, time[i], val[i]);
			if (time[i] > dXMax) dXMax = time[i];
			if (time[i] < dXMin) dXMin = time[i];	
			if (val[i] > dYMax) dYMax = val[i];
			if (val[i] < dYMin) dYMin = val[i];
			m_OrigSema.push_back(OneSema);
		}
	}
	else
	{
		//MessageBox(_T("��ȡcsv�������󣬲ⶨ����������һ��"));
		delete []pX;
		delete []pY;
		return;
	}

	delete []pX;
	delete []pY;

	file.close();	//�ر�CSV�ļ�

}
VOID CRealTime::SetSemaDate(double x[], double y[], int n)
{
	dXMax = x[0];
	dXMin = x[0];
	dYMax = y[0];
	dYMin = y[0];
	for (int i=0; i<n; ++i)
	{
		CSema OneSema(i, x[i], y[i]);
		if (x[i] > dXMax) dXMax = x[i];
		if (x[i] < dXMin) dXMin = x[i];	
		if (y[i] > dYMax) dYMax = y[i];
		if (y[i] < dYMin) dYMin = y[i];
		m_OrigSema.push_back(OneSema);
	}
}
VOID CRealTime::SetChartCtrl(CSuperChartCtrl* p)
{
	if ((p!=NULL) && (m_pCtrl != p))
	{
		m_pCtrl = p;

		if (m_pDrawCurve == NULL) 
			m_pDrawCurve = m_pCtrl->CreateLineSerie(false); //  RealDraw(GROUPID_FIRST);
		if (m_pLinePeakSeek == NULL) 
			m_pLinePeakSeek = m_pCtrl->CreateLineSerie(false); //RealDraw(GROUPID_FIRST);
		if (m_pPoint == NULL) 
		{
			m_pPoint = m_pCtrl->CreatePointsSerie(); //��������յ���
//			m_pPoint->SetGroupID(GROUPID_SECOND);
		}
		if (m_pPeak  == NULL) 
		{
			m_pPeak  = m_pCtrl->CreatePointsSerie(); //����������
//			m_pPeak->SetGroupID(GROUPID_SECOND);
		}
	}
}
VOID CRealTime::SetParameter(bool IsDividePeak, bool HasNagetiveSpike, double Threshold, 
	double HalfPeakWidth, double PeakArea, int Series=3, int Avgs=5)
{
	m_IsDividePeak=IsDividePeak; //�Ƿ�����ֱ�ַ�
	m_HasNagetiveSpike=HasNagetiveSpike; //�Ƿ��и������

	m_Threshold=Threshold; //����б��
	m_HalfPeakWidth=HalfPeakWidth; //��С����
	m_PeakArea=PeakArea; //��С�����

	m_Series=Series; //�ڲ��������жϡ�Ĭ��3
	m_Avgs=Avgs; //�ڲ�ƽ������.Ĭ��5
}

VOID CRealTime::ClearLine()
{
	if (counter==0) 
	{
		counter++;
		return;
	}
	//m_pCtrl->EnableRefresh(false);

	//SetShowHide(true, true);
	//m_pCtrl->EnableRefresh(true);
	//SignalDisplay();
	//m_pCtrl->RefreshCtrl();	
	//::AfxMessageBox( _T("---"));
	m_pCtrl->EnableRefresh(false);


	m_pDrawCurve->ClearSerie();
	m_pLinePeakSeek->ClearSerie();          //����ϴ�����
	m_pPoint->ClearSerie();					//����ϴθ�����յ�
	m_pPeak->ClearSerie();					//����ϴθ����岨�ȵ�
	//m_pDrawCurve = NULL;
	//m_pLinePeakSeek = NULL;
	//m_pPoint = NULL; 
	//m_pPeak  = NULL; 

	if (m_phemline.size()!=0) //�������
	{
		for (unsigned i=0; i<m_phemline.size(); i++)
			m_phemline[i]->ClearSerie();  
		m_phemline.clear();
	}

	if (m_pDividePeakBaseLine.size()!=0) //�����ֱ�ַ������
	{
		typedef std::list<CChartLineSerie*>::iterator LI;
		for (LI iter=m_pDividePeakBaseLine.begin(); iter!=m_pDividePeakBaseLine.end(); iter++)
			(*iter)->ClearSerie();
		m_pDividePeakBaseLine.clear();
	}

	if (m_pTagline.size()!=0)//���֮ǰ�ı����
	{
		typedef std::list<CChartLineSerie*>::iterator LI;
		for (LI iter=m_pTagline.begin(); iter!=m_pTagline.end(); iter++)
			(*iter)->ClearSerie();
		m_pTagline.clear();
	}

	m_pCtrl->RemoveAllChartStrings();  //����ϴ��ַ���
	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	
}

VOID CRealTime::ClearSemaDate()
{
	m_OrigSema.clear();
	m_Peak.clear();
}

VOID CRealTime::DrawBaseLine()
{
	if (m_Peak.size()==0)
	{
		return;
	}
	bool isdraw = true;
	const unsigned PeakSize = m_Peak.size();
	if ((!m_IsDividePeak) && (PeakSize!=0)) //�Ǵ�ֱ�ַ�
	{
		for (unsigned i=0; i<PeakSize; ++i)
		{//������ֱ��ָ��
			CChartLineSerie* line;  
			line = m_pCtrl->CreateLineSerie(); 
			line->SetColor(m_BaseLineColor/*RGB(210, 0, 0)*/); //���û�����ɫ
			m_phemline.push_back(line);
		}
		if (m_phemline.size()==0)
		{
			isdraw = false;
		}
		else
		{
			const unsigned m_phemlineSize = m_phemline.size();
			for (unsigned i=0; i<PeakSize; ++i)
			{//����ֱ��
//				m_phemline[i]->SetGroupID(GROUPID_THREE);		
				m_phemline[i]->m_vPoints.InitPoints(4);
				double dx[] = { m_Peak[i].StartSema.x, m_Peak[i].FinSema.x };
				double dy[] = { m_Peak[i].StartSema.y, m_Peak[i].FinSema.y };
				m_phemline[i]->m_vPoints.AddPoints(dx, dy, 2);   //������
			}
		}
	}

	else if (PeakSize!=0 && isdraw)
	{
		unsigned nodeIndex = 0;  //����ֱ�ַ��ߵ�����
		unsigned bg = 0;
		unsigned ed = bg;
		bool isSignalWave = false;
		for (; bg<PeakSize; ++bg)
		{
			if (m_Peak[bg].Type==1 || m_Peak[bg].Type==-1)
			{
				ed = bg;
				isSignalWave = true;
			}
			else /*(Peak[bg].Type>1)*/ //�ص�����
			{
				ed = bg;
				int mark = m_Peak[bg].Type;
				while (ed<PeakSize && m_Peak[ed].Type==mark)
				{
					if (ed+1==PeakSize && m_Peak[PeakSize-1].Type==mark)
					{
						break;
					}
					ed++;
					if (m_Peak[ed].Type!=mark)
					{
						ed--;
						break;
					}
				}
			}
			if (isSignalWave && nodeIndex!=PeakSize) //������ַ���
			{
				CChartLineSerie* line;  
				line = m_pCtrl->CreateLineSerie();
				double dx[] = { m_Peak[nodeIndex].StartSema.x, m_Peak[nodeIndex].FinSema.x };
				double dy[] = { m_Peak[nodeIndex].StartSema.y, m_Peak[nodeIndex].FinSema.y };
				nodeIndex++;
				line->SetColor(m_BaseLineColor/*RGB(210, 0, 0)*/); //���û�����ɫ
//				line->SetGroupID(GROUPID_THREE);
				line->m_vPoints.InitPoints(4);
				line->m_vPoints.AddPoints(dx, dy, 2);   
				isSignalWave = false;
				m_phemline.push_back(line);
			}
			else if (!isSignalWave && nodeIndex!=PeakSize) //��������ַ���
			{
				for (; bg!=ed; ++bg)
				{
					CChartLineSerie* line1/*, *line2, *line3*/;  
					line1 = m_pCtrl->CreateLineSerie(); 
					double dx[] = { m_Peak[nodeIndex].StartSema.x, m_Peak[nodeIndex].FinSema.x };
					double dy[] = { m_Peak[nodeIndex].StartSema.y, m_Peak[nodeIndex].FinSema.y };
					nodeIndex++;
					line1->SetColor(m_BaseLineColor/*RGB(210, 0, 0)*/); //���û�����ɫ
//					line1->SetGroupID(GROUPID_THREE);
					line1->m_vPoints.InitPoints(4);
					line1->m_vPoints.AddPoints(dx, dy, 2);
					m_phemline.push_back(line1);

					if (bg+1==ed)
					{
						double dxx[] = { m_Peak[nodeIndex].StartSema.x, m_Peak[nodeIndex].FinSema.x };
						double dyy[] = { m_Peak[nodeIndex].StartSema.y, m_Peak[nodeIndex].FinSema.y };
						nodeIndex++;
						line1->m_vPoints.InitPoints(4);
						line1->m_vPoints.AddPoints(dxx, dyy, 2);
						m_phemline.push_back(line1);
					}
				}
			}
		}
	}
	m_pCtrl->EnableRefresh(true);

}
VOID CRealTime::DrawDividePeakBaseLine()
{
	if (m_Peak.size()==0)
	{
		return;
	}
	if (!m_IsDividePeak)
	{
		return;
	}

	m_pCtrl->EnableRefresh(false);

	const unsigned PeakSize = m_Peak.size();
	for (unsigned i=0; i<PeakSize-1; ++i)
	{
		if (abs(m_Peak[i].Type)!=1)
		{
			int PeakType = m_Peak[i].Type;
			while (m_Peak[i].Type == PeakType && m_Peak[i+1].Type == PeakType )
			{
				CChartLineSerie* line;  
				line = m_pCtrl->CreateLineSerie();//����һ�����ߵ�������
				line->SetColor(m_BaseLineColor/*RGB(210, 0, 0)*/); //���û�����ɫ
//				line->SetGroupID(GROUPID_THREE);
				line->m_vPoints.InitPoints(4);
				double dx[] = { m_Peak[i].FinSema.x, m_Peak[i].FinSema.x };
				double dy[] = { m_OrigSema[m_Peak[i].FinSema.idx].y, m_Peak[i].FinSema.y };
				line->m_vPoints.AddPoints(dx, dy, 2);
				m_pDividePeakBaseLine.push_back(line);
				i++;
			}
		}
	}
	m_pCtrl->EnableRefresh(true);
}
 
VOID CRealTime::DrawResultLine()
{
	m_pCtrl->EnableRefresh(false);
	m_pLinePeakSeek->SetColor(RGB(200, 200, 200)); //�������ߵ���ɫ(�׻�)
	CChartAxis* pAxisSelect;

	m_pCtrl->SetBackColor(RGB(60, 60, 60)); //���ñ�����ɫ
	pAxisSelect = m_pCtrl->GetLeftAxis(); //����Y����
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(dYMin*1.0, dYMax*1.1, COORDINATE_SET);
	pAxisSelect = m_pCtrl->GetBottomAxis(); //����X����
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(dXMin, dXMax, COORDINATE_SET);
	m_pPoint->SetColor(RGB(255, 70, 70)); //���ñ����������ɫ(��ɫ)
	m_pPeak->SetPointType(CChartPointsSerie::ptTriangle); //���ò���������Ǳ��
	m_pPeak->SetColor (RGB(0, 138, 138)); //���ñ겨�岨�ȵ����ɫ(��ɫ)
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetVisible(true);

	m_pLinePeakSeek->m_vPoints.InitPoints(m_OrigSema.size());
	for (size_t i=0; i<m_OrigSema.size();++i)
	{//������
		m_pLinePeakSeek->m_vPoints.AddPoint(m_OrigSema[i].x, m_OrigSema[i].y);
	}

	if (m_Peak.size()==0)
	{
		m_pCtrl->EnableRefresh(true);
		m_pCtrl->RefreshCtrl();	
		return;
	}

	m_pPoint->m_vPoints.InitPoints(m_Peak.size());
	m_pPeak->m_vPoints.InitPoints(m_Peak.size());
	for (unsigned i=0; i<m_Peak.size(); ++i)
	{//���������
		double x[]={m_Peak[i].StartSema.x, m_Peak[i].FinSema.x};
		double y[]={m_Peak[i].StartSema.y, m_Peak[i].FinSema.y};
		m_pPoint->m_vPoints.AddPoints(x, y, 2);
		m_pPeak->m_vPoints.AddPoint(m_Peak[i].PeakSema.x, m_Peak[i].PeakSema.y);//���岨��
	}

	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	

}
VOID CRealTime::SignalProc()
{
	m_Peak.clear();
	if (m_OrigSema.size()==0) return;

	CWaveletTransform wt;
	double avgDiff = wt.Reshape_v2(m_OrigSema);   //�ź����εȼ��ʹÿ������֮���ʱ����X��
	if (avgDiff==0) return;

	std::vector<CSema> AvgData;
	//step1: ����avgΪ1���ƽ�����ߣ�ʣ���������Ϊ1��
	if ((m_OrigSema.size()/m_Avgs)<9)
	{
		::AfxMessageBox( _T("�ڲ�ƽ������࣬��Ҫ���µ���"));
		return;
	}
	new_Averaging(m_OrigSema, AvgData, m_Avgs); 
	const unsigned AvgDataSize = AvgData.size();

	//step2: ���ƽ��������ÿһ���б��
	std::vector<double> FirstOrder;					      //б������
	double maxSlope = 0.0;								  //���������б��
	new_GetAvgDataSlope(AvgData, FirstOrder, maxSlope);

	// step3: ������������˳��Ѱ��
	test_GetpeakSearch_Series(AvgData, FirstOrder, m_Peak, m_Threshold, m_HasNagetiveSpike, m_Series);

	// step4: �̶��㻹ԭ��ԭ����
	new_RestorePeakData(m_OrigSema, m_Peak, m_Avgs, m_Threshold);  //��ԭ��ԭ����

	//-----------------------------------------------------
	// �����㷨���ڷַ�ǰ���˳�����������2���
	test_ErasePeakByNextto(m_Peak);
	new_PeakTypeChange(m_Peak); //�����ͷ����ı�
	//-----------------------------------------------------

	// step5: ��ֱ�ַ壬����У��
	if (m_IsDividePeak)
	{
		new_BaselineAdjust(m_OrigSema, m_Peak, m_HasNagetiveSpike);    //����У��
		test_DividePeak(m_OrigSema, m_Peak);  //new_DividePeak(m_Peak);//��ֱ�ַ�
		new_BaselineAdjust(m_OrigSema, m_Peak, m_HasNagetiveSpike);    //����У��
	}
	else
	{
		new_BaselineCheak(m_OrigSema, m_Peak, m_HasNagetiveSpike);  //����У��
	}
	new_PeakTypeChange(m_Peak); //�����ͷ����ı�
	new_GetPeakWidth_v2(m_OrigSema, m_Peak, m_IsDividePeak); 
	new_GetPeakArea(m_OrigSema, m_Peak);

	//step6: �������ȼ��ٲ���
	new_ErasePeakByHalfWidth(m_Peak, m_HalfPeakWidth);  //���ݰ�������ɾ����
	new_ErasePeakByPeakArea(m_Peak, m_PeakArea); //���ݷ��������ɾ����
	new_GetTagPoint(m_Peak, m_TagPoint);
}
VOID CRealTime::SignalDisplay()
{
	DrawBaseLine();
	DrawDividePeakBaseLine();
	DrawResultLine();
}
VOID CRealTime::DrawCurve()
{
	m_pCtrl->EnableRefresh(false);

	m_pDrawCurve->SetColor(RGB(200, 200, 200)); //�������ߵ���ɫ(�׻�)
	CChartAxis* pAxisSelect;
	m_pCtrl->SetBackColor(RGB(60, 60, 60)); //���ñ�����ɫ
	pAxisSelect = m_pCtrl->GetLeftAxis(); //����Y����
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(dYMin*1.0, dYMax*1.1, COORDINATE_SET);
	pAxisSelect = m_pCtrl->GetBottomAxis(); //����X����
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(dXMin, dXMax, COORDINATE_SET);
	pAxisSelect->SetVisible(true);

	m_pDrawCurve->m_vPoints.InitPoints(m_OrigSema.size());
	for (size_t i=0; i<m_OrigSema.size(); ++i)
	{//������
		m_pDrawCurve->m_vPoints.AddPoint(m_OrigSema[i].x, m_OrigSema[i].y);
	}

	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	

}

// !-----------------------------------------------------------------
// !
// ! 2010.11.04 ���ּܳ�������޸ĺ�ӿڵĹ���
// !
// !-----------------------------------------------------------------

VOID CRealTime::ClearBaseLine() //������л���
{
	if (counter==0) 
	{
		counter++;
		return;
	}
	m_pCtrl->EnableRefresh(false);
	if (m_phemline.size()!=0) //�������
	{
		for (unsigned i=0; i<m_phemline.size(); i++)
			m_phemline[i]->ClearSerie();  
		m_phemline.clear();
	}

	if (m_pDividePeakBaseLine.size()!=0) //�����ֱ�ַ������
	{
		typedef std::list<CChartLineSerie*>::iterator LI;
		for (LI iter=m_pDividePeakBaseLine.begin(); iter!=m_pDividePeakBaseLine.end(); iter++)
			(*iter)->ClearSerie();
		m_pDividePeakBaseLine.clear();
	}
	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	
}
VOID CRealTime::ClearStartFinishPoints() // �����㣬��㣬�����
{
	if (counter==0) 
	{
		counter++;
		return;
	}
	m_pCtrl->EnableRefresh(false);
	m_pPoint->ClearSerie();					//����ϴθ�����յ�
	m_pPeak->ClearSerie();					//����ϴθ����岨�ȵ�
	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	
}
VOID CRealTime::ClearOrigCurve() // ���ԭ����
{
	if (counter==0) 
	{
		counter++;
		return;
	}
	m_pCtrl->EnableRefresh(false);
	m_pDrawCurve->ClearSerie();
	m_pLinePeakSeek->ClearSerie();          //����ϴ�����
	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	
}
VOID CRealTime::SetBaseLineColor(COLORREF cr)
{
	m_pCtrl->EnableRefresh(false);
	if (!m_phemline.empty())
	{
		if (m_phemline[0]->GetColor()!=cr)
		{
			for (unsigned i=0; i<m_phemline.size(); i++)
				m_phemline[i]->SetColor(cr); //���û�����ɫ;  
		}
	}

	if (!m_pDividePeakBaseLine.empty())
	{
		typedef std::list<CChartLineSerie*>::iterator LI;
		LI iter=m_pDividePeakBaseLine.begin();
		if ((*iter)->GetColor()!=cr)
		{
			for (; iter!=m_pDividePeakBaseLine.end(); iter++)
				(*iter)->SetColor(cr); //���ô�ֱ�ַ������ɫ; 
		}
	}

	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	
}

// !-----------------------2010.12.09 -------------------------------
VOID CRealTime::SetRealTimeData(int begin, int end)
{
	if (!m_InputSema.empty())
		m_InputSema.clear();

	for (; begin!=end; ++begin)
	{
		//CRealTimeSema RealTimeSema(m_OrigSema[begin]);
		m_InputSema.push_back( m_OrigSema[begin] );
	}
}
VOID CRealTime::ClearRealTimeCurve() // ���ʵʱ����
{
	if (counter==0) 
	{
		counter++;
		return;
	}
	m_pCtrl->EnableRefresh(false);
	m_pDrawCurve->ClearSerie();
	//m_pLinePeakSeek->ClearSerie();          //����ϴ�����
	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	
}
VOID CRealTime::DrawRealTimeCurve()
{
	m_pCtrl->EnableRefresh(false);

	m_pDrawCurve->SetColor(RGB(200, 200, 200)); //�������ߵ���ɫ(�׻�)
	CChartAxis* pAxisSelect;
	m_pCtrl->SetBackColor(RGB(60, 60, 60)); //���ñ�����ɫ
	pAxisSelect = m_pCtrl->GetLeftAxis(); //����Y����
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(dYMin*1.0, dYMax*1.1, COORDINATE_SET);
	pAxisSelect = m_pCtrl->GetBottomAxis(); //����X����
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(dXMin, dXMax, COORDINATE_SET);
	pAxisSelect->SetVisible(true);

	m_pDrawCurve->m_vPoints.InitPoints(m_OrigSema.size());
	for (size_t i=0; i<m_RTTotalSema.size(); ++i)
	{//������
		m_pDrawCurve->m_vPoints.AddPoint(m_RTTotalSema[i].x, m_RTTotalSema[i].y);
	}

	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	
}
VOID CRealTime::DrawRealTimeResult()
{
	m_pCtrl->EnableRefresh(false);
	//m_pLinePeakSeek->SetColor(RGB(200, 200, 200)); //�������ߵ���ɫ(�׻�)
	CChartAxis* pAxisSelect;

	m_pCtrl->SetBackColor(RGB(60, 60, 60)); //���ñ�����ɫ
	pAxisSelect = m_pCtrl->GetLeftAxis(); //����Y����
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(dYMin*1.0, dYMax*1.1, COORDINATE_SET);
	pAxisSelect = m_pCtrl->GetBottomAxis(); //����X����
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(dXMin, dXMax, COORDINATE_SET);
	//m_pPoint->SetColor(RGB(255, 70, 70)); //���ñ����������ɫ(��ɫ)
	m_pPeak->SetPointType(CChartPointsSerie::ptTriangle); //���ò���������Ǳ��
	m_pPeak->SetColor (RGB(0, 138, 138)); //���ñ겨�岨�ȵ����ɫ(��ɫ)
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetVisible(true);

	//m_pLinePeakSeek->m_vPoints.InitPoints(m_OrigSema.size());
	//for (size_t i=0; i<m_OrigSema.size();++i)
	//{//������
	//	m_pLinePeakSeek->m_vPoints.AddPoint(m_OrigSema[i].x, m_OrigSema[i].y);
	//}
	//
	//if (m_Peak.size()==0)
	//{
	//	m_pCtrl->EnableRefresh(true);
	//	m_pCtrl->RefreshCtrl();	
	//	return;
	//}

	//m_pPoint->m_vPoints.InitPoints(m_Peak.size());
	m_pPeak->m_vPoints.Clear();
	m_pPeak->m_vPoints.InitPoints(m_Peak.size());
	for (unsigned i=0; i<m_RTPeak.size(); ++i)
	{//���������
		//double x[]={m_RTPeak[i].StartSema.x, m_RTPeak[i].FinSema.x};
		//double y[]={m_RTPeak[i].StartSema.y, m_RTPeak[i].FinSema.y};
		//m_pPoint->m_vPoints.AddPoints(x, y, 2);
		m_pPeak->m_vPoints.AddPoint(m_RTPeak[i].x, m_RTPeak[i].y); //point
	}

	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	

}


// !-----------------------2013.04.19 -------------------------------
// !-----------------------ʵʱ���߻��ֹ��ܽ��빤��վ--------------------
VOID CRealTime::RealTimeProcess(CAnalysisData* pSample_Parameter_PeakProcess, 
						double xSrc[], 
						double ySrc[], 
						int nSrc, 
						CPeakList &cPeakList,
						CPeakList &cAllPeakList)
{
	if (!m_isRealTimeParametersSet)
	{
		RT_SetProcessParameter(pSample_Parameter_PeakProcess);
	}

	RT_AddRealTimePoints(xSrc, ySrc, nSrc);
	
	cPeakList.RemoveAll(); //��֤ÿ����������µ�ʵʱ���ֽ������

	m_InputNums += nSrc;
	if (m_InputNums >= 100) //ÿ100�����ݽ���һ��ʵʱ���߻���
	{
		m_InputNums = 0;
		RT_Averaging();
		RT_AvgDataSlope();
		RT_PeakSearch();
		RT_PeakReturn();
		RT_RealTimeSema2Peak();
		RT_GetPeakArea();
		RT_GetPeakHeight();
		RT_Peak2PeakList(cPeakList, cAllPeakList);
		//RT_Save2PeakList(cPeakList);
	}
}


VOID CRealTime::RT_SetProcessParameter(CAnalysisData* pSample_Parameter_PeakProcess)
{
	double threshold, minHalfPeakWidth, minPeakArea, minPeakHeight;
	CString szAnalysisData = pSample_Parameter_PeakProcess->GetXMLString();
	if (pSample_Parameter_PeakProcess->GetValue_Double(XMLPATH_PEAKPROC_TRD, threshold)!=IAERROR_SUCCESS)  return;
	if (pSample_Parameter_PeakProcess->GetValue_Double(XMLPATH_PEAKPROC_MPW, minHalfPeakWidth)!=IAERROR_SUCCESS) return;
	if (pSample_Parameter_PeakProcess->GetValue_Double(XMLPATH_PEAKPROC_MPA, minPeakArea)!=IAERROR_SUCCESS) return;
	if (pSample_Parameter_PeakProcess->GetValue_Double(XMLPATH_PEAKPROC_MPH, minPeakHeight)!=IAERROR_SUCCESS) return;

	// CP �� AP ��������
	int cp = 3; //�����е�
	int ap = 5; //ƽ������
	if (pSample_Parameter_PeakProcess->GetValue_Int(XMLPATH_PEAKPROC_CP, cp)!=IAERROR_SUCCESS)  cp = 3;
	if (pSample_Parameter_PeakProcess->GetValue_Int(XMLPATH_PEAKPROC_AP, ap)!=IAERROR_SUCCESS)  ap = 5;
	if (cp>10) cp=10; 
	else if (cp<0)  cp=3;
	if (ap>10) ap=10;
	else if (ap<0) ap=5; 

	RT_SetProcessParameter(false, false, ap, cp, threshold, minHalfPeakWidth, minPeakArea);

	m_isRealTimeParametersSet = true;
}
VOID CRealTime::RT_SetProcessParameter(bool is_divide_peak,
									   bool has_nagetive_spike,
									   int avgs,
									   int series,
									   double threshold,
									   double half_peak_width,
									   double peak_area)
{
	m_IsDividePeak = is_divide_peak; //�Ƿ�����ֱ�ַ�
	m_HasNagetiveSpike = has_nagetive_spike; //�Ƿ��и������
	m_Avgs = avgs; //�ڲ�ƽ������.Ĭ��5
	m_Series = series; //�ڲ��������жϡ�Ĭ��3
	m_Threshold = threshold; //����б��
	m_HalfPeakWidth = half_peak_width; //��С����
	m_PeakArea = peak_area; //��С�����

	m_maxSlope = 0.0f;
}

VOID CRealTime::RT_AddRealTimePoints(double xSrc[], double ySrc[], int nSrc)
{
	const int orig_sema_size = m_OrigSema.size();
	if (nSrc==0) return;

	for (int i=0; i<nSrc; i++)
	{
		int sema_no = orig_sema_size + i;
		CSema OneSema(sema_no, xSrc[i], ySrc[i]);
		m_OrigSema.push_back(OneSema);
		m_InputSema.push_back(OneSema);
	}
}

BOOL CRealTime::addRealTimePoints(const CCurveData &curveData)
{
	const int orig_sema_size = m_OrigSema.size();
	const int curveSize = curveData.m_arrayData.GetSize(); 
	if (curveSize==0) return FALSE;
	
	for (int i=0; i<curveSize; i++)
	{
		int sema_no = orig_sema_size + i;
		CSema OneSema(sema_no, curveData.m_arrayData[i].dX, curveData.m_arrayData[i].dY);
		m_OrigSema.push_back(OneSema);
		m_InputSema.push_back(OneSema);
	}

	return TRUE;
}

// ʵʱ���ߴ���
//static std::deque<CRealTimeSema> dequetmpSema;// ȫ�־�̬������Ϊƽ��������
//static std::deque<CRealTimeSema> dequeBufSema;// ȫ�־�̬������ΪѰ�建��


VOID CRealTime::RT_Averaging()
{//step1: ����m_AvgsΪ1���ƽ�����ߣ�ʣ���������Ϊ1��
	if (m_Avgs < 1)
	{
		return; //MessageBox(_T("����ƽ����"));
	}

	const unsigned TotalNumber = m_RTTotalSema.size();
	const unsigned InputNum = m_InputSema.size(); //�����ź�������
	const unsigned TmpNum = dequetmpSema.size();  //��������ź�������
	const unsigned CurrNum = InputNum + TmpNum;
	if (CurrNum > m_Avgs) //�Ӷ�����ȡ��ƽ����
	{
		dequetmpSema.insert(dequetmpSema.end(), m_InputSema.begin(), m_InputSema.end());
		m_InputSema.clear();
		for (unsigned i=0; i<=CurrNum; ++i)
		{
			double m_AvgsTime = 0.0;
			double m_AvgsSema = 0.0;
			if (i % m_Avgs == 0 && i+m_Avgs < CurrNum) 
			{
				for (unsigned j=i; j< i+m_Avgs; j++)
				{
					m_AvgsTime += dequetmpSema[0].x;
					m_AvgsSema += dequetmpSema[0].y;
					m_RTTotalSema.push_back(dequetmpSema[0]);
					dequetmpSema.pop_front(); //����
				}
				m_AvgsTime = m_AvgsTime / m_Avgs;
				m_AvgsSema = m_AvgsSema / m_Avgs;

				CRealTimeSema OneRTSema(i+TotalNumber, m_AvgsTime, m_AvgsSema);
				m_InputSema.push_back(OneRTSema);
			}
			if (dequetmpSema.size() < m_Avgs) //ʣ��
			{
				break;
			}
		}
	}
	else //ֱ�ӷ��ڶ�����
	{
		dequetmpSema.insert(dequetmpSema.end(), m_InputSema.begin(), m_InputSema.end());
	}
}
VOID CRealTime::RT_AvgDataSlope()         
{//step2: ���ƽ��������ÿһ���б��

	//������Ҫ�Ż�
	if (!m_firstOrder.empty())
	{
		const unsigned totsize =  m_RTTotalAvgSema.size()-1;
		double ydiff = m_InputSema[0].y -  m_RTTotalAvgSema[totsize].y;
		double xdiff = m_InputSema[0].x -  m_RTTotalAvgSema[totsize].x;
		double first_k = ydiff / xdiff;
		m_firstOrder.push_back(first_k);
		if (first_k>=m_maxSlope)
			m_maxSlope =first_k;
	}

	const unsigned avgDataSize = m_InputSema.size()-1;
	double *k = new double[avgDataSize];
	for (unsigned i=0; i<avgDataSize; ++i)
	{	
		double ydiff = m_InputSema[i+1].y - m_InputSema[i].y;
		double xdiff = m_InputSema[i+1].x - m_InputSema[i].x;
		k[i] = (ydiff) / (xdiff);
		if (k[i]>=m_maxSlope)
			m_maxSlope = k[i];
		m_firstOrder.push_back(k[i]);
	}

	delete [] k;
	k = NULL;

	for (unsigned i=0; i<m_InputSema.size(); ++i)
	{
		 m_RTTotalAvgSema.push_back(m_InputSema[i]);
	}
}
VOID CRealTime::RT_PeakSearch()
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

	//����ǰ�����ݳ�ʼ��
	static enumState lastState = STATELESS; //��һ��δ������ϵ�״̬
	//static unsigned m_id = 0;//��̬��m_firstOrder�±꣩
	static bool OnlyOnce = true;
	const unsigned firstOrderSize = m_firstOrder.size();
	unsigned DataSize = 0;
	if (OnlyOnce)
	{
		DataSize = m_InputSema.size() + dequeBufSema.size()-1; //��ν��д�������ݸ���
		OnlyOnce = !OnlyOnce;
	} 
	else
		DataSize = m_InputSema.size() + dequeBufSema.size();
	dequeBufSema.insert(dequeBufSema.end(),m_InputSema.begin(),m_InputSema.end());//������źŶ��ŵ�����ĩ��
	m_InputSema.clear(); //�����ź����,�ⲽ���Էŵ���һ������������
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
			for (; i<DataSize-m_Series && hState==STATELESS; ++i, ++m_id)
			{
				if (i>=DataSize-m_Series-1 || m_id>=firstOrderSize-m_Series) 
				{
					++m_id;
					hState = HANDLE_POSTPROCESS;
					break;
				}

				if (m_firstOrder[m_id]<=0)
				{//�������
					if (m_HasNagetiveSpike) //���Ǹ������
					{
						if (fabs(m_firstOrder[m_id])<m_Threshold)
							continue;
						if (fabs(m_firstOrder[m_id])>=m_Threshold)
						{
							for (int s=1; s<=m_Series; ++s)
							{
								if (fabs(m_firstOrder[m_id+s])<m_Threshold)
								{
									isContinuousWave = false; 
									break;
								}
								if (fabs(m_firstOrder[m_id+s])>m_Threshold)
								{
									if (s==m_Series)
									{
										if ((!m_RTPeak.empty())
											&& (m_RTPeak[m_RTPeak.size()-1].pts == CRealTimeSema::MINUS_CONTINUOUS_WAVE)
											&& isContinuousWave)
										{
											//::AfxMessageBox(_T("dequeBufSema.empty()"));
											m_RTPeak.push_back(m_RTPeak[m_RTPeak.size()-1]); //�ҵ�(����)���
										}
										//�����������
										else
										{
											dequeBufSema[i].pts = CRealTimeSema::MINUS_START_POINT; //���Ϊ�������
											m_RTPeak.push_back(dequeBufSema[i]); //�ҵ����

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
				if (m_firstOrder[m_id]>=0) //�Ǹ��忼�����
				{
					if (m_firstOrder[m_id]>m_Threshold)
					{
						for (int s=1; s<=m_Series; ++s)
						{
							if (m_firstOrder[m_id+s]<m_Threshold)
							{
								isContinuousWave = false; 
								break;
							}
							if (m_firstOrder[m_id+s]>=m_Threshold)
							{
								if (s==m_Series)
								{
									if ((!m_RTPeak.empty())
										&& (m_RTPeak[m_RTPeak.size()-1].pts == CRealTimeSema::PLUS_CONTINUOUS_WAVE)
										&& isContinuousWave)
									{
										//::AfxMessageBox(_T("dequeBufSema.empty()"));
										m_RTPeak.push_back(m_RTPeak[m_RTPeak.size()-1]); //�ҵ�(����)���
									}
									//�����������
									else
									{
										dequeBufSema[i].pts = CRealTimeSema::PLUS_START_POINT; //���Ϊ���
										m_RTPeak.push_back(dequeBufSema[i]); //�ҵ����

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
			for (j=i; j<DataSize-m_Series && hState == PLUS_UPCOMING_WAVE; ++j, ++m_id)
			{

				if (j>=DataSize-m_Series-1 || m_id>=firstOrderSize-m_Series) 
				{
					++m_id;
					hState = HANDLE_POSTPROCESS;
					break;
				}

				if (m_firstOrder[m_id]<0)
					continue;
				if (m_firstOrder[m_id]>=0)
				{
					for (int s=1; s<=m_Series; ++s)
					{
						if (m_firstOrder[m_id+s]>0)
							break;
						if (m_firstOrder[m_id+s]<0)
						{
							if (s==m_Series)
							{
								dequeBufSema[j+s].pts = CRealTimeSema::PLUS_PEAK_POINT; //��ǲ���
								m_RTPeak.push_back(dequeBufSema[j+s]); //�ҵ�����

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
			for (k = j; k<DataSize-m_Series && hState == PLUS_DOWNGOING_WAVE; ++k, ++m_id)
			{
				if (k>=DataSize-m_Series-1 || m_id>=firstOrderSize-m_Series) 
				{
					++m_id;
					hState = HANDLE_POSTPROCESS;
					break;
				}

				if ( m_firstOrder[m_id]<0 ) 
				{
					if (fabs(m_firstOrder[m_id])>m_Threshold)  //2010.08.24 ���ƽͷ�崦������
					{
						isFlatPeakProcessed = true;
						continue;
					}

					if (isFlatPeakProcessed && (fabs(m_firstOrder[m_id]) < m_Threshold) && (m_firstOrder[m_id]!=0.000000))
					{
						for (int s=1; s<=m_Series; ++s)
						{
							if (fabs(m_firstOrder[m_id+s])>m_Threshold)
								break;
							if (fabs(m_firstOrder[m_id+s])<m_Threshold && (m_firstOrder[m_id+s]!=0.000000))
							{
								if (s==m_Series)
								{
									//getFinishPoint = k+1;
									dequeBufSema[k+1].pts = CRealTimeSema::PLUS_FINISH_POINT; //����յ�
									m_RTPeak.push_back(dequeBufSema[k+1]); //�ҵ��յ�
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
				if ( m_firstOrder[m_id]>0 )
				{
					//getFinishPoint = k;
					dequeBufSema[k+1].pts = CRealTimeSema::PLUS_CONTINUOUS_WAVE; //��ǣ��������յ�
					m_RTPeak.push_back(dequeBufSema[k+1]); //�ҵ��յ�
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
			for (j=i; j<DataSize-m_Series && hState==MINUS_DOWNGOING_WAVE; ++j,++m_id)
			{
				if (j>=DataSize-m_Series-1 || m_id >= firstOrderSize-m_Series) 
				{
					++m_id;
					hState = HANDLE_POSTPROCESS;
					break;
				}

				if (m_firstOrder[m_id]>0)
					continue;
				if (m_firstOrder[m_id]<=0)
				{
					for (int s=1; s<=m_Series; ++s)
					{
						if (m_firstOrder[m_id+s]<0)
							break;
						if (m_firstOrder[m_id+s]>0)
						{
							if (s==m_Series)
							{
								dequeBufSema[j+s].pts = CRealTimeSema::MINUS_TROUGH_POINT; //��ǲ���
								m_RTPeak.push_back(dequeBufSema[j+s]); //�ҵ�����

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
			for (k=j; k<DataSize-m_Series && hState==MINUS_UPCOMING_WAVE; ++k,++m_id)
			{
				if (k>=DataSize-m_Series-1 || m_id >= firstOrderSize-m_Series)
				{
					++m_id;
					hState = HANDLE_POSTPROCESS;
					break;
				}
				if (m_firstOrder[m_id]>=0)
				{
					if (m_firstOrder[m_id]>m_Threshold)  //2010.08.25 ���ƽͷ�崦������
					{	
						isFlatPeakProcessed = true;
						continue;
					}
					if (isFlatPeakProcessed && m_firstOrder[m_id]<m_Threshold && (m_firstOrder[m_id]!=0.000000)) 
					{
						for (int s=1; s<=m_Series; ++s)
						{
							if (m_firstOrder[m_id+s]>m_Threshold)
								break;
							if (m_firstOrder[m_id+s]<m_Threshold  && (m_firstOrder[m_id+s]!=0.000000))
							{
								if (s==m_Series)
								{
									//getFinishPoint = k+1;
									dequeBufSema[k+1].pts = CRealTimeSema::MINUS_FINISH_POINT; //��Ǹ����յ�
									m_RTPeak.push_back(dequeBufSema[k+1]); //�ҵ������յ�
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
				if (m_firstOrder[m_id]<0)
				{
					//getFinishPoint = k;
					dequeBufSema[k+1].pts = CRealTimeSema::MINUS_CONTINUOUS_WAVE; //��ǣ��������յ�
					m_RTPeak.push_back(dequeBufSema[k+1]); //�ҵ��յ�
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

			m_id -= dequeBufSema.size()-dequeId-m_Series-1; //�����±���ʼ��

			for (unsigned x=0; x<dequeId; ++x)
				dequeBufSema.pop_front();

			hState = HANDLE_ACCOMPLISH;
			break;

		default:
			break;
		}
	}
}

VOID CRealTime::RT_PeakReturn()  
{// step4: ������ĵ㻹ԭ��Ԫ����
	const unsigned currentTotalSemaSize = m_RTTotalSema.size();
	const unsigned peakSemaSize = m_RTPeak.size();
	static unsigned i=0; //������㣬���壬����3���±�
	static unsigned p=1; //���Ƽ�ʱ��ԭ��������±� d

	// ��ʱ��ԭ�������
	for ( ; p<peakSemaSize-3 && peakSemaSize>2; p+=3)
	{
		double maxValue = -INT_MAX;
		for (unsigned x = m_RTPeak[p-1].idx; x<m_RTPeak[p].idx+m_Avgs && m_RTPeak[p].idx+m_Avgs < currentTotalSemaSize; ++x)
		{
			if (maxValue <= m_RTTotalSema[x].y)
			{
				maxValue = m_RTTotalSema[x].y;
				m_RTPeak[p] = m_RTTotalSema[x];
			}
		}
	}
	//�ȵ���㣬���壬����3������֮���ٻ�ԭ
	for ( ; i<peakSemaSize-3 && peakSemaSize>2 /*&& peakSemaSize%3==0*/; i+=3)  //���
	{
		for (unsigned j=m_RTPeak[i].idx; j<m_RTPeak[i].idx+m_Avgs && j<currentTotalSemaSize-1; j++) //�Ȼ�ԭ���
		{
			double k=(m_RTTotalSema[j+1].y-m_RTTotalSema[j].y)/(m_RTTotalSema[j+1].x-m_RTTotalSema[j].x);
			if (fabs(k)>m_Threshold)
			{
				if (j==m_RTPeak[i].idx+m_Avgs-1)
				{
					m_RTPeak[i] = m_RTTotalSema[j];
					break;
				}
			}
			else
			{
				m_RTPeak[i] = m_RTTotalSema[j];
				break;
			}
		}
		for (unsigned j=m_RTPeak[i+2].idx; j<m_RTPeak[i+2].idx+m_Avgs && j<currentTotalSemaSize-1; j++) //�ٻ�ԭ���
		{
			double k=(m_RTTotalSema[j+1].y-m_RTTotalSema[j].y)/(m_RTTotalSema[j+1].x-m_RTTotalSema[j].x);
			if (fabs(k)>m_Threshold)
			{
				if (j==m_RTPeak[i+2].idx+m_Avgs-1)
				{
					m_RTPeak[i+2] = m_RTTotalSema[j];
					break;
				}
			}
			else
			{
				m_RTPeak[i+2] = m_RTTotalSema[j];
				break;
			}
		}

		//���ԭ������߲��ȵ�
		double maxValue = -INT_MAX;
		double minValue = INT_MAX;
		//if (Peak[i].Type>=1) //��ԭ����
		//	{
		for (unsigned j=m_RTPeak[i].idx; j<m_RTPeak[i+2].idx; ++j)
		{
			if (maxValue <= m_RTTotalSema[j].y)
			{
				maxValue = m_RTTotalSema[j].y;
				m_RTPeak[i+1] = m_RTTotalSema[j];
			}
		}
	}
}


VOID CRealTime::RT_RealTimeSema2Peak()
{
	const int rt_peak_size = m_RTPeak.size();
	const int peak_size = int(rt_peak_size / 3);

	for (size_t i=m_prevprocess_peak_nums; i<peak_size; ++i)
	{
		CSema start_sema(m_RTPeak[i*3+0].idx, m_RTPeak[i*3+0].x, m_RTPeak[i*3+0].y);
		CSema peak_sema( m_RTPeak[i*3+1].idx, m_RTPeak[i*3+1].x, m_RTPeak[i*3+1].y);
		CSema fin_sema(  m_RTPeak[i*3+2].idx, m_RTPeak[i*3+2].x, m_RTPeak[i*3+2].y);

		CPeak peak(start_sema, peak_sema, fin_sema);
		m_Peak.push_back(peak);
	}

	m_prevprocess_peak_nums = peak_size;
}
VOID CRealTime::RT_GetPeakArea()
{
	bool isSetUnit = false;
	{//���Խ��ּܳ���ʱע������������ڵĴ��� 
		bool isGC1120  = ((g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC_PX) || (g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC1120))  ? true : false;
		bool isGC2400  = (g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC2400)  ? true : false;
		bool isGC1100w = (g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC1100W) ? true : false;
		bool isGC1100p = (g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC1100P) ? true : false;
		isSetUnit = (isGC1120 || isGC2400 || isGC1100w || isGC1100p);
	}

	double magnification = 1.0f;
	if (isSetUnit) 
		magnification = 60000.0f;
	else
		magnification = 60.0f;


	const unsigned PeakSize = m_Peak.size();
	const unsigned SemaSize = m_RTTotalSema.size();
	//unsigned j = 0;
	for (unsigned i=0; i<PeakSize; ++i)  
	{
		double x2 = m_Peak[i].FinSema.x;           
		double y2 = m_Peak[i].FinSema.y;
		double x1 = m_Peak[i].StartSema.x;
		double y1 = m_Peak[i].StartSema.y;
		double k = (y2-y1)/(x2-x1);
		double c = y1-k*x1;

		double area = 0.0;
		for (unsigned j = 0; j<SemaSize; ++j)
		{
			if (m_RTTotalSema[j].x - m_Peak[i].StartSema.x >= 0.000000) //�ҵ����
			{	
				do 
				{
					double x4 = m_RTTotalSema[j+1].x;
					double y4 = m_RTTotalSema[j+1].y;
					double x3 = m_RTTotalSema[j].x;
					double y3 = m_RTTotalSema[j].y;

					double h = fabs(x4-x3);
					double front = fabs(y3-(x3*k+c));
					double behind = fabs(y4-(x4*k+c));
					area += (front+behind)*h/2;
					j++;
				} while (m_RTTotalSema[j].x - x2 <= 0.0000f); //�ҵ��յ�
				m_Peak[i].PeakArea = area * magnification;
				break;
			}
		}
	}
}
VOID CRealTime::RT_GetPeakHeight()
{
	const int PeakSize = m_Peak.size();

	for (int i=0; i<PeakSize; ++i)
	{
		double x1 = m_Peak[i].StartSema.x;
		double y1 = m_Peak[i].StartSema.y;
		double x2 = m_Peak[i].FinSema.x;
		double y2 = m_Peak[i].FinSema.y;

		double tana = (y2-y1)/(x2-x1);
		double c = y1 - x1 * tana;

		double py = m_Peak[i].PeakSema.y; 
		double px = m_Peak[i].PeakSema.x;
		double ty = px*tana+c;

		double peakHeight = fabs(ty - py); //���ָ�ֵ����Ŀ���
		m_Peak[i].PeakHeight = peakHeight; //���
	}
}
VOID CRealTime::RT_Peak2PeakList(CPeakList &cPeakList, CPeakList &cAllPeakList)
{
	

	
	const int peak_size = m_Peak.size();

	for (size_t i=0; i<peak_size; i++)
	{
		CPeakStandard aPeakStard;
		aPeakStard.m_dReserveTime = m_Peak[i].PeakSema.x; //����ʱ��
		CCurveDataSingle startCurveData(m_Peak[i].StartSema.x, m_Peak[i].StartSema.y);
		CCurveDataSingle topCurveData(m_Peak[i].PeakSema.x, m_Peak[i].PeakSema.y);
		CCurveDataSingle endCurveData(m_Peak[i].FinSema.x, m_Peak[i].FinSema.y);
		aPeakStard.m_dataBegin = startCurveData;
		aPeakStard.m_dataTop = topCurveData;
		aPeakStard.m_dataEnd = endCurveData;
		//aPeakStard.m_dHalfWidth = m_Peak[i].HalfPeakWidth;	//����
		//aPeakStard.m_dWidth = m_Peak[i].PeakWidth;		//���
		aPeakStard.m_dHeight = m_Peak[i].PeakHeight;		//���
		//aPeakStard.m_dHeightPer = m_Peak[i].PeakHighPercent;	//���%
		aPeakStard.m_dArea = m_Peak[i].PeakArea;		//�����
		//aPeakStard.m_dAreaPer = m_Peak[i].PeakAreaPercent;		//�����%
		//aPeakStard.m_dDisfactor = m_Peak[i].Disfactor;   //��������??
		//aPeakStard.m_dSymmetry = m_Peak[i].Tailing;    //��β����
		//aPeakStard.m_dResolution = m_Peak[i].Resolution;  //�����
		//aPeakStard.m_dColumnPlate = m_Peak[i].ColumnPlate; //����������
		//aPeakStard.m_dResultConcentration = 0; //Ũ��
		cPeakList.m_cArrayData.Add(aPeakStard);
		cAllPeakList.m_cArrayData.Add(aPeakStard);
	}

	m_Peak.clear();
}


VOID CRealTime::RT_Save2PeakList(CPeakList &cPeakList)
{
	cPeakList.RemoveAll(); //��֤ÿ����������µ�ʵʱ���ֽ������

	const int rt_peak_size = m_RTPeak.size();
	const int peak_size = int(rt_peak_size / 3);

	//CPeak����ת���CPeakList ����cPeakList
	for (size_t i=m_prevprocess_peak_nums; i<peak_size; ++i)
	{
		CPeakStandard aPeakStard;
		//aPeakStard.m_cPeakType.all = m_Peak[i].PeakType.all;//������
		aPeakStard.m_dReserveTime = m_RTPeak[i*3+1].x; //����ʱ��
		CCurveDataSingle startCurveData(m_RTPeak[i*3+0].x, m_RTPeak[i*3+0].y);
		CCurveDataSingle topCurveData(m_RTPeak[i*3+1].x, m_RTPeak[i*3+1].y);
		CCurveDataSingle endCurveData(m_RTPeak[i*3+2].x, m_RTPeak[i*3+2].y);
		aPeakStard.m_dataBegin = startCurveData;
		aPeakStard.m_dataTop = topCurveData;
		aPeakStard.m_dataEnd = endCurveData;
		//aPeakStard.m_dHalfWidth = m_Peak[i].HalfPeakWidth;	//����
		//aPeakStard.m_dWidth = m_Peak[i].PeakWidth;		//���
		//aPeakStard.m_dHeight = m_Peak[i].PeakHeight;		//���
		//aPeakStard.m_dHeightPer = m_Peak[i].PeakHighPercent;	//���%
		//aPeakStard.m_dArea = m_Peak[i].PeakArea;		//�����
		//aPeakStard.m_dAreaPer = m_Peak[i].PeakAreaPercent;		//�����%
		//aPeakStard.m_dDisfactor = m_Peak[i].Disfactor;   //��������??
		//aPeakStard.m_dSymmetry = m_Peak[i].Tailing;    //��β����
		//aPeakStard.m_dResolution = m_Peak[i].Resolution;  //�����
		//aPeakStard.m_dColumnPlate = m_Peak[i].ColumnPlate; //����������
		//aPeakStard.m_dResultConcentration = 0; //Ũ��
		cPeakList.m_cArrayData.Add(aPeakStard);
	}

	m_prevprocess_peak_nums = peak_size;
}
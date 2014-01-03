#include "StdAfx.h"
#include "PeakSeak.h"
#include "WaveletTransform.h"
#include <fstream>
   
CPeakSeak::CPeakSeak(void)
{
}
CPeakSeak::~CPeakSeak(void)
{

}
 
VOID CPeakSeak::GetPeakResult(std::vector<CPeak> &Peak)
{	
	Peak = m_Peak;
}
VOID CPeakSeak::SetSemaDate(CString filePathName)
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
		delete [] pX;
		delete [] pY;
		return;
	}

	delete [] pX;
	delete [] pY;

	file.close();	//�ر�CSV�ļ�

}
VOID CPeakSeak::SetSemaDate(double x[], double y[], int n)
{
	//һ���Է���vector�Ĵ�С
	m_OrigSema.resize(n);

	dXMax = x[0];
	dXMin = x[0];
	dYMax = y[0];
	dYMin = y[0];
	int i=0;
	typedef std::vector<CSema>::iterator ITER;
	for (ITER iter=m_OrigSema.begin(); iter!=m_OrigSema.end(); ++iter)
	{
		CSema OneSema(i, x[i], y[i]);
		if (x[i] > dXMax) dXMax = x[i];
		if (x[i] < dXMin) dXMin = x[i];	
		if (y[i] > dYMax) dYMax = y[i];
		if (y[i] < dYMin) dYMin = y[i];
		*iter = OneSema;
		i++;
	}
	//dXMax = x[0];
	//dXMin = x[0];
	//dYMax = y[0];
	//dYMin = y[0];
	//for (int i=0; i<n; ++i)
	//{
	//	CSema OneSema(i, x[i], y[i]);
	//	if (x[i] > dXMax) dXMax = x[i];
	//	if (x[i] < dXMin) dXMin = x[i];	
	//	if (y[i] > dYMax) dYMax = y[i];
	//	if (y[i] < dYMin) dYMin = y[i];
	//	m_OrigSema.push_back(OneSema);
	//}
}
VOID CPeakSeak::SetSemaDate(const CChartPointsArray& vPoints)
{
	if(vPoints.GetPointsCount() == 0) return;
	dXMax = dXMin = vPoints.m_pPoints[0].x;
	dYMax = dYMin = vPoints.m_pPoints[0].y;
	for (size_t i=0; i<vPoints.GetPointsCount(); ++i)
	{
		CSema OneSema(i, vPoints.m_pPoints[i].x, vPoints.m_pPoints[i].y);
		if (OneSema.x > dXMax) dXMax = OneSema.x;
		if (OneSema.x < dXMin) dXMin = OneSema.x;	
		if (OneSema.y > dYMax) dYMax = OneSema.y;
		if (OneSema.y < dYMin) dYMin = OneSema.y;
		m_OrigSema.push_back(OneSema);
	}
}
VOID CPeakSeak::SetSemaDate(const CChartSerie* pSerie)
{
	if(!pSerie) return;
	SetSemaDate(pSerie->m_vPoints);
}
VOID CPeakSeak::SetParameter(bool IsDividePeak, bool HasNagetiveSpike, 
							double Threshold, double ThresholdFactor,
							double HalfPeakWidth, double PeakArea, double PeakHeight,
							int Series, int Avgs)
{
	m_IsDividePeak=IsDividePeak; //�Ƿ�����ֱ�ַ�
	m_HasNagetiveSpike=HasNagetiveSpike; //�Ƿ��и������

	m_Threshold=Threshold; //����б��
	m_thresholdFactor=ThresholdFactor;

	m_HalfPeakWidth=HalfPeakWidth; //��С����
	m_PeakArea=PeakArea; //��С�����
	m_PeakHeight=PeakHeight; //��С���

	m_Series=Series; //�ڲ��������жϡ�Ĭ��3
	m_Avgs=Avgs; //�ڲ�ƽ������.Ĭ��5
}
VOID CPeakSeak::ClearSemaDate()
{
	m_OrigSema.clear();
	m_Peak.clear();
}

/*
BOOL CPeakSeak::SignalProcess(
	CAnalysisData* pSample_Parameter_PeakProcess, 
	CPeakList &cPeakList, 
	CArray<double> *pcArrayHPW, 
	CArray<double> *pcArrayMPA, 
	CArray<double> *pcArrayMH, 
	int *pMinHalfPeakWidthEarseNum,
	int *pMinPeakAreaEarseNum,
	int *pMinPeakHeightEarseNum,
	CArray<structLine> *pcVLines,
	double *pdMinSlope, 
	double *pdMaxSlope)
{
	double threshold,minHalfPeakWidth,minPeakArea,minPeakHeight;
	CString szAnalysisData = pSample_Parameter_PeakProcess->GetXMLString();
	if (pSample_Parameter_PeakProcess->GetValue_Double(XMLPATH_PEAKPROC_TRD, threshold)!=IAERROR_SUCCESS)  return FALSE;
	if (pSample_Parameter_PeakProcess->GetValue_Double(XMLPATH_PEAKPROC_MPW, minHalfPeakWidth)!=IAERROR_SUCCESS) return FALSE;
	if (pSample_Parameter_PeakProcess->GetValue_Double(XMLPATH_PEAKPROC_MPA, minPeakArea)!=IAERROR_SUCCESS) return FALSE;
	if (pSample_Parameter_PeakProcess->GetValue_Double(XMLPATH_PEAKPROC_MPH, minPeakHeight)!=IAERROR_SUCCESS) return FALSE;

	// CP �� AP ��������
	int cp = 3; //�����е�
	int ap = 5; //ƽ������
	if (pSample_Parameter_PeakProcess->GetValue_Int(XMLPATH_PEAKPROC_CP, cp)!=IAERROR_SUCCESS)  cp = 3;
	if (pSample_Parameter_PeakProcess->GetValue_Int(XMLPATH_PEAKPROC_AP, ap)!=IAERROR_SUCCESS)  ap = 5;
	if (cp>10) cp=10; 
	else if (cp<0)  cp=3;
	if (ap>10) ap=10;
	else if (ap<0) ap=5; 
	

	// ����Ѱ����� (Ĭ��series, m_iavgN��Ҫ�޸�
	//��1-2���������Ƿ�����ֱ�ַ壬�Ƿ��и������
	//��3-5������������б�ʣ���С������С���������С���
	double threshold_factor = 1.0f;
	SetParameter(true, false, threshold, threshold_factor, minHalfPeakWidth, minPeakArea, minPeakHeight, cp, ap);	


	// ��ȡ���÷崦�������
	CIADataTable cPPTimeTable; 
	if (pSample_Parameter_PeakProcess->GetValue_Table(XMLPATH_PEAKPROC_PPT, cPPTimeTable) != IAERROR_SUCCESS) 
		return FALSE;

	// ��Ҫ����Ĳ�������pAnalysisData��������
	const long ConfigArgumentNum = cPPTimeTable.GetRowsNumber(); //�������������������
	deque<pair<int, ConfigArgument> > dep_cargs;
	for (long i=0; i<ConfigArgumentNum; ++i)
	{
		CString szState, szNo, szName, szBegainTime, szEndTime, szValue;
		if(!cPPTimeTable.GetCellValue(i, _T("State"), szState)) return FALSE;
		if(!cPPTimeTable.GetCellValue(i, _T("ID"), szNo)) return FALSE; 
		if(!cPPTimeTable.GetCellValue(i, _T("Name"), szName)) return FALSE; 
		if(!cPPTimeTable.GetCellValue(i, _T("BegainTime"), szBegainTime)) return FALSE;
		if(!cPPTimeTable.GetCellValue(i, _T("EndTime"), szEndTime)) return FALSE;
		if(!cPPTimeTable.GetCellValue(i, _T("Value"), szValue)) return FALSE;

		double BegainTime = _tstof(szBegainTime);
		double EndTime = _tstof(szEndTime);
		double Value = _tstof(szValue);

		if (szValue==_T("ǰ��")||szValue==_T("����"))
			Value = -1.0f;
		if (szValue==_T("����")||szValue==_T("����"))
			Value = 1.0f;

		if (szState == _T("��"))
		{
			int no = _wtoi(szNo);
			if (no<0) return FALSE;
			ConfigArgument cArg(szName, BegainTime, EndTime,  Value);
			dep_cargs.push_back(std::make_pair(no, cArg));
		}
	}

	std::sort(dep_cargs.begin(), dep_cargs.end(), strict_weak_ordering());
	typedef deque<pair<int, ConfigArgument> >::iterator DEP_ITER;
	deque<ConfigArgument> cArgs; 
	for (DEP_ITER iter=dep_cargs.begin(); iter!=dep_cargs.end(); ++iter)
	{
		cArgs.push_back(iter->second);
	}

	//������Ҫ��������ת������XML�ϵ��ֶ�ת���ɷ崦�����
	m_AnalysisConfigArg.TransformArgs(cArgs);//���һ��Ԥ�����������
	m_AnalysisConfigArg.SetArgs(cArgs);
	m_AnalysisConfigArg.PreprocessConfigArgs(); //���ò���Ԥ����
	


	//�崦����
	//��һ��ɨ�裺Ѱ��ؼ���
	SP_FirstScan();

	//�ڶ���ɨ�裺ȥ����Ҫ�ķ�
	SP_SecondScan();

	//������ɨ�裺�ص��崦��
	SP_ThirdScan();

	//���Ĵ�ɨ�裺���ߵ�У��
	SP_FourthScan();
	SP_FourthScan(); // 1 more time

	//�����ɨ�裺�ֹ�������
	SP_FifthScan(); 

	//������ɨ�裺���ֲ�����
	SP_SixthScan();

	//��ԭ���ƶ�ƽ��֮ǰ����
	SP_RestoreOriginalData();


	//CPeak����ת���CPeakList ����cPeakList
	for (size_t i=0; i<m_Peak.size(); ++i)
	{
		//int					m_nType;		//������

		CPeakStandard aPeakStard;
		aPeakStard.m_cPeakType.all = m_Peak[i].PeakType.all;//������
		aPeakStard.m_dReserveTime = m_Peak[i].PeakSema.x; //����ʱ��
		CCurveDataSingle startCurveData(m_Peak[i].StartSema.x, m_Peak[i].StartSema.y);
		CCurveDataSingle topCurveData(m_Peak[i].PeakSema.x, m_Peak[i].PeakSema.y);
		CCurveDataSingle endCurveData(m_Peak[i].FinSema.x, m_Peak[i].FinSema.y);
		aPeakStard.m_dataBegin = startCurveData;
		aPeakStard.m_dataTop = topCurveData;
		aPeakStard.m_dataEnd = endCurveData;
		aPeakStard.m_dHalfWidth = m_Peak[i].HalfPeakWidth;	//����
		aPeakStard.m_dWidth = m_Peak[i].PeakWidth;		//���
		aPeakStard.m_dHeight = m_Peak[i].PeakHeight;		//���
		aPeakStard.m_dHeightPer = m_Peak[i].PeakHighPercent;	//���%
		aPeakStard.m_dArea = m_Peak[i].PeakArea;		//�����
		aPeakStard.m_dAreaPer = m_Peak[i].PeakAreaPercent;		//�����%
		//aPeakStard.m_dResultPer = m_Peak[i];	//����������ٷֱ�%���ڷ����������ʹ�ã����б��в�����ʾ
		aPeakStard.m_dDisfactor = m_Peak[i].Disfactor;   //��������??
		aPeakStard.m_dSymmetry = m_Peak[i].Tailing;    //��β����
		aPeakStard.m_dResolution = m_Peak[i].Resolution;  //�����
		aPeakStard.m_dColumnPlate = m_Peak[i].ColumnPlate; //����������
		aPeakStard.m_dResultConcentration = 0; //Ũ��
		cPeakList.m_cArrayData.Add(aPeakStard);
	}

	//���м���������еİ�������еķ������ȡ��
	if (pcArrayHPW != NULL)
	{
		for (int i=0; i<m_cArrayHPW.GetSize(); ++i)
		{
			pcArrayHPW->Add(m_cArrayHPW.GetAt(i)); 
		}
	}

	if (pcArrayMPA != NULL)
	{
		for (int i=0; i<m_cArrayMPA.GetSize(); ++i)
		{
			pcArrayMPA->Add(m_cArrayMPA.GetAt(i)); 
		}
	}

	if(pcArrayMH != NULL)
	{
		for (int i=0; i<m_cArrayMH.GetSize(); ++i)
		{
			pcArrayMH->Add(m_cArrayMH.GetAt(i)); 
		}
	}

	if ( pMinHalfPeakWidthEarseNum != NULL )
		*pMinHalfPeakWidthEarseNum = m_MinHalfPeakWidthEarseNum;

	if ( pMinPeakAreaEarseNum != NULL )
		*pMinPeakAreaEarseNum = m_MinPeakAreaEarseNum;

	if ( pMinPeakHeightEarseNum != NULL )
		*pMinPeakHeightEarseNum = m_MinPeakHeightEarseNum;

	if ( pdMinSlope != NULL)
		*pdMinSlope = m_dMinSlope;

	if ( pdMaxSlope != NULL)
		*pdMaxSlope = m_dMaxSlope;

	if (pcVLines!=NULL)
	{
		const unsigned cArraySize = m_cArrayVLine.GetSize();
		for (unsigned i=0; i<cArraySize; ++i){
			structLine sLine=m_cArrayVLine[i];
			pcVLines->Add(sLine);
		}
	}
	
	return TRUE;
}
*/

VOID CPeakSeak::GetPeakTypeStr(CString &str) 
{
	str.Empty();
	const size_t PeakSize = m_Peak.size();
	for (size_t i=0 ; i<PeakSize; ++i)
	{
		CString szID;
		szID.Format(_T("%d"), i+1);
		str += _T("[") + szID + _T("] : ");
		str += m_Peak[i].PeakType.ConvertToTypeName(); 
		str += '\n';
	}
}


//������CHEM2009��Ŀ����Ҫ�õ��Ľӿ�
int CPeakDraw::GROUPID_FIRST = 1;  
int CPeakDraw::GROUPID_SECOND = 2;  
int CPeakDraw::GROUPID_THREE = 3;    

CPeakDraw::CPeakDraw()
{
	counter = 0;
	m_pDrawCurve = NULL;
	m_pLinePeakSeek = NULL;
	m_pPoint = NULL; 
	m_pPeak  = NULL;
	m_BaseLineColor = RGB(210, 0, 0);
}

VOID CPeakDraw::SetChartCtrl(CSuperChartCtrl* p)
{
	if ((p!=NULL) && (m_pCtrl != p))
	{
		m_pCtrl = p;

		if (!m_pCtrl->IsGroupExist((size_t)1))
			GROUPID_FIRST = m_pCtrl->CreatNewGroup((size_t)1); //�½��ؼ�Ԫ����1�����ڻ�ԭ��������

		if (!m_pCtrl->IsGroupExist((size_t)2))
			GROUPID_SECOND = m_pCtrl->CreatNewGroup((size_t)2);//�½��ؼ�Ԫ����2�����ڻ�Ѱ���ĵ�

		if (!m_pCtrl->IsGroupExist((size_t)3))
			GROUPID_THREE = m_pCtrl->CreatNewGroup((size_t)3);//�½��ؼ�Ԫ����2�����ڻ�Ѱ���Ļ���


		if (m_pDrawCurve == NULL) m_pDrawCurve = m_pCtrl->RealDraw(GROUPID_FIRST);
		if (m_pLinePeakSeek == NULL) m_pLinePeakSeek = m_pCtrl->RealDraw(GROUPID_FIRST);
		if (m_pPoint == NULL) 
		{
			m_pPoint = m_pCtrl->CreatePointsSerie(); //��������յ���
			m_pPoint->SetGroupID(GROUPID_SECOND);
		}
		if (m_pPeak  == NULL) 
		{
			m_pPeak  = m_pCtrl->CreatePointsSerie(); //����������
			m_pPeak->SetGroupID(GROUPID_SECOND);
		}
	}
}
VOID CPeakDraw::ClearLine()
{
	if (counter==0) 
	{
		counter++;
		return;
	}

	m_pCtrl->EnableRefresh(false);


	m_pDrawCurve->ClearSerie();
	m_pLinePeakSeek->ClearSerie();          //����ϴ�����
	m_pPoint->ClearSerie();					//����ϴθ�����յ�
	m_pPeak->ClearSerie();					//����ϴθ����岨�ȵ�


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

//	m_pCtrl->RemoveAllChartStrings();  //����ϴ��ַ���
	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	
}
VOID CPeakDraw::DrawCurve()
{
	m_pCtrl->EnableRefresh(false);

	m_pDrawCurve->SetColor(RGB(200, 200, 200)); //�������ߵ���ɫ(�׻�)
	CChartAxis* pAxisSelect;
	m_pCtrl->SetBackColor(RGB(60, 60, 60)); //���ñ�����ɫ
	pAxisSelect = m_pCtrl->GetLeftAxis(); //����Y����
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(m_PeakSeak.dYMin*1.0, m_PeakSeak.dYMax*1.1, COORDINATE_SET);
	pAxisSelect = m_pCtrl->GetBottomAxis(); //����X����
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(m_PeakSeak.dXMin, m_PeakSeak.dXMax, COORDINATE_SET);
	pAxisSelect->SetVisible(true);

	m_pDrawCurve->m_vPoints.InitPoints(m_PeakSeak.m_OrigSema.size());
	for (size_t i=0; i<m_PeakSeak.m_OrigSema.size(); ++i)
	{//������
		m_pDrawCurve->m_vPoints.AddPoint(m_PeakSeak.m_OrigSema[i].x, m_PeakSeak.m_OrigSema[i].y);
	}

	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	

}
VOID CPeakDraw::CA_SignalDisplay()
{
	CA_DrawBaseLine();//����
	DrawResultLine();//�����
}
VOID CPeakDraw::CA_DrawBaseLine()
{
	if (m_PeakSeak.m_Peak.size()==0)
	{
		return;
	}

	const unsigned PeakSize = m_PeakSeak.m_Peak.size();
	//������������ӵ�ֱ��ָ��(��һ�֣������������յ����ӵĻ���)
	//�������У����У���ֱ�ַ�
	for (unsigned i=0; i<PeakSize; ++i)
	{
		CChartLineSerie* baseline;  
		baseline = m_pCtrl->CreateLineSerie(); 
		baseline->SetColor(m_BaseLineColor); //���û�����ɫ/*RGB(210, 0, 0)*/
		m_phemline.push_back(baseline);
	}
	//������
	for (unsigned i=0; i<PeakSize; ++i)
	{
		//m_phemline[i]->SetGroupID(GROUPID_THREE);	
		m_phemline[i]->m_vPoints.Clear();
		m_phemline[i]->m_vPoints.InitPoints(4);
		double dx[] = { m_PeakSeak.m_Peak[i].StartSema.x, m_PeakSeak.m_Peak[i].FinSema.x };
		double dy[] = { m_PeakSeak.m_Peak[i].StartSema.y, m_PeakSeak.m_Peak[i].FinSema.y };
		m_phemline[i]->m_vPoints.AddPoints(dx, dy, 2);   //������
	}
	//����ֱ�ַ�ķַ���
	deque<pair<unsigned, unsigned> > VertDivideArgs;  //��ֱ�ַ�
	CA_GetBaseLineArgs(CPeak::VERTDIVIDE, m_PeakSeak.m_Peak, VertDivideArgs);
	while (!VertDivideArgs.empty())
	{
		//�ַ���
		unsigned begin = VertDivideArgs[0].first;
		unsigned end = VertDivideArgs[0].second;
		for (unsigned i= begin; i<end; ++i)
		{
			CChartLineSerie* divPeakLine;  
			divPeakLine = m_pCtrl->CreateLineSerie();//����һ�����ߵ�������
			divPeakLine->SetColor(m_BaseLineColor); //���û�����ɫ/*RGB(210, 0, 0)*/
			//divPeakLine->SetGroupID(GROUPID_THREE);
			divPeakLine->m_vPoints.InitPoints(4);
			double dx[] = { m_PeakSeak.m_Peak[i].FinSema.x, m_PeakSeak.m_Peak[i].FinSema.x };
			double dy[] = { m_PeakSeak.m_OrigSema[m_PeakSeak.m_Peak[i].FinSema.idx].y, m_PeakSeak.m_Peak[i].FinSema.y };
			divPeakLine->m_vPoints.AddPoints(dx, dy, 2);
			m_pDividePeakBaseLine.push_back(divPeakLine);
		}
		VertDivideArgs.pop_front();
	}
	m_pCtrl->EnableRefresh(true);
}
VOID CPeakDraw::SetCPeakSeakData(const CPeakSeak &inPeak)
{
	if (!m_PeakSeak.m_OrigSema.empty()) m_PeakSeak.m_OrigSema.clear();
	if (!m_PeakSeak.m_Peak.empty())     m_PeakSeak.m_Peak.clear();

	m_PeakSeak.m_OrigSema = inPeak.m_OrigSema;
	m_PeakSeak.m_Peak = inPeak.m_Peak;     //�������

	////���ò����ṹAnalysisConfigArguments m_AnalysisConfigArg;

	//�������
	m_PeakSeak.m_IsDividePeak = inPeak.m_IsDividePeak ; //�Ƿ�����ֱ�ַ�
	m_PeakSeak.m_HasNagetiveSpike = inPeak.m_HasNagetiveSpike; //�Ƿ��и������
	m_PeakSeak.m_Avgs = inPeak.m_Avgs; //�ڲ�ƽ������.Ĭ��5
	m_PeakSeak.m_Series = inPeak.m_Series; //�ڲ��������жϡ�Ĭ��3
	m_PeakSeak.m_Threshold = inPeak.m_Threshold; //����б��
	m_PeakSeak.m_HalfPeakWidth = inPeak.m_HalfPeakWidth; //��С����
	m_PeakSeak.m_PeakArea = inPeak.m_PeakArea; //��С�����

	//�ؼ������᷶Χ
	m_PeakSeak.dXMin = inPeak.dXMin;
	m_PeakSeak.dXMax = inPeak.dXMax;
	m_PeakSeak.dYMin = inPeak.dYMin;
	m_PeakSeak.dYMax = inPeak.dYMax;

}
VOID CPeakDraw::SignalDisplay()
{
	DrawBaseLine(); //����
	DrawDividePeakBaseLine();
	DrawResultLine(); //�����
}
VOID CPeakDraw::DrawBaseLine()
{
	if (m_PeakSeak.m_Peak.empty())
	{
		return;
	}
	bool isdraw = true;
	const unsigned PeakSize = m_PeakSeak.m_Peak.size();
	if ((!m_PeakSeak.m_IsDividePeak) && (PeakSize!=0)) //�Ǵ�ֱ�ַ�
	//if ((m_PeakSeak.m_Peak.dType != CPeak::VERTDIVIDE) && (PeakSize!=0)) //�Ǵ�ֱ�ַ�
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
				m_phemline[i]->SetGroupID(GROUPID_THREE);		
				m_phemline[i]->m_vPoints.InitPoints(4);
				double dx[] = { m_PeakSeak.m_Peak[i].StartSema.x, m_PeakSeak.m_Peak[i].FinSema.x };
				double dy[] = { m_PeakSeak.m_Peak[i].StartSema.y, m_PeakSeak.m_Peak[i].FinSema.y };
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
			if (m_PeakSeak.m_Peak[bg].Type==1 || m_PeakSeak.m_Peak[bg].Type==-1)
			{
				ed = bg;
				isSignalWave = true;
			}
			else /*(Peak[bg].Type>1)*/ //�ص�����
			{
				ed = bg;
				int mark = m_PeakSeak.m_Peak[bg].Type;
				while (ed<PeakSize && m_PeakSeak.m_Peak[ed].Type==mark)
				{
					if (ed+1==PeakSize && m_PeakSeak.m_Peak[PeakSize-1].Type==mark)
					{
						break;
					}
					ed++;
					if (m_PeakSeak.m_Peak[ed].Type!=mark)
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
				double dx[] = { m_PeakSeak.m_Peak[nodeIndex].StartSema.x, m_PeakSeak.m_Peak[nodeIndex].FinSema.x };
				double dy[] = { m_PeakSeak.m_Peak[nodeIndex].StartSema.y, m_PeakSeak.m_Peak[nodeIndex].FinSema.y };
				nodeIndex++;
				line->SetColor(m_BaseLineColor/*RGB(210, 0, 0)*/); //���û�����ɫ
				line->SetGroupID(GROUPID_THREE);
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
					double dx[] = { m_PeakSeak.m_Peak[nodeIndex].StartSema.x, m_PeakSeak.m_Peak[nodeIndex].FinSema.x };
					double dy[] = { m_PeakSeak.m_Peak[nodeIndex].StartSema.y, m_PeakSeak.m_Peak[nodeIndex].FinSema.y };
					nodeIndex++;
					line1->SetColor(m_BaseLineColor/*RGB(210, 0, 0)*/); //���û�����ɫ
					line1->SetGroupID(GROUPID_THREE);
					line1->m_vPoints.InitPoints(4);
					line1->m_vPoints.AddPoints(dx, dy, 2);
					m_phemline.push_back(line1);

					if (bg+1==ed)
					{
						double dxx[] = { m_PeakSeak.m_Peak[nodeIndex].StartSema.x, m_PeakSeak.m_Peak[nodeIndex].FinSema.x };
						double dyy[] = { m_PeakSeak.m_Peak[nodeIndex].StartSema.y, m_PeakSeak.m_Peak[nodeIndex].FinSema.y };
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
VOID CPeakDraw::DrawDividePeakBaseLine()
{
	if (m_PeakSeak.m_Peak.size()==0)
	{
		return;
	}
	
	m_pCtrl->EnableRefresh(false);

	const unsigned PeakSize = m_PeakSeak.m_Peak.size();
	for (unsigned i=0; i<PeakSize-1; ++i)
	{
		if (m_PeakSeak.m_Peak[i].dType == CPeak::VERTDIVIDE) //���ж��Ƿ��Ǵ�ֱ�ַ�
		{
			CChartLineSerie* line;  
			line = m_pCtrl->CreateLineSerie();//����һ�����ߵ�������
			line->SetColor(m_BaseLineColor/*RGB(210, 0, 0)*/); //���û�����ɫ
			line->m_vPoints.InitPoints(4);
			double dx[] = { m_PeakSeak.m_Peak[i].FinSema.x, m_PeakSeak.m_Peak[i].FinSema.x };
			double dy[] = { m_PeakSeak.m_OrigSema[m_PeakSeak.m_Peak[i].FinSema.idx].y, m_PeakSeak.m_Peak[i].FinSema.y };
			line->m_vPoints.AddPoints(dx, dy, 2);
			m_pDividePeakBaseLine.push_back(line);
		}
	}

	m_pCtrl->EnableRefresh(true);
}
VOID CPeakDraw::DrawResultLine()
{
	m_pCtrl->EnableRefresh(false);
	m_pLinePeakSeek->SetColor(RGB(200, 200, 200)); //�������ߵ���ɫ(�׻�)
	CChartAxis* pAxisSelect;

	m_pCtrl->SetBackColor(RGB(60, 60, 60)); //���ñ�����ɫ
	pAxisSelect = m_pCtrl->GetLeftAxis(); //����Y����
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(m_PeakSeak.dYMin*1.0, m_PeakSeak.dYMax*1.1, COORDINATE_SET);
	pAxisSelect = m_pCtrl->GetBottomAxis(); //����X����
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(m_PeakSeak.dXMin, m_PeakSeak.dXMax, COORDINATE_SET);
	m_pPoint->SetColor(RGB(255, 70, 70)); //���ñ����������ɫ(��ɫ)
	m_pPeak->SetPointType(CChartPointsSerie::ptTriangle); //���ò���������Ǳ��
	m_pPeak->SetColor (RGB(0, 138, 138)); //���ñ겨�岨�ȵ����ɫ(��ɫ)
	if (pAxisSelect==NULL) return;
	pAxisSelect->SetVisible(true);

	m_pLinePeakSeek->m_vPoints.InitPoints(m_PeakSeak.m_OrigSema.size());
	for (size_t i=0; i<m_PeakSeak.m_OrigSema.size();++i)
	{//������
		m_pLinePeakSeek->m_vPoints.AddPoint(m_PeakSeak.m_OrigSema[i].x, m_PeakSeak.m_OrigSema[i].y);
	}

	if (m_PeakSeak.m_Peak.size()==0)
	{
		m_pCtrl->EnableRefresh(true);
		m_pCtrl->RefreshCtrl();	
		return;
	}
	m_pPoint->m_vPoints.Clear();
	m_pPeak->m_vPoints.Clear();
	m_pPoint->m_vPoints.InitPoints(m_PeakSeak.m_Peak.size());
	m_pPeak->m_vPoints.InitPoints(m_PeakSeak.m_Peak.size());
	for (unsigned i=0; i<m_PeakSeak.m_Peak.size(); ++i)
	{//���������
		double x[]={m_PeakSeak.m_Peak[i].StartSema.x, m_PeakSeak.m_Peak[i].FinSema.x};
		double y[]={m_PeakSeak.m_Peak[i].StartSema.y, m_PeakSeak.m_Peak[i].FinSema.y};
		m_pPoint->m_vPoints.AddPoints(x, y, 2);
		m_pPeak->m_vPoints.AddPoint(m_PeakSeak.m_Peak[i].PeakSema.x, m_PeakSeak.m_Peak[i].PeakSema.y);//���岨��
	}

	m_pCtrl->EnableRefresh(true);
	m_pCtrl->RefreshCtrl();	

}

//--------------------------------------------------------------------------------------------------------------------------------
// ɨ�����
//--------------------------------------------------------------------------------------------------------------------------------
void CPeakSeak::SP_FirstScan(){
	m_Peak.clear();

	if (m_OrigSema.size()==0) return; 
	
	//step0: ��ԭ�����ƶ�ƽ��
	int moving_avg_n = 5;
	SP_MovingAverage(m_OrigSema, moving_avg_n);

	//step1: ����avgΪ1���ƽ�����ߣ�ʣ���������Ϊ1��
	std::vector<CSema> AvgData;
	if ((m_OrigSema.size()/m_Avgs)<9) {
		::AfxMessageBox( _T("ԭ���ݲ���̫�٣��޷������߽��л���"));
		return;
	}

	SP_Averaging(AvgData); 
	

	//step2: ���ƽ��������ÿһ���б��
	std::vector<double> FirstOrder;					      //б������
	double maxSlope = 0.0;								  //���������б��
	SP_GetAvgDataSlope(AvgData, FirstOrder, maxSlope);
	SP_GetMinMaxSlope(/*AvgData, */FirstOrder, 5);

	// step3: ������������˳��Ѱ��
	const unsigned AvgDataSize = AvgData.size();
	m_AnalysisConfigArg.SetPeakSearchArgs(AvgDataSize, m_Threshold);
	m_AnalysisConfigArg.SetMinusCheckArgs(AvgDataSize);
	m_AnalysisConfigArg.Analysis_PeakSearchSlopeBased(AvgData);
	m_AnalysisConfigArg.Analysis_PeakSearchMinusCheck(AvgData);

	SP_GetpeakSearch_Series_plus(AvgData, FirstOrder, 
		m_AnalysisConfigArg.GetPeakSearchArgs(), //�ֶ���������
		m_AnalysisConfigArg.GetMinusCheckArgs(), //�ֶθ�����
		m_thresholdFactor
	);

	// step4: �̶��㻹ԭ��ԭ����
	std::vector<double> SlopeBased(m_OrigSema.size(), m_Threshold);
	SP_RestorePeakData(SlopeBased);  //��ԭ��ԭ����
}
void CPeakSeak::SP_SecondScan() {
	// ��ֹ����ɾ����
	m_AnalysisConfigArg.Analysis_BanPeak(m_Peak); //�������ֲ���
	SP_BanPeak(m_AnalysisConfigArg.GetBanPeakArgs()); //��ֹ����
	SP_GetHalfPeakWidth(); //��ð���

	// ��С����ɾ����
	const unsigned PeakSize = m_Peak.size();
	m_AnalysisConfigArg.SetMinPeakWidthArgs(PeakSize, m_HalfPeakWidth);
	m_AnalysisConfigArg.Analysis_MinHalfPeakWidth(m_Peak); //�����ֶ���С�������

	SP_GetAllHPW();//���еİ���ֵ����Ա����m_cArrayHPW
	SP_EraseByHalfPeakWidth(m_AnalysisConfigArg.GetMinHalfPeakWidthArgs()); //ɾ����������������ķ�
}
void CPeakSeak::SP_ThirdScan(){

	SP_CheckOverlapPoint(); //׷�ӣ��ص�������

	// step5: �ص��崦��(�ַ�)
	vector<AnalysisConfigArguments::PeakSepaArg> dequeIdx;
	m_AnalysisConfigArg.Analysis_PeakSeparation(m_Peak, dequeIdx);
	SP_TangentDividPeak(dequeIdx); //���߷ַ壬֮����Ҫ����У��

	deque<pair<unsigned, unsigned> > seriesPeak;  //��������ţ�Ĭ��������ȫ����ֱ�ַ崦��
	SP_GetSeriesPeakDeque(seriesPeak);
	for (size_t i=0; i<seriesPeak.size(); ++i)
		SP_PeakValleyBaseLineCheck(seriesPeak[i].first, seriesPeak[i].second); //��Ȼ���У�� 
	//SP_PeakTypeChange();

	// �ų����з�����ߵ�������
	typedef deque<pair<unsigned, unsigned> >::iterator deIter;
	for (size_t i=0; i<dequeIdx.size(); ++i) {
		if (dequeIdx[i].SepaType!=1)//�ų��Ǵ�ֱ�ַ�
		{
			unsigned startPeakNo = dequeIdx[i].StartPeakNo;
			unsigned finishPeakNo = dequeIdx[i].FinishPeakNo;
			for (deIter iter=seriesPeak.begin(); iter!=seriesPeak.end(); ++iter) {
				if (((*iter).first<=startPeakNo && (*iter).second>=startPeakNo) ||
					((*iter).first<=finishPeakNo && (*iter).second>=finishPeakNo))
				{
					seriesPeak.erase(remove(seriesPeak.begin(), seriesPeak.end(), *iter), seriesPeak.end());
					break;
				}
			}
		}
	}
	     

	// �¼Ӳ��� ��ȱ�  
	bool isPeak2Valley = m_AnalysisConfigArg.IsPeak2Valley(); 
	if (isPeak2Valley) 
	{  
		double dPeak2valley_Ratio = m_AnalysisConfigArg.GetDefaultPeak2Valley(); //��÷�ȱȲ�������Ĭ��ֵ
		SP_CheckPeakToValley(dPeak2valley_Ratio, seriesPeak);
		SP_PeakTypeChange();
	}  
	else
	{
		for (size_t i=0; i<seriesPeak.size(); ++i) {
			dequeIdx.clear();
			AnalysisConfigArguments::PeakSepaArg idx(seriesPeak[i].first, seriesPeak[i].second, 1);
			dequeIdx.push_back(idx);
		}
		SP_DividePeak(dequeIdx);       //��ֱ�ַ�
	}

}
void CPeakSeak::SP_FourthScan() {
	// ����У��	
	deque<pair<unsigned, unsigned> > VertDivideArgs;  //��ֱ�ַ�
	deque<pair<unsigned, unsigned> > PeakVallyArgs;   //�������
	deque<pair<unsigned, unsigned> > PlusTangetArgs;  //���зַ�
	deque<pair<unsigned, unsigned> > MinusTangetArgs; //���зַ�

	SP_GetBaseLineArgs(CPeak::VERTDIVIDE, VertDivideArgs);
	while (!VertDivideArgs.empty())
	{
		SP_VertDivideBaseLineCheck(
			VertDivideArgs[0].first,
			VertDivideArgs[0].second); //��ֱ�ַ����У��
		VertDivideArgs.pop_front();
	}

	SP_GetBaseLineArgs(CPeak::PLUSTANGET, PlusTangetArgs);
	while (!PlusTangetArgs.empty())
	{
		SP_PlusTangetBaseLineCheck(
			PlusTangetArgs[0].first,
			PlusTangetArgs[0].second); //���зַ����У��
		PlusTangetArgs.pop_front();
	}

	SP_GetBaseLineArgs(CPeak::MINUSTANGET, MinusTangetArgs);
	while (!MinusTangetArgs.empty())
	{
		SP_MinusTangetBaseLineCheck(
			MinusTangetArgs[0].first,
			MinusTangetArgs[0].second); //���зַ����У��
		MinusTangetArgs.pop_front();
	}

	SP_GetBaseLineArgs(CPeak::PEAKVALLEY, PeakVallyArgs);
	while (!PeakVallyArgs.empty()) 
	{
		SP_PeakValleyBaseLineCheck(
			PeakVallyArgs[0].first,
			PeakVallyArgs[0].second); //��Ȼ���У��
		PeakVallyArgs.pop_front();
	}
	
	SP_PeakTypeChange();
	
}
void CPeakSeak::SP_FifthScan(){

	SP_CheckOverlapPoint(); //׷�ӣ��ص�������
	
	// 2012-08-07 ������
	// ���ִ���ʱ��Ҫ���ֹ��������������ѷ塢�ֹ����ߺ��ֹ��嶥���Ĵ���ŵ���С���������С���֮��
	SP_GetOtherPeakArgs(); //��÷��
	// �ֶ���С����� 
	SP_GetPeakArea(); //�����
	// ���������������Ӧ�ôӻ����㷨���Ƴ����ŵ������¼�֮��
	bool isSetUnit = false;

	{//���Խ��ּܳ���ʱע������������ڵĴ���
		//bool isGC1120  = ((g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC_PX) || (g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC1120))  ? true : false;
		//bool isGC2400  = (g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC2400)  ? true : false;
		//bool isGC1100w = (g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC1100W) ? true : false;
		//bool isGC1100p = (g_pcConfigIni->CurrentSystemName() == PASYSTEM_NAME_GC1100P) ? true : false;
		//isSetUnit = (isGC1120 || isGC2400 || isGC1100w || isGC1100p);
	}
	 
	m_Magnification = 1.0f;
	if (isSetUnit) 
		m_Magnification = 60000.0f;
	else
		m_Magnification = 60.0f;
	SP_SetPeakAreaUnit(m_Magnification);
	
	unsigned PeakSize = m_Peak.size();
	m_AnalysisConfigArg.SetMinPeakAreaArgs(PeakSize, m_PeakArea);
	m_AnalysisConfigArg.Analysis_MinPeakArea(m_Peak);
	SP_GetAllMPA(); //���еķ������ֵ����Ա���� m_cArrayMPA
	SP_EraseByPeakArea(m_AnalysisConfigArg.GetMinPeakAreaArgs());
	

	// �ֶ���С���
	PeakSize = m_Peak.size();
	m_AnalysisConfigArg.SetMinPeakHeightArgs(PeakSize, m_PeakHeight);
	m_AnalysisConfigArg.Analysis_MinPeakHeight(m_Peak);
	SP_GetAllMH();
	SP_EraseByPeakHeight(m_AnalysisConfigArg.GetMinPeakHeightArgs());
	

	// �ϲ��� �� �ֹ����� ����˳���û�����¼�˳��
	deque<ConfigArgument> allConfigArgument = m_AnalysisConfigArg.GetAllArguments();
	const int configArgSize = allConfigArgument.size();
	for (int i=0; i<configArgSize; i++)
	{
		if (allConfigArgument[i].ArgName == _T("�ϲ���"))
		{
			//�ϲ���
			m_AnalysisConfigArg.Analysis_MergePeak(m_Peak);
			SP_MergePeak(m_AnalysisConfigArg.GetMergePeakNo());
			SP_PeakTypeChange(); //�ϲ����ķ����͵���
		}

		else if (allConfigArgument[i].ArgName == _T("�ֹ��ַ�"))
		{
			//�ֹ��ַ� => ���ѷ�
			deque<double> handPeakSera;
			m_AnalysisConfigArg.Analysis_HandPeakSeparation(m_Peak, handPeakSera);
			
			while (!handPeakSera.empty()) //�õ���handPeakSeraδ����
			{
				int nfindIndex = m_AnalysisConfigArg.Analysis_OneHandPeakSepar(m_Peak, handPeakSera[0]);
				if (nfindIndex != -1) //�ҵ��������ֹ��ַ�ķ��±�
				{
					SP_SplitPeak(nfindIndex, handPeakSera[0]);
					SP_PeakTypeChange(); //�ֹ��ַ��Է��ŵĵ���
				}
				handPeakSera.pop_front();
			}
		}

		else if (allConfigArgument[i].ArgName == _T("�ֹ�����"))
		{
			//�ֹ��ⶥ
			m_AnalysisConfigArg.Analysis_HandAdjustPeak(m_Peak);
			SP_HandAdjustPeak(m_AnalysisConfigArg.GetHandAdjustPeakArgs());  //�������

			//�ֹ�����
			m_AnalysisConfigArg.Analysis_HandworkBaseLine(m_Peak);
			SP_HandAdjustStart(m_AnalysisConfigArg.GetHandAdjustStartArgs());//�������
			SP_HandAdjustFinish(m_AnalysisConfigArg.GetHandAdjustFinishArgs());//�����յ�
		}
	}

}
void CPeakSeak::SP_SixthScan(){
	// step7:��������
	// ��������������������ӣ���β���ӣ���ߣ����
	SP_GetOtherPeakArgs();

	// �ټ���һ�� �ֶ���С����� 
	SP_GetPeakArea(); //�����
	// ���������������Ӧ�ôӻ����㷨���Ƴ����ŵ������¼�֮��
	SP_SetPeakAreaUnit(m_Magnification);


	// ���ӷ� �� ɾ���� ��Ҫ���û���ӻ����¼���˳����е���
	typedef void (CPeakSeak::*pFuncPointer)(ConfigArgument &);
	pFuncPointer fp_addpeak = &CPeakSeak::SP_AddPeak_One; 
	pFuncPointer fp_delpeak = &CPeakSeak::SP_DelPeak;

	m_AnalysisConfigArg.Analysis_AddelPeak(m_Peak);
	deque<ConfigArgument> AddDelPeakArgs = m_AnalysisConfigArg.GetAddDelPeakArgs();
	while (!AddDelPeakArgs.empty())
	{	
		ConfigArgument arg = AddDelPeakArgs[0];  
		if (arg.ArgName==_T("���ӷ�"))	(this->*fp_addpeak)(arg);
		if (arg.ArgName==_T("ɾ����"))	(this->*fp_delpeak)(arg);
		AddDelPeakArgs.pop_front();
	}

	// ���İٷֱȣ���ߣ��������
	SP_GetPeakPercent();

	// �жϷ�ľ�������
	SP_CheckPeakType(); //�ȼ�����
	SP_SetPeakType();

	// ��������
	SP_CalcResolution();

	// ���洹ֱ�ַ�����
//	SP_GetDividePeakLine();
}

//--------------------------------------------------------------------------------------------------------------------------------
// ��ȫ��CA_ �����Ƶ�CPeakSeak������                                                                                               
//--------------------------------------------------------------------------------------------------------------------------------
//��1��ɨ��
void CPeakSeak::SP_MovingAverage(vector<CSema> &OrigData, int N=5){
	m_OrigSema_backup = OrigData; //??? TODO

	int OrigDataSize = OrigData.size();
	if (OrigDataSize < N) return;
	
	int i=0;
	typedef vector<CSema>::iterator ITER;
	ITER flag_iter;
	for (ITER iter=OrigData.begin(); iter!=OrigData.end(); iter++)
	{
		if (i == OrigDataSize - N)
			flag_iter = iter;

		if (i >= OrigDataSize - N)
		{
			(*iter).x = (*flag_iter).x;
			(*iter).y = (*flag_iter).y;
			continue;
		}

		for (int j=1; j<N; j++)
		{
			ITER next_iter = iter;
			int next = 0;
			while (next!=j)
			{
				next_iter++;
				next++;
			}
			(*iter).y = (*iter).y + (*next_iter).y;
		}
		(*iter).y = (*iter).y / N;
		i++;
	}



	//for (int i=0; i<OrigDataSize - N; i++)
	//{
	//	for (int j=1; j<N; j++)
	//	{
	//		OrigData[i].y = OrigData[i].y + OrigData[i+j].y;
	//	}
	//	OrigData[i].y = OrigData[i].y / N;
	//}
	//for (int i=OrigDataSize-N; i<OrigDataSize; i++)
	//{
	//	OrigData[i].x = OrigData[OrigDataSize-N].x;
	//	OrigData[i].y = OrigData[OrigDataSize-N].y;
	//}
}
void CPeakSeak::SP_Averaging(std::vector<CSema> &avgData) { 
	if (m_Avgs==0) 
	{
		avgData = m_OrigSema;
		return;
	}

	const unsigned handleSemaSize = m_OrigSema.size();
	for (unsigned i=0; i<handleSemaSize; ++i)
	{
		double avgTime = 0.0;
		double avgSema = 0.0;
		if (i % m_Avgs == 0 && i+m_Avgs < handleSemaSize) 
		{
			for (unsigned j=i; j< i+m_Avgs; j++)
			{
				avgTime += m_OrigSema[j].x;
				avgSema += m_OrigSema[j].y;
			}
			avgTime = avgTime / m_Avgs;
			avgSema = avgSema / m_Avgs;
			CSema OneSema(i, avgTime, avgSema);
			avgData.push_back(OneSema);
		}
		if ((handleSemaSize-m_Avgs*avgData.size())/m_Avgs<1) //ʣ��
		{
			for (unsigned j=m_Avgs*avgData.size(); j<handleSemaSize; ++j)
			{
				avgTime += m_OrigSema[j].x;    
				avgSema += m_OrigSema[j].y;  
			}
			avgTime = avgTime/(handleSemaSize-m_Avgs*avgData.size()+1);
			avgSema = avgSema/(handleSemaSize-m_Avgs*avgData.size()+1);
			CSema OneSema(m_Avgs*avgData.size(), avgTime, avgSema);
			avgData.push_back(OneSema);
			break;
		}
	}
}
void CPeakSeak::SP_GetAvgDataSlope(const vector<CSema> &avgData, vector<double> &firstOrder, double &maxSlope)					//���б��            
{//step2: ���ƽ��������ÿһ���б��
	//�������N�����ݣ�����������N-1�����
	//���һ������û������б��

	const unsigned avgDataSize = avgData.size()-1;
	double *k = new double[avgDataSize];
	firstOrder.resize(avgDataSize);

	typedef vector<double>::iterator ITER_D;
	typedef vector<CSema>::const_iterator ITER_C;
	ITER_C av_iter = avgData.begin();

	int i=0;
	for (ITER_D iter=firstOrder.begin(); iter!=firstOrder.end(); iter++)
	{
		ITER_C next_av_iter = av_iter;
		next_av_iter++;

		double ydiff = (*next_av_iter).y - (*av_iter).y;
		double xdiff = (*next_av_iter).x - (*av_iter).x;
		k[i] = (ydiff) / (xdiff);
		if ( k[i] >= maxSlope ) maxSlope = k[i];
		(*iter) = k[i];

		av_iter = next_av_iter;
		i++;
	}

	delete [] k;
	k = NULL;

	//const unsigned avgDataSize = avgData.size()-1;
	//double *k = new double[avgDataSize];
	//for (unsigned i=0; i<avgDataSize; ++i)
	//{	
	//	double ydiff = avgData[i+1].y - avgData[i].y;
	//	double xdiff = avgData[i+1].x - avgData[i].x;
	//	k[i] = (ydiff) / (xdiff);
	//	if (k[i]>=maxSlope)
	//		maxSlope = k[i];
	//	firstOrder.push_back(k[i]);
	//}
	//delete [] k;
	//k = NULL;
}
void CPeakSeak::SP_GetMinMaxSlope(/*const vector<CSema> &avgData,*/ const vector<double> &firstOrder,int N)
{
	if (N<1 || static_cast<size_t>(N)>firstOrder.size())  return;

	//��ʼ��б����ֵ
	m_dMinSlope = (double)INT_MAX;
	m_dMaxSlope = 0.0;

	//б�ʱȽϻ���
	deque<double> tmpSlope;

	typedef vector<double>::const_iterator ITER;
	for (ITER iter=firstOrder.begin(); iter!=firstOrder.end(); iter++)
	{
		for (int j=0; j<N; j++)
		{
			if (*iter <= 0.0f) 
			{
				continue;
			}
			tmpSlope.push_back(*iter);
		}

		if (tmpSlope.empty())
		{
			continue;
		}

		double maxS = tmpSlope[0];
		double minS = tmpSlope[0];
		while (!tmpSlope.empty()) 
		{
			if (tmpSlope[0]>maxS) maxS = tmpSlope[0];
			if (tmpSlope[0]<minS) minS = tmpSlope[0];
			tmpSlope.pop_front();
		}
		if(maxS < m_dMinSlope) m_dMinSlope = maxS;
		if(minS > m_dMaxSlope) m_dMaxSlope = minS;
	}


	//for (size_t i=0; i<firstOrder.size(); i+=N)
	//{
	//	for (int j=0; j<N; ++j)
	//	{
	//		if (firstOrder[i]<=0) //С�ڵ���0��б�ʲ�Ҫ
	//			continue;
	//		tmpSlope.push_back(firstOrder[i]);
	//	}
	//	if (tmpSlope.empty()) continue;
	//	double maxS = tmpSlope[0];
	//	double minS = tmpSlope[0];
	//	while (!tmpSlope.empty()) //�Ƚϻ��棬�õ����ֵ����Сֵ
	//	{
	//		if (tmpSlope[0]>maxS) maxS = tmpSlope[0];
	//		if (tmpSlope[0]<minS) minS = tmpSlope[0];
	//		tmpSlope.pop_front();
	//	}

	//	if(maxS < m_dMinSlope) m_dMinSlope = maxS;
	//	if(minS > m_dMaxSlope) m_dMaxSlope = minS;
	//}
}

void CPeakSeak::SP_GetpeakSearch_Series(const vector<CSema> &avgData,   const vector<double> &firstOrder,vector<double> &slopeBased, deque<bool> &hasNagetiveSpike)   
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
			for ( ; i<avgDataSize-m_Series && hState==STATELESS; ++i)  
			{
				if (i>=avgDataSize-m_Series-1) 
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
							for (int s=1; s<=m_Series; ++s)
							{
								if (fabs(firstOrder[i+s])<slopeBased[i])
									break;
								if (fabs(firstOrder[i+s])>slopeBased[i])
								{
									if (s==m_Series)
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
						for (int s=1; s<=m_Series; ++s)
						{
							if (fabs(firstOrder[i+s])<slopeBased[i+s])
								break;
							if (fabs(firstOrder[i+s])>=slopeBased[i+s])
							{
								if (s==m_Series)
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
					if (j>=avgDataSize-m_Series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}

					if (firstOrder[j]<0)
						continue;
					if (firstOrder[j]>=0)
					{
						for (int s=1; s<=m_Series; ++s)
						{
							if (firstOrder[j+s]>0)//
								break;
							if (firstOrder[j+s]<0)
							{
								if (s==m_Series)
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
					if (j>=avgDataSize-m_Series-1)
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
							for (int s=1; s<=m_Series; ++s)
							{
								if (firstOrder[j+s]>slopeBased[j+s])
									break;
								if (firstOrder[j+s]<slopeBased[j+s]  && (firstOrder[j+s]!=0.000000))
								{
									if (s==m_Series)
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
					m_Peak.push_back(OnePeak); //����

					unsigned t = m_Peak.size()-1;
					if (t>0 && (m_Peak[t].StartSema.x-m_Peak[t-1].FinSema.x)==0.000000)
					{
						m_Peak[t-1].Type = -mark;// ֮ǰ�ı�־��Ϊ��������
						m_Peak[t].Type = -mark;  //��������
					}
					else
					{
						m_Peak[t].Type = -1; //��������
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
					if (k>=avgDataSize-m_Series-1)
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
							for (int s=1; s<=m_Series; ++s)
							{
								if (fabs(firstOrder[k+s])>slopeBased[k+s])
									break;
								if (fabs(firstOrder[k+s])<slopeBased[k+s] && (firstOrder[k+s]!=0.000000))
								{
									if (s==m_Series)
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
					m_Peak.push_back(OnePeak);  //����

					unsigned t = m_Peak.size()-1;

					if (t>0 && (m_Peak[t].StartSema.x-m_Peak[t-1].FinSema.x)==0.000000)
					{
						m_Peak[t-1].Type = mark;	// ֮ǰ�ı�־��Ϊ��������
						m_Peak[t].Type = mark;	//��������
					}
					else
					{
						m_Peak[t].Type = 1; //��������
						mark++;
					}
				}
			}
			else
			{//���岨��
				for (k=i+1; k<avgDataSize-1 && hState==DOWNGOING_WAVE; ++k)
				{
					if (k>=avgDataSize-m_Series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[k]>0)
						continue;

					if (firstOrder[k]<=0)
					{
						for (int s=1; s<=m_Series; ++s)
						{
							if (firstOrder[k+s]<0)
								break;
							if (firstOrder[k+s]>0)
							{
								if (s==m_Series)
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

// ��ǿ�棺��ƽͷ��Ĵ���
void CPeakSeak::SP_GetpeakSearch_Series_plus(const vector<CSema> &avgData, const vector<double> &firstOrder,vector<double> &slopeBased, deque<bool> &hasNagetiveSpike, double thresholdFactor)   
{// step3: ������������˳��Ѱ��

	enum enumState {
		STATELESS,			//��״̬
		UPCOMING_WAVE,		//���в�
		DOWNGOING_WAVE,		//���в�
		HANDLE_ACCOMPLISH   //�������
	} hState;				//��������еı��

	const double threshold_factor = thresholdFactor; //1.0f; //���в�����б�ʳ˻�����
	const unsigned series_factor = 0;

	const unsigned avgDataSize = avgData.size();
	unsigned i=0, j=0, k=0;
	unsigned getStartPoint=0, getpeakPoint=0, getFinishPoint=0;
	int mark = 1; //�����ͱ��: 1=����; -1=����; 2,3,4,...Ϊ�ص�����; -2,-3,-4...Ϊ�ص�����
	bool isNagetiveSpike = false;
	bool processed = false;
	bool isTwiceCheck = false;
	hState = STATELESS;
	while (hState != HANDLE_ACCOMPLISH)
	{
		switch (hState)
		{
		case STATELESS:  //��״̬
			for ( ; i<avgDataSize-m_Series && hState==STATELESS; ++i)  
			{
				if (i>=avgDataSize-m_Series-1) 
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
							for (int s=1; s<=m_Series; ++s)
							{
								if (fabs(firstOrder[i+s])<slopeBased[i])
									break;
								if (fabs(firstOrder[i+s])>slopeBased[i])
								{
									if (s==m_Series)
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
						for (int s=1; s<=m_Series; ++s)
						{
							if (fabs(firstOrder[i+s])<slopeBased[i+s])
								break;
							if (fabs(firstOrder[i+s])>=slopeBased[i+s])
							{
								if (s==m_Series)
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
					if (j>=avgDataSize-m_Series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}

					if (firstOrder[j]<0)
						continue;
					if (firstOrder[j]>=0)
					{
						for (int s=1; s<=m_Series; ++s)
						{
							if (firstOrder[j+s]>0)
								break;
							if (firstOrder[j+s]<0)
							{
								if (s==m_Series)
								{
									getpeakPoint = j+s;
									isTwiceCheck = true; // ��ƽͷ������жϿ���
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
					if (j>=avgDataSize-m_Series-1)
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
							for (int s=1; s<=m_Series; ++s)
							{
								if (firstOrder[j+s]>slopeBased[j+s])
									break;
								if (firstOrder[j+s]<slopeBased[j+s]  && (firstOrder[j+s]!=0.000000))
								{
									if (s==m_Series)
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
					m_Peak.push_back(OnePeak); //����

					unsigned t = m_Peak.size()-1;
					if (t>0 && (m_Peak[t].StartSema.x-m_Peak[t-1].FinSema.x)==0.000000)
					{
						m_Peak[t-1].Type = -mark;// ֮ǰ�ı�־��Ϊ��������
						m_Peak[t].Type = -mark;  //��������
					}
					else
					{
						m_Peak[t].Type = -1; //��������
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
					if (k>=avgDataSize-m_Series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}

					if ( firstOrder[k]<0 ) 
					{
						if (!processed) // ����ƽͷ���жϿ���
						{
							if (fabs(firstOrder[k])>slopeBased[k])  //2010.08.24 ���ƽͷ�崦������
							{
								for (unsigned s=k; s<=k+m_Series && s<avgDataSize; ++s)
								{
									if (fabs(firstOrder[k]) < slopeBased[k])
									{
										processed = false;
										break;
									}
									else
									{
										processed = true;
									}
								}
								//continue;
							}
						}

						if (processed && (fabs(firstOrder[k]) > slopeBased[k]) && (firstOrder[k]!=0.000000)) // ֮ǰ<
						{
							int new_series = m_Series+series_factor;
							for (int s=1; s<=new_series; ++s)
							{
								double new_slopeBased = slopeBased[k+s] * threshold_factor;
								if (fabs(firstOrder[k+s])>new_slopeBased)
									break;
								if (fabs(firstOrder[k+s])<new_slopeBased && (firstOrder[k+s]!=0.000000))
								{
									if (s==m_Series)
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
					if (processed && firstOrder[k]>0 )
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
					m_Peak.push_back(OnePeak);  //����

					processed = false;

					unsigned t = m_Peak.size()-1;

					if (t>0 && (m_Peak[t].StartSema.x-m_Peak[t-1].FinSema.x)==0.000000)
					{
						m_Peak[t-1].Type = mark;	// ֮ǰ�ı�־��Ϊ��������
						m_Peak[t].Type = mark;	//��������
					}
					else
					{
						m_Peak[t].Type = 1; //��������
						mark++;
					}
				}
			}
			else
			{//���岨��
				for (k=i+1; k<avgDataSize-1 && hState==DOWNGOING_WAVE; ++k)
				{
					if (k>=avgDataSize-m_Series-1)
					{
						hState = HANDLE_ACCOMPLISH;
						break;
					}
					if (firstOrder[k]>0)
						continue;

					if (firstOrder[k]<=0)
					{
						for (int s=1; s<=m_Series; ++s)
						{
							if (firstOrder[k+s]<0)
								break;
							if (firstOrder[k+s]>0)
							{
								if (s==m_Series)
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

void CPeakSeak::SP_RestorePeakData(const vector<double> &slopeBased) {
	// step4: ������ĵ㻹ԭ��Ԫ����
	const unsigned handleSemaSize = m_OrigSema.size();
	const unsigned peakSemaSize = m_Peak.size();

	for (unsigned i=0; i<peakSemaSize; ++i)  //����ÿһ����
	{
		for (unsigned j=m_Peak[i].StartSema.idx; j<m_Peak[i].StartSema.idx+m_Avgs && j<handleSemaSize-1; j++) //�Ȼ�ԭ���
		{
			double k=(m_OrigSema[j+1].y-m_OrigSema[j].y)/(m_OrigSema[j+1].x-m_OrigSema[j].x);
			if (fabs(k)>slopeBased[j])
			{
				if (j==m_Peak[i].StartSema.idx+m_Avgs-1)
				{
					m_Peak[i].StartSema = m_OrigSema[j];
					break;
				}
			}
			else
			{
				m_Peak[i].StartSema = m_OrigSema[j];
				break;
			}
		}

		for (unsigned j=m_Peak[i].FinSema.idx; j<m_Peak[i].FinSema.idx+m_Avgs && j<handleSemaSize-1; j++) //�ٻ�ԭ���
		{
			double k=(m_OrigSema[j+1].y-m_OrigSema[j].y)/(m_OrigSema[j+1].x-m_OrigSema[j].x);
			if (fabs(k)>slopeBased[j])
			{
				if (j==m_Peak[i].FinSema.idx+m_Avgs-1)
				{
					m_Peak[i].FinSema = m_OrigSema[j];
					break;
				}
			}
			else
			{
				m_Peak[i].FinSema = m_OrigSema[j];
				break;
			}
		}

		//���ԭ������߲��ȵ�
		double maxValue = -INT_MAX;
		double minValue = INT_MAX;
		if (m_Peak[i].Type>=1) //��ԭ����
		{
			for (unsigned j=m_Peak[i].StartSema.idx; j<m_Peak[i].FinSema.idx; ++j)
			{
				if (maxValue <= m_OrigSema[j].y)
				{
					maxValue = m_OrigSema[j].y;
					m_Peak[i].PeakSema = m_OrigSema[j];
				}
			}
		}
		else //��ԭ����
		{
			for (unsigned j=m_Peak[i].StartSema.idx; j<m_Peak[i].FinSema.idx; ++j)
			{
				if (minValue >= m_OrigSema[j].y)
				{
					minValue = m_OrigSema[j].y;
					m_Peak[i].PeakSema = m_OrigSema[j];	
				}
			}
		}
	}
}
// ��2��ɨ��
void CPeakSeak::SP_BanPeak(vector<unsigned> BanPeakNo)
{
	vector<CPeak> buffPeak;
	const unsigned PeakSize = m_Peak.size();
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
			buffPeak.push_back(m_Peak[i]); 
		}
	}
	const unsigned buffPeakSize = buffPeak.size();
	if (buffPeakSize!=PeakSize) //������ɾ����
	{
		m_Peak.swap(buffPeak);
		SP_PeakTypeChange(); //ɾ���ķ����͵���
	}
}
void CPeakSeak::SP_PeakTypeChange() {
	const unsigned PeakSize = m_Peak.size();
	if (PeakSize==0) return;
	for (unsigned i=0; i<PeakSize-1; ++i)
	{
		if (  (m_Peak[i].dType == CPeak::PLUSTANGET)
			||(m_Peak[i].dType == CPeak::MINUSTANGET))
		{
			continue;
		}

		if (m_Peak[i].dType == CPeak::PEAKVALLEY)
		{
			m_Peak[i].Type = m_Peak[i].Type/abs(m_Peak[i].Type);
		}

		if (abs(m_Peak[i].Type)!=1) 
		{
			if (m_Peak[i].FinSema==m_Peak[i+1].StartSema)
			{
				continue;
			}
			else /*if (m_Peak[i].FinSema!=m_Peak[i+1].StartSema)*/
			{
				if (i!=0) 
				{  // ���ǵ�һ����
					if (m_Peak[i].StartSema==m_Peak[i-1].FinSema)
						continue;
			
					if (m_Peak[i].Type==0)
						continue;

					m_Peak[i].Type  = m_Peak[i].Type/abs(m_Peak[i].Type);
					m_Peak[i].dType = CPeak::PEAKVALLEY;
				}
				else
				{
					if (abs(m_Peak[i].Type)!=1)
					{
						m_Peak[i].Type = m_Peak[i].Type/abs(m_Peak[i].Type);
						m_Peak[i].dType = CPeak::PEAKVALLEY;
					}
				}
			}
		}
		else //���壬�Ѵ�ֱ�ַ�ĳɷ������
		{
			m_Peak[i].dType = CPeak::PEAKVALLEY;
		}
	}
	// ���һ��������
	if (PeakSize>=2)
	{
		if (  (m_Peak[PeakSize-1].dType == CPeak::PLUSTANGET)
			||(m_Peak[PeakSize-1].dType == CPeak::MINUSTANGET))
		{
			return;
		}

		if (m_Peak[PeakSize-1].StartSema != m_Peak[PeakSize-2].FinSema)
		{
			m_Peak[PeakSize-1].Type = m_Peak[PeakSize-1].Type/abs(m_Peak[PeakSize-1].Type);
			m_Peak[PeakSize-1].dType = CPeak::PEAKVALLEY;
		}
	}

	// ���ǵ��壬����յ������ԭ������ (��Ȼ�޷��������߷ַ��Ļ���У������
	for (unsigned i=0; i<PeakSize; ++i){
		if (abs(m_Peak[i].Type)==1){
			unsigned start_idx = m_Peak[i].StartSema.idx;
			m_Peak[i].StartSema = m_OrigSema[start_idx];

			unsigned end_idx = m_Peak[i].FinSema.idx;
			m_Peak[i].FinSema = m_OrigSema[end_idx];
		}

	}

	// ���ǵ������߷ַ壬У��Ϊ��Ȼ���
	for (unsigned i=0; i<PeakSize; ++i){
		if (m_Peak[i].dType == CPeak::VERTDIVIDE){
			int peakType = m_Peak[i].Type;
			int peakType_sum = 0;
			for (unsigned j=0; j<PeakSize; ++j){
				if (m_Peak[j].Type == peakType)
					peakType_sum += peakType;
			}
			int peak_amount = peakType_sum / peakType;
			if (peak_amount == 1)
			{
				m_Peak[i].dType = CPeak::PEAKVALLEY;
				m_Peak[i].Type = abs(m_Peak[i].Type) / m_Peak[i].Type;

				unsigned start_idx = m_Peak[i].StartSema.idx;
				m_Peak[i].StartSema = m_OrigSema[start_idx];

				unsigned end_idx = m_Peak[i].FinSema.idx;
				m_Peak[i].FinSema = m_OrigSema[end_idx];

			}
		}
	}
}
void CPeakSeak::SP_GetHalfPeakWidth() {

	const unsigned PeakSize = m_Peak.size();
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = m_Peak[i].StartSema.x;
		double y1 = m_Peak[i].StartSema.y;
		double x2 = m_Peak[i].FinSema.x;
		double y2 = m_Peak[i].FinSema.y;

		double peakWidth = fabs(x2-x1);

		double tana = (y2-y1)/(x2-x1);
		double c = y1 - x1 * tana;

		double py = m_Peak[i].PeakSema.y; 
		double px = m_Peak[i].PeakSema.x;
		double ty = px*tana+c;

		double hx = px;
		double hy = (py-ty)/2 + ty;
		double _c = hy-hx*tana;		

		double diff_y = INT_MAX;
		unsigned idxl=0, idxr=0;

		for (unsigned l=m_Peak[i].StartSema.idx; l<=m_Peak[i].PeakSema.idx; ++l) //���У��ƽ��е�
		{
			double ly = m_OrigSema[l].x * tana + _c;
			if (diff_y >= fabs(m_OrigSema[l].y - ly))
			{
				diff_y = fabs(m_OrigSema[l].y - ly);
				idxl = l;
			}
		}
		CSema tmpFront(m_OrigSema[idxl]); //������
		m_Peak[i].MidStartSema = tmpFront;

		diff_y = INT_MAX;

		for (unsigned r=m_Peak[i].PeakSema.idx; r<=m_Peak[i].FinSema.idx; ++r) //���У��ƽ��е�
		{
			double ry = m_OrigSema[r].x * tana + _c;

			if (diff_y >= fabs(m_OrigSema[r].y - ry))
			{
				diff_y = fabs(m_OrigSema[r].y - ry);
				idxr = r;
			}
		}
		CSema tmpLast(m_OrigSema[idxr]);   //������
		m_Peak[i].MidFinSema = tmpLast;

		double hk = (m_Peak[i].MidFinSema.y-m_Peak[i].MidStartSema.y)/(m_Peak[i].MidFinSema.x-m_Peak[i].MidStartSema.x);
		double hc = m_Peak[i].MidStartSema.y - m_Peak[i].MidStartSema.x * hk;
		double rey = hx*hk+hc;
		CSema HalfPeak(m_Peak[i].PeakSema.idx, hx, rey); // ����е�
		m_Peak[i].MidPeakSema = HalfPeak;
	}

	//���Ӱ���ֱ�ߣ��벨��Ĵ����ཻ��һ�㡣���������ǰ�󳤶�
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = m_Peak[i].MidStartSema.x; 
		double y1 = m_Peak[i].MidStartSema.y;
		double x2 = m_Peak[i].MidFinSema.x; 
		double y2 = m_Peak[i].MidFinSema.y;

		double halfPeakWidth = fabs(x2-x1);
		m_Peak[i].HalfPeakWidth = halfPeakWidth;  //�����
	}
}
void CPeakSeak::SP_GetAllHPW()
{//ɾ��֮ǰ�����еİ���ֵ����Ա����m_cArrayHPW

	if (m_cArrayHPW.GetSize()!=0)
		m_cArrayHPW.RemoveAll();

	const size_t PeakSize = m_Peak.size();
	for (size_t i=0; i<PeakSize; ++i)
	{
		m_cArrayHPW.Add(m_Peak[i].HalfPeakWidth);
	}
}
void CPeakSeak::SP_EraseByHalfPeakWidth(vector<double> &MinHalfWidth)
{
	const unsigned PeakSize = m_Peak.size();
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

		if (abs(m_Peak[i].Type)==1) //����
		{
			if (m_Peak[i].HalfPeakWidth - MinHalfWidth[i] >= 0.000000)//������С����
			{
				buffPeak.push_back(m_Peak[i]);
			}
		}
		else //������
		{
			lastType = m_Peak[i].Type; //��¼��������ʼ����
			while ( lastType == m_Peak[i].Type && i<PeakSize) //ֱ�����������
			{
				if (m_Peak[i].HalfPeakWidth - MinHalfWidth[i] >= 0.000000) //������С����
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
					buffPeak.push_back(m_Peak[j]);
				}
				flag = !flag;
			}
			i = FinishMarkPoint;  //���¶�λi���±�
		}
	}

	const unsigned buffPeakSize = buffPeak.size();
	m_MinHalfPeakWidthEarseNum = PeakSize - buffPeakSize;
	if (m_MinHalfPeakWidthEarseNum!=0) //������ɾ����
	{
		m_Peak.swap(buffPeak);
		SP_PeakTypeChange(); //ɾ���ķ����͵���
	}
}
//��3��ɨ�� 
void CPeakSeak::SP_TangentDividPeak(vector<AnalysisConfigArguments::PeakSepaArg> &PeakSepaArgs)
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
				unsigned sp = m_Peak[start].StartSema.idx;
				unsigned ep = m_Peak[fin].FinSema.idx;
				double _k = (m_OrigSema[ep].y-m_OrigSema[sp].y)/(m_OrigSema[ep].x-m_OrigSema[sp].x);
				double _c = m_OrigSema[sp].y- _k*m_OrigSema[sp].x;
				for (unsigned i=m_Peak[start].PeakSema.idx; i<=m_Peak[fin].PeakSema.idx; ++i)
				{
					double _y = _k * m_OrigSema[i].x + _c;
					if (m_Peak[start].Type < 0) //���Ǹ������
					{
						if ((fabs(_y) < fabs(m_OrigSema[i].y) && _y>0)
							||(fabs(_y) > fabs(m_OrigSema[i].y) && _y>=0))
						{
							start++;
							break;
						}
					}
					else
					{
						if ((fabs(_y) > fabs(m_OrigSema[i].y) && _y>0)//������������������ڻ�����
							||(fabs(_y) < fabs(m_OrigSema[i].y) && _y<=0))
						{
							start++;
							break;
						}
					}
				}
			}

			m_Peak[start].FinSema = m_Peak[fin].FinSema;
			//��Ƿַ�����//
			for (unsigned i=start; i<=fin; ++i){
				if(start==fin) return; //���巵��
	
				m_Peak[i].dType = CPeak::PLUSTANGET;
				m_Peak[i].Type =  abs(m_Peak[i].Type) / m_Peak[i].Type;
			}
			

			for ( ; start!=fin+1; ++start)
			{

				//У�����������
				unsigned s = m_Peak[start].StartSema.idx;
				unsigned f = m_Peak[fin].FinSema.idx;
				//���У��
				bool isfail = true;
				for (; s<=m_Peak[start].PeakSema.idx && isfail; ++s)
				{
					double k = (m_OrigSema[f].y-m_OrigSema[s].y)/(m_OrigSema[f].x-m_OrigSema[s].x);
					double c = m_OrigSema[s].y-k*m_OrigSema[s].x;
					for (unsigned m=s; m<=m_Peak[start].PeakSema.idx; ++m) 
					{
						double _y = k * m_OrigSema[m].x+c;
						if (m_Peak[start].Type < 0) //���Ǹ������
						{
							if (fabs(_y) > fabs(m_OrigSema[m].y))
							{
								isfail = true;
								break;
							}
						}
						else
						{
							if (fabs(_y) > fabs(m_OrigSema[m].y) && _y>0)
							{
								isfail = true;
								break;
							}
							else if(fabs(_y) < fabs(m_OrigSema[m].y) && _y<=0)
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
						m_Peak[start].StartSema = m_OrigSema[s];
						isfail = false;
					}
				}isfail = true;	//�յ�У��
				for (; f>=m_Peak[fin].PeakSema.idx && isfail; --f)
				{
					double k = (m_OrigSema[f].y-m_OrigSema[s].y)/(m_OrigSema[f].x-m_OrigSema[s].x);
					double c = m_OrigSema[f].y-k*m_OrigSema[f].x;
					for (unsigned n=f; n>=m_Peak[fin].PeakSema.idx; --n)
					{
						double _y = k * m_OrigSema[n].x + c;
						if (m_Peak[start].Type < 0) //���Ǹ������
						{
							if (fabs(_y) > fabs(m_OrigSema[n].y))
							{
								isfail = true;
								break;
							}
						}
						else
						{
							if (fabs(_y) > fabs(m_OrigSema[n].y) && _y>0)
							{
								isfail = true;
								break;
							}
							else if(fabs(_y) < fabs(m_OrigSema[n].y) && _y<=0)
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
						m_Peak[fin].FinSema = m_OrigSema[f];
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
				unsigned sp = m_Peak[start].FinSema.idx;
				unsigned ep = m_Peak[fin].StartSema.idx;
				double _k = (m_OrigSema[ep].y-m_OrigSema[sp].y)/(m_OrigSema[ep].x-m_OrigSema[sp].x);
				double _c = m_OrigSema[sp].y- _k*m_OrigSema[sp].x;
				for (unsigned i=m_Peak[start].PeakSema.idx; i>=m_Peak[fin].PeakSema.idx; --i)
				{
					double _y = _k * m_OrigSema[i].x + _c;
					if (m_Peak[start].Type < 0) //���Ǹ������
					{
						if ((fabs(_y) < fabs(m_OrigSema[i].y) && _y>0)
							||(fabs(_y) > fabs(m_OrigSema[i].y) && _y>=0))
						{
							start++;
							break;
						}
					}
					else
					{
						if ((fabs(_y) > fabs(m_OrigSema[i].y) && _y>0)//������������������ڻ�����
							||(fabs(_y) < fabs(m_OrigSema[i].y) && _y<=0))
						{
							start--;
							break;
						}
					}
				}
			}

			m_Peak[start].StartSema = m_Peak[fin].StartSema;
			//��Ƿַ�����//
			for (int i=start; i>=fin; i--){
				if(start==fin) return; //���巵��
				if (i<0) break;
				m_Peak[i].dType = CPeak::MINUSTANGET;
				m_Peak[i].Type =  abs(m_Peak[i].Type) / m_Peak[i].Type;
			}
			

			for ( ; start!=fin-1; --start)
			{
				if (start<0) continue;
				
				//У�����������
				unsigned s = m_Peak[start].FinSema.idx;
				unsigned f = m_Peak[fin].StartSema.idx;
				//���У��
				bool isfail = true;
				for (; s>=m_Peak[start].PeakSema.idx && isfail; --s)
				{
					double k = (m_OrigSema[f].y-m_OrigSema[s].y)/(m_OrigSema[f].x-m_OrigSema[s].x);
					double c = m_OrigSema[s].y-k*m_OrigSema[s].x;
					for (unsigned m=s; m>=m_Peak[start].PeakSema.idx; --m) 
					{
						double _y = k * m_OrigSema[m].x+c;
						if (m_Peak[start].Type < 0) //���Ǹ������
						{
							if (fabs(_y) > fabs(m_OrigSema[m].y))
							{
								isfail = true;
								break;
							}
						}
						else
						{
							if (fabs(_y) > fabs(m_OrigSema[m].y) && _y>0)
							{
								isfail = true;
								break;
							}
							else if(fabs(_y) < fabs(m_OrigSema[m].y) && _y<=0)
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
						m_Peak[start].FinSema = m_OrigSema[s];
						isfail = false;
					}
				}isfail = true;	//�յ�У��
				for (; f<=m_Peak[fin].PeakSema.idx && isfail; ++f)
				{
					double k = (m_OrigSema[f].y-m_OrigSema[s].y)/(m_OrigSema[f].x-m_OrigSema[s].x);
					double c = m_OrigSema[f].y-k*m_OrigSema[f].x;
					for (unsigned n=f; n<=m_Peak[fin].PeakSema.idx; ++n)
					{
						double _y = k * m_OrigSema[n].x + c;
						if (m_Peak[start].Type < 0) //���Ǹ������
						{
							if (fabs(_y) > fabs(m_OrigSema[n].y))
							{
								isfail = true;
								break;
							}
						}
						else
						{
							if (fabs(_y) > fabs(m_OrigSema[n].y) && _y>0)
							{
								isfail = true;
								break;
							}
							else if(fabs(_y) < fabs(m_OrigSema[n].y) && _y<=0)
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
						m_Peak[fin].StartSema = m_OrigSema[f];
						isfail = false;
					}
				}
			}
		}
	}
}
void CPeakSeak::SP_GetSeriesPeakDeque(deque<pair<unsigned, unsigned> > &seriesPeak){
	if (!seriesPeak.empty()) seriesPeak.clear();
	const unsigned peakSize = m_Peak.size();
	if (peakSize == 0) return;

	vector<int> peakType;
	for (unsigned i=0; i<peakSize; ++i) 
		peakType.push_back(m_Peak[i].Type);

	for (unsigned i=0; i<peakSize-1; ++i) {
		if (abs(peakType[i])!=1) //find begin 
		{
			for (unsigned j=i+1; j<peakSize; ++j)
			{
				if (peakType[j]!=peakType[i])
				{
					seriesPeak.push_back(std::make_pair(i,j-1));
					i=j-1;
					break;
				}
			}
		}
	}

	//���һ���
	unsigned lastPeak = peakSize-1;
	if (abs(peakType[lastPeak])!=1)
	{
		int end_peak_no = lastPeak;
		while (lastPeak - 1 >=0)
		{
			lastPeak -= 1;
			if (lastPeak==0)
			{
				if (peakType[lastPeak]!=peakType[end_peak_no])
				{
					seriesPeak.push_back(std::make_pair(lastPeak+1,end_peak_no));
					return;
				}
				seriesPeak.push_back(std::make_pair(0,end_peak_no));
				return; 
			}
			if (peakType[lastPeak]!=peakType[end_peak_no]){
				seriesPeak.push_back(std::make_pair(lastPeak+1,end_peak_no));
				return; //ֻȡһ��
			}
		}
	}
}
void CPeakSeak::SP_CheckPeakToValley(const double p2vRatio, deque<pair<unsigned, unsigned> > &DividePeakId)
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
		double X1 = m_Peak[bgId].StartSema.x;
		double Y1 = m_Peak[bgId].StartSema.y;
		double X2 = m_Peak[edId].FinSema.x;
		double Y2 = m_Peak[edId].FinSema.y;
		double K = (Y2-Y1)/(X2-X1);
		double C = Y2 - K * X2;
		for (unsigned i=bgId; i<=edId; ++i)
		{
			double XH = m_Peak[i].PeakSema.x;
			double YH = m_Peak[i].PeakSema.y;

			double XR = m_Peak[i].FinSema.x;
			double YR = m_Peak[i].FinSema.y;

			double H = K * XH + C;
			double R = K * XR + C;

			double PeakH = fabs(fabs(YH)-fabs(H));
			double P2VRatio = fabs(fabs(YR)-fabs(R));

			pPEAKh.push_back(PeakH);
			pP2VRatio.push_back(P2VRatio);

			CSema sema(m_Peak[i].FinSema);
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
				m_Peak[bgId+i].dType = CPeak::VERTDIVIDE;
				m_Peak[bgId+i+1].dType = CPeak::VERTDIVIDE;
				m_Peak[bgId+i].FinSema  = secPoint[i];
				m_Peak[bgId+i+1].StartSema  = secPoint[i];
			}
			else //��ȱȸ����û�ָ��ֵ���������
			{
				m_Peak[bgId+i].dType = CPeak::PEAKVALLEY;
			}
		}

		//if (bSucceed)
		//{
		//	std::pair<unsigned, unsigned> p = std::make_pair(bgId, edId);
		//	tmpDeque.push_back(p);
		//}

		DividePeakId.pop_front();
	}

	tmpDeque.swap(DividePeakId);
}
void CPeakSeak::SP_SingleVertDivideBaseLineCheck(unsigned no) {
	unsigned startIdx = m_Peak[no].StartSema.idx;
	unsigned peakIdx  = m_Peak[no].PeakSema.idx;
	unsigned finIdx   = m_Peak[no].FinSema.idx;

	bool isSuss = false;
	bool isChange = false;
	double x1=m_Peak[no].StartSema.x;
	double y1=m_Peak[no].StartSema.y;
	double x2=m_Peak[no].FinSema.x;
	double y2=m_Peak[no].FinSema.y;
	double k=(y2-y1)/(x2-x1);
	double c=y1-k*x1;

	for (unsigned i=finIdx-1;  i>=startIdx+1; --i) {
		double line_y  = k*m_OrigSema[i].x+c;  //������
		double curve_y = m_OrigSema[i].y;      //ԭ������

		//�ж��Ƿ�������Ӱ���ҵ�һ���������ô��߷ַ�
		//��2013-05-14 �߼��ϵ�����������÷���������Ŵ�ֱ�ַ壬��ôӦ�ð�����ƶ�������Ӱ����
		//				���ȫ�����ڻ����·��ű�Ƿ�����ߣ�
		bool isAboveBaseLine = false;
		if (curve_y > 0)
		{
			isAboveBaseLine = (curve_y - line_y >= 0.0001f) ? true: false;
		}
		else
		{
			isAboveBaseLine = (fabs(curve_y) - fabs(line_y) <= 0.0001f) ? true: false;
		}

		if (!isAboveBaseLine) // �����ڻ����·�
		{
			if (i==startIdx+1)
			{
				isChange = true;
			}
			else
			{
				if (i>=peakIdx)
					isChange = true;
				else
					m_Peak[no].StartSema = m_OrigSema[i];
			}

			break;
		}

	}

	if (isChange) //���ص���Ӱ����
	{
		if (m_Peak[no].dType == CPeak::VERTDIVIDE){
			m_Peak[no].dType = CPeak::PEAKVALLEY;
			m_Peak[no].Type = abs(m_Peak[no].Type)/m_Peak[no].Type;
		}

	}


	//for (unsigned i=startIdx+1; i<finIdx; ++i) {
	//	double line_y  = k*m_OrigSema[i].x+c;  //������
	//	double curve_y = m_OrigSema[i].y;      //ԭ������

	//	//�ж��Ƿ�������Ӱ���ҵ�һ���������ô��߷ַ�
	//	bool isAboveBaseLine = false;
	//	if (curve_y > 0)
	//	{
	//		isAboveBaseLine = (curve_y - line_y >= 0.0001f) ? true: false;
	//	}
	//	else
	//	{
	//		isAboveBaseLine = (fabs(curve_y) - fabs(line_y) <= 0.0001f) ? true: false;
	//	}
	//	if (isAboveBaseLine)
	//	{
	//		if (i==finIdx-1) 
	//			isSuss = true;
	//		continue;
	//	}

	//	break;
	//}
	//
	//if (!isSuss) //���ص���Ӱ����
	//{
	//	if (m_Peak[no].dType == CPeak::VERTDIVIDE){
	//		m_Peak[no].dType = CPeak::PEAKVALLEY;
	//		m_Peak[no].Type = abs(m_Peak[no].Type)/m_Peak[no].Type;
	//	}
	//	
	//}

}
void CPeakSeak::SP_SinglePeakValleyBaseLineCheck(unsigned no) {
	unsigned startIdx = m_Peak[no].StartSema.idx;
	unsigned peakIdx  = m_Peak[no].PeakSema.idx;
	unsigned finIdx   = m_Peak[no].FinSema.idx;

	//����ǰ��
	for (unsigned i=startIdx; i<peakIdx; ++i) {
		bool isSuss = false;
		double x1=m_OrigSema[i].x;
		double y1=m_OrigSema[i].y;
		double x2=m_Peak[no].FinSema.x;
		double y2=m_Peak[no].FinSema.y;
		double k=(y2-y1)/(x2-x1);
		double c=y1-k*x1;

		for (unsigned ii=i+1; ii<=peakIdx; ++ii) 
		{
			double line_y= k*m_OrigSema[ii].x+c;
			double curve_y = m_OrigSema[ii].y;

			//�����ж��Ƿ�������Ӱ���ҵ�һ����ȫ����Ӱ�Ĳ��ε��Ǹ����
			
			bool isAboveBaseLine = false;

			if (curve_y > 0)
			{
				isAboveBaseLine = (curve_y - line_y >= 0.0000f) ? true: false;
			}
			else
			{
				isAboveBaseLine = (fabs(curve_y) - fabs(line_y) <= 0.0000f) ? true: false;
			}
			
			if (isAboveBaseLine)
			{
				if (ii==peakIdx) 
					isSuss = true;
				continue;
			}

			break;
		}

		if (isSuss)
		{
			if (m_Peak[no].dType == CPeak::VERTDIVIDE)
			{
			
				int srt_idx = m_Peak[no].StartSema.idx;
				int orig_idx = m_OrigSema[i].idx;
				if (orig_idx != srt_idx)
					m_Peak[no].StartSema = m_OrigSema[i];
			}
			else 
				m_Peak[no].StartSema = m_OrigSema[i];

			break;
		}
		else //���ص���Ӱ����
		{
			if (m_Peak[no].dType == CPeak::VERTDIVIDE)
				m_Peak[no].dType = CPeak::PEAKVALLEY;
		}
	}

	//������
	for (unsigned i=finIdx; i>peakIdx; --i) {
		bool isSuss = false;
		double x1=m_OrigSema[i].x;
		double y1=m_OrigSema[i].y;
		double x2=m_Peak[no].StartSema.x;
		double y2=m_Peak[no].StartSema.y;
		double k=(y2-y1)/(x2-x1);
		double c=y1-k*x1;

		for (unsigned ii=i-1; ii>=peakIdx; --ii) {
			double line_y= k*m_OrigSema[ii].x+c;
			double curve_y = m_OrigSema[ii].y;

			bool isAboveBaseLine = false;

			if (curve_y > 0)
			{
				isAboveBaseLine = (curve_y - line_y >= 0.0000f) ? true: false;
			}
			else
			{
				isAboveBaseLine = (fabs(curve_y) - fabs(line_y) <= 0.0000f) ? true: false;
			}

			if (isAboveBaseLine)
			{
				if (ii==peakIdx) 
					isSuss = true;
				continue;
			}

			break;
		}
		if (isSuss)
		{
			if (m_Peak[no].dType == CPeak::VERTDIVIDE)
			{
				int fin_idx = m_Peak[no].FinSema.idx;
				int orig_idx = m_OrigSema[i].idx;
				if (orig_idx != fin_idx)
					m_Peak[no].FinSema = m_OrigSema[i];
			}
			else
				m_Peak[no].FinSema = m_OrigSema[i];

			break;
		}
		else //���ص���Ӱ����
		{
			if (m_Peak[no].dType == CPeak::VERTDIVIDE)
				m_Peak[no].dType = CPeak::PEAKVALLEY;
		}
	}
}
void CPeakSeak::SP_PeakValleyBaseLineCheck(unsigned begin, unsigned end) 
{// ��Ȼ���У���㷨
	if (begin > end)
	{
		ASSERT(_T("(begin > end)"));
		return;
	}

	const unsigned PeakSize = m_Peak.size();
	unsigned bg = begin;
	unsigned ed = end;
	bool getIndex = false; 

	deque<bool> MinusCheck(PeakSize, false); //�ֶθ�����
	for (unsigned i=begin; i<=end; ++i)
	{
		if (m_Peak[i].Type < 0)
		{
			MinusCheck[i] = true;
		}
	}

	for (; begin<=end; ++begin)
	{  
		bg = begin;
		if (m_Peak[bg].Type==1 || m_Peak[bg].Type==-1) //�������
		{
			ed = bg;
			getIndex = true;
		}

		else //�ص�����
		{
			ed = bg+1;
			int mark = m_Peak[bg].Type;
			for (; ed<PeakSize; ++ed)
			{
				if (m_Peak[ed].Type!=mark || m_Peak[ed].dType!=CPeak::PEAKVALLEY)
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
			unsigned j = m_Peak[i].StartSema.idx;
			do 
			{
				// �����յ�϶���ԭ�����غ�
				int srt_idx = m_Peak[i].StartSema.idx;
				double orig_srt_p = m_OrigSema[srt_idx].y;
				m_Peak[i].StartSema.y = orig_srt_p;

				int fin_idx = m_Peak[o].FinSema.idx;
				double orig_fin_p = m_OrigSema[fin_idx].y;
				m_Peak[o].FinSema.y = orig_fin_p;

				SP_SinglePeakValleyBaseLineCheck(i);
				
				i++;	

			} while (i!=o+1);
			getIndex = false;
		}
		begin = ed;
	}
}
void CPeakSeak::SP_DividePeak(vector<AnalysisConfigArguments::PeakSepaArg> &SepaArgs)        
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
		if (m_Peak[sPNo].Type > 0) isplus = true;
		else isplus = false;


loop: //gotoѭ��
		for ( ; sPNo <= ePNo; --ePNo)
		{
			//�����յ������
			double _k = (m_Peak[ePNo].FinSema.y-m_Peak[sPNo].StartSema.y)/(m_Peak[ePNo].FinSema.x-m_Peak[sPNo].StartSema.x);
			double _b = m_Peak[sPNo].StartSema.y - _k*m_Peak[sPNo].StartSema.x;
			for (unsigned i=m_Peak[sPNo].StartSema.idx; i<m_Peak[ePNo].FinSema.idx; ++i)
			{ 
				double Y0 = _k * m_OrigSema[i].x + _b; //YO>y��ֱ���ϣ�YO<y��ֱ���·���=y����ֱ����
				if (isplus) //�������
				{
					if ( (Y0>0 && fabs(Y0) > fabs(m_OrigSema[i].y) ) || 
						(Y0<0 && fabs(Y0) < fabs(m_OrigSema[i].y) ) ) 
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
					if (fabs(Y0) > fabs(m_OrigSema[i].y)) 
					{
						if ( (Y0<0 && fabs(Y0) > fabs(m_OrigSema[i].y) ) ||
							(Y0>0 && fabs(Y0) < fabs(m_OrigSema[i].y) ) )
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
			int typeLab = 0;
			for (unsigned i=s; i<=e; ++i)
			{
				if (abs(m_Peak[i].Type) != 1)
				{
					typeLab = m_Peak[i].Type;
					break;
				}
			}
			for (; s<e; ++s)
			{
				m_Peak[s].dType = CPeak::VERTDIVIDE;
				m_Peak[s+1].dType = CPeak::VERTDIVIDE;
				m_Peak[s].Type = typeLab;
				m_Peak[s+1].Type = typeLab;
				m_Peak[s].FinSema.y = _k*m_Peak[s].FinSema.x+_b;
				m_Peak[s+1].StartSema = m_Peak[s].FinSema;

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
void CPeakSeak::SP_CheckOverlapPoint(){
	const double overlap_range = 0.006f;
	
	const int PeakSize = m_Peak.size(); 
	if (PeakSize<=1) return;

	//���ҵ����õķ�������ʼ���
	int peakno_flag = 0;
	for (int i=0; i<PeakSize; i++){
		int current_peakno_flag = abs(m_Peak[i].Type);
		if (current_peakno_flag >= peakno_flag)
		{
			peakno_flag = current_peakno_flag;
		}
	}
	peakno_flag++;

	for (int i=0; i<PeakSize-1; i++)
	{
		if (m_Peak[i].dType!=CPeak::PEAKVALLEY)
			continue;

		//if (abs(m_Peak[i].Type)!=1)
		//	continue;

		double next_peak_start_x = m_Peak[i+1].StartSema.x;
		double curr_peak_end_x = m_Peak[i].FinSema.x;

		if (next_peak_start_x - curr_peak_end_x <= overlap_range) // ��Ϊ�ص�
		{
			unsigned orig_start_idx = m_Peak[i].FinSema.idx;
			unsigned orig_end_idx = m_Peak[i+1].StartSema.idx;

			unsigned overlap_idx = 0;
			double overlap_point = static_cast<double>(INT_MAX*1.0f);
			for (unsigned idx=orig_start_idx; idx<=orig_end_idx; idx++)
			{
				double origin_point = m_OrigSema[idx].y;
				if (origin_point <= overlap_point)
				{
					overlap_point = origin_point;
					overlap_idx = idx;
				}
			}

			// ���±���ص���
			m_Peak[i].FinSema = m_OrigSema[overlap_idx];
			m_Peak[i+1].StartSema = m_OrigSema[overlap_idx];

			if (abs(m_Peak[i].Type)!=1){
				m_Peak[i+1].Type = m_Peak[i].Type;
			}
			else{
				m_Peak[i].Type = peakno_flag;
				m_Peak[i+1].Type = peakno_flag;
				peakno_flag++;
			}

			
		}
	}
}
//��4��ɨ��
void CPeakSeak::SP_SplitPeak(const int index,const double Splitime) {
	//�ҵ�����ʱ����Ԫ�����ϵĵ�
	unsigned s=m_Peak[index].StartSema.idx;
	unsigned p=m_Peak[index].PeakSema.idx;
	unsigned f=m_Peak[index].FinSema.idx;

	double sx=m_Peak[index].StartSema.x;
	double px=m_Peak[index].PeakSema.x;
	double fx=m_Peak[index].FinSema.x;

	unsigned idx=0; //�����±�
	double diff = (double)INT_MAX;

	if (Splitime>sx && Splitime<px) //�����
	{
		for (unsigned i=s; i!=p; ++i){
			if (fabs(m_OrigSema[i].x-Splitime)<diff){
				diff=fabs(m_OrigSema[i].x-Splitime);
				idx=i;
			}
		}
		SP_SplitPeak_Left(index, idx);
	}

	else if (Splitime>px && Splitime<fx) //���Ҷ�
	{
		for (unsigned i=p; i!=f; ++i){
			if (fabs(m_OrigSema[i].x-Splitime)<diff){
				diff=fabs(m_OrigSema[i].x-Splitime);
				idx=i;
			}
		}
		SP_SplitPeak_Right(index, idx);
	}

	else
		return;
}
void CPeakSeak::SP_SplitPeak_Left(const int index,const unsigned left) {
	//�ҵ�����ʱ����Ԫ�����ϵĵ�
	unsigned l=left;
	unsigned s=m_Peak[index].StartSema.idx;

	//Ѱ�ҷ��ѷ�Ĳ���
	double height=0.0f;
	unsigned p=0; //���ѷ岨���±�
	for (unsigned i=s; i!=l; ++i){
		if (fabs(m_OrigSema[i].y)>fabs(height)){
			height=m_OrigSema[i].y;
			p=i;
		}
	}
	if (p==0.0f) return;

	//�Է������ķ������У��(��ֱ�Ӳ��ں��棬Ȼ��˳��Ի�
	int findType = 1;
	std::vector<CPeak> buffVect;
	for (unsigned i=0; i<(unsigned)index; ++i)
	{	
		if (abs(m_Peak[i].Type)!=1)
			findType = m_Peak[i].Type;
		buffVect.push_back(m_Peak[i]); //ǰ�벿��
	}

	if (findType>0) findType++;
	else findType--;

	CPeak newPeak(findType, m_Peak[index].StartSema, m_OrigSema[p], m_OrigSema[l]); 
	m_Peak[index].StartSema = m_OrigSema[l];
	m_Peak[index].Type = findType;


	//�����߷ַ�ֱ����Ϊ��ֱ�ַ�
	double x1 = newPeak.StartSema.x;	//m_Peak[index].StartSema.x;
	double y1 = newPeak.StartSema.y;	//m_Peak[index].StartSema.y;
	double x2 = m_Peak[index].FinSema.x;//newPeak.FinSema.x;
	double y2 = m_Peak[index].FinSema.y;//newPeak.FinSema.y;
	double k = (y2-y1)/(x2-x1);
	double c = y1 - k * x1;
	double old_x = m_Peak[index].StartSema.x;
	double new_y = old_x * k + c;
	m_Peak[index].StartSema.y = new_y;
	newPeak.FinSema.y = new_y;
	//������߷ַ�Ϊ��ֱ�ַ�
	m_Peak[index].dType = CPeak::VERTDIVIDE;
	newPeak.dType = CPeak::VERTDIVIDE;

	buffVect.push_back(newPeak);
	buffVect.push_back(m_Peak[index]);
	
	for (unsigned i=index+1; i<m_Peak.size(); ++i)
	{
		if (abs(m_Peak[i].Type)!=1){
			if (m_Peak[i].Type>0) m_Peak[i].Type++;
			else m_Peak[i].Type--;
		}
		buffVect.push_back(m_Peak[i]); //��벿��
	}
	m_Peak.swap(buffVect);
}
void CPeakSeak::SP_SplitPeak_Right(const int index,const unsigned right) {
	//�ҵ�����ʱ����Ԫ�����ϵĵ�
	unsigned r=right;
	unsigned f=m_Peak[index].FinSema.idx;

	//Ѱ�ҷ��ѷ�Ĳ���
	double height=0.0f;
	unsigned p=0; //���ѷ岨���±�
	for (unsigned i=r; i!=f; ++i){
		if (fabs(m_OrigSema[i].y)>fabs(height)){
			height=m_OrigSema[i].y;
			p=i;
		}
	}
	if (p==0.0f) return;

	//�Է������ķ������У��
	int findType = 1;
	std::vector<CPeak> buffVect;
	for (unsigned i=0; i<(unsigned)index; ++i)
	{	if (abs(m_Peak[i].Type)!=1)
	findType = m_Peak[i].Type;
	buffVect.push_back(m_Peak[i]); //ǰ�벿��
	}

	if (findType>0) findType++;
	else findType--;

	CPeak newPeak(findType, m_OrigSema[r], m_OrigSema[p], m_Peak[index].FinSema); 
	m_Peak[index].FinSema = m_OrigSema[r];
	//m_Peak[index].PeakSema = leftSema;
	m_Peak[index].Type = findType;


	//�����߷ַ�ֱ����Ϊ��ֱ�ַ�
	double x1 = m_Peak[index].StartSema.x;
	double y1 = m_Peak[index].StartSema.y;
	double x2 = newPeak.FinSema.x;
	double y2 = newPeak.FinSema.y;
	double k = (y2-y1)/(x2-x1);
	double c = y1 - k * x1;
	double old_x = m_Peak[index].FinSema.x;
	double new_y = old_x * k + c;
	m_Peak[index].FinSema.y = new_y;
	newPeak.StartSema.y = new_y;
	//������߷ַ�Ϊ��ֱ�ַ�
	m_Peak[index].dType = CPeak::VERTDIVIDE;
	newPeak.dType = CPeak::VERTDIVIDE;

	buffVect.push_back(m_Peak[index]);
	buffVect.push_back(newPeak);

	for (unsigned i=index+1; i<m_Peak.size(); ++i)
	{
		if (abs(m_Peak[i].Type)!=1){
			if (m_Peak[i].Type>0) m_Peak[i].Type++;
			else m_Peak[i].Type--;
		}
		buffVect.push_back(m_Peak[i]); //��벿��
	}
	m_Peak.swap(buffVect);
}
bool CPeakSeak::SP_HandPeakSpration(const int index,const double Spra_t)
{
	//�ҵ��ַ�ʱ����ԭ�����ϵĵ�
	if (index < 0 || index >= (int)m_Peak.size())
		return false;
	unsigned SpIndex = INT_MAX; //�ַ���ԭ�����ϵĵ���±�
	double diff = (double)INT_MAX;
	for (unsigned i=m_Peak[index].StartSema.idx; i<m_Peak[index].FinSema.idx; ++i)
	{
		if (fabs(m_OrigSema[i].x - Spra_t) <= diff)
		{
			diff = fabs(m_OrigSema[i].x - Spra_t);
			SpIndex = i;
		}
	}

	if (SpIndex==INT_MAX) return false; //δ�ҵ��±�

	//����
	if (m_Peak[index].Type > 0)
	{
		bool bFindLeft = false;
		bool bFindRight = false;
		CSema leftSema; //��ߵķ�
		double leftMax = static_cast<double>(-INT_MAX);
		for (unsigned l=SpIndex; l>m_Peak[index].StartSema.idx; --l)
		{//������ߣ��ҵ�y��仯���ơ����ȫ���½�����Ѱ��ʧ��
			if (m_OrigSema[l].y <= m_OrigSema[l-1].y)
			{
				//leftSema = m_OrigSema[l-1];
				bFindLeft = true;
			}
			if (bFindLeft && fabs(m_OrigSema[l].y) >= leftMax) //�б仯���ƺ�ſ�ʼ�����ߵ�
			{
				leftMax = fabs(m_OrigSema[l].y);
				leftSema = m_OrigSema[l];
			}
		}

		CSema rightSema; //�ұߵķ�
		double rightMax = static_cast<double>(-INT_MAX);
		for (unsigned r=SpIndex; r<m_Peak[index].FinSema.idx-1; ++r)
		{
			if (m_OrigSema[r+1].y >= m_OrigSema[r].y)
			{
				//rightSema = m_OrigSema[r+1];
				bFindRight = true;
			}
			if (bFindRight && fabs(m_OrigSema[r].y) >= rightMax)
			{
				rightMax = fabs(m_OrigSema[r].y);
				rightSema = m_OrigSema[r];
			}
		}
		if (bFindLeft && bFindRight) //���Էַ�
		{
			//���ֹ��ַ��ʱ����У��
			diff = (double)INT_MAX;
			for (unsigned i=leftSema.idx; i<rightSema.idx; ++i)
			{
				if (fabs(m_OrigSema[i].y)<=diff)
				{
					diff = fabs(m_OrigSema[i].y);
					SpIndex = i; //�õ���ȷ�ķַ�ʱ�䣬ԭ���ߵ��±�
				}
			}
			//�Է������ķ������У��
			int pType = 1;
			std::vector<CPeak> buffVect;
			for (unsigned i=0; i<(unsigned)index; ++i)
			{	if (m_Peak[i].Type!=1)
			pType = m_Peak[i].Type;
			buffVect.push_back(m_Peak[i]); //ǰ�벿��
			}

			CPeak addOne(pType+1, m_OrigSema[SpIndex], rightSema, m_Peak[index].FinSema); //tmp
			m_Peak[index].FinSema = m_OrigSema[SpIndex];
			m_Peak[index].PeakSema = leftSema;
			m_Peak[index].Type = pType+1;
			//�����߷ַ�ֱ����Ϊ��ֱ�ַ�
			double x1 = m_Peak[index].StartSema.x;
			double y1 = m_Peak[index].StartSema.y;
			double x2 = addOne.FinSema.x;
			double y2 = addOne.FinSema.y;
			double k = (y2-y1)/(x2-x1);
			double c = y1 - k * x1;
			double old_x = m_Peak[index].FinSema.x;
			double new_y = old_x * k + c;
			m_Peak[index].FinSema.y = new_y;
			addOne.StartSema.y = new_y;
			//������߷ַ�Ϊ��ֱ�ַ�
			m_Peak[index].dType = CPeak::VERTDIVIDE;
			addOne.dType = CPeak::VERTDIVIDE;

			buffVect.push_back(m_Peak[index]);
			buffVect.push_back(addOne);

			for (unsigned i=index+1; i<m_Peak.size(); ++i)
			{
				if (m_Peak[i].Type!=1)
					m_Peak[i].Type += 1;
				buffVect.push_back(m_Peak[i]); //��벿��
			}
			m_Peak.swap(buffVect);
			return true;
		}
	}
	else //����
	{
		bool bFindLeft = false;
		bool bFindRight = false;
		CSema leftSema; //��ߵķ�
		double leftMax = static_cast<double>(-INT_MAX);
		for (unsigned l=SpIndex; l>m_Peak[index].StartSema.idx; --l)
		{//������ߣ��ҵ�y��仯���ơ����ȫ���½�����Ѱ��ʧ��
			if (m_OrigSema[l].y >= m_OrigSema[l-1].y)
			{
				//leftSema = m_OrigSema[l-1];
				bFindLeft = true;
			}
			if (bFindLeft && fabs(m_OrigSema[l].y) >= leftMax) //�б仯���ƺ�ſ�ʼ�����ߵ�
			{
				leftMax = fabs(m_OrigSema[l].y);
				leftSema = m_OrigSema[l];
			}
		}

		CSema rightSema; //�ұߵķ�
		double rightMax = static_cast<double>(-INT_MAX);
		for (unsigned r=SpIndex; r<m_Peak[index].FinSema.idx-1; ++r)
		{
			if (m_OrigSema[r+1].y <= m_OrigSema[r].y)
			{
				bFindRight = true;
			}
		}
		if (bFindLeft && bFindRight) //���Էַ�
		{
			//���ֹ��ַ��ʱ����У��
			diff = static_cast<double>(-INT_MAX);
			for (unsigned i=leftSema.idx; i<rightSema.idx; ++i)
			{
				if (m_OrigSema[i].y>=diff)
				{
					diff = m_OrigSema[i].y;
					SpIndex = i; //�õ���ȷ�ķַ�ʱ�䣬ԭ���ߵ��±�
				}
			}
			//�Է������ķ������У��
			int pType = -1;
			std::vector<CPeak> buffVect;
			for (unsigned i=0; i<(unsigned)index; ++i)
			{	
				if (m_Peak[i].Type!=-1)
					pType = m_Peak[i].Type;
				buffVect.push_back(m_Peak[i]); //ǰ�벿��
			}

			CPeak addOne(pType-1, m_OrigSema[SpIndex], rightSema, m_Peak[index].FinSema); //tmp
			m_Peak[index].FinSema = m_OrigSema[SpIndex];
			m_Peak[index].PeakSema = leftSema;
			m_Peak[index].Type = pType-1;
			buffVect.push_back(m_Peak[index]);
			buffVect.push_back(addOne);

			for (unsigned i=index+1; i<m_Peak.size(); ++i)
			{
				if (m_Peak[i].Type!=-1)
					m_Peak[i].Type -= 1;
				buffVect.push_back(m_Peak[i]); //��벿��
			}
			m_Peak.swap(buffVect);
			return true;
		}
	}
	return false;
}
void CPeakSeak::SP_HandAdjustStart(vector<pair<unsigned, double> > &Arg)
{
	const unsigned ArgSize =  Arg.size();
	const unsigned PeakSize = m_Peak.size();
	const unsigned SemaSize = m_OrigSema.size();
	vector<pair<unsigned, double> > buffVect; //��ʱ����������Arg�����ݶԻ�

	for (unsigned i=0; i<ArgSize; ++i)
	{
		unsigned p = Arg[i].first;         //�������ķ��
		double dModify_time = Arg[i].second; //����ʱ��

		//�Ѿ��ҵ����,�Է�Ž����ж��Ƿ��������������
		bool clear = false; //�Ƿ������������

		if (p == 0)//��һ��������
		{
			if (m_Peak[p].PeakSema.x > dModify_time)
				clear = true;
		}
		else //�ڶ������Ժ�����
		{
			if (abs(m_Peak[p].Type)==1) //����
			{
				if ((m_Peak[p].PeakSema.x > dModify_time) && (m_Peak[p-1].FinSema.x <= dModify_time))
					clear = true;
			}
			else
			{
				if ((m_Peak[p].PeakSema.x > dModify_time) && (m_Peak[p-1].PeakSema.x <= dModify_time))
					clear = true;
			}
		}

		if (clear) //�ֹ�������Խ���
		{
			unsigned left = 0;
			unsigned right = 0;
			if (p == 0)
				right = m_Peak[p].PeakSema.idx;
			else
			{
				left = m_Peak[p-1].PeakSema.idx;
				right = m_Peak[p].PeakSema.idx;
			}

			//�ҵ����������Ӧԭ�����ϵĵ�
			for (unsigned index = left; index < right; ++index)
			{
				if ((m_OrigSema[index].x <= dModify_time) && (m_OrigSema[index+1].x >= dModify_time))
				{
					buffVect.push_back(std::make_pair(p, m_OrigSema[index].x)); //��ǰ���������ȷ����У��
					//�ֹ�����
					//if (abs(m_Peak[Ano].Type)==1) //����
					m_Peak[p].StartSema = m_OrigSema[index];
					break;
					//else //������
					//{
					//	m_Peak[Ano].StartSema = m_OrigSema[index]; //���ı�
					//	m_Peak[Ano-1].FinSema = m_Peak[Ano].StartSema; //��һ��������յ�ı�
					//}
				}
			}
		}
	}
	Arg.swap(buffVect);
}
void CPeakSeak::SP_HandAdjustFinish(vector<pair<unsigned, double> > &Arg)
{
	const unsigned ArgSize =  Arg.size();
	const unsigned PeakSize = m_Peak.size();
	const unsigned SemaSize = m_OrigSema.size();
	vector<pair<unsigned, double> > buffVect; //��ʱ����������Arg�����ݶԻ�
	
	if(PeakSize<1) return;//  �������¾ͻ�����Խ��[12/28/2012 ���]

	for (unsigned i=0; i<ArgSize; ++i)
	{
		unsigned p = Arg[i].first;         //�������ķ��
		double dModify_time = Arg[i].second; //����ʱ��

		//�Ѿ��ҵ����,�Է�Ž����ж��Ƿ��������������
		bool clear = false; //�Ƿ������������
		
		if (p == PeakSize-1)//���һ��������
		{
			if (m_Peak[p].PeakSema.x < dModify_time)
				clear = true;
		}
		else //��һ���嵽���һ����֮������
		{
			if (abs(m_Peak[p].Type)==1) //����
			{
				if ((m_Peak[p].PeakSema.x < dModify_time) && (m_Peak[p+1].StartSema.x >= dModify_time))
					clear = true;
			}
			else
			{
				if ((m_Peak[p].PeakSema.x < dModify_time) && (m_Peak[p+1].PeakSema.x >= dModify_time))
					clear = true;
			}
		}
	
		if (clear) //�ֹ�������Խ���
		{
			unsigned left = 0;
			unsigned right = 0;
			if (p == PeakSize-1)
			{
				left = m_Peak[p].PeakSema.idx;
				right = m_OrigSema.size();
			}
			else
			{
				left = m_Peak[p].PeakSema.idx;
				right = m_Peak[p+1].StartSema.idx;
			}

			//�ҵ����������Ӧԭ�����ϵĵ�
			for (unsigned index = left; index < right; ++index)
			{
				if ((m_OrigSema[index].x <= dModify_time) && (m_OrigSema[index+1].x >= dModify_time))
				{
					buffVect.push_back(std::make_pair(p, m_OrigSema[index].x)); //��ǰ���������ȷ����У��
					//�ֹ�����
					//if (abs(m_Peak[Ano].Type)==1) //����
					//{
						m_Peak[p].FinSema = m_OrigSema[index];
						break;
					//}
					//else //������
					//{
					//	m_Peak[Ano].FinSema = m_OrigSema[index]; //���ı�
					//	m_Peak[Ano+1].StartSema = m_Peak[Ano].FinSema; //��һ��������յ�ı�
					//	break;
					//}
				}
			}
		}
	}
	Arg.swap(buffVect);
}
void CPeakSeak::SP_HandAdjustPeak(vector<double> &Arg)
{
	const unsigned ArgSize =  Arg.size();
	const unsigned PeakSize = m_Peak.size();
	const unsigned SemaSize = m_OrigSema.size();
	//���ֹ����壨��ȣ��������б���
	for (unsigned ArgIndex = 0; ArgIndex<ArgSize; ++ArgIndex)
	{
		for (unsigned i=0; i<PeakSize; ++i) //�ҵ���Ҫ�����ķ��λ���ĸ�����
		{
			if((Arg[ArgIndex] > m_Peak[i].StartSema.x)
				&&(Arg[ArgIndex] < m_Peak[i].FinSema.x)) //�ҵ�
			{
				//��ԭ��Ԫ������
				unsigned left = m_Peak[i].StartSema.idx;
				unsigned right = m_Peak[i].FinSema.idx;
				for (unsigned j=left; j<right-1; ++j)
				{
					if ((Arg[ArgIndex] >= m_OrigSema[j].x)
						&&(Arg[ArgIndex] <= m_OrigSema[j+1].x))//��ԭ
					{//�Ƿ�Ҫ��׼������һ���㣬�Աȸ��������һ��
						m_Peak[i].PeakSema = m_OrigSema[j];
						Arg[ArgIndex] = m_OrigSema[j].x; //У�����ֹ����壨��ȣ�
					}
				}
			}
		}
	}
}
//��5��ɨ��
bool CPeakSeak::SP_TwoLineIsIntersect(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3)
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
void CPeakSeak::SP_GetBaseLineArgs(CPeak::DivideType divType, deque<pair<unsigned, unsigned> > &Arg)
{
	if (m_Peak.empty()) return;

	const unsigned ResultSize = m_Peak.size();
	if (ResultSize==1)
	{
		if (m_Peak[0].dType == divType) {
			pair<unsigned, unsigned> p = std::make_pair(0,0); //ֱ�ӷ����һ����
			Arg.push_back(p);
		}
		return;
	}
	
	//�Ȱѷ��Ϸַ���������Ŷ�����ͬһ������
	vector<unsigned> pIndex;
	for (unsigned i=0; i<ResultSize-1; ++i)
	{
		if (m_Peak[i].dType == divType){

			pIndex.push_back(i);

			if (i+1 == ResultSize-1) //��ֹ��(2,2)�����
				if (m_Peak[i+1].dType == divType)
					pIndex.push_back(i+1);

			if (m_Peak[i].Type != m_Peak[i+1].Type)
				pIndex.push_back(UINT_MAX);
		}
	}
	pIndex.push_back(UINT_MAX);
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
			if (rIndex.front() == UINT_MAX && rIndex.back()== UINT_MAX){
				rIndex.clear();
				rIndex.push_back(pIndex[i]);
				continue;
			}

			pair<unsigned, unsigned> p = std::make_pair(rIndex.front(), rIndex.back());
			Arg.push_back(p);
			rIndex.clear();
			rIndex.push_back(pIndex[i]);
		}
	}
	if (Arg.empty()) // if (!rIndex.empty())
	{
		if ((rIndex.front()==UINT_MAX) || (rIndex.back()==UINT_MAX))
			return;

		pair<unsigned, unsigned> p = std::make_pair(rIndex.front(), rIndex.back());
		Arg.push_back(p);

	}
}
void CPeakSeak::SP_VertDivideBaseLineCheck(unsigned begin, unsigned end)
{
	const unsigned start_no=begin;
	const unsigned end_no=end;
	//��ֱ�ַ����У��
	if (begin > end)
	{
		ASSERT(_T("(begin > end)"));
		return;
	}

	// �����յ�϶���ԭ�����غ�
	int srt_idx = m_Peak[begin].StartSema.idx;
	double orig_srt_p = m_OrigSema[srt_idx].y;
	m_Peak[begin].StartSema.y = orig_srt_p;

	int fin_idx = m_Peak[end].FinSema.idx;
	double orig_fin_p = m_OrigSema[fin_idx].y;
	m_Peak[end].FinSema.y = orig_fin_p;

	const unsigned PeakSize = m_Peak.size();
	unsigned bg = begin;
	unsigned ed = end;
	bool getIndex = false; 
	deque<bool> MinusCheck(PeakSize, false); //�ֶθ�����
	for (unsigned i=begin; i<=end; ++i)
	{
		if (m_Peak[i].Type < 0)
		{
			MinusCheck[i] = true;
		}
	}
	for (; begin<=end; ++begin)
	{
		bg = start_no;

		if (abs(m_Peak[bg].Type)==1) //�������
		{
			ed = bg;
		}
		else				  //�ط����
		{
			ed = bg+1;
			int mark = m_Peak[bg].Type;
			for (; ed<PeakSize; ++ed)
			{

				if (m_Peak[ed].Type != mark || m_Peak[ed].dType != CPeak::VERTDIVIDE)
				{
					ed--;
					break;
				}
				if (ed+1 == PeakSize) //modify if (ed == PeakSize)
				{
					break;
				}

			}
		}
		begin = ed;
		for (unsigned i=bg; i<=ed; i++)
			SP_SingleVertDivideBaseLineCheck(i);
		 
	}

	
}
void CPeakSeak::SP_PlusTangetBaseLineCheck(unsigned begin, unsigned end)
{
	for (unsigned i=begin; i<=end; ++i)//�Ը����������ű���
	{
		for (unsigned j=end; j>=i; --j)
		{
			if (m_Peak[i].FinSema.x >= m_Peak[j].StartSema.x ) //�ҵ�һ�����������зַ�
			{
				SP_PeakValleyBaseLineCheck(i+1, j);
				//�߶�һ��
				double x0 = m_Peak[i].StartSema.x;
				double y0 = m_Peak[i].StartSema.y;
				double x1 = m_Peak[i].FinSema.x;
				double y1 = m_Peak[i].FinSema.y;
				//�߶ζ���
				double x2 = m_Peak[j].StartSema.x;
				double y2 = m_Peak[j].StartSema.y;
				double x3 = m_Peak[j].FinSema.x;
				double y3 = m_Peak[j].FinSema.y;
				//�ж������߶��Ƿ��ཻ���������������Ƿ����ص���
				if (SP_TwoLineIsIntersect(x0,y0,x1,y1,x2,y2,x3,y3))
				{
					//::AfxMessageBox( _T("�߶��ཻ"));
					m_Peak[i].FinSema = m_Peak[j].FinSema;
				}
				i = j;
				break;
			}
		}
	}
}
void CPeakSeak::SP_MinusTangetBaseLineCheck(unsigned begin,unsigned end)
{
	for (int i=end; i!=begin-1; --i)//�Ը����������ű���
	{
		if (i<0) break;

		for (unsigned j=begin; j<=i; ++j)
		{
			if (m_Peak[j].FinSema.x >= m_Peak[i].StartSema.x ) //�ҵ�һ�������ķ��зַ�
			{
				CA_PeakValleyBaseLineCheck(m_OrigSema, m_Peak, j, i-1);
				//�߶�һ��
				double x0 = m_Peak[i].StartSema.x;
				double y0 = m_Peak[i].StartSema.y;
				double x1 = m_Peak[i].FinSema.x;
				double y1 = m_Peak[i].FinSema.y;

				//�߶ζ���
				double x2 = m_Peak[j].StartSema.x;
				double y2 = m_Peak[j].StartSema.y;
				double x3 = m_Peak[j].FinSema.x;
				double y3 = m_Peak[j].FinSema.y;
				//�ж������߶��Ƿ��ཻ���������������Ƿ����ص���
				if (TwoLineIsIntersect(x0,y0,x1,y1,x2,y2,x3,y3))
				{
					//::AfxMessageBox( _T("�߶��ཻ"));
					m_Peak[i].StartSema = m_Peak[j].StartSema;
				}
				i = j;
				break;
			}
		}
	}
}
void CPeakSeak::SP_MergePeak(vector<pair<unsigned,unsigned> > &Arg)
{//�Ժ���������ϲ�����������ƣ��Ժ��衾�Ż���
	const unsigned ArgsSize = Arg.size();
	const unsigned SemaSize = m_OrigSema.size();
	unsigned PeakSize = m_Peak.size();

	for (unsigned ai=0; ai<ArgsSize; ++ai)
	{
		unsigned beginNo = Arg[ai].first;
		unsigned endNo = Arg[ai].second;

		CSema startSema(m_Peak[beginNo].StartSema);
		CSema finishSeam(m_Peak[endNo].FinSema);
		CSema peakSema;
		bool isPlus = abs(m_Peak[beginNo].Type)==m_Peak[beginNo].Type? true: false;
		double dPeakMax=static_cast<double>(-1*INT_MAX), dPeakMin=static_cast<double>(INT_MAX);
		unsigned maxNo=-1, minNo=-1;
		for (unsigned i=beginNo; i<=endNo; ++i)
		{
			if (m_Peak[i].PeakSema.y > dPeakMax)
			{
				dPeakMax = m_Peak[i].PeakSema.y;
				maxNo = i;
			}
			if (m_Peak[i].PeakSema.y < dPeakMin)
			{
				dPeakMin = m_Peak[i].PeakSema.y;
				minNo = i;
			}
		}

		if (maxNo==-1 && minNo==-1) return;

		if (isPlus)
			peakSema = m_Peak[maxNo].PeakSema;
		else
			peakSema = m_Peak[minNo].PeakSema;

		//�����еĲ���������������
		vector<CPeak> buffPeak;
		for (unsigned pi=0; pi!=beginNo; ++pi)
		{
			buffPeak.push_back(m_Peak[pi]);
		}

		CPeak cpeak(1, startSema, peakSema, finishSeam);
		buffPeak.push_back(cpeak);

		PeakSize = m_Peak.size();
		for (unsigned pi=endNo+1; pi<PeakSize; ++pi)
		{
			buffPeak.push_back(m_Peak[pi]);
		}
		m_Peak.swap(buffPeak);

		//��Ȼ���У��
		//SP_PeakValleyBaseLineCheck(beginNo,beginNo);

	}
}
void CPeakSeak::SP_AddPeak(deque<ConfigArgument> &AddArgs)
{
	const unsigned OrigSemaSize = m_OrigSema.size();
	while (!AddArgs.empty())
	{
		//1, �ҵ�ԭ�����ϵ���ʼ��
		bool bfStart = false;
		CSema StartPoint;
		double starttime = AddArgs[0].BeginTime;
		for (unsigned i=0; i<OrigSemaSize-1; ++i)
		{
			if (m_OrigSema[i].x <= starttime && m_OrigSema[i+1].x >= starttime)
			{
				//starttime = m_OrigSema[i+1].x;
				bfStart = true;
				StartPoint = m_OrigSema[i+1];
				break;
			}
		}

		//2, �ҵ�ԭ�����ϵ���ֹ��
		bool bfEnd = false;
		CSema EndPoint;
		double endtime = AddArgs[0].EndTime;
		for (unsigned i=0; i<OrigSemaSize-1; ++i)
		{
			if (m_OrigSema[i].x <= endtime && m_OrigSema[i+1].x >= endtime)
			{
				//endtime = m_OrigSema[i];
				bfEnd = true;
				EndPoint = m_OrigSema[i];
				break;
			}
		}

		//3,��Ԫ���ߵĲ���򲨹�
		CSema m_PeakPoint;
		bool bFindPeak = false;
		double dPeakType = AddArgs[0].InputValue;
		if (bfStart && bfEnd) 
		{

			if (dPeakType > 0.0f) //�Ҳ���
			{

				for (unsigned i = StartPoint.idx; i<EndPoint.idx; ++i)
				{
					if (m_OrigSema[i].y >= m_PeakPoint.y)
					{
						m_PeakPoint = m_OrigSema[i];
						bFindPeak = true;
					}
				}
			}
			else if (dPeakType < 0.0f)
				//�Ҳ���
			{
				m_PeakPoint.y = (double)INT_MAX;
				for (unsigned i = StartPoint.idx; i<EndPoint.idx; ++i)
				{
					if (m_OrigSema[i].y <= m_PeakPoint.y)
					{
						m_PeakPoint = m_OrigSema[i];
						bFindPeak = true;
					}
				}
			}
		}

		//4,��һ��3�����������������
		if (bFindPeak)
		{
			CPeak peak(StartPoint, m_PeakPoint, EndPoint);
			//��֮���ӷ�������Ϊ���壬��ʱ�������ڳ�Ѫ˫�����û����и���
			if (dPeakType - 1 == 0.000000)
			{
				peak.Type = 1;
			}
			else if (dPeakType + 1 == 0.000000)
			{
				peak.Type = -1;
			}
			m_Peak.push_back(peak);

			CA_PeakValleyBaseLineCheck(m_OrigSema, m_Peak, 
				m_Peak.size()-1,
				m_Peak.size()-1); //��Ȼ���У��
		}


		AddArgs.pop_front();
	}
	//�Ƿ���Ҫ�Խ����������



	// ����һ���������
	SP_GetOtherPeakArgs();// ��������������������ӣ���β���ӣ�
	SP_GetPeakArea(); //�����
	SP_SetPeakAreaUnit(m_Magnification);
	//SP_GetAllMPA();//ɾ��֮ǰ�����еķ������ֵ����Ա����m_cArrayMPA
	//SP_GetAllMH();
}
void CPeakSeak::SP_AddPeak_One(ConfigArgument &AddArg){
	//���жϵ�ǰ��ӷ�����Ƿ����֮ǰ�ķ�����ص�
	const unsigned PeakSize = m_Peak.size();
 
	double dA = AddArg.BeginTime;
	double dB = AddArg.EndTime;
	for (unsigned i=0; i<PeakSize; ++i)  // ����ǰ���ж��Ƿ�����ص�
	{
		double dC = m_Peak[i].StartSema.x;
		double dD = m_Peak[i].FinSema.x;

		if (m_AnalysisConfigArg.IsOverlapping(dA, dB, dC, dD))
		{
			return;
		}
	}

	const unsigned OrigSemaSize = m_OrigSema.size();

	//1,�ҵ�ԭ�����ϵ���ʼ��
	bool bfStart = false;
	CSema StartPoint;
	double starttime = AddArg.BeginTime;
	for (unsigned i=0; i<OrigSemaSize-1; ++i)
	{
		if (m_OrigSema[i].x <= starttime && m_OrigSema[i+1].x >= starttime)
		{
			//starttime = m_OrigSema[i+1].x;
			bfStart = true;
			StartPoint = m_OrigSema[i+1];
			break;
		}
	}

	//2,�ҵ�ԭ�����ϵ���ֹ��
	bool bfEnd = false;
	CSema EndPoint;
	double endtime = AddArg.EndTime;
	for (unsigned i=0; i<OrigSemaSize-1; ++i)
	{
		if (m_OrigSema[i].x <= endtime && m_OrigSema[i+1].x >= endtime)
		{
			//endtime = m_OrigSema[i];
			bfEnd = true;
			EndPoint = m_OrigSema[i];
			break;
		}
	}

	//3,��Ԫ���ߵĲ���򲨹�
	CSema m_PeakPoint;
	bool bFindPeak = false;
	double dPeakType = AddArg.InputValue;
	if (bfStart && bfEnd)
	{

		if (dPeakType > 0.0f) //�Ҳ���
		{
			m_PeakPoint.y = -(double)INT_MAX;
			for (unsigned i = StartPoint.idx; i<EndPoint.idx; ++i)
			{
				if (m_OrigSema[i].y >= m_PeakPoint.y)
				{
					m_PeakPoint = m_OrigSema[i];
					bFindPeak = true;
				}
			}
		}
		else if (dPeakType < 0.0f)
		//�Ҳ��� 
		{
			m_PeakPoint.y = (double)INT_MAX;
			for (unsigned i = StartPoint.idx; i<EndPoint.idx; ++i)
			{
				if (m_OrigSema[i].y <= m_PeakPoint.y)
				{
					m_PeakPoint = m_OrigSema[i];
					bFindPeak = true;
				}
			}
		}
	}

	//4,��һ��3�����������������
	if (bFindPeak)
	{
		CPeak peak(StartPoint, m_PeakPoint, EndPoint);
		//��֮���ӷ�������Ϊ���壬��ʱ�������ڳ�Ѫ˫�����û����и���
		if (dPeakType - 1 == 0.000000)
		{
			peak.Type = 1;
		}
		else if (dPeakType + 1 == 0.000000)
		{
			peak.Type = -1;
		}
		m_Peak.push_back(peak);


		SP_PeakValleyBaseLineCheck(m_Peak.size()-1,m_Peak.size()-1); //��Ȼ���У��

	}
	 

	//�Խ����������
	std::sort(m_Peak.begin(), m_Peak.end());

	// ����һ���������
	SP_GetOtherPeakArgs();// ��������������������ӣ���β���ӣ�
	SP_GetPeakArea(); //�����
	SP_SetPeakAreaUnit(m_Magnification);
}
//��6��ɨ��
void CPeakSeak::SP_GetOtherPeakArgs()
{
	const unsigned PeakSize = m_Peak.size();
	double factor = 1.0f; //��������
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = m_Peak[i].StartSema.x;
		double y1 = m_Peak[i].StartSema.y;
		double x2 = m_Peak[i].FinSema.x;
		double y2 = m_Peak[i].FinSema.y;

		double peakWidth = fabs(x2-x1);
		m_Peak[i].PeakWidth = peakWidth;  //���

		double tana = (y2-y1)/(x2-x1);
		double c = y1 - x1 * tana;

		double py = m_Peak[i].PeakSema.y; 
		double px = m_Peak[i].PeakSema.x;
		double ty = px*tana+c;

		double peakHeight = fabs(ty - py); //���ָ�ֵ����Ŀ���
		m_Peak[i].PeakHeight = peakHeight; //���

		//double PeakWidthLeft =  sqrt((ty/factor-y1/factor)*(ty/factor-y1/factor)+(px-x1)*(px-x1));  
		//double PeakWidthRight = sqrt((y2/factor-ty/factor)*(y2/factor-ty/factor)+(x2-px)*(x2-px));
		double PeakWidthLeft  = fabs(px-x1);
		double PeakWidthRight = fabs(x2-px);
		//double lr = PeakWidthLeft/PeakWidthRight;
		//m_Peak[i].Tailing = lr; //��β����

		double hx = px;
		double hy = (py-ty)/2 + ty;
		double _c = hy-hx*tana;		

		double diff_y = INT_MAX;
		//unsigned idxl, idxr;
		unsigned idxl=0, idxr=0;

		for (unsigned l=m_Peak[i].StartSema.idx; l<=m_Peak[i].PeakSema.idx; ++l) //���У��ƽ��е�
		{
			double ly = m_OrigSema[l].x * tana + _c;
			if (diff_y >= fabs(m_OrigSema[l].y - ly))
			{
				diff_y = fabs(m_OrigSema[l].y - ly);
				idxl = l;
			}
		}
		CSema tmpFront(m_OrigSema[idxl]); //������
		m_Peak[i].MidStartSema = tmpFront;

		diff_y = INT_MAX;

		for (unsigned r=m_Peak[i].PeakSema.idx; r<=m_Peak[i].FinSema.idx; ++r) //���У��ƽ��е�
		{
			double ry = m_OrigSema[r].x * tana + _c;

			if (diff_y >= fabs(m_OrigSema[r].y - ry))
			{
				diff_y = fabs(m_OrigSema[r].y - ry);
				idxr = r;
			}
		}
		CSema tmpLast(m_OrigSema[idxr]);   //������
		m_Peak[i].MidFinSema = tmpLast;

		double hk = (m_Peak[i].MidFinSema.y-m_Peak[i].MidStartSema.y)/(m_Peak[i].MidFinSema.x-m_Peak[i].MidStartSema.x);
		double hc = m_Peak[i].MidStartSema.y - m_Peak[i].MidStartSema.x * hk;
		double rey = hx*hk+hc;
		CSema HalfPeak(m_Peak[i].PeakSema.idx, hx, rey); // ����е�
		m_Peak[i].MidPeakSema = HalfPeak;

	}

	//���Ӱ���ֱ�ߣ��벨��Ĵ����ཻ��һ�㡣���������ǰ�󳤶�
	//const unsigned HalfSize = Half.size();
	//ASSERT(HalfSize==PeakSize);
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = m_Peak[i].MidStartSema.x; 
		double y1 = m_Peak[i].MidStartSema.y;
		double x2 = m_Peak[i].MidFinSema.x; 
		double y2 = m_Peak[i].MidFinSema.y;
		//double halfPeakWidth = sqrt((y2/factor-y1/factor)*(y2/factor-y1/factor)+(x2-x1)*(x2-x1));
		double halfPeakWidth = fabs(x2-x1);
		m_Peak[i].HalfPeakWidth = halfPeakWidth;  //�����

		double x0 = m_Peak[i].MidPeakSema.x;      //�����ǰ�󳤶�
		double y0 = m_Peak[i].MidPeakSema.y;
		//double halfPeakWidthLeft =  sqrt((y0/factor-y1/factor)*(y0/factor-y1/factor)+(x0-x1)*(x0-x1));
		//double halfPeakWidthRight = sqrt((y2/factor-y0/factor)*(y2/factor-y0/factor)+(x2-x0)*(x2-x0));
		double halfPeakWidthLeft =  fabs(x0-x1);
		double halfPeakWidthRight = fabs(x2-x0);
		double disfactor = halfPeakWidthLeft/halfPeakWidthRight;
		m_Peak[i].Disfactor = disfactor;  //���������


		double resTime = m_Peak[i].PeakSema.x;
		double w50 = m_Peak[i].HalfPeakWidth;
		double rw_tmp = 0.0000f;
		if (w50 != 0.0000f) 
			rw_tmp = resTime / w50;
		m_Peak[i].ColumnPlate = 5.54 * rw_tmp * rw_tmp; //������
	}

	SP_GetTailFactor();
}
void CPeakSeak::SP_GetTailFactor(){

	const unsigned PeakSize = m_Peak.size();
	for (unsigned i=0; i<PeakSize; ++i)
	{
		double x1 = m_Peak[i].StartSema.x;
		double y1 = m_Peak[i].StartSema.y;
		double x2 = m_Peak[i].FinSema.x;
		double y2 = m_Peak[i].FinSema.y;
		double peak_y = m_Peak[i].PeakSema.y; 
		double peak_x = m_Peak[i].PeakSema.x;

		// ��������
		double k = (y2-y1)/(x2-x1);
		double c = y1 - x1 * k;
		// ����ʱ���ڻ����ϵĵ�
		double baseline_y = peak_x * k + c;

		double height_5percent = (peak_y-baseline_y)*0.05 + baseline_y; //5%���
		double height_5percent_c = height_5percent - peak_x * k;

		//�󽻵�
		double diff_y = INT_MAX;
		unsigned idxl, idxr; //���ҵ�����
		for (unsigned l=m_Peak[i].StartSema.idx; l<=m_Peak[i].PeakSema.idx; ++l) //���У��ƽ��е�
		{
			double ly = m_OrigSema[l].x * k + height_5percent_c;
			if (diff_y >= fabs(m_OrigSema[l].y - ly))
			{
				diff_y = fabs(m_OrigSema[l].y - ly);
				idxl = l;
			}
		}
		CSema left_at_height_5percent(m_OrigSema[idxl]); //5%������

		//�ҽ���
		diff_y = INT_MAX;
		for (unsigned r=m_Peak[i].PeakSema.idx; r<=m_Peak[i].FinSema.idx; ++r) //���У��ƽ��е�
		{
			double ry = m_OrigSema[r].x * k + height_5percent_c;
			if (diff_y >= fabs(m_OrigSema[r].y - ry))
			{
				diff_y = fabs(m_OrigSema[r].y - ry);
				idxr = r;
			}
		}
		CSema right_at_height_5percent(m_OrigSema[idxr]); //5%����յ�


		//W5.0Ϊ5%��ߴ��ķ��min����twΪ5%��ߴ�ǰ����屣��ʱ���ʱ������min����
		double w5p0 = right_at_height_5percent.x - left_at_height_5percent.x;
		double tw = peak_x - left_at_height_5percent.x;
		if (tw!=0.0000f)
			m_Peak[i].Tailing =  w5p0 / (tw * 2); //��β����
	}
}

void CPeakSeak::SP_CalcResolution()
{//������ڰ������֮��

	const unsigned PeakSize = m_Peak.size();
	if (PeakSize == 1) 
	{
		m_Peak[0].Resolution = -1.000f;
		return;
	}

	for (unsigned i=1; i<PeakSize; ++i) 
	{
		double res_time_diff = m_Peak[i].PeakSema.x - m_Peak[i-1].PeakSema.x;
		double half_peak_sum = m_Peak[i].HalfPeakWidth + m_Peak[i-1].HalfPeakWidth;

		if (half_peak_sum == 0.0000f)
		{
			m_Peak[i].Resolution = 0.000f;
		}

		double R = (2.35/2.0) * res_time_diff / half_peak_sum;
		m_Peak[i].Resolution = R;
	}
}

void CPeakSeak::SP_GetPeakArea()
{
	const unsigned PeakSize = m_Peak.size();
	const unsigned SemaSize = m_OrigSema.size();
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
			if (m_OrigSema[j].x-m_Peak[i].StartSema.x >= 0.000000) //�ҵ����
			{	
				do 
				{
					double x4 = m_OrigSema[j+1].x;
					double y4 = m_OrigSema[j+1].y;
					double x3 = m_OrigSema[j].x;
					double y3 = m_OrigSema[j].y;

					double h = fabs(x4-x3);
					double front = fabs(y3-(x3*k+c));
					double behind = fabs(y4-(x4*k+c));
					area += (front+behind)*h/2;
					j++;
				} while (m_OrigSema[j].x - x2 <= 0.0000f); //�ҵ��յ�
				m_Peak[i].PeakArea = area;
				break;
			}
		}
	}
	//�������ÿ���������������������߷ַ�����Ҫ��ȥ�ظ��������һ����
	deque<pair<unsigned, unsigned> > PlusTangetArgs;  //���зַ�
	deque<pair<unsigned, unsigned> > MinusTangetArgs; //���зַ�
	CA_GetBaseLineArgs(CPeak::PLUSTANGET, m_Peak, PlusTangetArgs);
	while (!PlusTangetArgs.empty())
	{
		deque<pair<unsigned, unsigned> > dequBuff;
		unsigned begin = PlusTangetArgs[0].first;
		unsigned end =  PlusTangetArgs[0].second;
		for (unsigned i = begin; i<=end; ++i)
		{
			if (m_Peak[i].FinSema.x < m_Peak[i+1].StartSema.x)
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
				if (m_Peak[mainPeakNo].PeakArea < m_Peak[i].PeakArea)
				{
					ASSERT(_T("CPeak::PLUSTANGET (m_Peak[mainPeakNo].PeakArea < m_Peak[i].PeakArea)"));
				}
				m_Peak[mainPeakNo].PeakArea -= m_Peak[i].PeakArea;
			}
			dequBuff.pop_front();
		}
		PlusTangetArgs.pop_front();
	}

	CA_GetBaseLineArgs(CPeak::MINUSTANGET, m_Peak, MinusTangetArgs);
	while (!MinusTangetArgs.empty())
	{
		deque<pair<unsigned, unsigned> > dequBuff;
		unsigned begin = MinusTangetArgs[0].second;
		unsigned end =  MinusTangetArgs[0].first;
		for (int i = begin; i!=end-1; --i)
		{
			if (i<0) continue;
			
			if (m_Peak[i].StartSema.x > m_Peak[i-1].FinSema.x)
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
			for (int i=subFirstPeakNo; i!=subLastPeakNo-1; --i)
			{
				if (m_Peak[mainPeakNo].PeakArea < m_Peak[i].PeakArea)
				{
					ASSERT(_T("CPeak::MINUSTANGET (m_Peak[mainPeakNo].PeakArea < m_Peak[i].PeakArea)"));
				}
				m_Peak[mainPeakNo].PeakArea -= m_Peak[i].PeakArea;
			}
			dequBuff.pop_front();
		}
		MinusTangetArgs.pop_front();
	}
}
void CPeakSeak::SP_SetPeakAreaUnit(double factor){
	const unsigned PeakSize = m_Peak.size();
	for (unsigned i=0; i<PeakSize; ++i){
		m_Peak[i].PeakArea =m_Peak[i].PeakArea * factor;
	}
}
void CPeakSeak::SP_GetAllMPA()
{
	if (m_cArrayMPA.GetSize()!=0)
		m_cArrayMPA.RemoveAll();

	const size_t PeakSize = m_Peak.size();
	for (size_t i=0; i<PeakSize; ++i)
	{
		m_cArrayMPA.Add(m_Peak[i].PeakArea);
	}
}
void CPeakSeak::SP_GetAllMH()
{
	if (m_cArrayMH.GetSize()!=0)
		m_cArrayMH.RemoveAll();

	const size_t PeakSize = m_Peak.size();
	for (size_t i=0; i<PeakSize; ++i)
	{
		m_cArrayMH.Add(m_Peak[i].PeakHeight);
	}
}
void CPeakSeak::SP_EraseByPeakArea(vector<double> &PeakArea)
{//��ɾ���壬δ�Է������������
	vector<CPeak> buffPeak;
	const unsigned PeakSize = m_Peak.size();
	const unsigned PeakAreaSize = PeakArea.size();
	if (PeakSize!=PeakAreaSize)
	{
		TRACE(_T("(PeakSize!=PeakAreaSize)"));
		ASSERT(TRUE);
	}
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (fabs(m_Peak[i].PeakArea) > PeakArea[i])
		{
			buffPeak.push_back(m_Peak[i]);
		}
	}
	const unsigned buffPeakSize = buffPeak.size();
	m_MinPeakAreaEarseNum = PeakSize - buffPeakSize;
	if (m_MinPeakAreaEarseNum!=0)
	{
		m_Peak.swap(buffPeak);
		SP_PeakTypeChange();
	}
}
void CPeakSeak::SP_EraseByPeakHeight(vector<double> &PeakHeight)
{
	vector<CPeak> buffPeak;
	const unsigned PeakSize = m_Peak.size();
	const unsigned PeakHeightSize = PeakHeight.size();
	if (PeakSize!=PeakHeightSize) ASSERT(TRUE);

	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (fabs(m_Peak[i].PeakHeight) > PeakHeight[i])
			buffPeak.push_back(m_Peak[i]);
	}
	const unsigned buffPeakSize = buffPeak.size();
	m_MinPeakHeightEarseNum = PeakSize - buffPeakSize;
	if (m_MinPeakHeightEarseNum!=0)
	{
		m_Peak.swap(buffPeak);
		SP_PeakTypeChange();
	}
	
}
void CPeakSeak::SP_DelPeak(deque<ConfigArgument> &delArgs){

	vector<unsigned> DelArgs; //ɾ�������

	//ֻҪɾ������������ĳ����ķ嶥����ɾ�������
	unsigned ArgsSize = delArgs.size();
	const unsigned PeakSize = m_Peak.size();

	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (delArgs[i].ArgName == _T("ɾ����")) 
		{
			double startTime = delArgs[i].BeginTime;
			double endTime   = delArgs[i].EndTime;
			for (unsigned j=0; j<PeakSize; ++j)
			{
				double peakTime = m_Peak[j].PeakSema.x;

				// ���Ƿ��ѷ������յ��벨���غ����
				double spStartTime = m_Peak[j].StartSema.x;
				double spFinTime = m_Peak[j].FinSema.x;

				// ����2λС������
				int ispPeak  = static_cast<int>((peakTime + 0.005) * 100);
				int ispStart = static_cast<int>((spStartTime+0.005) * 100);
				int ispFin   = static_cast<int>((spFinTime + 0.005) * 100);
				peakTime = (double)ispPeak / 100;
				spStartTime = (double)ispStart / 100;
				spFinTime = (double)ispFin / 100;

				if (( fabs(spStartTime - peakTime)<=0.001f ) || ( fabs(peakTime - spFinTime)<=0.001f ))
				{
					//if ((fabs(spStartTime - startTime)<=0.01f && fabs(spStartTime - endTime)<=0.01f) ||
					//	(fabs(spFinTime   - startTime)<=0.01f && fabs(spFinTime   - endTime)<=0.01f))
					if (((spStartTime >= startTime) && (spStartTime <= endTime)) ||
						((spFinTime   >= startTime) && (spFinTime   <= endTime)))
					{
						DelArgs.push_back(j);
					}
				}
				else //�Ƿ��ѷ�
				{
					if (peakTime >= startTime && peakTime <= endTime)
					{
						DelArgs.push_back(j);
					}
				}
			}
		}
	}

	vector<CPeak> buffPeak;
	//const unsigned PeakSize = m_Peak.size();
	ArgsSize = DelArgs.size();

	for (unsigned i=0; i<PeakSize; ++i)
	{
		bool isRepeat = false;
		for (unsigned j=0; j<ArgsSize; ++j)
			if (i==DelArgs[j])	isRepeat = true;
		if (!isRepeat)	buffPeak.push_back(m_Peak[i]);
		isRepeat = false;
	}

	buffPeak.swap(m_Peak);

	// ���ɾ�����������еķ壬�����¸�����
	SP_PeakTypeChange();

	// �޸ķ������յ�
	//SP_AdjustStartandEndPoint();

}
void CPeakSeak::SP_DelPeak(ConfigArgument &delArg){

	int DelArg = -1; //ɾ�������
	vector<unsigned> DelArgs; //ɾ�������

	//ֻҪɾ������������ĳ����ķ嶥����ɾ�������
	const unsigned PeakSize = m_Peak.size();


	if (delArg.ArgName == _T("ɾ����"))
	{
		double startTime = delArg.BeginTime;
		double endTime   = delArg.EndTime;
		for (unsigned j=0; j<PeakSize; ++j)
		{
			double peakTime = m_Peak[j].PeakSema.x;

			// ���Ƿ��ѷ������յ��벨���غ����
			double spStartTime = m_Peak[j].StartSema.x;
			double spFinTime = m_Peak[j].FinSema.x;

			// ����2λС������
			int ispPeak  = static_cast<int>((peakTime + 0.005) * 100);
			int ispStart = static_cast<int>((spStartTime+0.005) * 100);
			int ispFin   = static_cast<int>((spFinTime + 0.005) * 100);
			peakTime = (double)ispPeak / 100;
			spStartTime = (double)ispStart / 100;
			spFinTime = (double)ispFin / 100;

			if (( fabs(spStartTime - peakTime)<=0.001f ) || ( fabs(peakTime - spFinTime)<=0.001f ))
			{
				if (((spStartTime >= startTime) && (spStartTime <= endTime)) ||
					((spFinTime   >= startTime) && (spFinTime   <= endTime)))
				{
					DelArgs.push_back(j);
				}
			}
			else //�Ƿ��ѷ�
			{
				if (peakTime >= startTime && peakTime <= endTime)
				{
					DelArgs.push_back(j);
				}
			}
		}
	}

	vector<CPeak> buffPeak;
	//const unsigned PeakSize = m_Peak.size();
	const unsigned ArgsSize = DelArgs.size();

	for (unsigned i=0; i<PeakSize; ++i)
	{
		bool isRepeat = false;
		for (unsigned j=0; j<ArgsSize; ++j)
			if (i==DelArgs[j])	isRepeat = true;
		if (!isRepeat)	buffPeak.push_back(m_Peak[i]);
		isRepeat = false;
	}

	buffPeak.swap(m_Peak);

	// ���ɾ�����������еķ壬�����¸�����
	SP_PeakTypeChange();

	// �޸ķ������յ�
	//SP_AdjustStartandEndPoint();

}
void CPeakSeak::SP_DelPeak_One(ConfigArgument &delArg){

	int DelArg = -1; //ɾ�������

	//ֻҪɾ������������ĳ����ķ嶥����ɾ�������
	const unsigned PeakSize = m_Peak.size();


	if (delArg.ArgName == _T("ɾ����"))
	{
		double startTime = delArg.BeginTime;
		double endTime   = delArg.EndTime;
		for (unsigned j=0; j<PeakSize; ++j)
		{
			double peakTime = m_Peak[j].PeakSema.x;

			// ���Ƿ��ѷ������յ��벨���غ����
			double spStartTime = m_Peak[j].StartSema.x;
			double spFinTime = m_Peak[j].FinSema.x;

			// ����2λС������
			int ispPeak  = static_cast<int>((peakTime + 0.005) * 100);
			int ispStart = static_cast<int>((spStartTime+0.005) * 100);
			int ispFin   = static_cast<int>((spFinTime + 0.005) * 100);
			peakTime = (double)ispPeak / 100;
			spStartTime = (double)ispStart / 100;
			spFinTime = (double)ispFin / 100;

			if (( fabs(spStartTime - peakTime)<=0.001f ) || ( fabs(peakTime - spFinTime)<=0.001f ))
			{
				if (((spStartTime >= startTime) && (spStartTime <= endTime)) ||
					((spFinTime   >= startTime) && (spFinTime   <= endTime)))
				{
					DelArg = j; 
				}
			}
			else //�Ƿ��ѷ�
			{
				if (peakTime >= startTime && peakTime <= endTime)
				{
					DelArg = j;
				}
			}
		}
	}

	if (DelArg==-1) return;
	
	vector<CPeak> buffPeak;
	
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if (i != DelArg)	
			buffPeak.push_back(m_Peak[i]);
	}

	buffPeak.swap(m_Peak);

	// ���ɾ�����������еķ壬�����¸�����
	SP_PeakTypeChange();

	// �޸ķ������յ�
	//SP_AdjustStartandEndPoint();
}
void CPeakSeak::SP_AdjustStartandEndPoint() {
	const unsigned PeakSize = m_Peak.size();
	for (unsigned i=0; i<PeakSize; ++i) {
		//ֻ���������������㣨�����Ӵ�ֱ�ַ���������Ԫ�����ϣ�
		if (abs(m_Peak[i].Type)==1) {
			unsigned curve_start_idx = m_Peak[i].StartSema.idx;
			unsigned curve_end_idx = m_Peak[i].FinSema.idx;
			m_Peak[i].StartSema.y = m_OrigSema[curve_start_idx].y;
			m_Peak[i].FinSema.y = m_OrigSema[curve_end_idx].y;
		}
	}
}
void CPeakSeak::SP_GetPeakPercent()
{
	// ���ķ����% �� ���%
	const unsigned PeakSize = m_Peak.size();

	double totPeakArea=0.0, totPeakHeigh=0.0;
	for (unsigned i=0; i<PeakSize; ++i)
	{
		totPeakArea += m_Peak[i].PeakArea;
		totPeakHeigh += m_Peak[i].PeakHeight;
	}
	if (totPeakArea==0.0 || totPeakHeigh==0.0)
	{
		ASSERT(totPeakArea==0.0 || totPeakHeigh==0.0);
	}
	for (unsigned i=0; i<PeakSize; ++i)
	{
		m_Peak[i].PeakAreaPercent = m_Peak[i].PeakArea / totPeakArea * 100.0f;
		m_Peak[i].PeakHighPercent = m_Peak[i].PeakHeight / totPeakHeigh * 100.0f;
	}
}
void CPeakSeak::SP_CheckPeakType()
{
	const size_t PeakSize = m_Peak.size();
	for (size_t i=0; i<PeakSize; ++i) {
		if (m_Peak[i].dType == CPeak::VERTDIVIDE){
			//��֤���õĴ�ֱ�ַ���������Ƿ�ֱ�ַ�
			//ֻҪ�������յ��Ƿ�����ԭ������
			int fin_idx = m_Peak[i].FinSema.idx;
			double fin_p = m_Peak[i].FinSema.y;
			double orig_fin_p = m_OrigSema[fin_idx].y;
			bool is_fin_overlap = (fin_p == orig_fin_p) ? true : false;
			if (!is_fin_overlap) // û���غϣ�ȷʵ�Ǵ�ֱ�ַ壬���ؼ����ж���
				continue;

			// �����ж����
			int srt_idx = m_Peak[i].StartSema.idx;
			double srt_p = m_Peak[i].StartSema.y;
			double orig_srt_p = m_OrigSema[srt_idx].y;
			bool is_srt_overlap = (srt_p == orig_srt_p) ? true : false;
			if (is_srt_overlap) 
				m_Peak[i].dType = CPeak::PEAKVALLEY; //��㣬�յ㶼�غϣ��ض��Ƿ������
		}
	}
}
void CPeakSeak::SP_SetPeakType()
{
	const size_t PeakSize = m_Peak.size();
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
		if (m_Peak[i].Type < 0) m_Peak[i].PeakType.negative = 1;
		if (abs(m_Peak[i].Type) == 1) m_Peak[i].PeakType.single = 1;
		if (m_Peak[i].dType == CPeak::PEAKVALLEY)   m_Peak[i].PeakType.overlap_pl = 1; 
		if (m_Peak[i].dType == CPeak::MINUSTANGET)  m_Peak[i].PeakType.overlap_ls = 1; 
		if (m_Peak[i].dType == CPeak::PLUSTANGET)   m_Peak[i].PeakType.overlap_rs = 1; 
		if (m_Peak[i].dType == CPeak::VERTDIVIDE)   m_Peak[i].PeakType.overlap_vs = 1; 
		if (m_Peak[i].Tailing > TAILING) m_Peak[i].PeakType.tail = 1;
	}
	//���������߷ַ�ķǼ�������������
	for (size_t i=0; i<PeakSize; ++i)
	{
		if (m_Peak[i].dType == CPeak::PLUSTANGET)//���ǵ�һ����Ϊ������
		{
			unsigned bg = i;
			double endx = m_Peak[i].FinSema.x;
			for ( ; i<PeakSize-1; ++i)
			{
				if (endx < m_Peak[i+1].StartSema.x)
				{
					break;
					i--;
				}
			}
			m_Peak[bg].PeakType.overlap_pl = 1; 
		} 
	}

	//��//��������ַ�ķǼ�������������
	for (int i=PeakSize-1; i>=0; --i)
	{
		if (m_Peak[i].dType == CPeak::MINUSTANGET)
		{
			unsigned bg = i;
			double startx = m_Peak[i].StartSema.x;
			for ( ; i>0; --i)
			{
				if (startx > m_Peak[i-1].StartSema.x)
				{
					break;
					i++;
				}
			}
			m_Peak[bg].PeakType.overlap_pl = 1; 
		} 
	}
}
//void CPeakSeak::SP_GetDividePeakLine() {
//	if (m_cArrayVLine.GetSize()!=0)
//		m_cArrayVLine.RemoveAll();
//
//	const unsigned PeakSize = m_Peak.size();
//	if (PeakSize==0) return;
//	
//	for (unsigned i=0; i<PeakSize; ++i)
//	{
//		if (m_Peak[i].dType == CPeak::VERTDIVIDE) //���ж��Ƿ��Ǵ�ֱ�ַ�
//		{
//			structLine vLine_left,vLine_right;
//
//			//��˴���
//			vLine_left.begin.x=m_Peak[i].StartSema.x;
//			vLine_left.begin.y=m_OrigSema[m_Peak[i].StartSema.idx].y;
//			vLine_left.end.x=m_Peak[i].StartSema.x;
//			vLine_left.end.y=m_Peak[i].StartSema.y;
//
//			//�ұߴ���
//			vLine_right.begin.x=m_Peak[i].FinSema.x;
//			vLine_right.begin.y=m_OrigSema[m_Peak[i].FinSema.idx].y;
//			vLine_right.end.x=m_Peak[i].FinSema.x;
//			vLine_right.end.y=m_Peak[i].FinSema.y;
//
//			m_cArrayVLine.Add(vLine_left); 
//			m_cArrayVLine.Add(vLine_right); 
//		}
//	}
//}
//���ջ�ɨ��
void CPeakSeak::SP_RestoreOriginalData() {
	//�ѻ��ֽ�����ݻ�ԭ�������ƶ�ƽ����֮ǰ
	const int c_av = 5;
	const size_t peakSize = m_Peak.size();
	const unsigned origPeakSize = m_OrigSema_backup.size();

	for (size_t i=0; i<peakSize; ++i)
	{
		if (m_Peak[i].dType==CPeak::VERTDIVIDE)
		{
			// ���廹ԭ
			unsigned peak_index = m_Peak[i].PeakSema.idx;
			
			double peak_y = (double)INT_MAX * -1.0;
			for (int j = peak_index-c_av; j<peak_index+c_av; ++j)
			{
				if (j<0 || j>=origPeakSize) continue;
				double orig_peak_y = m_OrigSema_backup[j].y;
				if (peak_y <= orig_peak_y)
				{
					peak_y = orig_peak_y;
					m_Peak[i].PeakSema = m_OrigSema_backup[j];
				}
			}
			

			// �ж��Ƿ��������ϣ��ھͻ�ԭ

			// ��ԭ���
			unsigned start_index = m_Peak[i].StartSema.idx;
			double start_y = m_Peak[i].StartSema.y;
			double origin_y = m_OrigSema[start_index].y;
			if (fabs(start_y - origin_y) <= 0.0000001f)
			{
				m_Peak[i].StartSema = m_OrigSema_backup[start_index];

				start_y = (double)INT_MAX * 1.0;
				for (int j = start_index; j<start_index+c_av; ++j){
					if (j<0 || j>=origPeakSize) continue;
					double orig_start_y = m_OrigSema_backup[j].y;
					if (start_y >= orig_start_y){
						start_y = orig_start_y;
						m_Peak[i].StartSema = m_OrigSema_backup[j];
					}
				}
			}
			// ��ԭ�յ�
			unsigned end_index = m_Peak[i].FinSema.idx;
			double end_y = m_Peak[i].FinSema.y;
			origin_y =  m_OrigSema[end_index].y;
			if (fabs(end_y - origin_y) <= 0.0000001f)
			{
				m_Peak[i].FinSema = m_OrigSema_backup[end_index];

				end_y = (double)INT_MAX * 1.0;
				for (int j = end_index; j<end_index+c_av; ++j){
					if (j<0 || j>=origPeakSize) continue;
					double orig_end_y = m_OrigSema_backup[j].y;
					if (end_y >= orig_end_y){
						end_y = orig_end_y;
						m_Peak[i].FinSema = m_OrigSema_backup[j];
					}
			    }

			}

		}
		else //�Ǵ��߷ַ壬ֱ�ӻ�ԭ
		{

			// ���廹ԭ
			unsigned peak_index = m_Peak[i].PeakSema.idx;

			double peak_y = (double)INT_MAX * -1.0;
			for (int j = peak_index-c_av; j<peak_index+c_av; ++j)
			{
				if (j<0 || j>=origPeakSize) continue;
				double orig_peak_y = m_OrigSema_backup[j].y;
				if (peak_y <= orig_peak_y)
				{
					peak_y = orig_peak_y;
					m_Peak[i].PeakSema = m_OrigSema_backup[j];
				}
			}

			// ��ԭ���
			unsigned start_index = m_Peak[i].StartSema.idx;
			double start_y = m_Peak[i].StartSema.y;
			double origin_y = m_OrigSema[start_index].y;
			if (fabs(start_y - origin_y) <= 0.0000001f)
			{
				m_Peak[i].StartSema = m_OrigSema_backup[start_index];

				start_y = (double)INT_MAX * 1.0;
				for (int j = start_index; j<start_index+c_av; ++j){
					if (j<0 || j>=origPeakSize) continue;
					double orig_start_y = m_OrigSema_backup[j].y;
					if (start_y >= orig_start_y){
						start_y = orig_start_y;
						m_Peak[i].StartSema = m_OrigSema_backup[j];
					}
				}
			}
			// ��ԭ�յ�
			unsigned end_index = m_Peak[i].FinSema.idx;
			double end_y = m_Peak[i].FinSema.y;
			origin_y =  m_OrigSema[end_index].y;
			if (fabs(end_y - origin_y) <= 0.0000001f)
			{
				m_Peak[i].FinSema = m_OrigSema_backup[end_index];

				end_y = (double)INT_MAX * 1.0;
				for (int j = end_index; j<end_index+c_av; ++j){
					if (j<0 || j>=origPeakSize) continue;
					double orig_end_y = m_OrigSema_backup[j].y;
					if (end_y >= orig_end_y){
						end_y = orig_end_y;
						m_Peak[i].FinSema = m_OrigSema_backup[j];
					}
				}
			}



		}
	}
}
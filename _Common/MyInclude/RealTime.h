#pragma once
#include "CSVFile.h"
#include "Peak.h"
#include <deque>



class CPeakList;
class CRealTimeSema;

class CRealTime
{
public:
	CRealTime(void);
	CRealTime(CSuperChartCtrl*);
	~CRealTime(void);

public:
	//�ź��������ݽṹ
	std::vector<CRealTimeSema> m_InputSema;
	std::vector<CRealTimeSema> m_OutputSema;
	std::vector<CRealTimeSema> m_RTTotalAvgSema; //�ܵ�ƽ�����ź���
	std::vector<CRealTimeSema> m_RTTotalSema;    //�ܵ��ź���
	// ������
	int m_prevprocess_peak_nums;
	std::vector<CRealTimeSema> m_RTPeak;   //�������
	std::vector<CPeak> m_Peak;			   //�������

	//Ѱ����Ҫ�ı���
	std::vector<double> m_firstOrder;	//б�ʴ������
	double m_maxSlope;
	int m_id;//��̬��m_firstOrder�±꣩

	//�������
	bool m_isRealTimeParametersSet; //���ִ�����ϱ�־λ
	int m_InputNums; //��������ÿ50���㴦��һ��
	bool m_IsDividePeak; //�Ƿ�����ֱ�ַ�
	bool m_HasNagetiveSpike; //�Ƿ��и������
	int m_Avgs; //�ڲ�ƽ������.Ĭ��5
	int m_Series; //�ڲ��������жϡ�Ĭ��3
	double m_Threshold; //����б��
	double m_HalfPeakWidth; //��С����
	double m_PeakArea; //��С�����

	//��ԭ��ȫ�ֱ����ĳɳ�Ա����
	std::deque<CRealTimeSema> dequetmpSema;// ȫ�־�̬������Ϊƽ��������
	std::deque<CRealTimeSema> dequeBufSema;// ȫ�־�̬������ΪѰ�建��


public:
	// !-----------------------2013.04.19 -------------------------------
	// !-----------------------ʵʱ���߻��ֹ��ܽ��빤��վ--------------------
	//����ӿ�
	//VOID CRealTime::RealTimeProcess(CAnalysisData* pSample_Parameter_PeakProcess, 
	//					const CCurveData &curveData, 
	//					CPeakList &cPeakList);

	VOID CRealTime::RealTimeProcess(CAnalysisData* pSample_Parameter_PeakProcess, 
						double xSrc[], 
						double ySrc[], 
						int nSrc, 
						CPeakList &cPeakList,
						CPeakList &cAllPeakList);

	BOOL CRealTime::addRealTimePoints(const CCurveData &curveData); //׼������
	VOID CRealTime::RT_AddRealTimePoints(double xSrc[], double ySrc[], int nSrc);
	VOID CRealTime::RT_SetProcessParameter(CAnalysisData* pSample_Parameter_PeakProcess);
	VOID CRealTime::RT_SetProcessParameter(bool,bool,int,int,double,double,double);
	VOID CRealTime::RT_Averaging();
	VOID CRealTime::RT_AvgDataSlope();
	VOID CRealTime::RT_PeakSearch();
	VOID CRealTime::RT_PeakReturn();

	VOID CRealTime::RT_RealTimeSema2Peak(); //+ �ѽ�����������ݽṹת��
	VOID CRealTime::RT_GetPeakArea(); //+
	VOID CRealTime::RT_GetPeakHeight(); //+
	VOID CRealTime::RT_Peak2PeakList(CPeakList &cPeakList, CPeakList &cAllPeakList); //+
	VOID CRealTime::RT_Save2PeakList(CPeakList &cPeakList);

	// !-----------------------2010.12.09 -------------------------------
	VOID SetRealTimeData(int begin, int end);
	VOID ClearRealTimeCurve(); // ���ʵʱ����
	VOID DrawRealTimeCurve(); //��ʵʱ����
	VOID DrawRealTimeResult(); //����������ߣ��㣩
	VOID SetSemaDate(CString filePathName); //��ô������ź�������
	VOID SetSemaDate(double x[], double y[], int n);
	VOID ClearSemaDate(); //����ź�������
	VOID SetParameter(bool, bool, double, double, double, int, int);  //����Ѱ�����
	VOID SetChartCtrl(CSuperChartCtrl* p);  //���û�ͼ�ؼ�ָ��
	VOID SetShowHide(const bool g1, const bool g2); //���ÿؼ�����������ʾ 
	VOID GetShowHide(bool &g1, bool &g2); //��ÿؼ�����������ʾ״̬
	VOID SignalProc(); //�ӿ��źŴ�����
	VOID SignalDisplay(); //�ӿ��ź���ʾ����
	VOID GetPeakResult(std::vector<CPeak> &Peak);
	VOID ClearLine();  //����ϴλ�ͼ�Ķ���
	VOID DrawBaseLine();  //������
	VOID DrawDividePeakBaseLine();  //����ֱ�ַ�Ļ���
	VOID DrawResultLine(); //����������ߣ��㣩
	VOID DrawCurve();  //��ԭ������

	// !-----------------------------------------------------------------
	// !
	// ! ���ּܳ�������޸ĺ�ӿڵĹ���
	// !
	// !-----------------------2010.11.04 -------------------------------
	VOID ClearBaseLine();				// !(new)������л���
	VOID ClearStartFinishPoints();	    // !(new)�����㣬��㣬�����
	VOID ClearOrigCurve();				// !(new)���ԭ����
	VOID SetBaseLineColor(COLORREF cr); // !(new)���õ�ǰ��֮����ߵ���ɫ
	// !-----------------------2010.11.05 -------------------------------
	//VOID DisplayBaseLine();				// !(new)��ʾ����
	//VOID HideBaseLine();				// !(new)���ػ���
	//VOID DisplayStartFinishPoints();	// !(new)��ʾ�����
	//VOID HideStartFinishPoints();		// !(new)���������
	//VOID DisplayOrigCurve();			// !(new)��ʾԪ����
	//VOID HideOrigCurve();				// !(new)����Ԫ����

	////////////////////////////////////////////////////////////////////////
	//������
	int counter;
	//������ɫ
	COLORREF m_BaseLineColor;
	std::vector<CSema> m_OrigSema; //ԭʼ����
	std::vector<CSema> m_TagPoint; //���ߵ�����±���

	//�ؼ������᷶Χ
	double dXMin;
	double dXMax;
	double dYMin;
	double dYMax;

	//�ؼ�
	CSuperChartCtrl* m_pCtrl; 

	//�ؼ��ڻ���
	std::vector<CChartLineSerie*> m_phemline;  //���ߵױ���
	std::list<CChartLineSerie*> m_pTagline;    //�����
	std::list<CChartLineSerie*> m_pDividePeakBaseLine;    //��ֱ�ַ��������
	CChartPointsSerie *m_pPoint; //����ʼ����յ�
	CChartPointsSerie *m_pPeak;//����ʼ����յ�
	CChartLineSerie* m_pDrawCurve; //Ԫ���� 
	CChartLineSerie* m_pLinePeakSeek;//Ѱ������� 

	////�ؼ��ڷ���
	//static int GROUPID_FIRST;
	//static int GROUPID_SECOND;
	//static int GROUPID_THREE; // !new
	//bool m_G1_Show;
	//bool m_G2_Show;
	////////////////////////////////////////////////////////////////////////
};
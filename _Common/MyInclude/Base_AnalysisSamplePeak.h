#pragma once
#include "CSVFile.h"
#include "Peak.h"
#include "ConfigArgument.h"
#include "AnalysisData.h"
#include "Base_AnalysisSample.h"

const double TAILING = 1.000000; //��β���ӣ��б�������Ƿ��/��β

class CPeakList;

class CPeakSeak 
{
public:
	//�Զ������ź��������ݽṹ
	std::vector<CSema> m_OrigSema; //ԭʼ����
	std::vector<CPeak> m_Peak;     //�������
	//std::vector<CSema> m_TagPoint; //���ߵ�����±���

	//���ò����ṹ
	AnalysisConfigArguments m_AnalysisConfigArg;

	//�������
	bool m_IsDividePeak; //�Ƿ�����ֱ�ַ�
	bool m_HasNagetiveSpike; //�Ƿ��и������
	int m_Avgs; //�ڲ�ƽ������.Ĭ��5
	int m_Series; //�ڲ��������жϡ�Ĭ��3
	double m_Threshold; //����б��
	double m_HalfPeakWidth; //��С����
	double m_PeakArea; //��С�����
	double m_PeakHeight; //��С���

	DWORD GetPeakCount() const { return static_cast<DWORD>(m_Peak.size()); }//��ȡ�����
	void AutoSetPeakArea(DWORD dwSmallPeakCount){ASSERT(false);}//��Ҫȥ��dwSmallPeakCount��С��
	DWORD GetPeakAreaCount(double dblPeakArea){ASSERT(false);return 0;}//��ȡ�����dblPeakAreaΪ�ֽ��С�����

	//�ؼ������᷶Χ
	double dXMin;
	double dXMax;
	double dYMin;
	double dYMax;

	//����б�ʵ���ֵ
	double m_dMaxSlope; 
	double m_dMinSlope;

	//�崦���������Ҫ���м����
	CArray<double> m_cArrayHPW; //��С�����ڽ�����С����ɾ��֮ǰ��
	CArray<double> m_cArrayMPA; //��С��������ڽ�����С�����ɾ��֮ǰ��
	CArray<double> m_cArrayMH; //��С��ߣ��ڽ�����С���ɾ��֮ǰ��
	typedef CBase_AnalysisSample::structLine structLine;
	CArray<structLine> m_cArrayVLine; //��ֱ�ַ���
	
public:
	CPeakSeak(void);
	~CPeakSeak(void);
	CPeakSeak& operator= (const CPeakSeak& rhs);

	//���ֽӿ�
	BOOL SignalProcess(CAnalysisData* pSample_Parameter_PeakProcess, 
		 CPeakList &cPeakList, 
		 CArray<double> *pcArrayHPW=NULL, 
		 CArray<double> *pcArrayMPA=NULL,
		 CArray<double> *pcArrayMH=NULL,
		 CArray<structLine> *pcArrayVLine=NULL,
		 double *pdMinSlope=NULL,
		 double *pdMaxSlope=NULL); //��׼


	VOID SetSemaDate(CString filePathName); //����������ź�������
	VOID SetSemaDate(double x[], double y[], int n);
	VOID SetSemaDate(const CChartPointsArray& vPoints);
	VOID SetSemaDate(const CChartSerie* pSerie);
	VOID ClearSemaDate(); //����ź�������
	VOID SetParameter(bool IsDividePeak, bool HasNagetiveSpike, double Threshold, 
	double HalfPeakWidth, double PeakArea, double PeakHeight, int Series=3, int Avgs=5);  //����Ѱ�����
	//VOID SetNoChangeCoordinate(); //����ԭ�������귶Χ;  new��
	VOID GetPeakResult(std::vector<CPeak> &Peak);

	// !-----------------------2010.12.17 -------------------------------
	// !-----------------�����ò����ķ崦���㷨����---------------------------
	VOID ConfigArgs_SignalProc();
	VOID ConfigArgs_FirstScan();
	VOID ConfigArgs_SecondScan();
	VOID ConfigArgs_ThirdScan();
	VOID ConfigArgs_FourthScan();
	VOID ConfigArgs_FifthScan();
	VOID ConfigArgs_SixthScan();

	VOID GetPeakTypeStr(CString &str);

public:
	void SP_FirstScan(); 
	void SP_SecondScan(); 
	void SP_ThirdScan(); 
	void SP_FourthScan(); 
	void SP_FifthScan(); 
	void SP_SixthScan(); 
private:
	//��1��ɨ�� 
	void CPeakSeak::SP_Averaging(std::vector<CSema> &avgData);
	void CPeakSeak::SP_GetAvgDataSlope(const vector<CSema> &avgData, vector<double> &firstOrder, double &maxSlope);
	void CPeakSeak::SP_GetMinMaxSlope(const vector<CSema> &avgData, const vector<double> &firstOrder,int N);
	void CPeakSeak::SP_GetpeakSearch_Series(const vector<CSema> &avgData, const vector<double> &firstOrder, vector<double> &slopeBased, deque<bool> &hasNagetiveSpike);
	void CPeakSeak::SP_RestorePeakData(const vector<double> &slopeBased);
	//��2��ɨ�� 
	void CPeakSeak::SP_BanPeak(vector<unsigned> BanPeakNo);
	void CPeakSeak::SP_PeakTypeChange();
	void CPeakSeak::SP_GetHalfPeakWidth();
	void CPeakSeak::SP_GetAllHPW();
	void CPeakSeak::SP_EraseByHalfPeakWidth(vector<double> &MinHalfWidth);
	//��3��ɨ��
	void CPeakSeak::SP_TangentDividPeak(vector<AnalysisConfigArguments::PeakSepaArg> &PeakSepaArgs);
	void CPeakSeak::SP_GetSeriesPeakDeque(deque<pair<unsigned, unsigned> > &seriesPeak);
	void CPeakSeak::SP_CheckPeakToValley(const double p2vRatio, deque<pair<unsigned, unsigned> > &DividePeakId);
	void CPeakSeak::SP_SinglePeakValleyBaseLineCheck(unsigned no);
	void CPeakSeak::SP_PeakValleyBaseLineCheck(unsigned begin, unsigned end);
	void CPeakSeak::SP_DividePeak(vector<AnalysisConfigArguments::PeakSepaArg> &SepaArgs);    
	//��4��ɨ��
	bool CPeakSeak::SP_HandPeakSpration(const int index,const double Spra_t); //׼������
	void CPeakSeak::SP_SplitPeak(const int index,const double Splitime);
	void CPeakSeak::SP_SplitPeak_Left(const int index,const unsigned left);
	void CPeakSeak::SP_SplitPeak_Right(const int index,const unsigned right);
	void CPeakSeak::SP_HandAdjustStart(vector<double> &Arg);
	void CPeakSeak::SP_HandAdjustFinish(vector<double> &Arg);
	void CPeakSeak::SP_HandAdjustPeak(vector<double> &Arg);
	void CPeakSeak::SP_MergePeak(vector<pair<unsigned,unsigned> > &Arg);
	//��5��ɨ��
	bool CPeakSeak::SP_TwoLineIsIntersect(double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3);
	void CPeakSeak::SP_GetBaseLineArgs(CPeak::DivideType divType, deque<pair<unsigned, unsigned> > &Arg);
	void CPeakSeak::SP_VertDivideBaseLineCheck(unsigned begin, unsigned end);
	void CPeakSeak::SP_PlusTangetBaseLineCheck(unsigned begin, unsigned end);
	void CPeakSeak::SP_MinusTangetBaseLineCheck(unsigned begin,unsigned end);
	//��6��ɨ��
	void CPeakSeak::SP_GetOtherPeakArgs();
	void CPeakSeak::SP_GetPeakArea();
	void CPeakSeak::SP_SetPeakAreaUnit(double factor);
	void CPeakSeak::SP_GetAllMPA();
	void CPeakSeak::SP_GetAllMH();
	void CPeakSeak::SP_EraseByPeakArea(vector<double> &PeakArea);
	void CPeakSeak::SP_EraseByPeakHeight(vector<double> &PeakHeight);
	void CPeakSeak::SP_GetPeakPercent();
	void CPeakSeak::SP_SetPeakType();
	void CPeakSeak::SP_GetDividePeakLine();
};



// !----------------------- 2011.04.19 -------------------------------
//CHEM2009��Ŀ����Ҫ�õ�����
class CPeakDraw 
{
public:
	CPeakDraw();

public:
	CPeakSeak m_PeakSeak;
	VOID SetCPeakSeakData(const CPeakSeak &PeakSeak);

	//������
	int counter;

	//�ؼ�
	CSuperChartCtrl* m_pCtrl; 
	
	//������ɫ
	COLORREF m_BaseLineColor;

	//�ؼ��ڻ���
	std::vector<CChartLineSerie* > m_phemline;  //���ߵױ���             
	std::list<CChartLineSerie* > m_pTagline;    //�����                 
	std::list<CChartLineSerie* > m_pDividePeakBaseLine; //��ֱ�ַ��������
	CChartPointsSerie* m_pPoint;        //����ʼ����յ�                    
	CChartPointsSerie* m_pPeak;         //����ʼ����յ�                    
	CChartLineSerie* m_pDrawCurve;		 //Ԫ����                         
	CChartLineSerie* m_pLinePeakSeek;	//Ѱ�������                      

	//�ؼ��ڷ���
	static int GROUPID_FIRST;  
	static int GROUPID_SECOND; 
	static int GROUPID_THREE;   

public:
	VOID SignalDisplay(); //�ӿ��ź���ʾ����
	VOID DrawBaseLine();  //������
	VOID DrawDividePeakBaseLine();  //����ֱ�ַ�Ļ���
	VOID CA_SignalDisplay();
	VOID SetChartCtrl(CSuperChartCtrl* p);  //���û�ͼ�ؼ�ָ��
	VOID ClearLine();  //����ϴλ�ͼ�Ķ���
	VOID DrawCurve();  //��ԭ������
	VOID CA_DrawBaseLine();
	VOID DrawResultLine(); //����������ߣ��㣩
	//
};
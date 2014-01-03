#pragma once
#include "ConfigArgument.h"

//#include "Common\MyInclude\Base_SampleChart.h"
//#include "Common\MyInclude\Base_AnalysisSample.h"

#include <deque>
using std::deque;

class CCurveData;
class CPeakList;



struct  Para
{
	int pn;    //������
	double st; //���ʱ��
	double ft; //�յ�ʱ��
	Para(double s, double f) : pn(0), st(s), ft(f) {  }
	Para(int n, double s, double f) : pn(n), st(s), ft(f) {  }
};

class CSema {
public:
	unsigned  idx;  //�õ��ź��������ź����е�����
	double x;		//ʱ�������x��					
	double y;		//�ź�����y��					
public:
	CSema();
	CSema(int index, double time, double val);
	bool operator<(const CSema &s);
	bool operator==(const CSema &rhs);
	bool operator!=(const CSema &rhs);
	virtual ~CSema();
};

// ===================================================================
// --2010.12.09--
// ����һ��ʵʱ�㣬��¼ÿ��������ĵ�ǰ״̬����㣬����������㣬���ȵ㣬�½����յ㣩
class CRealTimeSema : public CSema
{
public: 
	enum RealTimeStates{
		//δ֪״̬
		UNKNOWN,		  //��ʼ��
		HANDLED,          //�Ѵ������޷��жϴ��ں���״̬
		//�������
		PLUS_START_POINT, //���
		PLUS_RAISE_POINT, //����
		PLUS_PEAK_POINT,  //�����
		PLUS_DROP_POINT,  //�½�
		PLUS_FINISH_POINT, //�յ�
		PLUS_CONTINUOUS_WAVE, //������
		//�������
		MINUS_START_POINT, //���
		MINUS_DROP_POINT,  //�½�
		MINUS_TROUGH_POINT,//���ȵ�
		MINUS_RAISE_POINT, //����
		MINUS_FINISH_POINT, //�յ�
		MINUS_CONTINUOUS_WAVE //������
	};
	CRealTimeSema(int index, double time, double val);
	CRealTimeSema(int index, double time, double val, RealTimeStates rts);
	CRealTimeSema(const CSema& sema);

public:
	RealTimeStates pts; //�õ�ĵ�ǰ״̬
}; 

// --------------------------------------------------------------------------
class CPeak
{
public:
	enum DivideType {
		PEAKVALLEY, //��Ȼ��ߣ���ʼ����
		VERTDIVIDE, //��ֱ�ַ�
		PLUSTANGET, //���зַ�
		MINUSTANGET //���зַ�
	};
	union CPeakType 
	{
		CPeakType() : all(0){ }
		struct {
			unsigned short negative:			1;	//����
			unsigned short single :				1;	//����
			unsigned short overlap_pl:			1;	//�ص���
			unsigned short overlap_ls:			1;	//�ص���(���)
			unsigned short overlap_rs:			1;	//�ص���(�Ҽ�)
			unsigned short overlap_vs:			1;	//�ص���(��ֱ�ַ�)
			unsigned short tail:				1;	//��β
			//------------------------
			unsigned short reserve2future:		9;	//�Ժ���				
		};
		unsigned short all;
	
	//---------------------------
	void CleanMark(void){ all=0; }
	//��÷����͵���������
	CString ConvertToTypeName(void); 
	void	GetFromTypeName(CString cStrName);
	};
public: 
	CPeakType PeakType;		//������,��CPeakStandardƥ��
	int    Type;			//������,1=����; -1=����; >1�ص�����; <-1�ص�����
	DivideType dType;		//�ص���ַ�����
	CSema  StartSema;		//�����
	CSema  PeakSema;		//�壬��
	CSema  FinSema;			//�����
	double PeakWidth;		//���
	double PeakHeight;		//���
	CSema  MidStartSema;	//������
	CSema  MidPeakSema;		//��壬��
	CSema  MidFinSema;		//������
	double HalfPeakWidth;	//����
	double PeakArea;		//�����
	double Disfactor;		//��������
	double Tailing;			//��β���� 
	double PeakAreaPercent; //�����%
	double PeakHighPercent; //���%
	double ColumnPlate;     //����������
	double Resolution;      //�����
public:
	CPeak(void);
	CPeak(CSema startSema, CSema peakSema, CSema finSema);
	CPeak(int sgn,CSema startSema, CSema peakSema, CSema finSema);
	CPeak(int sgn, CSema startSema, CSema peakSema, CSema finSema, double pw, double ph,
			CSema MidStartSema, CSema MidPeakSema, CSema MidFinSema,double hpw, double pa, double df, double tg);
	bool operator==(const CPeak &rhs);
	bool operator==(int value);
	bool operator!=(const CPeak &rhs);
	bool operator< (const CPeak &rhs) const;
	CPeak& operator= (const CPeak& rhs);
	~CPeak(void);
};

//----------------------------------------------------------------------------------------------------------------------
// Ѱ���õ�һϵ���㷨
//----------------------------------------------------------------------------------------------------------------------
void new_Averaging(const std::vector<CSema> &handleSema, std::vector<CSema> &avgData, unsigned avg);	
void new_GetAvgDataSlope(const std::vector<CSema> &avgData, std::vector<double> &firstOrder, double &maxSlope);										         
void new_GetpeakSearch(const std::vector<CSema> &avgData, const std::vector<double> &firstOrder, std::vector<CPeak> &Peak,   
						const double slopeBased, bool hasNagetiveSpike, bool isTwiceJudge); 
void new_RestorePeakData(const std::vector<CSema> &handleSema,std::vector<CPeak> &Peak,const int avg, const double slopeBased); 
void new_DividePeak(std::vector<CPeak> &Peak);
void new_BaselineAdjust(const std::vector<CSema> &handleSema,std::vector<CPeak> &Peak,bool hasNagetiveSpike);
void new_BaselineCheak(const std::vector<CSema> &handleSema,std::vector<CPeak> &Peak,bool);
void new_PeakTypeChange(std::vector<CPeak> &Peak);
void new_GetPeakWidth(const std::vector<CSema> &handleSema,std::vector<CPeak> &Peak,bool);
void new_GetPeakArea(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak);
void new_ErasePeakByHalfWidth(std::vector<CPeak> &Peak, const double halfPeakWidth);
void new_ErasePeakByPeakArea(std::vector<CPeak> &Peak,const double peakArea);
void new_GetTagPoint(const std::vector<CPeak> &Peak, std::vector<CSema> &TagPoint);

//----------------------------------------------------------------------------------------------------------------------
// �����õ��㷨
//----------------------------------------------------------------------------------------------------------------------
VOID test_GetDividePeakPoint(std::vector<CPeak> &Peak, unsigned bg, unsigned ed);
VOID test_DividePeak(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak);
VOID test_ErasePeakByNextto(std::vector<CPeak> &Peak);
VOID new_GetPeakWidth_v2(const std::vector<CSema> &handleSema,std::vector<CPeak> &Peak,bool);
VOID test_GetpeakSearch(const std::vector<CSema> &avgData, const std::vector<double> &firstOrder, std::vector<CPeak> &Peak,   
						const double slopeBased, bool hasNagetiveSpike, bool isTwiceJudge);
VOID test_GetpeakSearch_Series(const std::vector<CSema> &avgData,   //ƽ�����õ��ź���
							   const std::vector<double> &firstOrder,//ƽ�����õ��ź�����б��
							   std::vector<CPeak> &Peak,    //(���)�������
							   double slopeBased, //����б��
							   bool hasNagetiveSpike,//��������������Ƿ��и������
							   const int series);
//----------------------------------------------------------------------------------------------------------------------
// ׷��3���¹���  
// 1. ���ߵ���
// 2. ��������ɾ����
// 3. �����������ӷ�
//----------------------------------------------------------------------------------------------------------------------
int  BinarySearch(const std::vector<CSema> &handleSema, double x); //���ֲ����㷨
VOID one_BaselineCheak(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, const int PeakNo, bool hasNagetiveSpike);
int  one_GetPeakWidth( const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, const int PeakNo, bool isDividePeak);
VOID one_GetPeakArea(const std::vector<CSema> &handleSema,std::vector<CPeak> &Peak, const int PeakNo);
VOID neo_BaseLineAdj(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, Para& p); // 1. ���ߵ���
VOID neo_DelOnePeak(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, Para& p); // 2. ��������ɾ����
VOID neo_AddNewPeak(const std::vector<CSema> &handleSema, std::vector<CPeak> &Peak, Para& p);	// 3. �����������ӷ�

//----------------------------------------------------------------------------------------------------------------------
// ����ʵʱ����
// ����Ѱ��ӿڣ�����Ϊ���������״̬�ж�

void RealTime_Averaging(const std::vector<CRealTimeSema> &InputSema, //����:���ź���
						std::vector<CRealTimeSema> &OutputSema,		//����:ƽ�����õ��ź���
						std::vector<CRealTimeSema> &TotalAvgSema,   //�ܵ�ƽ�����õ��ź���
						std::vector<CRealTimeSema> &TotalSema,      //�ܵ��ź���
						unsigned avg );	//ƽ������

void RealTime_GetAvgDataSlope(  std::vector<CRealTimeSema> &InputSema, //ƽ�����õ��ź���
								std::vector<CRealTimeSema> &TotalAvgSema,
								std::vector<double> &firstOrder,	//б�ʴ������
								double &maxSlope);					//���б�� 

void RealTime_GetpeakSearch(std::vector<CRealTimeSema> &InputSema, //ƽ�����õ��ź���
	const std::vector<double> &firstOrder,		//ƽ�����õ��ź�����б��
	std::vector<CRealTimeSema> &TotAvgSema,
	std::vector<CRealTimeSema> &RealTimeSema,   //(���)�������
	const double slopeBased, bool hasNagetiveSpike, bool isTwiceJudge);

void RealTime_GetpeakSearch_v2(std::vector<CRealTimeSema> &InputSema, //ƽ�����õ��ź���
	const std::vector<double> &firstOrder,		//ƽ�����õ��ź�����б��
	std::vector<CRealTimeSema> &TotAvgSema,
	std::vector<CRealTimeSema> &RealTimeSema,   //(���)�������
	const double slopeBased, bool hasNagetiveSpike, bool isTwiceJudge,
	const int series);

void RealTime_RestorePeakData(std::vector<CRealTimeSema> &InputSema, //���������ź������������
	std::vector<CRealTimeSema> &TotalSema,      //�ܵ��ź���
	std::vector<CRealTimeSema> &RealTimeSema,   //��㲨�岨���յ�����
	const int avg,
	const double slopeBased);  

//----------------------------------------------------------------------------------------------------------------------
// �������ò����㷨����   
//----------------------------------------------------------------------------------------------------------------------
bool TwoLineIsIntersect(double x0, double y0, 
	double x1, double y1, 
	double x2, double y2, 
	double x3, double y3/*, float &InterX, float &InterY*/);

//
// ��һ��ɨ����
//
VOID CA_GetMinMaxSlope(const std::vector<CSema> &avgData,
	const std::vector<double> &firstOrder,
	double &MinSlope, 
	double &MaxSlope,
	int N);


VOID CA_GetpeakSearch_Series(const std::vector<CSema> &avgData,   //ƽ�����õ��ź���
	const std::vector<double> &firstOrder,//ƽ�����õ��ź�����б��
	std::vector<CPeak> &Peak,    //(���)�������
	std::vector<double> &slopeBased, //������б������
	std::deque<bool> &hasNagetiveSpike,//��������������Ƿ��и������
	const int series);

VOID CA_RestorePeakData(const std::vector<CSema> &handleSema, //����ԭ���ź���
	std::vector<CPeak> &Peak,
	const int avg,
	const std::vector<double> &slopeBased); //����б������) 

//
//�ڶ���ɨ����
//
VOID CA_BanPeak(std::vector<CPeak> &Peak,
	vector<unsigned> &BanPeakNo);

VOID CA_GetHalfPeakWidth(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak);

VOID CA_GetAllHPW(std::vector<CPeak> &Peak, CArray<double> &cArray);

VOID CA_EraseByHalfPeakWidth(std::vector<CPeak> &Peak,
	vector<double> &MinHalfWidth);

//
//������ɨ����
//
VOID CA_TangentDividPeak(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	vector<AnalysisConfigArguments::PeakSepaArg> &dequeIdx);

VOID CA_DividePeak_legacy(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<AnalysisConfigArguments::PeakSepaArg> &PeakSepaArgs);

VOID CA_DividePeak(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<AnalysisConfigArguments::PeakSepaArg> &PeakSepaArgs);
//
//���Ĵ�ɨ����
//
bool CA_HandPeakSpration(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	const int index,      //�ַ��±�
	const double Spra_t); //�ַ�ʱ��

 //��ֹ��   VOID CA_HandAddPeak(const std::vector<CSema> &handleSema,
 //��ֹ��	std::vector<CPeak> &Peak,
 //��ֹ��	vector<pair<double, double> >&addPeakArgs);  
VOID CA_HandAdjustStart(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<double> &Arg);

VOID CA_HandAdjustFinish(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<double> &Arg);

VOID CA_HandAdjustPeak(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<double> &Arg);

VOID CA_MergePeak(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	vector<pair<double,double> > &Arg,
	AnalysisConfigArguments* pCA);

VOID CA_AddPeak(const std::vector<CSema> &handleSema, 
	std::vector<CPeak> &Peak,
	deque<ConfigArgument> &AddArgs);
//
//�����ɨ����
//
VOID CA_GetVertDivideBaseLineArgs(const std::vector<CPeak> &Peak,
	deque<pair<unsigned, unsigned> > &Arg); //throw

VOID CA_GetBaseLineArgs(CPeak::DivideType divType,
	const std::vector<CPeak> &Peak,
	deque<pair<unsigned, unsigned> > &Arg);

VOID CA_VertDivideBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end);

BOOL GetLeftPoint(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end,
	unsigned LeftIdx);

BOOL GetRightPoint(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end,
	unsigned RightIdx);

VOID CA_NewPeakValleyBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end);

VOID CA_PeakValleyBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end);

VOID CA_PlusTangetBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end);

VOID CA_MinusTangetBaseLineCheck(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak,
	unsigned begin,
	unsigned end);

//�Աȷ�ȱ�ϵ��
VOID CA_CheckPeakToValley(
	std::vector<CPeak> &Peak,
	const double p2vRatio,
	deque<pair<unsigned, unsigned> > &DividePeakId);


//
//������ɨ����
//
VOID CA_GetOtherPeakArgs(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak);

VOID CA_GetPeakArea(const std::vector<CSema> &handleSema,
	std::vector<CPeak> &Peak);

VOID CA_GetAllMPA(std::vector<CPeak> &Peak, CArray<double> &cArray);
VOID CA_GetAllMH(std::vector<CPeak> &Peak, CArray<double> &cArray);

VOID CA_EraseByPeakArea(std::vector<CPeak> &Peak,
	vector<double> &PeakArea);

VOID CA_EraseByPeakHeight(std::vector<CPeak> &Peak,
	vector<double> &PeakHeight);

VOID CA_PeakTypeChange(std::vector<CPeak> &Peak);

//add
VOID CA_GetPeakPercent(std::vector<CPeak> &Peak);

//add ��ô�ֱ�ַ��ϵĴ���
//VOID CA_GetValleyLine(const CCurveData &curveData, const CPeakList &peakList,  CArrayPALine &PALineResult);
	
VOID CA_SetPeakType(std::vector<CPeak> &Peak);
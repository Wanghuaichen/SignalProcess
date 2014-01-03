
#pragma once

#include "MovingAverage.h"



/*
//---------------------------�������ݶ���--------------------------------
//
//		ɫ���еĶ��壺ʱ�䣨���룩+ ����ֵ��΢����
//
//-----------------------------------------------------------------------
class CHRO_PIC_DATA {
public:
	CHRO_PIC_DATA():dwNo(0),lVal(0){}
	~CHRO_PIC_DATA(){}

	void operator=(const CHRO_PIC_DATA &ori) {dwNo=ori.dwNo;lVal=ori.lVal;}

	DWORD  dwNo;
	long  lVal;
};

typedef CArray<CHRO_PIC_DATA> CDataArray;
*/

//---------------------------�������ݶ���--------------------------------
//
//		ɫ���еĶ��壺ʱ�䣨���ӣ�+ ����ֵ��������
//
//-----------------------------------------------------------------------
class CCurveDataSingle{
public:
	CCurveDataSingle():dX(0.0),dY(0.0){}
	CCurveDataSingle(double x, double y):dX(x),dY(y){}
	~CCurveDataSingle(){}

	void operator=(const CCurveDataSingle &ori) {dX=ori.dX;dY=ori.dY;}

	double  dX;
	double  dY;
};

class CCurveData {
public:
	CCurveData(){m_arrayData.SetSize(0, 20*60*5);}//��ÿ��20�����������㣬����5��������}
	~CCurveData(){}

	void operator=(const CCurveData &ori) {m_arrayData.Copy(ori.m_arrayData);}
	void RemoveAll(void) {m_arrayData.RemoveAll();}
	void AppendData(double x, double y){m_arrayData.Add(CCurveDataSingle(x,y));}
	void AppendData(const CCurveData &data){m_arrayData.Append(data.m_arrayData);}
	//�������ͷ�x,yָ��
	int  ConvertData(double *&x, double *&y, double dScaleX, double dScaleY, double xOffset, double yOffset) const {
		x=NULL;y=NULL;
		int nNum = m_arrayData.GetCount();
		if(nNum==0) return 0;
		int validCount = 0;
		x = new double[nNum]; y= new double[nNum];
		for(int i=0; i<nNum; ++i) {
			x[validCount]=m_arrayData[i].dX*dScaleX + xOffset;
			y[validCount]=m_arrayData[i].dY*dScaleY + yOffset;
			if(x[validCount] < 0) continue;
			++validCount;
		}
		return validCount;
		}


	CArray<CCurveDataSingle> m_arrayData;

};

class CPALine {
public:
	CCurveDataSingle m_point[2];
	//CPALine(CCurveDataSingle oneSingle, CCurveDataSingle twoSingle) 
	//{ m_point[0] = oneSingle; m_point[1] = twoSingle; }
};

typedef CArray<CPALine>  CArrayPALine;


//------------------------------------------------------------------------

class CBase_AnalysisSample;


#define ID_MENU_EXPORT_DATA		50001


class CBase_RealtimeDataMonitor
{
public:
	virtual void OnRealtimeData(CBase_AnalysisSample* pSample, const double xSrc[], const double ySrc[], int nSrc, const double xProcessed[], const double yProcessed[], int nProcessed) = 0;
	virtual void OnStartRun(CBase_AnalysisSample* pSample) = 0;
	virtual void OnEndRun(CBase_AnalysisSample* pSample) = 0;
};


class CBase_SampleChart : public CSuperChartCtrl
{
	DECLARE_DYNAMIC(CBase_SampleChart)
	DECLARE_MESSAGE_MAP()
	afx_msg void OnExportData();

	struct structMinMaxInfo
	{
		double minx;
		double maxx;
		double miny;
		double maxy;
	};
	static void LoadSerieFromFile(structMinMaxInfo&mmi, CChartSerie* pSerie, LPCTSTR filename);

public:
	CBase_SampleChart();
	virtual ~CBase_SampleChart();

public:
	void RefreshSample_Curve2DSeries(CBase_AnalysisSample* pSample ,const CString& dataFolder, bool bRefreshAxisRange);

	void SetRealtimeDataMonitor(CBase_RealtimeDataMonitor* pMonitor){m_RealtimeDataMonitor = pMonitor;}

protected:
	CString m_cStrMainGroup, m_cStrMainSerie;

public:
	CChartSerie* GetMainSerie(){return m_pcCurveMain;}
	const CChartSerie* GetMainSerie() const{return m_pcCurveMain;}

protected:
	CChartSerie* CreateSerie(CChartSerie::enumSerieType eType);
	CChartSerie* CreateSerie();
	CChartSerie* CreateCurve2DSerie(const CString& group, const CString& serie);
	//����������
	CChartSerie* CreateCurve2DMain(void);

	virtual enumMenu OnPrepareMenu(CMenu& defaultMenu, CMenu& customMenu);

public://���ݴ����ͼ
	enum enumPeakFlag
	{
		FLAG_DATA_SOURCE = 0X00000001,//ԭʼ�������ߣ���������
// 		FLAG_POINTS_BEGIN = 0X00000002,//�����
// #ifdef _DEBUG
// 		FLAG_POINTS_PEAK = 0X00000004,//���,
// #else
// 		FLAG_POINTS_PEAK = 0X00000000,//���,
// #endif
// 		FLAG_POINTS_END = 0X00000008,//���յ�
		FLAG_LINES_BASE = 0X00000010,//����
		FLAG_LINES_DIVIDEBASE = 0X00000020,//��ֱ�ַ����
		FLAG_STRINGS_RESERVEDTIME = 0X00000040,//����ʱ��
		FLAG_STRINGS_CONCENTRATION = 0X00000080,//Ũ��
		FLAG_STRINGS_COMPONENTNAME = 0X00000100,//�����
		FLAG_STRINGS_FMJ = 0X00000200,//�����
		FLAG_STRINGS_FG = 0X00000400,//���

		FLAGMASK_BASELINE = /*FLAG_POINTS_BEGIN | FLAG_POINTS_PEAK | FLAG_POINTS_END | */FLAG_LINES_BASE | FLAG_LINES_DIVIDEBASE,
		FLAGMASK_STRINGS_ASSISTANT = FLAG_STRINGS_RESERVEDTIME|FLAG_STRINGS_CONCENTRATION|FLAG_STRINGS_COMPONENTNAME|FLAG_STRINGS_FMJ|FLAG_STRINGS_FG,
		FLAGMASK_ALL = 0XFFFFFFFF,
	};
	enum enumMode
	{
		MODE_SETALL,
		MODE_APPENDONLY,
	};

	//����ָ�������߽��д���δָ�������߱��ֵ�ǰ״̬
	void RedrawPeak(const CBase_AnalysisSample* pSample, DWORD dwFlag, enumMode eMode = MODE_SETALL, bool bRefresh = true);
	void RedrawPeakOnRealTime(const CBase_AnalysisSample* pSample, DWORD dwFlag, enumMode eMode = MODE_SETALL, bool bRefresh = true);

	void ClearPeak(const CBase_AnalysisSample* pSample, DWORD dwFlag);
	void ShowPeak(const CBase_AnalysisSample* pSample, DWORD dwFlag);
	void HidePeak(const CBase_AnalysisSample* pSample, DWORD dwFlag);

	void SetColor(DWORD dwFlag, COLORREF color);//���������
	COLORREF GetColor(enumPeakFlag eFlag) const;//ö��

	void ClearCompSeries();
	void ClearCurve2DSeries();
	void ClearMarkLines();
	void ClearTemporary();

public:
	size_t GetTemporaryGroupID() const{return m_nTemporaryGroupID;}
	CTWenChartPan* ShowRangeFlag(double fromX, double toX, COLORREF cr, size_t nGroupID = -1);//-1��ʾ��ӵ���ʱ��
	CChartFill* ShowPeakFill(const CChartPointsArray& arrPoints, COLORREF cr, size_t nGroupID = -1);//-1��ʾ��ӵ���ʱ��

protected:
	friend class CBase_AnalysisSample;
	CBase_RealtimeDataMonitor* m_RealtimeDataMonitor;
	CMovingAverage m_ma;
//	CChartPointsSerie *m_pBeginPoints; //�����
//	CChartPointsSerie *m_pPeakPoints;//���
//	CChartPointsSerie *m_pEndPoints; //���յ�
	CArray<CChartLineSerie*, CChartLineSerie*> m_arrBaseLines;//����
	CArray<CChartLineSerie*, CChartLineSerie*> m_arrDivideBaseLines;//��ֱ�ַ����
	CArray<CSuperChartString*, CSuperChartString*> m_arrAssistants;//������ʾ�еı���ʱ�䡢Ũ�ȡ������
	COLORREF m_crBaseLines, m_crDivideBaseLines, m_crAssistants;
	size_t m_nTemporaryGroupID;
	DWORD m_drawMask; //��ͼ��ʾѡ����

public:
	CMapStringToPtr m_mapCompSeries;//�Ա�����,ע�⣬һ��Ҫ��(CChartSerie*)��ʽ���
	CMapStringToPtr m_mapCurve2DVisibilites;
	//�����ߵ����г���������ʹ��
	CChartSerie*	m_pcCurveMain;
	CMapStringToPtr m_mapCurve2DSeries;//��Ʒ�е��������ߣ������������������,ע�⣬һ��Ҫ��CreateCurve2DSerie��ʽ
//	CChartSerie* m_pStartSerie;
//	CChartSerie* m_pStopSerie;
	CTWenChartPan* m_pYLabel;
	CTWenChartPan* m_pXLabel;
	CTWenChartPan* m_pYLabelBk;
	CTWenChartPan* m_pXLabelBk;


protected:
	//CBase_AnalysisSample* m_pSample;���಻��Ҫ��Ʒָ�룬����ֱ�ӱ���Ʒ������
	CChartSerie::enumSerieType m_eSerieType;
	void OnBackColorChanged();


	//
	//-------------------�ӿں���------------------------------------------
	//
public:
	//�����TRUE����ʾ������ʵʱ���ݲɼ�״̬
	// ���Խ���ʵʱ���ݣ�����ʾ
	// ���򲻽���ʵʱ����
	//BOOL	m_bRealtimeStatus;
	
	//
	//����Ʒ���ݳ�ʼ��
	//�жϸ���Ʒ��״̬�������...��ô��״̬תΪm_bRealtimeStatus=FALSE�����...״̬תΪm_bRealtimeStatus=TRUE
	//void Init(CBase_AnalysisSample* pSample, CChartSerie::enumSerieType eType = CChartSerie::stLineSerie);//����ڴ���֮�����
	void Init(CChartSerie::enumSerieType eType = CChartSerie::stLineSerie);//����ڴ���֮�����
	void SetDrawMask(DWORD dMask);
	void ResetAxis();
	
	
	// ˢ��״̬������bRealTimeStatusת����ǰ��m_bRealtimeStatus
	// ���״̬�����仯��ͼ����Ӧ��Ҫ�仯�����bRedrawΪFALSE����ô����״̬�仯�������²���
	// ��m_bRealtimeStatus��TRUE���FALSE��ʱ�������ǰͼ������Ԫ�أ��ػ���Ʒ��������
	// ��m_bRealtimeStatus��FALSE���TRUE��ʱ�������ǰͼ������Ԫ�أ��ȴ�ʵʱ���ݣ�
	// ���bRedrawΪTRUE����ô������ζ�ִ�����ϲ���
	//void UpdateStatus(BOOL bRealTimeStatus, BOOL bRedraw=FALSE);

	//�������������
	//�� m_bRealtimeStatus��TRUE �Ž��в��������򷵻�FALSE
	// �������̣�
	// 1) �������������
	// 2) ���� CRealTime::SetSemaDate(double x[], double y[], int n);
	// 3) ���� CRealTime::GetPeakResult(std::vector<CPeak> &Peak),Ȼ��Peak���ݻ�����ͼ����
	BOOL AddRealTimeCurveDataMain(double x[], double y[], int n); 
	BOOL AddRealTimeCurveDataMainWithProcess(CBase_AnalysisSample* pSample, bool isProcess, double x[], double y[], int n); //��������� ʵʱ���߻��� ����
	//dScale��ʾ�������ֵ��Ҫ���Զ��ٱ���
	//Offset��ʾ�������ֵ��Scale֮��Ҫ���Ӷ���
	BOOL AddRealTimeCurveDataMainWithProcess(CBase_AnalysisSample* pSample, bool isProcess, const CCurveData &curveData, double dScaleX, double dScaleY, double xOffset, double yOffset);
	//���������������
	BOOL AddRealTimeCurveData(const CString& group, const CString& serie, double x[], double y[], int n);

	//���ƿ�ʼ�ͽ�����
	//�� m_bRealtimeStatus��TRUE �Ž��в��������򷵻�FALSE
	// 1�ǿ�ʼ�ߣ�2�����ߣ�3�Ժ���ʱû����
	// �������꣬��ʼ�㣺���������һ��ĺ����ꣻ�����꣺���һ���������
	void AddMarkLine(int bStart);
};


class CParamCommon
{
public:
// 	CParamCommon(void);
// 	~CParamCommon(void);
// 
// public:
// 	static const CString& ThresholdString(double dblValue);
// 	static const CString& MinHalfPeakWidthString(double dblValue);
// 	static const CString& MinPeakAreaString(double dblValue);
// 	static const CString& MinPeakHeightString(double dblValue);
	static CString TimeString(double dblValue);
//	static const CString& ConcentrationString(double dblValue);
};
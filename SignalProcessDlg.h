// SignalProcessDlg.h : ͷ�ļ�
#pragma once
#include "afxwin.h"
#include <vector>
#include <list>
#include <utility>
#include "afxcmn.h"
#include "Peak.h"
#include "PeakSeak.h"
//#include "RealTime.h"
//#include "EditListCtrl.h"

// CSignalProcessDlg �Ի���
class CSignalProcessDlg : public CDialog
{
// ����
	enum SignalProStatus 
	{ 
		LOADFILE = 0,
		FILTER = 1,
		PEAKSEEK = 2,
	};
public:
	CSignalProcessDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CSignalProcessDlg();

// �Ի�������
	enum { IDD = IDD_SIGNALPROCESS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoadCsvFile();
	bool CStringToDouble(CStringArray &DataArr); //�ַ�ת����double��
	void vect2pointer(const std::vector<std::pair<double,double> > &Data); //vector����ת����ָ��
	void GetPeakValue(double x[], double y[], int n, std::vector<std::pair<double,double> > &PeakData); //��÷�ֵ

protected:
	SignalProStatus m_SignalProcessStatus;

private:
//----------------------------------------------------------------
// ����CPeak����Ϊ�ź�������
	std::vector<CSema> m_OrigSema; //ԭʼ����
	std::vector<CPeak> m_Peak;     //�������
	std::vector<CSema> m_TagPoint; //���ߵ�����±���
//----------------------------------------------------------------

	
	CSuperChartCtrl m_ChcarCtrlPeakSeek; 
	CString m_CsvFilePathName;

	CStringArray m_data;
	CChartLineSerie* m_pLine;				   //ԭʼ������
	std::vector<CChartLineSerie*> m_phemline;  //���ߵױ���
	std::list<CChartLineSerie*> m_pTagline;    //�����
	std::list<CChartLineSerie*> m_pDividePeakBaseLine;    //��ֱ�ַ��������
	CChartPointsSerie *m_pPoint; //����ʼ����յ�
	CChartPointsSerie *m_pPeak;//����ʼ����յ�
	CChartLineSerie* m_pLineFilter;  //�˲�������
	CChartLineSerie* m_pLinePeakSeek;//Ѱ�������
	double *pX; //ԭʼ����
	double *pY;

	std::vector<std::pair<double,double> > m_OriginalData; //ԭʼ����
	std::vector<std::pair<double,double> > m_ReshapeData;  //��������
	std::vector<std::pair<double,double> > m_WaveletData;  //С������
	std::vector<std::pair<double,double> > m_PeakData;     //������

	double *pDWTX; //С��������
	double *pDWTY;
	double dXMinVal;
	double dXMaxVal;
	double dYMinVal;
	double dYMaxVal;

	CRect m_rcChcarCtrl;          //1ԭʼͼ�ؼ�����
	//CRect m_rcChcarCtrlFilter;	  //2�˲�ͼ�ؼ�����
	//CRect m_rcChcarCtrlPeakSeek;  //3Ѱ��ͼ�ؼ�����
	

public:
	afx_msg void OnBnClickedFilterButton();
	afx_msg void OnBnClickedPeakseekButton();

private:
	CEdit m_MovingAvgTimes;

/*
   2010.05.26����С���㷨��
*/
public:
	double *m_pdSaveArray;

	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedSignalprocess();
	afx_msg void OnEnChangeMovingaveragetimes();
	afx_msg void OnRClickDatareport(NMHDR *pNMHDR, LRESULT *pResult); //����ؼ��һ�
	afx_msg void OnDblclkDatareport(NMHDR *pNMHDR, LRESULT *pResult); //����ؼ�˫��
private:
	CEdit m_HalfPeakWidth;
	CEdit m_Slope;
	CEdit m_SlopeFactor;
	CEdit m_PeakArea;
	CEdit m_RunTime;
	CListCtrl m_Report;  
	CListCtrl m_Config;  //���ò����б�
	CComboBox m_avgN;	 //�ڲ�����ƽ�����˳��Ӳ���
	CComboBox m_Series;  //���������жϣ�����ƽͷ��


public:
	void ClearLineOn3rdCtrl();
	void DrawLineOn3rdCtrl();
	void DrawBaseLineOn3rdCtrl(bool isDividePeak);
	void DrawDividePeakBaseLine(bool isDividePeak);
	void DrawTagLineOn3rdCtrl();
	void OutputReport();
	void OutputConfigArgs();// !new
	void OutputResultConfigArgs(); //new!
	void AddStringsOn3rdCtrl();

public:
	VOID AddNewPeak();	   //��������
	VOID DelOnePeak();	   //����ɾ��
	VOID SaveConfigParameter(CString &opt, const Para &para); //�������ò���
	VOID LoadConfigParameter(/*CString &opt, const Para &para*/); //�������ò���
	int  GetMoreAvgPara(); //���ø����ڲ�ƽ������
	int m_OptTimes;  //���ò����Ĳ�������
	int m_iavgN;


//�ӿڷ�װ����
	CWnd hControl; //��ǰ�����ڻ�ͼ�ؼ����
	CPeakSeak m_PeakSeak;
	CPeakDraw m_PeakDraw;
	//CRealTime m_RealTime;

//���ò����ṹ
	deque<ConfigArgument> m_ConfigArgs;

public:
	afx_msg void OnBnClickedStatrfinal();
	afx_msg void OnBnClickedStatrfinal2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeMoreAvgPara();
	//afx_msg void OnBnClickedFunctest();
	//afx_msg void OnBnClickedDateLoadTest();
	afx_msg void OnBnClickedReadshow();
	//afx_msg void OnBnClickedDeltest();
	//afx_msg void OnBnClickedColtest();
	//afx_msg void OnBnClickedShowhidetest();
	//afx_msg void OnRealTimeTest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnConfigArgsTest();

	
	afx_msg void OnConfigArgsSelchange();
	
	
	afx_msg void OnPreprocessConfigArgs();
	afx_msg void OnFirstScan();
	afx_msg void OnSecondScan();
	afx_msg void OnThirdScan();
	afx_msg void OnFourthScan();
	
	afx_msg void OnFifthScan();
	afx_msg void OnSixthScan();
	afx_msg void OnCbnDropdownSeries();
	afx_msg void OnBnClickedClearResult();
	afx_msg void OnBnClickedProcessPort1();
	
	afx_msg void OnBnClickedPreprocess();
};

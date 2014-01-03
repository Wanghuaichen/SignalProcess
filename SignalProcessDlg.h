// SignalProcessDlg.h : ͷ�ļ�
#pragma once
#include <vector>
#include <list>
#include <utility>
#include "afxwin.h"
#include "afxcmn.h"
#include "Peak.h"
#include "PeakSeak.h"


// CSignalProcessDlg �Ի���
class CSignalProcessDlg : public CDialog
{
public:
	CSignalProcessDlg(CWnd* pParent = NULL);
	~CSignalProcessDlg();

	enum { IDD = IDD_SIGNALPROCESS_DIALOG };

	//void ClearLineOn3rdCtrl();
	//void DrawLineOn3rdCtrl();
	//void DrawBaseLineOn3rdCtrl(bool isDividePeak);
	void DrawDividePeakBaseLine(bool isDividePeak);
	//void DrawTagLineOn3rdCtrl();
	void OutputReport();
	void OutputConfigArgs();
	void OutputResultConfigArgs(); 
	void AddStringsOn3rdCtrl();

	//VOID AddNewPeak();	   //��������
	//VOID DelOnePeak();	   //����ɾ��
	VOID SaveConfigParameter(CString &opt, const Para &para); //�������ò���
	VOID LoadConfigParameter(); //�������ò���
	int  GetMoreAvgPara(); //���ø����ڲ�ƽ������

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	HICON m_hIcon;

	afx_msg void OnLoadCsvFile();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedFilterButton();
	afx_msg void OnBnClickedPeakseekButton();
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedSignalprocess();
	afx_msg void OnEnChangeMovingaveragetimes();
	afx_msg void OnRClickDatareport(NMHDR *pNMHDR, LRESULT *pResult); //����ؼ��һ�
	afx_msg void OnDblclkDatareport(NMHDR *pNMHDR, LRESULT *pResult); //����ؼ�˫��
	afx_msg void OnBnClickedStatrfinal();
	afx_msg void OnBnClickedStatrfinal2();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeMoreAvgPara();
	afx_msg void OnBnClickedReadshow();
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

	DECLARE_MESSAGE_MAP()

public:
	bool CStringToDouble(CStringArray &DataArr); //�ַ�ת����double��
	void vect2pointer(const std::vector<std::pair<double,double> > &Data); //vector����ת����ָ��
	void GetPeakValue(double x[], double y[], int n, std::vector<std::pair<double,double> > &PeakData); //��÷�ֵ

private:
	std::vector<CSema> m_OrigSema; //ԭʼ����
	std::vector<CPeak> m_Peak;     //�������
	std::vector<CSema> m_TagPoint; //���ߵ�����±���
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

	CRect m_rcChcarCtrl; //1ԭʼͼ�ؼ�����

	int m_OptTimes;  //���ò����Ĳ�������
	int m_iavgN;
	
	CEdit m_MovingAvgTimes;
	double *m_pdSaveArray;

	CEdit m_HalfPeakWidth;
	CEdit m_Slope;
	CEdit m_SlopeFactor;
	CEdit m_PeakArea;
	CEdit m_RunTime;
	CListCtrl m_Report;  
	CListCtrl m_Config;  //���ò����б�
	CComboBox m_avgN;	 //�ڲ�����ƽ�����˳��Ӳ���
	CComboBox m_Series;  //���������жϣ�����ƽͷ��

    //�ӿڷ�װ����
	CWnd hControl; //��ǰ�����ڻ�ͼ�ؼ����
	CPeakSeak m_PeakSeak;
	CPeakDraw m_PeakDraw;

    //���ò����ṹ
	deque<ConfigArgument> m_ConfigArgs;
};

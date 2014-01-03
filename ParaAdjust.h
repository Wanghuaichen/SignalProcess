#pragma once
#include "afxwin.h"


// CParaAdjust �Ի���

class CParaAdjust : public CDialog
{
	DECLARE_DYNAMIC(CParaAdjust)

public:
	CParaAdjust(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CParaAdjust();

// �Ի�������
	enum { IDD = IDD_PARA_ADJUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


public:
	CEdit m_iPeakNo;
	CEdit m_dStartSema;
	CEdit m_dFinSema;

	int m_PeakNo;
	double m_StartSema;
	double m_FinSema;

	int m_DlgStyle;

public:
	afx_msg void OnBnClickedAdjustment();
	virtual BOOL OnInitDialog();
	CButton m_Button;
	afx_msg void OnClickedCancle();
};

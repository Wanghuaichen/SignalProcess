#pragma once
#include "afxwin.h"


// CSetAvgDlg �Ի���

class CSetAvgDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetAvgDlg)

public:
	CSetAvgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetAvgDlg();

// �Ի�������
	enum { IDD = IDD_SET_AVGPOINTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOK();
	afx_msg void OnBnClickedCancle();

public:
	CEdit m_ParaAvgTimes; 
	int m_iParaAvgTimes;
};

#pragma once
#include "afxwin.h"


// CShowHide �Ի���

class CShowHide : public CDialog
{
	DECLARE_DYNAMIC(CShowHide)

public:
	CShowHide(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowHide();

// �Ի�������
	enum { IDD = IDD_SHOWHIDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	CButton m_G1show;
	CButton m_G2show;
	afx_msg void OnBnClickedCancle();
	afx_msg void OnBnClickedOK();
	//VOID GetShowHide(bool &g1, bool &g2);

public:
	bool g1_IsShow;
	bool g2_IsShow;
	virtual BOOL OnInitDialog();
};

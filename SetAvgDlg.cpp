// SetAvgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SignalProcess.h"
#include "SetAvgDlg.h"


// CSetAvgDlg �Ի���

IMPLEMENT_DYNAMIC(CSetAvgDlg, CDialog)

CSetAvgDlg::CSetAvgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetAvgDlg::IDD, pParent)
{

}

CSetAvgDlg::~CSetAvgDlg()
{
}

void CSetAvgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ParaAvgTimes);
}


BEGIN_MESSAGE_MAP(CSetAvgDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetAvgDlg::OnBnClickedOK)
	ON_BN_CLICKED(IDC_BUTTON2, &CSetAvgDlg::OnBnClickedCancle)
END_MESSAGE_MAP()


// CSetAvgDlg ��Ϣ�������

void CSetAvgDlg::OnBnClickedOK()
{
	CString szParaAvgTimes;    //�����ڲ�ƽ������
	m_ParaAvgTimes.GetWindowText(szParaAvgTimes);
	m_iParaAvgTimes = _wtoi(szParaAvgTimes);
	CSetAvgDlg::OnOK();
}

void CSetAvgDlg::OnBnClickedCancle()
{
	CSetAvgDlg::OnCancel();
}

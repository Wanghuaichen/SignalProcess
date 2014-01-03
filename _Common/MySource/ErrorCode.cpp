#include "StdAfx.h"
#include "ErrorCode.h"



#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "Log_D.lib")
#else
#pragma comment(lib, "Log_R.lib")
#endif


///////////////////////////////////////////////////////////////////////////
// log���
///////////////////////////////////////////////////////////////////////////
CErrorLog CPAInformation::log;
CMFCCaptionBar* CPAInformation::m_pcCaptionBar = NULL;
CString CPAInformation::m_strMsgboxtitle;

CStringArray& CPAInformation_DefaultButtons()
{
	static CStringArray sa;
	if(sa.IsEmpty())
	{
		//sa.Add(_T("ȷ��"));
		sa.Add(_T("�ر�"));
	}
	return sa;
}

CString CPAInformation_ToFormatFixedLength(const CString& s, enumTextFormat eFormat)
{
	static int length = 40;
	if(s.GetLength() < length) return s;

	CString t;
	int l = 0;
	for(int i = 0; i<s.GetLength(); ++i)
	{
#if defined(UNICODE) || defined(_UNICODE)
		TCHAR c = s.GetAt(i);
		if(c < 256) ++l;
		else l += 2;
		t += c;
		if(l >= length)
		{
			l = 0;
			if(i != s.GetLength()-1)
			{
				switch(eFormat)
				{
					case TEXTFORMAT_TEXT:
						t += _T("\r\n      ");break;
					case TEXTFORMAT_HTML:
						t += _T("<br/>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");break;
				}
			}
		}
#endif
	}
	return t;
}
CString CPAInformation_ToFormat(enumTextFormat eFormat, LPCTSTR lpcszErrorText, LPCTSTR lpcszOperation, LPCTSTR lpcszWhere, LPCTSTR file, int line, LPCTSTR function)
{
	CString s, t;
	switch(eFormat)
	{
	case TEXTFORMAT_TEXT:
		if(lpcszErrorText){s += _T("��Ϣ��");s += CPAInformation_ToFormatFixedLength(lpcszErrorText, eFormat);s += _T("\r\n");}
		if(lpcszOperation){s += _T("������");s += CPAInformation_ToFormatFixedLength(lpcszOperation, eFormat);s += _T("\r\n");}
		if(lpcszWhere){s += _T("λ�ã�");s += CPAInformation_ToFormatFixedLength(lpcszWhere, eFormat);s += _T("\r\n");}
		if(file){s += _T("�ļ���");s += CPAInformation_ToFormatFixedLength(file, eFormat);s += _T("\r\n");}
		if(line > -1){s += _T("�кţ�");t.Format(_T("%d"), line);s += t;s += _T("\r\n");}
		if(function){s += _T("������");s += function;s += _T("\r\n");}
		return s;
	case TEXTFORMAT_HTML:
		if(lpcszErrorText){s += _T("<font color='blue'>��Ϣ��</font>");s += CPAInformation_ToFormatFixedLength(lpcszErrorText, eFormat);s += _T("<br/>");}
		if(lpcszOperation){s += _T("<font color='blue'>������</font>");s += CPAInformation_ToFormatFixedLength(lpcszOperation, eFormat);s += _T("<br/>");}
		if(lpcszWhere){s += _T("<font color='blue'>λ�ã�</font>");s += CPAInformation_ToFormatFixedLength(lpcszWhere, eFormat);s += _T("<br/>");}
		if(file){s += _T("<font color='blue'>�ļ���</font>");s += CPAInformation_ToFormatFixedLength(file, eFormat);s += _T("<br/>");}
		if(line > -1){s += _T("<font color='blue'>�кţ�</font>");t.Format(_T("%d"), line);s += t;s += _T("<br/>");}
		if(function){s += _T("<font color='blue'>������</font>");s += function;s += _T("<br/>");}
		return s;
	default:return _T("");
	}
}
///////////////////////////////////////////////////////////////////////////
CString CPAInformation::ErrorCode2String(ERRORCODE errorCode, void* pErrorArguments)
{
	CString ret;
	switch(errorCode)
	{
	case IAERROR_SUCCESS:ret.Format(_T("�ɹ�!"));break;
	case IAERROR_UNKNOWN:ret.Format(_T("δ֪����!"));break;
	case IAERROR_SYSTEMERROR:ret.Format(_T("ϵͳ�ڲ�����!"));break;

	case IAERROR_FILE_NOTFOUND:return(_T("δ�ҵ��ļ�"));break;
	case IAERROR_FILE_FORMATERROR:return(_T("�ļ���ʽ����"));break;
	case IAERROR_FILE_SAVEFAILURE:return(_T("�ļ����̳���"));break;

	case IAERROR_DB_NOTFOUND:return(_T("δ�ҵ����ݿ���"));break;

	case IAERROR_XML_INITERROR:return(_T("XML��ʼ������"));break;
	case IAERROR_XML_NOTVALID:return(_T("XML���ݲ��Ϸ�"));break;
	case IAERROR_XML_ITEMNOTFOUND:return(_T("XML��û���ҵ�"));break;
	case IAERROR_XML_SYSERROR:return(_T("XML����"));break;

	case IAERROR_PARAMETER_WRONGTYPE:return(_T("�������������"));break;

	case IAERROR_XML_NOTREADCHANNEL:return(_T("��ȡͨ���Ŵ���"));break;
	case IAERROR_CONTENTNOTNULL:return(_T("������ݲ���Ϊ��"));break;

	//ɫ�״�����Ϣ
	case IAERROR_CG_ACQUIRING:return(_T("���ڲ���"));break;
	case IAERROR_CG_ACQUIRED:return(_T("�����Ѿ�����"));break;
	case IAERROR_CG_ACQUIRER_NOTFOUND:return(_T("û���������ӿڳ���"));break;
	case IAERROR_CG_ACQUIRER_CANNOTRUN:return(_T("�����ӿڳ����޷���������"));break;
	case IAERROR_CG_ACQUIRER_NOTREADY:return(_T("�����ӿ�û��׼����"));break;

	case IAERROR_CG_SAMPLE_BUSY:return(_T("��Ʒ��æ���޷���Ӧ��ز���"));break;
	case IAERROR_CG_SAMPLE_RUNNING:return(_T("��Ʒ���ڲ������޷���Ӧ��ز���"));break;
	case IAERROR_CG_SAMPLE_DATAEXIST:return(_T("��Ʒ�����Ѿ����ڣ��޷���Ӧ��ز���"));break;
	case IAERROR_CG_SAMPLE_NOTREADY	:return(_T("��Ʒû��׼����"));break;
	case IAERROR_CG_SAMPLE_STATUSERROR:return(_T("��Ʒ״̬����"));break;
	case IAERROR_CG_INSTRUMENT_ERROR:return(_T("��������"));break;
	case IAERROR_CG_INSTRUMENT_BUSY:return(_T("�����������в���,���Ѿ���ռ��"));break;
	case IAERROR_SAMPLE_NODATA:return(_T("��Ʒû������"));break;

	case IAERROR_INSTRUMENT_NOTCONNECT:return(_T("����û������"));break;
	case IAERROR_INSTRUMENT_TEMPNOTREADY:return(_T("�����¶�û�ﵽ"));break;
	case IAERROR_INSTRUMENT_CHANNELERROR:return(_T("����ͨ������"));break;

	case IAERROR_INSTRUMENT_PARAMNOTREADY:return(_T("��������û�ﵽ�趨��׼"));break;
	case IAERROR_INSTRUMENT_PARAMSETERROR:return(_T("�޷��趨��������"));break;
	case IAERROR_INSTRUMENT_PARAMGETERROR:return(_T("�޷������������"));break;

	//��������
	case IAERROR_VALVE_COMM	:return(_T("����ͨѶ����"));break;
	default:ret.Format(_T("δ֪����(%d)!"), errorCode);
	}
	return ret;
}

CString CPAInformation::ToTString(LPCSTR s)
{
	USES_CONVERSION;
	return A2T(s);
}

///////////////////////////////////////////////////////////////////////////
void CPAInformation::Init(LPCTSTR logfile, LPCTSTR msgboxtitle, CMFCRibbonBaseElement* pStatusBar, CMFCCaptionBar* pCaptionBar)
{
	if(msgboxtitle) m_strMsgboxtitle = msgboxtitle;
	log.SetTextFormat(TEXTFORMAT_TEXT);
	log.SetMessageParser(ErrorCode2String);
	log.SetFile(logfile);
	log.SetParent(AfxGetMainWnd(), msgboxtitle, &CPAInformation_DefaultButtons());
	log.SetRibbonStatusBar(pStatusBar);
	m_pcCaptionBar = pCaptionBar;
}

void CPAInformation::ErrorLog(CWnd* parent, DWORD dwTarget, enumErrorLevel eErrorLevel, LPCTSTR lpcszErrorText, LPCTSTR lpcszOperation, LPCTSTR lpcszWhere, LPCTSTR file, int line, LPCTSTR function)
{
	DWORD dwOldTarget = dwTarget;
	dwTarget &= ~LOGTARGET_MESSAGEBOX;
	if(dwTarget) log.Report(CPAInformation_ToFormat(TEXTFORMAT_TEXT, lpcszErrorText, lpcszOperation, lpcszWhere, file, line, function), 
			dwTarget, eErrorLevel, false);

	if(dwOldTarget && LOGTARGET_MESSAGEBOX)
	{
		CString s = CPAInformation_ToFormat(TEXTFORMAT_HTML, lpcszErrorText, lpcszOperation, lpcszWhere, file, line, function);
		if(parent) log.Report2MessageBox(s, parent, m_strMsgboxtitle, &CPAInformation_DefaultButtons(), eErrorLevel);
		else log.Report2MessageBox(s, eErrorLevel);
	}
}
void CPAInformation::Alert(CWnd* parent, enumHTMLMessageBoxIcon eIcon, LPCTSTR lpcszErrorText, LPCTSTR lpcszOperation, LPCTSTR lpcszWhere)
{
	CString s = CPAInformation_ToFormat(TEXTFORMAT_HTML, lpcszErrorText, lpcszOperation, lpcszWhere, NULL, -1, NULL);
	HTMLMessageBox(parent, s, m_strMsgboxtitle, &CPAInformation_DefaultButtons(), eIcon);
}

BOOL CPAInformation::PostInformation(LPCTSTR lpcszMessage, LPCTSTR lpcszButtonFace, UINT uiCmdUI, HBITMAP hBitmap, COLORREF clrTransparent)
{
	if(!m_pcCaptionBar) return FALSE;

	//�����ͼ����ʾ
	if(hBitmap!=0) {
		m_pcCaptionBar->SetBitmap(hBitmap, clrTransparent, FALSE, CMFCCaptionBar::ALIGN_LEFT);
	}

	//��ʾ��Ϣ
	m_pcCaptionBar->SetText(lpcszMessage, CMFCCaptionBar::ALIGN_LEFT);


	//�����ťû�����֣�˵������Ҫ��ʾ��ť
	if (lpcszButtonFace == NULL || !*lpcszButtonFace) {
		m_pcCaptionBar->RemoveButton();
		m_pcCaptionBar->SetButton(_T(" "), 0, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	}
	//������Ҫ��ʾ��ť
	else {
		//m_pcCaptionBar->RemoveButton();//  [12/17/2012 ���] ��ֹ��˸
		m_pcCaptionBar->SetButton(lpcszButtonFace, uiCmdUI, CMFCCaptionBar::ALIGN_LEFT, FALSE);
	}

	return TRUE;
}
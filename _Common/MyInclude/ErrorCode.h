#pragma once



///////////////////////////////////////////////////////////////////////////
// ������
///////////////////////////////////////////////////////////////////////////

#define IAERROR_SUCCESS					0
#define IAERROR_UNKNOWN					255	//δ֪����
#define IAERROR_SYSTEMERROR				254	//ϵͳ�ڲ�����


#define IAERROR_FILE_NOTFOUND			1	//δ�ҵ��ļ�
#define IAERROR_FILE_FORMATERROR		2	//�ļ���ʽ����
#define IAERROR_FILE_SAVEFAILURE		3	//�ļ����̳���

#define IAERROR_DB_NOTFOUND				21	//δ�ҵ����ݿ���

#define IAERROR_XML_INITERROR			40	//XML��ʼ������
#define IAERROR_XML_NOTVALID			41	//XML���ݲ��Ϸ�
#define IAERROR_XML_ITEMNOTFOUND		42	//XML��û���ҵ�
#define IAERROR_XML_SYSERROR			43	//XML����

#define IAERROR_PARAMETER_WRONGTYPE		61	//�������������

#define IAERROR_XML_NOTREADCHANNEL      100 //��ȡͨ���Ŵ���
#define IAERROR_CONTENTNOTNULL          101 //������ݲ���Ϊ��

//ɫ�״�����Ϣ
#define IAERROR_CG_ACQUIRING			201 //���ڲ���
#define IAERROR_CG_ACQUIRED				202 //�����Ѿ�����
#define IAERROR_CG_ACQUIRER_NOTFOUND	203 //û���������ӿڳ���
#define IAERROR_CG_ACQUIRER_CANNOTRUN	204 //�����ӿڳ����޷���������
#define IAERROR_CG_ACQUIRER_NOTREADY	205 //�����ӿ�û��׼����

#define IAERROR_CG_SAMPLE_BUSY			206 //��Ʒ��æ���޷���Ӧ��ز���
#define IAERROR_CG_SAMPLE_RUNNING		207 //��Ʒ���ڲ������޷���Ӧ��ز���
#define	IAERROR_CG_SAMPLE_DATAEXIST		208	//��Ʒ�����Ѿ����ڣ��޷���Ӧ��ز���
#define	IAERROR_CG_SAMPLE_NOTREADY		209	//��Ʒû��׼����
#define	IAERROR_CG_SAMPLE_STATUSERROR	210	//��Ʒ״̬����
#define	IAERROR_CG_INSTRUMENT_ERROR		211	//��������
#define	IAERROR_CG_INSTRUMENT_BUSY		212	//�����������в���,���Ѿ���ռ��
#define	IAERROR_SAMPLE_NODATA			213	//��Ʒû������

#define	IAERROR_INSTRUMENT_NOTCONNECT	300	//����û������
#define	IAERROR_INSTRUMENT_TEMPNOTREADY	301	//�����¶�û�ﵽ
#define	IAERROR_INSTRUMENT_CHANNELERROR	302	//����ͨ������

#define	IAERROR_INSTRUMENT_PARAMNOTREADY 303	//��������û�ﵽ�趨��׼
#define	IAERROR_INSTRUMENT_PARAMSETERROR 304	//�޷��趨��������
#define	IAERROR_INSTRUMENT_PARAMGETERROR 305	//�޷������������


//��������
#define IAERROR_VALVE_COMM				0x87FE	//����ͨѶ����





///////////////////////////////////////////////////////////////////////////
// log���
///////////////////////////////////////////////////////////////////////////

#define SHP_DLL_LOG_RIBBON

#include <Log/Log.h>

SHP_DLL_LOG_NAMESPACE_USE;

// ע��鿴<Log/Log.h>�ļ��е�enumLogTargetö�������Ի��Log���������Ŀ��
// �������ṩ�����Ŀ�꣬���幤���оͲ��ܳ���ԭ��̬��ͬ������䣬����TRACE������塢MessageBox��AfxMessageBox��File��SetWindowText��


//�ڹ��ÿ��У�����ʹ��AfxMessageBox���MessageBox
//�ھ���Ĺ����У�����ʹ��HTMLMessageBox��ToHTMLMessageBox(ALERTϵ�к�)���MessageBox��AfxMessageBox
//�ھ���Ĺ����У���ΪLog����Ϣ����ʹ��DEBUGLOG��LOG��DEBUGLOGTHIS��LOGTHIS
// DEBUGLOGTHIS��LOGTHIS����DEBUGLOG��LOG�Ļ����������˵�ǰ����ָ��������ò���������LOGTARGET_MESSAGEBOX
// DEBUGLOG��DEBUGLOGTHIS����LOG��LOGTHIS�Ļ����������˵�ǰԴ�ļ����кźͺ����������
// ���û�����ֱ�Ӳ����Ĳ���������ΪlpcszOperation����������ΪlpcszWhere
// 
class CPAInformation
{
public:
	static CString ErrorCode2String(ERRORCODE errorCode, void* pErrorArguments);
	static CString ToTString(LPCSTR s);

public:
	static void Init(LPCTSTR logfile, LPCTSTR msgboxtitle, CMFCRibbonBaseElement* pStatusBar, CMFCCaptionBar* pCaptionBar);

	static void ErrorLog(CWnd* parent, DWORD dwTarget, enumErrorLevel eErrorLevel, LPCTSTR lpcszErrorText, LPCTSTR lpcszOperation, LPCTSTR lpcszWhere, LPCTSTR file, int line, LPCTSTR function);
	static void Alert(CWnd* parent, enumHTMLMessageBoxIcon eIcon, LPCTSTR lpcszErrorText, LPCTSTR lpcszOperation, LPCTSTR lpcszWhere);

	static BOOL PostInformation(LPCTSTR lpcszMessage, LPCTSTR lpcszButtonFace = NULL, UINT uiCmdUI = 0, HBITMAP hBitmap = 0, COLORREF clrTransparent = RGB(255,255,255));

public:
	static CErrorLog log;

private:
	static CMFCCaptionBar* m_pcCaptionBar;
	static CString m_strMsgboxtitle;
};


#define DEBUGLOG(dwTarget, eErrorLevel, lpcszErrorText, lpcszOperation, lpcszWhere) CPAInformation::ErrorLog(NULL, dwTarget, eErrorLevel, lpcszErrorText, lpcszOperation, lpcszWhere, CPAInformation::ToTString(__FILE__), __LINE__, CPAInformation::ToTString(__FUNCDNAME__))
#define DEBUGLOGTHIS(dwTarget, eErrorLevel, lpcszErrorText, lpcszOperation, lpcszWhere) CPAInformation::ErrorLog(this, dwTarget, eErrorLevel, lpcszErrorText, lpcszOperation, lpcszWhere, CPAInformation::ToTString(__FILE__), __LINE__, CPAInformation::ToTString(__FUNCDNAME__))

#define LOG(dwTarget, eErrorLevel, lpcszErrorText, lpcszOperation, lpcszWhere) CPAInformation::ErrorLog(NULL, dwTarget, eErrorLevel, lpcszErrorText, lpcszOperation, lpcszWhere, NULL, -1, NULL)
#define LOGTHIS(dwTarget, eErrorLevel, lpcszErrorText, lpcszOperation, lpcszWhere) CPAInformation::ErrorLog(this, dwTarget, eErrorLevel, lpcszErrorText, lpcszOperation, lpcszWhere, NULL, -1, NULL)

#define ALERT(eIcon, lpcszErrorText, lpcszOperation, lpcszWhere) CPAInformation::Alert(NULL, eIcon, lpcszErrorText, lpcszOperation, lpcszWhere)
#define ALERTTHIS(eIcon, lpcszErrorText, lpcszOperation, lpcszWhere) CPAInformation::Alert(this, eIcon, lpcszErrorText, lpcszOperation, lpcszWhere)

#define DISPifERROR(errCode, lpcszOperation, lpcszWhere)	{int __nRet = (errCode);if(__nRet!=IAERROR_SUCCESS) ALERT(ICON_CRITICAL, CPAInformation::ErrorCode2String(__nRet, NULL), lpcszOperation, lpcszWhere);}
#define DISPifERRORTHIS(errCode, lpcszOperation, lpcszWhere)	{int __nRet = (errCode);if(__nRet!=IAERROR_SUCCESS) ALERTTHIS(ICON_CRITICAL, CPAInformation::ErrorCode2String(__nRet, NULL), lpcszOperation, lpcszWhere);}
#define PASUCCESS(nRet) (nRet==IAERROR_SUCCESS)
#define RETURNifERROR(errCode)	{int __nRet = (errCode);if(__nRet!=IAERROR_SUCCESS) return __nRet;}
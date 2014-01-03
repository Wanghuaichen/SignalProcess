#pragma once

////////////////////////////////////////////////////////////////////////////////
//	����ȼ�
enum PAErrorLevel {
	pael_WriteDebugWindow=0,
	pael_WriteStatusBar,
	pael_WriteStatusWindow,
	pael_WriteLogFile,
	pael_PopMessage,
	pael_NeedRestart
};

////////////////////////////////////////////////////////////////////////////////
//	����ṹ
typedef struct tagPAError {
	int iErrorCode;
	int iErrorLevel;
	LPCTSTR lpcszErrorText;
}	PA_ERROR, *P_PA_ERROR;
typedef const PA_ERROR *PC_PA_ERROR;

//
//--------ϵͳ���ش������-------------------------------
//
#define IAERROR_SUCCESS					0
#define IAERROR_UNKNOWN					255	//δ֪����


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


//��������
#define IAERROR_VALVE_COMM				0x87FE	//����ͨѶ����
#define IAERROR_RGA_COMM				0x87FF	//�����ͨѶ����
#define IAERROR_RGA_BASE				0x8800
#define IAERROR_RGA_ERRCNT_11			(IAERROR_RGA_BASE|11)	//"Bad command received"
#define IAERROR_RGA_ERRCNT_12			(IAERROR_RGA_BASE|12)	//"Bad Parameter received"
#define IAERROR_RGA_ERRCNT_13			(IAERROR_RGA_BASE|13)	//"Command-too-long"
#define IAERROR_RGA_ERRCNT_14			(IAERROR_RGA_BASE|14)	//"OVERWRITE in receiving"
#define IAERROR_RGA_ERRCNT_15			(IAERROR_RGA_BASE|15)	//"Transmit buffer overwrite"
#define IAERROR_RGA_ERRCNT_16			(IAERROR_RGA_BASE|16)	//"Jumper protection violation"
#define IAERROR_RGA_ERRCNT_17			(IAERROR_RGA_BASE|17)	//"Parameter conflict"
#define IAERROR_RGA_ERRCNT_21			(IAERROR_RGA_BASE|21)	//"Single filament operation"
#define IAERROR_RGA_ERRCNT_26			(IAERROR_RGA_BASE|26)	//"Vacuum Chamber pressure too high"
#define IAERROR_RGA_ERRCNT_27			(IAERROR_RGA_BASE|27)	//"Unable to set the requested emission current"
#define IAERROR_RGA_ERRCNT_28			(IAERROR_RGA_BASE|28)	//"No filament detected"
#define IAERROR_RGA_ERRCNT_48			(IAERROR_RGA_BASE|48)	//"No Electron Multiplier Option installed"
#define IAERROR_RGA_ERRCNT_55			(IAERROR_RGA_BASE|55)	//"Power supply in current limited mode"
#define IAERROR_RGA_ERRCNT_57			(IAERROR_RGA_BASE|57)	//"Primary current exceeds 2.0A"
#define IAERROR_RGA_ERRCNT_58			(IAERROR_RGA_BASE|58)	//"RF_CT exceeds (V_EXT- 2V) at M_MAX"
#define IAERROR_RGA_ERRCNT_62			(IAERROR_RGA_BASE|62)	//"OP-AMP Input Offset Voltage out of range"
#define IAERROR_RGA_ERRCNT_64			(IAERROR_RGA_BASE|64)	//"COMPENSATE fails to read -5nA input current"
#define IAERROR_RGA_ERRCNT_65			(IAERROR_RGA_BASE|65)	//"COMPENSATE fails to read +5nA input current"
#define IAERROR_RGA_ERRCNT_66			(IAERROR_RGA_BASE|66)	//"DETECT fails to read -5nA input current"
#define IAERROR_RGA_ERRCNT_67			(IAERROR_RGA_BASE|67)	//"DETECT fails to read +5nA input current"
#define IAERROR_RGA_ERRCNT_68			(IAERROR_RGA_BASE|68)	//"ADC16 Test failure"
#define IAERROR_RGA_ERRCNT_77			(IAERROR_RGA_BASE|77)	//"Voltage <22V"
#define IAERROR_RGA_ERRCNT_78			(IAERROR_RGA_BASE|78)	//"Voltage >26V"

//����ȫ�ֱ����ĺ궨�壬����ʹ��
#define DISPifERROR(errCode,strWhere)	g_pcInformation->reportError(errCode,strWhere, 4)
#define PASUCCESS(nRet) (nRet==IAERROR_SUCCESS)
#define RETURNifERROR(errCode)	{int nRet =errCode;if(nRet!=IAERROR_SUCCESS) return nRet;}

//�˺������չ�������ļ��ε��ã��Ժ�Ҫȥ��������ʹ��
#define DisplayIfError(cStrWhere,nErrorCode)	g_pcInformation->reportError(nErrorCode,cStrWhere)


class AFX_EXT_CLASS CPASystemInformation
{
public:
	CPASystemInformation();
	virtual ~CPASystemInformation();

	void bindParentWindow(HWND hWnd) {m_hParentWnd = hWnd;}
	void SetStatusBar(CMFCRibbonBaseElement* pCWndStatusElement) {m_pCWndStatusElement = pCWndStatusElement;}
	void InitCaptionBar(CMFCCaptionBar	*pcCaptionBar){m_pcCaptionBar=pcCaptionBar;}
	void setLogPath(LPCTSTR lpcszPath) {StrCpy(m_szLogPath, lpcszPath);}
public:
	//CString GetErrorString (int nErrorCode){void* pcRet;m_cMapErrMsg.Lookup(nErrorCode, pcRet);return (PTCHAR)pcRet;}

	//�ú��������Ի�����ʾnErrorCode��Ӧ�Ĵ�����Ϣ,��ʽ����:
	// (...cStrWhere...)����: (...������Ϣ...)
	//ע�⵱nErrorCode==IAERROR_SUCCESS �������Ի���!
	//��������ֵ����nErrorCode
	//�ȽϷ�����÷�����: ����������YourFunction()���ش����ʱ����ʾ������Ϣ
	//
	//   DisplayInformationIfError (YourFunction(), _T("����ط�����"));
	//
	//int	DisplayInformationIfError(int nErrorCode, CString cStrWhere=_T(""));

// 	BOOL	PostInformation(int nErrorCode,					//������붨��,IAERROR_...
// 							CString cStrButtonFace=_T(""),	//���Ҫ���밴ť,���ð�ť����ʾ�ַ�
// 							UINT uiCmdUI=0,					//CMaimFrame����Ӧ���������
// 							HBITMAP hBitmap=0,				//���Ҫ��ʾͼ��,ͼ��ľ��
// 							COLORREF clrTransparent=RGB(255,255,255)	//ͼ���е�͸��ɫ
// 							);
	//������ĺ���һ��,ֻ��ֱ�Ӵ��������Ϣ�ַ���cStrMessage,���ô������
	BOOL PostInformation(LPCTSTR lpcszMessage, LPCTSTR lpcszButtonFace = NULL, UINT uiCmdUI = 0, HBITMAP hBitmap = 0, COLORREF clrTransparent = RGB(255,255,255));
	void SetMsgBoxInfo(HINSTANCE hInstance, LPCTSTR lpszIcon);//AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDR_MAINFRAME)

private:
	HINSTANCE m_hInstance_MsgBoxInfo;
	LPCTSTR m_lpszIcon_MsgBoxInfo;
	//CMapWordToPtr	m_cMapErrMsg;


public:
	void SetErrorInfo(PA_ERROR	*pcErrorInf, int nNumber){m_nErrorNumber=nNumber;m_pcErrorInf=pcErrorInf;}
	int reportError(int iErrorCode, LPCTSTR lpcszWhere = NULL, int iErrorLevel = 4, LPCTSTR lpcszFilename = NULL, int iFileLine = -1, UINT uCmdID = 0, LPCTSTR lpcszCmdText = NULL);
	void reportSpecialError(LPCTSTR lpcszErrorText, LPCTSTR lpcszWhere = NULL, PAErrorLevel eErrorLevel = pael_WriteDebugWindow, LPCTSTR lpcszFilename = NULL, int iFileLine = -1, UINT uCmdID = 0, LPCTSTR lpcszCmdText = NULL);
	int reportSysError(int iSysErrorCode, LPCTSTR lpcszWhere = NULL, PAErrorLevel eErrorLevel = pael_WriteDebugWindow, LPCTSTR lpcszFilename = NULL, int iFileLine = -1, UINT uCmdID = 0, LPCTSTR lpcszCmdText = NULL);
protected:
	enum {UserType, SpecialType, SysType};
	void processError(int iErrorType, int iErrorCode, LPCTSTR lpcszWhere, int iErrorLevel, LPCTSTR lpcszFilename, int iFileLine, UINT uCmdID, LPCTSTR lpcszCmdText);
	void saveLog(LPSYSTEMTIME lptNowTime, LPCTSTR lpcszContent);
private:
	HWND m_hParentWnd;		//������
	CMFCRibbonBaseElement* m_pCWndStatusElement;//״̬����Ԫ��
	CMFCCaptionBar* m_pcCaptionBar;//��Ϣ����
	TCHAR m_szLogPath[MAX_PATH];	//��־Ŀ¼
public:
	//static PC_PA_ERROR findError(int iErrorCode);
	PC_PA_ERROR findError(int iErrorCode);

	//��������ˣ��滻Ϊ�����Ǹ���ͨ�ð汾
	//static const PA_ERROR l_tErrorInfo[];

	PA_ERROR	*m_pcErrorInf;
	int			m_nErrorNumber;

};



#pragma once


//#include "AnalysisMethod.h"
#include "Base_AnalysisSample.h"
#include "Base_AnalysisInstrument.h"
//#include "AnalysisInject.h"


//
//���������һ����ϣ���������+������Ʒ+��������
//ͬʱ����������������
//
//ͬһ��ģ���²�������ͬ�ķ�������Ʒ����

enum enumPADATATYPENAME
{
	PA_DATATYPENAME_DATA			=1,		//��������Ŀ¼
	PA_DATATYPENAME_SAMPLE			=2,		//��Ʒ����Ŀ¼
	PA_DATATYPENAME_TASK			=3,		//�����ļ�Ŀ¼
	PA_DATATYPENAME_COMPOUND		=4,		//��������Ϣ��Ŀ¼
	PA_DATATYPENAME_METHOD			=5,		//����Ŀ¼
	PA_DATATYPENAME_LOG				=6,		//��־����Ŀ¼
	PA_DATATYPENAME_CRYSTALREPORT	=7,

	PA_DATATYPENAME_DEFAULTMETHOD	=6,		//�����ļ�
	PA_DATATYPENAME_INJECT			=7,		//�����������ļ�
	PA_DATATYPENAME_INSTRUMENT		=8,		//���������ļ�
};


class CPAWindowTask;
class CPAWindowMain;

struct CInstrument2Sample {
	CBase_AnalysisInstrument *m_pcInstrument;
	int m_nChannel;
	CBase_AnalysisSample *m_pcSample;

	CInstrument2Sample(): m_pcSample(NULL), m_pcInstrument(NULL), m_nChannel(0){}

};

typedef CArray <CInstrument2Sample, CInstrument2Sample&> CArrayI2S;



class CConfigIni 
{
public:
	struct structSystem
	{
		CString m_szWindowName;
		CString m_szProgramName;
		CString m_szMessageName;
		CString m_szSignalName;
		CString m_szXSLFolder;
		CString m_szPathData;
		CString m_szPathTask;
		CString m_szPathSample;
		CString m_szPathCompound;
		CString m_szPathMethod;
		CString m_szPathCrystalReport;
		CString m_szPathLog;
		CString m_szDefaultMethod;
		CString m_szInject;
		CString m_szInstrument;
		CString m_szLastTask;
		int m_iRGACOM;
		int m_iValveCOM;
		int m_iPumpCOM;
		int m_iNextSampleID;
		BOOL m_bShowAutoSampler;

		structSystem():m_iRGACOM(0),m_iValveCOM(0),m_iPumpCOM(0),m_iNextSampleID(0),m_bShowAutoSampler(false){}
	};

public:
	CConfigIni();
	virtual ~CConfigIni();

	bool LoadConfig(LPCTSTR strIni = NULL);
	bool SaveConfig(LPCTSTR strIni = NULL) const;

	inline const CString& CurrentSystemName() const{return m_szName;}
	bool SetCurrentSystemName(const CString& strSystem, bool bRefresh);
	
	inline const structSystem& CurrentSystem() const{return m_System;}
	inline structSystem& CurrentSystem(){return m_System;}

	bool LoadSections(CStringArray& arrSections) const;
	bool LoadInstruSections(CStringArray& arrSections) const;

	bool LoadString(const CString& strSection, const CString& strItem, CString& value) const;
	bool LoadPath(const CString& strSection, const CString& strItem, CString& value) const;
	bool LoadInt(const CString& strSection, const CString& strItem, int& value) const;
	
	bool SaveString(const CString& strSection, const CString& strItem, const CString& value) const;
	bool SavePath(const CString& strSection, const CString& strItem, const CString& value) const;
	bool SaveInt(const CString& strSection, const CString& strItem, int value) const;

	bool SaveSystemName(const CString& strSystem) const;

	const CString& ConfigPath() const{return m_szPath;}

private:
	CString m_szName;
	structSystem m_System;
	CString m_szIni;
	CString m_szPath;

protected:
	bool LoadSystem(const CString& systemName, structSystem& system);
	bool SaveSystem(const CString& systemName, const structSystem& system) const;
	bool LoadCurrentSystem();

	static const TCHAR l_szIniFilename[];
	static const TCHAR l_szIniKey_Sys[];
	static const TCHAR l_szIniItem_Sys[];
	static const TCHAR l_szIniItem_PathData[];
	static const TCHAR l_szIniItem_PathTask[];
	static const TCHAR l_szIniItem_PathSample[];
	static const TCHAR l_szIniItem_PathCompound[];
	static const TCHAR l_szIniItem_PathMethod[];
	static const TCHAR l_szIniItem_PathCrystalReport[];	
	static const TCHAR l_szIniItem_PathLog[];
	static const TCHAR l_szIniItem_DefaultMethod[];
	static const TCHAR l_szIniItem_Inject[];
	static const TCHAR l_szIniItem_Instrument[];
	static const TCHAR l_szIniItem_RGACOM[];
	static const TCHAR l_szIniItem_ValveCOM[];
	static const TCHAR l_szIniItem_PumpCOM[];
	static const TCHAR l_szIniItem_NextSampleID[];	
	static const TCHAR l_szIniItem_WindowName[];
	static const TCHAR l_szIniItem_ProgramName[];
	static const TCHAR l_szIniItem_MessageName[];
	static const TCHAR l_szIniItem_SignalName[];
	static const TCHAR l_szIniItem_XSLFolder[];
	static const TCHAR l_szIniItem_LastTask[];
	static const TCHAR l_szIniItem_ShowAutoSampler[];
};

class CBase_AnalysisTemplate
{
public:
	CBase_AnalysisTemplate(CString cStrSystemName);
	virtual ~CBase_AnalysisTemplate();

	BOOL m_bValid;
	BOOL IsValid(void){return m_bValid;}

protected:

	//
	//----------ҵ�����ݲ���-------------------------------------
	//
	//ϵͳ����
	CString				m_cStrSystemName;

	//������ָ��
	CBase_AnalysisInstrument	*m_pcInstrment;
	//��ǰ��Ʒ��ָ��
	CBase_AnalysisSample		*m_pcCurrentSample;
	//��ǰ��������ָ��
	CBase_AnalysisMethod		*m_pcCurrentMethod;
	//���������ò���
	CBase_AnalysisInject		*m_pcInject;
	//�ɼ���ָ��
	//CBase_AnalysisAcquirer		*m_pcAcquirer;

	//��������Ʒ������
	CArrayI2S	m_cArrayInstrument2Sample;

	// [2013-01-16 chainyu]
	CMapStringToPtr m_mapSample; //�Աȴ���Ʒ�ļ��Ķ���ָ��
	//int m_pSampleRefCount; // ��Ʒָ������ü���
	//virtual BOOL ParseSampleFileFormat(CString& strPath);


public:
	CString	GetSystemName(void) {return m_cStrSystemName;}

	CBase_AnalysisInstrument	*GetInstrument(void)	{return m_pcInstrment;}
	CBase_AnalysisSample		*GetCurrentSample(void)	{return m_pcCurrentSample;}
	CBase_AnalysisMethod		*GetCurrentMethod(void)	{return m_pcCurrentMethod;}
	CBase_AnalysisInject		*GetInject()			{return m_pcInject;}
//	CBase_AnalysisAcquirer		*GetAcquirer()			{return m_pcAcquirer;}

	//
	//----------------------------��Ҫ������ָ��------------------------------
	//
	CPAWindowTask			*m_pcWindowTask;//���񴰿�
	CPAWindowMain			*m_pcWindowMain;//������ӿ�
public:
	void SetMainWnd(CPAWindowMain* pcWnd){ m_pcWindowMain=pcWnd; }
	void SetTaskWnd(CPAWindowTask* pcWnd){ m_pcWindowTask=pcWnd; }

	//
	//���л�������
	//
public:
	CString	GetRuntimeForlder(void);       // �õ���ǰ������Ŀ¼���磺 d:\run\ ...
	CString GetDataFolder(enumPADATATYPENAME nDataType);
	CString GetDataFilename(enumPADATATYPENAME nDataType);

	// 
	//----------------------------�����йز���--------------------------------------------
	// 
	BOOL ChangeCurrentTask(CString cStrTaskName);//{m_pcWindowMain->Nodify_CurrentTaskChanged(cStrTaskName);}

	//
	//----------------------------�����йز���--------------------------------------------
	//
	CBase_AnalysisMethod	*CreateNewMethod(CString cStrName, CString cStrNameFrom=_T(""));//cStrNameFrom==_T("")��ʾȱʡ����//�����߸����ͷ�
	CBase_AnalysisMethod	*LoadMethod(CString cStrMethodName);//�����߸����ͷ�ָ��
	BOOL GetMethodNameList(CStringArray &arrayMethod){return m_pcCurrentMethod->GetMethodNameList(this, arrayMethod);}
	BOOL ChangeCurrentMethod(CString cStrMethod, BOOL bSaveOldMethod=TRUE);//Ѱ�Ҹ������µķ�����ԭ�з�����ʾ���̣����ø÷�����Run()
	BOOL ChangeCurrentMethod(CBase_AnalysisMethod* pMethod, BOOL bSaveOldMethod=TRUE);

	//
	//------------------------��Ʒ�йز���-------------------------------------------
	//

	//��Ʒ��ʼ������ʾ
	virtual CBase_AnalysisSample *CreateNewSample(CString cStrName, int nInjectNO, CString cStrNameFrom)=0;	//�����߸����ͷ�
	//virtual void DestroySample(CBase_AnalysisSample *pcSample){delete pcSample;}
	virtual CBase_AnalysisSample *LoadSample(CString cStrSampleName, int nInjectNO)=0;						//������Ʒ�������߸����ͷ�pcSample
	virtual CBase_AnalysisSample *LoadSampleFile(CString cStrSampleFullPathFileName)=0;						//������Ʒ�������߸����ͷ�pcSample
	virtual CMDIChildWnd *Sample_LoadAndShow(CString cStrSampleName,int nInjectNo)=0;		//Ҫ����ʾ��Ʒ���������Ƶ��룩
	virtual CMDIChildWnd *Sample_Show(CBase_AnalysisSample *pcSample, BOOL bFreeTheSamplePointer=FALSE)=0;		//Ҫ����ʾ��Ʒ

//	virtual void Sample_ParamChanged(CString cStrParamXMLPath){AfxMessageBox(_T("�˺�����Ҫ�ϳ�����Notify_SampleDataChanged���棡"));}

	//��Ʒ����
	virtual void Sample_Reset(CBase_AnalysisSample *pcSample);	//Ҫ����Ʒ����������ݣ�״̬�ص�null
	virtual int Sample_LinkToInstrument(CBase_AnalysisSample *pcSample, CBase_AnalysisInstrument *pcInstrument, int nChannel);
	virtual int Sample_PrepareAcquire(CBase_AnalysisSample *pcSample);	//��Ʒ׼�����в���
	virtual int Sample_StartRequire(CBase_AnalysisSample *pcSample);	//Ҫ����Ʒ��ʼ����
	virtual int Sample_AbortReady(CBase_AnalysisSample *pcSample);	//Ҫ����ֹ��Ʒ׼��
	virtual int Sample_AbortRun(CBase_AnalysisSample *pcSample)=0;	//Ҫ����ֹ��Ʒ����
	virtual int Sample_StopRun(CBase_AnalysisSample *pcSample);	//Ҫ����Ʒ��������
	virtual int Sample_ReleaseFromInstrument(CBase_AnalysisSample *pcSample);
	virtual int Sample_DataProcess(CBase_AnalysisSample *pcSample);//Ҫ����Ʒ�������ݴ���
	virtual int Sample_ReportPrint(CBase_AnalysisSample *pcSample, BOOL bAuto=TRUE)=0;//Ҫ����Ʒ���б����ӡ���,bAuto=TRUE��ʾֱ��ȱʡ����������˹���Ԥ
	virtual int Sample_Close(CString cStrSampleName)=0;				//�ر���Ʒ
	virtual int Sample_Close(CBase_AnalysisSample *pcSample)=0;		//�ر���Ʒ
	//������Ʒ��ʾ��pcStrParamPath����һ���ֲ���
	virtual void Sample_UpdateView(CBase_AnalysisSample *pcSample, const CBase_AnalysisSample::CParamChanged &itemChanged)=0;				//������Ʒ��ʾ

	virtual BOOL ChangeCurrentSample(CBase_AnalysisSample *pcNewSample);
	virtual BOOL CurrentSample_Changed(CBase_AnalysisSample *pcNewSample);//  [12/17/2012 ���] �ú���Ӧ����OnMDIActivate�е��ã�������ǰ��Ʒ�Ѿ��ı䣬�����ǽ�Ҫ�ı�

	//��Ʒ�¼�
	//
	//------------------------�����йز���--------------------------
	//
	virtual CBase_AnalysisInstrument *CreateNewInstrument(void)=0;//{return new CBase_AnalysisInstrument(this);}//�����߸����ͷ�
	virtual CBase_AnalysisInject* CreateNewInject()=0;// {return new CBase_AnalysisInject(_T("ɫ�׽�����"),this);}

	//------------------��������ĺ������̾���ȷ��-------------------
	virtual int Instrument_SetParam(CBase_AnalysisInstrument *pcInstrument, CBase_AnalysisSample *pcSample);
	virtual int Instrument_SetParam(CBase_AnalysisInstrument *pcInstrument, const CString & cStrXML, const CString & cStrFullXMLpath);

	virtual void SendRealTimeData(CBase_AnalysisInstrument *pcInstrument, int nChannel, CCurveData &curveData);

	//֪ͨ��Ϣ
	void Notify_SampleDead(CBase_AnalysisSample *pcSample);
	void Notify_InstrumentDead(CBase_AnalysisInstrument *pcInstrument);
	void Notify_StartSignal(CBase_AnalysisInstrument *pcInstrument, int nChannel, DWORD dwTime);
	void Notify_SampleStatusChanged(CBase_AnalysisSample *pcSample);
	void Notify_SampleDataChanged(CBase_AnalysisSample *pcSample, const CBase_AnalysisSample::CParamChanged &itemChanged){Sample_UpdateView(pcSample, itemChanged);}

	void ZeroFlag(CBase_AnalysisInstrument *pcInstrument, bool bZero);
};


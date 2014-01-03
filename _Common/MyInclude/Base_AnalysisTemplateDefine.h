
#pragma once


#include "ConfigIni.h"

#include <memory>

#ifndef TSharedPtr
#define TSharedPtr std::tr1::shared_ptr
#endif TSharedPtr


class CBase_AnalysisMethod;
class CBase_AnalysisSample;
class CBase_AnalysisInstrument;
class CBase_AnalysisInject;
class CPAWindowTask;
class CPAWindowMain;


enum enumPADATATYPENAME
{
	PA_DATATYPENAME_DATA			=1,		//��������Ŀ¼
	PA_DATATYPENAME_SAMPLE			=2,		//��Ʒ����Ŀ¼
	PA_DATATYPENAME_TASK			=3,		//�����ļ�Ŀ¼
	PA_DATATYPENAME_COMPOUND		=4,		//��������Ϣ��Ŀ¼
	PA_DATATYPENAME_METHOD			=5,		//����Ŀ¼
	PA_DATATYPENAME_LOG				=6,		//��־����Ŀ¼

	PA_DATATYPENAME_DEFAULTMETHOD	=6,		//�����ļ�
	PA_DATATYPENAME_INJECT			=7,		//�����������ļ�
	PA_DATATYPENAME_INSTRUMENT		=8,		//���������ļ�
};

struct CInstrument2Sample
{
	CBase_AnalysisInstrument *m_pcInstrument;
	int m_nChannel;
	CBase_AnalysisSample *m_pcSample;

	CInstrument2Sample(): m_pcSample(NULL), m_pcInstrument(NULL), m_nChannel(0){}
};
typedef CArray <CInstrument2Sample, CInstrument2Sample&> CArrayI2S;

///////////////////////////////////////////////////////////////////////////
// CAnalysisConfig
///////////////////////////////////////////////////////////////////////////
class CAnalysisConfig : public CConfigIni
{
public:
	struct structSystem
	{
		CString WindowName;
		CString ProgramName;
		CString MessageName;
		CString SignalName;
		CString XSLFolder;
		CString PathData;
		CString PathTask;
		CString PathSample;
		CString PathCompound;
		CString PathMethod;
		CString PathLog;
		CString DefaultMethod;
		CString Inject;
		CString Instrument;
		int NextSampleID;

		structSystem():NextSampleID(0){}
	};

public:
	CAnalysisConfig(const CString& strIni);
	~CAnalysisConfig();
	
	void Reload();//���ļ��м���CurrentSystemName��System
	void Save() const;//��CurrentSystemName��System���浽�ļ�

	bool GetInstruSections(CStringArray& arrSections) const;

protected:
	bool LoadSystem(const CString& systemName, structSystem& system);
	bool LoadCurrentSystem();

public:
	CString CurrentSystemName;
	structSystem System;
};


//���������һ����ϣ���������+������Ʒ+��������
//ͬʱ����������������
///////////////////////////////////////////////////////////////////////////
// CBase_AnalysisTemplate
///////////////////////////////////////////////////////////////////////////

class CBase_AnalysisTemplate
{
public:
	CBase_AnalysisTemplate(const CString& cStrSystemName);
	virtual ~CBase_AnalysisTemplate();

public:
	BOOL IsValid() const{return m_bValid;}

	const CString& GetSystemName() const{return m_cStrSystemName;}

	CBase_AnalysisMethod* GetCurrentMethod(){return m_pcCurrentMethod;}
	CBase_AnalysisSample* GetCurrentSample(){return m_pcCurrentSample;}
	CBase_AnalysisInstrument* GetInstrument(){return m_pcInstrment;}
	CBase_AnalysisInject* GetInject(){return m_pcInject;}
	
	void SetMainWnd(CPAWindowMain* pcWnd){ m_pcWindowMain=pcWnd; }
	void SetTaskWnd(CPAWindowTask* pcWnd){ m_pcWindowTask=pcWnd; }

	virtual TSharedPtr<CBase_AnalysisInstrument> NewInstrument() = 0;
	virtual TSharedPtr<CBase_AnalysisInject> NewInject() = 0;

	const CStringArray& GetWorkFolders() const;//�����ƶ���Config�У�
	
	CString GetSampleFolder(const CString& strWorkFolder) const;
	CString GetMethodFolder(const CString& strWorkFolder) const;

	//AnalysisFile��صĺ�����������ȫ·����Ϊ����
	TSharedPtr<CBase_AnalysisSample> LoadSample(const CString& strPathName) = 0;//������ڴ��в����ڣ����Զ�����
	TSharedPtr<CBase_AnalysisSample> NewSample(const CString& strPathName) = 0;

	TSharedPtr<CBase_AnalysisMethod> LoadMethod(const CString& strPathName) = 0;//������ڴ��в����ڣ����Զ�����
	TSharedPtr<CBase_AnalysisMethod> NewMethod(const CString& strPathName) = 0;

	BOOL ChangeCurrentTask(CString cStrTaskName);
	BOOL ChangeCurrentMethod(CBase_AnalysisMethod* pMethod, BOOL bSaveOldMethod = TRUE);//Ѱ�Ҹ������µķ�����ԭ�з�����ʾ���̣����ø÷�����Run()
	virtual BOOL ChangeCurrentSample(CBase_AnalysisSample *pcNewSample);

protected:
	BOOL						m_bValid;
	CString						m_cStrSystemName;
	CBase_AnalysisMethod		*m_pcCurrentMethod;
	CBase_AnalysisSample		*m_pcCurrentSample;
	CBase_AnalysisInject		*m_pcInject;
	CBase_AnalysisInstrument	*m_pcInstrment;
	CPAWindowTask				*m_pcWindowTask;//���񴰿�
	CPAWindowMain				*m_pcWindowMain;//������ӿ�
	CArrayI2S					m_cArrayInstrument2Sample;//��������Ʒ������









	
public:
	CString	GetRuntimeForlder(void);       // �õ���ǰ������Ŀ¼���磺 d:\run\ ...
	CString GetDefaultDataFolder(enumPADATATYPENAME nDataType);
	CString GetDataFilename(enumPADATATYPENAME nDataType);

	//------------------------��Ʒ�йز���-------------------------------------------
	//

	//��Ʒ��ʼ������ʾ
	virtual CMDIChildWnd *Sample_LoadAndShow(CString cStrSampleName,int nInjectNo)=0;		//Ҫ����ʾ��Ʒ���������Ƶ��룩
	virtual CMDIChildWnd *Sample_Show(CBase_AnalysisSample *pcSample, BOOL bFreeTheSamplePointer=FALSE)=0;		//Ҫ����ʾ��Ʒ

	//��Ʒ����
	virtual void Sample_Reset(CBase_AnalysisSample *pcSample);	//Ҫ����Ʒ����������ݣ�״̬�ص�null
	virtual int Sample_LinkToInstrument(CBase_AnalysisSample *pcSample, CBase_AnalysisInstrument *pcInstrument, int nChannel);
	virtual int Sample_PrepareAcquire(CBase_AnalysisSample *pcSample);	//��Ʒ׼�����в���
	virtual int Sample_StartRequire(CBase_AnalysisSample *pcSample);	//Ҫ����Ʒ��ʼ����
	virtual int Sample_AbortRun(CBase_AnalysisSample *pcSample)=0;	//Ҫ����ֹ��Ʒ����������FALSE��ʾ�޷���ֹ
	virtual int Sample_StopRun(CBase_AnalysisSample *pcSample);	//Ҫ����Ʒ��������������FALSE��ʾ�޷������������״̬
	virtual int Sample_ReleaseFromInstrument(CBase_AnalysisSample *pcSample);
	virtual int Sample_DataProcess(CBase_AnalysisSample *pcSample);//Ҫ����Ʒ�������ݴ���
	virtual int Sample_ReportPrint(CBase_AnalysisSample *pcSample, BOOL bAuto=TRUE)=0;//Ҫ����Ʒ���б����ӡ���,bAuto=TRUE��ʾֱ��ȱʡ����������˹���Ԥ
	virtual int Sample_Close(CString cStrSampleName)=0;				//�ر���Ʒ
	virtual int Sample_Close(CBase_AnalysisSample *pcSample)=0;		//�ر���Ʒ
	//������Ʒ��ʾ��pcStrParamPath����һ���ֲ���
	virtual void Sample_UpdateView(CBase_AnalysisSample *pcSample, const CBase_AnalysisSample::CParamChanged &itemChanged)=0;				//������Ʒ��ʾ


	//��Ʒ�¼�

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
};


#include "stdafx.h"

#include "Base_AnalysisTemplate.h"
#include "CStringExt.h"
//#include "ChildFrmChart.h"
//#include "CMTG2009.h"

//extern CPPMass_AcquireApp theApp;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern bool SelectCurrentSystem(CString& strSystemName);
extern void g_TracePersistentlyWithTime(LPCTSTR s);

enum {MAX_LENGTH_SECTION_OR_NODE = 64000};

const TCHAR CConfigIni::l_szIniFilename[] = _T("Config.ini");
const TCHAR CConfigIni::l_szIniKey_Sys[] = _T("CurrentSystem");
const TCHAR CConfigIni::l_szIniItem_Sys[] = _T("Name");
const TCHAR CConfigIni::l_szIniItem_PathData[] = _T("PathData");
const TCHAR CConfigIni::l_szIniItem_PathTask[] = _T("PathTask");
const TCHAR CConfigIni::l_szIniItem_PathSample[] = _T("PathSample");
const TCHAR CConfigIni::l_szIniItem_PathCompound[] = _T("PathCompound");
const TCHAR CConfigIni::l_szIniItem_PathMethod[] = _T("PathMethod");
const TCHAR CConfigIni::l_szIniItem_PathCrystalReport[] = _T("PathCrystalReport"); //ˮ�������ļ���[Report]
const TCHAR CConfigIni::l_szIniItem_PathLog[] = _T("PathLog");
const TCHAR CConfigIni::l_szIniItem_DefaultMethod[] = _T("DefaultMethod");
const TCHAR CConfigIni::l_szIniItem_Inject[] = _T("Inject");
const TCHAR CConfigIni::l_szIniItem_Instrument[] = _T("Instrument");
const TCHAR CConfigIni::l_szIniItem_RGACOM[] = _T("RGACOM");
const TCHAR CConfigIni::l_szIniItem_ValveCOM[] = _T("ValveCOM");
const TCHAR CConfigIni::l_szIniItem_PumpCOM[] = _T("PumpCOM");
const TCHAR CConfigIni::l_szIniItem_NextSampleID[] = _T("NextSampleID");
const TCHAR CConfigIni::l_szIniItem_WindowName[] = _T("WindowName");
const TCHAR CConfigIni::l_szIniItem_ProgramName[] = _T("ProgramName");
const TCHAR CConfigIni::l_szIniItem_MessageName[] = _T("MessageName");
const TCHAR CConfigIni::l_szIniItem_SignalName[] = _T("SignalName");
const TCHAR CConfigIni::l_szIniItem_XSLFolder[] = _T("XSLFolder");
const TCHAR CConfigIni::l_szIniItem_LastTask[] = _T("LastTask");
const TCHAR CConfigIni::l_szIniItem_ShowAutoSampler[] = _T("ShowAutoSampler");



CConfigIni::CConfigIni(){LoadConfig(NULL);}
CConfigIni::~CConfigIni(){SaveConfig(NULL);}


bool CConfigIni::LoadSections(CStringArray& arrSections) const
{
	if(m_szIni.IsEmpty()) return false;
	TCHAR szTmpName[MAX_LENGTH_SECTION_OR_NODE+1] = {0};
	if(!GetPrivateProfileSectionNames(szTmpName, MAX_LENGTH_SECTION_OR_NODE, m_szIni)) return false;
	arrSections.RemoveAll();
	int i = 0;
	for(;;)
	{
		if(szTmpName[i] == 0) break;
		arrSections.Add(szTmpName + i);
		i += _tcslen(szTmpName + i) + 1;
	}
	return true;
}
bool CConfigIni::LoadInstruSections(CStringArray& arrSections) const
{
	CStringArray arr;
	if(!LoadSections(arr)) return false;
	arrSections.RemoveAll();

	for(int i=0; i<arr.GetCount(); i++)
	{
		CString Name = arr.GetAt(i);
		if(Name.IsEmpty() || Name == CConfigIni::l_szIniKey_Sys) continue;
		arrSections.Add(Name);
	}
	return true;
}
bool CConfigIni::LoadString(const CString& strSection, const CString& strItem, CString& value) const
{
	if(m_szIni.IsEmpty()) return false;
	TCHAR szTmpName[MAX_LENGTH_SECTION_OR_NODE+1] = {0};
	if(!GetPrivateProfileString(strSection, strItem, NULL, szTmpName, MAX_LENGTH_SECTION_OR_NODE, m_szIni)) return false;
	value = szTmpName;
	return true;
}
bool CConfigIni::LoadPath(const CString& strSection, const CString& strItem, CString& value) const
{
	CString v;
	if(!LoadString(strSection, strItem, v)) return false;
	value = m_szPath;
	value += v;
	PathCanonicalize(v.GetBuffer(MAX_LENGTH_SECTION_OR_NODE), value.GetBuffer());
	v.ReleaseBuffer();
	value.ReleaseBuffer();
	value = v;
	return true;
}
bool CConfigIni::LoadInt(const CString& strSection, const CString& strItem, int& value) const
{
	CString v;
	if(!LoadString(strSection, strItem, v)) return false;
	value = _ttoi(v);
	return true;
}
bool CConfigIni::SaveString(const CString& strSection, const CString& strItem, const CString& value) const
{
	return WritePrivateProfileString(strSection, strItem, value, m_szIni) != 0;
}
bool CConfigIni::SavePath(const CString& strSection, const CString& strItem, const CString& value) const
{
	ASSERT(false);//·��ֻ�ܴ������ļ���ȡ�����ܴӳ���������
	return false;/*
	CString t(value);
	t.Replace(m_szPath, _T(""));
	return SaveString(strSection, strItem, t);//*/
}
bool CConfigIni::SaveInt(const CString& strSection, const CString& strItem, int value) const
{
	CString t;
	t.Format(_T("%d"), value);
	return SaveString(strSection, strItem, t);
}
bool CConfigIni::SaveSystemName(const CString& strSystem) const
{
	return SaveString(l_szIniKey_Sys, l_szIniItem_Sys, strSystem);
}
bool CConfigIni::LoadSystem(const CString& systemName, CConfigIni::structSystem& system)
{
	//·��
	LoadPath(systemName, l_szIniItem_PathData, system.m_szPathData);
	LoadPath(systemName, l_szIniItem_PathTask, system.m_szPathTask);
	LoadPath(systemName, l_szIniItem_PathSample, system.m_szPathSample);
	LoadPath(systemName, l_szIniItem_PathCompound, system.m_szPathCompound);
	LoadPath(systemName, l_szIniItem_PathMethod, system.m_szPathMethod);
	LoadPath(systemName, l_szIniItem_PathCrystalReport, system.m_szPathCrystalReport);
	LoadPath(systemName, l_szIniItem_PathLog, system.m_szPathLog);
	//����
	LoadString(systemName, l_szIniItem_DefaultMethod, system.m_szDefaultMethod);
	LoadString(systemName, l_szIniItem_Inject, system.m_szInject);
	//	LoadString(systemName, _T("SCParam"), system.m_szSCParam);
	//	LoadString(systemName, _T("SysSettings"), system.m_szSysSettings);
	LoadString(systemName, l_szIniItem_Instrument, system.m_szInstrument);
	//Ӳ��
	LoadInt(systemName, l_szIniItem_RGACOM, system.m_iRGACOM);
	LoadInt(systemName, l_szIniItem_ValveCOM, system.m_iValveCOM);
	LoadInt(systemName, l_szIniItem_PumpCOM, system.m_iPumpCOM);

	LoadInt(systemName, l_szIniItem_NextSampleID, system.m_iNextSampleID);

	LoadString(systemName, l_szIniItem_WindowName, system.m_szWindowName);
	LoadString(systemName, l_szIniItem_ProgramName, system.m_szProgramName);
	LoadString(systemName, l_szIniItem_MessageName, system.m_szMessageName);
	LoadString(systemName, l_szIniItem_SignalName, system.m_szSignalName);
	LoadString(systemName, l_szIniItem_XSLFolder, system.m_szXSLFolder);
	if(system.m_szXSLFolder.GetLength())
	{
		TCHAR c = system.m_szXSLFolder.GetAt(system.m_szXSLFolder.GetLength() - 1);
		if(c == '/' || c == '\\') system.m_szXSLFolder.Delete(system.m_szXSLFolder.GetLength() - 1);	
	}
	LoadString(systemName, l_szIniItem_LastTask, system.m_szLastTask);

	LoadInt(systemName, l_szIniItem_ShowAutoSampler, system.m_bShowAutoSampler);

	return true;
}
bool CConfigIni::SaveSystem(const CString& systemName, const CConfigIni::structSystem& system) const
{
	//·��
	//SavePath(systemName, l_szIniItem_PathData, system.m_szPathData);
	//SavePath(systemName, l_szIniItem_PathTask, system.m_szPathTask);
	//SavePath(systemName, l_szIniItem_PathSample, system.m_szPathSample);
	//SavePath(systemName, l_szIniItem_PathCompound, system.m_szPathCompound);
	//SavePath(systemName, l_szIniItem_PathMethod, system.m_szPathMethod);
	//SavePath(systemName, l_szIniItem_PathCrystalReport, system.m_szPathCrystalReport);
	//SavePath(systemName, l_szIniItem_PathLog, system.m_szPathLog);
	//����
	SaveString(systemName, l_szIniItem_DefaultMethod, system.m_szDefaultMethod);
	SaveString(systemName, l_szIniItem_Inject, system.m_szInject);
	//	SaveString(systemName, _T("SCParam"), system.m_szSCParam);
	//	SaveString(systemName, _T("SysSettings"), system.m_szSysSettings);
	SaveString(systemName, l_szIniItem_Instrument, system.m_szInstrument);
	//Ӳ��
	SaveInt(systemName, l_szIniItem_RGACOM, system.m_iRGACOM);
	SaveInt(systemName, l_szIniItem_ValveCOM, system.m_iValveCOM);
	SaveInt(systemName, l_szIniItem_PumpCOM, system.m_iPumpCOM);

	SaveInt(systemName, l_szIniItem_NextSampleID, system.m_iNextSampleID);

	SaveString(systemName, l_szIniItem_WindowName, system.m_szWindowName);
	SaveString(systemName, l_szIniItem_ProgramName, system.m_szProgramName);
	SaveString(systemName, l_szIniItem_MessageName, system.m_szMessageName);
	SaveString(systemName, l_szIniItem_SignalName, system.m_szSignalName);
	SaveString(systemName, l_szIniItem_XSLFolder, system.m_szXSLFolder);
	SaveString(systemName, l_szIniItem_LastTask, system.m_szLastTask);

	return true;
}
bool CConfigIni::LoadCurrentSystem()
{
	if(m_szName == _T("?"))
	{
		if(!SelectCurrentSystem(m_szName)) return false;
	}

	return LoadSystem(m_szName, m_System);
}
bool CConfigIni::SetCurrentSystemName(const CString& strSystem, bool bRefresh)
{
	m_szName = strSystem;
	if(bRefresh) return LoadCurrentSystem();
	else return true;
}
bool CConfigIni::LoadConfig(LPCTSTR strIni)
{
	if(strIni == NULL)
	{
		TCHAR l_szIniPath[MAX_PATH] = {0};
		GetModuleFileName(AfxGetInstanceHandle(), l_szIniPath, MAX_PATH);
		PathRemoveFileSpec(l_szIniPath);
		PathAddBackslash(l_szIniPath);
		m_szPath = l_szIniPath;
		StrCat(l_szIniPath, l_szIniFilename);
		m_szIni = l_szIniPath;
	}
	else m_szIni = strIni;

	//�ڵ���
	LoadString(l_szIniKey_Sys, l_szIniItem_Sys, m_szName);

	return LoadCurrentSystem();
}
bool CConfigIni::SaveConfig(LPCTSTR strIni) const
{
	SaveSystemName(m_szName);
	return SaveSystem(m_szName, m_System); 
}




CBase_AnalysisTemplate::CBase_AnalysisTemplate(CString cStrSystemName)
:m_pcInject(NULL),m_pcCurrentSample(NULL),m_pcInstrment(NULL),m_pcCurrentMethod(NULL),
m_pcWindowMain(NULL), m_pcWindowTask(NULL)
//,m_bDisplayCurve(TRUE)
//,m_pcCurrentChildFrmChart(NULL)
{

	//����ϵͳ����
	if(cStrSystemName.GetLength()<1) 
		return;

	m_cStrSystemName = cStrSystemName;
/*
	//
	//��Config.ini�ļ��еõ���ʼ������
	//

	//���Ӧ��ϵͳ������

	//
	//------------------------------- PMASS��ҵ���׷���ϵͳ -------------------------------------
	//
	if(!g_cConfigIni.m_szName.Compare(PASYSTEM_NAME_INMS)) {
		//�趨�������͵�ȫ�ֱ���
		//g_cStrInstrumentType = PASYSTEM_NAME_INMS;
		//
		//��ý�����������Ϣ
		//
		m_pcInject = CreateNewInject();
		if(DisplayIfError(_T("�������������"), m_pcInject->GetInject()->LoadFromFile(g_cConfigIni.m_szInject))!=IAERROR_SUCCESS){
			return ;
		}

		//
		//���ȱʡ������Ϊ��ǰ����
		//
		m_pcCurrentMethod = LoadMethod(g_cConfigIni.m_szDefaultMethod);

		if(!m_pcCurrentMethod) {
			AfxMessageBox(_T("���ش���:ȱʡ����û���ҵ�,ϵͳ�޷���������!! \n\n���˳�ϵͳ���°�װ���߻ָ�ȱʡ����!"));
		}


		///�����������
		m_pcInstrment = CreateNewInstrument();
		m_pcInstrment->UploadFromDevice();


		//��ò������Ĳ���
		m_pcAcquirer = CreateNewAcquirer();

	}
	//
	//------------------------------- ��������ϵͳ -------------------------------------
	//
	else if(FALSE) {

	}
	else {
		AfxMessageBox(_T("ϵͳ������, ϵͳ�޷��������У�����Config.ini�ļ�!"));
		return ;
	}	


	{
		// ��Ҫ����һЩ�ж�
		m_bValid = TRUE;
	}


	//Ӧ���ݸ���ϵͳ���ƴ���������
	//��ǰ��Ʒ��ָ��
	//	m_pcCurrentSample= new CAnalysisSample;
	//��ǰ��������ָ��
*/
}

CString	CBase_AnalysisTemplate::GetRuntimeForlder(void)
{
	CString sPath,cStrPathRerturn;
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	int i=sPath.GetLength()-1;
	while(i>0)
	{
		if(sPath.GetAt(i)=='/' || sPath.GetAt(i)=='\\') break;
		i--;
	}
	if(i<0) return false;
	if(i==0) cStrPathRerturn=".\\";
	else cStrPathRerturn=sPath.Left(i)+_T("\\") ;
	return cStrPathRerturn;
}



CBase_AnalysisTemplate::~CBase_AnalysisTemplate()
{

	//��ס���棡����
	//m_pcInstrment->m_cCurrentInstrumentParam.SaveToFile(g_cConfigIni.m_szSCParam);

	if(m_pcInject){delete m_pcInject;m_pcInject=NULL;}
	if(m_pcInstrment){delete m_pcInstrment;m_pcInstrment=NULL;}
	if(m_pcCurrentSample){delete m_pcCurrentSample;m_pcCurrentSample=NULL;}
	if(m_pcCurrentMethod) {
		m_pcCurrentMethod->Save();
		delete m_pcCurrentMethod;
		m_pcCurrentMethod=NULL;
	}
}


/*
CBase_AnalysisSample *CBase_AnalysisTemplate::CreateNewSample(CString cStrName, CString cStrNameFrom)
{
	AfxMessageBox(_T("�����޷�ʵ�ָù��ܣ�"));
	return NULL;
}
*/

CBase_AnalysisMethod	*CBase_AnalysisTemplate::CreateNewMethod(CString cStrName, CString cStrNameFrom)
{
	//�����������
	if(cStrName.GetLength()<1) return NULL;

	CBase_AnalysisMethod *pcMethod = NULL;

	if(m_pcCurrentMethod->IsNameExist(cStrName)) {
		AfxMessageBox(_T("�÷��������Ѿ����ڣ�"));
		return NULL;
	}

	pcMethod = new CBase_AnalysisMethod(cStrName, this);


	//�жϲ��շ����Ƿ����
	if(cStrNameFrom.GetLength()>0) {
		CBase_AnalysisMethod *pcMethodFrom = LoadMethod(cStrNameFrom);
		if(pcMethodFrom) {
			pcMethod->GetDataFrom(*pcMethodFrom);
			delete pcMethodFrom;
		}
	}

	return pcMethod;
}


CBase_AnalysisMethod	*CBase_AnalysisTemplate::LoadMethod(CString cStrMethodName)
{
	CBase_AnalysisMethod *pcMethod = NULL;

	if(cStrMethodName.GetLength()<1) return NULL;


	pcMethod = new CBase_AnalysisMethod(_T(""), this);

	if(pcMethod->Load(cStrMethodName)) 
		return pcMethod;
	else {
		delete pcMethod;
		return NULL;
	}

}




BOOL CBase_AnalysisTemplate::ChangeCurrentMethod(CString cStrMethod, BOOL bSaveOldMethod)
{
	//ͨ�����Ƶ��뷽��
	if(cStrMethod.GetLength()<1) return FALSE;
	if(m_pcCurrentMethod && m_pcCurrentMethod->GetName()==cStrMethod)
	{
		//���������ͬ������ʹ�õ�ǰ�ڴ��еķ���
		return TRUE;//m_pcCurrentMethod->Run();//tiewen 09-06-02 16:05
	}

	//���뷽��
	CBase_AnalysisMethod *pcMethod = LoadMethod(cStrMethod);
	if(!pcMethod) return FALSE;
/*
	//���ϵķ�������
	if(bSaveOldMethod) {
		if(!m_pcCurrentMethod->Save()) {
			delete pcMethod;
			return FALSE;
		}
	}
*/
	return ChangeCurrentMethod(pcMethod, bSaveOldMethod);
/*
	if(m_pcCurrentMethod)
		delete m_pcCurrentMethod;

	//�����뷽����Ϊȱʡ����
	m_pcCurrentMethod = pcMethod;

	//���µ����� 090608 16��34
	AfxGetMainWnd()->SendMessage(WM_REFRESHCURRENTMETHOD);

	//���и÷���
	return 
		m_pcCurrentMethod->Run();
*/
}
BOOL CBase_AnalysisTemplate::ChangeCurrentMethod(CBase_AnalysisMethod* pMethod, BOOL bSaveOldMethod)
{
	if(m_pcCurrentMethod && m_pcCurrentMethod == pMethod)
	{
		//���������ͬ������ʹ�õ�ǰ�ڴ��еķ���
		return TRUE;//m_pcCurrentMethod->Run();//tiewen 09-06-02 16:05
	}

	//���ϵķ�������
	if(bSaveOldMethod)
	{
		//if(m_pcCurrentMethod && !m_pcCurrentMethod->Save()) return FALSE;
		m_pcCurrentMethod->Save();
	}
	if(m_pcCurrentMethod) delete m_pcCurrentMethod;

	//�����뷽����Ϊȱʡ����
	m_pcCurrentMethod = pMethod;

	//���µ����� 090608 16��34
	m_pcWindowMain->Notify_CurrentMethodChanged();


	return TRUE;
	//AfxGetMainWnd()->SendMessage(WM_REFRESHCURRENTMETHOD);
/*
	if(m_pcCurrentMethod) //���и÷���
		return m_pcCurrentMethod->Run();
	else return true;
*/
}

BOOL CBase_AnalysisTemplate::ChangeCurrentSample(CBase_AnalysisSample *pcNewSample)
{

	if(m_pcCurrentSample==pcNewSample) return TRUE;

//  Ϊ�˼ӿ��л��ٶȣ�������Ʒ�رյ�ʱ�����̡��Ƿ�᲻�ף�[12/17/2012 ���] 
/* 
	if(m_pcCurrentSample)
	{
		//���ϵ���Ʒ����
		if(!m_pcCurrentSample->Save()) {
			AfxMessageBox(_T("��Ʒ�������"));
		}
	}
*/
	//�ͷ�
	//delete m_pcCurrentSample;

	m_pcCurrentSample = pcNewSample;// �Ƶ�ǰ��ȥ�� [12/17/2012 ���]


	//�򿪵�ǰ��Ʒ
	if(pcNewSample)
	{
		Sample_Show(pcNewSample, false);

		bool bNeedDownload = true;
		if(m_pcWindowTask)
		{
			CAnalysisTask* pTask = m_pcWindowTask->GetTask();
			if(pTask)
			{
				const CAnalysisTaskItem* pItem = pTask->GetItem(pcNewSample);
				if(pItem && pItem->m_bUsingCurrentArgs) bNeedDownload = false;
			}
		}
		if(!bNeedDownload)
		{
			switch(pcNewSample->GetStatus())
			{
			case CBase_AnalysisSample::null:
			case CBase_AnalysisSample::abort:
				DISPifERROR(Sample_PrepareAcquire(pcNewSample), NULL, _T("׼������"));
			}
		}
	}
	else 
		CurrentSample_Changed(NULL);

	//�ı�ȱʡ��Ʒ
	//m_pcCurrentSample = pcNewSample;// �Ƶ�ǰ��ȥ�� [12/17/2012 ���]

//* ��Щ�ƶ�������CBase_AnalysisTemplate������ȥ�� ---����Ѿ���CBase_AnalysisTemplate��
	//�ı������������ʾ
	if(m_pcWindowTask)
		m_pcWindowTask->Notify_CurrentSampleChanged(pcNewSample);

	//�ı����������ʾ
	if(m_pcWindowMain){
		m_pcWindowMain->Notify_CurrentSampleChanged();
		m_pcWindowMain->Notify_CurrentReportChanged();
	}
//*/
	return TRUE;

}

BOOL CBase_AnalysisTemplate::CurrentSample_Changed(CBase_AnalysisSample *pcNewSample)//  [12/17/2012 ���] �ú���Ӧ����OnMDIActivate�е��ã�������ǰ��Ʒ�Ѿ��ı䣬�����ǽ�Ҫ�ı�
{

	m_pcCurrentSample = pcNewSample;// �Ƶ�ǰ��ȥ�� [12/17/2012 ���]

	//�ı������������ʾ
	if(m_pcWindowTask)
		m_pcWindowTask->Notify_CurrentSampleChanged(pcNewSample);

	//�ı����������ʾ
	if(m_pcWindowMain){
		m_pcWindowMain->Notify_CurrentSampleChanged();
		m_pcWindowMain->Notify_CurrentReportChanged();
	}
	return TRUE;

}


int CBase_AnalysisTemplate::Sample_DataProcess(CBase_AnalysisSample *pcSample)
{
	RETURNifERROR(pcSample->Base_DataProcess()); 
	return pcSample->Base_DataCalibrate();
}

void CBase_AnalysisTemplate::Notify_SampleStatusChanged(CBase_AnalysisSample *pcSample)
{
	if(m_pcWindowTask) m_pcWindowTask->Notify_SampleStatusChanged(pcSample);
	if(m_pcWindowMain) m_pcWindowMain->Notify_SampleStatusChanged((void*)pcSample);
}

CString CBase_AnalysisTemplate::GetDataFolder(enumPADATATYPENAME nDataType)
{
	TCHAR szOut[MAX_PATH];
	DWORD dwAttr;
	switch(nDataType) {
	case PA_DATATYPENAME_SAMPLE:
		StrCpy(szOut, g_cConfigIni.CurrentSystem().m_szPathSample);
		break;
	case PA_DATATYPENAME_TASK:
		StrCpy(szOut, g_cConfigIni.CurrentSystem().m_szPathTask);
		break;
	case PA_DATATYPENAME_COMPOUND:
		StrCpy(szOut, g_cConfigIni.CurrentSystem().m_szPathCompound);
		break;
	case PA_DATATYPENAME_METHOD:
		StrCpy(szOut, g_cConfigIni.CurrentSystem().m_szPathMethod);
		break;
	case PA_DATATYPENAME_CRYSTALREPORT:
		StrCpy(szOut, g_cConfigIni.CurrentSystem().m_szPathCrystalReport);
		break;
	case PA_DATATYPENAME_LOG:
		StrCpy(szOut, g_cConfigIni.CurrentSystem().m_szPathLog);
		break;
	case PA_DATATYPENAME_DATA:
	default:
		StrCpy(szOut, g_cConfigIni.CurrentSystem().m_szPathData);
	}

	dwAttr = GetFileAttributes(szOut);
	if (dwAttr == INVALID_FILE_ATTRIBUTES || !(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) {
		SHCreateDirectory(NULL, szOut);
	}
	PathAddBackslash(szOut);
	return CString(szOut);
}

CString CBase_AnalysisTemplate::GetDataFilename(enumPADATATYPENAME nDataType)
{
	switch(nDataType) {
	case PA_DATATYPENAME_DEFAULTMETHOD:
		return g_cConfigIni.CurrentSystem().m_szDefaultMethod;
	case PA_DATATYPENAME_INJECT:
		return g_cConfigIni.CurrentSystem().m_szInject;
	case PA_DATATYPENAME_INSTRUMENT:
		return g_cConfigIni.CurrentSystem().m_szInstrument;
	}
	return CString(_T(""));
}


int CBase_AnalysisTemplate::Sample_LinkToInstrument(CBase_AnalysisSample *pcSample, CBase_AnalysisInstrument *pcInstrument, int nChannel)
{
	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcInstrument==pcInstrument&&m_cArrayInstrument2Sample[i].m_nChannel==nChannel) {
			if(m_cArrayInstrument2Sample[i].m_pcSample!=pcSample)
				return IAERROR_CG_INSTRUMENT_BUSY;
			else 
				return IAERROR_SUCCESS;
		}
	}

	//�鿴��Ʒ��״̬�����Ƿ��ܹ����ӵ�����
	if(pcSample->GetStatus()==CBase_AnalysisSample::idle||
		pcSample->GetStatus()==CBase_AnalysisSample::finish){
			return IAERROR_CG_SAMPLE_STATUSERROR;
	}

	//�鿴�������Ƿ�֧�����ͨ��
	if(!pcInstrument->Base_IsChannelAvailable(nChannel)) {
		return IAERROR_INSTRUMENT_CHANNELERROR;
	}
			


	//����ǵ�һ�Σ���Ҫ����Ʒ�������͵�����
// 	int nRet = Instrument_SetParam(pcInstrument, pcSample);
// 	if(nRet!=IAERROR_SUCCESS) return nRet;

	CInstrument2Sample item;

	item.m_pcSample = pcSample;
	item.m_pcInstrument = pcInstrument;
	item.m_nChannel = nChannel;

	m_cArrayInstrument2Sample.Add(item);

	//���Ӻ������Ժ󣬸���Ʒһ�����������׼��
	VERIFY(pcSample->Base_GetReady());

	return IAERROR_SUCCESS;

}

int CBase_AnalysisTemplate::Sample_ReleaseFromInstrument(CBase_AnalysisSample *pcSample)
{
	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcSample==pcSample) {
			m_cArrayInstrument2Sample.RemoveAt(i);	
			
			g_TracePersistentlyWithTime(_T("Sample_Stop/Sample_Drop: CBase_AnalysisTemplate::Sample_ReleaseFromInstrument.\r\n"));

			return IAERROR_SUCCESS;
		}
	}

	return IAERROR_SYSTEMERROR;

}
int CBase_AnalysisTemplate::Sample_AbortReady(CBase_AnalysisSample *pcSample)	
{
	int nRet  = pcSample->Base_AbortReady();
	if(nRet != IAERROR_SUCCESS) return nRet;

	Sample_ReleaseFromInstrument(pcSample);

	return IAERROR_SUCCESS;
}




int CBase_AnalysisTemplate::Instrument_SetParam(CBase_AnalysisInstrument *pcInstrument, const CString & cStrFullXMLpath, const CString & cStrXML)
{
	return pcInstrument->Base_SetParam(cStrFullXMLpath, cStrXML);
}


int CBase_AnalysisTemplate::Instrument_SetParam(CBase_AnalysisInstrument *pcInstrument, CBase_AnalysisSample *pcSample)
{

	//����Ʒ�������ݸ�����
	//
	//�趨��������,�����ݽ���ı���Ʒ״̬�ͷ���ֵ
	//
	int nRet;
	CString cStrXMLParam;

	if(IAERROR_SUCCESS!=pcSample->m_data.GetValue_XMLString(_T("Sample\\Parameter\\Instrument"), cStrXMLParam)) 
		return IAERROR_XML_ITEMNOTFOUND;//û���ҵ���Ӧ����������

	return Instrument_SetParam(pcInstrument, _T("Instrument"), cStrXMLParam);

}


void CBase_AnalysisTemplate::SendRealTimeData(CBase_AnalysisInstrument *pcInstrument, int nChannel, CCurveData &curveData)
{
	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcInstrument==pcInstrument&&m_cArrayInstrument2Sample[i].m_nChannel==nChannel) {
			if(m_cArrayInstrument2Sample[i].m_pcSample)
				m_cArrayInstrument2Sample[i].m_pcSample->Base_RealtimeData(curveData);
		}
	}
}


void CBase_AnalysisTemplate::Notify_SampleDead(CBase_AnalysisSample *pcSample)
{

	//����������⿪
	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcSample==pcSample) {
			m_cArrayInstrument2Sample.RemoveAt(i);
			--i;
		}
	}

	//��ǰ��Ʒȥ��
	if(m_pcCurrentSample==pcSample)
		m_pcCurrentSample = NULL;
}

void CBase_AnalysisTemplate::Notify_InstrumentDead(CBase_AnalysisInstrument *pcInstrument)
{
	//����������⿪
	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcInstrument==pcInstrument) {
			m_cArrayInstrument2Sample.RemoveAt(i);
			--i;
		}
	}

	//��ǰ��Ʒȥ��
	if(m_pcInstrment==pcInstrument)
		m_pcInstrment = NULL;

}

void CBase_AnalysisTemplate::Notify_StartSignal(CBase_AnalysisInstrument *pcInstrument, int nChannel, DWORD dwTime)
{
	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcInstrument==pcInstrument&&m_cArrayInstrument2Sample[i].m_nChannel==nChannel) {
			if(m_cArrayInstrument2Sample[i].m_pcSample)
				m_cArrayInstrument2Sample[i].m_pcSample->Base_StartRun(dwTime);
		}
	}

}

int CBase_AnalysisTemplate::Sample_PrepareAcquire(CBase_AnalysisSample *pcSample)
{
	if(!pcSample) {
		return IAERROR_PARAMETER_WRONGTYPE;
	}

	CBase_AnalysisInstrument *pcInstru=NULL;

	BOOL bConnected=FALSE;
	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcSample==pcSample) {
			bConnected = TRUE;
			pcInstru = m_cArrayInstrument2Sample[i].m_pcInstrument;
			g_TracePersistentlyWithTime(_T("Sample_Prepare: CBase_AnalysisTemplate::Sample_PrepareAcquire(connected).\r\n"));
			break;
		}
	}

	if(!bConnected) pcInstru = GetInstrument();

	if(pcInstru == NULL) return IAERROR_INSTRUMENT_NOTCONNECT;

	if(!pcInstru->Instrument_IsReady())
	{
		//ALERT(ICON_CRITICAL, _T("����δ����"), _T("��Ʒ׼��"), NULL);
		return IAERROR_INSTRUMENT_NOTCONNECT;
	}

	bool bNeedDownload = true;
	if(m_pcWindowTask)
	{
		CAnalysisTask* pTask = m_pcWindowTask->GetTask();
		if(pTask)
		{
			const CAnalysisTaskItem* pItem = pTask->GetItem(pcSample);
			if(pItem && pItem->m_bUsingCurrentArgs) bNeedDownload = false;
		}
	}
	if(bNeedDownload)
	{
		//�����Ƚ�
		//����ȡ�õ�ǰ��������
		CString xmlString;
		if(pcInstru->Base_UploadFromDevice(xmlString) != IAERROR_SUCCESS) return IAERROR_CG_SAMPLE_NOTREADY;
		CAnalysisData ad;
		if(ad.LoadFromDocument(xmlString) != IAERROR_SUCCESS) return IAERROR_CG_SAMPLE_NOTREADY;
		//Ȼ�����Ʒ�еĲ������бȽ�
		CAnalysisData& adSample = pcSample->m_data;
		CStringArray notequal;
		static CString sp(_T("Sample/Parameter/"));
		if((g_cConfigIni.CurrentSystemName().Compare(PASYSTEM_NAME_GC_PX) == 0) || (g_cConfigIni.CurrentSystemName().Compare(PASYSTEM_NAME_GC1120) == 0))
		{
			static LPCTSTR vs[] =
			{
				_T("Instrument/MainFrame/Temp_ColumnBox"),
				_T("Instrument/MainFrame/Temp_FrontDetector"),
				_T("Instrument/MainFrame/Temp_RearDectetor"),
				_T("Instrument/MainFrame/Temp_FrontInjector"),
				_T("Instrument/MainFrame/Temp_RearInjector")
			};
			for(int i=0; i<sizeof(vs)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs[i], ad, vs[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs[i]);
			}
		}
		else if(g_cConfigIni.CurrentSystemName().Compare(PASYSTEM_NAME_GC2400) == 0)
		{
			static LPCTSTR vs[] =
			{
				_T("Instrument/MainFrame/Temp_ColumnBox"),
				_T("Instrument/MainFrame/Temp_FrontInjector"),
				_T("Instrument/MainFrame/Temp_RearDectetor")
			};
			for(int i=0; i<sizeof(vs)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs[i], ad, vs[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs[i]);
			}
		}
		else if((g_cConfigIni.CurrentSystemName().Compare(PASYSTEM_NAME_GC1100W) == 0))
		{
			static LPCTSTR vs[] =
			{
				_T("Instrument/MainFrame/Temp_ColumnBox"),
				_T("Instrument/MainFrame/Temp_Detector")
			};
			for(int i=0; i<sizeof(vs)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs[i], ad, vs[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs[i]);
			}
		}
		else if((g_cConfigIni.CurrentSystemName().Compare(PASYSTEM_NAME_GC1100P) == 0))
		{
			static LPCTSTR vs[] =
			{
				_T("Instrument/MainFrame/Temp_InputDetect"),
				_T("Instrument/MainFrame/Pressure"),
				_T("Instrument/MainFrame/Flow_ZQ"),
				_T("Instrument/MainFrame/Flow_WC")
			};
			for(int i=0; i<sizeof(vs)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs[i], ad, vs[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs[i]);
			}
		}
		else if(g_cConfigIni.CurrentSystemName().Compare(PASYSTEM_NAME_LC1620A) == 0)
		{
			static LPCTSTR vs[] =
			{
				_T("Instrument/Pump/InitFLowAPercent"),
				_T("Instrument/Pump/InitFLowBPercent"),
				_T("Instrument/Pump/InitFLow")
			};
			for(int i=0; i<sizeof(vs)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs[i], ad, vs[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs[i]);
			}

			static LPCTSTR vs2[] =
			{
				_T("Instrument/UVDetector/WaveLength")
			};
			for(int i=0; i<sizeof(vs2)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs2[i], ad, vs2[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs2[i]);
			}
		}
		else if(g_cConfigIni.CurrentSystemName().Compare(PASYSTEM_NAME_IC1800) == 0)
		{
			static LPCTSTR vs[] =
			{
				_T("Instrument/Pump/InitFLow")
			};
			for(int i=0; i<sizeof(vs)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs[i], ad, vs[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs[i]);
			}

			static LPCTSTR vs2[] =
			{
				_T("Instrument/Detector/Temp"),
				_T("Instrument/Detector/YZQDL")/*,
											   _T("Instrument/Detector/Range")//*/
			};
			for(int i=0; i<sizeof(vs2)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs2[i], ad, vs2[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs2[i]);
			}

			static LPCTSTR vs3[] =
			{
				_T("Instrument/ColumnBox/Temp")
			};
			for(int i=0; i<sizeof(vs3)/sizeof(LPCTSTR); ++i)
			{
				if(adSample.CompareValue_Double(sp + vs3[i], ad, vs3[i]) != CAnalysisData::CR_EQUAL) notequal.Add(vs3[i]);
			}
		}
		if(!notequal.IsEmpty())
		{
			CString name, sv, cv, tr;
			// ������ͬ�Ի���
			CString msg(_T("<table border='1'><tr><td>����</td><td>��Ʒ��ֵ</td><td>��ǰ��ֵ</td></tr>"));
			for(int i=0; i<=notequal.GetUpperBound(); ++i)
			{
				const CString& si = notequal.GetAt(i);
				adSample.GetProperty(sp + si, XMLPROP_NAME, name);
				adSample.GetValue_String(sp + si, sv);
				if((ad.GetValue_String(si, cv) != IAERROR_SUCCESS) || cv.IsEmpty()) cv = _T("&nbsp;");
				if((g_cConfigIni.CurrentSystemName().Compare(PASYSTEM_NAME_IC1800) == 0) && (si == _T("Instrument/Detector/Temp")))
				{
					if(sv == _T("0")) sv = _T("40��");
					else if(sv == _T("1")) sv = _T("50��");
					else if(sv == _T("2")) sv = _T("60��");
					else sv = _T("��������");

					if(cv == _T("0")) cv = _T("40��");
					else if(cv == _T("1")) cv = _T("50��");
					else if(cv == _T("2")) cv = _T("60��");
					else cv = _T("��������");
				}
				tr.Format(_T("<tr><td>%s</td><td>%s</td><td>%s</td></tr>"), name, sv, cv);
				msg += tr;
			}
			msg += _T("</table>");
			switch(HTMLMessageBox(NULL, msg, _T("��ǰ���ò�������Ʒ�е�����������һ��"), _T("������Ʒ�е���������;���õ�ǰ���ò���;ȡ��"), _T(";"), ICON_QUESTION))
			{
			case 0:
				// ������Ʒ�е���������
				if(adSample.GetValue_XMLString(XMLPATH_SAMPLE_PARAM_INSTRUMENT, xmlString) != IAERROR_SUCCESS) return IAERROR_CG_SAMPLE_NOTREADY;
				if(pcInstru->Base_DownloadToDevice(xmlString) != IAERROR_SUCCESS) return IAERROR_CG_SAMPLE_NOTREADY;
				break;
			case 1:break;
			default://ȡ����X
				return IAERROR_CG_SAMPLE_NOTREADY;
			}
		}
	}

	if(!bConnected)
	{
		//��������
		int nret = Sample_LinkToInstrument(pcSample, pcInstru, pcSample->GetInstrumentChannel());
		RETURNifERROR(nret);
		CString t;
		t.Format(_T("Sample_Prepare: CBase_AnalysisTemplate::Sample_LinkToInstrument:%d.\r\n"), nret);
		g_TracePersistentlyWithTime(t);
	}

	//�趨��������
	int nRet = IAERROR_SUCCESS;

// 	CString cStrXMLParam;
// 
// 	if(IAERROR_SUCCESS!=pcSample->m_data.GetValue_XMLString(_T("Sample\\Parameter\\Instrument"), cStrXMLParam)) 
// 		return IAERROR_XML_ITEMNOTFOUND;//û���ҵ���Ӧ����������
// 
// 	nRet = Instrument_SetParam(pcInstru, _T("Instrument"), cStrXMLParam);

	if(nRet==IAERROR_CG_INSTRUMENT_ERROR) {
		return IAERROR_CG_INSTRUMENT_ERROR;
	}
	else if(nRet==IAERROR_SUCCESS) {
		return IAERROR_SUCCESS;
	}
	else
		return nRet;

	return IAERROR_CG_SAMPLE_NOTREADY;
}

int  CBase_AnalysisTemplate::Sample_StartRequire(CBase_AnalysisSample *pcSample)
{
	if(!pcSample) 
		return IAERROR_SYSTEMERROR;

	//�ж���Ʒ�Ƿ�׼����
	if(!pcSample->Base_IsReady()) {
		return IAERROR_CG_SAMPLE_STATUSERROR;
	}


	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcSample==pcSample) {
			//�ж������Ƿ�׼����
			if(!m_cArrayInstrument2Sample[i].m_pcInstrument->Base_IsReady()) {
				return IAERROR_CG_INSTRUMENT_ERROR;
			}
			g_TracePersistentlyWithTime(_T("Sample_Start: CBase_AnalysisTemplate::Sample_StartRequire.\r\n"));
			return m_cArrayInstrument2Sample[i].m_pcInstrument->Base_StartRequire(m_cArrayInstrument2Sample[i].m_nChannel);
		}
	}

	return IAERROR_SYSTEMERROR;
}

void CBase_AnalysisTemplate::Sample_Reset(CBase_AnalysisSample *pcSample)	//Ҫ����Ʒ����������ݣ�״̬�ص�null
{
	pcSample->Base_ResetData();
}

int CBase_AnalysisTemplate::Sample_StopRun(CBase_AnalysisSample *pcSample)
{
	g_TracePersistentlyWithTime(_T("Sample_Stop: CBase_AnalysisTemplate::Sample_StopRun.\r\n"));

	//������Ʒֹͣ����
	int nRet  = pcSample->Base_StopRun();
	//�����Ʒ˵����ͣ���ǾͲ�ͣ

	CString t;
	t.Format(_T("Sample_Stop: pcSample->Base_StopRun:%d.\r\n"), nRet);
	g_TracePersistentlyWithTime(t);

	if(nRet!=IAERROR_SUCCESS)
		return nRet;



	//�ҳ���Ӧ������
	CBase_AnalysisInstrument *pcInstru=NULL;
	int nChannel = 0;

	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) {
		if(m_cArrayInstrument2Sample[i].m_pcSample==pcSample) {
			pcInstru = m_cArrayInstrument2Sample[i].m_pcInstrument;
			nChannel = m_cArrayInstrument2Sample[i].m_nChannel;
		}
	}

	//֪ͨ����ֹͣ����
	if(pcInstru) {
		pcInstru->Base_StopNotify(nChannel);
	}
	else
		ASSERT(FALSE);

	//�Ͽ���������Ʒ������
	Sample_ReleaseFromInstrument(pcSample);

	
	//�����Ƚ�
	//����ȡ�õ�ǰ��������
	CString xmlString;
	if(pcInstru->Base_UploadFromDevice(xmlString) == IAERROR_SUCCESS)
	{
		pcSample->Xml_PutParameter_Instrument2(xmlString);
	}

	return IAERROR_SUCCESS;
}

BOOL CBase_AnalysisTemplate::ChangeCurrentTask(CString cStrTaskName)
{
	if(m_pcWindowMain)
		m_pcWindowMain->Notify_CurrentTaskChanged(cStrTaskName);
	return TRUE;
}


/*
void CBase_AnalysisTemplate::CloseCurveView()
{
	try//��ֹ�ֶ��رմ��ڲ�������
	{
		if(m_pcCurrentChildFrmChart && IsWindow(m_pcCurrentChildFrmChart->GetSafeHwnd()))
		{
			m_pcCurrentChildFrmChart->Close();
		}
	}
	catch(...){}
	m_pcCurrentChildFrmChart = NULL;
}

*/

void CBase_AnalysisTemplate::ZeroFlag(CBase_AnalysisInstrument *pcInstrument, bool bZero)
{
	for(int i=0; i<m_cArrayInstrument2Sample.GetCount(); ++i) 
	{
		if(m_cArrayInstrument2Sample[i].m_pcInstrument==pcInstrument) 
		{
			if(m_cArrayInstrument2Sample[i].m_pcSample)
				m_cArrayInstrument2Sample[i].m_pcSample->m_bZero = bZero;
		}
	}

}
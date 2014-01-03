#pragma once
#include "AnalysisData.h"
#include "Base_SampleChart.h"
#include "RealTime.h"

class CBase_AnalysisTemplate;
class CTaskItemOption;
class CBase_AnalysisMethod;
class CBase_AnalysisInstrument;
class CBase_RealtimeDataMonitor;

#define ANALYSIS_SAMPLE_FILE_EXT _T(".cg.smp")
#define CRYSTAL_REPORT_FILE_EXT  _T(".rpt")
#define STRING_MAINCURVE_GROUP		_T("ResultPoints")
#define STRING_MAINCURVE_NAME		_T("AcquireData")
//"��Ʒ����#������__Curve.csv"
#define STRING_MAINCURVE_FILENAME	_T("%s#%d_Curve.csv")


#define XMLPROP_SAMPLECHART_RANGEMIN_BOTTOM _T("rangeBottomMin")
#define XMLPROP_SAMPLECHART_RANGEMAX_BOTTOM _T("rangeBottomMax")
#define XMLPROP_SAMPLECHART_RANGEMIN_LEFT _T("rangeLeftMin")
#define XMLPROP_SAMPLECHART_RANGEMAX_LEFT _T("rangeLeftMax")



class CPeakStandard
{
public:
	CPeakStandard():m_dReserveTime(0.0),m_dResultConcentration(0.0){}

	union CPeakType 
	{
		CPeakType():all(0){}
		struct 
		{
			unsigned short negative:			1;	//����
			unsigned short single :				1;	//����
			unsigned short overlap_pl:			1;	//�ص���
			unsigned short overlap_ls:			1;	//�ص���(���)
			unsigned short overlap_rs:			1;	//�ص���(�Ҽ�)
			unsigned short overlap_vs:			1;	//�ص���(��ֱ�ַ�)
			unsigned short tail:				1;	//��β
			//------
			unsigned short reserve2future:		9;	//�Ժ���				
		};
		unsigned short all;

		//--------
		void CleanMark(void){all=0;}
		//��÷����͵���������
		CString ConvertToTypeName(void); 
		void	GetFromTypeName(CString cStrName);
	} ;
	
public:
	CPeakType			m_cPeakType;	//������
	//int					m_nType;		//������
	double				m_dReserveTime;	//����ʱ��
	CCurveDataSingle	m_dataBegin;    //�����
	CCurveDataSingle	m_dataTop;		//�嶥���壬�ȣ�
	CCurveDataSingle	m_dataEnd;      //�����
	double				m_dHalfWidth;	//����
	double				m_dWidth;		//���
	double				m_dHeight;		//���
	double				m_dHeightPer;	//���%
	double				m_dArea;		//�����
	double				m_dAreaPer;		//�����%
	double				m_dDisfactor;   //��������??
	double				m_dSymmetry;    //��β����
	double				m_dResolution;  //�����
	double				m_dColumnPlate; //����������
	//������ResultTable��ʹ��
	double				m_dResultConcentration;	//Ũ�ȣ�����������ٷֱ�%���ڷ����������ʹ�ã����б��в�����ʾ
	CString				m_cStrComponentName;//�������
};

class CPeakList
{
//private:
//	CPeakList(){}
public:
	CString			m_cStrPathName;
	CAnalysisData *m_pcDataXML;
public:
	CPeakList(const CString &cStrPathName, CAnalysisData *pcDataXML){m_cStrPathName=cStrPathName;m_pcDataXML=pcDataXML;}
	void RemoveAll(void){m_cArrayData.RemoveAll();}
	void Add(CPeakStandard &item){m_cArrayData.Add(item);}

	//��m_cArrayData����ͬ����m_pcDataXML�У�����m_cStrPathName·��
	BOOL SynchronizeToXML(void); //{AfxMessageBox(_T("С�ڣ���m_cArrayData����ͬ����m_pcDataXML�У�����m_cStrPathName·��"));}
	//��m_pcDataXML�У�����m_cStrPathName·��������ݵ�m_cArrayData
	BOOL SynchronizeFromXML(void); //{AfxMessageBox(_T("��m_pcDataXML�У�����m_cStrPathName·��������ݵ�m_cArrayData"));}

	BOOL SyncFromXML_PeakListTable(void);
	BOOL SyncFromXML_PeakListTable(CIADataTable* pCIADataTable);
	BOOL SyncFromXML_ResultTable(void);
	BOOL SyncFromXML_ResultTable(CIADataTable* pCIADataTable);


	//�崦��������
	CArray<CPeakStandard,CPeakStandard&> m_cArrayData;
};



//ͨ����Ʒ��
class CBase_AnalysisSample : public CObject
{
	friend class CBase_AnalysisTemplate;
	friend class CBase_SampleChart;

public:
	enum enumSampleType
	{
		SAMPLE_TEST,
		SAMPLE_STANDARD,
	};

	enum STATUS
	{
		null = 0		,	//���У�����Ʒ��					
		preparing		,	//��Ʒ׼���У���						(�Ѿ���������)
		ready			,	//��Ʒ�Լ�׼�����ˣ�������û��ϵ��	(�Ѿ���������)
		running			,	//����								(�Ѿ���������)	
		idle			,	//����״̬���������жϣ���δ������ݴ���
		finish			,	//���						
		abort			,	//�쳣��ֹ�����������Ӧ�ö��ϳ�
		busy				//��Ʒæ�������ж�״̬
	};

	enum enumDrawType
	{
		DT_NONE = 0,
		DT_LINE = 1,
		DT_POINT = 2,
	};

	//���ڱ����Ʒ��һ���ֲ����Ѿ����¡�
	union CParamChanged{
		CParamChanged():allparameters(0){}
		struct {
			unsigned short curve_main :			1;	//������
			unsigned short curve_temperature:	1;	//�¶�����

			unsigned short param_integration:	1;	//���ֲ���
			unsigned short param_information:	1;	//��Ʒ��Ϣ
			unsigned short param_instrument:	1;	//��������
			unsigned short param_calibration:	1;	//У����������

			unsigned short result_integration:	1;	//���ֽ��
			unsigned short result_analysis:		1;	//�������

			unsigned short filepath:			1;	//����·��
			//------
			unsigned short reserve2future:		7;	//�Ժ���				
		};
		unsigned short allparameters;
		
		//--------
		void CleanMark(void){allparameters=0;}
		void FullMark(void) {allparameters=0xFFFF;}
		void Params(){param_integration=1;param_information=1;param_instrument=1;param_calibration=1;/*param_systemAdvice=1;*/}
	};

	struct structLine
	{
		struct structPoint
		{
			double x,y;
		}begin,end;
	};

protected:
	CBase_AnalysisSample(CBase_AnalysisTemplate* );

public:
	virtual ~CBase_AnalysisSample();

public://xml����Ʒ�ļ���ز���
	CString GetSampleName() const;
	bool SetSampleName(LPCTSTR lpcszName);//���³ɹ��������UpdateNames

	CString GetReportName() const;
	bool SetReportName(LPCTSTR lpcszName);

	int GetInjectNO() const;//������
	bool SetInjectNO(int iNO);//���³ɹ��������UpdateNames

	enumSampleType GetType() const;
	bool SetType(enumSampleType eType);
	LPCTSTR TypeDescription() const;
	static LPCTSTR ToTypeDescription(enumSampleType eType);
	static LPCTSTR ToTypeString(enumSampleType eType);
	static enumSampleType FromTypeString(const CString& typeString, enumSampleType eDefault =  SAMPLE_TEST);

	static CString ToInstrumentChannelString(int iSignalChannel){return CString((TCHAR)('A' + iSignalChannel));}
	int GetInstrumentChannel(void) const;//�����ź�ͨ��
	bool SetInstrumentChannel(int iChannel){return m_data.SetValue_Int(XMLPATH_SAMPLE_PARAM_INSTRU_SC, iChannel) == IAERROR_SUCCESS;}

	bool SetChannel(int iChannel){return m_data.SetValue_Int(XMLPATH_SAMPLE_PARAM_INSTRU_IC, iChannel) == IAERROR_SUCCESS;}

	CString GetSourceMethod() const;//��Ʒ�ļ��б����ֵ
	bool SetSourceMethod(LPCTSTR lpcszMethodName);

	enumDrawType GetDrawType() const;//�滭����
	bool SetDrawType(enumDrawType eDrawType);

public:
	CBase_AnalysisSample::STATUS GetStatus(void) const{return m_statusCurrent;}
	LPCTSTR GetStatusString(void) const;
	void SetStatus(CBase_AnalysisSample::STATUS status);

	bool SetAcquisitionTime(DWORD dwMinutes){return m_data.SetValue_Int(XMLPATH_SAMPLE_PARAM_INSTRU_AT, dwMinutes) == IAERROR_SUCCESS;}

	CString GetCurMethod() const;//��ǰʵ��ʹ�õķ���

	virtual void SetChartCtrl(CBase_SampleChart* pChartCtrl){m_pChartCtrl=pChartCtrl;}
	bool SaveChart2File(int iWidth, int iHeight, const CString& file);

	virtual void RefreshDraw(enumDrawType eDrawType);


public://����
	static CString CreateSampleFileName(CString cStrSampleName, int nInjectNO);
	CString GetSampleFileTitle() const;
	CString GetSampleFileName() const{return m_strSampleFileName;}

	CString CreateMainCurveFileName(void){CString cStrTemp; cStrTemp.Format(STRING_MAINCURVE_FILENAME, GetSampleName(), GetInjectNO()); return cStrTemp;}
	CString GetMainCurveFileName(void){return GetCurveFileName(STRING_MAINCURVE_GROUP, STRING_MAINCURVE_NAME);}
	CString CreateCurveFileName(const CString& cStrMainGroup, const CString& cStrMainSerie){CString cStrTemp; cStrTemp.Format(_T("%s#%d_%s@%s"), GetSampleName(), GetInjectNO(),cStrMainSerie, cStrMainGroup); return cStrTemp;}
	CString GetCurveFileName(const CString& cStrMainGroup, const CString& cStrMainSerie);

	bool GetIADataCurve2D_MainInfo(CString* pMainGroup, CString* pMainSerie) const//��Ʒ������������������е�������
	{
		//Ŀǰ�涨�����������ʽ�棬���m_xmlC2D�л�ȡ���������ڣ����Զ�����
		if(pMainGroup) *pMainGroup = _T("ResultPoints");
		if(pMainSerie) *pMainSerie = _T("AcquireData");
		return true;
	}
	bool IsStartRealtimeProcess() const;
	CPeakList& GetPeakList() { return m_cPeakList; }

	bool UpdateNames();
	bool UpdateSampleName(LPCTSTR lpszSampleName, int nInjectNO);

public:
	int GetIADataCurve2D(CIADataCurve2D& xmlC2D) const;
	
	BOOL Create(LPCTSTR lpszSampleName, int nInjectNO);
	BOOL Load(LPCTSTR lpszSampleName, int nInjectNO);
	BOOL LoadSampleFile(const CString& strPathName);
	BOOL Save();
	BOOL Delete();

	virtual void GetDataFrom(const CBase_AnalysisSample &src){m_data.SetXMLString(src.m_data.GetXMLString());}
	virtual void GetDataFrom(const CBase_AnalysisMethod &src);

	virtual void XML_ClearData();//������߽ڵ㡢����ڵ�(����ɾ����ص��ļ�)

protected:
	void CreateID();
	virtual	void XML_InitMainCurveDataAndClearOtherCurves(void);
	CBase_AnalysisSample::STATUS DetectSampleStatusAndLoadMainCurveData();
	void LoadMainCurveData(const CString& strMainCurvePathName);

public:
	static BOOL IsNameExist(LPCTSTR lpcszName, int nInjectNO,CBase_AnalysisTemplate* pTemplate);
	static BOOL Delete(LPCTSTR lpcszName, int nInjectNO, CBase_AnalysisTemplate* pTemplate);
	static BOOL GetSampleNameList(CStringArray &arrayNames, CBase_AnalysisTemplate* pTemplate);
	static BOOL GetReportNameList(CStringArray &arrayName, CBase_AnalysisTemplate* pTemplate);

public:
	CBase_AnalysisTemplate*	m_pTemplate;
	CAnalysisData	m_data;
	CCurveData		m_dataMainCurve;
	CBase_AnalysisMethod*	m_pMethod;//��ǰʵ��ʹ�õķ���
	CBase_SampleChart*		m_pChartCtrl;
	CString					m_strSavePath;		//��Ʒ�ļ�����·����ͼ���ļ���·�������ͬ��
	CString					m_strSampleFileName;
	CString					m_strOldSampleFileName;
	int	m_ptrRefCount; //ָ�����ü���

	COleDateTime m_dtStartRun,m_dtStopRun;//��ʼ�ͽ���������ʱ��
	bool m_bNeedStop;
	double m_dblAutoStopMins;
	bool m_bZero;
	DWORD m_dStartAcquireTime_mins;//��ʼ����ʱ�䣬���ʱ�䲻��������ʱ��


	//chain: ��ʱ�ӿڣ���ResultPeakList�ڻ�ȡ���߷ַ������
	int GetVLines();

private:
	//�ñ�������ֱ�Ӹ�ֵ(�����ڱ������������)���������ChangeStatus(),�˺�������һЩ֪ͨ
	CBase_AnalysisSample::STATUS	m_statusCurrent;

public:
	//����Ϊ��ʱ������������m_data���ݱ���ͬ����Ϊ�����Ч��
	// m_data�е����ݣ����𾫶ȣ������ڱ����ʾ���������������ڻ�ͼ������
	//���ֽ��
	CPeakList			m_cPeakList;
	CPeakList			m_cResultList;

	CPeakList			m_cAllRealTimePeakList; // ʵʱ���ַ�����ȫ���ģ�

	//ʵʱ���߻���
	CRealTime			m_RealTime;

	//��ʱ���ݣ�ÿ�λ��ֺ�ֵ
	CArray<double>		m_arrayMPW;	//��С��������֮ǰ�Ļ��ֽ��������
	CArray<double>		m_arrayMPA;	//��С���������֮ǰ�Ļ��ֽ��������
	CArray<double>		m_arrayMPH;	//��С�������֮ǰ�Ļ��ֽ��������
	int		m_nFilteredMPW;
	int		m_nFilteredMPA;
	int		m_nFilteredMPH;
	CArray<structLine>	m_arrayVLines;		//��ֱ�ַ���
	double				m_dMaxThreShold;	//��������б��
	double				m_dMinThreShold;	//��С������б��

public:
	bool Xml_PutParameter_Method(CAnalysisData& adMethod);
	bool Xml_PutParameter_Method(LPCTSTR xmlString);//����������������ļ���xml�ı�������Method��ǣ�
	bool Xml_PutParameter_Instrument(LPCTSTR xmlString);//�����������Method/Instrument��ȫ�����ݣ�����Instrument��ǣ�
	bool Xml_PutParameter_Instrument2(LPCTSTR xmlString);//�����������Method/Instrument��ȫ�����ݣ�����Instrument��ǣ�
	bool Xml_PutParameter_Calibrate(LPCTSTR xmlString);//�����������Method/Calibrate��ȫ�����ݣ�����Calibrate��ǣ�
	bool Xml_PutParameter_PeakProcess(LPCTSTR xmlString);
	bool Xml_PutParameter_Report(LPCTSTR xmlString);
	bool Xml_GetParameter_Method(CString& xmlString) const;
	bool Xml_GetParameter_Instrument(CString& xmlString) const;
	bool Xml_GetParameter_Calibrate(CString& xmlString) const;
	bool Xml_GetParameter_PeakProcess(CString& xmlString) const;
	bool Xml_GetParameter_Report(CString& xmlString) const;

	CAnalysisData& GetAnalysisData(){return m_data;}

public:
	virtual int Base_RealtimeData(const CCurveData &curveData);

	virtual BOOL Base_IsReady(void){if(GetStatus()==ready) return TRUE; else return FALSE;}
	virtual BOOL Base_GetReady(void);//�ú�����һ���������Ʒ���Լ����жϻ������������Ʒ�������ǰ��׼��
	virtual void Base_ResetData(void);
	virtual int	 Base_StartRun(double dTimeMins);
	virtual int  Base_StopRun(void);
	virtual int  Base_AbortReady(void);
	virtual int  Base_AbortRun(void);
	virtual int	 Base_DataProcess(void);
	virtual int	 Base_DataCalibrate(void);
	virtual int  Base_ReportPrint(BOOL bAutoPrint=TRUE)=0;
};

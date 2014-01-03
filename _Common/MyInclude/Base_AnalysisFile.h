#pragma once
#include "AnalysisData.h"
#include "Base_SampleChart.h"


class CBase_AnalysisTemplate;
class CTaskItemOption;
class CBase_AnalysisMethod;
class CBase_AnalysisInstrument;

#define ANALYSIS_SAMPLE_FILE_EXT _T(".cg.smp")
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
		struct {
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
	BOOL SyncFromXML_PeakListTable(CIADataTable* pCIADataTable);
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
public:
	CBase_AnalysisSample(LPCTSTR cStrName, int nInjectNO, CBase_AnalysisTemplate* );
	virtual ~CBase_AnalysisSample();

	//��Ʒ���в���
	CBase_AnalysisTemplate*	m_pTemplate;		//����ģ��
	DWORD					m_dStartAcquireTime_mins;//��ʼ����ʱ�䣬���ʱ�䲻��������ʱ��
	CString					m_cStrSavePath;		//��Ʒ�ļ�����·����ͼ���ļ���·�������ͬ��
	CString					m_cStrMainFileName;	//��Ʒ�ļ�������


	//
	//---------------------------------------------------------------------
	//-----------------------------��Ʒ����--------------------------------
	//---------------------------------------------------------------------
	// 
	public:
		CAnalysisData			m_data;				//��Ʒ���в���
		//����Ϊ��ʱ������������m_data���ݱ���ͬ����Ϊ�����Ч��
		//���ֽ��
		CPeakList			m_cPeakList;
		CPeakList			m_cResultList;

		//��ʱ���ݣ�ÿ�λ��ֺ�ֵ
		CArray<double>		m_arrayInterRes1;	//��С��������֮ǰ�Ļ��ֽ��������
		CArray<double>		m_arrayInterRes2;	//��С���������֮ǰ�Ļ��ֽ��������
		CArray<double>		m_arrayInterRes3;	//��С�������֮ǰ�Ļ��ֽ��������
		struct structLine
		{
			struct structPoint
			{
				double x,y;
			}begin,end;
		};
		CArray<structLine>	m_arrayVLines;		//��ֱ�ַ���
		double				m_dMaxThreShold;	//��������б��
		double				m_dMinThreShold;	//��С������б��
	protected:
		//�����������
		CCurveData		m_dataMainCurve;

	//
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	//---------------------------------------------------------------------
	// 


private:
	//�����ߴ����ļ���
//	CString			m_cStrCurveFileName;
	bool m_bDeleted;




public:

	//
	//--------------��Ʒ�����Ķ�д-------------------
	//
	//��Ʒ����
	enumSampleType GetType() const;
	bool SetType(enumSampleType eType);
	LPCTSTR TypeDescription() const;
	static LPCTSTR ToTypeDescription(enumSampleType eType);
	static LPCTSTR ToTypeString(enumSampleType eType);
	static enumSampleType FromTypeString(const CString& typeString, enumSampleType eDefault =  SAMPLE_TEST);
	static CString ToInstrumentChannelString(int iSignalChannel){return CString((TCHAR)('A' + iSignalChannel));}

	//��Ʒ����
	CString GetSampleName() const;//���������ţ����������������������ļ���
	bool ChangeSampleName(LPCTSTR lpcszName, int nInjectNO);
	//����׺���ļ�������������·��������������
	//�˺���ֻ�ڵ�һ������Ʒ��ʱ��ʹ��
	
	static CString CreateSampleFileName(CString cStrSampleName, int nInjectNO) {CString cStrTemp;cStrTemp.Format(_T("%s_#%d%s"), cStrSampleName, nInjectNO, ANALYSIS_SAMPLE_FILE_EXT);return cStrTemp;}
	CString CreateSampleFileName() {return CreateSampleFileName(GetSampleName(), GetInjectNO());}
	CString GetSampleFileName() const{return m_cStrMainFileName;}//����׺���ļ�������������·��������������

	//������
	int GetInjectNO() const;
	bool SetInjectNO(int iNO);

	//�����ź�ͨ��
	int GetInstrumentChannel(void) const;

private:
	friend class CAnalysisTaskItem;
	bool SetInstrumentChannel(int iChannel){return m_data.SetValue_Int(XMLPATH_SAMPLE_PARAM_INSTRU_SC, iChannel) == IAERROR_SUCCESS;}
	bool SetChannel(int iChannel){return m_data.SetValue_Int(XMLPATH_SAMPLE_PARAM_INSTRU_IC, iChannel) == IAERROR_SUCCESS;}
	bool SetAcquisitionTime(DWORD dwMinutes){return m_data.SetValue_Int(XMLPATH_SAMPLE_PARAM_INSTRU_AT, dwMinutes) == IAERROR_SUCCESS;}

public:
	//���õķ���
	CString GetSourceMethod() const;
	bool SetSourceMethod(LPCTSTR lpcszMethodName);
	CString GetCurMethod() const;

	//��������
	//int GetCount() const;
	//bool SetCount(int iCount);

	//�滭����
	enumDrawType GetDrawType() const;
	bool SetDrawType(enumDrawType eDrawType);

	//�ۺ�����
	//bool SetAttribute(LPCTSTR lpcszName, LPCTSTR lpcszMethodName, int iCount);

	int GetIADataCurve2D(CIADataCurve2D& xmlC2D) const;
	//��Ʒ������������������е�������
	bool GetIADataCurve2D_MainInfo(CString* pMainGroup, CString* pMainSerie) const
	{
		//Ŀǰ�涨�����������ʽ�棬���m_xmlC2D�л�ȡ���������ڣ����Զ�����
		if(pMainGroup) *pMainGroup = _T("ResultPoints");
		if(pMainSerie) *pMainSerie = _T("AcquireData");
		return true;
	}

	//������غ���
	CString CreateMainCurveFileName(void){CString cStrTemp; cStrTemp.Format(STRING_MAINCURVE_FILENAME, GetSampleName(), GetInjectNO()); return cStrTemp;}
	CString GetMainCurveFileName(void){return GetCurveFileName(STRING_MAINCURVE_GROUP, STRING_MAINCURVE_NAME);}
	CString CreateCurveFileName(const CString& cStrMainGroup, const CString& cStrMainSerie){CString cStrTemp; cStrTemp.Format(_T("%s#%d_%s@%s"), GetSampleName(), GetInjectNO(),cStrMainSerie, cStrMainGroup); return cStrTemp;}
	CString GetCurveFileName(const CString& cStrMainGroup, const CString& cStrMainSerie);





public:
	//
	//-----------------��������-----------------------------
	//
	//��Ʒ��ǰ״̬
	CBase_AnalysisSample::STATUS GetStatus(void) const{return m_statusCurrent;}
	LPCTSTR GetStatusString(void) const
	{
		switch(m_statusCurrent)
		{
			case null:return _T("����Ʒ");
			case preparing:return _T("��Ʒ׼��");
			case ready:return _T("��Ʒ׼����");		
			case running:return _T("������...");		
			case idle:return _T("��������");		
			case finish:return _T("���");		
			case abort:return _T("�쳣��ֹ");		
			case busy:return _T("��Ʒæ");
			default:return _T("δ֪״̬");
		}
	}
	void SetStatus(CBase_AnalysisSample::STATUS status);
	private:
		//�ñ�������ֱ�Ӹ�ֵ(�����ڱ������������)���������ChangeStatus(),�˺�������һЩ֪ͨ
		CBase_AnalysisSample::STATUS	m_statusCurrent;


public:
	//static BOOL IsNameExist(LPCTSTR, CBase_AnalysisTemplate*);
	static BOOL IsNameExist(LPCTSTR lpcszName, int nInjectNO,CBase_AnalysisTemplate* pTemplate);

	static BOOL Delete(LPCTSTR, CBase_AnalysisTemplate*);
	static BOOL GetSampleNameList(CStringArray &arrayName, CBase_AnalysisTemplate*);

	//��XML������صĺ���
protected:
	virtual	void XML_InitMainCurveData(void);
	void UpdateXMLCurve2DNames();

public:
		bool Xml_PutParameter_Method(CAnalysisData& adMethod);
		bool Xml_PutParameter_Method(LPCTSTR xmlString);//����������������ļ���xml�ı�������Method��ǣ�
		bool Xml_PutParameter_Instrument(LPCTSTR xmlString);//�����������Method/Instrument��ȫ�����ݣ�����Instrument��ǣ�
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
	//
	//-----------------��������-----------------------------
	//
	virtual void GetDataFrom(const CBase_AnalysisSample &src){m_data.SetXMLString(src.m_data.GetXMLString());}
	virtual void DuplicateFrom(const CBase_AnalysisSample &src){GetDataFrom(src);m_pTemplate = src.m_pTemplate;}
	virtual void GetDataFrom(const CBase_AnalysisMethod &src);
	 
	void CreateID();
	//ͨ����Ʒ���ƴӱ�׼Ŀ¼�е���
	BOOL Load(LPCTSTR lpszSampleName,int nInjectNO);
	BOOL LoadSampleFile(CString cStrFullPathName);
	BOOL Save();
	BOOL Delete();

public:
	//
	//-------------��������Ʒ��������------------------
	//
	virtual BOOL Base_IsReady(void){if(GetStatus()==ready) return TRUE; else return FALSE;}
	virtual BOOL Base_GetReady(void);//�ú�����һ���������Ʒ���Լ����жϻ������������Ʒ�������ǰ��׼��
	virtual void Base_ResetData(void);
	virtual int	 Base_StartRun(double dTimeMins);
	virtual int  Base_StopRun(void);
	virtual int  Base_AbortRun(void);
	virtual int	 Base_DataProcess(void);
	virtual int	 Base_DataCalibrate(void);
	virtual int  Base_ReportPrint(BOOL bAutoPrint=TRUE)=0;

public:
	CBase_AnalysisMethod*	m_pMethod;			//���õķ���
	CBase_SampleChart*		m_pChartCtrl;		//��Ӧ�����ߴ���
	virtual void SetChartCtrl(CBase_SampleChart* pChartCtrl){m_pChartCtrl=pChartCtrl;}
	virtual void RefreshDraw(enumDrawType eDrawType);
	COleDateTime m_dtStartRun,m_dtStopRun;//��ʼ�ͽ���������ʱ��
	bool m_bNeedStop;
	DWORD m_dwAutoStopMins;


	//-----------��������ĺ������̾���ȷ��------------------------------------
	//-----------------------------------------------------------------------
public:
	virtual int Base_RealtimeData(const CCurveData &curveData);

public:
	bool SaveChart2File(int iWidth, int iHeight, const CString& file);
};

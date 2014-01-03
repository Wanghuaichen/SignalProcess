#pragma once


class CBase_AnalysisTemplate;

class CBase_AnalysisMethod : public CObject
{
public:
	CBase_AnalysisMethod(CString cStrName, CBase_AnalysisTemplate *pcTemplate);
	virtual ~CBase_AnalysisMethod(){;}
	//�Ƿ񷽷��Ѿ�׼���ã��ؼ���Ӳ�������Ƿ�λ�������ļ��Ƿ񶼽�����
	BOOL m_bReady;

	//private:
	//����������������
	CAnalysisData	m_cCurrentMethodData;
	//����ģ��ָ��
	CBase_AnalysisTemplate  *m_pcTemplate;

public:
	//��������
	//�����������⣬������������
	void GetDataFrom(CBase_AnalysisMethod &other);

	// �жϸ÷����ļ��Ƿ����
	BOOL IsNameExist(CString cStrName);
	BOOL GetMethodNameList(CStringArray &arrayName);
	static BOOL GetMethodNameList(CBase_AnalysisTemplate *pcTemplate, CStringArray &arrayName);
	CString GetFilePathName() const;
	static CString GetFilePathName(const CString& strMethodName, CBase_AnalysisTemplate* pcTemplate);
	//�õ���ǰ��������
	CString GetName(void) const;
	//���е�ǰ�������ɹ���m_bReady=TRUE;
	BOOL Run(void);
	BOOL IsReady(void);
	//ͨ�����Ƶ��뷽��
	BOOL Load(CString cStrName);
	//���淽��
	BOOL Save(void);
	BOOL GetInfoString(CString& info);	

	BOOL SaveInstrumentParam(CString cStrXMLPath, CString xmlString);
//	BOOL SetInstrumentParam(CString cStrXMLPath, BOOL bDataToWindow);

	BOOL RunToStruct(LPVOID lpvStruct);
	BOOL SetInstrumentParamToStruct(CString cStrXMLPath, LPVOID lpvStruct);

	//BOOL GetInstrumentParamSize(int& nCols, int& nRows);
	//BOOL GetInstrumentParam(int* pMZ,   CString* pCompoundName, double* pSensitivity, double* pRationTable,  double* pNoise);
	CString GetInstrumentParam(CString cStrXMLPath);
	void PreviewCurruentMethod();
	BOOL SaveAs(const CString& name);//�����ƣ�������·���ͺ�׺
	BOOL Construct();

	CAnalysisData& GetAnalysisData() { return m_cCurrentMethodData; }
private:


};


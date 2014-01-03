#pragma once

//#include "Base_AnalysisTemplate.h"

class CBase_AnalysisTemplate;

class CBase_AnalysisInstrument : public CObject {
public:
	friend class CBase_AnalysisTemplate;

protected:
	CBase_AnalysisInstrument(CBase_AnalysisTemplate*  pcTemplate=NULL);
	virtual ~CBase_AnalysisInstrument();


	//ϵͳģ��
	CBase_AnalysisTemplate* m_pTemplate;

	//���ӵ��������ϵ���Ʒ������ʵʱ���ݣ�
	//CBase_AnalysisSample*	m_pcSampleConnect;

	//��������
	CAnalysisData m_dataSet;		//�����趨����
	CAnalysisData m_dataCurrent;	//����ʵʱ����

	//�����Ƿ����ӣ���������Ƿ����������ߣ�
	BOOL m_bConnect;


	//�Ƿ�����׼����


	//-----------��������ĺ������̾���ȷ��------------------------------------
	//-----------------------------------------------------------------------
public:
	virtual BOOL Base_IsReady()=0;
	virtual BOOL Instrument_IsReady() = 0;
	BOOL SaveToFile(LPCTSTR lpcszFilename, BOOL bCurrentParam=TRUE);
	BOOL LoadFromFile(LPCTSTR lpcszFilename, BOOL bCurrent=TRUE);
	virtual int	 Base_SetParam(const CString &cStrFullXMLpath, const CString &cStrXML);
	//�����������·�����������ִ�С����pXMLStringΪ�վ��·� m_dataSet, ����洢pXMLString��m_dataSet��Ȼ���·�
	virtual int	 Base_DownloadToDevice(const CString &XMLString)=0;
	virtual int  Base_UploadFromDevice(CString& xmlString)=0;//���豸���أ����浽��Ա����
	virtual int  Base_StartRequire(int nChannel)=0;
	virtual int  Base_StopNotify(int nChannel)=0;
	virtual BOOL Base_IsChannelAvailable(int nChannel)=0;
	virtual int  Base_GetChannelNum(void)=0;


protected:
	//������������
	virtual BOOL Initialize(void)=0;
	//virtual int  Base_AttachSample(CBase_AnalysisSample * pcSample){if(!m_pcSampleConnect){ m_pcSampleConnect = pcSample; return IAERROR_SUCCESS;} else return IAERROR_CG_INSTRUMENT_BUSY;}
	//virtual void Base_ReleaseSample(void){m_pcSampleConnect = NULL;}

	//�����ڲ�ʹ�ã����յ�ʵʱ���ݺ����
	virtual void  Base_SendRealTimeData(CCurveData &curveData, int nChannel);

};

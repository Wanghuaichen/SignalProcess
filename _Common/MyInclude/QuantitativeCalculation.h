#pragma once
#include "CaliData.h"


/* 
	######################################################################################################################################

				������ΪУ�����ڶ�δ֪��Ʒ���з�ƥ��Ͷ��������ҵ���߼���
				Ҳ���Կ��ǰѷ�ƥ��Ͷ�������ָ��2����

	######################################################################################################################################
*/


class CQuantitativeCalculation  
{
public:
	 
	CQuantitativeCalculation(CAnalysisData *pAnalysisData);
	~CQuantitativeCalculation(void);

	//����ӿ�:���������������
	int QuantitativeCalculationProcess(); 

private:
	typedef map<int, CaliItem> MatchType;
	 

	CString m_Method; //У������
	CString m_Object; //У������

	CCaliData m_cDuplicate;  
	CAnalysisData *pcData;
	MatchType m_Result;

	bool SetCaliData();
	bool SetMethodAndObject();
	bool ReadyCheckPass();  
	CString GetCalibrateObjectAttr();   

	//��ƥ��
	void GetRef_IntPeakRow_ByArea_or_Height(MatchType &PeakRows);  
	void Remove_Repeat_PeakRow(MatchType &result);  
	void GetOtherPeakRow_ByResTime(double compResTime, MatchType &PeakRows);  
	int MatchRefReak();  
	int MatchIntReak();   
	int MatchOtherReak();   

	//����ת��
	void ClearTableRowData(CIADataTable *pTable);  //��ձ���������ݣ���������   
	void PeakListData2ResultData(const CStringArray &PeakList, CStringArray &ResultData, int nID);  
	void GenerateResultData(); //��m_Result����ResultData 
	void DirectGenerateResultData(); //ֱ����m_cDuplicate����ResultData
	
	//��������
	void QC_percentage(); //�������ٷֱȷ�  
	void QC_external(); //��������귨  
	void QC_internal(); //�������ڱ귨  
	void QC_external_percentage(); //���������ٷֱȷ� 
	void QC_internal_percentage(); //�������ڱ�ٷֱȷ�  
	void QC_normalization(); //����������һ����  
	CString ConvertScientificNotation(double contents); //ת���ɷ��Ͽ�ѧ��������

	double GetProductFactor();
	double GetDilutionFactor();
	double GetSampleAmount();
	double GetColumnTotal(CIADataTable *cTable, const CString& strColumn);  
	int FindMatchedCompoundGroupRow(const CString& cStrInteriorNo);
	void FindInteriorContentAndAttr(const CString& cStrInteriorNo, CaliItem& IntItem);
	void FindReMatchedPeakListItem(const CString& cStrInteriorNo, CaliItem& peakListItem); //���ڱ���ݷ�ƥ�䵽Peaklistĳ������
	int QuantitativeCalculation();
};


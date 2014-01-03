#include "StdAfx.h"
#include "QuantitativeCalculation.h"


CQuantitativeCalculation::CQuantitativeCalculation(CAnalysisData *pAnalysisData)
	:pcData(pAnalysisData) 
{
}

CQuantitativeCalculation::~CQuantitativeCalculation(void)
{
}

int CQuantitativeCalculation::QuantitativeCalculationProcess() {
	if (!ReadyCheckPass()) return IAERROR_XML_NOTVALID;  //����������ʼ�����޷�����֮���ҵ���߼�
	
	//��ƥ�����

	//��ȡ��ǲο���
	//���ڲο��������ڵķ壬Ϊ�ο��壻���ж����������ͬ���䣬������ȡ�������Ϊ�ο���
	//�ҵ����вο������չ�ʽ������У������������ı���ʱ��
	if(MatchRefReak()!=IAERROR_SUCCESS) return IAERROR_XML_NOTVALID; 

	//��ȡ����ڱ��
	//�����������ı���ʱ�䴰�ڣ�ʶ���ڱ�塣�����ڶ����������ȡ�������ѡ����
	if(MatchIntReak()!=IAERROR_SUCCESS) return IAERROR_XML_NOTVALID;  

	//��ȡ������
	//ʶ������У���壬�����ڶ����ȡ����ʱ����ӽ���
	if(MatchOtherReak()!=IAERROR_SUCCESS) return IAERROR_XML_NOTVALID; 

	//���ݷ�ƥ�������У����ɽ��������
	if (m_Method == _T("�ٷֱȷ�")) {
		DirectGenerateResultData();
		return IAERROR_SUCCESS;
	}
	else
		GenerateResultData();

	//�����������
	if(QuantitativeCalculation()!=IAERROR_SUCCESS) return IAERROR_XML_NOTVALID;

	CString szWatch = pcData->GetXMLString();
	return IAERROR_SUCCESS;
}

bool CQuantitativeCalculation::SetMethodAndObject() {
	CString szWatch = pcData->GetXMLString();
	if(pcData->GetValue_String(XMLPATH_SAMPLE_PARAM_CALI_METHOD, m_Method)!=IAERROR_SUCCESS) return false;
	if(pcData->GetValue_String(XMLPATH_SAMPLE_PARAM_CALI_OBJECT, m_Object)!=IAERROR_SUCCESS) return false;
	return true;
}

bool CQuantitativeCalculation::SetCaliData() {
	if (pcData==NULL) return false;
	CIADataTable cCalibrateResultTable;
	//if (m_Method == _T("�ٷֱȷ�")) {
	//	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cCalibrateResultTable) != IAERROR_SUCCESS) return false;
	//}
	//else
	//{
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_PARAM_CALI_RESULT_TABLE, cCalibrateResultTable) != IAERROR_SUCCESS) return false;
	//}
	

	//if(!m_cDuplicate.AutoAddItem(&cCalibrateResultTable, XMLPATH_SAMPLE_PARAM_CALI_RESULT_TABLE)) return false;
	m_cDuplicate.InitCaliData(&cCalibrateResultTable);
	return true;
}

bool CQuantitativeCalculation::ReadyCheckPass() {
	if (pcData==NULL) return false;
	if (!SetMethodAndObject()) return false;
	if (m_cDuplicate.GetItemSize()==0) 
		if (!SetCaliData())
			return false;
	return true;
}

/************************************************************************************************************************************
/
/                                                  ��ƥ�� ��غ���
/
/************************************************************************************************************************************/

void CQuantitativeCalculation::GetRef_IntPeakRow_ByArea_or_Height(MatchType &PeakRows) {
	if (PeakRows.empty()) return;
	//������ȡ�������Ϊ�ο���
	if (PeakRows.size()>=2) {
		CString szAttr=GetCalibrateObjectAttr();

		CIADataTable cPeakList;
		if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cPeakList) != IAERROR_SUCCESS) return;

		MatchType::iterator iter=PeakRows.begin(); 

		int maxVal_Row=iter->first;			
		double maxVal=0.0f;

		for ( ; iter!=PeakRows.end(); ++iter) {
			CString szVal;
			int row = iter->first;
			if (!cPeakList.GetCellValue(row, szAttr, szVal)) return;
			double dVal=_tstof(szVal);
			if (dVal > maxVal)	{
				maxVal = dVal;
				maxVal_Row = row;
			}
		}
		m_Result.insert(make_pair(maxVal_Row, PeakRows[maxVal_Row]));
	}
	else
		m_Result.insert(PeakRows.begin(), PeakRows.end());
}


void CQuantitativeCalculation::Remove_Repeat_PeakRow(MatchType &result){
	MatchType tmpRow;
	typedef MatchType::iterator Iter;
	typedef MatchType::const_iterator CIter;

	bool repeat=false;	
	for (Iter restIter=result.begin(); restIter!=result.end(); ++restIter) {
		repeat = false; // ���ñ�ǣ���Ȼһ���ظ���Ĭ��ȫ���ظ����
		for (CIter sourIter=m_Result.begin(); sourIter!=m_Result.end(); ++sourIter) {
			if (restIter->first==sourIter->first)
				repeat=true;
		}
		if (!repeat)
			tmpRow.insert(make_pair(restIter->first, restIter->second));
	}
	tmpRow.swap(result);
}


void CQuantitativeCalculation::GetOtherPeakRow_ByResTime(double compResTime, MatchType &PeakRows) {
		
	if (PeakRows.empty()) return;
	//����ӽ�����ʱ��Ϊ������
		if (PeakRows.size()>=2) {
			CIADataTable cPeakList;
			if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cPeakList) != IAERROR_SUCCESS) return;
	
			MatchType::iterator iter=PeakRows.begin();
	
			int minDiff_Row=iter->first;			
			double minDiff=static_cast<double>(INT_MAX);
	
			for ( ; iter!=PeakRows.end(); ++iter) {
				CString szResTime;
				int row=iter->first;
				if (!cPeakList.GetCellValue(row, _T("ReserveTime"), szResTime)) return;
				double dResTime=_tstof(szResTime);
				if (fabs(dResTime-compResTime)<minDiff) {
					minDiff = fabs(dResTime-compResTime);
					minDiff_Row = row;
				}
			}
			m_Result.insert(make_pair(minDiff_Row, PeakRows[minDiff_Row]));
		}
		else
			m_Result.insert(PeakRows.begin(), PeakRows.end());
}


int CQuantitativeCalculation::MatchRefReak() {
	
	//*ȡm_cCaliData�еĲο���
	CCaliData cRefPeak;
	m_cDuplicate.GetRefPeakData(cRefPeak);
	//*��ȡcPeakList�����ȡ����ʱ��
	CIADataTable cPeakList;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cPeakList) != IAERROR_SUCCESS) return IAERROR_XML_NOTVALID;
	CStringArray caResTime;
	cPeakList.GetColumnValues(_T("ReserveTime"), caResTime);
	double dRefVal; //�ο��崰��ֵ

	CString watch = pcData->GetXMLString();
	if (pcData->GetValue_Double(XMPPATH_SAMPLE_PARAM_CALI_REFERENCEPEAK, dRefVal) != IAERROR_SUCCESS) return IAERROR_XML_NOTVALID;
	
	double dlower=1.0f-dRefVal/100.0f;//���ڿ�
	double dupper=1.0f+dRefVal/100.0f;//���ڱ�
	//*�ο��屣��ʱ��
	for (int i=0; i<cRefPeak.GetItemSize(); ++i) { //��Ӧÿһ���ο���
		bool IsMatch = false;
		CString szRefPeak_ResTime;
		cRefPeak.GetReserveTime(i,szRefPeak_ResTime);
		double dRefPeak_ResTime=_tstof(szRefPeak_ResTime);
		double dRefPeak_lower=dRefPeak_ResTime*dlower;
		double dRefPeak_upper=dRefPeak_ResTime*dupper;

		MatchType RefPeakRows;
		for (long row=0; row<cPeakList.GetRowsNumber(); ++row) { //��Ӧÿһ��PeakList�ڷ�
			CString szPeakList_ResTime;
			if (!cPeakList.GetCellValue(row, _T("ReserveTime"), szPeakList_ResTime)) return IAERROR_XML_NOTVALID;
			double dPeakList_ResTime=_tstof(szPeakList_ResTime);
			if (dPeakList_ResTime>dRefPeak_lower && dPeakList_ResTime<dRefPeak_upper){ //PeakList����ο��� 
				CaliItem cItem = cRefPeak.GetRowDate(i);
				RefPeakRows.insert(make_pair(row,cItem));
				IsMatch = true;
			}
		}
		//��ǰ�ο���δ��ƥ���ϣ�ȡ���ο�����
		if (!IsMatch)
			m_cDuplicate.CancelRefPeakMark(i);
			
		//������ȡ�������Ϊ�ο���
		GetRef_IntPeakRow_ByArea_or_Height(RefPeakRows);
	}

	////�ҵ����вο������չ�ʽ������У������������ı���ʱ��
	vector<double> afterResTime;
	typedef map<int, CaliItem>::iterator Iter;
	for (Iter iter=m_Result.begin(); iter!=m_Result.end(); ++iter)
	{
		CString afterRest;
		int row = iter->first;
		if (!cPeakList.GetCellValue(row, _T("ReserveTime"), afterRest)) return IAERROR_XML_NOTVALID;
		double rt = _tstof(afterRest);
		afterResTime.push_back(rt);
	}

	m_cDuplicate.ModifyResTime(afterResTime);

	return IAERROR_SUCCESS;
}


int CQuantitativeCalculation::MatchIntReak() {
	//��ȡ����ڱ��
	CCaliData cIntPeak;
	m_cDuplicate.GetIntPeakData(cIntPeak); //��Ҫ�Ӹ����л��

	//�����������ı���ʱ�䴰�ڣ�ʶ���ڱ�� �����ڶ����������ȡ�������ѡ����

	CIADataTable cPeakList;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cPeakList) != IAERROR_SUCCESS) return IAERROR_XML_NOTVALID;

	double dWndVal; //�����崰��ֵ
	if (pcData->GetValue_Double(XMPPATH_SAMPLE_PARAM_CALI_OTHERCALIPEAK, dWndVal) != IAERROR_SUCCESS) return IAERROR_XML_NOTVALID;
	double dlower=1.0f-dWndVal/100.0f;//���ڿ�
	double dupper=1.0f+dWndVal/100.0f;//���ڱ�

	//*�ڱ�屣��ʱ��
	for (int i=0; i<cIntPeak.GetItemSize(); ++i) { //��Ӧÿһ���ڱ��
		bool IsMatch = false;
		
		CString szIntPeak_ResTime;
		cIntPeak.GetReserveTime(i, szIntPeak_ResTime);
		double dIntPeak_ResTime=_tstof(szIntPeak_ResTime);
		double dIntPeak_lower=dIntPeak_ResTime*dlower;
		double dIntPeak_upper=dIntPeak_ResTime*dupper;

		MatchType IntPeakRows;
		for (long row=0; row<cPeakList.GetRowsNumber(); ++row) { //��Ӧÿһ��PeakList�ڷ�
			CString szPeakList_ResTime;
			if (!cPeakList.GetCellValue(row, _T("ReserveTime"), szPeakList_ResTime)) return IAERROR_XML_NOTVALID;
			double dPeakList_ResTime=_tstof(szPeakList_ResTime);
			if (dPeakList_ResTime>dIntPeak_lower && dPeakList_ResTime<dIntPeak_upper){ //PeakList����ڱ��
				CaliItem cItem = cIntPeak.GetRowDate(i);
				IntPeakRows.insert(make_pair(row, cItem));
				IsMatch = true;
			}
		}
		
		//��ǰ�ڱ��δ��ƥ���ϣ�ȡ���ڱ����
		if (!IsMatch)
			m_cDuplicate.CancelIntPeakMark(i);

		//ȥ���ظ��ķ��к�
		Remove_Repeat_PeakRow(IntPeakRows);

		//������ȡ�������Ϊ�ڱ��
		GetRef_IntPeakRow_ByArea_or_Height(IntPeakRows);
	}
	return IAERROR_SUCCESS;
}


int CQuantitativeCalculation::MatchOtherReak(){
	//��ȡʣ���
	CCaliData cOtherPeak;
	m_cDuplicate.GetcOtherPeakData(cOtherPeak); 

	//ʶ������У���壬�����ڶ����ȡ����ʱ����ӽ���

	CIADataTable cPeakList;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cPeakList) != IAERROR_SUCCESS) return IAERROR_XML_NOTVALID;

	double dWndVal; //�����崰��ֵ
	if (pcData->GetValue_Double(XMPPATH_SAMPLE_PARAM_CALI_OTHERCALIPEAK, dWndVal) != IAERROR_SUCCESS) return IAERROR_XML_NOTVALID;
	double dlower=1.0f-dWndVal/100.0f;//���ڿ�
	double dupper=1.0f+dWndVal/100.0f;//���ڱ�

	//*�����屣��ʱ��
	for (int i=0; i<cOtherPeak.GetItemSize(); ++i) { //��Ӧÿһ��������
		CString szOtherPeak_ResTime;
		cOtherPeak.GetReserveTime(i, szOtherPeak_ResTime);
		double dOtherPeak_ResTime=_tstof(szOtherPeak_ResTime);
		double dOtherPeak_lower=dOtherPeak_ResTime*dlower;
		double dOtherPeak_upper=dOtherPeak_ResTime*dupper;

		MatchType OtherPeakRows;
		for (long row=0; row<cPeakList.GetRowsNumber(); ++row) { //��Ӧÿһ��PeakList�ڷ�
			CString szPeakList_ResTime;
			if (!cPeakList.GetCellValue(row, _T("ReserveTime"), szPeakList_ResTime)) return IAERROR_XML_NOTVALID;
			double dPeakList_ResTime=_tstof(szPeakList_ResTime);
			if (dPeakList_ResTime>dOtherPeak_lower && dPeakList_ResTime<dOtherPeak_upper){ //PeakList����ο���
				 CaliItem cItem = cOtherPeak.GetRowDate(i);
				 OtherPeakRows.insert(make_pair(row, cItem));
			}
		}

		//ȥ���ظ��ķ��к�
		Remove_Repeat_PeakRow(OtherPeakRows);

		//����ӽ�����ʱ��Ϊ������
		GetOtherPeakRow_ByResTime(dOtherPeak_ResTime, OtherPeakRows);
	}

	return IAERROR_SUCCESS;
}





/************************************************************************************************************************************
/
/                                                   ��������
/
/************************************************************************************************************************************/

void CQuantitativeCalculation::ClearTableRowData(CIADataTable *pTable) {
	if (pTable==NULL) ASSERT(TRUE);
	long row = pTable->GetRowsNumber();
	while (row!=0) {
		pTable->DeleteRow(0);
		row = pTable->GetRowsNumber();
	}
}


void CQuantitativeCalculation::PeakListData2ResultData(const CStringArray &PeakList, CStringArray &ResultData, int nID) {
	const int ResultDataRow = 23;
	const int PeakListRow = 23;
	if (PeakList.GetSize()!=PeakListRow) return;
	if (!ResultData.IsEmpty()) ResultData.RemoveAll();

	for (int i=0; i<ResultDataRow; ++i) {
		CString szVal(_T(""));
		ResultData.Add(szVal);
	}

	CString szID;
	szID.Format(_T("%d"), nID);
	ResultData[1]=szID;			//���

	ResultData[0]=_T("{[(BITMAP_PREDEF:_Action_Info.bmp)]}");
	ResultData[2]=PeakList[2]; //����ʱ��
	ResultData[3]=PeakList[17]; //�����
	ResultData[4]=PeakList[3]; //����
	ResultData[5]=PeakList[4]; //�����
	ResultData[6]=PeakList[6]; //���
	ResultData[7]=PeakList[5]; //�����%
	ResultData[8]=PeakList[19]; //���%

	if (m_Method == _T("�ٷֱȷ�"))
	{
		if (m_Object==_T("�����"))
			ResultData[9]=PeakList[5]; //����
		else if (m_Object==_T("���"))
			ResultData[9]=PeakList[19]; //����
	}
	
	ResultData[10]=_T("{[(BITMAP_PREDEF:_Action_Delete.bmp)]}");

	ResultData[11]=PeakList[18]; //����
	ResultData[12]=PeakList[9]; //�Գ�����
	ResultData[13]=PeakList[21]; //��������
	ResultData[14]=PeakList[8]; //������
	ResultData[15]=PeakList[22]; //�����
	ResultData[16]=PeakList[10]; //�����X
	ResultData[17]=PeakList[13]; //�����Y
	ResultData[18]=PeakList[14]; //�嶥��X
	ResultData[19]=PeakList[15]; //�嶥��Y
	ResultData[20]=PeakList[11]; //�����X
	ResultData[21]=PeakList[16]; //�����Y
	ResultData[22]=PeakList[7];  //���
}

void CQuantitativeCalculation::GenerateResultData() {
	
	CIADataTable cPeakList;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cPeakList) != IAERROR_SUCCESS) return;

	CIADataTable cResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
	ClearTableRowData(&cResultTable);

	typedef MatchType::iterator Iter;
	int row=0;
	for (Iter iter=m_Result.begin(); iter!=m_Result.end(); ++iter) {
		row++;
		CStringArray caPeakListRowData, caResultRowData;
		int peakRows=iter->first;
		//�������ֵ
		CString szCompName=(iter->second).szComponentName;
		if(!cPeakList.SetCellValue(peakRows, _T("ComponentName"), szCompName)) continue;

		if(!cPeakList.GetRowValues(peakRows, caPeakListRowData)) continue;
		PeakListData2ResultData(caPeakListRowData, caResultRowData, row);
		cResultTable.AppendRow(caResultRowData);
	}

	if (pcData->SetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
	//CString szWatch = cResultTable.GetXMLString();
}


void CQuantitativeCalculation::DirectGenerateResultData() {

	CIADataTable cPeakList;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cPeakList) != IAERROR_SUCCESS) return;

	CIADataTable cResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
	ClearTableRowData(&cResultTable);

	int rowSize = cPeakList.GetRowsNumber(); //m_cDuplicate.GetItemSize();
	vector<CString> vectCompName(rowSize);
	

	typedef MatchType::iterator Iter;
	int row=0;
	for (Iter iter=m_Result.begin(); iter!=m_Result.end(); ++iter) { //�����Ƿ����m_Result�� 
		row++;
		int peakRows=iter->first;
		CString szCompName=(iter->second).szComponentName;
		if (peakRows<=rowSize)
		{
			vectCompName[peakRows] = szCompName;
		}
	}

	
	for (int row=0; row<rowSize; ++row)
	{
		CStringArray caPeakListRowData, caResultRowData;
		if(!cPeakList.GetRowValues(row, caPeakListRowData)) continue;
		PeakListData2ResultData(caPeakListRowData, caResultRowData, row+1);
		caResultRowData[3] = vectCompName[row]; // ��Ȼ���ѿ���������仯̫��
		cResultTable.AppendRow(caResultRowData);
	}

	if (pcData->SetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
	//CString szWatch = cResultTable.GetXMLString();
}



/************************************************************************************************************************************
/
/                                                  �������� ��غ���
/
/************************************************************************************************************************************/

CString CQuantitativeCalculation::GetCalibrateObjectAttr(){
	CString szAttr;
	if (m_Object==XMLVALUE_CONSTENT_c_peak_area)	 
		szAttr=_T("PeakArea");
	else	
		szAttr=_T("PeakHeight");
	return szAttr;
}


double CQuantitativeCalculation::GetColumnTotal(CIADataTable *pTable, const CString& strColumn) {

	CString szWatch=pTable->GetXMLString();
	double dResult=0.0f;
	CStringArray cItems;
	if (!pTable->GetColumnValues(strColumn, cItems))	return dResult;
	const int itemSize = cItems.GetSize();
	if (itemSize==0) return dResult;

	for (int i=0; i<itemSize; ++i) {
		CString szItem=cItems[i];
		double dItem=_tstof(szItem);
		dResult += dItem;
	}

	return dResult;
}


void CQuantitativeCalculation::QC_percentage(){
	
	//�ٷֱȷ� : ÿ����ķ����ռ���з��������İٷֱ�
	CString szAttr=GetCalibrateObjectAttr();
	CIADataTable cResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;

	double total=GetColumnTotal(&cResultTable, szAttr);
	const long rowSize=cResultTable.GetRowsNumber();
	for (long row=0; row<rowSize; row++) {
		CString szItem;
		if (!cResultTable.GetCellValue(row, szAttr, szItem)) return;
		double dItem=_tstof(szItem);
		double dContents = dItem/total*100.0f;
		CString szContents = ConvertScientificNotation(dContents);
		//szContents.Format(CONTENTS_DATA_FORMAT, dContents); // %0.4E
		if (!cResultTable.SetCellValue(row, _T("Concentration"), szContents)) return;
	}

	if (pcData->SetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
}


double CQuantitativeCalculation::GetProductFactor() {
	double dProductFactor; //�˻�����
	if (pcData->GetValue_Double(XMPPATH_SAMPLE_PARAM_CALI_PRODUCTFACTOR, dProductFactor) != IAERROR_SUCCESS) return 1.0f;
	return dProductFactor;
}


double CQuantitativeCalculation::GetDilutionFactor(){
	double dDilutionFactor; //ϡ������
	if (pcData->GetValue_Double(XMPPATH_SAMPLE_PARAM_CALI_DILUTIONFACTOR, dDilutionFactor) != IAERROR_SUCCESS) return 1.0f;
	return dDilutionFactor;
}


double CQuantitativeCalculation::GetSampleAmount(){
	double dSampleAmount; //����ֵ
	if (pcData->GetValue_Double(XMPPATH_SAMPLE_PARAM_CALI_SAMPLEAMOUNT, dSampleAmount) != IAERROR_SUCCESS) return 1.0f;
	return dSampleAmount;
}


void CQuantitativeCalculation::QC_external(){

	//�������X��У������Ϊy=ax+b��
	//���X�ľ��Ժ���=(�����(X)-b)/a*�˻�����*ϡ������
	CString szAttr=GetCalibrateObjectAttr();
	CIADataTable cResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;

	double dProduct = GetProductFactor();  //�˻�����
	double dDilution= GetDilutionFactor(); //ϡ������

	const long rowSize=cResultTable.GetRowsNumber();
	if (rowSize!=m_Result.size()) ASSERT(TRUE);

	CString szItem;
	typedef MatchType::iterator Iter;
	Iter iter = m_Result.begin();
	for (long row=0; row<rowSize; row++) {
		CaliItem caliItem = iter->second;
		iter++;
		double a=_tstof(caliItem.szSlope);
		double b=_tstof(caliItem.szIntercept);

		if (!cResultTable.GetCellValue(row, szAttr, szItem)) return;
		double dItem=_tstof(szItem);

		CString szContents; 
		double dContents=0.0f;

		//���У������a=0����ô�൱�����У����������Ч�ģ����������޷����ж������㡣
		//�����Ʒ���������ƥ���������֣���ô�ڽ��������ʾ����ֵĺ���Ϊ0
		if (a!=0.0f)
			dContents=(dItem-b)*dProduct*dDilution/a;
		szContents = ConvertScientificNotation(dContents);
		//szContents.Format(CONTENTS_DATA_FORMAT, dContents); // %0.4E
		if (!cResultTable.SetCellValue(row, _T("Concentration"), szContents)) return;
	}

	if (pcData->SetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
}


int CQuantitativeCalculation::FindMatchedCompoundGroupRow(const CString& cStrInteriorNo) {
	
	int findRow=-1;

	CIADataTable cCaliResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_PARAM_CALI_RESULT_TABLE, cCaliResultTable) != IAERROR_SUCCESS) return findRow;
	CStringArray ArrIsInterior, ArrInteriorNo;
	if(!cCaliResultTable.GetColumnValues(_T("IsInterior"), ArrIsInterior)) return findRow;
	if(!cCaliResultTable.GetColumnValues(_T("InteriorNo"), ArrInteriorNo)) return findRow;
	const int ArrIsInteriorSize=ArrIsInterior.GetSize();
	const int ArrInteriorNoSize=ArrInteriorNo.GetSize();
	if(ArrInteriorNoSize!=ArrIsInteriorSize) return findRow;
	
	for (int i=0; i<ArrInteriorNoSize; ++i) {
		if (cStrInteriorNo==ArrInteriorNo[i])
			if (_T("��")==ArrIsInterior[i]){
				findRow=i;
				break;
			}
	}
	return findRow;
}


void CQuantitativeCalculation::FindInteriorContentAndAttr(const CString& cStrInteriorNo, CaliItem& intItem) {

	int findRow = FindMatchedCompoundGroupRow(cStrInteriorNo);
	if (findRow==-1) return;

	CIADataTable cCaliResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_PARAM_CALI_RESULT_TABLE, cCaliResultTable) != IAERROR_SUCCESS) return ;
	CStringArray ArrIntItem;
	if(!cCaliResultTable.GetRowValues(findRow, ArrIntItem)) return;
	CaliItem caliItem(ArrIntItem, CALIBRATERESULTTABLE);
	intItem = caliItem;
}


void CQuantitativeCalculation::FindReMatchedPeakListItem(const CString& cStrInteriorNo, CaliItem& peakListItem){
	
	FindInteriorContentAndAttr(cStrInteriorNo, peakListItem);
	
	int findPeakListRow=-1;
	typedef MatchType::iterator Iter;
	for (Iter iter=m_Result.begin(); iter!=m_Result.end(); ++iter) {
		CaliItem caliItem = iter->second;
		if (peakListItem==caliItem){
			findPeakListRow=iter->first;
			break;
		}
	}

	if (findPeakListRow==-1) return;

	CIADataTable cPeakListTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_PEAKLIST, cPeakListTable) != IAERROR_SUCCESS) return;

	CStringArray ArrPeakListItem;
	if(!cPeakListTable.GetRowValues(findPeakListRow, ArrPeakListItem)) return;
	CaliItem caliItem(ArrPeakListItem, PEAKLISTTABLE);
	peakListItem = caliItem;
}


void CQuantitativeCalculation::QC_internal(){
/*
	���X�ľ��Ժ���=((�����(X))/(�����(�ڱ�))-b)/a���ڱ���Ժ������˻����ӡ�ϡ������
	ע��
		1�����X���õ��ڱ�ΪУ��������X�ڱ���һ�µ��ڱ���ڱ���Ժ����ͷ����(�ڱ�)Ϊ��ǰ��Ʒ�е��ڱ꺬���ͷ������
		2���ڱ���Ժ���Ϊ�û�����Ʒ��Ϣģ���е��ڱ���Ϣ���������ֵ��
		3�������(�ڱ�)Ϊ��У����������ڱ���ƥ�����ֵķ������
*/

	CString szAttr=GetCalibrateObjectAttr();
	CIADataTable cResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;

	double dProduct=GetProductFactor();  //�˻�����
	double dDilution=GetDilutionFactor(); //ϡ������

	const long rowSize=cResultTable.GetRowsNumber();
	if (rowSize!=m_Result.size()) ASSERT(TRUE);
	
	CString szItem;
	typedef MatchType::iterator Iter;
	Iter iter = m_Result.begin();
	for (long row=0; row<rowSize; row++) {
		if (!cResultTable.GetCellValue(row, szAttr, szItem)) return;
		CaliItem caliItem = iter->second;
		iter++;

		//ȡ��ƥ�����ݵ��ڱ��ţ�Ȼ���*pcData��CalibrateTable���ҳ�ƥ����ڱ��ţ�����1�ģ�����ȡ���ڱ���Ժ������Ͷ�Ӧ�ġ����������ߡ�
		CaliItem InterItem;
		CString cStrInteriorNo = caliItem.szInteriorNo;
		FindInteriorContentAndAttr(cStrInteriorNo, InterItem);

		if (caliItem.szIsInterior==_T("��"))
			if (!cResultTable.SetCellValue(row, _T("Concentration"), InterItem.szStdContent)) //ֱ��ƥ���ڱ�ľ��Ժ������û���������
				return;


		if (caliItem.szIsInterior!=_T("��")) continue; //���ڱ�Ž��ж������㣬�ڱ���Ժ����Ѿ����û�������  


	    //Ҫƥ���ƥ�䵽��У�����ݣ����ڱ�����ƥ�䵽���ڱ���ݣ����ڱ����ƥ���PeakList�ڵļ�¼
		CaliItem PeakListItem;
		FindReMatchedPeakListItem(cStrInteriorNo, PeakListItem);
		CString szValue_in;
		if (szAttr==_T("PeakArea"))
			szValue_in=PeakListItem.szPeakArea;
		else
			szValue_in=PeakListItem.szPeakHeight;

		double dValue=_tstof(szItem);
		double dValue_in=_tstof(szValue_in);
		double a=_tstof(caliItem.szSlope);
		double b=_tstof(caliItem.szIntercept);
		double dStdContent_in=_tstof(InterItem.szStdContent); //�ڱ���Ժ���

		if (dValue_in==0.0f)
		{
			//MessageBox(_T(this, "�ڱ���������Ϊ0�������޷��������У���Ҫ�������"), MB_OK|MB_ICONERROR);
			ASSERT(TRUE);
		}
		
		CString szContents;
		double dContents=0.0f;
		if (a!=0.0f && dValue_in!=0.0f)
			dContents=((dValue/dValue_in)-b)*dStdContent_in*dProduct*dDilution/a;

		szContents = ConvertScientificNotation(dContents);
		//szContents.Format(CONTENTS_DATA_FORMAT, dContents); //0.4E
		if (!cResultTable.SetCellValue(row, _T("Concentration"), szContents)) return;
	}

	if (pcData->SetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;

}


void CQuantitativeCalculation::QC_external_percentage(){
/*
		���X����Ժ���=���X�ľ��Ժ���/��Ʒ��
		ע��
		1�����X�ľ��Ժ�������귨����ó���
		2����Ʒ�����ԡ�У�����á��еĺ���ֵ��
*/
	double dSampleAmount=GetSampleAmount();
	if (dSampleAmount==0.0) {
		//MessageBox(_T("���ԡ�У�����á��еĺ���ֵΪ0�������޷���������"));
		return;
	}
	
	QC_external();

	CIADataTable cResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;

	const long rowSize=cResultTable.GetRowsNumber();
	for (long row=0; row<rowSize; row++) {
		CString szValue;
		if (!cResultTable.GetCellValue(row, _T("Concentration"), szValue)) return;
		double dValue=_tstof(szValue);
		double dContents = dValue/dSampleAmount*100.0f; //���X����Ժ���=���X�ľ��Ժ���/��Ʒ��
		CString szContents = ConvertScientificNotation(dContents);
		//szContents.Format(CONTENTS_DATA_FORMAT, dContents); //0.4E
		if (!cResultTable.SetCellValue(row, _T("Concentration"), szContents)) return;
	}

	if (pcData->SetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
}


void CQuantitativeCalculation::QC_internal_percentage(){
	/*
		���X����Ժ���=���X�ľ��Ժ���/��Ʒ��
		ע��
		1�����X�ľ��Ժ�������귨����ó���
		2����Ʒ�����ԡ�У�����á��еĺ���ֵ��
*/
	double dSampleAmount=GetSampleAmount();

	if (dSampleAmount==0.0) {
		//MessageBox(_T("���ԡ�У�����á��еĺ���ֵΪ0�������޷���������"));
		return;
	}

	QC_internal();

	CIADataTable cResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;

	const long rowSize=cResultTable.GetRowsNumber();
	for (long row=0; row<rowSize; row++) {
		CString szValue;
		if (!cResultTable.GetCellValue(row, _T("Concentration"), szValue)) return;
		double dValue=_tstof(szValue);
		double dContents = dValue/dSampleAmount*100.0f; //���X����Ժ���=���X�ľ��Ժ���/��Ʒ��
		CString szContents = ConvertScientificNotation(dContents);
		//szContents.Format(CONTENTS_DATA_FORMAT, dContents); //0.4E
		if (!cResultTable.SetCellValue(row, _T("Concentration"), szContents)) return;
	}

	if (pcData->SetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
}


void CQuantitativeCalculation::QC_normalization(){
/*
		���X�İٷֺ���=���X�ľ��Ժ���/(�Ƹ���ֵľ��Ժ���)
		ע��
		1�����X�ľ��Ժ�������귨����ó���
		2��������ֵľ��Ժ�����Ҫ����귨����ó�
*/
	QC_external();

	CIADataTable cResultTable;
	if (pcData->GetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;

	CString strColumn(_T("Concentration"));
	double dContentTotal = GetColumnTotal(&cResultTable, strColumn); //�Ƹ���ֵľ��Ժ���

	const long rowSize=cResultTable.GetRowsNumber();
	for (long row=0; row<rowSize; row++) {
		CString szValue;
		if (!cResultTable.GetCellValue(row, strColumn, szValue)) return;
		double dValue=_tstof(szValue);
		double dContents = dValue/dContentTotal*100.0f;  //���X�İٷֺ���=���X�ľ��Ժ���/(�Ƹ���ֵľ��Ժ���)
		CString szContents = ConvertScientificNotation(dContents);
		//szContents.Format(CONTENTS_DATA_FORMAT, dContents); // %0.4E
		if (!cResultTable.SetCellValue(row, strColumn, szContents)) return;
	}

	if (pcData->SetValue_Table(XMLPATH_SAMPLE_RESULT_RESULT, cResultTable) != IAERROR_SUCCESS) return;
}

CString CQuantitativeCalculation::ConvertScientificNotation(double contents) {
/*
	1. ������ֵ��0.1ʱ���Ų��ÿ�ѧ��������
	2. e�������ֵ�м�λ��ʾ��λ�����磬���Ϊe-2������e-002��
	3. ��ֱֵ�ӱ��Ϊ0.000��
*/
	CString retVal(_T(""));
	
	if (contents == 0.0000f)
	{
		retVal = _T("0.000");
	}
	else if (contents < 0.1f)
	{
		retVal.Format(CONTENTS_DATA_FORMAT, contents); // %0.4E

		int strLen = retVal.GetLength();
		int pos = retVal.Find(_T("E-00"));
		if (pos!=-1)
		{
			retVal.Delete(pos+2, 2);
			return retVal;
		}
		pos = retVal.Find(_T("E-0"));
		if (pos!=-1)
		{
			retVal.Delete(pos+2);
			return retVal;
		}

		return retVal;
	}
	else //����0.1 �����ÿ�ѧ����
	{
		retVal.Format(_T("%.3f"), contents); // %0.4E
		return retVal;
	}
	return retVal;
}

int CQuantitativeCalculation::QuantitativeCalculation() {
	
	if (m_Method==XMLVALUE_CONST_c_percentage)
		QC_percentage(); //�������ٷֱȷ�
	else if (m_Method==XMLVALUE_CONST_c_external)
		QC_external();   //��������귨
	else if (m_Method==XMLVALUE_CONST_c_internal)
		QC_internal(); 	 //�������ڱ귨
	else if (m_Method==XMLVALUE_CONST_c_external_percentage)
		QC_external_percentage(); //���������ٷֱȷ�
	else if (m_Method==XMLVALUE_CONST_c_internal_percentage)
		QC_internal_percentage(); //�������ڱ�ٷֱȷ�
	else if (m_Method==XMLVALUE_CONST_c_normalization)
		QC_normalization();       //����������һ����
	else 
		return IAERROR_XML_NOTVALID;

	return IAERROR_SUCCESS;
}












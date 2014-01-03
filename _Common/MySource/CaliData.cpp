#include "StdAfx.h"
#include "CaliData.h"
//#include "ChildViewCaliMain.h"
#include <set>


CaliItem::CaliItem(const CStringArray& data)
{
	if (data.GetSize()<18) ASSERT(TRUE);
	
	szNO			= data[1];	//���
	szReserveTime	= data[2];	//����ʱ��
	szComponentName = data[3];	//�����
	szLevel			= data[4];	//����
	szContents		= data[5];  //����
	szPeakArea		= data[6];	//�����
	szPeakHeight	= data[7];	//���
	szResFactor		= data[8];	//��Ӧ����
	szIsReference	= data[9];	//�Ƿ�α�
	szIsInterior	= data[10];	//�Ƿ��ڱ�
	szStdContent	= data[11];; //�ڱ���Ժ���:����Ʒ��Ϣ�еı��������
	szInteriorNo	= data[12];	//�ڱ���
	szSampleFullName= data[13]; 
	szPeakBeginX    = data[14];
	szPeakEndX      = data[15];
	szSlope         = data[16];
	szIntercept     = data[17];
	szCoefficient   = data[18];
}


CaliItem::CaliItem(const CStringArray& data, const CString tableName){
	if (tableName==CALIBRATETABLE) {
		szNO			= data[1];	//���
		szReserveTime	= data[2];	//����ʱ��
		szComponentName = data[3];	//�����
		szLevel			= data[4];	//����
		szContents		= data[5];  //����
		szPeakArea		= data[6];	//�����
		szPeakHeight	= data[7];	//���
		szResFactor		= data[8];	//��Ӧ����
		szIsReference	= data[9];	//�Ƿ�α�
		szIsInterior	= data[10];	//�Ƿ��ڱ�
		szStdContent	= data[11]; //�ڱ���Ժ���:����Ʒ��Ϣ�еı��������
		szInteriorNo	= data[12];	//�ڱ���
		szSampleFullName= data[13]; 
		szPeakBeginX    = data[14];
		szPeakEndX      = data[15];
		szSlope         = data[16];
		szIntercept     = data[17];
		szCoefficient   = data[18];
	}
	else if (tableName==CALIBRATERESULTTABLE)
	{
		szNO			= data[1];	//���
		szReserveTime	= data[2];	//����ʱ��
		szComponentName = data[3];	//�����
		szSlope         = data[4];
		szIntercept     = data[5];
		szCoefficient   = data[6];
		szIsReference	= data[7];	//�Ƿ�α�
		szIsInterior	= data[8];	//�Ƿ��ڱ�
		szInteriorNo	= data[9];	//�ڱ���
		szStdContent	= data[10]; //�ڱ���Ժ���:����Ʒ��Ϣ�еı��������
	}
	else if (tableName==PEAKLISTTABLE){
		szNO			= data[1];	//���
		szReserveTime	= data[2];	//����ʱ��
		szComponentName = _T("");	//data[3];	//�����
		szLevel			= _T("");	//data[4];	//����
		szContents		= _T("");	//data[5];  //����
		szPeakArea		= data[4];	//�����
		szPeakHeight	= data[6];	//���
		szResFactor		= _T("");	//data[8];	//��Ӧ����
		szIsReference	= _T("");	//data[9];	//�Ƿ�α�
		szIsInterior	= _T("");	//data[10];	//�Ƿ��ڱ�
		szStdContent	= _T("");	//data[11];; //�ڱ���Ժ���:����Ʒ��Ϣ�еı��������
		szInteriorNo	= _T("");	//data[12];	//�ڱ���
		szSampleFullName= _T("");	//data[13]; 
		szPeakBeginX    = _T("");	//data[14];
		szPeakEndX      = _T("");	//data[15];
		szSlope         = _T("");	//data[16];
		szIntercept     = _T("");	//data[17];
		szCoefficient   = _T("");	//data[18];
	}
	

}


bool CaliItem::operator== (const CaliItem& rhs){
	return 	((szNO				== rhs.szNO)	//���
			&&(szReserveTime	== rhs.szReserveTime)	//����ʱ��
			&&(szComponentName  == rhs.szComponentName)	//�����
			&&(szLevel			== rhs.szLevel)	//����
			&&(szContents		== rhs.szContents)  //����
			&&(szPeakArea		== rhs.szPeakArea)	//�����
			&&(szPeakHeight		== rhs.szPeakHeight)	//���
			&&(szResFactor		== rhs.szResFactor)	//��Ӧ����
			&&(szIsReference	== rhs.szIsReference)	//�Ƿ�α�
			&&(szIsInterior		== rhs.szIsInterior)	//�Ƿ��ڱ�
			&&(szStdContent		== rhs.szStdContent) //�ڱ���Ժ���:����Ʒ��Ϣ�еı��������
			&&(szInteriorNo		== rhs.szInteriorNo)	//�ڱ���
			&&(szSampleFullName == rhs.szSampleFullName)
			&&(szPeakBeginX     == rhs.szPeakBeginX)
			&&(szPeakEndX       == rhs.szPeakEndX)
			&&(szSlope          == rhs.szSlope)
			&&(szIntercept      == rhs.szIntercept)
			&&(szCoefficient    == rhs.szCoefficient));
}

CCaliData::CCaliData(void)
{
}

CCaliData::~CCaliData(void)
{
}

void CCaliData::InitCaliData(CIADataTable* pCIADataTable) {
	CString szWatch = pCIADataTable->GetXMLString();

	//���һ�ű�����,�Զ������µ��ڲ�����
	if (!m_CaliItems.empty()) m_CaliItems.clear();

	DWORD dwOrders=0;
	pCIADataTable->GetTable(&m_tableName, &dwOrders);

	const long TableRows = pCIADataTable->GetRowsNumber();

	for (long i=0; i<TableRows; ++i)
	{
		CStringArray arrItem;
		pCIADataTable->GetRowValues(i, arrItem);
		CaliItem caliItem(arrItem, m_tableName);
		m_CaliItems.push_back(caliItem);
	}

	VerifyContent();
}




bool CCaliData::ItemSaveToTable(CIADataTable* pCIADataTable)
{
	CString szWatch = pCIADataTable->GetXMLString();

	pCIADataTable->DeleteAllRows();
	const int TableCols=pCIADataTable->GetColumnNumber();
	if (TableCols!=20)
	{
		::AfxMessageBox(_T("��ǰҪ������ļ��ڲ���������"));
		return false;
	}
	const int ItemNum = m_CaliItems.size();
	for (int i=0; i<ItemNum; ++i)
	{
		CStringArray arrItem;
		for (int c=0; c<TableCols; ++c)
		{
			CString str(_T(""));
			arrItem.Add(str);
		}
		arrItem[0]=_T("{[(BITMAP_PREDEF:_Action_Info.bmp)]}");
		arrItem[1]=m_CaliItems[i].szNO;
		arrItem[2]=m_CaliItems[i].szReserveTime;
		arrItem[3]=m_CaliItems[i].szComponentName;
		arrItem[4]=m_CaliItems[i].szLevel;
		arrItem[5]=m_CaliItems[i].szContents;
		arrItem[6]=m_CaliItems[i].szPeakArea;
		arrItem[7]=m_CaliItems[i].szPeakHeight;
		arrItem[8]=m_CaliItems[i].szResFactor;
		arrItem[9]=m_CaliItems[i].szIsReference;
		arrItem[10]=m_CaliItems[i].szIsInterior;
		arrItem[11]=m_CaliItems[i].szStdContent;
		arrItem[12]=m_CaliItems[i].szInteriorNo;
		arrItem[13]=m_CaliItems[i].szSampleFullName;
		arrItem[14]=m_CaliItems[i].szPeakBeginX;
		arrItem[15]=m_CaliItems[i].szPeakEndX;
		arrItem[16]=m_CaliItems[i].szSlope;
		arrItem[17]=m_CaliItems[i].szIntercept;
		arrItem[18]=m_CaliItems[i].szCoefficient;
		arrItem[19]=_T("{[(BITMAP_PREDEF:_Action_Delete.bmp)]}");

		pCIADataTable->AppendRow(arrItem);
	}

	szWatch = pCIADataTable->GetXMLString();
	return true;
}

bool CCaliData::ItemSaveToCalibrateResultTable(CIADataTable* pCIADataTable){
	CString szWatch = pCIADataTable->GetXMLString();

	pCIADataTable->DeleteAllRows();
	const int TableCols=pCIADataTable->GetColumnNumber();
	if (TableCols!=12)
	{
		::AfxMessageBox(_T("��ǰҪ������ļ��ڲ���������"));
		return false;
	}
	const int ItemNum = m_CaliItems.size();
	for (int i=0; i<ItemNum; ++i)
	{
		CStringArray arrItem;
		for (int c=0; c<TableCols; ++c)
		{
			CString str(_T(""));
			arrItem.Add(str);
		}
		arrItem[0]=_T("{[(BITMAP_PREDEF:_Action_Info.bmp)]}");
		arrItem[1]=m_CaliItems[i].szNO; 
		arrItem[2]=m_CaliItems[i].szReserveTime;
		arrItem[3]=m_CaliItems[i].szComponentName;
		arrItem[4]=m_CaliItems[i].szSlope;
		arrItem[5]=m_CaliItems[i].szIntercept;
		arrItem[6]=m_CaliItems[i].szCoefficient;
		arrItem[7] =m_CaliItems[i].szIsReference;
		arrItem[8]=m_CaliItems[i].szIsInterior;
		arrItem[9]=m_CaliItems[i].szInteriorNo;
		arrItem[10]=m_CaliItems[i].szStdContent;
		arrItem[11]=_T("{[(BITMAP_PREDEF:_Action_Delete.bmp)]}");

		pCIADataTable->AppendRow(arrItem);
	}

	szWatch = pCIADataTable->GetXMLString();
	return true;
}

bool CCaliData::GenerateTable(CIADataTable &cIADataTable, const CString& caliMethod)
{	
	//ȷ�����Ʊ������
	m_tableType = caliMethod;

	//�����Է����ʾ���͵ı��
	CStringArray ArrTopics, ArrTitles, ArrColWidth, ArrAlign;
	ArrTopics.Add(_T("icon_inf"));      ArrTitles.Add(_T("i"));			ArrColWidth.Add(_T("20"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("NO"));            ArrTitles.Add(_T("���"));		ArrColWidth.Add(_T("40"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("ReserveTime"));	ArrTitles.Add(_T("����ʱ��"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("right|vcenter"));
	ArrTopics.Add(_T("ComponentName"));	ArrTitles.Add(_T("�����"));		ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("Level"));			ArrTitles.Add(_T("��Ʒ����"));	ArrColWidth.Add(_T("60"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("Contents"));		ArrTitles.Add(_T("����"));		ArrColWidth.Add(_T("100"));	ArrAlign.Add(_T("right|vcenter"));
	
	if (m_tableType == _T("�����"))
	{
		ArrTopics.Add(_T("PeakArea"));		ArrTitles.Add(_T("�����"));		ArrColWidth.Add(_T("60"));	ArrAlign.Add(_T("right|vcenter"));
	}
	else if (m_tableType == _T("���"))
	{
		ArrTopics.Add(_T("PeakHeight"));	ArrTitles.Add(_T("���"));		ArrColWidth.Add(_T("60"));	ArrAlign.Add(_T("right|vcenter"));
	}
	else
	{
		::AfxMessageBox(_T("�����Ƿ�������Ƿ�ߣ�"));	
		return false;
	}
	
	ArrTopics.Add(_T("ResFactor"));		ArrTitles.Add(_T("��Ӧ����"));	ArrColWidth.Add(_T("100"));	ArrAlign.Add(_T("right|vcenter"));
	ArrTopics.Add(_T("IsReference"));	ArrTitles.Add(_T("�Ƿ�α�"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("IsInterior"));	ArrTitles.Add(_T("�Ƿ��ڱ�"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("InteriorNo"));	ArrTitles.Add(_T("�ڱ���"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("icon_del"));      ArrTitles.Add(_T("ɾ"));			ArrColWidth.Add(_T("20"));	ArrAlign.Add(_T("center|vcenter"));

	//��������Ϣ
	if(!cIADataTable.SetColumns(ArrTopics, &ArrTitles)) 
	{
		::AfxMessageBox(_T("��������Ϣʧ��"));
		return FALSE;
	}

	//�����п���Ϣ
	for (int i=0; i<ArrTitles.GetSize(); ++i)
	{
		if(!cIADataTable.SetColumnInfo(ArrTopics.GetAt(i), XMLPROP_TAB_COL_WIDTH, ArrColWidth.GetAt(i))) 
		{
			::AfxMessageBox(_T("�����п�ʧ��"));
			return FALSE;
		}
	}

	//���Ӷ�����Ϣ
	for (int i=0; i<ArrAlign.GetSize(); ++i)
	{
		if(!cIADataTable.SetColumnInfo(ArrTopics.GetAt(i), XMLPROP_TAB_COL_ALIGN, ArrAlign.GetAt(i))) 
		{
			::AfxMessageBox(_T("�������뷽ʽʧ��"));
			return FALSE;
		}
	}

	// *********�����������
	ClearUnnecessaryCell();

	//�����������
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		CStringArray arrItem;
		if (!GetRowData(i, arrItem))
		{
			::AfxMessageBox(_T("δȡ����Ӧ������"));
			return false;
		}
		cIADataTable.AppendRow(arrItem);
	}

	//CString szWatch =  cIADataTable.GetXMLString();
	return true;
}

bool CCaliData::GenerateCalibrateResultTable(CIADataTable &cIADataTable) {
	
	CStringArray ArrTopics, ArrTitles, ArrColWidth, ArrAlign;
	ArrTopics.Add(_T("icon_inf"));      ArrTitles.Add(_T("i"));			ArrColWidth.Add(_T("20"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("NO"));            ArrTitles.Add(_T("���"));		ArrColWidth.Add(_T("40"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("ReserveTime"));	ArrTitles.Add(_T("����ʱ��"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("right|vcenter"));
	ArrTopics.Add(_T("ComponentName"));	ArrTitles.Add(_T("�����"));		ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("Slope"));			ArrTitles.Add(_T("б��"));		ArrColWidth.Add(_T("100"));	ArrAlign.Add(_T("right|vcenter"));
	ArrTopics.Add(_T("Intercept"));		ArrTitles.Add(_T("�ؾ�"));		ArrColWidth.Add(_T("100"));	ArrAlign.Add(_T("right|vcenter"));
	ArrTopics.Add(_T("Coefficient"));	ArrTitles.Add(_T("���ϵ��"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("right|vcenter"));
	ArrTopics.Add(_T("IsReference"));	ArrTitles.Add(_T("�Ƿ�α�"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("IsInterior"));	ArrTitles.Add(_T("�Ƿ��ڱ�"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("InteriorNo"));	ArrTitles.Add(_T("�ڱ���"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));
	ArrTopics.Add(_T("StdContent"));	ArrTitles.Add(_T("�ڱ꺬��"));	ArrColWidth.Add(_T("80"));	ArrAlign.Add(_T("center|vcenter"));

	//��������Ϣ
	if(!cIADataTable.SetColumns(ArrTopics, &ArrTitles)) 
	{
		::AfxMessageBox(_T("��������Ϣʧ��"));
		return FALSE;
	}

	//�����п���Ϣ
	for (int i=0; i<ArrTitles.GetSize(); ++i)
	{
		if(!cIADataTable.SetColumnInfo(ArrTopics.GetAt(i), XMLPROP_TAB_COL_WIDTH, ArrColWidth.GetAt(i))) 
		{
			::AfxMessageBox(_T("�����п�ʧ��"));
			return FALSE;
		}
	}

	//���Ӷ�����Ϣ
	for (int i=0; i<ArrAlign.GetSize(); ++i)
	{
		if(!cIADataTable.SetColumnInfo(ArrTopics.GetAt(i), XMLPROP_TAB_COL_ALIGN, ArrAlign.GetAt(i))) 
		{
			::AfxMessageBox(_T("�������뷽ʽʧ��"));
			return FALSE;
		}
	}

	//�����������
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		CStringArray arrItem;
		if (!GetCaliResultRowData(i, arrItem))
		{
			::AfxMessageBox(_T("δȡ����Ӧ������"));
			return false;
		}
		cIADataTable.AppendRow(arrItem);
	}

	//CString szWatch =  cIADataTable.GetXMLString();
	return true;
}

bool CCaliData::GetRowData(int nRow, CStringArray& strArry)//��õ�i������
{
	if (nRow < 0)
	{
		TRACE(_T("rowΪ����"));
		return false;
	}

	const int itemNum = m_CaliItems.size();
	if (nRow > itemNum-1)
	{
		TRACE(_T("row�������ݸ���"));
		return false;
	}

	strArry.Add(_T("{[(BITMAP_PREDEF:_Action_Info.bmp)]}"));
	strArry.Add(m_CaliItems[nRow].szNO);			//���
	strArry.Add(m_CaliItems[nRow].szReserveTime);	//����ʱ��
	strArry.Add(m_CaliItems[nRow].szComponentName);	//�����
	strArry.Add(m_CaliItems[nRow].szLevel);			//����
	
	double dContents = _tstof(m_CaliItems[nRow].szContents);
	if (dContents == 0.000f)
	{
		m_CaliItems[nRow].szContents = _T("0.000");
	}
	else if (dContents < 0.1f)
	{
		m_CaliItems[nRow].szContents.Format(_T("%.3E"), dContents);
		EraseZeroMinus(m_CaliItems[nRow].szContents);
	}
	else
	{
		ConvertDecimalDigits(m_CaliItems[nRow].szContents, 3);
	}
	


	strArry.Add(m_CaliItems[nRow].szContents);		//����
	//strArry.Add(_T("")); //�����û���������

	if (m_tableType == _T("�����"))
	{
		ConvertDecimalDigits(m_CaliItems[nRow].szPeakArea, 3);
		strArry.Add(m_CaliItems[nRow].szPeakArea);		//�����
	}
	else if (m_tableType == _T("���"))
	{
		ConvertDecimalDigits(m_CaliItems[nRow].szPeakHeight, 3);
		strArry.Add(m_CaliItems[nRow].szPeakHeight);	//���
	}
	else
	{
		::AfxMessageBox(_T("�����Ƿ�������Ƿ�ߣ�����"));
		return false;
	}

	
	double dResFactor = _tstof(m_CaliItems[nRow].szResFactor);
	if (dResFactor==0.0000f)
	{
		m_CaliItems[nRow].szResFactor = _T("0.0000");
	}
	else
	{
		m_CaliItems[nRow].szResFactor.Format(_T("%.4E"), dResFactor);
		if (dResFactor >= 1.000000f)
			EraseZeroPlus(m_CaliItems[nRow].szResFactor);
		else
			EraseZeroMinus(m_CaliItems[nRow].szResFactor);
	}
	strArry.Add(m_CaliItems[nRow].szResFactor);		//��Ӧ����


	strArry.Add(m_CaliItems[nRow].szIsReference);	//�Ƿ�α�
	strArry.Add(m_CaliItems[nRow].szIsInterior);	//�Ƿ��ڱ�
	strArry.Add(m_CaliItems[nRow].szInteriorNo);	//�ڱ���
	strArry.Add(_T("{[(BITMAP_PREDEF:_Action_Delete.bmp)]}"));	
	return true;
}

bool CCaliData::GetInteriorNoDate(CStringArray& strArry) {
	 
	const int itemNum = m_CaliItems.size();
	if (itemNum <= 0)
	{
		TRACE(_T("������"));
		return false;
	}

	for (int row=0; row<itemNum; ++row)
	{
		strArry.Add(m_CaliItems[row].szInteriorNo);
	}
	return true;
}


bool CCaliData::GetCaliResultRowData(int nRow, CStringArray& strArry) {
	if (nRow < 0) return false;

	const int itemNum = m_CaliItems.size();
	if (nRow > itemNum-1) return false;

	strArry.Add(_T("{[(BITMAP_PREDEF:_Action_Info.bmp)]}"));
	strArry.Add(m_CaliItems[nRow].szNO);			//���
	strArry.Add(m_CaliItems[nRow].szReserveTime);	//����ʱ��
	strArry.Add(m_CaliItems[nRow].szComponentName);	//�����
	strArry.Add(m_CaliItems[nRow].szSlope);			//б��
	strArry.Add(m_CaliItems[nRow].szIntercept);		//�ؾ�
	strArry.Add(m_CaliItems[nRow].szCoefficient);	//���ϵ��
	strArry.Add(m_CaliItems[nRow].szIsReference);	//�Ƿ�α�
	strArry.Add(m_CaliItems[nRow].szIsInterior);	//�Ƿ��ڱ�
	strArry.Add(m_CaliItems[nRow].szInteriorNo);	//�ڱ���
	strArry.Add(m_CaliItems[nRow].szStdContent);	//�ڱ꺬��
	strArry.Add(_T("{[(BITMAP_PREDEF:_Action_Delete.bmp)]}"));	
	return true;
}

CaliItem& CCaliData::GetRowDate(int row){
	//�Ƿ����ͣ��ٶ������rowһ����������
	return m_CaliItems[row];
}

//**********************************************************************************
//���룬һ������֤���½���Ʒ�ı���ʱ��
//�����instA, instB�ֱ��ʾͬһ��������к�
//**********************************************************************************
bool CCaliData::VerifyReserveTime(const CString& szReserveTime, int& match_no, int& instA, int& instB)
{
	//bool IsDefiniteIns = false; //�����Ƿ���Բ���
	double dInResTime = _tstof(szReserveTime);
	const int itemRow = m_CaliItems.size();
	instB = itemRow-1;
	for (int i=0; i<itemRow; ++i)
	{
		if (m_CaliItems[i].szNO!=_T(""))
		{
			double dResTime = _tstof(m_CaliItems[i].szReserveTime);
			double upper_RT = dResTime * (1.00000000 - 5.0/100); //0.95;
			double lower_RT = dResTime * (1.00000000 + 5.0/100); //1.05;
			if (upper_RT  <= dInResTime && dInResTime <= lower_RT)
			{//����ƥ�䣬��ͬ������Ľ���λ��
				instA = i; //ȷ��ͬ������Ŀ�ʼλ��

				match_no = _wtoi(m_CaliItems[i].szNO);

				for (int j=i+1; j<itemRow; ++j)
				{
					if (m_CaliItems[j].szNO!=_T(""))
					{
						instB = j - 1;
						break;
					}
				}
				if (instA!=instB) instA=instB;
				return true;
			}
		}
		
	}
	return false;
}

void CCaliData::ConvertDecimalDigits(CString& obj, int dts)
{
	double num = _tstof(obj);
	CString szFormat;
	szFormat.Format(_T("%%.%df"), dts);
	CString szConvert;
	szConvert.Format(szFormat, num);
	obj = szConvert;
}

void CCaliData::EraseZeroPlus(CString& str)  
{  
	int strLen = str.GetLength();
	int pos= str.Find(_T("E+")); 
	if (pos+5==strLen) //X.XXXe+000
		str.Delete(pos+2);
} 

void CCaliData::EraseZeroMinus(CString& str)  
{  
	int strLen = str.GetLength();
	int pos = str.Find(_T("E-00"));
	if (pos!=-1)
	{
		str.Delete(pos+2, 2);
		return;
	}
	pos = str.Find(_T("E-0"));
	if (pos!=-1)
	{
		str.Delete(pos+2);
		return;
	}
	//int pos= str.Find(_T("E-")); 
	//if (pos+5==strLen){ // E-000
	//	int next_pos = str.Find(_T("E-0"));
	//	if (next_pos!=-1)
	//		str.Delete(next_pos); 
		
} 

bool CCaliData::IsDigital(CString& str)
{
	const int strLen = str.GetLength();
	for (int i=0; i<strLen; ++i)
	{
		TCHAR tch = str.GetAt(i);
		if ((tch>='0' && tch<='9') || (tch == '.'))
			continue;
		else
			return false;
	}
	return true;
}

bool CCaliData::IsNaturalNumber(CString& str)
{
	const int strLen = str.GetLength();
	for (int i=0; i<strLen; ++i) {
		TCHAR tch = str.GetAt(i);
		if ((tch>='1' && tch<='9'))	continue;
		else	return false;
	}
	return true;
}

bool CCaliData::UpdateRowData(int iRow, const CStringArray& strArry)
{
	const int itemNum = m_CaliItems.size();
	if (iRow<0 || iRow>itemNum-1) {
		return false;
	}

	m_CaliItems[iRow].szPeakArea = strArry[4]; //�����
	ConvertDecimalDigits(m_CaliItems[iRow].szPeakArea, 3);

	m_CaliItems[iRow].szPeakHeight = strArry[6]; //���
	ConvertDecimalDigits(m_CaliItems[iRow].szPeakHeight, 3);

	m_CaliItems[iRow].szPeakBeginX     = strArry[10];
	m_CaliItems[iRow].szPeakEndX       = strArry[11];

	double dCont = _tstof(m_CaliItems[iRow].szContents);
	double dPeakArea = _tstof(m_CaliItems[iRow].szPeakArea);
	double dPeakHeight = _tstof(m_CaliItems[iRow].szPeakHeight);
	CString szFactor;

	if (m_tableType == _T("�����"))
	{
		double dResFactor = 0.0f / dPeakArea; // ������Ӧ����
		szFactor.Format(_T("%.4e"), dResFactor);
		if (dResFactor >= 1.000000f)
			EraseZeroPlus(szFactor);
		else
			EraseZeroMinus(szFactor);
		m_CaliItems[iRow].szResFactor		= szFactor;	
	}
	else if (m_tableType == _T("���"))
	{
		double dResFactor = 0.0f / dPeakHeight;// ������Ӧ����
		szFactor.Format(_T("%.4e"), dResFactor);
		if (dResFactor >= 1.000000f)
			EraseZeroPlus(szFactor);
		else
			EraseZeroMinus(szFactor);
		m_CaliItems[iRow].szResFactor		= szFactor;	
	}
	else
	{
		::AfxMessageBox(_T("�����Ƿ�������Ƿ�ߣ�"));
		return false;
	}

	return true;

}

bool CCaliData::InsertRowData(int iRow, int iStart, const CStringArray& strArry, LPCTSTR lpcszPathName)
{
	CaliItem insertCaliItem(m_CaliItems[iRow]);
	insertCaliItem.szNO				= _T("");	//���
	insertCaliItem.szReserveTime	= _T("");	//����ʱ��
	insertCaliItem.szComponentName  = _T("");	//����� 
	
	//�����µ���Ʒ����
	insertCaliItem.szLevel			= _T("#LEVEL#");	//����
	insertCaliItem.szContents		= _T("");//strArry[10];  //�������û���������
	ConvertDecimalDigits(insertCaliItem.szContents, 3);

	insertCaliItem.szPeakArea		= strArry[4];	//�����
	ConvertDecimalDigits(insertCaliItem.szPeakArea, 3);

	insertCaliItem.szPeakHeight		= strArry[6];	//���
	ConvertDecimalDigits(insertCaliItem.szPeakHeight, 3);

	double dCont = _tstof(insertCaliItem.szContents);
	double dPeakArea = _tstof(insertCaliItem.szPeakArea);
	double dPeakHeight = _tstof(insertCaliItem.szPeakHeight);
	CString szFactor;

	if (m_tableType == _T("�����"))
	{
		double dResFactor = 0.0f / dPeakArea; // ������Ӧ����
		szFactor.Format(_T("%.4e"), dResFactor);
		if (dResFactor >= 1.000000f)
			EraseZeroPlus(szFactor);
		else
			EraseZeroMinus(szFactor);
		insertCaliItem.szResFactor		= szFactor;	
	}
	else if (m_tableType == _T("���"))
	{
		double dResFactor = 0.0f / dPeakHeight;// ������Ӧ����
		szFactor.Format(_T("%.4e"), dResFactor);
		if (dResFactor >= 1.000000f)
			EraseZeroPlus(szFactor);
		else
			EraseZeroMinus(szFactor);
		insertCaliItem.szResFactor		= szFactor;	
	}
	else
	{
		::AfxMessageBox(_T("�����Ƿ�������Ƿ�ߣ�2"));
		return false;
	}

	insertCaliItem.szIsReference	= _T("��");	//�Ƿ�α�
	insertCaliItem.szIsInterior		= _T("��");	//�Ƿ��ڱ�
	insertCaliItem.szStdContent		= _T("");	//�ڱ꺬��
	insertCaliItem.szInteriorNo		= _T("");	//�ڱ���
	insertCaliItem.szSampleFullName = lpcszPathName;
	insertCaliItem.szPeakBeginX     = strArry[10];
	insertCaliItem.szPeakEndX       = strArry[11];

	//�ѱ���ʱ����ĳɲ���ı���ʱ��
	m_CaliItems[iStart].szReserveTime = strArry[2];


	//��ʼ���ݲ���
	vector<CaliItem> temp;
	const int itemNum = m_CaliItems.size();
	if (iRow<0 || iRow>itemNum-1)
	{
		::AfxMessageBox(_T("�޷��ڷ�Χ���������"));
		return false;
	}
	for (int i=0; i<=iRow; i++)
	{
		temp.push_back(m_CaliItems[i]);
	}
	temp.push_back(insertCaliItem);
	for (int i=iRow+1; i<itemNum; i++) 
	{
		temp.push_back(m_CaliItems[i]);
	}

	m_CaliItems.swap(temp);

	return true;
}

bool CCaliData::InsertRowData(int iRow, const CStringArray& strArry, LPCTSTR lpcszPathName)
{
	CaliItem insertCaliItem;
	CString szRow;
	szRow.Format(_T("%d"), iRow+1);
	insertCaliItem.szNO				= szRow;	//���
	insertCaliItem.szReserveTime	= strArry[2];	//����ʱ��
	insertCaliItem.szComponentName  = _T("Compound")+szRow;	//�����

	//�����µ���Ʒ����
	insertCaliItem.szLevel			= _T("1");//����
	insertCaliItem.szContents		= _T(""); //strArry[11];  //�������ȴ��û���������
	insertCaliItem.szPeakArea		= strArry[4];	//�����
	insertCaliItem.szPeakHeight		= strArry[6];	//���

	double dCont = _tstof(insertCaliItem.szContents);
	double dPeakArea = _tstof(insertCaliItem.szPeakArea);
	double dPeakHeight = _tstof(insertCaliItem.szPeakHeight);
	CString szFactor;

	if (m_tableType == _T("�����"))
	{
		double dResFactor = 0.0f / dPeakArea;  //������Ӧ����
		szFactor.Format(_T("%0.2f"), dResFactor);
		insertCaliItem.szResFactor		= szFactor;	
	}
	else if (m_tableType == _T("���"))
	{
		double dResFactor = 0.0 / dPeakHeight;	//������Ӧ����
		szFactor.Format(_T("%0.2f"), dResFactor);
		insertCaliItem.szResFactor		= szFactor;
	}
	else
	{
		::AfxMessageBox(_T("�����Ƿ�������Ƿ�ߣ�2"));
		return false;
	}

	insertCaliItem.szIsReference	= _T("��");	//�Ƿ�α�
	insertCaliItem.szIsInterior		= _T("��");	//�Ƿ��ڱ�
	insertCaliItem.szStdContent		= _T("");	//�ڱ꺬��
	insertCaliItem.szInteriorNo		= _T("");	//�ڱ���
	insertCaliItem.szSampleFullName = lpcszPathName;
	insertCaliItem.szPeakBeginX     = strArry[10];
	insertCaliItem.szPeakEndX       = strArry[11];

	m_CaliItems.push_back(insertCaliItem);
	return true;
}

bool CCaliData::GetSampleLevel(int& iValue)
{
	const int itemNum = m_CaliItems.size();
	int maxLv = 0;
	for (int i=0; i<itemNum; ++i)
	{
		CString szLevel = m_CaliItems[i].szLevel;
		
		int nLv = _wtoi(szLevel);
		if (nLv> maxLv)
		{
			maxLv = nLv;
		}
	}

	iValue = maxLv+1;
	return true;
}

bool CCaliData::ModifyInsertItemLevel()
{
	int iLevel=0;
	if(!GetSampleLevel(iLevel))
	{
		::AfxMessageBox(_T("�޷��Զ�������Ʒ����"));
		return false;
	}

	CString szLv;
	szLv.Format(_T("%d"), iLevel);

	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		CString szLevel = m_CaliItems[i].szLevel;
		if (szLevel == _T("#LEVEL#"))
		{
			m_CaliItems[i].szLevel = szLv;
		}
	}
	return true;
}

bool CCaliData::GetSampleFullPath(CStringArray& strArry)
{
	if(!strArry.IsEmpty()) strArry.RemoveAll();

	const int itemNum = m_CaliItems.size();
	std::set<CString> sampleFullPath;
	for (int i=0; i<itemNum; i++)
	{
		sampleFullPath.insert(m_CaliItems[i].szSampleFullName);
	}
	//����SampleNames���������deque<SamleFileInfo> m_InFileInfo
	typedef std::set<CString>::iterator SetIter;
	for (SetIter iter=sampleFullPath.begin(); iter!=sampleFullPath.end(); ++iter)
	{
		CString szPath = *iter;
		strArry.Add(szPath);
	}
	return true;
}

bool CCaliData::GetSampleFullPath(int row, CString& strVal)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1) return false;
	strVal = m_CaliItems[row].szSampleFullName;
	return true;
}

void CCaliData::SubOne(vector<int> &vec, int i)
{
	int itemNum = vec.size();
	for (int j=0; j<itemNum; ++j)
	{
		if (i == vec[j])
		{
			vec[j]--;
		}
	}
}

bool CCaliData::CheakCopyInfo(int iRow)
{
	int itemNum = m_CaliItems.size();
	if (iRow+1 >= itemNum) return false;

	CString strLevel = m_CaliItems[iRow].szLevel;
	int iLevel = _wtoi(strLevel);
	 

	CString strName = m_CaliItems[iRow+1].szComponentName;
	if (strName == _T("")) //˵����Ҫ����
	{
		m_CaliItems[iRow+1].szNO = m_CaliItems[iRow].szNO;
		m_CaliItems[iRow+1].szReserveTime = m_CaliItems[iRow].szReserveTime;		//����ʱ��
		m_CaliItems[iRow+1].szComponentName = m_CaliItems[iRow].szComponentName;	//�����
		m_CaliItems[iRow+1].szIsInterior = m_CaliItems[iRow].szIsInterior;	 
		m_CaliItems[iRow+1].szIsReference = m_CaliItems[iRow].szIsReference;
		m_CaliItems[iRow+1].szStdContent = m_CaliItems[iRow].szStdContent; // �ڱ꺬��
		m_CaliItems[iRow+1].szInteriorNo = m_CaliItems[iRow].szInteriorNo; // �ڱ���
	}
	return true;
}

bool CCaliData::DeleteRowData(CString& szPath)
{
	for (size_t i=0; i<m_CaliItems.size(); ++i)
	{
		if (m_CaliItems[i].szSampleFullName==szPath)
		{
			DeleteRowData(i);
			i = -1; //���¿�ʼѭ��
		}
	}
	return true;
}

bool CCaliData::DeleteRowData(int iRow)
{
	//�Ƿ�ɾ�����һ��
	if (m_CaliItems.size()==1)
	{
		if (iRow==0)
		{
			m_CaliItems.clear();
			return true;
		}
	}

	//���ɾ�������Ƿ���ĳ��ݵ�����
	CheakCopyInfo(iRow);

	vector<int> vecCL; //��ݼ��𣬴���������
	vector<CaliItem> tmpItem;
	int itemNum = m_CaliItems.size();
	int max_level = INT_MIN;
	int min_level = 1;//INT_MAX;
	for (int i = 0; i<itemNum; ++i)
	{
		if (i==iRow) //��������ɾ������
		{
			continue;
		}

		CString strLevel = m_CaliItems[i].szLevel;
		//���Ƿ���Ҫ���Ӷ��ַ���is���ֵļ�飿
		int iLevel = _wtoi(strLevel);
		if (iLevel == 0) ASSERT(TRUE); //���𲻿���Ϊ��
		
		if (iLevel > max_level) max_level = iLevel;
		
		vecCL.push_back(iLevel);
		tmpItem.push_back(m_CaliItems[i]);
	}
	
	//��������
	if (max_level == INT_MIN/* || min_level == INT_MAX*/)
	{
		return false;
	}

	int vecCLSize = vecCL.size();
	for (int i=min_level; i<=max_level; ++i)
	{
		for (int j=0; j<vecCLSize; ++j)
		{
			if (i == vecCL[j])
			{
				break;
			}
			if (j== vecCLSize-1)
			{
				int subObj = i+1;
				SubOne(vecCL, subObj);
			}
		}
	}

	//���¸�ֵ
	int tempItemNum = tmpItem.size();
	if (tempItemNum != vecCL.size())
	{
		return false;
	}
	for (int i=0; i<tempItemNum; ++i)
	{
		int iLevel = vecCL[i];
		CString strLevel;
		strLevel.Format(_T("%d"), iLevel);
		tmpItem[i].szLevel = strLevel;
	}

	tmpItem.swap(m_CaliItems);

	return true;
}

bool CCaliData::DeleteNotFirstRow() {
	vector<CaliItem> tmpItem;
	int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) {
		if (m_CaliItems[i].szComponentName!=_T(""))
			tmpItem.push_back(m_CaliItems[i]);
	}
	tmpItem.swap(m_CaliItems);
	return true;
}

void CCaliData::VerifyContent(){
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) {
		if (!VertifyIsInterior(i)) //���ڱ꣬����û�������ڱ꺬��
			ClearContent(i);
	}
}

bool CCaliData::ModifyContent(int nRow, CString& szContent)
{
	const int itemNum = m_CaliItems.size();
	if (nRow<0 || nRow>itemNum-1)
		return false;
	
	m_CaliItems[nRow].szStdContent = szContent;
	return true;
}

bool CCaliData::ClearContent(int nRow)
{
	const int itemNum = m_CaliItems.size();
	if (nRow<0 || nRow>itemNum-1)
		return false;

	m_CaliItems[nRow].szStdContent = _T("");
	return true;
}

bool CCaliData::GetPreContent(int nRow, CString& szContent)
{
	const int itemNum = m_CaliItems.size();
	if (nRow<0 || nRow>itemNum-1)
	{
		return false;
	}

	szContent = m_CaliItems[nRow].szStdContent;
	return true;
}

void CCaliData::CaliTable2CaliResultTable(const CCaliData& CaliTable){
	CCaliData temp;
	CaliTable.GenerateDuplicate(temp);
	
	const int tempSize = temp.GetItemSize();
	const int caliItemsSize = GetItemSize();

	int currSize =  (tempSize > caliItemsSize) ? caliItemsSize : tempSize;

	for (int i=0; i<currSize; ++i) {
		temp.m_CaliItems[i].szStdContent = m_CaliItems[i].szStdContent;
	}
	
	temp.m_CaliItems.swap(m_CaliItems);
}

//�ڱ�����غ���
bool CCaliData::AddInteriorNo(int nRow)
{
	//�߽���
	const int itemNum = m_CaliItems.size();
	if (nRow<0 || nRow>itemNum-1)
		return false;

	//�жϵ�ǰ�������Ƿ��Ѵ����ڱ���
	bool bIsExist = IsExistInteriorNo();
	if (!bIsExist)
	{//������
		return AllInteriorNoBe1();
	}
	//����
	////�ж�֮ǰ�Ƿ��Ѿ���ǳ��ڱ�
	//CString strItemNo = m_CaliItems[nRow].szInteriorNo;
	//if(strItemNo!=_T("1") && strItemNo!=_T(""))
	//	return true; //�����ı�
	//else{
	//	if(IsOnlyInteriorNo1(nRow))
	//		return true;
	//}
			

	//�ҳ�������ڱ���
	int no = AutoFindInteriorNo();
	if (no == -1)
	{
		::AfxMessageBox(_T("ϵͳ��֧��3���ڱ�"));
		return false;
	}
	CString szNo;
	szNo.Format(_T("%d"), no);
	m_CaliItems[nRow].szInteriorNo = szNo;

	return true;
}

bool CCaliData::IsOnlyInteriorNo1(int row)
{//���Ե�ǰ���Ƿ���Ψһ�ڱ���Ϊ1��
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		if(m_CaliItems[i].szComponentName!=_T(""))
		{
			if (m_CaliItems[i].szInteriorNo == _T("1"))
			{
				if (i!=row)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

bool CCaliData::IsExistInteriorNo()
{//����������Ƿ��Ѵ����ڱ���
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		if(m_CaliItems[i].szInteriorNo!=_T(""))
			return true;
	}
	return false;
}

bool CCaliData::AllInteriorNoBe1()
{//ÿ����ݵ��ڱ���Ϊ1
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		if(m_CaliItems[i].szComponentName!=_T(""))
		{
			m_CaliItems[i].szInteriorNo = _T("1");
		}
	}
	return true;
}

int  CCaliData::AutoFindInteriorNo()
{//�ҳ�������ڱ���
	int no=1;
	set<int> set_No;
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		if(m_CaliItems[i].szComponentName!=_T(""))
		{
			int itemNo = _wtoi(m_CaliItems[i].szInteriorNo);
			if (itemNo!=0)
				set_No.insert(itemNo);
		}
	}

	if (set_No.size()>=3)
	{
		return -1; //Ŀǰ��֧��3���ڱ�
	}

	typedef set<int>::iterator SETITER;
	for (SETITER iter = set_No.begin(); iter!=set_No.end(); ++iter)
	{
		if (*iter==no)
		{
			no++;
		}
	}
	return no;
}

int CCaliData::FindMinInteriorNo() 
{
	int min_no = 4; //��ΪĿǰ����ڱ겻�ᳬ��3
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		if(m_CaliItems[i].szInteriorNo!=_T(""))
		{
			int itemNo = _wtoi(m_CaliItems[i].szInteriorNo);
			if (itemNo <= min_no)
				min_no = itemNo;
		}
	}
	return min_no;
}

bool CCaliData::IsInteriorNoOnlyOne()
{
	set<int> set_No;
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		if(m_CaliItems[i].szIsInterior!=_T(""))
		{
			int itemNo = _wtoi(m_CaliItems[i].szInteriorNo);
			set_No.insert(itemNo);
		}
	}
	if (set_No.size()!=1)
	{
		return false;
	}
	return true;
}

bool CCaliData::CheckExistInteriorNo(int row, const CString& strVal)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1) return false;
	//CString target = m_CaliItems[row].szInteriorNo; //�ҵ�Դ���ڱ���
	for (int i=0; i<itemNum; ++i)
	{
		if (m_CaliItems[i].szIsInterior==_T("��"))
		{
			if (i!=row)
			{
				if (m_CaliItems[i].szInteriorNo==strVal)
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool CCaliData::ChangeSameCompnentInteriorNo(int row, const CString& dest)
{//�ı�ͬ�ڱ��ŵ��������
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1)
		return false;

	CString sour = m_CaliItems[row].szInteriorNo; //�ҵ�Դ���ڱ���
	//�����޸�
	for (int i=0; i<itemNum; ++i)
	{
		if (m_CaliItems[i].szInteriorNo==sour)
			m_CaliItems[i].szInteriorNo = dest;
	}
	return true;

}

bool CCaliData::VertifyInteriorNoInterval(const CString& strNo)
{//����strNo�Ƿ��ڵ�ǰ�ڱ��ŷ�Χ�ڡ�����ֵ��Χ��������ƥ�䡿
	set<int> set_No;
	int no = _wtoi(strNo);
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		int itemNo = _wtoi(m_CaliItems[i].szInteriorNo);
		if (itemNo!=0)
			set_No.insert(itemNo);
	}
	typedef set<int>::iterator SETITER;
	for (SETITER iter = set_No.begin(); iter!=set_No.end(); ++iter)
	{
		if (*iter==no)
			return true;
	}
	return false;
}

bool CCaliData::ManualInteriorNo(int col, int row, LPCTSTR szVal)
{
	CString strVal(szVal); 
	//***�ж����������Ƿ�����Ȼ��
	if(!IsNaturalNumber(strVal)) {
		::AfxMessageBox(_T("�ڱ��ű���Ϊ����0����Ȼ��"));
		return false;
	}

	//***�жϸõ�Ԫ���Ӧ�Ƿ���ָ��Ϊ�ڱ�
	if(VertifyIsInterior(row)) {
	//���ڱ�
		if (strVal==_T("")) {
			::AfxMessageBox(_T("���ڱ���ֵı��ɾ��Ϊ��"));
			return false;
		}
		//��鵱ǰ�ı�ı���Ƿ���Ѵ��ڵı�ų�ͻ
		if(!CheckExistInteriorNo(row, strVal)) {
			::AfxMessageBox(_T("�޸��ڱ��������е��ڱ����ظ�"));
			return false;
		}

		//�ı�ͬ�ڱ��ŵ��������
		ChangeSameCompnentInteriorNo(row, strVal);
	}
	else {
	//�����ڱ�
		//����ֶ��޸ĵ��ڱ�ķ�Χ�����ܳ�����ǰ���еķ�Χ
		if(VertifyInteriorNoInterval(strVal)) {
		//�ڷ�Χ�ڿ���ֱ���޸�
			m_CaliItems[row].szInteriorNo = strVal;
		}
		else //������Χ�������޸�
		{
			::AfxMessageBox(_T("���޸ĵ��ڱ��ų�����ǰ��Χ"));
			return false;
		}
	}
	return true;
}

void CCaliData::AutoCheckInteriorNoAfterDel()
{
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) {
		bool needDel = true;
		if (m_CaliItems[i].szIsInterior==_T("��")) {
			CString cStrINO = m_CaliItems[i].szInteriorNo;
			for (int j=0; j<itemNum; ++j) {
				if (m_CaliItems[j].szIsInterior==_T("��")) {
					if (cStrINO==m_CaliItems[j].szInteriorNo)
					{
						needDel = false;
					}
				}
			}
			if (needDel)
			{
				m_CaliItems[i].szInteriorNo = _T("");
			}
		}
	}
}

void CCaliData::AutoReorderID() {
	int reorderId = 1;
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) {
		if (m_CaliItems[i].szNO!=_T("")) {
			CString cStrReorderID;
			cStrReorderID.Format(_T("%d"), reorderId);
			m_CaliItems[i].szNO = cStrReorderID;
			reorderId++;
		}
	}
}

bool CCaliData::DesignatedReference(int row, bool bIsReference)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1)
		return false;

	if (bIsReference)
		m_CaliItems[row].szIsReference = _T("��");
	else
		m_CaliItems[row].szIsReference = _T("��");
	return true;
}

bool CCaliData::DesignatedInterior(int row, bool bIsInterior)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1)
	{
		return false;
	}
	if (bIsInterior)
		m_CaliItems[row].szIsInterior = _T("��");
	else
		m_CaliItems[row].szIsInterior = _T("��");
	
	return true;
}

bool CCaliData::SyncDelInterior(int row)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1) return false;
	if (m_CaliItems[row].szIsInterior==_T("��")) return false;

	CString cStrNo = m_CaliItems[row].szInteriorNo;
	for (int i=0; i<itemNum; ++i)
	{
		if (m_CaliItems[i].szInteriorNo==cStrNo)
			m_CaliItems[i].szInteriorNo = _T("");
	}
	return true;
}

void CCaliData::ClearAllInteriorNo()
{
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		if(m_CaliItems[i].szInteriorNo!=_T(""))
		{
			m_CaliItems[i].szInteriorNo=_T("");
		}
	}
}

bool CCaliData::SynEditedCell(int col, int row, LPCTSTR szVal)
{
	if (szVal == _T("")) { 
		//::AfxMessageBox(_T("�����������Ϊ��"));
		return false;
	}

	if (col == 5){
		CString cStr_tmp;
		cStr_tmp.Format(_T("%s"), szVal);
		if (!IsDigital(cStr_tmp))
			return false;
	}

	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1)
	{
		return false;
	}

	if (col==3)
	{
		if (m_CaliItems[row].szComponentName != _T("")) {
			m_CaliItems[row].szComponentName = szVal;
		}
		else{
			//::AfxMessageBox(_T("�������ڴ˵�Ԫ���޸�����"));
			return false;
		}
	}
	else if (col==5)
	{
		CString szTmp;
		szTmp.Format(_T("%s"), szVal);
		//double dContents = _tstof(szTmp);
		//
		//if (dContents < 0.1f)
		//{
		//	szTmp.Format(_T("%e"), dContents);
		//}
		//else
		//{
		//	ConvertDecimalDigits(szTmp, 3); // ǿ�Ʊ���С��λ��3λ
		//}
		m_CaliItems[row].szContents = szTmp;

	    // ���¼���[��Ӧ����]
		double dCont       = _tstof(m_CaliItems[row].szContents);
		double dPeakArea   = _tstof(m_CaliItems[row].szPeakArea);
		double dPeakHeight = _tstof(m_CaliItems[row].szPeakHeight);
		CString szFactor;
		if (m_tableType == _T("�����"))
		{
			double dResFactor = dCont / dPeakArea;
			m_CaliItems[row].szResFactor.Format(_T("%.4e"), dResFactor);

			//if (dResFactor == 0.0000f)
			//{
			//	m_CaliItems[row].szResFactor = _T("0.0000");
			//}
			//else
			//{
			//	szFactor.Format(_T("%.4e"), dResFactor);
			//	if (dResFactor >= 1.000000f)
			//		EraseZeroPlus(szFactor);
			//	else
			//		EraseZeroMinus(szFactor);
			//	m_CaliItems[row].szResFactor		= szFactor;	// ��Ӧ����
			//}

		}
		else if (m_tableType == _T("���"))
		{
			double dResFactor = dCont / dPeakHeight;
			m_CaliItems[row].szResFactor.Format(_T("%.4e"), dResFactor);

			//if (dResFactor == 0.0000f)
			//{
			//	m_CaliItems[row].szResFactor = _T("0.0000");
			//}
			//else
			//{
			//	szFactor.Format(_T("%.4e"), dResFactor);
			//	if (dResFactor >= 1.000000f)
			//		EraseZeroPlus(szFactor);
			//	else
			//		EraseZeroMinus(szFactor);
			//	m_CaliItems[row].szResFactor		= szFactor;	// ��Ӧ����
			//}
		}
		else
		{
			//::AfxMessageBox(_T("�����Ƿ�������Ƿ�ߣ�2"));
			return false;
		}
	}
	else
	{
		//::AfxMessageBox(_T("��������")); // �����ϲ������е�����
		return false;
	}

	return true;
}

bool CCaliData::VertifyFirstLine(int row)
{
	ClearUnnecessaryCell(row);//�����һ��������е�������Ԫ��������

	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1)
		return false;

	if (m_CaliItems[row].szComponentName != _T(""))
		return true;
	
	return false;
}

bool CCaliData::VertifyIsReference(int row)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1)
		return false;
	if (m_CaliItems[row].szIsReference==_T("��"))
		return true;
	return false;
}

bool CCaliData::VertifyIsInterior(int row)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1)
		return false;
	if (m_CaliItems[row].szIsInterior==_T("��"))
		return true;
	return false;
}

void CCaliData::ClearUnnecessaryCell()
{//�����һ��������е�������Ԫ��������[���][����ʱ��][�����][�Ƿ�α�][�Ƿ��ڱ�][�ڱ���]
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		if (m_CaliItems[i].szComponentName == _T("")) {
			m_CaliItems[i].szNO			 = _T("");
			m_CaliItems[i].szReserveTime = _T("");
			m_CaliItems[i].szIsReference = _T("");
			m_CaliItems[i].szIsInterior	 = _T("");
			m_CaliItems[i].szInteriorNo  = _T(""); 
		}
	}
}

void CCaliData::ClearUnnecessaryCell(int row)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1)
		return ;
	if (m_CaliItems[row].szComponentName == _T(""))
	{
		m_CaliItems[row].szNO			 = _T("");
		m_CaliItems[row].szReserveTime   = _T("");
		m_CaliItems[row].szIsReference   = _T("");
		m_CaliItems[row].szIsInterior	 = _T("");
		m_CaliItems[row].szInteriorNo    = _T("");
	}
}

void CCaliData::UnifyDecimalDigits()
{
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i)
	{
		CString szFactor;
		double dCont       = _tstof(m_CaliItems[i].szContents);
		double dPeakArea   = _tstof(m_CaliItems[i].szPeakArea);
		double dPeakHeight = _tstof(m_CaliItems[i].szPeakHeight);

		ConvertDecimalDigits(m_CaliItems[i].szContents, 3); //����
		ConvertDecimalDigits(m_CaliItems[i].szPeakArea, 3); //�����
		ConvertDecimalDigits(m_CaliItems[i].szPeakHeight, 3); //���

		if (m_tableType == _T("�����"))
		{
			double dResFactor = dCont / dPeakArea;
			szFactor.Format(_T("%.4e"), dResFactor);
			if (dResFactor >= 1.000000f)
				EraseZeroPlus(szFactor);
			else
				EraseZeroMinus(szFactor);
			m_CaliItems[i].szResFactor		= szFactor;	// ��Ӧ����
		}
		else if (m_tableType == _T("���"))
		{
			double dResFactor = dCont / dPeakHeight;
			szFactor.Format(_T("%.4e"), dResFactor);
			if (dResFactor >= 1.000000f)
				EraseZeroPlus(szFactor);
			else
				EraseZeroMinus(szFactor);
			m_CaliItems[i].szResFactor		= szFactor;	// ��Ӧ����
		}
		else
		{
			::AfxMessageBox(_T("�����Ƿ�������Ƿ��??"));
			return;
		}
	}
}

void CCaliData::GetComponentName(int row, CString& strName)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1) return;
	while (row!=-1)
	{
		if (m_CaliItems[row].szComponentName!=_T(""))
		{
			strName = m_CaliItems[row].szComponentName;
			break;
		}
		else
			row--;
	}
}

void CCaliData::GetReserveTime(int row, CString& strVal)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1) return;
	while (row!=-1)
	{
		if (m_CaliItems[row].szReserveTime!=_T(""))
		{
			strVal = m_CaliItems[row].szReserveTime;
			if (!IsDigital(strVal))
				Write_Error("����ʱ��������ַ���");
			break;
		}
		else
			row--;
	}
}

void CCaliData::GetOpEdXTime(int row, double &dOp, double& dEd)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1) return;
	dOp =  _tstof(m_CaliItems[row].szPeakBeginX);
	dEd =  _tstof(m_CaliItems[row].szPeakEndX);
}

bool CCaliData::GetComponentIndex(int row, int& b, int& e)
{
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1) return false;
	for (int i=row; i>=0; --i)
	{
		if (m_CaliItems[i].szComponentName!=_T(""))
		{
			b = i;
			break;
		}
	}
	if (b+1==itemNum)
	{
		e = b;
		return true;
	}
	for (int i=b+1; i<itemNum; ++i)
	{
		if (m_CaliItems[i].szComponentName!=_T(""))
		{
			e = i-1;
			break;
		}

		if (i==itemNum-1)
		{
			e=itemNum-1;
			break;
		}

		else
		{
			e = b;
		}
	}
	return true;
}

bool CCaliData::GetCaliCurveData_TypeA(int row, deque<CaliCurveData>& data) {
	int b=0,e=-1;
	GetComponentIndex(row, b, e);
	if (b > e) return false;
	for (int row=b; row<=e; ++row)
	{
		if (m_CaliItems[row].szContents==_T("")){
			data.clear();
			return false;
		}
		double dContents = _tstof(m_CaliItems[row].szContents);
		if (dContents==0.0000f) continue;

		if (m_tableType==_T("�����")) {
			double dPeakArea = _tstof(m_CaliItems[row].szPeakArea);
			CaliCurveData ccd(dContents, dPeakArea);                 
			data.push_back(ccd);
		}
		else if (m_tableType==_T("���")) {
			double dPeakHeight = _tstof(m_CaliItems[row].szPeakHeight);   
			CaliCurveData ccd(dContents, dPeakHeight);                 
			data.push_back(ccd);
		}
		else 
			return false;
	}
	return true;
}

bool CCaliData::GetCaliCurveData_TypeB(int row, deque<CaliCurveData>& data) {
                                                         //need refactoring
	//�ж������ڱ��������Ƿ������1���ڱ꣬��������ʾ
	bool hasInterior=false;
	const int itemSize = m_CaliItems.size();
	for (int i=0; i<itemSize; ++i) {
		if (m_CaliItems[i].szIsInterior==_T("��")){
			hasInterior=true;
			break;
		}
	}
	if (!hasInterior) return false;

	
	//�ж�ѡ������Ƿ���Ϊ�ڱ�
	int b=0,e=-1;
	GetComponentIndex(row, b, e);
	if (b > e) return false;
	//У��������ʾ"��ЧУ������"��ֱ������ʾ
	if (m_CaliItems[b].szIsInterior==_T("��")) return false;
	//ѡ��������ڱ��ŵģ����������У������	
	if (m_CaliItems[b].szInteriorNo==_T(""))   return false;


	//��ô�ҵ�ͬ�ڱ�����ͬ������ڱ�
	CString cStrNo = m_CaliItems[b].szInteriorNo;
	const int itemNum = m_CaliItems.size();
	int bb=0,ee=0;
	for (int i=0; i<itemNum; ++i)
	{
		if (i>=b && i<=e) continue;
		if (m_CaliItems[i].szInteriorNo == cStrNo)
		{
			if (m_CaliItems[i].szIsInterior==_T("��"))
			{
				GetComponentIndex(i, bb, ee);
				break;
			}
		}
	}

	for (int row=b; row<=e; ++row)
	{
		CString cStrLv = m_CaliItems[row].szLevel;
		for (int irow = bb; irow<=ee; irow++)
		{
			if (m_CaliItems[irow].szLevel == cStrLv) //ȫƥ��
			{
				if (m_CaliItems[row].szContents==_T("")||m_CaliItems[irow].szContents==_T("")){
					data.clear();
					return false;
				}
				double dContents_c = _tstof(m_CaliItems[row].szContents);
				double dContents_i = _tstof(m_CaliItems[irow].szContents);
				if (dContents_i==0.0000f) continue;
				double _x = dContents_c/dContents_i;
				double _y;
				if (m_tableType==_T("�����")) {
					double dPeakArea_c = _tstof(m_CaliItems[row].szPeakArea); 
					double dPeakArea_i = _tstof(m_CaliItems[irow].szPeakArea); 
					if (dPeakArea_i==0.0000f) continue;
					_y = dPeakArea_c/dPeakArea_i;
				} else if(m_tableType==_T("���")) {
					double dPeakHeight_c = _tstof(m_CaliItems[row].szPeakHeight); 
					double dPeakHeight_i = _tstof(m_CaliItems[irow].szPeakHeight); 
					if (dPeakHeight_i==0.0000f) continue;
					_y = dPeakHeight_c/dPeakHeight_i;
				} else {
					return false;
				}
				if (_x==0.0000f) continue;

				if (_x!=1.0000f || _y!=1.0000f) { //�����ԭ��&& �ĳ�||
					CaliCurveData ccd(_x, _y);
					data.push_back(ccd);
				}
			}
		} 
	}
	return true;
}

bool CCaliData::SetSlope_Intercept_Coefficient(int row, double s, double i, double c) {
	const int itemNum = m_CaliItems.size();
	if (row<0 || row>itemNum-1) return false;

	//if (s==i==c==0.0f)
	//{
	//	m_CaliItems[row].szSlope = _T("");
	//	m_CaliItems[row].szIntercept = _T("");
	//	m_CaliItems[row].szCoefficient = _T("");
	//}

	
	CString cStrSlope, cStrIntercept, cStrCoefficient;
	cStrSlope.Format(SLOPE_DATA_FORMAT, s);
	cStrIntercept.Format(INTERCEPT_DATA_FORMAT, i);
	cStrCoefficient.Format(COEFFICIENT_DATA_FORMAT, c);

	m_CaliItems[row].szSlope = cStrSlope;
	m_CaliItems[row].szIntercept = cStrIntercept;
	m_CaliItems[row].szCoefficient = cStrCoefficient;

	return true;
}

void CCaliData::AddData(const CaliItem& item){
	this->m_CaliItems.push_back(item);
}

void CCaliData::GenerateDuplicate(CCaliData& data)const {
	if (data.GetItemSize()==0)	data.ClearAllData();

	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) 
		data.AddData(m_CaliItems[i]);
}

void CCaliData::GetRefPeakData(CCaliData& data){
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) {
		if(m_CaliItems[i].szIsReference == _T("��"))
			data.AddData(m_CaliItems[i]);
	}
}

void CCaliData::CancelRefPeakMark(int no){
	int find_no=0;
	for (int i=0; i<GetItemSize(); i++)
	{
		if (m_CaliItems[i].szIsReference == _T("��"))
		{
			if (find_no==no)
			{
				m_CaliItems[i].szIsReference = _T("��");
			}
		}
	}
}

void CCaliData::CancelIntPeakMark(int no){
	int find_no=0;
	for (int i=0; i<GetItemSize(); i++)
	{
		if (m_CaliItems[i].szIsInterior == _T("��"))
		{
			if (find_no==no)
			{
				m_CaliItems[i].szIsInterior = _T("��");
			}
		}
	}
}

void CCaliData::GetIntPeakData(CCaliData& data){
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) {
		if(m_CaliItems[i].szIsInterior == _T("��"))
			data.AddData(m_CaliItems[i]);
	}
}

void CCaliData::GetcOtherPeakData(CCaliData& data){
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) {
		if(m_CaliItems[i].szIsReference == _T("��"))
			if(m_CaliItems[i].szIsInterior == _T("��"))
				data.AddData(m_CaliItems[i]);
	}
}

void CCaliData::ModifyResTime(vector<double> &afterResTime) {
	if (afterResTime.empty()) return;

	const int itemNum = m_CaliItems.size();

	CCaliData RefPeak;
	GetRefPeakData(RefPeak);

	if (RefPeak.GetItemSize()!=afterResTime.size())
	{
		::AfxMessageBox(_T("�ο��������ƥ��"));
		ASSERT(TRUE);
		return;
	}

	for (int i=0; i<itemNum; ++i) {
		if(m_CaliItems[i].szIsReference != _T("��")) //���ֶ�Ӧ�����ǣ��񣬿� 3������
			continue;
		int iStart=0, iEnd=INT_MAX; //��δ֪���Ӧ�Ĳ����������

		GetRefPeakInterval(i, iStart, iEnd);

		CString Ref_i_1_before;
		CString Ref_i_before;
		CString NofRef = m_CaliItems[i].szReserveTime;

		double  dRef_i_1_after;
		double  dRef_i_after;
		double  dRef_i_1_before;
		double  dRef_i_before;
		double  dNofRef_before = _tstof(NofRef);

		if (iStart==0){
			dRef_i_1_before=0.0f;
			dRef_i_1_after=0.0f;
		}
		else {
			RefPeak.GetReserveTime(iStart-1, Ref_i_1_before);
			dRef_i_1_before=_tstof(Ref_i_1_before);
			dRef_i_1_after = afterResTime[iStart-1];
		}
		

		if (iEnd==INT_MAX){
			dRef_i_before=0.0f;
			dRef_i_after =0.0f;
		}
		else{
			RefPeak.GetReserveTime(iEnd-1, Ref_i_before);
			dRef_i_before=_tstof(Ref_i_before);
			dRef_i_after =afterResTime[iEnd-1];
		}
		

		if (dRef_i_before-dRef_i_1_before==0.0f)
		{
			::AfxMessageBox(_T("�ο���ǰ������һ�£���������ݼ��"));
			continue;
		}
		double  dNofRef_after = (dNofRef_before-dRef_i_1_before)*(dRef_i_after-dRef_i_1_after)/(dRef_i_before-dRef_i_1_before)+dRef_i_1_after;
		CString szModifyResTime;
		szModifyResTime.Format(RESERVETIME_DATA_FORMAT, dNofRef_after);
		m_CaliItems[i].szReserveTime=szModifyResTime;
	}
}

void CCaliData::GetRefPeakInterval(int n, int& before, int& after) {
	const int itemNum = m_CaliItems.size();
	for (int i=n-1; i>=0; --i) {
		if(m_CaliItems[i].szIsReference == _T("��")) {
			before = Row2No(i); 
			break;
		}
	}

	for (int i=n+1; i<itemNum; ++i) {
		if(m_CaliItems[i].szIsReference == _T("��")) {
			after = Row2No(i); 
			break;
		}
	}
}

int CCaliData::Row2No(int row) {
	int result=0;
	const int itemNum = m_CaliItems.size();
	for (int i=0; i<itemNum; ++i) {
		if(m_CaliItems[i].szIsReference == _T("��")) {
			result++;
			if (i==row)
				return result;
		}
	}

	return 0;
}




const CStringArray& CCaliMethod::StringIDs()
{
	static CStringArray sa;
	if(sa.IsEmpty())
	{
		sa.Add(XMLVALUE_CONST_c_percentage);
		sa.Add(XMLVALUE_CONST_c_normalization);
		sa.Add(XMLVALUE_CONST_c_internal);
		sa.Add(XMLVALUE_CONST_c_external);
		sa.Add(XMLVALUE_CONST_c_internal_percentage);
		sa.Add(XMLVALUE_CONST_c_external_percentage);
	}
	return sa;
}
const CStringArray& CCaliMethod::StringNames()
{
	static CStringArray sa;
	if(sa.IsEmpty())
	{
		sa.Add(_T("�ٷֱȷ�"));
		sa.Add(_T("��һ����"));
		sa.Add(_T("�ڱ귨"));
		sa.Add(_T("��귨"));
		sa.Add(_T("�ڱ�ٷֱȷ�"));
		sa.Add(_T("���ٷֱȷ�"));
	}
	return sa;
}
int CCaliMethod::IndexByStringID(const CString& id)
{
	const CStringArray& sa = StringIDs();
	for(int i=0; i<sa.GetCount(); ++i)
	{
		if(sa.GetAt(i) == id) return i;
	}
	return -1;
}
const CString& CCaliMethod::StringIDByIndex(int index)
{
	static CString e;
	const CStringArray& sa = StringIDs();
	if(index < 0 || index >= sa.GetCount()) return e;
	return sa.GetAt(index);
}
const CString* CCaliMethod::NameByID(const CString& id)
{
	int i = IndexByStringID(id);
	if(i < 0) return NULL;
	return &StringNames().GetAt(i);
}

const CStringArray& CCaliMethodOn::StringIDs()
{
	static CStringArray sa;
	if(sa.IsEmpty())
	{
		sa.Add(XMLVALUE_CONSTENT_c_peak_area);
		sa.Add(XMLVALUE_CONSTENT_c_peak_height);
	}
	return sa;
}
const CStringArray& CCaliMethodOn::StringNames()
{
	static CStringArray sa;
	if(sa.IsEmpty())
	{
		sa.Add(_T("�����"));
		sa.Add(_T("���"));
	}
	return sa;
}
int CCaliMethodOn::IndexByStringID(const CString& id)
{
	const CStringArray& sa = StringIDs();
	for(int i=0; i<sa.GetCount(); ++i)
	{
		if(sa.GetAt(i) == id) return i;
	}
	return -1;
}
const CString& CCaliMethodOn::StringIDByIndex(int index)
{
	static CString e;
	const CStringArray& sa = StringIDs();
	if(index < 0 || index >= sa.GetCount()) return e;
	return sa.GetAt(index);
}
const CString* CCaliMethodOn::NameByID(const CString& id)
{
	int i = IndexByStringID(id);
	if(i < 0) return NULL;
	return &StringNames().GetAt(i);
}

void CCaliUI::InitComboBox(CComboBox* pCB, const CStringArray* pSA)
{
	if(!pCB) return;
	pCB->ResetContent();
	if(pSA)
	{
		for(int i=0; i<pSA->GetCount(); ++i)
		{
			pCB->AddString(pSA->GetAt(i));
		}
	}
}
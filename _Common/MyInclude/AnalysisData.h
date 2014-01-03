#pragma once
/*
#include <xercesc/dom/DOM.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/StdOutFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xqilla/xqilla-dom3.hpp>
#include <xqilla/xqilla-simple.hpp>


XERCES_CPP_NAMESPACE_USE
*/

//#include "ugctrl.h"

#include "MarkupMSXML.h"
#include "CStringExt.h"
#include "ErrorCode.h"
#include "AnalysisXMLDefine.h"

//////////////////////////////////////////////////////////////////////////
// ��������
//////////////////////////////////////////////////////////////////////////

//-------------------------����ʱ��ת���ĺ�---------------------------------------
#define TIME_TO_STRING_FORMAT		_T("%Y-%m-%d %H:%M:%S")	//2005-01-10T12:30:00
#define STRING_TO_TIME_FORMAT		_T("%d-%d-%d %d:%d:%d")
#define STRING_TO_TIME_FORMAT_ST	_T("%d-%d-%dT%d:%d:%d")

extern CString TimeToXMLString(const CTime& time);
extern CTime XMLStringToTime(const CString & timeStr);
extern bool SaveXmlToFile(LPCTSTR file, LPCTSTR xmlString);


//--------�������ݵ����ݽṹ--------------------------------
class CIADataTable;
class CIADataCurve2D;




//------------------------------------------------------------------------+
//-------------------------------- XML���Զ��� ----------------------------|
//------------------------------------------------------------------------+

//--------ͨ������----
// #define XMLPROP_NAME		_T("_E_Name_")		//����
// #define XMLPROP_VALUETYPE	_T("_E_Type_")		//��������
// #define XMLPROP_FORMAT		_T("_E_Format_")	//������ʾ��ʽ
// #define XMLPROP_UNIT		_T("_E_Unit_")		//���ݵ�λ
// #define XMLPROP_VALUE		_T("_E_Value_")		//����(ȱʡ)ֵ

//--------�������----
// #define XMLPROP_TAB_ORDERS			_T("Orders")
// #define XMLPROP_TAB_ORDERBY			_T("OrderBy")
// #define XMLPROP_TAB_ASC				_T("Asc")

//ÿ������(ע��ÿ��ͬʱ��������5��ͨ������
// #define XMLPROP_TAB_COLUMN			_T("Column")		//�б��
// 
// #define XMLPROP_TAB_COL_TITLE		_T("Title")			//��ID
// #define XMLPROP_TAB_COL_ID			_T("ID")			//��ID
// #define XMLPROP_TAB_COL_WIDTH		_T("ColumnWidth")	//�п��
// #define XMLPROP_TAB_COL_READONLY	_T("ReadOnly")		//true����false,�Ƿ��ܱ༭
// 
// #define XMLPROP_TAB_ROW				_T("Row")			//�б��


//------------------------------------------------------------------------+
//--------------------------------- XMLֵ���� -----------------------------|
//------------------------------------------------------------------------+
// #define XMLVALUE_TYPE_STRING	_T("string")
// #define XMLVALUE_TYPE_INT		_T("int")
// #define XMLVALUE_TYPE_FLOAT		_T("float")
// #define XMLVALUE_TYPE_TIME		_T("time")
// #define XMLVALUE_TYPE_TABLE		_T("table")
// #define XMLVALUE_TYPE_CURVE2D	_T("curve2d")

////////////////////////////////////////////////////////////////////////////////////
// CAnalysisData
////////////////////////////////////////////////////////////////////////////////////
//����int�͵ķ���ֵ����������˵��������IAERROR_XXX
//·��_T("/")��ʾ��Ŀ¼

/*
//�����ӽڵ�
CAnalysisData ad;
ad.LoadFromFile(file);
CAnalysisData::NODE node = NULL;
CAnalysisData::static_FisrtChild(ad.GetRoot(), node);
while(node)
{
	//����node����node��ad.GetRoot()���ӽڵ㣬���Ҫ����ad.GetRoot()����ڵ�򳬴��ڵ㣬��Ҫ�ݹ�
	CAnalysisData::static_NextChild(node);
}

//����һ��ָ��·��
����·��Ϊ/A/B/C
CStringArray sa;
sa.Add(_T("A"));
sa.Add(_T("B"));
sa.Add(_T("C"));
CAnalysisData::NODE node = ad.GetRoot();
for(int i=0; i<sa.GetCount(); ++i)
{
	node = CAnalysisData::static_SubNode(node, sa.GetAt(i), false);
	if(node)//�ҵ��ˣ�0��A��1��B��2��C
	{
	}
	else break;
}

//��������
	CAnalysisData ad;
	ad.LoadFromFile(_T("D:\\PWB\\chem2009\\Debug\\Data\\Sample\\11111_#4.cg.smp"));
	CAnalysisData::NODE node = CAnalysisData::static_Path(ad.GetRoot(), _T("Sample/MethodID"), false);
	DWORD dwCount = CAnalysisData::static_PropertiesCount(node);
	for(DWORD i=0; i<dwCount; ++i)
	{
		CAnalysisData::NODE pnode = CAnalysisData::static_NodeProperty(node, i);
		CString name = CAnalysisData::static_NodeName(pnode);
		CString v;
		CAnalysisData::static_GetValue(pnode, v);
	}
//*/

class CAnalysisData
{
#ifdef _DEBUG
public:
	static bool EnableAssertionsForInfoValidating;
#endif

public:
	typedef XercesDOMParser _PARSER;
	typedef XercesDOMParser* PARSER;
	typedef const XercesDOMParser* CONST_PARSER;

	typedef xercesc_2_8::DOMDocument* DOCUMENT;
	typedef const xercesc_2_8::DOMDocument* CONST_DOCUMENT;

	typedef DOMNode* NODE;
	typedef const DOMNode* CONST_NODE;
	
    enum enumNodeType
	{
		ELEMENT_NODE                = 1,
		ATTRIBUTE_NODE              = 2,
		TEXT_NODE                   = 3,
		CDATA_SECTION_NODE          = 4,
		ENTITY_REFERENCE_NODE       = 5,
		ENTITY_NODE                 = 6,
		PROCESSING_INSTRUCTION_NODE = 7,
		COMMENT_NODE                = 8,
		DOCUMENT_NODE               = 9,
		DOCUMENT_TYPE_NODE          = 10,
		DOCUMENT_FRAGMENT_NODE      = 11,
		NOTATION_NODE               = 12
    };

	static CString static_NodeName(CONST_NODE pNode);
	static enumNodeType static_NodeType(CONST_NODE pNode);
	
	static DWORD static_ChildrenCount(CONST_NODE pParent);
	static NODE static_ChildNode(NODE pParent, DWORD dwIndex);
	static void static_FisrtChild(NODE pParent, NODE& pos);
	static NODE static_NextChild(NODE& pos);
	static void static_LastChild(NODE pParent, NODE& pos);
	static NODE static_PrevChild(NODE& pos);
	static void static_FisrtChild_Const(CONST_NODE pParent, CONST_NODE& pos);
	static CONST_NODE static_NextChild_Const(CONST_NODE& pos);
	static void static_LastChild_Const(CONST_NODE pParent, CONST_NODE& pos);
	static CONST_NODE static_PrevChild_Const(CONST_NODE& pos);
	
	static DWORD static_PropertiesCount(CONST_NODE pParent);
	static NODE static_NodeProperty(NODE pParent, DWORD dwIndex);

	static NODE static_SubNode(NODE pParent, const CString& strSubTag, bool bAutoCreate);
	static NODE static_NodeProperty(NODE pNode, const CString& strProperty);
	static NODE static_Path(NODE pRoot, LPCTSTR strXMLPath, bool bAutoCreate);
	static NODE static_Path(PARSER pParser, LPCTSTR strXMLPath, bool bAutoCreate);

	static int static_GetOuterXML(CONST_NODE pNode, CString& xml);
	static int static_GetInnerXML(CONST_NODE pNode, CString& xml);
	static int static_SetOuterXML(NODE pNode, LPCTSTR xml);
	static int static_SetInnerXML(NODE pNode, LPCTSTR xml);
	static int static_Replace(NODE pOldNode, NODE pNewNode);
	static int static_Remove(NODE pNode);//ɾ���Ǹ��ڵ�
	
	static PARSER static_LoadXML(LPCTSTR xml);
	static PARSER static_LoadXMLFile(LPCTSTR xmlFile);
	static int static_Save(CONST_PARSER pParser, LPCTSTR xmlFile);
	static int static_Save(CONST_NODE pNode, LPCTSTR xmlFile);
	
	static bool static_GetValue(CONST_NODE pNode, CString& value);
	static bool static_SetValue(NODE pNode, LPCTSTR value);
	static bool static_GetProperty(CONST_NODE pNode, const CString& strProperty, CString& value);
	static bool static_SetProperty(NODE pNode, const CString& strProperty, LPCTSTR value);

	static bool static_IsEqual(CONST_NODE pNode1, CONST_NODE pNode2);

	enum enumAssignMode
	{
		ASSIGN_SELF = 0X10000000,
		ASSIGN_CHILDREN = 0X20000000,

		ASSIGN_NODE_EXISTING = 0X00000001,
		ASSIGN_NODE_NOTEXISTING = 0X00000002,
		ASSIGN_NODE_ALL = 0X00000003,

		ASSIGN_PROPERTY_EXISTING = 0X00010000,
		ASSIGN_PROPERTY_NOTEXISTING = 0X00020000,
		ASSIGN_PROPERTY_ALL = 0X00030000,
	};
	static void static_Assign(NODE pNode1, CONST_NODE pNode2, DWORD dwMode = ASSIGN_CHILDREN | ASSIGN_NODE_ALL | ASSIGN_PROPERTY_ALL);

public:
	enum enumType
	{
		ADT_CHAR,
		ADT_STRING,
		ADT_INT8,
		ADT_UINT8,
		ADT_INT16,
		ADT_UINT16,
		ADT_INT32,
		ADT_UINT32,
		ADT_INT64,
		ADT_UINT64,
		ADT_FLOAT32,
		ADT_FLOAT = ADT_FLOAT32,
		ADT_FLOAT64,
		ADT_DOUBLE = ADT_FLOAT64,
		ADT_TIME,
		ADT_DATE,
		ADT_DATETIME,
		ADT_OBJECT_TABLE,	//CIADataTable
		ADT_OBJECT_CURVE2D,	//CIADataCurve2D
		ADT_ICON,
		ADT_OBJECT,			//CAnalysisData
		ADT_UNKNOWN,
	};
	enum enumFlag
	{
		//GETTING��SETTING�� MODE��FORMAT�������
		ADTF_VALIDATEONGETTING_SAME			= 0x00000001,//��ȡʱ��֤���ͣ�����Ҫ����ȫ����ƥ�䣬�磬��long��ȡint��short��unsigned long�ͻ�ʧ��
		ADTF_VALIDATEONGETTING_COMPATIBLE	= 0x00000002,//��ȡʱ��֤���ͣ�ʹ�ü�������ƥ�䣬�磬��long��ȡint��short��unsigned long��Ȼ��ɹ�
		ADTF_VALIDATEONGETTING_DISABLED		= 0x00000003,//��ȡʱ����֤����

		ADTF_VALIDATEONSETTING_SAME			= 0x00010000,
		ADTF_VALIDATEONSETTING_COMPATIBLE	= 0x00020000,
		ADTF_VALIDATEONSETTING_DISABLED		= 0x00030000,

		ADTF_MODEONSETTING_CREATE			= 0x01000000,//��Setʱ������Ӧ�����Ͳ����ڣ��򴴽�����δָ�������룬��ʧ�ܣ�

		ADTF_VALIDATE_DISABLED				= ADTF_VALIDATEONGETTING_DISABLED | ADTF_VALIDATEONSETTING_DISABLED,

		ADTF_MASK_GETTING		= 0x0000FFFF,
		ADTF_MASK_SETTING		= 0x00FF0000,

		
		ADTF_USINGFORMATONGETTING		= 0x02000000,
		ADTF_USINGFORMATONSETTING		= 0x04000000,
	};
	static LPCTSTR static_TypeString(CAnalysisData::enumType eDataType);
	static CAnalysisData::enumType static_Type(const CString& typeString);
	static CAnalysisData::enumType static_Type(CONST_NODE pInfoNode);
	static bool static_Format(CONST_NODE pInfoNode, CString& strFormat);
	static bool static_CanExtendTo(CAnalysisData::enumType eDataType1, CAnalysisData::enumType eDataType2);//eDataType1�Ƿ���eDataType2�ļ������ͣ�eDataType1�ķ�Χ�Ƿ�С��eDataType2�����磬ADT_INT8��ADT_INT16�ļ������ͣ�������������
	//ע�⣬�������������ڸ�����
	
public:
	static CString static_XMLNodeNameFromPath(const CString& xmlPath);

public:
	CAnalysisData();
	virtual ~CAnalysisData();

private://��ֱֹ�ӿ�����ʹ��GetXMLString��SetXMLString�ȴ���
	CAnalysisData& operator = (const CAnalysisData &cad);
	CAnalysisData(const CAnalysisData &cad);

public:
	DOCUMENT GetDocument();
	CONST_DOCUMENT GetDocument() const;
	NODE GetRoot();
	CONST_NODE GetRoot() const;

public:
	int Create(const CString& xmlPath);//���tagΪ�գ���ʹ�ó�ʼ·���е�����;
	int LoadFromDocument(LPCTSTR strXMLString);
	int LoadFromFile(LPCTSTR strXMLFile);
	int SaveToFile(LPCTSTR strXMLFile) const;
	
public:
	CString GetXMLString() const;
	int SetXMLString(LPCTSTR strXMLString);

	int GetOuterXML(LPCTSTR strXMLPath, CString& strXMLString) const;
	int SetOuterXML(LPCTSTR strXMLPath, LPCTSTR strXMLString);
	int GetInnerXML(LPCTSTR strXMLPath, CString& strXMLString) const;
	int SetInnerXML(LPCTSTR strXMLPath, LPCTSTR strXMLString);

	int GetValue(LPCTSTR strXMLPath, CString &strValue) const;
	int SetValue(LPCTSTR strXMLPath, LPCTSTR strValue);

	int GetProperty(LPCTSTR strXMLPath, LPCTSTR strProperty, CString &strValue) const;
	int SetProperty(LPCTSTR strXMLPath, LPCTSTR strProperty, LPCTSTR strValue);

public:
	int	GetValue_XMLString(LPCTSTR strXMLPath, CString &value) const;//�õ���·���µ�XMLԭʼ�ִ�,�൱��GetOuterXML
	int	SetValue_XMLString(LPCTSTR strXMLPath, LPCTSTR value);//�൱��SetOuterXML

	// ������Щ�������������ü���ȡ�Զ���������
	// ����������Ͳ�ƥ�䣬����IAERROR_XML_NOTVALID
	int	GetValue_Double(LPCTSTR strXMLPath, double &value) const;
	int	GetValue_Int(LPCTSTR strXMLPath, int &value) const;
	int	GetValue_String(LPCTSTR strXMLPath, CString &value) const;
	int	GetValue_Time(LPCTSTR strXMLPath, CTime &value) const;
	int	GetValue_Time(LPCTSTR strXMLPath, COleDateTime &value) const;
	int	GetValue_Table(LPCTSTR strXMLPath, CIADataTable &value) const;
	int	GetValue_Curve2D(LPCTSTR strXMLPath, CIADataCurve2D &value) const;
	int GetValue_AnalysisData(LPCTSTR strXMLPath, CAnalysisData &value) const;

	int	SetValue_Double(LPCTSTR strXMLPath, double value);
	int	SetValue_Int(LPCTSTR strXMLPath, int value);
	int	SetValue_String(LPCTSTR strXMLPath, LPCTSTR value);
	int	SetValue_Time(LPCTSTR strXMLPath, const CTime &value);
	int	SetValue_Time(LPCTSTR strXMLPath, const COleDateTime &value);
	int	SetValue_Table(LPCTSTR strXMLPath, const CIADataTable &value);//�൱��SetOuterXML(..., value.GetXMLString())
	int	SetValue_Table2(LPCTSTR strXMLPath, const CIADataTable &value);
	int	SetValue_Curve2D(LPCTSTR strXMLPath, const CIADataCurve2D &value);//�൱��SetOuterXML(..., value.GetXMLString())
	int SetValue_AnalysisData(LPCTSTR strXMLPath, const CAnalysisData &value);//�൱��SetOuterXML(..., value.GetXMLString())
	

	int	GetDefaultValue_Double(LPCTSTR strXMLPath, double &value) const;
	int	GetDefaultValue_Int(LPCTSTR strXMLPath, int &value) const;
	int	GetDefaultValue_String(LPCTSTR strXMLPath, CString &value) const;
	int	GetDefaultValue_Time(LPCTSTR strXMLPath, CTime &value) const;
	int	GetDefaultValue_Time(LPCTSTR strXMLPath, COleDateTime &value) const;

	int	SetDefaultValue_Double(LPCTSTR strXMLPath, double value);
	int	SetDefaultValue_Int(LPCTSTR strXMLPath, int value);
	int	SetDefaultValue_String(LPCTSTR strXMLPath, LPCTSTR value);
	int	SetDefaultValue_Time(LPCTSTR strXMLPath, const CTime &value);
	int	SetDefaultValue_Time(LPCTSTR strXMLPath, const COleDateTime &value);

public:
	enum enumCompareResult
	{
		CR_XMLPATH_NOT_EXIST_THIS,
		CR_XMLPATH_NOT_EXIST_PASSED,
		CR_XMLPATH_NOT_EXIST_THIS_AND_PASSED,
		CR_EQUAL,
		CR_GREATER,
		CR_LESS,
	};
	//���strXMLPathPassedΪ�գ���ʹ��strXMLPath
	enumCompareResult CompareValue_Double(LPCTSTR strXMLPath, const CAnalysisData& adPassed, LPCTSTR strXMLPathPassed = NULL) const;
	enumCompareResult CompareValue_Int(LPCTSTR strXMLPath, const CAnalysisData& adPassed, LPCTSTR strXMLPathPassed = NULL) const;
	enumCompareResult CompareValue_String(LPCTSTR strXMLPath, const CAnalysisData& adPassed, LPCTSTR strXMLPathPassed = NULL, bool bCaseSensitive = true) const;

public:
	void SetFlags(DWORD dwFlags){m_dwDataTypeMask = dwFlags;}
	DWORD GetFlags() const{return m_dwDataTypeMask;}
	bool SetFormat(LPCTSTR strXMLPath, LPCTSTR strFormat);

	//��xml�е�ָ����ʽ���ַ���ת��Ϊ�����ֵ
	bool FromFormat_Double(LPCTSTR strXMLPath, const CString& vString, double& value) const;
	bool FromFormat_Int(LPCTSTR strXMLPath, const CString& vString, int& value) const;
	bool FromFormat_String(LPCTSTR strXMLPath, const CString& vString, CString& value) const;
	bool FromFormat_Time(LPCTSTR strXMLPath, const CString& vString, CTime &value) const;
	bool FromFormat_Time(LPCTSTR strXMLPath, const CString& vString, COleDateTime &value) const;

	//��ֵת��Ϊָ��xml�е�ָ����ʽ���ַ���
	CString	FormatValue_Double(LPCTSTR strXMLPath, double value);
	CString	FormatValue_Int(LPCTSTR strXMLPath, int value);
	CString	FormatValue_String(LPCTSTR strXMLPath, LPCTSTR value);
	CString	FormatValue_Time(LPCTSTR strXMLPath, const CTime &value);
	CString	FormatValue_Time(LPCTSTR strXMLPath, const COleDateTime &value);

public:
	bool IsEqual(const CAnalysisData& ad) const;

protected:
	virtual void OnLoadedFromDocument(){}
	virtual void OnLoadedFromFile(LPCTSTR strXMLFile){}
	virtual bool CanGet(CAnalysisData::CONST_NODE pSrcNode, CAnalysisData::enumType eGettingType) const;//eRequiredType���û���Ҫ��ȡ�������õ����ͣ�����������ȡ�������ã�����false
	virtual bool CanSet(CAnalysisData::NODE pSrcNode, CAnalysisData::enumType eSettingType);
	virtual CAnalysisData::CONST_NODE TypeNodeOnGetting(CAnalysisData::CONST_NODE pNode) const{return pNode;}//ָ��pNode�Ӻδ���ȡ������Ϣ
	virtual CAnalysisData::CONST_NODE FormatNodeOnGetting(CAnalysisData::CONST_NODE pNode) const{return pNode;}//ָ��pNode�Ӻδ���ȡ��ʽ��Ϣ
	virtual CAnalysisData::NODE TypeNodeOnSetting(CAnalysisData::NODE pNode){return pNode;}//ָ��pNode�Ӻδ�����������Ϣ
	virtual CAnalysisData::NODE FormatNodeOnSetting(CAnalysisData::NODE pNode){return pNode;}//ָ��pNode�Ӻδ����ø�ʽ��Ϣ

private:
	int _LoadFromDocument(LPCTSTR strXMLString, bool bFireEvent);

	CString TypeOnGetting(LPCTSTR strXMLPath) const;
	CString TypeOnSetting(LPCTSTR strXMLPath);
	bool SetType(LPCTSTR strXMLPath, CAnalysisData::enumType eDataType);
	CString FormatOnGetting(LPCTSTR strXMLPath) const;
	CString FormatOnSetting(LPCTSTR strXMLPath);

protected:
	PARSER m_pParser;
	DWORD m_dwDataTypeMask;//Ϊ����ǰ���ּ��ݣ�Ĭ��Ϊ������
};

////////////////////////////////////////////////////////////////////////////////////
// CIADataTable
////////////////////////////////////////////////////////////////////////////////////
//Title:����IATable��xml��������ʱ���ɱ�������֧�ֶ��б��⣬����Ч���ο�IATable
 

//strValue1 > strValue2:return 1;
//strValue1 < strValue2:return -1;
//strValue1 = strValue2:return 0;
typedef int (*CIADATATABLE_SORTCALLBACK)(LPCTSTR strColumn, LPCTSTR strValue1, LPCTSTR strValue2);

class CIADataTable
{
protected:
	static int Default_SortCallback(LPCTSTR strColumn, LPCTSTR strValue1, LPCTSTR strValue2);

public:
	CIADataTable();
	~CIADataTable();

public:
	int Create(const CString& xmlPath);
	CString GetXMLString() const;
	int SetXMLString(LPCTSTR strXMLString);

public:
	//���ݵ�ǰ��������Ϣ����ע�⣬�������ñ�����ʱ�Ż�����
	//���cbΪ�գ���ʹ��Ĭ�ϵ�����ʽ(string)
	bool Sort(CIADATATABLE_SORTCALLBACK cb = NULL);

public:
	//strTable:Ԫ������
	bool GetTable(CString* pstrTable, DWORD* pdwOrders) const;
	bool SetTable(DWORD dwOrders);
	bool GetSortInfo(DWORD dwIndex, CString& strOrderBy, bool& bAsc) const;
	bool SetSortInfo(DWORD dwIndex, LPCTSTR strOrderBy, bool bAsc);

	//Topic�ı��������Ϊxml��������
	bool GetColumns(CStringArray* pArrTopics, CStringArray* pArrTitles = NULL) const;
	bool SetColumns(const CStringArray& arrTopics, const CStringArray* pArrTitles = NULL);
	bool GetColumn(long iTopic, CString* pstrTopic, CString* pstrTitle = NULL) const;
	bool SetColumn(long iTopic, LPCTSTR strTopic, LPCTSTR strTitle = NULL);//�����Ѿ����ڵı���
	bool SetColumn(LPCTSTR strOldTopic, LPCTSTR strNewTopic, LPCTSTR strTitle = NULL);//�����Ѿ����ڵı���
	bool GetColumnInfo(long iTopic, LPCTSTR propertyName, CString& propertyValue) const;
	bool SetColumnInfo(long iTopic, LPCTSTR propertyName, LPCTSTR propertyValue);
	bool GetColumnInfo(LPCTSTR strTopic, LPCTSTR propertyName, CString& propertyValue) const;
	bool SetColumnInfo(LPCTSTR strTopic, LPCTSTR propertyName, LPCTSTR propertyValue);
	bool GetColumnInfo(long iTopic, const CStringArray& propertiesNames, CStringArray& propertiesValues) const;
	bool SetColumnInfo(long iTopic, const CStringArray& propertiesNames, const CStringArray& propertiesValues);
	bool GetColumnInfo(LPCTSTR strTopic, const CStringArray& propertiesNames, CStringArray& propertiesValues) const;
	bool SetColumnInfo(LPCTSTR strTopic, const CStringArray& propertiesNames, const CStringArray& propertiesValues);

	long AppendColumn(LPCTSTR strTopic, LPCTSTR strTitle = NULL);
	long InsertColumn(int iTopic, LPCTSTR strTopic, LPCTSTR strTitle = NULL);

	long GetColumnNumber() const;
	bool DeleteColumn(long iTopic);
	bool DeleteColumn(LPCTSTR strTopic);
	bool DeleteAllColumn();
	long GetColumnIndex(LPCTSTR strTopic) const;

	long AppendRow();
	long InsertRow(int iLine);
	long AppendRow(const CStringArray &arrayString);
	long InsertRow(int iLine, const CStringArray &arrayString);

	long GetRowsNumber() const;
	bool DeleteRow(long iLine);
	bool DeleteAllRows();

	bool GetRowValues(long iLine, CStringArray& values) const;	
	bool SetRowValues(long iLine, const CStringArray& values);

	bool GetColumnValues(long iColumn, CStringArray& values) const;
	bool GetColumnValues(LPCTSTR strColumn, CStringArray& values) const;
	bool SetColumnValues(long iColumn, const CStringArray& values);
	bool SetColumnValues(LPCTSTR strColumn, const CStringArray& values);

	bool GetCellValue(long iLine, long iColumn, CString& colValue) const;
	bool GetCellValue(long iLine, LPCTSTR strColumn, CString& colValue) const;
	bool SetCellValue(long iLine, long iColumn, LPCTSTR colValue);
	bool SetCellValue(long iLine, LPCTSTR strColumn, LPCTSTR colValue);
	
protected:
	CAnalysisData::NODE GetColumn(long iTopic) const;
	CAnalysisData::NODE GetColumn(LPCTSTR strTopic) const;
	bool GetColumn(CAnalysisData::CONST_NODE pTopic, CString* pstrTopic, CString* pstrTitle) const;
	bool SetColumn(CAnalysisData::NODE pTopic, LPCTSTR strTopic, LPCTSTR strTitle);
	bool _AppendColumn(LPCTSTR strTopic, LPCTSTR strTitle);
	CAnalysisData::NODE GetRow(long iLine) const;

protected:
	CAnalysisData m_oTable;
	friend class CAnalysisData;
};

////////////////////////////////////////////////////////////////////////////////////
// CIATable
////////////////////////////////////////////////////////////////////////////////////
// �������ɹ������Ȼ�ȡ��������
//				�ϲ��ٽ���ͬ�ı������ӵĿձ���
//��������Titles��<1,2,3,4,5><c1><r1,c2><r1,r2,c3><r1,r2,c4>
/*
---------------------------------------------------------------------------
| 1 |    |      r1      |
|---|    |--------------|--------------------------------------------------
| 2 |    | c2 |   r2    |
|---|    |    |---------|--------------------------------------------------
| 3 | c1 |    | c3 | c4 |
|---|    |    |    |    |--------------------------------------------------
| 4 |    |    |    |    |
|---|    |    |    |    |--------------------------------------------------
| 5 |    |    |    |    |
---------------------------------------------------------------------------
*/

#define TABLE_BITMAP_DELETE			_T("{[(BITMAP_PREDEF:_Action_Delete.bmp)]}")
#define TABLE_BITMAP_INF			_T("{[(BITMAP_PREDEF:_Action_Info.bmp)]}")
#define TABLE_BITMAP_ADD			_T("{[(BITMAP_PREDEF:_Action_Add.bmp)]}")
#define TABLE_BITMAP_ERROR			_T("{[(BITMAP_PREDEF:_Action_Error.bmp)]}")
#define TABLE_BITMAP_MANUAL_GO		_T("{[(BITMAP_PREDEF:_Action_ManualGO.bmp)]}")
#define TABLE_BITMAP_MANUAL_NEXT	_T("{[(BITMAP_PREDEF:_Action_ManualGONext.bmp)]}")
#define TABLE_BITMAP_MANUAL_OK		_T("{[(BITMAP_PREDEF:_Action_ManualOk.bmp)]}")
#define TABLE_BITMAP_AUTO_GO		_T("{[(BITMAP_PREDEF:_Action_AutoGO.bmp)]}")
#define TABLE_BITMAP_AUTO_NEXT		_T("{[(BITMAP_PREDEF:_Action_AutoGONext.bmp)]}")
#define TABLE_BITMAP_AUTO_OK		_T("{[(BITMAP_PREDEF:_Action_AutoOk.bmp)]}")
#define TABLE_BITMAP_RUNNING		_T("{[(BITMAP_PREDEF:_Action_Running.bmp)]}")
#define TABLE_BITMAP_PAUSE			_T("{[(BITMAP_PREDEF:_Action_Pause.bmp)]}")
#define TABLE_BITMAP_STOP			_T("{[(BITMAP_PREDEF:_Action_Stop.bmp)]}")
//��ԴID								_T("{[(STRING_RESID:1001)]}")
//��ԴID								_T("{[(BITMAP_RESID:1001)]}")
//��ԴID								_T("{[(ICON_RESID:1001)]}")
//��ԴID								_T("{[(CURSOR_RESID:1001)]}")
//�ļ�								_T("{[(BITMAP_FILE: ·��)]}")//·�������Ǿ���·����Ҳ����ʹ��ռλ��
//·��ռλ����[AppFolder],[...����]
//�ļ�								_T("{[(ICON_FILE: ·��)]}")
//�ļ�								_T("{[(CURSOR_FILE: ·��)]}")
//data								_T("{[(BITMAP_DATA: ����)]}")
//data								_T("{[(ICON_DATA: ����)]}")
//data								_T("{[(CURSOR_DATA: ����)]}")

class CIATable: public CUGCtrl
{
	static CString begin;
	static CString end;
	static CString bitmap_predef;

	static CString static_ValueFromFullTag(LPCTSTR tag);

public:
	enum enumBitmapIndex
	{
		BITMAP_NULL = 0,	//��
		BITMAP_ADD,			//���
		BITMAP_DELETE,		//ɾ��
		BITMAP_INF,			//��Ϣ
		BITMAP_MANUAL_GO,	//�ֹ�
		BITMAP_MANUAL_NEXT,	//׼��ִ��
		BITMAP_MANUAL_OK,	//���
		BITMAP_AUTO_GO,		//�Զ�
		BITMAP_AUTO_NEXT,	//�Զ�
		BITMAP_AUTO_OK,		//�Զ�
		BITMAP_RUNNING,		//����ִ��
		BITMAP_ERROR,		//����
		BITMAP_PAUSE,
		BITMAP_STOP,

		BITMAP_COUNT,
	};

public:
	CIATable();
	virtual ~CIATable(void){}

public:
	int GetBitmapIndex(enumBitmapIndex eBitmap) const;
	CString GetBitmapFile(enumBitmapIndex eBitmap) const;
	LPCTSTR GetBitmapFlag(enumBitmapIndex eBitmap) const;
	const CBitmap* GetBitmapObject(enumBitmapIndex eBitmap) const;

	void SetAutoAdd_AddRow(bool bAuto){m_bAutoAdd_RowButtonAdd = bAuto;}//��һ���Ա���� "����"��ť

	BOOL CreateTable(CWnd *pcParent, CRect rect);
	BOOL SetTitle(int nCol, LPCTSTR strTitleName, int nColWidth, int nTitleNO = -1);
	void AppendRow_ButtonAdd();
	void RomoveRow_ButtonAdd();
	int GetTH_NumberRows() const{return m_GI?m_GI->m_numberTopHdgRows:-1;}
	
	LPCTSTR GetXMLBitmapFlag(int iCol, int iRow) const;
	long GetBitmapIndexFromXMLFlag(const CString& flag) const;
	void UnJoinTitles();
	void DeleteAllItems();

	int		InsertRow(long row);
	int		AppendRow();
	virtual const CIADataTable& GetNewXMLData(LPCTSTR strXMLPath);//���������ת��ΪXML,ע�⣬����ID��Title���п�֮����κ�Topic���ԣ���ֵ���ͣ�ֵ��ʽ��ֵ���ƣ�ֵ��λ�ȣ�����ʧ
	virtual const CIADataTable& UpdateXMLData();//���������ת��ΪXML,���ݱ���͵�ǰ�ı��ⶨ����ƥ��
	virtual int SetXMLData(const CIADataTable &xmlData);//��XML����д����

	enum enumResourceType
	{
		STRING,
		BITMAP_PREDEF,

		//���������ݲ�֧��
// 		STRING_RESID,
// 		BITMAP_RESID,
// 		ICON_RESID,
// 		CURSOR_RESID,
// 
// 		BITMAP_FILE,
// 		ICON_FILE,
// 		CURSOR_FILE,
	};
	static CString XMLData_ParseToText(const CString& cellValueFromXml);
	static void XMLData_Parse(const CString& cellValueFromXml, CStringArray& arrContent, CArray<enumResourceType, enumResourceType>& arrType);
	static CString XMLData_ToText(const CStringArray& arrContent, const CArray<enumResourceType, enumResourceType>& arrType);

protected:
	virtual void OnSetup();
	virtual void OnSettedXMLLine(int iRow){}//SetXMLData����
	virtual void OnSettedXMLData(){}//SetXMLData����
	int UpdateUI_ByLocalXMLData();
	void XMLData_ApplyCellContent(int col, int row, const CString& cellContent);
	
protected:
	virtual void SetAppDataToTable() = 0;
	virtual void SetTableToAppData() = 0;

	//������Ԫ����õĶ���
	virtual void Action_AddRow()=0;
	virtual void Action_DeleteRow(int nRow) = 0;
	virtual void Action_DisplayRowInf(int nRow) = 0;
	//	virtual void Action_RunRow(int nRow) = 0;

	virtual void Action_Manual_Finished(int nRow, int nColumn){/*AfxMessageBox(_T("�ֹ���������ɣ���ʾ��ɺ��������ۡ��û���ѡ������ִ�в�����")); */}
	virtual void Action_Manual_RunNext(int nRow, int nColumn){AfxMessageBox(_T("�û��������һ��ִ�еİ�ť����ʵ�ָöδ��룡"));}
	virtual void Action_Manual_Run(int nRow, int nColumn){/*AfxMessageBox(_T("�û������ִ�еİ�ť����û�а�˳��ִ�У���ʵ�ָöδ��룡\n\nԭ����Action_RunRow����"));*/}

	virtual void Action_Auto_Finished(int nRow, int nColumn){/*AfxMessageBox(_T("�Զ���������ɣ���ʾ��ɺ��������ۡ�"));*/}
	virtual void Action_Auto_Run(int nRow, int nColumn){AfxMessageBox(_T("�������Զ�ִ�У������˹�������"));}
	virtual void Action_Auto_RunNext(int nRow, int nColumn){AfxMessageBox(_T("�������Զ�ִ�У������˹�������"));}

	virtual void Action_Running(int nRow, int nColumn){AfxMessageBox(_T("�û�����ĸ�������ִ�в�����"));}
	virtual void Action_Error(int nRow, int nColumn){AfxMessageBox(_T("�û�����ĸó�����ʾ������Ϣ��"));}

	//��������ͨ�ò�������
	virtual void OnLClicked(int col,long row, int updn, RECT *rect, POINT *point, int processed);
	virtual  int OnCellTypeNotify(long ID, int col,long row, long msg, long param); 

	//�ı�ѡ���б�����ɫ
	virtual void ChangeRowBackGroundColor(long colNum, long rowNum, long currRow);
	virtual void ChangeSelectedBKColor(long colNum, long rowNum);
	virtual void ChangeCurrentBKColor(long colNum, long currRow);

	virtual int  OnCanSizeCol(int col);

public:
	CFont* IA_GetTitleFont(){return GetFont(m_nFontID_Title);}
	CFont* IA_GetContentFont(){return GetFont(m_nFontID_Content);}

protected:
	int		m_nFontID_Title;
	int		m_nFontID_Content;			//����е���������
	COLORREF m_colorContent;
	COLORREF m_colorBackGround;
	bool	m_DislplayFirstColumn;
	bool	m_bAutoAdd_RowButtonAdd;	//��ÿ��RedrawAll()��ʱ���Զ����롡������
	int		m_nAdditionRow;				//���ӵĹ���������(�����),������������ʱ��Ҫ�۵�
	int		m_pBitmaps[BITMAP_COUNT];
	CString ErrMsg;
	CIADataTable	m_dataTable;
	bool	m_bIsSettingup;

private:
	void ApplyIADataTable_CellProperties(long row);
};
 
/*
class CIADataCurve3D
{

};*/

//���ݱ����ʽ
/*
<Sample>
	<Result>
		<CurveDatas>
			<CaptionDefine>
				<ResultPoints>�����������</ResultPoints>
			</CaptionDefine>

			<CurveData Type='ResultPoints' count="10" m_dwSecondsPrev="0" nDrawType="1">
				<Item index="6" name="H2O">n002_H2O.csv</Item>
				<Item index="7" name="N2">n002_N2.csv</Item>
				<Item index="8" name="O2">n002_O2.csv</Item>
				<Item index="9" name="CO2">n002_CO2.csv</Item>
			</CurveData>


			<CurveData Caption="��������������" Type='M'  count="10" m_dwSecondsPrev="0" nDrawType="1">
				<Item index="2" name="M18" type="M">n002_M18.csv</Item>
				<Item index="3" name="M28" type="M">n002_M28.csv</Item>
				<Item index="4" name="M32" type="M">n002_M32.csv</Item>
				<Item index="5" name="M44" type="M">n002_M44.csv</Item>
			</CurveData>


			<CurveData Caption="����ǿ����������" Type='FL'  count="10" m_dwSecondsPrev="0" nDrawType="1">
				<Item index="0" name="FL" type="FL">n002_FL.csv</Item>
			</CurveData>


			<CurveData Caption="������ǿ����������" Type='I_Total'  count="10" m_dwSecondsPrev="0" nDrawType="1">
				<Item index="1" name="I_Total" type="I_Total">n002_I_Total.csv</Item>
			</CurveData>
		</CurveDatas>
	</Result>
</Sample>

rangeMin,rangeMax:��Ʒ���ߵ���ʾ��Χ
//*/
class CIADataCurve2DSingle;
class CIADataCurve2D : CObject// : public CIADataXML
	//���ṩCurve2D��xml����
{
public:
	static LPCTSTR DefaultXML(){return _T("<CurveDatas><CaptionDefine></CaptionDefine></CurveDatas>");}
	static LPCTSTR Path_CaptionDefine(){return _T("CurveDatas/CaptionDefine");}

public:
	CIADataCurve2D(LPCTSTR cStrXMLPath = _T("")):m_cStrXMLPath(cStrXMLPath),m_bInit(false){SetXMLString(DefaultXML());}
	virtual ~CIADataCurve2D(){}

public:
	CString m_cStrXMLPath;	//XML·��
	int SetXMLPath(const CString &cStrXMLPath){m_cStrXMLPath=cStrXMLPath; return IAERROR_SUCCESS;}

public:
	int SetXMLString(LPCTSTR cXMLString);//���Ҫ�½�������cXMLStringΪNULL����"<CurveDatas></CurveDatas>"
	CString GetXMLString() const;

	//CString GetProperty(LPCTSTR prop);
	//bool SetProperty(LPCTSTR prop, LPCTSTR val);

	CString GetCurveCaption(LPCTSTR type) const;
	void SetCurveCaption(LPCTSTR type, LPCTSTR caption);//����ȷ��Path_CaptionDefine()��·���Ѿ�����
	
	bool GetCurve2DSingle(LPCTSTR type, CIADataCurve2DSingle& single) const;
	bool AutoCurve2DSingle(LPCTSTR type, CIADataCurve2DSingle& single);

	void BeginEnum();//����ʹ��const�汾
	bool GetNext(CString& type, CIADataCurve2DSingle& single);//����ʹ��const�汾
	
	void BeginEnum(long& pos) const;
	bool GetNext(long& pos, CString& type, CIADataCurve2DSingle& single) const;

	bool QuickGetCurve2DFile(LPCTSTR type, LPCTSTR name, CString& filename) const;
	bool QuickSetCurve2DFile(LPCTSTR type, LPCTSTR name, LPCTSTR filename);
	bool QuickDeleteCurve2DFile(LPCTSTR type, LPCTSTR name);

public:
	bool m_bInit;

protected:
	CTWenXMLBase xml;
	int m_iEnum;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// CIADataCurve2DSingle
// ���ܣ���������������Ϣ������ʽ��
/*
<CurveData Type='ResultPoints' count="10" m_dwSecondsPrev="0" nDrawType="1">
	<Item index="6" name="H2O">n002_H2O.csv</Item>
	<Item index="7" name="N2">n002_N2.csv</Item>
	<Item index="8" name="O2">n002_O2.csv</Item>
	<Item index="9" name="CO2">n002_CO2.csv</Item>
</CurveData>
//*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
class CIADataCurve2DSingle : CObject// : public CIADataXML
	//���ṩCurve2D��xml����
{
public:
	CIADataCurve2DSingle():m_pSingle(NULL){}

public:
	bool ItemExist(LPCTSTR name);

	bool GetItemByName(LPCTSTR name, CString& fileName);

	void SetItemByName(LPCTSTR name, LPCTSTR fileName);

	void AppendItem(LPCTSTR name, LPCTSTR fileName);

	bool DeleteItemByName(LPCTSTR name);

	bool GetProperty(LPCTSTR prop, CString& val);
	bool SetProperty(LPCTSTR prop, LPCTSTR val);

	void BeginEnum();
	bool GetNext(CString& name, CString& fileName);
	bool SetEnum(LPCTSTR fileName);
	bool GetEnumProperty(LPCTSTR prop, CString& val) const;
	bool SetEnumProperty(LPCTSTR prop, LPCTSTR val);

	int GetCount() const{return m_pSingle?m_pSingle->Children.GetCount():-1;}

protected:
	static CString strType;
	CTWenXMLSingle* m_pSingle;
	friend class CIADataCurve2D;
	CTWenXMLSingle* GetItem(LPCTSTR name) const;
	int m_iEnum;
	CTWenXMLSingle* m_pEnumSingle;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// CIAFileCurve2D
// ���ܣ������ݴ���
// 1.�������ݣ�Open();AppendPoint()
// 2.ö�����е㣺Open();BeginEnum();while(GetNext()) ;
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CIAFileCurve2D
	//���ṩCurve2D�������ļ�����
{
public:
	CIAFileCurve2D():m_pcChars(NULL){m_file.m_hFile = CFile::hFileNull;m_Ext.SetSplitString(_T('\n'));}
	~CIAFileCurve2D(){Close();}
public:
	BOOL Open(LPCTSTR file);//�����ļ�ȫ·������һ����csvΪ��׺����csv��ʽ���ļ�

	BOOL ApppendPoint(double x, double y);//x,y�����ڵ�������

	void BeginEnum();
	bool GetNext(double& x, double& y, CTime& time);//timeĿǰ��Ч

	void Close();


	CStringExt m_Ext;

protected:
	CFile m_file;
	static char strSep;//csv�ָ��ַ�

	static bool Read(CFile& file,CString& content,char token);
	char* m_pcChars;
};
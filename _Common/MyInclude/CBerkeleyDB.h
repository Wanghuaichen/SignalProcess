#pragma once

#include "dbxml/DbXml.hpp"
#include "KMDB_Error.h"

using namespace DbXml;

#define MAX_ALIASNAME		50

#ifndef KMDB_ITEMP_ID
	#define KMDB_ITEMP_ID			_T("id")
#endif
#ifndef KMDB_ID_SECURITY_INF
	#define KMDB_ID_SECURITY_INF	_T("_SECURITY_INF_")	//��ȫ��Ϣ
#endif


void DbException_DisplayError(DbException  &e,int nLine);
void XmlException_DisplayError(XmlException &e,int nLine);
void BDBErrorCallback(const DbEnv *dbenv, const char *errpfx, const char *msg);



class __declspec(dllexport)  COneLevelXMLData
{
public:
	COneLevelXMLData(void):	m_cStrXML(_T("<Item></Item>")){}
	virtual ~COneLevelXMLData(void){}


//	void InitWithDefaultData(void){m_cStrXML = KMDB_DEFAULT_FILEXMLDATA;}
//	void InitWithCurrencyData(void){m_cStrXML = KMDB_CURRENCY_FILEXMLDATA;}

	//֧��ѹ������ǰ���ڴ��XML���ݣ��������ݴ�����ѹ������������
	CString m_cStrXML;

	//
	//�����滻����XML����
	void operator =(const COneLevelXMLData &other){m_cStrXML=other.m_cStrXML;}
	BOOL ReplaceWith(const TCHAR* strXML);	//����strXML���ݸ�ʽ���Ϸ�������FALSE
	void Clear(void){COneLevelXMLData empty;*this=empty;};	//�����������
	//
	//-----------���º���������ӡ��޸ġ�ɾ��XML�ĸ�������---------------------------------------
	//
	//���²��ֻ�ȫ�����ݡ�������������д��ڵ��������������ݣ�û�е�����ӵ�����������С�
	virtual void Data_Update(COneLevelXMLData &other);
	virtual BOOL Data_Update(const TCHAR* strXML);	//����strXML���ݸ�ʽ���Ϸ�������FALSE.strXML�������<Item><\Item>
	//
	//���õ������ֵ�������ڵ��������ӣ����ڵĽ����滻��
	virtual int Data_Item_Set(const TCHAR* strName,const TCHAR* strProperty, const TCHAR* strValue);
	//ɾ��ĳһ�����һ�������ж�������
	BOOL Data_Item_Del(const TCHAR* strName);

	//�ú��������û�����Ե������ã�֧�ֶ����ͬʱ���ڡ�
	int Data_MultiItem_Set(CString cStrName, CString cStrValue);
	int Data_MultiItem_Set(CString cStrName, const CStringArray &arrayProperty, CString cStrValue);


	//------------------------------------------------------------------------------
	//
	//-----------���º������ڻ��XML�ĸ�������---------------------------------------
	//
	//
	//��������XML����
	BOOL GetXMLString(TCHAR* &strXML);		//�����߸����ͷ�ָ��
	void GetXMLString(CString &cStrXML){cStrXML=m_cStrXML;}
	CString GetXMLString(void){return m_cStrXML;}

	//�õ�������Ŀ��ֵ
	/*
	*   ���������ڣ���ʱ��,����ֵarrayRes�϶����ڣ���ֻ�Ǹ���arrayRes[i]����Ϊ_T("")��
	*	0����ʾ�ɹ�
	*	3-- ����XML��ItemXML��ʽ����
	*	15��δ֪����
	*	16����XMLԪ�ز�����
	*	17����XML���Բ�����
	*	18��ֵΪ��
	*/
	//����ֵ�����arrayRes��������
	int GetData(const wchar_t* strName, const wchar_t* strProperty, CStringArray &arrayRes) const;
	CString GetData(const wchar_t* strName, const wchar_t* strProperty) const;


	//�����ļ��ı��
	CString GetItemID(void) const; 


	/*�����ļ����
	*	0-�ɹ�
	*	-1 XML ���Ϸ�
	*	-2 XML ��������
	*/ 
	int SetItemID(CString id);

	//��ID=..��������ȥ��,��Ҫ��Ϊ�����˵�����AddDocument()
	void EraseItemID(void){	if(SetItemID(_T("weoqrhcniwe8432nsvbs"))==KMERROR_SUCCESS){
		CString cStrTemp = KMDB_ITEMP_ID; cStrTemp+=_T("=\"weoqrhcniwe8432nsvbs\"");
		VERIFY(m_cStrXML.Replace(cStrTemp, _T(""))!=0);
	}
	}
};


typedef CArray<COneLevelXMLData, COneLevelXMLData &> ARRAY_OLXDATA;



class __declspec(dllexport) CBerkeleyDB
{
protected:
	//	static DbEnv * m_DBEnv;
	DbEnv * m_DBEnv;
	char  m_szAliasName[MAX_ALIASNAME];

public:
	CBerkeleyDB(void);
	~CBerkeleyDB(void);

protected:
	CString		m_cStrBDBName;
	XmlContainer container;
	BOOL opened;

public:
	XmlManager* myManager;
	XmlContainer *GetContainer(void){return &container;}

	virtual BOOL Init(const wchar_t * dbpath, const char *pszAlias); 

	BOOL CBerkeleyDB::OpenBDBOtherPlace(const wchar_t *dbpath, const char *pszAlias, BOOL bCreateIfNotExist);

	BOOL close();
	BOOL IsOpen(void){return opened;}

	/*	Add document to container
	*	the document should be well formated and incoding
	*/
	virtual int AddDocument(const wchar_t *str,wchar_t *& pk);

	virtual int AddDocumentByKey(const wchar_t * key,const wchar_t *str,wchar_t *& pk);

	/*
	*	Do update documents
	*	return:
	*		-1	db not opened, use init first
	*		-2	failed 
	*		>=0	the number of document counts have been modified
	*/
	virtual int DoUpdate(const wchar_t * docName,const wchar_t * node_path,const wchar_t * value);

	virtual BOOL DoUpdateDocument(const wchar_t * docName, const wchar_t * value);

	//�����ݿ�仯ʱ����
	//nAction: -1 ɾ��  0 �ı�  1 ����
	//�̳���ʹ��
	virtual void ContentChanged(const wchar_t *szKey, int nAction){}
	virtual int GetDBItemVersion(const wchar_t *szKey){ASSERT(FALSE);return KMERROR_UNKNOWN;}

	/*
	*	Get specyfied item
	*	0����ʾ�ɹ�
	*	3-- ����XML��reswchar��ʽ����
	*	7���ñ�Ų�����
	*	15-δ֪����
	*/
	virtual int GetAItem(const wchar_t *szFileID, wchar_t* &resItem);
	virtual int GetAItem(const wchar_t *szFileID, CString &resItem);
	//		int GetAItem(const wchar_t *szFileID, CString &cStrRes);
	virtual int GetAllItems( ARRAY_OLXDATA * arrayResult );

	/*
	*	Delete Speicyfied Document
	*	0-��ʾ�ɹ�
	*	����-����
	*/
	int DeleteDoc(const wchar_t * key);

	/*	doquery
	*	query should be a xquery string for search
	*	Return format as "<SearchRes><SRItem>result</SRItem></SearchRes>"
	*  to free reswchar, new need user "delete []"
	*  
	*  ���أ�0--������-1--���ݿ�δ�򿪡�����--��¼�� <-1--��¼�������Ƿ���ʧ��reswcharΪ��
	*/
	int DoQuery(const wchar_t * str,wchar_t* &reswchar);
	int DoQuery(const wchar_t *str, ARRAY_OLXDATA *arrayResult);

	/*	Search
	*	����: 0-����;	-1--���ݿ�δ�� -2---��ѯ��������Ϊ��	-3--û���ҵ�ƥ����Ŀ
	*/
	virtual int SearchDocuments(const CString& tag,const CString& searchStr, ARRAY_OLXDATA& arrayRes,BOOL blur=FALSE, BOOL bCaseInsensitive=TRUE);
	//	virtual int SearchDocumentsBySort(const CString & searchStr,ARRAY_OLXDATA& arrayRes, BOOL bIncludeSub=FALSE){return 255;}
	//	virtual int SearchDocumentsByKeyWord(const CString & searchStr,ARRAY_OLXDATA& arrayRes){return 255;}
	//	virtual int SearchDocumentsByTime(const CTime & from,const CTime & to,ARRAY_OLXDATA& arrayRes,int type=1){return 255;}


	//�ú������ڵ�һ�ν���DB������д���ʼ������
	virtual int InitNewDB(CString cStrKMDBName);

	//������׼����
//	virtual BOOL CreateDefaultIndex(BOOL bCreateIfNoe=TRUE);
	virtual BOOL CreateElementIndex(const std::string &NodeName, const std::string &method);
	int GetIndexNumber(void);
};

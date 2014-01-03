// Copyright (c) 2004, AOpen Inc.
// All rights reserved.
// This file is a part of the Application Class Library.
//
// [Filename]: DB_ADOClass.h
// [Comment ]: 
// [Author  ]: Leo Y.J. Chang
// [Version ]: R0.01
// [Release ]: 2004/02/03
// 
// [Contains]:
// [Related ]:
//
// ============================================================================
// History:
// ============================================================================
// 0.01   02/03/04  Leo     - Initial
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _INCLUDE_DB_ADOCLASS_INCLUDE_
#define _INCLUDE_DB_ADOCLASS_INCLUDE_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4239)
#pragma warning(disable: 4244)

#include <icrsint.h>

// ���� ado �� -----------------------------------------------------------
#pragma warning(disable:4146)
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" named_guids rename("EOF","adoEOF"), rename("BOF","adoBOF")
#pragma warning(default:4146)
using namespace ADODB;  

class CAdoConnection;
class CAdoRecordSet;
class CAdoCommand;
class CAdoParameter;

// ��ֵ����ת�� -----------------------------------
COleDateTime vartodate(const _variant_t& var);
COleCurrency vartocy(const _variant_t& var);
bool vartobool(const _variant_t& var);
BYTE vartoby(const _variant_t& var);
short vartoi(const _variant_t& var);
long vartol(const _variant_t& var);
double vartof(const _variant_t& var);
CString vartostr(const _variant_t& var);

/*########################################################################
------------------------------------------------
CAdoConnection class
------------------------------------------------
########################################################################*/
class CAdoConnection
{
    // ����/���� ------------------------------------------
public:
    CAdoConnection();
    virtual ~CAdoConnection();
    
protected:
    void Release();
    
    // ���� -----------------------------------------------
public:
    // ���Ӷ��� ----------------------------------
    _ConnectionPtr& GetConnection() {return m_pConnection;};
    
    // �쳣��Ϣ ----------------------------------
    CString GetLastErrorText();
    ErrorsPtr GetErrors();
    ErrorPtr  GetError(long index);
    
    // �����ִ� ----------------------------------
    CString GetConnectionText() {return m_strConnect;}
    
    // ������Ϣ ----------------------------------
    CString GetProviderName();
    CString GetVersion();
    CString GetDefaultDatabase();
    
    // ����״̬ ----------------------------------
    BOOL IsOpen();
    long GetState();
    
    // ����ģʽ ----------------------------------
    ConnectModeEnum GetMode();
    BOOL SetMode(ConnectModeEnum mode);
    
    // ����ʱ�� ----------------------------------
    long GetConnectTimeOut();
    BOOL SetConnectTimeOut(long lTime = 5);
    
    // ����Դ��Ϣ -------------------------------
    _RecordsetPtr OpenSchema(SchemaEnum QueryType);
    
    // ���� -----------------------------------------------
public:
    // ���ݿ����� --------------------------------
    BOOL Open(LPCTSTR lpszConnect =_T(""), long lOptions = adConnectUnspecified);
    BOOL ConnectSQLServer(CString dbsrc, CString dbname, CString user, CString pass, long lOptions = adConnectUnspecified);
    BOOL ConnectAccess(CString dbpath, CString pass = _T(""), long lOptions = adConnectUnspecified);
    BOOL OpenUDLFile(LPCTSTR strFileName, long lOptions = adConnectUnspecified);
    void Close();
    
    // ���� -----------------------------------------------
public:
    // ������ ----------------------------------
    long BeginTrans();
    BOOL RollbackTrans();
    BOOL CommitTrans();
    
    // ִ�� SQL ��� ------------------------------
    _RecordsetPtr Execute(LPCTSTR strSQL, long lOptions = adCmdText);
    BOOL Cancel();
    
    // ���� -----------------------------------------------
protected:
    CString			m_strConnect;
    _ConnectionPtr	m_pConnection;
};

/*########################################################################
			 ------------------------------------------------
						  CAdoRecordSet class
			 ------------------------------------------------
  ########################################################################*/

class CAdoRecordSet 
{
// ����/�۹� --------------------------------------------
public:
	CAdoRecordSet();
	CAdoRecordSet(CAdoConnection *pConnection);

	virtual ~CAdoRecordSet();
	
protected:
	void Release();

// ���� ------------------------------------------------	
public:
	// ��ǰ�༭״̬ ----------------------------
	EditModeEnum GetEditMode();
	
	// ��ǰ״̬ --------------------------------
	BOOL IsEOF();
	BOOL IsBOF();
	BOOL IsOpen();
	long GetState();
	long GetStatus();

	// �����ص�����¼�� --------------------
	long GetMaxRecordCount();
	BOOL SetMaxRecordCount(long count);
	
	// ���λ�� --------------------------------
	CursorLocationEnum GetCursorLocation();
	BOOL SetCursorLocation(CursorLocationEnum CursorLocation = adUseClient);
	
	// ������� --------------------------------
	CursorTypeEnum GetCursorType();
	BOOL SetCursorType(CursorTypeEnum CursorType = adOpenStatic);
	
	// ��ǩ --------------------------------
	_variant_t GetBookmark();
	BOOL SetBookmark(_variant_t varBookMark = _variant_t((long)adBookmarkFirst));
	
	// ��ǰ��¼λ�� ------------------------
	long GetAbsolutePosition();
	BOOL SetAbsolutePosition(int nPosition);

	long GetAbsolutePage();
	BOOL SetAbsolutePage(int nPage);

	// ÿҳ�ļ�¼�� ------------------------
	long GetPageSize();
	BOOL SetCacheSize(const long& lCacheSize);	

	// ҳ�� --------------------------------
	long GetPageCount();

	// ��¼�����ֶ��� ----------------------
	long GetRecordCount();
	long GetFieldsCount();
	
	// ��ѯ�ַ��� --------------------------
	CString GetSQLText() {return m_strSQL;}
	void SetSQLText(LPCTSTR strSQL) {m_strSQL = strSQL;}
	
	// ���Ӷ��� ----------------------------
	CAdoConnection* GetConnection() {return m_pConnection;}
	void SetAdoConnection(CAdoConnection *pConnection);

	// ��¼������ --------------------------
	_RecordsetPtr& GetRecordset();

	CString GetLastError();

// �ֶ����� ----------------------------------------------
public:
	// �ֶμ� -------------------------------
	FieldsPtr GetFields();

	// �ֶζ��� -----------------------------
	FieldPtr  GetField(long lIndex);
	FieldPtr  GetField(LPCTSTR lpszFieldName);
	
	// �ֶ��� -------------------------------
	CString GetFieldName(long lIndex);
	
	// �ֶ��������� -------------------------
	DataTypeEnum GetFieldType(long lIndex);
	DataTypeEnum GetFieldType(LPCTSTR lpszFieldName);

	// �ֶ����� -----------------------------
	long  GetFieldAttributes(long lIndex);
	long  GetFieldAttributes(LPCTSTR lpszFieldName);

	// �ֶζ��峤�� -------------------------
	long  GetFieldDefineSize(long lIndex);
	long  GetFieldDefineSize(LPCTSTR lpszFieldName);

	// �ֶ�ʵ�ʳ��� -------------------------
	long  GetFieldActualSize(long lIndex);
	long  GetFieldActualSize(LPCTSTR lpszFieldName);

	// �ֶ��Ƿ�ΪNULL -----------------------
	BOOL  IsFieldNull(long index);
	BOOL  IsFieldNull(LPCTSTR lpFieldName);

// ��¼���� --------------------------------------------
public:
	BOOL AddNew();
	BOOL Update();
	BOOL UpdateBatch(AffectEnum AffectRecords = adAffectAll); 
	BOOL CancelUpdate();
	BOOL CancelBatch(AffectEnum AffectRecords = adAffectAll);
	BOOL Delete(AffectEnum AffectRecords = adAffectCurrent);
	
	// ˢ�¼�¼���е����� ------------------
	BOOL Requery(long Options = adConnectUnspecified);
	BOOL Resync(AffectEnum AffectRecords = adAffectAll, ResyncEnum ResyncValues = adResyncAllValues);   

	BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);
	BOOL AddNew(CADORecordBinding &pAdoRecordBinding);
	
// ��¼���������� --------------------------------------
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
	BOOL Move(long lRecords, _variant_t Start = _variant_t((long)adBookmarkFirst));
	
	// ����ָ���ļ�¼ ----------------------
	BOOL Find(LPCTSTR lpszFind, SearchDirectionEnum SearchDirection = adSearchForward);
	BOOL FindNext();

// ��ѯ ------------------------------------------------
public:
	BOOL Open(LPCTSTR strSQL, long lOption = adCmdText, CursorTypeEnum CursorType = adOpenStatic, LockTypeEnum LockType = adLockOptimistic);
	BOOL Cancel();
	void Close();

	// ����/����־����ļ� -----------------
	BOOL Save(LPCTSTR strFileName = _T(""), PersistFormatEnum PersistFormat = adPersistXML);
	BOOL Load(LPCTSTR strFileName);
	
// �ֶδ�ȡ --------------------------------------------
public:
	BOOL PutCollect(long index, const _variant_t &value);
	BOOL PutCollect(long index, const CString &value);
	BOOL PutCollect(long index, const double &value);
	BOOL PutCollect(long index, const float  &value);
	BOOL PutCollect(long index, const long   &value);
	BOOL PutCollect(long index, const DWORD  &value);
	BOOL PutCollect(long index, const int    &value);
	BOOL PutCollect(long index, const short  &value);
	BOOL PutCollect(long index, const BYTE   &value);
	BOOL PutCollect(long index, const bool   &value);
	BOOL PutCollect(long index, const COleDateTime &value);
	BOOL PutCollect(long index, const COleCurrency &value);

	BOOL PutCollect(LPCTSTR strFieldName, const _variant_t &value);
	BOOL PutCollect(LPCTSTR strFieldName, const CString &value);
	BOOL PutCollect(LPCTSTR strFieldName, const double &value);
	BOOL PutCollect(LPCTSTR strFieldName, const float  &value);
	BOOL PutCollect(LPCTSTR strFieldName, const long   &value);
	BOOL PutCollect(LPCTSTR strFieldName, const DWORD  &value);
	BOOL PutCollect(LPCTSTR strFieldName, const int    &value);
	BOOL PutCollect(LPCTSTR strFieldName, const short  &value);
	BOOL PutCollect(LPCTSTR strFieldName, const BYTE   &value);
	BOOL PutCollect(LPCTSTR strFieldName, const bool   &value);
	BOOL PutCollect(LPCTSTR strFieldName, const COleDateTime &value);
	BOOL PutCollect(LPCTSTR strFieldName, const COleCurrency &value);

	// ---------------------------------------------------------

	BOOL GetCollect(long index, CString &value);
	BOOL GetCollect(long index, double  &value);
	BOOL GetCollect(long index, float   &value);
	BOOL GetCollect(long index, long    &value);
	BOOL GetCollect(long index, DWORD   &value);
	BOOL GetCollect(long index, int     &value);
	BOOL GetCollect(long index, short   &value);
	BOOL GetCollect(long index, BYTE    &value);
	BOOL GetCollect(long index, bool   &value);
	BOOL GetCollect(long index, COleDateTime &value);
	BOOL GetCollect(long index, COleCurrency &value);

	BOOL GetCollect(LPCSTR strFieldName, CString &strValue);
	BOOL GetCollect(LPCSTR strFieldName, double &value);
	BOOL GetCollect(LPCSTR strFieldName, float  &value);
	BOOL GetCollect(LPCSTR strFieldName, long   &value);
	BOOL GetCollect(LPCSTR strFieldName, DWORD  &value);
	BOOL GetCollect(LPCSTR strFieldName, int    &value);
	BOOL GetCollect(LPCSTR strFieldName, short  &value);
	BOOL GetCollect(LPCSTR strFieldName, BYTE   &value);
	BOOL GetCollect(LPCSTR strFieldName, bool   &value);
	BOOL GetCollect(LPCSTR strFieldName, COleDateTime &value);
	BOOL GetCollect(LPCSTR strFieldName, COleCurrency &value);

	// BLOB ���ݴ�ȡ ------------------------------------------
	BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(long index, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(LPCSTR strFieldName, LPVOID lpData, UINT nBytes);
	BOOL AppendChunk(long index, LPCTSTR lpszFileName);
	BOOL AppendChunk(LPCSTR strFieldName, LPCTSTR lpszFileName);

	BOOL GetChunk(FieldPtr pField, LPVOID lpData);
	BOOL GetChunk(long index, LPVOID lpData);
	BOOL GetChunk(LPCSTR strFieldName, LPVOID lpData);
	BOOL GetChunk(long index, CBitmap &bitmap);
	BOOL GetChunk(LPCSTR strFieldName, CBitmap &bitmap);

// �������� --------------------------------------------
public:
	// ���˼�¼ ---------------------------------
	BOOL SetFilter(LPCTSTR lpszFilter);

	// ���� -------------------------------------
	BOOL SetSort(LPCTSTR lpszCriteria);

	// �����Ƿ�֧��ĳ���� -----------------------
	BOOL Supports(CursorOptionEnum CursorOptions = adAddNew);

	// ��¡ -------------------------------------
	BOOL Clone(CAdoRecordSet &pRecordSet);
	_RecordsetPtr operator = (_RecordsetPtr &pRecordSet);
	
	// ��ʽ�� _variant_t ����ֵ -----------------
	
//��Ա���� --------------------------------------------
protected:
	CAdoConnection     *m_pConnection;
	_RecordsetPtr		m_pRecordset;
	CString				m_strSQL;
	CString				m_strFind;
	CString				m_strFileName;
	IADORecordBinding	*m_pAdoRecordBinding;
	SearchDirectionEnum m_SearchDirection;
public:
	_variant_t			m_varBookmark;
};

//________________________________________________________________________

/*########################################################################
			------------------------------------------------
						   CAdoParameter class
			------------------------------------------------
  ########################################################################*/
class CAdoParameter
{
// ����/�۹� -------------------------------------------
public:
	CAdoParameter();
	CAdoParameter(DataTypeEnum DataType, long lSize, ParameterDirectionEnum Direction, CString strName);
	virtual ~CAdoParameter();
// ���� ------------------------------------------------
public:
	_ParameterPtr GetParameter() {return m_pParameter;}

	// �������� ---------------------------
	BOOL SetPrecision(char nPrecision);

	// ����С��λ�� -----------------------
	BOOL SetNumericScale(int nScale);

	// �������� ---------------------------
	ParameterDirectionEnum GetDirection();
	BOOL SetDirection(ParameterDirectionEnum Direction);
	
	// �������� ---------------------------
	CString GetName();
	BOOL SetName(CString strName);

	// �������� ---------------------------
	int GetSize();
	BOOL SetSize(int size);

	// ���������� -------------------------
	DataTypeEnum GetType();
	BOOL SetType(DataTypeEnum DataType);

// ���� ------------------------------------------------
public:	
	BOOL GetValue(COleDateTime &value);
	BOOL GetValue(CString &value);
	BOOL GetValue(double &value);
	BOOL GetValue(long &value);
	BOOL GetValue(int &value);
	BOOL GetValue(short &value);
	BOOL GetValue(BYTE &value);
	BOOL GetValue(bool &value);

	BOOL SetValue(const float &value);
	BOOL SetValue(const short &value);
	BOOL SetValue(const BYTE &value);
	BOOL SetValue(const COleDateTime &value);
	BOOL SetValue(const CString &value);
	BOOL SetValue(const double &value);
	BOOL SetValue(const long &value);
	BOOL SetValue(const int &value);
	BOOL SetValue(const bool &value);
	BOOL SetValue(const _variant_t &value);

//�������� ------------------------------
public:
	_ParameterPtr& operator =(_ParameterPtr& pParameter);

// ���� ------------------------------------------------
protected:
	_ParameterPtr m_pParameter;
	CString m_strName;
	DataTypeEnum m_nType;
};


/*########################################################################
			------------------------------------------------
						   CAdoCommand class
			------------------------------------------------
  ########################################################################*/

class CAdoCommand
{
// ����/�۹� -------------------------------------------
public:
	CAdoCommand();
	CAdoCommand(CAdoConnection* pAdoConnection, CString strCommandText = _T(""), CommandTypeEnum CommandType = adCmdStoredProc);
	virtual ~CAdoCommand();

// ���� ------------------------------------------------
public:
	_variant_t GetValue(LPCTSTR lpstrName);
	_variant_t GetValue(long index);

	_ParameterPtr GetParameter(long index);
	_ParameterPtr GetParamter(LPCTSTR lpstrName);

	BOOL Append(_ParameterPtr param);

	ParametersPtr GetParameters();
	
	BOOL SetCommandTimeOut(long lTime);

	long GetState();

	BOOL SetCommandType(CommandTypeEnum CommandType);
	BOOL SetCommandText(LPCTSTR lpstrCommand);

	CAdoParameter operator [](int index);
	CAdoParameter operator [](LPCTSTR lpszParamName);

// ʵ�� ------------------------------------------------
public:
	_ParameterPtr CreateParameter(LPCTSTR lpstrName, DataTypeEnum Type, 
								  ParameterDirectionEnum Direction, 
								  long Size, _variant_t Value);
	_RecordsetPtr CAdoCommand::Execute(long Options = adCmdStoredProc);
	BOOL Cancel();
	
// �������� --------------------------------------------
public:
	_CommandPtr& GetCommand();
	BOOL SetConnection(CAdoConnection *pConnect);

// ���� ------------------------------------------------
protected:
	void Release();
	_CommandPtr		m_pCommand;
	CString			m_strSQL;
};


#endif // _INCLUDE_DB_ADOCLASS_INCLUDE_

/*
�鵵��������
������String��split����
���ԣ�

�÷���
CStringExt ext("1;12;123;;1234;",';');
///////11111111111111111111
cout<<ext.GetCount()<<"\n";
ext.BeginEnum();
CString value;
while(ext.GetNext(value)) cout<<(LPCTSTR)value<<"\n";
///////22222222222222222222
DWORD len=ext.GetCount();
for(DWORD i=0;i<len;i++) cout<<(LPCTSTR)ext[i]<<"\n";
cout<<(LPCTSTR)ext[len]<<"\n";
˵����
��Ҫʹ�ñ���ʱ������ʹ��ext.BeginEnum();while(ext.GetNext(value));
GetEnum��[]����ʹ��
����޸����ڣ�
����޸��ߣ�tiewen
//*/
#ifndef _TWEN_SDK_CStringExt_
#define _TWEN_SDK_CStringExt_

typedef void (*EnumCallBack)(const CString&value);

//������AttachԴ�ַ�����ʽ
class CStringExt
{
public:
	static bool File_Exist(LPCTSTR filePathName);
	static bool GetAppPath(CString &path);//����������'\\'

public:
	enum enumEnumAllSubs_FileList
	{
		EASFL_SubFilesOnly=0x00000001,
		EASFL_ReturnPath=0x00000002,
		EASFL_NoExt=0x00000004,//�������ļ���׺������.
		EASFL_SemicolonSep=0x00000008,//�ֺ�,��EASFL_CommaSepͬʱ���ã���������
		EASFL_CommaSep=0x00000010,//����
	};
public:
	static void ToTime(LPCTSTR txt, CTime& time);//���ַ���ת��Ϊʱ�䣬�ַ�����ʽΪ2009-12-31 23:59:59
	static bool IsLong(LPCTSTR txt);//-2147483648-2147483647,static
	static bool IsDWORD(LPCTSTR txt);//0-4294967295,static
	static bool IsLongLong(LPCTSTR txt);//�C9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
	static bool IsDouble(LPCTSTR txt);//û�бȽ���ֵ��Χ

	static bool Split(LPCTSTR srcString,LPCTSTR splitString,BYTE nNum,...);//...:CString*,static
	static bool Split(LPCTSTR srcString,LPCTSTR splitString,BYTE nNum,CString*values);//CString values[nNum],static

	static int LastIndexOf(CString in,TCHAR c);
	static CString GetFileExt(CString fn,LPCTSTR notFound);
	//�ص������У�fileName�������ļ��ĺ�׺��
	static void EnumAllSubs(LPCTSTR folder,LPCTSTR types,LPCTSTR typeSep,
		void (*cb)(LPCTSTR path,LPCTSTR fileName,LPCTSTR fileType,DWORD extraData),bool bSubFilesOnly,DWORD extraData);
	static void EnumAllSubs_Ex(LPCTSTR folder,LPCTSTR types,LPCTSTR typeSep,
		DWORD (*cbFolder)(LPCTSTR path,LPCTSTR folderName,DWORD extraData),
		void (*cbFolderEnd)(LPCTSTR path,LPCTSTR folderName,DWORD extraData),
		void (*cbFile)(LPCTSTR path,LPCTSTR fileName,DWORD extraData),bool bSubFilesOnly,DWORD extraData);
	//ö��folderĿ¼�µ�types�����ļ������ļ��б���;�ŷָ����
	//bSubFilesOnly��ʾ�Ƿ�Ҫ�ݹ���Ŀ¼
	//bPath��ʾ���ص��ļ��б��Ƿ�Ҫ����·��
	static CString EnumAllSubs_FileList(LPCTSTR folder,LPCTSTR types,LPCTSTR typeSep,DWORD flag);

private:
	static void EnumAllSubs_CB_FileList(LPCTSTR path,LPCTSTR fileName,LPCTSTR fileType,DWORD extraData);
	static CString m_stc_strEnumAllSubs_CB_FileList;

public:
	CStringExt();
	CStringExt(LPCTSTR srcString,TCHAR splitChar);
	CStringExt(LPCTSTR srcString,LPCTSTR splitString);
	~CStringExt();

	void Clone(const CStringExt& ext);
	CStringExt& operator =( const CStringExt& ext ){Clone(ext);return *this;}

	void SetSRCString(LPCTSTR srcString);
	void SetSplitString(LPCTSTR splitString);
	void SetSplitString(TCHAR splitChar);
	LPCTSTR GetSplitString() const{return m_splitString;}
	DWORD GetCount();

	//for while support
	void SetNullNext(bool bNullNext);//ѭ������ʱ,����ǰֵΪ�մ�,�Ƿ��Զ���ȡ��һ��
	void BeginEnum();//����ʹ��ѭ������
	bool GetNext(CString&value);

	//Ч���д��Ľ�
	void BeginMove();//ָ��������¼
	void MoveNext();
	void MovePrev();
	void MoveFirst();
	void MoveLast();
	void Move(long lCount);
	bool GetCurrent(CString&value);
	bool IsBOF() const;
	bool IsEOF() const;

	//for single support
	bool GetEnum(DWORD dwIndex,CString&value);
	CString operator[](DWORD dwIndex);

private:
	void Count();
	void Positions();

private:
	CString m_srcString;
	CString m_splitString;
	DWORD m_curPos;
	DWORD m_count;
	DWORD m_curCount;
	bool m_bNullNext;//��ö��ʱ������ǰֵΪ�գ���ö����һ����Ĭ��Ϊfalse
	DWORD *m_pPositions;//���������Ƚ����ַ������Ѹ��ָ���λ�ñ��浽�������У�
	long m_lMoveIndex;
};

/////////////////////////////////////////////////////////////////////////////////
//������,Ψһ����,�ɿ����ʹ��
/*
����A:CSingleAccess sa("mutex");
sa.Access();
...
sa.LeaveAccess();
����:
CWaitingSignal sa;
sa.Setup("mutex");
sa.Access();
...
sa.LeaveAccess();
����B:CSingleAccess sa;
sa.Open("mutex");
sa.Access();
...
sa.LeaveAccess();
*/
class CSingleAccess  
{
public:
	CSingleAccess(LPCTSTR signalName=NULL);
	virtual ~CSingleAccess();

	bool Setup(LPCTSTR signalName);
	bool Open(LPCTSTR signalName);
	void UnInit();
	void Access(bool bUI=false);
	void LeaveAccess();
protected:
	HANDLE m_hSignal;
#ifdef _DEBUG
	CString m_signalName;
#endif
};
/////////////////////////////////////////////////////////////////////////////////
/*
����:����A�д����ź���,�ȴ�����B���ø��ź�����,����A�ټ���ִ��
����A:CWaitingSignal ws("mutex",true);
	ws.Wait();
	����:
	CWaitingSignal ws;
	ws.Open("mutex");
	ws.Reset();
	ws.Wait();
����B:CWaitingSignal ws("mutex",false);
	ws.EndWait();
	����:
	CWaitingSignal ws;
	ws.Open("mutex");
	ws.EndWait();
*/
class CWaitingSignal
{
public:
	static bool Wait(HANDLE handle,bool bUI=false,DWORD dwMilliseconds=INFINITE);
	//��̬����,�ȴ�handle����/������(��ȴ��߳̽���,�ȴ��ź���������)
public:
	CString m_signalName;
	bool m_bInWaitProccess;
public:
	CWaitingSignal(LPCTSTR signalName=NULL,bool bInWaitProccess=true);
	virtual ~CWaitingSignal();

	bool Setup(LPCTSTR signalName,bool bInWaitProccess);//�����ź���
	void UnInit();//�ر�/ɾ���ź���
	bool Open(LPCTSTR signalName);//�����е��ź���
	void Reset();//���ô�����򿪵��ź���
	bool Wait(bool bUI=false,DWORD dwMilliseconds=INFINITE);
	//�ȴ������߳�/���������ź���
	//�����Ƿ�ɹ��ȴ����ź�������ȷ����(ʧ�ܵ�ԭ���кܶ�,���ɹ�ֻ��һ��)
	void EndWait();//�����ź���
protected:
	HANDLE m_hSignal;
};

#endif
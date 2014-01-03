#include "StdAfx.h"
#include <xercesc/dom/DOM.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include "PPFunction.h"
//#include <Winerror.h>

XERCES_CPP_NAMESPACE_USE

extern "C" void  PASCAL EXPORT GetSimpleChar(const TCHAR * wstr,char * & str){
	DWORD dwNum = WideCharToMultiByte(CP_UTF8,NULL,wstr,-1,NULL,0,NULL,FALSE);

	char *psText;
	psText = new char[dwNum];
	WideCharToMultiByte (CP_UTF8,NULL,wstr,-1,psText,dwNum,NULL,FALSE);
	str=psText;
}

extern "C" void  PASCAL EXPORT GetWideChar(const std::string & resstr,wchar_t * & wstr){
	DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, resstr.c_str(), -1, NULL, 0);
	wchar_t *pwText;
	pwText = new wchar_t[dwNum];
	MultiByteToWideChar (CP_UTF8, 0, resstr.c_str(), -1, pwText, dwNum);
	wstr=pwText;
}


std::string GetSimpleChar(const CString & wstr){
	DWORD dwNum = WideCharToMultiByte(CP_UTF8,NULL,wstr,-1,NULL,0,NULL,FALSE);
	char *psText;
	psText = new char[dwNum];
	WideCharToMultiByte (CP_UTF8,NULL,wstr,-1,psText,dwNum,NULL,FALSE);
	std::string str(psText);
	delete [] psText;
	return str;
}

CString GetWideChar(const std::string & resstr){
	DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, resstr.c_str(), -1, NULL, 0);
	wchar_t *pwText;
	pwText = new wchar_t[dwNum];
	MultiByteToWideChar (CP_UTF8, 0, resstr.c_str(), -1, pwText, dwNum);
	CString wstr;
	wstr=pwText;
	delete [] pwText;
	return wstr;
}


extern "C" BOOL  PASCAL EXPORT IsWellFormXML(const TCHAR * xmlStr){
	try
	{
		_XML_INIT_; 
	}catch(const XMLException&)
	{
		return FALSE;
	}// Initialize Xerces infrastructure

	
		XercesDOMParser * m_Parser=new XercesDOMParser();
		
		m_Parser->setValidationScheme( XercesDOMParser::Val_Never );
		m_Parser->setDoNamespaces( false );
		m_Parser->setDoSchema( false );
		m_Parser->setLoadExternalDTD( false );
		
		
		HandlerBase handler;
		m_Parser->setErrorHandler(&handler);
		
		
		char * _xml_Str=NULL;
		GetSimpleChar(xmlStr,_xml_Str);
		
		InputSource* pInputSource = new MemBufInputSource((XMLByte *)_xml_Str, strlen(_xml_Str), _T("CHECK_WELL_FORMAT"));
		if (NULL == pInputSource) {
			if(_xml_Str){delete _xml_Str; _xml_Str=NULL;}
			delete m_Parser;
			_XML_RELEASE_;
			return FALSE; //Memory lack or unknown error
		}
		BOOL retVal=TRUE;
		try{
			m_Parser->parse(*pInputSource);
		}catch (...)
		{
			retVal=FALSE;
		}

		if(_xml_Str){delete _xml_Str; _xml_Str=NULL;}

		delete m_Parser;
		delete pInputSource;
		_XML_RELEASE_;
		return retVal;

}



extern "C" void PASCAL SplitCommaString(CString cStrSource, CStringArray &cArrayStr, TCHAR chSeparator, BOOL bIsXML)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	//**---- ���� ----**
	int ipos = 0;    

	//�����XML���Ȱѿǰ���
	if(bIsXML) {
		ipos = cStrSource.Find(_T(">")); 
		if(ipos >= 0)
		{   
			++ipos;
			//ȥ��ǰ����ܵĿո��Tab�ܻ��з���
			//\n, ���з�(ASCII 10). \r, �س�(ASCII 13). \t, Tab (ASCII 9). \v, ��ֱ�Ʊ��(ASCII 11). 
			for(int i=ipos;i<cStrSource.GetLength();++i) {
				if((int)cStrSource[i]>=9&&(int)cStrSource[i]<=13) {
					++ipos;
					continue;
				}
				else break;
			}
			cStrSource = cStrSource.Right(cStrSource.GetLength()-ipos);
		}

		ipos = cStrSource.Find(_T("<")); 
		if(ipos >= 0)
		{   
			//ȥ��������ܵĿո��Tab�ܻ��з���
			//\n, ���з�(ASCII 10). \r, �س�(ASCII 13). \t, Tab (ASCII 9). \v, ��ֱ�Ʊ��(ASCII 11). 
			for(int i=ipos-1;i>=0;--i) {
				if((int)cStrSource[i]>=9&&(int)cStrSource[i]<=13) {
					--ipos;
					continue;
				}
				else break;
			}

			cStrSource = cStrSource.Left(ipos);
		}
	}



	while(TRUE)
	{	
		ipos = cStrSource.Find(chSeparator);   

		if(ipos >= 0)
		{   
			cArrayStr.Add(cStrSource.Left(ipos));   
			cStrSource = cStrSource.Mid(ipos+1);   
		}
		else if (cStrSource.GetLength() > 0)
		{   
			cArrayStr.Add(cStrSource);   
			break;
		}
		else
		{
			break;
		}
	}

}


extern "C" void PASCAL SplitFile2Part(CString cStrSource, CString &cStrPath, CString &cStrFileTitle, CString &cStrPostfix)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//
	//���ȿ��ǲ���Ŀ¼
	//
	CFileFind finder;

	//����ҵ���
	if(finder.FindFile(cStrSource)) {
		finder.FindNextFile();
		//�����Ŀ¼
		if(finder.IsDirectory()) {
			cStrPath = cStrSource;
			cStrFileTitle = _T("");
			cStrPostfix = _T("");
			return;
		}
	}
	//����û�ҵ���ҲҪ�����ǲ���Ŀ¼
	else {
		if(cStrSource.GetAt(cStrSource.GetLength()-1)==_T('\\')||cStrSource.GetAt(cStrSource.GetLength()-1)==_T(':')){
			cStrPath = cStrSource;
			cStrFileTitle = _T("");
			cStrPostfix = _T("");
			return;
		}
	}

	//���ײ���Ŀ¼����������


	CFile file;
	file.SetFilePath(cStrSource);
	CString cStrFileFullName = file.GetFileName();
	int nPos = -1;
	int nRet;
	//�ҵ����һ�� '.'
	for(;;) {
		nRet = cStrFileFullName.Find(_T('.'), nPos+1);
		if(nRet==-1) break;
		else nPos = nRet;
	}
	if(nPos == -1) {//������û�� '.'
		cStrPostfix = _T("");
		cStrFileTitle = cStrFileFullName;
	}
	else { 
		cStrPostfix = cStrFileFullName.Right(cStrFileFullName.GetLength()-nPos-1);
		cStrFileTitle = cStrFileFullName.Left(nPos);
	}
	cStrPath = cStrSource.Left(cStrSource.Find(cStrFileFullName));

}




//
//����ֵ:
//KMERROR_USER_CANCEL--�û�ȡ��
//
//

extern "C" int PASCAL FileAndFolderOperate(CString cStrFrom, CString cStrTo, UINT nType, FILEOP_FLAGS flags)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	TCHAR *szFrom = NULL;
	TCHAR *szTo = NULL;

	if(cStrFrom.GetLength()>0) {
		szFrom= new TCHAR[cStrFrom.GetLength()+2];
		_tcsncpy_s(szFrom, cStrFrom.GetLength()+1, cStrFrom, cStrFrom.GetLength());
		szFrom[cStrFrom.GetLength()+1] = _T('\0');
	}
	if(cStrTo.GetLength()>0) {
		szTo = new TCHAR[cStrTo.GetLength()+2];;
		_tcsncpy_s(szTo, cStrTo.GetLength()+1, cStrTo, cStrTo.GetLength());
		szTo[cStrTo.GetLength()+1] = _T('\0');
	}


	SHFILEOPSTRUCT   FileOp;   

	FileOp.hwnd=NULL;   
	FileOp.wFunc=nType;   
	FileOp.pFrom= szFrom;
	FileOp.pTo = szTo;
	FileOp.fFlags=flags;//FOF_SIMPLEPROGRESS;//|FOF_RENAMEONCOLLISION;//FOF_NOCONFIRMATION|FOF_SILENT|FOF_ALLOWUNDO;   
	FileOp.fAnyOperationsAborted=FALSE;   
	FileOp.hNameMappings   =NULL;
	FileOp.lpszProgressTitle   =_T("");   
	int   rval=SHFileOperation(&FileOp);

	delete [] szTo;
	delete [] szFrom;

	if(rval==0&&FileOp.fAnyOperationsAborted==TRUE){                   
		//AfxMessageBox(_T("���������Ƿ�ȡ��������ԭĿ¼���ļ����������ԣ�"));
		return KMERROR_USER_CANCEL;
	}
	if(rval!=0){
		if(rval==ERROR_INVALID_NAME)
			return KMERROR_FILE_INVALIDNAME;
		else
			return KMERROR_FILE_OPERATEFAILURE;
	}

	return KMERROR_SUCCESS;
}


//���ֽ���ת��Ϊ "123,567 KB"����ʾ��ʽ
extern "C" void PASCAL FormatTheSizeOfFile(ULONGLONG ulSize, CString &cStrDisp)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(ulSize>1024) {
		cStrDisp.Format(_T("%llu"), ulSize/1024);
		int nOriSize = cStrDisp.GetLength();
		int nCommaPos = cStrDisp.GetLength();
		for(;;) {
			nCommaPos -= 3;
			if(nCommaPos>0)
				cStrDisp.Insert(nCommaPos,_T(','));
			else break;
		}
		cStrDisp += _T(" KB");
	}
	else {
		cStrDisp.Format(_T("%u Byte"), ulSize);
	}
}

//�Ƿ���ϵͳĿ¼��
extern "C" BOOL PASCAL IsInSystemFolder(CString cStrTheFileName, CString &cStrFolderNameText, int &nCSIDL, BOOL &bEqual)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	if(cStrTheFileName.GetLength()<2) {
		bEqual = FALSE;
		return FALSE;
	}

	
	CStringArray arraySysFolder;
	CStringArray arraySysFolderName;
	CWordArray	 arrayCSIDL;
	TCHAR szPath[MAX_PATH];
	CString cStrPathWithEnd;

	//System32
	szPath[0]=TCHAR(0);
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, 0, SHGFP_TYPE_CURRENT,szPath))&&_tcslen(szPath)>2) {
		cStrPathWithEnd = szPath;
		if(szPath[_tcslen(szPath)-1]!=_T('\\'))
			cStrPathWithEnd += _T('\\');
		arraySysFolder.Add(cStrPathWithEnd);
		arraySysFolderName.Add(_T("ϵͳ�ļ��У�System32��"));
		arrayCSIDL.Add(CSIDL_SYSTEM);
	}
	//Windows Ŀ¼
	szPath[0]=TCHAR(0);
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_WINDOWS , 0, SHGFP_TYPE_CURRENT,szPath))&&_tcslen(szPath)>2) {
		cStrPathWithEnd = szPath;
		if(szPath[_tcslen(szPath)-1]!=_T('\\'))
			cStrPathWithEnd += _T('\\');
		arraySysFolder.Add(cStrPathWithEnd);
		arraySysFolderName.Add(_T("Windowsϵͳ�ļ���"));
		arrayCSIDL.Add(CSIDL_WINDOWS);
	}
	//����վ
	szPath[0]=TCHAR(0);
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_BITBUCKET, 0, SHGFP_TYPE_CURRENT,szPath))&&_tcslen(szPath)>2) {
		cStrPathWithEnd = szPath;
		if(szPath[_tcslen(szPath)-1]!=_T('\\'))
			cStrPathWithEnd += _T('\\');
		arraySysFolder.Add(cStrPathWithEnd);
		arraySysFolderName.Add(_T("����վ�ļ���"));
		arrayCSIDL.Add(CSIDL_BITBUCKET);
	}
	//Program files
	szPath[0]=TCHAR(0);
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, 0, SHGFP_TYPE_CURRENT,szPath))&&_tcslen(szPath)>2) {
		cStrPathWithEnd = szPath;
		if(szPath[_tcslen(szPath)-1]!=_T('\\'))
			cStrPathWithEnd += _T('\\');
		arraySysFolder.Add(cStrPathWithEnd);
		arraySysFolderName.Add(_T("�����ļ��У�Program files��"));
		arrayCSIDL.Add(CSIDL_PROGRAM_FILES);
	}

	cStrPathWithEnd = cStrTheFileName;
	if(cStrTheFileName[cStrTheFileName.GetLength()-1]!=_T('\\'))
		cStrPathWithEnd += _T('\\');

	for(int i=0; i<arraySysFolder.GetCount(); ++i) {
		if(cStrPathWithEnd.MakeUpper().Find(arraySysFolder[i].MakeUpper())==0) {
			cStrFolderNameText=arraySysFolderName[i];
			nCSIDL = arrayCSIDL[i];
			//��Ŀ¼������������Ŀ¼
			if(arraySysFolder[i].MakeUpper()==cStrPathWithEnd.MakeUpper())
				bEqual = TRUE;
			else
				bEqual = FALSE;
			return TRUE;
		}
	}

	//��֤����ȣ���Ϊ��������϶������
	bEqual = FALSE;
	return FALSE;
}

extern "C" int PASCAL GetFolderInformation(
	CString cStrFolderName, 
	BOOL	bIncludeSubFolder,
	BOOL	bIncludeHTMLSubFolder,
	CStringArray &arrayFilesResult, 
	INT64 &nTotalSize, 
	int &nSubFolderNumber, 
	int &nFilesNumber)
{


	//
	//�����ļ���Ŀ¼������
	//

	nFilesNumber = 0;
	nSubFolderNumber = 0;
	nTotalSize = 0;


	//
	//	����������ļ��б�
	//
	CFileFind finder;

	CStringArray arrayFileName;
	//	arrayFileName.Copy(*m_pArrayFileName);
	arrayFileName.Add(cStrFolderName);
	BOOL	bNoFolderAnymore = TRUE;
	arrayFilesResult.RemoveAll();//�Ȱ����յ��ļ��б����

	//-------�������ļ��⿪ͳ���ļ��ܵĴ�С,ͬʱ�����ȫչ������ļ��б�-------------
	//���ڷ�չ��Ŀ¼ѡ��������m_cArrayFile2BeDone ��û�������
	for(int ii=0;;++ii) {
		bNoFolderAnymore = TRUE;
		nFilesNumber = 0;
		nTotalSize = 0;
		BOOL  bIsHTMLFileFolder;
		for(int i=0; i<arrayFileName.GetCount(); ++i) {
			if(finder.FindFile(arrayFileName[i])==0) {
				continue;
			}
			finder.FindNextFile();

			//HTML����������Ŀ¼����
			if(!bIncludeHTMLSubFolder&&finder.IsDirectory()) {
				bIsHTMLFileFolder = FALSE;
				CString cStrHTML = arrayFileName[i].Right(6);
				if(cStrHTML.CompareNoCase(_T("_files"))==0||cStrHTML.CompareNoCase(_T(".files"))==0) {//��ʾ���6���ַ����� _files����.files
					cStrHTML = arrayFileName[i].Left(arrayFileName[i].GetLength()-6) + _T(".htm");
					//���ҿ�ͬ����û�ж�Ӧ��HTML�ļ�
					for(int jk=0; jk<arrayFileName.GetCount(); ++jk) {
						if(arrayFileName[jk].CompareNoCase(cStrHTML)==0||arrayFileName[jk].CompareNoCase(cStrHTML+_T("l"))==0) {
							bIsHTMLFileFolder = TRUE;
							break;
						}
					}
				}
				if(bIsHTMLFileFolder) continue;
			}
			//���������Ŀ¼
			if((finder.IsDirectory()&&bIncludeSubFolder)||(finder.IsDirectory()&&ii==0)) {//i==0��ʾ�Ǳ���ĸ�Ŀ¼
				++nSubFolderNumber;
				//���Ҹ�Ŀ¼�µ������ļ�(������Ŀ¼)
				CString cStrSearch;
				cStrSearch = arrayFileName[i] + _T("\\*");
				WIN32_FIND_DATA findData;
				HANDLE hFind = FindFirstFile(cStrSearch, &findData);
				if (hFind == INVALID_HANDLE_VALUE)//��Ŀ¼û���ļ�,�Ͳ�Ҫ��
					continue;
				//����������Ŀ¼���ļ�
				do {
					if (_tcscmp(findData.cFileName, _T(".")) == 0 || 
						_tcscmp(findData.cFileName, _T("..")) == 0)
						continue;

					//�������ļ��ӽ�ȥ
					{
						arrayFilesResult.Add(arrayFileName[i]+_T("\\")+findData.cFileName);
						//�ж�����ļ������Ƿ񳬱�
						//						if(arrayFilesResult.GetCount()>MAX_FILENUMBER_TO_IMPORT_FOLDER) {
						//							AfxMessageBox(_T("����ļ�������������������(10��)!\n\n������������߽�Ŀ¼��Ϊ����(һ���ĵ�)���!"));
						//							OnCancel();//�˳��Ի���
						//						}
					}
					//ͳ���ļ���С
					if(findData.dwFileAttributes!=FILE_ATTRIBUTE_DIRECTORY) {
						nTotalSize = findData.nFileSizeHigh;
						nTotalSize *= (MAXDWORD+(INT64)1);
						nTotalSize += findData.nFileSizeLow;
						++nFilesNumber;
					}

				} while (FindNextFile(hFind, &findData) != 0);

				FindClose(hFind);
				//������Ŀ¼,����
				bNoFolderAnymore = FALSE;
			}
			//������ļ�
			else if(!finder.IsDirectory()){
				arrayFilesResult.Add(arrayFileName[i]);
				//ͳ���ļ���С
				nTotalSize += finder.GetLength();
				++nFilesNumber;
				//�ж�����ļ������Ƿ񳬱�
				//				if(arrayFilesResult.GetCount()>MAX_FILENUMBER_TO_IMPORT_FOLDER) {
				//					AfxMessageBox(_T("����ļ�������������������(10��)!\n\n������������߽�Ŀ¼��Ϊ����(һ���ĵ�)���!"));
				//					OnCancel();//�˳��Ի���
				//				}
			}
		}//
		//�����һ�λ���Ŀ¼���ֽ�,�Ǿ�����ȥ
		if(!bNoFolderAnymore) {
			arrayFileName.RemoveAll();
			arrayFileName.Copy(arrayFilesResult);
			arrayFilesResult.RemoveAll();
		}
		else break;
	}//�ֽ��ļ�ѭ������


	return 0;

}


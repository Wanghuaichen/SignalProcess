
#include "stdafx.h"
#include "Joytech.Exception.h"



MFC_NAMESPACE_BEGIN(Joytech)
MFC_NAMESPACE_BEGIN(Exception)

///////////////////////////////////////////////////////////////////////////
// CJoytechException
///////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CJoytechException, CException)

CJoytechException::CJoytechException(LPCTSTR lpszWhat, LPCTSTR lpszFun, LPCTSTR lpszFile, int iLine):What(lpszWhat)
{
	if(lpszFun) What = What + _T("\r\n������") + lpszFun;
	if(lpszFile) What = What + _T("\r\n�ļ���") + lpszFile;
	if(iLine > -1)
	{
		CString t;
		t.Format(_T("%d"), iLine);
		What = What + _T("\r\n�У�") + t;
	}
}


///////////////////////////////////////////////////////////////////////////
// CJoytechRangeException
///////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CJoytechRangeException, CJoytechException)

CJoytechRangeException::CJoytechRangeException(int iCurrent, int iMin, int iMax, LPCTSTR lpszWhat, LPCTSTR lpszFun, LPCTSTR lpszFile, int iLine):CJoytechException(lpszWhat, lpszFun, lpszFile, iLine)
{
	CString t;
	t.Format(_T("Խ�磺��ǰ��%d����Ч[%d, %d]"), iCurrent, iMin, iMax);
	What = t + _T("\r\n") + What;
}



MFC_NAMESPACE_END(Exception)
MFC_NAMESPACE_END(Joytech)
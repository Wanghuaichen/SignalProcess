// CProcessTools.h: interface for the CProcessTools class.


#if !defined(AFX_CProcessTools_H__3D91DB41_C873_4321_8343_4829079D466A__INCLUDED_)
#define AFX_CProcessTools_H__3D91DB41_C873_4321_8343_4829079D466A__INCLUDED_

#include <afxmt.h>


class CProcessTools //���ݳ� 
{
public:
	static long KillProcess(LPCTSTR processName,long lWhich=-1);
	//ǿ�йر������������ӳ������ΪprocessName�Ľ���
	//lWhich:�رյڼ���ӳ������ΪprocessName�Ľ��̣�-1��ʾȫ����0��ʾ��һ��
	//���سɹ��رյĸ���
	static long ProcessKill(LPCTSTR processName,DWORD dwExceptPID);
	
	//�ر�ӳ������processNames����ID����dwExceptPIDs�е����н���
	static long KillProcess(LPCTSTR processNames,LPCTSTR strExceptPIDs,LPCTSTR sep);
	static long KillProcess(LPCTSTR processNames,LPCTSTR sep,DWORD dwExceptPID);
};

#endif // !defined(AFX_CProcessTools_H__3D91DB41_C873_4321_8343_4829079D466A__INCLUDED_)

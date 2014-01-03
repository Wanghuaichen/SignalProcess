

#ifndef TWEN_SDK_AutoPositionSize
#define TWEN_SDK_AutoPositionSize

#include "afxcoll.h"

enum enumAutoPositionSizeType
{
	APST_LEAVE,//���ı�
	APST_SIZE,//�ı��С
	APST_POSITION,//�ı�λ��
};

class CAutoPositionSize
{
public:
	CAutoPositionSize();
	~CAutoPositionSize();

	void Auto(DWORD uiResID, CWnd* parent, enumAutoPositionSizeType eX, enumAutoPositionSizeType eY);
	void Auto(CWnd* pWnd, enumAutoPositionSizeType eX, enumAutoPositionSizeType eY);//�����pWndҪ�־ñ��棬���Բ�Ҫ����GetDlgItem(IDC_XXX)�ķ���ֵ

	void Delete(DWORD uiResID, CWnd* parent);
	void Delete(CWnd* pWnd);

	void DeleteAll();

	void InitInfo(CWnd* parent);
	void AutoPositionSize();//��onresized�е���

	void ShowWindow(int swCommand);

	inline bool IsInitialized() const{return m_bInit;}

private:
	long _Index(DWORD uiResID, CWnd* parent);
	long _Index(CWnd* pWnd);
	void _PS(enumAutoPositionSizeType e, int ov, long& lt, long& rb);
	CArray<DWORD, DWORD> m_arrInfos;
	CWnd* m_pParent;
	CRect m_rcParentInit;
	bool m_bInit;
};

#endif
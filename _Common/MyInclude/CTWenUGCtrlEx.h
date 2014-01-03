#pragma once

#include "UGCtrl.h"

class CTWenUGCtrlEx: public CUGCtrl
{
public:
	CTWenUGCtrlEx();
	virtual ~CTWenUGCtrlEx(){}

	BOOL CreateTable(CWnd *pcParent, CRect rect);

	bool SelfOrSub(HWND hWnd) const;

public:
	BOOL SetTitle(int nCol, CString cStrTitleName, int nColWidth, int nTitleNO = -1);
	void SetAutoAdd_AddRow(bool bAuto){m_bAutoAdd_RowButtonAdd = bAuto;}
	void Sort_ByNumber(bool emptyFirst, int col, long rowFrom, long rowTo, long rowHead = -1);
	void SwapRow(long i, long j);

public:
	void AppendRow_ButtonAdd();//����ʹ�ã������ظ����
	void RomoveRow_ButtonAdd();//����ʹ�ã�������Ҳ�������ɾ��

private:
	long RowIndexOfMinValue(bool emptyFirst, int col, long rowFrom, long rowTo);
	virtual void Action_AddRow() = 0;
	virtual void Action_DeleteRow(int nRow) = 0;
	virtual void Action_DisplayRowInf(int nRow) = 0;

protected:
	virtual void OnSetup(void);
	void OnLClicked(int col,long row, int updn, RECT *rect, POINT *point, int processed);
	virtual void OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed);
	virtual int EditCtrlFinished(LPCTSTR string, BOOL cancelFlag, BOOL continueFlag, int continueCol, long continueRow);
	virtual void OnEditCtrlFinished(){}

public:
	int m_nIndexBMP_Delete;		//ɾ��ͼ��
	int m_nIndexBMP_Add;		//���һ�е�ͼ��
	int m_nIndexBMP_INF;		//�鿴��Ϣͼ��
	int m_nFontID_Title;
	int m_nFontID_Content;		//����е���������
	COLORREF m_crContent;

	bool m_bDislplayFirstColumn;
	bool m_bAutoAdd_RowButtonAdd;	//�ڱ��ĩβ�Զ�����[������]
};
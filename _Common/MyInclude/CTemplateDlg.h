
/*
	CTemplateDlg dlg(this);
	dlg.SetTitle(_T("TemplateDlg"));
	UINT id=CTemplateDlg::ID_Begin;
	CString ids[10];
	for(int i=0; i<10; i++)
	{
		ids[i].Format(_T("%d"),id+i);
	}
	dlg.SetCtrlInfo(21,_T("edit"),CTemplateDlg::CT_Edit,ES_READONLY|WS_BORDER,NULL,CTemplateDlg::VT_Text|CTemplateDlg::VT_NoZero,_T("DefaultEditValue|12345678"),//text,dwctrltype,dwOptionalStyle,dwselecttype,dwvaluetype(|���ID��ʡ)
		_T("ע��:"),CTemplateDlg::CT_Label,NULL,NULL,NULL,_T("����һ����ͨ��Label"),
		_T("����һ����ͨ��Label����һ�о�ֻ�����Label"),CTemplateDlg::CT_None,NULL,NULL,NULL,NULL,
		_T("ע��:"),CTemplateDlg::CT_Label,NULL,NULL,NULL,_T("����һ����ͨ��Label"),
		_T("checkbox"),CTemplateDlg::CT_CheckBox,NULL,NULL,NULL,_T("1|this is a checkbox|")+ids[0],//value(1,0:�Ƿ�ѡ)[|windowtext|ID]
		_T("ComboBoxDropList"),CTemplateDlg::CT_ComboBoxDropList,NULL,NULL,CTemplateDlg::VT_ItemData,_T("30|it1:10,it2:20,it3:30|")+ids[1],//value|list[|ID]
		_T("ComboBoxDropList"),CTemplateDlg::CT_ComboBoxDropList,NULL,NULL,CTemplateDlg::VT_Index,_T("0|it1,it2,it3"),
		_T("ComboBoxDropdown"),CTemplateDlg::CT_ComboBoxDropdown,NULL,NULL,CTemplateDlg::VT_Text|CTemplateDlg::VT_NoZero,_T("it2|it1,it2,it3"),
		_T("date"),CTemplateDlg::CT_Date,NULL,NULL,NULL,_T("2008-9-10|")+ids[2],//value[|ID]
		_T("time"),CTemplateDlg::CT_Time,NULL,NULL,NULL,_T("08:9:10|")+ids[3],
		_T("char"),CTemplateDlg::CT_Edit,WS_DISABLED,NULL,CTemplateDlg::VT_Char,_T("c"),
		_T("signedchar"),CTemplateDlg::CT_Edit,NULL,NULL,CTemplateDlg::VT_SignedChar,_T("127"),
		_T("BYTE"),CTemplateDlg::CT_Edit,WS_BORDER,NULL,CTemplateDlg::VT_BYTE,_T("255"),
		_T("short"),CTemplateDlg::CT_Edit,NULL,NULL,CTemplateDlg::VT_Short,_T("32767"),
		_T("WORD"),CTemplateDlg::CT_Edit,NULL,NULL,CTemplateDlg::VT_WORD,_T("65535"),
		_T("long"),CTemplateDlg::CT_Edit,NULL,NULL,CTemplateDlg::VT_Long,_T("2147483647|")+ids[4],
		_T("DWORD"),CTemplateDlg::CT_Edit,NULL,NULL,CTemplateDlg::VT_DWORD,_T("4294967295|")+ids[5],
		_T("longlong"),CTemplateDlg::CT_Edit,NULL,NULL,CTemplateDlg::VT_LongLong,_T("9223372036854775807|")+ids[6],
		_T("float"),CTemplateDlg::CT_Edit,NULL,NULL,CTemplateDlg::VT_Float,_T("3.4E +/- 38 (7 digits)"),
		_T("double"),CTemplateDlg::CT_Edit,NULL,NULL,CTemplateDlg::VT_Double,_T("1.7E +/- 308 (15 digits)"),
		_T("ComboBoxDropListString"),CTemplateDlg::CT_ComboBoxDropList,NULL,NULL,CTemplateDlg::VT_ItemDataString,_T("V2|it1:V1,it2:V2,it3:V3")//value|list[|ID]
		);
	//checkbox
	dlg.SetRelation_Enabled(id,id+1,true);
	dlg.SetRelation_Enabled(id,id+3,false);
	dlg.SetRelation_Visible(id,id+2,true);
	dlg.SetRelation_Visible(id,id+4,false);
	//combobox
	dlg.SetRelation_Enabled(5,id+1,id+5,_T("10,30"),_T(","));//VT_ItemData
	dlg.SetRelation_Enabled(5,id+1,id+6,_T("10"),_T(","));//VT_ItemData
	dlg.SetRelation_Visible(5,id+1,id+5,_T("20"),_T(","));//VT_ItemData
	dlg.SetRelation_Visible(5,id+1,id+6,_T("30"),_T(","));//VT_ItemData
	if(dlg.DoModal()==IDOK) MessageBox(dlg.m_strValue);
	CString values[21];
	CStringExt::Split(dlg.m_strValue,dlg.m_strSep,21,values);
	values[0]:��һ���ؼ���ֵ
	values[20]:���һ���ؼ�����"ComboBoxDropListString"��Ӧ�Ŀؼ���ֵ
*/

/*
	//SetRelation_Setup��ʾ����ʡ����ѡ��Ϊ��
	CTemplateDlg dlg(this);
	dlg.SetTitle(_T("TemplateDlg"));
	UINT id=CTemplateDlg::ID_Begin;
	CString ids[10];
	for(int i=0; i<10; i++)
	{
		ids[i].Format(_T("%d"),id+i);
	}
	dlg.SetCtrlInfo(3,
		_T("ComboBoxDropList"),CTemplateDlg::CT_ComboBoxDropList,NULL,NULL,CTemplateDlg::VT_Text,_T("ʡ1|ʡ1,ʡ2,ʡ3|")+ids[0],//value|list[|ID]
		_T("ComboBoxDropList"),CTemplateDlg::CT_ComboBoxDropList,NULL,NULL,CTemplateDlg::VT_Text,_T("||")+ids[1],
		_T("ComboBoxDropdown"),CTemplateDlg::CT_ComboBoxDropdown,NULL,NULL,CTemplateDlg::VT_Text,_T("||")+ids[2]
		);

	//3��ʡ������Ϣ
	dlg.SetRelation_Setup(0,id,1,id+1,_T("��11|��11,��12,��13"),NULL,_T("ʡ1"),_T(","));
	dlg.SetRelation_Setup(0,id,1,id+1,_T("��21|��21,��22,��23"),NULL,_T("ʡ2"),_T(","));
	dlg.SetRelation_Setup(0,id,1,id+1,_T("��31|��31,��32,��33"),NULL,_T("ʡ3"),_T(","));

	//9���е�����Ϣ
	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��111|��111,��112,��113"),NULL,_T("��11"),_T(","));
	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��121|��121,��122,��113"),NULL,_T("��12"),_T(","));
	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��131|��131,��132,��113"),NULL,_T("��13"),_T(","));

	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��211|��211,��212,��213"),NULL,_T("��21"),_T(","));
	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��221|��221,��222,��213"),NULL,_T("��22"),_T(","));
	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��231|��231,��232,��213"),NULL,_T("��23"),_T(","));

	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��311|��311,��312,��313"),NULL,_T("��31"),_T(","));
	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��321|��321,��322,��313"),NULL,_T("��32"),_T(","));
	dlg.SetRelation_Setup(1,id+1,2,id+2,_T("��331|��331,��332,��313"),NULL,_T("��33"),_T(","));
//*/

/*
//��CTemplateDlg��������ʾ
class CMyTDlg : public CTemplateDlg
{
public:
	CMyTDlg(CWnd* pParent = NULL);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnEnChangeEdit();
};

CMyTDlg::CMyTDlg(CWnd* pParent):CTemplateDlg(pParent)
{
	UINT id=CTemplateDlg::ID_Begin;
	CString ids[10];
	for(int i=0; i<10; i++)
	{
		ids[i].Format(_T("%d"),id+i);
	}
	SetCtrlInfo(2,
		_T("ComboBoxDropList"),CTemplateDlg::CT_ComboBoxDropList,NULL,NULL,CTemplateDlg::VT_Text,_T("ʡ1|ʡ1,ʡ2,ʡ3|")+ids[0],
		_T("Text Edit"),CTemplateDlg::CT_Edit,WS_BORDER,NULL,CTemplateDlg::VT_Text,_T("this is a text control.|")+ids[1]
	);
}

BEGIN_MESSAGE_MAP(CMyTDlg, CTemplateDlg)
	ON_CBN_SELCHANGE(CTemplateDlg::ID_Begin, &CMyTDlg::OnCbnSelchangeCombo)
	ON_EN_CHANGE(CTemplateDlg::ID_Begin+1, &CMyTDlg::OnEnChangeEdit)
END_MESSAGE_MAP()

void CMyTDlg::OnCbnSelchangeCombo()
{
	MessageBox(_T("selection changed!"));
}

void CMyTDlg::OnEnChangeEdit()
{
	MessageBox(_T("content changed!"));
}

//���ԣ�
	CMyTDlg dlg(this);
	dlg.DoModal();
	���ߣ�
	CMyTDlg m_dlg;
	...
	m_dlg.Create(this);
	m_dlg.ShowWindow(SW_SHOW);
//*/

#if !defined(AFX_TemplateDlg_H__D0874304_E71E_4B46_BC84_6AB7150EE4EC__INCLUDED_)
#define AFX_TemplateDlg_H__D0874304_E71E_4B46_BC84_6AB7150EE4EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TemplateDlg.h : header file
//

////////////////////////////////////////////////////////////////////////////////////////////////
//����SetRelationϵ�к���������û��Ϊ��Ŀ����������Ŀ��Ӧ�Ŀؼ���ID����ӳ���ϵ�����������ú����У�
//����ͬʱҪ��ͬһ����Ŀ���������Ϳؼ�ID��������������������Գ�����һ����Ҫ�Ľ�
////////////////////////////////////////////////////////////////////////////////////////////////

//CTemplateDlgͬʱ֧��ģʽ�ͷ�ģʽ

class CDlgTemplateBuilder
{
public:
	enum enumWPAN// Windows predefined atom names
	{
		BUTTON=0x0080,
		EDIT,
		STATIC,
		LISTBOX,
		SCROLLBAR,
		COMBOBOX
	};
public:
	CDlgTemplateBuilder(DWORD dwLength=0);
	~CDlgTemplateBuilder();
public:// functions to build the template
	void SetDataLength(DWORD dwLength);
	DLGTEMPLATE* GetTemplate();
	DLGTEMPLATE* BeginTemplate(DWORD dwStyle,LPCRECT rc,LPCTSTR caption,DWORD dwStyleEx=0);
	void AddItem(WORD wType, DWORD dwStyle,LPCRECT rc,LPCTSTR text,WORD nID=-1,DWORD dwStyleEx=0);
	void AddItem(WORD wType, DWORD dwStyle,LPCRECT rc,WORD nResID,WORD nID=-1,DWORD dwStyleEx=0);
protected:
	WORD* AddText(WORD* buf,LPCTSTR text);
	void AddItemTemplate(WORD wType, DWORD dwStyle,LPCRECT rc,WORD nID,DWORD dwStyleEx);	
	WORD* AlignDWORD(WORD* ptr);// align ptr to nearest DWORD
protected:
	WORD* m_pBuffer;// internal buffer holds dialog template
	WORD* m_pNext;// next WORD to copy stuff
	WORD* m_pEndBuf;
};

/////////////////////////////////////////////////////////////////////////////
// CTemplateDlg dialog

class CTemplateDlg : public CDialog
{
public:
	static int GetFirstSel(CListBox*pLB);
	static int GetCurSel(CListCtrl&m_lst);
	static bool _IsControlVisible(CWnd*pWnd);

public:
	enum enumSelectType//ListBox,ListCtrl��Ч
	{
		ST_Single=0x00000001,
		ST_Multiple=0x00000002,
		ST_Extended=0x00000004,//����ѡ��,�ͷ��ؽ����Ӱ��
		ST_None=0x00000008
	};
	enum enumValueType//ǰ8b����
	{
		VT_ItemData=0x00000000,//GetItemData
		VT_ItemDataString,//GetItemData��Ӧ�ı�����һ���ַ���
		VT_Index,//has a list
		VT_Text,//�ݲ�֧�ֳ��Ⱥ���ֵ��Χ
		//������Editר��,�ݲ�֧�ֳ��Ⱥ���ֵ��Χ
		VT_Char,
		VT_SignedChar,
		VT_BYTE,
		VT_Short,
		VT_WORD,
		VT_Long,
		VT_DWORD,
		VT_LongLong,
		VT_Float,
		VT_Double,
		VT_URLHTTPWeak,//��http,�ݲ�֧��,��VT_Text
		//��localhost->http://localhost/,163->http://www.163.net,
		//163.com->http://www.163.com
		VT_URLHTTPStrong,//http,�ݲ�֧��,��VT_Text
		//��localhost(error),163.com(error),
		//http://localhost/(ok),http://www.163.com(OK)
		VT_URLFTPWeak,//�ݲ�֧��,��VT_Text
		VT_URLFTPStrong,//�ݲ�֧��,��VT_Text
		//ʱ��������Ҫע�ⷵ�صĸ�ʽ
		VT_DateTime,
		VT_Date,
		VT_Time,

		VT_Length=0x01000000,//��鳤�ȷ�Χ(-1���ޣ�1��-1��-1��10��)����edit��VT_Text��Ч���ݲ�֧��
		VT_NoZero=0x02000000,//VT_Textֵ����ʱ������Ϊ��

		VT_GetType=0x00FFFFFF//������;
	};
	enum enumControlType
	{
		CT_None=0x00000000,//������һ��label
		CT_Label,
		CT_Button,//�������ͬCT_Label
		CT_Edit,
		CT_CheckBox,
		CT_ComboBoxDropList,//�������������
		CT_ComboBoxDropdown,//���������������
		CT_Date,
		CT_Time
	};

public:
	enum enumRangeControlID
	{
		ID_Begin=WM_USER+101,
		ID_End=WM_USER+10101,
	};

	//��Ŀ������IDӳ�俪ʼ
public:
	UINT NewControlID();//�����µ�ID��
	CString NewControlIDString();

	void SetRelation_Enabled_ID(UINT idDetected,UINT idHandled,bool bSame);//for checkbox
	void SetRelation_Visible_ID(UINT idDetected,UINT idHandled,bool bSame);//for checkbox

	void SetRelation_Enabled_ID(UINT idDetected,UINT idHandled,LPCTSTR values,LPCTSTR sep);//for ComboBox
	void SetRelation_Visible_ID(UINT idDetected,UINT idHandled,LPCTSTR values,LPCTSTR sep);//for ComboBox

	void SetRelation_Value_ID(UINT idDetected,UINT idHandled,LPCTSTR value,LPCTSTR values,LPCTSTR sep);//Ŀǰi������combobox

	void SetRelation_Setup_ID(UINT idDetected,UINT idHandled,LPCTSTR newValue,LPCTSTR newValueSep,LPCTSTR values,LPCTSTR sep);//Ŀǰi��j������combobox

protected:
	CMap<UINT,UINT,UINT,UINT> m_mapIndexID;
	UINT m_uiControlID;

	void SetMapIndexID(UINT index, UINT id);

	//���º���������SetupUI֮�����
	UINT GetIndexByID(UINT id) const;//������SetupUI֮�����
	UINT GetIDByIndex(UINT index) const;//������SetupUI֮�����

	void SetRelation_Enabled_Index(UINT indexDetected,UINT indexHandled,bool bSame);//for checkbox,������SetupUI֮�����
	void SetRelation_Visible_Index(UINT indexDetected,UINT indexHandled,bool bSame);//for checkbox,������SetupUI֮�����

	void SetRelation_Enabled_Index(UINT indexDetected,UINT indexHandled,LPCTSTR values,LPCTSTR sep);//for ComboBox,������SetupUI֮�����
	void SetRelation_Visible_Index(UINT indexDetected,UINT indexHandled,LPCTSTR values,LPCTSTR sep);//for ComboBox,������SetupUI֮�����

	void SetRelation_Value_Index(UINT indexDetected,UINT indexHandled,LPCTSTR value,LPCTSTR values,LPCTSTR sep);//Ŀǰi������combobox,������SetupUI֮�����

	void SetRelation_Setup_Index(UINT indexDetected,UINT indexHandled,LPCTSTR newValue,LPCTSTR newValueSep,LPCTSTR values,LPCTSTR sep);//Ŀǰi��j������combobox,������SetupUI֮�����

	//��Ŀ������IDӳ�����

public:
	//����ʹ�ܹ�ϵ
	//1.bSameΪtrue����IDΪuiIDDetected��checkbox��ѡ�У���uiIDHandled��Ӧ�Ĵ��ڱ�������򱻻ҵ�
	//2.bSameΪfalse����IDΪuiIDDetected��checkbox��ѡ�У���uiIDHandled��Ӧ�Ĵ��ڱ��ҵ������򱻼���
	void SetRelation_Enabled(UINT uiIDDetected,UINT uiIDHandled,bool bSame);//for checkbox,Ϊ�����ϰ汾���ڲ�ʹ��
	//���ÿ��ӹ�ϵ
	//1.bSameΪtrue����IDΪuiIDDetected��checkbox��ѡ�У���uiIDHandled��Ӧ�Ĵ��ڱ���ʾ����������
	//2.bSameΪfalse����IDΪuiIDDetected��checkbox��ѡ�У���uiIDHandled��Ӧ�Ĵ��ڱ����أ�������ʾ
	void SetRelation_Visible(UINT uiIDDetected,UINT uiIDHandled,bool bSame);//for checkbox,Ϊ�����ϰ汾���ڲ�ʹ��

	//����ʹ�ܹ�ϵ
	//values��ComboBox��ֵ�б�,��sep�ָ��ֵӦ����uiIDDetected��ֵ���Ͷ�Ӧ
	//�����ǰֵ���б��У���ʹ��uiIDHandled������ҵ�uiIDHandled
	void SetRelation_Enabled(int i,UINT uiIDDetected,UINT uiIDHandled,LPCTSTR values,LPCTSTR sep);//for ComboBox,Ϊ�����ϰ汾���ڲ�ʹ��
	//���ÿ��ӹ�ϵ
	void SetRelation_Visible(int i,UINT uiIDDetected,UINT uiIDHandled,LPCTSTR values,LPCTSTR sep);//for ComboBox,Ϊ�����ϰ汾���ڲ�ʹ��

	//����ֵ��ϵ
	void SetRelation_Value(int i,UINT uiIDDetected,int j,UINT uiIDHandled,LPCTSTR value,LPCTSTR values,LPCTSTR sep);//Ŀǰi������combobox,Ϊ�����ϰ汾���ڲ�ʹ��

	//�����б��������Ŀ�����ؽ���ϵ
	//newValue���ο�ʾ���е������Ŀ������֧�����Ŀؼ�ID
	//newValueSep��������Ŀǰ��Ч
	void SetRelation_Setup(int i,UINT uiIDDetected,int j,UINT uiIDHandled,LPCTSTR newValue,LPCTSTR newValueSep,LPCTSTR values,LPCTSTR sep);//Ŀǰi��j������combobox,Ϊ�����ϰ汾���ڲ�ʹ��
/*
	DWORD NewGroup();
	void AddToGroup(DWORD dwGroup, int i);
	void AddToGroup(DWORD dwGroup, UINT id);
//*/
protected:
	void MakeSureRelation();
	void _RefreshDialog();//���ݿ���������ˢ�¸��Ӵ���λ�úʹ��ڴ�С�������ı䴰�ڵ����Ͻ�����

	struct structRelationMapItem
	{
		UINT uiID;
		bool bSame;
	};
	typedef CArray<structRelationMapItem,structRelationMapItem> CTemplateDlg_MapItem;
	typedef CMap<UINT,UINT,CTemplateDlg_MapItem*,CTemplateDlg_MapItem*> CTemplateDlg_Map;
	CTemplateDlg_Map m_mapEnabled,m_mapVisible;
	void SaveRelationItem(CTemplateDlg_MapItem*pMapItem,UINT uiIDHandled,bool bSame);
	void Relation_Enabled(UINT id, CTemplateDlg_MapItem& item);
	void Relation_Visible(UINT id, CTemplateDlg_MapItem& item);
	void Relation_Enabled(UINT id, structRelationMapItem& item);
	void Relation_Visible(UINT id, structRelationMapItem& item);
	//���򷵻أ����ޣ���bNewΪtrue���Ӻ󷵻أ����򷵻�NULL
	CTemplateDlg_MapItem* GetTemplateDlg_MapItem(CTemplateDlg_Map&map, UINT key, bool bAutoNew);

	struct structRelationMapItem_ComboBox
	{
		int i;
		UINT uiID;
		CString values;
		CString sep;
	};
	typedef CArray<structRelationMapItem_ComboBox,structRelationMapItem_ComboBox> CTemplateDlg_MapItem_ComboBox;
	typedef CMap<UINT,UINT,CTemplateDlg_MapItem_ComboBox*,CTemplateDlg_MapItem_ComboBox*> CTemplateDlg_Map_ComboBox;
	CTemplateDlg_Map_ComboBox m_mapEnabled_ComboBox,m_mapVisible_ComboBox;
	void SaveRelationItem(CTemplateDlg_MapItem_ComboBox*pMapItem,int i,UINT uiIDHandled,LPCTSTR values,LPCTSTR sep);
	void Relation_Enabled(UINT id, CTemplateDlg_MapItem_ComboBox& item);
	void Relation_Visible(UINT id, CTemplateDlg_MapItem_ComboBox& item);
	void Relation_Enabled(UINT id, structRelationMapItem_ComboBox& item);
	void Relation_Visible(UINT id, structRelationMapItem_ComboBox& item);
	//���򷵻أ����ޣ���bNewΪtrue���Ӻ󷵻أ����򷵻�NULL
	CTemplateDlg_MapItem_ComboBox* GetTemplateDlg_MapItem(CTemplateDlg_Map_ComboBox&map, UINT key, bool bAutoNew);

	struct structValueMapItem
	{
		int i;
		UINT uiID;
		int j;
		CString value;
		CString values;
		CString sep;
	};
	typedef CArray<structValueMapItem,structValueMapItem> CTemplateDlg_MapItem_Value;
	typedef CMap<UINT,UINT,CTemplateDlg_MapItem_Value*,CTemplateDlg_MapItem_Value*> CTemplateDlg_Map_Value;
	CTemplateDlg_Map_Value m_mapValue;
	void Relation_Value(UINT id, CTemplateDlg_MapItem_Value& item);
	void Relation_Value(UINT id, structValueMapItem& item);
	//���򷵻أ����ޣ���bNewΪtrue���Ӻ󷵻أ����򷵻�NULL
	CTemplateDlg_MapItem_Value* GetTemplateDlg_MapItem(CTemplateDlg_Map_Value&map, UINT key, bool bAutoNew);

	struct structSetupMapItem
	{
		int i;
		UINT uiID;
		int j;
		CString newValue;
		CString newValueSep;//����
		CString values;
		CString sep;
	};
	typedef CArray<structSetupMapItem,structSetupMapItem> CTemplateDlg_MapItem_Setup;
	typedef CMap<UINT,UINT,CTemplateDlg_MapItem_Setup*,CTemplateDlg_MapItem_Setup*> CTemplateDlg_Map_Setup;
	CTemplateDlg_Map_Setup m_mapSetup;
	void Relation_Setup(UINT id, CTemplateDlg_MapItem_Setup& item);
	void Relation_Setup(UINT id, structSetupMapItem& item);
	//���򷵻أ����ޣ���bNewΪtrue���Ӻ󷵻أ����򷵻�NULL
	CTemplateDlg_MapItem_Setup* GetTemplateDlg_MapItem(CTemplateDlg_Map_Setup&map, UINT key, bool bAutoNew);
public:
	CDlgTemplateBuilder m_builder;
	bool m_bEnableBtnOK;
	bool m_bEnableBtnCancel;
	bool m_bShowBtnOK;
	bool m_bShowBtnCancel;
	CString m_strTitle;
	CString m_strValue;//"value1;value2;",�ǵ��ȷ����������ĸ��ؼ���ֵ����m_strSep�ָ�
	CString m_strSep;//Ĭ��ΪӢ��;��

// Construction
public:
	CTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTemplateDlg();

	void SetTitle(LPCTSTR title);
	void SetDefaultFont(CFont* pFont){m_pDefaultFont=pFont;}
	void SetUsingOKFont(bool bUsingFont){m_bUsingOKFont=bUsingFont;}
	BOOL Create(CWnd* pParentWnd=NULL,DWORD dwStyle=WS_POPUP | WS_CAPTION | WS_SYSMENU);
	void SetCtrlInfo(BYTE nNum,...);//text,dwctrltype,dwOptionalStyle,dwselecttype,dwvaluetype,strDefault
	//strDefault:initSelect|item1:dword1,item2:dword2(:dword����valuetypeΪItemDataʱ��Ч)
	//2006-1-2(CT_Date),16:10:2(CT_Time)
	void SetCtrlInfo(BYTE nNum, const CString* pText, const DWORD* pdwCtrlType, const DWORD* pdwOptionalStyle, const DWORD* pdwSelectType, const DWORD* pdwValueType, const CString* pstrDefault);
	void SetDlgMargin(int l,int t,int r,int b);//dlg�߾�
	void SetColumn1Margin(int l,int t,int r,int b);
	void SetColumn2Width(int ew);
	void SetColumn1WH(int w,int h,bool bAutoSet);//������margin
	void SetSpacing(int hs,int vs);
	void SetCBHeight(int iCBHeight);//combobox�߶�
	void SetOKCancelTopPadding(int iTopPadding);

// Dialog Data
	//{{AFX_DATA(CTemplateDlg)
	CButton	m_btnOK;
	CButton	m_btnCancel;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTemplateDlg)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void BeforeRelation(){}
	void DoSaveData();
	void DoTemplate(DWORD dwStyle=WS_POPUP | WS_CAPTION | WS_SYSMENU);
	void Free();
	void GetSelectValue(CString&st,CComboBox*pCB,DWORD dwValueType);
	void GetSelectValue(CString&st,CListCtrl*pLC,int iItem,int iSubItem,DWORD dwValueType);
	void GetSelectValue(CString&st,CListBox*pLB,int iItem,DWORD dwValueType);
	void GetSelectValue(CString&st,CListCtrl*pLC,int iSubItem,DWORD dwSelectType,DWORD dwValueType);
	void GetSelectValue(CString&st,CListBox*pLB,DWORD dwSelectType,DWORD dwValueType);
	void MakeSureC1WH();
	void MakeSureSys(CRect&rcDlg,POINT&ptOKCancel);
	void SetupUI();
	void SetupCB(CComboBox*pCB,DWORD dwValueType,LPCTSTR value);
	void SetCBDropListSel(CComboBox*pCB,DWORD dwValueType,LPCTSTR value);
	void SetDate(CDateTimeCtrl*pDT,LPCTSTR date);
	void SetTime(CDateTimeCtrl*pDT,LPCTSTR time);
	void GetDateTime(CString&st,CDateTimeCtrl*pDT,DWORD dwValueType);
	bool MakeSureText(BYTE i,CString&text,DWORD dwValueType,CString&error);//�ݲ�֧�ֳ��Ⱥ���ֵ��Χ
	CString m_strText;
	BYTE m_nNum;
	CStatic*m_pStatics;
	CWnd**m_pCtrls;
	DWORD*m_pdwCtrlTypes,*m_pdwOptionalStyles,*m_pdwSelectTypes,*m_pdwValueTypes;
	CString*m_pDefaultValues;
	RECT m_rcDlgMargin,m_rcC1Margin;
	int m_iColumn2Width,m_iOKCancelTopPadding;
	POINT m_ptSpacing,m_ptC1;
	bool m_bAutoSet;
	CRect m_rcDlg;
	POINT m_ptDlgClient,m_ptOKCancel;
	int m_iCBHeight;
	CFont*m_pDefaultFont;
	bool m_bError;
	bool m_bUsingOKFont;//ʹ��OK��ť������
	// Generated message map functions
	//{{AFX_MSG(CTemplateDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRangeIDs(UINT nID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TemplateDlg_H__D0874304_E71E_4B46_BC84_6AB7150EE4EC__INCLUDED_)

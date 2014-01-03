#pragma once

#include <vector>
#include <deque>
using std::vector;
using std::deque;

struct CaliItem {
	CString szNO;				//���
	CString szReserveTime;		//����ʱ��
	CString szComponentName;	//�����
	CString szLevel;			//����
	CString szContents;			//����
	CString szPeakArea;			//�����
	CString szPeakHeight;		//���
	CString szResFactor;		//��Ӧ����
	CString szIsReference;		//�Ƿ�α�(�ο���
	CString szIsInterior;		//�Ƿ��ڱ�(�ڱ��
	CString szStdContent;		//�ڱ꺬�����ڱ���Ժ���
	CString szInteriorNo;		
	//�ڱ���
	CString szSampleFullName;	//��Ʒ�ļ�ȫ·����
	CString szPeakBeginX;		//��ʼ��
	CString szPeakEndX;			//��ֹ��
	CString szSlope;			//б��
	CString szIntercept;		//�ؾ�
	CString szCoefficient;		//���ϵ��
	CString szPeakType;         //������
	CString szPeakAreaPercent;  //�����%
	CString szPeakHighPercent;  //���%
	CString szConcentration;    //����[ug/mL] (����������
	
	CaliItem() { }
	CaliItem(const CStringArray& data);
	CaliItem(const CStringArray& data, const CString tableName);
	bool operator== (const CaliItem& rhs);
};



struct CaliCurveData {
	double _x; //x��
	double _y; //y��
	CaliCurveData() : _x(0.0), _y(0.0) { }
	CaliCurveData(double dx, double dy) : _x(dx), _y(dy) { } 
};





class CCaliData {
private:
	vector<CaliItem> m_CaliItems; 
	CString m_tableType; //���Ʊ�����ͣ������||���
	CString m_tableName; //ת���ı����

public:
	CCaliData(void);
	~CCaliData(void);
public: //���Žӿ�

	//�ⲿ�����������ڲ�����
	void InitCaliData(CIADataTable* pCIADataTable); 

	//�����������
	bool ItemSaveToTable(CIADataTable* pCIADataTable);
	bool ItemSaveToCalibrateResultTable(CIADataTable* pCIADataTable);

	//���ɶ��Ʊ������
	bool GenerateTable(CIADataTable &cIADataTable, const CString& caliMethod);
	bool GenerateCalibrateResultTable(CIADataTable &cIADataTable); //����У�������
	
	//��ǰ�ж���������
	int GetItemSize() { return static_cast<int>(m_CaliItems.size()); }

	//�½���Ʒ�ı���ʱ���ж�
	bool VerifyReserveTime(const CString& szReserveTime, int& match_no, int& instA, int& instB);

	//�������ݵ�i��֮�� iStart��ͬ����ݿ�ʼ���к�
	bool InsertRowData(int iRow, int iStart, const CStringArray& strArry, LPCTSTR lpcszPathName);
	bool InsertRowData(int iRow, const CStringArray& strArry, LPCTSTR lpcszPathName);
	//����������
	bool UpdateRowData(int iRow, const CStringArray& strArry);

	//�����������ݼ���
	bool ModifyInsertItemLevel();

	//�����Ʒ�ļ���ȫ·�������ظ���
	bool GetSampleFullPath(CStringArray& strArry);
	bool GetSampleFullPath(int row, CString& strVal);

	//ɾ��i������
	bool DeleteRowData(int iRow);
	bool DeleteRowData(CString& szPath); //ɾ�����Ǵ���szPath����Ʒ�ļ�
	bool DeleteNotFirstRow();

	//��֤�ڱ꺬���Ƿ�Ӧ�ô���
	void VerifyContent();

	//�޸�i�������е��ڱ꺬��
	bool ModifyContent(int nRow, CString& szContent); 
	
	//ɾ��i�������е��ڱ꺬��
	bool ClearContent(int nRow);

	//��ȡi�е��ڱ꺬�� 
	bool GetPreContent(int nRow, CString& szContent); 

	//���������ת��
	void CaliTable2CaliResultTable(const CCaliData& CaliTable);

	//�������
	void ClearAllData() { m_CaliItems.clear(); }

//*****�ڱ�����غ���
	//�Զ������ڱ���
	bool AddInteriorNo(int nRow);
	//����������Ƿ��Ѵ����ڱ���
	bool IsExistInteriorNo();
	//ÿ����ݵ��ڱ���Ϊ1
	bool AllInteriorNoBe1();
	//�ҳ�������ڱ���
	int AutoFindInteriorNo();
	//�ҳ���ǰ��С�ڱ���
	int FindMinInteriorNo();
	//���Ե�ǰ���Ƿ���Ψһ�ڱ���Ϊ1��
	bool IsOnlyInteriorNo1(int row);
	//��ǰ���ڱ����Ƿ�Ψһ
	bool IsInteriorNoOnlyOne();
	//����޸ĵı���Ƿ��Ѿ�����
	bool CheckExistInteriorNo(int row, const CString& strVal);
	//�ı�ͬ�ڱ��ŵ��������
	bool ChangeSameCompnentInteriorNo(int row, const CString& dest);
	//����strNo�Ƿ��ڵ�ǰ�ڱ��ŷ�Χ��
	bool VertifyInteriorNoInterval(const CString& strNo);
	//ͬ���ڱ������ݣ��ֶ����
	bool ManualInteriorNo(int col, int row, LPCTSTR szVal);
	//�Զ���������ݱ�ɾ�����ڱ����ȷ��
	void AutoCheckInteriorNoAfterDel();
	//�Զ����¶Ա�Ž���������
	void AutoReorderID();
	//ָ��Ϊ�α�
	bool DesignatedReference(int row, bool bIsReference);
	//ָ��Ϊ�ڱ�
	bool DesignatedInterior(int row, bool bIsInterior); 
	//ͬ��ɾ�������ڱ���ֺʹ�ǰ���ڸ��ڱ�ķ��ڱ���ֵı�ű�Ϊ��
	bool SyncDelInterior(int row);
	//��������ڱ���
	void ClearAllInteriorNo();
	//ͬ���༭�ĵ�Ԫ����ڲ����ݵ�ֵ
	bool SynEditedCell(int col, int row, LPCTSTR szVal);
	 
	// ���Ե�ǰ���Ƿ���һ����ݵ�����
	bool VertifyFirstLine(int row);

	// ����row�������Ƿ����ڱ�
	bool VertifyIsReference(int row);
	bool VertifyIsInterior(int row);
	
	//�����һ��������е�������Ԫ��������
	void ClearUnnecessaryCell();
	void ClearUnnecessaryCell(int row);

	// ͳһС��λ��ʽ
	void UnifyDecimalDigits();

	//��ȡrow�ж�Ӧ�������
	void GetComponentName(int row, CString& strName);

	//��ȡrow�ж�Ӧ�ı���ʱ��
	void GetReserveTime(int row, CString& strVal);
	void GetOpEdXTime(int row, double &dOp, double& dEd); //�Ż���Op��EdӦ����Ϊһ�����������

	//[��귨/���ٷֱȷ�/����һ����]+[�����/���]������
	bool GetCaliCurveData_TypeA(int row, deque<CaliCurveData>& data);

	//[�ڱ귨/�ڱ�ٷֱ�]+[�����/���]������
	bool GetCaliCurveData_TypeB(int row, deque<CaliCurveData>& data);
	
	//����б�ʣ��ؾ࣬���ϵ��
	bool SetSlope_Intercept_Coefficient(int row, double s, double i, double c);

	//У�α���
	bool _GenerateCabParamPageTable(CIADataTable &cIADataTable);

	//�������㲿��
	void AddData(const CaliItem& item);
	void GetRefPeakData(CCaliData& data);
	void GetIntPeakData(CCaliData& data);
	void GetcOtherPeakData(CCaliData& data);
	void GenerateDuplicate(CCaliData& data) const; //���һ�ݸ���ǰ������ͬ�ĸ���
	void ModifyResTime(vector<double> &afterResTime); 
	void GetRefPeakInterval(int n, int& before, int& after);
	int  Row2No(int row); //��������ת��Ϊ�ڼ����ο���
	void CancelRefPeakMark(int no); //ȡ����no���ο�����
	void CancelIntPeakMark(int no); //ȡ����no���ڱ����
	CaliItem& GetRowDate(int row); //��õ�ǰ������

	bool GetRowData(int nRow, CStringArray& strArry); //��õ�i������
	bool GetInteriorNoDate(CStringArray& strArry);    //��á��ڱ��š�������

private: //˽�нӿ�
	bool GetComponentIndex(int row, int& b, int& e);

	bool GeneratePHTable(CIADataTable &cIADataTable);
	bool GeneratePATable(CIADataTable &cIADataTable);
	
	
	bool GetCaliResultRowData(int nRow, CStringArray& strArry); //��õ�i������
	

	// �����һ��ƥ��ļ���
	bool GetSampleLevel(int& iValue); 
	void SubOne(vector<int> &vec, int i);

	// ɾ�����Ƿ�����ݵ�����,�ǵĻ�����.��֤��ɾ����ʱ����ʧ��Ʒ��Ϣ
	bool CheakCopyInfo(int iRow);

	// ����С��λ��
	void ConvertDecimalDigits(CString& obj, int dts);

	// �ѿ�ѧ������ʽָ��λ��e000���e00
	void EraseZeroPlus(CString& str); 
	void EraseZeroMinus(CString& str); 

	//�����ַ����Ƿ�������
	bool IsDigital(CString& str); 
	//�����ַ����Ƿ�����Ȼ��
	bool IsNaturalNumber(CString& str);
};



class CCaliMethod
{
public:
	static const CStringArray& StringIDs();
	static const CStringArray& StringNames();
	static int IndexByStringID(const CString& id);
	static const CString& StringIDByIndex(int index);
	static const CString* NameByID(const CString& id);
};
class CCaliMethodOn
{
public:
	static const CStringArray& StringIDs();
	static const CStringArray& StringNames();
	static int IndexByStringID(const CString& id);
	static const CString& StringIDByIndex(int index);
	static const CString* NameByID(const CString& id);
};

class CCaliUI
{
public:
	static void InitComboBox(CComboBox* pCB, const CStringArray* pSA);
};
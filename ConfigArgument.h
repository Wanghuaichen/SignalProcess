#pragma once

#include <string>
#include <vector>
#include <deque>
using std::string;
using std::vector;
using std::deque;
using std::pair;


class ConfigArgument  
{
public:
	CString ArgName;	    //��������
	double BeginTime;       //��ʼʱ��
	double EndTime;         //����ʱ��
	double InputValue;      //ֵ
	ConfigArgument();
	ConfigArgument(CString argname, double begint);
	ConfigArgument(CString argname, double begint, double endt);
	ConfigArgument(CString argname, double begint, double endt, double val);
	ConfigArgument(double begint, double val);

	bool operator== (const ConfigArgument &rhs);
};

class strict_weak_ordering 
{
public:
	inline bool operator()(const std::pair<int, ConfigArgument> a, const std::pair<int, ConfigArgument> b) 
	{ 
		return a.first < b.first; 
	} 
};

class CSema;
class CPeak;
class AnalysisConfigArguments //�����崦�����ò���
{
public:
	struct PeakSepaArg { //�ַ����
		unsigned StartPeakNo;
		unsigned FinishPeakNo;
		int SepaType; //�ַ����ͣ�0=��Ȼ��ߣ�1=��ֱ�ַ壬2=���зַ壬3=���зַ�
		explicit PeakSepaArg(unsigned, unsigned, int);
	};
	//���캯��
	AnalysisConfigArguments();
	explicit AnalysisConfigArguments(double, double, double, double);

	//�ⲿ�崦����� --> �ڲ��崦�����
	void TransformArgs(deque<ConfigArgument>& args);

	//�������ò���
	void SetArgs(const deque<ConfigArgument>& args);

	//���������ò���
	void addArgs(const ConfigArgument& configArg);


//--------------------------------------------------------------------------------------------
//                                           Ԥ������                                           
//--------------------------------------------------------------------------------------------
	//���ò���Ԥ����
	void PreprocessConfigArgs();

	//���÷ֶ��������Ų���Ĭ��ֵ
	void SetPeakSearchArgs(int size, double defaultSlope);

	//���÷ֶθ��������Ĭ��ֵ
	void SetMinusCheckArgs(int size);

	//���÷ֶ���С����ֵ����Ĭ��ֵ
	void SetMinPeakWidthArgs(int size, double defaultVal);

	//���÷ֶ���С���������Ĭ��ֵ
	void SetMinPeakAreaArgs(int size, double defaultVal);

	//���÷ֶ���С��߲���Ĭ��ֵ
	void SetMinPeakHeightArgs(int size, double defaultVal);
//--------------------------------------------------------------------------------------------
//                                           ��������  
//--------------------------------------------------------------------------------------------
	//����Ѱ����Ҫ����������
	VOID Analysis_PeakSearchSlopeBased(const vector<CSema> &Data);

	//����Ѱ����Ҫ�ĸ�����
	VOID Analysis_PeakSearchMinusCheck(const vector<CSema> &Data);

	//������ֹ������Ҫ�Ĳ���
	VOID Analysis_BanPeak(const vector<CPeak> &Peak);

	//������С������Ҫ�Ĳ���
	VOID Analysis_MinHalfPeakWidth(const vector<CPeak> &Peak);

	//�������߷ַ���Ҫ�Ĳ���
	void Analysis_TangentDividPeak(const vector<CPeak> &Peak, deque<PeakSepaArg> &dequeIdx);

	//�����ص��崦����Ҫ�Ĳ���
	VOID Analysis_PeakSeparation(const vector<CPeak> &Peak, vector<PeakSepaArg> &psArgs);

	//�����ֹ��ַ���Ҫ�Ĳ���
	VOID Analysis_HandPeakSeparation(const vector<CPeak> &Peak, deque<double> &HandPeakArgs);
	//�����ֹ��ַ���Ҫ��һ�����������ط������ڶ�Ӧ�ķ��±ꡣ-1��ʾ�Ҳ������������ķ��±�
	int  Analysis_OneHandPeakSepar(const vector<CPeak> &Peak, double starttime);

	//�����ֹ�������Ҫ�Ĳ������ֱ�װ��3������
	VOID Analysis_HandAdjustPeak(const vector<CPeak> &Peak);
	VOID Analysis_HandworkBaseLine(const vector<CPeak> &Peak); //�Զ��ж��ǵ�����㻹�����

	//�����ϲ�����Ҫ�Ĳ���
	VOID Analysis_MergePeak(const vector<CPeak> &Peak);

	//�����ֶ���С�����
	VOID Analysis_MinPeakArea(const vector<CPeak> &Peak);

	//�����ֶ���С���
	VOID Analysis_MinPeakHeight(const vector<CPeak> &Peak);

	//�����ֹ��ⶥ�����ӷ壩��Ҫ�Ĳ��� ��2011.07.21׷�ӣ�
	VOID Analysis_HandAddPeak(const vector<CPeak> &Peak);

	//����ɾ����������
	VOID Analysis_HandDelPeak(const vector<CPeak> &Peak);

	//������ӷ��ɾ�����ǰ��˳�����
	VOID Analysis_AddelPeak(const vector<CPeak> &Peak);

	BOOL IsOverlapping(double dA, double dB, double dC, double dD);

//----------------------------------------------------------------------------------------------
//                                           ��������Ĳ���                                           
//----------------------------------------------------------------------------------------------
	vector<double>& GetPeakSearchArgs();
	deque<bool>& GetMinusCheckArgs();
	vector<unsigned>& GetBanPeakArgs();
	vector<double>& GetMinHalfPeakWidthArgs();
	vector<double>& GetMinPeakAreaArgs();
	vector<double>& GetMinPeakHeightArgs();
	vector<double>& GetHandAdjustPeakArgs();
	vector<pair<unsigned, double> >& GetHandAdjustStartArgs();
	vector<pair<unsigned, double> >& GetHandAdjustFinishArgs();
	vector<pair<double, double> >& GetMergePeakArgs(); //�ع�������ɾ��
	vector<pair<unsigned, unsigned> >& GetMergePeakNo();
	deque<ConfigArgument>& GetAddPeakArgs();
	deque<ConfigArgument>& GetDelPeakArgs();
	deque<ConfigArgument>& GetAddDelPeakArgs();
	bool GetMinusCheakStatus(double point);
	deque<ConfigArgument>& GetAllArguments();
	deque<ConfigArgument>& GetResultArgument();
	double& GetDefaultPeak2Valley();
	bool IsPeak2Valley();

private:
	void Pre_PeakSearchSlopeBased(); //Ԥ����ֶ����޲���
	void Pre_PeakSearchMinusCheck(); //Ԥ����ֶθ�����
	void Pre_HalfPeakWidth();        //Ԥ����ֶ���С����
	void Pre_PeakSeparation();       //Ԥ����ַ����
	void Pre_Peak2ValleyRatio();     //Ԥ�����ȱȲ���

public:
	deque<ConfigArgument> _ConfigArgument; //ȫ�������ò����б�
	deque<ConfigArgument> _ResultArgument; //���ȫ����ȷ�Ĳ���

	vector<double> _SlopeBased;	  //�ֶ���������
	vector<unsigned> _BanPeakNo;  //�ֶν�ֹ���ַ��
	deque<bool> _MinusCheck;	  //�ֶθ�����
	vector<double> _MinHalfWidth; //�ֶ���С����������Ԫ�ظ����벨�����һ��
	vector<double> _MinPeakArea;  //�ֶ���С�����
	vector<double> _MinPeakHeight; //�ֶ���С���
	vector<double> _HandPeakSpra; //�ֹ��ַ���ʼʱ��

	vector<pair<unsigned,double> > _AdjustStart;  //�������
	vector<double> _AdjustPeak;   //�������
	vector<pair<unsigned,double> > _AdjustFinish; //�����յ�
	vector<pair<double, double> > _MergePeak; //�ϲ������
	vector<pair<unsigned, unsigned> > _MergePeakNo; //�ϲ����

	deque<ConfigArgument> _AddPeak;   //���ӷ����(�ֹ��ⶥ)
	deque<ConfigArgument> _DelPeak;   //ɾ������
	deque<ConfigArgument> _AddelPeak; //���ӷ��ɾ����Ļ������
	
	//vector<unsigned> _DelPeak;		  //ɾ������

	double _defaultSlope;	  //Ĭ������
	double _defaultHalfWidth; //Ĭ����С����
	double _defaultPeakArea;  //Ĭ����С�����
	double _defaultPeakHeight; //Ĭ����С���

//������������ȱ�
	double _defaultPeak2Valley; //Ĭ�Ϸ�ȱ�
	
};


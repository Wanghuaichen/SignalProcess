#include "StdAfx.h"
#include "ConfigArgument.h"
#include "Peak.h"
#include <math.h>

#define DEFAULTPEAK2VALLEY 500.0f

ConfigArgument::ConfigArgument()
{  }

ConfigArgument::ConfigArgument(CString argname, double begint)
	:ArgName(argname),
	BeginTime(begint),
	EndTime(-1.1f),
	InputValue(0.0f)
{  }

ConfigArgument::ConfigArgument(CString argname, double begint, double endt)
	:ArgName(argname),
	BeginTime(begint),
	EndTime(endt),
	InputValue(0.0f)
{  }

ConfigArgument::ConfigArgument(CString argname, double begint, double endt, double val)
	:ArgName(argname),
	 BeginTime(begint),
	 EndTime(endt),
	 InputValue(val)
{  }

ConfigArgument::ConfigArgument(double begint, double val)
	:ArgName(_T("�ֹ�����")),
	BeginTime(begint),
	InputValue(val)
{  }

bool ConfigArgument::operator== (const ConfigArgument &rhs)
{
	return (this->ArgName==rhs.ArgName)  
		&& (this->BeginTime - rhs.BeginTime == 0.0000f)
		&& (this->EndTime - rhs.EndTime == 0.0000f) 
		&& (this->InputValue - rhs.InputValue == 0.0000f);
}

AnalysisConfigArguments::PeakSepaArg::PeakSepaArg(unsigned b, unsigned e, int t)
	:StartPeakNo(b),
	FinishPeakNo(e),
	SepaType(t)
{  }

AnalysisConfigArguments::AnalysisConfigArguments()
	:_defaultSlope(0.0000f),
	_defaultHalfWidth(0.0000f),
	_defaultPeakArea(0.0000f),
	_defaultPeakHeight(0.0000f),
	_defaultPeak2Valley(DEFAULTPEAK2VALLEY) //�ڲ�����Ĭ�Ϸ�ȱȣ�����Ҫ�û�����
{  }


AnalysisConfigArguments::AnalysisConfigArguments(double defaultSlope, double defaultHalfWidth, double defaultPeakArea, double defaultPeakHeight)
	:_defaultSlope(defaultSlope),
	 _defaultHalfWidth(defaultHalfWidth),
	 _defaultPeakArea(defaultPeakArea),
	 _defaultPeakHeight(defaultPeakHeight),
	 _defaultPeak2Valley(DEFAULTPEAK2VALLEY) //�ڲ�����Ĭ�Ϸ�ȱȣ�����Ҫ�û�����
{

}

void AnalysisConfigArguments::TransformArgs(deque<ConfigArgument>& args) {
	/* 
	��С����(�ֶ�) --> �ֶ���С����;
	б��(�ֶ�)		--> �ֶ���������; 
	��С�����(�ֶ�) --> �ֶ���С�����; 
	��С���(�ֶ�)	-->  
	��ֹ����         --> ��ֹ����
	������			--> �ֶθ�����;
	��Ȼ���			--> �ص��崦��; ��ȱ�;
	���߷ַ�value>=1.0Ϊǰ�У�����Ϊ���� --> �ص��崦��;
	�ϲ���			--> �ϲ���;
	���ѷ�			--> �ֹ��ַ�;
	�ֹ�����			--> �ֹ�����;
	�ֹ��嶥			--> �ֹ�����;
	��ӷ�			--> ���ӷ�;
	*/
	deque<ConfigArgument> tmp;
	size_t argsSize = args.size();
	for (size_t i=0; i<argsSize; ++i) {
		if (args[i].ArgName == _T("��С����(�ֶ�)"))
			args[i].ArgName = _T("�ֶ���С����");

		else if (args[i].ArgName == _T("б��(�ֶ�)"))
			args[i].ArgName = _T("�ֶ���������");

		else if (args[i].ArgName == _T("��С�����(�ֶ�)"))
			args[i].ArgName = _T("�ֶ���С�����");

		else if (args[i].ArgName == _T("��С���(�ֶ�)"))
			args[i].ArgName = _T("�ֶ���С���");

		else if (args[i].ArgName == _T("������"))
			args[i].ArgName = _T("�ֶθ�����");

		else if (args[i].ArgName == _T("��Ȼ���")) {
			args[i].ArgName = _T("�ص��崦��");
			args[i].InputValue = 0.0;
		}
		else if (args[i].ArgName == _T("���߷ַ�")) {
			args[i].ArgName = _T("�ص��崦��");
			if(args[i].InputValue>=1.0) args[i].InputValue = 2.0;
			else args[i].InputValue = 3.0;
		}
		else if (args[i].ArgName == _T("���ѷ�"))
			args[i].ArgName = _T("�ֹ��ַ�");

		else if (args[i].ArgName == _T("�ֹ��嶥")){
			args[i].ArgName = _T("�ֹ�����");
			args[i].InputValue = 22;
		}
		else if (args[i].ArgName == _T("��ӷ�"))
			args[i].ArgName = _T("���ӷ�");
		else{// do nothing
		}
	}
}

void AnalysisConfigArguments::SetArgs(const deque<ConfigArgument>& args)
{
	if (!_ConfigArgument.empty())
		_ConfigArgument.clear();
	_ConfigArgument.insert(_ConfigArgument.end(),
							args.begin(),
							args.end());
}

//---------------------------------------------------------------------------------
// ����Ĭ�ϲ���
//---------------------------------------------------------------------------------
void AnalysisConfigArguments::SetPeakSearchArgs(int size, double defaultSlope)
{
	std::vector<double> vectBuff(size, defaultSlope);
	_SlopeBased.swap(vectBuff);
}

void AnalysisConfigArguments::SetMinusCheckArgs(int size)
{
	std::deque<bool> dequeBuff(size, false);
	_MinusCheck.swap(dequeBuff);
}

void AnalysisConfigArguments::SetMinPeakWidthArgs(int size, double defaultVal)
{
	std::vector<double> vectBuff(size, defaultVal);
	_MinHalfWidth.swap(vectBuff);
}

void AnalysisConfigArguments::SetMinPeakAreaArgs(int size, double defaultVal)
{
	std::vector<double> vectBuff(size, defaultVal);
	_MinPeakArea.swap(vectBuff);
}

void AnalysisConfigArguments::SetMinPeakHeightArgs(int size, double defaultVal){
	std::vector<double> vectBuff(size, defaultVal);
	_MinPeakHeight.swap(vectBuff);
}

void AnalysisConfigArguments::addArgs(const ConfigArgument& configArg) 
{
	_ConfigArgument.push_back(configArg);
}

//---------------------------------------------------------------------------------
// ���ò���Ԥ����
//---------------------------------------------------------------------------------
void AnalysisConfigArguments::Pre_PeakSearchSlopeBased()
{
	//�����������������ֵ
	//------------------------------------------------------------------
	//---�������Ŵ���ԭ��: 
	//---������A��B�����䣬A������Ϊ5000��B������Ϊ8000���������ص��ˡ�
	//---�򰴣�����A����5000��ʣ�²���ΪB����8000�������ص�����ȫ����A�����5000��
	//------------------------------------------------------------------
	const unsigned ArgsSize = _ConfigArgument.size();
	double lastTime = static_cast<double>(-INT_MAX);
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֶ���������"))
		{
			if (_ConfigArgument[i].BeginTime > lastTime) //�������ص�
			{
				continue;
			}
			else //�����ص�
			{
				_ConfigArgument[i].BeginTime = lastTime;
			}
			lastTime = _ConfigArgument[i].EndTime;
		}
	}
	//Sleep(2);
}

void AnalysisConfigArguments::Pre_PeakSearchMinusCheck()
{
	//�ֶθ��������������ֵ
	//------------------------------------------------------------------
	//---�ֶθ����⴦��ԭ��: 
	//---������A��B�����䣬A������Ϊ5000��B������Ϊ8000���������ص��ˡ�
	//---�򰴣�����A����5000��ʣ�²���ΪB����8000�������ص�����ȫ����A�����5000��
	//------------------------------------------------------------------
	const unsigned ArgsSize = _ConfigArgument.size();
	double lastTime = static_cast<double>(-INT_MAX);
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֶθ�����"))
		{
			if (_ConfigArgument[i].BeginTime > lastTime) //�������ص�
			{
				continue;
			}
			else //�����ص�
			{
				_ConfigArgument[i].BeginTime = lastTime;
			}
			lastTime = _ConfigArgument[i].EndTime;
		}
	}
	//Sleep(2);
}

void AnalysisConfigArguments::Pre_HalfPeakWidth()
{
	//�ֶ���С���� ���������ֵ
	//------------------------------------------------------------------
	//---�ֶ���С���� ����ԭ��: 
	//---������A��B�����䣬A����С����5000��B����С����Ϊ8000���������ص��ˡ�
	//---�򰴣�����A����5000��ʣ�²���ΪB����8000�������ص�����ȫ����A�����5000��
	//------------------------------------------------------------------
	const unsigned ArgsSize = _ConfigArgument.size();
	double lastTime = static_cast<double>(-INT_MAX);
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֶ���С����"))
		{
			if (_ConfigArgument[i].BeginTime > lastTime) //�������ص�
			{
				//continue;
			}
			else //�����ص�
			{
				_ConfigArgument[i].BeginTime = lastTime;
			}
			//������ȷ��������ò���
			ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
				_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
			_ResultArgument.push_back(configArg);
			lastTime = _ConfigArgument[i].EndTime;
		}
	}
	//Sleep(2);
}

void AnalysisConfigArguments::Pre_PeakSeparation()
{
	//�ص��崦�� ���������ֵ
	//------------------------------------------------------------------
	//---�ص��崦�� ����ԭ��: 
	//---A,B�����ص�������A�� #B��A��������ʼ
	//------------------------------------------------------------------

	const unsigned ArgsSize = _ConfigArgument.size();

	deque<ConfigArgument> deque_buff;

	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName != _T("�ص��崦��")){
			deque_buff.push_back(_ConfigArgument[i]);
			continue;
		}
		

		bool isOverlap = false;
		double start_time = _ConfigArgument[i].BeginTime;
		double end_time = _ConfigArgument[i].EndTime;
		// 121204 ����ķ����䲻���ص�֮ǰ������
		for (unsigned j=0; j<i; ++j)
		{
			if (_ConfigArgument[j].ArgName == _T("�ص��崦��")){

				double prev_start_time = _ConfigArgument[j].BeginTime;
				double prev_end_time = _ConfigArgument[j].BeginTime;

				if ((start_time > prev_start_time && start_time < prev_end_time) 
					||(end_time > prev_start_time && end_time < prev_end_time) )
				{ 
					isOverlap = true;
					break;
				}
			}
		}
		if (!isOverlap)
		{
			//������ȷ��������ò���
			deque_buff.push_back(_ConfigArgument[i]);

			ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
				_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
			_ResultArgument.push_back(configArg);
		}
	}

	deque_buff.swap(_ConfigArgument);

}

void AnalysisConfigArguments::Pre_Peak2ValleyRatio()
{
	if (_ConfigArgument.empty()) return;
	const unsigned ArgsSize = _ConfigArgument.size();
	unsigned ResultInx = 0;
	bool bSucceed = false;
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("��ȱ�"))
		{
			_defaultPeak2Valley = _ConfigArgument[i].InputValue;
			ResultInx = i;
			bSucceed = true;
		}
	}
	//������ȷ��������ò���
	if (bSucceed)
	{
		ConfigArgument configArg(_ConfigArgument[ResultInx].ArgName, _ConfigArgument[ResultInx].BeginTime, 
			_ConfigArgument[ResultInx].EndTime,_ConfigArgument[ResultInx].InputValue);
		_ResultArgument.push_back(configArg);
	}
}

void AnalysisConfigArguments::PreprocessConfigArgs()
{
	
	Pre_PeakSearchSlopeBased(); //�������� ���������ֵ
	Pre_PeakSearchMinusCheck(); //�ֶθ����� ���������ֵ
	Pre_HalfPeakWidth();        //�ֶ���С���� ���������ֵ
	Pre_PeakSeparation();       //�ص��崦�� ���������ֵ

	Pre_Peak2ValleyRatio(); // ��ȱ�
}


//---------------------------------------------------------------------------------
// ��������Ҫ�Ĳ���
//---------------------------------------------------------------------------------
VOID AnalysisConfigArguments::Analysis_PeakSearchSlopeBased
	(const vector<CSema> &Data)
{
	deque<ConfigArgument> dequeBuffArgs;
	deque<double> dequeBuff;
	//����Ĳ����Ѿ�����
	//�������еĲ������ҵ����ֶ��������š�������
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned DataSize = Data.size();

	double lastTime = -INT_MAX;
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֶ���������"))
		{//�ص�����Ĵ���
			if (_ConfigArgument[i].BeginTime - lastTime >= 0.000000)
			{
				dequeBuff.push_back(_ConfigArgument[i].BeginTime);
				dequeBuff.push_back(_ConfigArgument[i].InputValue);
				dequeBuff.push_back(_ConfigArgument[i].EndTime);
				//������ȷ��������ò���
				ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
					_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
				_ResultArgument.push_back(configArg);
			}
			lastTime = _ConfigArgument[i].EndTime;
		}
	}


	if (dequeBuff.empty())
		TRACE(_T("dequeBuff.empty()"));	

	//���ֶ��������Ÿ�ֵ
	for (unsigned i=0; i<DataSize && !dequeBuff.empty(); ++i)
	{
		if (Data[i].x - dequeBuff[0] >= 0.000000)
		{
			_SlopeBased[i]=dequeBuff[1];
		}
		if (Data[i].x - dequeBuff[2] > 0.000000)
		{
			dequeBuff.pop_front();
			dequeBuff.pop_front();
			dequeBuff.pop_front();
		}
	}
}

//����Ѱ����Ҫ�ĸ�����
VOID AnalysisConfigArguments::Analysis_PeakSearchMinusCheck
	(const vector<CSema> &Data)
{
	deque<ConfigArgument> dequeBuffArgs;
	deque<double> dequeBuff;
	//����Ĳ����Ѿ�����
	//�������еĲ������ҵ����ֶθ����⡱������
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned DataSize = Data.size();

	double lastTime = -INT_MAX;
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֶθ�����"))
		{//�ص�����Ĵ���
			if (_ConfigArgument[i].BeginTime - lastTime >= 0.000000 )
			{
				dequeBuff.push_back(_ConfigArgument[i].BeginTime);
				dequeBuff.push_back(_ConfigArgument[i].EndTime);
				//������ȷ��������ò���
				ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
					_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
				_ResultArgument.push_back(configArg);
			}
			lastTime = _ConfigArgument[i].EndTime;
		}
	}


	if (dequeBuff.empty())
		TRACE(_T("dequeBuff.empty()"));	

	//���ֶθ����⸳ֵ
	for (unsigned i=0; i<DataSize && !dequeBuff.empty(); ++i)
	{
		if (Data[i].x - dequeBuff[0] >= 0.000000)
		{
			_MinusCheck[i] = true; 
		}
		if (Data[i].x - dequeBuff[1] > 0.000000)
		{
			dequeBuff.pop_front();
			dequeBuff.pop_front();
		}
	}
	//Sleep(2);
}

void AnalysisConfigArguments::Analysis_TangentDividPeak
	(const vector<CPeak> &Peak, deque<PeakSepaArg> &dequeIdx)
{	//���贫��Ĳ����Ѿ����򣨼������źŴ���˳����ͬ���Ƶķ�һ��
	//�������еĲ������ҵ������߷ַ塱������
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();

	deque<PeakSepaArg> dequeBuffIdx;
	//ʱ���ϵ�ɸѡ
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		unsigned BgIdx = 0;
		unsigned EdIdx = 0;
		bool findBgIdx = false;
		bool findEdIdx = false;
		if (_ConfigArgument[i].ArgName == _T("���߷ַ�"))
		{
			for (unsigned spoint=0; spoint<PeakSize; ++spoint)//�����
			{
				if (Peak[spoint].PeakSema.x - _ConfigArgument[i].BeginTime >= 0.000000)
				{
					BgIdx = spoint;
					if (dequeBuffIdx.empty())
					{
						findBgIdx = true;
						break;
					}
					else if (!dequeBuffIdx.empty() && BgIdx > dequeBuffIdx[dequeBuffIdx.size()-1].FinishPeakNo)
					{
						findBgIdx = true;
						break;
					}
				}
			}
			for (int epoint=PeakSize-1; epoint>=0; --epoint)//���յ�
			{
				if (Peak[epoint].PeakSema.x - _ConfigArgument[i].EndTime <= 0.000000)
				{
					EdIdx = epoint;
					if (EdIdx!=BgIdx) //���Ե�һ�����߷ַ�
						findEdIdx = true;
					break;
				}
			}
	
		}
		if (findBgIdx && findEdIdx)
		{
			//std::pair<unsigned, unsigned> p = std::make_pair(BgIdx, EdIdx);
			AnalysisConfigArguments::PeakSepaArg tangConfig(BgIdx, EdIdx, (int)_ConfigArgument[i].InputValue);
			dequeBuffIdx.push_back(tangConfig);
		}
	}

	
	//�����͵�ɸѡ
	for (unsigned nowsize=0; nowsize<dequeBuffIdx.size(); ++nowsize)
	{
		int val = dequeBuffIdx[nowsize].SepaType;
		bool findBg = false;
		bool findEd = false;
		unsigned bg = 0;
		unsigned ed = 0;
		int pType = 0; //������
		
		for (; dequeBuffIdx[nowsize].StartPeakNo!=dequeBuffIdx[nowsize].FinishPeakNo+1;
			dequeBuffIdx[nowsize].StartPeakNo++)
		{
			if (abs(Peak[dequeBuffIdx[nowsize].StartPeakNo].Type)==1)//����
			{
				if (findBg)
				{
					ed = dequeBuffIdx[nowsize].StartPeakNo-1;
					//std::pair<unsigned, unsigned> p = std::make_pair(bg, ed);
					AnalysisConfigArguments::PeakSepaArg tangConfig(bg, ed, val);
					dequeIdx.push_back(tangConfig);
					findBg = false;
				}
			}
			else //������
			{
				if (findBg && (Peak[dequeBuffIdx[nowsize].StartPeakNo].Type != pType))
				{
					ed = dequeBuffIdx[nowsize].StartPeakNo-1;
					//std::pair<unsigned, unsigned> p = std::make_pair(bg, ed);
					AnalysisConfigArguments::PeakSepaArg tangConfig(bg, ed, val);
					dequeIdx.push_back(tangConfig);
					pType = Peak[dequeBuffIdx[nowsize].StartPeakNo].Type;
					findBg = false;	
				}
				if (!findBg)
				{
					bg = dequeBuffIdx[nowsize].StartPeakNo;
					pType = Peak[bg].Type;
					findBg = true;

				}
				if ((dequeBuffIdx[nowsize].StartPeakNo==dequeBuffIdx[nowsize].FinishPeakNo)
					&& (bg!=dequeBuffIdx[nowsize].FinishPeakNo)) //��β���ų�����
				{
					AnalysisConfigArguments::PeakSepaArg tangConfig(bg, dequeBuffIdx[nowsize].FinishPeakNo, val);
					dequeIdx.push_back(tangConfig);
				}
			}
		}
	}
	//Sleep(2);
}

VOID AnalysisConfigArguments::Analysis_PeakSeparation
	(const vector<CPeak> &Peak, vector<PeakSepaArg> &psArgs)
{//���ò����Ѿ�����Ԥ����ȫ�����仯�����������ص�����
	if (!psArgs.empty()) psArgs.clear();

	//Ĭ��ȫ�����Ǵ�ֱ�ַ�
	//ConfigArgument argument(_T("�ص��崦��"),0.0, (double)INT_MAX,1);
	//_ConfigArgument.push_back(argument);

	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();

	deque<PeakSepaArg> dequeBuffIdx;
	//ʱ���ϵ�ɸѡ
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		unsigned BgIdx = 0;
		unsigned EdIdx = 0;
		bool findBgIdx = false;
		bool findEdIdx = false;
		if (_ConfigArgument[i].ArgName == _T("�ص��崦��"))
		{
			for (unsigned spoint=0; spoint<PeakSize; ++spoint)//�����
			{
				if (Peak[spoint].PeakSema.x - _ConfigArgument[i].BeginTime >= 0.000000)
				{
					BgIdx = spoint;
					if (dequeBuffIdx.empty())
					{
						findBgIdx = true;
						break;
					}
					else if (!dequeBuffIdx.empty() && BgIdx > dequeBuffIdx[dequeBuffIdx.size()-1].FinishPeakNo)
					{
						findBgIdx = true;
						break;
					}
				}
			}
			for (int epoint=PeakSize-1; epoint>=0; --epoint)//���յ�
			{
				if (Peak[epoint].PeakSema.x - _ConfigArgument[i].EndTime <= 0.000000)
				{
					EdIdx = epoint;
					if (EdIdx!=BgIdx) //���Ե�һ�����߷ַ�
						findEdIdx = true;
					break;
				}
			}

		}
		if (findBgIdx && findEdIdx)
		{
			AnalysisConfigArguments::PeakSepaArg tangConfig(BgIdx, EdIdx, (int)_ConfigArgument[i].InputValue);
			dequeBuffIdx.push_back(tangConfig);
		}
	}


	//�����͵�ɸѡ
	for (unsigned nowsize=0; nowsize<dequeBuffIdx.size(); ++nowsize)
	{
		int val = dequeBuffIdx[nowsize].SepaType;
		bool findBg = false;
		bool findEd = false;
		unsigned bg = 0;
		unsigned ed = 0;
		int pType = 0; //������

		for (; dequeBuffIdx[nowsize].StartPeakNo!=dequeBuffIdx[nowsize].FinishPeakNo+1;
			dequeBuffIdx[nowsize].StartPeakNo++)
		{
			if (abs(Peak[dequeBuffIdx[nowsize].StartPeakNo].Type)==1)//����
			{
				if (findBg)
				{
					ed = dequeBuffIdx[nowsize].StartPeakNo-1;
					//std::pair<unsigned, unsigned> p = std::make_pair(bg, ed);
					AnalysisConfigArguments::PeakSepaArg tangConfig(bg, ed, val);
					psArgs.push_back(tangConfig);
					findBg = false;
				}
			}
			else //������
			{
				if (findBg && (Peak[dequeBuffIdx[nowsize].StartPeakNo].Type != pType))
				{
					ed = dequeBuffIdx[nowsize].StartPeakNo-1;
					//std::pair<unsigned, unsigned> p = std::make_pair(bg, ed);
					AnalysisConfigArguments::PeakSepaArg tangConfig(bg, ed, val);
					psArgs.push_back(tangConfig);
					pType = Peak[dequeBuffIdx[nowsize].StartPeakNo].Type;
					findBg = false;	
				}
				if (!findBg)
				{
					bg = dequeBuffIdx[nowsize].StartPeakNo;
					pType = Peak[bg].Type;
					findBg = true;
				}
				if ((dequeBuffIdx[nowsize].StartPeakNo==dequeBuffIdx[nowsize].FinishPeakNo)
					&& (bg!=dequeBuffIdx[nowsize].FinishPeakNo)) //��β���ų�����
				{
					AnalysisConfigArguments::PeakSepaArg tangConfig(bg, dequeBuffIdx[nowsize].FinishPeakNo, val);
					psArgs.push_back(tangConfig);
				}
			}
		}
	}
}

//�����ֹ��ַ�ķ��
VOID AnalysisConfigArguments::Analysis_HandPeakSeparation
	(const vector<CPeak> &Peak, deque<double> &HandPeakArgs)
{//��ǰ���ֹ��ַ�[��ʼʱ��]������һ���������������
 //�������������ֹ��ַ�[��ʼʱ��]����һ��������
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֹ��ַ�"))
		{
			//������ȷ��������ò���
			ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
				_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
			_ResultArgument.push_back(configArg);

			for (unsigned j=0; j<PeakSize; ++j)
			{//�ַ�����λ��һ���������������
				if ((Peak[j].FinSema.x - _ConfigArgument[i].BeginTime > 0.000000)
				  &&(_ConfigArgument[i].BeginTime - Peak[j].StartSema.x > 0.000000))
				{//ֱ�ӷ���������δ��ʱ������
					_HandPeakSpra.push_back(_ConfigArgument[i].BeginTime);
				}
			}
		}
	}
	HandPeakArgs.insert(HandPeakArgs.end(), _HandPeakSpra.begin(), _HandPeakSpra.end());
}

//�����ֹ��ַ���Ҫ��һ�����������ط������ڶ�Ӧ�ķ��±ꡣ-1��ʾ�Ҳ������������ķ��±�
int  AnalysisConfigArguments::Analysis_OneHandPeakSepar
	(const vector<CPeak> &Peak, double starttime)
{
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<PeakSize; ++i)
	{
		if ((Peak[i].FinSema.x - starttime > 0.000000)
			&&(starttime - Peak[i].StartSema.x > 0.000000))
		{
			return (int)i; //�ҵ���Ӧ�±�
		}
	}
	return -1; //not found
}


VOID AnalysisConfigArguments::Analysis_HandAdjustPeak
	(const vector<CPeak> &Peak)
{//��ǰ���ֹ�����[����ʱ��]������һ���������������
	//�������������ֹ�����[����ʱ��]�ֱ����3��������
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֹ�����"))
		{

			if (_ConfigArgument[i].InputValue == 11) //��������:���
			{
				//_AdjustStart.push_back(_ConfigArgument[i].BeginTime);
			}
			else if (_ConfigArgument[i].InputValue == 33) //��������:�յ�
			{
				//_AdjustFinish.push_back(_ConfigArgument[i].BeginTime);
			}
			else if (_ConfigArgument[i].InputValue == 22) //��������:���
			{//�������������жϸõ���ʱ���Ƿ���ĳ�����������
				for (unsigned j=0; j<PeakSize; ++j)
				{
					if((_ConfigArgument[i].BeginTime > Peak[j].StartSema.x)
					&&(_ConfigArgument[i].BeginTime < Peak[j].FinSema.x))
					{
						_AdjustPeak.push_back(_ConfigArgument[i].BeginTime);
						break;
					}
				}
			}
			//������ȷ��������ò���
			ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
				_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
			_ResultArgument.push_back(configArg);
		}	
	}
}

VOID AnalysisConfigArguments::Analysis_HandworkBaseLine
	(const vector<CPeak> &Peak) {

	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֹ�����"))
		{
			double matchingTime = _ConfigArgument[i].BeginTime;
			double targetTime = _ConfigArgument[i].EndTime;
			//�Ҹ�matchtime��ӽ���һ����
			double diff = (double)INT_MAX;
			unsigned srt_no = 0; // ȷ�������ķ��-���
			unsigned fin_no = 0; // ȷ�������ķ��-�յ�
			bool isStart = false;
			for (unsigned j=0; j<PeakSize; ++j)
			{
				double sx = Peak[j].StartSema.x;
				double fx = Peak[j].FinSema.x;

				if (fabs(matchingTime - sx)<=diff)
				{
					diff = fabs(matchingTime - sx);
					srt_no = j;
					isStart = true;
				}
				if (fabs(matchingTime - fx)<=diff)
				{
					diff = fabs(matchingTime - fx);
					fin_no = j;
					isStart = false;
				}
			}
			if (isStart)
				_AdjustStart.push_back(std::make_pair(srt_no, targetTime));
			else
				_AdjustFinish.push_back(std::make_pair(fin_no, targetTime));
		}
	}
}


//VOID AnalysisConfigArguments::Analysis_MergePeak
//	(const vector<CPeak> &Peak)
//{//�ϲ���������ж�������
// //�����ڱ�Ȼ���ڶ����߶������ϵķ�
// //�������������Ա����������
// //����������ȥ
//	const unsigned ArgsSize = _ConfigArgument.size();
//	const unsigned PeakSize = Peak.size();
//	for (unsigned ai=0; ai<ArgsSize; ++ai)
//	{
//		if (_ConfigArgument[ai].ArgName == "�ϲ���")
//		{
//			double start = _ConfigArgument[ai].BeginTime; //���
//			double end = _ConfigArgument[ai].EndTime;     //�յ�
//			unsigned sNo = INT_MAX; //�ϲ�������±�
//			unsigned eNo = INT_MAX; //�ϲ����յ��±�
//			//���������ȷ�����ڵ�N����֮ǰ
//			for (unsigned pi=0; pi<PeakSize; ++pi)
//			{
//				if (Peak[pi].PeakSema.x >= start) //�ж�������ֻҪ����������
//				{
//					sNo = pi;
//					break;
//				}
//			}
//			//�������յ�ȷ�����ڵ�N����֮��
//			for (signed pi=PeakSize-1; pi>=0; --pi)
//			{
//				if (Peak[pi].PeakSema.x <= end)
//				{
//					eNo = pi;
//					break;
//				}
//			}
//			//�Ժϲ����±��ж�
//			if (sNo!=INT_MAX && eNo!=INT_MAX)
//			{
//				unsigned diff = eNo - sNo;
//				if (diff >= 1)//�ϲ�������������������Ժϲ�
//				{
//					//������ȷ��������ò���
//					ConfigArgument configArg(_ConfigArgument[ai].ArgName, _ConfigArgument[ai].BeginTime, 
//						_ConfigArgument[ai].EndTime,_ConfigArgument[ai].InputValue);
//					_ResultArgument.push_back(configArg);
//
//					pair<double, double> buffPair = std::make_pair(start, end);
//					_MergePeak.push_back(buffPair);
//				}
//			}
//		}
//	}
//}

VOID AnalysisConfigArguments::Analysis_MergePeak
	(const vector<CPeak> &Peak)
{//�ϲ���������ж�������
	//�����ڱ�Ȼ���ڶ����߶������ϵķ�
	//�������ʼʱ�䣬����ʱ�䣩�ڰ���������͸��壬�����ܽ�����͸�����кϲ��������Ըû����¼���
	//�������������Ա����������
	//����������ȥ
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();
	for (unsigned ai=0; ai<ArgsSize; ++ai)
	{
		if (_ConfigArgument[ai].ArgName == _T("�ϲ���"))
		{
			double start = _ConfigArgument[ai].BeginTime; //���
			double end = _ConfigArgument[ai].EndTime;     //�յ�
			unsigned sNo = INT_MAX; //�ϲ�������±�
			unsigned eNo = INT_MAX; //�ϲ����յ��±�
			//���������ȷ�����ڵ�N����֮ǰ
			for (unsigned pi=0; pi<PeakSize; ++pi)
			{
				if (Peak[pi].PeakSema.x >= start) //�ж�������ֻҪ����������
				{
					sNo = pi;
					break;
				}
			}
			//�������յ�ȷ�����ڵ�N����֮��
			for (signed pi=PeakSize-1; pi>=0; --pi)
			{
				if (Peak[pi].PeakSema.x <= end)
				{
					eNo = pi;
					break;
				}
			}
			//�Ժϲ����±��ж�
			if (sNo!=INT_MAX && eNo!=INT_MAX)
			{
				unsigned diff = eNo - sNo;
				if (diff >= 1)//�ϲ�������������������Ժϲ�
				{
					bool isPlus = abs(Peak[sNo].Type)==Peak[sNo].Type? true : false;
					for (unsigned i=sNo+1; i<=eNo; ++i)
					{
						bool isClear;
						if (isPlus)
							isClear = abs(Peak[i].Type)==Peak[i].Type?  true: false;
						else
							isClear = abs(Peak[i].Type)!=Peak[i].Type?  true: false;

						if (!isClear) break;

						if (i==eNo)
						{
							//������ȷ��������ò���
							//ConfigArgument configArg(_ConfigArgument[ai].ArgName, _ConfigArgument[ai].BeginTime, 
							//	_ConfigArgument[ai].EndTime,_ConfigArgument[ai].InputValue);
							//_ResultArgument.push_back(configArg);

							pair<unsigned, unsigned> bufPair = std::make_pair(sNo, eNo);
							_MergePeakNo.push_back(bufPair);
						}
					}
				}
			}
		}
	}
}

VOID AnalysisConfigArguments::Analysis_HandAddPeak(const vector<CPeak> &Peak)
{//�ֹ����ӷ崦����̣�
 //1�����ȶԲ�������һ�飬��֤û���ص����䣬��������ص����䣬����������
 //2���������õ��Ĳ����Ա��Ѿ����ֵĲ������䣬����������ص����䣬�������ò���

	//�����ӷ���������ò�������
	deque<ConfigArgument> addArgu; //���ӷ����
	deque<ConfigArgument> othArgu; //��������

	const unsigned ArgsSize = _ConfigArgument.size();
	for (unsigned ai=0; ai<ArgsSize; ++ai)
	{
		if (_ConfigArgument[ai].ArgName == _T("���ӷ�"))
			addArgu.push_back(_ConfigArgument[ai]);
		else
			othArgu.push_back(_ConfigArgument[ai]);
	}

	//1,�����ӷ��������
	deque<ConfigArgument> NoOverlapArgs; //���ص�����Ĳ���
	if (addArgu.empty()) return;
	const unsigned addArgsSize = addArgu.size();
	NoOverlapArgs.push_back(addArgu[0]); 
	for (unsigned i=0; i<addArgsSize; ++i)
	{
		bool isPutIn = true;
		for (unsigned j=0; j<NoOverlapArgs.size(); ++j)
		{
			double dA = addArgu[i].BeginTime;
			double dB = addArgu[i].EndTime;
			double dC = NoOverlapArgs[j].BeginTime;
			double dD = NoOverlapArgs[j].EndTime;
		
			if(IsOverlapping(dA,dB,dC,dD))
			{
				isPutIn = false;
				break;
			}
		}
		if (isPutIn)
		{
			NoOverlapArgs.push_back(addArgu[i]);
		}
		
	}

	//2,�������õ��Ĳ����Ա��Ѿ����ֵĲ������䣬����������ص����䣬�������ò���
	_AddPeak.clear();  //��գ�װ�����շ��������Ĳ���
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<NoOverlapArgs.size(); ++i)
	{
		bool isPutIn = true;
		for (unsigned j=0; j<PeakSize; ++j)
		{
			double dA = Peak[j].StartSema.x;
			double dB = Peak[j].FinSema.x;
			double dC = NoOverlapArgs[i].BeginTime;
			double dD = NoOverlapArgs[i].EndTime;

			if (IsOverlapping(dA,dB,dC,dD))
			{
				isPutIn = false;
				break;
			}
		}
		if (isPutIn)
		{
			_AddPeak.push_back(NoOverlapArgs[i]);

			//������ȷ��������ò���
			_ResultArgument.push_back(NoOverlapArgs[i]);
		}
	}
}

VOID AnalysisConfigArguments::Analysis_HandDelPeak(const vector<CPeak> &Peak){
	
	const unsigned ArgsSize = _ConfigArgument.size();
	for (unsigned ai=0; ai<ArgsSize; ++ai)
	{
		if (_ConfigArgument[ai].ArgName == _T("ɾ����"))
			_DelPeak.push_back(_ConfigArgument[ai]);
	} 
}

VOID AnalysisConfigArguments::Analysis_AddelPeak(const vector<CPeak> &Peak){

	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();

	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("���ӷ�"))
		{
			{
				_AddelPeak.push_back(_ConfigArgument[i]);

				//������ȷ��������ò���
				_ResultArgument.push_back(_ConfigArgument[i]);
			}

		}
		if (_ConfigArgument[i].ArgName == _T("ɾ����"))
			_AddelPeak.push_back(_ConfigArgument[i]);
	}
}

BOOL AnalysisConfigArguments::IsOverlapping(double dA, double dB, double dC, double dD)
{
	if (dA > dB)
	{
		double tmp = dB;
		dB = dA;
		dA = tmp;
	}

	if (dC > dD)
	{
		double tmp = dD;
		dD = dC;
		dC = tmp;
	}

	if ((dC >= dB) || (dD <= dA))
		return FALSE;
	else
		return TRUE;
}


//�����ֹ��ⶥ�����ӷ壩��Ҫ�Ĳ���
/*VOID AnalysisConfigArguments::Analysis_HandAddPeak
	(const vector<CPeak> &Peak)
{*///��ǰ���ֹ��嶥[��ʼʱ�䣬����ʱ��]������һ���������������
	//��������һ�㲻�ܱ�����������һ�����������
	//�������������ֹ��ַ�[��ʼʱ�䣬����ʱ��]����һ����Ա���������ڣ�vector<pair<double, double> >��
	//ȱ�ٶ��ظ�������жϣ�
//	const unsigned ArgsSize = _ConfigArgument.size();
//	const unsigned PeakSize = Peak.size();
//	for (unsigned i=0; i<ArgsSize; ++i)
//	{
//		bool startOK = true;
//		bool finishOK = true;
//		if (_ConfigArgument[i].ArgName == "�ֹ��ⶥ")
//		{
//			for (unsigned j=0; j<PeakSize; ++j)
//			{
//				if ((_ConfigArgument[i].BeginTime > Peak[j].StartSema.x)
//					&&(Peak[j].FinSema.x > _ConfigArgument[i].BeginTime)) //�ⶥ���λ��ĳ����������
//				{
//					startOK = false;
//					break;
//				}
//			}
//			if (startOK)
//			{
//				for (unsigned k=0; k<PeakSize; ++k)
//				{
//					if ((_ConfigArgument[i].EndTime > Peak[k].StartSema.x)
//						&&(Peak[k].FinSema.x > _ConfigArgument[i].EndTime)) //�ⶥ�յ�λ��ĳ����������
//					{
//						finishOK = false;
//						break;
//					}
//				}
//			}
//			if (startOK && finishOK)
//			{
//				std::pair<double,double> buffpair = std::make_pair(_ConfigArgument[i].BeginTime, 
//																_ConfigArgument[i].EndTime);
//				_AddPeak.push_back(buffpair);
//			}
//		}
//	}
//}

//������ֹ���ֵķ��
VOID AnalysisConfigArguments::Analysis_BanPeak
	(const vector<CPeak> &Peak)
{
	//����Ĳ����Ѿ�����,Ԥ����
	//�������еĲ������ҵ�����ֹ���֡�������
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("��ֹ����"))
		{
			//������ȷ��������ò���
			ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
				_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
			_ResultArgument.push_back(configArg);

			for (unsigned j=0; j<PeakSize; ++j)
			{//ֻҪ�嶥������ʱ��������
				if ((Peak[j].PeakSema.x - _ConfigArgument[i].BeginTime > 0.000000)
					&&(_ConfigArgument[i].EndTime - Peak[j].PeakSema.x > 0.000000))
				{
					_BanPeakNo.push_back(j);
				}
			}
		}
	}
}

VOID AnalysisConfigArguments::Analysis_MinHalfPeakWidth
	(const vector<CPeak> &Peak)
{
	//����Ĳ����Ѿ�����,Ԥ����
	//�������еĲ������ҵ����ֶ���С����������
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֶ���С����"))
		{
			for (unsigned j=0; j<PeakSize; ++j)
			{//ֻҪ�嶥������ʱ��������
				if ((Peak[j].PeakSema.x - _ConfigArgument[i].BeginTime > 0.000000)
					&&(_ConfigArgument[i].EndTime - Peak[j].PeakSema.x > 0.000000))
				{
					_MinHalfWidth[j] = _ConfigArgument[i].InputValue; //�ı�Ĭ��ֵ
				}
				//���ı�Ĭ��ֵ
			}
		}
	}
}

VOID AnalysisConfigArguments::Analysis_MinPeakArea
	(const vector<CPeak> &Peak)
{
	//����Ĳ����Ѿ�����,Ԥ����
	//�������еĲ������ҵ����ֶ���С�������������
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();
	if (_MinPeakArea.size() != PeakSize)
	{
		ASSERT(_MinPeakArea.size() != PeakSize);
	}
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֶ���С�����"))
		{
			

			ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
				_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
			_ResultArgument.push_back(configArg);

			for (unsigned j=0; j<PeakSize; ++j)
			{//ֻҪ�嶥������ʱ��������
				if ((Peak[j].PeakSema.x - _ConfigArgument[i].BeginTime > 0.000000)
					&&(_ConfigArgument[i].EndTime - Peak[j].PeakSema.x > 0.000000))
				{
					_MinPeakArea[j] = _ConfigArgument[i].InputValue; //�ı�Ĭ��ֵ
				}
			}
		}
	}
}

VOID AnalysisConfigArguments::Analysis_MinPeakHeight
	(const vector<CPeak> &Peak) 
{
	const unsigned ArgsSize = _ConfigArgument.size();
	const unsigned PeakSize = Peak.size();
	if (_MinPeakHeight.size() != PeakSize)
	{
		ASSERT(_MinPeakHeight.size() != PeakSize);
	}
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == _T("�ֶ���С���"))
		{

			ConfigArgument configArg(_ConfigArgument[i].ArgName, _ConfigArgument[i].BeginTime, 
				_ConfigArgument[i].EndTime,_ConfigArgument[i].InputValue);
			_ResultArgument.push_back(configArg);

			for (unsigned j=0; j<PeakSize; ++j)
			{//ֻҪ�嶥������ʱ��������
				if ((Peak[j].PeakSema.x - _ConfigArgument[i].BeginTime > 0.000000)
					&&(_ConfigArgument[i].EndTime - Peak[j].PeakSema.x > 0.000000))
				{
					_MinPeakHeight[j] = _ConfigArgument[i].InputValue; //�ı�Ĭ��ֵ
				}
			}
		}
	}
}


//---------------------------------------------------------------------------------
// ��ø���ɨ������Ҫ�Ĳ���
//---------------------------------------------------------------------------------
vector<double>& AnalysisConfigArguments::GetPeakSearchArgs()
{ return _SlopeBased; }

deque<bool>& AnalysisConfigArguments::GetMinusCheckArgs()
{ return _MinusCheck; }

vector<unsigned>& AnalysisConfigArguments::GetBanPeakArgs()
{ return _BanPeakNo; }

vector<double>& AnalysisConfigArguments::GetMinHalfPeakWidthArgs()
{ return _MinHalfWidth; }

vector<double>& AnalysisConfigArguments::GetMinPeakAreaArgs()
{ return _MinPeakArea; }

vector<double>& AnalysisConfigArguments::GetMinPeakHeightArgs()
{ return _MinPeakHeight; }

vector<double>& AnalysisConfigArguments::GetHandAdjustPeakArgs()
{ return _AdjustPeak; }

vector<pair<unsigned, double> >& AnalysisConfigArguments::GetHandAdjustStartArgs()
{ return _AdjustStart; }

vector<pair<unsigned, double> >& AnalysisConfigArguments::GetHandAdjustFinishArgs()
{ return _AdjustFinish; }

vector<pair<double, double> >& AnalysisConfigArguments::GetMergePeakArgs()
{ return _MergePeak; }

vector<pair<unsigned, unsigned> >& AnalysisConfigArguments::GetMergePeakNo()
{ return _MergePeakNo; }

deque<ConfigArgument>& AnalysisConfigArguments::GetAddPeakArgs()
{ return _AddPeak; }

deque<ConfigArgument>& AnalysisConfigArguments::GetDelPeakArgs()
{ return _DelPeak; }

deque<ConfigArgument>& AnalysisConfigArguments::GetAddDelPeakArgs()
{ return _AddelPeak; }

bool AnalysisConfigArguments::GetMinusCheakStatus(double point)
{
	//�������еĲ������ҵ����ֶθ����⡱������
	const unsigned ArgsSize = _ConfigArgument.size();
	deque<double> dequeBuff;
	double lastTime = -INT_MAX;
	for (unsigned i=0; i<ArgsSize; ++i)
	{
		if (_ConfigArgument[i].ArgName == "�ֶθ�����")
		{//�ص�����Ĵ���
			if (_ConfigArgument[i].BeginTime - lastTime >= 0.000000 )
			{
				dequeBuff.push_back(_ConfigArgument[i].BeginTime);
				dequeBuff.push_back(_ConfigArgument[i].EndTime);
			}
			lastTime = _ConfigArgument[i].EndTime;
		}
	}
	if (dequeBuff.empty()) return false;
	while(!dequeBuff.empty())
	{
		if ((dequeBuff[0]<=point)
			&(dequeBuff[1]>=point))
		{
			return true;
		}
		dequeBuff.pop_front();
		dequeBuff.pop_front();
	}
	return false;
}

deque<ConfigArgument>& AnalysisConfigArguments::GetAllArguments()
{ return _ConfigArgument; }

deque<ConfigArgument>& AnalysisConfigArguments::GetResultArgument()
{ return _ResultArgument; }

double& AnalysisConfigArguments::GetDefaultPeak2Valley()
{ return _defaultPeak2Valley; }

bool AnalysisConfigArguments::IsPeak2Valley()
{
	bool bResult = true;
	int iCount = 0;
	const unsigned ArgsSize = _ConfigArgument.size();
	for (unsigned ai=0; ai<ArgsSize; ++ai)
	{
		if (_ConfigArgument[ai].ArgName == "�ص��崦��")
		{
			iCount++;
			if (iCount >= 2) //��Ϊ��1��������������Ϊȫ���ߴ�ֱ�ַ�ģ������ٳ���һ������������÷�ȱ�
			{
				bResult = false;
				break;
			}
		}
	}
	return bResult;
}
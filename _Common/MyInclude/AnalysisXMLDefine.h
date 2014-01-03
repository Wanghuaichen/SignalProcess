#pragma once

//------------------------------------------------------------------------|
//------------------------------------------------------------------------|
//----------------------  ����ϵͳXML(�ļ�) ����� ------------------------|
//------------------------------------------------------------------------|
//------------------------------------------------------------------------|

//
//-------------------------------------------------------------------------
//	���ݸ����ƶ���
//-------------------------------------------------------------------------
#define		XMLROOT_INSTRUMENT		_T("Instrument")
#define		XMLROOT_TASK			_T("Task")
//#define	XMLROOT_METHOD			_T("Method")
//#define	XMLROOT_SAMPLE			_T("Sample")
//#define	XMLROOT_ACQUIRER		_T("Acquirer")
#define		XMLROOT_SYSSETTINGS		_T("SysSettings")



//-------------------------------------------------------------------------
//---------------------------�������ļ��ṹ��·����-------------------------
//-------------------------------------------------------------------------
#define	XMLROOT_INJECT				_T("Inject")
#define XMLPATH_INJECT_INLETVALVE	_T("Inject/InletValve")	//����·������������


//-------------------------------------------------------------------------
//---------------------------�����ļ��ṹ��·����---------------------------
//-------------------------------------------------------------------------
//Method(Root)
#define	XMLROOT_METHOD							_T("Method")

//Method(Root)-->��������
#define XMLPATH_METHOD_METHODID					_T("Method/MethodID")			//����ID
#define XMLPATH_METHOD_METHODNAME				_T("Method/MethodName")			//��������
#define XMLPATH_METHOD_SYSTEM					_T("Method/System")				//����ϵͳ
#define XMLPATH_METHOD_APP						_T("Method/Application")		//��������

//Method(Root)-->��Ʒȱʡ����
#define XMLPATH_METHOD_SAMPLEID					_T("Method/SampleID")			//��Ʒȱʡ
#define XMLPATH_METHOD_SAMPLENAME				_T("Method/SampleName")			//��Ʒȱʡ
#define XMLPATH_METHOD_SAMPLEPURPOSE			_T("Method/SamplePurpose")		//��Ʒ���ͣ�������������
#define XMLPATH_METHOD_SAMPLEINJECTNO			_T("Method/SampleInjectNO")		//��Ʒ������

//Method(Root)-->����
#define XMLPATH_METHOD_PARAM					_T("Method/Parameter")   
//Methodϵͳ�Ƽ�����
#define XMLPATH_METHOD_PARAM_SYSTEM_THRESHOLD			_T("Method/Parameter/PeakProcess/Threshold")  
#define XMLPATH_METHOD_PARAM_SYSTEM_MINHALFPEAKWIDTH	_T("Method/Parameter/PeakProcess/MinHalfPeakWidth")  
#define XMLPATH_METHOD_PARAM_SYSTEM_MINPEAKAREA			_T("Method/Parameter/PeakProcess/MinPeakArea")  
#define XMLPATH_METHOD_PARAM_SYSTEM_HEIGHT				_T("Method/Parameter/PeakProcess/Height")         
//Method(Root)-->����-->��Ʒȱʡ��Ϣ
#define XMLPATH_METHOD_PARAM_SAMPLE				_T("Method/Parameter/Information")  

//Method(Root)-->����-->��������
#define XMLPATH_METHOD_PARAM_INSTRUMENT			_T("Method/Parameter/Instrument")

//Method(Root)-->����-->��������-->��������-->�ź�ͨ��
#define XMLPATH_METHOD_PARAM_INSTRU_SC			_T("Method/Parameter/Instrument/Acquisition/SignalChannel")
//Method(Root)-->����-->��������-->��������-->����ͨ��
#define XMLPATH_METHOD_PARAM_INSTRU_IC			_T("Method/Parameter/Instrument/Acquisition/InjectionChannel")
//Method(Root)-->����-->��������-->��������-->����ʱ��
#define XMLPATH_METHOD_PARAM_INSTRU_AT			_T("Method/Parameter/Instrument/Acquisition/AcquisitionTime")

//Method(Root)-->����-->У������
#define XMLPATH_METHOD_PARAM_CALIBRATE			_T("Method/Parameter/Calibrate")
//Method(Root)-->����-->У������-->У������
#define XMLPATH_METHOD_PARAM_CALI_METHOD		_T("Method/Parameter/Calibrate/CalibrateMethod")
//Method(Root)-->����-->У������-->У������
#define XMLPATH_METHOD_PARAM_CALI_OBJEC			_T("Method/Parameter/Calibrate/CalibrateObject")
//Method(Root)-->����-->У������-->У����
#define XMLPATH_METHOD_PARAM_CALI_TABLE			_T("Method/Parameter/Calibrate/CalibrateTable")
//Sample(Root)-->����-->У������-->У�������
#define XMLPATH_METHOD_PARAM_CALI_RESULT_TABLE	_T("Method/Parameter/Calibrate/CalibrateResultTable")
//Method(Root)-->����-->У������-->У����ݱ�
#define XMLPATH_METHOD_PARAM_CALI_COMPONENTTAB	_T("Method/Parameter/Calibrate/CalibrateTable_Component")
//Method(Root)-->����-->�崦�����
#define XMLPATH_METHOD_PARAM_PEAKPROCESS		_T("Method/Parameter/PeakProcess")
//Method(Root)-->����-->�崦�����-->��������
#define XMLPATH_METHOD_PARAM_PEAKPROC_TRD		_T("Method/Parameter/PeakProcess/Threshold")
//Method(Root)-->����-->�崦�����-->��С����
#define XMLPATH_METHOD_PARAM_PEAKPROC_MPW		_T("Method/Parameter/PeakProcess/MinHalfPeakWidth")
//Method(Root)-->����-->�崦�����-->��С�����
#define XMLPATH_METHOD_PARAM_PEAKPROC_MPA		_T("Method/Parameter/PeakProcess/MinPeakArea")
//Method(Root)-->����-->�崦�����-->�崦�������
#define XMLPATH_METHOD_PARAM_PEAKPROC_PPT		_T("Method/Parameter/PeakProcess/PPTimeTable")

//Methor(Root)-->����-->У���趨����
#define XMPPATH_METHOD_PARAM_CALI_SAMPLEAMOUNT   _T("Method/Parameter/Calibrate/SampleAmount")
#define XMPPATH_METHOD_PARAM_CALI_SAMPLEUNIT     _T("Method/Parameter/Calibrate/SampleUnit")
#define XMPPATH_METHOD_PARAM_CALI_PRODUCTFACTOR  _T("Method/Parameter/Calibrate/ProductFactor")
#define XMPPATH_METHOD_PARAM_CALI_DILUTIONFACTOR _T("Method/Parameter/Calibrate/DilutionFactor")
#define XMPPATH_METHOD_PARAM_CALI_CURVERFITTYPE  _T("Method/Parameter/Calibrate/CurverFitType")
#define XMPPATH_METHOD_PARAM_CALI_ORIGTREATMENT  _T("Method/Parameter/Calibrate/OrigTreatment")
#define XMPPATH_METHOD_PARAM_CALI_CALIWEIGHTING  _T("Method/Parameter/Calibrate/CaliWeighting")	
#define XMPPATH_METHOD_PARAM_CALI_REFERENCEPEAK  _T("Method/Parameter/Calibrate/ReferencePeak")	
#define XMPPATH_METHOD_PARAM_CALI_OTHERCALIPEAK  _T("Method/Parameter/Calibrate/OtherCaliPeak")	


//Method(Root)-->����
#define XMLPATH_METHOD_DATA						_T("Method/Data")          
//Method(Root)-->����-->��������
#define XMLPATH_METHOD_DATA_CURVE				_T("Method/CurveDatas")          

//Method(Root)-->���
#define XMLPATH_METHOD_RESULT					_T("Method/Result")          
//Method(Root)-->���-->���б�
#define XMLPATH_METHOD_RESULT_PEAKLIST			_T("Method/Result/PeakListTable")          
//Method(Root)-->���-->�����
#define XMLPATH_METHOD_RESULT_RESULT			_T("Method/Result/ResultTable")   
//Method(Root)-->���-->��ӡ·��
#define XMLPATH_METHOD_RESULT_PRINTPIC			_T("Method/Result/PrintPic")

//Method(Root)-->����
#define XMLPATH_METHOD_REPORT					_T("Method/Report")          


//#define XMLPATH_METHOD_INMS_QTABLE		_T("Method/Calibrate/INMS_Table")				//��ҵ���׶�����
//#define XMLPATH_METHOD_INMS_ST_BK		_T("Method/Calibrate/INMS_StandardSample_BK")	//��ҵ���ױ���У��
//#define XMLPATH_METHOD_INMS_ST_FG		_T("Method/Calibrate/INMS_StandardSample_FG")	//��ҵ������ƬУ��
//#define XMLPATH_METHOD_INMS_ST_SN		_T("Method/Calibrate/INMS_StandardSample_SN")	//��ҵ����������У��
//#define XMLPATH_METHOD_CALIBRATE_COMPOUND	_T("Method/Calibrate/Compound")	
//#define XMLPATH_METHOD_INSTRUMENTPARAM  _T("Method/Instrument/InstrumentParam")
//#define XMLPATH_METHOD_CALIBRATE_SCALE  _T("Method/Calibrate/Scale")


//------------------------------------------------------------------------+
//-------------------------- ��Ʒ�ļ��ṹ(·��) ---------------------------|
//------------------------------------------------------------------------+
//Sample(Root)
#define	XMLROOT_SAMPLE						_T("Sample")


//#define XMLPATH_SAMPLE_DRAWTYPE		_T("Sample/DrawType")

//Sample(Root)-->��������
#define XMLPATH_SAMPLE_METHODID				_T("Sample/MethodID")			//����ID
#define XMLPATH_SAMPLE_METHODNAME			_T("Sample/MethodName")			//��������
#define XMLPATH_SAMPLE_SYSTEM				_T("Sample/System")				//����ϵͳ
#define XMLPATH_SAMPLE_APP					_T("Sample/Application")		//��������

//Sample(Root)-->��Ʒ����
#define XMLPATH_SAMPLE_SAMPLEID				_T("Sample/SampleID")			//
#define XMLPATH_SAMPLE_SAMPLENAME			_T("Sample/SampleName")			//

#define XMLPATH_SAMPLE_SAMPLEPURPOSE		_T("Sample/SamplePurpose")		//��Ʒ���ͣ�������������
#define XMLPATH_SAMPLE_SAMPLEINJECTNO		_T("Sample/SampleInjectNO")		//��Ʒ������
#define XMLPATH_SAMPLE_PARAM_INFO_OPERATOR			_T("Sample/Parameter/Information/Operator")
#define XMLPATH_SAMPLE_PARAM_INFO_SENDER			_T("Sample/Parameter/Information/Sender")
#define XMLPATH_SAMPLE_PARAM_INFO_SENDTIME			_T("Sample/Parameter/Information/SendTime")
#define XMLPATH_SAMPLE_PARAM_INFO_ANALYSISTIME		_T("Sample/Parameter/Information/AnalysisTime")
#define XMLPATH_SAMPLE_PARAM_INFO_WEIGHT			_T("Sample/Parameter/Information/Weight")
#define XMLPATH_SAMPLE_PARAM_INFO_INTERNALWEIGHT    _T("Sample/Parameter/Information/InternalWeight")
#define XMLPATH_SAMPLE_PARAM_INFO_INJECTVOLUME      _T("Sample/Parameter/Information/InjectVolume")
#define XMLPATH_SAMPLE_PARAM_INFO_NOTE			    _T("Sample/Parameter/Information/note")
#define XMLPATH_SAMPLE_PARAM_INFO_TESTER			_T("Sample/Parameter/Information/Tester")
#define XMLPATH_SAMPLE_PARAM_INFO_SAMPLEAMOUNT      _T("Sample/Parameter/Information/SampleAmount")


//Sample(Root)-->����
#define XMLPATH_SAMPLE_PARAM					_T("Sample/Parameter")          
//Sample(Root)-->����-->��Ʒȱʡ��Ϣ
#define XMLPATH_SAMPLE_PARAM_SAMPLE				_T("Sample/Parameter/Information")  

//Sample(Root)-->����-->��������
#define XMLPATH_SAMPLE_PARAM_INSTRUMENT			_T("Sample/Parameter/Instrument")
//Sample(Root)-->����-->��������-->��������-->�ź�ͨ��
#define XMLPATH_SAMPLE_PARAM_INSTRU_SC			_T("Sample/Parameter/Instrument/Acquisition/SignalChannel")
//Sample(Root)-->����-->��������-->��������-->����ͨ��
#define XMLPATH_SAMPLE_PARAM_INSTRU_IC			_T("Sample/Parameter/Instrument/Acquisition/InjectionChannel")
//Sample(Root)-->����-->��������-->��������-->����ʱ��
#define XMLPATH_SAMPLE_PARAM_INSTRU_AT			_T("Sample/Parameter/Instrument/Acquisition/AcquisitionTime")

#define	XMLPATH_SAMPLE_PARAM_INSTRU_MF_PROGTAB	_T("Sample/Parameter/Instrument/MainFrame/Temp_ProgramTable")
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_TCB      _T("Sample/Parameter/Instrument/MainFrame/Temp_ColumnBox")
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_TRD      _T("Sample/Parameter/Instrument/MainFrame/Temp_RearDectetor")
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_TFI      _T("Sample/Parameter/Instrument/MainFrame/Temp_FrontInjector")
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_TPI      _T("Sample/Parameter/Instrument/MainFrame/Temp_ProgramInit")
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_TPIH     _T("Sample/Parameter/Instrument/MainFrame/Temp_ProgramInitHold")
#define XMLPATH_SAMPLE_PARAM_INSTRU_PUMP_FPTAB			_T("Sample/Parameter/Instrument/Pump/Flow_ProgramTable")
#define XMLPATH_SAMPLE_PARAM_INSTRU_PUMP_INITFLOW       _T("Sample/Parameter/Instrument/Pump/InitFLow")
#define XMLPATH_SAMPLE_PARAM_INSTRU_UVD_WL				_T("Sample/Parameter/Instrument/UVDetector/WaveLength")

//Sample(Root)-->����-->У������
#define XMLPATH_SAMPLE_PARAM_CALIBRATE			_T("Sample/Parameter/Calibrate")
//Sample(Root)-->����-->У������-->У������
#define XMLPATH_SAMPLE_PARAM_CALI_METHOD		_T("Sample/Parameter/Calibrate/CalibrateMethod")
//Sample(Root)-->����-->У������-->У������
#define XMLPATH_SAMPLE_PARAM_CALI_OBJECT		_T("Sample/Parameter/Calibrate/CalibrateObject")
//Sample(Root)-->����-->У������-->У�����̱�
#define XMLPATH_SAMPLE_PARAM_CALI_TABLE			_T("Sample/Parameter/Calibrate/CalibrateTable")
//Sample(Root)-->����-->У������-->У�������
#define XMLPATH_SAMPLE_PARAM_CALI_RESULT_TABLE	_T("Sample/Parameter/Calibrate/CalibrateResultTable")
//Sample(Root)-->����-->У������-->У����ݱ�
#define XMLPATH_SAMPLE_PARAM_CALI_COMPONENTTAB	_T("Sample/Parameter/Calibrate/CalibrateTable_Component")

//Sample(Root)-->����-->У���趨����
#define XMPPATH_SAMPLE_PARAM_CALI_SAMPLEAMOUNT   _T("Sample/Parameter/Calibrate/SampleAmount")
#define XMPPATH_SAMPLE_PARAM_CALI_SAMPLEUNIT     _T("Sample/Parameter/Calibrate/SampleUnit")
#define XMPPATH_SAMPLE_PARAM_CALI_PRODUCTFACTOR  _T("Sample/Parameter/Calibrate/ProductFactor")
#define XMPPATH_SAMPLE_PARAM_CALI_DILUTIONFACTOR _T("Sample/Parameter/Calibrate/DilutionFactor")
#define XMPPATH_SAMPLE_PARAM_CALI_CURVERFITTYPE  _T("Sample/Parameter/Calibrate/CurverFitType")
#define XMPPATH_SAMPLE_PARAM_CALI_ORIGTREATMENT  _T("Sample/Parameter/Calibrate/OrigTreatment")
#define XMPPATH_SAMPLE_PARAM_CALI_CALIWEIGHTING  _T("Sample/Parameter/Calibrate/CaliWeighting")	
#define XMPPATH_SAMPLE_PARAM_CALI_REFERENCEPEAK  _T("Sample/Parameter/Calibrate/ReferencePeak")	
#define XMPPATH_SAMPLE_PARAM_CALI_OTHERCALIPEAK  _T("Sample/Parameter/Calibrate/OtherCaliPeak")	


//Sample(Root)-->����-->�崦�����
#define XMLPATH_SAMPLE_PARAM_PEAKPROCESS		_T("Sample/Parameter/PeakProcess")
//Sample(Root)-->����-->�崦�����-->CP����(��������)
#define XMLPATH_SAMPLE_PARAM_PEAKPROC_CP        _T("Sample/Parameter/PeakProcess/CPcoefficient")
#define XMLPATH_PEAKPROC_CP						_T("PeakProcess/CPcoefficient")
//Sample(Root)-->����-->�崦�����-->AP����(��������)
#define XMLPATH_SAMPLE_PARAM_PEAKPROC_AP        _T("Sample/Parameter/PeakProcess/APcoefficient")
#define XMLPATH_PEAKPROC_AP						_T("PeakProcess/APcoefficient")
//Sample(Root)-->����-->�崦�����-->��������
#define XMLPATH_SAMPLE_PARAM_PEAKPROC_TRD		_T("Sample/Parameter/PeakProcess/Threshold")
#define XMLPATH_PEAKPROC_TRD					_T("PeakProcess/Threshold")
//Sample(Root)-->����-->�崦�����-->��С����
#define XMLPATH_SAMPLE_PARAM_PEAKPROC_MPW		_T("Sample/Parameter/PeakProcess/MinHalfPeakWidth")
#define XMLPATH_PEAKPROC_MPW					_T("PeakProcess/MinHalfPeakWidth")
//Sample(Root)-->����-->�崦�����-->��С�����
#define XMLPATH_SAMPLE_PARAM_PEAKPROC_MPA		_T("Sample/Parameter/PeakProcess/MinPeakArea")
#define XMLPATH_PEAKPROC_MPA					_T("PeakProcess/MinPeakArea")
//Sample(Root)-->����-->�崦�����-->��С���
#define XMLPATH_SAMPLE_PARAM_PEAKPROC_MPH	    _T("Sample/Parameter/PeakProcess/Height")
#define XMLPATH_PEAKPROC_MPH					_T("PeakProcess/Height")
//Sample(Root)-->����-->�崦�����-->�崦�������
#define XMLPATH_SAMPLE_PARAM_PEAKPROC_PPT		_T("Sample/Parameter/PeakProcess/PPTimeTable")
#define XMLPATH_PEAKPROC_PPT					_T("PeakProcess/PPTimeTable")
//Sample(Root)-->����-->����-->Ĭ�ϱ���
#define XMLPATH_SAMPLE_PARAM_REPORT				_T("Sample/Parameter/Report")
#define XMLPATH_SAMPLE_REPORTNAME			    _T("Sample/Parameter/Report/ReportDefault")

//Sample(Root)-->����
#define XMLPATH_SAMPLE_DATA						_T("Sample/Data")          
//Sample(Root)-->����-->��������
#define XMLPATH_SAMPLE_DATA_CURVE				_T("Sample/Data/CurveDatas")          

//Sample(Root)-->���
#define XMLPATH_SAMPLE_RESULT					_T("Sample/Result")          
//Sample(Root)-->���-->���б�
#define XMLPATH_SAMPLE_RESULT_PEAKLIST			_T("Sample/Result/PeakListTable")          
//Sample(Root)-->���-->�����
#define XMLPATH_SAMPLE_RESULT_RESULT			_T("Sample/Result/ResultTable")   
//Sample(Root)-->���-->��ӡ·��
#define XMLPATH_SAMPLE_RESULT_PRINTPIC			_T("Sample/Result/PrintPic")  

//Sample(Root)-->����
#define XMLPATH_SAMPLE_REPORT					_T("Sample/Report")          

//�û��Զ��������ֶ�
#define XMLPATH_SAMPLE_REPORT_RT				_T("Sample/Parameter/Report/ReportTitle")
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_MFM      _T("Sample/Parameter/Instrument/MainFrame/MainFrameModel")
#define XMLPATH_SAMPLE_PARAM_INSTRUMENT_IM		_T("Sample/Parameter/Instrument/InjectionMethod")
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_TI		_T("Sample/Parameter/Instrument/MainFrame/Temp_Injector")
#define XMLPATH_SAMPLE_PARAM_INFO_IV			_T("Sample/Parameter/Information/InjectVolume") 
#define XMLPATH_SAMPLE_PARAM_INSTRU_CI		    _T("Sample/Parameter/Instrument/ColumnInfo")
#define XMLPATH_SAMPLE_PARAM_INSTRU_CG		    _T("Sample/Parameter/Instrument/CarrierGas")
#define XMLPATH_SAMPLE_PARAM_INSTRU_FR		    _T("Sample/Parameter/Instrument/FlowRate")
#define XMLPATH_SAMPLE_PARAM_INSTRU_D			_T("Sample/Parameter/Instrument/Detector")
#define XMLPATH_SAMPLE_PARAM_INSTRU_DETECTOR_CD   _T("Sample/Parameter/Instrument/Detector/CurrentDetector")
#define XMLPATH_SAMPLE_PARAM_INSTRU_DETECTOR_TEMP _T("Sample/Parameter/Instrument/Detector/Temp")
#define XMLPATH_SAMPLE_PARAM_INSTRU_DETECTOR_YZQDL _T("Sample/Parameter/Instrument/Detector/YZQDL")
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_D        _T("Sample/Parameter/Instrument/MainFrame/Detector") //������
#define XMLPATH_SAMPLE_PARAM_INSTRU_MF_TD       _T("Sample/Parameter/Instrument/MainFrame/Temp_Detector")



//------------------------------------------------------------------------+
//-------------------------------- XML���Զ��� ----------------------------|
//------------------------------------------------------------------------+

//--------ͨ������----
#define XMLPROP_DISPLAY     _T("_E_Display_")   //��ʾ
#define XMLPROP_NAME		_T("_E_Name_")		//����
#define XMLPROP_VALUETYPE	_T("_E_Type_")		//��������
#define XMLPROP_FORMAT		_T("_E_Format_")	//������ʾ��ʽ
#define XMLPROP_UNIT		_T("_E_Unit_")		//���ݵ�λ
#define XMLPROP_VALUE		_T("_E_Value_")		//����(ȱʡ)ֵ
#define XMLPROP_SOURCE      _T("_E_Source_")    //�ֶ���Դ���ǲ��������û��Լ�����
#define XMLPROP_DEFAULTVALUE      _T("DefaultValue")


//--------�������----
#define XMLPROP_TAB_ORDERS			_T("Orders")
#define XMLPROP_TAB_ORDERBY			_T("OrderBy")
#define XMLPROP_TAB_ASC				_T("Asc")

//ÿ������(ע��ÿ��ͬʱ��������5��ͨ������
#define XMLPROP_TAB_COLUMN			_T("Column")		//�б��

#define XMLPROP_TAB_COL_ID			_T("ID")			//��ID
#define XMLPROP_TAB_COL_TITLE		_T("Title")			//�б���
#define XMLPROP_TAB_COL_WIDTH		_T("ColumnWidth")	//�п��
#define XMLPROP_TAB_COL_READONLY	_T("Readonly")		//true����false,�Ƿ��ܱ༭
#define XMLPROP_TAB_COL_CANSIZE		_T("CanSize")		//true����false,�Ƿ��ܸı��п�
#define XMLPROP_TAB_COL_ALIGN		_T("Align")			//left,center,right|top,vcenter,middle,bottom,���߸�ֵ�����ִ�Сд
#define XMLPROP_TAB_COL_CTRLTYPE	_T("CtrlType")		//�ؼ�����
#define XMLPROP_TAB_COL_CTRLDATA	_T("CtrlData")		//�ؼ����ݣ��Ϳؼ����Ͷ�Ӧ
#define XMLPROP_TAB_ROW				_T("Row")			//�б��

//------------------------------------------------------------------------+
//--------------------------------- XMLֵ���� -----------------------------|
//------------------------------------------------------------------------+
#define XMLVALUE_TYPE_STRING	_T("string")
#define XMLVALUE_TYPE_INT		_T("int")
#define XMLVALUE_TYPE_FLOAT		_T("float")
#define XMLVALUE_TYPE_TIME		_T("time")
#define XMLVALUE_TYPE_TABLE		_T("table")
#define XMLVALUE_TYPE_CURVE2D	_T("curve2d")
#define XMLVALUE_TYPE_ICON		_T("icon")

#define XMLVALUE_CTRLTYPE_DEFAULT		_T("default")
#define XMLVALUE_CTRLTYPE_CHECKBOX		_T("checkbox")
#define XMLVALUE_CTRLTYPE_DROPLIST		_T("droplist")

//------------------------------------------------------------------------+
//-------------------------------������ֵ���� -----------------------------|
//------------------------------------------------------------------------+
// ���� Sample/Calibrate ����ĳ�������
#define XMLVALUE_CONST_c_percentage				    _T("�ٷֱȷ�")		//�ٷֱȷ�
#define XMLVALUE_CONST_c_normalization				_T("��һ����")	//��һ����
#define XMLVALUE_CONST_c_internal					_T("�ڱ귨")		//�ڱ귨
#define XMLVALUE_CONST_c_external					_T("��귨")		//��귨
#define XMLVALUE_CONST_c_internal_percentage   		_T("�ڱ�ٷֱȷ�")	//�ڱ�ٷֱȷ�
#define XMLVALUE_CONST_c_external_percentage		_T("���ٷֱȷ�")	//���ٷֱȷ�

#define XMLVALUE_CONSTENT_c_peak_area		_T("�����")		//�����
#define XMLVALUE_CONSTENT_c_peak_height		_T("���")		//���
/*
#define XMLVALUE_CONST_c_percentage				    _T("c_percentage")		//�ٷֱȷ�
#define XMLVALUE_CONST_c_normalization				_T("c_normalization")	//����һ����
#define XMLVALUE_CONST_c_internal					_T("c_internal")		//�ڱ귨
#define XMLVALUE_CONST_c_external					_T("c_external")		//��귨
#define XMLVALUE_CONST_c_internal_percentage   		_T("c_internal_percentage")	//�ڱ�ٷֱȷ�
#define XMLVALUE_CONST_c_external_percentage		_T("c_external_percentage")	//���ٷֱȷ�

#define XMLVALUE_CONSTENT_c_peak_area		_T("c_peak_area")		//�����
#define XMLVALUE_CONSTENT_c_peak_height		_T("c_peak_height")		//���
*/



// ���ݱ�����ʽ
#define SLOPE_DATA_FORMAT					_T("%.4f")  // б�ʡ��ؾ�����ϵ��������С�����4λ
#define INTERCEPT_DATA_FORMAT				_T("%.4f")
#define COEFFICIENT_DATA_FORMAT				_T("%.4f")  
#define RESERVETIME_DATA_FORMAT				_T("%.4f")  // ����ʱ��
#define CONTENTS_DATA_FORMAT				_T("%.3E")  // ����

//------------------------------------------------------------------------ +
//----------------------        �������      ----------------------------- |
//------------------------------------------------------------------------ +
#define CALIBRATETABLE						 _T("CalibrateTable")		//У�����̱�
#define CALIBRATERESULTTABLE				 _T("CalibrateResultTable") //У�������
#define PEAKLISTTABLE						 _T("PeakListTable")		//���ֽ����
#define RESULTTABLE							 _T("ResultTable")		    //���������

/*

//	|	
//	--->��������
#define XMLPATH_TASK_NAME				_T("Task/Name")			//��������
#define XMLPATH_TASK_CREATETIME			_T("Task/CreateTime")	//����ʱ��
#define XMLPATH_TASK_LIST				_T("Task/TaskList")		//�����б�
#define XMLPATH_TASK_TIMES				_T("Task/Times")		//�����б�
//	|	
*/


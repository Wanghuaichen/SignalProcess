#pragma once

enum GCType {
	gcT_get = 0x55,
	gcT_set = 0x5A,
	gcT_sample = 0x51
};

enum GCFlag {
	gcF_NULL,
	gcF_PillarArkTemp,					//������趨�¶�(��λ0.01��)
	gcF_ForeInputTemp,					//ǰ���������趨�¶�
	gcF_BackInputTemp,					//����������趨�¶�
	gcF_ForeDetectTemp,					//ǰ��������趨�¶�
	gcF_BackDetectTemp,					//���������趨�¶�
	gcF_AssistantTemp,					//�������趨�¶�
	gcF_PillarArkK,						//����ļ��ȿ���
	gcF_ForeInputK,						//ǰ�������ļ��ȿ���
	gcF_BackInputK,						//��������ļ��ȿ���
	gcF_ForeDetectK,					//ǰ������ļ��ȿ���
	gcF_BackDetectK,					//�������ļ��ȿ���
	gcF_AssistantK,						//�����ļ��ȿ���
	gcF_HotUpKeepTime = 0x10,			//�������¶ȵı���ʱ��(0 base)(��λ0.1��)
	gcF_HotUpSpeed,						//����������������(1 base)(��λ0.1��/��)
	gcF_HotUpTemp,						//���������¶�(1 base)(��λ0.01��)
	gcF_HotUpK = 0x26,					//����������������
	gcF_FIDSensitiveSW = 0x30,			//FID�������л�����
	gcF_FIDPolaritySW,					//FID�����л�
	gcF_FIDAFire,						//FIDA·���
	gcF_FIDBFire,						//FIDB·���
	gcF_TCDCurrent,						//TCD���ŵ���
	gcF_TCDPolaritySW,					//TCD�����л�
	gcF_HotUpSpeed8 = 0x40,				//����������������(8 base)(��λ0.1��/��)
	gcF_HotUpTemp8,						//���������¶�(8 base)(��λ0.01��)
	gcF_HotUpKeepTime8,					//�������¶ȵı���ʱ��(8 base)(��λ0.1��)
	gcF_ECDCurrentSel = 0x4A,			//ECD����ѡ��
	gcF_ECDMeasureSel,					//ECD����ѡ��
	gcF_FPDMeasureSel = 0x50,			//FPD����ѡ��
	gcF_FID2SensitiveSW = 0x55,			//FID1�������л�����
	gcF_FID2PolaritySW,					//FID1�����л�
	gcF_FID2AFire,						//FID1A·���
	gcF_FID2BFire						//FID1B·���
};

enum GCSampleFlag {
	gcsF_Cycle = 1,
	gcsF_Run = 2,
	gcsF_Data = 5
};

#define TEMP_MAX_VALUE					40000	//����¶�(��λ0.01��)
#define HOT_UP_MAX_VALUE				400	//�������(��λ0.1��/��)
#define HOT_UP_SPACE_SIZE				3	//��������������
#define HOT_UP_LEVEL1					7	//���½���
#define HOT_UP_LEVEL2					10	//���½���(��չ�������)
#define TCD_CURRENT_MAX_VALUE			255	//���TCD�ŵ���
#define GC_TABLE1_SIZE					30	//��1:����
#define GC_TABLE2_SIZE					28	//��2:����
#define TABLE1_TEMP_SPACE_SIZE			4	//��1:�¶ȼ��
#define TABLE1_SET_TEMP_OFS				0	//��1:�����¶�ƫ����
#define TABLE1_TRUE_TEMP_OFS			2	//��1:ʵ���¶�ƫ����
#define TABLE1_TEMP_SWITCH_OFS			24	//��1:�¶ȿ���ƫ����
#define TABLE1_TEMP_SWITCH_NUM			6	//��1:�¶ȿ��ظ���
#define TABLE2_FID_POLARITY_OFS			0	//��2:FID����ƫ����
#define TABLE2_FID_SENSITIVE_OFS		1	//��2:FID������ƫ����
#define TABLE2_TCD_CURRENT_OFS			2	//��2:TCD�ŵ���
#define TABLE2_TCD_POLARITY_OFS			3	//��2:TCD����
#define TABLE2_FPD_MEASURE_OFS			4	//��2:FPD����ѡ��
#define TABLE2_ECD_CURRENT_OFS			5	//��2:ECD����ѡ��
#define TABLE2_ECD_MEASURE_OFS			6	//��2:ECD����ѡ��
#define TABLE2_FID2_SENSITIVE_OFS		7	//��2:FID2������ƫ����
#define TABLE2_FID2_POLARITY_OFS		8	//��2:FID2����ƫ����
#define TABLE2_FLUX_SPACE_SIZE			2	//��2:�������
#define TABLE2_FLUX_OFS					12	//��2:����ƫ����
#define TABLE2_FLUX_NUM					6	//��2:��������

#define DEFAULT_SAMPLE_CYCLE			66	//��������(��λms)
#define MAX_SAMPLE_RESULT_NUM			41	//һ������ȡ�����

#pragma pack(push, 1)
typedef struct tagSampleResult {
	DWORD dwTime;
	union {
		LONG lValue;
		float fValue;
	};
}	SAMPLE_RESULT, *P_SAMPLE_RESULT;
#pragma pack(pop)

enum GCFIDSensitive {
	FIDs_10_10, FIDs_10_9, FIDs_10_8, FIDs_10_7
};

enum GCReturnCode {
	gc_FearfulErr = -2, gc_COMErr = -1, gc_OK = 0, gc_Invalid, gc_Failed
};

#ifdef GC_EXPORTS
#define GC_DLL	__declspec(dllexport)
#else
#define GC_DLL	__declspec(dllimport)
#endif

extern GC_DLL const BYTE* WINAPI GC_getTable(int iTable, UINT &uSize);//ȡ�ڲ����ݱ� -
//��1�ֶ� -
extern GC_DLL int WINAPI GC_getSettedTemp(const BYTE* pcdbTable, UINT uFlag, UINT &uValue);//�ӱ�1��ȡ���õ��¶� -
extern GC_DLL int WINAPI GC_getTrueTemp(const BYTE* pcdbTable, UINT uFlag, UINT &uValue);//�ӱ�1��ȡʵ���¶� -
extern GC_DLL int WINAPI GC_getTempSwitch(const BYTE* pcdbTable, UINT uFlag, BOOL &bValue);//�ӱ�1��ȡ�¶ȿ���״̬ -
//��2�ֶ� -
extern GC_DLL int WINAPI GC_getFIDValue(const BYTE* pcdbTable, UINT uFlag, UINT &uValue);//�ӱ�2��ȡFIDֵ -
extern GC_DLL int WINAPI GC_getTCDValue(const BYTE* pcdbTable, UINT uFlag, UINT &uValue);//�ӱ�2��ȡTCDֵ -
extern GC_DLL int WINAPI GC_getFPDValue(const BYTE* pcdbTable, UINT uFlag, UINT &uValue);//�ӱ�2��ȡFPDֵ -
extern GC_DLL int WINAPI GC_getECDValue(const BYTE* pcdbTable, UINT uFlag, UINT &uValue);//�ӱ�2��ȡECDֵ -
extern GC_DLL int WINAPI GC_getFlux(const BYTE* pcdbTable, UINT uIdx, UINT &uValue);//�ӱ�2��ȡ���� -
//�������� -
extern GC_DLL BOOL WINAPI GC_Connect(int iComIdx = 0, int* piComIdx = NULL, int iBPS = 19200, int iTimeout = 3000);
extern GC_DLL void WINAPI GC_Close();
extern GC_DLL int WINAPI GC_Send(UINT uType, UINT uAddr, UINT uflag, LPBYTE lpdbData, UINT uLen, LPBYTE lpdbOut, UINT &uOutLen, BOOL bOutCntOnly);
//�¶ȿ��� -
extern GC_DLL int WINAPI GC_getData(UINT uFlag, UINT &uResult);//��ȡ����(ͬʱˢ�±�1) -
extern GC_DLL int WINAPI GC_setData(UINT uFlag, UINT uData, UINT &uResult);//��������(ͬʱˢ�±�2) -
extern GC_DLL int WINAPI GC_setHotUpData(UINT uFlag, UINT uIdx, UINT uData, UINT &uResult);//�������� -
//���� -
extern GC_DLL void WINAPI GC_initSampleTimeCount(UINT uChannel);//��ʼ�������� -
extern GC_DLL int WINAPI GC_getSampleCycle(UINT uChannel, UINT &uCycle);//ȡ�ò������� -
extern GC_DLL int WINAPI GC_runSample(UINT uChannel, BOOL &bSuccessd);//��ʼ�������� -
extern GC_DLL int WINAPI GC_getRealtimeData(UINT uChannel, UINT uCycle, P_SAMPLE_RESULT ptsr, int &iNum, BOOL &bStartFlag);//ȡ���� -
//#define GC_USE_OLD_SAMPLE	//ʹ���ϵĲ������ݸ�ʽ -

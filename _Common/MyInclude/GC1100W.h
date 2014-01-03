#pragma once

enum GCType {
	gcT_sample = 0x51
};

enum GCSampleFlag {
	gcsF_Cycle = 1,
	gcsF_Run = 2,
	gcsF_Data = 5
};

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

enum GCReturnCode {
	gc_FearfulErr = -2, gc_COMErr = -1, gc_OK = 0, gc_Invalid, gc_Failed
};

#ifdef GC1100W_EXPORTS
#define GC1100W_DLL	__declspec(dllexport)
#else
#define GC1100W_DLL	__declspec(dllimport)
#endif

//�������� -
extern GC1100W_DLL HANDLE WINAPI GC_Create(int iComIdx, int iBPS = 9600, int iTimeout = 500);
extern GC1100W_DLL void WINAPI GC_Close(HANDLE h);
//�¶ȿ��� -
extern GC1100W_DLL int WINAPI GC_getPillarArk1Temp(HANDLE h, int &iTemp);//ȡ������1�¶� -
extern GC1100W_DLL int WINAPI GC_setPillarArk1Temp(HANDLE h, int iTemp);//�趨����1�¶� -
extern GC1100W_DLL int WINAPI GC_getInputDetectTemp(HANDLE h, int &iTemp);//ȡ�ý�������¶� -
extern GC1100W_DLL int WINAPI GC_setInputDetectTemp(HANDLE h, int iTemp);//�趨��������¶� -
extern GC1100W_DLL int WINAPI GC_startHotUp(HANDLE h);//��ʼ���� -
extern GC1100W_DLL int WINAPI GC_stopHotUp(HANDLE h);//ֹͣ���� -
extern GC1100W_DLL int WINAPI GC_doFire(HANDLE h);//��� -
extern GC1100W_DLL int WINAPI GC_getStatus(HANDLE h, int &iStatus);//��״̬ -
//���� -
extern GC1100W_DLL int WINAPI GC_initSampleTimeCount(HANDLE h, UINT uChannel);//��ʼ�������� -
extern GC1100W_DLL int WINAPI GC_getSampleCycle(HANDLE h, UINT uChannel, UINT &uCycle);//ȡ�ò������� -
extern GC1100W_DLL int WINAPI GC_runSample(HANDLE h, UINT uChannel, BOOL &bSuccessd);//��ʼ�������� -
extern GC1100W_DLL int WINAPI GC_getRealtimeData(HANDLE h, UINT uChannel, UINT uCycle, P_SAMPLE_RESULT ptsr, int &iNum, BOOL &bStartFlag);//ȡ���� -
//#define GC_USE_OLD_SAMPLE	//ʹ���ϵĲ������ݸ�ʽ -

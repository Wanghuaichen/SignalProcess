#pragma once

#define MSB_DEVICE

enum LCType {
	lcT_query = 0x51,
	lcT_cmd,
	lcT_run,
	lcT_sample = 0x51
};

enum LCAddr {
	lcA_PumpA = 1,
	lcA_PumpB,
	lcA_PumpC,
	lcA_PumpD,
	lcA_Detect,
};

enum LCFlag {
	lcF_D_qDeuteriumTime = 1,			//ѯ��뮵��ۼ�ʱ -
	lcF_D_qIP,							//ѯ��IP��ַ -
	lcF_D_qWavelength,					//ѯ�ʵ�ǰ����ִ����� -
	lcF_D_qWarning,						//������ѯ -
	lcF_D_qWarningResponse,				//������ѯ��Ӧ��(���ܷ�)
	lcF_D_qFeedback = 0x10,				//ѯ��������Ϣ���� -

	lcsF_Cycle = 1,
	lcsF_Run = 2,
	lcsF_Data = 5,

	lcF_D_cWavelength = 1,				//�����趨�������������� -
	lcF_D_cWavelengthScan,				//�趨����ɨ�� -
	lcF_D_cWavelengthTable,				//���Ĳ�������� -
	lcF_D_cZero,						//���ĵ������� -
	lcF_D_cIP,							//����IP��ַ -
	lcF_D_cPWD,							//�������� -
	lcF_D_cAdjustWavelength,			//����У׼ -
	lcF_D_cDeuteriumTime,				//뮵�ʱ����� -
	lcF_D_cResetWarning,				//������λ -
	lcF_D_cFeedback = 0x10,				//������Ϣ���� -

	lcF_D_rCmd = 1,						//�������뮵ƿ��������򲨳����У����㣬���ߣ� -
	lcF_D_rFeedback = 0x10,				//����������Ϣ���� -

	lcF_P_qIdxNumber = 1,				//ѯ��A�úͱ����� -
	lcF_P_qConstantSpeed,				//ѯ�ʺ����ٶ� -
	lcF_P_qIP,							//ѯ��IP��ַ -
	lcF_P_qGland,						//ѯ���ܷ�Ȧ -
	lcF_P_qPressure,					//ѯ�ʵ�ǰѹ��ֵ��ѹ����λ�����١����� -
	lcF_P_qWarning,						//������ѯ -
	lcF_P_qWarningResponse,				//������ѯ��Ӧ��(���ܷ�)
	lcF_P_qFeedback = 0x10,				//ѯ��������Ϣ���� -

	lcF_P_cIdxNumber = 1,				//���ıúźͱ����� -
	lcF_P_cConstantSpeed,				//���ĺ����ٶ� -
	lcF_P_cPressure,					//����ѹ��ֵ��ѹ����λ -
	lcF_P_cWashParam,					//����ϴ�Ѳ���(2�ֽ����٣�1�ֽ�ʱ�䣩 -
	lcF_P_cPressureZero,				//����ѹ������ -
	lcF_P_cRippleCompress,				//��������ѹ�� -
	lcF_P_cIP,							//����IP��ַ -
	lcF_P_cGradsTable,					//�����ݶȱ� -
	lcF_P_cPWD,							//�������� -
	lcF_P_cPressureAdjust,				//ѹ������ -
	lcF_P_cVelocityAdjust,				//���ٵ��� -
	lcF_P_cResetWarning,				//������λ -
	lcF_P_cFeedback = 0x10,				//������Ϣ���� -

	lcF_P_rCmd = 1,						//��������ÿ������ݶ����У���ϴ�����ߣ� -
	lcF_P_rFeedback = 0x10				//����������Ϣ���� -
};

//�������������ö�� -
enum LCDetectCmd {
	lcC_Detect_Sleep = 1,				//0x01����������� -
	lcC_Detect_WakeUp = 0,				//0x00: ��������� -
	lcC_Detect_StopWave = 2,			//0x02�������༭����ֹͣ -
	lcC_Detect_StartWave = 3,			//0x03: �����༭�������� -
	lcC_Detect_CloseDeuterium = 4,		//0x04��뮵ƹ� -
	lcC_Detect_OpenDeuterium = 5,		//0x05: 뮵ƿ� -
	lcC_Detect_NoZero = 6,				//0x06�������� -
	lcC_Detect_DoZero = 7				//0x07: ���� -
};

//����������ö�� -
enum LCPumpCmd {
	lcC_Pump_Sleep = 1,					//0x01�������� -
	lcC_Pump_WakeUp = 0,				//0x00: �û��� -
	lcC_Pump_StopGrads = 2,				//0x02: �ݶ�ֹͣ -
	lcC_Pump_ReadyGrads = 3,			//0x03: �ݶ�׼�� -
	lcC_Pump_RunGrads = 4,				//0x04: �ݶ����� -
	lcC_Pump_Stop = 5,					//0x05: ��ֹͣ -
	lcC_Pump_Run = 6,					//0x06: ������ -
	lcC_Pump_StopWash = 7,				//0x07: �ó�ϴֹͣ -
	lcC_Pump_RunWash = 8				//0x08: �ó�ϴ���� -
};

#pragma pack(push, 1)
typedef struct tagGetDetectWavelength {
	WORD	wWavelength;				//��ǰ���� -
	DWORD	dwAbsorbency;				//����� -
	DWORD	dwSampleValue;				//��Ʒֵ -
	DWORD	dwCompareValue;				//�α�ֵ -
	BYTE	dbWorkStatus;				//����״̬ -
	BYTE	dbSysStatus;				//ϵͳ��� -
}	GET_DETECT_WAVE_LENGTH, *P_GET_DETECT_WAVE_LENGTH;

typedef struct tagGetDetectWarning {
	BYTE	dbType;						//�������� -
	BYTE	dbWarning;					//������� -
	DWORD	dwSettingParam;				//�趨���� -
	DWORD	dwNowParam;					//��ǰ���� -
}	GET_DETECT_WARNING, *P_GET_DETECT_WARNING;

typedef struct tagSetDetectWavelength {
	WORD	wWavelength;				//�趨���� -
	BYTE	dbSampleSpeed;				//�������� -
}	SET_DETECT_WAVE_LENGTH, *P_SET_DETECT_WAVE_LENGTH;

typedef struct tagSetDetectWavelengthScan {
	BYTE	dbWavelengthScan;			//����ɨ�� -
	WORD	wStartWavelength;			//��ʼ���� -
	WORD	wEndWavelength;				//�������� -
}	SET_DETECT_WAVE_LENGTH_SCAN, *P_SET_DETECT_WAVE_LENGTH_SCAN;

typedef struct tagWavelengthTable{
	DWORD	dwTime;						//ʱ�� -
	WORD	wWavelength;				//���� -
	BYTE	dbSampleSpeed;				//���� -
}	WAVELENGTH_TABLE, *P_WAVELENGTH_TABLE;

typedef struct tagSetDetectWavelengthTable{
	BYTE	dbTotalNum;					//������ -
	BYTE	dbNow;						//��ǰ�� -
	WAVELENGTH_TABLE tWavelengthTable;	//��������� -
}	SET_DETECT_WAVELENGTH_TABLE, *P_SET_DETECT_WAVELENGTH_TABLE;

typedef struct tagGetPumpGland {
	DWORD	dwLeftGland;				//���ܷ�Ȧ -
	DWORD	dwRightGland;				//���ܷ�Ȧ -
}	GET_PUMP_GLAND, *P_GET_PUMP_GLAND;

typedef struct tagGetPumpPressure {
	WORD	wPressure;					//ѹ��ֵ -
	BYTE	dbUnit;						//ѹ����λ -
	DWORD	dwPressureRange;			//ѹ�������� -
	WORD	wVelocity;					//��ǰ���� -
	BYTE	dbSysStatus;				//ϵͳ��� -
	BYTE	dbWorkStatus;				//����״̬ -
}	GET_PUMP_PRESSURE, *P_GET_PUMP_PRESSURE;

typedef struct tagGetPumpWarning {
	BYTE	dbType;						//�������� -
	BYTE	dbWarning;					//������� -
	DWORD	dwSettingParam;				//�趨���� -
	DWORD	dwNowParam;					//��ǰ���� -
}	GET_PUMP_WARNING, *P_GET_PUMP_WARNING;

typedef struct tagSetPumpPressure {
	WORD	wPressureHigh;				//ѹ��ֵ(����) -
	WORD	wPressureLow;				//ѹ��ֵ(����) -
	BYTE	dbUnit;						//ѹ����λ -
}	SET_PUMP_PRESSURE, *P_SET_PUMP_PRESSURE;

typedef struct tagSetPumpWashParam {
	WORD	wVelocity;					//���� -
	BYTE	dbTime;						//ʱ�� -
}	SET_PUMP_WASH_PARAM, *P_SET_PUMP_WASH_PARAM;

typedef struct tagGradsTable{
	DWORD	dwTime;						//ʱ�� -
	WORD	wScaleA;					//A���� -
	WORD	wScaleB;					//B���� -
	WORD	wScaleC;					//C���� -
	WORD	wScaleD;					//D���� -
	WORD	wVelocity;					//���� -
}	GRADS_TABLE, *P_GRADS_TABLE;

typedef struct tagSetPumpGradsTable{
	BYTE	dbTotalNum;					//������ -
	BYTE	dbNow;						//��ǰ�� -
	GRADS_TABLE tGradsTable;			//�ݶȱ� -
}	SET_PUMP_GRADS_TABLE, *P_SET_PUMP_GRADS_TABLE;

typedef struct tagSampleResult {
	DWORD dwTime;
	LONG lValue;
}	SAMPLE_RESULT, *P_SAMPLE_RESULT;
#pragma pack(pop)

#define LC_IP_LEN						4
#define LC_PWD_LEN						6
#define LC_FEEDBACK_LEN					1

enum LCReturnCode {
	lc_COMErr = -1, lc_OK = 0, lc_Invalid, lc_Failed
};

//������ -
enum LCFeedbackCode {
	lcfb_OK, lcfb_Err1, lcfb_Err2, lcfb_Err3
};

//�����ϵͳ��� -
#define LC_DETECT_SYS_VALVE_OPEN		0x80	//�������� -
#define LC_DETECT_SYS_PC_CONTROL		0x40	//PC���� -
#define LC_DETECT_SYS_WAVELENGTH_COME	0x20	//������λ -
#define LC_DETECT_SYS_HAS_WARNING		0x10	//�Ƿ��б���  -

//���������״̬ -
enum LCDetectWorkStatus {
	lcDS_UVSleep,						//0��UV���� -
	lcDS_DeuteriumClose,				//1��뮵ƹ� -
	lcDS_DeuteriumOpen,					//2�����߼��ӣ�뮵ƿ��� -
	lcDS_SpecialWavelengthSample,		//3������������ -
	lcDS_ProgramWavelengthSample,		//4�����򲨳����� -
	lcDS_WavelengthScan					//5������ɨ�� -
};

//��ϵͳ���˵�� -
#define LC_PUMP_SYS_PC_CONTROL			0x40	//PC����
#define LC_PUMP_SYS_HAS_WARNING			0x10	//�Ƿ��б���

//�ù���״̬ -
enum LCPumpWorkStatus {
	lcPS_PumpSleep,						// 0: ������ -
	lcPS_PumpConstantSpeed,				// 1���������� -
	lcPS_GradsReady,					// 2���ݶ�׼�� -
	lcPS_GradsRun,						// 3���ݶ����� -
	lcPS_PumpWash,						// 4���ó�ϴ -
	lcPS_PumpStop						// 5����ֹͣ -
};

//�þ����� -
enum LCPumpWarningCode {
	lcPW_PumpPressureUpper = 1,			//ѹ�����ޱ��� -
	lcPW_PumpPressureLower				//ѹ�����ޱ��� -
};

#define MAX_WARNING_RECORD_NUM			24	//һ������ȡ������ -
#define DEFAULT_SAMPLE_CYCLE			20	//��������(��λms)
#define MAX_SAMPLE_RESULT_NUM			41	//һ������ȡ����� -

#ifdef LC_EXPORTS
#define LC_DLL	__declspec(dllexport)
#else
#define LC_DLL	__declspec(dllimport)
#endif

//�������� -
extern LC_DLL BOOL WINAPI LC_Connect(int iComIdx = 0, int* piComIdx = NULL, int iBPS = 9600, int iTimeout = 500);
extern LC_DLL void WINAPI LC_Close();
extern LC_DLL int WINAPI LC_Send(UINT uType, UINT uAddr, UINT uflag, LPBYTE lpdbData, UINT uLen, LPBYTE lpdbOut, UINT &uOutLen, BOOL bOutCntOnly);

//���⺯�� -
extern LC_DLL P_GET_DETECT_WAVE_LENGTH WINAPI LC_GetWavelengthScanData(int &iNum, int &iErrorCode);
//extern LC_DLL BOOL WINAPI LC_GetWavelengthProgramData(P_GET_DETECT_WAVE_LENGTH p, int &iErrorCode, BOOL &bEndFlag);
extern LC_DLL BOOL WINAPI LC_GetRunCmdResult(int &iErrorCode, BOOL bWaitEnd = TRUE);

//����� -
extern LC_DLL int WINAPI LC_GetDetectDeuteriumTime(WORD &wDeuteriumTime);//ѯ��뮵��ۼ�ʱ -
extern LC_DLL int WINAPI LC_GetDetectIP(BYTE dbIP[LC_IP_LEN]);//ѯ��IP��ַ -
extern LC_DLL int WINAPI LC_GetDetectWavelength(WORD &wWavelength, DWORD &dwAbsorbency, DWORD &dwSampleValue, DWORD &dwCompareValue, BYTE &dbSysStatus, BYTE &dbWorkStatus);//ѯ�ʵ�ǰ����ִ����� -
extern LC_DLL int WINAPI LC_GetDetectWarning(BYTE dbType, P_GET_DETECT_WARNING ptgdw, int &iNum);//������ѯ -
extern LC_DLL int WINAPI LC_GetDetectFeedback(BYTE &dbFeedback);//ѯ��������Ϣ���� -

extern LC_DLL int WINAPI LC_SetDetectWavelength(WORD wWavelength, BYTE dbSampleSpeed);//�����趨�������������� -
extern LC_DLL int WINAPI LC_SetDetectWavelengthScan(BYTE dbWavelengthScan, WORD wStartWavelength, WORD wEndWavelength);//�趨����ɨ�� -
extern LC_DLL int WINAPI LC_SetDetectWavelengthTable(BYTE dbTotalNum, BYTE dbNow, P_WAVELENGTH_TABLE pwt);//���Ĳ�������� -
extern LC_DLL int WINAPI LC_SetDetectZero(BYTE dbZero);//���ĵ������� -
extern LC_DLL int WINAPI LC_SetDetectIP(BYTE dbIP[LC_IP_LEN]);//����IP��ַ -
extern LC_DLL int WINAPI LC_SetDetectPWD(BYTE dbPWD[LC_PWD_LEN]);//�������� -
extern LC_DLL int WINAPI LC_SetDetectAdjustWavelength(WORD wAdjustWavelength);//����У׼ -
extern LC_DLL int WINAPI LC_SetDetectDeuteriumTime(BYTE dbDeuteriumTime);//뮵�ʱ����� -
extern LC_DLL int WINAPI LC_SetDetectResetWarning(BYTE dbType, BYTE dbWarning);//������λ -
extern LC_DLL int WINAPI LC_SetDetectFeedback(BYTE &dbFeedback);//������Ϣ���� -

extern LC_DLL int WINAPI LC_RunDetectCmd(BYTE dbCmd, DWORD dwTimeOut = 0, BOOL bWaitEnd = TRUE);//�������뮵ƿ��������򲨳����У����㣬���ߣ� -
extern LC_DLL int WINAPI LC_RunDetectFeedback(BYTE &dbFeedback);//����������Ϣ���� -

//�����::���� -
extern LC_DLL void WINAPI LC_initSampleTimeCount();//��ʼ�������� -
extern LC_DLL int WINAPI LC_getSampleCycle(UINT &uCycle);//ȡ�ò������� -
extern LC_DLL int WINAPI LC_runSample(BOOL &bSuccessd);//��ʼ�������� -
extern LC_DLL int WINAPI LC_getRealtimeData(UINT uCycle, P_SAMPLE_RESULT ptsr, int &iNum, BOOL &bStartFlag);//ȡ���� -

//�� -
extern LC_DLL int WINAPI LC_GetPumpIdxNumber(int idx, BYTE &dbPumpIdx, BYTE &dbPumpNum);//ѯ��A�úͱ����� -
extern LC_DLL int WINAPI LC_GetPumpConstantSpeed(int idx, WORD &wConstantSpeed);//ѯ�ʺ����ٶ� -
extern LC_DLL int WINAPI LC_GetPumpIP(int idx, BYTE dbIP[LC_IP_LEN]);//ѯ��IP��ַ -
extern LC_DLL int WINAPI LC_GetPumpGland(int idx, DWORD &dwLeftGland, DWORD &dwRightGland);//ѯ���ܷ�Ȧ -
extern LC_DLL int WINAPI LC_GetPumpPressure(int idx, WORD &wPressure, BYTE &dbUnit, DWORD &dwPressureRange, WORD &wVelocity, BYTE &dbSysStatus, BYTE &dbWorkStatus);//ѯ�ʵ�ǰѹ��ֵ��ѹ����λ��ѹ�������ޡ����١����� -
extern LC_DLL int WINAPI LC_GetPumpWarning(int idx, BYTE dbType, P_GET_PUMP_WARNING ptgpw, int &iNum);//������ѯ -
extern LC_DLL int WINAPI LC_GetPumpFeedback(int idx, BYTE &dbFeedback);//ѯ��������Ϣ���� -

extern LC_DLL int WINAPI LC_SetPumpIdxNumber(int idx, BYTE dbPumpIdx, BYTE dbPumpNum);//���ıúźͱ����� -
extern LC_DLL int WINAPI LC_SetPumpConstantSpeed(int idx, WORD wConstantSpeed);//���ĺ����ٶ� -
extern LC_DLL int WINAPI LC_SetPumpPressure(int idx, WORD wPressureHigh, WORD wPressureLow, BYTE dbUnit);//����ѹ��ֵ��ѹ����λ -
extern LC_DLL int WINAPI LC_SetPumpWashParam(int idx, WORD wVelocity, BYTE dbTime);//����ϴ�Ѳ���(2�ֽ����٣�1�ֽ�ʱ�䣩 -
extern LC_DLL int WINAPI LC_SetPumpPressureZero(int idx, BYTE dbPressureZero);//����ѹ������ -
extern LC_DLL int WINAPI LC_SetPumpRippleCompress(int idx, WORD wRippleCompress);//��������ѹ�� -
extern LC_DLL int WINAPI LC_SetPumpIP(int idx, BYTE dbIP[LC_IP_LEN]);//����IP��ַ -
extern LC_DLL int WINAPI LC_SetPumpGradsTable(int idx, BYTE dbTotalNum, BYTE dbNow, P_GRADS_TABLE pgt);//�����ݶȱ� -
extern LC_DLL int WINAPI LC_SetPumpPWD(int idx, BYTE dbPWD[LC_PWD_LEN]);//�������� -
extern LC_DLL int WINAPI LC_SetPumpPressureAdjust(int idx, WORD wPressureAdjust);//ѹ������ -
extern LC_DLL int WINAPI LC_SetPumpVelocityAdjust(int idx, WORD wVelocityAdjust);//���ٵ��� -
extern LC_DLL int WINAPI LC_SetPumpResetWarning(int idx, BYTE dbType, BYTE dbWarning);//������λ -
extern LC_DLL int WINAPI LC_SetPumpFeedback(int idx, BYTE &dbFeedback);//������Ϣ���� -
extern LC_DLL int WINAPI LC_RunPumpCmd(int idx, BYTE dbCmd, DWORD dwTimeOut = 0, BOOL bWaitEnd = TRUE);//��������ÿ������ݶ����У���ϴ�����ߣ� -
extern LC_DLL int WINAPI LC_RunPumpFeedback(int idx, BYTE &dbFeedback);//����������Ϣ���� -

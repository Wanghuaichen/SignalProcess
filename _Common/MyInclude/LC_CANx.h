#pragma once

//�����豸ö�� -
enum LCDevCanId {
	lcDC_Detect = 0x205,
	lcDC_PumpA = 0x301,
	lcDC_PumpB = 0x302,
	lcDC_PumpC = 0x303,
	lcDC_PumpD = 0x304,
	lcDC_AllPump = 0xFFF,
	lcDC_All = 0x1FFF
};

//���󷵻� -
enum LCReturnCode {
	lc_COMErr = -1,						//CANͨѶ�����ʱ -
	lc_OK = 0,							//�ɹ���� -
	lc_IsRunning = 1,					//�Ѿ���ʼ���У�������δ���� -
	lc_UnknowCmd = 2,					//��֧�ֵ�ָ�� -
	lc_DeviceTypeErr = 3,				//�豸���ʹ��� -
	lc_SystemBusy = 4,					//ϵͳæ -
	lc_StartSample = 5,					//��ʼ���� -
	lc_InvalidParam = 6,				//����������� -
	lc_FixedCanId = 7,					//���ܸ���CAN�ڵ��ַ -
	lc_WaveGradsBegin = 8,				//�����ݶȸöο�ʼ -
	lc_WaveGradsEnd = 9					//�����ݶȸöν��� -
};

//����� -
enum LCType {
	lcT_query = 0x51,
	lcT_set = 0x52,
	lcT_broadcast = 0x53
};

//������� -
enum LCSubCmd {
	lcsc_All_Can = 1,					//CAN
	lcsc_All_LogicIndex = 2,			//�߼����
	lcsc_All_Time = 3,					//ʱ��
	lcsc_All_RealtimeData = 5,			//ʵʱ����
	lcsc_All_Warning = 6,				//����
	lcsc_All_Rest = 9,					//����
	lcsc_UV_Status = 10,				//״̬	��ѯ
	lcsc_UV_DeuteriumLamp = 11,			//뮵�	��ѯ������
	lcsc_UV_Wavelength = 12,			//����	��ѯ���趨
	lcsc_UV_WavelengthScan = 13,		//		����ɨ���ѯ��ִ��
	lcsc_UV_WavelengthTable_Get = 14,	//���������	��ѯ
	lcsc_UV_WavelengthTable_Cls = 15,	//		���
	lcsc_UV_WavelengthTable_Add = 16,	//		����
	lcsc_UV_WavelengthTable_Del = 17,	//		ɾ��
	lcsc_UV_WavelengthAdjust_Zero = 18,	//����У��	��������
	lcsc_UV_WavelengthAdjust_Adj = 19,	//		����У׼
	lcsc_UV_WavelengthAdjust_Chk = 20,	//		�Լ�
	lcsc_UV_WavelengthAdjust = 21,		//		����У��ֵ�趨����ѯ
	lcsc_UV_RunCmd = 22,				//��������ִ��	��ѡ������
	lcsc_UV_Run_WavelengthScan = 23,	//		ִ�в���ɨ��
	lcsc_UV_Run_DeuteriumLamp = 24,		//		ִ��뮵ƿ���
	lcsc_UV_Run_WavelengthTable = 25,	//		ִ�г��򲨳�
	lcsc_UV_Run_Zero = 26,				//		ִ�е���
	lcsc_UV_WorkData = 27,				//ʵʱ��������	��ѯ
	lcsc_UV_WorkMultiData = 28,			//		��ѯ��֡
	lcsc_Pump_Status = 30,				//״̬	��ѯ -
	lcsc_Pump_Speed = 31,				//����	���á���ѯ -
	lcsc_Pump_Pressure = 32,			//ѹ��	��Χ�趨 -
	lcsc_Pump_PressureZero = 33,		//		���á���ѯ��λ��ʾ -
	lcsc_Pump_Ripple = 34,				//		�����趨 -
	lcsc_Pump_GradsTable_Get = 35,		//ѹ���ݶ�	��ѯ -
	lcsc_Pump_GradsTable_Cls = 36,		//		��� -
	lcsc_Pump_GradsTable_Add = 37,		//		����/�޸� -
	lcsc_Pump_GradsTable_Del = 38,		//		ɾ�� -
	lcsc_Pump_Wash = 39,				//ϴ��	���á���ѯ -
	lcsc_Pump_RunCmd = 40,				//������ִ��	������ִ�� -
	lcsc_Pump_Gland = 41,				//�ܷ�Ȧ	��ѯ������ -
	lcsc_Pump_Switch_Run = 42,			//		ִ�бÿ��� -
	lcsc_Pump_Switch_Wash = 43,			//		ִ�бó�ϴ -
	lcsc_Pump_Switch_GradsReady = 44,	//		ִ���ݶ�׼�� -
	lcsc_Pump_Switch_Grads = 45,		//		ִ���ݶ� -
	lcsc_Pump_Time = 46,				//��ʱ��	������ʱ�� -
	//����ɫ���¼ӹ��� -
	lcsc_Temp_BPS = 60,					//�¿�	ͨѶ���� -
	lcsc_Temp_Temp = 61,				//		�¶� -
	lcsc_CCS_Vol = 70,					//����Դ	�������� -
	lcsc_CCS_Temp = 71,					//		�����¶� -
	lcsc_CCS_TSw = 72,					//		�絼���¶ȿ��� -
	lcsc_CCS_Range = 73,				//		���̿��� -
	lcsc_CCS_DASw = 74,					//		�������� -
	lcsc_CCS_DAVal = 75					//		����ֵ -
};

//������������ö��(������ѱ��ײ����) -
// enum LCUVCmd {
// 	lcC_UV_Sleep = 1,					//0x01�������� -
// 	lcC_UV_WakeUp = 0,					//0x00: �û��� -
// 	lcC_UV_StopWave = 2,				//0x02: �����༭����ֹͣ -
// 	lcC_UV_RunWave = 3,					//0x03: �����༭�������� -
// 	lcC_UV_CloseDeuterium = 4,			//0x04: 뮵ƹ� -
// 	lcC_UV_OpenDeuterium = 5,			//0x05: 뮵ƿ� -
// 	lcC_UV_NoZero = 6,					//0x06: ������ -
// 	lcC_UV_DoZero = 7					//0x07: ���� -
// };

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

//����Դ����ö�� -
enum CCS_TSW_Val {
	T_SW_40 = 1, T_SW_50 = 2, T_SW_60 = 3
};
enum CCS_RANGE_Val {
	RANGE_uS = 1, RANGE_mS = 2
};

#pragma pack(push, 1)
//CAN�ڵ����� -
typedef struct tagCanInfo {
	BYTE	dbType;						//�豸���� -
	int		iCanID;						//�豸��ַ -
	int		iDeviceIdx;					//�豸��� -
}	CAN_INFO, *P_CAN_INFO;
//������Ϣ -
typedef struct tagWarningInfo {
	BYTE	dbType;						//�������� -
	BYTE	dbIdx;						//������� -
	DWORD	dwSetParam;					//�趨���� -
	DWORD	dwNowParam;					//��ǰ���� -
	DWORD	dwTime;						//����ʱ�� -
	WORD	wIdx;						//������� -
}	WARNING_INFO, *P_WARNING_INFO;
//ʵʱ���� -
typedef struct tagSampleResult {
	DWORD	dwTime;						//ʱ�� -
	float	fValue;						//���� -
}	SAMPLE_RESULT, *P_SAMPLE_RESULT;
//���⹤������ -
typedef struct tagUVWorkData {
	WORD	wWavelength;				//��ǰ���� -
	LONG	lAbsorbency;				//����� -
	DWORD	dwSampleValue;				//��Ʒֵ -
	DWORD	dwCompareValue;				//�α�ֵ -
}	UV_WORK_DATA, *P_UV_WORK_DATA;
//��������� -
typedef struct tagWavelengthTable {
	DWORD	dwTime;						//ʱ�� -
	WORD	wWavelength;				//���� -
	BYTE	dbSampleSpeed;				//���� -
}	WAVELENGTH_TABLE, *P_WAVELENGTH_TABLE;
//�ݶȱ� -
typedef struct tagGradsTable {
	DWORD	dwTime;						//ʱ�� -
	WORD	wScaleA;					//A���� -
	WORD	wScaleB;					//B���� -
	WORD	wScaleC;					//C���� -
	WORD	wScaleD;					//D���� -
	WORD	wVelocity;					//���� -
}	GRADS_TABLE, *P_GRADS_TABLE;
#pragma pack(pop)

#define MAX_LC_BUFFER_SIZE				260	//LCЭ�����󳤶� -
#define DEVICE_INFO_MAX_NUM				32	//������Ϣһ����෵�ظ��� -
#define WARNING_DATA_MAX_NUM			15	//����һ����෵�ظ��� -
#define REALTIME_DATA_MAX_NUM			31	//ʵʱ����һ����෵�ظ��� -
#define UV_WORK_DATA_MAX_NUM			17	//���⹤������һ����෵�ظ��� -


//�豸�������Ӻ��� -
extern BOOL WINAPI LC_initDLL();//��ʼ��DLL���� -
extern void WINAPI LC_exitDLL();//�˳�ǰ���� -
extern void WINAPI LC_setVCOM2CANFindOrder(int iDevType, int iLevel);//���ô���תCAN���豸�Ĳ���˳�� -
extern BOOL WINAPI LC_Connect(int &iComIdx);//�����豸 -
extern void WINAPI LC_Close();//�ر��豸 -
extern void WINAPI LC_setWindowMessage(HWND hWnd, UINT uMsg);//���ô��ھ������Ϣ -
extern void WINAPI LC_setCallbackFunction(BOOL (CALLBACK* pCallback)(int, LPBYTE, LPVOID), LPVOID lpParam);//���ûص����� -
extern void WINAPI LC_setWaitTimeout(DWORD dwWaitTimeout = 3000);//���ó�ʱ -

////////////////////////////////////////////////////////////////////////////////
//�첽������
//�豸�첽����(����)
extern int WINAPI LC_p_GetCurrentTime(int iCanID);//��ѯ��ǰʱ�� -
extern int WINAPI LC_p_SetCurrentTime(int iCanID, DWORD dwTime);//���õ�ǰʱ�� -
extern int WINAPI LC_p_SetCanID(int iCanID, int iNewCanID);//�趨�豸CAN��ַ -
extern int WINAPI LC_p_GetCanInfo(int iCanID);//ѯ��CAN�ڵ����� -
extern int WINAPI LC_p_RunWake(int iCanID, BOOL bOnFlag);//ִ�л��� -
extern int WINAPI LC_p_GetWarning(int iCanID, BYTE dbType, BYTE dbIdx);//��ѯ���� -
extern int WINAPI LC_p_SetWarning(int iCanID, BYTE dbType, BYTE dbIdx, WORD wIdx);//������λ -
extern int WINAPI LC_p_GetRealtimeData(int iCanID);//ʵʱ���� -
extern int WINAPI LC_p_SetLogicIndex(int iCanID, WORD wLogicIndex);//�����豸���߼���� -
//�����첽����(����)
extern int WINAPI LC_p_GetUVStatus(int iCanID);//��ѯ����״̬ -
extern int WINAPI LC_p_GetUVWorkData(int iCanID, BOOL bMultiData);//��ѯ���⹤������ -
extern int WINAPI LC_p_GetUVDeuteriumLampTime(int iCanID);//��ѯ뮵��ۼ�����ʱ -
extern int WINAPI LC_p_GetUVWavelength(int iCanID);//��ѯ�趨���� -
extern int WINAPI LC_p_GetUVWavelengthTable(int iCanID, BYTE dbLine);//��ѯ���������
extern int WINAPI LC_p_GetUVWavelengthScanRange(int iCanID);//��ѯ����ɨ�跶Χ -
extern int WINAPI LC_p_SetUVWavelengthSmooth(int iCanID, WORD wWavelength, BYTE dbSmoothValue);//�趨������ƽ��ָ�� -
extern int WINAPI LC_p_SetUVWavelengthScan(int iCanID, BYTE dbWavelengthScan, WORD wStartWavelength, WORD wEndWavelength);//�趨(ִ��)����ɨ�� -
extern int WINAPI LC_p_ClsUVWavelengthTable(int iCanID);//������������ -
extern int WINAPI LC_p_SetUVWavelengthTable(int iCanID, P_WAVELENGTH_TABLE pwt);//����/�޸Ĳ�������� -
extern int WINAPI LC_p_DelUVWavelengthTable(int iCanID, DWORD dwTime);//ɾ������������� -
extern int WINAPI LC_p_SetUVWavelengthZero(int iCanID, BOOL bZeroFlag);//���ĵ������� -
extern int WINAPI LC_p_AdjUVWavelength(int iCanID, WORD wParam);//У׼���� -
extern int WINAPI LC_p_SetUVDeuteriumLampTime(int iCanID);//����뮵�ʱ�� -
extern int WINAPI LC_p_RunUVWavelengthScan(int iCanID, BOOL bOnFlag);//ִ�в���ɨ�� -
extern int WINAPI LC_p_RunUVDeuteriumLamp(int iCanID, BOOL bOnFlag);//ִ��뮵ƿ��� -
extern int WINAPI LC_p_RunUVWavelengthTable(int iCanID, BOOL bOnFlag);//ִ�г��򲨳� -
extern int WINAPI LC_p_RunUVWavelengthZero(int iCanID);//ִ�е��� -
extern int WINAPI LC_p_RunUVWavelengthSelfCheck(int iCanID);//�����Լ� -
extern int WINAPI LC_p_SetUVWavelengthAdjust(int iCanID, short sAdjustValue);//�趨����У��ֵ -
extern int WINAPI LC_p_GetUVWavelengthAdjust(int iCanID);//��ѯ����У��ֵ -
//���첽����(����)
extern int WINAPI LC_p_GetPumpConstantSpeed(int iCanID);//��ѯ�����ٶ� -
extern int WINAPI LC_p_GetPumpGland(int iCanID);//��ѯ�ܷ�Ȧ -
extern int WINAPI LC_p_SetPumpGlandZero(int iCanID);//�ܷ�Ȧ�������� -
extern int WINAPI LC_p_GetPumpPressure(int iCanID);//��ѯ��״̬ -
extern int WINAPI LC_p_SetPumpConstantSpeed(int iCanID, WORD wConstantSpeed);//���ú����ٶ� -
extern int WINAPI LC_p_SetPumpPressure(int iCanID, WORD wPressureHigh, WORD wPressureLow, BYTE dbUnit);//����ѹ��ֵ��Χ -
extern int WINAPI LC_p_SetPumpWashParam(int iCanID, WORD wVelocity, WORD wTime);//����ϴ�Ѳ��� -
extern int WINAPI LC_p_GetPumpWashParam(int iCanID);//��ѯϴ�Ѳ��� -
extern int WINAPI LC_p_SetPumpPressureZero(int iCanID, WORD wPressureZero);//����ѹ����ʾ��λ -
extern int WINAPI LC_p_SetPumpRippleCompress(int iCanID, WORD wRippleCompress);//��������ѹ�� -
extern int WINAPI LC_p_ClsPumpGradsTable(int iCanID);//���ѹ���ݶȱ� -
extern int WINAPI LC_p_SetPumpGradsTable(int iCanID, P_GRADS_TABLE pgt);//����/�޸�ѹ���ݶȱ� -
extern int WINAPI LC_p_DelPumpGradsTable(int iCanID, DWORD dwTime);//ɾ��ѹ���ݶȱ��� -
extern int WINAPI LC_p_GetPumpGradsTable(int iCanID, BYTE dbLine);//��ѯѹ���ݶȱ� -
extern int WINAPI LC_p_GetPumpPressureZero(int iCanID);//��ѯ��ѹ����λֵ -
extern int WINAPI LC_p_RunPumpCmd(int iCanID, enum LCPumpCmd eCmd);//��ִ������ -
extern int WINAPI LC_p_RunPumpSwitch(int iCanID, BOOL bOnFlag);//ִ�бÿ��� -
extern int WINAPI LC_p_RunPumpWash(int iCanID, BOOL bOnFlag);//ִ�бó�ϴ -
extern int WINAPI LC_p_RunPumpGradsReady(int iCanID, BOOL bOnFlag);//ִ���ݶ�׼�� -
extern int WINAPI LC_p_RunPumpGrads(int iCanID, BOOL bOnFlag);//ִ���ݶ� -
extern int WINAPI LC_p_GetPumpTime(int iCanID);//��ѯ������ʱ�� -
//ɫ���첽����(����)
extern int WINAPI LC_p_RunGCSample(int iCanID, BYTE dbChannel);//��ʼ���� -
extern int WINAPI LC_p_GetGCRealtimeData(int iCanID, BYTE dbChannel);//ʵʱ���� -
//�¿��첽����(����)
extern int WINAPI LC_p_GetTempBPS(int iCanID);//��ȡ�¿�ͨѶ���� -
extern int WINAPI LC_p_SetTempBPS(int iCanID, WORD wBPS);//�趨�¿�ͨѶ���� -
extern int WINAPI LC_p_GetTempTemp(int iCanID, BYTE dbAddr);//��ѯ�趨�¶� -
extern int WINAPI LC_p_SetTempTemp(int iCanID, BYTE dbAddr, WORD wTemp);//�趨�¶� -
//����Դ�첽����(����)
extern int WINAPI LC_p_GetCCSVol(int iCanID);//��ѯ����Դ���� -
extern int WINAPI LC_p_GetCCSTemp(int iCanID);//��ѯ����Դ�¶� -
extern int WINAPI LC_p_GetCCSTSw(int iCanID);//��ѯ�絼���¶� -
extern int WINAPI LC_p_SetCCSTSw(int iCanID, BYTE eVal);//���õ絼���¶� -
extern int WINAPI LC_p_GetCCSRange(int iCanID);//��ѯ���� -
extern int WINAPI LC_p_SetCCSRange(int iCanID, BYTE eVal);//�������� -
extern int WINAPI LC_p_GetCCSDASw(int iCanID);//��ѯ�������� -
extern int WINAPI LC_p_RunCCSDASw(int iCanID, BYTE bOnFlag);//���ú������� -
extern int WINAPI LC_p_GetCCSDAVal(int iCanID);//��ѯ�������� -
extern int WINAPI LC_p_SetCCSDAVal(int iCanID, BYTE dbVal);//���ú������� -
////////////////////////////////////////
//�豸�첽����(����)
extern int WINAPI LC_r_GetCurrentTime(int iCanID, DWORD &dwTime);//��ѯ��ǰʱ�� -
extern int WINAPI LC_r_SetCurrentTime(int iCanID);//���õ�ǰʱ�� -
extern int WINAPI LC_r_SetCanID(int iCanID);//�趨�豸CAN��ַ -
extern int WINAPI LC_r_GetCanInfo(int iCanID, P_CAN_INFO ptCanInfo);//ѯ��CAN�ڵ����� -
extern int WINAPI LC_r_RunWake(int iCanID);//ִ�л��� -
extern int WINAPI LC_r_GetWarning(int iCanID, P_WARNING_INFO ptWarningInfo, int &iNum);//��ѯ���� -
extern int WINAPI LC_r_SetWarning(int iCanID);//������λ -
extern int WINAPI LC_r_GetRealtimeData(int iCanID, P_SAMPLE_RESULT ptSample, int &iNum, LCReturnCode &eStatus, DWORD &dwTime, DWORD &dwWaveLength);//ʵʱ���� -
extern int WINAPI LC_r_SetLogicIndex(int iCanID);//�����豸���߼���� -
//�����첽����(����)
extern int WINAPI LC_r_GetUVStatus(int iCanID, BYTE &dbSysStatus, BYTE &dbWorkStatus);//��ѯ����״̬ -
extern int WINAPI LC_r_GetUVWorkData(int iCanID, P_UV_WORK_DATA ptUVWorkData, int &iNum, BOOL bMultiData);//��ѯ���⹤������ -
extern int WINAPI LC_r_GetUVDeuteriumLampTime(int iCanID, WORD &wDeuteriumTime);//��ѯ뮵��ۼ�����ʱ -
extern int WINAPI LC_r_GetUVWavelength(int iCanID, WORD &wWavelength);//��ѯ�趨���� -
extern int WINAPI LC_r_GetUVWavelengthTable(int iCanID, BYTE &dbTotalLines, BYTE &dbNowLine, P_WAVELENGTH_TABLE pwt);//��ѯ���������
extern int WINAPI LC_r_GetUVWavelengthScanRange(int iCanID, WORD &wStartWavelength, WORD &wEndWavelength);//��ѯ����ɨ�跶Χ -
extern int WINAPI LC_r_SetUVWavelengthSmooth(int iCanID);//�趨������ƽ��ָ�� -
extern int WINAPI LC_r_SetUVWavelengthScan(int iCanID);//�趨(ִ��)����ɨ�� -
extern int WINAPI LC_r_ClsUVWavelengthTable(int iCanID);//������������ -
extern int WINAPI LC_r_SetUVWavelengthTable(int iCanID);//����/�޸Ĳ�������� -
extern int WINAPI LC_r_DelUVWavelengthTable(int iCanID);//ɾ������������� -
extern int WINAPI LC_r_SetUVWavelengthZero(int iCanID);//���ĵ������� -
extern int WINAPI LC_r_AdjUVWavelength(int iCanID);//У׼���� -
extern int WINAPI LC_r_SetUVDeuteriumLampTime(int iCanID);//����뮵�ʱ�� -
extern int WINAPI LC_r_RunUVWavelengthScan(int iCanID);//ִ�в���ɨ�� -
extern int WINAPI LC_r_RunUVDeuteriumLamp(int iCanID);//ִ��뮵ƿ��� -
extern int WINAPI LC_r_RunUVWavelengthTable(int iCanID);//ִ�г��򲨳� -
extern int WINAPI LC_r_RunUVWavelengthZero(int iCanID);//ִ�е��� -
extern int WINAPI LC_r_RunUVWavelengthSelfCheck(int iCanID);//�����Լ� -
extern int WINAPI LC_r_SetUVWavelengthAdjust(int iCanID);//�趨����У��ֵ -
extern int WINAPI LC_r_GetUVWavelengthAdjust(int iCanID, short &sAdjustValue);//��ѯ����У��ֵ -
//���첽����(����)
extern int WINAPI LC_r_GetPumpConstantSpeed(int iCanID, WORD &wConstantSpeed);//��ѯ�����ٶ� -
extern int WINAPI LC_r_GetPumpGland(int iCanID, DWORD &dwLeftGland, DWORD &dwRightGland);//��ѯ�ܷ�Ȧ -
extern int WINAPI LC_r_SetPumpGlandZero(int iCanID);//�ܷ�Ȧ�������� -
extern int WINAPI LC_r_GetPumpPressure(int iCanID, WORD &wPressure, BYTE &dbUnit, DWORD &dwPressureRange, WORD &wVelocity, BYTE &dbSysStatus, BYTE &dbWorkStatus);//��ѯ��״̬ -
extern int WINAPI LC_r_SetPumpConstantSpeed(int iCanID);//���ú����ٶ� -
extern int WINAPI LC_r_SetPumpPressure(int iCanID);//����ѹ��ֵ��Χ -
extern int WINAPI LC_r_SetPumpWashParam(int iCanID);//����ϴ�Ѳ��� -
extern int WINAPI LC_r_GetPumpWashParam(int iCanID, WORD &wVelocity, WORD &wTime);//��ѯϴ�Ѳ��� -
extern int WINAPI LC_r_SetPumpPressureZero(int iCanID);//����ѹ����ʾ��λ -
extern int WINAPI LC_r_SetPumpRippleCompress(int iCanID);//��������ѹ�� -
extern int WINAPI LC_r_ClsPumpGradsTable(int iCanID);//���ѹ���ݶȱ� -
extern int WINAPI LC_r_SetPumpGradsTable(int iCanID);//����/�޸�ѹ���ݶȱ� -
extern int WINAPI LC_r_DelPumpGradsTable(int iCanID);//ɾ��ѹ���ݶȱ��� -
extern int WINAPI LC_r_GetPumpGradsTable(int iCanID, BYTE &dbTotalLines, BYTE &dbNowLine, P_GRADS_TABLE pgt);//��ѯѹ���ݶȱ� -
extern int WINAPI LC_r_GetPumpPressureZero(int iCanID, WORD &wPressureZero);//��ѯ��ѹ����λֵ -
extern int WINAPI LC_r_RunPumpCmd(int iCanID);//��ִ������ -
extern int WINAPI LC_r_RunPumpSwitch(int iCanID);//ִ�бÿ��� -
extern int WINAPI LC_r_RunPumpWash(int iCanID);//ִ�бó�ϴ -
extern int WINAPI LC_r_RunPumpGradsReady(int iCanID);//ִ���ݶ�׼�� -
extern int WINAPI LC_r_RunPumpGrads(int iCanID);//ִ���ݶ� -
extern int WINAPI LC_r_GetPumpTime(int iCanID, DWORD &dwPumpTime);//��ѯ������ʱ�� -
//ɫ���첽����(����)
extern int WINAPI LC_r_RunGCSample(int iCanID);//��ʼ���� -
extern int WINAPI LC_r_GetGCRealtimeData(int iCanID, P_SAMPLE_RESULT ptSample, int &iNum, BOOL &bStartFlag);//ʵʱ���� -
//�¿��첽����(����)
extern int WINAPI LC_r_GetTempBPS(int iCanID, WORD &wBPS);//��ȡ�¿�ͨѶ���� -
extern int WINAPI LC_r_SetTempBPS(int iCanID);//�趨�¿�ͨѶ���� -
extern int WINAPI LC_r_GetTempTemp(int iCanID, BYTE &dbAddr, WORD &wTrueTemp, WORD &wTemp);//��ѯ�趨�¶� -
extern int WINAPI LC_r_SetTempTemp(int iCanID);//�趨�¶� -
//����Դ�첽����(����)
extern int WINAPI LC_r_GetCCSVol(int iCanID, WORD &wVol);//��ѯ����Դ���� -
extern int WINAPI LC_r_GetCCSTemp(int iCanID, WORD &wTemp);//��ѯ�絼���¶� -
extern int WINAPI LC_r_GetCCSTSw(int iCanID, BYTE &eVal);//��ѯ�絼���¶ȿ��� -
extern int WINAPI LC_r_SetCCSTSw(int iCanID);//���õ絼���¶ȿ��� -
extern int WINAPI LC_r_GetCCSRange(int iCanID, BYTE &eVal);//��ѯ���̿��� -
extern int WINAPI LC_r_SetCCSRange(int iCanID);//�������̿��� -
extern int WINAPI LC_r_GetCCSDASw(int iCanID, BYTE &bOnFlag);//��ѯ�������� -
extern int WINAPI LC_r_RunCCSDASw(int iCanID);//���ú������� -
extern int WINAPI LC_r_GetCCSDAVal(int iCanID, BYTE &dbVal, BYTE &dbNowVal);//��ѯ�������� -
extern int WINAPI LC_r_SetCCSDAVal(int iCanID);//���ú������� -

////////////////////////////////////////////////////////////////////////////////
//ͬ��������
//�豸ͬ������ -
extern int WINAPI LC_GetCurrentTime(int iCanID, DWORD &dwTime);//��ѯ��ǰʱ�� -
extern int WINAPI LC_SetCurrentTime(int iCanID, DWORD dwTime);//���õ�ǰʱ�� -
extern int WINAPI LC_SetCanID(int iCanID, int iNewCanID);//�趨�豸CAN��ַ -
extern int WINAPI LC_GetCanInfo(int iCanID, P_CAN_INFO ptCanInfo, int &iNum);//ѯ��CAN�ڵ����� -
extern int WINAPI LC_RunWake(int iCanID, BOOL bOnFlag);//ִ�л��� -
extern int WINAPI LC_GetWarning(int iCanID, BYTE dbType, BYTE dbIdx, P_WARNING_INFO ptWarningInfo, int &iNum);//��ѯ���� -
extern int WINAPI LC_SetWarning(int iCanID, BYTE dbType, BYTE dbIdx, WORD wIdx);//������λ -
extern int WINAPI LC_GetRealtimeData(int iCanID, P_SAMPLE_RESULT ptSample, int &iNum, LCReturnCode &eStatus, DWORD &dwTime, DWORD &dwWaveLength);//ʵʱ���� -
extern int WINAPI LC_SetLogicIndex(int iCanID, WORD wLogicIndex);//�����豸���߼���� -
//����ͬ������ -
extern int WINAPI LC_GetUVStatus(int iCanID, BYTE &dbSysStatus, BYTE &dbWorkStatus);//��ѯ����״̬ -
extern int WINAPI LC_GetUVWorkData(int iCanID, P_UV_WORK_DATA ptUVWorkData, int &iNum, BOOL bMultiData = FALSE);//��ѯ���⹤������ -
extern int WINAPI LC_GetUVDeuteriumLampTime(int iCanID, WORD &wDeuteriumTime);//��ѯ뮵��ۼ�����ʱ -
extern int WINAPI LC_GetUVWavelength(int iCanID, WORD &wWavelength);//��ѯ�趨���� -
extern int WINAPI LC_GetUVWavelengthTable(int iCanID, BYTE dbLine, BYTE &dbTotalLines, BYTE &dbNowLine, P_WAVELENGTH_TABLE pwt);//��ѯ���������
extern int WINAPI LC_GetUVWavelengthScanRange(int iCanID, WORD &wStartWavelength, WORD &wEndWavelength);//��ѯ����ɨ�跶Χ -
extern int WINAPI LC_SetUVWavelengthSmooth(int iCanID, WORD wWavelength, BYTE dbSmoothValue);//�趨������ƽ��ָ�� -
extern int WINAPI LC_SetUVWavelengthScan(int iCanID, BYTE dbWavelengthScan, WORD wStartWavelength, WORD wEndWavelength);//�趨(ִ��)����ɨ�� -
extern int WINAPI LC_ClsUVWavelengthTable(int iCanID);//������������ -
extern int WINAPI LC_SetUVWavelengthTable(int iCanID, P_WAVELENGTH_TABLE pwt);//����/�޸Ĳ�������� -
extern int WINAPI LC_DelUVWavelengthTable(int iCanID, DWORD dwTime);//ɾ������������� -
extern int WINAPI LC_SetUVWavelengthZero(int iCanID, BOOL bZeroFlag);//���ĵ������� -
extern int WINAPI LC_AdjUVWavelength(int iCanID, WORD wParam);//У׼���� -
extern int WINAPI LC_SetUVDeuteriumLampTime(int iCanID);//����뮵�ʱ�� -
extern int WINAPI LC_RunUVWavelengthScan(int iCanID, BOOL bOnFlag);//ִ�в���ɨ�� -
extern int WINAPI LC_RunUVDeuteriumLamp(int iCanID, BOOL bOnFlag);//ִ��뮵ƿ��� -
extern int WINAPI LC_RunUVWavelengthTable(int iCanID, BOOL bOnFlag);//ִ�г��򲨳� -
extern int WINAPI LC_RunUVWavelengthZero(int iCanID);//ִ�е��� -
extern int WINAPI LC_RunUVWavelengthSelfCheck(int iCanID);//�����Լ� -
extern int WINAPI LC_SetUVWavelengthAdjust(int iCanID, short sAdjustValue);//�趨����У��ֵ -
extern int WINAPI LC_GetUVWavelengthAdjust(int iCanID, short &sAdjustValue);//��ѯ����У��ֵ -
//��ͬ������ -
extern int WINAPI LC_GetPumpConstantSpeed(int iCanID, WORD &wConstantSpeed);//��ѯ�����ٶ� -
extern int WINAPI LC_GetPumpGland(int iCanID, DWORD &dwLeftGland, DWORD &dwRightGland);//��ѯ�ܷ�Ȧ -
extern int WINAPI LC_SetPumpGlandZero(int iCanID);//�ܷ�Ȧ�������� -
extern int WINAPI LC_GetPumpPressure(int iCanID, WORD &wPressure, BYTE &dbUnit, DWORD &dwPressureRange, WORD &wVelocity, BYTE &dbSysStatus, BYTE &dbWorkStatus);//��ѯ��״̬ -
extern int WINAPI LC_SetPumpConstantSpeed(int iCanID, WORD wConstantSpeed);//���ú����ٶ� -
extern int WINAPI LC_SetPumpPressure(int iCanID, WORD wPressureHigh, WORD wPressureLow, BYTE dbUnit);//����ѹ��ֵ��Χ -
extern int WINAPI LC_SetPumpWashParam(int iCanID, WORD wVelocity, WORD wTime);//����ϴ�Ѳ��� -
extern int WINAPI LC_GetPumpWashParam(int iCanID, WORD &wVelocity, WORD &wTime);//��ѯϴ�Ѳ��� -
extern int WINAPI LC_SetPumpPressureZero(int iCanID, WORD wPressureZero);//����ѹ����ʾ��λ -
extern int WINAPI LC_SetPumpRippleCompress(int iCanID, WORD wRippleCompress);//��������ѹ�� -
extern int WINAPI LC_ClsPumpGradsTable(int iCanID);//���ѹ���ݶȱ� -
extern int WINAPI LC_SetPumpGradsTable(int iCanID, P_GRADS_TABLE pgt);//����/�޸�ѹ���ݶȱ� -
extern int WINAPI LC_DelPumpGradsTable(int iCanID, DWORD dwTime);//ɾ��ѹ���ݶȱ��� -
extern int WINAPI LC_GetPumpGradsTable(int iCanID, BYTE dbLine, BYTE &dbTotalLines, BYTE &dbNowLine, P_GRADS_TABLE pgt);//��ѯѹ���ݶȱ� -
extern int WINAPI LC_GetPumpPressureZero(int iCanID, WORD &wPressureZero);//��ѯ��ѹ����λֵ -
extern int WINAPI LC_RunPumpCmd(int iCanID, enum LCPumpCmd eCmd);//��ִ������ -
extern int WINAPI LC_RunPumpSwitch(int iCanID, BOOL bOnFlag);//ִ�бÿ��� -
extern int WINAPI LC_RunPumpWash(int iCanID, BOOL bOnFlag);//ִ�бó�ϴ -
extern int WINAPI LC_RunPumpGradsReady(int iCanID, BOOL bOnFlag);//ִ���ݶ�׼�� -
extern int WINAPI LC_RunPumpGrads(int iCanID, BOOL bOnFlag);//ִ���ݶ� -
extern int WINAPI LC_GetPumpTime(int iCanID, DWORD &dwPumpTime);//��ѯ������ʱ�� -
//ɫ��ͬ������ -
extern int WINAPI LC_RunGCSample(int iCanID, BYTE dbChannel);//��ʼ���� -
extern int WINAPI LC_GetGCRealtimeData(int iCanID, BYTE dbChannel, P_SAMPLE_RESULT ptSample, int &iNum, BOOL &bStartFlag);//ʵʱ���� -
extern void WINAPI LC_TranslateGCRealtimeData(P_SAMPLE_RESULT ptDst, P_SAMPLE_RESULT ptSrc, int iNum, BYTE eVal);//��������ת����� -
//�¿�ͬ������ -
extern int WINAPI LC_GetTempBPS(int iCanID, WORD &wBPS);//��ȡ�¿�ͨѶ���� -
extern int WINAPI LC_SetTempBPS(int iCanID, WORD wBPS);//�趨�¿�ͨѶ���� -
extern int WINAPI LC_GetTempTemp(int iCanID, BYTE dbAddr, WORD &wTrueTemp, WORD &wTemp);//��ѯ�趨�¶� -
extern int WINAPI LC_SetTempTemp(int iCanID, BYTE dbAddr, WORD wTemp);//�趨�¶� -
//����Դͬ������ -
extern int WINAPI LC_GetCCSVol(int iCanID, WORD &wVol);//��ѯ����Դ���� -
extern int WINAPI LC_GetCCSTemp(int iCanID, WORD &wTemp);//��ѯ�絼���¶� -
extern int WINAPI LC_GetCCSTSw(int iCanID, BYTE &eVal);//��ѯ�絼���¶ȿ��� -
extern int WINAPI LC_SetCCSTSw(int iCanID, BYTE eVal);//���õ絼���¶ȿ��� -
extern int WINAPI LC_GetCCSRange(int iCanID, BYTE &eVal);//��ѯ���̿��� -
extern int WINAPI LC_SetCCSRange(int iCanID, BYTE eVal);//�������̿��� -
extern int WINAPI LC_GetCCSDASw(int iCanID, BYTE &bOnFlag);//��ѯ�������� -
extern int WINAPI LC_RunCCSDASw(int iCanID, BYTE bOnFlag);//���ú������� -
extern int WINAPI LC_GetCCSDAVal(int iCanID, BYTE &dbVal, BYTE &dbNowVal);//��ѯ�������� -
extern int WINAPI LC_SetCCSDAVal(int iCanID, BYTE dbVal);//���ú������� -

//�����Ӧ������ -
extern int WINAPI LC_ResponseCanInfo(int iCanID);

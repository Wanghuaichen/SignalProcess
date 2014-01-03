#include "stdafx.h"

#define LC_CAN_DLL_PROC_ENTITY
#include "LC_CAN.h"

#include "LC_CANx.h"

#define LC_LENGTH_OFS	1
#define LC_PARAM_OFS	7
#define LC_TYPE_OFS		4
#define LC_ADDR_OFS		5
#define LC_SUBCMD_OFS	6
#define LC_RETURN_ERRCODE(s)	(s[LC_TYPE_OFS]>=0x60)
#define PC_CAN_ID		0

////////////////////////////////////////
//�ֲ�����::Э���װ
static void WINAPI _xor_Data(LPBYTE p, int iStart, int iEnd)
{
	int i;
	BYTE dbXor = 0;
	for (i = iStart; i < iEnd; i++) {
		dbXor ^= p[i];
	}
	p[i] = dbXor;
}
static void WINAPI _make_LCCmd(BYTE dbLCCmd[MAX_LC_BUFFER_SIZE], enum LCType eType, enum LCSubCmd eSubCmd, int iParaLen = 0, LPBYTE lpdbPara = NULL)
{
	dbLCCmd[0] = dbLCCmd[3] = 0x68;
	dbLCCmd[1] = (BYTE)(iParaLen + 5);
	dbLCCmd[2] = ~dbLCCmd[1];
	dbLCCmd[4] = (BYTE)eType;
	dbLCCmd[5] = 0;
	dbLCCmd[6] = (BYTE)eSubCmd;
	if (lpdbPara) CopyMemory(&dbLCCmd[LC_PARAM_OFS], lpdbPara, iParaLen);
	_xor_Data(dbLCCmd, 4, iParaLen + LC_PARAM_OFS);
	dbLCCmd[iParaLen + LC_PARAM_OFS + 1] = 0x16;
}
////////////////////////////////////////
//�ֲ�����::����ת��
__declspec(naked) void __cdecl LSB2MSB16(LPVOID lpMSB, WORD wLSB)
{
	__asm {
		push	ebp
		mov		ebp, esp
		mov		edx, lpMSB
		mov		ax, wLSB
		xchg	al, ah
		mov		[edx], ax
		leave
		ret
	}
}
__declspec(naked) void __cdecl LSB2MSB32(LPVOID lpMSB, DWORD dwLSB)
{
	__asm {
		push	ebp
		mov		ebp, esp
		mov		edx, lpMSB
		mov		eax, dwLSB
		xchg	al, ah
		ror		eax, 16
		xchg	al, ah
		mov		[edx], eax
		leave
		ret
	}
}
__declspec(naked) WORD __cdecl MSB2LSB16(LPCVOID lpMSB)
{
	__asm {
		push	ebp
		mov		ebp, esp
		mov		edx, lpMSB
		mov		ax, [edx]
		xchg	al, ah
		leave
		ret
	}
}
__declspec(naked) DWORD __cdecl MSB2LSB32(LPCVOID lpMSB)
{
	__asm {
		push	ebp
		mov		ebp, esp
		mov		edx, lpMSB
		mov		eax, [edx]
		xchg	al, ah
		ror		eax, 16
		xchg	al, ah
		leave
		ret
	}
}
int WINAPI BCD2INT(LPVOID lpBCD, int iLen)
{
	int i,iValue;
	LPBYTE p = (LPBYTE)lpBCD;
	for (iValue = 0, i = 1; i < iLen; i++) {
		iValue *= 10;
		if (i & 1) {
			iValue += (int)(*p & 0x0F);
			p++;
		}
		else {
			iValue += (int)(*p >> 4);
		}
	}
	if (*(LPBYTE)lpBCD & 0x80) {
		iValue = -iValue;
	}
	return iValue;
}
int WINAPI INT2BCD(LPVOID lpBCD, int iInt, int iLen)
{
	int i,j;
	BYTE dbTmp[32];
	LPBYTE p = (LPBYTE)lpBCD;
	if (iInt < 0) {
		*p = 0x80;
		iInt = -iInt;

	}
	else {
		*p = 0;
	}
	for (i = 1; i < iLen; i++) {
		dbTmp[i] = (BYTE)(iInt % 10);
		iInt /= 10;
	}
	for (j = 1; i > 1; j++) {
		i--;
		if (j & 1) {
			*p |= dbTmp[i];
			p++;
		}
		else {
			*p = dbTmp[i] << 4;
		}
		
	}
	return j;
}

////////////////////////////////////////
//�豸�������Ӻ��� -
static CRITICAL_SECTION l_hCriticalSection;//�ٽ������ڷ�ֹ���߳�ͬʱ���� -
static BOOL l_bCriticalSectionActiveFlag = FALSE;//�ٽ����Ƿ���Ч -
BOOL WINAPI LC_initDLL()//��ʼ��DLL���� -
{
	BOOL bRet;
	bRet = LC_CAN_DLL_INIT(NULL);
	if (!l_bCriticalSectionActiveFlag) {
		InitializeCriticalSection(&l_hCriticalSection);
		l_bCriticalSectionActiveFlag = TRUE;
	}
	return bRet;
}
void WINAPI LC_exitDLL()
{
	if (l_bCriticalSectionActiveFlag) {
		l_bCriticalSectionActiveFlag = FALSE;
		DeleteCriticalSection(&l_hCriticalSection);
	}
}
typedef struct {
	int iDevType;
	int iLevel;
}	VCOM2CAN_FIND_ORDER;
#define VCOM2CAN_DEV_NUM	2
static VCOM2CAN_FIND_ORDER l_tVCom2CanFindOrder[VCOM2CAN_DEV_NUM] = {
	{SHP_USB2CAN, 0},
	{RBT_USB2CAN, 1},
};
void WINAPI LC_setVCOM2CANFindOrder(int iDevType, int iLevel)//���ô���תCAN���豸�Ĳ���˳�� -
{
	int i;
	if (iLevel > VCOM2CAN_DEV_NUM) iLevel = VCOM2CAN_DEV_NUM;
	for (i = 0; i < VCOM2CAN_DEV_NUM; i++) {
		if (l_tVCom2CanFindOrder[i].iDevType == iDevType) l_tVCom2CanFindOrder[i].iLevel = iLevel;
	}
}
BOOL WINAPI LC_Connect(int &iComIdx)//�����豸 -
{
	BOOL bRet;
	int i,j,iParas[3] = {0, 0, 0};
	if (l_bCriticalSectionActiveFlag) EnterCriticalSection(&l_hCriticalSection);
	if (!iComIdx) {//������USBCAN-I�豸 -
		bRet = LC_CAN_init(PC_CAN_ID, VCI_USBCAN1, 0, iParas);
	}
	else {//���മ��תCAN���豸 -
		for (bRet = FALSE, i = 0; i <= VCOM2CAN_DEV_NUM; i++) {
			for (j = 0; j < VCOM2CAN_DEV_NUM; j++) {
				if (l_tVCom2CanFindOrder[j].iLevel == i) {
					iParas[0] = iComIdx;
					bRet = LC_CAN_init(PC_CAN_ID, l_tVCom2CanFindOrder[j].iDevType, 0, iParas);
					if (bRet) {//�ɹ��ҵ��豸 -
						if (iComIdx < 0) iComIdx = iParas[0];//������Զ����Ҵ��ںŵĳ��ϣ�����ʵ�ʴ��ں� -
						i = VCOM2CAN_DEV_NUM;
						break;
					}
				}
			}
		}
	}
	if (l_bCriticalSectionActiveFlag) LeaveCriticalSection(&l_hCriticalSection);
	return bRet;
}
void WINAPI LC_Close()//�ر��豸 -
{
	if (l_bCriticalSectionActiveFlag) EnterCriticalSection(&l_hCriticalSection);
	LC_CAN_exit();
	if (l_bCriticalSectionActiveFlag) LeaveCriticalSection(&l_hCriticalSection);
}
void WINAPI LC_setWindowMessage(HWND hWnd, UINT uMsg)//���ô��ھ������Ϣ -
{
	LC_CAN_setWindowMessage(hWnd, uMsg);
}
void WINAPI LC_setCallbackFunction(BOOL (CALLBACK* pCallback)(int, LPBYTE, LPVOID), LPVOID lpParam)//���ûص����� -
{
	LC_CAN_setCallbackFunction(pCallback, lpParam);
}

#define WAIT_TIME_SPACE	50
static DWORD volatile l_dwWaitTimeout = 3000;
void WINAPI LC_setWaitTimeout(DWORD dwWaitTimeout)
{
	l_dwWaitTimeout = dwWaitTimeout;
}

////////////////////////////////////////
//ͨѶ��װ -
extern void g_TraceWithTime(LPCTSTR s);
static int WINAPI _lc_can_send(int iCanID, LPBYTE lpdbData)
{
#ifdef TWEN_TRACE_COMMAND
	CString cmd;
	cmd.Format(_T("��ַ:%x;����:"), iCanID);
	CString x;
	for(int i=0; i<20; i++)
	{
		x.Format(_T("%x"), lpdbData[i]);
		cmd+=x;
	}
	g_TraceWithTime(_T("[��ʼ]") + cmd + _T("\n"));
#endif
	BOOL bRet;
	if (l_bCriticalSectionActiveFlag) EnterCriticalSection(&l_hCriticalSection);
	bRet = LC_CAN_send(iCanID, lpdbData);
	if (l_bCriticalSectionActiveFlag) LeaveCriticalSection(&l_hCriticalSection);
#ifdef TWEN_TRACE_COMMAND
	g_TraceWithTime(_T("[����]") + cmd + _T("\n"));
#endif
	return (bRet) ? 0 : -1;
}
static int WINAPI _lc_can_recvSpecial(LPBYTE lpdbData, int iCanID, enum LCType eType, enum LCSubCmd eSubCmd)
{
#ifdef TWEN_TRACE_COMMAND
	CString cmd;
	cmd.Format(_T("��ַ:%x;LCType:%x;LCSubCmd:%x;����:"), iCanID, eType, eSubCmd);
	CString x;
	for(int i=0; i<20; i++)
	{
		x.Format(_T("%x"), lpdbData[i]);
		cmd+=x;
	}
	g_TraceWithTime(_T("[��ʼ]") + cmd + _T("\n"));
#endif
	DWORD i;
	BOOL bRet;
	for (i = 0; i <= l_dwWaitTimeout; i += WAIT_TIME_SPACE) {
		if (l_bCriticalSectionActiveFlag) EnterCriticalSection(&l_hCriticalSection);
		bRet = LC_CAN_recvSpecial(lpdbData, iCanID, (BYTE)eType, (BYTE)eSubCmd);
		if (l_bCriticalSectionActiveFlag) LeaveCriticalSection(&l_hCriticalSection);
		if (bRet)
		{
#ifdef TWEN_TRACE_COMMAND
			g_TraceWithTime(_T("[����]") + cmd + _T("\n"));
#endif
			return 0;
		}
		Sleep(WAIT_TIME_SPACE);
	}
#ifdef TWEN_TRACE_COMMAND
	g_TraceWithTime(_T("[��ʱ]") + cmd + _T("\n"));
#endif
	return -1;
}
static int WINAPI _lc_can_listResponseID(int iCanID, int* piID)
{
	int iIDType;
	BOOL bRet;
	switch(iCanID) {
	case lcDC_All:
		iIDType = -1;
		break;
	case lcDC_AllPump:
		iIDType = 0x300;
		break;
	default:
		iIDType = iCanID;
	}
	Sleep(3000);//���ù̶��ȴ�ʱ��
	//Sleep(l_dwWaitTimeout);
	if (l_bCriticalSectionActiveFlag) EnterCriticalSection(&l_hCriticalSection);
	bRet = LC_CAN_listResponseID(iIDType, piID);
	if (l_bCriticalSectionActiveFlag) LeaveCriticalSection(&l_hCriticalSection);
	return bRet;
}

////////////////////////////////////////
//�첽������
//�豸�첽����(����)
int WINAPI LC_p_GetCurrentTime(int iCanID)//��ѯ��ǰʱ�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_All_Time);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetCurrentTime(int iCanID, DWORD dwTime)//���õ�ǰʱ�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS], dwTime);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_All_Time, 4);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetCanID(int iCanID, int iNewCanID)//�趨�豸CAN��ַ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS], iNewCanID);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_All_Can, 4);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetCanInfo(int iCanID)//ѯ��CAN�ڵ����� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_broadcast, lcsc_All_Can);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunWake(int iCanID, BOOL bOnFlag)//ִ������ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_All_Rest, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetWarning(int iCanID, BYTE dbType, BYTE dbIdx)//��ѯ���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbType;
	dbLCCmd[LC_PARAM_OFS + 1] = dbIdx;
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS + 2], 0);
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS + 6], 0);
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_All_Warning, 10);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetWarning(int iCanID, BYTE dbType, BYTE dbIdx, WORD wIdx)//������λ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbType;
	dbLCCmd[LC_PARAM_OFS + 1] = dbIdx;
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 2], wIdx);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_All_Warning, 4);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetRealtimeData(int iCanID)//ʵʱ���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_All_RealtimeData);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetLogicIndex(int iCanID, WORD wLogicIndex)//�����豸���߼���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wLogicIndex);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_All_LogicIndex, 2);
	return _lc_can_send(iCanID, dbLCCmd);
};
//�����첽����(����)
int WINAPI LC_p_GetUVStatus(int iCanID)//��ѯ����״̬ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_UV_Status);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetUVWorkData(int iCanID, BOOL bMultiData)//��ѯ���⹤������ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, (bMultiData) ? lcsc_UV_WorkMultiData : lcsc_UV_WorkData);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetUVDeuteriumLampTime(int iCanID)//��ѯ뮵��ۼ�����ʱ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_UV_DeuteriumLamp);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetUVWavelength(int iCanID)//��ѯ�趨���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_UV_Wavelength);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetUVWavelengthTable(int iCanID, BYTE dbLine)//��ѯ���������
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_UV_WavelengthTable_Get);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetUVWavelengthScanRange(int iCanID)//��ѯ����ɨ�跶Χ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_UV_WavelengthScan);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetUVWavelengthSmooth(int iCanID, WORD wWavelength, BYTE dbSmoothValue)//�趨������ƽ��ָ�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wWavelength);
	dbLCCmd[LC_PARAM_OFS + 2] = dbSmoothValue;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_Wavelength, 3);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetUVWavelengthScan(int iCanID, BYTE dbWavelengthScan, WORD wStartWavelength, WORD wEndWavelength)//�趨(ִ��)����ɨ�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbWavelengthScan;
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 1], wStartWavelength);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 3], wEndWavelength);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_WavelengthScan, 5);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_ClsUVWavelengthTable(int iCanID)//������������ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_WavelengthTable_Cls);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetUVWavelengthTable(int iCanID, P_WAVELENGTH_TABLE pwt)//����/�޸Ĳ�������� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS], pwt->dwTime);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 4], pwt->wWavelength);
	dbLCCmd[LC_PARAM_OFS + 6] = pwt->dbSampleSpeed;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_WavelengthTable_Add, 7);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_DelUVWavelengthTable(int iCanID, DWORD dwTime)//ɾ������������� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS], dwTime);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_WavelengthTable_Del, 4);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetUVWavelengthZero(int iCanID, BOOL bZeroFlag)//���ĵ������� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bZeroFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_WavelengthAdjust_Zero, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_AdjUVWavelength(int iCanID, WORD wParam)//У׼���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wParam);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_WavelengthAdjust_Adj, 2);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetUVDeuteriumLampTime(int iCanID)//����뮵�ʱ�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = 1;//ִ�д���	����Ϊ1
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_DeuteriumLamp, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunUVWavelengthScan(int iCanID, BOOL bOnFlag)//ִ�в���ɨ�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_Run_WavelengthScan, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunUVDeuteriumLamp(int iCanID, BOOL bOnFlag)//ִ��뮵ƿ��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_Run_DeuteriumLamp, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunUVWavelengthTable(int iCanID, BOOL bOnFlag)//ִ�г��򲨳� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_Run_WavelengthTable, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunUVWavelengthZero(int iCanID)//ִ�е��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = 1;//ִ�ж���	����Ϊ1
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_Run_Zero, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunUVWavelengthSelfCheck(int iCanID)//�����Լ� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_WavelengthAdjust_Chk);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetUVWavelengthAdjust(int iCanID, short sAdjustValue)//�趨����У��ֵ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	WORD wValue;
	//����У��ֵ��Ҫ���⴦�� -
	if (sAdjustValue < 0)
		wValue = (-sAdjustValue & 0xFF) | 0x0100;
	else
		wValue = sAdjustValue & 0xFF;
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wValue);

	_make_LCCmd(dbLCCmd, lcT_set, lcsc_UV_WavelengthAdjust, 2);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetUVWavelengthAdjust(int iCanID)//��ѯ����У��ֵ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_UV_WavelengthAdjust);
	return _lc_can_send(iCanID, dbLCCmd);
}
//���첽����(����)
int WINAPI LC_p_GetPumpConstantSpeed(int iCanID)//��ѯ�����ٶ� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Pump_Speed);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetPumpGland(int iCanID)//��ѯ�ܷ�Ȧ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Pump_Gland);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetPumpGlandZero(int iCanID)//�ܷ�Ȧ�������� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Gland);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetPumpPressure(int iCanID)//��ѯ��״̬ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Pump_Status);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetPumpConstantSpeed(int iCanID, WORD wConstantSpeed)//���ú����ٶ� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wConstantSpeed);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Speed, 2);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetPumpPressure(int iCanID, WORD wPressureHigh, WORD wPressureLow, BYTE dbUnit)//����ѹ��ֵ��Χ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wPressureHigh);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 2], wPressureLow);
	dbLCCmd[LC_PARAM_OFS + 4] = dbUnit;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Pressure, 5);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetPumpWashParam(int iCanID, WORD wVelocity, WORD wTime)//����ϴ�Ѳ��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wVelocity);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 2], wTime);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Wash, 4);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetPumpWashParam(int iCanID)//��ѯϴ�Ѳ��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Pump_Wash);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetPumpPressureZero(int iCanID, WORD wPressureZero)//����ѹ����ʾ��λ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wPressureZero);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_PressureZero, 2);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetPumpRippleCompress(int iCanID, WORD wRippleCompress)//��������ѹ�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wRippleCompress);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Ripple, 2);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_ClsPumpGradsTable(int iCanID)//���ѹ���ݶȱ� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_GradsTable_Cls);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetPumpGradsTable(int iCanID, P_GRADS_TABLE pgt)//����/�޸�ѹ���ݶȱ� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS], pgt->dwTime);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 4], pgt->wScaleA);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 6], pgt->wScaleB);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 8], pgt->wScaleC);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 10], pgt->wScaleD);
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 12], pgt->wVelocity);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_GradsTable_Add, 14);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_DelPumpGradsTable(int iCanID, DWORD dwTime)//ɾ��ѹ���ݶȱ��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS], dwTime);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_GradsTable_Del, 4);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetPumpGradsTable(int iCanID, BYTE dbLine)//��ѯѹ���ݶȱ� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbLine;
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Pump_GradsTable_Get, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetPumpPressureZero(int iCanID)//��ѯ��ѹ����λֵ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Pump_PressureZero);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunPumpCmd(int iCanID, enum LCPumpCmd eCmd)//��ִ������ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)eCmd;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_RunCmd, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunPumpSwitch(int iCanID, BOOL bOnFlag)//ִ�бÿ��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Switch_Run, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunPumpWash(int iCanID, BOOL bOnFlag)//ִ�бó�ϴ -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Switch_Wash, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunPumpGradsReady(int iCanID, BOOL bOnFlag)//ִ���ݶ�׼�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Switch_GradsReady, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunPumpGrads(int iCanID, BOOL bOnFlag)//ִ���ݶ� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = (BYTE)bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Pump_Switch_Grads, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetPumpTime(int iCanID)//��ѯ������ʱ�� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Pump_Time);
	return _lc_can_send(iCanID, dbLCCmd);
}
//ɫ���첽����(����)
int WINAPI LC_p_RunGCSample(int iCanID, BYTE dbChannel)//��ʼ���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbChannel;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_All_RealtimeData, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetGCRealtimeData(int iCanID, BYTE dbChannel)//ʵʱ���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbChannel;
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_All_RealtimeData, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
//�¿��첽����(����)
int WINAPI LC_p_GetTempBPS(int iCanID)//��ѯ�¿�ͨѶ���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Temp_BPS);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetTempBPS(int iCanID, WORD wBPS)//�趨�¿�ͨѶ���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS], wBPS);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Temp_BPS, 2);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetTempTemp(int iCanID, BYTE dbAddr)//��ѯ�趨�¶� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbAddr;
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_Temp_Temp, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetTempTemp(int iCanID, BYTE dbAddr, WORD wTemp)//�趨�¶� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbAddr;
	LSB2MSB16(&dbLCCmd[LC_PARAM_OFS + 1], wTemp);
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_Temp_Temp, 3);
	return _lc_can_send(iCanID, dbLCCmd);
}
//����Դ�첽����(����)
int WINAPI LC_p_GetCCSVol(int iCanID)//��ѯ����Դ���� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_CCS_Vol);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetCCSTemp(int iCanID)//��ѯ�絼���¶� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_CCS_Temp);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetCCSTSw(int iCanID)//��ѯ�絼���¶ȿ��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_CCS_TSw);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetCCSTSw(int iCanID, BYTE eVal)//���õ絼���¶ȿ��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = eVal;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_CCS_TSw, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetCCSRange(int iCanID)//��ѯ���̿��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_CCS_Range);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetCCSRange(int iCanID, BYTE eVal)//�������̿��� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = eVal;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_CCS_Range, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetCCSDASw(int iCanID)//��ѯ�������� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_CCS_DASw);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_RunCCSDASw(int iCanID, BYTE bOnFlag)//���ú������� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = bOnFlag;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_CCS_DASw, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_GetCCSDAVal(int iCanID)//��ѯ�������� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	_make_LCCmd(dbLCCmd, lcT_query, lcsc_CCS_DAVal);
	return _lc_can_send(iCanID, dbLCCmd);
}
int WINAPI LC_p_SetCCSDAVal(int iCanID, BYTE dbVal)//���ú������� -
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = dbVal;
	_make_LCCmd(dbLCCmd, lcT_set, lcsc_CCS_DAVal, 1);
	return _lc_can_send(iCanID, dbLCCmd);
}

////////////////////////////////////////
//�޲�����Ĺ��� -
static int WINAPI _LC_r_Set_(int iCanID, enum LCType eType, enum LCSubCmd eSubCmd)
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, eType, eSubCmd)) == 0) {
		iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
	}
	return iRet;
}
//�豸�첽����(����)
int WINAPI LC_r_GetCurrentTime(int iCanID, DWORD &dwTime)//��ѯ��ǰʱ�� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_All_Time)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			dwTime = MSB2LSB32(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
int WINAPI LC_r_SetCurrentTime(int iCanID)//���õ�ǰʱ�� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_All_Time);
}
int WINAPI LC_r_SetCanID(int iCanID)//�趨�豸CAN��ַ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_All_Can);
}
int WINAPI LC_r_GetCanInfo(int iCanID, P_CAN_INFO ptCanInfo)//ѯ��CAN�ڵ����� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_broadcast, lcsc_All_Can)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			ptCanInfo->dbType = dbLCData[LC_PARAM_OFS];
			ptCanInfo->iCanID = (int)MSB2LSB32(&dbLCData[LC_PARAM_OFS + 1]);
			ptCanInfo->iDeviceIdx = (int)MSB2LSB32(&dbLCData[LC_PARAM_OFS + 5]);
		}
	}
	return iRet;
}
int WINAPI LC_r_RunWake(int iCanID)//ִ������ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_All_Rest);
}
int WINAPI LC_r_GetWarning(int iCanID, P_WARNING_INFO ptWarningInfo, int &iNum)//��ѯ���� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int i,iRet;
	LPBYTE p;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_All_Warning)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			iNum = (int)(dbLCData[LC_LENGTH_OFS] - 5) / sizeof(WARNING_INFO);
			for (p = &dbLCData[LC_PARAM_OFS], i = 0; i < iNum; i++, p += sizeof(WARNING_INFO)) {
				ptWarningInfo[i].dbType = p[0];
				ptWarningInfo[i].dbIdx = p[1];
				ptWarningInfo[i].dwSetParam = MSB2LSB32(&p[2]);
				ptWarningInfo[i].dwNowParam = MSB2LSB32(&p[6]);
				ptWarningInfo[i].dwTime = MSB2LSB32(&p[10]);
				ptWarningInfo[i].wIdx = MSB2LSB16(&p[14]);
			}
		}
	}
	return iRet;
}
int WINAPI LC_r_SetWarning(int iCanID)//������λ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_All_Warning);
}
#ifdef _DEBUG
static void _saveErrorData(LPCTSTR lpcszFilename, LPVOID lpdbData, DWORD dwLen)
{
	HANDLE hf;
	DWORD dwSize;
	SYSTEMTIME st;
	CString szFilename;
	if (lpcszFilename == NULL) {
		GetLocalTime(&st);
		szFilename.Format(_T("E:\\yx.log\\%u%02u%02u_%02u%02u%02u.LC"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		lpcszFilename = (LPCTSTR)szFilename;
	}
	if ((hf = CreateFile(lpcszFilename, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, NULL)) == INVALID_HANDLE_VALUE) return;
	WriteFile(hf, lpdbData, dwLen, &dwSize, NULL);
	CloseHandle(hf);
}
#endif
int WINAPI LC_r_GetRealtimeData(int iCanID, P_SAMPLE_RESULT ptSample, int &iNum, LCReturnCode &eStatus, DWORD &dwTime, DWORD &dwWaveLength)//ʵʱ���� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int i,j,iRet;
	DWORD dwThisTime,dwLastTime;
	LPBYTE p;
	union {
		LONG lValue;
		float fValue;
	} tValue;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_All_RealtimeData)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) {
			iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
			switch(iRet) {
			case 5://���ⷵ�أ�������ʼ!
			case 8://���ⷵ�أ��ݶȿ�ʼ!
			case 9://���ⷵ�أ��ݶȽ���!
				iNum = 0;
				eStatus = (LCReturnCode)iRet;
				ptSample->dwTime = dwTime = MSB2LSB32(&dbLCData[LC_PARAM_OFS + 2]);
				if (iRet != 5) dwWaveLength = MSB2LSB32(&dbLCData[LC_PARAM_OFS + 6]);
				iRet = lc_OK;
				break;
			}
		}
		else {
			iNum = (int)(dbLCData[LC_LENGTH_OFS] - 5) / sizeof(SAMPLE_RESULT);
			for (p = &dbLCData[LC_PARAM_OFS], j = 0, i = 0; i < iNum; i++, p += sizeof(SAMPLE_RESULT)) {
				dwThisTime = MSB2LSB32(p);
				tValue.lValue = (LONG)MSB2LSB32(&p[4]);//��Э�飺����ֵ��FLOAT����!!!
				if (dwThisTime % 50 || (i && dwThisTime - dwLastTime > 200) || tValue.fValue < -2.0f || tValue.fValue > 2.0f) {
					continue;
				}
				ptSample[j].dwTime = dwThisTime;
				ptSample[j].fValue = tValue.fValue;
				j++;
				dwLastTime = dwThisTime;
			}
			iNum = j;
#ifdef _DEBUG
			if (i != j) {
				_saveErrorData(NULL, dbLCData, MAX_LC_BUFFER_SIZE);
			}
#endif
			eStatus = lc_OK;
		}
	}
	return iRet;
}
int WINAPI LC_r_SetLogicIndex(int iCanID)//�����豸���߼���� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_All_LogicIndex);
}
//�����첽����(����)
int WINAPI LC_r_GetUVStatus(int iCanID, BYTE &dbSysStatus, BYTE &dbWorkStatus)//��ѯ����״̬ -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_UV_Status)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			dbSysStatus = dbLCData[LC_PARAM_OFS];
			dbWorkStatus = dbLCData[LC_PARAM_OFS + 1];
		}
	}
	return iRet;
}
int WINAPI LC_r_GetUVWorkData(int iCanID, P_UV_WORK_DATA ptUVWorkData, int &iNum, BOOL bMultiData)//��ѯ���⹤������ -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int i,iRet;
	DWORD dwValue;
	LPBYTE p;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, (bMultiData) ? lcsc_UV_WorkMultiData : lcsc_UV_WorkData)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			iNum = (int)(dbLCData[LC_LENGTH_OFS] - 5) / sizeof(UV_WORK_DATA);
			for (p = &dbLCData[LC_PARAM_OFS], i = 0; i < iNum; i++, p += sizeof(UV_WORK_DATA)) {
				ptUVWorkData[i].wWavelength = MSB2LSB16(p);
				//�������Ҫ���⴦�� -
				dwValue = MSB2LSB32(&p[2]);
				ptUVWorkData[i].lAbsorbency = (LONG)(dwValue & 0x00FFFFFF);
				if (dwValue >> 24) ptUVWorkData[i].lAbsorbency = -ptUVWorkData[i].lAbsorbency;

				ptUVWorkData[i].dwSampleValue = MSB2LSB32(&p[6]);
				ptUVWorkData[i].dwCompareValue = MSB2LSB32(&p[10]);
			}
		}
	}
	return iRet;
}
int WINAPI LC_r_GetUVDeuteriumLampTime(int iCanID, WORD &wDeuteriumTime)//��ѯ뮵��ۼ�����ʱ -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_UV_DeuteriumLamp)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wDeuteriumTime = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
int WINAPI LC_r_GetUVWavelength(int iCanID, WORD &wWavelength)//��ѯ�趨���� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_UV_Wavelength)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wWavelength = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
int WINAPI LC_r_GetUVWavelengthTable(int iCanID, BYTE &dbTotalLines, BYTE &dbNowLine, P_WAVELENGTH_TABLE pwt)//��ѯ���������
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_UV_WavelengthTable_Get)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			dbTotalLines = dbLCData[LC_PARAM_OFS];
			dbNowLine = dbLCData[LC_PARAM_OFS + 1];
			pwt->dwTime = MSB2LSB32(&dbLCData[LC_PARAM_OFS + 2]);
			pwt->wWavelength = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 6]);
			pwt->dbSampleSpeed = dbLCData[LC_PARAM_OFS + 8];
		}
	}
	return iRet;
}
int WINAPI LC_r_GetUVWavelengthScanRange(int iCanID, WORD &wStartWavelength, WORD &wEndWavelength)//��ѯ����ɨ�跶Χ -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_UV_WavelengthScan)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wStartWavelength = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
			wEndWavelength = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 2]);
		}
	}
	return iRet;
}
int WINAPI LC_r_SetUVWavelengthSmooth(int iCanID)//�趨������ƽ��ָ�� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_Wavelength);
}
int WINAPI LC_r_SetUVWavelengthScan(int iCanID)//�趨(ִ��)����ɨ�� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_WavelengthScan);
}
int WINAPI LC_r_ClsUVWavelengthTable(int iCanID)//������������ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_WavelengthTable_Cls);
}
int WINAPI LC_r_SetUVWavelengthTable(int iCanID)//����/�޸Ĳ�������� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_WavelengthTable_Add);
}
int WINAPI LC_r_DelUVWavelengthTable(int iCanID)//ɾ������������� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_WavelengthTable_Del);
}
int WINAPI LC_r_SetUVWavelengthZero(int iCanID)//���ĵ������� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_WavelengthAdjust_Zero);
}
int WINAPI LC_r_AdjUVWavelength(int iCanID)//У׼���� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_WavelengthAdjust_Adj);
}
int WINAPI LC_r_SetUVDeuteriumLampTime(int iCanID)//����뮵�ʱ�� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_DeuteriumLamp);
}
int WINAPI LC_r_RunUVWavelengthScan(int iCanID)//ִ�в���ɨ�� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_Run_WavelengthScan);
}
int WINAPI LC_r_RunUVDeuteriumLamp(int iCanID)//ִ��뮵ƿ��� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_Run_DeuteriumLamp);
}
int WINAPI LC_r_RunUVWavelengthTable(int iCanID)//ִ�г��򲨳� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_Run_WavelengthTable);
}
int WINAPI LC_r_RunUVWavelengthZero(int iCanID)//ִ�е��� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_Run_Zero);
}
int WINAPI LC_r_RunUVWavelengthSelfCheck(int iCanID)//�����Լ� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_WavelengthAdjust_Chk);
}
int WINAPI LC_r_SetUVWavelengthAdjust(int iCanID)//�趨����У��ֵ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_UV_WavelengthAdjust);
}
int WINAPI LC_r_GetUVWavelengthAdjust(int iCanID, short &sAdjustValue)//��ѯ����У��ֵ -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	WORD wValue;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_UV_WavelengthAdjust)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			//����У��ֵ��Ҫ���⴦�� -
			wValue = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
			sAdjustValue = (short)(wValue & 0x00FF);
			if (wValue >> 8) sAdjustValue = -sAdjustValue;
		}
	}
	return iRet;
}
//���첽����(����)
int WINAPI LC_r_GetPumpConstantSpeed(int iCanID, WORD &wConstantSpeed)//��ѯ�����ٶ� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Pump_Speed)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wConstantSpeed = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
int WINAPI LC_r_GetPumpGland(int iCanID, DWORD &dwLeftGland, DWORD &dwRightGland)//��ѯ�ܷ�Ȧ -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Pump_Gland)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			dwLeftGland = MSB2LSB32(&dbLCData[LC_PARAM_OFS]);
			dwRightGland = MSB2LSB32(&dbLCData[LC_PARAM_OFS + 4]);
		}
	}
	return iRet;
}
int WINAPI LC_r_SetPumpGlandZero(int iCanID)//�ܷ�Ȧ�������� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Gland);
}
int WINAPI LC_r_GetPumpPressure(int iCanID, WORD &wPressure, BYTE &dbUnit, DWORD &dwPressureRange, WORD &wVelocity, BYTE &dbSysStatus, BYTE &dbWorkStatus)//��ѯ��״̬ -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Pump_Status)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wPressure = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
			dbUnit = dbLCData[LC_PARAM_OFS + 2];
			dwPressureRange = MSB2LSB32(&dbLCData[LC_PARAM_OFS + 3]);
			wVelocity = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 7]);
			dbSysStatus = dbLCData[LC_PARAM_OFS + 9];
			dbWorkStatus = dbLCData[LC_PARAM_OFS + 10];
		}
	}
	return iRet;
}
int WINAPI LC_r_SetPumpConstantSpeed(int iCanID)//���ú����ٶ� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Speed);
}
int WINAPI LC_r_SetPumpPressure(int iCanID)//����ѹ��ֵ��Χ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Pressure);
}
int WINAPI LC_r_SetPumpWashParam(int iCanID)//����ϴ�Ѳ��� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Wash);
}
int WINAPI LC_r_GetPumpWashParam(int iCanID, WORD &wVelocity, WORD &wTime)//��ѯϴ�Ѳ��� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Pump_Wash)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wVelocity = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
			wTime = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 2]);
		}
	}
	return iRet;
}
int WINAPI LC_r_SetPumpPressureZero(int iCanID)//����ѹ����ʾ��λ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_PressureZero);
}
int WINAPI LC_r_SetPumpRippleCompress(int iCanID)//��������ѹ�� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Ripple);
}
int WINAPI LC_r_ClsPumpGradsTable(int iCanID)//���ѹ���ݶȱ� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_GradsTable_Cls);
}
int WINAPI LC_r_SetPumpGradsTable(int iCanID)//����/�޸�ѹ���ݶȱ� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_GradsTable_Add);
}
int WINAPI LC_r_DelPumpGradsTable(int iCanID)//ɾ��ѹ���ݶȱ��� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_GradsTable_Del);
}
int WINAPI LC_r_GetPumpGradsTable(int iCanID, BYTE &dbTotalLines, BYTE &dbNowLine, P_GRADS_TABLE pgt)//��ѯѹ���ݶȱ� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Pump_GradsTable_Get)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			dbTotalLines = dbLCData[LC_PARAM_OFS];
			if (dbNowLine = dbLCData[LC_PARAM_OFS + 1]) {
				pgt->dwTime = MSB2LSB32(&dbLCData[LC_PARAM_OFS + 2]);
				pgt->wScaleA = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 6]);
				pgt->wScaleB = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 8]);
				pgt->wScaleC = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 10]);
				pgt->wScaleD = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 12]);
				pgt->wVelocity = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 14]);
			}
		}
	}
	return iRet;
}
int WINAPI LC_r_GetPumpPressureZero(int iCanID, WORD &wPressureZero)//��ѯ��ѹ����λֵ -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Pump_PressureZero)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wPressureZero = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
int WINAPI LC_r_RunPumpCmd(int iCanID)//��ִ������ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_RunCmd);
}
int WINAPI LC_r_RunPumpSwitch(int iCanID)//ִ�бÿ��� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Switch_Run);
}
int WINAPI LC_r_RunPumpWash(int iCanID)//ִ�бó�ϴ -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Switch_Wash);
}
int WINAPI LC_r_RunPumpGradsReady(int iCanID)//ִ���ݶ�׼�� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Switch_GradsReady);
}
int WINAPI LC_r_RunPumpGrads(int iCanID)//ִ���ݶ� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Pump_Switch_Grads);
}
int WINAPI LC_r_GetPumpTime(int iCanID, DWORD &dwPumpTime)//��ѯ������ʱ�� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Pump_Time)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			dwPumpTime = MSB2LSB32(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
//ɫ���첽����(����)
int WINAPI LC_r_RunGCSample(int iCanID)//��ʼ���� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_All_RealtimeData);
}
int WINAPI LC_r_GetGCRealtimeData(int iCanID, P_SAMPLE_RESULT ptSample, int &iNum, BOOL &bStartFlag)//ʵʱ���� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int i,j,iRet;
	DWORD dwThisTime,dwLastTime;
	LPBYTE p;
	union {
		LONG lValue;
		float fValue;
	} tValue;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_All_RealtimeData)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			iNum = (int)(dbLCData[LC_LENGTH_OFS] - 5) / sizeof(SAMPLE_RESULT);
			bStartFlag = (dbLCData[LC_ADDR_OFS] & 0x80) ? TRUE : FALSE;
			for (p = &dbLCData[LC_PARAM_OFS], j = 0, i = 0; i < iNum; i++, p += sizeof(SAMPLE_RESULT)) {
				dwThisTime = MSB2LSB32(p);
				tValue.lValue = (LONG)MSB2LSB32(&p[4]);//��Э�飺����ֵ��FLOAT����!!!
				ptSample[j].dwTime = dwThisTime;
				ptSample[j].fValue = tValue.fValue;
				j++;
				dwLastTime = dwThisTime;
			}
			iNum = j;
		}
	}
	return iRet;
}
//�¿��첽����(����)
int WINAPI LC_r_GetTempBPS(int iCanID, WORD &wBPS)//��ѯ�¿�ͨѶ���� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Temp_BPS)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wBPS = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
int WINAPI LC_r_SetTempBPS(int iCanID)//�趨�¿�ͨѶ���� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Temp_BPS);
}
int WINAPI LC_r_GetTempTemp(int iCanID, BYTE &dbAddr, WORD &wTrueTemp, WORD &wTemp)//��ѯ�趨�¶� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_Temp_Temp)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			dbAddr = dbLCData[LC_PARAM_OFS];
			wTrueTemp = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 1]);
			wTemp = MSB2LSB16(&dbLCData[LC_PARAM_OFS + 3]);
		}
	}
	return iRet;
}
int WINAPI LC_r_SetTempTemp(int iCanID)//�趨�¶� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_Temp_Temp);
}
//����Դ�첽����(����)
int WINAPI LC_r_GetCCSVol(int iCanID, WORD &wVol)//��ѯ����Դ���� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_CCS_Vol)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wVol = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
int WINAPI LC_r_GetCCSTemp(int iCanID, WORD &wTemp)//��ѯ�絼���¶� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_CCS_Temp)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			wTemp = MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		}
	}
	return iRet;
}
int WINAPI LC_r_GetCCSTSw(int iCanID, BYTE &eVal)//��ѯ�絼���¶ȿ��� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_CCS_TSw)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			eVal = dbLCData[LC_PARAM_OFS];
		}
	}
	return iRet;
}
int WINAPI LC_r_SetCCSTSw(int iCanID)//���õ絼���¶ȿ��� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_CCS_TSw);
}
int WINAPI LC_r_GetCCSRange(int iCanID, BYTE &eVal)//��ѯ���̿��� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_CCS_Range)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			eVal = dbLCData[LC_PARAM_OFS];
		}
	}
	return iRet;
}
int WINAPI LC_r_SetCCSRange(int iCanID)//�������̿��� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_CCS_Range);
}
int WINAPI LC_r_GetCCSDASw(int iCanID, BYTE &bOnFlag)//��ѯ�������� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_CCS_DASw)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			bOnFlag = dbLCData[LC_PARAM_OFS];
		}
	}
	return iRet;
}
int WINAPI LC_r_RunCCSDASw(int iCanID)//���ú������� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_CCS_DASw);
}
int WINAPI LC_r_GetCCSDAVal(int iCanID, BYTE &dbVal, BYTE &dbNowVal)//��ѯ�������� -
{
	BYTE dbLCData[MAX_LC_BUFFER_SIZE];
	int iRet;
	if ((iRet = _lc_can_recvSpecial(dbLCData, iCanID, lcT_query, lcsc_CCS_DAVal)) == 0) {
		if (LC_RETURN_ERRCODE(dbLCData)) iRet = (int)MSB2LSB16(&dbLCData[LC_PARAM_OFS]);
		else {
			dbVal = dbLCData[LC_PARAM_OFS];
			dbNowVal = dbLCData[LC_PARAM_OFS + 1];
		}
	}
	return iRet;
}
int WINAPI LC_r_SetCCSDAVal(int iCanID)//���ú������� -
{
	return _LC_r_Set_(iCanID, lcT_set, lcsc_CCS_DAVal);
}

////////////////////////////////////////
//ͬ��������
//�豸ͬ������ -
int WINAPI LC_GetCurrentTime(int iCanID, DWORD &dwTime)//��ѯ��ǰʱ�� -
{
	int iRet;
	if ((iRet = LC_p_GetCurrentTime(iCanID)) == 0)
		iRet = LC_r_GetCurrentTime(iCanID, dwTime);
	return iRet;
}
int WINAPI LC_SetCurrentTime(int iCanID, DWORD dwTime)//���õ�ǰʱ�� -
{
	int iRet;
	if ((iRet = LC_p_SetCurrentTime(iCanID, dwTime)) == 0)
		iRet = LC_r_SetCurrentTime(iCanID);
	return iRet;
}
int WINAPI LC_SetCanID(int iCanID, int iNewCanID)//�趨�豸CAN��ַ -
{
	int iRet;
	if ((iRet = LC_p_SetCanID(iCanID, iNewCanID)) == 0)
		iRet = LC_r_SetCanID(iCanID);
	return iRet;
}
int WINAPI LC_GetCanInfo(int iCanID, P_CAN_INFO ptCanInfo, int &iNum)//ѯ��CAN�ڵ����� -
{
	int i,j,iTotal,iRet,iCanIDs[32];
	if ((iRet = LC_p_GetCanInfo(iCanID)) == 0) {
		iTotal = _lc_can_listResponseID(iCanID, iCanIDs);
		for (j = i = 0; i < iTotal; i++) {
			if (LC_r_GetCanInfo(iCanIDs[i], &ptCanInfo[j]) == 0) {
				j++;
			}
		}
		iNum = j;
	}
	return iRet;
}
int WINAPI LC_RunWake(int iCanID, BOOL bOnFlag)//ִ������ -
{
	int iRet;
	if ((iRet = LC_p_RunWake(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunWake(iCanID);
	return iRet;
}
int WINAPI LC_GetWarning(int iCanID, BYTE dbType, BYTE dbIdx, P_WARNING_INFO ptWarningInfo, int &iNum)//��ѯ���� -
{
	int iRet;
	if ((iRet = LC_p_GetWarning(iCanID, dbType, dbIdx)) == 0)
		iRet = LC_r_GetWarning(iCanID, ptWarningInfo, iNum);
	return iRet;
}
int WINAPI LC_SetWarning(int iCanID, BYTE dbType, BYTE dbIdx, WORD wIdx)//������λ -
{
	int iRet;
	if ((iRet = LC_p_SetWarning(iCanID, dbType, dbIdx, wIdx)) == 0)
		iRet = LC_r_SetWarning(iCanID);
	return iRet;
}
int WINAPI LC_GetRealtimeData(int iCanID, P_SAMPLE_RESULT ptSample, int &iNum, LCReturnCode &eStatus, DWORD &dwTime, DWORD &dwWaveLength)//ʵʱ���� -
{
	int iRet;
	if ((iRet = LC_p_GetRealtimeData(iCanID)) == 0)
		iRet = LC_r_GetRealtimeData(iCanID, ptSample, iNum, eStatus, dwTime, dwWaveLength);
	return iRet;
}
int WINAPI LC_SetLogicIndex(int iCanID, WORD wLogicIndex)//�����豸���߼���� -
{
	int iRet;
	if ((iRet = LC_p_SetLogicIndex(iCanID, wLogicIndex)) == 0)
		iRet = LC_r_SetLogicIndex(iCanID);
	return iRet;
}
//����ͬ������ -
int WINAPI LC_GetUVStatus(int iCanID, BYTE &dbSysStatus, BYTE &dbWorkStatus)//��ѯ����״̬ -
{
	int iRet;
	if ((iRet = LC_p_GetUVStatus(iCanID)) == 0)
		iRet = LC_r_GetUVStatus(iCanID, dbSysStatus, dbWorkStatus);
	return iRet;
}
int WINAPI LC_GetUVWorkData(int iCanID, P_UV_WORK_DATA ptUVWorkData, int &iNum, BOOL bMultiData)//��ѯ���⹤������ -
{
	int iRet;
	if ((iRet = LC_p_GetUVWorkData(iCanID, bMultiData)) == 0)
		iRet = LC_r_GetUVWorkData(iCanID, ptUVWorkData, iNum, bMultiData);
	return iRet;
}
int WINAPI LC_GetUVDeuteriumLampTime(int iCanID, WORD &wDeuteriumTime)//��ѯ뮵��ۼ�����ʱ -
{
	int iRet;
	if ((iRet = LC_p_GetUVDeuteriumLampTime(iCanID)) == 0)
		iRet = LC_r_GetUVDeuteriumLampTime(iCanID, wDeuteriumTime);
	return iRet;
}
int WINAPI LC_GetUVWavelength(int iCanID, WORD &wWavelength)//��ѯ�趨���� -
{
	int iRet;
	if ((iRet = LC_p_GetUVWavelength(iCanID)) == 0)
		iRet = LC_r_GetUVWavelength(iCanID, wWavelength);
	return iRet;
}
int WINAPI LC_GetUVWavelengthTable(int iCanID, BYTE dbLine, BYTE &dbTotalLines, BYTE &dbNowLine, P_WAVELENGTH_TABLE pwt)//��ѯ���������
{
	int iRet;
	if ((iRet = LC_p_GetUVWavelengthTable(iCanID, dbLine)) == 0)
		iRet = LC_r_GetUVWavelengthTable(iCanID, dbTotalLines, dbNowLine, pwt);
	return iRet;
}
int WINAPI LC_GetUVWavelengthScanRange(int iCanID, WORD &wStartWavelength, WORD &wEndWavelength)//��ѯ����ɨ�跶Χ -
{
	int iRet;
	if ((iRet = LC_p_GetUVWavelengthScanRange(iCanID)) == 0)
		iRet = LC_r_GetUVWavelengthScanRange(iCanID, wStartWavelength, wEndWavelength);
	return iRet;
}
int WINAPI LC_SetUVWavelengthSmooth(int iCanID, WORD wWavelength, BYTE dbSmoothValue)//�趨������ƽ��ָ�� -
{
	int iRet;
	if ((iRet = LC_p_SetUVWavelengthSmooth(iCanID, wWavelength, dbSmoothValue)) == 0)
		iRet = LC_r_SetUVWavelengthSmooth(iCanID);
	return iRet;
}
int WINAPI LC_SetUVWavelengthScan(int iCanID, BYTE dbWavelengthScan, WORD wStartWavelength, WORD wEndWavelength)//�趨(ִ��)����ɨ�� -
{
	int iRet;
	if ((iRet = LC_p_SetUVWavelengthScan(iCanID, dbWavelengthScan, wStartWavelength, wEndWavelength)) == 0)
		iRet = LC_r_SetUVWavelengthScan(iCanID);
	return iRet;
}
int WINAPI LC_ClsUVWavelengthTable(int iCanID)//������������ -
{
	int iRet;
	if ((iRet = LC_p_ClsUVWavelengthTable(iCanID)) == 0)
		iRet = LC_r_ClsUVWavelengthTable(iCanID);
	return iRet;
}
int WINAPI LC_SetUVWavelengthTable(int iCanID, P_WAVELENGTH_TABLE pwt)//����/�޸Ĳ�������� -
{
	int iRet;
	if ((iRet = LC_p_SetUVWavelengthTable(iCanID, pwt)) == 0)
		iRet = LC_r_SetUVWavelengthTable(iCanID);
	return iRet;
}
int WINAPI LC_DelUVWavelengthTable(int iCanID, DWORD dwTime)//ɾ������������� -
{
	int iRet;
	if ((iRet = LC_p_DelUVWavelengthTable(iCanID, dwTime)) == 0)
		iRet = LC_r_DelUVWavelengthTable(iCanID);
	return iRet;
}
int WINAPI LC_SetUVWavelengthZero(int iCanID, BOOL bZeroFlag)//���ĵ������� -
{
	int iRet;
	if ((iRet = LC_p_SetUVWavelengthZero(iCanID, bZeroFlag)) == 0)
		iRet = LC_r_SetUVWavelengthZero(iCanID);
	return iRet;
}
int WINAPI LC_AdjUVWavelength(int iCanID, WORD wParam)//У׼���� -
{
	int iRet;
	if ((iRet = LC_p_AdjUVWavelength(iCanID, wParam)) == 0)
		iRet = LC_r_AdjUVWavelength(iCanID);
	return iRet;
}
int WINAPI LC_SetUVDeuteriumLampTime(int iCanID)//����뮵�ʱ�� -
{
	int iRet;
	if ((iRet = LC_p_SetUVDeuteriumLampTime(iCanID)) == 0)
		iRet = LC_r_SetUVDeuteriumLampTime(iCanID);
	return iRet;
}
int WINAPI LC_RunUVWavelengthScan(int iCanID, BOOL bOnFlag)//ִ�в���ɨ�� -
{
	int iRet;
	if ((iRet = LC_p_RunUVWavelengthScan(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunUVWavelengthScan(iCanID);
	return iRet;
}
int WINAPI LC_RunUVDeuteriumLamp(int iCanID, BOOL bOnFlag)//ִ��뮵ƿ��� -
{
	int iRet;
	if ((iRet = LC_p_RunUVDeuteriumLamp(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunUVDeuteriumLamp(iCanID);
	return iRet;
}
int WINAPI LC_RunUVWavelengthTable(int iCanID, BOOL bOnFlag)//ִ�г��򲨳� -
{
	int iRet;
	if ((iRet = LC_p_RunUVWavelengthTable(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunUVWavelengthTable(iCanID);
	return iRet;
}
int WINAPI LC_RunUVWavelengthZero(int iCanID)//ִ�е��� -
{
	int iRet;
	if ((iRet = LC_p_RunUVWavelengthZero(iCanID)) == 0)
		iRet = LC_r_RunUVWavelengthZero(iCanID);
	return iRet;
}
int WINAPI LC_RunUVWavelengthSelfCheck(int iCanID)//�����Լ� -
{
	int iRet;
	if ((iRet = LC_p_RunUVWavelengthSelfCheck(iCanID)) == 0)
		iRet = LC_r_RunUVWavelengthSelfCheck(iCanID);
	return iRet;
}
int WINAPI LC_SetUVWavelengthAdjust(int iCanID, short sAdjustValue)//�趨����У��ֵ -
{
	int iRet;
	if ((iRet = LC_p_SetUVWavelengthAdjust(iCanID, sAdjustValue)) == 0)
		iRet = LC_r_SetUVWavelengthAdjust(iCanID);
	return iRet;
}
int WINAPI LC_GetUVWavelengthAdjust(int iCanID, short &sAdjustValue)//��ѯ����У��ֵ -
{
	int iRet;
	if ((iRet = LC_p_GetUVWavelengthAdjust(iCanID)) == 0)
		iRet = LC_r_GetUVWavelengthAdjust(iCanID, sAdjustValue);
	return iRet;
}
//��ͬ������ -
int WINAPI LC_GetPumpConstantSpeed(int iCanID, WORD &wConstantSpeed)//��ѯ�����ٶ� -
{
	int iRet;
	if ((iRet = LC_p_GetPumpConstantSpeed(iCanID)) == 0)
		iRet = LC_r_GetPumpConstantSpeed(iCanID, wConstantSpeed);
	return iRet;
}
int WINAPI LC_GetPumpGland(int iCanID, DWORD &dwLeftGland, DWORD &dwRightGland)//��ѯ�ܷ�Ȧ -
{
	int iRet;
	if ((iRet = LC_p_GetPumpGland(iCanID)) == 0)
		iRet = LC_r_GetPumpGland(iCanID, dwLeftGland, dwRightGland);
	return iRet;
}
int WINAPI LC_SetPumpGlandZero(int iCanID)//�ܷ�Ȧ�������� -
{
	int iRet;
	if ((iRet = LC_p_SetPumpGlandZero(iCanID)) == 0)
		iRet = LC_r_SetPumpGlandZero(iCanID);
	return iRet;
}
int WINAPI LC_GetPumpPressure(int iCanID, WORD &wPressure, BYTE &dbUnit, DWORD &dwPressureRange, WORD &wVelocity, BYTE &dbSysStatus, BYTE &dbWorkStatus)//��ѯ��״̬ -
{
	int iRet;
	if ((iRet = LC_p_GetPumpPressure(iCanID)) == 0)
		iRet = LC_r_GetPumpPressure(iCanID, wPressure, dbUnit, dwPressureRange, wVelocity, dbSysStatus, dbWorkStatus);
	return iRet;
}
int WINAPI LC_SetPumpConstantSpeed(int iCanID, WORD wConstantSpeed)//���ú����ٶ� -
{
	int iRet;
	if ((iRet = LC_p_SetPumpConstantSpeed(iCanID, wConstantSpeed)) == 0)
		iRet = LC_r_SetPumpConstantSpeed(iCanID);
	return iRet;
}
int WINAPI LC_SetPumpPressure(int iCanID, WORD wPressureHigh, WORD wPressureLow, BYTE dbUnit)//����ѹ��ֵ��Χ -
{
	int iRet;
	if ((iRet = LC_p_SetPumpPressure(iCanID, wPressureHigh, wPressureLow, dbUnit)) == 0)
		iRet = LC_r_SetPumpPressure(iCanID);
	return iRet;
}
int WINAPI LC_SetPumpWashParam(int iCanID, WORD wVelocity, WORD wTime)//����ϴ�Ѳ��� -
{
	int iRet;
	if ((iRet = LC_p_SetPumpWashParam(iCanID, wVelocity, wTime)) == 0)
		iRet = LC_r_SetPumpWashParam(iCanID);
	return iRet;
}
int WINAPI LC_GetPumpWashParam(int iCanID, WORD &wVelocity, WORD &wTime)//��ѯϴ�Ѳ��� -
{
	int iRet;
	if ((iRet = LC_p_GetPumpWashParam(iCanID)) == 0)
		iRet = LC_r_GetPumpWashParam(iCanID, wVelocity, wTime);
	return iRet;
}
int WINAPI LC_SetPumpPressureZero(int iCanID, WORD wPressureZero)//����ѹ����ʾ��λ -
{
	int iRet;
	if ((iRet = LC_p_SetPumpPressureZero(iCanID, wPressureZero)) == 0)
		iRet = LC_r_SetPumpPressureZero(iCanID);
	return iRet;
}
int WINAPI LC_SetPumpRippleCompress(int iCanID, WORD wRippleCompress)//��������ѹ�� -
{
	int iRet;
	if ((iRet = LC_p_SetPumpRippleCompress(iCanID, wRippleCompress)) == 0)
		iRet = LC_r_SetPumpRippleCompress(iCanID);
	return iRet;
}
int WINAPI LC_ClsPumpGradsTable(int iCanID)//���ѹ���ݶȱ� -
{
	int iRet;
	if ((iRet = LC_p_ClsPumpGradsTable(iCanID)) == 0)
		iRet = LC_r_ClsPumpGradsTable(iCanID);
	return iRet;
}
int WINAPI LC_SetPumpGradsTable(int iCanID, P_GRADS_TABLE pgt)//����/�޸�ѹ���ݶȱ� -
{
	int iRet;
	if ((iRet = LC_p_SetPumpGradsTable(iCanID, pgt)) == 0)
		iRet = LC_r_SetPumpGradsTable(iCanID);
	return iRet;
}
int WINAPI LC_DelPumpGradsTable(int iCanID, DWORD dwTime)//ɾ��ѹ���ݶȱ��� -
{
	int iRet;
	if ((iRet = LC_p_DelPumpGradsTable(iCanID, dwTime)) == 0)
		iRet = LC_r_DelPumpGradsTable(iCanID);
	return iRet;
}
int WINAPI LC_GetPumpGradsTable(int iCanID, BYTE dbLine, BYTE &dbTotalLines, BYTE &dbNowLine, P_GRADS_TABLE pgt)//��ѯѹ���ݶȱ� -
{
	int iRet;
	if ((iRet = LC_p_GetPumpGradsTable(iCanID, dbLine)) == 0)
		iRet = LC_r_GetPumpGradsTable(iCanID, dbTotalLines, dbNowLine, pgt);
	return iRet;
}
int WINAPI LC_GetPumpPressureZero(int iCanID, WORD &wPressureZero)//��ѯ��ѹ����λֵ -
{
	int iRet;
	if ((iRet = LC_p_GetPumpPressureZero(iCanID)) == 0)
		iRet = LC_r_GetPumpPressureZero(iCanID, wPressureZero);
	return iRet;
}
int WINAPI LC_RunPumpCmd(int iCanID, enum LCPumpCmd eCmd)//��ִ������ -
{
	int iRet;
	if ((iRet = LC_p_RunPumpCmd(iCanID, eCmd)) == 0)
		iRet = LC_r_RunPumpCmd(iCanID);
	return iRet;
}
int WINAPI LC_RunPumpSwitch(int iCanID, BOOL bOnFlag)//ִ�бÿ��� -
{
	int iRet;
	if ((iRet = LC_p_RunPumpSwitch(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunPumpSwitch(iCanID);
	return iRet;
}
int WINAPI LC_RunPumpWash(int iCanID, BOOL bOnFlag)//ִ�бó�ϴ -
{
	int iRet;
	if ((iRet = LC_p_RunPumpWash(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunPumpWash(iCanID);
	return iRet;
}
int WINAPI LC_RunPumpGradsReady(int iCanID, BOOL bOnFlag)//ִ���ݶ�׼�� -
{
	int iRet;
	if ((iRet = LC_p_RunPumpGradsReady(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunPumpGradsReady(iCanID);
	return iRet;
}
int WINAPI LC_RunPumpGrads(int iCanID, BOOL bOnFlag)//ִ���ݶ� -
{
	int iRet;
	if ((iRet = LC_p_RunPumpGrads(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunPumpGrads(iCanID);
	return iRet;
}
int WINAPI LC_GetPumpTime(int iCanID, DWORD &dwPumpTime)//��ѯ��ѹ����λֵ -
{
	int iRet;
	if ((iRet = LC_p_GetPumpTime(iCanID)) == 0)
		iRet = LC_r_GetPumpTime(iCanID, dwPumpTime);
	return iRet;
}
//ɫ��ͬ������ -
int WINAPI LC_RunGCSample(int iCanID, BYTE dbChannel)//��ʼ���� -
{
	int iRet;
	if ((iRet = LC_p_RunGCSample(iCanID, dbChannel)) == 0)
		iRet = LC_r_RunGCSample(iCanID);
	return iRet;
}
int WINAPI LC_GetGCRealtimeData(int iCanID, BYTE dbChannel, P_SAMPLE_RESULT ptSample, int &iNum, BOOL &bStartFlag)//ʵʱ���� -
{
	int iRet;
	if ((iRet = LC_p_GetGCRealtimeData(iCanID, dbChannel)) == 0)
		iRet = LC_r_GetGCRealtimeData(iCanID, ptSample, iNum, bStartFlag);
	return iRet;
}
void WINAPI LC_TranslateGCRealtimeData(P_SAMPLE_RESULT ptDst, P_SAMPLE_RESULT ptSrc, int iNum, BYTE eVal)//��������ת����� -
{
	int i;
	float fMul,fMax;
	switch(eVal) {
	case RANGE_uS:
		fMul = 1000.0f;
		fMax = 1024.0f;
		break;
	case RANGE_mS:
		fMul = 10000.0f;
		fMax = 4000.0f;
		break;
	default:
		return;
	}
	for (i = 0; i < iNum; i++) {
		if ((ptDst[i].fValue = ptSrc[i].fValue * fMul) > fMax) ptDst[i].fValue = fMax;
	}
}
//�¿�ͬ������ -
int WINAPI LC_GetTempBPS(int iCanID, WORD &wBPS)//��ѯ�¿�ͨѶ���� -
{
	int iRet;
	if ((iRet = LC_p_GetTempBPS(iCanID)) == 0)
		iRet = LC_r_GetTempBPS(iCanID, wBPS);
	return iRet;
}
int WINAPI LC_SetTempBPS(int iCanID, WORD wBPS)//�趨�¿�ͨѶ���� -
{
	int iRet;
	if ((iRet = LC_p_SetTempBPS(iCanID, wBPS)) == 0)
		iRet = LC_r_SetTempBPS(iCanID);
	return iRet;
}
int WINAPI LC_GetTempTemp(int iCanID, BYTE dbAddr, WORD &wTrueTemp, WORD &wTemp)//��ѯ�趨�¶� -
{
	int iRet;
	if ((iRet = LC_p_GetTempTemp(iCanID, dbAddr)) == 0)
		iRet = LC_r_GetTempTemp(iCanID, dbAddr, wTrueTemp, wTemp);
	return iRet;
}
int WINAPI LC_SetTempTemp(int iCanID, BYTE dbAddr, WORD wTemp)//�趨�¶� -
{
	int iRet;
	if ((iRet = LC_p_SetTempTemp(iCanID, dbAddr, wTemp)) == 0)
		iRet = LC_r_SetTempTemp(iCanID);
	return iRet;
}
//����Դͬ������ -
int WINAPI LC_GetCCSVol(int iCanID, WORD &wVol)//��ѯ����Դ���� -
{
	int iRet;
	if ((iRet = LC_p_GetCCSVol(iCanID)) == 0)
		iRet = LC_r_GetCCSVol(iCanID, wVol);
	return iRet;
}
int WINAPI LC_GetCCSTemp(int iCanID, WORD &wTemp)//��ѯ�絼���¶� -
{
	int iRet;
	if ((iRet = LC_p_GetCCSTemp(iCanID)) == 0)
		iRet = LC_r_GetCCSTemp(iCanID, wTemp);
	return iRet;
}
int WINAPI LC_GetCCSTSw(int iCanID, BYTE &eVal)//��ѯ�絼���¶ȿ��� -
{
	int iRet;
	if ((iRet = LC_p_GetCCSTSw(iCanID)) == 0)
		iRet = LC_r_GetCCSTSw(iCanID, eVal);
	return iRet;
}
int WINAPI LC_SetCCSTSw(int iCanID, BYTE eVal)//���õ絼���¶ȿ��� -
{
	int iRet;
	if ((iRet = LC_p_SetCCSTSw(iCanID, eVal)) == 0)
		iRet = LC_r_SetCCSTSw(iCanID);
	return iRet;
}
int WINAPI LC_GetCCSRange(int iCanID, BYTE &eVal)//��ѯ���̿��� -
{
	int iRet;
	if ((iRet = LC_p_GetCCSRange(iCanID)) == 0)
		iRet = LC_r_GetCCSRange(iCanID, eVal);
	return iRet;
}
int WINAPI LC_SetCCSRange(int iCanID, BYTE eVal)//�������̿��� -
{
	int iRet;
	if ((iRet = LC_p_SetCCSRange(iCanID, eVal)) == 0)
		iRet = LC_r_SetCCSRange(iCanID);
	return iRet;
}
int WINAPI LC_GetCCSDASw(int iCanID, BYTE &bOnFlag)//��ѯ�������� -
{
	int iRet;
	if ((iRet = LC_p_GetCCSDASw(iCanID)) == 0)
		iRet = LC_r_GetCCSDASw(iCanID, bOnFlag);
	return iRet;
}
int WINAPI LC_RunCCSDASw(int iCanID, BYTE bOnFlag)//���ú������� -
{
	int iRet;
	if ((iRet = LC_p_RunCCSDASw(iCanID, bOnFlag)) == 0)
		iRet = LC_r_RunCCSDASw(iCanID);
	return iRet;
}
int WINAPI LC_GetCCSDAVal(int iCanID, BYTE &dbVal, BYTE &dbNowVal)//��ѯ�������� -
{
	int iRet;
	if ((iRet = LC_p_GetCCSDAVal(iCanID)) == 0)
		iRet = LC_r_GetCCSDAVal(iCanID, dbVal, dbNowVal);
	return iRet;
}
int WINAPI LC_SetCCSDAVal(int iCanID, BYTE dbVal)//���ú������� -
{
	int iRet;
	if ((iRet = LC_p_SetCCSDAVal(iCanID, dbVal)) == 0)
		iRet = LC_r_SetCCSDAVal(iCanID);
	return iRet;
}

//�����Ӧ������ -
int WINAPI LC_ResponseCanInfo(int iCanID)
{
	BYTE dbLCCmd[MAX_LC_BUFFER_SIZE];
	dbLCCmd[LC_PARAM_OFS] = 1;
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS + 1], PC_CAN_ID);
	LSB2MSB32(&dbLCCmd[LC_PARAM_OFS + 4], 1);
	_make_LCCmd(dbLCCmd, lcT_broadcast, lcsc_All_Can, 9);
	return _lc_can_send(iCanID, dbLCCmd);
}

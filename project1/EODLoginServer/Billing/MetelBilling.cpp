#include "../stdafx.h"
#ifdef _METEL_LOGIN_200608_
#include <list>
#include <set>
#include <map>
#include "../Billing/GameAPI.h"
#include "../Odbc/ODBC_.h"

// PC방 인증
COdbc*	g_pDummyAccount = NULL;
std::set<DWORD/*PCBang IP LIst*/>		g_setPCBangIp;
CRITICAL_SECTION						g_csPCBangIp;

BOOL GetPCBangIpList(COdbc* pDB, std::set<DWORD>* pIpSet) {
	struct stResult {
		TCHAR	pszIp[IPV4_BUF_SIZE];
	};

	TCHAR	pszQuery[256] = {0};
	int		iBindColCnt = 0;
	stResult	res;
	DWORD	dwIp = 0;
	BOOL	bRet = FALSE;

	_stprintf(pszQuery, _T("select szIp from NineDragons.gamedbo.tblPCBangIp"));
	Logout(_T("%s"), pszQuery);
	if(FALSE == pDB->Exec(pszQuery)) {
		Logout(_T("#ERR %s(%d): pDB->Exec(%s) 실패"), _T(__FILE__), __LINE__, pszQuery);
		pDB->Clear();
		return FALSE;
	}

	while(TRUE) {
		if(0 < pDB->GetResultColCnt()) {
			switch(iBindColCnt++) {
			case 0:
				pDB->BindCol(1, SQL_C_CHAR, res.pszIp, IPV4_BUF_SIZE);
				while(pDB->Fetch()) {
					dwIp = inet_addr(res.pszIp);
					_tprintf(_T("PCBangIp: ip=%s \n"), res.pszIp);
					::PrintConsole("PCBangIp: ip=%s \n", res.pszIp);
					EnterCriticalSection(&g_csPCBangIp);
					g_setPCBangIp.insert(dwIp);
					LeaveCriticalSection(&g_csPCBangIp);
				}
				break;
			}
		}

		if(!pDB->MoreResult()) {
			pDB->Clear();
			break;
		}
	}
	return TRUE;
}

BOOL IsPCBangUser(DWORD dwIp) {
	std::set<DWORD>::iterator it;
	BOOL bRet = FALSE;

	EnterCriticalSection(&g_csPCBangIp);
	it = g_setPCBangIp.find(dwIp);
	if(it == g_setPCBangIp.end()) {
		bRet = FALSE;
	}
	else {
		bRet = TRUE;
	}
	LeaveCriticalSection(&g_csPCBangIp);

	return bRet;
}

// 빌링
extern bool		g_bContinue;	// 각종 쓰레드를 종료하는 변수

std::list<METEL_BILLING_LOGIN*>			g_listMetelBillingLogin;
CRITICAL_SECTION						g_csMetelBillingLogin;

std::map<HANDLE, METEL_BILLING_INFO*>	g_mapMetelBillingInfo;
CRITICAL_SECTION						g_csMetelBillingInfo;

HANDLE									g_hMetelLoginProc;
HANDLE									g_hMetelCallBackEvent;
METEL_BILLING_INFO						g_TempBillingInfo;

unsigned __stdcall MetelLoginProc(void* pVoid) {
	g_hMetelLoginProc = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hMetelCallBackEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	int		iRet = 0;
	enum { // iRet
		BILLING_RES__FAIL_IDENTIFY = -1,
		BILLING_RES__FAIL_LOGIN = -2,
		BILLING_RES__FAIL_TIMEOUT = -3,
	};

	DWORD	dwRet = 0;
	METEL_BILLING_LOGIN*	pBL = NULL;
	METEL_BILLING_INFO*		pBI = NULL;
	while(g_bContinue) {
		dwRet = WaitForSingleObject(g_hMetelLoginProc, 1000);
		if(dwRet == WAIT_TIMEOUT) continue;
		else if(dwRet != WAIT_OBJECT_0) continue;
		else if(dwRet == WAIT_FAILED) {
			::PrintConsole("(%d) Critical error occured!! \n", __LINE__);
			::PrintConsole("(%d) WAIT_FAILED = MetelLoginProc::WaitForSingleObject() \n", __LINE__);
			continue;
		}

		while(TRUE) {
			EnterCriticalSection(&g_csMetelBillingLogin);
			if(FALSE == g_listMetelBillingLogin.empty()) {
				pBL = g_listMetelBillingLogin.front();
				g_listMetelBillingLogin.pop_front();
			}
			else {
				LeaveCriticalSection(&g_csMetelBillingLogin);
				break;
			}
			LeaveCriticalSection(&g_csMetelBillingLogin);
			
			pBI = new METEL_BILLING_INFO;
			memset(pBI, -1, sizeof(METEL_BILLING_INFO));
			
			ResetEvent( g_hMetelCallBackEvent );
			iRet = BillingLogin(pBL->pszId, pBL->pszPass, pBL->pszIp, -1);
			::PrintConsole("%d = BillingLogin(%s, %s, %s) \n", iRet, pBL->pszId, pBL->pszPass, pBL->pszIp);
			if(iRet < 0) {
				// fail to BillingLogin
				// but!! must wait for the results of the billing-callback until TIME_OUT time
				BillingLogout(pBL->pszId, iRet, -1);
				::PrintConsole("BillingLogout(%s, %d) \n", pBL->pszId, iRet);
			}
			
			dwRet = WaitForSingleObject(g_hMetelCallBackEvent, 8000);
			if(dwRet == WAIT_TIMEOUT) {
				::PrintConsole("(%d) TIMEOUT(g_hMetelCallBackEvent) id=%s \n", __LINE__, pBL->pszId);
				pBI->iCallbackResult = _LOGIN_ERROR_CODE_METEL_UNKNOWN;
				pBI->iLoginSN = iRet;
			}
			else {
				memcpy(pBI, &g_TempBillingInfo, sizeof(METEL_BILLING_INFO));
			}
			
			EnterCriticalSection(&g_csMetelBillingInfo);
			g_mapMetelBillingInfo[pBL->h] = pBI;
			LeaveCriticalSection(&g_csMetelBillingInfo);
			SetEvent(pBL->h);
			SAFE_DELETE(pBL);
		}
	}

	return 0;
}

void BillingIdentifyCallback(int iResult, char* szResultMsg, char* szUserID, int iBillingType, int iPoints, int iFreePoints) {
	::PrintConsole("BillingIdentifyCallback(iResult=%d) \n", iResult);
	if(iResult < 0) {
		::PrintConsole("BillingIdentifyCallbackMsg:%s \n", szResultMsg);
	}
	return;
}

void BillingLoginCallback(int iResult, char* szResultMsg, int iLoginSN, int iBillingType, int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime, TGATime* lpEndTime) {
	::PrintConsole("BillingLoginCallback(iResult=%d) \n", iResult);

	memset(&g_TempBillingInfo, -1, sizeof(METEL_BILLING_INFO));

	if(iResult < 0) {
		::PrintConsole("BillingLoginCallbackMsg:%s \n", szResultMsg);

		// set billing info
		g_TempBillingInfo.iLoginSN = iLoginSN;

		switch(iResult) {
		case -10000:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_E50;
			break;
		case -10010:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_E51;
			break;
		case -15000:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_E52;
			break;
		case -15010:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_E53;
			break;
		case -15020:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_E54;
			break;
		case -15030:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_E55;
			break;
		case -15040:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_E56;
			break;
		case -15050:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_E57;
			break;
		
		default:
			g_TempBillingInfo.iCallbackResult = _LOGIN_ERROR_CODE_METEL_UNKNOWN;
			break;
		}
	}
	else {
		// set billing info
		g_TempBillingInfo.iCallbackResult = iResult;

		g_TempBillingInfo.iLoginSN = iLoginSN;
		g_TempBillingInfo.iBillingType = iBillingType;
		g_TempBillingInfo.iUsedPoint = iUsedPoints;
		g_TempBillingInfo.iPoint = iPoints;
		g_TempBillingInfo.iUsedFreePoint = iUsedFreePoints;
		g_TempBillingInfo.iFreePoint = iFreePoints;
		memcpy(&g_TempBillingInfo.Time, lpTime, sizeof(TGATime));
		memcpy(&g_TempBillingInfo.EndTime, lpEndTime, sizeof(TGATime));
		::PrintConsole("iLoginSN=%d, iBillingType=%d, iUsedPoint=%d, iPoint=%d, iUsedFreePoint=%d, iFreePoint=%d \n",
			iLoginSN, iBillingType, iUsedPoints, iPoints, iUsedFreePoints, iFreePoints);
		::PrintConsole("lpTime: year=%d, month=%d, day=%d, hour=%d, min=%d, sec=%d \n",
			lpTime->iYear, lpTime->iMonth, lpTime->iDay, lpTime->iHour, lpTime->iMinute, lpTime->iSecond );
		::PrintConsole("lpEndTime: year=%d, month=%d, day=%d, hour=%d, min=%d, sec=%d \n",
			lpEndTime->iYear, lpEndTime->iMonth, lpEndTime->iDay, lpEndTime->iHour, lpEndTime->iMinute, lpEndTime->iSecond );
	}
	
	SetEvent(g_hMetelCallBackEvent);
}

void BillingLogoutCallback(int iResult, char* szResultMsg, int iLoginSN, int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime, TGATime* lpEndTime) {
	::PrintConsole("BillingLogoutCallback(iResult=%d) \n", iResult);
	if(iResult < 0) {
		::PrintConsole("BillingLogoutCallbackMsg:%s \n", szResultMsg);
	}
	return;
}

#endif //_METEL_LOGIN_200608_
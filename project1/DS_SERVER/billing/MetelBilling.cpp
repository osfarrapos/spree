#include "../stdafx.h"

#include "../MyLibrary/gDefine.h"

#ifdef _METEL_LOGIN_200608_
#include <list>
#include <map>
#include "../Billing/GameAPI.h"

void BillingIdentifyCallback(int iResult, char* szResultMsg, char* szUserID, int iBillingType, int iPoints, int iFreePoints) {
	Logout(_T("BillingIdentifyCallback(iResult=%d)"), iResult);
	if(iResult < 0) {
		::PrintConsole("%s \n", szResultMsg);
		Logout(_T("%s"), szResultMsg);
	}
	return;
}

void BillingLoginCallback(int iResult, char* szResultMsg, int iLoginSN, int iBillingType, int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime, TGATime* lpEndTime) {
	Logout(_T("BillingLoginCallback(iResult=%d)"), iResult);
	if(iResult < 0) {
		::PrintConsole("%s \n", szResultMsg);
		Logout(_T("%s"), szResultMsg);
	}
	return;
}

void BillingLogoutCallback(int iResult, char* szResultMsg, int iLoginSN, int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime, TGATime* lpEndTime) {
	Logout(_T("BillingLogoutCallback(iResult=%d)"), iResult);
	if(iResult < 0) {
		::PrintConsole("%s \n", szResultMsg);
		Logout(_T("%s"), szResultMsg);
	}
	return;
}

#endif //_METEL_LOGIN_200608_
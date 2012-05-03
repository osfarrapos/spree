//-----------------------------------------------------------------------------
// Game Server API of MGS v1.30
// Created 2002/3/28
//-----------------------------------------------------------------------------
#ifndef BILLING_GAMEAPI_H
#define BILLING_GAMEAPI_H
//-----------------------------------------------------------------------------
// Definitions of some constents
//-----------------------------------------------------------------------------
// IP Adddress & Port setting file for AP Servers
#define APLIST_FILE		"aplist.txt"
// Maximum LoginSN, LoginSN = 0..319999
#define GA_LOGINSN_MAX 320000
//-----------------------------------------------------------------------------
// Definitions of Callback functions
//-----------------------------------------------------------------------------
typedef struct {
	int iYear, iMonth, iDay, iHour, iMinute, iSecond;
}TGATime;
//void BillingIdentifyCallback(int iResult, char* szUserID, int iBillingType,
//	int iPoints, int iFreePoints);
typedef void (*PFN_IDENTIFYCALLBACK)(int, char*, char*, int, int, int);
//void BillingLoginCallback(int iResult, int iLoginSN, int iBillingType,
//	int iUsedPoints, int iPoints, int iUsedFreePoints, int iFreePoints, TGATime *lpTime);
typedef void (*PFN_LOGINCALLBACK)(int, char*, int, int, int, int, int, int, TGATime*, TGATime*);
//void BillingLogoutCallback(int iResult, int iLoginSN, int iUsedPoints, int iPoints,
//	int iUsedFreePoints, int iFreePoints, TGATime *lpTime);
typedef void (*PFN_LOGOUTCALLBACK)(int, char*, int, int, int, int, int, TGATime*, TGATime*);
//-----------------------------------------------------------------------------
// Functions of Game Server API
//-----------------------------------------------------------------------------
int InitBilling(int iPlayerMax,
	PFN_IDENTIFYCALLBACK IdentifyCallback,
	PFN_LOGINCALLBACK LoginCallback, PFN_LOGOUTCALLBACK LogoutCallback);
int BillingIdentify(char* szUserID, char* szPassword, char* szUserIPAddr);
int BillingLogin(char* szUserID, char* szPassword, char* szUserIPAddr, int iPlayerCount);
int BillingLogout(char* szUserID, int iLoginSN, int iPlayerCount);
int CloseBilling();
//-----------------------------------------------------------------------------
// Error Code
//-----------------------------------------------------------------------------
#define GA_ERR_NOAPLIST     -1      // File "aplist.txt" not exist
#define GA_ERR_BADAPLIST    -2      // Bad format in "aplist.txt"
#define GA_ERR_CONNECTFAIL  -3      // Fail to connect to MGS
#define GA_ERR_INITFAIL     -4      // InitBilling Fail
#define GA_ERR_NOINIT       -10     // Calling GameAPI before InitBiiling
#define GA_ERR_BADMSG       -20     // Received a bad message
#define GA_ERR_TIMEOUT      -21     // MGS Timeout
#define GA_ERR_SENDERR      -22     // Fail to send message to MGS
#define GA_ERR_TOOMANYUSER  -30     // Too many user login at the same time
//-----------------------------------------------------------------------------
#endif
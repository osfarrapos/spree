#include "../stdafx.h"
#ifdef _METEL_LOGIN_200608_
#include "../Billing/BillingTimer.h"

extern bool g_Threading;

unsigned __stdcall CBillingTimer::BillingTimerProc(void* pVoid) {
	CBillingTimer* pThis = (CBillingTimer*)pVoid;

	HANDLE hTimer = CreateWaitableTimer( NULL, FALSE, NULL ) ;
	LARGE_INTEGER li = {0} ;

	// 1min = 60sec = 60,000ms
	SetWaitableTimer( hTimer, &li, 60000, NULL, NULL, TRUE ) ;

	CUser*		pUser = NULL;
	int				iCurTime = 0;
	while(g_Threading) {
		WaitForSingleObject( hTimer, INFINITE ) ;

		pUser = pThis->Get(iCurTime);
		if(pUser == NULL) continue;

		// pUser의 빌링정보 재계산해서 TimerEvent 예약
		// ...
	}
	return 0;
}

#endif //_METEL_LOGIN_200608_
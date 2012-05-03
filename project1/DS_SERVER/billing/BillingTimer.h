#pragma once

#ifdef _METEL_LOGIN_200608_

struct BILLING_TIMER_NTF_USER {
	CUser*		pUser;
	int				iUid;
};

class CBillingTimer {
private:
	std::multimap<int, BILLING_TIMER_NTF_USER*>*	pmapEvent;
	CRITICAL_SECTION	csEvent;
	
	int		iUid;

public:
	CBillingTimer() {
		CS_INIT(csEvent);
		pmapEvent = NULL;
		iUid = 0;
		
	}
	~CBillingTimer() {
		CS_LOCK(csEvent);
		std::multimap<int, BILLING_TIMER_NTF_USER*>::iterator it;
		while(pmapEvent != NULL) {
			if(pmapEvent->empty() == TRUE) break;
			it = pmapEvent->begin();
			SAFE_DELETE(it->second);
			pmapEvent->erase(it);
		}
		SAFE_DELETE(pmapEvent);
		CS_UNLOCK(csEvent);
		CS_DEINIT(csEvent);
	}

	// iDestTime에 TimerEvent가 발생하도록 예약
	void Reserve(int iDestTime, CUser* pUser) {
		BILLING_TIMER_NTF_USER* p = new BILLING_TIMER_NTF_USER;
		p->pUser = pUser;
		p->iUid = ++iUid;
		CS_LOCK(csEvent);
		pmapEvent->insert(std::multimap<int, BILLING_TIMER_NTF_USER*>::value_type(iDestTime, p));
		CS_UNLOCK(csEvent);
	}

	// iCurTime에 발생할 TimerEvent가 있는지 확인
	// 없으면, return NULL
	CUser* Get(int iCurTime) {
		CS_LOCK(csEvent);
		std::multimap<int, BILLING_TIMER_NTF_USER*>::iterator it;
		it = pmapEvent->begin();
		if(iCurTime < it->first) {
			CS_UNLOCK(csEvent);
			return NULL;
		}
		else {
			CUser* pUser = it->second->pUser;
			pmapEvent->erase(it);
			CS_UNLOCK(csEvent);
			return pUser;
		}
	}

	static unsigned __stdcall BillingTimerProc(void* pVoid);
};

#endif //_METEL_LOGIN_200608_
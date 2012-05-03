#ifndef	_CRITICAL_SECTION_
#define _CRITICAL_SECTION_

class CS {
private:
	CRITICAL_SECTION	cs;

public:
	void Init() {
		//InitializeCriticalSectionAndSpinCount(&cs, (0x80000000)|2000);
		InitializeCriticalSection(&cs);
	}
	void Del() {
		DeleteCriticalSection(&cs);
	}
	void Lock() {
		EnterCriticalSection(&cs);
	}
	void Unlock() {
		LeaveCriticalSection(&cs);
	}
};

#endif	_CRITICAL_SECTION_
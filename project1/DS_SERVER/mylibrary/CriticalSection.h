#ifndef	_CRITICAL_SECTION_
#define _CRITICAL_SECTION_

#ifndef	_WIN32_WINNT
#define	_WIN32_WINNT		0x0501
#endif

class CS {
private:
	CRITICAL_SECTION	cs;

public:
	CS() { Init(); }
	~CS() { Del(); }

	void Init() {
		InitializeCriticalSectionAndSpinCount(&cs, (0x80000000)|2000);
		//InitializeCriticalSection(&cs);
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
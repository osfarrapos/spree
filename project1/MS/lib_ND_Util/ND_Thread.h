#ifndef _ND_SERVER_THREAD_OBJECT_MADE_BY_HSM_COPY_RAY_
#define _ND_SERVER_THREAD_OBJECT_MADE_BY_HSM_COPY_RAY_


/*#################################################################################
//
//  CThread : 2004/05/28
		JetByte 의 CThread를 간략히한 버전.
//
//###############################################################################*/

namespace ND_Util {

class CThread 
{
public :
	
	CThread();
	
	virtual ~CThread();
	
	HANDLE GetHandle() const;
	
	bool Wait() const;
	
	bool Wait(DWORD timeoutMillis) const;
	
	bool Start();
	
	void Terminate(
		DWORD exitCode = 0);
	
private :
	
	virtual int Run() = 0;
	
	static unsigned int __stdcall ThreadFunction(void *pV);
	
	HANDLE m_hThread;
	
	// No copies do not implement
	CThread(const CThread &rhs);
	CThread &operator=(const CThread &rhs);
};

} // ND_Util

#endif
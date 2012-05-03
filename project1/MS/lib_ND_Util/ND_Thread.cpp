#include "stdafx.h"
#include "ND_Thread.h"

namespace ND_Util {


///////////////////////////////////////////////////////////////////////////////
// CThread
///////////////////////////////////////////////////////////////////////////////

CThread::CThread()
   :  m_hThread(INVALID_HANDLE_VALUE)
{

}
      
CThread::~CThread()
{
   if (m_hThread != INVALID_HANDLE_VALUE)
   {
      ::CloseHandle(m_hThread);
   }

   //lint -e{1540} m_hThread neither freed nor zeroed in destructor
}

HANDLE CThread::GetHandle() const
{
   return m_hThread;
}

bool CThread::Start()
{
   if (m_hThread == INVALID_HANDLE_VALUE)
   {
      unsigned int threadID = 0;

      m_hThread = (HANDLE)::_beginthreadex(0, 0, ThreadFunction, (void*)this, 0, &threadID);

      if (m_hThread == INVALID_HANDLE_VALUE)
      {
         return false ;
      }

	  return true ;
   }
   else
   {
      return false ;
   }
}

bool CThread::Wait() const
{
   if (!Wait(INFINITE))
   {
		return false ;
   }
   return true ;
}

bool CThread::Wait(DWORD timeoutMillis) const
{
   // TODO base class? Waitable?
   bool ok;

   DWORD result = ::WaitForSingleObject(m_hThread, timeoutMillis);

   if (result == WAIT_TIMEOUT)
   {
      ok = false;
   }
   else if (result == WAIT_OBJECT_0)
   {
      ok = true;
   }
   else
   {
      ok = false ;
   }
    
   return ok;
}

unsigned int __stdcall CThread::ThreadFunction(void *pV)
{
   int result = 0;

   CThread* pThis = (CThread*)pV;
   
   if (pThis)
   {
      try
      {
         result = pThis->Run();
      }
      catch(...)
      {
      }
   }

   return result;
}

void CThread::Terminate(
   DWORD exitCode /* = 0 */)
{
   if (!::TerminateThread(m_hThread, exitCode))
   {
      // TODO we could throw an exception here...
   }
}




} // ND_Util

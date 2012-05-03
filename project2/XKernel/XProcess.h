// XProcess.h: interface for the XProcess class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPROCESS_H_
#define _XPROCESS_H_

#define _MAXLEN_PROCESSNAME		  64
#define _MAXACTIVEPROCESSES		 100

#define _XGPF_INITED			0x01
#define _XGPF_PAUSED			0x02

#include <windows.h>
#include "xinput.h"

class _XProcess
{
protected:
	int		m_processID;

public:

	DWORD	m_dwFlag;
	int     m_StackPlace;
	
	TCHAR	m_processname[ _MAXLEN_PROCESSNAME ];
	TCHAR	m_srcfilename[ _MAXLEN_PROCESSNAME ];

	BOOL    m_Initialized;	
public:
	
	_XProcess()
	{
		m_processID = -1;
		m_Initialized	= FALSE;
		memset( m_processname, 0, _MAXLEN_PROCESSNAME );
		memset( m_srcfilename, 0, _MAXLEN_PROCESSNAME );
	}
	_XProcess( int processid, TCHAR* processname, TCHAR* srcfilename  )
	{
		m_Initialized	= FALSE;
		setProcessInfo( processid, processname, srcfilename );
	}

	void setProcessInfo( int processid, TCHAR* processname, TCHAR* srcfilename )
	{
		m_processID = processid;
		memset( m_processname, 0, _MAXLEN_PROCESSNAME );
		strncpy( m_processname, processname, _MAXLEN_PROCESSNAME-1 );
		memset( m_srcfilename, 0, _MAXLEN_PROCESSNAME );
		strncpy( m_srcfilename, srcfilename, _MAXLEN_PROCESSNAME-1 );
	}

	void    SetEnableProcess( BOOL enable ){ m_Initialized = enable; }

public:
	virtual BOOL InitializeProcess( void ){ return TRUE; }
	virtual void DestroyProcess( void ){}

	virtual bool InitDeviceObject( void ){ return true; }
	virtual void ReleaseDeviceObject( void ){ }
	
	virtual void PauseProcess( void ){}
	virtual void UnPauseProcess( void ){}

	virtual void Draw( void ){}
	virtual bool Process( void ) = 0;
	
	virtual void MessageProc( UINT message, WPARAM wparam, LPARAM lparam ){}
	virtual void OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam ){}
	virtual void OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam ){}
	virtual void OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam ){}
	virtual void OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam ){}
	virtual bool OnKeyboardPoll(_XInput *pInput){ return true; }	
	virtual bool OnKeyDown(WPARAM wparam, LPARAM lparam){ return true; }
	virtual bool OnKeyUp(WPARAM wparam, LPARAM lparam){ return true; }
	virtual bool OnMousePoll(MouseState *pState, ScrnPos *pPos){ return true; }
	virtual bool OnMouseButton(_XMouseButton p_MB, bool bPushed){ return true; }
	virtual void OnMouseMove( WPARAM wparam, LPARAM lparam ){}
	virtual void OnMouseWheel( short zDelta ){}
	virtual void OnMouseLBtnDblClick( WPARAM wparam ){}
	virtual void OnMouseRBtnDblClick( WPARAM wparam ){}
	virtual void OnMouseMBtnDblClick( WPARAM wparam ){}
	virtual void OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam ){}
	virtual void OnWindowSizeChangeNotify( void ){}

	virtual void OnTimer( WPARAM wparam, LPARAM lparam ){}
	virtual void OnPaint( WPARAM wparam, LPARAM lparam ){}
	
	virtual void OnNexonMessangerEvent( WPARAM wparam, LPARAM lparam ){} // Nexon messanger event
};



class _XProcessManager
{
protected:
	CRITICAL_SECTION m_cs;

	_XProcess* m_CurrProcess;
	_XProcess* m_PreviousProcess;
	_XProcess* m_ProcessStack[_MAXACTIVEPROCESSES];
	int        m_NProcesses;
	int		   m_bForceChangeProcess;


public:
	_XProcessManager();
	virtual ~_XProcessManager();
	
	void Lock();
	void Unlock();
	
	void ChangeProcess(_XProcess* process);

	void ForceChangeProcess(_XProcess* process);

	bool ProcessHandler(void);

	void SuspendProcess(_XProcess* process);

	void ResumeProcess(_XProcess* process);

	void ResumeProcessCloseAll(_XProcess* process);

	void CloseAllProcess();

	// Message handler
	void OnPaintHandler( void );
	bool OnKeyboardPoll(_XInput *pInput);	
	bool OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool OnMousePoll(MouseState *pState, ScrnPos *pPos);
	bool OnMouseButton(_XMouseButton p_MB, bool bPushed);
	void OnMouseMove( WPARAM wparam, LPARAM lparam );
	void OnMessageHandler( UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	void OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam );
	void OnMouseWheel( short zDelta );
	void OnMouseLBtnDblClick( WPARAM wparam );
	void OnMouseRBtnDblClick( WPARAM wparam );
	void OnMouseMBtnDblClick( WPARAM wparam );
	void OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam );
	void OnWindowSizeChangeNotify( void );
	void OnTimer( WPARAM wparam, LPARAM lparam );
	void OnPaint( WPARAM wparam, LPARAM lparam );
	void OnNexonMessangerEvent( WPARAM wparam, LPARAM lparam ); // Nexon messanger event

	bool InitProcessDeviceObject( void );
	void ReleaseProcessDeviceObject( void );

	inline _XProcess* GetCurrentProcess(){ return m_CurrProcess; }
	inline _XProcess* GetPreviousProcess(){ return m_PreviousProcess; }	

};



#endif 

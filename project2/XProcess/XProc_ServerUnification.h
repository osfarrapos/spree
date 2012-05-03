// XProc_ServerUnification.h: interface for the XProc_ServerUnification class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_SERVERUNIFICATION_H__E7F6BBE9_966A_471A_A4E6_935814D82418__INCLUDED_)
#define AFX_XPROC_SERVERUNIFICATION_H__E7F6BBE9_966A_471A_A4E6_935814D82418__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class XProc_ServerUnification : public _XProcess  
{
private:
	_XWindowManager		m_WindowManager;
	
	_XTextureManager	m_TextureArchive1;
	_XTextureManager	m_TextureArchive2;

	_XImageStatic		m_BackgroundStatic;	
public:
	XProc_ServerUnification();
	virtual ~XProc_ServerUnification();

	BOOL			InitializeProcess( void );
	void			DestroyProcess( void );
	
	bool			InitDeviceObject( void );
	void			ReleaseDeviceObject( void );

	void			PauseProcess( void );
	void			UnPauseProcess( void );

	void			Draw( void );
	bool			Process( void );
	
	void			MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	void			OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );
	void			OnPaint(WPARAM wparam, LPARAM lparam);

	void			ConfirmDevice(void);
	
	BOOL			CreateOffScreen( void );
	_XWindowManager* GetWindowManagerPtr( void) { return &(this->m_WindowManager);	};
};

#endif // !defined(AFX_XPROC_SERVERUNIFICATION_H__E7F6BBE9_966A_471A_A4E6_935814D82418__INCLUDED_)

// XProc_DeleteCharacter.h: interface for the XProc_DeleteCharacter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_DELETECHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_)
#define AFX_XPROC_DELETECHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XLodTerrain.h"
#include "X3PCamera.h"
#include "XBipedController.h"
#include "XUser.h"

class XProc_DeleteCharacter : public _XProcess  
{

public:

	XProc_DeleteCharacter();
	virtual ~XProc_DeleteCharacter();

	BOOL			InitializeProcess( void );
	void			DestroyProcess( void );
	
	bool			InitDeviceObject( void );
	void			ReleaseDeviceObject( void );
	
	void			PauseProcess( void );
	void			UnPauseProcess( void );

	void			ConfirmDevice(void);
	void			DrawFadeInOut(void);

	void			Draw( void );
	bool			Process( void );
	
	void			MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	void			OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam );	
	bool			OnKeyboardPoll(_XInput *pInput);
	bool			OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool			OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool			OnMousePoll(MouseState *pState, ScrnPos *pPos);
	bool			OnMouseButton(_XMouseButton p_MB, bool bPushed);
    void			OnMouseWheel( short zDelta );
	void			OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam );	
	void			OnWindowSizeChangeNotify( void );

public:
};

#endif // !defined(AFX_XPROC_DELETECHARACTER_H__8ADB701F_F4F4_4082_A6EC_FDD912F19D75__INCLUDED_)

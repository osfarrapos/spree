// XProc_NineDragonsTitle.h: interface for the XProc_NineDragonsTitle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XPROC_NINEDRAGONSTITLE_H__D273E067_ED9F_4DE5_9CF4_B61ED9424F49__INCLUDED_)
#define AFX_XPROC_NINEDRAGONSTITLE_H__D273E067_ED9F_4DE5_9CF4_B61ED9424F49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class CFlashControlInterface;

typedef enum _XNDTitlePhase
{
	_XNDTitlePhase_Indy21 = 0,
	_XNDTitlePhase_TripleXLogo,
	_XNDTitlePhase_NineDragonsTitle
};

class XProc_NineDragonsTitle : public _XProcess  
{
public:
	_XCamera				m_Player;	
	_XImageStatic			m_FlashStatic;
	_XNDTitlePhase			m_TitlePhase;
	DWORD					m_ProcessEndTimer;
public:
	XProc_NineDragonsTitle();
	virtual ~XProc_NineDragonsTitle();


	BOOL InitializeProcess( void );
	void DestroyProcess( void );
	
	bool InitDeviceObject( void );
	void ReleaseDeviceObject( void );
	
	void Draw( void );
	bool Process( void );
	
	void FlashDraw( void );
	
	void MessageProc( UINT message, WPARAM wparam, LPARAM lparam );	
	bool OnKeyboardPoll(_XInput *pInput);
	bool OnKeyDown(WPARAM wparam, LPARAM lparam);
	bool OnKeyUp(WPARAM wparam, LPARAM lparam);
	bool OnMousePoll(MouseState *pState, ScrnPos *pPos);
	void OnMouseMove( WPARAM wparam, LPARAM lparam );
	bool OnMouseButton(_XMouseButton p_MB, bool bPushed);
	void OnTimer(WPARAM wparam, LPARAM lparam);	
	void OnPaint(WPARAM wparam, LPARAM lparam);	
	
	BOOL CreateOffScreen( void );
};

#endif // !defined(AFX_XPROC_NINEDRAGONSTITLE_H__D273E067_ED9F_4DE5_9CF4_B61ED9424F49__INCLUDED_)

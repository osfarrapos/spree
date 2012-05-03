// XWindow_FSWindow.h: interface for the XWindow_FSWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_FSWINDOW_H__DC042D95_B008_4266_82C3_4CC2821A4D14__INCLUDED_)
#define AFX_XWINDOW_FSWINDOW_H__DC042D95_B008_4266_82C3_4CC2821A4D14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_FSWindow : public _XWindow  
{
public:
	
	_XStickGauge*			m_pLifePowerBorder;
	_XStickGauge*			m_pForcePowerBorder;	
	_XStickGauge*			m_pConcentrationPowerBorder;
	
protected :
	
public :
	
public:
	_XWindow_FSWindow();
	virtual ~_XWindow_FSWindow();
	
	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void MoveWindow( int X, int Y );
	
	void SetDefaultPosition( void );
	void SaveWindowProperty( void );
	
	void SetParameter( void );	
};

#endif // !defined(AFX_XWINDOW_FSWINDOW_H__DC042D95_B008_4266_82C3_4CC2821A4D14__INCLUDED_)

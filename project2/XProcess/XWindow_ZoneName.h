// XWindow_ZoneName.h: interface for the _XWindow_ZoneName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_ZONENAME_H__5CCE1428_D9D8_44F3_BD74_E4AE36137729__INCLUDED_)
#define AFX_XWINDOW_ZONENAME_H__5CCE1428_D9D8_44F3_BD74_E4AE36137729__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"
#include "XStickGauge.h"

typedef enum _XZoneWindowEffectClass
{
	_XZoneWindowEffectClass_Stop = 0,
	_XZoneWindowEffectClass_Open,
	_XZoneWindowEffectClass_Close
};

#define _XDEF_MAXZONENAMEWINDOWSTRINGSIZE	64

class _XWindow_ZoneName : public _XWindow  
{
	_XImageStatic	m_pLeftStick;
	_XImageStatic	m_pRightStick;
	_XImageStatic	m_pBorder;
#ifdef _XDEF_EXPAND_ZONENAME
	_XImageStatic	m_pBorder2;
#endif

	

	_XZoneWindowEffectClass	m_EffectStaus;
	FLOAT					m_EffectProgress;
	BOOL					m_DropStatus;
	FLOAT					m_DropProgress;
	DWORD					m_EffectTime;
	BOOL					m_WorldNameDisplayMode;
	TCHAR					m_ZoneName[_XDEF_MAXZONENAMEWINDOWSTRINGSIZE];

#ifdef _XJAPANESE
	TCHAR					m_ZoneHiraganaName[_XDEF_MAXZONENAMEWINDOWSTRINGSIZE];
#endif

public:
	_XWindow_ZoneName();
	virtual ~_XWindow_ZoneName();

	BOOL Initialize( void );
	void DestroyWindow( void );
	
	void ShowWindow( BOOL show );
	void MoveWindow( int X, int Y );

	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void StartAnimation( _XZoneWindowEffectClass effectclass );	

#ifdef _XJAPANESE
	void	SetZoneName( LPTSTR zonename, LPTSTR zonehiragananame );
#else
	void	SetZoneName( LPTSTR zonename );
#endif
	LPTSTR	GetZoneName( void ){ return m_ZoneName; }
	
	BOOL CompareLastLandMark( LPTSTR landmarkname );

	void SetWorldNameDisplayMode( BOOL mode )
	{
		m_WorldNameDisplayMode = mode;
	}
	BOOL GetWorldNameDisplayMode( void )
	{
		return m_WorldNameDisplayMode;
	}
};

#endif // !defined(AFX_XWINDOW_ZONENAME_H__5CCE1428_D9D8_44F3_BD74_E4AE36137729__INCLUDED_)

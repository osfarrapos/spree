// XWindow_GM_UserCoordination.h: interface for the _XWindow_GM_UserCoordination class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GM_USERCOORDINATION_H__E80692FB_1BDF_4660_8673_02FE981D9039__INCLUDED_)
#define AFX_XWINDOW_GM_USERCOORDINATION_H__E80692FB_1BDF_4660_8673_02FE981D9039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum _XGMCoordControlType
{
	_XGMCOORDCONTROLTYPE_GMMOVE = 0,
	_XGMCOORDCONTROLTYPE_GMMOVETOUSER,
	_XGMCOORDCONTROLTYPE_MOVEUSER,
};

class _XWindow_GM_UserCoordination : public _XWindow  
{
public:
	_XIMEContainer				m_XCoordIMEControl;
	_XIMEContainer				m_ZCoordIMEControl;
	_XIMEContainer				m_CharacterNameIMEControl;
	
public:
	_XWindow_GM_UserCoordination();
	virtual ~_XWindow_GM_UserCoordination();


	BOOL						Initialize( void );
	void						DestroyWindow( void );
	
	void						Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL						Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void						MoveWindow( int X, int Y );
	void						ShowWindow( BOOL show );

	void						Run( _XGMCoordControlType mode );

};

#endif // !defined(AFX_XWINDOW_GM_USERCOORDINATION_H__E80692FB_1BDF_4660_8673_02FE981D9039__INCLUDED_)

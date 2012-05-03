// XWindow_GM_UserCoordination.h: interface for the _XWindow_GM_EventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GM_EVENTMANAGER_H__E80692FB_1BDF_4660_8673_02FE981D9039__INCLUDED_)
#define AFX_XWINDOW_GM_EVENTMANAGER_H__E80692FB_1BDF_4660_8673_02FE981D9039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_GM_EventManager : public _XWindow  
{
public:
		
public:
	_XWindow_GM_EventManager();
	virtual ~_XWindow_GM_EventManager();


	BOOL						Initialize( void );
	void						DestroyWindow( void );
	
	void						Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL						Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	
	void						MoveWindow( int X, int Y );
	void						ShowWindow( BOOL show );
};

#endif // !defined(AFX_XWINDOW_GM_EVENTMANAGER_H__E80692FB_1BDF_4660_8673_02FE981D9039__INCLUDED_)

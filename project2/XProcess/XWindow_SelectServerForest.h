// XWindow_SelectServerForest.h: interface for the _XWindow_SelectServerForest class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SELECTSERVERFOREST_H__E23A1545_159E_4C72_9F70_B47772C38230__INCLUDED_)
#define AFX_XWINDOW_SELECTSERVERFOREST_H__E23A1545_159E_4C72_9F70_B47772C38230__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_SelectServerForest : public _XWindow  
{
public:
	_XListBox*			m_pListBox;
	
public:
	_XWindow_SelectServerForest();
	virtual ~_XWindow_SelectServerForest();


	BOOL Initialize( _XTextureManager& texturearchive );
	void DestroyWindow( void );	

	void ShowWindow( BOOL show );
	void MoveWindow( int X, int Y );
	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

};

#endif // !defined(AFX_XWINDOW_SELECTSERVERFOREST_H__E23A1545_159E_4C72_9F70_B47772C38230__INCLUDED_)

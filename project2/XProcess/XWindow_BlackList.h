// XWindow_BlackList.h: interface for the _XWindow_BlackList class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_BLACKLIST_H_
#define _XWINDOW_BLACKLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_BlackList : public _XWindow  
{
public:	
	
	_XListBox*			m_listboxBlackList;

	_XList				m_BlackList;

public:
	_XWindow_BlackList();
	virtual ~_XWindow_BlackList();

	BOOL Initialize( void );
	void DestroyWindow( void );
	void MoveWindow( int X, int Y );
	
	void ShowWindow( BOOL show ); 	
	void Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );	
	BOOL Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void RebuildListBox(void);

	BOOL InsertCharacter( LPCTSTR charactername );
	
	BOOL DeleteCharacter( LPCTSTR charactername );
	BOOL DeleteCharacter( int listindex );

	BOOL FindCharacter  ( LPCTSTR charactername );

	BOOL LoadList( LPCTSTR charactername );
	BOOL SaveList( LPCTSTR charactername );
};

#endif // !defined(_XWINDOW_BLACKLIST_H_)

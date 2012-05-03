// XGUIObject.h: interface for the _XGUIObject class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XGUIOBJECT_H_
#define _XGUIOBJECT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"

#define _XDEF_PURE	0

class  _XGUIObject;
extern _XGUIObject*				g_CurrentFocusedObject;
class _XWindow;

class _XGUIObject : public _XItem  
{
protected :	
	BOOL			m_Enable;		
	BOOL			m_ShowWindow;
	POINT			m_WindowPosition;
	SIZE			m_WindowSize;

	DWORD			m_CommandID;
	int				m_lParam;

public:	
	_XWindow*	m_pParentWindow;

public:
	_XGUIObject();
	virtual ~_XGUIObject();

	virtual void  SetLParam( int lparam ){ m_lParam = lparam; }
	virtual int	  GetLParam( void ){ return m_lParam; }

	virtual POINT GetWindowPos( void ){ return m_WindowPosition; }	
	virtual SIZE  GetWindowSize( void ){ return m_WindowSize; }	
	virtual void  SetWindowSize( SIZE size ){ m_WindowSize = size; }	
	virtual void  SetWindowSize( int width, int height ){ m_WindowSize.cx = width; m_WindowSize.cy = height; }	

	virtual void  MoveWindow( int X, int Y ) = _XDEF_PURE;
	virtual void  DestroyWindow( void ) = _XDEF_PURE;

	virtual void  Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject ) = _XDEF_PURE;
	virtual BOOL  Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject ) = _XDEF_PURE;
	
	virtual void  EnableWindow( BOOL enable ) = _XDEF_PURE;
	virtual void  ShowWindow( BOOL show ) = _XDEF_PURE;
	virtual BOOL  CheckMousePosition( void ) = _XDEF_PURE;

	virtual void  RefreshData( void ){ }
	
	virtual BOOL  GetShowStatus(void)		{	return m_ShowWindow;	 };
	virtual BOOL  GetEnableStatus(void)		{	return m_Enable;	 };

	virtual BOOL  ProcessWheelMessage( short zDelta ){ return FALSE; } 
	
	virtual void  NotifyUIChange( void ){ }
	
	virtual void  CenterMouse( void );
};

#endif // !defined(_XGUIOBJECT_H_)

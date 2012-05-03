// XWindow_MnDTotalPoint.h: interface for the _XWindow_MnDTotalPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MNDTOTALPOINT_H__AB7097FF_FBF9_4488_BDD3_A9955BCB7A70__INCLUDED_)
#define AFX_XWINDOW_MNDTOTALPOINT_H__AB7097FF_FBF9_4488_BDD3_A9955BCB7A70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

class _XTextureAnimation;

class _XWindow_MnDTotalPoint : public _XWindow
{
public:
	_XWindow_MnDTotalPoint();
	~_XWindow_MnDTotalPoint();

	BOOL	Initialize();
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	void	SetNumberProcess();

public:
	_XTextureAnimation* m_pFullPointStartAni; //처음에 한번만 나오는 애니
	_XTextureAnimation* m_pFullPointAni;	  //999 일 때 계속 나옴
	_XTextureAnimation* m_pNumberAni[3][10];  //3자리 10개 숫자

private :
	_XImageStatic*	m_pTitleImage;
	int	m_nDrawNumber[3]; //3자리에 그려야 되는 숫자
};

#endif // !defined(AFX_XWINDOW_MNDTOTALPOINT_H__AB7097FF_FBF9_4488_BDD3_A9955BCB7A70__INCLUDED_)

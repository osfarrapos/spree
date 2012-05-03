// XWindow_AgeGrade.cpp: implementation of the XWindow_AgeGrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"

#include "XWindow_AgeGrade.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_AgeGrade* _XWindow_AgeGrade::m_pInstance = 0;

_XWindow_AgeGrade::_XWindow_AgeGrade() : m_dwCurrentCheckTime(0)
{

}

_XWindow_AgeGrade::~_XWindow_AgeGrade()
{
	
}

_XWindow_AgeGrade* _XWindow_AgeGrade::GetInstance()
{
	if( m_pInstance == 0 )
	{
		m_pInstance = new _XWindow_AgeGrade;
	}

	return m_pInstance;
}

void _XWindow_AgeGrade::Destroy()
{
	if( _XWindow_AgeGrade::m_pInstance ) //인스턴스 삭제
	{
		delete _XWindow_AgeGrade::m_pInstance;
		_XWindow_AgeGrade::m_pInstance = NULL;
	}
}

BOOL _XWindow_AgeGrade::Initialize()
{
	_XImageStatic* pImage = new _XImageStatic;
	pImage->Create( 0, 0, 402, 167, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_agegrade.tga" ) );	
	pImage->SetClipRect( 110, 0, 512, 167 );
	InsertChildObject( pImage );

	return TRUE;
}

void _XWindow_AgeGrade::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_AgeGrade::ShowWindow( BOOL show )
{
	if( show )
	{
		m_dwCurrentCheckTime = g_LocalSystemTime;
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_AgeGrade::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_AgeGrade::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;

	_XWindow::Draw(pfocusobject);
}

BOOL _XWindow_AgeGrade::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	if( (g_LocalSystemTime - m_dwCurrentCheckTime) > 3500 )
		this->ShowWindow(FALSE);

	return TRUE;
}
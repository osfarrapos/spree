// XWindow_Mark.cpp: implementation of the _XWindow_Mark class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_Mark.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Mark::_XWindow_Mark()
{
	m_pImgAttackType[0] = NULL;
	m_pImgAttackType[1] = NULL;
	m_pImgCaptain = NULL;
}

_XWindow_Mark::~_XWindow_Mark()
{

}

BOOL _XWindow_Mark::Initialize()
{
	for(int i = 0 ; i < 2 ; ++i)
	{
		m_pImgAttackType[i] = new _XImageStatic;
		m_pImgAttackType[i]->Create(0, 0, 30, 23, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_Manor02.tga"));
	}
	m_pImgAttackType[0]->SetClipRect(94, 129, 124, 152);
	m_pImgAttackType[1]->SetClipRect(125, 129, 155, 152);

	m_pImgCaptain = new _XImageStatic;
	m_pImgCaptain->Create(0, 0, 18, 18, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_Manor02.tga"));
	m_pImgCaptain->SetClipRect(156, 129, 174, 147);

	return TRUE;
}

void _XWindow_Mark::DestroyWindow()
{
	SAFE_DELETE(m_pImgAttackType[0]);
	SAFE_DELETE(m_pImgAttackType[1]);
	SAFE_DELETE(m_pImgCaptain);

	_XWindow::DestroyWindow();
}

void _XWindow_Mark::Draw(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return;

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
}

BOOL _XWindow_Mark::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if( !_XWindow::Process(pfocusobject) ) 
		return FALSE;

	return TRUE;
}

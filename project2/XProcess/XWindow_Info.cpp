// XWindow_Info.cpp: implementation of the _XWindow_Info class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "XWindow_Info.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Info::_XWindow_Info()
{
	m_bSetTooltip = FALSE;
}

_XWindow_Info::~_XWindow_Info()
{

}

BOOL _XWindow_Info::Initialize()
{
	_XImageStatic* pIcon = new _XImageStatic;
	pIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("skill_icon_08_02.tga"));
	pIcon->SetClipRect(162, 2, 192, 32);
	InsertChildObject(pIcon);

	_XImageStatic* pBar = new _XImageStatic;
	pBar->Create(35, 0, 40, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_char.tga"));
	pBar->SetClipRect(251, 3, 256, 33);
	pBar->SetScale(-1, 1);
	InsertChildObject(pBar);

	return TRUE;
}

void _XWindow_Info::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_Info::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
	_XDrawRectAngle(m_WindowPosition.x, m_WindowPosition.y,
						m_WindowPosition.x+30, m_WindowPosition.y+30, 0.0f, D3DCOLOR_ARGB(255, 158, 155, 148));

	if(CheckMousePosition())
	{
		ShowTooltip(TRUE);
	}
	else
	{
		ShowTooltip(FALSE);
	}
}

BOOL _XWindow_Info::Process( _XGUIObject*& pfocusobject )
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

void _XWindow_Info::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow(show);
}

void _XWindow_Info::ShowTooltip(BOOL show)
{
	if(show)
	{
		if(!m_bSetTooltip)
		{
			g_pMiniTooltip_Window->SetTooltipMode(MTMODE_CASTLEINFO, 0, 0, 0);	
			g_pMiniTooltip_Window->ShowWindow(TRUE);
		}
		else
		{
			if(!g_pMiniTooltip_Window->GetShowStatus())
				g_pMiniTooltip_Window->ShowWindow(TRUE);
		}
	}
	else
	{
		if(g_pMiniTooltip_Window)
		{
			if(g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_CASTLEINFO)
				g_pMiniTooltip_Window->ShowWindow(FALSE);

			m_bSetTooltip = FALSE;
		}
	}
}
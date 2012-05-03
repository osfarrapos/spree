// XWindow_RewardNick.cpp: implementation of the _XWindow_RewardNick class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_RewardNick.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_RewardNick* g_pRewardNick_Window = NULL;

_XWindow_RewardNick::_XWindow_RewardNick()
{

}

_XWindow_RewardNick::~_XWindow_RewardNick()
{

}

BOOL _XWindow_RewardNick::Initialize(void)
{
	g_pRewardNick_Window = this;

	_XImageStatic* pDecoBorder = new _XImageStatic;
	pDecoBorder->Create(0 , 0, 226, 81, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_pvp_03.tga"));
	pDecoBorder->SetClipRect(16, 33, 241, 112);
	InsertChildObject(pDecoBorder);

	return TRUE;
}

void _XWindow_RewardNick::DestroyWindow()
{
}

void _XWindow_RewardNick::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	if(m_nNick > 0)
	{
		g_XBaseFont->SetColor(0xFFFFF3F5);
		g_XBaseFont->PrintAlign(m_WindowPosition.x+113, m_WindowPosition.y+48, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(m_nNick));
		g_XBaseFont->Flush();
	}
}

BOOL _XWindow_RewardNick::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_RewardNick::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow(show);
}
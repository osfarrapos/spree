// XWindow_MatchEvent.cpp: implementation of the _XWindow_MatchEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_MatchEvent.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchEvent::_XWindow_MatchEvent()
{
	m_pImgYellowDot = NULL;
	m_pImgRedDot = NULL;
	
	for(int i = 0 ; i < 10 ; ++i)
	{
		m_pImgYellowNumber[i] = NULL;
		m_pImgRedNumber[i] = NULL;
	}
}

_XWindow_MatchEvent::~_XWindow_MatchEvent()
{

}

BOOL _XWindow_MatchEvent::Initialize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_pvp_03.tga");

	_XImageStatic* pWinCnt = new _XImageStatic;
	pWinCnt->Create(0, 0, 76, 65, &g_MainInterfaceTextureArchive, resourceindex);
	pWinCnt->SetClipRect(29, 120, 105, 185);
	InsertChildObject(pWinCnt);

	_XImageStatic* pLoseCnt = new _XImageStatic;
	pLoseCnt->Create(279, 0, 279+76, 65, &g_MainInterfaceTextureArchive, resourceindex);
	pLoseCnt->SetClipRect(119, 120, 195, 185);
	InsertChildObject(pLoseCnt);

	for(int i = 0 ; i < 10 ; ++i)
	{
		m_pImgYellowNumber[i] = new _XImageStatic;
		m_pImgYellowNumber[i]->Create(0, 0, 13, 16, &g_MainInterfaceTextureArchive, resourceindex);
		m_pImgYellowNumber[i]->SetClipRect(24, 195, 24+(i*18), 195+16);

		m_pImgRedNumber[i] = new _XImageStatic;
		m_pImgRedNumber[i]->Create(0, 0, 13, 16, &g_MainInterfaceTextureArchive, resourceindex);
		m_pImgRedNumber[i]->SetClipRect(0, 0, 24+(i*18), 217+16);
	}

	return TRUE;
}

void _XWindow_MatchEvent::DestroyWindow()
{
	for(int i = 0 ; i < 10 ; ++i)
	{
		SAFE_DELETE(m_pImgYellowNumber[i]);
		SAFE_DELETE(m_pImgRedNumber[i]);
	}
	SAFE_DELETE(m_pImgYellowDot);
	SAFE_DELETE(m_pImgRedDot);
}

void _XWindow_MatchEvent::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XLargeFont->SetColor(0xFFE2E059);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+62, m_WindowPosition.y+43, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_nKillCount);
	g_XLargeFont->Flush();

	g_XLargeFont->SetColor(0xFFE90100);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+342, m_WindowPosition.y+43, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_nDeadCount);
	g_XLargeFont->Flush();
	g_XLargeFont->DisableGrowMode();

	int min = 0, sec = 0;
	g_MatchEventManager.GetLeftTimeMatchEvent(min, sec);
	g_XLargeFont->SetColor(0xFFF5E88A);
	
	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN)
		g_XLargeFont->PrintAlign(m_WindowPosition.x+181, m_WindowPosition.y+105, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%02d;%02d", min, sec);
	else
		g_XLargeFont->PrintAlign(m_WindowPosition.x+181, m_WindowPosition.y+105, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%02d:%02d", min, sec);
	g_XLargeFont->Flush();

}

BOOL _XWindow_MatchEvent::Process(_XGUIObject*& pfocusobject)
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

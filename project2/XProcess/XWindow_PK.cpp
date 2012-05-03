// XWindow_PK.cpp: implementation of the _XWindow_PK class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_PK.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_PK::_XWindow_PK()
{
	m_TimeImage = NULL;
}

_XWindow_PK::~_XWindow_PK()
{
	
}

BOOL _XWindow_PK::Initialize(void)
{
	m_borderstatic.SetClipRect(0, 0, 178, 42);
	
	m_TimeImage = new _XImageStatic;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		m_TimeImage->Create(0, 0, 52, 18, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("tw_mi_pvp_01.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		m_TimeImage->Create(0, 0, 52, 18, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("us_mi_pvp_01.tga"));
	}	
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_TimeImage->Create(0, 0, 52, 18, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("rs_mi_pvp_01.tga"));
	}
	else
	{
		m_TimeImage->Create(0, 0, 52, 18, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_pvp_01.tga"));
	}
	
	
	m_TimeImage->SetClipRect(135, 68, 187, 86);

	return TRUE;
}

void _XWindow_PK::DestroyWindow()
{
	SAFE_DELETE(m_TimeImage);
}

void _XWindow_PK::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
	{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		if(!g_pLocalUser->GetEnterMatchEvent())
		{
#endif
			// 비무 대회 중일때는 보여주지 않음
			m_TimeImage->Draw(m_WindowPosition.x+65, m_WindowPosition.y+45);
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
		}
#endif

		if(g_pLocalUser->m_PKModeStartTime > 0)
		{
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 0, 0));

			if((g_pLocalUser->m_PKModeStartTime + g_pLocalUser->m_PKModeLeftTime) >= g_LocalSystemTime)
			{
				DWORD lefttime = (g_pLocalUser->m_PKModeStartTime + g_pLocalUser->m_PKModeLeftTime) - g_LocalSystemTime;	
				lefttime /= 1000; 

				int min = lefttime / 60;
				int sec = lefttime % 60;
				
				g_XBaseFont->PrintAlign(m_WindowPosition.x+91, m_WindowPosition.y+49, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%02d:%02d", min, sec);
			}
			else
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+91, m_WindowPosition.y+49, 1.0f, _XFONT_ALIGNTYPE_CENTER, "00:00");
			}
			g_XBaseFont->Flush();
		}
	}
}

BOOL _XWindow_PK::Process(_XGUIObject*& pfocusobject)
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

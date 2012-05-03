// XWindow_MatchMessage.cpp: implementation of the _XWindow_MatchMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_MatchMessage.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchMessage::_XWindow_MatchMessage()
{

}

_XWindow_MatchMessage::~_XWindow_MatchMessage()
{

}

BOOL _XWindow_MatchMessage::Initialize()
{

	int numberresourceindex = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		numberresourceindex = g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		numberresourceindex = g_MainInterfaceTextureArchive.FindResource("us_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		numberresourceindex = g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_04.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		numberresourceindex = g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_04.tga");
	}
	else 
	{
		numberresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_unki_04.tga");
	}	

	for(int i = 0 ; i < 6 ; i++)
		m_NumberBigImage[i].Create(0, 0, 32, 48, &g_MainInterfaceTextureArchive, numberresourceindex);

	m_NumberBigImage[0].SetClipRect(69, 167, 69+31, 167+44);
	m_NumberBigImage[1].SetClipRect(3, 121, 3+31, 121+44);
	m_NumberBigImage[2].SetClipRect(35, 121, 35+31, 121+44);
	m_NumberBigImage[3].SetClipRect(69, 121, 69+31, 121+44);
	m_NumberBigImage[4].SetClipRect(103, 121, 103+31, 121+44);
	m_NumberBigImage[5].SetClipRect(136, 121, 136+31, 121+44);

	int pvpresourceindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("vn_mi_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pvp_02.tga");
	}
	else 
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_pvp_02.tga");
	}
	

	m_StartMatchImage.Create(0, 0, 117, 58, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_StartMatchImage.SetClipRect(132, 190, 249, 248);

	m_EndMatchImage.Create(0, 0, 115, 58, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_EndMatchImage.SetClipRect(8, 190, 123, 248);

	m_TimeOverImage.Create(0, 0, 79, 50, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_TimeOverImage.SetClipRect(6, 138, 85, 188);
	
	m_TimeOverEndImage.Create(0, 0, 79, 50, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_TimeOverEndImage.SetClipRect(6, 138, 85, 188);

	return TRUE;
}

void _XWindow_MatchMessage::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;
	
	// TODO
	if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_COUNTING)
	{
		if(g_LocalSystemTime - g_pLocalUser->m_MatchStateChangeTime < 1000)
		{
			m_NumberBigImage[5].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else if(g_LocalSystemTime - g_pLocalUser->m_MatchStateChangeTime < 2000)
		{
			m_NumberBigImage[4].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else if(g_LocalSystemTime - g_pLocalUser->m_MatchStateChangeTime < 3000)
		{
			m_NumberBigImage[3].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else if(g_LocalSystemTime - g_pLocalUser->m_MatchStateChangeTime < 4000)
		{
			m_NumberBigImage[2].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else if(g_LocalSystemTime - g_pLocalUser->m_MatchStateChangeTime < 5000)
		{
			m_NumberBigImage[1].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else
		{
			m_NumberBigImage[0].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
	}
	else if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT)
	{
		if(g_LocalSystemTime - g_pLocalUser->m_MatchStateChangeTime <= 5000)
		{
			m_StartMatchImage.Draw((gnWidth>>1)-58, m_WindowPosition.y+132);
		}
	}
	else if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_END)
	{
		if(g_LocalSystemTime - g_pLocalUser->m_MatchStateChangeTime <= 10000)
		{
			if(g_pLocalUser->m_MatchResult == 0 || g_pLocalUser->m_MatchResult == 1)
			{
				m_EndMatchImage.Draw((gnWidth>>1)-52, m_WindowPosition.y+132);
			}
			else
			{
				m_TimeOverImage.Draw((gnWidth>>1)-38, m_WindowPosition.y+132);
			}
		}
	}

	// 끝나기전 countdown
	if(m_bCountdown)
	{
		m_TimeOverEndImage.Draw((gnWidth>>1)-39, m_WindowPosition.y+70);

		if(g_LocalSystemTime - m_dwCountdownStartTime< 2000)
		{
			m_NumberBigImage[5].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else if(g_LocalSystemTime - m_dwCountdownStartTime < 3000)
		{
			m_NumberBigImage[4].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else if(g_LocalSystemTime - m_dwCountdownStartTime < 4000)
		{
			m_NumberBigImage[3].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else if(g_LocalSystemTime - m_dwCountdownStartTime < 5000)
		{
			m_NumberBigImage[2].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else if(g_LocalSystemTime - m_dwCountdownStartTime < 6000)
		{
			m_NumberBigImage[1].Draw((gnWidth>>1)-16, m_WindowPosition.y+132);
		}
		else
		{
			m_bCountdown = FALSE;
		}
	}
}

BOOL _XWindow_MatchMessage::Process(_XGUIObject*& pfocusobject)
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

void _XWindow_MatchMessage::ResetData()
{
	m_bCountdown = FALSE;
	m_dwCountdownStartTime = 0;
}
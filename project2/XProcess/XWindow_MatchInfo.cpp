// XWindow_MatchInfo.cpp: implementation of the _XWindow_MatchInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_MatchInfo.h"
#include "EODEXTDEF_GlobalObject.h"
#include "Xproc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchInfo::_XWindow_MatchInfo()
{
	m_CloseButton = NULL;
}

_XWindow_MatchInfo::~_XWindow_MatchInfo()
{

}

BOOL _XWindow_MatchInfo::Initialize(void)
{
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {145, 50}, {46, 17},
		_XDEF_MATCHINFO_CLOSE,
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		&g_MainInterfaceTextureArchive
	};
	m_CloseButton = new _XButton;
	m_CloseButton->Create(btnstruct);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 13, 47, 30);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 30, 47, 48);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 47, 47, 64);
	m_CloseButton->SetButtonTextColor(_XSC_DEFAULT);
	m_CloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CLOSE);	// "´Ý±â"
	InsertChildObject(m_CloseButton);

	m_MatchID = 0;
	memset(m_NameString, 0, sizeof(TCHAR)*128);

	return TRUE;
}

void _XWindow_MatchInfo::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_MatchInfo::Draw(_XGUIObject*& pfocusobject)
{
/*	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	_XDrawRectAngle(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 
		0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	g_XBaseFont->Puts(m_WindowPosition.x+21, m_WindowPosition.y+13, _XGETINTERFACETEXT(ID_STRING_MATCH_MATCHTYPE));	// "ºñ¹« ÇüÅÂ"

	_XFlag* pFlag = (_XFlag*)g_FlagList.Find(m_MatchID);
	if(pFlag)
	{
		switch(pFlag->m_MatchType)
		{
		case en_match_type_pvp :
			g_XBaseFont->Puts(m_WindowPosition.x+81, m_WindowPosition.y+13, "1:1");
			break;
		case en_match_type_p_party :
			g_XBaseFont->Puts(m_WindowPosition.x+81, m_WindowPosition.y+13, _XGETINTERFACETEXT(ID_STRING_MATCH_TYPE2));	// "1:µ¿Çà"
			break;
		case en_match_type_party_p :
			g_XBaseFont->Puts(m_WindowPosition.x+81, m_WindowPosition.y+13, _XGETINTERFACETEXT(ID_STRING_MATCH_TYPE3));	// "µ¿Çà:1"
			break;
		case en_match_type_party_death :
			g_XBaseFont->Puts(m_WindowPosition.x+81, m_WindowPosition.y+13, _XGETINTERFACETEXT(ID_STRING_MATCH_TYPE4));	// "Àü¸êÀü"
			break;
		case en_match_type_party_round :
			g_XBaseFont->Puts(m_WindowPosition.x+81, m_WindowPosition.y+13, _XGETINTERFACETEXT(ID_STRING_MATCH_TYPE5));	// "´Ü½Â½Ä"
			break;
		case en_match_type_party_series :
			g_XBaseFont->Puts(m_WindowPosition.x+81, m_WindowPosition.y+13, _XGETINTERFACETEXT(ID_STRING_MATCH_TYPE6));	// "¿¬½Â½Ä"
			break;
		}

		g_XBaseFont->Puts(m_WindowPosition.x+21, m_WindowPosition.y+28, m_NameString);
		g_XBaseFont->Flush();
	}*/
}

BOOL _XWindow_MatchInfo::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return FALSE;
	if(!this->m_Enable)
		return FALSE;

	// TODO

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_MatchInfo::Reset(void)
{
	m_MatchID = 0;
	memset(m_NameString, 0, sizeof(TCHAR)*128);
}

void _XWindow_MatchInfo::ShowWindow(BOOL show)
{
	if(show)
	{
		TCHAR cName1[13];
		TCHAR cName2[13];
		memset(cName1, 0, sizeof(TCHAR)*13);
		memset(cName2, 0, sizeof(TCHAR)*13);

		_XFlag* pFlag = (_XFlag*)g_FlagList.Find(m_MatchID);
		if(pFlag)
		{
			if(g_pLocalUser->m_Uniqueid == pFlag->m_Applicant1)
			{
				strcpy(cName1, g_pLocalUser->m_CharacterInfo.charactername);
				_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(pFlag->m_Applicant2);
				if(pTargetUser)
					strcpy(cName2, pTargetUser->m_CharacterInfo.charactername);
			}
			else if(g_pLocalUser->m_Uniqueid == pFlag->m_Applicant2)
			{
				strcpy(cName2, g_pLocalUser->m_CharacterInfo.charactername);
				_XUser* pTargetUser = (_XUser*)g_Userlist.FindUser(pFlag->m_Applicant1);
				if(pTargetUser)
					strcpy(cName1, pTargetUser->m_CharacterInfo.charactername);
			}
			else
			{
				_XUser* pApplicant1 = (_XUser*)g_Userlist.FindUser(pFlag->m_Applicant1);
				_XUser* pApplicant2 = (_XUser*)g_Userlist.FindUser(pFlag->m_Applicant2);
				
				if(pApplicant1 && pApplicant2)
				{
					strcpy(cName1, pApplicant1->m_CharacterInfo.charactername);
					strcpy(cName2, pApplicant2->m_CharacterInfo.charactername);
				}
			}
		}
		
		sprintf(m_NameString, "%s  vs  %s", cName1, cName2);
		
		D3DXVECTOR3 rvec;
		D3DXVECTOR3 mpos = pFlag->m_Position;
		mpos.y += 2.2f;
		D3DXVec3Project(&rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);

		MoveWindow(rvec.x-(m_WindowSize.cx>>1), rvec.y);
	}

	_XWindow::ShowWindow(show);
}
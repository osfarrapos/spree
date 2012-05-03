// XWindow_MatchWindow.cpp: implementation of the _XWindow_MatchWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_MatchWindow.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchWindow::_XWindow_MatchWindow()
{
	m_AcceptButton = NULL;
	m_RefuseButton = NULL;
	m_DetailInfoButton = NULL;
}

_XWindow_MatchWindow::~_XWindow_MatchWindow()
{
}

BOOL _XWindow_MatchWindow::Initialize(void)
{
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {50, 113}, {46, 17},
		_XDEF_MATCH_ACCEPT,
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		&g_MainInterfaceTextureArchive
	};
	m_AcceptButton = new _XButton;
	m_AcceptButton->Create(btnstruct);
	m_AcceptButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 13, 47, 30);
	m_AcceptButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 30, 47, 48);
	m_AcceptButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 47, 47, 64);
	m_AcceptButton->SetButtonTextColor(_XSC_DEFAULT);
	m_AcceptButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_ACCEPT);	// "수락"
	InsertChildObject(m_AcceptButton);

	btnstruct.position.x = 245;
	btnstruct.commandid = _XDEF_MATCH_REFUSE;

	m_RefuseButton = new _XButton;
	m_RefuseButton->Create(btnstruct);
	m_RefuseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 13, 47, 30);
	m_RefuseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 30, 47, 48);
	m_RefuseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 47, 47, 64);
	m_RefuseButton->SetButtonTextColor(_XSC_DEFAULT);
	m_RefuseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_REFUSE);	// "거절"
	InsertChildObject(m_RefuseButton);

	btnstruct.position.x = 92;
	btnstruct.position.y = 27;
	btnstruct.windowsize.cx = 13;
	btnstruct.windowsize.cy = 13;
	btnstruct.commandid = _XDEF_MATCH_DETAILINFO_CHARAC;

	m_DetailInfoButton = new _XButton;
	m_DetailInfoButton->Create(btnstruct);
	m_DetailInfoButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 0, 13, 13);
	m_DetailInfoButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 13, 0, 26, 13);
	m_DetailInfoButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 26, 0, 39, 13);
	m_DetailInfoButton->SetButtonTextColor(_XSC_DEFAULT);
	m_DetailInfoButton->SetButtonText("*");//"★"
	InsertChildObject(m_DetailInfoButton);

	memset(m_FMessageString, 0, sizeof(TCHAR)*1024);
	memset(m_SMessageString, 0, sizeof(TCHAR)*1024);
	
	return TRUE;
}

void _XWindow_MatchWindow::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_MatchWindow::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	_XDrawRectAngle(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 
		0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));

	g_XBaseFont->SetColor(_XSC_DEFAULT);
	g_XBaseFont->Puts(m_WindowPosition.x+38, m_WindowPosition.y+28, m_FMessageString);
	g_XBaseFont->Puts(m_WindowPosition.x+38, m_WindowPosition.y+50, m_SMessageString);
	g_XBaseFont->Flush();
}

BOOL _XWindow_MatchWindow::Process(_XGUIObject*& pfocusobject)
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

void _XWindow_MatchWindow::Reset(void)
{
	m_cMatchType		= 0;
	m_usFromID			= 0;

	m_sInnerLevel		= 0;
	m_cGroupIndex		= 0;

	m_cPartyMemberCount = 0;
	m_cRoundCount		= 0;
	m_sAverageLevel		= 0;
	m_sMaxLevel			= 0;

	memset(m_FMessageString, 0, sizeof(TCHAR)*1024);
	memset(m_SMessageString, 0, sizeof(TCHAR)*1024);
}

void _XWindow_MatchWindow::ShowWindow(BOOL show)
{
	if(show)
	{
		TCHAR		cGroup[64];
		TCHAR		cNickName[13];
		TCHAR		cName[13];
		TCHAR		cLevel[64];
		TCHAR		cForm[64];
		TCHAR		cRound[64];

		memset(cGroup, 0, sizeof(TCHAR)*64);
		memset(cNickName, 0, sizeof(TCHAR)*13);
		memset(cName, 0, sizeof(TCHAR)*13);
		memset(cLevel, 0, sizeof(TCHAR)*64);
		memset(cForm, 0, sizeof(TCHAR)*64);
		memset(cRound, 0, sizeof(TCHAR)*64);

		_XUser* pUser = (_XUser*)g_Userlist.FindUser(m_usFromID);
		if(pUser)
		{
			// 문파
			sprintf(cGroup, "%s", g_JoinGroupName[m_cGroupIndex]);
			// 별호
			sprintf(cNickName, "%s", pUser->m_CharacterInfo.m_NickName);
			// 이름
			sprintf(cName, "%s", pUser->m_CharacterInfo.charactername);
			// 레벨
			
			LPTSTR param1 = g_LevelName[m_sInnerLevel/12];
			int    param2 = m_sInnerLevel%12;
			strcpy( cLevel, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MATCH_2190, param1, &param2 ) );

			// 비무형식
			switch(m_cMatchType)
			{
			case 0 :
				// 1:1
				sprintf(cForm, "%s", "[1:1]");
				break;
			case 1 :
				sprintf(cForm, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_ETYPE1));	// "[1:동행]"
				break;
			case 2 :
				sprintf(cForm, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_ETYPE2));	// "[동행:1]"
				break;
			case 3 :
				sprintf(cForm, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_ETYPE3));	// "[집단][전멸전]"
				break;
			case 4 :
				sprintf(cForm, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_ETYPE4));	// "[집단][단승식]"
				break;
			case 5 :
				sprintf(cForm, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_ETYPE5));	// "[집단][연승식]"
				break;
			}

			// "%s의      %s %s 님께서"
			strcpy( m_FMessageString, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_WHO, cGroup, cNickName, cName ) );			

			if(m_cRoundCount > 0)
			{
				sprintf(cRound, _XGETINTERFACETEXT(ID_STRING_MATCH_ROUND), m_cRoundCount);	// "[%d판]"

				// "%s%s 형태의 비무를 신청하셨습니다."
				strcpy( m_SMessageString, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_APPLICATIONMATCHTYPE1, cForm, cRound ) );
			}
			else
				sprintf(m_SMessageString, _XGETINTERFACETEXT(ID_STRING_MATCH_APPLICATIONMATCHTYPE2), cForm);	// "%s 형태의 비무를 신청하셨습니다."
		}
		else
		{
			sprintf(m_FMessageString, _XGETINTERFACETEXT(ID_STRING_MATCH_ERROR));	// "오류 - 상대가 없습니다."
		}
	}
	
	_XWindow::ShowWindow(show);
}

void _XWindow_MatchWindow::Test(void)
{
	m_cGroupIndex = 1;
	m_cMatchType = 5;
	m_sInnerLevel = 13;
	
	m_cRoundCount = 5;

	if(this->GetShowStatus())
		this->ShowWindow(FALSE);
	else
		this->ShowWindow(TRUE);
}
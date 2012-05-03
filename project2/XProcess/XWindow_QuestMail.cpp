// XWindow_QuestMail.cpp: implementation of the _XWindow_QuestMail class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_QuestMail.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_QuestMail::_XWindow_QuestMail()
{
	m_SkinMode				=	0;
	m_QuestID				=	-1;
	m_QuestMailMode			=	_XQUESTMAILMODE_OK;
	m_bStartQuest			=   FALSE;
	m_listboxQuestMessage	=	NULL;		// 풍운방 내용을 표시하는 리스트 박스 
	
	m_btnQuestMailOK		=	NULL;		// 풍운방 확인 버튼 
	m_btnQuestMailYES		=	NULL;		// 풍운방 수락 버튼 
	m_btnQuestMailNO		=	NULL;		// 풍운방 거부 버튼 

}

_XWindow_QuestMail::~_XWindow_QuestMail()
{

}

_XWindow_QuestMail* _XWindow_QuestMail::CreateQuestMail( _XWindowManager* pWindowManager, int questid, int skinmode )
{
	// 먼저 현재 떠있는 풍운방이 같은 퀘스트면 넘어간다.
	for( int windowid = 60051; windowid < 60056; windowid++ )
	{
		_XWindow_QuestMail* pQuestMail_Window = (_XWindow_QuestMail*)g_MainWindowManager.FindWindow( windowid );
		if( pQuestMail_Window )
		{
			if( pQuestMail_Window->m_QuestID == questid )
				return NULL;

		}
	}

	// 같은 퀘스트의 풍운방이 없으면 풍운방을 만든다.
	for( windowid = 60051; windowid < 60056; windowid++ )
	{
		if( !pWindowManager->FindWindow(windowid) )
		{			
			_XWindow_QuestMail* pNewQuestMail = new _XWindow_QuestMail;
			pNewQuestMail->Create( pWindowManager, (DWORD)windowid, skinmode );
			pNewQuestMail->m_QuestID = questid;
			pNewQuestMail->m_SkinMode = skinmode;
			
			return pNewQuestMail;
		}

	}
	return NULL;
}

void _XWindow_QuestMail::Create( _XWindowManager* pWindowManager, DWORD windowid, int skinmode )
{
	if( skinmode == 0 )
	{
		m_WindowID = windowid;
		_XWINDOW_STRUCTURE windowstruct = 
		{			
			TRUE, { 606,5 }, 
			{ 262, 252 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ),
			windowid
		};
		
#ifdef _ACCLAIM_RUBICONADSYSTEM
		if( g_ViewRubiconAD )
		{
			windowstruct.position.y = g_RubiconADWindowSize.cy + 5;
		}		
#endif


		_XWindow::Create( windowstruct );
		
		int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
		int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	
		// 풍운방 바탕 이미지 ( 262 X 252 )
		_XImageStatic* pQuestMailBorder = new _XImageStatic;
		pQuestMailBorder->Create( 0, 0, 262, 252, &g_MainInterfaceTextureArchive, npcresourceindex1 );
		pQuestMailBorder->SetClipRect(0, 0, 425, 410);
		pQuestMailBorder->SetScale( 0.61647f, 0.61647f );
		pQuestMailBorder->ShowWindow(TRUE);
		InsertChildObject( pQuestMailBorder );
		
		// 타이틀바 바탕 이미지 ( 221 X 23 )
		_XImageStatic* pTitleBar = new _XImageStatic;
		pTitleBar->Create( 22, 13, 401, 50, &g_MainInterfaceTextureArchive, npcresourceindex1 );
		pTitleBar->SetClipRect(4, 461, 379, 486);
		pTitleBar->SetScale( 0.61647f, 1.0f );
		pTitleBar->ShowWindow(TRUE);
		InsertChildObject( pTitleBar );
		
		// NPC 대화를 표시할 때 사용될 리스트 박스
		_XLISTBOX_STRUCTURE npcdialogstructure = 
		{
			TRUE,											// 윈도우 활성화
			{ 30, 50 },		// 윈도우 좌표
			{ 205, 162 },	// 윈도우 사이즈
			(((DWORD)windowid<<16)|_XQUESTMAILBUTTON_LISTBOXMESSAGE),	// 윈도우 command 아이디
			-1,												// 윈도우 border image
			&g_MainInterfaceTextureArchive,					// Texture list
			_T(" "),										// 윈도우 타이틀
			18,												// 리스트 아이템 라인 피치
			9,												// 리스트 박스에 보이는 최대 라인수
			_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
			
			// scroll bar property
			{ 12, 162 },									// 스크롤바 사이즈
			12,												// 스크롤바 상하 버튼 사이즈
			22,												// 스크롤바 트랙바 사이즈
			150,											// 전체 리스트 갯수
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
		};
		m_listboxQuestMessage = new _XListBox;
		m_listboxQuestMessage->Create(npcdialogstructure);
		
		_XVScrollBar* pScrollBar = m_listboxQuestMessage->GetScrollBarObject();
		if( pScrollBar )
		{
			_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
			{
				{ 0,  0, 12, 12 },
				{ 24, 0, 36, 12 },
				{ 0,  12, 12, 24 },
				{ 24, 12, 36, 24 },
				{ 40,157, 52, 179}
			};
			pScrollBar->SetClipRect( cliprectstructure );
			pScrollBar->SetViewNullBorder(FALSE);
		}
		
		POINT columnoffset1 = { 0, 0 };
		m_listboxQuestMessage->SetColumnOffset(0, columnoffset1);			// column offset
		m_listboxQuestMessage->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);	// column style (아이콘)
		m_listboxQuestMessage->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
		m_listboxQuestMessage->SetColumnWidth(0, 338);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
		
		m_listboxQuestMessage->EnableSelectItem(FALSE);
		m_listboxQuestMessage->ShowWindow(TRUE);
		InsertChildObject(m_listboxQuestMessage);
			
		// 풍운방 확인 버튼
		_XBTN_STRUCTURE okbtnstruct = 
		{
			TRUE, {88, 215}, {91, 27}, (((DWORD)windowid<<16)|_XQUESTMAILBUTTON_OK),
				npcresourceindex1, npcresourceindex1, npcresourceindex1,
				&g_MainInterfaceTextureArchive
		};
		m_btnQuestMailOK = new _XButton;
		m_btnQuestMailOK->Create(okbtnstruct);
		m_btnQuestMailOK->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
		m_btnQuestMailOK->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
		m_btnQuestMailOK->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
		m_btnQuestMailOK->SetButtonTextColor( _XSC_DEFAULT );
		m_btnQuestMailOK->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK );
		m_btnQuestMailOK->ShowWindow(TRUE);
		InsertChildObject(m_btnQuestMailOK);

		// 풍운방 수락 버튼
		_XBTN_STRUCTURE yesbtnstruct = 
		{
			TRUE, {35, 215}, {91, 27}, (((DWORD)windowid<<16)|_XQUESTMAILBUTTON_YES),
				npcresourceindex1, npcresourceindex1, npcresourceindex1,
				&g_MainInterfaceTextureArchive
		};
		m_btnQuestMailYES = new _XButton;
		m_btnQuestMailYES->Create(yesbtnstruct);
		m_btnQuestMailYES->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
		m_btnQuestMailYES->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
		m_btnQuestMailYES->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
		m_btnQuestMailYES->SetButtonTextColor( _XSC_DEFAULT );
		m_btnQuestMailYES->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_ACCEPT );
		m_btnQuestMailYES->ShowWindow(FALSE);
		InsertChildObject(m_btnQuestMailYES);
		
		// 풍운방 확인 버튼
		_XBTN_STRUCTURE nobtnstruct = 
		{
			TRUE, {150, 215}, {91, 27}, (((DWORD)windowid<<16)|_XQUESTMAILBUTTON_NO),
				npcresourceindex1, npcresourceindex1, npcresourceindex1,
				&g_MainInterfaceTextureArchive
		};
		m_btnQuestMailNO = new _XButton;
		m_btnQuestMailNO->Create(nobtnstruct);
		m_btnQuestMailNO->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
		m_btnQuestMailNO->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
		m_btnQuestMailNO->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
		m_btnQuestMailNO->SetButtonTextColor( _XSC_DEFAULT );
		m_btnQuestMailNO->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_QUEST_2294);
		m_btnQuestMailNO->ShowWindow(FALSE);
		InsertChildObject(m_btnQuestMailNO);
		
	}
	else if( skinmode == 1 )
	{
		m_WindowID = windowid;
		_XWINDOW_STRUCTURE windowstruct = 
		{
			TRUE, { 312, 100 }, 
			{ 400, 300 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 1, 0, 0, 0 ),
			windowid
		};
		
		_XWindow::Create( windowstruct );
		
		int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
		int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	
		// 풍운방 바탕 이미지 ( 400 X 300 )
		_XImageStatic* pQuestMailBorder = new _XImageStatic;
		pQuestMailBorder->Create( 0, 0, 400, 300, &g_MainInterfaceTextureArchive, npcresourceindex1 );
		pQuestMailBorder->SetClipRect(0, 0, 425, 410);
		pQuestMailBorder->SetScale( 0.94117f, 0.73170f );
		pQuestMailBorder->ShowWindow(TRUE);
		InsertChildObject( pQuestMailBorder );
		
		// 타이틀바 바탕 이미지 ( 359 X 23 )
		_XImageStatic* pTitleBar = new _XImageStatic;
		pTitleBar->Create( 22, 13, 381, 50, &g_MainInterfaceTextureArchive, npcresourceindex1 );
		pTitleBar->SetClipRect(4, 461, 379, 486);
		pTitleBar->SetScale( 0.95733f, 1.0f );
		pTitleBar->ShowWindow(TRUE);
		InsertChildObject( pTitleBar );
		
		// NPC 대화를 표시할 때 사용될 리스트 박스
		_XLISTBOX_STRUCTURE npcdialogstructure = 
		{
			TRUE,											// 윈도우 활성화
			{ 30, 50 },		// 윈도우 좌표
			{ 345, 198 },	// 윈도우 사이즈
			(((DWORD)windowid<<16)|_XQUESTMAILBUTTON_LISTBOXMESSAGE),	// 윈도우 command 아이디
			-1,												// 윈도우 border image
			&g_MainInterfaceTextureArchive,					// Texture list
			_T(" "),										// 윈도우 타이틀
			18,												// 리스트 아이템 라인 피치
			11,												// 리스트 박스에 보이는 최대 라인수
			_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
			
			// scroll bar property
			{ 12, 198 },									// 스크롤바 사이즈
			12,												// 스크롤바 상하 버튼 사이즈
			22,												// 스크롤바 트랙바 사이즈
			150,											// 전체 리스트 갯수
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
		};
		m_listboxQuestMessage = new _XListBox;
		m_listboxQuestMessage->Create(npcdialogstructure);
		
		_XVScrollBar* pScrollBar = m_listboxQuestMessage->GetScrollBarObject();
		if( pScrollBar )
		{
			_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
			{
				{ 0,  0, 12, 12 },
				{ 24, 0, 36, 12 },
				{ 0,  12, 12, 24 },
				{ 24, 12, 36, 24 },
				{ 40,157, 52, 179}
			};
			pScrollBar->SetClipRect( cliprectstructure );
			pScrollBar->SetViewNullBorder(FALSE);
		}
		
		POINT columnoffset1 = { 0, 0 };
		m_listboxQuestMessage->SetColumnOffset(0, columnoffset1);			// column offset
		m_listboxQuestMessage->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);	// column style (아이콘)
		m_listboxQuestMessage->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
		m_listboxQuestMessage->SetColumnWidth(0, 338);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
		
		m_listboxQuestMessage->EnableSelectItem(FALSE);
		m_listboxQuestMessage->ShowWindow(TRUE);
		InsertChildObject(m_listboxQuestMessage);
			
		// 풍운방 확인 버튼
		_XBTN_STRUCTURE okbtnstruct = 
		{
			TRUE, {155, 250}, {91, 27}, (((DWORD)windowid<<16)|_XQUESTMAILBUTTON_OK),
				npcresourceindex1, npcresourceindex1, npcresourceindex1,
				&g_MainInterfaceTextureArchive
		};
		m_btnQuestMailOK = new _XButton;
		m_btnQuestMailOK->Create(okbtnstruct);
		m_btnQuestMailOK->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
		m_btnQuestMailOK->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
		m_btnQuestMailOK->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
		m_btnQuestMailOK->SetButtonTextColor( _XSC_DEFAULT );
		m_btnQuestMailOK->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK );
		m_btnQuestMailOK->ShowWindow(TRUE);
		InsertChildObject(m_btnQuestMailOK);

		// 풍운방 수락 버튼
		_XBTN_STRUCTURE yesbtnstruct = 
		{
			TRUE, {84, 250}, {91, 27}, (((DWORD)windowid<<16)|_XQUESTMAILBUTTON_YES),
				npcresourceindex1, npcresourceindex1, npcresourceindex1,
				&g_MainInterfaceTextureArchive
		};
		m_btnQuestMailYES = new _XButton;
		m_btnQuestMailYES->Create(yesbtnstruct);
		m_btnQuestMailYES->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
		m_btnQuestMailYES->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
		m_btnQuestMailYES->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
		m_btnQuestMailYES->SetButtonTextColor( _XSC_DEFAULT );
		m_btnQuestMailYES->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_ACCEPT );
		m_btnQuestMailYES->ShowWindow(FALSE);
		InsertChildObject(m_btnQuestMailYES);
		
		// 풍운방 확인 버튼
		_XBTN_STRUCTURE nobtnstruct = 
		{
			TRUE, {225, 250}, {91, 27}, (((DWORD)windowid<<16)|_XQUESTMAILBUTTON_NO),
				npcresourceindex1, npcresourceindex1, npcresourceindex1,
				&g_MainInterfaceTextureArchive
		};
		m_btnQuestMailNO = new _XButton;
		m_btnQuestMailNO->Create(nobtnstruct);
		m_btnQuestMailNO->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
		m_btnQuestMailNO->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
		m_btnQuestMailNO->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
		m_btnQuestMailNO->SetButtonTextColor( _XSC_DEFAULT );
		m_btnQuestMailNO->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_QUEST_2294);
		m_btnQuestMailNO->ShowWindow(FALSE);
		InsertChildObject(m_btnQuestMailNO);
	
	}

	
	if( pWindowManager )
		pWindowManager->InsertWindow( this );
}

void _XWindow_QuestMail::DestroyWindow()
{
	_XWindow::DestroyWindow();	
}

void _XWindow_QuestMail::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	
	g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
	g_XBaseFont->SetBoldMode( TRUE );
	if(m_SkinMode == 0 )
	{
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 104, m_WindowPosition.y + 18, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_QUEST_2295) );
	}
	else
	{
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 200, m_WindowPosition.y + 18, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3053) );
	}
	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode( FALSE );
}


BOOL _XWindow_QuestMail::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_QuestMail::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow( X, Y);
	
}
BOOL _XWindow_QuestMail::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}


void _XWindow_QuestMail::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow(show);
}

void _XWindow_QuestMail::SetMode( _XQUESTMAIL_MODE questmailmode )
{
	switch(questmailmode) {
	case _XQUESTMAILMODE_OK:
		{
			m_listboxQuestMessage->ShowWindow(TRUE);	// 풍운방 내용을 표시하는 리스트 박스 
			
			m_btnQuestMailOK->ShowWindow(TRUE);			// 풍운방 확인 버튼 
			m_btnQuestMailYES->ShowWindow(FALSE);		// 풍운방 수락 버튼 
			m_btnQuestMailNO->ShowWindow(FALSE);		// 풍운방 거부 버튼 
		}
		break;
	case _XQUESTMAILMODE_YESNO:
		{
			m_listboxQuestMessage->ShowWindow(TRUE);	// 풍운방 내용을 표시하는 리스트 박스 
			
			m_btnQuestMailOK->ShowWindow(FALSE);		// 풍운방 확인 버튼 

			m_btnQuestMailYES->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_ACCEPT ); //수락
			m_btnQuestMailNO->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_QUEST_2294); //거부

			m_btnQuestMailYES->ShowWindow(TRUE);		// 풍운방 수락 버튼 
			m_btnQuestMailNO->ShowWindow(TRUE);			// 풍운방 거부 버튼 
		}
		break;
	case _XQUESTMAILMODE_TUTORIALONLY :
		{
			m_listboxQuestMessage->ShowWindow(TRUE);	// 풍운방 내용을 표시하는 리스트 박스 
			
			m_btnQuestMailOK->ShowWindow(FALSE);		// 풍운방 확인 버튼 

			m_btnQuestMailYES->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_YES ); //예
			m_btnQuestMailNO->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSAGEBOX_NO ); //아니오

			m_btnQuestMailYES->ShowWindow(TRUE);		// 풍운방 수락 버튼 
			m_btnQuestMailNO->ShowWindow(TRUE);			// 풍운방 거부 버튼 
		}
		break;
	}

}

void _XWindow_QuestMail::SetMessage( LPSTR messagestring )
{
	if( !messagestring ) return;
	if( strlen(messagestring) <= 0 ) return;
	
	m_listboxQuestMessage->DeleteAllItem();
	
	_XStringSplitInfo questmailinfo;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SplitString(messagestring, 200, questmailinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
	}
	else
	{
		g_XBaseFont->SplitStringByCR(messagestring, questmailinfo);
	}

	int		linecount = 0;
	TCHAR	buffer[2048];
	
	for(int i = 0 ; i < questmailinfo.splitcount ; i++)
	{
		memset(buffer, 0, sizeof(buffer));
		strncpy(buffer, questmailinfo.splitpoint[i], questmailinfo.splitlength[i]);
		m_listboxQuestMessage->InsertListItem(_T(""), 0, 0);
		m_listboxQuestMessage->SetItemText(linecount, 0, buffer);
		m_listboxQuestMessage->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 0, 0, 0));
		
		linecount++;
	}	
	m_listboxQuestMessage->SetLineScrollPos(0);
	
}
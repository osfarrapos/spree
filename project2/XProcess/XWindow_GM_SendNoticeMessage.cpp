// XWindow_GM_SendNoticeMessage.cpp: implementation of the _XWindow_GM_SendNoticeMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_GM_SendNoticeMessage.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_GMNoticeTimerSettingTabKey( _XIMEContainer* pIMEContainer )
{
	_XWindow_GM_SendNoticeMessage* pWindow = (_XWindow_GM_SendNoticeMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW);
	if( pWindow )
	{
		if( pIMEContainer == &pWindow->m_HourIMEControl )
		{
			pWindow->m_MinuteControl.SetFocus();
		}
		else if( pIMEContainer == &pWindow->m_MinuteControl )
		{
			pWindow->m_SecondControl.SetFocus();
		}
		else if( pIMEContainer == &pWindow->m_SecondControl )
		{
			pWindow->m_NoticeIMEControl.SetFocus();			
		}
		else if( pIMEContainer == &pWindow->m_NoticeIMEControl )
		{
			pWindow->m_HourIMEControl.SetFocus();			
		}
	}
}

void __stdcall _XIMECallback_GMNoticeTimerSettingEscapeKey( _XIMEContainer* pIMEContainer )
{
	_XWindow_GM_SendNoticeMessage* pWindow = (_XWindow_GM_SendNoticeMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW);
	if( pWindow )
	{
		_XIMEKERNEL.SetFocus( NULL );
	}
}

void __stdcall _XIMECallback_GMNoticeTimerSettingEnterKey( _XIMEContainer* pIMEContainer )
{
	_XWindow_GM_SendNoticeMessage* pWindow = (_XWindow_GM_SendNoticeMessage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMNOTICEMESSAGEWINDOW);
	if( pWindow )
	{
		pWindow->InsertMessage();		
	}
}

_XWindow_GM_SendNoticeMessage::_XWindow_GM_SendNoticeMessage()
{
	m_pListBox = NULL;
}

_XWindow_GM_SendNoticeMessage::~_XWindow_GM_SendNoticeMessage()
{

}


BOOL _XWindow_GM_SendNoticeMessage::Initialize( void )
{
	// Title---------------------------------------------------------------------------------------=
	int titleimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 177, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleLeftBar->SetClipRect( 0, 0, 177, 24 );
	InsertChildObject( pTitleLeftBar );
	
	_XImageStatic* pTitleCentertBar = new _XImageStatic;
	pTitleCentertBar->Create( 176, 0, 177+175, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleCentertBar->SetClipRect( 79, 0, 177, 24 );
	InsertChildObject( pTitleCentertBar );
	
	_XImageStatic* pTitleRightBar = new _XImageStatic; 
	pTitleRightBar->Create( 451, 0, 451+177, 23,
		&g_MainInterfaceTextureArchive, titleimageindex );
	pTitleRightBar->SetClipRect( 0, 0, 177, 24 );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );


	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 430, 5 }, { 12, 12 }, _XDEF_GMNOTICEMESSAGEWINDOWCLOSEBTN,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
	
	
	m_HourIMEControl.ResetContainer();
	m_HourIMEControl.ResizeWindow(16, 12);
	m_HourIMEControl.MoveWindow( m_WindowPosition.x + 8 , m_WindowPosition.y + 30 );	
	m_HourIMEControl.ShowWindow(TRUE);
	m_HourIMEControl.ShowWindowRectAngle( TRUE );
	m_HourIMEControl.SetMaxLength(2);
	m_HourIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );
	m_HourIMEControl.SetHorizScrollMode( FALSE );		
	m_HourIMEControl.SetTabKeyCallback( _XIMECallback_GMNoticeTimerSettingTabKey );	
	m_HourIMEControl.SetEscapeKeyCallback( _XIMECallback_GMNoticeTimerSettingEscapeKey );

	m_MinuteControl.ResetContainer();
	m_MinuteControl.ResizeWindow(16, 12);
	m_MinuteControl.MoveWindow( m_WindowPosition.x + 42 , m_WindowPosition.y + 30 );	
	m_MinuteControl.ShowWindow(TRUE);
	m_MinuteControl.ShowWindowRectAngle( TRUE );
	m_MinuteControl.SetMaxLength(2);
	m_MinuteControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );	
	m_MinuteControl.SetHorizScrollMode( FALSE );		
	m_MinuteControl.SetTabKeyCallback( _XIMECallback_GMNoticeTimerSettingTabKey );	
	m_MinuteControl.SetEscapeKeyCallback( _XIMECallback_GMNoticeTimerSettingEscapeKey );
	
	m_SecondControl.ResetContainer();
	m_SecondControl.ResizeWindow(16, 12);
	m_SecondControl.MoveWindow( m_WindowPosition.x + 76 , m_WindowPosition.y + 30 );	
	m_SecondControl.ShowWindow(TRUE);
	m_SecondControl.ShowWindowRectAngle( TRUE );
	m_SecondControl.SetMaxLength(2);
	m_SecondControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );	
	m_SecondControl.SetHorizScrollMode( FALSE );		
	m_SecondControl.SetTabKeyCallback( _XIMECallback_GMNoticeTimerSettingTabKey );	
	m_SecondControl.SetEscapeKeyCallback( _XIMECallback_GMNoticeTimerSettingEscapeKey );

	m_NoticeIMEControl.ResetContainer();
	m_NoticeIMEControl.ResizeWindow(265, 12);
	m_NoticeIMEControl.MoveWindow( m_WindowPosition.x + 114 , m_WindowPosition.y + 30 );	
	m_NoticeIMEControl.ShowWindow(TRUE);
	m_NoticeIMEControl.ShowWindowRectAngle( TRUE );
	m_NoticeIMEControl.SetMaxLength(80);
	m_NoticeIMEControl.SetAlignType( _XFONT_ALIGNTYPE_LEFT );	
	m_NoticeIMEControl.SetHorizScrollMode( TRUE );
	m_NoticeIMEControl.SetTabKeyCallback( _XIMECallback_GMNoticeTimerSettingTabKey );
	m_NoticeIMEControl.SetEscapeKeyCallback( _XIMECallback_GMNoticeTimerSettingEscapeKey );
	m_NoticeIMEControl.SetReturnKeyCallback( _XIMECallback_GMNoticeTimerSettingEnterKey  );

	int tradeindex	= g_MainInterfaceTextureArchive.FindResource("MI_Trade.tga");
	_XBTN_STRUCTURE sendbutton = 
	{
		TRUE, {m_WindowSize.cx - 64, 27}, {56, 18},
		_XDEF_GMNOTICEMESSAGEINSERTBTN, 
		tradeindex, tradeindex, tradeindex,
		&g_MainInterfaceTextureArchive
	};
	_XButton* btnInsertNewMessage = new _XButton;
	btnInsertNewMessage->Create( sendbutton );	
	btnInsertNewMessage->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	btnInsertNewMessage->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	btnInsertNewMessage->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	btnInsertNewMessage->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	btnInsertNewMessage->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_MAILSEND);	// _T("보내기")
	btnInsertNewMessage->ShowWindow(TRUE);
	InsertChildObject( btnInsertNewMessage );

	_XBTN_STRUCTURE deletebutton = 
	{
		TRUE, {m_WindowSize.cx - 63, m_WindowSize.cy-21}, {56, 18},
			_XDEF_GMNOTICEMESSAGEDELETEBTN, 
			tradeindex, tradeindex, tradeindex,
			&g_MainInterfaceTextureArchive
	};
	_XButton* btnDeleteMessage = new _XButton;
	btnDeleteMessage->Create( deletebutton);	 
	btnDeleteMessage->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	btnDeleteMessage->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	btnDeleteMessage->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	btnDeleteMessage->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	btnDeleteMessage->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_GMCOMMAND_2073);	
	btnDeleteMessage->ShowWindow(TRUE);
	InsertChildObject( btnDeleteMessage );

	_XButton* btnDeleteAllMessage = new _XButton;
	deletebutton.commandid = _XDEF_GMNOTICEMESSAGEDELETEALLBTN;
	deletebutton.position.x = m_WindowSize.cx - 63 - 58;
	btnDeleteAllMessage->Create( deletebutton);	 
	btnDeleteAllMessage->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 189, 78, 245, 96 );
	btnDeleteAllMessage->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 189, 98, 245, 116 );
	btnDeleteAllMessage->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 189, 118, 245, 136 );
	btnDeleteAllMessage->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	btnDeleteAllMessage->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_GMCOMMAND_2074);
	btnDeleteAllMessage->ShowWindow(TRUE);
	InsertChildObject( btnDeleteAllMessage );

	int chatquickimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );

	_XLISTBOX_STRUCTURE listboxstructure = 
	{ 
		TRUE,								// 윈도우 활성화
		{ 8, 46 },							// 윈도우 좌표
		{ m_WindowSize.cx-16, m_WindowSize.cy - 68 },					// 윈도우 사이즈
		_XDEF_GMNOTICEMESSAGEWINDOWMESSAGELIST,	// 윈도우 아이디
		-1,									// 윈도우 border 이미지 인덱스
		&g_MainInterfaceTextureArchive,		// Texture 리스트
		_T(" "),							// 윈도우 타이틀
		14,									// 리스트 아이템 라인 피치
		9,									// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,			// 스크롤바의 위치
		
		// Scroll bar property
		{12, m_WindowSize.cy - 68},
		12,
		22,
		150,
		chatquickimageindex,
		chatquickimageindex,
		chatquickimageindex,
		chatquickimageindex,
		chatquickimageindex
	};
	
	m_pListBox = new _XListBox;
	m_pListBox->Create(listboxstructure);
	
	POINT coloffset = { 2, 2 };
	m_pListBox->SetColumnOffset(0, coloffset);
	m_pListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_pListBox->SetColumnWidth(0, 24 );

	coloffset.x = 28;
	m_pListBox->SetColumnOffset(1, coloffset);
	m_pListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_pListBox->SetColumnWidth(1, 64 );

	coloffset.x = 28 + 54;
	m_pListBox->SetColumnOffset(2, coloffset);
	m_pListBox->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	m_pListBox->SetColumnWidth(2, 300 );	

	m_pListBox->SetTrackHeight(14);
	m_pListBox->SetTrackBarColor( D3DCOLOR_ARGB(64,128,128,128) );
	m_pListBox->SetAlwaysShowScrollBarMode( FALSE );
	m_pListBox->SetSelectedTrackBarColor( D3DCOLOR_ARGB(128,148,128,148) );
	m_pListBox->EnableSelectItem( TRUE );
	
	m_pListBox->SetMaxViewLineCount( (m_WindowSize.cy - 52) / 14 );
	
	_XVScrollBar* pScrollBar = m_pListBox->GetScrollBarObject();
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
		pScrollBar->SetViewNullBorder( TRUE );
	}
	
	InsertChildObject( m_pListBox );
	
	return TRUE;
}

void _XWindow_GM_SendNoticeMessage::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_GM_SendNoticeMessage::Draw( _XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// Draw Border, Line ---------------------------------------------------------------------=	
	_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, D3DCOLOR_ARGB(128, 0, 0, 0) );
	
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+23, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0.0f,0xFF000000 );
	_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+24, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f,0xFF9F9A84 );
	_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+25, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f,0xFF040000 );
	_XDrawRectAngle( m_WindowPosition.x+3, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f,0xFF1A211A );

	_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+46, m_WindowPosition.x+m_WindowSize.cx-9, m_WindowPosition.y+m_WindowSize.cy-22, 0.0f,0xFF000000 );

	_XWindow::Draw(pfocusobject);
		
	// Title Name
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_ADMINTALK) );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->PutsAlign(m_WindowPosition.x+27, m_WindowPosition.y+30 , _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_2075) );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+61, m_WindowPosition.y+30 , _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_2076) );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+95, m_WindowPosition.y+30 , _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_2077) );
	g_XBaseFont->Flush();

	m_HourIMEControl.Draw(pfocusobject);
	m_MinuteControl.Draw(pfocusobject);
	m_SecondControl.Draw(pfocusobject);
	m_NoticeIMEControl.Draw(pfocusobject);
}

BOOL _XWindow_GM_SendNoticeMessage::Process( _XGUIObject*& pfocusobject )
{	
	if( !m_MessageList.listEmpty() )
	{
		m_MessageList.resetList();
		
		int index = 0;
		TCHAR tempbuffer[128];

		do 
		{
			_XStringItem* pMessageString = (_XStringItem*)m_MessageList.currentItem();
			if( !pMessageString )
			{
				break;
			}
			
			if( pMessageString->Get_Attribute( 0 ) > 0 )
			{
				int lefttime = pMessageString->Get_Attribute( 0 ) - (int)g_fElapsedFrameTime;
				
				if( lefttime <= 0 )
				{
					pMessageString->Set_Attribute( 0, 0 );
					sprintf( tempbuffer, "Complete" );
					m_pListBox->SetItemText( index, 1, tempbuffer, TRUE );
					m_pListBox->SetItemAttrib( index, 1, 0xFFFF0000 );
					m_pListBox->SetItemAttribHighlight( index, 1, 0xFFFF0000 );

					g_NetworkKernel.SendChatMessage(pMessageString->Get_String(), _XDEF_CHATMESSAGEMODE_ADMINNOTICE );
				}
				else
				{
					int lefthour = lefttime / (3600000);
					int modminute = (lefttime % (3600000));
					int leftminute = modminute / (60000);
					int leftsecond = (modminute % (60000)) / 1000;
					
					pMessageString->Set_Attribute( lefttime );
					sprintf( tempbuffer, "%d:%d:%d", lefthour, leftminute, leftsecond );
					m_pListBox->SetItemText( index, 1, tempbuffer );
				}
			}
			
			index++;
			m_MessageList.nextItem();
		} while( !m_MessageList.atHeadOfList() );
	}


	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckMousePosition() )
				{
					if( m_HourIMEControl.CheckMousePosition() )
						m_HourIMEControl.SetFocus();
					else if( m_MinuteControl.CheckMousePosition() )
						m_MinuteControl.SetFocus();
					else if( m_SecondControl.CheckMousePosition() )
						m_SecondControl.SetFocus();
					else if( m_NoticeIMEControl.CheckMousePosition() )
						m_NoticeIMEControl.SetFocus();
				}
			}
		}
	}
		
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	

void _XWindow_GM_SendNoticeMessage::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );

	m_HourIMEControl.MoveWindow( m_WindowPosition.x + 8 , m_WindowPosition.y + 30 );	
	m_MinuteControl.MoveWindow( m_WindowPosition.x + 42 , m_WindowPosition.y + 30 );	
	m_SecondControl.MoveWindow( m_WindowPosition.x + 76 , m_WindowPosition.y + 30 );	
	m_NoticeIMEControl.MoveWindow( m_WindowPosition.x + 114 , m_WindowPosition.y + 30 );	
}

void _XWindow_GM_SendNoticeMessage::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow(show);
	if( show )
	{
		m_HourIMEControl.SetFocus();
		m_pMyManager->SetTopWindow( this );
	}
	else
	{
		_XIMEKERNEL.SetFocus( NULL );
	}
}

void _XWindow_GM_SendNoticeMessage::InsertMessage( void )
{
	if( strlen( m_NoticeIMEControl.GetText() ) <= 0 )
	{
		g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_NOMESSAGE), TRUE );
		return;
	}

	int hour = atoi( m_HourIMEControl.GetText() );
	int minute = atoi( m_MinuteControl.GetText() );
	int second = atoi( m_SecondControl.GetText() );

	if( hour <= 0 && minute <= 0 && second <= 0 )
	{
		g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_TIMEERROR), TRUE );
		return;
	}

	TCHAR tempbuffer[128];
	sprintf( tempbuffer, "%d", m_pListBox->GetItemCount()+1 );
	m_pListBox->InsertListItem( tempbuffer, _XSC_DEFAULT );
	m_pListBox->SetItemCommandID( m_pListBox->GetItemCount()-1, 1 );
	
	long lefttime = (second + (minute * 60) + ( hour * 3600 )) * 1000;

	_XStringItem* pNewMessage = new _XStringItem( m_NoticeIMEControl.GetText(), lefttime );
	m_MessageList.resetList();
	m_MessageList.insertItem( pNewMessage );

	m_pListBox->SetItemAttribHighlight( m_pListBox->GetItemCount()-1, 0, _XSC_DEFAULT_HIGHLIGHT );

	sprintf( tempbuffer, "%d:%d:%d", hour, minute, second );
	m_pListBox->SetItemText( m_pListBox->GetItemCount()-1, 1, tempbuffer );
	m_pListBox->SetItemAttrib( m_pListBox->GetItemCount()-1, 1, _XSC_DEFAULT );
	m_pListBox->SetItemAttribHighlight( m_pListBox->GetItemCount()-1, 1, _XSC_DEFAULT_HIGHLIGHT );

	m_pListBox->SetItemText( m_pListBox->GetItemCount()-1, 2, m_NoticeIMEControl.GetText() );	
	m_pListBox->SetItemAttrib( m_pListBox->GetItemCount()-1, 2, _XSC_DEFAULT );
	m_pListBox->SetItemAttribHighlight( m_pListBox->GetItemCount()-1, 2, _XSC_DEFAULT_HIGHLIGHT );

	m_pListBox->SetScrollPos( 9999 );
}

void _XWindow_GM_SendNoticeMessage::DeleteMessage( void )
{
	int selectedindex = m_pListBox->GetSelectedItem();
	if( selectedindex < 0 )
	{
		g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_SELECTDELETEMESSAGE), TRUE );	
		return;		
	}
	
	m_MessageList.deleteItem( selectedindex );
	m_pListBox->DeleteItem( selectedindex );
	
}


void _XWindow_GM_SendNoticeMessage::DeleteAllMessage( void )
{
	m_MessageList.disposeList();
	m_pListBox->DeleteAllItem();
}




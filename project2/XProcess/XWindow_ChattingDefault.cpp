// XWindow_ChattingDefault.cpp: implementation of the _XWindow_ChattingDefault class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_SystemMsgDefault.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_ChattingDefault.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#ifdef _XDEF_CASHITEM_SHOUT
D3DCOLOR g_ChattingStringColor[16] = 
{
		D3DCOLOR_ARGB( 255, 180, 180, 180 ), // Normal	217, 227, 229
		D3DCOLOR_ARGB( 255, 255, 255, 255 ), // My chat message  192, 192, 255
		D3DCOLOR_ARGB( 255, 243, 162, 241 ), // Whisper 
		D3DCOLOR_ARGB( 255, 0,	 0,   0   ), // Whole
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		D3DCOLOR_ARGB( 255, 0, 255, 0 ),	 // User clan
#else
		D3DCOLOR_ARGB( 255, 191, 236, 125 ), // User clan
#endif

		D3DCOLOR_ARGB( 255, 241, 223, 72  ), // Party
		D3DCOLOR_ARGB( 255, 64,  255, 64  ), // Global notice
		D3DCOLOR_ARGB( 255, 255, 64,  64  ), // Admin notice
		D3DCOLOR_ARGB( 255, 255, 240,  0 ),  // Item
		//D3DCOLOR_ARGB( 255, 0,   255, 234 ), // Quest
		D3DCOLOR_ARGB( 255, 226, 201, 170 ), // ZoneChatting Text 
		D3DCOLOR_ARGB( 255, 191, 236, 125 ), // Local message		
		D3DCOLOR_ARGB( 255, 254, 132, 33  ), // Sysetem Message general
		D3DCOLOR_ARGB( 255, 255, 0,   0   ), // Sysetem Message loss
		D3DCOLOR_ARGB( 255, 255, 196, 54  ), // Sysetem Message pick up
		D3DCOLOR_ARGB( 255, 255, 255, 255 ), // GM Block
		D3DCOLOR_ARGB( 255, 3, 254, 255 )	 // Shout Text 			
};
//#else
//D3DCOLOR g_ChattingStringColor[16] = 
//{
//		D3DCOLOR_ARGB( 255, 180, 180, 180 ), // Normal	217, 227, 229
//		D3DCOLOR_ARGB( 255, 255, 255, 255 ), // My chat message  192, 192, 255
//		D3DCOLOR_ARGB( 255, 243, 162, 241 ), // Whisper 
//		D3DCOLOR_ARGB( 255, 0,	 0,   0   ), // Whole
//		D3DCOLOR_ARGB( 255, 191, 236, 125 ), // User clan
//		D3DCOLOR_ARGB( 255, 241, 223, 72  ), // Party
//		D3DCOLOR_ARGB( 255, 64,  255, 64  ), // Global notice
//		D3DCOLOR_ARGB( 255, 255, 64,  64  ), // Admin notice
//		D3DCOLOR_ARGB( 255, 255, 240, 0   ), // Item
//		D3DCOLOR_ARGB( 255, 0,   255, 234 ), // Quest
//		D3DCOLOR_ARGB( 255, 255, 110, 250 ), // Local message		
//		D3DCOLOR_ARGB( 255, 254, 132, 33  ), // Sysetem Message general
//		D3DCOLOR_ARGB( 255, 255, 0,   0   ), // Sysetem Message loss
//		D3DCOLOR_ARGB( 255, 255, 196, 54  ), // Sysetem Message pick up
//		D3DCOLOR_ARGB( 255, 255, 255, 255 ), // GM Block
//		D3DCOLOR_ARGB( 255, 251, 214, 187 ), // Emotion Text 		
//};
//#endif

//#ifdef _XDEF_CASHITEM_SHOUT
D3DCOLOR g_ChatFunctionButtonStringColor[_XCHATMODE_TOTALCOUNT] = 
{
		0xFFFC832A,
		0xFF03FEFF,
		0xFF8BC7F6,
		0xFFBCEB7F,
		0xFFF1Df48,
		0xFFF3A2F0,
		0xFFE2C9AA,
		0xFFFFFFFF
};
//#else
//D3DCOLOR g_ChatFunctionButtonStringColor[6] = 
//{
//		0xFFFC832A,
//		0xFF8BC7F6,
//		0xFFBCEB7F,
//		0xFFF1Df48,
//		0xFFF3A2F0,
//		0xFFFFFFFF
//};
//#endif

void __stdcall _XIMECallback_ChatWhisperReturn( _XIMEContainer* pIMEContainer )
{
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	if( !pChatDefault_Window->GetShowStatus() ) return;

	TCHAR tbuf[100];
	memset( tbuf, 0, sizeof(TCHAR) * 100 );
	strcpy( tbuf, pChatDefault_Window->m_WhisperEditContainer.GetText());
	
	if( strlen( tbuf ) )
	{		
#ifdef _XDEF_NEWSTRINGMANAGER
		pChatDefault_Window->m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[5], _XFONT_ALIGNTYPE_LEFT, 6 );		
#else
		pChatDefault_Window->m_btnChatFunction->SetButtonText( g_ChatFunctionButtonString[4] );
#endif
		pChatDefault_Window->m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[5] );
		pChatDefault_Window->SetChatMode(_XCHATMODE_WHISPER);
		strcpy( pChatDefault_Window->m_WhisperTargetName, tbuf );
		pChatDefault_Window->InsertRecentWhisperUserList( tbuf, pChatDefault_Window->m_SelectedWhisperTarget );	
	}
	else
	{
		if( strlen(pChatDefault_Window->m_SavedWhisperTargetName) )
		{
			strcpy( pChatDefault_Window->m_WhisperTargetName, pChatDefault_Window->m_SavedWhisperTargetName );
			pChatDefault_Window->m_WhisperTargetButton[pChatDefault_Window->m_SelectedWhisperTarget].SetButtonText(pChatDefault_Window->m_SavedWhisperTargetName);
		}		
		memset( pChatDefault_Window->m_SavedWhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
	}
	
	_XIMEKERNEL.ResetCombinationBuffer();
	pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();
	pChatDefault_Window->m_WhisperEditContainer.ShowWindow( FALSE );
	
	eodapp->m_proc_maingame.m_ChatEditContainer.ClearBuffer();	
	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();
	pChatDefault_Window->SetChangeWhisperName( pChatDefault_Window->m_SelectedWhisperTarget );
}

void __stdcall _XIMECallback_ChatWhisperLeftArrow( _XIMEContainer* pIMEContainer )
{
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	
	if( pChatDefault_Window->m_ChatMode == _XCHATMODE_WHISPER )
	{
		if( !pChatDefault_Window->m_WhisperEditContainer.GetFocus() )
		{
			int tmp = pChatDefault_Window->m_SelectedWhisperTarget-1;
			if(  tmp < 0 )
				tmp = 2;
			
			pChatDefault_Window->m_WhisperTargetButton[0].SetCheck( FALSE );
			pChatDefault_Window->m_WhisperTargetButton[1].SetCheck( FALSE );
			pChatDefault_Window->m_WhisperTargetButton[2].SetCheck( FALSE );
			pChatDefault_Window->m_WhisperTargetButton[tmp].SetCheck( TRUE );
			
			pChatDefault_Window->m_SelectedWhisperTarget = tmp;
			
			if( pChatDefault_Window->m_WhisperTargetButton[tmp].GetButtonText() == NULL )
			{
				POINT tmpPos = pChatDefault_Window->GetWindowPos();
				SIZE tmpSize = pChatDefault_Window->GetWindowSize();
				memset( pChatDefault_Window->m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
				pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();				
				pChatDefault_Window->m_WhisperEditContainer.MoveWindow( tmpPos.x+95+(tmp*93), tmpPos.y + tmpSize.cy - 34 );
				pChatDefault_Window->m_WhisperEditContainer.ShowWindow( TRUE );
				pChatDefault_Window->m_WhisperEditContainer.SetFocus();
			}
			else
			{
				CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
				_XIMEKERNEL.ResetCombinationBuffer();
				pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();
				pChatDefault_Window->m_WhisperEditContainer.ShowWindow( FALSE );
				
				eodapp->m_proc_maingame.m_ChatEditContainer.ClearBuffer();	
				eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();
			}
		}
	}
}

void __stdcall _XIMECallback_ChatWhisperRightArrow( _XIMEContainer* pIMEContainer )
{
	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );
	
	if( pChatDefault_Window->m_ChatMode == _XCHATMODE_WHISPER )
	{
		if( !pChatDefault_Window->m_WhisperEditContainer.GetFocus() )
		{
			int tmp = pChatDefault_Window->m_SelectedWhisperTarget+1;
			if(  tmp >= _XDEF_MAXWHISPERTARGETCOUNT )	
				tmp = 0;
			
			pChatDefault_Window->m_WhisperTargetButton[0].SetCheck( FALSE );
			pChatDefault_Window->m_WhisperTargetButton[1].SetCheck( FALSE );
			pChatDefault_Window->m_WhisperTargetButton[2].SetCheck( FALSE );
			pChatDefault_Window->m_WhisperTargetButton[tmp].SetCheck( TRUE );
			
			pChatDefault_Window->m_SelectedWhisperTarget = tmp;
			
			if( pChatDefault_Window->m_WhisperTargetButton[tmp].GetButtonText() == NULL )
			{
				POINT tmpPos = pChatDefault_Window->GetWindowPos();
				SIZE tmpSize = pChatDefault_Window->GetWindowSize();
				memset( pChatDefault_Window->m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
				pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();				
				pChatDefault_Window->m_WhisperEditContainer.MoveWindow( tmpPos.x+95+(tmp*93), tmpPos.y + tmpSize.cy - 34 );
				pChatDefault_Window->m_WhisperEditContainer.ShowWindow( TRUE );
				pChatDefault_Window->m_WhisperEditContainer.SetFocus();
			}
			else
			{
				CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
				_XIMEKERNEL.ResetCombinationBuffer();
				pChatDefault_Window->m_WhisperEditContainer.ClearBuffer();
				pChatDefault_Window->m_WhisperEditContainer.ShowWindow( FALSE );
				
				eodapp->m_proc_maingame.m_ChatEditContainer.ClearBuffer();	
				eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();
			}
		}
	}
}

void __stdcall _XCallback_CD_CheckSM(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;//아이템몰이 있는 상태에서는 툴팁을 그리지 않는다.
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_CLOSEMSG), _XSC_INFORMATION );		// _T("시스템 메세지 열기/닫기")
}

void __stdcall _XChattingDefaultCallback_ChatResize(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;//아이템몰이 있는 상태에서는 툴팁을 그리지 않는다.
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_2007), _XSC_INFORMATION );
}

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

void __stdcall _XCallback_LockChatFilterMenuButton(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;

	_XWindow_ChattingDefault* pChatDefault_Window = (_XWindow_ChattingDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_CHATTINGWINDOWDEFAULT );

	if( pChatDefault_Window && pChatDefault_Window->m_pLockChatFilterMenuButton )
	{
		if( pChatDefault_Window->m_pLockChatFilterMenuButton->GetCheck() )
			g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3470), _XSC_INFORMATION );
		else
			g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3471), _XSC_INFORMATION );
	}
}

void __stdcall _XCallback_BlackListButton(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3472), _XSC_INFORMATION );
}

void __stdcall _XCallback_ChatFilterButton1(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3466), _XSC_INFORMATION );
}

void __stdcall _XCallback_ChatFilterButton2(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3467), _XSC_INFORMATION );
}

void __stdcall _XCallback_ChatFilterButton3(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3468), _XSC_INFORMATION );
}

void __stdcall _XCallback_ChatFilterButton4(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3469), _XSC_INFORMATION );
}

#endif

#define _XDEF_CHATWINDOWMINSIZE_WIDTH		374
#define _XDEF_CHATWINDOWMINSIZE_HEIGHT		214

#define _XDEF_CHATWINDOWMAXSIZE_WIDTH		374
#define _XDEF_CHATWINDOWMAXSIZE_HEIGHT		574

_XWindow_ChattingDefault::_XWindow_ChattingDefault()
{

	m_Resizing				= FALSE;
	m_ResizeMode			= _XWRESIZE_NONE;
	
	m_ChatListScrollBar		= NULL;
	m_pScrbarFocusedObject	= NULL;
	m_ChatListScrollPos     = 0;

	m_MaxViewCount			= 0;
	m_ChatMode				= _XCHATMODE_NORMAL;
	m_PrevChatMode			= _XCHATMODE_NORMAL;
	m_CurrentSeletedEmotion	= 0;
	memset( m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
	memset( m_SavedWhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );

	m_btnChatResize			= NULL;	
	m_btnMaximumMapModeExit	= NULL;
	
	m_MovingChatWindow		= TRUE;
	m_ChatBorderLeft		= NULL;
	m_ChatBorderRight		= NULL;


	m_btnChatFunction		= NULL;	
	m_btnimageChatFunction	= NULL;

	m_WhisperStaticButton	= NULL;
	m_SelectedWhisperTarget = 0;	

	m_DrawBorder			= FALSE;	

//#ifdef _XDEF_CASHITEM_SHOUT
	for( int i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
//#else
//	for( int i = 0; i < 6; i++ )
//#endif
	{
		m_btnChatFunctionMenuButton[i] = NULL;
		m_btnimageChatFunctionMenuButton[i] = NULL;
	}	

	m_EmotionListBox = NULL;

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

	m_LockChatFilterMenu = TRUE;
	m_pLockChatFilterMenuButton = NULL;

	m_HeroBandChatMode	= FALSE;
#endif
}

_XWindow_ChattingDefault::~_XWindow_ChattingDefault()	
{
	SAFE_DELETE( m_ChatListScrollBar );
	DestroyWindow();
}

void _XWindow_ChattingDefault::ResetChattingDefault(void)
{
	m_MovingChatWindow		= TRUE;
	/*m_ChatMode				= _XCHATMODE_NORMAL;	
	m_SelectedWhisperTarget = 0;	

	memset( m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
	memset( m_SavedWhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );

	m_WhisperTargetButton[0].SetButtonText( _T("") );
	m_WhisperTargetButton[1].SetButtonText( _T("") );
	m_WhisperTargetButton[2].SetButtonText( _T("") );*/
}

BOOL _XWindow_ChattingDefault::Initialize( void )
{	
	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	int chattingbuttonindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back02.tga" ); //Author : 양희왕 //breif : 시스템메시지, 블랙리스트 버튼 

	SAFE_DELETE( m_ChatListScrollBar );
	m_ChatListScrollBar = new _XVScrollBar;
	_XSCRBAR_STRUCTURE chatlistscrbarstructure = 
	{
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
			TRUE, {m_WindowPosition.x,m_WindowPosition.y + 1 }, 
			12, m_WindowSize.cy - 46, // width, height
#else
			TRUE, {m_WindowPosition.x+1,m_WindowPosition.y + 1 }, 
			12, m_WindowSize.cy - 38, // width, height
#endif
			12,		 // btnysize
			22,		 // trackbar size
			_XDEF_CHATLIST_QUESIZE, m_MaxViewCount, 0,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			chattingbuttonindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_ChatListScrollBar->Create( chatlistscrbarstructure );
	m_ChatListScrollBar->ShowWindow( TRUE );
	m_ChatListScrollBar->SetViewBorder( TRUE );
	m_ChatListScrollBar->SetViewNullBorder( FALSE );
	m_ChatListScrollBar->SetTotalLineCount(0);	
	m_ChatListScrollBar->UpdateData();
	
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	m_MaxViewCount = ((m_WindowSize.cy-46) / 15);
#else
	m_MaxViewCount = ((m_WindowSize.cy-34) / 15);
#endif
	m_ChatListScrollBar->SetViewLineCount( m_MaxViewCount );

	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_ChatListScrollBar->SetClipRect( cliprectstructure );
	
	m_ChatBorderLeft = new _XImageStatic;
	m_ChatBorderLeft->Create( m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy - 22, 256, 21, &g_MainInterfaceTextureArchive, chattingbuttonindex );
	m_ChatBorderLeft->SetClipRect( 0, 54, 256, 75  );
	InsertChildObject( m_ChatBorderLeft );
	
	m_ChatBorderRight = new _XImageStatic;
	m_ChatBorderRight->Create( m_WindowPosition.x + 256, m_WindowPosition.y + m_WindowSize.cy - 22, 118, 21, &g_MainInterfaceTextureArchive, chattingbuttonindex );
	m_ChatBorderRight->SetClipRect( 138, 75, 256, 96  );
	InsertChildObject( m_ChatBorderRight );

	
	//Author : 양희왕 //breif : 시스템 메시지 버튼 수정
	_XCHECKBTN_STRUCTURE dockingsystemmessagebtnstruct = { TRUE, { m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 46 }, { 24, 24 }, 
													  _XDEF_CHATDEFAULT_DOCKINGSYSTEMMESSAGE,
													  chattingbuttonindex2,chattingbuttonindex2, &g_MainInterfaceTextureArchive };

	//_XCHECKBTN_STRUCTURE dockingsystemmessagebtnstruct = { TRUE, { m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 34 }, { 12, 12 }, 
	//												  _XDEF_CHATDEFAULT_DOCKINGSYSTEMMESSAGE,
	//												  chattingbuttonindex,chattingbuttonindex, &g_MainInterfaceTextureArchive };
	m_DockingSystemMessageButton.Create( dockingsystemmessagebtnstruct );	
	m_DockingSystemMessageButton.SetCallBackFunction_ToolTip( _XCallback_CD_CheckSM );	
	
	m_DockingSystemMessageButton.SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 0, 34, 23, 56 );	
	m_DockingSystemMessageButton.SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  47, 34, 71, 56 );
	//m_DockingSystemMessageButton.SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 0, 24, 12, 36 );	
	//m_DockingSystemMessageButton.SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  24, 24,36, 36 );
	
	m_DockingSystemMessageButton.SetButtonSwapMode( TRUE );
	m_DockingSystemMessageButton.SetCheck( TRUE );
	m_DockingSystemMessageButton.ShowWindow( TRUE );
	
	_XCHECKBTN_STRUCTURE whispertargetbtnstruct = { TRUE, { m_WindowPosition.x+92, m_WindowPosition.y + m_WindowSize.cy - 39 }, { 96, 17 }, 
		_XDEF_CHATDEFAULT_WHISPERTARGETBUTTON1,
		chattingbuttonindex,
		chattingbuttonindex, 
		&g_MainInterfaceTextureArchive };
	
	for( int i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
	{			
		m_WhisperTargetButton[i].Create( whispertargetbtnstruct );
		m_WhisperTargetButton[i].MoveWindow( m_WindowPosition.x+91+(i*93), m_WindowPosition.y + m_WindowSize.cy - 39 );
		m_WhisperTargetButton[i].SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 0, 92,  97, 110 );
		m_WhisperTargetButton[i].SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  96,92,  97, 108 );
		m_WhisperTargetButton[i].SetButtonSwapMode( TRUE );
		m_WhisperTargetButton[i].SetCheck( FALSE );
		m_WhisperTargetButton[i].ShowWindow( FALSE );
		whispertargetbtnstruct.commandid++;
	}
	
	m_WhisperTargetButton[0].SetCheck( TRUE );
	m_WhisperTargetButton[0].SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, -1 );
	m_WhisperTargetButton[1].SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, -1 );
	m_WhisperTargetButton[2].SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, -1 );

	_XBTN_STRUCTURE whisperstaticbutton = { TRUE, {  1, m_WindowSize.cy - 39 },{ 92, 18 },
											NULL,
											chattingbuttonindex,chattingbuttonindex,chattingbuttonindex, &g_MainInterfaceTextureArchive };
	m_WhisperStaticButton = new _XButton;
	m_WhisperStaticButton->Create( whisperstaticbutton );
	m_WhisperStaticButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 75, 91, 92 );
	m_WhisperStaticButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 75, 91, 92 );
	m_WhisperStaticButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 75, 91, 92 );
	m_WhisperStaticButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CHATTINGDEFAULT_WHISPERTARGET);	// _T("귓속말 상대")
	m_WhisperStaticButton->SetButtonTextColor( g_ChatFunctionButtonStringColor[5]);
	m_WhisperStaticButton->ShowWindow( FALSE );
	InsertChildObject( m_WhisperStaticButton );
			
	// 채팅창 크기 조절 버튼		
	_XBTN_STRUCTURE chattingresizebutton = { TRUE, {  m_WindowPosition.x + 359, m_WindowPosition.y + 154 },{ 12, 12 }, 
											 _XDEF_CHATDEFAULT_RESIZEBUTTON,
											 chattingbuttonindex,chattingbuttonindex,chattingbuttonindex, &g_MainInterfaceTextureArchive };
	
	m_btnChatResize = new _XButton;
	m_btnChatResize->Create( chattingresizebutton );
	m_btnChatResize->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 36, 12, 48 );
	m_btnChatResize->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  12,36, 24, 48 );
	m_btnChatResize->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  24,36, 36, 48 );
	m_btnChatResize->SetCallBackFunction_ToolTip( _XChattingDefaultCallback_ChatResize );
	InsertChildObject( m_btnChatResize );
	
	// 채팅창 기능 버튼
	_XBTN_STRUCTURE chattingfunctionbutton = { TRUE, {  m_WindowPosition.x + 4, m_WindowPosition.y  - m_WindowSize.cy - 19 },{ 86, 16 },
											   _XDEF_CHATDEFAULT_FUNCTIONBUTTON,
		                                       chattingbuttonindex,chattingbuttonindex,chattingbuttonindex, &g_MainInterfaceTextureArchive };
	
	m_btnChatFunction = new _XButton;
	m_btnChatFunction->Create( chattingfunctionbutton );
	m_btnChatFunction->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 37, 0, 123, 16 );
	m_btnChatFunction->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  37, 16, 123, 32 );
	m_btnChatFunction->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  37, 32, 123, 48 );
	m_btnChatFunction->SetButtonTextClipping( TRUE );
#ifdef _XDEF_NEWSTRINGMANAGER
//#ifdef _XDEF_CASHITEM_SHOUT
	m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[_XCHATMODE_NORMAL], _XFONT_ALIGNTYPE_CENTER, 6 );
//#else
//	m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[5], _XFONT_ALIGNTYPE_LEFT, 6 );
//#endif
#else // _XDEF_NEWSTRINGMANAGER
//#ifdef _XDEF_CASHITEM_SHOUT
	m_btnChatFunction->SetButtonText( g_ChatFunctionButtonString[_XCHATMODE_NORMAL] );
//#else 
//	m_btnChatFunction->SetButtonText( g_ChatFunctionButtonString[5] );
//#endif
#endif // _XDEF_NEWSTRINGMANAGER

//#ifdef _XDEF_CASHITEM_SHOUT
	m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[_XCHATMODE_NORMAL] );
//#else
//	m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[5] );
//#endif
	InsertChildObject( m_btnChatFunction );
		
	m_btnimageChatFunction = new _XImageStatic;
	m_btnimageChatFunction->Create( 6, chattingfunctionbutton.position.y + 3, 3, 9, &g_MainInterfaceTextureArchive, chattingbuttonindex );
//#ifdef _XDEF_CASHITEM_SHOUT
	m_btnimageChatFunction->SetClipRect( 236, 0, 238, 8  );
//#else
//	m_btnimageChatFunction->SetClipRect( 212, 0, 215, 9  );
//#endif
	InsertChildObject( m_btnimageChatFunction );

	// Menu Tab ------------------------------------------------------------------------------------------------------------=
	
#ifdef _XDEF_CASHITEM_SHOUT	
#ifdef _XDEF_ZONECHATTING_20070108	
	int totalline = 5 + (18*_XCHATMODE_TOTALCOUNT) +18 ;
#else
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-1)) +18 ;
#endif//#ifdef _XDEF_ZONECHATTING_20070108	
#else
#ifdef _XDEF_ZONECHATTING_20070108	
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-1)) +18 ;
#else
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-2)) +18 ;
#endif//#ifdef _XDEF_ZONECHATTING_20070108	
#endif//#ifdef _XDEF_CASHITEM_SHOUT

	chattingfunctionbutton.position.x = 3;
//	chattingfunctionbutton.position.y = m_WindowSize.cy - 5 -(18*_XCHATMODE_TOTALCOUNT)-18;
	chattingfunctionbutton.position.y = m_WindowSize.cy - totalline;
	chattingfunctionbutton.commandid = _XDEF_CHATDEFAULT_FUNCTION_EMOTIONBUTTON;
	chattingfunctionbutton.windowsize.cx = 88;
	chattingfunctionbutton.windowsize.cy = 18;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_GMWindow.tga" );
		chattingfunctionbutton.windowsize.cx = 115;
		chattingfunctionbutton.imageindex_click = chattingbuttonindex;
		chattingfunctionbutton.imageindex_normal= chattingbuttonindex;
		chattingfunctionbutton.imageindex_ready = chattingbuttonindex;
	}
	
//#ifdef _XDEF_CASHITEM_SHOUT
	for( i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
	{
		
		m_btnChatFunctionMenuButton[i] = new _XButton;
		m_btnChatFunctionMenuButton[i]->Create( chattingfunctionbutton );
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 177, 116, 195 );
			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 195, 116, 213 );
			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 213, 116, 231 );
		}
		else
		{
			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 124, 0, 212, 18 );
			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  124, 18, 212, 36 );
			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  124, 36, 212, 54 );
		}
#ifdef _XDEF_NEWSTRINGMANAGER
		m_btnChatFunctionMenuButton[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[i], _XFONT_ALIGNTYPE_LEFT, 10, 0 );
#else
		m_btnChatFunctionMenuButton[i]->SetButtonText( g_ChatFunctionButtonString[i], _XFONT_ALIGNTYPE_LEFT, 10, 0 );
#endif
		m_btnChatFunctionMenuButton[i]->SetButtonTextColor( g_ChatFunctionButtonStringColor[i] );

		m_btnimageChatFunctionMenuButton[i] = new _XImageStatic;
		m_btnimageChatFunctionMenuButton[i]->Create( 5, chattingfunctionbutton.position.y + 5, 3, 9, &g_MainInterfaceTextureArchive, chattingbuttonindex );
		m_btnimageChatFunctionMenuButton[i]->SetClipRect( 212 + (i<<2), 0, 215 + (i<<2), 9  );
			
		m_btnChatFunctionMenuButton[i]->ShowWindow( FALSE );
		m_btnimageChatFunctionMenuButton[i]->ShowWindow( FALSE );
		
		if( i == 1 )
		{
			chattingfunctionbutton.commandid ++;
#ifdef _XDEF_CASHITEM_SHOUT	
			chattingfunctionbutton.position.y += 18;
#endif
		}
		else if( i == 6)
		{
			chattingfunctionbutton.commandid ++;
#ifdef _XDEF_ZONECHATTING_20070108	
			chattingfunctionbutton.position.y += 18;
#endif
		}
		else
		{
			chattingfunctionbutton.commandid ++;
			chattingfunctionbutton.position.y += 18;	
		}
	
	}
//#else //_XDEF_CASHITEM_SHOUT
//	for( i = 0; i < 6; i++ )
//	{
//		m_btnChatFunctionMenuButton[i] = new _XButton;
//		m_btnChatFunctionMenuButton[i]->Create( chattingfunctionbutton );
//		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
//		{
//			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 177, 116, 195 );
//			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 195, 116, 213 );
//			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 213, 116, 231 );
//		}
//		else
//		{
//			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 124, 0, 212, 18 );
//			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  124, 18, 212, 36 );
//			m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  124, 36, 212, 54 );
//		}
//#ifdef _XDEF_NEWSTRINGMANAGER
//		m_btnChatFunctionMenuButton[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[i], _XFONT_ALIGNTYPE_LEFT, 10, 0 );
//#else
//		m_btnChatFunctionMenuButton[i]->SetButtonText( g_ChatFunctionButtonString[i], _XFONT_ALIGNTYPE_LEFT, 10, 0 );
//#endif
//		m_btnChatFunctionMenuButton[i]->SetButtonTextColor( g_ChatFunctionButtonStringColor[i] );
//		
//		m_btnimageChatFunctionMenuButton[i] = new _XImageStatic;
//		m_btnimageChatFunctionMenuButton[i]->Create( 5, chattingfunctionbutton.position.y + 5, 3, 9, &g_MainInterfaceTextureArchive, chattingbuttonindex );
//		m_btnimageChatFunctionMenuButton[i]->SetClipRect( 212 + (i<<2), 0, 215 + (i<<2), 9  );
//		
//		m_btnChatFunctionMenuButton[i]->ShowWindow( FALSE );
//		m_btnimageChatFunctionMenuButton[i]->ShowWindow( FALSE );
//
//		chattingfunctionbutton.commandid ++;
//		chattingfunctionbutton.position.y += 18;		
//	}
//#endif //_XDEF_CASHITEM_SHOUT 

#ifdef _XDEF_CASHITEM_SHOUT
	m_btnChatFunctionMenuButton[1]->EnableWindow(FALSE);	//외치기 disable
	m_btnChatFunctionMenuButton[2]->EnableWindow( FALSE );	// 문파채팅 제한
#else
	// 임시 제한 문파챗
	m_btnChatFunctionMenuButton[2]->EnableWindow( FALSE );
#endif
	
	// ---------------------------------------------------------------------------------------------------------------------=	
	// Emotion ListBox -----------------------------------------------------------------------------------------------------=	
	// 퀘스트에 사용될 리스트 박스
	_XLISTBOX_STRUCTURE emtionstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 95, gnHeight-239 },							// 윈도우 좌표
		{ 70, 160 },	// 윈도우 사이즈
		_XDEF_CHATDEFAULT_EMOTIONLISTBOX,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		16,												// 리스트 아이템 라인 피치
		10,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_NOSCROLLBAR,							// 스크롤바의 위치
		
		// scroll bar property
		{ 0, 0 },											// 스크롤바 사이즈
		0,													// 스크롤바 상하 버튼 사이즈
		0,													// 스크롤바 트랙바 사이즈
		10,													// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		emtionstructure.position.x = 116;
	}
	
	if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		emtionstructure.windowsize.cx = 110;
	}
	
	m_EmotionListBox = new _XListBox;
	m_EmotionListBox->Create(emtionstructure);
	POINT coloffset = { 32, 3 };
	
	if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		coloffset.x += 15;

	m_EmotionListBox->SetColumnOffset(0, coloffset);
	//m_EmotionListBox->EnableSelectItem(FALSE);
	m_EmotionListBox->SetTextXOffset(3);
	m_EmotionListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_EmotionListBox->SetAlignType(0, _XFONT_ALIGNTYPE_CENTER);
	m_EmotionListBox->SetBorderColorFactor( 0xB1000000 );
	//m_EmotionListBox->SetTrackBarColor( D3DCOLOR_ARGB(255,255,135,48) );
	m_EmotionListBox->SetColumnWidth(0, 60 );
	m_EmotionListBox->SetSelectedImagePosition(5, 2);
	m_EmotionListBox->SetSelectedImageHeight(16);
	
	if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		m_EmotionListBox->SetTrackWidth(66+15);
	else
		m_EmotionListBox->SetTrackWidth(66);

	
	for( i = 0; i < 13; i++ )
	{
		m_EmotionListBox->InsertListItem(g_ChatEmotionString[i], 0xFFFCD7BC, i+1 );
			
		m_EmotionListBox->SetItemAttribHighlight( m_EmotionListBox->GetItemCount()-1, 0, 0xFFFCD7BC );
	}

	m_EmotionListBox->ShowWindow( FALSE );
	
	int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
	_XBTN_STRUCTURE maximummapexitbtnstruct = 
	{
		TRUE, {gnWidth - (1024 - 923), gnHeight - (768- 730)}, {80, 26}, _XDEF_MAXMINIMAP_CLOSEBUTTON,
			questresourceindex2, questresourceindex2, questresourceindex2,
			&g_MainInterfaceTextureArchive
	};
	
	m_btnMaximumMapModeExit = new _XButton;
	m_btnMaximumMapModeExit->Create(maximummapexitbtnstruct);
	m_btnMaximumMapModeExit->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL,80, 190, 160, 216);
	m_btnMaximumMapModeExit->SetButtonImageClipRect(_XBUTTONIMAGE_READY,80, 190, 160, 216);
	m_btnMaximumMapModeExit->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 190, 80, 216);
	m_btnMaximumMapModeExit->SetButtonTextColor( _XSC_DEFAULT_HIGHLIGHT );
	m_btnMaximumMapModeExit->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CHATTINGDEFAULT_2008 );
	m_btnMaximumMapModeExit->ShowWindow(FALSE);
	InsertChildObject( m_btnMaximumMapModeExit );
	// ---------------------------------------------------------------------------------------------------------------------=	

	// Set window position...
	int chatwinxpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHATWINDOWXPOS, 0, TRUE );
	if( chatwinxpos < 0 ) chatwinxpos = 0;
	int chatwinypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHATWINDOWYPOS, gnHeight-_XDEF_CHATWINDOWMINSIZE_HEIGHT, TRUE );
	if( chatwinypos < 0 ) chatwinxpos = 0;
	int chatwinwidth  = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHATWINDOWWIDTH , _XDEF_CHATWINDOWMINSIZE_WIDTH  ,TRUE );
	int chatwinheight = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHATWINDOWHEIGHT, _XDEF_CHATWINDOWMINSIZE_HEIGHT ,TRUE );
	
	// set Whisper ime
	m_WhisperEditContainer.ResetContainer();
	m_WhisperEditContainer.MoveWindow( m_WindowPosition.x+95, m_WindowPosition.y + m_WindowSize.cy - 34 );
	m_WhisperEditContainer.ResizeWindow( 91, 12 );
	m_WhisperEditContainer.ShowWindow(FALSE);
	m_WhisperEditContainer.SetMaxLength(12);
	m_WhisperEditContainer.SetReturnKeyCallback( _XIMECallback_ChatWhisperReturn );
	m_WhisperEditContainer.SetLeftArrowKeyCallback( _XIMECallback_ChatWhisperLeftArrow );
	m_WhisperEditContainer.SetRightArrowKeyCallback( _XIMECallback_ChatWhisperRightArrow );


#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

	int npcresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "mi_bloodpoint.tga" );

	_XCHECKBTN_STRUCTURE lockbtnstruct = { TRUE, { -1, 1-17 },
	{ 74, 16 },
	_XDEF_CHATDEFAULT_LOCKFILTERMENUBUTTON,
	npcresourceindex1,npcresourceindex1,
	&g_MainInterfaceTextureArchive };
	
	m_pLockChatFilterMenuButton = new _XCheckButton;
	m_pLockChatFilterMenuButton->Create( lockbtnstruct );	
	m_pLockChatFilterMenuButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 180, 224, 256, 240 );
	m_pLockChatFilterMenuButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  180, 240, 256, 256 );
	m_pLockChatFilterMenuButton->SetButtonSwapMode( TRUE );
	m_pLockChatFilterMenuButton->SetCheck( TRUE );
	m_pLockChatFilterMenuButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3473 );//_T("채팅보기")
	m_pLockChatFilterMenuButton->SetButtonTextColor( D3DCOLOR_ARGB(255, 200, 200, 0), D3DCOLOR_ARGB(255, 255, 255, 0), D3DCOLOR_ARGB(255, 255, 255, 0) );
	m_pLockChatFilterMenuButton->SetCallBackFunction_ToolTip( _XCallback_LockChatFilterMenuButton );
	InsertChildObject( m_pLockChatFilterMenuButton );

	m_pLockChatFilterMenuButton->ShowWindow( FALSE );
	
	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XCHECKBTN_STRUCTURE filterbtnstruct = { 
		TRUE, { 73+61, -15 },{ 14, 14 },_XDEF_CHATDEFAULT_NORMALFILTERMENUBUTTON,
		optionmenuinterface,optionmenuinterface,	&g_MainInterfaceTextureArchive 
	};

	for( i = 0; i < 4; i++ )
	{
		m_pChatFilterButton[i] = new _XCheckButton;

		m_pChatFilterButton[i]->Create( filterbtnstruct );

		m_pChatFilterButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
		m_pChatFilterButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
		
		m_pChatFilterButton[i]->SetButtonSwapMode( FALSE );
		m_pChatFilterButton[i]->SetCheck( TRUE );
		m_pChatFilterButton[i]->ShowWindow( FALSE );
		
		InsertChildObject( m_pChatFilterButton[i] );

		filterbtnstruct.position.x += 74;
		filterbtnstruct.commandid++;
	}

	m_pChatFilterButton[0]->SetCallBackFunction_ToolTip( _XCallback_ChatFilterButton1 );
	m_pChatFilterButton[1]->SetCallBackFunction_ToolTip( _XCallback_ChatFilterButton2 );
	m_pChatFilterButton[2]->SetCallBackFunction_ToolTip( _XCallback_ChatFilterButton3 );
	m_pChatFilterButton[3]->SetCallBackFunction_ToolTip( _XCallback_ChatFilterButton4 );

	m_LockButtonBackImage.Create( 0, 0, 74, 16, &g_MainInterfaceTextureArchive, npcresourceindex1 );
	m_LockButtonBackImage.SetClipRect( 180, 224, 256, 240  );

	m_LockButtonColorIndicateImage.Create( 0, 0, 74, 16, &g_MainInterfaceTextureArchive, chattingbuttonindex );
	m_LockButtonColorIndicateImage.SetClipRect( 212, 0, 215, 9  );	

	//Author : 양희왕 //breif : 메시지버튼 수정
	_XCHECKBTN_STRUCTURE blacklistbtnstruct = 
	{ 
		TRUE, 
		{ 25, m_WindowPosition.y + m_WindowSize.cy - 46 }, 
		{ 24, 24 }, 
		_XDEF_CHATDEFAULT_CHATBLACKLISTUBUTTON,
		chattingbuttonindex2,chattingbuttonindex2, &g_MainInterfaceTextureArchive 
	};
	/*_XBTN_STRUCTURE blacklistbtnstruct = 
	{
		TRUE, {12, m_WindowPosition.y + m_WindowSize.cy - 34 }, {12, 12}, _XDEF_CHATDEFAULT_CHATBLACKLISTUBUTTON,
			chattingbuttonindex, chattingbuttonindex, chattingbuttonindex,
			&g_MainInterfaceTextureArchive
	};*/
	
	m_pChatBlackListButton = new _XCheckButton;
	m_pChatBlackListButton->Create(blacklistbtnstruct);

	m_pChatBlackListButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 0, 58, 24, 80);
	//m_pChatBlackListButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 24, 56, 48, 80);
	m_pChatBlackListButton->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 48, 58, 72, 80);

	m_pChatBlackListButton->SetButtonSwapMode( TRUE );
	m_pChatBlackListButton->SetCheck( FALSE );

	//m_pChatBlackListButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL,96, 78, 108, 90);
	//m_pChatBlackListButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY,108, 78, 120, 90);
	//m_pChatBlackListButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,120, 78, 132, 90);

	m_pChatBlackListButton->SetCallBackFunction_ToolTip( _XCallback_BlackListButton );
	
	InsertChildObject( m_pChatBlackListButton );	
	
#endif

	SetDefaultPosition();
	return TRUE;
}

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
int _XWindow_ChattingDefault::RecalcChattingListLength( void )
{
	_XStringItem* curstring;
	int			  colorindex;
	int			  filtereditemcounter = 0;

	if( !g_Chatlist.listEmpty() )
	{
		g_Chatlist.resetList();

		do 
		{
			curstring = (_XStringItem*)g_Chatlist.currentItem();
			colorindex = (((DWORD)curstring->Get_Attribute()) % 16);
			
			if( colorindex == _XDEF_CHATMESSAGECOLOR_WHISPER )
			{
				if( m_pChatFilterButton[1]->GetCheck() )
					filtereditemcounter++;

			}
			else if( colorindex == _XDEF_CHATMESSAGECOLOR_PARTY )
			{
				if( m_pChatFilterButton[2]->GetCheck() )
					filtereditemcounter++;
			}
			else if( colorindex == _XDEF_CHATMESSAGECOLOR_USERCLAN || colorindex == _XDEF_CHATMESSAGECOLOR_ALLY )
			{
				if( m_pChatFilterButton[3]->GetCheck() )
					filtereditemcounter++;
			}
			else // normal message
			{
				if( m_pChatFilterButton[0]->GetCheck() )
					filtereditemcounter++;
			}

			g_Chatlist.nextItem();

		} while( !g_Chatlist.atHeadOfList() );
	}

	return filtereditemcounter;
}
#endif

void _XWindow_ChattingDefault::SetDefaultPosition( void )
{
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	if( eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() )
		ResizeWindow( 0, gnHeight-_XDEF_CHATWINDOWMINSIZE_HEIGHT,_XDEF_CHATWINDOWMINSIZE_WIDTH, gnHeight );		
}

void _XWindow_ChattingDefault::SaveWindowProperty( void )
{
	// save chat window properties...	
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHATWINDOWXPOS,   GetWindowPos().x );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHATWINDOWYPOS,   GetWindowPos().y );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHATWINDOWWIDTH,	 GetWindowSize().cx );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_CHATWINDOWHEIGHT, GetWindowSize().cy );	
}

BOOL _XWindow_ChattingDefault::CheckMousePosition(void)
{
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	if( !eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() )
	{
		return FALSE;
	}
	else
	{
		return _XWindow::CheckMousePosition();
	}
}

void _XWindow_ChattingDefault::DestroyWindow( void )
{
	m_WhisperEditContainer.ShowWindow( FALSE );
	_XWindow::DestroyWindow();
/*#ifdef _XDEF_CASHITEM_SHOUT*/
//#else
//	for( int i = 0; i < 6; i++ )
//#endif
	for( int i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
	{	
		if( m_btnChatFunctionMenuButton[i] )
		{
			delete m_btnChatFunctionMenuButton[i];
			m_btnChatFunctionMenuButton[i] = NULL;
		}
		if( m_btnimageChatFunctionMenuButton[i] )
		{
			delete m_btnimageChatFunctionMenuButton[i];
			m_btnimageChatFunctionMenuButton[i] = NULL;
		}
	}
	
	if( m_EmotionListBox )
	{
		delete m_EmotionListBox;
		m_EmotionListBox = NULL;
	}
}

void _XWindow_ChattingDefault::ShowWindow( BOOL show )
{		
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	
	if(!show)
	{
		_XIMEKERNEL.ResetCombinationBuffer();
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA		
		eodapp->m_proc_maingame.m_ChatEditContainer.ClearBuffer();
#endif
		m_WhisperEditContainer.ClearBuffer();
		m_EmotionListBox->ShowWindow( FALSE );
		_XIMEKERNEL.SetFocus( NULL ); 
	}
	//else
	//{
	//	m_pMyManager->SetTopWindow(this);
	//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();
	//}

//#ifdef _XDEF_CASHITEM_SHOUT
	for( int i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
//#else
//	for( int i = 0; i < 6; i++ )
//#endif
	{
		if( i == _XCHATMODE_SHOUT )
		{
#ifdef _XDEF_CASHITEM_SHOUT
			m_btnChatFunctionMenuButton[i]->ShowWindow( show );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( show );
#else
			m_btnChatFunctionMenuButton[i]->ShowWindow( FALSE );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( FALSE );
#endif
		}
		else if( i == _XCHATMODE_ZONE )
		{
#ifdef _XDEF_ZONECHATTING_20070108
			m_btnChatFunctionMenuButton[i]->ShowWindow( show );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( show );
#else
			m_btnChatFunctionMenuButton[i]->ShowWindow( FALSE );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( FALSE );

#endif
		}
		else
		{
			m_btnChatFunctionMenuButton[i]->ShowWindow( show );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( show );
		}
	}

	_XWindow::ShowWindow( show );	

	ShowChatFunctionMenu(FALSE);
}

void _XWindow_ChattingDefault::MoveWindow( int X, int Y )
{			
	// Clipping from screen size	
	_XWindow::MoveWindow( X, Y );

	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;	
	if( eodapp )
	{		
		eodapp->m_proc_maingame.m_ChatEditContainer.MoveWindow( X+96, this->m_WindowPosition.y + this->m_WindowSize.cy - 17 );
	}
	
	m_WhisperEditContainer.MoveWindow(X+95, this->m_WindowPosition.y + this->m_WindowSize.cy - 34 );

	m_btnChatResize->MoveWindow( m_WindowPosition.x + 360, m_WindowPosition.y + m_WindowSize.cy - 18 );	
	
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	m_ChatListScrollBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 1 );
#else	
	m_ChatListScrollBar->MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 1 );
#endif
	
	if( m_ChatMode == _XCHATMODE_WHISPER )
	{
		m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 63 );
		//m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 51 );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		//m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 53 );
		m_ChatListScrollBar->SetWindowHeight( m_DockingSystemMessageButton.GetWindowPos().y - m_WindowPosition.y );

		if(m_pChatBlackListButton)
			m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+25, m_WindowPosition.y + m_WindowSize.cy - 63 );
			//m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+12, m_WindowPosition.y + m_WindowSize.cy - 51 );
#else
		m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 53 );
#endif
	}
	else
	{
		m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 46 );
		//m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 34 );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		//m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 36 );
		m_ChatListScrollBar->SetWindowHeight( m_DockingSystemMessageButton.GetWindowPos().y - m_WindowPosition.y );

		if(m_pChatBlackListButton)
			m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+25, m_WindowPosition.y + m_WindowSize.cy - 46 );
			//m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+12, m_WindowPosition.y + m_WindowSize.cy - 34 );
#else
		m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 38 );
#endif
	}
	
	m_WhisperStaticButton->MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + m_WindowSize.cy - 39 );
	for( int i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
	{
		m_WhisperTargetButton[i].MoveWindow( m_WindowPosition.x+91+(i*93), m_WindowPosition.y + m_WindowSize.cy - 39 );
	}

	m_ChatBorderLeft->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy - 22 );	
	m_ChatBorderRight->MoveWindow( m_WindowPosition.x + 256, m_WindowPosition.y + m_WindowSize.cy - 22 );

	m_btnChatFunction->MoveWindow( m_WindowPosition.x + 3 , m_WindowPosition.y + m_WindowSize.cy - 20 );
	m_btnimageChatFunction->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - 20 + 3 );
	
//#ifdef _XDEF_CASHITEM_SHOUT	
#ifdef _XDEF_CASHITEM_SHOUT	
#ifdef _XDEF_ZONECHATTING_20070108	
	int totalline = 5 + (18*_XCHATMODE_TOTALCOUNT) +18 ;
#else
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-1)) +18 ;
#endif//#ifdef _XDEF_ZONECHATTING_20070108	
#else
#ifdef _XDEF_ZONECHATTING_20070108	
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-1)) +18 ;
#else
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-2)) +18 ;
#endif//#ifdef _XDEF_ZONECHATTING_20070108	
#endif//#ifdef _XDEF_CASHITEM_SHOUT

	int count = 0;
	for( i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
	{
		if( i == _XCHATMODE_SHOUT )
		{
#ifdef _XDEF_CASHITEM_SHOUT	
			m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - totalline + (count * 18) );
			m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - totalline + 4 + (count * 18) );
			++count;
#endif
		}
		else if( i == _XCHATMODE_ZONE)
		{
#ifdef _XDEF_ZONECHATTING_20070108	
			m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - totalline + (count * 18) );
			m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - totalline + 4 + (count * 18) );
			++count;
#endif
		}
		else
		{
			m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - totalline + (count * 18) );
			m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - totalline + 4 + (count * 18) );
			++count;
		}
//		m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 149 + (i * 18) );
//		m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - 149 + 4 + (i * 18) );
	}
//#else //_XDEF_CASHITEM_SHOUT
//	for( i = 0; i < 6; i++ )
//	{
//		m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 131 + (i * 18) );
//		m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - 131 + 4 + (i * 18) );
//	}
//#endif //_XDEF_CASHITEM_SHOUT
	m_btnMaximumMapModeExit->MoveWindow( gnWidth - (1024 - 923), gnHeight - (768- 730) );
}

void _XWindow_ChattingDefault::ResizeWindow( int SX, int SY, int EX, int EY )
{
	if( EX-SX < _XDEF_CHATWINDOWMINSIZE_WIDTH && EY-SY < _XDEF_CHATWINDOWMINSIZE_HEIGHT ) return;
	if( EX-SX > _XDEF_CHATWINDOWMAXSIZE_WIDTH && EY-SY > _XDEF_CHATWINDOWMAXSIZE_HEIGHT ) return;
	
	if( EX-SX > _XDEF_CHATWINDOWMAXSIZE_WIDTH )
	{
		if( SX == m_WindowPosition.x )
		{
			EX = m_WindowPosition.x + _XDEF_CHATWINDOWMAXSIZE_WIDTH;
		}
		else
		{
			SX = m_WindowPosition.x + m_WindowSize.cx - _XDEF_CHATWINDOWMAXSIZE_WIDTH;
		}
	}
	else if( EX-SX < _XDEF_CHATWINDOWMINSIZE_WIDTH )
	{
		if( SX == m_WindowPosition.x )
		{
			EX = m_WindowPosition.x + _XDEF_CHATWINDOWMINSIZE_WIDTH;
		}
		else
		{
			SX = m_WindowPosition.x + m_WindowSize.cx - _XDEF_CHATWINDOWMINSIZE_WIDTH;
		}
	}
	
	if( EY-SY > _XDEF_CHATWINDOWMAXSIZE_HEIGHT )
	{
		if( SY == m_WindowPosition.y )
		{
			EY = m_WindowPosition.y + _XDEF_CHATWINDOWMAXSIZE_HEIGHT;
		}
		else
		{
			SY = m_WindowPosition.y + m_WindowSize.cy - _XDEF_CHATWINDOWMAXSIZE_HEIGHT;
		}
	}
	else if( EY-SY < _XDEF_CHATWINDOWMINSIZE_HEIGHT )
	{
		if( SY == m_WindowPosition.y )
		{
			EY = m_WindowPosition.y + _XDEF_CHATWINDOWMINSIZE_HEIGHT;
		}
		else
		{
			SY = m_WindowPosition.y + m_WindowSize.cy - _XDEF_CHATWINDOWMINSIZE_HEIGHT;
		}
	}
	
	m_WindowPosition.x = SX;
	m_WindowPosition.y = SY;
	
	m_WindowSize.cx = EX-SX;
	m_WindowSize.cy = EY-SY;
	
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;	
	if( eodapp )
	{					
		eodapp->m_proc_maingame.m_ChatEditContainer.MoveWindow( this->m_WindowPosition.x + 96, 
			this->m_WindowPosition.y + this->m_WindowSize.cy - 17 );
	}
	
	m_WhisperEditContainer.MoveWindow( this->m_WindowPosition.x + 95, this->m_WindowPosition.y + this->m_WindowSize.cy - 34 );

	m_solidborderstatic.MoveWindow( SX, SY+1, EX, EY );
				
	m_WhisperStaticButton->MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + m_WindowSize.cy - 39 );
	for( int i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
	{
		m_WhisperTargetButton[i].MoveWindow( m_WindowPosition.x+91+(i*93), m_WindowPosition.y + m_WindowSize.cy - 39 );
	}
	
	if( m_ChatMode == _XCHATMODE_WHISPER )
	{
		m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 63 );
		//m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 51 );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		//m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 53 );
		m_ChatListScrollBar->SetWindowHeight( m_DockingSystemMessageButton.GetWindowPos().y - m_WindowPosition.y );

		if(m_pChatBlackListButton)
			m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+25, m_WindowPosition.y + m_WindowSize.cy - 63 );
			//m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+12, m_WindowPosition.y + m_WindowSize.cy - 51 );
#else
		m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 53 );
#endif	
		m_MaxViewCount = ((m_WindowSize.cy-50) / 15);
	}
	else
	{
		m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 46 );
		//m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 34 );

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		//m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 36 );
		m_ChatListScrollBar->SetWindowHeight( m_DockingSystemMessageButton.GetWindowPos().y - m_WindowPosition.y );

		if(m_pChatBlackListButton)
			m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+25, m_WindowPosition.y + m_WindowSize.cy - 46 );
			//m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+12, m_WindowPosition.y + m_WindowSize.cy - 34 );
#else
		m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 38 );
#endif
		m_MaxViewCount = ((m_WindowSize.cy-34) / 15);
	}
	
	m_btnChatResize->MoveWindow( m_WindowPosition.x + 360, m_WindowPosition.y + m_WindowSize.cy - 18 );	

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	m_ChatListScrollBar->MoveWindow( m_WindowPosition.x,m_WindowPosition.y + 1 );
#else
	m_ChatListScrollBar->MoveWindow( m_WindowPosition.x+1,m_WindowPosition.y + 1 );
#endif
		
	m_ChatListScrollBar->SetViewLineCount( m_MaxViewCount );
	m_ChatListScrollBar->RebuildTrackBar();
	m_ChatListScrollBar->SetLineScrollPos( 999999 ); 
	
	m_ChatBorderLeft->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy - 22 );	
	m_ChatBorderRight->MoveWindow( m_WindowPosition.x + 256, m_WindowPosition.y + m_WindowSize.cy - 22 );

	m_btnChatFunction->MoveWindow( m_WindowPosition.x + 3 , m_WindowPosition.y + m_WindowSize.cy - 20 );
	m_btnimageChatFunction->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - 20 + 3 );


#ifdef _XDEF_CASHITEM_SHOUT	
#ifdef _XDEF_ZONECHATTING_20070108	
	int totalline = 5 + (18*_XCHATMODE_TOTALCOUNT) +18 ;
#else
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-1)) +18 ;
#endif//#ifdef _XDEF_ZONECHATTING_20070108	
#else
#ifdef _XDEF_ZONECHATTING_20070108	
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-1)) +18 ;
#else
	int totalline = 5 + (18*(_XCHATMODE_TOTALCOUNT-2)) +18 ;
#endif//#ifdef _XDEF_ZONECHATTING_20070108	
#endif//#ifdef _XDEF_CASHITEM_SHOUT
	
	int count = 0;
	for( i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
	{
		if( i == _XCHATMODE_SHOUT )
		{
#ifdef _XDEF_CASHITEM_SHOUT	
			m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - totalline + (count * 18) );
			m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - totalline + 4 + (count * 18) );
			++count;
#endif
		}
		else if( i == _XCHATMODE_ZONE)
		{
#ifdef _XDEF_ZONECHATTING_20070108	
			m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - totalline + (count * 18) );
			m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - totalline + 4 + (count * 18) );
			++count;
#endif
		}
		else
		{
			m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - totalline + (count * 18) );
			m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - totalline + 4 + (count * 18) );
			++count;
		}
	}

//	for( i = 0; i < 8; i++ )
//	{
//		m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 149 + (i * 18) );
//		m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - 149 + 4 + (i * 18) );
//	}
//#else
//	for( i = 0; i < 6; i++ )
//	{
//		m_btnChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 131 + (i * 18) );
//		m_btnimageChatFunctionMenuButton[i]->MoveWindow( m_WindowPosition.x + 5, m_WindowPosition.y + m_WindowSize.cy - 131 + 4 + (i * 18) );
//	}
//#endif
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

	if( m_pLockChatFilterMenuButton )
	{
		m_pLockChatFilterMenuButton->MoveWindow( m_WindowPosition.x - 1, m_WindowPosition.y + 1 - 17 );
	}
	
	int filterbtnxpos = 73+61;
	for( i = 0; i < 4; i++ )
	{
		if( m_pChatFilterButton[i] )
			m_pChatFilterButton[i]->MoveWindow( m_WindowPosition.x + filterbtnxpos, m_WindowPosition.y - 15 );

		filterbtnxpos += 74;		
	}

#endif
}

#define _XDEF_CHATMESSAGEBLINKTIME		10000


void _XWindow_ChattingDefault::Draw( _XGUIObject*& pfocusobject )
{	

	BOOL checkinsidefiltermenu = FALSE;
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;

	if(this->m_ShowWindow)	
	{
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

	if( eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() ) 
	{
		checkinsidefiltermenu = gpInput->CheckMousePos( m_WindowPosition.x, m_WindowPosition.y - 17, m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y-1 );

		LPTSTR m_pLockButtonString[4] = 
		{
			_XGETINTERFACETEXT(ID_STRING_NEW_3421), //"일반"
			_XGETINTERFACETEXT(ID_STRING_REQUESTPARTY_WISPER), 
			_XGETINTERFACETEXT(ID_STRING_PARTY_PARTY),
			_XGETINTERFACETEXT(ID_STRING_NEW_3129),			
		};

		D3DCOLOR lockbuttoncolortable[4] = 
		{
			D3DCOLOR_ARGB( 255, 180, 180, 180 ),
			D3DCOLOR_ARGB( 255, 243, 162, 241 ),
			D3DCOLOR_ARGB( 255, 241, 223, 72 ),
			D3DCOLOR_ARGB( 255, 191, 236, 125 )
		};

		BOOL drawfiltermenu = FALSE;
		if( !m_LockChatFilterMenu )
		{
			if( checkinsidefiltermenu )
			{
				drawfiltermenu = TRUE;
			}
		}
		else
		{
			drawfiltermenu = TRUE;
		}

		if(drawfiltermenu)
		{
			_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y - 17, m_WindowPosition.x + m_WindowSize.cx-1, m_WindowPosition.y, 0x70000000 );
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y - 17, m_WindowPosition.x + m_WindowSize.cx-1, m_WindowPosition.y, 0.0f, 0x7F000000 );			

			int lockbuttonpos = 75;
			for( int li = 0; li < 4; li++ )
			{
				if( m_pChatFilterButton[li]->GetCheck() )
					m_LockButtonBackImage.SetClipRect( 180, 224, 256, 240  );
				else
					m_LockButtonBackImage.SetClipRect( 180, 240, 256, 256  );

				m_LockButtonBackImage.Draw( m_WindowPosition.x + lockbuttonpos, m_WindowPosition.y - 16 );

				g_XBaseFont->SetColor( lockbuttoncolortable[li] );
				g_XBaseFont->Print( m_WindowPosition.x + lockbuttonpos + 7, m_WindowPosition.y - 14, 1.0f, m_pLockButtonString[li] );
				
				m_LockButtonColorIndicateImage.SetClipRect( 236 - (li<<2), 0, 239 - (li<<2), 9  );
				m_LockButtonColorIndicateImage.Draw( m_WindowPosition.x + lockbuttonpos +3, m_WindowPosition.y - 13 );

				lockbuttonpos += 74;
			}

			g_XBaseFont->Flush();
		}
	}

#endif
	}
	
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( !this->m_Enable     ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	if( eodapp->m_proc_maingame.m_bNPCDialogMode || eodapp->m_proc_maingame.m_bNPCDialogWideView || g_ZeroLevelMode ) 
		return;

	if( g_pLocalUser->m_ChatMuteTime > 0 ) // 채팅 금지 처리
	{
		if( eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() ) 
		{
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
			_XDrawSolidBar( m_WindowPosition.x+1, m_WindowPosition.y - 13, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y, 0x80000000 );
#else
			_XDrawSolidBar( m_WindowPosition.x+1, m_WindowPosition.y - 13, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y, 0x40000000 );			
#endif
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y - 14, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y, 0.0f, 0x7F000000 );
		}

		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, D3DCOLOR_ARGB( 255, 46, 104, 212 ) );

		int minutes = g_pLocalUser->m_ChatMuteTime / 60000;

		g_XBaseFont->SetColor( 0xFFFFFFF );
		if( minutes > 0 )
		{
			g_XBaseFont->Print( m_WindowPosition.x + 3, m_WindowPosition.y - 12, 1.0f, _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_2009), minutes );
		}
		else
		{
			int seconds = (g_pLocalUser->m_ChatMuteTime % 60000) / 1000;
			g_XBaseFont->Print( m_WindowPosition.x + 3, m_WindowPosition.y - 12, 1.0f, _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_2010), seconds );
		}
		g_XBaseFont->Flush();
		g_XBaseFont->DisableGrowMode();
	}

	// Window rect
	if( eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() || m_WhisperEditContainer.GetFocus() )
	{
		//_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	}
	else	
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );

	_XWindow_SystemMsgDefault* defaultsystemmsgwindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );

	if( !g_Chatlist.listEmpty() )
	{
		g_Chatlist.resetList();

		_XStringItem* curstring;
		int ypos = 0;
		DWORD colorindex;

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

		int filtereditemcounter = 0;
		do 
		{
			curstring = (_XStringItem*)g_Chatlist.currentItem();
			colorindex = (((DWORD)curstring->Get_Attribute()) % 16);
			
			if( colorindex == _XDEF_CHATMESSAGECOLOR_WHISPER )
			{
				if( m_pChatFilterButton[1]->GetCheck() )
					filtereditemcounter++;

			}
			else if( colorindex == _XDEF_CHATMESSAGECOLOR_PARTY )
			{
				if( m_pChatFilterButton[2]->GetCheck() )					
					filtereditemcounter++;
			}
			else if( colorindex == _XDEF_CHATMESSAGECOLOR_USERCLAN || colorindex == _XDEF_CHATMESSAGECOLOR_ALLY )
			{
				if( m_pChatFilterButton[3]->GetCheck() )					
					filtereditemcounter++;
			}
			else // normal message
			{
				if( m_pChatFilterButton[0]->GetCheck() )					
					filtereditemcounter++;
			}

			g_Chatlist.nextItem();

			if( filtereditemcounter >= m_ChatListScrollPos )
				break;

		} while( !g_Chatlist.atHeadOfList() );

#else
		for( int i = 0; i < m_ChatListScrollPos; i++ ) g_Chatlist.nextItem();
#endif

		do
		{			
			curstring = (_XStringItem*)g_Chatlist.currentItem();			
			colorindex = (((DWORD)curstring->Get_Attribute()) % 16);
			DWORD attrib1 = LOWORD( (DWORD)curstring->Get_Attribute() );
			DWORD offset  = HIWORD( (DWORD)curstring->Get_Attribute() );
			
			int CheckAdminAttrib  = (attrib1 / _XDEF_CHATMESSAGEMODE_ADMINNOTICE);
			int CheckGlobalAttrib = (attrib1 / _XDEF_CHATMESSAGEMODE_GLOBALNOTICE);

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
			BOOL skipitem = FALSE;
			if( colorindex == _XDEF_CHATMESSAGECOLOR_WHISPER )
			{
				if( !m_pChatFilterButton[1]->GetCheck() ) skipitem = TRUE;					
			}
			else if( colorindex == _XDEF_CHATMESSAGECOLOR_PARTY )
			{
				if( !m_pChatFilterButton[2]->GetCheck() ) skipitem = TRUE;
			}
			else if( colorindex == _XDEF_CHATMESSAGECOLOR_USERCLAN || colorindex == _XDEF_CHATMESSAGECOLOR_ALLY )
			{
				if( !m_pChatFilterButton[3]->GetCheck() ) skipitem = TRUE;
			}
			else // normal message
			{
				if( !m_pChatFilterButton[0]->GetCheck() ) skipitem = TRUE;
			}

			if( !skipitem )
			{
#endif
			
			if( CheckAdminAttrib == 1 )
			{		
				// 시간이 지나면 깜빡임을 없앰.
				if( (g_LocalSystemTime - curstring->Get_Attribute(1)) > 3000)//_XDEF_CHATMESSAGEBLINKTIME ) // 2004.06.07->oneway48 insert
				{
					//curstring->Set_Attribute( colorindex, 0 );
					g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);
					if( defaultsystemmsgwindow )
					{
						if( defaultsystemmsgwindow->GetShowStatus() )
							
							g_XBaseFont->PutsAlign( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_LEFT, curstring->Get_String() );
						else
							g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 6 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );							
					}
				}
				else
				{
					if( (gnFPS/3) < gnFrameCounter )
					{
						g_XBaseFont->SetColor(g_ChattingStringColor[ _XDEF_CHATMESSAGECOLOR_ADMINNOTICE ]);					
						//g_XBaseFont->Puts( this->m_WindowPosition.x + 20 + offset, (m_WindowPosition.y+4) + (ypos * 12),
						//	                  curstring->Get_String() );
						if( defaultsystemmsgwindow )
						{
							if( defaultsystemmsgwindow->GetShowStatus() )
								g_XBaseFont->PutsAlign( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_LEFT, curstring->Get_String() );
							else
								g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 6 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );
						}
											
					}
				}
			}
			else if( CheckGlobalAttrib == 1 )
			{
				// 시간이 지나면 깜빡임을 없앰.
				if( (g_LocalSystemTime - curstring->Get_Attribute(1)) > 3000)//_XDEF_CHATMESSAGEBLINKTIME ) // 2004.06.01->oneway48 insert
				{
					//curstring->Set_Attribute( colorindex, 0 );
					g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);
					if( defaultsystemmsgwindow )
					{
						if( defaultsystemmsgwindow->GetShowStatus() )
							g_XBaseFont->PutsAlign( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_LEFT, curstring->Get_String() );
						else
							g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 6 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );
					}					
				}
				else
				{
					if( (gnFPS/3) < gnFrameCounter )
					{
						g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);
						//g_XBaseFont->Puts( this->m_WindowPosition.x + 20 + offset, (m_WindowPosition.y+4) + (ypos * 12),
						//	                  curstring->Get_String() );
						if( defaultsystemmsgwindow )
						{
							if( defaultsystemmsgwindow->GetShowStatus() )
								g_XBaseFont->PutsAlign( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_LEFT, curstring->Get_String() );
							else
								g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 6 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );
						}						
					}
				}
			}
			else
			{			
				g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);				
				
				if( colorindex == _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL	||
					colorindex == _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS		||
					colorindex == _XDEF_CHATMESSAGECOLOR_SYSTEMGET )
				{
					if( defaultsystemmsgwindow )
					{
						if( defaultsystemmsgwindow->GetShowStatus() )
							g_XBaseFont->PutsAlign( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_LEFT, curstring->Get_String() );
						else
							g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 6 + offset, (m_WindowPosition.y + 5) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );
					}
				}
				else
				{
					if( pfocusobject == this )
					{
						if( curstring->Get_Attribute(1) != 0x0000000 )	// ID 일 경우
						{
							TCHAR idnamebuffer[128];
							memset( idnamebuffer, 0, sizeof(TCHAR) * 128 );
							strncpy( idnamebuffer, curstring->Get_String(), curstring->Get_Attribute(1) );
							
							int width = g_XBaseFont->GetWidth( idnamebuffer );
							
							if( gpInput->CheckMousePos( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 5) + (ypos * 15), 
								this->m_WindowPosition.x + 16 + offset + width, (m_WindowPosition.y + 5) + (ypos * 15) + g_XBaseFont->GetHeight() ) )
							{
								MouseState* mousestate = gpInput->GetMouseState();
								if( mousestate->bButton[0] )
								{
									if( eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() ) 
									{
										_XIMEKERNEL.ResetCombinationBuffer();
#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
										eodapp->m_proc_maingame.m_ChatEditContainer.ClearBuffer();
#endif
										eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();										
#ifdef _XDEF_NEWSTRINGMANAGER
										m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[4], _XFONT_ALIGNTYPE_LEFT, 6 );
#else
										m_btnChatFunction->SetButtonText( g_ChatFunctionButtonString[4] );
#endif
										m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[4] );
										SetChatMode(_XCHATMODE_WHISPER);										
									}
									strcpy( m_WhisperTargetName, idnamebuffer );
									InsertRecentWhisperUserList(idnamebuffer, 3);
								}
								else if ( mousestate->bButton[1] )
								{
									// Menu 띄우기									
								}							
							}
						}
					}					
					
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
					/*
					if( curstring->Get_Attribute(2) ) // black list user
					{
						int strlength = curstring->Get_Strlength();

						if( curstring->Get_Attribute(1) != 0x0000000 )	// ID 일 경우
						{
							TCHAR dummytext[256];
							memset( dummytext, _T('*'), strlength );
							dummytext[strlength] = NULL;

							strncpy( dummytext, curstring->Get_String(), curstring->Get_Attribute(1) + 1 );
							g_XBaseFont->Puts( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 8) + (ypos * 15), dummytext );
						}
						else
						{
							TCHAR dummytext[256];
							LPTSTR pString = curstring->Get_String();
							for( int i = 0; i < strlength ; i++ )
							{
								if( pString[i] == _T(' ') )
								{
									dummytext[i] = _T(' ');
								}
								else
								{
									dummytext[i] = _T('*');
								}
							}

							dummytext[strlength] = NULL;
							g_XBaseFont->Puts( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 8) + (ypos * 15), dummytext );
						}
					}
					else
					{
						g_XBaseFont->Puts( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 8) + (ypos * 15), curstring->Get_String() );
					}
					*/
					g_XBaseFont->Puts( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 5) + (ypos * 15), curstring->Get_String() );
#else
					g_XBaseFont->Puts( this->m_WindowPosition.x + 16 + offset, (m_WindowPosition.y + 5) + (ypos * 15), curstring->Get_String() );
#endif
				}
			}
			
			ypos++;  

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
			}
#endif
			
			if( ypos >= m_MaxViewCount ) break;
			
			g_Chatlist.nextItem();
			
		}while( !g_Chatlist.atHeadOfList() );
	}	
	
	g_XBaseFont->Flush();	

	g_XBaseFont->DisableGrowMode();
	
	for( int i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
	{
		m_WhisperTargetButton[i].Draw( pfocusobject );
	}
	
	m_ChatListScrollPos = m_ChatListScrollBar->GetLineScrollPos();
	m_ChatListScrollBar->Draw( m_pScrbarFocusedObject );
	m_DockingSystemMessageButton.Draw(pfocusobject);

	// 채팅 모드 버튼을 그리는 부분
	// 외치기나 지역채팅 옵션에 따라 버튼을 그릴지 그리지 않을지를 결정한다.
	for( i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
	{
		if( i == _XCHATMODE_SHOUT )
		{
#ifdef _XDEF_CASHITEM_SHOUT	
			m_btnChatFunctionMenuButton[i]->Draw( pfocusobject );
			m_btnimageChatFunctionMenuButton[i]->Draw( pfocusobject );
#endif
		}
		else if( i == _XCHATMODE_ZONE)
		{
#ifdef _XDEF_ZONECHATTING_20070108	
			m_btnChatFunctionMenuButton[i]->Draw( pfocusobject );
			m_btnimageChatFunctionMenuButton[i]->Draw( pfocusobject );
#endif
		}
		else
		{
			m_btnChatFunctionMenuButton[i]->Draw( pfocusobject );
			m_btnimageChatFunctionMenuButton[i]->Draw( pfocusobject );
		}
	}
	
	// Menu edge
	if( m_btnChatFunctionMenuButton[0]->GetShowStatus() )
	{
		// 버튼 테두리 박스를 그리는 부분
		// 해외 버젼에 따라 박스의 넓이가 틀려진다.
		int boxwidth = 92;
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			boxwidth = 117;
		}
		// 각각의 옵션에 따라 박스의 높이가 틀려진다.
#ifdef _XDEF_CASHITEM_SHOUT
#ifdef _XDEF_ZONECHATTING_20070108

		_XDrawRectAngle( m_WindowPosition.x + 1, m_WindowPosition.y + m_WindowSize.cy - 169, 
			m_WindowPosition.x + boxwidth , m_WindowPosition.y + m_WindowSize.cy - 169 + 129+18, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		
		_XDrawRectAngle( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy - 168, 
			m_WindowPosition.x + boxwidth-1 , m_WindowPosition.y + m_WindowSize.cy - 169 + 128+18, 0.0f, 0xFF3D3A33 );
#else
		_XDrawRectAngle( m_WindowPosition.x + 1, m_WindowPosition.y + m_WindowSize.cy - 151, 
			m_WindowPosition.x + boxwidth , m_WindowPosition.y + m_WindowSize.cy - 151 + 129, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		
		_XDrawRectAngle( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy - 150, 
			m_WindowPosition.x + boxwidth-1 , m_WindowPosition.y + m_WindowSize.cy - 151 + 128, 0.0f, 0xFF3D3A33 );
		
#endif//#ifdef _XDEF_ZONECHATTING_20070108
#else
#ifdef _XDEF_ZONECHATTING_20070108
		_XDrawRectAngle( m_WindowPosition.x + 1, m_WindowPosition.y + m_WindowSize.cy - 151, 
			m_WindowPosition.x + boxwidth , m_WindowPosition.y + m_WindowSize.cy - 151 + 129, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		
		_XDrawRectAngle( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy - 150, 
			m_WindowPosition.x + boxwidth-1 , m_WindowPosition.y + m_WindowSize.cy - 151 + 128, 0.0f, 0xFF3D3A33 );
#else
		_XDrawRectAngle( m_WindowPosition.x + 1, m_WindowPosition.y + m_WindowSize.cy - 133, 
			m_WindowPosition.x + boxwidth , m_WindowPosition.y + m_WindowSize.cy - 133 + 111, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		
		_XDrawRectAngle( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy - 132, 
			m_WindowPosition.x + boxwidth-1 , m_WindowPosition.y + m_WindowSize.cy - 133 + 110, 0.0f, 0xFF3D3A33 );

#endif//#ifdef _XDEF_ZONECHATTING_20070108
#endif//#ifdef _XDEF_CASHITEM_SHOUT
	

		int tmp = (int)m_ChatMode;

		if( tmp < _XCHATMODE_TOTALCOUNT )
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_btnChatFunctionMenuButton[tmp]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 177, 116, 195 );
			}
			else
			{
				m_btnChatFunctionMenuButton[tmp]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  124, 18, 212, 36 );
			}
			
			m_btnChatFunctionMenuButton[tmp]->Draw();
			m_btnimageChatFunctionMenuButton[tmp]->SetClipRect( 212 + (tmp<<2), 0, 215 + (tmp<<2), 9  );
			m_btnimageChatFunctionMenuButton[tmp]->Draw();
		}
	}

	eodapp->m_proc_maingame.m_ChatEditContainer.Draw();
	m_WhisperEditContainer.Draw();

	DrawEmotion(pfocusobject);
}

void _XWindow_ChattingDefault::DrawEmotion( _XGUIObject*& pfocusobject )
{
	if( m_EmotionListBox->GetShowStatus() )
	{
		POINT listwinpos;
		SIZE listwinsize;
		
		listwinpos = m_EmotionListBox->GetWindowPos();
		listwinsize = m_EmotionListBox->GetWindowSize();

		_XDrawSolidBar(listwinpos.x - 2, listwinpos.y-7, listwinpos.x+listwinsize.cx+2, listwinpos.y+listwinsize.cy+7, 0xB1000000 );
		g_pDefaultTooltip_Window->DrawTooltipLine(listwinpos.x - 2, listwinpos.y-7, listwinpos.x+listwinsize.cx+2, listwinpos.y+listwinsize.cy+7);
				
		m_EmotionListBox->Draw(pfocusobject);
	}
}

#define _XDEF_RESIZETRACKSIZE	4

BOOL _XWindow_ChattingDefault::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_Enable     ) return FALSE;	
	
	if( m_pScrbarFocusedObject ) return TRUE;
	
	m_ChatListScrollBar->Process(pfocusobject);
	//if( pfocusobject == NULL )
	//	m_MovingChatWindow = TRUE;

	int changeY = m_WindowPosition.y+m_WindowSize.cy;
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
	
	int bottomYPos = gnHeight;
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	if(g_pLocalUser->GetMotionClass() == _XACTION_DIE || g_pLocalUser->GetMotionClass() == _XACTION_DYING)
	{
		// 사망, 빈사상태일 경우 채팅창 아래로 내림
		bottomYPos = gnHeight;
	}
	else
	{
		bottomYPos = gnHeight - 47;
	}
#endif
	
	// chat 창 업다운 ================================================
	if( eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() ) 
	{
		if( m_MovingChatWindow )
		{
			if( changeY-2 < bottomYPos )
			{
				changeY = bottomYPos;
				m_MovingChatWindow = FALSE;
			}
			else
				changeY -= 2;
			
			MoveWindow( 0, changeY-m_WindowSize.cy );
			m_ChatListScrollBar->SetLineScrollPos( 999999 ); 
			
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			if( g_pExp_Window )
			{
				g_MainWindowManager.SetTopWindow( g_pExp_Window );
			}
#endif
		}				
	}
	else
	{
		if( m_MovingChatWindow )
		{
			if( m_ChatMode  == _XCHATMODE_WHISPER )	
			{
				if( changeY+2 > bottomYPos+40 )
				{
					changeY = bottomYPos+40;
					m_MovingChatWindow = FALSE;
					return FALSE;
				}
				else
					changeY+=2;
			}
			else
			{
				if( changeY+2 > bottomYPos+22 )
				{
					changeY = bottomYPos+22;
					m_MovingChatWindow = FALSE;
					return FALSE;
				}
				else
					changeY+=2;
			}			
			
			MoveWindow( 0, changeY-m_WindowSize.cy );
			m_ChatListScrollBar->SetLineScrollPos( 999999 ); 

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			if( g_pExp_Window )
			{
				g_MainWindowManager.SetTopWindow( g_pExp_Window );
			}
#endif
		}		
	}	// ===================================================	

//#ifdef _XDEF_CASHITEM_SHOUT
	for( int i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
//#else
//	for( int i = 0; i < 6; i++ )
//#endif
	{
		m_btnChatFunctionMenuButton[i]->Process();
		m_btnimageChatFunctionMenuButton[i]->Process();
	}

	if( !m_ObjectList.listEmpty() )
	{
		m_ObjectList.resetList();
		_XGUIObject* currentObject = NULL;
		
		do
		{
			currentObject = (_XGUIObject*)m_ObjectList.currentItem();
			
			if(currentObject) currentObject->Process( pfocusobject );
			
			m_ObjectList.nextItem();
		}while( !m_ObjectList.atHeadOfList() );
	}
	
	for( i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
	{
		m_WhisperTargetButton[i].Process();
	}

	m_DockingSystemMessageButton.Process();

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();	
	BOOL changedmousecursor = FALSE;

	// EmotionListBox Process ----------------------------------------=	
	m_EmotionListBox->Process(pfocusobject);
	
	if(mousestate)
	{
		if( pfocusobject != m_EmotionListBox )
		{
			if(mousestate->bButton[0])
			{
				if(!m_EmotionListBox->CheckMousePosition())
					m_EmotionListBox->ShowWindow(FALSE);
			}
		}
	}
	// ---------------------------------------------------------------=
	
	if( mousestate && scrnpos && !m_pScrbarFocusedObject )
	{			
		if( !pfocusobject )
		{
			if( eodapp->m_proc_maingame.m_ChatEditContainer.CheckMousePosition() )
			{
				if( mousestate->bButton[0] )
				{					
					if( (scrnpos->x > this->m_WindowPosition.x + 20 ) && (scrnpos->x < this->m_WindowPosition.x + 290 ) &&
						(scrnpos->z > (this->m_WindowPosition.y + this->m_WindowSize.cy - 38) && (this->m_WindowPosition.y + this->m_WindowSize.cy - 26) ) ) 
					{
						if( eodapp )
						{	
							eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();
						}
					}
				}
			}			
			
			if( CheckMousePosition() )
			{		
				if( mousestate->bButton[0] )
				{					
					if( (scrnpos->x > this->m_WindowPosition.x + 20 ) && (scrnpos->x < this->m_WindowPosition.x + 290 ) &&
						(scrnpos->z > (this->m_WindowPosition.y + this->m_WindowSize.cy - 38) && (this->m_WindowPosition.y + this->m_WindowSize.cy - 26) ) ) 
					{
						if( eodapp )
						{	
							eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();
						}
					}
				}

//#define _XDEF_FULLRESIZEMODE
#ifdef	_XDEF_FULLRESIZEMODE
				if( scrnpos->x < m_WindowPosition.x+_XDEF_RESIZETRACKSIZE )
				{	
					if( scrnpos->z < m_WindowPosition.y+_XDEF_RESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						}											
						
						m_ResizeMode = _XWRESIZE_LEFTBOTTOM;
						changedmousecursor = TRUE;
					}
					else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_RESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y+m_WindowSize.cy - scrnpos->z;
						}
												
						m_ResizeMode = _XWRESIZE_LEFTBOTTOM;
						changedmousecursor = TRUE;
					}
					else
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
						}
												
						m_ResizeMode = _XWRESIZE_LEFT;
						changedmousecursor = TRUE;
					}

				}
				else if( scrnpos->x >= m_WindowPosition.x+m_WindowSize.cx-_XDEF_RESIZETRACKSIZE )
				{	
					if( scrnpos->z < m_WindowPosition.y+_XDEF_RESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						}
												
						m_ResizeMode = _XWRESIZE_RIGHTTOP;
						changedmousecursor = TRUE;
					}
					else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_RESIZETRACKSIZE )
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y+m_WindowSize.cy - scrnpos->z;
						}
						
						m_ResizeMode = _XWRESIZE_RIGHTBOTTOM;
						changedmousecursor = TRUE;
					}
					else
					{
						if( mousestate->bButton[0] )
						{
							pfocusobject = (_XGUIObject*)this;
							m_Resizing = TRUE;												
							m_DragStartPos.x = m_WindowPosition.x+m_WindowSize.cx - scrnpos->x;							
						}
												
						m_ResizeMode = _XWRESIZE_RIGHT;
						changedmousecursor = TRUE;
					}
				}
				else 
#endif				
				if( scrnpos->z < m_WindowPosition.y+_XDEF_RESIZETRACKSIZE )
				{
					if( mousestate->bButton[0] )
					{
						pfocusobject = (_XGUIObject*)this;
						m_Resizing = TRUE;											
						m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
					}
										
					m_ResizeMode = _XWRESIZE_TOP;
					changedmousecursor = TRUE;
				}
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
				//하단측 리사이즈 드래깅 막음.
#else
				else if( scrnpos->z >= m_WindowPosition.y+m_WindowSize.cy-_XDEF_RESIZETRACKSIZE )
				{
					if( mousestate->bButton[0] )
					{
						pfocusobject = (_XGUIObject*)this;
						m_Resizing = TRUE;											
						m_DragStartPos.y = m_WindowPosition.y + m_WindowSize.cy - scrnpos->z;
					}
										
					m_ResizeMode = _XWRESIZE_BOTTOM;
					changedmousecursor = TRUE;
				}					
#endif
				else
				{		
					if( mousestate->bButton[0] )
					{
						pfocusobject = (_XGUIObject*)this;
						if( (m_WindowMoveMode != _XWMOVE_FIXED) && !m_Dragging )
						{					
							m_Dragging = TRUE;
							m_DragStartPos.x = m_WindowPosition.x - scrnpos->x;
							m_DragStartPos.y = m_WindowPosition.y - scrnpos->z;
						}
					}					
				}									
			}			
		}

		// 리사이즈 모드가 아니면 이전 마우스 포인터 복구
		if( ( !pfocusobject || pfocusobject == this ) && (changedmousecursor || m_Resizing) )
		{
			switch( m_ResizeMode ) {
			case _XWRESIZE_LEFTTOP :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_DIAGONAL_LEFT );
				break;
			case _XWRESIZE_RIGHTTOP :				
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_DIAGONAL_RIGHT );
				break;
			case _XWRESIZE_LEFTBOTTOM :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_DIAGONAL_RIGHT );				
				break;
			case _XWRESIZE_RIGHTBOTTOM :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_DIAGONAL_LEFT );
				break;
			case _XWRESIZE_LEFT :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_HORIZ );
				break;
			case _XWRESIZE_TOP :
				eodapp->ReadyCursor( _XMOUSETYPE_CHATRESIZE );
				break;
			case _XWRESIZE_RIGHT :
				eodapp->ReadyCursor( _XMOUSETYPE_RESIZE_HORIZ );
				break;
			case _XWRESIZE_BOTTOM :	
				eodapp->ReadyCursor( _XMOUSETYPE_CHATRESIZE );
				break;			
			default:
				break;
			}			
		}
		
		if( !mousestate->bButton[0] )
		{
			m_Dragging = FALSE;	
			m_Resizing = FALSE;
			m_DragStartPos.x = m_DragStartPos.y = 0;			

			if( CheckMousePosition() )
			{	
				if( pfocusobject == (_XGUIObject*)this )
				{
					if( m_CommandID )
					{						
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(m_CommandID, 0), (LPARAM)gHWnd);
						mousestate->bButton[0] = 0;
					}						
					pfocusobject = NULL;
					return TRUE;
				}		
			}
			if( pfocusobject == (_XGUIObject*)this )
			{
				pfocusobject = NULL;
				return FALSE;
			}			
		}
		else if( pfocusobject == (_XGUIObject*)this )
		{
			if( m_Resizing )
			{				
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_CHATRESIZE );

				RECT winrect;
				switch(m_ResizeMode) 
				{
				case _XWRESIZE_LEFTTOP:
					winrect.left   = scrnpos->x+m_DragStartPos.x; 
					winrect.top    = scrnpos->z+m_DragStartPos.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy; 
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_RIGHTTOP:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = scrnpos->z+m_DragStartPos.y;
					winrect.right  = scrnpos->x+m_DragStartPos.x;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_LEFTBOTTOM:
					winrect.left   = scrnpos->x+m_DragStartPos.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = scrnpos->z+m_DragStartPos.y;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_RIGHTBOTTOM:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = scrnpos->x+m_DragStartPos.x;
					winrect.bottom = scrnpos->z+m_DragStartPos.y;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_TOP:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = scrnpos->z+m_DragStartPos.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_BOTTOM:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = scrnpos->z+m_DragStartPos.y;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_LEFT:
					winrect.left   = scrnpos->x+m_DragStartPos.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = m_WindowPosition.x+m_WindowSize.cx;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				case _XWRESIZE_RIGHT:
					winrect.left   = m_WindowPosition.x;
					winrect.top    = m_WindowPosition.y;
					winrect.right  = scrnpos->x+m_DragStartPos.x;
					winrect.bottom = m_WindowPosition.y+m_WindowSize.cy;
					ResizeWindow( winrect );
					break;
				default:
					break;
				}
			}
			else if( m_Dragging )
			{				
				int newxpos = scrnpos->x + m_DragStartPos.x;
				int newypos = scrnpos->z + m_DragStartPos.y;
				
				if( newxpos < 0	) newxpos = 0;
				if( newypos < 0	) newypos = 0;				
				if( newxpos+m_WindowSize.cx >= gnWidth ) newxpos = gnWidth - m_WindowSize.cx;
				if( newypos+m_WindowSize.cy >= bottomYPos) newypos = bottomYPos- m_WindowSize.cy;
				
				switch( m_WindowMoveMode )
				{
				case _XWMOVE_FREE :
					WindowPosChanging( newxpos, newypos );
					MoveWindow( newxpos, newypos );
					break;
				case _XWMOVE_HORIZ :
					WindowPosChanging( newxpos, (int&)m_WindowPosition.y );
					MoveWindow( newxpos, this->m_WindowPosition.y );
					break;
				case _XWMOVE_VERT :								
					WindowPosChanging( (int&)this->m_WindowPosition.x, newypos );
					MoveWindow( this->m_WindowPosition.x, newypos );
					break;
				}				

				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_PICKUP_CLICK );
			}
		}
	}

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503

	if( eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() ) 
	{
		if( !m_LockChatFilterMenu )
		{
			if( gpInput->CheckMousePos( m_WindowPosition.x, m_WindowPosition.y - 17, m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y-1 ) )
			{
				m_pLockChatFilterMenuButton->ShowWindow( TRUE );
				for( int i = 0; i < 4; i++ )
				{
					if( m_pChatFilterButton[i] )
						m_pChatFilterButton[i]->ShowWindow( TRUE );
				}
			}
			else
			{
				m_pLockChatFilterMenuButton->ShowWindow( FALSE );
				for( int i = 0; i < 4; i++ )
				{
					if( m_pChatFilterButton[i] )
						m_pChatFilterButton[i]->ShowWindow( FALSE );
				}
			}
		}
		else
		{
			m_pLockChatFilterMenuButton->ShowWindow( TRUE );
			for( int i = 0; i < 4; i++ )
			{
				if( m_pChatFilterButton[i] )
					m_pChatFilterButton[i]->ShowWindow( TRUE );
			}
		}
	}
	else
	{
		m_pLockChatFilterMenuButton->ShowWindow( FALSE );
		for( int i = 0; i < 4; i++ )
		{
			if( m_pChatFilterButton[i] )
				m_pChatFilterButton[i]->ShowWindow( FALSE );
		}
	}

#endif

	return TRUE;
}

BOOL _XWindow_ChattingDefault::OnKeyDown(WPARAM wparam, LPARAM lparam)
{
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;						
	if( eodapp )
	{	
		if( !eodapp->m_proc_maingame.m_ChatEditContainer.GetShowStatus() )
			return FALSE;
	}
	
	switch( (U32)wparam )
	{
	case VK_UP  :
		{
 			if( m_EmotionListBox->GetShowStatus() )
			{
				m_CurrentSeletedEmotion--;
				if( m_CurrentSeletedEmotion < 0 )
					m_CurrentSeletedEmotion = 9;

				m_EmotionListBox->SetSelectedItem( m_CurrentSeletedEmotion );
			}
			else
			{
				m_ChatListScrollBar->LineUp( abs(3) );
			}			
			return TRUE;
		}
		break;
	case VK_DOWN :
		{
			if( m_EmotionListBox->GetShowStatus() )
			{
				m_CurrentSeletedEmotion++;
				if( m_CurrentSeletedEmotion > 9 )
					m_CurrentSeletedEmotion = 0;
				
				m_EmotionListBox->SetSelectedItem( m_CurrentSeletedEmotion );
			}
			else
			{
				m_ChatListScrollBar->LineDown( abs(-3) );
			}
			return TRUE;
		}
		break;
	}
	return FALSE;
}

BOOL _XWindow_ChattingDefault::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( m_ChatListScrollBar )
	{
		if( m_ChatListScrollBar->ProcessWheelMessage( zDelta ) )
		{
			return TRUE;
		}
		
		if( CheckMousePosition() )
		{
			int MoveDelta  = (FLOAT)zDelta / 40;			
			
			if( zDelta > 0 )
			{
				m_ChatListScrollBar->LineUp( abs(MoveDelta) );
			}
			else
			{
				m_ChatListScrollBar->LineDown( abs(MoveDelta) );
			}
			return TRUE;				
		}
	}
	
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_ChattingDefault::SetChatMode(_XCHATMODE chatmode)
{
	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( pMainGameProcess->m_ChatEditContainer.GetShowStatus() && pMainGameProcess->m_ChatEditContainer.GetFocus() )
	{
		_XIMEKERNEL.ResetCombinationBuffer();

#ifndef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
		pMainGameProcess->m_ChatEditContainer.ClearBuffer();
#endif
		
//#ifdef _XDEF_ZONECHATTING_20070108
//		if( g_bEnableZoneChatting )
//		{
//			g_bProgressZoneChatting = FALSE;	// 지역 대화인지 아닌지를 알 수 있도록 한다. 
//			m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);
//		}
//#endif
		switch(chatmode) 
		{
		case _XCHATMODE_EMOTION :
			break;
#ifdef _XDEF_CASHITEM_SHOUT
		case _XCHATMODE_SHOUT :
			#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			#endif
			pMainGameProcess->m_ChatEditContainer.AddString( "%", 1);
			break;
#endif
		case _XCHATMODE_CLAN :
			#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			#endif
			break;
		case _XCHATMODE_SUBCLAN :
			#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			#endif
			
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
			{
				if(m_HeroBandChatMode)
				{
					int orgnamelength = strlen(g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName);
					if( orgnamelength > 0 )
					{						
						TCHAR herobandnamestring[128];
						sprintf( herobandnamestring, _T("&(%s)"), g_pLocalUser->m_CharacterInfo.m_OrganizationInfo.m_cName );
						
						if( pMainGameProcess->m_ChatEditContainer.GetText()[0] != _T('&') )
						{
							pMainGameProcess->m_ChatEditContainer.AddString( herobandnamestring, strlen(herobandnamestring), FALSE );
						}
					}
					else
					{
						pMainGameProcess->m_ChatEditContainer.AddString( "&", 1, FALSE );
					}
				}
				else
				{
					pMainGameProcess->m_ChatEditContainer.AddString( "@", 1, FALSE );
				}
			}
#else				
			pMainGameProcess->m_ChatEditContainer.AddString( "@", 1);
#endif
			break;
		case _XCHATMODE_PARTY :
			#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			#endif
			pMainGameProcess->m_ChatEditContainer.AddString( "!", 1);
			break;
		case _XCHATMODE_WHISPER :
			{
			#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			#endif
				if( strlen( m_WhisperTargetName ) )
				{
					TCHAR tempstr[_XDEF_MAX_USERNAMESTRINGLENGTH];
					sprintf( tempstr, "/%s ", m_WhisperTargetName );
					pMainGameProcess->m_ChatEditContainer.AddString( tempstr, strlen(m_WhisperTargetName)+2 );					
				}
				else
					pMainGameProcess->m_ChatEditContainer.AddString( "/", 1);
			}			
			break;			
#ifdef _XDEF_ZONECHATTING_20070108
		case _XCHATMODE_ZONE :
			{
			#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			#endif
				pMainGameProcess->m_ChatEditContainer.AddString( "*", 1);
				// 지역 대화인지 아닌지를 알 수 있도록 한다. 
				// 지역 대화인 경우 딜레이 시간 동안 전체대화로 변경 되어야 한다. 
				g_bProgressZoneChatting = TRUE;
			}
			break;
#endif
		case _XCHATMODE_NORMAL :
			#ifdef ALL_RB_CHATTING_KEEPTEXT_070712_KERYGMA
				pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			#endif
			break;		
		}
	}

	m_ChatMode = chatmode;

	if( m_ChatMode == _XCHATMODE_WHISPER )
	{
		m_WhisperStaticButton->ShowWindow( TRUE );
		m_EmotionListBox->ShowWindow( FALSE );
		for( int i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
		{
			m_WhisperTargetButton[i].ShowWindow( TRUE );
		}

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 63 );
		//m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 51 );

		//m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 53 );
		m_ChatListScrollBar->SetWindowHeight( m_DockingSystemMessageButton.GetWindowPos().y - m_WindowPosition.y );

		m_MaxViewCount = ((m_WindowSize.cy-62) / 15);
		if(m_pChatBlackListButton)
			m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+25, m_WindowPosition.y + m_WindowSize.cy - 63 );
			//m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+12, m_WindowPosition.y + m_WindowSize.cy - 51 );
#else
		m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 53 );
		m_MaxViewCount = ((m_WindowSize.cy-50) / 15);
#endif
		
		m_ChatListScrollBar->SetViewLineCount( m_MaxViewCount );
		m_ChatListScrollBar->RebuildTrackBar();
#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		m_ChatListScrollBar->MoveWindow( m_WindowPosition.x,m_WindowPosition.y + 1 );
#else
		m_ChatListScrollBar->MoveWindow( m_WindowPosition.x+1,m_WindowPosition.y + 1 );
#endif
	}
	else
	{
		if( m_ChatMode == _XCHATMODE_EMOTION && m_btnChatFunctionMenuButton[0]->GetShowStatus() )
		{
			m_EmotionListBox->SetSelectedItem( m_CurrentSeletedEmotion );
			m_EmotionListBox->ShowWindow( TRUE );
		}
		else
			m_EmotionListBox->ShowWindow( FALSE );

		m_WhisperStaticButton->ShowWindow( FALSE );
		for( int i = 0; i < _XDEF_MAXWHISPERTARGETCOUNT; i++ )
		{
			m_WhisperTargetButton[i].ShowWindow( FALSE );
		}

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 46 );
		//m_DockingSystemMessageButton.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y + m_WindowSize.cy - 34 );

		m_ChatListScrollBar->SetWindowHeight( m_DockingSystemMessageButton.GetWindowPos().y - m_WindowPosition.y );
		//m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 36 );

		m_MaxViewCount = ((m_WindowSize.cy-46) / 15);
		if(m_pChatBlackListButton)
			m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+25, m_WindowPosition.y + m_WindowSize.cy - 46 );
			//m_pChatBlackListButton->MoveWindow( m_WindowPosition.x+12, m_WindowPosition.y + m_WindowSize.cy - 34 );
#else
		m_ChatListScrollBar->SetWindowHeight( m_WindowSize.cy - 38 );
		m_MaxViewCount = ((m_WindowSize.cy-34) / 15);
#endif		
		m_ChatListScrollBar->SetViewLineCount( m_MaxViewCount );
		m_ChatListScrollBar->RebuildTrackBar();

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
		m_ChatListScrollBar->MoveWindow( m_WindowPosition.x,m_WindowPosition.y + 1 );
#else
		m_ChatListScrollBar->MoveWindow( m_WindowPosition.x+1,m_WindowPosition.y + 1 );
#endif
	}

	m_ChatListScrollBar->SetLineScrollPos(99999);
//#ifdef _XDEF_CASHITEM_SHOUT
	for( int i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
//#else
//	for( int i = 0; i < 6; i++ )
//#endif
	{
		if( m_ChatMode != i )
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 177, 116, 195 );
			}
			else
			{
				m_btnChatFunctionMenuButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 124, 0, 212, 18 );
			}
		}
	}
	
#ifdef _XDEF_NEWSTRINGMANAGER
	m_btnChatFunction->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, g_ChatFunctionButtonStringID[m_ChatMode], _XFONT_ALIGNTYPE_LEFT, 6 );
#else
	m_btnChatFunction->SetButtonText( g_ChatFunctionButtonString[m_ChatMode] );
#endif
	m_btnChatFunction->SetButtonTextColor( g_ChatFunctionButtonStringColor[m_ChatMode] );
}

void _XWindow_ChattingDefault::ShowChatFunctionMenu( BOOL show )
{
//#ifdef _XDEF_CASHITEM_SHOUT
	for( int i = 0; i < _XCHATMODE_TOTALCOUNT; i++ )
//#else
//	for( int i = 0; i < 6; i++ )
//#endif
	{
		if( i == _XCHATMODE_SHOUT )
		{
#ifdef _XDEF_CASHITEM_SHOUT	
			m_btnChatFunctionMenuButton[i]->ShowWindow( show );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( show );
#else
			m_btnChatFunctionMenuButton[i]->ShowWindow( FALSE );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( FALSE );
#endif
		}
		else if( i == _XCHATMODE_ZONE)
		{
#ifdef _XDEF_ZONECHATTING_20070108	
			m_btnChatFunctionMenuButton[i]->ShowWindow( show );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( show );
#else
			m_btnChatFunctionMenuButton[i]->ShowWindow( FALSE );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( FALSE );
#endif
		}
		else
		{
			m_btnChatFunctionMenuButton[i]->ShowWindow( show );
			m_btnimageChatFunctionMenuButton[i]->ShowWindow( show );
		}
	}
	
#ifdef _XDEF_ZONECHATTING_20070108			
	if( !show ) return;
	if( g_bEnableZoneChatting )
		m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(TRUE);
	else
		m_btnChatFunctionMenuButton[_XCHATMODE_ZONE]->EnableWindow(FALSE);
#endif
}

void _XWindow_ChattingDefault::InsertRecentWhisperUserList( LPTSTR szUsername, int target )
{
	_XStringItem* pStringItem = NULL;
	_XStringItem* pFirstItem = NULL;
	int cnt = 0;
	
	if( m_RecentChatList.getitemcount() >= _XDEF_MAXWHISPERTARGETCOUNT && target < 3 )
	{
		m_RecentChatList.resetList();
		m_RecentChatList.prevItem();			
		do 
		{
			pStringItem = (_XStringItem*)m_RecentChatList.currentItem();
			
			if( cnt == m_SelectedWhisperTarget )
			{
				m_WhisperTargetButton[cnt].SetButtonText( szUsername );
				pStringItem->Set_String(szUsername);
				
				m_WhisperTargetButton[0].SetCheck( FALSE );
				m_WhisperTargetButton[1].SetCheck( FALSE );
				m_WhisperTargetButton[2].SetCheck( FALSE );
				m_WhisperTargetButton[cnt].SetCheck( TRUE );
				return;
			}
			cnt++;
			m_RecentChatList.prevItem();
		} while( !m_RecentChatList.atEndOfList() );
	}

	// Check already inserted...
	if( !m_RecentChatList.listEmpty() )
	{
		m_RecentChatList.resetList();
		m_RecentChatList.prevItem();
		pFirstItem = (_XStringItem*)m_RecentChatList.currentItem();
		do 
		{
			pStringItem = (_XStringItem*)m_RecentChatList.currentItem();			
			
			if( strcmpi( pStringItem->Get_String(), szUsername ) == 0 )
			{
				if( cnt != 0 )
				{
					TCHAR tbuf[128];
					memset( tbuf, 0, sizeof(TCHAR) * 128 );
					strcpy(tbuf, pFirstItem->Get_String() );
					
					m_WhisperTargetButton[0].SetButtonText( szUsername );
					pFirstItem->Set_String(szUsername);					
					m_WhisperTargetButton[cnt].SetButtonText( tbuf );
					pStringItem->Set_String(tbuf);
				}
				
				m_WhisperTargetButton[0].SetCheck( TRUE );
				m_WhisperTargetButton[1].SetCheck( FALSE );
				m_WhisperTargetButton[2].SetCheck( FALSE );
				return;
			}
			cnt++;
			m_RecentChatList.prevItem();
		} while( !m_RecentChatList.atEndOfList() );
	}
	
	
	if( m_RecentChatList.getitemcount() < _XDEF_MAXWHISPERTARGETCOUNT )
	{
		m_RecentChatList.resetList();
		m_RecentChatList.insertItem( new _XStringItem( szUsername , 0, 0 ) );
	}
	else
	{
		m_RecentChatList.resetList();
		m_RecentChatList.deleteItem( 0 );
		
		m_RecentChatList.resetList();
		m_RecentChatList.insertItem( new _XStringItem( szUsername , 0, 0 ) );
	}
	
	// Rebuild button text	
	if( !m_RecentChatList.listEmpty() )
	{
		cnt = 0;
		
		m_RecentChatList.resetList();
		m_RecentChatList.prevItem();		
		
		do 
		{
			pStringItem = (_XStringItem*)m_RecentChatList.currentItem();
			
			m_WhisperTargetButton[cnt].SetButtonText( pStringItem->Get_String() );
			
			cnt++;			
			if( cnt >= _XDEF_MAXWHISPERTARGETCOUNT ) break;
			
			m_RecentChatList.prevItem();
		} while( !m_RecentChatList.atEndOfList() );
	}
	
	m_WhisperTargetButton[0].SetCheck( TRUE );
	m_WhisperTargetButton[1].SetCheck( FALSE );
	m_WhisperTargetButton[2].SetCheck( FALSE );
	m_SelectedWhisperTarget = 0;
}

void _XWindow_ChattingDefault::SelectWhisperTargetTab( int index )
{
	if( index >= _XDEF_MAXWHISPERTARGETCOUNT ) return;
	
	if( m_WhisperEditContainer.GetFocus() ) 
	{
		m_WhisperTargetButton[0].SetCheck( FALSE );
		m_WhisperTargetButton[1].SetCheck( FALSE );
		m_WhisperTargetButton[2].SetCheck( FALSE );
		m_WhisperTargetButton[m_SelectedWhisperTarget].SetCheck( TRUE );
		return;
	}	


	m_WhisperTargetButton[0].SetCheck( FALSE );
	m_WhisperTargetButton[1].SetCheck( FALSE );
	m_WhisperTargetButton[2].SetCheck( FALSE );
	m_WhisperTargetButton[index].SetCheck( TRUE );
	
	if( index > -1 && index < _XDEF_MAXWHISPERTARGETCOUNT )
	{
		if( m_SelectedWhisperTarget == index )		// rename
		{
			if( !m_WhisperEditContainer.GetFocus() )
			{
				memset( m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
				if( m_WhisperTargetButton[index].GetButtonText() )
				{
					strcpy( m_SavedWhisperTargetName, m_WhisperTargetButton[index].GetButtonText() );
					m_WhisperTargetButton[index].SetButtonText(_T(""));
				}				

				m_WhisperEditContainer.ClearBuffer();				
				m_WhisperEditContainer.MoveWindow( m_WindowPosition.x+95+(index*93), m_WindowPosition.y + m_WindowSize.cy - 34 );
				m_WhisperEditContainer.ShowWindow( TRUE );
				m_WhisperEditContainer.SetFocus();

				SetChangeWhisperName(index);
			}			
		}
		else
		{
			SetChangeWhisperName(index);
		}
	}
	else
	{
		memset( m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );		
	}
}

void _XWindow_ChattingDefault::SetChangeWhisperName( int changeindex )
{
	XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( pMainGameProcess )
	{
		if( pMainGameProcess->m_ChatEditContainer.GetShowStatus() && pMainGameProcess->m_ChatEditContainer.GetFocus() )
		{
			_XIMEKERNEL.ResetCombinationBuffer();
			pMainGameProcess->m_ChatEditContainer.ClearBuffer();
			
			m_SelectedWhisperTarget = changeindex;	
			TCHAR tmpbuf[_XDEF_MAX_USERNAMESTRINGLENGTH];
			memset( tmpbuf, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH);
			if( m_WhisperTargetButton[m_SelectedWhisperTarget].GetButtonText() )
				strcpy( tmpbuf, m_WhisperTargetButton[m_SelectedWhisperTarget].GetButtonText() );
			
			if( strlen(tmpbuf) )
			{
				memset( m_WhisperTargetName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
				strcpy( m_WhisperTargetName, tmpbuf );
				
				if( strlen( m_WhisperTargetName ) )
				{
					TCHAR tempstr[_XDEF_MAX_USERNAMESTRINGLENGTH];
					sprintf( tempstr, "/%s ", m_WhisperTargetName );
					pMainGameProcess->m_ChatEditContainer.AddString( tempstr, strlen(m_WhisperTargetName)+2 );					
				}
				else
					pMainGameProcess->m_ChatEditContainer.AddString( "/", 1);			
			}			
		}
	}	
}

void _XWindow_ChattingDefault::SetPositionDie(BOOL bDie)
{
	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;	
	if( eodapp->m_proc_maingame.m_ChatEditContainer.GetFocus() ) 
	{
		if(bDie)
		{
			// 빈사, 혼수 상태
			MoveWindow(0, gnHeight-m_WindowSize.cy);
		}
		else
		{
			MoveWindow(0, gnHeight-47-m_WindowSize.cy);
		}
	}
	else
	{
		if(bDie)
		{
			// 빈사, 혼수 상태
			MoveWindow(0, gnHeight-m_WindowSize.cy+22);
		}
		else
		{
			MoveWindow(0, gnHeight-47-m_WindowSize.cy+22);
		}
	}
}
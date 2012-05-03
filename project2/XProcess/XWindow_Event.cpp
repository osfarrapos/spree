// XWindow_Event.cpp: implementation of the _XWindow_Event class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "XProc_ProcessDefine.h"
#include "XMessageWindow.h"

#include "XWindow_EventIcon.h"
#include "XWindow_Event.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_SerialNumberReturn( _XIMEContainer* pIMEContainer )
{
	_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
	if( !pEvent_Window ) return;

	if( !pEvent_Window->m_IMESerialNumber.GetShowStatus() ) return;
	if( pEvent_Window->m_IMESerialNumber.GetLength() == 0 ) return;
	
	POINT pt = pEvent_Window->m_IMESerialNumber.GetWindowPosition();
	POINT winpos = pEvent_Window->GetWindowPos();
	
	
	if( pt.x == winpos.x+18 )
	{
		if( !pEvent_Window->CheckSerialNumber(0) ) return;
		
		memset( pEvent_Window->m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[0] , pEvent_Window->m_IMESerialNumber.GetText());
		
		memset( pEvent_Window->m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+73, winpos.y+293 );
		pEvent_Window->m_IMESerialNumber.SetMaxLength(pEvent_Window->m_SelectedMaxSerialNumber[1]);
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();
		pEvent_Window->m_IMESerialNumber.SetFocus();
	}
	else if( pt.x == winpos.x+73 )
	{
		if( !pEvent_Window->CheckSerialNumber(1) ) return;
		
		memset( pEvent_Window->m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[1] , pEvent_Window->m_IMESerialNumber.GetText());
		
		memset( pEvent_Window->m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+128, winpos.y+293 );
		pEvent_Window->m_IMESerialNumber.SetMaxLength(pEvent_Window->m_SelectedMaxSerialNumber[2]);
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();
		pEvent_Window->m_IMESerialNumber.SetFocus();
	}
	else if( pt.x == winpos.x+128 )
	{
		if( !pEvent_Window->CheckSerialNumber(2) ) return;
		
	
		memset( pEvent_Window->m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[2] , pEvent_Window->m_IMESerialNumber.GetText());
		
		memset( pEvent_Window->m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+183, winpos.y+293 );
		pEvent_Window->m_IMESerialNumber.SetMaxLength(pEvent_Window->m_SelectedMaxSerialNumber[3]);
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();
		pEvent_Window->m_IMESerialNumber.SetFocus();
	}
	else if( pt.x == winpos.x+183 )
	{
		if( !pEvent_Window->CheckSerialNumber(3) ) return;
		
		
		memset( pEvent_Window->m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[3] , pEvent_Window->m_IMESerialNumber.GetText());
		
		memset( pEvent_Window->m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+238, winpos.y+293 );
		pEvent_Window->m_IMESerialNumber.SetMaxLength(pEvent_Window->m_SelectedMaxSerialNumber[4]);
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();
		pEvent_Window->m_IMESerialNumber.SetFocus();
	}
	else if( pt.x == winpos.x+238 )
	{
		if( !pEvent_Window->CheckSerialNumber(4) ) return;
		
		
		memset( pEvent_Window->m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[4] , pEvent_Window->m_IMESerialNumber.GetText());
		
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+1, winpos.y+293 );

		pEvent_Window->m_IMESerialNumber.ClearBuffer();	
		_XIMEKERNEL.SetFocus( NULL );	
		_XIMEKERNEL.ResetCombinationBuffer();	
	//	pEvent_Window->m_IMESerialNumber.SetFocus();
	}
}

void __stdcall _XIMECallback_SerialNumberTab( _XIMEContainer* pIMEContainer )
{
	_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
	if( !pEvent_Window ) return;
	
	if( !pEvent_Window->m_IMESerialNumber.GetShowStatus() ) return;
	if( pEvent_Window->m_IMESerialNumber.GetLength() == 0 ) return;
	
	POINT pt = pEvent_Window->m_IMESerialNumber.GetWindowPosition();
	POINT winpos = pEvent_Window->GetWindowPos();
	
	
	if( pt.x == winpos.x+18 )
	{
		if( !pEvent_Window->CheckSerialNumber(0) ) return;
		
		memset( pEvent_Window->m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[0] , pEvent_Window->m_IMESerialNumber.GetText());
		
		memset( pEvent_Window->m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+73, winpos.y+293 );
		pEvent_Window->m_IMESerialNumber.SetMaxLength(pEvent_Window->m_SelectedMaxSerialNumber[1]);
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();
		pEvent_Window->m_IMESerialNumber.SetFocus();
	}
	else if( pt.x == winpos.x+73 )
	{
		if( !pEvent_Window->CheckSerialNumber(1) ) return;
		
		memset( pEvent_Window->m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[1] , pEvent_Window->m_IMESerialNumber.GetText());
		
		memset( pEvent_Window->m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+128, winpos.y+293 );
		pEvent_Window->m_IMESerialNumber.SetMaxLength(pEvent_Window->m_SelectedMaxSerialNumber[2]);
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();
		pEvent_Window->m_IMESerialNumber.SetFocus();
	}
	else if( pt.x == winpos.x+128 )
	{
		if( !pEvent_Window->CheckSerialNumber(2) ) return;
		
		
		memset( pEvent_Window->m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[2] , pEvent_Window->m_IMESerialNumber.GetText());
		
		memset( pEvent_Window->m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+183, winpos.y+293 );
		pEvent_Window->m_IMESerialNumber.SetMaxLength(pEvent_Window->m_SelectedMaxSerialNumber[3]);
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();
		pEvent_Window->m_IMESerialNumber.SetFocus();
	}
	else if( pt.x == winpos.x+183 )
	{
		if( !pEvent_Window->CheckSerialNumber(3) ) return;
		
		
		memset( pEvent_Window->m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[3] , pEvent_Window->m_IMESerialNumber.GetText());
		
		memset( pEvent_Window->m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+238, winpos.y+293 );
		pEvent_Window->m_IMESerialNumber.SetMaxLength(pEvent_Window->m_SelectedMaxSerialNumber[4]);
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();
		pEvent_Window->m_IMESerialNumber.SetFocus();
	}
	else if( pt.x == winpos.x+238 )
	{
		if( !pEvent_Window->CheckSerialNumber(4) ) return;
		
		
		memset( pEvent_Window->m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
		strcpy( pEvent_Window->m_SerialNumber[4] , pEvent_Window->m_IMESerialNumber.GetText());
		
		pEvent_Window->m_IMESerialNumber.MoveWindow( winpos.x+1, winpos.y+293 );
		
		pEvent_Window->m_IMESerialNumber.ClearBuffer();	
		_XIMEKERNEL.SetFocus( NULL );	
		_XIMEKERNEL.ResetCombinationBuffer();	
		//	pEvent_Window->m_IMESerialNumber.SetFocus();
	}
}

_XWindow_Event::_XWindow_Event() : m_bGetItem(NULL)
{	
	m_listboxEventTitle			= NULL;	// 이벤트 목록을 보여주는 리스트 박스
	m_listboxEventDescription	= NULL;	// 이벤트 설명을 보여주는 리스트 박스
	
	m_imageIMEBorderSerialNumber		= NULL;

	m_btnSerialNumberOK			= NULL;

	memset( m_SelectedEventName, 0, sizeof(m_SelectedEventName));
	memset( m_SelectedEventTerm, 0, sizeof(m_SelectedEventTerm));
	memset( m_SerialNumber, 0, sizeof(m_SerialNumber) );
	memset( m_EventInfo, 0, sizeof(m_EventInfo)); //Author : 양희왕 추가
	
	m_SelectedEventID	= -1;
	m_SelectedEventType = -1;
	m_nEventIDinfo = 0;
	m_bBuffEvent = FALSE;
	m_bFirstSetListItem = FALSE;

	for( int i = 0; i < 5; ++i )
	{
		m_SelectedMaxSerialNumber[i] = -1;
	}

}

_XWindow_Event::~_XWindow_Event()
{
	DestroyWindow();
}

BOOL _XWindow_Event::Initialize( void )
{
	int mainframeinterface			= g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int chattingbuttonindex			= g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	int chartextureinterface		= g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int npcresourceindex1			= g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
	int lifeskillresourceindex		= g_MainInterfaceTextureArchive.FindResource( "MI_LifeSkill.tga" );
	int optionmenuinterface			= g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );	

	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 153, 23,
						   &g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleLeftBar->SetClipRect( 0, 0, 154, 24 );
	InsertChildObject( pTitleLeftBar );

	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create( 307, 0, 307, 23,
							&g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleRightBar->SetClipRect( 0, 0, 154, 24 );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );	

	
	_XBTN_STRUCTURE helpbtnstruct = { TRUE, { 275, 5 }, { 12, 12 }, _XDEF_EVENT_BTN_HELP,
									  chartextureinterface,
									  chartextureinterface,
									  chartextureinterface,
									  &g_MainInterfaceTextureArchive };
	// 도움말 버튼 
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpbtnstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );

	helpbtnstruct.commandid = _XDEF_EVENT_BTN_CLOSE;
	helpbtnstruct.position.x += 13;

	// 닫기 버튼
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( helpbtnstruct );	
		
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );	

	//받기 버튼 //Author : 양희왕 
	_XBTN_STRUCTURE Getbtnstruct = 
	{ 
		TRUE, { 123, 300  }, { 60, 24 }, 
		_XDEF_EVENT_BTN_GET, optionmenuinterface, optionmenuinterface, optionmenuinterface, &g_MainInterfaceTextureArchive
	};
	m_bGetItem = new _XButton;
	m_bGetItem->Create( Getbtnstruct );	
	m_bGetItem->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
#ifdef _XTAIWANESE
	m_bGetItem->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3846);				// _T("교환하기-대만")
#else
	m_bGetItem->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCRESOURCE_2265);		// _T("교환하기")
#endif
	m_bGetItem->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	m_bGetItem->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );
	m_bGetItem->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_bGetItem->ShowWindow(FALSE);
	InsertChildObject( m_bGetItem );

	
	
	// 이벤트 목록을 보여주는 리스트 박스
	_XLISTBOX_STRUCTURE queststructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 9, 38 },		// 윈도우 좌표
		{ 290, 102 },	// 윈도우 사이즈
		_XDEF_EVENT_LISTBOX_TITLE,							// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		17,												// 리스트 아이템 라인 피치
		6,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 104 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		50,													// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	m_listboxEventTitle = new _XListBox;
	m_listboxEventTitle->Create(queststructure);
	
	_XVScrollBar* pScrollBar = m_listboxEventTitle->GetScrollBarObject();
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
	}
	
	POINT columnoffset1 = { 5, 1 };
	POINT columnoffset2 = { 20, 3 };
	m_listboxEventTitle->SetColumnOffset(0, columnoffset1);				// column offset
	m_listboxEventTitle->SetColumnOffset(1, columnoffset2);
	
	m_listboxEventTitle->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxEventTitle->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	
	m_listboxEventTitle->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxEventTitle->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxEventTitle->SetColumnWidth(0, 278);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxEventTitle->SetColumnWidth(1, 278);
	
	m_listboxEventTitle->SetSelectedImageArchive(npcresourceindex1, &g_MainInterfaceTextureArchive);	
	_XImageStatic* selectbar = m_listboxEventTitle->GetSelectBarObject();
	selectbar->SetClipRect( 196, 490, 470, 506 );
	m_listboxEventTitle->SetSelectedImagePosition(5, 1);
	m_listboxEventTitle->SetSelectedImageHeight(17);
	m_listboxEventTitle->SetTrackWidth(220);
	
	m_listboxEventTitle->ShowWindow(FALSE);
	InsertChildObject(m_listboxEventTitle);
	
	// 퀘스트 설명에 사용될 리스트 박스
	_XLISTBOX_STRUCTURE eventdescstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 9, 198 },										// 윈도우 좌표
		{ 290, 90 },									// 윈도우 사이즈
		_XDEF_EVENT_LISTBOX_DESCRIPTION,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		15,												// 리스트 아이템 라인 피치
		6,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 90 },										// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		50,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	m_listboxEventDescription = new _XListBox;
	m_listboxEventDescription->Create(eventdescstructure);
	
	pScrollBar = m_listboxEventDescription->GetScrollBarObject();
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
	}
	//m_listboxEventDescription->SetAlwaysShowScrollBarMode(TRUE);
	//m_listboxEventDescription->SetMoveScrollBarMode(TRUE);
	//m_listboxEventDescription->MoveScrollBarPos( 324, -82 );
	
	POINT desccolumnoffset1 = { 20, 2 };
	
	m_listboxEventDescription->SetColumnOffset(0, desccolumnoffset1);		// column offset	
	m_listboxEventDescription->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);	// column style (아이콘)	
	m_listboxEventDescription->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type	
	m_listboxEventDescription->SetColumnWidth(0, 257);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxEventDescription->EnableSelectItem(FALSE);
	m_listboxEventDescription->ShowWindow(FALSE);
	InsertChildObject(m_listboxEventDescription);
	

	// 바탕 타일 생성
	m_BorderTile.Create( m_WindowPosition.x + 4, m_WindowPosition.y + 24, m_WindowPosition.x + 301, m_WindowPosition.y + 337);
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );

	FLOAT ufactor = 301.0f/128.0f;
	FLOAT vfactor = 337.0f/128.0f;

	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );

	m_BorderGradationTitle.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 305, m_WindowPosition.y + 20, 
								  &g_MainInterfaceTextureArchive, mainframeinterface );
	m_BorderGradationTitle.SetClipRect( 206, 227, 244, 246 );
	m_BorderGradationTitle.SetScale( 312.0f/39.0f, 1.0f );
	
	
	// 시리얼 번호 입력창
	m_IMESerialNumber.ResetContainer();
	m_IMESerialNumber.MoveWindow(m_WindowPosition.x+1, m_WindowPosition.y+293);
	m_IMESerialNumber.ResizeWindow(50, 20);
	m_IMESerialNumber.SetMaxLength(6);
	m_IMESerialNumber.SetAlignType( _XFONT_ALIGNTYPE_CENTER );
	m_IMESerialNumber.SetTextColor(_XSC_DEFAULT_BLACK);	
	m_IMESerialNumber.SetCaretColor(_XSC_DEFAULT_BLACK);
	m_IMESerialNumber.SetReturnKeyCallback( _XIMECallback_SerialNumberReturn );
	m_IMESerialNumber.SetTabKeyCallback( _XIMECallback_SerialNumberTab );
	m_IMESerialNumber.ShowWindow(FALSE);
		
	// IME 바탕 이미지 
	m_imageIMEBorderSerialNumber = new _XImageStatic;
	m_imageIMEBorderSerialNumber->Create( 18, 289, 68, 300, &g_MainInterfaceTextureArchive, lifeskillresourceindex );
	m_imageIMEBorderSerialNumber->SetClipRect( 136, 73, 186, 93);
	m_imageIMEBorderSerialNumber->ShowWindow(FALSE);
	InsertChildObject( m_imageIMEBorderSerialNumber);
	
	// 시리얼 번호 입력 완료 버튼 
	_XBTN_STRUCTURE okbtnstruct = { TRUE, { 122, 310 }, { 62, 26 }, _XDEF_EVENT_BTN_OK,
		optionmenuinterface,optionmenuinterface,optionmenuinterface, 
		&g_MainInterfaceTextureArchive };
	m_btnSerialNumberOK = new _XButton;
	m_btnSerialNumberOK->Create( okbtnstruct );
	m_btnSerialNumberOK->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 186, 126, 248, 152 );
	m_btnSerialNumberOK->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  186, 151, 248, 177 );
	m_btnSerialNumberOK->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  186, 176, 248, 202 );
	m_btnSerialNumberOK->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK ); // 확인
	m_btnSerialNumberOK->ShowWindow(FALSE);
	InsertChildObject( m_btnSerialNumberOK );	

#ifdef _XDEF_EVENTWINDOW
	// 이벤트 정보 읽기 
	LoadEventInfo();

	// 이벤트 정보 세팅 
	SetListItem();
#endif

	return TRUE;
}


BOOL _XWindow_Event::LoadEventInfo( void )
{	
	_XMEMORYUSECHECKREADY

	_XMEMORYUSECHECKSTART
		
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
	FILE* fileptr = NULL;	
	fileptr = g_ScriptArchive.GetPackedFile( _T("event_main.txt") );
	
	if( !fileptr ) return FALSE;
	
	TCHAR buffer[1024];
	TCHAR tempnbuffer1[128];
	TCHAR tempnbuffer2[128];
	int tempindex = 0;
	_XEvnetInfo* neweventinfo = NULL;
	int linecount = 0;

	memset(tempnbuffer1, 0, sizeof(tempnbuffer1));
	memset(tempnbuffer2, 0, sizeof(tempnbuffer2));
	do 
	{
		if(feof(fileptr))
			break;
		
		memset(buffer, 0, sizeof(TCHAR)*1024);
		if(!fgets(buffer, 1024, fileptr))
			break;
		
		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if( linecount == 0 )
			{
				neweventinfo = new _XEvnetInfo;
				sscanf( buffer, "%s %d", 
					tempnbuffer1,						// Event
					&neweventinfo->eventid				// Event ID
					);
				++linecount;

			}
			else if( linecount == 1)
			{
				sscanf( buffer, "%s %d", 
					tempnbuffer1,						// Type
					&neweventinfo->eventtype			// Event Type
					);
				++linecount;
			}
			else if( linecount == 2)
			{
				if(neweventinfo->eventtype == 6) //Author : 양희왕 //breif : 타입 6 시간까지 계산 //date : 08/07/31
				{
					sscanf( buffer, "%s %d %d %d %d %d %d %d %d", 
						tempnbuffer1,						// Date
						&neweventinfo->startyear,			// Start Year
						&neweventinfo->startmonth,			// Start Month
						&neweventinfo->startday,			// Start Day
						&neweventinfo->nStartTime,
						&neweventinfo->endyear,				// End Year
						&neweventinfo->endmonth,			// End Month
						&neweventinfo->endday,				// End Day
						&neweventinfo->nEndTime
						);
				}
				else
				{
					sscanf( buffer, "%s %d %d %d %d %d %d", 
						tempnbuffer1,						// Date
						&neweventinfo->startyear,			// Start Year
						&neweventinfo->startmonth,			// Start Month
						&neweventinfo->startday,			// Start Day
						&neweventinfo->endyear,				// End Year
						&neweventinfo->endmonth,			// End Month
						&neweventinfo->endday				// End Day
						);
				}
				++linecount;
			}
			else if( linecount == 3)
			{
				sscanf( buffer, "%s %s", 
					tempnbuffer1,						// Name
					neweventinfo->eventname				// Event Name
				);
				
				for(int i = 0; i < 512; i++)
				{
					if( neweventinfo->eventname[i] == '_' )
						neweventinfo->eventname[i] = ' ';			
				}	
				++linecount;					  
			}
			else if( linecount == 4)
			{
				sscanf( buffer, "%s %s", 
					tempnbuffer1,						// Name
					neweventinfo->eventdes				// Event Description
					);
				for( int i = 0; i < 512; i++)
				{
					if( neweventinfo->eventdes[i] == '_' )
						neweventinfo->eventdes[i] = ' ';		
					
					if( neweventinfo->eventdes[i] == '/' )
						neweventinfo->eventdes[i] = '\n';
				}
				++linecount;
			}
			else if( linecount == 5)
			{
				sscanf( buffer, "%s %d %d %d %d %d ", 
					tempnbuffer1,						// Slot
					&neweventinfo->serialslot[0],		// Slot1
					&neweventinfo->serialslot[1],		// Slot2
					&neweventinfo->serialslot[2],		// Slot3
					&neweventinfo->serialslot[3],		// Slot4
					&neweventinfo->serialslot[4]		// Slot5
					);
				++linecount;
			}
			//Author : 양희왕 //breif : 레벨업 이벤트
			else if( linecount == 6)
			{
				if(neweventinfo->eventtype == 6)
				{
					sscanf( buffer, "%s %d %d", tempnbuffer1, &neweventinfo->nPackageIndex, &neweventinfo->nLevel );
					m_listEventInfo.push_back(neweventinfo);
					linecount = 0;
				}
			#if defined(_XDEF_LEVELEVENT_080319_MAGVIPER) || defined(_XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI)
				else if(neweventinfo->eventtype == 4)
				{
					sscanf( buffer, "%s %d", tempnbuffer1, &neweventinfo->nPackageIndex	);
					++linecount;
				}
				else if(neweventinfo->eventtype == 5)
				{
					sscanf( buffer, "%s %d", tempnbuffer1, &neweventinfo->nPackageIndex	);
					m_listEventInfo.push_back(neweventinfo);
					linecount = 0;
				}
			#endif
				else
				{
					sscanf( buffer, "%s %s", tempnbuffer1, tempnbuffer2	);
					++linecount;
				}
			}
			else if( linecount == 7)
			{
			#ifdef _XDEF_LEVELEVENT_080319_MAGVIPER 
				if(neweventinfo->eventtype == 4 )
				{
					sscanf( buffer, "%s %d", tempnbuffer1, &neweventinfo->nLevel );			
					m_listEventInfo.push_back(neweventinfo);
					linecount = 0;
				}
				else
			#endif
				{
					sscanf( buffer, "%s %s %d", tempnbuffer1, tempnbuffer2, &tempindex);
					++linecount;
				}
			}
			else if( linecount == 8)
			{
				sscanf( buffer, "%s %s", tempnbuffer1, tempnbuffer2	);
				++linecount;
			}
			else if( linecount == 9)
			{
				sscanf( buffer, "%s %s %d", tempnbuffer1,tempnbuffer2, &tempindex);
				m_listEventInfo.push_back(neweventinfo);
				linecount = 0;
			}	
		}
	} while(!feof(fileptr));
	
	
	g_ScriptArchive.ClosePackage();
	_XMEMORYUSECHECKEND("Load eventinfo package script");
	return TRUE;
}


void _XWindow_Event::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow(X, Y);

	m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 306, m_WindowPosition.y + 342 );
	m_BorderGradationTitle.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24 );
//	m_IMESerialNumber.MoveWindow(m_WindowPosition.x+18, m_WindowPosition.y+293);
}

void _XWindow_Event::ShowWindow( BOOL show )
{
	if( show )
	{
		SetListItem();	
	}
	else
	{
		memset( m_SerialNumber, 0, sizeof(TCHAR)*(5*8) );
		m_IMESerialNumber.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y+293 );
		m_IMESerialNumber.ClearBuffer();
						
	}
	
	m_btnSerialNumberOK->EnableWindow(show);
	m_IMESerialNumber.ShowWindow( show );
	_XWindow::ShowWindow( show );
}

void _XWindow_Event::DestroyWindow( void )
{
	memset( m_SelectedEventName, 0, sizeof(m_SelectedEventName));
	memset( m_SelectedEventTerm, 0, sizeof(m_SelectedEventTerm));
	memset( m_SerialNumber, 0, sizeof(m_SerialNumber) );
	memset( m_EventInfo, 0, sizeof(m_EventInfo)); //Author : 양희왕 추가
	
	for( int i = 0; i < 5; ++i )
	{
		m_SelectedMaxSerialNumber[i] = -1;
	}

	_XEvnetInfo* peventinfo = NULL;
	list <_XEvnetInfo*>::iterator iter_eventinfo;
	for(iter_eventinfo = m_listEventInfo.begin() ; iter_eventinfo != m_listEventInfo.end() ; ++iter_eventinfo)
	{	
		peventinfo = *iter_eventinfo;
		if(peventinfo)
		{
			SAFE_DELETE(peventinfo);
			iter_eventinfo = m_listEventInfo.erase(iter_eventinfo);
		}
	}

	m_listEventInfo.clear();
	
	_XWindow::DestroyWindow();
}

void _XWindow_Event::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));		

		// Draw Border, Line ---------------------------------------------------------------------=
		_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+23, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0.0f,0xFF000000 );
		_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+24, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f,0xFF9F9A84 );
		_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+25, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f,0xFF040000 );
		_XDrawRectAngle( m_WindowPosition.x+3, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f,0xFF1A211A );
		// ---------------------------------------------------------------------------------------=		
		m_BorderGradationTitle.Draw( pfocusobject );
		
		// 이벤트 목록이 들어갈 박스 
		_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+33, m_WindowPosition.x+302, m_WindowPosition.y+146, 0x7F000400 );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+34, m_WindowPosition.x+300, m_WindowPosition.y+144, 0.0f, 0xFF9F9A84 );		
		_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+35, m_WindowPosition.x+299, m_WindowPosition.y+143, 0.0f,0xFF302923 );		
		
		// 이벤트 목록이 들어갈 박스 
		_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+148, m_WindowPosition.x+302, m_WindowPosition.y+338, 0x7F000400 );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+149, m_WindowPosition.x+300, m_WindowPosition.y+337, 0.0f, 0xFF9F9A84 );		
		_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+150, m_WindowPosition.x+299, m_WindowPosition.y+336, 0.0f,0xFF302923 );		


	}
	else
		return;

	_XWindow::Draw(pfocusobject);	

	// Title Name
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFD7D7D7);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3046) );

	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
	if( m_SelectedEventName[0] != 0 &&  m_SelectedEventTerm[0] != 0 )
	{
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 138, 84));
		g_XBaseFont->Puts( m_WindowPosition.x + 18, m_WindowPosition.y + 158, m_SelectedEventName );
		
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 112, 174, 94));
		g_XBaseFont->Puts( m_WindowPosition.x + 18, m_WindowPosition.y + 178, m_SelectedEventTerm );
		
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
	}

	if( m_SelectedEventType == 0 )
	{
		for( int i = 0; i < 5; ++i )
		{
			if( m_SelectedMaxSerialNumber[i] != 0 )
				m_imageIMEBorderSerialNumber->Draw( m_WindowPosition.x+18+(i*55), m_WindowPosition.y+289 );
		//	m_imageIMEBorderSerialNumber->Draw( m_WindowPosition.x+73, m_WindowPosition.y+289 );
		//	m_imageIMEBorderSerialNumber->Draw( m_WindowPosition.x+128, m_WindowPosition.y+289 );
		//	m_imageIMEBorderSerialNumber->Draw( m_WindowPosition.x+183, m_WindowPosition.y+289 );
		//	m_imageIMEBorderSerialNumber->Draw( m_WindowPosition.x+238, m_WindowPosition.y+289 );
		}

		if( m_IMESerialNumber.GetShowStatus() )
		{
			m_IMESerialNumber.Draw();
		}

		// 시리얼 넘버를 표시하는 부분
		g_XBaseFont->SetColor(_XSC_DEFAULT_BLACK);
		for(  i=0; i<5; ++i )
		{
			if( m_SerialNumber[i] )
			{
				if( m_SerialNumber[i][0] != 0 )
				{
					g_XBaseFont->PutsAlign( m_WindowPosition.x + 43+(i*55), m_WindowPosition.y + 293, _XFONT_ALIGNTYPE_CENTER, m_SerialNumber[i] );
				}
			}
		}
	}
	
	g_XBaseFont->Flush();
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
}


BOOL _XWindow_Event::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{			
			if(mousestate->bButton[0])//마우스 왼쪽 클릭시 
			{
				m_WindowMouseState = _XW_STATE_LEFTCLICK;
			}//마우스 왼쪽 클릭하고 있을 때 처리 끝 
		}
	}
	
	
	if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
	{			
		if(!mousestate->bButton[0])
		{	
						
			bool checknumber = false;
			if( m_IMESerialNumber.GetShowStatus() )
			{
				if(CheckSerialNumberBox1())
				{
					POINT pt = m_IMESerialNumber.GetWindowPosition();
					POINT winpos = GetWindowPos();
					if( pt.x == winpos.x+18 )
					{
						CheckSerialNumber(0);
						memset( m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
						strcpy( m_SerialNumber[0] , m_IMESerialNumber.GetText());
						checknumber = true;
//						if( CheckSerialNumber(0) )
//						{
//							memset( m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
//							strcpy( m_SerialNumber[0] , m_IMESerialNumber.GetText());
//							checknumber = true;
//						}		
					
					}
					else if( pt.x == winpos.x+73 )
					{
						if( CheckSerialNumber(1) )
						{
							memset( m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[1] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+128 )
					{
						if( CheckSerialNumber(2) )
						{
							memset( m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[2] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+183 )
					{
						if( CheckSerialNumber(3) )
						{
							memset( m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[3] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+238 )
					{
						if( CheckSerialNumber(4) ) 
						{
							memset( m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[4] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else
					{
						checknumber = true;
					}

					if( checknumber )
					{
						m_IMESerialNumber.ClearBuffer();
						
						memset( m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
						m_IMESerialNumber.MoveWindow( m_WindowPosition.x+18, m_WindowPosition.y+293 );
						m_IMESerialNumber.SetMaxLength(m_SelectedMaxSerialNumber[0]);
						m_IMESerialNumber.SetFocus();
					}
				}
				else if(CheckSerialNumberBox2())
				{
					POINT pt = m_IMESerialNumber.GetWindowPosition();
					POINT winpos = GetWindowPos();
					if( pt.x == winpos.x+18 )
					{
						if( CheckSerialNumber(0) )
						{
							memset( m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[0] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+73 )
					{
						CheckSerialNumber(1);
						memset( m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
						strcpy( m_SerialNumber[1] , m_IMESerialNumber.GetText());
						checknumber = true;
//						if( CheckSerialNumber(1) )
//						{
//							memset( m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
//							strcpy( m_SerialNumber[1] , m_IMESerialNumber.GetText());
//							checknumber = true;
//						}
					}
					else if( pt.x == winpos.x+128 )
					{
						if( CheckSerialNumber(2) )
						{
							memset( m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[2] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+183 )
					{
						if( CheckSerialNumber(3) )
						{
							memset( m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[3] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+238 )
					{
						if( CheckSerialNumber(4) )
						{
							memset( m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[4] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else
					{
						checknumber = true;
					}

					
					if( checknumber )
					{
						m_IMESerialNumber.ClearBuffer();
						
						memset( m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
						m_IMESerialNumber.MoveWindow( m_WindowPosition.x+73, m_WindowPosition.y+293 );
						m_IMESerialNumber.SetMaxLength(m_SelectedMaxSerialNumber[1]);
						m_IMESerialNumber.SetFocus();
					}
				}
				else if(CheckSerialNumberBox3())
				{
					POINT pt = m_IMESerialNumber.GetWindowPosition();
					POINT winpos = GetWindowPos();
					if( pt.x == winpos.x+18 )
					{
						if( CheckSerialNumber(0) )
						{
							memset( m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[0] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+73 )
					{
						if( CheckSerialNumber(1) )
						{
							memset( m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[1] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+128 )
					{
						CheckSerialNumber(2);
						memset( m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
						strcpy( m_SerialNumber[2] , m_IMESerialNumber.GetText());
						checknumber = true;
//						if( CheckSerialNumber(2) )
//						{
//							memset( m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
//							strcpy( m_SerialNumber[2] , m_IMESerialNumber.GetText());
//							checknumber = true;
//						}		
					}
					else if( pt.x == winpos.x+183 )
					{
						if( CheckSerialNumber(3) )
						{
							memset( m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[3] , m_IMESerialNumber.GetText());
							checknumber = true;
						}		
					}
					else if( pt.x == winpos.x+238 )
					{
						if( CheckSerialNumber(4) )
						{
							memset( m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[4] , m_IMESerialNumber.GetText());
							checknumber = true;
						}		
					}
					else
					{
						checknumber = true;
					}

					
					if( checknumber )
					{
						m_IMESerialNumber.ClearBuffer();
						
						memset( m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
						m_IMESerialNumber.MoveWindow( m_WindowPosition.x+128, m_WindowPosition.y+293 );
						m_IMESerialNumber.SetMaxLength(m_SelectedMaxSerialNumber[2]);
						m_IMESerialNumber.SetFocus();
					}
				}
				else if(CheckSerialNumberBox4())
				{
					POINT pt = m_IMESerialNumber.GetWindowPosition();
					POINT winpos = GetWindowPos();
					if( pt.x == winpos.x+18 )
					{
						if( CheckSerialNumber(0) )
						{
							memset( m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[0] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+73 )
					{
						if( CheckSerialNumber(1) )
						{
							memset( m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[1] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+128 )
					{
						if( CheckSerialNumber(2) )
						{
							memset( m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[2] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+183 )
					{
						CheckSerialNumber(3);
						memset( m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
						strcpy( m_SerialNumber[3] , m_IMESerialNumber.GetText());
						checknumber = true;
//						if( CheckSerialNumber(3) )
//						{
//							memset( m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
//							strcpy( m_SerialNumber[3] , m_IMESerialNumber.GetText());
//							checknumber = true;
//						}		
					}
					else if( pt.x == winpos.x+238 )
					{
						if( CheckSerialNumber(4) ) 
						{
							memset( m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[4] , m_IMESerialNumber.GetText());
							checknumber = true;
						}		
					}
					else
					{
						checknumber = true;
					}

					
					if( checknumber )
					{
						m_IMESerialNumber.ClearBuffer();
						
						memset( m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
						m_IMESerialNumber.MoveWindow( m_WindowPosition.x+183, m_WindowPosition.y+293 );
						m_IMESerialNumber.SetMaxLength(m_SelectedMaxSerialNumber[3]);
						m_IMESerialNumber.SetFocus();
					}
				}
				else if(CheckSerialNumberBox5())
				{
					POINT pt = m_IMESerialNumber.GetWindowPosition();
					POINT winpos = GetWindowPos();
					if( pt.x == winpos.x+18 )
					{
						if( CheckSerialNumber(0) )
						{
							memset( m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[0] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+73 )
					{
						if( CheckSerialNumber(1) )
						{
							memset( m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[1] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+128 )
					{
						if( CheckSerialNumber(2) ) 
						{
							memset( m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[2] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+183 )
					{
						if( CheckSerialNumber(3) )
						{
							memset( m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
							strcpy( m_SerialNumber[3] , m_IMESerialNumber.GetText());
							checknumber = true;
						}
					}
					else if( pt.x == winpos.x+238 )
					{
						CheckSerialNumber(4);
						memset( m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
						strcpy( m_SerialNumber[4] , m_IMESerialNumber.GetText());
						checknumber = true;
//						if( CheckSerialNumber(4) )
//						{
//							memset( m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
//							strcpy( m_SerialNumber[4] , m_IMESerialNumber.GetText());
//							checknumber = true;
//						}
					}
					else
					{
						checknumber = true;
					}

					
					if( checknumber )
					{
						m_IMESerialNumber.ClearBuffer();
						
						memset( m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
						m_IMESerialNumber.MoveWindow( m_WindowPosition.x+238, m_WindowPosition.y+293 );
						m_IMESerialNumber.SetMaxLength(m_SelectedMaxSerialNumber[4]);
						m_IMESerialNumber.SetFocus();
					}
				}
				else
				{
					POINT pt = m_IMESerialNumber.GetWindowPosition();
					POINT winpos = GetWindowPos();
					if( pt.x == winpos.x+18 )
					{
						if( m_SerialNumber[0][0] == 0 )
						{
							if( CheckSerialNumber(0) ) 
							{
								memset( m_SerialNumber[0], 0, sizeof(TCHAR)*8 );
								strcpy( m_SerialNumber[0] , m_IMESerialNumber.GetText());
								checknumber = true;
							}
						}						
					}
					else if( pt.x == winpos.x+73 )
					{
						if( m_SerialNumber[1][0] == 0 )
						{
							if( CheckSerialNumber(1) )
							{
								memset( m_SerialNumber[1], 0, sizeof(TCHAR)*8 );
								strcpy( m_SerialNumber[1] , m_IMESerialNumber.GetText());
								checknumber = true;
							}
						}						
					}
					else if( pt.x == winpos.x+128 )
					{						
						if( m_SerialNumber[2][0] == 0 )
						{
							if( CheckSerialNumber(2) )
							{
								memset( m_SerialNumber[2], 0, sizeof(TCHAR)*8 );
								strcpy( m_SerialNumber[2] , m_IMESerialNumber.GetText());
								checknumber = true;
							}
						}						
					}
					else if( pt.x == winpos.x+183 )
					{
						if( m_SerialNumber[3][0] == 0 )
						{
							if( CheckSerialNumber(3) ) 
							{
								memset( m_SerialNumber[3], 0, sizeof(TCHAR)*8 );
								strcpy( m_SerialNumber[3] , m_IMESerialNumber.GetText());
								checknumber = true;
							}
						}						
					}
					else if( pt.x == winpos.x+238 )
					{
						if( m_SerialNumber[4][0] == 0 )
						{
							if( CheckSerialNumber(4) )
							{
								memset( m_SerialNumber[4], 0, sizeof(TCHAR)*8 );
								strcpy( m_SerialNumber[4] , m_IMESerialNumber.GetText());
								checknumber = true;
							}
						}
					}
					else
					{
						checknumber = true;
					}


					if( checknumber )
					{
						m_IMESerialNumber.MoveWindow( m_WindowPosition.x+1, m_WindowPosition.y+293 );
						m_IMESerialNumber.ClearBuffer();
						_XIMEKERNEL.SetFocus( NULL );	
						_XIMEKERNEL.ResetCombinationBuffer();	
					}
				}
			}			
		}
	}
	
	if(!mousestate->bButton[0])
	{
		if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
			m_WindowMouseState = _XW_STATE_NONE;
	}
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}
BOOL _XWindow_Event::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
			
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_Event::SetListItem(void)
{
	SYSTEMTIME TimeInfo;
	GetLocalTime( &TimeInfo );
	TCHAR tempLastConnectDate[64];
	memset( tempLastConnectDate, 0, sizeof(TCHAR) * 64 );
	sprintf( tempLastConnectDate, "%d%d%d", TimeInfo.wYear, TimeInfo.wMonth, TimeInfo.wDay );			
			

	//Author : 양희왕 //breif : 초기화 추가
	m_bGetItem->ShowWindow(FALSE);
	m_imageIMEBorderSerialNumber->ShowWindow(FALSE);
	m_btnSerialNumberOK->ShowWindow(FALSE);
	memset( m_SelectedEventName, 0, sizeof(m_SelectedEventName));
	memset( m_SelectedEventTerm, 0, sizeof(m_SelectedEventTerm));
	memset( m_SerialNumber, 0, sizeof(m_SerialNumber) );
	m_SelectedEventID	= -1;
	m_SelectedEventType = -1;
	m_listboxEventTitle->DeleteAllItem();	
	m_listboxEventDescription->DeleteAllItem();
	m_listboxEventDescription->SetScrollPos(0);
	m_listboxEventDescription->ShowWindow(FALSE);

	int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
	RECT rect = {238, 3, 251, 19}; //파란색 두루마기 이미지 							
	
	//Author : 양희왕 //breif : 날짜 계산 변경
	int nCurrentDay = GetTotalCurrentDay( TimeInfo.wYear, TimeInfo.wMonth, TimeInfo.wDay );  //현재 날짜

	int count = 0;
	bool checkdate = false;
	bool bchecktype = false; //Author : 양희왕 //breif : 타입 4는 한번만 넣는다.
	// 리스트 아이템 텍스트 삽입				
	_XEvnetInfo* currenteventinfo = NULL;
	list <_XEvnetInfo*>::iterator iter_eventinfo;
	for(iter_eventinfo = m_listEventInfo.begin() ; iter_eventinfo != m_listEventInfo.end() ; ++iter_eventinfo)
	{
		currenteventinfo = *iter_eventinfo;
		if( currenteventinfo )
		{
			checkdate = false;
			
			//Author : 양희왕   //breif : 레벨 선물 이벤트  Date :08/03/19
			if(currenteventinfo->eventtype == 4 )
			{
				if( currenteventinfo->eventid == m_nEventIDinfo ) //서버에서 받은 이벤트 넘버와 같은 번호라면
				{
					//날짜 계산을 하지 않는다.
					checkdate = true;
				}
			}
			else
			{
			#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘
				if( currenteventinfo->eventtype == 2 || currenteventinfo->eventtype == 3 )
				{
					bool bGiveItem = false;
					for(int i = 0; i < 8; i++)
					{
						if( currenteventinfo->eventid == m_EventInfo[i].nEventID && m_EventInfo[i].bGiveItem )
						{
							bGiveItem = true;
							break;
						}

					}

					if( bGiveItem ) //아이템을 받았으니 목록에 넣지 않는다.
						continue;
				}
			#endif

				int nStartDay = GetTotalCurrentDay( currenteventinfo->startyear, currenteventinfo->startmonth, currenteventinfo->startday );
				int nEndDay = GetTotalCurrentDay( currenteventinfo->endyear, currenteventinfo->endmonth, currenteventinfo->endday);

				if( nStartDay <= nCurrentDay && nEndDay >= nCurrentDay ) // 현재 날짜가 이벤트 날짜 안에 속한다
				{
					//Author : 양희왕 //brief : 6번은 아이콘을 띄워줘야함,, 그리고 리스트 목록에 처음 들어온다
					if( currenteventinfo->eventtype == 6 && !m_bFirstSetListItem) 
					{
						m_bBuffEvent = TRUE;
					}
					
					checkdate = true;
				}
			}
			
			if( checkdate )
			{
				m_listboxEventTitle->InsertListItem(_T(""), 0, currenteventinfo->eventid);			
				m_listboxEventTitle->SetIconArchive(count, &g_MainInterfaceTextureArchive);
				m_listboxEventTitle->SetItemAttrib(count, 0,questresourceindex2, rect);
				m_listboxEventTitle->SetItemText(count, 1,currenteventinfo->eventname );
				m_listboxEventTitle->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_listboxEventTitle->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
				++count;
			}
		}
	}	
	m_listboxEventTitle->ShowWindow(TRUE);

	m_bFirstSetListItem = TRUE;
}

int _XWindow_Event::GetTotalCurrentDay( int nYear, int nMonth, int nDay )
{
	int nCurrentDay = 0;
	int nMonthDay[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    nCurrentDay = ( nYear - 1 ) * 365 ;
    // 윤년 횟수를 더해줌.
    // 4년마다 한번씩 윤년, 100년에 한번씩 윤년 아님, 400년에 한번씩 윤년임
    // 올해는 제외 해야 하니 year-1
    nCurrentDay = nCurrentDay + ( nYear - 1 ) / 4 - ( nYear - 1 ) / 100 + ( nYear - 1 ) / 400;


    //올해 윤년 체크
    if ( nYear % 4 == 0 )
    {
        nMonthDay[2] = 29;

        if ( nYear % 100 == 0 && nYear % 400 != 0 )
            nMonthDay[2] = 28;
    }

    // 저번 달까지 모두 더해줌.
    for (int i = 1 ; i< nMonth ; i ++ )
        nCurrentDay += nMonthDay[i];

    // 이번달에서 지난 날들을 더해줌
    nCurrentDay += nDay;

	return nCurrentDay;
}

void _XWindow_Event::SetSelectedListItme( int selectedeventid)
{
	
	_XEvnetInfo* currenteventinfo = NULL;
	list <_XEvnetInfo*>::iterator iter_eventinfo;
	for(iter_eventinfo = m_listEventInfo.begin() ; iter_eventinfo != m_listEventInfo.end() ; ++iter_eventinfo)
	{
		currenteventinfo = *iter_eventinfo;
		if( currenteventinfo )
		{
			if( currenteventinfo->eventid == selectedeventid )
			{
				memset( m_SelectedEventName, 0, sizeof(TCHAR)*512 );
				memset( m_SelectedEventTerm, 0, sizeof(TCHAR)*512 );

				strcpy( m_SelectedEventName, currenteventinfo->eventname );
				//sprintf( m_SelectedEventTerm, _T("<기간> %d년 %d월 %d일 ~ %d년 %d월 %d일"),
				//					currenteventinfo->startyear,currenteventinfo->startmonth, currenteventinfo->startday,
				//					currenteventinfo->endyear,currenteventinfo->endmonth, currenteventinfo->endday);
				
				// 영문 베트남의 경우 어순처리 필요.
				TCHAR argstring[256];
				sprintf( argstring, _T("%s ~ %s"), _XGETINTERFACETEXT( ID_STRING_ORGANIZATION_2102 ), _XGETINTERFACETEXT( ID_STRING_ORGANIZATION_2102 ) );
				
				sprintf( m_SelectedEventTerm, argstring,
					currenteventinfo->startyear,currenteventinfo->startmonth, currenteventinfo->startday,
					currenteventinfo->endyear,currenteventinfo->endmonth, currenteventinfo->endday);
				
				if( currenteventinfo->eventdes[0] != 0 )
				{
					_XStringSplitInfo splitinfo;
					g_XBaseFont->SplitString(currenteventinfo->eventdes, 257, splitinfo, _XSTRINGSPLITFUNCTION_SPACECHAR);
					
					TCHAR msgbuffer[512];
					int linecount = 0;
					
					m_listboxEventDescription->DeleteAllItem();
					m_listboxEventDescription->SetScrollPos(0);
					
					for(int i = 0 ; i < splitinfo.splitcount ; ++i)
					{
						memset(msgbuffer, 0, sizeof(msgbuffer));
						strncpy(msgbuffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
						m_listboxEventDescription->InsertListItem(_T(""), 0, 0);
						m_listboxEventDescription->SetItemText(linecount, 0, msgbuffer);
						m_listboxEventDescription->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
						linecount++;
					}
					
					m_listboxEventDescription->SetScrollPos(0);
					m_listboxEventDescription->SetLineScrollPos(0);
				}

				for( int index = 0; index < 5; ++index )
				{					
					m_SelectedMaxSerialNumber[index] = currenteventinfo->serialslot[index];
				}
				m_SelectedEventType = currenteventinfo->eventtype;
				if( m_SelectedEventType == 0 ) // 시리얼 번호를 넣는 종류
				{
					m_imageIMEBorderSerialNumber->ShowWindow( TRUE );
					m_IMESerialNumber.ShowWindow(TRUE);
					m_btnSerialNumberOK->ShowWindow(TRUE);

				#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘
					m_bGetItem->ShowWindow(FALSE);
				#endif
				}
				else if(m_SelectedEventType == 5 || m_SelectedEventType == 1 || m_SelectedEventType == 6)
				{
					// 아무것도 안나오게
					m_imageIMEBorderSerialNumber->ShowWindow( FALSE );
					m_IMESerialNumber.ShowWindow(FALSE);
					m_btnSerialNumberOK->ShowWindow(FALSE);

				#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘
					m_bGetItem->ShowWindow(FALSE);
				#endif
				}
				else
				{
					m_imageIMEBorderSerialNumber->ShowWindow( FALSE );
					m_IMESerialNumber.ShowWindow(FALSE);
					m_btnSerialNumberOK->ShowWindow(FALSE);

				#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : 양희왕 //breif : 이벤트 리뉴얼 아이콘
					m_bGetItem->ShowWindow(TRUE);
				#endif
				}

				
				break;
			}
		}
	}

	m_listboxEventDescription->ShowWindow(TRUE);
}

BOOL _XWindow_Event::CheckSerialNumberBox1(void)
{
	if( m_SelectedMaxSerialNumber[0] == 0 ) return FALSE;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 18) &&
		scrnpos->x < (m_WindowPosition.x + 18 + 50) &&
		scrnpos->z > (m_WindowPosition.y + 289) &&
		scrnpos->z < (m_WindowPosition.y + 289 + 20))
		return TRUE;
	
	return FALSE;
}

BOOL _XWindow_Event::CheckSerialNumberBox2(void)
{
	if( m_SelectedMaxSerialNumber[1] == 0 ) return FALSE;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 73) &&
		scrnpos->x < (m_WindowPosition.x + 73 + 50) &&
		scrnpos->z > (m_WindowPosition.y + 289) &&
		scrnpos->z < (m_WindowPosition.y + 289 + 20))
		return TRUE;
	
	return FALSE;
}

BOOL _XWindow_Event::CheckSerialNumberBox3(void)
{
	if( m_SelectedMaxSerialNumber[2] == 0 ) return FALSE;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 128) &&
		scrnpos->x < (m_WindowPosition.x + 128 + 50) &&
		scrnpos->z > (m_WindowPosition.y + 289) &&
		scrnpos->z < (m_WindowPosition.y + 289 + 20))
		return TRUE;
	
	return FALSE;
}

BOOL _XWindow_Event::CheckSerialNumberBox4(void)
{
	if( m_SelectedMaxSerialNumber[3] == 0 ) return FALSE;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 183) &&
		scrnpos->x < (m_WindowPosition.x + 183 + 50) &&
		scrnpos->z > (m_WindowPosition.y + 289) &&
		scrnpos->z < (m_WindowPosition.y + 289 + 20))
		return TRUE;
	
	return FALSE;
}

BOOL _XWindow_Event::CheckSerialNumberBox5(void)
{
	if( m_SelectedMaxSerialNumber[4] == 0 ) return FALSE;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 238) &&
		scrnpos->x < (m_WindowPosition.x + 238 + 50) &&
		scrnpos->z > (m_WindowPosition.y + 289) &&
		scrnpos->z < (m_WindowPosition.y + 289 + 20))
		return TRUE;
	
	return FALSE;
}

BOOL _XWindow_Event::CheckSerialNumber( int serialslotnumber )
{
	if( strlen( m_IMESerialNumber.GetText()) !=  m_SelectedMaxSerialNumber[serialslotnumber] )
	{
		TCHAR argstring[256];
		sprintf( argstring, _XGETINTERFACETEXT( ID_STRING_NEW_3047 ), m_SelectedMaxSerialNumber[serialslotnumber]);
				
		g_NetworkKernel.InsertChatString( argstring, _XDEF_CHATMESSAGECOLOR_SYSTEMGENERAL, TRUE );
		
		return FALSE;
	}
	
	return TRUE;
}


// _XWindow_TestServerNotice.cpp: implementation of the _XWindow_TestServerNotice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_TestServerNotice.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_TestServerNotice::_XWindow_TestServerNotice()
{
	m_pListBox = NULL;
}

_XWindow_TestServerNotice::~_XWindow_TestServerNotice()
{

}

BOOL _XWindow_TestServerNotice::Initialize( _XTextureManager& texturearchive )
{
	int mainframeinterface = texturearchive.FindResource( "MI_login_00.tga" );
	
	_XImageStatic* pTitleCenterBar = new _XImageStatic;
	pTitleCenterBar->Create( 98, 0, 98 + 16, 24,
		&texturearchive, mainframeinterface );
	pTitleCenterBar->SetClipRect( 232, 117, 249, 141  );
	pTitleCenterBar->SetScale( 272.0f / 16.0f, 1.0f );
	InsertChildObject( pTitleCenterBar );

	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 98, 24,
		&texturearchive, mainframeinterface );
	pTitleLeftBar->SetClipRect( 150, 117, 248, 141 );
	InsertChildObject( pTitleLeftBar );
	
	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create( m_WindowSize.cx, 0, m_WindowSize.cx + 98, 24,
					   &texturearchive, mainframeinterface );
	pTitleRightBar->SetClipRect( 150, 117, 248, 141  );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );


	int scrollbarimageindex = texturearchive.FindResource( "MI_chat_quick.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 6, 26 },										// 윈도우 좌표
		{ 440, 280 },									// 윈도우 사이즈
		0,												// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&texturearchive,								// Texture list
		_T(" "),										// 윈도우 타이틀
		16,												// 리스트 아이템 라인 피치
		17,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 280 },									// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		30,												// 전체 리스트 갯수
		scrollbarimageindex,
		scrollbarimageindex,
		scrollbarimageindex,
		scrollbarimageindex,
		scrollbarimageindex,
	};
	m_pListBox = new _XListBox;
	m_pListBox->Create(listboxstructure);
	
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
		pScrollBar->SetViewNullBorder(FALSE);
	}
	
	POINT coloffset = { 2, 10 };
	m_pListBox->SetColumnOffset(0, coloffset);
	m_pListBox->EnableSelectItem(FALSE);	
	m_pListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_pListBox->SetColumnWidth(0, 420 );	
	InsertChildObject(m_pListBox);

	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN) 
	{
		if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
		
#ifdef _XDEF_NEXONUNIFICATIONGUIDE //정식서버 넥슨 통합 안내문 표시 	
		FILE* licensefile = g_ScriptArchive.GetPackedFile( _T("LoginScreenText.txt") );
#else
		FILE* licensefile = g_ScriptArchive.GetPackedFile( _T("TestServerLicense.txt") );
#endif
		
		if( !licensefile )
		{
			g_ScriptArchive.ClosePackage();
			m_pListBox->InsertListItem( _T("License file not found"), 0xAFFFFFFF, 1 );
			return FALSE;
		}
		
		TCHAR buffer[256];
		
		if( feof( licensefile ) ) 
		{
			g_ScriptArchive.ClosePackage();
			m_pListBox->InsertListItem( _T("License file not found"), 0xAFFFFFFF, 1 );
			return FALSE;
		}
		
		int index = 0;
		do 
		{
			if( feof(licensefile) ) break;
			
			memset( buffer, 0, sizeof(TCHAR)*256 );
			if( !fgets( buffer, 255, licensefile ) )
			{
				break;
			}		
			
			if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
			
			if( /*( strlen(buffer) > 0 ) &&*/ buffer[0] != _T(';') /*&& buffer[0] != _T('\n') && buffer[0] != _T('\r')*/ )
			{
				//TCHAR strbuffer[256];
				//sscanf( buffer, "%s", strbuffer );
				m_pListBox->InsertListItem(buffer, 0xAFFFFFFF, index+1 );
				index++;
			}
			
		} while( !feof(licensefile) );
		
		g_ScriptArchive.ClosePackage();
	}

	g_MainViewPort.X = 0;
	g_MainViewPort.Y = 0;
	g_MainViewPort.Width = gnWidth;
	g_MainViewPort.Height = gnHeight;
	g_MainViewPort.MinZ = 0.0f;
	g_MainViewPort.MaxZ = 1.0f;


	_XBTN_STRUCTURE closebtnstruct = 
	{
		TRUE, 
		{ (m_WindowSize.cx>>1)-(73>>1), m_WindowSize.cy - 28 }, { 73, 24 },
		_XDEF_LS_TESTSERVERNOTICEWINDOWCLOSEBTN,
		mainframeinterface,
		mainframeinterface,
		mainframeinterface,
		&texturearchive
	};
	_XButton* pCloseButton = new _XButton;
	pCloseButton->Create(closebtnstruct);
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 176, 146, 249, 170  );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  176, 173, 249, 197  );
	pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  176, 200, 249, 224  );
	pCloseButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_TESTSERVER_2579 );
	pCloseButton->SetButtonTextColor( 0xFFFFFFFF );
	pCloseButton->SetGrowTextMode( TRUE );
	InsertChildObject(pCloseButton);
	
	return TRUE;
}

void _XWindow_TestServerNotice::DestroyWindow( void )
{

}

void _XWindow_TestServerNotice::Draw(_XGUIObject*& pfocusobject)
{
	if( this->m_ShowWindow )
	{
		_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+24, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0.0f, D3DCOLOR_ARGB(160,179,179,179) );
		_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+25, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	}

	_XWindow::Draw( pfocusobject );

	if( this->m_ShowWindow )
	{
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		
#ifdef _XDEF_NEXONUNIFICATIONGUIDE //정식서버 넥슨 통합 안내문 표시 	
		g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 6, _XFONT_ALIGNTYPE_CENTER, _T("넥슨 통합 안내문") );
#else
		g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 6, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TESTSERVER_2578) );
#endif
		g_XBaseFont->Flush();
	}
}

BOOL _XWindow_TestServerNotice::Process(_XGUIObject*& pfocusobject)
{
	_XWindow::Process( pfocusobject );
	return TRUE;
}

BOOL _XWindow_TestServerNotice::ProcessWheelMessage( short zDelta )
{
	return m_pListBox->ProcessWheelMessage( zDelta );
}

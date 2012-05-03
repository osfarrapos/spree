// XWindow_Hedek.cpp: implementation of the _XWindow_Hedek class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Hedek.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TCHAR* m_HedekTypeName[30] = 
{
	_T("_XHEDEK_NONE"),
	_T("_XHEDEK_CINFO"),
	_T("_XHEDEK_INVEN"),
	_T("_XHEDEK_SKILL"),
	_T("_XHEDEK_QUEST"),
	_T("_XHEDEK_NPCBUSS"),
	_T("_XHEDEK_PRESENT"),
	_T("_XHEDEK_WAREHOUSE"),
	_T("_XHEDEK_REPAIR"),
	_T("_XHEDEK_EDUCATION"),

	_T("_XHEDEK_OUTFIT"),
	_T("_XHEDEK_OPTION"),
	_T("_XHEDEK_GRAPHIC"),
	_T("_XHEDEK_SOUND"),
	_T("_XHEDEK_GAME"),
	_T("_XHEDEK_COMMUNITY"),
	_T("_XHEDEK_MESSENGER"),
	_T("_XHEDEK_PAGE"),
	_T("_XHEDEK_PTOP"),
	_T("_XHEDEK_DECO"),

	_T("_XHEDEK_MESSENGERWIN"),
	_T("_XHEDEK_POCKET"),
	_T("_XHEDEK_MASTERY"),
	_T("_XHEDEK_GUILD"),
	_T("_XHEDEK_LIFE"),
	_T("_XHEDEK_SOLOSHOP"),
	_T("_XHEDEK_BONUSBOX"),
	_T("_XHEDEK_NICK"),
	_T("_XHEDEK_PLAYERINFO"),
	_T("_XHEDEK_PKTOTAL")
};

_XWindow_Hedek::_XWindow_Hedek()
{
	m_HedekListBox = NULL;
}

_XWindow_Hedek::~_XWindow_Hedek()
{

}

BOOL _XWindow_Hedek::Initialize( void )
{
	// Create GUI Object ============================================================================

	// Border---------------------------------------------------------------------------------------=	
	int hedekimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 180, 18, &g_MainInterfaceTextureArchive, hedekimageindex );
	pTitleBar->SetClipRect( 0, 125, 180, 143 );
	InsertChildObject(pTitleBar);
	_XImageStatic* pTitleBarAdd = new _XImageStatic;
	pTitleBarAdd->Create(180, 0, 256, 18, &g_MainInterfaceTextureArchive, hedekimageindex );
	pTitleBarAdd->SetClipRect( 106, 125, 183, 143 );
	InsertChildObject(pTitleBarAdd);
	// ---------------------------------------------------------------------------------------------=

	// close button --------------------------------------------------------------------------------=	
	hedekimageindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 239, 2 }, { 12, 12 }, _XDEF_HEDEK_CLOSE,
		hedekimageindex,hedekimageindex,hedekimageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
	// ---------------------------------------------------------------------------------------------=

	// stamp image ---------------------------------------------------------------------------------=
	hedekimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_party.tga" );
	_XImageStatic* pHedekStampImage = new _XImageStatic;
	pHedekStampImage->Create( 157, 162, 244, 254, &g_MainInterfaceTextureArchive, hedekimageindex );
	pHedekStampImage->SetClipRect(168, 88, 256, 181);	
	pHedekStampImage->m_FColor = D3DCOLOR_ARGB( 76, 255, 255, 255 );
	InsertChildObject( pHedekStampImage );
	// ---------------------------------------------------------------------------------------------=

	// ok button -----------------------------------------------------------------------------------=
	hedekimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	_XBTN_STRUCTURE okbtnstruct = 
	{
		TRUE, {132, 230}, {61, 25},
		_XDEF_HEDEK_CLOSE,
		hedekimageindex, hedekimageindex, hedekimageindex,
		&g_MainInterfaceTextureArchive
	};
	_XButton* pOKButton = new _XButton;
	pOKButton->Create(okbtnstruct);
	pOKButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 188, 166, 249, 191);
	pOKButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 188, 193, 249, 218);
	pOKButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 220, 249, 245);
	pOKButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	pOKButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);
	InsertChildObject( pOKButton );
	
	okbtnstruct.commandid++;
	okbtnstruct.position.x = 66;
	_XButton* pHelpbookButton = new _XButton;
	pHelpbookButton->Create(okbtnstruct);
	pHelpbookButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 188, 166, 249, 191);
	pHelpbookButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 188, 193, 249, 218);
	pHelpbookButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 220, 249, 245);
	pHelpbookButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	pHelpbookButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_HELP);
	InsertChildObject( pHelpbookButton );
	// ---------------------------------------------------------------------------------------------=

	// HedekListBox --------------------------------------------------------------------------------=
	hedekimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	_XLISTBOX_STRUCTURE hedekstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 6, 22 },										// 윈도우 좌표
		{ 244, 209 },									// 윈도우 사이즈
		_XDEF_HEDEK_HEDEKLISTBOX,						// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		16,												// 리스트 아이템 라인 피치
		12,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 210 },									// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		30,												// 전체 리스트 갯수
		hedekimageindex,
		hedekimageindex,
		hedekimageindex,
		hedekimageindex,
		hedekimageindex,
	};
	m_HedekListBox = new _XListBox;
	m_HedekListBox->Create(hedekstructure);

	_XVScrollBar* pScrollBar = m_HedekListBox->GetScrollBarObject();
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

	POINT coloffset = { 5, 10 };
	m_HedekListBox->SetColumnOffset(0, coloffset);
	m_HedekListBox->EnableSelectItem(FALSE);	
	m_HedekListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_HedekListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_HedekListBox->SetColumnWidth(0, 230 );	
	InsertChildObject(m_HedekListBox);
	// ---------------------------------------------------------------------------------------------=
	return TRUE;
}

void _XWindow_Hedek::DestroyWindow( void )
{
	
}

void _XWindow_Hedek::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;

	// Draw Border, Line ---------------------------------------------------------------------=	
	_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, D3DCOLOR_ARGB(178, 219, 202, 176) );
	
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+16, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0.0f,D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+16+1, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f,D3DCOLOR_ARGB(255,173,152,101) );
	_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+16+2, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f,D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+3, m_WindowPosition.y+16+3, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f,D3DCOLOR_ARGB(178,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+16+4, m_WindowPosition.x+m_WindowSize.cx-4, m_WindowPosition.y+m_WindowSize.cy-4, 0.0f,D3DCOLOR_ARGB(127,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+16+5, m_WindowPosition.x+m_WindowSize.cx-5, m_WindowPosition.y+m_WindowSize.cy-5, 0.0f,D3DCOLOR_ARGB(64,0,0,0) );	
	// ---------------------------------------------------------------------------------------=

	_XWindow::Draw(pfocusobject);

	// Title Name
	g_XBaseFont->SetColor( 0xFFF5E8CF );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2), m_WindowPosition.y+4 , _XFONT_ALIGNTYPE_CENTER, m_TitleText);
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx/2)+1, m_WindowPosition.y+4 , _XFONT_ALIGNTYPE_CENTER, m_TitleText);
	g_XBaseFont->Flush();
}

BOOL _XWindow_Hedek::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow )		return FALSE;
	if( !this->m_Enable  )			return FALSE;

	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

// m_HedekType에 따라 필요한 헤덱내용만 로딩함.
BOOL _XWindow_Hedek::LoadHedekData()
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* pScript = NULL;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{		
		pScript = g_ScriptArchive.GetPackedFile( _T("vn_hedektext.txt") );
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read hedek text[%s] file open", _T("vn_hedektext.txt"));
			return FALSE;
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{		
		pScript = g_ScriptArchive.GetPackedFile( _T("tw_hedektext.txt") );
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read hedek text[%s] file open", _T("tw_hedektext.txt"));
			return FALSE;
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{		
		pScript = g_ScriptArchive.GetPackedFile( _T("us_hedektext.txt") );
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read hedek text[%s] file open", _T("us_hedektext.txt"));
			return FALSE;
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{		
		pScript = g_ScriptArchive.GetPackedFile( _T("jp_hedektext.txt") );
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read hedek text[%s] file open", _T("jp_hedektext.txt"));
			return FALSE;
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{		
		pScript = g_ScriptArchive.GetPackedFile( _T("rs_hedektext.txt") );
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read hedek text[%s] file open", _T("rs_hedektext.txt"));
			return FALSE;
		}
	}
	else
	{
		pScript = g_ScriptArchive.GetPackedFile( _T("hedektext.txt") );
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read hedek text[%s] file open", _T("hedektext.txt"));
			return FALSE;
		}
	}
	

	TCHAR buffer[256];
	TCHAR buffer2[256];
	
	while( !feof(pScript) )
	{				
		memset( buffer, 0, 256 );
		fscanf( pScript, "%s", buffer );

		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		if( buffer[0] == ';' || buffer[0] == '\n' || !strlen(buffer) ) continue;

		if( _stricmp( buffer, m_HedekTypeName[m_HedekType] ) == 0 )
		{
			memset( buffer, 0, 256 );
			while(1)
			{
				fgets( buffer, 255, pScript );
				memset( buffer2, 0, 256 );
				sscanf( buffer, "%s", buffer2 ); 
				if( strlen( buffer2 ) > 0 )
				{
					break;
				}
			}

			memset( m_TitleText, 0, sizeof(char) * 64 );
			memcpy( m_TitleText, buffer, strlen(buffer) );
			//sscanf( buffer, "%s", m_TitleText );

			int linecount = 0;
			//memset( buffer, 0, 256 );
			//fscanf( pScript, "%s", buffer );
			fscanf( pScript, "%d", &linecount );			
			//linecount = atoi(buffer);
			if( linecount <= 0 )
			{
#ifdef _XDWDEBUG
				_XFatalError("Empty hedek text");
#endif
				g_ScriptArchive.ClosePackage();
				return FALSE;
			}

			// 갯수 다음 문자열 처리
			memset( buffer, 0, 256 );
			fgets( buffer, 255, pScript );
			m_HedekListBox->DeleteAllItem();

			for( int i = 0; i < linecount; i++ )
			{
				memset( buffer, 0, 256 );
				fgets( buffer, 255, pScript );

				m_HedekListBox->InsertListItem(buffer, 0xFF000000, i+1 );
			}
			break;
		}		
	}		
	
	g_ScriptArchive.ClosePackage();

	return TRUE;		
}

void _XWindow_Hedek::ShowWindow(BOOL show)
{
	if(show)
	{
		_XVScrollBar* pScrollBar = m_HedekListBox->GetScrollBarObject();
		if( pScrollBar )
		{
			pScrollBar->SetLineScrollPos(0);
		}
		XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

		if( pMainGameProcess->m_bNPCDialogMode || pMainGameProcess->m_bNPCDialogWideView )
			MoveWindow( 10, 104 );
		else
		{
			#ifdef _ACCLAIM_RUBICONADSYSTEM	
				if( g_ViewRubiconAD )
					MoveWindow( gnWidth-420, 10 + g_RubiconADWindowSize.cy );
				else
					MoveWindow( gnWidth-420, 10 );
			#else
				MoveWindow( gnWidth-420, 10 );
			#endif
		}

		g_MainWindowManager.SetTopWindow(this);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_Hedek::SetHedekType( _XHEDEKTYPE type )
{
#ifdef _XTS_SHOWTUTORIAL
	this->ShowWindow(TRUE);
#else
	this->ShowWindow(FALSE);
#endif
	m_HedekType = type;
	
	LoadHedekData();	
}
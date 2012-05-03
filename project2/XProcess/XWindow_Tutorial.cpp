// XWindow_Tutorial.cpp: implementation of the _XWindow_Tutorial class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Tutorial.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 추가될 때마다 삽입 해야 함
TCHAR* m_TutorialTypeName[53] = 
{
	_T("_XTUTORIAL_NONE"),
	_T("_XTUTORIAL_001"),
	_T("_XTUTORIAL_002"),
	_T("_XTUTORIAL_003"),
	_T("_XTUTORIAL_004"),
	_T("_XTUTORIAL_005"),
	_T("_XTUTORIAL_006"),
	_T("_XTUTORIAL_007"),
	_T("_XTUTORIAL_008"),
	_T("_XTUTORIAL_009"),
	_T("_XTUTORIAL_010"),
	_T("_XTUTORIAL_011"),
	_T("_XTUTORIAL_012"),
	_T("_XTUTORIAL_013"),
	_T("_XTUTORIAL_014"),
	_T("_XTUTORIAL_015"),
	_T("_XTUTORIAL_016"),
	_T("_XTUTORIAL_017"),
	_T("_XTUTORIAL_018"),
	_T("_XTUTORIAL_019"),
	_T("_XTUTORIAL_020"),
	_T("_XTUTORIAL_021"),
	_T("_XTUTORIAL_022"),
	_T("_XTUTORIAL_023"),
	_T("_XTUTORIAL_024"),
	_T("_XTUTORIAL_025"),
	_T("_XTUTORIAL_026"),
	_T("_XTUTORIAL_027"),
	_T("_XTUTORIAL_028"),
	_T("_XTUTORIAL_029"),
	_T("_XTUTORIAL_030"),
	_T("_XTUTORIAL_031"),
	_T("_XTUTORIAL_032"),
	_T("_XTUTORIAL_033"),
	_T("_XTUTORIAL_034"),
	_T("_XTUTORIAL_035"),
	_T("_XTUTORIAL_036"),
	_T("_XTUTORIAL_037"),
	_T("_XTUTORIAL_038"),
	_T("_XTUTORIAL_039"),
	_T("_XTUTORIAL_040"),
	_T("_XTUTORIAL_041"),
	_T("_XTUTORIAL_042"),
	_T("_XTUTORIAL_043"),
	_T("_XTUTORIAL_044"),
	_T("_XTUTORIAL_045"),
	_T("_XTUTORIAL_046"),
	_T("_XTUTORIAL_047"),
	_T("_XTUTORIAL_048"),
	_T("_XTUTORIAL_049"),
	_T("_XTUTORIAL_050"),
	_T("_XTUTORIAL_051"),
	_T("_XTUTORIAL_052")
};

_XWindow_Tutorial::_XWindow_Tutorial()
{
	for( int i = 0; i < 4; i++ )
		m_RoundEdgeImage[i] = NULL;

	m_LTitleImage		= NULL;
	m_MTitleImage		= NULL;
	m_RTitleImage		= NULL;

	m_Horizontal		= NULL;
	m_Vertically		= NULL;
	m_Mark				= NULL;
	m_btnTutorialClose	= NULL;
	m_pHelpbookCallButton = NULL;
}

_XWindow_Tutorial::~_XWindow_Tutorial()
{

}

// m_TutorialType에 따라 필요한 것만 로딩
BOOL _XWindow_Tutorial::LoadTutorialData()
{
	for( int i = 0; i < _MAX_TUTORIALCOUNT; i++ )
	{
		memset( m_CurTutorialText[i], 0, sizeof(TCHAR)*128);
	}
	
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
	FILE* pScript = NULL;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		pScript = g_ScriptArchive.GetPackedFile( _T("vn_tutorialtext.txt") );
		
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read tutorial text[%s] file open", _T("vn_tutorialtext.txt"));
			return FALSE;
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pScript = g_ScriptArchive.GetPackedFile( _T("tw_tutorialtext.txt") );
		
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read tutorial text[%s] file open", _T("tw_tutorialtext.txt"));
			return FALSE;
		}
	}	
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pScript = g_ScriptArchive.GetPackedFile( _T("us_tutorialtext.txt") );
		
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read tutorial text[%s] file open", _T("us_tutorialtext.txt"));
			return FALSE;
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		pScript = g_ScriptArchive.GetPackedFile( _T("jp_tutorialtext.txt") );
		
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read tutorial text[%s] file open", _T("jp_tutorialtext.txt"));
			return FALSE;
		}
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pScript = g_ScriptArchive.GetPackedFile( _T("rs_tutorialtext.txt") );
		
		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read tutorial text[%s] file open", _T("rs_tutorialtext.txt"));
			return FALSE;
		}
	}
	else
	{
		pScript = g_ScriptArchive.GetPackedFile( _T("tutorialtext.txt") );

		if( !pScript )
		{
			g_ScriptArchive.ClosePackage();
			_XFatalError("Could not read tutorial text[%s] file open", _T("tutorialtext.txt"));
			return FALSE;
		}
	}
	
	TCHAR buffer[256];
	BOOL flag = FALSE;

	m_TextWidth = 0;
	
	while( !feof(pScript) )
	{				
		memset( buffer, 0, 128 );
		fscanf( pScript, "%s", buffer );
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		if( buffer[0] == ';' || buffer[0] == '\n' || !strlen(buffer) ) continue;
		
		if( _stricmp( buffer, m_TutorialTypeName[m_TutorialType] ) == 0 )
		{
			flag = TRUE;
			int linecount = 0;
			memset( buffer, 0, 256 );
			fscanf( pScript, "%s", buffer );
			linecount = atoi(buffer);
			if( linecount <= 0 )
			{
#ifdef _XDWDEBUG
				_XFatalError("Empty tutorial text");
#endif
				g_ScriptArchive.ClosePackage();
				return FALSE;
			}
			
			// 갯수 다음 문자열 처리
			memset( buffer, 0, 256 );
			fgets( buffer, 256, pScript );
			
			for( int i = 0; i < linecount; i++ )
			{
				memset( buffer, 0, 256 );
				fgets( buffer, 256, pScript );

				if( i <= _MAX_TUTORIALCOUNT )
				{
					strcpy( m_CurTutorialText[i], buffer );

					int width = g_XBaseFont->GetWidth(m_CurTutorialText[i]);
					if(width > m_TextWidth)
						m_TextWidth = width;
				}
			}

			if(linecount > 6)
			{
				m_TextHeight = 160 + (linecount-6)*13;
			}
			else
			{
				m_TextHeight = 160;
			}

			break;
		}		
	}
	
	g_ScriptArchive.ClosePackage();

	// 텍스트 크기에 맞춰서 크기 변경
	this->SetWindowSize(m_TextWidth + 18, m_TextHeight);
	m_ScaleX = ((FLOAT)m_WindowSize.cx - (36*2)) / 10.0f;
	m_ScaleY = ((FLOAT)m_WindowSize.cy - (36*2)) / 10.0f;

	m_ScaleTitleBar = ((FLOAT)m_WindowSize.cx-(9+9+3+3)) / 10.0f;
	
	return flag;
}

// 40100번 초과시 XProc_ProcessDefine에 더이상 쓰겠다고 정의할것(주석으로 밑에 부분 되어있음)
_XWindow_Tutorial*	_XWindow_Tutorial::CreateTutorialDialogBox( _XWindowManager* pWindowManager )
{
	for( int windowid = 40000; windowid < 40100; windowid++ )
	{
		if( !pWindowManager->FindWindow(windowid) )
		{
			_XWindow_Tutorial* pNewTutorialDialogBox = new _XWindow_Tutorial;
			pNewTutorialDialogBox->Create( pWindowManager, (DWORD)windowid );
			return pNewTutorialDialogBox;
		}
	}

	return NULL;	
}

void _XWindow_Tutorial::Create( _XWindowManager* pWindowManager, DWORD windowid )
{
	m_WindowID = windowid;
	_XWINDOW_STRUCTURE windowstruct = 
	{
		TRUE, { gnWidth-330, 234 }, { 320, 160 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0,0,0,0),
			windowid
	};
	_XWindow::Create( windowstruct );
	
	m_solidborderstatic.Create( m_WindowPosition.x,m_WindowPosition.y, 
								m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+ m_WindowSize.cy ,
								_XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(0, 0, 0, 0) );

	Initialize();
	// close button ----------------------------------------------------------------------------------=	
	int btnimgindex = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	_XBTN_STRUCTURE okbtnstruct = 
	{
		TRUE, {168, 124}, {61, 25},
		((DWORD)windowid<<16),
		btnimgindex, btnimgindex, btnimgindex,
		&g_MainInterfaceTextureArchive
	};
	SAFE_DELETE( m_btnTutorialClose );	
	m_btnTutorialClose = new _XButton;
	m_btnTutorialClose->Create(okbtnstruct);
	m_btnTutorialClose->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 188, 166, 249, 191);
	m_btnTutorialClose->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 188, 193, 249, 218);
	m_btnTutorialClose->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 220, 249, 245);
	m_btnTutorialClose->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnTutorialClose->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK);
	m_btnTutorialClose->ShowWindow(TRUE);
	
	_XBTN_STRUCTURE helpbookbtnstruct = 
	{
		TRUE, {91, 124}, {61, 25},
		_XDEF_HEDEK_HELPBOOKCALL,
		btnimgindex, btnimgindex, btnimgindex,
		&g_MainInterfaceTextureArchive
	};
	SAFE_DELETE(m_pHelpbookCallButton);
	m_pHelpbookCallButton = new _XButton;
	m_pHelpbookCallButton->Create(helpbookbtnstruct);
	m_pHelpbookCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 188, 166, 249, 191);
	m_pHelpbookCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 188, 193, 249, 218);
	m_pHelpbookCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 220, 249, 245);
	m_pHelpbookCallButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pHelpbookCallButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_HELP);
	m_pHelpbookCallButton->ShowWindow(TRUE);
	
	// -----------------------------------------------------------------------------------------------=

	if( pWindowManager )
		pWindowManager->InsertWindow( this );
}

BOOL _XWindow_Tutorial::Initialize( void )
{
	// Create GUI Object ============================================================================	

	// Round Img -----------------------------------------------------------------------------------=	
	for( int i = 0; i < 4; i++ )
	{
		SAFE_DELETE( m_RoundEdgeImage[i] );	
		m_RoundEdgeImage[i]= new _XImageStatic;
	}

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_Messenger2.tga");

	m_RoundEdgeImage[0]->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[0]->SetClipRect(175, 124, 211, 160);
	m_RoundEdgeImage[1]->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[1]->SetClipRect(218, 124, 254, 160);
	m_RoundEdgeImage[2]->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[2]->SetClipRect(175, 162, 211, 198);
	m_RoundEdgeImage[3]->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[3]->SetClipRect(218, 162, 254, 198);

	SAFE_DELETE(m_LTitleImage);
	m_LTitleImage = new _XImageStatic;
	m_LTitleImage->Create(0, 0, 3, 16, &g_MainInterfaceTextureArchive, resourceindex);
	m_LTitleImage->SetClipRect(0, 123, 3, 139);

	SAFE_DELETE( m_MTitleImage );
	m_MTitleImage = new _XImageStatic;
	m_MTitleImage->Create( 0, 0, 10, 16, &g_MainInterfaceTextureArchive, resourceindex );
	m_MTitleImage->SetClipRect( 2, 123, 12, 139 );

	SAFE_DELETE(m_RTitleImage);
	m_RTitleImage = new _XImageStatic;
	m_RTitleImage->Create(0, 0, 3, 16, &g_MainInterfaceTextureArchive, resourceindex);
	m_RTitleImage->SetClipRect(0, 123, 3, 139);

	SAFE_DELETE( m_Horizontal );
	m_Horizontal = new _XImageStatic;
	m_Horizontal->Create( 0, 0, 10, 6, &g_MainInterfaceTextureArchive, resourceindex );
	m_Horizontal->SetClipRect( 200, 124, 210, 130 );	

	SAFE_DELETE( m_Vertically );
	m_Vertically = new _XImageStatic;
	m_Vertically->Create( 0, 0, 6, 10, &g_MainInterfaceTextureArchive, resourceindex );
	m_Vertically->SetClipRect( 175, 149, 181, 159 );

	SAFE_DELETE( m_Mark );
	m_Mark = new _XImageStatic;
	m_Mark->Create( 0, 0, 16, 16, &g_MainInterfaceTextureArchive, resourceindex );
	m_Mark->SetClipRect( 153, 123, 169, 139 );

	// ---------------------------------------------------------------------------------------------= 

	m_TextWidth			= 0;
	m_TextHeight		= 0;
	m_ScaleX			= 1.0f;
	m_ScaleY			= 1.0f;
	m_ScaleTitleBar		= 1.0f;

	return TRUE;
}

void _XWindow_Tutorial::DestroyWindow( void )
{
	SAFE_DELETE( m_pHelpbookCallButton );
	SAFE_DELETE( m_btnTutorialClose );	
	SAFE_DELETE( m_Mark );
	SAFE_DELETE( m_Vertically );
	SAFE_DELETE( m_Horizontal );
	SAFE_DELETE( m_LTitleImage );
	SAFE_DELETE( m_RTitleImage );
	SAFE_DELETE( m_MTitleImage );

	for( int i = 0; i < 4; i++ )
		SAFE_DELETE(m_RoundEdgeImage[i]);
	
	_XWindow::DestroyWindow();
}

void _XWindow_Tutorial::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;
	_XWindow::Draw(pfocusobject);
	// Draw BackGround
	_XDrawSolidBar(m_WindowPosition.x+4, m_WindowPosition.y+4, m_WindowPosition.x+m_WindowSize.cx-5, m_WindowPosition.y+m_WindowSize.cy-5, D3DCOLOR_ARGB(204,231,211,177) );

	// Draw Horizontal, Vertically
	m_Horizontal->SetScale( m_ScaleX, 1.0f );
	m_Horizontal->Draw( m_WindowPosition.x+36, m_WindowPosition.y );
	m_Horizontal->SetScale( m_ScaleX, -1.0f );
	m_Horizontal->Draw( m_WindowPosition.x+36, m_WindowPosition.y+m_WindowSize.cy );

	m_Vertically->SetScale( 1.0f, m_ScaleY );
	m_Vertically->Draw( m_WindowPosition.x, m_WindowPosition.y+36 );
	m_Vertically->SetScale( -1.0f, m_ScaleY );
	m_Vertically->Draw( m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+36 );

	// Draw Corner
	m_RoundEdgeImage[0]->Draw( m_WindowPosition.x, m_WindowPosition.y );
	m_RoundEdgeImage[1]->Draw( m_WindowPosition.x+m_WindowSize.cx-36, m_WindowPosition.y );
	m_RoundEdgeImage[2]->Draw( m_WindowPosition.x, m_WindowPosition.y+m_WindowSize.cy-36 );
	m_RoundEdgeImage[3]->Draw( m_WindowPosition.x+m_WindowSize.cx-36, m_WindowPosition.y+m_WindowSize.cy-36 );

	// Draw TitleBar
	m_LTitleImage->Draw(m_WindowPosition.x+9, m_WindowPosition.y+8);

	m_MTitleImage->SetScale( m_ScaleTitleBar, 1.0f );
	m_MTitleImage->Draw( m_WindowPosition.x+9+3, m_WindowPosition.y+8 );

	m_RTitleImage->SetScale( -1.0f, 1.0f );
	m_RTitleImage->Draw(m_WindowPosition.x+m_WindowSize.cx-9, m_WindowPosition.y+8);

	m_Mark->Draw( m_WindowPosition.x+13, m_WindowPosition.y+8 );
	// Draw Btn
	m_btnTutorialClose->Draw();
	m_pHelpbookCallButton->Draw();
	
	// Draw Text
	if( strlen( m_CurTutorialText[0]) )
	{
		g_XBaseFont->SetColor( 0xFFFFFFFF );	
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+33, m_WindowPosition.y + 10, _XFONT_ALIGNTYPE_LEFT, m_CurTutorialText[0] );
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
	}

	g_XBaseFont->SetColor( 0xFF000000 );
	for( int i = 1; i < _MAX_TUTORIALCOUNT; i++ )
	{
		if( strlen( m_CurTutorialText[i]) )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+11, m_WindowPosition.y + 17+i*15, _XFONT_ALIGNTYPE_LEFT, m_CurTutorialText[i] );
		}
	}

	g_XBaseFont->Flush();
}

BOOL _XWindow_Tutorial::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow )		return FALSE;
	if( !this->m_Enable  )			return FALSE;

	m_btnTutorialClose->Process(pfocusobject);
	m_pHelpbookCallButton->Process(pfocusobject);
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

void _XWindow_Tutorial::ShowWindow(BOOL show)
{
	if(show)
	{
		//XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

		//if( pMainGameProcess->m_bNPCDialogMode || pMainGameProcess->m_bNPCDialogWideView )
		//	MoveWindow( 10, 375 );
		//else
		//	MoveWindow( gnWidth-361, 235 );

		g_MainWindowManager.SetTopWindow(this);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_Tutorial::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow( X, Y);

	m_btnTutorialClose->MoveWindow(X+(m_WindowSize.cx>>1)+5, Y+m_WindowSize.cy-36);	
	m_pHelpbookCallButton->MoveWindow(X+(m_WindowSize.cx>>1)-(5+m_pHelpbookCallButton->GetWindowSize().cx), Y+m_WindowSize.cy-36);
}

BOOL _XWindow_Tutorial::SetTutorialType( _XTUTORIALTYPE type )
{
	m_TutorialType = type;
	
	return LoadTutorialData();
}

// ---------------------------------------------------------------------------------------------------------=
// -------------------------------------필수 튜토리얼 창----------------------------------------------------=
// ---------------------------------------------------------------------------------------------------------=
_XWindow_TutorialEx::_XWindow_TutorialEx()
{
	for( int i = 0; i < 2; i++ )
		m_RoundEdgeImage[i] = NULL;

	m_LTitleImage		= NULL;
	m_MTitleImage		= NULL;
	m_RTitleImage		= NULL;

	m_Horizontal		= NULL;
	m_Vertically		= NULL;
	m_Mark				= NULL;
	m_btnTutorialClose	= NULL;
	m_pHelpbookCallButton = NULL;
}

_XWindow_TutorialEx::~_XWindow_TutorialEx()
{

}

_XWindow_TutorialEx*	_XWindow_TutorialEx::CreateTutorialDialogBox( _XWindowManager* pWindowManager )
{
	for( int windowid = 40000; windowid < 40100; windowid++ )
	{
		if( !pWindowManager->FindWindow(windowid) )
		{
			_XWindow_TutorialEx* pNewTutorialDialogBox = new _XWindow_TutorialEx;
			pNewTutorialDialogBox->Create( pWindowManager, (DWORD)windowid );
			return pNewTutorialDialogBox;
		}
	}

	return NULL;	
}

void _XWindow_TutorialEx::Create( _XWindowManager* pWindowManager, DWORD windowid )
{
	m_WindowID = windowid;
	_XWINDOW_STRUCTURE windowstruct = 
	{
		TRUE, { gnWidth-330, 234 }, { 320, 274 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0,0,0,0),
			windowid
	};
	_XWindow::Create( windowstruct );
	
	m_solidborderstatic.Create( m_WindowPosition.x,m_WindowPosition.y, 
								m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+ m_WindowSize.cy ,
								_XDEF_DEFAULTDEPTH_FORINTERFACE, D3DCOLOR_ARGB(0, 0, 0, 0) );

	Initialize();
	// close button ----------------------------------------------------------------------------------=	
	int btnimgindex = g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	_XBTN_STRUCTURE okbtnstruct = 
	{
		TRUE, {168, 124}, {61, 25},
		((DWORD)windowid<<16),
		btnimgindex, btnimgindex, btnimgindex,
		&g_MainInterfaceTextureArchive
	};
	SAFE_DELETE( m_btnTutorialClose );	
	m_btnTutorialClose = new _XButton;
	m_btnTutorialClose->Create(okbtnstruct);
	m_btnTutorialClose->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 188, 166, 249, 191);
	m_btnTutorialClose->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 188, 193, 249, 218);
	m_btnTutorialClose->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 220, 249, 245);
	m_btnTutorialClose->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_btnTutorialClose->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK);
	m_btnTutorialClose->ShowWindow(TRUE);
	
	_XBTN_STRUCTURE helpbookbtnstruct = 
	{
		TRUE, {91, 124}, {61, 25},
		_XDEF_HEDEK_HELPBOOKCALL,
		btnimgindex, btnimgindex, btnimgindex,
		&g_MainInterfaceTextureArchive
	};
	SAFE_DELETE(m_pHelpbookCallButton);
	m_pHelpbookCallButton = new _XButton;
	m_pHelpbookCallButton->Create(helpbookbtnstruct);
	m_pHelpbookCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 188, 166, 249, 191);
	m_pHelpbookCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 188, 193, 249, 218);
	m_pHelpbookCallButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 220, 249, 245);
	m_pHelpbookCallButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pHelpbookCallButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_HELP);
	m_pHelpbookCallButton->ShowWindow(TRUE);
	
	// -----------------------------------------------------------------------------------------------=

	if( pWindowManager )
		pWindowManager->InsertWindow( this );
}

BOOL _XWindow_TutorialEx::Initialize( void )
{
	// Create GUI Object ============================================================================	

	// Round Img -----------------------------------------------------------------------------------=	
	for( int i = 0; i < 2; i++ )
	{
		SAFE_DELETE( m_RoundEdgeImage[i] );	
		m_RoundEdgeImage[i]= new _XImageStatic;
	}

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_Messenger2.tga");

	m_RoundEdgeImage[0]->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[0]->SetClipRect(175, 199, 211, 235);
	m_RoundEdgeImage[1]->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex );
	m_RoundEdgeImage[1]->SetClipRect(218, 199, 254, 235);

	SAFE_DELETE( m_Horizontal );
	m_Horizontal = new _XImageStatic;
	m_Horizontal->Create( 0, 0, 10, 6, &g_MainInterfaceTextureArchive, resourceindex );
	m_Horizontal->SetClipRect( 200, 199, 210, 205 );	

	SAFE_DELETE( m_Vertically );
	m_Vertically = new _XImageStatic;
	m_Vertically->Create( 0, 0, 6, 10, &g_MainInterfaceTextureArchive, resourceindex );
	m_Vertically->SetClipRect( 175, 224, 181, 234 );

	SAFE_DELETE( m_Mark );
	m_Mark = new _XImageStatic;
	m_Mark->Create( 0, 0, 16, 16, &g_MainInterfaceTextureArchive, resourceindex );
	m_Mark->SetClipRect( 153, 123, 169, 139 );


	resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_Messenger1.tga");

	SAFE_DELETE(m_LTitleImage);
	m_LTitleImage = new _XImageStatic;
	m_LTitleImage->Create(0, 0, 3, 18, &g_MainInterfaceTextureArchive, resourceindex);
	m_LTitleImage->SetClipRect(82, 136, 85, 154);
	
	SAFE_DELETE( m_MTitleImage );
	m_MTitleImage = new _XImageStatic;
	m_MTitleImage->Create(0, 0, 10, 18, &g_MainInterfaceTextureArchive, resourceindex);
	m_MTitleImage->SetClipRect( 85, 136, 95, 154 );

	SAFE_DELETE(m_RTitleImage);
	m_RTitleImage = new _XImageStatic;
	m_RTitleImage->Create(0, 0, 3, 18, &g_MainInterfaceTextureArchive, resourceindex);
	m_RTitleImage->SetClipRect(82, 136, 85, 154);

	// ---------------------------------------------------------------------------------------------= 
	return TRUE;
}

void _XWindow_TutorialEx::DestroyWindow( void )
{
	SAFE_DELETE( m_pHelpbookCallButton );
	SAFE_DELETE( m_btnTutorialClose );	
	SAFE_DELETE( m_Mark );
	SAFE_DELETE( m_Vertically );
	SAFE_DELETE( m_Horizontal );
	SAFE_DELETE( m_LTitleImage );
	SAFE_DELETE( m_MTitleImage );
	SAFE_DELETE( m_RTitleImage );

	for( int i = 0; i < 2; i++ )
		SAFE_DELETE(m_RoundEdgeImage[i]);
	
	_XWindow::DestroyWindow();
}

void _XWindow_TutorialEx::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;
	_XWindow::Draw(pfocusobject);
	// Draw BackGround
	_XDrawSolidBar(m_WindowPosition.x+4, m_WindowPosition.y+4, m_WindowPosition.x+m_WindowSize.cx-5, m_WindowPosition.y+m_WindowSize.cy-5, D3DCOLOR_ARGB(204,231,211,177) );

	// Draw Horizontal, Vertically
	m_Horizontal->SetScale( m_ScaleX, 1.0f );
	m_Horizontal->Draw( m_WindowPosition.x+36, m_WindowPosition.y );
	m_Horizontal->SetScale( m_ScaleX, -1.0f );
	m_Horizontal->Draw( m_WindowPosition.x+36, m_WindowPosition.y+m_WindowSize.cy );

	m_Vertically->SetScale( 1.0f, m_ScaleY );
	m_Vertically->Draw( m_WindowPosition.x, m_WindowPosition.y+36 );
	m_Vertically->SetScale( -1.0f, m_ScaleY );
	m_Vertically->Draw( m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+36 );

	// Draw Corner
	m_RoundEdgeImage[0]->SetScale( 1.0f, 1.0f);
	m_RoundEdgeImage[0]->Draw( m_WindowPosition.x, m_WindowPosition.y );
	m_RoundEdgeImage[0]->SetScale( 1.0f, -1.0f);
	m_RoundEdgeImage[0]->Draw( m_WindowPosition.x, m_WindowPosition.y+m_WindowSize.cy ); 

	m_RoundEdgeImage[1]->SetScale( 1.0f, 1.0f);
	m_RoundEdgeImage[1]->Draw( m_WindowPosition.x+m_WindowSize.cx-36, m_WindowPosition.y );	
	m_RoundEdgeImage[1]->SetScale( 1.0f, -1.0f);
	m_RoundEdgeImage[1]->Draw( m_WindowPosition.x+m_WindowSize.cx-36, m_WindowPosition.y+m_WindowSize.cy );

	// Draw TitleBar
	m_LTitleImage->Draw(m_WindowPosition.x+9, m_WindowPosition.y+8);

	m_MTitleImage->SetScale(m_ScaleTitleBar, 1.0f);
	m_MTitleImage->Draw( m_WindowPosition.x+9+3, m_WindowPosition.y+8 );

	m_RTitleImage->SetScale(-1.0f, 1.0f);
	m_RTitleImage->Draw(m_WindowPosition.x+m_WindowSize.cx-9, m_WindowPosition.y+8);

	m_Mark->Draw( m_WindowPosition.x+13, m_WindowPosition.y+8 );
	// Draw Btn
	m_btnTutorialClose->Draw();
	m_pHelpbookCallButton->Draw();
	
	// Draw Text
	if( strlen( m_CurTutorialText[0]) )
	{
		g_XBaseFont->SetColor( 0xFFFFFFFF );	
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+33, m_WindowPosition.y + 11, _XFONT_ALIGNTYPE_LEFT, m_CurTutorialText[0] );
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
	}

	g_XBaseFont->SetColor( 0xFF000000 );
	for( int i = 1; i < _MAX_TUTORIALCOUNT; i++ )
	{
		if( strlen( m_CurTutorialText[i]) )
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+11, m_WindowPosition.y + 17+i*15, _XFONT_ALIGNTYPE_LEFT, m_CurTutorialText[i] );
		}
	}

	g_XBaseFont->Flush();	
}


// XWindow_HelpBook.cpp: implementation of the _XWindow_HelpBook class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_HelpBook.h"
#include "XSR_STRINGHEADER.H"

#define _XDEF_HTMLLOADTEST
extern void ReplaceCharacter(LPTSTR _string, TCHAR srcchar, TCHAR changechar);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_HelpBook::_XWindow_HelpBook()
{
	m_pScrbarFocusedObject = NULL;
	m_HTMLListCount = 0;
	m_ppHTMLNameList = NULL;
}

_XWindow_HelpBook::~_XWindow_HelpBook()
{
}

BOOL _XWindow_HelpBook::Initialize( void )
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

	// Button ----------------------------------------------------------------------------------------=
	int btnimageindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 429, 6 }, { 12, 12 }, _XDEF_HELPBOOK_CLOSE,
									  btnimageindex,btnimageindex,btnimageindex, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
		
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	btnimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Trade.tga" );

	#ifdef _XRUSSIAN	
	_XBTN_STRUCTURE close2btnstruct = { TRUE, { 403-15, 292 }, { 55, 17 }, _XDEF_HELPBOOK_CLOSE,
	#else
	_XBTN_STRUCTURE close2btnstruct = { TRUE, { 403, 292 }, { 41, 17 }, _XDEF_HELPBOOK_CLOSE,
	#endif
									  btnimageindex,btnimageindex,btnimageindex, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pClose2button = new _XButton;
	pClose2button->Create( close2btnstruct );
	#ifdef _XRUSSIAN	
	pClose2button->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 190, 79, 245, 93  );
	pClose2button->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  190, 99, 245, 116 );
	pClose2button->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  190, 119, 245, 136);
	#else
	pClose2button->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 214, 220, 255, 237 );
	pClose2button->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  214, 202, 255, 219 );
	pClose2button->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  214, 238, 255, 255 );
	#endif

	pClose2button->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WAREHOUSE_CLOSE );
	InsertChildObject( pClose2button );

	_XBTN_STRUCTURE leftstepbtnstruct = { TRUE, { 184, 296 }, { 12, 12 }, _XDEF_HELPBOOK_LEFTSTEP,
									  btnimageindex,btnimageindex,btnimageindex, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pLeftStepbutton = new _XButton;
	pLeftStepbutton->Create( leftstepbtnstruct );
	pLeftStepbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 229, 164, 241, 176 );
	pLeftStepbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  229, 176, 241, 188 );
	pLeftStepbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  229, 188, 241, 200 );
	InsertChildObject( pLeftStepbutton );

	_XBTN_STRUCTURE rightstepbtnstruct = { TRUE, { 258, 296 }, { 12, 12 }, _XDEF_HELPBOOK_RIGHTSTEP,
									  btnimageindex,btnimageindex,btnimageindex, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pRightStepbutton = new _XButton;
	pRightStepbutton->Create( rightstepbtnstruct );
	pRightStepbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 241, 164, 253, 176 );
	pRightStepbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  241, 176, 253, 188 );
	pRightStepbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  241, 188, 253, 200 );
	InsertChildObject( pRightStepbutton );




	///////////////////////////////////////////////////////////////////////////////////////
	// Help list 
	///////////////////////////////////////////////////////////////////////////////////////
	int hedekimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
#ifdef _XDEF_HTMLLOADTEST

	_XTREEBOX_STRUCTURE treeboxstructure = 
	{ 
		TRUE,								// 윈도우 활성화
		{ 9, 31 },							// 윈도우 좌표
		{ 186-8, 286-30 },					// 윈도우 사이즈
		_XDEF_HELPBOOK_HELPLIST,			// 윈도우 아이디
		-1,									// 윈도우 border 이미지 인덱스
		&g_MainInterfaceTextureArchive,		// Texture 리스트
		_T(" "),							// 윈도우 타이틀
		14,									// 리스트 아이템 라인 피치
		9,									// 리스트 박스에 보이는 최대 라인수		
		
		// Scroll bar property
		{12, 258},
		12,
		22,
		150,
		hedekimageindex,
		hedekimageindex,
		hedekimageindex,
		hedekimageindex,
		hedekimageindex
	};
	
	m_pTreeBox = new _XTreeBox;
	m_pTreeBox->Create(treeboxstructure);
	
	POINT coloffset = { 2, 2 };
	m_pTreeBox->SetColumnOffset(coloffset);
	m_pTreeBox->SetColumnWidth(164);
	m_pTreeBox->SetTrackHeight(14);
	m_pTreeBox->SetTrackBarColor( D3DCOLOR_ARGB(64,128,128,128) );
	m_pTreeBox->SetAlwaysShowScrollBarMode( FALSE );

	m_pTreeBox->SetMaxViewLineCount( 258 / 14 );
	//RECT backgroundrect = { 0,  12, 12, 24 };
	//RECT iconrect = {241, 164, 253, 176};
	int tb_imageone = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	RECT backgroundrect = { 200,  29, 212, 41 };
	int tb_imagetwo = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	RECT iconrect = {178, 12, 190, 24};
	
	m_pTreeBox->SettingIcon(tb_imagetwo, tb_imageone, iconrect, backgroundrect );

	_XVScrollBar* pScrollBar = m_pTreeBox->GetScrollBarObject();
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

	InsertChildObject( m_pTreeBox );

#endif 

	///////////////////////////////////////////////////////////////////////////////////////
	// Help list 
	///////////////////////////////////////////////////////////////////////////////////////
	_XSCRBAR_STRUCTURE htmlscrbarstructure = 
	{
		TRUE, { m_WindowPosition.x + 430, m_WindowPosition.y + 30 }, 
			12, 258, // width, height
			12,		 // btnysize
			22,		 // trackbar size
			256, 256, 0,
			hedekimageindex,
			hedekimageindex,
			hedekimageindex,
			hedekimageindex,
			hedekimageindex,
			&g_MainInterfaceTextureArchive
	};

	m_HTMLListScrollBar.Create( htmlscrbarstructure );
	m_HTMLListScrollBar.ShowWindow( TRUE );
	m_HTMLListScrollBar.SetViewBorder( TRUE );
	m_HTMLListScrollBar.SetViewNullBorder( FALSE );
	m_HTMLListScrollBar.SetViewLineCount(256);
	m_HTMLListScrollBar.UpdateData();
	m_HTMLListScrollBar.SetPagecount( 64 );
	
	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{ 
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_HTMLListScrollBar.SetClipRect( cliprectstructure );

	m_HTMLViewStatic.Create( m_WindowPosition.x + 195, m_WindowPosition.y + 31, 
		                     m_WindowPosition.x + 429, m_WindowPosition.y + 294, 
							 &g_MainInterfaceTextureArchive, 0 );

	m_HTMLViewStatic.SetClipRect( 0, 0, 233, 256 );

	m_pScrbarFocusedObject = NULL;

	g_HTMLWrapper.SetViewHeight( 256 );

	if( LoadHelpBookData() ) 
	{
		SetHelpHtml(0);
	}
	else
		return FALSE;

	return TRUE;
}

void _XWindow_HelpBook::DestroyWindow( void )
{
	m_pScrbarFocusedObject = NULL;
	if( m_ppHTMLNameList )
	{
		for( int i = 0; i < m_HTMLListCount; i++ )
			SAFE_DELETE_ARRAY(m_ppHTMLNameList[i]);

		SAFE_DELETE_ARRAY( m_ppHTMLNameList );
	}
}


void _XWindow_HelpBook::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;
	
	// Draw Border, Line ---------------------------------------------------------------------=	
	_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, D3DCOLOR_ARGB(178, 0, 0, 0) );
	
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+23, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy, 0.0f,0xFF000000 );
	_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+24, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f,0xFF9F9A84 );
	_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+25, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f,0xFF040000 );
	_XDrawRectAngle( m_WindowPosition.x+3, m_WindowPosition.y+26, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f,0xFF1A211A );

	// left edge
	_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+29, m_WindowPosition.x+188, m_WindowPosition.y+288, 0.0f,0xFF000400 );
	_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+30, m_WindowPosition.x+187, m_WindowPosition.y+287, 0.0f,0xFF1D1F1A );	

	// right edge
	_XDrawRectAngle( m_WindowPosition.x+193, m_WindowPosition.y+29, m_WindowPosition.x+442, m_WindowPosition.y+288, 0.0f,0xFF000000 );
	_XDrawRectAngle( m_WindowPosition.x+194, m_WindowPosition.y+30, m_WindowPosition.x+441, m_WindowPosition.y+287, 0.0f,0xFF302923 );
	// ---------------------------------------------------------------------------------------=
	
	_XWindow::Draw(pfocusobject);
	
	// Title Name
	g_XBaseFont->SetColor( 0xFFE9DC96 );
	TCHAR pagecountstr[64];
	sprintf( pagecountstr, "%d / %d", m_pTreeBox->GetSelectedCommandId(), m_HTMLListCount );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+227, m_WindowPosition.y+296 , _XFONT_ALIGNTYPE_CENTER, pagecountstr );

	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_HELP_2103) );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_HTMLWrapper.UpdateTextureBuffer( m_HTMLListScrollBar.GetLineScrollPos() );
	m_HTMLListScrollBar.Draw( m_pScrbarFocusedObject );

	m_HTMLViewStatic.DrawWithRegion( g_HTMLWrapper.m_pHTMLTexture );
}


BOOL _XWindow_HelpBook::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow )		return FALSE;
	if( !this->m_Enable  )			return FALSE;

	if( m_pScrbarFocusedObject ) return TRUE;
	
	m_HTMLListScrollBar.Process( pfocusobject );

	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

void _XWindow_HelpBook::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );

	m_HTMLViewStatic.MoveWindow( m_WindowPosition.x + 195, m_WindowPosition.y +31 );
	m_HTMLListScrollBar.MoveWindow( m_WindowPosition.x + 430, m_WindowPosition.y + 30 );
}

BOOL _XWindow_HelpBook::LoadHelpBookData( void )
{

#ifdef _XDEF_HTMLLOADTEST

	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* pScript = NULL;
	TCHAR helpfilename[64];

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( helpfilename, _T("vn_helpbook.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( helpfilename, _T("tw_helpbook.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strcpy( helpfilename, _T("us_helpbook.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strcpy( helpfilename, _T("jp_helpbook.txt") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strcpy( helpfilename, _T("rs_helpbook.txt") );
	}
	else		
	{
		strcpy( helpfilename, _T("helpbook.txt") );		
	}
	
	pScript = g_ScriptArchive.GetPackedFile( helpfilename );

	if( !pScript )
	{
		g_ScriptArchive.ClosePackage();
		_XFatalError("Could not read helpbook[%s] file open", helpfilename );
		return FALSE;
	}
	

	TCHAR buffer[256];		
	int parentindex = 0;
	int commandid = 0;
	BOOL bReadCount = FALSE;
	TCHAR htitlename[64];
	TCHAR tempindex[8];	

	while( !feof(pScript) )
	{				
		memset( buffer, 0, 256 );
		if(!fgets(buffer, 256, pScript))
			break;	

		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		if( buffer[0] == ';' || buffer[0] == '\n' || !strlen(buffer) ) continue;

		if( !bReadCount )
		{
			sscanf(buffer, "%d", &m_HTMLListCount);
			if( m_HTMLListCount > 0 )
			{
				m_ppHTMLNameList = new TCHAR*[m_HTMLListCount];
				for( int i = 0; i < m_HTMLListCount; i++ )
				{
					m_ppHTMLNameList[i] = NULL;
					m_ppHTMLNameList[i] = new TCHAR[64];
					memset(m_ppHTMLNameList[i],0, sizeof(TCHAR)*64 );
				}
				bReadCount = TRUE;	
			}
		}
		else
		{
			sscanf( buffer, "%s %s %s", tempindex, htitlename, m_ppHTMLNameList[commandid] );
			parentindex = atoi(tempindex);
			
			if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
			{
				// 일본어는 _ 에 의해 일부 글자가 깨진다. 스페이스 처리 안함.
			}
			else
			{
				ReplaceCharacter(htitlename, _T('_'), _T(' '));
			}

			m_pTreeBox->InsertListItem( htitlename, commandid, parentindex, &g_MainInterfaceTextureArchive );
			if( parentindex > 1 )
				m_pTreeBox->SetItemAttrib( commandid+1, 0xFFD5C58D );
			m_pTreeBox->SetItemAttribHighlight( commandid+1, 0xFFE5E312 );
			commandid++;
		}
	}		
	
	g_ScriptArchive.ClosePackage();
#endif

	return TRUE;
}



void _XWindow_HelpBook::SetHelpHtml( int helpid )
{
#ifdef _XDEF_HTMLLOADTEST

	if( helpid < 0 || helpid > m_HTMLListCount ) return;

	TCHAR urlstring[_XDEF_MAX_URLSTRINGLENGTH];
	strcpy( urlstring, gModulePath );
	strcat( urlstring, _T("\\HelpSystem\\") );

	strcat( urlstring, m_ppHTMLNameList[helpid] );

	g_HTMLWrapper.LoadHTML( urlstring, 233 );
	g_HTMLWrapper.UpdateTextureBuffer( 0 );
	
	m_HTMLListScrollBar.SetTotalLineCount( g_HTMLWrapper.m_uMaxHeight );
	m_HTMLListScrollBar.SetScrollPos( 0 );
	m_HTMLListScrollBar.SetLineScrollPos( 0 );
	m_HTMLListScrollBar.SetPagecount( 64 );	
#endif

}

BOOL _XWindow_HelpBook::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( m_HTMLListScrollBar.ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	if( gpInput->CheckMousePos( m_WindowPosition.x + 195, m_WindowPosition.y + 31, m_WindowPosition.x + 429, m_WindowPosition.y + 294 ) )
	{
		int MoveDelta  = (FLOAT)zDelta;
		
		if( zDelta > 0 )
		{
			m_HTMLListScrollBar.LineUp( abs(MoveDelta) );
		}
		else
		{
			m_HTMLListScrollBar.LineDown( abs(MoveDelta) );
		}

		return TRUE;
	}
		
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

void _XWindow_HelpBook::PrevPage()
{
	m_pTreeBox->SetSelectItemChange(0);
}

void _XWindow_HelpBook::NextPage()
{
	m_pTreeBox->SetSelectItemChange(1);
}

void _XWindow_HelpBook::ShowWindow(BOOL show)
{
	if(show)
	{
		g_MainWindowManager.SetTopWindow(this);
	}
	_XWindow::ShowWindow(show);
}

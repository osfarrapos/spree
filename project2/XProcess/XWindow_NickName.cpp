// XWindow_NickName.cpp: implementation of the _XWindow_NickName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_NickName.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "XMessageWindow.h"
#include "XUser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
BOOL __stdcall _NickNameListMouseHoverCallback( int param1, int param2, int XPos, int YPos )
{
	_XWindow_NickName* pNickNameWindow = (_XWindow_NickName*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NICKNAME);
	if( !pNickNameWindow ) return FALSE;
		
	if( param2 == -1 )
	{
		
	}
	else
	{
		LPTSTR szitemtext = pNickNameWindow->m_listboxGeneralNickName->GetItemText( param2, 0 );
		if( szitemtext )
		{
			int strwidth = g_XBaseFont->GetWidth( szitemtext );
			
			if( strwidth >= 125 )
			{
				g_XBaseFont->SetColor(0xFF669900);
				//_XDrawSolidBar( XPos+38, YPos+4, XPos+38+strwidth+4, YPos+18 , D3DCOLOR_ARGB(180,61,68,73) );
				_XDrawSolidBar( XPos, YPos, XPos+strwidth+4, YPos+g_XBaseFont->GetHeight()+2 , D3DCOLOR_ARGB(255,0,0,0) );
				_XDrawRectAngle( XPos, YPos, XPos+strwidth+4, YPos+g_XBaseFont->GetHeight()+2 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
				
				g_XBaseFont->Puts( XPos+2, YPos+2, szitemtext );

				if( param2 == pNickNameWindow->m_listboxGeneralNickName->GetSelectedItem() )
				{
					g_XBaseFont->Puts( XPos+3, YPos+2, szitemtext );
				}
				
				g_XBaseFont->Flush();
			}
		}
		else
		{
			int spacesize = g_XBaseFont->GetWidth( _T("   ") );
			LPTSTR szitemtext = g_pLifeSkill_Window->m_listboxResourceItem->GetItemText( param2, 1 );
			if( szitemtext )
			{
				int strwidth = g_XBaseFont->GetWidth( szitemtext );
				
				if( strwidth >= (125 - spacesize) )
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
					//_XDrawSolidBar( XPos+18, YPos+4, XPos+18+strwidth+4, YPos+18 , D3DCOLOR_ARGB(180,61,68,73) );
					_XDrawSolidBar( XPos+spacesize, YPos, XPos+strwidth+spacesize+4, YPos+g_XBaseFont->GetHeight()+2 , D3DCOLOR_ARGB(255,0,0,0) );
					_XDrawRectAngle( XPos+spacesize, YPos, XPos+strwidth+spacesize+4, YPos+g_XBaseFont->GetHeight()+2 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
					
					g_XBaseFont->Puts( XPos+spacesize+2, YPos+2, szitemtext );
					
					if( param2 == g_pLifeSkill_Window->m_listboxResourceItem->GetSelectedItem() )
					{
						g_XBaseFont->Puts( XPos+spacesize+3, YPos+2, szitemtext );
					}
					
					g_XBaseFont->Flush();
				}
			}
			
		}
	}
	
	return TRUE;
}

_XWindow_NickName::_XWindow_NickName()
{	
	m_OKButton					= NULL;
	m_DeleteButton				= NULL;
	m_ApplyButton				= NULL;
	
	m_listboxSpecialNickName	= NULL;
	m_listboxGeneralNickName	= NULL;	
	m_listboxUniqueNickName		= NULL;
	m_listboxInfoNickName		= NULL;

	m_SelectedNickNameSlotNumber = -1;
	m_RemainChangeTime = 0;
}

_XWindow_NickName::~_XWindow_NickName()
{
	
}

BOOL _XWindow_NickName::Initialize( void )
{
	int mainframeinterface = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );

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

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {68, 311}, {108, 24},
			_XDEF_NICKNAME_APPLY,
			g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga"),
			g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga"),
			g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga"),
			&g_MainInterfaceTextureArchive
	};

	m_ApplyButton = new _XButton;
	m_ApplyButton->Create(btnstruct);
	m_ApplyButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 176);
	m_ApplyButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
	m_ApplyButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
	m_ApplyButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_ApplyButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MINITOOLTIP_APPLY);
	m_ApplyButton->EnableWindow(TRUE);
	InsertChildObject(m_ApplyButton);
	
	btnstruct.position.x = 178;
	btnstruct.commandid = _XDEF_NICKNAME_NOTAPPLY;
	m_OKButton = new _XButton;
	m_OKButton->Create(btnstruct);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 176);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
	m_OKButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_OKButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSCRIPT_2542);
	m_OKButton->EnableWindow(TRUE);
	InsertChildObject(m_OKButton);

	_XBTN_STRUCTURE delbtnstruct = 
	{
		TRUE, {20, 311}, {40, 24},
			_XDEF_NICKNAME_DELETE,
			g_MainInterfaceTextureArchive.FindResource("MI_Messenger1.tga"),
			g_MainInterfaceTextureArchive.FindResource("MI_Messenger1.tga"),
			g_MainInterfaceTextureArchive.FindResource("MI_Messenger1.tga"),
			&g_MainInterfaceTextureArchive
	};

	m_DeleteButton = new _XButton;
	m_DeleteButton->Create(delbtnstruct);	
	m_DeleteButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 75, 157, 115, 181);
	m_DeleteButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 32, 157, 72, 181);	
	m_DeleteButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 118, 157, 158, 181);
	m_DeleteButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MESSENGER_MAILDEL );
	InsertChildObject(m_DeleteButton);

	// 은거 체크버튼
	int checkbtnimage = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	
	/*_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 115, 316 }, 
											{ 14, 14 }, 
											_XDEF_NICKNAME_RANKHIDING,
											checkbtnimage,checkbtnimage,
											&g_MainInterfaceTextureArchive };

	m_RankHidingButton= new _XCheckButton;
	m_RankHidingButton->Create( checkbtnstruct );	
	m_RankHidingButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_RankHidingButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_RankHidingButton->SetCheck( FALSE );
	m_RankHidingButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_RankHidingButton );*/

	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 275, 5 }, { 12, 12 }, _XDEF_NICKNAME_HELP,
									  g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
									  g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
									  g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
									  &g_MainInterfaceTextureArchive };

	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( closebtnstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );

	closebtnstruct.commandid = _XDEF_NICKNAME_CLOSE;
	closebtnstruct.position.x += 13;

	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
		
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );	
	
	// 특수 별호 리스트
	int scrollbarindex = g_MainInterfaceTextureArchive.FindResource("MI_chat_quick.tga");
	
	_XLISTBOX_STRUCTURE speciallistboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 8, 35 },		// 윈도우 좌표
		{ 138, 60 },	// 윈도우 사이즈
		_XDEF_NICKNAME_SPECIALNICKNAMELIST,			// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// 윈도우 타이틀
		15,												// 리스트 아이템 라인 피치
		4,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치

		// scroll bar property
		{ 12, 60 },										// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		100,											// 전체 리스트 갯수
		scrollbarindex,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex
	};
	
	m_listboxSpecialNickName = new _XListBox;
	m_listboxSpecialNickName->Create(speciallistboxstructure);

	_XStatic* tempstatic = m_listboxSpecialNickName->GetBorderStatic();
	tempstatic->SetFaceColor(0x7F000000);

	_XVScrollBar* pScrollBar = m_listboxSpecialNickName->GetScrollBarObject();
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
	
	POINT coloffset = { 2, 2 };
	m_listboxSpecialNickName->SetColumnOffset(0, coloffset);				// column offset	
	m_listboxSpecialNickName->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style 	
	m_listboxSpecialNickName->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type	
	m_listboxSpecialNickName->SetColumnWidth(0, 125);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxSpecialNickName->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_listboxSpecialNickName->SetSelectedImagePosition(0, 1);
	m_listboxSpecialNickName->SetSelectedImageHeight(15);
	m_listboxSpecialNickName->SetTrackWidth(125);
	m_listboxSpecialNickName->SetScrollPos(0);
	
	InsertChildObject(m_listboxSpecialNickName);
	
	// 고유 별호 리스트
	_XLISTBOX_STRUCTURE uniquelistboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 8, 103 },		// 윈도우 좌표
		{ 138, 20 },	// 윈도우 사이즈
		_XDEF_NICKNAME_UNIQUENICKNAMEIST,			// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// 윈도우 타이틀
		15,												// 리스트 아이템 라인 피치
		1,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_NOSCROLLBAR						// 스크롤바의 위치
	};
	
	m_listboxUniqueNickName = new _XListBox;
	m_listboxUniqueNickName->Create(uniquelistboxstructure);
	tempstatic = m_listboxUniqueNickName->GetBorderStatic();
	tempstatic->SetFaceColor(0x7F000000);		
	
	m_listboxUniqueNickName->SetColumnOffset(0, coloffset);				// column offset	
	m_listboxUniqueNickName->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style 	
	m_listboxUniqueNickName->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type	
	m_listboxUniqueNickName->SetColumnWidth(0, 125);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxUniqueNickName->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_listboxUniqueNickName->SetSelectedImagePosition(0, 1);
	m_listboxUniqueNickName->SetSelectedImageHeight(15);
	m_listboxUniqueNickName->SetTrackWidth(125);
	m_listboxUniqueNickName->SetScrollPos(0);
	
	InsertChildObject(m_listboxUniqueNickName);
	
	// 일반 별호 리스트
	_XLISTBOX_STRUCTURE generallistboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 8, 131 },										// 윈도우 좌표
		{ 138, 174 },	 								// 윈도우 사이즈
		_XDEF_NICKNAME_GENERALNICKNAMEIST,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(""),											// 윈도우 타이틀
		15,												// 리스트 아이템 라인 피치
		11,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		 
		// scroll bar property
		{ 12, 174   },										// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		22,												// 스크롤바 트랙바 사이즈
		150,												// 전체 리스트 갯수
		scrollbarindex,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex
	};
	m_listboxGeneralNickName = new _XListBox;
	m_listboxGeneralNickName->Create(generallistboxstructure);

	tempstatic = m_listboxGeneralNickName->GetBorderStatic();
	tempstatic->SetFaceColor(0x7F000000);

	_XVScrollBar* pgeneralScrollBar = m_listboxGeneralNickName->GetScrollBarObject();
	if( pgeneralScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}			
		};
		pgeneralScrollBar->SetClipRect( cliprectstructure );	
	}

	m_listboxGeneralNickName->SetColumnOffset(0, coloffset);
	m_listboxGeneralNickName->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_listboxGeneralNickName->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_listboxGeneralNickName->SetColumnWidth(0, 125);
	
	m_listboxGeneralNickName->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_listboxGeneralNickName->SetSelectedImagePosition(0, 1);
	m_listboxGeneralNickName->SetSelectedImageHeight(15);
	m_listboxGeneralNickName->SetTrackWidth(125);
	m_listboxGeneralNickName->SetScrollPos(0);


	m_listboxGeneralNickName->SetSelectItemMouseHoverCallBack( _NickNameListMouseHoverCallback );

	InsertChildObject(m_listboxGeneralNickName);

	// 설명, 효과 
	_XLISTBOX_STRUCTURE infolistboxstructure = 
	{
		TRUE,								// 윈도우 활성화
		{ 154,  103 },						// 윈도우 좌표
		{ 146, 202 },						// 윈도우 사이즈
		NULL,								// 윈도우 아이디
		-1,									// 윈도우 border 이미지 인덱스
		&g_MainInterfaceTextureArchive,		// Texture 리스트
		_T(" "),							// 윈도우 타이틀
		15,									// 리스트 아이템 라인 피치
		12,									// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,			// 스크롤바의 위치
		
		// Scroll bar property
		{12, 202},
		12, 
		22,
		50,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex,
		scrollbarindex
	};
	
	m_listboxInfoNickName = new _XListBox;
	m_listboxInfoNickName->Create(infolistboxstructure);
	m_listboxInfoNickName->SetBorderColorFactor( 0x19FFFFFF );
	
	POINT coloffset2 = { 7, 6 };
	m_listboxInfoNickName->SetColumnOffset(0, coloffset2);
	m_listboxInfoNickName->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_listboxInfoNickName->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_listboxInfoNickName->SetColumnWidth(0, 125 );
	m_listboxInfoNickName->SetTrackWidth(125);
	m_listboxInfoNickName->SetTrackHeight(80);
	m_listboxInfoNickName->SetTrackBarColor( D3DCOLOR_ARGB(128,0,0,0) );	
	m_listboxInfoNickName->EnableSelectItem(FALSE);		
	
	_XVScrollBar* pinfolistScrollBar = m_listboxInfoNickName->GetScrollBarObject();
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
		pinfolistScrollBar->SetClipRect( cliprectstructure );	
	}
	
	InsertChildObject(m_listboxInfoNickName);

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

	return TRUE;
}

void _XWindow_NickName::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow(X, Y);

	m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 306, m_WindowPosition.y + 342 );
	m_BorderGradationTitle.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24 );
}

void _XWindow_NickName::SetDefaultPosition( void )
{
	_XWindow_MainFrame* pMainFrame_Window = (_XWindow_MainFrame*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MAINFRAME);

	POINT pos = pMainFrame_Window->GetWindowPos();
	SIZE size = pMainFrame_Window->GetWindowSize();
	MoveWindow( pos.x + size.cx, pos.y );
}

void _XWindow_NickName::SaveWindowProperty( void )
{	
	//	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_FSWINDOWXPOS,   GetWindowPos().x );
	//	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_FSWINDOWYPOS,   GetWindowPos().y );	
	
}

void _XWindow_NickName::ShowWindow( BOOL show )
{
	if( show )
	{
		SetNickNameList();
	}
	else
	{
		if( g_pCInfo_Window )
			g_pCInfo_Window->m_pNickNameTabbutton[2]->SetCheck( FALSE );
	}

	_XWindow::ShowWindow( show );
}

void _XWindow_NickName::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();
}

void _XWindow_NickName::Draw( _XGUIObject*& pfocusobject )
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

		// 왼상
		_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+34, m_WindowPosition.x+147, m_WindowPosition.y+96, 0.0f,0xFF000400 );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+35, m_WindowPosition.x+146, m_WindowPosition.y+95, 0.0f,0xFF302923 );

		// 왼중
		_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+102, m_WindowPosition.x+147, m_WindowPosition.y+124, 0.0f,0xFF000400 );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+103, m_WindowPosition.x+146, m_WindowPosition.y+123, 0.0f,0xFF302923 );

		// 왼하
		_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+130, m_WindowPosition.x+147, m_WindowPosition.y+306, 0.0f,0xFF000400 );
		_XDrawRectAngle( m_WindowPosition.x+7, m_WindowPosition.y+131, m_WindowPosition.x+146, m_WindowPosition.y+305, 0.0f,0xFF302923 );

		// 우 선택된 별호
		_XDrawSolidBar( m_WindowPosition.x+152, m_WindowPosition.y+33, m_WindowPosition.x+302, m_WindowPosition.y+81, 0x7F000400 );
		_XDrawRectAngle( m_WindowPosition.x+153, m_WindowPosition.y+34, m_WindowPosition.x+300, m_WindowPosition.y+79, 0.0f, 0xFF9F9A84 );		
		_XDrawRectAngle( m_WindowPosition.x+154, m_WindowPosition.y+35, m_WindowPosition.x+299, m_WindowPosition.y+78, 0.0f,0xFF302923 );		

		// 우 설명창 
		_XDrawRectAngle( m_WindowPosition.x+152, m_WindowPosition.y+102, m_WindowPosition.x+301, m_WindowPosition.y+306, 0.0f,0xFF000400 );
		_XDrawRectAngle( m_WindowPosition.x+153, m_WindowPosition.y+103, m_WindowPosition.x+300, m_WindowPosition.y+305, 0.0f,0xFF302923 );

		// 별호 분류명은 아이디가 1000 이상으로 설정되어 있음 
		if( m_SelectedNickNameSlotNumber > 0 && m_SelectedNickNameSlotNumber < 1000 )
		{
			g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 230, 200, 120 ));
			g_XBaseFont->Puts_Separate( m_WindowPosition.x+163, m_WindowPosition.y+44, g_NickNameInfoTable[m_listboxNickIndex[m_SelectedNickNameSlotNumber]].cNickName, 126, 1.0, 3 );
			g_XBaseFont->Puts_Separate( m_WindowPosition.x+164, m_WindowPosition.y+44, g_NickNameInfoTable[m_listboxNickIndex[m_SelectedNickNameSlotNumber]].cNickName, 126, 1.0, 3 );
		}
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		if( m_RemainChangeTime == 0 )
			g_XBaseFont->PrintAlign( m_WindowPosition.x+227, m_WindowPosition.y+85, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2543) );
		else
		{
			int time = m_RemainChangeTime / 60;
			if( time > 0 )
			{
				int param2 = m_RemainChangeTime%60;												
				g_XBaseFont->PutsAlign( m_WindowPosition.x+227, m_WindowPosition.y+85, _XFONT_ALIGNTYPE_CENTER, 
										g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_NICKNAME_2544, &time, &param2 ) );
			}
			else
				g_XBaseFont->PrintAlign( m_WindowPosition.x+227, m_WindowPosition.y+85, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1861), m_RemainChangeTime );
		}
	}
	else
		return;

	_XWindow::Draw(pfocusobject);	
	
	// Title Name
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFD7D7D7);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CINFO_NICKNAME) );
	//g_XBaseFont->PutsAlign( m_WindowPosition.x + 95, m_WindowPosition.y + 317, _XFONT_ALIGNTYPE_CENTER, _T("은거") );

	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
}


BOOL _XWindow_NickName::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}

void _XWindow_NickName::SetNickNameList( void )
{
	m_listboxSpecialNickName->DeleteAllItem();
	m_listboxGeneralNickName->DeleteAllItem();
	m_listboxUniqueNickName->DeleteAllItem();
	m_listboxInfoNickName->DeleteAllItem();

	memset( m_listboxNickIndex, 0, sizeof(m_listboxNickIndex));
	
	int speciallistcount = 0;
	int generallistcount = 0;

	bool flag_showdown = false;
	bool flag_quest = false;
	bool flag_group = false;
	bool flag_make = false;
	bool flag_gater = false;
	bool flag_event = false;
	bool flag_mon = false;	

	TCHAR ChangeStr[256];

	// 고유 별호
	for( int i = 0; i < _XDEF_NICKNAME_TOTALCOUNT; i++)
	{
		if( i >=1 && i < 11 )// // 특수비무별호 
		{
			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}
			if( tableindex != -1 )
			{
				TCHAR tempNickname[256];
				ShortenedNickName( tempNickname, g_NickNameInfoTable[tableindex].cNickName );
				
				m_listboxSpecialNickName->InsertListItem(_T(""), 0, i );		
				m_listboxSpecialNickName->SetItemText(speciallistcount, 0, tempNickname);
				m_listboxSpecialNickName->SetItemAttrib(speciallistcount, 0, D3DCOLOR_ARGB(255, 250, 160, 40));
				m_listboxSpecialNickName->SetItemAttribHighlight(speciallistcount, 0, D3DCOLOR_ARGB( 255, 250, 160, 40 ));
				m_listboxNickIndex[i] = tableindex;
				speciallistcount++;
			}			
		}
		else if( i >=11 && i < 21 )// 특수문파별호
		{
			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;					
					break;
				}
			}
			if( tableindex != -1 )
			{
				TCHAR tempNickname[256];
				ShortenedNickName( tempNickname, g_NickNameInfoTable[tableindex].cNickName );

				m_listboxSpecialNickName->InsertListItem(_T(""), 0, i );	
				m_listboxSpecialNickName->SetItemText(speciallistcount, 0, tempNickname);
				m_listboxSpecialNickName->SetItemAttrib(speciallistcount, 0, D3DCOLOR_ARGB(255, 250, 160, 40));
				m_listboxSpecialNickName->SetItemAttribHighlight(speciallistcount, 0, D3DCOLOR_ARGB( 255, 250, 160, 40 ));
				m_listboxNickIndex[i] = tableindex;
				speciallistcount++;
			}			
		}
		else if( i >=21 && i < 31 )// 특수등급별호
		{
			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}

			if( tableindex != -1 )
			{			
				TCHAR tempNickname[256];
				ShortenedNickName( tempNickname, g_NickNameInfoTable[tableindex].cNickName );

				m_listboxSpecialNickName->InsertListItem(_T(""), 0, i );	
				m_listboxSpecialNickName->SetItemText(speciallistcount, 0, tempNickname);
				m_listboxSpecialNickName->SetItemAttrib(speciallistcount, 0, D3DCOLOR_ARGB(255, 250, 160, 40));
				m_listboxSpecialNickName->SetItemAttribHighlight(speciallistcount, 0, D3DCOLOR_ARGB( 255, 250, 160, 40 ));
				m_listboxNickIndex[i] = tableindex;
				speciallistcount++;
			}
		}
		else if( i >=31 && i < 81 )// 특수무공별호
		{
			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}
			if( tableindex != -1 )
			{
				TCHAR tempNickname[256];
				ShortenedNickName( tempNickname, g_NickNameInfoTable[tableindex].cNickName );
				
				m_listboxSpecialNickName->InsertListItem(_T(""), 0, i );	
				m_listboxSpecialNickName->SetItemText(speciallistcount, 0, tempNickname);
				m_listboxSpecialNickName->SetItemAttrib(speciallistcount, 0, D3DCOLOR_ARGB(255, 250, 160, 40));
				m_listboxSpecialNickName->SetItemAttribHighlight(speciallistcount, 0, D3DCOLOR_ARGB( 255, 250, 160, 40 ));
				m_listboxNickIndex[i] = tableindex;
				speciallistcount++;
			}
		}	
		else if( i >= 81 && i < 91 )	// 비무별호
		{
			if( !flag_showdown )
			{
				flag_showdown = true;
				
				int tempCount = 0;
				for( int ii = 81; ii < 91; ii++ )
				{
					if( g_pLocalUser->m_CharacterInfo.m_NickNameList[ii] > 0)
						tempCount++;
				}

				if( tempCount > 0 )
				{			
					TCHAR tempTitleName[256];
					memset(tempTitleName, 0, sizeof(TCHAR)*256);
					sprintf( tempTitleName, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2545), tempCount );

					m_listboxGeneralNickName->InsertListItem(_T(""), 0, i+1000 );
					m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempTitleName);
					
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, 0xFF669900);
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, 0xFF669900);
					
					generallistcount++;
				}
			}

			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}
			if( tableindex != -1 )
			{	
				TCHAR tempNickname[256];
				memset(ChangeStr, 0, sizeof(TCHAR)*256);
				sprintf( ChangeStr, "   %s", g_NickNameInfoTable[tableindex].cNickName );
				ShortenedNickName( tempNickname, ChangeStr );

				m_listboxGeneralNickName->InsertListItem(_T(""), 0, i );
				m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempNickname);
				if( g_NickNameInfoTable[tableindex].cType2 == 1)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 230, 200, 120));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 230, 200, 120 ));
				}
				else if( g_NickNameInfoTable[tableindex].cType2 == 2)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 180, 180, 180 ));
				}
				m_listboxNickIndex[i] = tableindex;
				generallistcount++;			
			}
		}
		else if( i >= 91 && i < 141 )	// 풍운록 별호
		{
			if( !flag_quest )
			{
				flag_quest = true;
				
				int tempCount = 0;
				for( int ii = 91; ii < 141; ii++ )
				{
					if( g_pLocalUser->m_CharacterInfo.m_NickNameList[ii] > 0)
						tempCount++;
				}

				if( tempCount > 0 )
				{			
					TCHAR tempTitleName[256];
					memset(tempTitleName, 0, sizeof(TCHAR)*256);
					sprintf( tempTitleName, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2546), tempCount );

					m_listboxGeneralNickName->InsertListItem(_T(""), 0, i+1000 );
					m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempTitleName);
					
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, 0xFF669900);
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, 0xFF669900);
					
					generallistcount++;
				}
			}

			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}
			if( tableindex != -1 )
			{	
				TCHAR tempNickname[256];

				memset(ChangeStr, 0, sizeof(TCHAR)*256);
				sprintf( ChangeStr, "  %s", g_NickNameInfoTable[tableindex].cNickName );
				ShortenedNickName( tempNickname, ChangeStr );

				m_listboxGeneralNickName->InsertListItem(_T(""), 0, i );
				m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempNickname);
				if( g_NickNameInfoTable[tableindex].cType2 == 1)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 230, 200, 120));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 230, 200, 120 ));
				}
				else if( g_NickNameInfoTable[tableindex].cType2 == 2)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 180, 180, 180 ));
				}				
				m_listboxNickIndex[i] = tableindex;
				generallistcount++;			
			}
		}
		else if( i == 141 )	// 문파 별호
		{
			if( !flag_group )
			{
				flag_group = true;
				
				if( g_pLocalUser->m_CharacterInfo.m_NickNameList[i] > 0)
				{
					TCHAR tempTitleName[256];
					memset(tempTitleName, 0, sizeof(TCHAR)*256);
					strcpy( tempTitleName, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2547) );

					m_listboxGeneralNickName->InsertListItem(_T(""), 0, i+1000 );
					m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempTitleName);
					
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, 0xFF669900);
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, 0xFF669900);
					
					generallistcount++;
				}
			}

			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}

			if( tableindex != -1 )
			{
				TCHAR tempNickname[256];

				memset(ChangeStr, 0, sizeof(TCHAR)*256);
				sprintf( ChangeStr, "  %s", g_NickNameInfoTable[tableindex].cNickName );
				ShortenedNickName( tempNickname, ChangeStr );

				m_listboxGeneralNickName->InsertListItem(_T(""), 0, i );
				m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempNickname);
				if( g_NickNameInfoTable[tableindex].cType2 == 1)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 230, 200, 120));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 230, 200, 120 ));
				}
				else if( g_NickNameInfoTable[tableindex].cType2 == 2)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 180, 180, 180 ));
				}
				m_listboxNickIndex[i] = tableindex;
				generallistcount++;			
			}
		}
		else if( i >= 142 && i < 152 )	// 제작별호
		{
			if( !flag_make )
			{
				flag_make = true;
				
				int tempCount = 0;
				for( int ii = 142; ii < 152; ii++ )
				{
					if( g_pLocalUser->m_CharacterInfo.m_NickNameList[ii] > 0)
						tempCount++;
				}

				if( tempCount > 0 )
				{			
					TCHAR tempTitleName[256];
					memset(tempTitleName, 0, sizeof(TCHAR)*256);
					sprintf( tempTitleName, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2548), tempCount );

					m_listboxGeneralNickName->InsertListItem(_T(""), 0, i+1000 );
					m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempTitleName);
					
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, 0xFF669900);
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, 0xFF669900);
					
					generallistcount++;
				}
			}

			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}
			if( tableindex != -1 )
			{	
				TCHAR tempNickname[256];

				memset(ChangeStr, 0, sizeof(TCHAR)*256);
				sprintf( ChangeStr, "  %s", g_NickNameInfoTable[tableindex].cNickName );
				ShortenedNickName( tempNickname, ChangeStr );

				m_listboxGeneralNickName->InsertListItem(_T(""), 0, i );
				m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempNickname);

				if( g_NickNameInfoTable[tableindex].cType2 == 1)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 230, 200, 120));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 230, 200, 120 ));
				}
				else if( g_NickNameInfoTable[tableindex].cType2 == 2)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 180, 180, 180 ));
				}				
				m_listboxNickIndex[i] = tableindex;
				generallistcount++;			
			}
		}
		else if( i >= 152 && i < 162 )	// 수집별호
		{

			if( !flag_gater )
			{
				flag_gater = true;
				
				int tempCount = 0;
				for( int ii = 152; ii < 162; ii++ )
				{
					if( g_pLocalUser->m_CharacterInfo.m_NickNameList[ii] > 0)
						tempCount++;
				}

				if( tempCount > 0 )
				{			
					TCHAR tempTitleName[256];
					memset(tempTitleName, 0, sizeof(TCHAR)*256);
					sprintf( tempTitleName, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2549), tempCount );

					m_listboxGeneralNickName->InsertListItem(_T(""), 0, i+1000 );
					m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempTitleName);
					
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, 0xFF669900);
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, 0xFF669900);
					
					generallistcount++;
				}
			}

			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}
			if( tableindex != -1 )
			{	
				TCHAR tempNickname[256];

				memset(ChangeStr, 0, sizeof(TCHAR)*256);
				sprintf( ChangeStr, "  %s", g_NickNameInfoTable[tableindex].cNickName );
				ShortenedNickName( tempNickname, ChangeStr );

				m_listboxGeneralNickName->InsertListItem(_T(""), 0, i );
				m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempNickname);
				if( g_NickNameInfoTable[tableindex].cType2 == 1)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 230, 200, 120));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 230, 200, 120 ));
				}
				else if( g_NickNameInfoTable[tableindex].cType2 == 2)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 180, 180, 180 ));
				}
				m_listboxNickIndex[i] = tableindex;
				generallistcount++;			
			}
		}
		else if( i >= 162 && i < 182 )	// 이벤트별호
		{
			if( !flag_event )
			{
				flag_event = true;
				
				int tempCount = 0;
				for( int ii = 162; ii < 182; ii++ )
				{
					if( g_pLocalUser->m_CharacterInfo.m_NickNameList[ii] > 0)
						tempCount++;
				}

				if( tempCount > 0 )
				{			
					TCHAR tempTitleName[256];
					memset(tempTitleName, 0, sizeof(TCHAR)*256);
					sprintf( tempTitleName, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2550), tempCount );

					m_listboxGeneralNickName->InsertListItem(_T(""), 0, i+1000 );
					m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempTitleName);
					
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, 0xFF669900);
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, 0xFF669900);
					
					generallistcount++;
				}
			}

			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}
			if( tableindex != -1 )
			{	
				TCHAR tempNickname[256];

				memset(ChangeStr, 0, sizeof(TCHAR)*256);
				sprintf( ChangeStr, "  %s", g_NickNameInfoTable[tableindex].cNickName );
				ShortenedNickName( tempNickname, ChangeStr );

				m_listboxGeneralNickName->InsertListItem(_T(""), 0, i );
				m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempNickname);
				if( g_NickNameInfoTable[tableindex].cType2 == 1)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 230, 200, 120));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 230, 200, 120 ));
				}
				else if( g_NickNameInfoTable[tableindex].cType2 == 2)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 180, 180, 180 ));
				}
				m_listboxNickIndex[i] = tableindex;
				generallistcount++;			
			}
		}
		else if( i >= 182 && i < 212 )	// 몹관련별호
		{
			if( !flag_mon )
			{
				flag_mon = true;
				
				int tempCount = 0;
				for( int ii = 182; ii < 212; ii++ )
				{
					if( g_pLocalUser->m_CharacterInfo.m_NickNameList[ii] > 0)
						tempCount++;
				}

				if( tempCount > 0 )
				{			
					TCHAR tempTitleName[256];
					memset(tempTitleName, 0, sizeof(TCHAR)*256);
					sprintf( tempTitleName, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2551), tempCount );

					m_listboxGeneralNickName->InsertListItem(_T(""), 0, i+1000 );
					m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempTitleName);
					
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, 0xFF669900);
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, 0xFF669900);
					
					generallistcount++;
				}
			}

			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[i] )
				{
					tableindex = j;
					break;
				}
			}
			if( tableindex != -1 )
			{		
				TCHAR tempNickname[256];

				memset(ChangeStr, 0, sizeof(TCHAR)*256);
				sprintf( ChangeStr, "  %s", g_NickNameInfoTable[tableindex].cNickName );
				ShortenedNickName( tempNickname, ChangeStr );
				
				m_listboxGeneralNickName->InsertListItem(_T(""), 0, i );
				m_listboxGeneralNickName->SetItemText(generallistcount, 0, tempNickname);
				if( g_NickNameInfoTable[tableindex].cType2 == 1)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 230, 200, 120));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 230, 200, 120 ));
				}
				else if( g_NickNameInfoTable[tableindex].cType2 == 2)
				{
					m_listboxGeneralNickName->SetItemAttrib(generallistcount, 0, D3DCOLOR_ARGB(255, 180, 180, 180));
					m_listboxGeneralNickName->SetItemAttribHighlight(generallistcount, 0, D3DCOLOR_ARGB( 255, 180, 180, 180 ));
				}
				m_listboxNickIndex[i] = tableindex;
				generallistcount++;			
			}
		}
	}
	
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI
	if(g_pLocalUser->m_nWinnerNickName > 0)	//비무 별호 처리
	{
		int tableindex = -1;
		for(int j = 0; j < g_MaxNickNameCount; j++)
		{
			if(g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_nWinnerNickName)
			{
				tableindex = j;
				break;
			}
		}
		if( tableindex != -1 )
		{
			TCHAR tempNickname[256];
			ShortenedNickName(tempNickname, g_NickNameInfoTable[tableindex].cNickName);
			
			m_listboxSpecialNickName->InsertListItem(_T(""), 0, g_pLocalUser->m_nWinnerNickName+10000);
			m_listboxSpecialNickName->SetItemText(speciallistcount, 0, tempNickname);
			m_listboxSpecialNickName->SetItemAttrib(speciallistcount, 0, D3DCOLOR_ARGB(255, 250, 160, 40));
			m_listboxSpecialNickName->SetItemAttribHighlight(speciallistcount, 0, D3DCOLOR_ARGB( 255, 250, 160, 40));
			m_listboxNickIndex[212] = tableindex;
			speciallistcount++;
		}	
	}
#endif
}

void _XWindow_NickName::SetSelectedNickNameInfo( void )
{
	if( m_listboxInfoNickName )
	{
		m_listboxInfoNickName->DeleteAllItem();

		_XStringSplitInfo splitinfo;
		int viewsize = 126;
		
		BOOL bFreeMatch = FALSE;
		int tableindex = 0;
		if(g_pLocalUser->m_nWinnerNickName == m_SelectedNickNameSlotNumber-10000)	// 자유비무 별호
		{
			tableindex = m_listboxNickIndex[212];
			bFreeMatch = TRUE;
		}
		else
		{
			tableindex = m_listboxNickIndex[m_SelectedNickNameSlotNumber];
		}
		
		if( (m_SelectedNickNameSlotNumber > 0 && m_SelectedNickNameSlotNumber < 1000) || bFreeMatch)
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
			{
				g_XBaseFont->SplitString(g_NickNameInfoTable[tableindex].cNickInfo, viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
			}
			else
			{
				g_XBaseFont->SplitString(g_NickNameInfoTable[tableindex].cNickInfo, viewsize, splitinfo);
			}

			TCHAR	splitstring[256];		
			for(int i = 0; i < splitinfo.splitcount ; i++)
			{
				memset(splitstring, 0, sizeof(TCHAR)*256);
				strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
				
				m_listboxInfoNickName->InsertListItem( splitstring, _XSC_DEFAULT );
				m_listboxInfoNickName->SetItemCommandID( m_listboxInfoNickName->GetItemCount()-1, 0 );
				m_listboxInfoNickName->SetItemAttrib( m_listboxInfoNickName->GetItemCount()-1, 0, _XSC_DEFAULT );
			}

			// 빈 라인 
			m_listboxInfoNickName->InsertListItem( _T(""), _XSC_DEFAULT );
			m_listboxInfoNickName->SetItemCommandID( m_listboxInfoNickName->GetItemCount()-1, 0 );
			m_listboxInfoNickName->SetItemAttrib( m_listboxInfoNickName->GetItemCount()-1, 0, _XSC_DEFAULT );
			
			for( i = 0; i < 5; i++ )
			{
				if( g_NickNameInfoTable[tableindex]._Effect[i].usEffectIndex > 0 )
				{
					int effindex = g_NickNameInfoTable[tableindex]._Effect[i].usEffectIndex;
					TCHAR temp_buf[255];
					memset( temp_buf, 0, sizeof(TCHAR)*255);
					
					map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(effindex);
					if(iter_table != g_CharacterStateTable.end())
					{
						if( strlen(g_CharacterStateTable[effindex]->concept) )
						{
							int tempcount = 0;
							for( int ii = 0; ii < strlen( g_CharacterStateTable[effindex]->concept ); ii++ )
							{
								if( g_CharacterStateTable[effindex]->concept[ii] == '%' )
									tempcount++;
							}

							switch(tempcount) 
							{
							case 3:
								{
									// 리스트 박스에 추가되면 %가 지워지므로 임의로 %를 한번더 넣어준다.
									TCHAR temp_buf2[255];
									memset( temp_buf2, 0, sizeof(TCHAR)*255);

									sprintf( temp_buf, g_CharacterStateTable[effindex]->concept, 
										g_NickNameInfoTable[tableindex]._Effect[i].cEffectProbability );

									/*char* ptr = strrchr(temp_buf2, '%');
									if( ptr != NULL )
									{
										int point = strlen(ptr);
										point = strlen( temp_buf2 ) - point+1;
										strncpy( temp_buf, temp_buf2, point );
										strcat( temp_buf, ptr );
									}*/
								}
								break;
							case 1:
								{									
									sprintf( temp_buf, g_CharacterStateTable[effindex]->concept, 
										g_NickNameInfoTable[tableindex]._Effect[i].usEffectValue );
								}
								break;
							default:
								{
									sprintf( temp_buf, g_CharacterStateTable[effindex]->concept );
								}
								break;
							}
						}
						
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
						{
							g_XBaseFont->SplitString(temp_buf, viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);
						}
						else
						{
							g_XBaseFont->SplitString(temp_buf, viewsize, splitinfo);
						}
						TCHAR	splitstring[256];		
						for( int j = 0; j < splitinfo.splitcount ; j++)
						{
							memset(splitstring, 0, sizeof(TCHAR)*256);
							strncpy(splitstring, splitinfo.splitpoint[j], splitinfo.splitlength[j]);
							
							m_listboxInfoNickName->InsertListItem( splitstring, 0xFFE7B329 );
							m_listboxInfoNickName->SetItemCommandID( m_listboxInfoNickName->GetItemCount()-1, 0 );
							m_listboxInfoNickName->SetItemAttrib( m_listboxInfoNickName->GetItemCount()-1, 0, 0xFFE7B329 );
						}
					}										
				}
			}
		}				
	}
}

void _XWindow_NickName::ShortenedNickName( TCHAR* comNick, TCHAR* srcNick )
{
	memset( comNick, 0, sizeof(TCHAR)*256 );

	int length = strlen( srcNick );	

	if( length > 20 )
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN || g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE ||
			g_LanguageType == _XLANGUAGE_TYPE_CHINESE || g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
		{
			if( srcNick[18] & 0x80 ) //한글일때 2바이트 
			{
				int compairindex = 0;
				while (compairindex < 18) 
				{
					if( srcNick[compairindex] & 0x80 )
						compairindex+= 2;
					else
						compairindex++;
				}
				
				strncpy( comNick, srcNick, sizeof(TCHAR)*compairindex );	
				strcat( comNick, _T("..") );
			}
			else
			{
				strncpy( comNick, srcNick, sizeof(TCHAR)*18 );	
				strcat( comNick, _T("..") );
			}
		}
		else
		{
			strncpy( comNick, srcNick, sizeof(TCHAR)*18 );	
			strcat( comNick, _T("..") );
		}
	}
	else
	{
		strcpy( comNick, srcNick );
	}
}

void _XWindow_NickName::ResetNicknameInfo(void)
{
	m_listboxSpecialNickName->DeleteAllItem();
	m_listboxGeneralNickName->DeleteAllItem();
	m_listboxUniqueNickName->DeleteAllItem();
	m_listboxInfoNickName->DeleteAllItem();

	memset( m_listboxNickIndex, 0, sizeof(m_listboxNickIndex));
	m_SelectedNickNameSlotNumber = 0;
	m_RemainChangeTime = 0;
}

void _XWindow_NickName::CheckDeleteNickname(void)
{
	// 비무 별호 선택시
	if(m_SelectedNickNameSlotNumber > 10000)
	{
		int tableindex = -1;
		for( int j = 0; j < g_MaxNickNameCount; j++)
		{
			if( g_NickNameInfoTable[j].usIndex == (m_SelectedNickNameSlotNumber - 10000) )
			{
				tableindex = j;
				break;
			}
		}

		if( tableindex != -1 )
		{			
			TCHAR tempStr[128];
			memset( tempStr, 0, sizeof(TCHAR)*128);
			sprintf( tempStr, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2553), g_NickNameInfoTable[tableindex].cNickName);
			g_NetworkKernel.InsertChatString(tempStr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
		}
		return;
	}

	if( g_NickNameInfoTable[m_listboxNickIndex[m_SelectedNickNameSlotNumber]].cType3 == 12 ||
		g_NickNameInfoTable[m_listboxNickIndex[m_SelectedNickNameSlotNumber]].cType3 == 16 ||
		g_NickNameInfoTable[m_listboxNickIndex[m_SelectedNickNameSlotNumber]].cType3 == 17 )	
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
		if( pMessageBox )
		{
			int tableindex = -1;
			for( int j = 0; j < g_MaxNickNameCount; j++)
			{
				if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[m_SelectedNickNameSlotNumber] )
				{
					tableindex = j;
					break;
				}
			}
			
			if( tableindex != -1 )
			{
				TCHAR tempStr[128];
				memset( tempStr, 0, sizeof(TCHAR)*128);
				sprintf( tempStr, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2552), g_NickNameInfoTable[tableindex].cNickName);
				pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), tempStr, TRUE, _XDEF_NICKNAME_DELETE_YES, _XDEF_NICKNAME_DELETE_NO);	
			}
		}
	}
	else
	{
		int tableindex = -1;
		for( int j = 0; j < g_MaxNickNameCount; j++)
		{
			if( g_NickNameInfoTable[j].usIndex == g_pLocalUser->m_CharacterInfo.m_NickNameList[m_SelectedNickNameSlotNumber] )
			{
				tableindex = j;
				break;
			}
		}

		if( tableindex != -1 )
		{			
			TCHAR tempStr[128];
			memset( tempStr, 0, sizeof(TCHAR)*128);
			sprintf( tempStr, _XGETINTERFACETEXT(ID_STRING_NICKNAME_2553), g_NickNameInfoTable[tableindex].cNickName);
			g_NetworkKernel.InsertChatString(tempStr, _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
		}
	}
}

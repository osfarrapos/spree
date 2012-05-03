// XWindow_LifeSkill.cpp: implementation of the _XWindow_LifeSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_LifeSkill.h"
#include "XWindowObjectDefine.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"
#include "SoundEffectList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL __stdcall _ResourceNeedItemListBoxItemMouseHoverCallback( int param1, int param2, int XPos, int YPos )
{
	if( param2 == -1 )
	{
		
	}
	else
	{
		LPTSTR szitemtext = g_pLifeSkill_Window->m_listboxResourceNeedItem->GetItemText( param2, 2 );
		if( szitemtext )
		{
			int strwidth = g_XBaseFont->GetWidth( szitemtext );
			
			if( strwidth >= 136 )
			{
				LPTSTR szitemtext1 = g_pLifeSkill_Window->m_listboxResourceNeedItem->GetItemText( param2, 3 );
				if( szitemtext1 )
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
					_XDrawSolidBar( XPos+34, YPos+1, XPos+34+strwidth+4, YPos+34 , D3DCOLOR_ARGB(255,0,0,0) );
					_XDrawRectAngle( XPos+34, YPos+1, XPos+34+strwidth+4, YPos+34 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
					
					g_XBaseFont->Puts( XPos+36, YPos+4, szitemtext );
					g_XBaseFont->Puts( XPos+36, YPos+18, szitemtext1 );
				
				
					if( param2 == g_pLifeSkill_Window->m_listboxResourceNeedItem->GetSelectedItem() )
					{
						g_XBaseFont->Puts( XPos+37, YPos+4, szitemtext );
						g_XBaseFont->Puts( XPos+37, YPos+18, szitemtext1 );
					}
					
					g_XBaseFont->Flush();
				}
			}
		}
	}
	
	return TRUE;
}

BOOL __stdcall _ResourceItemListBoxItemMouseHoverCallback( int param1, int param2, int XPos, int YPos )
{
	if( param2 == -1 )
	{
		
	}
	else
	{
		LPTSTR szitemtext = g_pLifeSkill_Window->m_listboxResourceItem->GetItemText( param2, 3 );
		if( szitemtext )
		{
			int strwidth = g_XBaseFont->GetWidth( szitemtext );
			
			if( strwidth >= 94 )
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
				_XDrawSolidBar( XPos+38, YPos+4, XPos+38+strwidth+4, YPos+18 , D3DCOLOR_ARGB(180,0,0,0) );
				//_XDrawSolidBar( XPos+38, YPos+4, XPos+38+strwidth+4, YPos+18 , D3DCOLOR_ARGB(255,0,0,0) );
				_XDrawRectAngle( XPos+38, YPos+4, XPos+38+strwidth+4, YPos+18 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
				
				g_XBaseFont->Puts( XPos+40, YPos+6, szitemtext );
				
				if( param2 == g_pLifeSkill_Window->m_listboxResourceItem->GetSelectedItem() )
				{
					g_XBaseFont->Puts( XPos+41, YPos+6, szitemtext );
				}
				
				g_XBaseFont->Flush();
			}
		}
		else
		{
			LPTSTR szitemtext = g_pLifeSkill_Window->m_listboxResourceItem->GetItemText( param2, 1 );
			if( szitemtext )
			{
				int strwidth = g_XBaseFont->GetWidth( szitemtext );
				
				if( strwidth >= 114 )
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
					_XDrawSolidBar( XPos+18, YPos+4, XPos+18+strwidth+4, YPos+18 , D3DCOLOR_ARGB(180,0,0,0) );
					//_XDrawSolidBar( XPos+18, YPos+4, XPos+18+strwidth+4, YPos+18 , D3DCOLOR_ARGB(255,0,0,0) );
					_XDrawRectAngle( XPos+18, YPos+4, XPos+18+strwidth+4, YPos+18 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
					
					g_XBaseFont->Puts( XPos+20, YPos+6, szitemtext );
					
					if( param2 == g_pLifeSkill_Window->m_listboxResourceItem->GetSelectedItem() )
					{
						g_XBaseFont->Puts( XPos+21, YPos+6, szitemtext );
					}
					
					g_XBaseFont->Flush();
				}
			}
			
		}
	}
	
	return TRUE;
}

_XWindow_LifeSkill::_XWindow_LifeSkill()
{ 
	// 공통
	m_imageSelectedButton			= NULL;		// 정사각형 버튼이 선택되었을 때 테두리 이미지
	m_imageTypeButtonBorder			= NULL;		// 정사각형 버튼이 바탕 이미지
	m_imageItemBorder				= NULL;		// 아이템 테두리
	m_imageCompleteItemBorder		= NULL;		// 아이템 테두리

	// 수집 관련 탭 
	m_imageCollectionTypeBorder1	= NULL;		// 수집 타입 바탕 생성 1 
	m_imageCollectionTypeBorder2	= NULL;		// 수집 타입 바탕 생성 2 
	
	// 자원 관련 탭 
	
	// 행동 관련 탭
	m_imageStoreBorder1				= NULL;		// 개인 상점 바탕 생성 1 ( 44 X 46 )
	m_imageStoreBorder2				= NULL;		// 개인 상점 바탕 생성 2 ( 100 X 46 )
	m_imageStoreBorder3				= NULL;		// 개인 상점 바탕 생성 3 ( 12 X 46 )
	m_imageStoreBorderEdge1			= NULL;		// 개인 상점 바탕 테두리 1 ( 44 X 46 )
	m_imageStoreBorderEdge2			= NULL;		// 개인 상점 바탕 테두리 2 ( 100 X 46 )
	m_imageStoreBorderEdge3			= NULL;		// 개인 상점 바탕 테두리 3 ( 12 X 46 )
	m_imagePVPBorder1				= NULL;		// 비무 홍보 바탕 생성 1 ( 44 X 46 )
	m_imagePVPBorder2				= NULL;		// 비무 홍보 바탕 생성 2 ( 100 X 46 )
	m_imagePVPBorder3				= NULL;		// 비무 홍보 바탕 생성 3 ( 12 X 46 )
	m_imagePVPBorderEdge1			= NULL;		// 비무 홍보 바탕 테두리 1 ( 44 X 46 )
	m_imagePVPBorderEdge2			= NULL;		// 비무 홍보 바탕 테두리 2 ( 100 X 46 )
	m_imagePVPBorderEdge3			= NULL;		// 비무 홍보 바탕 테두리 3 ( 12 X 46 )
	m_imageEventBorder1				= NULL;		// 이벤트 바탕 생성 1 ( 44 X 46 )
	m_imageEventBorder2				= NULL;		// 이벤트 바탕 생성 2 ( 100 X 46 )
	m_imageEventBorder3				= NULL;		// 이벤트 바탕 생성 3 ( 12 X 46 )
	m_imageEventBorderEdge1			= NULL;		// 이벤트 바탕 테두리 1 ( 44 X 46 )
	m_imageEventBorderEdge2			= NULL;		// 이벤트 바탕 테두리 2 ( 100 X 46 )
	m_imageEventBorderEdge3			= NULL;		// 이벤트 바탕 테두리 3 ( 12 X 46 )
	m_imagePVPMark					= NULL;		// 비무 마크
	m_imagePersonalStoreIcon		= NULL;		// 개인 상점
	m_imageEventIcon				= NULL;		// 이벤트창 마크 
		
	m_btnLifeSkillClassTab			= NULL;		// 생활 기술이 확장 되면 삭제해야 함. 삭제 예정
	
	for( int i = 0; i < _XLIFETAB_TOTALCOUNT; i++ ) 
	{			
		m_btnLifeSkillTab[i]		= NULL;		// 수집 자원 수배 행동 기술
	}	
	
	for( i = 0; i < _XCOLLECTIONTYPE_TOTALCOUNT; i++ ) 
	{			
		m_btnType[i]				= NULL;		// 수집 대분류 버튼 : 하품,중품,상품,명품
	}	
	
	// 수집 관련 탭 
	m_btnCollectionNeedItemInput	= NULL;		// 명품집에 넣기 위한 버튼

	// 자원 관련 탭 
	
	// 행동 관련 탭
	m_btnLifeSkillUse				= NULL;		// 개인상점버튼
	m_btnPVPPR						= NULL;		// 비무홍보버튼
	m_btnEvent						= NULL;		// 이벤트버튼

	
	m_listboxCollectionItem			= NULL;		// 수집을 표시하는 리스트 박스
	m_listboxResourceItem			= NULL;		// 자원을 표시하는 리스트 박스
	m_listboxResourceNeedItem		= NULL;
	
	m_pSelectedCollectionItem		= NULL;		// 선택된 수집 아이템

	memset(&m_IconDragStartPos, 0, sizeof(POINT));	
		
	m_indexSelectedResourceItem		= -1;
	m_indexCompleteNeedItem			= -1;
	m_MouseSlotLifeSkillID			= 0;

	m_bHaveMouseSlot				= false;	
	m_bDragState					= false;
	
	m_SelectedTab					= _XLIFETAB_LIFECLASS1;
	m_SelectedTypeClass				= _XCOLLECTION_TYPE1;

	m_lstboxCollectionExplain		= NULL;
	
	m_nResourceTabTotalCount		= 0;
}

_XWindow_LifeSkill::~_XWindow_LifeSkill()
{
	DestroyWindow();
}


void _XWindow_LifeSkill::DestroyWindow(void)
{
	SAFE_DELETE( m_imageItemBorder );
	SAFE_DELETE( m_imageCompleteItemBorder );
	SAFE_DELETE( m_imageSelectedButton );
	SAFE_DELETE( m_imageTypeButtonBorder );
	SAFE_DELETE( m_imagePVPMark);
	SAFE_DELETE( m_imagePersonalStoreIcon);
	SAFE_DELETE( m_imageEventIcon );
	_XWindow::DestroyWindow();
}

BOOL _XWindow_LifeSkill::Initialize(void)
{
	int skillinterface				= g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	int questresourceindex2			= g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
	int optionmenuinterface			= g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );	
	int npcresourceindex1			= g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );
	int chattingbuttonindex			= g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	int mainbackinterface			= g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	int characterwindowimageindex	= g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );	
	int npcresourceindex3			= g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_03.tga" );
	int lifeskillindex				= g_MainInterfaceTextureArchive.FindResource( "MI_LifeSkill.tga" );	

	int	pvpindex2					= -1;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "vn_MI_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "us_MI_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "tw_MI_pvp_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "rs_MI_pvp_02.tga");
	}
	else
	{
		pvpindex2					= g_MainInterfaceTextureArchive.FindResource( "MI_pvp_02.tga");
	}
	
	int szButtonstring[5];	
	szButtonstring[0] = ID_STRING_MAINGAMEMESSAGEHANDLER_1877;
	szButtonstring[1] = ID_STRING_MAINGAMEMESSAGEHANDLER_1878;
	szButtonstring[2] = ID_STRING_LIFESKILL_2160;
	szButtonstring[3] = ID_STRING_LIFESKILL_2161;
	szButtonstring[4] = ID_STRING_LIFESKILL_2162;
			
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		/*
		// 생활기술 클래스 버튼
		_XCHECKBTN_STRUCTURE classtabbtnstruct = 
		{   
			TRUE, { 350, 11 }, { 165, 22 }, 
				_XDEF_LIFESKILL_CLASSBUTTON1, npcresourceindex3, npcresourceindex3, 
				&g_MainInterfaceTextureArchive 
		};	
		for( int i = 0; i < _XLIFETAB_TOTALCOUNT; i++ ) 
		{			
			classtabbtnstruct.position.y = 4+(i*22);
			m_btnLifeSkillTab[i] = new _XCheckButton;
			m_btnLifeSkillTab[i]->Create( classtabbtnstruct );		
			m_btnLifeSkillTab[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, szButtonstring[i] );
			m_btnLifeSkillTab[i]->SetGrowTextMode( TRUE, 0xFF000000 );
			m_btnLifeSkillTab[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
			
			m_btnLifeSkillTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 91, 208, 256, 230 );
			m_btnLifeSkillTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 91, 233, 256, 255 );
			m_btnLifeSkillTab[i]->SetCheck( FALSE );
			m_btnLifeSkillTab[i]->EnableWindow(TRUE);
			InsertChildObject( m_btnLifeSkillTab[i] );
			
			classtabbtnstruct.commandid++;
		}	
		m_btnLifeSkillTab[0]->SetCheck( TRUE );	
		*/

		int skillresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

		// 생활기술 클래스 버튼
		_XCHECKBTN_STRUCTURE classtabbtnstruct = 
		{   
			TRUE, { 4, 2 }, { 108, 24 }, 
				_XDEF_LIFESKILL_CLASSBUTTON1, questresourceindex2,	questresourceindex2, 
				&g_MainInterfaceTextureArchive 
		};	
		
		for( int i = 0; i < _XLIFETAB_TOTALCOUNT; i++ ) 
		{			
			if( i < 2 )
			{
				classtabbtnstruct.position.x = 6+(i*171);
				classtabbtnstruct.position.y = 0;
				classtabbtnstruct.windowsize.cx = 165;
				classtabbtnstruct.imageindex_checked = npcresourceindex3; 
				classtabbtnstruct.imageindex_normal = npcresourceindex3;
				m_btnLifeSkillTab[i] = new _XCheckButton;
				m_btnLifeSkillTab[i]->Create( classtabbtnstruct );
				m_btnLifeSkillTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 91, 208, 256, 230 );
				m_btnLifeSkillTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 91, 233, 256, 255 );
			}
			else // next line
			{
				classtabbtnstruct.position.x = 5+((i-2)*115);
				classtabbtnstruct.position.y = 20;
				classtabbtnstruct.windowsize.cx = 108;
				classtabbtnstruct.imageindex_checked = skillresourceindex; 
				classtabbtnstruct.imageindex_normal = skillresourceindex;
				m_btnLifeSkillTab[i] = new _XCheckButton;
				m_btnLifeSkillTab[i]->Create( classtabbtnstruct );
				m_btnLifeSkillTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 2, 176, 110, 200 );
				m_btnLifeSkillTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 2, 200, 110, 224 );
			}
			
			m_btnLifeSkillTab[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, szButtonstring[i] );
			m_btnLifeSkillTab[i]->SetGrowTextMode( TRUE, 0xFF000000 );
			m_btnLifeSkillTab[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
			m_btnLifeSkillTab[i]->SetCheck( FALSE );
			m_btnLifeSkillTab[i]->EnableWindow(TRUE);
			InsertChildObject( m_btnLifeSkillTab[i] );
			
			classtabbtnstruct.commandid++;
		}	
		m_btnLifeSkillTab[0]->SetCheck( TRUE );	
	}
	else
	{
		// 생활기술 클래스 버튼
		_XCHECKBTN_STRUCTURE classtabbtnstruct = 
		{   
			TRUE, { 4, 11 }, { 66, 26 }, 
				_XDEF_LIFESKILL_CLASSBUTTON1, questresourceindex2,	questresourceindex2, 
				&g_MainInterfaceTextureArchive 
		};	

		for( int i = 0; i < _XLIFETAB_TOTALCOUNT; i++ ) 
		{			
			classtabbtnstruct.position.x = 4+(i*69);
			m_btnLifeSkillTab[i] = new _XCheckButton;
			m_btnLifeSkillTab[i]->Create( classtabbtnstruct );		
			m_btnLifeSkillTab[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, szButtonstring[i] );
			m_btnLifeSkillTab[i]->SetGrowTextMode( TRUE, 0xFF000000 );
			m_btnLifeSkillTab[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
			
			m_btnLifeSkillTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 1, 229, 67, 255 );
			m_btnLifeSkillTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK, 68, 229, 134, 255 );
			m_btnLifeSkillTab[i]->SetCheck( FALSE );
			m_btnLifeSkillTab[i]->EnableWindow(TRUE);
			InsertChildObject( m_btnLifeSkillTab[i] );
			
			classtabbtnstruct.commandid++;
		}	
		m_btnLifeSkillTab[0]->SetCheck( TRUE );	
	}
	
		
	// 정사각형 버튼이 선택되었을 때 테두리 이미지 ( 36 X 36 )
	m_imageSelectedButton = new _XImageStatic;
	m_imageSelectedButton->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, skillinterface);
	m_imageSelectedButton->SetClipRect(170, 0, 206, 36);
	
	// 정사각형 버튼이 바탕 이미지 ( 36 X 36 )	
	m_imageTypeButtonBorder = new _XImageStatic;
	m_imageTypeButtonBorder->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, skillinterface);
	m_imageTypeButtonBorder->SetClipRect(207, 0, 243, 36);
	
	// 아이템 테두리 ( 32 X 32 )
	m_imageItemBorder = new _XImageStatic;
	m_imageItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, mainbackinterface);
	m_imageItemBorder->SetClipRect(214, 31, 246, 63);
	
	// 전부 모은 필요아이템 바탕 이미지 ( 32 X 32 )
	m_imageCompleteItemBorder = new _XImageStatic;
	m_imageCompleteItemBorder->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex);
	m_imageCompleteItemBorder->SetClipRect(191, 33, 223, 65);
	
	///////////////////////////////////////////////////////////////////////////////////////////////
	//								수집 탭에 관련된 이미지들									 //
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	// 수집 타입 바탕 생성 1 
	m_imageCollectionTypeBorder1 = new _XImageStatic;
	m_imageCollectionTypeBorder1->Create(4, 43, 169, 91, &g_MainInterfaceTextureArchive, skillinterface);
	m_imageCollectionTypeBorder1->SetClipRect(0, 0, 169, 48);
	InsertChildObject(m_imageCollectionTypeBorder1);
	
	// 수집 타입 바탕 생성 2
	m_imageCollectionTypeBorder2 = new _XImageStatic;
	m_imageCollectionTypeBorder2->Create(342, 43, 515, 91, &g_MainInterfaceTextureArchive, skillinterface);
	m_imageCollectionTypeBorder2->SetClipRect(0, 0, 169, 48);
	m_imageCollectionTypeBorder2->SetScale(-1, 1);
	InsertChildObject(m_imageCollectionTypeBorder2);

	_XBTN_STRUCTURE skillbtnstruct = 
	{
		TRUE, {13, 53}, {30, 30}, _XDEF_LIFESKILL_TYPEBUTTON1,
			skillinterface,
			skillinterface,
			skillinterface,
			&g_MainInterfaceTextureArchive
	};
	
	m_btnType[0] = new _XButton;
	m_btnType[0]->Create(skillbtnstruct);
	m_btnType[0]->ShowWindow(FALSE);
	InsertChildObject(m_btnType[0]);
	for(int i = 1 ; i < _XCOLLECTIONTYPE_TOTALCOUNT ; i++)
	{
		skillbtnstruct.position.x = 13+ 36*i;
		if(i == _XCOLLECTION_TYPE7)
		{
			skillbtnstruct.commandid = _XDEF_LIFESKILL_TYPEBUTTON7;
		}
		else
		{ 
			skillbtnstruct.commandid++;
		}
		m_btnType[i] = new _XButton;
		m_btnType[i]->Create(skillbtnstruct);
		m_btnType[i]->ShowWindow(FALSE);
		InsertChildObject(m_btnType[i]);
	}
	
	// 수집을 표시하는 리스트 박스
	_XLISTBOX_STRUCTURE collectionstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 5, 96 },		// 윈도우 좌표
		{ 335, 105 },	// 윈도우 사이즈
		_XDEF_LIFESKILL_COLLECTIONLISTBOX,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		17,												// 리스트 아이템 라인 피치
		6,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 105 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		255,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	
	m_listboxCollectionItem = new _XListBox;
	m_listboxCollectionItem->Create(collectionstructure);

	_XVScrollBar* pScrollBar = m_listboxCollectionItem->GetScrollBarObject();
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
	m_listboxCollectionItem->SetAlwaysShowScrollBarMode(TRUE);
	
	POINT columnoffset1 = { 5, 3 };
	POINT columnoffset2 = { 25, 6 };
	m_listboxCollectionItem->SetColumnOffset(0, columnoffset1);				// column offset
	m_listboxCollectionItem->SetColumnOffset(1, columnoffset2);
	
	m_listboxCollectionItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxCollectionItem->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	
	m_listboxCollectionItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxCollectionItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxCollectionItem->SetColumnWidth(0, 335 - 25 - 13);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxCollectionItem->SetColumnWidth(1, 335 - 25 - 13);
	
	m_listboxCollectionItem->SetSelectedImageArchive(npcresourceindex1, &g_MainInterfaceTextureArchive);	
	_XImageStatic* selectbar = m_listboxCollectionItem->GetSelectBarObject();
	selectbar->SetClipRect( 196, 490, 507, 506 );
	m_listboxCollectionItem->SetSelectedImagePosition(5, 3);
	m_listboxCollectionItem->SetSelectedImageHeight(17);
	m_listboxCollectionItem->SetTrackWidth(322);
	
	m_listboxCollectionItem->ShowWindow(FALSE);
	InsertChildObject(m_listboxCollectionItem);
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		// 명품집에 넣기 위한 버튼 (165 X 22)
		_XBTN_STRUCTURE inputstruct = { TRUE, { 167, 280 }, { 168, 22 }, _XDEF_LIFESKILL_INPUTCOLLECTIONBUTTON,
			npcresourceindex3,npcresourceindex3,npcresourceindex3, 
			&g_MainInterfaceTextureArchive };
		m_btnCollectionNeedItemInput = new _XButton;
		m_btnCollectionNeedItemInput->Create( inputstruct );		
		m_btnCollectionNeedItemInput->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 91, 208, 256, 230 );
		m_btnCollectionNeedItemInput->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  91, 183, 256, 205 );
		m_btnCollectionNeedItemInput->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  91, 233, 256, 255 );
		m_btnCollectionNeedItemInput->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LIFESKILL_2163 );
		m_btnCollectionNeedItemInput->SetButtonTextColor( D3DCOLOR_ARGB(255, 52, 221, 34) );
		m_btnCollectionNeedItemInput->SetGrowTextMode( TRUE );
		m_btnCollectionNeedItemInput->ShowWindow(FALSE);
		InsertChildObject( m_btnCollectionNeedItemInput );	
	}
	else
	{
		// 명품집에 넣기 위한 버튼 (111 X 26)
		_XBTN_STRUCTURE inputstruct = { TRUE, { 215, 280 }, { 111, 26 }, _XDEF_LIFESKILL_INPUTCOLLECTIONBUTTON,
			characterwindowimageindex,characterwindowimageindex,characterwindowimageindex, 
			&g_MainInterfaceTextureArchive };
		m_btnCollectionNeedItemInput = new _XButton;
		m_btnCollectionNeedItemInput->Create( inputstruct );
		m_btnCollectionNeedItemInput->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 105, 112, 131 );
		m_btnCollectionNeedItemInput->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  1, 105, 112, 131 );
		m_btnCollectionNeedItemInput->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 131, 112, 157 );
		m_btnCollectionNeedItemInput->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LIFESKILL_2163 );
		m_btnCollectionNeedItemInput->SetButtonTextColor( D3DCOLOR_ARGB(255, 52, 221, 34) );
		m_btnCollectionNeedItemInput->SetGrowTextMode( TRUE );
		m_btnCollectionNeedItemInput->ShowWindow(FALSE);
		InsertChildObject( m_btnCollectionNeedItemInput );	
	}
			
	///////////////////////////////////////////////////////////////////////////////////////////////
	//								자원 탭에 관련된 이미지들									 //
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	// 자원 수집에 필요한 수집 아이템 리스트 박스
	_XLISTBOX_STRUCTURE resourceneeditemstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 158, 96+53 },									// 윈도우 좌표
		{ 183, 238 },									// 윈도우 사이즈
		_XDEF_LIFESKILL_RESOURCENEEDITEMLISTBOX,		// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		34,												// 리스트 아이템 라인 피치
		7,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 238 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		150,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	
	m_listboxResourceNeedItem = new _XListBox;
	m_listboxResourceNeedItem->Create(resourceneeditemstructure);
	
	pScrollBar = m_listboxResourceNeedItem->GetScrollBarObject();
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
	m_listboxResourceNeedItem->SetAlwaysShowScrollBarMode(TRUE);
	m_listboxResourceNeedItem->SetSelectItemMouseHoverCallBack( _ResourceNeedItemListBoxItemMouseHoverCallback );
	
	POINT columnoffset21 = { 2, 2 };
	POINT columnoffset22 = { 1, 1 };
	POINT columnoffset23 = { 34, 4 };
	POINT columnoffset24 = { 34, 18 };
	
	m_listboxResourceNeedItem->SetColumnOffset(0, columnoffset21);				// column offset
	m_listboxResourceNeedItem->SetColumnOffset(1, columnoffset22);
	m_listboxResourceNeedItem->SetColumnOffset(2, columnoffset23);
	m_listboxResourceNeedItem->SetColumnOffset(3, columnoffset24);
	m_listboxResourceNeedItem->SetColumnOffset(4, columnoffset21);
	
	m_listboxResourceNeedItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxResourceNeedItem->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);
	m_listboxResourceNeedItem->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	m_listboxResourceNeedItem->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	m_listboxResourceNeedItem->SetColumnStyle(4, _XLCOLUMNSTYLE_ICONRECT);
	
	m_listboxResourceNeedItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxResourceNeedItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxResourceNeedItem->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	m_listboxResourceNeedItem->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	m_listboxResourceNeedItem->SetAlignType(4, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxResourceNeedItem->SetColumnWidth(0, 170);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxResourceNeedItem->SetColumnWidth(1, 170);
	m_listboxResourceNeedItem->SetColumnWidth(2, 136);
	m_listboxResourceNeedItem->SetColumnWidth(3, 170);
	m_listboxResourceNeedItem->SetColumnWidth(4, 170);
	
	m_listboxResourceNeedItem->SetTrackWidth(170);	
	m_listboxResourceNeedItem->ShowWindow(FALSE);
	InsertChildObject(m_listboxResourceNeedItem);
	
	// 자원리스트를 가지고 있는 리스트 박스
	_XLISTBOX_STRUCTURE resourcestructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 4, 96 },										// 윈도우 좌표
		{ 146, 289 },									// 윈도우 사이즈
		_XDEF_LIFESKILL_RESOURCELISTBOX,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		17,												// 리스트 아이템 라인 피치
		17,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{ 12, 292 },										// 스크롤바 사이즈
		12,													// 스크롤바 상하 버튼 사이즈
		22,													// 스크롤바 트랙바 사이즈
		150,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
	};
	
	m_listboxResourceItem = new _XListBox;
	m_listboxResourceItem->Create(resourcestructure);

	pScrollBar = m_listboxResourceItem->GetScrollBarObject();
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
	m_listboxResourceItem->SetAlwaysShowScrollBarMode(TRUE);
	m_listboxResourceItem->SetSelectItemMouseHoverCallBack( _ResourceItemListBoxItemMouseHoverCallback );

	POINT columnoffset11 = { 5, 5 };
	POINT columnoffset12 = { 20, 6 };
	POINT columnoffset13 = { 22, 3 };
	POINT columnoffset14 = { 40, 6 };
	
	m_listboxResourceItem->SetColumnOffset(0, columnoffset11);				// column offset
	m_listboxResourceItem->SetColumnOffset(1, columnoffset12);
	m_listboxResourceItem->SetColumnOffset(2, columnoffset13);
	m_listboxResourceItem->SetColumnOffset(3, columnoffset14);
	
	m_listboxResourceItem->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);		// column style (아이콘)
	m_listboxResourceItem->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_listboxResourceItem->SetColumnStyle(2, _XLCOLUMNSTYLE_ICONRECT);
	m_listboxResourceItem->SetColumnStyle(3, _XLCOLUMNSTYLE_STRING);
	
	m_listboxResourceItem->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxResourceItem->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_listboxResourceItem->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	m_listboxResourceItem->SetAlignType(3, _XFONT_ALIGNTYPE_LEFT);
	
	m_listboxResourceItem->SetColumnWidth(0, 146);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	m_listboxResourceItem->SetColumnWidth(1, 114);
	m_listboxResourceItem->SetColumnWidth(2, 146);
	m_listboxResourceItem->SetColumnWidth(3, 94 );
	
	m_listboxResourceItem->SetSelectedImageArchive(npcresourceindex1, &g_MainInterfaceTextureArchive);	
	selectbar = m_listboxResourceItem->GetSelectBarObject();
	selectbar->SetClipRect( 196, 490, 325, 506 );
	m_listboxResourceItem->SetSelectedImagePosition(5, 3);
	m_listboxResourceItem->SetSelectedImageHeight(17);
	m_listboxResourceItem->SetTrackWidth(146);
	
	m_listboxResourceItem->ShowWindow(FALSE);
	InsertChildObject(m_listboxResourceItem);
	
		
	// 슬롯 영역 셋팅
	int column = 0;
	int row	= 0;

	POINT mfposition;
	if(g_pMainFrame_Window)
		mfposition = g_pMainFrame_Window->GetWindowPos();
	else
	{
		mfposition.x = 0;
		mfposition.y = 0;
	}


	for( i = 0; i < 16; i++ )
	{
		column = i%8;
		row = i/8;

		m_CollectionNeedItemSlotPosition[i].x = mfposition.x+m_WindowPosition.x+35+(37*column);
		m_CollectionNeedItemSlotPosition[i].y = mfposition.y+m_WindowPosition.y+310+(37*row);
	}
	
	for( i = 0; i < 8; i++ )
	{
		m_ResourcNeedItemSlotPosition[i].x = mfposition.x+m_WindowPosition.x+157;
		m_ResourcNeedItemSlotPosition[i].y = mfposition.y+m_WindowPosition.y+96+17+(34*i);
	}
		
	///////////////////////////////////////////////////////////////////////////////////////////////
	//								행동 탭에 관련된 이미지들									 //
	///////////////////////////////////////////////////////////////////////////////////////////////
			
	// 개인 상점 바탕 생성 ( 311 X 46 )
	// 개인 상점 바탕 생성 1 ( 44 X 46 )
	m_imageStoreBorder1 = new _XImageStatic;
	m_imageStoreBorder1->Create( 10, 54, 54, 100, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageStoreBorder1->SetClipRect( 4, 52 , 48, 98 );
	InsertChildObject( m_imageStoreBorder1 );

	// 개인 상점 바탕 생성 2 ( 100 X 46 )
	m_imageStoreBorder2 = new _XImageStatic;
	m_imageStoreBorder2->Create( 54, 54, 309, 100, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageStoreBorder2->SetClipRect( 48, 52, 148, 98 );
	m_imageStoreBorder2->SetScale( 2.55f, 1.0f);
	InsertChildObject( m_imageStoreBorder2);
	
	// 개인 상점 바탕 생성 3 ( 12 X 46 )
	m_imageStoreBorder3= new _XImageStatic;
	m_imageStoreBorder3->Create( 309, 54, 321, 100, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageStoreBorder3->SetClipRect( 148, 52, 160, 98 );
	InsertChildObject( m_imageStoreBorder3);
	
	// 개인 상점 바탕 테두리  생성 ( 311 X 46 )
	// 개인 상점 바탕 테두리 1 ( 44 X 46 )
	m_imageStoreBorderEdge1 = new _XImageStatic;
	m_imageStoreBorderEdge1->Create( 10, 54, 54, 100, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageStoreBorderEdge1->SetClipRect( 4, 103 , 48, 149 );
	InsertChildObject( m_imageStoreBorderEdge1);
	
	// 개인 상점 바탕 테두리 2 ( 100 X 46 )
	m_imageStoreBorderEdge2 = new _XImageStatic;
	m_imageStoreBorderEdge2->Create( 54, 54, 309, 100, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageStoreBorderEdge2->SetClipRect( 48, 103, 148, 149 );
	m_imageStoreBorderEdge2->SetScale( 2.55f, 1.0f);
	InsertChildObject( m_imageStoreBorderEdge2 );
	
	// 개인 상점 바탕 테두리 3 ( 12 X 46 )
	m_imageStoreBorderEdge3 = new _XImageStatic;
	m_imageStoreBorderEdge3->Create( 309, 54, 321, 100, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageStoreBorderEdge3->SetClipRect( 148, 103, 160, 149 );
	InsertChildObject( m_imageStoreBorderEdge3 );
	
	//임시 상점 아이콘 ( 31 X 31)
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("item_mon.tga");
	m_imagePersonalStoreIcon = new _XImageStatic;
	m_imagePersonalStoreIcon->Create( 20, 58, 51, 89, &g_MainInterfaceTextureArchive, resourceindex );	
	m_imagePersonalStoreIcon->SetClipRect( 0, 0, 32, 32 );

	// 비무 홍보 바탕 생성 ( 311 X 46 )
	// 비무 홍보 바탕 생성 1 ( 44 X 46 )
	m_imagePVPBorder1 = new _XImageStatic;
	m_imagePVPBorder1->Create( 10, 105, 54, 151, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imagePVPBorder1->SetClipRect( 4, 52 , 48, 98 );
	InsertChildObject( m_imagePVPBorder1);
	
	// 비무 홍보 바탕 생성 2 ( 100 X 46 )
	m_imagePVPBorder2 = new _XImageStatic;
	m_imagePVPBorder2->Create( 54, 105, 309, 151, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imagePVPBorder2->SetClipRect( 48, 52, 148, 98 );
	m_imagePVPBorder2->SetScale( 2.55f, 1.0f);
	InsertChildObject( m_imagePVPBorder2 );
	
	// 비무 홍보 바탕 생성 3 ( 12 X 46 )
	m_imagePVPBorder3 = new _XImageStatic;
	m_imagePVPBorder3->Create( 309, 105, 321, 151, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imagePVPBorder3->SetClipRect( 148, 52, 160, 98 );
	InsertChildObject( m_imagePVPBorder3);
	
	// 비무 홍보 바탕 테두리  생성 ( 311 X 46 )
	// 비무 홍보 바탕 테두리 1 ( 44 X 46 )
	m_imagePVPBorderEdge1 = new _XImageStatic;
	m_imagePVPBorderEdge1->Create( 10, 105, 54, 151, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imagePVPBorderEdge1->SetClipRect( 4, 103 , 48, 149 );
	InsertChildObject( m_imagePVPBorderEdge1);
	
	// 비무 홍보 바탕 테두리 2 ( 100 X 46 )
	m_imagePVPBorderEdge2 = new _XImageStatic;
	m_imagePVPBorderEdge2->Create( 54, 105, 309, 151, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imagePVPBorderEdge2->SetClipRect( 48, 103, 148, 149 );
	m_imagePVPBorderEdge2->SetScale( 2.55f, 1.0f);
	InsertChildObject( m_imagePVPBorderEdge2 );
	
	// 비무 홍보 바탕 테두리 3 ( 12 X 46 )
	m_imagePVPBorderEdge3 = new _XImageStatic;
	m_imagePVPBorderEdge3->Create( 309, 105, 321, 151, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imagePVPBorderEdge3->SetClipRect( 148, 103, 160, 149 );
	InsertChildObject( m_imagePVPBorderEdge3 );
		
	// 비무 마크 ( 30 X 30 )
	m_imagePVPMark = new _XImageStatic;
	m_imagePVPMark->Create( 18, 113, 49, 143, &g_MainInterfaceTextureArchive, pvpindex2 );	
	m_imagePVPMark->SetClipRect( 225, 33, 255, 63 );
	//InsertChildObject( m_imagePVPMark );
#ifdef _XDEF_EVENTWINDOW
	// 이벤트 바탕 생성 ( 311 X 46 )
	// 이벤트 바탕 생성 1 ( 44 X 46 )
	m_imageEventBorder1 = new _XImageStatic;
	m_imageEventBorder1->Create( 10, 105+51, 54, 151+51, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageEventBorder1->SetClipRect( 4, 52 , 48, 98 );
	InsertChildObject( m_imageEventBorder1);
	
	// 이벤트 바탕 생성 2 ( 100 X 46 )
	m_imageEventBorder2 = new _XImageStatic;
	m_imageEventBorder2->Create( 54, 105+51, 309, 151+51, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageEventBorder2->SetClipRect( 48, 52, 148, 98 );
	m_imageEventBorder2->SetScale( 2.55f, 1.0f);
	InsertChildObject( m_imageEventBorder2 );
	
	// 이벤트 바탕 생성 3 ( 12 X 46 )
	m_imageEventBorder3 = new _XImageStatic;
	m_imageEventBorder3->Create( 309, 105+51, 321, 151+51, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageEventBorder3->SetClipRect( 148, 52, 160, 98 );
	InsertChildObject( m_imageEventBorder3);
	
	// 이벤트 바탕 테두리  생성 ( 311 X 46 )
	// 이벤트 바탕 테두리 1 ( 44 X 46 )
	m_imageEventBorderEdge1 = new _XImageStatic;
	m_imageEventBorderEdge1->Create( 10, 105+51, 54, 151+51, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageEventBorderEdge1->SetClipRect( 4, 103 , 48, 149 );
	InsertChildObject( m_imageEventBorderEdge1);
	
	// 이벤트 바탕 테두리 2 ( 100 X 46 )
	m_imageEventBorderEdge2 = new _XImageStatic;
	m_imageEventBorderEdge2->Create( 54, 105+51, 309, 151+51, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageEventBorderEdge2->SetClipRect( 48, 103, 148, 149 );
	m_imageEventBorderEdge2->SetScale( 2.55f, 1.0f);
	InsertChildObject( m_imageEventBorderEdge2 );
	
	// 이벤트 바탕 테두리 3 ( 12 X 46 )
	m_imageEventBorderEdge3 = new _XImageStatic;
	m_imageEventBorderEdge3->Create( 309, 105+51, 321, 151+51, &g_MainInterfaceTextureArchive, skillinterface );	
	m_imageEventBorderEdge3->SetClipRect( 148, 103, 160, 149 );
	InsertChildObject( m_imageEventBorderEdge3 );
	
	// 이벤트 마크 ( 30 X 30 )
	m_imageEventIcon = new _XImageStatic;
	m_imageEventIcon->Create( 18, 113+51, 49, 143+51, &g_MainInterfaceTextureArchive, lifeskillindex );	
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
	m_imageEventIcon->SetClipRect( 223, 225, 253, 255 );
#else
	m_imageEventIcon->SetClipRect( 129, 225, 159, 255 );
#endif
#endif

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int skillresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
		// 개인 상점 버튼
		_XBTN_STRUCTURE usebtnstruct = { TRUE, { 212, 55 }, { 108, 24 }, _XDEF_LIFESKILL_USEBUTTON,
			skillresourceindex,skillresourceindex,skillresourceindex, 
			&g_MainInterfaceTextureArchive };
		m_btnLifeSkillUse = new _XButton;
		m_btnLifeSkillUse->Create( usebtnstruct );
		m_btnLifeSkillUse->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 2, 152, 110, 176);
		m_btnLifeSkillUse->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
		m_btnLifeSkillUse->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
		m_btnLifeSkillUse->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LIFESKILL_OPENSELFSTORE ); // 노점
		InsertChildObject( m_btnLifeSkillUse );	
		
		// 비무 홍보 버튼
		_XBTN_STRUCTURE usebtnstruct1 = { TRUE, { 212, 106 }, { 108, 24 }, _XDEF_LIFESKILL_PVPPRBUTTON,
			skillresourceindex,skillresourceindex,skillresourceindex, 
			&g_MainInterfaceTextureArchive };
		m_btnPVPPR = new _XButton;
		m_btnPVPPR->Create( usebtnstruct1 );
		m_btnPVPPR->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 2, 152, 110, 176);
		m_btnPVPPR->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
		m_btnPVPPR->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
		m_btnPVPPR->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2189 ); 
		InsertChildObject( m_btnPVPPR );
		
#ifdef _XDEF_EVENTWINDOW
		// 이벤트 버튼
		_XBTN_STRUCTURE usebtnstruct2 = { TRUE, { 212, 106+51 }, { 108, 24 }, _XDEF_LIFESKILL_EVENTBUTTON,
			skillresourceindex,skillresourceindex,skillresourceindex, 
			&g_MainInterfaceTextureArchive };
		m_btnEvent = new _XButton;
		m_btnEvent->Create( usebtnstruct2 );
		m_btnEvent->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 2, 152, 110, 176);
		m_btnEvent->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200);
		m_btnEvent->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
		m_btnEvent->SetButtonText(_T("Event"));
//		m_btnEvent->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2189 ); 
		InsertChildObject( m_btnEvent );
		
#endif
	}
	else
	{
		// 개인 상점 버튼
		_XBTN_STRUCTURE usebtnstruct = { TRUE, { 257, 55 }, { 62, 26 }, _XDEF_LIFESKILL_USEBUTTON,
			optionmenuinterface,optionmenuinterface,optionmenuinterface, 
			&g_MainInterfaceTextureArchive };
		m_btnLifeSkillUse = new _XButton;
		m_btnLifeSkillUse->Create( usebtnstruct );
		m_btnLifeSkillUse->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 186, 126, 248, 152 );
		m_btnLifeSkillUse->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  186, 151, 248, 177 );
		m_btnLifeSkillUse->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  186, 176, 248, 202 );
		m_btnLifeSkillUse->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LIFESKILL_OPENSELFSTORE ); // 노점
		InsertChildObject( m_btnLifeSkillUse );	

		// 비무 홍보 버튼
		_XBTN_STRUCTURE usebtnstruct1 = { TRUE, { 257, 106 }, { 62, 26 }, _XDEF_LIFESKILL_PVPPRBUTTON,
			optionmenuinterface,optionmenuinterface,optionmenuinterface, 
			&g_MainInterfaceTextureArchive };
		m_btnPVPPR = new _XButton;
		m_btnPVPPR->Create( usebtnstruct1 );
		m_btnPVPPR->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 186, 126, 248, 152 );
		m_btnPVPPR->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  186, 151, 248, 177 );
		m_btnPVPPR->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  186, 176, 248, 202 );
		m_btnPVPPR->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2189 ); 
		InsertChildObject( m_btnPVPPR );		
#ifdef _XDEF_EVENTWINDOW
		// 이벤트 버튼
		_XBTN_STRUCTURE usebtnstruct2 = { TRUE, { 257, 106+51 }, { 62, 26 }, _XDEF_LIFESKILL_EVENTBUTTON,
			optionmenuinterface,optionmenuinterface,optionmenuinterface, 
			&g_MainInterfaceTextureArchive };
		m_btnEvent = new _XButton;
		m_btnEvent->Create( usebtnstruct2 );
		m_btnEvent->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 186, 126, 248, 152 );
		m_btnEvent->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  186, 151, 248, 177 );
		m_btnEvent->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  186, 176, 248, 202 );
		m_btnEvent->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3046 ); 
		InsertChildObject( m_btnEvent );		
#endif

	}

#ifdef _XTS_NEWLIFEWINDOW
	_XLISTBOX_STRUCTURE collectionexplain = 
	{
		TRUE,											// 윈도우 활성화
		{26, 228},										// 윈도우 좌표
		{315, 49},										// 윈도우 사이즈
		_XDEF_LIFESKILL_COLLECTIONEXPLAIN,				// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		17,												// 리스트 아이템 라인 피치
		3,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,						// 스크롤바의 위치
		
		// scroll bar property
		{12, 45},										// 스크롤바 사이즈
		12,												// 스크롤바 상하 버튼 사이즈
		10,												// 스크롤바 트랙바 사이즈
		50,												// 전체 리스트 갯수
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex,
		chattingbuttonindex
	};

	m_lstboxCollectionExplain = new _XListBox;
	m_lstboxCollectionExplain->Create(collectionexplain);
	

	pScrollBar = m_lstboxCollectionExplain->GetScrollBarObject();
	if(pScrollBar)
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 167}
		};
		pScrollBar->SetClipRect(cliprectstructure);
	}

	POINT columnoffset = {0, 0};
	
	m_lstboxCollectionExplain->SetColumnOffset(0, columnoffset);
	m_lstboxCollectionExplain->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lstboxCollectionExplain->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxCollectionExplain->SetColumnWidth(0, 300);
	m_lstboxCollectionExplain->EnableSelectItem(FALSE);
	m_lstboxCollectionExplain->ShowWindow(TRUE);
	InsertChildObject(m_lstboxCollectionExplain);
#endif

	return TRUE;
}
void _XWindow_LifeSkill::ShowWindow(BOOL show)
{
	if(show)
	{
		ChangeTab(m_SelectedTab);

		if(m_SelectedTab == _XLIFETAB_LIFECLASS1)
		{
			SetListCollectionItem();
			SetListItemCollectionExplain();
		}
		else if( m_SelectedTab== _XLIFETAB_LIFECLASS2 )
		{
			SetListResourceItem(); //Author :양희왕 //breif: 변경 .. 선택도 안됬는데,, 필요아이템을 보여줘선 안됨
			//SetListResourceNeedItem();
		}
	}
	else
	{
		m_pSelectedCollectionItem = NULL;
	}
	
	_XWindow::ShowWindow(show);
}
void _XWindow_LifeSkill::Draw(_XGUIObject*& pfocusobject)
{	
	if(this->m_ShowWindow)
	{
		switch(m_SelectedTab) {
		case _XLIFETAB_LIFECLASS1: //수집
			{
				// 수집 분류 바탕
				_XDrawSolidBar( m_WindowPosition.x+5,  m_WindowPosition.y+44, m_WindowPosition.x+340, m_WindowPosition.y+90 , D3DCOLOR_ARGB(255,0,0,0) );
				
				// 수집 리스트 바탕 ( 340 X 85)
				_XDrawSolidBar( m_WindowPosition.x+5,  m_WindowPosition.y+96, m_WindowPosition.x+340, m_WindowPosition.y+201 , D3DCOLOR_ARGB(127,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+4,  m_WindowPosition.y+95, m_WindowPosition.x+340, m_WindowPosition.y+201 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
				
				// 수집 설명 바탕 ( 340 X 200)
				_XDrawSolidBar( m_WindowPosition.x+5,  m_WindowPosition.y+208, m_WindowPosition.x+340, m_WindowPosition.y+387 , D3DCOLOR_ARGB(127,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+4,  m_WindowPosition.y+207, m_WindowPosition.x+340, m_WindowPosition.y+387 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
				
			}
			break;
		case _XLIFETAB_LIFECLASS2: //재료
			{
				// 재료 위치 분류 바탕
				_XDrawSolidBar( m_WindowPosition.x+5,  m_WindowPosition.y+44, m_WindowPosition.x+340, m_WindowPosition.y+90 , D3DCOLOR_ARGB(255,0,0,0) );
				
				// 재료 리스트 바탕 ( 150 X 340)
				_XDrawSolidBar( m_WindowPosition.x+5,  m_WindowPosition.y+96, m_WindowPosition.x+150, m_WindowPosition.y+387 , D3DCOLOR_ARGB(127,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+4,  m_WindowPosition.y+95, m_WindowPosition.x+150, m_WindowPosition.y+387 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
				
				// 재료 리스트 바탕 ( 150 X 340)
				_XDrawSolidBar( m_WindowPosition.x+158,  m_WindowPosition.y+96, m_WindowPosition.x+340, m_WindowPosition.y+387 , D3DCOLOR_ARGB(127,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+157,  m_WindowPosition.y+95, m_WindowPosition.x+340, m_WindowPosition.y+387 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

				// 그리는 순서 변경 - 툴팁이 제일 위에 오도록.
				if( m_indexSelectedResourceItem != -1 && m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
				{
					g_XBaseFont->SetClippingArea( m_WindowPosition.x + 160, m_WindowPosition.y + 99, m_WindowPosition.x + 160 + 178, m_WindowPosition.y + 99 + 48 );

					g_XBaseFont->SetColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
					TCHAR messagestring[256];
					if( g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_ResourceName[0] != 0 )
					{
						g_XBaseFont->SetBoldMode( TRUE );
						g_XBaseFont->Puts( m_WindowPosition.x+160,  m_WindowPosition.y+99, g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_ResourceName );
						g_XBaseFont->Flush();
						g_XBaseFont->SetBoldMode( FALSE );
											
						g_XBaseFont->Print( m_WindowPosition.x + 300, m_WindowPosition.y + 99, 1.0f, "(%d/%d)", 
							g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_currentcountNeeditem,
							g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_totalcountNeeditem );
						g_XBaseFont->Flush();
						
					}
					memset( messagestring, 0, sizeof(TCHAR)*256 );
					sprintf(messagestring, "[ %s ] ", 
						_XGameItem::GetItemName(g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_RewardItem.Get_m_cType(),g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_RewardItem.Get_m_sID())	);
					
					g_XBaseFont->Puts_Separate( m_WindowPosition.x+195,  m_WindowPosition.y+117, messagestring, 143, 1.0f, 2 );
					
					m_imageCompleteItemBorder->Draw(m_WindowPosition.x+159, m_WindowPosition.y+114);
					g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+160, m_WindowPosition.y+115, &g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_RewardItem);
					
					g_XBaseFont->Flush();

					g_XBaseFont->DisableClippingArea();
				}
			}
			break;
		case _XLIFETAB_LIFECLASS3: //수배
			{
			}
			break;
		case _XLIFETAB_LIFECLASS4: //행동
			{
				// 바탕 ( 322 X 340)
				_XDrawSolidBar( m_WindowPosition.x+5,  m_WindowPosition.y+47, m_WindowPosition.x+327, m_WindowPosition.y+387 , D3DCOLOR_ARGB(127,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+4,  m_WindowPosition.y+46, m_WindowPosition.x+327, m_WindowPosition.y+387 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5,  m_WindowPosition.y+47, m_WindowPosition.x+326, m_WindowPosition.y+386 , 0.0f, D3DCOLOR_ARGB(191,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+6,  m_WindowPosition.y+48, m_WindowPosition.x+325, m_WindowPosition.y+385 , 0.0f, D3DCOLOR_ARGB(63,0,0,0) );
			}
			break;
		case _XLIFETAB_LIFECLASS5: //기술
			{
			}
			break;
		}	
	}
	
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;
	
	switch(m_SelectedTab) {
	case _XLIFETAB_LIFECLASS1:
		{
			for(int i = 0 ; i < _XCOLLECTIONTYPE_TOTALCOUNT-1 ; i++)
			{
				m_imageTypeButtonBorder->Draw(m_WindowPosition.x+10+(i*36), m_WindowPosition.y+50);
			}
			
			m_imageSelectedButton->Draw(m_WindowPosition.x+10+(m_SelectedTypeClass*36), m_WindowPosition.y+50);
			
			if( m_pSelectedCollectionItem )
			{
				g_XBaseFont->SetClippingArea( m_WindowPosition.x+5,  m_WindowPosition.y+208, m_WindowPosition.x+340, m_WindowPosition.y+387 );

				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 245, 116, 34)); 
				if( m_pSelectedCollectionItem->m_CollectionName[0] != 0 )
				{
					g_XBaseFont->Puts( m_WindowPosition.x+15,  m_WindowPosition.y+213, m_pSelectedCollectionItem->m_CollectionName );
					g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+213, m_pSelectedCollectionItem->m_CollectionName );
				}
				
#ifdef _XTS_NEWLIFEWINDOW
#else
				g_XBaseFont->SetColor( _XSC_DEFAULT );
				if( m_pSelectedCollectionItem->m_CollectionExplain[0][0] != 0 )
				{
					g_XBaseFont->Puts( m_WindowPosition.x+25,  m_WindowPosition.y+233, m_pSelectedCollectionItem->m_CollectionExplain[0] );
					if( m_pSelectedCollectionItem->m_CollectionExplain[1][0] != 0 )
					{
						g_XBaseFont->Puts( m_WindowPosition.x+25,  m_WindowPosition.y+250, m_pSelectedCollectionItem->m_CollectionExplain[1] );
						if( m_pSelectedCollectionItem->m_CollectionExplain[2][0] != 0 )
						{
							g_XBaseFont->Puts( m_WindowPosition.x+25,  m_WindowPosition.y+267, m_pSelectedCollectionItem->m_CollectionExplain[2] );
						}
					}
				}
#endif
				
				if( !m_pSelectedCollectionItem->m_bComplete && !m_pSelectedCollectionItem->m_bEnableExchage )
				{
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(TCHAR)*256 );
					
					int param1 = m_pSelectedCollectionItem->m_CompleteItemCurrentCount;
					int param2 = m_pSelectedCollectionItem->m_NeedItemTotalCount;
					strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_LIFESKILL_2164, &param1, &param2 ) );
					
					g_XBaseFont->Puts( m_WindowPosition.x+15,  m_WindowPosition.y+290, messagestring );
					g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+290, messagestring );
					
				}
				else
				{
					g_XBaseFont->Puts( m_WindowPosition.x+15,  m_WindowPosition.y+290, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2165) );
					g_XBaseFont->Puts( m_WindowPosition.x+16,  m_WindowPosition.y+290, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2165) );
				}

				g_XBaseFont->Flush();
				g_XBaseFont->DisableClippingArea();
		

				int count	= 0;
				int column	= 0;
				if( !m_pSelectedCollectionItem->m_listNeedItem.empty() )
				{
					list <_XCollectionResourceItem*>::iterator iter_needitem;
					_XCollectionResourceItem* currentneeditem = NULL;
					for(iter_needitem = m_pSelectedCollectionItem->m_listNeedItem.begin() ; iter_needitem != m_pSelectedCollectionItem->m_listNeedItem.end() ; iter_needitem++)
					{
						currentneeditem = *iter_needitem;
						if(currentneeditem)
						{	
							if( count < 8 )
							{
								column = count;
								g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+35+(column*37), m_WindowPosition.y+310, currentneeditem);
								if( currentneeditem->m_bComplete )
								{
									m_imageCompleteItemBorder->Draw(m_WindowPosition.x+34+(column*37), m_WindowPosition.y+309);
								}
								else
								{
									m_imageItemBorder->Draw(m_WindowPosition.x+34+(column*37), m_WindowPosition.y+309);
									_XDrawSolidBar( m_WindowPosition.x+35+(column*37), m_WindowPosition.y+310, 
										m_WindowPosition.x+65+(column*37), m_WindowPosition.y+340 , D3DCOLOR_ARGB(127,0,0,0) );
								}
							}
							else
							{
								column = count - 8;
								g_pInventory_Window->DrawIcon_LifeSkill(TRUE, m_WindowPosition.x+35+(column*37), m_WindowPosition.y+347, currentneeditem);
								if( currentneeditem->m_bComplete )
								{
									m_imageCompleteItemBorder->Draw(m_WindowPosition.x+34+(column*37), m_WindowPosition.y+346);
								}
								else
								{
									m_imageItemBorder->Draw(m_WindowPosition.x+34+(column*37), m_WindowPosition.y+346);
									_XDrawSolidBar( m_WindowPosition.x+35+(column*37), m_WindowPosition.y+347, 
										m_WindowPosition.x+65+(column*37), m_WindowPosition.y+377 , D3DCOLOR_ARGB(127,0,0,0) );
								}
							}
							count++;
						}
					}
				}
				

				g_XBaseFont->Flush();

			}
		}
		break;
	case _XLIFETAB_LIFECLASS2:
		{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
			for(int i = 0 ; i < m_nResourceTabTotalCount ; i++)
#else
			for(int i = 0 ; i < 4 ; i++)
#endif
			{
				m_imageTypeButtonBorder->Draw(m_WindowPosition.x+10+(i*36), m_WindowPosition.y+50);
			}
			
			m_imageSelectedButton->Draw(m_WindowPosition.x+10+(m_SelectedTypeClass*36), m_WindowPosition.y+50);

			// 그리는 순서 변경. 툴팁이 제일 위에 오도록 하기 위해서 위치 변경했음.
			/*
			if( m_indexSelectedResourceItem != -1 && m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
			{
				g_XBaseFont->SetClippingArea( m_WindowPosition.x + 160, m_WindowPosition.y + 99, m_WindowPosition.x + 160 + 178, m_WindowPosition.y + 99 + 48 );

				g_XBaseFont->SetColor( D3DCOLOR_ARGB(255, 255, 255, 255) );
				TCHAR messagestring[64];
				if( g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_ResourceName[0] != 0 )
				{
					g_XBaseFont->SetBoldMode( TRUE );
					g_XBaseFont->Puts( m_WindowPosition.x+160,  m_WindowPosition.y+99, g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_ResourceName );
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode( FALSE );
										
					g_XBaseFont->Print( m_WindowPosition.x + 300, m_WindowPosition.y + 99, 1.0f, "(%d/%d)", 
						g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_currentcountNeeditem,
						g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_totalcountNeeditem );
					g_XBaseFont->Flush();
					
				}
				memset( messagestring, 0, sizeof(TCHAR)*64 );
				sprintf(messagestring, "[ %s ] ", 
					_XGameItem::GetItemName(g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_RewardItem.m_cType,g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_RewardItem.Get_m_sID())	);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x+195,  m_WindowPosition.y+117, messagestring, 143, 1.0f, 2 );
				
				m_imageCompleteItemBorder->Draw(m_WindowPosition.x+159, m_WindowPosition.y+114);
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+160, m_WindowPosition.y+115, &g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_RewardItem);
				
				g_XBaseFont->Flush();

				g_XBaseFont->DisableClippingArea();
			}*/
			
			// 대분류 버튼 툴팁 처리하기
			for( i = 0; i < m_nResourceTabTotalCount; i++ )
			{
#ifndef _XDEF_RESOURCE_RENEWAL_071018_KUKURI	// define 되지 않았을때
				if(i >= 4)
					break;
#endif

				if( m_btnType[i]->CheckMousePosition() )
				{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
					switch(i)
					{
					case 0 :
						{
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, 
								_XGETINTERFACETEXT(ID_STRING_MAINFRAME_GROUP), _XSC_INFORMATION );//문파
						}
						break;
					case 1 :
						{
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, 
								_XGETINTERFACETEXT(ID_STRING_LIFESKILL_2166), _XSC_INFORMATION );//합비
						}
						break;
					case 2 :
						{
							TCHAR messagestr[256];
							sprintf(messagestr, "%s/%s", _XGETINTERFACETEXT(ID_STRING_NEW_3491), _XGETINTERFACETEXT(ID_STRING_NEW_3492));
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, 
								messagestr, _XSC_INFORMATION );//정주/석가장
						}
						break;
					case 3 :
						{
							TCHAR messagestr[256];
							sprintf(messagestr, "%s/%s", _XGETINTERFACETEXT(ID_STRING_NEW_3494), _XGETINTERFACETEXT(ID_STRING_NEW_3493));
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, 
								messagestr, _XSC_INFORMATION );//대동/제남
						}
						break;
					case 4 :
						{
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, 
								_XGETINTERFACETEXT(ID_STRING_LIFESKILL_2169), _XSC_INFORMATION );//항주/남창
						}
						break;
					case 5 :
						{
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, 
								_XGETINTERFACETEXT(ID_STRING_NEW_3002), _XSC_INFORMATION );//요동
						}
						break;
					case 6 :
						{
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, 
								_XGETINTERFACETEXT(ID_STRING_NEW_3418), _XSC_INFORMATION );//북해빙궁
						}
						break;
					}
#else
					if( i == 0 )
					{
						g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2166), _XSC_INFORMATION );	
					}
					else if( i == 1 )
					{
						g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2167), _XSC_INFORMATION );	
					}
					else if( i == 2 )
					{
						g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2168), _XSC_INFORMATION );	
					}
					else if( i == 3 )
					{
						g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+33+(36*i), m_WindowPosition.y+38, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2169), _XSC_INFORMATION );	
					}	
#endif
				}
			}			
		}
		break;
	case _XLIFETAB_LIFECLASS3:
		{
		}
		break;
	case _XLIFETAB_LIFECLASS4:
		{
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->Puts( m_WindowPosition.x+61,  m_WindowPosition.y+61, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2170) );
			g_XBaseFont->Puts( m_WindowPosition.x+61,  m_WindowPosition.y+112, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2171) );
#ifdef _XDEF_EVENTWINDOW
			g_XBaseFont->Puts( m_WindowPosition.x+61,  m_WindowPosition.y+163, _XGETINTERFACETEXT(ID_STRING_NEW_3046) );
#endif
			g_XBaseFont->Flush();
			
			m_imagePersonalStoreIcon->Draw( m_WindowPosition.x+17,  m_WindowPosition.y+61 );
			m_imagePVPMark->Draw( m_WindowPosition.x+18,  m_WindowPosition.y+112 );			
#ifdef _XDEF_EVENTWINDOW
			m_imageEventIcon->Draw( m_WindowPosition.x+18,  m_WindowPosition.y+163 );
#endif
		}
		break;
	case _XLIFETAB_LIFECLASS5:
		{
		}
		break;
	}	
}
BOOL _XWindow_LifeSkill::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
		
	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )						
			g_pDefaultTooltip_Window->ShowWindow( FALSE );
	}
	
	if( m_SelectedTab ==  _XCOLLECTION_TYPE1 )
	{
		if( m_listboxCollectionItem->CheckMousePosition() )
		{
			m_listboxCollectionItem->ProcessWheelMessage(zDelta);
			return TRUE;
		}
#ifdef _XTS_NEWLIFEWINDOW
		if( m_lstboxCollectionExplain->CheckMousePosition() )
		{
			m_lstboxCollectionExplain->ProcessWheelMessage(zDelta);
			return TRUE;
		}
#endif
		
	}
	else if( m_SelectedTab ==  _XCOLLECTION_TYPE2 )
	{
		if( m_listboxResourceItem->CheckMousePosition() )
		{
			m_listboxResourceItem->ProcessWheelMessage(zDelta);
			return TRUE;
		}
		if( m_listboxResourceNeedItem->CheckMousePosition() )
		{
			m_listboxResourceNeedItem->ProcessWheelMessage(zDelta);
			return TRUE;
		}

	}
	else if( m_SelectedTab ==  _XCOLLECTION_TYPE3 )
	{
	}
	else if( m_SelectedTab ==  _XCOLLECTION_TYPE4 )
	{
	}
	else if( m_SelectedTab ==  _XCOLLECTION_TYPE5 )
	{
	}
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}
BOOL _XWindow_LifeSkill::Process(_XGUIObject*& pfocusobject)
{		
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
	// TODO			
	if( m_SelectedTab ==  _XLIFETAB_LIFECLASS1 )
	{
		BOOL	bInTrade = FALSE;
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		
		if(mousestate)
		{
			if(!pfocusobject)
			{
				if(mousestate->bButton[1])
				{
					if(CheckMousePosition())
						m_WindowMouseState = _XW_STATE_RIGHTCLICK;					
				}

				if(mousestate->bButton[0])
				{
					if(CheckMousePosition())
					{				
						bool binvendrag = false;
						bool bfindneeditem = false;
						if( CheckMousePositionInCollectionItme() )
						{
							if(g_pInventory_Window )	// 인벤토리에서 드래그 했을 경우
							{
								if(g_pInventory_Window->GetShowStatus())
								{
									if(g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
									{
										// 필요한 아이템의 타입과 아이디와 갯수를 검색하고 조건에 맞는 인덱스가 있으면 패킷을 보낸다.
										// 그렇지 않을 경우 에러 메세지를 보낸다.
										ProcessCollectionNeedItem();
										bInTrade = TRUE;
										binvendrag = true;
									}
								}
							}

							// 수집 필요 아이템의 툴팁 처리하기 
							if( !binvendrag )
							{
								int selectitem = CheckSelectedCollectionNeedItem();
								if( selectitem != -1 )
								{						
									if( g_pDefaultTooltip_Window )
									{			
										if( m_pSelectedCollectionItem )
										{
											int type = -1;
											int id = 0;
											list <_XCollectionResourceItem*>::iterator iter_needitem;
											_XCollectionResourceItem* currentneeditem = NULL;
											for(iter_needitem = m_pSelectedCollectionItem->m_listNeedItem.begin() ; iter_needitem != m_pSelectedCollectionItem->m_listNeedItem.end() ; iter_needitem++)
											{
												currentneeditem = *iter_needitem;
												if(currentneeditem)
												{	
													if( currentneeditem->m_Index == selectitem )
													{
														type = currentneeditem->Get_m_cType();
														id = currentneeditem->Get_m_sID();
														bfindneeditem = true;
														break;
													}
												}
											}

											if( bfindneeditem )
											{
												if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_LIFESKILLCOLLECTIONITEM, selectitem, type, id ) )									
												{
													g_pDefaultTooltip_Window->ShowWindow( TRUE );
												}
											}														
										}//if( m_pSelectedCollectionItem )
									}//if( g_pDefaultTooltip_Window )
								}
							}
						}//if( CheckMousePositionInCollectionItme() )

						if( !bfindneeditem )
						{
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
																	
						m_WindowMouseState = _XW_STATE_LEFTCLICK;
					}
					else
					{
						if(m_bHaveMouseSlot)
						{
							if(g_pQuickSlot_Window)
							{
								if(!g_pQuickSlot_Window->CheckMousePosition())
									ResetMouseSlot();
							}
						}

						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}

			if(!mousestate->bButton[1])
			{
				if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
				{
					m_WindowMouseState = _XW_STATE_NONE;
				}
			}
			if(!mousestate->bButton[0])
			{
				if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
				{
					m_WindowMouseState = _XW_STATE_NONE;
				}
			}

		}
	}
	else if( m_SelectedTab ==  _XLIFETAB_LIFECLASS2 )
	{
		BOOL	bInTrade = FALSE;
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

		if(mousestate)
		{
			if(!pfocusobject)
			{
				if(mousestate->bButton[1])
				{
					if(CheckMousePosition())
						m_WindowMouseState = _XW_STATE_RIGHTCLICK;				
				}

				if(mousestate->bButton[0])
				{
					if(CheckMousePosition())
					{				
						int selectitem = CheckSelectedResourceNeedItem();
						bool bfindneeditem = false;
						if( selectitem != -1 )
						{						
							if( g_pDefaultTooltip_Window )
							{			
								if( m_indexSelectedResourceItem != -1 && m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
								{
									int type = -1;
									int id = 0;
									if( selectitem == 0 ) // 보상물 툴팁
									{
										type = g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_RewardItem.Get_m_cType();
										id	= g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_RewardItem.Get_m_sID();
									}
									else
									{
										selectitem = selectitem+m_listboxResourceNeedItem->GetLineScrollPos();

										DWORD cmdid = (DWORD)m_listboxResourceNeedItem->GetItemCommandID( selectitem-1 );
										type = (cmdid & 0xFFFF0000) >> 16;
										id = (cmdid & 0x0000FFFF);
									}

									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_LIFESKILLRESOURCEITEM, selectitem, type, id ) )									
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}			
									bfindneeditem = true;
								}//if( m_indexSelectedResourceItem != -1 && m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem)
							}//if( g_pDefaultTooltip_Window )
						}
						if( !bfindneeditem )
						{
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
					else
					{
						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
										
					m_WindowMouseState = _XW_STATE_LEFTCLICK;				
				}
			}

			if(!mousestate->bButton[1])
			{
				if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
				{
					m_WindowMouseState = _XW_STATE_NONE;
				}
			}
			if(!mousestate->bButton[0])
			{
				if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
				{
					m_WindowMouseState = _XW_STATE_NONE;
				}
			}
		}
	}
	else if( m_SelectedTab ==  _XLIFETAB_LIFECLASS3 ) // 수배
	{
	}
	else if( m_SelectedTab ==  _XLIFETAB_LIFECLASS4 ) // 행동
	{
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		
		if(mousestate)
		{
			if(!pfocusobject)
			{
				if(mousestate->bButton[1])
				{
					if(CheckMousePosition())
						m_WindowMouseState = _XW_STATE_RIGHTCLICK;
					else
					{
						if(m_bHaveMouseSlot)
							ResetMouseSlot();
					}
				}
				
				if(mousestate->bButton[0])
				{
					if(CheckMousePosition())
					{				
						if(!m_bHaveMouseSlot)
						{
							// Icon 안쪽을 클릭 했으면
							int selectedlifeskillindex = CheckSelectedIconItem();
							if( selectedlifeskillindex != -1 )
							{
								m_bDragState = true;
							}
						}
						else
						{
							ResetMouseSlot();
						}
						
						m_WindowMouseState = _XW_STATE_LEFTCLICK;
					}
					else
					{
						if(m_bHaveMouseSlot)
						{
							if(g_pQuickSlot_Window)
							{
								if(!g_pQuickSlot_Window->CheckMousePosition())
									ResetMouseSlot();
							}
						}
						
						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}
			
			if(!mousestate->bButton[1])
			{
				if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
				{
					// QuickSlot에 등록
					if(m_bHaveMouseSlot)
						ResetMouseSlot();
					else
					{
						int selectedlifeskillindex = CheckSelectedIconItem();
						if( selectedlifeskillindex != -1 )
						{
							if(g_pQuickSlot_Window)
							{
								g_pQuickSlot_Window->RegistLifeSkillNextEmptySlot(selectedlifeskillindex);
							}
						}
					}
					
					m_WindowMouseState = _XW_STATE_NONE;
				}
			}
			if(!mousestate->bButton[0])
			{
				if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
				{
					// icon 안쪽인지 체크
					int selectedlifeskillindex = CheckSelectedIconItem();
					if( selectedlifeskillindex != -1 )
					{
						if(m_bDragState)
						{
							// 드래그
							m_MouseSlotLifeSkillID = (short)selectedlifeskillindex;
							m_bHaveMouseSlot = true;
							m_bDragState = false;
						}
					}
					else
					{
						m_bDragState = false;
					}
					m_WindowMouseState = _XW_STATE_NONE;
				}
			}
		}
	}
	else if( m_SelectedTab ==  _XLIFETAB_LIFECLASS5 )	// 기술
	{
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_LifeSkill::MoveWindow( int X, int Y ) //Author : 양희왕 //breif : 툴팁 클릭이 안되던 버그로 인해 수정..
{			
	_XWindow::MoveWindow( X, Y );

	int column = 0;
	int row	= 0;

	POINT mfposition;
	if(g_pMainFrame_Window)
		mfposition = g_pMainFrame_Window->GetWindowPos();
	else
	{
		mfposition.x = 0;
		mfposition.y = 0;
	}


	for(int i = 0; i < 16; i++ )
	{
		column = i%8;
		row = i/8;

		m_CollectionNeedItemSlotPosition[i].x = m_WindowPosition.x+35+(37*column);
		m_CollectionNeedItemSlotPosition[i].y = m_WindowPosition.y+310+(37*row);
	}
	
	for( i = 0; i < 8; i++ )
	{
		m_ResourcNeedItemSlotPosition[i].x = m_WindowPosition.x+157;
		m_ResourcNeedItemSlotPosition[i].y = m_WindowPosition.y+96+17+(34*i);
	}
}

void _XWindow_LifeSkill::ProcessCollectionNeedItem(void)
{
	if(!m_pSelectedCollectionItem)
		return;
	
	// 노점이 열린 상태에서는 아이템의 이동을 막는다. 
	if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{		
		g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_NEW_3137), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
		_XPlayInterfaceSound(ID_SR_INTERFACE_ERROR01_WAV);
		return;
	}
	
	if( m_pSelectedCollectionItem->m_bComplete )
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );		
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2172), TRUE);
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		return;
	}
	else if( m_pSelectedCollectionItem->m_bEnableExchage )
	{
		_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );		
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2173), TRUE);
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		return;
	}



	if( !m_pSelectedCollectionItem->m_listNeedItem.empty() )
	{		
		m_indexCompleteNeedItem = -1;
		bool bfinditem = false;
		list <_XCollectionResourceItem*>::iterator iter_needitem;
		_XCollectionResourceItem* currentneeditem = NULL;
		for(iter_needitem = m_pSelectedCollectionItem->m_listNeedItem.begin() ; iter_needitem != m_pSelectedCollectionItem->m_listNeedItem.end() ; iter_needitem++)
		{
			currentneeditem = *iter_needitem;
			if(currentneeditem)
			{	
				if( currentneeditem->Get_m_cType() == g_pInventory_Window->m_MouseSlotItem.Get_m_cType() &&
					currentneeditem->Get_m_sID() == g_pInventory_Window->m_MouseSlotItem.Get_m_sID() )
				{
					bfinditem = true;
					if( !currentneeditem->m_bComplete )
					{						
						if( currentneeditem->Get_m_ucCount()<= g_pInventory_Window->m_MouseSlotItem.Get_m_ucCount() )
						{					
							m_indexCompleteNeedItem = currentneeditem->m_Index;
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );
							TCHAR		namestring[256];
							memset(namestring, 0, sizeof(TCHAR)*256);
							strcpy(namestring, _XGameItem::GetItemName(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(), g_pInventory_Window->m_MouseSlotItem.Get_m_sID()));

							//Author : 양희왕 //breif : 제련된 무기에 대해 질의
							TCHAR messagestring[256];
							memset(messagestring, 0, sizeof(TCHAR)*256);

							switch( g_pInventory_Window->m_MouseSlotItem.Get_m_cType() )
							{
							case _XGI_FC_WEAPON :
							case _XGI_FC_WEAPON2 :
							case _XGI_FC_WEAPON3 :
								{
									if( g_pInventory_Window->m_MouseSlotItem.Get_m_ucStrength() > 0 ||									
									    g_pInventory_Window->m_MouseSlotItem.m_usCurProtect != 0 ||
									    g_pInventory_Window->m_MouseSlotItem.m_usCurMaxProtect != _XGameItem::GetNature(g_pInventory_Window->m_MouseSlotItem.Get_m_cType(), g_pInventory_Window->m_MouseSlotItem.Get_m_sID()) ||
									    g_pInventory_Window->m_MouseSlotItem.m_usUsedProtect != 0 )
									{
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3756), namestring);	
									}
									else
									{
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2174), namestring);	
									}
								}
								break;
							default:
								sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2174), namestring);	
								break;
							}//date : 07/11/15
							
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_LIFESKILL_INPUTCOLLECTION_OK, _XDEF_LIFESKILL_INPUTCOLLECTION_NO);
						}
						else
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );		
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2175), TRUE);
							if(g_pInventory_Window)
							{
								g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
								g_pInventory_Window->m_bHaveMouseSlot = FALSE;
								
								if( g_pDefaultTooltip_Window )
								{
									if( g_pDefaultTooltip_Window->GetShowStatus() )						
										g_pDefaultTooltip_Window->ShowWindow( FALSE );
								}
							}
						}
					}
					else
					{
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );		
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2176), TRUE);
						if(g_pInventory_Window)
						{
							g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
							g_pInventory_Window->m_bHaveMouseSlot = FALSE;
							
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
					
					break;
				}
			}
		}
		if( !bfinditem )
		{
			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_NPCTRADEMESSAGEWINDOW );		
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, NULL, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2177), TRUE);
			if(g_pInventory_Window)
			{
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				
				if( g_pDefaultTooltip_Window )
				{
					if( g_pDefaultTooltip_Window->GetShowStatus() )						
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
		}
	}		
}

void _XWindow_LifeSkill::ResetMouseSlot(void)
{
	m_MouseSlotLifeSkillID = -1;
	m_bHaveMouseSlot = false;
}

BOOL _XWindow_LifeSkill::CheckMousePositionInCollectionItme()
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x >= m_WindowPosition.x+5 &&	scrnpos->x <= m_WindowPosition.x+340 &&
		scrnpos->z >= m_WindowPosition.y+208 &&	scrnpos->z <= m_WindowPosition.y+387)
	{		
		return TRUE;		
	}
	
	return FALSE;
}
BOOL _XWindow_LifeSkill::CheckSelectedResourceNeedItem()
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	for( int i = 0; i < 8; i++ )
	{
		if(scrnpos->x >= m_ResourcNeedItemSlotPosition[i].x &&	scrnpos->x <= m_ResourcNeedItemSlotPosition[i].x+32 &&
			scrnpos->z >= m_ResourcNeedItemSlotPosition[i].y &&	scrnpos->z <= m_ResourcNeedItemSlotPosition[i].y+32)
		{
			return i;
		}
	}
	return -1;
}


BOOL _XWindow_LifeSkill::CheckSelectedCollectionNeedItem()
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	for( int i = 0; i < 16; i++ )
	{
		if(scrnpos->x >= m_CollectionNeedItemSlotPosition[i].x &&	scrnpos->x <= m_CollectionNeedItemSlotPosition[i].x+32 &&
			scrnpos->z >= m_CollectionNeedItemSlotPosition[i].y &&	scrnpos->z <= m_CollectionNeedItemSlotPosition[i].y+32)
		{
			return i;
		}
	}
	return -1;
}
void _XWindow_LifeSkill::CheckCollectionCompleteNeedItem(void)
{
	if( !m_pSelectedCollectionItem->m_listNeedItem.empty() )
	{
		bool benableexchange = false;
		list <_XCollectionResourceItem*>::iterator iter_needitem;
		_XCollectionResourceItem* currentneeditem = NULL;
		for(iter_needitem = m_pSelectedCollectionItem->m_listNeedItem.begin() ; iter_needitem != m_pSelectedCollectionItem->m_listNeedItem.end() ; iter_needitem++)
		{
			currentneeditem = *iter_needitem;
			if(currentneeditem)
			{	
				if( currentneeditem->m_Index == m_indexCompleteNeedItem )
				{
					m_indexCompleteNeedItem = -1;
					currentneeditem->m_bComplete = true;
					break;
				}
			}
		}

		m_pSelectedCollectionItem->m_CompleteItemCurrentCount = 0;
		for(iter_needitem = m_pSelectedCollectionItem->m_listNeedItem.begin() ; iter_needitem != m_pSelectedCollectionItem->m_listNeedItem.end() ; iter_needitem++)
		{
			currentneeditem = *iter_needitem;
			if(currentneeditem)
			{	
				if( currentneeditem->m_bComplete )
				{
					m_pSelectedCollectionItem->m_CompleteItemCurrentCount++;
				}
			}
		}

		if( m_pSelectedCollectionItem->m_CompleteItemCurrentCount == m_pSelectedCollectionItem->m_NeedItemTotalCount )
		{
			m_btnCollectionNeedItemInput->ShowWindow(FALSE);
			m_pSelectedCollectionItem->m_bEnableExchage = true;
			SetListCollectionItem();
			SetListItemCollectionExplain();
		}
		else
		{
			m_btnCollectionNeedItemInput->ShowWindow(TRUE);
		}
	}		
}

int _XWindow_LifeSkill::CheckSelectedIconItem()
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	int maxcount = 2;
#ifdef _XDEF_EVENTWINDOW
	maxcount = 3;
#endif
	
	for(int i = 0 ; i < maxcount ; i++)
	{
		if(scrnpos->x >= m_WindowPosition.x+17  &&
			scrnpos->x <= m_WindowPosition.x+48 &&
			scrnpos->z >= m_WindowPosition.y+61 + i*51 &&
			scrnpos->z <= m_WindowPosition.y+92 + i*51 )
		{
			return i+1;			
		}
	}
	
	return -1;

//	if(scrnpos->x >= m_WindowPosition.x+17 &&	scrnpos->x <= m_WindowPosition.x+48 &&
//		scrnpos->z >= m_WindowPosition.y+61 &&	scrnpos->z <= m_WindowPosition.y+92)
//	{		
//		return TRUE;		
//	}
//	
//	return FALSE;

}

void _XWindow_LifeSkill::DrawDragIcon(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if( m_MouseSlotLifeSkillID == 1 )
	{
		m_imagePersonalStoreIcon->Draw(scrnpos->x-16, scrnpos->z-16);
	}
	else if( m_MouseSlotLifeSkillID == 2 )
	{
		m_imagePVPMark->Draw(scrnpos->x-16, scrnpos->z-16);
	}
#ifdef _XDEF_EVENTWINDOW
	else if( m_MouseSlotLifeSkillID == 3 )
	{
		m_imageEventIcon->Draw(scrnpos->x-16, scrnpos->z-16);
	}
#endif
}

void _XWindow_LifeSkill::ChangeTab(_XLifeWindowTab selectedtab)
{
	if( selectedtab ==  _XCOLLECTION_TYPE3 || selectedtab ==  _XCOLLECTION_TYPE5 )
	{
		m_btnLifeSkillTab[2]->SetCheck( FALSE );
		m_btnLifeSkillTab[4]->SetCheck( FALSE );
		return;
	}

	if( m_SelectedTab != selectedtab )
		m_SelectedTypeClass = _XCOLLECTION_TYPE1;

	m_SelectedTab = selectedtab;
	
	// 수집 탭 이미지
	m_imageCollectionTypeBorder1->ShowWindow(FALSE);
	m_imageCollectionTypeBorder2->ShowWindow(FALSE);
	m_listboxCollectionItem->ShowWindow(FALSE);
	m_listboxCollectionItem->EnableWindow(FALSE);
	m_btnCollectionNeedItemInput->ShowWindow(FALSE);
	
	// 자원 탭 이미지
	m_listboxResourceItem->ShowWindow(FALSE);
	m_listboxResourceNeedItem->ShowWindow(FALSE);
	
#ifdef _XTS_NEWLIFEWINDOW
	m_lstboxCollectionExplain->ShowWindow(FALSE);
#endif

	// 행동 탭 이미지
	m_imageStoreBorder1->ShowWindow(FALSE);
	m_imageStoreBorder2->ShowWindow(FALSE);
	m_imageStoreBorder3->ShowWindow(FALSE);
	m_imageStoreBorderEdge1->ShowWindow(FALSE);
	m_imageStoreBorderEdge2->ShowWindow(FALSE);
	m_imageStoreBorderEdge3->ShowWindow(FALSE);
	m_imagePVPBorder1->ShowWindow(FALSE);
	m_imagePVPBorder2->ShowWindow(FALSE);
	m_imagePVPBorder3->ShowWindow(FALSE);
	m_imagePVPBorderEdge1->ShowWindow(FALSE);
	m_imagePVPBorderEdge2->ShowWindow(FALSE);
	m_imagePVPBorderEdge3->ShowWindow(FALSE);	
#ifdef _XDEF_EVENTWINDOW
	m_imageEventBorder1->ShowWindow(FALSE);
	m_imageEventBorder2->ShowWindow(FALSE);
	m_imageEventBorder3->ShowWindow(FALSE);
	m_imageEventBorderEdge1->ShowWindow(FALSE);
	m_imageEventBorderEdge2->ShowWindow(FALSE);
	m_imageEventBorderEdge3->ShowWindow(FALSE);
	m_btnEvent->ShowWindow(FALSE);
#endif
	//m_imagePVPMark->ShowWindow(FALSE);
	//m_imagePersonalStoreIcon->ShowWindow(FALSE);
	m_btnLifeSkillUse->ShowWindow(FALSE);
	m_btnPVPPR->ShowWindow(FALSE);

	for( int i = 0; i < _XLIFETAB_TOTALCOUNT; i++ )
	{			
		m_btnLifeSkillTab[i]->SetCheck( FALSE );
		
	}	
	SetTypeButtonTexture();
	
	if( m_SelectedTab == _XLIFETAB_LIFECLASS1)			//수집
	{
		m_btnLifeSkillTab[0]->SetCheck( TRUE );		
		
		// 수집 탭 이미지
		m_imageCollectionTypeBorder1->ShowWindow(TRUE);
		m_imageCollectionTypeBorder2->ShowWindow(TRUE);
		
		m_listboxCollectionItem->ShowWindow(TRUE);
		m_listboxCollectionItem->EnableWindow(TRUE);
		
		if( m_pSelectedCollectionItem )
		{
			if( !m_pSelectedCollectionItem->m_bComplete && !m_pSelectedCollectionItem->m_bEnableExchage )
			{
				m_btnCollectionNeedItemInput->ShowWindow(TRUE);
			}
			else
			{
				m_btnCollectionNeedItemInput->ShowWindow(FALSE);
			}
		}
		else
		{
			m_btnCollectionNeedItemInput->ShowWindow(FALSE);
		}
		ChangeType(m_SelectedTypeClass);
		
#ifdef _XTS_NEWLIFEWINDOW
		m_lstboxCollectionExplain->ShowWindow(TRUE);
#endif
	}
	else if( m_SelectedTab == _XLIFETAB_LIFECLASS2)		//재료
	{
		m_btnLifeSkillTab[1]->SetCheck( TRUE );

		m_imageCollectionTypeBorder1->ShowWindow(TRUE);
		m_imageCollectionTypeBorder2->ShowWindow(TRUE);
		
		m_listboxResourceItem->ShowWindow(TRUE);
		m_listboxResourceNeedItem->ShowWindow(TRUE);
		// 재료 수집 아이템 리스트가 전혀 없을 때 
		if( m_indexSelectedResourceItem == -1 )
		{
			int resourceindex = -1;
			if( !g_ResourceManager.m_listResourceGroupItem.empty() )
			{
				_XResourceGroupItem* currentgroup = NULL;
				_XResourcePackage* currentpackage = NULL;
				list <_XResourceGroupItem*>::iterator iter_group;
				for(iter_group = g_ResourceManager.m_listResourceGroupItem.begin() ; iter_group != g_ResourceManager.m_listResourceGroupItem.end() ; iter_group++)
				{
					currentgroup = *iter_group;
					if(currentgroup)
					{				
						currentgroup->m_bSelectedGroupSubItem = true;
						list <_XResourcePackage*>::iterator iter_package;
						for(iter_package = g_ResourceManager.m_listResourcePackage.begin() ; iter_package != g_ResourceManager.m_listResourcePackage.end() ; iter_package++)
						{
							currentpackage = *iter_package;
							if(currentpackage)
							{
								if( currentpackage->m_ResourcePackageIndex == currentgroup->m_ResourcePackageIndex )
								{
									int resourceindex = 0;
									list <int>::iterator iter_resourceindex;
									for(iter_resourceindex = currentpackage->m_listResourceItemIndex.begin() ; iter_resourceindex != currentpackage->m_listResourceItemIndex.end() ; iter_resourceindex++)
									{
										resourceindex = *iter_resourceindex;
										if( resourceindex != -1 )
										{	
											DWORD commandid = (DWORD)2<<16 | g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceIndex;
											SetSelectedResourceItemOrGrupItem(commandid);				
											break;
										}
									}
									break;
								}
							}
						}
						break;
						
					}
					
				}
			}
			ChangeType(m_SelectedTypeClass);
		}//if( m_indexSelectedResourceItem == -1 )
		else
		{
			ChangeType(m_SelectedTypeClass);
		}
	}
	else if( m_SelectedTab == _XLIFETAB_LIFECLASS3)		//수배
	{
	}
	else if( m_SelectedTab == _XLIFETAB_LIFECLASS4)		//행동
	{
		m_btnLifeSkillTab[3]->SetCheck( TRUE );
		// 행동 탭 이미지
		m_imageStoreBorder1->ShowWindow(TRUE);
		m_imageStoreBorder2->ShowWindow(TRUE);
		m_imageStoreBorder3->ShowWindow(TRUE);
		m_imageStoreBorderEdge1->ShowWindow(TRUE);
		m_imageStoreBorderEdge2->ShowWindow(TRUE);
		m_imageStoreBorderEdge3->ShowWindow(TRUE);
		m_imagePVPBorder1->ShowWindow(TRUE);
		m_imagePVPBorder2->ShowWindow(TRUE);
		m_imagePVPBorder3->ShowWindow(TRUE);
		m_imagePVPBorderEdge1->ShowWindow(TRUE);
		m_imagePVPBorderEdge2->ShowWindow(TRUE);
		m_imagePVPBorderEdge3->ShowWindow(TRUE);
#ifdef _XDEF_EVENTWINDOW
		m_imageEventBorder1->ShowWindow(TRUE);
		m_imageEventBorder2->ShowWindow(TRUE);
		m_imageEventBorder3->ShowWindow(TRUE);
		m_imageEventBorderEdge1->ShowWindow(TRUE);
		m_imageEventBorderEdge2->ShowWindow(TRUE);
		m_imageEventBorderEdge3->ShowWindow(TRUE);
		m_btnEvent->ShowWindow(TRUE);
#endif
		m_imagePVPMark->ShowWindow(TRUE);
		m_imagePersonalStoreIcon->ShowWindow(TRUE);
		m_btnLifeSkillUse->ShowWindow(TRUE);
		m_btnPVPPR->ShowWindow(TRUE);
	}
	else if( m_SelectedTab == _XLIFETAB_LIFECLASS5)		//기술
	{
	}			

	
	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )						
			g_pDefaultTooltip_Window->ShowWindow( FALSE );
	}
}

void _XWindow_LifeSkill::ChangeType(_XCollectionType selectedtype)
{
	m_pSelectedCollectionItem = NULL;
	m_btnCollectionNeedItemInput->ShowWindow(FALSE);

	m_SelectedTypeClass = selectedtype;
	if( m_SelectedTab == _XLIFETAB_LIFECLASS1)	
	{
		SetListCollectionItem();
		SetListItemCollectionExplain();
	}
	else if( m_SelectedTab == _XLIFETAB_LIFECLASS2 )
	{
		SetListResourceItem();
	}	
}

void _XWindow_LifeSkill::SetTypeButtonTexture(void)
{
	
	int resourceindex = g_MainInterfaceTextureArchive.FindResource(g_MI_LifeSkillTextureName);

	m_nResourceTabTotalCount = _XCOLLECTIONTYPE_TOTALCOUNT;

//	if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE) //Author : 양희왕 //brief : 대만 북해빙궁 들어감 Date : 08/03/24
//		m_nResourceTabTotalCount = 6;	// 북해빙궁 삭제
	if(g_LanguageType == _XLANGUAGE_TYPE_JAPANESE)
		m_nResourceTabTotalCount = 5;	// 요동, 북해 삭제
	
	if( m_SelectedTab == _XLIFETAB_LIFECLASS1)			//명품수집
	{
		for(int i = 0 ; i < _XCOLLECTIONTYPE_TOTALCOUNT ; i++)
		{
			m_btnType[i]->SetButtonImage(resourceindex, resourceindex, resourceindex, &g_MainInterfaceTextureArchive);
			m_btnType[i]->ShowWindow(TRUE);
			
			switch(i)
			{
			case _XCOLLECTION_TYPE1 :
				{
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 193, 1, 223, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 193, 1, 223, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 193, 1, 223, 31);
				}
				break;
			case _XCOLLECTION_TYPE2 :
				{
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 33, 1, 63, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 33, 1, 63, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 33, 1, 63, 31);
				}
				break;
			case _XCOLLECTION_TYPE3 :
				{
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 65, 1, 95, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 65, 1, 95, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 65, 1, 95, 31);
				}
				break;
			case _XCOLLECTION_TYPE4 :
				{
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 97, 1, 127, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 97, 1, 127, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 97, 1, 127, 31);
				}
				break;
			case _XCOLLECTION_TYPE5 :
				{
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 129, 1, 159, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 129, 1, 159, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 129, 1, 159, 31);
				}
				break;
			case _XCOLLECTION_TYPE6 :
				{
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 161, 1, 191, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 161, 1, 191, 31);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 161, 1, 191, 31);
				}
				break;
			case _XCOLLECTION_TYPE7 :
				{
					m_btnType[i]->ShowWindow(FALSE);
				}
				break;
			}		
		}
	}
	else if( m_SelectedTab == _XLIFETAB_LIFECLASS2)			//재료수집
	{
		int posx = 0;

		for(int i = 0 ; i < m_nResourceTabTotalCount ; i++)
		{
			m_btnType[i]->SetButtonImage(resourceindex, resourceindex, resourceindex, &g_MainInterfaceTextureArchive);
			m_btnType[i]->ShowWindow(TRUE);
			
			switch(i)
			{
			case _XCOLLECTION_TYPE1 :
				{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
		posx = 0;
#else
		posx = 1;
#endif
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, posx, 225, posx+30, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, posx, 225, posx+30, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, posx, 225, posx+30, 255);
				}
				break;
			case _XCOLLECTION_TYPE2 :
				{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
		posx = 31;
#else
		posx = 33;
#endif
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, posx, 225, posx+30, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, posx , 225, posx+30, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, posx, 225, posx+30, 255);
				}
				break;
			case _XCOLLECTION_TYPE3 :
				{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
		posx = 63;
#else
		posx = 65;
#endif
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, posx, 225, posx+30, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, posx, 225, posx+30, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, posx, 225, posx+30, 255);
				}
				break;
			case _XCOLLECTION_TYPE4 :
				{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
		posx = 95;
#else
		posx = 97;
#endif
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, posx, 225, posx+30, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, posx, 225, posx+30, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, posx, 225, posx+30, 255);
				}
				break;
			case _XCOLLECTION_TYPE5 :
				{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 127, 225, 157, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 127, 225, 157, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 127, 225, 157, 255);
#else
					m_btnType[i]->ShowWindow(FALSE);
#endif
				}
				break;
			case _XCOLLECTION_TYPE6 :
				{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 159, 225, 189, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 159, 225, 189, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 159, 225, 189, 255);
#else
					m_btnType[i]->ShowWindow(FALSE);
#endif
				}
				break;
			case _XCOLLECTION_TYPE7 :
				{
#ifdef _XDEF_RESOURCE_RENEWAL_071018_KUKURI 

					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 191, 225, 221, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 191, 225, 221, 255);
					m_btnType[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 191, 225, 221, 255);
#else
					m_btnType[i]->ShowWindow(FALSE);
#endif
				}
				break;
			}		
		}

		// 안나오는 버튼 안보이게
		for(i = m_nResourceTabTotalCount ; i < _XCOLLECTIONTYPE_TOTALCOUNT ; ++i)
		{
			m_btnType[i]->ShowWindow(FALSE);
		}
	}		
	else
	{
		for(int i = 0 ; i < _XCOLLECTIONTYPE_TOTALCOUNT ; i++)
		{
			m_btnType[i]->ShowWindow(FALSE);
		}
	}

}

void _XWindow_LifeSkill::SetListCollectionItem()
{
	if( !m_listboxCollectionItem ) return;
	m_listboxCollectionItem->DeleteAllItem();		
	int lifeskillindex = g_MainInterfaceTextureArchive.FindResource(g_MI_LifeSkillTextureName);	
	RECT rect = {117, 75, 133, 91}; //하품
	bool playcollection = false;
	TCHAR messagestring[256];
	
	switch(m_SelectedTypeClass) {
	case _XCOLLECTION_TYPE1:
		{				
			if( !g_CollectionManager.m_listCollectionItem.empty() )
			{
				int count = 0 ;	
				_XCollectionItem* currentcollection = NULL;
				list <_XCollectionItem*>::iterator iter_collection;
				for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
				{
					currentcollection = *iter_collection;
					if(currentcollection)
					{	
						playcollection = false;
						// 레벨 체크 
						if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= currentcollection->m_CollectionMinimumLevel)	
						{
							playcollection = true;
						}

						if( playcollection )
						{
							playcollection = false;
							// 선행 수집항목 체크
							int precedecollectiontotalcount = 0;
							int precedecollectioncurrentcount = 0;
							
							for(int i = 0; i < 5; i++)
							{
								if( currentcollection->m_PrecedeCollectionIndex[i] != -1 )
								{
									precedecollectiontotalcount++;
									_XCollectionItem* checkcollection = NULL;
									list <_XCollectionItem*>::iterator iter_collection;
									for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
									{
										checkcollection = *iter_collection;
										if(checkcollection && checkcollection->m_bComplete )
										{
											if( checkcollection->m_CollectionIndex == currentcollection->m_PrecedeCollectionIndex[i] )
											{
												precedecollectioncurrentcount++;
												break;
											}
										}
									}
								}
							}
							if( precedecollectiontotalcount == precedecollectioncurrentcount)
							{
								playcollection = true;
							}
						}

						
						if( playcollection )
						{
							switch(currentcollection->m_CollectionType) {
							case 1:
								{
									rect.top = 75;
									rect.bottom = 91;
								}
								break;
							case 2:
								{
									rect.top = 93;
									rect.bottom = 109;
								}
								break;
							case 3:
								{
									rect.top = 111;
									rect.bottom = 127;
								}
								break;
							case 4:
								{
									rect.top = 129;
									rect.bottom = 145;
								}
								break;
							case 5:
								{
									rect.top = 147;
									rect.bottom = 163;
								}
								break;
							}
							
							
							m_listboxCollectionItem->InsertListItem(_T(""), 0, currentcollection->m_CollectionIndex+1);		
							m_listboxCollectionItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
							m_listboxCollectionItem->SetItemAttrib(count, 0, lifeskillindex , rect);
							
							
							if( currentcollection->m_CollectionName[0] != 0 )
							{
								if( currentcollection->m_bComplete )
								{
									memset( messagestring, 0, sizeof(TCHAR)*256 );
									sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2422), currentcollection->m_CollectionName);
								}
								else if( currentcollection->m_bEnableExchage )
								{
									memset( messagestring, 0, sizeof(TCHAR)*256 );
									sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2423), currentcollection->m_CollectionName);
								}
								else
								{
									memset( messagestring, 0, sizeof(TCHAR)*256 );
									sprintf(messagestring, "%s", currentcollection->m_CollectionName);
								}
								m_listboxCollectionItem->SetItemText(count, 1, messagestring );
							}
							
							m_listboxCollectionItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
							m_listboxCollectionItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
							
							count++;				
						}

					}
				}
				if( !m_pSelectedCollectionItem )
				{
					if( count != 0 )
					{
						// 제대로 동작하지 않으므로 코드 삭제
/*						list <_XCollectionItem*>::iterator iter_collection;
						for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
						{
							currentcollection = *iter_collection;
							if(currentcollection)
							{	
								if( currentcollection->m_CollectionType == 0 )
								{
									m_pSelectedCollectionItem = currentcollection;
									SetListItemCollectionExplain();
									break;
								}
							}
						}*/
					}
					else
					{	
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2178), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				}
			}
		}
		break;
	case _XCOLLECTION_TYPE2:
		{
			if( !g_CollectionManager.m_listCollectionItem.empty() )
			{
				_XCollectionItem* currentcollection = NULL;
				int count = 0 ;	
				list <_XCollectionItem*>::iterator iter_collection;
				for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
				{
					currentcollection = *iter_collection;
					if(currentcollection)
					{				
						if( currentcollection->m_CollectionType == 1 )
						{
							playcollection = false;
							// 레벨 체크 
							if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= currentcollection->m_CollectionMinimumLevel)	
							{
								playcollection = true;
							}

							if( playcollection )
							{
								playcollection = false;
								// 선행 수집항목 체크
								int precedecollectiontotalcount = 0;
								int precedecollectioncurrentcount = 0;
								
								for(int i = 0; i < 5; i++)
								{
									if( currentcollection->m_PrecedeCollectionIndex[i] != -1 )
									{
										precedecollectiontotalcount++;
										_XCollectionItem* checkcollection = NULL;
										list <_XCollectionItem*>::iterator iter_collection;
										for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
										{
											checkcollection = *iter_collection;
											if(checkcollection && checkcollection->m_bComplete )
											{
												if( checkcollection->m_CollectionIndex == currentcollection->m_PrecedeCollectionIndex[i] )
												{
													precedecollectioncurrentcount++;
													break;
												}
											}
										}
									}
								}
								if( precedecollectiontotalcount == precedecollectioncurrentcount)
								{
									playcollection = true;
								}
							}

							
							if( playcollection )
							{
								rect.top = 75;
								rect.bottom = 91;							
								
								m_listboxCollectionItem->InsertListItem(_T(""), 0, currentcollection->m_CollectionIndex+1);			
								m_listboxCollectionItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
								m_listboxCollectionItem->SetItemAttrib(count, 0, lifeskillindex , rect);
								
								if( currentcollection->m_CollectionName[0] != 0 )
								{
									if( currentcollection->m_bComplete )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2422), currentcollection->m_CollectionName);
									}
									else if( currentcollection->m_bEnableExchage )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2423), currentcollection->m_CollectionName);
									}
									else
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, "%s", currentcollection->m_CollectionName);
									}
									m_listboxCollectionItem->SetItemText(count, 1, messagestring );
								}

								m_listboxCollectionItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								m_listboxCollectionItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								
								count++;		
							}		
						}
					}
				}
				if( !m_pSelectedCollectionItem )
				{
					if( count != 0 )
					{
						// 제대로 동작하지 않으므로 코드 삭제
						/*list <_XCollectionItem*>::iterator iter_collection;
						for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
						{
							currentcollection = *iter_collection;
							if(currentcollection)
							{				
								if( currentcollection->m_CollectionType == 1 )
								{
									m_pSelectedCollectionItem = currentcollection;
									SetListItemCollectionExplain();
									break;
								}
							}
						}*/
					}
					else
					{	
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2178), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				}
			}
		}
		break;
	case _XCOLLECTION_TYPE3:
		{
			if( !g_CollectionManager.m_listCollectionItem.empty() )
			{
				_XCollectionItem* currentcollection = NULL;
				int count = 0 ;	
				list <_XCollectionItem*>::iterator iter_collection;
				for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
				{
					currentcollection = *iter_collection;
					if(currentcollection)
					{				
						if( currentcollection->m_CollectionType == 2 )
						{
							playcollection = false;
							// 레벨 체크 
							if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= currentcollection->m_CollectionMinimumLevel)	
							{
								playcollection = true;
							}

							if( playcollection )
							{
								playcollection = false;
								// 선행 수집항목 체크
								int precedecollectiontotalcount = 0;
								int precedecollectioncurrentcount = 0;
								
								for(int i = 0; i < 5; i++)
								{
									if( currentcollection->m_PrecedeCollectionIndex[i] != -1 )
									{
										precedecollectiontotalcount++;
										_XCollectionItem* checkcollection = NULL;
										list <_XCollectionItem*>::iterator iter_collection;
										for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
										{
											checkcollection = *iter_collection;
											if(checkcollection && checkcollection->m_bComplete )
											{
												if( checkcollection->m_CollectionIndex == currentcollection->m_PrecedeCollectionIndex[i] )
												{
													precedecollectioncurrentcount++;
													break;
												}
											}
										}
									}
								}
								if( precedecollectiontotalcount == precedecollectioncurrentcount)
								{
									playcollection = true;
								}
							}

							
							if( playcollection )
							{
								rect.top = 93;
								rect.bottom = 109;							
								
								m_listboxCollectionItem->InsertListItem(_T(""), 0, currentcollection->m_CollectionIndex+1);			
								m_listboxCollectionItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
								m_listboxCollectionItem->SetItemAttrib(count, 0, lifeskillindex , rect);
								
								if( currentcollection->m_CollectionName[0] != 0 )
								{
									if( currentcollection->m_bComplete )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2422), currentcollection->m_CollectionName);
									}
									else if( currentcollection->m_bEnableExchage )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2423), currentcollection->m_CollectionName);
									}
									else
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, "%s", currentcollection->m_CollectionName);
									}
									m_listboxCollectionItem->SetItemText(count, 1, messagestring );
								}
								m_listboxCollectionItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								m_listboxCollectionItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								
								count++;				
							}
						}
					}
				}
				if( !m_pSelectedCollectionItem )
				{
					if( count != 0 )
					{
						// 제대로 동작하지 않으므로 코드 삭제
						/*list <_XCollectionItem*>::iterator iter_collection;
						for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
						{
							currentcollection = *iter_collection;
							if(currentcollection)
							{				
								if( currentcollection->m_CollectionType == 2 )
								{
									m_pSelectedCollectionItem = currentcollection;
									SetListItemCollectionExplain();
									break;
								}
							}
						}*/
					}
					else
					{									
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2178), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				}
			}
		}
		break;
	case _XCOLLECTION_TYPE4:
		{
			if( !g_CollectionManager.m_listCollectionItem.empty() )
			{
				_XCollectionItem* currentcollection = NULL;
				int count = 0 ;	
				list <_XCollectionItem*>::iterator iter_collection;
				for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
				{
					currentcollection = *iter_collection;
					if(currentcollection)
					{				
						if( currentcollection->m_CollectionType == 3 )
						{
							playcollection = false;
							// 레벨 체크 
							if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= currentcollection->m_CollectionMinimumLevel)	
							{
								playcollection = true;
							}

							if( playcollection )
							{
								playcollection = false;
								// 선행 수집항목 체크
								int precedecollectiontotalcount = 0;
								int precedecollectioncurrentcount = 0;
								
								for(int i = 0; i < 5; i++)
								{
									if( currentcollection->m_PrecedeCollectionIndex[i] != -1 )
									{
										precedecollectiontotalcount++;
										_XCollectionItem* checkcollection = NULL;
										list <_XCollectionItem*>::iterator iter_collection;
										for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
										{
											checkcollection = *iter_collection;
											if(checkcollection && checkcollection->m_bComplete )
											{
												if( checkcollection->m_CollectionIndex == currentcollection->m_PrecedeCollectionIndex[i] )
												{
													precedecollectioncurrentcount++;
													break;
												}
											}
										}
									}
								}
								if( precedecollectiontotalcount == precedecollectioncurrentcount)
								{
									playcollection = true;
								}
							}

							
							if( playcollection )
							{
								rect.top = 111;
								rect.bottom = 127;							
								
								m_listboxCollectionItem->InsertListItem(_T(""), 0, currentcollection->m_CollectionIndex+1);			
								m_listboxCollectionItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
								m_listboxCollectionItem->SetItemAttrib(count, 0, lifeskillindex , rect);
								
								if( currentcollection->m_CollectionName[0] != 0 )
								{
									if( currentcollection->m_bComplete )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2422), currentcollection->m_CollectionName);
									}
									else if( currentcollection->m_bEnableExchage )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2423), currentcollection->m_CollectionName);
									}
									else
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, "%s", currentcollection->m_CollectionName);
									}
									m_listboxCollectionItem->SetItemText(count, 1, messagestring );
								}
								
								m_listboxCollectionItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								m_listboxCollectionItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								
								count++;			
							}	
						}
					}
				}
				if( !m_pSelectedCollectionItem )
				{
					if( count != 0 )
					{
						// 제대로 동작하지 않으므로 코드 삭제
						/*list <_XCollectionItem*>::iterator iter_collection;
						for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
						{
							currentcollection = *iter_collection;
							if(currentcollection)
							{				
								if( currentcollection->m_CollectionType == 3 )
								{
									m_pSelectedCollectionItem = currentcollection;
									SetListItemCollectionExplain();
									break;
								}
							}
						}*/
					}
					else
					{									
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2178), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				}
			}
		}
		break;
	case _XCOLLECTION_TYPE5:
		{
			if( !g_CollectionManager.m_listCollectionItem.empty() )
			{
				_XCollectionItem* currentcollection = NULL;
				int count = 0 ;	
				list <_XCollectionItem*>::iterator iter_collection;
				for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
				{
					currentcollection = *iter_collection;
					if(currentcollection)
					{				
						if( currentcollection->m_CollectionType == 4 )
						{
							playcollection = false;
							// 레벨 체크 
							if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= currentcollection->m_CollectionMinimumLevel)	
							{
								playcollection = true;
							}

							if( playcollection )
							{
								playcollection = false;
								// 선행 수집항목 체크
								int precedecollectiontotalcount = 0;
								int precedecollectioncurrentcount = 0;
								
								for(int i = 0; i < 5; i++)
								{
									if( currentcollection->m_PrecedeCollectionIndex[i] != -1 )
									{
										precedecollectiontotalcount++;
										_XCollectionItem* checkcollection = NULL;
										list <_XCollectionItem*>::iterator iter_collection;
										for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
										{
											checkcollection = *iter_collection;
											if(checkcollection && checkcollection->m_bComplete )
											{
												if( checkcollection->m_CollectionIndex == currentcollection->m_PrecedeCollectionIndex[i] )
												{
													precedecollectioncurrentcount++;
													break;
												}
											}
										}
									}
								}
								if( precedecollectiontotalcount == precedecollectioncurrentcount)
								{
									playcollection = true;
								}
							}

							
							if( playcollection )
							{
								rect.top = 129;
								rect.bottom = 145;							
								
								m_listboxCollectionItem->InsertListItem(_T(""), 0, currentcollection->m_CollectionIndex+1);			
								m_listboxCollectionItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
								m_listboxCollectionItem->SetItemAttrib(count, 0, lifeskillindex , rect);
								
								if( currentcollection->m_CollectionName[0] != 0 )
								{
									if( currentcollection->m_bComplete )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2422), currentcollection->m_CollectionName);
									}
									else if( currentcollection->m_bEnableExchage )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2423), currentcollection->m_CollectionName);
									}
									else
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, "%s", currentcollection->m_CollectionName);
									}
									m_listboxCollectionItem->SetItemText(count, 1, messagestring );
								}
								m_listboxCollectionItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								m_listboxCollectionItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								
								count++;			
							}	
						}
					}
				}
				if( !m_pSelectedCollectionItem )
				{
					if( count != 0 )
					{
						// 제대로 동작하지 않으므로 코드 삭제
						/*list <_XCollectionItem*>::iterator iter_collection;
						for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
						{
							currentcollection = *iter_collection;
							if(currentcollection)
							{				
								if( currentcollection->m_CollectionType == 4 )
								{
									m_pSelectedCollectionItem = currentcollection;
									SetListItemCollectionExplain();
									break;
								}
							}
						}*/
					}
					else
					{									
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2178), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				}
			}
		}
		break;
	case _XCOLLECTION_TYPE6:
		{
			if( !g_CollectionManager.m_listCollectionItem.empty() )
			{
				_XCollectionItem* currentcollection = NULL;
				int count = 0 ;	
				list <_XCollectionItem*>::iterator iter_collection;
				for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
				{
					currentcollection = *iter_collection;
					if(currentcollection)
					{				
						if( currentcollection->m_CollectionType == 5 )
						{
							playcollection = false;
							// 레벨 체크 
							if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= currentcollection->m_CollectionMinimumLevel)	
							{
								playcollection = true;
							}

							if( playcollection )
							{
								playcollection = false;
								// 선행 수집항목 체크
								int precedecollectiontotalcount = 0;
								int precedecollectioncurrentcount = 0;
								
								for(int i = 0; i < 5; i++)
								{
									if( currentcollection->m_PrecedeCollectionIndex[i] != -1 )
									{
										precedecollectiontotalcount++;
										_XCollectionItem* checkcollection = NULL;
										list <_XCollectionItem*>::iterator iter_collection;
										for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
										{
											checkcollection = *iter_collection;
											if(checkcollection && checkcollection->m_bComplete )
											{
												if( checkcollection->m_CollectionIndex == currentcollection->m_PrecedeCollectionIndex[i] )
												{
													precedecollectioncurrentcount++;
													break;
												}
											}
										}
									}
								}
								if( precedecollectiontotalcount == precedecollectioncurrentcount)
								{
									playcollection = true;
								}
							}

							
							if( playcollection )
							{
								rect.top = 147;
								rect.bottom = 163;							
								
								m_listboxCollectionItem->InsertListItem(_T(""), 0, currentcollection->m_CollectionIndex+1);			
								m_listboxCollectionItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
								m_listboxCollectionItem->SetItemAttrib(count, 0, lifeskillindex , rect);
								
								if( currentcollection->m_CollectionName[0] != 0 )
								{
									if( currentcollection->m_bComplete )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2422), currentcollection->m_CollectionName);
									}
									else if( currentcollection->m_bEnableExchage )
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2423), currentcollection->m_CollectionName);
									}
									else
									{
										memset( messagestring, 0, sizeof(TCHAR)*256 );
										sprintf(messagestring, "%s", currentcollection->m_CollectionName);
									}
									m_listboxCollectionItem->SetItemText(count, 1, messagestring );
								}
								m_listboxCollectionItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								m_listboxCollectionItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
								
								count++;				
							}
						}
					}
				}
				if( !m_pSelectedCollectionItem )
				{
					if( count != 0 )
					{
						// 제대로 동작하지 않으므로 코드 삭제
						/*list <_XCollectionItem*>::iterator iter_collection;
						for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
						{
							currentcollection = *iter_collection;
							if(currentcollection)
							{				
								if( currentcollection->m_CollectionType == 5 )
								{
									m_pSelectedCollectionItem = currentcollection;
									SetListItemCollectionExplain();
									break;
								}
							}
						}*/
					}
					else
					{									
						g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_LIFESKILL_2178), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				}
			}
		}
		break;

	}
}

void _XWindow_LifeSkill::SetListResourceItem()
{
	if( !m_listboxResourceItem ) return;
	if( !m_listboxResourceNeedItem ) return;

	m_listboxResourceNeedItem->DeleteAllItem();	//선택 된게 없으면 지워야지..
	m_indexSelectedResourceItem	= -1;

	m_listboxResourceItem->DeleteAllItem();		
	RECT rect = {117, 165, 133, 181}; //자원
	int lifeskillindex = g_MainInterfaceTextureArchive.FindResource(g_MI_LifeSkillTextureName);
	if( !g_ResourceManager.m_listResourceGroupItem.empty() )
	{
		int count = 0 ;
		int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
		RECT rect = {12, 12, 24, 24};
		_XResourceGroupItem* currentgroup = NULL;
		_XResourcePackage* currentpackage = NULL;
		list <_XResourceGroupItem*>::iterator iter_group;
		for(iter_group = g_ResourceManager.m_listResourceGroupItem.begin() ; iter_group != g_ResourceManager.m_listResourceGroupItem.end() ; iter_group++)
		{
			currentgroup = *iter_group;
			if(currentgroup)
			{				
				if( currentgroup->m_ResourceGroupItemIndex == m_SelectedTypeClass )
				{
					if( currentgroup->m_bSelectedGroupSubItem )	
					{
						rect.top =0;
						rect.left = 12;
						rect.bottom = 12;
						rect.right = 24;
						
						DWORD commandid = (DWORD)1<<16 | currentgroup->m_ResourceGroupSubItemIndex;								
						
						m_listboxResourceItem->InsertListItem(_T(""), 0, commandid );		
						m_listboxResourceItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxResourceItem->SetItemAttrib(count, 0, chattingbuttonindex , rect);
						m_listboxResourceItem->SetItemAttrib(count, 2, -1, rect);
						
						if( currentgroup->m_ResourceGroupSubItemName[0] != 0 )
							m_listboxResourceItem->SetItemText(count, 1, currentgroup->m_ResourceGroupSubItemName );
						
						m_listboxResourceItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 235, 217, 49));
						m_listboxResourceItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 235, 217, 49));						
						count++;			
				
						list <_XResourcePackage*>::iterator iter_package;
						for(iter_package = g_ResourceManager.m_listResourcePackage.begin() ; iter_package != g_ResourceManager.m_listResourcePackage.end() ; iter_package++)
						{
							currentpackage = *iter_package;
							if(currentpackage)
							{
								if( currentpackage->m_ResourcePackageIndex == currentgroup->m_ResourcePackageIndex )
								{
									int resourceindex = 0;
									list <int>::iterator iter_resourceindex;
									for(iter_resourceindex = currentpackage->m_listResourceItemIndex.begin() ; iter_resourceindex != currentpackage->m_listResourceItemIndex.end() ; iter_resourceindex++)
									{
										resourceindex = *iter_resourceindex;
										if( resourceindex != -1 && resourceindex < g_ResourceManager.m_TotalCountResourceItem )
										{	
											//자원
											rect.left =117;
											rect.top = 165;
											rect.right = 133;
											rect.bottom = 181;

											DWORD commandid = (DWORD)2<<16 | g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceIndex;								
											
											m_listboxResourceItem->InsertListItem(_T(""), 0, commandid );		
											m_listboxResourceItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
											m_listboxResourceItem->SetItemAttrib(count, 0, -1 , rect);
											m_listboxResourceItem->SetItemAttrib(count, 2, lifeskillindex, rect);
											
											if( g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceName[0] != 0 )
												m_listboxResourceItem->SetItemText(count, 3, g_ResourceManager.m_arrayResourceItem[resourceindex].m_ResourceName );
																								
											m_listboxResourceItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));
											m_listboxResourceItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 255, 255, 255));						
											count++;			
										}
									}

									break;
								}
							}
						}
					
					}
					else
					{
						rect.top = rect.left = 12;
						rect.bottom = rect.right = 24;
						
						DWORD commandid = (DWORD)1<<16 | currentgroup->m_ResourceGroupSubItemIndex;								
						
						m_listboxResourceItem->InsertListItem(_T(""), 0, commandid );		
						m_listboxResourceItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxResourceItem->SetItemAttrib(count, 0, chattingbuttonindex , rect);
						m_listboxResourceItem->SetItemAttrib(count, 2, -1, rect);
						
						if( currentgroup->m_ResourceGroupSubItemName[0] != 0 )
							m_listboxResourceItem->SetItemText(count, 1, currentgroup->m_ResourceGroupSubItemName );
						
						m_listboxResourceItem->SetItemAttrib(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));
						m_listboxResourceItem->SetItemAttribHighlight(count, 1, D3DCOLOR_ARGB(255, 255, 255, 255));						
						count++;				
					}
					
				}

				
			}
		}
	}
}
void _XWindow_LifeSkill::SetListResourceNeedItem(BOOL bRenewScroll)
{
	if( !m_listboxResourceNeedItem ) return;
	if( g_ResourceManager.m_TotalCountResourceItem == 0 ) return;
	if( m_indexSelectedResourceItem == -1 ) return;
	if( m_indexSelectedResourceItem >= g_ResourceManager.m_TotalCountResourceItem ) return;
	
	m_listboxResourceNeedItem->DeleteAllItem();	
	
	int count = 0;
	int itemtotalcount = 0;
	int	imageiconindex = -1;
	int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	int characterwindowimageindex	= g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int nGradeiconindex = g_MainInterfaceTextureArchive.FindResource( "MI_grade_01.tga" ); //Author : 양희왕 //breif : 그레이드 추가
	g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_currentcountNeeditem = 0;
	RECT rect = {0, 0, 0, 0};
	RECT rect1 = {214, 31, 246, 63};
	RECT rect2 = {191, 33, 223, 65};
	TCHAR messagestring[256];
	_XCollectionResourceItem* currentneeditem = NULL;
	list <_XCollectionResourceItem*>::iterator iter_needitem;
	for(iter_needitem = g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.begin() ; iter_needitem != g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_listNeedItem.end() ; iter_needitem++)
	{
		currentneeditem = *iter_needitem;
		if(currentneeditem)
		{				
			int category = currentneeditem->Get_m_cType();
			int itemid = currentneeditem->Get_m_sID();									
			
			g_pInventory_Window->GetIconRect(category, itemid, imageiconindex, rect, TRUE);
			
			int grade = _XGameItem::GetItemGrade(category, itemid);
			
			DWORD commandid = (DWORD)category<<16 | itemid;
								
			m_listboxResourceNeedItem->InsertListItem(_T(""), 0, commandid);
			m_listboxResourceNeedItem->SetIconArchive(count, &g_MainInterfaceTextureArchive);
			m_listboxResourceNeedItem->SetItemAttrib(count, 0, imageiconindex, rect);
			
			if( grade > 0 && grade < 6 )	// 현재 6개 등급이 존재
			{
				grade--;
				RECT rect3 = {1+32*grade, 1, 1+32*grade+31, 31 };
				m_listboxResourceNeedItem->SetItemAttrib(count, 4, nGradeiconindex, rect3);
			}
			//m_listboxResourceNeedItem->SetItemAttrib(count, 1, inventoryimageindex, rect1);
			
			m_listboxResourceNeedItem->SetItemText(count, 2, _XGameItem::GetItemName(category, itemid) );

			
			itemtotalcount = 0;
			for(int i = _XINVENTORY_SLOTTYPE_POCKETSTART ; i < _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART+g_pLocalUser->m_cBackPackExtend ; i++)
			{
				if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == category && g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();	
			}
		#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장 //빈슬롯 없으면 캐시 행낭 검색
			for(i = g_pInventory_Window->GetCashBackpackStartSlotPosition(); i < g_pLocalUser->m_nCashBackPackMaxValue; i++)
			{
				if(g_pLocalUser->m_UserItemList[i].Get_m_cType() == category && g_pLocalUser->m_UserItemList[i].Get_m_sID() == itemid)
					itemtotalcount += g_pLocalUser->m_UserItemList[i].Get_m_ucCount();												
			}
		#endif

			if( itemtotalcount >= currentneeditem->Get_m_ucCount() )
			{
				memset( messagestring, 0, sizeof(TCHAR)*256 );
				sprintf(messagestring, "(%d / %d)", currentneeditem->Get_m_ucCount(), currentneeditem->Get_m_ucCount() );
				currentneeditem->m_bComplete = true;
				g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_currentcountNeeditem++;
									
				m_listboxResourceNeedItem->SetItemAttrib(count, 1, characterwindowimageindex, rect2);

				if( messagestring[0] != 0 )
					m_listboxResourceNeedItem->SetItemText(count, 3, messagestring );

				m_listboxResourceNeedItem->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 235, 217, 49));
				m_listboxResourceNeedItem->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 235, 217, 49));		
				m_listboxResourceNeedItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 235, 217, 49));
				m_listboxResourceNeedItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 235, 217, 49));		
			}
			else
			{
				memset( messagestring, 0, sizeof(TCHAR)*256 );
				sprintf(messagestring, "(%d / %d)", itemtotalcount, currentneeditem->Get_m_ucCount() );
				currentneeditem->m_bComplete = false;
				
				m_listboxResourceNeedItem->SetItemAttrib(count, 1, inventoryimageindex, rect1);

				if( messagestring[0] != 0 )
					m_listboxResourceNeedItem->SetItemText(count, 3, messagestring );
				
				m_listboxResourceNeedItem->SetItemAttrib(count, 2, D3DCOLOR_ARGB(255, 180, 180, 180));
				m_listboxResourceNeedItem->SetItemAttribHighlight(count, 2, D3DCOLOR_ARGB(255, 180, 180, 180));		
				m_listboxResourceNeedItem->SetItemAttrib(count, 3, D3DCOLOR_ARGB(255, 180, 180, 180));
				m_listboxResourceNeedItem->SetItemAttribHighlight(count, 3, D3DCOLOR_ARGB(255, 180, 180, 180));		
			}
			
			count++;				
		}
	}
			
	if( bRenewScroll )
		m_listboxResourceNeedItem->SetLineScrollPos(0);
}

void _XWindow_LifeSkill::SetListItemCollectionExplain(void)
{
#ifdef _XTS_NEWLIFEWINDOW
	if(!m_pSelectedCollectionItem)
	{
		m_lstboxCollectionExplain->DeleteAllItem();
		m_lstboxCollectionExplain->SetScrollPos(0);
		return;
	}

	_XStringSplitInfo splitinfo;
	
	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN ||
        g_LanguageType == _XLANGUAGE_TYPE_JAPANESE ||
        g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
		g_XBaseFont->SplitString(m_pSelectedCollectionItem->m_CollectionExplain, 270, splitinfo);
    else
		g_XBaseFont->SplitString(m_pSelectedCollectionItem->m_CollectionExplain, 300, splitinfo, _XSTRINGSPLITFUNCTION_SPACECHAR);

	TCHAR msgbuffer[512];
	int linecount = 0;

	m_lstboxCollectionExplain->DeleteAllItem();
	m_lstboxCollectionExplain->SetScrollPos(0);

	for(int i = 0 ; i < splitinfo.splitcount ; ++i)
	{
		memset(msgbuffer, 0, sizeof(msgbuffer));
		strncpy(msgbuffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
		m_lstboxCollectionExplain->InsertListItem(_T(""), 0, 0);
		m_lstboxCollectionExplain->SetItemText(linecount, 0, msgbuffer);
		m_lstboxCollectionExplain->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 181, 181, 181));
		linecount++;
	}

	m_lstboxCollectionExplain->SetScrollPos(0);
	m_lstboxCollectionExplain->SetLineScrollPos(0);
#endif
}

void _XWindow_LifeSkill::SetSelectedCollectionItemIndex(int index)
{
	if( !g_CollectionManager.m_listCollectionItem.empty() )
	{
		index--;//1을 뺀다.		
		int count = 0 ;	
		list <_XCollectionItem*>::iterator iter_collection;
		for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; iter_collection++)
		{
			_XCollectionItem* currentcollection = *iter_collection;
			if(currentcollection)
			{				
				if( currentcollection->m_CollectionIndex == index )
				{
					m_pSelectedCollectionItem = currentcollection;

					if( m_pSelectedCollectionItem )
					{
						if( !m_pSelectedCollectionItem->m_bComplete && !m_pSelectedCollectionItem->m_bEnableExchage )
						{
							m_btnCollectionNeedItemInput->ShowWindow(TRUE);
						}
						else
						{
							m_btnCollectionNeedItemInput->ShowWindow(FALSE);
						}
#ifdef _XTS_NEWLIFEWINDOW
						SetListItemCollectionExplain();
#endif
					}
					break;
				}
			}
		}
	}
}
void _XWindow_LifeSkill::SetSelectedResourceItemOrGrupItem(DWORD commandid)
{
	int grouporitem = (commandid & 0xFFFF0000) >> 16;
	int index = (commandid & 0x0000FFFF);
	
	if( grouporitem == 1 )// 그룹이 선택되었다.
	{
		if( !g_ResourceManager.m_listResourceGroupItem.empty() )
		{
			_XResourceGroupItem* currentgroup = NULL;
			list <_XResourceGroupItem*>::iterator iter_group;
			for(iter_group = g_ResourceManager.m_listResourceGroupItem.begin() ; iter_group != g_ResourceManager.m_listResourceGroupItem.end() ; iter_group++)
			{
				currentgroup = *iter_group;
				if(currentgroup)
				{				
					if( currentgroup->m_ResourceGroupSubItemIndex == index )
					{
						if( currentgroup->m_bSelectedGroupSubItem)
						{
							currentgroup->m_bSelectedGroupSubItem = false;
						}
						else
						{
							currentgroup->m_bSelectedGroupSubItem = true;
						}
						SetListResourceItem();
						return;
					}
				}
			}
		}
	}
	else if( grouporitem == 2 )// 자원 아이템이 선택되었다.
	{
		m_indexSelectedResourceItem = index;
		SetListResourceNeedItem();
	}
}


int _XWindow_LifeSkill::SearchSlotRegion(RECT* pRect)
{
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	int count = 0;
	switch(m_SelectedTab) 
	{
	case _XLIFETAB_LIFECLASS1 :
		{
			if( m_pSelectedCollectionItem )
			{
				count = m_pSelectedCollectionItem->m_listNeedItem.size();
				if( count > 16 )
					count = 16;
				
				for(int i = 0 ; i < count ; i++)
				{
					if( scrnpos->x > m_CollectionNeedItemSlotPosition[i].x && 
						scrnpos->x < m_CollectionNeedItemSlotPosition[i].x+32 &&
						scrnpos->z > m_CollectionNeedItemSlotPosition[i].y &&
						scrnpos->z < m_CollectionNeedItemSlotPosition[i].y+32 )
					{
						if( pRect )
						{
							pRect->left   = m_CollectionNeedItemSlotPosition[i].x-1;
							pRect->top    = m_CollectionNeedItemSlotPosition[i].y-1;
							pRect->right  = m_CollectionNeedItemSlotPosition[i].x + 29;
							pRect->bottom = m_CollectionNeedItemSlotPosition[i].y + 29;
						}
						
						return i;					
					}
				}
			}
		}
		break;
	case _XLIFETAB_LIFECLASS2 :
		{
			if( m_indexSelectedResourceItem != -1 && m_indexSelectedResourceItem < g_ResourceManager.m_TotalCountResourceItem )
			{
				count = g_ResourceManager.m_arrayResourceItem[m_indexSelectedResourceItem].m_totalcountNeeditem+1;
				
				if( count > 8 )
					count = 8;
				
				for(int i = 0 ; i < count ; i++)
				{
					if( scrnpos->x > m_ResourcNeedItemSlotPosition[i].x && 
						scrnpos->x < m_ResourcNeedItemSlotPosition[i].x+32 &&
						scrnpos->z > m_ResourcNeedItemSlotPosition[i].y && 
						scrnpos->z < m_ResourcNeedItemSlotPosition[i].y+32 )
					{
						if( pRect )
						{
							pRect->left   = m_ResourcNeedItemSlotPosition[i].x-1;
							pRect->top    = m_ResourcNeedItemSlotPosition[i].y-1;
							pRect->right  = m_ResourcNeedItemSlotPosition[i].x + 29;
							pRect->bottom = m_ResourcNeedItemSlotPosition[i].y + 29;
						}
						
						return i;
					}
				}
			}
		}
		break;
	}

	return -1;
}
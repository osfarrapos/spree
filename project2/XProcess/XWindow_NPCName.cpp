// XWindow_NPCName.cpp: implementation of the _XWindow_NPCName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_NPCName.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCName::_XWindow_NPCName()
{
	memset( m_NPCOccupationString, 0, sizeof(TCHAR) * _XDEF_MAXNPCNAMESTRINGSIZE );
	memset( m_NPCNameString, 0, sizeof(TCHAR) * _XDEF_MAXNPCNAMESTRINGSIZE );	
	
	m_YellowImpressionMark	= NULL;
	m_GreenImpressionMark	= NULL;
	m_BlueImpressionMark	= NULL;
	m_OrangeImpressionMark	= NULL;
	m_WhiteImpressionMark	= NULL;
	
	m_YellowQuestionMark	= NULL;
	m_GreenQuestionMark		= NULL;
	m_BlueQuestionMark		= NULL;
	m_OrangeQuestionMark	= NULL;
	m_WhiteQuestionMark		= NULL;
	
	m_imageIconBorder		= NULL;			// NPC의 직업을 나타내는 아이콘의 바탕  
	m_imageNPCNameBorder	= NULL;			// NPC의 이름 윈도우
	m_imageNPCJobIcon		= NULL;			// NPC의 직업을 나타내는 아이콘
	
	m_imageMatchPRLeftSide			= NULL;
	m_imageMatchPRRightSide			= NULL;
	m_imageMatchPRBorder			= NULL;
	m_imageMatchPRIcon				= NULL;
	
	m_imagePersonalStoreBorder		= NULL;
	m_imagePersonalStoreIcon		= NULL;
	m_imagePersonalStoreLeftSide	= NULL;
	m_imagePersonalStoreRightSide	= NULL;
	m_imageEventIcon01				= NULL;
	m_imageEventIcon02				= NULL;
	m_imageEventIcon03				= NULL;
	m_imageEventIcon04				= NULL;
	
#ifdef	_XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130 
	m_imageCastleCaptainBorder		= NULL;			// 장주 NPC 창 이미지
#endif

	m_EventType = -1;	
}

_XWindow_NPCName::~_XWindow_NPCName()
{
	DestroyWindow();
}


BOOL _XWindow_NPCName::Initialize( void )
{		
	int npcnameindex				= g_MainInterfaceTextureArchive.FindResource( "mi_npcname.tga" ); 
	int npcnameindex3				= g_MainInterfaceTextureArchive.FindResource( "mi_npcname3.tga" ); 
	int mainbuttonindex				= g_MainInterfaceTextureArchive.FindResource( "MI_main_button.tga" );
	int eventindex					= g_MainInterfaceTextureArchive.FindResource( "mi_npcevent.tga" );
	int quest1resourceindex			= g_MainInterfaceTextureArchive.FindResource("MI_quest1.tga");	
	int resourceindex				= g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	int characterwindowimageindex	= g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int manorresourceindex02		= g_MainInterfaceTextureArchive.FindResource("MI_manor02.tga");

	int pvpindex1 = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pvpindex1 = g_MainInterfaceTextureArchive.FindResource("tw_mi_pvp_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pvpindex1 = g_MainInterfaceTextureArchive.FindResource("us_mi_pvp_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pvpindex1 = g_MainInterfaceTextureArchive.FindResource("rs_mi_pvp_01.tga");
	}
	else
	{
		pvpindex1 = g_MainInterfaceTextureArchive.FindResource("mi_pvp_01.tga");
	}
	
	int pvpindex2					= -1;
	
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

	m_WindowPosition.x = 0;
	m_WindowPosition.y = 0;

	m_WindowSize.cx = 135;
	m_WindowSize.cy = 36;
	
	// 비무 마크 ( 30 X 30 )
	m_imageMatchPRIcon = new _XImageStatic;
	m_imageMatchPRIcon->Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, pvpindex2 );	
	m_imageMatchPRIcon->SetClipRect( 225, 33, 255, 63 );
	
	// 비무 홍보를 나타내는 윈도우 바탕  (310 X 36)
	m_imageMatchPRBorder = new _XImageStatic;
	m_imageMatchPRBorder->Create( 0, 0, 310, 36,	&g_MainInterfaceTextureArchive, quest1resourceindex );
	m_imageMatchPRBorder->SetClipRect(  11, 118, 211, 154 );
	m_imageMatchPRBorder->SetScale( 1.55f, 1.0f );
	
	// 비무 홍보를 나타내는 윈도우 왼쪽 바탕  (8 X 40)
	m_imageMatchPRLeftSide = new _XImageStatic;
	m_imageMatchPRLeftSide->Create( 0, 0, 8, 40,	&g_MainInterfaceTextureArchive, pvpindex1 );
	m_imageMatchPRLeftSide->SetClipRect( 134, 216, 142, 256 );
	
	// 비무 홍보를 나타내는 윈도우 오른쪽 바탕  (8 X 40)
	m_imageMatchPRRightSide = new _XImageStatic;
	m_imageMatchPRRightSide->Create( 0, 0, 8, 40, &g_MainInterfaceTextureArchive, pvpindex1 );
	m_imageMatchPRRightSide->SetClipRect( 134, 216, 142, 256 );
	m_imageMatchPRRightSide->SetScale( -1.0f, 1.0f );

	// 노점 아이콘 ( 16 X 16) 
	m_imagePersonalStoreIcon = new _XImageStatic;
	m_imagePersonalStoreIcon->Create( 0, 0, 16, 16, &g_MainInterfaceTextureArchive, resourceindex );	
	m_imagePersonalStoreIcon->SetClipRect( 184, 4, 200, 20 );
	
	// 노점을 나타내는 윈도우 바탕  (186 X 20)
	m_imagePersonalStoreBorder = new _XImageStatic;
	m_imagePersonalStoreBorder->Create( 0, 0, 186, 20,	&g_MainInterfaceTextureArchive, quest1resourceindex );
	m_imagePersonalStoreBorder->SetClipRect(  23, 118, 209, 138 );
	
	// 노점을 나타내는 윈도우 왼쪽 바탕  (10 X 30)
	m_imagePersonalStoreLeftSide = new _XImageStatic;
	m_imagePersonalStoreLeftSide->Create( 0, 0, 10, 30,	&g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_imagePersonalStoreLeftSide->SetClipRect( 166, 13, 176, 43 );
	
	// 노점을 나타내는 윈도우 오른쪽 바탕  (10 X 30)
	m_imagePersonalStoreRightSide = new _XImageStatic;
	m_imagePersonalStoreRightSide->Create( 0, 0, 10, 30, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_imagePersonalStoreRightSide->SetClipRect( 166, 13, 176, 43 );
	m_imagePersonalStoreRightSide->SetScale( -1.0f, 1.0f );
	
	// NPC의 이름 윈도우 (102 X 36)
	m_imageNPCNameBorder = new _XImageStatic;

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_imageNPCNameBorder->Create( m_WindowPosition.x + 33, m_WindowPosition.y, m_WindowPosition.x + 164+33, m_WindowPosition.y + 36,
									&g_MainInterfaceTextureArchive, npcnameindex3 );
		m_imageNPCNameBorder->SetClipRect(  0, 107, 164, 142 );
	}
	else
	{
		m_imageNPCNameBorder->Create( m_WindowPosition.x + 33, m_WindowPosition.y, m_WindowPosition.x + 135, m_WindowPosition.y + 36,
									 &g_MainInterfaceTextureArchive, npcnameindex3 );
		m_imageNPCNameBorder->SetClipRect(  37, 68, 139, 104 );
	}	
	m_imageNPCNameBorder->ShowWindow(TRUE);
	
#ifdef	_XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130 
	// 장주 NPC 창 이미지( 163 X 58 )
	m_imageCastleCaptainBorder = new _XImageStatic;
	m_imageCastleCaptainBorder->Create( m_WindowPosition.x + 33, m_WindowPosition.y, m_WindowPosition.x + 135, m_WindowPosition.y + 36,
		&g_MainInterfaceTextureArchive, manorresourceindex02 );
	m_imageCastleCaptainBorder->SetClipRect(  2, 71, 165, 129 );
#endif
	
	// NPC의 직업을 나타내는 아이콘 (30 X 30)
	m_imageNPCJobIcon = new _XImageStatic;	
	m_imageNPCJobIcon->Create( m_WindowPosition.x + 3, m_WindowPosition.y + 3, m_WindowPosition.x + 33, m_WindowPosition.y + 33,
		&g_MainInterfaceTextureArchive, npcnameindex );
	m_imageNPCJobIcon->SetClipRect(  10, 10, 40, 40 );
	m_imageNPCJobIcon->ShowWindow(TRUE);
	
	// NPC의 직업을 나타내는 아이콘의 바탕 ( 36 X 36)
	m_imageIconBorder = new _XImageStatic;
	m_imageIconBorder->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+36, m_WindowPosition.y+36,
		&g_MainInterfaceTextureArchive, npcnameindex3);
	m_imageIconBorder->SetClipRect( 0, 68, 36, 104);
	m_imageIconBorder->ShowWindow(TRUE);
			
	// 행운의 문자 이벤트 아이콘 ( 53 X 37)
	m_imageEventIcon01 = new _XImageStatic;
	m_imageEventIcon01->Create( 0, 0, 53, 37,	&g_MainInterfaceTextureArchive, eventindex);
	m_imageEventIcon01->SetClipRect( 3, 2, 56, 39 );
	
	// 훼밀리마트 이벤트 아이콘 ( 49 X 45)
	m_imageEventIcon02 = new _XImageStatic;
	m_imageEventIcon02->Create( 0, 0, 49, 45,	&g_MainInterfaceTextureArchive, eventindex);
	m_imageEventIcon02->SetClipRect( 55, 0, 104, 45 );
	
	// 예약가입 이벤트 아이콘 ( 152 X 40)
	m_imageEventIcon03 = new _XImageStatic;
	m_imageEventIcon03->Create( 0, 0, 152, 40,	&g_MainInterfaceTextureArchive, eventindex);
	m_imageEventIcon03->SetClipRect( 104, 0, 256, 40 );
	
	// 상용화 이벤트 아이콘 ( 92 X 33)
	m_imageEventIcon04 = new _XImageStatic;
	m_imageEventIcon04->Create( 0, 0, 218, 94,	&g_MainInterfaceTextureArchive, eventindex);
	m_imageEventIcon04->SetClipRect( 10, 48, 228, 142 );
	

	// 퀘스트 마크 !
	m_BlueImpressionMark = new _XImageStatic;
	m_BlueImpressionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT bluerect = { 0, 200, 28, 228  };
	m_BlueImpressionMark->SetClipRect( bluerect );
	
	m_YellowImpressionMark = new _XImageStatic;
	m_YellowImpressionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT yellowrect = { 28, 200, 56, 228  };
	m_YellowImpressionMark->SetClipRect( yellowrect );
	
	m_GreenImpressionMark = new _XImageStatic;
	m_GreenImpressionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT greenrect = { 56, 200, 84, 228  };
	m_GreenImpressionMark->SetClipRect( greenrect );
	
	m_OrangeImpressionMark = new _XImageStatic;
	m_OrangeImpressionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+2816,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT orangerect = { 84, 200, 112, 228  };
	m_OrangeImpressionMark->SetClipRect( orangerect );
	
	m_WhiteImpressionMark = new _XImageStatic;
	m_WhiteImpressionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT whiterect = { 136, 98, 164, 126  };
	m_WhiteImpressionMark->SetClipRect( whiterect );
	
	// 힌트 마크 ?
	m_BlueQuestionMark = new _XImageStatic;
	m_BlueQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT questionrect = { 0, 228, 28, 256  };
	m_BlueQuestionMark->SetClipRect( questionrect );
	
	m_YellowQuestionMark = new _XImageStatic;
	m_YellowQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT questionrect2 = { 28, 228, 56, 256  };
	m_YellowQuestionMark->SetClipRect( questionrect2 );
	
	m_GreenQuestionMark = new _XImageStatic;
	m_GreenQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT questionrect3 = { 56, 228, 84, 256  };
	m_GreenQuestionMark->SetClipRect( questionrect3 );
	
	m_OrangeQuestionMark = new _XImageStatic;
	m_OrangeQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT questionrect4 = { 84, 228, 112, 256  };
	m_OrangeQuestionMark->SetClipRect( questionrect4 );
	
	m_WhiteQuestionMark = new _XImageStatic;
	m_WhiteQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+28, m_WindowPosition.y+28,
		&g_MainInterfaceTextureArchive, mainbuttonindex);
	RECT questionrect5 = { 136, 126, 164, 154  };
	m_WhiteQuestionMark->SetClipRect( questionrect5 );
	
	
	return TRUE;
}

void _XWindow_NPCName::MoveWindow( int X, int Y )
{	
	m_WindowPosition.x = X;
	m_WindowPosition.y = Y;

	m_imageNPCNameBorder->MoveWindow( X + 33, Y, X + 135, Y + 36);		// NPC의 이름 윈도우 (102 X 36)
	m_imageNPCJobIcon->MoveWindow( X + 3, Y + 3, X + 33, Y + 33);		// NPC의 직업을 나타내는  아이콘 (30 X 30)
	m_imageIconBorder->MoveWindow( X, Y, X + 36, Y + 36);				// NPC의 직업을 나타내는 아이콘의 바탕 ( 36 X 36)		
		
}

void _XWindow_NPCName::DestroyWindow( void )
{	
	SAFE_DELETE( m_imageMatchPRLeftSide );	
	SAFE_DELETE( m_imageMatchPRRightSide );
	SAFE_DELETE( m_imageMatchPRBorder );	
	SAFE_DELETE( m_imageMatchPRIcon );		

	SAFE_DELETE( m_imagePersonalStoreLeftSide );
	SAFE_DELETE( m_imagePersonalStoreRightSide );
	SAFE_DELETE( m_imagePersonalStoreBorder );	
	SAFE_DELETE( m_imagePersonalStoreIcon );	
	SAFE_DELETE( m_imageEventIcon01 );			
	SAFE_DELETE( m_imageEventIcon02 );			
	SAFE_DELETE( m_imageEventIcon03 );			
	SAFE_DELETE( m_imageEventIcon04 );			

#ifdef	_XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130 
	SAFE_DELETE( m_imageCastleCaptainBorder );
#endif
	
	SAFE_DELETE( m_imageIconBorder );
	SAFE_DELETE( m_imageNPCNameBorder );
	SAFE_DELETE( m_imageNPCJobIcon );

	SAFE_DELETE( m_YellowImpressionMark );
	SAFE_DELETE( m_GreenImpressionMark );
	SAFE_DELETE( m_BlueImpressionMark );
	SAFE_DELETE( m_OrangeImpressionMark );
	SAFE_DELETE( m_WhiteImpressionMark );
	
	SAFE_DELETE( m_YellowQuestionMark );
	SAFE_DELETE( m_GreenQuestionMark );
	SAFE_DELETE( m_BlueQuestionMark );
	SAFE_DELETE( m_OrangeQuestionMark );
	SAFE_DELETE( m_WhiteQuestionMark );

}

void _XWindow_NPCName::Draw( int X, int Y )
{
	int centerxpos = X + 85;
	
	m_imageNPCNameBorder->MoveWindow( X + 33, Y);		// NPC의 이름 윈도우 (102 X 36)
	m_imageNPCJobIcon->MoveWindow( X + 3, Y + 3);		// NPC의 직업을 나타내는 아이콘 (30 X 30)
	m_imageIconBorder->MoveWindow( X, Y);				// NPC의 직업을 나타내는 아이콘의 바탕 ( 36 X 36)	

	m_imageNPCNameBorder->DrawWithRegion();	
	m_imageNPCJobIcon->DrawWithRegion();	
	m_imageIconBorder->DrawWithRegion();

	g_XBaseFont->SetColor( _XSC_INFORMATION );
	g_XBaseFont->PrintAlign( centerxpos, Y + 5,  1.0f, _XFONT_ALIGNTYPE_CENTER, m_NPCOccupationString );
	g_XBaseFont->PrintAlign( centerxpos, Y + 21, 1.0f, _XFONT_ALIGNTYPE_CENTER, m_NPCNameString );
	
	g_XBaseFont->Flush();
}

void _XDrawRoundRectAngle( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT baseheight, D3DCOLOR color );

void _XWindow_NPCName::Draw( int X, int Y, LPTSTR pNPCOccupationStr, LPTSTR pNPCNameStr, BOOL selected, 
							_XNPCQUESTTYPE questtype, _XNPCJOBCLASS jobclass, _XNPCNAME_MODE npcnamemode, _XFONT_ALIGNTYPE aligntype )
{
	if( npcnamemode == _XNPCNAMEMODE_NPC)
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
			g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			SetNPCJobIcon( jobclass );
			
			int width1 = g_XBaseFont->GetWidth( pNPCOccupationStr );
			int width2 = g_XBaseFont->GetWidth( pNPCNameStr );
			if( width1 < width2 )
			{
				width1 = width2;
			}
			
			if( aligntype == _XFONT_ALIGNTYPE_LEFT )
			{
				m_WindowPosition.x = X;
			}
			else
			{
				m_WindowPosition.x = X - (width1>>1) - (36>>1);
			}
			m_WindowPosition.y = Y;
			
			m_WindowSize.cx = 36 + width1;
			m_WindowSize.cy = 36;

			_XDrawSolidBar( m_WindowPosition.x-1, m_WindowPosition.y - 1, m_WindowPosition.x + m_WindowSize.cx + 2, m_WindowPosition.y + m_WindowSize.cy+1, D3DCOLOR_ARGB(96, 0,0,0) );
			_XDrawRoundRectAngle( m_WindowPosition.x-2, m_WindowPosition.y - 2, m_WindowPosition.x + m_WindowSize.cx + 2, m_WindowPosition.y + m_WindowSize.cy + 1, 0.0f, 0xFF7F7F7F );			

			//m_imageNPCNameBorder->MoveWindow( X - (m_WindowSize.cx>>1) + 33, Y);		// NPC의 이름 윈도우 (102 X 36)
			m_imageIconBorder->MoveWindow( m_WindowPosition.x, Y);				// NPC의 직업을 나타내는 아이콘의 바탕 ( 36 X 36)
			m_imageNPCJobIcon->MoveWindow( m_WindowPosition.x + 3, Y + 3);		// NPC의 직업을 나타내는 아이콘 (30 X 30)	
			
			//m_imageNPCNameBorder->DrawWithRegion();	
			m_imageNPCJobIcon->DrawWithRegion();
			m_imageIconBorder->DrawWithRegion();	
			
			if( pNPCOccupationStr )
			{
				g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
				g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000 );
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 36 + (width1>>1), Y + 5,  1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCOccupationStr );
				g_XBaseFont->Flush();
				g_XBaseFont->DisableGrowMode();
			}
			
			if( pNPCNameStr )
			{
				if( selected )	
					g_XBaseFont->SetColor( _XSC_DEFAULT_WHITE );		
				else	
					g_XBaseFont->SetColor( _XSC_INFORMATION );		
				
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 36 + (width1>>1), Y + 21, 1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCNameStr );
			}
			
			g_XBaseFont->Flush();
						
			int posy = Y-33;
			
			/*
			if( m_EventType != -1 )
			{
				if( X != 85 || Y != 15 )//NPC 대화모드 일때는 그리지 않는다. ( 좌표(85,15)는 대화모드시 고정 위치)
				{
					if( m_EventType == 0 )
					{
						posy = Y-33-37;
						g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
						m_imageEventIcon01->Draw( X-72, Y-37 );
						g_XBaseFont->PrintAlign( X+21, Y-15-11, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NPCNAME_2263) );
					}
					else if( m_EventType == 1 )
					{
						posy = Y-33-37;
						g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
						m_imageEventIcon02->Draw( X-72, Y-45 );
						g_XBaseFont->PrintAlign( X+25, Y-15-11, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NPCNAME_2264) );
					}
					else if( m_EventType == 2 )
					{
						posy = Y-33-37;
						g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
						m_imageEventIcon03->Draw( X-72, Y-38 );
						//g_XBaseFont->PrintAlign( X+21, Y-15-11, 1.0f, _XFONT_ALIGNTYPE_CENTER, _T("예약가입이벤트") ); //add stringtable
					}
					else if( m_EventType == 3 )// 상용화 이벤트
					{
						posy = Y-33-37;
						g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
						m_imageEventIcon04->Draw( X-46, Y-38 );
					}
					else if( m_EventType == 4 )// 월드컵 이벤트
					{
						posy = Y-94-37;
						g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
						m_imageEventIcon04->Draw( X-109, Y-99 );
					}
				}
			}
			*/


			switch(questtype)
			{
			case _XNPC_HAVEQUEST_YELLOW :
				m_YellowImpressionMark->Draw(X-16 , posy);
				break;
			case _XNPC_HAVEQUEST_GREEN :
				m_GreenImpressionMark->Draw(X-16, posy);
				break;
			case _XNPC_HAVEQUEST_BLUE :
				m_BlueImpressionMark->Draw(X-16, posy);
				break;
			case _XNPC_HAVEQUEST_ORANGE:
				m_OrangeImpressionMark->Draw(X-16, posy);
				break;		
			case _XNPC_HAVEQUEST_WHITE:
				m_WhiteImpressionMark->Draw(X-16, posy);
				break;		
			case _XNPC_HAVEQUEST_QUESTIONYELLOW:
				m_YellowQuestionMark->Draw(X-16, posy);
				break;
			case _XNPC_HAVEQUEST_QUESTIONGREEN:
				m_GreenQuestionMark->Draw(X-16, posy);
				break;
			case _XNPC_HAVEQUEST_QUESTIONBLUE:
				m_BlueQuestionMark->Draw(X-16, posy);
				break;
			case _XNPC_HAVEQUEST_QUESTIONORANGE:
				m_OrangeQuestionMark->Draw(X-16, posy);
				break;	
			case _XNPC_HAVEQUEST_QUESTIONWHITE:
				m_WhiteQuestionMark->Draw(X-16, posy);
				break;	
			}		
		}
		else
		{
#ifdef	_XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130 
			if( jobclass != _XNPC_JOBCLASS_CASTLECAPTAIN )
			{
#endif
				SetNPCJobIcon( jobclass );
				
				m_WindowPosition.x = X;
				m_WindowPosition.y = Y;
				
				m_WindowSize.cx = 135;
				m_WindowSize.cy = 36;
				
				m_imageNPCNameBorder->MoveWindow( X - (m_WindowSize.cx>>1) + 33, Y);		// NPC의 이름 윈도우 (102 X 36)
				m_imageIconBorder->MoveWindow( X - (m_WindowSize.cx>>1), Y);				// NPC의 직업을 나타내는 아이콘의 바탕 ( 36 X 36)
				m_imageNPCJobIcon->MoveWindow( X - (m_WindowSize.cx>>1) + 3, Y + 3);		// NPC의 직업을 나타내는 아이콘 (30 X 30)	
				m_imageNPCNameBorder->DrawWithRegion();	
				m_imageNPCJobIcon->DrawWithRegion();
				m_imageIconBorder->DrawWithRegion();	
				
				if( pNPCOccupationStr )
				{
					g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
					g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000 );
					g_XBaseFont->PrintAlign( X + 17, Y + 5,  1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCOccupationStr );
					g_XBaseFont->Flush();
					g_XBaseFont->DisableGrowMode();
				}
				
				if( pNPCNameStr )
				{
					if( selected )	
						g_XBaseFont->SetColor( _XSC_DEFAULT_WHITE );		
					else	
						g_XBaseFont->SetColor( _XSC_INFORMATION );		
					
					g_XBaseFont->PrintAlign( X + 17, Y + 21, 1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCNameStr );
				}
				
				g_XBaseFont->Flush();
				
				int posy = Y-33;
				if( m_EventType != -1 )
				{
					if( X != 85 || Y != 15 )//NPC 대화모드 일때는 그리지 않는다. ( 좌표(85,15)는 대화모드시 고정 위치)
					{
						if( m_EventType == 0 )
						{
							posy = Y-33-37;
							g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
							m_imageEventIcon01->Draw( X-72, Y-37 );
							g_XBaseFont->PrintAlign( X+21, Y-15-11, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NPCNAME_2263) );
						}
						else if( m_EventType == 1 )
						{
							posy = Y-33-37;
							g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
							m_imageEventIcon02->Draw( X-72, Y-45 );
							g_XBaseFont->PrintAlign( X+25, Y-15-11, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NPCNAME_2264) );
						}
						else if( m_EventType == 2 )
						{
							posy = Y-33-37;
							g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
							m_imageEventIcon03->Draw( X-72, Y-38 );
							//g_XBaseFont->PrintAlign( X+21, Y-15-11, 1.0f, _XFONT_ALIGNTYPE_CENTER, _T("예약가입이벤트") ); //add stringtable
						}
						else if( m_EventType == 3 )// 상용화 이벤트
						{
							posy = Y-33-37;
							g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
							m_imageEventIcon04->Draw( X-46, Y-38 );
						}
						else if( m_EventType == 4 )// 월드컵 이벤트
						{
							posy = Y-94-37;
							g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 255, 196, 54  ) );
							m_imageEventIcon04->Draw( X-109, Y-99 );
						}
					}
				}

				
				switch(questtype)
				{
				case _XNPC_HAVEQUEST_YELLOW :
					m_YellowImpressionMark->Draw(X-16 , posy);
					break;
				case _XNPC_HAVEQUEST_GREEN :
					m_GreenImpressionMark->Draw(X-16, posy);
					break;
				case _XNPC_HAVEQUEST_BLUE :
					m_BlueImpressionMark->Draw(X-16, posy);
					break;
				case _XNPC_HAVEQUEST_ORANGE:
					m_OrangeImpressionMark->Draw(X-16, posy);
					break;		
				case _XNPC_HAVEQUEST_WHITE:
					m_WhiteImpressionMark->Draw(X-16, posy);
					break;		
				case _XNPC_HAVEQUEST_QUESTIONYELLOW:
					m_YellowQuestionMark->Draw(X-16, posy);
					break;
				case _XNPC_HAVEQUEST_QUESTIONGREEN:
					m_GreenQuestionMark->Draw(X-16, posy);
					break;
				case _XNPC_HAVEQUEST_QUESTIONBLUE:
					m_BlueQuestionMark->Draw(X-16, posy);
					break;
				case _XNPC_HAVEQUEST_QUESTIONORANGE:
					m_OrangeQuestionMark->Draw(X-16, posy);
					break;	
				case _XNPC_HAVEQUEST_QUESTIONWHITE:
					m_WhiteQuestionMark->Draw(X-16, posy);
					break;	
				}
				
#ifdef	_XDEF_CASTLEBATTLE_CASTLECAPTAIN_20070130 
			}
			else
			{
				SetNPCJobIcon( jobclass );
				
				m_WindowPosition.x = X;
				m_WindowPosition.y = Y;
				
				m_WindowSize.cx = 135;
				m_WindowSize.cy = 36;
				
				m_imageCastleCaptainBorder->MoveWindow( X - (m_WindowSize.cx>>1) + 33 -60, Y-16);		// NPC의 이름 윈도우 (102 X 36)
				m_imageIconBorder->MoveWindow( X - (m_WindowSize.cx>>1), Y);				// NPC의 직업을 나타내는 아이콘의 바탕 ( 36 X 36)
				m_imageNPCJobIcon->MoveWindow( X - (m_WindowSize.cx>>1) + 3, Y + 3);		// NPC의 직업을 나타내는 아이콘 (30 X 30)	
				m_imageCastleCaptainBorder->DrawWithRegion();	
				m_imageNPCJobIcon->DrawWithRegion();
				m_imageIconBorder->DrawWithRegion();	
				
				if( pNPCOccupationStr )
				{
					g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
					g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000 );
					g_XBaseFont->PrintAlign( X + 17, Y + 5,  1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCOccupationStr );
					g_XBaseFont->Flush();
					g_XBaseFont->DisableGrowMode();
				}
				
				if( pNPCNameStr )
				{
					if( selected )	
						g_XBaseFont->SetColor( _XSC_DEFAULT_WHITE );		
					else	
						g_XBaseFont->SetColor( _XSC_INFORMATION );		
					
					g_XBaseFont->PrintAlign( X + 17, Y + 21, 1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCNameStr );
				}
				
				g_XBaseFont->Flush();
			}
#endif

		}
	}
	else if( npcnamemode == _XNPCNAMEMODE_PCPERSONALSTORE ) // 개인 상점
	{
		m_WindowPosition.x = X;
		m_WindowPosition.y = Y;
		
		m_WindowSize.cx = 192;
		m_WindowSize.cy = 26;

		m_imagePersonalStoreBorder->MoveWindow( X - (m_WindowSize.cx>>1) +3, Y+3);		
		m_imagePersonalStoreIcon->MoveWindow( X - (m_WindowSize.cx>>1) +5, Y+4);	
		m_imagePersonalStoreBorder->DrawWithRegion();
		m_imagePersonalStoreIcon->DrawWithRegion();
		
		_XDrawRectAngle( X-1- (m_WindowSize.cx>>1),  Y-1, X+192- (m_WindowSize.cx>>1), Y+26-2 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
		_XDrawRectAngle( X  - (m_WindowSize.cx>>1),  Y,   X+191- (m_WindowSize.cx>>1), Y+25-2 , 0.0f, D3DCOLOR_ARGB(255,144,108,60) );	
		_XDrawRectAngle( X+1- (m_WindowSize.cx>>1),  Y+1, X+190- (m_WindowSize.cx>>1), Y+24-2 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
		_XDrawRectAngle( X+2- (m_WindowSize.cx>>1),  Y+2, X+189- (m_WindowSize.cx>>1), Y+23-2 , 0.0f, D3DCOLOR_ARGB(255,119,116,109) );	
		
		m_imagePersonalStoreLeftSide->MoveWindow( X-6- (m_WindowSize.cx>>1), Y-5);		
		m_imagePersonalStoreRightSide->MoveWindow( X+198- (m_WindowSize.cx>>1), Y-5);	
		m_imagePersonalStoreLeftSide->DrawWithRegion();
		m_imagePersonalStoreRightSide->DrawWithRegion();
		
		if( pNPCOccupationStr )
		{
			g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
			g_XBaseFont->PrintAlign( X + 8, Y + 7,  1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCOccupationStr );
			g_XBaseFont->PrintAlign( X + 9, Y + 7,  1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCOccupationStr );
			g_XBaseFont->Flush();
		}
		
		if( pNPCNameStr )
		{
			g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
			g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 241, 169, 253 ));
			g_XBaseFont->PutsAlign( X, Y-16, _XFONT_ALIGNTYPE_CENTER, pNPCNameStr );
			g_XBaseFont->Flush();
			g_XBaseFont->DisableGrowMode();
		}

	}
	else if( npcnamemode == _XNPCNAMEMODE_PCMATCHPRMODE ) // 비무 홍보
	{
		Y = Y - 40;
		m_WindowPosition.x = X;
		m_WindowPosition.y = Y;
		
		m_WindowSize.cx = 311;
		m_WindowSize.cy = 40;
				
		m_imageMatchPRBorder->MoveWindow( X - (m_WindowSize.cx>>1) +2, Y+2);		
		m_imageMatchPRIcon->MoveWindow( X - (m_WindowSize.cx>>1) +5, Y+4);	
		m_imageMatchPRBorder->DrawWithRegion();
		m_imageMatchPRIcon->DrawWithRegion();
		
		_XDrawRectAngle( X-1- (m_WindowSize.cx>>1),  Y-1, X+311- (m_WindowSize.cx>>1), Y+40-2 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
		_XDrawRectAngle( X  - (m_WindowSize.cx>>1),  Y,   X+310- (m_WindowSize.cx>>1), Y+39-2 , 0.0f, D3DCOLOR_ARGB(255,144,108,60) );	
		_XDrawRectAngle( X+1- (m_WindowSize.cx>>1),  Y+1, X+309- (m_WindowSize.cx>>1), Y+38-2 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
		_XDrawRectAngle( X+2- (m_WindowSize.cx>>1),  Y+2, X+308- (m_WindowSize.cx>>1), Y+37-2 , 0.0f, D3DCOLOR_ARGB(255,119,116,109) );	
		
		m_imageMatchPRLeftSide->MoveWindow( X-6- (m_WindowSize.cx>>1), Y-2);		
		m_imageMatchPRRightSide->MoveWindow( X+317- (m_WindowSize.cx>>1), Y-2);	
		m_imageMatchPRLeftSide->DrawWithRegion();
		m_imageMatchPRRightSide->DrawWithRegion();
		
		if( pNPCOccupationStr )
		{
			g_XBaseFont->SetColor( _XSC_DEFAULT_BLACK );
			g_XBaseFont->PrintAlign( X + 15, Y + 23,  1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCOccupationStr );
			g_XBaseFont->PrintAlign( X + 16, Y + 23,  1.0f, _XFONT_ALIGNTYPE_CENTER, pNPCOccupationStr );
			g_XBaseFont->Flush();
		}
		
		if( pNPCNameStr )
		{
			g_XBaseFont->SetColor(D3DCOLOR_ARGB( 255, 204, 51, 170 ));
			g_XBaseFont->PutsAlign( X + 15, Y + 7, _XFONT_ALIGNTYPE_CENTER, pNPCNameStr );
			g_XBaseFont->PutsAlign( X + 16, Y + 7, _XFONT_ALIGNTYPE_CENTER, pNPCNameStr );
			g_XBaseFont->Flush();
		}
		
	}

}

void _XWindow_NPCName::SetNPCJobIcon( _XNPCJOBCLASS jobclass )
{
	switch(jobclass) {
	case _XNPC_JOBCLASS_FOOD:
		{
			m_imageNPCJobIcon->SetClipRect(  9, 9, 39, 39 );
		}
		break;
	case _XNPC_JOBCLASS_CLOTHES:
		{
			m_imageNPCJobIcon->SetClipRect(  57, 9, 87, 39 );
		}
		break;
	case _XNPC_JOBCLASS_REFINING:
		{
			m_imageNPCJobIcon->SetClipRect(  105, 9, 135, 39 );
		}
		break;
	case _XNPC_JOBCLASS_HOTEL:
		{
			m_imageNPCJobIcon->SetClipRect(  153, 9, 183, 39 );
		}
		break;
	case _XNPC_JOBCLASS_WAREHOUSE:
		{
			m_imageNPCJobIcon->SetClipRect(  202, 9, 232, 39 );
		}
		break;
	case _XNPC_JOBCLASS_DRUGSTORE:
		{
			m_imageNPCJobIcon->SetClipRect(  9, 57, 39, 87 );
		}
		break;
	case _XNPC_JOBCLASS_SHOP:
		{
			m_imageNPCJobIcon->SetClipRect(  57, 57, 87, 87 );
		}
		break;
	case _XNPC_JOBCLASS_WEAPON:
		{
			m_imageNPCJobIcon->SetClipRect(  105, 57, 135, 87 );
		}
		break;
	case _XNPC_JOBCLASS_COMMONPEOPLE:
		{
			m_imageNPCJobIcon->SetClipRect(  153, 57, 183, 87 );
		}
		break;
	case _XNPC_JOBCLASS_GAEBANGPEOPLE:
		{
			m_imageNPCJobIcon->SetClipRect(  202, 57, 232, 87 );
		}
		break;
	case _XNPC_JOBCLASS_SORIMPEOPLE:
		{
			m_imageNPCJobIcon->SetClipRect(  9, 105, 39, 135 );
		}
		break;
	case _XNPC_JOBCLASS_NOCKRIMPEOPLE:
		{
			m_imageNPCJobIcon->SetClipRect(  57, 105, 87, 135 );
		}
		break;
	case _XNPC_JOBCLASS_BEEGOONGPEOPLE:
		{
			m_imageNPCJobIcon->SetClipRect(  105, 105, 135, 135 );
		}
		break;
	case _XNPC_JOBCLASS_MOODANGPEOPLE:
		{
			m_imageNPCJobIcon->SetClipRect(  9, 153, 39, 183 );
		}
		break;
	case _XNPC_JOBCLASS_MAKYOPEOPLE:
		{
			m_imageNPCJobIcon->SetClipRect(  57, 153, 87, 183 );
		}
		break;
	case _XNPC_JOBCLASS_BLACKMARKET:
		{
			m_imageNPCJobIcon->SetClipRect(  153, 105, 183, 135 );
		}
		break;
	case _XNPC_JOBCLASS_MILITARYOFFICER:
		{
			m_imageNPCJobIcon->SetClipRect(  202, 105, 232, 135 );
		}
		break;		
	case _XNPC_JOBCLASS_HEANGSANGIN:
		{
			m_imageNPCJobIcon->SetClipRect(  153, 153, 183, 183 );
		}
		break;		
	case _XNPC_JOBCLASS_BUFF:
		{
			m_imageNPCJobIcon->SetClipRect(  202, 153, 232, 183 );
		}
		break;		
	case _XNPC_JOBCLASS_RESOURCE:
		{
			m_imageNPCJobIcon->SetClipRect(  9, 202, 39, 232 );
		}
		break;		
	case _XNPC_JOBCLASS_COLLECTION:
		{
			m_imageNPCJobIcon->SetClipRect(  57, 202, 87, 232 );
		}
		break;		
	case _XNPC_JOBCLASS_GUARD:
		{
			m_imageNPCJobIcon->SetClipRect(  105, 202, 135, 232 );
		}
		break;		
	case _XNPC_JOBCLASS_REPAIR:
		{
			m_imageNPCJobIcon->SetClipRect(  153, 202, 183, 232 );
		}
		break;		
	case _XNPC_JOBCLASS_CASTLECAPTAIN:
	case _XNPC_JOBCLASS_CASTLEATTACT :
	case _XNPC_JOBCLASS_CASTLEDEFFENCE :
	case _XNPC_JOBCLASS_TRACER :
		{
			m_imageNPCJobIcon->SetClipRect(  201, 201, 231, 231 );
		}
		break;
	}
}


void _XWindow_NPCName::SetNPCName( LPTSTR pNPCOccupationStr, LPTSTR pNPCNameStr )
{
	memset( m_NPCOccupationString, 0, sizeof(TCHAR) * _XDEF_MAXNPCNAMESTRINGSIZE );
	memset( m_NPCNameString, 0, sizeof(TCHAR) * _XDEF_MAXNPCNAMESTRINGSIZE );	

	if( pNPCOccupationStr )
	{
		strcpy( m_NPCOccupationString, pNPCOccupationStr );		
	}
	if( pNPCNameStr )
	{
		strcpy( m_NPCNameString, pNPCNameStr );
	}
}

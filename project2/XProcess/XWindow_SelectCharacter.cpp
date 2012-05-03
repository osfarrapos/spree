// XWindow_SelectCharacter.cpp: implementation of the _XWindow_SelectCharacter class.
//
// Created Date		: 2003.05.06
// Author			: Sohyun, Park (kukuri)
// Desc				: Character Zoom Window - Character Create
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "emperorofdragons.h"
#include "XWindow_SelectCharacter.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

RECT g_CharacterZoneNameImageRect[ 9 ] = 
{
	{ 2, 2, 55, 27 },
	{ 55, 2, 108, 27 },
	{ 117, 2, 169, 27 },
	{ 179, 2, 248, 27 },
	{ 2, 33, 73, 58 },
	{ 83, 33, 154, 58 },
	{ 162, 33, 212, 58 },
};


extern void _XDrawEdgedSolidBar ( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color );
extern void _XDrawSolidBar (FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color);
extern void	_XDrawLine2D( FLOAT px1, FLOAT py1, FLOAT px2, FLOAT py2, D3DCOLOR color1, D3DCOLOR color2 );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_SelectCharacter::_XWindow_SelectCharacter()
{
	memset(m_Name, 0, sizeof(TCHAR)*128);
	memset(m_Level, 0, sizeof(TCHAR)*128);
	memset(m_SubLevel, 0, sizeof(TCHAR)*128);
	memset(m_RollClass, 0, sizeof(TCHAR)*128);
	memset(m_CharacterLocation, 0, sizeof(TCHAR)*128);

	m_constitution	= 0;
	m_zen			= 0;
	m_intelligence	= 0;
	m_dexterity		= 0;
	m_strength		= 0;
	m_SelectedCharacter = 0;

	m_pMinimapTexture[0] = m_pMinimapTexture[1] = m_pMinimapTexture[2] = NULL;
	m_MinimapAlphaLevel[0] = m_MinimapAlphaLevel[1] = m_MinimapAlphaLevel[2] = 0;
	m_CharacterGMLevel = 0;
}

_XWindow_SelectCharacter::~_XWindow_SelectCharacter()
{
	SAFE_RELEASE( m_pMinimapTexture[0] );
	SAFE_RELEASE( m_pMinimapTexture[1] );
	SAFE_RELEASE( m_pMinimapTexture[2] );
}

BOOL _XWindow_SelectCharacter::Initialize(_XTextureManager& texturearchive)
{
	_XWindow::SetDrawBorder( FALSE );

	// XProc_Selectcharacter InitializeProcess()에서 미리 텍스쳐를 쓰겠다고 등록해 놔야 쓸 수 있음
	int btnimageindex = texturearchive.FindResource("MI_Login_00.tga");
	int edgeimageindex = texturearchive.FindResource("MI_Login_01.tga");
	int zonenameindex = texturearchive.FindResource("MI_Login_03.tga");
	
	_XImageStatic* pImageStatic = new _XImageStatic;
	//m_GroupTitle_Left
	pImageStatic->Create( -2, 0, 97, 43, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 33, 119, 133, 162 );
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;
	//m_GroupTitle_Right
	pImageStatic->Create( 198, 0, 298, 43, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 33, 119, 133, 162 );
	pImageStatic->SetScale( -1.0f, 1.0f );
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;
	//m_InfoTitle_Left
	pImageStatic->Create( 0, 51, 98, 51+24, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 150, 117, 248, 141 );
	InsertChildObject( pImageStatic );
	
	pImageStatic = new _XImageStatic;
	//m_InfoTitle_Right
	pImageStatic->Create( 196, 51, 294, 51+24, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 150, 117, 248, 141 );
	pImageStatic->SetScale( -1.0f, 1.0f );
	InsertChildObject( pImageStatic );
	
	m_MenuEdge_Top.Create( gnWidth - (1024 - 885), gnHeight - (768 - 641), gnWidth - (1024 - 997), gnHeight - (768 - 642), &texturearchive, edgeimageindex );
	m_MenuEdge_Top.SetClipRect( 3, 7, 115, 8 );
	
	m_MenuEdge_Bottom.Create( gnWidth - (1024 - 885), gnHeight - (768 - 748), gnWidth - (1024 - 997), gnHeight - (768 - 749), &texturearchive, edgeimageindex );
	m_MenuEdge_Bottom.SetClipRect( 3, 11, 115, 12 );
	
	m_MenuEdge_Left.Create( gnWidth - (1024 - 884), gnHeight - (768 - 642), gnWidth - (1024 - 885), gnHeight - (768 - 748), &texturearchive, edgeimageindex );
	m_MenuEdge_Left.SetClipRect( 244, 0, 245, 106 );
	
	m_MenuEdge_Right.Create( gnWidth - (1024 - 997), gnHeight - (768 - 642), gnWidth - (1024 - 998), gnHeight - (768 - 748), &texturearchive, edgeimageindex );
	m_MenuEdge_Right.SetClipRect( 247, 0, 248, 106 );
	
	pImageStatic = new _XImageStatic;
	//m_InfoEdge_Top
	pImageStatic->Create( 28, 137, 170, 138, &texturearchive, edgeimageindex );
	pImageStatic->SetClipRect( 0, 0, 142, 1 );
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;
	//m_InfoEdge_Bottom
	pImageStatic->Create( 28, 259, 170, 260, &texturearchive, edgeimageindex );
	pImageStatic->SetClipRect( 0, 2, 142, 3 );
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;
	//m_InfoEdge_Left
	pImageStatic->Create( 27, 138, 28, 259, &texturearchive, edgeimageindex );
	pImageStatic->SetClipRect( 252, 0, 253, 121 );
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;
	//m_InfoEdge_Right
	pImageStatic->Create( 170, 138, 171, 259, &texturearchive, edgeimageindex );
	pImageStatic->SetClipRect( 255, 0, 256, 121 );
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;
	//m_InfoBarEdge_Bottom
	pImageStatic->Create( 0, 267, 196, 271, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 0, 252, 196, 256 );
	InsertChildObject( pImageStatic );
	
	pImageStatic = new _XImageStatic;
	//m_InfoBarEdge_Left
	pImageStatic->Create( 0, 74, 3, 267, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 254, 0, 256, 193 );
	InsertChildObject( pImageStatic );
	
	pImageStatic = new _XImageStatic;
	//m_InfoBarEdge_Right
	pImageStatic->Create( 195, 74, 198, 267, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 254, 0, 256, 193 );
	pImageStatic->SetScale( -1.0f, 1.0f );
	InsertChildObject( pImageStatic );
	
	pImageStatic = new _XImageStatic;
	//m_ClassBorder
	pImageStatic->Create( 44, 85, 153, 106, &texturearchive, edgeimageindex );
	pImageStatic->SetClipRect( 4, 150, 113, 172 );
	if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pImageStatic->ShowWindow( FALSE );
	}
	InsertChildObject( pImageStatic );
	
	pImageStatic = new _XImageStatic;
	//m_LevelBorder
	pImageStatic->Create( 44, 108, 153, 129, &texturearchive, edgeimageindex );
	pImageStatic->SetClipRect( 4, 150, 113, 172 );
	if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pImageStatic->ShowWindow( FALSE );
	}
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;
	//m_MinimapTitle_Left
	pImageStatic->Create( 0, 284, 98, 284+24, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 150, 117, 248, 141 );
	InsertChildObject( pImageStatic );
	
	pImageStatic = new _XImageStatic;
	//m_MinimapTitle_Right
	pImageStatic->Create( 196, 284, 294, 284+24, &texturearchive, btnimageindex );
	pImageStatic->SetClipRect( 150, 117, 248, 141 );
	pImageStatic->SetScale( -1.0f, 1.0f );
	InsertChildObject( pImageStatic );

	m_pMinimapImageStatic = new _XImageStatic;
	
	m_pMinimapImageStatic->Create( 2, 309, 194, 309+192, &texturearchive, -1 );
	m_pMinimapImageStatic->SetClipRect( 0, 0, 256, 256 );
	m_pMinimapImageStatic->SetScale( 192.0f / 256.0f, 192.0f / 256.0f );
	InsertChildObject( m_pMinimapImageStatic );

	m_SpawnPointStatic.Create( 0, 0, 19, 19, &texturearchive, btnimageindex );
	m_SpawnPointStatic.SetClipRect( 0, 232, 19, 251 );	
	
	m_pCharacterGroupNameImageStatic = new _XImageStatic;	
	m_pCharacterGroupNameImageStatic->Create( 0, 0, 1, 1, &texturearchive, zonenameindex );
	RebuildCharacterGroupNameStatic( _XGROUP_NANGIN );
	InsertChildObject( m_pCharacterGroupNameImageStatic );

	_XBTN_STRUCTURE startgamebtn = 
	{
		TRUE, 
		{ gnWidth - (1024 - 886), gnHeight - (768-643) }, {110, 25},
		_XDEF_CS_CONNECTBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};	
	m_StartGameButton.Create(startgamebtn);
	m_StartGameButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_StartGameButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_StartGameButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_StartGameButton.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_START ); // 구룡쟁패 시작
	m_StartGameButton.SetGrowTextMode( TRUE );
	
	_XBTN_STRUCTURE createcharacterbtn = 
	{
		TRUE, 
		{ gnWidth - (1024 - 886), gnHeight - (768-669) }, {110, 25},
		_XDEF_CS_CREATEBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};	
	m_CreateCharacterButton.Create(createcharacterbtn);
	m_CreateCharacterButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_CreateCharacterButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_CreateCharacterButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_CreateCharacterButton.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_CREATE );	// _T("인물생성")
	m_CreateCharacterButton.SetGrowTextMode( TRUE );
	
	_XBTN_STRUCTURE deletecharacterbtn = 
	{
		TRUE, 
		{ gnWidth - (1024 - 886), gnHeight - (768-695) }, {110, 25},
		_XDEF_CS_DELETEBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};	
	m_DeleteCharacterButton.Create(deletecharacterbtn);
	m_DeleteCharacterButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_DeleteCharacterButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_DeleteCharacterButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_DeleteCharacterButton.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_DELETE );	// _T("인물삭제")
	m_DeleteCharacterButton.SetGrowTextMode( TRUE );

	_XBTN_STRUCTURE exitbtn = 
	{
		TRUE, 
		{ gnWidth - (1024 - 886), gnHeight - (768-721) }, {110, 25},
		_XDEF_CS_CANCELBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};
	m_ExitButton.Create(exitbtn);
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_ExitButton.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_EXIT );	// _T("종료")
	m_ExitButton.SetGrowTextMode( TRUE );
	
	_XBTN_STRUCTURE blank01btn = 
	{
		TRUE, 
		{ gnWidth - (1024 - 897), gnHeight - (768-457) }, {226, 41},
		_XDEF_CS_CHARACTER01BUTTON,
		edgeimageindex,
		edgeimageindex,
		edgeimageindex,
		&texturearchive
	};	
	m_Blank01Button.Create(blank01btn);
	m_Blank01Button.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 30, 215, 256, 256 );
	m_Blank01Button.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  30, 215, 256, 256 );
	m_Blank01Button.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  30, 215, 256, 256 );
	m_Blank01Button.SetGrowTextMode( TRUE, 0xFF303030 );
	m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
		
	_XBTN_STRUCTURE blank02btn = 
	{
		TRUE, 
		{ gnWidth - (1024 - 897), gnHeight - (768-497) }, {226, 41},
		_XDEF_CS_CHARACTER02BUTTON,
		edgeimageindex,
		edgeimageindex,
		edgeimageindex,
		&texturearchive
	};	
	m_Blank02Button.Create(blank02btn);
	m_Blank02Button.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 30, 215, 256, 256 );
	m_Blank02Button.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  30, 215, 256, 256 );
	m_Blank02Button.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  30, 215, 256, 256 );
	m_Blank02Button.SetGrowTextMode( TRUE, 0xFF303030 );
	m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
		
	_XBTN_STRUCTURE blank03btn = 
	{
		TRUE, 
			{ gnWidth - (1024 - 897), gnHeight - (768-538) }, {226, 41},
		_XDEF_CS_CHARACTER03BUTTON,
		edgeimageindex,
		edgeimageindex,
		edgeimageindex,
		&texturearchive
	};	
	m_Blank03Button.Create(blank03btn);
	m_Blank03Button.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 30, 215, 256, 256 );
	m_Blank03Button.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  30, 215, 256, 256 );
	m_Blank03Button.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  30, 215, 256, 256 );
	m_Blank03Button.SetGrowTextMode( TRUE, 0xFF303030 );
	m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );
	
	SetWindowMoveMode(_XWMOVE_FIXED);

	if( g_pLocalUser->m_CharacterCount >= 1 )
		m_Blank01Button.SetButtonText( g_pLocalUser->m_AllCharacterInfo[0].charactername, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	
	else
		m_Blank01Button.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_CHARACTER1, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	// _T("인물 1")

	if( g_pLocalUser->m_CharacterCount >= 2 )
		m_Blank02Button.SetButtonText( g_pLocalUser->m_AllCharacterInfo[1].charactername, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	
	else
		m_Blank02Button.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_CHARACTER2, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	// _T("인물 2")

	if( g_pLocalUser->m_CharacterCount >= 3 )
		m_Blank03Button.SetButtonText( g_pLocalUser->m_AllCharacterInfo[2].charactername, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	
	else
		m_Blank03Button.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_CHARACTER3, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	// _T("인물 3")

	m_DrawCharacterInfo = FALSE;
	m_SelectedCharacter = 0;

	m_MinimapAlphaLevel[0] = m_MinimapAlphaLevel[1] = m_MinimapAlphaLevel[2] = 0;
	
	g_LodTerrain.m_GenerateRealTimeObject = FALSE;

	m_CharacterGMLevel = 0;

	return TRUE;
}

void _XWindow_SelectCharacter::DestroyWindow(void)
{
	SAFE_RELEASE( m_pMinimapTexture[0] );
	SAFE_RELEASE( m_pMinimapTexture[1] );
	SAFE_RELEASE( m_pMinimapTexture[2] );
}

void _XWindow_SelectCharacter::Draw(_XGUIObject*& pfocusobject)
{
	if( m_DrawCharacterInfo )
	{
		if(this->m_ShowWindow)
			_XDrawSolidBar( m_WindowPosition.x+1, m_WindowPosition.y+53, m_WindowPosition.x+194, m_WindowPosition.y+267, 0x80000000 );

		_XWindow::Draw(pfocusobject);
	}

	if(!this->m_ShowWindow)
		return;

	m_StartGameButton.Draw( pfocusobject );
	m_CreateCharacterButton.Draw( pfocusobject );
	m_DeleteCharacterButton.Draw( pfocusobject );
	m_ExitButton.Draw( pfocusobject );

	m_MenuEdge_Top.Draw();
	m_MenuEdge_Bottom.Draw();
	m_MenuEdge_Left.Draw();
	m_MenuEdge_Right.Draw();
		
	if( m_DrawCharacterInfo )
	{
		_XDrawEdgedSolidBar( m_WindowPosition.x+1, m_WindowPosition.y+308, m_WindowPosition.x+194, m_WindowPosition.y+308+193, 0x80000000 );

		const alphaanispeed = 10;
		for( int i = 0; i < 3; i++ )
		{
			if( i == m_SelectedCharacter )
			{
				if( m_MinimapAlphaLevel[i] < 255 )
				{
					m_MinimapAlphaLevel[i] += alphaanispeed;
					if( m_MinimapAlphaLevel[i] > 255 )
					{
						m_MinimapAlphaLevel[i] = 255;
					}
				}
			}
			else
			{
				if( m_MinimapAlphaLevel[i] > 0 )
				{
					m_MinimapAlphaLevel[i] -= alphaanispeed;
					if( m_MinimapAlphaLevel[i] < 0 )
					{
						m_MinimapAlphaLevel[i] = 0;
					}
				}
			}		

			if( m_MinimapAlphaLevel[i] > 0 )
			{
				m_pMinimapImageStatic->m_FColor = D3DCOLOR_ARGB( m_MinimapAlphaLevel[i], 0xFF,0xFF,0xFF );
				m_pMinimapImageStatic->DrawWithRegion( m_pMinimapTexture[i] );
			
				int mapsize = g_ZoneInfoTable[g_pLocalUser->m_AllCharacterInfo[i].zoneuniqid-1].mapsize;

				if( mapsize != 0 )
				{
					FLOAT xpos = m_WindowPosition.x +   2 + (( (FLOAT)(g_pLocalUser->m_AllCharacterInfo[i].lastposition.x + (mapsize/2)) / (FLOAT)mapsize ) * 192.0f);
					FLOAT ypos = m_WindowPosition.y + 309 + 192.0f - (( (FLOAT)(g_pLocalUser->m_AllCharacterInfo[i].lastposition.y + (mapsize/2)) / (FLOAT)mapsize ) * 192.0f);

					/*
					_XDrawLine2D( xpos, m_WindowPosition.y + 309, xpos, ypos, D3DCOLOR_ARGB( m_MinimapAlphaLevel[i], 0xFF,0xFF,0xFF ), D3DCOLOR_ARGB( 0x00, 0x00,0x00,0x00 ) );
					_XDrawLine2D( xpos, ypos, xpos, m_WindowPosition.y + 308+193, D3DCOLOR_ARGB( 0x00, 0x00,0x00,0x00 ), D3DCOLOR_ARGB( m_MinimapAlphaLevel[i], 0xFF,0xFF,0xFF ) );

					_XDrawLine2D( m_WindowPosition.x + 2, ypos, xpos, ypos,  D3DCOLOR_ARGB( m_MinimapAlphaLevel[i], 0xFF,0xFF,0xFF ), D3DCOLOR_ARGB( 0x00, 0x00,0x00,0x00 ) );
					_XDrawLine2D( xpos, ypos, m_WindowPosition.x + 194, ypos, D3DCOLOR_ARGB( 0x00, 0x00,0x00,0x00 ), D3DCOLOR_ARGB( m_MinimapAlphaLevel[i], 0xFF,0xFF,0xFF ) );
					*/

					m_SpawnPointStatic.Draw( xpos - 9, ypos - 8 );
				}
			}
		}
	}

	m_Blank01Button.Draw(pfocusobject);
	m_Blank02Button.Draw(pfocusobject);
	m_Blank03Button.Draw(pfocusobject);

	if(m_DrawCharacterInfo)
	{
		DrawCharacterInfo();
	}
}

BOOL _XWindow_SelectCharacter::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if(g_pLocalUser->m_CharacterCount >= 3)
		m_CreateCharacterButton.EnableWindow(FALSE);
	else
		m_CreateCharacterButton.EnableWindow(TRUE);

	if(g_pLocalUser->m_CharacterCount <= 0)
	{
		m_DeleteCharacterButton.EnableWindow(FALSE);
		m_StartGameButton.EnableWindow(FALSE);
	}
	else
	{
		m_DeleteCharacterButton.EnableWindow(TRUE);
		m_StartGameButton.EnableWindow(TRUE);
	}

	m_StartGameButton.Process( pfocusobject );		
	m_CreateCharacterButton.Process( pfocusobject );		
	m_DeleteCharacterButton.Process( pfocusobject );		
	m_ExitButton.Process( pfocusobject );
	
	m_Blank01Button.Process( pfocusobject );
	m_Blank02Button.Process( pfocusobject );
	m_Blank03Button.Process( pfocusobject );

	if( m_DrawCharacterInfo )
	{
		if(!_XWindow::Process(pfocusobject))
			return FALSE;
	}

	return TRUE;
}

void _XWindow_SelectCharacter::DrawCharacterInfo(void)
{
	// 이름	
	if( m_CharacterGMLevel >= 2 )
	{
		g_XBaseFont->SetColor(0xFFFF3FFF);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 98, m_WindowPosition.y + 56, 1.0f, _XFONT_ALIGNTYPE_CENTER, "GM[ %s ]",m_Name );
		g_XBaseFont->Flush();
		g_XBaseFont->SetColor(0xFFFFFFFF);
	}
	else
	{		
#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
		if( g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].ucChangeNameFlag )
			g_XBaseFont->SetColor(0xFFFCF904);
		else
			g_XBaseFont->SetColor(0xFFFFFFFF);
#else
		g_XBaseFont->SetColor(0xFFFFFFFF);
#endif
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 98, m_WindowPosition.y + 56, _XFONT_ALIGNTYPE_CENTER, m_Name );
	}
			
	g_XBaseFont->SetColor(0xFFD5D5D5);
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 98, m_WindowPosition.y + 291, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_CURRENTZONE), m_CharacterLocation );	// _T("현 위치 : %s")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->SetColor(0xFFBEB86E);
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 98, m_WindowPosition.y + 84, _XFONT_ALIGNTYPE_CENTER, m_RollClass );
		
		// 문파
		//g_XBaseFont->PrintAlign(m_WindowPosition.x + 145, m_WindowPosition.y + 444, 1.0f, _XFONT_ALIGNTYPE_CENTER, m_JoinGroup );
		
		// 레벨
		g_XBaseFont->SetColor(0xFFCACBD0);
		g_XBaseFont->PutsAlign(m_WindowPosition.x + 98, m_WindowPosition.y + 100, _XFONT_ALIGNTYPE_CENTER, m_Level);

		g_XBaseFont->PutsAlign(m_WindowPosition.x + 98, m_WindowPosition.y + 116, _XFONT_ALIGNTYPE_CENTER, m_SubLevel);
	}
	else
	{
		g_XBaseFont->SetColor(0xFFBEB86E);
		g_XBaseFont->PutsAlign( m_WindowPosition.x + 98, m_WindowPosition.y + 91, _XFONT_ALIGNTYPE_CENTER, m_RollClass );
		
		// 문파
		//g_XBaseFont->PrintAlign(m_WindowPosition.x + 145, m_WindowPosition.y + 444, 1.0f, _XFONT_ALIGNTYPE_CENTER, m_JoinGroup );

		// 레벨
		g_XBaseFont->SetColor(0xFFCACBD0);
		g_XBaseFont->PutsAlign(m_WindowPosition.x + 98, m_WindowPosition.y + 114, _XFONT_ALIGNTYPE_CENTER, m_Level);
	}	

	int basepos = 52;
	int basepos2 = 98;
	if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		basepos = 32;
		basepos2 = 120;
	}
	
	g_XBaseFont->SetColor(0xFFBEBBB6);
	g_XBaseFont->Puts(m_WindowPosition.x + basepos, m_WindowPosition.y + 147,  _XGETINTERFACETEXT(ID_STRING_CINFO_STR), 1.0f );	// "근력"
	g_XBaseFont->Puts(m_WindowPosition.x + basepos, m_WindowPosition.y + 170,  _XGETINTERFACETEXT(ID_STRING_CINFO_INNERENERGY), 1.0f );	// "진기"
	g_XBaseFont->Puts(m_WindowPosition.x + basepos, m_WindowPosition.y + 193,  _XGETINTERFACETEXT(ID_STRING_CINFO_HEART), 1.0f );	// "지혜"
	g_XBaseFont->Puts(m_WindowPosition.x + basepos, m_WindowPosition.y + 216,  _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTHENERGY), 1.0f );	// "건강"
	g_XBaseFont->Puts(m_WindowPosition.x + basepos, m_WindowPosition.y + 239,  _XGETINTERFACETEXT(ID_STRING_CINFO_DEX), 1.0f );	// "민첩"

	g_XBaseFont->SetColor(0xFFB9B183);
	g_XBaseFont->Print(m_WindowPosition.x + basepos2, m_WindowPosition.y + 147,  1.0f, "%d",m_strength);
	g_XBaseFont->Print(m_WindowPosition.x + basepos2, m_WindowPosition.y + 170,  1.0f, "%d",m_zen);
	g_XBaseFont->Print(m_WindowPosition.x + basepos2, m_WindowPosition.y + 193,  1.0f, "%d",m_intelligence);
	g_XBaseFont->Print(m_WindowPosition.x + basepos2, m_WindowPosition.y + 216,  1.0f, "%d",m_constitution);
	g_XBaseFont->Print(m_WindowPosition.x + basepos2, m_WindowPosition.y + 239,  1.0f, "%d",m_dexterity);

	g_XBaseFont->Flush();
}

void _XWindow_SelectCharacter::RebuildCharacterGroupNameStatic( int group )
{
	m_pCharacterGroupNameImageStatic->SetClipRect( g_CharacterZoneNameImageRect[group] );		
	m_pCharacterGroupNameImageStatic->MoveWindow( m_WindowPosition.x + 98 - ((g_CharacterZoneNameImageRect[group].right-g_CharacterZoneNameImageRect[group].left) >> 1), 
												  m_WindowPosition.y + 20 - ((g_CharacterZoneNameImageRect[group].bottom-g_CharacterZoneNameImageRect[group].top) >> 1) );
}

BOOL _XWindow_SelectCharacter::LoadMinimapTexture( int index, LPTSTR zonefilenmae )
{
	SAFE_RELEASE( m_pMinimapTexture[index] );

	TCHAR minimapnamestr[_MAX_PATH];
	memset( minimapnamestr, 0, _MAX_PATH );
	
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{			
		strcpy( minimapnamestr, "Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( minimapnamestr, "vn_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		strcpy( minimapnamestr, "us_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( minimapnamestr, "tw_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strcpy( minimapnamestr, "jp_Mini" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		strcpy( minimapnamestr, "rs_Mini" );
	}
	else
	{
		strcpy( minimapnamestr, "Mini" );
	}		

	strcat( minimapnamestr, zonefilenmae );
	
	LPTSTR cutpoint = strstr( minimapnamestr, "xtv" );
	if( cutpoint )
	{
		*cutpoint = NULL;
	}
	
	strcat( minimapnamestr, "tga");
	
	//_XLog("ci index : %d, %s", index,  minimapnamestr );
		
	_XPackageArchive TextureArchive;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	strcat( packagefilename, _XDEF_TEXTUREFILE_A );

	if( !TextureArchive.OpenPackage( packagefilename ) ) return FALSE;	
	
	int resourceindex = TextureArchive.FindResource( minimapnamestr );
	if( resourceindex < 0 )
	{
		strcpy( minimapnamestr, "Mini" );
		strcat( minimapnamestr, zonefilenmae );
		
		LPTSTR cutpoint = strstr( minimapnamestr, "xtv" );
		if( cutpoint )
		{
			*cutpoint = NULL;
		}
		
		resourceindex = TextureArchive.FindResource( minimapnamestr );		
		strcat( minimapnamestr, "tga");

		if( resourceindex < 0 )
		{
			_XFatalError( "Resource not found in the texture archive [%s]", minimapnamestr);
			return FALSE;
		}
	}
	
	int   filesize = TextureArchive.GetPackedFileSize(resourceindex);
	FILE* pFP = TextureArchive.GetPackedFile(resourceindex);
	
	LPBYTE pImageVFMemory = NULL;
	pImageVFMemory = new BYTE[ filesize ];
	if( !pImageVFMemory )
	{
		_XFatalError( "Can't allocate memory for virtual file" );
		TextureArchive.FinalizePackage();
		return FALSE;
	}
	
	if( fread( pImageVFMemory, filesize, 1, pFP ) < 1 )
	{
		_XFatalError( "Can't read virtual file data" );
		delete[] pImageVFMemory;
		TextureArchive.FinalizePackage();
		return FALSE;
	}
	
	TextureArchive.FinalizePackage();
	
	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
		256, 256, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
		D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		0, NULL, NULL, &m_pMinimapTexture[index] ) ) )
	{
		_XFatalError( "Can't create texture from memory" );
		delete[] pImageVFMemory;		
		return FALSE;
	}
	
	delete[] pImageVFMemory;
		
	return TRUE;
}
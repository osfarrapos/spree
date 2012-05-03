// XWindow_TargetInfo.cpp: implementation of the _XWindow_TargetInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_TargetInfo.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// 해외 버젼일 때 

#ifdef _XVIETNAMESE
	POINT g_TargetInfoItemCoord[10] = 
	{
		{  80,233 },	// 상의		0
		{  80,266 },	// 하의		1
		{  115,299 },	// 아대		2
		{  80,299 },	// 신발		3
		{  98,200 },	// 모자		4
		{  115,233 },	// 갑주 하	5
		{  115,266 },	// 갑주 상	6
		{  -25,100   },	// 배낭		7
		
		{  42,217  },	// 무기 1	8
		{  42,253  },	// 무기 2	9
	};
#else
	#if defined(_XENGLISH) || defined(_XRUSSIAN)
		POINT g_TargetInfoItemCoord[10] = 
		{
			{  80,233 },	// 상의		0
			{  80,266 },	// 하의		1
			{  115,299 },	// 아대		2
			{  80,299 },	// 신발		3
			{  98,200 },	// 모자		4
			{  115,233 },	// 갑주 하	5
			{  115,266 },	// 갑주 상	6
			{  -25,100   },	// 배낭		7
			
			{  42,217  },	// 무기 1	8
			{  42,253  },	// 무기 2	9
		};
	#else
		POINT g_TargetInfoItemCoord[10] = 
		{
			{  105,133 },	// 상의		0
			{  105,166 },	// 하의		1
			{  140,199 },	// 아대		2
			{  105,199 },	// 신발		3
			{  123,100  },	// 모자		4
			{  140,133 },	// 갑주 하	5
			{  140,166 },	// 갑주 상	6
			{  0,0 },	// 배낭		7
			
			{  67,117  },	// 무기 1	8
			{  67,153 },	// 무기 2	9
		};
	#endif
#endif


_XWindow_TargetInfo::_XWindow_TargetInfo()
{
	m_SelectedSlotIndex = -1;
	memset(m_SendName, 0, sizeof(TCHAR)*13);
	memset(m_SendTempName, 0, sizeof(TCHAR)*13);	
	m_PtoPLevelDef = 0;
	m_SelectedTargetInfo.reset();
}

_XWindow_TargetInfo::~_XWindow_TargetInfo()
{

}

BOOL _XWindow_TargetInfo::Initialize( void )
{
	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int questwinimage = g_MainInterfaceTextureArchive.FindResource( "MI_NPCQuestwin_01.tga" );
	int mainframeinterface = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
	int mainborderimage = g_MainInterfaceTextureArchive.FindResource("MI_System.tga");

	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 90, 23,
						   &g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleLeftBar->SetClipRect( 0, 0, 91, 24 );
	InsertChildObject( pTitleLeftBar );

	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create( 180, 0, 180, 23,
							&g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleRightBar->SetClipRect( 0, 0, 91, 24 );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );	
	
	// 해외 버젼일 때 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		_XImageStatic* pLeftLIne = new _XImageStatic;
		pLeftLIne->Create( 1, 24, 2, 342, &g_MainInterfaceTextureArchive, mainborderimage );
		pLeftLIne->SetClipRect( 254, 0, 255, 100 );
		pLeftLIne->SetScale( 1.0f, 3.18f);
		InsertChildObject( pLeftLIne );
		
		_XImageStatic* pRightLIne = new _XImageStatic;
		pRightLIne->Create( 178, 24, 179, 342, &g_MainInterfaceTextureArchive, mainborderimage );
		pRightLIne->SetClipRect( 254, 0, 255, 100 );
		pRightLIne->SetScale( 1.0f, 3.18f);
		InsertChildObject( pRightLIne );
		
		_XImageStatic* pBottomLIne = new _XImageStatic;
		pBottomLIne->Create( 2, 341, 178, 342,
			&g_MainInterfaceTextureArchive, mainborderimage );
		pBottomLIne->SetClipRect( 2, 209, 178, 210 );
		InsertChildObject( pBottomLIne );		

	}
	else
	{
		_XImageStatic* pLeftLIne = new _XImageStatic;
		pLeftLIne->Create( 1, 24, 2, 242,
			&g_MainInterfaceTextureArchive, mainborderimage );
		pLeftLIne->SetClipRect( 254, 0, 255, 218 );
		InsertChildObject( pLeftLIne );
		
		_XImageStatic* pRightLIne = new _XImageStatic;
		pRightLIne->Create( 178, 24, 179, 242,
			&g_MainInterfaceTextureArchive, mainborderimage );
		pRightLIne->SetClipRect( 254, 0, 255, 218 );
		InsertChildObject( pRightLIne );

		_XImageStatic* pBottomLIne = new _XImageStatic;
		pBottomLIne->Create( 2, 241, 178, 242,
			&g_MainInterfaceTextureArchive, mainborderimage );
		pBottomLIne->SetClipRect( 2, 209, 178, 210 );
		InsertChildObject( pBottomLIne );		
	}

	
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 162, 7 }, { 12, 12 }, _XDEF_TARGETINFOWINDOW_CLOSEBUTTON,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
		
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 149, 7 }, { 12, 12 }, _XDEF_TARGETINFOWINDOW_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );

	_XImageStatic* pCharacterNameBorder = new _XImageStatic;	
	pCharacterNameBorder->Create( 9, 32, 170, 53, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pCharacterNameBorder->SetClipRect( 0, 0, 162, 22 );
	InsertChildObject( pCharacterNameBorder );

	_XImageStatic* pCharacterNameTextureBorder = new _XImageStatic;	
	pCharacterNameTextureBorder->Create( 11, 34, 168, 51, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	pCharacterNameTextureBorder->SetClipRect( 0, 23, 158, 41 );
	InsertChildObject( pCharacterNameTextureBorder );
	
	// 해외 버젼일 때 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		m_BorderTile.Create( m_WindowPosition.x+1, m_WindowPosition.y, m_WindowPosition.x+179, m_WindowPosition.y+238 );
		m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
		m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
		
		FLOAT ufactor = 178.0f/128.0f;
		FLOAT vfactor = 320.0f/128.0f;
		
		m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );

		_XImageStatic* pEdge_Top = new _XImageStatic;
		pEdge_Top->Create( 11, 107, 61, 108, &g_MainInterfaceTextureArchive, questwinimage );
		pEdge_Top->SetClipRect( 24, 492, 74, 493 );
		pEdge_Top->SetScale( 3.2f, 1.0f );
		InsertChildObject( pEdge_Top );
		
		_XImageStatic* pEdge_Bottom = new _XImageStatic;
		pEdge_Bottom->Create( 11, 193, 61, 194, &g_MainInterfaceTextureArchive, questwinimage );
		pEdge_Bottom->SetClipRect( 24, 500, 74, 501 );
		pEdge_Bottom->SetScale( 3.2f, 1.0f );
		InsertChildObject( pEdge_Bottom );
		
		_XImageStatic* pEdge_Left = new _XImageStatic;
		pEdge_Left->Create( 10, 108, 11, 193 , &g_MainInterfaceTextureArchive, questwinimage );
		pEdge_Left->SetClipRect( 497, 283, 498, 368 );
		InsertChildObject( pEdge_Left );
		
		_XImageStatic* pEdge_Right = new _XImageStatic;
		pEdge_Right->Create( 171, 108, 172, 193, &g_MainInterfaceTextureArchive, questwinimage );
		pEdge_Right->SetClipRect( 501, 283, 502, 368 );
		InsertChildObject( pEdge_Right );
		
	}
	else// 국내 버젼일 때 
	{
		m_BorderTile.Create( m_WindowPosition.x+1, m_WindowPosition.y, m_WindowPosition.x+179, m_WindowPosition.y+238 );
		m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
		m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
		
		FLOAT ufactor = 178.0f/128.0f;
		FLOAT vfactor = 220.0f/128.0f;
		
		m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );

		_XImageStatic* pEdge_Top = new _XImageStatic;
		pEdge_Top->Create( 11, 107, 61, 108, &g_MainInterfaceTextureArchive, questwinimage );
		pEdge_Top->SetClipRect( 24, 492, 74, 493 );
		InsertChildObject( pEdge_Top );
		
		_XImageStatic* pEdge_Bottom = new _XImageStatic;
		pEdge_Bottom->Create( 11, 193, 61, 194, &g_MainInterfaceTextureArchive, questwinimage );
		pEdge_Bottom->SetClipRect( 24, 500, 74, 501 );
		InsertChildObject( pEdge_Bottom );
		
		_XImageStatic* pEdge_Left = new _XImageStatic;
		pEdge_Left->Create( 10, 108, 11, 193 , &g_MainInterfaceTextureArchive, questwinimage );
		pEdge_Left->SetClipRect( 497, 283, 498, 368 );
		InsertChildObject( pEdge_Left );
		
		_XImageStatic* pEdge_Right = new _XImageStatic;
		pEdge_Right->Create( 61, 108, 62, 193, &g_MainInterfaceTextureArchive, questwinimage );
		pEdge_Right->SetClipRect( 501, 283, 502, 368 );
		InsertChildObject( pEdge_Right );
		
	}
	
	_XImageStatic* pEquipedItemIndicateImage = NULL;
	
	int equippointindex[7] = { 0, 1, 2, 3, 4, 8, 9 };
	POINT equipedimageuv[7] = 
	{
		{ 56,24 },	// 상의
		{ 0, 52 },  // 하의
		{ 56,52 },  // 아대
		{ 28,52 },  // 신발
		{ 28,24 },  // 모자
		{ 0, 24 },  // 무기 1
		{ 0, 24 },  // 무기 2		
	};
	
	int mainimagebackindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	
	for( int i = 0; i < 7; i++ )
	{
		pEquipedItemIndicateImage = new _XImageStatic;
		pEquipedItemIndicateImage->Create( g_TargetInfoItemCoord[equippointindex[i]].x, g_TargetInfoItemCoord[equippointindex[i]].y, 
			g_TargetInfoItemCoord[equippointindex[i]].x+28, g_TargetInfoItemCoord[equippointindex[i]].y+28, 
			&g_MainInterfaceTextureArchive, mainimagebackindex );
		pEquipedItemIndicateImage->SetClipRect( equipedimageuv[i].x, equipedimageuv[i].y,
			equipedimageuv[i].x+28, equipedimageuv[i].y+28 );
		
		InsertChildObject( pEquipedItemIndicateImage );
	}
	
	m_EquipedItemEdgeIcon[0].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_EquipedItemEdgeIcon[0].SetClipRect( 191, 0, 223, 32 );
	
	m_EquipedItemEdgeIcon[1].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_EquipedItemEdgeIcon[1].SetClipRect( 191, 33, 223, 65 );
	
	m_EquipedItemEdgeIcon[2].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_EquipedItemEdgeIcon[2].SetClipRect( 224, 33, 256, 65 );
	
	m_EquipedItemEdgeIcon[3].Create( 0, 0, 32, 32, &g_MainInterfaceTextureArchive, characterwindowimageindex );
	m_EquipedItemEdgeIcon[3].SetClipRect( 224, 0, 256, 32 );
	
	m_EquipedItemEdgeIcon[4].Create( 0, 0, 30, 30, &g_MainInterfaceTextureArchive, mainimagebackindex );
	m_EquipedItemEdgeIcon[4].SetClipRect( 216, 0, 246, 30 );

	return TRUE;
}

void _XWindow_TargetInfo::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_TargetInfo::ShowWindow(BOOL show)
{
	if(show)
	{
		if( g_pDefaultTooltip_Window)
			g_pDefaultTooltip_Window->SetTargetInfoWindowPtr( this );
	}
	else
	{
		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )						
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_TargetInfo::Draw( _XGUIObject*& pfocusobject )
{
	if( this->m_ShowWindow )
	{
		// 해외 버젼일 때 
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{	
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+23, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));
			
			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+30, m_WindowPosition.x+173, m_WindowPosition.y+100, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+29, m_WindowPosition.x+173, m_WindowPosition.y+100, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
		//	_XDrawSolidBar( m_WindowPosition.x+10, m_WindowPosition.y+71, m_WindowPosition.x+170, m_WindowPosition.y+86, 0x7F000000 );
		//	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+71, m_WindowPosition.x+170, m_WindowPosition.y+86, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+104, m_WindowPosition.x+173, m_WindowPosition.y+336, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+103, m_WindowPosition.x+173, m_WindowPosition.y+336, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
			int ypos = 0;
			for( int i = 0; i < 5; i++ ) // 근력 진기 보더....
			{
				_XDrawSolidBar( m_WindowPosition.x+110, m_WindowPosition.y+113 +ypos, m_WindowPosition.x+162, m_WindowPosition.y+127+ypos, 0x7F000000 );			
				ypos += 16;
			}
		}
		else
		{
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+23, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));
			
			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+30, m_WindowPosition.x+173, m_WindowPosition.y+88, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+29, m_WindowPosition.x+173, m_WindowPosition.y+88, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
			_XDrawSolidBar( m_WindowPosition.x+10, m_WindowPosition.y+71, m_WindowPosition.x+170, m_WindowPosition.y+86, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+71, m_WindowPosition.x+170, m_WindowPosition.y+86, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
			_XDrawSolidBar( m_WindowPosition.x+7, m_WindowPosition.y+92, m_WindowPosition.x+173, m_WindowPosition.y+236, 0x7F000000 );
			_XDrawRectAngle( m_WindowPosition.x+6, m_WindowPosition.y+91, m_WindowPosition.x+173, m_WindowPosition.y+236, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
			int ypos = 0;
			for( int i = 0; i < 5; i++ ) // 근력 진기 보더....
			{
				_XDrawSolidBar( m_WindowPosition.x+39, m_WindowPosition.y+111 +ypos, m_WindowPosition.x+58, m_WindowPosition.y+125+ypos, 0x7F000000 );			
				ypos += 16;
			}
		}
	}

	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;	
	
	TCHAR targetRollName[128];
	memset(targetRollName, 0, sizeof(TCHAR)*128);
	TCHAR targetGroupName[128];
	memset(targetGroupName, 0, sizeof(TCHAR)*128);
	TCHAR targetlevelName[128];
	memset(targetlevelName, 0, sizeof(TCHAR)*128);
	TCHAR targetStr[128];
	memset(targetStr, 0, sizeof(TCHAR)*128);
	TCHAR targetZen[128];
	memset(targetZen, 0, sizeof(TCHAR)*128);
	TCHAR targetInt[128];
	memset(targetInt, 0, sizeof(TCHAR)*128);
	TCHAR targetCon[128];
	memset(targetCon, 0, sizeof(TCHAR)*128);
	TCHAR targetDex[128];
	memset(targetDex, 0, sizeof(TCHAR)*128);
	
	TCHAR	titlestring[128];
	memset(titlestring, 0, sizeof(TCHAR)*128);
	strcpy(titlestring, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));	

	BOOL bShowDetailLevel = FALSE;
	
	switch(m_TargetInfoMode) 
	{
	case tm_simple_info :
		{
			strcpy( targetStr, "---" );
			strcpy( targetZen, "---" );
			strcpy( targetInt, "---" );
			strcpy( targetCon, "---" );
			strcpy( targetDex, "---" );

			switch(m_PtoPLevelDef) 
			{
			case 1:
				{
					strcpy( targetRollName, "------" );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					strcpy( targetlevelName, "-------- ----" );
				}
				break;
			case 2:
				{
					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					strcpy( targetlevelName, "-------- ----" );
				}
				break;
			case 3:
				{
					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					int templevel = ( m_SelectedTargetInfo.sInnerLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
					sprintf( targetlevelName, "%s ----", g_LevelName[ templevel ] );
				}
				break;
			case 4:
				{
					bShowDetailLevel = TRUE;

					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(m_SelectedTargetInfo.sInnerLevel);				// 성 ( 1 - 12 )
					int templevel = ( m_SelectedTargetInfo.sInnerLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
#ifdef _XDEF_MODIFY_UI_070710_KUKURI
					sprintf( targetlevelName, "%s %d%s[%d]", g_LevelName[ templevel ], tempstep, titlestring, m_SelectedTargetInfo.sInnerLevel  );
#else
					sprintf( targetlevelName, "%s %d%s", g_LevelName[ templevel ], tempstep, titlestring  );
#endif
				}
				break;
			case 5:
				{
					strcpy( targetRollName, "------" );
					strcpy( m_SelectedTargetInfo.cTargetName, _XGETINTERFACETEXT(ID_STRING_NEW_2659) );	// "복면인(覆面人)"
					strcpy( targetGroupName, "----" );
					strcpy( targetlevelName, "-------- ----" );
				}
				break;
			}
		}
		break;
	case tm_items_info :
		{
			strcpy( targetStr, "---" );
			strcpy( targetZen, "---" );
			strcpy( targetInt, "---" );
			strcpy( targetCon, "---" );
			strcpy( targetDex, "---" );

			switch(m_PtoPLevelDef) 
			{
			case 1:
				{
					strcpy( targetRollName, "------" );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					strcpy( targetlevelName, "-------- ----" );
				}
				break;
			case 2:
				{
					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					strcpy( targetlevelName, "-------- ----" );
				}
				break;
			case 3:
				{
					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					int templevel = ( m_SelectedTargetInfo.sInnerLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
					sprintf( targetlevelName, "%s ----", g_LevelName[ templevel ] );
				}
				break;
			case 4:
				{
					bShowDetailLevel = TRUE;
					
					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(m_SelectedTargetInfo.sInnerLevel);				// 성 ( 1 - 12 )
					int templevel = ( m_SelectedTargetInfo.sInnerLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
#ifdef _XDEF_MODIFY_UI_070710_KUKURI
					sprintf( targetlevelName, "%s %d%s[%d]", g_LevelName[ templevel ], tempstep, titlestring, m_SelectedTargetInfo.sInnerLevel  );
#else
					sprintf( targetlevelName, "%s %d%s", g_LevelName[ templevel ], tempstep, titlestring  );
#endif
				}
				break;
			}
		}
		break;
	case tm_ability_info :
		{
			itoa(m_SelectedTargetInfo.sStr, targetStr, 10 );
			itoa(m_SelectedTargetInfo.sZen, targetZen, 10 );
			itoa(m_SelectedTargetInfo.sIntelligence, targetInt, 10 );
			itoa(m_SelectedTargetInfo.sConstitution, targetCon, 10 );
			itoa(m_SelectedTargetInfo.sDexterity, targetDex, 10 );

			switch(m_PtoPLevelDef) 
			{
			case 1:
				{
					strcpy( targetRollName, "------" );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					strcpy( targetlevelName, "-------- ----" );
				}
				break;
			case 2:
				{
					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					strcpy( targetlevelName, "-------- ----" );
				}
				break;
			case 3:
				{
					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					int templevel = ( m_SelectedTargetInfo.sInnerLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
					sprintf( targetlevelName, "%s ----", g_LevelName[ templevel ] );
				}
				break;
			case 4:
				{
					bShowDetailLevel = TRUE;

					strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
					strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
					int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(m_SelectedTargetInfo.sInnerLevel);				// 성 ( 1 - 12 )
					int templevel = ( m_SelectedTargetInfo.sInnerLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
#ifdef _XDEF_MODIFY_UI_070710_KUKURI
					sprintf( targetlevelName, "%s %d%s[%d]", g_LevelName[ templevel ], tempstep, titlestring, m_SelectedTargetInfo.sInnerLevel  );
#else
					sprintf( targetlevelName, "%s %d%s", g_LevelName[ templevel ], tempstep, titlestring  );
#endif
				}
				break;
			}
		}
		break;
	case tm_all_info :
		{
			bShowDetailLevel = TRUE;

			itoa(m_SelectedTargetInfo.sStr, targetStr, 10 );
			itoa(m_SelectedTargetInfo.sZen, targetZen, 10 );
			itoa(m_SelectedTargetInfo.sIntelligence, targetInt, 10 );
			itoa(m_SelectedTargetInfo.sConstitution, targetCon, 10 );
			itoa(m_SelectedTargetInfo.sDexterity, targetDex, 10 );

			strcpy( targetRollName, g_CharacterRollNameTable[m_SelectedTargetInfo.cMoonpa][m_SelectedTargetInfo.cClass][m_SelectedTargetInfo.cClassGrade] );
			strcpy( targetGroupName, g_JoinGroupName[ m_SelectedTargetInfo.cMoonpa ] );
			int tempstep = g_pDefaultTooltip_Window->GetInnerLevel(m_SelectedTargetInfo.sInnerLevel);				// 성 ( 1 - 12 )
			int templevel = ( m_SelectedTargetInfo.sInnerLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
#ifdef _XDEF_MODIFY_UI_070710_KUKURI
			sprintf( targetlevelName, "%s %d%s[%d]", g_LevelName[ templevel ], tempstep, titlestring, m_SelectedTargetInfo.sInnerLevel  );
#else
			sprintf( targetlevelName, "%s %d%s", g_LevelName[ templevel ], tempstep, titlestring  );
#endif
		}
		break;
	}
	
	// 해외 버젼일 때 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 37, 1.0f, _XFONT_ALIGNTYPE_CENTER, m_SelectedTargetInfo.cTargetName);
		
//		g_XBaseFont->SetColor( 0xFFDFC61F );
//		g_XBaseFont->PrintAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 57, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetGroupName);
		
		if( targetRollName[0] != 0 )
		{
			g_XBaseFont->SetColor( 0xFFDFC61F );
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 53, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetGroupName);
			
			g_XBaseFont->SetColor( 0xFFBCB96F );	
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 66, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetRollName);
			
			g_XBaseFont->SetColor( 0xFFC9CAC6 );
			g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 79, targetlevelName, 170, _XFONT_ALIGNTYPE_CENTER,1.0f, -1 );	
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFDFC61F );
			g_XBaseFont->PrintAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 57, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetGroupName);

			g_XBaseFont->SetColor( 0xFFC9CAC6 );
			g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x + 90, m_WindowPosition.y + 72, targetlevelName, 170, _XFONT_ALIGNTYPE_CENTER );		
		}
		
		g_XBaseFont->SetColor( 0xFFC8C4C3 );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 45, m_WindowPosition.y + 113, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CINFO_STR));
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 45, m_WindowPosition.y + 129, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CINFO_INNERENERGY));
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 45, m_WindowPosition.y + 145, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CINFO_HEART));
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 45, m_WindowPosition.y + 161, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTHENERGY));
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 45, m_WindowPosition.y + 177, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CINFO_DEX));
		
		
		// 근력	
		g_XBaseFont->SetColor( 0xFF59C74E );		
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 135, m_WindowPosition.y + 113, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetStr);
		
		// 진기
		g_XBaseFont->SetColor( 0xFF59C74E );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 135, m_WindowPosition.y + 129, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetZen);
		
		// 지혜	
		g_XBaseFont->SetColor( 0xFF59C74E );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 135, m_WindowPosition.y + 145, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetInt);
		
		// 건강	
		g_XBaseFont->SetColor( 0xFF59C74E );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 135, m_WindowPosition.y + 161, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetCon);
		
		// 민첨
		g_XBaseFont->SetColor( 0xFF59C74E );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 135, m_WindowPosition.y + 177, 1.0f, _XFONT_ALIGNTYPE_CENTER, targetDex);
		
		g_XBaseFont->Flush();
	}
	else // 국내 버젼일때 
	{
		g_XBaseFont->SetColor( 0xFFBCB96F );	
		g_XBaseFont->Puts( m_WindowPosition.x + 13, m_WindowPosition.y + 37, targetRollName );					  
		
		g_XBaseFont->SetColor( 0xFFFFFFFF );
		g_XBaseFont->Puts( m_WindowPosition.x + 92, m_WindowPosition.y + 37, m_SelectedTargetInfo.cTargetName );		
		
		g_XBaseFont->SetColor( 0xFFDFC61F );
		g_XBaseFont->Puts( m_WindowPosition.x + 17, m_WindowPosition.y + 57, targetGroupName );
		
		g_XBaseFont->SetColor( 0xFFC9CAC6 );
		g_XBaseFont->Puts( m_WindowPosition.x + 70, m_WindowPosition.y + 57, targetlevelName );
		
		g_XBaseFont->SetColor( 0xFFC8C4C3 );
		g_XBaseFont->Puts( m_WindowPosition.x + 15, m_WindowPosition.y + 113,  _XGETINTERFACETEXT(ID_STRING_CINFO_STR));	// _T("근력")
		g_XBaseFont->Puts( m_WindowPosition.x + 15, m_WindowPosition.y + 129,  _XGETINTERFACETEXT(ID_STRING_CINFO_INNERENERGY)); 	// _T("진기")
		g_XBaseFont->Puts( m_WindowPosition.x + 15, m_WindowPosition.y + 145,  _XGETINTERFACETEXT(ID_STRING_CINFO_HEART));	// _T("지혜")
		g_XBaseFont->Puts( m_WindowPosition.x + 15, m_WindowPosition.y + 161,  _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTHENERGY));	// _T("건강")
		g_XBaseFont->Puts( m_WindowPosition.x + 15, m_WindowPosition.y + 177,  _XGETINTERFACETEXT(ID_STRING_CINFO_DEX));	// _T("민첩")
		
		// 근력	
		g_XBaseFont->SetColor( 0xFF59C74E );		
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 57, m_WindowPosition.y + 113, 1.0f, _XFONT_ALIGNTYPE_RIGHT, targetStr);
		
		// 진기
		g_XBaseFont->SetColor( 0xFF59C74E );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 57, m_WindowPosition.y + 129, 1.0f, _XFONT_ALIGNTYPE_RIGHT, targetZen);
		
		// 지혜	
		g_XBaseFont->SetColor( 0xFF59C74E );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 57, m_WindowPosition.y + 145, 1.0f, _XFONT_ALIGNTYPE_RIGHT, targetInt);
		
		// 건강	
		g_XBaseFont->SetColor( 0xFF59C74E );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 57, m_WindowPosition.y + 161, 1.0f, _XFONT_ALIGNTYPE_RIGHT, targetCon);
		
		// 민첨
		g_XBaseFont->SetColor( 0xFF59C74E );
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 57, m_WindowPosition.y + 177, 1.0f, _XFONT_ALIGNTYPE_RIGHT, targetDex);
		
		g_XBaseFont->Flush();

	}

	DrawEquippedItemList();	

	// Hovering mini item tooltip
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if( scrnpos->x > m_WindowPosition.x+65 && scrnpos->x < m_WindowPosition.x+171 &&		 // Equiped item area
		scrnpos->z > m_WindowPosition.y+98 && scrnpos->z < m_WindowPosition.y+230 )
	{				
		RECT iconrect;
		int hoveredslot = SearchSlotRegion(&iconrect);

		if( m_SelectedSlotIndex != hoveredslot )
		{
			if(m_SelectedTargetInfo.getOnItems[hoveredslot].Get_m_sID() > 0)
			{
				DrawItemTooltip(hoveredslot, iconrect);
			}
		}
	}

	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFFFFFFF);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TARGETINFO_2300) );	
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1)+1, m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_TARGETINFO_2300) );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

#ifdef _XTS_LEVELTOOLTIP
	if(bShowDetailLevel)
	{
		// level tooltip
		MouseState* mousestate = gpInput->GetMouseState();
		scrnpos = gpInput->GetMouseVirScreenPos();
		
		// 해외 버젼일 때 
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
//			if( targetRollName[0] != 0 )
//			{
//				if( ((scrnpos->x > m_WindowPosition.x+20) && (scrnpos->x < m_WindowPosition.x+20+150) &&
//					(scrnpos->z > m_WindowPosition.y+95 ) && (scrnpos->z < m_WindowPosition.y+95+15) ) )
//				{
//					TCHAR messagebuffer[256];
//					sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), m_SelectedTargetInfo.sInnerLevel);		// "레벨 %d"
//					g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+76+76, m_WindowPosition.y+57+11, messagebuffer, _XSC_INFORMATION, TRUE);
//				}
//			}
//			else
//			{
//				if( ((scrnpos->x > m_WindowPosition.x+20) && (scrnpos->x < m_WindowPosition.x+20+150) &&
//					(scrnpos->z > m_WindowPosition.y+77 ) && (scrnpos->z < m_WindowPosition.y+77+15) ) )
//				{
//					TCHAR messagebuffer[256];
//					sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), m_SelectedTargetInfo.sInnerLevel);		// "레벨 %d"
//					g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+76+76, m_WindowPosition.y+57+11, messagebuffer, _XSC_INFORMATION, TRUE);
//				}
//			}
		}
		else
		{
			if( ((scrnpos->x > m_WindowPosition.x+76) && (scrnpos->x < m_WindowPosition.x+76+76) &&
				(scrnpos->z > m_WindowPosition.y+57 ) && (scrnpos->z < m_WindowPosition.y+57+11) ) )
			{
				if(m_TargetInfoMode == tm_all_info || m_PtoPLevelDef == 4)
				{
					TCHAR messagebuffer[256];
					sprintf(messagebuffer, _XGETINTERFACETEXT(ID_STRING_NEW_2753), m_SelectedTargetInfo.sInnerLevel);		// "레벨 %d"
					g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+76+76, m_WindowPosition.y+57+11, messagebuffer, _XSC_INFORMATION, TRUE);
				}
			}

#ifdef _XDEF_MODIFY_UI_070710_KUKURI
			if( ((scrnpos->x > m_WindowPosition.x+13) && (scrnpos->x < m_WindowPosition.x+13+36) &&
				(scrnpos->z > m_WindowPosition.y+36) && (scrnpos->z < m_WindowPosition.y+36+14) ) )
			{
				if(m_TargetInfoMode == tm_all_info || m_PtoPLevelDef == 2 || m_PtoPLevelDef == 3 || m_PtoPLevelDef == 4)
				{
					if(g_pLocalUser->m_CharacterInfo.Get_cClass() > 0)
					{
						int classid = g_pLocalUser->m_CharacterInfo.Get_cClass();
						if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
						{
							if(classid == 7)	// 섭혼녀
								classid = 4;
						}
						else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
						{
							if(classid == 8)	// 철갑귀
								classid = 4;
						}

						g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+24, m_WindowPosition.y+54, 
							_XGETINTERFACETEXT(ID_STRING_NEW_2660+classid-1), _XSC_INFORMATION, TRUE);
					}
				}
			}
#endif
		}
	}
#endif
}

BOOL _XWindow_TargetInfo::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if( !_XWindow::Process(pfocusobject) ) return FALSE;		

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	if(mousestate)
	{
		if(pfocusobject)
		{
			// 왼쪽 버튼 클릭 시 처리 
			if(mousestate->bButton[0])
			{
				// left down
				RECT iconrect;
				int selectedindex = SearchSlotRegion(&iconrect);					
				if( selectedindex > -1 )
				{
					m_SelectedSlotIndex = selectedindex;
					if( m_SelectedTargetInfo.getOnItems[m_SelectedSlotIndex].Get_m_sID() > 0 )
					{
						char cType = m_SelectedTargetInfo.getOnItems[m_SelectedSlotIndex].Get_m_cType();
						short sID = m_SelectedTargetInfo.getOnItems[m_SelectedSlotIndex].Get_m_sID();
						
						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_TARGETINFO, m_SelectedSlotIndex, cType, sID ) )
							{
								g_pDefaultTooltip_Window->ShowWindow( TRUE );
								g_MainWindowManager.SetTopWindow( g_pDefaultTooltip_Window );
							}
						}
					}	
					else
					{
						if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}
				}
				else
				{
					if( g_pDefaultTooltip_Window && g_pDefaultTooltip_Window->GetShowStatus() )
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}				
			}
		}
	}
	
	return TRUE;
}

void _XWindow_TargetInfo::DrawEquippedItemList(void)
{
	for( int iter = _TARGETINFO::pos_amor; iter < _TARGETINFO::get_on_max ; iter++)
	{
		if( iter != _TARGETINFO::pos_backpack )
		{		
			if(m_SelectedTargetInfo.getOnItems[iter].Get_m_sID() > 0)
			{
				if( iter >= _XINVENTORY_SLOTTYPE_WEAPON_1 )
				{
					m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_WEAPON].Draw( m_WindowPosition.x + g_TargetInfoItemCoord[iter].x-1, 
						m_WindowPosition.y + g_TargetInfoItemCoord[iter].y-1 );
				}			
				else 
				{
					if( iter != _TARGETINFO::pos_gap_upper && iter != _TARGETINFO::pos_gap_lower )
					{
						m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_CLOTHES].Draw( m_WindowPosition.x + g_TargetInfoItemCoord[iter].x-1, 
							m_WindowPosition.y + g_TargetInfoItemCoord[iter].y-1 );
					}					
				}
				
				if( iter != _TARGETINFO::pos_gap_upper && iter != _TARGETINFO::pos_gap_lower )
				{
					g_pInventory_Window->DrawIcon(TRUE,m_WindowPosition.x + g_TargetInfoItemCoord[iter].x, 
						m_WindowPosition.y + g_TargetInfoItemCoord[iter].y, &m_SelectedTargetInfo.getOnItems[iter]);					
				}				
			}
			else
			{
				m_EquipedItemEdgeIcon[_XEQUIPEDITEMEDGEICONTYPE_EMPTY].Draw( m_WindowPosition.x + g_TargetInfoItemCoord[iter].x-1, 
					m_WindowPosition.y + g_TargetInfoItemCoord[iter].y-1 );				
			}
		}
	}
	
}

int	_XWindow_TargetInfo::SearchSlotRegion(  RECT* pRect  )
{
	ScrnPos* scrnpos	= gpInput->GetMouseVirScreenPos();
			
	// 장착 슬롯 검사
	for( int iter = _XINVENTORY_SLOTTYPE_AMOR; iter <= _XINVENTORY_SLOTTYPE_WEAPON_2; iter++ )
	{
		if( ((scrnpos->x > m_WindowPosition.x+g_TargetInfoItemCoord[iter].x )   && 
			 (scrnpos->x < m_WindowPosition.x+g_TargetInfoItemCoord[iter].x+32) &&
			 (scrnpos->z > m_WindowPosition.y+g_TargetInfoItemCoord[iter].y )   && 
			 (scrnpos->z < m_WindowPosition.y+g_TargetInfoItemCoord[iter].y+32) ) )
		{			
			if( pRect )
			{
				pRect->left   = m_WindowPosition.x+g_TargetInfoItemCoord[iter].x-1;
				pRect->top    = m_WindowPosition.y+g_TargetInfoItemCoord[iter].y-1;
				pRect->right  = m_WindowPosition.x+g_TargetInfoItemCoord[iter].x + 29;
				pRect->bottom = m_WindowPosition.y+g_TargetInfoItemCoord[iter].y + 29;
			}
			return iter;
		}		
	}

	return -1;
}

void _XWindow_TargetInfo::MoveWindow(int X, int Y)
{	
	_XWindow::MoveWindow(X, Y);
		
	// 해외 버젼일 때 
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y, m_WindowPosition.x + 179, m_WindowPosition.y + 342 );
	}
	else
	{
		m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y, m_WindowPosition.x + 179, m_WindowPosition.y + 242 );
	}
}

#ifdef _XTS_ITEM_OPTIMIZATION
void _XWindow_TargetInfo::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = m_SelectedTargetInfo.getOnItems[slotnumber].Get_m_cType();
	short sID = m_SelectedTargetInfo.getOnItems[slotnumber].Get_m_sID();
	unsigned char cStrength = m_SelectedTargetInfo.getOnItems[slotnumber].Get_m_ucStrength();

	if( sID > 0 )
	{
		TCHAR tempbuf[256];
		switch(GetEqualItemType(cType)) 
		{
		case _XGI_FC_WEAPON:
			{				
#ifdef _XTS_ITEM_V20
				if( cStrength > 0 )
					sprintf( tempbuf, "%s +%d", _XGameItem::GetItemFullName(cType, sID), cStrength );
				else
					strcpy( tempbuf, _XGameItem::GetItemFullName(cType, sID) );
#else
				if( cStrength > 0 )
					sprintf( tempbuf, "%s %s +%d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength );
				else
					sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
#endif
			}
			break;
		case _XGI_FC_CLOTHES:
			{
#ifdef _XTS_ITEM_V20
				strcpy( tempbuf, _XGameItem::GetItemFullName(cType, sID) );
#else
				sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
#endif
				
			}
			break;
		default:
			{
				strcpy( tempbuf, _XGameItem::GetItemName(cType, sID));
			}
			break;
		}
		g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
	}
}
#else
void _XWindow_TargetInfo::DrawItemTooltip(int slotnumber, RECT iconrect)
{
	char cType = m_SelectedTargetInfo.getOnItems[slotnumber].m_cType;
	short sID = m_SelectedTargetInfo.getOnItems[slotnumber].m_sID;
	unsigned char cStrength = m_SelectedTargetInfo.getOnItems[slotnumber].m_ucStrength;

	if( sID > 0 )
	{
		TCHAR tempbuf[256];
		switch(cType) 
		{
		case _XGI_FC_WEAPON:
		case _XGI_FC_WEAPON2:	
		case _XGI_FC_WEAPON3:
			{				
				if( cStrength > 0 )
					sprintf( tempbuf, "%s %s +%d", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID), cStrength );
				else
					sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
			}
			break;
		case _XGI_FC_CLOTHES:		
		case _XGI_FC_CLOTHES2:
		case _XGI_FC_CLOTHES3:
			{
				sprintf( tempbuf, "%s %s", _XGameItem::GetItemNick(cType, sID), _XGameItem::GetItemName(cType, sID) );
				
			}
			break;
		default:
			{
				strcpy( tempbuf, _XGameItem::GetItemName(cType, sID));
			}
			break;
		}
		g_InterfaceTooltip.SetToolTipString(iconrect.left+33, iconrect.top-7, tempbuf, _XGameItem::GetGradeColor(cType, sID), TRUE);
	}
}
#endif


// XWindow_MaximumMap.cpp: implementation of the _XWindow_MaximumMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_MaximumMap.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

#define _XDEF_MAXIMUMMAP_LEFT			(gnWidth>>1)-224
#define _XDEF_MAXIMUMMAP_TOP			(gnHeight>>1)-224
#define _XDEF_MAXIMUMMAP_RIGHT			(gnWidth>>1)+225
#define _XDEF_MAXIMUMMAP_BOTTOM			(gnHeight>>1)+225

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MaximumMap::_XWindow_MaximumMap()
{
	m_CurrentServerIndex = 1;

}

_XWindow_MaximumMap::~_XWindow_MaximumMap()
{

}

BOOL _XWindow_MaximumMap::Initialize( void )
{	
	_XImageStatic* pTopBorder = new _XImageStatic;
	pTopBorder->Create( 0, 0, 134, 206,
					   &g_MainInterfaceTextureArchive, 
					   g_MainInterfaceTextureArchive.FindResource( "maximummap_base01.tga" ) );	
	InsertChildObject( pTopBorder );
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create( 0, 206, 134, 362,
					   &g_MainInterfaceTextureArchive, 
					   g_MainInterfaceTextureArchive.FindResource( "maximummap_base02.tga" ) );
	InsertChildObject( pBottomBorder );	

	int buttonindex = g_MainInterfaceTextureArchive.FindResource( "maximummap_button.tga" );
	_XBTN_STRUCTURE minimizebtnstruct = { TRUE, { 11, 337 }, { 112, 15 }, _XDEF_MAXMINIMAP_CLOSEBUTTON,
		buttonindex,buttonindex,buttonindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* minimizebutton = new _XButton;
	minimizebutton->Create( minimizebtnstruct );
	minimizebutton->SetButtonTextColor( _XSC_WARNING );
	minimizebutton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAXIMUMMAP_CLOSE);
	InsertChildObject( minimizebutton );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 0, 113,15 );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 30,113,45 );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 15,113,30 );
	
	_XBTN_STRUCTURE applybtnstruct = { TRUE, { 11, 318 }, { 112, 15 }, 
									   _XDEF_MAXMINIMAP_APPLYBUTTON, buttonindex,buttonindex,buttonindex, 
									  &g_MainInterfaceTextureArchive };
	_XButton* applybutton = new _XButton;
	applybutton->Create( applybtnstruct );		
	applybutton->SetButtonTextColor( _XSC_DEFAULT_HIGHLIGHT );
	applybutton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAXIMUMMAP_APPLY);	
	applybutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 0, 113,15 );
	applybutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 30,113,45 );
	applybutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 15,113,30 );
	InsertChildObject( applybutton );



	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 102, 275}, { 15, 15 }, 
											_XDEF_MAXMINIMAP_NPCVIEWBUTTON,
											buttonindex,buttonindex,
											&g_MainInterfaceTextureArchive };
	
	m_pNPCViewButton = new _XCheckButton;	
	m_pNPCViewButton->Create( checkbtnstruct );	
	m_pNPCViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 108,45, 123, 60 );
	m_pNPCViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  93, 45, 108, 60 );
	m_pNPCViewButton->SetCheck( TRUE  );
	InsertChildObject( m_pNPCViewButton );	
	
	checkbtnstruct.position.y = 291;
	checkbtnstruct.commandid = _XDEF_MAXMINIMAP_PARTYVIEWBUTTON;
	m_pPartyViewButton = new _XCheckButton;	
	m_pPartyViewButton->Create( checkbtnstruct );	
	m_pPartyViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 108, 45, 123, 60 );
	m_pPartyViewButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  93,  45, 108, 60 );
	m_pPartyViewButton->SetCheck( FALSE );
	InsertChildObject( m_pPartyViewButton );
	
	int minampbuttonimageindex = g_MainInterfaceTextureArchive.FindResource( "minimap_button.tga" );
	
	m_MoonImage.Create( m_WindowPosition.x + 14, m_WindowPosition.y + 61, 
						m_WindowPosition.x + 27, m_WindowPosition.y + 74,
						&g_MainInterfaceTextureArchive, minampbuttonimageindex );
	RECT cliprect = { 2, 22, 13, 33  };
	m_MoonImage.SetClipRect( cliprect );

	m_LargeMinimapImage.Create( (gnWidth>>1)-224, (gnHeight>>1)-224, (gnWidth>>1)+225, (gnHeight>>1)+225,
							  &g_MainInterfaceTextureArchive, -1 );	
	m_LargeMinimapImage.SetClipRect( 0, 0, 512, 512 );
	m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);

	// player position
	m_MaximummapArrow.Create(_XDEF_MAXIMUMMAP_LEFT - 18, _XDEF_MAXIMUMMAP_BOTTOM - 18,
		_XDEF_MAXIMUMMAP_LEFT+17,_XDEF_MAXIMUMMAP_BOTTOM+17);
	m_MaximummapArrow.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mm_mark_character.tga"));
	
	m_FrustumImage.Create(_XDEF_MAXIMUMMAP_LEFT-32, _XDEF_MAXIMUMMAP_BOTTOM-32, 
		_XDEF_MAXIMUMMAP_LEFT+31, _XDEF_MAXIMUMMAP_BOTTOM+31,
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "mm_mark_frustum.tga" ));
	m_FrustumImage.SetCenter(32.0f, 32.0f);
	
	m_NPCImageTradeType.Create( m_WindowPosition.x, m_WindowPosition.y, 
							 m_WindowPosition.x + 8, m_WindowPosition.y + 8,
							 &g_MainInterfaceTextureArchive, minampbuttonimageindex );
	RECT NPCTradeTypecliprect = { 140, 7, 148, 15  };
	m_NPCImageTradeType.SetClipRect( NPCTradeTypecliprect );
	m_NPCImageTradeType.SetScale(0.7f,0.7f);
	
	m_NPCImageEventType.Create( m_WindowPosition.x, m_WindowPosition.y, 
							 m_WindowPosition.x + 9, m_WindowPosition.y + 10,
							 &g_MainInterfaceTextureArchive, minampbuttonimageindex );
	RECT NPCEventcliprect = { 131, 6, 140, 16  };
	m_NPCImageEventType.SetClipRect( NPCEventcliprect );
	m_NPCImageEventType.SetScale(0.7f,0.7f);
	
	m_PartyPlayerImage.Create( m_WindowPosition.x, m_WindowPosition.y, 
							 m_WindowPosition.x + 6, m_WindowPosition.y + 6,
							 &g_MainInterfaceTextureArchive, minampbuttonimageindex );
	RECT PartyPlayercliprect = { 139, 0, 146, 7  }; // 2004.05.12->oneway48 modify
	m_PartyPlayerImage.SetClipRect( PartyPlayercliprect );
	
	m_BlueImpressionMark.Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+16, m_WindowPosition.y+16,
		&g_MainInterfaceTextureArchive, minampbuttonimageindex);
	RECT bluerect = { 0, 52, 14, 66  };
	m_BlueImpressionMark.SetClipRect( bluerect );
	
	m_YellowImpressionMark.Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+16, m_WindowPosition.y+16,
		&g_MainInterfaceTextureArchive, minampbuttonimageindex);
	RECT yellowrect = { 14, 52, 28, 66  };
	m_YellowImpressionMark.SetClipRect( yellowrect );
	
	m_GreenImpressionMark.Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+16, m_WindowPosition.y+16,
		&g_MainInterfaceTextureArchive, minampbuttonimageindex);
	RECT greenrect = { 28, 52, 42, 66  };
	m_GreenImpressionMark.SetClipRect( greenrect );
	
	m_OrangeImpressionMark.Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+16, m_WindowPosition.y+16,
		&g_MainInterfaceTextureArchive, minampbuttonimageindex);
	RECT orangerect = { 42, 52, 56, 66  };
	m_OrangeImpressionMark.SetClipRect( orangerect );
	
	// 힌트마크 생성(?)
	m_BlueQuestionMark.Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+16, m_WindowPosition.y+16,
		&g_MainInterfaceTextureArchive, minampbuttonimageindex);
	RECT bluehintrect = { 56, 52, 70, 66  };
	m_BlueQuestionMark.SetClipRect( bluehintrect );
	
	m_YellowQuestionMark.Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+16, m_WindowPosition.y+16,
		&g_MainInterfaceTextureArchive, minampbuttonimageindex);
	RECT yellowhintrect = { 70, 52, 84, 66  };
	m_YellowQuestionMark.SetClipRect( yellowhintrect );
	
	m_GreenQuestionMark.Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+16, m_WindowPosition.y+16,
		&g_MainInterfaceTextureArchive, minampbuttonimageindex);
	RECT greenhintrect = { 84, 52, 98, 66  };
	m_GreenQuestionMark.SetClipRect( greenhintrect );
	
	m_OrangeQuestionMark.Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+16, m_WindowPosition.y+16,
		&g_MainInterfaceTextureArchive, minampbuttonimageindex);
	RECT orangehintrect = { 98, 52, 112, 66  };
	m_OrangeQuestionMark.SetClipRect( orangehintrect );
	
	
	SetWindowMoveMode( _XWMOVE_FIXED );
	SetDrawBorder( FALSE );

	return TRUE;
}

BOOL _XWindow_MaximumMap::SetMinimapImage( LPTSTR mapname )
{
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

	strcat( minimapnamestr, mapname );

	LPTSTR cutpoint = strstr( minimapnamestr, "xtv" );
	if( cutpoint )
	{
		*cutpoint = NULL;
	}

	strcat( minimapnamestr, "tga");
	
	int resourceindex = g_TerrainTextureArchive.GetResourceIndex( minimapnamestr, _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE );
	if( resourceindex < 0 )
	{

		strcpy( minimapnamestr, "Mini" );
		strcat( minimapnamestr, mapname );	
		LPTSTR cutpoint = strstr( minimapnamestr, "xtv" );
		if( cutpoint )
		{
			*cutpoint = NULL;
		}	
		strcat( minimapnamestr, "tga");

		int resourceindex = g_TerrainTextureArchive.GetResourceIndex( minimapnamestr, _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE );

		if( resourceindex < 0 )
		{
			_XFatalError( "Resource not found in the texture archive [%s]", minimapnamestr );
		}
	}

	m_LargeMinimapImage.SetTexture( &g_TerrainTextureArchive, resourceindex );

	return TRUE;
}

void _XWindow_MaximumMap::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );

	m_MoonImage.MoveWindow( m_WindowPosition.x + 13, m_WindowPosition.y + 60 );	
}

void _XWindow_MaximumMap::SaveWindowProperty( void )
{

}

void _XWindow_MaximumMap::DestroyWindow( void )
{	
	m_MaximummapArrow.ReleaseObject();
	_XWindow::DestroyWindow();
}

void _XWindow_MaximumMap::Draw( _XGUIObject*& pfocusobject )
{			
	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw Window border
	_XDrawSolidBar( (gnWidth>>1)-229, (gnHeight>>1)-229, (gnWidth>>1)+230, (gnHeight>>1)+230, D3DCOLOR_ARGB(128,0,0,0) );
	_XDrawRectAngle( (gnWidth>>1)-230, (gnHeight>>1)-230, (gnWidth>>1)+230, (gnHeight>>1)+230, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( (gnWidth>>1)-227, (gnHeight>>1)-227, (gnWidth>>1)+227, (gnHeight>>1)+227, 0.0f, D3DCOLOR_ARGB(128,96,96,96) );
	_XDrawRectAngle( (gnWidth>>1)-226, (gnHeight>>1)-226, (gnWidth>>1)+226, (gnHeight>>1)+226, 0.0f, D3DCOLOR_ARGB(128,0,0,0) );

	POINT pos;
	pos.x = m_WindowPosition.x + 13;
	pos.y = m_WindowPosition.y + 60;
	
	/*
	if( g_LodTerrain.m_Environment == _XE_DAWN )
	{
		m_MoonImage.SetClipRect( 2, 22, 13, 33 );
		m_MoonImage.DrawWithRegion();
		
		int percentage = 14 - (int)(14.0f *  ( (FLOAT)(_XDEF_ENVTIMESTARTTIME_DAY - g_LodTerrain.m_EnvironmentTime) / 
			(FLOAT)_XDEF_ENVTIMESTARTTIME_DAY ) );
		_XDrawSolidBar( pos.x, pos.y + percentage, pos.x+14, pos.y + 14, D3DCOLOR_ARGB( 64, 255, 255, 255) );
	}
	else if( g_LodTerrain.m_Environment == _XE_DAY )
	{
		//m_MoonImage.SetClipRect( 16, 22, 27, 33 );		
		m_MoonImage.DrawWithRegion();
		
		int percentage = 14 - (int)(14.0f *  ( (FLOAT)(_XDEF_ENVTIMESTARTTIME_NIGHTFALL - g_LodTerrain.m_EnvironmentTime) / 
			(FLOAT)(_XDEF_ENVTIMESTARTTIME_NIGHTFALL - _XDEF_ENVTIMESTARTTIME_DAY ) ) );
		_XDrawSolidBar( pos.x, pos.y + percentage, pos.x+14, pos.y + 14, D3DCOLOR_ARGB( 64, 255, 255, 255) );
	}
	else if( g_LodTerrain.m_Environment == _XE_NIGHTFALL )
	{
		m_MoonImage.SetClipRect( 30, 22, 41, 33 );		
		m_MoonImage.DrawWithRegion();		
		
		int percentage = 14 - (int)(14.0f * ( (FLOAT)(_XDEF_ENVTIMESTARTTIME_NIGHT - g_LodTerrain.m_EnvironmentTime) / 
			(FLOAT)(_XDEF_ENVTIMESTARTTIME_NIGHT - _XDEF_ENVTIMESTARTTIME_NIGHTFALL) ) );
		_XDrawSolidBar( pos.x, pos.y + percentage, pos.x+14, pos.y + 14, D3DCOLOR_ARGB( 64, 255, 255, 255) );
	}
	else if( g_LodTerrain.m_Environment == _XE_NIGHT )
	{
		m_MoonImage.SetClipRect( 44, 22, 55, 33 );
		m_MoonImage.DrawWithRegion();
		
		int percentage = 14 - (int)(14.0f * ( (FLOAT)(_XDEF_ENVTIMESTARTTIME_MAX - g_LodTerrain.m_EnvironmentTime) / 
			(FLOAT)(_XDEF_ENVTIMESTARTTIME_MAX - _XDEF_ENVTIMESTARTTIME_NIGHT ) ) );
		_XDrawSolidBar( pos.x, pos.y + percentage, pos.x+14, pos.y + 14, D3DCOLOR_ARGB( 64, 255, 255, 255) );
	}

  */
	
	m_LargeMinimapImage.DrawWithRegion();

	if(m_pNPCViewButton->GetCheck())
		DrawNPCPosition();
	
	if((m_pPartyViewButton->GetCheck()) && (g_pLocalUser->m_InParty))
		DrawPartyPlayerPosition();
	
	if(!g_pLocalUser->m_InParty)	
		m_pPartyViewButton->SetCheck( FALSE );
	
	// player position
	m_FrustumImage.Draw();
	m_MaximummapArrow.RenderAlphaBlend();
	
	// Draw font
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	if( g_ZeroLevelMode )		// hotblood zerolevel
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 68, m_WindowPosition.y + 18, 1.0, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2649) );
	else
		g_XBaseFont->PrintAlign( m_WindowPosition.x + 68, m_WindowPosition.y + 18, 1.0, _XFONT_ALIGNTYPE_CENTER, g_ZoneInfoTable[m_CurrentServerIndex-1].zonename );
	
	int param1 = g_ServerGameTime.Year;
	int param2 = g_ServerGameTime.Month+1;
	int param3 = g_ServerGameTime.Day+1;

	g_XBaseFont->Puts( m_WindowPosition.x + 30, m_WindowPosition.y + 63, 
					   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAXIMUMMAP_DATE, &param1, &param2, &param3 ),
					   1.0f );

	g_XBaseFont->Puts( m_WindowPosition.x + 19, m_WindowPosition.y + 278, "NPC" );
	g_XBaseFont->Puts( m_WindowPosition.x + 19, m_WindowPosition.y + 293, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_PARTY) );

	g_XBaseFont->SetColor( _XSC_INFORMATION );
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 68, m_WindowPosition.y + 37, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%d %d", 
							_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x), _XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
	
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 68, m_WindowPosition.y + 86, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_MAP) );
	g_XBaseFont->Puts( m_WindowPosition.x + 68, m_WindowPosition.y + 221, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_COMPASS) );
	g_XBaseFont->PrintAlign( m_WindowPosition.x + 68, m_WindowPosition.y + 256, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_POSITION) );
	
	g_XBaseFont->Flush();


}

BOOL _XWindow_MaximumMap::Process( _XGUIObject*& pfocusobject )
{
	if( !_XWindow::Process(pfocusobject) ) 
		return FALSE;

	SetLocalUserPosition();
			
	return TRUE;
}

void _XWindow_MaximumMap::SetLocalUserPosition(void)
{
	float RateOfMapSizeToTerrain = 450.0f / g_LodTerrain.m_Width;
	
	float x = g_pLocalUser->m_Position.x * RateOfMapSizeToTerrain + 225.0f;
	float z = -g_pLocalUser->m_Position.z * RateOfMapSizeToTerrain - 225.0f;
	
	m_MaximummapArrow.MoveWindow(_XDEF_MAXIMUMMAP_LEFT - 18 + x, 
								_XDEF_MAXIMUMMAP_BOTTOM - 18 + z, 
								_XDEF_MAXIMUMMAP_LEFT + 17 + x, 
								_XDEF_MAXIMUMMAP_BOTTOM + 17+ z);
	
	_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
	
	m_MaximummapArrow.Rotate(pminimapwindow->m_RotateAngle);
	m_FrustumImage.MoveWindow( _XDEF_MAXIMUMMAP_LEFT - 32 + x, 
								_XDEF_MAXIMUMMAP_BOTTOM - 32 + z, 
								_XDEF_MAXIMUMMAP_LEFT + 31 + x, 
								_XDEF_MAXIMUMMAP_BOTTOM + 31+ z);
	m_FrustumImage.SetRotateAngle(pminimapwindow->m_FrustumRotateAngle);
}

void _XWindow_MaximumMap::DrawPartyPlayerPosition(void)
{	
	float RateOfMapSizeToTerrain = 450.0f / g_LodTerrain.m_Width;
	
	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++)  // 2004.06.01->oneway48 modify : 15->9
	{
		if( g_pLocalUser->m_PartyInfo[i].playername[0] != 0 && g_pLocalUser->m_PartyInfo[i].isValid)
		{
			BOOL isCurrentUser = FALSE;
			CheckCurrentUser(i,isCurrentUser);
			
			if(!isCurrentUser)
			{
				float x = g_pLocalUser->m_PartyInfo[i].posX * RateOfMapSizeToTerrain + 225.0f;
				float z = -g_pLocalUser->m_PartyInfo[i].posZ * RateOfMapSizeToTerrain - 225.0f;
				
				m_PartyPlayerImage.MoveWindow(_XDEF_MAXIMUMMAP_LEFT - 3 + x, 
											_XDEF_MAXIMUMMAP_BOTTOM - 3 + z, 
											_XDEF_MAXIMUMMAP_LEFT + 2 + x, 
											_XDEF_MAXIMUMMAP_BOTTOM + 2 + z);
				m_PartyPlayerImage.DrawWithRegion();
			}
		}
	}
}

void _XWindow_MaximumMap::CheckCurrentUser(int partyindex, BOOL &isCurrentUser)
{
	if(g_pLocalUser->m_PartyInfo[partyindex].playername[0] != 0)
	{
		if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, g_pLocalUser->m_PartyInfo[partyindex].playername) != 0)
		{
			isCurrentUser = FALSE;
		}
		else
		{
			isCurrentUser = TRUE;
		}
	}
}

void _XWindow_MaximumMap::DrawNPCPosition(void)
{	
	float RateOfMapSizeToTerrain = 450.0f / g_LodTerrain.m_Width;
	
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; i++)
	{		
		float x = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._41 * RateOfMapSizeToTerrain + 225.0f;
		float z = -g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._43 * RateOfMapSizeToTerrain - 225.0f;
		
		if(g_Npclist.m_pNPCObjectArray[i].m_NPCRollType == _XNPCROLL_TRADE)
		{
			m_NPCImageTradeType.MoveWindow(_XDEF_MAXIMUMMAP_LEFT - 4 + x, 
										_XDEF_MAXIMUMMAP_BOTTOM - 4 + z, 
										_XDEF_MAXIMUMMAP_LEFT + 3 + x, 
										_XDEF_MAXIMUMMAP_BOTTOM + 3 + z);
			m_NPCImageTradeType.DrawWithRegion();
		}
		else
		{
			m_NPCImageEventType.MoveWindow(_XDEF_MAXIMUMMAP_LEFT - 4 + x, 
										_XDEF_MAXIMUMMAP_BOTTOM - 4 + z, 
										_XDEF_MAXIMUMMAP_LEFT + 3 + x, 
										_XDEF_MAXIMUMMAP_BOTTOM + 3 + z);
			m_NPCImageEventType.DrawWithRegion();
		}
	}
	if( !g_QuestScriptManager.m_listQuestMark.empty() )
	{		
		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; iter_questmark++)
		{
			_XQUEST_MARKINFO questmark = *iter_questmark;
			
			int questmarkpositionX = questmark.MarkPosition_X * RateOfMapSizeToTerrain + 225.0f;
			int questmarkpositionY = -questmark.MarkPosition_Y * RateOfMapSizeToTerrain - 225.0f;
			
			if( questmark.questid >= 10000 &&  questmark.questid < 20000 )
			{
				m_YellowImpressionMark.Draw(_XDEF_MAXIMUMMAP_LEFT - 7 + questmarkpositionX, _XDEF_MAXIMUMMAP_BOTTOM - 7 + questmarkpositionY);
			}
			else
			{
				m_BlueImpressionMark.Draw(_XDEF_MAXIMUMMAP_LEFT - 7 + questmarkpositionX, _XDEF_MAXIMUMMAP_BOTTOM - 7 + questmarkpositionY);
			}			
		}
	}
	if( !g_QuestScriptManager.m_listHintMark.empty() )
	{		
		list <_XQUEST_MARKINFO>::iterator iter_hintmark;
		for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; iter_hintmark++)
		{
			_XQUEST_MARKINFO hintmark = *iter_hintmark;
			
			int hintmarkpositionX = hintmark.MarkPosition_X * RateOfMapSizeToTerrain + 225.0f;
			int hinttmarkpositionY = -hintmark.MarkPosition_Y * RateOfMapSizeToTerrain - 225.0f;
			
			if( hintmark.questid >= 10000 &&  hintmark.questid < 20000 )
			{
				m_YellowQuestionMark.Draw(_XDEF_MAXIMUMMAP_LEFT - 7 + hintmarkpositionX, _XDEF_MAXIMUMMAP_BOTTOM - 7 + hinttmarkpositionY);
			}
			else
			{
				m_BlueQuestionMark.Draw(_XDEF_MAXIMUMMAP_LEFT - 7 + hintmarkpositionX, _XDEF_MAXIMUMMAP_BOTTOM - 7 + hinttmarkpositionY);
			}
	
		}
	}
}
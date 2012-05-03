// XWindow_QSlot.cpp: implementation of the _XWindow_QSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_WorldMinimap.h"
#include "XWindow_QSlot.h"
#include "XWindow_Skill.h"
#include "XWindow_Inventory.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"

void __stdcall _XCallback_QS_Next(  POINT pos, SIZE size  )
{		
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_QSLOT_NEXT));
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y+4, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_QS_Prev(  POINT pos, SIZE size  )
{		
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_QSLOT_PREV));
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y+4, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_QS_LevelDown(  POINT pos, SIZE size  )
{		
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_QSLOT_DOWN));
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y+4, messagestring, _XSC_INFORMATION );	
}

void __stdcall _XCallback_QS_LevelUp(  POINT pos, SIZE size  )
{		
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_QSLOT_UP));
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y+4, messagestring, _XSC_INFORMATION );	
}


void __stdcall _XCallback_QS_TypeChange(  POINT pos, SIZE size  )
{		
	TCHAR messagestring[256];
	memset(messagestring, 0, sizeof(TCHAR)*256);
	strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_QSLOT_CHANGE));
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y+4, messagestring, _XSC_INFORMATION );	
}

/*
#define _XDEF_USERINFOFILE_VERSION	0xFE02

BOOL _XQuickSlotInfoList::LoadQuickSlotInfo( void )
{
	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return TRUE;

	disposeList();

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
	FILE* pFilePtr = fopen( _T("UserInfo.XUI"), "rb" );
	
	if( !pFilePtr ) 
	{
		FILE* pFilePtr = fopen( _T("UserInfo.XUI"), "wb" );
		int _infocount = 0;
		DWORD version = _XDEF_USERINFOFILE_VERSION;
		fwrite( &version, sizeof(DWORD), 1, pFilePtr );
		fwrite( &_infocount , sizeof(int), 1, pFilePtr );
		fclose( pFilePtr );
		return TRUE;
	}
	
	DWORD version = 0;
	fread( &version, sizeof(DWORD), 1, pFilePtr );

	if( version != _XDEF_USERINFOFILE_VERSION )
	{
		_XLog( "WARNING : Not supported file version %d/%d", version, _XDEF_USERINFOFILE_VERSION );
		fclose( pFilePtr );

		pFilePtr = fopen( _T("UserInfo.XUI"), "wb" );
		int _infocount = 0;
		DWORD version = _XDEF_USERINFOFILE_VERSION;
		fwrite( &version, sizeof(DWORD), 1, pFilePtr );
		fwrite( &_infocount , sizeof(int), 1, pFilePtr );
		fclose( pFilePtr );

		return FALSE;
	}

	int _infocount = 0;
	fread( &_infocount , sizeof(int), 1, pFilePtr );
	
	_XQuickSlotSaveInfo* pQuickSlotSaveInfo;
	
	for( int i = 0; i < _infocount; i++ )
	{
		pQuickSlotSaveInfo = new _XQuickSlotSaveInfo;

		if( pQuickSlotSaveInfo )
		{
			memset(pQuickSlotSaveInfo->Owner, 0, _XDEF_MAX_USERNAMESTRINGLENGTH );
			fread( pQuickSlotSaveInfo->Owner, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH, 1, pFilePtr );
			fread( pQuickSlotSaveInfo->m_QuickSlot, sizeof(_XQSLOT_INFO) * 27, 1, pFilePtr );
			insertItem( pQuickSlotSaveInfo );
		}
	}
	
	fclose( pFilePtr );	
	return TRUE;
}

void _XQuickSlotInfoList::UpdateQuickSlotInfo( void )
{
	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return;
	
	_XQuickSlotSaveInfo* pQuickSlotSaveInfo;

	BOOL savedcurrentuser = FALSE;
	
	_XWindow_QSlot* pQSlot_Window = (_XWindow_QSlot *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUICKSLOT);
	if( !pQSlot_Window ) return;
	
	if( !listEmpty() )
	{
		resetList();
		
		do 
		{
			pQuickSlotSaveInfo = (_XQuickSlotSaveInfo*)currentItem();
			if( pQuickSlotSaveInfo )
			{
				if( strcmpi( pQuickSlotSaveInfo->Owner, g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
				{
					savedcurrentuser = TRUE;
					memcpy( pQSlot_Window->m_QuickSlot, pQuickSlotSaveInfo->m_QuickSlot, sizeof( _XQSLOT_INFO ) * 27 );
					break;
				}				
			}
			
			nextItem();
		} while( !atHeadOfList() );
	}
	
	if( !savedcurrentuser )
	{
		pQuickSlotSaveInfo = new _XQuickSlotSaveInfo;
		memset( pQuickSlotSaveInfo->Owner, 0, _XDEF_MAX_USERNAMESTRINGLENGTH );
		strcpy( pQuickSlotSaveInfo->Owner, g_pLocalUser->m_CharacterInfo.charactername );
		
		memset( pQuickSlotSaveInfo->m_QuickSlot, 0, sizeof( _XQSLOT_INFO ) * 27 );
		memcpy( pQuickSlotSaveInfo->m_QuickSlot, pQSlot_Window->m_QuickSlot, sizeof( _XQSLOT_INFO ) * 27 );
		
		insertItem( pQuickSlotSaveInfo );
	}
}

void _XQuickSlotInfoList::UpdateInfoList( void )
{
	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return;
	
	_XQuickSlotSaveInfo* pQuickSlotSaveInfo;
	BOOL savedcurrentuser = FALSE;
	
	_XWindow_QSlot* pQSlot_Window = (_XWindow_QSlot *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUICKSLOT);
	if( !pQSlot_Window ) return;
	
	if( !listEmpty() )
	{
		resetList();
		
		do 
		{
			pQuickSlotSaveInfo = (_XQuickSlotSaveInfo*)currentItem();
			if( pQuickSlotSaveInfo )
			{
				if( strcmpi( pQuickSlotSaveInfo->Owner, g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
				{
					savedcurrentuser = TRUE;
					memcpy( pQuickSlotSaveInfo->m_QuickSlot, pQSlot_Window->m_QuickSlot, sizeof( _XQSLOT_INFO ) * 27 );
					break;
				}				
			}
			
			nextItem();
		} while( !atHeadOfList() );
	}
	
	if( !savedcurrentuser )
	{
		pQuickSlotSaveInfo = new _XQuickSlotSaveInfo;
		memset( pQuickSlotSaveInfo->Owner, 0, _XDEF_MAX_USERNAMESTRINGLENGTH );
		strcpy( pQuickSlotSaveInfo->Owner, g_pLocalUser->m_CharacterInfo.charactername );
		
		memset( pQuickSlotSaveInfo->m_QuickSlot, 0, sizeof( _XQSLOT_INFO ) * 27 );
		memcpy( pQuickSlotSaveInfo->m_QuickSlot, pQSlot_Window->m_QuickSlot, sizeof( _XQSLOT_INFO ) * 27 );
		
		insertItem( pQuickSlotSaveInfo );
	}
}

BOOL _XQuickSlotInfoList::SaveQuickSlotInfo( void )
{
	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return TRUE;

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
	FILE* pFilePtr = fopen( _T("UserInfo.XUI"), "wb" );

	if( !pFilePtr ) 
	{
		_XLog( "WARNING : Can't open user informations file" );
		return FALSE;
	}
	
	DWORD version = _XDEF_USERINFOFILE_VERSION;
	fwrite( &version, sizeof(DWORD), 1, pFilePtr );
	fwrite( &itemcount, sizeof(int), 1, pFilePtr );

	_XQuickSlotSaveInfo* pQuickSlotSaveInfo;

	if( !listEmpty() )
	{
		resetList();

		do 
		{
			pQuickSlotSaveInfo = (_XQuickSlotSaveInfo*)currentItem();
			if( pQuickSlotSaveInfo )
			{
				fwrite( pQuickSlotSaveInfo->Owner, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH, 1, pFilePtr );
				fwrite( pQuickSlotSaveInfo->m_QuickSlot, sizeof(_XQSLOT_INFO) * 27, 1, pFilePtr );
			}
			
			nextItem();
		} while( !atHeadOfList() );
	}
	
	fclose( pFilePtr );

	return TRUE;
}
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_QSlot::_XWindow_QSlot()
{
	m_pInventory				=	 NULL;
	m_WindowStyle				=	_XQS_WINSTYLE_HORIZ;
	m_PrevWindowStyle			=	_XQS_WINSTYLE_HORIZ;
	typechangebutton			=   NULL;

	m_ButtonPositionArray[0].x = 40; m_ButtonPositionArray[0].y = 7;
	m_ButtonPositionArray[1].x = 107; m_ButtonPositionArray[1].y = 2;
	m_ButtonPositionArray[2].x = 141; m_ButtonPositionArray[2].y = 2;
	m_ButtonPositionArray[3].x = 175; m_ButtonPositionArray[3].y = 2;
	m_ButtonPositionArray[4].x = 209; m_ButtonPositionArray[4].y = 2;
	m_ButtonPositionArray[5].x = 243; m_ButtonPositionArray[5].y = 2;
	m_ButtonPositionArray[6].x = 277; m_ButtonPositionArray[6].y = 2;
	m_ButtonPositionArray[7].x = 311; m_ButtonPositionArray[7].y = 2;
	m_ButtonPositionArray[8].x = 345; m_ButtonPositionArray[8].y = 2;
	m_ButtonPositionArray[9].x = 379; m_ButtonPositionArray[9].y = 2;

	m_bSelectedCheck = FALSE;
	m_SpeedCounter = 0;
	m_SelectedEffectFrame = 0.0f;
	m_SelectedEffectPrevFrame = 0;

	InitializeQuickSlotInfo();
	
	m_SkillIcon_Default		= NULL;
	m_SkillIcon_NangIn		= NULL;
	m_SkillIcon_GaeBang		= NULL;
	m_SkillIcon_BeeGoong	= NULL;
	m_SkillIcon_SoRim		= NULL;
	m_SkillIcon_NockRim		= NULL;
	
	m_SkillIcon_Default_g	= NULL;
	m_SkillIcon_NangIn_g	= NULL;
	m_SkillIcon_GaeBang_g	= NULL;
	m_SkillIcon_BeeGoong_g	= NULL;
	m_SkillIcon_SoRim_g		= NULL;
	m_SkillIcon_NockRim_g	= NULL;
	
	m_SelectImage = NULL;
	m_CooldownImage = NULL;

	// 2004.06.23->oneway48 insert
	m_pSkillExpGuage1 = NULL;
	m_pSkillExpGuage2 = NULL;
	m_pSkillExpGuage3 = NULL;
	m_pSkillExpGuage4 = NULL;
	m_pSkillExpGuage5 = NULL;
	m_pSkillExpGuage6 = NULL;
	m_pSkillExpGuage7 = NULL;
	m_pSkillExpGuage8 = NULL;
	m_pSkillExpGuage9 = NULL;
}

_XWindow_QSlot::~_XWindow_QSlot()
{
	DestroyWindow();
}

void _XWindow_QSlot::InitializeQuickSlotInfo( void )
{
	m_SelectedSetNumber = 0;
	m_SelectedSlotNumber = -1;
	
	for(int i = 0 ; i < 27 ; i++)// 2004.05.22->oneway48 modify
	{
		m_QuickSlot[i].iconid = 0;
		m_QuickSlot[i].slotnumber = -1; // 2004.06.10->oneway48 insert
		m_QuickSlot[i].used = FALSE;
		m_QuickSlot[i].bskillicon = FALSE;
	}
}

BOOL _XWindow_QSlot::Initialize( void )
{	
	// Horiz window ---------------------------------------------------------------------------------------------------
	m_HorizBorderStatic.Create(m_WindowPosition.x , 
								m_WindowPosition.y , 
								m_WindowPosition.x + 434, 
								m_WindowPosition.y + 46,
								&g_MainInterfaceTextureArchive, 
								g_MainInterfaceTextureArchive.FindResource( "quickslot_base.tga" ) );	
	m_HorizBorderStatic.SetClipRect( 0, 0, 434, 46 );

	int quickslotbuttonindex = g_MainInterfaceTextureArchive.FindResource( "quickslot_button.tga" );

	m_SelectImage = new _XImageStatic;
	m_SelectImage->Create(m_WindowPosition.x + 21, m_WindowPosition.y + 1, m_WindowPosition.x + 21 + 70, m_WindowPosition.y + 45, 
		&g_MainInterfaceTextureArchive, 
		g_MainInterfaceTextureArchive.FindResource("quickslot_effect.tga"));
	m_SelectImage->SetClipRect(0, 0, 70, 44);
	

	// prev/next slot button ------------------------------------------------------------------------------------------
	
	_XBTN_STRUCTURE prevbtnstruct = { TRUE, { 94, 3 }, { 10, 10}, _XDEF_QS_PREVBUTTON,
									quickslotbuttonindex,
									quickslotbuttonindex,
									quickslotbuttonindex, 
									&g_MainInterfaceTextureArchive };
	
	prevbutton = new _XButton;
	prevbutton->Create( prevbtnstruct );
	prevbutton->SetCallBackFunction_ToolTip( _XCallback_QS_Prev );
	prevbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  0, 0, 10, 10 );
	prevbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  10, 0, 20, 10 );
	prevbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  20, 0, 30, 10 );
	
	_XBTN_STRUCTURE nextbtnstruct = { TRUE, { 94, 33 }, { 10, 10 }, _XDEF_QS_NEXTBUTTON,
									quickslotbuttonindex,
									quickslotbuttonindex,
									quickslotbuttonindex, 
									&g_MainInterfaceTextureArchive };
	nextbutton = new _XButton;
	nextbutton->Create( nextbtnstruct );	
	nextbutton->SetCallBackFunction_ToolTip( _XCallback_QS_Next );
	nextbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  0, 10, 10, 20  );
	nextbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  10, 10, 20, 20  );
	nextbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  20, 10, 30, 20  );
	
	InsertChildObject( nextbutton );
	InsertChildObject( prevbutton );

	//2004.06.23->oneway48 insert 
	int gaugeimageindex = g_MainInterfaceTextureArchive.FindResource( "gaugebar_common.tga" );
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { m_WindowPosition.x + 108, m_WindowPosition.y + 36 }, { 30, 7 },	// 2004.05.22->oneway48 modify
				gaugeimageindex, -1, &g_MainInterfaceTextureArchive
	};
	
	m_pSkillExpGuage1 = new _XStickGauge;
	m_pSkillExpGuage1->Create( gaugestruct );	
	m_pSkillExpGuage1->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage1->SetGaugeFactor(0);
	m_pSkillExpGuage1->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage1 );
	
	gaugestruct.position.x += 34;	
	m_pSkillExpGuage2 = new _XStickGauge;
	m_pSkillExpGuage2->Create( gaugestruct );	
	m_pSkillExpGuage2->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage2->SetGaugeFactor(0);
	m_pSkillExpGuage2->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage2 );
	
	gaugestruct.position.x += 34;	
	m_pSkillExpGuage3 = new _XStickGauge;
	m_pSkillExpGuage3->Create( gaugestruct );	
	m_pSkillExpGuage3->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage3->SetGaugeFactor(0);
	m_pSkillExpGuage3->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage3 );
	
	gaugestruct.position.x += 34;	
	m_pSkillExpGuage4 = new _XStickGauge;
	m_pSkillExpGuage4->Create( gaugestruct );	
	m_pSkillExpGuage4->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage4->SetGaugeFactor(0);
	m_pSkillExpGuage4->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage4 );
	
	gaugestruct.position.x += 34;	
	m_pSkillExpGuage5 = new _XStickGauge;
	m_pSkillExpGuage5->Create( gaugestruct );	
	m_pSkillExpGuage5->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage5->SetGaugeFactor(0);
	m_pSkillExpGuage5->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage5 );
	
	gaugestruct.position.x += 34;	
	m_pSkillExpGuage6 = new _XStickGauge;
	m_pSkillExpGuage6->Create( gaugestruct );	
	m_pSkillExpGuage6->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage6->SetGaugeFactor(0);
	m_pSkillExpGuage6->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage6 );
	
	gaugestruct.position.x += 34;	
	m_pSkillExpGuage7 = new _XStickGauge;
	m_pSkillExpGuage7->Create( gaugestruct );	
	m_pSkillExpGuage7->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage7->SetGaugeFactor(0);
	m_pSkillExpGuage7->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage7 );
	gaugestruct.position.x += 34;	
	
	m_pSkillExpGuage8 = new _XStickGauge;
	m_pSkillExpGuage8->Create( gaugestruct );	
	m_pSkillExpGuage8->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage8->SetGaugeFactor(0);
	m_pSkillExpGuage8->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage8 );
	
	gaugestruct.position.x += 34;	
	m_pSkillExpGuage9 = new _XStickGauge;
	m_pSkillExpGuage9->Create( gaugestruct );	
	m_pSkillExpGuage9->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0, 18, 30, 25 );
	m_pSkillExpGuage9->SetGaugeFactor(0);
	m_pSkillExpGuage9->SetFactorDraw(FALSE);
	InsertChildObject( m_pSkillExpGuage9 );

	// Set quick slot previous setting---------------------------------------------------------------------------------

	m_WindowStyle = (_XQSWindowStyle)g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_QUICKSLOTSTYLE, 
												   _XQS_WINSTYLE_HORIZ, TRUE );
	m_WindowStyle = _XQS_WINSTYLE_HORIZ;
	SetQSWindowStyle( m_WindowStyle );

	//BOOL quickslotflag  = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWQUICKSLOT, TRUE, TRUE );
	//ShowWindow( quickslotflag );
	ShowWindow( TRUE );
	
	int  xpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_QUICKSLOTXPOS, m_WindowPosition.x, TRUE );
	int  ypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_QUICKSLOTYPOS, m_WindowPosition.y, TRUE );
	if( xpos < 0 ) xpos = 0;
	if( xpos >= gnWidth-m_WindowSize.cx ) xpos = gnWidth-m_WindowSize.cx;
	if( ypos >= gnHeight-m_WindowSize.cy ) ypos = gnHeight-m_WindowSize.cy;
	MoveWindow( xpos, ypos );

	CheckOutside();

	// Icon -----------------------------------------------------------------------------------------------------------
	SAFE_DELETE( m_SkillIcon_Default );
	SAFE_DELETE( m_SkillIcon_NangIn );
	SAFE_DELETE( m_SkillIcon_GaeBang );
	SAFE_DELETE( m_SkillIcon_BeeGoong );
	SAFE_DELETE( m_SkillIcon_SoRim );
	SAFE_DELETE( m_SkillIcon_NockRim );

	SAFE_DELETE( m_SkillIcon_Default_g );
	SAFE_DELETE( m_SkillIcon_NangIn_g );
	SAFE_DELETE( m_SkillIcon_GaeBang_g );
	SAFE_DELETE( m_SkillIcon_BeeGoong_g );
	SAFE_DELETE( m_SkillIcon_SoRim_g );
	SAFE_DELETE( m_SkillIcon_NockRim_g );
	
	m_SkillIcon_Default = new _XImageStatic;
	m_SkillIcon_Default->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_10.tga"));
	m_SkillIcon_NangIn = new _XImageStatic;
	m_SkillIcon_NangIn->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_00.tga"));
	m_SkillIcon_GaeBang = new _XImageStatic;
	m_SkillIcon_GaeBang->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_01.tga"));
	m_SkillIcon_BeeGoong = new _XImageStatic;
	m_SkillIcon_BeeGoong->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_02.tga"));
	m_SkillIcon_SoRim = new _XImageStatic;
	m_SkillIcon_SoRim->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_03.tga"));
	m_SkillIcon_NockRim = new _XImageStatic;
	m_SkillIcon_NockRim->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_04.tga"));

	m_SkillIcon_Default_g = new _XImageStatic;
	m_SkillIcon_Default_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_10.tga"));
	m_SkillIcon_NangIn_g = new _XImageStatic;
	m_SkillIcon_NangIn_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_00.tga"));
	m_SkillIcon_GaeBang_g = new _XImageStatic;
	m_SkillIcon_GaeBang_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_01.tga"));
	m_SkillIcon_BeeGoong_g = new _XImageStatic;
	m_SkillIcon_BeeGoong_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_02.tga"));
	m_SkillIcon_SoRim_g = new _XImageStatic;
	m_SkillIcon_SoRim_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_03.tga"));
	m_SkillIcon_NockRim_g = new _XImageStatic;
	m_SkillIcon_NockRim_g->Create(5, 5, 32, 32, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_04.tga"));

	// Cooldown image --------------------------------------------------------------------------------------------------
	SAFE_DELETE(m_CooldownImage);

	m_CooldownImage = new _XImageStatic;
	m_CooldownImage->Create(0, 0, 30, 30, 
		&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("skill_gauge.tga"));
	
	m_SelectedSkillID = 0;
	m_ActiveSlotID = 0;
	m_bNotYetUse = FALSE;
			
	return TRUE;
}


void _XWindow_QSlot::DestroyWindow( void )
{	
	SAFE_DELETE( m_SkillIcon_Default );
	SAFE_DELETE( m_SkillIcon_NangIn );
	SAFE_DELETE( m_SkillIcon_GaeBang );
	SAFE_DELETE( m_SkillIcon_BeeGoong );
	SAFE_DELETE( m_SkillIcon_SoRim );
	SAFE_DELETE( m_SkillIcon_NockRim );

	SAFE_DELETE( m_SkillIcon_Default_g );
	SAFE_DELETE( m_SkillIcon_NangIn_g );
	SAFE_DELETE( m_SkillIcon_GaeBang_g );
	SAFE_DELETE( m_SkillIcon_BeeGoong_g );
	SAFE_DELETE( m_SkillIcon_SoRim_g );
	SAFE_DELETE( m_SkillIcon_NockRim_g );

	SAFE_DELETE(m_SelectImage);
	SAFE_DELETE(m_CooldownImage);

	_XWindow::DestroyWindow();
}

void _XWindow_QSlot::SetDefaultPosition( void )
{	
	SetQSWindowStyle( _XQS_WINSTYLE_HORIZ );
	MoveWindow( 0, gnHeight-46 );
	CheckOutside();
}

void _XWindow_QSlot::SaveWindowProperty( void )
{
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWQUICKSLOT, GetShowStatus() );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_QUICKSLOTSTYLE,m_WindowStyle );		
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_QUICKSLOTXPOS, m_WindowPosition.x );	
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_QUICKSLOTYPOS, m_WindowPosition.y );	
}

void _XWindow_QSlot::MoveWindow( int X, int Y )
{				
	m_SelectImage->MoveWindow(X + 21, Y + 1 );
	_XWindow::MoveWindow( X, Y );
	
	m_HorizBorderStatic.MoveWindow(m_WindowPosition.x, m_WindowPosition.y );
}


void _XWindow_QSlot::Draw( _XGUIObject*& pfocusobject )
{	
	if( this->m_ShowWindow && m_WindowAnimationType == _XW_ANIMTYPE_NONE )
	{		
		m_HorizBorderStatic.DrawWithRegion();// 2004.05.14->oneway48 insert
		DrawSlotButtons();
	}
	
	_XWindow::Draw(pfocusobject);

	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
		
	if( m_bSelectedCheck )
	{
		if( m_SpeedCounter == 0 ) m_SpeedCounter = g_LocalSystemTime;
		int elapsedtimetick = g_LocalSystemTime - m_SpeedCounter;
		m_SpeedCounter = g_LocalSystemTime;
		
		m_SelectedEffectFrame += ((FLOAT)elapsedtimetick / 50.0f);
		int selectedframe = (int)m_SelectedEffectFrame; 
		if( selectedframe > 4 )
		{		
			selectedframe = 0;
			m_bSelectedCheck = FALSE;
			m_SelectedEffectFrame = 0 ;// 2004.05.21->oneway48 insert
			m_SpeedCounter = 0;// 2004.05.21->oneway48 insert
		}
		
		if( m_SelectedEffectPrevFrame != selectedframe )
		{	
			RECT	cliprect;
			int		column, row;
			column = selectedframe % 2;
			row	   = selectedframe / 2;
			cliprect.left  = column * 70;
			cliprect.right = cliprect.left+70;
			cliprect.top  = row * 44;
			cliprect.bottom = cliprect.top+44;
			m_SelectImage->SetClipRect( cliprect );
		}

		m_SelectedEffectPrevFrame = selectedframe;
		
		m_SelectImage->DrawWithRegion();
	}

	// 2004.05.21->oneway48 insert
	g_XBaseFont->SetColor( _XSC_DEFAULT );
	g_XBaseFont->Print( m_WindowPosition.x + 96, m_WindowPosition.y +18, 1.0f, "%d", m_SelectedSetNumber+1 );
	g_XBaseFont->Flush();
	// insert end

	DrawSkillIcon();
	DrawCooldownImage();

	DrawSelectedIconBorder();
	
	DrawSelectedSkillIcon();	
}

BOOL _XWindow_QSlot::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
			
	MouseState* mousestate = gpInput->GetMouseState();

	if(pfocusobject == this)
	{
		if(!mousestate->bButton[0])
		{
			// 마우스 클릭 처리
			int index = 0;
			short iconid = 0;
			bool bskillicon = FALSE;
			if(CheckUsedSlotNumber(index, iconid, bskillicon))
			{				
				if(bskillicon)
				{
					switch(index)
					{
					case 0 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT1BUTTON, iconid);
						break;
					case 1 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT2BUTTON, iconid);
						break;
					case 2 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT3BUTTON, iconid);
						break;
					case 3 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT4BUTTON, iconid);
						break;
					case 4 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT5BUTTON, iconid);
						break;
					case 5 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT6BUTTON, iconid);
						break;
					case 6 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT7BUTTON, iconid);
						break;
					case 7 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT8BUTTON, iconid);
						break;
					case 8 :
						PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT9BUTTON, iconid); // 2004.05.21->oneway48 insert
						break;
					}
				}
				else
				{
					int inventoryslotnumber = m_QuickSlot[(m_SelectedSetNumber*9) + index].slotnumber;
					
					if(g_pLocalUser->m_UserItemList[inventoryslotnumber].m_sID > 0)
					{
						if( g_pLocalUser->m_UserItemList[inventoryslotnumber].m_cType == _XGI_FC_POTION )
						{
							switch(g_PotionItemProperty[iconid].cSecondType) 
							{
							case 0://내상 회복제
								{
									if( m_pInventory->m_bUsingPotionItem)
									{
										g_NetworkKernel.InsertChatString("먼저 사용한 회복제의 사용을 끝내고 사용하여 주십시요.", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
									}
									else
									{
										m_SelectedSlotNumber = index;
										g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, inventoryslotnumber);	
									}	
								}
								break;
							case 1://외상 회복제
								{
									if( m_pInventory->m_bUsingPotionItem )
									{
										g_NetworkKernel.InsertChatString("먼저 사용한 회복제의 사용을 끝내고 사용하여 주십시요.", _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
									}
									else
									{
										m_SelectedSlotNumber = index;
										g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, inventoryslotnumber);		
									}
								}
								break;
							case 2://피로 회복제
								break;
							}
							
						}				
					}
				}

			}
		}
	}
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
				
	return TRUE;
}


void _XWindow_QSlot::DrawSlotButtons( void )
{
	switch( m_WindowStyle )
	{
	case _XQS_WINSTYLE_HORIZ :
		break;
	case _XQS_WINSTYLE_VERTLEFT :
		break;
	case _XQS_WINSTYLE_VERTRIGHT :
		break;
	case _XQS_WINSTYLE_FLOATING :
		break;
	}
}

void _XWindow_QSlot::SetQSWindowStyle( _XQSWindowStyle windowstyle )
{	
	m_WindowStyle = windowstyle;	

	if( m_WindowStyle != _XQS_WINSTYLE_FLOATING ) m_PrevWindowStyle = m_WindowStyle;

	if( windowstyle == _XQS_WINSTYLE_HORIZ )
	{		
		m_solidborderstatic.MoveWindow( m_solidborderstatic.m_fLeft, m_solidborderstatic.m_fTop,
								        m_solidborderstatic.m_fLeft+434, m_solidborderstatic.m_fTop+46 );
		
		m_WindowPosition.x = m_solidborderstatic.m_fLeft;
		m_WindowPosition.y = m_solidborderstatic.m_fTop-1;	// 에지 한라인을 지우기 위해서... _XWindow::MoveWindow쪽 참고 
		m_WindowSize.cx = 434;
		m_WindowSize.cy = 46;
				
		if( typechangebutton )
		{			
			typechangebutton->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 7, m_WindowPosition.y+1 );	
		}
			
		if( prevbutton )
		{			
			prevbutton->MoveWindow( m_WindowPosition.x+94, m_WindowPosition.y+3 );
			prevbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  0, 0, 10, 10 );
			prevbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  10, 0, 20, 10 );
			prevbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  20, 0, 30, 10 );			
			prevbutton->SetWindowSize( 10, 10 );
		}
		if( nextbutton )
		{			
			nextbutton->MoveWindow( m_WindowPosition.x+94, m_WindowPosition.y+33 );
			nextbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  0, 10, 10, 20  );
			nextbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  10, 10, 20, 20  );
			nextbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  20, 10, 30, 20  );
			nextbutton->SetWindowSize( 10, 10 );
		}		

		m_SelectImage->MoveWindow(m_WindowPosition.x + 21, m_WindowPosition.y + 1); // 2004.05.14->oneway48 modify
	}
}

void _XWindow_QSlot::SetQSWindowNextStyle( void )
{	
	if( m_WindowStyle != _XQS_WINSTYLE_FLOATING ) 
	{
		SetQSWindowStyle( _XQS_WINSTYLE_FLOATING );
	}
	else
	{
		SetQSWindowStyle( m_PrevWindowStyle );
	}	

	CheckOutside();
}

BOOL _XWindow_QSlot::CheckSlotNumber(BOOL bEmpty)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int		index = 0;
	BOOL	result = FALSE;

	if(scrnpos->x > (m_WindowPosition.x + m_ButtonPositionArray[0].x) &&
		scrnpos->x < (m_WindowPosition.x + m_ButtonPositionArray[0].x + 32) &&
		scrnpos->z > (m_WindowPosition.y + m_ButtonPositionArray[0].y) &&
		scrnpos->z < (m_WindowPosition.y + m_ButtonPositionArray[0].y + 32))
	{
		_XPlayInterfaceSound(ID_SR_INTERFACE_INTERFACE_ERROR01_WAV);
		return FALSE;
	}

	for(int i = 0 ; i < 9 ; i++)// 2004.05.21->oneway48 modify
	{
		if( scrnpos->x > (m_WindowPosition.x + m_ButtonPositionArray[i+1].x) &&
			scrnpos->x < (m_WindowPosition.x + m_ButtonPositionArray[i+1].x + 32) &&
			scrnpos->z > (m_WindowPosition.y + m_ButtonPositionArray[i+1].y) &&
			scrnpos->z < (m_WindowPosition.y + m_ButtonPositionArray[i+1].y + 32))
		{
			if(bEmpty)
			{
				if(!m_QuickSlot[(m_SelectedSetNumber*9) + i].used)// 2004.05.21->oneway48 modify
				{
					m_SelectedSlotNumber = i;
					return TRUE;
				}
			}
			else
			{
				if(m_QuickSlot[(m_SelectedSetNumber*9) + i].used)// 2004.05.21->oneway48 modify
				{
					m_SelectedSlotNumber = i;	
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


extern BOOL __stdcall _XSkillSelectCheckCallBack( int param1, int param2 );

BOOL _XWindow_QSlot::CheckUsedSlotNumber(int& index, short& iconid, bool& bskillicon)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		
	for(int i = 0 ; i < 9 ; i++)// 2004.05.21->oneway48 modify
	{
		if(scrnpos->x > (m_WindowPosition.x + m_ButtonPositionArray[i+1].x) &&
			scrnpos->x < (m_WindowPosition.x + m_ButtonPositionArray[i+1].x + 32) &&
			scrnpos->z > (m_WindowPosition.y + m_ButtonPositionArray[i+1].y) &&
			scrnpos->z < (m_WindowPosition.y + m_ButtonPositionArray[i+1].y + 32))
		{
			if(m_QuickSlot[(m_SelectedSetNumber*9) + i].used)// 2004.05.21->oneway48 modify
			{
				if(m_bNotYetUse)
				{
					m_bNotYetUse = FALSE;
					return FALSE;
				}

				if( m_QuickSlot[(m_SelectedSetNumber*9) + i].bskillicon ) // 스킬 아이콘일때 
				{
					if(_XSkillSelectCheckCallBack(0, m_QuickSlot[(m_SelectedSetNumber*9) + i].iconid))// 2004.05.21->oneway48 modify
					{
						bskillicon = TRUE;
						iconid = m_QuickSlot[(m_SelectedSetNumber*9) + i].iconid;// 2004.05.21->oneway48 modify
						index = i;
						return TRUE;
					}
				}
				else
				{
					bskillicon = FALSE;
					iconid = m_QuickSlot[(m_SelectedSetNumber*9) + i].iconid;
					index = i;
					return TRUE;
				}

			} // end if(퀵슬롯에 있으면)
			else
			{
				// 빈슬롯이면 선택무공 삭제
				DeleteSelectedSkill();
				return FALSE;
			}
		}
	}
	return FALSE;
}

BOOL _XWindow_QSlot::CheckUsedKeySlotNumber(int index, short& iconid, bool& bskillicon)
{	
	if(	g_pLocalUser->GetMotionClass() != _XACTION_DYING	&&
		g_pLocalUser->GetMotionClass() != _XACTION_DIE		&& 
		g_pLocalUser->GetMotionClass() != _XACTION_REBIRTH  && 
		g_pLocalUser->m_CharacterInfo.current_lifepower > 0 ) 
	{
		if(m_QuickSlot[(m_SelectedSetNumber*9) + index].used)// 2004.05.21->oneway48 modify
		{
			if( m_QuickSlot[(m_SelectedSetNumber*9) + index].bskillicon ) // 스킬 아이콘일때 
			{
				if( _XSkillSelectCheckCallBack( 0, m_QuickSlot[(m_SelectedSetNumber*9) + index].iconid  ) )// 2004.05.21->oneway48 modify
				{
					bskillicon = TRUE;
					iconid = m_QuickSlot[(m_SelectedSetNumber*9) + index].iconid;// 2004.05.21->oneway48 modify;
					return TRUE;
				}
				else 
				{
					iconid = 0;
					return FALSE;
				}

			}
			else // 아이템 아이콘일 때 
			{
				bskillicon = FALSE;
				iconid = m_QuickSlot[(m_SelectedSetNumber*9) + index].iconid;
				return TRUE;

			}
		}
		else
		{
			iconid = 0;
			return FALSE;
		}
	}
	return FALSE;
}

void _XWindow_QSlot::SetSelectedSkillIcon(int index, short iconid)
{
	m_SelectedSlotNumber = index;
	m_SelectedSkillID = iconid;

	if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
	{
		m_ActiveSlotID = m_SelectedSkillID;
		m_bSelectedCheck = TRUE; // 2004.05.25->oneway48 move : Active Skill일때만 반짝거림.
	}
}


void _XWindow_QSlot::SetSkillToQSlot(short iconid)
{
	m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].bskillicon = TRUE;
	m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].used = TRUE;// 2004.05.22->oneway48 modify
	m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].iconid = iconid;// 2004.05.22->oneway48 modify
	m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].slotnumber = -1;

	m_bNotYetUse = TRUE;
}

// 2004.06.10->oneway48 insert
void _XWindow_QSlot::SetItemToQSlot(short slotnumber)
{
	
	m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].bskillicon = FALSE;
	m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].used = TRUE;
	m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].slotnumber = slotnumber;
	m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].iconid = g_pLocalUser->m_UserItemList[slotnumber].m_sID;
	
	m_bNotYetUse = TRUE;
}

void _XWindow_QSlot::GenerateIconRect(short iconid, RECT& rect, BOOL& nextresource)
{	
	short		iconskillid = 0;
	nextresource = FALSE;
	
	if(iconid > 0)
	{
		iconskillid = iconid - (g_SkillProperty[iconid]->clanType*100);
		if(iconskillid > 64)
		{
			iconskillid -= 64;
			nextresource = TRUE;
		}
	}
	else
	{
		nextresource = FALSE;
		if(g_pLocalUser->m_CharacterInfo.modeldescript.item1 == 0)
		{
			iconskillid = 1;
		}
		else
		{
			switch(g_WeaponItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.item1].cSecondType)
			{
			case _XGI_SC_WEAPON_SHORT :
				{
					if(g_WeaponItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST)
						iconskillid = 1;
					else
						iconskillid = 2;
				}
				break;
			case _XGI_SC_WEAPON_LONG :
				{
					iconskillid = 3;
				}
				break;
			case _XGI_SC_WEAPON_SOFT :
			case _XGI_SC_WEAPON_HIDDEN :
			case _XGI_SC_WEAPON_MUSICAL :
			case _XGI_SC_WEAPON_SPECIAL :
				{
					iconskillid = 3;		// 임시
				}
				break;
			}
		} // else
	}

	rect.left	= ((iconskillid - 1) %8) * 32;
	rect.right	= ((iconskillid - 1) %8 + 1) * 32;
	rect.top	= ((iconskillid - 1) /8) * 32;
	rect.bottom	= ((iconskillid - 1) /8 + 1) * 32;
}

void _XWindow_QSlot::DrawSkillIcon(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	for(int i = 0 ; i < 9 ; i++)
	{
		int index = (m_SelectedSetNumber*9) + i;// 2004.05.22->oneway48 modify
		if(m_QuickSlot[index].used)
		{
			RECT rect;
			POINT position;
			position.x = m_WindowPosition.x + m_ButtonPositionArray[i+1].x;
			position.y = m_WindowPosition.y + m_ButtonPositionArray[i+1].y;

			BOOL nextresource;
			if( !m_QuickSlot[index].bskillicon )
			{
				m_pInventory->DrawIcon(TRUE,position.x, position.y, &g_pLocalUser->m_UserItemList[m_QuickSlot[index].slotnumber]);
				
				if( scrnpos->x > (m_WindowPosition.x + m_ButtonPositionArray[i+1].x) &&
					scrnpos->x < (m_WindowPosition.x + m_ButtonPositionArray[i+1].x + 32) &&
					scrnpos->z > (m_WindowPosition.y + m_ButtonPositionArray[i+1].y) &&
					scrnpos->z < (m_WindowPosition.y + m_ButtonPositionArray[i+1].y + 32))
				{
					
					char cType = g_pLocalUser->m_UserItemList[m_QuickSlot[index].slotnumber].m_cType;
					short sID = g_pLocalUser->m_UserItemList[m_QuickSlot[index].slotnumber].m_sID;
					
					TCHAR cName[13];
					memset(cName, 0, sizeof(TCHAR)*13);
					strncpy(cName, _XGameItem::GetItemName(cType, sID), 12);
					
					g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x + m_ButtonPositionArray[i+1].x + 32, 
						m_WindowPosition.y + m_ButtonPositionArray[i+1].y - 10, cName, _XSC_INFORMATION );
				}
				
			}
			else// 2004.06.10->oneway48 insert : slotnumber가 -1일때는 무공
			{
				int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_QuickSlot[index].iconid);

				GenerateIconRect(m_QuickSlot[index].iconid, rect, nextresource);
				switch(g_SkillProperty[m_QuickSlot[index].iconid]->clanType)
				{
				case _XGROUP_NANGIN :
					if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
						m_SkillIcon_NangIn_g->DrawWithRegion(position.x, position.y, rect);
					else
						m_SkillIcon_NangIn->DrawWithRegion(position.x, position.y, rect);
					break;
					
				case _XGROUP_GAEBANG :
					if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
						m_SkillIcon_GaeBang_g->DrawWithRegion(position.x, position.y, rect);
					else
						m_SkillIcon_GaeBang->DrawWithRegion(position.x, position.y, rect);
					break;

				case _XGROUP_BEEGOONG :
					if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
						m_SkillIcon_BeeGoong_g->DrawWithRegion(position.x, position.y, rect);
					else
						m_SkillIcon_BeeGoong->DrawWithRegion(position.x, position.y, rect);
					break;
					
				case _XGROUP_SORIM :
					if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
						m_SkillIcon_SoRim_g->DrawWithRegion(position.x, position.y, rect);
					else
						m_SkillIcon_SoRim->DrawWithRegion(position.x, position.y, rect);
					break;

				case _XGROUP_NOCKRIM :
					if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
						m_SkillIcon_NockRim_g->DrawWithRegion(position.x, position.y, rect);
					else
						m_SkillIcon_NockRim->DrawWithRegion(position.x, position.y, rect);
					break;
					
				case _XGROUP_MOODANG :
					break;

				case _XGROUP_MAKYO :
					break;

				case _XGROUP_SEGA :
					break;

				case _XGROUP_NOFIXED :
					break;

				default :
					break;
				}
				
				if( scrnpos->x > (m_WindowPosition.x + m_ButtonPositionArray[i+1].x) &&
					scrnpos->x < (m_WindowPosition.x + m_ButtonPositionArray[i+1].x + 32) &&
					scrnpos->z > (m_WindowPosition.y + m_ButtonPositionArray[i+1].y) &&
					scrnpos->z < (m_WindowPosition.y + m_ButtonPositionArray[i+1].y + 32))
				{
					g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x + m_ButtonPositionArray[i+1].x + 32, 
														 m_WindowPosition.y + m_ButtonPositionArray[i+1].y - 10, 
														 g_SkillProperty[m_QuickSlot[index].iconid]->skillName, _XSC_INFORMATION );
				}

				int skillindex = g_pLocalUser->GetLearnedSkillIndex(m_QuickSlot[index].iconid);

				FLOAT exp = 0.0f;
				if(g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel >= 11)
				{
					exp = (FLOAT)g_SkillProperty[m_QuickSlot[index].iconid]->detailInfo[g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel].skillExp;
				}
				else
				{
					exp = (FLOAT)g_SkillProperty[m_QuickSlot[index].iconid]->detailInfo[g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel+1].skillExp;
				}
				FLOAT gaugefactor = ((FLOAT)g_pLocalUser->m_SkillList[skillindex].m_iSkillExp / (FLOAT)exp) * 100;
				
				switch(i) {
				case 0:
					{
						m_pSkillExpGuage1->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage1->SetGaugeFactor(gaugefactor);
					}
					break;
				case 1:
					{
						m_pSkillExpGuage2->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage2->SetGaugeFactor(gaugefactor);
					}
					break;
				case 2:
					{
						m_pSkillExpGuage3->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage3->SetGaugeFactor(gaugefactor);
					}
					break;
				case 3:
					{
						m_pSkillExpGuage4->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage4->SetGaugeFactor(gaugefactor);
					}
					break;
				case 4:
					{
						m_pSkillExpGuage5->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage5->SetGaugeFactor(gaugefactor);
					}
					break;
				case 5:
					{
						m_pSkillExpGuage6->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage6->SetGaugeFactor(gaugefactor);
					}
					break;
				case 6:
					{
						m_pSkillExpGuage7->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage7->SetGaugeFactor(gaugefactor);
					}
					break;
				case 7:
					{
						m_pSkillExpGuage8->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage8->SetGaugeFactor(gaugefactor);
					}
					break;
				case 8:
					{
						m_pSkillExpGuage9->MoveWindow(m_WindowPosition.x + 108 + i*34, m_WindowPosition.y + 36);
						m_pSkillExpGuage9->SetGaugeFactor(gaugefactor);
					}
					break;
				}
				
				g_XSmallFont.SetColor( _XSC_DEFAULT );
				g_XSmallFont.PrintAlign( m_WindowPosition.x + m_ButtonPositionArray[i+1].x + 15,
					m_WindowPosition.y + m_ButtonPositionArray[i+1].y + 33, 
					1.0f,  _XFONT_ALIGNTYPE_CENTER, "%d", g_pLocalUser->m_SkillList[skillindex].m_cSkillLevel+1 );
				g_XSmallFont.Flush();			
			}
		}
		else
		{
			
			switch(i) {
			case 0:
				m_pSkillExpGuage1->SetGaugeFactor(0);
				break;
			case 1:
				m_pSkillExpGuage2->SetGaugeFactor(0);
				break;
			case 2:
				m_pSkillExpGuage3->SetGaugeFactor(0);
				break;
			case 3:
				m_pSkillExpGuage4->SetGaugeFactor(0);
				break;
			case 4:
				m_pSkillExpGuage5->SetGaugeFactor(0);
				break;
			case 5:
				m_pSkillExpGuage6->SetGaugeFactor(0);
				break;
			case 6:
				m_pSkillExpGuage7->SetGaugeFactor(0);
				break;
			case 7:
				m_pSkillExpGuage8->SetGaugeFactor(0);
				break;
			case 8:
				m_pSkillExpGuage9->SetGaugeFactor(0);
				break;
			}
			
		}
	}
}

void _XWindow_QSlot::DrawSelectedIconBorder(void)
{
	if(m_SelectedSlotNumber == -1)
		return;

	if(!m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].used)// 2004.05.22->oneway48 modify
		return ;

	FLOAT left = (FLOAT)(m_WindowPosition.x + m_ButtonPositionArray[m_SelectedSlotNumber+1].x - 1);
	FLOAT right = (FLOAT)(m_WindowPosition.x + m_ButtonPositionArray[m_SelectedSlotNumber+1].x + 32);
	FLOAT top = (FLOAT)(m_WindowPosition.y + m_ButtonPositionArray[m_SelectedSlotNumber+1].y - 1);
	FLOAT bottom = (FLOAT)(m_WindowPosition.y + m_ButtonPositionArray[m_SelectedSlotNumber+1].y + 32);
	FLOAT height = 0.0f;
	_XDrawRectAngle(left, top, right, bottom, height, D3DCOLOR_ARGB(255, 255, 255, 255));

}

void _XWindow_QSlot::DeleteSelectedSlot(void)
{
	if( m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].bskillicon )
	{
		// 선택된 무공이면 선택된 무공 삭제
		if(m_SelectedSkillID != 0 && m_SelectedSkillID == m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].iconid)// 2004.05.22->oneway48 modify
		{
			DeleteSelectedSkill();
		}

		m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].used = FALSE;// 2004.05.22->oneway48 modify
		m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].iconid = 0;// 2004.05.22->oneway48 modify
		m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].slotnumber = -1;

		m_SelectedSlotNumber = -1;
	}
	else
	{
		m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].used = FALSE;// 2004.05.22->oneway48 modify
		m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].iconid = 0;// 2004.05.22->oneway48 modify
		m_QuickSlot[(m_SelectedSetNumber*9) + m_SelectedSlotNumber].slotnumber = -1;
		
		m_SelectedSlotNumber = -1;
	}
}

int _XWindow_QSlot::FindSkillFromQSlot(short iconid)
{
	for(int i = 0 ; i < 27 ; i++)
	{
		if(m_QuickSlot[i].iconid == iconid && m_QuickSlot[i].used)
		{
			// 2004.05.22->oneway48 modify
			m_SelectedSetNumber = i / 9;
			m_SelectedSlotNumber = i % 9;
			return i;
		}
	}
	m_SelectedSlotNumber = -1;

	return -1;
}



void _XWindow_QSlot::DrawSelectedSkillIcon(void)
{
	RECT rect;
	BOOL nextresource;
	short iconid;

	// Active skill일때만 선택 icon drawing 한다
	if(g_SkillProperty[m_SelectedSkillID]->usefulType != _XSI_USEFULTYPE_ACTIVESKILL)
	{
		iconid = m_ActiveSlotID;
	}
	else if(m_SelectedSkillID == 0)
	{
		iconid = m_SelectedSkillID;
	}
	else
	{
		iconid = m_SelectedSkillID;
	}

	if(iconid > 0)
	{	
		GenerateIconRect(iconid, rect, nextresource);
		switch(g_SkillProperty[iconid]->clanType)
		{
		case _XGROUP_NANGIN :
			{
				if(g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(iconid)].m_ucSkillState == 1)
					m_SkillIcon_NangIn_g->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
				else
					m_SkillIcon_NangIn->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
			}
			break;
		case _XGROUP_GAEBANG :
			{
				if(g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(iconid)].m_ucSkillState == 1)
					m_SkillIcon_GaeBang_g->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
				else
					m_SkillIcon_GaeBang->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
			}
			break;
					
		case _XGROUP_BEEGOONG :
			{
				if(g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(iconid)].m_ucSkillState == 1)
					m_SkillIcon_BeeGoong_g->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
				else
					m_SkillIcon_BeeGoong->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
			}
			break;

		case _XGROUP_SORIM :
			{
				if(g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(iconid)].m_ucSkillState == 1)
					m_SkillIcon_SoRim_g->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
				else
					m_SkillIcon_SoRim->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
			}
			break;
		case _XGROUP_NOCKRIM :
			{
				if(g_pLocalUser->m_SkillList[g_pLocalUser->GetLearnedSkillIndex(iconid)].m_ucSkillState == 1)
					m_SkillIcon_NockRim_g->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
				else
					m_SkillIcon_NockRim->DrawWithRegion( m_WindowPosition.x + m_ButtonPositionArray[0].x, 
					m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
			}
			break;
		case _XGROUP_MOODANG :
			break;
		case _XGROUP_MAKYO :
			break;
		case _XGROUP_SEGA :
			break;
		case _XGROUP_NOFIXED :
			break;
			
		default :
			break;
		}

		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

		if( scrnpos->x > (m_WindowPosition.x + m_ButtonPositionArray[0].x) &&
			scrnpos->x < (m_WindowPosition.x + m_ButtonPositionArray[0].x + 32) &&
			scrnpos->z > (m_WindowPosition.y + m_ButtonPositionArray[0].y) &&
			scrnpos->z < (m_WindowPosition.y + m_ButtonPositionArray[0].y + 32))
		{
			g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x + m_ButtonPositionArray[0].x + 32, 
												 m_WindowPosition.y + m_ButtonPositionArray[0].y - 10, 
												 g_SkillProperty[iconid]->skillName, _XSC_INFORMATION );
		}
	}
	else
	{
		GenerateIconRect(iconid, rect, nextresource);
		m_SkillIcon_Default->DrawWithRegion(m_WindowPosition.x + m_ButtonPositionArray[0].x, 
			m_WindowPosition.y + m_ButtonPositionArray[0].y, rect);
	}
}

BOOL _XWindow_QSlot::CheckSelectedIconClick(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		
	if( scrnpos->x > m_WindowPosition.x + m_ButtonPositionArray[0].x &&
		scrnpos->x < m_WindowPosition.x + m_ButtonPositionArray[0].x + 32 &&
		scrnpos->z > m_WindowPosition.y + m_ButtonPositionArray[0].y &&
		scrnpos->z < m_WindowPosition.y + m_ButtonPositionArray[0].y + 32)
	{
		if(m_SelectedSkillID != 0)
			return TRUE;
	}
	return FALSE;
}

void _XWindow_QSlot::DrawSelectedSelectedIconBorder(void)
{
	FLOAT left   = (FLOAT)(m_WindowPosition.x + m_ButtonPositionArray[0].x - 1 );
	FLOAT right  = (FLOAT)(m_WindowPosition.x + m_ButtonPositionArray[0].x + 32);
	FLOAT top    = (FLOAT)(m_WindowPosition.y + m_ButtonPositionArray[0].y - 1 );
	FLOAT bottom = (FLOAT)(m_WindowPosition.y + m_ButtonPositionArray[0].y + 32);
	FLOAT height = 0.0f;
	
	_XDrawRectAngle(left, top, right, bottom, height, D3DCOLOR_ARGB(255, 255, 255, 255));
}

// 선택 무공 삭제
void _XWindow_QSlot::DeleteSelectedSkill(void)
{
	m_SelectedSkillID = 0;
	g_pLocalUser->m_SelectedSkillID = 0;
	g_pLocalUser->m_SelectedSkillItem = NULL;

	g_pLocalUser->UpdateLocalUserAttackLevel();
	g_NetworkKernel.SendPacket( MSG_NO_SKILL_SELECT );

	if(g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
	{
		m_ActiveSlotID = 0;
		g_pLocalUser->m_ActiveSlotID = 0;

		// 무공 기본으로
		if( g_pLocalUser->m_CharacterInfo.modeldescript.item1 == 0 )// 아무것도 안들은 경우 
		{								
			g_pLocalUser->m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
		}
		else
		{
			switch( g_WeaponItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.item1].cSecondType )
			{				
			case _XGI_SC_WEAPON_SHORT				:				// 단병기
				if( g_WeaponItemProperty[g_pLocalUser->m_CharacterInfo.modeldescript.item1].cThirdType == _XGI_TC_WEAPON_FIST )
				{
					g_pLocalUser->m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;										
				}
				else
				{
					g_pLocalUser->m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_SHORTWEAPON;
				}
				break;
			case _XGI_SC_WEAPON_LONG				:				// 장병기
				g_pLocalUser->m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_LONGWEAPON;									
				break;

			case _XGI_SC_WEAPON_SOFT				:				// 연병기
				g_pLocalUser->m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
				break;

			case _XGI_SC_WEAPON_HIDDEN				:				// 암병기
				g_pLocalUser->m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
				break;

			case _XGI_SC_WEAPON_MUSICAL				:				// 악병기
				g_pLocalUser->m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
				break;

			case _XGI_SC_WEAPON_SPECIAL				:				// 특수병기
				g_pLocalUser->m_CharacterInfo.animationdescript.attacktype = _XAN_NI_ATTACK_KK;
				break;
			}				
		}						
		
		if(g_pLocalUser->GetMotionClass() == _XACTION_ATTACK)
		{
			g_pLocalUser->m_ModelDescriptor.SetCurMotion( g_pLocalUser->m_CharacterInfo.animationdescript.attacktype, TRUE);
		}
		else if(g_pLocalUser->GetMotionClass() == _XACTION_IDLE)
		{
			g_pLocalUser->SetAnimation(_XACTION_IDLE);
		}
	}
}

void _XWindow_QSlot::DrawCooldownImage(void)
{
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID == 0)
			continue;
		// 2004.05.22->oneway48 modify
		for(int j = 0 ; j < 9 ; j++)
		{
			if(g_pLocalUser->m_SkillList[i].m_sSkillID == m_QuickSlot[(m_SelectedSetNumber/ 9) + j].iconid)
			{
				if(g_pLocalUser->m_SkillList[i].m_dwCooldownStartTime > 0)
				{
					DWORD elapsedtime = g_LocalSystemTime - g_pLocalUser->m_SkillList[i].m_dwCooldownStartTime;
					FLOAT drawpercent = ((FLOAT)elapsedtime / g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->detailInfo[g_pLocalUser->m_SkillList[i].m_cSkillLevel].coolDown) * 30.;
					int startpixel = (int)drawpercent;
					
					if(startpixel < 0)
						startpixel = 0;
					if(startpixel > 30)
						startpixel = 30;
					
					POINT position;
					position.x = m_WindowPosition.x + m_ButtonPositionArray[j+1].x;
					position.y = m_WindowPosition.y + m_ButtonPositionArray[j+1].y;
					RECT rect = {104, 8, 134, 38};
					rect.top += startpixel;
					m_CooldownImage->DrawWithRegion(position.x, position.y + startpixel, rect);
				}
			}
		}
	}
}


void _XWindow_QSlot::GetInventoryPtr(void)
{
	m_pInventory = (_XWindow_Inventory*)m_pMyManager->FindWindow(_XDEF_WTITLE_INVENTORY);
}
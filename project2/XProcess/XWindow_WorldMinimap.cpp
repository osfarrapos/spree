// XWindow_WorldMinimap.cpp: implementation of the _XWindow_WorldMinimap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_WorldMinimap.h"
#include "XWindow_MaximumMap.h"
#include "XWindow_TutorialIcon.h"
#include "math.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_GM_UserCoordination.h"
#include "XWindow_Debug.h"
#include "XWindow_NPCSearch.h"
#include "Xwindow_Poison.h"
#include "XWindow_Info.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define _XDEF_LARGEMINIMAP_LEFT				3
#define _XDEF_LARGEMINIMAP_TOP				16+15
#define _XDEF_LARGEMINIMAP_RIGHT			453
#define _XDEF_LARGEMINIMAP_BOTTOM			466+15

#define _XDEF_MINIMAPARROW_LEFT				77-17//65-17
#define _XDEF_MINIMAPARROW_TOP				91+15-17//95-17
#define _XDEF_MINIMAPARROW_RIGHT			77+16//65+16
#define _XDEF_MINIMAPARROW_BOTTOM			91+15+16 //95+16

#define _XDEF_MINIMAPFRUSTRUM_LEFT			77-32//65-32
#define _XDEF_MINIMAPFRUSTRUM_TOP			91+15-32//95-32
#define _XDEF_MINIMAPFRUSTRUM_RIGHT			77+31//65+31
#define _XDEF_MINIMAPFRUSTRUM_BOTTOM		91+15+31//65+31

#define _XDEF_MINIMAP_LEFT					3 
#define _XDEF_MINIMAP_TOP					17+16
#define _XDEF_MINIMAP_RIGHT					152
#define _XDEF_MINIMAP_BOTTOM				166+16


// minimap center position
#define _XDEF_MINIMAP_ORIGIN_X				77
#define _XDEF_MINIMAP_ORIGIN_Y				91+15


void __stdcall _XCallback_MM_Zoomin(  POINT pos, SIZE size  )
{		
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_ZOOMIN), _XSC_INFORMATION );	
}

void __stdcall _XCallback_MM_Zoomout(  POINT pos, SIZE size  )
{	
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_ZOOMOUT), _XSC_INFORMATION );	
}

void __stdcall _XCallback_MM_MaximumMap(  POINT pos, SIZE size  )
{	
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_MAXIMUMMAP), _XSC_INFORMATION );	
}

void __stdcall _XCallback_MM_WindowClose(  POINT pos, SIZE size  )
{	
 	if( g_bShowItemMallWindow ) return;	
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y+15, _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_CLOSE), _XSC_INFORMATION ); // 지도최소화
}

void __stdcall _XCallback_MM_WorldMap(  POINT pos, SIZE size  )
{
 	if( g_bShowItemMallWindow ) return;
	// 임시 제한
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NINEDRAGONMAP), _XSC_INFORMATION ); // 구룡전도
}

void __stdcall _XCallback_MM_SunMoonIcon( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, g_SunMoonIconText[g_EnvironmentManager.m_Environment], _XSC_INFORMATION );		
}

void __stdcall _XCallback_MM_NPCSearch( POINT pos, SIZE size )
{
	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_2691), _XSC_INFORMATION );		
}
#ifdef _XDEF_WEBBOARD_20061211
void __stdcall _XCallback_MM_WebBoard( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3166), _XSC_INFORMATION );// 게시판 열기	
}
#endif

#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
void __stdcall _XCallback_MM_PKTracing( POINT pos, SIZE size )
{
	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_NEW_3387), _XSC_INFORMATION );
}
#endif			

_XWindow_WorldMinimap::_XWindow_WorldMinimap() : m_pAlphaSlider(NULL), m_nTransparency(0)
{
	m_MiniMapMode					= _XMINIMAPMODE_SMALL;
	m_RotateAngle					= 0.0f;
	m_MinimapZoomRate				= 0.155f;
	m_CurrentServerIndex			= 1;

	m_RateOfMapSizeToTerrain		= 1024.0f / g_LodTerrain.m_Width;
	m_RateOfLargeMapSizeToTerrain	= 450.0f / g_LodTerrain.m_Width;

	m_MinimizeStatus				= FALSE;
	m_bDrawNPCSearchEffect			= FALSE;

	m_bDrawPartyMemberSearchEffect	= FALSE;
	m_dwPrevDrawTimePartySearch		= 0;
	m_fEffectFramePartySearch		= 0;
	m_nTotalDrawTimePartySearch		= 0;
	m_nSearchPartyMemberIndex		= -1;
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	m_bDrawPKWarning				= FALSE;//임시로 TRUE //PK범 경고 이미지표시하기 
	m_dwPrevDrawTimePKWarning		= 0;
	m_fEffectFramePKWarning			= 0.0f;	
	m_PKWarningLevel				= 0;
	memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif

	m_pTitleBar						= NULL;
	m_pBottomLeftBar				= NULL;
	m_pBottomRightBar				= NULL;
	m_pLeftEdgeImage				= NULL;
	m_pSunMoonIcon					= NULL;
	
	
	m_pMaximumMapTitleLeftBar		= NULL;
	m_pMaximumMapTitleMiddleBar		= NULL;
	m_pMaximumMapTitleRightBar		= NULL;
	
	m_pMaximumMapBottomMiddleBar1	= NULL;
	m_pMaximumMapBottomMiddleBar2	= NULL;
	
	m_CheckLocalUserPosition_X		= 0.0f;
	m_CheckLocalUserPosition_Y		= 0.0f;
	
	m_TooltipTargetPosition_X		= 0;
	m_TooltipTargetPosition_Y		= 0;
	
	m_SelectedQuestType				= _XQUEST_TYPE_NONE;
	m_SelectedQuestID				= -1;
	m_SelectedPartyMemberIndex		= -1;
	m_SelectedNPCIndex				= -1;

	m_bDrawInformationTooltip		= FALSE;
	
	m_PrevDrawTime			= 0;
	m_EffectOrderSub		= false;
	m_EffectFrame			= 0.0f;

	m_PrevDrawTimeNPCSearch = 0;
	m_EffectFrameNPCSearch	= 0.0f;	

	m_pMinimapTexture				= NULL;
	for( int i = 0; i < 4; ++i)
	{		
		m_YellowImpressionMark[i] = NULL;
		m_GreenImpressionMark[i] = NULL;
		m_BlueImpressionMark[i] = NULL;
		m_OrangeImpressionMark[i] = NULL;
		m_WhiteImpressionMark[i] = NULL;

		m_imageNPCSearchMark[i] = NULL;		
	}
	m_YellowQuestionMark = NULL;
	m_GreenQuestionMark = NULL;
	m_BlueQuestionMark = NULL;
	m_OrangeQuestionMark = NULL;
	m_WhiteQuestionMark = NULL;
	
	minimizebutton = NULL;
	zoommaxbutton = NULL;
	zoominbutton = NULL;
	zoomoutbutton = NULL;
	worldmapbutton = NULL;
	helpbookbutton = NULL;
	m_btnNPCSearch = NULL;
#ifdef _XDEF_WEBBOARD_20061211
	m_btnWebBoard = NULL;
#endif
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	m_btnPKTracingInfo = NULL;
#endif			

	m_pNPCSearchWindowPtr = NULL;
	m_SelectedNPCJobIndex = _XNPC_JOBCLASS_FOOD;

	m_bDisableZoomButton = FALSE;
}

_XWindow_WorldMinimap::~_XWindow_WorldMinimap()
{
	
	list <int>::iterator iter_drawnpc;
	for(iter_drawnpc = m_listDrawNPCIndex.begin() ; iter_drawnpc != m_listDrawNPCIndex.end() ; )
	{
		iter_drawnpc = m_listDrawNPCIndex.erase(iter_drawnpc);
	}
	m_listDrawNPCIndex.clear();
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
	_XPKTRACING_POSITION_INFO* pktracing = NULL;
	list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracing;
	for(iter_pktracing = m_listPKTracingPosition.begin() ; iter_pktracing != m_listPKTracingPosition.end() ; ++iter_pktracing)
	{
		pktracing = *iter_pktracing;
		SAFE_DELETE( pktracing );
	}
	m_listPKTracingPosition.clear();
#endif
	SAFE_RELEASE( m_pMinimapTexture );

	SAFE_DELETE( helpbookbutton );

	for( int i = 0; i < 4; ++i)
	{		
		SAFE_DELETE( m_YellowImpressionMark[i] );
		SAFE_DELETE( m_GreenImpressionMark[i] );
		SAFE_DELETE( m_BlueImpressionMark[i] );
		SAFE_DELETE( m_OrangeImpressionMark[i] );
		SAFE_DELETE( m_WhiteImpressionMark[i] );
		SAFE_DELETE( m_imageNPCSearchMark[i] );
	}

	SAFE_DELETE( m_YellowQuestionMark );
	SAFE_DELETE( m_GreenQuestionMark );
	SAFE_DELETE( m_BlueQuestionMark );
	SAFE_DELETE( m_OrangeQuestionMark );
	SAFE_DELETE( m_WhiteQuestionMark );
}

BOOL _XWindow_WorldMinimap::Initialize( void )
{
	SetDrawBorder( FALSE );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int mainbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_main_button.tga" );
	int mainunkiiconindex = g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" );

	m_pTitleBar = new _XImageStatic;
	m_pTitleBar->Create( 0, 0, 154, 17,
						 &g_MainInterfaceTextureArchive, charinfointerface );
	m_pTitleBar->SetClipRect( 0, 206, 153, 223 );
	InsertChildObject( m_pTitleBar );

	m_pSubBar = new _XImageStatic;
	m_pSubBar->Create( 3, 17, 154, 32,
		&g_MainInterfaceTextureArchive, charinfointerface );
	m_pSubBar->SetClipRect( 0, 223, 150, 238 );
	InsertChildObject( m_pSubBar );

	m_pBottomLeftBar = new _XImageStatic;
	m_pBottomLeftBar->Create( 0, m_WindowSize.cy-27 , 96, m_WindowSize.cy-27+18,
						      &g_MainInterfaceTextureArchive, charinfointerface );	
	m_pBottomLeftBar->SetClipRect( 0, 238, 96, 256 );
	InsertChildObject( m_pBottomLeftBar );

	m_pBottomRightBar = new _XImageStatic;
	m_pBottomRightBar->Create( m_WindowSize.cx - 60, m_WindowSize.cy-27, 59, m_WindowSize.cy-27+26,
						       &g_MainInterfaceTextureArchive, charinfointerface );	
	m_pBottomRightBar->SetClipRect( 180, 165, 238, 191 );
	InsertChildObject( m_pBottomRightBar );

	m_pLeftEdgeImage = new _XImageStatic;
	m_pLeftEdgeImage->Create( 0, 17, 2, 170+15,
							 &g_MainInterfaceTextureArchive, charinfointerface );	
	
	m_pLeftEdgeImage->SetScale( 1.0f, 1.2f); // 길이 167				
	m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 256 );
	InsertChildObject( m_pLeftEdgeImage );

	// 제일 큰 모드일때 	
	// 큰지도일때 타이틀바 왼쪽 부분 ( 77X 17 )
	m_pMaximumMapTitleLeftBar = new _XImageStatic;
	m_pMaximumMapTitleLeftBar->Create( 0, 0, 77, 17,
		&g_MainInterfaceTextureArchive, charinfointerface );
	m_pMaximumMapTitleLeftBar->SetClipRect( 0, 206, 77, 223 );
	m_pMaximumMapTitleLeftBar->ShowWindow(FALSE);
	InsertChildObject( m_pMaximumMapTitleLeftBar );
	
	// 큰지도일때 타이틀바 가운데 부분 ( 374 X 17 )
	m_pMaximumMapTitleMiddleBar = new _XImageStatic;
	m_pMaximumMapTitleMiddleBar->Create( 0, 0, 74, 17,
		&g_MainInterfaceTextureArchive, charinfointerface );
	m_pMaximumMapTitleMiddleBar->SetClipRect( 77, 206, 151, 223 );
	m_pMaximumMapTitleMiddleBar->SetScale( 5.054f, 1.0f );
	m_pMaximumMapTitleMiddleBar->ShowWindow(FALSE);
	InsertChildObject( m_pMaximumMapTitleMiddleBar );
	
	// 큰지도일때 타이틀바 오른쪽 부분 ( 3 X 17 )
	m_pMaximumMapTitleRightBar = new _XImageStatic;
	m_pMaximumMapTitleRightBar->Create( 0, 0, 3, 17,
		&g_MainInterfaceTextureArchive, charinfointerface );
	m_pMaximumMapTitleRightBar->SetClipRect( 151, 206, 154, 223 );
	m_pMaximumMapTitleRightBar->ShowWindow(FALSE);
	InsertChildObject( m_pMaximumMapTitleRightBar );
	
	m_pMaximumMapBottomMiddleBar1	= NULL;
	m_pMaximumMapBottomMiddleBar2	= NULL;
	
	// 아래쪽 바탕 중간 부분 1( 93 X 17 ) 3번 그려야 한다.
	m_pMaximumMapBottomMiddleBar1 = new _XImageStatic;
	m_pMaximumMapBottomMiddleBar1->Create( 0, m_WindowSize.cy-27 , 96, 18,
		&g_MainInterfaceTextureArchive, charinfointerface );	
	m_pMaximumMapBottomMiddleBar1->SetClipRect( 3, 238, 96, 256 );
	m_pMaximumMapBottomMiddleBar1->ShowWindow(FALSE);
	InsertChildObject( m_pMaximumMapBottomMiddleBar1 );
	
	// 아래쪽 바탕 중간 부분 2( 20 X 17 )
	m_pMaximumMapBottomMiddleBar2 = new _XImageStatic;
	m_pMaximumMapBottomMiddleBar2->Create( 0, m_WindowSize.cy-27 , 20, 18,
		&g_MainInterfaceTextureArchive, charinfointerface );	
	m_pMaximumMapBottomMiddleBar2->SetClipRect( 3, 238, 23, 256 );
	m_pMaximumMapBottomMiddleBar2->ShowWindow(FALSE);
	InsertChildObject( m_pMaximumMapBottomMiddleBar2 );
	


	m_pSunMoonIcon = new _XImageStatic;
	m_pSunMoonIcon->Create( 127 , 15, 143, 16 +15, &g_MainInterfaceTextureArchive, mainbuttonindex );	
	m_pSunMoonIcon->SetClipRect( 65, 147, 80, 163 );
	m_pSunMoonIcon->SetCallBackFunction_ToolTip( _XCallback_MM_SunMoonIcon );
	InsertChildObject( m_pSunMoonIcon );

	
	_XBTN_STRUCTURE minimizebtnstruct = { TRUE, { 137, 2 }, { 12, 12 }, _XDEF_MINIMAP_CLOSEBUTTON,
										  charinfointerface,charinfointerface,charinfointerface, 
										 &g_MainInterfaceTextureArchive };
	minimizebutton = new _XButton;
	minimizebutton->Create( minimizebtnstruct );	
	minimizebutton->SetCallBackFunction_ToolTip( _XCallback_MM_WindowClose );
	
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 155, 212, 167, 224 );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  167, 212, 179, 224 );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  179, 212, 191, 224 );
	
#ifdef _XDEF_NEW_WORLDMAP_070528_MAGVIPER //Author : 양희왕 //breif : 신구룡전도
	_XBTN_STRUCTURE worldmapbtnstruct = { TRUE, { 124, 190 }, { 19, 19 }, _XDEF_WORLDMAP_STARTBUTTON,
											charinfointerface,charinfointerface,charinfointerface,
											&g_MainInterfaceTextureArchive };
#else
	_XBTN_STRUCTURE worldmapbtnstruct = { TRUE, { 124, 190 }, { 19, 19 }, _XDEF_MINIMAP_WORLDMAPBUTTON,
											charinfointerface,charinfointerface,charinfointerface,
											&g_MainInterfaceTextureArchive };
#endif

	worldmapbutton = new _XButton;
	worldmapbutton->Create( worldmapbtnstruct );	
	worldmapbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 195, 213, 214, 232 );
	worldmapbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  214, 213, 233, 232 );
	worldmapbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  233, 213, 252, 232 );
	worldmapbutton->SetCallBackFunction_ToolTip( _XCallback_MM_WorldMap );
	
	_XBTN_STRUCTURE zoommaxbtnstruct = { TRUE, { 100, 186 }, { 23, 23 }, _XDEF_MINIMAP_ZOOMMAXBUTTON,
										charinfointerface,charinfointerface,charinfointerface,
										&g_MainInterfaceTextureArchive };
	zoommaxbutton = new _XButton;
	zoommaxbutton->Create( zoommaxbtnstruct );	
	zoommaxbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 184, 232, 207, 256 );
	zoommaxbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  207, 232, 230, 256 );
	zoommaxbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  230, 232, 253, 256 );
	zoommaxbutton->SetCallBackFunction_ToolTip( _XCallback_MM_MaximumMap );
		
	_XBTN_STRUCTURE zoominbtnstruct = { TRUE, { 138, 184 }, { 9, 9 }, 0 /*_XDEF_MINIMAP_ZOOMINBUTTON*/,
										charinfointerface,charinfointerface,charinfointerface,
									   &g_MainInterfaceTextureArchive };
	zoominbutton = new _XButton;
	zoominbutton->Create( zoominbtnstruct );
	zoominbutton->setMessageMode( _BTNMESSAGEMODE_BTNDOWN );
	zoominbutton->SetCallBackFunction_ToolTip( _XCallback_MM_Zoomin );
	zoominbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 183, 194, 192, 203 );
	zoominbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  194, 194, 203, 203 );
	zoominbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  205, 194, 214, 203 );

		
	_XBTN_STRUCTURE zoomoutbtnstruct = { TRUE, { 144, 192 }, { 9, 9 }, 0 /*_XDEF_MINIMAP_ZOOMOUTBUTTON*/,
										charinfointerface,charinfointerface,charinfointerface,
									   &g_MainInterfaceTextureArchive };
	zoomoutbutton = new _XButton;
	zoomoutbutton->Create( zoomoutbtnstruct );
	zoomoutbutton->setMessageMode( _BTNMESSAGEMODE_BTNDOWN );
	zoomoutbutton->SetCallBackFunction_ToolTip( _XCallback_MM_Zoomout );
	zoomoutbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 216, 194, 226, 204 );
	zoomoutbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  227, 194, 237, 204 );
	zoomoutbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  238, 194, 248, 204 );	

	int tutorialresourceindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		tutorialresourceindex = g_MainInterfaceTextureArchive.FindResource( "us_tutorial_01.tga" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		tutorialresourceindex = g_MainInterfaceTextureArchive.FindResource( "rs_tutorial_01.tga" );
	}
	else
	{
		tutorialresourceindex = g_MainInterfaceTextureArchive.FindResource( "tutorial_01.tga" );
	}
	
	_XBTN_STRUCTURE npcsearchbtnstruct = { TRUE, { 3, 185 }, { /*37*/33, 25 }, _XDEF_MINIMAP_NPCSEARCHBTN,
		tutorialresourceindex,tutorialresourceindex,tutorialresourceindex,	&g_MainInterfaceTextureArchive };
	m_btnNPCSearch = new _XButton;
	m_btnNPCSearch->Create( npcsearchbtnstruct );
	m_btnNPCSearch->SetCallBackFunction_ToolTip( _XCallback_MM_NPCSearch );
	m_btnNPCSearch->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 218, 1, 255, 26 );
	m_btnNPCSearch->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  218, 28, 255, 53 );
	m_btnNPCSearch->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  218, 55, 255, 80 );

	int webboardresourceindex1 = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_01.tga" );

#ifdef _XDEF_WEBBOARD_20061211
	_XBTN_STRUCTURE webboardbtnstruct = { TRUE, {/* 40 */31, 184 }, { 30, 25 }, _XDEF_MINIMAP_BTN_WEBBOARD,
		webboardresourceindex1,webboardresourceindex1,webboardresourceindex1,	&g_MainInterfaceTextureArchive };
	m_btnWebBoard = new _XButton;
	m_btnWebBoard->Create( webboardbtnstruct );
	m_btnWebBoard->SetCallBackFunction_ToolTip( _XCallback_MM_WebBoard );
	m_btnWebBoard->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 219, 41, 246, 63 );
	m_btnWebBoard->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  219, 68, 246, 90 );
	m_btnWebBoard->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  219, 94, 246, 117 );
//	m_btnWebBoard->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 213, 39, 254, 64 );
//	m_btnWebBoard->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  213, 66, 254, 91 );
//	m_btnWebBoard->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  213, 93, 254, 118 );
	InsertChildObject( m_btnWebBoard );
#endif
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
	_XBTN_STRUCTURE pktracingbtnstruct = { TRUE, { 70, 184 }, { 30, 25 }, _XDEF_MINIMAP_BTN_PKTRACING,
		webboardresourceindex1,webboardresourceindex1,webboardresourceindex1,	&g_MainInterfaceTextureArchive };
	m_btnPKTracingInfo = new _XButton;
	m_btnPKTracingInfo->Create( pktracingbtnstruct );
	m_btnPKTracingInfo->SetCallBackFunction_ToolTip( _XCallback_MM_PKTracing );
	m_btnPKTracingInfo->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 166, 151, 193, 174 );
	m_btnPKTracingInfo->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  166, 178, 193, 201 );
	m_btnPKTracingInfo->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  166, 206, 193, 228 );
	InsertChildObject( m_btnPKTracingInfo );
	#endif
#endif			


	InsertChildObject( worldmapbutton );
	InsertChildObject( minimizebutton );		
	InsertChildObject( zoommaxbutton );
	InsertChildObject( zoomoutbutton );
	InsertChildObject( zoominbutton );
	InsertChildObject( m_btnNPCSearch );

	_XBTN_STRUCTURE helpbookbtnstruct = { TRUE, { 0, 0 }, { 96, 28 }, _XDEF_MINIMAP_HELPBOOKBTN,
										g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ),
										g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ),
										g_MainInterfaceTextureArchive.FindResource( "MI_main_unki_icon.tga" ),
									   &g_MainInterfaceTextureArchive };

	helpbookbutton = new _XButton;
	helpbookbutton->Create( helpbookbtnstruct );
	helpbookbutton->SetButtonTextColor(0xFFD6C597);
	helpbookbutton->SetGrowTextMode(TRUE, 0xFF000000);
	helpbookbutton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_WORLDMINIMAP_2304);	
	helpbookbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 96, 96, 124 );
	helpbookbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 125, 96, 151 );
	helpbookbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 152, 96, 180 );


	m_MinimapArrow.Create( m_WindowPosition.x + _XDEF_MINIMAPARROW_LEFT, 
						   m_WindowPosition.y + _XDEF_MINIMAPARROW_TOP, 
						   m_WindowPosition.x + _XDEF_MINIMAPARROW_RIGHT, 
						   m_WindowPosition.y + _XDEF_MINIMAPARROW_BOTTOM );
	m_MinimapArrow.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "mm_mark_character.tga" ) );

	m_FrustumeImage.Create( m_WindowPosition.x + _XDEF_MINIMAPFRUSTRUM_LEFT, 
							m_WindowPosition.y + _XDEF_MINIMAPFRUSTRUM_TOP, 
							m_WindowPosition.x + _XDEF_MINIMAPFRUSTRUM_RIGHT, 
							m_WindowPosition.y + _XDEF_MINIMAPFRUSTRUM_BOTTOM,
							&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "mm_mark_frustum.tga" ) );
	m_FrustumeImage.SetCenter( 32.0f, 32.0f );


	m_Minimap.Create( m_WindowPosition.x + _XDEF_MINIMAP_LEFT, 
		m_WindowPosition.y + _XDEF_MINIMAP_TOP, 
		m_WindowPosition.x + _XDEF_MINIMAP_RIGHT, 
		m_WindowPosition.y + _XDEF_MINIMAP_BOTTOM );
	
	m_LargeMinimapImage.Create( m_WindowPosition.x + 3, m_WindowPosition.y + 16 +15, m_WindowPosition.x + 453, m_WindowPosition.y + 466+15,
		&g_MainInterfaceTextureArchive, -1 );	
	m_LargeMinimapImage.SetClipRect( 0, 0, 512, 512 );
	m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);
	m_LargeMinimapImage.ShowWindow(FALSE);
	
	
	m_NPCImageTradeType.Create( m_WindowPosition.x, m_WindowPosition.y, 
							 m_WindowPosition.x + 8, m_WindowPosition.y + 8,
							 &g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT NPCTradeTypecliprect = { 137, 226, 145, 234 };
	m_NPCImageTradeType.SetClipRect( NPCTradeTypecliprect );
	m_NPCImageTradeType.SetScale(0.7f,0.7f);
	
	m_NPCImageEventType.Create( m_WindowPosition.x, m_WindowPosition.y, 
							 m_WindowPosition.x + 9, m_WindowPosition.y + 9,
							 &g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT NPCEventcliprect = { 128, 226, 137, 235  };
	m_NPCImageEventType.SetClipRect( NPCEventcliprect );
	m_NPCImageEventType.SetScale(0.7f,0.7f);
	
	m_PartyPlayerImage.Create( m_WindowPosition.x, m_WindowPosition.y, 
							 m_WindowPosition.x + 7, m_WindowPosition.y + 7,
							 &g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT PartyPlayercliprect = { 136, 219, 143, 226  }; 
	m_PartyPlayerImage.SetClipRect( PartyPlayercliprect );
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	m_imagePKTracingPoint.Create( m_WindowPosition.x, m_WindowPosition.y, 
							 m_WindowPosition.x + 8, m_WindowPosition.y + 8,
							 &g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT PKTracingcliprect = { 129, 219, 136, 226 };
	m_imagePKTracingPoint.SetClipRect( PKTracingcliprect );

	m_imageSomeOneTracingPoint.Create( m_WindowPosition.x, m_WindowPosition.y, 
							 m_WindowPosition.x + 8, m_WindowPosition.y + 8,
							 &g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT SomeoneTracingcliprect = { 129, 211, 136, 218 };
	m_imageSomeOneTracingPoint.SetClipRect( SomeoneTracingcliprect );
#endif			
	SetLocalUserPosition();
	
	// 퀘스트마크 생성(!)
	
	for( int i = 0; i < 4; ++i)
	{
		m_BlueImpressionMark[i] = new _XImageStatic;
		m_BlueImpressionMark[i]->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
			&g_MainInterfaceTextureArchive, mainunkiiconindex );
		RECT bluerect = { 200+i*14, 96, 214+i*14, 110  };
		m_BlueImpressionMark[i]->SetClipRect( bluerect );
		
		m_YellowImpressionMark[i] = new _XImageStatic;
		m_YellowImpressionMark[i]->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
			&g_MainInterfaceTextureArchive, mainunkiiconindex );
		RECT yellowrect = { 200+i*14, 110, 214+i*14, 124  };
		m_YellowImpressionMark[i]->SetClipRect( yellowrect );
		
		m_GreenImpressionMark[i] = new _XImageStatic;
		m_GreenImpressionMark[i]->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
			&g_MainInterfaceTextureArchive, mainunkiiconindex );
		RECT greenrect = { 200+i*14, 124, 214+i*14, 138  };
		m_GreenImpressionMark[i]->SetClipRect( greenrect );
		
		m_OrangeImpressionMark[i] = new _XImageStatic;
		m_OrangeImpressionMark[i]->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
			&g_MainInterfaceTextureArchive, mainunkiiconindex );
		RECT orangerect = { 200+i*14, 138, 214+i*14, 152  };
		m_OrangeImpressionMark[i]->SetClipRect( orangerect );
		
		m_WhiteImpressionMark[i] = new _XImageStatic;
		m_WhiteImpressionMark[i]->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
			&g_MainInterfaceTextureArchive, mainunkiiconindex );
		RECT whiterect = { 200+i*14, 152, 214+i*14, 166  };
		m_WhiteImpressionMark[i]->SetClipRect( whiterect );

		// NPC 찾기 마크
		m_imageNPCSearchMark[i] = new _XImageStatic;
		m_imageNPCSearchMark[i]->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+32, m_WindowPosition.y+32,
			&g_MainInterfaceTextureArchive, tutorialresourceindex );
		RECT npcrect = { 60+i*34, 115, 92+i*34, 147  };
		m_imageNPCSearchMark[i]->SetClipRect( npcrect );
		
	}
	
	// 힌트마크 생성(?)
	m_BlueQuestionMark = new _XImageStatic;
	m_BlueQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
		&g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT bluehintrect = { 183, 239, 197, 253  };
	m_BlueQuestionMark->SetClipRect( bluehintrect );
	
	m_YellowQuestionMark = new _XImageStatic;
	m_YellowQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
		&g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT yellowhintrect = { 197, 239, 211, 253  };
	m_YellowQuestionMark->SetClipRect( yellowhintrect );
	
	m_GreenQuestionMark = new _XImageStatic;
	m_GreenQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
		&g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT greenhintrect = { 211, 239, 225, 253  };
	m_GreenQuestionMark->SetClipRect( greenhintrect );
	
	m_OrangeQuestionMark = new _XImageStatic;
	m_OrangeQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
		&g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT orangehintrect = { 225, 239, 239, 253  };
	m_OrangeQuestionMark->SetClipRect( orangehintrect );
	
	m_WhiteQuestionMark = new _XImageStatic;
	m_WhiteQuestionMark->Create(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+14, m_WindowPosition.y+14,
		&g_MainInterfaceTextureArchive, mainbuttonindex );
	RECT whitehintrect = { 99, 130, 113, 144  };
	m_WhiteQuestionMark->SetClipRect( whitehintrect );

#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕추가

	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	int optionmenuinterface2 = g_MainInterfaceTextureArchive.FindResource( "MI_System2.tga" );
	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );

	_XSLIDERCONTROL_STRUCTURE	slidercreatestruct =
	{
		TRUE, { 0, 0 }, 
		{ 140, 8 }, _XDEF_MAXMINIMAP_ALPHASLIDER,
		optionmenuinterface2,
		optionmenuinterface2,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		{ -5, -4 }, { 12, 12 },
		optionmenuinterface2,
		&g_MainInterfaceTextureArchive,
		"HSlider"
	};

	SIZE spinbtnsize = { 12, 12 };
	RECT leftspinbtncliprect[3] =
	{
		{ 200, 17, 212, 29 },
		{ 200, 29, 212, 41 },
		{ 200, 41, 212, 53 },
	};

	RECT rightspinbtncliprect[3] =
	{
		{ 178,  0,  190, 12 },
		{ 178,  12, 190, 24 },
		{ 178,  24, 190, 36 },
	};

	m_pAlphaSlider = new _XHSlider;
	m_pAlphaSlider->Create( slidercreatestruct );
	m_pAlphaSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pAlphaSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pAlphaSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pAlphaSlider->m_IndicatorImage.SetClipRect( 2, 243, 140, 247 );
	m_pAlphaSlider->SetClipRect( 1,247, 141, 255 );
	m_pAlphaSlider->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_pAlphaSlider->SetGabSize( 1 );
	m_pAlphaSlider->ShowWindow( TRUE );
	m_pAlphaSlider->EnableWindow( TRUE );

	m_pAlphaSlider->SetGaugeValue( 100.0f );
	m_pAlphaSlider->SetUseSpinButton( TRUE );
	m_pAlphaSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );
		
	InsertChildObject( m_pAlphaSlider );

	//처음 최대치 값
	m_nTransparency = 204;

	//투명도는 최대 80% 이다..
	m_LargeMinimapImage.m_FColor = D3DCOLOR_ARGB( 204, 255, 255, 255 );

#endif
	

	// Set minimap previous setting...
	BOOL viewminimapflag = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWMINIMAP, TRUE, TRUE );
	ShowWindow( viewminimapflag );	

	int xpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MINIMAPXPOS, gnWidth-130, TRUE );
	int ypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MINIMAPYPOS, 0, TRUE );
	MoveWindow( xpos, ypos );
	CheckOutside();

	return TRUE;
}

void _XWindow_WorldMinimap::ShowWindow(BOOL show)
{	
	if( show )
	{
		_XWindow_NPCSearch* pNPCSearch_Window = (_XWindow_NPCSearch*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_NPCSEARCHWINDOW );	
		if( pNPCSearch_Window )
		{
			m_pNPCSearchWindowPtr = pNPCSearch_Window;
		}
	}
	_XWindow::ShowWindow(show);
}

void _XWindow_WorldMinimap::RebuildDrawNPCList( float userposition_x, float userposition_z )
{
	float Map_NPC_X, Map_NPC_Z;
	if( !m_listDrawNPCIndex.empty() )
	{
		list <int>::iterator iter_drawnpc;
		for(iter_drawnpc = m_listDrawNPCIndex.begin() ; iter_drawnpc != m_listDrawNPCIndex.end() ; )
		{
			iter_drawnpc = m_listDrawNPCIndex.erase(iter_drawnpc);
		}
	}
	
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
	{
		Map_NPC_X = (g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._41 - userposition_x) * m_RateOfMapSizeToTerrain;
		Map_NPC_Z = -(g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._43 - userposition_z) * m_RateOfMapSizeToTerrain;
		
		if((_XDEF_MINIMAP_ORIGIN_X + Map_NPC_X > _XDEF_MINIMAP_LEFT + 4) && (_XDEF_MINIMAP_ORIGIN_X + Map_NPC_X < _XDEF_MINIMAP_RIGHT))
		{
			if((_XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z < _XDEF_MINIMAP_BOTTOM) && (_XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z > _XDEF_MINIMAP_TOP + 4))
			{									
				m_listDrawNPCIndex.push_back(i);
			}
		}
	}
}

BOOL _XWindow_WorldMinimap::SetMinimapImage( LPTSTR mapname  )
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

	/*
	int minimapimageindex = g_TerrainTextureArchive.GetResourceIndex( minimapnamestr, _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE );
	
	m_Minimap.SetTexture( &g_TerrainTextureArchive, minimapimageindex );
	
	m_LargeMinimapImage.SetTexture( &g_TerrainTextureArchive, minimapimageindex );
	*/

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
		strcat( minimapnamestr, mapname );
		
		LPTSTR cutpoint = strstr( minimapnamestr, "xtv" );
		if( cutpoint )
		{
			*cutpoint = NULL;
		}

		resourceindex = TextureArchive.FindResource( minimapnamestr );
		strcat( minimapnamestr, "tga");

		if( resourceindex < 0 )
		{
			_XFatalError( "Resource not found in the texture archive [%s]", minimapnamestr );
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
	
	SAFE_RELEASE( m_pMinimapTexture );

	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
		D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		0, NULL, NULL, &m_pMinimapTexture ) ) )
	{
		_XFatalError( "Can't create texture from memory" );
		delete[] pImageVFMemory;		
		return FALSE;
	}
	
	delete[] pImageVFMemory;

	m_RateOfLargeMapSizeToTerrain	= 450.0f / g_LodTerrain.m_Width;
	
	return TRUE;
}

void _XWindow_WorldMinimap::SetDefaultPosition( void )
{		
#ifdef _ACCLAIM_RUBICONADSYSTEM			
	if( g_ViewRubiconAD )
		MoveWindow( gnWidth-m_WindowSize.cx, g_RubiconADWindowSize.cy );	
	else
		MoveWindow( gnWidth-m_WindowSize.cx, 0 );
#else
	MoveWindow( gnWidth-m_WindowSize.cx, 0 );	
#endif

	CheckOutside();
}

void _XWindow_WorldMinimap::SaveWindowProperty( void )
{
	// save minimap window status...
	if( m_MiniMapMode == _XMINIMAPMODE_SMALL )
	{
		g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_VIEWMINIMAP, GetShowStatus() );
		g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MINIMAPXPOS, m_WindowPosition.x );
		g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MINIMAPYPOS, m_WindowPosition.y );
	}
}

void _XWindow_WorldMinimap::DestroyWindow( void )
{	
	_XWindow::DestroyWindow();

	m_MinimapArrow.ReleaseObject();	
	m_Minimap.ReleaseObject();

	SAFE_RELEASE( m_pMinimapTexture );
}

void _XWindow_WorldMinimap::SetRotateArrow( FLOAT angle )
{	
	m_RotateAngle = angle;
	m_MinimapArrow.Rotate( angle );	
}

void _XWindow_WorldMinimap::SetRotateFrustum( FLOAT angle )
{
	m_FrustumRotateAngle = angle;
	m_FrustumeImage.SetRotateAngle( angle );
}

void _XWindow_WorldMinimap::SetLocalUserPosition( void )
{
	if( !g_pLocalUser ) return;
	
	if( !m_MinimizeStatus )
	{
		if( m_MiniMapMode == _XMINIMAPMODE_SMALL )
		{
			FLOAT x = ( g_pLocalUser->m_Position.x + (g_LodTerrain.m_Width  /2.0f)) / g_LodTerrain.m_Width ;
			FLOAT z = ( -g_pLocalUser->m_Position.z + (g_LodTerrain.m_Height /2.0f)) / g_LodTerrain.m_Height;
			
			m_Minimap.ChangeUV( x, z, m_MinimapZoomRate );
			
			m_MinimapArrow.MoveWindow( m_WindowPosition.x + _XDEF_MINIMAPARROW_LEFT, m_WindowPosition.y + _XDEF_MINIMAPARROW_TOP,
				m_WindowPosition.x + _XDEF_MINIMAPARROW_RIGHT, m_WindowPosition.y + _XDEF_MINIMAPARROW_BOTTOM);	
			m_MinimapArrow.Rotate( m_RotateAngle  );	
			
			m_FrustumeImage.MoveWindow( m_WindowPosition.x + _XDEF_MINIMAPFRUSTRUM_LEFT, m_WindowPosition.y + _XDEF_MINIMAPFRUSTRUM_TOP,
				m_WindowPosition.x + _XDEF_MINIMAPFRUSTRUM_RIGHT, m_WindowPosition.y + _XDEF_MINIMAPFRUSTRUM_BOTTOM);
		}
		else
		{
			float x = g_pLocalUser->m_Position.x * m_RateOfLargeMapSizeToTerrain + 225.0f;
			float z = -g_pLocalUser->m_Position.z * m_RateOfLargeMapSizeToTerrain - 225.0f;
			
			m_MinimapArrow.MoveWindow(m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT - 18 + x, 
				m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 18 + z, 
				m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + 17 + x, 
				m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 17+ z);
			m_MinimapArrow.Rotate( m_RotateAngle );
			m_FrustumeImage.MoveWindow( m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 32 + x, 
				m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 32 + z, 
				m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + 31 + x, 
				m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 31+ z);
		}
	}
}

void _XWindow_WorldMinimap::SetLocalUserPosition( int left, int bottom, float rateofmapsizetoterrain, float size )
{
	if( !g_pLocalUser ) return;

	float x = g_pLocalUser->m_Position.x * rateofmapsizetoterrain + size/2;
	float z = -g_pLocalUser->m_Position.z * rateofmapsizetoterrain - size/2;
	
	m_MinimapArrow.MoveWindow(left - 18 + x, bottom - 18 + z, left + 17 + x, bottom + 17+ z);
	m_MinimapArrow.Rotate( m_RotateAngle );
	m_FrustumeImage.MoveWindow( left - 32 + x, bottom - 32 + z, left + 31 + x, bottom + 31+ z);
}

void _XWindow_WorldMinimap::MoveWindow( int X, int Y )
{		
	if( m_MiniMapMode == _XMINIMAPMODE_SMALL)
	{
		if( !m_MinimizeStatus )
		{
			m_Minimap.MoveWindow( X + _XDEF_MINIMAP_LEFT,  Y + _XDEF_MINIMAP_TOP,
				X + _XDEF_MINIMAP_RIGHT, Y + _XDEF_MINIMAP_BOTTOM);
			
			m_MinimapArrow.MoveWindow( X + _XDEF_MINIMAPARROW_LEFT,	 Y + _XDEF_MINIMAPARROW_TOP,
				X + _XDEF_MINIMAPARROW_RIGHT, Y + _XDEF_MINIMAPARROW_BOTTOM);	
			m_MinimapArrow.Rotate( m_RotateAngle  );	
			
			m_FrustumeImage.MoveWindow( X + _XDEF_MINIMAPFRUSTRUM_LEFT,	 Y + _XDEF_MINIMAPFRUSTRUM_TOP,
				X + _XDEF_MINIMAPFRUSTRUM_RIGHT, Y + _XDEF_MINIMAPFRUSTRUM_BOTTOM);

				
			if( m_pNPCSearchWindowPtr )
			{
				if( m_pNPCSearchWindowPtr->GetShowStatus() )
				{
					if( /*g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE ||*/ g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					{
						m_pNPCSearchWindowPtr->MoveWindow( m_WindowPosition.x - 154, m_WindowPosition.y );
					}
					else
					{
						m_pNPCSearchWindowPtr->MoveWindow( m_WindowPosition.x - 112, m_WindowPosition.y );
					}
				}
			}
			
		}
		m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 127 , m_WindowPosition.y + 15, m_WindowPosition.x + 143, m_WindowPosition.y + 16 +15 );			
	}
	else
	{
		if( !m_MinimizeStatus )
		{
			m_LargeMinimapImage.MoveWindow(m_WindowPosition.x + 3, m_WindowPosition.y + 16 +15, m_WindowPosition.x + 453, m_WindowPosition.y + 466+15);
			m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 297 , m_WindowPosition.y+15, m_WindowPosition.x + 383, m_WindowPosition.y + 16 +15 );

			
			if( m_pNPCSearchWindowPtr )
			{
				if( m_pNPCSearchWindowPtr->GetShowStatus() )
				{
					if( /*g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE ||*/ g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					{
						m_pNPCSearchWindowPtr->MoveWindow( m_WindowPosition.x - 154, m_WindowPosition.y );
					}
					else
					{
						m_pNPCSearchWindowPtr->MoveWindow( m_WindowPosition.x - 112, m_WindowPosition.y );
					}
				}
			}
		}
		else
		{
			m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 127 , m_WindowPosition.y + 15, m_WindowPosition.x + 143, m_WindowPosition.y + 16 +15 );
		}
	}
		
	_XWindow::MoveWindow( X, Y );
	helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );

	_XWindow_Poison* pPoison_Window = (_XWindow_Poison*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_POISON_WINDOW);
	if(pPoison_Window && pPoison_Window->GetShowStatus())
	{
		if(this->m_MinimizeStatus)	// minimize 되었음
		{
			pPoison_Window->MoveWindow(m_WindowPosition.x+2, m_WindowPosition.y+70);
		}
		else
		{
			if( m_MiniMapMode == _XMINIMAPMODE_SMALL)
				pPoison_Window->MoveWindow(m_WindowPosition.x+2, m_WindowPosition.y+214);
			else
				pPoison_Window->MoveWindow(m_WindowPosition.x+302, m_WindowPosition.y+513);
		}
	}
	
	_XWindow_Info* pInfo_Window = (_XWindow_Info*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_INFOWINDOW);
	if(pInfo_Window ) //창이 켜있을 때만 하는데 없을때도 해야한다
	{
		pInfo_Window->MoveWindow(m_WindowPosition.x-50, m_WindowPosition.y+10);
	}
}

void _XWindow_WorldMinimap::Draw( _XGUIObject*& pfocusobject )
{

	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;
				
	_XWindow::Draw(pfocusobject);
	
#ifdef _XTS_SHOWTUTORIAL
	if( g_pLocalUser->m_CharacterInfo.Get_innerlevel() < 10 )
		helpbookbutton->Draw(pfocusobject);
#else
	if( g_pLocalUser->m_CharacterInfo.innerlevel < 0 )
		helpbookbutton->Draw(pfocusobject);
#endif
	
	if( m_MinimizeStatus )
	{
		_XDrawLine2D( m_WindowPosition.x+m_WindowSize.cx - 2,  m_WindowPosition.y +17, 
					  m_WindowPosition.x + m_WindowSize.cx - 2, m_WindowPosition.y + 33+17, 0xFF000000, 0xFF000000 );
		_XDrawLine2D( m_WindowPosition.x+2,  m_WindowPosition.y +17, m_WindowPosition.x + 2, 
			m_WindowPosition.y + 33+17, 0xFF000000, 0xFF000000 );

		g_XBaseFont->SetColor( 0xFFCECECE );
		if( g_ZeroLevelMode )	// hotblood zerolevel
			g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y  + 34, _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NAMKYUNG) );	// _T("남경")
		else
			g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + 3 + 15, g_ZoneInfoTable[m_CurrentServerIndex-1].zoneshortname );
		//	g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + 34, g_ZoneInfoTable[m_CurrentServerIndex-1].zoneshortname );
						
		if(g_CurrentZoneInfoIndex != 18 && g_CurrentZoneInfoIndex != 19 && g_CurrentZoneInfoIndex != 21 &&
			g_CurrentZoneInfoIndex != 58 ) //Author : 양희왕 //breif : 요동 추가
		{
			g_XBaseFont->SetColor( 0xFFCDC6A6 );

			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 103, m_WindowPosition.y + 3 + 15, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 3 + 15, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d",
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
				/*g_XBaseFont->Print( m_WindowPosition.x + 42, m_WindowPosition.y + 34, 1.0f, "%d", 
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + 34, 1.0f, "%d",
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );*/
			}
			else
			{
				g_XBaseFont->Print( m_WindowPosition.x + 54, m_WindowPosition.y + 3 + 15, 1.0f, "%d", 
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + 3 + 15, 1.0f, "%d",
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
			}
		}
#ifdef _XDWDEBUG
		else
		{
			g_XBaseFont->SetColor( 0xFFCDC6A6 );
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 103, m_WindowPosition.y + 3 + 15, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 3 + 15, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d",
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
				/*g_XBaseFont->Print( m_WindowPosition.x + 42, m_WindowPosition.y + 34, 1.0f, "%d", 
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + 34, 1.0f, "%d",
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );*/
			}
			else
			{
				g_XBaseFont->Print( m_WindowPosition.x + 54, m_WindowPosition.y + 3 + 15, 1.0f, "%d", 
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + 3 + 15, 1.0f, "%d",
					_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
			}
		}
#endif

		g_XBaseFont->SetColor( 0xFFD6C597 );

		int param1 = g_ServerGameTime.Year;
		int param2 = g_ServerGameTime.Month+1;
		int param3 = g_ServerGameTime.Day+1;
		
		//g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + 3 + 15,
		//	g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAXIMUMMAP_DATE, &param1, &param2, &param3 ), 1.0f );
		
		g_XBaseFont->Flush();

#ifdef _ACCLAIM_IGAADSYSTEM
		XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

		if( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
			(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && 
			 g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
		  )	// 내공심법 시전중
		{
			if( !pMainGameProcess->m_ProcessLocalUserDieFlag &&
				g_pLocalUser->GetMotionClass() != _XACTION_DYING && g_pLocalUser->GetMotionClass() != _XACTION_DIE )
			{
				if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NONE )
				{
					pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_MEDITATION;
					g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_MEDITATION );
					g_IGAADWrapper.ShowWindow( TRUE );
					g_IGAADWrapper.StartDisplay();
				}
				else if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_MEDITATION )
				{
					if( !g_IGAADWrapper.GetShowStatus() )
					{
						pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
					}
					else
					{
						g_IGAADWrapper.MoveWindowPosition( helpbookbutton->GetWindowPos().x, helpbookbutton->GetWindowPos().y+2 );
						g_IGAADWrapper.Render();

						_XDrawRectAngle( helpbookbutton->GetWindowPos().x, 
										 helpbookbutton->GetWindowPos().y+1,
										 helpbookbutton->GetWindowPos().x-1+g_IGAADWrapper.GetWindowSize().cx, 
										 helpbookbutton->GetWindowPos().y+g_IGAADWrapper.GetWindowSize().cy, 
										 0.0f, D3DCOLOR_ARGB( g_IGAADWrapper.GetStaticImageAlphaLevel(), 0x5A, 0x5A, 0x00 ) ); 
						_XDrawRectAngle( helpbookbutton->GetWindowPos().x-1, 
										 helpbookbutton->GetWindowPos().y,
										 helpbookbutton->GetWindowPos().x+g_IGAADWrapper.GetWindowSize().cx, 
										 helpbookbutton->GetWindowPos().y+1+g_IGAADWrapper.GetWindowSize().cy, 
										 0.0f, D3DCOLOR_ARGB( g_IGAADWrapper.GetStaticImageAlphaLevel(), 0x28, 0x28, 0x28 ) );
					}
				}
			}
		}
		else if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_MEDITATION ) // 운기요상이 끝났을 경우
		{
			if( g_IGAADWrapper.GetFadeMode() == 0 )
				g_IGAADWrapper.SetFadeMode();

			if( !g_IGAADWrapper.GetShowStatus() )
			{
				pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
			}

			if( !pMainGameProcess->m_ProcessLocalUserDieFlag &&
				g_pLocalUser->GetMotionClass() != _XACTION_DYING && g_pLocalUser->GetMotionClass() != _XACTION_DIE )
			{
				g_IGAADWrapper.MoveWindowPosition( helpbookbutton->GetWindowPos().x, helpbookbutton->GetWindowPos().y+2 );
				g_IGAADWrapper.Render();

				_XDrawRectAngle( helpbookbutton->GetWindowPos().x, 
								 helpbookbutton->GetWindowPos().y+1,
								 helpbookbutton->GetWindowPos().x-1+g_IGAADWrapper.GetWindowSize().cx, 
								 helpbookbutton->GetWindowPos().y+g_IGAADWrapper.GetWindowSize().cy, 
								 0.0f, D3DCOLOR_ARGB( g_IGAADWrapper.GetStaticImageAlphaLevel(), 0x5A, 0x5A, 0x00 ) );
				_XDrawRectAngle( helpbookbutton->GetWindowPos().x-1, 
								 helpbookbutton->GetWindowPos().y,
								 helpbookbutton->GetWindowPos().x+g_IGAADWrapper.GetWindowSize().cx, 
								 helpbookbutton->GetWindowPos().y+1+g_IGAADWrapper.GetWindowSize().cy, 
								 0.0f, D3DCOLOR_ARGB( g_IGAADWrapper.GetStaticImageAlphaLevel(), 0x28, 0x28, 0x28 ) );
			}
		}
#endif

	}
	else
	{	
		if( m_MiniMapMode == _XMINIMAPMODE_SMALL )
		{
			_XDrawSolidBar( m_WindowPosition.x + 2,  m_WindowPosition.y + 16 +15, 
				m_WindowPosition.x + m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy - 27, D3DCOLOR_ARGB(128,0,0,0) );

			_XDrawLine2D( m_WindowPosition.x+m_WindowSize.cx -2,  m_WindowPosition.y+17, 
						  m_WindowPosition.x + m_WindowSize.cx - 2, m_WindowPosition.y+m_WindowSize.cy - 17, 0xFF000000, 0xFF000000 );
			_XDrawLine2D( m_WindowPosition.x+2,  m_WindowPosition.y+17, 
						  m_WindowPosition.x + 2, m_WindowPosition.y+m_WindowSize.cy - 10, 0xFF000000, 0xFF000000 );		
			m_Minimap.RenderClampMode( m_pMinimapTexture );

			if(g_CurrentZoneInfoIndex != 18 && g_CurrentZoneInfoIndex != 19 && g_CurrentZoneInfoIndex != 21 &&
				g_CurrentZoneInfoIndex != 58 ) //Author : 양희왕 //breif : 요동 추가
			{
				if( !g_pLocalUser->m_bIsBossBattle ) DrawNPCPosition(pfocusobject);
				if( g_pLocalUser->m_InParty ) DrawPartyPlayerPosition(pfocusobject);					
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
				DrawPKTracingPosition(pfocusobject);
#endif

				m_FrustumeImage.Draw();
				m_MinimapArrow.RenderAlphaBlend();
			}
#ifdef _XDWDEBUG
			else
			{
				m_FrustumeImage.Draw();
				m_MinimapArrow.RenderAlphaBlend();
			}
#endif
			
			//PK범이 주변에 오면 경고 표시를 한다.
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
			if( m_bDrawPKWarning )
				DrawPKWarning();
#endif			
			g_XBaseFont->SetColor( 0xFFCECECE );
			if( g_ZeroLevelMode )	// hotblood zerolevel
				g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + m_WindowSize.cy - 24, _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NAMKYUNG) );	// _T("남경")
			else
				g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + 3 + 15, g_ZoneInfoTable[m_CurrentServerIndex-1].zoneshortname );
				//g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + m_WindowSize.cy - 24, g_ZoneInfoTable[m_CurrentServerIndex-1].zoneshortname );
			
			if(g_CurrentZoneInfoIndex != 18 && g_CurrentZoneInfoIndex != 19 && g_CurrentZoneInfoIndex != 21 &&
				g_CurrentZoneInfoIndex != 58 ) //Author : 양희왕 //breif : 요동 추가
			{
				g_XBaseFont->SetColor( 0xFFCDC6A6 );
				
                //Author : 양희왕 //breif : 미국 맵 이름 길이 때문에 작을 땐 좌표 찍지 않음
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->PrintAlign( m_WindowPosition.x + 103, m_WindowPosition.y + 3 + 15, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 3 + 15, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
					/*g_XBaseFont->Print( m_WindowPosition.x + 42, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );*/
				}
				else
				{
					g_XBaseFont->Print( m_WindowPosition.x + 54, m_WindowPosition.y + 3 + 15, 1.0f, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + 3 + 15, 1.0f, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
				}

				//	g_XBaseFont->Print( m_WindowPosition.x + 54, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d", 
				//		_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				//	g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d",
				//		_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );	
			}
#ifdef _XDWDEBUG
			else
			{
				g_XBaseFont->SetColor( 0xFFCDC6A6 );
				
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->PrintAlign( m_WindowPosition.x + 103, m_WindowPosition.y + 3 + 15, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->PrintAlign( m_WindowPosition.x + 126, m_WindowPosition.y + 3 + 15, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
					/*g_XBaseFont->Print( m_WindowPosition.x + 42, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d", 
							_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d",
							_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );*/
				}
				else
				{
					g_XBaseFont->Print( m_WindowPosition.x + 54, m_WindowPosition.y + 3 + 15, 1.0f, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + 3 + 15, 1.0f, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
				}

			//	g_XBaseFont->SetColor( 0xFFCDC6A6 );
			//	g_XBaseFont->Print( m_WindowPosition.x + 54, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d", 
			//		_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
			//	g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d",
			//		_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );	
			}
#endif

			g_XBaseFont->SetColor( 0xFFD6C597 );

			int param1 = g_ServerGameTime.Year;
			int param2 = g_ServerGameTime.Month+1;
			int param3 = g_ServerGameTime.Day+1;
			
		//	g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + 3 + 15,
		//		g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAXIMUMMAP_DATE, &param1, &param2, &param3 ), 1.0f );
			g_XBaseFont->Flush();
			
			BOOL bDrawInformation = TRUE;
			if(g_CurrentZoneInfoIndex == 18 || g_CurrentZoneInfoIndex == 19)
			{
				bDrawInformation = FALSE;
			}

			if( m_bDrawInformationTooltip && bDrawInformation)
			{
				if( g_bShowItemMallWindow ) return;
				if( !g_pLocalUser->m_InParty )
				{
					if( m_SelectedNPCIndex != -1 )
					{							
						g_NPCNameWindow.m_EventType = g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_EventType;
						g_NPCNameWindow.Draw( m_TooltipTargetPosition_X - 72, m_TooltipTargetPosition_Y +5,
							g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCRoll,
							g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCName, 
							FALSE, _XNPC_HAVEQUEST_NONE,  g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCJobClass );	
					}
					else if( m_SelectedQuestID != -1 )
					{
						TCHAR messagestring[512];
						memset( messagestring, 0, sizeof(TCHAR)*512 );

						switch(m_SelectedQuestType) {
						case _XQUEST_TYPE_1://제룡행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE1), questscriptitem->GetQuestName() );	// "[제룡행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 255, 212, 0), FALSE );								
							}
							break;
						case _XQUEST_TYPE_2://무림행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE2), questscriptitem->GetQuestName() );	// "[무림행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 22, 146, 252), FALSE );
							}
							break;
						case _XQUEST_TYPE_3://협객행
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE3), questscriptitem->GetQuestName() );	// "[협객행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 245, 116, 34), FALSE );		
							}
							break;
						case _XQUEST_TYPE_4://기연행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE4), questscriptitem->GetQuestName() );	// "[기연행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 78, 229, 48), FALSE );	
							}
							break;
						case _XQUEST_TYPE_5://무림소사
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE5), questscriptitem->GetQuestName() );	// "[무림소사] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 255, 255, 255), FALSE );					
							}
							break;
						}						
						g_InterfaceTooltip.Draw();
					}
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
					else if( m_cPKTracingName[0] != 0 )
					{
						int lendth = strlen( m_cPKTracingName );
						g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									m_cPKTracingName, D3DCOLOR_ARGB(255, 255, 0, 0), FALSE );
						g_InterfaceTooltip.Draw();
					}
#endif
				}
				else
				{
					if( m_SelectedNPCIndex != -1 )
					{							
						g_NPCNameWindow.m_EventType = g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_EventType;
						g_NPCNameWindow.Draw( m_TooltipTargetPosition_X - 72, m_TooltipTargetPosition_Y +5,
							g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCRoll,
							g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCName, 
							FALSE, _XNPC_HAVEQUEST_NONE,  g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCJobClass );	
					}
					else if( m_SelectedQuestID != -1 )
					{
						TCHAR messagestring[512];
						memset( messagestring, 0, sizeof(TCHAR)*512 );
						
						switch(m_SelectedQuestType) {
						case _XQUEST_TYPE_1://제룡행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE1), questscriptitem->GetQuestName() );	// "[제룡행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 255, 212, 0), FALSE );								
							}
							break;
						case _XQUEST_TYPE_2://무림행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE2), questscriptitem->GetQuestName() );	// "[무림행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 22, 146, 252), FALSE );
							}
							break;
						case _XQUEST_TYPE_3://협객행
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE3), questscriptitem->GetQuestName() );	// "[협객행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 245, 116, 34), FALSE );		
							}
							break;
						case _XQUEST_TYPE_4://기연행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE4), questscriptitem->GetQuestName() );	// "[기연행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 78, 229, 48), FALSE );	
							}
							break;
						case _XQUEST_TYPE_5://무림소사
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE5), questscriptitem->GetQuestName() );	// "[무림소사] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 255, 255, 255), FALSE );					
							}
							break;
						}						
						g_InterfaceTooltip.Draw();
					}
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
					else if( m_cPKTracingName[0] != 0 )
					{
						int lendth = strlen( m_cPKTracingName );
						g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
							m_cPKTracingName, D3DCOLOR_ARGB(255, 255, 0, 0), FALSE );
						g_InterfaceTooltip.Draw();
					}
#endif
					else// 파티원 이름 
					{
						TCHAR messagestring[512];
						memset( messagestring, 0, sizeof(TCHAR)*512 );
						
						sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_PARTYMEMBER), g_pLocalUser->m_PartyInfo[m_SelectedPartyMemberIndex].playername );	// "[동행] %s"
						int lendth = strlen( messagestring );
						g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
							messagestring, _XSC_INFORMATION, FALSE );		
						g_InterfaceTooltip.Draw();			
					}
				}
			}
			
#ifdef _ACCLAIM_IGAADSYSTEM

			XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

			if( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
			    (g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && 
				 g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
			  )	// 내공심법 시전중
			{
				if( !pMainGameProcess->m_ProcessLocalUserDieFlag &&
					g_pLocalUser->GetMotionClass() != _XACTION_DYING && g_pLocalUser->GetMotionClass() != _XACTION_DIE )
				{
					if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NONE )
					{
						pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_MEDITATION;
						g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_MEDITATION );
						g_IGAADWrapper.ShowWindow( TRUE );
						g_IGAADWrapper.StartDisplay();
					}
					else if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_MEDITATION )
					{
						if( !g_IGAADWrapper.GetShowStatus() )
						{
							pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
						}
						else
						{
							g_IGAADWrapper.MoveWindowPosition( m_WindowPosition.x + _XDEF_MINIMAP_LEFT-1, m_WindowPosition.y + _XDEF_MINIMAP_TOP - 17 );
							g_IGAADWrapper.Render();
						}
					}
				}
			}
			else if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_MEDITATION ) // 운기요상이 끝났을 경우
			{
				if( g_IGAADWrapper.GetFadeMode() == 0 )
					g_IGAADWrapper.SetFadeMode();

				if( !pMainGameProcess->m_ProcessLocalUserDieFlag &&
					g_pLocalUser->GetMotionClass() != _XACTION_DYING && g_pLocalUser->GetMotionClass() != _XACTION_DIE )
				{
					if( !g_IGAADWrapper.GetShowStatus() )
					{
						pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
					}
					else
					{
						g_IGAADWrapper.MoveWindowPosition( m_WindowPosition.x + _XDEF_MINIMAP_LEFT-1, m_WindowPosition.y + _XDEF_MINIMAP_TOP - 17 );
						g_IGAADWrapper.Render();
					}
				}
			}
#endif	

		}
		else // Large mode
		{
			
			_XDrawSolidBar( m_WindowPosition.x + 2,  m_WindowPosition.y + 16 +15, 
				m_WindowPosition.x + m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy - 27, D3DCOLOR_ARGB(128,0,0,0) );
			
			_XDrawLine2D( m_WindowPosition.x+m_WindowSize.cx -2,  m_WindowPosition.y+17, 
				m_WindowPosition.x + m_WindowSize.cx - 2, m_WindowPosition.y+m_WindowSize.cy - 17, 0xFF000000, 0xFF000000 );
			_XDrawLine2D( m_WindowPosition.x+2,  m_WindowPosition.y+17, 
				m_WindowPosition.x + 2, m_WindowPosition.y+m_WindowSize.cy - 10, 0xFF000000, 0xFF000000 );		
			
			m_LargeMinimapImage.Draw( m_WindowPosition.x + 3, m_WindowPosition.y + 17 + 15, m_pMinimapTexture );

#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
			g_XBaseFont->SetColor(_XSC_DEFAULT_WHITE);
			g_XBaseFont->PutsAlign( m_WindowPosition.x + 155, m_WindowPosition.y+m_WindowSize.cy - 24, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3674) ); //투명도
#endif
			
			if(g_CurrentZoneInfoIndex != 18 && g_CurrentZoneInfoIndex != 19 && g_CurrentZoneInfoIndex != 21 &&
				g_CurrentZoneInfoIndex != 58 ) //Author : 양희왕 //breif : 요동 추가
			{
				if( !g_pLocalUser->m_bIsBossBattle )	DrawLargeMinipMapNPCPosition(pfocusobject);
				if( g_pLocalUser->m_InParty ) DrawLargeMinipMapPartyPlayerPosition(pfocusobject);		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
				DrawLargeMinipMapPKTracingPosition(pfocusobject);
#endif
				
				m_FrustumeImage.Draw();
			#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕추가
				m_MinimapArrow.RenderAlphaBlend(m_nTransparency * 0.39);
			#else
				m_MinimapArrow.RenderAlphaBlend();
			#endif
			}
#ifdef _XDWDEBUG
			else
			{
				m_FrustumeImage.Draw();
				
			#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕추가
				m_MinimapArrow.RenderAlphaBlend(m_nTransparency * 0.39);
			#else
				m_MinimapArrow.RenderAlphaBlend();
			#endif
			}			
#endif
			
			//PK범이 주변에 오면 경고 표시를 한다.
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
			if( m_bDrawPKWarning )
				DrawPKWarning();
#endif			
			m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 96, m_WindowPosition.y + m_WindowSize.cy - 27  );
			m_pMaximumMapBottomMiddleBar1->DrawWithRegion();
			m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 189 , m_WindowPosition.y + m_WindowSize.cy - 27  );
			m_pMaximumMapBottomMiddleBar1->DrawWithRegion();
			m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 282 , m_WindowPosition.y + m_WindowSize.cy - 27  );	
			m_pMaximumMapBottomMiddleBar1->DrawWithRegion();			
			m_pMaximumMapBottomMiddleBar2->MoveWindow( m_WindowPosition.x + 375 , m_WindowPosition.y + m_WindowSize.cy - 27  );
			m_pMaximumMapBottomMiddleBar2->DrawWithRegion();

			g_XBaseFont->SetColor( 0xFFD6C597 );

			int param1 = g_ServerGameTime.Year;
			int param2 = g_ServerGameTime.Month+1;
			int param3 = g_ServerGameTime.Day+1;
			
			//g_XBaseFont->Puts( m_WindowPosition.x + 173, m_WindowPosition.y + 3 + 15,
			//	g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAXIMUMMAP_DATE, &param1, &param2, &param3 ), 1.0f );
			g_XBaseFont->Flush();

			g_XBaseFont->SetColor( 0xFFCECECE );
			if( g_ZeroLevelMode )	// hotblood zerolevel
				g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + m_WindowSize.cy - 24, _XGETINTERFACETEXT(ID_STRING_WORLDMINIMAP_NAMKYUNG) );	// _T("남경")
			else
			{
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					g_XBaseFont->Puts( m_WindowPosition.x + 4, m_WindowPosition.y + 3 + 15, g_ZoneInfoTable[m_CurrentServerIndex-1].zoneshortname );
				else
					g_XBaseFont->Puts( m_WindowPosition.x + 4 + 173, m_WindowPosition.y + 3 + 15, g_ZoneInfoTable[m_CurrentServerIndex-1].zoneshortname );
			}
			
			if(g_CurrentZoneInfoIndex != 18 && g_CurrentZoneInfoIndex != 19 && g_CurrentZoneInfoIndex != 21 &&
				g_CurrentZoneInfoIndex != 58 ) //Author : 양희왕 //breif : 요동 추가
			{
				g_XBaseFont->SetColor( 0xFFCDC6A6 );
				
				if( /*g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE ||*/ g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->Print( m_WindowPosition.x + 42, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
				}
				else
				{
					g_XBaseFont->Print( m_WindowPosition.x + 54 + 173, m_WindowPosition.y + 3 + 15, 1.0f, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->Print( m_WindowPosition.x + 76 + 173, m_WindowPosition.y + 3 + 15, 1.0f, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
				}

				//g_XBaseFont->Print( m_WindowPosition.x + 54, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d", 
				//	_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				//g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d",
				//	_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );	
				g_XBaseFont->Flush();
			}
#ifdef _XDWDEBUG
			else
			{
				g_XBaseFont->SetColor( 0xFFCDC6A6 );
				
				if( /*g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE ||*/ g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN  )
				{
					g_XBaseFont->Print( m_WindowPosition.x + 42, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
				}
				else
				{
					g_XBaseFont->Print( m_WindowPosition.x + 54 + 173, m_WindowPosition.y + 3 + 15, 1.0f, "%d", 
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
					g_XBaseFont->Print( m_WindowPosition.x + 76 + 173, m_WindowPosition.y + 3 + 15, 1.0f, "%d",
						_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );
				}

				//g_XBaseFont->Print( m_WindowPosition.x + 54, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d", 
				//	_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.x));
				//g_XBaseFont->Print( m_WindowPosition.x + 76, m_WindowPosition.y + m_WindowSize.cy - 24, 1.0f, "%d",
				//	_XFC_FloatToIntRet(g_LodTerrain.m_HalfWorldSize+g_pLocalUser->m_Position.z) );	
				//g_XBaseFont->Flush();
			}			
#endif
			g_XBaseFont->Flush();

			BOOL bDrawInformation = TRUE;
			if(g_CurrentZoneInfoIndex == 18 || g_CurrentZoneInfoIndex == 19)
			{
				bDrawInformation = FALSE;
			}

			if( m_bDrawInformationTooltip && bDrawInformation)
			{
				if( g_bShowItemMallWindow ) return;
				if( !g_pLocalUser->m_InParty )
				{
					if( m_SelectedNPCIndex != -1 )
					{							
						g_NPCNameWindow.m_EventType = g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_EventType;
						g_NPCNameWindow.Draw( m_TooltipTargetPosition_X - 72, m_TooltipTargetPosition_Y +5,
							g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCRoll,
							g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCName, 
							FALSE, _XNPC_HAVEQUEST_NONE,  g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCJobClass );	
					}
					else if( m_SelectedQuestID != -1 )
					{
						TCHAR messagestring[512];
						memset( messagestring, 0, sizeof(TCHAR)*512 );

						switch(m_SelectedQuestType) {
						case _XQUEST_TYPE_1://제룡행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE1), questscriptitem->GetQuestName() );	// "[제룡행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 255, 212, 0), FALSE );								
							}
							break;
						case _XQUEST_TYPE_2://무림행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE2), questscriptitem->GetQuestName() );	// "[무림행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 22, 146, 252), FALSE );
							}
							break;
						case _XQUEST_TYPE_3://협객행
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE3), questscriptitem->GetQuestName() );	// "[협객행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 245, 116, 34), FALSE );		
							}
							break;
						case _XQUEST_TYPE_4://기연행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE4), questscriptitem->GetQuestName() );	// "[기연행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 78, 229, 48), FALSE );	
							}
							break;
						case _XQUEST_TYPE_5://무림소사
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE5), questscriptitem->GetQuestName() );	// "[무림소사] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 255, 255, 255), FALSE );					
							}
							break;
						}						
						g_InterfaceTooltip.Draw();
					}
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
					else if( m_cPKTracingName[0] != 0 )
					{
						int lendth = strlen( m_cPKTracingName );
						g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
							m_cPKTracingName, D3DCOLOR_ARGB(255, 255, 0, 0), FALSE );
						g_InterfaceTooltip.Draw();
					}
#endif
				}
				else
				{
					if( m_SelectedNPCIndex != -1 )
					{							
						g_NPCNameWindow.m_EventType = g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_EventType;
						g_NPCNameWindow.Draw( m_TooltipTargetPosition_X - 72, m_TooltipTargetPosition_Y +5,
							g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCRoll,
							g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCName, 
							FALSE, _XNPC_HAVEQUEST_NONE,  g_Npclist.m_pNPCObjectArray[m_SelectedNPCIndex].m_NPCJobClass );	
					}
					else if( m_SelectedQuestID != -1 )
					{
						TCHAR messagestring[512];
						memset( messagestring, 0, sizeof(TCHAR)*512 );
						
						switch(m_SelectedQuestType) {
						case _XQUEST_TYPE_1://제룡행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE1), questscriptitem->GetQuestName() );	// "[제룡행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 255, 212, 0), FALSE );								
							}
							break;
						case _XQUEST_TYPE_2://무림행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE2), questscriptitem->GetQuestName() );	// "[무림행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 22, 146, 252), FALSE );
							}
							break;
						case _XQUEST_TYPE_3://협객행
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE3), questscriptitem->GetQuestName() );	// "[협객행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 245, 116, 34), FALSE );		
							}
							break;
						case _XQUEST_TYPE_4://기연행 
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE4), questscriptitem->GetQuestName() );	// "[기연행] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 78, 229, 48), FALSE );	
							}
							break;
						case _XQUEST_TYPE_5://무림소사
							{
								_XQuestScriptItem* questscriptitem = g_QuestScriptManager.m_mapSubQuestScriptList[m_SelectedQuestID];
								sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_QUESTTYPE5), questscriptitem->GetQuestName() );	// "[무림소사] %s"
								int lendth = strlen( messagestring );
								g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
									messagestring, D3DCOLOR_ARGB(255, 255, 255, 255), FALSE );					
							}
							break;
						}						
						g_InterfaceTooltip.Draw();
					}
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
					else if( m_cPKTracingName[0] != 0 )
					{
						int lendth = strlen( m_cPKTracingName );
						g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
							m_cPKTracingName, D3DCOLOR_ARGB(255, 255, 0, 0), FALSE );
						g_InterfaceTooltip.Draw();
					}
#endif
					else// 파티원 이름 
					{
						TCHAR messagestring[512];
						memset( messagestring, 0, sizeof(TCHAR)*512 );
						
						sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_MAXIMUMMAP_PARTYMEMBER), g_pLocalUser->m_PartyInfo[m_SelectedPartyMemberIndex].playername );	// "[동행] %s"
						int lendth = strlen( messagestring );
						g_InterfaceTooltip.SetToolTipString( m_TooltipTargetPosition_X - lendth*6 - 15, m_TooltipTargetPosition_Y +10,
							messagestring, _XSC_INFORMATION, FALSE );		
						g_InterfaceTooltip.Draw();			
					}
				}
			}//if( m_bDrawInformationTooltip )

#ifdef _ACCLAIM_IGAADSYSTEM

			XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

			if( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
			    (g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && 
				 g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
			  )	// 내공심법 시전중
			{
				if( !pMainGameProcess->m_ProcessLocalUserDieFlag &&
					g_pLocalUser->GetMotionClass() != _XACTION_DYING && g_pLocalUser->GetMotionClass() != _XACTION_DIE )
				{
					if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_NONE )
					{
						pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_MEDITATION;
						g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_MEDITATION );
						g_IGAADWrapper.ShowWindow( TRUE );
						g_IGAADWrapper.StartDisplay();
					}
					else if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_MEDITATION )
					{
						if( !g_IGAADWrapper.GetShowStatus() )
						{
							pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
						}
						else
						{
							g_IGAADWrapper.MoveWindowPosition( m_WindowPosition.x + 3, m_WindowPosition.y + 17 + 15);
							g_IGAADWrapper.SetUseSecondWindowSize( TRUE, 450, 450 );
							g_IGAADWrapper.Render();
							g_IGAADWrapper.SetUseSecondWindowSize( FALSE );
						}
					}
				}
			}
			else if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_MEDITATION ) // 운기요상이 끝났을 경우
			{
				if( g_IGAADWrapper.GetFadeMode() == 0 )
					g_IGAADWrapper.SetFadeMode();

				if( !pMainGameProcess->m_ProcessLocalUserDieFlag &&
					g_pLocalUser->GetMotionClass() != _XACTION_DYING && g_pLocalUser->GetMotionClass() != _XACTION_DIE )
				{
					if( !g_IGAADWrapper.GetShowStatus() )
					{
						pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
					}
					else
					{
						g_IGAADWrapper.MoveWindowPosition( m_WindowPosition.x + 3, m_WindowPosition.y + 17 + 15 );
						g_IGAADWrapper.SetUseSecondWindowSize( TRUE, 450, 450 );
						g_IGAADWrapper.Render();
						g_IGAADWrapper.SetUseSecondWindowSize( FALSE );
					}
				}
			}
#endif
			
		}
	}

	if( !m_MinimizeStatus )
	{
		if( m_MiniMapMode == _XMINIMAPMODE_LARGE )
		{
			if( g_pLocalUser->m_CharacterInfo.Get_gm_level() > 0 )
			{
				if( gpInput->CheckMousePos( m_WindowPosition.x + 3, m_WindowPosition.y + 16 + 15, 
											m_WindowPosition.x + 453, m_WindowPosition.y + 466 + 15 ) )
				{
					ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
					_XDrawLine2D( m_WindowPosition.x + 3, scrnpos->z, m_WindowPosition.x + 453, scrnpos->z, 0x7FAC7E36, 0x7FAC7E36 );
					_XDrawLine2D( scrnpos->x, m_WindowPosition.y + 16 + 15, scrnpos->x, m_WindowPosition.y + 466 + 15, 0x7FAC7E36, 0x7FAC7E36 );
				}
			}
#ifdef _XDWDEBUG
			else
			{
				if( gpInput->CheckMousePos( m_WindowPosition.x + 3, m_WindowPosition.y + 16 + 15, 
					m_WindowPosition.x + 453, m_WindowPosition.y + 466 + 15 ) )
				{
					ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
					_XDrawLine2D( m_WindowPosition.x + 3, scrnpos->z, m_WindowPosition.x + 453, scrnpos->z, 0x7FAC7E36, 0x7FAC7E36 );
					_XDrawLine2D( scrnpos->x, m_WindowPosition.y + 16 + 15, scrnpos->x, m_WindowPosition.y + 466 + 15, 0x7FAC7E36, 0x7FAC7E36 );
				}
			}
#endif
		}
	}
	
#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
	if( m_pAlphaSlider )
		m_pAlphaSlider->Draw(pfocusobject); //Author : 양희왕
#endif

	m_pSunMoonIcon->DrawToolTip( pfocusobject );
}

void _XWindow_WorldMinimap::ProcessDblClick()
{	
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(m_MiniMapMode == _XMINIMAPMODE_SMALL )
	{
		if(scrnpos->x > m_WindowPosition.x )
		{
			if( scrnpos->x < (m_WindowPosition.x + 154 ) )
			{
				if( scrnpos->z > m_WindowPosition.y )
				{
					if( scrnpos->z < (m_WindowPosition.y + 17) )
					{
						ChangeMinimizeStatus();
					}
				}
			}
		}
	}
	else
	{
		if(scrnpos->x > m_WindowPosition.x )
		{
			if( scrnpos->x < (m_WindowPosition.x + 454 ) )
			{
				if( scrnpos->z > m_WindowPosition.y )
				{
					if( scrnpos->z < (m_WindowPosition.y + 17) )
					{
						ChangeMinimizeStatus();
					}
				}
			}
		}

		if( !m_MinimizeStatus )
		{
			if( m_MiniMapMode == _XMINIMAPMODE_LARGE )
			{
				if( g_pLocalUser->m_CharacterInfo.Get_gm_level() > 0 )
				{
					if( gpInput->CheckMousePos( m_WindowPosition.x + 3, m_WindowPosition.y + 16 + 15, 
												m_WindowPosition.x + 453, m_WindowPosition.y + 466 + 15 ) )
					{
						ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
						
						FLOAT sizefactor = g_LodTerrain.m_Width / 512.0f;
						
						int xcoord = (int)( ((FLOAT)(scrnpos->x - (m_WindowPosition.x + 3)) * 1.12109375f) * sizefactor );
						
						FLOAT zpos = ((FLOAT) (scrnpos->z - (m_WindowPosition.y + 16 + 15)) * 1.12109375f) * sizefactor;
						
						int zcoord = (int)( (g_LodTerrain.m_Height - zpos) );						

						/*
						int xcoord = 0;
						int zcoord = 0;

						int screenxcoord = scrnpos->x - (m_WindowPosition.x + 3);
						int screenzcoord = scrnpos->z - (m_WindowPosition.y + 16 + 15);
						FLOAT sizefactor = g_LodTerrain.m_Width / 512.0f;

						xcoord = (int)( ((FLOAT)screenxcoord * sizefactor ) );
						zcoord = g_LodTerrain.m_Height - (int)( (FLOAT)screenzcoord * sizefactor );
						*/

						if( xcoord < 0 )
							xcoord = 0;

						if( xcoord > g_LodTerrain.m_Width - 1 )
							xcoord = g_LodTerrain.m_Width - 1;

						if( zcoord < 0 )
							zcoord = 0;

						if( zcoord > g_LodTerrain.m_Height -1 )
							zcoord = g_LodTerrain.m_Height -1;

						_XWindow_GM_UserCoordination* pWindow = (_XWindow_GM_UserCoordination*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GMCHARACTERCOORDSETTINGWINDOW);
						if( pWindow )
						{
							TCHAR buffer[128];
							memset( buffer, 0, sizeof(TCHAR) * 128 );							
							itoa( xcoord, buffer, 10 );
							pWindow->m_XCoordIMEControl.ClearBuffer();
							pWindow->m_XCoordIMEControl.AddString( buffer, strlen(buffer) );

							memset( buffer, 0, sizeof(TCHAR) * 128 );
							itoa( zcoord, buffer, 10 );
							pWindow->m_ZCoordIMEControl.ClearBuffer();
							pWindow->m_ZCoordIMEControl.AddString( buffer, strlen(buffer) );
						}
					}
				}
#ifdef _XDWDEBUG
				else 
				{
					if( gpInput->CheckMousePos( m_WindowPosition.x + 3, m_WindowPosition.y + 16 + 15, 
						m_WindowPosition.x + 453, m_WindowPosition.y + 466 + 15 ) )
					{
						ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
						
						FLOAT sizefactor = g_LodTerrain.m_Width / (float)(gnWidth>>1);

						int xcoord = (int)( ((FLOAT)(scrnpos->x - (m_WindowPosition.x + 3)) * 1.12109375f) * sizefactor );

						FLOAT zpos = ((FLOAT) (scrnpos->z - (m_WindowPosition.y + 16 + 15)) * 1.12109375f) * sizefactor;

						int zcoord = (int)( (g_LodTerrain.m_Height - zpos) /*- g_LodTerrain.m_HalfWorldSize*/ );
						
						if( xcoord < 0 )
							xcoord = 0;
						
						if( xcoord > g_LodTerrain.m_Width - 1 )
							xcoord = g_LodTerrain.m_Width - 1;
						
						if( zcoord < 0 )
							zcoord = 0;
						
						if( zcoord > g_LodTerrain.m_Height -1 )
							zcoord = g_LodTerrain.m_Height -1;
						
						_XWindow_Debug* pDebugWindow = (_XWindow_Debug*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_XDEBUGWINDOW );
						if( pDebugWindow && pDebugWindow->GetShowStatus() )
						{
							TCHAR buffer[64];
							memset( buffer, 0, sizeof(TCHAR) * 64 );

							sprintf( buffer, "coord curuser %d %d", xcoord, zcoord );

							g_DebugCommandEditContainer.ClearBuffer();
							g_DebugCommandEditContainer.AddString( buffer, strlen(buffer) );
						}
					}
				}
#endif
			}
		}
	}
}

BOOL _XWindow_WorldMinimap::Process( _XGUIObject*& pfocusobject )
{
	if( !_XWindow::Process(pfocusobject) ) return FALSE;	

	// Tutorial : 2005.03.08->hotblood 
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_001 && CheckMousePosition() )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_001);
			g_TutorialInfo->_XTutorial_001 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif			
		}
	}	

	if( m_MiniMapMode == _XMINIMAPMODE_SMALL )
	{
		if( zoominbutton && !m_bDisableZoomButton )
		{
			if( g_CurrentFocusedObject == (_XGUIObject*)zoominbutton &&
				gpInput->GetMouseState()->bButton[0] )
			{
				m_MinimapZoomRate /= 1.05f;
				
				if( m_MinimapZoomRate < 0.075f )
					m_MinimapZoomRate = 0.075f;
				
				m_RateOfMapSizeToTerrain *= 1.05f;
				
				if(m_RateOfMapSizeToTerrain > 1.0f*(1024.0f/g_LodTerrain.m_Width))
					m_RateOfMapSizeToTerrain = 1.0f*(1024.0f/g_LodTerrain.m_Width);
				
				SetLocalUserPosition();
			}
		}
		
		if( zoomoutbutton && !m_bDisableZoomButton )
		{
			if( g_CurrentFocusedObject == (_XGUIObject*)zoomoutbutton &&
				gpInput->GetMouseState()->bButton[0] )
			{
				m_MinimapZoomRate *= 1.05f;
				
				if( m_MinimapZoomRate > 0.25f )
					m_MinimapZoomRate = 0.25f;
				
				m_RateOfMapSizeToTerrain /= 1.05;
				
				if(m_RateOfMapSizeToTerrain < 0.3f*(1024.0f/g_LodTerrain.m_Width))
					m_RateOfMapSizeToTerrain = 0.3f*(1024.0f/g_LodTerrain.m_Width);
				
				SetLocalUserPosition();
			}
		}
	}
	else
	{
		SetLocalUserPosition();
	}
	
#ifdef _XTS_SHOWTUTORIAL
	if( g_pLocalUser->m_CharacterInfo.Get_innerlevel() < 10 )
		helpbookbutton->Process(pfocusobject);
#else
	if( g_pLocalUser->m_CharacterInfo.Get_innerlevel() < 0 )
		helpbookbutton->Process(pfocusobject);
#endif

	if( !CheckMousePosition()) // && !m_bDrawPartyMemberSearchEffect)
	{
		m_bDrawInformationTooltip = FALSE;
	}

#ifdef _ACCLAIM_IGAADSYSTEM
	if( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
		(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && 
		 g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST)
	  )	// 내공심법 시전중
	{
		XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;

		if( pMainGameProcess->m_AcclaimADMode_MainGame == _XACCLAIMAD_MAINGAME_MEDITATION )
		{
			g_IGAADWrapper.Process();
		}
	}
#endif
		
	return TRUE;
}

void _XWindow_WorldMinimap::DrawNPCPosition(_XGUIObject*& pfocusobject)
{		
	// 10이상이면 화면에 표시할 NPC 리스트를 갱신한다
	int check_interval_X = abs(g_pLocalUser->m_Position.x-m_CheckLocalUserPosition_X); 
	int check_interval_Y = abs(g_pLocalUser->m_Position.z-m_CheckLocalUserPosition_Y);
	if(( check_interval_X > 10.0f )||( check_interval_Y > 10.0f )) 
	{		
		m_CheckLocalUserPosition_X = g_pLocalUser->m_Position.x;
		m_CheckLocalUserPosition_Y = g_pLocalUser->m_Position.z;
		RebuildDrawNPCList( m_CheckLocalUserPosition_X, m_CheckLocalUserPosition_Y );
	}

	m_bDrawInformationTooltip = FALSE;
	if( !m_listDrawNPCIndex.empty() )
	{		
		list <int>::iterator iter_drawnpc;
		for(iter_drawnpc = m_listDrawNPCIndex.begin() ; iter_drawnpc != m_listDrawNPCIndex.end() ; ++iter_drawnpc)
		{			
			int npcindex = *iter_drawnpc;			
			if( g_Npclist.m_pNPCObjectArray[npcindex].m_bShowNPCModel )
			{
				int Map_NPC_X = (int)(((int)g_Npclist.m_pNPCObjectArray[npcindex].m_matWorldPosition._41 - (int)g_pLocalUser->m_Position.x) * m_RateOfMapSizeToTerrain);
				int Map_NPC_Z = -(int)(((int)g_Npclist.m_pNPCObjectArray[npcindex].m_matWorldPosition._43 - (int)g_pLocalUser->m_Position.z) * m_RateOfMapSizeToTerrain);
				
				if((_XDEF_MINIMAP_ORIGIN_X + Map_NPC_X > _XDEF_MINIMAP_LEFT + 4) && (_XDEF_MINIMAP_ORIGIN_X + Map_NPC_X < _XDEF_MINIMAP_RIGHT))
				{
					if((_XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z < _XDEF_MINIMAP_BOTTOM) && (_XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z > _XDEF_MINIMAP_TOP + 4))
					{						
						if(g_Npclist.m_pNPCObjectArray[npcindex].m_NPCRollType == _XNPCROLL_TRADE || g_Npclist.m_pNPCObjectArray[npcindex].m_NPCRollType == _XNPCROLL_WAREHOUSE )
						{
							m_NPCImageTradeType.MoveWindow(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 4 + Map_NPC_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 4 + Map_NPC_Z, 
								m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 3 + Map_NPC_X,
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 3 + Map_NPC_Z);
							m_NPCImageTradeType.DrawWithRegion();	
							
							if( !m_bDrawInformationTooltip )
							{
								ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
								if((scrnpos->x > m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 4 + Map_NPC_X) )
								{
									if( scrnpos->x < m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 3 + Map_NPC_X )
									{
										if((scrnpos->z > m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 4 + Map_NPC_Z) )
										{
											if(scrnpos->z < m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 3 + Map_NPC_Z)
											{
												// NPC 위치에 오버됨. NPC 정보를 표시한다. 
												m_SelectedNPCIndex = npcindex;				
												m_TooltipTargetPosition_X		= m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + Map_NPC_X;
												m_TooltipTargetPosition_Y		= m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z;
												m_bDrawInformationTooltip = TRUE;
												
												m_SelectedQuestID				= -1;
												m_SelectedPartyMemberIndex		= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
												memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
											}
										}
									}
								}
							}//if( !m_bDrawInformationTooltip )
						}
						else if( g_Npclist.m_pNPCObjectArray[npcindex].m_NPCRollType == _XNPCROLL_EVENT )
						{
							m_NPCImageEventType.MoveWindow(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 4 + Map_NPC_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 4 + Map_NPC_Z, 
								m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 5 + Map_NPC_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 5 + Map_NPC_Z);
							m_NPCImageEventType.DrawWithRegion();	
							
							if( !m_bDrawInformationTooltip )
							{
								ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
								if((scrnpos->x > m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 4 + Map_NPC_X) )
								{
									if( scrnpos->x < m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 3 + Map_NPC_X )
									{
										if((scrnpos->z > m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 4 + Map_NPC_Z) )
										{
											if(scrnpos->z < m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 3 + Map_NPC_Z)
											{
												// NPC 위치에 오버됨. NPC 정보를 표시한다. 
												m_SelectedNPCIndex = npcindex;				
												m_TooltipTargetPosition_X		= m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + Map_NPC_X;
												m_TooltipTargetPosition_Y		= m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z;
												m_bDrawInformationTooltip = TRUE;
												
												m_SelectedQuestID				= -1;
												m_SelectedPartyMemberIndex		= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
												memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
											}
										}
									}
								}
							}//if( !m_bDrawInformationTooltip )
						}					
					}
				}		
			}	
		}		
	}
	
	if( !g_QuestScriptManager.m_listQuestMark.empty() )
	{		
		
		if(m_PrevDrawTime == 0)
			m_PrevDrawTime = g_LocalSystemTime;
		
		DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
		
		if(!m_EffectOrderSub)
			m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
		else 
			m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);
		
		if(m_EffectFrame > 4.0f)
		{
			m_EffectOrderSub = true;
			m_EffectFrame = 3.0f;
		}
		else if(m_EffectFrame <= 0.0f)
		{
			m_EffectOrderSub = false;
			m_EffectFrame = 0.0f;
		}
		
		int selectedframe = (int)m_EffectFrame;
		
		if(selectedframe > 4)
			selectedframe = 3;
		else if(selectedframe < 0)
			selectedframe = 0;
		
		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
		{
			_XQUEST_MARKINFO questmark = *iter_questmark;
			
			int questmarkpositionX = (int)(((int)questmark.MarkPosition_X - (int)g_pLocalUser->m_Position.x) * m_RateOfMapSizeToTerrain);
			int questmarkpositionY = -(int)(((int)questmark.MarkPosition_Y - (int)g_pLocalUser->m_Position.z) * m_RateOfMapSizeToTerrain);
				
			if((_XDEF_MINIMAP_ORIGIN_X + questmarkpositionX > _XDEF_MINIMAP_LEFT + 8) && (_XDEF_MINIMAP_ORIGIN_X + questmarkpositionX < _XDEF_MINIMAP_RIGHT))
			{
				if((_XDEF_MINIMAP_ORIGIN_Y + questmarkpositionY < _XDEF_MINIMAP_BOTTOM) && (_XDEF_MINIMAP_ORIGIN_Y + questmarkpositionY > _XDEF_MINIMAP_TOP + 8))
				{
						// 현재 진행되는 퀘스트의 종류에 따라 색을 바꿔서 표시한다.
					switch( questmark.questtype ) {
					case _XQUEST_TYPE_1://제룡행 
						{
							m_YellowImpressionMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + questmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + questmarkpositionY);							
						}
						break;
					case _XQUEST_TYPE_2://무림행 
						{
							m_BlueImpressionMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + questmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + questmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_3://협객행
						{
							m_OrangeImpressionMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + questmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + questmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_4://기연행 
						{
							m_GreenImpressionMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + questmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + questmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_5://무림소사
						{
							m_WhiteImpressionMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + questmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + questmarkpositionY);
						}
						break;
					}

					if( !m_bDrawInformationTooltip )
					{
						ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
						if((scrnpos->x > m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 9 + questmarkpositionX) )
						{
							if( scrnpos->x < m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 10 + questmarkpositionX )
							{
								if((scrnpos->z > m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 9 + questmarkpositionY) )
								{
									if(scrnpos->z < m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 10 + questmarkpositionY)
									{
										// Quest 위치에 오버됨. Quest 정보를 표시한다. 
										m_SelectedQuestType			= questmark.questtype;
										m_SelectedQuestID			= questmark.questid;
										
										m_TooltipTargetPosition_X	= m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + questmarkpositionX;
										m_TooltipTargetPosition_Y	= m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + questmarkpositionY;
										m_bDrawInformationTooltip	= TRUE;

										m_SelectedPartyMemberIndex		= -1;
										m_SelectedNPCIndex				= -1;	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
										memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
									}
								}
							}
						}
					}//if( !m_bDrawInformationTooltip )
				}
			}
		}
		
		m_PrevDrawTime = g_LocalSystemTime;
	}
	if( !g_QuestScriptManager.m_listHintMark.empty() )
	{		
		list <_XQUEST_MARKINFO>::iterator iter_hintmark;
		for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hintmark)
		{
			_XQUEST_MARKINFO hintmark = *iter_hintmark;
			
			int hintmarkpositionX = (int)(((int)hintmark.MarkPosition_X - (int)g_pLocalUser->m_Position.x) * m_RateOfMapSizeToTerrain);
			int hinttmarkpositionY = -(int)(((int)hintmark.MarkPosition_Y - (int)g_pLocalUser->m_Position.z) * m_RateOfMapSizeToTerrain);
			
			if((_XDEF_MINIMAP_ORIGIN_X + hintmarkpositionX > _XDEF_MINIMAP_LEFT + 8) && (_XDEF_MINIMAP_ORIGIN_X + hintmarkpositionX < _XDEF_MINIMAP_RIGHT))
			{
				if((_XDEF_MINIMAP_ORIGIN_Y + hinttmarkpositionY < _XDEF_MINIMAP_BOTTOM) && (_XDEF_MINIMAP_ORIGIN_Y + hinttmarkpositionY > _XDEF_MINIMAP_TOP + 8))
				{
					// 현재 진행되는 퀘스트의 종류에 따라 색을 바꿔서 표시한다.
					switch( hintmark.questtype ) {
					case _XQUEST_TYPE_1://제룡행 
						{
							m_YellowQuestionMark->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + hintmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + hinttmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_2://무림행 
						{
							m_BlueQuestionMark->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + hintmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + hinttmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_3://협객행
						{
							m_OrangeQuestionMark->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + hintmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + hinttmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_4://기연행 
						{
							m_GreenQuestionMark->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + hintmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + hinttmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_5://무림소사
						{
							m_WhiteQuestionMark->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 7 + hintmarkpositionX, m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 7 + hinttmarkpositionY);
						}
						break;
					}
					if( !m_bDrawInformationTooltip )
					{
						ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
						if((scrnpos->x > m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 9 + hintmarkpositionX) )
						{
							if( scrnpos->x < m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 10 + hintmarkpositionX )
							{
								if((scrnpos->z > m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 9 + hinttmarkpositionY) )
								{
									if(scrnpos->z < m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 10 + hinttmarkpositionY)
									{
										// Quest 위치에 오버됨. Quest 정보를 표시한다. 
										m_SelectedQuestType			= hintmark.questtype;
										m_SelectedQuestID			= hintmark.questid;
										
										m_TooltipTargetPosition_X	= m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + hintmarkpositionX;
										m_TooltipTargetPosition_Y	= m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + hinttmarkpositionY;
										m_bDrawInformationTooltip	= TRUE;

										m_SelectedPartyMemberIndex		= -1;
										m_SelectedNPCIndex				= -1;	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
										memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
									}
								}
							}
						}
					}//if( !m_bDrawInformationTooltip )
				}
			}		
		}
	}

	//NPC 찾기 기능에서 선택된 NPC 직책에 관련된 NPC 전부를 처리
	if( m_bDrawNPCSearchEffect )
	{
		list <int>::iterator iter_drawnpc;
		for(iter_drawnpc = m_listDrawNPCIndex.begin() ; iter_drawnpc != m_listDrawNPCIndex.end() ; ++iter_drawnpc)
		{			
			int npcindex = *iter_drawnpc;			
			if( g_Npclist.m_pNPCObjectArray[npcindex].m_bShowNPCModel )
			{						
				if( g_Npclist.m_pNPCObjectArray[npcindex].m_NPCJobClass == m_SelectedNPCJobIndex )
				{
					int Map_NPC_X = (int)(((int)g_Npclist.m_pNPCObjectArray[npcindex].m_matWorldPosition._41 - (int)g_pLocalUser->m_Position.x) * m_RateOfMapSizeToTerrain);
					int Map_NPC_Z = -(int)(((int)g_Npclist.m_pNPCObjectArray[npcindex].m_matWorldPosition._43 - (int)g_pLocalUser->m_Position.z) * m_RateOfMapSizeToTerrain);
					
					if((_XDEF_MINIMAP_ORIGIN_X + Map_NPC_X > _XDEF_MINIMAP_LEFT + 4) && (_XDEF_MINIMAP_ORIGIN_X + Map_NPC_X < _XDEF_MINIMAP_RIGHT))
					{
						if((_XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z < _XDEF_MINIMAP_BOTTOM) && (_XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z > _XDEF_MINIMAP_TOP + 4))
						{					
							
							if(m_PrevDrawTimeNPCSearch == 0)
								m_PrevDrawTimeNPCSearch = g_LocalSystemTime;
							
							DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTimeNPCSearch;
							
							m_EffectFrameNPCSearch += ((FLOAT)elapsedtime / 250.0f);
							
							if(m_EffectFrameNPCSearch >= 4.0f)
							{
								m_EffectFrameNPCSearch = 3.0f;
							}
							
							int selectedframe = (int)m_EffectFrameNPCSearch;
							
							m_imageNPCSearchMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + Map_NPC_X - 17, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + Map_NPC_Z - 17);
							
							if( selectedframe == 3 )
								m_EffectFrameNPCSearch = 0.0f;
							
							m_PrevDrawTimeNPCSearch = g_LocalSystemTime;
						}
					}
				}
			}
		}
	}

	if( !m_bDrawInformationTooltip )
	{
		m_SelectedQuestID				= -1;
//		m_SelectedPartyMemberIndex		= -1;
		m_SelectedNPCIndex				= -1;		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
		memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
	}
	
}

void _XWindow_WorldMinimap::DrawPartyPlayerPosition(_XGUIObject*& pfocusobject)
{	
	for( int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i )
	{
		if( g_pLocalUser->m_PartyInfo[i].playername[0] != 0 && g_pLocalUser->m_PartyInfo[i].isValid)
		{

			BOOL isCurrentUser = FALSE;
			CheckCurrentUser(i,isCurrentUser);
			
			if(!isCurrentUser)
			{
				if(g_pLocalUser->m_PartyInfo[i].serverno == g_CurrentZoneInfoIndex)
				{
					int Map_PartyPlayer_X = (int)(((int)g_pLocalUser->m_PartyInfo[i].posX - (int)g_pLocalUser->m_Position.x) * m_RateOfMapSizeToTerrain);
					int Map_PartyPlayer_Z = -(int)(((int)g_pLocalUser->m_PartyInfo[i].posZ - (int)g_pLocalUser->m_Position.z) * m_RateOfMapSizeToTerrain);
					
					if((_XDEF_MINIMAP_ORIGIN_X + Map_PartyPlayer_X > _XDEF_MINIMAP_LEFT + 4) && (_XDEF_MINIMAP_ORIGIN_X + Map_PartyPlayer_X < _XDEF_MINIMAP_RIGHT))
					{
						if((_XDEF_MINIMAP_ORIGIN_Y + Map_PartyPlayer_Z < _XDEF_MINIMAP_BOTTOM) && (_XDEF_MINIMAP_ORIGIN_Y + Map_PartyPlayer_Z > _XDEF_MINIMAP_TOP + 4))
						{
							m_PartyPlayerImage.MoveWindow(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 3 + Map_PartyPlayer_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 3 + Map_PartyPlayer_Z, 
								m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 2 + Map_PartyPlayer_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 2 + Map_PartyPlayer_Z);
							m_PartyPlayerImage.DrawWithRegion();
							
							if(m_bDrawPartyMemberSearchEffect)
							{
								if(m_nSearchPartyMemberIndex == i)
								{
									if(m_dwPrevDrawTimePartySearch == 0)
										m_dwPrevDrawTimePartySearch = g_LocalSystemTime;

									DWORD elapsedtime = g_LocalSystemTime - m_dwPrevDrawTimePartySearch;

									m_fEffectFramePartySearch += ((FLOAT)elapsedtime / 200.f);

									if(m_fEffectFramePartySearch >= 4.0f)
									{
										m_fEffectFramePartySearch = 3.0f;
									}

									int selectedframe = (int)m_fEffectFramePartySearch;

									m_imageNPCSearchMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + Map_PartyPlayer_X - 17+2, 
										m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + Map_PartyPlayer_Z - 17+2);

									g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + Map_PartyPlayer_X - 17,
										m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + Map_PartyPlayer_Z - 29,
										g_pLocalUser->m_PartyInfo[i].playername, _XSC_INFORMATION, FALSE );		
									g_InterfaceTooltip.Draw();

									if(selectedframe == 3)
									{
										m_fEffectFramePartySearch = 0.0f;
										m_nTotalDrawTimePartySearch++;
									}

									m_dwPrevDrawTimePartySearch = g_LocalSystemTime;

									if(m_nTotalDrawTimePartySearch > 4)
									{
										// 5번 깜빡임 이상이면 draw 중지
										m_bDrawPartyMemberSearchEffect = FALSE;
										m_nTotalDrawTimePartySearch = 0;
										m_fEffectFramePartySearch = 0;
										m_dwPrevDrawTimePartySearch = 0;
										m_nSearchPartyMemberIndex = -1;
									}
								}
							}
							else
							{
								if( !m_bDrawInformationTooltip )
								{
									ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
									if((scrnpos->x > m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 3 + Map_PartyPlayer_X) )
									{
										if( scrnpos->x < m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 4 + Map_PartyPlayer_X )
										{
											if((scrnpos->z > m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 3 + Map_PartyPlayer_Z) )
											{
												if(scrnpos->z < m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 4 + Map_PartyPlayer_Z)
												{
													// 파티원 위치에 오버됨. 파티원 정보를 표시한다. 
													m_SelectedPartyMemberIndex = i;	
													
													m_TooltipTargetPosition_X	= m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + Map_PartyPlayer_X;
													m_TooltipTargetPosition_Y	= m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + Map_PartyPlayer_Z;
													m_bDrawInformationTooltip	= TRUE;

													m_SelectedQuestID				= -1;
													m_SelectedNPCIndex				= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
													memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
												}
											}
										}
									}
								}//if( !m_bDrawInformationTooltip )
							}
						}
					}
				} // 같은 zone에 있음
			}
		}
	}
	if( !m_bDrawInformationTooltip )
	{
		m_SelectedQuestID				= -1;
		m_SelectedPartyMemberIndex		= -1;
		m_SelectedNPCIndex				= -1;		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
		memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
	}
	
}


#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
void _XWindow_WorldMinimap::DrawPKTracingPosition(_XGUIObject*& pfocusobject)
{	
	if( !m_listPKTracingPosition.empty() )
	{				
		_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
		list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
		for(iter_pktracingposition = m_listPKTracingPosition.begin() ; iter_pktracingposition != m_listPKTracingPosition.end(); ++iter_pktracingposition)
		{
			pktracingposition = *iter_pktracingposition;
			if( pktracingposition )
			{
				int Map_TracingPlayer_X = (int)(((int)pktracingposition->sPosX - (int)g_pLocalUser->m_Position.x) * m_RateOfMapSizeToTerrain);
				int Map_TracingPlayer_Z = -(int)(((int)pktracingposition->sPosZ - (int)g_pLocalUser->m_Position.z) * m_RateOfMapSizeToTerrain);
				
				if((_XDEF_MINIMAP_ORIGIN_X + Map_TracingPlayer_X > _XDEF_MINIMAP_LEFT + 4) && (_XDEF_MINIMAP_ORIGIN_X + Map_TracingPlayer_X < _XDEF_MINIMAP_RIGHT))
				{
					if((_XDEF_MINIMAP_ORIGIN_Y + Map_TracingPlayer_Z < _XDEF_MINIMAP_BOTTOM) && (_XDEF_MINIMAP_ORIGIN_Y + Map_TracingPlayer_Z > _XDEF_MINIMAP_TOP + 4))
					{
						if( pktracingposition->ucPKType == 2 )
						{
							m_imagePKTracingPoint.MoveWindow(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 3 + Map_TracingPlayer_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 3 + Map_TracingPlayer_Z, 
								m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 2 + Map_TracingPlayer_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 2 + Map_TracingPlayer_Z);
							m_imagePKTracingPoint.DrawWithRegion();
						}
						else if( pktracingposition->ucPKType == 3 )
						{
							m_imageSomeOneTracingPoint.MoveWindow(m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 3 + Map_TracingPlayer_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 3 + Map_TracingPlayer_Z, 
								m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 2 + Map_TracingPlayer_X, 
								m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 2 + Map_TracingPlayer_Z);
							m_imageSomeOneTracingPoint.DrawWithRegion();
						}

						
						if( !m_bDrawInformationTooltip )
						{
							ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
							if((scrnpos->x > m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X - 3 + Map_TracingPlayer_X) )
							{
								if( scrnpos->x < m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + 4 + Map_TracingPlayer_X )
								{
									if((scrnpos->z > m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y - 3 + Map_TracingPlayer_Z) )
									{
										if(scrnpos->z < m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + 4 + Map_TracingPlayer_Z)
										{
											memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
											sprintf(m_cPKTracingName, _XGETINTERFACETEXT(ID_STRING_NEW_3669), pktracingposition->szCharacName ); //_T("[추적] %s")
											m_TooltipTargetPosition_X	= m_WindowPosition.x + _XDEF_MINIMAP_ORIGIN_X + Map_TracingPlayer_X;
											m_TooltipTargetPosition_Y	= m_WindowPosition.y + _XDEF_MINIMAP_ORIGIN_Y + Map_TracingPlayer_Z;
											m_bDrawInformationTooltip	= TRUE;
											
											m_SelectedNPCIndex			= -1;		
											m_SelectedQuestID				= -1;
											m_SelectedPartyMemberIndex		= -1;
										}
									}
								}
							}
						}//if( !m_bDrawInformationTooltip )

					}
				}	
			}
		}
	}
}
#endif			

void _XWindow_WorldMinimap::CheckCurrentUser(int partyindex, BOOL &isCurrentUser)
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

void _XWindow_WorldMinimap::SetCheckLocalUserPosition(float x, float z)
{
	m_CheckLocalUserPosition_X = x;
	m_CheckLocalUserPosition_Y = z;
}

void _XWindow_WorldMinimap::SetMinimapRate()
{
	m_RotateAngle = 0.0f;
	//m_MinimapZoomRate = 0.155f;
	m_MinimapZoomRate = 0.25f;

	m_RateOfMapSizeToTerrain = 0.3f*(1024.0f/g_LodTerrain.m_Width);

	m_bDisableZoomButton = FALSE;
}


void _XWindow_WorldMinimap::RebuildCurrentTimeIcon( void )
{
	RECT iconrect[ 4 ] = 
	{
		{ 65, 147,  80, 163 },
		{ 87, 147, 102, 163 },
		{ 104,147, 119, 163 },
		{ 121,147, 136, 163 }
	};

	switch( g_EnvironmentManager.m_Environment ) 
	{
	default:
	case _XE_DAY		:
		m_pSunMoonIcon->SetClipRect( iconrect[0] );
		break;
	case _XE_NIGHTFALL	:
		m_pSunMoonIcon->SetClipRect( iconrect[1] );
		break;
	case _XE_NIGHT		:
		m_pSunMoonIcon->SetClipRect( iconrect[2] );
		break;
	case _XE_DAWN		:
		m_pSunMoonIcon->SetClipRect( iconrect[3] );
		break;	
	}
}

void _XWindow_WorldMinimap::ChangeMinimizeStatus( void )
{
	m_MinimizeStatus = !m_MinimizeStatus;
	
	switch( m_MiniMapMode ) {
	case _XMINIMAPMODE_SMALL:
		{
			if( m_MinimizeStatus )
			{
				SetWindowSize( 154, 43+15 );
				
				m_pSubBar->SetScale( 1.0f, 1.0f );

				m_pTitleBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
				m_pSubBar->MoveWindow( m_WindowPosition.x+3, m_WindowPosition.y + 17 );
				m_pBottomLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 17 +15  );
				m_pBottomRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 60, m_WindowPosition.y + 17+ 15  );
				
				m_pLeftEdgeImage->SetScale( 1.0f, 1.0f); // 길이 151
				m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 121 );

				minimizebutton->MoveWindow( m_WindowPosition.x + 137 , m_WindowPosition.y + 2 );				
				zoommaxbutton->MoveWindow( m_WindowPosition.x + 100, m_WindowPosition.y + 18 + 15 );
				zoominbutton->MoveWindow( m_WindowPosition.x + 138, m_WindowPosition.y + 16 + 15); 
				zoomoutbutton->MoveWindow( m_WindowPosition.x + 144, m_WindowPosition.y + 24 + 15);
				worldmapbutton->MoveWindow( m_WindowPosition.x + 124, m_WindowPosition.y + 22 +15 );				
				m_btnNPCSearch->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 26 );
#ifdef _XDEF_WEBBOARD_20061211
				m_btnWebBoard->MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y + m_WindowSize.cy - 24 );
#endif
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
				m_btnPKTracingInfo->MoveWindow(m_WindowPosition.x + 70, m_WindowPosition.y + m_WindowSize.cy - 25 );
	#endif
#endif
				helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );
				
				m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 127 , m_WindowPosition.y + 15 );
				
				m_pTitleBar->ShowWindow(TRUE);						
				m_pLeftEdgeImage->ShowWindow(TRUE);
				m_pBottomLeftBar->ShowWindow(TRUE);
				m_pBottomRightBar->ShowWindow(TRUE);
				
				m_pMaximumMapTitleLeftBar->ShowWindow(FALSE);
				m_pMaximumMapTitleMiddleBar->ShowWindow(FALSE);
				m_pMaximumMapTitleRightBar->ShowWindow(FALSE);				
				m_pMaximumMapBottomMiddleBar1->ShowWindow(FALSE);
				m_pMaximumMapBottomMiddleBar2->ShowWindow(FALSE);
				m_LargeMinimapImage.ShowWindow(FALSE);	
				
				if( m_pNPCSearchWindowPtr )
				{
					if( m_pNPCSearchWindowPtr->GetShowStatus() )
						m_pNPCSearchWindowPtr->ShowWindow( FALSE );
				}

#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				SetMinimapTransparency( 255 );
				m_pAlphaSlider->ShowWindow( FALSE );  //breif : 인터페이스 수정
#endif
			}
			else
			{
				SetWindowSize( 154, 195+15 );
				
				m_pSubBar->SetScale( 1.0f, 1.0f );
				
				m_pSubBar->MoveWindow( m_WindowPosition.x+3, m_WindowPosition.y + 17 );
				m_pBottomLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy-27 );
				m_pBottomRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 60, m_WindowPosition.y + m_WindowSize.cy-27 );
				m_pLeftEdgeImage->SetScale( 1.0f, 1.2f); // 길이 167
				m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 256 );
				
				minimizebutton->MoveWindow( m_WindowPosition.x + 137 , m_WindowPosition.y + 2 );	
				zoommaxbutton->MoveWindow( m_WindowPosition.x + 100, m_WindowPosition.y + 171+15 );
				zoominbutton->MoveWindow( m_WindowPosition.x + 138, m_WindowPosition.y + 169+15 ); 
				zoomoutbutton->MoveWindow( m_WindowPosition.x + 144, m_WindowPosition.y + 177+15 );
				worldmapbutton->MoveWindow( m_WindowPosition.x + 124, m_WindowPosition.y + 175+15 );		
				m_btnNPCSearch->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 26 );
#ifdef _XDEF_WEBBOARD_20061211
				m_btnWebBoard->MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y + m_WindowSize.cy - 24 );
#endif			
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
				m_btnPKTracingInfo->MoveWindow(m_WindowPosition.x + 70, m_WindowPosition.y + m_WindowSize.cy - 25 );
	#endif
#endif
				helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );
				
				m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 127 , m_WindowPosition.y + 15 );

				
				m_Minimap.MoveWindow( m_WindowPosition.x + _XDEF_MINIMAP_LEFT,  m_WindowPosition.y + _XDEF_MINIMAP_TOP,
					m_WindowPosition.x + _XDEF_MINIMAP_RIGHT, m_WindowPosition.y + _XDEF_MINIMAP_BOTTOM);
				
				m_MinimapArrow.MoveWindow( m_WindowPosition.x + _XDEF_MINIMAPARROW_LEFT,	 m_WindowPosition.y + _XDEF_MINIMAPARROW_TOP,
					m_WindowPosition.x + _XDEF_MINIMAPARROW_RIGHT, m_WindowPosition.y + _XDEF_MINIMAPARROW_BOTTOM);	
				m_MinimapArrow.Rotate( m_RotateAngle  );	
				
				m_FrustumeImage.MoveWindow( m_WindowPosition.x + _XDEF_MINIMAPFRUSTRUM_LEFT,	 m_WindowPosition.y + _XDEF_MINIMAPFRUSTRUM_TOP,
					m_WindowPosition.x + _XDEF_MINIMAPFRUSTRUM_RIGHT, m_WindowPosition.y + _XDEF_MINIMAPFRUSTRUM_BOTTOM);
				
				m_pTitleBar->ShowWindow(TRUE);						
				m_pLeftEdgeImage->ShowWindow(TRUE);
				m_pBottomLeftBar->ShowWindow(TRUE);
				m_pBottomRightBar->ShowWindow(TRUE);
				
				m_pMaximumMapTitleLeftBar->ShowWindow(FALSE);
				m_pMaximumMapTitleMiddleBar->ShowWindow(FALSE);
				m_pMaximumMapTitleRightBar->ShowWindow(FALSE);				
				m_pMaximumMapBottomMiddleBar1->ShowWindow(FALSE);
				m_pMaximumMapBottomMiddleBar2->ShowWindow(FALSE);
				m_LargeMinimapImage.ShowWindow(FALSE);	

#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				SetMinimapTransparency( 255 );
				m_pAlphaSlider->ShowWindow( FALSE );  //breif : 인터페이스 수정
#endif
			}
		}
		break;
	case _XMINIMAPMODE_LARGE:
		{
			if( m_MinimizeStatus )
			{
				if( m_WindowSize.cx == 454 )
				{
					SetWindowSize( 154, 43+15 );
					MoveWindow( m_WindowPosition.x + 300, m_WindowPosition.y );
				}
				else
				{
					SetWindowSize( 154, 43+15 );
				}
				
				m_pSubBar->SetScale( 1.0f, 1.0f );
				m_pTitleBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
				m_pSubBar->MoveWindow( m_WindowPosition.x+3, m_WindowPosition.y + 17 );
				m_pBottomLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 17 + 15  );
				m_pBottomRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 60, m_WindowPosition.y + 17 +15  );
				
				m_pLeftEdgeImage->SetScale( 1.0f, 1.0f); // 길이 151
				m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 121 );
				
				minimizebutton->MoveWindow( m_WindowPosition.x + 137 , m_WindowPosition.y + 2 );	
				zoommaxbutton->MoveWindow( m_WindowPosition.x + 100, m_WindowPosition.y + 18 +15);
				zoominbutton->MoveWindow( m_WindowPosition.x + 138, m_WindowPosition.y + 16 +15); 
				zoomoutbutton->MoveWindow( m_WindowPosition.x + 144, m_WindowPosition.y + 24 +15);
				worldmapbutton->MoveWindow( m_WindowPosition.x + 124, m_WindowPosition.y + 22 +15 );	
				m_btnNPCSearch->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 26 );
#ifdef _XDEF_WEBBOARD_20061211
				m_btnWebBoard->MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y + m_WindowSize.cy - 24 );
#endif
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
				m_btnPKTracingInfo->MoveWindow(m_WindowPosition.x + 70, m_WindowPosition.y + m_WindowSize.cy - 25 );
	#endif
#endif
				helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );
				
				m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 127 , m_WindowPosition.y + 15 );
				
				m_pTitleBar->ShowWindow(TRUE);						
				m_pLeftEdgeImage->ShowWindow(TRUE);
				m_pBottomLeftBar->ShowWindow(TRUE);
				m_pBottomRightBar->ShowWindow(TRUE);
				
				m_pMaximumMapTitleLeftBar->ShowWindow(FALSE);
				m_pMaximumMapTitleMiddleBar->ShowWindow(FALSE);
				m_pMaximumMapTitleRightBar->ShowWindow(FALSE);				
				m_pMaximumMapBottomMiddleBar1->ShowWindow(FALSE);
				m_pMaximumMapBottomMiddleBar2->ShowWindow(FALSE);
				
				m_LargeMinimapImage.ShowWindow(FALSE);	
				
				if( m_pNPCSearchWindowPtr )
				{
					if( m_pNPCSearchWindowPtr->GetShowStatus() )
						m_pNPCSearchWindowPtr->ShowWindow( FALSE );
				}

#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				SetMinimapTransparency( 255 );
				m_pAlphaSlider->ShowWindow( FALSE );  //breif : 인터페이스 수정
#endif

			}
			else
			{
				if( m_WindowSize.cx == 154 )
				{
					SetWindowSize( 454, 495+15 );
					MoveWindow( m_WindowPosition.x - 300, m_WindowPosition.y );
				}
				else
				{
					SetWindowSize( 454, 495+15 );
				}	
				
				m_pSubBar->SetScale( 3.0f, 1.0f );
				// 타이틀 바
				m_pMaximumMapTitleLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
				m_pMaximumMapTitleMiddleBar->MoveWindow( m_WindowPosition.x + 77, m_WindowPosition.y );
				m_pMaximumMapTitleRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx -3, m_WindowPosition.y );
				m_pSubBar->MoveWindow( m_WindowPosition.x+3, m_WindowPosition.y + 17 );
				
				// 아래쪽 바닥 바탕
				m_pBottomLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy - 27  );
				m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 96, m_WindowPosition.y + m_WindowSize.cy - 27  );
				m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 189 , m_WindowPosition.y + m_WindowSize.cy - 27  );
				m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 282 , m_WindowPosition.y + m_WindowSize.cy - 27  );				
				m_pMaximumMapBottomMiddleBar2->MoveWindow( m_WindowPosition.x + 375 , m_WindowPosition.y + m_WindowSize.cy - 27  );
				m_pBottomRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 60, m_WindowPosition.y + m_WindowSize.cy - 27  );
				
				// 왼쪽 사이드 이미지 
				m_pLeftEdgeImage->SetScale( 1.0f, 3.1f); // 길이 468
				m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 256 );
				m_pLeftEdgeImage->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 16 );

				// 버튼 위치
				minimizebutton->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 17 , m_WindowPosition.y + 2 );	
				zoommaxbutton->MoveWindow( m_WindowPosition.x + 400, m_WindowPosition.y + 471+15 );
				zoominbutton->MoveWindow( m_WindowPosition.x + 438, m_WindowPosition.y + 469+15 ); 
				zoomoutbutton->MoveWindow( m_WindowPosition.x + 444, m_WindowPosition.y + 477+15 );
				worldmapbutton->MoveWindow( m_WindowPosition.x + 424, m_WindowPosition.y + 475+15 );	
				m_btnNPCSearch->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 26 );
#ifdef _XDEF_WEBBOARD_20061211
				m_btnWebBoard->MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y + m_WindowSize.cy - 24 );
#endif
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
				m_btnPKTracingInfo->MoveWindow(m_WindowPosition.x + 70, m_WindowPosition.y + m_WindowSize.cy - 25 );
	#endif
#endif
				helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );
				
				m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 297 , m_WindowPosition.y + 15);
				
				m_pTitleBar->ShowWindow(FALSE);		
	
				m_pMaximumMapTitleLeftBar->ShowWindow(TRUE);
				m_pMaximumMapTitleMiddleBar->ShowWindow(TRUE);
				m_pMaximumMapTitleRightBar->ShowWindow(TRUE);
				
				m_pLeftEdgeImage->ShowWindow(TRUE);

				m_pBottomLeftBar->ShowWindow(TRUE);
				m_pMaximumMapBottomMiddleBar1->ShowWindow(TRUE);
				m_pMaximumMapBottomMiddleBar2->ShowWindow(TRUE);
				m_pBottomRightBar->ShowWindow(TRUE);		
				m_LargeMinimapImage.ShowWindow(TRUE);	
				
#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				m_pAlphaSlider->ShowWindow( TRUE );  //breif : 인터페이스 수정
				m_pAlphaSlider->MoveWindow( m_WindowPosition.x + 215, m_WindowPosition.y + m_WindowSize.cy - 24 );
				m_pAlphaSlider->m_IndicatorImage.MoveWindow( m_WindowPosition.x + 215, m_WindowPosition.y + m_WindowSize.cy - 22 );
				SetMinimapTransparency( m_nTransparency );
#endif
			}
		}
		break;
	}
}
void _XWindow_WorldMinimap::SetMode( _XMINIMAPMODE minimapmode )
{
	m_MiniMapMode = minimapmode;
	
	switch( m_MiniMapMode ) {
	case _XMINIMAPMODE_SMALL:
		{
			if( m_MinimizeStatus )
			{
				SetWindowSize( 154, 43+15 );
				
				m_pSubBar->SetScale( 1.0f, 1.0f );
				
				m_pTitleBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
				m_pSubBar->MoveWindow( m_WindowPosition.x+3, m_WindowPosition.y + 17 );
				m_pBottomLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 17 +15  );
				m_pBottomRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 60, m_WindowPosition.y + 17+ 15  );
				
				m_pLeftEdgeImage->SetScale( 1.0f, 1.0f); // 길이 151
				m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 120 );
				
				minimizebutton->MoveWindow( m_WindowPosition.x + 137 , m_WindowPosition.y + 2 );				
				zoommaxbutton->MoveWindow( m_WindowPosition.x + 100, m_WindowPosition.y + 18 + 15 );
				zoominbutton->MoveWindow( m_WindowPosition.x + 138, m_WindowPosition.y + 16 + 15); 
				zoomoutbutton->MoveWindow( m_WindowPosition.x + 144, m_WindowPosition.y + 24 + 15);
				worldmapbutton->MoveWindow( m_WindowPosition.x + 124, m_WindowPosition.y + 22 +15 );	
				m_btnNPCSearch->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 26 );
#ifdef _XDEF_WEBBOARD_20061211
				m_btnWebBoard->MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y + m_WindowSize.cy - 24 );
#endif
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
				m_btnPKTracingInfo->MoveWindow(m_WindowPosition.x + 70, m_WindowPosition.y + m_WindowSize.cy - 25 );
	#endif
#endif
				helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );				
				m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 127 , m_WindowPosition.y + 15);
				
				m_pTitleBar->ShowWindow(TRUE);						
				m_pLeftEdgeImage->ShowWindow(TRUE);
				m_pBottomLeftBar->ShowWindow(TRUE);
				m_pBottomRightBar->ShowWindow(TRUE);
				
				m_pMaximumMapTitleLeftBar->ShowWindow(FALSE);
				m_pMaximumMapTitleMiddleBar->ShowWindow(FALSE);
				m_pMaximumMapTitleRightBar->ShowWindow(FALSE);				
				m_pMaximumMapBottomMiddleBar1->ShowWindow(FALSE);
				m_pMaximumMapBottomMiddleBar2->ShowWindow(FALSE);
				m_LargeMinimapImage.ShowWindow(FALSE);	
				
				if( m_pNPCSearchWindowPtr )
				{
					if( m_pNPCSearchWindowPtr->GetShowStatus() )
						m_pNPCSearchWindowPtr->ShowWindow( FALSE );
				}

#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				m_pAlphaSlider->ShowWindow( FALSE );  //breif : 인터페이스 수정
				SetMinimapTransparency( 255 );
#endif
				
			}
			else
			{
				if( m_WindowSize.cx == 454 )
				{
					SetWindowSize( 154, 195+15 );
					MoveWindow( m_WindowPosition.x + 300, m_WindowPosition.y );
				}
				else
				{
					SetWindowSize( 154, 195+15 );
				}
				
				m_pSubBar->SetScale( 1.0f, 1.0f );
				
				m_pSubBar->MoveWindow( m_WindowPosition.x+3, m_WindowPosition.y + 17 );
				m_pBottomLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy-27 );
				m_pBottomRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 60, m_WindowPosition.y + m_WindowSize.cy-27 );
				m_pLeftEdgeImage->SetScale( 1.0f, 1.2f); // 길이 167
				m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 256 );
				
				minimizebutton->MoveWindow( m_WindowPosition.x + 137 , m_WindowPosition.y + 2 );	
				zoommaxbutton->MoveWindow( m_WindowPosition.x + 100, m_WindowPosition.y + 171+15 );
				zoominbutton->MoveWindow( m_WindowPosition.x + 138, m_WindowPosition.y + 169+15 ); 
				zoomoutbutton->MoveWindow( m_WindowPosition.x + 144, m_WindowPosition.y + 177+15 );
				worldmapbutton->MoveWindow( m_WindowPosition.x + 124, m_WindowPosition.y + 175+15 );	
				m_btnNPCSearch->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 26 );	
#ifdef _XDEF_WEBBOARD_20061211
				m_btnWebBoard->MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y + m_WindowSize.cy - 24 );
#endif		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
				m_btnPKTracingInfo->MoveWindow(m_WindowPosition.x + 70, m_WindowPosition.y + m_WindowSize.cy - 25 );
	#endif
#endif
				helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );
				
				m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 127 , m_WindowPosition.y + 15 );
			
				
				m_Minimap.MoveWindow( m_WindowPosition.x + _XDEF_MINIMAP_LEFT,  m_WindowPosition.y + _XDEF_MINIMAP_TOP,
					m_WindowPosition.x + _XDEF_MINIMAP_RIGHT, m_WindowPosition.y + _XDEF_MINIMAP_BOTTOM);
				
				m_MinimapArrow.MoveWindow( m_WindowPosition.x + _XDEF_MINIMAPARROW_LEFT,	 m_WindowPosition.y + _XDEF_MINIMAPARROW_TOP,
					m_WindowPosition.x + _XDEF_MINIMAPARROW_RIGHT, m_WindowPosition.y + _XDEF_MINIMAPARROW_BOTTOM);	
				m_MinimapArrow.Rotate( m_RotateAngle  );	
				
				m_FrustumeImage.MoveWindow( m_WindowPosition.x + _XDEF_MINIMAPFRUSTRUM_LEFT,	 m_WindowPosition.y + _XDEF_MINIMAPFRUSTRUM_TOP,
					m_WindowPosition.x + _XDEF_MINIMAPFRUSTRUM_RIGHT, m_WindowPosition.y + _XDEF_MINIMAPFRUSTRUM_BOTTOM);
				
				m_pTitleBar->ShowWindow(TRUE);						
				m_pLeftEdgeImage->ShowWindow(TRUE);
				m_pBottomLeftBar->ShowWindow(TRUE);
				m_pBottomRightBar->ShowWindow(TRUE);
				
				m_pMaximumMapTitleLeftBar->ShowWindow(FALSE);
				m_pMaximumMapTitleMiddleBar->ShowWindow(FALSE);
				m_pMaximumMapTitleRightBar->ShowWindow(FALSE);				
				m_pMaximumMapBottomMiddleBar1->ShowWindow(FALSE);
				m_pMaximumMapBottomMiddleBar2->ShowWindow(FALSE);
				
				m_LargeMinimapImage.ShowWindow(FALSE);

#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				m_pAlphaSlider->ShowWindow( FALSE );  //breif : 인터페이스 수정
				SetMinimapTransparency( 255 );
#endif
				
				if( m_pNPCSearchWindowPtr )
				{
					if( m_pNPCSearchWindowPtr->GetShowStatus() )
					{
						if( /*g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE ||*/ g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN  )
						{
							m_pNPCSearchWindowPtr->MoveWindow( m_WindowPosition.x - 154, m_WindowPosition.y );
						}
						else
						{
							m_pNPCSearchWindowPtr->MoveWindow( m_WindowPosition.x - 112, m_WindowPosition.y );
						}

					}
				}
			}
		}
		break;
	case _XMINIMAPMODE_LARGE:
		{
			if( m_MinimizeStatus )
			{
				if( m_WindowSize.cx == 454 )
				{
					SetWindowSize( 154, 43+15 );
					MoveWindow( m_WindowPosition.x + 300, m_WindowPosition.y );
				}
				else
				{
					SetWindowSize( 154, 43+15 );
				}
				
				m_pSubBar->SetScale( 1.0f, 1.0f );
				m_pTitleBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
				m_pSubBar->MoveWindow( m_WindowPosition.x+3, m_WindowPosition.y + 17 );
				m_pBottomLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 17 + 15  );
				m_pBottomRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 60, m_WindowPosition.y + 17 +15  );
				
				m_pLeftEdgeImage->SetScale( 1.0f, 1.0f); // 길이 151
				m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 121 );
				
				minimizebutton->MoveWindow( m_WindowPosition.x + 137 , m_WindowPosition.y + 2 );	
				zoommaxbutton->MoveWindow( m_WindowPosition.x + 100, m_WindowPosition.y + 18 +15);
				zoominbutton->MoveWindow( m_WindowPosition.x + 138, m_WindowPosition.y + 16 +15); 
				zoomoutbutton->MoveWindow( m_WindowPosition.x + 144, m_WindowPosition.y + 24 +15);
				worldmapbutton->MoveWindow( m_WindowPosition.x + 124, m_WindowPosition.y + 22 +15 );
				m_btnNPCSearch->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 26 );	
#ifdef _XDEF_WEBBOARD_20061211
				m_btnWebBoard->MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y + m_WindowSize.cy - 24 );
#endif		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
				m_btnPKTracingInfo->MoveWindow(m_WindowPosition.x + 70, m_WindowPosition.y + m_WindowSize.cy - 25 );
	#endif
#endif
				helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );
				
				m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 127 , m_WindowPosition.y +15);
								
				m_pTitleBar->ShowWindow(TRUE);						
				m_pLeftEdgeImage->ShowWindow(TRUE);
				m_pBottomLeftBar->ShowWindow(TRUE);
				m_pBottomRightBar->ShowWindow(TRUE);
				
				m_pMaximumMapTitleLeftBar->ShowWindow(FALSE);
				m_pMaximumMapTitleMiddleBar->ShowWindow(FALSE);
				m_pMaximumMapTitleRightBar->ShowWindow(FALSE);				
				m_pMaximumMapBottomMiddleBar1->ShowWindow(FALSE);
				m_pMaximumMapBottomMiddleBar2->ShowWindow(FALSE);
				
				m_LargeMinimapImage.ShowWindow(FALSE);	

				if( m_pNPCSearchWindowPtr )
				{
					if( m_pNPCSearchWindowPtr->GetShowStatus() )
						m_pNPCSearchWindowPtr->ShowWindow( FALSE );
				}

#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				m_pAlphaSlider->ShowWindow( FALSE );  //breif : 인터페이스 수정
				SetMinimapTransparency( 255 );
#endif
				
			}
			else
			{
				if( m_WindowSize.cx == 154 )
				{
					SetWindowSize( 454, 495+15 );
					MoveWindow( m_WindowPosition.x - 300, m_WindowPosition.y );
				}
				else
				{
					SetWindowSize( 454, 495+15 );
				}	
				
				m_pSubBar->SetScale( 3.0f, 1.0f );
				// 타이틀 바
				m_pMaximumMapTitleLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y );
				m_pMaximumMapTitleMiddleBar->MoveWindow( m_WindowPosition.x + 77, m_WindowPosition.y );
				m_pMaximumMapTitleRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx -3, m_WindowPosition.y );
				m_pSubBar->MoveWindow( m_WindowPosition.x+3, m_WindowPosition.y + 17 );
				
				// 아래쪽 바닥 바탕
				m_pBottomLeftBar->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy - 27  );
				m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 96, m_WindowPosition.y + m_WindowSize.cy - 27  );
				m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 189 , m_WindowPosition.y + m_WindowSize.cy - 27  );
				m_pMaximumMapBottomMiddleBar1->MoveWindow( m_WindowPosition.x + 282 , m_WindowPosition.y + m_WindowSize.cy - 27  );				
				m_pMaximumMapBottomMiddleBar2->MoveWindow( m_WindowPosition.x + 375 , m_WindowPosition.y + m_WindowSize.cy - 27  );
				m_pBottomRightBar->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 60, m_WindowPosition.y + m_WindowSize.cy - 27  );
				
				// 왼쪽 사이드 이미지 
				m_pLeftEdgeImage->SetClipRect( 254, 105, 256, 256 );
				m_pLeftEdgeImage->SetScale( 1.0f, 3.1f); // 길이 453
				m_pLeftEdgeImage->MoveWindow( m_WindowPosition.x, m_WindowPosition.y + 16 );
				
				// 버튼 위치
				minimizebutton->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 17 , m_WindowPosition.y + 2 );	
				zoommaxbutton->MoveWindow( m_WindowPosition.x + 400, m_WindowPosition.y + 471+15 );
				zoominbutton->MoveWindow( m_WindowPosition.x + 438, m_WindowPosition.y + 469+15 ); 
				zoomoutbutton->MoveWindow( m_WindowPosition.x + 444, m_WindowPosition.y + 477+15 );
				worldmapbutton->MoveWindow( m_WindowPosition.x + 424, m_WindowPosition.y + 475+15 );
				m_btnNPCSearch->MoveWindow( m_WindowPosition.x + 3, m_WindowPosition.y + m_WindowSize.cy - 26 );	
#ifdef _XDEF_WEBBOARD_20061211
				m_btnWebBoard->MoveWindow( m_WindowPosition.x + 40, m_WindowPosition.y + m_WindowSize.cy - 24 );
#endif		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	#ifdef _XDEF_PK_TRACINGSERVICES_20070402 //Author : 양희왕
				m_btnPKTracingInfo->MoveWindow(m_WindowPosition.x + 70, m_WindowPosition.y + m_WindowSize.cy - 25);
	#endif
#endif
				helpbookbutton->MoveWindow( m_WindowPosition.x + 2, m_WindowPosition.y + m_WindowSize.cy-2 );
				
				m_pSunMoonIcon->MoveWindow( m_WindowPosition.x + 297 , m_WindowPosition.y+15);
				
				m_pTitleBar->ShowWindow(FALSE);		
				
				m_pMaximumMapTitleLeftBar->ShowWindow(TRUE);
				m_pMaximumMapTitleMiddleBar->ShowWindow(TRUE);
				m_pMaximumMapTitleRightBar->ShowWindow(TRUE);
				
				m_pLeftEdgeImage->ShowWindow(TRUE);
				
				m_pBottomLeftBar->ShowWindow(TRUE);
				m_pMaximumMapBottomMiddleBar1->ShowWindow(TRUE);
				m_pMaximumMapBottomMiddleBar2->ShowWindow(TRUE);
				m_pBottomRightBar->ShowWindow(TRUE);
				
				m_LargeMinimapImage.SetScale( 0.87890625f, 0.87890625f);
				m_LargeMinimapImage.ShowWindow(TRUE);		
				
#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕
				m_pAlphaSlider->ShowWindow( TRUE );  //breif : 인터페이스 수정
				m_pAlphaSlider->MoveWindow( m_WindowPosition.x + 215, m_WindowPosition.y + m_WindowSize.cy - 24 );
				m_pAlphaSlider->m_IndicatorImage.MoveWindow( m_WindowPosition.x + 215, m_WindowPosition.y + m_WindowSize.cy - 22 );
				SetMinimapTransparency( m_nTransparency );
#endif

				if( m_pNPCSearchWindowPtr )
				{
					if( m_pNPCSearchWindowPtr->GetShowStatus() )
					{
						if( /*g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE ||*/ g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN  )
						{
							m_pNPCSearchWindowPtr->MoveWindow( m_WindowPosition.x - 154, m_WindowPosition.y );
						}
						else
						{
							m_pNPCSearchWindowPtr->MoveWindow( m_WindowPosition.x - 112, m_WindowPosition.y );
						}
					}
				}
			}
		}
		break;
	}
	SetLocalUserPosition();
}

void _XWindow_WorldMinimap::DrawLargeMinipMapNPCPosition(_XGUIObject*& pfocusobject)
{		
	m_bDrawInformationTooltip = FALSE;
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
	{		
		if( g_Npclist.m_pNPCObjectArray[i].m_bShowNPCModel )
		{
			float x = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._41 * m_RateOfLargeMapSizeToTerrain + 225.0f;
			float z = -g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._43 * m_RateOfLargeMapSizeToTerrain - 225.0f;
						
			if(g_Npclist.m_pNPCObjectArray[i].m_NPCRollType == _XNPCROLL_TRADE || g_Npclist.m_pNPCObjectArray[i].m_NPCRollType == _XNPCROLL_WAREHOUSE)
			{
				m_NPCImageTradeType.MoveWindow( m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT - 4 + x, 
					m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM - 4 + z, 
					m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + 3 + x, 
					m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + 3 + z);
				m_NPCImageTradeType.DrawWithRegion();
				
				if( !m_bDrawInformationTooltip )
				{
					ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
					if((scrnpos->x > m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT - 4 + x) )
					{
						if( scrnpos->x < m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + 3 + x )
						{
							if((scrnpos->z > m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM - 4 + z) )
							{
								if(scrnpos->z < m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + 3 + z)
								{
									// NPC 위치에 오버됨. NPC 정보를 표시한다. 
									m_SelectedNPCIndex			= i;				
									m_TooltipTargetPosition_X	= m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + x;
									m_TooltipTargetPosition_Y	= m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + z;
									m_bDrawInformationTooltip	= TRUE;
									
									m_SelectedQuestID				= -1;
									m_SelectedPartyMemberIndex		= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
									memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
								}
							}
						}
					}
				}
				
			}
			else if( g_Npclist.m_pNPCObjectArray[i].m_NPCRollType == _XNPCROLL_EVENT )
			{
				m_NPCImageEventType.MoveWindow(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 4 + x, 
					m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 4 + z, 
					m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + 3 + x, 
					m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 3 + z);
				m_NPCImageEventType.DrawWithRegion();

				if( !m_bDrawInformationTooltip )
				{
					ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
					if((scrnpos->x > m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT - 4 + x) )
					{
						if( scrnpos->x < m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + 3 + x )
						{
							if((scrnpos->z > m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM - 4 + z) )
							{
								if(scrnpos->z < m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + 3 + z)
								{
									// NPC 위치에 오버됨. NPC 정보를 표시한다. 
									m_SelectedNPCIndex			= i;				
									m_TooltipTargetPosition_X	= m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + x;
									m_TooltipTargetPosition_Y	= m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + z;
									m_bDrawInformationTooltip	= TRUE;
									
									m_SelectedQuestID				= -1;
									m_SelectedPartyMemberIndex		= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
									memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
								}
							}
						}
					}
				}
			}
		}		
	}
		
	if( !g_QuestScriptManager.m_listQuestMark.empty() )
	{		
		if(m_PrevDrawTime == 0)
			m_PrevDrawTime = g_LocalSystemTime;
		
		DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
		
		if(!m_EffectOrderSub)
			m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
		else 
			m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);
		
		if(m_EffectFrame > 4.0f)
		{
			m_EffectOrderSub = true;
			m_EffectFrame = 3.0f;
		}
		else if(m_EffectFrame <= 0.0f)
		{
			m_EffectOrderSub = false;
			m_EffectFrame = 0.0f;
		}
		
		int selectedframe = (int)m_EffectFrame;
		
		if(selectedframe > 4)
			selectedframe = 3;
		else if(selectedframe < 0)
			selectedframe = 0;

		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
		{
			_XQUEST_MARKINFO questmark = *iter_questmark;
			
			int questmarkpositionX = questmark.MarkPosition_X * m_RateOfLargeMapSizeToTerrain + 225.0f;
			int questmarkpositionY = -questmark.MarkPosition_Y * m_RateOfLargeMapSizeToTerrain - 225.0f;

			if(( questmarkpositionX > 0) && ( questmarkpositionX < _XDEF_LARGEMINIMAP_RIGHT))
			{
				if(( questmarkpositionY < 0) && ( questmarkpositionY > -(_XDEF_LARGEMINIMAP_BOTTOM)))
				{		
					// 현재 진행되는 퀘스트의 종류에 따라 색을 바꿔서 표시한다.
					switch( questmark.questtype ) {
					case _XQUEST_TYPE_1://제룡행 
						{
							m_YellowImpressionMark[selectedframe]->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + questmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + questmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_2://무림행 
						{
							m_BlueImpressionMark[selectedframe]->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + questmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + questmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_3://협객행
						{
							m_OrangeImpressionMark[selectedframe]->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + questmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + questmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_4://기연행 
						{
							m_GreenImpressionMark[selectedframe]->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + questmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + questmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_5://무림소사
						{
							m_WhiteImpressionMark[selectedframe]->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + questmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + questmarkpositionY);
						}
						break;
					}
					
					if( !m_bDrawInformationTooltip )
					{
						ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
						if((scrnpos->x > m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 9 + questmarkpositionX) )
						{
							if( scrnpos->x < m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + 10 + questmarkpositionX )
							{
								if((scrnpos->z > m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 9 + questmarkpositionY) )
								{
									if(scrnpos->z < m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 10 + questmarkpositionY)
									{
										// Quest 위치에 오버됨. Quest 정보를 표시한다. 
										m_SelectedQuestType			= questmark.questtype;
										m_SelectedQuestID			= questmark.questid;
										
										m_TooltipTargetPosition_X	= m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + questmarkpositionX;
										m_TooltipTargetPosition_Y	= m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + questmarkpositionY;
										m_bDrawInformationTooltip	= TRUE;
										
										m_SelectedPartyMemberIndex		= -1;
										m_SelectedNPCIndex				= -1;	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
										memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
									}
								}
							}
						}
					}//if( !m_bDrawInformationTooltip )				
				}
			}
		}
		
		m_PrevDrawTime = g_LocalSystemTime;
	}
	if( !g_QuestScriptManager.m_listHintMark.empty() )
	{		
		list <_XQUEST_MARKINFO>::iterator iter_hintmark;
		for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hintmark)
		{
			_XQUEST_MARKINFO hintmark = *iter_hintmark;
			
			int hintmarkpositionX = hintmark.MarkPosition_X * m_RateOfLargeMapSizeToTerrain + 225.0f;
			int hinttmarkpositionY = -hintmark.MarkPosition_Y * m_RateOfLargeMapSizeToTerrain - 225.0f;

			if(( hintmarkpositionX > 0) && ( hintmarkpositionX < _XDEF_LARGEMINIMAP_RIGHT))
			{
				if(( hinttmarkpositionY < 0) && ( hinttmarkpositionY > -(_XDEF_LARGEMINIMAP_BOTTOM)))
				{	
					
					// 현재 진행되는 퀘스트의 종류에 따라 색을 바꿔서 표시한다.
					switch( hintmark.questtype ) {
					case _XQUEST_TYPE_1://제룡행 
						{
							m_YellowQuestionMark->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + hintmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + hinttmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_2://무림행 
						{
							m_BlueQuestionMark->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + hintmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + hinttmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_3://협객행
						{
							m_OrangeQuestionMark->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + hintmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + hinttmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_4://기연행 
						{
							m_GreenQuestionMark->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + hintmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + hinttmarkpositionY);
						}
						break;
					case _XQUEST_TYPE_5://무림소사
						{
							m_WhiteQuestionMark->Draw(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 7 + hintmarkpositionX, 
								m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 7 + hinttmarkpositionY);
						}
						break;
					}
					
					if( !m_bDrawInformationTooltip )
					{
						ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
						if((scrnpos->x > m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 9 + hintmarkpositionX) )
						{
							if( scrnpos->x < m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + 10 + hintmarkpositionX )
							{
								if((scrnpos->z > m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 9 + hinttmarkpositionY) )
								{
									if(scrnpos->z < m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 10 + hinttmarkpositionY)
									{
										// Quest 위치에 오버됨. Quest 정보를 표시한다. 
										m_SelectedQuestType			= hintmark.questtype;
										m_SelectedQuestID			= hintmark.questid;
										
										m_TooltipTargetPosition_X	= m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + hintmarkpositionX;
										m_TooltipTargetPosition_Y	= m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + hinttmarkpositionY;
										m_bDrawInformationTooltip	= TRUE;
										
										m_SelectedPartyMemberIndex		= -1;
										m_SelectedNPCIndex				= -1;	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
										memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
									}
								}
							}
						}
					}//if( !m_bDrawInformationTooltip )
				}
			}
		}
	}
	
	//NPC 찾기 기능에서 선택된 NPC 직책에 관련된 NPC 전부를 처리
	if( m_bDrawNPCSearchEffect )
	{
		for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
		{		
			if( g_Npclist.m_pNPCObjectArray[i].m_bShowNPCModel )
			{
				if( g_Npclist.m_pNPCObjectArray[i].m_NPCJobClass == m_SelectedNPCJobIndex )
				{
					float x = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._41 * m_RateOfLargeMapSizeToTerrain + 225.0f;
					float z = -g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._43 * m_RateOfLargeMapSizeToTerrain - 225.0f;

					if(m_PrevDrawTimeNPCSearch == 0)
						m_PrevDrawTimeNPCSearch = g_LocalSystemTime;
					
					DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTimeNPCSearch;
					
					m_EffectFrameNPCSearch += ((FLOAT)elapsedtime / 500.0f);
					
					if(m_EffectFrameNPCSearch >= 4.0f)
					{
						m_EffectFrameNPCSearch = 3.0f;
					}
					
					int selectedframe = (int)m_EffectFrameNPCSearch;
					
					m_imageNPCSearchMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + x - 17, 
						m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + z - 17);
					
					if( selectedframe == 3 )
						m_EffectFrameNPCSearch = 0.0f;
					
					m_PrevDrawTimeNPCSearch = g_LocalSystemTime;
				}
			}
		}
	}

	if( !m_bDrawInformationTooltip )
	{
		m_SelectedQuestID				= -1;
		m_SelectedPartyMemberIndex		= -1;
		m_SelectedNPCIndex				= -1;		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
		memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
	}
}

void _XWindow_WorldMinimap::DrawLargeMinipMapPartyPlayerPosition(_XGUIObject*& pfocusobject)
{
	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)  
	{
		if( g_pLocalUser->m_PartyInfo[i].playername[0] != 0 && g_pLocalUser->m_PartyInfo[i].isValid)
		{
			BOOL isCurrentUser = FALSE;
			CheckCurrentUser(i,isCurrentUser);
			
			if(!isCurrentUser)
			{
				if(g_pLocalUser->m_PartyInfo[i].serverno == g_CurrentZoneInfoIndex)
				{
					float x = g_pLocalUser->m_PartyInfo[i].posX * m_RateOfLargeMapSizeToTerrain + 225.0f;
					float z = -g_pLocalUser->m_PartyInfo[i].posZ * m_RateOfLargeMapSizeToTerrain - 225.0f;

					m_PartyPlayerImage.MoveWindow(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 3 + x, 
						m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 3 + z, 
						m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + 4 + x, 
						m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 4 + z);
					m_PartyPlayerImage.DrawWithRegion();

					if(m_bDrawPartyMemberSearchEffect)
					{
						if(m_nSearchPartyMemberIndex == i)
						{
							if(m_dwPrevDrawTimePartySearch == 0)
								m_dwPrevDrawTimePartySearch = g_LocalSystemTime;

							DWORD elapsedtime = g_LocalSystemTime - m_dwPrevDrawTimePartySearch;

							m_fEffectFramePartySearch += ((FLOAT)elapsedtime / 200.f);

							if(m_fEffectFramePartySearch >= 4.0f)
							{
								m_fEffectFramePartySearch = 3.0f;
							}

							int selectedframe = (int)m_fEffectFramePartySearch;

							m_imageNPCSearchMark[selectedframe]->Draw(m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + x - 17+2, 
								m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + z - 17+2);

							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + x - 17,
										m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + z - 29,
										g_pLocalUser->m_PartyInfo[i].playername, _XSC_INFORMATION, FALSE );		
							g_InterfaceTooltip.Draw();

							if(selectedframe == 3)
							{
								m_fEffectFramePartySearch = 0.0f;
								m_nTotalDrawTimePartySearch++;
							}

							m_dwPrevDrawTimePartySearch = g_LocalSystemTime;

							if(m_nTotalDrawTimePartySearch > 4)
							{
								// 5번 깜빡임 이상이면 draw 중지
								m_bDrawPartyMemberSearchEffect = FALSE;
								m_nTotalDrawTimePartySearch = 0;
								m_fEffectFramePartySearch = 0;
								m_dwPrevDrawTimePartySearch = 0;
								m_nSearchPartyMemberIndex = -1;
							}
						}
					}
					else
					{
						if( !m_bDrawInformationTooltip )
						{
							ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
							if((scrnpos->x > m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 3 + x) )
							{
								if( scrnpos->x < m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + 4 + x )
								{
									if((scrnpos->z > m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 3 + z) )
									{
										if(scrnpos->z < m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 4 + z)
										{
											// 파티원 위치에 오버됨. 파티원 정보를 표시한다. 
											m_SelectedPartyMemberIndex	= i;	
											
											m_TooltipTargetPosition_X	= m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + x;
											m_TooltipTargetPosition_Y	= m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + z;
											m_bDrawInformationTooltip	= TRUE;
											
											m_SelectedQuestID			= -1;
											m_SelectedNPCIndex			= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
											memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
										}
									}
								}
							}
						}//if( !m_bDrawInformationTooltip )
					}
				}
			}
		}
	}
	if( !m_bDrawInformationTooltip )
	{
		m_SelectedQuestID				= -1;
		m_SelectedPartyMemberIndex		= -1;
		m_SelectedNPCIndex				= -1;		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
		memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
	}
}

#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
void _XWindow_WorldMinimap::DrawLargeMinipMapPKTracingPosition(_XGUIObject*& pfocusobject)
{
	if( !m_listPKTracingPosition.empty() )
	{				
		_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
		list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
		for(iter_pktracingposition = m_listPKTracingPosition.begin() ; iter_pktracingposition != m_listPKTracingPosition.end(); ++iter_pktracingposition)
		{
			pktracingposition = *iter_pktracingposition;
			if( pktracingposition )
			{
				
				float x = pktracingposition->sPosX * m_RateOfLargeMapSizeToTerrain + 225.0f;
				float z = -pktracingposition->sPosZ * m_RateOfLargeMapSizeToTerrain - 225.0f;

				if( pktracingposition->ucPKType == 2 )
				{
					m_imagePKTracingPoint.MoveWindow(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 3 + x, 
						m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 3 + z, 
						m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + 4 + x, 
						m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 4 + z);
					m_imagePKTracingPoint.DrawWithRegion();
				}
				else if( pktracingposition->ucPKType == 3 )
				{
					m_imageSomeOneTracingPoint.MoveWindow(m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT - 3 + x, 
						m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM - 3 + z, 
						m_WindowPosition.x +_XDEF_LARGEMINIMAP_LEFT + 4 + x, 
						m_WindowPosition.y +_XDEF_LARGEMINIMAP_BOTTOM + 4 + z);
					m_imageSomeOneTracingPoint.DrawWithRegion();
				}
				
				if( !m_bDrawInformationTooltip )
				{
					ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
					if((scrnpos->x > m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT - 4 + x) )
					{
						if( scrnpos->x < m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + 3 + x )
						{
							if((scrnpos->z > m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM - 4 + z ) )
							{
								if(scrnpos->z < m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + 3 + z )
								{
									
									memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
									sprintf(m_cPKTracingName, _XGETINTERFACETEXT(ID_STRING_NEW_3669), pktracingposition->szCharacName );	//_T("[추적] %s")
									m_TooltipTargetPosition_X	= m_WindowPosition.x + _XDEF_LARGEMINIMAP_LEFT + x;
									m_TooltipTargetPosition_Y	= m_WindowPosition.y + _XDEF_LARGEMINIMAP_BOTTOM + z;
									m_bDrawInformationTooltip	= TRUE;
									
									m_SelectedNPCIndex			= -1;		
									m_SelectedQuestID				= -1;
									m_SelectedPartyMemberIndex		= -1;
								}
							}//
						}
					}
				}//if( !m_bDrawInformationTooltip )
			}
		}
	}
}
#endif			

void _XWindow_WorldMinimap::DrawLargeMinipMapNPCPosition( int left, int bottom, float rateofmapsizetoterrain, float size  )
{		
	m_bDrawInformationTooltip = FALSE;
	for(int i = 0 ; i < g_Npclist.m_NPCObjectCount ; ++i)
	{		
		float x = g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._41 * rateofmapsizetoterrain + (size/2);
		float z = -g_Npclist.m_pNPCObjectArray[i].m_matWorldPosition._43 * rateofmapsizetoterrain - (size/2);
		
		if(g_Npclist.m_pNPCObjectArray[i].m_NPCRollType == _XNPCROLL_TRADE || g_Npclist.m_pNPCObjectArray[i].m_NPCRollType == _XNPCROLL_WAREHOUSE )
		{
			m_NPCImageTradeType.MoveWindow( left - 4 + x, bottom - 4 + z, left + 3 + x, bottom + 3 + z);
			m_NPCImageTradeType.DrawWithRegion();
			
		}
		else if( g_Npclist.m_pNPCObjectArray[i].m_NPCRollType == _XNPCROLL_EVENT )
		{
			m_NPCImageEventType.MoveWindow( left - 4 + x, bottom - 4 + z, left + 3 + x, bottom + 3 + z);
			m_NPCImageEventType.DrawWithRegion();
		}
		
		if( !m_bDrawInformationTooltip )
		{
			ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
			if((scrnpos->x > left - 4 + x) )
			{
				if( scrnpos->x < left + 3 + x )
				{
					if((scrnpos->z > bottom - 4 + z) )
					{
						if(scrnpos->z < bottom + 3 + z)
						{
							// NPC 위치에 오버됨. NPC 정보를 표시한다. 
							m_SelectedNPCIndex			= i;				
							m_TooltipTargetPosition_X	= left + x;
							m_TooltipTargetPosition_Y	= bottom + z;
							m_bDrawInformationTooltip	= TRUE;
							
							m_SelectedQuestID				= -1;
							m_SelectedPartyMemberIndex		= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
							memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
						}
					}
				}
			}
		}
	}
	
	if( !g_QuestScriptManager.m_listQuestMark.empty() )
	{		
		list <_XQUEST_MARKINFO>::iterator iter_questmark;
		for(iter_questmark = g_QuestScriptManager.m_listQuestMark.begin() ; iter_questmark != g_QuestScriptManager.m_listQuestMark.end() ; ++iter_questmark)
		{
			_XQUEST_MARKINFO questmark = *iter_questmark;
			
			int questmarkpositionX = questmark.MarkPosition_X * rateofmapsizetoterrain + size/2;
			int questmarkpositionY = -questmark.MarkPosition_Y * rateofmapsizetoterrain - size/2;
			
			// 현재 진행되는 퀘스트의 종류에 따라 색을 바꿔서 표시한다.
			switch( questmark.questtype ) {
			case _XQUEST_TYPE_1://제룡행 
				{
					m_YellowImpressionMark[0]->Draw( left - 7 + questmarkpositionX, bottom - 7 + questmarkpositionY);
				}
				break;
			case _XQUEST_TYPE_2://무림행 
				{
					m_BlueImpressionMark[0]->Draw( left - 7 + questmarkpositionX, bottom - 7 + questmarkpositionY);
				}
				break;
			case _XQUEST_TYPE_3://협객행
				{
					m_OrangeImpressionMark[0]->Draw( left - 7 + questmarkpositionX, bottom - 7 + questmarkpositionY);
				}
				break;
			case _XQUEST_TYPE_4://기연행 
				{
					m_GreenImpressionMark[0]->Draw(left - 7 + questmarkpositionX, bottom - 7 + questmarkpositionY);
				}
				break;
			case _XQUEST_TYPE_5://무림소사
				{
					m_WhiteImpressionMark[0]->Draw( left - 7 + questmarkpositionX, bottom - 7 + questmarkpositionY);
				}
				break;
			}
			
			if( !m_bDrawInformationTooltip )
			{
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
				if((scrnpos->x > left - 7 + questmarkpositionX) )
				{
					if( scrnpos->x < left + 8 + questmarkpositionX )
					{
						if((scrnpos->z > bottom - 7 + questmarkpositionY) )
						{
							if(scrnpos->z < bottom + 8 + questmarkpositionY)
							{
								// Quest 위치에 오버됨. Quest 정보를 표시한다. 
								m_SelectedQuestType			= questmark.questtype;
								m_SelectedQuestID			= questmark.questid;
								
								m_TooltipTargetPosition_X	= left + questmarkpositionX;
								m_TooltipTargetPosition_Y	= bottom + questmarkpositionY;
								m_bDrawInformationTooltip	= TRUE;
								
								m_SelectedPartyMemberIndex		= -1;
								m_SelectedNPCIndex				= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
								memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif	
							}
						}
					}
				}
			}//if( !m_bDrawInformationTooltip )
		}
	}
	if( !g_QuestScriptManager.m_listHintMark.empty() )
	{		
		list <_XQUEST_MARKINFO>::iterator iter_hintmark;
		for(iter_hintmark = g_QuestScriptManager.m_listHintMark.begin() ; iter_hintmark != g_QuestScriptManager.m_listHintMark.end() ; ++iter_hintmark)
		{
			_XQUEST_MARKINFO hintmark = *iter_hintmark;
			
			int hintmarkpositionX = hintmark.MarkPosition_X * rateofmapsizetoterrain + size/2;
			int hinttmarkpositionY = -hintmark.MarkPosition_Y * rateofmapsizetoterrain - size/2;
			
			// 현재 진행되는 퀘스트의 종류에 따라 색을 바꿔서 표시한다.
			switch( hintmark.questtype ) {
			case _XQUEST_TYPE_1://제룡행 
				{
					m_YellowQuestionMark->Draw(left - 7 + hintmarkpositionX, bottom - 7 + hinttmarkpositionY);
				}
				break;
			case _XQUEST_TYPE_2://무림행 
				{
					m_BlueQuestionMark->Draw(left - 7 + hintmarkpositionX, bottom - 7 + hinttmarkpositionY);
				}
				break;
			case _XQUEST_TYPE_3://협객행
				{
					m_OrangeQuestionMark->Draw(left - 7 + hintmarkpositionX, bottom - 7 + hinttmarkpositionY);
				}
				break;
			case _XQUEST_TYPE_4://기연행 
				{
					m_GreenQuestionMark->Draw(left - 7 + hintmarkpositionX, bottom - 7 + hinttmarkpositionY);
				}
				break;
			case _XQUEST_TYPE_5://무림소사
				{
					m_WhiteQuestionMark->Draw(left - 7 + hintmarkpositionX, bottom - 7 + hinttmarkpositionY);
				}
				break;
			}
			
			if( !m_bDrawInformationTooltip )
			{
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
				if((scrnpos->x > left - 7 + hintmarkpositionX) )
				{
					if( scrnpos->x < left + 8 + hintmarkpositionX )
					{
						if((scrnpos->z > bottom - 7 + hinttmarkpositionY) )
						{
							if(scrnpos->z < bottom + 8 + hinttmarkpositionY)
							{
								// Quest 위치에 오버됨. Quest 정보를 표시한다. 
								m_SelectedQuestType			= hintmark.questtype;
								m_SelectedQuestID			= hintmark.questid;
								
								m_TooltipTargetPosition_X	= left + hintmarkpositionX;
								m_TooltipTargetPosition_Y	= bottom + hinttmarkpositionY;
								m_bDrawInformationTooltip	= TRUE;
								
								m_SelectedPartyMemberIndex		= -1;
								m_SelectedNPCIndex				= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
								memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif	
							}
						}
					}
				}
			}//if( !m_bDrawInformationTooltip )
		}
	}

	if( !m_bDrawInformationTooltip )
	{
		m_SelectedQuestID				= -1;
		m_SelectedPartyMemberIndex		= -1;
		m_SelectedNPCIndex				= -1;	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
		memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif	
	}
}
void _XWindow_WorldMinimap::DrawLargeMinipMapPartyPlayerPosition(int left, int bottom, float rateofmapsizetoterrain, float size)
{
	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; ++i)  
	{
		if( g_pLocalUser->m_PartyInfo[i].playername[0] != 0 && g_pLocalUser->m_PartyInfo[i].isValid)
		{
			BOOL isCurrentUser = FALSE;
			CheckCurrentUser(i,isCurrentUser);
			
			if(!isCurrentUser)
			{
				if(g_pLocalUser->m_PartyInfo[i].serverno == g_CurrentZoneInfoIndex)
				{
					float x = g_pLocalUser->m_PartyInfo[i].posX * rateofmapsizetoterrain + size/2;
					float z = -g_pLocalUser->m_PartyInfo[i].posZ * rateofmapsizetoterrain - size/2;
					
					m_PartyPlayerImage.MoveWindow(left - 3 + x, bottom - 3 + z, left + 2 + x, bottom + 2 + z);
					m_PartyPlayerImage.DrawWithRegion();
					
					if( !m_bDrawInformationTooltip )
					{
						ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
						if((scrnpos->x > left - 3 + x) )
						{
							if( scrnpos->x < left + 4 + x )
							{
								if((scrnpos->z > bottom - 3 + z) )
								{
									if(scrnpos->z < bottom + 4 + z)
									{
										// 파티원 위치에 오버됨. 파티원 정보를 표시한다. 
										m_SelectedPartyMemberIndex	= i;	
										
										m_TooltipTargetPosition_X	= left + x;
										m_TooltipTargetPosition_Y	= bottom + z;
										m_bDrawInformationTooltip	= TRUE;
										
										m_SelectedQuestID			= -1;
										m_SelectedNPCIndex			= -1;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
										memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
									}
								}
							}
						}
					}//if( !m_bDrawInformationTooltip )
				}
			
			}
		}
	}
	if( !m_bDrawInformationTooltip )
	{
		m_SelectedQuestID				= -1;
		m_SelectedPartyMemberIndex		= -1;
		m_SelectedNPCIndex				= -1;		
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
		memset( m_cPKTracingName, 0, sizeof(TCHAR)*256 );
#endif
	}
}

#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
void _XWindow_WorldMinimap::DrawPKWarning( void )
{
	
	if(m_dwPrevDrawTimePKWarning == 0)
		m_dwPrevDrawTimePKWarning = g_LocalSystemTime;
	
	if( m_PKWarningLevel == 3 )
	{
		// 0.4초 깜빡임 2초 딜레이
		DWORD elapsedtime = g_LocalSystemTime - m_dwPrevDrawTimePKWarning;
		if( elapsedtime >= 1600 && elapsedtime <= 2000)
		{
			_XDrawSolidBar( m_WindowPosition.x + 2,  m_WindowPosition.y + 16 +15, 
				m_WindowPosition.x + m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy - 27 , D3DCOLOR_ARGB(128,255,0,0) );
			
		}
		else if( elapsedtime > 2400 )
		{
			m_dwPrevDrawTimePKWarning = 0;
		 }
	}
	else if( m_PKWarningLevel == 2 )
	{
		// 0.4초 깜빡임 1초 딜레이
		DWORD elapsedtime = g_LocalSystemTime - m_dwPrevDrawTimePKWarning;
		if( elapsedtime >= 600 && elapsedtime <= 1000)
		{
			_XDrawSolidBar( m_WindowPosition.x + 2,  m_WindowPosition.y + 16 +15, 
				m_WindowPosition.x + m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy - 27 , D3DCOLOR_ARGB(128,255,0,0) );
			
		}
		else if( elapsedtime > 1400 )
		{
			m_dwPrevDrawTimePKWarning = 0;
		 }
	}
	else if( m_PKWarningLevel == 1 )
	{
		// 0.4초 깜빡임 0.2초 딜레이
		DWORD elapsedtime = g_LocalSystemTime - m_dwPrevDrawTimePKWarning;
		if( elapsedtime >= 100 && elapsedtime <= 500)
		{
			_XDrawSolidBar( m_WindowPosition.x + 2,  m_WindowPosition.y + 16 +15, 
				m_WindowPosition.x + m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy - 27 , D3DCOLOR_ARGB(128,255,0,0) );
			
		}
		else if( elapsedtime > 600 )
		{
			m_dwPrevDrawTimePKWarning = 0;
		 }
	}
	
}

void _XWindow_WorldMinimap::ProcessPKTracingDistance( void )
{
	if( !m_listPKTracingPosition.empty() )
	{				
		BOOL bNewData = FALSE;
		int tempPKWarningLevel = 0;
		_XPKTRACING_POSITION_INFO* pktracingposition = NULL;
		list <_XPKTRACING_POSITION_INFO*>::iterator iter_pktracingposition;
		for(iter_pktracingposition = m_listPKTracingPosition.begin() ; iter_pktracingposition != m_listPKTracingPosition.end(); ++iter_pktracingposition)
		{
			pktracingposition = *iter_pktracingposition;
			if( pktracingposition )
			{
				FLOAT dx = g_pLocalUser->m_Position.x - pktracingposition->sPosX;
				FLOAT dz = g_pLocalUser->m_Position.z - pktracingposition->sPosZ;
				FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);

				if( distance <= 200 && distance >100)
				{					
					if( tempPKWarningLevel != 3 && tempPKWarningLevel != 2 && tempPKWarningLevel != 1)
					{
						tempPKWarningLevel = 3;	
						m_bDrawPKWarning = TRUE;
						bNewData = TRUE;
					}
				}
				else if( distance <= 100 && distance >50)
				{					
					if( tempPKWarningLevel != 2 && tempPKWarningLevel != 1)
					{
						tempPKWarningLevel = 2;	
						m_bDrawPKWarning = TRUE;
						bNewData = TRUE;
					}
				}
				else if( distance <= 50 )
				{				
					if( tempPKWarningLevel != 1 )
					{
						tempPKWarningLevel = 1;	
						m_bDrawPKWarning = TRUE;
						bNewData = TRUE;
					}
				}
				else
				{	
					m_PKWarningLevel = 0;
					m_bDrawPKWarning = FALSE;	
				}
			}
		}

		if( (tempPKWarningLevel != 0) && (tempPKWarningLevel != m_PKWarningLevel) )
		{
			m_PKWarningLevel = tempPKWarningLevel;
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3388),_XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );//은원관계가 있는 상대가 근접해 있습니다.
		}
	}
}

#endif

void _XWindow_WorldMinimap::SetMinimapRateInCastleBattle()
{
	if(!m_bDisableZoomButton)
	{
		m_bDisableZoomButton = TRUE;

		m_MinimapZoomRate = 0.075f;					
		m_RateOfMapSizeToTerrain = 1.0f;
		SetLocalUserPosition();
	}
}

void _XWindow_WorldMinimap::SetMinimapTransparency( int nResult )
{
#ifdef _XDEF_MINIMAP_TRANSPARENCY_071012_MAGVIPER //Author : 양희왕

	m_LargeMinimapImage.m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); //큰지도
	//m_MinimapArrow.RenderAlphaBlend( nResult );
	//m_MinimapArrow.m_FaceColor = D3DCOLOR_ARGB( nResult, nResult, nResult, nResult); 
	//m_MinimapArrow.SetFaceColor(D3DCOLOR_ARGB( nResult, nResult, nResult, nResult )); //화살표
	m_FrustumeImage.m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_PartyPlayerImage.m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_NPCImageTradeType.m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_NPCImageEventType.m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_imagePKTracingPoint.m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_imageSomeOneTracingPoint.m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 

	for( int i = 0; i < 4; i++ )
	{
		m_YellowImpressionMark[i]->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
		m_GreenImpressionMark[i]->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
		m_BlueImpressionMark[i]->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
		m_OrangeImpressionMark[i]->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
		m_WhiteImpressionMark[i]->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 

		// NPC 찾기 관련
		m_imageNPCSearchMark[i]->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	}
	
	m_YellowQuestionMark->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_GreenQuestionMark->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_BlueQuestionMark->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_OrangeQuestionMark->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	m_WhiteQuestionMark->m_FColor = D3DCOLOR_ARGB( nResult, 255, 255, 255 ); 
	
	
#endif
}
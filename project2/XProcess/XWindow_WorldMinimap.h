// _XWindow_WorldMinimap.h: interface for the _XWindow_WorldMinimap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX__XWINDOW_WORLDMINIMAP_H__E7F1E493_4EAE_4606_97E4_AED1FBB713F3__INCLUDED_)
#define AFX__XWINDOW_WORLDMINIMAP_H__E7F1E493_4EAE_4606_97E4_AED1FBB713F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XQuestScriptBaseType.h"
#include "XNPC.h"

typedef enum _XMiniMapMode
{
	_XMINIMAPMODE_SMALL		= 0,
		_XMINIMAPMODE_LARGE
		
}_XMINIMAPMODE;


#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321	// PK범위치추적, 레벨차 PK제한
// 상용 아이템 보관함 아이템 정보
typedef struct 
{
	u_char		ucPKType;	//1.Pk 2. 특정인
	char		szCharacName[_XDEF_MAX_USERNAMESTRINGLENGTH];
	BOOL		bRenewalData;
	
	short sPosX;
	short sPosZ;
	
}_XPKTRACING_POSITION_INFO;
#endif 

class _XWindow_NPCSearch;
class _XWindow_WorldMinimap : public _XWindow  
{
public:
	_XWindow_NPCSearch*		m_pNPCSearchWindowPtr;

	_XImageStatic*  m_pTitleBar;
	_XImageStatic*	m_pSubBar;
	_XImageStatic*  m_pBottomLeftBar;
	_XImageStatic*  m_pBottomRightBar;
	_XImageStatic*	m_pLeftEdgeImage;
	_XImageStatic*	m_pSunMoonIcon;
	
	// 큰지도 일때 
	_XImageStatic*  m_pMaximumMapTitleLeftBar;
	_XImageStatic*  m_pMaximumMapTitleMiddleBar;
	_XImageStatic*  m_pMaximumMapTitleRightBar;
	
	_XImageStatic*  m_pMaximumMapBottomMiddleBar1;
	_XImageStatic*  m_pMaximumMapBottomMiddleBar2;
	
	// Quest 관련
	_XImageStatic*	m_YellowImpressionMark[4];
	_XImageStatic*	m_GreenImpressionMark[4];
	_XImageStatic*	m_BlueImpressionMark[4];
	_XImageStatic*	m_OrangeImpressionMark[4];
	_XImageStatic*	m_WhiteImpressionMark[4];
	
	_XImageStatic*	m_YellowQuestionMark;
	_XImageStatic*	m_GreenQuestionMark;
	_XImageStatic*	m_BlueQuestionMark;
	_XImageStatic*	m_OrangeQuestionMark;
	_XImageStatic*	m_WhiteQuestionMark;
	
	// NPC 찾기 관련
	_XImageStatic*	m_imageNPCSearchMark[4];

	_XButton*		minimizebutton;
	_XButton*		zoommaxbutton;
	_XButton*		zoominbutton;
	_XButton*		zoomoutbutton;
	_XButton*		worldmapbutton;
	_XButton*		helpbookbutton;
	_XButton*		m_btnNPCSearch;	
#ifdef _XDEF_WEBBOARD_20061211
	_XButton*		m_btnWebBoard;	// 웹 게시판을 여는 버튼
#endif	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	_XButton*		m_btnPKTracingInfo;
#endif			

	DWORD			m_PrevDrawTime;
	DWORD			m_PrevDrawTimeNPCSearch;	
	// 동행 찾기 
	DWORD			m_dwPrevDrawTimePartySearch;
	//PK 경고 표시 
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	DWORD			m_dwPrevDrawTimePKWarning;
#endif			
	// Large Map Mode
	_XStatic		m_Minimap;
	_XStatic		m_MinimapArrow;	
	FLOAT			m_RotateAngle;
	
	FLOAT			m_FrustumRotateAngle;
	FLOAT			m_RateOfMapSizeToTerrain;
	FLOAT			m_RateOfLargeMapSizeToTerrain;	
	// Large Map Mode
	FLOAT			m_MinimapZoomRate; // UV....
	FLOAT			m_EffectFrame;
	FLOAT			m_EffectFrameNPCSearch;
	// 동행 찾기 
	FLOAT			m_fEffectFramePartySearch;
	//PK 경고 표시 
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	FLOAT			m_fEffectFramePKWarning;
#endif			
	
	float			m_CheckLocalUserPosition_X;
	float			m_CheckLocalUserPosition_Y;

	int				m_TooltipTargetPosition_X;
	int				m_TooltipTargetPosition_Y;
	int				m_SelectedQuestID;
	int				m_SelectedPartyMemberIndex;
	int				m_SelectedNPCIndex;
	int				m_CurrentServerIndex;
	// 동행 찾기 
	int				m_nTotalDrawTimePartySearch;	
	int				m_nSearchPartyMemberIndex;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	int				m_PKWarningLevel;
#endif			
	// 동행 찾기 		
	_XImageStatic	m_FrustumeImage;
	_XImageStatic	m_PartyPlayerImage;
	_XImageStatic	m_NPCImageTradeType;
	_XImageStatic	m_NPCImageEventType;
	// Large Map Mode
	_XImageStatic	m_LargeMinimapImage;
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	_XImageStatic	m_imagePKTracingPoint;
	_XImageStatic	m_imageSomeOneTracingPoint;
#endif			

	BOOL			m_MinimizeStatus;
	BOOL			m_bDrawInformationTooltip;
	BOOL			m_bDrawNPCSearchEffect;
	
	bool			m_EffectOrderSub;
	// 동행 찾기 
	BOOL			m_bDrawPartyMemberSearchEffect;
	
	//PK관련 
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	BOOL			m_bDrawPKWarning; //PK범 경고 이미지표시하기 
#endif			
	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	TCHAR			m_cPKTracingName[256]; //PK범 이름
#endif			

	_XMINIMAPMODE	m_MiniMapMode;
	_XQUEST_TYPE	m_SelectedQuestType;
	_XNPCJOBCLASS	m_SelectedNPCJobIndex;
	
	list <int>		m_listDrawNPCIndex;	
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	list <_XPKTRACING_POSITION_INFO*>		m_listPKTracingPosition;		// pk범 위치 추적 정보
#endif			

	LPDIRECT3DTEXTURE9 m_pMinimapTexture;

	BOOL			m_bDisableZoomButton;

	_XHSlider*		m_pAlphaSlider; //Author : 양희왕 //breif : 인터페이스 수정 미니맵 투명도
	int				m_nTransparency; //Author : 양희왕 //breif : 투명도

public:
	_XWindow_WorldMinimap();
	virtual ~_XWindow_WorldMinimap();
	
	BOOL			Initialize( void );
	void			DestroyWindow( void );
	
	void			MoveWindow( int X, int Y );
	void			Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL			Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void			ShowWindow(BOOL show);
	
	void			ProcessDblClick();
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	void			ProcessPKTracingDistance(void);
#endif			
	
	void			SetRotateArrow( FLOAT angle );
	void			SetLocalUserPosition( void );
	void			SetLocalUserPosition( int left, int bottom, float rateofmapsizetoterrain, float size );
	void			SetRotateFrustum( FLOAT angle );
	
	BOOL			SetMinimapImage( LPTSTR mapname );	
	void			SetMinimapRate();	// 2004.05.21->oneway48 insert
	
	void			SetDefaultPosition( void );
	void			SaveWindowProperty( void );
	
	void			DrawNPCPosition(_XGUIObject*& pfocusobject);
	void			DrawPartyPlayerPosition(_XGUIObject*& pfocusobject);
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	void			DrawPKTracingPosition(_XGUIObject*& pfocusobject);
#endif			
		
	void			DrawLargeMinipMapNPCPosition(_XGUIObject*& pfocusobject);
	void			DrawLargeMinipMapPartyPlayerPosition(_XGUIObject*& pfocusobject);
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	void			DrawLargeMinipMapPKTracingPosition(_XGUIObject*& pfocusobject);
#endif			

	void			DrawLargeMinipMapNPCPosition( int left, int bottom, float rateofmapsizetoterrain, float size );
	void			DrawLargeMinipMapPartyPlayerPosition( int left, int bottom, float rateofmapsizetoterrain, float size );
	
	//PK관련 
#ifdef _XDEF_PK_ADDTIONALFUNCTION01_20070321
	void			DrawPKWarning( void );	
#endif			

	void			CheckCurrentUser(int partyindex, BOOL& isCurrentUser);
	void			SetCheckLocalUserPosition( float x, float z);
	
	void			SetCurrentZoneIndex( int serverindex ){	m_CurrentServerIndex = serverindex; }

	void			RebuildCurrentTimeIcon( void );
	void			RebuildDrawNPCList( float userposition_x, float userposition_z );

	void			ChangeMinimizeStatus( void );

	void			SetMode( _XMINIMAPMODE minimapmode );
	_XMINIMAPMODE	GetMode( void ){ return m_MiniMapMode; }

	void			SetMinimapRateInCastleBattle(void);
	void			SetMinimapTransparency( int nResult );
};

#endif // !defined(AFX__XWINDOW_WORLDMINIMAP_H__E7F1E493_4EAE_4606_97E4_AED1FBB713F3__INCLUDED_)

// XWindow_PartyMemberMiniBar.cpp: implementation of the _XWindow_PartyMemberMiniBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_PartyMemberMiniBar.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XCallback_PartyMember_Close( POINT pos, SIZE size )
{
	if( g_pMainFrame_Window && !g_pMainFrame_Window->GetShowStatus() )
	{
//		bool checkleader = FALSE;
//		for(int i = 0 ; i < 9 ; i++)
//		{
//			if( g_pLocalUser->m_PartyInfo[i].isLeader )
//			{
//				if( strcmp(g_pLocalUser->m_PartyInfo[i].playername, g_pLocalUser->m_CharacterInfo.charactername) == 0 )
//				{
//					checkleader = TRUE;
//				}
//				break;
//			}
//		}
//		if( checkleader )
		
		if( g_pLocalUser->m_PartyInfo[g_pLocalUser->m_PartySelfIndex].isLeader )
		{
			g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_PARTY_2567), _XSC_INFORMATION );	
		}
		else
		{
			g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_PARTY_2568), _XSC_INFORMATION );	
		}
	}
	
}

void __stdcall _XCallback_PartyMember_Entrust( POINT pos, SIZE size )
{
	if( g_pMainFrame_Window && !g_pMainFrame_Window->GetShowStatus() )
	{
		g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_PARTY_2569), _XSC_INFORMATION );	
	}
}

_XWindow_PartyMemberMiniBar::_XWindow_PartyMemberMiniBar()
{
	m_CloseButtoncommandid			= 0;	
	m_EntrustButtoncommandid		= 0;
	m_imagePartyMemberBorder		= NULL;			// 동행 멤버창 바탕 이미지 	
	m_pPartyMemberLifePowerGauge	= NULL;			// 동행 멤버창 체력 게이지 
	m_pPartyMemberForcePowerGauge	= NULL;			// 동행 멤버창 진기 게이지 	
	m_btnPartyMemberClose			= NULL;			// 동행 행수창 닫기 버튼 
	m_btnPartyMasterEntrust			= NULL;			// 동행 행수창 위임 버튼 	
	
	// 경고 애니매이션 관련 변수 
	m_bDangerous					= FALSE;		// 위험한 상태일때 
	m_PrevDrawTime					= 0;
	m_EffectOrderSub				= FALSE;
	m_EffectFrame					= 0.0f;

	m_PlayerUniqueid				= 0;			//파티 멤버의 아이디 
	m_bIsCurrentUser				= FALSE;
	m_bDeathUser					= FALSE;
	
	m_PartyIndex					= -1;

	m_ZoneIndex						= -1;
	m_bIsCurrentZone				= FALSE;
	memset( m_PartyMemberName, 0, sizeof(m_PartyMemberName) );
	
	for(int i = 0 ; i < 3 ; i++)
		m_pPartyMemberDanger[i] = NULL;

	m_pFarActionIcon				= NULL;
	m_pOutActionIcon				= NULL;
	m_pClassIcon					= NULL;
	m_pClassBorderIcon				= NULL;
	
	m_bOut	= false;
	m_bFarAway = false;
}

_XWindow_PartyMemberMiniBar::~_XWindow_PartyMemberMiniBar()
{

}

BOOL _XWindow_PartyMemberMiniBar::Initialize( void )
{
	int partyresourceindex = g_MainInterfaceTextureArchive.FindResource( "mi_party.tga" );
	int charinforesourceindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	
#ifdef _XDEF_PARTY_NOTAVAILABLE
	// 동행 멤버창 바탕 이미지 ( 160 X 36 ) 
	m_imagePartyMemberBorder = new _XImageStatic;
	m_imagePartyMemberBorder->Create( 0, 0, 160, 36, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_party2.tga") );
	m_imagePartyMemberBorder->SetClipRect(0, 57, 160, 93);
	m_imagePartyMemberBorder->ShowWindow(TRUE);
	InsertChildObject( m_imagePartyMemberBorder );
#else
	// 동행 멤버창 바탕 이미지 ( 146 X 36 ) 
	m_imagePartyMemberBorder = new _XImageStatic;
	m_imagePartyMemberBorder->Create( 0, 0, 146, 36, &g_MainInterfaceTextureArchive, partyresourceindex );
	m_imagePartyMemberBorder->SetClipRect(0, 143, 146, 179);
	m_imagePartyMemberBorder->ShowWindow(TRUE);
	InsertChildObject( m_imagePartyMemberBorder );
#endif
	
	// 동행 멤버창 닫기 버튼	 ( 12 X 12 ) 
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 128, 5 }, { 12, 12 }, m_CloseButtoncommandid,
		charinforesourceindex,charinforesourceindex,charinforesourceindex, 
		&g_MainInterfaceTextureArchive };
	m_btnPartyMemberClose = new _XButton;
	m_btnPartyMemberClose->Create( closebtnstruct );	
	m_btnPartyMemberClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_btnPartyMemberClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_btnPartyMemberClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	m_btnPartyMemberClose->ShowWindow(TRUE);
	m_btnPartyMemberClose->SetCallBackFunction_ToolTip( _XCallback_PartyMember_Close );
	InsertChildObject( m_btnPartyMemberClose );
	
	// 동행 위임 버튼	 ( 12 X 12 ) 
	_XBTN_STRUCTURE entrustbtnstruct = { TRUE, { 115, 5 }, { 12, 12 }, m_EntrustButtoncommandid,
		partyresourceindex,partyresourceindex,partyresourceindex, 
		&g_MainInterfaceTextureArchive };
	m_btnPartyMasterEntrust = new _XButton;
	m_btnPartyMasterEntrust->Create( entrustbtnstruct );	
	m_btnPartyMasterEntrust->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 204, 2, 216, 14 );
	m_btnPartyMasterEntrust->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  216, 2, 228, 14 );
	m_btnPartyMasterEntrust->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  228, 2, 240, 14 );
	m_btnPartyMasterEntrust->ShowWindow(TRUE);
	m_btnPartyMasterEntrust->SetCallBackFunction_ToolTip( _XCallback_PartyMember_Entrust );
	InsertChildObject( m_btnPartyMasterEntrust );
	
	
	// 동행 멤버창 체력 게이지 ( 130 X 5)
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 8, 20 }, { 130, 5 },	
			partyresourceindex, -1, &g_MainInterfaceTextureArchive, 
	};
	
	m_pPartyMemberLifePowerGauge = new _XStickGauge;
	m_pPartyMemberLifePowerGauge->Create( gaugestruct );	
	m_pPartyMemberLifePowerGauge->SetImageClipRect( _XGAUGEIMAGE_BORDER,  3,  58, 133,  63 );
	m_pPartyMemberLifePowerGauge->SetFactorDraw(TRUE);	
	InsertChildObject( m_pPartyMemberLifePowerGauge );
	
	// 동행 멤버창 진기 게이지 ( 130 X 5)
	gaugestruct.position.y = 27;
	m_pPartyMemberForcePowerGauge = new _XStickGauge;
	m_pPartyMemberForcePowerGauge->Create( gaugestruct );	
	m_pPartyMemberForcePowerGauge->SetImageClipRect( _XGAUGEIMAGE_BORDER,  3,  65, 133,  70 );
	m_pPartyMemberForcePowerGauge->SetFactorDraw(TRUE);	
	InsertChildObject( m_pPartyMemberForcePowerGauge );
		
	// 위험 표시 이미지 ( 39 X 32 )
	for(int i = 0 ; i < 3 ; i++)
	{
		m_pPartyMemberDanger[i] = new _XImageStatic;
		m_pPartyMemberDanger[i]->Create(2, 0, 26, 17, &g_MainInterfaceTextureArchive, partyresourceindex);
	}
	m_pPartyMemberDanger[0]->SetClipRect(168, 17, 192, 34);
	m_pPartyMemberDanger[1]->SetClipRect(192, 17, 216, 34);
	m_pPartyMemberDanger[2]->SetClipRect(216, 17, 240, 34);	

	memset( m_PartyMemberName, 0, sizeof(m_PartyMemberName));
	
#ifdef _XTS_TARGET_STATE
	for(i = 0 ; i < 32 ; i++)
	{
		m_StateImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, -1);
	}
#endif

#ifdef _XDEF_PARTY_NOTAVAILABLE
	m_pFarActionIcon = new _XImageStatic;
	m_pFarActionIcon->Create(0, 0, 11, 11, &g_MainInterfaceTextureArchive, partyresourceindex);
	m_pFarActionIcon->SetClipRect(192, 36, 203, 47);

	m_pOutActionIcon = new _XImageStatic;
	m_pOutActionIcon->Create(0, 0, 11, 11, &g_MainInterfaceTextureArchive, partyresourceindex);
	m_pOutActionIcon->SetClipRect(192, 36, 203, 47);
#endif

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	m_pClassIcon = new _XImageStatic;
	m_pClassIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("skill_icon_08_02.tga"));
	m_pClassIcon->SetClipRect(2, 2, 32, 32);
	//m_pClassIcon->SetScale( 0.5f, 0.5f );

	m_pClassBorderIcon = new _XImageStatic;
	m_pClassBorderIcon->Create(0, 0, 40, 36, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_Char_01.tga"));
	m_pClassBorderIcon->SetClipRect(198, 121, 238, 157);
#endif

	return TRUE;
}

void _XWindow_PartyMemberMiniBar::DestroyWindow( void )
{	
	for(int i = 0 ; i < 3 ; i++)
		SAFE_DELETE(m_pPartyMemberDanger[i]);

	SAFE_DELETE(m_pFarActionIcon);
	SAFE_DELETE(m_pOutActionIcon);
	SAFE_DELETE(m_pClassIcon);
	SAFE_DELETE(m_pClassBorderIcon);

	_XWindow::DestroyWindow();	
}

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
extern void _XDrawShadowedRectAngle( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT baseheight, D3DCOLOR highlightcolor, D3DCOLOR shadowcolor  );
#endif

void _XWindow_PartyMemberMiniBar::Draw( _XGUIObject*& pfocusobject )
{			
	if( !this->m_ShowWindow ) return;
	
	_XWindow::Draw(pfocusobject);	

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
	
	if( m_pClassBorderIcon )
		m_pClassBorderIcon->Draw( m_WindowPosition.x - 40, m_WindowPosition.y );

	if( m_pClassIcon )
		m_pClassIcon->Draw( m_WindowPosition.x - 35, m_WindowPosition.y + 3 );

	//_XDrawShadowedRectAngle( m_WindowPosition.x + 8, m_WindowPosition.y + 2, 
	//						 m_WindowPosition.x + 8 + 16, m_WindowPosition.y + 2 + 16, 0.0f, D3DCOLOR_ARGB(255,100,100,100), D3DCOLOR_ARGB(255,32,32,32)  );

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();	
	
	if( ((scrnpos->x > m_WindowPosition.x-40) ) && (scrnpos->x < m_WindowPosition.x) &&
		(scrnpos->z > m_WindowPosition.y ) && (scrnpos->z < m_WindowPosition.y+36) )
	{
		int classid = g_pLocalUser->m_PartyInfo[m_PartyIndex].clanclass;		

		if(classid > 0)
		{
			if(g_pLocalUser->m_PartyInfo[m_PartyIndex].clan == _XGROUP_BEEGOONG)
			{
				if(classid == 7)	// 섭혼녀
					classid = 4;
			}
			else if(g_pLocalUser->m_PartyInfo[m_PartyIndex].clan == _XGROUP_NOCKRIM)
			{
				if(classid == 8)	// 철갑귀
					classid = 4;
			}

			g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x, m_WindowPosition.y+3, _XGETINTERFACETEXT(ID_STRING_NEW_2660+classid-1), _XSC_INFORMATION, FALSE);
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 0)
		{
			g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x, m_WindowPosition.y+3, _XGETINTERFACETEXT(ID_STRING_NEW_3713), _XSC_INFORMATION, FALSE);
		}
	}
#endif

	if( !m_bDangerous )
	{
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
		g_XBaseFont->Puts( m_WindowPosition.x +34, m_WindowPosition.y + 4, m_PartyMemberName );
		g_XBaseFont->Flush();	
		g_XBaseFont->DisableGrowMode();

		if( !m_bIsCurrentZone )
		{
			// ( 132 X 14 )
			_XDrawSolidBar( m_WindowPosition.x+7,  m_WindowPosition.y+19, m_WindowPosition.x+139, m_WindowPosition.y+33 , D3DCOLOR_ARGB(255,11,12,18) );
			_XDrawRectAngle( m_WindowPosition.x+6,  m_WindowPosition.y+18, m_WindowPosition.x+139, m_WindowPosition.y+33 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
			
			TCHAR messagestring[64];
			memset( messagestring, 0, sizeof(TCHAR)*64 );	
			sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_PARTY_2292), g_ZoneInfoTable[ m_ZoneIndex - 1].zoneshortname );
			g_XBaseFont->Puts( m_WindowPosition.x+13,  m_WindowPosition.y+21, messagestring );
			g_XBaseFont->Flush();	
		}
	}
	else
	{
		if( !m_bDeathUser )
		{
			DrawDangerMode();
			g_XBaseFont->SetColor( _XSC_WARNING_HIGHLIGHT );
			g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
			g_XBaseFont->Puts( m_WindowPosition.x +34, m_WindowPosition.y + 4, m_PartyMemberName );
			g_XBaseFont->Flush();	
			g_XBaseFont->DisableGrowMode();
		}
		else
		{
			g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 100, 100, 100 ) );
			g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
			g_XBaseFont->Puts( m_WindowPosition.x +34, m_WindowPosition.y + 4, m_PartyMemberName );
			g_XBaseFont->Flush();	
			g_XBaseFont->DisableGrowMode();
			
			_XDrawSolidBar( m_WindowPosition.x+7,  m_WindowPosition.y+19, m_WindowPosition.x+139, m_WindowPosition.y+33 , D3DCOLOR_ARGB(255,11,12,18) );
			_XDrawRectAngle( m_WindowPosition.x+6,  m_WindowPosition.y+18, m_WindowPosition.x+139, m_WindowPosition.y+33 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
			
			g_XBaseFont->SetColor( _XSC_WARNING_HIGHLIGHT );
			g_XBaseFont->Puts( m_WindowPosition.x+23,  m_WindowPosition.y+21, _XGETINTERFACETEXT(ID_STRING_PARTY_2293) );
			g_XBaseFont->Flush();	

		}
		
	}
	bool flag = false;
	if( CheckMousePosition() )
	{
		if( g_pMainFrame_Window && !g_pMainFrame_Window->GetShowStatus() )
		{
			if( !g_InterfaceTooltip.GetShowStatus() )
			{
				flag = true;
				if( g_pMiniTooltip_Window )
				{
					if( !m_bDangerous )
					{
						if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PARTYINFO, m_PartyIndex, 0, (int)m_bOut, (DWORD)m_bFarAway ) )
							g_pMiniTooltip_Window->ShowWindow( TRUE );
					}
					else
					{
						if( m_bDeathUser)
						{
							if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PARTYINFO, m_PartyIndex, 1, (int)m_bOut, (DWORD)m_bFarAway ) )
								g_pMiniTooltip_Window->ShowWindow( TRUE );
						}
						else
						{
							if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PARTYINFO, m_PartyIndex, 0, (int)m_bOut, (DWORD)m_bFarAway ) )
								g_pMiniTooltip_Window->ShowWindow( TRUE );
						}
					}
				}				
			}
		}
	}
	
	if( flag == false && g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_PARTYINFO && m_PartyIndex == g_pMiniTooltip_Window->GetCurrentSlotNumber() )
		g_pMiniTooltip_Window->ShowWindow( FALSE );

#ifdef _XDEF_PARTY_NOTAVAILABLE
	m_pFarActionIcon->Draw(m_WindowPosition.x+143, m_WindowPosition.y+7);
	m_pOutActionIcon->Draw(m_WindowPosition.x+143, m_WindowPosition.y+20);
#endif

#ifdef _XTS_TARGET_STATE					
	_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[m_PartyIndex].playeruniqueid);
	if(pUser)
	{
		for( int i = 0; i < m_StateCount; i++ )
		{
			m_StateImage[i].MoveWindow(m_WindowPosition.x+m_WindowSize.cx+3+17*(i%8), m_WindowPosition.y+2+17*(i/8));
			m_StateImage[i].Draw();
			if( m_StateImage[i].CheckMousePosition() )
			{
				map <int, _XCharacterState*>::iterator iter_state = pUser->m_StateList.m_mapCharacterStateList.begin();
				advance( iter_state, i );
				
				if( iter_state != pUser->m_StateList.m_mapCharacterStateList.end() )
				{
					_XCharacterState* pState = iter_state->second;
					if(pState)
					{
						POINT pos = m_StateImage[i].GetWindowPos();
						
						if(pState->m_FromType == 0)	// spell
						{								
							g_InterfaceTooltip.SetToolTipString( pos.x+32, pos.y+32, g_SkillProperty[pState->m_sSkillID]->skillName, _XSC_INFORMATION, TRUE);	
						}
						else if(pState->m_FromType == 1)	// effect
						{
							g_InterfaceTooltip.SetToolTipString( pos.x+32, pos.y+32, g_CharacterStateTable[pState->m_sEffectID]->name, _XSC_INFORMATION, TRUE);	
						}
					}
				}
			}						
		}
	}					
#endif
	
	
}

void _XWindow_PartyMemberMiniBar::DrawDangerMode(void)
{
	if(m_PrevDrawTime == 0)
		m_PrevDrawTime = g_LocalSystemTime;
	
	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
	
	if(!m_EffectOrderSub)
		m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
	else 
		m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);
	
	if(m_EffectFrame > 2.0f)
	{
		m_EffectOrderSub = true;
		m_EffectFrame = 2.0f;
	}
	else if(m_EffectFrame <= 0.0f)
	{
		m_EffectOrderSub = false;
		m_EffectFrame = 0.0f;
	}
	
	
	int selectedframe = (int)m_EffectFrame;
	
	if(selectedframe > 2 )
		selectedframe = 2;
	else if(selectedframe < 0)
		selectedframe = 0;
	
	m_pPartyMemberDanger[selectedframe]->Draw(m_WindowPosition.x+2, m_WindowPosition.y);
	m_PrevDrawTime = g_LocalSystemTime;
}


BOOL _XWindow_PartyMemberMiniBar::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if( CheckMousePosition() )
				{
					if( !m_btnPartyMemberClose->CheckMousePosition() 
					 && !m_btnPartyMasterEntrust->CheckMousePosition())
					{
						if( m_bIsCurrentUser )
						{
							g_pLocalUser->SetViewTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
							g_pLocalUser->SetAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
						}
						else
						{
							_XUser* pUser = NULL;					
							pUser = (_XUser*)g_Userlist.FindUser(m_PlayerUniqueid);
							if( pUser )
							{
								g_pLocalUser->SetViewTarget((_XItem*)pUser, _XPICKINGTARGET_PC);
							}

#ifdef _XDEF_SEARCHPARTYMEMBER
							_XWindow_WorldMinimap* pWorldMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
							if(pWorldMinimap_Window)
							{
								pWorldMinimap_Window->m_bDrawPartyMemberSearchEffect = TRUE;
								pWorldMinimap_Window->m_nSearchPartyMemberIndex = m_PartyIndex;
							}
#endif
						}
					}
				}//if( CheckMousePosition() )
			}
			else if( mousestate->bButton[1] )
			{
				// 오른쪽 클릭
				if(CheckMousePosition())
				{					
					if( !m_btnPartyMemberClose->CheckMousePosition() 
						&& !m_btnPartyMasterEntrust->CheckMousePosition())
					{
						if( !m_bIsCurrentUser )
						{
							m_WindowMouseState = _XW_STATE_RIGHTCLICK;
						}
					}
				}
			}
		}
		
		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				bool bchangetarget = false;
				char usefultype = 0;
				char targettype = 0;
				
				if(g_pLocalUser->m_SelectedSkillID > 0)
				{
					usefultype = g_pLocalUser->GetUsefulType(g_pLocalUser->m_SelectedSkillID);		
					targettype = _XSkillItem::GetTargetType(g_pLocalUser->m_SelectedSkillID); 
				}
				else
					usefultype = _XSI_USEFULTYPE_ACTIVESKILL;
				
				switch(usefultype)
				{
				case _XSI_USEFULTYPE_PASSIVESKILL :
				case _XSI_USEFULTYPE_PASSIVESPELL :
				case _XSI_USEFULTYPE_HIDDENMA :
					bchangetarget = true;
					break;
				case _XSI_USEFULTYPE_ACTIVESKILL :
					{
						if( g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || 
							g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_1 || 
							g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST )
						{
							bchangetarget = true;
						}
						
					}
					break;
				case _XSI_USEFULTYPE_CHARGESKILL :
				case _XSI_USEFULTYPE_CASTINGSPELL :
				case _XSI_USEFULTYPE_CHANNELINGSPELL :
				case _XSI_USEFULTYPE_FINISHSKILL :
				case _XSI_USEFULTYPE_ROUNDINGSPELL :
				case _XSI_USEFULTYPE_SMASHINGSKILL :
					{
						if( g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || 
							g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST )
						{
							bchangetarget = true;
						}
						
					}
					break;
				}
				
				if( bchangetarget )
				{
					_XUser* pUser = NULL;			 		
					pUser = (_XUser*)g_Userlist.FindUser(m_PlayerUniqueid);
					if( pUser )
					{
						if(pUser->m_AttackTarget)
						{
							if(pUser->m_AttackTarget == g_pLocalUser)
							{
								// user의 target이 나 인경우
								g_pLocalUser->SetViewTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
								g_pLocalUser->SetAttackTarget(g_pLocalUser, _XPICKINGTARGET_SELF);
							}
							else if(pUser->m_AttackTarget == pUser)
							{
								// user의 target이 그 user(자기자신) 인경우
								g_pLocalUser->SetViewTarget(pUser, _XPICKINGTARGET_PC);
								g_pLocalUser->SetAttackTarget(g_pLocalUser, _XPICKINGTARGET_PC);
							}
							else
							{
								g_pLocalUser->SetViewTarget((_XItem*)pUser->m_AttackTarget, pUser->m_AttackTargetObjectType);
								g_pLocalUser->SetAttackTarget((_XItem*)pUser->m_AttackTarget, pUser->m_AttackTargetObjectType );
							}
						}
					}
				}				
				
				m_WindowMouseState = _XW_STATE_NONE;
			}
		}
		
	}
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;

	
	return TRUE;
}

void _XWindow_PartyMemberMiniBar::SetPartyPlayerName(int partyindex, BOOL& isCurrentUser)
{
	memset(m_PartyMemberName, 0, sizeof(m_PartyMemberName));

	m_PartyIndex = partyindex;
	if(g_pLocalUser->m_PartyInfo[partyindex].playername[0] != 0)
	{
		if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, g_pLocalUser->m_PartyInfo[partyindex].playername) != 0)
		{
			strcpy(m_PartyMemberName, g_pLocalUser->m_PartyInfo[partyindex].playername);
			m_bIsCurrentUser = isCurrentUser = FALSE;
		}
		else
		{
			strcpy(m_PartyMemberName, g_pLocalUser->m_PartyInfo[partyindex].playername);
			m_bIsCurrentUser = isCurrentUser = TRUE;
		}
	}

	SetClassIcon();
}

void _XWindow_PartyMemberMiniBar::SetPartyPlayerVital(int partyindex, BOOL& isCurrentUser)
{
	m_PartyIndex = partyindex;
	if(g_pLocalUser->m_PartyInfo[partyindex].playername[0] != 0)
	{
		if( g_pLocalUser->m_PartyInfo[partyindex].serverno == g_CurrentZoneInfoIndex )
		{
			if( g_pLocalUser->m_PartyInfo[partyindex].vitalpercent <= 10 ) 
			{
				m_bDangerous = TRUE;
				if( g_pLocalUser->m_PartyInfo[partyindex].vitalpercent == 0 )
				{
					m_bDeathUser = TRUE;
				}
				else
				{
					m_bDeathUser = FALSE;
				}
			}
			else
			{
				m_bDangerous = FALSE;
			}
		}
		else
		{
			m_bDangerous = FALSE;
		}
		
		if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, g_pLocalUser->m_PartyInfo[partyindex].playername) != 0)
		{
			m_pPartyMemberLifePowerGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[partyindex].vitalpercent);
			m_pPartyMemberForcePowerGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[partyindex].zenpercent);
			m_bIsCurrentUser = isCurrentUser = FALSE;
		}
		else
		{			
			m_pPartyMemberLifePowerGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[partyindex].vitalpercent);
			m_pPartyMemberForcePowerGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[partyindex].zenpercent);
			m_bIsCurrentUser = isCurrentUser = TRUE;
		}
	}
}

void _XWindow_PartyMemberMiniBar::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_PartyMemberMiniBar::ProcessDblClick()
{	
	if( !m_bIsCurrentUser )
	{
		_XUser* pUser = NULL;					
		pUser = (_XUser*)g_Userlist.FindUser(m_PlayerUniqueid);
		if( pUser )
		{
			if(!g_pLocalUser->m_bUsingSkill)
			{
				g_pLocalUser->SetViewTarget((_XItem*)pUser, _XPICKINGTARGET_PC);
				g_pLocalUser->SetAttackTarget((_XItem*)pUser, _XPICKINGTARGET_PC);
				
				FLOAT range = g_pLocalUser->m_AttackRange + 1.0f;
				FLOAT dx = g_pLocalUser->m_Position.x - pUser->m_Position.x;
				FLOAT dz = g_pLocalUser->m_Position.z - pUser->m_Position.z;
				FLOAT distance = _XFC_sqrt(dx*dx + dz*dz);
				
				if(distance > range)
				{
					// PC쪽으로 이동
					D3DXVECTOR3 directionvector = g_pLocalUser->m_Position - pUser->m_Position;
					D3DXVec3Normalize(&directionvector, &directionvector);
					
					D3DXVECTOR3 targetposition = pUser->m_Position + (directionvector*(range*0.7f));
					targetposition.y = pUser->m_Position.y;
					g_pLocalUser->SetTargetPositionAngle(targetposition);
					g_pLocalUser->InsertTargetPositionEffect();
				}
				else
				{
					g_pLocalUser->m_RotateAngle = _XMeshMath_GetRotateRadian(g_pLocalUser->m_Position, pUser->m_Position);
					g_pLocalUser->UpdatePosition();
					
					if(pUser->GetMotionClass() == _XACTION_DYING)
						g_pLocalUser->ProcessRescueUser();
				}
			}
		}
	}
}


void _XWindow_PartyMemberMiniBar::SetZoneIndex( int zoneindex )
{	
	m_ZoneIndex = zoneindex;
	
	if( m_ZoneIndex == g_CurrentZoneInfoIndex)
	{
		m_bIsCurrentZone = TRUE;
	}
	else
	{
		m_bIsCurrentZone = FALSE;
		m_bDangerous = FALSE;
	}	
}

void _XWindow_PartyMemberMiniBar::SetPartyUserStateImage(void)
{
#ifdef _XTS_TARGET_STATE
	m_StateCount = 0;

	if(g_pLocalUser->m_PartyInfo[m_PartyIndex].playername[0] != 0)
	{
		if(g_pLocalUser->m_PartyInfo[m_PartyIndex].serverno == g_pLocalUser->m_CharacterInfo.zoneuniqid)
		{
			_XUser* pUser = g_Userlist.FindUser(g_pLocalUser->m_PartyInfo[m_PartyIndex].playeruniqueid);
			if(pUser)
			{
				int imageindex = 0;
				RECT rect;
				int i = 0;	
				
				m_StateCount = pUser->m_StateList.m_mapCharacterStateList.size();
				if( m_StateCount > 16 )
					m_StateCount = 16;
				
				map <int, _XCharacterState*>::iterator iter_state;
				for(iter_state = pUser->m_StateList.m_mapCharacterStateList.begin() ; iter_state != pUser->m_StateList.m_mapCharacterStateList.end() ; iter_state++)
				{
					if( i > 16 ) break;
					
					_XCharacterState* pState = iter_state->second;
					
					if(pState)
					{
						if( pState->m_FromType == 0 ) // spell
						{
							if(g_pQuickSlot_Window)
								g_pQuickSlot_Window->GetIconRect( pState->m_sSkillID, imageindex, rect, true, true);
							
							m_StateImage[i].SetTexture(&g_MainInterfaceTextureArchive, imageindex);
							m_StateImage[i].SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
							m_StateImage[i].SetScale(0.55f, 0.55f);
							i++;
						}
						else if( pState->m_FromType == 1 ) // effect
						{
							_XWindow_State* pState_Window = (_XWindow_State*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATEWINDOW);
							if(pState_Window)
								pState_Window->GetIconRect(pState->m_sEffectID, imageindex, rect, true);
							
							m_StateImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
							m_StateImage[i].SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
							m_StateImage[i].SetScale(0.55f, 0.55f);
							i++;
						}
					}
				}
			}
		}
	}
#endif
}

void _XWindow_PartyMemberMiniBar::SetOut(bool state)
{
#ifdef _XDEF_PARTY_NOTAVAILABLE
	if(state == m_bOut)
		return;

	m_bOut = state;

	if(state)
	{
		m_pOutActionIcon->SetClipRect(180, 36, 191, 47);
	}
	else
	{
		m_pOutActionIcon->SetClipRect(192, 36, 203, 47);
	}
#endif
}

void _XWindow_PartyMemberMiniBar::SetFarAway(bool state)
{
#ifdef _XDEF_PARTY_NOTAVAILABLE
	if(state == m_bFarAway)
		return;

	m_bFarAway = state;

	if(state)
	{
		m_pFarActionIcon->SetClipRect(168, 36, 179, 47);
	}
	else
	{
		m_pFarActionIcon->SetClipRect(192, 36, 203, 47);
	}
#endif
}

void _XWindow_PartyMemberMiniBar::Reset()
{
#ifdef _XDEF_PARTY_NOTAVAILABLE
	// 거리이탈, 자리비움 초기화
	m_bOut = m_bFarAway = false;
	m_pOutActionIcon->SetClipRect(192, 36, 203, 47);
	m_pFarActionIcon->SetClipRect(192, 36, 203, 47);
#endif
}

void _XWindow_PartyMemberMiniBar::SetClassIcon( void )
{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	
	int classid = g_pLocalUser->m_PartyInfo[m_PartyIndex].clanclass;		

	if( g_pLocalUser->m_PartyInfo[m_PartyIndex].clan == _XGROUP_BEEGOONG)
	{
		if(classid == 7)	// 섭혼녀
			classid = 4;
	}
	else if( g_pLocalUser->m_PartyInfo[m_PartyIndex].clan == _XGROUP_NOCKRIM)
	{
		if(classid == 8)	// 철갑귀
			classid = 4;
	}
	
	if( m_pClassIcon )
		m_pClassIcon->SetClipRect(2+32*classid, 2, 32+32*classid, 32);

#endif
}
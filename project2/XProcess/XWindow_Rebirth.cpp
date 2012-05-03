// XWindow_Rebirth.cpp: implementation of the _XWindow_Rebirth class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_Rebirth.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Rebirth::_XWindow_Rebirth()
{
	m_TownButton = NULL;
	m_FieldButton = NULL;
	m_UseItemButton = NULL;
//	m_InCBButton =  NULL;

	m_RebirthType = _XREBIRTH_NONE;
	m_bCastleBattle = FALSE;
#ifdef _ACCLAIM_IGAADSYSTEM
	m_ShowIGAADButton = FALSE;
	m_ButtonShowTimer = 0;
#endif

	memset(m_nButtonType, 0, sizeof(m_nButtonType));
}

_XWindow_Rebirth::~_XWindow_Rebirth()
{
	DestroyWindow();
}

BOOL _XWindow_Rebirth::Initialize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_death01.tga");

	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create(0, 0, 212, 18, &g_MainInterfaceTextureArchive, resourceindex);
	pTitleLeftBar->SetClipRect(0, 0, 212, 18);
	InsertChildObject(pTitleLeftBar);

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName);	
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, 
		{5, 20},
		{202, 22},
		_XDEF_REBIRTHWINDOW_TOWN,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};
	m_TownButton = new _XButton;
	m_TownButton->Create(btnstruct);
	m_TownButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_TownButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_TownButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_TownButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 233, 231, 232));
	m_TownButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REBIRTH_TOWN);	// "마을에서 소생합니다."
	InsertChildObject(m_TownButton);
	
	btnstruct.position.y = 43;
	btnstruct.commandid = _XDEF_REBIRTHWINDOW_SAVEPOINT;
	m_SavePointButton = new _XButton;
	m_SavePointButton->Create(btnstruct);
	m_SavePointButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_SavePointButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_SavePointButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_SavePointButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 233, 231, 232));
	InsertChildObject(m_SavePointButton);

	btnstruct.position.y = 66;
	btnstruct.commandid = _XDEF_REBIRTHWINDOW_FIELD;
	m_FieldButton = new _XButton;
	m_FieldButton->Create(btnstruct);
	m_FieldButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_FieldButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_FieldButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_FieldButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 233, 231, 232));
	m_FieldButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REBIRTH_THIS);	// "이 자리에서 소생합니다."
	InsertChildObject(m_FieldButton);
	
	btnstruct.position.y = 89;
	btnstruct.commandid = _XDEF_REBIRTHWINDOW_USEITEM;
	m_UseItemButton = new _XButton;
	m_UseItemButton->Create(btnstruct);
	m_UseItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_UseItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_UseItemButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_UseItemButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 204, 187, 51));
	m_UseItemButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REBIRTH_2571);
	m_UseItemButton->ShowWindow(FALSE);
	InsertChildObject(m_UseItemButton);

/*	btnstruct.position.y = 112;
	btnstruct.commandid = _XDEF_REBIRTHWINDOW_INCB;
	m_InCBButton = new _XButton;
	m_InCBButton->Create(btnstruct);
	m_InCBButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_InCBButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_InCBButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_InCBButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 233, 231, 232));
//	m_InCBButton->SetButtonText("게임을 종료합니다.");
	InsertChildObject(m_InCBButton);*/

#ifdef _ACCLAIM_IGAADSYSTEM
	btnstruct.position.y = 135;
	btnstruct.commandid = _XDEF_REBIRTHWINDOW_ACCLAIMVIDEOAD;
	m_AcclaimAdVideoButton = new _XButton;
	m_AcclaimAdVideoButton->Create(btnstruct);
	m_AcclaimAdVideoButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_AcclaimAdVideoButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_AcclaimAdVideoButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);	
	m_AcclaimAdVideoButton->SetButtonText( _T("WATCH & WIN: 15 sec video ad") );
	m_AcclaimAdVideoButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 204, 187, 51));	
	m_AcclaimAdVideoButton->SetButtonTextHighlightColor( 0xFFFFFF00 );
	m_AcclaimAdVideoButton->SetGrowTextMode( FALSE );
	m_AcclaimAdVideoButton->ShowWindow(FALSE);
	InsertChildObject(m_AcclaimAdVideoButton);
#endif
	
	for(int i = 0 ; i < 2 ; i++)
	{
		m_CornerImage[i].Create(0, 0, 3, 3, &g_MainInterfaceTextureArchive, resourceindex);
	}
	m_CornerImage[0].SetClipRect(213, 0, 216, 3);
	m_CornerImage[1].SetClipRect(217, 0, 220, 3);

	return TRUE;
}

void _XWindow_Rebirth::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_Rebirth::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		// 왼쪽 세로 
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+0), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+0), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+1), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+1), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));

		// 오른쪽 세로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+209), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+209), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+210), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+210), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+211), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+211), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));

		// 아래 가로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+4), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-3), (FLOAT)(m_WindowPosition.x+208), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-3), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+4), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-2), (FLOAT)(m_WindowPosition.x+208), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-2), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+4), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-1), (FLOAT)(m_WindowPosition.x+208), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-1), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));

		m_CornerImage[0].Draw(m_WindowPosition.x+1, m_WindowPosition.y+m_WindowSize.cy-3);
		m_CornerImage[1].Draw(m_WindowPosition.x+208, m_WindowPosition.y+m_WindowSize.cy-3);
	}

	_XWindow::Draw(pfocusobject);
}

BOOL _XWindow_Rebirth::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return FALSE;

	if(!this->m_Enable)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
#ifdef _ACCLAIM_IGAADSYSTEM

	if( m_AcclaimAdVideoButton->GetShowStatus() ) 
	{
		if( m_ButtonShowTimer == 0 )
			m_ButtonShowTimer = g_LocalSystemTime;

		if( g_LocalSystemTime - m_ButtonShowTimer > 3000 )
		{
			m_ShowIGAADButton = FALSE;
			ShowWindow( TRUE );			
			m_AcclaimAdVideoButton->ShowWindow( FALSE );

			XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
			pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
			pMainGameProcess->m_AcclaimADMode_DeathPhase = _XACCLAIMAD_DEATHPHASE_NONE;			

			if( g_CurrentFocusedObject == (_XGUIObject*)m_AcclaimAdVideoButton )
			{
				g_CurrentFocusedObject = NULL;
			}
		}
		else
		{
			TCHAR buttontext[64];
			sprintf( buttontext, _T("WATCH & WIN: 15 sec video ad : %d"), 3 - ((g_LocalSystemTime - m_ButtonShowTimer) / 1000) );
			m_AcclaimAdVideoButton->SetButtonText( buttontext );
		}
	}
#endif

	return TRUE;
}

#ifdef ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI

void _XWindow_Rebirth::ShowWindow(BOOL show)
{
	this->EnableWindow(TRUE);

	SetButton();

	if(show)
	{
#ifdef _ACCLAIM_IGAADSYSTEM
		m_VideoADCanceled = TRUE;
		m_ButtonShowTimer = 0;
#endif
		//Author : 양희왕 //breif : 창닫기
		if(g_pInventory_Window)
			g_pInventory_Window->ShowWindow(FALSE);
	}
	else
	{
		m_RebirthType = _XREBIRTH_NONE;
		m_bDeadInPK = FALSE;
		m_bDeadInCB = FALSE;
		m_bEnablePKTracing = FALSE;
		m_bCastleBattle = FALSE;

		memset(m_nButtonType, 0, sizeof(m_nButtonType));

#ifdef _ACCLAIM_IGAADSYSTEM
		XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
		pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
		pMainGameProcess->m_AcclaimADMode_DeathPhase = _XACCLAIMAD_DEATHPHASE_NONE;		
#endif
	}

	if(m_RebirthType == _XREBIRTH_BINSA)
	{
		if(g_pLocalUser->m_bIsBossBattle && g_pLocalUser->GetUserItemFromSecondType(_XGI_FC_POTION, _XGI_SC_POTION_HWATA))
		{
			g_CurrentFocusedObject = NULL;
			_XWindow::ShowWindow(show);	
		}
	}
	else
	{
		g_CurrentFocusedObject = NULL;
		_XWindow::ShowWindow(show);
	}
}

#else //ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI

void _XWindow_Rebirth::ShowWindow(BOOL show)
{
	BOOL bHaveItem = FALSE;

	if(show)
	{

#ifdef _ACCLAIM_IGAADSYSTEM
		m_VideoADCanceled = TRUE;
		m_ButtonShowTimer = 0;
#endif

#ifdef ALL_RB_REBIRTH_USERITEM_071108_KUKURI
		this->EnableWindow(TRUE);
#endif
		//Author : 양희왕 //breif : 창닫기
		if(g_pInventory_Window)
			g_pInventory_Window->ShowWindow(FALSE);

		m_TownButton->EnableWindow(TRUE);
		m_FieldButton->EnableWindow(TRUE);
		m_SavePointButton->EnableWindow(TRUE);
		m_UseItemButton->EnableWindow(TRUE);

		// 화타신단이 있는지 체크
		if(g_pLocalUser->GetUserItemFromSecondType(_XGI_FC_POTION, _XGI_SC_POTION_HWATA))
		{
			bHaveItem = TRUE;
		}
		if(m_RebirthType == _XREBIRTH_BINSA)
		{
			if(bHaveItem && g_pLocalUser->m_bIsBossBattle)
			{
				// 화타신단을 가지고 있고 생사결에 들어가 있을 경우 
				m_TownButton->ShowWindow(FALSE);
				m_SavePointButton->ShowWindow(FALSE);
				m_FieldButton->ShowWindow(FALSE);

				m_UseItemButton->ShowWindow(TRUE);
				m_UseItemButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 20);

#ifdef _ACCLAIM_IGAADSYSTEM				
				m_AcclaimAdVideoButton->ShowWindow(m_ShowIGAADButton);
				m_AcclaimAdVideoButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 20 + 23);				
				if(m_ShowIGAADButton)
				{					
					SetWindowSize(212, 44+23);
					m_AcclaimAdVideoButton->CenterMouse();
				}
				else
				{
					SetWindowSize(212, 44);
				}
#else
				SetWindowSize(212, 44);
#endif

				g_CurrentFocusedObject = NULL;
				_XWindow::ShowWindow(show);
			}
			else
			{
#ifdef _ACCLAIM_IGAADSYSTEM
				m_AcclaimAdVideoButton->ShowWindow(m_ShowIGAADButton);
				m_AcclaimAdVideoButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 20 + 23);				
				if(m_ShowIGAADButton)
				{
					SetWindowSize(212, 44+23);
					m_AcclaimAdVideoButton->CenterMouse();
				}
				else
				{
					SetWindowSize(212, 44);
				}
#else
				SetWindowSize(212, 44);
#endif
			}
		}
		else if(m_RebirthType == _XREBIRTH_HONSU)
		{
			m_TownButton->ShowWindow(TRUE);
			m_FieldButton->ShowWindow(TRUE);

			if(g_pLocalUser->m_bSaveRespawnPos || m_bCastleBattle)
			{
				m_SavePointButton->ShowWindow(TRUE);
				m_SavePointButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 43);
				m_FieldButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 66);

				if(bHaveItem)
				{
					m_UseItemButton->ShowWindow(TRUE);
					m_UseItemButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 89);

#ifdef _ACCLAIM_IGAADSYSTEM
					m_AcclaimAdVideoButton->ShowWindow(m_ShowIGAADButton);
					m_AcclaimAdVideoButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 89 + 23);					
					if(m_ShowIGAADButton)
					{
						SetWindowSize(212, 116+23);
						m_AcclaimAdVideoButton->CenterMouse();
					}
					else
					{
						SetWindowSize(212, 116);
					}
#else
					SetWindowSize(212, 116);
#endif
				}
				else
				{
					m_UseItemButton->ShowWindow(FALSE);
#ifdef _ACCLAIM_IGAADSYSTEM
					m_AcclaimAdVideoButton->ShowWindow(m_ShowIGAADButton);
					m_AcclaimAdVideoButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 66 + 23);					
					if(m_ShowIGAADButton)
					{
						SetWindowSize(212, 93+23);
						m_AcclaimAdVideoButton->CenterMouse();
					}
					else
					{
						SetWindowSize(212, 93);
					}
#else
					SetWindowSize(212, 93);
#endif
				}

				TCHAR buttontext[256];
				sprintf(buttontext, _XGETINTERFACETEXT(ID_STRING_REBIRTH_WHERE), g_pLocalUser->m_RespawnName);		// "%s에서 소생합니다."
				m_SavePointButton->SetButtonText(buttontext);
			}
			else
			{
				m_SavePointButton->ShowWindow(FALSE);
				m_FieldButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 43);

				if(bHaveItem)
				{
					m_UseItemButton->ShowWindow(TRUE);
					m_UseItemButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 66);
#ifdef _ACCLAIM_IGAADSYSTEM
					m_AcclaimAdVideoButton->ShowWindow(m_ShowIGAADButton);
					m_AcclaimAdVideoButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 66 + 23);
					if(m_ShowIGAADButton)
					{
						SetWindowSize(212, 93+23);
						m_AcclaimAdVideoButton->CenterMouse();
					}
					else
					{
						SetWindowSize(212, 93);
					}
#else
					SetWindowSize(212, 93);
#endif
				}
				else
				{
					m_UseItemButton->ShowWindow(FALSE);
#ifdef _ACCLAIM_IGAADSYSTEM
					m_AcclaimAdVideoButton->ShowWindow(m_ShowIGAADButton);
					m_AcclaimAdVideoButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 43 + 23);
					if(m_ShowIGAADButton)
					{
						SetWindowSize(212, 70+23);
						m_AcclaimAdVideoButton->CenterMouse();
					}
					else
					{
						SetWindowSize(212, 70);
					}
#else
					SetWindowSize(212, 70);
#endif
				}
			}

			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
				m_TownButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REBIRTH_TOWN);	// "마을에서 소생합니다."
			else
				m_TownButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REBIRTH_GROUP);	// "문파에서 소생합니다."
			
			if(g_CurrentZoneInfoIndex == 14)
			{
				// 이벤트 맵
				m_TownButton->EnableWindow(FALSE);
				m_SavePointButton->EnableWindow(FALSE);
				m_FieldButton->EnableWindow(TRUE);
				m_UseItemButton->EnableWindow(FALSE);
			}
			else
			{
				m_TownButton->EnableWindow(TRUE);
				m_SavePointButton->EnableWindow(TRUE);
				m_FieldButton->EnableWindow(TRUE);
				m_UseItemButton->EnableWindow(TRUE);
			}
			
#ifdef _XTS_PK
			if(g_pLocalUser->m_BattleMode == _XBATTLE_PK || m_bDeadInPK)
			{
				// pk중에는 제자리 부활 금지
				m_FieldButton->EnableWindow(FALSE);
			}
#endif
			if(m_bCastleBattle)
			{
				// 장원전 중에는 화타신단 외 나머지 불가
				if(g_pLocalUser->m_dwDeadPenaltyTime > 0)
				{
					m_TownButton->EnableWindow(FALSE);
					m_FieldButton->EnableWindow(FALSE);
					m_SavePointButton->EnableWindow(FALSE);
				}
				else
				{
					m_TownButton->EnableWindow(TRUE);
					m_SavePointButton->EnableWindow(TRUE);
				}
				m_UseItemButton->EnableWindow(TRUE);

				// SavePoint->장원 안/밖으로 수정
//				m_SavePointButton->ShowWindow(TRUE);

				TCHAR buttontext[256];
				if(g_pLocalUser->m_dwAttackType == 1)				// attacker	
					sprintf(buttontext, _XGETINTERFACETEXT(ID_STRING_NEW_3373));//"공격 진지에서 소생합니다."
				else if(g_pLocalUser->m_dwAttackType == 2)			// defender
					sprintf(buttontext, _XGETINTERFACETEXT(ID_STRING_NEW_3374));//"장원 안에서 소생합니다."

				m_SavePointButton->SetButtonText(buttontext);
			}

			g_CurrentFocusedObject = NULL;
			_XWindow::ShowWindow(show);
		}


#ifdef _ACCLAIM_IGAADSYSTEM // 광고화면을 위해 화면중앙으로 위치
		//MoveWindow( (gnWidth>>1)-106, (gnHeight>>1)-70 );
		/*
		XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
		pMainGameProcess->m_AcclaimADMode_DeathPhase	= _XACCLAIMAD_DEATHPHASE_STATICAD;
		pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_DEATH;		
		*/
#endif

	}
	else
	{
		g_CurrentFocusedObject = NULL;
		m_RebirthType = _XREBIRTH_NONE;
		_XWindow::ShowWindow(show);
		
		m_bDeadInPK = FALSE;
		m_bDeadInCB = FALSE;
		m_bEnablePKTracing = FALSE;
		SetCastleBattleState(FALSE);

		memset(m_nButtonType, 0, sizeof(m_nButtonType));

#ifdef _ACCLAIM_IGAADSYSTEM
		XProc_MainGame* pMainGameProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
		pMainGameProcess->m_AcclaimADMode_MainGame = _XACCLAIMAD_MAINGAME_NONE;
		pMainGameProcess->m_AcclaimADMode_DeathPhase = _XACCLAIMAD_DEATHPHASE_NONE;		
#endif

	}
}

#endif //ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI

void _XWindow_Rebirth::SetWindowSize(int width, int height)
{
	m_WindowSize.cx = width;
	m_WindowSize.cy = height;

	m_solidborderstatic.MoveWindow( m_WindowPosition.x, m_WindowPosition.y, 
		m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy );
}

void _XWindow_Rebirth::SetCastleBattleState(BOOL battle)
{

#ifdef ALL_RB_REBIRTHWINDOW_RENEWAL_071109_KUKURI

	m_bCastleBattle = battle;
	SetButton();

#else
	if(m_ShowWindow)
	{
		if(!battle)
		{
			m_TownButton->EnableWindow(TRUE);
			m_FieldButton->EnableWindow(TRUE);
			m_SavePointButton->EnableWindow(TRUE);
			m_UseItemButton->EnableWindow(TRUE);
		}
		else
		{
			if(m_ShowWindow)
			{
				m_TownButton->EnableWindow(FALSE);
				m_FieldButton->EnableWindow(FALSE);
				m_SavePointButton->EnableWindow(FALSE);
			}
		}
	}

	m_bCastleBattle = battle;
#endif
}

void _XWindow_Rebirth::SetButton()
{
	m_nTotalButtonCount = 0;

	if(m_RebirthType == _XREBIRTH_BINSA)
	{
		if(g_pLocalUser->m_bIsBossBattle && g_pLocalUser->GetUserItemFromSecondType(_XGI_FC_POTION, _XGI_SC_POTION_HWATA))
		{
			// 보스전에서 화타신단을 가지고 있을 때만 유효
			m_TownButton->ShowWindow(FALSE);
			m_SavePointButton->ShowWindow(FALSE);
			m_FieldButton->ShowWindow(FALSE);

			m_UseItemButton->ShowWindow(TRUE);
			m_UseItemButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + 20);
			m_nTotalButtonCount++;

        #ifdef _ACCLAIM_IGAADSYSTEM	
			if(m_ShowIGAADButton)
			{
				m_AcclaimAdVideoButton->ShowWindow(TRUE);
				m_AcclaimAdVideoButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y+20+23);
				m_AcclaimAdVideoButton->CenterMouse();
			}
        #endif

			SetWindowSize(212, 44+(m_nTotalButtonCount-1)*23);
		}
	}
	else if(m_RebirthType == _XREBIRTH_HONSU)
	{
		// 0.
		if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
			m_nButtonType[0] = 0;		// 마을
		else
			m_nButtonType[0] = 1;		// 문파

		m_nTotalButtonCount++;

		// 1.
		if(g_pLocalUser->m_bCastleBattle && m_bDeadInCB)
		{
			if(g_pLocalUser->m_dwAttackType == 1)
			{
				m_nButtonType[1] = 0;	// 공성
				m_nTotalButtonCount++;
			}
			else
			{
				m_nButtonType[1] = 1;	// 수성
				m_nTotalButtonCount++;
			}
		}
		else
		{
			if(g_pLocalUser->m_bSaveRespawnPos)
			{
				m_nButtonType[1] = 2;	// 여점 있음
				m_nTotalButtonCount++;
			}
			else
			{
				m_nButtonType[1] = 3;	// 여점 없음
			}

            // 2. 필드 버튼인데 장원전에서 죽으면 필드 버튼은 필요없다.
		    if(g_pLocalUser->m_BattleMode == _XBATTLE_PK || m_bDeadInPK)
		    {
			    m_nButtonType[2] = 0;		// pk로 죽음
			    m_nTotalButtonCount++;
		    }
		    else
		    {
			    m_nButtonType[2] = 1;		// monster에게 죽음
			    m_nTotalButtonCount++;
		    }
		}

		// 3.
		if(g_pLocalUser->GetUserItemFromSecondType(_XGI_FC_POTION, _XGI_SC_POTION_HWATA))
		{
			m_nButtonType[3] = 0;		// 화타신단 있음
			m_nTotalButtonCount++;
		}
		else
		{
			m_nButtonType[3] = 1;		// 화타신단 없음
		}

		// 4.
    #ifdef _ACCLAIM_IGAADSYSTEM
		if(!m_ShowIGAADButton)
		{
			m_nButtonType[4] = 0;		// 광고 없음
		}
		else
		{
			m_nButtonType[4] = 1;		// 광고 있음
			m_nTotalButtonCount++;
		}
    #else
		m_nButtonType[4] = 0;
    #endif

		TCHAR buttontext[256];

		// initialize buttons
		m_TownButton->ShowWindow(TRUE);
		m_SavePointButton->ShowWindow(TRUE);
		m_FieldButton->ShowWindow(TRUE);
		m_UseItemButton->ShowWindow(TRUE);

		m_TownButton->EnableWindow(TRUE);
		m_SavePointButton->EnableWindow(TRUE);
		m_FieldButton->EnableWindow(TRUE);
		m_UseItemButton->EnableWindow(TRUE);

        if( m_bCastleBattle && m_bDeadInCB ) //이게 트루 일 때는 장원전중 죽어서 패널티를 받는 중이다. 그리고 장원전 안에서 죽어야한다.
        {
            m_TownButton->EnableWindow(FALSE);
		    m_SavePointButton->EnableWindow(FALSE);
		    m_FieldButton->EnableWindow(FALSE);
        }

        if( m_bDeadInCB )
        {
            m_FieldButton->ShowWindow(FALSE); //장원에서 죽으면 제자리 부활은 없어진다
        }

    #ifdef _ACCLAIM_IGAADSYSTEM
		m_AcclaimAdVideoButton->ShowWindow(TRUE);
		m_AcclaimAdVideoButton->EnableWindow(TRUE);
    #endif

		// set window size
		SetWindowSize(212, 44+(m_nTotalButtonCount-1)*23);

		// set buttons

		// 0. Town button
		switch(m_nButtonType[0])
		{	
		case 0 :
			{
				m_TownButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REBIRTH_TOWN);	// "마을에서 소생합니다."
			}
			break;
		case 1 :
			{
				m_TownButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_REBIRTH_GROUP);// "문파에서 소생합니다."
			}
			break;
		}

		// 1. Savepoint button
		switch(m_nButtonType[1])
		{
		case 0 :
			{
				m_SavePointButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3373);//"공격 진지에서 소생합니다."
			}
			break;
		case 1 :
			{
				m_SavePointButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3374);//"장원 안에서 소생합니다."
			}
			break;
		case 2 :
			{
				sprintf(buttontext, _XGETINTERFACETEXT(ID_STRING_REBIRTH_WHERE), g_pLocalUser->m_RespawnName);		// "%s에서 소생합니다."
				m_SavePointButton->SetButtonText(buttontext);
			}
			break;
		case 3 :
			{
				m_SavePointButton->ShowWindow(FALSE);
			}
			break;
		}

		// 2. Field button
		switch(m_nButtonType[2])
		{
		case 0 :
			{
				m_FieldButton->EnableWindow(FALSE);
			}
			break;
		case 1 :
			{
                if( !m_bDeadInCB ) //장원전에서 죽으면 제자리 부활이 없다.
                    m_FieldButton->EnableWindow(TRUE);
			}
			break;
		}

		// 3. Useritem button
		switch(m_nButtonType[3])
		{
		case 0 :
			{
				m_UseItemButton->ShowWindow(TRUE);
			}
			break;
		case 1 :
			{
				m_UseItemButton->ShowWindow(FALSE);
			}
			break;
		}

    #ifdef _ACCLAIM_IGAADSYSTEM
		// 4. Ad button
		switch(m_nButtonType[4])
		{
		case 0 :
			{
				m_AcclaimAdVideoButton->ShowWindow(FALSE);
			}
			break;
		case 1 :
			{
				m_AcclaimAdVideoButton->ShowWindow(TRUE);
				m_AcclaimAdVideoButton->CenterMouse();
			}
			break;
		}
    #endif

        //Author : 양희왕 //breif : 위치 버그 보스전 들어갔다가 나오면 화타신단이 젤 위로 올라가버림 //우선 기본자리 //여점 부활이 없을경우 하나씩 당겨짐
        int nBtnPositionY = 20;
        if( m_TownButton->GetShowStatus() )
            m_TownButton->MoveWindow(m_WindowPosition.x + 5, m_WindowPosition.y + nBtnPositionY);

        if( m_SavePointButton->GetShowStatus() )
        {
            nBtnPositionY += 23;
            m_SavePointButton->MoveWindow(m_WindowPosition.x+5, m_WindowPosition.y+nBtnPositionY);
        }

        if( m_FieldButton->GetShowStatus() )
        {
            nBtnPositionY += 23;
            m_FieldButton->MoveWindow(m_WindowPosition.x+5, m_WindowPosition.y+nBtnPositionY);
        }

        if( m_UseItemButton->GetShowStatus() )
        {
            nBtnPositionY += 23;
            m_UseItemButton->MoveWindow(m_WindowPosition.x+5, m_WindowPosition.y+nBtnPositionY);
        }
		
    #ifdef _ACCLAIM_IGAADSYSTEM
        if( m_AcclaimAdVideoButton->GetShowStatus() )
        {
            nBtnPositionY += 23;
            m_AcclaimAdVideoButton->MoveWindow(m_WindowPosition.x+5, m_WindowPosition.y+nBtnPositionY);
        }
    #endif
	}
}
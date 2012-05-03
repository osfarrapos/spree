// XWindow_ChargeGauge.cpp: implementation of the _XWindow_ChargeGauge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_RepairItem.h"
#include "XWindow_SocketPlugIn.h"
#include "XWindow_Gamble.h"
#include "XWindow_ChargeGauge.h"
#include "XWindow_SmeltItem.h"
#include "XWindow_SocketDetach.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ChargeGauge::_XWindow_ChargeGauge()
{
	m_DefaultGaugeBorder	= NULL;
	m_ReadBookGaugeBorder	= NULL;
	m_ChargeGaugeBorder		= NULL;
	m_GaugePoint			= NULL;
	m_ThreatenGaugeBorder	= NULL;
	m_CurrentGaugeMode		= _XGAUGE_NONE;
	m_iGaugePointAlpha		= 0;
	m_bGaugePointAlphaFlag	= FALSE;
	m_bGaugeFadeOutMode		= FALSE;
	m_dwGaugeDelayTime		= 0;	
	m_dwElapsedTime			= 0;
	m_iSelectedNumber		= 0;	
	m_DrawPos.x = m_DrawPos.y = 0;
	m_FadeOutElapsedTime	= 0;
	m_FadeOutAlpha			= 0;
	m_bReadySandMsg			= FALSE;
	m_bGaugeStop			= FALSE;
}

_XWindow_ChargeGauge::~_XWindow_ChargeGauge()
{

}

BOOL _XWindow_ChargeGauge::Initailize(void)
{
	SetDrawBorder( FALSE );

	int gaugeimageindex = g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");	
	// default gauge------------------------------------------------------------------------=
	_XSTICKGAUGE_STRUCTURE defaultgaugestruct =
	{
		TRUE, {0, 0}, {159, 13},
		gaugeimageindex, gaugeimageindex, &g_MainInterfaceTextureArchive
	};
	
	m_DefaultGaugeBorder = new _XStickGauge;
	m_DefaultGaugeBorder->Create(defaultgaugestruct);
	m_DefaultGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_OVERLAY, 4, 230, 163, 243);
	m_DefaultGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_BORDER, 4, 243, 163, 256);	
	m_DefaultGaugeBorder->SetFactorDraw(FALSE);
	m_DefaultGaugeBorder->SetBackGroundDraw(TRUE);
	m_DefaultGaugeBorder->SetGaugeFactor( 0.0f );
	m_DefaultGaugeBorder->SetGaugeColorFactor( 0xFFFFFFFF, 0x00000000 );
	m_DefaultGaugeBorder->SetViewDelayGauge( FALSE );
	m_DefaultGaugeBorder->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_RESIZE);
	InsertChildObject(m_DefaultGaugeBorder);	
	// -------------------------------------------------------------------------------------=

	gaugeimageindex = g_MainInterfaceTextureArchive.FindResource("MI_npc_present.tga");

	// readbook gauge-----------------------------------------------------------------------=
	_XSTICKGAUGE_STRUCTURE readgaugestruct =
	{
		TRUE, {0, 0}, {154, 16},
		gaugeimageindex, gaugeimageindex, &g_MainInterfaceTextureArchive
	};
	
	m_ReadBookGaugeBorder = new _XStickGauge;
	m_ReadBookGaugeBorder->Create(readgaugestruct);	
	m_ReadBookGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_OVERLAY, 2, 149, 156, 165);
	m_ReadBookGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_BORDER, 2, 167, 156, 183);
	m_ReadBookGaugeBorder->SetFactorDraw(FALSE);
	m_ReadBookGaugeBorder->SetBackGroundDraw(TRUE);
	m_ReadBookGaugeBorder->SetGaugeFactor( 0.0f );
	m_ReadBookGaugeBorder->SetGaugeColorFactor( 0xFFFFFFFF, 0x00000000 );
	m_ReadBookGaugeBorder->SetViewDelayGauge( FALSE );
	m_ReadBookGaugeBorder->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_RESIZE);
	InsertChildObject(m_ReadBookGaugeBorder);
	// ------------------------------------------------------------------------------------=
	
	// charge gauge------------------------------------------------------------------------=
	_XSTICKGAUGE_STRUCTURE gaugestruct =
	{
		TRUE, {0, 0}, {203, 27},
		gaugeimageindex, gaugeimageindex, &g_MainInterfaceTextureArchive
	};
	
	m_ChargeGaugeBorder = new _XStickGauge;
	m_ChargeGaugeBorder->Create(gaugestruct);	
	m_ChargeGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_OVERLAY, 2, 202, 205, 229);
	m_ChargeGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_BORDER, 2, 229, 205, 256);
	m_ChargeGaugeBorder->SetFactorDraw(FALSE);
	m_ChargeGaugeBorder->SetBackGroundDraw(TRUE);
	m_ChargeGaugeBorder->SetGaugeFactor( 0.0f );
	m_ChargeGaugeBorder->SetGaugeColorFactor( 0xFFFFFFFF, 0x00000000 );
	m_ChargeGaugeBorder->SetViewDelayGauge( FALSE );
	m_ChargeGaugeBorder->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_RESIZE);
	InsertChildObject(m_ChargeGaugeBorder);	
	
	// -------------------------------------------------------------------------------------=

	// -------------------------------------------------------------------------------------=
	m_GaugePoint = new _XImageStatic;
	m_GaugePoint->Create( 0, 0, 11, 13, &g_MainInterfaceTextureArchive, gaugeimageindex );
	m_GaugePoint->SetClipRect(  166, 233, 176, 245 );
	m_GaugePoint->ShowWindow( FALSE );	// 임시적 제한
	InsertChildObject( m_GaugePoint );	
	// -------------------------------------------------------------------------------------=

	// threaten gauge------------------------------------------------------------------------=
	_XSTICKGAUGE_STRUCTURE thretengaugestruct =
	{
		TRUE, {0, 0}, {188, 18},
		g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga"),
		g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga"),
		&g_MainInterfaceTextureArchive
	};
	
	m_ThreatenGaugeBorder = new _XStickGauge;
	m_ThreatenGaugeBorder->Create(thretengaugestruct);	
	m_ThreatenGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_OVERLAY, 1, 49, 189, 67);
	m_ThreatenGaugeBorder->SetImageClipRect(_XGAUGEIMAGE_BORDER, 1, 31, 189, 49);
	m_ThreatenGaugeBorder->SetFactorDraw(TRUE);
	m_ThreatenGaugeBorder->SetBackGroundDraw(TRUE);
	m_ThreatenGaugeBorder->SetGaugeFactor( 0.0f );
	m_ThreatenGaugeBorder->SetGaugeColorFactor( 0xFFFFFFFF, 0x00000000 );
	m_ThreatenGaugeBorder->SetViewDelayGauge( FALSE );
	m_ThreatenGaugeBorder->SetGaugeImageDrawType(_XGAUGEIMAGEDRAWTYPE_RESIZE);
	InsertChildObject(m_ThreatenGaugeBorder);	
	
	// -------------------------------------------------------------------------------------=

	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_ChargeGauge::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_ChargeGauge::Draw(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return;

	_XWindow::Draw(pfocusobject);
	
	switch(m_CurrentGaugeMode) 
	{
	case _XGAUGE_CHARGE :
		{
			if( g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_CHARGESKILL )
			{
				if( g_pLocalUser->m_ChargeIterateValue >= 0 )
				{
					TCHAR tempstr[128];
					sprintf( tempstr, "%d", g_pLocalUser->m_ChargeIterateValue+1 );				
					g_XBaseFont->SetColor(0xFF000000);
					g_XBaseFont->PutsAlign( m_WindowPosition.x+101, m_WindowPosition.y+11, _XFONT_ALIGNTYPE_CENTER, tempstr );
					g_XBaseFont->PutsAlign( m_WindowPosition.x+103, m_WindowPosition.y+11, _XFONT_ALIGNTYPE_CENTER, tempstr );
					
					g_XBaseFont->SetColor(0xFFE0E0E0);
					g_XBaseFont->PutsAlign( m_WindowPosition.x+102, m_WindowPosition.y+11, _XFONT_ALIGNTYPE_CENTER, tempstr );
					
					g_XBaseFont->Flush();
				}
			}			
		}
		break;		
	case _XGAUGE_READ :
		{
			if( !m_bGaugeFadeOutMode )
			{
				g_XBaseFont->SetColor( ((m_FadeOutAlpha << 24) | 0xFFFFFF) );			
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+77, m_WindowPosition.y-16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CHARGEGAUGE_BOOK) ); // 구결을 습득 중입니다...
				g_XBaseFont->Flush();
			}			
		}
		break;
	case _XGAUGE_REPAIR :
		{
			if( !m_bGaugeFadeOutMode )
			{
				g_XBaseFont->SetColor( ((m_FadeOutAlpha << 24) | 0xFFFFFF) );
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+79, m_WindowPosition.y-16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CHARGEGAUGE_ITEM) ); // 물품을 수리 중입니다...
				g_XBaseFont->Flush();
			}
		}
		break;
	case _XGAUGE_SOCKET :
		{
			if( !m_bGaugeFadeOutMode )
			{
				g_XBaseFont->SetColor( ((m_FadeOutAlpha << 24) | 0xFFFFFF) );
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+79, m_WindowPosition.y-16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CHARGEGAUGE_SOCKET) ); // 장식물을 다는 중입니다...
				g_XBaseFont->Flush();
			}
		}
		break;
	case _XGAUGE_SOCKETDEATCH : //Author : 양희왕 // 소켓제거
		{
			if( !m_bGaugeFadeOutMode )
			{
				g_XBaseFont->SetColor( ((m_FadeOutAlpha << 24) | 0xFFFFFF) );
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+79, m_WindowPosition.y-16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3745) ); // 장식물을 제거 하는 중입니다...
				g_XBaseFont->Flush();
			}
		}
		break;
	case _XGAUGE_ELIXIR :
		{
			if( !m_bGaugeFadeOutMode )
			{
				g_XBaseFont->SetColor( ((m_FadeOutAlpha << 24) | 0xFFFFFF) );
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+102, m_WindowPosition.y-16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CHARGEGAUGE_2472) ); // 영약을 복용 중입니다...
				g_XBaseFont->Flush();
			}
		}
		break;
	case _XGAUGE_GAMBLE :
		{
			if( !m_bGaugeFadeOutMode )
			{
				g_XBaseFont->SetColor( ((m_FadeOutAlpha << 24) | 0xFFFFFF) );
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+79, m_WindowPosition.y-16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CHARGEGAUGE_2473) ); // 상자 여는 중
				g_XBaseFont->Flush();
			}
		}
		break;
/*	case _XGAUGE_THREATEN :
		{
			if(!m_bGaugeFadeOutMode)
			{
				g_XBaseFont->SetColor( ((m_FadeOutAlpha << 24) | 0xFFFFFF) );
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+79, m_WindowPosition.y-16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3254) );	//"장주를 협박하는 중"
				g_XBaseFont->Flush();
			}
		}
		break;*/
	case _XGAUGE_ESCAPE :
		{
			if(!m_bGaugeFadeOutMode)
			{
				g_XBaseFont->SetColor( ((m_FadeOutAlpha << 24) | 0xFFFFFF) );
				g_XBaseFont->PutsAlign(	m_WindowPosition.x+79, m_WindowPosition.y-41, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3244) );	//"탈출 중입니다."
				g_XBaseFont->Flush();
			}
		}
		break;
	}
}

BOOL _XWindow_ChargeGauge::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return FALSE;
	else
	{
		if( m_bGaugeFadeOutMode )
		{
			if( g_LocalSystemTime - m_FadeOutElapsedTime <= 500 )		// 1초
			{
				m_FadeOutAlpha = 255 - static_cast<DWORD> ( (g_LocalSystemTime - m_FadeOutElapsedTime)/500.0f * 255.0f );
				if( m_FadeOutAlpha < 0 )
					m_FadeOutAlpha = 0;
			}

			switch( m_CurrentGaugeMode ) 
			{
			case _XGAUGE_CHARGE :
				{
					if( !m_ChargeGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_ChargeGaugeBorder->ResetFadeOutMode();
					}
				}
				break;
			case _XGAUGE_READ :
				{
					if( !m_ReadBookGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_ReadBookGaugeBorder->ResetFadeOutMode();
					}
				}
				break;
			case _XGAUGE_REPAIR :
				{
					if( !m_DefaultGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_DefaultGaugeBorder->ResetFadeOutMode();
						
						_XWindow_RepairItem* pRepairItem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM );
						pRepairItem_Window->m_RepairMode = RM_ENDREPAIR;						
					}										
				}
				break;
			case _XGAUGE_SOCKET :
				{
					if( !m_ReadBookGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_ReadBookGaugeBorder->ResetFadeOutMode();
					}
				}
				break;
			case _XGAUGE_SOCKETDEATCH : //Author : 양희왕
				{
					if( !m_ReadBookGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_ReadBookGaugeBorder->ResetFadeOutMode();
					}
				}
				break;
			case _XGAUGE_BINSA :
				{
					if( !m_ReadBookGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_ReadBookGaugeBorder->ResetFadeOutMode();
					}
				}
				break;
			case _XGAUGE_ELIXIR :
				{
					if( !m_ChargeGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_ChargeGaugeBorder->ResetFadeOutMode();
					}
				}
				break;
			case _XGAUGE_GAMBLE :
				{
					if( !m_ReadBookGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_ReadBookGaugeBorder->ResetFadeOutMode();
					}
				}
				break;
			case _XGAUGE_INCHANT :
				{
					if( !m_DefaultGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_DefaultGaugeBorder->ResetFadeOutMode();
					}										
				}
				break;
/*			case _XGAUGE_THREATEN :
				{
					if(!m_ThreatenGaugeBorder->GetFadeMode())
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_ThreatenGaugeBorder->ResetFadeOutMode();
					}
				}
				break;*/
			case _XGAUGE_ESCAPE :
				{
					if(!m_DefaultGaugeBorder->GetFadeMode())
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_DefaultGaugeBorder->ResetFadeOutMode();
					}
				}
				break;
			case _XGAUGE_SMELTITEM : //Author : 양희왕
				{
					if( !m_DefaultGaugeBorder->GetFadeMode() )
					{
						m_bGaugeFadeOutMode = FALSE;
						ShowWindow(FALSE);
						m_DefaultGaugeBorder->ResetFadeOutMode();
					}										
				}
			}
		}

		if( m_bGaugePointAlphaFlag )
		{
			m_iGaugePointAlpha += 10;
			if( m_iGaugePointAlpha > 255 )
			{
				m_iGaugePointAlpha = 255;
				m_bGaugePointAlphaFlag = FALSE;
			}
		}
		else
		{
			m_iGaugePointAlpha -= 10;
			if( m_iGaugePointAlpha < 66 )
			{
				m_iGaugePointAlpha = 66;
				m_bGaugePointAlphaFlag = TRUE;
			}
		}
		
		m_GaugePoint->m_FColor = ((DWORD)m_iGaugePointAlpha << 24) | 0xffffff;

		int X = 0, Y = 0;
		SetWindowPosition(X, Y);
		MoveWindow(X, Y);
		if( m_CurrentGaugeMode == _XGAUGE_REPAIR )
		{
			_XWindow_RepairItem* pRepairItem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM );
			if( pRepairItem_Window )
			{
				if( pRepairItem_Window->m_RepairMode == RM_UNDERREPAIR )
				{

				}
				else
				{
					g_MainWindowManager.SetTopWindow(this);
				}				
			}
			else
			{
				g_MainWindowManager.SetTopWindow(this);
			}
		}
	}
	
	if(!this->m_Enable)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_ChargeGauge::ShowWindow(BOOL show)
{
	if(show)
	{
		int X = 0, Y = 0;
		m_FadeOutAlpha = 255;
		m_bReadySandMsg = TRUE;
		m_bGaugeStop = FALSE;
		SetWindowPosition(X, Y);
		MoveWindow(X, Y);	
	}
	else
	{
		switch(m_CurrentGaugeMode) 
		{
		case _XGAUGE_ELIXIR :
			{
				if( g_pLocalUser->m_CurrentInternalUseState == _XUSER_STATE_4 )
					g_pLocalUser->m_CurrentInternalUseState = _XUSER_STATE_5;
			}			
			break;
		case _XGAUGE_READ:
			{
				if( g_pLocalUser->m_CurrentReadBookState == _XUSER_STATE_4 )
					g_pLocalUser->m_CurrentReadBookState = _XUSER_STATE_5;
			}			
			break;
		case _XGAUGE_SOCKET :
			g_pLocalUser->m_bSocketPlugIn = FALSE;
			break;
		case _XGAUGE_GAMBLE :
			g_pLocalUser->m_bGambleBoxOpen = FALSE;
		}
		m_CurrentGaugeMode = _XGAUGE_NONE;
	}	

	_XWindow::ShowWindow(show);
}

void _XWindow_ChargeGauge::StartFadeOutMode()
{
	m_bGaugeFadeOutMode = TRUE;	
	m_FadeOutElapsedTime = g_LocalSystemTime;

	switch( m_CurrentGaugeMode ) 
	{
	case _XGAUGE_CHARGE :
	case _XGAUGE_ELIXIR :
		{
			m_ChargeGaugeBorder->SetFadeOutMode( TRUE );
		}
		break;
	case _XGAUGE_READ :
	case _XGAUGE_SOCKET :
	case _XGAUGE_SOCKETDEATCH : //Author : 양희왕
	case _XGAUGE_BINSA :
	case _XGAUGE_GAMBLE :
		{
			m_ReadBookGaugeBorder->SetFadeOutMode( TRUE );
		}
		break;
	case _XGAUGE_REPAIR :
	case _XGAUGE_INCHANT :
	case _XGAUGE_ESCAPE	:
	case _XGAUGE_SMELTITEM :
		{
			m_DefaultGaugeBorder->SetFadeOutMode( TRUE );
		}
		break;
	/*case _XGAUGE_THREATEN :
		{
			m_ThreatenGaugeBorder->SetFadeOutMode(TRUE);
		}
		break;*/
	}
}

void _XWindow_ChargeGauge::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
}

void _XWindow_ChargeGauge::SetWindowPosition(int& X, int& Y)
{
	D3DXVECTOR3 rvec;
	D3DXVECTOR3	position = g_pLocalUser->m_Position;
	
	switch( m_CurrentGaugeMode ) 
	{
	case _XGAUGE_CHARGE :
		{
			position.y = g_pLocalUser->m_Position.y + 1.5f;

			D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
				&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);			
			
			X = (int)(rvec.x - (203 >> 1));
			Y = (int)(rvec.y) - 27;
		}
		break;
	case _XGAUGE_READ :
		{
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );
			}
			else
			{
				if( m_bReadySandMsg )
				{
					g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, m_iSelectedNumber);
					m_bReadySandMsg = FALSE;
				}					
			}
			X = (gnWidth>>1) - 80;
			Y = (gnHeight>>1) - 50;
		}
		break;
	case _XGAUGE_REPAIR :
		{
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );
			}
			else
			{
				if( m_bReadySandMsg )
				{
					g_NetworkKernel.SendPacket(MSG_NO_NPC_ITEM_REPAIR_REQUEST, 1);
					m_bReadySandMsg = FALSE;
				}				
			}
			X = m_DrawPos.x;
			Y = m_DrawPos.y;
		}
		break;
	case _XGAUGE_SOCKET :
		{
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );
			}
			else
			{
				if( m_bReadySandMsg && !m_bGaugeStop )
				{
					g_pLocalUser->m_bSocketPlugIn = FALSE;
					g_pLocalUser->m_bSocketPlugInLock = FALSE;
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 //breif :소켓 제거
					g_NetworkKernel.SendPacket( MSG_NO_ITEM_SOCKET_GAMBLE, en_socket_gamble_mode_equip_socket ); //소켓장착
#else
					g_NetworkKernel.SendPacket(MSG_NO_ITEM_EQUIP_SOCKET);
#endif
					m_bReadySandMsg = FALSE;
				}				
			}
			position.y = g_pLocalUser->m_Position.y + 0.6f;

			D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
				&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);			
			
			X = (int)(rvec.x - (155 >> 1));
			Y = (int)(rvec.y) - 27;
		}
		break;
	case _XGAUGE_SOCKETDEATCH :
		{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 //breif :소켓 제거
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );
			}
			else
			{
				if( m_bReadySandMsg && !m_bGaugeStop )
				{
					g_pLocalUser->m_bSocketPlugIn = FALSE;
					g_pLocalUser->m_bSocketPlugInLock = FALSE;
					g_NetworkKernel.SendPacket( MSG_NO_ITEM_SOCKET_GAMBLE, en_socket_gamble_mode_separate_socket ); //소켓분리 요청
					m_bReadySandMsg = FALSE;
				}				
			}
			position.y = g_pLocalUser->m_Position.y + 0.6f;

			D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
				&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);			
			
			X = (int)(rvec.x - (155 >> 1));
			Y = (int)(rvec.y) - 27;
#endif
		}
		break;
	case _XGAUGE_BINSA :
		{
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = 100.0f - ((g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime);
				SetParameter( percentage );
			}
			else
			{
				if( m_bReadySandMsg )
				{
					g_pLocalUser->m_bReadingBook = FALSE;					
					m_bReadySandMsg = FALSE;
					StartFadeOutMode();
				}				
			}
			
#ifdef ALL_RB_NPCSCRIPT_DEATH_070611_KUKURI
			XProc_MainGame* pmaingameproc = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
			if(pmaingameproc->m_bNPCDialogMode || pmaingameproc->m_bNPCDialogWideView)
			{
				X = (gnWidth>>1) - 92;
				Y = (gnHeight>>1) - 95;
			}
			else
			{
				position.y = g_pLocalUser->m_Position.y + 1.0f;

				D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
					&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);			
				
				X = (int)(rvec.x) - 77;
				Y = (int)(rvec.y) - 27;
			}
#else
			position.y = g_pLocalUser->m_Position.y + 1.0f;

			D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
				&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);			
			
			X = (int)(rvec.x) - 77;
			Y = (int)(rvec.y) - 27;
#endif
		}
		break;
	case _XGAUGE_ELIXIR :
		{
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );
			}
			else
			{
				if( m_bReadySandMsg )
				{
					g_NetworkKernel.SendPacket(MSG_NO_USE_ITEM_REQ, m_iSelectedNumber, 0);					
					m_bReadySandMsg = FALSE;
				}					
			}
			
			position.y = g_pLocalUser->m_Position.y + 1.5f;

			D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
				&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);			
			
			X = (int)(rvec.x - (203 >> 1));
			Y = (int)(rvec.y) - 27;			
		}
		break;
	case _XGAUGE_GAMBLE :
		{
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );
			}
			else
			{
				if( m_bReadySandMsg && !m_bGaugeStop )
				{
					g_pLocalUser->m_bGambleBoxOpen = FALSE;
					g_pLocalUser->m_bGambleBoxOpenLock = FALSE;
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 //breif :소켓 제거
					g_NetworkKernel.SendPacket( MSG_NO_ITEM_SOCKET_GAMBLE, en_socket_gamble_mode_boxgamble ); //겜블
#else
					g_NetworkKernel.SendPacket(MSG_NO_ITEM_EQUIP_SOCKET, 1);
#endif
		
					m_bReadySandMsg = FALSE;
				}				
			}
			position.y = g_pLocalUser->m_Position.y + 0.6f;

			D3DXVec3Project(&rvec, &position, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(),
				&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);			
			
			X = (int)(rvec.x - (155 >> 1));
			Y = (int)(rvec.y) - 27;
		}
		break;
	case _XGAUGE_INCHANT :
		{
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );

				if( m_iSelectedNumber == 1 )	// 첫번째 제련 결과값으로 이용 75
				{
					if( percentage > 75.0f )
					{
						m_dwGaugeDelayTime = 0;						
					}
				}
				else if( m_iSelectedNumber == 2 )	// 50
				{
					if( percentage > 50.0f )
					{
						m_dwGaugeDelayTime = 0;
					}
				}
			}
			else
			{
				if( m_bReadySandMsg )
				{
					g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_str_complete );
					m_bReadySandMsg = FALSE;
				}				
			}
			X = m_DrawPos.x;
			Y = m_DrawPos.y;
		}
		break;
		
	case _XGAUGE_ESCAPE	:
		{
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );
			}
			else
			{
				// 플레이어 이동
			}
			X = m_DrawPos.x;
			Y = m_DrawPos.y;

		}
		break;
	case _XGAUGE_SMELTITEM: //Author : 양희왕
		{
#ifdef _XDEF_SMELT_ITEM
			if( g_LocalSystemTime - m_dwElapsedTime < m_dwGaugeDelayTime )
			{
				FLOAT percentage = (g_LocalSystemTime - m_dwElapsedTime)*100.0f / m_dwGaugeDelayTime;
				SetParameter( percentage );
			}
			else
			{
				XWindow_SmeltItem* pSmeltItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
				if( pSmeltItemWindow )
				{
					switch( pSmeltItemWindow->GetItemFlag() )
					{
					case TEAR :
						if( m_bReadySandMsg )
						{
							g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_protect_inc_str_req );
							m_bReadySandMsg = FALSE;
						}				
						break;
					case SCALES :
						if( m_bReadySandMsg )
						{
							int nTempBuffer = 0;
							int nTempCount = 0;
							int nSlot = 0;
							if( g_pLocalUser->m_UserItemList[pSmeltItemWindow->GetSmeltItemindex()].Get_m_ucCount() < pSmeltItemWindow->GetCount() ) //슬롯에 수량보다 바를 수량이 많은경우
							{
								while( nSlot != -1 )
								{
									//바를 용린 개수 저장
									nTempBuffer = pSmeltItemWindow->GetCount(); 

									//현재 바를 수량 저장//인벤 수량
									nTempCount = g_pLocalUser->m_UserItemList[pSmeltItemWindow->GetSmeltItemindex()].Get_m_ucCount();
									
									if( g_pLocalUser->m_UserItemList[pSmeltItemWindow->GetSmeltItemindex()].Get_m_ucCount() >= pSmeltItemWindow->GetCount() )
									{
										//인벤 수량이 많을 경우 바를 용린 개수를 인벤수량으로 변경하지않음
										//패킷을 보냄
										g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_protect_add_count_req );
										m_bReadySandMsg = FALSE;
										break;
									}
									else
									{
										//슬롯에 있는 수량을 저장
										pSmeltItemWindow->SetCount( g_pLocalUser->m_UserItemList[pSmeltItemWindow->GetSmeltItemindex()].Get_m_ucCount() );
										//패킷을 보냄
										g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_protect_add_count_req );

										//남은 바를 수량을 저장
										pSmeltItemWindow->SetCount( nTempBuffer - nTempCount );

										//새로운 슬롯을 찾음
										nSlot = pSmeltItemWindow->SearchSlotSmeltItem();
										if( nSlot != -1 )
										{
											//새로운 슬롯 번호를 저장
											pSmeltItemWindow->SetSmeltItemindex( nSlot );
										}
										else
										{
											pSmeltItemWindow->MessagePrint(3); //용린이 부족함
											m_bReadySandMsg = FALSE;
											break;
										}
									}
								}
							}
							else
							{
								g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_protect_add_count_req );
								m_bReadySandMsg = FALSE;
							}
						}				
						break;
					}
				}
			}

			X = m_DrawPos.x;
			Y = m_DrawPos.y;
#endif
		}
	/*case _XGAUGE_THREATEN :
		{
			SIZE size = this->GetWindowSize();
			X = (gnWidth>>1) - (size.cx>>1);
			Y = 92;
		}
		break;*/
	}	

	if(X < 0)
		X = 0;
	if(Y < 0)
		Y = 0;
}

void _XWindow_ChargeGauge::SetParameter(FLOAT percentage)
{
	switch( m_CurrentGaugeMode ) 
	{
	case _XGAUGE_CHARGE :
	case _XGAUGE_ELIXIR :
		{
			m_ChargeGaugeBorder->SetGaugeFactor(percentage);
		}
		break;
	case _XGAUGE_READ :
	case _XGAUGE_SOCKET :
	case _XGAUGE_SOCKETDEATCH : //Author : 양희왕
	case _XGAUGE_BINSA :
	case _XGAUGE_GAMBLE :
		{
			m_ReadBookGaugeBorder->SetGaugeFactor(percentage);
		}
		break;
	case _XGAUGE_REPAIR :
	case _XGAUGE_INCHANT :
	case _XGAUGE_ESCAPE	:
	case _XGAUGE_SMELTITEM : //Author : 양희왕
		{
			m_DefaultGaugeBorder->SetGaugeFactor(percentage);
		}
		break;
	/*case _XGAUGE_THREATEN :
		{
			m_ThreatenGaugeBorder->SetGaugeFactor(percentage);
		}
		break;*/
	}
}

void _XWindow_ChargeGauge::SetGaugeMode( _XGAUGE_MODE mode )
{
	m_CurrentGaugeMode = mode;
	switch(m_CurrentGaugeMode) 
	{
	case _XGAUGE_CHARGE :
	case _XGAUGE_ELIXIR :
		{
			m_DefaultGaugeBorder->ShowWindow( FALSE );
			m_ReadBookGaugeBorder->ShowWindow( FALSE );
			m_ChargeGaugeBorder->ShowWindow( TRUE );
			m_ThreatenGaugeBorder->ShowWindow(FALSE);
		}
		break;
	case _XGAUGE_READ:
	case _XGAUGE_SOCKET :
	case _XGAUGE_SOCKETDEATCH : //Author : 양희왕
	case _XGAUGE_GAMBLE :
		{
			m_DefaultGaugeBorder->ShowWindow( FALSE );
			m_ReadBookGaugeBorder->ShowWindow( TRUE );			
			m_ChargeGaugeBorder->ShowWindow( FALSE );
			m_ThreatenGaugeBorder->ShowWindow(FALSE);
		}
		break;
	case _XGAUGE_REPAIR:
	case _XGAUGE_INCHANT:
	case _XGAUGE_ESCAPE	:
	case _XGAUGE_SMELTITEM : // Author : 양희왕
		{
			m_DefaultGaugeBorder->ShowWindow( TRUE );
			m_ReadBookGaugeBorder->ShowWindow( FALSE );
			m_ChargeGaugeBorder->ShowWindow( FALSE );
			m_ThreatenGaugeBorder->ShowWindow(FALSE);
		}
		break;
	case _XGAUGE_BINSA :
		{
			m_DefaultGaugeBorder->ShowWindow( FALSE );
			m_ReadBookGaugeBorder->ShowWindow( TRUE );
			m_ChargeGaugeBorder->ShowWindow( FALSE );
			m_ThreatenGaugeBorder->ShowWindow(FALSE);
		}
		break;
	/*case _XGAUGE_THREATEN :
		{
			m_DefaultGaugeBorder->ShowWindow( FALSE );
			m_ReadBookGaugeBorder->ShowWindow( FALSE );
			m_ChargeGaugeBorder->ShowWindow( FALSE );
			m_ThreatenGaugeBorder->ShowWindow(TRUE);
		}
		break;*/
	}		
}

void _XWindow_ChargeGauge::SetDelayTime( FLOAT time ) 
{ 
	m_dwGaugeDelayTime = time;
	m_dwElapsedTime = g_LocalSystemTime;	
}

void _XWindow_ChargeGauge::SetGaugeStop()
{
	m_bGaugeStop = TRUE;
	m_bReadySandMsg = FALSE;
	
	switch(m_CurrentGaugeMode) 
	{
	case _XGAUGE_ELIXIR :		
		g_pLocalUser->m_CurrentInternalUseState = _XUSER_STATE_5;
		break;
	case _XGAUGE_READ:
		g_pLocalUser->m_CurrentReadBookState = _XUSER_STATE_5;
		break;
	case _XGAUGE_SOCKET :
		g_pLocalUser->m_bSocketPlugIn = FALSE;
		g_pLocalUser->m_bSocketPlugInLock = FALSE;
		break;
	case _XGAUGE_GAMBLE :
		g_pLocalUser->m_bGambleBoxOpen = FALSE;
		g_pLocalUser->m_bGambleBoxOpenLock = FALSE;
		break;
	}

	StartFadeOutMode();
}

void _XWindow_ChargeGauge::StatusBinsa()
{
	//if( m_CurrentGaugeMode == _XGAUGE_BINSA )
	//	return;
	
	switch(m_CurrentGaugeMode) 
	{
//	case _XGAUGE_THREATEN :
	case _XGAUGE_CHARGE :
	case _XGAUGE_ELIXIR :
	case _XGAUGE_ESCAPE	:	
		break;
	case _XGAUGE_READ:
		break;
	case _XGAUGE_SOCKET :
		{
			if( g_pInventory_Window )
			{
				_XWindow_SocketPlugIn* pSocketPlugIn_Window = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
				if( pSocketPlugIn_Window )
				{
					pSocketPlugIn_Window->ShowWindow(FALSE);
					pSocketPlugIn_Window->ResetSocketItemList();					
					g_pInventory_Window->ResetSocketIndexList();
					g_pInventory_Window->SetDefaultPosition();
					
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1609), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS );
				}
				g_pLocalUser->m_bSocketPlugIn = FALSE;
				g_pLocalUser->m_bSocketPlugInLock = FALSE;
			}			
		}
		break;
	case _XGAUGE_SOCKETDEATCH :
		{
#ifdef _XDEF_SOCKET_DETACHSYSTEM_070704_MAGVIPER //Author : 양희왕 //breif :소켓 제거
			_XWindow_SocketDetach* pSocketDetachWindow = (_XWindow_SocketDetach*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETDETACH_WINDOW);
			if( pSocketDetachWindow  )
			{
				pSocketDetachWindow->ShowWindow(FALSE);
				pSocketDetachWindow->ReBuild(MODE_NONE);					
				g_pInventory_Window->ResetSocketIndexList();
				g_pInventory_Window->SetDefaultPosition();
			}

			g_pLocalUser->m_bSocketPlugIn = FALSE;
			g_pLocalUser->m_bSocketPlugInLock = FALSE;	
#endif
		}
		break;
	case _XGAUGE_GAMBLE :
		{
			if( g_pInventory_Window )
			{
				_XWindow_Gamble* pGamble_Window = (_XWindow_Gamble*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GAMBLE);
				if( pGamble_Window )
				{
					pGamble_Window->ShowWindow(FALSE);
					pGamble_Window->ResetSocketItemList();					
					g_pInventory_Window->ResetSocketIndexList_GambleBox();
					g_pInventory_Window->SetDefaultPosition();

					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NETWORK_1610), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, FALSE );
				}
			
				g_pLocalUser->m_bGambleBoxOpen = FALSE;
				g_pLocalUser->m_bGambleBoxOpenLock = FALSE;
			}
		}
		break;
	case _XGAUGE_SMELTITEM : //Author : 양희왕
		{
			if( g_pInventory_Window )
			{
				XWindow_SmeltItem* pSmeltItemWindow = (XWindow_SmeltItem*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SMELT_ITEM_WINDOW);
				if( pSmeltItemWindow )
				{
					pSmeltItemWindow->ShowWindow(FALSE);
					pSmeltItemWindow->SetDefaultPosition();
				}
			}			
		}
		break;
	}	
	ShowWindow(FALSE);
}
// XProc_ServerUnification.cpp: implementation of the XProc_ServerUnification class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ServerUnification.h"
#include "XProc_ServerUnification.h"
#include "XWindow_ChangeName.h"
#include "XSR_STRINGHEADER.H"
#include "FlashControlInterface.h"


extern CFlashControlInterface*	g_pTitleFlashObject;
extern LPDIRECT3DTEXTURE9		g_pTitleFlashOffScreenTexture;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XProc_ServerUnification::XProc_ServerUnification()
{

}

XProc_ServerUnification::~XProc_ServerUnification()
{

}

BOOL XProc_ServerUnification::InitializeProcess( void )
{
#ifdef _XDWDEBUG 
	g_NormalGameExit = TRUE;
#else 
	g_NormalGameExit = FALSE;
#endif
	
	_XSetDefaultPath();
	
	// Build Texture =============================================================
	m_TextureArchive1.DisposeTexture();
	m_TextureArchive2.DisposeTexture();

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_INTERFACE.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_INTERFACE.XP") );
#endif

	m_TextureArchive1.SetTexturePackage( packagefilename );
	
	m_TextureArchive1.GetResourceIndex("MI_Login_00.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive1.GetResourceIndex("MI_Login_01.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive1.GetResourceIndex("MI_Login_02.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive1.GetResourceIndex("MI_Login_03.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);		
	
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_MAININTERFACE.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_MAININTERFACE.XP") );
#endif

	m_TextureArchive2.SetTexturePackage( packagefilename );

	m_TextureArchive2.GetResourceIndex("MI_Main_Back01.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive2.GetResourceIndex("MI_npcquestwin_01.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive2.GetResourceIndex("MI_System.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	
	m_TextureArchive1.BuildTextureList( TRUE );
	m_TextureArchive2.BuildTextureList( TRUE );
	
	// window create ===================================================================
	_XWindow_ServerUnification* pServerUnification_Window = new _XWindow_ServerUnification;
	_XWINDOW_STRUCTURE serverunificationstructure = 
	{
		TRUE, {80, 82}, {196, 502}, 0,
			-1,
			&m_TextureArchive1,
			D3DCOLOR_ARGB(128, 0, 0, 0),
			_XDEF_WTITLE_SERVERUNIFICATIONWINDOW
	};
	pServerUnification_Window->Create(serverunificationstructure);
	pServerUnification_Window->Initialize(m_TextureArchive1, m_TextureArchive2);	
	pServerUnification_Window->EnableWindowEffect(FALSE);
	pServerUnification_Window->SetDocking(FALSE);	
	pServerUnification_Window->ShowWindow( TRUE );
		
	m_WindowManager.InsertWindow(pServerUnification_Window);

	// 이름 변경 윈도우 
	_XWindow_ChangeName* pChangeName_Window = new _XWindow_ChangeName;
	_XWINDOW_STRUCTURE changenamewindowstructure = 
	{
		TRUE, {473 , 387}, {183, 84}, 0,
			-1,
			&m_TextureArchive1,
			D3DCOLOR_ARGB(255, 255, 255, 255),
			_XDEF_WTITLE_CHANGENAMEWINDOW
	};
	pChangeName_Window->Create(changenamewindowstructure);
	pChangeName_Window->Initialize(m_TextureArchive1, m_TextureArchive2);	
	pChangeName_Window->EnableWindowEffect(FALSE);
	pChangeName_Window->SetDocking(FALSE);
	pChangeName_Window->ShowWindow(FALSE);
	m_WindowManager.InsertWindow(pChangeName_Window);
	
	m_BackgroundStatic.Create( 0, 0, gnWidth, gnHeight, NULL, -1 );
	m_BackgroundStatic.m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	
	if( !CreateOffScreen() )
	{
		return FALSE;
	}
	
	if( !g_pTitleFlashObject )
	{
		g_pTitleFlashObject = GetFlashControl();
		
		if( !g_pTitleFlashObject ) return FALSE;
		
		g_pTitleFlashObject->initialize( gHWnd,gHInst,g_pTitleFlashOffScreenTexture);
		g_pTitleFlashObject->SetLowQuality();
	}
	
	HDC pDC = GetDC( gHWnd );
	g_pTitleFlashObject->SetDC( pDC );
	
	TCHAR flashfilename[64];
	
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{			
		strcpy( flashfilename, _T("SELECT00.swf") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( flashfilename, _T("vn_SELECT00.swf") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( flashfilename, _T("tw_SELECT00.swf") );
	}
	else
	{
		strcpy( flashfilename, _T("SELECT00.swf") );
	}

	
	_XPackageArchive package;
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
#ifdef _XTESTSERVER				
	package.OpenPackage( _T("tsXSCENESCRIPT.XP") );
#else
	package.OpenPackage( _T("XSCENESCRIPT.XP") );
#endif
	g_pTitleFlashObject->FreeBuffer();
	g_pTitleFlashObject->ControlClose();
	
	int resourceid = package.FindResource( flashfilename );
	if( resourceid >= 0 )
	{
		long filesize = package.GetPackedFileSize( resourceid );
		g_pTitleFlashObject->SetResourceFilePtr( package.GetPackedFile( resourceid ), filesize );
		g_pTitleFlashObject->ControlOpen( flashfilename );
		g_pTitleFlashObject->SetLowQuality();
	}
	package.FinalizePackage();
	
	g_pTitleFlashObject->SetDC( 0 );
	ReleaseDC( gHWnd, pDC );
	
	g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );

	InitDeviceObject();	

	g_MessageBox.ShowWindow( FALSE );	// 나중에 지워야함

	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );

	g_NetworkKernel.SendPacket( MSG_NO_UNI_GROUP_LOBBY, en_ug_req_simple_grp_info );
	
	return TRUE;
}

void XProc_ServerUnification::DestroyProcess( void )
{
	m_TextureArchive1.DisposeTexture();	
	m_TextureArchive2.DisposeTexture();
	m_WindowManager.DestroyManager();

	g_EnvironmentManager.m_ObjectLight.Position = D3DXVECTOR3( 100.0f, 40.0f, -100.0f );
	SetEnableProcess( FALSE );
}

bool XProc_ServerUnification::InitDeviceObject( void )
{
	m_TextureArchive1.InitDeviceObject();
	m_TextureArchive2.InitDeviceObject();

	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = gnWidth;
	vp.Height = gnHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	gpDev->SetViewport(&vp);
	
	gpDev->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
	gpDev->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

	gpDev->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    gpDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	
	gpDev->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	
	
	// 16비트 모드일 경우 디더링을 켜준다.
	//if( gfmtDepth == D3DFMT_D16 ) gpDev->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	//else gpDev->SetRenderState( D3DRS_DITHERENABLE,   FALSE ); 
	
    gpDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    
	gpDev->SetRenderState( D3DRS_ZENABLE,        g_RS_UseZBuffer );
	gpDev->SetRenderState( D3DRS_ZFUNC,			 g_RS_ZBufferFunc );
	gpDev->SetRenderState( D3DRS_DEPTHBIAS,		 0 );
	
#ifdef _XDEF_USETERRAINDIFFUSECOLOR
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_COLOR1 );
#else
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
#endif

	gpDev->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	gpDev->SetRenderState( D3DRS_LOCALVIEWER, FALSE );
	
	gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	
	// Alpha blend state
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );		
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	// Alpha test state
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
	gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);	
		
	ConfirmDevice();

	g_QuestScriptManager.SetQuestCameraMode(FALSE);
	g_LodTerrain.m_3PCamera.Set( 45.0f, (float)gnWidth/(float)gnHeight, 1.0f, 100.0f );
	g_LodTerrain.m_3PCamera.SetPitchLimits( -50.0f, 73.0f);
	g_LodTerrain.m_3PCamera.SetRollLimits(-0, 0);
	g_LodTerrain.m_3PCamera.SetDistanceLimits(1.0f, 10.f);
	g_LodTerrain.m_3PCamera.SetDistance( 3.4f );	
	g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3( 0.11f, 0.35f, 0.0f ) );
	g_LodTerrain.m_3PCamera.mp_fYaw = -2.400f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 12.800f;
	g_LodTerrain.m_3PCamera.mp_fRoll = 0.0f;
	g_LodTerrain.m_3PCamera.UpdateViewMatrix();
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();
	
	CreateOffScreen();
	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );
	}

	return true;
}	

void XProc_ServerUnification::ReleaseDeviceObject( void )
{

}

void XProc_ServerUnification::PauseProcess( void )
{

}

void XProc_ServerUnification::UnPauseProcess( void )
{

}

void XProc_ServerUnification::Draw( void )
{
	gpDev->BeginScene();

	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);

	gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState( D3DRS_LIGHTING, FALSE );	
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
	m_BackgroundStatic.DrawWithRegion( g_pTitleFlashOffScreenTexture );
	
	if( !g_pTitleFlashObject->IsPlaying() )
	{
		// Draw Selected Character	
		m_WindowManager.Draw();		
		g_MessageBox.Draw();
	}

	// End the scene
	gpDev->EndScene();
	
	// Present the backbuffer contents to the display	
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true; 
}

void XProc_ServerUnification::OnPaint(WPARAM wparam, LPARAM lparam)
{	
	Draw();
}

bool XProc_ServerUnification::Process( void )
{
	if(g_pTitleFlashObject) g_pTitleFlashObject->PlayTimerFire();
	
	if( !g_pTitleFlashObject->IsPlaying() )
	{
		g_MessageBox.Process();
		
		m_WindowManager.Process();		
	}
	
	return true;
}

void XProc_ServerUnification::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	_XWindow_ServerUnification* pServerUnification_Window = (_XWindow_ServerUnification*)m_WindowManager.FindWindow(_XDEF_WTITLE_SERVERUNIFICATIONWINDOW);
	if( !pServerUnification_Window )	return;

	if( message == WM_COMMAND )
	{
		switch( wparam ) 
		{
		case _XDEF_SU_NEXTBUTTON :
			{
				UNIFICATIONSTEP CurStep = pServerUnification_Window->GetUnificationStep();
				switch(CurStep) 
				{
				case SU_READRULE :
					{
						pServerUnification_Window->SetUnificationStep(SU_SELECTSERVER);
					}					
					break;
				case SU_SELECTSERVER :
					if( pServerUnification_Window->GetSelectServerIndex() > 0 )
					{
						g_NetworkKernel.SendPacket( MSG_NO_UNI_GROUP_LOBBY, en_ug_select_simple_grp );
					}
					else
					{
						// 에러 메세지 처리
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("이전할 서버를 선택하여 주십시요.") );	// not use string
						return;
					}
					break;
				case SU_SELECTCHARAC :
					{
						if( pServerUnification_Window->GetSelectWarehouseIndex() < 0 )
						{
							// 에러 메세지 처리
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("창고를 선택하지 않으셨습니다. 선택하지 않으면 복구가 불가능 합니다.서버 통합을 하시겠습니까?"), TRUE, _XDEF_SU_NOTSELECT_WAREHOUSE  );	// not use string
							return;
						}
						else
						{
							if( !pServerUnification_Window->GetSelectCharacterCount() )
							{
								// 에러 메세지 처리
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("선택하지 않은 캐릭터는 복구가 불가능 합니다.서버 통합을 하시겠습니까?"), TRUE, _XDEF_SU_NOTSELECT_CHARACTER );	// not use string
								return;
							}
							else
							{
								g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("선택하지 않은 창고와 캐릭터는 더이상 복구 할 수 없습니다.서버 통합을 하시겠습니까?"), TRUE, _XDEF_SU_UNIFICATION_YES );// not use string
								return;
							}
						}						
					}					
					break;
				case SU_CHECKCHARAC :
					{
						pServerUnification_Window->SetUnificationStep(SU_FINISH);
					}
					break;
				}
			}			
			break;
		case _XDEF_SU_PREVBUTTON :
			{
				UNIFICATIONSTEP CurStep = pServerUnification_Window->GetUnificationStep();
				switch(CurStep) 
				{
				case SU_SELECTSERVER :
					CurStep = SU_READRULE;
					break;
				case SU_SELECTCHARAC :
					CurStep = SU_SELECTSERVER;
					break;
				}
				pServerUnification_Window->SetUnificationStep(CurStep);
			}
			break;
		case _XDEF_SU_EXITBUTTON :
			{
				SetFocus( gHWnd );

				g_NetworkKernel.DisconnectServer();
				g_NetworkKernel.ReleaseNetwork();
				g_NetworkKernel.InitializeNetwork();
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_PrevProcess_SelectCharacter = TRUE;
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( 
					&((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );
			}
			break;
		case _XDEF_SU_CHECKBTN1 :
		case _XDEF_SU_CHECKBTN2	:
		case _XDEF_SU_CHECKBTN3	:
		case _XDEF_SU_CHECKBTN4	:
		case _XDEF_SU_CHECKBTN5	:
		case _XDEF_SU_CHECKBTN6	:
		case _XDEF_SU_CHECKBTN7	:
		case _XDEF_SU_CHECKBTN8	:
		case _XDEF_SU_CHECKBTN9	:
		case _XDEF_SU_CHECKBTN10:	
		case _XDEF_SU_CHECKBTN11:	
		case _XDEF_SU_CHECKBTN12:
			{
				pServerUnification_Window->ClickCheckButton(wparam-_XDEF_SU_CHECKBTN1);
			}
			break;
		case _XDEF_SU_CHANGEBTN1 :
		case _XDEF_SU_CHANGEBTN2 :
		case _XDEF_SU_CHANGEBTN3 :
			{
				_XWindow_ChangeName* pChangeName_Window = (_XWindow_ChangeName*)m_WindowManager.FindWindow(_XDEF_WTITLE_CHANGENAMEWINDOW);
				if( pChangeName_Window )
				{
					pChangeName_Window->SetSelectedIndex(wparam-_XDEF_SU_CHANGEBTN1);
					pChangeName_Window->ShowWindow(TRUE);
				}
			}
			break;
		case _XDEF_SU_NOTSELECT_WAREHOUSE :
			{
				if( !pServerUnification_Window->GetSelectCharacterCount() )
				{
					// 에러 메세지 처리
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("선택하지 않은 캐릭터는 복구가 불가능 합니다.서버 통합을 하시겠습니까?"), TRUE, _XDEF_SU_NOTSELECT_CHARACTER );	// not use string
					return;
				}
				else
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("선택하지 않은 창고와 캐릭터는 더이상 복구 할 수 없습니다.서버 통합을 하시겠습니까?"), TRUE, _XDEF_SU_UNIFICATION_YES );	// not use string
				}
			}
			break;
		case _XDEF_SU_NOTSELECT_CHARACTER :
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("선택하지 않은 창고와 캐릭터는 더이상 복구 할 수 없습니다.서버 통합을 하시겠습니까?"), TRUE, _XDEF_SU_UNIFICATION_YES );	// not use string
			}
			break;
		case _XDEF_SU_INPUT_JUMINNUMBER :
			{
				g_NetworkKernel.SendPacket( MSG_NO_UNI_GROUP_LOBBY, en_ug_move_request );				
			}
			break;
		case _XDEF_CN_CHANGENAMEYES :
			{
				_XWindow_ChangeName* pChangeName_Window = (_XWindow_ChangeName*)m_WindowManager.FindWindow(_XDEF_WTITLE_CHANGENAMEWINDOW);
				if( pChangeName_Window )
				{
					pChangeName_Window->ChangeNameFiltering();
					pChangeName_Window->ShowWindow(FALSE);
				}				
			}
			break;
		case _XDEF_CN_CHANGENAMENO :
			{
				_XWindow_ChangeName* pChangeName_Window = (_XWindow_ChangeName*)m_WindowManager.FindWindow(_XDEF_WTITLE_CHANGENAMEWINDOW);
				if( pChangeName_Window )
					pChangeName_Window->ShowWindow(FALSE);				
			}
			break;
		case _XDEF_CN_CHANGENAMEREALYES :
			{
				g_NetworkKernel.SendPacket( MSG_NO_UNI_GROUP_LOBBY, en_ug_change_name_request );
			}
			break;
		case _XDEF_SU_UNIFICATION_YES :
			{
				g_MessageBox.SetPasswordMode( TRUE );
				g_MessageBox.m_IMEControl.SetMaxLength(7);
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_INPUTSTRING, ID_STRING_SELECTCHARACTER_INPUTPASSWD, TRUE, _XDEF_SU_INPUT_JUMINNUMBER );
			}
			break;
		}
	}	
}
void XProc_ServerUnification::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{
		if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer )
		{
			g_NetworkKernel.ProcessNetworkError();
			g_NetworkKernel.DisconnectServer();
			g_NetworkKernel.ReleaseNetwork();
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_PrevProcess_SelectCharacter = TRUE;
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );
			return;
		}
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{	
	case FD_CONNECT:	
		
		if(WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE ) == SOCKET_ERROR)
		{
			g_NetworkKernel.ProcessNetworkError();
		}

		g_NetworkKernel.m_bConnected = TRUE;
						
		((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer = FALSE;
		
		_XDWINPRINT( "Reconnected server : character select process" );
		break;
		
	case FD_READ :
		if( !g_NetworkKernel.m_bConnected )
			g_NetworkKernel.m_bConnected = TRUE;

		if( wparam == g_NetworkKernel.m_hClientSocket )
		{
			if( !((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer )
				g_NetworkKernel.ReadSocket();
		}
		break;
	case FD_CLOSE :
		if( ((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MoveServer )
		{			
		}
		else
		{
			//_XDWINPRINT( "SERVER MESSAGE : Socket close message" );
			g_NetworkKernel.ProcessNetworkError();      
			g_NetworkKernel.DisconnectServer();
			g_NetworkKernel.ReleaseNetwork();
			
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( 
				&((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );	

		}
		break;
	}
};

void XProc_ServerUnification::ConfirmDevice(void)
{
	D3DCAPS9 d3dcaps;
	if(gpDev)
		gpDev->GetDeviceCaps(&d3dcaps);
	else
		return;
	
	if( d3dcaps.MaxSimultaneousTextures > 1 )
	{
		g_bCanDoMultitexture = TRUE;
	}
	
	if( d3dcaps.TextureOpCaps & D3DTEXOPCAPS_ADDSIGNED )
	{
		g_bCanDoAddSigned = TRUE;
	}
	
	if( !(d3dcaps.TextureOpCaps & D3DTEXOPCAPS_MODULATE2X) )
	{
		// the device can't do mod 2x.  If we also can't do add signed,
		// we have no way to do the multitexture.
		if( !g_bCanDoAddSigned )
		{
			// turn off multitexture and just go with the one detail texture
			g_bCanDoMultitexture = FALSE;
		}
	}
	
	g_bCanDoAlphaBlend = (d3dcaps.SrcBlendCaps & D3DPBLENDCAPS_SRCALPHA) &&
		(d3dcaps.DestBlendCaps & D3DPBLENDCAPS_INVSRCALPHA);
	
	g_bCanDoRangeFog = d3dcaps.RasterCaps & D3DPRASTERCAPS_FOGRANGE;
	g_bCanDoTableFog = d3dcaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE;
}

BOOL XProc_ServerUnification::CreateOffScreen( void )
{
	if( g_pTitleFlashOffScreenTexture ) return TRUE;
	
	SIZE size = m_BackgroundStatic.GetWindowSize();
	
	if( FAILED( gpDev->CreateTexture( size.cx, size.cy, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTitleFlashOffScreenTexture, NULL ) ) )
    {
        return FALSE;
    }
	
	return TRUE;
}
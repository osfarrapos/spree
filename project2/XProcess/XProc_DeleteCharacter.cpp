// XProc_DeleteCharacter.cpp: implementation of the XProc_DeleteCharacter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XProc_DeleteCharacter.h"
#include "XWindow_CreateCharacter.h"
#include "XWindow_ZoomWindow.h"
#include "XWindow_RotateWindow.h"
#include "XWindow_SelectLoginServer.h"
#include "XSR_STRINGHEADER.H"
#include "FlashControlInterface.h"

extern CFlashControlInterface*	g_pTitleFlashObject;
extern LPDIRECT3DTEXTURE9		g_pTitleFlashOffScreenTexture;
//////////////////////////////////////////////////////////////////////
// Local definition

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XProc_DeleteCharacter::XProc_DeleteCharacter()
{	
	setProcessInfo( 0, "Delete character", __FILE__ );
}

XProc_DeleteCharacter::~XProc_DeleteCharacter()
{

}

BOOL XProc_DeleteCharacter::InitializeProcess(void)
{
#ifdef _XDWDEBUG 
	g_NormalGameExit = TRUE;
#else 
	g_NormalGameExit = FALSE;
#endif
	
	_XSetDefaultPath();

	if( !g_pTitleFlashObject )
	{
		g_pTitleFlashObject = GetFlashControl();
		
		if( !g_pTitleFlashObject ) return FALSE;
		
		g_pTitleFlashObject->initialize( gHWnd,gHInst,g_pTitleFlashOffScreenTexture);
		g_pTitleFlashObject->SetLowQuality();
	}
	
	HDC pDC = GetDC( gHWnd );
	g_pTitleFlashObject->SetDC( pDC );
	
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{			
		strcpy(flashfilename, _T("MAKING00.swf"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy(flashfilename, _T("vn_MAKING00.swf"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy(flashfilename, _T("tw_MAKING00.swf"));
	}
	else
	{
		strcpy(flashfilename, _T("MAKING00.swf"));
	}
	
	_XPackageArchive package;
	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
#ifdef _XTESTSERVER				
	if( !package.OpenPackage( _T("tsXSCENESCRIPT.XP") ) )
	{
		return FALSE;
	}
#else
	if( !package.OpenPackage( _T("XSCENESCRIPT.XP") ) )
	{
		return FALSE;
	}
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
	
	g_EnvironmentManager.m_ObjectLight.Position = D3DXVECTOR3( -100.0f, 40.0f, -100.0f );

	m_SelectedZone = 1;

	
	InitDeviceObject();	
				
	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );

	return TRUE;
}

BOOL XProc_DeleteCharacter::CreateOffScreen( void )
{
	if( g_pTitleFlashOffScreenTexture ) return TRUE;
	
	SIZE size = m_BackgroundStatic.GetWindowSize();
	
	if( FAILED( gpDev->CreateTexture( size.cx, size.cy, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTitleFlashOffScreenTexture, NULL ) ) )
    {
        return FALSE;
    }
	
	return TRUE;
}

void XProc_DeleteCharacter::DestroyProcess( void )
{
	m_TextureArchive.DisposeTexture();
	m_WindowManager.DestroyManager();
	m_PreviewCharacter.Reset();

	g_EnvironmentManager.m_ObjectLight.Position = D3DXVECTOR3( 100.0f, 40.0f, -100.0f );

	g_QuestScriptManager.SetQuestCameraMode(FALSE);
	
//	if(g_BgmPlayFlag && g_BgmObject.Getm_dwDeviceID())  
//		g_BgmObject.Close();
	SetEnableProcess( FALSE );	
}

bool XProc_DeleteCharacter::InitDeviceObject(void)
{
	m_TextureArchive.InitDeviceObject();
	
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
	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	// 16비트 모드일 경우 디더링을 켜준다.
	//if( gfmtDepth == D3DFMT_D16 ) gpDev->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	//else gpDev->SetRenderState( D3DRS_DITHERENABLE,   FALSE ); 
	
    gpDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
    
	gpDev->SetRenderState( D3DRS_ZENABLE,        g_RS_UseZBuffer );
	gpDev->SetRenderState( D3DRS_ZFUNC,			 g_RS_ZBufferFunc );
	gpDev->SetRenderState( D3DRS_DEPTHBIAS ,			 0 );
	
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
	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	g_QuestScriptManager.SetQuestCameraMode(FALSE);
	g_LodTerrain.m_3PCamera.Set( 45.0f, (float)gnWidth/(float)gnHeight, 1.0f, 100.0f );
	g_LodTerrain.m_3PCamera.SetPitchLimits( -50.0f, 73.0f);
	g_LodTerrain.m_3PCamera.SetRollLimits(-0, 0);
	g_LodTerrain.m_3PCamera.SetDistanceLimits(1.4f, 10.f);
	g_LodTerrain.m_3PCamera.SetDistance( 2.6f );	
	g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3( 0.125f, 0.495f, 0.0f ) );
	g_LodTerrain.m_3PCamera.mp_fYaw = 0.0f;
	g_LodTerrain.m_3PCamera.mp_fPitch = 180.0f;
	g_LodTerrain.m_3PCamera.mp_fRoll = 0.0f;
	g_LodTerrain.m_3PCamera.UpdateViewMatrix();
	g_LodTerrain.m_3PCamera.UpdateProjMatrix();

	m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
	m_CameraMovingTargetDistance = 2.6f;
	m_CameraMoving = FALSE;

	//g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3( 0.125f, 0.895f, 0.0f ) );

	m_PreviewCharacterRotateAngle = 16.0f;
	D3DXMatrixRotationY( &m_PreviewCharacter.m_Position, _X_RAD( m_PreviewCharacterRotateAngle ) );
	m_PreviewCharacter.CreateShadowDecal();

	CreateOffScreen();
	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );
	}

	return true;
}

void XProc_DeleteCharacter::ReleaseDeviceObject( void )
{		
	
}

void XProc_DeleteCharacter::PauseProcess( void )
{

}

void XProc_DeleteCharacter::UnPauseProcess( void )
{
	
}

void XProc_DeleteCharacter::Draw(void)
{
	// Begin the scene
	gpDev->BeginScene();	

	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Set Third person view matrix
	gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
	gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());
		
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState( D3DRS_LIGHTING, FALSE );	
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
	m_BackgroundStatic.DrawWithRegion( g_pTitleFlashOffScreenTexture );


	if( !g_pTitleFlashObject->IsPlaying() )
	{
		// Draw Character ===============================================
		if(m_CharacterGender == _XGENDER_MAN)
			m_PreviewCharacter.SetCurMotion(0);
		else
			m_PreviewCharacter.SetCurMotion(0);

		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

		bool playsound = true;
		m_PreviewCharacter.Advance(playsound);
		m_PreviewCharacter.Animation();
		m_PreviewCharacter.RenderShadow();
		m_PreviewCharacter.RenderCharacter( NULL );

		gpDev->SetRenderState( D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );

		gpDev->LightEnable( 1, FALSE );	
		gpDev->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
		
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );

		m_BottomOverlayLeftImage.Draw();
		m_BottomOverlayRightImage.Draw();

		_XDrawSolidBar( 264, 680, 776, 768, 0xFF000000 );

		m_WindowManager.Draw();

		g_IMEWindow.Draw();	
		g_MessageBox.Draw();
	}

	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
		
#ifdef _XDWDEBUG		
	if( gpInput->CheckKeyPress(DIK_LCONTROL) )
	{
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->Print( 4, gnHeight-13, 1.0f, "FPS: %d", gnFPS );
		g_XBaseFont->Print( 4, gnHeight-39, 1.0f, "X: %f Y: %f Z: %f Yaw: %f Pitch: %f Roll: %f",
			g_LodTerrain.m_3PCamera.m_CameraPosition.x,g_LodTerrain.m_3PCamera.m_CameraPosition.y,
			g_LodTerrain.m_3PCamera.m_CameraPosition.z, g_LodTerrain.m_3PCamera.GetYaw(),
			g_LodTerrain.m_3PCamera.GetPitch(),g_LodTerrain.m_3PCamera.GetRoll() );
		g_XBaseFont->Print( 4, gnHeight-26, 1.0f, "Target X: %f Y: %f Z: %f Distance : %f %f",
			g_LodTerrain.m_3PCamera.m_TargetPosition.x,g_LodTerrain.m_3PCamera.m_TargetPosition.y,
			g_LodTerrain.m_3PCamera.m_TargetPosition.z,g_LodTerrain.m_3PCamera.m_TargetDistance, m_PreviewCharacterRotateAngle);
		g_XBaseFont->Flush();
	}
#endif	
	
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	
	gpDev->SetRenderState( D3DRS_ZENABLE,   g_RS_UseZBuffer );
	
	// End the scene
	gpDev->EndScene();
	
	// Present the backbuffer contents to the display	
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true;
}

bool XProc_DeleteCharacter::Process( void )
{	
	if(g_pTitleFlashObject) g_pTitleFlashObject->PlayTimerFire();

	g_MessageBox.Process();	
	g_IMEWindow.Process();
	m_WindowManager.Process();

	if( m_CameraMoving )
	{
		FLOAT fFrameSpeed = 0.0f;

		FLOAT distance = _XMeshMath_DistanceBetweenTwoPoint( m_CameraMovingTarget, g_LodTerrain.m_3PCamera.m_TargetPosition );
		FLOAT maxdistance = _XMeshMath_DistanceBetweenTwoPoint( D3DXVECTOR3( 0.125f, 0.495f, 0.0f ), D3DXVECTOR3( 0.125f, 0.895f, 0.0f ) );

		if( maxdistance - distance < 0.08f )
			 fFrameSpeed = g_fElapsedFrameMilisecondTime / 4.0f;
		else if( maxdistance - distance < 0.18f )
			 fFrameSpeed = g_fElapsedFrameMilisecondTime / 1.2f;
		else 
		{
			if( distance < 0.08f )
				fFrameSpeed = g_fElapsedFrameMilisecondTime / 2.0f;
			else if( distance < 0.18f )
				fFrameSpeed = g_fElapsedFrameMilisecondTime;
			else
				fFrameSpeed = g_fElapsedFrameMilisecondTime * 2.5f;
		}

		BOOL zooming = TRUE;
		if( m_CameraMovingTargetDistance > g_LodTerrain.m_3PCamera.GetDistance() )
		{
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() + fFrameSpeed * 5.0f );

			if( m_CameraMovingTargetDistance < g_LodTerrain.m_3PCamera.GetDistance() )
			{
				g_LodTerrain.m_3PCamera.SetDistance( m_CameraMovingTargetDistance );
				zooming = FALSE;
			}
		}
		else if( m_CameraMovingTargetDistance < g_LodTerrain.m_3PCamera.GetDistance() )
		{
			g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - fFrameSpeed * 5.0f );

			if( m_CameraMovingTargetDistance > g_LodTerrain.m_3PCamera.GetDistance() )
			{
				g_LodTerrain.m_3PCamera.SetDistance( m_CameraMovingTargetDistance );
				zooming = FALSE;
			}
		}
		else
		{
			zooming = FALSE;
		}
				
		if( distance < EPSILON3 )
		{
			g_LodTerrain.m_3PCamera.m_TargetPosition = m_CameraMovingTarget;
			g_LodTerrain.m_3PCamera.UpdateViewMatrix();
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();

			if( !zooming )
				m_CameraMoving = FALSE;
		}
		else
		{
			D3DXVECTOR3 direction = m_CameraMovingTarget - g_LodTerrain.m_3PCamera.m_TargetPosition;
			D3DXVec3Normalize( &direction, &direction );

			D3DXVECTOR3 prevPosition = g_LodTerrain.m_3PCamera.m_TargetPosition;

			g_LodTerrain.m_3PCamera.m_TargetPosition += direction * fFrameSpeed * 1.68f;

			FLOAT prevdistance = _XMeshMath_DistanceBetweenTwoPoint( prevPosition, m_CameraMovingTarget );
			FLOAT newdistance = _XMeshMath_DistanceBetweenTwoPoint( m_CameraMovingTarget, g_LodTerrain.m_3PCamera.m_TargetPosition );

			if( prevdistance < newdistance )
			{
				g_LodTerrain.m_3PCamera.m_TargetPosition = m_CameraMovingTarget;
				
				if( !zooming )
					m_CameraMoving = FALSE;
			}

			g_LodTerrain.m_3PCamera.UpdateViewMatrix();
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		}
	}


	return true;
}

void XProc_DeleteCharacter::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	_XWindow_CreateCharacter* pCreateCharacter_Window = (_XWindow_CreateCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_CREATECHARACTER);
	
	if( message == WM_COMMAND )
	{
		switch( wparam )
		{		
		case _XDEF_CC_CREATEBUTTON  :

			strcpy(  m_CharacterNameString, pCreateCharacter_Window->m_NameEdit.GetText() );

			if( strlen(m_CharacterNameString) <= 0 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_EMPTYNAME, TRUE );
			}
			else if( strlen(m_CharacterNameString) <= 1 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_SHORTNAME, TRUE );
			}			
			else if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_USEPOINT, TRUE );
			}			
			else
			{	
				if( strchr( pCreateCharacter_Window->m_NameEdit.GetText(), _T(' ') ) )
				{
					TCHAR warnstring[512];
					strcpy(warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FILTERNAME));					
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _T("인물 이름에 공백은 사용하실수 없습니다."), TRUE );
				}
				else
				{
					TCHAR resultstring[256]; 
					memset( resultstring, 0x00, 256 );
					strcpy( resultstring, pCreateCharacter_Window->m_NameEdit.GetText() );				
					g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_NAME | _XDEF_WORDFILETERINGTYPE_ALWAYS, TRUE );

					// 필터링을 정상적으로 통과하면 서버선택창을 연다.
					if( strcmpi( resultstring, pCreateCharacter_Window->m_NameEdit.GetText() ) == 0 )
					{
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_CREATECHARACTER_CREATECHARACTER, TRUE, _XDEF_CC_CREATECHARACTER );
					}				
					else
					{
						TCHAR warnstring[512];
						strcpy(warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FILTERNAME));
					
						TCHAR message[512];
						memset( message, 0x00, 512 );
						sprintf( message, "%s(은)는 사용하실수 없는 단어입니다.", g_WordFilteringManager.GetLastFilterdWord() );
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
					}
				}
			}
			break;		
		case _XDEF_CC_CREATECHARACTER:
			{
				BOOL gendercheck = TRUE;
				
				if( m_SelectedZone == 1 ) // 소림일때 
				{
					if( m_CharacterGender != _XGENDER_MAN )
					{
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("소림사에는 여자가 출입할 수 없습니다."), TRUE );
						gendercheck = FALSE;
					}
				}
				else if( m_SelectedZone == 4 ) // 비궁일때 
				{
					if( m_CharacterGender != _XGENDER_WOMAN )
					{
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("자하동에는 남자가 출입할 수 없습니다."), TRUE );
						gendercheck = FALSE;
					}
				}					
				
				if( gendercheck )
				{
					strcpy(  m_CharacterNameString, pCreateCharacter_Window->m_NameEdit.GetText() );
					if( pCreateCharacter_Window->m_BonusPoint == 0 && ( strlen(m_CharacterNameString) > 0 ) )
					{
						g_NetworkKernel.SendPacket( MSG_NO_CHARAC_CREATE );
					}
				}
			}			
			break;		
		case _XDEF_CC_CANCELBUTTON	:
			{
				((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_PrevProcess_Login = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter );
			}
			break;
		case _XDEF_CC_REQUESTCHARACTERLIST:
			{
				SetFocus( gHWnd );
				((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_PrevProcess_Login = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter );
			}
			break;

		// 성별
		case _XDEF_CC_WOMENBUTTON :
			{
				m_CharacterGender = _XGENDER_WOMAN;
				m_PreviewCharacter.LinkBipedObject( &g_FemaleBipedObject );
				SetCharacter();
				
				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
				m_CameraMovingTargetDistance = 2.6f;

				pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeManIndex;
				pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeManIndex;
				pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeManIndex;
			}
			break;
		case _XDEF_CC_MANBUTTON	:
			{
				m_CharacterGender = _XGENDER_MAN;
				m_PreviewCharacter.LinkBipedObject( &g_MaleBipedObject );
				SetCharacter();
				
				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
				m_CameraMovingTargetDistance = 2.6f;

				pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeWomanIndex;
				pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeWomanIndex;
				pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeWomanIndex;
			}
			break;

		// 얼굴
		case _XDEF_CC_FACELEFTBUTTON :
			{
				if( g_LodTerrain.m_3PCamera.GetDistance() == 1.4f )
				{
					if( m_CharacterGender == _XGENDER_MAN )
					{
						m_FaceTypeManIndex--;
						if(m_FaceTypeManIndex == 0)
							m_FaceTypeManIndex = 8;						

						pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeManIndex;
					}
					else
					{
						m_FaceTypeWomanIndex--;
						if(m_FaceTypeWomanIndex == 0)
							m_FaceTypeWomanIndex = 8;						

						pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeWomanIndex;
					}
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.895f, 0.0f );
				m_CameraMovingTargetDistance = 1.4f;
			}
			break;
		case _XDEF_CC_FACERIGHTBUTTON :
			{
				if( g_LodTerrain.m_3PCamera.GetDistance() == 1.4f )
				{
					if( m_CharacterGender == _XGENDER_MAN )
					{
						m_FaceTypeManIndex++;
						if(m_FaceTypeManIndex == 9)
							m_FaceTypeManIndex = 1;						
						
						pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeManIndex;
					}
					else
					{
						m_FaceTypeWomanIndex++;
						if(m_FaceTypeWomanIndex == 9)
							m_FaceTypeWomanIndex = 1;						

						pCreateCharacter_Window->m_FaceTypeIndex = m_FaceTypeWomanIndex;
					}
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.895f, 0.0f );
				m_CameraMovingTargetDistance = 1.4f;
			}
			break;

		// 머리
		case _XDEF_CC_HAIRLEFTBUTTON :
			{
				if( g_LodTerrain.m_3PCamera.GetDistance() == 1.4f )
				{
					if(m_CharacterGender == _XGENDER_MAN)
					{
						m_HairTypeManIndex--;
						if(m_HairTypeManIndex == 0)
							m_HairTypeManIndex = 18;

						pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeManIndex;
					}
					if(m_CharacterGender == _XGENDER_WOMAN)
					{
						m_HairTypeWomanIndex--;
						if(m_HairTypeWomanIndex == 0)
							m_HairTypeWomanIndex = 18;

						pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeWomanIndex;
					}
					SetCharacter();					
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.895f, 0.0f );
				m_CameraMovingTargetDistance = 1.4f;
			}
			break;
		case _XDEF_CC_HAIRRIGHTBUTTON :
			{
				if( g_LodTerrain.m_3PCamera.GetDistance() == 1.4f )
				{
					if(m_CharacterGender == _XGENDER_MAN)
					{
						m_HairTypeManIndex++;
						if(m_HairTypeManIndex == 19)
							m_HairTypeManIndex = 1;

						pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeManIndex;
					}
					if(m_CharacterGender == _XGENDER_WOMAN)
					{
						m_HairTypeWomanIndex++;
						if(m_HairTypeWomanIndex == 19)
							m_HairTypeWomanIndex = 1;

						pCreateCharacter_Window->m_HairTypeIndex = m_HairTypeWomanIndex;
					}
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.895f, 0.0f );
				m_CameraMovingTargetDistance = 1.4f;
			}
			break;

		// 의상
		case _XDEF_CC_CLOTHLEFTBUTTON :
			{
				if( g_LodTerrain.m_3PCamera.GetDistance() == 2.6f )
				{
					if(m_CharacterGender == _XGENDER_MAN)
					{
						if( m_ClothTypeManIndex == 1 )
						{
							m_ClothTypeManIndex = 2;
						}
						else
						{
							m_ClothTypeManIndex--;
						}
						
						pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeManIndex;
					}
					else
					{
						if( m_ClothTypeWomanIndex == 1 )
						{
							m_ClothTypeWomanIndex = 2;
						}
						else
						{
							m_ClothTypeWomanIndex--;
						}

						pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeWomanIndex;
					}					
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
				m_CameraMovingTargetDistance = 2.6f;
			}
			break;
		case _XDEF_CC_CLOTHRIGHTBUTTON :
			{
				if( g_LodTerrain.m_3PCamera.GetDistance() == 2.6f )
				{
					if(m_CharacterGender == _XGENDER_MAN)
					{
						if( m_ClothTypeManIndex == 2 )
						{
							m_ClothTypeManIndex = 1;
						}
						else
						{
							m_ClothTypeManIndex++;
						}

						pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeManIndex;
					}
					else
					{
						if( m_ClothTypeWomanIndex == 2 )
						{
							m_ClothTypeWomanIndex = 1;
						}
						else
						{
							m_ClothTypeWomanIndex++;
						}

						pCreateCharacter_Window->m_ClothTypeIndex = m_ClothTypeWomanIndex;
					}
					SetCharacter();
				}

				m_CameraMoving = TRUE;
				m_CameraMovingTarget = D3DXVECTOR3( 0.125f, 0.495f, 0.0f );
				m_CameraMovingTargetDistance = 2.6f;
			}
			break;

		case _XDEF_CC_ADDBUTON1 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[0]++;
				pCreateCharacter_Window->m_Parameter[0] = m_Parameter[0];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON1 :
			if( m_Parameter[0] > _XDEF_DEFALTPARAMETER_1 )
			{
				m_Parameter[0]--;
				pCreateCharacter_Window->m_Parameter[0] = m_Parameter[0];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_ADDBUTON2 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[1]++;
				pCreateCharacter_Window->m_Parameter[1] = m_Parameter[1];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON2 :
			if( m_Parameter[1] > _XDEF_DEFALTPARAMETER_2 )
			{
				m_Parameter[1]--;
				pCreateCharacter_Window->m_Parameter[1] = m_Parameter[1];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_ADDBUTON3 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[2]++;
				pCreateCharacter_Window->m_Parameter[2] = m_Parameter[2];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON3 :
			if( m_Parameter[2] > _XDEF_DEFALTPARAMETER_3 )
			{
				m_Parameter[2]--;
				pCreateCharacter_Window->m_Parameter[2] = m_Parameter[2];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_ADDBUTON4 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[3]++;
				pCreateCharacter_Window->m_Parameter[3] = m_Parameter[3];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON4 :
			if( m_Parameter[3] > _XDEF_DEFALTPARAMETER_4 )
			{
				m_Parameter[3]--;
				pCreateCharacter_Window->m_Parameter[3] = m_Parameter[3];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_ADDBUTON5 :
			if( pCreateCharacter_Window->m_BonusPoint > 0 )
			{
				m_Parameter[4]++;
				pCreateCharacter_Window->m_Parameter[4] = m_Parameter[4];
				pCreateCharacter_Window->m_BonusPoint--;
			}
			break;
		case _XDEF_CC_SUBBUTON5 :
			if( m_Parameter[4] > _XDEF_DEFALTPARAMETER_5 )
			{
				m_Parameter[4]--;
				pCreateCharacter_Window->m_Parameter[4] = m_Parameter[4];
				pCreateCharacter_Window->m_BonusPoint++;
			}
			break;
		case _XDEF_CC_AREACHECKBUTTON1 :
		case _XDEF_CC_AREACHECKBUTTON2 :
		case _XDEF_CC_AREACHECKBUTTON3 :
		case _XDEF_CC_AREACHECKBUTTON4 :
		case _XDEF_CC_AREACHECKBUTTON5 :
		case _XDEF_CC_AREACHECKBUTTON6 :
		case _XDEF_CC_AREACHECKBUTTON7 :
		case _XDEF_CC_AREACHECKBUTTON8 :
		case _XDEF_CC_AREACHECKBUTTON9 :
			{
				m_SelectedZone = (wparam - _XDEF_CC_AREACHECKBUTTON1) + 1;

				pCreateCharacter_Window->m_pAreaSelectCheckButton[0]->SetCheck( (m_SelectedZone) == 1 );
				pCreateCharacter_Window->m_pAreaSelectCheckButton[1]->SetCheck( (m_SelectedZone) == 2 );
				pCreateCharacter_Window->m_pAreaSelectCheckButton[2]->SetCheck( (m_SelectedZone) == 3 );
				pCreateCharacter_Window->m_pAreaSelectCheckButton[3]->SetCheck( (m_SelectedZone) == 4 );
			}
			break;		
		default:
			break;
		}
	}
}
	
bool XProc_DeleteCharacter::OnKeyboardPoll(_XInput *pInput)
{ 	
#ifdef _XDWDEBUG
	BOOL keypressed = FALSE;
	FLOAT direction = 0.0f;
	
	if( !_XIMEKERNEL.GetIMEContainer() ) 
	{
		
	}
#endif
	return true; 
}

bool XProc_DeleteCharacter::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
	return true;
}

bool XProc_DeleteCharacter::OnKeyDown(WPARAM wparam, LPARAM lparam)
{ 
	if( (U32)wparam == VK_RETURN )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CC_CREATEBUTTON, 0), (LPARAM)gHWnd);
	}	
	else if( (U32)wparam == VK_ESCAPE )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CC_CANCELBUTTON, 0), (LPARAM)gHWnd);
	}
	return true; 
}

int m_CameraOffsetX;
int m_CameraOffsetY;
POINT m_CameraDragPos;

bool XProc_DeleteCharacter::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{ 		
	// Get the pick ray from the mouse position	
	POINT mousept = { pPos->x, pPos->z };
	
	//if( !::g_CurrentFocusedObject || ::g_CurrentFocusedObject != _XDEF_DUMMYID_GAMEBACKGROUND )
	{	
		float fmx = pState->AX*0.6;
		float fmy = pState->AY*0.6;

		m_CameraOffsetX = pPos->x - m_CameraDragPos.x;
		m_CameraOffsetY = pPos->z - m_CameraDragPos.y;
		m_CameraDragPos.x = pPos->x;
		m_CameraDragPos.y = pPos->z;
		
		if( pState->bButton[1] )
		{	
			m_PreviewCharacterRotateAngle -= fmx;
			D3DXMatrixRotationY( &m_PreviewCharacter.m_Position, _X_RAD(m_PreviewCharacterRotateAngle) );
		}		
#ifdef _XDWDEBUG
		if( pState->bButton[2] )
		{
			D3DXVECTOR3 targetpos;
			D3DXMATRIX  matOrientation;			
			targetpos.x = -(m_CameraOffsetX*0.025f);
			targetpos.y =  (m_CameraOffsetY*0.025f);
			targetpos.z = 0.0f;			
			D3DXMatrixInverse( &matOrientation, NULL, &g_LodTerrain.m_3PCamera.GetViewMatrix() );
			D3DXVec3TransformNormal( &targetpos, &targetpos, &matOrientation );						
			g_LodTerrain.m_3PCamera.SetTargetPosition( g_LodTerrain.m_3PCamera.m_TargetPosition + targetpos );
			g_LodTerrain.m_3PCamera.UpdateViewMatrix();
			g_LodTerrain.m_3PCamera.UpdateProjMatrix();
		}
#endif
	}
	return true; 
}

void XProc_DeleteCharacter::OnMouseWheel( short zDelta )
{
	if( !m_WindowManager.ProcessWheelMessage(zDelta) )
	{
#ifdef _XADMINISTRATORMODE
		FLOAT MoveDelta  = (FLOAT)zDelta*0.005F;

		if( gpInput->CheckKeyPress( DIK_LCONTROL ) ) MoveDelta *= _XDEF_RAPID_RATE;
		g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - MoveDelta );
		g_LodTerrain.m_3PCamera.UpdateViewMatrix();
#endif
	}
}

bool XProc_DeleteCharacter::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{
	POINT pt;
	GetCursorPos( &pt );
	ScreenToClient( gHWnd, &pt );	

	switch( p_MB )
	{
	case _XmbLeft :
		if( bPushed )
		{
		}
		break;
	case _XmbRight :
		if( bPushed )
		{
			
		}
		
		break;
	case _XmbMiddle :		
		
		break;
	}

	return true; 
}	

void XProc_DeleteCharacter::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{		
		g_NetworkKernel.ProcessNetworkError();
		return;
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{	
	case FD_READ :
		g_NetworkKernel.ReadSocket();
		break;
	case FD_CLOSE :
		//_XDWINPRINT( "SERVER MESSAGE : Socket close message" );
		g_NetworkKernel.ProcessNetworkError();
		g_NetworkKernel.DisconnectServer();
		g_NetworkKernel.ReleaseNetwork();
		
		((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_PrevProcess_SelectCharacter = TRUE;
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( 
			&((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver );		
		
		break;
	}
}

void XProc_DeleteCharacter::OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam )
{
	if( g_BgmPlayFlag )
	{		
		
	}
}

void XProc_DeleteCharacter::OnWindowSizeChangeNotify( void )
{	
	g_MessageBox.MoveWindow((gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1), (gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1) - 80 );
}

void XProc_DeleteCharacter::SetCharacter( void )
{			
	int extravalue = 0;
	
	m_PreviewModelDescript.lhand		=	0;
	m_PreviewModelDescript.rhand		=	0;
		
	if( m_CharacterGender == _XGENDER_WOMAN )
	{	
		m_PreviewModelDescript.upperbody	=	g_Modeltype_UpperBody[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.lowerbody	=	g_Modeltype_LowerBody[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.adae			=   g_Modeltype_Hand[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.lfoot		=	g_Modeltype_Foot[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.rfoot		=	g_Modeltype_Foot[m_CharacterGender][m_ClothTypeWomanIndex];
		m_PreviewModelDescript.hair			=	m_HairTypeWomanIndex;
		m_PreviewModelDescript.face			=	m_FaceTypeWomanIndex;

		m_PreviewCharacter.LinkBipedObject( &g_FemaleBipedObject );		
	}
	else 
	{
		m_PreviewModelDescript.upperbody	=	g_Modeltype_UpperBody[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.lowerbody	=	g_Modeltype_LowerBody[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.adae			=   g_Modeltype_Hand[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.lfoot		=	g_Modeltype_Foot[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.rfoot		=	g_Modeltype_Foot[m_CharacterGender][m_ClothTypeManIndex];
		m_PreviewModelDescript.hair			=	m_HairTypeManIndex;				
		m_PreviewModelDescript.face			=	m_FaceTypeManIndex;

		m_PreviewCharacter.LinkBipedObject( &g_MaleBipedObject );
	}

	m_PreviewCharacter.SetMeshModel( m_PreviewModelDescript, m_CharacterGender, _XGROUP_NANGIN );
}

void XProc_DeleteCharacter::DrawFadeInOut(void)
{
	D3DCOLOR color = ((DWORD) g_FadeInOutLevel << 24);
	
	g_ScreenBurnOut[0].color = color;
	g_ScreenBurnOut[1].color = color;
	g_ScreenBurnOut[2].color = color;
	g_ScreenBurnOut[3].color = color;
	
	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	gpDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	gpDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
	gpDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	
	gpDev->SetTexture(0, NULL);
	
	gpDev->SetFVF(D3DFVF_XTLVERTEX);
	gpDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX));
	
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	gpDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	gpDev->SetRenderState(D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog);
}
// XProc_NineDragonsTitle.cpp: implementation of the XProc_NineDragonsTitle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"

#include "XProc_NineDragonsTitle.h"
#include "XProc_ProcessDefine.h"

#include "FlashControlInterface.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CFlashControlInterface*	g_pTitleFlashObject;
extern LPDIRECT3DTEXTURE9		g_pTitleFlashOffScreenTexture;

XProc_NineDragonsTitle::XProc_NineDragonsTitle()
{
	setProcessInfo( 0, "Nine dragons title", __FILE__ );
	m_TitlePhase = _XNDTitlePhase_Indy21;
}

XProc_NineDragonsTitle::~XProc_NineDragonsTitle()
{

}

BOOL XProc_NineDragonsTitle::InitializeProcess( void )
{	
	g_NormalGameExit = TRUE;
	
	m_FlashStatic.Create( 0, 0, gnWidth, gnHeight, NULL, -1 );

	if( !CreateOffScreen() )
	{
		return FALSE;
	}

	if (g_pTitleFlashObject == NULL)
	{
		g_pTitleFlashObject = GetFlashControl();
		
		if( !g_pTitleFlashObject ) return FALSE;

		g_pTitleFlashObject->initialize( gHWnd,gHInst,g_pTitleFlashOffScreenTexture );
		g_pTitleFlashObject->SetLowQuality();
	}
		
	HDC pDC = GetDC( gHWnd );
	g_pTitleFlashObject->SetDC( pDC );

	TCHAR flashfilename[64];
	
	if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
	{			
		strcpy( flashfilename, _T("TITLE00.swf") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( flashfilename, _T("vn_TITLE00.swf") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		//strcpy( flashfilename, _T("tw_TITLE00.swf") );
		strcpy( flashfilename, _T("tw_TITLE00.swf") );
	}
	else
	{
		strcpy( flashfilename, _T("TITLE00.swf") );
	}

	_XPackageArchive package;
	
	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Script\\") );
#ifdef _XTESTSERVER				
	strcat( packagefilename, _T("tsXSCENESCRIPT.XP") );
#else
	strcat( packagefilename, _T("XSCENESCRIPT.XP") );	
#endif

	package.OpenPackage( _T("XSCENESCRIPT.XP") );

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

	g_ReadyMouseCursorMode = _XMOUSETYPE_PEACE;
	((CEmperorOfDragonsApp*)gApp)->SetCursor( g_ReadyMouseCursorMode );
	
	m_ProcessEndTimer = 0;
	
	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// 프로세스 종료
void XProc_NineDragonsTitle::DestroyProcess( void )
{
	SetEnableProcess( FALSE );
}

//////////////////////////////////////////////////////////////////////////////////
// Initialize device defendent object
bool XProc_NineDragonsTitle::InitDeviceObject( void )
{		
	// Set the camera view and projection matrices	
	m_Player.Set(m_Player.GetType(), 60, (float)gnWidth/(float)gnHeight, 0.001f, 10.0f);
	m_Player.UpdateViewMatrix();
	m_Player.UpdateProjMatrix();
	
	D3DVIEWPORT9 vp;
	vp.X = vp.Y = 0;
	vp.Width = gnWidth;
	vp.Height = gnHeight;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	gpDev->SetViewport(&vp);
	
	// Set matrices
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_Player.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&m_Player.GetProjMatrix());
	
	// Wall texture just point mip mapping with linear filtering
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
	
	gpDev->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( 1.0, 1.0, 1.0, 1.0 ) );
	
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1  );
	
	gpDev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	
	// Alpha blend state
	gpDev->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE );
	gpDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE );
	gpDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );		
	gpDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	
	// Alpha test state
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );
	gpDev->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );
	
	D3DLIGHT9	light;
	D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 0.0f, 160.0f, 0.0f );
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 0.5f;
	light.Range = 100.0f;
	gpDev->SetLight( 0, &light );
	
	gpDev->SetRenderState( D3DRS_LIGHTING, TRUE );	
	gpDev->LightEnable( 0, TRUE );	
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	CreateOffScreen();
	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );
	}
					
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Clear device defendent object
void XProc_NineDragonsTitle::ReleaseDeviceObject( void )
{		
	SAFE_RELEASE( g_pTitleFlashOffScreenTexture );

	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->SetOffScreenTextureObject( NULL );
	}
}

void XProc_NineDragonsTitle::Draw( void )
{	
	if(g_pTitleFlashObject)
		g_pTitleFlashObject->PlayTimerFire();

	FlashDraw();
}

bool XProc_NineDragonsTitle::Process( void )
{	
	if( m_TitlePhase == _XNDTitlePhase_TripleXLogo )
	{
		DWORD elapsedtime = g_LocalSystemTime - m_ProcessEndTimer;
		if( elapsedtime > 2000 )
		{
			m_FlashStatic.m_FColor = D3DCOLOR_ARGB( (BYTE)(((FLOAT)(2500 - elapsedtime) / 500.0f) * 255.0f), 255, 255, 255 );

			if( elapsedtime > 2500 )
			{
				g_pTitleFlashObject->FreeBuffer();
				g_pTitleFlashObject->ControlClose();			
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_firstload);
			}
		}
	}

	if( g_pTitleFlashObject && !g_pTitleFlashObject->IsPlaying() )
	{
		if( m_TitlePhase == _XNDTitlePhase_Indy21 )
		{
			m_TitlePhase = _XNDTitlePhase_NineDragonsTitle;
			g_pTitleFlashObject->FreeBuffer();
			g_pTitleFlashObject->ControlClose();
									
			HDC pDC = GetDC( gHWnd );
			g_pTitleFlashObject->SetDC( pDC );
			
			_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );

			TCHAR flashfilename[64];
			if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
			{
				strcpy( flashfilename, _T("tw_TITLE00_1.swf") );
			}
			else
			{
				strcpy( flashfilename, _T("TITLE00_1.swf") );
			}
			
			_XPackageArchive package;

			TCHAR packagefilename[_MAX_PATH];
			packagefilename[0] = NULL;
			strcpy( packagefilename, gModulePath );
			strcat( packagefilename, _T("\\Data\\Script\\") );

#ifdef _XTESTSERVER				
			strcat( packagefilename, _T("tsXSCENESCRIPT.XP") );			
#else
			strcat( packagefilename, _T("XSCENESCRIPT.XP") );			
#endif

			if( !package.OpenPackage( packagefilename ) )
			{
				return false;
			}

			int resourceid = package.FindResource( flashfilename );
			if( resourceid >= 0 )
			{
				long filesize = package.GetPackedFileSize( resourceid );
				g_pTitleFlashObject->SetResourceFilePtr( package.GetPackedFile( resourceid ), filesize );
				g_pTitleFlashObject->ControlOpen( flashfilename );
				g_pTitleFlashObject->SetLowQuality();
			}
			package.ClosePackage();
			
			g_pTitleFlashObject->SetDC( 0 );
			ReleaseDC( gHWnd, pDC );
			
			m_TitlePhase = _XNDTitlePhase_TripleXLogo;
			m_ProcessEndTimer = g_LocalSystemTime;
		}
		else if( m_TitlePhase == _XNDTitlePhase_TripleXLogo )
		{
			g_pTitleFlashObject->FreeBuffer();
			g_pTitleFlashObject->ControlClose();

			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_firstload);
			
			/*
			HDC pDC = GetDC( gHWnd );
			g_pTitleFlashObject->SetDC( pDC );
			
			_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
			
			TCHAR flashfilename[64] = _T("DELETE00.swf");
			
			_XPackageArchive package;
#ifdef _XTESTSERVER				
			package.Open( _T("tsXSCENESCRIPT.XP") );
#else
			package.Open( _T("XSCENESCRIPT.XP") );
#endif
			g_pTitleFlashObject->SetResourceFilePtr( package.GetPackedFile( flashfilename ) );
			g_pTitleFlashObject->ControlOpen( flashfilename );
			g_pTitleFlashObject->SetLowQuality();
			package.Close();
			
			g_pTitleFlashObject->SetDC( 0 );
			ReleaseDC( gHWnd, pDC );
			
			m_TitlePhase = _XNDTitlePhase_NineDragonsTitle;
			*/
		}
		/*
		else if( m_TitlePhase == _XNDTitlePhase_NineDragonsTitle )
		{
			g_pTitleFlashObject->FreeBuffer();
			g_pTitleFlashObject->ControlClose();
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_firstload);
		}
		*/
	}		

	return true;
}

void XProc_NineDragonsTitle::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	if( message == WM_COMMAND )
	{
		//switch( wparam )
		{

		}		
	}
}

bool XProc_NineDragonsTitle::OnKeyboardPoll(_XInput *pInput)
{ 	
	return true; 
}

bool XProc_NineDragonsTitle::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
	return true;
}

bool XProc_NineDragonsTitle::OnKeyDown(WPARAM wparam, LPARAM lparam)
{ 	
	if( m_TitlePhase == _XNDTitlePhase_Indy21 )	
	{
		g_pTitleFlashObject->FreeBuffer();
		g_pTitleFlashObject->ControlClose();

		HDC pDC = GetDC( gHWnd );
		g_pTitleFlashObject->SetDC( pDC );
		
		_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
		
		TCHAR flashfilename[64];
		if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		{
			strcpy( flashfilename, _T("tw_TITLE00_1.swf") );
		}
		else
		{
			strcpy( flashfilename, _T("TITLE00_1.swf") );
		}
		
		_XPackageArchive package;
		TCHAR packagefilename[_MAX_PATH];
		packagefilename[0] = NULL;
		strcpy( packagefilename, gModulePath );
		strcat( packagefilename, _T("\\Data\\Script\\") );

#ifdef _XTESTSERVER				
		strcat( packagefilename, _T("tsXSCENESCRIPT.XP") );			
#else
		strcat( packagefilename, _T("XSCENESCRIPT.XP") );			
#endif
		if( !package.OpenPackage( packagefilename ) )
		{
			return false;
		}

		int resourceid = package.FindResource( flashfilename );
		if( resourceid >= 0 )
		{
			long filesize = package.GetPackedFileSize( resourceid );
			g_pTitleFlashObject->SetResourceFilePtr( package.GetPackedFile( resourceid ), filesize );		
			g_pTitleFlashObject->ControlOpen( flashfilename );
		}
		package.FinalizePackage();
		
		g_pTitleFlashObject->SetDC( 0 );
		ReleaseDC( gHWnd, pDC );

		m_ProcessEndTimer = g_LocalSystemTime;
		m_TitlePhase = _XNDTitlePhase_TripleXLogo;
	}
	else if( m_TitlePhase == _XNDTitlePhase_TripleXLogo )	
	{

	}

	return true; 
}

bool XProc_NineDragonsTitle::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{ 					
	return true; 
}

bool XProc_NineDragonsTitle::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	switch(p_MB) {
	case _XmbLeft:
		{
			if( bPushed )//Left Button Down
			{				
			}
			else//Left Button Up
			{
			}
		}
		break;
	}
	return TRUE; 
}	

void XProc_NineDragonsTitle::OnMouseMove( WPARAM wparam, LPARAM lparam )
{	
	int fwKeys = wparam;        // key flags 
	int xPos = LOWORD(lparam);  // horizontal position of cursor 
	int yPos = HIWORD(lparam);  // vertical position of cursor 
}

void XProc_NineDragonsTitle::FlashDraw( void )
{
	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	
	// Begin the scene
	gpDev->BeginScene();
	
	m_FlashStatic.DrawWithRegion( g_pTitleFlashOffScreenTexture );	
	
#ifdef _XDWDEBUG 
	//g_XBaseFont->SetColor( _XSC_DEFAULT );
	//g_XBaseFont->PrintAlign( 0, 0,1.0f, _XFONT_ALIGNTYPE_LEFT, "FPS:%d", gnFPS);
	//g_XBaseFont->Flush();	
#endif
				
	g_MessageBox.Draw();
	
	// End the scene
	gpDev->EndScene();
	
	// Present the backbuffer contents to the display
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr ) gApp->m_bDeviceLost = true;
}

void XProc_NineDragonsTitle::OnTimer(WPARAM wparam, LPARAM lparam)
{
	/*
	int wmId;
	wmId = wparam;             // timer identifier 
	if ( wmId == TIMER_PLAY )
		g_pTitleFlashObject->PlayTimerFire();
	else if ( wmId == TIMER_CURSOR )
		g_pTitleFlashObject->CursorTimerFire();
	
	FlashDraw();
	*/	
}

void XProc_NineDragonsTitle::OnPaint(WPARAM wparam, LPARAM lparam)
{
	/*
	if (g_pTitleFlashObject == NULL)
	{
		CreateOffScreen();
		g_pTitleFlashObject = GetFlashControl();
		g_pTitleFlashObject->initialize( gHWnd,gHInst,g_pTitleFlashOffScreenTexture );
		//g_pTitleFlashObject->SetLowQuality();
		return;
	}
	
	RECT rc; 
	GetClientRect( gHWnd, &rc);	
	g_pTitleFlashObject->Repaint( rc);
	
	FlashDraw();
	*/
	Draw();
}

BOOL XProc_NineDragonsTitle::CreateOffScreen( void )
{
	if( g_pTitleFlashOffScreenTexture ) return TRUE;
	
	SIZE size = m_FlashStatic.GetWindowSize();
	
	if( FAILED( gpDev->CreateTexture( size.cx, size.cy, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTitleFlashOffScreenTexture, NULL ) ) )
    {
        return FALSE;
    }
	
	return TRUE;
}
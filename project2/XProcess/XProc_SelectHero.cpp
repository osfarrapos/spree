// XProc_SelectHero.cpp: implementation of the XProc_SelectHero class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_SelectHero.h"
#include "XProc_ZeroLevel.h"

#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XProc_SelectHero::XProc_SelectHero()
{
	setProcessInfo( 0, "Select hero", __FILE__ );
}

XProc_SelectHero::~XProc_SelectHero()
{

}

BOOL XProc_SelectHero::InitializeProcess(void)
{
	m_Player.SetPitchLimits(-45, 45);
	m_Player.SetRollLimits(-10, 10);
	m_Player.SetType(camFly);

	m_TextureArchive.DisposeTexture();	

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_INTERFACE.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_INTERFACE.XP") );
#endif
	
	m_TextureArchive.SetTexturePackage( packagefilename );

	m_TextureArchive.GetResourceIndex("HeroSelect.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_soora.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_fist.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_bull.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_noklim.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_namgoong.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_hosansan.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_devil.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_sword.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("hero_lightning.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("zerolevel_startbutton.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);

	m_TextureArchive.BuildTextureList(TRUE);

	m_iDrawPosX = gnWidth - 1024;
	m_iDrawPosY = gnHeight - 768;

	int tmpimageindex = m_TextureArchive.FindResource("HeroSelect.tga");

	// 영웅 순서 : 마후 - 철권 - 불륜 - 녹림 - 남궁 - 호산 - 천마 - 능운 - 뇌제 
	m_HeroCharacterImage[0].Create( m_iDrawPosX+788, m_iDrawPosY+293, m_iDrawPosX+788+183, m_iDrawPosY+293+324, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[0].SetClipRect( 260, 20, 443, 344 );
	m_HeroCharacterImage[0].m_FColor = 0xff888888;
	
	m_HeroNameImage[0].Create( 0, 0, 255, 255, &m_TextureArchive,m_TextureArchive.FindResource("hero_soora.tga") );
	m_HeroNameImage[0].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[1].Create( m_iDrawPosX+622, m_iDrawPosY+227, m_iDrawPosX+622+226, m_iDrawPosY+227+390, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[1].SetClipRect( 0, 0, 226, 390 );
	m_HeroCharacterImage[1].m_FColor = 0xff888888;
	m_HeroNameImage[1].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_fist.tga") );
	m_HeroNameImage[1].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[2].Create( m_iDrawPosX+369, m_iDrawPosY+375, m_iDrawPosX+369+250, m_iDrawPosY+375+242, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[2].SetClipRect( 202, 382, 452, 624 );
	m_HeroCharacterImage[2].m_FColor = 0xff888888;
	m_HeroNameImage[2].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_bull.tga") );
	m_HeroNameImage[2].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[3].Create( m_iDrawPosX+204, m_iDrawPosY+401, m_iDrawPosX+204+198, m_iDrawPosY+401+215, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[3].SetClipRect( 0, 415, 198, 630 );
	m_HeroCharacterImage[3].m_FColor = 0xff888888;
	m_HeroNameImage[3].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_noklim.tga") );
	m_HeroNameImage[3].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[4].Create( m_iDrawPosX+720, m_iDrawPosY+141, m_iDrawPosX+720+292, m_iDrawPosY+141+445, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[4].SetClipRect( 731, 578, 1023, 1023 );
	m_HeroCharacterImage[4].m_FColor = 0xff888888;
	m_HeroNameImage[4].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_namgoong.tga") );
	m_HeroNameImage[4].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[5].Create( m_iDrawPosX+549, m_iDrawPosY+262, m_iDrawPosX+549+266, m_iDrawPosY+262+355, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[5].SetClipRect( 0, 668, 266, 1023 );
	m_HeroCharacterImage[5].m_FColor = 0xff888888;
	m_HeroNameImage[5].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_hosansan.tga") );
	m_HeroNameImage[5].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[6].Create( m_iDrawPosX+318, m_iDrawPosY+233, m_iDrawPosX+318+338, m_iDrawPosY+233+383, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[6].SetClipRect( 281, 640, 619, 1023 );
	m_HeroCharacterImage[6].m_FColor = 0xff888888;
	m_HeroNameImage[6].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_devil.tga") );
	m_HeroNameImage[6].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[7].Create( m_iDrawPosX+383, m_iDrawPosY+115, m_iDrawPosX+383+304, m_iDrawPosY+115+494, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[7].SetClipRect( 719, 0, 1023, 494 );	
	m_HeroCharacterImage[7].m_FColor = 0xff888888;
	m_HeroNameImage[7].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_sword.tga") );
	m_HeroNameImage[7].SetClipRect(0, 0, 255, 255);

	m_HeroCharacterImage[8].Create( m_iDrawPosX+633, m_iDrawPosY+61, m_iDrawPosX+633+210, m_iDrawPosY+61+552, 
									&m_TextureArchive, tmpimageindex );
	m_HeroCharacterImage[8].SetClipRect( 494, 0, 704, 552 );	
	m_HeroCharacterImage[8].m_FColor = 0xff888888;
	m_HeroNameImage[8].Create( 0, 0, 255, 255, &m_TextureArchive, m_TextureArchive.FindResource("hero_lightning.tga") );
	m_HeroNameImage[8].SetClipRect(0, 0, 255, 255);
	
	int btnimageindex = m_TextureArchive.FindResource( "zerolevel_startbutton.tga" );
	_XBTN_STRUCTURE startbtnstruct = { TRUE, { m_iDrawPosX+20, m_iDrawPosY+210 }, { 100, 30 }, _XDEF_ZEROLEVEL_START,
		btnimageindex,btnimageindex,btnimageindex, &m_TextureArchive };
	m_StartButton = new _XButton;
	m_StartButton->Create( startbtnstruct );
	m_StartButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 0,99,29 );
	m_StartButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0,60,99,89 );
	m_StartButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0,30,99,59 );
	m_StartButton->AttachXFont(_XFONTID_LARGE);	
	m_StartButton->SetButtonTextColor(_XSC_DEFAULT_HIGHLIGHT);
	m_StartButton->SetButtonTextID( 0,0 );	// 게임시작
	

	m_StartButton->EnableWindow( FALSE );
	m_StartButton->ShowWindow( FALSE );
	
	m_iSelectedHero = -1;

	InitDeviceObject();

	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );

	return TRUE;
}

void XProc_SelectHero::DestroyProcess( void )
{
	SetEnableProcess( FALSE );

	m_TextureArchive.DisposeTexture();
}

bool XProc_SelectHero::InitDeviceObject( void )
{
	m_TextureArchive.InitDeviceObject();

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
	
	D3DLIGHT9	light;
	D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, 0.0f, 160.0f, 0.0f );
	light.Diffuse.r = light.Diffuse.g = light.Diffuse.b = 0.5f;
	light.Range = 100.0f;
	gpDev->SetLight( 0, &light );

	gpDev->SetRenderState( D3DRS_LIGHTING, TRUE );	
	gpDev->LightEnable( 0, TRUE );	

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	return true;
}

void XProc_SelectHero::ReleaseDeviceObject( void )
{

}

void XProc_SelectHero::PauseProcess( void )
{

}
void XProc_SelectHero::UnPauseProcess( void )
{

}

void XProc_SelectHero::Draw( void )
{
	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	
	// Begin the scene
	gpDev->BeginScene();

	m_Player.UpdateViewMatrix();
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_Player.GetViewMatrix());
	
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 5 );

	for( int i = 8; i >= 0; i-- )
	{
		m_HeroCharacterImage[i].DrawWithRegion();
		m_HeroCharacterImage[i].m_FColor = 0xff888888;
	}	
	if( m_iSelectedHero > -1 )
	{
		m_HeroNameImage[m_iSelectedHero].DrawWithRegion();
		DrawHeroSpeech(m_iSelectedHero);
		m_StartButton->Draw();
	}
	
	// End the scene
	gpDev->EndScene();
	
	// Present the backbuffer contents to the display	
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true;

}

bool XProc_SelectHero::Process( void )
{
	static bool flag = true;
	m_StartButton->Process();

	for( int i = 0; i < 9; i++ )
	{
		if( m_HeroCharacterImage[i].CheckMousePosition() )
		{
			m_HeroCharacterImage[i].m_FColor = 0xffffffff;
			if( gpInput->GetMouseState()->bButton[0] && flag )
			{
				flag = false;
				m_iSelectedHero = i;
				if( m_iSelectedHero == 5 || m_iSelectedHero == 3 || m_iSelectedHero == 2 || m_iSelectedHero == 0 )
					m_StartButton->EnableWindow( TRUE );
				else
					m_StartButton->EnableWindow( FALSE );
				m_StartButton->ShowWindow( TRUE );
			}
			break;
		}
	}
	if( !gpInput->GetMouseState()->bButton[0] && !flag )
		flag = true;

	if( m_iSelectedHero > -1 )
		m_HeroCharacterImage[m_iSelectedHero].m_FColor = 0xffffffff;

	if( m_StartButton->CheckMousePosition() && gpInput->GetMouseState()->bButton[0] )
	{
		XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
		switch(m_iSelectedHero) 
		{
		case 0 : // 수라마후
			{
				pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_BEEGOONG);
				pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_SORIM);
				pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_WOMAN);
			}
			break;
		case 1 : // 철권 군유명
			break;
		case 2 : // 불륜화상
			{
				pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_SORIM);
				pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_BEEGOONG);
				pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_MAN);				
			}
			break;
		case 3 : // 녹림왕
			{
				pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_NOCKRIM);
				pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_GAEBANG);
				pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_MAN);				
			}			
			break;
		case 4 : // 남궁휘
			break;
		case 5 : // 호산산
			{
				pZeroLevelProcess->SetHeroGroupIndex(_XGROUP_GAEBANG);
				pZeroLevelProcess->SetEnemyGroupIndex(_XGROUP_NOCKRIM);
				pZeroLevelProcess->SetHeroCharacterGender(_XGENDER_WOMAN);				
			}
			break;
		case 6 : // 천마대제
			break;
		case 7 : // 능운도장
			break;
		case 8 : // 뇌제
			break;
		}	
		
		((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel );	
	}

	return true;	
}

void XProc_SelectHero::DrawHeroSpeech(int select)
{
	TCHAR szText[256];
	switch(select) 
	{
	case 0:
		{
			sprintf(szText, "%s", _T("살아있다는 건 저주야. 지옥의 칼날 위를 한 발 한 발 걸어가는 거지. 세상에 좋은 일이란 하나도 없어.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("아니, 딱 하나 있지.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("삶은 언젠가 끝나고 누구에게나 공평하게 죽음이 온다는 거야. 안식처럼 편안한 죽음이...") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();	
		}
		break;
	case 1:
		{
			sprintf(szText, "%s", _T("사람들은 운명의 힘에 밀려 표류하듯 살아간다. 나도 한때는 그랬다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("그러나 절망의 문턱에서 나는 깨달았다. 운명을 만드는 것은 의지와 욕망이라는 것을...") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("나는 이제 내 운명의 목을 잡아 비틀어 무림제패의 길로 향한다. 이 두 주먹으로!") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 2:
		{
			sprintf(szText, "%s", _T("옴 살바 못자모치 사다야 사바하. 옴 살바 못자모치 사다야 사바하. 옴 살바 못자모치...") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("용서하라. 나는 오늘 살계를 깰 수밖에 없으니.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("내가 먼저 지옥에 뛰어들지 않고서 어떻게 중생을 구하랴.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 3:
		{
			sprintf(szText, "%s", _T("주저앉아 운다고 해결되는 게 있나? 하늘을 원망해서 좋아지는 건 있어?") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("다 하기 나름인거야.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("어떤 처지에 떨어지건 최선을 다해야지. 그게 인생이야.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 4:
		{
			sprintf(szText, "%s", _T("가문 좋고 머리 좋고, 싸움도 잘하고, 잘 생기고, 말 잘하고, 예의도 바르지.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("그게 바로 나야.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("한가지 약점이 있다면... 없군.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 5:
		{
			sprintf(szText, "%s", _T("때로는 서러울지도 모르죠. 때로는 힘들지도 모르죠.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			
			sprintf(szText, "%s", _T("어쩌면 내면에 가라앉아 있는 자신도 모를 외로움이 있을지도 모르죠.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			
			sprintf(szText, "%s", _T("하지만 인생 별 거 있나요? 웃으며 삽시다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 6:
		{
			sprintf(szText, "%s", _T("세상은 불공평하고 그것을 바로잡아줄 신도 부처도 없다. 지지 않고 살아가기 위해서는 자신이 신이 되는 수밖에 없다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			g_XBaseFont->Flush();
			
			sprintf(szText, "%s", _T("나는 신이고 부처다. 나는 천마다. 천마는 군림의 길을 걸을 수밖에 없다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			g_XBaseFont->Flush();	

			sprintf(szText, "%s", _T("천마군림, 그게 나다. 그리고 그게 나의 길이다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 7:
		{
			sprintf(szText, "%s", _T("태극이 그리는 오묘한 진리. 한 자루 검이 토해내는 푸르른 울음소리.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			g_XBaseFont->Flush();
			
			sprintf(szText, "%s", _T("그리고..... 달빛 아래의 그림자.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			g_XBaseFont->Flush();	

			sprintf(szText, "%s", _T("내 모든 것이 이 셋 안에 있다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;
	case 8:
		{
			sprintf(szText, "%s", _T("세상은 승리한 자를 중심으로 움직인다. 도덕과 윤리는 패자의 변명에 불과하다. 승자의 뜻이 곧 법인 것이다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630, szText );
			g_XBaseFont->Flush();
			
			sprintf(szText, "%s", _T("나는 항상 승자가 되길 원한다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+15, szText );
			g_XBaseFont->Flush();	

			sprintf(szText, "%s", _T("그러므로 나는 궁극의 강함을 추구한다.") );
			g_XBaseFont->SetColor( 0xffffffff );
			g_XBaseFont->Puts( m_iDrawPosX + 20, m_iDrawPosY + 630+30, szText );
			g_XBaseFont->Flush();
		}
		break;	
	}
}
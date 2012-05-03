// XProc_LoginServer.cpp: implementation of the XProc_LoginServer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XWindow_SelectServerForest.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_Login.h"
#include "XProc_ProcessDefine.h"
#include "XProc_LoginServer.h"
#include "FlashControlInterface.h"
#include "SoundEffectList.h"
#include "XWindow_TestServerNotice.h"
#include "XWindow_DrawKeyboard.h"
#include "XWindow_QuickSlot.h"
#include "Request.h"
#include "XMD5Global.h"
#include "XMD5.h"

///////////////////////////////////////////////////////////////////////
// Pre.. define

CFlashControlInterface*	g_pTitleFlashObject = NULL;
LPDIRECT3DTEXTURE9		g_pTitleFlashOffScreenTexture;

#define _XDEF_DUMMYID_SELECTSERVERBUTTON	((_XGUIObject *)0xFED)

extern BOOL _XIsIPv4Address(char *pHost, int nLen);
extern BOOL _XGetHostName( LPCTSTR pHostName, LPTSTR pIPAdress, int _length );

char   g_szAcclaimID[64] = _T("");
char   g_szAcclaimPW[64] = _T("");

BOOL   g_OnetimeAutoLogin = TRUE;

#pragma pack( push, enter_game )
#pragma pack(1)

struct _ForestInfo {
	int		nVersion;	
	bool	bServerRun;		
	char	cForestState[_COUNT_MAX_GROUP_] ;		//	-2, 점검중 -1 : 오프중 ,  0~100 정상
};

#define MSG_NO_LOGIN_FOREST_LIST		5			// ps 에 접속하면 이 패킷을 보내주고 연결을 끊는다.

struct MSG_LOGIN_FOREST_LIST
{	
	_ForestInfo			ForestInfo ;				// 서버군 정보 & 버젼 정보
};

#pragma pack( pop, enter_game )


BOOL CheckInnerTester( void )
{
	TCHAR	LicenseCode[_REGSTRING_MAXLENGTH];
	memset( LicenseCode, 0, _REGSTRING_MAXLENGTH );
	g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LICENSECODE, "", LicenseCode, _REGSTRING_MAXLENGTH, FALSE);
	
	if( strlen( LicenseCode ) <= 0 )
	{
		return FALSE;
	}
	
	_XEncryptor encription;
	encription.setKey( 10, 31, "9E2GH4FWDWKC2MBL4EBNKUV63T" );
	
	if( encription.checksumSerial( LicenseCode ) == TRUE )
	{
		return TRUE;
	}
	
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XProc_LoginServer::XProc_LoginServer()
{		
	setProcessInfo( 0, "Login server", __FILE__ );
	m_PrevProcess_SelectCharacter = FALSE;
	m_MaxServerCount = 0;
	m_OneTimeNoticeViewFlag = TRUE;
	
#ifdef _X_US_EXPANDSERVERLIST
	m_LastSelectedRegion_Server = -1;
#endif

#ifdef _XNEXON_WEBLOGIN	
	m_ShowOneTimeAutologinServerListViewDelayTimer = 0;
#endif
}

XProc_LoginServer::~XProc_LoginServer()
{

}


BOOL XProc_LoginServer::InitializeProcess( void )
{		
	_XWindow::m_GlobalPlaySoundEffect = FALSE;

 	g_NormalGameExit = TRUE;	
	m_ConnectMode	 =	_XDEF_SERVERCONNECTMODE_LOGIN;

	g_CurrentFocusedObject = NULL;

	g_InnerUser = CheckInnerTester();

	m_AlphaValue = 0; 

	m_GraphUpdateTimer = 0;

	m_LastUpdateServerListTime = 0;
	memset( m_ServerTrafficFactor, 0, sizeof(_XSERVERTRAFFICDATA) * _COUNT_MAX_GROUP_ );
	memset( m_NextServerTrafficFactor, 0, sizeof(_XSERVERTRAFFICDATA) * _COUNT_MAX_GROUP_ );

	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer = FALSE; // 이플래그를 켜서 캐릭터 리스트를 받아옴.
	g_NetworkKernel.ReleaseNetwork();
	
	memset( g_pLocalUser->m_CharacterInfo.charactername, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH ); // 퀵슬롯이 초기화된 상태에서 저장되지 않도록...

	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	gpDev->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	
	gpDev->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
							
	//////////////////////////////////////////////////////////////////////////////////
	// Create xgui object
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
	
	m_TextureArchive.GetResourceIndex("MI_Login_00.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("MI_Login_01.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("MI_Login_03.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);


	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{	
		m_TextureArchive.GetResourceIndex("tw_CIBITotal.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);	
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{	
		m_TextureArchive.GetResourceIndex("vn_CIBITotal.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);	
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{	
#ifdef _ACCLAIM_VERSION
		m_TextureArchive.GetResourceIndex("us_CIBITotal.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);	
#else
		m_TextureArchive.GetResourceIndex("eu_CIBITotal.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
#endif
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{	
		m_TextureArchive.GetResourceIndex("jp_CIBITotal.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);	
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{	
		m_TextureArchive.GetResourceIndex("rs_CIBITotal.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	}
	else
	{
		m_TextureArchive.GetResourceIndex("CIBITotal.jpg", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);	
	}
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		m_TextureArchive.GetResourceIndex("MI_keyboard.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);	
	}

#ifdef _XTESTSERVER
	m_TextureArchive.GetResourceIndex("MI_chat_quick.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);	
#endif

	m_TextureArchive.BuildTextureList( TRUE );
				
	m_ServerLoginRequestTimeCheckFlag = FALSE;
	m_ServerLoginRequestTime		  = 0;
	
	g_MessageBox.ShowWindow( FALSE );

	memset( m_IDString, 0, sizeof(TCHAR) * 256 );
	memset( m_PWString, 0, sizeof(TCHAR) * 256 );
	
	_XWindow_Login* pLogin_Window = new _XWindow_Login;
	_XWINDOW_STRUCTURE loginwindowstructure = 
	{
		TRUE, {(gnWidth>>1) - (269>>1) , gnHeight - 154}, {269, 115}, 0,
			-1,
			&m_TextureArchive,
			D3DCOLOR_ARGB(255, 255, 255, 255),
			_XDEF_WTITLE_LOGIN
	};
	pLogin_Window->Create(loginwindowstructure);
	pLogin_Window->Initialize(m_TextureArchive);
	pLogin_Window->ShowWindow(TRUE);
	pLogin_Window->EnableWindowEffect(FALSE);
	pLogin_Window->SetDocking(FALSE);
	m_WindowManager.InsertWindow(pLogin_Window);

	// Get last user name from registry...
	BOOL saveuserid = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SAVELASTUSERNAME, FALSE, TRUE );
	if( saveuserid )
	{
		g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTUSERNAME, NULL, m_IDString, _MAX_IDSTRINGSIZE-1, FALSE );
		if( strlen(m_IDString) )
		{
			//_EncodeData( m_IDString, _ENCODEKEY );		
			pLogin_Window->m_IDEdit.ClearBuffer();
			pLogin_Window->m_IDEdit.AddString( m_IDString, strlen(m_IDString) );
			pLogin_Window->m_PWEdit.SetFocus();
		}
	}
	else
	{
		pLogin_Window->m_IDEdit.ClearBuffer();
		pLogin_Window->m_IDEdit.SetFocus();
	}
	
	_XIMEKERNEL.SetNativeMode( false );

	FLOAT yfactor = (FLOAT)gnHeight / 768.0f;

	int halfsize = 0;
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		halfsize = 448>>1;
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		halfsize = 496>>1;
	}
	else
	{
		halfsize = 487>>1;
	}
	
	_XWindow_ServerStatus* pServerStatus_Window = new _XWindow_ServerStatus;
	_XWINDOW_STRUCTURE serverstatuswindowstructure = 
	{
		TRUE, { 33, (int)((85.0f+halfsize)*yfactor) - halfsize }, {230, 487}, 0,
		-1,
		&m_TextureArchive,
		D3DCOLOR_ARGB(255, 255, 255, 255),
		_XDEF_WTITLE_LOGINSERVERLIST
	};

	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		serverstatuswindowstructure.windowsize.cx = 300;
		serverstatuswindowstructure.windowsize.cy = 448;
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		serverstatuswindowstructure.windowsize.cx = 300;
		serverstatuswindowstructure.windowsize.cy = 496;
	}

	pServerStatus_Window->Create(serverstatuswindowstructure);
	pServerStatus_Window->Initialize(m_TextureArchive);
	pServerStatus_Window->ShowWindow(FALSE);
	pServerStatus_Window->EnableWindowEffect(FALSE);
	pServerStatus_Window->SetDocking(FALSE);
	pServerStatus_Window->SetPlaySoundEfect( TRUE );
	m_WindowManager.InsertWindow(pServerStatus_Window);

	m_OneTimeNoticeViewFlag = TRUE;

#ifdef _XTESTSERVER
	_XWindow_TestServerNotice* pTestServerNoticeWindow = new _XWindow_TestServerNotice;
	_XWINDOW_STRUCTURE testservernoticewindowstructure = 
	{
		TRUE, { 289, 150 }, {448, 335}, 0,
		-1,
		&m_TextureArchive,
		D3DCOLOR_ARGB(128, 0, 0, 0),
		_XDEF_WTITLE_TESTSERVERNOTICEWINDOW	
	};
	pTestServerNoticeWindow->Create(testservernoticewindowstructure);
	pTestServerNoticeWindow->Initialize(m_TextureArchive);

	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN) 
	{
		pTestServerNoticeWindow->ShowWindow(TRUE);
	}
	else
	{
		pTestServerNoticeWindow->ShowWindow(FALSE);
	}

	pTestServerNoticeWindow->EnableWindowEffect(FALSE);
	pTestServerNoticeWindow->SetDocking(FALSE);
	m_WindowManager.InsertWindow(pTestServerNoticeWindow);
#else 
#ifdef _XDEF_NEXONUNIFICATIONGUIDE //정식서버 넥슨 통합 안내문 표시 	
	_XWindow_TestServerNotice* pTestServerNoticeWindow = new _XWindow_TestServerNotice;
	_XWINDOW_STRUCTURE testservernoticewindowstructure = 
	{
		TRUE, { (gnWidth>>1)-(448>>1), 150 }, {448, 335}, 0,
			-1,
			&m_TextureArchive,
			D3DCOLOR_ARGB(128, 0, 0, 0),
			_XDEF_WTITLE_TESTSERVERNOTICEWINDOW	
	};
	pTestServerNoticeWindow->Create(testservernoticewindowstructure);
	pTestServerNoticeWindow->Initialize(m_TextureArchive);
	
	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN) 
	{
#ifdef KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA
		pTestServerNoticeWindow->ShowWindow(FALSE);
		m_ShowNoticeWindowDelayTimer = 0;
#else
		pTestServerNoticeWindow->ShowWindow(TRUE);
#endif
	}
	else
	{
		pTestServerNoticeWindow->ShowWindow(FALSE);
	}
	
	pTestServerNoticeWindow->EnableWindowEffect(FALSE);
	pTestServerNoticeWindow->SetDocking(FALSE);
	m_WindowManager.InsertWindow(pTestServerNoticeWindow);
#endif
#endif

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		_XWindow_DrawKeyboard* pDrawKeyBoard_Window = new _XWindow_DrawKeyboard;
		_XWINDOW_STRUCTURE drawkeyboardwindowstructure = 
		{
			TRUE, { gnWidth - 370, gnHeight - 154 }, {344, 114}, 0,
			-1,
			&m_TextureArchive,
			D3DCOLOR_ARGB(128, 0, 0, 0),
			_XDEF_WTITLE_DRAWKEYBOARDWINDOW	
		};
		pDrawKeyBoard_Window->Create(drawkeyboardwindowstructure);
		pDrawKeyBoard_Window->Initialize(m_TextureArchive);
		pDrawKeyBoard_Window->ShowWindow(TRUE);
        pDrawKeyBoard_Window->SetWindowMoveMode( _XWMOVE_FIXED );
		pDrawKeyBoard_Window->m_LoginProcessMode = TRUE;

		m_WindowManager.InsertWindow(pDrawKeyBoard_Window);
	}
/*
	
	g_IGAADWrapper.Initialize();
	POINT windowposition = { 256,256 };
	SIZE  windowsize = { 256,256 };		
	g_IGAADWrapper.SetZoneInfo( _XIGAADZONE_GAMELOADING );
	g_IGAADWrapper.CreateAdWindow();
	g_IGAADWrapper.ShowWindow( TRUE );
	g_IGAADWrapper.SetDisplayTime( 10000 );
*/

	/*
	_XWindow_SelectServerForest* pSelectServerForestWindow = new _XWindow_SelectServerForest;
	_XWINDOW_STRUCTURE selectserverforestwindowstructure = 
	{
		TRUE, {(gnWidth>>1) - 163, (gnHeight>>1) - 108}, {326, 215}, 0,
			-1,
			&m_TextureArchive,
			D3DCOLOR_ARGB(255, 255, 255, 255),
			_XDEF_WTITLE_SELECTSERVERFOREST
	};
	pSelectServerForestWindow->Create(selectserverforestwindowstructure);
	pSelectServerForestWindow->Initialize(m_TextureArchive);
	pSelectServerForestWindow->SetDrawBorder( FALSE );
	pSelectServerForestWindow->ShowWindow(FALSE);
	pSelectServerForestWindow->EnableWindowEffect(FALSE);
	pSelectServerForestWindow->SetDocking(FALSE);
	pSelectServerForestWindow->SetWindowMoveMode( _XWMOVE_FIXED );
	m_WindowManager.InsertWindow(pSelectServerForestWindow);
	*/

	//////////////////////////////////////////////////////////////////////////////////
	// Auto login process
#ifdef _XDWDEBUG			

	/*
	pLogin_Window->m_IDEdit.ClearBuffer();
	pLogin_Window->m_IDEdit.AddString( m_IDString,strlen(m_IDString) );
	
	pLogin_Window->m_PWEdit.ClearBuffer();
	pLogin_Window->m_PWEdit.AddString( m_PWString,strlen(m_PWString) );
	
	static loginonceflag = TRUE;
	if( loginonceflag )
	{
		if(_XCmdLineOption("xautologin",0) != 0)
		{		
			PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_PWEDITENTER, 0), (LPARAM)gHWnd);
			_XDWINPRINT( "Send auto login message" );
			loginonceflag = FALSE;
		}
	}
	*/
#endif

	m_LoginProcess = LOGINPROC_START;
	g_FadeInOutLevel = 255;

	g_MessageBox.MoveWindow((gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1), (gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1) - 80 );

	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{		
		m_CIBILogoImage.Create( (gnWidth>>1) - 140, gnHeight-28, (gnWidth>>1) + 140, gnHeight-8, &m_TextureArchive, m_TextureArchive.FindResource("tw_CIBITotal.jpg"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{		
		m_CIBILogoImage.Create( (gnWidth>>1) - 140, gnHeight-28, (gnWidth>>1) + 140, gnHeight-8, &m_TextureArchive, m_TextureArchive.FindResource("vn_CIBITotal.jpg"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{		
		m_CIBILogoImage.Create( (gnWidth>>1) - 140, gnHeight-28, (gnWidth>>1) + 140, gnHeight-8, &m_TextureArchive, m_TextureArchive.FindResource("jp_CIBITotal.jpg"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
#ifdef _ACCLAIM_VERSION
		m_CIBILogoImage.Create( (gnWidth>>1) - 140, gnHeight-28, (gnWidth>>1) + 140, gnHeight-8, &m_TextureArchive, m_TextureArchive.FindResource("us_CIBITotal.jpg"));
#else
		m_CIBILogoImage.Create( (gnWidth>>1) - 140, gnHeight-28, (gnWidth>>1) + 140, gnHeight-8, &m_TextureArchive, m_TextureArchive.FindResource("eu_CIBITotal.jpg"));
#endif
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{		
		m_CIBILogoImage.Create( (gnWidth>>1) - 140, gnHeight-28, (gnWidth>>1) + 140, gnHeight-8, &m_TextureArchive, m_TextureArchive.FindResource("rs_CIBITotal.jpg"));
	}
	else
	{
		m_CIBILogoImage.Create( (gnWidth>>1) - 140, gnHeight-28, (gnWidth>>1) + 140, gnHeight-8, &m_TextureArchive, m_TextureArchive.FindResource("CIBITotal.jpg"));
	}

	m_CIBILogoImage.m_FColor = D3DCOLOR_ARGB(0, 255, 255, 255);
	
	m_BackgroundStatic.Create( 0, 0, gnWidth, gnHeight, NULL, -1 );
	m_BackgroundStatic.m_FColor = D3DCOLOR_ARGB(0, 255, 255, 255);

	//////////////////////////////////////////////////////////////////////////////////
	// Create device dependent object
	InitDeviceObject();

	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );

	//////////////////////////////////////////////////////////////////////////////////
	// Load bgm object
	_XSetDefaultPath();
	if( !m_PrevProcess_SelectCharacter )
	{
#ifdef _XDWDEBUG			
		if( g_BgmPlayFlag )
#endif
		{
#ifdef _XUSEFMOD
			g_FMODWrapper.StopBGM();
			g_FMODWrapper.PlayBGM( "Title.ogg", FALSE );

			/*
			int lengthms = FSOUND_Stream_GetLengthMs( g_FMODWrapper.m_pBGMStreamObject );
			g_FMODWrapper.SetBGMLoopPoints( 0, lengthms );
			FSOUND_Stream_SetLoopCount( g_FMODWrapper.m_pBGMStreamObject, 100 );
			*/
#else
			if( g_pBGMSoundObject[0] )
			{
				if( !g_pBGMSoundObject[0]->IsPlaying() )
				{
					g_pBGMSoundObject[0]->Stop();
				}
				g_pBGMSoundObject[0]->Destroy();
				g_pBGMSoundObject[0] = NULL;
			}

			if( !g_pBGMSoundObject[0] )
			{
				SoundInit initsound;
				initsound.m_bLooping = true;
				initsound.m_bStreaming = false;
				initsound.m_bMusic = true;

				initsound.m_sFileName = _T( gModulePath );
				initsound.m_sFileName += _T("\\Data\\Sound\\BGM\\Title.ogg");
								
				if(!AudioMgr()->CreateSound(g_pBGMSoundObject[0])) return false;
				if(!g_pBGMSoundObject[0]->Init(initsound))
				{
					_XFatalError("Error initializing 3d audio file");
					return false;
				}		

				g_pBGMSoundObject[0]->SetVolume( g_pBGMSoundMaxVolume );
				g_pBGMSoundObject[0]->Play();
			}
#endif
		}
	}

	m_PrevProcess_SelectCharacter = FALSE;

	// User Data Reset
	g_pLocalUser->ResetInventoryInfo();
	g_pLocalUser->ResetSkillList();
	g_pLocalUser->ResetLocalUserData();

/*	_XWindow_QSlot* pQSwindow = (_XWindow_QSlot*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_QUICKSLOT );
	if( pQSwindow )
	{
		pQSwindow->InitializeQuickSlotInfo();
	}
*/

	m_Player.SetPitchLimits(-45, 45);
	m_Player.SetRollLimits(-10, 10);	
	m_Player.SetType(camFly);
			
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
		strcpy( flashfilename, _T("TITLE01.swf") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		strcpy( flashfilename, _T("vn_TITLE01.swf") );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		strcpy( flashfilename, _T("tw_TITLE01.swf") );
	}
	else
	{
		strcpy( flashfilename, _T("TITLE01.swf") );
	}
	
	_XPackageArchive package;
	
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Script\\") );

#ifdef _XTESTSERVER				
	strcat( packagefilename, _T("tsXSCENESCRIPT.XP") );			
#else
	strcat( packagefilename, _T("XSCENESCRIPT.XP") );			
#endif
	package.OpenPackage( packagefilename );

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
	

	m_PrevSendedLoginMessageTime = 0;
	m_PrevSendedSelectForectTime = 0;

	_XWindow::m_GlobalPlaySoundEffect = TRUE;

#ifdef _X_US_EXPANDSERVERLIST
	m_LastSelectedRegion_Server = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSERVERGROUP, -1, TRUE );
#endif

	GetServerList( FALSE );

#ifdef _XNEXON_WEBLOGIN
	if( g_OnetimeAutoLogin )
	{
		BOOL readyautologin = TRUE;

		int parampos = _XCmdLineOption("passport=",0,8);
		if( parampos != 0 )
		{
			readyautologin = TRUE;
		}		
		else
		{
			readyautologin = FALSE;
			g_OnetimeAutoLogin = FALSE;
		}

		if( readyautologin )
		{
			pServerStatus_Window->SetWindowSize( 230, 448 );
			pServerStatus_Window->MoveWindow( (gnWidth>>1) - (230>>1), 70 );
			pServerStatus_Window->ShowWindow( !TRUE );

			if( pServerStatus_Window->m_pConnectButton )
			{
				pServerStatus_Window->m_pServerListUpdateButton->MoveWindow( pServerStatus_Window->m_pServerListUpdateButton->GetWindowPos().x, 
																			 pServerStatus_Window->GetWindowPos().y + pServerStatus_Window->GetWindowSize().cy - 39 );

				pServerStatus_Window->m_pConnectButton->MoveWindow( pServerStatus_Window->m_pConnectButton->GetWindowPos().x, 
																	pServerStatus_Window->GetWindowPos().y + pServerStatus_Window->GetWindowSize().cy - 39 );
				pServerStatus_Window->m_pConnectButton->ShowWindow( TRUE );
			}

			if( pServerStatus_Window->m_pCloseServerlistButton )
				pServerStatus_Window->m_pCloseServerlistButton->ShowWindow( FALSE );

			pLogin_Window->ShowWindow( FALSE );			

			if(pTestServerNoticeWindow)
				pTestServerNoticeWindow->ShowWindow( FALSE );
			
			m_OneTimeNoticeViewFlag = FALSE;

			//PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_PWEDITENTER, 0), (LPARAM)gHWnd);
			//_XDWINPRINT( "Send auto login message" );
		}
		else
		{
			if( pServerStatus_Window->m_pConnectButton )
				pServerStatus_Window->m_pConnectButton->ShowWindow( FALSE );
			
			if( pServerStatus_Window->m_pCloseServerlistButton )
				pServerStatus_Window->m_pCloseServerlistButton->ShowWindow( TRUE );

			// 계속 자동로그인. g_OnetimeAutoLogin = FALSE;
		}

		//g_OnetimeAutoLogin = FALSE;
	}
	else
	{
		if( pServerStatus_Window->m_pConnectButton )
			pServerStatus_Window->m_pConnectButton->ShowWindow( FALSE );

		if( pServerStatus_Window->m_pCloseServerlistButton )
			pServerStatus_Window->m_pCloseServerlistButton->ShowWindow( TRUE );
	}
#endif

#ifdef			_ACCLAIM_VERSION
	if( g_OnetimeAutoLogin )
	{
		BOOL readyautologin = TRUE;

		memset( g_szAcclaimID, 0, sizeof(TCHAR) * 64);
		memset( g_szAcclaimPW, 0, sizeof(TCHAR) * 64);
		
		int parampos = _XCmdLineOption("id",0,2);
		if( parampos != 0 )
		{
			TCHAR argstring[256];
			memset( argstring, 0, sizeof(TCHAR) * 256 );
			strcpy( argstring, _XArgv(parampos)+3 );
			
			LPTSTR pSeperater = strstr( argstring, _T(",") );
			if( !pSeperater )
			{
				//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Invalid login parameter."), TRUE );
				readyautologin = FALSE;
			}
			
			strncpy( g_szAcclaimID, argstring, pSeperater-argstring );
			strcpy( g_szAcclaimPW, pSeperater+1 );
		}
		else
		{
			//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Could not find login parameter."), TRUE );
			readyautologin = FALSE;
		}

		if( readyautologin )
		{
			pServerStatus_Window->MoveWindow( (gnWidth>>1) - (300>>1), 70 );
			pServerStatus_Window->ShowWindow( TRUE );

			if( pServerStatus_Window->m_pConnectButton )
				pServerStatus_Window->m_pConnectButton->ShowWindow( TRUE );

			if( pServerStatus_Window->m_pCloseServerlistButton )
				pServerStatus_Window->m_pCloseServerlistButton->ShowWindow( FALSE );

			pLogin_Window->ShowWindow( FALSE );			

			//PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_PWEDITENTER, 0), (LPARAM)gHWnd);
			//_XDWINPRINT( "Send auto login message" );
		}
		else
		{
			if( pServerStatus_Window->m_pConnectButton )
				pServerStatus_Window->m_pConnectButton->ShowWindow( FALSE );
			
			if( pServerStatus_Window->m_pCloseServerlistButton )
				pServerStatus_Window->m_pCloseServerlistButton->ShowWindow( TRUE );

			g_OnetimeAutoLogin = FALSE;
		}

		//g_OnetimeAutoLogin = FALSE;
	}
	else
	{
		if( pServerStatus_Window->m_pConnectButton )
			pServerStatus_Window->m_pConnectButton->ShowWindow( FALSE );

		if( pServerStatus_Window->m_pCloseServerlistButton )
			pServerStatus_Window->m_pCloseServerlistButton->ShowWindow( TRUE );
	}
#endif	

	_XWindow_QuickSlot* pQSlot_Window = (_XWindow_QuickSlot *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUICKSLOT);	
	if( pQSlot_Window )
	{	
		for( int i = 0; i < _QUICKSLOT_COUNT_; i++ )
		{
			memset( &(pQSlot_Window->m_QuickSlot[i]), 0, sizeof(_XQSlot_Info) );
		}
	}

#ifdef _XDEF_RENEWAL_CHATWINDOW_20070503
	g_Chatlist.disposeList();
	g_SystemMsgList.disposeList();
#endif

#ifdef _ACCLAIM_RUBICONADSYSTEM	
	TCHAR szRegString[512];
	memset( szRegString, 0, sizeof(TCHAR) * 512 );
	if( _XRegReadString(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Internet Explorer\\"), _T("Version"), _T(""),szRegString, 511, FALSE ) )
	{
		if( szRegString[0] < _T('6') )
		{
			TCHAR szMessageString[512];
			memset(szMessageString, 0, sizeof(szMessageString));
			_snprintf( szMessageString, sizeof(szMessageString), _T("Please install Microsoft® Internet Explorer version 6.0 or higher. [Ver %s]"), szRegString );
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), szMessageString, TRUE, 0, 0, 0 );
		}
	}
	else
	{
		_XLog("Can't access registry, [HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Internet Explorer\\Version");
	}
#endif	

	return TRUE;
}

BOOL XProc_LoginServer::CreateOffScreen( void )
{
	if( g_pTitleFlashOffScreenTexture ) return TRUE;
	
	SIZE size = m_BackgroundStatic.GetWindowSize();
	
	if( FAILED( gpDev->CreateTexture( size.cx, size.cy, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTitleFlashOffScreenTexture, NULL ) ) )
    {
        return FALSE;
    }
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// 로그인 서버 접속 대기시간 제한처리
#define _XDEF_SERVERLOGIN_RESENDTIME			 30000
#define _XDEF_SERVERLOGIN_WAITRESULTMESSAGETIME	 30000
#define _XDEF_SERVERLOGIN_MAXRETRYTIME				1

void XProc_LoginServer::LoginServerRequestTimeCheck( void )
{	
	if( !m_ServerLoginRequestTimeCheckFlag ) return;

	if( g_NetworkKernel.m_LoginProcessMode == _XDEF_LOGINPROCESS_READY ) 
	{
		if( m_ServerLoginRequestTime == 0 )
		{				
			m_ServerLoginRequestTime = timeGetTime();

			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_LOGINSERVER_CONNECTLOGINSERVER, 
										 TRUE, _XDEF_LS_CANCELLOGINSERVER, _XDEF_LS_CANCELLOGINSERVER, _XDEF_LS_CANCELLOGINSERVER );
			g_MessageBox.SetCloseButtonCmdID(_XDEF_LS_CANCELLOGINSERVER);
			g_MessageBox.SetMessageColor();
			g_MessageBox.StartTimer( _XDEF_SERVERLOGIN_RESENDTIME, TRUE );

			_XDWINPRINT( "Connecting login server." );
		}
		else
		{
			// 로그인 제한 시간이 지나면 로그인 실패
			DWORD progressTime = (timeGetTime() - m_ServerLoginRequestTime);
			if( progressTime > _XDEF_SERVERLOGIN_RESENDTIME )
			{				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_LOGINSERVER_CANNOTCONNECTLOGINSERVER, TRUE, _XDEF_LS_CANCELLOGINSERVER );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB(255,255,0,0) );

				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				g_NetworkKernel.ReleaseUDPSocket();
				
#ifdef _XDEF_NEXONLOGIN // Nexon login

#ifdef _XDEF_NEXONLOGINFINAL	// 넥슨 메신저 로그인
				if(strcmp(g_NetworkKernel.m_LOGINSERVER_ADDRESS, _T("221.147.34.19")) != 0)
				{
					g_NetworkKernel.ReleaseNMCO();
				}
#endif

#endif

				_XDWINPRINT( "Could not connect to login server." );
			}
			else
			{
				int lefttime = (_XDEF_SERVERLOGIN_RESENDTIME - progressTime) / 1000;
				if( lefttime < 7 && lefttime > 0 )
				{
					LPTSTR param1 = _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_CONNECTLOGINSERVER);					
					// "%s %d초 남았습니다."
					g_MessageBox.SetMessage( g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_LOGINSERVER_WAITTING, param1, &lefttime ) );	
				}
			}
		}
	}
	else if( g_NetworkKernel.m_LoginProcessMode == _XDEF_LOGINPROCESS_CONFIRM ) // 서버군 선택후 가야할 서버주소 패킷을 기다린다.
	{
		if( m_ServerLoginRequestTime == 0 )
		{ 
			m_ServerLoginRequestTime = timeGetTime();
			_XDWINPRINT( "Request login result." );
		}
		else
		{
			// 로그인 제한 시간이 지나면 로그인 실패
			
			DWORD progressTime = (timeGetTime() - m_ServerLoginRequestTime);
			if( progressTime > _XDEF_SERVERLOGIN_WAITRESULTMESSAGETIME )
			{	
				_XWindow_SelectServerForest* pSelectServerForestWindow = 
					(_XWindow_SelectServerForest*)m_WindowManager.FindWindow( _XDEF_WTITLE_SELECTSERVERFOREST );
				
				if( pSelectServerForestWindow )
				{				
					pSelectServerForestWindow->ShowWindow( FALSE );
				}
				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_LOGINSERVER_TIMEOUTSELECTSERVERFOREST, TRUE, _XDEF_LS_CANCELLOGINSERVER );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB(255,255,0,0) );

				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;				

				g_NetworkKernel.ReleaseUDPSocket();

#ifdef _XDEF_NEXONLOGIN // Nexon login
				
#ifdef _XDEF_NEXONLOGINFINAL	// 넥슨 메신저 로그인
				if(strcmp(g_NetworkKernel.m_LOGINSERVER_ADDRESS, _T("221.147.34.19")) != 0)
				{
					g_NetworkKernel.ReleaseNMCO();
				}
#endif
				
#endif
				
				_XDWINPRINT( "Can't download login result." );
			}					
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
// 서버 접속 대기시간 제한처리
#define _XDEF_SERVERLOGIN_REQUESTLIMITTIME		30000
void  XProc_LoginServer::GameServerRequestTimeCheck( void )
{
	if( !m_ServerLoginRequestTimeCheckFlag ) return;
	
	if( m_ServerLoginRequestTime == 0 )
	{
		m_ServerLoginRequestTime = timeGetTime();
		
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNOCANCEL, "Connect server", " ", 
			TRUE, 0, 0, _XDEF_LS_CANCELLOGINSERVER );
		g_MessageBox.m_OkBtn.ShowWindow( FALSE );
		g_MessageBox.m_CancelBtn.ShowWindow( FALSE );
		g_MessageBox.SetCloseButtonCmdID(_XDEF_LS_CANCELLOGINSERVER);
		g_MessageBox.SetMessageColor();
	}
	else
	{
		// 로그인 제한 시간이 지나면 에러메세지를 출력하고 로그인 프로세스 종료
		DWORD progressTime = (timeGetTime() - m_ServerLoginRequestTime);
		if( progressTime > _XDEF_SERVERLOGIN_REQUESTLIMITTIME )
		{							
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_LOGINSERVER_CANNOTCONNECTGAMESERVER, TRUE, _XDEF_LS_CANCELLOGINSERVER );
			g_MessageBox.SetMessageColor( D3DCOLOR_ARGB(255,255,0,0) );
			
			m_ServerLoginRequestTimeCheckFlag = FALSE;
			m_ServerLoginRequestTime		  = 0;
		}
		else
		{									
			int countdown = (_XDEF_SERVERLOGIN_REQUESTLIMITTIME - progressTime)/1000;
			
			if( countdown <= 15 )
			{
				if( countdown > 0 )
				{					
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_CANCEL, "Connect server", " ", 
						TRUE, _XDEF_LS_CANCELLOGINSERVER, _XDEF_LS_CANCELLOGINSERVER, _XDEF_LS_CANCELLOGINSERVER );					
					
					//					TCHAR messagestring[256];
					//					LoadString( gHInst, ID_STRING_LOGINSERVER_LOGINGAMESERVERCOUNTDOWN, messagestring, 255 );
					g_MessageBox.SetMessage( _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_LOGINGAMESERVERCOUNTDOWN), countdown );
				}
				else
				{
					
					//					TCHAR messagestring[256];
					//					LoadString( gHInst, ID_STRING_LOGINSERVER_CANCELCONNECT, messagestring, 255 );
					g_MessageBox.SetMessage( _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_CANCELCONNECT) );
				}
			}
			else
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_LOGINSERVER_LOGINGAMESERVER, 
					TRUE, _XDEF_LS_CANCELLOGINSERVER, _XDEF_LS_CANCELLOGINSERVER, _XDEF_LS_CANCELLOGINSERVER );
				g_MessageBox.SetMessageColor();				
			}			
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
// 로그인 프로세스 종료
void XProc_LoginServer::DestroyProcess( void )
{	
	_XIMEKERNEL.SetFocus( NULL );
	
	m_TextureArchive.DisposeTexture();	
	g_NetworkKernel.ReleaseUDPSocket();	
	m_WindowManager.DestroyManager();

	m_GraphUpdateTimer = 0;
	m_LastUpdateServerListTime = 0;

	g_CurrentFocusedObject = NULL;
	_XWindow::m_GlobalPlaySoundEffect = FALSE;

	SetEnableProcess( FALSE );
}

//////////////////////////////////////////////////////////////////////////////////
// Initialize device defendent object
bool XProc_LoginServer::InitDeviceObject( void )
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
	
	// Wall texture just point mip mapping with linear filtering
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
	gpDev->SetRenderState( D3DRS_DEPTHBIAS ,			 0 );
	
#ifdef _XDEF_USETERRAINDIFFUSECOLOR
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_COLOR1 );
#else
	gpDev->SetRenderState( D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_MATERIAL);
#endif
	
	gpDev->SetRenderState( D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	gpDev->SetRenderState( D3DRS_LOCALVIEWER, !FALSE );
	
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

	CreateOffScreen();
	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
// Clear device defendent object
void XProc_LoginServer::ReleaseDeviceObject( void )
{
	
}

void XProc_LoginServer::PauseProcess( void )
{
	
}

void XProc_LoginServer::UnPauseProcess( void )
{
	ReleaseDeviceObject();
	InitDeviceObject();	

	_XWindow_Login* login_window = (_XWindow_Login *)m_WindowManager.FindWindow(_XDEF_WTITLE_LOGIN);
	login_window->m_IDEdit.SetFocus();
}

void XProc_LoginServer::OnTimer(WPARAM wparam, LPARAM lparam)
{
	/*
	int wmId;
	wmId = wparam;             // timer identifier 
	if ( wmId == TIMER_PLAY )
		if(g_pTitleFlashObject) g_pTitleFlashObject->PlayTimerFire();
	else if ( wmId == TIMER_CURSOR )
		if(g_pTitleFlashObject) g_pTitleFlashObject->CursorTimerFire();
	*/
}

void XProc_LoginServer::OnPaint(WPARAM wparam, LPARAM lparam)
{
	/*
	if (g_pTitleFlashObject == NULL)
	{
		CreateOffScreen();
		g_pTitleFlashObject = GetFlashControl();
		g_pTitleFlashObject->initialize( gHWnd,gHInst,m_pOffScreenTexture );
		//g_pTitleFlashObject->SetLowQuality();
		return;
	}
	
	RECT rc; 
	GetClientRect( gHWnd, &rc);	
	g_pTitleFlashObject->Repaint( rc);
	
	Draw();
	*/
	//Draw();
}

void XProc_LoginServer::Draw( void )
{	
#ifdef KR_RB_DELAYSHOWNEXONNOTICEWINDOW_070530_KERYGMA

	if( m_OneTimeNoticeViewFlag )
	{
		if( m_ShowNoticeWindowDelayTimer == 0 )
		{
			m_ShowNoticeWindowDelayTimer = g_LocalSystemTime;
		}
		else
		{
			if( g_LocalSystemTime - m_ShowNoticeWindowDelayTimer > 8000 )
			{
				_XWindow_TestServerNotice* pTestServerNoticeWindow = (_XWindow_TestServerNotice*)m_WindowManager.FindWindow( _XDEF_WTITLE_TESTSERVERNOTICEWINDOW );
				if( pTestServerNoticeWindow )
				{
					if( !pTestServerNoticeWindow->GetShowStatus() )
					{
						pTestServerNoticeWindow->ShowWindow( TRUE );
					}
				}
			}
		}
	}
#endif

#ifdef _XNEXON_WEBLOGIN
	if( g_OnetimeAutoLogin )
	{
		if( m_ShowOneTimeAutologinServerListViewDelayTimer == 0 )
		{
			m_ShowOneTimeAutologinServerListViewDelayTimer = g_LocalSystemTime;
		}
		else
		{
			if( g_LocalSystemTime - m_ShowOneTimeAutologinServerListViewDelayTimer > 5000 )
			{
				_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
				if( pWindow )
				{
					if( !pWindow->GetShowStatus() )  
						pWindow->ShowWindow(TRUE);
				}
			}
		}
	}
#endif


	RebuildServerTrafficGraph();

	if( g_LocalSystemTime - m_LastUpdateServerListTime > 1500 )
	{	
		_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
		if( pWindow )
		{
			if( pWindow->m_pServerListUpdateButton )
			{
				pWindow->m_pServerListUpdateButton->EnableWindow( TRUE );
			}
		}				
	}

	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Begin the scene
	gpDev->BeginScene();

	// Set matrices
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_Player.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&m_Player.GetProjMatrix());

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
		
	m_BackgroundStatic.DrawWithRegion( g_pTitleFlashOffScreenTexture );

	/*
	g_XBaseFont->SetColor( 0xFFFF3000 );	
	g_XBaseFont->Print( 20, 20, 1.0f, "%d", gnFrameCounter );
	g_XBaseFont->Flush();
	*/

	m_WindowManager.Draw();
	
	m_CIBILogoImage.Draw();

	if( m_AlphaValue >= 255 )		
		g_IMEWindow.Draw();
	
	g_MessageBox.Draw();
	
	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
	
	/*
	_XDrawSolidBar( 50, 100, 300, 500, 0x3F000000 );
	_XDrawRectAngle( 50, 100, 300, 500, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );

	g_XBaseFont->SetColor( 0xFFB7B7B7 );
	for( int i = 0; i < 16; i++ )
	{
		g_XBaseFont->PrintAlign( 60, 100 + (i*16), 1.0f, _XFONT_ALIGNTYPE_LEFT, "%d XX서버", i );
	}
	g_XBaseFont->Flush();
	*/
	#ifndef _XRUSSIAN
	if( m_AlphaValue == 255 )
	{
		g_XBaseFont->SetColor( 0xFFB7B7B7 );
		g_XBaseFont->PrintAlign( 9, gnHeight - 23, 1.0f, _XFONT_ALIGNTYPE_LEFT, _T("Copyright 2002-07 Indy21 Co., LTD. All rights reserved.") );
		g_XBaseFont->Flush();
	}
	#endif

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE  || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		_XDrawSolidBar ( (gnWidth>>1)-164, gnHeight-214, (gnWidth>>1)+164, gnHeight-187, 0x7F000000 ); 
		_XDrawRectAngle( (gnWidth>>1)-165, gnHeight-215, (gnWidth>>1)+164, gnHeight-187, 0.0f, 0xFF000000 );
		_XDrawRectAngle( (gnWidth>>1)-166, gnHeight-216, (gnWidth>>1)+165, gnHeight-186, 0.0f, 0xFF7E7C7F );
		
		if( !g_CurrentFocusedObject )
		{
			if( gpInput->CheckMousePos( (gnWidth>>1)-164, gnHeight-214, (gnWidth>>1)+164, gnHeight-187 ) )
			{
				_XDrawRectAngle( (gnWidth>>1)-165, gnHeight-215, (gnWidth>>1)+164, gnHeight-187, 0.0f, 0xFFFFE401 );
				
				if( gpInput->GetMouseState()->bButton[0] )
				{
					g_CurrentFocusedObject = _XDEF_DUMMYID_SELECTSERVERBUTTON;				
				}			
			}
		}
		else if( g_CurrentFocusedObject == _XDEF_DUMMYID_SELECTSERVERBUTTON )
		{
			if( !gpInput->GetMouseState()->bButton[0] )
			{
				g_CurrentFocusedObject = NULL;			
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LS_VIEWSERVERLISTBUTTON	, 0), (LPARAM)gHWnd);
			}
		}
	}
	else
	{
		_XDrawSolidBar ( (gnWidth>>1)-124, gnHeight-214, (gnWidth>>1)+124, gnHeight-187, 0x7F000000 ); 
		_XDrawRectAngle( (gnWidth>>1)-125, gnHeight-215, (gnWidth>>1)+124, gnHeight-187, 0.0f, 0xFF000000 );
		_XDrawRectAngle( (gnWidth>>1)-126, gnHeight-216, (gnWidth>>1)+125, gnHeight-186, 0.0f, 0xFF7E7C7F );

		if( !g_CurrentFocusedObject )
		{
			if( gpInput->CheckMousePos( (gnWidth>>1)-124, gnHeight-214, (gnWidth>>1)+124, gnHeight-187 ) )
			{
				_XDrawRectAngle( (gnWidth>>1)-125, gnHeight-215, (gnWidth>>1)+124, gnHeight-187, 0.0f, 0xFFFFE401 );

				if( gpInput->GetMouseState()->bButton[0] )
				{
					g_CurrentFocusedObject = _XDEF_DUMMYID_SELECTSERVERBUTTON;				
				}			
			}
		}
		else if( g_CurrentFocusedObject == _XDEF_DUMMYID_SELECTSERVERBUTTON )
		{
			if( !gpInput->GetMouseState()->bButton[0] )
			{
				g_CurrentFocusedObject = NULL;			
				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LS_VIEWSERVERLISTBUTTON	, 0), (LPARAM)gHWnd);
			}
		}
	}

	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );

#ifdef _XTESTSERVER
	if( _XCmdLineOption("xinnertestserverlogin",0) == 0 )
	{

#ifdef _X_US_EXPANDSERVERLIST
		if( m_LastSelectedServer < 0 || m_LastSelectedRegion_Server < 0)
		{
			g_XBaseFont->SetColor( 0xFFB50000 );
			g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1807) );
		}
#else	
		if( m_LastSelectedServer < 0 )
		{
			g_XBaseFont->SetColor( 0xFFB50000 );
			g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1807) );
		}
#endif
		else
		{
			if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 0 )
			{
				if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor == _FOREST_STATE_FULL )
				{
					g_XBaseFont->SetColor( 0xFFB90000 );	
					g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1800));
				}
				else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor == _FOREST_STATE_STOP )
				{
					g_XBaseFont->SetColor( 0xFF898989 );	
					g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1801) );
				}
				else
				{
					g_XBaseFont->SetColor( 0xFF898989 );	
					g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1802) );
				}
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 25  )
			{
				g_XBaseFont->SetColor( 0xFF00FF00 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1803) );		
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 50  )
			{
				g_XBaseFont->SetColor( 0xFFD2D2D2 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1804) );		
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 75  )
			{
				g_XBaseFont->SetColor( 0xFFD2D2D2 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1805) );		
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor <= 100  )
			{
				g_XBaseFont->SetColor( 0xFFFF3000 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1806) );
			}
		}
	}
	else // 내부 서버 접속시
	{
#ifdef _X_US_EXPANDSERVERLIST
		if( m_LastSelectedServer < 0 || m_LastSelectedRegion_Server < 0)
		{
			g_XBaseFont->SetColor( 0xFFB50000 );
			g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1807) );
		}
#else	
		if( m_LastSelectedServer < 0 )
		{
			g_XBaseFont->SetColor( 0xFFB50000 );
			g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1807) );
		}
#endif
		else
		{
			if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 0 )
			{
				if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor == _FOREST_STATE_FULL )
				{
					g_XBaseFont->SetColor( 0xFFB90000 );	
					g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1808),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);	
				}
				else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor == _FOREST_STATE_STOP )
				{
					g_XBaseFont->SetColor( 0xFF898989 );	
					g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1809),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);	
				}
				else
				{
					g_XBaseFont->SetColor( 0xFF898989 );	
					g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1810),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);
				}
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 25  )
			{
				g_XBaseFont->SetColor( 0xFF00FF00 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1811),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);		
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 50  )
			{
				g_XBaseFont->SetColor( 0xFFD2D2D2 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1812),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);		
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 75  )
			{
				g_XBaseFont->SetColor( 0xFFD2D2D2 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1813),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);		
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor <= 100  )
			{
				g_XBaseFont->SetColor( 0xFFFF3000 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1814),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);
			}
		}
	}
#else
#ifdef _X_US_EXPANDSERVERLIST
	if( m_LastSelectedServer < 0 || m_LastSelectedRegion_Server < 0)
	{
		g_XBaseFont->SetColor( 0xFFB50000 );
		g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1815) );
	}
#else
	if( m_LastSelectedServer < 0 )
	{
		g_XBaseFont->SetColor( 0xFFB50000 );
		g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1815) );
	}
#endif
	else
	{
		if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 0 )
		{
			if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor == _FOREST_STATE_FULL )
			{
				g_XBaseFont->SetColor( 0xFFB90000 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1808),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);
			}
			else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor == _FOREST_STATE_STOP )
			{
				g_XBaseFont->SetColor( 0xFF898989 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1809),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);
			}
			else
			{
				g_XBaseFont->SetColor( 0xFF898989 );	
				g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1810),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);
			}
		}
		else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 25  )
		{
			g_XBaseFont->SetColor( 0xFF00FF00 );	
			g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1811),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);		
		}
		else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 50  )
		{
			g_XBaseFont->SetColor( 0xFFD2D2D2 );	
			g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1812),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);		
		}
		else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 75  )
		{
			g_XBaseFont->SetColor( 0xFFD2D2D2 );	
			g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1813),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);		
		}
		else if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor <= 100  )
		{
			g_XBaseFont->SetColor( 0xFFFF3000 );	
			g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight-206, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1814),g_szServerNameString[m_NextServerTrafficFactor[m_LastSelectedServer].serverindex]);
		}
	}
#endif
	
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
#ifdef _PREOBVERSION 		
	g_XBaseFont->SetColor( 0xFFFF3000 );
	g_XBaseFont->Print( 2, 2, 1.0f, _T("[PreOBT Version]") );
	g_XBaseFont->Flush();
#endif
	
#ifdef _TWIMETESTVERSION
	g_XBaseFont->SetColor( 0xFFFF3000 );
	g_XBaseFont->Print( 2, 2, 1.0f, _T("[IME Test version]") );
	g_XBaseFont->Flush();
#endif

	/*
	TCHAR outputstring[15];
	memset( outputstring, 0, sizeof(TCHAR) * 15);

	wchar_t srcstring[3];
	srcstring[0] = 0x55D9;
	srcstring[1] = 0x8D0B;
	srcstring[2] = 0;

	int requirelength = WideCharToMultiByte(950, 0, srcstring, -1, outputstring, 15, NULL, NULL);

	g_XBaseFont->SetColor( 0xFFFF3000 );
	g_XBaseFont->Print( 2, 2, 1.0f, outputstring );
	g_XBaseFont->Flush();
	*/

#ifdef _XDWDEBUG		
	if( gpInput->CheckKeyPress(DIK_LCONTROL) )
	{
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->Print( 4, gnHeight-25, 1.0f, "FPS: %d", gnFPS );
		g_XBaseFont->Print( 4, gnHeight-51, 1.0f, "X: %f Y: %f Z: %f Yaw: %f Pitch: %f Roll: %f",
			g_LodTerrain.m_3PCamera.m_CameraPosition.x,g_LodTerrain.m_3PCamera.m_CameraPosition.y,
			g_LodTerrain.m_3PCamera.m_CameraPosition.z, g_LodTerrain.m_3PCamera.GetYaw(),
			g_LodTerrain.m_3PCamera.GetPitch(),g_LodTerrain.m_3PCamera.GetRoll() );
		g_XBaseFont->Print( 4, gnHeight-38, 1.0f, "Target X: %f Y: %f Z: %f Distance : %f",
			g_LodTerrain.m_3PCamera.m_TargetPosition.x,g_LodTerrain.m_3PCamera.m_TargetPosition.y,
			g_LodTerrain.m_3PCamera.m_TargetPosition.z,g_LodTerrain.m_3PCamera.m_TargetDistance);
		g_XBaseFont->Flush();
	}
#endif	
		
	// End the scene
	gpDev->EndScene();
			
	// Present the backbuffer contents to the display

	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	

	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true;
}

bool XProc_LoginServer::Process( void )
{
	if(g_pTitleFlashObject) g_pTitleFlashObject->PlayTimerFire();

	if( m_ConnectMode == _XDEF_SERVERCONNECTMODE_LOGIN )
	{
		LoginServerRequestTimeCheck();
	}
	else if( m_ConnectMode == _XDEF_SERVERCONNECTMODE_GAMESERVER )
	{
		GameServerRequestTimeCheck();
	}

	g_MessageBox.Process();
	
	if( m_AlphaValue >= 255 )
		g_IMEWindow.Process();

	m_WindowManager.Process();
	
	if( m_AlphaValue < 255 )
	{
		m_AlphaValue += 5;
		if(m_AlphaValue >= 255) m_AlphaValue = 255;
		m_CIBILogoImage.m_FColor = D3DCOLOR_ARGB(m_AlphaValue, 255, 255, 255);
		m_BackgroundStatic.m_FColor = D3DCOLOR_ARGB(m_AlphaValue, 255, 255, 255);
	}
	else
	{
		if(m_AlphaValue >= 255) m_AlphaValue = 255;
	}

#ifdef _XUSEFMOD
	g_FMODWrapper.ProcessBGM();
	g_FMODWrapper.FMODUpdate();	
#endif
	
	return true;
}

void XProc_LoginServer::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	_XWindow_Login* login_window = (_XWindow_Login *)m_WindowManager.FindWindow(_XDEF_WTITLE_LOGIN);

	if( message == WM_COMMAND )
	{
		switch( wparam )
		{
		case _XPROCDEF_IDEDITENTER:
			{
				login_window->m_PWEdit.SetFocus();
			}
			break;
		case _XDEF_LS_LOGINBUTTON:
		case _XPROCDEF_PWEDITENTER:
			{ 
/*
#ifdef _XTAIWANESE // 임시..
				m_LastSelectedServer = 0;
#endif
*/
				// 베트남어일 경우 스크린키보드로 최소 3회 이상 입력해야 한다.
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
				{
					if( login_window->m_PWEdit.m_VKeyboardTypedCount < 3 )
					{
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3045), TRUE );
						
						login_window->m_PWEdit.ClearBuffer();
						login_window->m_PWEdit.m_VKeyboardTypedCount = 0;
						break;
					}
				}
				// 미국일 경우 자동로그인이 아닐때만 스크린키보드로 최소 3회 이상 입력해야 한다.
				else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
				{
					int parampos = _XCmdLineOption("id",0,2);
					if( parampos == 0 )
					{
						if( login_window->m_PWEdit.m_VKeyboardTypedCount < 3 )
						{
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3045), TRUE );
							
							login_window->m_PWEdit.ClearBuffer();
							login_window->m_PWEdit.m_VKeyboardTypedCount = 0;
							break;
						}
					}
				}
				
#ifdef _X_US_EXPANDSERVERLIST
				if( m_LastSelectedServer < 0 || m_LastSelectedRegion_Server < 0)
				{
#else
				if( m_LastSelectedServer < 0 )
				{
#endif
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1815), TRUE );
					
					_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
					if( pWindow )
					{
						if( !pWindow->GetShowStatus() )
							pWindow->ShowWindow( TRUE );
					}
					
					return;
				}

				if( !g_InnerUser )
				{
#ifdef _XTESTSERVER
					if( _XCmdLineOption("xinnertestserverlogin",0) == 0 )
					{					
						if( m_NextServerTrafficFactor[_COUNT_MAX_GROUP_-1].trafficfactor < 0 )
						{
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1816), TRUE );
							
							_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
							if( pWindow )
							{
								if( !pWindow->GetShowStatus() )
									pWindow->ShowWindow( TRUE );
							}
							
							return;
						}
					}
					else
					{
						if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 0 )
						{
							g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1816), TRUE );
							
							_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
							if( pWindow )
							{
								if( !pWindow->GetShowStatus() )
									pWindow->ShowWindow( TRUE );
							}
							
							return;
						}
					}
#else
					if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 0 )
					{
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1816), TRUE );
						
						_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
						if( pWindow )
						{
							if( !pWindow->GetShowStatus() )
								pWindow->ShowWindow( TRUE );
						}
						
						return;
					}
#endif
				}

				SetFocus( gHWnd );
	
#ifdef			_ACCLAIM_VERSION // acclaim version일 경우 Web 인증을 한다.

				_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
				if( pWindow )
				{
					pWindow->ShowWindow( FALSE );
					pWindow->MoveWindow( 33, 85 );

					if( pWindow->m_pConnectButton )
					{
						pWindow->m_pConnectButton->ShowWindow( FALSE );
						pWindow->m_pCloseServerlistButton->ShowWindow( TRUE );
					}
				}

				login_window->ShowWindow( TRUE );

				BOOL _autologinflag = FALSE;

				if( !g_OnetimeAutoLogin )
				{
					strncpy( m_IDString, login_window->m_IDEdit.GetText(), sizeof(m_IDString) );
					strncpy( g_szAcclaimID, login_window->m_IDEdit.GetText(), sizeof(g_szAcclaimID) );

					unsigned char digest[16];
					unsigned int len = strlen (login_window->m_PWEdit.GetText());				
					MD5_CTX context;
					_XMD5Init (&context);
					_XMD5Update (&context, (unsigned char*)login_window->m_PWEdit.GetText(), len);
					_XMD5Final (digest, &context);

					memset( g_szAcclaimPW, 0, sizeof(g_szAcclaimPW) );

					unsigned int i;
					TCHAR szCode[5];
					for (i = 0; i < 16; i++)
					{
						_snprintf(szCode, sizeof(szCode), "%02x", digest[i]);
						strcat( g_szAcclaimPW, szCode );
					}
				}	
				else
				{
					_autologinflag = TRUE;
					login_window->m_IDEdit.ClearBuffer();
					login_window->m_IDEdit.AddString( g_szAcclaimID, strlen(g_szAcclaimID) );
				}
				
				g_OnetimeAutoLogin = FALSE;
				
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Connect to authentication server"), TRUE );
				this->Draw();
				
#define _XDEF_NEWACCLAIMLOGIN

#ifdef _XDEF_NEWACCLAIMLOGIN
				int result = AcclaimAuthentication( g_szAcclaimID, g_szAcclaimPW );

				switch( result ) 
				{
				case 0 : 
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Connected to the authentication server."), TRUE );
					break;
				case 1 :
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_INCORRECTID), TRUE );					
					return;
					//break;
				case 2 :
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_WRONGPASSWD), TRUE );
					return;
					//break;
				case 3 :
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("The account is currently restricted from logging in."), TRUE );
					return;
					//break;
				case 4 :
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("The account is currently restricted from logging in."), TRUE );
					return;
					//break;
				case 99 :
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Acclaim authentication : System error has occurred."), TRUE );
					return;
					//break;
				default:
					{
						TCHAR errormessage[128];
						_snprintf( errormessage, sizeof(errormessage), _T("An unknown error has occurred. errorcode = %d"), result );
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), errormessage, TRUE );
					}
					return;
				}
#else
				Request	webrequest;
				char			szMessage[2048];
				char			szHeaderSend[2048];
				char			szHeaderReceive[2048];
				char			szURL[512];
								
				memset( szURL, 0, sizeof(szURL) );
				_snprintf( szURL, sizeof(szURL), _T("http://system.acclaim.com/9dragons/authenticate.jsp?username=%s&pwd=%s"), g_szAcclaimID, g_szAcclaimPW );

				int returncode = webrequest.SendRequest( FALSE, (LPCTSTR)szURL, 2048, szHeaderSend, szHeaderReceive, szMessage );
				
				if( returncode != 0 )
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Could not connect the authentication server..."), TRUE );
					return;
				}

				if( strlen( szMessage ) <= 0 )
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Invalid data was recieved from the authentication server."), TRUE );
					_XLog( _T("Invalid data was recieved from the authentication server") );
					return;
				}

				char returncode1 = -1, returncode2 = -1, returncode3 = -1;
				sscanf( szMessage, _T("%c%c%c"), &returncode1, &returncode2, &returncode3 );

				int iretcode1 = -1, iretcode2 = -1;
				iretcode1 = returncode1 - _T('0');
				iretcode2 = returncode3 - _T('0');
				
				_XLog("returncode1 : %c iretcode1 : %d", returncode1, iretcode1);

				switch( iretcode1 ) 
				{
				case 0 : 
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Connected to the authentication server."), TRUE );
					break;
				case 1 :
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NETWORK_WRONGPASSWD), TRUE );
					return;
					break;
				case 2 :
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("Unauthorized account."), TRUE );
					return;
					break;
				case 3 :
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("The account is currently restricted from logging in."), TRUE );
					return;
					break;
				default:
					{
						TCHAR errormessage[128];
						_snprintf( errormessage, sizeof(errormessage), _T("An unknown web login error has occurred. errorcode = %d"), returncode1 );
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), errormessage, TRUE );
					}
					return;
				}
#endif // _XDEF_NEWACCLAIMLOGIN

				strncpy( m_IDString, g_szAcclaimID, sizeof(m_IDString) );
				strncpy( m_PWString, login_window->m_PWEdit.GetText(), sizeof(m_PWString) );
#else
				strncpy( m_IDString, login_window->m_IDEdit.GetText(), sizeof(m_IDString) );
				strncpy( m_PWString, login_window->m_PWEdit.GetText(), sizeof(m_PWString) );
#endif
				// 소문자로 변환하기 전에 레지스트리에 쓰기
				//_EncodeData( m_IDString, _ENCODEKEY );
				BOOL saveuserid = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SAVELASTUSERNAME, FALSE, TRUE );
				if( saveuserid )
				{
					g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTUSERNAME, m_IDString );
				}
				//_EncodeData( m_IDString, _ENCODEKEY );

				/*
				char *lowerstring;
				lowerstring = _strlwr( _strdup( m_IDString ) );
				strcpy( m_IDString, lowerstring );				
				free( lowerstring );				
				*/

#ifdef			_ACCLAIM_VERSION // acclaim version일 경우 Web 인증을 한다.

				BOOL IDPWCheck = FALSE;

				if( _autologinflag )
				{
					if( strlen(g_szAcclaimID) && strlen(g_szAcclaimPW) )
					{
						IDPWCheck = TRUE;
					}
				}
				else
				{
					if( strlen(m_IDString) && strlen(m_PWString) )
					{
						IDPWCheck = TRUE;
					}
				}		
				
				if( IDPWCheck )
#else				
	#ifdef _XNEXON_WEBLOGIN
				//#define _XNEXON_WEBLOGIN_ENABLERETRY

				#ifdef _XNEXON_WEBLOGIN_ENABLERETRY
				
				#else
				
				// 로그인 실패했을 경우를 위해 윈도우 원래대로 재정렬. 
                //Author : 양희왕 //breif : 실패해도 다시 접속 할수 있어야함
                BOOL readyautologin = TRUE;

		        int parampos = _XCmdLineOption("passport=",0,8);
		        if( parampos != 0 )
		        {
			        readyautologin = TRUE;
		        }		
		        else
		        {
			        readyautologin = FALSE;
		        }

				if( g_OnetimeAutoLogin )
				{
					if( readyautologin )
					{
						_XLog("LoginServer.cpp : Connect Failed");
						_XWindow_ServerStatus* pServerStatus_Window = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
						_XWindow_TestServerNotice* pTestServerNoticeWindow = (_XWindow_TestServerNotice*)m_WindowManager.FindWindow( _XDEF_WTITLE_TESTSERVERNOTICEWINDOW );

						pServerStatus_Window->SetWindowSize( 230, 448 );
						pServerStatus_Window->MoveWindow( (gnWidth>>1) - (230>>1), 70 );
						pServerStatus_Window->ShowWindow( !TRUE );

						if( pServerStatus_Window->m_pConnectButton )
						{
							pServerStatus_Window->m_pServerListUpdateButton->MoveWindow( pServerStatus_Window->m_pServerListUpdateButton->GetWindowPos().x, 
																						 pServerStatus_Window->GetWindowPos().y + pServerStatus_Window->GetWindowSize().cy - 39);

							pServerStatus_Window->m_pConnectButton->MoveWindow( pServerStatus_Window->m_pConnectButton->GetWindowPos().x, 
																				pServerStatus_Window->GetWindowPos().y + pServerStatus_Window->GetWindowSize().cy - 39);
							pServerStatus_Window->m_pConnectButton->ShowWindow( TRUE );
						}

						if( pServerStatus_Window->m_pCloseServerlistButton )
							pServerStatus_Window->m_pCloseServerlistButton->ShowWindow( FALSE );

						//pLogin_Window->ShowWindow( FALSE );			

						if(pTestServerNoticeWindow)
							pTestServerNoticeWindow->ShowWindow( FALSE );
						
						m_OneTimeNoticeViewFlag = FALSE;
					}
					else
					{
						_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
						if( pWindow )
						{
							FLOAT yfactor = (FLOAT)gnHeight / 768.0f;
							int halfsize = 487>>1;	
			
							pWindow->ShowWindow( FALSE );
							pWindow->SetWindowSize( 230, 487 );
							pWindow->MoveWindow( 33, (int)((85.0f+halfsize)*yfactor) - halfsize );

							if( pWindow->m_pConnectButton )
							{
								pWindow->m_pConnectButton->ShowWindow( FALSE );

								pWindow->m_pServerListUpdateButton->MoveWindow( pWindow->m_pServerListUpdateButton->GetWindowPos().x, 
																				pWindow->m_pCloseServerlistButton->GetWindowPos().y );
								pWindow->m_pCloseServerlistButton->ShowWindow( TRUE );
							}
						}
						g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "로그인에 실패하였습니다. 프로그램을 종료하고 다시 실행하여 주십시오." ), TRUE, _XDEF_LS_LOGINFAILED);			
					}
				}
				

				#endif

				//login_window->ShowWindow( TRUE );

				BOOL IDPWCheck = FALSE;
				if( g_OnetimeAutoLogin )
				{
					IDPWCheck = TRUE;
				}
				else
				{
					if( strlen(m_IDString) && strlen(m_PWString) ) // 넥슨 web login일 경우 id password 길이 체크 안함.
					{
						IDPWCheck = TRUE;
					}
				}

				if( IDPWCheck )
	#else
				if( strlen(m_IDString) && strlen(m_PWString) )
	#endif
#endif
				{	
					if( m_PrevSendedLoginMessageTime == 0 )
					{
						m_PrevSendedLoginMessageTime = g_LocalSystemTime - 5001;
					}
					
					// 5초 동안 재 접속 불가 
					if( (g_LocalSystemTime - m_PrevSendedLoginMessageTime) > 5000 )
					{
						memset(g_pLocalUser->m_UserName, 0, sizeof(g_pLocalUser->m_UserName));
						memset(g_pLocalUser->m_UserPW, 0, sizeof(g_pLocalUser->m_UserPW));
						
						strncpy( g_pLocalUser->m_UserName, m_IDString, sizeof(g_pLocalUser->m_UserName) );
						strncpy( g_pLocalUser->m_UserPW, m_PWString, sizeof(g_pLocalUser->m_UserPW) );
						
						// Send connecting message to server...
						if( !g_NetworkKernel.m_bLoginServerConnected )
						{	
							m_PrevSendedLoginMessageTime		= g_LocalSystemTime;

							if( g_NetworkKernel.InitializeNetwork() )
							{
								if( g_NetworkKernel.InitializeUDPSocket() )
								{
									
								}
							}
						}
					}
				} 
				else
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_LOGINSERVER_INPUTIDPASSWD, TRUE, _XDEF_LS_LOGINFAILED );
				}

				/*
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
				{
					login_window->m_PWEdit.ResetContainer();
					login_window->m_PWEdit.m_VKeyboardTypedCount = 0;
				}
				*/
			}
			break;
		case _XDEF_LS_CANCELLOGINSERVER:
			{
				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				m_PrevSendedLoginMessageTime = 0;
				m_PrevSendedSelectForectTime = 0;

				_XWindow_SelectServerForest* pSelectServerForestWindow = 
						(_XWindow_SelectServerForest*)m_WindowManager.FindWindow( _XDEF_WTITLE_SELECTSERVERFOREST );
				
				if( pSelectServerForestWindow )
				{				
					pSelectServerForestWindow->ShowWindow( FALSE );
				}


				if( strlen(m_IDString) == 0 )
					login_window->m_IDEdit.SetFocus();
				else
					login_window->m_PWEdit.SetFocus();

				if( m_ConnectMode == _XDEF_SERVERCONNECTMODE_LOGIN )
				{
					g_NetworkKernel.ReleaseUDPSocket();
					g_NetworkKernel.InitializeNetwork();
				}
				else
				{
					g_NetworkKernel.ReleaseNetwork();
					g_NetworkKernel.InitializeNetwork();
					m_ConnectMode = _XDEF_SERVERCONNECTMODE_LOGIN;
				}
			}
			break;
		// Success connect to login server..
		case _XDEF_LS_DOCONNECTGAMESERVER:
			{	
				SetFocus( gHWnd );
				
				// 로그인 서버에 접속후 정상 처리가 되면 부여된 게임서버에 접속함.
				g_NetworkKernel.ReleaseUDPSocket();

#ifdef _XDEF_NEXONLOGIN // Nexon login
				
#else
				m_ServerLoginRequestTimeCheckFlag	= TRUE;
				m_ServerLoginRequestTime			= 0;
#endif
				m_ConnectMode						= _XDEF_SERVERCONNECTMODE_GAMESERVER;
								
				if( g_NetworkKernel.ConnectServer() )
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_LOGINSERVER_LOGINGAMESERVER,
														 TRUE, 0, 0, _XDEF_LS_CANCELLOGINSERVER );
					g_MessageBox.SetMessageColor();					
				}				
				else // 네트워크 초기화에 실패하면 로그인 서버 접속을 준비함 (사실상 프로그램 종료)
				{
					m_ServerLoginRequestTimeCheckFlag	= FALSE;
					m_ServerLoginRequestTime			= 0;
					m_ConnectMode						= _XDEF_SERVERCONNECTMODE_LOGIN;
				}				
			}
			break;
		/*
		case  _XDEF_SELECTSERVERLISTCTRL :
			{				
				if( m_PrevSendedSelectForectTime == 0 )
				{
					m_PrevSendedSelectForectTime = g_LocalSystemTime - 5001;
				}
				
				// 5초 동안 재 시도 불가 
				if( (g_LocalSystemTime - m_PrevSendedSelectForectTime) > 5000 )
				{
					m_PrevSendedSelectForectTime = g_LocalSystemTime;
					g_NetworkKernel.UDPSendPacket( MSG_NO_LOGIN_FOREST_SELECT, lparam );
				}
			}
			break;
		*/
		// Success connect to server..
		case _XPROCDEF_CONNECTEDSERVER:
			{
				if(g_NetworkKernel.m_bConnected)
				{
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;

					SetFocus( gHWnd );

					m_LoginProcess = LOGINPROC_END;

#ifdef _XTS_SERVERUNIFICATION
					int selectedservernum = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0, FALSE );
					if( selectedservernum == _GR_INDEX_UNION_ )
					{
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, _T("통합서버에 접속했습니다."), TRUE );

						((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_serverunification );
					}
					else
					{											
						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_LOGINSERVER_CONNECTGAMESERVER, TRUE );					

						((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_PrevProcess_Login = TRUE;
						memset( ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName, 0, sizeof(TCHAR) * 13 );
						((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter );
					}					
#else
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_LOGINSERVER_CONNECTGAMESERVER, TRUE );					

					((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_PrevProcess_Login = TRUE;
					memset( ((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.m_NewCreatedCharacterName, 0, sizeof(TCHAR) * 13 );
					((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess( &((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter );
#endif
					
					_XPlayInterfaceSound( ID_SR_INTERFACE_STARTGAME_WAV );
				}
			}
			break;
		case _XDEF_LS_EXITBUTTON:
			{			
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_LOGINSERVER_EXITGAME, TRUE, _XDEF_LS_EXITPROGRAM );
				g_MessageBox.StartTimer( 20000, TRUE );
			}
			break;
		case _XDEF_LS_EXITPROGRAM:
			{				
				PostMessage( gHWnd, WM_CLOSE, 0, 0 );
			}
			break;
		case _XDEF_LS_LOGINFAILED:

			g_NetworkKernel.ReleaseUDPSocket();
			g_NetworkKernel.DisconnectServer();

			m_ServerLoginRequestTimeCheckFlag = FALSE;
			m_ServerLoginRequestTime		  = 0;
			m_ConnectMode					  = _XDEF_SERVERCONNECTMODE_LOGIN;

			if( strlen(m_IDString) == 0 )
				login_window->m_IDEdit.SetFocus();
			else
				login_window->m_PWEdit.SetFocus();
			break;
		case _XDEF_LS_SERVERREFRESHBUTTON :
			{
				_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
				if( pWindow )
				{
					if( pWindow->m_pServerListUpdateButton )
					{
						pWindow->m_pServerListUpdateButton->EnableWindow( FALSE );
					}

					GetServerList( TRUE );
				}				
			}
			break;
		case _XDEF_LS_SERVERLISTCLOSEBUTTON :
			{
				_XWindow* pWindow = m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
				if( pWindow )
				{
					pWindow->ShowWindow( FALSE );
				}
			}
			break;
		case _XDEF_LS_VIEWSERVERLISTBUTTON :
			{
				_XWindow* pWindow = m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
				if( pWindow )
				{					
					pWindow->ShowWindow( !pWindow->GetShowStatus() );
				}
			}
			break;
		case _XDEF_LS_TESTSERVERNOTICEWINDOWCLOSEBTN :
			{
				_XWindow* pWindow = m_WindowManager.FindWindow( _XDEF_WTITLE_TESTSERVERNOTICEWINDOW	 );
				if( pWindow )
				{					
					pWindow->ShowWindow( FALSE );
					m_OneTimeNoticeViewFlag = FALSE;
				}
			}
			break;
		case _XDEF_LS_SAVELASTLOGINUSER :
			{
				BOOL saveid = (BOOL)lparam;
				g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SAVELASTUSERNAME, saveid );
				if( !saveid )
				{
					g_RegManager._XRegWriteString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTUSERNAME, _T("\0") );
				}
			}
			break;
		case _XDEF_LS_DRAWKEYBOARD :
			{
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
				{
					_XWindow* pDrawKeyboardWindow = m_WindowManager.FindWindow( _XDEF_WTITLE_DRAWKEYBOARDWINDOW );					
					if( pDrawKeyboardWindow )
					{
						if( pDrawKeyboardWindow->GetShowStatus() )				
							pDrawKeyboardWindow->ShowWindow(FALSE);
						else
							pDrawKeyboardWindow->ShowWindow(TRUE);
					}
				}
			}
			break;
		case _XDEF_LS_COMPULSORYSTEP1YES :
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("서버통합이 완료되지 않았습니다. 서버 통합을 하려면 먼저 서버 리스트에서 “서버 통합” 서버를 선택하셔야 합니다."),
						TRUE, _XDEF_LS_COMPULSORYSTEP2YES );
			}
			break;
		case _XDEF_LS_COMPULSORYSTEP2YES :
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("강제접속 중입니다. 강제접속은 구 서버에 캐릭터가 없는 신규유저를 위한 접속입니다. 강제 접속 시 구서버의 캐릭터들은 삭제됩니다."),
						TRUE, _XDEF_LS_COMPULSORYSTEP3YES );
			}
			break;
		case _XDEF_LS_COMPULSORYSTEP3YES :
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, _T(""), _T("그래도 강제 접속을 하시겠습니까? 서버 통합을 하려면 먼저 서버 리스트에서 “서버 통합”서버를 선택하셔야 합니다."),
						TRUE, _XDEF_LS_COMPULSORYSTEP4YES );
			}
			break;
		case _XDEF_LS_COMPULSORYSTEP4YES :
			{
				g_MessageBox.SetPasswordMode( TRUE );
				g_MessageBox.m_IMEControl.SetMaxLength(7);
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_INPUTSTRING, ID_STRING_SELECTCHARACTER_INPUTPASSWD, TRUE, _XDEF_LS_COMPULSORYSTEP5YES );
			}
			break;
		case _XDEF_LS_COMPULSORYSTEP5YES :
			{
				g_NetworkKernel.SendPacket( MSG_NO_UNI_GROUP_LOBBY, en_ug_force_use_charac_in_new );
			}
			break;
		}
	}
}

void XProc_LoginServer::OnUserMessage_UDPSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{	
		int errorcode = WSAGetLastError();

		if( errorcode == WSAEWOULDBLOCK )
		{
			_XLog( "WARNING : LS : TCP I/O blocking");
			Sleep( 50 );
		}
		else
		{
			if( errorcode != ERROR_IO_PENDING )
			{
				if( !g_NetworkKernel.m_bConnected )
				{
					g_NetworkKernel.ProcessUDPNetworkError();
					_XLog( "ERROR : XProc_LoginServer:OnUserMessage_UDPSocket::Disconnected" );
				}
				return;
			}
			else
			{
				_XLog( "WARNING : LSUDP : TCP I/O pending" );
				Sleep( 50 );
			}
		}
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{
	case FD_ACCEPT:		
		break;
	case FD_CONNECT:		
		WSAAsyncSelect(g_NetworkKernel.m_hLoginUDPSocket, gHWnd, WM_USER_UDPSOCKET, FD_READ | FD_CLOSE );
		
		g_NetworkKernel.m_bLoginConnected = TRUE;

		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_LOGINSERVER_CONNECTLOGINSERVER,
								 TRUE, 0, 0, _XDEF_LS_CANCELLOGINSERVER );
		g_MessageBox.SetMessageColor();
		
		// Connect  to login server...							
		//g_NetworkKernel.UDPSendPacket( MSG_NO_LOGIN );
		
		m_ServerLoginRequestTime		  = 0;
		m_ServerLoginRequestTimeCheckFlag = TRUE;

		break;
	case FD_READ :
		g_NetworkKernel.UDPReadPacket();
		break;
	case FD_CLOSE :		
		//g_NetworkKernel.ProcessNetworkError();		
		_XDWINPRINT( "SERVER MESSAGE : Login socket close message" );
		break;
	}
}

void XProc_LoginServer::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{		
		int errorcode = WSAGetLastError();

		if( errorcode == WSAEWOULDBLOCK )
		{
			_XLog( "WARNING : LS : TCP I/O blocking");
			Sleep( 50 );
		}
		else
		{
			if( errorcode != ERROR_IO_PENDING )
			{
				if( !g_NetworkKernel.m_bConnected )
				{
					g_NetworkKernel.ProcessNetworkError();
					return;
				}
			}
			else
			{
				_XLog( "WARNING : LS : TCP I/O pending" );
				Sleep( 50 );
			}
		}
	}

	switch (WSAGETSELECTEVENT(lparam)) 
	{
	case FD_CONNECT:
		WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_READ | FD_CLOSE );
		g_NetworkKernel.m_bConnected = TRUE;
		_XDWINPRINT( "Connected server" );
				
		// 게임서버에 접속되면 서버측에 엔터링 메세지 전달
		//g_NetworkKernel.SendPacket( MSG_NO_SV_ENTER_GAME );
		break;
	case FD_READ :
		g_NetworkKernel.ReadSocket();
		break;
	case FD_CLOSE :
		g_NetworkKernel.ProcessNetworkError();		
		_XDWINPRINT( "SERVER MESSAGE : Socket close message" );
		break;
	}
}

void XProc_LoginServer::OnUserMessage_GetHost( UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{		
		g_NetworkKernel.ProcessNetworkError();
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, "Network error", " ", TRUE );
		g_MessageBox.SetMessage( "Failed get host name.[_ECode:%d]", WSAGETASYNCERROR(lparam) );	
		g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );		
		_XUpdateWindow();		
		return;
	}
	else if (g_NetworkKernel.m_hClientSocket != NULL) 
	{
		struct sockaddr_in addr;
		struct hostent *pHost = (struct hostent*) g_NetworkKernel.m_HostBuffer;

		memset(&addr, 0x00, sizeof(addr));
		addr.sin_family = AF_INET;
		memcpy(&addr.sin_addr.s_addr, pHost->h_addr_list[0], pHost->h_length);
		addr.sin_port = htons(g_NetworkKernel.m_SERVER_PORT);
		WSAAsyncSelect(g_NetworkKernel.m_hClientSocket, gHWnd, WM_USER_SOCKET, FD_CONNECT);

		if (connect(g_NetworkKernel.m_hClientSocket, (struct sockaddr*) &addr, sizeof(addr)) == SOCKET_ERROR) 
		{
			int nError = WSAGetLastError();

			if (nError == WSAEWOULDBLOCK)
			{

			}
			else
			{	
				g_NetworkKernel.ProcessNetworkError();				
				return;
			}
		}

		_XDWINPRINT( "Succeed query host name." );
	}
}
	
bool XProc_LoginServer::OnKeyboardPoll(_XInput *pInput)
{ 
	return true; 
}

bool XProc_LoginServer::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
	return true;
}

bool XProc_LoginServer::OnKeyDown(WPARAM wparam, LPARAM lparam)
{ 	
	if( ::g_CurrentFocusedObject ) return true;
	if( (U32)wparam == VK_RETURN )
	{
		
	}
	else if( (U32)wparam == VK_ESCAPE )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_LS_EXITBUTTON, 0), (LPARAM)gHWnd);
	}
	else if( (U32)wparam == VK_F2 )
	{
		//gApp->Pause(true);
		//gApp->UserSelectNewDevice();
		//gApp->Pause(false);
	}	
	/*else if( (U32)wparam == VK_LCONTROL )
	{		
		if(g_BgmPlayFlag && g_BgmObject.Getm_dwDeviceID())  
		   g_BgmObject.Close();

		g_BgmPlayFlag = FALSE;
	}*/
	else if( (U32)wparam == VK_F9 )
	{
		g_BgmPlayFlag = !g_BgmPlayFlag;
		g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, g_BgmPlayFlag );

#ifdef _XUSEFMOD
		if( g_BgmPlayFlag )
		{
			g_FMODWrapper.PlayBGM( "Title.ogg" );
		}
		else
		{
			g_FMODWrapper.StopBGM();
		}
#endif

	}
	else if( (U32)wparam == VK_CAPITAL )
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		{
			_XWindow_DrawKeyboard* pDrawKeyBoard_Window = (_XWindow_DrawKeyboard*)m_WindowManager.FindWindow(_XDEF_WTITLE_DRAWKEYBOARDWINDOW);
			if( pDrawKeyBoard_Window )
			{
				if( pDrawKeyBoard_Window->GetShiftDownValue() == 48 )
					pDrawKeyBoard_Window->SetShiftDownValue(0);
				else
					pDrawKeyBoard_Window->SetShiftDownValue(48);
			}				
		}
	}
	
	return true; 
}

bool XProc_LoginServer::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{ 	
	return true; 
}

void XProc_LoginServer::OnMouseWheel( short zDelta )
{
	m_WindowManager.ProcessWheelMessage(zDelta);
}

bool XProc_LoginServer::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{
	if( ::g_CurrentFocusedObject ) return true;

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
		if( bPushed )
		{
			
		}
		
		break;
	}

	return true; 
}	


void XProc_LoginServer::OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam )
{
		
}

void XProc_LoginServer::OnWindowSizeChangeNotify( void )
{
	_XWindow_Login* login_window = (_XWindow_Login *)m_WindowManager.FindWindow(_XDEF_WTITLE_LOGIN);

	login_window->MoveWindow( (gnWidth>>1) - (269>>1) , gnHeight-154 );
	g_MessageBox.MoveWindow((gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1), (gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1) - 80 );
	m_CIBILogoImage.MoveWindow( (gnWidth>>1) - 140, gnHeight-28 );
}

void XProc_LoginServer::DrawFadeInOut( D3DCOLOR color )
{
	D3DCOLOR alphacolor = ((DWORD) g_FadeInOutLevel << 24) | (color&0xFFFFFF);

	g_ScreenBurnOut[0].color = alphacolor;
	g_ScreenBurnOut[1].color = alphacolor;
	g_ScreenBurnOut[2].color = alphacolor;
	g_ScreenBurnOut[3].color = alphacolor;

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

void XProc_LoginServer::ProcessEnvironment(void)
{
	//g_LodTerrain.m_FogDensity = 0.097f;
	//gpDev->SetRenderState(D3DRS_FOGDENSITY, g_LodTerrain.m_FogDensity);
}

void __stdcall _XCallback_LoginButton( void )
{	
	_XWindow_Login* pLogin_Window = 
		(_XWindow_Login *)((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_WindowManager.FindWindow(_XDEF_WTITLE_LOGIN);
	POINT pos = pLogin_Window->m_LoginButton->GetWindowPos();
	
	TCHAR messagestring[64];
	LoadString( gHInst, ID_STRING_LOGINSERVER_LOGIN, messagestring, 63 );
	g_InterfaceTooltip.SetToolTipString( pos.x+42, pos.y-12, messagestring, _XSC_INFORMATION );		
}

void __stdcall _XCallback_ProgramExit( void )
{
	_XWindow_Login* pLogin_Window = 
		(_XWindow_Login *)((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_WindowManager.FindWindow(_XDEF_WTITLE_LOGIN);
	POINT pos = pLogin_Window->m_ExitButton->GetWindowPos();	

	TCHAR messagestring[64];
	LoadString( gHInst, IDS_TOOLTIP_ENDGAME, messagestring, 63 );
	g_InterfaceTooltip.SetToolTipString( pos.x+28, pos.y-14, messagestring, _XSC_INFORMATION );		
}

int XProc_LoginServer::GetServerList( BOOL alwaysviewwindow )
{
	#ifdef _X_US_EXPANDSERVERLIST
		if( m_LastSelectedRegion_Server < 0)
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1815), TRUE );
			return -1;
		}
	#endif
	
	if( m_LastUpdateServerListTime == 0 )
	{
		m_LastUpdateServerListTime = g_LocalSystemTime;
	}
	else
	{
		if( g_LocalSystemTime - m_LastUpdateServerListTime < 1500 )
		{
			return 0;
		}
		else
		{
			m_LastUpdateServerListTime = g_LocalSystemTime;
		}
	}

	m_MaxServerCount = 0;

	char    pRecvBuf[1024] = {0};
	memset( pRecvBuf, 0, sizeof(char) * 1024 );
	
	m_LastSelectedServer = -1;
		
	// 1. 윈속 라이브러리 로드 및 초기화
	int			nReturn		= 0;
	WSADATA		wsadata		= {0};
	
	_XLog( "WSAStartup...\n" );
	nReturn = WSAStartup(MAKEWORD(2,2), &wsadata);
	if (nReturn != 0)
	{
		_XFatalError( "Failed WSAStartup. Error No.%d\n", WSAGetLastError() );
		return -1;
	}

	sockaddr_in	addr		= {0};
	int			addrlen		= sizeof(addr);
	SOCKET		clientsock	= INVALID_SOCKET;

	// 2. 소켓 커널 오브젝트 생성
	_XLog( "Request server list\n" );
	_XLog( "Create socket object...\n" );
	clientsock = socket(AF_INET, SOCK_STREAM, 0);
	if (clientsock == INVALID_SOCKET)
	{
		_XFatalError( "Failed to socket intialize. Error No.%d\n", WSAGetLastError());
		closesocket(clientsock) ;
		
		_XLog( "WSA Cleanup...\n" );
		WSACleanup();

		return -1;
	}
	_XLog(  "Created socket object...\n" );
	
	// 3. 소켓에 IP 주소 부여
	
	char registrybuffer[_REGSTRING_MAXLENGTH];
	memset( registrybuffer, 0, _REGSTRING_MAXLENGTH );

	/*if( m_TestServerConnectMode )
	{
		g_RegManager._XRegReadString( ID_REGSTR_MASTERKEY, ID_REGSTR_INDYKEY, ID_REGSTR_TESTVERSIONSERVERADDRESS, "221.147.34.21", registrybuffer, _REGSTRING_MAXLENGTH, TRUE);
	}
	else*/
	{
	#ifdef _X_US_EXPANDSERVERLIST
		if( m_LastSelectedRegion_Server == 0)
		{
			g_ServerAddressManager.GetServerInfo();		
			strcpy( registrybuffer,  g_ServerAddressManager.m_ServerInfo.patchserveraddress );
		}
		else // Europe login server
		{
			// login address 변경 : 2008.05.15 박소현
			strcpy( registrybuffer,  _T("85.17.144.236") );
//			strcpy( registrybuffer,  _T("85.17.144.237") );
		}
	#else
		g_ServerAddressManager.GetServerInfo();		
		strcpy( registrybuffer,  g_ServerAddressManager.m_ServerInfo.patchserveraddress );
	#endif
	}
	
	TCHAR loginipadress[256];
	memset( loginipadress, 0, sizeof(TCHAR) * 256 );
	
	if( !_XIsIPv4Address( registrybuffer, strlen(registrybuffer) ) )
	{		
		if( !_XGetHostName( registrybuffer, loginipadress, 256 ) )
		{
			_XFatalError( "Failed to get host name" );
			return FALSE;
		}
	}
	else
	{
		strcpy( loginipadress, registrybuffer );
	}

	addr.sin_family = AF_INET;	
	addr.sin_addr.s_addr = inet_addr(loginipadress);	
	addr.sin_port = htons(50001);

	// 4. 서버 접속 
	_XLog(  "Connect version server...\n" );
	
	nReturn = connect(clientsock, (sockaddr*)&addr, addrlen);
	if (nReturn == SOCKET_ERROR)
	{
		int nError = WSAGetLastError();		
		
		if( nError != WSAEWOULDBLOCK )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1817), TRUE );
			g_MessageBox.SetMessageColor( D3DCOLOR_ARGB(255,255,0,0) );

			_XLog(  "ERROR : connect failed. Error No.%d\n", WSAGetLastError());
			//closesocket(clientsock) ;
			_XLog( "WSA Cleanup...\n" );
			WSACleanup();
			return -1;
		}
	}
	_XLog(  "Connected version server...\n" );
	
	_XLog(  "Recieve version...\n" );
	
	char* pCurrentRecieveBuffer = pRecvBuf;
	
	DWORD connecttime = timeGetTime();//g_LocalSystemTime;

	DWORD totalrecievesize = 0;

	while ( 1 ) 
	{	
		nReturn = recv( clientsock, pCurrentRecieveBuffer, sizeof(char) * 1024, NULL );
		if ( nReturn < 1 ) 
		{
			_XLog(  "ERROR : Recieve failed patch number : %d \n", GetLastError() );
			_XLog( "WSA Cleanup...\n" );
			WSACleanup();
			return -1;
		}

		totalrecievesize += nReturn;
		pCurrentRecieveBuffer += nReturn;

		if( totalrecievesize >= sizeof(MSG_LOGIN_FOREST_LIST) )
		{
			break;
		}

		if( (timeGetTime() - connecttime) > 20000 )
		{
			_XLog( "ERROR : Connect time out" );

			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1817), TRUE );
			g_MessageBox.SetMessageColor( D3DCOLOR_ARGB(255,255,0,0) );
			
			_XLog(  "Close socket...\n" );
			closesocket(clientsock);
			_XLog( "WSA Cleanup...\n" );
			WSACleanup();
			return -2;			
		}
	}
	_XLog(  "Recieved version...\n" );
	
	MSG_LOGIN_FOREST_LIST* pLoginForestList = (MSG_LOGIN_FOREST_LIST*)pRecvBuf;
	
	if( pLoginForestList->ForestInfo.bServerRun == 0 )
	{
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1818), TRUE );	
		g_MessageBox.SetMessageColor( D3DCOLOR_ARGB(255,255,0,0) );		
		
		_XLog(  "Close socket...\n" );
		closesocket(clientsock);
		_XLog(  _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_1818) );	
		_XLog( "WSA Cleanup...\n" );
		WSACleanup();
		return -2;
	}

	_XLog(  "Close socket...\n" );
	closesocket(clientsock);
	_XLog( "WSA Cleanup...\n" );
	WSACleanup();
	
	int serverversion = pLoginForestList->ForestInfo.nVersion;

#ifdef _XTESTSERVER
	if( _XCmdLineOption("xinnertestserverlogin",0) == 0 )
	{
		m_MaxServerCount = 1;//_COUNT_MAX_GROUP_;
		
		m_NextServerTrafficFactor[0].trafficfactor = pLoginForestList->ForestInfo.cForestState[1];

		m_NextServerTrafficFactor[0].serverindex = 0;
		m_ServerTrafficFactor[0].serverindex = 0;
		
		if( m_NextServerTrafficFactor[0].trafficfactor >= 0  )
		{
			m_LastSelectedServer = 0;
		}
		else
		{
			m_LastSelectedServer = -1;
		}
	}	
	else
	{
		int index = 0;
		for( int i = 0; i < _COUNT_MAX_GROUP_; ++i )
		{
			if( pLoginForestList->ForestInfo.cForestState[i] != -1 )
			{
				m_NextServerTrafficFactor[index].trafficfactor = pLoginForestList->ForestInfo.cForestState[i];
				m_NextServerTrafficFactor[index].serverindex = i;
				m_ServerTrafficFactor[index].serverindex = i;
				index++;
			}			
		}
		m_MaxServerCount = index;

		for( i = 0; i < m_MaxServerCount; i++ )
		{
			if( m_NextServerTrafficFactor[i].trafficfactor == _FOREST_STATE_OFF )
			{
				m_NextServerTrafficFactor[i].trafficfactor = 101;			
			}
			else if( m_NextServerTrafficFactor[i].trafficfactor == _FOREST_STATE_STOP )
			{
				m_NextServerTrafficFactor[i].trafficfactor = 102;
			}
			else if( m_NextServerTrafficFactor[i].trafficfactor == _FOREST_STATE_FULL )
			{
				m_NextServerTrafficFactor[i].trafficfactor = 103;
			}
		}

		for( i = 0; i < m_MaxServerCount; i++ )
		{
			for( int j = 1; j < m_MaxServerCount; j++ )
			{
				if( m_NextServerTrafficFactor[j-1].trafficfactor > m_NextServerTrafficFactor[j].trafficfactor )
				{
					_SWAP( m_NextServerTrafficFactor[j-1], m_NextServerTrafficFactor[j] );
				}
			}
		}

		for( i = 0; i < m_MaxServerCount; i++ )
		{
			if( m_NextServerTrafficFactor[i].trafficfactor == 101 )
			{
				m_NextServerTrafficFactor[i].trafficfactor = _FOREST_STATE_OFF;
			}
			else if( m_NextServerTrafficFactor[i].trafficfactor == 102 )
			{
				m_NextServerTrafficFactor[i].trafficfactor = _FOREST_STATE_STOP;
			}
			else if( m_NextServerTrafficFactor[i].trafficfactor == 103 )
			{
				m_NextServerTrafficFactor[i].trafficfactor = _FOREST_STATE_FULL;
			}
		}
		
		//g_PatchVersionManager.GetVersionInfo();
		//_XLog( "Version information server : %d,%d %d %d\n", serverversion, g_PatchVersionManager.m_VersionInfo.exeversion_1,g_PatchVersionManager.m_VersionInfo.exeversion_2,g_PatchVersionManager.m_VersionInfo.exeversion_4 );
		
		int lastselectedrealserverindex = (int)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, -1, FALSE );	

		if( lastselectedrealserverindex == -1 )
		{
			m_LastSelectedServer = -1;
			
			_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
			if( pWindow )
			{
				_XWindow::m_GlobalPlaySoundEffect = FALSE;
				pWindow->ShowWindow( TRUE );
				_XWindow::m_GlobalPlaySoundEffect = TRUE;
			}				
		}
		else
		{
			if( !alwaysviewwindow )
			{
				_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
				if( pWindow )
				{
					_XWindow::m_GlobalPlaySoundEffect = FALSE;
					pWindow->ShowWindow( FALSE );
					_XWindow::m_GlobalPlaySoundEffect = TRUE;
				}
			}
		}

		if( lastselectedrealserverindex > -1 )
		{
			for( i = 0; i < m_MaxServerCount; i++ )
			{
				if( m_NextServerTrafficFactor[i].serverindex == lastselectedrealserverindex )
				{
					m_LastSelectedServer = i;
				}
			}	

			if( m_LastSelectedServer > m_MaxServerCount )
			{
				m_LastSelectedServer = 0;
				g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0 );
			}
			
			if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 0 ) // 최종 선택된 서버가 서비스 불가인 경우
			{
				for( i = 0; i < m_MaxServerCount; i++ )
				{
					if( m_NextServerTrafficFactor[i].trafficfactor >= 0 )
					{
						m_LastSelectedServer = i;
						g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, m_NextServerTrafficFactor[i].serverindex );
						break;
					}
				}		
			}
		}
		else
		{ 
			m_LastSelectedServer = 0;//m_NextServerTrafficFactor[0].serverindex;
			g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, m_NextServerTrafficFactor[0].serverindex );
		}
	}

	//g_PatchVersionManager.GetVersionInfo();
	//_XLog( "Version information server : %d,%d %d %d\n", serverversion, g_PatchVersionManager.m_VersionInfo.exeversion_1,g_PatchVersionManager.m_VersionInfo.exeversion_2,g_PatchVersionManager.m_VersionInfo.exeversion_4 );
	
#else
	int index = 0;
	for( int i = 0; i < _COUNT_MAX_GROUP_; ++i )
	{
		if( pLoginForestList->ForestInfo.cForestState[i] != -1 )
		{
#ifdef _XDWDEBUG
#else

	#ifdef _XGMCLIENT
		// 넥슨 IDC 청홍서버 보여주도록 세팅
	#else

			// 임시로 구서버를 안보여줌
			if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
			{
				if( i < 1 || i > 9 )
				{
					m_NextServerTrafficFactor[index].trafficfactor = pLoginForestList->ForestInfo.cForestState[i];
					m_NextServerTrafficFactor[index].serverindex = i;
					m_ServerTrafficFactor[index].serverindex = i;
					index++;
				}			
			}
			else
	#endif

#endif
			{
				m_NextServerTrafficFactor[index].trafficfactor = pLoginForestList->ForestInfo.cForestState[i];
				m_NextServerTrafficFactor[index].serverindex = i;
				m_ServerTrafficFactor[index].serverindex = i;
				index++;
			}
		}			
	}
	m_MaxServerCount = index;	

	for( i = 0; i < m_MaxServerCount; i++ )
	{
		if( m_NextServerTrafficFactor[i].trafficfactor == _FOREST_STATE_OFF )
		{
			m_NextServerTrafficFactor[i].trafficfactor = 101;			
		}
		else if( m_NextServerTrafficFactor[i].trafficfactor == _FOREST_STATE_STOP )
		{
			m_NextServerTrafficFactor[i].trafficfactor = 102;
		}
		else if( m_NextServerTrafficFactor[i].trafficfactor == _FOREST_STATE_FULL )
		{
			m_NextServerTrafficFactor[i].trafficfactor = 103;
		}
	}

	for( i = 0; i < m_MaxServerCount; i++ )
	{
		for( int j = 1; j < m_MaxServerCount; j++ )
		{
			if( m_NextServerTrafficFactor[j-1].trafficfactor > m_NextServerTrafficFactor[j].trafficfactor )
			{
				_SWAP( m_NextServerTrafficFactor[j-1], m_NextServerTrafficFactor[j] );
			}
		}
	}

	for( i = 0; i < m_MaxServerCount; i++ )
	{
		if( m_NextServerTrafficFactor[i].trafficfactor == 101 )
		{
			m_NextServerTrafficFactor[i].trafficfactor = _FOREST_STATE_OFF;
		}
		else if( m_NextServerTrafficFactor[i].trafficfactor == 102 )
		{
			m_NextServerTrafficFactor[i].trafficfactor = _FOREST_STATE_STOP;
		}
		else if( m_NextServerTrafficFactor[i].trafficfactor == 103 )
		{
			m_NextServerTrafficFactor[i].trafficfactor = _FOREST_STATE_FULL;
		}
	}
	
	//g_PatchVersionManager.GetVersionInfo();
	//_XLog( "Version information server : %d,%d %d %d\n", serverversion, g_PatchVersionManager.m_VersionInfo.exeversion_1,g_PatchVersionManager.m_VersionInfo.exeversion_2,g_PatchVersionManager.m_VersionInfo.exeversion_4 );
	
	int lastselectedrealserverindex = (int)g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, -1, FALSE );	

	if( lastselectedrealserverindex == -1 )
	{
		m_LastSelectedServer = -1;
		
		_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
		if( pWindow )
		{
			_XWindow::m_GlobalPlaySoundEffect = FALSE;
			pWindow->ShowWindow( TRUE );
			_XWindow::m_GlobalPlaySoundEffect = TRUE;
		}				
	}
	else
	{
		if( !alwaysviewwindow )
		{
			_XWindow_ServerStatus* pWindow = (_XWindow_ServerStatus*)m_WindowManager.FindWindow( _XDEF_WTITLE_LOGINSERVERLIST );
			if( pWindow )
			{
				_XWindow::m_GlobalPlaySoundEffect = FALSE;
				pWindow->ShowWindow( FALSE );
				_XWindow::m_GlobalPlaySoundEffect = TRUE;
			}
		}
	}

	if( lastselectedrealserverindex > -1 )
	{
		for( i = 0; i < m_MaxServerCount; i++ )
		{
			if( m_NextServerTrafficFactor[i].serverindex == lastselectedrealserverindex )
			{
				m_LastSelectedServer = i;
			}
		}	

		if( m_LastSelectedServer > m_MaxServerCount )
		{
			m_LastSelectedServer = 0;
			g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, 0 );
		}
		
		if( m_NextServerTrafficFactor[m_LastSelectedServer].trafficfactor < 0 ) // 최종 선택된 서버가 서비스 불가인 경우
		{
			for( i = 0; i < m_MaxServerCount; i++ )
			{
				if( m_NextServerTrafficFactor[i].trafficfactor >= 0 )
				{
					m_LastSelectedServer = i;
					g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, m_NextServerTrafficFactor[i].serverindex );
					break;
				}
			}		
		}
	}
	else
	{
		m_LastSelectedServer = 0;//m_NextServerTrafficFactor[0].serverindex;
		g_RegManager._XRegWriteInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDSERVER, m_NextServerTrafficFactor[0].serverindex );
	}
#endif

	return 0;
}

void XProc_LoginServer::RebuildServerTrafficGraph( void )
{
	if( m_GraphUpdateTimer == 0 )
	{
		m_GraphUpdateTimer = g_LocalSystemTime;
	}
	else
	{
		if( g_LocalSystemTime - m_GraphUpdateTimer > 30 )
		{
			m_GraphUpdateTimer = g_LocalSystemTime;
		}
		else
		{
			return;
		}
	}

	for( int serverindex = 0; serverindex < m_MaxServerCount; serverindex++ )
	{
		if( m_ServerTrafficFactor[serverindex].trafficfactor != m_NextServerTrafficFactor[serverindex].trafficfactor)
		{
			if( m_ServerTrafficFactor[serverindex].trafficfactor< m_NextServerTrafficFactor[serverindex].trafficfactor)
			{
				m_ServerTrafficFactor[serverindex].trafficfactor+=8;	
				
				if( m_ServerTrafficFactor[serverindex].trafficfactor> m_NextServerTrafficFactor[serverindex].trafficfactor)
				{
					m_ServerTrafficFactor[serverindex].trafficfactor= m_NextServerTrafficFactor[serverindex].trafficfactor;
				}
			}
			else if( m_ServerTrafficFactor[serverindex].trafficfactor > m_NextServerTrafficFactor[serverindex].trafficfactor )
			{
				m_ServerTrafficFactor[serverindex].trafficfactor-=8;
				
				if( m_ServerTrafficFactor[serverindex].trafficfactor < m_NextServerTrafficFactor[serverindex].trafficfactor )
				{
					m_ServerTrafficFactor[serverindex].trafficfactor = m_NextServerTrafficFactor[serverindex].trafficfactor;
				}
				
				if( m_ServerTrafficFactor[serverindex].trafficfactor < 0 )
				{
					m_ServerTrafficFactor[serverindex].trafficfactor = 0;
				}
			}
		}
	}
}

#ifdef _XDEF_NEXONLOGIN // Nexon login
#include "NMPCH.h"
#include "NMManager.h"

#if defined(_XNEXON_WEBLOGIN) || defined(_XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA)

#include "NMEventHandler.h"

class CNMMessengerEventHandler : public CNMEventHandler
{
public:
/*
	virtual void OnRefreshEvent( CNMRefreshEvent* pEvent )
	{
		CNMRefreshEvent* pRefreshEvent = (CNMRefreshEvent*)pEvent;
		_XLog( _T( "NMRefreshEvent : %d" ), pRefreshEvent->uType );
	}

	virtual void	OnServerMessageEvent( CNMServerMessageEvent* pEvent )
	{
		_XLog( _T( "NMServerMessageEvent : %d" ), pEvent->msgData.nCode );
		
		switch ( pEvent->msgData.nCode )
		{
		case kSystemMsgCode_AdminNotice:
			g_MessageBox.SetMessage( pEvent->msgData.szMessage );			
			//MessageBox( strMessage, _T( "운영자 알림" ) );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
			
		case kSystemMsgCode_Advertisement:
			g_MessageBox.SetMessage( pEvent->msgData.szMessage );			
			//MessageBox( strMessage, _T( "광고" ) );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
			
		case kSystemMsgCode_ErrorMessage:
			g_MessageBox.SetMessage( pEvent->msgData.szMessage );
			//MessageBox( strMessage, _T( "에러" ), MB_ICONSTOP | MB_APPLMODAL );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
			
		case kSystemMsgCode_GeneralMessage:
			g_MessageBox.SetMessage( pEvent->msgData.szMessage );
			//MessageBox( strMessage, _T( "메시지" ) );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		}
	}

	virtual	void	OnMessengerReplyEvent( CNMMessengerReplyEvent* pEvent )
	{
		_XLog( _T( "NMLoginReplyEvent : %d" ), pEvent->nResultCode );
		
		switch( pEvent->nResultCode )
		{
		case kMessengerReplyOK:
		case kMessengerReplyNewUser:
			{
				g_MessageBox.SetMessage( _T("넥슨닷컴에 로그인 하였습니다. 서버에 접속합니다.") );
				
				//m_ServerLoginRequestTimeCheckFlag	= TRUE;
				//m_ServerLoginRequestTime			= 0;

				_XLog( "Try to login" );
				g_NetworkKernel.UDPSendPacket( MSG_NO_LOGIN );
			}
			break;
		case kMessengerReplyWrongId:
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_NOTEXISTACCOUNT, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		case kMessengerReplyWrongPwd:
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_WRONGPASSWD, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		case kMessengerReplyServiceShutdown:
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_SERVEROFF, TRUE, _XDEF_LS_LOGINFAILED );
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		case kMessengerReplyLockedByAnotherProcess:
			{
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "이미 다른 넥슨닷컴 아이디가 로그인 중입니다.\n넥슨닷컴은 한 컴퓨터에 대하여 동시에 한 아이디의 로그인만을 허용합니다."), TRUE, _XDEF_LS_LOGINFAILED);
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		case kMessengerReplyWrongOwner:
			{
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "아이디가 본인이 아님이 확인되어 사용이 중지된 아이디입니다." ), TRUE, _XDEF_LS_LOGINFAILED);
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		case kMessengerReplyBlockByAdmin:
			{
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "관리자에 의하여 영구 정지된 아이디입니다." ), TRUE, _XDEF_LS_LOGINFAILED);
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		case kMessengerReplyTempBlockByWarning:
			{
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "경고로 인하여 사용이 제한된 아이디입니다." ), TRUE, _XDEF_LS_LOGINFAILED);
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		case kMessengerReplyTempBlockByLoginFail:
			{
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "잘못된 비밀번호 3회 입력으로 10분간 로그인이 제한됩니다." ), TRUE, _XDEF_LS_LOGINFAILED);
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		case kMessengerReplyBlockedIp:
			{
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "특정 IP 및 운영체제에서는 로그인을 할 수 없습니다. 자세한 내용은 고객센터를 참고하세요." ), TRUE, _XDEF_LS_LOGINFAILED);
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		default:
			{
				g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "넥슨닷컴 로그인에 실패하였습니다." ), TRUE, _XDEF_LS_LOGINFAILED);
				g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
				((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			}
			break;
		}
	}
	
	virtual void OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent * pEvent )
	{
		_XLog( _T( "NMConnectionCloseEvent : %d" ), pEvent->uType );

		switch( pEvent->uType )
		{
		case CNMMsgConnectionClosedEvent::kType_Gracefully:
			// Real User를 Logout할 때 성공의 결과로 전달되는 이벤트 입니다.
			_XLog( _T( "Connection closed gracefully\r\n" ) );
			break;
		case CNMMsgConnectionClosedEvent::kType_ByServer:
			_XLog( _T( "Connection closed by Server\r\n" ) );
//			g_MessageBox.SetMessage( _T("넥슨닷컴 서버와의 접속이 해제되었습니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		case CNMMsgConnectionClosedEvent::kType_ByError:
			_XLog( _T( "Connection closed by Error\r\n" ) );
			g_MessageBox.SetMessage( _T("네트워크 에러로 인하여 넥슨닷컴 서버와의 접속이 해제 되었습니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		default:
			_XLog( _T( "CNMConnectionClosedEvent::Unknown Type\r\n" ) );
			g_MessageBox.SetMessage( _T("알수 없는 에러로 인하여 넥슨닷컴 서버와의 접속이 해제 되었습니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		}
		//ClientUtil::Log::LogTime( _T("%s, %d, %s"), _T(__FUNCTION__), pEvent->uType, ClientUtil::Convert::AuthResultToString( static_cast<NMLoginAuthReplyCode>( pEvent->uType ) ) );
	}
	*/
	
	virtual void OnAuthConnectionClosedEvent( CNMAuthConnectionClosedEvent * pEvent )
	{
		_XLog( _T( "NMAuthConnectionClosedEvent : %d" ), pEvent->uType );

		switch( pEvent->uType )
		{
		case CNMAuthConnectionClosedEvent::kType_SystemError :
			_XLog( _T( "Auth connection closed event : system error\r\n" ) );
			g_MessageBox.SetMessage( _T("넥슨닷컴 인증 서비스가 중지 되었습니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		case CNMAuthConnectionClosedEvent::kType_NetworkError:
			_XLog( _T( "Connection closed by Server\r\n" ) );
			g_MessageBox.SetMessage( _T("넥슨닷컴 인증 서버와의 네트워크 연결에 실패하였습니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		case CNMAuthConnectionClosedEvent::kType_InvalidUserIP:
			_XLog( _T( "Connection closed by Error\r\n" ) );
			g_MessageBox.SetMessage( _T("IP 주소가 넥슨패스포트 정보와 일치하지 않습니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		case CNMAuthConnectionClosedEvent::kType_InvalidPassport:
			g_MessageBox.SetMessage( _T("잘못된 패스포트입니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		case CNMAuthConnectionClosedEvent::kType_Disconnected :
			g_MessageBox.SetMessage( _T("다른 세션에 의해 로그아웃 되었습니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		default:
			_XLog( _T( "CNMConnectionClosedEvent::Unknown Type\r\n" ) );
			g_MessageBox.SetMessage( _T("알수 없는 에러로 인하여 넥슨닷컴 서버와의 접속이 해제 되었습니다.") );
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTimeCheckFlag = FALSE;
			((CEmperorOfDragonsApp*)gApp)->m_proc_loginserver.m_ServerLoginRequestTime		  = 0;
			break;
		}
	}
};


CNMMessengerEventHandler g_nmmessengereventhandler;

#endif


#endif

void XProc_LoginServer::OnNexonMessangerEvent( WPARAM wparam, LPARAM lparam ) // Nexon messanger event
{

#if defined(_XNEXON_WEBLOGIN) || defined(_XDEF_NEXONNEWTESTSERVERLOGIN_071125_KERYGMA)
		g_nmmessengereventhandler.HandleNMEvent( wparam, lparam );
#else
	#ifdef _XDEF_NEXONLOGIN // Nexon login

		CNMEvent*	pEvent = CNMUtil::CreateEventFromCodeAndBuffer( (_UInt32_) wparam, (BYTE*) lparam );
		
		if( !pEvent )
		{
			g_MessageBox.SetMessage( _T("넥슨닷컴에 로그인 할수 없었습니다. ") );
			_XLog( _T("ERROR : <NMEvent = NULL>") );
			return;
		}

		_XLog(_T("GetCode : %d "), pEvent->GetCode());

		if( pEvent->GetCode() == CNMRefreshEvent::kCode )
		{
			CNMRefreshEvent* pRefreshEvent = (CNMRefreshEvent*)pEvent;
			_XLog( _T( "NMRefreshEvent : %d" ), pRefreshEvent->uType );

	//		delete pEvent;
	//		return;
		}
		else if( pEvent->GetCode() == CNMServerMessageEvent::kCode )
		{
			CNMServerMessageEvent* pServerMessageEvent = (CNMServerMessageEvent*)pEvent;
			_XLog( _T( "NMServerMessageEvent : %d" ), pServerMessageEvent->msgData.nCode );

			switch ( pServerMessageEvent->msgData.nCode )
			{
			case kSystemMsgCode_AdminNotice:
				g_MessageBox.SetMessage( pServerMessageEvent->msgData.szMessage );			
				//MessageBox( strMessage, _T( "운영자 알림" ) );
				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				break;
				
			case kSystemMsgCode_Advertisement:
				g_MessageBox.SetMessage( pServerMessageEvent->msgData.szMessage );			
				//MessageBox( strMessage, _T( "광고" ) );
				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				break;
				
			case kSystemMsgCode_ErrorMessage:
				g_MessageBox.SetMessage( pServerMessageEvent->msgData.szMessage );
				//MessageBox( strMessage, _T( "에러" ), MB_ICONSTOP | MB_APPLMODAL );
				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				break;
				
			case kSystemMsgCode_GeneralMessage:
				g_MessageBox.SetMessage( pServerMessageEvent->msgData.szMessage );
				//MessageBox( strMessage, _T( "메시지" ) );
				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				break;
			}
		}
		else if( pEvent->GetCode() == CNMConnectionClosedEvent::kCode )
		{
			CNMConnectionClosedEvent* pConnectionCloseEvent = (CNMConnectionClosedEvent*)pEvent;

			_XLog( _T( "NMConnectionCloseEvent : %d" ), pConnectionCloseEvent->uType );

			switch( pConnectionCloseEvent->uType )
			{
			case CNMConnectionClosedEvent::kType_Gracefully:
				// Real User를 Logout할 때 성공의 결과로 전달되는 이벤트 입니다.
				_XLog( _T( "Connection closed gracefully\r\n" ) );
				break;
			case CNMConnectionClosedEvent::kType_ByServer:
				_XLog( _T( "Connection closed by Server\r\n" ) );
	//			g_MessageBox.SetMessage( _T("넥슨닷컴 서버와의 접속이 해제되었습니다.") );
				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				break;
			case CNMConnectionClosedEvent::kType_ByError:
				_XLog( _T( "Connection closed by Error\r\n" ) );
				g_MessageBox.SetMessage( _T("네트워크 에러로 인하여 넥슨닷컴 서버와의 접속이 해제 되었습니다.") );
				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				break;
			default:
				_XLog( _T( "CNMConnectionClosedEvent::Unknown Type\r\n" ) );
				g_MessageBox.SetMessage( _T("알수 없는 에러로 인하여 넥슨닷컴 서버와의 접속이 해제 되었습니다.") );
				m_ServerLoginRequestTimeCheckFlag = FALSE;
				m_ServerLoginRequestTime		  = 0;
				break;
			}
		}
		else if( pEvent->GetCode() == CNMLoginReplyEvent::kCode )
		{
			CNMLoginReplyEvent*	pLoginReplyEvent = (CNMLoginReplyEvent*)pEvent;

			_XLog( _T( "NMLoginReplyEvent : %d" ), pLoginReplyEvent->nResultCode );
			
			switch( pLoginReplyEvent->nResultCode )
			{
			case kLoginOK:
			case kLoginNewUser:
				{
					g_MessageBox.SetMessage( _T("넥슨닷컴에 로그인 하였습니다. 서버에 접속합니다.") );
					
					//m_ServerLoginRequestTimeCheckFlag	= TRUE;
					//m_ServerLoginRequestTime			= 0;

					_XLog( "Try to login" );
					g_NetworkKernel.UDPSendPacket( MSG_NO_LOGIN );
				}
				break;
			case kLoginWrongId:
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_NOTEXISTACCOUNT, TRUE, _XDEF_LS_LOGINFAILED );
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			case kLoginWrongPwd:
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_WRONGPASSWD, TRUE, _XDEF_LS_LOGINFAILED );
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			case kLoginServiceShutdown:
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_NETWORK_SERVEROFF, TRUE, _XDEF_LS_LOGINFAILED );
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			case kLoginLockedByAnotherProcess:
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "이미 다른 넥슨닷컴 아이디가 로그인 중입니다.\n넥슨닷컴은 한 컴퓨터에 대하여 동시에 한 아이디의 로그인만을 허용합니다."), TRUE, _XDEF_LS_LOGINFAILED);
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			case kLoginWrongOwner:
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "아이디가 본인이 아님이 확인되어 사용이 중지된 아이디입니다." ), TRUE, _XDEF_LS_LOGINFAILED);
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			case kLoginBlockByAdmin:
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "관리자에 의하여 영구 정지된 아이디입니다." ), TRUE, _XDEF_LS_LOGINFAILED);
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			case kLoginTempBlockByWarning:
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "경고로 인하여 사용이 제한된 아이디입니다." ), TRUE, _XDEF_LS_LOGINFAILED);
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			case kLoginTempBlockByLoginFail:
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "잘못된 비밀번호 3회 입력으로 10분간 로그인이 제한됩니다." ), TRUE, _XDEF_LS_LOGINFAILED);
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			case kLoginBlockedIp:
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "특정 IP 및 운영체제에서는 로그인을 할 수 없습니다. 자세한 내용은 고객센터를 참고하세요." ), TRUE, _XDEF_LS_LOGINFAILED);
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			default:
				{
					g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _T( "넥슨닷컴 로그인에 실패하였습니다." ), TRUE, _XDEF_LS_LOGINFAILED);
					g_MessageBox.SetMessageColor( D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
					m_ServerLoginRequestTimeCheckFlag = FALSE;
					m_ServerLoginRequestTime		  = 0;
				}
				break;
			}
		}

		SAFE_DELETE( pEvent );

	#endif

#endif

}
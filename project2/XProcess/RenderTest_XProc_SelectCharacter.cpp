// XProc_SelectCharacter.cpp: implementation of the XProc_SelectCharacter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "XProc_SelectCharacter.h"
#include "XWindow_SelectCharacter.h"
#include "XWindow_ZoomWindow.h"
#include "XWindow_RotateWindow.h"
#include "XWindow_Messenger.h"
#include "XWindow_ChangeName.h"
#include "XWindow_DrawKeyboard.h"

#include "XSR_STRINGHEADER.H"
#include "FlashControlInterface.h"
#include "XParticleIDDefine.h"
#include "SoundEffectList.h"

#ifdef _XDEF_NOTICE_STRING_PROCESS
	#include "Notice/XNoticeProcess.h"
#endif

//Author : 양희왕	
#if defined(_ACCLAIM_VERSION) /*|| defined(_XVIETNAMESE)*/ || defined(_XJAPANESE)
	#define _XENABLE_GAMEGUARD
#endif

#ifdef _XUSEAPEXMODULE
	#undef _XENABLE_GAMEGUARD
#endif

#if !defined(_XDWDEBUG) && !defined(_XADMINISTRATORMODE) && !defined(_DEBUG) && !defined(_XGMCLIENT)
	#ifdef _XENABLE_GAMEGUARD
		#define _XUSE_GAMEGUARD
	#endif
#endif
//Last updated: 07/01/22

#ifdef _XDEF_TESTRENDER
_XModelDescriptor g_TestRenderModelDescriptor;
#endif

extern CFlashControlInterface*	g_pTitleFlashObject;
extern LPDIRECT3DTEXTURE9		g_pTitleFlashOffScreenTexture;

//#define _XDEF_NEWANIMATIONLIST // 캐릭터 선택 및 2차경공 추가

#ifndef _XDEF_NEWANIMATIONLIST

int	g_PreviewCharacterStanceIndex[_XGROUP_MAXGROUPCOUNT][2][2] = 
{
	// 낭인
	{
		{  1320, 1320 },	//남자
		{  1314, 1314 }   //여자
	},
		// 개방 
	{
		{ 1321, 1322 },	//남자
		{ 1315, 1316 }   //여자
	},
		// 비궁
	{
		{  0, 0 },	//남자
		{  1317, 1318 }   //여자		
	},
		// 소림
	{
		{  1323, 1324 },	//남자
		{  0, 0 }   //여자		
	},
		// 녹림
	{
		{  1325, 1326 },	//남자
		{  1319, 1320 }   //여자
	},
		// 무당
	{
		{  1327, 1328 },	//남자
		{  1321, 1322 }   //여자
	},
		// 마교
	{
		{  1329, 1330 },	//남자
		{  1323, 1324 }   //여자
	}
};

#else


int g_SelectAnimationFrameTime = 0;

int	g_PreviewCharacterStanceMotionIndex[10] = 
{
	103, 2, 2, 2, 2, 2,	2, 4, 4, 2
};

int g_PreviewCharacterRefLUT[10][9] = 
{
	{ 1, 1, 1, 1, 1, 0, 1, 1, 1 }, // 검
	{ 1, 1, 1, 1, 1, 1, 0, 1, 1 }, // 도
	{ 1, 0, 1, 1, 1, 1, 1, 1, 1 }, // 봉
	{ 4, 0, 4, 1, 4, 2, 3, 4, 4 }, // 권각
	{ 1, 1, 0, 1, 1, 1, 1, 1, 1 }, // 단검
	{ 1, 1, 1, 1, 0, 1, 1, 1, 1 }, // 부
	{ 1, 1, 0, 1, 1, 1, 1, 1, 1 }, // 쌍환
	{ 1, 1, 1, 1, 0, 1, 1, 1, 1 }, // 창 
	{ 1, 1, 1, 0, 1, 1, 1, 1, 1 }, // 곤
	{ 6, 0, 1, 2, 3, 4, 5, 6, 6 }  // 강
};

int	g_PreviewCharacterMotionIndex[10][7][3] = 
{
	{ // 검 0
		{ 158,159,160 }, // 무당
		{ 188,189,194}   // ETC
	},
	{ // 도 1
		{ 161,162,163 }, // 마교
		{ 191,191,195 }  // ETC
	},
	{ // 봉 2
		{ 164,165,166 }, // 개방
		{ 191,191,196 }  // ETC
	},
	{ // 권각 3
		{ 167,168,169 }, // 개방
		{ 182,183,184 }, // 소림
		{ 170,171,172 }, // 무당
		{ 173,174,175 }, // 마교
		{ 190,190,197 }  // ETC
	},
	{ // 단검 4
		{ 182,183,184 }, // 비궁
		{ 191,191,198 }  // ETC
	},
	{ // 부 5
		{ 176,177,178 }, // 녹림
		{ 193,193,199 }  // ETC
	},
	{ // 쌍환 6
		{ 185,186,187 }, // 비궁
		{ 191,191,200 }  // ETC
	},
	{ // 창 7
		{ 179,180,181 }, // 녹림
		{ 192,192,201 }  // ETC
	},
	{ // 곤 8
		{ 185,186,187 }, // 소림
		{ 192,192,202 }  // ETC
	},
	{ // 강 9
		{ 203,204,205 },  // 개방 0
		{ 206,207,208 },  // 비궁 1
		{ 206,207,208 },  // 소림 2	
		{ 209,210,211 },  // 녹림 3	
		{ 212,213,214 },  // 무당 4	
		{ 215,216,217 },  // 마교 5	
		{ 190,190,197 }   // ETC  6
	},
};

#ifdef _XTS_ITEM_OPTIMIZATION
int _GetWeponCategory( int itemid, int itemidCategory )
{
	if( itemid == 0 )
	{					
		return 3; // 권각
	}
	else
	{
		if( _XGI_FC_WEAPON == GetEqualItemType(itemidCategory) )
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(itemidCategory);
			if( WeaponItem )
			{
				switch( WeaponItem[itemid].cSecondType )
				{
				case _XGI_SC_WEAPON_SHORT				:				// 단병기
					if( WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_FIST )
					{
						return 3; // 권각
					}
					else if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_SWORD)
					{
						return 0; // 검
					}
					else if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_KNIFE)
					{
						return 1; // 도
					}
					else if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_SECRET)
					{
						return 4; // 단검
					}
					else if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_CLUB || 
						WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_HIT)
					{
						return 2; // 단봉
					}
					else if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_AXE )
					{
						return 5; // 부
					}
					else if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_RING )
					{
						return 6; // 쌍환 6
					}
					else
					{
						return 3; // 권각
					}
					break;
				case _XGI_SC_WEAPON_LONG				:				// 장병기			
					if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_GLAIVE )
					{
						return 1;// 대도
					}
					else if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_SPEAR )
					{
						return 7; // 창 7
					}
					else if(WeaponItem[itemid].cThirdType == _XGI_TC_WEAPON_STAFF )
					{
						return 8; // 곤 8
					}
					else
					{
						return 7; 
					}
					break;
				case _XGI_SC_WEAPON_SOFT				:				// 연병기						
					return 3; // 권각
					break;
				case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
					return 3; // 권각
					break;
				case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
					return 3; // 권각
					break;
				case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
					return 9; // 강병기
					break;
				}
			}	
		}		
	}

	return 3;
}
#else
int _GetWeponCategory( int itemid, int itemidCategory )
{
	if( itemid == 0 )
	{					
		return 3; // 권각
	}
	else
	{
		if( itemidCategory == _XGI_FC_WEAPON || itemidCategory == _XGI_FC_WEAPON2 
			|| itemidCategory == _XGI_FC_WEAPON3 )
		{
			_XGI_WeaponItem_Property* pWeaponItemProperty = NULL;
			
			if( itemidCategory == _XGI_FC_WEAPON )
				pWeaponItemProperty = g_WeaponItemProperty;
			else if( itemidCategory == _XGI_FC_WEAPON2 )
				pWeaponItemProperty = g_WeaponItemProperty2;
			else if( itemidCategory == _XGI_FC_WEAPON3 )
				pWeaponItemProperty = g_WeaponItemProperty3;
			
			switch( pWeaponItemProperty[itemid].cSecondType )
			{
			case _XGI_SC_WEAPON_SHORT				:				// 단병기
				if( pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_FIST )
				{
					return 3; // 권각
				}
				else if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_SWORD)
				{
					return 0; // 검
				}
				else if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_KNIFE)
				{
					return 1; // 도
				}
				else if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_SECRET)
				{
					return 4; // 단검
				}
				else if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_CLUB || 
					    pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_HIT)
				{
					return 2; // 단봉
				}
				else if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_AXE )
				{
					return 5; // 부
				}
				else if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_RING )
				{
					return 6; // 쌍환 6
				}
				else
				{
					return 3; // 권각
				}
				break;
			case _XGI_SC_WEAPON_LONG				:				// 장병기			
				if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_GLAIVE )
				{
					return 1;// 대도
				}
				else if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_SPEAR )
				{
					return 7; // 창 7
				}
				else if(pWeaponItemProperty[itemid].cThirdType == _XGI_TC_WEAPON_STAFF )
				{
					return 8; // 곤 8
				}
				else
				{
					return 7; 
				}
				break;
			case _XGI_SC_WEAPON_SOFT				:				// 연병기						
				return 3; // 권각
				break;
			case _XGI_SC_WEAPON_HIDDEN		:						// 암기						
				return 3; // 권각
				break;
			case _XGI_SC_WEAPON_MUSICAL		:						// 악병기
				return 3; // 권각
				break;
			case _XGI_SC_WEAPON_SPECIAL		:						// 특수무기
				return 9; // 강병기
				break;
			}
		}		
	}

	return 3;
}
#endif

#endif

//////////////////////////////////////////////////////////////////////
// Local definition
#define _XDEF_SELECTCHARACTERBGM				"Data\\Sound\\Bgm\\ruin.mp3"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#ifdef _XDWDEBUG
int		_CameraOffsetX;
int		_CameraOffsetY;
POINT	_CameraDragPos;
#endif

XProc_SelectCharacter::XProc_SelectCharacter()
{	
	setProcessInfo( 0, "Select character", __FILE__ );
	m_PrevProcess_Login = TRUE;
	memset( m_NewCreatedCharacterName, 0, sizeof(TCHAR) * 13 );
}

XProc_SelectCharacter::~XProc_SelectCharacter()
{

}

BOOL XProc_SelectCharacter::InitializeProcess(void)
{
#ifdef _XDWDEBUG 
	g_NormalGameExit = TRUE;
#else 
	g_NormalGameExit = FALSE;
#endif

#ifdef _XDEF_NEWANIMATIONLIST
	g_SelectAnimationFrameTime = 0;
#endif
	
	_XSetDefaultPath();
	
	((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.m_MovingGameServer = FALSE; // 이플래그를 켜서 캐릭터 리스트를 받아옴.
	m_SelectedCharacter				=	-1;
	m_LockCharacterSelection		=   FALSE;
	m_CurrentCharacterLevelIndex	=	0;
	m_CurrentCharacterSubLevel		=	0;

	// Build Texture =============================================================
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
	m_TextureArchive.GetResourceIndex("MI_Login_02.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive.GetResourceIndex("MI_Login_03.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		m_TextureArchive.GetResourceIndex("MI_keyboard.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);	
	}

	m_TextureArchive.BuildTextureList( TRUE );

	m_TextureArchive2.DisposeTexture();

	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
		
#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("\\Data\\Texture\\tsTR_MAININTERFACE.XP") );
#else
	strcat( packagefilename, _T("\\Data\\Texture\\TR_MAININTERFACE.XP") );
#endif

	m_TextureArchive2.SetTexturePackage( packagefilename );

	m_TextureArchive2.GetResourceIndex("MI_System.tga", _XDEF_DEFAULTTEXTUREMIPLEVEL, TRUE);
	m_TextureArchive2.BuildTextureList( TRUE );

	// window create ===================================================================

	FLOAT xfactor = (FLOAT)(gnWidth)  / 1024.0f;
	FLOAT yfactor = (FLOAT)gnHeight / 768.0f;

	int halfsize = (196>>1);
	_XWindow_SelectCharacter* pSelectCharacter_Window = new _XWindow_SelectCharacter;
	_XWINDOW_STRUCTURE selectcharacterstructure = 
	{
		TRUE, {(int)((80.0f+halfsize)*xfactor) - halfsize, (int)(82*yfactor)}, {196, 502}, 0,
			-1,
			&m_TextureArchive,
			D3DCOLOR_ARGB(128, 0, 0, 0),
			_XDEF_WTITLE_SELECTCHARACTER
	};
	pSelectCharacter_Window->Create(selectcharacterstructure);
	pSelectCharacter_Window->Initialize(m_TextureArchive);	
	pSelectCharacter_Window->EnableWindowEffect(FALSE);
	pSelectCharacter_Window->SetDocking(FALSE);	
	pSelectCharacter_Window->ShowWindow( TRUE );

	pSelectCharacter_Window->m_DrawCharacterInfo = (g_pLocalUser->m_CharacterCount > 0);
	
	m_WindowManager.InsertWindow(pSelectCharacter_Window);
	
#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
	// 이름 변경 윈도우 
	_XWindow_ChangeName* pChangeName_Window = new _XWindow_ChangeName;
	_XWINDOW_STRUCTURE changenamewindowstructure = 
	{
		TRUE, {473 , 387}, {183, 84}, 0,
			-1,
			&m_TextureArchive,
			D3DCOLOR_ARGB(255, 255, 255, 255),
			_XDEF_WTITLE_CHANGENAMEWINDOW
	};
	pChangeName_Window->Create(changenamewindowstructure);
	pChangeName_Window->Initialize(m_TextureArchive, m_TextureArchive2);	
	pChangeName_Window->EnableWindowEffect(FALSE);
	pChangeName_Window->SetDocking(FALSE);
	pChangeName_Window->ShowWindow(FALSE);
	m_WindowManager.InsertWindow(pChangeName_Window);
#endif
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
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
		pDrawKeyBoard_Window->ShowWindow(FALSE);
		pDrawKeyBoard_Window->m_LoginProcessMode = FALSE;
		
		m_WindowManager.InsertWindow(pDrawKeyBoard_Window);
	}
	
	m_BackgroundStatic.Create( 0, 0, gnWidth, gnHeight, NULL, -1 );
	m_BackgroundStatic.m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);

	InitDeviceObject();	

			
#ifdef _XDWDEBUG
	
	static BOOL	characterautoselectonceflag = TRUE;

	if( characterautoselectonceflag )
	{
		if( g_pLocalUser->m_CharacterCount >= 1 )
		{
			if(_XCmdLineOption("xautologin",0) != 0)
			{				
				characterautoselectonceflag = FALSE;

				if( g_pLocalUser->m_CharacterCount == 3 )
					this->m_SelectedCharacter = 2;
				else
					this->m_SelectedCharacter = 0;
					
				g_pLocalUser->SelectCharacter( this->m_SelectedCharacter );

				PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CONNECTBUTTON, 0), (LPARAM)gHWnd);
			}
		}
	}

	int lastselectedcharacter = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDCHARACTER, -1, TRUE );
	
	if( lastselectedcharacter > -1 )
	{
		if( lastselectedcharacter < g_pLocalUser->m_CharacterCount )
		{
			this->m_SelectedCharacter = lastselectedcharacter;
		}
		else
		{
			this->m_SelectedCharacter = 0;			
		}
	}
	else
	{
		this->m_SelectedCharacter = -1;
	}
#else
	int lastselectedcharacter = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDCHARACTER, -1, TRUE );

	if( lastselectedcharacter > -1 )
	{
		if( lastselectedcharacter < g_pLocalUser->m_CharacterCount )
		{
			this->m_SelectedCharacter = lastselectedcharacter;
		}
		else
		{
			this->m_SelectedCharacter = 0;			
		}
	}
	else
	{
		this->m_SelectedCharacter = -1;
	}
#endif	

	if( g_pLocalUser->m_CharacterCount-1 < this->m_SelectedCharacter )
	{
		this->m_SelectedCharacter = g_pLocalUser->m_CharacterCount-1;		
	}

	if( this->m_SelectedCharacter == -1 )
	{
		if( g_pLocalUser->m_CharacterCount > 0 )
		{
			this->m_SelectedCharacter = 0;			
		}		
	}

	if( this->m_SelectedCharacter > -1 )
	{
		_XLocalUser::GetUserLevel( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].innerlevel, m_CurrentCharacterLevelIndex, m_CurrentCharacterSubLevel );
	}

	if( g_pLocalUser->m_CharacterCount <= 0 )
	{
		TCHAR messagestring[64];
		strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_FAILCREATE));

		if( strcmpi( g_MessageBox.m_Message, messagestring ) != 0 )
		{
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_SELECTCHARACTER_NOCHARACTER, TRUE );		// "생성되어있는 인물이 없습니다"
			g_MessageBox.ShowWindow( TRUE );
		}
	}
	else
	{
		TCHAR messagestring[64];
		strcpy(messagestring, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_FAILCREATE));

		if( strcmpi( g_MessageBox.m_Message, messagestring ) != 0 )
		{
			g_MessageBox.ShowWindow( FALSE );
		}
		else
		{
			g_MessageBox.ShowWindow( TRUE );
		}
	}

	if( strlen( m_NewCreatedCharacterName ) ) // 새로 생성된 캐릭터를 선택함.
	{
		if( g_pLocalUser->m_CharacterCount > 0 )
		{
			for( int i = 0; i < g_pLocalUser->m_CharacterCount; i++ )
			{
				// 마지막으로 생성된 이름이 같으면 선택 
				if( strcmpi( m_NewCreatedCharacterName, g_pLocalUser->m_AllCharacterInfo[i].charactername ) == 0 ) 
				{
					g_pLocalUser->SelectCharacter( i );
					this->m_SelectedCharacter = i;
				}
			}
		}		
	}
	else
	{
		if( this->m_SelectedCharacter > -1 )
		{
			g_pLocalUser->SelectCharacter( this->m_SelectedCharacter );
		}
	}
	
	g_pLocalUser->SetAnimationIndex( 0 );
	g_pLocalUser->m_ModelDescriptor.PlayAnimation( TRUE );	
	g_pLocalUser->m_Position = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	D3DXMatrixIdentity( &g_pLocalUser->m_ModelDescriptor.m_Position );
	g_LodTerrain.m_3PCamera.SetTargetPosition( D3DXVECTOR3( 0.11f, 0.35f, 0.0f ) );

	for( int tz = 31; tz <= 33; tz++  )
	{
		for( int tx = 31; tx <= 33; tx++  )
		{
			for( int vtx = 0; vtx < TILE_VERTS*TILE_VERTS; vtx++ )
				g_LodTerrain.m_TerrainTile[tz][tx].VertexBuffer[vtx].y = 0.0f;
		}
	}

	_XUSER_MODELDESCRIPTTYPE modeldescript;
	
	memset( &modeldescript, 0, sizeof( _XUSER_MODELDESCRIPTTYPE ) );
	for(int i = 0; i < g_pLocalUser->m_CharacterCount; i++ )
	{		
		modeldescript = g_pLocalUser->m_AllCharacterInfo[i].modeldescript;
		
		m_PreviewCharacter[i].ReserveModelStack( _XDEF_MAXMODEL_COMBINATIONCOUNT );
		
		if( g_pLocalUser->m_AllCharacterInfo[i].gender == _XGENDER_MAN )
		{
			m_PreviewCharacter[i].LinkBipedObject( &g_MaleBipedObject );
		}
		else
		{
			m_PreviewCharacter[i].LinkBipedObject( &g_FemaleBipedObject );
		}

		m_PreviewCharacter[i].SetMeshModel( modeldescript, g_pLocalUser->m_AllCharacterInfo[i].gender, g_pLocalUser->m_AllCharacterInfo[i].groupindex );		

#ifdef _XDEF_NEWANIMATIONLIST
		int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1, 
											  g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1Category );

		m_PreviewCharacter[i].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory] );		
#else
		m_PreviewCharacter[i].SetCurMotion( 0 );
#endif

		m_PreviewCharacter[i].PlayAnimation( TRUE );
		m_PreviewCharacter[i].SetShadowDecalInfo( 0.65f, 0.85f, 1.85f );
		m_PreviewCharacter[i].InitEffectManager(1,0);
		
		m_PreviewCharacter[i].m_CameraDistance = 1.0f;
	}
	
	memset( m_String_None, 0, sizeof(TCHAR)*5 );
	strncpy(m_String_None, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR), 4);

	memset( m_String_LevelStar, 0, sizeof(TCHAR)*128 );
	strncpy(m_String_LevelStar, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL), 127);

	// Character Information Setting =======================================================
	SetCharacterInfoWindow();

	// Set user position ===========================================================================
	D3DXMatrixIdentity(&g_pLocalUser->m_ModelDescriptor.m_Position);

	D3DXMATRIX	rotmat,charrotmat;
	D3DXVECTOR3 position;
	//D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.82f, 0.45f);		
	D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.0f, 0.45f);		
	D3DXVECTOR3	rotcharacterpos;

	for( i = 0; i < g_pLocalUser->m_CharacterCount; i++ )
	{
		D3DXMatrixRotationY(&rotmat, _X_RAD(-120.0f * i));
		D3DXMatrixRotationY(&charrotmat, _X_RAD(180.0f - 120.0f * i));

		D3DXVec3TransformCoord( &rotcharacterpos, &characterpos, &rotmat );
		m_PreviewCharacter[i].m_Position = charrotmat;
		m_PreviewCharacter[i].m_Position._41 = rotcharacterpos.x;
		m_PreviewCharacter[i].m_Position._42 = rotcharacterpos.y;
		m_PreviewCharacter[i].m_Position._43 = rotcharacterpos.z;
#ifdef _XDEF_NEWANIMATIONLIST
		int effectid = _XGameItem::GetInchantEffectId( g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1Category,
			g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1, g_pLocalUser->m_AllCharacterInfo[i].weaponStrength );
		if( effectid > 0 )
		{
			m_PreviewCharacter[i].m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
				effectid, &m_PreviewCharacter[i].m_Position, NULL, TRUE );
			
			//_XLog( "effect id : %d : %d, %d %d %d", i, effectid, g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1Category,
			//		g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1, g_pLocalUser->m_AllCharacterInfo[i].weaponStrength );
		}
#endif
		//D3DXMatrixTranslation( &m_PreviewCharacter[i].m_Position, rotcharacterpos.x-0.34f, rotcharacterpos.y, rotcharacterpos.z );
	}	

	m_CurrentRotateAngle		= 0.0f;
	m_TargetRotateAngle			= 0.0f;
	m_CharacterLocalRotateAngle[0] = m_CharacterLocalRotateAngle[1] = m_CharacterLocalRotateAngle[2] = 0.0f;

	_XSetDefaultPath( _XDEF_DEFAULTPATH_TEXTURE );

#ifdef _XTESTSERVER			
	if( !g_TerrainTextureArchive.m_RenderStyleManager.Load( _T("tsEnvironment.xrs") ) ) return false;	
#else
	if( !g_TerrainTextureArchive.m_RenderStyleManager.Load( _T("Environment.xrs") ) ) return false;	
#endif

	if( !CreateOffScreen() )
	{
		return FALSE;
	}
	
	if( !g_pTitleFlashObject )
	{
		g_pTitleFlashObject = GetFlashControl();
		
		if( !g_pTitleFlashObject ) return FALSE;
		
		g_pTitleFlashObject->initialize( gHWnd,gHInst,g_pTitleFlashOffScreenTexture);
	}
		
	HDC pDC = GetDC( gHWnd );
	g_pTitleFlashObject->SetDC( pDC );
	
	TCHAR flashfilename[64];

	if( m_PrevProcess_Login )
	{
		memset( flashfilename, 0, sizeof(TCHAR) * 64 );
		
		if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
		{			
			strcpy( flashfilename, _T("TITLE02.swf") );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			strcpy( flashfilename, _T("vn_TITLE02.swf") );
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		{
			strcpy( flashfilename, _T("tw_TITLE02.swf") );
		}
		else
		{
			strcpy( flashfilename, _T("TITLE02.swf") );
		}
	}
	else
	{
		memset( flashfilename, 0, sizeof(TCHAR) * 64 );

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


	g_LodTerrain.m_3PCamera.SetDistance( 10.0f );	
	g_LodTerrain.m_3PCamera.mp_fPitch = 72.0f;
	g_LodTerrain.m_3PCamera.UpdateViewMatrix();

	g_EnvironmentManager.m_ObjectLight.Position = D3DXVECTOR3( 100.0f, 40.0f, 100.0f );

	m_FirstRotateAnimation = TRUE;
	m_FirstRotateAngle = 0.0f;

	m_CharacterDeleteMode = FALSE;
	m_CharacterDeleteModeTimer = 0;
	m_PrevSelectedCharacterPosition = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	m_CharacterSelectedMode = FALSE;
	m_CharacterSelectedModeTimer = 0;

	int overlayimageindex = m_TextureArchive.FindResource("MI_login_02.tga");

	m_BottomOverlayLeftImage.Create( 100, 680-100, 356, 680+28, &m_TextureArchive, overlayimageindex );
	m_BottomOverlayLeftImage.SetClipRect( 0, 0, 256, 128 );		
	m_BottomOverlayRightImage.Create( 356, 680-100, 356+256, 680+28, &m_TextureArchive, overlayimageindex );
	m_BottomOverlayRightImage.SetClipRect( 0, 128, 256, 256 );
	
#ifdef _XDWDEBUG
	_CameraOffsetX = _CameraOffsetY = 0;
	_CameraDragPos.x = _CameraDragPos.y = 0;
#endif

	// Initialize live packet timer
	m_LivePacketTimer = 0;

	// 초기화가 끝나면 프로세스 활성화..
	SetEnableProcess( TRUE );

#ifdef _XDEF_TESTRENDER
	D3DXMatrixIdentity( &g_TestRenderModelDescriptor.m_Position );
	g_TestRenderModelDescriptor.m_Position._41 = -0.03f;	
	g_TestRenderModelDescriptor.m_Position._42 = -1.25f;
	g_TestRenderModelDescriptor.m_Position._43 = 1.1f;
	D3DXMATRIX _rotmat;
	D3DXMatrixRotationY( &_rotmat, _X_RAD(-180.0f) );
	D3DXMatrixMultiply( &g_TestRenderModelDescriptor.m_Position, &g_TestRenderModelDescriptor.m_Position, &_rotmat );
#endif		

	return TRUE;
}

BOOL XProc_SelectCharacter::CreateOffScreen( void )
{
	if( g_pTitleFlashOffScreenTexture ) return TRUE;
	
	SIZE size = m_BackgroundStatic.GetWindowSize();
	
	if( FAILED( gpDev->CreateTexture( size.cx, size.cy, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &g_pTitleFlashOffScreenTexture, NULL ) ) )
    {
        return FALSE;
    }
	
	return TRUE;
}

void XProc_SelectCharacter::SetCharacterInfoWindow( void )
{
	_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_SELECTCHARACTER);

	if( !pSelectCharacter_Window )
	{
		return;
	}

	if(this->m_SelectedCharacter >= 0)
	{
		switch(this->m_SelectedCharacter)
		{
		case 0 :
			{
				pSelectCharacter_Window->m_SelectedCharacter = m_SelectedCharacter;

				pSelectCharacter_Window->m_Blank01Button.MoveWindow( gnWidth - (1024 - 857), gnHeight - (768 - 457) );
				pSelectCharacter_Window->m_Blank02Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 497) );
				pSelectCharacter_Window->m_Blank03Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 538) );

#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
				if( g_pLocalUser->m_AllCharacterInfo[0].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFFFFFF );
				if( g_pLocalUser->m_AllCharacterInfo[1].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
				if( g_pLocalUser->m_AllCharacterInfo[2].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );

#else
				pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFFFFFF );
				pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
				pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );
#endif

				pSelectCharacter_Window->RebuildCharacterGroupNameStatic( g_pLocalUser->m_CharacterInfo.groupindex );

				strcpy(pSelectCharacter_Window->m_Name, g_pLocalUser->m_CharacterInfo.charactername);
				TCHAR levelname[128];

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
				{
					strcpy(pSelectCharacter_Window->m_Level, g_LevelName[ m_CurrentCharacterLevelIndex ] );
					sprintf(pSelectCharacter_Window->m_SubLevel, "%d %s",  m_CurrentCharacterSubLevel, m_String_LevelStar );
				}
				else
				{
					sprintf(levelname, "%s %d %s",  g_LevelName[ m_CurrentCharacterLevelIndex ],m_CurrentCharacterSubLevel, m_String_LevelStar );				
					strcpy(pSelectCharacter_Window->m_Level, levelname);
				}

				strcpy(pSelectCharacter_Window->m_CharacterLocation, g_ZoneInfoTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid-1].zoneshortname );
				
				if( strlen( g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
													[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
													[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] ) > 0 )
				{
					strcpy(pSelectCharacter_Window->m_RollClass, 
						g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
												[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
												[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] );
				}
				else
				{
					strcpy(pSelectCharacter_Window->m_RollClass, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_MU) );
				}

				pSelectCharacter_Window->m_constitution	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution;
				pSelectCharacter_Window->m_zen			= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen;
				pSelectCharacter_Window->m_intelligence	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence;
				pSelectCharacter_Window->m_dexterity	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity;
				pSelectCharacter_Window->m_strength		= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength;
				pSelectCharacter_Window->m_DrawCharacterInfo = TRUE;
				pSelectCharacter_Window->m_CharacterGMLevel = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].Get_gm_level();
			}
			break;
			
		case 1 :
			{
				pSelectCharacter_Window->m_SelectedCharacter = m_SelectedCharacter;

				pSelectCharacter_Window->m_Blank01Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 457) );
				pSelectCharacter_Window->m_Blank02Button.MoveWindow( gnWidth - (1024 - 857), gnHeight - (768 - 497) );
				pSelectCharacter_Window->m_Blank03Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 538) );

#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
				if( g_pLocalUser->m_AllCharacterInfo[0].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
				if( g_pLocalUser->m_AllCharacterInfo[1].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFFFFFF );
				if( g_pLocalUser->m_AllCharacterInfo[2].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );

#else
				pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
				pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFFFFFF );
				pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );
#endif

				pSelectCharacter_Window->RebuildCharacterGroupNameStatic( g_pLocalUser->m_CharacterInfo.groupindex );

				strcpy(pSelectCharacter_Window->m_Name, g_pLocalUser->m_CharacterInfo.charactername);				
				TCHAR levelname[128];

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
				{
					strcpy(pSelectCharacter_Window->m_Level, g_LevelName[ m_CurrentCharacterLevelIndex ] );
					sprintf(pSelectCharacter_Window->m_SubLevel, "%d %s",  m_CurrentCharacterSubLevel, m_String_LevelStar );
				}
				else
				{
					sprintf(levelname, "%s %d %s",  g_LevelName[ m_CurrentCharacterLevelIndex ],m_CurrentCharacterSubLevel, m_String_LevelStar );
					strcpy(pSelectCharacter_Window->m_Level, levelname);
				}

				strcpy(pSelectCharacter_Window->m_CharacterLocation, g_ZoneInfoTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid-1].zoneshortname );
				
				if( strlen( g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
													[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
													[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] ) > 0 )
				{
					strcpy(pSelectCharacter_Window->m_RollClass, 
						g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
												[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
												[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] );
				}
				else
				{
					strcpy(pSelectCharacter_Window->m_RollClass, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_MU) );
				}

				pSelectCharacter_Window->m_constitution	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution;
				pSelectCharacter_Window->m_zen			= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen;
				pSelectCharacter_Window->m_intelligence	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence;
				pSelectCharacter_Window->m_dexterity	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity;
				pSelectCharacter_Window->m_strength		= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength;
				pSelectCharacter_Window->m_DrawCharacterInfo = TRUE;
				pSelectCharacter_Window->m_CharacterGMLevel = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].Get_gm_level();
			}
			break;
			
		case 2 :
			{
				pSelectCharacter_Window->m_SelectedCharacter = m_SelectedCharacter;

				pSelectCharacter_Window->m_Blank01Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 457) );
				pSelectCharacter_Window->m_Blank02Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 497) );
				pSelectCharacter_Window->m_Blank03Button.MoveWindow( gnWidth - (1024 - 857), gnHeight - (768 - 538) );

#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
				if( g_pLocalUser->m_AllCharacterInfo[0].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
				if( g_pLocalUser->m_AllCharacterInfo[1].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
				if( g_pLocalUser->m_AllCharacterInfo[2].ucChangeNameFlag )
					pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
				else
					pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFFFFFF );

#else
				pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
				pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
				pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFFFFFF );
#endif

				pSelectCharacter_Window->RebuildCharacterGroupNameStatic( g_pLocalUser->m_CharacterInfo.groupindex );

				strcpy(pSelectCharacter_Window->m_Name, g_pLocalUser->m_CharacterInfo.charactername);				
				TCHAR levelname[128];
				
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
				{
					strcpy(pSelectCharacter_Window->m_Level, g_LevelName[ m_CurrentCharacterLevelIndex ] );
					sprintf(pSelectCharacter_Window->m_SubLevel, "%d %s",  m_CurrentCharacterSubLevel, m_String_LevelStar );
				}
				else
				{
					sprintf(levelname, "%s %d %s",  g_LevelName[ m_CurrentCharacterLevelIndex ],m_CurrentCharacterSubLevel, m_String_LevelStar );
					strcpy(pSelectCharacter_Window->m_Level, levelname);					
				}

				strcpy(pSelectCharacter_Window->m_CharacterLocation, g_ZoneInfoTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid-1].zoneshortname );
				
				if( strlen( g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
													[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
													[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] ) > 0 )
				{
					strcpy(pSelectCharacter_Window->m_RollClass, 
						g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
												[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
												[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] );
				}
				else
				{
					strcpy(pSelectCharacter_Window->m_RollClass, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_MU) );
				}

				pSelectCharacter_Window->m_constitution	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution;
				pSelectCharacter_Window->m_zen			= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen;
				pSelectCharacter_Window->m_intelligence	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence;
				pSelectCharacter_Window->m_dexterity	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity;
				pSelectCharacter_Window->m_strength		= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength;
				pSelectCharacter_Window->m_DrawCharacterInfo = TRUE;
				pSelectCharacter_Window->m_CharacterGMLevel = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].Get_gm_level();
			}
			break;
		default :
			{
				pSelectCharacter_Window->m_DrawCharacterInfo = FALSE;
				pSelectCharacter_Window->m_CharacterGMLevel = 0;
			}
			break;
		}
	}
	else
		pSelectCharacter_Window->m_DrawCharacterInfo = FALSE;
}

void XProc_SelectCharacter::DestroyProcess(void)
{
	m_PreviewCharacter[0].Reset();
	m_PreviewCharacter[1].Reset();
	m_PreviewCharacter[2].Reset();

	m_TextureArchive2.DisposeTexture();
	m_TextureArchive.DisposeTexture();
	m_WindowManager.DestroyManager();
	
	g_EnvironmentManager.m_ObjectLight.Position = D3DXVECTOR3( 100.0f, 40.0f, -100.0f );

//	Author : 양희왕 		
#ifdef _XDEF_NOTICE_STRING_PROCESS
	_XNoticeProcess*	pNoticeString = _XNoticeProcess::GetInstance();
	pNoticeString->Destroy();
#endif
//	Last updated: 07/01/04	

	SetEnableProcess( FALSE );
}

bool XProc_SelectCharacter::InitDeviceObject(void)
{
	m_TextureArchive.InitDeviceObject();
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
	
		
	D3DUtil_InitLight( g_EnvironmentManager.m_CharacterLight, D3DLIGHT_DIRECTIONAL, 100.0f, 40.0f, -100.0f );
	D3DXVECTOR3 ldnv = D3DXVECTOR3( 0.650945f, -0.390567f, -0.650945f );
	D3DXVec3Normalize( &ldnv, &ldnv );
	g_EnvironmentManager.m_CharacterLight.Direction = ldnv;	

	if( !_XAnimMeshAbstractClass::m_UseVertexShaderAnimation )
	{
		g_EnvironmentManager.m_CharacterLight.Diffuse.r = 0.68F;
		g_EnvironmentManager.m_CharacterLight.Diffuse.g = 0.68F;
		g_EnvironmentManager.m_CharacterLight.Diffuse.b = 0.68F;

		g_EnvironmentManager.m_CharacterLight.Ambient.r = 0.43f;
		g_EnvironmentManager.m_CharacterLight.Ambient.g = 0.43f;
		g_EnvironmentManager.m_CharacterLight.Ambient.b = 0.43f;
	}
	else
	{
		g_EnvironmentManager.m_CharacterLight.Diffuse.r = 0.68F;
		g_EnvironmentManager.m_CharacterLight.Diffuse.g = 0.68F;
		g_EnvironmentManager.m_CharacterLight.Diffuse.b = 0.68F;

		g_EnvironmentManager.m_CharacterLight.Ambient.r = 1.0f;
		g_EnvironmentManager.m_CharacterLight.Ambient.g = 1.0f;
		g_EnvironmentManager.m_CharacterLight.Ambient.b = 1.0f;
	}


	g_EnvironmentManager.m_CharacterAmbientColor = D3DCOLOR_ARGB( 255, 79, 79, 81 );

	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
	gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight );
	gpDev->SetRenderState( D3DRS_LIGHTING, TRUE );
	gpDev->LightEnable( 0, TRUE );

	m_SelectedCharacterLight = g_EnvironmentManager.m_CharacterLight;
	m_SelectedCharacterLight.Diffuse.r = 0.76f;
	m_SelectedCharacterLight.Diffuse.g = 0.76f;
	m_SelectedCharacterLight.Diffuse.b = 0.76f;

	CreateOffScreen();
	if(g_pTitleFlashObject)
	{
		g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );
	}

	_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_SELECTCHARACTER);

	if( pSelectCharacter_Window )
	{
		for( int i = 0; i < g_pLocalUser->m_CharacterCount; i++ )
		{
			pSelectCharacter_Window->LoadMinimapTexture( i, g_ZoneInfoTable[g_pLocalUser->m_AllCharacterInfo[i].zoneuniqid-1].zonefilename );
		}
	}

	m_CharacterOBB.SetCenter( D3DXVECTOR3(0.0f, 0.65, 0.0f ) );
	m_CharacterOBB.SetExtents( D3DXVECTOR3(0.12f, 0.65f, 0.12f ) );
	D3DXVECTOR3 Axis[3] = 
	{
		D3DXVECTOR3( 1.0f, 0.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f ),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f )
	};
	m_CharacterOBB.SetAxis( Axis );
	m_CharacterOBB.RecalcRadius();

#ifdef _XDWDEBUG
	m_CharacterOBB.CreateOBBModel();
#endif

	return true;
}

void XProc_SelectCharacter::ReleaseDeviceObject( void )
{		
	_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_SELECTCHARACTER);
	
	if( pSelectCharacter_Window )
	{
		for( int i = 0; i < 3; i++ )
		{
			SAFE_RELEASE( pSelectCharacter_Window->m_pMinimapTexture[i] );
		}
	}
}

void XProc_SelectCharacter::PauseProcess( void )
{

}

void XProc_SelectCharacter::UnPauseProcess( void )
{
	
}

void XProc_SelectCharacter::Draw(void)
{
	// Begin the scene
	gpDev->BeginScene();

	gpDev->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);

	gpDev->SetTransform(D3DTS_VIEW, &g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, &g_LodTerrain.m_3PCamera.GetProjMatrix());

	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
	gpDev->SetRenderState( D3DRS_LIGHTING, FALSE );	
	g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );

#ifdef _XDEF_TESTRENDER		
#else
	m_BackgroundStatic.DrawWithRegion( g_pTitleFlashOffScreenTexture );
#endif

	if( !g_pTitleFlashObject->IsPlaying() )
	{
		// Draw Selected Character	
		gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_CharacterAmbientColor );
		gpDev->SetRenderState( D3DRS_LIGHTING, TRUE );	

#ifdef _XDEF_TESTRENDER		
	bool ss;
	g_TestRenderModelDescriptor.Advance(ss);
	g_TestRenderModelDescriptor.Animation();
	g_TestRenderModelDescriptor.Render();
#endif		
		
		bool soundplay = true;
		for( int i = 0; i < g_pLocalUser->m_CharacterCount; i++ )
		{
			if( i == m_SelectedCharacter )
			{
				gpDev->SetLight( 0, &m_SelectedCharacterLight );
			}
			else
			{
				gpDev->SetLight( 0, &g_EnvironmentManager.m_CharacterLight);
			}

			if( m_CharacterDeleteMode || m_CharacterSelectedMode )
			{
				if( i == m_SelectedCharacter )
				{
					m_PreviewCharacter[i].Advance(soundplay);					
					m_PreviewCharacter[i].Animation();
#ifdef _XDEF_TESTRENDER
#else
					m_PreviewCharacter[i].RenderShadow();
					m_PreviewCharacter[i].RenderCharacter( NULL );

					if( m_PreviewCharacter[i].m_EffectManager )
					{
						m_PreviewCharacter[i].m_EffectManager->Update(&m_PreviewCharacter[i], NULL, NULL, NULL, NULL );
						m_PreviewCharacter[i].m_EffectManager->Render();
					}
#endif
				}
			}
			else
			{
				m_PreviewCharacter[i].Advance(soundplay);
				m_PreviewCharacter[i].Animation();
#ifdef _XDEF_TESTRENDER
#else
				m_PreviewCharacter[i].RenderShadow();
				m_PreviewCharacter[i].RenderCharacter( g_pLocalUser->m_AllCharacterInfo[i] );
#endif
				
				/*if( m_PreviewCharacter[i].m_EffectManager )
				{
					m_PreviewCharacter[i].m_EffectManager->Update(&m_PreviewCharacter[i], NULL, NULL, NULL, NULL );
					m_PreviewCharacter[i].m_EffectManager->Render();
				}*/

				/*

				D3DXVECTOR3 pos;	
				pos.x = m_PreviewCharacter[i].m_Position._41;
				pos.y = m_PreviewCharacter[i].m_Position._42;
				pos.z = m_PreviewCharacter[i].m_Position._43;
				D3DXVECTOR3 pickedpoint;
				
				if( m_CharacterOBB.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, pos, pickedpoint ) )
				{
					m_CharacterOBB.m_OBBModel.Render( m_PreviewCharacter[i].m_Position, NULL );	
				}
				else
				{
					m_CharacterOBB.m_OBBModel.Render( m_PreviewCharacter[i].m_Position, NULL, D3DCOLOR_ARGB( 255, 255, 0, 0 ) );
				}
				*/

				if(m_PreviewCharacter[i].m_FrameResult & _XDEF_ACI_LASTFRAME)
				{
	
#ifdef _XDEF_NEWANIMATIONLIST
					int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1, g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1Category );
					
					int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[i].groupindex];

					if( m_PreviewCharacter[i].GetCurMotion() == g_PreviewCharacterMotionIndex[itemcategory][lutindex][0] )
					{
						m_PreviewCharacter[i].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][1], true );
					}
#else
					if( m_PreviewCharacter[i].GetCurMotion() == g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[i].groupindex]
																							 [g_pLocalUser->m_AllCharacterInfo[i].gender][0] )
																							 
					{
						m_PreviewCharacter[i].SetCurMotion( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[i].groupindex]
															[g_pLocalUser->m_AllCharacterInfo[i].gender][1], true );
					}
#endif
				}
			}
		}

		
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		gpDev->SetRenderState( D3DRS_LIGHTING, FALSE );

		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 0 );
		
		/*
		if( m_CharacterDeleteMode )
		{
			m_BottomOverlayLeftImage.Draw();
			m_BottomOverlayRightImage.Draw();
			_XDrawSolidBar( 100, 680+28, 612, 768, 0xFF000000 );
		}
		*/
#ifdef _XDEF_TESTRENDER	
		
		// End the scene
		gpDev->EndScene();
		
		// Present the backbuffer contents to the display	
		HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
		
		if( D3DERR_DEVICELOST == hr )
			gApp->m_bDeviceLost = true;

		return;
#endif

		m_WindowManager.Draw();
		
		g_MessageBox.Draw();

		g_IMEWindow.Draw();
		
		gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
		g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 4 );
		
		if( m_CharacterDeleteMode )
		{
			g_XBaseFont->SetColor( 0xFFFFFFFF );
			g_XBaseFont->Puts( 16, gnHeight-32, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_CHARACTERINFO) );	// "삭제할 인물의 정보를 확인하여 주십시오."
			g_XBaseFont->Puts( 17, gnHeight-32, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_CHARACTERINFO) );	// "삭제할 인물의 정보를 확인하여 주십시오."
			g_XBaseFont->Flush();
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFFFFFFF );

			if( g_pLocalUser->m_CharacterCount < 1 )
			{
				g_XBaseFont->Puts( 16, gnHeight-32, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHARACTER) );	// "생성되어 있는 인물이 없습니다. 인물을 생성하여 주십시오."
				g_XBaseFont->Puts( 17, gnHeight-32, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHARACTER) );	// "생성되어 있는 인물이 없습니다. 인물을 생성하여 주십시오."
			}
			else
			{
				// 인물을 선택하여 주십시오.
				g_XBaseFont->Puts( 16, gnHeight-32, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SELECTCHARACTER) );
				g_XBaseFont->Puts( 17, gnHeight-32, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SELECTCHARACTER) );				
			}

			g_XBaseFont->Flush();
		}
		
		static int  twinklecounter = 0;
		static BOOL	twinklemode = FALSE;
		static FLOAT  alphalevel;
		if( twinklecounter == 0 )
		{
			twinklecounter = g_LocalSystemTime;
		}

		int elapsedtime = g_LocalSystemTime - twinklecounter;
		twinklecounter = g_LocalSystemTime;

		if( twinklemode )
		{
			alphalevel += ((elapsedtime/1000.0f) * 20.0f);
			if( alphalevel > 255.0f ) 
			{
				alphalevel = 255.0f;
				twinklemode = !twinklemode;
			}
		}
		else
		{
			alphalevel -= ((elapsedtime/1000.0f) * 20.0f);
			if( alphalevel < 0 ) 
			{
				alphalevel = 0.0f;
				twinklemode = !twinklemode;
			}
		}

		if( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution == 8 &&
			g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen == 8 &&
			g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence == 8 &&
			g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity == 8 &&
			g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength == 8 )
		{
			g_XLargeFont->SetColor( D3DCOLOR_ARGB( (int)alphalevel, 255, 170, 48 ) );
			g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
			g_XLargeFont->PutsAlign( (gnWidth>>1), 640, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2449) );
			g_XLargeFont->PutsAlign( (gnWidth>>1), 640+30, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAME_2403) );
			g_XLargeFont->PutsAlign( (gnWidth>>1), 640+60, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2450) );
			g_XLargeFont->Flush();
		}
		else
		{
			if( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].Get_gm_level() <= -1 )
			{
				g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
				g_XBaseFont->SetColor( 0xFFFF0F0F );

				g_XBaseFont->PutsAlign( (gnWidth>>1), 580, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2609) );
				g_XBaseFont->PutsAlign( (gnWidth>>1), 600, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2610) );
								
				time_t _blockingtime = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].blockingenddate;
				struct tm *today;
				char tmpbuf[256];
				today = localtime( &_blockingtime );
				
				strftime( tmpbuf, 128, _XGETINTERFACETEXT(ID_STRING_NEW_2776), today );	// "접속 제한 종료 일자 : %Y년 %m월 %d일 %H:%M:%S"

				g_XBaseFont->PrintAlign( (gnWidth>>1), 623, 1.0f, _XFONT_ALIGNTYPE_CENTER, tmpbuf );
				
				g_XBaseFont->Flush();
			}
/*
			if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
			{
				g_ViewLocalUserBillingInfo = FALSE;				
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			{
				g_ViewLocalUserBillingInfo = FALSE;				
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
			{
				g_ViewLocalUserBillingInfo = FALSE;				
			}
			else
			{
				g_ViewLocalUserBillingInfo = FALSE;
			}

			if( g_ViewLocalUserBillingInfo )
			{
				g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
				g_XBaseFont->SetColor( 0xFFFFFFFF );

				if( g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_Home )
				{
					// 개인무료,프리미엄 유저					

					if( g_LocalUserBillingInformations.BillingStatusWhere == _XB_BillingStatusWhere_Home )
					{
						if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Free )
						{
							// 무료이용/집
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2451) );
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670+16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2452) );
							
							int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
							int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

							g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
	   												 g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_SELECTCHARACTER_2453, &remainhour, &remainminute ) );
						}
						else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
						{
							// 개인정액/집
							g_XBaseFont->SetColor( 0xFFCCBB33 );
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2454) );

							if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
							{
								int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
								int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
								
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
														g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2434, &remainhour, &remainminute ) );
							}
							else
							{
								int param1 = g_LocalUserBillingInformations.BillExpireYear;
								int param2 = g_LocalUserBillingInformations.BillExpireMonth;
								int param3 = g_LocalUserBillingInformations.BillExpireDay;
								
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
														 g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2435, &param1, &param2, &param3 ) );
							}
						}
						else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
						{
							// 개인프리미엄/집							
							g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 36, 164, 255 ) );
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2455) );

							if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
							{
								int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
								int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
								

								g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
														g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2434, &remainhour, &remainminute ) );
							}
							else
							{
								int param1 = g_LocalUserBillingInformations.BillExpireYear;
								int param2 = g_LocalUserBillingInformations.BillExpireMonth;
								int param3 = g_LocalUserBillingInformations.BillExpireDay;
								
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
														g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2435, &param1, &param2, &param3 ) );
							}
						}
						else
						{
							// Error
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAME_2432) );
						}
					}
					else if( g_LocalUserBillingInformations.BillingStatusWhere == _XB_BillingStatusWhere_PCRoom )
					{
						// 정액제 이상의 유저들.
						if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
						{
							if( g_LocalUserBillingInformations.BillingETCData == 0  )
							{
								// 정액유저/체험피씨방
								g_XBaseFont->SetColor( 0xFFCCBB33 );
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2454) );

								if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
								{
									int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
									int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
									
									g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
															g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2434, &remainhour, &remainminute ) );
								}
								else
								{
									int param1 = g_LocalUserBillingInformations.BillExpireYear;
									int param2 = g_LocalUserBillingInformations.BillExpireMonth;
									int param3 = g_LocalUserBillingInformations.BillExpireDay;
									
									g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
															g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2435, &param1, &param2, &param3 ) );									
								}
							}
							else
							{
								// 정액유저/프리미엄피씨방
								g_XBaseFont->SetColor( 0xFFCCBB33 );
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2456) );
								
								if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
								{
									int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
									int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
									
									g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
															g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2434, &remainhour, &remainminute ) );
								}
								else
								{
									int param1 = g_LocalUserBillingInformations.BillExpireYear;
									int param2 = g_LocalUserBillingInformations.BillExpireMonth;
									int param3 = g_LocalUserBillingInformations.BillExpireDay;
									
									g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
															g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2435, &param1, &param2, &param3 ) );
								
								}

								int param1 = g_LocalUserBillingInformations.PCRoomBillExpireYear;
								int param2 = g_LocalUserBillingInformations.PCRoomBillExpireMonth;
								int param3 = g_LocalUserBillingInformations.PCRoomBillExpireDay;						
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670+54, _XFONT_ALIGNTYPE_CENTER, 
														 g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2436, &param1, &param2, &param3 ) );
							}
						}
						else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
						{
							if( g_LocalUserBillingInformations.BillingETCData == 0  )
							{
								// 프리미엄유저/체험피씨방
								g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 36, 164, 255 ) );
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2455) );
								
								if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
								{
									int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
									int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
									
									g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
															g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2434, &remainhour, &remainminute ) );
								}
								else
								{
									int param1 = g_LocalUserBillingInformations.BillExpireYear;
									int param2 = g_LocalUserBillingInformations.BillExpireMonth;
									int param3 = g_LocalUserBillingInformations.BillExpireDay;
									
									g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
															g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2435, &param1, &param2, &param3 ) );
								}
							}
							else
							{
								// 프리미엄유저/프리미엄피씨방
								g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 36, 164, 255 ) );
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2457) );

								if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
								{
									int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
									int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
									
									g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
															g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2434, &remainhour, &remainminute ) );
								}
								else
								{
									int param1 = g_LocalUserBillingInformations.BillExpireYear;
									int param2 = g_LocalUserBillingInformations.BillExpireMonth;
									int param3 = g_LocalUserBillingInformations.BillExpireDay;
									
									g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
															g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2435, &param1, &param2, &param3 ) );
								}

								int param1 = g_LocalUserBillingInformations.PCRoomBillExpireYear;
								int param2 = g_LocalUserBillingInformations.PCRoomBillExpireMonth;
								int param3 = g_LocalUserBillingInformations.PCRoomBillExpireDay;						
								g_XBaseFont->PutsAlign( (gnWidth>>1), 670+54, _XFONT_ALIGNTYPE_CENTER, 
														g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2436, &param1, &param2, &param3 ) );
							}
						}
						else
						{
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAME_2432) );						
						}					
					}
					else
					{
						g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MAINGAME_2432) );
					}
				}
				else if( g_LocalUserBillingInformations.BillingStatusWho == _XB_BillingStatusWho_PC )
				{
					if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Basic )
					{
						// 무료이용/정액제피씨방
						g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2458) );

						if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
						{
							int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
							int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;

							g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2439, &remainhour, &remainminute ) );
						}
						else
						{
							int param1 = g_LocalUserBillingInformations.PCRoomBillExpireYear;
							int param2 = g_LocalUserBillingInformations.PCRoomBillExpireMonth;
							int param3 = g_LocalUserBillingInformations.PCRoomBillExpireDay;						
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2436, &param1, &param2, &param3 ) );
							
						}
					}
					else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Fixed )
					{
						// 무료이용/정량제피씨방
						g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2458) );

						if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
						{
							int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
							int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
							
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2439, &remainhour, &remainminute ) );
						}
						else
						{
							int param1 = g_LocalUserBillingInformations.PCRoomBillExpireYear;
							int param2 = g_LocalUserBillingInformations.PCRoomBillExpireMonth;
							int param3 = g_LocalUserBillingInformations.PCRoomBillExpireDay;

							g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2436, &param1, &param2, &param3 ));
						}
					}
					else if( g_LocalUserBillingInformations.BillMethod == _XB_BillingMethod_Premium )
					{
						// 무료이용/프리미엄피씨방
						g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2458) );

						if( g_LocalUserBillingInformations.AlertCode == _PACKET_ALERT_REMAIN_TIME )
						{
							int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
							int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;
							
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2439, &remainhour, &remainminute ) );
						}
						else
						{
							int param1 = g_LocalUserBillingInformations.PCRoomBillExpireYear;
							int param2 = g_LocalUserBillingInformations.PCRoomBillExpireMonth;
							int param3 = g_LocalUserBillingInformations.PCRoomBillExpireDay;						
							g_XBaseFont->PutsAlign( (gnWidth>>1), 670+40, _XFONT_ALIGNTYPE_CENTER, 
													g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2436, &param1, &param2, &param3 ) );
						}
					}
					else 
					{
						// 무료이용/체험피씨방
						g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2459) );
						g_XBaseFont->PutsAlign( (gnWidth>>1), 670+16, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2452) );
						
						/*
						int remainhour = (g_LocalUserBillingInformations.BillRemain / 3600000);
						int remainminute = (g_LocalUserBillingInformations.BillRemain % 3600000) / 60000;					
						g_XBaseFont->PrintAlign( (gnWidth>>1), 670+40, 1.0f, _XFONT_ALIGNTYPE_CENTER, _T("오늘 남은 무료 시간 : %d시간 %d분"), remainhour, remainminute );
					
					}
				}
				else //	시간을 모두 소비한 무료이용 유저.
				{
					g_XBaseFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2460) );
				}

				g_XBaseFont->Flush();				
				g_XBaseFont->DisableGrowMode();
			}*/
			
			// 베트남 셧다운 
#ifdef _XDEF_SHUTDOWN_20061027			
			if( g_PlayLeftTime > 0 )
			{
				int remainhour = (g_PlayLeftTime / 3600000);
				int remainminute = (g_PlayLeftTime % 3600000) / 60000;	
				
				TCHAR szBuffer[256];
				memset( szBuffer, 0, sizeof(TCHAR)*256 );
				sprintf( szBuffer, _XGETINTERFACETEXT(ID_STRING_NEW_3147), remainhour, remainminute );
				
				g_XBaseFont->SetBoldMode(TRUE);
				g_XBaseFont->SetColor( 0xFFFFFFFF );
				g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3148) );
				g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+20, _XFONT_ALIGNTYPE_CENTER, szBuffer );
				g_XBaseFont->Flush();				
				g_XBaseFont->SetBoldMode(FALSE);
			}
			else
			{
				TCHAR szBuffer[256];
				memset( szBuffer, 0, sizeof(TCHAR)*256 );
				sprintf( szBuffer, _XGETINTERFACETEXT(ID_STRING_NEW_3147), 0, 0 );

				g_XBaseFont->SetBoldMode(TRUE);
				g_XBaseFont->SetColor( 0xFFFFFFFF );
				//g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3162) );
				//g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+20, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3148) );
				g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3148) );
				g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+20, _XFONT_ALIGNTYPE_CENTER, szBuffer );
				g_XBaseFont->Flush();				
				g_XBaseFont->SetBoldMode(FALSE);
			}
#endif
			
#ifdef _XVNBILLINGSYSTEM_TEST
			g_LocalUserBillingInformations.BillMethod = _VNBILLING_BILLMETHOD_FREEOFCHARGE;

			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->SetColor( 0xFFFFFF00 );

			switch( g_LocalUserBillingInformations.BillMethod )
			{
			case _VNBILLING_BILLMETHOD_FREEOFCHARGE					:
			case _VNBILLING_BILLMETHOD_FREEOFCHARGE_EVENT			:
				{
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+43, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2451) );
				
					// 번역 끝난 후 년월일시간분으로 변경해야함.
					g_XBaseFont->PrintAlign( (gnWidth>>1), gnHeight*0.87239583f+63, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2453),  
											 g_LocalUserBillingInformations.BillExpireTime, g_LocalUserBillingInformations.BillExpireMinute );
				}
				break;
			case _VNBILLING_BILLMETHOD_BASIC_FIXEDAMOUNTSYSTEM		:
				{
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+43, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2454) );
				
					int param1 = g_LocalUserBillingInformations.BillExpireYear;
					int param2 = g_LocalUserBillingInformations.BillExpireMonth;
					int param3 = g_LocalUserBillingInformations.BillExpireDay;
					
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+63, _XFONT_ALIGNTYPE_CENTER, 
											 g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2435, &param1, &param2, &param3 ));
				}
				break;
			case _VNBILLING_BILLMETHOD_BASIC_FIXEDQUANTITYSYSTEM	:
				{
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+43, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2454) );
				
					int param1 = g_LocalUserBillingInformations.BillRemain / 3600000;
					int param2 = (g_LocalUserBillingInformations.BillRemain - (param1 * 3600000)) / 60000;
										
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+63, _XFONT_ALIGNTYPE_CENTER, 
											 g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2434, &param1, &param2 ));
				}
				break;
			case _VNBILLING_BILLMETHOD_PREMIUM_FIXEDAMOUNTSYSTEM	:
				{
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+43, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2455) );
				
					int param1 = g_LocalUserBillingInformations.BillExpireYear;
					int param2 = g_LocalUserBillingInformations.BillExpireMonth;
					int param3 = g_LocalUserBillingInformations.BillExpireDay;
					
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+63, _XFONT_ALIGNTYPE_CENTER, 
											 g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2435, &param1, &param2, &param3 ));
				}
				break;
			case _VNBILLING_BILLMETHOD_PREMIUM_FIXEDQUANTITYSYSTEM	:
				{
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+43, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_2455) );
				
					int param1 = g_LocalUserBillingInformations.BillRemain / 3600000;
					int param2 = (g_LocalUserBillingInformations.BillRemain - (param1 * 3600000)) / 60000;
										
					g_XBaseFont->PutsAlign( (gnWidth>>1), gnHeight*0.87239583f+63, _XFONT_ALIGNTYPE_CENTER, 
											 g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MAINGAME_2434, &param1, &param2 ));
				}
				break;
			}

			g_XBaseFont->Flush();				
			g_XBaseFont->SetBoldMode(FALSE);
#endif			
		}

				
	#ifdef _XDWDEBUG		
		/*
		if( gpInput->CheckKeyPress(DIK_LCONTROL) )
		{
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->Print( 4, gnHeight-13, 1.0f, "FPS: %d", gnFPS );
			g_XBaseFont->Print( 4, gnHeight-39, 1.0f, "X: %f Y: %f Z: %f Yaw: %f Pitch: %f Roll: %f",
				g_LodTerrain.m_3PCamera.m_CameraPosition.x,g_LodTerrain.m_3PCamera.m_CameraPosition.y,
				g_LodTerrain.m_3PCamera.m_CameraPosition.z, g_LodTerrain.m_3PCamera.GetYaw(),
				g_LodTerrain.m_3PCamera.GetPitch(),g_LodTerrain.m_3PCamera.GetRoll() );
			g_XBaseFont->Print( 4, gnHeight-26, 1.0f, "Target X: %f Y: %f Z: %f Distance : %f",
				g_LodTerrain.m_3PCamera.m_TargetPosition.x,g_LodTerrain.m_3PCamera.m_TargetPosition.y,
				g_LodTerrain.m_3PCamera.m_TargetPosition.z,g_LodTerrain.m_3PCamera.m_TargetDistance);
			g_XBaseFont->Flush();
		}	
		g_XBaseFont->Print( 4, gnHeight-60, 1.0f, "Current : %f, Target : %f, Direction : %f", m_CurrentRotateAngle, m_TargetRotateAngle, m_RotateDirection );
		g_XBaseFont->Print( 4, gnHeight-80, 1.0f, "%f %f %f", m_PreviewCharacter[m_SelectedCharacter].m_Position._41,m_PreviewCharacter[m_SelectedCharacter].m_Position._42,m_PreviewCharacter[m_SelectedCharacter].m_Position._43 );
		g_XBaseFont->Flush();
		*/
	#endif		
	}

	if( m_CharacterSelectedMode )
	{
#ifdef _XDEF_NEWANIMATIONLIST
		if( g_SelectAnimationFrameTime > 0 )
		{
			DWORD elapsedtime = g_LocalSystemTime - m_CharacterSelectedModeTimer;
			DWORD halftime = (g_SelectAnimationFrameTime>>1);
			if( elapsedtime > halftime )
			{
				D3DCOLOR color = ((DWORD)( ((elapsedtime-halftime)/(FLOAT)halftime) * 255.0f ) << 24);
				g_ScreenBurnOut[0].color = color;
				g_ScreenBurnOut[1].color = color;
				g_ScreenBurnOut[2].color = color;
				g_ScreenBurnOut[3].color = color;
				gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
				g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );
				gpDev->SetFVF( D3DFVF_XTLVERTEX );
				gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );
			}
		}
#else
		DWORD elapsedtime = g_LocalSystemTime - m_CharacterSelectedModeTimer;
		if( elapsedtime > 500 )
		{
			D3DCOLOR color = ((DWORD)( ((elapsedtime-500)/500.0f) * 255.0f ) << 24);
			g_ScreenBurnOut[0].color = color;
			g_ScreenBurnOut[1].color = color;
			g_ScreenBurnOut[2].color = color;
			g_ScreenBurnOut[3].color = color;
			gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );
			g_MainInterfaceTextureArchive.m_RenderStyleManager.SetRenderStyle( NULL, 3 );
			gpDev->SetFVF( D3DFVF_XTLVERTEX );
			gpDev->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, g_ScreenBurnOut, sizeof(_XTLVERTEX) );
		}
#endif
	}

	// End the scene
	gpDev->EndScene();
	
	// Present the backbuffer contents to the display	
	HRESULT hr = gpDev->Present(NULL, NULL, NULL, NULL);	
	
	if( D3DERR_DEVICELOST == hr )
        gApp->m_bDeviceLost = true;
}

void XProc_SelectCharacter::OnPaint(WPARAM wparam, LPARAM lparam)
{	
	Draw();
}

bool XProc_SelectCharacter::Process( void )
{
	// 빌링 카운터 처리
	//ProcessBilling();
	
//	Author : 양희왕 
//게임가드 유/무 검사 5초 마다
#if defined(_XDEF_NPROTECT_GUARD) && defined(_XUSE_GAMEGUARD)

	static DWORD iCurrentCheckTime = 0;
	if( iCurrentCheckTime == 0 )
	{
		iCurrentCheckTime = g_LocalSystemTime;
	}
	
	if( (g_LocalSystemTime - iCurrentCheckTime) > 10000 )
	{
		iCurrentCheckTime = g_LocalSystemTime;

		if( g_GameGuardInstance.Check() != NPGAMEMON_SUCCESS ) 
		{
			extern LPTSTR g_nProtectModuleCheckMessage;
			_XFatalError(g_nProtectModuleCheckMessage);
			return false;
		}
	}
	
#endif
//Last updated: 07/01/22

	// 서버 연결 유지 패킷 전송
	if( m_LivePacketTimer == 0 )
	{
		m_LivePacketTimer = g_LocalSystemTime;
	}

	if( g_LocalSystemTime - m_LivePacketTimer > 180000 ) // 3분에 한번씩 전송
	{		
		m_LivePacketTimer = g_LocalSystemTime;
		g_NetworkKernel.SendPacket( MSG_NO_TIME_CHECK );
	}

	if(g_pTitleFlashObject) g_pTitleFlashObject->PlayTimerFire();

	if( !g_pTitleFlashObject->IsPlaying() )
	{
		g_MessageBox.Process();
		
		m_WindowManager.Process();		
			
		if( m_FirstRotateAnimation )
		{
			FLOAT elapsedtime = g_fElapsedFrameMilisecondTime * 66.0f;
			FLOAT accelfactor = 1.0f * elapsedtime;

			if( g_LodTerrain.m_3PCamera.GetDistance() == 3.4f )
			{
				if( m_FirstRotateAngle > 330.0f ) accelfactor = 0.34f * elapsedtime;
				else if( m_FirstRotateAngle > 315.0f ) accelfactor = 0.65f * elapsedtime;
				else if( m_FirstRotateAngle > 300.0f ) accelfactor = 0.85f * elapsedtime;
				else accelfactor = 1.0f * elapsedtime;
			}
			else
			{
				if( g_LodTerrain.m_3PCamera.GetDistance() > 9.0f )
				{
					accelfactor = 0.2f * elapsedtime;
				}
				else if( g_LodTerrain.m_3PCamera.GetDistance() > 8.2f )
				{
					accelfactor = 0.6f * elapsedtime;
				}
				else
				{
					if( g_LodTerrain.m_3PCamera.GetDistance() < 4.0f )
					{
						accelfactor = 0.4f * elapsedtime;
					}
					else if( g_LodTerrain.m_3PCamera.GetDistance() < 3.6f )
					{
						accelfactor = 0.14f * elapsedtime;
					}
					else
					{
						accelfactor = 1.0f * elapsedtime;
					}
				}
			}

			BOOL animationcamera = FALSE;
			if( g_LodTerrain.m_3PCamera.GetDistance() > 3.4f )
			{
				g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - (0.3f * accelfactor)  );
				
				if( g_LodTerrain.m_3PCamera.GetDistance() < 3.4f )
				{
					g_LodTerrain.m_3PCamera.SetDistance( 3.4f );
				}
				
				animationcamera = TRUE;
			}
			
			if( g_LodTerrain.m_3PCamera.mp_fPitch > 12.8f )
			{
				g_LodTerrain.m_3PCamera.mp_fPitch = g_LodTerrain.m_3PCamera.mp_fPitch - (3.5f*accelfactor);
				
				if( g_LodTerrain.m_3PCamera.mp_fPitch < 12.8f )
				{
					g_LodTerrain.m_3PCamera.mp_fPitch = 12.8f;
				}
				
				animationcamera = TRUE;
			}
			
			if( animationcamera )
				g_LodTerrain.m_3PCamera.UpdateViewMatrix();

			if( m_FirstRotateAngle < 360.0f )
			{
				const FLOAT rotatespeed = 7.0f;

				m_FirstRotateAngle += (rotatespeed * accelfactor);

				if( m_FirstRotateAngle > 360.0f )
				{
					m_FirstRotateAngle = 0.0f;
					m_FirstRotateAnimation = FALSE;
					m_TargetRotateAngle = m_CurrentRotateAngle = (m_SelectedCharacter * 120.0f);

#ifdef _XDEF_NEWANIMATIONLIST
					if( m_SelectedCharacter >= 0 )
					{
						int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1, 
							g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1Category );				
						int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex];
						
						m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][0], true );				
					}
#endif
				}			
			}
			else
			{
				m_FirstRotateAngle = 0.0f;
				m_FirstRotateAnimation = FALSE;
				m_TargetRotateAngle = m_CurrentRotateAngle = (m_SelectedCharacter * 120.0f);

#ifdef _XDEF_NEWANIMATIONLIST
				if( m_SelectedCharacter >= 0 )
				{
					int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1, 
														  g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1Category );				
					int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex];
					m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][0], true );
				}
#endif
			}

			D3DXMATRIX	rotmat,charrotmat;
			D3DXVECTOR3 position;
			D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.0f, 0.45f);		
			D3DXVECTOR3	rotcharacterpos;
			
			for( int i = 0; i < 3; i++ )
			{
				D3DXMatrixRotationY(&rotmat, _X_RAD( (-120.0f * i) + m_FirstRotateAngle + (m_SelectedCharacter * 120.0f) ));
				D3DXMatrixRotationY(&charrotmat, _X_RAD(180.0f - 120.0f * i + m_FirstRotateAngle + (m_SelectedCharacter * 120.0f) ));
				
				D3DXVec3TransformCoord( &rotcharacterpos, &characterpos, &rotmat );
				m_PreviewCharacter[i].m_Position = charrotmat;
				m_PreviewCharacter[i].m_Position._41 = rotcharacterpos.x;
				m_PreviewCharacter[i].m_Position._42 = rotcharacterpos.y;
				m_PreviewCharacter[i].m_Position._43 = rotcharacterpos.z;
			}
		}
		else if( !m_CharacterDeleteMode )
		{
			if( fabs( fabs(m_CurrentRotateAngle) - fabs(m_TargetRotateAngle) ) < EPSILON3 )
			{
				m_CurrentRotateAngle = m_TargetRotateAngle;
				m_RotateDirection = 0.0f;
			}
			else
			{
				const FLOAT rotatespeed = 6.0f;

				m_CurrentRotateAngle += rotatespeed * m_RotateDirection;

				if( m_CurrentRotateAngle > 360.0f )
					m_CurrentRotateAngle = fmod( m_CurrentRotateAngle, 360.0f );

				if( m_CurrentRotateAngle < 0.0f )
					m_CurrentRotateAngle = 360.0f - m_CurrentRotateAngle;

				if( m_RotateDirection == -1.0f )
				{			
					if( m_CurrentRotateAngle < m_TargetRotateAngle )
					{
						m_CurrentRotateAngle = m_TargetRotateAngle;
						m_RotateDirection = 0.0f;
					}
				}
				else
				{
					if( m_CurrentRotateAngle > m_TargetRotateAngle )
					{
						m_CurrentRotateAngle = m_TargetRotateAngle;
						m_RotateDirection = 0.0f;
					}
				}
				
				D3DXMATRIX	rotmat,charrotmat;
				D3DXVECTOR3 position;
				//D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.82f, 0.45f);		
				D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.0f, 0.45f);		
				D3DXVECTOR3	rotcharacterpos;
				
				for( int i = 0; i < 3; i++ )
				{
					D3DXMatrixRotationY(&rotmat, _X_RAD( (-120.0f * i) + m_CurrentRotateAngle ));
					D3DXMatrixRotationY(&charrotmat, _X_RAD(180.0f - 120.0f * i + m_CurrentRotateAngle + m_CharacterLocalRotateAngle[i] ));
					
					D3DXVec3TransformCoord( &rotcharacterpos, &characterpos, &rotmat );
					m_PreviewCharacter[i].m_Position = charrotmat;
					m_PreviewCharacter[i].m_Position._41 = rotcharacterpos.x;
					m_PreviewCharacter[i].m_Position._42 = rotcharacterpos.y;
					m_PreviewCharacter[i].m_Position._43 = rotcharacterpos.z;
				}
			}
		}
	}

	if( m_CharacterSelectedMode )
	{
#ifdef _XDEF_NEWANIMATIONLIST
		if( m_SelectedCharacter >= 0 )
		{
			int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1, 
												  g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1Category );
			int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex];

			if( m_PreviewCharacter[m_SelectedCharacter].GetCurMotion() != g_PreviewCharacterMotionIndex[itemcategory][lutindex][2] )
			{
				m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][2] );
				
				_LPXM_MOTION pMotion = m_PreviewCharacter[m_SelectedCharacter].m_pBipedController->GetMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][2] );

				g_SelectAnimationFrameTime = (int)(((FLOAT)((pMotion->EndFrame - pMotion->StartFrame + 1) * pMotion->FrameSpeed)) * 0.95f);
			}
		
			DWORD elapsedtime = g_LocalSystemTime - m_CharacterSelectedModeTimer;
			if( elapsedtime > g_SelectAnimationFrameTime )
			{
				((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);
			}
			else
			{
				D3DXMATRIX	rotmat,charrotmat;
				D3DXVECTOR3 position;
				//D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.82f, 0.45f);		
				D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.3f * (elapsedtime / 1000.0f), 0.45f + 0.35f * (elapsedtime / 1000.0f) );
				D3DXVECTOR3	rotcharacterpos;

				D3DXMatrixRotationY(&rotmat, _X_RAD( (-120.0f * m_SelectedCharacter) + m_CurrentRotateAngle ));
				D3DXMatrixRotationY(&charrotmat, _X_RAD(180.0f - 120.0f * m_SelectedCharacter + m_CurrentRotateAngle ));
				
				D3DXVec3TransformCoord( &rotcharacterpos, &characterpos, &rotmat );
				m_PreviewCharacter[m_SelectedCharacter].m_Position = charrotmat;
				m_PreviewCharacter[m_SelectedCharacter].m_Position._41 = rotcharacterpos.x;
				m_PreviewCharacter[m_SelectedCharacter].m_Position._42 = rotcharacterpos.y;
				m_PreviewCharacter[m_SelectedCharacter].m_Position._43 = rotcharacterpos.z;
				
				/*
				if( m_PreviewCharacter[m_SelectedCharacter].m_EffectManager )
					m_PreviewCharacter[m_SelectedCharacter].m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_LOGIN,
					&m_PreviewCharacter[m_SelectedCharacter].m_Position, NULL, TRUE);
				*/
			}
		}
#else
		DWORD elapsedtime = g_LocalSystemTime - m_CharacterSelectedModeTimer;
		if( elapsedtime > 1000 )
		{
			((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_loading);
		}
		else
		{
			D3DXMATRIX	rotmat,charrotmat;
			D3DXVECTOR3 position;
			//D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.82f, 0.45f);		
			D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.3f * (elapsedtime / 1000.0f), 0.45f + 1.0 * (elapsedtime / 1000.0f) );
			D3DXVECTOR3	rotcharacterpos;
			
			D3DXMatrixRotationY(&rotmat, _X_RAD( (-120.0f * m_SelectedCharacter) + m_CurrentRotateAngle ));
			D3DXMatrixRotationY(&charrotmat, _X_RAD(180.0f - 120.0f * m_SelectedCharacter + m_CurrentRotateAngle ));
			
			D3DXVec3TransformCoord( &rotcharacterpos, &characterpos, &rotmat );
			m_PreviewCharacter[m_SelectedCharacter].m_Position = charrotmat;
			m_PreviewCharacter[m_SelectedCharacter].m_Position._41 = rotcharacterpos.x;
			m_PreviewCharacter[m_SelectedCharacter].m_Position._42 = rotcharacterpos.y;
			m_PreviewCharacter[m_SelectedCharacter].m_Position._43 = rotcharacterpos.z;
			m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( 185 );
			
			/*
			if( m_PreviewCharacter[m_SelectedCharacter].m_EffectManager )
			m_PreviewCharacter[m_SelectedCharacter].m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, _XDEF_MONSTERID_LOGIN,
			&m_PreviewCharacter[m_SelectedCharacter].m_Position, NULL, TRUE);
			*/
		}
#endif
	}
	else
	{
		for( int i = 0; i < g_pLocalUser->m_CharacterCount; i++ )
		{	
			D3DXVECTOR3 pos;	
			pos.x = m_PreviewCharacter[i].m_Position._41;
			pos.y = m_PreviewCharacter[i].m_Position._42;
			pos.z = m_PreviewCharacter[i].m_Position._43;
			D3DXVECTOR3 pickedpoint;

			if( m_CharacterOBB.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, pos, pickedpoint ) )
			{
				
			}
		}
	}
#ifdef _XDEF_NEWANIMATIONLIST						
	g_CharacterEffectGroupManager.Update( g_fElapsedFrameMilisecondTime, g_fLocalSystemTime );
#endif
	
#ifdef _XUSEFMOD
	g_FMODWrapper.ProcessBGM();
	g_FMODWrapper.FMODUpdate();	
#endif

	return true;
}

void XProc_SelectCharacter::MessageProc( UINT message, WPARAM wparam, LPARAM lparam )
{
	_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_SELECTCHARACTER);
	
	if( message == WM_COMMAND )
	{
		switch( wparam )
		{
		case _XDEF_CS_CONNECTBUTTON :
			
			// 베트남 셧다운 
#ifdef _XDEF_SHUTDOWN_20061027	
			// 20061200 셧다운 일때 남은 플레이시간이 0이면 게임에 들어갈 수 없다.
			
			// 20070112 시간이 다 소모해도 게임에 들어갈 수 있다.
			//if( g_PlayLeftTime == 0 )
			//	break;
#endif			
			if( !g_pTitleFlashObject->IsPlaying() && !m_CharacterDeleteMode && !m_FirstRotateAnimation )
			{
				if( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].Get_gm_level() <= -1 )
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2777) ); //사용 제한된 인물은 선택하실 수 없습니다.
				}
				else
				{
					
#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
					if( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].ucChangeNameFlag )
					{
						_XWindow_ChangeName* pChangeName_Window = (_XWindow_ChangeName*)m_WindowManager.FindWindow(_XDEF_WTITLE_CHANGENAMEWINDOW);
						if( pChangeName_Window )
						{
							pChangeName_Window->SetSelectedIndex(this->m_SelectedCharacter);
							pChangeName_Window->ShowWindow(TRUE);
						}
					}
					else
					{
						m_LockCharacterSelection = TRUE;
						g_NetworkKernel.SendPacket( MSG_NO_CHARAC_SELECT );
						pSelectCharacter_Window->m_StartGameButton.EnableWindow(FALSE);
						g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDCHARACTER, this->m_SelectedCharacter );
						
						// parameter setting
						g_pLocalUser->m_Strength.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength;
						g_pLocalUser->m_Zen.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen;
						g_pLocalUser->m_Intelligence.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence;
						g_pLocalUser->m_Dex.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity;
						g_pLocalUser->m_Constitution.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution;
						
						// 플레이 타임 체크 시작
/*#ifdef _XTS_WARNPLAYTIME
						g_PlayStartTime = 0;
						g_ViewWarnPlayMessage = 0;
						g_bViewRegistrationNumberMessage = FALSE;
#endif*/
						
						_XPlayInterfaceSound( ID_SR_INTERFACE_CHARACTER_START_WAV );
					}
#else
					m_LockCharacterSelection = TRUE;
					g_NetworkKernel.SendPacket( MSG_NO_CHARAC_SELECT );
					pSelectCharacter_Window->m_StartGameButton.EnableWindow(FALSE);
					g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_LASTSELECTEDCHARACTER, this->m_SelectedCharacter );
					
					// parameter setting
					g_pLocalUser->m_Strength.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength;
					g_pLocalUser->m_Zen.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen;
					g_pLocalUser->m_Intelligence.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence;
					g_pLocalUser->m_Dex.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity;
					g_pLocalUser->m_Constitution.org = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution;

					// 플레이 타임 체크 시작
#ifdef _XTS_WARNPLAYTIME
					g_PlayStartTime = 0;
					g_ViewWarnPlayMessage = 0;
					g_bViewRegistrationNumberMessage = FALSE;
#endif // #ifdef _XTS_WARNPLAYTIME
					
					_XPlayInterfaceSound( ID_SR_INTERFACE_CHARACTER_START_WAV );
#endif // #if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
				}	
			}
			break;
		case _XDEF_CS_CREATEBUTTON  :
			{
				if( pSelectCharacter_Window->m_CreateCharacterButton.IsButtonEnabled() )			
				{
					if( !g_pTitleFlashObject->IsPlaying() && !m_CharacterDeleteMode && !m_CharacterSelectedMode && !m_FirstRotateAnimation )
					{
						((CEmperorOfDragonsApp*)gApp)->m_ProcessManager.ChangeProcess(&((CEmperorOfDragonsApp*)gApp)->m_proc_createcharacter);
					}
				}
			}
			break;
		case _XDEF_CS_DELETEBUTTON  :
			{
				if( !g_pTitleFlashObject->IsPlaying() && !m_CharacterDeleteMode && !m_CharacterSelectedMode && !m_FirstRotateAnimation )
				{
					HDC pDC = GetDC( gHWnd );
					g_pTitleFlashObject->SetDC( pDC );
					
					TCHAR flashfilename[64];
					
					memset( flashfilename, 0, sizeof(TCHAR) * 64 );

					if( g_LanguageType == _XLANGUAGE_TYPE_KOREAN )
					{			
						strcpy( flashfilename, _T("DELETE00.swf") );
					}
					else if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
					{
						strcpy( flashfilename, _T("vn_DELETE00.swf") );
					}
					else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
					{
						strcpy( flashfilename, _T("tw_DELETE00.swf") );
					}
					else
					{
						strcpy( flashfilename, _T("DELETE00.swf") );
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

					_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_SELECTCHARACTER);				
					if( pSelectCharacter_Window )
					{
						FLOAT xfactor = (FLOAT)gnWidth  / 1024.0f;
						FLOAT yfactor = (FLOAT)gnHeight / 768.0f;

						int halfsize = (196>>1);												
						pSelectCharacter_Window->MoveWindow( (int)((702.0f+halfsize)*xfactor) - halfsize, (int)(82*yfactor) );
						pSelectCharacter_Window->m_Blank01Button.ShowWindow( FALSE );
						pSelectCharacter_Window->m_Blank02Button.ShowWindow( FALSE );
						pSelectCharacter_Window->m_Blank03Button.ShowWindow( FALSE );

						pSelectCharacter_Window->m_StartGameButton.ShowWindow( FALSE );
						pSelectCharacter_Window->m_CreateCharacterButton.ShowWindow( FALSE );
						pSelectCharacter_Window->m_DeleteCharacterButton.ShowWindow( FALSE );
						pSelectCharacter_Window->m_ExitButton.ShowWindow( FALSE );

						pSelectCharacter_Window->m_MenuEdge_Top.ShowWindow( FALSE );
						pSelectCharacter_Window->m_MenuEdge_Bottom.ShowWindow( FALSE );
						pSelectCharacter_Window->m_MenuEdge_Left.ShowWindow( FALSE );
						pSelectCharacter_Window->m_MenuEdge_Right.ShowWindow( FALSE );
					}		

					m_CharacterDeleteMode = TRUE;

					if(m_SelectedCharacter >= 0)
					{
						m_PrevSelectedCharacterPosition.x = m_PreviewCharacter[m_SelectedCharacter].m_Position._41;
						m_PrevSelectedCharacterPosition.y = m_PreviewCharacter[m_SelectedCharacter].m_Position._42;
						m_PrevSelectedCharacterPosition.z = m_PreviewCharacter[m_SelectedCharacter].m_Position._43;

						D3DXMatrixRotationYawPitchRoll(&m_PreviewCharacter[m_SelectedCharacter].m_Position, _X_RAD( 180 ), _X_RAD( 0.8f), _X_RAD( -3.0f ) );
						m_PreviewCharacter[m_SelectedCharacter].m_Position._41 = 0.49f;//0.36f;//0.26f;w	
						m_PreviewCharacter[m_SelectedCharacter].m_Position._42 = 0.003f;//-0.35f//-0.14f;
						m_PreviewCharacter[m_SelectedCharacter].m_Position._43 = 0.88f;////1.7f;//2.13f;

#ifdef _XDEF_NEWANIMATIONLIST					
					
						int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1, 
															  g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1Category );
						m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory] );
#else				
						m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( 0 );
#endif
					}
					
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_SELECTCHARACTER_WARNDELETECHARACTER, TRUE, 
											 _XDEF_CS_TYPEPASSWORD, _XDEF_CS_CANCELDELETEBUTTON, _XDEF_CS_CANCELDELETEBUTTON );
				}
			}
			break;
		case _XDEF_CS_TYPEPASSWORD :			
			g_MessageBox.SetPasswordMode( TRUE );
			g_MessageBox.m_IMEControl.m_VKeyboardTypedCount = 0;
			
			// Vietnamese password delete.
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			{
				g_MessageBox.m_IMEControl.SetMaxLength(24);
				g_MessageBox.m_IMEControl.SetHorizScrollMode(TRUE);
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
			{
				g_MessageBox.m_IMEControl.SetMaxLength(12);
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
			{
				g_MessageBox.m_IMEControl.SetMaxLength(12);
			}
			else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_MessageBox.m_IMEControl.SetMaxLength(12);
			}
			else
			{
				g_MessageBox.m_IMEControl.SetMaxLength(7);
			}

#ifdef _XDEF_NEWDELETECHARACTER
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_SOCIALSENUMBER, ID_STRING_SELECTCHARACTER_INPUTPASSWD, TRUE, 
									 _XDEF_CS_DELETECHARACTER, _XDEF_CS_CANCELDELETEBUTTON, _XDEF_CS_CANCELDELETEBUTTON );
#else
			g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_INPUTSTRING, ID_STRING_SELECTCHARACTER_INPUTPASSWD, TRUE, 
									 _XDEF_CS_DELETECHARACTER, _XDEF_CS_CANCELDELETEBUTTON );
#endif
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
			{
				_XWindow_DrawKeyboard* pDrawKeyBoard_Window = (_XWindow_DrawKeyboard*)m_WindowManager.FindWindow(_XDEF_WTITLE_DRAWKEYBOARDWINDOW);
				if(pDrawKeyBoard_Window)
					pDrawKeyBoard_Window->ShowWindow(TRUE);
			}

			break;		
		case _XDEF_CS_DELETEAGAINCHARACTER :
			{
				g_MessageBox.m_IMEControl.m_VKeyboardTypedCount = 0;
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_SELECTCHARACTER_WARNDELETECHARACTER, TRUE, 
										 _XDEF_CS_TYPEPASSWORD, _XDEF_CS_CANCELDELETEBUTTON, _XDEF_CS_CANCELDELETEBUTTON );
			}
			break;
		case _XDEF_CS_DELETECHARACTER :
			{
				// 베트남어일 경우 스크린키보드로 최소 3회 이상 입력해야 한다.
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
				{
					if( g_MessageBox.m_IMEControl.m_VKeyboardTypedCount < 3 )
					{
						g_MessageBox.m_IMEControl.ClearBuffer();
						g_MessageBox.m_IMEControl.m_VKeyboardTypedCount = 0;

						g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3045), TRUE, _XDEF_CS_DELETEAGAINCHARACTER );
						break;
					}
				}

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
				{
					_XWindow_DrawKeyboard* pDrawKeyBoard_Window = (_XWindow_DrawKeyboard*)m_WindowManager.FindWindow(_XDEF_WTITLE_DRAWKEYBOARDWINDOW);
					if(pDrawKeyBoard_Window)
						pDrawKeyBoard_Window->ShowWindow(FALSE);
				}

				g_MessageBox.m_IMEControl.SetMaxLength(MAX_IMEBUFFERLENGTH);
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_NOTICE, ID_STRING_SELECTCHARACTER_DELETECHARACTER, TRUE );
				g_MessageBox.StartTimer( 20000 );
				g_NetworkKernel.SendPacket( MSG_NO_CHARAC_DELETE );
			}
			break;
		case _XDEF_CS_CANCELDELETEBUTTON :
			{
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
				{
					_XWindow_DrawKeyboard* pDrawKeyBoard_Window = (_XWindow_DrawKeyboard*)m_WindowManager.FindWindow(_XDEF_WTITLE_DRAWKEYBOARDWINDOW);
					if(pDrawKeyBoard_Window)
						pDrawKeyBoard_Window->ShowWindow(FALSE);
				}

				g_MessageBox.m_IMEControl.ResetContainer();
				g_MessageBox.m_IMEControl.SetMaxLength(MAX_IMEBUFFERLENGTH);

				HDC pDC = GetDC( gHWnd );
				g_pTitleFlashObject->SetDC( pDC );
				
				TCHAR flashfilename[64];
				
				memset( flashfilename, 0, sizeof(TCHAR) * 64 );

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
				package.ClosePackage();
				
				g_pTitleFlashObject->SetDC( 0 );
				ReleaseDC( gHWnd, pDC );
				
				g_pTitleFlashObject->SetOffScreenTextureObject( g_pTitleFlashOffScreenTexture );				

				_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_SELECTCHARACTER);				
				if( pSelectCharacter_Window )
				{
					FLOAT xfactor = (FLOAT)gnWidth  / 1024.0f;
					FLOAT yfactor = (FLOAT)gnHeight / 768.0f;
					int halfsize = (196>>1);

					pSelectCharacter_Window->MoveWindow( (int)((80.0f+halfsize)*xfactor) - halfsize, (int)(82*yfactor) );
					pSelectCharacter_Window->m_Blank01Button.ShowWindow( TRUE );
					pSelectCharacter_Window->m_Blank02Button.ShowWindow( TRUE );
					pSelectCharacter_Window->m_Blank03Button.ShowWindow( TRUE );
					
					pSelectCharacter_Window->m_StartGameButton.ShowWindow( TRUE );
					pSelectCharacter_Window->m_CreateCharacterButton.ShowWindow( TRUE );
					pSelectCharacter_Window->m_DeleteCharacterButton.ShowWindow( TRUE );
					pSelectCharacter_Window->m_ExitButton.ShowWindow( TRUE );
					
					pSelectCharacter_Window->m_MenuEdge_Top.ShowWindow( TRUE );
					pSelectCharacter_Window->m_MenuEdge_Bottom.ShowWindow( TRUE );
					pSelectCharacter_Window->m_MenuEdge_Left.ShowWindow( TRUE );
					pSelectCharacter_Window->m_MenuEdge_Right.ShowWindow( TRUE );
				}

				m_CharacterDeleteMode = FALSE;
				if(m_SelectedCharacter >= 0)
				{
					D3DXMatrixRotationY(&m_PreviewCharacter[m_SelectedCharacter].m_Position, _X_RAD(180.0f - 120.0f * m_SelectedCharacter + m_CurrentRotateAngle ));
					m_PreviewCharacter[m_SelectedCharacter].m_Position._41 = m_PrevSelectedCharacterPosition.x;
					m_PreviewCharacter[m_SelectedCharacter].m_Position._42 = m_PrevSelectedCharacterPosition.y;
					m_PreviewCharacter[m_SelectedCharacter].m_Position._43 = m_PrevSelectedCharacterPosition.z;
				
#ifdef _XDEF_NEWANIMATIONLIST					
					int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1, 
														  g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1Category );

					int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex];

					m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][0], true );
#else
					m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex]
																	  [g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].gender][0], true );
#endif
				}
			}
			break;
		case _XDEF_CS_REQUESTDELETECHARACTER :
			{
				HDC pDC = GetDC( gHWnd );
				g_pTitleFlashObject->SetDC( pDC );
				
				TCHAR flashfilename[64];
				
				memset( flashfilename, 0, sizeof(TCHAR) * 64 );

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

				//g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_SELECTCHARACTER_DELETEDCHARACTER, TRUE );

				if( g_pLocalUser->m_CharacterCount > 0 )
				{
					pSelectCharacter_Window->m_SelectedCharacter = this->m_SelectedCharacter = 0;
					g_pLocalUser->SelectCharacter( this->m_SelectedCharacter );	
					
					pSelectCharacter_Window->m_StartGameButton.EnableWindow(TRUE);
					pSelectCharacter_Window->m_DeleteCharacterButton.EnableWindow(TRUE);

					if(g_pLocalUser->m_CharacterCount < 3)
						pSelectCharacter_Window->m_CreateCharacterButton.EnableWindow(TRUE);
					else 
						pSelectCharacter_Window->m_CreateCharacterButton.EnableWindow(FALSE);
				}				
				else
				{
					this->m_SelectedCharacter = -1;
					pSelectCharacter_Window->m_StartGameButton.EnableWindow(FALSE);
					pSelectCharacter_Window->m_DeleteCharacterButton.EnableWindow(FALSE);
					pSelectCharacter_Window->m_CreateCharacterButton.EnableWindow(TRUE);
				}

				if( g_pLocalUser->m_CharacterCount >= 1 )
					pSelectCharacter_Window->m_Blank01Button.SetButtonText( g_pLocalUser->m_AllCharacterInfo[0].charactername, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	
				else
					pSelectCharacter_Window->m_Blank01Button.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_CHARACTER1, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	// 인물 一
				
				if( g_pLocalUser->m_CharacterCount >= 2 )
					pSelectCharacter_Window->m_Blank02Button.SetButtonText( g_pLocalUser->m_AllCharacterInfo[1].charactername, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	
				else
					pSelectCharacter_Window->m_Blank02Button.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_CHARACTER2, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	// 인물 二
				
				if( g_pLocalUser->m_CharacterCount >= 3 )
					pSelectCharacter_Window->m_Blank03Button.SetButtonText( g_pLocalUser->m_AllCharacterInfo[2].charactername, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	
				else
					pSelectCharacter_Window->m_Blank03Button.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SELECTCHARACTER_CHARACTER3, _XFONT_ALIGNTYPE_LEFT, 37, 0 );	// 인물 三

				if( g_pLocalUser->m_CharacterCount < 1 )
				{
					pSelectCharacter_Window->m_DrawCharacterInfo = FALSE;
				}
				else
				{
					pSelectCharacter_Window->m_DrawCharacterInfo = TRUE;
				}
				
				SetCharacterInfoWindow();

				FLOAT xfactor = (FLOAT)gnWidth  / 1024.0f;
				FLOAT yfactor = (FLOAT)gnHeight / 768.0f;
				int halfsize = (196>>1);
				pSelectCharacter_Window->MoveWindow( (int)((80.0f+halfsize)*xfactor) - halfsize, (int)(82*yfactor) );
				
				pSelectCharacter_Window->m_Blank01Button.ShowWindow( TRUE );
				pSelectCharacter_Window->m_Blank02Button.ShowWindow( TRUE );
				pSelectCharacter_Window->m_Blank03Button.ShowWindow( TRUE );
				
				pSelectCharacter_Window->m_StartGameButton.ShowWindow( TRUE );
				pSelectCharacter_Window->m_CreateCharacterButton.ShowWindow( TRUE );
				pSelectCharacter_Window->m_DeleteCharacterButton.ShowWindow( TRUE );
				pSelectCharacter_Window->m_ExitButton.ShowWindow( TRUE );
				
				pSelectCharacter_Window->m_MenuEdge_Top.ShowWindow( TRUE );
				pSelectCharacter_Window->m_MenuEdge_Bottom.ShowWindow( TRUE );
				pSelectCharacter_Window->m_MenuEdge_Left.ShowWindow( TRUE );
				pSelectCharacter_Window->m_MenuEdge_Right.ShowWindow( TRUE );				

				m_CharacterDeleteMode = FALSE;
				

				_XUSER_MODELDESCRIPTTYPE modeldescript;
				memset( &modeldescript, 0, sizeof( _XUSER_MODELDESCRIPTTYPE ) );
				for(int i = 0; i < 3; i++ )
				{										
					m_PreviewCharacter[i].Reset();

					if( i < g_pLocalUser->m_CharacterCount )
					{ 
						modeldescript = g_pLocalUser->m_AllCharacterInfo[i].modeldescript;
						m_PreviewCharacter[i].ReserveModelStack( _XDEF_MAXMODEL_COMBINATIONCOUNT );
						
						if( g_pLocalUser->m_AllCharacterInfo[i].gender == _XGENDER_MAN )
						{
							m_PreviewCharacter[i].LinkBipedObject( &g_MaleBipedObject );
						}
						else
						{
							m_PreviewCharacter[i].LinkBipedObject( &g_FemaleBipedObject );
						}
						
						m_PreviewCharacter[i].SetMeshModel( modeldescript, g_pLocalUser->m_AllCharacterInfo[i].gender, g_pLocalUser->m_AllCharacterInfo[i].groupindex );
						
#ifdef _XDEF_NEWANIMATIONLIST
						int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1, 
															  g_pLocalUser->m_AllCharacterInfo[i].modeldescript.item1Category );
						m_PreviewCharacter[i].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory] );
#else
						m_PreviewCharacter[i].SetCurMotion( 0 );
#endif
						
						m_PreviewCharacter[i].PlayAnimation( TRUE );
						m_PreviewCharacter[i].SetShadowDecalInfo( 0.65f, 0.85f, 1.85f );

						m_PreviewCharacter[i].InitEffectManager(1,0);
					}
				}

				
				D3DXMATRIX	rotmat,charrotmat;
				D3DXVECTOR3 position;
				//D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.82f, 0.45f);		
				D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.0f, 0.45f);		
				D3DXVECTOR3	rotcharacterpos;
				
				for( i = 0; i < 3; i++ )
				{
					D3DXMatrixRotationY(&rotmat, _X_RAD( (-120.0f * i) ));
					D3DXMatrixRotationY(&charrotmat, _X_RAD(180.0f - 120.0f * i ));
					
					D3DXVec3TransformCoord( &rotcharacterpos, &characterpos, &rotmat );
					m_PreviewCharacter[i].m_Position = charrotmat;
					m_PreviewCharacter[i].m_Position._41 = rotcharacterpos.x;
					m_PreviewCharacter[i].m_Position._42 = rotcharacterpos.y;
					m_PreviewCharacter[i].m_Position._43 = rotcharacterpos.z;
				}

				m_SelectedCharacter = 0;
				m_CurrentRotateAngle = 0.0f;
				m_TargetRotateAngle = 0.0f;

				if(m_SelectedCharacter >= 0)
				{
#ifdef _XDEF_NEWANIMATIONLIST
					int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1, 
														  g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1Category );

					int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex];

					m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][0], true );
					
					itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[1].modeldescript.item1, 
													  g_pLocalUser->m_AllCharacterInfo[1].modeldescript.item1Category );
					m_PreviewCharacter[1].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory], true );

					itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[2].modeldescript.item1, 
													  g_pLocalUser->m_AllCharacterInfo[2].modeldescript.item1Category );
					m_PreviewCharacter[2].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory], true );				
#else
					m_PreviewCharacter[m_SelectedCharacter].SetCurMotion( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex]
																									   [g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].gender][0], true );
					m_PreviewCharacter[1].SetCurMotion( 0, true );
					m_PreviewCharacter[2].SetCurMotion( 0, true );
#endif
				}
			}
			break;
		case _XDEF_CS_CANCELBUTTON	:
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
		case _XDEF_CS_CHARACTER01BUTTON :
			{
				if( m_LockCharacterSelection ) break;

				if(g_pLocalUser->m_CharacterCount >= 1)
				{
					if( m_RotateDirection == 0.0f )
					{
						m_CharacterLocalRotateAngle[ 0 ] = 0.0f;
						m_CharacterLocalRotateAngle[ 1 ] = 0.0f;
						m_CharacterLocalRotateAngle[ 2 ] = 0.0f;

						if( this->m_SelectedCharacter == 2 )
						{
							m_RotateDirection = 1.0f;
							m_TargetRotateAngle = 360.0f;
						}
						else
						{
							m_RotateDirection = -1.0f;
							m_TargetRotateAngle = 0.0f;
						}				

						if( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid != 
							g_pLocalUser->m_AllCharacterInfo[0].zoneuniqid )
						{
							pSelectCharacter_Window->m_MinimapAlphaLevel[0] = 0;
						}
						else
						{
							pSelectCharacter_Window->m_MinimapAlphaLevel[0] = 255;
						}

						pSelectCharacter_Window->m_SelectedCharacter = this->m_SelectedCharacter = 0;		
						
						pSelectCharacter_Window->m_Blank01Button.MoveWindow( gnWidth - (1024 - 857), gnHeight - (768 - 457) );
						pSelectCharacter_Window->m_Blank02Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 497) );
						pSelectCharacter_Window->m_Blank03Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 538) );

#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
						if( g_pLocalUser->m_AllCharacterInfo[0].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFFFFFF );
						if( g_pLocalUser->m_AllCharacterInfo[1].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
						if( g_pLocalUser->m_AllCharacterInfo[2].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );

#else
						pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFFFFFF );
						pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
						pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );
#endif

						pSelectCharacter_Window->RebuildCharacterGroupNameStatic( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex );

						_XLocalUser::GetUserLevel( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].innerlevel, m_CurrentCharacterLevelIndex, m_CurrentCharacterSubLevel );
						g_pLocalUser->SelectCharacter(this->m_SelectedCharacter);
						strcpy(pSelectCharacter_Window->m_Name, g_pLocalUser->m_CharacterInfo.charactername);					
						TCHAR levelname[128]; 
						
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
						{
							strcpy(pSelectCharacter_Window->m_Level, g_LevelName[ m_CurrentCharacterLevelIndex ] );
							sprintf(pSelectCharacter_Window->m_SubLevel, "%d %s",  m_CurrentCharacterSubLevel, m_String_LevelStar );
						}
						else
						{
							sprintf(levelname, "%s %d %s",  g_LevelName[ m_CurrentCharacterLevelIndex ],
															m_CurrentCharacterSubLevel, m_String_LevelStar );
							strcpy(pSelectCharacter_Window->m_Level, levelname);
						}

						strcpy(pSelectCharacter_Window->m_CharacterLocation, g_ZoneInfoTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid-1].zoneshortname );

						if( strlen( g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
															[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
															[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] ) > 0 )
						{
							strcpy(pSelectCharacter_Window->m_RollClass, 
								g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
														[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
														[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] );
						}
						else
						{
							strcpy(pSelectCharacter_Window->m_RollClass, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_MU) );
						}
						
						pSelectCharacter_Window->m_constitution	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution;
						pSelectCharacter_Window->m_zen			= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen;
						pSelectCharacter_Window->m_intelligence	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence;
						pSelectCharacter_Window->m_dexterity	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity;
						pSelectCharacter_Window->m_strength		= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength;
						pSelectCharacter_Window->m_DrawCharacterInfo = TRUE;
						pSelectCharacter_Window->m_CharacterGMLevel = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].Get_gm_level();
						
						m_PreviewCharacter[0].m_FrameResult = _XDEF_ACI_NONE;
						m_PreviewCharacter[1].m_FrameResult = _XDEF_ACI_NONE;
						m_PreviewCharacter[2].m_FrameResult = _XDEF_ACI_NONE;

#ifdef _XDEF_NEWANIMATIONLIST
						int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[0].modeldescript.item1, g_pLocalUser->m_AllCharacterInfo[0].modeldescript.item1Category );						
						int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[0].groupindex];

						m_PreviewCharacter[0].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][0], true );

						//m_PreviewCharacter[0].SetCurMotion( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[0].groupindex]
						//																 [g_pLocalUser->m_AllCharacterInfo[0].gender][0], true );
						itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[1].modeldescript.item1, 
							g_pLocalUser->m_AllCharacterInfo[1].modeldescript.item1Category );
						m_PreviewCharacter[1].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory], true );
						
						itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[2].modeldescript.item1, 
							g_pLocalUser->m_AllCharacterInfo[2].modeldescript.item1Category );
						m_PreviewCharacter[2].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory], true );				
#else
						m_PreviewCharacter[0].SetCurMotion( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[0].groupindex]
																						 [g_pLocalUser->m_AllCharacterInfo[0].gender][0], true );
						m_PreviewCharacter[1].SetCurMotion( 0, true );
						m_PreviewCharacter[2].SetCurMotion( 0, true );
#endif
					}
				}
			}
			break;
		case _XDEF_CS_CHARACTER02BUTTON :
			{
				if( m_LockCharacterSelection ) break;
				
				if(g_pLocalUser->m_CharacterCount >= 2)
				{
					if( m_RotateDirection == 0.0f )
					{
						m_CharacterLocalRotateAngle[ 0 ] = 0.0f;
						m_CharacterLocalRotateAngle[ 1 ] = 0.0f;
						m_CharacterLocalRotateAngle[ 2 ] = 0.0f;

						if( this->m_SelectedCharacter == 0 )
						{
							m_RotateDirection = 1.0f;
						}
						else
						{
							m_RotateDirection = -1.0f;
						}

						m_TargetRotateAngle = 120.0f;

						if( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid != 
							g_pLocalUser->m_AllCharacterInfo[1].zoneuniqid )
						{
							pSelectCharacter_Window->m_MinimapAlphaLevel[1] = 0;
						}
						else
						{
							pSelectCharacter_Window->m_MinimapAlphaLevel[1] = 255;
						}

						pSelectCharacter_Window->m_SelectedCharacter = this->m_SelectedCharacter = 1;
						
						pSelectCharacter_Window->m_Blank01Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 457) );
						pSelectCharacter_Window->m_Blank02Button.MoveWindow( gnWidth - (1024 - 857), gnHeight - (768 - 497) );
						pSelectCharacter_Window->m_Blank03Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 538) );

#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
						if( g_pLocalUser->m_AllCharacterInfo[0].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
						if( g_pLocalUser->m_AllCharacterInfo[1].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFFFFFF );
						if( g_pLocalUser->m_AllCharacterInfo[2].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );

#else
						pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
						pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFFFFFF );
						pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFB1B1B3 );
#endif

						pSelectCharacter_Window->RebuildCharacterGroupNameStatic( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex );

						_XLocalUser::GetUserLevel( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].innerlevel, m_CurrentCharacterLevelIndex, m_CurrentCharacterSubLevel );
						g_pLocalUser->SelectCharacter(this->m_SelectedCharacter);
						strcpy(pSelectCharacter_Window->m_Name, g_pLocalUser->m_CharacterInfo.charactername);					
						TCHAR levelname[128];

						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
						{
							strcpy(pSelectCharacter_Window->m_Level, g_LevelName[ m_CurrentCharacterLevelIndex ] );
							sprintf(pSelectCharacter_Window->m_SubLevel, "%d %s",  m_CurrentCharacterSubLevel, m_String_LevelStar );
						}
						else
						{
							sprintf(levelname, "%s %d %s",  g_LevelName[ m_CurrentCharacterLevelIndex ],
															m_CurrentCharacterSubLevel, m_String_LevelStar );
							strcpy(pSelectCharacter_Window->m_Level, levelname);
						}

						strcpy(pSelectCharacter_Window->m_CharacterLocation, g_ZoneInfoTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid-1].zoneshortname );

						if( strlen( g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
															[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
															[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] ) > 0 )
						{
							strcpy(pSelectCharacter_Window->m_RollClass, 
								g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
														[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
														[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] );
						}
						else
						{
							strcpy(pSelectCharacter_Window->m_RollClass, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_MU) );
						}

						pSelectCharacter_Window->m_constitution	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution;
						pSelectCharacter_Window->m_zen			= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen;
						pSelectCharacter_Window->m_intelligence	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence;
						pSelectCharacter_Window->m_dexterity	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity;
						pSelectCharacter_Window->m_strength		= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength;
						pSelectCharacter_Window->m_DrawCharacterInfo = TRUE;
						pSelectCharacter_Window->m_CharacterGMLevel = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].Get_gm_level();

						m_PreviewCharacter[0].m_FrameResult = _XDEF_ACI_NONE;
						m_PreviewCharacter[1].m_FrameResult = _XDEF_ACI_NONE;
						m_PreviewCharacter[2].m_FrameResult = _XDEF_ACI_NONE;

#ifdef _XDEF_NEWANIMATIONLIST
						int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[0].modeldescript.item1, 
															  g_pLocalUser->m_AllCharacterInfo[0].modeldescript.item1Category );
						m_PreviewCharacter[0].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory], true );
																		
						itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[1].modeldescript.item1, g_pLocalUser->m_AllCharacterInfo[1].modeldescript.item1Category );
						int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[1].groupindex];

						m_PreviewCharacter[1].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][0], true );

						itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[2].modeldescript.item1, 
							g_pLocalUser->m_AllCharacterInfo[2].modeldescript.item1Category );
						m_PreviewCharacter[2].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory], true );
#else
						m_PreviewCharacter[0].SetCurMotion( 0, true );
						m_PreviewCharacter[1].SetCurMotion( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[1].groupindex]
							[g_pLocalUser->m_AllCharacterInfo[1].gender][0], true );
						m_PreviewCharacter[2].SetCurMotion( 0, true );
#endif
					}
				}
			}
			break;
		case _XDEF_CS_CHARACTER03BUTTON :
			{
				if( m_LockCharacterSelection ) break;
				
				if(g_pLocalUser->m_CharacterCount >= 3)
				{
					if( m_RotateDirection == 0.0f )
					{
						m_CharacterLocalRotateAngle[ 0 ] = 0.0f;
						m_CharacterLocalRotateAngle[ 1 ] = 0.0f;
						m_CharacterLocalRotateAngle[ 2 ] = 0.0f;

						if( this->m_SelectedCharacter == 0 )
						{
							m_RotateDirection = -1.0f;
						}
						else
						{
							m_RotateDirection = 1.0f;
						}

						m_TargetRotateAngle = 240.0f;

						if( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid != 
							g_pLocalUser->m_AllCharacterInfo[2].zoneuniqid )
						{
							pSelectCharacter_Window->m_MinimapAlphaLevel[2] = 0;
						}
						else
						{
							pSelectCharacter_Window->m_MinimapAlphaLevel[2] = 255;
						}

						pSelectCharacter_Window->m_SelectedCharacter = this->m_SelectedCharacter = 2;				
						
						pSelectCharacter_Window->m_Blank01Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 457) );
						pSelectCharacter_Window->m_Blank02Button.MoveWindow( gnWidth - (1024 - 897), gnHeight - (768 - 497) );
						pSelectCharacter_Window->m_Blank03Button.MoveWindow( gnWidth - (1024 - 857), gnHeight - (768 - 538) );
						
#if defined(_XTS_SERVERUNIFICATION) || defined(_XTS_NEWCHARACLIST) 
						if( g_pLocalUser->m_AllCharacterInfo[0].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
						if( g_pLocalUser->m_AllCharacterInfo[1].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
						if( g_pLocalUser->m_AllCharacterInfo[2].ucChangeNameFlag )
							pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFCF904 );
						else
							pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFFFFFF );

#else
						pSelectCharacter_Window->m_Blank01Button.SetButtonTextColor( 0xFFB1B1B3 );
						pSelectCharacter_Window->m_Blank02Button.SetButtonTextColor( 0xFFB1B1B3 );
						pSelectCharacter_Window->m_Blank03Button.SetButtonTextColor( 0xFFFFFFFF );
#endif

						pSelectCharacter_Window->RebuildCharacterGroupNameStatic( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex );

						_XLocalUser::GetUserLevel( g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].innerlevel, m_CurrentCharacterLevelIndex, m_CurrentCharacterSubLevel );
						g_pLocalUser->SelectCharacter(this->m_SelectedCharacter);
						strcpy(pSelectCharacter_Window->m_Name, g_pLocalUser->m_CharacterInfo.charactername);
						TCHAR levelname[128];

						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
						{
							strcpy(pSelectCharacter_Window->m_Level, g_LevelName[ m_CurrentCharacterLevelIndex ] );
							sprintf(pSelectCharacter_Window->m_SubLevel, "%d %s",  m_CurrentCharacterSubLevel, m_String_LevelStar );
						}
						else
						{
							sprintf(levelname, "%s %d %s",  g_LevelName[ m_CurrentCharacterLevelIndex ],
															m_CurrentCharacterSubLevel, m_String_LevelStar );
							strcpy(pSelectCharacter_Window->m_Level, levelname);
						}

						strcpy(pSelectCharacter_Window->m_CharacterLocation, g_ZoneInfoTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zoneuniqid-1].zoneshortname );

						if( strlen( g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
															[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
															[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] ) > 0 )
						{
							strcpy(pSelectCharacter_Window->m_RollClass, 
								g_CharacterRollNameTable[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].groupindex]
														[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClass]
														[g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].cClassGrade ] );
						}
						else
						{
							strcpy(pSelectCharacter_Window->m_RollClass, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_MU) );
						}

						pSelectCharacter_Window->m_constitution	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].constitution;
						pSelectCharacter_Window->m_zen			= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].zen;
						pSelectCharacter_Window->m_intelligence	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].intelligence;
						pSelectCharacter_Window->m_dexterity	= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].dexterity;
						pSelectCharacter_Window->m_strength		= g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].strength;
						pSelectCharacter_Window->m_DrawCharacterInfo = TRUE;
						pSelectCharacter_Window->m_CharacterGMLevel = g_pLocalUser->m_AllCharacterInfo[this->m_SelectedCharacter].Get_gm_level();

						m_PreviewCharacter[0].m_FrameResult = _XDEF_ACI_NONE;
						m_PreviewCharacter[1].m_FrameResult = _XDEF_ACI_NONE;
						m_PreviewCharacter[2].m_FrameResult = _XDEF_ACI_NONE;

#ifdef _XDEF_NEWANIMATIONLIST
						int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[0].modeldescript.item1, 
															  g_pLocalUser->m_AllCharacterInfo[0].modeldescript.item1Category );
						m_PreviewCharacter[0].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory], true );

						itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[1].modeldescript.item1, 
															  g_pLocalUser->m_AllCharacterInfo[1].modeldescript.item1Category );
						m_PreviewCharacter[1].SetCurMotion( g_PreviewCharacterStanceMotionIndex[itemcategory], true );

						//m_PreviewCharacter[2].SetCurMotion( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[2].groupindex]
						//																 [g_pLocalUser->m_AllCharacterInfo[2].gender][0], true );
						itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[2].modeldescript.item1, g_pLocalUser->m_AllCharacterInfo[2].modeldescript.item1Category );
						int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[2].groupindex];

						m_PreviewCharacter[2].SetCurMotion( g_PreviewCharacterMotionIndex[itemcategory][lutindex][0], true );
#else
						m_PreviewCharacter[0].SetCurMotion( 0, true );
						m_PreviewCharacter[1].SetCurMotion( 0, true );
						m_PreviewCharacter[2].SetCurMotion( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[2].groupindex]
																						 [g_pLocalUser->m_AllCharacterInfo[2].gender][0], true );
#endif
					}
				}
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

		default:
			break;
		}
	}
}

bool XProc_SelectCharacter::OnKeyboardPoll(_XInput *pInput)
{
	return true; 
}

bool XProc_SelectCharacter::OnKeyUp(WPARAM wparam, LPARAM lparam)
{
	return true;
}

bool XProc_SelectCharacter::OnKeyDown(WPARAM wparam, LPARAM lparam)
{ 	
	if( ::g_CurrentFocusedObject ) return true;
		
	U32 key = (U32)wparam;

#ifdef _XDEF_TESTRENDER
	if( key == VK_0 )
	{
		extern void _XTRCaptureScreen( void );
		_XTRCaptureScreen();
	}
#endif

	if( key == VK_RETURN )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CONNECTBUTTON, 0), (LPARAM)gHWnd);
	}
	else if( key == VK_INSERT )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CREATEBUTTON, 0), (LPARAM)gHWnd);
	}
	else if( key == VK_DELETE )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_DELETEBUTTON, 0), (LPARAM)gHWnd);
	}
	else if( key == VK_ESCAPE )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CANCELBUTTON, 0), (LPARAM)gHWnd);
	}
	else if( key == VK_LEFT )
	{
		if( !g_pTitleFlashObject->IsPlaying() && !m_CharacterDeleteMode && !m_CharacterSelectedMode && !m_FirstRotateAnimation )
		{
			if( m_RotateDirection == 0.0f )
			{
				if( m_SelectedCharacter == 0 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER02BUTTON, 0), (LPARAM)gHWnd);
				}
				else if( m_SelectedCharacter == 1 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER03BUTTON, 0), (LPARAM)gHWnd);
				}
				else if( m_SelectedCharacter == 2 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER01BUTTON, 0), (LPARAM)gHWnd);										
				}
			}
		}
	}
	else if( key == VK_RIGHT )
	{
		if( !g_pTitleFlashObject->IsPlaying() && !m_CharacterDeleteMode && !m_CharacterSelectedMode && !m_FirstRotateAnimation )
		{
			if( m_RotateDirection == 0.0f )
			{
				if( m_SelectedCharacter == 0 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER03BUTTON, 0), (LPARAM)gHWnd);
				}
				else if( m_SelectedCharacter == 1 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER01BUTTON, 0), (LPARAM)gHWnd);
				}
				else if( m_SelectedCharacter == 2 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER02BUTTON, 0), (LPARAM)gHWnd);										
				}
			}
		}		
	}
	else if( key == VK_1 )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER01BUTTON, 0), (LPARAM)gHWnd);
	}
	else if( key == VK_2 )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER02BUTTON, 0), (LPARAM)gHWnd);
	}
	else if( key == VK_3 )
	{
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER03BUTTON, 0), (LPARAM)gHWnd);
	}

	return true; 
}

bool XProc_SelectCharacter::OnMousePoll(MouseState *pState, ScrnPos *pPos)
{
	POINT mousept = { pPos->x, pPos->z };
	GetVectorFromMousePos( mousept, g_vPickRayDir, g_vPickRayOrig );
	g_vPickInfinityRayDir = g_vPickRayDir * 1000.0f;
	
	if( !g_pTitleFlashObject->IsPlaying() && !m_CharacterDeleteMode && !m_CharacterSelectedMode && !m_FirstRotateAnimation )
	{
		if( !::g_CurrentFocusedObject )
		{	
			float fmx = pState->AX*0.6;
			//float fmy = pState->AY*0.6;

			if( pState->bButton[0] )
			{
				if( m_RotateDirection == 0.0f )
				{
					for( int i = 0; i < g_pLocalUser->m_CharacterCount; i++ )
					{
						if( m_SelectedCharacter != i )
						{
							D3DXVECTOR3 pos;	
							pos.x = m_PreviewCharacter[i].m_Position._41;
							pos.y = m_PreviewCharacter[i].m_Position._42;
							pos.z = m_PreviewCharacter[i].m_Position._43;
							D3DXVECTOR3 pickedpoint;
							
							if( m_CharacterOBB.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, pos, pickedpoint ) )
							{
								if( i == 0 )
								{
									PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER01BUTTON, 0), (LPARAM)gHWnd);
								}
								else if( i == 1 )
								{
									PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER02BUTTON, 0), (LPARAM)gHWnd);
								}
								else if( i == 2 )
								{
									PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER03BUTTON, 0), (LPARAM)gHWnd);										
								}
							}
						}
					}
				}
			}
			else if( pState->bButton[1] )
			{	
				if( !m_CharacterDeleteMode )
				{
					BOOL enablerotate = TRUE;
					
					if(m_SelectedCharacter >= 0)
					{
#ifdef _XDEF_NEWANIMATIONLIST
					int itemcategory = _GetWeponCategory( g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1, 
						g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].modeldescript.item1Category );
					
					int lutindex = g_PreviewCharacterRefLUT[itemcategory][g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex];

					if( g_PreviewCharacterMotionIndex[itemcategory][lutindex][0] != g_PreviewCharacterMotionIndex[itemcategory][lutindex][1] )
					{
						if( m_PreviewCharacter[m_SelectedCharacter].GetCurMotion() == g_PreviewCharacterMotionIndex[itemcategory][lutindex][0] )
						{
							enablerotate = FALSE;
						}
					}
#else
					if( g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex]
						[g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].gender][0] != 
						g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex]
						[g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].gender][1] )
						
						
					{
						if( m_PreviewCharacter[m_SelectedCharacter].GetCurMotion() == g_PreviewCharacterStanceIndex[g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].groupindex]
							[g_pLocalUser->m_AllCharacterInfo[m_SelectedCharacter].gender][0] )
						{
							enablerotate = FALSE;
						}
					}
#endif
					}

					if( enablerotate )
					{
						m_CharacterLocalRotateAngle[ m_SelectedCharacter ] -= fmx;

						D3DXMATRIX	rotmat,charrotmat;
						D3DXVECTOR3 position;
						D3DXVECTOR3	characterpos = D3DXVECTOR3(0.0f, 0.0f, 0.45f);
						D3DXVECTOR3	rotcharacterpos;
						
						D3DXMatrixRotationY(&rotmat, _X_RAD( (-120.0f * m_SelectedCharacter) + m_CurrentRotateAngle ));
						D3DXMatrixRotationY(&charrotmat, _X_RAD(180.0f - 120.0f * m_SelectedCharacter + m_CurrentRotateAngle + m_CharacterLocalRotateAngle[m_SelectedCharacter] ));
						
						D3DXVec3TransformCoord( &rotcharacterpos, &characterpos, &rotmat );
						m_PreviewCharacter[m_SelectedCharacter].m_Position = charrotmat;
						m_PreviewCharacter[m_SelectedCharacter].m_Position._41 = rotcharacterpos.x;
						m_PreviewCharacter[m_SelectedCharacter].m_Position._42 = rotcharacterpos.y;
						m_PreviewCharacter[m_SelectedCharacter].m_Position._43 = rotcharacterpos.z;
					}
				}
			}		

	#ifdef _XDWDEBUG
			else if( pState->bButton[2] )
			{
				if( _CameraDragPos.y == 0 ) _CameraDragPos.y = pPos->z;			

				_CameraOffsetX = pPos->x - _CameraDragPos.x;
				_CameraOffsetY = pPos->z - _CameraDragPos.y;
				_CameraDragPos.x = pPos->x;
				_CameraDragPos.y = pPos->z;			
				
				//m_PreviewCharacter[m_SelectedCharacter].m_Position._41 -= _CameraOffsetX * 0.02f;
				m_PreviewCharacter[m_SelectedCharacter].m_Position._42 -= _CameraOffsetY * 0.02f;
			}
			else _CameraDragPos.y = 0;
	#endif
		}
	}

	return true;
}

void XProc_SelectCharacter::OnMouseWheel(short zDelta)
{
	if( !m_WindowManager.ProcessWheelMessage(zDelta) )
	{
		//FLOAT MoveDelta  = (FLOAT)zDelta*0.005F;
		
			//플래시 쪽 버그 일단 게임 접속 되도록
		if( !g_pTitleFlashObject->IsPlaying() && !m_CharacterDeleteMode && !m_CharacterSelectedMode && !m_FirstRotateAnimation )
		{
			if( zDelta < 0 )
			{
				if( m_RotateDirection == 0.0f )
				{
					if( m_SelectedCharacter == 0 )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER02BUTTON, 0), (LPARAM)gHWnd);
					}
					else if( m_SelectedCharacter == 1 )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER03BUTTON, 0), (LPARAM)gHWnd);
					}
					else if( m_SelectedCharacter == 2 )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER01BUTTON, 0), (LPARAM)gHWnd);										
					}
				}			
			}
			else if( zDelta > 0 )
			{
				if( m_RotateDirection == 0.0f )
				{
					if( m_SelectedCharacter == 0 )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER03BUTTON, 0), (LPARAM)gHWnd);
					}
					else if( m_SelectedCharacter == 1 )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER01BUTTON, 0), (LPARAM)gHWnd);
					}
					else if( m_SelectedCharacter == 2 )
					{
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER02BUTTON, 0), (LPARAM)gHWnd);										
					}
				}						
			}
		}

	/*
#ifdef _XDWDEBUG
		FLOAT MoveDelta  = (FLOAT)zDelta*0.005F;
#ifdef _XADMINISTRATORMODE
		if( gpInput->CheckKeyPress( DIK_LCONTROL ) ) MoveDelta *= _XDEF_RAPID_RATE;
#endif

		if( gpInput->CheckKeyPress( DIK_LCONTROL ) ) MoveDelta *= _XDEF_RAPID_RATE;
		g_LodTerrain.m_3PCamera.SetDistance( g_LodTerrain.m_3PCamera.GetDistance() - MoveDelta );
		g_LodTerrain.m_3PCamera.UpdateViewMatrix();		
#endif
		*/
	}
}

void XProc_SelectCharacter::OnMouseLBtnDblClick( WPARAM wparam )
{
	_XWindow_SelectCharacter* pSelectCharacter_Window = (_XWindow_SelectCharacter *)m_WindowManager.FindWindow(_XDEF_WTITLE_SELECTCHARACTER);
	
			//플래시 쪽 버그 일단 게임 접속 되도록
	if( !g_pTitleFlashObject->IsPlaying() && !m_CharacterDeleteMode && !m_CharacterSelectedMode && !m_FirstRotateAnimation )
	{
		if( m_RotateDirection == 0.0f )
		{
			if( pSelectCharacter_Window->m_Blank01Button.CheckMousePosition() )
			{
				if( g_pLocalUser->m_CharacterCount >= 1 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER01BUTTON, 0), (LPARAM)gHWnd);
					m_SelectedCharacter = 0;

					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CONNECTBUTTON, 0), (LPARAM)gHWnd);
					return;
				}
				else
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CREATEBUTTON, 0), (LPARAM)gHWnd);
					return;
				}
			}
			
			if( pSelectCharacter_Window->m_Blank02Button.CheckMousePosition() )
			{
				if( g_pLocalUser->m_CharacterCount >= 2 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER02BUTTON, 0), (LPARAM)gHWnd);
					m_SelectedCharacter = 1;

					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CONNECTBUTTON, 0), (LPARAM)gHWnd);
					return;
				}
				else
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CREATEBUTTON, 0), (LPARAM)gHWnd);
					return;
				}
			}
			
			if( pSelectCharacter_Window->m_Blank03Button.CheckMousePosition() )
			{
				if( g_pLocalUser->m_CharacterCount >= 3 )
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CHARACTER03BUTTON, 0), (LPARAM)gHWnd);
					m_SelectedCharacter = 2;
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CONNECTBUTTON, 0), (LPARAM)gHWnd);
					return;
				}
				else
				{
					PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CREATEBUTTON, 0), (LPARAM)gHWnd);
					return;
				}
			}	

			for( int i = 0; i < g_pLocalUser->m_CharacterCount; i++ )
			{
				D3DXVECTOR3 pos;	
				pos.x = m_PreviewCharacter[i].m_Position._41;
				pos.y = m_PreviewCharacter[i].m_Position._42;
				pos.z = m_PreviewCharacter[i].m_Position._43;
				D3DXVECTOR3 pickedpoint;
				
				if( m_CharacterOBB.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, pos, pickedpoint ) )
				{
					if( i == 0 )
					{
						if( g_pLocalUser->m_CharacterCount >= 1 )
						{
							if( m_SelectedCharacter == 0 )
							{
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CONNECTBUTTON, 0), (LPARAM)gHWnd);
								return;
							}
						}
					}
					else if( i == 1 )
					{
						if( g_pLocalUser->m_CharacterCount >= 2 )
						{
							if( m_SelectedCharacter == 1 )
							{
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CONNECTBUTTON, 0), (LPARAM)gHWnd);
								return;
							}
						}
					}
					else if( i == 2 )
					{
						if( g_pLocalUser->m_CharacterCount >= 3 )
						{
							if( m_SelectedCharacter == 2 )
							{	
								PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_CS_CONNECTBUTTON, 0), (LPARAM)gHWnd);
								return;
							}
						}
					}
				}
			}
		}
	}
}

bool XProc_SelectCharacter::OnMouseButton(_XMouseButton p_MB, bool bPushed)
{
	return true; 
}	

void XProc_SelectCharacter::OnUserMessage_MessengerSocket(  UINT message, WPARAM wparam, LPARAM lparam )
{
//	if (WSAGETASYNCERROR(lparam) != 0) 
//	{		
//		if( !g_NetworkKernel.m_bConnected )
//		{
//			g_NetworkKernel.ProcessMessengerNetworkError();			
//		}
//		return;
//	}
//	
//	switch (WSAGETSELECTEVENT(lparam)) 
//	{
//	case FD_CONNECT:
//		WSAAsyncSelect(g_NetworkKernel.m_hMessengerSocket, gHWnd, WM_USER_MESSENGERSOCKET, FD_READ | FD_CLOSE );
//		g_NetworkKernel.m_bMessengerServerConnected = TRUE;
//		_XDWINPRINT( "Connected Messenger server" );
//		
//		break;
//	case FD_READ :
//		g_NetworkKernel.ReadMessengerPacket();
//		break;
//	case FD_CLOSE :
//		g_NetworkKernel.ProcessMessengerNetworkError();	// 2004.04.24->oneway48 delete
//		_XDWINPRINT( "SERVER MESSAGE : Messenger Socket close message" );
//		break;
//	}

}

void XProc_SelectCharacter::OnUserMessage_Socket(  UINT message, WPARAM wparam, LPARAM lparam )
{
	if (WSAGETASYNCERROR(lparam) != 0) 
	{
		int errorcode = WSAGetLastError();

		if( errorcode == WSAEWOULDBLOCK )
		{
			_XLog( "WARNING : SC : TCP I/O blocking");
			Sleep( 50 );
		}
		else
		{
			if( errorcode != ERROR_IO_PENDING )
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
			else
			{
				_XLog( "WARNING : SC : TCP I/O pending" );
				Sleep( 50 );
			}
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

void XProc_SelectCharacter::OnMCINotify( UINT message, WPARAM wparam, LPARAM lparam )
{
	/*if( g_BgmPlayFlag )
	{		
		g_BgmObject.LoopPlay();
	}	*/
}

void XProc_SelectCharacter::OnWindowSizeChangeNotify( void )
{	
	g_MessageBox.MoveWindow((gnWidth >> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_WIDTH >>1), (gnHeight>> 1) - (_XDEF_MESSAGEDIALOGDLGPOS_HEIGHT>>1) - 80 );
}

void XProc_SelectCharacter::ConfirmDevice(void)
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

void XProc_SelectCharacter::DrawFadeInOut( D3DCOLOR color )
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

void XProc_SelectCharacter::GetVectorFromMousePos( POINT& pt, D3DXVECTOR3& raydir, D3DXVECTOR3& rayorg )
{		    
    // Compute the vector of the pick ray in screen space
    D3DXVECTOR3 v;
    v.x =  ( ( ( 2.0f * pt.x ) / gnWidth  ) - 1.0f ) / g_LodTerrain.m_3PCamera.mp_proj_matrix._11;
    v.y = -( ( ( 2.0f * pt.y ) / gnHeight ) - 1.0f ) / g_LodTerrain.m_3PCamera.mp_proj_matrix._22;
    v.z =  1.0f;
	
    // Get the inverse view matrix
    D3DXMATRIX m;
    D3DXMatrixInverse( &m, NULL, &g_LodTerrain.m_3PCamera.mp_view_matrix);
	
    // Transform the screen space pick ray into 3D space
    raydir.x  = v.x*m._11 + v.y*m._21 + v.z*m._31;
    raydir.y  = v.x*m._12 + v.y*m._22 + v.z*m._32;
    raydir.z  = v.x*m._13 + v.y*m._23 + v.z*m._33;
    rayorg.x  = m._41;
    rayorg.y  = m._42;
    rayorg.z  = m._43;
}

void XProc_SelectCharacter::ProcessBilling( void )
{
	return;
	if( g_LocalSystemTime > g_BillingRemainTimeCounter )
	{
		int elapsedtime = g_LocalSystemTime - g_BillingRemainTimeCounter;
		
		g_LocalUserBillingInformations.BillRemain -= elapsedtime;
		
		if( g_LocalUserBillingInformations.BillRemain < 0 )
			g_LocalUserBillingInformations.BillRemain = 0;
		
		g_BillingRemainTimeCounter = g_LocalSystemTime;
	}	
}


#ifdef _XDEF_TESTRENDER

extern BOOL SaveSurfaceToJPG( IDirect3DSurface9*& pSrcSurface, LPTSTR szFilename );

void _XTRCaptureScreen( void )
{
	D3DXVECTOR3 pos = (D3DXVECTOR3(0.0f,0.0f,0.0f));
	for( int xx = 0 ; xx < _XDEF_ITEMMODELTYPECOUNT_WEAPON; xx++ )
	{
		if( !g_ItemModel_Weapon[xx] )
			continue;
		
		g_ItemModel_Weapon[0][xx]->Render( pos, 0 );

	/*
	for( int xx = 0 ; xx < _XDEF_MODELCLASSCOUNT_MOB; xx++ )
	{
		if(!g_MOBMODEL[xx])
			continue;
		if( g_MOBMODEL[xx]->m_strFileName.length() <= 0 ||
			g_MOBBIPEDMODEL[xx].m_strBipedName.length() <= 0 )
			continue;

		g_TestRenderModelDescriptor.ReserveModelStack(1);
		g_TestRenderModelDescriptor.SetMeshModel( 0, g_MOBMODEL[xx] );
		g_TestRenderModelDescriptor.LinkBipedObject( &g_MOBBIPEDMODEL[xx] );
		g_TestRenderModelDescriptor.PlayAnimation( true );

		if( !g_ModelArchive.ReOpenPackage() ) 
			return ;
		
		if( !g_AniArchive.ReOpenPackage() ) 
		{
			g_ModelArchive.ClosePackage();
			return ;
		}

		if( !g_MOBBIPEDMODEL[xx].Load( g_ModelArchive.GetPackedFile(const_cast<char*>(g_MOBBIPEDMODEL[xx].m_strBipedName.c_str())), const_cast<char*>(g_MOBBIPEDMODEL[xx].m_strBipedName.c_str()) ) )
		{
			g_ModelArchive.ClosePackage();
			g_AniArchive.ClosePackage();
			return ;
		}
		if( !g_MOBBIPEDMODEL[xx].ReserveAnimationStack( g_MOBBIPEDMODEL[xx].m_svTempMotion.size() ) )
		{
			g_ModelArchive.ClosePackage();
			g_AniArchive.ClosePackage();
			return ;
		}

		int i = 0;
		svdef_TEMP_MOTION::iterator it;
		for (it = g_MOBBIPEDMODEL[xx].m_svTempMotion.begin(); it != g_MOBBIPEDMODEL[xx].m_svTempMotion.end(); ++ i, ++ it)
		{
			TEMP_MOTION* pTempBipedNode = (*it);
			if( !g_MOBBIPEDMODEL[xx].LoadKey( g_AniArchive.GetPackedFile( const_cast<char*>(pTempBipedNode->strKeyname.c_str()) ), const_cast<char*>(pTempBipedNode->strKeyname.c_str()) ) )
			{
				g_ModelArchive.ClosePackage();
				g_AniArchive.ClosePackage();
				return ;
			}
		}

		if (!g_MOBBIPEDMODEL[xx].m_strParseBipedName.empty())
		{
			int nodeindex = g_MOBBIPEDMODEL[xx].FindObject( const_cast<char*>(g_MOBBIPEDMODEL[xx].m_strParseBipedName.c_str()) );
			if( nodeindex < 0 )
			{
				_XFatalError( "Can't find linked biped node : %s %s", g_MonsterName[xx], g_MOBBIPEDMODEL[xx].m_strParseBipedName.c_str() );
				g_ModelArchive.ClosePackage();
				g_AniArchive.ClosePackage();
				return ;
			}
		}

		g_ModelArchive.ClosePackage();
		g_AniArchive.ClosePackage();
		*/

		((CEmperorOfDragonsApp*)gApp)->m_proc_selectcharacter.Draw();


		IDirect3DSurface9* pSrcSurface = NULL;
			
		if( SUCCEEDED( gpDev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pSrcSurface ) ) )
		{
			if( pSrcSurface )
			{												
				_XSetDefaultPath();

				if( _chdir( _T("./rendertest") ) == -1 )
				{
					_mkdir( _T("./rendertest") );

					if( _chdir( _T("./rendertest") ) == -1 )
					{
						_XSetDefaultPath();
					}
				}

				TCHAR filename[_MAX_PATH];
				sprintf( filename, _T("%d.jpg"), xx );
								
				_XLog( "Capture : Start backbuffer capture : save backbuffer" );

				if( !SaveSurfaceToJPG( pSrcSurface, filename ) )
				{
					_XDWINPRINT("WARNING : Failed screen save : %s", filename );
				}

				/*memset( filename, 0, sizeof(TCHAR) * _MAX_PATH );
				strftime( filename, 128, "%Y%B%d %H-%M-%S.bmp", today );
				D3DXSaveSurfaceToFile( filename, D3DXIFF_BMP, pSrcSurface, NULL, NULL );*/

				SAFE_RELEASE( pSrcSurface );
				
				_XLog( "Capture : End back buffer capture" );

				_XDWINPRINT("NOTICE : Screen captured : %s", filename );
			}
		}
		else
		{
			_XLog( "Capture : ERROR : GetBackBuffer" );
		}
	}

}
	
#endif
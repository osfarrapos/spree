// XWindow_MonsterStatus.cpp: implementation of the _XWindow_MonsterStatus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XWindow_State.h"
#include "XWindow_RequestParty.h"

#include "XWindow_TargetInfoSelect.h"

#include "XWindow_MonsterStatus.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

RECT g_MobTargetIconPos[_XTARGET_MAX] =
{
	{   0,   0,  22,  22 },		// 낭인 00
	{  22,   0,  44,  22 },		// 개방 01
	{  44,   0,  66,  22 },		// 비궁 02
	{  66,   0,  88,  22 },		// 소림 03	

	{   0,  22,  22,  44 },		// 녹림 04
	{  22,  22,  44,  44 },		// 무당 05
	{  44,  22,  66,  44 },		// 마교 06
	{  66,  22,  88,  44 },		// 세가 07

	{   0, 132,	 22, 154 },		// 진위장군 08
	{   0,   0,	  0,   0 },		// 임시 09
	{   0,   0,	  0,   0 },		// 임시 10

	{   0,  44,  22,  66 },		// 늑대 / 혈랑 11
	{   0,  44,  22,  66 },		// 호랑이 12
	{   0,  44,  22,  66 },		// 곰 13
	{   0,  44,  22,  66 },		// 표범 14
	{   0,  44,  22,  66 },		// 거미/뱀/독사 15
	{   0,  44,  22,  66 },		// 여우 16

	{  22,  44,  44,  66 },		// 음화단 17
	{  22,  44,  44,  66 },		// 혈교 18
	
	{  44,  44,  66,  66 },		// 고루 19
	{  66,  44,  88,  66 },		// 비영단 20

	{   0,  66,  22,  88 },		// 사망곡 21
	{  22,  66,  44,  88 },		// 염왕곡 22
	{  44,  66,  66,  88 },		// 남림야수족 23
	{  66,  66,  88,  88 },		// 금전방 24


	{   0,  88,  22, 110 },		// 백성 25
	{  22,  88,  44, 110 },		// 가축 26
	{  44,  88,  66, 110 },		// 수련더미 27
	{  66,  88,  88, 110 },		// 알수없음 28

	{   0, 110,  22, 132 },		// 암상인 29
	{  22, 110,  44, 132 },		// 무관관주 30
	{  44, 110,  66, 132 },		// 관병 31
	{  22,  44,  44,  66 },		// 무적몬스터 32
	
	{  180,  0,  202,  22 },	// PK 낭인 33
	{  180, 22,  202,  44 },	// PK 개방 34
	{  180, 44,  202,  66 },	// PK 비궁 35
	{  180, 66,  202,  88 },	// PK 소림 36
	{  180, 88,  202, 110 },	// PK 녹림 37
	{  180,110,  202, 132 },	// PK 무당 38
	{  180,132,  202, 154 },	// PK 마교 39
	{   0,   0,	  0,   0 },		// PK 임시 40
	{   0,   0,	  0,   0 },		// PK 임시 41
	{   0,   0,	  0,   0 },		// PK 임시 42
	{  180,154,  202, 176 },	// PK 알수 없음 43
	{   0,   0,	  0,   0 },		// 임시 44
	{   0, 132,	 22, 154 },		// 토기병 45
	{  22, 132,	 44, 154 },		//  46
	
	{  88,   0, 110,  22 },		// 47 북해빙궁
	{  66, 132,  88, 154 },		// 48 북해설인
	{   0,  44,  22,  66 },		// 49 북해동물
	{  88,  22, 110,  44 }		// 50 위치 추적자
	
};

void __stdcall _XCallback_CharInfo( POINT pos, SIZE size )
{
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_CHARACTERINFO), _XSC_INFORMATION );		// _T("인물정보")
}

void __stdcall _XCallback_CharInfoMenu( POINT pos, SIZE size )
{
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_CHARACTERMENU), _XSC_INFORMATION );		// _T("인물기능 메뉴")
}

void __stdcall _XCallback_TargetClan( POINT pos, SIZE size )
{
	_XWindow_MonsterStatus* pMonStatus = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MONSTERSTATUSWINDOW);
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, g_TargetFullName[pMonStatus->GetTargetGroupIndex()], _XSC_INFORMATION );	
}

void __stdcall _XCallback_HonorMenu( POINT pos, SIZE size )
{
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_2254), _XSC_INFORMATION );	
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MonsterStatus::_XWindow_MonsterStatus() : m_pExpandBorder(NULL), m_pCloseButton(NULL)
{
	m_TargetGroupIndex = -1;
	m_pLastCalculateTarget = NULL;

	m_Target = NULL;
	m_TargetType = _XPICKINGTARGET_NONE;
	m_StateCount = 0;
	m_PopUpButton[0] = NULL;
	m_PopUpButton[1] = NULL;
	
	for( int i = 0; i < 4; i++ )
		m_CharInfoPopUpButton[i] = NULL;

	m_ShowPopUp = FALSE;
	m_ShowTargetInfoPopUp = FALSE;	

	m_PrevDrawTime = 0;
	m_EffectFrame = 0;
	m_EffectOrderSub = FALSE;

	m_pExpandBossBorder[0] = NULL;
	m_pExpandBossBorder[1] = NULL; 

	m_pMonsterStatusButton[0] = NULL; 
	m_pMonsterStatusButton[1] = NULL; 
}

_XWindow_MonsterStatus::~_XWindow_MonsterStatus()
{

}

BOOL _XWindow_MonsterStatus::Initialize( void )
{
	m_borderstatic.SetClipRect( 0, 157, 180, 206 );

	int additionalindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		additionalindex = g_MainInterfaceTextureArchive.FindResource( "us_MI_namedmob01.tga" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		additionalindex = g_MainInterfaceTextureArchive.FindResource( "rs_MI_namedmob01.tga" );
	}
	else
	{
		additionalindex = g_MainInterfaceTextureArchive.FindResource( "MI_namedmob01.tga" );
	}
	
	if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		additionalindex = g_MainInterfaceTextureArchive.FindResource( "MI_namedmob02.tga" );

		m_pExpandBorder = new _XImageStatic;
		m_pExpandBorder->Create( 0, 0, 235, 64, &g_MainInterfaceTextureArchive, additionalindex );	
		m_pExpandBorder->SetClipRect( 3, 3, 238, 67 );
		InsertChildObject( m_pExpandBorder );

		m_pExpandBossBorder[0] = new _XImageStatic;
		m_pExpandBossBorder[0]->Create( 0, 0, 126, 66, &g_MainInterfaceTextureArchive, additionalindex );	
		m_pExpandBossBorder[0]->SetClipRect( 3, 77, 129, 143 );
		InsertChildObject( m_pExpandBossBorder[0] );

		m_pExpandBossBorder[1] = new _XImageStatic;
		m_pExpandBossBorder[1]->Create( 126, 0, 126+165, 66, &g_MainInterfaceTextureArchive, additionalindex);	
		m_pExpandBossBorder[1]->SetClipRect( 3, 149, 168, 215 );
		InsertChildObject( m_pExpandBossBorder[1] );

		additionalindex = g_MainInterfaceTextureArchive.FindResource( "MI_namedmob03.tga" );
		// 보스몹
		m_AdditionalImage[0].Create(0, 0, 145, 112, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[0].SetClipRect(0, 0, 145, 112);
		m_AdditionalImage[1].Create(0, 0, 145, 112, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[1].SetClipRect(0, 0, 145, 112);
		m_AdditionalImage[1].SetScale(-1, 1);

		// 네임드 (퀘스트)
		m_AdditionalImage[2].Create(0, 0, 90, 65, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[2].SetClipRect(10, 132, 100, 197);
		m_AdditionalImage[3].Create(0, 0, 45, 58, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[3].SetClipRect(110, 147, 154, 205);

		// 레이드
		m_AdditionalImage[4].Create(0, 0, 31, 72, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[4].SetClipRect(147, 16, 178, 88);
		m_AdditionalImage[5].Create(0, 0, 31, 72, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[5].SetClipRect(212, 16, 243, 88);
	}
	else
	{
		// 보스몹
		m_AdditionalImage[0].Create(0, 0, 136, 92, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[0].SetClipRect(0, 0, 142, 93);
		m_AdditionalImage[1].Create(0, 0, 136, 92, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[1].SetClipRect(0, 0, 142, 93);
		m_AdditionalImage[1].SetScale(-1, 1);

		// 네임드 (퀘스트)
		m_AdditionalImage[2].Create(0, 0, 88, 79, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[2].SetClipRect(8, 97, 96, 176);
		m_AdditionalImage[3].Create(0, 0, 47, 60, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[3].SetClipRect(175, 109, 222, 169);

		// 레이드
		m_AdditionalImage[4].Create(0, 0, 27, 62, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[4].SetClipRect(149, 16, 176, 78);
		m_AdditionalImage[5].Create(0, 0, 88, 79, &g_MainInterfaceTextureArchive, additionalindex);
		m_AdditionalImage[5].SetClipRect(183, 16, 210, 78);
	}

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 35, 22 }, { 136, 9 },	
		charinfointerface, -1, &g_MainInterfaceTextureArchive, 
	};
	
	m_pMonsterStaminaGauge = new _XStickGauge;
	m_pMonsterStaminaGauge->Create( gaugestruct );	
	m_pMonsterStaminaGauge->SetImageClipRect( _XGAUGEIMAGE_BORDER,  50,  123, 186,  132 );
	m_pMonsterStaminaGauge->SetFactorDraw(TRUE);
	m_pMonsterStaminaGauge->SetGaugeFactor( 0.0f );
	//m_pMonsterStaminaGauge->SetGaugeImageDrawType( _XGAUGEIMAGEDRAWTYPE_RESIZE );
	m_pMonsterStaminaGauge->SetGaugeImageScaleFactor(1.0f);
	m_pMonsterStaminaGauge->SetGaugeImageDrawType( _XGAUGEIMAGEDRAWTYPE_SCALE );
	m_pMonsterStaminaGauge->SetScyncDelayGaugeFactor();
	m_pMonsterStaminaGauge->SetViewDelayGauge( TRUE, 20 );
	m_pMonsterStaminaGauge->SetBlinking( TRUE, 10 );
	
	InsertChildObject( m_pMonsterStaminaGauge );

	/*
	_XSTICKGAUGE_STRUCTURE	levelgaugestruct = 
	{
		TRUE, { 35, 38 }, { 63, 5 },
		charinfointerface, -1, &g_MainInterfaceTextureArchive, 
	};

	m_pMonsterLevelGauge = new _XStickGauge;
	m_pMonsterLevelGauge->Create( levelgaugestruct );	
	m_pMonsterLevelGauge->SetImageClipRect( _XGAUGEIMAGE_BORDER,  183,  207, 246,  211 );
	m_pMonsterLevelGauge->SetFactorDraw(FALSE);
	m_pMonsterLevelGauge->SetGaugeFactor( 0.0f );
	m_pMonsterLevelGauge->SetGaugeImageDrawType( _XGAUGEIMAGEDRAWTYPE_RESIZE );

	InsertChildObject( m_pMonsterLevelGauge );	
	*/
	
	m_pMonsterLevelIndicator = new _XImageStatic;
	m_pMonsterLevelIndicator->Create( 35, 38, 35+9, 38+5, &g_MainInterfaceTextureArchive, charinfointerface );
	m_pMonsterLevelIndicator->SetClipRect( 183, 206, 192, 211 );
	InsertChildObject( m_pMonsterLevelIndicator );

	m_pTargetClanIcon = new _XImageStatic;
	m_pTargetClanIcon->Create( 7, 21, 26, 40,
							 &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "mi_npcname2.tga" ) );	
	m_pTargetClanIcon->SetClipRect( 0, 0, 0, 0 );
	m_pTargetClanIcon->SetCallBackFunction_ToolTip( _XCallback_TargetClan );
	InsertChildObject( m_pTargetClanIcon );	

	_XBTN_STRUCTURE functionbtnstruct = { TRUE, { 152, 36 }, { 10, 10 }, _XDEF_MONSTERSTATUS_FUNCTIONBUTTON1,
										  charinfointerface,charinfointerface,charinfointerface,										  
										 &g_MainInterfaceTextureArchive };

	m_pMonsterStatusButton[0] = new _XButton;	
	m_pMonsterStatusButton[0]->Create( functionbtnstruct );
	m_pMonsterStatusButton[0]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 151,236,161,245 );
	m_pMonsterStatusButton[0]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  161,236,171,245 );
	m_pMonsterStatusButton[0]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  171,236,181,245 );
	m_pMonsterStatusButton[0]->SetCallBackFunction_ToolTip( _XCallback_CharInfo );
	InsertChildObject( m_pMonsterStatusButton[0] );

	m_pMonsterStatusButton[1] = new _XButton;
	functionbtnstruct.commandid = _XDEF_MONSTERSTATUS_FUNCTIONBUTTON2;
	functionbtnstruct.position.x = 164;
	m_pMonsterStatusButton[1]->Create( functionbtnstruct );
	m_pMonsterStatusButton[1]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 151,246,161,255 );
	m_pMonsterStatusButton[1]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  161,246,171,255 );
	m_pMonsterStatusButton[1]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  171,246,181,255 );
	m_pMonsterStatusButton[1]->SetCallBackFunction_ToolTip( _XCallback_CharInfoMenu );
	InsertChildObject( m_pMonsterStatusButton[1] );

	// 팝업버튼들 //따로 뺀다,, Author : 양희왕 //Date :080704
	/*_XBTN_STRUCTURE charinfopopupbtnstruct = 
	{
		TRUE, 
		{4, 4},
		{88, 19},
		_XDEF_MONSTERSTATUS_INFOPOPUPBUTTON1,
		g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName),
		&g_MainInterfaceTextureArchive
	};

	for( int jj = 0; jj < 4; jj++ )
	{
		m_CharInfoPopUpButton[jj] = new _XButton;
		m_CharInfoPopUpButton[jj]->Create(charinfopopupbtnstruct);
		m_CharInfoPopUpButton[jj]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_CharInfoPopUpButton[jj]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_CharInfoPopUpButton[jj]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_CharInfoPopUpButton[jj]->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));		
		m_CharInfoPopUpButton[jj]->EnableWindow(TRUE);
		m_CharInfoPopUpButton[jj]->ShowWindow(FALSE);

		charinfopopupbtnstruct.position.y += 18;
		charinfopopupbtnstruct.commandid++;
	}

	m_CharInfoPopUpButton[0]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MONSTERSTATUS_2255);
	m_CharInfoPopUpButton[1]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_1589);
	m_CharInfoPopUpButton[2]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_1590);
	m_CharInfoPopUpButton[3]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_1591);

	for( jj = 0; jj < 4; jj++ )
		InsertChildObject(m_CharInfoPopUpButton[jj]);*/	

#ifdef _XTS_NEW_HONOR2
	pButton = new _XButton;
	functionbtnstruct.commandid = _XDEF_MONSTERSTATUS_FUNCTIONBUTTON3;
	functionbtnstruct.position.x = 140;
	pButton->Create( functionbtnstruct );
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 151,246,161,255 );
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  161,246,171,255 );
	pButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  171,246,181,255 );
	pButton->SetCallBackFunction_ToolTip( _XCallback_HonorMenu );
	InsertChildObject( pButton );

	// 팝업버튼들
	_XBTN_STRUCTURE popupbtnstruct = 
	{
		TRUE, 
		{4, 4},
		{88, 19},
		_XDEF_MONSTERSTATUS_POPUPBUTTON1,
		g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName),
		g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName),
		&g_MainInterfaceTextureArchive
	};

	for( int ii = 0; ii < 2; ii++ )
	{
		m_PopUpButton[ii] = new _XButton;
		m_PopUpButton[ii]->Create(popupbtnstruct);
		m_PopUpButton[ii]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		m_PopUpButton[ii]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		m_PopUpButton[ii]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		m_PopUpButton[ii]->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));		
		m_PopUpButton[ii]->EnableWindow(TRUE);
		m_PopUpButton[ii]->ShowWindow(FALSE);

		popupbtnstruct.position.y += 18;
		popupbtnstruct.commandid++;
	}

	m_PopUpButton[0]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MONSTERSTATUS_2256);
	m_PopUpButton[1]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MONSTERSTATUS_2257);

	InsertChildObject(m_PopUpButton[0]);
	InsertChildObject(m_PopUpButton[1]);

#endif

	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 162, 2 }, { 12, 12 }, _XDEF_MONSTERSTATUS_CLOSEBUTTON,
									   charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	m_pCloseButton = new _XButton;
	m_pCloseButton->Create( closebtnstruct );	
	
	m_pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_pCloseButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( m_pCloseButton );

	int mswinxpos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MSWINDOWXPOS, (gnWidth>>1)-90, TRUE );
	if( mswinxpos < 0 ) mswinxpos = 0;
	int mswinypos = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MSWINDOWYPOS, 22, TRUE );
	if( mswinypos < 0 ) mswinypos = 0;
	MoveWindow( mswinxpos, mswinypos );
	CheckOutside();

	m_pLastCalculateTarget = NULL;

	for(int i = 0 ; i < 32 ; i++)
	{
		m_StateImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, -1);
	}

	m_vecState.reserve(64);

	return TRUE;
}

void _XWindow_MonsterStatus::SetDefaultPosition( void )
{	
#ifdef _ACCLAIM_RUBICONADSYSTEM	
	if( g_ViewRubiconAD )
	{
		MoveWindow( (gnWidth>>1)-90, 22  + g_RubiconADWindowSize.cy );
	}
	else
	{
		MoveWindow( (gnWidth>>1)-90, 22 );
	}
#else
	MoveWindow( (gnWidth>>1)-90, 22 );
#endif

	CheckOutside();
}

void _XWindow_MonsterStatus::SaveWindowProperty( void )
{
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MSWINDOWXPOS,   GetWindowPos().x );
	g_RegManager._XRegWriteInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_MSWINDOWYPOS,   GetWindowPos().y );		
}

void _XWindow_MonsterStatus::DestroyWindow( void )
{	
	_vec_MSState::iterator iter_vec = m_vecState.begin();
	for( ; iter_vec != m_vecState.end() ; ++iter_vec)
	{
		_X_MS_State* pState = *iter_vec;
		if(pState)
		{
			if(pState->imageIcon)
			{
				SAFE_DELETE(pState->imageIcon);
			}
			SAFE_DELETE(pState);
		}
	}
	m_vecState.clear();

	_XWindow::DestroyWindow();
}

void _XWindow_MonsterStatus::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);		
	
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;	

	m_pMonsterLevelIndicator->ShowWindow( FALSE );

	if(m_TargetType == _XPICKINGTARGET_MOB)
	{
		DrawMonster();
	}
	else if(m_TargetType == _XPICKINGTARGET_PC)
	{
		DrawUser();
	}
	else if(m_TargetType == _XPICKINGTARGET_NPC)
	{
		DrawNPC();
	}
	else if(m_TargetType == _XPICKINGTARGET_SELF)
	{
		DrawSelf();
	}
	else if(m_TargetType == _XPICKINGTARGET_DUMMY)
	{
		DrawDummy();
	}
	else if(m_TargetType == _XPICKINGTARGET_SEAT)
	{
		DrawDummy();
	}
	else if(m_TargetType == _XPICKINGTARGET_FUNCTIONALOBJECT)
	{	
		DrawFunctionalObject();
	}

	DrawName();

	if(!m_Target)
	{
		m_pLastCalculateTarget = NULL;		
		m_pMonsterStaminaGauge->ShowWindow(FALSE);
	}

	// 상태 효과
	DrawStateImage();

	m_pTargetClanIcon->DrawToolTip( pfocusobject );	
}

BOOL _XWindow_MonsterStatus::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;		
	
	MouseState* mousestate = gpInput->GetMouseState();
#ifdef _XTS_NEW_HONOR2	

	if(mousestate->bButton[0] && !CheckMousePosition() )
	{
		if( !m_PopUpButton[0]->CheckMousePosition() && !m_PopUpButton[1]->CheckMousePosition() )
		{
			m_ShowPopUp = FALSE;
			m_PopUpButton[0]->ShowWindow(FALSE);
			m_PopUpButton[1]->ShowWindow(FALSE);
		}		
	}
#endif
	/*if( mousestate->bButton[0] && !CheckMousePosition() )
	{
		if( m_ShowTargetInfoPopUp )
		{
			if( !m_CharInfoPopUpButton[0]->CheckMousePosition() && !m_CharInfoPopUpButton[1]->CheckMousePosition() &&
				!m_CharInfoPopUpButton[2]->CheckMousePosition() && !m_CharInfoPopUpButton[3]->CheckMousePosition() )
			{
				m_ShowTargetInfoPopUp = FALSE;
				m_CharInfoPopUpButton[0]->ShowWindow(FALSE);
				m_CharInfoPopUpButton[1]->ShowWindow(FALSE);
				m_CharInfoPopUpButton[2]->ShowWindow(FALSE);
				m_CharInfoPopUpButton[3]->ShowWindow(FALSE);
			}
		}
	}*/
	return TRUE;
}

void _XWindow_MonsterStatus::MoveWindow(int X, int Y)
{//Author : 양희왕 //breif : 추가
	if( m_ShowTargetInfoPopUp )
	{
		_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);

		if(pInfoSelect_Window)
		{
			m_ShowTargetInfoPopUp = FALSE;
			pInfoSelect_Window->ShowWindow(FALSE);
		}
	}

	_XWindow::MoveWindow(X, Y);
}

void _XWindow_MonsterStatus::ShowWindow(BOOL show)
{
	if(show)
	{
		int	viewcase = 0;
		
		if(!g_pLocalUser->m_AttackTarget && g_pLocalUser->m_ViewTarget)			// view target만 있음
			viewcase = 1;
		else if(g_pLocalUser->m_AttackTarget && !g_pLocalUser->m_ViewTarget)	// attack target만 있음
			viewcase = 2;
		else if(g_pLocalUser->m_AttackTarget && g_pLocalUser->m_ViewTarget)
		{
			if(g_pLocalUser->m_AttackTarget == g_pLocalUser->m_ViewTarget)
				viewcase = 3;													// 둘다 있고 동일
			else 
				viewcase = 4;													// 둘다 있고 동일 하지 않음
		}
		
		switch(viewcase)
		{
		case 1 :
			{
				m_Target = g_pLocalUser->m_ViewTarget;
				m_TargetType = g_pLocalUser->m_ViewTargetType;
			}
			break;
		case 2 :
			{
				m_Target = g_pLocalUser->m_AttackTarget;
				m_TargetType = g_pLocalUser->m_AttackTargetObjectType;
			}
			break;
		case 3 :
			{
				m_Target = g_pLocalUser->m_ViewTarget;
				m_TargetType = g_pLocalUser->m_ViewTargetType;
			}
			break;
		case 4 :
			{
				m_Target = g_pLocalUser->m_ViewTarget;
				m_TargetType = g_pLocalUser->m_ViewTargetType;
			}
			break;
		}

		SetStateImage();
		if( m_TargetType == _XPICKINGTARGET_FUNCTIONALOBJECT )
		{
			if( !GetShowStatus() )
			{
				m_pMonsterStaminaGauge->SetGaugeFactor( -1.0f );
			}
		}

		//Author : 양희왕 //breif : 일본 몬스터 창 확장 -----------------------
		if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
		{
			switch(m_TargetType)
			{
			case _XPICKINGTARGET_MOB :
				{
#ifdef _XJAPANESE
					if( g_MonsterHiraganaName[((_XMob*)m_Target)->m_MobType] &&
						strlen(g_MonsterHiraganaName[((_XMob*)m_Target)->m_MobType]) > 0 )
					{
						TCHAR szHiraganaName[256];
						memset( szHiraganaName, 0, sizeof(szHiraganaName));
						sprintf(szHiraganaName, _T("%s"), g_MonsterHiraganaName[((_XMob*)m_Target)->m_MobType] );

						int textwidth = g_XBaseFont->GetWidth(szHiraganaName); //228

						m_borderstatic.ShowWindow(FALSE); //예전 바닥창,, 이건 꺼버리자..

						if( textwidth > 228 ) //228 작은 몬스터창 사이즈.. 228 이 넘으면 큰 창을 킨다
						{
							m_WindowSize.cx = 291;
							m_WindowSize.cy = 66;

							MoveWindow( (gnWidth>>1)-140, 22 );
							m_pTargetClanIcon->MoveWindow( m_WindowPosition.x + 7, m_WindowPosition.y + 36 );
							
							m_pMonsterStaminaGauge->ShowWindow(FALSE);
							m_pMonsterStaminaGauge->SetWindowSize( 240, 9 );
							m_pMonsterStaminaGauge->SetGaugeImageScaleFactor(240.f/136.f);
						
							m_pExpandBossBorder[0]->ShowWindow(TRUE);
							m_pExpandBossBorder[1]->ShowWindow(TRUE);

							m_pExpandBorder->ShowWindow(FALSE);
						}
						else
						{
							m_WindowSize.cx = 235;
							m_WindowSize.cy = 64;

							MoveWindow( (gnWidth>>1)-152, 22 );
							m_pTargetClanIcon->MoveWindow( m_WindowPosition.x + 7, m_WindowPosition.y + 35 );
							
							m_pMonsterStaminaGauge->ShowWindow(FALSE);
							m_pMonsterStaminaGauge->SetWindowSize( 185, 9 );
							m_pMonsterStaminaGauge->SetGaugeImageScaleFactor(185.f/136.f);

							m_pExpandBossBorder[0]->ShowWindow(FALSE);
							m_pExpandBossBorder[1]->ShowWindow(FALSE);

							m_pExpandBorder->ShowWindow(TRUE);
						}
					}
#endif
				}
				break;
			default :
				{
					m_WindowSize.cx = 180;
					m_WindowSize.cy = 49;

					MoveWindow( (gnWidth>>1)-90, 22 );
					m_pTargetClanIcon->MoveWindow( m_WindowPosition.x + 7, m_WindowPosition.y + 21 );
					
					m_pMonsterStaminaGauge->ShowWindow(FALSE);
					m_pMonsterStaminaGauge->SetWindowSize( 136, 9 );
					m_pMonsterStaminaGauge->SetGaugeImageScaleFactor(1.0f);
					m_pMonsterStaminaGauge->SetViewDelayGauge( FALSE, 20 );
					m_pMonsterStaminaGauge->SetViewDelayGauge( TRUE, 20 );
					m_pMonsterStaminaGauge->SetScyncDelayGaugeFactor();

					m_pExpandBorder->ShowWindow(FALSE);
					m_pExpandBossBorder[0]->ShowWindow(FALSE);
					m_pExpandBossBorder[1]->ShowWindow(FALSE);
					m_borderstatic.ShowWindow(TRUE);
				}
				break;
			}

			m_pMonsterStaminaGauge->MoveWindow( m_WindowPosition.x + 35, 
												m_WindowPosition.y + m_WindowSize.cy - 27 );

			m_pMonsterStatusButton[0]->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 28, 
												   m_WindowPosition.y + m_WindowSize.cy - 13 );
			m_pMonsterStatusButton[1]->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 16, 
												   m_WindowPosition.y + m_WindowSize.cy - 13 );

			m_pCloseButton->MoveWindow( m_WindowPosition.x + m_WindowSize.cx - 18,
										m_WindowPosition.y + 2 );
		}
		//Author : 양희왕 //breif : 일본 몬스터 창 확장 ---------긴데,, 함수로 빼도 되겠는걸--------------

	} // if(show)
	else
	{
		_XWindow_TargetInfoSelect* pInfoSelect_Window = (_XWindow_TargetInfoSelect*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TARGETINFOSELECTWINNDOW);
		m_ShowTargetInfoPopUp = FALSE;
		if(pInfoSelect_Window)
			pInfoSelect_Window->ShowWindow(FALSE);

		m_StateCount = 0;	
		m_pMonsterStaminaGauge->SetGaugeFactor( -1.0f );

		_XWindow_RequestParty* pRequestParty_Window = (_XWindow_RequestParty*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_REQUESTPARTY);
		if(pRequestParty_Window)
		{
			if(pRequestParty_Window->GetShowStatus())
				pRequestParty_Window->ShowWindow(FALSE);
		}
		
		m_pLastCalculateTarget = NULL;

		m_PrevDrawTime = 0;
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_MonsterStatus::SetStateImage(void)
{
	RECT rect;
	int imageindex = 0;

	_vec_MSState::iterator iter_vec = m_vecState.begin();
	for( ; iter_vec != m_vecState.end() ; ++iter_vec)
	{
		_X_MS_State* pState = *iter_vec;
		if(pState)
		{
			if(pState->imageIcon)
			{
				SAFE_DELETE(pState->imageIcon);
			}
			SAFE_DELETE(pState);
		}
	}
	m_vecState.clear();

	switch(m_TargetType)
	{
	case _XPICKINGTARGET_MOB :
		{
			_XMob* pMob = (_XMob*)m_Target;
			if(pMob)
			{
				list <_XMOB_STATE*>::iterator iter_state = pMob->m_StateList.begin();
				for( ; iter_state != pMob->m_StateList.end() ; ++iter_state)
				{
					// spell, effect
					_XMOB_STATE* pMobState = *iter_state;
					if(pMobState)
					{
						_X_MS_State* state = new _X_MS_State;
						state->nType = pMobState->type;
						state->nId = pMobState->spellID;
						
						switch(state->nType)
						{
						case 0 :	// spell
							{
								if(g_pQuickSlot_Window)
									g_pQuickSlot_Window->GetIconRect(state->nId, imageindex, rect, true, true);
							}
							break;
						case 1 :	// effect
							{
								_XWindow_State* pState_Window = (_XWindow_State*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_STATEWINDOW);
									if(pState_Window)
										pState_Window->GetIconRect(state->nId, imageindex, rect, true);
							}
							break;
						}
					
						state->imageIcon = new _XImageStatic;
						state->imageIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
						state->imageIcon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
						state->imageIcon->SetScale(0.67f, 0.67f);

						m_vecState.push_back(state);
					}
				}
				// condition
				for(int j = 1 ; j < 33 ; j++)
				{
					DWORD condition = pMob->m_ConditionList >> (j-1);
					DWORD result = condition & 0x00000001;
					
					if(result == 1)
					{
						_X_MS_State* state = new _X_MS_State;
						state->nType = 2;
						state->nId = j-1;

						GetIconRect(state->nId, imageindex, rect, true);

						state->imageIcon = new _XImageStatic;
						state->imageIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
						state->imageIcon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
						state->imageIcon->SetScale(0.67f, 0.67f);

						m_vecState.push_back(state);
					}
				}
				//resist - 처리 필요
				/*if(g_MobAttribTable[pMob->m_MobType].resistMinus > 0)
				{
					_X_MS_State state;
					state.nType = 3;
					state.nId = 1;

					if(g_pState_Window)
					{
						g_pState_Window->GetIconRect(state.nId+36, imageindex, rect, true);
					}

					state.imageIcon.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
					state.imageIcon.SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
					state.imageIcon.SetScale(0.67f, 0.67f);

					m_vecState.push_back(state);
				}
				if(g_MobAttribTable[pMob->m_MobType].resistPlus > 0)
				{
					_X_MS_State state;
					state.nType = 3;
					state.nId = 2;

					if(g_pState_Window)
					{
						g_pState_Window->GetIconRect(state.nId+36, imageindex, rect, true);
					}

					state.imageIcon.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
					state.imageIcon.SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
					state.imageIcon.SetScale(0.67f, 0.67f);

					m_vecState.push_back(state);
				}
				if(g_MobAttribTable[pMob->m_MobType].resistHon > 0)
				{
					_X_MS_State state;
					state.nType = 3;
					state.nId = 3;

					if(g_pState_Window)
					{
						g_pState_Window->GetIconRect(state.nId+36, imageindex, rect, true);
					}

					state.imageIcon.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
					state.imageIcon.SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
					state.imageIcon.SetScale(0.67f, 0.67f);

					m_vecState.push_back(state);
				}
				if(g_MobAttribTable[pMob->m_MobType].resistSin > 0)
				{
					_X_MS_State state;
					state.nType = 3;
					state.nId = 4;

					if(g_pState_Window)
					{
						g_pState_Window->GetIconRect(state.nId+36, imageindex, rect, true);
					}

					state.imageIcon.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
					state.imageIcon.SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
					state.imageIcon.SetScale(0.67f, 0.67f);

					m_vecState.push_back(state);
				}*/
			}
		}
		break;
	case _XPICKINGTARGET_PC :
		{
			BOOL bInsertExtraSkill = FALSE;

			_XUser* pUser = static_cast<_XUser*>(m_Target);
			if(pUser)
			{
#ifdef _XTS_SAFETYZONE
				map<int, _XCharacterState*>::iterator iter_userstate = pUser->m_StateList.m_mapCharacterStateList.begin();
				for( ; iter_userstate != pUser->m_StateList.m_mapCharacterStateList.end() ; iter_userstate++)
				{
					_XCharacterState* pState = iter_userstate->second;
					if(pState)
					{
						if(pState->m_FromType == en_from_extraskill && !bInsertExtraSkill)
						{
							_X_MS_State* state = new _X_MS_State;
							state->nType = 0;
							state->nId = pState->m_sSkillID;
							bInsertExtraSkill = TRUE;

							if(g_pQuickSlot_Window)
								g_pQuickSlot_Window->GetIconRect( pState->m_sSkillID, imageindex, rect, true, true);
		
							state->imageIcon = new _XImageStatic;
							state->imageIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
							state->imageIcon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
							state->imageIcon->SetScale(0.67f, 0.67f);

							m_vecState.push_back(state);

//							_XLog("state push : %d", state.nId);
						}
					}
				}
#endif
				// resist - 처리 필요
#ifdef _XDEF_4EFF_SYSTEM_070625_KUKURI
				if(pUser->m_CharacterInfo.m_b4Eff_ResMinus)
				{
					_X_MS_State* state = new _X_MS_State;
					state->nType = 3;
					state->nId = 1;

					if(g_pState_Window)
					{
						g_pState_Window->GetIconRect(state->nId+36, imageindex, rect, true);
					}

					state->imageIcon = new _XImageStatic;
					state->imageIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
					state->imageIcon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
					state->imageIcon->SetScale(0.67f, 0.67f);

					m_vecState.push_back(state);
				}
				if(pUser->m_CharacterInfo.m_b4Eff_ResPlus)
				{
					_X_MS_State* state = new _X_MS_State;
					state->nType = 3;
					state->nId = 2;

					if(g_pState_Window)
					{
						g_pState_Window->GetIconRect(state->nId+36, imageindex, rect, true);
					}

					state->imageIcon = new _XImageStatic;
					state->imageIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
					state->imageIcon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
					state->imageIcon->SetScale(0.67f, 0.67f);

					m_vecState.push_back(state);
				}
				if(pUser->m_CharacterInfo.m_b4Eff_ResHon)
				{
					_X_MS_State* state = new _X_MS_State;
					state->nType = 3;
					state->nId = 3;

					if(g_pState_Window)
					{
						g_pState_Window->GetIconRect(state->nId+36, imageindex, rect, true);
					}

					state->imageIcon = new _XImageStatic;
					state->imageIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
					state->imageIcon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
					state->imageIcon->SetScale(0.67f, 0.67f);

					m_vecState.push_back(state);
				}
				if(pUser->m_CharacterInfo.m_b4Eff_ResSin)
				{
					_X_MS_State* state = new _X_MS_State;
					state->nType = 3;
					state->nId = 4;

					if(g_pState_Window)
					{
						g_pState_Window->GetIconRect(state->nId+36, imageindex, rect, true);
					}

					state->imageIcon = new _XImageStatic;
					state->imageIcon->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, imageindex);
					state->imageIcon->SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
					state->imageIcon->SetScale(0.67f, 0.67f);

					m_vecState.push_back(state);
				}
#endif
			}
		}
		break;
	}
}

BOOL _XWindow_MonsterStatus::GetIconRect(short conditionid, int& resourceindex, RECT& rect, bool smallicon)
{
	int iconid = conditionid + 2;
	
	rect.left = ((iconid - 1) % 8) * 32;
	rect.right = ((iconid - 1) % 8 + 1) * 32;
	rect.top = ((iconid - 1) / 8) * 32;
	rect.bottom = ((iconid - 1) / 8 + 1) *32;
	
	if(smallicon)
	{
		rect.left  += 1;
		rect.top   += 1;
		rect.right -= 1;
		rect.bottom-= 1;
	}
		
	resourceindex = g_MainInterfaceTextureArchive.FindResource("Condition_icon_01.tga");
	return TRUE;
}

void _XWindow_MonsterStatus::SetPKMode(void)
{
	int pkuiresourcindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pk_01.tga");
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pk_01.tga");	
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pk_01.tga");	
	else
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("mi_pk_01.tga");	

	m_borderstatic.SetTexture(&g_MainInterfaceTextureArchive, pkuiresourcindex);
	m_borderstatic.SetClipRect(0, 42, 180, 91);
	
	int	normalresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_npcname2.tga");

	if(m_TargetType == _XPICKINGTARGET_PC)
		m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, pkuiresourcindex);
	else
		m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, normalresourceindex);
}

void _XWindow_MonsterStatus::ResetPKMode(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName);
	
	m_borderstatic.SetTexture(&g_MainInterfaceTextureArchive, resourceindex);
	m_borderstatic.SetClipRect(0, 157, 180, 206);

	resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_npcname2.tga");

	m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, resourceindex);
}

void _XWindow_MonsterStatus::SetTargetGroupIndexNTexture(_XItem* target, _XPICKINGTARGETTYPE targettype)
{
	int pkuiresourcindex = -1;
	
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pk_01.tga");
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pk_01.tga");	
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pk_01.tga");	
	else
		pkuiresourcindex = g_MainInterfaceTextureArchive.FindResource("mi_pk_01.tga");	

	int	normalresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_npcname2.tga");

	switch(targettype)
	{
	case _XPICKINGTARGET_PC :
		{
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END)
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, pkuiresourcindex);
				if( ((_XUser*)m_Target)->m_bFaceMaskMode )
					m_TargetGroupIndex = 43;
				else
					m_TargetGroupIndex = ((_XUser*)m_Target)->m_CharacterInfo.groupindex + 33;
			}
			else
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, normalresourceindex);
				if( ((_XUser*)m_Target)->m_bFaceMaskMode )
					m_TargetGroupIndex = 28;
				else
					m_TargetGroupIndex = ((_XUser*)m_Target)->m_CharacterInfo.groupindex;
			}

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI	// 비무 중에는 클랜 아이콘 보여주지 않음

	#ifdef _XDEF_MATCH_EVENT_PREVENT_ABUSE_080718_KUKURI
			if(((_XUser*)m_Target)->GetFreeMatchState())	// 내 상태와 관련없이 대회 중인 유저는 모두 안보이도록.
			{
					m_TargetGroupIndex = 28;
			}
	#else
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_hideme))
			{
				if(((_XUser*)m_Target)->GetEnterMatchEvent())
					m_TargetGroupIndex = 28;
			}
	#endif
#endif
		}
		break;
	case _XPICKINGTARGET_MOB :
		{
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END)
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, normalresourceindex);
			}

			m_TargetGroupIndex = g_MobAttribTable[((_XMob*)m_Target)->m_MobType].party;

			if(m_TargetGroupIndex == _XTARGET_MAX)	// Exception proc
				m_TargetGroupIndex = 8;		// 나중에 수정 원호씨.....

			// 수상한 필터 처리 --------------------------------------------------------------=
			if(g_MonsterName[((_XMob*)m_Target)->m_MobType])
			{
				if( (_strnicmp(g_MonsterName[((_XMob*)m_Target)->m_MobType], _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_SUSPICIOUSNESS), 6 )==0) )	// "수상한"
				{
					m_pLastCalculateTarget = NULL;
					m_TargetGroupIndex = 28;
				}
			}
		}
		break;
	case _XPICKINGTARGET_NPC :
		{
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END)
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, normalresourceindex);
			}

		//	if( ((_XNPCObject*)m_Target)->m_NPCClass == 0 )
		//	{
			int npcjobclass = ((_XNPCObject*)m_Target)->m_NPCJobClass;
			switch( npcjobclass ) {
			case _XNPC_JOBCLASS_GAEBANGPEOPLE :// 개방
				{
					m_TargetGroupIndex = 1;
				}
				break;
			case _XNPC_JOBCLASS_SORIMPEOPLE :// 소림
				{
					m_TargetGroupIndex = 3;
				}
				break;
			case _XNPC_JOBCLASS_NOCKRIMPEOPLE :// 녹림
				{
					m_TargetGroupIndex = 4;
				}
				break;
			case _XNPC_JOBCLASS_BEEGOONGPEOPLE :// 비궁
				{
					m_TargetGroupIndex = 2;
				}
				break;
			case _XNPC_JOBCLASS_MOODANGPEOPLE :// 무당
				{
					m_TargetGroupIndex = 5;
				}
				break;
			case _XNPC_JOBCLASS_MAKYOPEOPLE :// 마교
				{
					m_TargetGroupIndex = 6;
				}
				break;
			case _XNPC_JOBCLASS_BLACKMARKET :// 암상인
				{
					m_TargetGroupIndex = 29;
				}
				break;
			case _XNPC_JOBCLASS_MILITARYOFFICER :// 무관관주
				{
					m_TargetGroupIndex = 30;
				}
				break;
			case _XNPC_JOBCLASS_TRACER :
				{
					m_TargetGroupIndex = 50;
				}
				break;
			default:// 나머지 백성
				{
					m_TargetGroupIndex = 25;
				}
				break;
			}				
		//	}
		//	else if( ((_XNPCObject*)m_Target)->m_NPCClass == 1 )
		//	{
		//		m_TargetGroupIndex = ((_XNPCObject*)m_Target)->m_NPCClanClass;
		//	}
		}
		break;
	case _XPICKINGTARGET_ITEM :
		{
		}
		break;
	case _XPICKINGTARGET_DUMMY :
		{
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END)
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, normalresourceindex);
			}

			m_TargetGroupIndex = 27;
		}
		break;
	case _XPICKINGTARGET_SEAT :
		{
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END)
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, normalresourceindex);
			}
			
			m_TargetGroupIndex = 27;
		}
		break;
	case _XPICKINGTARGET_SELF :
		{
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END)
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, pkuiresourcindex);
				m_TargetGroupIndex = g_pLocalUser->m_CharacterInfo.groupindex + 33;
			}
			else
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, normalresourceindex);
				m_TargetGroupIndex = g_pLocalUser->m_CharacterInfo.groupindex;
			}
		}
		break;
	case _XPICKINGTARGET_FUNCTIONALOBJECT :
		{
			if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END)
			{
				m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, normalresourceindex);
			}
			
			m_TargetGroupIndex = 28;
		}
		break;
	}
}

void _XWindow_MonsterStatus::SetTargetClanIconFromModeChange(_XItem* target, _XPICKINGTARGETTYPE targettype)
{
	int groupindex;
	bool pkflag = (g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END) ? true : false ;

	switch(targettype)
	{
	case _XPICKINGTARGET_PC :
		{
			if(pkflag)
			{
				if( ((_XUser*)m_Target)->m_bFaceMaskMode )
					groupindex = 43;
				else
					groupindex = ((_XUser*)m_Target)->m_CharacterInfo.groupindex+33;
			}
			else
			{
				if( ((_XUser*)m_Target)->m_bFaceMaskMode )
					groupindex = 28;
				else
					groupindex = ((_XUser*)m_Target)->m_CharacterInfo.groupindex;				
			}

#ifdef _XDEF_MATCH_EVENT_071025_KUKURI

	#ifdef _XDEF_MATCH_EVENT_PREVENT_ABUSE_080718_KUKURI
			if(((_XUser*)m_Target)->GetFreeMatchState())	// 내 상태와 관련없이 대회 중인 유저는 모두 안보이도록.
			{
					groupindex = 43;
			}
	#else
			if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_hideme))
			{
				if(((_XUser*)m_Target)->GetEnterMatchEvent())
					groupindex = 43;
			}
	#endif
#endif
		}
		break;
	case _XPICKINGTARGET_MOB :
		{
		}
		break;
	case _XPICKINGTARGET_NPC :
		{
		}
		break;
	case _XPICKINGTARGET_ITEM :
		{
		}
		break;
	case _XPICKINGTARGET_DUMMY :
		{
		}
		break;
	case _XPICKINGTARGET_SEAT :
		{
		}
		break;
	case _XPICKINGTARGET_SELF :
		{
			if(pkflag)
				groupindex = g_pLocalUser->m_CharacterInfo.groupindex+33;
			else
				groupindex = g_pLocalUser->m_CharacterInfo.groupindex;
		}
		break;
	case _XPICKINGTARGET_FUNCTIONALOBJECT :
		{
		}
		break;
	}

	if( m_TargetGroupIndex != groupindex )
	{
		if(m_TargetType == _XPICKINGTARGET_PC || m_TargetType == _XPICKINGTARGET_SELF)
		{
			int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_npcname2.tga");
			if(pkflag)
			{
				if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
					resourceindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pk_01.tga");
				else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
					resourceindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pk_01.tga");	
				else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					resourceindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pk_01.tga");	
				else
					resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_pk_01.tga");	
			}
			
			m_TargetGroupIndex = groupindex;
			m_pTargetClanIcon->SetTexture(&g_MainInterfaceTextureArchive, resourceindex);
			m_pTargetClanIcon->SetClipRect( g_MobTargetIconPos[m_TargetGroupIndex].left, g_MobTargetIconPos[m_TargetGroupIndex].top, 
				g_MobTargetIconPos[m_TargetGroupIndex].right, g_MobTargetIconPos[m_TargetGroupIndex].bottom );
		}	
	}	
}

void _XWindow_MonsterStatus::DrawTargetClanIcon(void)
{
	if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_PK_READY && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_PK_END)
	{
		if(m_PrevDrawTime == 0)
			m_PrevDrawTime = g_LocalSystemTime;
		
		DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
		
		if(!m_EffectOrderSub)
			m_EffectFrame += ((FLOAT)elapsedtime / 250.0f);
		else 
			m_EffectFrame -= ((FLOAT)elapsedtime / 250.0f);
		
		if(m_EffectFrame > 3.0f)
		{
			m_EffectOrderSub = TRUE;
			m_EffectFrame = 2.0f;
		}
		else if(m_EffectFrame <= 0.0f)
		{
			m_EffectOrderSub = FALSE;
			m_EffectFrame = 1.0f;
		}
		
		int selectedframe = (int)m_EffectFrame;
		
		if(selectedframe > 2)
			selectedframe = 2;
		else if(selectedframe < 0)
			selectedframe = 0;
		
		m_pTargetClanIcon->SetClipRect( g_MobTargetIconPos[m_TargetGroupIndex].left+selectedframe*22, g_MobTargetIconPos[m_TargetGroupIndex].top, 
			g_MobTargetIconPos[m_TargetGroupIndex].right+selectedframe*22, g_MobTargetIconPos[m_TargetGroupIndex].bottom );
		
		m_PrevDrawTime = g_LocalSystemTime;
	}
	else
	{
		m_pTargetClanIcon->SetClipRect( g_MobTargetIconPos[m_TargetGroupIndex].left, g_MobTargetIconPos[m_TargetGroupIndex].top, 
			g_MobTargetIconPos[m_TargetGroupIndex].right, g_MobTargetIconPos[m_TargetGroupIndex].bottom );
	}
}

void _XWindow_MonsterStatus::DrawMonster()
{
	m_pMonsterStaminaGauge->ShowWindow(TRUE);
	m_pMonsterLevelIndicator->ShowWindow(TRUE);
	
	if(m_pLastCalculateTarget != m_Target)
	{
		// 처음 클릭
		m_pLastCalculateTarget = m_Target;
		
		DrawLevel(g_MobAttribTable[((_XMob*)m_Target)->m_MobType].level);

		m_pMonsterStaminaGauge->SetViewDelayGauge(FALSE, 20);
		m_pMonsterStaminaGauge->SetGaugeFactor(((_XMob*)m_Target)->m_DisplayHealthPercentage);
		m_pMonsterStaminaGauge->SetViewDelayGauge(TRUE, 20);
		m_pMonsterStaminaGauge->SetScyncDelayGaugeFactor();

		SetTargetGroupIndexNTexture(m_Target, m_TargetType);

		m_pTargetClanIcon->SetClipRect(g_MobTargetIconPos[m_TargetGroupIndex].left, g_MobTargetIconPos[m_TargetGroupIndex].top, 
				g_MobTargetIconPos[m_TargetGroupIndex].right, g_MobTargetIconPos[m_TargetGroupIndex].bottom);
	}
	else
	{
		DrawLevel(g_MobAttribTable[((_XMob*)m_Target)->m_MobType].level);
		m_pMonsterStaminaGauge->SetGaugeFactor(((_XMob*)m_Target)->m_DisplayHealthPercentage);
	}

	//g_XSmallFont->SetColor(_XSC_DEFAULT);
	//g_XSmallFont->Puts(m_WindowPosition.x+111, m_WindowPosition.y + m_WindowSize.cy - 28, "%");
	g_XSmallFont->Flush();
	
#ifdef _XDEF_MODIFY_UI_MONSTER_070710_KUKURI
	BOOL bDrawLevel = TRUE;

	// 퀘스트, 레이드, 보스 몹
	if(g_MobAttribTable[((_XMob*)m_Target)->m_MobType].rank == 1 || g_MobAttribTable[((_XMob*)m_Target)->m_MobType].rank == 2 ||
		g_MobAttribTable[((_XMob*)m_Target)->m_MobType].rank == 3 || g_MobAttribTable[((_XMob*)m_Target)->m_MobType].rank == 4 ||
		g_MobAttribTable[((_XMob*)m_Target)->m_MobType].rank == 8)
	{
		bDrawLevel = FALSE;
	}

	// 경비, 무림 명숙
	if(g_MobAttribTable[((_XMob*)m_Target)->m_MobType].scripttype >= 91 &&  g_MobAttribTable[((_XMob*)m_Target)->m_MobType].scripttype <= 95)
	{
		bDrawLevel = FALSE;
	}

	// 자신 레벨과 비교 - 07년 9월 27일 국내지원 요청 : 모든 몬스터 
//	int difflevel = g_pLocalUser->m_CharacterInfo.innerlevel - g_MobAttribTable[((_XMob*)m_Target)->m_MobType].level;
//	if(difflevel < -12 || difflevel > 12)
//	{
//		bDrawLevel = FALSE;
//	}

	if(bDrawLevel)
	{
		g_XSmallFont->SetColor(0xFFFFFFFF);
		g_XSmallFont->Print(m_WindowPosition.x+102, m_WindowPosition.y + m_WindowSize.cy - 13, 1.0f, "(Lv. %d)", g_MobAttribTable[((_XMob*)m_Target)->m_MobType].level);
		g_XSmallFont->Flush();
	}
#endif

	// 보스몹 처리 //Author : 양희왕 //breif : 사이즈 에 따라 조절되게 수정 08/01/18
	switch(g_MobAttribTable[((_XMob*)m_Target)->m_MobType].rank)
	{
	case 1 :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
			{
				m_AdditionalImage[2].Draw(m_WindowPosition.x-19, m_WindowPosition.y-18);
				m_AdditionalImage[3].Draw(m_WindowPosition.x + m_WindowSize.cx - 32, m_WindowPosition.y-7);
			}
			else
			{
				// named - quest
				m_AdditionalImage[2].Draw(m_WindowPosition.x-19, m_WindowPosition.y-18);
				m_AdditionalImage[3].Draw(m_WindowPosition.x + m_WindowSize.cx - 32, m_WindowPosition.y-6);
			}
		}
		break;
	case 2 : 
		{
			// 레이드
			m_AdditionalImage[4].Draw(m_WindowPosition.x-26, m_WindowPosition.y);
			m_AdditionalImage[5].Draw(m_WindowPosition.x + m_WindowSize.cx - 1, m_WindowPosition.y);
		}
		break;
	case 3 :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
			{
				m_AdditionalImage[0].Draw(m_WindowPosition.x + m_WindowSize.cx - 96, m_WindowPosition.y - 33);
				m_AdditionalImage[1].Draw(m_WindowPosition.x + 96, m_WindowPosition.y- 33);
			}
			else
			{
				// 호법 - 보스와 같이 처리
				m_AdditionalImage[0].Draw(m_WindowPosition.x+84, m_WindowPosition.y-33);
				m_AdditionalImage[1].Draw(m_WindowPosition.x+96, m_WindowPosition.y-33);
			}
		}
		break;
	case 8 :
	case 4 :
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
			{
				m_AdditionalImage[0].Draw(m_WindowPosition.x + m_WindowSize.cx - 96, m_WindowPosition.y - 33);
				m_AdditionalImage[1].Draw(m_WindowPosition.x + 96, m_WindowPosition.y- 33);
			}
			else
			{
				// 보스
				m_AdditionalImage[0].Draw(m_WindowPosition.x+84, m_WindowPosition.y-33);
				m_AdditionalImage[1].Draw(m_WindowPosition.x+96, m_WindowPosition.y-33);
			}
		}
		break;
	}
}

void _XWindow_MonsterStatus::DrawUser()
{
	m_pMonsterStaminaGauge->ShowWindow(FALSE);

#ifdef _XTS_VIEWPKTARGETLEVEL
	if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
	{
		m_pMonsterLevelIndicator->ShowWindow(TRUE);
		DrawLevel(((_XUser*)m_Target)->m_CharacterInfo.Get_innerlevel());
	}
#endif

	if( m_pLastCalculateTarget != m_Target )
	{
		m_pLastCalculateTarget = m_Target;

		if((g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_FIGHT) && (((_XUser*)m_Target)->m_Uniqueid == g_pLocalUser->m_MatchTargetID))
		{
			m_pMonsterStaminaGauge->SetGaugeFactor(g_pLocalUser->m_myMatchMemberInfo.ucVitalRate);
		}
		else
		{
			m_pMonsterStaminaGauge->ShowWindow(FALSE);
			
			int textwidth = g_XSmallFont->GetWidth( "???" );		
			
			g_XSmallFont->SetColor( 0xFF000000 );
			g_XSmallFont->Puts( m_WindowPosition.x + 82 - (textwidth >> 1), m_WindowPosition.y + 4, "???" );
			g_XSmallFont->SetColor( 0xFFFFFFFF );
			g_XSmallFont->Puts( m_WindowPosition.x + 81 - (textwidth >> 1), m_WindowPosition.y + 3, "???" );
			g_XSmallFont->Flush();
		}

		SetTargetGroupIndexNTexture(m_Target, m_TargetType);
		m_pTargetClanIcon->SetClipRect( g_MobTargetIconPos[m_TargetGroupIndex].left, g_MobTargetIconPos[m_TargetGroupIndex].top, 
										g_MobTargetIconPos[m_TargetGroupIndex].right, g_MobTargetIconPos[m_TargetGroupIndex].bottom );
	}

	DrawTargetClanIcon();

	SetTargetClanIconFromModeChange(m_Target, m_TargetType);

#ifdef _XTS_NEW_HONOR2
	if( m_ShowPopUp )
	{
		_XDrawRectAngle( m_WindowPosition.x+85, m_WindowPosition.y+51, m_WindowPosition.x+88+87+3, m_WindowPosition.y+54+36+2, 0.0f,0xFF000000 );
		_XDrawRectAngle( m_WindowPosition.x+86, m_WindowPosition.y+52, m_WindowPosition.x+88+87+2, m_WindowPosition.y+54+36+1, 0.0f,0xFFD2D2D2 );
		_XDrawRectAngle( m_WindowPosition.x+87, m_WindowPosition.y+53, m_WindowPosition.x+88+87+1, m_WindowPosition.y+54+36, 0.0f,0xFF000000 );
	}
	
	int honorstep = g_Userlist.CalcHonorStep(((_XUser*)m_Target)->m_CharacterInfo.honor );
	if( honorstep != -1 )
	{
		sprintf( cName, "%s %s", g_HonorName[honorstep], ((_XUser*)m_Target)->m_CharacterInfo.charactername );
		g_XBaseFont->SetColor(g_HonorColor[honorstep]);			
	}		
#endif

	/*if( m_ShowTargetInfoPopUp )
	{
		_XDrawRectAngle( m_WindowPosition.x+85, m_WindowPosition.y+51, m_WindowPosition.x+88+87+3, m_WindowPosition.y+54+72+2, 0.0f,0xFF000000 );
		_XDrawRectAngle( m_WindowPosition.x+86, m_WindowPosition.y+52, m_WindowPosition.x+88+87+2, m_WindowPosition.y+54+72+1, 0.0f,0xFFD2D2D2 );
		_XDrawRectAngle( m_WindowPosition.x+87, m_WindowPosition.y+53, m_WindowPosition.x+88+87+1, m_WindowPosition.y+54+72, 0.0f,0xFF000000 );
	}*/
}

void _XWindow_MonsterStatus::DrawNPC()
{
	m_pMonsterStaminaGauge->ShowWindow(TRUE);

	if( m_pLastCalculateTarget != m_Target )
	{
		m_pLastCalculateTarget = m_Target;

		m_pMonsterStaminaGauge->ShowWindow(TRUE);
		m_pMonsterStaminaGauge->SetViewDelayGauge( FALSE, 20 );
		m_pMonsterStaminaGauge->SetGaugeFactor( 100.0f );
		m_pMonsterStaminaGauge->SetViewDelayGauge( TRUE, 20 );
		m_pMonsterStaminaGauge->SetScyncDelayGaugeFactor();
		
		SetTargetGroupIndexNTexture(m_Target, m_TargetType);

		m_pTargetClanIcon->SetClipRect( g_MobTargetIconPos[m_TargetGroupIndex].left, g_MobTargetIconPos[m_TargetGroupIndex].top, 
										g_MobTargetIconPos[m_TargetGroupIndex].right, g_MobTargetIconPos[m_TargetGroupIndex].bottom );
	}
	else
	{
		m_pMonsterStaminaGauge->SetGaugeFactor( 100.0f );
	}
}

void _XWindow_MonsterStatus::DrawSelf()
{
	m_pMonsterStaminaGauge->ShowWindow(TRUE);

	if( m_pLastCalculateTarget != m_Target )
	{
		m_pLastCalculateTarget = m_Target;

		int healthPercentage = (int)(((FLOAT)g_pLocalUser->m_CharacterInfo.Get_current_lifepower() / (FLOAT)g_pLocalUser->m_MaxLifePower.Get_result())*100.f);

		m_pMonsterStaminaGauge->ShowWindow(TRUE);
		m_pMonsterStaminaGauge->SetViewDelayGauge( FALSE, 20 );
		m_pMonsterStaminaGauge->SetGaugeFactor(healthPercentage);
		m_pMonsterStaminaGauge->SetViewDelayGauge( TRUE, 20 );
		m_pMonsterStaminaGauge->SetScyncDelayGaugeFactor();

		SetTargetGroupIndexNTexture(m_Target, m_TargetType);
		m_pTargetClanIcon->SetClipRect( g_MobTargetIconPos[m_TargetGroupIndex].left, g_MobTargetIconPos[m_TargetGroupIndex].top, 
										g_MobTargetIconPos[m_TargetGroupIndex].right, g_MobTargetIconPos[m_TargetGroupIndex].bottom );
	}
	else
	{
		int healthPercentage = (int)(((FLOAT)g_pLocalUser->m_CharacterInfo.Get_current_lifepower() / (FLOAT)g_pLocalUser->m_MaxLifePower.Get_result())*100.f);
		m_pMonsterStaminaGauge->SetGaugeFactor(healthPercentage);
	}

	DrawTargetClanIcon();

	SetTargetClanIconFromModeChange(m_Target, m_TargetType);

	g_XSmallFont->SetColor(_XSC_DEFAULT);
	g_XSmallFont->Puts(m_WindowPosition.x+111, m_WindowPosition.y+21, "%");
	g_XSmallFont->Flush();
}

void _XWindow_MonsterStatus::DrawFunctionalObject()
{
	m_pMonsterStaminaGauge->ShowWindow(TRUE);
		
	m_pMonsterStaminaGauge->SetGaugeFactor(((_XFunctionalObject*)m_Target)->m_HealthPercentage);
	
	if(((_XFunctionalObject*)m_Target)->m_HealthPercentage != 0)
	{
		g_XSmallFont->SetColor(_XSC_DEFAULT);
		g_XSmallFont->Puts(m_WindowPosition.x+111, m_WindowPosition.y+21, "%");
		g_XSmallFont->Flush();
	}
	else
	{
		g_XSmallFont->SetColor(_XSC_DEFAULT);
		g_XSmallFont->Puts(m_WindowPosition.x+111, m_WindowPosition.y+21, "0%");
		g_XSmallFont->Flush();
	}

	if( m_pLastCalculateTarget != m_Target )
	{
		m_pLastCalculateTarget = m_Target;
	
		SetTargetGroupIndexNTexture(m_Target, m_TargetType);
		m_pTargetClanIcon->SetClipRect( g_MobTargetIconPos[m_TargetGroupIndex].left, g_MobTargetIconPos[m_TargetGroupIndex].top, 
			g_MobTargetIconPos[m_TargetGroupIndex].right, g_MobTargetIconPos[m_TargetGroupIndex].bottom );
	}
}

void _XWindow_MonsterStatus::DrawDummy()
{
	if( m_pLastCalculateTarget != m_Target )
	{
		m_pLastCalculateTarget = m_Target;

		m_pMonsterStaminaGauge->ShowWindow(FALSE);
		
		SetTargetGroupIndexNTexture(m_Target, m_TargetType);
		m_pTargetClanIcon->SetClipRect( g_MobTargetIconPos[m_TargetGroupIndex].left, g_MobTargetIconPos[m_TargetGroupIndex].top, 
										g_MobTargetIconPos[m_TargetGroupIndex].right, g_MobTargetIconPos[m_TargetGroupIndex].bottom );
	}
}

void _XWindow_MonsterStatus::DrawStateImage()
{
	int cnt = 0;

	_vec_MSState::iterator iter_state = m_vecState.begin();
	for( ; iter_state != m_vecState.end() ; ++iter_state)
	{
		_X_MS_State* pState = *iter_state;
		if(pState)
		{
			if(pState->imageIcon)
			{
				pState->imageIcon->MoveWindow(m_WindowPosition.x+7+21*(cnt%8), m_WindowPosition.y+m_WindowSize.cy+21*(cnt/8));
				pState->imageIcon->Draw();

				if(pState->imageIcon->CheckMousePosition())
				{
					POINT pos = pState->imageIcon->GetWindowPos();

					if(pState->nType == 0)
						g_InterfaceTooltip.SetToolTipString(pos.x+32, pos.y+32, g_SkillProperty[pState->nId]->skillName, _XSC_INFORMATION, TRUE);
					else if(pState->nType == 1)
						g_InterfaceTooltip.SetToolTipString(pos.x+32, pos.y+32, g_CharacterStateTable[pState->nId]->name, _XSC_INFORMATION, TRUE);
					else if(pState->nType == 2)
					{
						if(pState->nId > 0)
							g_InterfaceTooltip.SetToolTipString(pos.x+32, pos.y+32, g_CharacterConditionTable[pState->nId-1].name, _XSC_INFORMATION, TRUE);
					}
					else if(pState->nType == 3)
					{
						// resist - 처리 필요
						if(g_CharacterStateTable[pState->nId+36]->name)
							g_InterfaceTooltip.SetToolTipString(pos.x+32, pos.y+32, g_CharacterStateTable[pState->nId+36]->name, _XSC_INFORMATION, TRUE);
					}
				}
			}
		}

		++cnt;
	}
}

void _XWindow_MonsterStatus::DrawLevel(int level)
{
	int difflevel = level - g_pLocalUser->m_CharacterInfo.Get_innerlevel();
	if(difflevel > 4)	// 5 이상
	{
		m_pMonsterLevelIndicator->MoveWindow( m_WindowPosition.x + 35 + 54, m_WindowPosition.y + m_WindowSize.cy - 11);// m_WindowPosition.x + 35 + 54 + 9, m_WindowPosition.y + 38 + 5 );
		m_pMonsterLevelIndicator->SetClipRect( 237, 206, 246, 211 );
	}				
	else if(difflevel > 2 && difflevel <= 4)	// 3, 4
	{
		m_pMonsterLevelIndicator->MoveWindow( m_WindowPosition.x + 35 + 45, m_WindowPosition.y + m_WindowSize.cy - 11);// m_WindowPosition.x + 35 + 45 + 9, m_WindowPosition.y + 38 + 5 );
		m_pMonsterLevelIndicator->SetClipRect( 228, 206, 237, 211 );
	}
	else if(difflevel > 0 && difflevel <= 2)	// 1, 2
	{
		m_pMonsterLevelIndicator->MoveWindow( m_WindowPosition.x + 35 + 36, m_WindowPosition.y + m_WindowSize.cy - 11);// m_WindowPosition.x + 35 + 36 + 9, m_WindowPosition.y + 38 + 5 );
		m_pMonsterLevelIndicator->SetClipRect( 219, 206, 228, 211 );
	}
	else if(difflevel == 0)						// 0
	{
		m_pMonsterLevelIndicator->MoveWindow( m_WindowPosition.x + 35 + 27, m_WindowPosition.y + m_WindowSize.cy - 11);// m_WindowPosition.x + 35 + 27 + 9, m_WindowPosition.y + 38 + 5 );
		m_pMonsterLevelIndicator->SetClipRect( 210, 206, 219, 211 );
	}
	else if(difflevel < 0 && difflevel >= -2)	// -1, -2
	{
		m_pMonsterLevelIndicator->MoveWindow( m_WindowPosition.x + 35 + 18, m_WindowPosition.y + m_WindowSize.cy - 11);// m_WindowPosition.x + 35 + 18 + 9, m_WindowPosition.y + 38 + 5 );
		m_pMonsterLevelIndicator->SetClipRect( 201, 206, 210, 211 );
	}
	else if(difflevel < -2 && difflevel >= -4)	// -3, -4
	{
		m_pMonsterLevelIndicator->MoveWindow( m_WindowPosition.x + 35 + 9, m_WindowPosition.y + m_WindowSize.cy - 11);// m_WindowPosition.x + 35 + 9 + 9, m_WindowPosition.y + 38 + 5 );
		m_pMonsterLevelIndicator->SetClipRect( 192, 206, 201, 211 );
	}
	else if(difflevel < -4)						// -5 이상
	{
		m_pMonsterLevelIndicator->MoveWindow( m_WindowPosition.x + 35  , m_WindowPosition.y + m_WindowSize.cy - 11 );//, m_WindowPosition.x + 35 + 9, m_WindowPosition.y + 38 + 5 );
		m_pMonsterLevelIndicator->SetClipRect( 183, 206, 192, 211 );
	}
}

void _XWindow_MonsterStatus::DrawName()
{
	TCHAR cName[256];
	TCHAR szHiraganaName[256];

	memset(cName, 0, sizeof(cName));
	memset(szHiraganaName, 0, sizeof(szHiraganaName));

	switch(m_TargetType)
	{
	case _XPICKINGTARGET_MOB :
		{
			if(strlen(g_MonsterName[((_XMob*)m_Target)->m_MobType]) > 0)
			{
#ifdef _XJAPANESE
				if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
				{
					if(g_MonsterHiraganaName[((_XMob*)m_Target)->m_MobType] && 
					   strlen(g_MonsterHiraganaName[((_XMob*)m_Target)->m_MobType]) )
					{
						sprintf(cName, _T("%s"),g_MonsterName[((_XMob*)m_Target)->m_MobType] );
						sprintf(szHiraganaName, _T("(%s)"), g_MonsterHiraganaName[((_XMob*)m_Target)->m_MobType] );
					}
					else
					{
						strcpy(cName, g_MonsterName[((_XMob*)m_Target)->m_MobType]);
					}
				}
				else
#endif
				{
					strcpy(cName, g_MonsterName[((_XMob*)m_Target)->m_MobType]);
				}
			}
		}
		break;
	case _XPICKINGTARGET_PC :
		{
			if(strlen(((_XUser*)m_Target)->m_CharacterInfo.charactername ) > 0)
			{
#ifdef _XDEF_MATCH_EVENT_071025_KUKURI

	#ifdef _XDEF_MATCH_EVENT_PREVENT_ABUSE_080718_KUKURI
				if(((_XUser*)m_Target)->GetFreeMatchState())	// 내 상태와 관련없이 대회 중인 유저는 모두 안보이도록.
				{
					break;
				}
	#else
				if(g_pLocalUser->GetEnterMatchEvent() && g_MatchEventManager.GetActionConstraint(_xen_const_hideme))
				{
					if(((_XUser*)m_Target)->GetEnterMatchEvent())
					{
						break;	// 이름 보여 주지 않음
					}
				}
	#endif
#endif

				if( ((_XUser*)m_Target)->m_bFaceMaskMode )
					strcpy(cName, _XGETINTERFACETEXT(ID_STRING_NEW_2659) );	// "복면인(覆面人)"
				else
					strcpy(cName, ((_XUser*)m_Target)->m_CharacterInfo.charactername);
			}
		}
		break;
	case _XPICKINGTARGET_NPC :
		{
			if(((_XNPCObject*)m_Target)->m_NPCName && strlen(((_XNPCObject*)m_Target)->m_NPCName) > 0) 
				strcpy(cName,((_XNPCObject*)m_Target)->m_NPCName);
		}
		break;
	case _XPICKINGTARGET_SELF :
		{
			if(strlen(g_pLocalUser->m_CharacterInfo.charactername) > 0)
				strcpy(cName, g_pLocalUser->m_CharacterInfo.charactername);
		}
		break;
	case _XPICKINGTARGET_DUMMY :
		{
			strcpy(cName, _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_ACTIVE));	// "무공수련용"
		}
		break;
	case _XPICKINGTARGET_SEAT :
		{
			strcpy(cName, _XGETINTERFACETEXT(ID_STRING_MONSTERSTATUS_PASSIVE));	//"기공수련용"
		}
		break;
	case _XPICKINGTARGET_FUNCTIONALOBJECT :
		{
			if(((_XFunctionalObject*)m_Target)->m_ObjectName)
			strcpy(cName, ((_XFunctionalObject*)m_Target)->m_ObjectName);
		}
		break;
	}

	if(m_Target)
	{
		int textwidth = g_XBaseFont->GetWidth(cName);

		g_XBaseFont->SetColor(0xFF000000);
		/*
		if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
		{
			if(textwidth >= 174)
			{				
				bool expand = false;
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
				if( scrnpos->z > m_WindowPosition.y+5 )
				{
					if( scrnpos->z < m_WindowPosition.y+18 )
					{
						if( scrnpos->x > m_WindowPosition.x+3 )
						{
							if( scrnpos->x < m_WindowPosition.x+177 )
							{
								expand = true;
							}
						}
					}
				}

				if( !expand )
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+3, m_WindowPosition.y+5, m_WindowPosition.x+177, m_WindowPosition.y+18);
					g_XBaseFont->PutsAlign(m_WindowPosition.x+90, m_WindowPosition.y+6, _XFONT_ALIGNTYPE_CENTER, cName);
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}
				else
				{
					
					_XDrawSolidBar( m_WindowPosition.x+90-(textwidth>>1)-2, m_WindowPosition.y+4, m_WindowPosition.x+90+(textwidth>>1)+2, m_WindowPosition.y+4+17 , D3DCOLOR_ARGB(255,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+90-(textwidth>>1)-2, m_WindowPosition.y+4, m_WindowPosition.x+90+(textwidth>>1)+2, m_WindowPosition.y+4+17 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
					
					g_XBaseFont->PutsAlign(m_WindowPosition.x+90, m_WindowPosition.y+6, _XFONT_ALIGNTYPE_CENTER, cName);
				}
			}
			else
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x + 90, m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, cName);
			}
		}
		else if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			if(textwidth >= 111)
			{				
				bool expand = false;
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
				if( scrnpos->z > m_WindowPosition.y+5 )
				{
					if( scrnpos->z < m_WindowPosition.y+18 )
					{
						if( scrnpos->x > m_WindowPosition.x+52 )
						{
							if( scrnpos->x < m_WindowPosition.x+161 )
							{
								expand = true;
							}
						}
					}
				}

				if( !expand )
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+52, m_WindowPosition.y+5, m_WindowPosition.x+161, m_WindowPosition.y+18);
					g_XBaseFont->PrintAlign(m_WindowPosition.x+52, m_WindowPosition.y+6, 1.0f, _XFONT_ALIGNTYPE_LEFT, cName);
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}
				else
				{
					
					_XDrawSolidBar( m_WindowPosition.x+49, m_WindowPosition.y+4, m_WindowPosition.x+49+textwidth+5, m_WindowPosition.y+4+17 , D3DCOLOR_ARGB(255,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+49, m_WindowPosition.y+4, m_WindowPosition.x+49+textwidth+5, m_WindowPosition.y+4+17 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
					
					g_XBaseFont->PutsAlign(m_WindowPosition.x+52, m_WindowPosition.y+6, _XFONT_ALIGNTYPE_LEFT, cName);
				}
			}
			else
			{
				g_XBaseFont->Puts(m_WindowPosition.x + 82 - (textwidth >> 1), m_WindowPosition.y + 5, cName);
			}
		}
		else
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 82 - (textwidth >> 1), m_WindowPosition.y + 5, cName);
		}
		*/

#ifdef _XTS_NEW_HONOR2
		if(m_TargetType == _XPICKINGTARGET_PC)
		{
			int honorstep = g_Userlist.CalcHonorStep(((_XUser*)m_Target)->m_CharacterInfo.honor );
			if( honorstep != -1 )
				g_XBaseFont->SetColor(g_HonorColor[honorstep]);
			else
				g_XBaseFont->SetColor(0xFFFFFFFF);
		}
		else if( m_TargetType == _XPICKINGTARGET_SELF )
		{
			int honorstep = g_Userlist.CalcHonorStep( g_pLocalUser->m_CharacterInfo.honor );
			if( honorstep != -1 )
				g_XBaseFont->SetColor(g_HonorColor[honorstep]);
			else
				g_XBaseFont->SetColor(0xFFFFFFFF);
		}
		else
			g_XBaseFont->SetColor(0xFFFFFFFF);
#else
		g_XBaseFont->SetColor(0xFFFFFFFF);
#endif
		
#ifdef _XDEF_4EFF_MOBNAME_DISPLAY_071106_KUKURI
		// 음양혼신 속성에 따라 몬스터 이름 색상 다르게 표시한다
		if(m_TargetType == _XPICKINGTARGET_MOB)
		{
			D3DCOLOR color = 0xFFFFFFFF;
			switch(g_MobAttribTable[((_XMob*)m_Target)->m_MobType].attribute)
			{
			case 1 :
				{
					// 음 - 파란색
					color = 0xFF0000FF;
				}
				break;
			case 2 :
				{
					// 양 - 빨간색
					color = 0xFFFF4040;
				}
				break;
			case 3 :
				{
					// 혼 - 보라색
					color = 0xFFE066FF;
				}
				break;
			case 4 :
				{
					// 신 - 노란색
					color = 0xFFEEEE00;
				}
				break;
			default :
				{
					color = 0xFFFFFFFF;
				}
				break;
			}

			g_XBaseFont->SetColor(color);
		}
#endif

		if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
		{
			//Author : 양희왕 //breif : 몬스터 창 확장으로 수정 //Date : 08/01/18
			g_XBaseFont->PutsAlign(m_WindowPosition.x + (m_WindowSize.cx/2), m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, cName);
			g_XBaseFont->PutsAlign(m_WindowPosition.x + (m_WindowSize.cx/2), m_WindowPosition.y + 5 + 16, _XFONT_ALIGNTYPE_CENTER, szHiraganaName);
			/*if(textwidth >= 156)
			{				
				bool expand = false;
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
				if( scrnpos->z > m_WindowPosition.y+5 )
				{
					if( scrnpos->z < m_WindowPosition.y+20 )
					{
						if( scrnpos->x > m_WindowPosition.x+6 )
						{
							if( scrnpos->x < m_WindowPosition.x+162 )
							{
								expand = true;
							}
						}
					}
				}

				if( !expand )
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+6, m_WindowPosition.y+5, m_WindowPosition.x+162, m_WindowPosition.y+18);
					g_XBaseFont->Puts(m_WindowPosition.x+6, m_WindowPosition.y+6, cName, 1.0f);
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}
				else
				{
					
					_XDrawSolidBar( m_WindowPosition.x+4, m_WindowPosition.y+4, m_WindowPosition.x+textwidth+6, m_WindowPosition.y+4+16 , D3DCOLOR_ARGB(255,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+4, m_WindowPosition.y+4, m_WindowPosition.x+textwidth+6, m_WindowPosition.y+4+16 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
					
					g_XBaseFont->Puts(m_WindowPosition.x+6, m_WindowPosition.y+6, cName, 1.0f);
				}
			}
			else
			{
				g_XBaseFont->PutsAlign(m_WindowPosition.x + 90, m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, cName);
			}*/
		}
		else if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			if(textwidth >= 111)
			{				
				bool expand = false;
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();				
				if( scrnpos->z > m_WindowPosition.y+5 )
				{
					if( scrnpos->z < m_WindowPosition.y+18 )
					{
						if( scrnpos->x > m_WindowPosition.x+52 )
						{
							if( scrnpos->x < m_WindowPosition.x+161 )
							{
								expand = true;
							}
						}
					}
				}

				if( !expand )
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+52, m_WindowPosition.y+5, m_WindowPosition.x+161, m_WindowPosition.y+18);
					g_XBaseFont->PutsAlign(m_WindowPosition.x+52, m_WindowPosition.y+6, _XFONT_ALIGNTYPE_LEFT, cName);
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}
				else
				{
					
					_XDrawSolidBar( m_WindowPosition.x+49, m_WindowPosition.y+4, m_WindowPosition.x+49+textwidth+5, m_WindowPosition.y+4+17 , D3DCOLOR_ARGB(255,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+49, m_WindowPosition.y+4, m_WindowPosition.x+49+textwidth+5, m_WindowPosition.y+4+17 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
					
					g_XBaseFont->PutsAlign(m_WindowPosition.x+52, m_WindowPosition.y+6, _XFONT_ALIGNTYPE_LEFT, cName);
				}
			}
			else
			{
				g_XBaseFont->Puts(m_WindowPosition.x + 82 - (textwidth >> 1), m_WindowPosition.y + 5, cName);
			}
		}
		else
		{
			g_XBaseFont->Puts(m_WindowPosition.x + 81 - (textwidth >> 1), m_WindowPosition.y + 4, cName);
		}

		g_XBaseFont->Flush();
	}
}
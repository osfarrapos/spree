// XWindow_PartyMasterMiniBar.cpp: implementation of the _XWindow_PartyMasterMiniBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_PartyMasterMiniBar.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XCallback_Minimize( POINT pos, SIZE size )
{
	if( g_pMainFrame_Window && !g_pMainFrame_Window->GetShowStatus() )
	{
		g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_PARTY_2289), _XSC_INFORMATION );
	}
}

void __stdcall _XCallback_Maximize( POINT pos, SIZE size )
{
	if( g_pMainFrame_Window && !g_pMainFrame_Window->GetShowStatus() )
	{
		g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_PARTY_2290), _XSC_INFORMATION );	
	}
}

void __stdcall _XCallback_Close( POINT pos, SIZE size )
{
	if( g_pMainFrame_Window && !g_pMainFrame_Window->GetShowStatus() )
	{
		g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_PARTY_2291), _XSC_INFORMATION );	
	}
}

_XWindow_PartyMasterMiniBar::_XWindow_PartyMasterMiniBar()
{	
	m_PartyMasterMode					= _XPARTYMASTERMODE_MAXIMUM;

	m_imagePartyMasterBorder			= NULL;				// 동행 행수창 바탕 이미지 
	m_imagePartyMasterMinimizeBorder	= NULL;				// 동행 행수창 축소시 바탕 이미지
	m_pPartyMasterLifePowerGauge		= NULL;				// 동행 행수창 체력 게이지
	m_pPartyMasterForcePowerGauge		= NULL;				// 동행 행수창 진기 게이지 
	
	m_btnPartyMasterMinimize			= NULL;				// 동행 행수창 축소 버튼 
	m_btnPartyMasterMaximize			= NULL;				// 동행 행수창 확대 버튼 
	m_btnPartyMasterClose				= NULL;				// 동행 행수창 닫기 버튼 

	m_btnPartyExperienceSharing			= NULL;				// 동행 행수창 경험치 분배 버튼 
	m_btnPartyItemSharing				= NULL;				// 동행 행수창 아이템 분배 버튼 
	
	// 경고 애니매이션 관련 변수 
	m_bDangerous						= FALSE;		// 위험한 상태일때 
	m_PrevDrawTime						= 0;
	m_EffectOrderSub					= FALSE;
	m_EffectFrame						= 0.0f;

	m_CloseButtoncommandid				= 0;

	m_listboxPartyExperienceSharing		= NULL;
	m_indexPartyExperienceSharing		= 0;

	m_listboxPartyItemSharing			= NULL;
	m_indexPartyItemSharing				= 0;
	
	m_PlayerUniqueid					= 0;			//파티 멤버의 아이디 
	m_bIsCurrentUser					= FALSE;
	m_bDeathUser						= FALSE;
	
	m_ZoneIndex							= -1;
	m_bIsCurrentZone					= FALSE;
	memset( m_PartyMasterName, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH );
	
	m_PartyIndex						= -1;
	for(int i = 0 ; i < 3 ; i++)
		m_pPartyMasterDanger[i] = NULL;

	m_pFarActionIcon = NULL;
	m_pOutActionIcon = NULL;
	m_pClassIcon	 = NULL;
	m_pClassBorderIcon = NULL;

	m_bOut = false;
	m_bFarAway = false;
}

_XWindow_PartyMasterMiniBar::~_XWindow_PartyMasterMiniBar()
{

}


BOOL _XWindow_PartyMasterMiniBar::Initialize( void )
{
	int partyresourceindex = g_MainInterfaceTextureArchive.FindResource( "mi_party.tga" );
	int charinforesourceindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int chattingbuttonindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
		
#ifdef _XDEF_PARTY_NOTAVAILABLE
	// 동행 행수창 바탕 이미지 ( 179 X 56 ) 
	m_imagePartyMasterBorder = new _XImageStatic;
	m_imagePartyMasterBorder->Create( 0, 0, 179, 56, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_party2.tga") );
	m_imagePartyMasterBorder->SetClipRect(0, 0, 180, 57);
	m_imagePartyMasterBorder->ShowWindow(TRUE);
	InsertChildObject( m_imagePartyMasterBorder );
#else
	// 동행 행수창 바탕 이미지 ( 165 X 56 ) 
	m_imagePartyMasterBorder = new _XImageStatic;
	m_imagePartyMasterBorder->Create( 0, 0, 165, 56, &g_MainInterfaceTextureArchive, partyresourceindex );
	m_imagePartyMasterBorder->SetClipRect(0, 0, 165, 56);
	m_imagePartyMasterBorder->ShowWindow(TRUE);
	InsertChildObject( m_imagePartyMasterBorder );
#endif
	
	// 동행 행수창 축소시 바탕 이미지 ( 165 X 36 ) 
	m_imagePartyMasterMinimizeBorder = new _XImageStatic;
	m_imagePartyMasterMinimizeBorder->Create( 0, 0, 165, 36, &g_MainInterfaceTextureArchive, partyresourceindex );
	m_imagePartyMasterMinimizeBorder->SetClipRect(0, 106, 165, 142);
	m_imagePartyMasterMinimizeBorder->ShowWindow(FALSE);
	InsertChildObject( m_imagePartyMasterMinimizeBorder );
	
	//동행 행수창 축소 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE minimizestruct = { TRUE, { 134, 4 }, { 12, 12 }, _XDEF_MINIPARTYWINDOW_MINIMIZEBUTTON,
		charinforesourceindex,charinforesourceindex,charinforesourceindex, 
		&g_MainInterfaceTextureArchive };
	m_btnPartyMasterMinimize = new _XButton;
	m_btnPartyMasterMinimize->Create( minimizestruct );		
	m_btnPartyMasterMinimize->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 155, 212, 167, 224);
	m_btnPartyMasterMinimize->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  167, 212, 179, 224 );
	m_btnPartyMasterMinimize->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  179, 212, 191, 224 );
	m_btnPartyMasterMinimize->ShowWindow(TRUE);
	m_btnPartyMasterMinimize->SetCallBackFunction_ToolTip( _XCallback_Minimize );
	InsertChildObject( m_btnPartyMasterMinimize );
	
	//동행 행수창 확대 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE maximizestruct = { TRUE, { 134, 4 }, { 12, 12 }, _XDEF_MINIPARTYWINDOW_MAXIMIZEBUTTON,
		partyresourceindex,partyresourceindex,partyresourceindex, 
		&g_MainInterfaceTextureArchive };
	m_btnPartyMasterMaximize = new _XButton;
	m_btnPartyMasterMaximize->Create( maximizestruct );		
	m_btnPartyMasterMaximize->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 167, 2, 179, 14);
	m_btnPartyMasterMaximize->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  179, 2, 191, 14 );
	m_btnPartyMasterMaximize->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  191, 2, 203, 14 );
	m_btnPartyMasterMaximize->ShowWindow(FALSE);
	m_btnPartyMasterMaximize->SetCallBackFunction_ToolTip( _XCallback_Maximize );
	InsertChildObject( m_btnPartyMasterMaximize );
	
	// 동행 행수창 닫기 버튼	 ( 12 X 12 ) 
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 147, 4 }, { 12, 12 }, m_CloseButtoncommandid,
		charinforesourceindex,charinforesourceindex,charinforesourceindex, 
		&g_MainInterfaceTextureArchive };
	m_btnPartyMasterClose = new _XButton;
	m_btnPartyMasterClose->Create( closebtnstruct );	
	m_btnPartyMasterClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_btnPartyMasterClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_btnPartyMasterClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	m_btnPartyMasterClose->ShowWindow(TRUE);
	m_btnPartyMasterClose->SetCallBackFunction_ToolTip( _XCallback_Close );
	InsertChildObject( m_btnPartyMasterClose );
	
	
	// 동행 행수창 체력 게이지 ( 130 X 5)
	_XSTICKGAUGE_STRUCTURE	gaugestruct = 
	{
		TRUE, { 30, 20 }, { 130, 5 },	
			partyresourceindex, -1, &g_MainInterfaceTextureArchive, 
	};
	
	m_pPartyMasterLifePowerGauge = new _XStickGauge;
	m_pPartyMasterLifePowerGauge->Create( gaugestruct );	
	m_pPartyMasterLifePowerGauge->SetImageClipRect( _XGAUGEIMAGE_BORDER,  3,  58, 133,  63 );
	m_pPartyMasterLifePowerGauge->SetFactorDraw(TRUE);	
	InsertChildObject( m_pPartyMasterLifePowerGauge );
	
	// 동행 행수창 진기 게이지 ( 130 X 5)
	gaugestruct.position.y = 27;
	m_pPartyMasterForcePowerGauge = new _XStickGauge;
	m_pPartyMasterForcePowerGauge->Create( gaugestruct );	
	m_pPartyMasterForcePowerGauge->SetImageClipRect( _XGAUGEIMAGE_BORDER,  3,  65, 133,  70 );
	m_pPartyMasterForcePowerGauge->SetFactorDraw(TRUE);	
	InsertChildObject( m_pPartyMasterForcePowerGauge );

		
	//동행 행수창 경험치 분배 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE experiencestruct = { TRUE, { 69, 38 }, { 12, 12 }, _XDEF_MINIPARTYWINDOW_EXPSHARINGBUTTON,
		chattingbuttonindex,chattingbuttonindex,chattingbuttonindex, 
		&g_MainInterfaceTextureArchive };
	m_btnPartyExperienceSharing = new _XButton;
	m_btnPartyExperienceSharing->Create( experiencestruct );		
	m_btnPartyExperienceSharing->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,  12, 12, 24 );
	m_btnPartyExperienceSharing->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  12, 12, 24, 24 );
	m_btnPartyExperienceSharing->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  24, 12, 36, 24 );
	m_btnPartyExperienceSharing->ShowWindow(TRUE);
	m_btnPartyExperienceSharing->EnableWindow(FALSE);
	InsertChildObject( m_btnPartyExperienceSharing );
	
	//동행 행수창 아이템 분배 버튼 ( 12 X 12 ) 
	_XBTN_STRUCTURE itemstruct = { TRUE, { 148, 38 }, { 12, 12 }, _XDEF_MINIPARTYWINDOW_ITEMSHARINGBUTTON,
		chattingbuttonindex,chattingbuttonindex,chattingbuttonindex, 
		&g_MainInterfaceTextureArchive };
	m_btnPartyItemSharing = new _XButton;
	m_btnPartyItemSharing->Create( itemstruct );		
	m_btnPartyItemSharing->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,  12, 12, 24 );
	m_btnPartyItemSharing->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  12, 12, 24, 24 );
	m_btnPartyItemSharing->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  24, 12, 36, 24 );
	m_btnPartyItemSharing->ShowWindow(TRUE);
	m_btnPartyItemSharing->EnableWindow(FALSE);

	//m_btnPartyItemSharing->ShowWindow(TRUE);
	//m_btnPartyItemSharing->EnableWindow(FALSE);
	InsertChildObject( m_btnPartyItemSharing );
	
	
	// 위험 표시 이미지 ( 39 X 32 )
	for(int i = 0 ; i < 3 ; i++)
	{
		m_pPartyMasterDanger[i] = new _XImageStatic;
		m_pPartyMasterDanger[i]->Create(0, 0, 39, 32, &g_MainInterfaceTextureArchive, partyresourceindex);
	}
	m_pPartyMasterDanger[0]->SetClipRect(0, 71, 39, 103);
	m_pPartyMasterDanger[1]->SetClipRect(40, 71, 79, 103);
	m_pPartyMasterDanger[2]->SetClipRect(81, 71, 120, 103);	
	
	// ---------------------------------------------------------------------------------------------------------------------=		
	// 경험치 분배 설정에 사용될 리스트 박스
	_XLISTBOX_STRUCTURE sharingstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 2, 53 },										// 윈도우 좌표
		{ 80, 38 },										// 윈도우 사이즈
		_XDEF_MINIPARTYWINDOW_EXPSHAERINGLISTBOX,		// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		19,												// 리스트 아이템 라인 피치
		2,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_NOSCROLLBAR,							// 스크롤바의 위치
	};
	m_listboxPartyExperienceSharing = new _XListBox;
	m_listboxPartyExperienceSharing->Create(sharingstructure);
	
	POINT columnoffset1 = { 10, 3 };
	m_listboxPartyExperienceSharing->SetColumnOffset(0, columnoffset1);			// column offset	
	m_listboxPartyExperienceSharing->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style (아이콘)	
	m_listboxPartyExperienceSharing->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxPartyExperienceSharing->SetColumnWidth(0, 80);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxPartyExperienceSharing->SetBorderColorFactor( 0xB1000000 );
	m_listboxPartyExperienceSharing->SetSelectedImagePosition(0, 3);
	m_listboxPartyExperienceSharing->SetSelectedImageHeight(19);
	m_listboxPartyExperienceSharing->SetTrackWidth(70);
	m_listboxPartyExperienceSharing->SetTrackHeight(19);
	
	for( i = 0; i < 2; i++ )
	{
		m_listboxPartyExperienceSharing->InsertListItem(_T(""), 0, i+1);		
		
		m_listboxPartyExperienceSharing->SetItemText(i, 0, g_stringExperienceSharingForm[i+1]);
		m_listboxPartyExperienceSharing->SetItemAttrib(i, 0, _XSC_DEFAULT);
		m_listboxPartyExperienceSharing->SetItemAttribHighlight(i, 0, _XSC_DEFAULT_HIGHLIGHT);		
	}
	m_listboxPartyExperienceSharing->ShowWindow(FALSE);
	InsertChildObject(m_listboxPartyExperienceSharing);
	
	// ---------------------------------------------------------------------------------------------------------------------=		
	// 아이템 분배 설정에 사용될 리스트 박스
	_XLISTBOX_STRUCTURE itemsharingstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 83, 53 },										// 윈도우 좌표
		{ 80, 38 },										// 윈도우 사이즈 -> 나중에 38-> 57 변경예정
		_XDEF_MINIPARTYWINDOW_ITEMSHAERINGLISTBOX,		// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		19,												// 리스트 아이템 라인 피치
		3,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_NOSCROLLBAR,							// 스크롤바의 위치
	};
	m_listboxPartyItemSharing = new _XListBox;
	m_listboxPartyItemSharing->Create(itemsharingstructure);
	
	POINT itemcolumnoffset1 = { 10, 3 };
	m_listboxPartyItemSharing->SetColumnOffset(0, itemcolumnoffset1);			// column offset	
	m_listboxPartyItemSharing->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style (아이콘)	
	m_listboxPartyItemSharing->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxPartyItemSharing->SetColumnWidth(0, 80);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxPartyItemSharing->SetBorderColorFactor( 0xB1000000 );
	m_listboxPartyItemSharing->SetSelectedImagePosition(0, 3);
	m_listboxPartyItemSharing->SetSelectedImageHeight(19);
	m_listboxPartyItemSharing->SetTrackWidth(70);
	m_listboxPartyItemSharing->SetTrackHeight(19);
	
	for( i = 0; i < 2; i++ ) 
	{
		m_listboxPartyItemSharing->InsertListItem(_T(""), 0, i+1);		
		
		m_listboxPartyItemSharing->SetItemText(i, 0, g_stringItemSharingForm[i+1]);
		m_listboxPartyItemSharing->SetItemAttrib(i, 0, _XSC_DEFAULT);
		m_listboxPartyItemSharing->SetItemAttribHighlight(i, 0, _XSC_DEFAULT_HIGHLIGHT);		
	}
	m_listboxPartyItemSharing->ShowWindow(FALSE);
	InsertChildObject(m_listboxPartyItemSharing);
	


	memset( m_PartyMasterName, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );

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
	m_pClassBorderIcon->Create(0, 0, 44, 48, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_Char_01.tga"));
	m_pClassBorderIcon->SetClipRect(153, 121, 196, 168);
#endif

	return TRUE;
}

void _XWindow_PartyMasterMiniBar::DestroyWindow( void )
{	
	for(int i = 0 ; i < 3 ; i++)
		SAFE_DELETE(m_pPartyMasterDanger[i]);

	SAFE_DELETE(m_pFarActionIcon);
	SAFE_DELETE(m_pOutActionIcon);
	SAFE_DELETE(m_pClassIcon);
	SAFE_DELETE(m_pClassBorderIcon);
	
	_XWindow::DestroyWindow();
}

BOOL _XWindow_PartyMasterMiniBar::CheckMousePositionExperienceSharingBox(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 4) )
		if(scrnpos->x < (m_WindowPosition.x + 82) )
			if( scrnpos->z > (m_WindowPosition.y + 36) )
				if( scrnpos->z < (m_WindowPosition.y + 52) )
					return TRUE;
				
				return FALSE;
}
BOOL _XWindow_PartyMasterMiniBar::CheckMousePositionItemSharingBox(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 83) )
		if(scrnpos->x < (m_WindowPosition.x + 161) )
			if( scrnpos->z > (m_WindowPosition.y + 36) )
				if( scrnpos->z < (m_WindowPosition.y + 52) )
					return TRUE;
				
				return FALSE;
}

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
extern void _XDrawShadowedRectAngle( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT baseheight, D3DCOLOR highlightcolor, D3DCOLOR shadowcolor  );
#endif

void _XWindow_PartyMasterMiniBar::Draw( _XGUIObject*& pfocusobject )
{			
	if( !this->m_ShowWindow ) return;
	
	_XWindow::Draw(pfocusobject);	

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
	if( m_PartyMasterMode == _XPARTYMASTERMODE_MAXIMUM )
	{
		if( m_pClassBorderIcon )
			m_pClassBorderIcon->Draw( m_WindowPosition.x - 43, m_WindowPosition.y + 4 );

		if( m_pClassIcon )
			m_pClassIcon->Draw( m_WindowPosition.x - 35, m_WindowPosition.y + 12 );
		
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();	
		
		if( ((scrnpos->x > m_WindowPosition.x-43) ) && (scrnpos->x < m_WindowPosition.x-1) &&
			(scrnpos->z > m_WindowPosition.y+4 ) && (scrnpos->z < m_WindowPosition.y+4+48) )
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

				g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x, m_WindowPosition.y + 12, _XGETINTERFACETEXT(ID_STRING_NEW_2660+classid-1), _XSC_INFORMATION, FALSE);
			}
			else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 0)
			{
				g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x, m_WindowPosition.y + 12, _XGETINTERFACETEXT(ID_STRING_NEW_3713), _XSC_INFORMATION, FALSE);
			}
		}
	}
	else
	{
		if( m_pClassBorderIcon )
			m_pClassBorderIcon->Draw( m_WindowPosition.x - 40, m_WindowPosition.y );

		if( m_pClassIcon )
			m_pClassIcon->Draw( m_WindowPosition.x - 35, m_WindowPosition.y + 3 );

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
	}
#endif
	
	switch(m_PartyMasterMode) {
	case _XPARTYMASTERMODE_MAXIMUM:
		{			
			if( !m_bDangerous )
			{
				g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );
				g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
				g_XBaseFont->Puts( m_WindowPosition.x +52, m_WindowPosition.y + 4, m_PartyMasterName );
				g_XBaseFont->Flush();	
				g_XBaseFont->DisableGrowMode();
				
				g_XBaseFont->SetColor( _XSC_DEFAULT );
				g_XBaseFont->Puts( m_WindowPosition.x +8, m_WindowPosition.y + 39, g_stringExperienceSharingForm[m_indexPartyExperienceSharing] );
				g_XBaseFont->Puts( m_WindowPosition.x +88, m_WindowPosition.y + 39, g_stringItemSharingForm[m_indexPartyItemSharing] );
				
				if( !m_bIsCurrentZone )
				{
					// ( 132 X 14 )
					_XDrawSolidBar( m_WindowPosition.x+29,  m_WindowPosition.y+19, m_WindowPosition.x+161, m_WindowPosition.y+33 , D3DCOLOR_ARGB(255,11,12,18) );
					_XDrawRectAngle( m_WindowPosition.x+28,  m_WindowPosition.y+18, m_WindowPosition.x+161, m_WindowPosition.y+33 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
				
					TCHAR messagestring[64];
					memset( messagestring, 0, sizeof(TCHAR)*64 );	
					sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_PARTY_2292), g_ZoneInfoTable[ m_ZoneIndex - 1].zoneshortname );		
					g_XBaseFont->Puts( m_WindowPosition.x+35,  m_WindowPosition.y+21, messagestring );
				}

				g_XBaseFont->Flush();
			}
			else
			{
				if( !m_bDeathUser )
				{
					DrawDangerMode();
					g_XBaseFont->SetColor( _XSC_WARNING_HIGHLIGHT );
					g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
					g_XBaseFont->Puts( m_WindowPosition.x +52, m_WindowPosition.y + 4, m_PartyMasterName );
					g_XBaseFont->Flush();	
					g_XBaseFont->DisableGrowMode();

					g_XBaseFont->SetColor( _XSC_DEFAULT );
					g_XBaseFont->Puts( m_WindowPosition.x +8, m_WindowPosition.y + 39, g_stringExperienceSharingForm[m_indexPartyExperienceSharing] );
					g_XBaseFont->Puts( m_WindowPosition.x +88, m_WindowPosition.y + 39, g_stringItemSharingForm[m_indexPartyItemSharing] );
					g_XBaseFont->Flush();	
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 100, 100, 100 ) );
					g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
					g_XBaseFont->Puts( m_WindowPosition.x +52, m_WindowPosition.y + 4, m_PartyMasterName );
					g_XBaseFont->Flush();	
					g_XBaseFont->DisableGrowMode();
					
					_XDrawSolidBar( m_WindowPosition.x+29,  m_WindowPosition.y+19, m_WindowPosition.x+161, m_WindowPosition.y+33 , D3DCOLOR_ARGB(255,11,12,18) );
					_XDrawRectAngle( m_WindowPosition.x+28,  m_WindowPosition.y+18, m_WindowPosition.x+161, m_WindowPosition.y+33 , 0.0f, D3DCOLOR_ARGB(255,0,0,0) );	
					
					g_XBaseFont->SetColor( _XSC_WARNING_HIGHLIGHT );
					g_XBaseFont->Puts( m_WindowPosition.x+45,  m_WindowPosition.y+21, _XGETINTERFACETEXT(ID_STRING_PARTY_2293) );
					g_XBaseFont->Flush();	
					
					g_XBaseFont->SetColor( _XSC_DEFAULT );
					g_XBaseFont->Puts( m_WindowPosition.x +8, m_WindowPosition.y + 39, g_stringExperienceSharingForm[m_indexPartyExperienceSharing] );
					g_XBaseFont->Puts( m_WindowPosition.x +88, m_WindowPosition.y + 39, g_stringItemSharingForm[m_indexPartyItemSharing] );
					g_XBaseFont->Flush();	
				}
				
			}
			
			bool flag = false;
			if( CheckMousePositionExperienceSharingBox() )
			{		
				if( g_pMainFrame_Window && !g_pMainFrame_Window->GetShowStatus() )
				{
					g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x +4, m_WindowPosition.y + 15, _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONEXP), _XSC_INFORMATION, FALSE );	//_T("경험치분배")
					g_InterfaceTooltip.Draw();			
				}
			}
			else if( CheckMousePositionItemSharingBox() )
			{		
				if( g_pMainFrame_Window && !g_pMainFrame_Window->GetShowStatus() )
				{
					g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x +83, m_WindowPosition.y + 15, _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_DIVISIONITEM), _XSC_INFORMATION, FALSE );		// _T("전리품분배")
					g_InterfaceTooltip.Draw();			
				}
			}
			else if( CheckMousePosition() )
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
									if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PARTYINFO, m_PartyIndex, 1, 0, (int)m_bOut, (DWORD)m_bFarAway ) )
										g_pMiniTooltip_Window->ShowWindow( TRUE );
								}
								else
								{
									if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PARTYINFO, m_PartyIndex, 0, 0, (int)m_bOut, (DWORD)m_bFarAway ) )
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
			m_pFarActionIcon->Draw(m_WindowPosition.x+164, m_WindowPosition.y+19);
			m_pOutActionIcon->Draw(m_WindowPosition.x+164, m_WindowPosition.y+32);
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
		break;
		
	case _XPARTYMASTERMODE_MINIMUM:
		{
			if( !m_bDangerous )
			{
				g_XBaseFont->SetColor(  _XSC_DEFAULT_HIGHLIGHT);
				g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
				g_XBaseFont->Puts( m_WindowPosition.x +52, m_WindowPosition.y + 4, m_PartyMasterName );
				g_XBaseFont->Flush();	
				g_XBaseFont->DisableGrowMode();
				
				g_XBaseFont->SetColor( _XSC_DEFAULT );
				g_XBaseFont->Print( m_WindowPosition.x +18, m_WindowPosition.y + 21, 1.0f, _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_PARTYCOUNT), g_pLocalUser->m_PartyMemberCount );	// "동행인원 : %d명"
				g_XBaseFont->Flush();	
			}
			else
			{
				
				DrawDangerMode();

				g_XBaseFont->SetColor( _XSC_WARNING_HIGHLIGHT );
				g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
				g_XBaseFont->Puts( m_WindowPosition.x +52, m_WindowPosition.y + 4, m_PartyMasterName );
				g_XBaseFont->Flush();	
				g_XBaseFont->DisableGrowMode();

				g_XBaseFont->SetColor( _XSC_DEFAULT );
				g_XBaseFont->Print( m_WindowPosition.x +18, m_WindowPosition.y + 21, 1.0f, _XGETINTERFACETEXT(ID_STRING_PARTYMASTERMINIBAR_PARTYCOUNT), g_pLocalUser->m_PartyMemberCount );	// "동행인원 : %d명"
				g_XBaseFont->Flush();	
			}

			if( g_pMiniTooltip_Window->GetShowStatus() )
			{
				if( g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_PARTYINFO )
					g_pMiniTooltip_Window->ShowWindow( FALSE );
			}
		}
		break;
	}
	

}

void _XWindow_PartyMasterMiniBar::DrawDangerMode(void)
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
		m_EffectFrame = 1.0f;
	}
	
	
	int selectedframe = (int)m_EffectFrame;
	
	if(selectedframe > 2)
		selectedframe = 2;
	else if(selectedframe < 0)
		selectedframe = 0;
	
	m_pPartyMasterDanger[selectedframe]->Draw(m_WindowPosition.x, m_WindowPosition.y);
	m_PrevDrawTime = g_LocalSystemTime;
}

BOOL _XWindow_PartyMasterMiniBar::Process( _XGUIObject*& pfocusobject )
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
					if( m_PartyMasterMode == _XPARTYMASTERMODE_MAXIMUM)
					{
						if( !m_btnPartyMasterMinimize->CheckMousePosition() 
							&& !m_btnPartyMasterClose->CheckMousePosition()
							&& !m_btnPartyExperienceSharing->CheckMousePosition()
							&& !m_btnPartyItemSharing->CheckMousePosition() )
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
					}//if( m_PartyMasterMode == _XPARTYMASTERMODE_MAXIMUM)
				}//if( CheckMousePosition() )
			}
			else if( mousestate->bButton[1] )
			{
				// 오른쪽 클릭
				if(CheckMousePosition())
				{					
					if( !m_btnPartyMasterMinimize->CheckMousePosition() 
						&& !m_btnPartyMasterClose->CheckMousePosition()
						&& !m_btnPartyExperienceSharing->CheckMousePosition()
						&& !m_btnPartyItemSharing->CheckMousePosition() )
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

void _XWindow_PartyMasterMiniBar::SetPartyMasterName(int partyindex, BOOL& isCurrentUser)
{
	memset(m_PartyMasterName, 0, sizeof(TCHAR)*_XDEF_MAX_USERNAMESTRINGLENGTH);
	
	m_PartyIndex = partyindex;
	if(g_pLocalUser->m_PartyInfo[partyindex].playername[0] != 0)
	{
		if(strcmp(g_pLocalUser->m_CharacterInfo.charactername, g_pLocalUser->m_PartyInfo[partyindex].playername) != 0)
		{
			strcpy(m_PartyMasterName, g_pLocalUser->m_PartyInfo[partyindex].playername);
			m_btnPartyMasterClose->EnableWindow(FALSE);
			m_bIsCurrentUser = isCurrentUser = FALSE;
		}
		else
		{
			strcpy(m_PartyMasterName, g_pLocalUser->m_PartyInfo[partyindex].playername);
			m_btnPartyExperienceSharing->EnableWindow(TRUE);
			m_btnPartyItemSharing->EnableWindow(TRUE);
			m_btnPartyMasterClose->EnableWindow(TRUE);
			m_bIsCurrentUser = isCurrentUser = TRUE;
		}

		SetClassIcon();
	}
}

void _XWindow_PartyMasterMiniBar::SetPartyMasterVital(int partyindex, BOOL& isCurrentUser)
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
			m_pPartyMasterLifePowerGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[partyindex].vitalpercent);
			m_pPartyMasterForcePowerGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[partyindex].zenpercent);
			m_bIsCurrentUser = isCurrentUser = FALSE;
		}
		else
		{
			m_pPartyMasterLifePowerGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[partyindex].vitalpercent);
			m_pPartyMasterForcePowerGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[partyindex].zenpercent);
			m_bIsCurrentUser = isCurrentUser = TRUE;
		}
	}
}

void _XWindow_PartyMasterMiniBar::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
}


void _XWindow_PartyMasterMiniBar::SetMode( _XPARTYMASTER_MODE partymastermode )
{
	m_PartyMasterMode = partymastermode;
	
	switch(partymastermode) {
	case _XPARTYMASTERMODE_MAXIMUM:
		{			
			SetWindowSize(165,56);

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
			m_WindowPosition.x = 50;
			m_pClassBorderIcon->SetClipRect(153, 121, 196, 168);
#else			
			m_WindowPosition.x = 14;
#endif
			m_WindowPosition.y = 114;
			
			_XStatic* imageborder = GetBorderStatic();
			imageborder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+ 165, m_WindowPosition.y+ 56);

			m_imagePartyMasterBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y);
			m_btnPartyMasterMinimize->MoveWindow(m_WindowPosition.x+134, m_WindowPosition.y+4);
			m_btnPartyMasterClose->MoveWindow(m_WindowPosition.x+147, m_WindowPosition.y+4);
			m_pPartyMasterLifePowerGauge->MoveWindow(m_WindowPosition.x+30, m_WindowPosition.y+20);
			m_pPartyMasterForcePowerGauge->MoveWindow(m_WindowPosition.x+30, m_WindowPosition.y+27);
			m_btnPartyExperienceSharing->MoveWindow(m_WindowPosition.x+69, m_WindowPosition.y+38);
			m_btnPartyItemSharing->MoveWindow(m_WindowPosition.x+148, m_WindowPosition.y+38);
			m_listboxPartyExperienceSharing->MoveWindow(m_WindowPosition.x+2, m_WindowPosition.y+53);
			m_listboxPartyItemSharing->MoveWindow(m_WindowPosition.x+83, m_WindowPosition.y+53);

			m_imagePartyMasterBorder->ShowWindow(TRUE);					// 동행 행수창 바탕 이미지 
			m_imagePartyMasterMinimizeBorder->ShowWindow(FALSE);		// 동행 행수창 축소시 바탕 이미지
			m_pPartyMasterLifePowerGauge->ShowWindow(TRUE);				// 동행 행수창 체력 게이지
			m_pPartyMasterForcePowerGauge->ShowWindow(TRUE);			// 동행 행수창 진기 게이지 
			
			m_btnPartyExperienceSharing->ShowWindow(TRUE);				// 동행 행수창 경험치 분배 버튼 
			m_btnPartyItemSharing->ShowWindow(TRUE);					// 동행 행수창 아이템 분배 버튼 
			m_btnPartyMasterMinimize->ShowWindow(TRUE);					// 동행 행수창 축소 버튼 
			m_btnPartyMasterMaximize->ShowWindow(FALSE);				// 동행 행수창 확대 버튼 
			m_listboxPartyExperienceSharing->ShowWindow(FALSE);
			m_listboxPartyItemSharing->ShowWindow(FALSE);
		}
		break;

	case _XPARTYMASTERMODE_MINIMUM:
		{
			SetWindowSize(165,36);
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI	
			m_WindowPosition.x = 50;
			m_pClassBorderIcon->SetClipRect(198, 121, 238, 157);
#else
			m_WindowPosition.x = 14;
#endif
			
#ifdef _ACCLAIM_RUBICONADSYSTEM
			m_WindowPosition.y = 114+((g_ViewRubiconAD)?g_RubiconADWindowSize.cy:0);
#else
			m_WindowPosition.y = 114;
#endif
			
			_XStatic* imageborder = GetBorderStatic();
			imageborder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+ 165, m_WindowPosition.y+ 36);
			
			m_imagePartyMasterMinimizeBorder->MoveWindow(m_WindowPosition.x, m_WindowPosition.y);
			m_btnPartyMasterMaximize->MoveWindow(m_WindowPosition.x+134, m_WindowPosition.y+4);
			m_btnPartyMasterClose->MoveWindow(m_WindowPosition.x+147, m_WindowPosition.y+4);

			m_imagePartyMasterBorder->ShowWindow(FALSE);				// 동행 행수창 바탕 이미지 
			m_imagePartyMasterMinimizeBorder->ShowWindow(TRUE);			// 동행 행수창 축소시 바탕 이미지
			m_pPartyMasterLifePowerGauge->ShowWindow(FALSE);			// 동행 행수창 체력 게이지
			m_pPartyMasterForcePowerGauge->ShowWindow(FALSE);			// 동행 행수창 진기 게이지 
			
			m_btnPartyExperienceSharing->ShowWindow(FALSE);				// 동행 행수창 경험치 분배 버튼 
			m_btnPartyItemSharing->ShowWindow(FALSE);					// 동행 행수창 아이템 분배 버튼 
			m_btnPartyMasterMinimize->ShowWindow(FALSE);				// 동행 행수창 축소 버튼 
			m_btnPartyMasterMaximize->ShowWindow(TRUE);					// 동행 행수창 확대 버튼 
			m_listboxPartyExperienceSharing->ShowWindow(FALSE);
		}
		break;
	}
}

void _XWindow_PartyMasterMiniBar::ProcessDblClick()
{	
	if( !m_bIsCurrentUser )
	{
		_XUser* pUser = NULL;					
		pUser = (_XUser*)g_Userlist.FindUser(m_PlayerUniqueid);
		if( pUser )
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


void _XWindow_PartyMasterMiniBar::SetZoneIndex( int zoneindex )
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

void _XWindow_PartyMasterMiniBar::SetPartyUserStateImage(void)
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
				
				map <int, _XCharacterState*>::iterator iter_state;
				for(iter_state = pUser->m_StateList.m_mapCharacterStateList.begin() ; iter_state != pUser->m_StateList.m_mapCharacterStateList.end() ; iter_state++)
				{
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

void _XWindow_PartyMasterMiniBar::SetOut(bool state)
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

void _XWindow_PartyMasterMiniBar::SetFarAway(bool state)
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

void _XWindow_PartyMasterMiniBar::Reset()
{
#ifdef _XDEF_PARTY_NOTAVAILABLE
	// 거리이탈, 자리비움 초기화
	m_bOut = m_bFarAway = false;
	m_pOutActionIcon->SetClipRect(192, 36, 203, 47);
	m_pFarActionIcon->SetClipRect(192, 36, 203, 47);
#endif
}

void _XWindow_PartyMasterMiniBar::SetClassIcon( void )
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
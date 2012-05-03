// XWindow_Party.cpp: implementation of the _XWindow_Party class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Party.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Party::_XWindow_Party()
{	
	m_SelectedTab = _XPARTYTAB_PARTY;	
	m_ChangeTab = TRUE;
	m_SelectedPartyMember = -1;

	for( int i = 0; i < _XPARTYTAB_TOTALCOUNT; i++ )
	{			
		m_PartyTab[i] = NULL;
	}
}

_XWindow_Party::~_XWindow_Party()
{

}

BOOL _XWindow_Party::Initialize( void )
{

	// Create window sub border ---------------------------------------------------------------------------------------
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( -1, 0, 205, 16,
					   &g_MainInterfaceTextureArchive, 
					   g_MainInterfaceTextureArchive.FindResource( "window_upsidebar.tga" ) );	
	InsertChildObject( pTitleBar );
	
	_XImageStatic* pTopBorder = new _XImageStatic;
	pTopBorder->Create( 3, 36, 196, 179,
					   &g_MainInterfaceTextureArchive, 
					   g_MainInterfaceTextureArchive.FindResource( "party_window01.tga" ) );	
	InsertChildObject( pTopBorder );
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create( 3, 180, 196, 326,
					   &g_MainInterfaceTextureArchive, 
					   g_MainInterfaceTextureArchive.FindResource( "party_window02.tga" ) );	
	InsertChildObject( pBottomBorder );	

	// Create party list box window -----------------------------------------------------------------------------------
	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "scrollbar.tga" );
	
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		// window property
		TRUE,														//윈도우 활성화			
		{7, 91},		//윈도우 좌표
		{190, 229},	//윈도우 사이즈
		_XDEF_PARTYWINDOW_LISTBOX,									//윈도우 command id
		-1,															//윈도우 border image
		&g_MainInterfaceTextureArchive,								// Texture list
		_T(""),														//윈도우 title
		46,							
		5,
		_XLSTYLE_RIGHTSCROLLBAR,
		
		// Scroll bar property
		{10, 229},
		10, 
		20,
		100,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex
	};
	
	m_PartyList = new _XListBox;
	m_PartyList->Create(listboxstructure);
	
	POINT coloffset1 = { 0, 0 };
	POINT coloffset2 = {6, 5};
	POINT coloffset3 = {6, 18};
	m_PartyList->SetColumnOffset(0, coloffset1);
	m_PartyList->SetColumnOffset(1, coloffset2);
	m_PartyList->SetColumnOffset(2, coloffset3);
	
	m_PartyList->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_PartyList->SetColumnStyle(1, _XLCOLUMNSTYLE_STRING);
	m_PartyList->SetColumnStyle(2, _XLCOLUMNSTYLE_STRING);
	
	m_PartyList->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);
	m_PartyList->SetAlignType(2, _XFONT_ALIGNTYPE_LEFT);
	
	m_PartyList->SetColumnWidth(1, 76);
	m_PartyList->SetColumnWidth(2, 76);
	
	m_PartyList->SetSelectedImageArchive(-1, &g_MainInterfaceTextureArchive);
	m_PartyList->SetSelectedImagePosition(0, 0);
	m_PartyList->SetSelectedImageHeight(46);
	m_PartyList->SetTrackWidth( 179 );
	
	_XVScrollBar* pScrollBar = m_PartyList->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 10, 10 },
			{ 20, 0, 30, 10 },
			{ 0, 10, 10, 20 },
			{ 20,10, 30, 20 },
			{ 20,20, 30, 40 }			
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	
	InsertChildObject(m_PartyList);
	
	// Create minimize button -----------------------------------------------------------------------------------------
	int closebuttonindex = g_MainInterfaceTextureArchive.FindResource( "window_closebutton.tga" );
	
	_XBTN_STRUCTURE minimizebtnstruct = { TRUE, { 192, 0 },{ 12, 12 }, _XDEF_PARTYWINDOW_CLOSE, 
		closebuttonindex,closebuttonindex,closebuttonindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* minimizebutton = new _XButton;
	minimizebutton->Create( minimizebtnstruct );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,  0, 12, 12 );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  12, 0, 24, 12 );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  24, 0, 36, 16 );
	InsertChildObject( minimizebutton );

	// Create party tab button  ---------------------------------------------------------------------------------------
	_XCHECKBTN_STRUCTURE partytabbtnstruct = 
	{   
		TRUE, { 3, 19 }, { 49, 18 }, 
		_XDEF_PARTYWINDOW_TAB_BANGHOI,
		g_MainInterfaceTextureArchive.FindResource( "party_gauge.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "party_gauge.tga" ), 
		&g_MainInterfaceTextureArchive 
	};

	for( int i = 0; i < _XPARTYTAB_TOTALCOUNT; i++ )
	{			
		m_PartyTab[i] = new _XCheckButton;
		m_PartyTab[i]->Create( partytabbtnstruct );
		m_PartyTab[i]->MoveWindow( 3+(i*50), 19 );
		m_PartyTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 4, 77,  53, 95 );
		m_PartyTab[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  53, 77,  102, 95 );
		m_PartyTab[i]->SetCheck( FALSE );
		InsertChildObject( m_PartyTab[i] );
		
		partytabbtnstruct.commandid++;
	}
		
	m_PartyTab[0]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_PARTY_BANGHOI);	
	m_PartyTab[1]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_PARTY_SEGA );
	m_PartyTab[2]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_PARTY_STUDENT );
	m_PartyTab[3]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_PARTY_PARTY );
	m_PartyTab[3]->SetCheck( TRUE );
	

	// Create child control -------------------------------------------------------------------------------------------
	m_PartyMasterBorder = new _XImageStatic;
	m_PartyMasterBorder->Create( 6, 39, 198, 89,
								 &g_MainInterfaceTextureArchive, 
								 g_MainInterfaceTextureArchive.FindResource( "party_window03.tga" ) );	
	m_PartyMasterBorder->SetClipRect( 0, 0, 193, 51 );
	m_PartyMasterBorder->ShowWindow(FALSE);
	InsertChildObject( m_PartyMasterBorder );


	// Party master 협필살기 상태 아이콘 ------------------------------------------------------------------------------
	m_PartyMasterMortalBlowStatusImage = new _XImageStatic;
	m_PartyMasterMortalBlowStatusImage->Create( 14, 45, 23, 54,
												&g_MainInterfaceTextureArchive, 
												g_MainInterfaceTextureArchive.FindResource( "icon_deathblow.tga" ) );	
	m_PartyMasterMortalBlowStatusImage->SetClipRect( 0, 0, 10, 10 );
	m_PartyMasterMortalBlowStatusImage->ShowWindow(FALSE);
	InsertChildObject( m_PartyMasterMortalBlowStatusImage );
	
	int buttonimageindex = g_MainInterfaceTextureArchive.FindResource( "party_gauge.tga" );
	
	// Party master miniinfo button -----------------------------------------------------------------------------------
	_XCHECKBTN_STRUCTURE miniinfobtnstruct = { TRUE, { 13, 59 }, { 12, 12 }, 
											   _XDEF_PARTYWINDOW_MASTERMINI, 
											   buttonimageindex, buttonimageindex,
											   &g_MainInterfaceTextureArchive };

	m_PartyMasterMiniInfoButton = new _XCheckButton;
	m_PartyMasterMiniInfoButton->Create( miniinfobtnstruct );
	m_PartyMasterMiniInfoButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 0, 26,  12, 38 );
	m_PartyMasterMiniInfoButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  36,26,  48, 38 );		
	m_PartyMasterMiniInfoButton->SetButtonSwapMode( TRUE );
	m_PartyMasterMiniInfoButton->ShowWindow(FALSE);
	InsertChildObject( m_PartyMasterMiniInfoButton );


	// Party master gauge----------------------------------------------------------------------------------------------
	_XSTICKGAUGE_STRUCTURE	gaugestruct = { TRUE, { 111, 46 }, { 83, 7 }, 
		                    buttonimageindex, -1, &g_MainInterfaceTextureArchive };

	m_PartyMasterVitalityGauge = new _XStickGauge;
	m_PartyMasterVitalityGauge->Create( gaugestruct );	
	m_PartyMasterVitalityGauge->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  0, 83, 7 );
	m_PartyMasterVitalityGauge->SetImageClipRect( _XGAUGEIMAGE_OVERLAY, 0,  0, 83, 7 );
	m_PartyMasterVitalityGauge->SetFactorDraw(TRUE);
	m_PartyMasterVitalityGauge->ShowWindow(FALSE);
	InsertChildObject( m_PartyMasterVitalityGauge );

	gaugestruct.position.x = 111;
	gaugestruct.position.y = 61;
	m_PartyMasterZenBorderGauge = new _XStickGauge;
	m_PartyMasterZenBorderGauge->Create( gaugestruct );	
	m_PartyMasterZenBorderGauge->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  7, 83, 14 );
	m_PartyMasterZenBorderGauge->SetImageClipRect( _XGAUGEIMAGE_OVERLAY, 0,  7, 83, 14 );
	m_PartyMasterZenBorderGauge->SetFactorDraw(TRUE);
	m_PartyMasterZenBorderGauge->ShowWindow(FALSE);
	InsertChildObject( m_PartyMasterZenBorderGauge );
	
	_XBTN_STRUCTURE leavebuttonbtnstruct = { TRUE, { 103, 76 }, { 95, 13 }, 
										  _XDEF_PARTYWINDOW_LEAVE,
										  buttonimageindex,
										  buttonimageindex, 
										  buttonimageindex,
										  &g_MainInterfaceTextureArchive };

	m_PartyLeaveButton = new _XButton;
	m_PartyLeaveButton->Create( leavebuttonbtnstruct );
	m_PartyLeaveButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 38, 95, 51 );
	m_PartyLeaveButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 51, 95, 64 );
	m_PartyLeaveButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 64, 95, 76 );
	m_PartyLeaveButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_PARTY_SECESSION);
	m_PartyLeaveButton->ShowWindow(FALSE);
	InsertChildObject( m_PartyLeaveButton );

	_XBTN_STRUCTURE dissolutionbuttonbtnstruct = { TRUE, { 7, 76 }, { 95, 13 }, 
		_XDEF_PARTYWINDOW_DISSOLUTION,
		buttonimageindex,
		buttonimageindex, 
		buttonimageindex,
		&g_MainInterfaceTextureArchive };
	
	m_PartyDissolutionButton = new _XButton;
	m_PartyDissolutionButton->Create( dissolutionbuttonbtnstruct );
	m_PartyDissolutionButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 38, 95, 51 );
	m_PartyDissolutionButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  0, 51, 95, 64 );
	m_PartyDissolutionButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  0, 64, 95, 76 );
	m_PartyDissolutionButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_PARTY_DISSOLUTION);
	m_PartyDissolutionButton->ShowWindow(FALSE);
	InsertChildObject( m_PartyDissolutionButton );

	// Create party user controls--------------------------------------------------------------------------------------

	gaugestruct.position.x = 98;
	gaugestruct.position.y = 97;

	int statusiconindex = g_MainInterfaceTextureArchive.FindResource( "icon_deathblow.tga" );

	for( i = 0 ; i < _XDEF_MAXVIEWPARTYLIST; i++ )
	{
		// Party user gauge--------------------------------------------------------------------------------------------
		m_PartyUserVitalityGauge[i] = new _XStickGauge;
		m_PartyUserVitalityGauge[i]->Create( gaugestruct );	
		m_PartyUserVitalityGauge[i]->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  0, 83, 7 );
		m_PartyUserVitalityGauge[i]->SetImageClipRect( _XGAUGEIMAGE_OVERLAY, 0,  0, 83, 7 );
		m_PartyUserVitalityGauge[i]->SetFactorDraw(TRUE);
		m_PartyUserVitalityGauge[i]->ShowWindow(FALSE);
		InsertChildObject( m_PartyUserVitalityGauge[i] );

		gaugestruct.position.y += 15; 
		m_PartyUserZenGauge[i] = new _XStickGauge;
		m_PartyUserZenGauge[i]->Create( gaugestruct );	
		m_PartyUserZenGauge[i]->SetImageClipRect( _XGAUGEIMAGE_BORDER,  0,  7, 83, 14 );
		m_PartyUserZenGauge[i]->SetImageClipRect( _XGAUGEIMAGE_OVERLAY, 0,  7, 83, 14 );
		m_PartyUserZenGauge[i]->SetFactorDraw(TRUE);
		m_PartyUserZenGauge[i]->ShowWindow(FALSE);
		InsertChildObject( m_PartyUserZenGauge[i] );
		
		gaugestruct.position.y += 31;

		// Party user   협필살기 상태 아이콘 --------------------------------------------------------------------------
		m_PartyUserMortalBlowStatusImage[i] = new _XImageStatic;
		m_PartyUserMortalBlowStatusImage[i]->Create( 97, 123 + (i * 46), 106, 132 + (i * 46),
												     &g_MainInterfaceTextureArchive, 
												     statusiconindex );	
		m_PartyUserMortalBlowStatusImage[i]->SetClipRect( 0, 0, 10, 10 );
		m_PartyUserMortalBlowStatusImage[i]->ShowWindow(FALSE);
		InsertChildObject( m_PartyUserMortalBlowStatusImage[i] );

		// Party user miniinfo button ---------------------------------------------------------------------------------
		_XCHECKBTN_STRUCTURE userminiinfobtnstruct = { TRUE, { 141, 122 + (i * 46) }, { 12, 12 }, 
												       _XDEF_PARTYWINDOW_USERMINI, 
													   buttonimageindex, buttonimageindex,
												      &g_MainInterfaceTextureArchive };
		
		m_PartyUserMiniInfoButton[i] = new _XCheckButton;
		m_PartyUserMiniInfoButton[i]->Create( userminiinfobtnstruct );
		m_PartyUserMiniInfoButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 0, 26,  12, 38 );
		m_PartyUserMiniInfoButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  36,26,  48, 38 );		
		m_PartyUserMiniInfoButton[i]->SetButtonSwapMode( TRUE );
		m_PartyUserMiniInfoButton[i]->ShowWindow(FALSE);
		InsertChildObject( m_PartyUserMiniInfoButton[i] );

		// Party user leave button ---------------------------------------------------------------------------------		
		_XBTN_STRUCTURE secessionbuttonbtnstruct = { TRUE, { 157, 122  + (i * 46) }, { 12, 12 }, 
													  _XDEF_PARTYWINDOW_USERSECESSION,
													  buttonimageindex,
													  buttonimageindex, 
													  buttonimageindex,
													 &g_MainInterfaceTextureArchive };
		
		m_PartyUserSecessionButton[i] = new _XButton;
		m_PartyUserSecessionButton[i]->Create( secessionbuttonbtnstruct );
		m_PartyUserSecessionButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 14, 12, 26 );
		m_PartyUserSecessionButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  12,14, 24, 26 );
		m_PartyUserSecessionButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  24,14, 36, 26 );
		m_PartyUserSecessionButton[i]->ShowWindow(FALSE);
		InsertChildObject( m_PartyUserSecessionButton[i] );

		// Party user secession button --------------------------------------------------------------------------------		
		_XBTN_STRUCTURE leavebuttonbtnstruct = { TRUE, { 170, 122  + (i * 46) }, { 12, 12 }, 
												  _XDEF_PARTYWINDOW_USERLEAVE,
												  buttonimageindex,
												  buttonimageindex, 
												  buttonimageindex,
												  &g_MainInterfaceTextureArchive };
		
		m_PartyUserLeaveButton[i] = new _XButton;
		m_PartyUserLeaveButton[i]->Create( leavebuttonbtnstruct );
		m_PartyUserLeaveButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 36, 14,48, 26 );
		m_PartyUserLeaveButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  48,14, 60, 26 );
		m_PartyUserLeaveButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  60,14, 72, 26 );
		m_PartyUserLeaveButton[i]->ShowWindow(FALSE);
		InsertChildObject( m_PartyUserLeaveButton[i] );

	}
	
	return TRUE;
}

void _XWindow_Party::SetDefaultPosition( void )
{	
	MoveWindow( gnWidth-204, (gnHeight>>1)-205 );
	CheckOutside();
}


void _XWindow_Party::DestroyWindow(void)
{	
	_XWindow::DestroyWindow();
}

void _XWindow_Party::MoveWindow( int X, int Y )
{		
	_XWindow::MoveWindow( X, Y );	
}


void _XWindow_Party::Draw( _XGUIObject*& pfocusobject )
{	
	_XWindow::Draw(pfocusobject);
	
	if( !this->m_ShowWindow )	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	if( m_PartyList->GetItemCount() > 0)
	{
		int startnumber = m_PartyList->GetScrollPos();
		int count = 0;// 2004.06.04->oneway48 insert
				
		for( int i = 0; i < _XDEF_MAXVIEWPARTYLIST; i++ )
		{
			if( i >= startnumber && i < startnumber + 5 )
			{			
				if( i < m_PartyList->GetItemCount() )
				{
					m_PartyUserVitalityGauge[i]->ShowWindow( TRUE );
					m_PartyUserZenGauge[i]->ShowWindow( TRUE );
					m_PartyUserMortalBlowStatusImage[i]->ShowWindow( TRUE );
					m_PartyUserMiniInfoButton[i]->ShowWindow( TRUE );
					m_PartyUserLeaveButton[i]->ShowWindow( TRUE );
					m_PartyUserSecessionButton[i]->ShowWindow( TRUE );
					
					// 2004.06.04->oneway48 modify
					m_PartyUserVitalityGauge[i]->MoveWindow( m_WindowPosition.x + 99, m_WindowPosition.y + 97 + (count * 46) );
					m_PartyUserZenGauge[i]->MoveWindow( m_WindowPosition.x + 99, m_WindowPosition.y + 112 + (count * 46));
					m_PartyUserMortalBlowStatusImage[i]->MoveWindow( m_WindowPosition.x + 97, m_WindowPosition.y + 123 + (count * 46) );
					m_PartyUserMiniInfoButton[i]->MoveWindow( m_WindowPosition.x + 141, m_WindowPosition.y + 122 + (count * 46) );
					m_PartyUserLeaveButton[i]->MoveWindow( m_WindowPosition.x + 170, m_WindowPosition.y + 122  + (count * 46) );
					m_PartyUserSecessionButton[i]->MoveWindow( m_WindowPosition.x + 157, m_WindowPosition.y + 122  + (count * 46) );	
					count++;
				}				
			}
			else
			{
				m_PartyUserVitalityGauge[i]->ShowWindow( FALSE );
				m_PartyUserZenGauge[i]->ShowWindow( FALSE );
				m_PartyUserMortalBlowStatusImage[i]->ShowWindow( FALSE );
				m_PartyUserMiniInfoButton[i]->ShowWindow( FALSE );
				m_PartyUserLeaveButton[i]->ShowWindow( FALSE );
				m_PartyUserSecessionButton[i]->ShowWindow( FALSE );
			}
		}
	}
		
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+15, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	
	if(m_ChangeTab)
	{
		switch(m_SelectedTab)
		{
		case _XPARTYTAB_BANGHOI :
			SetBanghoiList();
			m_ChangeTab = FALSE;
			break;
		case _XPARTYTAB_SEGA :
			SetSegaList();
			m_ChangeTab = FALSE;
			break;
		case _XPARTYTAB_PARTY :
			SetPartyList();
			m_ChangeTab = FALSE;
			break;
		case _XPARTYTAB_FRIEND :
			SetFriendList();
			m_ChangeTab = FALSE;
			break;
		}
	}	


	// 파티장 정보

	if( m_PartyList->GetItemCount() )
	{
		TCHAR namestring[128];
		TCHAR levelstring[128];
		TCHAR buffer[128];

		memset(namestring, 0, sizeof(TCHAR)*128);
		memset(levelstring, 0, sizeof(TCHAR)*128);		
		memset(buffer, 0, sizeof(TCHAR)*128);
		
		BOOL findflag = FALSE;

		for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) // 2004.06.01->oneway48 modify : 15->9
		{
			if(g_pLocalUser->m_PartyInfo[i].isLeader)
			{				
				if(g_pLocalUser->m_PartyInfo[i].playername[0] != 0)
					strcpy(namestring, g_pLocalUser->m_PartyInfo[i].playername);

				DWORD levelmager = 0;
				DWORD levelminor = 0;
				
				_XLocalUser::GetUserLevel( g_pLocalUser->m_PartyInfo[i].innerlevel, levelmager, levelminor );

				m_PartyMasterVitalityGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[i].vitalpercent);
				m_PartyMasterZenBorderGauge->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[i].zenpercent);
				
				switch(levelmager)
				{
				default :
				case 0 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR));
					break;
				case 1 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_01));
					break;
				case 2 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_02));
					break;
				case 3 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_03));
					break;
				case 4 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_04));
					break;
				case 5 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_05));
					break;
				case 6 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_06));
					break;
				case 7 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_07));
					break;
				case 8 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_08));
					break;
				case 9 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_09));
					break;
				case 10 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_10));
					break;
				case 11 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_11));
					break;
				case 12 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_12));
					break;
				case 13 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_13));
					break;
				case 14 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_14));
					break;
				case 15 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_15));
					break;
				case 16 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_16));
					break;
				case 17 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_17));
					break;
				case 18 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_18));
					break;
				case 19 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_19));
					break;
				case 20 :
					strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_20));
					break;
				}
								
				sprintf(buffer, "%s %d%s", levelstring, levelminor, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));				

				findflag = TRUE;
				break;
			}
		}

		if( findflag )
		{
			g_XBaseFont->SetColor( _XSC_DEFAULT );
			g_XBaseFont->Puts( m_WindowPosition.x + 27, m_WindowPosition.y + 45, namestring ); 
			g_XBaseFont->Puts(m_WindowPosition.x + 27, m_WindowPosition.y + 58, buffer);
			g_XBaseFont->Flush();
		}
	}
}

BOOL _XWindow_Party::Process( _XGUIObject*& pfocusobject  )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;
	
	
	if( m_PartyList->GetItemCount() > 0)
	{
		
		int startnumber =  m_PartyList->GetScrollPos();
		
		for(int i = 0 ; i < _XDEF_MAXVIEWPARTYLIST ; i++)
		{
			if( i >= startnumber && i < startnumber + 5 )
			{
				m_PartyUserMiniInfoButton[i]->EnableWindow( TRUE );
				m_PartyUserSecessionButton[i]->EnableWindow( TRUE );
				m_PartyUserLeaveButton[i]->EnableWindow( TRUE );
			}
			else
			{
				m_PartyUserMiniInfoButton[i]->EnableWindow(FALSE);
				m_PartyUserSecessionButton[i]->EnableWindow(FALSE);
				m_PartyUserLeaveButton[i]->EnableWindow(FALSE);
			}
		}
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		
		int selectedlistnumber = -1;
		int rowpoint = m_WindowPosition.y + 91;	
		int count = 0;
		
		for( int row = 0; row < 5 ; row++ )
		{
			if( ((scrnpos->x > m_WindowPosition.x + 7  ) && (scrnpos->x < m_WindowPosition.x + 179) && 
				(scrnpos->z > rowpoint )    && (scrnpos->z < rowpoint+46) ) )
			{
				selectedlistnumber = count;
			}
			rowpoint += 46;
			count++;
			if( selectedlistnumber != -1 ) break;
		}
		
		if( selectedlistnumber != -1 )
		{
			if( m_PartyList->GetSelectedItem() != ( selectedlistnumber + startnumber )  )
				m_PartyList->SelectItem( selectedlistnumber + startnumber );
			
		}
/*
		if(mousestate)
		{
			if(mousestate->bButton[0])
			{				
				m_WindowMouseState = _XW_STATE_LEFTCLICK; 
			}

			if( !mousestate->bButton[0] )
			{
				
				if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
				{
					int selectedlistnumber = -1;
					int rowpoint = m_WindowPosition.y + 91;	
					int count = 0;
					
					for( int row = 0; row < 5 ; row++ )
					{
						if( ((scrnpos->x > m_WindowPosition.x + 7  ) && (scrnpos->x < m_WindowPosition.x + 179) && 
							(scrnpos->z > rowpoint )    && (scrnpos->z < rowpoint+46) ) )
						{
							selectedlistnumber = count;
						}
						rowpoint += 15;
						count++;
						if( selectedlistnumber != -1 ) break;
					}
					
					if( selectedlistnumber != -1 )
					{
						m_PartyList->SelectItem( selectedlistnumber + startnumber );
						
					}
					m_WindowMouseState = _XW_STATE_NONE;
				}
			}
		}
*/
		

/*
		int startnumber = m_PartyList->GetScrollPos();
		
		for( int i = 0; i < _XDEF_MAXVIEWPARTYLIST; i++ )
		{
			if( i >= startnumber && i < startnumber + 5 )
			{			
				if( i < m_PartyList->GetItemCount() )
				{
					m_PartyUserMiniInfoButton[i]->EnableWindow( TRUE );
					m_PartyUserSecessionButton[i]->EnableWindow( TRUE );
					m_PartyUserLeaveButton[i]->EnableWindow( TRUE );
				}				
			}
			else
			{
				m_PartyUserMiniInfoButton[i]->EnableWindow(FALSE);
				m_PartyUserSecessionButton[i]->EnableWindow(FALSE);
				m_PartyUserLeaveButton[i]->EnableWindow(FALSE);
			}
		}
*/
	}
	
	
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}

void _XWindow_Party::ChangeTab(_XPartyWindowTab selectedtab)
{
	m_SelectedTab = selectedtab;
	m_ChangeTab = TRUE;

	for( int i = 0; i < 4; i++ )
	{			
		m_PartyTab[i]->SetCheck( i == m_SelectedTab );
	}	
}

// ====================================================================================================================
// 각 커뮤티티별 유저리스트를 설정 
void _XWindow_Party::SetBanghoiList(void)
{
	m_PartyList->DeleteAllItem();

	SetUserButtonEnable();
}


void _XWindow_Party::SetSegaList(void)
{
	m_PartyList->DeleteAllItem();

	SetUserButtonEnable();
}


void _XWindow_Party::SetPartyList(void)
{
	if( !m_PartyList ) return;

	int selecteditem = m_PartyList->GetSelectedItem();

	m_PartyList->DeleteAllItem();

	int		ListItemEdgeIconIndex = g_MainInterfaceTextureArchive.FindResource( "party_window03.tga" );
	RECT	rect = { 0, 51, 179, 96 };

	int		partycount = 0;	
	int startnumber =  m_PartyList->GetScrollPos();

	for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) // 2004.06.01->oneway48 modify : 15->9
	{
		if(g_pLocalUser->m_PartyInfo[i].playername[0] != 0)
		{
			if(!g_pLocalUser->m_PartyInfo[i].isLeader)
			{
				m_PartyList->InsertListItem(_T(""), 0, partycount+1);	//파티원 ID를 넣어준다.
					
				m_PartyList->SetIconArchive(partycount, &g_MainInterfaceTextureArchive );
				m_PartyList->SetItemAttrib(partycount, 0, ListItemEdgeIconIndex, rect);
					
				m_PartyList->SetItemText(partycount, 1, g_pLocalUser->m_PartyInfo[i].playername);
				m_PartyList->SetItemAttrib(partycount, 1, _XSC_DEFAULT);
				m_PartyList->SetItemAttribHighlight(partycount, 1, _XSC_DEFAULT_HIGHLIGHT);

				if(g_pLocalUser->m_PartyInfo[i].serverno != g_pLocalUser->m_CharacterInfo.zoneuniqid)
				{
					m_PartyList->SetItemText(partycount, 2, g_ZoneInfoTable[g_pLocalUser->m_PartyInfo[i].serverno - 1].zonename);
					m_PartyList->SetItemAttrib(partycount, 2, _XSC_DEFAULT);
					m_PartyList->SetItemAttribHighlight(partycount, 2, _XSC_DEFAULT_HIGHLIGHT);
					m_PartyUserVitalityGauge[partycount]->SetGaugeFactor(0);					
					m_PartyUserZenGauge[partycount]->SetGaugeFactor(0);
				}
				else
				{
					DWORD levelmager = 0;
					DWORD levelminor = 0;
					
					_XLocalUser::GetUserLevel( g_pLocalUser->m_PartyInfo[i].innerlevel, levelmager, levelminor );
					
					
				
					m_PartyUserVitalityGauge[partycount]->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[i].vitalpercent);					
					m_PartyUserZenGauge[partycount]->SetGaugeFactor((FLOAT)g_pLocalUser->m_PartyInfo[i].zenpercent);
					if( partycount >= startnumber && partycount < startnumber + 5)
					{
						m_PartyUserVitalityGauge[partycount]->ShowWindow(TRUE);
						m_PartyUserZenGauge[partycount]->ShowWindow(TRUE);
					}
					else
					{
						m_PartyUserVitalityGauge[partycount]->ShowWindow(FALSE);
						m_PartyUserZenGauge[partycount]->ShowWindow(FALSE);
					}
					

					TCHAR levelstring[128];
					 
					switch(levelmager)
					{
					default :
					case 0 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_NOCHAR));
						break;
					case 1 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_01));
						break;
					case 2 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_02));
						break;
					case 3 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_03));
						break;
					case 4 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_04));
						break;
					case 5 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_05));
						break;
					case 6 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_06));
						break;
					case 7 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_07));
						break;
					case 8 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_08));
						break;
					case 9 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_09));
						break;
					case 10 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_10));
						break;
					case 11 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_11));
						break;
					case 12 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_12));
						break;
					case 13 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_13));
						break;
					case 14 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_14));
						break;
					case 15 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_15));
						break;
					case 16 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_16));
						break;
					case 17 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_17));
						break;
					case 18 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_18));
						break;
					case 19 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_19));
						break;
					case 20 :
						strcpy(levelstring, _XGETINTERFACETEXT(ID_STRING_LEVELNAME_20));
						break;
					}
					
					TCHAR buffer[128];
					sprintf(buffer, "%s %d%s", levelstring, levelminor, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));
					
					m_PartyList->SetItemText(partycount, 2, buffer);
					m_PartyList->SetItemAttrib(partycount, 2, _XSC_DEFAULT);
					m_PartyList->SetItemAttribHighlight(partycount, 2, _XSC_DEFAULT_HIGHLIGHT);
					
				}
				partycount++;
			}
		}
	}

	if( partycount != 0 )
	{
		m_PartyList->SelectItem(selecteditem);	
	}	
	SetUserButtonEnable();	
}

void _XWindow_Party::SetFriendList(void)
{
	m_PartyList->DeleteAllItem();


	SetUserButtonEnable();
}

// ====================================================================================================================
// 파티가 생성됬는지 체크하고 버튼들을 활성/비활성 한다.
void _XWindow_Party::SetUserButtonEnable( void )
{
	int itemcount = m_PartyList->GetItemCount();

	BOOL checkpartyusers = m_PartyList->GetItemCount() > 0 ? TRUE : FALSE;

	m_PartyMasterBorder->ShowWindow( checkpartyusers );
	m_PartyMasterMortalBlowStatusImage->ShowWindow( checkpartyusers );
	m_PartyMasterMiniInfoButton->ShowWindow( checkpartyusers );
	m_PartyMasterVitalityGauge->ShowWindow( checkpartyusers );
	m_PartyMasterZenBorderGauge->ShowWindow( checkpartyusers );
	m_PartyLeaveButton->ShowWindow( checkpartyusers );
	
	for( int count = 0 ; count < _XDEF_MAXVIEWPARTYLIST; count++ )
	{
		BOOL showflag = ( count < m_PartyList->GetItemCount() );

		m_PartyUserVitalityGauge[count]->ShowWindow( showflag );
		m_PartyUserZenGauge[count]->ShowWindow( showflag );
		m_PartyUserMortalBlowStatusImage[count]->ShowWindow( showflag );
		m_PartyUserMiniInfoButton[count]->ShowWindow( showflag );
		m_PartyUserLeaveButton[count]->ShowWindow( showflag );
		m_PartyUserSecessionButton[count]->ShowWindow( showflag );
	}

	if(checkpartyusers)
	{
		for(int i = 0 ; i < _XDEF_MAXPARTYMEMBERCOUNT ; i++) // 2004.06.01->oneway48 modify : 15->9
		{
			if(g_pLocalUser->m_PartyInfo[i].isLeader)
			{
				if(strcmp(g_pLocalUser->m_PartyInfo[i].playername, g_pLocalUser->m_CharacterInfo.charactername) == 0)
				{
					m_PartyDissolutionButton->ShowWindow(TRUE);
					break;
				}
			}
		}
		if(i >= 9)
		{
			m_PartyDissolutionButton->ShowWindow(FALSE);
		}
	}
	else
	{
		m_PartyDissolutionButton->ShowWindow(FALSE);
	}
}

void _XWindow_Party::ShowWindow(BOOL show)
{
	if(!show)
		m_SelectedPartyMember = -1;
	else
	{
		//MoveWindow( gnWidth-204, (gnHeight>>1)-205 );
		SetPartyList();
	}

	_XWindow::ShowWindow(show);
}
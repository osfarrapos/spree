// XWindow_MainFrame.cpp: implementation of the _XWindow_MainFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_MainFrame.h"


#include "XWindow_CInfo.h"
#include "XWindow_QuestWindow.h"
#include "XWindow_MasterySkill.h"
#include "XWindow_LifeSkill.h"
#include "xwindow_inventory.h"
#include "XWindow_Skill.h"
#include "XWindow_Group.h"
#include "XWindow_SocketPlugIn.h"
#include "XWindow_TutorialIcon.h"
#include "XWindow_NickName.h"
#include "XWindow_PKRecord.h"
#include "XWindowObjectDefine.h"

_XWindow_QuestWindow*	g_pQuest_Window = NULL;
_XWindow_CInfo*			g_pCInfo_Window = NULL;
_XWindow_MasterySkill*	g_pMasterySkill_Window = NULL;
_XWindow_Skill*			g_pSkill_Window = NULL;
_XWindow_LifeSkill*		g_pLifeSkill_Window = NULL;
_XWindow_Group*			g_pGroup_Window = NULL;
_XWindow_PKRecord*		g_pPKRecord_Window = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MainFrame::_XWindow_MainFrame()
{
	for( int i = 0; i < _XDEF_MAXWINDOWTABCOUNT; i++ )
	{
		m_pWindowTabbutton[i] = NULL;
		m_pWindowObject[i] = NULL;
	}
	
	m_SelectedTab = 0;
}

_XWindow_MainFrame::~_XWindow_MainFrame()
{

}

BOOL _XWindow_MainFrame::Initialize( void )
{	
	int mainframeinterface = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );

	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 175, 23,
						   &g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleLeftBar->SetClipRect( 0, 0, 176, 24 );
	InsertChildObject( pTitleLeftBar );

	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create( 352, 0, 352, 23,
							&g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleRightBar->SetClipRect( 0, 0, 176, 24 );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );

	
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 335, 5 }, { 12, 12 }, _XDEF_MAINFRAME_CLOSEBUTTON,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
		
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 322, 5 }, { 12, 12 }, _XDEF_MAINFRAME_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	

	FLOAT ufactor = 0.0f;
	FLOAT vfactor = 0.0f;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_BorderTile.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 351, m_WindowPosition.y + 450);
		ufactor = 350.0f/128.0f;
		vfactor = 426.0f/128.0f;
	}
	else
	{
		m_BorderTile.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 351, m_WindowPosition.y + 440);
		ufactor = 350.0f/128.0f;
		vfactor = 416.0f/128.0f;
	}
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );

	m_BorderGradationTitle.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 351, m_WindowPosition.y + 20, 
								  &g_MainInterfaceTextureArchive, mainframeinterface );
	m_BorderGradationTitle.SetClipRect( 206, 227, 244, 246 );
	m_BorderGradationTitle.SetScale( 359.5f/39.0f, 1.0f );


	int szButtonstring[7] = 
	{
		ID_STRING_MAINFRAME_CINFO, // _T("인물"),
#ifdef _XTS_PKRECORD
		ID_STRING_NEW_2648,		   // 전적
#else
		ID_STRING_MAINFRAME_GROUP, // _T("문파"),
#endif
		ID_STRING_MAINFRAME_MASTERYSKILL, // _T("오의"),
		ID_STRING_MAINFRAME_SKILL, // _T("무공"),
		ID_STRING_MAINFRAME_LIFESKILL, // _T("생활"),
		ID_STRING_NEW_2953, // _T("강호풍운록"),
		ID_STRING_MAINFRAME_CLAN, // _T("군웅")
	};
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		int gminterfaceimage = g_MainInterfaceTextureArchive.FindResource( "MI_GMWindow.tga" );

		_XCHECKBTN_STRUCTURE windowtabbuttonstruct = { TRUE, { 0, 414 }, { 88, 20 }, 
			_XDEF_MAINFRAME_CHARACTORTAB,
			gminterfaceimage,
			gminterfaceimage, 
			&g_MainInterfaceTextureArchive };
		
		for( int i = 0; i < _XDEF_MAXWINDOWTABCOUNT; i++ )
		{	
			m_pWindowTabbutton[i] = new _XCheckButton;
			
			if( i < 4 )
			{
				m_pWindowTabbutton[i]->Create( windowtabbuttonstruct );
				m_pWindowTabbutton[i]->SetButtonSwapMode( TRUE );
				m_pWindowTabbutton[i]->SetCheck( FALSE );
				m_pWindowTabbutton[i]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR,  szButtonstring[i] );
				m_pWindowTabbutton[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
				m_pWindowTabbutton[i]->SetGrowTextMode( TRUE, 0xFF000000 );
				
				windowtabbuttonstruct.position.x += 88;
				m_pWindowTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 0, 154,  88, 174 );
				m_pWindowTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  0, 134,  88, 154 );

				if( i == 3 )
				{
					windowtabbuttonstruct.position.x = 0;
					windowtabbuttonstruct.position.y += 19;
				}
			}
			else
			{
				windowtabbuttonstruct.windowsize.cx = 117;
				
				m_pWindowTabbutton[i]->Create( windowtabbuttonstruct );		
				m_pWindowTabbutton[i]->SetButtonSwapMode( TRUE );
				m_pWindowTabbutton[i]->SetCheck( FALSE );
				m_pWindowTabbutton[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, szButtonstring[i] );
				m_pWindowTabbutton[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
				m_pWindowTabbutton[i]->SetGrowTextMode( TRUE, 0xFF000000 );
				
				windowtabbuttonstruct.position.x += 117;
				m_pWindowTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 88, 154,  205, 174 );
				m_pWindowTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  88, 134,  205, 154 );
			}			
			
			InsertChildObject( m_pWindowTabbutton[i] );
			
			windowtabbuttonstruct.commandid++;
		}

	}
	else
	{
		_XCHECKBTN_STRUCTURE windowtabbuttonstruct = { TRUE, { 0, 414 }, { 45, 28 }, 
													   _XDEF_MAINFRAME_CHARACTORTAB,
													   mainframeinterface,
													   mainframeinterface, 
													   &g_MainInterfaceTextureArchive };

		for( int i = 0; i < _XDEF_MAXWINDOWTABCOUNT; i++ )
		{	
			m_pWindowTabbutton[i] = new _XCheckButton;
			
			if( windowtabbuttonstruct.commandid == _XDEF_MAINFRAME_QUESTTAB )
			{
				windowtabbuttonstruct.windowsize.cx = 81;

				m_pWindowTabbutton[i]->Create( windowtabbuttonstruct );		
				m_pWindowTabbutton[i]->SetButtonSwapMode( TRUE );
				m_pWindowTabbutton[i]->SetCheck( FALSE );
				m_pWindowTabbutton[i]->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, szButtonstring[i] );
				m_pWindowTabbutton[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
				m_pWindowTabbutton[i]->SetGrowTextMode( TRUE, 0xFF000000 );

				windowtabbuttonstruct.position.x += 81;
				m_pWindowTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 101, 87,  182, 115 );
				m_pWindowTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  101, 57,  182, 85 );

				windowtabbuttonstruct.windowsize.cx = 45;
			}
			else
			{
				m_pWindowTabbutton[i]->Create( windowtabbuttonstruct );		
				m_pWindowTabbutton[i]->SetButtonSwapMode( TRUE );
				m_pWindowTabbutton[i]->SetCheck( FALSE );
				m_pWindowTabbutton[i]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR,  szButtonstring[i] );
				m_pWindowTabbutton[i]->SetButtonTextColor( 0xFF747474, 0xFFFFFFFF );
				m_pWindowTabbutton[i]->SetGrowTextMode( TRUE, 0xFF000000 );

				windowtabbuttonstruct.position.x += 45;
				m_pWindowTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 101, 25,  146, 53 );
				m_pWindowTabbutton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  147, 25,  192, 53 );
			}

			InsertChildObject( m_pWindowTabbutton[i] );

			windowtabbuttonstruct.commandid++;
		}

#ifdef _XJAPANESE	// 일본 - 전적창 미 포함
		m_pWindowTabbutton[1]->EnableWindow(FALSE);
#endif
	}


	// 캐릭터 정보 창
	g_pCInfo_Window = new _XWindow_CInfo;
	_XWINDOW_STRUCTURE characterinfowindowstruct = 
	{
		TRUE, { 3, 24} , { 346, 390 }, 0, 
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB( 127, 0, 0, 0 ),
		_XDEF_WTITLE_CHARACTERINFO
	};
	g_pCInfo_Window->Create( characterinfowindowstruct ); 
	g_pCInfo_Window->Initialize();
	g_pCInfo_Window->ShowWindow( FALSE );
	g_pCInfo_Window->EnableWindowEffect( FALSE );
	g_pCInfo_Window->SetDocking( FALSE, 0 );
	g_pCInfo_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	m_pWindowObject[0] = g_pCInfo_Window;

	InsertChildObject( g_pCInfo_Window );

#ifdef _XTS_PKRECORD
	// 전적창
	g_pPKRecord_Window = new _XWindow_PKRecord;
	_XWINDOW_STRUCTURE pkrwindowstruct = 
	{
		TRUE, {3, 24}, {346, 390}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_PKRECORD
	};
	g_pPKRecord_Window->Create(pkrwindowstruct);
	g_pPKRecord_Window->Initialize();
	g_pPKRecord_Window->ShowWindow(FALSE);
	g_pPKRecord_Window->EnableWindowEffect(FALSE);
	g_pPKRecord_Window->SetDocking(FALSE, 0);
	g_pPKRecord_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	m_pWindowObject[1] = g_pPKRecord_Window;
	
	InsertChildObject(g_pPKRecord_Window);
#endif

	// 오의창
	g_pMasterySkill_Window = new _XWindow_MasterySkill;
	_XWINDOW_STRUCTURE masterywindowstruct = 
	{
		TRUE, {3, 24}, {346, 390}, 0,
		-1,
		&g_MainInterfaceTextureArchive,
		D3DCOLOR_ARGB(0, 0, 0, 0),
		_XDEF_WTITLE_MASTERYSKILL
	};
	g_pMasterySkill_Window->Create(masterywindowstruct);
	g_pMasterySkill_Window->Initialize();
	g_pMasterySkill_Window->ShowWindow(FALSE);
	g_pMasterySkill_Window->EnableWindowEffect(FALSE);
	g_pMasterySkill_Window->SetDocking(FALSE, 0);
	g_pMasterySkill_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	m_pWindowObject[2] = g_pMasterySkill_Window;

	InsertChildObject(g_pMasterySkill_Window);

	// 스킬 창
	g_pSkill_Window = new _XWindow_Skill;
	_XWINDOW_STRUCTURE skillstruct = 
	{
		TRUE, {3, 24}, {346, 390}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_SKILL
	};
	g_pSkill_Window->Create(skillstruct);
	g_pSkill_Window->Initialize();
	g_pSkill_Window->ShowWindow(FALSE);
	g_pSkill_Window->EnableWindowEffect(FALSE);
	g_pSkill_Window->SetDocking(FALSE, 0);
	g_pSkill_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	m_pWindowObject[3] = g_pSkill_Window;
	
	InsertChildObject(g_pSkill_Window);
	
	// 생활 창
	g_pLifeSkill_Window = new _XWindow_LifeSkill;
	_XWINDOW_STRUCTURE lifestruct = 
	{
		TRUE, {3, 24}, {346, 390}, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB(0, 0, 0, 0),
			_XDEF_WTITLE_LIFESKILL
	};
	g_pLifeSkill_Window->Create(lifestruct);
	g_pLifeSkill_Window->Initialize();
	g_pLifeSkill_Window->ShowWindow(FALSE);
	g_pLifeSkill_Window->EnableWindowEffect(FALSE);
	g_pLifeSkill_Window->SetDocking(FALSE, 0);
	g_pLifeSkill_Window->SetWindowMoveMode(_XWMOVE_FIXED);
	m_pWindowObject[4] = g_pLifeSkill_Window;
	
	InsertChildObject(g_pLifeSkill_Window);
	
	// 퀘스트 창 
	g_pQuest_Window = new _XWindow_QuestWindow;
	_XWINDOW_STRUCTURE questwindowstruct = 
	{
		TRUE, { 3, 24 }, { 346, 390 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ),
			_XDEF_WTITLE_QUESTWINDOW
	};
	g_pQuest_Window->Create( questwindowstruct );
	g_pQuest_Window->Initialize();
	g_pQuest_Window->ShowWindow( FALSE );
	g_pQuest_Window->EnableWindowEffect( FALSE );
	g_pQuest_Window->EnableWindowEffect( FALSE );
	g_pQuest_Window->SetDocking( FALSE, 0 );
	g_pQuest_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	m_pWindowObject[5] = g_pQuest_Window;
	
	InsertChildObject( g_pQuest_Window );


	g_pGroup_Window = new _XWindow_Group;
	_XWINDOW_STRUCTURE groupwindowstruct = 
	{
		TRUE, { 3, 24 }, { 346, 390 }, 0,
			-1,
			&g_MainInterfaceTextureArchive,
			D3DCOLOR_ARGB( 0, 0, 0, 0 ),
			_XDEF_WTITLE_GROUPWINDOW
	};
	g_pGroup_Window->Create( groupwindowstruct );
	g_pGroup_Window->Initialize();
	g_pGroup_Window->ShowWindow( FALSE );
	g_pGroup_Window->EnableWindowEffect( FALSE );
	g_pGroup_Window->EnableWindowEffect( FALSE );
	g_pGroup_Window->SetDocking( FALSE, 0 );
	g_pGroup_Window->SetWindowMoveMode( _XWMOVE_FIXED );
	m_pWindowObject[6] = g_pGroup_Window;
	
	InsertChildObject( g_pGroup_Window );

#ifdef _XTS_PKRECORD
#else
	m_pWindowTabbutton[1]->EnableWindow( FALSE );
#endif

	ChangeWindowTab( _XDEF_MAINFRAME_CHARACTORTAB );


	return TRUE;
}


void _XWindow_MainFrame::SetDefaultPosition( void )
{		
	#ifdef _ACCLAIM_RUBICONADSYSTEM	
	if( g_ViewRubiconAD )
	{
		MoveWindow( 68, 114 + g_RubiconADWindowSize.cy );
	}
	else
	{
		MoveWindow( 68, 114 );
	}
	#else
		MoveWindow( 68, 114 );
	#endif
}

void _XWindow_MainFrame::SaveWindowProperty( void )
{
	
}


void _XWindow_MainFrame::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_MainFrame::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y+23, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));
		m_BorderGradationTitle.Draw( pfocusobject );
	}

	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// erase first button dot...
	_XDrawLine2D( m_WindowPosition.x, m_pWindowTabbutton[0]->GetWindowPos().y, m_WindowPosition.x, m_pWindowTabbutton[0]->GetWindowPos().y+3, 0XFF000000, 0XFF000000 );

	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFFFFFFF);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, m_WindowTitle );
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1)+1, m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, m_WindowTitle );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
}



BOOL _XWindow_MainFrame::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)		return FALSE;
	if(!this->m_ShowWindow) 
	{
		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )
			{			
				if( !g_pInventory_Window->GetShowStatus() )	
				{
					_XWindow_NPCScript* pNPCScript_Window = (_XWindow_NPCScript*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCSCRIPT);
					if( !pNPCScript_Window )
					{
						g_pDefaultTooltip_Window->ShowWindow( FALSE );						
					}
				}
			}
		}
		return FALSE;		
	}


	if(!_XWindow::Process(pfocusobject)) return FALSE;
	
	return TRUE;
}

void _XWindow_MainFrame::MoveWindow(int X, int Y)
{	
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
	_XWindow::MoveWindow(X, Y);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 351, m_WindowPosition.y + 450 );
	else
		m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 351, m_WindowPosition.y + 440 );

	m_BorderGradationTitle.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24 );
#else
	_XWindow::MoveWindow(X, Y);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 351, m_WindowPosition.y + 450 );
	else
		m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 351, m_WindowPosition.y + 440 );

	m_BorderGradationTitle.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 24 );
#endif
}

void _XWindow_MainFrame::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow(show);

	_XWindow_NickName* pNickname_Window = (_XWindow_NickName*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NICKNAME);
	if( pNickname_Window )
	{
		if( pNickname_Window->GetShowStatus() )
			pNickname_Window->ShowWindow(FALSE);
	}
}

void _XWindow_MainFrame::ChangeWindowTab( DWORD windowid )
{
	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
	if( g_TutorialInfo )
	{
		if( windowid == _XDEF_MAINFRAME_SKILLTAB && !g_TutorialInfo->_XTutorial_004 && (g_pLocalUser->m_SkillList[0].m_sSkillID > 0) )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_004);
			g_TutorialInfo->_XTutorial_004 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif			
		}
		if( !g_TutorialInfo->_XTutorial_041 )
		{
			if( windowid == _XDEF_MAINFRAME_SKILLTAB )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_041);
				g_TutorialInfo->_XTutorial_041 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}			
		}
		if( !g_TutorialInfo->_XTutorial_046 )
		{
			if( windowid == _XDEF_MAINFRAME_LIFETAB )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_046);
				g_TutorialInfo->_XTutorial_046 = TRUE;
#ifdef _XDEF_HELPSYSTEMSERVERSAVE							
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 11 );
#endif
			}			
		}		
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=
	for( int i = 0; i < _XDEF_MAXWINDOWTABCOUNT; i++ )
	{
		if( m_pWindowTabbutton[i]->GetCommandID() == windowid )
		{
			if( m_pWindowObject[i] )
			{
				m_pWindowObject[i]->ShowWindow( TRUE );
			}

			m_pWindowTabbutton[i]->SetCheck( TRUE );

			m_SelectedTab = i;

			memset( m_WindowTitle, 0, _XDEF_WINDOWTITLESTRINGSIZE );
			
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			switch(windowid)
			{
			case _XDEF_MAINFRAME_CHARACTORTAB :
				strcpy( m_WindowTitle, _XGETINTERFACETEXT(ID_STRING_MAINMENU_CINFO) );
				break;
			case _XDEF_MAINFRAME_PKRECORDTAB  :
				strcpy( m_WindowTitle, _XGETINTERFACETEXT(ID_STRING_NEW_3671) );
				break;
			case _XDEF_MAINFRAME_MASTERYTAB   :
				strcpy( m_WindowTitle, _XGETINTERFACETEXT(ID_STRING_NEW_3672) );
			    break;
			case _XDEF_MAINFRAME_SKILLTAB     :
				strcpy( m_WindowTitle, _XGETINTERFACETEXT(ID_STRING_MAINMENU_SKILL) );
			    break;
			case _XDEF_MAINFRAME_LIFETAB      :
				strcpy( m_WindowTitle, _XGETINTERFACETEXT(ID_STRING_NEW_3673) );
				break;
			case _XDEF_MAINFRAME_QUESTTAB     :
				strcpy( m_WindowTitle, _XGETINTERFACETEXT(ID_STRING_MAINMENU_QUEST) );
				break;
			case _XDEF_MAINFRAME_COMMUNITYTAB :
				strcpy( m_WindowTitle, _XGETINTERFACETEXT(ID_STRING_MAINMENU_PARTY) );
			    break;			
			default:
			    break;
			}
#else
			if( m_pWindowTabbutton[i]->GetCommandID() == _XDEF_MAINFRAME_QUESTTAB )
			{
				strcpy( m_WindowTitle, _XGETINTERFACETEXT(ID_STRING_MAINFRAME_QUEST) );
			}
			else
			{
				strcpy( m_WindowTitle, m_pWindowTabbutton[i]->GetButtonText() );
			}
#endif
		}
		else
		{
			if( m_pWindowObject[i] )
			{
				m_pWindowObject[i]->ShowWindow( FALSE );
			}

			m_pWindowTabbutton[i]->SetCheck( FALSE );
		}
	}
	
	if( g_pInventory_Window )
	{
		if( g_pInventory_Window->GetShowStatus() )
		{
			_XWindow_SocketPlugIn* pSocketPlugInWindow = (_XWindow_SocketPlugIn*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGIN);
			_XWindow_PCTrade* pPCTrade_Window = (_XWindow_PCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_PCTRADE);
			if( windowid == _XDEF_MAINFRAME_CHARACTORTAB )
			{			
				if( pSocketPlugInWindow )
				{
					if( pSocketPlugInWindow->GetShowStatus() )
					{

					}
				}
				else if( pPCTrade_Window )
				{
					if( pPCTrade_Window->GetShowStatus() )
					{

					}
				}
				else
				{
					POINT pos = this->GetWindowPos();
					SIZE size = this->GetWindowSize();
					g_pInventory_Window->SetExpandDockingBorderFlag(FALSE);
					g_pInventory_Window->m_ExpandDockingBorder->ShowWindow( FALSE );
					g_pInventory_Window->MoveWindow( pos.x + size.cx, pos.y );
				}			
			}
			else
			{
				if( pSocketPlugInWindow )
				{
					if( pSocketPlugInWindow->GetShowStatus() )
					{

					}
				}
				else if( pPCTrade_Window )
				{
					if( pPCTrade_Window->GetShowStatus() )
					{

					}
				}
				else
				{
					g_pInventory_Window->SetDefaultPosition();
				}			
			}
		}
	}
	
	// 툴팁 제거
	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )	
			g_pDefaultTooltip_Window->ShowWindow( FALSE );
	}
}

DWORD _XWindow_MainFrame::GetCurrentSelectedTab(void)
{
	return m_pWindowTabbutton[m_SelectedTab]->GetCommandID();
}
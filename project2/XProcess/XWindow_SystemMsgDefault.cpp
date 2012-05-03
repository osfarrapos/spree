// XWindow_SystemMsgDefault.cpp: implementation of the _XWindow_SystemMsgDefault class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Email.h"
#include "xwindow_messenger.h"
#include "XWindow_SystemMsgDefault.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "XWindow_BloodPoint.h"
#include "XWindow_EventIcon.h"
#include "XWindow_MnDTotalPoint.h" //Author : ¾çÈñ¿Õ //breif :»çÁ¦ ½Ã½ºÅÛ
#include "XWindow_MnDCurrentPoint.h" //Author : ¾çÈñ¿Õ //breif :»çÁ¦ ½Ã½ºÅÛ

extern D3DCOLOR g_ChattingStringColor[16];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XCallback_SM_Close(  POINT pos, SIZE size  )
{		
	
 	if( g_bShowItemMallWindow ) return;//¾ÆÀÌÅÛ¸ôÀÌ ÀÖ´Â »óÅÂ¿¡¼­´Â ÅøÆÁÀ» ±×¸®Áö ¾Ê´Â´Ù.
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_CHATTINGDEFAULT_CLOSESYSTEMMSG), _XSC_INFORMATION );	
}

void _XInsertSystemMessage( int messagemode, TCHAR* args, ... )
{
	if( !args ) return;
	
	TCHAR strVBuffer[512];   	
	va_list ap;   
	va_start(ap, args);   
	vsprintf(strVBuffer, args, ap); 
	va_end(ap);


	_XWindow_SystemMsgDefault* pSystemMsgWindow = (_XWindow_SystemMsgDefault*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_SYSTEMMSGDEFAULT );
	
	if(pSystemMsgWindow)
	{	
		_XStringItem* newmessage = new _XStringItem( (LPTSTR)strVBuffer, messagemode, g_LocalSystemTime );

		g_SystemMsgList.resetList();
		g_SystemMsgList.insertItem( newmessage );

		if( pSystemMsgWindow->m_SystemMsgListScrollBar )
		{
			pSystemMsgWindow->m_SystemMsgListScrollBar->SetTotalLineCount( g_SystemMsgList.getitemcount() );
			pSystemMsgWindow->m_SystemMsgListScrollBar->UpdateData();
			pSystemMsgWindow->m_SystemMsgListScrollBar->SetLineScrollPos( 999999 ); 
		}
	}
}

_XWindow_SystemMsgDefault::_XWindow_SystemMsgDefault()
{
	m_pEmailWindowPtr				= NULL;	
	m_pMessengerWindowPtr			= NULL;

	m_SystemMsgListScrollPos		= 0;
	m_SystemMsgListScrollBar		= NULL;	
	m_pScrbarFocusedObject			= NULL;	
	m_MaxViewCount					= 6;
	m_btnFunctionalObjectViewMode	= NULL;
}

_XWindow_SystemMsgDefault::~_XWindow_SystemMsgDefault()
{
	DestroyWindow();
}

BOOL _XWindow_SystemMsgDefault::Initialize( void )
{
	int systemmsgimgindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	
	_XBTN_STRUCTURE minimizebtnstruct = { TRUE, { m_WindowSize.cx-13, 0 },
		{ 12, 12 }, _XDEF_SYSTEMMSGDEFAULT_CLOSE, 
		systemmsgimgindex,systemmsgimgindex,systemmsgimgindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* minimizebutton = new _XButton;
	minimizebutton->Create( minimizebtnstruct );
	minimizebutton->SetCallBackFunction_ToolTip( _XCallback_SM_Close );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 73, 237, 85, 249 );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  85, 237, 97, 249 );
	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  97, 237, 109, 249 );
	InsertChildObject( minimizebutton );	

	SAFE_DELETE( m_SystemMsgListScrollBar );
	m_SystemMsgListScrollBar = new _XVScrollBar;
	_XSCRBAR_STRUCTURE systemMsglistscrbarstructure = 
	{
		TRUE, {m_WindowPosition.x+m_WindowSize.cx-13,m_WindowPosition.y+13 }, 
			12, m_WindowSize.cy - 13, // width, height
			12,		 // btnysize
			22,		 // trackbar size
			_XDEF_CHATLIST_QUESIZE, m_MaxViewCount, 0,
			systemmsgimgindex,
			systemmsgimgindex,
			systemmsgimgindex,
			systemmsgimgindex,
			systemmsgimgindex,
			&g_MainInterfaceTextureArchive
	};
	
	m_SystemMsgListScrollBar->Create( systemMsglistscrbarstructure );
	m_SystemMsgListScrollBar->ShowWindow( TRUE );
	m_SystemMsgListScrollBar->SetViewBorder( TRUE );
	m_SystemMsgListScrollBar->SetViewNullBorder( FALSE );
	m_SystemMsgListScrollBar->SetTotalLineCount(0);	
	m_SystemMsgListScrollBar->UpdateData();
	
	m_MaxViewCount = ((m_WindowSize.cy) / 15);

	_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
	{
		{ 0,  0, 12, 12 },
		{ 24, 0, 36, 12 },
		{ 0,  12, 12, 24 },
		{ 24, 12, 36, 24 },
		{ 40,157, 52, 179}
	};
	m_SystemMsgListScrollBar->SetClipRect( cliprectstructure );
	
	int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
	_XBTN_STRUCTURE functionalobjectexitbtnstruct = 
	{
		TRUE, {260, 157}, {80, 26}, _XDEF_FUNCTONIONALOBJRECTVIEW_CLOSE, questresourceindex2, questresourceindex2, questresourceindex2,
			&g_MainInterfaceTextureArchive
	};
	
	m_btnFunctionalObjectViewMode = new _XButton;
	m_btnFunctionalObjectViewMode->Create(functionalobjectexitbtnstruct);
	m_btnFunctionalObjectViewMode->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL,80, 190, 160, 216);
	m_btnFunctionalObjectViewMode->SetButtonImageClipRect(_XBUTTONIMAGE_READY,80, 190, 160, 216);
	m_btnFunctionalObjectViewMode->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 190, 80, 216);
	m_btnFunctionalObjectViewMode->SetButtonTextColor( _XSC_DEFAULT_HIGHLIGHT );
	m_btnFunctionalObjectViewMode->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MAINGAME_1835);	
	m_btnFunctionalObjectViewMode->ShowWindow(FALSE);
	InsertChildObject(m_btnFunctionalObjectViewMode);

	return TRUE;
}

void _XWindow_SystemMsgDefault::DestroyWindow( void )
{
	SAFE_DELETE( m_SystemMsgListScrollBar );
	_XWindow::DestroyWindow();
}

void _XWindow_SystemMsgDefault::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( !this->m_Enable     ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;

	if( g_ZeroLevelMode ) return;

	/*if( eodapp->m_proc_maingame.m_bNPCDialogMode || eodapp->m_proc_maingame.m_bNPCDialogWideView )
	{
		if( !g_SystemMsgList.listEmpty() )
		{
			g_SystemMsgList.resetList();
			for( int i = 0; i < m_SystemMsgListScrollPos; i++ ) g_SystemMsgList.nextItem();
			
			_XStringItem* curstring;
			int ypos = 0;
			DWORD colorindex;
			
			do
			{			
				curstring = (_XStringItem*)g_SystemMsgList.currentItem();			
				colorindex = (((DWORD)curstring->Get_Attribute()) % 16);			
				DWORD offset  = HIWORD( (DWORD)curstring->Get_Attribute() );
				
				g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);
				g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 13 + offset, (m_WindowPosition.y + 8) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );		
				ypos++;  
				
				if( ypos >= m_MaxViewCount ) break;
				
				g_SystemMsgList.nextItem();
				
			}while( !g_SystemMsgList.atHeadOfList() );
		}	
		
		g_XBaseFont->Flush();	
	}
	else*/
	{
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
		
		if( !g_SystemMsgList.listEmpty() )
		{
			g_SystemMsgList.resetList();
			for( int i = 0; i < m_SystemMsgListScrollPos; i++ ) g_SystemMsgList.nextItem();
			
			_XStringItem* curstring;
			int ypos = 0;
			DWORD colorindex;
			
			do
			{			
				curstring = (_XStringItem*)g_SystemMsgList.currentItem();			
				colorindex = (((DWORD)curstring->Get_Attribute()) % 16);			
				DWORD attrib1 = LOWORD( (DWORD)curstring->Get_Attribute() );
				DWORD offset  = HIWORD( (DWORD)curstring->Get_Attribute() );				
				
				// ½Ã°£ÀÌ Áö³ª¸é ±ôºýÀÓÀ» ¾ø¾Ú.
				if( (g_LocalSystemTime - curstring->Get_Attribute(1)) > 3000)//_XDEF_CHATMESSAGEBLINKTIME ) // 2004.06.07->oneway48 insert
				{
					//curstring->Set_Attribute( colorindex, 0 );
					
					if( colorindex == _XDEF_CHATMESSAGECOLOR_GMBLOCKSTATUS )
					{
						g_XBaseFont->Flush();
						g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_GLOW, D3DCOLOR_ARGB( 255, 46, 104, 212 ) );
						g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);
						g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 13 + offset, (m_WindowPosition.y + 8) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );						
						g_XBaseFont->Flush();
						g_XBaseFont->DisableGrowMode();
					}
					else
					{
						g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);
						g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 13 + offset, (m_WindowPosition.y + 8) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );
					}
				}	
				else
				{
					if( (gnFPS/3) < gnFrameCounter )
					{
						if( colorindex == _XDEF_CHATMESSAGECOLOR_GMBLOCKSTATUS )
						{
							g_XBaseFont->Flush();							
							g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_GLOW, D3DCOLOR_ARGB( 255, 46, 104, 212 ) );
							g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);
							g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 13 + offset, (m_WindowPosition.y + 8) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );							
							g_XBaseFont->Flush();
							g_XBaseFont->DisableGrowMode();
						}
						else
						{
							g_XBaseFont->SetColor(g_ChattingStringColor[ colorindex ]);												
							g_XBaseFont->PutsAlign( this->m_WindowPosition.x + m_WindowSize.cx - 13 + offset, (m_WindowPosition.y + 8) + (ypos * 15), _XFONT_ALIGNTYPE_RIGHT, curstring->Get_String() );							
						}
					}
				}				
				ypos++;  
				
				if( ypos >= m_MaxViewCount ) break;
				
				g_SystemMsgList.nextItem();
				
			}while( !g_SystemMsgList.atHeadOfList() );
		}	
		
		g_XBaseFont->Flush();	
		
		g_XBaseFont->DisableGrowMode();
	}

	m_SystemMsgListScrollPos = m_SystemMsgListScrollBar->GetLineScrollPos();
	m_SystemMsgListScrollBar->Draw( m_pScrbarFocusedObject );

}

BOOL _XWindow_SystemMsgDefault::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_Enable     ) return FALSE;		
	if( m_pScrbarFocusedObject ) return TRUE;	
	
	m_SystemMsgListScrollBar->Process( pfocusobject );
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	return TRUE;
}

BOOL _XWindow_SystemMsgDefault::ProcessWheelMessage( short zDelta )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( m_SystemMsgListScrollBar )
	{
		if( m_SystemMsgListScrollBar->ProcessWheelMessage( zDelta ) )
		{
			return TRUE;
		}
		
		if( CheckMousePosition() )
		{
			int MoveDelta  = (FLOAT)zDelta / 40;			
			
			if( zDelta > 0 )
			{
				m_SystemMsgListScrollBar->LineUp( abs(MoveDelta) );
			}
			else
			{
				m_SystemMsgListScrollBar->LineDown( abs(MoveDelta) );
			}
			return TRUE;				
		}
	}
	
	if( _XWindow::ProcessWheelMessage( zDelta ) )
	{
		return TRUE;
	}
	
	return FALSE;	
}

BOOL _XWindow_SystemMsgDefault::CheckMousePosition(void)
{
	_XWindow::CheckMousePosition();
	return FALSE;
}

void _XWindow_SystemMsgDefault::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
}

void _XWindow_SystemMsgDefault::ShowWindow(BOOL show)
{

	if(show)
	{
		#ifdef _XTS_BLOODPOINT
		_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
		if(pBloodPoint_Window)
		{
			pBloodPoint_Window->MoveWindow(gnWidth - 72, gnHeight - 217);
		}
		#endif

	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
		_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
		if(pMnDCurrentPoint_Window)
		{
			pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
		}
		
		_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
		if(pMnDTotalPoint_Window)
		{
			pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
		}
	#endif

	#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ¾çÈñ¿Õ //breif : ÀÌº¥Æ® ¾ÆÀÌÄÜ
		_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
		if( g_pInventory_Window && !g_pInventory_Window->GetShowStatus() )
		{
			if( pBloodPoint_Window && pEventIcon)
			{
			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
				pEventIcon->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDTotalPoint_Window->GetWindowPos().y - 50 );
			#else
				pEventIcon->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 50 );
			#endif
			}
		}
	#endif
	}
	else
	{
		#ifdef _XTS_BLOODPOINT
		_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
		if(pBloodPoint_Window)
		{
			pBloodPoint_Window->MoveWindow(gnWidth - 72, gnHeight - 217 + 99);
		}
		#endif

	#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
		_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
		if(pMnDCurrentPoint_Window)
		{
			pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
		}
		
		_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
		if(pMnDTotalPoint_Window)
		{
			pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
		}
	#endif

		#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ¾çÈñ¿Õ //breif : ÀÌº¥Æ® ¾ÆÀÌÄÜ
		_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
		if( g_pInventory_Window && !g_pInventory_Window->GetShowStatus() )
		{
			if( pBloodPoint_Window && pEventIcon)
			{
			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
				pEventIcon->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDTotalPoint_Window->GetWindowPos().y - 50 );
			#else
				pEventIcon->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 50 );
			#endif
			}
		}
		#endif
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_SystemMsgDefault::SetDockingMode(BOOL flag)
{
	if( flag )
	{	
		MoveWindow(gnWidth - 557, gnHeight-195);
		m_SystemMsgListScrollBar->MoveWindow(gnWidth - 196 ,gnHeight - 182);

		// ´Ù¸¥ window¿Í ºÙ¾îÀÖÀ½
		if(m_ShowWindow)
		{
			#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ¾çÈñ¿Õ //breif : ÀÌº¥Æ® ¾ÆÀÌÄÜ
			_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
			if( g_pInventory_Window && pEventIcon )
				pEventIcon->MoveWindow( gnWidth - 50, g_pInventory_Window->GetWindowPos().y - 50 );
			#endif

			#ifdef _XTS_BLOODPOINT
			// system msg Ã¢ ¶°ÀÖÀ½
			_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
			if(pBloodPoint_Window)
			{
				pBloodPoint_Window->MoveWindow(gnWidth - 60 - 185, gnHeight - 120 - 96 );
			}
			#endif
		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
			_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
			if(pMnDCurrentPoint_Window)
			{
				pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
			}
			
			_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
			if(pMnDTotalPoint_Window)
			{
				pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
			}
		#endif
		}
		else
		{
			#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ¾çÈñ¿Õ //breif : ÀÌº¥Æ® ¾ÆÀÌÄÜ
			_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
			if( g_pInventory_Window && pEventIcon )
				pEventIcon->MoveWindow( gnWidth - 50, g_pInventory_Window->GetWindowPos().y - 50 );
			#endif

			#ifdef _XTS_BLOODPOINT
			// system msg Ã¢ ¾øÀ½
			_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
			if(pBloodPoint_Window)
			{
				pBloodPoint_Window->MoveWindow(gnWidth - 60 - 185, gnHeight - 99 - 96 );
			}
			#endif

		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
			_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
			if(pMnDCurrentPoint_Window)
			{
				pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
			}
			
			_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
			if(pMnDTotalPoint_Window)
			{
				pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
			}
		#endif
		}
	}
	else
	{
		MoveWindow(gnWidth - 374, gnHeight-195);
		m_SystemMsgListScrollBar->MoveWindow(gnWidth - 13 ,gnHeight - 182);

		// ´Ù¸¥ window ¾øÀ½
		if(m_ShowWindow)
		{
			#ifdef _XTS_BLOODPOINT
			// system msg Ã¢ ¶°ÀÖÀ½
			_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
			if(pBloodPoint_Window)
			{
				//pBloodPoint_Window->MoveWindow(gnWidth-374+302, gnHeight-195-23);
				pBloodPoint_Window->MoveWindow( gnWidth - 60, gnHeight - 120 - 96 );
			}
			#endif

		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
			_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
			if(pMnDCurrentPoint_Window)
			{
				pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
			}
			
			_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
			if(pMnDTotalPoint_Window)
			{
				pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
			}
		#endif

		#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ¾çÈñ¿Õ //breif : ÀÌº¥Æ® ¾ÆÀÌÄÜ
			_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
			if( pBloodPoint_Window && pEventIcon)
			{
			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
				pEventIcon->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDTotalPoint_Window->GetWindowPos().y - 50 );
			#else
				pEventIcon->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 50 );
			#endif
			}
		#endif
		}
		else
		{
			#ifdef _XTS_BLOODPOINT
			// system msg Ã¢ ¾øÀ½
			_XWindow_BloodPoint* pBloodPoint_Window = (_XWindow_BloodPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_BLOODPOINTWINDOW);
			if(pBloodPoint_Window)
			{
				//pBloodPoint_Window->MoveWindow((gnWidth>>1)+440, (gnHeight>>1)+167+99);
				pBloodPoint_Window->MoveWindow( gnWidth - 60, gnHeight - 99 );
			}
			#endif

		#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
			_XWindow_MnDCurrentPoint* pMnDCurrentPoint_Window = (_XWindow_MnDCurrentPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDCURRENTPOINTWINDOW);
			if(pMnDCurrentPoint_Window)
			{
				pMnDCurrentPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 19 );
			}
			
			_XWindow_MnDTotalPoint* pMnDTotalPoint_Window = (_XWindow_MnDTotalPoint*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MNDTOTALPOINTWINDOW);
			if(pMnDTotalPoint_Window)
			{
				pMnDTotalPoint_Window->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDCurrentPoint_Window->GetWindowPos().y - 19 );
			}
		#endif

		#ifdef _XDEF_EVENT_RENEWAL_080201_MAGVIPER //Author : ¾çÈñ¿Õ //breif : ÀÌº¥Æ® ¾ÆÀÌÄÜ
			_XWindow_EventIcon* pEventIcon = (_XWindow_EventIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_EVENT_RENEWAL_WINDOW);
			if( pBloodPoint_Window && pEventIcon)
			{
			#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : ¾çÈñ¿Õ //breif : »çÁ¦
				pEventIcon->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pMnDTotalPoint_Window->GetWindowPos().y - 50 );
			#else
				pEventIcon->MoveWindow( pBloodPoint_Window->GetWindowPos().x, pBloodPoint_Window->GetWindowPos().y - 50 );
			#endif
			}
		#endif
		}	
	}
}
// XWindow_TutorialIcon.cpp: implementation of the _XWindow_TutorialIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "xwindow_chattingdefault.h"
#include "XWindow_Tutorial.h"
#include "XWindow_TutorialIcon.h"
#include "SoundEffectList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int _MAX_TUTORIALICON = 5;

_XWindow_TutorialIcon::_XWindow_TutorialIcon()
{
	for( int i = 0; i < 8 /*_MAX_TUTORIALICON*/; i++)
		m_BtnTutorialIcon[i] = NULL;

	m_pChattingDefault_Window = NULL;
	m_ExternalTutorialCount = 0;
	m_InternalTutorialCount = 0;
	m_EffectCount = 0;
	m_EffectTime = 0;
}

_XWindow_TutorialIcon::~_XWindow_TutorialIcon()
{

}

BOOL _XWindow_TutorialIcon::Initialize( void )
{
	SetWindowMoveMode(_XWMOVE_FIXED);
	
	// Icon button ---------------------------------------------------------------------------------------------=
	int btnimgindex = g_MainInterfaceTextureArchive.FindResource(g_MI_KiUpTextureName);
	_XBTN_STRUCTURE iconbtnstruct = { TRUE, { 0, 0 }, { 32, 42 }, _XDEF_TUTORIAL_ICON1,
										btnimgindex, btnimgindex, btnimgindex, &g_MainInterfaceTextureArchive };

	for( int i = 0; i < _MAX_TUTORIALICON; i++)
	{
		SAFE_DELETE( m_BtnTutorialIcon[i] );
		
		m_BtnTutorialIcon[i] = new _XButton;
		m_BtnTutorialIcon[i]->Create( iconbtnstruct );	
		m_BtnTutorialIcon[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 165, 119, 197, 161 );
		m_BtnTutorialIcon[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  202, 163, 234, 205 );
		m_BtnTutorialIcon[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  165, 163, 197, 205 );
		m_BtnTutorialIcon[i]->ShowWindow(FALSE);
		InsertChildObject( m_BtnTutorialIcon[i] );		
		iconbtnstruct.commandid ++;
	}
	// ---------------------------------------------------------------------------------------------------------=
	return TRUE;
}

void _XWindow_TutorialIcon::DestroyWindow( void )
{
	if( !m_ExternalTutorialIconLIst.empty() )
		m_ExternalTutorialIconLIst.clear();
	if( !m_InternalTutorialIconLIst.empty() )
		m_InternalTutorialIconLIst.clear();	

	if( !m_ExternalTutorialDialogLIst.empty() )
	{
		list<_XTUTORIALDATA*>::iterator iter = m_ExternalTutorialDialogLIst.begin();
		while (iter != m_ExternalTutorialDialogLIst.end()) 
		{
			_XTUTORIALDATA* tempData = *iter;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter = m_ExternalTutorialDialogLIst.erase( iter );
			}
			else
			{
				iter++;
			}
		}		
		m_ExternalTutorialDialogLIst.clear();
	}
	if( !m_InternalTutorialDialogLIst.empty() )
	{
		list<_XTUTORIALDATA*>::iterator iter = m_InternalTutorialDialogLIst.begin();
		while (iter != m_InternalTutorialDialogLIst.end()) 
		{
			_XTUTORIALDATA* tempData = *iter;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter = m_InternalTutorialDialogLIst.erase( iter );
			}
			else
			{
				iter++;
			}
		}		
		m_InternalTutorialDialogLIst.clear();
	}

	_XWindow::DestroyWindow();
}

void _XWindow_TutorialIcon::ResetTutorialData()
{
	m_ExternalTutorialCount = 0;
	m_InternalTutorialCount = 0;
	for( int i = 0; i < _MAX_TUTORIALICON; i++ )
		m_BtnTutorialIcon[i]->ShowWindow(FALSE);	
	
	list<_XTUTORIALDATA*>::iterator iter = m_ExternalTutorialDialogLIst.begin();
	for( ; iter != m_ExternalTutorialDialogLIst.end(); iter++ )
	{
		_XTUTORIALDATA* tempData = *iter;
		if( tempData )
			g_MainWindowManager.DeleteWindow( tempData->winId );
	}
		
	list<_XTUTORIALDATA*>::iterator iter2 = m_InternalTutorialDialogLIst.begin();
	for( ; iter2 != m_InternalTutorialDialogLIst.end(); iter2++ )
	{
		_XTUTORIALDATA* tempData = *iter2;
		if( tempData )
			g_MainWindowManager.DeleteWindow( tempData->winId );
	}

	if( !m_ExternalTutorialIconLIst.empty() )
		m_ExternalTutorialIconLIst.clear();
	if( !m_InternalTutorialIconLIst.empty() )
		m_InternalTutorialIconLIst.clear();	

	if( !m_ExternalTutorialDialogLIst.empty() )
	{
		list<_XTUTORIALDATA*>::iterator iter = m_ExternalTutorialDialogLIst.begin();
		while (iter != m_ExternalTutorialDialogLIst.end()) 
		{
			_XTUTORIALDATA* tempData = *iter;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter = m_ExternalTutorialDialogLIst.erase( iter );
			}
			else
			{
				iter++;
			}
		}
		m_ExternalTutorialDialogLIst.clear();
	}
	if( !m_InternalTutorialDialogLIst.empty() )
	{
		list<_XTUTORIALDATA*>::iterator iter = m_InternalTutorialDialogLIst.begin();
		while (iter != m_InternalTutorialDialogLIst.end()) 
		{
			_XTUTORIALDATA* tempData = *iter;
			if( tempData )
			{
				SAFE_DELETE(tempData);
				iter = m_ExternalTutorialDialogLIst.erase( iter );
			}
			else
			{
				iter++;
			}
		}
		m_InternalTutorialDialogLIst.clear();
	}		
}

void _XWindow_TutorialIcon::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;	
}

BOOL _XWindow_TutorialIcon::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow )		return FALSE;
	if( !this->m_Enable  )			return FALSE;	

	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView )	// 일반 화면
	{
		list<_XTUTORIALTYPE>::iterator iter = m_ExternalTutorialIconLIst.begin();
		int btnindex = 0;
		POINT pos = m_pChattingDefault_Window->GetWindowPos();
		for( ; iter != m_ExternalTutorialIconLIst.end(); iter++ )
		{
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			m_BtnTutorialIcon[btnindex]->MoveWindow( pos.x+5+(34*btnindex), pos.y - 47 - 18); // Chatting창의 Function과 EXP 창 추가로 빼준다.
#else
			m_BtnTutorialIcon[btnindex]->MoveWindow( pos.x+5+(34*btnindex), pos.y - 47 );			
#endif
			btnindex++;
		}

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
		MoveWindow( pos.x+5, pos.y-47-18 );
#else
		MoveWindow( pos.x+5, pos.y-47 );
#endif
		
		if( m_EffectCount < 8 )
		{
			if( g_LocalSystemTime - m_EffectTime > 250 )
			{
				if( m_EffectCount % 2 )
					m_BtnTutorialIcon[m_ExternalTutorialIconLIst.size()-1]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 202, 119, 234, 161 );
				else
					m_BtnTutorialIcon[m_ExternalTutorialIconLIst.size()-1]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 165, 119, 197, 161 );	
				
				m_EffectCount++;
				m_EffectTime = g_LocalSystemTime;
			}					
		}			
	}
	else
	{
		list<_XTUTORIALTYPE>::iterator iter = m_InternalTutorialIconLIst.begin();
		int btnindex = 0;		
		for( ; iter != m_InternalTutorialIconLIst.end(); iter++ )
		{
			m_BtnTutorialIcon[btnindex]->MoveWindow( 12+(34*btnindex), gnHeight-278 );			
			btnindex++;
		}
		MoveWindow( 12, gnHeight - 278 );
		
		if( m_EffectCount < 8 )
		{
			if( g_LocalSystemTime - m_EffectTime > 250 )
			{
				if( m_EffectCount % 2 )
					m_BtnTutorialIcon[m_InternalTutorialIconLIst.size()-1]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 202, 119, 234, 161 );					
				else
					m_BtnTutorialIcon[m_InternalTutorialIconLIst.size()-1]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 165, 119, 197, 161 );					
				
				m_EffectCount++;
				m_EffectTime = g_LocalSystemTime;
			}					
		}
	}
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}

void _XWindow_TutorialIcon::AllTutorialDraw()
{
	list<_XTUTORIALDATA*>::iterator iter = m_ExternalTutorialDialogLIst.begin();
	for( ; iter != m_ExternalTutorialDialogLIst.end(); iter++ )
	{
		_XTUTORIALDATA* tempData = *iter;
		if( tempData )
		{
			if( tempData->winPtr )
				tempData->winPtr->Draw();
		}
		
	}
	this->Draw();
}

void _XWindow_TutorialIcon::AllTutorialProcess()
{
	list<_XTUTORIALDATA*>::iterator iter = m_ExternalTutorialDialogLIst.begin();
	for( ; iter != m_ExternalTutorialDialogLIst.end(); iter++ )
	{
		_XTUTORIALDATA* tempData = *iter;
		if( tempData )
		{
			if( tempData->winPtr )
				tempData->winPtr->Process();
		}
	}
	this->Process();
}

void _XWindow_TutorialIcon::InsertTutorialIcon( _XTUTORIALTYPE type )
{
	if( !g_ViewGuide ) return;	// 가이드 끄기 버튼 활성화시
#ifndef _XTS_SHOWTUTORIAL
	return;
#endif

	_XPlayInterfaceSound(ID_SR_INTERFACE_TUTO_01_WAV);
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView )	// 일반 화면
	{
		m_ExternalTutorialIconLIst.push_back( type );
		if( m_ExternalTutorialIconLIst.size() > _MAX_TUTORIALICON )
		{
			list<_XTUTORIALTYPE>::iterator iter = m_ExternalTutorialIconLIst.begin();
			iter = m_ExternalTutorialIconLIst.erase(iter);		
		}

		int btnindex = 0;
		POINT pos = m_pChattingDefault_Window->GetWindowPos();
		list<_XTUTORIALTYPE>::iterator iter = m_ExternalTutorialIconLIst.begin();
		for( ; iter != m_ExternalTutorialIconLIst.end(); iter++ )
		{
			m_BtnTutorialIcon[btnindex]->ShowWindow( TRUE );
			
#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
			m_BtnTutorialIcon[btnindex]->MoveWindow( pos.x+5+(34*btnindex), pos.y - 47 - 18); // Chatting창의 Function과 EXP 창 추가로 빼준다.
#else
			m_BtnTutorialIcon[btnindex]->MoveWindow( pos.x+5+(34*btnindex), pos.y - 47);
#endif
			btnindex++;
		}

		SIZE windowsize;
		windowsize.cx = 34 * m_ExternalTutorialIconLIst.size();
		windowsize.cy = 42;
		this->SetWindowSize(windowsize);	

#ifdef _XDEF_INTERFACE_RENEWAL_071002_KUKURI
		this->MoveWindow( pos.x+5, pos.y-47-18 ); // Chatting창의 Function과 EXP 창 추가로 빼줌.
#else
		this->MoveWindow( pos.x+5, pos.y-47 );
#endif
		this->ShowWindow( TRUE );

	}
	else	// 와이드뷰 화면
	{
		m_InternalTutorialIconLIst.push_back( type );
		if( m_InternalTutorialIconLIst.size() > _MAX_TUTORIALICON-1 )
		{
			while( m_InternalTutorialIconLIst.size() < _MAX_TUTORIALICON ) // 4개 이하가 될때 까지 반복 삭제
			{
				list<_XTUTORIALTYPE>::iterator iter = m_InternalTutorialIconLIst.begin();
				iter = m_InternalTutorialIconLIst.erase(iter);
			}
		}

		int btnindex = 0;
		list<_XTUTORIALTYPE>::iterator iter = m_InternalTutorialIconLIst.begin();
		for( ; iter != m_InternalTutorialIconLIst.end(); iter++ )
		{
			if( btnindex >= _MAX_TUTORIALICON )
			{
				//_XLog( "WARNING : access violation [%d/%d]", m_InternalTutorialIconLIst.size(), _MAX_TUTORIALICON );
				break;
			}
			m_BtnTutorialIcon[btnindex]->ShowWindow( TRUE );
			m_BtnTutorialIcon[btnindex]->MoveWindow( 12+(34*btnindex), gnHeight-278 );
			btnindex++;
		}
		SIZE windowsize;
		windowsize.cx = 34 * m_InternalTutorialIconLIst.size();
		windowsize.cy = 42;
		this->SetWindowSize(windowsize);	
		this->MoveWindow( 12, gnHeight-278 );
		this->ShowWindow( TRUE );
	}
	
	m_EffectCount = 0;
	m_EffectTime = g_LocalSystemTime;
}

void _XWindow_TutorialIcon::RemoveTutorialIcon( _XTUTORIALTYPE type )	// A타입 : No Icon
{
	if( !g_ViewGuide ) return;	// 가이드 끄기 버튼 활성화시
#ifndef _XTS_SHOWTUTORIAL
	return;
#endif
	
	_XPlayInterfaceSound(ID_SR_INTERFACE_TUTO_01_WAV);
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView )	// 일반 화면
	{
		_XWindow_TutorialEx* pTutorialdilog = _XWindow_TutorialEx::CreateTutorialDialogBox( &g_MainWindowManager );
		if( pTutorialdilog->SetTutorialType( type ) )
		{
			_XTUTORIALDATA* TempTutorialData = new _XTUTORIALDATA;
			TempTutorialData->winId = pTutorialdilog->GetWindowID();
			TempTutorialData->winPtr = (_XWindow*)pTutorialdilog;
			m_ExternalTutorialDialogLIst.push_back( TempTutorialData );

			pTutorialdilog->MoveWindow(gnWidth - (pTutorialdilog->GetWindowSize().cx+10)-(2*m_ExternalTutorialDialogLIst.size()), 
				234+(24*m_ExternalTutorialCount));

			m_ExternalTutorialCount++;
		}
		else
		{
			g_MainWindowManager.DeleteWindow( pTutorialdilog->GetWindowID() );
		}		
	}
	else
	{
		_XWindow_TutorialEx* pTutorialdilog = _XWindow_TutorialEx::CreateTutorialDialogBox( &g_MainWindowManager );
		if( pTutorialdilog->SetTutorialType( type ) )
		{	
			_XTUTORIALDATA* TempTutorialData = new _XTUTORIALDATA;
			TempTutorialData->winId = pTutorialdilog->GetWindowID();
			TempTutorialData->winPtr = (_XWindow*)pTutorialdilog;
			m_InternalTutorialDialogLIst.push_back( TempTutorialData );

			int pos_y = gnHeight - 228 + (24*m_InternalTutorialCount);
			
			if((pos_y + pTutorialdilog->GetWindowSize().cy) >= gnHeight)
			{
				pos_y = gnHeight - ((pTutorialdilog->GetWindowSize().cy+10)+(24*m_InternalTutorialCount));
			}
			
			pTutorialdilog->MoveWindow( 10+(2 * m_ExternalTutorialDialogLIst.size()), pos_y);

			m_InternalTutorialCount++;
		}
		else
		{
			g_MainWindowManager.DeleteWindow( pTutorialdilog->GetWindowID() );
		}
	}	
}

void _XWindow_TutorialIcon::RemoveTutorialIcon(int index)	// B타입 : Icon 
{
	if( !g_ViewGuide ) return;	// 가이드 끄기 버튼 활성화시
#ifndef _XTS_SHOWTUTORIAL
	return;
#endif
	
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView )	// 일반 화면
	{
		if( index < m_ExternalTutorialIconLIst.size() )
		{
			list<_XTUTORIALTYPE>::iterator iter = m_ExternalTutorialIconLIst.begin();
			advance( iter, index );
			
			m_BtnTutorialIcon[m_ExternalTutorialIconLIst.size()-1]->ShowWindow( FALSE );	
			m_WindowSize.cx -= 34;
			
			_XWindow_Tutorial* pTutorialdilog = _XWindow_Tutorial::CreateTutorialDialogBox( &g_MainWindowManager );
			if( pTutorialdilog->SetTutorialType( *iter ) )
			{
				_XTUTORIALDATA* TempTutorialData = new _XTUTORIALDATA;
				TempTutorialData->winId = pTutorialdilog->GetWindowID();
				TempTutorialData->winPtr = (_XWindow*)pTutorialdilog;
				m_ExternalTutorialDialogLIst.push_back( TempTutorialData );
				
				pTutorialdilog->MoveWindow( gnWidth-(pTutorialdilog->GetWindowSize().cx+10)-(2 * m_ExternalTutorialDialogLIst.size() ), 234+(24*m_ExternalTutorialCount) );

				SaveTutorialData( *iter);
				iter = m_ExternalTutorialIconLIst.erase( iter );
				m_ExternalTutorialCount++;
			}
			else
			{
				g_MainWindowManager.DeleteWindow( pTutorialdilog->GetWindowID() );
			}
		}
	}
	else
	{
		if( index < m_InternalTutorialIconLIst.size() )
		{
			list<_XTUTORIALTYPE>::iterator iter = m_InternalTutorialIconLIst.begin();
			advance( iter, index );
			
			m_BtnTutorialIcon[m_InternalTutorialIconLIst.size()-1]->ShowWindow( FALSE );
			m_WindowSize.cx -= 34;
			
			_XWindow_Tutorial* pTutorialdilog = _XWindow_Tutorial::CreateTutorialDialogBox( &g_MainWindowManager );
			if( pTutorialdilog->SetTutorialType( *iter ) )
			{
				_XTUTORIALDATA* TempTutorialData = new _XTUTORIALDATA;
				TempTutorialData->winId = pTutorialdilog->GetWindowID();
				TempTutorialData->winPtr = (_XWindow*)pTutorialdilog;
				m_InternalTutorialDialogLIst.push_back( TempTutorialData );

				int pos_y = gnHeight - 228 + (24*m_InternalTutorialCount);

				if((pos_y + pTutorialdilog->GetWindowSize().cy) >= gnHeight)
				{
					pos_y = gnHeight - ((pTutorialdilog->GetWindowSize().cy+10)+(24*m_InternalTutorialCount));
				}

				pTutorialdilog->MoveWindow( 10+(2 * m_ExternalTutorialDialogLIst.size()), pos_y);
				
				
				SaveTutorialData(*iter);
				iter = m_InternalTutorialIconLIst.erase( iter );
				m_InternalTutorialCount++;
			}			
			else
			{
				g_MainWindowManager.DeleteWindow( pTutorialdilog->GetWindowID() );
			}
		}
	}
}

void _XWindow_TutorialIcon::DeleteTutorialIDlgList( DWORD winid )
{
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView )
	{
		list<_XTUTORIALDATA*>::iterator iter = m_ExternalTutorialDialogLIst.begin();
		for( ; iter != m_ExternalTutorialDialogLIst.end(); iter++ )
		{
			_XTUTORIALDATA* tempData = *iter;
			if( tempData )
			{
				if( winid == tempData->winId )
				{
					SAFE_DELETE(tempData);
					iter = m_ExternalTutorialDialogLIst.erase( iter );
					if( m_ExternalTutorialDialogLIst.empty() )
						m_ExternalTutorialCount = 0;
					return;
				}
			}
		}		
	}
	else
	{
		list<_XTUTORIALDATA*>::iterator iter = m_InternalTutorialDialogLIst.begin();
		for( ; iter != m_InternalTutorialDialogLIst.end(); iter++ )
		{
			_XTUTORIALDATA* tempData = *iter;
			if( tempData )
			{
				if( winid == tempData->winId )
				{
					SAFE_DELETE(tempData);
					iter = m_InternalTutorialDialogLIst.erase( iter );
					if( m_InternalTutorialDialogLIst.empty() )
						m_InternalTutorialCount = 0;
					return;
				}			
			}
		}		
	}
}

void _XWindow_TutorialIcon::AllClearInternalTutorialIconList()
{
	list<_XTUTORIALDATA*>::iterator iter = m_InternalTutorialDialogLIst.begin();
	while (iter != m_InternalTutorialDialogLIst.end()) 
	{
		_XTUTORIALDATA* tempData = *iter;
		if( tempData )
		{
			g_MainWindowManager.DeleteWindow( tempData->winId );			

			SAFE_DELETE(tempData);
			iter = m_InternalTutorialDialogLIst.erase(iter);
		}		
		else
		{
			iter++;
		}
	}
	m_InternalTutorialDialogLIst.clear();
	
	list<_XTUTORIALTYPE>::iterator iter2 = m_InternalTutorialIconLIst.begin();
	int btnindex = 0;
	m_WindowSize.cx = 1;
	while (iter2 != m_InternalTutorialIconLIst.end()) 
	{
		if( btnindex >= _MAX_TUTORIALICON ) // begin == end 일때 예외 처리 할 것
		{
			//_XLog( "WARNING : access violation [%d/%d]", m_InternalTutorialIconLIst.size(), _MAX_TUTORIALICON );
			break;
		}

		if( m_BtnTutorialIcon[btnindex] )
		{
			m_BtnTutorialIcon[btnindex]->ShowWindow( FALSE );
		}
		else
		{
			break;
		}

		btnindex++;
		iter2++;
	}
	m_InternalTutorialIconLIst.clear();
}

void _XWindow_TutorialIcon::AllCloseExternalTutorialWindow()
{
	list<_XTUTORIALDATA*>::iterator iter = m_ExternalTutorialDialogLIst.begin();
	for( ; iter != m_ExternalTutorialDialogLIst.end(); iter++ )
	{
		_XTUTORIALDATA* tempData = *iter;
		if( tempData )
		{
			tempData->winPtr->ShowWindow( FALSE );
		}		
	}	
	
	list<_XTUTORIALTYPE>::iterator iter2 = m_ExternalTutorialIconLIst.begin();
	int btnindex = 0;
	m_WindowSize.cx = 0;
	for( ; iter2 != m_ExternalTutorialIconLIst.end(); iter2++ )
	{
		m_BtnTutorialIcon[btnindex]->ShowWindow( FALSE );
		btnindex++;
	}	
}

void _XWindow_TutorialIcon::AllOpenExternalTutorialWindow()
{
	AllClearInternalTutorialIconList();
	
	list<_XTUTORIALDATA*>::iterator iter = m_ExternalTutorialDialogLIst.begin();
	for( ; iter != m_ExternalTutorialDialogLIst.end(); iter++ )
	{
		_XTUTORIALDATA* tempData = *iter;
		if( tempData )
		{
			tempData->winPtr->ShowWindow( TRUE );
		}
	}
	list<_XTUTORIALTYPE>::iterator iter2 = m_ExternalTutorialIconLIst.begin();
	int btnindex = 0;	
	for( ; iter2 != m_ExternalTutorialIconLIst.end(); iter2++ )
	{
		m_BtnTutorialIcon[btnindex]->ShowWindow( TRUE );
		m_WindowSize.cx += 34;
		btnindex++;
	}
}

void _XWindow_TutorialIcon::SaveTutorialData(_XTUTORIALTYPE type)	// 실제 파일에 저장 시키는 곳
{
	if( !g_ViewGuide ) return;	// 가이드 끄기 버튼 활성화시
#ifndef _XTS_SHOWTUTORIAL
	return;
#endif
	
	if( g_TutorialInfo )
	{
		switch(type) 
		{
		case _XTUTORIAL_001: g_TutorialInfo->_XTutorial_001 = TRUE;	break;
		case _XTUTORIAL_002: g_TutorialInfo->_XTutorial_002 = TRUE;	break;
		case _XTUTORIAL_003: g_TutorialInfo->_XTutorial_003 = TRUE;	break;
		case _XTUTORIAL_004: g_TutorialInfo->_XTutorial_004 = TRUE;	break;
		case _XTUTORIAL_005: g_TutorialInfo->_XTutorial_005 = TRUE;	break;
		case _XTUTORIAL_006: g_TutorialInfo->_XTutorial_006 = TRUE;	break;
		case _XTUTORIAL_007: g_TutorialInfo->_XTutorial_007 = TRUE;	break;
		case _XTUTORIAL_008: g_TutorialInfo->_XTutorial_008 = TRUE;	break;
		case _XTUTORIAL_009: g_TutorialInfo->_XTutorial_009 = TRUE;	break;
		case _XTUTORIAL_010: g_TutorialInfo->_XTutorial_010 = TRUE;	break;
		case _XTUTORIAL_011: g_TutorialInfo->_XTutorial_011 = TRUE;	break;
		case _XTUTORIAL_012: g_TutorialInfo->_XTutorial_012 = TRUE;	break;
		case _XTUTORIAL_013: g_TutorialInfo->_XTutorial_013 = TRUE;	break;
		case _XTUTORIAL_014: g_TutorialInfo->_XTutorial_014 = TRUE;	break;
		case _XTUTORIAL_015: g_TutorialInfo->_XTutorial_015 = TRUE;	break;
		case _XTUTORIAL_016: g_TutorialInfo->_XTutorial_016 = TRUE;	break;
		case _XTUTORIAL_017: g_TutorialInfo->_XTutorial_017 = TRUE;	break;
		case _XTUTORIAL_018: g_TutorialInfo->_XTutorial_018 = TRUE;	break;
		case _XTUTORIAL_019: g_TutorialInfo->_XTutorial_019 = TRUE;	break;
		case _XTUTORIAL_020: g_TutorialInfo->_XTutorial_020 = TRUE;	break;
		case _XTUTORIAL_021: g_TutorialInfo->_XTutorial_021 = TRUE;	break;
		case _XTUTORIAL_022: g_TutorialInfo->_XTutorial_022 = TRUE;	break;
		case _XTUTORIAL_023: g_TutorialInfo->_XTutorial_023 = TRUE;	break;
		case _XTUTORIAL_024: g_TutorialInfo->_XTutorial_024 = TRUE;	break;
		case _XTUTORIAL_025: g_TutorialInfo->_XTutorial_025 = TRUE;	break;
		case _XTUTORIAL_026: g_TutorialInfo->_XTutorial_026 = TRUE;	break;
		case _XTUTORIAL_027: g_TutorialInfo->_XTutorial_027 = TRUE;	break;
		case _XTUTORIAL_028: g_TutorialInfo->_XTutorial_028 = TRUE;	break;
		case _XTUTORIAL_029: g_TutorialInfo->_XTutorial_029 = TRUE;	break;
		case _XTUTORIAL_030: g_TutorialInfo->_XTutorial_030 = TRUE;	break;
		case _XTUTORIAL_031: g_TutorialInfo->_XTutorial_031 = TRUE;	break;
		case _XTUTORIAL_032: g_TutorialInfo->_XTutorial_032 = TRUE;	break;
		case _XTUTORIAL_033: g_TutorialInfo->_XTutorial_033 = TRUE;	break;
		case _XTUTORIAL_034: g_TutorialInfo->_XTutorial_034 = TRUE;	break;
		case _XTUTORIAL_035: g_TutorialInfo->_XTutorial_035 = TRUE;	break;
		case _XTUTORIAL_036: g_TutorialInfo->_XTutorial_036 = TRUE;	break;
		case _XTUTORIAL_037: g_TutorialInfo->_XTutorial_037 = TRUE;	break;
		case _XTUTORIAL_038: g_TutorialInfo->_XTutorial_038 = TRUE;	break;
		case _XTUTORIAL_039: g_TutorialInfo->_XTutorial_039 = TRUE;	break;
		case _XTUTORIAL_040: g_TutorialInfo->_XTutorial_040 = TRUE;	break;
		case _XTUTORIAL_041: g_TutorialInfo->_XTutorial_041 = TRUE;	break;
		case _XTUTORIAL_042: g_TutorialInfo->_XTutorial_042 = TRUE;	break;			
		case _XTUTORIAL_043: g_TutorialInfo->_XTutorial_043 = TRUE;	break;
		case _XTUTORIAL_044: g_TutorialInfo->_XTutorial_044 = TRUE;	break;
		case _XTUTORIAL_045: g_TutorialInfo->_XTutorial_045 = TRUE;	break;
		case _XTUTORIAL_046: g_TutorialInfo->_XTutorial_046 = TRUE;	break;
		case _XTUTORIAL_047: g_TutorialInfo->_XTutorial_047 = TRUE;	break;
		case _XTUTORIAL_048: g_TutorialInfo->_XTutorial_048 = TRUE;	break;
		case _XTUTORIAL_049: g_TutorialInfo->_XTutorial_049 = TRUE;	break;
		case _XTUTORIAL_050: g_TutorialInfo->_XTutorial_050 = TRUE;	break;
		case _XTUTORIAL_051: g_TutorialInfo->_XTutorial_051 = TRUE;	break;
		}
	}	
}

BOOL _XWindow_TutorialIcon::EscapeKeyDown()
{
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView )
	{
		if( !m_ExternalTutorialDialogLIst.empty() )
		{
			list<_XTUTORIALDATA*>::reverse_iterator reiter(m_ExternalTutorialDialogLIst.end());
			list<_XTUTORIALDATA*>::reverse_iterator reiter_end(m_ExternalTutorialDialogLIst.begin());
			for( ; reiter != reiter_end; reiter++ )
			{
				_XTUTORIALDATA* tempdata = *reiter;
				if( tempdata )
				{
					g_MainWindowManager.DeleteWindow( tempdata->winId );
					DeleteTutorialIDlgList(tempdata->winId);
					return TRUE;
				}
			}
		}
	}
	
	return FALSE;
}
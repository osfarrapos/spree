// XWindow_EventIcon.cpp: implementation of the XWindow_EventIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XTextureAnimation.h"
#include "XWindow_Event.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_EventIcon.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_EventIcon::_XWindow_EventIcon() : m_pPresentAni(NULL), m_bShowItem(false)
{
	m_nEventType = 0;
	m_bShowMessageBox = FALSE;
}

_XWindow_EventIcon::~_XWindow_EventIcon()
{

}

void _XWindow_EventIcon::Destroy()
{

}

BOOL _XWindow_EventIcon::Initialize()
{
	int nRes  = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_01.tga" );
		
	m_pPresentAni = new _XTextureAnimation( 0, 0, 46, 46, this, &g_MainInterfaceTextureArchive, nRes,
										  3, 1, 3 );
	m_pPresentAni->ShowWindow( TRUE );

	return TRUE;
}

void _XWindow_EventIcon::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_EventIcon::ShowWindow( BOOL show )
{
	m_pPresentAni->ShowWindow( show );

	_XWindow::ShowWindow(show);
}

void _XWindow_EventIcon::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_EventIcon::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;

	_XWindow::Draw(pfocusobject);

	m_pPresentAni->Update(0.3f);
}

BOOL _XWindow_EventIcon::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(mousestate && !pfocusobject )
	{
		if( mousestate->bButton[0])
		{
			// mouse left down
			if(CheckMousePosition())
			{
				_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );	
				if( pEvent_Window )
					pEvent_Window->m_bBuffEvent = FALSE; //선물 아이콘 클릭했으니,, FALSE 로 바꾸자

				if(m_nEventType == 5)
				{
					// 경공 이벤트
					if(!m_bShowMessageBox)
					{
						ShowMessageBox();
					}
                    else
                    {
					    if( pEvent_Window )
					    {
						    pEvent_Window->ShowWindow( !pEvent_Window->GetShowStatus() );
					    }
                    }
				}
				else
				{
					if( pEvent_Window )
					{
						pEvent_Window->ShowWindow( !pEvent_Window->GetShowStatus() );
					}
				}

				if(pEvent_Window)
				{
					if( pEvent_Window->GetCurrentEventStatus() ||
						pEvent_Window->m_nEventIDinfo || //아이템을 받지 않았다. 진행완료 못한 이벤트가 있다. //m_nEventIDinfo 레벨업 이벤트가 있음.
						pEvent_Window->m_bBuffEvent )
						this->ShowWindow(TRUE);
					else
						this->ShowWindow(FALSE);
				}
			}
		}
	}
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_EventIcon::ShowMessageBox()
{
	_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);

	TCHAR messagestring[256];
	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3812), g_SkillProperty[g_pLocalUser->m_EventSkillList.m_sSkillID]->skillName); //%s를 받았습니다. 무공창을 확인해 주시기 바랍니다.

	pMessageBox->SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), messagestring, TRUE, 0 );
	pMessageBox->StartTimer(10000, TRUE);

	m_bShowMessageBox = TRUE;

	_XWindow_Event* pEvent_Window = (_XWindow_Event*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_EVENT );
	if(pEvent_Window)
	{
		if( pEvent_Window->GetCurrentEventStatus() ||
            pEvent_Window->m_nEventIDinfo || //아이템을 받지 않았다. 진행완료 못한 이벤트가 있다. //m_nEventIDinfo 레벨업 이벤트가 있음.
			pEvent_Window->m_bBuffEvent )
			this->ShowWindow(TRUE);
		else
			this->ShowWindow(FALSE);
	}
}
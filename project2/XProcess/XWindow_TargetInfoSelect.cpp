// _XWindow_TargetInfoSelect.cpp: implementation of the _XWindow_TargetInfoSelect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "SoundEffectList.h"
#include "XWindowObjectDefine.h"
#include "xsr_stringheader.h"

#include "XWindow_MonsterStatus.h"
#include "XWindow_TargetInfoSelect.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_TargetInfoSelect::_XWindow_TargetInfoSelect()
{

}

_XWindow_TargetInfoSelect::~_XWindow_TargetInfoSelect()
{

}

BOOL _XWindow_TargetInfoSelect::Initialize()
{
	int nIndex = g_MainInterfaceTextureArchive.FindResource( "mi_kiup.tga" );
	// 팝업버튼들
	_XBTN_STRUCTURE charinfopopupbtnstruct = 
	{
		TRUE, 
		{2, 2},
		{88, 19},
		_XDEF_MONSTERSTATUS_INFOPOPUPBUTTON1,
		nIndex,
		nIndex,
		nIndex,
		&g_MainInterfaceTextureArchive
	};

	_XButton* CharInfoPopUpButton[4];
	for( int i = 0; i < 4; i++ )
	{
		CharInfoPopUpButton[i] = new _XButton;
		CharInfoPopUpButton[i]->Create(charinfopopupbtnstruct);
		CharInfoPopUpButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 163, 54, 251, 72);
		CharInfoPopUpButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 163, 72, 251, 90);
		CharInfoPopUpButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 163, 90, 251, 108);
		CharInfoPopUpButton[i]->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));		
		CharInfoPopUpButton[i]->EnableWindow(TRUE);
		CharInfoPopUpButton[i]->ShowWindow(TRUE);

		charinfopopupbtnstruct.position.y += 18;
		charinfopopupbtnstruct.commandid++;
	}

	CharInfoPopUpButton[0]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MONSTERSTATUS_2255);
	CharInfoPopUpButton[1]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_1589);
	CharInfoPopUpButton[2]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_1590);
	CharInfoPopUpButton[3]->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NETWORK_1591);

	for( i = 0; i < 4; i++ )
		InsertChildObject(CharInfoPopUpButton[i]);	

	return TRUE;
}

void _XWindow_TargetInfoSelect::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
}

void _XWindow_TargetInfoSelect::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) 
		return;

	_XWindow::Draw(pfocusobject);

	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+87+3, m_WindowPosition.y+72+2, 0.0f,0xFF000000 );
	_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+1, m_WindowPosition.x+87+2, m_WindowPosition.y+72+1, 0.0f,0xFFD2D2D2 );
	_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+2, m_WindowPosition.x+87+1, m_WindowPosition.y+72, 0.0f,0xFF000000 );
}

BOOL _XWindow_TargetInfoSelect::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();

	if( mousestate->bButton[0] && !CheckMousePosition() ) //다른 부분을 클릭하면 닫자
	{
		this->ShowWindow(FALSE);
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_TargetInfoSelect::ShowWindow( BOOL show )
{
	if( show )
	{
		g_MainWindowManager.SetTopWindow(this);
	}
	else
	{
		_XWindow_MonsterStatus* pMonsterStatus_Window = (_XWindow_MonsterStatus*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MONSTERSTATUSWINDOW );
		pMonsterStatus_Window->m_ShowTargetInfoPopUp = FALSE;
	}
	_XWindow::ShowWindow( show );
}
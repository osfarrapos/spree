// XWindow_NameInput.cpp: implementation of the _XWindow_NameInput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_NameInput.h"

#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_WareHouse.h"
#include "XWindow_Inventory.h"
#include "XSR_STRINGHEADER.H"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_NameInputEditReturn( _XIMEContainer* pIMEContainer )
{
//	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();	
		PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NAMEINPUTWINDOW_OK, 0), (LPARAM)gHWnd); //2004.06.24->oneway48 insert
}

void __stdcall _XIMECallback_NameInputEditEscape( _XIMEContainer* pIMEContainer )
{			
//	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();			
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_NAMEINPUTWINDOW_CLOSE, 0), (LPARAM)gHWnd); //2004.06.24->oneway48 insert
}

_XWindow_NameInput::_XWindow_NameInput()
{	
	m_OKButton = NULL;
	m_CancelButton = NULL;
}

_XWindow_NameInput::~_XWindow_NameInput()
{
	DestroyWindow();

}

BOOL _XWindow_NameInput::Initialize(void)
{
	_XImageStatic* pLeftImage = new _XImageStatic;
	pLeftImage->Create(0, 1, 16, 64, 
		&g_MainInterfaceTextureArchive,
		g_MainInterfaceTextureArchive.FindResource("etcwindow.tga"));
	pLeftImage->SetClipRect(32, 39, 48, 102);     
	InsertChildObject(pLeftImage);

	_XImageStatic* pRightImage = new _XImageStatic;
	pRightImage->Create(270, 1, 16, 64, 
		&g_MainInterfaceTextureArchive,
		g_MainInterfaceTextureArchive.FindResource("etcwindow.tga"));
	pRightImage->SetClipRect(48, 39, 64, 102);
	InsertChildObject(pRightImage);

	_XImageStatic* pEditImage = new _XImageStatic;
	pEditImage->Create(83, 23, 117, 40,
		&g_MainInterfaceTextureArchive,
		g_MainInterfaceTextureArchive.FindResource("etcwindow.tga"));
	pEditImage->SetClipRect(50, 0, 168, 17);
	InsertChildObject(pEditImage);

	_XBTN_STRUCTURE okbtnstruct = 
	{
		TRUE, {95, 42}, {46, 17},
			_XDEF_NAMEINPUTWINDOW_OK,
			g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
			g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
			g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
			&g_MainInterfaceTextureArchive
	};
	m_OKButton = new _XButton;
	m_OKButton->Create(okbtnstruct);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 13, 47, 30);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 30, 47, 48);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 47, 47, 64);
	m_OKButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_OKButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK);
	InsertChildObject(m_OKButton);
	
	_XBTN_STRUCTURE cancelbtnstruct = 
	{
		TRUE, {145, 42}, {46, 17},
			_XDEF_NAMEINPUTWINDOW_CLOSE,
			g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
			g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
			g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
			&g_MainInterfaceTextureArchive
	};
	m_CancelButton = new _XButton;
	m_CancelButton->Create(cancelbtnstruct);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 13, 47, 30);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 30, 47, 48);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 47, 47, 64);
	m_CancelButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 214, 15, 15));
	m_CancelButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL);
	InsertChildObject(m_CancelButton);

	m_IMEControl.ResetContainer();
	m_IMEControl.MoveWindow(m_WindowPosition.x + 80, m_WindowPosition.y + 25);
	m_IMEControl.ResizeWindow(113, 12);
	m_IMEControl.ShowWindow(FALSE);
	m_IMEControl.SetMaxLength(17);
	m_IMEControl.SetFocus();
	m_IMEControl.SetAlignType( _XFONT_ALIGNTYPE_RIGHT );

	m_IMEControl.SetEscapeKeyCallback( _XIMECallback_NameInputEditEscape );
	m_IMEControl.SetReturnKeyCallback( _XIMECallback_NameInputEditReturn );

	return TRUE;
}

void _XWindow_NameInput::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_NameInput::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	
	switch(m_NameInputMode)
	{
	case _XNAMEINPUT_GROUPNAME :
		{
			g_XBaseFont->Print(m_WindowPosition.x + 42, m_WindowPosition.y + 8, 1.0f, _XGETINTERFACETEXT(ID_STRING_NAMEINPUT_CREATEGROUP));
		}
		break;
	case _XNAMEINPUT_CHARACTERNAME :
		{
			g_XBaseFont->Print(m_WindowPosition.x + 42, m_WindowPosition.y + 8, 1.0f, _XGETINTERFACETEXT(ID_STRING_NAMEINPUT_CREATEFRIEND));	
		}
		break;
	case _XNAMEINPUT_CHANGEGROUPNAME :
		{
			g_XBaseFont->Print(m_WindowPosition.x + 42, m_WindowPosition.y + 8, 1.0f, _XGETINTERFACETEXT(ID_STRING_NAMEINPUT_RENAMEGROUP));	
		}
		break;
	}
	

	g_XBaseFont->Flush();
	m_IMEControl.Draw();
}

BOOL _XWindow_NameInput::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckIMEBoxArea())
				{
					m_IMEControl.SetFocus();
				}
			}
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

BOOL _XWindow_NameInput::CheckIMEBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(scrnpos->x > (m_WindowPosition.x + 85) &&
		scrnpos->x < (m_WindowPosition.x + 85 + 113) &&
		scrnpos->z > (m_WindowPosition.y + 52) &&
		scrnpos->z < (m_WindowPosition.y + 52 + 12))
		return TRUE;

	return FALSE;
}

void _XWindow_NameInput::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	m_IMEControl.MoveWindow(m_WindowPosition.x + 80, m_WindowPosition.y + 25);
}

void _XWindow_NameInput::ShowWindow(BOOL show)
{
	if(!show)
	{
		m_IMEControl.ClearBuffer();
		_XIMEKERNEL.SetFocus( NULL );
	}
	else
	{
		m_pMyManager->SetTopWindow(this);
		m_IMEControl.SetFocus();
	}
	
	_XWindow::ShowWindow(show);
}

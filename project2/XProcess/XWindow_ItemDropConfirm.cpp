// XWindow_ItemDropConfirm.cpp: implementation of the _XWindow_ItemDropConfirm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "XWindow_ItemDropConfirm.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void __stdcall _XIMECallback_ItemDropConfirmEditReturn( _XIMEContainer* pIMEContainer )
{
//	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();	
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_ITEMDROPCONFIRM_OK, 0), (LPARAM)gHWnd); //2004.06.24->oneway48 insert
}

void __stdcall _XIMECallback_ItemDropConfirmEditEscape( _XIMEContainer* pIMEContainer )
{			
//	CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
//	eodapp->m_proc_maingame.m_ChatEditContainer.SetFocus();			
	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XDEF_ITEMDROPCONFIRM_CLOSE, 0), (LPARAM)gHWnd); //2004.06.24->oneway48 insert
}

_XWindow_ItemDropConfirm::_XWindow_ItemDropConfirm()
{
	m_OKButton = NULL;
	m_CancelButton = NULL;
	
	m_AllButton = NULL;// 2004.05.17->oneway48 insert
	
	m_DropCount = 0;
	m_DropMode = _XDROP_NONE;

	m_SelectedCategory1 = 0;
	m_SelectedItemID = 0;	
	
	m_SelectionQuestID = -1;

	memset(m_CountString, 0, sizeof(TCHAR)*64);
}

_XWindow_ItemDropConfirm::~_XWindow_ItemDropConfirm()
{
	DestroyWindow();
}

BOOL _XWindow_ItemDropConfirm::Initialize(void)
{
	int confirmimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Trade.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 208, 88, &g_MainInterfaceTextureArchive, confirmimageindex );	
	pTitleBar->SetClipRect( 0, 168, 207, 255 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pEditImage = new _XImageStatic;
	pEditImage->Create(29, 35, 127, 53,
		&g_MainInterfaceTextureArchive,
		g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga"));
	pEditImage->SetClipRect(7, 201, 104, 219);
	InsertChildObject(pEditImage);

	int buttonindex = g_MainInterfaceTextureArchive.FindResource("MI_Trade.tga");

	_XBTN_STRUCTURE allbtnstruct = 
	{
		TRUE, {128, 36}, {55, 17},
		_XDEF_ITEMDROPCONFIRM_ALL,
		buttonindex, buttonindex, buttonindex,
		&g_MainInterfaceTextureArchive
	};
	
	m_AllButton = new _XButton;
	m_AllButton->Create(allbtnstruct);
	m_AllButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 78, 244, 95);
	m_AllButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 98, 240, 115);
	m_AllButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 118, 240, 135);
	m_AllButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_AllButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_TRADECONFIRM_ALL);
	InsertChildObject(m_AllButton);
	
	_XBTN_STRUCTURE okbtnstruct = 
	{
		TRUE, {52, 59}, {52, 22},
		_XDEF_ITEMDROPCONFIRM_OK,
		buttonindex, buttonindex, buttonindex,
		&g_MainInterfaceTextureArchive
	};
	m_OKButton = new _XButton;
	m_OKButton->Create(okbtnstruct);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 28, 240, 49);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 3, 240, 24);
	m_OKButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 53, 240, 74);
	m_OKButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_OKButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK);
	InsertChildObject(m_OKButton);

	_XBTN_STRUCTURE cancelbtnstruct = 
	{
		TRUE, {107, 59}, {52, 22},
			_XDEF_ITEMDROPCONFIRM_CLOSE,
			buttonindex, buttonindex, buttonindex,
			&g_MainInterfaceTextureArchive
	};
	m_CancelButton = new _XButton;
	m_CancelButton->Create(cancelbtnstruct);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 189, 28, 240, 49);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 189, 3, 240, 24);
	m_CancelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 189, 53, 240, 74);
	m_CancelButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 214, 15, 15));
	m_CancelButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL);
	InsertChildObject(m_CancelButton);

	m_IMEControl.ResetContainer();
	m_IMEControl.MoveWindow(m_WindowPosition.x + 40, m_WindowPosition.y + 39);
	m_IMEControl.ResizeWindow(76, 12);
	m_IMEControl.ShowWindow(FALSE);
	m_IMEControl.SetMaxLength(8);
	m_IMEControl.SetFocus();
	m_IMEControl.SetAlignType( _XFONT_ALIGNTYPE_RIGHT );
	m_IMEControl.SetNumericEditMode( TRUE, TRUE );

	m_IMEControl.SetEscapeKeyCallback( _XIMECallback_ItemDropConfirmEditEscape );
	m_IMEControl.SetReturnKeyCallback( _XIMECallback_ItemDropConfirmEditReturn );	

	return TRUE;
}

void _XWindow_ItemDropConfirm::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_ItemDropConfirm::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;

	g_XBaseFont->SetColor(_XSC_DEFAULT);	
	
	switch(m_DropMode)
	{
	case _XDROP_NONE :
		{
		}
		break;
		
	case _XDROP_ITEM1 :
		{
			g_XBaseFont->Print(m_WindowPosition.x + 29, m_WindowPosition.y + 17, 1.0f, _XGETINTERFACETEXT(ID_STRING_ITEMDROPCONFIRM_INPUT) ); // 버릴 물품의 개수를 입력하십시오.
		}
		break;
		
	case _XDROP_MONEY1 :
		{	
			g_XBaseFont->Print(m_WindowPosition.x+41, m_WindowPosition.y+17, 1.0f, _XGETINTERFACETEXT(ID_STRING_ITEMDROPCONFIRM_DROPMONEY));						
		}
		break;
	}
	g_XBaseFont->Flush();

	m_IMEControl.Draw();
}

BOOL _XWindow_ItemDropConfirm::Process(_XGUIObject*& pfocusobject)
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

	strcpy(m_CountString, m_IMEControl.GetText());
	m_DropCount = atoi(m_CountString);	

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

BOOL _XWindow_ItemDropConfirm::CheckIMEBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if(scrnpos->x > (m_WindowPosition.x + 40) &&
		scrnpos->x < (m_WindowPosition.x + 40 + 76) &&
		scrnpos->z > (m_WindowPosition.y + 39) &&
		scrnpos->z < (m_WindowPosition.y + 39 + 12))
		return TRUE;

	return FALSE;
}

void _XWindow_ItemDropConfirm::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	m_IMEControl.MoveWindow(m_WindowPosition.x + 40, m_WindowPosition.y + 39);
}

void _XWindow_ItemDropConfirm::ShowWindow(BOOL show)
{
	if(!show)
	{
		memset(m_CountString, 0, sizeof(TCHAR)*64);
		m_IMEControl.ClearBuffer();
		m_DropMode = _XDROP_NONE;
		_XIMEKERNEL.SetFocus( NULL ); // 2004.05.17->oneway48 insert
	}
	else
	{
		m_pMyManager->SetTopWindow(this);
		m_IMEControl.SetFocus(); // 2004.05.17->oneway48 move
	}
	
	_XWindow::ShowWindow(show);

}

void _XWindow_ItemDropConfirm::SetMode( _XDROP_MODE mode )
{
	m_DropMode = mode;
	/*
	switch( mode ) {
	case _XDROP_NONE:
		{
		}
		break;
	case _XDROP_ITEM:
		{			
			m_SelectionQuestID = -1;			
			m_AllButton->ShowWindow(TRUE);
			m_IMEControl.MoveWindow(m_WindowPosition.x + 24, m_WindowPosition.y + 77);
			m_EditImage->MoveWindow( m_WindowPosition.x + 23, m_WindowPosition.y + 75 );
		}
		break;
	case _XDROP_MONEY:
		{
			m_SelectionQuestID = -1;
			m_AllButton->ShowWindow(TRUE);
			m_IMEControl.MoveWindow(m_WindowPosition.x + 24, m_WindowPosition.y + 77);
			m_EditImage->MoveWindow( m_WindowPosition.x + 23, m_WindowPosition.y + 75 );
		}
		break;
	case _XDROP_SELECTION:
		{
			m_AllButton->ShowWindow(FALSE);
			m_IMEControl.MoveWindow(m_WindowPosition.x + 24, m_WindowPosition.y + 188);
			m_EditImage->MoveWindow( m_WindowPosition.x + 23, m_WindowPosition.y + 186 );
		}
		break;
	}*/

}

void _XWindow_ItemDropConfirm::SetMessage( LPSTR messagestring )
{
	/*
	if( !messagestring ) return;
	if( strlen(messagestring) <= 0 ) return;
	
	
	// String splitting....
	_XStringSplitInfo splitinfo;	
	g_XBaseFont->SplitStringByCR(messagestring, splitinfo);
	//g_XBaseFont->SplitString( messagestring, m_WindowSize.cx - 26, splitinfo );
	
	
	TCHAR	buffer[1024];
	for( int i = 0; i < splitinfo.splitcount; i++ )
	{			
		memset(buffer, 0, sizeof(buffer));
		strncpy( buffer, splitinfo.splitpoint[i], splitinfo.splitlength[i] );	
			
		_XStringItem* contentsmessage = new _XStringItem( buffer,_XDEF_CHATMESSAGECOLOR_MYMESSAGE, g_LocalSystemTime );
		
		if( contentsmessage )
		{				
			m_Contentslist.resetList();
			m_Contentslist.insertItem( contentsmessage );
		}		
	}
	

	
	if( m_ContentsListScrollBar )
	{
		m_ContentsListScrollBar->SetTotalLineCount( m_Contentslist.getitemcount() );	
		m_ContentsListScrollBar->UpdateData();
		
	}								*/
	
}
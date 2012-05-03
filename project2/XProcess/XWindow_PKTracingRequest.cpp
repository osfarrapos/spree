// XWindow_PKTracingRequest.cpp: implementation of the _XWindow_PKTracingRequest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_PKTracingRequest.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_PKTracingRequest::_XWindow_PKTracingRequest()
{
	memset(m_cTargetName,0,sizeof(m_cTargetName));
	memset(m_cSpecialTargetName, 0, sizeof(m_cSpecialTargetName));
	memset(m_cDeleteTargetName, 0, sizeof(m_cDeleteTargetName));
	
	m_btnRequest1 = NULL;
	m_btnRequest2 = NULL;
	m_btnRequest3 = NULL;
	m_btnCancle = NULL;

}

_XWindow_PKTracingRequest::~_XWindow_PKTracingRequest()
{

}


BOOL _XWindow_PKTracingRequest::Initialize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_death01.tga");

	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create(0, 0, 212, 18, &g_MainInterfaceTextureArchive, resourceindex);
	pTitleLeftBar->SetClipRect(0, 0, 212, 18);
	InsertChildObject(pTitleLeftBar);

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName);	
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, 
		{5, 20},
		{202, 22},
		_XDEF_PKTRACINGREQUEST_BTN_REQUEST1,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};
	
	TCHAR messagestring[256];
	memset( messagestring, 0, sizeof(TCHAR)*256 );
	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3384), 1);	
	m_btnRequest1 = new _XButton;
	m_btnRequest1->Create(btnstruct);
	m_btnRequest1->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_btnRequest1->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_btnRequest1->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_btnRequest1->SetButtonTextColor(D3DCOLOR_ARGB(255, 233, 231, 232));
	m_btnRequest1->SetButtonText( messagestring );
	InsertChildObject(m_btnRequest1);
	
	memset( messagestring, 0, sizeof(TCHAR)*256 );
	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3384), 24);	
	btnstruct.position.y = 43;
	btnstruct.commandid = _XDEF_PKTRACINGREQUEST_BTN_REQUEST2;
	m_btnRequest2 = new _XButton;
	m_btnRequest2->Create(btnstruct);
	m_btnRequest2->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_btnRequest2->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_btnRequest2->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_btnRequest2->SetButtonTextColor(D3DCOLOR_ARGB(255, 233, 231, 232));
	m_btnRequest2->SetButtonText( messagestring );
	InsertChildObject(m_btnRequest2);
	
	memset( messagestring, 0, sizeof(TCHAR)*256 );
	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3384), 48);	
	btnstruct.position.y = 66;
	btnstruct.commandid = _XDEF_PKTRACINGREQUEST_BTN_REQUEST3;
	m_btnRequest3 = new _XButton;
	m_btnRequest3 = new _XButton;
	m_btnRequest3->Create(btnstruct);
	m_btnRequest3->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_btnRequest3->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_btnRequest3->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_btnRequest3->SetButtonTextColor(D3DCOLOR_ARGB(255, 233, 231, 232));
	m_btnRequest3->SetButtonText( messagestring );
	InsertChildObject(m_btnRequest3);
	
	btnstruct.position.y = 89;
	btnstruct.commandid = _XDEF_PKTRACINGREQUEST_BTN_CANCLE;
	m_btnCancle = new _XButton;
	m_btnCancle->Create(btnstruct);
	m_btnCancle->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 19, 204, 41);
	m_btnCancle->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 42, 204, 64);
	m_btnCancle->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 65, 204, 87);
	m_btnCancle->SetButtonTextColor(D3DCOLOR_ARGB(255, 204, 187, 51));
	m_btnCancle->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3385);
	InsertChildObject(m_btnCancle);

	
	for(int i = 0 ; i < 2 ; i++)
	{
		m_CornerImage[i].Create(0, 0, 3, 3, &g_MainInterfaceTextureArchive, resourceindex);
	}
	m_CornerImage[0].SetClipRect(213, 0, 216, 3);
	m_CornerImage[1].SetClipRect(217, 0, 220, 3);

	return TRUE;
}


void _XWindow_PKTracingRequest::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_PKTracingRequest::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		// 왼쪽 세로 
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+0), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+0), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+1), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+1), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+2), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		// 오른쪽 세로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+209), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+209), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+210), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+210), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+211), (FLOAT)(m_WindowPosition.y+17), (FLOAT)(m_WindowPosition.x+211), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-4), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		// 아래 가로
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+4), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-3), (FLOAT)(m_WindowPosition.x+208), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-3), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+4), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-2), (FLOAT)(m_WindowPosition.x+208), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-2), 
			D3DCOLOR_ARGB(255, 127, 120, 128), D3DCOLOR_ARGB(255, 127, 120, 128));
		_XDrawLine2D((FLOAT)(m_WindowPosition.x+4), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-1), (FLOAT)(m_WindowPosition.x+208), (FLOAT)(m_WindowPosition.y+m_WindowSize.cy-1), 
			D3DCOLOR_ARGB(255, 0, 0, 0), D3DCOLOR_ARGB(255, 0, 0, 0));
		
		m_CornerImage[0].Draw(m_WindowPosition.x+1, m_WindowPosition.y+m_WindowSize.cy-3);
		m_CornerImage[1].Draw(m_WindowPosition.x+208, m_WindowPosition.y+m_WindowSize.cy-3);
	}
	
	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	g_XBaseFont->SetColor(_XSC_DEFAULT);	
	g_XBaseFont->PrintAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 2, 1.0f,
		_XFONT_ALIGNTYPE_CENTER,_XGETINTERFACETEXT(ID_STRING_NEW_3386));
	g_XBaseFont->Flush();
}


BOOL _XWindow_PKTracingRequest::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!this->m_Enable)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
		
	return TRUE;
}

void _XWindow_PKTracingRequest::ShowWindow(BOOL show)
{
	if(show)
	{		
		m_btnRequest1->EnableWindow(TRUE);
		m_btnRequest2->EnableWindow(TRUE);
		m_btnRequest3->EnableWindow(TRUE);
		m_btnCancle->EnableWindow(TRUE);
	}
	_XWindow::ShowWindow(show);
}
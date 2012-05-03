// XWindow_FreeMatchInfo.cpp: implementation of the _XWindow_FreeMatchInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_FreeMatchInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_FreeMatchInfo::_XWindow_FreeMatchInfo()
{
	m_pSubTitle = NULL;
	m_pLeftEdge = NULL;
	m_pRightEdge = NULL;
	m_pTopBar = NULL;
	m_pBottomBar = NULL;

	memset(m_szDate, 0, sizeof(m_szDate));
	memset(m_szLevel, 0, sizeof(m_szLevel));
}

_XWindow_FreeMatchInfo::~_XWindow_FreeMatchInfo()
{

}

BOOL _XWindow_FreeMatchInfo::Initialize()
{
	int mainframeinterface = g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create(0, 0, 180, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleLeftBar->SetClipRect(0, 125, 180, 143);
	InsertChildObject(pTitleLeftBar);
	
	_XImageStatic* pTitleCenterBar = new _XImageStatic;
	pTitleCenterBar->Create(180, 0, 282, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleCenterBar->SetClipRect(77, 125, 180, 143);
	InsertChildObject(pTitleCenterBar);
	
	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create(283, 0, 355, 18, &g_MainInterfaceTextureArchive, mainframeinterface);
	pTitleRightBar->SetClipRect(110, 125, 183, 143);
	InsertChildObject(pTitleRightBar);
	
	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 17, 2, 48, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	pLeftBorder_1->SetScale(1.0f, 7.0f);
	InsertChildObject(pLeftBorder_1);
	
	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(353, 17, 355, 48, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, 7.0f);
	InsertChildObject(pRightBorder_1);
	
	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 241, 2, 265, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftCorner->SetClipRect(0, 205, 3, 230);
	InsertChildObject(pLeftCorner);
	
	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(353, 241, 355, 265, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightCorner->SetClipRect(180, 205, 183, 230);
	InsertChildObject(pRightCorner);
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 263, 179, 265, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	pBottomBorder->SetScale(1.9774f, 1.0f);
	InsertChildObject(pBottomBorder);
	
	m_BorderTitle.Create(m_WindowPosition.x+3, m_WindowPosition.y+18, m_WindowPosition.x+353, m_WindowPosition.y+263);
	m_BorderTitle.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_BorderTile.tga"));
	m_BorderTitle.SetFaceColor(D3DCOLOR_ARGB(128, 255, 255, 255));
	
	FLOAT ufactor = 350.0f / 128.0f;
	FLOAT vfactor = 245.0f / 128.0f;

	m_BorderTitle.ChangeUV(0.0f, ufactor, ufactor, 0.0f, 0.0f, 0.0f, vfactor, vfactor);

	int btnresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {124, 237}, {106, 22},
		_XDEF_NPCSCRIPT_CLOSE,
		btnresourceindex,
		btnresourceindex,
		btnresourceindex,
		&g_MainInterfaceTextureArchive
	};

	_XButton* pBtnClose = new _XButton;
	pBtnClose->Create(btnstruct);
	pBtnClose->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 3, 177, 109, 199);
	pBtnClose->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 3, 153, 109, 175);
	pBtnClose->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 3, 202, 109, 224);
	pBtnClose->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pBtnClose->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);
	InsertChildObject(pBtnClose);

	int charresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_characterwin.tga");

	m_pSubTitle = new _XImageStatic;
	m_pSubTitle->Create(0, 0, 158, 18, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pSubTitle->SetClipRect(0, 23, 158, 41);

	m_pLeftEdge = new _XImageStatic;
	m_pLeftEdge->Create(0, 0, 2, 22, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pLeftEdge->SetClipRect(0, 0, 2, 22);

	m_pRightEdge = new _XImageStatic;
	m_pRightEdge->Create(0, 0, 2, 22, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pRightEdge->SetClipRect(160, 0, 162, 22);

	m_pTopBar = new _XImageStatic;
	m_pTopBar->Create(0, 0, 158, 2, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pTopBar->SetClipRect(2, 0, 160, 2);

	m_pBottomBar = new _XImageStatic;
	m_pBottomBar->Create(0, 0, 158, 2, &g_MainInterfaceTextureArchive, charresourceindex);
	m_pBottomBar->SetClipRect(2, 20, 160, 22);

	return TRUE;
}

void _XWindow_FreeMatchInfo::DestroyWindow( void )
{
	SAFE_DELETE(m_pSubTitle);
	SAFE_DELETE(m_pLeftEdge);
	SAFE_DELETE(m_pRightEdge);
	SAFE_DELETE(m_pTopBar);
	SAFE_DELETE(m_pBottomBar);

	_XWindow::DestroyWindow();
}

void _XWindow_FreeMatchInfo::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		m_BorderTitle.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));
		
//		_XDrawSolidBar(m_WindowPosition.x+10, m_WindowPosition.y+74, m_WindowPosition.x+347, m_WindowPosition.y+228, 0x7f000000);
//		_XDrawRectAngle(m_WindowPosition.x+9, m_WindowPosition.y+73, m_WindowPosition.x+346, m_WindowPosition.y+227, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO

	// titles
	m_pSubTitle->SetScale(334.f/158.f, 1.0f);
	m_pSubTitle->Draw(m_WindowPosition.x+10, m_WindowPosition.y+23);

	m_pLeftEdge->Draw(m_WindowPosition.x+8, m_WindowPosition.y+21);

	m_pRightEdge->Draw(m_WindowPosition.x+344, m_WindowPosition.y+21);

	m_pTopBar->SetScale(334.f/158.f, 1.0f);
	m_pTopBar->Draw(m_WindowPosition.x+10, m_WindowPosition.y+21);

	m_pBottomBar->SetScale(334.f/158.f, 1.0f);
	m_pBottomBar->Draw(m_WindowPosition.x+10, m_WindowPosition.y+41);

	// solid
	_XDrawSolidBar(m_WindowPosition.x+9, m_WindowPosition.y+48, m_WindowPosition.x+346, m_WindowPosition.y+234, 0x7f000000);
	_XDrawRectAngle(m_WindowPosition.x+8, m_WindowPosition.y+47, m_WindowPosition.x+345, m_WindowPosition.y+233, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+27, _XGETINTERFACETEXT(ID_STRING_NEW_3776));//"대회 정보"
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	TCHAR tempString[512];
	memset(tempString, 0, sizeof(tempString));
	sprintf(tempString, _T("%s : %s"), _XGETINTERFACETEXT(ID_STRING_NEW_3777), m_szDate);//"다음 영웅대회"
	g_XBaseFont->Print(m_WindowPosition.x+19, m_WindowPosition.y+57, 1.0f, tempString);
	g_XBaseFont->Flush();

	memset(tempString, 0, sizeof(tempString));
	sprintf(tempString, _T("%s : %s"), _XGETINTERFACETEXT(ID_STRING_NEW_3778), m_szLevel);//"참가자"
	g_XBaseFont->Print(m_WindowPosition.x+19, m_WindowPosition.y+80, 1.0f, tempString);
	g_XBaseFont->Flush();
}

BOOL _XWindow_FreeMatchInfo::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if( !_XWindow::Process(pfocusobject) ) 
		return FALSE;

	return TRUE;
}

void _XWindow_FreeMatchInfo::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTitle.MoveWindow(X+3, Y+18);
}

void _XWindow_FreeMatchInfo::ShowWindow(BOOL show)
{
	if(show)
	{
		// 정보 요청
		g_NetworkKernel.SendPacket(MSG_NO_FREE_MATCH, en_gsfm_next_match_schedule);
	}
	else
	{
		memset(m_szDate, 0, sizeof(m_szDate));
		memset(m_szLevel, 0, sizeof(m_szLevel));
	}

	_XWindow::ShowWindow(show);
}
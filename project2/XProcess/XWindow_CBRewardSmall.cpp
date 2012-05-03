// XWindow_CBRewardSmall.cpp: implementation of the _XWindow_CBRewardSmall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_CBRewardSmall.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CBRewardSmall::_XWindow_CBRewardSmall()
{
	nPoint	= 0;
	nFame	= 0;
	dwMoney	= 0;
}

_XWindow_CBRewardSmall::~_XWindow_CBRewardSmall()
{

}

BOOL _XWindow_CBRewardSmall::Initialize()
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga");

	// 왼쪽 세로 테두리
	_XImageStatic* pLeftBar = new _XImageStatic;
	pLeftBar->Create(0, 0, 3, 438, &g_MainInterfaceTextureArchive, resourceindex);
	pLeftBar->SetClipRect(251, 2, 254, 98);
	pLeftBar->SetScale(1.0f, 177.f/96.f);
	InsertChildObject(pLeftBar);

	// 오른쪽 세로 테두리
	_XImageStatic* pRightBar = new _XImageStatic;
	pRightBar->Create(344, 0, 300+3, 438, &g_MainInterfaceTextureArchive, resourceindex);
	pRightBar->SetClipRect(251, 2, 254, 98);
	pRightBar->SetScale(1.0f, 177.f/96.f);
	InsertChildObject(pRightBar);

	// 위 가로 테두리
	_XImageStatic* pUpBar = new _XImageStatic;
	pUpBar->Create(0, 0, 81, 4, &g_MainInterfaceTextureArchive, resourceindex);
	pUpBar->SetClipRect(173, 101, 254, 105);
	pUpBar->SetScale(347.f/81.f, 1.0f);
	InsertChildObject(pUpBar);

	// 바닥 바1 ( 155 X 13 )
	_XImageStatic* pBottomBar1 = new _XImageStatic;
	pBottomBar1->Create(-3, 177, 174, 189, &g_MainInterfaceTextureArchive, resourceindex);
	pBottomBar1->SetClipRect(2, 2, 157, 15);
	pBottomBar1->SetScale(177.0f/155.0f, 1.0f);
	InsertChildObject( pBottomBar1);

	// 바닥 바2 ( 155 X 13 )
	_XImageStatic* pBottomBar2 = new _XImageStatic;
	pBottomBar2->Create(174, 177, 174+175, 189, &g_MainInterfaceTextureArchive, resourceindex);
	pBottomBar2->SetClipRect(2, 17, 157, 30);
	pBottomBar2->SetScale(177.0f/155.0f, 1.0f);
	InsertChildObject( pBottomBar2);

	// 위 장식
	_XImageStatic* pUpDecoImage = new _XImageStatic;
	pUpDecoImage->Create(75, -38, 75+195, -38+51, &g_MainInterfaceTextureArchive, resourceindex);
	pUpDecoImage->SetClipRect(59, 203, 254, 254);
	InsertChildObject(pUpDecoImage);

	_XImageStatic* pULine = new _XImageStatic;
	pULine->Create(60, 45, 60+81, 45+4, &g_MainInterfaceTextureArchive, resourceindex);
	pULine->SetClipRect(173, 101, 254, 105);
	pULine->SetScale(228.f/81.f, 1.0f);
	InsertChildObject(pULine);

	_XImageStatic* pBLine = new _XImageStatic;
	pBLine->Create(60, 123, 60+81, 123+4, &g_MainInterfaceTextureArchive, resourceindex);
	pBLine->SetClipRect(173, 108, 254, 112);
	pBLine->SetScale(228.f/81.f, 1.0f);		// 578/81 = 7.136
	InsertChildObject(pBLine);

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {124, 143}, {93, 23},
		_XDEF_CBREWARDSMALL_OK,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};

	_XButton* pOKButton = new _XButton;
	pOKButton->Create(btnstruct);
	pOKButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 129, 93, 152);
	pOKButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 154, 93, 177);
	pOKButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 179, 93, 202);

	pOKButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pOKButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2497);
	InsertChildObject(pOKButton);

	return TRUE;
}

void _XWindow_CBRewardSmall::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_CBRewardSmall::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		_XDrawSolidBar(m_WindowPosition.x+3, m_WindowPosition.y+4, m_WindowPosition.x+344, m_WindowPosition.y+177, 0x7f000000);
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
	g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XLargeFont->SetColor(D3DCOLOR_ARGB(255, 255, 227, 164));
	g_XLargeFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+20, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3639));// "장원전 기여 보상"

	g_XLargeFont->Puts(m_WindowPosition.x+71, m_WindowPosition.y+53, _XGETINTERFACETEXT(ID_STRING_NEW_3630));	// "대전점수"
	g_XLargeFont->Puts(m_WindowPosition.x+71, m_WindowPosition.y+77, _XGETINTERFACETEXT(ID_STRING_CINFO_FAME));	// 명성
	g_XLargeFont->Puts(m_WindowPosition.x+71, m_WindowPosition.y+103, _XGETINTERFACETEXT(ID_STRING_NEW_3633));	// "포상금"
	
	g_XLargeFont->PrintAlign(m_WindowPosition.x+296, m_WindowPosition.y+53, 1.0f, _XFONT_ALIGNTYPE_RIGHT,"%d", nPoint);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+296, m_WindowPosition.y+77, 1.0f, _XFONT_ALIGNTYPE_RIGHT,"%d", nFame);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+296, m_WindowPosition.y+103, 1.0f, _XFONT_ALIGNTYPE_RIGHT,"%d", dwMoney);

	g_XLargeFont->Flush();
}

BOOL _XWindow_CBRewardSmall::Process(_XGUIObject*& pfocusobject)
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

void _XWindow_CBRewardSmall::ShowWindow(BOOL show)
{
	if(!show)
	{
		nPoint	= 0;
		nFame	= 0;
		dwMoney	= 0;
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_CBRewardSmall::SetAward(MSG_CB_RECORD_PVP_AWARD* award)
{
	nPoint	= award->sPvPPoint;
	nFame	= award->iFame;
	dwMoney = award->uiMoney;
}
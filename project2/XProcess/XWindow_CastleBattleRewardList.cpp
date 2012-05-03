// XWindow_CastleBattleRewardList.cpp: implementation of the _XWindow_CastleBattleRewardList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_CastleBattleRewardList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CastleBattleRewardList::_XWindow_CastleBattleRewardList()
{
	nRank	= 0;
	nPoint	= 0;
	dwExp	= 0;
	nFame	= 0;
	dwMoney = 0;
	
	nNormalPoint	= 0;
	nNormalFame		= 0;
	dwNormalMoney	= 0;
}

_XWindow_CastleBattleRewardList::~_XWindow_CastleBattleRewardList()
{

}

BOOL _XWindow_CastleBattleRewardList::Initialize(void)
{
	int manorresourceindex02	= g_MainInterfaceTextureArchive.FindResource("MI_manor02.tga");
	
	// 타이틀 바 ( 195 X 51 )
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(88, 0, 88+195, 51, &g_MainInterfaceTextureArchive, manorresourceindex02);
	pTitleBar->SetClipRect(59, 203, 254, 254);	
	InsertChildObject( pTitleBar);

	// 바닥 바1 ( 155 X 13 )
	_XImageStatic* pBottomBar1 = new _XImageStatic;
	pBottomBar1->Create(-6, 388, 149, 401, &g_MainInterfaceTextureArchive, manorresourceindex02);
	pBottomBar1->SetClipRect(2, 2, 157, 15);
	pBottomBar1->SetScale(195.5f/155.0f, 1.0f);
	InsertChildObject( pBottomBar1);
	// 바닥 바2 ( 155 X 13 )
	_XImageStatic* pBottomBar2 = new _XImageStatic;
	pBottomBar2->Create(187, 388, 187+155, 401, &g_MainInterfaceTextureArchive, manorresourceindex02);
	pBottomBar2->SetClipRect(2, 17, 157, 30);
	pBottomBar2->SetScale(195.5f/155.0f, 1.0f);
	InsertChildObject( pBottomBar2);	
	
	// 장원전 포상내역 닫기 버튼  ( 91 X 23 )
	_XBTN_STRUCTURE okbtnstruct = { TRUE, { 142, 359 }, { 91, 23 }, _XDEF_CASTLEBATTLE_REWARDLIST_BTN_OK,
		manorresourceindex02,manorresourceindex02,manorresourceindex02,	&g_MainInterfaceTextureArchive };
	_XButton* pBtnOK = new _XButton;
	pBtnOK->Create( okbtnstruct );	
	
	pBtnOK->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 129, 93, 152 );
	pBtnOK->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 93, 175 );	
	pBtnOK->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 177, 93, 200 );
	pBtnOK->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pBtnOK->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2497);
	InsertChildObject( pBtnOK );
	
	_XImageStatic* pLineImg01 = new _XImageStatic;
	pLineImg01->Create(39, 83, 39+81, 83+4, &g_MainInterfaceTextureArchive, manorresourceindex02);
	pLineImg01->SetClipRect(173, 101, 254, 105);
	pLineImg01->SetScale(297.f/81.f, 1.0f);
	InsertChildObject(pLineImg01);

	_XImageStatic* pLineImg02 = new _XImageStatic;
	pLineImg02->Create(39, 209, 39+81, 209+4, &g_MainInterfaceTextureArchive, manorresourceindex02);
	pLineImg02->SetClipRect(173, 101, 254, 105);
	pLineImg02->SetScale(297.f/81.f, 1.0f);
	InsertChildObject(pLineImg02);

	_XImageStatic* pLineImg03 = new _XImageStatic;
	pLineImg03->Create(39, 259, 39+81, 259+4, &g_MainInterfaceTextureArchive, manorresourceindex02);
	pLineImg03->SetClipRect(173, 101, 254, 105);
	pLineImg03->SetScale(297.f/81.f, 1.0f);
	InsertChildObject(pLineImg03);

	_XImageStatic* pLineImg04 = new _XImageStatic;
	pLineImg04->Create(39, 338, 39+81, 338+4, &g_MainInterfaceTextureArchive, manorresourceindex02);
	pLineImg04->SetClipRect(173, 101, 254, 105);
	pLineImg04->SetScale(297.f/81.f, 1.0f);
	InsertChildObject(pLineImg04);
	
	return TRUE;
}

void _XWindow_CastleBattleRewardList::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_CastleBattleRewardList::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{		
		_XDrawSolidBar( m_WindowPosition.x-1, m_WindowPosition.y+41, m_WindowPosition.x+370, m_WindowPosition.y+392 , 0x7f000000 );
		_XDrawRectAngle( m_WindowPosition.x-3, m_WindowPosition.y+38, m_WindowPosition.x+372, m_WindowPosition.y+390 , 0.0f, 0xFF000000 );
		_XDrawRectAngle( m_WindowPosition.x-2, m_WindowPosition.y+39, m_WindowPosition.x+371, m_WindowPosition.y+391 , 0.0f, D3DCOLOR_ARGB(255,236,186,135) );
		_XDrawLine2D( m_WindowPosition.x-1, m_WindowPosition.y+40, m_WindowPosition.x+370, m_WindowPosition.y+40, D3DCOLOR_ARGB(255,84,65,48), D3DCOLOR_ARGB(255,84,65,48));
		_XDrawRectAngle( m_WindowPosition.x-1, m_WindowPosition.y+41, m_WindowPosition.x+370, m_WindowPosition.y+392 , 0.0f, 0xFF000000 );
	
	}

	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
	g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XLargeFont->SetColor(D3DCOLOR_ARGB(255, 255, 227, 164));
	g_XLargeFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+57, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3640));	// "장원전 특별 보상"

	g_XLargeFont->Puts(m_WindowPosition.x+48, m_WindowPosition.y+90, _XGETINTERFACETEXT(ID_STRING_NEW_3629));	// "순위"
	g_XLargeFont->Puts(m_WindowPosition.x+48, m_WindowPosition.y+90+24, _XGETINTERFACETEXT(ID_STRING_NEW_3630));	// "대전점수"
	g_XLargeFont->Puts(m_WindowPosition.x+48, m_WindowPosition.y+90+24*2, _XGETINTERFACETEXT(ID_STRING_NEW_3632));	// "경험치"
	g_XLargeFont->Puts(m_WindowPosition.x+48, m_WindowPosition.y+90+24*3, _XGETINTERFACETEXT(ID_STRING_CINFO_FAME));	//명성
	g_XLargeFont->Puts(m_WindowPosition.x+48, m_WindowPosition.y+90+24*4, _XGETINTERFACETEXT(ID_STRING_NEW_3633));	// "포상금"

	g_XLargeFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+234, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3639));	// "장원전 기여 보상"

	g_XLargeFont->Puts(m_WindowPosition.x+48, m_WindowPosition.y+267, _XGETINTERFACETEXT(ID_STRING_NEW_3630));	// "대전점수"
	g_XLargeFont->Puts(m_WindowPosition.x+48, m_WindowPosition.y+267+22, _XGETINTERFACETEXT(ID_STRING_CINFO_FAME));	// 명성
	g_XLargeFont->Puts(m_WindowPosition.x+48, m_WindowPosition.y+267+22*2, _XGETINTERFACETEXT(ID_STRING_NEW_3633));	// "포상금"

	TCHAR messagestr[64];
	memset(messagestr, 0, sizeof(messagestr));
	sprintf(messagestr, "%%d");
	strcat(messagestr, _XGETINTERFACETEXT(ID_STRING_NEW_3665) ); //위
	sprintf(messagestr, messagestr, nRank);		
	g_XLargeFont->PrintAlign(m_WindowPosition.x+328, m_WindowPosition.y+90, 1.0f, _XFONT_ALIGNTYPE_RIGHT, messagestr);
	//g_XLargeFont->PrintAlign(m_WindowPosition.x+256, m_WindowPosition.y+90, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_3564)"%d위", nRank);

	g_XLargeFont->PrintAlign(m_WindowPosition.x+328, m_WindowPosition.y+90+24, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", nPoint);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+328, m_WindowPosition.y+90+24*2, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", dwExp);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+328, m_WindowPosition.y+90+24*3, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", nFame);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+328, m_WindowPosition.y+90+24*4, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", dwMoney);

	g_XLargeFont->PrintAlign(m_WindowPosition.x+328, m_WindowPosition.y+267, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", nNormalPoint);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+328, m_WindowPosition.y+267+22, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", nNormalFame);
	g_XLargeFont->PrintAlign(m_WindowPosition.x+328, m_WindowPosition.y+267+22*2, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", dwNormalMoney);

	g_XLargeFont->Flush();
	g_XLargeFont->DisableGrowMode();

}

BOOL _XWindow_CastleBattleRewardList::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_CastleBattleRewardList::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
}

void _XWindow_CastleBattleRewardList::ShowWindow(BOOL show)
{	
	if( !show )
	{
		// reset data
		nRank	= 0;
		nPoint	= 0;
		dwExp	= 0;
		nFame	= 0;
		dwMoney = 0;

		nNormalPoint	= 0;
		nNormalFame		= 0;
		dwNormalMoney	= 0;
	}
	_XWindow::ShowWindow(show);
}

void _XWindow_CastleBattleRewardList::SetSpecialAward(MSG_CB_RECORD_WINNER_AWARD* award)
{
	nRank	= award->sRank;
	nPoint	= award->sCombatPoint;
	dwExp	= award->uiExp;
	nFame	= award->iHonor;
	dwMoney = award->uiMoney;
}

void _XWindow_CastleBattleRewardList::SetNormalAward(MSG_CB_RECORD_PVP_AWARD* award)
{
	nNormalPoint	= award->sPvPPoint;
	nNormalFame		= award->iFame;
	dwNormalMoney	= award->uiMoney;
}
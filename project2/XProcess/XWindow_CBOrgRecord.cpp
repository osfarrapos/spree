// XWindow_CBOrgRecord.cpp: implementation of the _XWindow_CBOrgRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_CBOrgRecord.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CBOrgRecord::_XWindow_CBOrgRecord()
{
	memset(m_Rank, 0, sizeof(m_Rank));
}

_XWindow_CBOrgRecord::~_XWindow_CBOrgRecord()
{

}

BOOL _XWindow_CBOrgRecord::Initialize()
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga");

	// 왼쪽 세로 테두리
	_XImageStatic* pLeftBar = new _XImageStatic;
	pLeftBar->Create(0, 0, 3, 438, &g_MainInterfaceTextureArchive, resourceindex);
	pLeftBar->SetClipRect(251, 2, 254, 98);
	pLeftBar->SetScale(1.0f, 4.5625f);		// 438/96 = 4.5625
	InsertChildObject(pLeftBar);

	// 오른쪽 세로 테두리
	_XImageStatic* pRightBar = new _XImageStatic;
	pRightBar->Create(701, 0, 701+3, 438, &g_MainInterfaceTextureArchive, resourceindex);
	pRightBar->SetClipRect(251, 2, 254, 98);
	pRightBar->SetScale(1.0f, 4.5625f);		// 438/96 = 4.5625
	InsertChildObject(pRightBar);

	// 위 가로 테두리
	_XImageStatic* pUpBar = new _XImageStatic;
	pUpBar->Create(0, 0, 81, 4, &g_MainInterfaceTextureArchive, resourceindex);
	pUpBar->SetClipRect(173, 101, 254, 105);
	pUpBar->SetScale(8.6914f, 1.0f);			// 704/81 = 8.6914
	InsertChildObject(pUpBar);

	// 아래 가로 테두리
	_XImageStatic* pBottomBar = new _XImageStatic;
	pBottomBar->Create(0, 434, 81, 434+4, &g_MainInterfaceTextureArchive, resourceindex);
	pBottomBar->SetClipRect(173, 108, 254, 112);
	pBottomBar->SetScale(8.6914f, 1.0f);		// 704/81 = 8.6914
	InsertChildObject(pBottomBar);

	// 위 장식
	_XImageStatic* pUpDecoImage = new _XImageStatic;
	pUpDecoImage->Create(256, -38, 256+195, -38+51, &g_MainInterfaceTextureArchive, resourceindex);
	pUpDecoImage->SetClipRect(59, 203, 254, 254);
	InsertChildObject(pUpDecoImage);

	// 왼쪽 위 장식
	_XImageStatic* pULImage = new _XImageStatic;
	pULImage->Create(3, 4, 3+47, 4+61, &g_MainInterfaceTextureArchive, resourceindex);
	pULImage->SetClipRect(196, 3, 243, 64);
	InsertChildObject(pULImage);

	// 오른쪽 위 장식
	_XImageStatic* pURImage = new _XImageStatic;
	pURImage->Create(655, 4, 655+47, 4+61, &g_MainInterfaceTextureArchive, resourceindex);
	pURImage->SetClipRect(196, 3, 243, 64);
	InsertChildObject(pURImage);

	// 왼쪽 아래 장식
	_XImageStatic* pBLImage = new _XImageStatic;
	pBLImage->Create(3, 406, 3+28, 406+28, &g_MainInterfaceTextureArchive, resourceindex);
	pBLImage->SetClipRect(180, 70, 208, 98);
	InsertChildObject(pBLImage);

	// 오른쪽 아래 장식
	_XImageStatic* pBRImage = new _XImageStatic;
	pBRImage->Create(673, 406, 673+28, 406+28, &g_MainInterfaceTextureArchive, resourceindex);
	pBRImage->SetClipRect(214, 70, 242, 98);
	InsertChildObject(pBRImage);

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {307, 407}, {91, 23},
		_XDEF_CBORGRECORD_OK,
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

	_XImageStatic* pULine = new _XImageStatic;
	pULine->Create(64, 53, 64+81, 53+4, &g_MainInterfaceTextureArchive, resourceindex);
	pULine->SetClipRect(173, 101, 254, 105);
	pULine->SetScale(7.136f, 1.0f);			// 578/81 = 7.136
	InsertChildObject(pULine);

	_XImageStatic* pMLine = new _XImageStatic;
	pMLine->Create(64, 83, 64+81, 83+4, &g_MainInterfaceTextureArchive, resourceindex);
	pMLine->SetClipRect(173, 101, 254, 105);
	pMLine->SetScale(7.136f, 1.0f);			// 578/81 = 7.136
	InsertChildObject(pMLine);

	_XImageStatic* pBLine = new _XImageStatic;
	pBLine->Create(64, 381, 64+81, 381+4, &g_MainInterfaceTextureArchive, resourceindex);
	pBLine->SetClipRect(173, 108, 254, 112);
	pBLine->SetScale(7.136f, 1.0f);		// 578/81 = 7.136
	InsertChildObject(pBLine);


	return TRUE;
}

void _XWindow_CBOrgRecord::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_CBOrgRecord::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		_XDrawSolidBar(m_WindowPosition.x+3, m_WindowPosition.y+4, m_WindowPosition.x+701, m_WindowPosition.y+434, 0x7f000000);
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
	g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XLargeFont->SetColor(D3DCOLOR_ARGB(255, 255, 227, 164));
	g_XLargeFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+19, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3628));	// "장원전에 큰 공을 세운 협객님의 순위입니다."

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
	{
		g_XLargeFont->Puts(m_WindowPosition.x+10, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3629));// "순위"
		g_XLargeFont->Puts(m_WindowPosition.x+114, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3630));//"대전점수"
		g_XLargeFont->Puts(m_WindowPosition.x+252, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3631));// "이 름"
		g_XLargeFont->Puts(m_WindowPosition.x+340, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3632));//"경험치"
		g_XLargeFont->Puts(m_WindowPosition.x+496, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_CINFO_FAME));// 명성
		g_XLargeFont->Puts(m_WindowPosition.x+589, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3633));// "포상금"
	}
	else
	{
		g_XLargeFont->Puts(m_WindowPosition.x+70, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3629));								// "순위"
		g_XLargeFont->Puts(m_WindowPosition.x+132, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3630));							//"대전점수"
		g_XLargeFont->PutsAlign(m_WindowPosition.x+295, m_WindowPosition.y+59, _XFONT_ALIGNTYPE_CENTER,_XGETINTERFACETEXT(ID_STRING_NEW_3631));// "이 름"
		g_XLargeFont->Puts(m_WindowPosition.x+385, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3632));							//"경험치"
		g_XLargeFont->Puts(m_WindowPosition.x+492, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_CINFO_FAME));								// 명성
		g_XLargeFont->Puts(m_WindowPosition.x+565, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_NEW_3633));							// "포상금"
	}

	for(int i = 0 ; i < 10 ; ++i)
	{
		if(m_Rank[i].cName[0] == 0)
		{
			g_XLargeFont->PrintAlign(m_WindowPosition.x+96, m_WindowPosition.y+91+i*30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", i+1);
			g_XLargeFont->PrintAlign(m_WindowPosition.x+349, m_WindowPosition.y+91+i*30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "---");
		}
		else
		{
			g_XLargeFont->PrintAlign(m_WindowPosition.x+96, m_WindowPosition.y+91+i*30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_Rank[i].nRank);
			g_XLargeFont->PrintAlign(m_WindowPosition.x+349, m_WindowPosition.y+91+i*30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%s", m_Rank[i].cName);
		}

		g_XLargeFont->PrintAlign(m_WindowPosition.x+187, m_WindowPosition.y+91+i*30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_Rank[i].nCombatPoint);
		g_XLargeFont->PrintAlign(m_WindowPosition.x+457, m_WindowPosition.y+91+i*30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_Rank[i].dwExp);
		g_XLargeFont->PrintAlign(m_WindowPosition.x+529, m_WindowPosition.y+91+i*30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_Rank[i].nFame);
		g_XLargeFont->PrintAlign(m_WindowPosition.x+637, m_WindowPosition.y+91+i*30, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_Rank[i].dwMoney);
	}

	g_XLargeFont->Flush();
	g_XLargeFont->DisableGrowMode();
}

BOOL _XWindow_CBOrgRecord::Process( _XGUIObject*& pfocusobject )
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

void _XWindow_CBOrgRecord::SetRank(_OneRank* rank)
{
	int startindex = 0;

	for(int i = 0 ; i < 10 ; ++i)
	{
		if(rank[i].charName[0] == 0)
			continue;

		startindex = rank[i].sRank-1;
		break;
	}

	for(i = startindex ; i < 10 ; ++i)
	{
		strcpy(m_Rank[i].cName, rank[i-startindex].charName);

		m_Rank[i].nRank			= rank[i-startindex].sRank;
		m_Rank[i].nCombatPoint	= rank[i-startindex].sCombatPoint;
		m_Rank[i].dwExp			= rank[i-startindex].uiExp;
		m_Rank[i].nFame			= rank[i-startindex].iHonor;
		m_Rank[i].dwMoney		= rank[i-startindex].uiMoney;
	}
}

void _XWindow_CBOrgRecord::ShowWindow(BOOL show)
{
	if(!show)
	{
		memset(m_Rank, 0, sizeof(m_Rank));
	}

	_XWindow::ShowWindow(show);
}
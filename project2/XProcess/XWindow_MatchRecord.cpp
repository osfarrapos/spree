// XWindow_MatchRecord.cpp: implementation of the _XWindow_MatchRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_MatchRecord.h"
#include "XWindow_MatchTablePVP.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchRecord::_XWindow_MatchRecord()
{

}

_XWindow_MatchRecord::~_XWindow_MatchRecord()
{

}

BOOL _XWindow_MatchRecord::Initialize()
{
	int mainframeinterface = g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");

	int pvpresourceindex = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pvp_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pvp_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pvp_01.tga");
	}
	else
	{
		pvpresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_pvp_01.tga");
	}
	
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create( 0, 0, 175, 23, &g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleLeftBar->SetClipRect( 0, 0, 176, 24 );
	InsertChildObject( pTitleLeftBar );
	
	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create( 306, 0, 306, 23, &g_MainInterfaceTextureArchive, mainframeinterface );
	pTitleRightBar->SetClipRect( 0, 0, 176, 24 );
	pTitleRightBar->SetScale( -1, 1 );
	InsertChildObject( pTitleRightBar );
	
	_XBTN_STRUCTURE btnstruct = 
	{ 
		TRUE, { 274, 5 }, { 12, 12 }, 
			-1,
			g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
			g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
			g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
			&g_MainInterfaceTextureArchive 
	};
	
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( btnstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	
	btnstruct.commandid = _XDEF_MATCHRECORD_CLOSE;
	btnstruct.position.x = 287;
	btnstruct.position.y = 5;
	
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( btnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );
	
	m_BorderTile.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 305, m_WindowPosition.y + 360);
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	FLOAT ufactor = 304.0f/128.0f;
	FLOAT vfactor = 336.0f/128.0f;
	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );

	// 가장자리 선
	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 24, 2, 56, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	pLeftBorder_1->SetScale(1.0f, 10.4f);
	InsertChildObject(pLeftBorder_1);
	
	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(303, 24, 305, 56, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, 10.4f);
	InsertChildObject(pRightBorder_1);

	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 336, 2, 360, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftCorner->SetClipRect(0, 205, 3, 230);
	InsertChildObject(pLeftCorner);
	
	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(303, 336, 305, 360, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightCorner->SetClipRect(180, 205, 183, 230);
	InsertChildObject(pRightCorner);
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 358, 179, 360, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	pBottomBorder->SetScale(1.695f, 1.0f);
	InsertChildObject(pBottomBorder);

	_XCHECKBTN_STRUCTURE chkbtnstruct = 
	{
		TRUE, {11, 29}, {66, 26},
		_XDEF_MATCHINFO_TAB,
		g_MainInterfaceTextureArchive.FindResource("mi_quest2.tga"),
		g_MainInterfaceTextureArchive.FindResource("mi_quest2.tga"),
		&g_MainInterfaceTextureArchive
	};

	m_chkbtnMatchInfo = new _XCheckButton;
	m_chkbtnMatchInfo->Create(chkbtnstruct);
	m_chkbtnMatchInfo->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2493);
	m_chkbtnMatchInfo->SetGrowTextMode(TRUE, D3DCOLOR_ARGB(255, 0, 0, 0));
	m_chkbtnMatchInfo->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_chkbtnMatchInfo->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 67, 230, 131, 254);
	m_chkbtnMatchInfo->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 2, 230, 66, 254);
	m_chkbtnMatchInfo->SetCheck(TRUE);
	m_chkbtnMatchInfo->EnableWindow(TRUE);
	InsertChildObject(m_chkbtnMatchInfo);
	
	chkbtnstruct.position.x = 83;
	chkbtnstruct.position.y = 29;
	chkbtnstruct.commandid = _XDEF_MATCHTABLEPVP_TAB;
	
	m_chkbtnMatchTablePVP = new _XCheckButton;
	m_chkbtnMatchTablePVP->Create(chkbtnstruct);
	m_chkbtnMatchTablePVP->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2494);
	m_chkbtnMatchTablePVP->SetGrowTextMode(TRUE, D3DCOLOR_ARGB(255, 0, 0, 0));
	m_chkbtnMatchTablePVP->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_chkbtnMatchTablePVP->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 67, 230, 131, 254);
	m_chkbtnMatchTablePVP->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 2, 230, 66, 254);
	m_chkbtnMatchTablePVP->SetCheck(FALSE);
	m_chkbtnMatchTablePVP->EnableWindow(FALSE);
	InsertChildObject(m_chkbtnMatchTablePVP);
	
	chkbtnstruct.position.x = 155;
	chkbtnstruct.position.y = 29; 
	chkbtnstruct.commandid = _XDEF_MATCHTABLEGROUP_TAB;
	
	m_chkbtnMatchTableGroup = new _XCheckButton;
	m_chkbtnMatchTableGroup->Create(chkbtnstruct);
	m_chkbtnMatchTableGroup->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2495);
	m_chkbtnMatchTableGroup->SetGrowTextMode(TRUE, D3DCOLOR_ARGB(255, 0, 0, 0));
	m_chkbtnMatchTableGroup->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_chkbtnMatchTableGroup->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 67, 230, 131, 254);
	m_chkbtnMatchTableGroup->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 2, 230, 66, 254);
	m_chkbtnMatchTableGroup->SetCheck(FALSE);
	m_chkbtnMatchTableGroup->EnableWindow(FALSE);
	InsertChildObject(m_chkbtnMatchTableGroup);
	
	chkbtnstruct.position.x = 228;
	chkbtnstruct.position.y = 29;
	chkbtnstruct.commandid = _XDEF_MATCHTABLERACE_TAB;
	
	m_chkbtnMatchTableRage = new _XCheckButton;
	m_chkbtnMatchTableRage->Create(chkbtnstruct);
	m_chkbtnMatchTableRage->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2496);
	m_chkbtnMatchTableRage->SetGrowTextMode(TRUE, D3DCOLOR_ARGB(255, 0, 0, 0));
	m_chkbtnMatchTableRage->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_chkbtnMatchTableRage->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 67, 230, 131, 254);
	m_chkbtnMatchTableRage->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 2, 230, 66, 254);
	m_chkbtnMatchTableRage->SetCheck(FALSE);
	m_chkbtnMatchTableRage->EnableWindow(FALSE);
	InsertChildObject(m_chkbtnMatchTableRage);

	// 이름 바닥
	_XImageStatic* pNameBorderLeft = new _XImageStatic;
	pNameBorderLeft->Create(12, 83, 152, 120, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pNameBorderLeft->SetClipRect(0, 0, 141, 38);
	InsertChildObject(pNameBorderLeft);
	
	_XImageStatic* pNameBorderRight = new _XImageStatic;
	pNameBorderRight->Create(293, 83, 293, 120, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pNameBorderRight->SetClipRect(0, 0, 141, 38);
	pNameBorderRight->SetScale(-1, 1);
	InsertChildObject(pNameBorderRight); 

	// 제목 바닥
	int trainingresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_tra_base.tga");
	
	m_LeftTitleBarImage.Create(0, 0, 170, 22, &g_MainInterfaceTextureArchive, trainingresourceindex);
	m_LeftTitleBarImage.SetClipRect(2, 19, 171, 40);
	m_RightTitleBarImage.Create(0, 0, 140, 22, &g_MainInterfaceTextureArchive, trainingresourceindex);
	m_RightTitleBarImage.SetClipRect(64, 19, 204, 41);

	_XBTN_STRUCTURE bigbtnstruct = 
	{
		TRUE, {98, 331}, {106, 22},
		_XDEF_MATCHRECORD_CLOSE,
		g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga"),
		g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga"),
		g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga"),
		&g_MainInterfaceTextureArchive
	};
	
	m_btnConfirm = new _XButton;
	m_btnConfirm->Create(bigbtnstruct);
	m_btnConfirm->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 3, 177, 109, 199);
	m_btnConfirm->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 3, 153, 109, 175);
	m_btnConfirm->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 3, 202, 109, 224);
	m_btnConfirm->SetGrowTextMode(TRUE);
	m_btnConfirm->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2497);
	m_btnConfirm->SetButtonTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	InsertChildObject(m_btnConfirm);
	
	m_GradeBorderImage.Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_GradeBorderImage.SetClipRect(189, 3, 221, 35);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		m_GradeImage.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("vn_mi_pvp_02.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		m_GradeImage.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("us_mi_pvp_02.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		m_GradeImage.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("tw_mi_pvp_02.tga"));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_GradeImage.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("rs_mi_pvp_02.tga"));
	}
	else
	{
		m_GradeImage.Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_pvp_02.tga"));
	}
	
	return TRUE;
}

void _XWindow_MatchRecord::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO

	// 각각의 타이틀 바닥
	m_LeftTitleBarImage.Draw(m_WindowPosition.x+11, m_WindowPosition.y+60);
	m_RightTitleBarImage.Draw(m_WindowPosition.x+155, m_WindowPosition.y+60);
	m_LeftTitleBarImage.Draw(m_WindowPosition.x+11, m_WindowPosition.y+174);
	m_RightTitleBarImage.Draw(m_WindowPosition.x+155, m_WindowPosition.y+174);
	m_LeftTitleBarImage.Draw(m_WindowPosition.x+11, m_WindowPosition.y+249);
	m_RightTitleBarImage.Draw(m_WindowPosition.x+155, m_WindowPosition.y+249);

	// Title Name
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFD7D7D7);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CINFO_PVP) );

	g_XBaseFont->SetColor(0xFFdecb80);
	g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_MATCH_2498));
	g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+179, _XGETINTERFACETEXT(ID_STRING_MATCH_2499));
	g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+254, _XGETINTERFACETEXT(ID_STRING_MATCH_2500));

	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->SetColor(0xFFd0d0d0);

	int extrapos = 0;

	// 개인
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+126,  _XGETINTERFACETEXT(ID_STRING_MATCH_2493));
		g_XBaseFont->Puts(m_WindowPosition.x+140, m_WindowPosition.y+126, _XGETINTERFACETEXT(ID_STRING_MATCH_2501));
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+142,  _XGETINTERFACETEXT(ID_STRING_MATCH_2502));
		g_XBaseFont->Puts(m_WindowPosition.x+140, m_WindowPosition.y+142, _XGETINTERFACETEXT(ID_STRING_MATCH_2503));
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+157,  _XGETINTERFACETEXT(ID_STRING_MATCH_2504));
		g_XBaseFont->Puts(m_WindowPosition.x+140, m_WindowPosition.y+156, _XGETINTERFACETEXT(ID_STRING_MATCH_2505));
		
		// 집단
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+201,  _XGETINTERFACETEXT(ID_STRING_MATCH_2493));
		g_XBaseFont->Puts(m_WindowPosition.x+140, m_WindowPosition.y+201, _XGETINTERFACETEXT(ID_STRING_MATCH_2501));
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+217,  _XGETINTERFACETEXT(ID_STRING_MATCH_2502));
		g_XBaseFont->Puts(m_WindowPosition.x+140, m_WindowPosition.y+217, _XGETINTERFACETEXT(ID_STRING_MATCH_2503));
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+232,  _XGETINTERFACETEXT(ID_STRING_MATCH_2504));
		
		// 비무대회
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+276,  _XGETINTERFACETEXT(ID_STRING_MATCH_2493));
		g_XBaseFont->Puts(m_WindowPosition.x+140, m_WindowPosition.y+276, _XGETINTERFACETEXT(ID_STRING_MATCH_2501));
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+292,  _XGETINTERFACETEXT(ID_STRING_MATCH_2502));
		g_XBaseFont->Puts(m_WindowPosition.x+140, m_WindowPosition.y+292, _XGETINTERFACETEXT(ID_STRING_MATCH_2503));
		g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+307,  _XGETINTERFACETEXT(ID_STRING_MATCH_2504));
		g_XBaseFont->Puts(m_WindowPosition.x+140, m_WindowPosition.y+307, _XGETINTERFACETEXT(ID_STRING_MATCH_2506));

		extrapos = 126;
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+126,  _XGETINTERFACETEXT(ID_STRING_MATCH_2493));
		g_XBaseFont->Puts(m_WindowPosition.x+166, m_WindowPosition.y+126, _XGETINTERFACETEXT(ID_STRING_MATCH_2501));
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+142,  _XGETINTERFACETEXT(ID_STRING_MATCH_2502));
		g_XBaseFont->Puts(m_WindowPosition.x+166, m_WindowPosition.y+142, _XGETINTERFACETEXT(ID_STRING_MATCH_2503));
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+157,  _XGETINTERFACETEXT(ID_STRING_MATCH_2504));
		g_XBaseFont->Puts(m_WindowPosition.x+166, m_WindowPosition.y+156, _XGETINTERFACETEXT(ID_STRING_MATCH_2505));

		// 집단
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+201,  _XGETINTERFACETEXT(ID_STRING_MATCH_2493));
		g_XBaseFont->Puts(m_WindowPosition.x+166, m_WindowPosition.y+201, _XGETINTERFACETEXT(ID_STRING_MATCH_2501));
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+217,  _XGETINTERFACETEXT(ID_STRING_MATCH_2502));
		g_XBaseFont->Puts(m_WindowPosition.x+166, m_WindowPosition.y+217, _XGETINTERFACETEXT(ID_STRING_MATCH_2503));
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+232,  _XGETINTERFACETEXT(ID_STRING_MATCH_2504));

		// 비무대회
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+276,  _XGETINTERFACETEXT(ID_STRING_MATCH_2493));
		g_XBaseFont->Puts(m_WindowPosition.x+166, m_WindowPosition.y+276, _XGETINTERFACETEXT(ID_STRING_MATCH_2501));
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+292,  _XGETINTERFACETEXT(ID_STRING_MATCH_2502));
		g_XBaseFont->Puts(m_WindowPosition.x+166, m_WindowPosition.y+292, _XGETINTERFACETEXT(ID_STRING_MATCH_2503));
		g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+307,  _XGETINTERFACETEXT(ID_STRING_MATCH_2504));
		g_XBaseFont->Puts(m_WindowPosition.x+166, m_WindowPosition.y+307, _XGETINTERFACETEXT(ID_STRING_MATCH_2506));

		extrapos = 138;
	}

	// 개인
	g_XBaseFont->SetColor(0xFFc8b684);

	// 비무전적
	g_XBaseFont->PrintAlign(m_WindowPosition.x+extrapos, m_WindowPosition.y+126, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", 
		g_pLocalUser->m_CharacterInfo.winscore+g_pLocalUser->m_CharacterInfo.losescore+g_pLocalUser->m_CharacterInfo.tiescore);
	// 승리수
	g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+126, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.winscore);
	// 무승부수
	g_XBaseFont->PrintAlign(m_WindowPosition.x+extrapos, m_WindowPosition.y+142, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.tiescore);
	// 패배수
	g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+142, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.losescore);

	// 집단 - 임시
	g_XBaseFont->PrintAlign(m_WindowPosition.x+extrapos, m_WindowPosition.y+201, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");
	g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+201, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");
	g_XBaseFont->PrintAlign(m_WindowPosition.x+extrapos, m_WindowPosition.y+217, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");
	g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+217, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");
	g_XBaseFont->PrintAlign(m_WindowPosition.x+126, m_WindowPosition.y+232, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d%%", 0);

	// 비무대회 - 임시
	g_XBaseFont->PrintAlign(m_WindowPosition.x+extrapos, m_WindowPosition.y+277, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");
	g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+277, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");
	g_XBaseFont->PrintAlign(m_WindowPosition.x+extrapos, m_WindowPosition.y+292, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");
	g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+292, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");
	g_XBaseFont->PrintAlign(m_WindowPosition.x+126, m_WindowPosition.y+307, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d%%", 0);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+307, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0/0");

	// 개인 - 연승수
	g_XBaseFont->SetColor(0xFFf0e82b);
	if(g_pLocalUser->m_CharacterInfo.prevmatchresult == 0)
		g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+158, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.seriesscore);
	else
		g_XBaseFont->PrintAlign(m_WindowPosition.x+292, m_WindowPosition.y+158, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0");

	FLOAT winrate = (FLOAT)g_pLocalUser->m_CharacterInfo.winscore /
		(FLOAT)(g_pLocalUser->m_CharacterInfo.winscore+g_pLocalUser->m_CharacterInfo.tiescore+g_pLocalUser->m_CharacterInfo.losescore) * 100.f;
	
	if(g_pLocalUser->m_CharacterInfo.winscore == 0 && g_pLocalUser->m_CharacterInfo.tiescore == 0 && g_pLocalUser->m_CharacterInfo.losescore == 0)
		winrate = 0.f;

	if(winrate >= 70.0f)
		g_XBaseFont->SetColor(0xFFf5e92d);
	else
		g_XBaseFont->SetColor(0xFFff3434);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+126, m_WindowPosition.y+157, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%.1f%%", winrate);

	// 계급장
	m_GradeImage.Draw(m_WindowPosition.x+28, m_WindowPosition.y+87);
	m_GradeBorderImage.Draw(m_WindowPosition.x+27, m_WindowPosition.y+86);

	g_XBaseFont->SetColor(0xFFFFFFFF);
	if(g_pLocalUser->m_MatchGrade == 0)
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+153, m_WindowPosition.y+97, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_MATCH_2507));
	}
	else
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+153, m_WindowPosition.y+97, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s", g_MatchGradeName[g_pLocalUser->m_MatchGrade]);
	}

	g_XBaseFont->Flush();

}

BOOL _XWindow_MatchRecord::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_MatchRecord::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTile.MoveWindow(m_WindowPosition.x+1, m_WindowPosition.y+24);
}

void _XWindow_MatchRecord::ShowWindow(BOOL show)
{
	if(show)
	{
		_XWindow_MatchTablePVP* pMatchTablePVP_Window = (_XWindow_MatchTablePVP*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MATCHTABLEPVP);
		if(pMatchTablePVP_Window)
		{
			RECT rect;
			pMatchTablePVP_Window->SetGradeImageRect(g_pLocalUser->m_MatchGrade, rect);
			m_GradeImage.SetClipRect(rect.left, rect.top, rect.right, rect.bottom);
		}

		if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_END)
		{
			EnableTablePVPButton(TRUE);
		}
		ChangeTab(0);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_MatchRecord::ChangeTab(int tab)
{
	switch(tab)
	{
	case 0 :
		{
			m_chkbtnMatchInfo->SetCheck(TRUE);
			m_chkbtnMatchTablePVP->SetCheck(FALSE);
			m_chkbtnMatchTableGroup->SetCheck(FALSE);
			m_chkbtnMatchTableRage->SetCheck(FALSE);
		}
		break;
	case 1 :
		{
			m_chkbtnMatchInfo->SetCheck(FALSE);
			m_chkbtnMatchTablePVP->SetCheck(TRUE);
			m_chkbtnMatchTableGroup->SetCheck(FALSE);
			m_chkbtnMatchTableRage->SetCheck(FALSE);
		}
		break;
	case 2 :
		{
			m_chkbtnMatchInfo->SetCheck(FALSE);
			m_chkbtnMatchTablePVP->SetCheck(FALSE);
			m_chkbtnMatchTableGroup->SetCheck(TRUE);
			m_chkbtnMatchTableRage->SetCheck(FALSE);
		}
		break;
	case 3 :
		{
			m_chkbtnMatchInfo->SetCheck(FALSE);
			m_chkbtnMatchTablePVP->SetCheck(FALSE);
			m_chkbtnMatchTableGroup->SetCheck(FALSE);
			m_chkbtnMatchTableRage->SetCheck(TRUE);
		}
		break;
	}
}

void _XWindow_MatchRecord::EnableTablePVPButton(BOOL enable)
{
	m_chkbtnMatchTablePVP->EnableWindow(enable);
}
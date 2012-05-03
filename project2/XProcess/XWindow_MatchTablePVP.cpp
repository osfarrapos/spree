// XWindow_MatchTablePVP.cpp: implementation of the _XWindow_MatchTablePVP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_MatchTablePVP.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchTablePVP::_XWindow_MatchTablePVP()
{
	m_chkbtnMatchInfo = NULL;
	m_chkbtnMatchTablePVP = NULL;
	m_chkbtnMatchTableGroup = NULL;
	m_chkbtnMatchTableRage = NULL;
	m_btnMoney = NULL;

	m_btnRecordType = NULL;
	m_btnMatchType = NULL;
	m_btnMatchTime = NULL;
	m_btnUseHyperrun = NULL;
	m_btnUseBuff = NULL;
	m_btnMatchBalance = NULL;

	m_btnConfirm = NULL;
	m_btnCancel = NULL;
}

_XWindow_MatchTablePVP::~_XWindow_MatchTablePVP()
{

}

BOOL _XWindow_MatchTablePVP::Initialize()
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
	
	btnstruct.commandid = _XDEF_MATCHTABLEPVP_CLOSE;
	btnstruct.position.x = 287;
	btnstruct.position.y = 5;
	
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( btnstruct );	
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	m_BorderTile.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 24, m_WindowPosition.x + 305, m_WindowPosition.y + 495);
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	FLOAT ufactor = 304.0f/128.0f;
	FLOAT vfactor = 471.0f/128.0f;
	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );

	// 각각 타이들 바닥
	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 24, 2, 55, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	pLeftBorder_1->SetScale(1.0f, 14.0f);
	InsertChildObject(pLeftBorder_1);
	
	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(303, 24, 305, 55, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, 14.0f);
	InsertChildObject(pRightBorder_1);
	
	// 가장자리 선
	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 472, 2, 496, &g_MainInterfaceTextureArchive, mainframeinterface);
	pLeftCorner->SetClipRect(0, 205, 3, 230);
	InsertChildObject(pLeftCorner);
	
	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(303, 472, 305, 496, &g_MainInterfaceTextureArchive, mainframeinterface);
	pRightCorner->SetClipRect(180, 205, 183, 230);
	InsertChildObject(pRightCorner);
	
	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 494, 179, 496, &g_MainInterfaceTextureArchive, mainframeinterface);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	pBottomBorder->SetScale(1.695f, 1.0f);
	InsertChildObject(pBottomBorder);

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
	m_chkbtnMatchInfo->SetCheck(FALSE);
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
	m_chkbtnMatchTablePVP->SetCheck(TRUE);
	m_chkbtnMatchTablePVP->EnableWindow(TRUE);
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

	int invimageindex = g_MainInterfaceTextureArchive.FindResource("mi_main_back01.tga");

	// empty
	m_ItemEdgeIcon[0].Create(0, 0, 34, 34, &g_MainInterfaceTextureArchive, invimageindex);
	m_ItemEdgeIcon[0].SetClipRect(213, 64, 247, 98);
	
	// normal
	m_ItemEdgeIcon[1].Create(0, 0, 34, 34, &g_MainInterfaceTextureArchive, invimageindex);
	m_ItemEdgeIcon[1].SetClipRect(213, 30, 247, 64);

	// 소지금액 버튼
	_XBTN_STRUCTURE moneybtnstruct = 
	{
		TRUE, {36, 437}, {15, 12},
		_XDEF_MATCHTABLEPVP_MONEY,
		invimageindex,
		invimageindex,
		invimageindex,
		&g_MainInterfaceTextureArchive
	};
	m_btnMoney = new _XButton;
	m_btnMoney->Create(moneybtnstruct);
	m_btnMoney->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 185, 6, 200, 18);
	m_btnMoney->SetButtonImageClipRect(_XBUTTONIMAGE_READY,  185, 6, 200, 18);
	m_btnMoney->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK,  185, 6, 200, 18);
	m_btnMoney->EnableWindow(TRUE);
	InsertChildObject(m_btnMoney);
	
	m_MoneyIconImage.Create(0, 0, 15, 12, &g_MainInterfaceTextureArchive, invimageindex);
	m_MoneyIconImage.SetClipRect(185, 6, 200, 18);

	m_MoneyBackImage.Create(0, 0, 96, 17, &g_MainInterfaceTextureArchive, invimageindex);
	m_MoneyBackImage.SetClipRect(8, 202, 104, 219);

	_XBTN_STRUCTURE bigbtnstruct = 
	{
		TRUE, {47, 469}, {106, 22},
		_XDEF_MATCHTABLEPVP_CONFIRM,
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
	m_btnConfirm->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2508);
	m_btnConfirm->SetButtonTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	InsertChildObject(m_btnConfirm);

	bigbtnstruct.position.x = 159;
	bigbtnstruct.position.y = 469;
	bigbtnstruct.commandid = _XDEF_MATCHTABLEPVP_CANCEL;

	m_btnCancel = new _XButton;
	m_btnCancel->Create(bigbtnstruct);
	m_btnCancel->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 3, 177, 109, 199);
	m_btnCancel->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 3, 153, 109, 175);
	m_btnCancel->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 3, 202, 109, 224);
	m_btnCancel->SetGrowTextMode(TRUE);
	m_btnCancel->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL);
	m_btnCancel->SetButtonTextColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	InsertChildObject(m_btnCancel);

	int trainingresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_tra_base.tga");

	m_LeftTitleBarImage.Create(0, 0, 170, 22, &g_MainInterfaceTextureArchive, trainingresourceindex);
	m_LeftTitleBarImage.SetClipRect(2, 19, 171, 40);
	m_RightTitleBarImage.Create(0, 0, 140, 22, &g_MainInterfaceTextureArchive, trainingresourceindex);
	m_RightTitleBarImage.SetClipRect(64, 19, 204, 41);

	m_VsImage.Create(0, 0, 19, 20, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_VsImage.SetClipRect(146, 1, 165, 21);

	// 비무 종류
	_XBTN_STRUCTURE combobtnstruct = 
	{
		TRUE, {277, 185}, {12, 12},
		_XDEF_MATCHTABLEPVP_MATCHCLASSBTN,
		g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga"),
		g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga"),
		g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga"),
		&g_MainInterfaceTextureArchive
	};

	m_btnRecordType = new _XButton;
	m_btnRecordType->Create(combobtnstruct);
	m_btnRecordType->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 12, 12, 24);
	m_btnRecordType->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 12, 12, 24, 24);
	m_btnRecordType->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 24, 12, 36, 24);
	m_btnRecordType->EnableWindow(FALSE);
	InsertChildObject(m_btnRecordType);

	// 비무 형식
	combobtnstruct.position.x = 277;
	combobtnstruct.position.y = 201;
	combobtnstruct.commandid = _XDEF_MATCHTABLEPVP_MATCHTYPEBTN;

	m_btnMatchType = new _XButton;
	m_btnMatchType->Create(combobtnstruct);
	m_btnMatchType->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 12, 12, 24);
	m_btnMatchType->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 12, 12, 24, 24);
	m_btnMatchType->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 24, 12, 36, 24);
	m_btnMatchType->EnableWindow(FALSE);
	InsertChildObject(m_btnMatchType);

	// 비무 시간
	combobtnstruct.position.x = 277;
	combobtnstruct.position.y = 217;
	combobtnstruct.commandid = _XDEF_MATCHTABLEPVP_MATCHTIMEBTN;
	
	m_btnMatchTime = new _XButton;
	m_btnMatchTime->Create(combobtnstruct);
	m_btnMatchTime->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 12, 12, 24);
	m_btnMatchTime->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 12, 12, 24, 24);
	m_btnMatchTime->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 24, 12, 36, 24);
	m_btnMatchTime->EnableWindow(FALSE);
	InsertChildObject(m_btnMatchTime);

	// 경공사용유무
	combobtnstruct.position.x = 277;
	combobtnstruct.position.y = 233;
	combobtnstruct.commandid = _XDEF_MATCHTABLEPVP_USEHYPERRUNBTN;
	
	m_btnUseHyperrun= new _XButton;
	m_btnUseHyperrun->Create(combobtnstruct);
	m_btnUseHyperrun->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 12, 12, 24);
	m_btnUseHyperrun->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 12, 12, 24, 24);
	m_btnUseHyperrun->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 24, 12, 36, 24);
	m_btnUseHyperrun->EnableWindow(FALSE);
	InsertChildObject(m_btnUseHyperrun);

	// 무공효과
	combobtnstruct.position.x = 277;
	combobtnstruct.position.y = 249;
	combobtnstruct.commandid = _XDEF_MATCHTABLEPVP_USEBUFFBTN;
	
	m_btnUseBuff= new _XButton;
	m_btnUseBuff->Create(combobtnstruct);
	m_btnUseBuff->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 12, 12, 24);
	m_btnUseBuff->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 12, 12, 24, 24);
	m_btnUseBuff->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 24, 12, 36, 24);
	m_btnUseBuff->EnableWindow(FALSE);
	InsertChildObject(m_btnUseBuff);

	// 비무균형조절
	combobtnstruct.position.x = 277;
	combobtnstruct.position.y = 265;
	combobtnstruct.commandid = _XDEF_MATCHTABLEPVP_MATCHBALANCEBTN;
	
	m_btnMatchBalance= new _XButton;
	m_btnMatchBalance->Create(combobtnstruct);
	m_btnMatchBalance->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 12, 12, 24);
	m_btnMatchBalance->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 12, 12, 24, 24);
	m_btnMatchBalance->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 24, 12, 36, 24);
	m_btnMatchBalance->EnableWindow(FALSE);
	InsertChildObject(m_btnMatchBalance);

	// 균형 조절 바 옆의 이미지
	_XImageStatic* pLeftBarImage = new _XImageStatic;
	pLeftBarImage->Create(28, 285, 43, 303, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pLeftBarImage->SetClipRect(168, 1, 184, 20);
	InsertChildObject(pLeftBarImage);

	_XImageStatic* pRightBarImage = new _XImageStatic;
	pRightBarImage->Create(268, 285, 283, 303, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pRightBarImage->SetClipRect(168, 1, 184, 20);
	InsertChildObject(pRightBarImage);

	// 균형바 - 빨간색
	_XImageStatic* pDefaultIndicator = new _XImageStatic;
	pDefaultIndicator->Create(45, 291, 264, 297, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pDefaultIndicator->SetClipRect(1, 60, 221, 66);
	InsertChildObject(pDefaultIndicator);

	int optionresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_system.tga");

	// 균형바
	_XSLIDERCONTROL_STRUCTURE sliderstruct = 
	{
		TRUE, {44, 288}, {222, 13},
		_XDEF_MATCHTABLEPVP_MATCHBALANCESLIDER,
		pvpresourceindex,
		pvpresourceindex,
		
		optionresourceindex,
		optionresourceindex,
		optionresourceindex,
		{-5, -4}, {12, 12},
		
		pvpresourceindex,
		
		&g_MainInterfaceTextureArchive,
		"HSlider"
	};
	
	m_sldMatchBalance = new _XHSlider;
	m_sldMatchBalance->Create(sliderstruct);
	m_sldMatchBalance->m_TrackBarButton.SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 200, 2, 212, 14);
	m_sldMatchBalance->m_TrackBarButton.SetButtonImageClipRect(_XBUTTONIMAGE_READY, 200, 2, 212, 14);
	m_sldMatchBalance->m_TrackBarButton.SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 200, 2, 212, 14);
	m_sldMatchBalance->m_IndicatorImage.SetClipRect(0, 49, 222, 58);
	m_sldMatchBalance->SetClipRect(0, 37, 222, 49);
	m_sldMatchBalance->m_FColor = D3DCOLOR_ARGB(255, 255, 255, 255);
	m_sldMatchBalance->SetGabSize(1);
	m_sldMatchBalance->ShowWindow(TRUE);
	m_sldMatchBalance->EnableWindow(TRUE);
	m_sldMatchBalance->SetGaugeValue(50);
	m_sldMatchBalance->SetUseSpinButton(FALSE);
	m_sldMatchBalance->EnableWindow(FALSE);
	InsertChildObject(m_sldMatchBalance);

	// Combo box용 Listbox
	// 비무 종류
	_XLISTBOX_STRUCTURE listboxstruct = 
	{
		TRUE,
		{159, 183},
		{113, 30},
		_XDEF_MATCHTABLEPVP_MATCHCLASSLBOX,
		-1,
		&g_MainInterfaceTextureArchive,
		_T(""),
		14,
		2,
		_XLSTYLE_NOSCROLLBAR
	};

	m_lboxRecordType = new _XListBox;
	m_lboxRecordType->Create(listboxstruct);
	_XStatic* tempstatic = m_lboxRecordType->GetBorderStatic();
	tempstatic->SetFaceColor(0xFF000000);

	m_lboxRecordType->SetColumnOffset(0, 2, 2);
	m_lboxRecordType->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lboxRecordType->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lboxRecordType->SetColumnWidth(0, 113);
	m_lboxRecordType->SetTrackWidth(113);
	m_lboxRecordType->SetTrackHeight(14);
	m_lboxRecordType->SetScrollPos(0);

	m_lboxRecordType->InsertListItem(_T(""), 0, 1);
	m_lboxRecordType->SetItemText(0, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2509), TRUE);
	m_lboxRecordType->SetItemAttrib(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxRecordType->SetItemAttribHighlight(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_lboxRecordType->InsertListItem(_T(""), 0, 2);
	m_lboxRecordType->SetItemText(1, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2510), TRUE);
	m_lboxRecordType->SetItemAttrib(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxRecordType->SetItemAttribHighlight(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_lboxRecordType->ShowWindow(FALSE);
	InsertChildObject(m_lboxRecordType);

	// 비무 형식
	listboxstruct.position.y = 199;
	listboxstruct.windowsize.cy = 16;
	listboxstruct.commandid = _XDEF_MATCHTABLEPVP_MATCHTYPELBOX;
	listboxstruct.maxviewlinecount = 1;

	m_lboxMatchType = new _XListBox;
	m_lboxMatchType->Create(listboxstruct);
	tempstatic = m_lboxMatchType->GetBorderStatic();
	tempstatic->SetFaceColor(0xFF000000);
	
	m_lboxMatchType->SetColumnOffset(0, 2, 2);
	m_lboxMatchType->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lboxMatchType->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lboxMatchType->SetColumnWidth(0, 113);
	m_lboxMatchType->SetTrackWidth(113);
	m_lboxMatchType->SetTrackHeight(14);
	m_lboxMatchType->SetScrollPos(0);
	
	m_lboxMatchType->InsertListItem(_T(""), 0, 1);
	m_lboxMatchType->SetItemText(0, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2486), TRUE);
	m_lboxMatchType->SetItemAttrib(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxMatchType->SetItemAttribHighlight(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_lboxMatchType->ShowWindow(FALSE);
	InsertChildObject(m_lboxMatchType);

	// 비무 시간
	listboxstruct.position.y = 215;
	listboxstruct.windowsize.cy = 30;
	listboxstruct.commandid = _XDEF_MATCHTABLEPVP_MATCHTIMELBOX;
	listboxstruct.maxviewlinecount = 2;
	
	m_lboxMatchTime = new _XListBox;
	m_lboxMatchTime->Create(listboxstruct);
	tempstatic = m_lboxMatchTime->GetBorderStatic();
	tempstatic->SetFaceColor(0xFF000000);
	
	m_lboxMatchTime->SetColumnOffset(0, 2, 2);
	m_lboxMatchTime->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lboxMatchTime->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lboxMatchTime->SetColumnWidth(0, 113);
	m_lboxMatchTime->SetTrackWidth(113);
	m_lboxMatchTime->SetTrackHeight(14);
	m_lboxMatchTime->SetScrollPos(0);
	
	m_lboxMatchTime->InsertListItem(_T(""), 0, 1);
	m_lboxMatchTime->SetItemText(0, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2511), TRUE);
	m_lboxMatchTime->SetItemAttrib(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxMatchTime->SetItemAttribHighlight(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_lboxMatchTime->InsertListItem(_T(""), 0, 2);
	m_lboxMatchTime->SetItemText(1, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2512), TRUE);
	m_lboxMatchTime->SetItemAttrib(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxMatchTime->SetItemAttribHighlight(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	m_lboxMatchTime->ShowWindow(FALSE);
	InsertChildObject(m_lboxMatchTime);

	// 경공사용유무
	listboxstruct.position.y = 231;
//	listboxstruct.windowsize.cy = 44;
	listboxstruct.windowsize.cy = 30;
	listboxstruct.commandid = _XDEF_MATCHTABLEPVP_USEHYPERRUNLBOX;
//	listboxstruct.maxviewlinecount = 3;
	listboxstruct.maxviewlinecount = 2;
	
	m_lboxUseHyperrun = new _XListBox;
	m_lboxUseHyperrun->Create(listboxstruct);
	tempstatic = m_lboxUseHyperrun->GetBorderStatic();
	tempstatic->SetFaceColor(0xFF000000);
	
	m_lboxUseHyperrun->SetColumnOffset(0, 2, 2);
	m_lboxUseHyperrun->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lboxUseHyperrun->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lboxUseHyperrun->SetColumnWidth(0, 113);
	m_lboxUseHyperrun->SetTrackWidth(113);
	m_lboxUseHyperrun->SetTrackHeight(14);
	m_lboxUseHyperrun->SetScrollPos(0);

	m_lboxUseHyperrun->InsertListItem(_T(""), 0, 1);
	m_lboxUseHyperrun->SetItemText(0, 0, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE), TRUE);
	m_lboxUseHyperrun->SetItemAttrib(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxUseHyperrun->SetItemAttribHighlight(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_lboxUseHyperrun->InsertListItem(_T(""), 0, 2);
	m_lboxUseHyperrun->SetItemText(1, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2513), TRUE);
	m_lboxUseHyperrun->SetItemAttrib(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxUseHyperrun->SetItemAttribHighlight(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
//	m_lboxUseHyperrun->InsertListItem(_T(""), 0, 3);
//	m_lboxUseHyperrun->SetItemText(2, 0, _T("피격시 중지"), TRUE);
//	m_lboxUseHyperrun->SetItemAttrib(2, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
//	m_lboxUseHyperrun->SetItemAttribHighlight(2, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	m_lboxUseHyperrun->ShowWindow(FALSE);
	InsertChildObject(m_lboxUseHyperrun);

	// 무공 효과
	listboxstruct.position.y = 247;
	listboxstruct.windowsize.cy = 30;
	listboxstruct.commandid = _XDEF_MATCHTABLEPVP_USEBUFFLBOX;
	listboxstruct.maxviewlinecount = 2;
	
	m_lboxUseBuff = new _XListBox;
	m_lboxUseBuff->Create(listboxstruct);
	tempstatic = m_lboxUseBuff->GetBorderStatic();
	tempstatic->SetFaceColor(0xFF000000);
	
	m_lboxUseBuff->SetColumnOffset(0, 2, 2);
	m_lboxUseBuff->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lboxUseBuff->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lboxUseBuff->SetColumnWidth(0, 113);
	m_lboxUseBuff->SetTrackWidth(113);
	m_lboxUseBuff->SetTrackHeight(14);
	m_lboxUseBuff->SetScrollPos(0);
	
	m_lboxUseBuff->InsertListItem(_T(""), 0, 1);
	m_lboxUseBuff->SetItemText(0, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2514), TRUE);
	m_lboxUseBuff->SetItemAttrib(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxUseBuff->SetItemAttribHighlight(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	m_lboxUseBuff->InsertListItem(_T(""), 0, 2);
	m_lboxUseBuff->SetItemText(1, 0, _XGETINTERFACETEXT(ID_STRING_GMCOMMAND_2074), TRUE);
	m_lboxUseBuff->SetItemAttrib(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxUseBuff->SetItemAttribHighlight(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	m_lboxUseBuff->ShowWindow(FALSE);
	InsertChildObject(m_lboxUseBuff);

	// 비무 균형 조절
	listboxstruct.position.y = 263;
	listboxstruct.windowsize.cy = 44;
	listboxstruct.commandid = _XDEF_MATCHTABLEPVP_MATCHBALANCELBOX;
	listboxstruct.maxviewlinecount = 3;
	
	m_lboxMatchBalance = new _XListBox;
	m_lboxMatchBalance->Create(listboxstruct);
	tempstatic = m_lboxMatchBalance->GetBorderStatic();
	tempstatic->SetFaceColor(0xFF000000);
	
	m_lboxMatchBalance->SetColumnOffset(0, 2, 2);
	m_lboxMatchBalance->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lboxMatchBalance->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lboxMatchBalance->SetColumnWidth(0, 113);
	m_lboxMatchBalance->SetTrackWidth(113);
	m_lboxMatchBalance->SetTrackHeight(14);
	m_lboxMatchBalance->SetScrollPos(0);

	m_lboxMatchBalance->InsertListItem(_T(""), 0, 1);
	m_lboxMatchBalance->SetItemText(0, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2515), TRUE);
	m_lboxMatchBalance->SetItemAttrib(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxMatchBalance->SetItemAttribHighlight(0, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	m_lboxMatchBalance->InsertListItem(_T(""), 0, 2);
	m_lboxMatchBalance->SetItemText(1, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2516), TRUE);
	m_lboxMatchBalance->SetItemAttrib(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxMatchBalance->SetItemAttribHighlight(1, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	m_lboxMatchBalance->InsertListItem(_T(""), 0, 3);
	m_lboxMatchBalance->SetItemText(2, 0, _XGETINTERFACETEXT(ID_STRING_MATCH_2517), TRUE);
	m_lboxMatchBalance->SetItemAttrib(2, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_lboxMatchBalance->SetItemAttribHighlight(2, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	m_lboxMatchBalance->ShowWindow(FALSE);
	InsertChildObject(m_lboxMatchBalance);

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

	ResetData();
	
	return TRUE;
}

void _XWindow_MatchTablePVP::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_MatchTablePVP::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));

		// 바닥들
		_XDrawRectAngle(m_WindowPosition.x+8, m_WindowPosition.y+58, m_WindowPosition.x+297, m_WindowPosition.y+463, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawSolidBar(m_WindowPosition.x+9, m_WindowPosition.y+59, m_WindowPosition.x+297, m_WindowPosition.y+463, D3DCOLOR_ARGB(64, 0, 0, 0));
		
		_XDrawRectAngle(m_WindowPosition.x+13, m_WindowPosition.y+122, m_WindowPosition.x+292, m_WindowPosition.y+155, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawSolidBar(m_WindowPosition.x+14, m_WindowPosition.y+123, m_WindowPosition.x+292, m_WindowPosition.y+155, D3DCOLOR_ARGB(64, 0, 0, 0));
		
		_XDrawRectAngle(m_WindowPosition.x+13, m_WindowPosition.y+181, m_WindowPosition.x+292, m_WindowPosition.y+322, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawSolidBar(m_WindowPosition.x+14, m_WindowPosition.y+182, m_WindowPosition.x+292, m_WindowPosition.y+322, D3DCOLOR_ARGB(64, 0, 0, 0));
		
		_XDrawRectAngle(m_WindowPosition.x+13, m_WindowPosition.y+346, m_WindowPosition.x+292, m_WindowPosition.y+458, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
		_XDrawSolidBar(m_WindowPosition.x+14, m_WindowPosition.y+347, m_WindowPosition.x+292, m_WindowPosition.y+458, D3DCOLOR_ARGB(64, 0, 0, 0));

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
	m_LeftTitleBarImage.Draw(m_WindowPosition.x+11, m_WindowPosition.y+158);
	m_RightTitleBarImage.Draw(m_WindowPosition.x+155, m_WindowPosition.y+158);
	m_LeftTitleBarImage.Draw(m_WindowPosition.x+11, m_WindowPosition.y+324);
	m_RightTitleBarImage.Draw(m_WindowPosition.x+155, m_WindowPosition.y+324);
	
	// Vs
	m_VsImage.Draw(m_WindowPosition.x+142, m_WindowPosition.y+61);

	// 계급장
	RECT rect;
	SetGradeImageRect(g_pLocalUser->m_MatchGrade, rect);
	m_GradeImage.DrawWithRegion(m_WindowPosition.x+23, m_WindowPosition.y+87, rect);
	SetGradeImageRect(g_pLocalUser->m_myMatchMemberInfo.ucMatchGrade, rect);
	m_GradeImage.DrawWithRegion(m_WindowPosition.x+254, m_WindowPosition.y+87, rect);

	// 계급장 바닥
	m_GradeBorderImage.Draw(m_WindowPosition.x+22, m_WindowPosition.y+86);
	m_GradeBorderImage.Draw(m_WindowPosition.x+253, m_WindowPosition.y+86);

	// Title Name
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFD7D7D7);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 5, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CINFO_PVP) );	

	g_XBaseFont->SetColor(0xFFdecb80);
	g_XBaseFont->Puts(m_WindowPosition.x+19, m_WindowPosition.y+163, _XGETINTERFACETEXT(ID_STRING_MATCH_2518), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+18, m_WindowPosition.y+329, _XGETINTERFACETEXT(ID_STRING_MATCH_2519), 1.0f);

	g_XBaseFont->Puts(m_WindowPosition.x+52, m_WindowPosition.y+66, g_pLocalUser->m_CharacterInfo.charactername, 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+195, m_WindowPosition.y+66, g_pLocalUser->m_myMatchMemberInfo.cName, 1.0f);

	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->SetColor(0xFFFFFFFF);
	g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+351, _XGETINTERFACETEXT(ID_STRING_MATCH_2520));
//	g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+351, g_pLocalUser->m_CharacterInfo.charactername, 1.0f);
//	g_XBaseFont->Puts(m_WindowPosition.x+174, m_WindowPosition.y+351, g_pLocalUser->m_myMatchMemberInfo.cName, 1.0f);

	g_XBaseFont->Flush();

	g_XBaseFont->SetColor(0xFFFFFFFF);
	
	TCHAR levelstring[256];
	sprintf(levelstring, "%s %d%s", 
		g_LevelName[g_pLocalUser->m_CharacterInfo.Get_level()], g_pLocalUser->m_CharacterInfo.Get_sublevel(), _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));	
	
	g_XBaseFont->SetClippingArea( m_WindowPosition.x+58, m_WindowPosition.y+89, m_WindowPosition.x+161, m_WindowPosition.y+89+g_XBaseFont->GetHeight() );
	g_XBaseFont->Puts(m_WindowPosition.x+58, m_WindowPosition.y+89, levelstring);
	g_XBaseFont->Flush();
	g_XBaseFont->DisableClippingArea();
		
	TCHAR levelstring2[256];
	sprintf(levelstring2, "%s %d%s", 
		g_LevelName[g_UserLevelTable[g_pLocalUser->m_myMatchMemberInfo.ucLevel].level], g_UserLevelTable[g_pLocalUser->m_myMatchMemberInfo.ucLevel].sublevel,
		_XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL));

	g_XBaseFont->SetClippingArea( m_WindowPosition.x+162, m_WindowPosition.y+89, m_WindowPosition.x+252, m_WindowPosition.y+89+g_XBaseFont->GetHeight() );
	g_XBaseFont->Puts(m_WindowPosition.x+162, m_WindowPosition.y+89, levelstring2);
	g_XBaseFont->Flush();
	g_XBaseFont->DisableClippingArea();

	g_XBaseFont->SetColor(0xFFF2E210);

	int temp = g_pLocalUser->m_CharacterInfo.winscore+g_pLocalUser->m_CharacterInfo.tiescore+g_pLocalUser->m_CharacterInfo.losescore;
	
	FLOAT winrate = 0.0f;
	if( temp != 0)
	{
		winrate = (FLOAT)g_pLocalUser->m_CharacterInfo.winscore /	(FLOAT)(temp) * 100.f;
	}
	//FLOAT winrate = (FLOAT)g_pLocalUser->m_CharacterInfo.winscore /
	//	(FLOAT)(g_pLocalUser->m_CharacterInfo.winscore+g_pLocalUser->m_CharacterInfo.tiescore+g_pLocalUser->m_CharacterInfo.losescore) * 100.f;
	
	TCHAR tempString[256];
	memset( tempString, 0, sizeof(tempString));
	int nResult = _snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_MATCH_2521), winrate);
	g_XBaseFont->Print(m_WindowPosition.x+57, m_WindowPosition.y+103, 1.0f, tempString);

	g_XBaseFont->SetColor(0xFFFC3410);
	winrate = (FLOAT)g_pLocalUser->m_myMatchMemberInfo.usWinRate / 10.0f;

	memset( tempString, 0, sizeof(tempString));
	nResult = _snprintf( tempString, sizeof(tempString), _XGETINTERFACETEXT(ID_STRING_MATCH_2521), winrate);
	g_XBaseFont->Print(m_WindowPosition.x+163, m_WindowPosition.y+103, 1.0f, tempString);


	g_XBaseFont->SetColor(0xFFd2d2d2);
	g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+126, _XGETINTERFACETEXT(ID_STRING_MATCH_2522), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+142, _XGETINTERFACETEXT(ID_STRING_MATCH_2523), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+185, _XGETINTERFACETEXT(ID_STRING_MATCH_2524), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+201, _XGETINTERFACETEXT(ID_STRING_MATCH_2525), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+217, _XGETINTERFACETEXT(ID_STRING_MATCH_2526), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+233, _XGETINTERFACETEXT(ID_STRING_MATCH_2527), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+249, _XGETINTERFACETEXT(ID_STRING_MATCH_2528), 1.0f);
	g_XBaseFont->Puts(m_WindowPosition.x+25, m_WindowPosition.y+265, _XGETINTERFACETEXT(ID_STRING_MATCH_2529), 1.0f);

	TCHAR messagestring[256];

	if(g_pLocalUser->m_myMatchTableInfo.usWaitTime >= 60)
	{
		int minutes = g_pLocalUser->m_myMatchTableInfo.usWaitTime/60;
		int seconds = g_pLocalUser->m_myMatchTableInfo.usWaitTime - (minutes*60);

		strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_JINSTATUS_2159, &minutes, &seconds ) );
	}
	else
	{
		sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_JINSTATUS_2158), g_pLocalUser->m_myMatchTableInfo.usWaitTime);
	}

	g_XBaseFont->Puts(m_WindowPosition.x+161, m_WindowPosition.y+126, messagestring);
	
	if(g_pLocalUser->m_myMatchTableInfo.cRecordType == 0)
	{
		// 공식 개인전
		g_XBaseFont->Print(m_WindowPosition.x+181, m_WindowPosition.y+141, 1.0f, "%d", g_MatchFightMoney[3]);
	}
	else if(g_pLocalUser->m_myMatchTableInfo.cRecordType == 1)
	{
		// 비공식 개인전
		g_XBaseFont->Print(m_WindowPosition.x+181, m_WindowPosition.y+141, 1.0f, "%d", g_MatchFightMoney[1]);
	}

	g_XBaseFont->Flush();

	g_XBaseFont->SetClippingArea( m_WindowPosition.x+161, m_WindowPosition.y+185, m_WindowPosition.x+161+113, m_WindowPosition.y+265+g_XBaseFont->GetHeight() );

	//Author : 양희왕 //breif : 툴팁 클릭 할때,, 선을 그려줘야겠다
	if(m_lboxRecordType->GetShowStatus())
	{
		_XDrawRectAngle( m_lboxRecordType->GetWindowPos().x, m_lboxRecordType->GetWindowPos().y, 
						m_lboxRecordType->GetWindowPos().x + m_lboxRecordType->GetWindowSize().cx, 
						m_lboxRecordType->GetWindowPos().y + m_lboxRecordType->GetWindowSize().cy, 
						0.0f,0xFF000000 );
		_XDrawRectAngle( m_lboxRecordType->GetWindowPos().x-1, m_lboxRecordType->GetWindowPos().y-1, 
						m_lboxRecordType->GetWindowPos().x + m_lboxRecordType->GetWindowSize().cx+1, 
						m_lboxRecordType->GetWindowPos().y + m_lboxRecordType->GetWindowSize().cy+1, 
						0.0f,0xFFD2D2D2 );
	}

	if(m_lboxMatchType->GetShowStatus())
	{
		_XDrawRectAngle( m_lboxMatchType->GetWindowPos().x, m_lboxMatchType->GetWindowPos().y, 
						m_lboxMatchType->GetWindowPos().x + m_lboxMatchType->GetWindowSize().cx, 
						m_lboxMatchType->GetWindowPos().y + m_lboxMatchType->GetWindowSize().cy, 
						0.0f,0xFF000000 );
		_XDrawRectAngle( m_lboxMatchType->GetWindowPos().x-1, m_lboxMatchType->GetWindowPos().y-1, 
						m_lboxMatchType->GetWindowPos().x + m_lboxMatchType->GetWindowSize().cx+1, 
						m_lboxMatchType->GetWindowPos().y + m_lboxMatchType->GetWindowSize().cy+1, 
						0.0f,0xFFD2D2D2 );
	}

	if(m_lboxMatchTime->GetShowStatus())
	{
		_XDrawRectAngle( m_lboxMatchTime->GetWindowPos().x, m_lboxMatchTime->GetWindowPos().y, 
						m_lboxMatchTime->GetWindowPos().x + m_lboxMatchTime->GetWindowSize().cx, 
						m_lboxMatchTime->GetWindowPos().y + m_lboxMatchTime->GetWindowSize().cy, 
						0.0f,0xFF000000 );
		_XDrawRectAngle( m_lboxMatchTime->GetWindowPos().x-1, m_lboxMatchTime->GetWindowPos().y-1, 
						m_lboxMatchTime->GetWindowPos().x + m_lboxMatchTime->GetWindowSize().cx+1, 
						m_lboxMatchTime->GetWindowPos().y + m_lboxMatchTime->GetWindowSize().cy+1, 
						0.0f,0xFFD2D2D2 );
	}

	if(m_lboxUseHyperrun->GetShowStatus())
	{
		_XDrawRectAngle( m_lboxUseHyperrun->GetWindowPos().x, m_lboxUseHyperrun->GetWindowPos().y, 
						m_lboxUseHyperrun->GetWindowPos().x + m_lboxUseHyperrun->GetWindowSize().cx, 
						m_lboxUseHyperrun->GetWindowPos().y + m_lboxUseHyperrun->GetWindowSize().cy, 
						0.0f,0xFF000000 );
		_XDrawRectAngle( m_lboxUseHyperrun->GetWindowPos().x-1, m_lboxUseHyperrun->GetWindowPos().y-1, 
						m_lboxUseHyperrun->GetWindowPos().x + m_lboxUseHyperrun->GetWindowSize().cx+1, 
						m_lboxUseHyperrun->GetWindowPos().y + m_lboxUseHyperrun->GetWindowSize().cy+1, 
						0.0f,0xFFD2D2D2 );
	}

	if(m_lboxUseBuff->GetShowStatus())
	{
		_XDrawRectAngle( m_lboxUseBuff->GetWindowPos().x, m_lboxUseBuff->GetWindowPos().y, 
						m_lboxUseBuff->GetWindowPos().x + m_lboxUseBuff->GetWindowSize().cx, 
						m_lboxUseBuff->GetWindowPos().y + m_lboxUseBuff->GetWindowSize().cy, 
						0.0f,0xFF000000 );
		_XDrawRectAngle( m_lboxUseBuff->GetWindowPos().x-1, m_lboxUseBuff->GetWindowPos().y-1, 
						m_lboxUseBuff->GetWindowPos().x + m_lboxUseBuff->GetWindowSize().cx+1, 
						m_lboxUseBuff->GetWindowPos().y + m_lboxUseBuff->GetWindowSize().cy+1, 
						0.0f,0xFFD2D2D2 );
	}

	if(m_lboxMatchBalance->GetShowStatus())
	{
		_XDrawRectAngle( m_lboxMatchBalance->GetWindowPos().x, m_lboxMatchBalance->GetWindowPos().y, 
						m_lboxMatchBalance->GetWindowPos().x + m_lboxMatchBalance->GetWindowSize().cx, 
						m_lboxMatchBalance->GetWindowPos().y + m_lboxMatchBalance->GetWindowSize().cy, 
						0.0f,0xFF000000 );
		_XDrawRectAngle( m_lboxMatchBalance->GetWindowPos().x-1, m_lboxMatchBalance->GetWindowPos().y-1, 
						m_lboxMatchBalance->GetWindowPos().x + m_lboxMatchBalance->GetWindowSize().cx+1, 
						m_lboxMatchBalance->GetWindowPos().y + m_lboxMatchBalance->GetWindowSize().cy+1, 
						0.0f,0xFFD2D2D2 );
	}

	if(!m_lboxRecordType->GetShowStatus())
		g_XBaseFont->Puts(m_WindowPosition.x+161, m_WindowPosition.y+185, m_strRecordType, 1.0f);

	if(!m_lboxRecordType->GetShowStatus() && !m_lboxMatchType->GetShowStatus())
		g_XBaseFont->Puts(m_WindowPosition.x+161, m_WindowPosition.y+201, m_strMatchType, 1.0f);

	if(!m_lboxMatchTime->GetShowStatus())
		g_XBaseFont->Puts(m_WindowPosition.x+161, m_WindowPosition.y+217, m_strMatchTime, 1.0f);

	if(!m_lboxMatchTime->GetShowStatus() && !m_lboxUseHyperrun->GetShowStatus())
		g_XBaseFont->Puts(m_WindowPosition.x+161, m_WindowPosition.y+233, m_strUseHyperrun, 1.0f);

	if(!m_lboxUseHyperrun->GetShowStatus() && !m_lboxUseBuff->GetShowStatus())
		g_XBaseFont->Puts(m_WindowPosition.x+161, m_WindowPosition.y+249, m_strUseBuff, 1.0f);

	if(!m_lboxUseHyperrun->GetShowStatus() && !m_lboxUseBuff->GetShowStatus() && !m_lboxMatchBalance->GetShowStatus())
		g_XBaseFont->Puts(m_WindowPosition.x+161, m_WindowPosition.y+265, m_strMatchBalance, 1.0f);

	g_XBaseFont->Flush();
	g_XBaseFont->DisableClippingArea();

	m_MoneyBackImage.Draw(m_WindowPosition.x+36, m_WindowPosition.y+437);
	m_MoneyBackImage.Draw(m_WindowPosition.x+175, m_WindowPosition.y+437);
	m_MoneyIconImage.Draw(m_WindowPosition.x+40, m_WindowPosition.y+440);
	m_MoneyIconImage.Draw(m_WindowPosition.x+178, m_WindowPosition.y+440);
	m_MoneyIconImage.Draw(m_WindowPosition.x+161, m_WindowPosition.y+140);
	
	// betting item
	for(int i = 0 ; i < 6 ; i++)
	{
		if(g_pLocalUser->m_myMatchTableInfo.myBetItem[i].Get_m_sID() > 0)
		{
			m_ItemEdgeIcon[1].Draw(m_WindowPosition.x+34+(33*(i%3)), m_WindowPosition.y+366+(33*(i/3)));
			g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+36+(33*(i%3)), m_WindowPosition.y+368+(33*(i/3)));
		}
		else
		{
			m_ItemEdgeIcon[0].Draw(m_WindowPosition.x+34+(33*(i%3)), m_WindowPosition.y+366+(33*(i/3)));
		}

		if(g_pLocalUser->m_myMatchTableInfo.targetBetItem[i].Get_m_sID() > 0)
		{
			m_ItemEdgeIcon[1].Draw(m_WindowPosition.x+173+(33*(i%3)), m_WindowPosition.y+366+(33*(i/3)));
			g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+175+(33*(i%3)), m_WindowPosition.y+368+(33*(i/3)));
		}
		else
		{
			m_ItemEdgeIcon[0].Draw(m_WindowPosition.x+173+(33*(i%3)), m_WindowPosition.y+366+(33*(i/3)));
		}
	}

	// 아이템 창 옆 테두리
	_XDrawRectAngle(m_WindowPosition.x+33, m_WindowPosition.y+365, m_WindowPosition.x+134, m_WindowPosition.y+433, 0.0f, D3DCOLOR_ARGB(255, 184, 177, 150));
	_XDrawRectAngle(m_WindowPosition.x+172, m_WindowPosition.y+365, m_WindowPosition.x+273, m_WindowPosition.y+433, 0.0f, D3DCOLOR_ARGB(255, 184, 177, 150));
	_XDrawRectAngle(m_WindowPosition.x+32, m_WindowPosition.y+364, m_WindowPosition.x+135, m_WindowPosition.y+434, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
	_XDrawRectAngle(m_WindowPosition.x+171, m_WindowPosition.y+364, m_WindowPosition.x+274, m_WindowPosition.y+434, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));

	g_XBaseFont->PrintAlign(m_WindowPosition.x+123, m_WindowPosition.y+441, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_myMatchTableInfo.myBetMoney);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+262, m_WindowPosition.y+441, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_myMatchTableInfo.targetBetMoney);

	
//	if(g_pLocalUser->m_myMatchTableInfo.cMatchBalance >= 0 && g_pLocalUser->m_myMatchTableInfo.cMatchBalance <= 20)
//	{
//		sprintf(messagestring, "'%s'님이 매우 약하게 설정되었습니다.", g_pLocalUser->m_CharacterInfo.charactername);
//	}
//	if(g_pLocalUser->m_myMatchTableInfo.cMatchBalance > 20 && g_pLocalUser->m_myMatchTableInfo.cMatchBalance <= 45)
//	{
//		sprintf(messagestring, "'%s'님이 약하게 설정되었습니다.", g_pLocalUser->m_CharacterInfo.charactername);
//	}
//	if(g_pLocalUser->m_myMatchTableInfo.cMatchBalance > 45 && g_pLocalUser->m_myMatchTableInfo.cMatchBalance <= 55)
//	{
//		sprintf(messagestring, "균형 조절이 없습니다.");
//	}
//	if(g_pLocalUser->m_myMatchTableInfo.cMatchBalance > 55 && g_pLocalUser->m_myMatchTableInfo.cMatchBalance <= 80)
//	{
//		sprintf(messagestring, "'%s'님이 약하게 설정되었습니다.", g_pLocalUser->m_myMatchMemberInfo.cName);
//	}
//	if(g_pLocalUser->m_myMatchTableInfo.cMatchBalance > 80 && g_pLocalUser->m_myMatchTableInfo.cMatchBalance <= 100)
//	{
//		sprintf(messagestring, "'%s'님이 매우 약하게 설정되었습니다.", g_pLocalUser->m_myMatchMemberInfo.cName);
//	}

	sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_MATCH_2530));
	g_XBaseFont->PrintAlign(m_WindowPosition.x+155, m_WindowPosition.y+307, 1.0f, _XFONT_ALIGNTYPE_CENTER, messagestring);

	g_XBaseFont->Flush();

	if( gpInput->CheckMousePos( m_WindowPosition.x+58, m_WindowPosition.y+89, m_WindowPosition.x+161, m_WindowPosition.y+89+g_XBaseFont->GetHeight() ) )
	{
		int width = g_XBaseFont->GetWidth( levelstring );
		
		if( width > 102 )
		{
			_XDrawSolidBar( m_WindowPosition.x+55, m_WindowPosition.y+87, 
				m_WindowPosition.x+61+width, m_WindowPosition.y+89+g_XBaseFont->GetHeight(), D3DCOLOR_ARGB(255, 32, 32, 32));
			_XDrawRectAngle(m_WindowPosition.x+55, m_WindowPosition.y+87, 
				m_WindowPosition.x+61+width, m_WindowPosition.y+89+g_XBaseFont->GetHeight(), 0.0f, D3DCOLOR_ARGB(128, 255, 255, 255));
						
			g_XBaseFont->SetColor(0xFFFFFFFF);
			g_XBaseFont->Puts(m_WindowPosition.x+58, m_WindowPosition.y+89, levelstring);
			g_XBaseFont->Flush();
		}
	}
	
	if( gpInput->CheckMousePos( m_WindowPosition.x+162, m_WindowPosition.y+89, m_WindowPosition.x+252, m_WindowPosition.y+89+g_XBaseFont->GetHeight() ) )
	{
		int width = g_XBaseFont->GetWidth( levelstring2 );
		
		if( width > 102 )
		{
			_XDrawSolidBar( m_WindowPosition.x+159, m_WindowPosition.y+87, 
				m_WindowPosition.x+165+width, m_WindowPosition.y+89+g_XBaseFont->GetHeight(), D3DCOLOR_ARGB(255, 32, 32, 32));
			_XDrawRectAngle(m_WindowPosition.x+159, m_WindowPosition.y+87, 
				m_WindowPosition.x+165+width, m_WindowPosition.y+89+g_XBaseFont->GetHeight(), 0.0f, D3DCOLOR_ARGB(128, 255, 255, 255));			
			
			g_XBaseFont->SetColor(0xFFFFFFFF);
			g_XBaseFont->Puts(m_WindowPosition.x+162, m_WindowPosition.y+89, levelstring2);
			g_XBaseFont->Flush();
		}
	}
}

BOOL _XWindow_MatchTablePVP::Process(_XGUIObject*& pfocusobject)
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

void _XWindow_MatchTablePVP::ResetData()
{
	memset(m_strRecordType, 0, sizeof(m_strRecordType));
	memset(m_strMatchType, 0, sizeof(m_strMatchType));
	memset(m_strMatchTime, 0, sizeof(m_strMatchTime));
	memset(m_strUseHyperrun, 0, sizeof(m_strUseHyperrun));
	memset(m_strUseBuff, 0, sizeof(m_strUseBuff));
	memset(m_strMatchBalance, 0, sizeof(m_strMatchBalance));

	g_pLocalUser->SetDefaultValueMatchTable();

	sprintf(m_strRecordType, _XGETINTERFACETEXT(ID_STRING_MATCH_2509));
	sprintf(m_strMatchType, _XGETINTERFACETEXT(ID_STRING_MATCH_2493));
	sprintf(m_strMatchTime, _XGETINTERFACETEXT(ID_STRING_MATCH_2511));
	sprintf(m_strUseHyperrun, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE));
	sprintf(m_strUseBuff, _XGETINTERFACETEXT(ID_STRING_MATCH_2514));
	sprintf(m_strMatchBalance, _XGETINTERFACETEXT(ID_STRING_MATCH_2515));

	m_btnConfirm->EnableWindow(TRUE);

	m_btnRecordType->EnableWindow(FALSE);
	m_btnMatchTime->EnableWindow(FALSE);
	m_btnUseHyperrun->EnableWindow(FALSE);
	m_btnUseBuff->EnableWindow(FALSE);

	if(g_pLocalUser->m_myMatchTeamID == 0)
	{
		// 신청자일때 수정 가능
//		m_btnRecordType->EnableWindow(TRUE);
//		m_btnMatchType->EnableWindow(TRUE);
		m_btnMatchTime->EnableWindow(TRUE);
		m_btnUseHyperrun->EnableWindow(TRUE);
		m_btnUseBuff->EnableWindow(TRUE);
//		m_btnMatchBalance->EnableWindow(TRUE);
//		m_sldMatchBalance->EnableWindow(TRUE);
	}
}

void _XWindow_MatchTablePVP::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_WAITING && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_END)
		{
			m_btnConfirm->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2531);
			m_btnConfirm->EnableWindow(TRUE);

			if(g_pLocalUser->m_UserState == _XDEF_USERSTATE_MATCH_WAITING)
			{
				m_btnCancel->EnableWindow(TRUE);
			}
			else
			{
				m_btnCancel->EnableWindow(FALSE);
			}

			m_btnRecordType->EnableWindow(FALSE);
			m_btnMatchType->EnableWindow(FALSE);
			m_btnMatchTime->EnableWindow(FALSE);
			m_btnUseHyperrun->EnableWindow(FALSE);
			m_btnUseBuff->EnableWindow(FALSE);
			m_btnMatchBalance->EnableWindow(FALSE);
			m_sldMatchBalance->EnableWindow(FALSE);
		}
		else
		{
			m_btnConfirm->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MATCH_2508);
			m_btnCancel->EnableWindow(TRUE);
		}

		ChangeTab(1);
	}
	else
	{
		m_lboxRecordType->ShowWindow(FALSE);
		m_lboxMatchType->ShowWindow(FALSE);
		m_lboxMatchTime->ShowWindow(FALSE);
		m_lboxUseHyperrun->ShowWindow(FALSE);
		m_lboxUseBuff->ShowWindow(FALSE);
	}
	
	_XWindow::ShowWindow(show);
}

void _XWindow_MatchTablePVP::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTile.MoveWindow(m_WindowPosition.x + 1, m_WindowPosition.y + 24);
}

void _XWindow_MatchTablePVP::SetGradeImageRect(unsigned char ucGrade, RECT& rect)
{
	switch(ucGrade)
	{
	default :
	case 9 :
		{
			rect.left = 1;
			rect.top = 1;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	case 8 :
		{
			rect.left = 33;
			rect.top = 1;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	case 7 :
		{
			rect.left = 65;
			rect.top = 1;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	case 6 :
		{
			rect.left = 97;
			rect.top = 1;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	case 5 :
		{
			rect.left = 129;
			rect.top = 1;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	case 4 :
		{
			rect.left = 161;
			rect.top = 1;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	case 3 :
		{
			rect.left = 193;
			rect.top = 1;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	case 2 :
		{
			rect.left = 225;
			rect.top = 1;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	case 1 :
		{
			rect.left = 1;
			rect.top = 33;
			rect.right = rect.left + 30;
			rect.bottom = rect.top + 30;
		}
		break;
	}
}

void _XWindow_MatchTablePVP::ChangeTab(int tab)
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
// XWindow_MatchNoticeLarge.cpp: implementation of the _XWindow_MatchNoticeLarge class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_MatchNoticeLarge.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MatchNoticeLarge::_XWindow_MatchNoticeLarge()
{
	m_MyVitalGauge = NULL;
	m_TargetVitalGauge = NULL;
	
	m_SelectedAreaID = 0;
	m_SelectedMatchInfo = NULL;
}

_XWindow_MatchNoticeLarge::~_XWindow_MatchNoticeLarge()
{

}

BOOL _XWindow_MatchNoticeLarge::Initialize()
{
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

	_XImageStatic* pNameLeftBorder_1 = new _XImageStatic;
	pNameLeftBorder_1->Create(0, 0, 48, 76, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pNameLeftBorder_1->SetClipRect(83, 180, 131, 256);
	InsertChildObject(pNameLeftBorder_1);

	_XImageStatic* pNameLeftBorder_2 = new _XImageStatic;
	pNameLeftBorder_2->Create(95, 0, 95, 76, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pNameLeftBorder_2->SetClipRect(83, 180, 130, 256);
	pNameLeftBorder_2->SetScale(-1, 1);
	InsertChildObject(pNameLeftBorder_2);

	_XImageStatic* pNameRightBorder_1 = new _XImageStatic;
	pNameRightBorder_1->Create(430, 0, 478, 76, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pNameRightBorder_1->SetClipRect(83, 180, 131, 256);
	InsertChildObject(pNameRightBorder_1);

	_XImageStatic* pNameRightBorder_2 = new _XImageStatic;
	pNameRightBorder_2->Create(525, 0, 525, 76, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pNameRightBorder_2->SetClipRect(83, 180, 130, 256);
	pNameRightBorder_2->SetScale(-1, 1);
	InsertChildObject(pNameRightBorder_2);

	_XImageStatic* pCenterBorder_1 = new _XImageStatic;
	pCenterBorder_1->Create(95, 0, 155, 76, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pCenterBorder_1->SetClipRect(0, 180, 60, 256);
	InsertChildObject(pCenterBorder_1);

	_XImageStatic* pCenterBorder_3 = new _XImageStatic;
	pCenterBorder_3->Create(420, 0, 430, 76, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pCenterBorder_3->SetClipRect(72, 180, 82, 256);
	InsertChildObject(pCenterBorder_3);

	_XImageStatic* pCenterBorder_2 = new _XImageStatic;
	pCenterBorder_2->Create(155, 0, 165 , 76, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pCenterBorder_2->SetClipRect(61, 180, 71, 256);
	pCenterBorder_2->SetScale(26.5f, 1.0f);
	InsertChildObject(pCenterBorder_2);

	_XImageStatic* pVSBorder = new _XImageStatic;
	pVSBorder->Create(246, 33, 278, 65, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pVSBorder->SetClipRect(223, 32, 256, 65);
	InsertChildObject(pVSBorder);

	_XImageStatic* pVSImage = new _XImageStatic;
	pVSImage->Create(249, 37, 274, 61, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pVSImage->SetClipRect(225, 6, 251, 31);
	InsertChildObject(pVSImage);

	_XImageStatic* pDecoLeftImage = new _XImageStatic;
	pDecoLeftImage->Create(-25, -22, 70, 90, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pDecoLeftImage->SetClipRect(161, 144, 256, 256);
	InsertChildObject(pDecoLeftImage);

	_XImageStatic* pDecoRightImage = new _XImageStatic;
	pDecoRightImage->Create(551, -22, 551, 90, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pDecoRightImage->SetClipRect(161, 144, 256, 256);
	pDecoRightImage->SetScale(-1, 1);
	InsertChildObject(pDecoRightImage);
	
	m_DefaultRecordImage.Create(0, 0, 15, 15, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_DefaultRecordImage.SetClipRect(113, 104, 128, 119);

	_XBTN_STRUCTURE btnstruct = 
	{ 
		TRUE, { 412, 3 }, { 12, 12 }, 
		_XDEF_MATCHNOTICE_CLOSE,
		g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
		g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
		g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName ),
		&g_MainInterfaceTextureArchive 
	};

	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create(btnstruct);
	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	_XImageStatic* pMyHpBorder_1 = new _XImageStatic;
	pMyHpBorder_1->Create(100, 40, 241, 51, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pMyHpBorder_1->SetClipRect(82, 137, 153, 149);
	InsertChildObject(pMyHpBorder_1);

	_XImageStatic* pMyHpBorder_2 = new _XImageStatic;
	pMyHpBorder_2->Create(241, 40, 241, 51, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pMyHpBorder_2->SetClipRect(82, 137, 153, 149);
	pMyHpBorder_2->SetScale(-1, 1);
	InsertChildObject(pMyHpBorder_2);

	_XImageStatic* pTargetHpBorder_1 = new _XImageStatic;
	pTargetHpBorder_1->Create(283, 40, 424, 51, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pTargetHpBorder_1->SetClipRect(82, 137, 153, 149);
	InsertChildObject(pTargetHpBorder_1);
	
	_XImageStatic* pTargetHpBorder_2 = new _XImageStatic;
	pTargetHpBorder_2->Create(424, 40, 424, 51, &g_MainInterfaceTextureArchive, pvpresourceindex);
	pTargetHpBorder_2->SetClipRect(82, 137, 153, 149);
	pTargetHpBorder_2->SetScale(-1, 1);
	InsertChildObject(pTargetHpBorder_2);

	_XSTICKGAUGE_STRUCTURE gaugestruct = 
	{
		TRUE, {103, 43}, {136, 6},
		pvpresourceindex,
		-1,
		&g_MainInterfaceTextureArchive
	};

	m_MyVitalGauge = new _XStickGauge;
	m_MyVitalGauge->Create(gaugestruct);
	m_MyVitalGauge->SetImageClipRect(_XGAUGEIMAGE_BORDER, 84, 123, 220, 129);
	m_MyVitalGauge->SetFactorDraw(TRUE);
	m_MyVitalGauge->SetGaugeFactor(100.0f);
	m_MyVitalGauge->SetGaugeImageDrawType( _XGAUGEIMAGEDRAWTYPE_RESIZE );
	m_MyVitalGauge->SetViewDelayGauge( TRUE, 20 );
	m_MyVitalGauge->SetBlinking( TRUE, 10 );
	InsertChildObject(m_MyVitalGauge);

	gaugestruct.position.x = 286;
	gaugestruct.position.y = 43;

	m_TargetVitalGauge = new _XStickGauge;
	m_TargetVitalGauge->Create(gaugestruct);
	m_TargetVitalGauge->SetImageClipRect(_XGAUGEIMAGE_BORDER, 84, 130, 220, 136);
	m_TargetVitalGauge->SetFactorDraw(TRUE);
	m_TargetVitalGauge->SetGaugeFactor(100.0f);
	m_TargetVitalGauge->SetGaugeImageDrawType( _XGAUGEIMAGEDRAWTYPE_RESIZE );
	m_TargetVitalGauge->SetViewDelayGauge( TRUE, 20 );
	m_TargetVitalGauge->SetBlinking( TRUE, 10 );
	InsertChildObject(m_TargetVitalGauge);

	m_WinImage.Create(0, 0, 36, 34, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_WinImage.SetClipRect(135, 87, 171, 121);

	m_LoseImage.Create(0, 0, 36, 34, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_LoseImage.SetClipRect(175, 87, 211, 121);

	m_TieImage.Create(0, 0, 36, 34, &g_MainInterfaceTextureArchive, pvpresourceindex);
	m_TieImage.SetClipRect(212, 87, 248, 121);

	return TRUE;
}

void _XWindow_MatchNoticeLarge::DestroyWindow()
{
	map <int, _XMatchNoticeInfo*>::iterator iter_notice;
	for(iter_notice = m_mapMatchNoticeInfo.begin() ; iter_notice != m_mapMatchNoticeInfo.end() ; iter_notice++)
	{
		_XMatchNoticeInfo* currentnotice = iter_notice->second;
		if(currentnotice)
		{
			delete currentnotice;
			currentnotice = NULL;
		}
	}
	m_mapMatchNoticeInfo.clear();

	_XWindow::DestroyWindow();
}

void _XWindow_MatchNoticeLarge::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;
 
	// TODO
	for(int i = 0 ; i < 10 ; i++)
	{
		m_DefaultRecordImage.Draw(m_WindowPosition.x+101+(i*14), m_WindowPosition.y+53);
		m_DefaultRecordImage.Draw(m_WindowPosition.x+284+(i*14), m_WindowPosition.y+53);
	}

	g_XBaseFont->SetColor(0xFFFFFFFF);
	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_FULLEDGE, 0xFF000000);
	g_XBaseFont->PutsAlign(m_WindowPosition.x+264, m_WindowPosition.y+5,_XFONT_ALIGNTYPE_CENTER, m_SelectedAreaName);
	g_XBaseFont->DisableGrowMode();
	g_XBaseFont->Flush();

	if(m_SelectedMatchInfo)
	{
		if(m_SelectedMatchInfo->ucMatchType == en_matchtype_pvp)
		{
			// 개인전
			if(m_SelectedMatchInfo->ucMatchState == en_matchstate_matching)
			{
				// 중앙 플레이어 이름 (체력바 위) ---
				BOOL bShowTooltip = FALSE;
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
				if((scrnpos->x > m_WindowPosition.x+103) && (scrnpos->x < m_WindowPosition.x+242))
				{
					if((scrnpos->z > m_WindowPosition.y+25) && (scrnpos->z < m_WindowPosition.y+25+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->Player[0].cName);
						if(strwidth >= (242-103))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->Player[0].cName, 109, 25);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+103, m_WindowPosition.y+25, m_WindowPosition.x+242, m_WindowPosition.y+25+g_XBaseFont->GetHeight());
					g_XBaseFont->SetColor(0xFFdecb80);
					g_XBaseFont->SetBoldMode(TRUE);
					g_XBaseFont->Puts(m_WindowPosition.x+109, m_WindowPosition.y+25, m_SelectedMatchInfo->Player[0].cName);
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode(FALSE);
					g_XBaseFont->DisableClippingArea();
				}

				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+206) && (scrnpos->x < m_WindowPosition.x+425))
				{
					if((scrnpos->z > m_WindowPosition.y+25) && (scrnpos->z < m_WindowPosition.y+25+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->Player[1].cName);
						if(strwidth >= (425-206))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->Player[1].cName, 417, 25);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+206, m_WindowPosition.y+25, m_WindowPosition.x+425, m_WindowPosition.y+25+g_XBaseFont->GetHeight());
					g_XBaseFont->SetColor(0xFFdecb80);
					g_XBaseFont->SetBoldMode(TRUE);
					g_XBaseFont->PutsAlign(m_WindowPosition.x+417, m_WindowPosition.y+25, _XFONT_ALIGNTYPE_RIGHT, m_SelectedMatchInfo->Player[1].cName);
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode(FALSE);
					g_XBaseFont->DisableClippingArea();
				}
				// ---
				
				// 양쪽 사이드 플레이어 이름 ---
				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+11) && (scrnpos->x < m_WindowPosition.x+87))
				{
					if((scrnpos->z > m_WindowPosition.y+11) && (scrnpos->z < m_WindowPosition.y+11+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->Player[0].cName);
						if(strwidth >= (87-11))
						{
							bShowTooltip = TRUE;
						}
					}
				}
				
				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->Player[0].cName, 12, 11);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+11, m_WindowPosition.y+11, m_WindowPosition.x+87, m_WindowPosition.y+11+g_XBaseFont->GetHeight()); 
					g_XBaseFont->SetColor(0xFFdecb80);
					g_XBaseFont->Puts(m_WindowPosition.x+12, m_WindowPosition.y+11, m_SelectedMatchInfo->Player[0].cName);
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}

				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+441) && (scrnpos->x < m_WindowPosition.x+517))
				{
					if((scrnpos->z > m_WindowPosition.y+11) && (scrnpos->z < m_WindowPosition.y+11+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->Player[1].cName);
						if(strwidth >= (517-441))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->Player[1].cName, 444, 11);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+441, m_WindowPosition.y+11, m_WindowPosition.x+517, m_WindowPosition.y+11+g_XBaseFont->GetHeight()); 
					g_XBaseFont->SetColor(0xFFdecb80);
					g_XBaseFont->Puts(m_WindowPosition.x+444, m_WindowPosition.y+11, m_SelectedMatchInfo->Player[1].cName);
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}
				// ---

				// 문파 ---
				BOOL bShowGroupLeftTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+7) && (scrnpos->x < m_WindowPosition.x+45))
				{
					if((scrnpos->z > m_WindowPosition.y+29) && (scrnpos->z < m_WindowPosition.y+29+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(g_JoinGroupName[m_SelectedMatchInfo->Player[0].ucMoonpa]);
						if(strwidth >= (45-7))
						{
							bShowGroupLeftTooltip = TRUE;
						}
					}
				}

				if(bShowGroupLeftTooltip)
				{
					DrawTooltip(g_JoinGroupName[m_SelectedMatchInfo->Player[0].ucMoonpa], 8, 29);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+7, m_WindowPosition.y+29, m_WindowPosition.x+45, m_WindowPosition.y+29+g_XBaseFont->GetHeight());
					g_XBaseFont->SetColor(0xFFFFFFFF);
					if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					{
						g_XBaseFont->Puts(m_WindowPosition.x+8, m_WindowPosition.y+29, g_JoinGroupName[m_SelectedMatchInfo->Player[0].ucMoonpa]);
					}
					else
					{
						g_XBaseFont->PrintAlign(m_WindowPosition.x+39, m_WindowPosition.y+29, 1.0f, _XFONT_ALIGNTYPE_RIGHT, g_JoinGroupName[m_SelectedMatchInfo->Player[0].ucMoonpa]);
					}
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}

				BOOL bShowGroupRightTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+437) && (scrnpos->x < m_WindowPosition.x+475))
				{
					if((scrnpos->z > m_WindowPosition.y+29) && (scrnpos->z < m_WindowPosition.y+29+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(g_JoinGroupName[m_SelectedMatchInfo->Player[1].ucMoonpa]);
						if(strwidth >= (475-437))
						{
							bShowGroupRightTooltip = TRUE;
						}
					}
				}

				if(bShowGroupRightTooltip)
				{
					DrawTooltip(g_JoinGroupName[m_SelectedMatchInfo->Player[1].ucMoonpa], 438, 29);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+437, m_WindowPosition.y+29, m_WindowPosition.x+475, m_WindowPosition.y+29+g_XBaseFont->GetHeight());
					g_XBaseFont->SetColor(0xFFFFFFFF);
					if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
					{
						g_XBaseFont->Puts(m_WindowPosition.x+438, m_WindowPosition.y+29, g_JoinGroupName[m_SelectedMatchInfo->Player[1].ucMoonpa]);
					}
					else
					{
						g_XBaseFont->PrintAlign(m_WindowPosition.x+468, m_WindowPosition.y+29, 1.0f, _XFONT_ALIGNTYPE_RIGHT, g_JoinGroupName[m_SelectedMatchInfo->Player[1].ucMoonpa]);
					}
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}
				// ---

				// 역할 ---
				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+49) && (scrnpos->x < m_WindowPosition.x+92))
				{
					if((scrnpos->z > m_WindowPosition.y+29) && (scrnpos->z < m_WindowPosition.y+29+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(g_CharacterRollNameTable[m_SelectedMatchInfo->Player[0].ucMoonpa][m_SelectedMatchInfo->Player[0].cClass][1]);
						if(strwidth >= (92-49))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(g_CharacterRollNameTable[m_SelectedMatchInfo->Player[0].ucMoonpa][m_SelectedMatchInfo->Player[0].cClass][1], 49, 29);
				}
				else
				{
					if(!bShowGroupLeftTooltip)	//문파 tooltip이 보일때는 print하지 않음
					{
						g_XBaseFont->SetClippingArea(m_WindowPosition.x+49, m_WindowPosition.y+29, m_WindowPosition.x+92, m_WindowPosition.y+29+g_XBaseFont->GetHeight());
						g_XBaseFont->SetColor(0xFFFFFFFF);
						g_XBaseFont->Print(m_WindowPosition.x+49, m_WindowPosition.y+29, 1.0f, 
							g_CharacterRollNameTable[m_SelectedMatchInfo->Player[0].ucMoonpa][m_SelectedMatchInfo->Player[0].cClass][1]);
						g_XBaseFont->Flush();
						g_XBaseFont->DisableClippingArea();
					}
				}

				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+478) && (scrnpos->x < m_WindowPosition.x+522))
				{
					if((scrnpos->z > m_WindowPosition.y+29) && (scrnpos->z < m_WindowPosition.y+29+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(g_CharacterRollNameTable[m_SelectedMatchInfo->Player[1].ucMoonpa][m_SelectedMatchInfo->Player[1].cClass][1]);
						if(strwidth >= (522-478))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(g_CharacterRollNameTable[m_SelectedMatchInfo->Player[1].ucMoonpa][m_SelectedMatchInfo->Player[1].cClass][1], 478, 29);
				}
				else
				{	
					if(!bShowGroupRightTooltip)	//문파 tooltip이 보일때는 print하지 않음
					{
						g_XBaseFont->SetClippingArea(m_WindowPosition.x+478, m_WindowPosition.y+29, m_WindowPosition.x+522, m_WindowPosition.y+29+g_XBaseFont->GetHeight());
						g_XBaseFont->SetColor(0xFFFFFFFF);
						g_XBaseFont->Print(m_WindowPosition.x+478, m_WindowPosition.y+29, 1.0f, 
							g_CharacterRollNameTable[m_SelectedMatchInfo->Player[1].ucMoonpa][m_SelectedMatchInfo->Player[1].cClass][1]);
						g_XBaseFont->Flush();
						g_XBaseFont->DisableClippingArea();
					}
				}
				// ---

				// 레벨 ---
				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+7) && (scrnpos->x < m_WindowPosition.x+92))
				{
					if((scrnpos->z > m_WindowPosition.y+44) && (scrnpos->z < m_WindowPosition.y+44+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->strLevelNamePlayer0);
						if(strwidth >= (92-7))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->strLevelNamePlayer0, 8, 44);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+7, m_WindowPosition.y+44, m_WindowPosition.x+92, m_WindowPosition.y+44+g_XBaseFont->GetHeight());
					g_XBaseFont->SetColor(0xFFD1D2CD);
					if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN)
					{
						g_XBaseFont->Print(m_WindowPosition.x+8, m_WindowPosition.y+44, 1.0f, "%s", m_SelectedMatchInfo->strLevelNamePlayer0);
					}
					else
					{
						g_XBaseFont->PrintAlign(m_WindowPosition.x+44, m_WindowPosition.y+44, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s", 
							m_SelectedMatchInfo->strLevelNamePlayer0);
					}
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}

				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+437) && (scrnpos->x < m_WindowPosition.x+522))
				{
					if((scrnpos->z > m_WindowPosition.y+44) && (scrnpos->z < m_WindowPosition.y+44+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->strLevelNamePlayer1);
						if(strwidth >= (522-437))
						{
							bShowTooltip = TRUE;
						}
					}
				}
				
				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->strLevelNamePlayer1, 438, 44);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+437, m_WindowPosition.y+44, m_WindowPosition.x+522, m_WindowPosition.y+44+g_XBaseFont->GetHeight());
					g_XBaseFont->SetColor(0xFFD1D2CD);
					if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN)
					{
						g_XBaseFont->Print(m_WindowPosition.x+438, m_WindowPosition.y+44, 1.0f, "%s", m_SelectedMatchInfo->strLevelNamePlayer1); 
					}
					else
					{
						g_XBaseFont->PrintAlign(m_WindowPosition.x+473, m_WindowPosition.y+44, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s", 
							m_SelectedMatchInfo->strLevelNamePlayer1);
					}
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}
				// ---

				g_XBaseFont->SetColor(0xFFD1D2CD);
				g_XBaseFont->Puts(m_WindowPosition.x+20, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_MATCH_2484));
				g_XBaseFont->Puts(m_WindowPosition.x+449, m_WindowPosition.y+59, _XGETINTERFACETEXT(ID_STRING_MATCH_2484));

				FLOAT winrate = (FLOAT)m_SelectedMatchInfo->Player[0].usWinRate / 10.f;
				g_XBaseFont->Print(m_WindowPosition.x+48, m_WindowPosition.y+59, 1.0f, "%.1f%%", winrate);

				g_XBaseFont->SetColor(0xFFFF2E19);
				winrate = (FLOAT)m_SelectedMatchInfo->Player[1].usWinRate / 10.f;
				g_XBaseFont->Print(m_WindowPosition.x+478, m_WindowPosition.y+59, 1.0f, "%.1f%%", winrate);
				
				g_XBaseFont->Flush();
			}
			else if(m_SelectedMatchInfo->ucMatchState == en_matchstate_just_end || m_SelectedMatchInfo->ucMatchState == en_matchstate_end)
			{
				// 중앙 플레이어 이름 (체력바 위) ---
				BOOL bShowTooltip = FALSE;
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
				if((scrnpos->x > m_WindowPosition.x+103) && (scrnpos->x < m_WindowPosition.x+242))
				{
					if((scrnpos->z > m_WindowPosition.y+25) && (scrnpos->z < m_WindowPosition.y+25+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->Player[0].cName);
						if(strwidth >= (242-103))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->Player[0].cName, 109, 25);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+103, m_WindowPosition.y+25, m_WindowPosition.x+242, m_WindowPosition.y+25+g_XBaseFont->GetHeight());
					g_XBaseFont->SetColor(0xFFdecb80);
					g_XBaseFont->SetBoldMode(TRUE);
					g_XBaseFont->Puts(m_WindowPosition.x+109, m_WindowPosition.y+25, m_SelectedMatchInfo->Player[0].cName);
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode(FALSE);
					g_XBaseFont->DisableClippingArea();
				}

				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+206) && (scrnpos->x < m_WindowPosition.x+425))
				{
					if((scrnpos->z > m_WindowPosition.y+25) && (scrnpos->z < m_WindowPosition.y+25+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->Player[1].cName);
						if(strwidth >= (425-206))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->Player[1].cName, 417, 25);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+206, m_WindowPosition.y+25, m_WindowPosition.x+425, m_WindowPosition.y+25+g_XBaseFont->GetHeight());
					g_XBaseFont->SetColor(0xFFdecb80);
					g_XBaseFont->SetBoldMode(TRUE);
					g_XBaseFont->PutsAlign(m_WindowPosition.x+417, m_WindowPosition.y+25, _XFONT_ALIGNTYPE_RIGHT, m_SelectedMatchInfo->Player[1].cName);
					g_XBaseFont->Flush();
					g_XBaseFont->SetBoldMode(FALSE);
					g_XBaseFont->DisableClippingArea();
				}
				// ---

				// 양쪽 사이드 플레이어 이름 ---
				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+11) && (scrnpos->x < m_WindowPosition.x+87))
				{
					if((scrnpos->z > m_WindowPosition.y+11) && (scrnpos->z < m_WindowPosition.y+11+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->Player[0].cName);
						if(strwidth >= (87-11))
						{
							bShowTooltip = TRUE;
						}
					}
				}
				
				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->Player[0].cName, 12, 11);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+11, m_WindowPosition.y+11, m_WindowPosition.x+87, m_WindowPosition.y+11+g_XBaseFont->GetHeight()); 
					g_XBaseFont->SetColor(0xFFdecb80);
					g_XBaseFont->Puts(m_WindowPosition.x+12, m_WindowPosition.y+11, m_SelectedMatchInfo->Player[0].cName);
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}

				bShowTooltip = FALSE;
				if((scrnpos->x > m_WindowPosition.x+441) && (scrnpos->x < m_WindowPosition.x+517))
				{
					if((scrnpos->z > m_WindowPosition.y+11) && (scrnpos->z < m_WindowPosition.y+11+g_XBaseFont->GetHeight()))
					{
						int strwidth = g_XBaseFont->GetWidth(m_SelectedMatchInfo->Player[1].cName);
						if(strwidth >= (517-441))
						{
							bShowTooltip = TRUE;
						}
					}
				}

				if(bShowTooltip)
				{
					DrawTooltip(m_SelectedMatchInfo->Player[1].cName, 444, 11);
				}
				else
				{
					g_XBaseFont->SetClippingArea(m_WindowPosition.x+441, m_WindowPosition.y+11, m_WindowPosition.x+517, m_WindowPosition.y+11+g_XBaseFont->GetHeight()); 
					g_XBaseFont->SetColor(0xFFdecb80);
					g_XBaseFont->Puts(m_WindowPosition.x+444, m_WindowPosition.y+11, m_SelectedMatchInfo->Player[1].cName);
					g_XBaseFont->Flush();
					g_XBaseFont->DisableClippingArea();
				}
				// ---

				if(m_SelectedMatchInfo->ucMatchRecord0[0] == 1)
					m_WinImage.Draw(m_WindowPosition.x+31, m_WindowPosition.y+31);
				else if(m_SelectedMatchInfo->ucMatchRecord0[0] == 2)
					m_LoseImage.Draw(m_WindowPosition.x+31, m_WindowPosition.y+31);
				else if(m_SelectedMatchInfo->ucMatchRecord0[0] == 3)
					m_TieImage.Draw(m_WindowPosition.x+31, m_WindowPosition.y+31);

				if(m_SelectedMatchInfo->ucMatchRecord1[0] == 1)
					m_WinImage.Draw(m_WindowPosition.x+460, m_WindowPosition.y+31);
				else if(m_SelectedMatchInfo->ucMatchRecord1[0] == 2)
					m_LoseImage.Draw(m_WindowPosition.x+460, m_WindowPosition.y+31);
				else if(m_SelectedMatchInfo->ucMatchRecord1[0] == 3)
					m_TieImage.Draw(m_WindowPosition.x+460, m_WindowPosition.y+31);
			}
		}
		else
		{
			// 집단전 etc
		}
	}
}

BOOL _XWindow_MatchNoticeLarge::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO
	map <int, _XMatchNoticeInfo*>::iterator iter_notice;
	for(iter_notice = m_mapMatchNoticeInfo.begin() ; iter_notice != m_mapMatchNoticeInfo.end() ; )
	{
		_XMatchNoticeInfo* currentnotice = iter_notice->second;
		if(currentnotice)
		{
			// 마지막 메세지를 받은지 12초 후 삭제
			if(g_LocalSystemTime - currentnotice->dwLastRecieveTime > 12000)
			{
				int id = iter_notice->first;
				if(id == m_SelectedAreaID)
				{
					// 창이 떠있으면 닫기
					if(this->m_ShowWindow)
						ShowWindow(FALSE);
				}

				delete currentnotice;
				currentnotice = NULL;
				iter_notice = m_mapMatchNoticeInfo.erase(iter_notice);
			}
			else
			{
				iter_notice++;
			}
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_MatchNoticeLarge::SetSelectedAreaID(int id)
{
	map <int, _XMatchNoticeInfo*>::iterator iter_notice = m_mapMatchNoticeInfo.find(id);
	if(iter_notice != m_mapMatchNoticeInfo.end())
	{
		m_SelectedMatchInfo = iter_notice->second;
		m_SelectedAreaID = id;

		if(m_SelectedMatchInfo->ucMatchState == en_matchstate_none)
		{
			m_MyVitalGauge->SetGaugeFactor(0);
			m_TargetVitalGauge->SetGaugeFactor(0);
			sprintf(m_SelectedAreaName, _XGETINTERFACETEXT(ID_STRING_MATCH_2485));
		}
		else
		{
			TCHAR messagestring[256];
			switch(m_SelectedMatchInfo->ucMatchType)
			{
			case en_matchtype_pvp :
				sprintf(messagestring, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2486));
				break;
			case en_matchtype_free_match :
				sprintf(messagestring, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2487));
				break;
			case en_matchtype_group_deathmatch :
				sprintf(messagestring, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_TYPE4));
				break;
			case en_matchtype_group_chief:
				sprintf(messagestring, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2488));
				break;
			case en_matchtype_group_serial :
				sprintf(messagestring, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2489));
				break;
			case en_matchtype_group_each :
				sprintf(messagestring, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2490));
				break;
			case en_matchtype_race_personal :
				sprintf(messagestring, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2491));
				break;
			case en_matchtype_race_group :
				sprintf(messagestring, "%s", _XGETINTERFACETEXT(ID_STRING_MATCH_2492));
				break;
			}

			sprintf(m_SelectedAreaName, "%s %s", g_AreaManager.GetAreaName(_XDEF_AREACATEGORY_PVPAREA, m_SelectedAreaID), messagestring);
		}
	}
	else
	{
		m_SelectedMatchInfo = NULL;
		m_SelectedAreaID = 0;
		m_MyVitalGauge->SetGaugeFactor(0);
		m_TargetVitalGauge->SetGaugeFactor(0);
		sprintf(m_SelectedAreaName, _XGETINTERFACETEXT(ID_STRING_MATCH_2485));
	}
}


void _XWindow_MatchNoticeLarge::SetParameter()
{
	m_MyVitalGauge->SetGaugeFactor(m_SelectedMatchInfo->Player[0].ucVitalRate);
	m_TargetVitalGauge->SetGaugeFactor(m_SelectedMatchInfo->Player[1].ucVitalRate);
}

void _XWindow_MatchNoticeLarge::DrawTooltip(LPSTR str, int x, int y)
{
	if(!str)
		return;

	g_XBaseFont->SetColor(0xFFFFFFFF);
	
	int strwidth = g_XBaseFont->GetWidth(str);

	_XDrawSolidBar( m_WindowPosition.x+x-4, m_WindowPosition.y+y-1, m_WindowPosition.x+x+strwidth+5, m_WindowPosition.y+y+g_XBaseFont->GetHeight() , D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+x-4, m_WindowPosition.y+y-1, m_WindowPosition.x+x+strwidth+5, m_WindowPosition.y+y+g_XBaseFont->GetHeight() , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );

	g_XBaseFont->Puts(m_WindowPosition.x+x, m_WindowPosition.y+y, str);

	g_XBaseFont->Flush();
}
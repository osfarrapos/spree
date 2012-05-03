// XWindow_NPCSkill.cpp: implementation of the _XWindow_NPCSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_NPCSkill.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_NPCSkill::_XWindow_NPCSkill()
{
	m_OwnerNPCID = 0;
	m_OwnerNPCJobClass = _XNPC_JOBCLASS_FOOD;	// 0
	m_OwnerNPCClan = _XGROUP_NANGIN;			// 0
	ResetAllSkillPackageID();
	
	m_LeftButton = NULL;
	m_RightButton = NULL;
	m_SkillListBox = NULL;

	for(int i = 0 ; i < 6 ; i++)
	{
		m_listboxPosition[i].x = 0;
		m_listboxPosition[i].y = 0;
	}

	m_SelectedSkillPackageID = -1;
	m_SelectedSkillNo = 0;
	m_SelectedSkillID = 0;

	m_SelectedListBoxPos = -1;
	m_bDrawSelectedImage = false;

	m_PrevScrollPos = -1;

	for(i = 0 ; i < 8 ; i++)
		m_SkillTypeButton[i] = NULL;

	memset(m_SkillTypeStr, 0, sizeof(m_SkillTypeStr));
	m_SelectedSkillTypeIndex = 0;
	m_StartIndex = 0;
	m_SkillTypeStrTotalCount = 0;
}

_XWindow_NPCSkill::~_XWindow_NPCSkill()
{

}

void _XWindow_NPCSkill::ResetAllSkillPackageID(void)
{
	for(int i = 0 ; i < 13 ; i++)
		ResetSkillPackageID(i);
}


BOOL _XWindow_NPCSkill::Initialize( void )
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
	
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource(g_MI_CharTextureName);
	_XBTN_STRUCTURE helpbtnstruct = 
	{
		TRUE, {339, 3}, {12, 12},
			_XDEF_NPCSKILLWINDOW_HELP,
			charinfointerface,
			charinfointerface,
			charinfointerface,
			&g_MainInterfaceTextureArchive
	};
	_XButton* pHelpButton = new _XButton;
	pHelpButton->Create(helpbtnstruct);
	pHelpButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpButton );

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	
	_XImageStatic* pTypeBorderLeft = new _XImageStatic;
	pTypeBorderLeft->Create(9, 22, 177, 69, &g_MainInterfaceTextureArchive, resourceindex);
	pTypeBorderLeft->SetClipRect(0, 0, 169, 48);
	InsertChildObject(pTypeBorderLeft);
	
	_XImageStatic* pTypeBorderRight = new _XImageStatic;
	pTypeBorderRight->Create(346, 22, 515, 69,  &g_MainInterfaceTextureArchive, resourceindex);
	pTypeBorderRight->SetClipRect(0, 0, 169, 48);
	pTypeBorderRight->SetScale(-1, 1);
	InsertChildObject(pTypeBorderRight);
	
	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {12, 25}, {19, 42}, _XDEF_NPCSKILLWINDOW_LEFT,
			resourceindex,
			resourceindex,
			resourceindex,
			&g_MainInterfaceTextureArchive
	};
	m_LeftButton = new _XButton;
	m_LeftButton->Create(btnstruct);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 191, 41, 210, 83);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 211, 41, 230, 83);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 171, 41, 190, 83);
	m_LeftButton->ShowWindow(FALSE);
	InsertChildObject(m_LeftButton);

	btnstruct.position.x = 325;
	btnstruct.position.y = 25;
	btnstruct.commandid = _XDEF_NPCSKILLWINDOW_RIGHT;
	m_RightButton = new _XButton;
	m_RightButton->Create(btnstruct);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 191, 84, 210, 126);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 211, 84, 230, 126);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 171, 84, 190, 126);
	m_RightButton->ShowWindow(FALSE);
	InsertChildObject(m_RightButton);

	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		// window property
		TRUE,										// 윈도우 활성화
		{9, 73},									// 윈도우 좌표 
		{338, 155},									// 윈도우 사이즈
		_XDEF_NPCSKILLWINDOW_LISTBOX,					// 윈도우 아이디
		-1,											// 윈도우 볼더 이미지 인덱스
		&g_MainInterfaceTextureArchive,				// 텍스쳐리스트
		_T(""),										// 윈도우 타이틀 
		150,										// 리스트 아이템 라인 피치
		3,											// 리스트 박스에 보이는 최대 라인수 
		_XLSTYLE_RIGHTSCROLLBAR,					// 스크롤바의 위치 		
		
		// Scroll bar property
		{12, 155},									// 스크롤바 사이즈
		12,											// 스크롤바 상하버튼 사이즈
		22,											// 스크롤바 트랙바 사이즈
		150,										// 전체 리스트 갯수 <설정 필요없슴>			
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex		
	};
	
	m_SkillListBox = new _XListBox;
	m_SkillListBox->Create( listboxstructure );
	m_SkillListBox->SetIconReportMode( TRUE );

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_SkillListBox->SetIconReportModeInfo( 3, 1, 50, 318 );
	}
	else
	{
		m_SkillListBox->SetIconReportModeInfo( 3, 2, 50, 159 );
	}
	m_SkillListBox->SetTrackWidth(159);
	m_SkillListBox->SetTrackHeight(50);
	
	_XVScrollBar* pScrollBar = m_SkillListBox->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}			
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}

	m_SkillListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_SkillListBox->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_SkillListBox->SetColumnOffset(0, 5+7, 6+7);
	}
	else
	{
		m_SkillListBox->SetColumnOffset(0, 5, 6);
	}
	
	m_SkillListBox->SetColumnOffset(1, 12, 13);

	m_SkillListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_SkillListBox->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_SkillListBox->SetColumnWidth(0, 316); 
	}
	else
	{
		m_SkillListBox->SetColumnWidth(0, 156);
	}
	m_SkillListBox->SetColumnWidth(1, 30);
	m_SkillListBox->EnableSelectItem(FALSE);
	InsertChildObject(m_SkillListBox);

	m_ButtonBorderImage.Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex);
	m_ButtonBorderImage.SetClipRect(207, 0, 243, 36);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_SelectedImage.Create(0, 0, 153, 46, &g_MainInterfaceTextureArchive, resourceindex);
		m_SelectedImage.SetClipRect(4, 103, 157, 149);
		
		m_SelectedRightImage.Create(0, 0, 153, 46, &g_MainInterfaceTextureArchive, resourceindex);
		m_SelectedRightImage.SetClipRect(4, 103, 157, 149);
		m_SelectedRightImage.SetScale( -1.0f, 1.0f );
	}
	else
	{
		m_SelectedImage.Create(0, 0, 156, 46, &g_MainInterfaceTextureArchive, resourceindex);
		m_SelectedImage.SetClipRect(4, 103, 160, 149);
	}

	for(int i = 0 ; i < 6 ; i++)
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
		{
			m_listboxPosition[i].x = 14;
			m_listboxPosition[i].y = 78 + 50 * i;
		}
		else
		{
			m_listboxPosition[i].x = 14 + (159*(i%2));
			m_listboxPosition[i].y = 78 + (50*(i/2));
		}
	}

	m_ExtraImage.Create(0, 0, 106, 8, &g_MainInterfaceTextureArchive, resourceindex);
	m_ExtraImage.SetClipRect(114, 154, 220, 162);

	_XBTN_STRUCTURE reqbtnstruct = 
	{
		TRUE, {241, 235}, {108, 23}, _XDEF_NPCSKILLWINDOW_REQLEARNSKILLBUTTON,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};
	_XButton* pReqButton = new _XButton;
	pReqButton->Create(reqbtnstruct);
	pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 199);
	pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 175);
	pReqButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 201, 110, 224);
	pReqButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 205, 205, 205));
	pReqButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NPCSKILL_LEARN);	// "배우기"
	InsertChildObject(pReqButton);

	_XBTN_STRUCTURE skillbtnstruct = 
	{
		TRUE, {37, 31}, {30, 30}, _XDEF_NPCSKILLWINDOW_SKILL_1,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};
	m_SkillTypeButton[0] = new _XButton;
	m_SkillTypeButton[0]->Create(skillbtnstruct);
	m_SkillTypeButton[0]->ShowWindow(FALSE);
	InsertChildObject(m_SkillTypeButton[0]);
	
	for(i = 1 ; i < 8 ; i++)
	{
		skillbtnstruct.position.x = 37+ 36*i;
		skillbtnstruct.commandid++;
		m_SkillTypeButton[i] = new _XButton;
		m_SkillTypeButton[i]->Create(skillbtnstruct);
		m_SkillTypeButton[i]->ShowWindow(FALSE);
		InsertChildObject(m_SkillTypeButton[i]);
	}

	m_SelectedSkillButtonImage.Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex);
	m_SelectedSkillButtonImage.SetClipRect(170, 0, 206, 36);
	
	m_MouseOverLeftImage.Create(0, 0, 78, 46, &g_MainInterfaceTextureArchive, resourceindex);
	m_MouseOverLeftImage.SetClipRect(115, 168, 193, 214);
	
	m_MouseOverRightImage.Create(156, 0, 156, 46, &g_MainInterfaceTextureArchive, resourceindex);
	m_MouseOverRightImage.SetClipRect(115, 168, 193, 214);
	m_MouseOverRightImage.SetScale(-1, 1);

	return TRUE;
}

void _XWindow_NPCSkill::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_NPCSkill::Draw( _XGUIObject*& pfocusobject )
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
	for(int i = 0 ; i < 8 ; i++)
	{
		m_ButtonBorderImage.Draw(m_WindowPosition.x+34+(i*36), m_WindowPosition.y+28);
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		for( i = 0; i <	3; i++ )
		{
			int leftcount = m_SkillListBox->GetItemCount() - m_SkillListBox->GetScrollPos();
			if( leftcount > i )
			{	
				m_SelectedImage.m_FColor = 0x3FFFFFFF;
				m_SelectedImage.Draw(m_WindowPosition.x+m_listboxPosition[i].x, m_WindowPosition.y+m_listboxPosition[i].y);
				m_SelectedImage.m_FColor = 0xFFFFFFFF;
				
				m_SelectedRightImage.m_FColor = 0x3FFFFFFF;
				m_SelectedRightImage.Draw(m_WindowPosition.x+m_listboxPosition[i].x+306, m_WindowPosition.y+m_listboxPosition[i].y);
				m_SelectedRightImage.m_FColor = 0xFFFFFFFF;
			}
		}
	}

	DrawText();
	DrawTooltip();

	if(m_bDrawSelectedImage)
	{
		if(m_SelectedListBoxPos >= 0)
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
			{
				m_SelectedImage.Draw(m_WindowPosition.x+m_listboxPosition[m_SelectedListBoxPos].x, m_WindowPosition.y+m_listboxPosition[m_SelectedListBoxPos].y+1);
				m_SelectedRightImage.Draw(m_WindowPosition.x+m_listboxPosition[m_SelectedListBoxPos].x+306, m_WindowPosition.y+m_listboxPosition[m_SelectedListBoxPos].y+1);				
			}
			else
			{
				m_SelectedImage.Draw(m_WindowPosition.x+m_listboxPosition[m_SelectedListBoxPos].x, m_WindowPosition.y+m_listboxPosition[m_SelectedListBoxPos].y+1);
			}
		}			
	}

	m_SelectedSkillButtonImage.Draw(m_WindowPosition.x+34+((m_SelectedSkillTypeIndex)*36), m_WindowPosition.y+28);

	DrawMouseOverImage();

	_XDrawRectAngle(m_WindowPosition.x+106, m_WindowPosition.y+236, m_WindowPosition.x+192, m_WindowPosition.y+256, 0.0f, D3DCOLOR_ARGB(255, 224, 219, 211));

	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 203, 203, 203));

	if(m_OwnerNPCJobClass == _XNPC_JOBCLASS_MILITARYOFFICER)
	{
		g_XBaseFont->Puts(m_WindowPosition.x+18, m_WindowPosition.y+240, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_CURRENTMONEY), 1.0f);	// "현재 소지금"
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+18, m_WindowPosition.y+240, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_CURRENTCONTRIBUTION), 1.0f);	// "현재문파기여도"
	}

	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 0, 255, 15));
	if(m_OwnerNPCJobClass == _XNPC_JOBCLASS_MILITARYOFFICER)
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+188, m_WindowPosition.y+240, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%u", g_pLocalUser->m_Money);
	}
	else
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+188, m_WindowPosition.y+240, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_iContribution());
	}
	g_XBaseFont->Flush();
}

void _XWindow_NPCSkill::DrawMouseOverImage(void)
{
	int selecteditem = CheckSelectedItem();
	
	if(selecteditem >= 0 && selecteditem != m_SelectedListBoxPos)
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
		{
			m_SelectedImage.m_FColor = 0x7FFFFFFF;
			m_SelectedImage.Draw(m_WindowPosition.x+m_listboxPosition[selecteditem].x, m_WindowPosition.y+m_listboxPosition[selecteditem].y);
			m_SelectedImage.m_FColor = 0xFFFFFFFF;
			
			m_SelectedRightImage.m_FColor = 0x7FFFFFFF;
			m_SelectedRightImage.Draw(m_WindowPosition.x+m_listboxPosition[selecteditem].x+306, m_WindowPosition.y+m_listboxPosition[selecteditem].y);
			m_SelectedRightImage.m_FColor = 0xFFFFFFFF;
		}
		else
		{
			m_MouseOverLeftImage.Draw(m_WindowPosition.x+m_listboxPosition[selecteditem].x, m_WindowPosition.y+m_listboxPosition[selecteditem].y);
			m_MouseOverRightImage.Draw(m_WindowPosition.x+m_listboxPosition[selecteditem].x+156, m_WindowPosition.y+m_listboxPosition[selecteditem].y);
		}
	}
}

BOOL _XWindow_NPCSkill::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	// listbox의 스크롤이 되면 selected image 삭제
	if(m_PrevScrollPos < 0)
		m_PrevScrollPos = m_SkillListBox->GetScrollPos();
	else
	{
		int scrollpos = m_SkillListBox->GetScrollPos();
		if(scrollpos != m_PrevScrollPos)
		{
			m_SelectedListBoxPos = -1;
			m_bDrawSelectedImage = false;
			m_SelectedSkillPackageID = -1;
			m_SelectedSkillNo = 0;
			m_SelectedSkillID = 0;
			m_PrevScrollPos = scrollpos;
		}
	}

	// TODO
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if(CheckMousePosition())
				{
					int selecteditem = CheckSelectedItem();
					if(selecteditem >= 0)
					{
						// Show Tooltip
						DWORD cmdid = m_SkillListBox->GetSelectedItemCommandID(selecteditem);						
						m_SelectedSkillID = cmdid & 0x0000FFFF;
						
						if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_TRADESKILL, (m_OwnerNPCJobClass == _XNPC_JOBCLASS_MILITARYOFFICER)?1:0, 0, m_SelectedSkillID ) )
						{
							g_pDefaultTooltip_Window->ShowWindow( TRUE );
						}
					}
					else
					{
						// Remove tooltip
						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}

					selecteditem = CheckSelectedItem(true);
					if(selecteditem >= 0)
					{
						m_bDrawSelectedImage = true;

						DWORD cmdid = m_SkillListBox->GetSelectedItemCommandID(selecteditem);
						m_SelectedSkillPackageID = (cmdid & 0xFF000000) >> 24;
						m_SelectedSkillNo = (cmdid & 0x00FF0000) >> 16;
						m_SelectedSkillID = cmdid & 0x0000FFFF;
					}
				}
				else
				{
					// 창 바깥쪽을 클릭해도 선택한 것 clear 하지 않음
//					m_SelectedListBoxPos = -1;
//					m_bDrawSelectedImage = false;
//					m_SelectedSkillPackageID = -1;
//					m_SelectedSkillNo = 0;
//					m_SelectedSkillID = 0;

					// Remove tooltip
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}
				}
			}
		}
	}

	if( !_XWindow::Process(pfocusobject) ) 
		return FALSE;

	return TRUE;
}	

int _XWindow_NPCSkill::CheckSelectedItem(bool bListPos)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int selecteditempos;
	
	int viewlistcount = 6;
	int checkwidth = 156;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		viewlistcount = 3;
		checkwidth = 316;
	}

	for(int i = 0 ; i < viewlistcount ; i++)
	{
		if(scrnpos->x >= m_WindowPosition.x + m_listboxPosition[i].x &&
			scrnpos->x <= m_WindowPosition.x + m_listboxPosition[i].x + checkwidth &&
			scrnpos->z >= m_WindowPosition.y + m_listboxPosition[i].y &&
			scrnpos->z <= m_WindowPosition.y + m_listboxPosition[i].y + 46)
		{
			selecteditempos = i;//m_listboxSkill->GetScrollPos() * m_listboxSkill->GetColumnCount() + i;
			
			if(m_SkillListBox->IsValidItem(selecteditempos))
			{
				if(bListPos)
					m_SelectedListBoxPos = i;
				
				return selecteditempos;
			}
		}
	}
		
	return -1;
}

int _XWindow_NPCSkill::CheckSelectedIconItem()
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int selecteditempos;
	
	for(int i = 0 ; i < 10 ; i++)
	{
		if(scrnpos->x >= m_WindowPosition.x + m_listboxPosition[i].x + 8&&
			scrnpos->x <= m_WindowPosition.x + m_listboxPosition[i].x + 38 &&
			scrnpos->z >= m_WindowPosition.y + m_listboxPosition[i].y +8 &&
			scrnpos->z <= m_WindowPosition.y + m_listboxPosition[i].y + 38)
		{
			selecteditempos = i;//m_listboxSkill->GetScrollPos() * m_listboxSkill->GetColumnCount() + i;
			
			if(m_SkillListBox->IsValidItem(selecteditempos))
			{
				return selecteditempos;
			}
			else
				return -1;
		}
	}
	
	return -1;
}

void _XWindow_NPCSkill::DrawText(void)
{
	int scrollpos = m_SkillListBox->GetScrollPos();
	int index = 0, learn = 0;
	DWORD cmdid;
	short skillid;
	
	TCHAR messagestr[256];
	memset(messagestr, 0, sizeof(messagestr));

	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF222222);

	int displaycount = 6;
	int displaypos = 146;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		displaycount = 3;
		displaypos = 300;
	}

	for(int i = 0 ; i < displaycount; i++)
	{
		index = i + scrollpos*m_SkillListBox->GetColumnCount();
		cmdid = m_SkillListBox->GetItemCommandID(index);
		skillid = cmdid & (0x0000FFFF);

		if(skillid > 0)
		{
			learn = GetCanLearnSkill(skillid);

			g_XBaseFont->SetColor(0xFFB4B4B4);
			g_XBaseFont->Puts(m_WindowPosition.x+m_listboxPosition[i].x+45, m_WindowPosition.y+m_listboxPosition[i].y+9, 
				g_SkillProperty[skillid]->skillName);

			switch(learn)
			{
			case 0 :
				{
					// 전수 가능
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 228, 198, 72));
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+displaypos, m_WindowPosition.y+m_listboxPosition[i].y+27,
						1.0f, _XFONT_ALIGNTYPE_RIGHT,_XGETINTERFACETEXT(ID_STRING_NPCSKILL_LEARNABLE) );	// _T("전수가능")
				}
				break;
			case 1 :
				{
					// 레벨 안됨
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 251, 4, 0));

					int level = g_UserLevelTable[g_SkillProperty[skillid]->requirementLevel].level;
					int sublevel = g_UserLevelTable[g_SkillProperty[skillid]->requirementLevel].sublevel;
					
					sprintf(messagestr, "%s %d%s", g_LevelName[level], sublevel, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG));
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+displaypos, m_WindowPosition.y+m_listboxPosition[i].y+27,
						1.0f, _XFONT_ALIGNTYPE_RIGHT, messagestr);
				}
				break;
			case 2 :
				{
					// 기여도 안됨
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 251, 4, 0));

					if(m_OwnerNPCJobClass == _XNPC_JOBCLASS_MILITARYOFFICER)
					{
						sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_MONEY), g_SkillProperty[skillid]->requirementContribution);	// "금 %d"
						g_XBaseFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+displaypos, m_WindowPosition.y+m_listboxPosition[i].y+27,
							1.0f, _XFONT_ALIGNTYPE_RIGHT, messagestr);
					}
					else
					{
						sprintf(messagestr, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_CONTRIBUTION), g_SkillProperty[skillid]->requirementContribution);	// "기여도 %d"
						g_XBaseFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+displaypos, m_WindowPosition.y+m_listboxPosition[i].y+27,
							1.0f, _XFONT_ALIGNTYPE_RIGHT, messagestr);
					}
				}
				break;
			case 3 :
			case 4 :
				{
					// 레벨 안됨
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 251, 4, 0));
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+displaypos, m_WindowPosition.y+m_listboxPosition[i].y+27,
						1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_ERROR1));	// _T("선수무공부족")
				}
				break;
			default :
				{
					// 그외
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 251, 4, 0));
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+displaypos, m_WindowPosition.y+m_listboxPosition[i].y+27,
						1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_ERROR2));	// _T("전수불가능")
				}
				break;
			}

			// 구멍 메우기
			m_ExtraImage.Draw(m_WindowPosition.x+m_listboxPosition[i].x+44, m_WindowPosition.y+m_listboxPosition[i].y+28);
		}
	}

	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
}


void _XWindow_NPCSkill::MoveWindow(int X, int Y)
{	
	_XWindow::MoveWindow(X, Y);
	m_BorderTitle.MoveWindow(X+3, Y+18);
}

void _XWindow_NPCSkill::SetListItem(void)
{
	m_SkillListBox->DeleteAllItem();
	m_SkillListBox->SetScrollPos(0);
	
	m_SelectedListBoxPos = -1;

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	RECT backgroundrect = {4, 52, 160, 98};

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		backgroundrect.left = 11;
		backgroundrect.top = 59;
		backgroundrect.right = 43;
		backgroundrect.bottom = 91;
	}

	int iconresourceindex = -1;
	RECT iconrect;
	short skillid = 0;
	DWORD cmdid = 0;
	int	learn = 0;
	bool bColor = true;

	int learnedskillindex = 0;

	int count = 0;

	for(int i = 0 ; i < 13 ; i++)
	{
		if(m_SkillPackageID[i] < 0)
			continue;

		int totalcount = g_SkillPackage[m_SkillPackageID[i]].nSkillCount;
		for(int j = 0 ; j < totalcount ; j++)
		{
			if(g_SkillPackage[m_SkillPackageID[i]].skillItem[j].sID > 0)
			{
				skillid = g_SkillPackage[m_SkillPackageID[i]].skillItem[j].sID;

				learnedskillindex = g_pLocalUser->GetLearnedSkillIndex(skillid);
				if(learnedskillindex < 0)
				{
					if(m_SkillTypeStr[m_StartIndex+m_SelectedSkillTypeIndex] == 0)
					{
						// 전체
						learn = GetCanLearnSkill(skillid);
						if(learn == 0)
							bColor = true;
						else
							bColor = false;
						
						if(g_pQuickSlot_Window)
						{
							g_pQuickSlot_Window->GetIconRect(skillid, iconresourceindex, iconrect, bColor, true);
						}
						cmdid = (DWORD)((((BYTE)i)<<24) | (((BYTE)j)<<16) | skillid);
						m_SkillListBox->InsertListItem(_T(""), 0, cmdid);
						m_SkillListBox->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_SkillListBox->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						m_SkillListBox->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						
						count++;
					}
					else
					{
						if(m_SkillTypeStr[m_StartIndex+m_SelectedSkillTypeIndex] == g_SkillProperty[skillid]->skillSType)
						{
							learn = GetCanLearnSkill(skillid);
							if(learn == 0)
								bColor = true;
							else
								bColor = false;
							
							if(g_pQuickSlot_Window)
							{
								g_pQuickSlot_Window->GetIconRect(skillid, iconresourceindex, iconrect, bColor, true);
							}
							cmdid = (DWORD)((((BYTE)i)<<24) | (((BYTE)j)<<16) | skillid);
							m_SkillListBox->InsertListItem(_T(""), 0, cmdid);
							m_SkillListBox->SetIconArchive(count, &g_MainInterfaceTextureArchive);
							m_SkillListBox->SetItemAttrib(count, 0, resourceindex, backgroundrect);
							m_SkillListBox->SetItemAttrib(count, 1, iconresourceindex, iconrect);
							
							count++;
						}
					}
				}
			}
		}
	}
}

void _XWindow_NPCSkill::ShowWindow(BOOL show)
{
	if(show)
	{	
		// Window 위치 재조정 =============================
		MoveWindow( (gnWidth>>2)+112, (gnHeight>>1)-34 );

		// Owner NPC의 문파 설정
		int index = 0;
		_XNPCObject* pNPC = NULL;

		for( int i = 0; i < g_Npclist.m_NPCObjectCount; i++ )
		{
			pNPC = (_XNPCObject*)&g_Npclist.m_pNPCObjectArray[i];
			if(pNPC)
			{
				if(pNPC->m_UniqueID == m_OwnerNPCID)
				{
					index = i;
					break;
				}
			}
		}
		if(i < g_Npclist.m_NPCObjectCount)
		{
			if(pNPC)
			{
				m_OwnerNPCJobClass = pNPC->m_NPCJobClass;

				switch(m_OwnerNPCJobClass)
				{
				case _XNPC_JOBCLASS_GAEBANGPEOPLE :
					m_OwnerNPCClan = _XGROUP_GAEBANG;
					break;
				case _XNPC_JOBCLASS_SORIMPEOPLE :
					m_OwnerNPCClan = _XGROUP_SORIM;
					break;
				case _XNPC_JOBCLASS_BEEGOONGPEOPLE :
					m_OwnerNPCClan = _XGROUP_BEEGOONG;
					break;
				case _XNPC_JOBCLASS_NOCKRIMPEOPLE :
					m_OwnerNPCClan = _XGROUP_NOCKRIM;
					break;
				case _XNPC_JOBCLASS_MOODANGPEOPLE :
					m_OwnerNPCClan = _XGROUP_MOODANG;
					break;
				case _XNPC_JOBCLASS_MAKYOPEOPLE :
					m_OwnerNPCClan = _XGROUP_MAKYO;
					break;
				default :
					m_OwnerNPCClan = _XGROUP_NANGIN;
					break;
				}
			}
		}

		SetSkillTypeButton();
	}	
	else 
	{
		m_SelectedListBoxPos = -1;
		m_bDrawSelectedImage = false;
		m_SelectedSkillPackageID = -1;
		m_SelectedSkillNo = 0;
		m_SelectedSkillID = 0;
		m_PrevScrollPos = -1;

		m_SkillTypeStrTotalCount = 0;
		m_StartIndex = 0;
		m_SelectedSkillTypeIndex = 0;
	}
	
	_XWindow::ShowWindow(show);
}

void _XWindow_NPCSkill::SetDefaultPosition( void )
{	
	//MoveWindow( 410, (gnHeight>>1)-205 );
	CheckOutside();
}

void _XWindow_NPCSkill::ProcessDblClick(void)
{
	if(m_SkillListBox->CheckMousePosition())// 2004.06.19->oneway48 insert 
	{
		
		int selecteditem = CheckSelectedItem();
		if(selecteditem >= 0)
		{		
			DWORD cmdid = (DWORD)m_SkillListBox->GetSelectedItemCommandID(selecteditem);
						
			m_SelectedSkillPackageID = (cmdid & 0xFF000000) >> 24;
			m_SelectedSkillNo = (cmdid & 0x00FF0000) >> 16;
			m_SelectedSkillID = (cmdid & 0x0000FFFF);
			if(m_SelectedSkillID > 0)
			{
#ifdef _XTS_NEWSKILLWINDOW
				int learnedskillcount = 0;
				for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; ++i)
				{
					if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
						learnedskillcount++;
				}

				if(learnedskillcount == _XDEF_MAX_LEARNSKILL)
				{
					// 무공 더이상 배울 수 없음
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_2986), TRUE); //더 이상 무공을 습득 할 수 없습니다. 무공을 삭제한 후 다시 시도하시기 바랍니다.
					pMessageBox->StartTimer(10000, TRUE) ;
					return;
				}
#endif
				
				if( g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL || g_SkillProperty[m_SelectedSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL )
				{						
					TCHAR skillnamestring[128];
					
					memset(skillnamestring, 0, sizeof(TCHAR)*128);
					sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANYOULEARN), g_SkillProperty[m_SelectedSkillID]->skillName);	// "%s(을)를 배우겠습니까?"
					
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);
				}
				else
				{
					if( g_SkillProperty[m_SelectedSkillID]->motion_male == -1 )//남자 동작이 없을 경우 : 여자만 배울 수 있다.
					{
						if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_MAN )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1911), TRUE);	
						}
						else
						{
							TCHAR skillnamestring[128];
							
							memset(skillnamestring, 0, sizeof(TCHAR)*128);
							sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANYOULEARN), g_SkillProperty[m_SelectedSkillID]->skillName);	// "%s(을)를 배우겠습니까?"
							
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);
						}
					}
					else if( g_SkillProperty[m_SelectedSkillID]->motion_female == -1 )//여자 동작이 없을 경우 : 남자만 배울 수 있다.
					{
						if( g_pLocalUser->m_CharacterInfo.gender == _XGENDER_WOMAN )
						{
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_1912), TRUE);
						}
						else
						{
							TCHAR skillnamestring[128];
							
							memset(skillnamestring, 0, sizeof(TCHAR)*128);
							sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANYOULEARN), g_SkillProperty[m_SelectedSkillID]->skillName);	//"%s(을)를 배우겠습니까?"
							
							_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
							pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);
						}
					}
					else
					{
						TCHAR skillnamestring[128];
						
						memset(skillnamestring, 0, sizeof(TCHAR)*128);
						sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_CANYOULEARN), g_SkillProperty[m_SelectedSkillID]->skillName);	// "%s(을)를 배우겠습니까?"
						
						_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
						pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);
					}
				}
			}
//			TCHAR skillnamestring[128];
//			memset(skillnamestring, 0, sizeof(TCHAR)*128);
//			sprintf(skillnamestring, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_CANYOULEARN), g_SkillProperty[m_SelectedSkillID]->skillName);	// "%s(을)를 배우겠습니까?"
//			
//			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW);
//			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), skillnamestring, TRUE, _XDEF_NPCSKILLWINDOW_REQLEARNSKILL, 0);			
		}
	}
}

int _XWindow_NPCSkill::GetCanLearnSkill(short skillid)
{
	bool level = false, contribution = false, requireskill_1 = false, requireskill_2 = false;

	if(g_pLocalUser->m_CharacterInfo.Get_innerlevel() >= g_SkillProperty[skillid]->requirementLevel)
		level = true;
	else
		level = false;

	if(m_OwnerNPCJobClass == _XNPC_JOBCLASS_MILITARYOFFICER)
	{
		if(g_pLocalUser->m_Money >= g_SkillProperty[skillid]->requirementContribution)
			contribution = true;
		else 
			contribution = false;	
	}
	else
	{
		if(g_pLocalUser->m_CharacterInfo.Get_iContribution() >= g_SkillProperty[skillid]->requirementContribution)
			contribution = true;
		else 
			contribution = false;
	}

	if(g_SkillProperty[skillid]->requirementSkill01 == 0)
		requireskill_1 = true;
	else
	{
		int learnedskillindex = g_pLocalUser->GetLearnedSkillIndex(g_SkillProperty[skillid]->requirementSkill01);
		if(learnedskillindex >= 0)
		{
			if(g_pLocalUser->m_SkillList[learnedskillindex].m_cSkillLevel >= 7)
				requireskill_1 = true;
		}
	}

	if(g_SkillProperty[skillid]->requirementSkill02 == 0)
		requireskill_2 = true;
	else
	{
		int learnedskillindex = g_pLocalUser->GetLearnedSkillIndex(g_SkillProperty[skillid]->requirementSkill02);
		if(learnedskillindex >= 0)
		{
			if(g_pLocalUser->m_SkillList[learnedskillindex].m_cSkillLevel >= 7)
				requireskill_2 = true;
		}
	}

	if(level && contribution && requireskill_1 && requireskill_2)
		return 0;			// 배울 수 있음

	if(!level)
		return 1;			// 레벨 안됨
	if(!contribution)
		return 2;			// 기여도 안됨
	if(!requireskill_1)
		return 3;			// 선수 무공 1 없음
	if(!requireskill_2)
		return 4;			// 선수 무공 2 없음

	return 5;				// 그외 에러
}

void _XWindow_NPCSkill::SetSkillTypeButton(void)
{
	for(int i = 0 ; i < 20 ; i++)
		m_SkillTypeStr[i] = -1;

	m_SkillTypeStr[0] = 0;
	m_SkillTypeStrTotalCount = 0;

	int ii = 1;
	for(i = 1 ; i < 20 ; i++)
	{
		if(g_SkillConceptProperty[m_OwnerNPCClan].stype_name[i])
		{
			if(HaveSkillType(i))
			{
				m_SkillTypeStr[ii] = i;
				m_SkillTypeStrTotalCount++;
				ii++;
			}
		}
	}
	m_SkillTypeStrTotalCount++;

	SetSkillTypeButtonTexture();
}

BOOL _XWindow_NPCSkill::HaveSkillType(char stype)
{
	
	for(int i = 0 ; i < 13 ; i++)
	{
		if(m_SkillPackageID[i] < 0)
			continue;
		
		int totalcount = g_SkillPackage[m_SkillPackageID[i]].nSkillCount;
		for(int j = 0 ; j < totalcount ; j++)
		{
			if(g_SkillPackage[m_SkillPackageID[i]].skillItem[j].sID > 0)
			{
				short skillid = g_SkillPackage[m_SkillPackageID[i]].skillItem[j].sID;

				int learnedskillindex = g_pLocalUser->GetLearnedSkillIndex(skillid);
				if(learnedskillindex < 0)
				{	
					if(_XSkillItem::FindSkillProperty(skillid))
					{
						if(g_SkillProperty[skillid]->skillSType == stype)
						{
							return TRUE;
						}
					}
				}
			}
		}
	}

	return FALSE;
}

void _XWindow_NPCSkill::SetSkillTypeButtonTexture(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	
	for(int i = 0 ; i < 8 ; i++)
	{
		if(m_SkillTypeStr[m_StartIndex+i] == 0)
		{
			m_SkillTypeButton[i]->SetButtonImage(resourceindex, resourceindex, resourceindex, &g_MainInterfaceTextureArchive);
			m_SkillTypeButton[i]->ShowWindow(TRUE);
			
			// 문파 아이콘
			switch(m_OwnerNPCClan)
			{
			case _XGROUP_NANGIN :
				{
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 120, 226, 150, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 120, 226, 150, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 120, 226, 150, 256);
				}
				break;
			case _XGROUP_GAEBANG :
				{
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 90, 226, 120, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 90, 226, 120, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 90, 226, 120, 256);
				}
				break;
			case _XGROUP_BEEGOONG :
				{
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 226, 30, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 226, 30, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 226, 30, 256);
				}
				break;
			case _XGROUP_SORIM :
				{
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 60, 226, 90, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 60, 226, 90, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 60, 226, 90, 256);
				}
				break;
			case _XGROUP_NOCKRIM :
				{
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 30, 226, 60, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 30, 226, 60, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 30, 226, 60, 256);
				}
				break;
			case _XGROUP_MOODANG :
				{
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 30, 226, 60, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 30, 226, 60, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 30, 226, 60, 256);
				}
				break;
			case _XGROUP_MAKYO :
				{
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 30, 226, 60, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 30, 226, 60, 256);
					m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 30, 226, 60, 256);
				}
				break;
			}
		}
		else if(m_SkillTypeStr[m_StartIndex+i] > 0)
		{
			char clan = m_OwnerNPCClan;
			int subindex = clan/5 + 1;
			if(clan > 4)
			{
				clan -= 4;
			}
			
			TCHAR resourcename[64];
			sprintf(resourcename, "icon_skilllist%02d.tga", subindex);
			
			int resourceindex_1 = g_MainInterfaceTextureArchive.FindResource(resourcename);
			m_SkillTypeButton[i]->SetButtonImage(resourceindex_1, resourceindex_1, resourceindex_1, &g_MainInterfaceTextureArchive);
			m_SkillTypeButton[i]->ShowWindow(TRUE);
			
			RECT rect;
			if(m_SkillTypeStr[m_StartIndex+i] < 9)
			{
				rect.left = 1 + (m_SkillTypeStr[m_StartIndex+i]-1)*32;
				rect.top = 1 + (clan-1)*64;
				rect.right = rect.left + 30;
				rect.bottom = rect.top + 30;
			}
			else
			{
				rect.left = 1 + (m_SkillTypeStr[m_StartIndex+i]-9)*32;
				rect.top = 1 + (clan-1)*64 + 32;
				rect.right = rect.left + 30;
				rect.bottom = rect.top + 30;
			}
			
			m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, rect.left, rect.top, rect.right, rect.bottom);
			m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, rect.left, rect.top, rect.right, rect.bottom);
			m_SkillTypeButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, rect.left, rect.top, rect.right, rect.bottom);
		}
		else
		{
			m_SkillTypeButton[i]->ShowWindow(FALSE);
		}
	}
	
	if(m_StartIndex > 0)
		m_LeftButton->ShowWindow(TRUE);
	else
		m_LeftButton->ShowWindow(FALSE);
	
	if(m_StartIndex+8 < m_SkillTypeStrTotalCount)
		m_RightButton->ShowWindow(TRUE);
	else
		m_RightButton->ShowWindow(FALSE);
	
	SetListItem();
}

void _XWindow_NPCSkill::SetStartIndex(int index)
{
	if(index < 0)
		index = 0;
	
	if(index >= m_SkillTypeStrTotalCount)
		index = m_SkillTypeStrTotalCount-1;
	
	m_StartIndex = index;
}

void _XWindow_NPCSkill::DrawTooltip(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	for(int i = 0 ; i < 8 ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x+37+36*i && scrnpos->x < m_WindowPosition.x+37+36*i+30 &&
			scrnpos->z > m_WindowPosition.y+31 && scrnpos->z < m_WindowPosition.y+31+30)
		{
			if((m_StartIndex+i) == 0)
			{
				g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+37+36*i, m_WindowPosition.y+10, _XGETINTERFACETEXT(ID_STRING_NPCSKILL_ALL), _XSC_INFORMATION);	// "전체"
			}
			else
			{
				if(g_SkillConceptProperty[m_OwnerNPCClan].stype_name[m_SkillTypeStr[m_StartIndex+i]])
				{
					g_InterfaceTooltip.SetToolTipString(m_WindowPosition.x+37+36*i, m_WindowPosition.y+10, 
						g_SkillConceptProperty[m_OwnerNPCClan].stype_name[m_SkillTypeStr[m_StartIndex+i]], _XSC_INFORMATION);
				}
			}
			return;
		}
	}
}

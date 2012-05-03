// XWindow_Skill.cpp: implementation of the _XWindow_Skill class.
//
// Modified Date	: 2003.03.03 
// Author			: Sohyun, Park 
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_Skill.h"
#include "XSR_STRINGHEADER.H"
#include "XWindowObjectDefine.h"
#include "SoundEffectList.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Skill::_XWindow_Skill()
{
	m_ButtonBorderImage = NULL;
	m_LeftButton		= NULL;
	m_RightButton		= NULL;
	m_listboxSkill		= NULL;
	m_SkillExp			= NULL;
	m_SelectedImage		= NULL;
	m_SelectedRightImage= NULL;

	for(int i = 0 ; i < 8 ; i++)
	{
		m_SkillButton[i] = NULL;
	}

	m_SelectedSkillButtonImage = NULL;

	m_SelectedListBoxPos	= -1;
	m_SelectedSkillID		= 0;
	m_bDrawSelectedImage	= false;
	
	m_bHaveMouseSlot		= false;
	m_MouseSlotSkillID		= 0;
	m_bDragState			= false;

	memset(m_listboxPosition, 0, sizeof(POINT)*10);
	memset(&m_IconDragStartPos, 0, sizeof(POINT));
	
	m_SelectedSkillButtonIndex	= 0;
	m_PrevScrollPos				= -1;
	m_StartIndex				= 0;
	m_SkillButtonStrTotalCount	= 0;
	
	m_lstboxSkillDesc		= NULL;
}

_XWindow_Skill::~_XWindow_Skill()
{
	DestroyWindow();
}

BOOL _XWindow_Skill::Initialize()
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	_XImageStatic* pTypeBorderLeft = new _XImageStatic;
	pTypeBorderLeft->Create(4, 8, 169, 48, &g_MainInterfaceTextureArchive, resourceindex);
	pTypeBorderLeft->SetClipRect(0, 0, 169, 48);
	InsertChildObject(pTypeBorderLeft);

	_XImageStatic* pTypeBorderRight = new _XImageStatic;
	pTypeBorderRight->Create(342, 8, 515, 82, &g_MainInterfaceTextureArchive, resourceindex);
	pTypeBorderRight->SetClipRect(0, 0, 169, 48);
	pTypeBorderRight->SetScale(-1, 1);
	InsertChildObject(pTypeBorderRight);

	m_ButtonBorderImage = new _XImageStatic;
	m_ButtonBorderImage->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex);
	m_ButtonBorderImage->SetClipRect(207, 0, 243, 36);

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {8, 11}, {19, 42}, _XDEF_SKILLWINDOW_LEFT,
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

	btnstruct.position.x = 320;
	btnstruct.position.y = 11;
	btnstruct.commandid = _XDEF_SKILLWINDOW_RIGHT;
	m_RightButton = new _XButton;
	m_RightButton->Create(btnstruct);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 191, 84, 210, 126);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 211, 84, 230, 126);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 171, 84, 190, 126);
	m_RightButton->ShowWindow(FALSE);
	InsertChildObject(m_RightButton);

	int scrollresourceindex = g_MainInterfaceTextureArchive.FindResource("MI_chat_quick.tga");

#ifdef _XTS_NEWSKILLWINDOW
	_XLISTBOX_STRUCTURE skillstructure = 
	{
		TRUE,
		{4, 150},
		{338, 234},
		_XDEF_SKILLWINDOW_LISTBOX,
		-1,
		&g_MainInterfaceTextureArchive,
		_T(""),
		46,
		5,
		_XLSTYLE_RIGHTSCROLLBAR,
		
		{12, 234},
		12,
		22,
		150,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex
	};
#else
	_XLISTBOX_STRUCTURE skillstructure = 
	{
		TRUE,
		{4, 131},
		{338, 254},
		_XDEF_SKILLWINDOW_LISTBOX,
		-1,
		&g_MainInterfaceTextureArchive,
		_T(""),
		50,
		5,
		_XLSTYLE_RIGHTSCROLLBAR,

		{12, 254},
		12,
		22,
		150,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex
	};
#endif
	m_listboxSkill = new _XListBox;
	m_listboxSkill->Create(skillstructure);

	_XVScrollBar* pScrollBar = m_listboxSkill->GetScrollBarObject();
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

	m_listboxSkill->SetIconReportMode(TRUE);
#ifdef _XTS_NEWSKILLWINDOW	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_listboxSkill->SetIconReportModeInfo(5, 1, 46, 312);
	}
	else
	{
		m_listboxSkill->SetIconReportModeInfo(5, 2, 46, 159);
	}
#else
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_listboxSkill->SetIconReportModeInfo(5, 1, 50, 312);
	}
	else
	{
		m_listboxSkill->SetIconReportModeInfo(5, 2, 50, 159);
	}
#endif
	m_listboxSkill->SetTrackWidth(159);
#ifdef _XTS_NEWSKILLWINDOW
	m_listboxSkill->SetTrackHeight(46);
#else
	m_listboxSkill->SetTrackHeight(50);
#endif

	m_listboxSkill->SetColumnStyle(0, _XLCOLUMNSTYLE_ICONRECT);
	m_listboxSkill->SetColumnStyle(1, _XLCOLUMNSTYLE_ICONRECT);

#ifdef _XTS_NEWSKILLWINDOW
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_listboxSkill->SetColumnOffset(0, 5+7, 2+7);
	}
	else
	{
		m_listboxSkill->SetColumnOffset(0, 5, 2);		
	}
	
	m_listboxSkill->SetColumnOffset(1, 12, 9);

#else
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_listboxSkill->SetColumnOffset(0, 5+7, 6+7);
	}
	else
	{
		m_listboxSkill->SetColumnOffset(0, 5, 6);
	}

	m_listboxSkill->SetColumnOffset(1, 12, 13);
#endif

	m_listboxSkill->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_listboxSkill->SetAlignType(1, _XFONT_ALIGNTYPE_LEFT);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_listboxSkill->SetColumnWidth(0, 316);			
	}
	else
	{
		m_listboxSkill->SetColumnWidth(0, 156);
	}
	
	m_listboxSkill->SetColumnWidth(1, 30);

	m_listboxSkill->EnableSelectItem(FALSE);
	InsertChildObject(m_listboxSkill);

	_XSTICKGAUGE_STRUCTURE stickstruct = 
	{
		TRUE, {0, 0}, {104, 6}, g_MainInterfaceTextureArchive.FindResource("MI_Characterwin.tga"), -1, &g_MainInterfaceTextureArchive
	};
	m_SkillExp = new _XStickGauge;
	m_SkillExp->Create(stickstruct);
	m_SkillExp->SetImageClipRect(_XGAUGEIMAGE_BORDER, 0, 42, 104, 48);
	m_SkillExp->SetFactorDraw(FALSE);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		m_SelectedImage = new _XImageStatic;
		m_SelectedImage->Create(0, 0, 153, 46, &g_MainInterfaceTextureArchive, resourceindex);
		m_SelectedImage->SetClipRect(4, 103, 157, 149);

		m_SelectedRightImage = new _XImageStatic;
		m_SelectedRightImage->Create(0, 0, 153, 46, &g_MainInterfaceTextureArchive, resourceindex);
		m_SelectedRightImage->SetClipRect(4, 103, 157, 149);
		m_SelectedRightImage->SetScale( -1.0f, 1.0f );
	}
	else
	{
		m_SelectedImage = new _XImageStatic;
		m_SelectedImage->Create(0, 0, 156, 46, &g_MainInterfaceTextureArchive, resourceindex);
		m_SelectedImage->SetClipRect(4, 103, 160, 149);
	}	

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		for(int i = 0 ; i < 10 ; i++)
		{
			m_listboxPosition[i].x = 9;
#ifdef _XTS_NEWSKILLWINDOW
			m_listboxPosition[i].y = 150 + i * 46;
#else
			m_listboxPosition[i].y = 136 + i * 50;
#endif
		}
	}
	else
	{
		for(int i = 0 ; i < 10 ; i++)
		{
			m_listboxPosition[i].x = 9 + (159*(i%2));
#ifdef _XTS_NEWSKILLWINDOW
			m_listboxPosition[i].y = 150 + (46*(i/2));
#else
			m_listboxPosition[i].y = 136 + (50*(i/2));
#endif
		}
	}

/*	m_ClanImage = new _XImageStatic;
	m_ClanImage->Create(0, 0, 31, 31, &g_MainInterfaceTextureArchive, 0);
	m_ClanImage->SetClipRect(0, 0, 32, 32);

	m_STypeImage = new _XImageStatic;
	m_STypeImage->Create(0, 0, 31, 31, &g_MainInterfaceTextureArchive, 0);
	m_STypeImage->SetClipRect(0, 0, 32, 32);
*/
	_XBTN_STRUCTURE skillbtnstruct = 
	{
		TRUE, {33, 18}, {30, 30}, _XDEF_SKILLWINDOW_SKILL_1,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};
	m_SkillButton[0] = new _XButton;
	m_SkillButton[0]->Create(skillbtnstruct);
	m_SkillButton[0]->ShowWindow(FALSE);
	InsertChildObject(m_SkillButton[0]);

	for(int i = 1 ; i < 8 ; i++)
	{
		skillbtnstruct.position.x = 33+ 36*i;
		skillbtnstruct.commandid++;
		m_SkillButton[i] = new _XButton;
		m_SkillButton[i]->Create(skillbtnstruct);
		m_SkillButton[i]->ShowWindow(FALSE);
		InsertChildObject(m_SkillButton[i]);
	}

/*	for(i = 0 ; i < 8 ; i++)
	{
		m_SkillButtonImage[i] = new _XImageStatic;
		m_SkillButtonImage[i]->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, 0);
	}	
*/
	m_SelectedSkillButtonImage = new _XImageStatic;
	m_SelectedSkillButtonImage->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex);
	m_SelectedSkillButtonImage->SetClipRect(170, 0, 206, 36);

	m_MouseOverLeftImage.Create(0, 0, 78, 46, &g_MainInterfaceTextureArchive, resourceindex);
	m_MouseOverLeftImage.SetClipRect(115, 168, 193, 214);

	m_MouseOverRightImage.Create(156, 0, 156, 46, &g_MainInterfaceTextureArchive, resourceindex);
	m_MouseOverRightImage.SetClipRect(115, 168, 193, 214);
	m_MouseOverRightImage.SetScale(-1, 1);

#ifdef _XTS_NEWSKILLWINDOW
	_XCHECKBTN_STRUCTURE checkbtnstruct = 
	{ 
		TRUE, {9, 133}, {40, 17},
		_XDEF_SKILLWINDOW_SKILLSORT_1, 
		g_MainInterfaceTextureArchive.FindResource("mi_trade.tga"), 
		g_MainInterfaceTextureArchive.FindResource("mi_trade.tga"),
		&g_MainInterfaceTextureArchive
	};

	for(i = 0 ; i < 8 ; i++)
	{
		checkbtnstruct.position.x = 9 + (i*41);

		m_SkillSortButton[i] = new _XCheckButton;
		m_SkillSortButton[i]->Create(checkbtnstruct);
		m_SkillSortButton[i]->SetCheck(FALSE);
		m_SkillSortButton[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 214, 220, 254, 237);
		m_SkillSortButton[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 214, 238, 254, 255);
		InsertChildObject(m_SkillSortButton[i]);

		checkbtnstruct.commandid++;
	}

	m_SkillSortButton[0]->SetCheck(TRUE);

	m_SkillSortButton[0]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALL));// "전체"
	m_SkillSortButton[1]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2765));		// "기본"
	m_SkillSortButton[2]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2766));		// "초식"
	m_SkillSortButton[3]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2767));		// "일격"
	m_SkillSortButton[4]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2768));		// "특수"
	m_SkillSortButton[5]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2769));		// "강기"
	m_SkillSortButton[6]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2770));		// "공기"
	m_SkillSortButton[7]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2771));		// "잡기"
#endif

	_XLISTBOX_STRUCTURE skilldescstruct = 
	{
		TRUE,
		{14, 66},
		{328, 62},
		_XDEF_SKILLWINDOW_SKDESCLSTBOX,
		-1,
		&g_MainInterfaceTextureArchive,
		_T(""),
		15,
		4,
		_XLSTYLE_RIGHTSCROLLBAR,

		// scroll bar property
		{12, 62},
		12,
		22,
		50,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex,
		scrollresourceindex
	};
	m_lstboxSkillDesc = new _XListBox;
	m_lstboxSkillDesc->Create(skilldescstruct);

	pScrollBar = m_lstboxSkillDesc->GetScrollBarObject();
	if(pScrollBar)
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect(cliprectstructure);
	}

	POINT columnoffset = {0, 0};

	m_lstboxSkillDesc->SetColumnOffset(0, columnoffset);
	m_lstboxSkillDesc->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lstboxSkillDesc->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_lstboxSkillDesc->SetColumnWidth(0, 323);
	m_lstboxSkillDesc->EnableSelectItem(FALSE);
	m_lstboxSkillDesc->ShowWindow(TRUE);
	InsertChildObject(m_lstboxSkillDesc);

	return TRUE;
}

void _XWindow_Skill::DestroyWindow()
{
	SAFE_DELETE(m_ButtonBorderImage);
	SAFE_DELETE(m_SkillExp);
	SAFE_DELETE(m_SelectedImage);
	SAFE_DELETE(m_SelectedRightImage);
//	SAFE_DELETE(m_ClanImage);
//	SAFE_DELETE(m_STypeImage);

	for(int i = 0 ; i < 8 ; i++)
	{
//		SAFE_DELETE(m_SkillButton[i]);
//		SAFE_DELETE(m_SkillButtonImage[i]);
	}

	SAFE_DELETE(m_SelectedSkillButtonImage);

	_XWindow::DestroyWindow();
}

void _XWindow_Skill::Draw(_XGUIObject*& pfocusobject)
{
	if(m_ShowWindow)
	{
		_XDrawSolidBar(m_WindowPosition.x+6, m_WindowPosition.y+60, m_WindowPosition.x+342, m_WindowPosition.y+129, 0x7f000000);
		_XDrawRectAngle(m_WindowPosition.x+5, m_WindowPosition.y+59, m_WindowPosition.x+342, m_WindowPosition.y+129, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));

#ifdef _XTS_NEWSKILLWINDOW
		_XDrawSolidBar(m_WindowPosition.x+6, m_WindowPosition.y+151, m_WindowPosition.x+328, m_WindowPosition.y+385, 0x7f000000);
		_XDrawRectAngle(m_WindowPosition.x+5, m_WindowPosition.y+150, m_WindowPosition.x+328, m_WindowPosition.y+385, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
#else
		_XDrawSolidBar(m_WindowPosition.x+6, m_WindowPosition.y+133, m_WindowPosition.x+328, m_WindowPosition.y+385, 0x7f000000);
		_XDrawRectAngle(m_WindowPosition.x+5, m_WindowPosition.y+132, m_WindowPosition.x+328, m_WindowPosition.y+385, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
#endif
	}	

	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)	return;
	
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO 
	for(int i = 0 ; i < 8 ; i++)
	{
		m_ButtonBorderImage->Draw(m_WindowPosition.x+30+(i*36), m_WindowPosition.y+15);

/*		if(m_SkillButton[i]->GetShowStatus())
		{
			POINT position = m_SkillButton[i]->GetWindowPos();
			m_SkillButtonImage[i]->Draw(position.x+3, position.y+3);
		}
*/	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		for( i = 0; i <	5; i++ )
		{
			int leftcount = m_listboxSkill->GetItemCount() - m_listboxSkill->GetScrollPos();
			if( leftcount > i )
			{	
				m_SelectedImage->m_FColor = 0x3FFFFFFF;
				m_SelectedImage->Draw(m_WindowPosition.x+m_listboxPosition[i].x, m_WindowPosition.y+m_listboxPosition[i].y);
				m_SelectedImage->m_FColor = 0xFFFFFFFF;
				
				m_SelectedRightImage->m_FColor = 0x3FFFFFFF;
				m_SelectedRightImage->Draw(m_WindowPosition.x+m_listboxPosition[i].x+306, m_WindowPosition.y+m_listboxPosition[i].y);
				m_SelectedRightImage->m_FColor = 0xFFFFFFFF;
			}
		}
	}

	DrawText();

	if(m_bDrawSelectedImage)
	{
		if(m_SelectedListBoxPos >= 0)
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
				g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				m_SelectedImage->Draw(m_WindowPosition.x+m_listboxPosition[m_SelectedListBoxPos].x, m_WindowPosition.y+m_listboxPosition[m_SelectedListBoxPos].y+1);
				m_SelectedRightImage->Draw(m_WindowPosition.x+m_listboxPosition[m_SelectedListBoxPos].x+306, m_WindowPosition.y+m_listboxPosition[m_SelectedListBoxPos].y+1);				
			}
			else
			{
				m_SelectedImage->Draw(m_WindowPosition.x+m_listboxPosition[m_SelectedListBoxPos].x, m_WindowPosition.y+m_listboxPosition[m_SelectedListBoxPos].y+1);
			}
		}
	}

	DrawMouseOverImage();

//	POINT position = m_SkillButton[m_SelectedSkillButtonIndex]->GetWindowPos();
//	position.x = m_WindowPosition.x+30+(m_SelectedSkillButtonIndex*36);
	m_SelectedSkillButtonImage->Draw(m_WindowPosition.x+30+((m_SelectedSkillButtonIndex)*36), m_WindowPosition.y+15);
}

void _XWindow_Skill::DrawMouseOverImage(void)
{
	int selecteditem = CheckSelectedItem();
	
	if(selecteditem >= 0 && selecteditem != m_SelectedListBoxPos)
	{
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
			g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			m_SelectedImage->m_FColor = 0x7FFFFFFF;
			m_SelectedImage->Draw(m_WindowPosition.x+m_listboxPosition[selecteditem].x, m_WindowPosition.y+m_listboxPosition[selecteditem].y);
			m_SelectedImage->m_FColor = 0xFFFFFFFF;

			m_SelectedRightImage->m_FColor = 0x7FFFFFFF;
			m_SelectedRightImage->Draw(m_WindowPosition.x+m_listboxPosition[selecteditem].x+306, m_WindowPosition.y+m_listboxPosition[selecteditem].y);
			m_SelectedRightImage->m_FColor = 0xFFFFFFFF;
		}
		else
		{
			m_MouseOverLeftImage.Draw(m_WindowPosition.x+m_listboxPosition[selecteditem].x, m_WindowPosition.y+m_listboxPosition[selecteditem].y);
			m_MouseOverRightImage.Draw(m_WindowPosition.x+m_listboxPosition[selecteditem].x+156, m_WindowPosition.y+m_listboxPosition[selecteditem].y);
		}
	}
}

BOOL _XWindow_Skill::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// listbox의 스크롤이 되면 selected image 삭제
	if(m_PrevScrollPos < 0)
		m_PrevScrollPos = m_listboxSkill->GetScrollPos();
	else
	{
		int scrollpos = m_listboxSkill->GetScrollPos();
		if(scrollpos != m_PrevScrollPos)
		{
			m_SelectedListBoxPos = -1;
			m_bDrawSelectedImage = false;
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
			if(mousestate->bButton[1])
			{
				if(CheckMousePosition())
					m_WindowMouseState = _XW_STATE_RIGHTCLICK;
				else
				{
					if(m_bHaveMouseSlot)
						ResetMouseSlot();
				}
			}

			if(mousestate->bButton[0])
			{
				if(CheckMousePosition())
				{
					if(!m_bHaveMouseSlot)
					{
						// Icon 안쪽을 클릭 했으면
						int selectedicon = CheckSelectedIconItem();
						if(selectedicon >= 0)
							m_bDragState = true;

						if( g_pDefaultTooltip_Window )
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
					else
					{
						ResetMouseSlot();
					}
					
					m_WindowMouseState = _XW_STATE_LEFTCLICK;
				}
				else
				{
					if(m_bHaveMouseSlot)
					{
						if(g_pQuickSlot_Window)
						{
							if(!g_pQuickSlot_Window->CheckMousePosition())
								ResetMouseSlot();
						}
					}
				}
			}
		}

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				// QuickSlot에 등록
				if(m_bHaveMouseSlot)
					ResetMouseSlot();
				else
				{
					int selecteditem = CheckSelectedItem();
					if(selecteditem >= 0)
					{
						m_SelectedSkillID = (short)m_listboxSkill->GetSelectedItemCommandID(selecteditem);
						if(g_pQuickSlot_Window)
							g_pQuickSlot_Window->RegistSkillNextEmptySlot(m_SelectedSkillID);
					}
				}

				m_WindowMouseState = _XW_STATE_NONE;
			}
		}
		if(!mousestate->bButton[0])
		{
			if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
			{
				// icon 안쪽인지 체크
				int selectedicon = CheckSelectedIconItem();
				if(selectedicon >= 0)
				{
					if(m_bDragState)
					{
						// 드래그
						m_MouseSlotSkillID = (short)m_listboxSkill->GetSelectedItemCommandID(selectedicon);
						m_bHaveMouseSlot = true;
						m_bDragState = false;
					}
				}
				else
					m_bDragState = false;

				int selecteditem = CheckSelectedItem(true);
				if(selecteditem >= 0)
				{
					// 툴팁 띄우기
					m_bDrawSelectedImage = true;
					int id = (short)m_listboxSkill->GetSelectedItemCommandID(selecteditem);
					if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_SKILL, 0, 0, id ) )
					{
						g_pDefaultTooltip_Window->ShowWindow( TRUE );
					}

					_XPlayInterfaceSound(ID_SR_INTERFACE_SKILL_CLICK_WAV);
				}
				else
				{
					m_bDrawSelectedImage = false;
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}
				}

				m_WindowMouseState = _XW_STATE_NONE;
			}
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_Skill::SetListItem(void)
{
#ifdef _XTS_NEWSKILLWINDOW
	for(int ii = 0 ; ii < 8 ; ii++)
	{
		m_SkillSortButton[ii]->SetCheck(FALSE);
	}
	m_SkillSortButton[0]->SetCheck(TRUE);
#endif

	m_listboxSkill->DeleteAllItem();
	m_listboxSkill->SetScrollPos(0);

	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	RECT backgroundrect = {4, 52, 160, 98};
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		backgroundrect.left = 11;
		backgroundrect.top = 59;
		backgroundrect.right = 43;
		backgroundrect.bottom = 91;
	}
		
	int iconresourceindex = -1;
	RECT iconrect;

	TCHAR buffer[256];
	memset(buffer, 0, sizeof(TCHAR)*256);

	int count = 0;

	if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].stype == 0)
	{
		if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan == g_pLocalUser->m_CharacterInfo.groupindex)
		{
			// 전부
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
					
					m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);					
					m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
					
					if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
					{
						m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
					}
					
					count++;
				}
			}
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
			if(g_pLocalUser->m_EventSkillList.m_sSkillID > 0)
			{
				m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_EventSkillList.m_sSkillID);
					
				m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);					
				m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
				
				if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_EventSkillList.m_sSkillID, iconresourceindex, iconrect))
				{
					m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
				}

				count++;
			}
#endif
		}
		else if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan == _XGROUP_NANGIN)
		{
			// 낭인무공만
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->clanType == _XGROUP_NANGIN)
					{
						m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
						
						m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						
						if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
						{
							m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						}
						
						count++;
					}
				}
			}
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
			if(g_pLocalUser->m_EventSkillList.m_sSkillID > 0)
			{
				m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_EventSkillList.m_sSkillID);
					
				m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);					
				m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
				
				if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_EventSkillList.m_sSkillID, iconresourceindex, iconrect))
				{
					m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
				}

				count++;
			}
#endif
		}
	}
	else
	{
		// 해당 타입의 무공만
		for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
		{
			if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
			{
				if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->clanType == m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan &&
					g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].stype)
				{
					m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
					
					m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
					m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
					
					if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
					{
						m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
					}
					
					count++;
				}
			}
		}
	}

	// 무공 설명
	TCHAR descbuffer[2048];
	memset(descbuffer, 0, sizeof(descbuffer));

	TCHAR tempbuffer[1024];
	memset(tempbuffer, 0, sizeof(tempbuffer));

	int linecount = 0;

	m_lstboxSkillDesc->DeleteAllItem();
	m_lstboxSkillDesc->SetScrollPos(0);

	if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].stype == 0)
	{
		if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan == g_pLocalUser->m_CharacterInfo.groupindex)
		{
			// 문파 무공 설명
			sprintf(tempbuffer, _XGETINTERFACETEXT(ID_STRING_SKILL_WHO), g_JoinGroupName[g_pLocalUser->m_CharacterInfo.groupindex]); // %s의 무공
			sprintf(descbuffer, "  %s", tempbuffer);
			m_lstboxSkillDesc->InsertListItem(_T(""), 0, 0);
			m_lstboxSkillDesc->SetItemText(linecount, 0, descbuffer, TRUE);
			m_lstboxSkillDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 181, 181, 181));
			linecount++;

			_XStringSplitInfo splitinfo;
			g_XBaseFont->SplitString(g_SkillConceptProperty[g_pLocalUser->m_CharacterInfo.groupindex].clan_concept, 317, splitinfo, _XSTRINGSPLITFUNCTION_SPACECHAR);

			for(int i = 0 ; i < splitinfo.splitcount ; ++i)
			{
				memset(descbuffer, 0, sizeof(descbuffer));
				strncpy(descbuffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
				m_lstboxSkillDesc->InsertListItem(_T(""), 0, 0);
				m_lstboxSkillDesc->SetItemText(linecount, 0, descbuffer);
				m_lstboxSkillDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 181, 181, 181));
				linecount++;
			}
			m_lstboxSkillDesc->SetScrollPos(0);
		}
		else if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan == _XGROUP_NANGIN)
		{
			// 낭인 무공 설명
			sprintf(tempbuffer, _XGETINTERFACETEXT(ID_STRING_SKILL_WHO), g_JoinGroupName[_XGROUP_NANGIN]); // %s의 무공
			sprintf(descbuffer, "  %s", tempbuffer);
			m_lstboxSkillDesc->InsertListItem(_T(""), 0, 0);
			m_lstboxSkillDesc->SetItemText(linecount, 0, descbuffer, TRUE);
			m_lstboxSkillDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 181, 181, 181));
			linecount++;
			
			_XStringSplitInfo splitinfo;
			g_XBaseFont->SplitString(g_SkillConceptProperty[_XGROUP_NANGIN].clan_concept, 317, splitinfo, _XSTRINGSPLITFUNCTION_SPACECHAR);
			
			for(int i = 0 ; i < splitinfo.splitcount ; ++i)
			{
				memset(descbuffer, 0, sizeof(descbuffer));
				strncpy(descbuffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
				m_lstboxSkillDesc->InsertListItem(_T(""), 0, 0);
				m_lstboxSkillDesc->SetItemText(linecount, 0, descbuffer);
				m_lstboxSkillDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 181, 181, 181));
				linecount++;
			}
			m_lstboxSkillDesc->SetScrollPos(0);
		}
	}
	else
	{
		// 해당 타입 무공 설명
		char clan = m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan;
		char stype = m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].stype;

		sprintf(descbuffer, "  %s", g_SkillConceptProperty[clan].stype_name[stype]);
		m_lstboxSkillDesc->InsertListItem(_T(""), 0, 0);
		m_lstboxSkillDesc->SetItemText(linecount, 0, descbuffer, TRUE);
		m_lstboxSkillDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 181, 181, 181));
		linecount++;
		
		_XStringSplitInfo splitinfo;
		g_XBaseFont->SplitString(g_SkillConceptProperty[clan].stype_concept[stype], 317, splitinfo, _XSTRINGSPLITFUNCTION_SPACECHAR);
		
		for(int i = 0 ; i < splitinfo.splitcount ; ++i)
		{
			memset(descbuffer, 0, sizeof(descbuffer));
			strncpy(descbuffer, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
			m_lstboxSkillDesc->InsertListItem(_T(""), 0, 0);
			m_lstboxSkillDesc->SetItemText(linecount, 0, descbuffer);
			m_lstboxSkillDesc->SetItemAttrib(linecount, 0, D3DCOLOR_ARGB(255, 181, 181, 181));
			linecount++;
		}
		m_lstboxSkillDesc->SetScrollPos(0);
	}
}

#ifdef _XTS_NEWSKILLWINDOW
void _XWindow_Skill::SetListItemBySType(void)
{
	m_listboxSkill->DeleteAllItem();
	m_listboxSkill->SetScrollPos(0);
	
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	
	RECT backgroundrect = {4, 52, 160, 98};
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		backgroundrect.left = 11;
		backgroundrect.top = 59;
		backgroundrect.right = 43;
		backgroundrect.bottom = 91;
	}

	int iconresourceindex = -1;
	RECT iconrect;
	
	TCHAR buffer[256];
	memset(buffer, 0, sizeof(TCHAR)*256);
	
	int count = 0;

	switch(m_SelectedSortIndex)
	{
	case 0 :
		{
			// 전체
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
					
					m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
					m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
					
					if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
					{
						m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
					}
					
					count++;
				}
			}
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
			if(g_pLocalUser->m_EventSkillList.m_sSkillID > 0)
			{
				m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_EventSkillList.m_sSkillID);
					
				m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);					
				m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
				
				if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_EventSkillList.m_sSkillID, iconresourceindex, iconrect))
				{
					m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
				}

				count++;
			}
#endif
		}
		break;
	case 1 :
		{
			// 기본
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_02 ||
						g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_13 ||
						g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_03)
					{
						m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
						
						m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						
						if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
						{
							m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						}
						
						count++;
					}
				}
			}
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
			// 경공이므로 기본 탭에 추가
			if(g_pLocalUser->m_EventSkillList.m_sSkillID > 0)
			{
				m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_EventSkillList.m_sSkillID);
					
				m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);					
				m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
				
				if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_EventSkillList.m_sSkillID, iconresourceindex, iconrect))
				{
					m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
				}

				count++;
			}
#endif
		}
		break;
	case 2 :
		{
			// 초식
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)
					{
						m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
						
						m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						
						if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
						{
							m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						}
						
						count++;
					}
				}
			}
		}
		break;
	case 3 :
		{
			// 일격
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_SMASHINGSKILL)
					{
						m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
						
						m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						
						if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
						{
							m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						}
						
						count++;
					}
				}
			}
		}
		break;
	case 4 :
		{
			// 특수
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_CHARGESKILL ||
						g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_FINISHSKILL)
					{
						m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
						
						m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						
						if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
						{
							m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						}
						
						count++;
					}
				}
			}
		}
		break;
	case 5 :
		{
			// 강기
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
#ifdef _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI 
					// stype 10 제거
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_11 ||
						g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_07)
#else
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_10 ||
						g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_11 ||
						g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_07)
#endif
					{
						m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
						
						m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						
						if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
						{
							m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						}
						
						count++;
					}
				}
			}
		}
		break;
	case 6 :
		{
			// 공기
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_08 ||
						g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_09 ||
						g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType == _XSI_STYPE_12)
					{
						m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
						
						m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						
						if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
						{
							m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						}
						
						count++;
					}
				}
			}
		}
		break;
	case 7 :
		{
			// 잡기
			for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
				{
					if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_ACTIONSKILL)
					{
						m_listboxSkill->InsertListItem(_T(""), 0, g_pLocalUser->m_SkillList[i].m_sSkillID);
						
						m_listboxSkill->SetIconArchive(count, &g_MainInterfaceTextureArchive);
						m_listboxSkill->SetItemAttrib(count, 0, resourceindex, backgroundrect);
						
						if(g_pQuickSlot_Window->GetIconRect(g_pLocalUser->m_SkillList[i].m_sSkillID, iconresourceindex, iconrect))
						{
							m_listboxSkill->SetItemAttrib(count, 1, iconresourceindex, iconrect);
						}
						
						count++;
					}
				}
			}
		}
		break;
	}
}
#endif

void _XWindow_Skill::DrawText(void)
{
	int scrollpos = m_listboxSkill->GetScrollPos();
	int count = 0;
	FLOAT exp = 0.0f, gaugefactor = 0.0f;

	int index = 0, learnedskill = 0;
	short id = 0;

	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF222222);

	int listcount = 10;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		listcount = 5;
	}

	for(int i = 0 ; i < listcount; i++)
	{
		index = i + scrollpos*m_listboxSkill->GetColumnCount();
		id = m_listboxSkill->GetItemCommandID(index);

		if(id > 0)
		{
			learnedskill = g_pLocalUser->GetLearnedSkillIndex(id, TRUE);

			_XUserSkill* pSkill = NULL;
			if(learnedskill == _XDEF_MAX_LEARNSKILL)
			{
				pSkill = &g_pLocalUser->m_EventSkillList;
			}
			else
			{
				pSkill = g_pLocalUser->GetSkillItem(id);
			}

			// skillname, level
			g_XBaseFont->SetColor(0xFFB4B4B4);

#ifdef _XJAPANESE
			if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
			{
				if( g_SkillProperty[id]->skillHiraganaName && strlen(g_SkillProperty[id]->skillHiraganaName) )
				{
					g_XBaseFont->Print(m_WindowPosition.x+m_listboxPosition[i].x+45, m_WindowPosition.y+m_listboxPosition[i].y+9, 1.0f,
									   _T("%s (%s)"), g_SkillProperty[id]->skillName, g_SkillProperty[id]->skillHiraganaName );
				}
				else
				{
					g_XBaseFont->Puts(m_WindowPosition.x+m_listboxPosition[i].x+45, m_WindowPosition.y+m_listboxPosition[i].y+9, 
					g_SkillProperty[id]->skillName);
				}
			}
			else
#endif
			{
				g_XBaseFont->Puts(m_WindowPosition.x+m_listboxPosition[i].x+45, m_WindowPosition.y+m_listboxPosition[i].y+9, 
					g_SkillProperty[id]->skillName);
			}
			
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
				g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{

			}
			else
			{
				g_XBaseFont->Puts(m_WindowPosition.x+m_listboxPosition[i].x+138, m_WindowPosition.y+m_listboxPosition[i].y+9, 
								  _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG)	);
			}
			
			if(pSkill->m_cSkillLevel >= 11)
			{
				g_XBaseFont->SetColor(0xFFFCCE47);
				exp = (FLOAT)g_SkillProperty[id]->detailInfo[pSkill->m_cSkillLevel].skillExp;
				gaugefactor = 100.0f;
			}
			else
			{
				exp = (FLOAT)g_SkillProperty[id]->detailInfo[pSkill->m_cSkillLevel+1].skillExp;
				if(exp > 0.0f)
				{
					gaugefactor = ((FLOAT)pSkill->m_iSkillExp / exp) * 100;
				}
				else
				{
					gaugefactor = 0.0f;
				}
			}
			
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
				g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+303, m_WindowPosition.y+m_listboxPosition[i].y+26,
										1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d%s", pSkill->m_cSkillLevel+1, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG));
			}
			else
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+137, m_WindowPosition.y+m_listboxPosition[i].y+9,
										1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", pSkill->m_cSkillLevel+1);
			}			
			
			// draw gauge
			m_SkillExp->MoveWindow(m_WindowPosition.x+m_listboxPosition[i].x+45, m_WindowPosition.y+m_listboxPosition[i].y+29);
			m_SkillExp->SetGaugeFactor(gaugefactor);
			m_SkillExp->Draw();
			
			g_XSmallFont->SetColor(0xFFB4B4B4);
			if(pSkill->m_cSkillLevel >= 11)
			{
				g_XSmallFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+92, m_WindowPosition.y+m_listboxPosition[i].y+27, 
					1.0f, _XFONT_ALIGNTYPE_CENTER, "%3.2f%%", gaugefactor);
			}
			else
			{
				g_XSmallFont->PrintAlign(m_WindowPosition.x+m_listboxPosition[i].x+92, m_WindowPosition.y+m_listboxPosition[i].y+27, 
					1.0f, _XFONT_ALIGNTYPE_CENTER, "%3.2f%%", gaugefactor);
			}
			g_XSmallFont->Flush();

#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
			{
				if(pSkill->m_cCurMission == 1)
				{
					// 이벤트 무공이다. 끝나는 시간 알려주기
					int param1 = 0, param2 = 0, param3 = 0, param4 = 0, param5 = 0;

					param1 = g_pLocalUser->m_EventSkillEndTime.dwYear + 2000;
					param2 = g_pLocalUser->m_EventSkillEndTime.dwMonth;
					param3 = g_pLocalUser->m_EventSkillEndTime.dwDay;
					param4 = g_pLocalUser->m_EventSkillEndTime.dwHour;
					param5 = g_pLocalUser->m_EventSkillEndTime.dwMinute;

					g_XBaseFont->SetColor(_XSC_DEFAULT);

					g_XBaseFont->Print(m_WindowPosition.x+m_listboxPosition[i].x+152, m_WindowPosition.y+m_listboxPosition[i].y+26, 1.0f, "%s", 
						g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3171, 
					&param1, &param2, &param3, &param4,	&param5 ));
				}
			}
#endif
		}
	}

	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	/*
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 181, 181, 181));

	if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].stype == 0)
	{
		if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan == g_pLocalUser->m_CharacterInfo.groupindex)
		{
			// 문파 무공 설명
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->Print(m_WindowPosition.x+21, m_WindowPosition.y+67, 1.0f, _XGETINTERFACETEXT(ID_STRING_SKILL_WHO), g_JoinGroupName[g_pLocalUser->m_CharacterInfo.groupindex]);	// "%s의 무공"
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);

			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
			{
				g_XBaseFont->SetClippingArea( m_WindowPosition.x+6, m_WindowPosition.y+82, m_WindowPosition.x+6+336, m_WindowPosition.y+82+46 );
				g_XBaseFont->Puts_Separate(m_WindowPosition.x+7, m_WindowPosition.y+84, g_SkillConceptProperty[g_pLocalUser->m_CharacterInfo.groupindex].clan_concept, 335);
			}
			else
			{
				g_XBaseFont->SetClippingArea( m_WindowPosition.x+20, m_WindowPosition.y+82, m_WindowPosition.x+20+308, m_WindowPosition.y+82+46 );
				g_XBaseFont->Puts_Separate(m_WindowPosition.x+21, m_WindowPosition.y+84, g_SkillConceptProperty[g_pLocalUser->m_CharacterInfo.groupindex].clan_concept, 316);
			}

			g_XBaseFont->Flush();
			g_XBaseFont->DisableClippingArea();
		}
		else if(m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan == _XGROUP_NANGIN)
		{
			// 낭인 무공 설명
			g_XBaseFont->SetBoldMode(TRUE);
			g_XBaseFont->Print(m_WindowPosition.x+21, m_WindowPosition.y+67, 1.0f, _XGETINTERFACETEXT(ID_STRING_SKILL_WHO), g_JoinGroupName[_XGROUP_NANGIN]);	// "%s의 무공"
			g_XBaseFont->Flush();
			g_XBaseFont->SetBoldMode(FALSE);

			if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
			{
				g_XBaseFont->SetClippingArea( m_WindowPosition.x+6, m_WindowPosition.y+82, m_WindowPosition.x+20+336, m_WindowPosition.y+82+46 );
				g_XBaseFont->Puts_Separate(m_WindowPosition.x+7, m_WindowPosition.y+84, g_SkillConceptProperty[_XGROUP_NANGIN].clan_concept, 335);
			}
			else
			{
				g_XBaseFont->SetClippingArea( m_WindowPosition.x+20, m_WindowPosition.y+82, m_WindowPosition.x+20+308, m_WindowPosition.y+82+46 );
				g_XBaseFont->Puts_Separate(m_WindowPosition.x+21, m_WindowPosition.y+84, g_SkillConceptProperty[_XGROUP_NANGIN].clan_concept, 316);
			}

			g_XBaseFont->Flush();
			g_XBaseFont->DisableClippingArea();
		}
	}
	else
	{
		// 해당 타입의 무공 설명
		char clan = m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].clan;
		char stype = m_SkillButtonStr[m_StartIndex+m_SelectedSkillButtonIndex].stype;
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->Puts(m_WindowPosition.x+21, m_WindowPosition.y+67, g_SkillConceptProperty[clan].stype_name[stype]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);

		if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
		{
			g_XBaseFont->SetClippingArea( m_WindowPosition.x+6, m_WindowPosition.y+82, m_WindowPosition.x+20+336, m_WindowPosition.y+82+46 );
			g_XBaseFont->Puts_Separate(m_WindowPosition.x+7, m_WindowPosition.y+84, g_SkillConceptProperty[clan].stype_concept[stype], 335);
		}
		else
		{
			g_XBaseFont->SetClippingArea( m_WindowPosition.x+20, m_WindowPosition.y+82, m_WindowPosition.x+20+308, m_WindowPosition.y+82+46 );
			g_XBaseFont->Puts_Separate(m_WindowPosition.x+21, m_WindowPosition.y+84, g_SkillConceptProperty[clan].stype_concept[stype], 316);
		}

		g_XBaseFont->Flush();
		g_XBaseFont->DisableClippingArea();
	}
	*/
}

int _XWindow_Skill::CheckSelectedItem(bool bListPos)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int selecteditempos;


	int viewlistcount = 10;
	int checkwidth = 156;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		viewlistcount = 5;
		checkwidth = 316;
	}

	for(int i = 0 ; i < viewlistcount ; i++)
	{
		if( scrnpos->x >= m_WindowPosition.x + m_listboxPosition[i].x &&
			scrnpos->x <= m_WindowPosition.x + m_listboxPosition[i].x + checkwidth &&
			scrnpos->z >= m_WindowPosition.y + m_listboxPosition[i].y &&
			scrnpos->z <= m_WindowPosition.y + m_listboxPosition[i].y + 46)
		{
			selecteditempos = i;//m_listboxSkill->GetScrollPos() * m_listboxSkill->GetColumnCount() + i;

			if(m_listboxSkill->IsValidItem(selecteditempos))
			{
				if(bListPos)
					m_SelectedListBoxPos = i;

				return selecteditempos;
			}
		}
	}

	if(bListPos)
		m_SelectedListBoxPos = -1;

	return -1;
}

int _XWindow_Skill::CheckSelectedIconItem()
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int selecteditempos;
	
	int viewlistcount = 10;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		viewlistcount = 5;
	}

	for(int i = 0 ; i < viewlistcount ; i++)
	{
		if(scrnpos->x >= m_WindowPosition.x + m_listboxPosition[i].x + 8&&
			scrnpos->x <= m_WindowPosition.x + m_listboxPosition[i].x + 38 &&
			scrnpos->z >= m_WindowPosition.y + m_listboxPosition[i].y +8 &&
			scrnpos->z <= m_WindowPosition.y + m_listboxPosition[i].y + 38)
		{
			selecteditempos = i;//m_listboxSkill->GetScrollPos() * m_listboxSkill->GetColumnCount() + i;
			
			if(m_listboxSkill->IsValidItem(selecteditempos))
			{
				return selecteditempos;
			}
			else
				return -1;
		}
	}
	
	return -1;
}

void _XWindow_Skill::ProcessDblClick(void)
{
	int selecteditem = CheckSelectedItem();
	if(selecteditem >= 0)
	{
		m_SelectedSkillID = (short)m_listboxSkill->GetSelectedItemCommandID(selecteditem);
		if(g_pQuickSlot_Window)
			g_pQuickSlot_Window->RegistSkillNextEmptySlot(m_SelectedSkillID);
	}
}

void _XWindow_Skill::ResetMouseSlot(void)
{
	m_MouseSlotSkillID = 0;
	m_bHaveMouseSlot = false;
}

void _XWindow_Skill::DrawDragIcon(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	g_pQuickSlot_Window->DrawSkillIcon(m_MouseSlotSkillID, scrnpos->x-16, scrnpos->z-16);
}

void _XWindow_Skill::DrawClanStypeIcon(void)
{
	switch(g_pLocalUser->m_CharacterInfo.groupindex)
	{
	case _XGROUP_NANGIN :
		{
		}
		break;
	}
}

void _XWindow_Skill::SetSkillButton(void)
{
	int count = 0;
	m_SelectedSkillButtonIndex = 0;

	for(int i = 0 ; i < 20 ; i++)
	{
		m_SkillButtonStr[i].clan = -1;
		m_SkillButtonStr[i].stype = 0;
	}

	m_SkillButtonStr[0].clan = g_pLocalUser->m_CharacterInfo.groupindex;		// 문파 번호
	m_SkillButtonStr[0].stype = 0;
	count++;

	if(g_pLocalUser->m_CharacterInfo.groupindex > 0)
	{
//		if(g_pLocalUser->HaveSkillType(g_pLocalUser->m_CharacterInfo.groupindex))
		{
			m_SkillButtonStr[1].clan = 0;		// 문파에 가입한 경우 낭인 무공
			m_SkillButtonStr[1].stype = 0;
			count++;
		}
	}

	if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
	{
		// 공통 무공
//		m_SkillButtonStr[1].clan = 0;
//		m_SkillButtonStr[1].stype = 1;
	}
	else
	{
		// 문파 있음
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() > 0)
		{
			// 역할 있음
			int j = 2;
			for(int i = 2 ; i < 20 ; i++)
			{
				if(g_SkillConceptProperty[g_pLocalUser->m_CharacterInfo.groupindex].class_stype[g_pLocalUser->m_CharacterInfo.Get_cClass()][i-2] > 0)
				{
					if(g_pLocalUser->HaveSkillType(g_pLocalUser->m_CharacterInfo.groupindex, g_SkillConceptProperty[g_pLocalUser->m_CharacterInfo.groupindex].class_stype[g_pLocalUser->m_CharacterInfo.Get_cClass()][i-2]))
					{
						m_SkillButtonStr[j].clan = g_pLocalUser->m_CharacterInfo.groupindex;
						m_SkillButtonStr[j].stype = g_SkillConceptProperty[g_pLocalUser->m_CharacterInfo.groupindex].class_stype[g_pLocalUser->m_CharacterInfo.Get_cClass()][i-2];
						count++;
						j++;
					}
				}
				else
				{
//					m_SkillButtonStr[i].clan = -1;
//					m_SkillButtonStr[i].stype = 0;
				}
			}
		}
		else
		{
			// 역할 없음
		}
	}

	m_SkillButtonStrTotalCount = count;
	SetSkillButtonTexture();
}

void _XWindow_Skill::SetSkillButtonTexture(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	for(int i = 0 ; i < 8 ; i++)
	{
		if(m_SkillButtonStr[m_StartIndex+i].clan >= 0 && m_SkillButtonStr[m_StartIndex+i].stype == 0)
		{
			m_SkillButton[i]->SetButtonImage(resourceindex, resourceindex, resourceindex, &g_MainInterfaceTextureArchive);
			m_SkillButton[i]->ShowWindow(TRUE);

			// 문파 아이콘			
			switch(m_SkillButtonStr[m_StartIndex+i].clan)
			{
			case _XGROUP_NANGIN :
				{
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 120, 226, 150, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 120, 226, 150, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 120, 226, 150, 256);
				}
				break;
			case _XGROUP_GAEBANG :
				{
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 90, 226, 120, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 90, 226, 120, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 90, 226, 120, 256);
				}
				break;
			case _XGROUP_BEEGOONG :
				{
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 226, 30, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 226, 30, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 226, 30, 256);
				}
				break;
			case _XGROUP_SORIM :
				{
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 60, 226, 90, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 60, 226, 90, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 60, 226, 90, 256);
				}
				break;
			case _XGROUP_NOCKRIM :
				{
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 30, 226, 60, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 30, 226, 60, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 30, 226, 60, 256);
				}
				break;
			case _XGROUP_MOODANG :
				{
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 30, 226, 60, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 30, 226, 60, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 30, 226, 60, 256);
				}
				break;
			case _XGROUP_MAKYO :
				{
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 30, 226, 60, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 30, 226, 60, 256);
					m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 30, 226, 60, 256);
				}
				break;
			}
		}
		else if(m_SkillButtonStr[m_StartIndex+i].clan > 0 && m_SkillButtonStr[m_StartIndex+i].stype > 0)
		{
			char clan = m_SkillButtonStr[m_StartIndex+i].clan;
			int subindex = clan/5 + 1;
			if(clan > 4)
			{
				clan -= 4;
			}

			TCHAR resourcename[32];
			sprintf(resourcename, "icon_skilllist%02d.tga", subindex);
			
			int resourceindex_1 = g_MainInterfaceTextureArchive.FindResource(resourcename);
			m_SkillButton[i]->SetButtonImage(resourceindex_1, resourceindex_1, resourceindex_1, &g_MainInterfaceTextureArchive);
			m_SkillButton[i]->ShowWindow(TRUE);

			RECT rect;
			if(m_SkillButtonStr[m_StartIndex+i].stype < 9)
			{
				rect.left = 1 + (m_SkillButtonStr[m_StartIndex+i].stype-1)*32;
				rect.top = 1 + (clan-1)*64;
				rect.right = rect.left + 30;
				rect.bottom = rect.top + 30;
			}
			else
			{
				rect.left = 1 + (m_SkillButtonStr[m_StartIndex+i].stype-9)*32;
				rect.top = 1 + (clan-1)*64 + 32;
				rect.right = rect.left + 30;
				rect.bottom = rect.top + 30;
			}

			m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, rect.left, rect.top, rect.right, rect.bottom);
			m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, rect.left, rect.top, rect.right, rect.bottom);
			m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, rect.left, rect.top, rect.right, rect.bottom);
		}
		else
		{
			m_SkillButton[i]->ShowWindow(FALSE);
		}
	}

	if(m_StartIndex > 0)
		m_LeftButton->ShowWindow(TRUE);
	else
		m_LeftButton->ShowWindow(FALSE);

	if(m_StartIndex+8 < m_SkillButtonStrTotalCount)
		m_RightButton->ShowWindow(TRUE);
	else
		m_RightButton->ShowWindow(FALSE);

	SetListItem();
}

void _XWindow_Skill::RefreshData(void)
{
	m_PrevScrollPos = -1;
	m_SelectedListBoxPos = -1;
	m_SelectedSkillID = 0;
	m_bDrawSelectedImage = false;
	m_SelectedSkillButtonIndex = 0;
	m_StartIndex = 0;
	
	SetSkillButton();
}

void _XWindow_Skill::SetStartIndex(int index)
{
	if(index < 0)
		index = 0;

	if(index >= m_SkillButtonStrTotalCount)
		index = m_SkillButtonStrTotalCount-1;

	m_StartIndex = index;
}

#ifdef _XTS_NEWSKILLWINDOW
void _XWindow_Skill::ChangeTab(int tab)
{
	m_SelectedSortIndex = tab;
	
	for(int i = 0 ; i < 8 ; i++)
	{
		m_SkillSortButton[i]->SetCheck(FALSE);
	}
	
	m_SkillSortButton[tab]->SetCheck(TRUE);
}
#endif


BOOL __stdcall _XSkillSelectCheckCallBack( int param1, int param2 )
{
#ifdef _XDWDEBUG
	if( g_SinglePlayMode )
	{
		return TRUE;	
	}
#endif
	
	if( g_ZeroLevelMode )	// hotblood zerolevel
		return TRUE;
	
	if(g_pSkill_Window)
	{				
		short SkillID = (short)param2;
		
		// 운기 조식일 경우 무기에 상관없이
		if(g_SkillProperty[SkillID]->usefulType == _XSI_USEFULTYPE_HIDDENMA)
		{
//			if(g_SkillProperty[SkillID]->skillSType == _XSI_STYPE_02)
				return TRUE;
		}
		
		int skillmaxuselevel = -1;
		
		// 무공을 사용할 수 있는 지 검사 - 현재 장착된 무기와 검사				
		WORD ItemUniqueID = g_pLocalUser->m_CharacterInfo.modeldescript.item1;
		
		if(ItemUniqueID == 0)
		{
			if(g_pLocalUser->m_CurrentWeaponID > 0)
				ItemUniqueID = g_pLocalUser->m_CurrentWeaponID;
		}
		
		if(ItemUniqueID != 0)
		{
#ifdef _XTS_ITEM_OPTIMIZATION
			if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_CharacterInfo.modeldescript.item1Category) )
			{
				_XGI_WeaponItem_Property* tempWeaponitem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(g_pLocalUser->m_CharacterInfo.modeldescript.item1Category);
				if( tempWeaponitem )
				{
					if(tempWeaponitem[ItemUniqueID].cSecondType == g_SkillProperty[SkillID]->weaponCType)
					{
						if(tempWeaponitem[ItemUniqueID].cThirdType == g_SkillProperty[SkillID]->weaponSType)
						{
							return TRUE;
						}
					}
				}
			}
#else
			if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON )
			{
				if(g_WeaponItemProperty[ItemUniqueID].cSecondType == g_SkillProperty[SkillID]->weaponCType)
				{
					if(g_WeaponItemProperty[ItemUniqueID].cThirdType == g_SkillProperty[SkillID]->weaponSType)
					{
						return TRUE;
					}
				}
			}
			else if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON2 )
			{
				if(g_WeaponItemProperty2[ItemUniqueID].cSecondType == g_SkillProperty[SkillID]->weaponCType)
				{
					if(g_WeaponItemProperty2[ItemUniqueID].cThirdType == g_SkillProperty[SkillID]->weaponSType)
					{
						return TRUE;
					}
				}
			}
			else if( g_pLocalUser->m_CharacterInfo.modeldescript.item1Category == _XGI_FC_WEAPON3 )
			{
				if(g_WeaponItemProperty3[ItemUniqueID].cSecondType == g_SkillProperty[SkillID]->weaponCType)
				{
					if(g_WeaponItemProperty3[ItemUniqueID].cThirdType == g_SkillProperty[SkillID]->weaponSType)
					{
						return TRUE;
					}
				}
			}
#endif
			
			if(g_SkillProperty[SkillID]->weaponCType == -1 && g_SkillProperty[SkillID]->weaponSType == 0)
			{
				// 모든 무기 장착 가능
				return TRUE;
			}
		}
		else 
		{
			if(g_SkillProperty[SkillID]->weaponCType == -1 && g_SkillProperty[SkillID]->weaponSType == 0)
			{
				return TRUE;
			}
			// 장착된 무기 없음
			else if(g_SkillProperty[SkillID]->weaponCType == -1 && g_SkillProperty[SkillID]->weaponSType == -1)
			{
				return TRUE;
			}
			else if( ( g_SkillProperty[SkillID]->weaponCType == _XGI_SC_WEAPON_SHORT ) && 
				( g_SkillProperty[SkillID]->weaponSType == _XGI_TC_WEAPON_FIST ) )
			{
				return TRUE;
			}
		}
		
	}
	
	g_NetworkKernel.InsertChatString( _XGETINTERFACETEXT(ID_STRING_SKILL_WARNUSESKILL), _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE, 0xFFFF0000 );
	
	return FALSE;
}
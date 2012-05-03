// XWindow_QuickSlot.cpp: implementation of the _XWindow_QuickSlot class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_TutorialIcon.h"
#include "XWindow_GuideLine.h"
#include "XWindow_QuickSlot.h"
#include "SoundEffectList.h"

_XImageStatic				g_SkillIconImage;

_XWindow_QuickSlot*			g_pQuickSlot_Window;

void __stdcall _XCallback_PeaceMode( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_QSLOT_PEACE), _XSC_INFORMATION );		// _T("평화상태")
}
void __stdcall _XCallback_BattleMode( POINT pos, SIZE size )
{
 	if( g_bShowItemMallWindow ) return;
	g_InterfaceTooltip.SetToolTipString( pos.x+4, pos.y-15, _XGETINTERFACETEXT(ID_STRING_QSLOT_BATTLE), _XSC_INFORMATION );		// _T("전투상태")
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_QuickSlot::_XWindow_QuickSlot()
{
	g_pQuickSlot_Window		= this;

	m_SelectedActiveImage	= NULL;
	m_SelectedOtherImage	= NULL;
	
	m_PrevSetButton			= NULL;
	m_NextSetButton			= NULL;

	m_PeaceModeButton		= NULL;
	m_BattleModeButton		= NULL;

	for(int i = 0 ; i < 10 ; i++)
	{
		m_SkillExpGauge[i]	= NULL;
		m_SlotNumberImage[i]	= NULL;
	}

	for(i = 0 ; i < 5 ; i++)
	{
		m_BattleModeImage[i] = NULL;
#ifdef _XTS_PK
		m_PKModeImage[i] = NULL;
#endif
	}

	m_PrevDrawTime			= 0;
	m_EffectOrderSub		= false;
	m_EffectFrame			= 0.0f;

	m_SelectedSet			= 0;
	m_SelectedSlot			= 0;
	memset(m_QuickSlot, 0, sizeof(m_QuickSlot));
	memset(m_PositionArray, 0, sizeof(m_PositionArray));

	m_MouseSlotType			= _XQS_SLOTTYPE_NONE;
	m_MouseSlotSkillID		= 0;
	m_bHaveMouseSlot		= false;
	m_OriginSlotIndex		= -1;	
	memset(&m_IconDragStartPos, 0, sizeof(POINT));

	m_MouseSlotItem.ResetMouseSlotItem();

	m_bDragState			= false;
	m_LastDraggingSlot		= -1;

	m_CurrentSelectedActiveIndex	= -1;
	m_CurrentSelectedOtherIndex		= -1;
	m_CurrentSelectedItemIndex		= -1;

	m_CurrentSelectedItemStartTime		= 0;
	m_CurrentSelectedActiveStartTime	= 0;
	m_CurrentSelectedOtherStartTime		= 0;

	m_pSkillExpGauge		= NULL;	
	
	m_LeftLevelButton		= NULL;
	m_RightLevelButton		= NULL;
	m_bDrawLevelButton		= false;
	m_LevelButtonIndex		= -1;

	m_PrevPassiveDrawTime	= 0;
	m_PassiveEffectFrame	= 0.0f;

	memset(&m_CooldownIndex, 0, sizeof(m_CooldownIndex));
	memset(&m_DelayTimeIndex, 0, sizeof(m_DelayTimeIndex));

	m_bChangeItem = FALSE;
}

_XWindow_QuickSlot::~_XWindow_QuickSlot()
{
	DestroyWindow();
}

BOOL _XWindow_QuickSlot::Initialize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_chat_quick.tga");

	_XImageStatic* pLeftImage = new _XImageStatic;
	pLeftImage->Create(0, 0, 256, 48, &g_MainInterfaceTextureArchive, resourceindex);
	pLeftImage->SetClipRect(0, 109, 256, 157);
	InsertChildObject(pLeftImage);

	_XImageStatic* pRightImage = new _XImageStatic;
	pRightImage->Create(256, 0, 204, 48, &g_MainInterfaceTextureArchive, resourceindex);
	pRightImage->SetClipRect(52, 157, 256, 205);
	InsertChildObject(pRightImage);

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {211, 2}, {38, 38}, _XDEF_QS_BATTLEMODEBUTTON,
		resourceindex,
		resourceindex, 
		resourceindex,
		&g_MainInterfaceTextureArchive
	};
	m_PeaceModeButton = new _XButton;
	m_PeaceModeButton->Create(btnstruct);
	m_PeaceModeButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 157, 38, 195);
	m_PeaceModeButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 157, 38, 195);
	m_PeaceModeButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 157, 38, 195);
	m_PeaceModeButton->SetCallBackFunction_ToolTip( _XCallback_PeaceMode );
	InsertChildObject(m_PeaceModeButton);

	m_BattleModeButton = new _XButton;
	m_BattleModeButton->Create(btnstruct);
	m_BattleModeButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 195, 38, 233);
	m_BattleModeButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 195, 38, 233);
	m_BattleModeButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 195, 38, 233);
	m_BattleModeButton->ShowWindow(FALSE);
	m_BattleModeButton->SetCallBackFunction_ToolTip( _XCallback_BattleMode );
	InsertChildObject(m_BattleModeButton);

	btnstruct.position.x = 211;
	btnstruct.position.y = 41;
	btnstruct.windowsize.cx = 13;
	btnstruct.windowsize.cy = 6;
	btnstruct.commandid = _XDEF_QS_PREVBUTTON;
	m_PrevSetButton = new _XButton;
	m_PrevSetButton->Create(btnstruct);
	m_PrevSetButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 235, 13, 241);
	m_PrevSetButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 13, 235, 26, 241);
	m_PrevSetButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 26, 235, 39, 241);
	InsertChildObject(m_PrevSetButton);

	btnstruct.position.x = 236;
	btnstruct.position.y = 41;
	btnstruct.commandid = _XDEF_QS_NEXTBUTTON;
	m_NextSetButton = new _XButton;
	m_NextSetButton->Create(btnstruct);
	m_NextSetButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 241, 13, 247);
	m_NextSetButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 13, 241, 26, 247);
	m_NextSetButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 26, 241, 39, 247);
	InsertChildObject(m_NextSetButton);

	g_SkillIconImage.Create(0, 0, 31, 31, &g_MainInterfaceTextureArchive, 0);
	g_SkillIconImage.SetClipRect(0, 0, 32, 32);

	for(int i = 0 ; i < 5 ; i++)
	{
		m_BattleModeImage[i] = new _XImageStatic;
		m_BattleModeImage[i]->Create(0, 0, 27, 27, &g_MainInterfaceTextureArchive, resourceindex);
	}
	m_BattleModeImage[0]->SetClipRect(38, 205, 67, 234);
	m_BattleModeImage[1]->SetClipRect(67, 205, 96, 234);
	m_BattleModeImage[2]->SetClipRect(96, 205, 125, 234);
	m_BattleModeImage[3]->SetClipRect(125, 205, 154, 234);
	m_BattleModeImage[4]->SetClipRect(154, 205, 183, 234);

#ifdef _XTS_PK
	int pkresourceindex = -1;

	if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
		pkresourceindex = g_MainInterfaceTextureArchive.FindResource("tw_mi_pk_01.tga");
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		pkresourceindex = g_MainInterfaceTextureArchive.FindResource("us_mi_pk_01.tga");	
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		pkresourceindex = g_MainInterfaceTextureArchive.FindResource("rs_mi_pk_01.tga");	
	else
		pkresourceindex = g_MainInterfaceTextureArchive.FindResource("mi_pk_01.tga");	

	for(i = 0 ; i < 5 ; i++)
	{
		m_PKModeImage[i] = new _XImageStatic;
		m_PKModeImage[i]->Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, pkresourceindex);
	}
	m_PKModeImage[0]->SetClipRect(0, 92, 30, 122);
	m_PKModeImage[1]->SetClipRect(31, 92, 61, 122);
	m_PKModeImage[2]->SetClipRect(62, 92, 92, 122);
	m_PKModeImage[3]->SetClipRect(93, 92, 123, 122);
	m_PKModeImage[4]->SetClipRect(124, 92, 154, 122);
#endif
		
	for(i = 0 ; i < 10 ; i++)
	{
		m_SlotNumberImage[i] = new _XImageStatic;
		m_SlotNumberImage[i]->Create(0, 0, 7, 7, &g_MainInterfaceTextureArchive, resourceindex);		
	}

	m_SlotNumberImage[0]->SetClipRect(135, 235, 142, 242);
	m_SlotNumberImage[1]->SetClipRect(142, 235, 149, 242);
	m_SlotNumberImage[2]->SetClipRect(150, 235, 157, 242);
	m_SlotNumberImage[3]->SetClipRect(158, 235, 165, 242);
	m_SlotNumberImage[4]->SetClipRect(166, 235, 173, 242);
	m_SlotNumberImage[5]->SetClipRect(135, 243, 142, 250);
	m_SlotNumberImage[6]->SetClipRect(142, 243, 149, 250);
	m_SlotNumberImage[7]->SetClipRect(150, 243, 157, 250);
	m_SlotNumberImage[8]->SetClipRect(158, 243, 165, 250);
	m_SlotNumberImage[9]->SetClipRect(166, 243, 173, 250);

	_XSTICKGAUGE_STRUCTURE gaugestruct =
	{
		TRUE, {0, 0}, {39, 6}, resourceindex, -1, &g_MainInterfaceTextureArchive
	};

	m_pSkillExpGauge = new _XStickGauge;
	m_pSkillExpGauge->Create(gaugestruct);
	m_pSkillExpGauge->SetImageClipRect(_XGAUGEIMAGE_BORDER, 0, 250, 38, 256);
	m_pSkillExpGauge->SetFactorDraw(FALSE);

	m_SelectedActiveImage = new _XImageStatic;
	m_SelectedActiveImage->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex);
	m_SelectedActiveImage->SetClipRect(184, 220, 220, 256);

	m_SelectedOtherImage = new _XImageStatic;
	m_SelectedOtherImage->Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, resourceindex);
	m_SelectedOtherImage->SetClipRect(220, 220, 256, 256);

	btnstruct.position.x = 0;
	btnstruct.position.y = 0;
	btnstruct.windowsize.cx = 6;
	btnstruct.windowsize.cy = 7;
	btnstruct.commandid = _XDEF_QS_LEVELDOWNBUTTON;
	m_LeftLevelButton = new _XButton;
	m_LeftLevelButton->Create(btnstruct);
	m_LeftLevelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 54, 236, 60, 243);
	m_LeftLevelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 54, 236, 60, 243);
	m_LeftLevelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 42, 236, 48, 243);
	m_LeftLevelButton->ShowWindow(FALSE);
	InsertChildObject(m_LeftLevelButton);

	btnstruct.position.x = 0;
	btnstruct.position.y = 0;
	btnstruct.windowsize.cx = 6;
	btnstruct.windowsize.cy = 7;
	btnstruct.commandid = _XDEF_QS_LEVELUPBUTTON;
	m_RightLevelButton = new _XButton;
	m_RightLevelButton->Create(btnstruct);
	m_RightLevelButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 60, 236, 66, 243);
	m_RightLevelButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 60, 236, 66, 243);
	m_RightLevelButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 48, 236, 54, 243);
	m_RightLevelButton->ShowWindow(FALSE);
	InsertChildObject(m_RightLevelButton);


	for(i = 0 ; i < 5 ; i++)
	{
		m_PositionArray[i].x = 8 + 40*i;
		m_PositionArray[i].y = 3;
	}
	for(i = 0 ; i < 5 ; i++)
	{
		m_PositionArray[i+5].x = 256 + 40*i;
		m_PositionArray[i+5].y = 3;
	}

	for(i = 0 ; i < 64 ; i++)
	{
		m_CooldownImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_cooltime.tga"));
		m_CooldownImage[i].SetClipRect(1+(i%8)*32, 1+(i/8)*32, 1+(i%8)*32+30, 1+(i/8)*32+30);
	}
	for(i = 64 ; i < 125 ; i++)
	{
		m_CooldownImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_cooltime_02.tga"));
		m_CooldownImage[i].SetClipRect(1+((i-64)%8)*32, 1+((i-64)/8)*32, 1+((i-64)%8)*32+30, 1+((i-64)/8)*32+30);
	}
	
	for(i = 0 ; i < 8 ; i++)
	{
		m_PassiveOnImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_tra_effect.tga"));
		m_PassiveOnImage[i].SetClipRect(1+(32*i), 181, 1+(32*i)+30, 211);
	}
	m_PassiveOnImage[8].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_tra_effect.tga"));
	m_PassiveOnImage[8].SetClipRect(1, 213, 31, 243);
	
	int resourceindex1 = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "us_tutorial_01.tga" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "rs_tutorial_01.tga" );
	}
	else
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "tutorial_01.tga" );
	}

	m_GuideLineImage.Create(0, 0, 207, 45, &g_MainInterfaceTextureArchive, resourceindex1);
	m_GuideLineImage.SetClipRect( 0, 148, 208, 194 );
	m_GuideLineImage.ShowWindow(FALSE);
	

	return TRUE;
}

void _XWindow_QuickSlot::DestroyWindow(void)
{
	for(int i = 0 ; i < 10 ; i++)
		SAFE_DELETE(m_SlotNumberImage[i]);

	for(i = 0 ; i < 5 ; i++)
	{
		SAFE_DELETE(m_BattleModeImage[i]);
#ifdef _XTS_PK
		SAFE_DELETE(m_PKModeImage[i]);
#endif
	}

	SAFE_DELETE(m_pSkillExpGauge);
	SAFE_DELETE(m_SelectedActiveImage);
	SAFE_DELETE(m_SelectedOtherImage);

	_XWindow::DestroyWindow();
}

void _XWindow_QuickSlot::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	if(g_pLocalUser->m_BattleMode != _XBATTLE_PEACE)
		DrawBattleMode();

	int			index = 0;
	POINT		position;

	for(int i = 0 ; i < 10 ; i++)
	{
		index = (m_SelectedSet*10) + i;

		if(m_QuickSlot[index].used)
		{
			position.x = m_WindowPosition.x + m_PositionArray[i].x + 2;
			position.y = m_WindowPosition.y + m_PositionArray[i].y + 2;

			if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
			{
				g_SkillIconImage.EnableWindow(!m_QuickSlot[index].bChangeItem); //Author : 양희왕 //brief : 아이템 교체중은 비활성화
				DrawSkillIcon(m_QuickSlot[index].skillid, position.x, position.y);
			}
			else if(m_QuickSlot[index].type == _XQS_SLOTTYPE_ITEM)
			{
				if(g_pInventory_Window)
					g_pInventory_Window->DrawIcon(TRUE, position.x, position.y, &g_pLocalUser->m_UserItemList[m_QuickSlot[index].invenNumber]);
			}
			else if(m_QuickSlot[index].type == _XQS_SLOTTYPE_LIFESKILL)
			{
				if(g_pLifeSkill_Window)
				{
					if( m_QuickSlot[index].lifeskillid == 1)
					{
						g_pLifeSkill_Window->m_imagePersonalStoreIcon->Draw(position.x, position.y);
					}
					else if(m_QuickSlot[index].lifeskillid == 2)
					{
						g_pLifeSkill_Window->m_imagePVPMark->Draw(position.x, position.y);
					}
#ifdef _XDEF_EVENTWINDOW
					else if(m_QuickSlot[index].lifeskillid == 3)
					{
						g_pLifeSkill_Window->m_imageEventIcon->Draw(position.x, position.y);
					}
#endif
				}
			}
		}
	}

	// Slot Number
	for(i = 0 ; i < 10 ; i++)
	{
		m_SlotNumberImage[i]->Draw(m_WindowPosition.x+m_PositionArray[i].x+25, m_WindowPosition.y+m_PositionArray[i].y+4);
	}

	g_XSmallFont->SetColor(D3DCOLOR_ARGB(255, 253, 255, 215));
	g_XSmallFont->Print(m_WindowPosition.x + 227, m_WindowPosition.y + 39, 1.0f, "%d", m_SelectedSet+1);
	g_XSmallFont->Flush();

	if(m_CurrentSelectedActiveIndex > -1 && m_CurrentSelectedActiveIndex < 10)
	{
		if(g_LocalSystemTime - m_CurrentSelectedActiveStartTime <= 150)
		{
		}
		else if(g_LocalSystemTime - m_CurrentSelectedActiveStartTime <= 300)
		{
			m_SelectedActiveImage->Draw(m_WindowPosition.x+m_PositionArray[m_CurrentSelectedActiveIndex].x, 
				m_WindowPosition.y+m_PositionArray[m_CurrentSelectedActiveIndex].y);
		}
		else if(g_LocalSystemTime - m_CurrentSelectedActiveStartTime <= 450)
		{
		}
		else if(g_LocalSystemTime - m_CurrentSelectedActiveStartTime <= 600)
		{
			m_SelectedActiveImage->Draw(m_WindowPosition.x+m_PositionArray[m_CurrentSelectedActiveIndex].x, 
				m_WindowPosition.y+m_PositionArray[m_CurrentSelectedActiveIndex].y);
		}
		else if(g_LocalSystemTime - m_CurrentSelectedActiveStartTime <= 750)
		{
		}
		else
		{
			m_SelectedActiveImage->Draw(m_WindowPosition.x+m_PositionArray[m_CurrentSelectedActiveIndex].x, 
				m_WindowPosition.y+m_PositionArray[m_CurrentSelectedActiveIndex].y);
		}
	}
	if(m_CurrentSelectedOtherIndex > -1 && m_CurrentSelectedOtherIndex < 10)
	{
		if(g_LocalSystemTime - m_CurrentSelectedOtherStartTime <= 150)
		{
		}
		else if(g_LocalSystemTime - m_CurrentSelectedOtherStartTime <= 300)
		{
			m_SelectedOtherImage->Draw(m_WindowPosition.x+m_PositionArray[m_CurrentSelectedOtherIndex].x, 
				m_WindowPosition.y+m_PositionArray[m_CurrentSelectedOtherIndex].y);
		}
		else if(g_LocalSystemTime - m_CurrentSelectedOtherStartTime <= 450)
		{
		}
		else if(g_LocalSystemTime - m_CurrentSelectedOtherStartTime <= 600)
		{
			m_SelectedOtherImage->Draw(m_WindowPosition.x+m_PositionArray[m_CurrentSelectedOtherIndex].x, 
				m_WindowPosition.y+m_PositionArray[m_CurrentSelectedOtherIndex].y);
		}
		else if(g_LocalSystemTime - m_CurrentSelectedOtherStartTime <= 750)
		{
		}
		else
		{
			m_SelectedOtherImage->Draw(m_WindowPosition.x+m_PositionArray[m_CurrentSelectedOtherIndex].x, 
				m_WindowPosition.y+m_PositionArray[m_CurrentSelectedOtherIndex].y);
		}
	}
	if(m_CurrentSelectedItemIndex > -1 && m_CurrentSelectedItemIndex < 10)
	{
		if(g_LocalSystemTime - m_CurrentSelectedItemStartTime <= 150)
		{
		}
		else if(g_LocalSystemTime - m_CurrentSelectedItemStartTime <= 300)
		{
			m_SelectedActiveImage->Draw(m_WindowPosition.x+m_PositionArray[m_CurrentSelectedItemIndex].x, 
				m_WindowPosition.y+m_PositionArray[m_CurrentSelectedItemIndex].y);
		}
		else if(g_LocalSystemTime - m_CurrentSelectedItemStartTime <= 450)
		{
		}
		else if(g_LocalSystemTime - m_CurrentSelectedItemStartTime <= 600)
		{
			m_SelectedActiveImage->Draw(m_WindowPosition.x+m_PositionArray[m_CurrentSelectedItemIndex].x, 
				m_WindowPosition.y+m_PositionArray[m_CurrentSelectedItemIndex].y);
		}
		else
			m_CurrentSelectedItemIndex = -1;
	}

	if(!m_bDrawLevelButton)
		DrawSkillExpGauge();

	DrawCooldownImage();
	DrawCooldownLastImage();
	DrawPassiveOnImage();

	DrawDelayTime();

	DrawSkillLevel();
	DrawTooltip();

	if( m_GuideLineImage.GetShowStatus() )
	{
		m_GuideLineImage.Draw(m_WindowPosition.x+2, m_WindowPosition.y+1 );
		m_GuideLineImage.Draw(m_WindowPosition.x+251, m_WindowPosition.y+1 );
	}
}

void _XWindow_QuickSlot::DrawBattleMode(void)
{
	if(m_PrevDrawTime == 0)
		m_PrevDrawTime = g_LocalSystemTime;

	DWORD elapsedtime = g_LocalSystemTime - m_PrevDrawTime;
	
	if(!m_EffectOrderSub)
		m_EffectFrame += ((FLOAT)elapsedtime / 100.0f);
	else 
		m_EffectFrame -= ((FLOAT)elapsedtime / 100.0f);

	if(m_EffectFrame > 5.0f)
	{
		m_EffectOrderSub = true;
		m_EffectFrame = 4.0f;
	}
	else if(m_EffectFrame <= 0.0f)
	{
		m_EffectOrderSub = false;
		m_EffectFrame = 1.0f;
	}


	int selectedframe = (int)m_EffectFrame;

	if(selectedframe > 4)
		selectedframe = 4;
	else if(selectedframe < 0)
		selectedframe = 0;
		
#ifdef _XTS_PK
	if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
		m_PKModeImage[selectedframe]->Draw(m_WindowPosition.x+215, m_WindowPosition.y+6);
	else if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
#endif
		m_BattleModeImage[selectedframe]->Draw(m_WindowPosition.x+216, m_WindowPosition.y+7);

	m_PrevDrawTime = g_LocalSystemTime;
}


BOOL _XWindow_QuickSlot::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO
	BOOL		bRegist = false;
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[1])
			{
				BOOL clickedwindowregion = CheckMousePosition();
				if( g_pLocalUser->m_bMatchPRMode )
				{
					if( clickedwindowregion )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_QSLOT_2570), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				} 
				else 
				{
					// right button down
					if(clickedwindowregion)
						m_WindowMouseState = _XW_STATE_RIGHTCLICK;
					else
					{
						if(m_bHaveMouseSlot)
						{
							m_bHaveMouseSlot = false;
							ResetMouseSlot();
						}
					}
				}
			}
			if(mousestate->bButton[0])
			{
				BOOL clickedwindowregion = CheckMousePosition();

				if(g_pLocalUser->m_bMatchPRMode)
				{
					if( clickedwindowregion )
					{
						g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_QSLOT_2570), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
					}
				} 
				else 
				{
					// left button down
					if(CheckMousePosition())
					{
						if((scrnpos->x > m_WindowPosition.x+6 && scrnpos->x < m_WindowPosition.x+205 &&
							scrnpos->z > m_WindowPosition.y+2 && scrnpos->z < m_WindowPosition.y+39) ||

							(scrnpos->x > m_WindowPosition.x+255 && scrnpos->x < m_WindowPosition.x+464 &&
							scrnpos->z > m_WindowPosition.y+2 && scrnpos->z < m_WindowPosition.y+39))
						{
							pfocusobject = _XDEF_DUMMYID_QUICKSLOT;
						}

						if(g_pInventory_Window)
						{
							// item 등록
							if(g_pInventory_Window->m_bHaveMouseSlot)
							{
								m_LastDraggingSlot = SearchSlotIndex(scrnpos->x, scrnpos->z);
								if(m_LastDraggingSlot > -1)
								{
									RegistItem();
									g_pInventory_Window->m_bHaveMouseSlot = FALSE;
									g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();

									bRegist = true;
								}
							}
						}
						if(g_pSkill_Window)
						{
							if(g_pSkill_Window->GetHaveMouseSlot())
							{
								m_LastDraggingSlot = SearchSlotIndex(scrnpos->x, scrnpos->z);
								if(m_LastDraggingSlot > -1)
								{
									RegistSkill();
									g_pSkill_Window->ResetMouseSlot();
									
									bRegist = true;
								}
							}
						}
						if(g_pLifeSkill_Window)
						{
							if(g_pLifeSkill_Window->GetHaveMouseSlot())
							{
								m_LastDraggingSlot = SearchSlotIndex(scrnpos->x, scrnpos->z);
								if(m_LastDraggingSlot > -1)
								{
									RegistLifeSkill();
									g_pLifeSkill_Window->ResetMouseSlot();
									
									bRegist = true;
								}
							}
						}
						if(!bRegist)
						{
							if(!m_bHaveMouseSlot)
							{
								// Mouse down 위치 저장
								m_IconDragStartPos.x = scrnpos->x;
								m_IconDragStartPos.y = scrnpos->z;
								m_LastDraggingSlot = SearchSlotIndex(scrnpos->x, scrnpos->z);
								m_bDragState = true;
							}
							else
							{

								// click한 위치 저장
								m_LastDraggingSlot = SearchSlotIndex(scrnpos->x, scrnpos->z);
							}
						}
					} // if(CheckMousePosition())
					else
					{
						if(m_bHaveMouseSlot)
						{
							DeleteQuickSlotFromMouse();
							ResetMouseSlot();
							m_bHaveMouseSlot = false;
							m_LastDraggingSlot = -1;
						}
					}
				} // check 비무홍보중...
			}
		} // if(!pfocusobject)
		if(pfocusobject == _XDEF_DUMMYID_QUICKSLOT)
		{
			if(mousestate->bButton[0])
			{
				// 드래그 처리
				if(m_bDragState && !m_bHaveMouseSlot)
				{
					if(scrnpos->x != m_IconDragStartPos.x || scrnpos->z != m_IconDragStartPos.y)
					{
						if(CopyMouseSlot(m_LastDraggingSlot))
						{
							// drag할 icon이 있음
							m_bHaveMouseSlot = true;
							m_LastDraggingSlot = -1;
						}
					}
				}  // if(m_bDragState || !m_bHaveMouseSlot)
			}

			if(!mousestate->bButton[0])
			{
				m_bDrawSelectedIcon = true;

				if(scrnpos->x == m_IconDragStartPos.x && scrnpos->z == m_IconDragStartPos.y)
				{
					// 제자리에서 클릭 - 선택
					int index = m_SelectedSet*10 + m_LastDraggingSlot;
					if(index >= 0 && index < 30)
					{
						m_bDragState = false;
						if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
						{
							if(m_QuickSlot[index].bChangeItem)
								g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//지금은 사용할 수 없습니다.
							else if(m_LastDraggingSlot > -1 && m_LastDraggingSlot < 10)
								PostMessage(gHWnd, WM_COMMAND, _XDEF_QS_SHORTCUT1BUTTON+m_LastDraggingSlot, m_QuickSlot[index].skillid);
						}
					}
				}
				else
				{
					if(m_bDragState && m_bHaveMouseSlot)
					{
						m_bDragState = false;
					}
				}

				if(m_bHaveMouseSlot)
				{
					ProcessIconMove();
				}
			}
		} // if(pfocusobject == _XDEF_DUMMYID_QUICKSLOT)

		if(!mousestate->bButton[1])
		{
			if(m_WindowMouseState == _XW_STATE_RIGHTCLICK)
			{
				if(m_bHaveMouseSlot)
				{
					ResetMouseSlot();
					m_bHaveMouseSlot = false;
					m_LastDraggingSlot = -1;
				}
				else
				{
					// icon 지우기 //Author : 양희왕  breif : 제거 // 기공 딜레이 단축 버그 .. 
					//DeleteSelectedSkill();
//					DeleteQuickSlot();
				}
				m_WindowMouseState = _XW_STATE_NONE;
			}
		}

		if(!mousestate->bButton[0])
		{
			if(pfocusobject == _XDEF_DUMMYID_QUICKSLOT)
				pfocusobject = NULL;

			if(CheckMousePosition())
			{
				for(int i = 0 ; i < 10 ; i++)
				{
					if(scrnpos->x > m_WindowPosition.x + m_PositionArray[i].x &&
						scrnpos->x < m_WindowPosition.x + m_PositionArray[i].x + 36 &&
						scrnpos->z > m_WindowPosition.y + m_PositionArray[i].y &&
						scrnpos->z < m_WindowPosition.y + m_PositionArray[i].y + 45)
					{
						if(m_SelectedSet*10+i > -1 && m_SelectedSet*10+i < 30)
						{
							if(m_QuickSlot[m_SelectedSet*10+i].used && m_QuickSlot[m_SelectedSet*10+i].type == _XQS_SLOTTYPE_SKILL && m_LevelButtonIndex != i)
							{
								short skillid = m_QuickSlot[m_SelectedSet*10+i].skillid;
								if(g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL 
									&& _XSkillItem::GetTargetType(skillid)/*g_SkillProperty[skillid]->targetType*/ == _XSI_TARGETTYPE_SELF
									&& g_SkillProperty[skillid]->skillSType == _XSI_STYPE_03)
								{
									// 경공일때는 Level up/down 버튼 보이지 않음
								}
								else
								{
									if(m_LeftLevelButton && m_RightLevelButton)
									{
										m_LeftLevelButton->MoveWindow(m_WindowPosition.x+m_PositionArray[i].x + 4, m_WindowPosition.y+m_PositionArray[i].y + 38);
										m_RightLevelButton->MoveWindow(m_WindowPosition.x+m_PositionArray[i].x + 27, m_WindowPosition.y+m_PositionArray[i].y + 38);
										m_LeftLevelButton->ShowWindow(TRUE);
										m_RightLevelButton->ShowWindow(TRUE);
										m_bDrawLevelButton = true;
										m_LevelButtonIndex = i;
									}
									break;
								}
							}
							else
								break;
						}
						else
							break;
					}
				}
				if(i == 10)
				{
					if(m_bDrawLevelButton)
					{
						if(m_LeftLevelButton && m_RightLevelButton)
						{
							m_LeftLevelButton->ShowWindow(FALSE);
							m_RightLevelButton->ShowWindow(FALSE);
						}
						m_bDrawLevelButton = false;
						m_LevelButtonIndex = -1;
					}
				}
			} // if(CheckMousePosition())
			else
			{
				if(m_bDrawLevelButton)
				{
					if(m_LeftLevelButton && m_RightLevelButton)
					{
						m_LeftLevelButton->ShowWindow(FALSE);
						m_RightLevelButton->ShowWindow(FALSE);
					}
					m_bDrawLevelButton = false;
					m_LevelButtonIndex = -1;
				}
			}
		}
	}

	ProcessCooldown();
	ProcessCooldownLastImage();
	ProcessDelayTime();

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_QuickSlot::ChangePeaceBattleMode(void)
{
	if(g_pLocalUser->m_BattleMode == _XBATTLE_PEACE)
	{
		if(m_PeaceModeButton && m_BattleModeButton)
		{
			if(!m_PeaceModeButton->GetShowStatus())
			{
				m_BattleModeButton->ShowWindow(FALSE);
				m_PeaceModeButton->ShowWindow(TRUE);
			}
		}
	}
	else if(g_pLocalUser->m_BattleMode == _XBATTLE_BATTLE)
	{
		if(m_PeaceModeButton && m_BattleModeButton)
		{
			if(!m_BattleModeButton->GetShowStatus())
			{
				m_PeaceModeButton->ShowWindow(FALSE);
				m_BattleModeButton->ShowWindow(TRUE);
			}
		}
	}
}

BOOL _XWindow_QuickSlot::GetIconRect(short skillid, int& resourceindex, RECT& rect, bool bColor, bool smallicon, bool bNPCSkill)
{
	int iconsubid = 0;	
	if(skillid > 0)
		iconsubid = skillid - (g_SkillProperty[skillid]->clanType * 1000);
	else
		return FALSE;

	if(!bNPCSkill)
	{
		int learnedskillindex = g_pLocalUser->GetLearnedSkillIndex(skillid);
		if(learnedskillindex >= 0)
		{
			if(g_pLocalUser->m_SkillList[learnedskillindex].m_ucSkillState == 1)
				bColor = FALSE;
		}
	}

	if(!bColor)
	{
		return GetIconRectTrainingStateIcon(skillid, resourceindex, rect, smallicon);
	}
	
	int subindex = (iconsubid-1) / 64;
	int iconid = (iconsubid-1) % 64;

	if(iconid < 0)
		return FALSE;

	rect.left = ((iconid) % 8) * 32;
	rect.right = ((iconid ) % 8 + 1) * 32;
	rect.top = ((iconid) / 8) * 32;
	rect.bottom = ((iconid) / 8 + 1) *32;
	
	if(smallicon)
	{
		rect.left  += 1;
		rect.top   += 1;
		rect.right -= 1;
		rect.bottom-= 1;
	}

	switch(g_SkillProperty[skillid]->clanType)
	{
	case _XGROUP_NANGIN :
		{
			TCHAR resourcename[32];
			sprintf(resourcename, "skill_icon_00_%02d.tga", subindex);
			resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
		}
		break;
	case _XGROUP_GAEBANG :
		{
			TCHAR resourcename[32];
			sprintf(resourcename, "skill_icon_01_%02d.tga", subindex);
			resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
		}
		break;
	case _XGROUP_BEEGOONG :
		{
			TCHAR resourcename[32];
			sprintf(resourcename, "skill_icon_02_%02d.tga", subindex);
			resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
		}
		break;
	case _XGROUP_SORIM :
		{
			TCHAR resourcename[32];
			sprintf(resourcename, "skill_icon_03_%02d.tga", subindex);
			resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
		}
		break;
	case _XGROUP_NOCKRIM :
		{
			TCHAR resourcename[32];
			sprintf(resourcename, "skill_icon_04_%02d.tga", subindex);
			resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
		}
		break;
	case _XGROUP_MOODANG :
		{
			TCHAR resourcename[32];
			sprintf(resourcename, "skill_icon_05_%02d.tga", subindex);
			resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
		}
		break;
	case _XGROUP_MAKYO :
		{
			TCHAR resourcename[32];
			sprintf(resourcename, "skill_icon_06_%02d.tga", subindex);
			resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
		}
		break;
	// 이벤트 용
	case _XGROUP_NOFIXED :
		{
			TCHAR resourcename[32];
			sprintf(resourcename, "skill_icon_08_%02d.tga", subindex);
			resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);
		}
		break;
	}
	
	if(resourceindex == -1)
	{
		// 없는 아이콘
		resourceindex = g_MainInterfaceTextureArchive.FindResource("item_pot_w1_001.tga");
		rect.left = 129;
		rect.top = 1;
		rect.right = 151;
		rect.bottom = 30;
	}

	return TRUE;
}

BOOL _XWindow_QuickSlot::GetIconRectTrainingStateIcon(short skillid, int& resourceindex, RECT& rect, bool smallicon)
{
	int iconid = 0;	

	if(g_SkillProperty[skillid]->clanType == _XGROUP_NANGIN)
	{
		if(skillid > 0)
			iconid = skillid - 1;
		else
			return FALSE;
		
		resourceindex = g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_00.tga");
	}
	else if(g_SkillProperty[skillid]->clanType >= _XGROUP_GAEBANG && g_SkillProperty[skillid]->clanType <= _XGROUP_NOCKRIM)
	{
		if(skillid > 0)
			iconid = g_SkillProperty[skillid]->skillSType - 1;
		else
			return FALSE;

		resourceindex = g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_01.tga");
	}
	else if(g_SkillProperty[skillid]->clanType >= _XGROUP_MOODANG && g_SkillProperty[skillid]->clanType <= _XGROUP_MAKYO)
	{
		if(skillid > 0)
			iconid = g_SkillProperty[skillid]->skillSType - 1;
		else
			return FALSE;
		
		resourceindex = g_MainInterfaceTextureArchive.FindResource("Skill_Icon_g_02.tga");
	}
	
	if(iconid < 0)
		return FALSE;

	rect.left = ((iconid) % 8) * 32;
	rect.right = ((iconid) % 8 + 1) * 32;
	rect.top = ((iconid) / 8) * 32;
	rect.bottom = ((iconid) / 8 + 1) *32;

	switch(g_SkillProperty[skillid]->clanType)
	{
	case _XGROUP_GAEBANG :
		{
		}
		break;
	case _XGROUP_BEEGOONG :
		{
			rect.top += 64;
			rect.bottom += 64;
		}
		break;
	case _XGROUP_SORIM :
		{
			rect.top += 128;
			rect.bottom += 128;
		}
		break;
	case _XGROUP_NOCKRIM :
		{
			rect.top += 192;
			rect.bottom += 192;
		}
		break;
	case _XGROUP_MOODANG :
		{
		}
		break;
	case _XGROUP_MAKYO :
		{
			rect.top += 64;
			rect.bottom += 64;
		}
		break;
	}

	if(smallicon)
	{
		rect.left  += 1;
		rect.top   += 1;
		rect.right -= 1;
		rect.bottom-= 1;
	}

	return TRUE;
}

void _XWindow_QuickSlot::DrawSkillIcon(short skillid, int X, int Y, bool bColor)
{
	int		iconimageindex = -1;
	RECT	imagerect;

	if(GetIconRect(skillid, iconimageindex, imagerect, bColor))
	{
		g_SkillIconImage.SetTexture(&g_MainInterfaceTextureArchive, iconimageindex);
		g_SkillIconImage.DrawWithRegion(X, Y, imagerect);
	}
}

int _XWindow_QuickSlot::SearchSlotIndex(int X, int Y)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		if(X > m_WindowPosition.x+m_PositionArray[i].x && X < m_WindowPosition.x+m_PositionArray[i].x+32)
		{
			if(Y > m_WindowPosition.y+m_PositionArray[i].y && Y < m_WindowPosition.y+m_PositionArray[i].y+32)
			{
				return i;
			}
		}
	}

	return -1;
}

void _XWindow_QuickSlot::GetSlotPosition( int slotnumber, RECT& rect )
{
	if( slotnumber > -1 && slotnumber < 10 )
	{
		rect.left = m_WindowPosition.x + m_PositionArray[slotnumber].x;
		rect.top = m_WindowPosition.y + m_PositionArray[slotnumber].y;
		rect.right = m_WindowPosition.x + m_PositionArray[slotnumber].x + 32;
		rect.bottom = m_WindowPosition.y + m_PositionArray[slotnumber].y + 32;
	}
}

bool _XWindow_QuickSlot::CopyMouseSlot(int draggingslot)
{
	if(draggingslot == -1)
		return false;

	int index = m_SelectedSet*10 + draggingslot;
	if(index < 0 || index >= 30)
		return false;

	if(m_QuickSlot[index].used)
	{
		if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
		{
			m_MouseSlotType = _XQS_SLOTTYPE_SKILL;
			m_MouseSlotSkillID = m_QuickSlot[index].skillid;
			m_OriginSlotIndex = draggingslot;
			return true;
		}
		else if(m_QuickSlot[index].type == _XQS_SLOTTYPE_ITEM)
		{
			m_MouseSlotType = _XQS_SLOTTYPE_ITEM;
			m_MouseSlotItem.Set_m_cType(m_QuickSlot[index].itemCType);
			m_MouseSlotItem.Set_m_sID( m_QuickSlot[index].itemSID );
			m_MouseSlotItem.m_InventoryNumber = m_QuickSlot[index].invenNumber;
			m_OriginSlotIndex = draggingslot;
			return true;
		}
		else if(m_QuickSlot[index].type == _XQS_SLOTTYPE_LIFESKILL)
		{
			m_MouseSlotType = _XQS_SLOTTYPE_LIFESKILL;
			m_MouseSlotItem.Set_m_sID(m_QuickSlot[index].lifeskillid);
			m_OriginSlotIndex = draggingslot;
			return true;
		}

	}

	return false;
}

void _XWindow_QuickSlot::ResetMouseSlot(void)
{
	m_MouseSlotType = _XQS_SLOTTYPE_NONE;
	m_MouseSlotSkillID = 0;
	m_MouseSlotItem.ResetMouseSlotItem();
	m_OriginSlotIndex = -1;
	m_bHaveMouseSlot = false;

	m_bDrawSelectedIcon = false;
}

void _XWindow_QuickSlot::ProcessIconMove(void)
{
	if(m_LastDraggingSlot == -1)
		return;

	ResetQuickSlot(m_OriginSlotIndex);

	int index = m_SelectedSet*10 + m_LastDraggingSlot;
	if(index < 0 || index >= 30)
		return;

	if(m_MouseSlotType == _XQS_SLOTTYPE_SKILL)
	{
		m_QuickSlot[index].type = _XQS_SLOTTYPE_SKILL;
		m_QuickSlot[index].skillid = m_MouseSlotSkillID;
		m_QuickSlot[index].used = true;

#ifdef _XDEF_QUICKSLOTSERVERSAVE
		g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 1, index );
#endif
		ResetMouseSlot();
	}
	else if(m_MouseSlotType == _XQS_SLOTTYPE_ITEM)
	{
		m_QuickSlot[index].type = _XQS_SLOTTYPE_ITEM;
		m_QuickSlot[index].itemCType = m_MouseSlotItem.Get_m_cType();
		m_QuickSlot[index].itemSID = m_MouseSlotItem.Get_m_sID();
		m_QuickSlot[index].invenNumber = m_MouseSlotItem.m_InventoryNumber;
		m_QuickSlot[index].used = true;

#ifdef _XDEF_QUICKSLOTSERVERSAVE
		g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 1, index );
#endif
		ResetMouseSlot();
	}
	else if(m_MouseSlotType == _XQS_SLOTTYPE_LIFESKILL)
	{
		m_QuickSlot[index].type = _XQS_SLOTTYPE_LIFESKILL;
		m_QuickSlot[index].lifeskillid = m_MouseSlotItem.Get_m_sID();
		m_QuickSlot[index].used = true;

#ifdef _XDEF_QUICKSLOTSERVERSAVE
		g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 1, index );
#endif		
		ResetMouseSlot();
	}
}

void _XWindow_QuickSlot::RegistItem(void)
{
	if(g_pInventory_Window->m_MouseSlotItem.Get_m_cType() == _XGI_FC_POTION || 
		/*g_pInventory_Window->m_MouseSlotItem.Get_m_cType() == _XGI_FC_BOOK || */
		g_pInventory_Window->m_MouseSlotItem.Get_m_cType() == _XGI_FC_LIFE ||
		g_pInventory_Window->m_MouseSlotItem.Get_m_cType() == _XGI_FC_CONSUMING )
	{
		if( g_pInventory_Window->m_MouseSlotItem.Get_m_cType() == _XGI_FC_POTION && 
			g_pInventory_Window->m_MouseSlotItem.Get_m_cSecond() == _XGI_SC_POTION_HWATA )			
			return;
		int index = m_SelectedSet*10 + m_LastDraggingSlot;
		if(index < 0 || index >= 30)
			return;

		m_QuickSlot[index].type = _XQS_SLOTTYPE_ITEM;
		m_QuickSlot[index].itemCType = g_pInventory_Window->m_MouseSlotItem.Get_m_cType();
		m_QuickSlot[index].itemSID = g_pInventory_Window->m_MouseSlotItem.Get_m_sID();
		m_QuickSlot[index].invenNumber = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
		m_QuickSlot[index].used = true;

#ifdef _XDEF_QUICKSLOTSERVERSAVE
		g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 1, index );
#endif
	}
}

void _XWindow_QuickSlot::RegistSkill(void)
{
	CheckTutorial(g_pSkill_Window->GetMouseSlotSkillID());
	
	int index = m_SelectedSet*10 + m_LastDraggingSlot;
	if(index < 0 || index >= 30)
		return;

	m_QuickSlot[index].type = _XQS_SLOTTYPE_SKILL;
	m_QuickSlot[index].skillid = g_pSkill_Window->GetMouseSlotSkillID();
	m_QuickSlot[index].used = true;

#ifdef _XDEF_QUICKSLOTSERVERSAVE
	g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 1, index );
#endif

	_XPlayInterfaceSound(ID_SR_INTERFACE_SKILL_DOWN_WAV);
}

void _XWindow_QuickSlot::RegistLifeSkill(void)
{
	int index = m_SelectedSet*10 + m_LastDraggingSlot;
	if(index < 0 || index >= 30)
		return;
	
	m_QuickSlot[index].type = _XQS_SLOTTYPE_LIFESKILL;
	m_QuickSlot[index].lifeskillid = g_pLifeSkill_Window->GetMouseSlotLifeSkillID();
	m_QuickSlot[index].used = true;		

#ifdef _XDEF_QUICKSLOTSERVERSAVE
	g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 1, index );
#endif
}

void _XWindow_QuickSlot::RegistLifeSkillNextEmptySlot(short skillid)
{
	int index = 0;
	for(int i = 0 ; i < 10 ; i++)
	{
		index = m_SelectedSet*10 + i;
		if(index < 0 || index >= 30)
			return;

		if(!m_QuickSlot[index].used)
		{
			m_QuickSlot[index].type = _XQS_SLOTTYPE_LIFESKILL;
			m_QuickSlot[index].lifeskillid = skillid;
			m_QuickSlot[index].used = true;					
			_XPlayInterfaceSound(ID_SR_INTERFACE_SKILL_DOWN_WAV);

#ifdef _XDEF_QUICKSLOTSERVERSAVE
			g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 1, index );
#endif
			return;
		}
	}
}

BOOL _XWindow_QuickSlot::FindQuickSlotItem(_XQSlot_Type type, short id)
{
	int index = 0;
	for( int i = 0; i < 10; i++ )
	{
		index = m_SelectedSet*10+i;
		if( index < 0 || index >= 30 )
			return FALSE;

		if( m_QuickSlot[index].used )
		{
			if( m_QuickSlot[index].type == type )
			{
				switch(type) 
				{
				case _XQS_SLOTTYPE_SKILL:
					{
						if( m_QuickSlot[index].skillid == id )
							return TRUE;
					}
					break;
				case _XQS_SLOTTYPE_ITEM:
					{
						if( m_QuickSlot[index].itemSID == id )
							return TRUE;
					}
					break;
				case _XQS_SLOTTYPE_LIFESKILL:
					{
						if( m_QuickSlot[index].lifeskillid == id )
							return TRUE;
					}
					break;
				}				
			}
		}		
	}	
	return FALSE;
}

void _XWindow_QuickSlot::DrawMouseSlotIcon(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(m_MouseSlotType == _XQS_SLOTTYPE_SKILL)
	{
		DrawSkillIcon(m_MouseSlotSkillID, scrnpos->x-16, scrnpos->z-16);
	}
	else if(m_MouseSlotType == _XQS_SLOTTYPE_ITEM)
	{
		g_pInventory_Window->DrawIcon(TRUE, scrnpos->x-16, scrnpos->z-16, &m_MouseSlotItem);
	}
	else if(m_MouseSlotType == _XQS_SLOTTYPE_LIFESKILL)
	{
		if(g_pLifeSkill_Window)
		{
			if( m_MouseSlotItem.Get_m_sID() == 1)
			{
				g_pLifeSkill_Window->m_imagePersonalStoreIcon->Draw(scrnpos->x-16, scrnpos->z-16);
			}
			else if(m_MouseSlotItem.Get_m_sID() == 2)
			{
				g_pLifeSkill_Window->m_imagePVPMark->Draw(scrnpos->x-16, scrnpos->z-16);
			}
		}
	}
}

void _XWindow_QuickSlot::ResetQuickSlot(int index)
{
	index += m_SelectedSet*10;
	if(index < 0 || index >= 30)
		return;

	memset(&m_QuickSlot[index], 0, sizeof(_XQSlot_Info));

#ifdef _XDEF_QUICKSLOTSERVERSAVE
	g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 0, index );
#endif
}

void _XWindow_QuickSlot::ResetQuickSlotCurrentSet(void)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		if(m_QuickSlot[i].type == _XQS_SLOTTYPE_SKILL)
		{
			if(g_pLocalUser->m_SelectedSkillID == m_QuickSlot[i].skillid)
			{
				if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
				{
					g_pLocalUser->m_SelectedSkillID = 0;
					g_pLocalUser->m_SelectedSkillItem = NULL;
					
					g_pLocalUser->UpdateLocalUserAttackLevel();
					
					g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
					g_NetworkKernel.SendPacket(MSG_NO_STANCE_MOTION_CHANGE, 0);
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_QSLOT_ERRORDELETE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "지금은 삭제할 수 없습니다."
					return;
				}
			}
			if(g_pLocalUser->m_ActiveSlotID == m_QuickSlot[i].skillid)
			{
				g_pLocalUser->m_ActiveSlotID = 0;
			}
		}
		
		if(i == m_CurrentSelectedActiveIndex)
			m_CurrentSelectedActiveIndex = -1;
		if(i == m_CurrentSelectedOtherIndex)
			m_CurrentSelectedOtherIndex = -1;
		if(i == m_CurrentSelectedItemIndex)
			m_CurrentSelectedItemIndex = -1;

		ResetQuickSlot(i);
	}
}

void _XWindow_QuickSlot::DeleteQuickSlot(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int index = SearchSlotIndex(scrnpos->x, scrnpos->z);
	if(index < 0 || index >= 30)
		return;

	if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
	{
		if(g_pLocalUser->m_SelectedSkillID == m_QuickSlot[index].skillid)
		{
			if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
			{
				g_pLocalUser->m_SelectedSkillID = 0;
				g_pLocalUser->m_SelectedSkillItem = NULL;
				
				g_pLocalUser->UpdateLocalUserAttackLevel();
				
				g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
				g_NetworkKernel.SendPacket(MSG_NO_STANCE_MOTION_CHANGE, 0);
			}
			else
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_QSLOT_ERRORDELETE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "지금은 삭제할 수 없습니다."
				return;
			}
		}
		if(g_pLocalUser->m_ActiveSlotID == m_QuickSlot[index].skillid)
		{
			g_pLocalUser->m_ActiveSlotID = 0;
		}
	}

	if(index == m_CurrentSelectedActiveIndex)
		m_CurrentSelectedActiveIndex = -1;
	if(index == m_CurrentSelectedOtherIndex)
		m_CurrentSelectedOtherIndex = -1;
	if(index == m_CurrentSelectedItemIndex)
		m_CurrentSelectedItemIndex = -1;

	ResetQuickSlot(index);
}

void _XWindow_QuickSlot::DeleteSelectedSkill(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	int index = SearchSlotIndex(scrnpos->x, scrnpos->z);
	if(index < 0 || index >= 30)
		return;
	
	if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
	{
		if(g_pLocalUser->m_SelectedSkillID == m_QuickSlot[index].skillid)
		{
			if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
			{
				g_pLocalUser->m_SelectedSkillID = 0;
				g_pLocalUser->m_SelectedSkillItem = NULL;
				
				g_pLocalUser->UpdateLocalUserAttackLevel();
				
				g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
				g_NetworkKernel.SendPacket(MSG_NO_STANCE_MOTION_CHANGE, 0);
			}
			else
			{
				g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_QSLOT_ERRORSELECT), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "지금은 선택 해제할 수 없습니다."
				return;
			}
		}
		if(g_pLocalUser->m_ActiveSlotID == m_QuickSlot[index].skillid)
		{
			g_pLocalUser->m_ActiveSlotID = 0;
		}
	}
	
	if(index == m_CurrentSelectedActiveIndex)
		m_CurrentSelectedActiveIndex = -1;
	if(index == m_CurrentSelectedOtherIndex)
		m_CurrentSelectedOtherIndex = -1;
	if(index == m_CurrentSelectedItemIndex)
		m_CurrentSelectedItemIndex = -1;
}

void _XWindow_QuickSlot::DeleteQuickSlotFromMouse(void)
{
	if(m_MouseSlotType == _XQS_SLOTTYPE_SKILL)
	{
		int index = m_OriginSlotIndex;
		if(index < 0 || index >= 30)
			return;

		if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
		{
			if(g_pLocalUser->m_SelectedSkillID == m_QuickSlot[index].skillid)
			{
				if(g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_NONE || g_pLocalUser->m_CurrentBattleState == _XUSER_STATE_LAST)
				{
					g_pLocalUser->m_SelectedSkillID = 0;
					g_pLocalUser->m_SelectedSkillItem = NULL;
					
					g_pLocalUser->UpdateLocalUserAttackLevel();
					
					g_NetworkKernel.SendPacket(MSG_NO_SKILL_SELECT);
					g_NetworkKernel.SendPacket(MSG_NO_STANCE_MOTION_CHANGE, 0);
				}
				else
				{
					g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_QSLOT_ERRORDELETE), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	// "지금은 삭제할 수 없습니다."
					return;
				}
			}
			if(g_pLocalUser->m_ActiveSlotID == m_QuickSlot[index].skillid)
			{
				g_pLocalUser->m_ActiveSlotID = 0;
			}
		}
		
		if(index == m_CurrentSelectedActiveIndex)
			m_CurrentSelectedActiveIndex = -1;
		if(index == m_CurrentSelectedOtherIndex)
			m_CurrentSelectedOtherIndex = -1;
		if(index == m_CurrentSelectedItemIndex)
			m_CurrentSelectedItemIndex = -1;
		
		ResetQuickSlot(index);
	}
	else if( m_MouseSlotType == _XQS_SLOTTYPE_LIFESKILL )
	{
		ResetQuickSlot(m_OriginSlotIndex);
	}
	else if(m_MouseSlotType == _XQS_SLOTTYPE_ITEM)
	{		
		if(m_QuickSlot[m_OriginSlotIndex].type == _XQS_SLOTTYPE_ITEM)
		{
			ResetQuickSlot(m_OriginSlotIndex);
		}
	}
}

void _XWindow_QuickSlot::RegistSkillNextEmptySlot(short skillid)
{
	CheckTutorial(skillid);

	int index = 0;
	for(int i = 0 ; i < 10 ; i++)
	{
		index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(!m_QuickSlot[index].used)
			{
				m_QuickSlot[index].type = _XQS_SLOTTYPE_SKILL;
				m_QuickSlot[index].skillid = skillid;
				m_QuickSlot[index].used = true;

#ifdef _XDEF_QUICKSLOTSERVERSAVE
				g_NetworkKernel.SendPacket( MSG_NO_QUICKSLOT, 1, index );
#endif

				_XPlayInterfaceSound(ID_SR_INTERFACE_SKILL_DOWN_WAV);
				return;
			}
		}
	}
}

void _XWindow_QuickSlot::DrawSkillExpGauge(void)
{
	int		index = 0, learnedskillindex = 0;
	FLOAT	exp = 0.0f, gaugefactor = 0.0f;

	for(int i = 0 ; i < 10 ; i++)
	{
		index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(m_QuickSlot[index].used)
			{
				if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
				{
					learnedskillindex = g_pLocalUser->GetLearnedSkillIndex(m_QuickSlot[index].skillid, TRUE);

					_XUserSkill* pSkill = NULL;
					if(learnedskillindex == _XDEF_MAX_LEARNSKILL)
					{
						pSkill = &g_pLocalUser->m_EventSkillList;
					}
					else
					{
						pSkill = g_pLocalUser->GetSkillItem(m_QuickSlot[index].skillid);
					}


					if(pSkill)
					{
						if(pSkill->m_cSkillLevel >= 11)
							exp = (FLOAT)g_SkillProperty[m_QuickSlot[index].skillid]->detailInfo[pSkill->m_cSkillLevel].skillExp;
						else
							exp = (FLOAT)g_SkillProperty[m_QuickSlot[index].skillid]->detailInfo[pSkill->m_cSkillLevel+1].skillExp;

						if(exp > 0)
							gaugefactor = ((FLOAT)pSkill->m_iSkillExp / exp) * 100;
						else
							gaugefactor = 0.0f;

						m_pSkillExpGauge->MoveWindow(m_WindowPosition.x+m_PositionArray[i].x-1, m_WindowPosition.y+m_PositionArray[i].y+38);
						m_pSkillExpGauge->SetGaugeFactor(gaugefactor);
						m_pSkillExpGauge->Draw();
					}
				}
			}
		}
	}
}

_XQSlot_Type _XWindow_QuickSlot::GetQuickSlotType(int index)
{
	int slotindex = m_SelectedSet*10 + index;
	if(slotindex < 0 || slotindex >= 30)
		return _XQS_SLOTTYPE_NONE;

	if(m_QuickSlot[slotindex].used)
	{
		if(m_QuickSlot[slotindex].bChangeItem)
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	//지금은 사용할 수 없습니다.		
			return _XQS_SLOTTYPE_NONE;
		}
		else
		{
			return m_QuickSlot[slotindex].type;
		}
	}
	else
		return _XQS_SLOTTYPE_NONE;
}

short _XWindow_QuickSlot::GetQuickSlotSkillID(int index)
{
	int slotindex = m_SelectedSet*10 + index;
	return m_QuickSlot[slotindex].skillid;
}

int _XWindow_QuickSlot::GetQuickSlotItemInvenNumber(int index)
{
	int slotindex = m_SelectedSet*10 + index;
	return m_QuickSlot[slotindex].invenNumber;
}

void _XWindow_QuickSlot::DrawTooltip(void)
{
 	if( g_bShowItemMallWindow ) return;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	bool flag = false;

	for(int i = 0 ; i < 10 ; i++)
	{
		int index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(m_QuickSlot[index].used)
			{
				if(scrnpos->x > m_WindowPosition.x+m_PositionArray[i].x &&
					scrnpos->x < m_WindowPosition.x+m_PositionArray[i].x+32 &&
					scrnpos->z > m_WindowPosition.y+m_PositionArray[i].y &&
					scrnpos->z < m_WindowPosition.y+m_PositionArray[i].y+32)
				{
					if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
					{
						flag = true;					
						if( g_pMiniTooltip_Window )
						{
							if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_SKILL, i, 0, m_QuickSlot[index].skillid ) )
								g_pMiniTooltip_Window->ShowWindow( TRUE );
						}					
					}
					else if(m_QuickSlot[index].type == _XQS_SLOTTYPE_ITEM)
					{
						flag = true;					
						if( g_pMiniTooltip_Window )
						{
							if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_ITEM, i, m_QuickSlot[index].itemCType, m_QuickSlot[index].itemSID ) )
								g_pMiniTooltip_Window->ShowWindow( TRUE );
						}					
					}
					else if(m_QuickSlot[index].type == _XQS_SLOTTYPE_LIFESKILL)
					{
						if( m_QuickSlot[index].lifeskillid == 1)
						{
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+m_PositionArray[i].x+4, m_WindowPosition.y+m_PositionArray[i].y-15, 
																_XGETINTERFACETEXT(ID_STRING_LIFESKILL_STORE), _XSC_INFORMATION ); // _T("노점")
						}
						else if( m_QuickSlot[index].lifeskillid == 2 )
						{
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+m_PositionArray[i].x+4, m_WindowPosition.y+m_PositionArray[i].y-15, 
																_XGETINTERFACETEXT(ID_STRING_MATCH_2189), _XSC_INFORMATION ); //_T("비무홍보")
						}
						else if( m_QuickSlot[index].lifeskillid == 3 )
						{
							g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+m_PositionArray[i].x+4, m_WindowPosition.y+m_PositionArray[i].y-15, 
								_XGETINTERFACETEXT(ID_STRING_NEW_3046), _XSC_INFORMATION ); //_T("비무홍보")
						}
					}
				}
			}
			else
			{
				if(scrnpos->x > m_WindowPosition.x+m_PositionArray[i].x &&
					scrnpos->x < m_WindowPosition.x+m_PositionArray[i].x+32 &&
					scrnpos->z > m_WindowPosition.y+m_PositionArray[i].y &&
					scrnpos->z < m_WindowPosition.y+m_PositionArray[i].y+32)
				{
					TCHAR tempstr[128];
					sprintf( tempstr, _XGETINTERFACETEXT(ID_STRING_QSLOT_EMPTY), i+1 );	// "비어있음[%d]"
					g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x+m_PositionArray[i].x+4, m_WindowPosition.y+m_PositionArray[i].y-15, tempstr, _XSC_INFORMATION );
				}
			}
		}
	}

	if( flag == false)
	{
		if( g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_SKILL || g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_ITEM )
			g_pMiniTooltip_Window->ShowWindow( FALSE );
	}
}

void _XWindow_QuickSlot::CheckQuickSlotItem(void)
{
	for(int i = 0 ; i < 30 ; i++)
	{
		if(m_QuickSlot[i].used)
		{
			if(m_QuickSlot[i].type == _XQS_SLOTTYPE_ITEM)
			{
				if(g_pLocalUser->m_UserItemList[m_QuickSlot[i].invenNumber].Get_m_cType() != m_QuickSlot[i].itemCType ||
					g_pLocalUser->m_UserItemList[m_QuickSlot[i].invenNumber].Get_m_sID() != m_QuickSlot[i].itemSID)
					memset(&m_QuickSlot[i], 0, sizeof(_XQSlot_Info));
			}
		}
	}
}

void _XWindow_QuickSlot::CheckQuickSlotSkill(void)
{
	for(int i = 0 ; i < 30 ; i ++)
	{
		if(m_QuickSlot[i].used)
		{
			if(m_QuickSlot[i].type == _XQS_SLOTTYPE_SKILL)
			{
				if(_XSkillItem::FindSkillProperty(m_QuickSlot[i].skillid))
				{
					if(g_pLocalUser->GetLearnedSkillIndex(m_QuickSlot[i].skillid, TRUE) == -1)
						memset(&m_QuickSlot[i], 0, sizeof(_XQSlot_Info));
				}
				else
				{
					memset(&m_QuickSlot[i], 0, sizeof(_XQSlot_Info));
				}
			}
		}
	}
}

void _XWindow_QuickSlot::DeleteQuickSlot(short skillid)
{
	for(int i = 0 ; i < 30 ; i++)
	{
		if(m_QuickSlot[i].used)
		{
			if(m_QuickSlot[i].type == _XQS_SLOTTYPE_SKILL)
			{
				if(m_QuickSlot[i].skillid == skillid)
				{
					memset(&m_QuickSlot[i], 0, sizeof(_XQSlot_Info));
					return;
				}
			}
		}
	}
}

void _XWindow_QuickSlot::DeleteQuickSlot(u_char invennumber)
{
	for(int i = 0 ; i < 30 ; i++)
	{
		if(m_QuickSlot[i].used)
		{
			if(m_QuickSlot[i].type == _XQS_SLOTTYPE_ITEM)
			{
				if(m_QuickSlot[i].invenNumber == invennumber)
				{
					memset(&m_QuickSlot[i], 0, sizeof(_XQSlot_Info));
					return;
				}
			}
		}
	}
}

void _XWindow_QuickSlot::DrawSkillLevel(void)
{
	int		index = 0, learnedskillindex = 0;
	
	for(int i = 0 ; i < 10 ; i++)
	{
		index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(m_QuickSlot[index].used)
			{
				if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
				{
					learnedskillindex = g_pLocalUser->GetLearnedSkillIndex(m_QuickSlot[index].skillid, TRUE);

					_XUserSkill* pSkill = NULL;
					if(learnedskillindex == _XDEF_MAX_LEARNSKILL)
					{
						pSkill = &g_pLocalUser->m_EventSkillList;
					}
					else
					{
						pSkill = g_pLocalUser->GetSkillItem(m_QuickSlot[index].skillid);
					}

					if(learnedskillindex >= 0)
					{
						if(pSkill->m_cSelectedSkillLevel == pSkill->m_cSkillLevel)
							g_XSmallFont->SetColor(D3DCOLOR_ARGB(255, 165, 165, 165));
						else
							g_XSmallFont->SetColor(D3DCOLOR_ARGB(255, 253, 255, 211));

						g_XSmallFont->PrintAlign(m_WindowPosition.x+m_PositionArray[i].x+17, m_WindowPosition.y+m_PositionArray[i].y+36,
							1.0f, _XFONT_ALIGNTYPE_CENTER, "%d", pSkill->m_cSelectedSkillLevel+1);
						g_XSmallFont->Flush();
					}
				}
			}
		}
	}
}

void _XWindow_QuickSlot::SetCurrentSelectedActiveIndex(int index)
{
	m_CurrentSelectedActiveIndex = index;
	m_CurrentSelectedOtherIndex = -1;
	if(index < 0)
		m_CurrentSelectedActiveStartTime = 0;
	else
		m_CurrentSelectedActiveStartTime = g_LocalSystemTime;
}

void _XWindow_QuickSlot::SetCurrentSelectedOtherIndex(int index)
{
	m_CurrentSelectedOtherIndex = index;
	if(index < 0)
		m_CurrentSelectedOtherStartTime = 0;
	else
		m_CurrentSelectedOtherStartTime = g_LocalSystemTime;
}

void _XWindow_QuickSlot::ProcessCooldown(void)
{
	int index = 0;

	// skill
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID <= 0)
			continue;

		if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL ||
			g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
			continue;

		for(index = 0 ; index < 30 ; index++)
		{
			if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
			{
				if(g_pLocalUser->m_SkillList[i].m_sSkillID == m_QuickSlot[index].skillid)
				{
					if(g_pLocalUser->m_SkillList[i].m_dwCooldownStartTime > 0)
					{
						DWORD elapsedtime = g_LocalSystemTime - g_pLocalUser->m_SkillList[i].m_dwCooldownStartTime;
						FLOAT drawpercent = ((FLOAT)elapsedtime /
							(FLOAT)(g_pLocalUser->m_SkillList[i].m_dwCooldownTime))*120.;

						m_CooldownIndex[index] = (int)drawpercent;

						if(m_CooldownIndex[index] < 0)
							m_CooldownIndex[index] = 0;
						if(m_CooldownIndex[index] > 119)
							m_CooldownIndex[index] = 119;

						m_QuickSlot[index].cooldowndraw = true;
					}
					else
					{
						if(m_QuickSlot[index].cooldowndraw)
						{
							m_QuickSlot[index].cooldownlastdraw = true;
							m_QuickSlot[index].cooldowndraw = false;
							m_CooldownIndex[index] = 0;
							_XPlayInterfaceSound(ID_SR_INTERFACE_COOLDOWN_WAV);
						}
					}
				}
			} // if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
		}
	}

#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
	if(g_pLocalUser->m_EventSkillList.m_sSkillID > 0)
	{	
		for(index = 0 ; index < 30 ; index++)
		{
			if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
			{
				if(g_pLocalUser->m_EventSkillList.m_sSkillID == m_QuickSlot[index].skillid)
				{
					if(g_pLocalUser->m_EventSkillList.m_dwCooldownStartTime > 0)
					{
						DWORD elapsedtime = g_LocalSystemTime - g_pLocalUser->m_EventSkillList.m_dwCooldownStartTime;
						FLOAT drawpercent = ((FLOAT)elapsedtime /
							(FLOAT)(g_pLocalUser->m_EventSkillList.m_dwCooldownTime))*120.;
						
						m_CooldownIndex[index] = (int)drawpercent;
						
						if(m_CooldownIndex[index] < 0)
							m_CooldownIndex[index] = 0;
						if(m_CooldownIndex[index] > 119)
							m_CooldownIndex[index] = 119;
						
						m_QuickSlot[index].cooldowndraw = true;
					}
					else
					{
						if(m_QuickSlot[index].cooldowndraw)
						{
							m_QuickSlot[index].cooldownlastdraw = true;
							m_QuickSlot[index].cooldowndraw = false;
							m_CooldownIndex[index] = 0;
							_XPlayInterfaceSound(ID_SR_INTERFACE_COOLDOWN_WAV);
						}
					}
				}
			} // if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
		}
	}
#endif

	// item
	for(index = 0 ; index < 30 ; index++)
	{
		if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_ITEM)
		{
			int invennumber = m_QuickSlot[index].invenNumber;

			if(g_pLocalUser->m_UserItemList[invennumber].Get_m_cType() == m_QuickSlot[index].itemCType &&
				g_pLocalUser->m_UserItemList[invennumber].Get_m_sID() == m_QuickSlot[index].itemSID)
			{
				if(g_pLocalUser->m_UserItemList[invennumber].m_dwCooldownStartTime > 0)
				{
					DWORD elapsedtime = g_LocalSystemTime - g_pLocalUser->m_UserItemList[invennumber].m_dwCooldownStartTime;
					DWORD totalCooldownTime = 0;

					if(m_QuickSlot[index].itemCType == _XGI_FC_POTION)
					{
						totalCooldownTime = g_PotionItemProperty[m_QuickSlot[index].itemSID].sCooldown*1000;
					}
					
					if(totalCooldownTime > 0)
					{
						FLOAT drawpercent = ((FLOAT)elapsedtime / (FLOAT)(totalCooldownTime))*120.;
						
						m_CooldownIndex[index] = (int)drawpercent;
						
						if(m_CooldownIndex[index] < 0)
							m_CooldownIndex[index] = 0;
						if(m_CooldownIndex[index] > 119)
							m_CooldownIndex[index] = 119;
						
						m_QuickSlot[index].cooldowndraw = true;
					}
					else
					{
						g_pLocalUser->m_UserItemList[invennumber].m_dwCooldownStartTime = 0;
					}
				}
				else
				{
					if(m_QuickSlot[index].cooldowndraw)
					{
						m_QuickSlot[index].cooldownlastdraw = true;
						m_QuickSlot[index].cooldowndraw = false;
						m_CooldownIndex[index] = 0;
						_XPlayInterfaceSound(ID_SR_INTERFACE_COOLDOWN_WAV);
					}
				}
			}
		}
	}
}

void _XWindow_QuickSlot::ProcessCooldownLastImage(void)
{
	int index = 0;
	
	// skill
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID <= 0)
			continue;
		
		if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL ||
			g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
			continue;
		
		for(index = 0 ; index < 30 ; index++)
		{
			if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
			{
				if(m_QuickSlot[index].cooldownlastdraw)
				{
					if(m_QuickSlot[index].prevdrawcooldowntime == 0)
						m_QuickSlot[index].prevdrawcooldowntime = g_LocalSystemTime;
					
					DWORD elapsedtime = g_LocalSystemTime - m_QuickSlot[index].prevdrawcooldowntime;
					
					m_QuickSlot[index].cooldowneffectframe += ((FLOAT)elapsedtime / 100.0f);
					
					if(m_QuickSlot[index].cooldowneffectframe > 5.0f)
						m_QuickSlot[index].cooldowneffectframe = 4.0f;
					
					m_CooldownIndex[index] = 120 + (int)m_QuickSlot[index].cooldowneffectframe;
					if(m_CooldownIndex[index] >= 124)
						m_CooldownIndex[index] = 124;
					
					m_QuickSlot[index].prevdrawcooldowntime = g_LocalSystemTime;
					
					if(m_CooldownIndex[index] >= 124)
					{
						m_QuickSlot[index].cooldowneffectframe = 0.0f;
						m_QuickSlot[index].prevdrawcooldowntime = 0;
						m_QuickSlot[index].cooldownlastdraw = false;
						m_CooldownIndex[index] = 0;
					}
				}
			} // if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
		}
	}

#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
	if(g_pLocalUser->m_EventSkillList.m_sSkillID > 0)
	{
		for(index = 0 ; index < 30 ; index++)
		{
			if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
			{
				if(m_QuickSlot[index].cooldownlastdraw)
				{
					if(m_QuickSlot[index].prevdrawcooldowntime == 0)
						m_QuickSlot[index].prevdrawcooldowntime = g_LocalSystemTime;
					
					DWORD elapsedtime = g_LocalSystemTime - m_QuickSlot[index].prevdrawcooldowntime;
					
					m_QuickSlot[index].cooldowneffectframe += ((FLOAT)elapsedtime / 100.0f);
					
					if(m_QuickSlot[index].cooldowneffectframe > 5.0f)
						m_QuickSlot[index].cooldowneffectframe = 4.0f;
					
					m_CooldownIndex[index] = 120 + (int)m_QuickSlot[index].cooldowneffectframe;
					if(m_CooldownIndex[index] >= 124)
						m_CooldownIndex[index] = 124;
					
					m_QuickSlot[index].prevdrawcooldowntime = g_LocalSystemTime;
					
					if(m_CooldownIndex[index] >= 124)
					{
						m_QuickSlot[index].cooldowneffectframe = 0.0f;
						m_QuickSlot[index].prevdrawcooldowntime = 0;
						m_QuickSlot[index].cooldownlastdraw = false;
						m_CooldownIndex[index] = 0;
					}
				}
			} // if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
		}
	}
#endif
	
	// item
	for(index = 0 ; index < 30 ; index++)
	{
		if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_ITEM)
		{
			if(m_QuickSlot[index].cooldownlastdraw)
			{
				if(m_QuickSlot[index].prevdrawcooldowntime == 0)
					m_QuickSlot[index].prevdrawcooldowntime = g_LocalSystemTime;
				
				DWORD elapsedtime = g_LocalSystemTime - m_QuickSlot[index].prevdrawcooldowntime;
				
				m_QuickSlot[index].cooldowneffectframe += ((FLOAT)elapsedtime / 100.0f);
				
				if(m_QuickSlot[index].cooldowneffectframe > 5.0f)
					m_QuickSlot[index].cooldowneffectframe = 4.0f;
				
				m_CooldownIndex[index] = 120 + (int)m_QuickSlot[index].cooldowneffectframe;
				if(m_CooldownIndex[index] >= 124)
					m_CooldownIndex[index] = 124;
				
				m_QuickSlot[index].prevdrawcooldowntime = g_LocalSystemTime;
				
				if(m_CooldownIndex[index] >= 124)
				{
					m_QuickSlot[index].cooldowneffectframe = 0.0f;
					m_QuickSlot[index].prevdrawcooldowntime = 0;
					m_QuickSlot[index].cooldownlastdraw = false;					
					m_CooldownIndex[index] = 0;
				}
			}
		}
	}
}

void _XWindow_QuickSlot::ProcessDelayTime(void)
{
	if(g_pLocalUser->m_bCheckDelayTime)
	{
		if(g_pLocalUser->m_ChangeDelayStartTime > 0)
		{
			DWORD elapsedtime = g_LocalSystemTime - g_pLocalUser->m_ChangeDelayStartTime;

			int delaytime = 0;

//Author : 양희왕
#ifdef _XDEF_SKILLDEALY_MODIFY
			if(g_pLocalUser->GetFinallySelectedSkillID() > 0)
				delaytime = _XSkillItem::GetDelay(g_pLocalUser->GetFinallySelectedSkillID(), g_pLocalUser->GetFinallySelectedSkillItem()->m_cSelectedSkillLevel);
#else
			if(g_pLocalUser->m_SelectedSkillID > 0)
				delaytime = _XSkillItem::GetDelay(g_pLocalUser->m_SelectedSkillID, g_pLocalUser->m_SelectedSkillItem->m_cSelectedSkillLevel);
#endif
//Last Updated : 07/03/23
			
			else
				delaytime = 500;
			
			if(g_pLocalUser->m_Lock.adddelaytime == 1)
			{
				// 상태효과 - dealy time 증가
				delaytime *= 2;
			}
			else if(g_pLocalUser->m_bAddDelayTime)
			{
				delaytime += 1000;
			}

			FLOAT drawpercent = ((FLOAT)elapsedtime / (FLOAT)delaytime)*120.0f;
			m_DelayTimeIndex = (int)drawpercent;
			if(m_DelayTimeIndex < 0)
				m_DelayTimeIndex = 0;
			if(m_DelayTimeIndex > 119)
				m_DelayTimeIndex = 119;
		}
		else
		{
			m_DelayTimeIndex = 0;
		}
	}
	else
	{
		m_DelayTimeIndex = 0;
	}
}

void _XWindow_QuickSlot::DrawDelayTime(void)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		int index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(g_pLocalUser->m_bCheckDelayTime)
			{
				if(g_pLocalUser->m_ChangeDelayStartTime > 0)
				{
					if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
					{
						m_CooldownImage[m_DelayTimeIndex].Draw(m_WindowPosition.x+m_PositionArray[i].x+3, m_WindowPosition.y+m_PositionArray[i].y+3);
					}
				}
			}
		}
	}
}

void _XWindow_QuickSlot::SetDrawGuideLine(BOOL flag)
{
	if( flag )
	{
		m_GuideLineImage.ShowWindow(TRUE);
	}
	else
	{
		m_GuideLineImage.ShowWindow(FALSE);
	}
}

void _XWindow_QuickSlot::CheckTutorial(short skillid)
{
#ifdef _XDEF_EVENT_ADD_HYPERRUN_080417_KUKURI
	if(g_pLocalUser->GetLearnedSkillIndex(skillid) == _XDEF_MAX_LEARNSKILL)	// 이벤트 스킬일 경우 처리하지 않음
		return;
#endif

	// Tutorial : 2005.03.08->hotblood --------------------------------------------------------------------------------------------=			
	if( g_TutorialInfo )
	{
		if( !g_TutorialInfo->_XTutorial_007 && (g_SkillProperty[skillid]->skillCType == _XSI_CTYPE_SPELL) )
		{
			_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
			pTutorialIcon->InsertTutorialIcon(_XTUTORIAL_007);
			g_TutorialInfo->_XTutorial_007 = TRUE;
		}
		
		if( !g_TutorialInfo->_XTutorial_030 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);				
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_030);
				g_TutorialInfo->_XTutorial_030 = TRUE;
			}			
		}
		
		if( !g_TutorialInfo->_XTutorial_039 )
		{
			int learnedindex = g_pLocalUser->GetLearnedSkillIndex(skillid);
			if( g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState > 0)
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_039);
				g_TutorialInfo->_XTutorial_039 = TRUE;
			}		
		}
		
		if( !g_TutorialInfo->_XTutorial_042 )
		{
			if( g_SkillProperty[skillid]->usefulType == _XSI_USEFULTYPE_CHANNELINGSPELL && 
				g_SkillProperty[skillid]->skillSType == 3 )
			{
				_XWindow_TutorialIcon* pTutorialIcon = (_XWindow_TutorialIcon*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_TUTORIALICON);
				pTutorialIcon->RemoveTutorialIcon(_XTUTORIAL_042);
				g_TutorialInfo->_XTutorial_042 = TRUE;
			}					
		}
		
		if( !g_TutorialInfo->_XTutorial_052 )
		{
			if( skillid == 1 )	// 양생도인술
			{
				_XWindow_GuideLine* pGuideLine_Window = (_XWindow_GuideLine*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_GUIDELINEWINDOW);
				if( pGuideLine_Window )
				{
					if( pGuideLine_Window->GetShowStatus() )
					{
						pGuideLine_Window->SetGuideLIneStep(GS_SECOND);						
					}
				}
			}
		}
	}	
	// ----------------------------------------------------------------------------------------------------------------------------=
}

void _XWindow_QuickSlot::DrawCooldownImage(void)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		int index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(m_QuickSlot[index].cooldowndraw)
			{
				m_CooldownImage[m_CooldownIndex[index]].Draw(m_WindowPosition.x+m_PositionArray[i].x+3, m_WindowPosition.y+m_PositionArray[i].y+3);
			}
		}
	}
}

void _XWindow_QuickSlot::DrawCooldownLastImage(void)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		int index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(m_QuickSlot[index].cooldownlastdraw)
			{
				m_CooldownImage[m_CooldownIndex[index]].Draw(m_WindowPosition.x+m_PositionArray[i].x+3, m_WindowPosition.y+m_PositionArray[i].y+3);
			}
		}
	}
}

void _XWindow_QuickSlot::SetCurrentSelectedActiveIndexFromSkillID(short skillid)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		int index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
			{
				if(m_QuickSlot[index].skillid == skillid)
				{
					SetCurrentSelectedActiveIndex(i);
					return;
				}
			}
		}
	}
}

void _XWindow_QuickSlot::SetCurrentSelectedOtherIndexFromSkillID(short skillid)
{
	for(int i = 0 ; i < 10 ; i++)
	{
		int index = m_SelectedSet*10 + i;
		if(index >= 0 && index < 30)
		{
			if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
			{
				if(m_QuickSlot[index].skillid == skillid)
				{
					SetCurrentSelectedOtherIndex(i);
					return;
				}
			}
		}
	}
}

void _XWindow_QuickSlot::DrawPassiveOnImage(void)
{
	int index = 0;
	
	// skill
	for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
	{
		if(g_pLocalUser->m_SkillList[i].m_sSkillID <= 0)
			continue;
		
		for(int j = 0 ; j < 10 ; j++)
		{
			index = m_SelectedSet*10 + j;
			if(index >= 0 && index < 30)
			{
				if(m_QuickSlot[index].used && m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
				{
					if(g_pLocalUser->m_SkillList[i].m_sSkillID == m_QuickSlot[index].skillid)
					{
						if(g_SkillProperty[m_QuickSlot[index].skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESKILL || 
							g_SkillProperty[m_QuickSlot[index].skillid]->usefulType == _XSI_USEFULTYPE_PASSIVESPELL)
						{
							if(g_pLocalUser->m_SkillList[i].m_cOnOff == 1)
							{
								// Passive On
								if(m_PrevPassiveDrawTime == 0)
									m_PrevPassiveDrawTime = g_LocalSystemTime;

								DWORD elapsedtime = g_LocalSystemTime - m_PrevPassiveDrawTime;

								m_PassiveEffectFrame += ((FLOAT)elapsedtime / 100.0f);

								if(m_PassiveEffectFrame >= 9.0f)
									m_PassiveEffectFrame = 0.0f;

								int selectedframe = (int)m_PassiveEffectFrame;

								if(selectedframe > 8)
									selectedframe = 0;

								m_PassiveOnImage[selectedframe].Draw(m_WindowPosition.x+m_PositionArray[j].x+3, m_WindowPosition.y+m_PositionArray[j].y+3);
								m_PrevPassiveDrawTime = g_LocalSystemTime;
							}
						}
					}
				} // if(m_QuickSlot[index].type == _XQS_SLOTTYPE_SKILL)
			}
		}
	}
}

void _XWindow_QuickSlot::SetDefaultPosition()
{

}

void _XWindow_QuickSlot::ReplacementOfItems(BOOL bChange)
{
	for(int i = 0 ; i < 30 ; i++)
	{
		if(m_QuickSlot[i].used && m_QuickSlot[i].type == _XQS_SLOTTYPE_SKILL)
			m_QuickSlot[i].bChangeItem = bChange;			
	}

	m_bChangeItem = bChange;
}

// ----------------------------------------------------------------------------
// Quick slot 저장 class

#define _XDEF_USERINFOFILE_VERSION	0xFE04

BOOL _XQuickSlotInfoList::LoadQuickSlotInfo( void )
{

#ifdef _XDEF_QUICKSLOTSERVERSAVE
		return TRUE;
#endif

	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return TRUE;

	disposeList();

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
	FILE* pFilePtr = fopen( _T("UserInfo.XUI"), "rb" );
	
	if( !pFilePtr ) 
	{
		FILE* pFilePtr = fopen( _T("UserInfo.XUI"), "wb" );
		int _infocount = 0;
		DWORD version = _XDEF_USERINFOFILE_VERSION;
		fwrite( &version, sizeof(DWORD), 1, pFilePtr );
		fwrite( &_infocount , sizeof(int), 1, pFilePtr );
		fclose( pFilePtr );
		return TRUE;
	}
	
	DWORD version = 0;
	fread( &version, sizeof(DWORD), 1, pFilePtr );

	if( version != _XDEF_USERINFOFILE_VERSION )
	{
		_XLog( "WARNING : Not supported file version %d/%d", version, _XDEF_USERINFOFILE_VERSION );
		fclose( pFilePtr );

		pFilePtr = fopen( _T("UserInfo.XUI"), "wb" );
		int _infocount = 0;
		DWORD version = _XDEF_USERINFOFILE_VERSION;
		fwrite( &version, sizeof(DWORD), 1, pFilePtr );
		fwrite( &_infocount , sizeof(int), 1, pFilePtr );
		fclose( pFilePtr );

		return FALSE;
	}

	int _infocount = 0;
	fread( &_infocount , sizeof(int), 1, pFilePtr );
	
	_XQuickSlotSaveInfo* pQuickSlotSaveInfo;
	
	for( int i = 0; i < _infocount; i++ )
	{
		pQuickSlotSaveInfo = new _XQuickSlotSaveInfo;

		if( pQuickSlotSaveInfo )
		{
			memset(pQuickSlotSaveInfo->Owner, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
			fread( pQuickSlotSaveInfo->Owner, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH, 1, pFilePtr );
			fread( pQuickSlotSaveInfo->m_QuickSlot, sizeof(_XQSlot_Info) * 30, 1, pFilePtr );
			insertItem( pQuickSlotSaveInfo );
			
//#ifdef _XDWDEBUG
//			_XDWINPRINT("QuickSlot Infomation [Owner :%s]===========================", pQuickSlotSaveInfo->Owner);
//			for(int i = 0 ; i < 30 ; i++)
//			{
//				_XDWINPRINT("QUickSlot [index:%d] [type:%d] [skillid:%d] [lifeskillid:%d] [itemctype:%d] [itemstype:%d] [inven:%d] [used:%d]", 
//					i, pQuickSlotSaveInfo->m_QuickSlot[i].type, pQuickSlotSaveInfo->m_QuickSlot[i].skillid, pQuickSlotSaveInfo->m_QuickSlot[i].lifeskillid,
//					pQuickSlotSaveInfo->m_QuickSlot[i].itemCType, pQuickSlotSaveInfo->m_QuickSlot[i].itemSID, pQuickSlotSaveInfo->m_QuickSlot[i].invenNumber,
//					pQuickSlotSaveInfo->m_QuickSlot[i].used);
//			}
//			_XDWINPRINT("===========================================================");
//#endif

		}
	}

	fclose( pFilePtr );	
	return TRUE;
}

void _XQuickSlotInfoList::UpdateQuickSlotInfo( void )
{

#ifdef _XDEF_QUICKSLOTSERVERSAVE	
	return;
#endif

	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return;
	
	_XQuickSlotSaveInfo* pQuickSlotSaveInfo;

	BOOL savedcurrentuser = FALSE;
	
	_XWindow_QuickSlot* pQSlot_Window = (_XWindow_QuickSlot *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUICKSLOT);
	if( !pQSlot_Window ) return;
	
	if( !listEmpty() )
	{
		resetList();
		
		do 
		{
			pQuickSlotSaveInfo = (_XQuickSlotSaveInfo*)currentItem();
			if( pQuickSlotSaveInfo )
			{
				if( strcmpi( pQuickSlotSaveInfo->Owner, g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
				{
//#ifdef _XDWDEBUG
//					_XDWINPRINT("Find LocalUser's Quickslot information [%s]", g_pLocalUser->m_CharacterInfo.charactername);
//#endif

					savedcurrentuser = TRUE;
					memcpy( pQSlot_Window->m_QuickSlot, pQuickSlotSaveInfo->m_QuickSlot, sizeof( _XQSlot_Info ) * 30 );
					break;
				}				
			}
			
			nextItem();
		} while( !atHeadOfList() );
	}
	
	if( !savedcurrentuser )
	{
		pQuickSlotSaveInfo = new _XQuickSlotSaveInfo;
		memset( pQuickSlotSaveInfo->Owner, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
		strcpy( pQuickSlotSaveInfo->Owner, g_pLocalUser->m_CharacterInfo.charactername );
		
		memset( pQuickSlotSaveInfo->m_QuickSlot, 0, sizeof(_XQSlot_Info) * 30 );
		memcpy( pQuickSlotSaveInfo->m_QuickSlot, pQSlot_Window->m_QuickSlot, sizeof( _XQSlot_Info ) * 30 );
		
		insertItem( pQuickSlotSaveInfo );
	}

	pQSlot_Window->CheckQuickSlotSkill();
	pQSlot_Window->CheckQuickSlotItem();
}

void _XQuickSlotInfoList::UpdateInfoList( void )
{

#ifdef _XDEF_QUICKSLOTSERVERSAVE	
	return;
#endif

	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return;
	
	_XQuickSlotSaveInfo* pQuickSlotSaveInfo;
	BOOL savedcurrentuser = FALSE;
	
	_XWindow_QuickSlot* pQSlot_Window = (_XWindow_QuickSlot *)g_MainWindowManager.FindWindow(_XDEF_WTITLE_QUICKSLOT);
	if( !pQSlot_Window ) return;
	
	if( !listEmpty() )
	{
		resetList();
		
		do 
		{
			pQuickSlotSaveInfo = (_XQuickSlotSaveInfo*)currentItem();
			if( pQuickSlotSaveInfo )
			{
				for(int i = 0 ; i < 30 ; i++)
				{
					if(pQuickSlotSaveInfo->m_QuickSlot[i].type == _XQS_SLOTTYPE_SKILL)
					{
						if(!_XSkillItem::FindSkillProperty(pQuickSlotSaveInfo->m_QuickSlot[i].skillid))
							return;
					}
				}
			}
			
			nextItem();
		} while( !atHeadOfList() );
	}
	
	if( !listEmpty() )
	{
		resetList();
		
		do 
		{
			pQuickSlotSaveInfo = (_XQuickSlotSaveInfo*)currentItem();
			if( pQuickSlotSaveInfo )
			{
				if( strcmpi( pQuickSlotSaveInfo->Owner, g_pLocalUser->m_CharacterInfo.charactername ) == 0 )
				{
					savedcurrentuser = TRUE;
					memcpy( pQuickSlotSaveInfo->m_QuickSlot, pQSlot_Window->m_QuickSlot, sizeof( _XQSlot_Info ) * 30 );
					break;
				}				
			}
			
			nextItem();
		} while( !atHeadOfList() );
	}
	
	if( !savedcurrentuser )
	{
		pQuickSlotSaveInfo = new _XQuickSlotSaveInfo;
		memset( pQuickSlotSaveInfo->Owner, 0, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH );
		strcpy( pQuickSlotSaveInfo->Owner, g_pLocalUser->m_CharacterInfo.charactername );
		
		memset( pQuickSlotSaveInfo->m_QuickSlot, 0, sizeof( _XQSlot_Info ) * 30 );
		memcpy( pQuickSlotSaveInfo->m_QuickSlot, pQSlot_Window->m_QuickSlot, sizeof( _XQSlot_Info ) * 30 );
		
		insertItem( pQuickSlotSaveInfo );
	}
}

BOOL _XQuickSlotInfoList::SaveQuickSlotInfo( void )
{
#ifdef _XDEF_QUICKSLOTSERVERSAVE	
	return TRUE;
#endif

	if( strlen( g_pLocalUser->m_CharacterInfo.charactername ) <= 0 ) return TRUE;

	_XSetDefaultPath( _XDEF_DEFAULTPATH_SCRIPTER );
	FILE* pFilePtr = fopen( _T("UserInfo.XUI"), "wb" );

	if( !pFilePtr ) 
	{
		_XLog( "WARNING : Can't open user informations file" );
		return FALSE;
	}
	
	DWORD version = _XDEF_USERINFOFILE_VERSION;
	fwrite( &version, sizeof(DWORD), 1, pFilePtr );
	fwrite( &itemcount, sizeof(int), 1, pFilePtr );

	_XQuickSlotSaveInfo* pQuickSlotSaveInfo;

	if( !listEmpty() )
	{
		resetList();

		do 
		{
			pQuickSlotSaveInfo = (_XQuickSlotSaveInfo*)currentItem();
			if( pQuickSlotSaveInfo )
			{
				fwrite( pQuickSlotSaveInfo->Owner, sizeof(TCHAR) * _XDEF_MAX_USERNAMESTRINGLENGTH, 1, pFilePtr );
				fwrite( pQuickSlotSaveInfo->m_QuickSlot, sizeof(_XQSlot_Info) * 30, 1, pFilePtr );
			}
			
			nextItem();
		} while( !atHeadOfList() );
	}
	
	fclose( pFilePtr );

	return TRUE;
}

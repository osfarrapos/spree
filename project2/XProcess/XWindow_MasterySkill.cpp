// XWindow_MasterySkill.cpp: implementation of the _XWindow_MasterySkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_MasterySkill.h"
#include "XWindowObjectDefine.h"
#include "XMessageWindow.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MasterySkill::_XWindow_MasterySkill()
{
	for(int i = 0 ; i < 4 ; i++)
		m_MasteryClassTab[i] = NULL;

	m_MasteryPointImage		= NULL;

	m_SelectedTab				= 0;
	
	memset(m_MasterySkillSlot, 0, sizeof(m_MasterySkillSlot));

	for(i = 0 ; i < 4 ; i++)
		m_SlotImage[i] = NULL;

	m_ArrowImage			= NULL;
	m_ValidLineImage		= NULL;
	m_InvalidLineImage		= NULL;
	m_SelectedImage			= NULL;
	m_bMasteryPointClickable = FALSE;
	m_iSelectedSlot			= -1;

	m_TotalPoint			= 0;
}

_XWindow_MasterySkill::~_XWindow_MasterySkill()
{
	DestroyWindow();
}

BOOL _XWindow_MasterySkill::Initialize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("MI_Characterwin.tga");
	int resourceindex_2 = g_MainInterfaceTextureArchive.FindResource("MI_quest2.tga");
	
	_XCHECKBTN_STRUCTURE tabstruct = 
	{
		TRUE, {1, 8}, {76, 22}, _XDEF_MASTERYSKILL_CLASS_1,
		resourceindex_2, resourceindex_2, &g_MainInterfaceTextureArchive
	};

	for(int i = 0 ; i < 4 ; i++)
	{
		tabstruct.position.x = 1 + (i*87);
		m_MasteryClassTab[i] = new _XCheckButton;
		m_MasteryClassTab[i]->Create(tabstruct);

		m_MasteryClassTab[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 81, 192, 157, 214);
		m_MasteryClassTab[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 0, 190, 76, 212);
		m_MasteryClassTab[i]->SetCheck(FALSE);
		m_MasteryClassTab[i]->EnableWindow(FALSE);
		InsertChildObject(m_MasteryClassTab[i]);

		tabstruct.commandid++;
	}

	m_MasteryClassTab[0]->SetCheck(TRUE);

	m_MasteryPointImage = new _XImageStatic;	
	#ifdef _XRUSSIAN
	m_MasteryPointImage->Create(5, 361, 96, 385, &g_MainInterfaceTextureArchive, resourceindex);
	m_MasteryPointImage->SetClipRect(2, 107, 111, 130);
	#else
	m_MasteryPointImage->Create(5, 357, 96, 386, &g_MainInterfaceTextureArchive, resourceindex);
	m_MasteryPointImage->SetClipRect(0, 73, 91, 102);
	#endif
	
	for(i = 0 ; i < 4 ; i++)
	{
		m_SlotImage[i] = new _XImageStatic;
		m_SlotImage[i]->Create(0, 0, 66, 40, &g_MainInterfaceTextureArchive, resourceindex);
	}
	m_SlotImage[0]->SetClipRect(6, 162, 72, 202);			// 현재 배운 오의
	m_SlotImage[1]->SetClipRect(81, 162, 147, 202);			// 배울수 있는 오의 - 하나도 배우지 않음
	m_SlotImage[2]->SetClipRect(6, 212, 72, 252);			// 배울수 없는 오의
	m_SlotImage[3]->SetClipRect(157, 211, 223, 251);		// 마스터한 오의

	m_ArrowImage = new _XImageStatic;
	m_ArrowImage->Create(0, 0, 11, 9, &g_MainInterfaceTextureArchive, resourceindex);
	m_ArrowImage->SetClipRect(116, 146, 127, 155);

	m_ValidLineImage = new _XImageStatic;
	m_ValidLineImage->Create(0, 0, 11, 1, &g_MainInterfaceTextureArchive, resourceindex);
	m_ValidLineImage->SetClipRect(116, 139, 127, 140);

	m_InvalidLineImage = new _XImageStatic;
	m_InvalidLineImage->Create(0, 0, 11, 1, &g_MainInterfaceTextureArchive, resourceindex);
	m_InvalidLineImage->SetClipRect(116, 137, 127, 138);

	m_SelectedImage = new _XImageStatic;
	m_SelectedImage->Create(0, 0, 66, 40, &g_MainInterfaceTextureArchive, resourceindex);
	m_SelectedImage->SetClipRect(81, 211, 147, 251);

	m_MasterySkillIcon.Create(0, 0, 31, 31, &g_MainInterfaceTextureArchive, 0);
	m_MasterySkillGrayIcon.Create(0, 0, 31, 31, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_cooltime.tga"));

	return TRUE;
}

void _XWindow_MasterySkill::DestroyWindow(void)
{
	SAFE_DELETE(m_MasteryPointImage);
	SAFE_DELETE(m_ArrowImage);
	SAFE_DELETE(m_ValidLineImage);
	SAFE_DELETE(m_InvalidLineImage);
	SAFE_DELETE(m_SelectedImage);

	for(int i = 0 ; i < 4 ; i++)
		SAFE_DELETE(m_SlotImage[i]);

	_XWindow::DestroyWindow();
}

void _XWindow_MasterySkill::Draw(_XGUIObject*& pfocusobject)
{
	if(m_ShowWindow)
	{
		_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+39, m_WindowPosition.x+328, m_WindowPosition.y+353, 0x7F000000 );
		_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+38, m_WindowPosition.x+328, m_WindowPosition.y+353, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	DrawMasterySkill();
	if( !m_bMasteryPointClickable )
		DrawTooltip();

	#ifdef _XRUSSIAN
	m_MasteryPointImage->Draw(m_WindowPosition.x+5, m_WindowPosition.y+361);
	#else
	m_MasteryPointImage->Draw(m_WindowPosition.x+5, m_WindowPosition.y+357);
	#endif

	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE,0xFF000000);
	g_XBaseFont->SetColor(0xFFE6DABD);
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		g_XBaseFont->Puts(m_WindowPosition.x+9, m_WindowPosition.y+366, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_PASSIVESKILL));	// "오의"
	else
		g_XBaseFont->Puts(m_WindowPosition.x+14, m_WindowPosition.y+366, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_PASSIVESKILL));	// "오의"
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	// 남은 오의 포인트
	g_XBaseFont->SetColor(0xFF6D993);
	g_XBaseFont->PrintAlign(m_WindowPosition.x+85, m_WindowPosition.y+366, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint());
	g_XBaseFont->Flush();

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 181, 181, 181));

	// 오의 총합
#ifdef _XDEF_MASTERY_TOTALPOINT_080313_KUKURI

	g_XBaseFont->PutsAlign(m_WindowPosition.x+314, m_WindowPosition.y+366, _XFONT_ALIGNTYPE_RIGHT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MASTERYSKILL_2483, 
							   _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_ALL), &m_TotalPoint )); //전체 오의 총합 : %d

#else

	if(m_SelectedTab == 0)
		g_XBaseFont->PrintAlign(m_WindowPosition.x+314, m_WindowPosition.y+366, 1.0f, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SKILLCOUNT), m_TotalPoint);	// "기본 오의 총합 : %d"
	else
	{
	#ifdef _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI
		g_XBaseFont->PutsAlign(m_WindowPosition.x+314, m_WindowPosition.y+366, _XFONT_ALIGNTYPE_RIGHT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MASTERYSKILL_2483, 
							   g_StringDataBase.GetTextString(_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, GetMasterySkillNameId()), &m_TotalPoint ) );
	#else 
		LPTSTR param1 = GetMasterySkillTypeName(g_pLocalUser->m_CharacterInfo.Get_cClass(), m_SelectedTab-1);
		
		g_XBaseFont->PutsAlign(m_WindowPosition.x+314, m_WindowPosition.y+366, _XFONT_ALIGNTYPE_RIGHT, 
							   g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_MASTERYSKILL_2483, param1, &m_TotalPoint ) );
	#endif
	}

#endif

	g_XBaseFont->Flush();
}


BOOL _XWindow_MasterySkill::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO
	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if( CheckMousePosition() )
				{
					for(int i = 0 ; i < 24 ; i++)
					{				
						if(scrnpos->x > m_WindowPosition.x+m_MasterySkillSlot[i].position.x &&
							scrnpos->x < m_WindowPosition.x+m_MasterySkillSlot[i].position.x + 66 &&
							scrnpos->z > m_WindowPosition.y+m_MasterySkillSlot[i].position.y &&
							scrnpos->z < m_WindowPosition.y+m_MasterySkillSlot[i].position.y + 40)
						{
							m_WindowMouseState = _XW_STATE_LEFTCLICK;
							break;
						}
					}
				}					
			}
			if(!mousestate->bButton[0])
			{
				if(m_WindowMouseState == _XW_STATE_LEFTCLICK)
				{
					if( m_iSelectedSlot == -1 && !m_bMasteryPointClickable )
					{
						for(int i = 0 ; i < 24 ; i++)
						{				
							if(scrnpos->x > m_WindowPosition.x+m_MasterySkillSlot[i].position.x &&
								scrnpos->x < m_WindowPosition.x+m_MasterySkillSlot[i].position.x + 66 &&
								scrnpos->z > m_WindowPosition.y+m_MasterySkillSlot[i].position.y &&
								scrnpos->z < m_WindowPosition.y+m_MasterySkillSlot[i].position.y + 40)
							{
								if(m_MasterySkillSlot[i].skillid >= 0)
								{
									m_iSelectedSlot = -1;
									m_bMasteryPointClickable = TRUE;
									m_iSelectedSlot = i;
									
									
//#ifdef _XDEF_SHUTDOWN_20061027
//									// 셧다운시에 금지 사항
//									// 오의 변경금지
//									if( g_PlayLeftTime == 0 )
//										break;
//									
//#endif

									_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_MESSAGEWINDOW );
									if( !pMessageBox )
									{
										RemoveTooltip();
										break;
									}

									if(m_MasterySkillSlot[m_iSelectedSlot].prop == _XMS_PROP_NOTLEARN)
									{
										if(g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() > 0)
										{											
											TCHAR		messagestring[256];
											int index = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[m_iSelectedSlot].skillid);
											if( index != -1 )
											{
												if( g_pLocalUser->m_MasterySkillList[index].cPoint+1 <= 20 )
													index = g_pLocalUser->m_MasterySkillList[index].cPoint+1;
												else 
													index = 0;
											}
											else
												index = 1;
											
											// "%s (을)를 %d성으로 올리시겠습니까?"
											if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[m_iSelectedSlot].skillid))
											{
												strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MASTERYSKILL_REQSKILLUP, 
																						g_SkillMasteryProperty[m_MasterySkillSlot[m_iSelectedSlot].skillid]->cName, &index ) );
											}
											else
											{
												TCHAR szStr[16] = _T("NONAME");
												strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MASTERYSKILL_REQSKILLUP, 
																						szStr, &index ) );
											}

											pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_MASTERYSKILL_APPLYOK, _XDEF_MASTERYSKILL_APPLYCANCLE);
											
											if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_MASTERYSKILL, 0, 0, m_MasterySkillSlot[m_iSelectedSlot].skillid ) )
											{
												g_pDefaultTooltip_Window->ShowWindow( TRUE );
											}											
											break;										
										}
										else
										{
											// 남은 오의 점수가 없습니다.
											pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_NOPOINT), TRUE, _XDEF_MASTERYSKILL_APPLYOK);
											break;
										}
									}
									else if(m_MasterySkillSlot[m_iSelectedSlot].prop == _XMS_PROP_UNDER || m_MasterySkillSlot[m_iSelectedSlot].prop == _XMS_PROP_OVER)
									{
										if(g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() > 0)
										{
											char maxMP = 20;
											if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[m_iSelectedSlot].skillid))
												maxMP = g_SkillMasteryProperty[m_MasterySkillSlot[m_iSelectedSlot].skillid]->cMaxMP; 

											if(g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[m_iSelectedSlot].learnedskillid].cPoint >= maxMP)
											{
												// "모두 깨우친 오의 입니다."
												pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_COMPLETESKILL), TRUE, _XDEF_MASTERYSKILL_APPLYOK);
												if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_MASTERYSKILL, 0, 0, m_MasterySkillSlot[m_iSelectedSlot].skillid ) )
												{
													g_pDefaultTooltip_Window->ShowWindow( TRUE );
												}
												break;
											}
											else
											{												
												m_iSelectedSlot = i;
												TCHAR		messagestring[256];
												int index = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[m_iSelectedSlot].skillid);
												if( index != -1 )
												{
													if( g_pLocalUser->m_MasterySkillList[index].cPoint+1 <= 20 )
														index = g_pLocalUser->m_MasterySkillList[index].cPoint+1;
													else 
														index = 0;
												}
												else
													index = 1;

												// "%s (을)를 %d성으로 올리시겠습니까?"
												if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[m_iSelectedSlot].skillid))
												{
													strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MASTERYSKILL_REQSKILLUP, 
															g_SkillMasteryProperty[m_MasterySkillSlot[m_iSelectedSlot].skillid]->cName, &index ) );
												}
												else
												{
													TCHAR szStr[16] = _T("NONAME");
													strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_MASTERYSKILL_REQSKILLUP, 
																											szStr, &index ) );
												}

												pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, NULL, messagestring, TRUE, _XDEF_MASTERYSKILL_APPLYOK, _XDEF_MASTERYSKILL_APPLYCANCLE);
												
												if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_MASTERYSKILL, 0, 0, m_MasterySkillSlot[m_iSelectedSlot].skillid ) )
												{
													g_pDefaultTooltip_Window->ShowWindow( TRUE );
												}												
												break;
											}
										}
										else
										{
											// 남은 오의 점수가 없습니다.
											pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_NOPOINT), TRUE, _XDEF_MASTERYSKILL_APPLYOK);	
											break;
										}
									}
									else if(m_MasterySkillSlot[m_iSelectedSlot].prop == _XMS_PROP_CANNOTLEARN)
									{
										if(g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() <= 0)
										{
											// 남은 오의 점수가 없습니다.
											pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_NOPOINT), TRUE, _XDEF_MASTERYSKILL_APPLYOK);
											break;
										}
										else
										{
											short needPageTotalPoint = 0;
											if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[m_iSelectedSlot].skillid))
												needPageTotalPoint = g_SkillMasteryProperty[m_MasterySkillSlot[m_iSelectedSlot].skillid]->sNeedPageTotalPoint;
											if(m_TotalPoint < needPageTotalPoint)
											{
												// "오의 총합이 부족합니다."
												pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3372), TRUE, _XDEF_MASTERYSKILL_APPLYOK);
												if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_MASTERYSKILL, 0, 0, m_MasterySkillSlot[m_iSelectedSlot].skillid ) )
												{
													g_pDefaultTooltip_Window->ShowWindow( TRUE );
												}
												break;
											}
											else
											{
												// "선수 무공을 배우지 않았습니다."
												pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK, _T(""), _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_NOTLEARNEDSKILL), TRUE, _XDEF_MASTERYSKILL_APPLYOK);
												if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_MASTERYSKILL, 0, 0, m_MasterySkillSlot[m_iSelectedSlot].skillid ) )
												{
													g_pDefaultTooltip_Window->ShowWindow( TRUE );
												}
												break;
											}
										}
									}
								}
								else
									m_WindowMouseState = _XW_STATE_NONE;
							}
						}
					}	// if( m_iSelectedSkillID > 0 )
				}
			} // if(!mousestate->bButton[0])
		}
	} // if(mousestate)

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_MasterySkill::ChangeTab(int selectedtab)
{
	m_SelectedTab = selectedtab;
	
	for(int i = 0 ; i < 4 ; i++)
	{
		m_MasteryClassTab[i]->SetCheck(FALSE);
	}

	m_MasteryClassTab[m_SelectedTab]->SetCheck(TRUE);

	ParseMasterySkill();
	CheckDependency();
}

TCHAR* _XWindow_MasterySkill::GetMasterySkillTypeName(int cClass, int index)
{
	TCHAR* name = NULL;

	switch( g_pLocalUser->m_CharacterInfo.groupindex ) 
	{
	case _XGROUP_GAEBANG :	// 개방
		{
			switch(cClass) 
			{
#ifdef _XTS_MASTERYOPTIONAL
			case 0 :
				name = g_SkillMasteryNameProperty_01_00;
				break;
#endif
			case 1:
				name = g_SkillMasteryNameProperty_01_01[index];
				break;
			case 2:
				name = g_SkillMasteryNameProperty_01_02[index];
				break;
			case 3:
				name = g_SkillMasteryNameProperty_01_03[index];
				break;
			case 4:
				name = g_SkillMasteryNameProperty_01_04[index];
				break;
			case 5:
				name = g_SkillMasteryNameProperty_01_05[index];
				break;
			case 6:
				name = g_SkillMasteryNameProperty_01_06[index];
				break;
			case 7:
				name = g_SkillMasteryNameProperty_01_07[index];
				break;
			case 8:
				name = g_SkillMasteryNameProperty_01_08[index];
				break;
			case 9:
				name = g_SkillMasteryNameProperty_01_09[index];
				break;
			case 10:
				name = g_SkillMasteryNameProperty_01_10[index];
				break;
			case 11:
				name = g_SkillMasteryNameProperty_01_11[index];
				break;			
			}
		}
		break;
	case _XGROUP_BEEGOONG :		// 비궁
		{
			switch(cClass) 
			{
#ifdef _XTS_MASTERYOPTIONAL
			case 0 :
				name = g_SkillMasteryNameProperty_02_00;
				break;
#endif			
			case 1:
				name = g_SkillMasteryNameProperty_02_01[index];
				break;
			case 2:
				name = g_SkillMasteryNameProperty_02_02[index];
				break;
			case 3:
				name = g_SkillMasteryNameProperty_02_03[index];
				break;
			case 4:
				name = g_SkillMasteryNameProperty_02_04[index];
				break;
			case 5:
				name = g_SkillMasteryNameProperty_02_05[index];
				break;
			case 6:
				name = g_SkillMasteryNameProperty_02_06[index];
				break;
			case 7:
				name = g_SkillMasteryNameProperty_02_07[index];
				break;
			case 8:
				name = g_SkillMasteryNameProperty_02_08[index];
				break;
			case 9:
				name = g_SkillMasteryNameProperty_02_09[index];
				break;
			case 10:
				name = g_SkillMasteryNameProperty_02_10[index];
				break;
			case 11:
				name = g_SkillMasteryNameProperty_02_11[index];
				break;			
			}
		}
		break;
	case _XGROUP_SORIM :	// 소림
		{
			switch(cClass) 
			{
#ifdef _XTS_MASTERYOPTIONAL
			case 0 :
				name = g_SkillMasteryNameProperty_03_00;
				break;
#endif	
			case 1:
				name = g_SkillMasteryNameProperty_03_01[index];
				break;
			case 2:
				name = g_SkillMasteryNameProperty_03_02[index];
				break;
			case 3:
				name = g_SkillMasteryNameProperty_03_03[index];
				break;
			case 4:
				name = g_SkillMasteryNameProperty_03_04[index];
				break;
			case 5:
				name = g_SkillMasteryNameProperty_03_05[index];
				break;
			case 6:
				name = g_SkillMasteryNameProperty_03_06[index];
				break;
			case 7:
				name = g_SkillMasteryNameProperty_03_07[index];
				break;
			case 8:
				name = g_SkillMasteryNameProperty_03_08[index];
				break;
			case 9:
				name = g_SkillMasteryNameProperty_03_09[index];
				break;
			case 10:
				name = g_SkillMasteryNameProperty_03_10[index];
				break;
			case 11:
				name = g_SkillMasteryNameProperty_03_11[index];
				break;			
			}
		}
		break;
	case _XGROUP_NOCKRIM :		// 녹림
		{
			switch(cClass) 
			{
#ifdef _XTS_MASTERYOPTIONAL
			case 0 :
				name = g_SkillMasteryNameProperty_04_00;
				break;
#endif	
			case 1:
				name = g_SkillMasteryNameProperty_04_01[index];
				break;
			case 2:
				name = g_SkillMasteryNameProperty_04_02[index];
				break;
			case 3:
				name = g_SkillMasteryNameProperty_04_03[index];
				break;
			case 4:
				name = g_SkillMasteryNameProperty_04_04[index];
				break;
			case 5:
				name = g_SkillMasteryNameProperty_04_05[index];
				break;
			case 6:
				name = g_SkillMasteryNameProperty_04_06[index];
				break;
			case 7:
				name = g_SkillMasteryNameProperty_04_07[index];
				break;
			case 8:
				name = g_SkillMasteryNameProperty_04_08[index];
				break;
			case 9:
				name = g_SkillMasteryNameProperty_04_09[index];
				break;
			case 10:
				name = g_SkillMasteryNameProperty_04_10[index];
				break;
			case 11:
				name = g_SkillMasteryNameProperty_04_11[index];
				break;			
			}
		}
		break;	
	case _XGROUP_MOODANG :		// 무당
		{
			switch(cClass) 
			{
#ifdef _XTS_MASTERYOPTIONAL
			case 0 :
				name = g_SkillMasteryNameProperty_05_00;
				break;
#endif	
			case 1:
				name = g_SkillMasteryNameProperty_05_01[index];
				break;
			case 2:
				name = g_SkillMasteryNameProperty_05_02[index];
				break;
			case 3:
				name = g_SkillMasteryNameProperty_05_03[index];
				break;
			case 4:
				name = g_SkillMasteryNameProperty_05_04[index];
				break;
			case 5:
				name = g_SkillMasteryNameProperty_05_05[index];
				break;
			case 6:
				name = g_SkillMasteryNameProperty_05_06[index];
				break;
			case 7:
				name = g_SkillMasteryNameProperty_05_07[index];
				break;
			case 8:
				name = g_SkillMasteryNameProperty_05_08[index];
				break;
			case 9:
				name = g_SkillMasteryNameProperty_05_09[index];
				break;
			case 10:
				name = g_SkillMasteryNameProperty_05_10[index];
				break;
			case 11:
				name = g_SkillMasteryNameProperty_05_11[index];
				break;			
			}
		}
		break;
	case _XGROUP_MAKYO :	// 마교
		{
			switch(cClass) 
			{
#ifdef _XTS_MASTERYOPTIONAL
			case 0 :
				name = g_SkillMasteryNameProperty_06_00;
				break;
#endif	
			case 1:
				name = g_SkillMasteryNameProperty_06_01[index];
				break;
			case 2:
				name = g_SkillMasteryNameProperty_06_02[index];
				break;
			case 3:
				name = g_SkillMasteryNameProperty_06_03[index];
				break;
			case 4:
				name = g_SkillMasteryNameProperty_06_04[index];
				break;
			case 5:
				name = g_SkillMasteryNameProperty_06_05[index];
				break;
			case 6:
				name = g_SkillMasteryNameProperty_06_06[index];
				break;
			case 7:
				name = g_SkillMasteryNameProperty_06_07[index];
				break;
			case 8:
				name = g_SkillMasteryNameProperty_06_08[index];
				break;
			case 9:
				name = g_SkillMasteryNameProperty_06_09[index];
				break;
			case 10:
				name = g_SkillMasteryNameProperty_06_10[index];
				break;
			case 11:
				name = g_SkillMasteryNameProperty_06_11[index];
				break;			
			}
		}
		break;
	}

	return name;
}

bool _XWindow_MasterySkill::GetMasterySkillProp(int index)
{
	for( int i = 0; i < 24; i++ )
	{
		if( m_MasterySkillSlot[i].skillid == index )
		{
			if( m_MasterySkillSlot[i].prop == _XMS_PROP_CANNOTLEARN )
				return false;
			else
				return true;
		}
	}
	return false;
}

#ifdef _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI

void _XWindow_MasterySkill::GetMasterySkillName()
{
	if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
	{
		// 낭인 (1page)
		int index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex, 0, 0, 1);
		if(index >= 0)
		{
			m_MasteryClassTab[0]->SetButtonTextID(_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, g_SkillMasteryManager.GetMasteryNameId(index));
			m_MasteryClassTab[0]->ShowWindow(TRUE);
			m_MasteryClassTab[0]->EnableWindow(TRUE);
		}

		for(int i = 1 ; i < 4 ; i++)
		{
			m_MasteryClassTab[i]->ShowWindow(FALSE);
		}
	}
	else
	{
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 0)
		{
			// 문파 가입, 직책 없음 (1page) : 전투형 기본으로 열기
			int index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex, 
				1, 1, 1);
			if(index >= 0)
			{
				m_MasteryClassTab[0]->SetButtonTextID(_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, g_SkillMasteryManager.GetMasteryNameId(index));
				m_MasteryClassTab[0]->ShowWindow(TRUE);
				m_MasteryClassTab[0]->EnableWindow(TRUE);
			}

			for(int i = 1 ; i < 4 ; i++)
			{
				m_MasteryClassTab[i]->ShowWindow(FALSE);
			}
		}
		else
		{
			if(g_pLocalUser->m_CharacterInfo.Get_cClassGrade() == 0 || g_pLocalUser->m_CharacterInfo.Get_cClassGrade() == 1)
			{
				// 문파 가입, 1차 직책 (1page)
				int index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex, 
					g_pLocalUser->m_CharacterInfo.Get_cClass(), g_pLocalUser->m_CharacterInfo.Get_cClassGrade(), 1);
				if(index >= 0)
				{
					m_MasteryClassTab[0]->SetButtonTextID(_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, g_SkillMasteryManager.GetMasteryNameId(index));
					m_MasteryClassTab[0]->ShowWindow(TRUE);
					m_MasteryClassTab[0]->EnableWindow(TRUE);
				}

				for(int i = 1 ; i < 4 ; i++)
				{
					m_MasteryClassTab[i]->ShowWindow(FALSE);
				}
			}
			else
			{
				// 문파 가입, 2차 직책 = 문파 기본 + 2차 직책1 + 2차 직책 2(있을 경우) = 3age
				int index = -1;
				 
				index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex, 
					g_pLocalUser->m_CharacterInfo.Get_cClass(), 1, 1);

				if(index >= 0)
				{
					m_MasteryClassTab[0]->SetButtonTextID(_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, g_SkillMasteryManager.GetMasteryNameId(index));
					m_MasteryClassTab[0]->ShowWindow(TRUE);
					m_MasteryClassTab[0]->EnableWindow(TRUE);
				}

				index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex, 
					g_pLocalUser->m_CharacterInfo.Get_cClass(), g_pLocalUser->m_CharacterInfo.Get_cClassGrade(), 2);

				if(index >= 0)
				{
					m_MasteryClassTab[1]->SetButtonTextID(_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, g_SkillMasteryManager.GetMasteryNameId(index));
					m_MasteryClassTab[1]->ShowWindow(TRUE);
					m_MasteryClassTab[1]->EnableWindow(TRUE);
				}

				index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex, 
					g_pLocalUser->m_CharacterInfo.Get_cClass(), g_pLocalUser->m_CharacterInfo.Get_cClassGrade(), 3);

				if(index >= 0)
				{
					m_MasteryClassTab[2]->SetButtonTextID(_XSTRINGDB_SN_MASTERY_TABLE_XMS_NAME, g_SkillMasteryManager.GetMasteryNameId(index));
					m_MasteryClassTab[2]->ShowWindow(TRUE);
					m_MasteryClassTab[2]->EnableWindow(TRUE);
				}
				else
				{
					m_MasteryClassTab[2]->ShowWindow(FALSE);
				}

				for(int i = 3 ; i < 4 ; i++)
				{
					m_MasteryClassTab[i]->ShowWindow(FALSE);
				}
			}
		}
	}
}

#else

void _XWindow_MasterySkill::GetMasterySkillName(void)
{
	TCHAR* name = NULL;

#ifdef _XTS_MASTERYOPTIONAL
	if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
	{
		// 낭인 - 공통
		name = g_SkillMasteryNameProperty_00_00;
		if(name)
		{
			m_MasteryClassTab[0]->SetButtonText(name);
			m_MasteryClassTab[0]->ShowWindow(TRUE);
			m_MasteryClassTab[0]->EnableWindow(TRUE);
		}

		for(int i = 1 ; i < 4 ; i++)
		{
			m_MasteryClassTab[i]->ShowWindow(FALSE);
		}
	}
	else
	{
		// 가입 후
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 0)
		{
			// 직책 없음
			// 문파 - 공통
			name = GetMasterySkillTypeName(g_pLocalUser->m_CharacterInfo.Get_cClass(), 0);
			if(name)
			{
				m_MasteryClassTab[0]->SetButtonText(name);
				m_MasteryClassTab[0]->ShowWindow(TRUE);
				m_MasteryClassTab[0]->EnableWindow(TRUE);
			}

			for(int i = 1 ; i < 4 ; i++)
			{
				m_MasteryClassTab[i]->ShowWindow(FALSE);
			}
		}
		else
		{
			// 직책 있음
			
			// 문파 - 공통
			name = GetMasterySkillTypeName(0, 0);
			if(name)
			{
				m_MasteryClassTab[0]->SetButtonText(name);
				m_MasteryClassTab[0]->ShowWindow(TRUE);
				m_MasteryClassTab[0]->EnableWindow(TRUE);
			}

			for(int i = 1 ; i < 4 ; i++)
			{
				m_MasteryClassTab[i]->ShowWindow(FALSE);
			}

			if(g_pLocalUser->m_bViewMasterySkill)
			{
				for(int i = 0 ; i < 3 ; i++)
				{
					// 문파 - 직책오의
					name = GetMasterySkillTypeName(g_pLocalUser->m_CharacterInfo.Get_cClass(), i);
					if(name)
					{
						m_MasteryClassTab[i+1]->SetButtonText(name);
						m_MasteryClassTab[i+1]->ShowWindow(TRUE);
						m_MasteryClassTab[i+1]->EnableWindow(TRUE);
					}
					else
					{
						m_MasteryClassTab[i+1]->ShowWindow(FALSE);
					}
				}
			}
		}
	}
#else
	if(g_pLocalUser->m_CharacterInfo.groupindex > 0 && g_pLocalUser->m_CharacterInfo.Get_cClass() > 0)
	{
		// 문파 가입 && 직책 있음

		// 기본 - 공통
		name = g_SkillMasteryNameProperty_00_00;
		if(name)
		{
			m_MasteryClassTab[0]->SetButtonText(name);
			m_MasteryClassTab[0]->ShowWindow(TRUE);
			m_MasteryClassTab[0]->EnableWindow(TRUE);
		}

#ifdef _XTS_MASTERY
		if(g_pLocalUser->m_bViewMasterySkill)
		{
			for(int i = 0 ; i < 3 ; i++)
			{
				// 문파 - 직책오의
				name = GetMasterySkillTypeName(g_pLocalUser->m_CharacterInfo.Get_cClass(), i);
				if(name)
				{
					m_MasteryClassTab[i+1]->SetButtonText(name);
					m_MasteryClassTab[i+1]->ShowWindow(TRUE);
					m_MasteryClassTab[i+1]->EnableWindow(TRUE);
				}
				else
				{
					m_MasteryClassTab[i+1]->ShowWindow(FALSE);
				}
			}
		}
		else
		{
			for(int i = 1 ; i < 4 ; i++)
			{
				m_MasteryClassTab[i]->ShowWindow(FALSE);
			}
		}
#else
		for(int i = 1 ; i < 4 ; i++)
		{
			m_MasteryClassTab[i]->ShowWindow(FALSE);
		}
#endif

	}
	else
	{
		// 문파 가입전이거나 직책 없음

		// 기본 - 공통
		name = g_SkillMasteryNameProperty_00_00;

		if(name)
		{
			m_MasteryClassTab[0]->SetButtonText(name);
			m_MasteryClassTab[0]->ShowWindow(TRUE);
			m_MasteryClassTab[0]->EnableWindow(TRUE);
		}

		for(int i = 1 ; i < 4 ; i++)
		{
			m_MasteryClassTab[i]->ShowWindow(FALSE);
		}
	}
#endif
}

#endif //_XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI


#ifdef _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI

void _XWindow_MasterySkill::ParseMasterySkill()
{
	memset(m_MasterySkillSlot, 0, sizeof(m_MasterySkillSlot));

	int index = -1;
	if(m_SelectedTab == 0)
	{
		if(g_pLocalUser->m_CharacterInfo.groupindex != 0)
		{
			if(g_pLocalUser->m_CharacterInfo.Get_cClass() != 0)
			{
				// 가입-직책 있음 -> 직책별 기본으로 열기
				index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex, 
					g_pLocalUser->m_CharacterInfo.Get_cClass(), 1, 1);
			}
			else
			{
				// 가입-직책 없음 -> 전투형 기본으로 열기
				index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex, 1, 1, 1);
			}
		}
		else
		{
			// 낭인은 낭인 페이지
			index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex,
				g_pLocalUser->m_CharacterInfo.Get_cClass(), 0, 1);
		}
	}
	else
	{
		index = g_SkillMasteryManager.GetMasteryDisplayTableIndex(g_pLocalUser->m_CharacterInfo.groupindex,
			g_pLocalUser->m_CharacterInfo.Get_cClass(), g_pLocalUser->m_CharacterInfo.Get_cClassGrade(), m_SelectedTab+1);
	}

	if(index < 0)
		return;

	for(int i = 0 ; i < 24 ; ++i)
	{
		m_MasterySkillSlot[i].skillid = g_SkillMasteryManager.GetSkillIDFromMasteryDisplayTable(index+i);
		m_MasterySkillSlot[i].needprev = g_SkillMasteryManager.GetNeedprevFromMasteryDisplayTable(index+i);
		m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);

		m_MasterySkillSlot[i].position.x = 15 + 76*(i%4);
		m_MasterySkillSlot[i].position.y = 48 + 52*(i/4);

		_XDWINPRINT("mastery : %d / %d", m_MasterySkillSlot[i].skillid, m_MasterySkillSlot[i].needprev);
	}	
}

#else

void _XWindow_MasterySkill::ParseMasterySkill(void)
{
	memset(m_MasterySkillSlot, 0, sizeof(_XMasterySkillSlot)*24);

	if(m_SelectedTab == 0)
	{
		for(int i = 0 ; i < 24 ; i++)
		{
#ifdef _XTS_MASTERYOPTIONAL
			if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NANGIN)
			{
				// 낭인 - 기본
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_00_00[i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_00_00[i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
			else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG)
			{
				// 개방 - 기본
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_00[i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_00[i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
			else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
			{
				// 비궁 - 기본
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_00[i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_00[i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
			else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM)
			{
				// 소림 - 기본
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_00[i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_00[i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
			else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
			{
				// 녹림 - 기본
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_00[i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_00[i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
			else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG)
			{
				// 무당 - 기본
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_00[i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_00[i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
			else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO)
			{
				// 마교 - 기본
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_00[i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_00[i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
#else
			// 기본
			m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_00_00[i].skillid;
			m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_00_00[i].needprev;
			m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
#endif
		}
	}
	else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_GAEBANG)
	{
		// 개방 - 직책에 따른 오의 트리 
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 1)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_01[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_01[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 2)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_02[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_02[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 3)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_03[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_03[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 4)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_04[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_04[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 5)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_05[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_05[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 6)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_06[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_06[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 7)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_07[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_07[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 8)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_08[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_08[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 9)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_09[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_09[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 10)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_10[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_10[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 11)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_01_11[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_01_11[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
	}
	else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_BEEGOONG)
	{
		// 비궁 - 직책에 따른 오의 트리 
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 1)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_01[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_01[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 2)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_02[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_02[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 3)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_03[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_03[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 4)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_04[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_04[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 5)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_05[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_05[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 6)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_06[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_06[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 7)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_07[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_07[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 8)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_08[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_08[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 9)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_09[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_09[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 10)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_10[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_10[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 11)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_02_11[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_02_11[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
	}
	else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_SORIM)
	{
		// 소림 - 직책에 따른 오의 트리 
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 1)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_01[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_01[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 2)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_02[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_02[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 3)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_03[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_03[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 4)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_04[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_04[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 5)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_05[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_05[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 6)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_06[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_06[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 7)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_07[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_07[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 8)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_08[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_08[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 9)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_09[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_09[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 10)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_10[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_10[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 11)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_03_11[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_03_11[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
	}
	else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_NOCKRIM)
	{
		// 녹림 - 직책에 따른 오의 트리 
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 1)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_01[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_01[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 2)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_02[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_02[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 3)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_03[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_03[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 4)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_04[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_04[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 5)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_05[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_05[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 6)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_06[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_06[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 7)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_07[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_07[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 8)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_08[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_08[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 9)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_09[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_09[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 10)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_10[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_10[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 11)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_04_11[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_04_11[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
	}
	else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MOODANG)
	{
		// 무당 - 직책에 따른 오의 트리 
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 1)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_01[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_01[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 2)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_02[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_02[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 3)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_03[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_03[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 4)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_04[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_04[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 5)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_05[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_05[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 6)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_06[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_06[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 7)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_07[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_07[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 8)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_08[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_08[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 9)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_09[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_09[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 10)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_10[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_10[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 11)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_05_11[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_05_11[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
	}
	else if(g_pLocalUser->m_CharacterInfo.groupindex == _XGROUP_MAKYO )
	{
		// 마교 - 직책에 따른 오의 트리 
		if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 1)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_01[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_01[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 2)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_02[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_02[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 3)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_03[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_03[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 4)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_04[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_04[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 5)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_05[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_05[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 6)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_06[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_06[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 7)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_07[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_07[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 8)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_08[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_08[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 9)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_09[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_09[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 10)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_10[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_10[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
		else if(g_pLocalUser->m_CharacterInfo.Get_cClass() == 11)
		{
			for(int i = 0 ; i < 24 ; i++)
			{
				m_MasterySkillSlot[i].skillid = g_SkillMasteryDisplayProperty_06_11[m_SelectedTab-1][i].skillid;
				m_MasterySkillSlot[i].needprev = g_SkillMasteryDisplayProperty_06_11[m_SelectedTab-1][i].needprev;
				m_MasterySkillSlot[i].learnedskillid = g_pLocalUser->GetLearnedMasterySkillIndex(m_MasterySkillSlot[i].skillid);
			}
		}
	}

	for(int i = 0 ; i < 24 ; i++)
	{
		m_MasterySkillSlot[i].position.x = 15 + 76*(i%4);
		m_MasterySkillSlot[i].position.y = 48 + 52*(i/4);
	}
}

#endif //_XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI

void _XWindow_MasterySkill::CheckDependency(void)
{
#ifdef _XDEF_MASTERY_TOTALPOINT_080313_KUKURI

	// total point를 전체 적용

	// reset
	m_TotalPoint = 0;
	for(int i = 0 ; i < 24 ; i++)
	{
		m_MasterySkillSlot[i].prev = -1;
		m_MasterySkillSlot[i].next = -1;
	}

	for(i = 0 ; i < _XDEF_MAX_LEARNMASTERYSKILL ; ++i)
	{
		if(g_pLocalUser->m_MasterySkillList[i].skillid > 0)
		{
			for(int detail = 0 ; detail < g_pLocalUser->m_MasterySkillList[i].cPoint ; ++detail)
			{
				m_TotalPoint += g_SkillMasteryProperty[g_pLocalUser->m_MasterySkillList[i].skillid]->detailInfo[detail].sReqMp;
			}
		}
	}
#else
	// Reset
	m_TotalPoint = 0;
	for(int i = 0 ; i < 24 ; i++)
	{
		m_MasterySkillSlot[i].prev = -1;
		m_MasterySkillSlot[i].next = -1;

		if(m_MasterySkillSlot[i].skillid < 0)
			continue;

		if(m_MasterySkillSlot[i].learnedskillid >= 0)
		{
			for(int ii = 0 ; ii < g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint ; ii++)
			{
				m_TotalPoint += g_SkillMasteryProperty[m_MasterySkillSlot[i].skillid]->detailInfo[ii].sReqMp;
			}
		}
	}
#endif

	// prev setting
	for(i = 23 ; i >= 0 ; i--)
	{
		if(m_MasterySkillSlot[i].skillid < 0)
			continue;
		
		for(int k = 1 ; k < 6 ; k++)
		{
			if(i - (k*4) >= 0)
			{
				if(m_MasterySkillSlot[i].needprev == 1)
				{
					if(m_MasterySkillSlot[i].prev == -1)
					{
						if(m_MasterySkillSlot[i-(k*4)].skillid > 0)
						{
							m_MasterySkillSlot[i].prev = i - (k*4);
							m_MasterySkillSlot[i-(k*4)].next = i;
						}
					}
				}
			}
		}
	}

	// prop setting
	for(i = 0 ; i < 24 ; i++)
	{
		if(m_MasterySkillSlot[i].skillid < 0)
			continue;

		if(m_MasterySkillSlot[i].learnedskillid >= 0)
		{
			char maxMP = 20;
			if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[i].skillid))
				maxMP = g_SkillMasteryProperty[m_MasterySkillSlot[i].skillid]->cMaxMP;

			if(g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint == maxMP)
			{
				// 극성 달성
				m_MasterySkillSlot[i].prop = _XMS_PROP_OVER;
			}
			else if(g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint >= 0)
			{
				// 처리
				if(m_MasterySkillSlot[i].prev >= 0)
				{
					if( m_MasterySkillSlot[m_MasterySkillSlot[i].prev].prop == _XMS_PROP_UNDER ||
						m_MasterySkillSlot[m_MasterySkillSlot[i].prev].prop == _XMS_PROP_NOTLEARN ||
						m_MasterySkillSlot[m_MasterySkillSlot[i].prev].prop == _XMS_PROP_CANNOTLEARN )
					{
						// 선행 스킬 만족하지 않음
						m_MasterySkillSlot[i].prop = _XMS_PROP_CANNOTLEARN;
					}
					else
					{
						short needPageTotalPoint = 0;
						if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[i].skillid))
							needPageTotalPoint = g_SkillMasteryProperty[m_MasterySkillSlot[i].skillid]->sNeedPageTotalPoint;

						if(m_TotalPoint >= needPageTotalPoint)
						{
							// 총합 만족
							m_MasterySkillSlot[i].prop = _XMS_PROP_UNDER;
						}
						else
						{
							// 총합 만족하지 않음
							m_MasterySkillSlot[i].prop = _XMS_PROP_CANNOTLEARN;
						}
					}
				} // if(m_MasterySkillSlot[m_MasterySkillSlot[i].prev >= 0)
				else
				{
					short needPageTotalPoint = 0;
					if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[i].skillid))
						needPageTotalPoint = g_SkillMasteryProperty[m_MasterySkillSlot[i].skillid]->sNeedPageTotalPoint;

					// 선행 스킬이 없으므로 총합 체크
					if(m_TotalPoint >= needPageTotalPoint)
					{
						// 총합 만족
						m_MasterySkillSlot[i].prop = _XMS_PROP_UNDER;
					}
					else
					{
						// 총합 만족하지 않음
						m_MasterySkillSlot[i].prop = _XMS_PROP_CANNOTLEARN;
					}
				}
			} // if(g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint >= 0)
		}
		else
		{
			// 아예	배우지 않은 오의
			if(m_MasterySkillSlot[i].prev >= 0)
			{
				if( m_MasterySkillSlot[m_MasterySkillSlot[i].prev].prop == _XMS_PROP_UNDER ||
					m_MasterySkillSlot[m_MasterySkillSlot[i].prev].prop == _XMS_PROP_NOTLEARN ||
					m_MasterySkillSlot[m_MasterySkillSlot[i].prev].prop == _XMS_PROP_CANNOTLEARN )
				{
					// 선행 스킬 만족하지 않음
					m_MasterySkillSlot[i].prop = _XMS_PROP_CANNOTLEARN;
				}
				else
				{
					short needPageTotalPoint = 0;
					if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[i].skillid))
						needPageTotalPoint = g_SkillMasteryProperty[m_MasterySkillSlot[i].skillid]->sNeedPageTotalPoint;

					if(m_TotalPoint >= needPageTotalPoint)
					{
						// 총합 만족
						m_MasterySkillSlot[i].prop = _XMS_PROP_NOTLEARN;
					}
					else
					{
						// 총합 만족하지 않음
						m_MasterySkillSlot[i].prop = _XMS_PROP_CANNOTLEARN;
					}
				}
			} // if(m_MasterySkillSlot[m_MasterySkillSlot[i].prev >= 0)
			else
			{
				// 선행 스킬이 없으므로 총합 체크
				short needPageTotalPoint = 0;
				if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[i].skillid))
					needPageTotalPoint = g_SkillMasteryProperty[m_MasterySkillSlot[i].skillid]->sNeedPageTotalPoint;

				if(m_TotalPoint >= needPageTotalPoint)
				{
					// 총합 만족
					m_MasterySkillSlot[i].prop = _XMS_PROP_NOTLEARN;
				}
				else
				{
					// 총합 만족하지 않음
					m_MasterySkillSlot[i].prop = _XMS_PROP_CANNOTLEARN;
				}
			}
		} // 아예 배우지 않은 오의
	}
}

void _XWindow_MasterySkill::DrawMasterySkill(void)
{
	int x = 0, starty = 0, sizey = 0;
	bool bValid;
	
	for(int i = 0 ; i < 24 ; i++)
	{
		// Arrow / Line
		if(m_MasterySkillSlot[i].skillid >= 0)
		{
			x = m_WindowPosition.x + m_MasterySkillSlot[i].position.x + 28;
			starty = m_WindowPosition.y + m_MasterySkillSlot[i].position.y + 40;
			if(m_MasterySkillSlot[i].next >= 0)
				sizey = m_WindowPosition.y + m_MasterySkillSlot[m_MasterySkillSlot[i].next].position.y - starty;
			else 
				sizey = 0;

			if(m_MasterySkillSlot[i].prop == _XMS_PROP_OVER || m_MasterySkillSlot[i].prop == _XMS_PROP_UNDER)
				bValid = true;
			else
				bValid = false;

		}

		if(m_MasterySkillSlot[i].needprev)
		{
			m_ArrowImage->Draw(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+28, m_WindowPosition.y+m_MasterySkillSlot[i].position.y-9);
		}

		if(bValid)
		{
			m_ValidLineImage->SetScale(1.0f, (FLOAT)sizey);
			m_ValidLineImage->Draw(x, starty);
		}
		else
		{
			m_InvalidLineImage->SetScale(1.0f, (FLOAT)sizey);
			m_InvalidLineImage->Draw(x, starty);
		}

		// Image / Icon
		if(m_MasterySkillSlot[i].skillid < 0)
			continue;

		switch(m_MasterySkillSlot[i].prop)
		{
		case _XMS_PROP_OVER :
			{
				m_SlotImage[3]->Draw(m_WindowPosition.x+m_MasterySkillSlot[i].position.x, m_WindowPosition.y+m_MasterySkillSlot[i].position.y);
				DrawMasterySkillIcon(m_MasterySkillSlot[i].skillid, m_WindowPosition.x+m_MasterySkillSlot[i].position.x+5, 
					m_WindowPosition.y+m_MasterySkillSlot[i].position.y+5, true, true);
				g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF222222);
								
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SetColor(0xFFF1CB66);
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+62, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+23, 1.0f,
						_XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint);
				}
				else 
				{
					g_XBaseFont->SetColor(0xFFF1CB66);
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+50, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+23, 1.0f,
											_XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint);
					g_XBaseFont->SetColor(0xFFB6B6B6);
					g_XBaseFont->Puts(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+50, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+22, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG));
				}	
				
				
				g_XBaseFont->Flush();
				g_XBaseFont->DisableGrowMode();
			}
			break;
		case _XMS_PROP_UNDER :
			{
				m_SlotImage[0]->Draw(m_WindowPosition.x+m_MasterySkillSlot[i].position.x, m_WindowPosition.y+m_MasterySkillSlot[i].position.y);
				DrawMasterySkillIcon(m_MasterySkillSlot[i].skillid, m_WindowPosition.x+m_MasterySkillSlot[i].position.x+5, 
					m_WindowPosition.y+m_MasterySkillSlot[i].position.y+5, true, true);
				g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF222222);

				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SetColor(0xFFE89850);
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+62, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+23, 1.0f,
						_XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint);
				}
				else
				{
					g_XBaseFont->SetColor(0xFFE89850);
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+50, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+23, 1.0f,
						_XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint);

					g_XBaseFont->SetColor(0xFFB6B6B6);
					g_XBaseFont->Puts(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+50, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+22, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG));
				}
				
				g_XBaseFont->Flush();
				g_XBaseFont->DisableGrowMode();
			}
			break;
		case _XMS_PROP_NOTLEARN :
			{
				m_SlotImage[1]->Draw(m_WindowPosition.x+m_MasterySkillSlot[i].position.x, m_WindowPosition.y+m_MasterySkillSlot[i].position.y);
				DrawMasterySkillIcon(m_MasterySkillSlot[i].skillid, m_WindowPosition.x+m_MasterySkillSlot[i].position.x+5, 
					m_WindowPosition.y+m_MasterySkillSlot[i].position.y+5, true, true);
				g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF222222);
								
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SetColor(0xFFB6B6B6);
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+62, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+23, 1.0f,
						_XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint);
				}
				else
				{
					g_XBaseFont->SetColor(0xFFB6B6B6);
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+50, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+23, 1.0f,
						_XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint);

					g_XBaseFont->SetColor(0xFFB6B6B6);
					g_XBaseFont->Puts(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+50, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+22, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG) );
				}				
				
				g_XBaseFont->Flush();
				g_XBaseFont->DisableGrowMode();
			}
			break;
		case _XMS_PROP_CANNOTLEARN :
			{
				m_SlotImage[2]->Draw(m_WindowPosition.x+m_MasterySkillSlot[i].position.x, m_WindowPosition.y+m_MasterySkillSlot[i].position.y);
				DrawMasterySkillIcon(m_MasterySkillSlot[i].skillid, m_WindowPosition.x+m_MasterySkillSlot[i].position.x+5, 
					m_WindowPosition.y+m_MasterySkillSlot[i].position.y+5, false, true);
				g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF181818);
								
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SetColor(0xFF454545);
					g_XBaseFont->PrintAlign(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+62, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+23, 1.0f,
						_XFONT_ALIGNTYPE_RIGHT, "%d", g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[i].learnedskillid].cPoint);
				}
				else
				{
					g_XBaseFont->SetColor(0xFF454545);
					g_XBaseFont->Puts(m_WindowPosition.x+m_MasterySkillSlot[i].position.x+50, m_WindowPosition.y+m_MasterySkillSlot[i].position.y+22, _XGETINTERFACETEXT(ID_STRING_MASTERYSKILL_SUNG));
				}				
				
				g_XBaseFont->Flush();
				g_XBaseFont->DisableGrowMode();
			}
			break;
		}
	}
}

void _XWindow_MasterySkill::DrawTooltip(void)
{
 	if( g_bShowItemMallWindow ) return;

	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	for(int i = 0 ; i < 24 ; i++)
	{

		if(scrnpos->x > m_WindowPosition.x+m_MasterySkillSlot[i].position.x &&
			scrnpos->x < m_WindowPosition.x+m_MasterySkillSlot[i].position.x + 66 &&
			scrnpos->z > m_WindowPosition.y+m_MasterySkillSlot[i].position.y &&
			scrnpos->z < m_WindowPosition.y+m_MasterySkillSlot[i].position.y + 40)
		{
			if(m_MasterySkillSlot[i].skillid >= 0)
			{
				if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_MASTERYSKILL, 0, 0, m_MasterySkillSlot[i].skillid ) )
				{
					g_pDefaultTooltip_Window->ShowWindow( TRUE );
				}

				if(m_MasterySkillSlot[i].prop == _XMS_PROP_UNDER || m_MasterySkillSlot[i].prop == _XMS_PROP_NOTLEARN)
					m_SelectedImage->Draw(m_WindowPosition.x+m_MasterySkillSlot[i].position.x, m_WindowPosition.y+m_MasterySkillSlot[i].position.y);
			}
		}
	}
}

void _XWindow_MasterySkill::RefreshData(void)
{
	GetMasterySkillName();
	ChangeTab(0);
}

void _XWindow_MasterySkill::DrawMasterySkillIcon(short skillid, int X, int Y, bool bColor, bool smallicon)
{
#ifdef _XDEF_NEW_MASTERY_SYSTEM_080303_KUKURI

	int iconsubid = 0;

	if(_XSkillItem::FindMasterySkillProperty(skillid))
	{
		iconsubid = skillid;
	}
	else
	{
		_XDWINPRINT("[WARNING] Not found mastery skill table : %d", skillid);
		return;
	}

	int subindex = (iconsubid-1) / 64;
	int iconid = (iconsubid-1) % 64;

	if(iconid < 0)
		return;

	int resourceindex = -1;
	RECT rect;

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

	TCHAR resourcename[128];
	sprintf(resourcename, "Mastery_Icon_%02d_00_01.tga", subindex);
	resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);

	if(resourceindex != -1)
	{
		// resource 있음
		m_MasterySkillIcon.SetTexture(&g_MainInterfaceTextureArchive, resourceindex);
		m_MasterySkillIcon.DrawWithRegion(X, Y, rect);
	}
	else
	{
		// resource 없음 - 더미 icon 출력
		rect.left = 65;
		rect.top = 1;
		rect.right = 95;
		rect.bottom = 31;

		resourceindex = g_MainInterfaceTextureArchive.FindResource("Mastery_Icon_00.tga");
		m_MasterySkillIcon.SetTexture(&g_MainInterfaceTextureArchive, resourceindex);
		m_MasterySkillIcon.DrawWithRegion(X, Y, rect);
	}

	if(!bColor)
	{
		// 배울수 없는 오의의 경우 icon위에 gray layer 덮어줌
		rect.left = 1;
		rect.top = 1;
		rect.right = 31;
		rect.bottom = 31;
		m_MasterySkillGrayIcon.DrawWithRegion(X, Y, rect);
	}

#else

	char	cClass = 0;
	if(_XSkillItem::FindMasterySkillProperty(skillid))
	{
		cClass = g_SkillMasteryProperty[skillid]->cClass;
	}
	else
	{
		_XDWINPRINT("[WARNING] Not found mastery skill table : %d", skillid);
		return;
	}

	int iconsubid = skillid - (g_SkillMasteryProperty[skillid]->cClanType * 1000);
	int subindex = 1;
	int iconid = 0;
	if(cClass > 0)
		iconsubid -= (cClass - 1)*150;		// 역할별로 150개

	if(g_SkillMasteryProperty[skillid]->cMasteryType > 1)
		subindex = g_SkillMasteryProperty[skillid]->cMasteryType - 1;

	if(g_SkillMasteryProperty[skillid]->cMasteryType > 1)
		iconid = iconsubid - 50*(g_SkillMasteryProperty[skillid]->cMasteryType-2);
	else
		iconid = iconsubid;

	if(iconid < 0)
	{
		_XDWINPRINT("[WARNING] Invalid Icon id");
		return;
	}

	int		resourceindex = -1;
	RECT	rect;

	rect.left = ((iconid-1) % 8) * 32;
	rect.right = ((iconid-1) % 8 + 1) * 32;
	rect.top = ((iconid-1) / 8) * 32;
	rect.bottom = ((iconid-1) / 8 + 1) *32;
	
	if(smallicon)
	{
		rect.left  += 1;
		rect.top   += 1;
		rect.right -= 1;
		rect.bottom-= 1;
	}

	TCHAR resourcename[128];
	sprintf(resourcename, "Mastery_Icon_%02d_%02d_%02d.tga", g_SkillMasteryProperty[skillid]->cClanType, cClass, subindex);
	resourceindex = g_MainInterfaceTextureArchive.FindResource(resourcename);

	if(resourceindex != -1)
	{
		// resource 있음
		m_MasterySkillIcon.SetTexture(&g_MainInterfaceTextureArchive, resourceindex);
		m_MasterySkillIcon.DrawWithRegion(X, Y, rect);
	}
	else
	{
		// resource 없음 - 더미 icon 출력
		rect.left = 65;
		rect.top = 1;
		rect.right = 95;
		rect.bottom = 31;

		resourceindex = g_MainInterfaceTextureArchive.FindResource("Mastery_Icon_00.tga");
		m_MasterySkillIcon.SetTexture(&g_MainInterfaceTextureArchive, resourceindex);
		m_MasterySkillIcon.DrawWithRegion(X, Y, rect);
	}

	if(!bColor)
	{
		// 배울수 없는 오의의 경우 icon위에 gray layer 덮어줌
		rect.left = 1;
		rect.top = 1;
		rect.right = 31;
		rect.bottom = 31;
		m_MasterySkillGrayIcon.DrawWithRegion(X, Y, rect);
	}

#endif
}

void _XWindow_MasterySkill::DecisionMasteryPoint(void)
{
	if(m_MasterySkillSlot[m_iSelectedSlot].skillid >= 0)
	{
		if(m_MasterySkillSlot[m_iSelectedSlot].prop == _XMS_PROP_NOTLEARN)
		{
			if(g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() > 0)
			{				
				// Send Packet
				g_NetworkKernel.SendPacket(MSG_NO_MASTERY_POINT_USE, m_SelectedTab+1, m_MasterySkillSlot[m_iSelectedSlot].skillid);				
			}
		}
		else if(m_MasterySkillSlot[m_iSelectedSlot].prop == _XMS_PROP_UNDER || m_MasterySkillSlot[m_iSelectedSlot].prop == _XMS_PROP_OVER)
		{
			if(g_pLocalUser->m_CharacterInfo.Get_leftmasterypoint() > 0)
			{
				char maxMP = 20;
				if(_XSkillItem::FindMasterySkillProperty(m_MasterySkillSlot[m_iSelectedSlot].skillid))
					maxMP = g_SkillMasteryProperty[m_MasterySkillSlot[m_iSelectedSlot].skillid]->cMaxMP;

				if(g_pLocalUser->m_MasterySkillList[m_MasterySkillSlot[m_iSelectedSlot].learnedskillid].cPoint < maxMP)
				{
					// Send Packet
					g_NetworkKernel.SendPacket(MSG_NO_MASTERY_POINT_USE, m_SelectedTab+1, m_MasterySkillSlot[m_iSelectedSlot].skillid);					
				}
			}			
		}
	} // if(m_MasterySkillSlot[i].skillid >= 0)		

	RemoveTooltip();
}

void _XWindow_MasterySkill::RemoveTooltip(void)
{
	if( g_pDefaultTooltip_Window )
	{
		if( g_pDefaultTooltip_Window->GetShowStatus() )
			g_pDefaultTooltip_Window->ShowWindow( FALSE );
	}
	m_iSelectedSlot = -1;
	m_bMasteryPointClickable = FALSE; 
	m_WindowMouseState = _XW_STATE_NONE;
}

int _XWindow_MasterySkill::GetMasterySkillNameId()
{
	if(m_SelectedTab >= 0 && m_SelectedTab < 4)
	{
		return m_MasteryClassTab[m_SelectedTab]->GetButtonTextID();
	}

	return 0;
}
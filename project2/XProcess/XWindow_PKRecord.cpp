// XWindow_PKRecord.cpp: implementation of the _XWindow_PKRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_PKRecord.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_PKRecord::_XWindow_PKRecord()
{
	for(int i = 0 ; i < 5 ; i++)
	{
		m_RecordTab[i] = NULL;
	}
}

_XWindow_PKRecord::~_XWindow_PKRecord()
{

}

BOOL _XWindow_PKRecord::Initialize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_quest2.tga");

	_XCHECKBTN_STRUCTURE tabstruct = 
	{
		TRUE, {3, 10}, {66, 26}, _XDEF_PKRECORD_TAB_1,
		resourceindex, resourceindex, &g_MainInterfaceTextureArchive
	};

	for(int i = 0 ; i < 5 ; i++)
	{
		tabstruct.position.x = 3 + (i*70);
		m_RecordTab[i] = new _XCheckButton;
		m_RecordTab[i]->Create(tabstruct);
		
		m_RecordTab[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_NORMAL, 1, 229, 67, 255);
		m_RecordTab[i]->SetButtonImageClipRect(_XCHECKBUTTONIMAGE_CLICK, 67, 229, 133, 255);
		m_RecordTab[i]->SetCheck(FALSE);
		m_RecordTab[i]->EnableWindow(FALSE);
		InsertChildObject(m_RecordTab[i]);
		
		tabstruct.commandid++;
	}

	m_RecordTab[0]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2755));		// 대전전황
	m_RecordTab[1]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2756));		// 전적통계
	m_RecordTab[2]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2757));		// 영웅대전
	m_RecordTab[3]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2758));		// 문파대전
	m_RecordTab[4]->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_2759));		// 흑백대전

	m_RecordTab[0]->SetCheck(TRUE);
	m_RecordTab[0]->EnableWindow(TRUE);
	m_RecordTab[1]->EnableWindow(TRUE);

	m_LBigBorder.Create(0, 0, 219, 25, &g_MainInterfaceTextureArchive, resourceindex);
	m_LBigBorder.SetClipRect(0, 117, 219, 142);
	m_RBigBorder.Create(0, 0, 85, 25, &g_MainInterfaceTextureArchive, resourceindex);
	m_RBigBorder.SetClipRect(0, 143, 85, 166);

	m_LWinBorder.Create(0, 0, 50, 25, &g_MainInterfaceTextureArchive, resourceindex);
	m_LWinBorder.SetClipRect(0, 117, 50, 142);
	m_RWinBorder.Create(0, 0, 85, 25, &g_MainInterfaceTextureArchive, resourceindex);
	m_RWinBorder.SetClipRect(0, 143, 85, 166);

	m_LLoseBorder.Create(0, 0, 50, 25, &g_MainInterfaceTextureArchive, resourceindex);
	m_LLoseBorder.SetClipRect(0, 117, 50, 142);
	m_RLoseBorder.Create(0, 0, 85, 25, &g_MainInterfaceTextureArchive, resourceindex);
	m_RLoseBorder.SetClipRect(0, 143, 85, 166);

	int optionresource = g_MainInterfaceTextureArchive.FindResource("mi_system.tga");

	m_LWinRateBorder_u.Create(0, 0, 2, 45, &g_MainInterfaceTextureArchive, optionresource);
	m_LWinRateBorder_u.SetClipRect(194, 2, 196, 47);
	m_LWinRateBorder_d.Create(0, 0, 2, 3, &g_MainInterfaceTextureArchive, optionresource);
	m_LWinRateBorder_d.SetClipRect(194, 91, 196, 94);

	m_RWinRateBorder_u.Create(0, 0, 2, 45, &g_MainInterfaceTextureArchive, optionresource);
	m_RWinRateBorder_u.SetClipRect(194, 2, 196, 47);
	m_RWinRateBorder_u.SetScale(-1, 1);
	m_RWinRateBorder_d.Create(0, 0, 2, 3, &g_MainInterfaceTextureArchive, optionresource);
	m_RWinRateBorder_d.SetScale(-1, 1);
	m_RWinRateBorder_d.SetClipRect(194, 91, 196, 94);

	m_UWinRateBorder.Create(0, 0, 154, 2, &g_MainInterfaceTextureArchive, optionresource);
	m_UWinRateBorder.SetClipRect(2, 219, 156, 221);
	m_UWinRateBorder.SetScale(2.16f, 1.0f);

	m_DWinRateBorder.Create(0, 0, 154, 2, &g_MainInterfaceTextureArchive, optionresource);
	m_DWinRateBorder.SetClipRect(2, 223, 156, 225);
	m_DWinRateBorder.SetScale(2.16f, 1.0f);

	m_UGroupRateBorder_l.Create(0, 0, 18, 2, &g_MainInterfaceTextureArchive, optionresource);
	m_UGroupRateBorder_l.SetClipRect(2, 219, 20, 221);

	m_UGroupRateBorder_r.Create(0, 0, 14, 2, &g_MainInterfaceTextureArchive, optionresource);
	m_UGroupRateBorder_r.SetClipRect(142, 219, 156, 221);

	m_LGroupRateBorder.Create(0, 0, 2, 118, &g_MainInterfaceTextureArchive, optionresource);
	m_LGroupRateBorder.SetClipRect(188, 1, 190, 119);
	m_LGroupRateBorder.SetScale(1.0f, 1.75f);

	m_RGroupRateBorder.Create(0, 0, 2, 118, &g_MainInterfaceTextureArchive, optionresource);
	m_RGroupRateBorder.SetClipRect(188, 1, 190, 119);
	m_RGroupRateBorder.SetScale(-1.0f, 1.75f);

	m_LClassRateBorder.Create(0, 0, 2, 118, &g_MainInterfaceTextureArchive, optionresource);
	m_LClassRateBorder.SetClipRect(188, 1, 190, 119);
	m_LClassRateBorder.SetScale(1.0f, 1.06f);
	
	m_RClassRateBorder.Create(0, 0, 2, 118, &g_MainInterfaceTextureArchive, optionresource);
	m_RClassRateBorder.SetClipRect(188, 1, 190, 119);
	m_RClassRateBorder.SetScale(-1.0f, 1.06f);

	int skillresource = g_MainInterfaceTextureArchive.FindResource(g_MI_LifeSkillTextureName);

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		m_TableColumnBorder.Create(0, 0, 102, 20, &g_MainInterfaceTextureArchive, skillresource);
		m_TableColumnBorder.SetClipRect(0, 32, 102, 52);
	}
	else
	{
		m_TableColumnBorder.Create(0, 0, 64, 20, &g_MainInterfaceTextureArchive, skillresource);
		m_TableColumnBorder.SetClipRect(0, 32, 64, 52);
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		m_TableRowBorder.Create(0, 0, 55, 20, &g_MainInterfaceTextureArchive, skillresource);
		m_TableRowBorder.SetClipRect(0, 32, 55, 52);
	}
	else
	{
		m_TableRowBorder.Create(0, 0, 72, 20, &g_MainInterfaceTextureArchive, skillresource);
		m_TableRowBorder.SetClipRect(0, 32, 72, 52);
	}

	m_TableRateBorder.Create(0, 0, 37, 20, &g_MainInterfaceTextureArchive, skillresource);
	m_TableRateBorder.SetClipRect(0, 32, 37, 52);

	m_TableFinishBorder.Create(0, 0, 1, 20, &g_MainInterfaceTextureArchive, skillresource);
	m_TableFinishBorder.SetClipRect(203, 32, 204, 52);

	int partyresource = g_MainInterfaceTextureArchive.FindResource("mi_party.tga");

	for(i = 0 ; i < 6 ; i++)
	{
		m_GroupIcon[i].Create(0, 0, 15, 15, &g_MainInterfaceTextureArchive, partyresource);
		m_GroupIcon[i].SetClipRect(166+(i*15), 226, 181+(i*15), 241);
	}
	m_GroupIcon[6].Create(0, 0, 15, 15, &g_MainInterfaceTextureArchive, partyresource);
	m_GroupIcon[6].SetClipRect(166, 241, 181, 256);
	m_GroupIcon[7].Create(0, 0, 15, 15, &g_MainInterfaceTextureArchive, partyresource);
	m_GroupIcon[7].SetClipRect(181, 241, 196, 256);

	m_GroupIndex[0] = 1;
	m_GroupIndex[1] = 3;
	m_GroupIndex[2] = 5;
	m_GroupIndex[3] = 4;
	m_GroupIndex[4] = 2;
	m_GroupIndex[5] = 6;
	m_GroupIndex[6] = 0;
	m_GroupIndex[7] = 7;
	
	return TRUE;
}

void _XWindow_PKRecord::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_PKRecord::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	switch(m_CurrentSelectedTab)
	{
	case _XDEF_PKRECORD_TAB_1 :
		{
			DrawPKRecord();

			// tooltip
			if( !DrawTooltip() )
			{
				if( g_pMiniTooltip_Window->GetMiniTooltipMode() == MTMODE_PK_RECORD  )
					g_pMiniTooltip_Window->ShowWindow( FALSE );
			}
		}
		break;
	case _XDEF_PKRECORD_TAB_2 :
		{
			DrawTotalRecord();
		}
		break;
	}	
}
BOOL _XWindow_PKRecord::DrawTooltip(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if( scrnpos->x > m_WindowPosition.x+11 && scrnpos->x < m_WindowPosition.x+170 &&		 // Equiped item area
		scrnpos->z > m_WindowPosition.y+151 && scrnpos->z < m_WindowPosition.y+378 )
	{				
		if( g_pMiniTooltip_Window )
		{
			int selectid = -1;
			selectid = scrnpos->z - (m_WindowPosition.y+151);
			selectid /= 21;
			if( selectid != -1 )
			{
				if( m_WinRecord[selectid].option )
				{
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PK_RECORD, 0, 0, selectid ) )
					{
						g_pMiniTooltip_Window->ShowWindow( TRUE );
						return TRUE;
					}
				}				
			}			
		}
	}
	if( scrnpos->x > m_WindowPosition.x+176 && scrnpos->x < m_WindowPosition.x+335 &&		 // Equiped item area
		scrnpos->z > m_WindowPosition.y+151 && scrnpos->z < m_WindowPosition.y+378 )
	{
		if( g_pMiniTooltip_Window )
		{
			int selectid = -1;
			selectid = scrnpos->z - (m_WindowPosition.y+151);
			selectid /= 21;
			if( selectid != -1 )
			{
				if( m_LoseRecord[selectid].option )
				{
					if( g_pMiniTooltip_Window->SetTooltipMode( MTMODE_PK_RECORD, 0, 1, selectid ) )
					{
						g_pMiniTooltip_Window->ShowWindow( TRUE );
						return TRUE;
					}
				}				
			}			
		}
	}

	return FALSE;
}

BOOL _XWindow_PKRecord::Process(_XGUIObject*& pfocusobject)
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

void _XWindow_PKRecord::RefreshData(void)
{
	m_CurrentSelectedTab = _XDEF_PKRECORD_TAB_1;

	for(int i = 0 ; i < 5 ; i++)
	{
		m_RecordTab[i]->SetCheck(FALSE);
	}
	m_RecordTab[0]->SetCheck(TRUE);
}

void _XWindow_PKRecord::ChangeTab(int tab)
{
	m_CurrentSelectedTab = tab;

	for(int i = 0 ; i < 5 ; i++)
	{
		m_RecordTab[i]->SetCheck(FALSE);
	}

	m_RecordTab[tab -_XDEF_PKRECORD_TAB_1]->SetCheck(TRUE);
}

void _XWindow_PKRecord::DrawPKRecord(void)
{
	// Title
	m_LBigBorder.Draw(m_WindowPosition.x+23, m_WindowPosition.y+43);
	m_RBigBorder.Draw(m_WindowPosition.x+242, m_WindowPosition.y+43);

	m_LWinBorder.Draw(m_WindowPosition.x+23, m_WindowPosition.y+123);
	m_RWinBorder.Draw(m_WindowPosition.x+73, m_WindowPosition.y+123);

	m_LLoseBorder.Draw(m_WindowPosition.x+188, m_WindowPosition.y+123);
	m_RLoseBorder.Draw(m_WindowPosition.x+238, m_WindowPosition.y+123);

	// Font
	g_XBaseFont->SetBoldMode(TRUE);

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 227, 187, 29));
	g_XBaseFont->Puts(m_WindowPosition.x+146, m_WindowPosition.y+49, _XGETINTERFACETEXT(ID_STRING_NEW_2755));		// _T("대전전황")
	
	g_XBaseFont->Puts(m_WindowPosition.x+62, m_WindowPosition.y+129, _XGETINTERFACETEXT(ID_STRING_NEW_2760));		// _T("승전 기록")

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 253, 0, 0));
	g_XBaseFont->Puts(m_WindowPosition.x+227, m_WindowPosition.y+129, _XGETINTERFACETEXT(ID_STRING_NEW_2761));	// _T("패전 기록")

	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);

	// Border
	_XDrawSolidBar(m_WindowPosition.x+6, m_WindowPosition.y+72, m_WindowPosition.x+339, m_WindowPosition.y+116, 0x8F000000);

	// Lines
	m_LWinRateBorder_u.Draw(m_WindowPosition.x+4, m_WindowPosition.y+70);
	m_LWinRateBorder_d.Draw(m_WindowPosition.x+4, m_WindowPosition.y+115);

	m_RWinRateBorder_u.Draw(m_WindowPosition.x+340, m_WindowPosition.y+70);
	m_RWinRateBorder_d.Draw(m_WindowPosition.x+340, m_WindowPosition.y+115);

	m_UWinRateBorder.Draw(m_WindowPosition.x+6, m_WindowPosition.y+70);
	m_DWinRateBorder.Draw(m_WindowPosition.x+6, m_WindowPosition.y+116);

	// Border
	_XDrawSolidBar(m_WindowPosition.x+11, m_WindowPosition.y+151, m_WindowPosition.x+170, m_WindowPosition.y+378, 0x8F000000);
	_XDrawRectAngle(m_WindowPosition.x+10, m_WindowPosition.y+150, m_WindowPosition.x+170, m_WindowPosition.y+378, 0.0f, D3DCOLOR_ARGB(255,0,0,0));

	_XDrawSolidBar(m_WindowPosition.x+176, m_WindowPosition.y+151, m_WindowPosition.x+335, m_WindowPosition.y+378, 0x8F000000);
	_XDrawRectAngle(m_WindowPosition.x+175, m_WindowPosition.y+150, m_WindowPosition.x+335, m_WindowPosition.y+378, 0.0f, D3DCOLOR_ARGB(255,0,0,0));

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 227, 187, 29));
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->Puts(m_WindowPosition.x+106, m_WindowPosition.y+77, _XGETINTERFACETEXT(ID_STRING_NEW_2762));		// _T("전체승률")
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+136, m_WindowPosition.y+77, _XGETINTERFACETEXT(ID_STRING_NEW_2762));		// _T("전체승률")
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->Puts(m_WindowPosition.x+70, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2655));		// _T("전")
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+104, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2655));		// _T("전")
	}

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 209, 209, 209));
	if(m_TotalWinCount + m_TotalLoseCount > 0)
	{
		if(m_TotalWinRate < 50)
		{
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 253, 0, 0));
		}
		else if(m_TotalWinRate > 50)
		{
			g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 142, 241, 1));
		}
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+222, m_WindowPosition.y+77, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d%%", m_TotalWinRate);
	}
	else
	{
		g_XBaseFont->PrintAlign(m_WindowPosition.x+202, m_WindowPosition.y+77, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalWinRate);
		g_XBaseFont->Puts(m_WindowPosition.x+203, m_WindowPosition.y+77, "%");
	}	

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 209, 209, 209));

	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->Puts(m_WindowPosition.x+181, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2656));//_T("승"));
		
		g_XBaseFont->PrintAlign(m_WindowPosition.x+69, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalWinCount+m_TotalLoseCount);
		g_XBaseFont->PrintAlign(m_WindowPosition.x+180, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalWinCount);
		
		g_XBaseFont->Puts(m_WindowPosition.x+275, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2657));//_T("패"));
		g_XBaseFont->PrintAlign(m_WindowPosition.x+275, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalLoseCount);
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+215, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2656));//_T("승"));
		
		g_XBaseFont->PrintAlign(m_WindowPosition.x+103, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalWinCount+m_TotalLoseCount);
		g_XBaseFont->PrintAlign(m_WindowPosition.x+214, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalWinCount);

		g_XBaseFont->Puts(m_WindowPosition.x+309, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2657));//_T("패"));
		g_XBaseFont->PrintAlign(m_WindowPosition.x+309, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalLoseCount);
	}

/*	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->Puts(m_WindowPosition.x+302, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2657));//_T("패"));
		g_XBaseFont->PrintAlign(m_WindowPosition.x+302, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalLoseCount);
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+309, m_WindowPosition.y+99, _XGETINTERFACETEXT(ID_STRING_NEW_2657));//_T("패"));
		g_XBaseFont->PrintAlign(m_WindowPosition.x+309, m_WindowPosition.y+99, 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_TotalLoseCount);
	}
*/
	
	

	for(int i = 0 ; i < 10 ; i++)
	{
		if(m_WinRecord[i].option > 0)
		{
			if(GetRecentRecord(0, i))
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 186, 105, 186));
			}
			else
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 209, 209, 209));
			}

			if(m_WinRecord[i].option == 2 || m_WinRecord[i].option == 4)
			{
				g_XBaseFont->Print(m_WindowPosition.x+66, m_WindowPosition.y+160+(i*21), 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2659));	// _T("복면인")
				m_GroupIcon[7].Draw(m_WindowPosition.x+38, m_WindowPosition.y+158+(i*21)); 
			}
			else
			{
				g_XBaseFont->Print(m_WindowPosition.x+66, m_WindowPosition.y+160+(i*21), 1.0f, "%s", m_WinRecord[i].name);
				m_GroupIcon[m_WinRecord[i].group].Draw(m_WindowPosition.x+38, m_WindowPosition.y+158+(i*21));
			}
		}

		if(m_LoseRecord[i].option > 0)
		{
			if(GetRecentRecord(1, i))
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 186, 105, 105));
			}
			else
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 209, 209, 209));
			}

			if(m_LoseRecord[i].option == 2 || m_LoseRecord[i].option == 4)
			{
				g_XBaseFont->Print(m_WindowPosition.x+231, m_WindowPosition.y+160+(i*21), 1.0f, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2659));	// _T("복면인")
				m_GroupIcon[7].Draw(m_WindowPosition.x+203, m_WindowPosition.y+158+(i*21));
			}
			else
			{
				g_XBaseFont->Print(m_WindowPosition.x+231, m_WindowPosition.y+160+(i*21), 1.0f, "%s", m_LoseRecord[i].name);
				m_GroupIcon[m_LoseRecord[i].group].Draw(m_WindowPosition.x+203, m_WindowPosition.y+158+(i*21));
			}
		}
	}

	g_XBaseFont->Flush();
}

void _XWindow_PKRecord::DrawTotalRecord(void)
{
	// Border
	_XDrawSolidBar(m_WindowPosition.x+6, m_WindowPosition.y+50, m_WindowPosition.x+339, m_WindowPosition.y+252, 0x8F000000);
	_XDrawSolidBar(m_WindowPosition.x+6, m_WindowPosition.y+262, m_WindowPosition.x+339, m_WindowPosition.y+384, 0x8F000000);

	// Title
	
	m_LBigBorder.Draw(m_WindowPosition.x+23, m_WindowPosition.y+43);
	m_RBigBorder.Draw(m_WindowPosition.x+242, m_WindowPosition.y+43);

	m_LBigBorder.Draw(m_WindowPosition.x+23, m_WindowPosition.y+255);
	m_RBigBorder.Draw(m_WindowPosition.x+242, m_WindowPosition.y+255);

	// Lines
	m_UGroupRateBorder_l.Draw(m_WindowPosition.x+5, m_WindowPosition.y+48);
	m_UGroupRateBorder_r.Draw(m_WindowPosition.x+325, m_WindowPosition.y+48);
	
	m_UGroupRateBorder_l.Draw(m_WindowPosition.x+5, m_WindowPosition.y+261);
	m_UGroupRateBorder_r.Draw(m_WindowPosition.x+325, m_WindowPosition.y+261);

	m_LGroupRateBorder.Draw(m_WindowPosition.x+4, m_WindowPosition.y+48);
	m_RGroupRateBorder.Draw(m_WindowPosition.x+340, m_WindowPosition.y+48);

	m_LClassRateBorder.Draw(m_WindowPosition.x+4, m_WindowPosition.y+261);
	m_RClassRateBorder.Draw(m_WindowPosition.x+340, m_WindowPosition.y+261);

	m_DWinRateBorder.Draw(m_WindowPosition.x+6, m_WindowPosition.y+252);
	m_DWinRateBorder.Draw(m_WindowPosition.x+6, m_WindowPosition.y+384);
	

	// Title Font
	g_XBaseFont->SetBoldMode(TRUE);

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 227, 187, 29));
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+49, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2763));			// _T("문파전적")
	g_XBaseFont->PutsAlign(m_WindowPosition.x+(m_WindowSize.cx>>1), m_WindowPosition.y+261,_XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2764));			// _T("직책별 전적")

	g_XBaseFont->Flush();
	g_XBaseFont->SetBoldMode(FALSE);

	// Table
	for(int i = 0 ; i < 9 ; i++)
	{
		m_TableColumnBorder.Draw(m_WindowPosition.x+12, m_WindowPosition.y+68+(i*20));
		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
		{
			m_TableFinishBorder.Draw(m_WindowPosition.x+114, m_WindowPosition.y+68+(i*20));
		}
		else
		{
			m_TableFinishBorder.Draw(m_WindowPosition.x+76, m_WindowPosition.y+68+(i*20));
		}
	}

	for(i = 0 ; i < 5 ; i++)
	{
		m_TableColumnBorder.Draw(m_WindowPosition.x+12, m_WindowPosition.y+280+(i*20));

		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
		{
			m_TableFinishBorder.Draw(m_WindowPosition.x+114, m_WindowPosition.y+280+(i*20));
		}
		else
		{
			m_TableFinishBorder.Draw(m_WindowPosition.x+76, m_WindowPosition.y+280+(i*20));
		}
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		int xpos = 114;
		
		for(i = 0 ; i < 4 ; i++)
		{
			m_TableRowBorder.Draw(m_WindowPosition.x+xpos, m_WindowPosition.y+68);
			m_TableFinishBorder.Draw(m_WindowPosition.x+xpos+55, m_WindowPosition.y+68);
			
			m_TableRowBorder.Draw(m_WindowPosition.x+xpos, m_WindowPosition.y+280);
			m_TableFinishBorder.Draw(m_WindowPosition.x+xpos+55, m_WindowPosition.y+280);

			xpos += 55;
		}		
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		int xpos = 71+43;
		
		for(i = 0 ; i < 4 ; i++)
		{
			if( i == 1 || i == 2 )
			{
				m_TableRateBorder.Draw(m_WindowPosition.x+xpos, m_WindowPosition.y+68);
				m_TableFinishBorder.Draw(m_WindowPosition.x+xpos+37, m_WindowPosition.y+68);
				
				m_TableRateBorder.Draw(m_WindowPosition.x+xpos, m_WindowPosition.y+280);
				m_TableFinishBorder.Draw(m_WindowPosition.x+xpos+37, m_WindowPosition.y+280);
				
				xpos += 37;
			}
			else
			{
				m_TableRowBorder.Draw(m_WindowPosition.x+xpos, m_WindowPosition.y+68);
				m_TableFinishBorder.Draw(m_WindowPosition.x+xpos+72, m_WindowPosition.y+68);
				
				m_TableRowBorder.Draw(m_WindowPosition.x+xpos, m_WindowPosition.y+280);
				m_TableFinishBorder.Draw(m_WindowPosition.x+xpos+72, m_WindowPosition.y+280);
				
				xpos += 73;
			}
		}		
	}
	else
	{
		for(i = 0 ; i < 3 ; i++)
		{
			m_TableRowBorder.Draw(m_WindowPosition.x+77+(i*73), m_WindowPosition.y+68);
			m_TableFinishBorder.Draw(m_WindowPosition.x+149+(i*73), m_WindowPosition.y+68);

			m_TableRowBorder.Draw(m_WindowPosition.x+77+(i*73), m_WindowPosition.y+280);
			m_TableFinishBorder.Draw(m_WindowPosition.x+149+(i*73), m_WindowPosition.y+280);
		}

		m_TableRateBorder.Draw(m_WindowPosition.x+296, m_WindowPosition.y+68);
		m_TableFinishBorder.Draw(m_WindowPosition.x+333, m_WindowPosition.y+68);

		m_TableRateBorder.Draw(m_WindowPosition.x+296, m_WindowPosition.y+280);
		m_TableFinishBorder.Draw(m_WindowPosition.x+333, m_WindowPosition.y+280);
	}

	for(i = 0 ; i < 7 ; i++)
	{
		_XDrawLine2D(m_WindowPosition.x+79, m_WindowPosition.y+108+(i*20), m_WindowPosition.x+334, m_WindowPosition.y+108+(i*20),
			D3DCOLOR_ARGB(255, 116, 116, 116), D3DCOLOR_ARGB(255, 116, 116, 116));
	}
	
	for(i = 0 ; i < 3 ; i++)
	{
		_XDrawLine2D(m_WindowPosition.x+79, m_WindowPosition.y+320+(i*20), m_WindowPosition.x+334, m_WindowPosition.y+320+(i*20), 
			D3DCOLOR_ARGB(255, 116, 116, 116), D3DCOLOR_ARGB(255, 116, 116, 116));
	}

	// Font
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 227, 187, 29));
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+114+55, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2655));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+187, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2655));
	}
	else
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+149, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2655));
	}
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+114+55+55, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2656));
	}
	else
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+223, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2656));
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+114+55, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2655));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+187, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2655));
	}
	else
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+149, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2655));
	}	
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+114+55+55, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2656));
	}
	else
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+223, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2656));
	}
	g_XBaseFont->Flush();

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 209, 209, 209));
	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+114+55+55+55, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2657));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+260, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2657));
	}
	else
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+297, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2657));
	}
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+114+55+55+55+55, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2658));
	}
	else 
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+330, m_WindowPosition.y+72, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2658));
	}

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+114+55+55+55, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2657));
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN ) 
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+260, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2657));
	}
	else
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+297, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2657));
	}
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+114+55+55+55+55, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2658));
	}
	else 
	{
		g_XBaseFont->PutsAlign(m_WindowPosition.x+330, m_WindowPosition.y+284, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_NEW_2658));
	}
	g_XBaseFont->Flush();

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 186, 186, 186));

	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->SetClippingArea( m_WindowPosition.x + 12, m_WindowPosition.y+93, m_WindowPosition.x + 71+43, m_WindowPosition.y+400 );

		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+93,  _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+113, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+133, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+153, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+173, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+193, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+213, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+233, _XGETINTERFACETEXT(ID_STRING_NEW_2659));
		
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+305, _XGETINTERFACETEXT(ID_STRING_NEW_2660));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+325, _XGETINTERFACETEXT(ID_STRING_NEW_2661));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+345, _XGETINTERFACETEXT(ID_STRING_NEW_2662));
		g_XBaseFont->Puts(m_WindowPosition.x+13, m_WindowPosition.y+365, _XGETINTERFACETEXT(ID_STRING_NEW_2663));

		g_XBaseFont->Flush();
		g_XBaseFont->DisableClippingArea();
	}
	else
	{
		g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+93,  _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01));
		g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+113, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03));
		g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+133, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05));
		g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+153, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04));
		g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+173, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02));
		g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+193, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06));
		g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+213, _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00));
		g_XBaseFont->Puts(m_WindowPosition.x+36, m_WindowPosition.y+233, _XGETINTERFACETEXT(ID_STRING_NEW_2659));
		
		g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+305, _XGETINTERFACETEXT(ID_STRING_NEW_2660));
		g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+325, _XGETINTERFACETEXT(ID_STRING_NEW_2661));
		g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+345, _XGETINTERFACETEXT(ID_STRING_NEW_2662));
		g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+365, _XGETINTERFACETEXT(ID_STRING_NEW_2663));
	}

	FLOAT fRate = 0;
	int rate = 0;

	// 문파 전적
	for(i = 0 ; i < 8 ; i++)
	{
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 186, 186, 186));

		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+114+55, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_GroupRecord[m_GroupIndex[i]].win+m_GroupRecord[m_GroupIndex[i]].lose);
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+187, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_GroupRecord[m_GroupIndex[i]].win+m_GroupRecord[m_GroupIndex[i]].lose);
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+149, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_GroupRecord[m_GroupIndex[i]].win+m_GroupRecord[m_GroupIndex[i]].lose);
		}
		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+114+55+55, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_GroupRecord[m_GroupIndex[i]].win);			
		}
		else 
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+223, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_GroupRecord[m_GroupIndex[i]].win);
		}

		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{			
			g_XBaseFont->PrintAlign(m_WindowPosition.x+114+55+55+55, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_GroupRecord[m_GroupIndex[i]].lose);
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+260, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_GroupRecord[m_GroupIndex[i]].lose);
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+297, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_GroupRecord[m_GroupIndex[i]].lose);
		}

		if(m_GroupRecord[m_GroupIndex[i]].win+m_GroupRecord[m_GroupIndex[i]].lose > 0)
		{
			fRate = ((FLOAT)m_GroupRecord[m_GroupIndex[i]].win / (m_GroupRecord[m_GroupIndex[i]].win+m_GroupRecord[m_GroupIndex[i]].lose))*100.0f;
			rate = (int)fRate;
			
			if(rate  < 50)
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 253, 0, 0));
			}
			else if(rate > 50)
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 142, 241, 1));
			}
			
			g_XBaseFont->PrintAlign(m_WindowPosition.x+330, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d%%", rate);
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+330, m_WindowPosition.y+93+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0%%");
		}

		// icon
		if( !(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN) )
		{
			m_GroupIcon[m_GroupIndex[i]].Draw(m_WindowPosition.x+15, m_WindowPosition.y+91+(i*20));
		}
	}

	// 직책 전적
	for(i = 0 ; i < 4 ; i++)
	{
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 186, 186, 186));

		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+114+55, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_ClassRecord[i].win+m_ClassRecord[i].lose);
		}		
		else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+187, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_ClassRecord[i].win+m_ClassRecord[i].lose);
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+149, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_ClassRecord[i].win+m_ClassRecord[i].lose);
		}

		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+114+55+55, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_ClassRecord[i].win);
		}
		else 
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+223, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_ClassRecord[i].win);
		}
		
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+114+55+55+55, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_ClassRecord[i].lose);
		}
		else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+260, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_ClassRecord[i].lose);
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+297, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d", m_ClassRecord[i].lose);
		}

		if(m_ClassRecord[i].win+m_ClassRecord[i].lose > 0)
		{
			fRate = ((FLOAT)m_ClassRecord[i].win / (m_ClassRecord[i].win+m_ClassRecord[i].lose))*100.0f;
			rate = (int)fRate;
			
			if(rate  < 50)
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 253, 0, 0));
			}
			else if(rate > 50)
			{
				g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 142, 241, 1));
			}
			
			g_XBaseFont->PrintAlign(m_WindowPosition.x+330, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "%d%%", rate);
		}
		else
		{
			g_XBaseFont->PrintAlign(m_WindowPosition.x+330, m_WindowPosition.y+305+(i*20), 1.0f, _XFONT_ALIGNTYPE_RIGHT, "0%%");
		}
	}

	g_XBaseFont->Flush();
}

void _XWindow_PKRecord::ResetRecord(void)
{
	m_TotalWinCount		= 0;
	m_TotalLoseCount	= 0;
	m_TotalWinRate		= 0;
	
	memset(m_WinRecord, 0, sizeof(m_WinRecord));
	memset(m_LoseRecord, 0, sizeof(m_LoseRecord));
	
	memset(m_GroupRecord, 0, sizeof(m_GroupRecord));
	memset(m_ClassRecord, 0, sizeof(m_ClassRecord));
}

BOOL _XWindow_PKRecord::GetRecentRecord(int win, int index)
{
	SYSTEMTIME systemtime;
	GetLocalTime(&systemtime);

	int elapsedtime = 0;

	if(win == 0)
	{
		if(m_WinRecord[index].year+2006 == systemtime.wYear && m_WinRecord[index].month == systemtime.wMonth &&
			m_WinRecord[index].day == systemtime.wDay)
		{
			// 같은 날
			if(systemtime.wHour - m_WinRecord[index].hour == 1)
			{
				if(systemtime.wMinute >= m_WinRecord[index].minute)
				{
					elapsedtime = 60 + (systemtime.wMinute - m_WinRecord[index].minute);

					if(elapsedtime <= 60)
						return TRUE;
				}
				else
				{
					elapsedtime = 60 - m_WinRecord[index].minute + systemtime.wMinute;

					if(elapsedtime <= 60)
						return TRUE;
				}
			}
			else if(systemtime.wHour == m_WinRecord[index].hour)
			{
				elapsedtime = systemtime.wMinute - m_WinRecord[index].minute;

				if(elapsedtime <= 60)
					return TRUE;
			}
		}
	}
	else if(win == 1)
	{
		if(m_LoseRecord[index].year+2006 == systemtime.wYear && m_LoseRecord[index].month == systemtime.wMonth &&
			m_LoseRecord[index].day == systemtime.wDay)
		{
			// 같은 날
			if(systemtime.wHour - m_LoseRecord[index].hour == 1)
			{
				if(systemtime.wMinute >= m_LoseRecord[index].minute)
				{
					elapsedtime = 60 + (systemtime.wMinute - m_LoseRecord[index].minute);
					
					if(elapsedtime <= 60)
						return TRUE;
				}
				else
				{
					elapsedtime = 60 - m_LoseRecord[index].minute + systemtime.wMinute;
					
					if(elapsedtime <= 60)
						return TRUE;
				}
			}
			else if(systemtime.wHour == m_LoseRecord[index].hour)
			{
				elapsedtime = systemtime.wMinute - m_LoseRecord[index].minute;
				
				if(elapsedtime <= 60)
					return TRUE;
			}
		}
	}

	return FALSE;
}
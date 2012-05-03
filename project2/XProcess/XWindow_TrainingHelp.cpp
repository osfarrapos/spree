// XWindow_TrainingHelp.cpp: implementation of the _XWindow_TrainingHelp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindow_TrainingHelp.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_TrainingHelp::_XWindow_TrainingHelp()
{
	m_StartIndex = 0;
	m_SelectedSkillButtonIndex = -1;

	m_EndButton = NULL;
}

_XWindow_TrainingHelp::~_XWindow_TrainingHelp()
{

}


BOOL _XWindow_TrainingHelp::Initialize(void)
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_npcquestwin_01.tga");
	int pointindex = g_MainInterfaceTextureArchive.FindResource("mi_npcquestwin_02.tga");
	
	_XImageStatic* pBorder = new _XImageStatic;
	pBorder->Create(0, 0, 425, 408, &g_MainInterfaceTextureArchive, resourceindex);
	pBorder->SetClipRect(0, 0, 425, 408);
	InsertChildObject(pBorder);

	_XImageStatic* pTitle = new _XImageStatic;
	pTitle->Create(21, 24, 700, 324, &g_MainInterfaceTextureArchive, resourceindex);
	pTitle->SetClipRect(4, 461, 379, 487);
	InsertChildObject(pTitle);
	
	m_PointImageLeft = new _XImageStatic;
	m_PointImageLeft->Create(15, 255, 271, 361, &g_MainInterfaceTextureArchive, pointindex);
	m_PointImageLeft->SetClipRect(0, 0, 256, 112);
	m_PointImageLeft->ShowWindow(FALSE);
	InsertChildObject(m_PointImageLeft);

	m_PointImageRight = new _XImageStatic;
	m_PointImageRight->Create(271, 255, 418, 362, &g_MainInterfaceTextureArchive, pointindex);
	m_PointImageRight->SetClipRect(0, 112, 147, 219);
	m_PointImageRight->ShowWindow(FALSE);
	InsertChildObject(m_PointImageRight);

	int skillindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");

	m_TypeBorderLeft = new _XImageStatic;
	m_TypeBorderLeft->Create(40, 288, 209, 336, &g_MainInterfaceTextureArchive, skillindex);
	m_TypeBorderLeft->SetClipRect(0, 0, 169, 48);
	m_TypeBorderLeft->ShowWindow(FALSE);
	InsertChildObject(m_TypeBorderLeft);

	m_TypeBorderRight = new _XImageStatic;
	m_TypeBorderRight->Create(378, 288, 547, 336, &g_MainInterfaceTextureArchive, skillindex);
	m_TypeBorderRight->SetClipRect(0, 0, 169, 48);
	m_TypeBorderRight->SetScale(-1, 1);
	m_TypeBorderRight->ShowWindow(FALSE);
	InsertChildObject(m_TypeBorderRight);

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {118, 350}, {91, 27},
		_XDEF_TRAINING_HELP_START,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};
	_XButton* pStartButton = new _XButton;
	pStartButton->Create(btnstruct);
	pStartButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
	pStartButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
	pStartButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
	pStartButton->SetGrowTextMode(0xFFFFFFFF);
	pStartButton->SetButtonTextColor(0xFFFFFFFF);
	pStartButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_TRAININGHELP_START);	// "수련시작"
	InsertChildObject(pStartButton);

	btnstruct.position.x = 215;
	btnstruct.position.y = 350;
	btnstruct.commandid = _XDEF_TRAINING_HELP_END;
	m_EndButton = new _XButton;
	m_EndButton->Create(btnstruct);
	m_EndButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 400, 425, 491, 452);
	m_EndButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 400, 457, 491, 484);
	m_EndButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 400, 457, 491, 484);
	m_EndButton->SetGrowTextMode(0xFFFFFFFF);
	m_EndButton->SetButtonTextColor(0xFFFFFFFF);
	m_EndButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);	// "취소"
	InsertChildObject(m_EndButton);

	_XBTN_STRUCTURE skillbtnstruct = 
	{
		TRUE, {43, 291}, {19, 42}, _XDEF_TRAINING_HELP_LEFT,
		skillindex,
		skillindex,
		skillindex,
		&g_MainInterfaceTextureArchive
	};
	m_LeftButton = new _XButton;
	m_LeftButton->Create(skillbtnstruct);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 191, 41, 210, 83);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 211, 41, 230, 83);
	m_LeftButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 171, 41, 190, 83);
	m_LeftButton->ShowWindow(FALSE);
	InsertChildObject(m_LeftButton);

	skillbtnstruct.position.x = 356;
	skillbtnstruct.position.y = 291;
	skillbtnstruct.commandid = _XDEF_TRAINING_HELP_RIGHT;
	m_RightButton = new _XButton;
	m_RightButton->Create(skillbtnstruct);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 191, 84, 210, 126);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 211, 84, 230, 126);
	m_RightButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 171, 84, 190, 126);
	m_RightButton->ShowWindow(FALSE);
	InsertChildObject(m_RightButton);

	skillbtnstruct.position.x = 67;
	skillbtnstruct.position.y = 296;
	skillbtnstruct.windowsize.cx = 30;
	skillbtnstruct.windowsize.cy = 30;
	skillbtnstruct.commandid = _XDEF_TRAINING_HELP_SKILL_1;
	m_SkillButton[0] = new _XButton;
	m_SkillButton[0]->Create(skillbtnstruct);
	m_SkillButton[0]->ShowWindow(FALSE);
	InsertChildObject(m_SkillButton[0]);

	for(int i = 1 ; i < 8 ; i++)
	{
		skillbtnstruct.position.x = 67 + 36*i;
		skillbtnstruct.commandid++;
		m_SkillButton[i] = new _XButton;
		m_SkillButton[i]->Create(skillbtnstruct);
		m_SkillButton[i]->ShowWindow(FALSE);
		InsertChildObject(m_SkillButton[i]);
	}

	m_ButtonBorderImage.Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, skillindex);
	m_ButtonBorderImage.SetClipRect(207, 0, 243, 36);

	m_SelectedSkillButtonImage.Create(0, 0, 36, 36, &g_MainInterfaceTextureArchive, skillindex);
	m_SelectedSkillButtonImage.SetClipRect(170, 0, 206, 36);

	int meditationindex_1 = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		meditationindex_1 = g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		meditationindex_1 = g_MainInterfaceTextureArchive.FindResource("us_MI_unki_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		meditationindex_1 = g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_01.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		meditationindex_1 = g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_01.tga");
	}
	else 
	{
		meditationindex_1 = g_MainInterfaceTextureArchive.FindResource("MI_unki_01.tga");		
	}	

	int meditationindex_2 = -1;	
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE )
	{
		meditationindex_2 = g_MainInterfaceTextureArchive.FindResource("vn_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		meditationindex_2 = g_MainInterfaceTextureArchive.FindResource("us_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE )
	{
		meditationindex_2 = g_MainInterfaceTextureArchive.FindResource("tw_MI_unki_02.tga");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		meditationindex_2 = g_MainInterfaceTextureArchive.FindResource("rs_MI_unki_02.tga");
	}
	else 
	{
		meditationindex_2 = g_MainInterfaceTextureArchive.FindResource("MI_unki_02.tga");		
	}	

	for(i = 0 ; i < 3 ; i++)
		m_MeditationImage[i].Create(0, 0, 30, 30, &g_MainInterfaceTextureArchive, meditationindex_1);
	for(i = 3 ; i < 6 ; i++)
		m_MeditationImage[i].Create(0, 0, 60, 30, &g_MainInterfaceTextureArchive, meditationindex_2);

	m_MeditationImage[0].SetClipRect(2, 2, 32, 32);			// 소양
	m_MeditationImage[1].SetClipRect(2, 36, 32, 66);		// 소음
	m_MeditationImage[2].SetClipRect(2, 70, 32, 100);		// 소무
	m_MeditationImage[3].SetClipRect(130, 2, 190, 32);		// 대양
	m_MeditationImage[4].SetClipRect(194, 138, 254, 168);	// 대음
	m_MeditationImage[5].SetClipRect(2, 2, 62, 32);			// 대무

	_XTOOLTIP_STRUCTURE tooltipstruct = 
	{
		{0, 0},
		{78, 13},
		{1, 1},
		-1,
		NULL
	};

	m_Tooltip.Create(tooltipstruct);
	
	return TRUE;
} 

void _XWindow_TrainingHelp::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
	if(m_TrainingType == _XTRAINING_DUMMY)
	{
		g_XBaseFont->SetColor(0xFFFFFFFF);
		g_XBaseFont->Puts(m_WindowPosition.x+179, m_WindowPosition.y+31, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLTRAINING));	// "무공 수련"
		g_XBaseFont->Puts(m_WindowPosition.x+178, m_WindowPosition.y+31, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLTRAINING));	// "무공 수련"

		g_XBaseFont->SetColor(0xFF000000);
		/// 스트링 문자열을 변경하면 더 손쉽게 변경할 수 있음.
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			int drawPos = 68;
			TCHAR tempBuf[4][256];
			memset( tempBuf, 0, sizeof(tempBuf) );
			strcpy( tempBuf[0], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE1)+2 );
			strcpy( tempBuf[1], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE2)+2 );
			strcat( tempBuf[1], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE3) );
			strcat( tempBuf[1], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE4) );
			strcpy( tempBuf[2], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE5)+2 );
			strcat( tempBuf[2], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE6) );
			strcpy( tempBuf[3], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE7)+2 );
			strcat( tempBuf[3], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE8) );		
			
			int linecount = 0;
			for( int i = 0; i < 4; ++i )
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+47, m_WindowPosition.y+drawPos, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%d.", i+1);
				linecount = g_XBaseFont->Puts_Separate(m_WindowPosition.x+62, m_WindowPosition.y+drawPos, tempBuf[i], 330 );
				drawPos += linecount * 18;
			}
		}
		else
		{
			g_XBaseFont->Puts(m_WindowPosition.x+47, m_WindowPosition.y+68, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE1));
			g_XBaseFont->Puts(m_WindowPosition.x+47, m_WindowPosition.y+86, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE2));
			g_XBaseFont->Puts(m_WindowPosition.x+64, m_WindowPosition.y+104, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE3));
			g_XBaseFont->Puts(m_WindowPosition.x+64, m_WindowPosition.y+122, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE4));
			g_XBaseFont->Puts(m_WindowPosition.x+47, m_WindowPosition.y+148, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE5));
			g_XBaseFont->Puts(m_WindowPosition.x+64, m_WindowPosition.y+166, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE6));
			g_XBaseFont->Puts(m_WindowPosition.x+47, m_WindowPosition.y+192, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE7));
			g_XBaseFont->Puts(m_WindowPosition.x+64, m_WindowPosition.y+210, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SKILLGUIDE8));
		}		

		g_XBaseFont->SetColor(0xFF000000);
		g_XBaseFont->Puts(m_WindowPosition.x+157, m_WindowPosition.y+271, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SELECTSKILL));	// "[ 수련 무공 선택 ]"
		g_XBaseFont->Puts(m_WindowPosition.x+156, m_WindowPosition.y+271, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SELECTSKILL));	// "[ 수련 무공 선택 ]"

		for(int i = 0 ; i < 8 ; i++)
		{
			m_ButtonBorderImage.Draw(m_WindowPosition.x+65+(i*36), m_WindowPosition.y+294);		
		}
		
		if(m_SelectedSkillButtonIndex >= 0)
		{
			POINT pos = m_SkillButton[m_SelectedSkillButtonIndex]->GetWindowPos();
			m_SelectedSkillButtonImage.Draw(pos.x-2, pos.y-2);
		}
	}
	else if(m_TrainingType == _XTRAINING_SPELL)
	{
		g_XBaseFont->SetColor(0xFFFFFFFF);
		g_XBaseFont->Puts(m_WindowPosition.x+179, m_WindowPosition.y+31, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCETRANING));	// "기공 수련"
		g_XBaseFont->Puts(m_WindowPosition.x+178, m_WindowPosition.y+31, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCETRANING));	// "기공 수련"

		g_XBaseFont->SetColor(0xFF000000);
		/// 스트링 문자열을 변경하면 더 손쉽게 변경할 수 있음.
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			int drawPos = 68;
			TCHAR tempBuf[4][256];
			memset( tempBuf, 0, sizeof(tempBuf) );
			strcpy( tempBuf[0], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE1)+2 );
			strcpy( tempBuf[1], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE2)+2 );
			strcat( tempBuf[1], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE3) );
			strcat( tempBuf[1], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE4) );
			strcpy( tempBuf[2], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE5)+2 );
			strcat( tempBuf[2], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE6) );
			strcpy( tempBuf[3], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE7)+2 );
			strcat( tempBuf[3], _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE8) );		
			
			int linecount = 0;
			for( int i = 0; i < 4; ++i )
			{
				g_XBaseFont->PrintAlign(m_WindowPosition.x+47, m_WindowPosition.y+drawPos, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%d.", i+1);
				linecount = g_XBaseFont->Puts_Separate(m_WindowPosition.x+62, m_WindowPosition.y+drawPos, tempBuf[i], 330 );
				drawPos += linecount * 18;
			}
		}
		else
		{
			g_XBaseFont->Puts(m_WindowPosition.x+47, m_WindowPosition.y+68, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE1));
			g_XBaseFont->Puts(m_WindowPosition.x+47, m_WindowPosition.y+86, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE2));
			g_XBaseFont->Puts(m_WindowPosition.x+64, m_WindowPosition.y+104, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE3));
			g_XBaseFont->Puts(m_WindowPosition.x+64, m_WindowPosition.y+122, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE4));
			g_XBaseFont->Puts(m_WindowPosition.x+47, m_WindowPosition.y+148, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE5));
			g_XBaseFont->Puts(m_WindowPosition.x+64, m_WindowPosition.y+166, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE6));
			g_XBaseFont->Puts(m_WindowPosition.x+47, m_WindowPosition.y+192, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE7));
			g_XBaseFont->Puts(m_WindowPosition.x+64, m_WindowPosition.y+210, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_FORCEGUIDE8));
		}

		g_XBaseFont->SetColor(0xFF000000);
		g_XBaseFont->Puts(m_WindowPosition.x+157, m_WindowPosition.y+271, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SELECTSKILL));	// "[ 수련 무공 선택 ]"
		g_XBaseFont->Puts(m_WindowPosition.x+156, m_WindowPosition.y+271, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_SELECTSKILL));	// "[ 수련 무공 선택 ]"

		for(int i = 0 ; i < 8 ; i++)
		{
			m_ButtonBorderImage.Draw(m_WindowPosition.x+65+(i*36), m_WindowPosition.y+294);		
		}
		
		if(m_SelectedSkillButtonIndex >= 0)
		{
			POINT pos = m_SkillButton[m_SelectedSkillButtonIndex]->GetWindowPos();
			m_SelectedSkillButtonImage.Draw(pos.x-2, pos.y-2);
		}
	}
	else if(m_TrainingType == _XTRAINING_MEDITATION)
	{
		g_XBaseFont->SetColor(0xFFFFFFFF);
		g_XBaseFont->Puts(m_WindowPosition.x+179, m_WindowPosition.y+31, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATION));	// "운기 조식"

		g_XBaseFont->SetColor(0xFF000000);
		/// 스트링 문자열을 변경하면 더 손쉽게 변경할 수 있음.
		if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			int drawPos = 64;
			int linecount = 0;

			TCHAR tempBuf[256];
			memset( tempBuf, 0, sizeof(tempBuf) );
			strcpy( tempBuf, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE1) );
			strcat( tempBuf, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE2) );
			
			g_XBaseFont->Puts_Separate(m_WindowPosition.x+30, m_WindowPosition.y+drawPos, tempBuf, 360 );

			g_XBaseFont->Puts(m_WindowPosition.x+70, m_WindowPosition.y+113, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE3));
			g_XBaseFont->Puts(m_WindowPosition.x+70, m_WindowPosition.y+146, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE4));
			g_XBaseFont->Puts(m_WindowPosition.x+70, m_WindowPosition.y+180, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE5));
			g_XBaseFont->Puts(m_WindowPosition.x+228, m_WindowPosition.y+113, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE6));
			g_XBaseFont->Puts(m_WindowPosition.x+228, m_WindowPosition.y+146, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE7));
			g_XBaseFont->Puts(m_WindowPosition.x+228, m_WindowPosition.y+180, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE8));

			drawPos = 212;
			linecount = g_XBaseFont->Puts_Separate(m_WindowPosition.x+30, m_WindowPosition.y+drawPos, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE9), 360 );
			drawPos += linecount * 18;
			linecount = g_XBaseFont->Puts_Separate(m_WindowPosition.x+30, m_WindowPosition.y+drawPos, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE10), 360 );
			drawPos += linecount * 18;
			linecount = g_XBaseFont->Puts_Separate(m_WindowPosition.x+30, m_WindowPosition.y+drawPos, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE11), 360 );
			drawPos += linecount * 18;
			linecount = g_XBaseFont->Puts_Separate(m_WindowPosition.x+30, m_WindowPosition.y+drawPos, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE12), 360 );
			drawPos += linecount * 18;
			linecount = g_XBaseFont->Puts_Separate(m_WindowPosition.x+30, m_WindowPosition.y+drawPos, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE13), 360 );
			drawPos += linecount * 18;
			linecount = g_XBaseFont->Puts_Separate(m_WindowPosition.x+30, m_WindowPosition.y+drawPos, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE14), 360 );

			m_MeditationImage[0].Draw(m_WindowPosition.x+113, m_WindowPosition.y+104);
			m_MeditationImage[1].Draw(m_WindowPosition.x+113, m_WindowPosition.y+138);
			m_MeditationImage[2].Draw(m_WindowPosition.x+113, m_WindowPosition.y+172);
			m_MeditationImage[3].Draw(m_WindowPosition.x+152, m_WindowPosition.y+104);
			m_MeditationImage[4].Draw(m_WindowPosition.x+152, m_WindowPosition.y+138);
			m_MeditationImage[5].Draw(m_WindowPosition.x+152, m_WindowPosition.y+172);
		}
		else
		{
			g_XBaseFont->Puts(m_WindowPosition.x+61, m_WindowPosition.y+64, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE1));
			g_XBaseFont->Puts(m_WindowPosition.x+42, m_WindowPosition.y+82, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE2));
			g_XBaseFont->Puts(m_WindowPosition.x+70, m_WindowPosition.y+123, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE3));
			g_XBaseFont->Puts(m_WindowPosition.x+70, m_WindowPosition.y+156, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE4));
			g_XBaseFont->Puts(m_WindowPosition.x+70, m_WindowPosition.y+190, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE5));
			g_XBaseFont->Puts(m_WindowPosition.x+228, m_WindowPosition.y+123, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE6));
			g_XBaseFont->Puts(m_WindowPosition.x+228, m_WindowPosition.y+156, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE7));
			g_XBaseFont->Puts(m_WindowPosition.x+228, m_WindowPosition.y+190, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE8));
			g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+236, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE9));
			g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+254, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE10));
			g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+272, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE11));
			g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+290, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE12));
			g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+308, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE13));
			g_XBaseFont->Puts(m_WindowPosition.x+30, m_WindowPosition.y+326, _XGETINTERFACETEXT(ID_STRING_TRAININGHELP_MEDITATIONGUIDE14));

			m_MeditationImage[0].Draw(m_WindowPosition.x+113, m_WindowPosition.y+114);
			m_MeditationImage[1].Draw(m_WindowPosition.x+113, m_WindowPosition.y+148);
			m_MeditationImage[2].Draw(m_WindowPosition.x+113, m_WindowPosition.y+182);
			m_MeditationImage[3].Draw(m_WindowPosition.x+152, m_WindowPosition.y+114);
			m_MeditationImage[4].Draw(m_WindowPosition.x+152, m_WindowPosition.y+148);
			m_MeditationImage[5].Draw(m_WindowPosition.x+152, m_WindowPosition.y+182);
		}
	}

	g_XBaseFont->Flush(); 

	DrawTooltip();
}
BOOL _XWindow_TrainingHelp::Process(_XGUIObject*& pfocusobject)
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

void _XWindow_TrainingHelp::ShowWindow(BOOL show)
{
	if(show)
	{
		m_SkillStr.clear();
		m_StartIndex = 0;
		m_SelectedSkillButtonIndex = -1;
		int index = 0;

		switch(m_TrainingType)
		{
		case _XTRAINING_DUMMY :
			{
				// 무공 수련
				for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
				{
					if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
					{
						if(g_pLocalUser->m_SkillList[i].m_ucSkillState == 1 || g_pLocalUser->m_SkillList[i].m_cSkillLevel < 3)
						{
							if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_ACTIVESKILL)// ||
//								g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->usefulType == _XSI_USEFULTYPE_SMASHINGSKILL)
							{
								m_SkillStr[index] = g_pLocalUser->m_SkillList[i].m_sSkillID;
								index++;
							}
						}
					}
				} // for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
				
				m_EndButton->EnableWindow(TRUE);
				m_PointImageLeft->ShowWindow(TRUE);
				m_PointImageRight->ShowWindow(TRUE);
				m_TypeBorderLeft->ShowWindow(TRUE);
				m_TypeBorderRight->ShowWindow(TRUE);
				SetSkillButtonTexture();
			}
			break;
		case _XTRAINING_SPELL :
			{
				// 기공 수련
				for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)
				{
					if(g_pLocalUser->m_SkillList[i].m_sSkillID > 0)
					{
						if(g_pLocalUser->m_SkillList[i].m_ucSkillState == 1 || g_pLocalUser->m_SkillList[i].m_cSkillLevel < 3)
						{
							if(g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillCType == _XSI_CTYPE_SPELL &&
								g_SkillProperty[g_pLocalUser->m_SkillList[i].m_sSkillID]->skillSType != _XSI_STYPE_03)
							{
								m_SkillStr[index] = g_pLocalUser->m_SkillList[i].m_sSkillID;
								index++;
							}
						}
					}
				} // for(int i = 0 ; i < _XDEF_MAX_LEARNSKILL ; i++)

				m_EndButton->EnableWindow(TRUE);
				m_PointImageLeft->ShowWindow(TRUE);
				m_PointImageRight->ShowWindow(TRUE);
				m_TypeBorderLeft->ShowWindow(TRUE);
				m_TypeBorderRight->ShowWindow(TRUE);
				SetSkillButtonTexture();
			}
			break;
		case _XTRAINING_MEDITATION:
			{
				m_PointImageLeft->ShowWindow(FALSE);
				m_PointImageRight->ShowWindow(FALSE);
				m_TypeBorderLeft->ShowWindow(FALSE);
				m_TypeBorderRight->ShowWindow(FALSE);
				MoveWindow(((gnWidth>>1)-(m_WindowSize.cx>>1)), 44);
			}
			break;
		}
	}
	else
	{
		for(int i = 0 ; i < 8 ; i++)
			m_SkillButton[i]->ShowWindow(FALSE);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_TrainingHelp::SetSkillButtonTexture(void)
{
	int resourceindex = 0;
	RECT rect;
	bool bColor;

	int count = 0;
	int	i = 0;

	map <int, short>::iterator iter_index;
	for(iter_index = m_SkillStr.begin() ; iter_index != m_SkillStr.end() ; iter_index++)
	{
		if(m_SkillStr[m_StartIndex+i] > 0)
		{
			if(i < 8)
			{
				int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_SkillStr[m_StartIndex+i]);
				if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
					bColor = false;
				else
					bColor = true;
				
				g_pQuickSlot_Window->GetIconRect(m_SkillStr[m_StartIndex+i], resourceindex, rect, bColor);
				
				m_SkillButton[i]->SetButtonImage(resourceindex, resourceindex, resourceindex, &g_MainInterfaceTextureArchive);
				m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, rect.left, rect.top, rect.right, rect.bottom);
				m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, rect.left, rect.top, rect.right, rect.bottom);
				m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, rect.left, rect.top, rect.right, rect.bottom);
				
				m_SkillButton[i]->ShowWindow(TRUE);
			}
			i++;
			count++;
		}
	}


/*	for(int i = 0 ; i < 8 ; i++)
	{
		if(m_SkillStr[m_StartIndex+i] > 0)
		{
			int learnedindex = g_pLocalUser->GetLearnedSkillIndex(m_SkillStr[m_StartIndex+i]);
			if(g_pLocalUser->m_SkillList[learnedindex].m_ucSkillState == 1)
				bColor = false;
			else
				bColor = true;

			g_pQuickSlot_Window->GetIconRect(m_SkillStr[m_StartIndex+i], resourceindex, rect, bColor);

			m_SkillButton[i]->SetButtonImage(resourceindex, resourceindex, resourceindex, &g_MainInterfaceTextureArchive);
			m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, rect.left, rect.top, rect.right, rect.bottom);
			m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, rect.left, rect.top, rect.right, rect.bottom);
			m_SkillButton[i]->SetButtonImageClipRect(_XBUTTONIMAGE_READY, rect.left, rect.top, rect.right, rect.bottom);

			m_SkillButton[i]->ShowWindow(TRUE);

			count++;
		}
	}*/

	if(m_StartIndex > 0)
		m_LeftButton->ShowWindow(TRUE);
	else
		m_LeftButton->ShowWindow(FALSE);

	if(count > 8)
		m_RightButton->ShowWindow(TRUE);
	else
		m_RightButton->ShowWindow(FALSE);
}

void _XWindow_TrainingHelp::DrawTooltip(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	for(int i = 0 ; i < 8 ; i++)
	{
		if(m_SkillButton[i]->GetShowStatus())
		{
			POINT pos = m_SkillButton[i]->GetWindowPos();

			if(scrnpos->x >= pos.x 
				&& scrnpos->x <= pos.x+30
				&& scrnpos->z >= pos.y 
				&& scrnpos->z <= pos.y+30)
			{
				m_Tooltip.SetToolTipString(pos.x, pos.y-13,g_SkillProperty[m_SkillStr[m_StartIndex+i]]->skillName, _XSC_INFORMATION);
				m_Tooltip.Draw();
			}
		} // if(m_SkillButton[i]->GetShowStatus())
	} // for(int i = 0 ; i < 8 ; i++)
}

void _XWindow_TrainingHelp::SetStartIndex(int index)
{
	if(index < 0)
		index = 0;

	if(index >= m_SkillStr.size())
		index = m_SkillStr.size() - 1;

	m_StartIndex = index;
}

short _XWindow_TrainingHelp::GetSelectedSkillID(void)
{
	return m_SkillStr[m_SelectedSkillButtonIndex + m_StartIndex];
}
// XWindow_SelectCharacter.h: interface for the _XWindow_SelectCharacter class.
//
// Created Date		: 2003.04.30
// Author			: Sohyun, Park (kukuri)
// Desc				: Character Select Window
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUXWINDOW_SELECTCHARACTER_H__BFEA4AAF_3E63_40DB_B4A4_8C62F34B2684__INCLUDED_)
#define AFX_KUXWINDOW_SELECTCHARACTER_H__BFEA4AAF_3E63_40DB_B4A4_8C62F34B2684__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_SelectCharacter : public _XWindow  
{
public :	

	_XImageStatic*  m_pMinimapImageStatic;

	_XImageStatic   m_MenuEdge_Top;
	_XImageStatic   m_MenuEdge_Bottom;
	_XImageStatic   m_MenuEdge_Left;
	_XImageStatic   m_MenuEdge_Right;

	_XButton		m_StartGameButton;
	_XButton		m_CreateCharacterButton;
	_XButton		m_DeleteCharacterButton;
	_XButton		m_ExitButton;

	_XButton		m_Blank01Button;
	_XButton		m_Blank02Button;
	_XButton		m_Blank03Button;
	
	TCHAR			m_Name[128];	
	TCHAR			m_Level[128];
	TCHAR			m_SubLevel[128];
	TCHAR			m_RollClass[128];
	TCHAR			m_CharacterLocation[128];
	
	short			m_constitution;
	short			m_zen;		
	short			m_intelligence;
	short			m_dexterity;	
	short			m_strength;	
	
	BOOL			m_DrawCharacterInfo;
	
	int				m_CharacterGMLevel;
	
	_XImageStatic*  m_pCharacterGroupNameImageStatic;
	_XImageStatic   m_SpawnPointStatic;

	int					m_SelectedCharacter;
	int					m_MinimapAlphaLevel[3];
	LPDIRECT3DTEXTURE9	m_pMinimapTexture[3];	
			
public:
	_XWindow_SelectCharacter();
	virtual ~_XWindow_SelectCharacter();
	
	BOOL			Initialize(_XTextureManager& texturearchive);
	void			DestroyWindow(void);
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			DrawCharacterInfo(void);
	
	void			RebuildCharacterGroupNameStatic( int group );

	BOOL			LoadMinimapTexture( int index, LPTSTR zonefilenmae );
};

#endif // !defined(AFX_KUXWINDOW_SELECTCHARACTER_H__BFEA4AAF_3E63_40DB_B4A4_8C62F34B2684__INCLUDED_)

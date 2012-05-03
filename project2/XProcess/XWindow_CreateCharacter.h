// XWindow_CreateCharacter.h: interface for the _XWindow_CreateCharacter class.
//
// Created Date		: 2003.05.05
// Author			: Sohyun, Park (kukuri)
// Desc				: Character Create Window
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_KUXWINDOW_CREATECHARACTER_H__3AC4C158_4A68_49A2_9576_8BA3BC087AE2__INCLUDED_)
#define AFX_KUXWINDOW_CREATECHARACTER_H__3AC4C158_4A68_49A2_9576_8BA3BC087AE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_CreateCharacter : public _XWindow  
{
public :
	_XIMEContainer		m_NameEdit;
	
	_XCheckButton*		m_ManButton;
	_XCheckButton*		m_WomanButton;
	
	_XButton*			m_FaceLeftButton;
	_XButton*			m_FaceRightButton;
	_XButton*			m_HairLeftButton;
	_XButton*			m_HairRightButton;
	_XButton*			m_ClothLeftButton;
	_XButton*			m_ClothRightButton;
	
	_XButton*			m_VitalityPlusButton;
	_XButton*			m_VitalityMinusButton;
	_XButton*			m_ZenPlusButton;
	_XButton*			m_ZenMinusButton;
	_XButton*			m_SoulPlusButton;
	_XButton*			m_SoulMinusButton;
	_XButton*			m_HeartPlusButton;
	_XButton*			m_HeartMinusButton;
	_XButton*			m_StaminaPlusButton;
	_XButton*			m_StaminaMinusButton;
	
	_XImageStatic		m_MenuEdge_Top;
	_XImageStatic		m_MenuEdge_Bottom;
	_XImageStatic		m_MenuEdge_Left;
	_XImageStatic		m_MenuEdge_Right;
	
	_XButton			m_CreateButton;
	_XButton			m_CancelButton;
	_XButton			m_ExitButton;
	
	int					m_BonusPoint;
	int					m_Parameter[5];

	_XCheckButton*		m_pAreaSelectCheckButton[6];

	int					m_FaceTypeIndex;
	int					m_HairTypeIndex;
	int					m_ClothTypeIndex;

public:
	_XWindow_CreateCharacter();
	virtual ~_XWindow_CreateCharacter();
	
	BOOL				Initialize(_XTextureManager& texturearchive);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				MoveWindow(int X, int Y);
	
	BOOL				CheckNameEditBoxArea(void);
	
	void				DrawParameter(void);
};

#endif // !defined(AFX_KUXWINDOW_CREATECHARACTER_H__3AC4C158_4A68_49A2_9576_8BA3BC087AE2__INCLUDED_)

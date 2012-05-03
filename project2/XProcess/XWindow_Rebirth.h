// XWindow_Rebirth.h: interface for the _XWindow_Rebirth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_REBIRTH_H__DAB96BC9_6B9A_4677_912B_3E4F7A485F7C__INCLUDED_)
#define AFX_XWINDOW_REBIRTH_H__DAB96BC9_6B9A_4677_912B_3E4F7A485F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum _XREBIRTHTYPE
{
	_XREBIRTH_NONE,
	_XREBIRTH_BINSA,
	_XREBIRTH_HONSU
};

// 부활창 리뉴얼하면서 button type 추가
//
// 총 버튼은 5개 존재한다. 
//
//- _XREBIRTH_HONSU 일때
//		1. 마을(문파)부활
//		2. 여점/공격진지/장원안 부활
//		3. 제자리 부활
//		4. 화타신단 사용
//		5. 광고 보기 (미국만)
//
// 각 버튼의 값에 따른 의미는 다음과 같다.
//		0)	- 0 : 마을 부활(낭인)
//			- 1 : 문파 부활(문파가입)
//		1)	- 0 : 장원전중 (공성)
//			- 1 : 장원전중 (수성)
//			- 2 : 여점 있음
//			- 3 : 여점 없음
//		2)	- 0 : pk로 죽음
//			- 1 : 몬스터에게 죽음
//		3)	- 0 : 화타신단 있음
//			- 1 : 화타신단 없음
//		4)	- 0 : 광고 없음
//			- 1 : 광고 있음
//
//- _XREBIRTH_BINSA 일때
//	 따로 처리한다.
//

class _XWindow_Rebirth : public _XWindow  
{
public :
	_XButton*			m_TownButton;
	_XButton*			m_FieldButton;
	_XButton*			m_UseItemButton;	
//	_XButton*			m_InCBButton;
	
	_XButton*			m_SavePointButton;

	_XImageStatic		m_CornerImage[2];
	
	_XREBIRTHTYPE		m_RebirthType;
	
	BOOL				m_bCastleBattle;

	BOOL				m_bDeadInPK;		// Pk 중에 죽었음
	BOOL				m_bDeadInCB;		// 장원전 중에 죽었음
	BOOL				m_bEnablePKTracing;	// PK범 위치 추적 가능한지 여부 

	int					m_nButtonType[5];
	int					m_nTotalButtonCount;	// 보여지는 버튼 수 (window size 결정)
	
#ifdef _ACCLAIM_IGAADSYSTEM
	_XButton*			m_AcclaimAdVideoButton;
	BOOL				m_ShowIGAADButton;
	int					m_ButtonShowTimer;
	BOOL				m_VideoADCanceled;
#endif
	
public:
	_XWindow_Rebirth();
	virtual ~_XWindow_Rebirth();
	
	BOOL				Initialize(void);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				ShowWindow(BOOL show);
	void				SetWindowSize(int width, int height);

	void				SetCastleBattleState(BOOL battle);

	void				SetButton(void);

	inline void			SetRebirthMode(_XREBIRTHTYPE type)	{	m_RebirthType = type;		};
	inline BOOL			GetCastleBattleState(void)			{	return m_bCastleBattle;		};
	
};

#endif // !defined(AFX_XWINDOW_REBIRTH_H__DAB96BC9_6B9A_4677_912B_3E4F7A485F7C__INCLUDED_)

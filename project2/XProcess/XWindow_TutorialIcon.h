// XWindow_TutorialIcon.h: interface for the _XWindow_TutorialIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TUTORIALICON_H__5BCB6A0F_7DB0_474D_B3DB_E37AB10B7A4E__INCLUDED_)
#define AFX_XWINDOW_TUTORIALICON_H__5BCB6A0F_7DB0_474D_B3DB_E37AB10B7A4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include <list>
using namespace std;

/**
 * \brief 튜토리얼 아이콘
 * 발생조건에 부합할 때 나타나는 팁 아이콘 
 * 
 * 일반 화면과 npc wide 화면 두 곳에서 나타날 수 있음
 * _MAX_TUTORIALICON 까지만 생성됨 (cpp에 정의)
 * 튜토리얼은 아이콘으로 표시되는 일반 튜토리얼과 
 * 아이콘 없이 바로 창이 뜨는 강제 튜토리얼이 있음.
 * 튜토리얼 클릭정보는 개인의 pc에 아이디로 저장된다.
 * npcmode 튜토리얼 아이콘은 일반화면으로 돌아왔을 시 감춘다.
*/

class _XWindow_ChattingDefault;

/// 하나의 icon Data
typedef struct tagTutorialData
{
	DWORD winId;
	_XWindow* winPtr;
} _XTUTORIALDATA;

class _XWindow_TutorialIcon : public _XWindow  
{
private:
	_XButton*						m_BtnTutorialIcon[8];
	_XWindow_ChattingDefault*		m_pChattingDefault_Window;
	int								m_ExternalTutorialCount;		///< 일반 화면 갯수 
	int								m_InternalTutorialCount;		///< npcmode 화면 갯수
	int								m_EffectCount;
	DWORD							m_EffectTime;

	list<_XTUTORIALTYPE> 			m_ExternalTutorialIconLIst;		///< 일반 화면 튜토리얼 타입 리스트
	list<_XTUTORIALTYPE> 			m_InternalTutorialIconLIst;		///< npcmode 화면 튜토리얼 타입 리스트

	list<_XTUTORIALDATA*> 			m_ExternalTutorialDialogLIst;	///< 일반 화면 튜토리얼 데이타 리스트
	list<_XTUTORIALDATA*>			m_InternalTutorialDialogLIst;	///< npcmode 화면 튜토리얼 데이타 리스트

	void				SaveTutorialData(_XTUTORIALTYPE type);
	void				AllClearInternalTutorialIconList();			///< npcmode 모든 튜토리얼 창 닫기	
	
public:
	_XWindow_TutorialIcon();
	virtual ~_XWindow_TutorialIcon();

	BOOL				Initialize( void );
	void				DestroyWindow( void );
	
	void				Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL				Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void				AllTutorialDraw();
	void				AllTutorialProcess();
	
	void				InsertTutorialIcon( _XTUTORIALTYPE type );	///< 튜토리얼 아이콘 추가
	void				RemoveTutorialIcon( _XTUTORIALTYPE type );	///< icon 없는 튜토리얼 클릭시
	void				RemoveTutorialIcon( int index );			///< icon 있는 튜토리얼 클릭시

	void				SetChattingDefaultWindowPtr( _XWindow_ChattingDefault* windowptr) { m_pChattingDefault_Window = windowptr;	};
	void				DeleteTutorialIDlgList( DWORD winid);	///< 해당 튜토리얼 창 삭제
	
	void				AllCloseExternalTutorialWindow();
	void				AllOpenExternalTutorialWindow();
	void				ResetTutorialData();
	
	BOOL				EscapeKeyDown();	///< 튜토리얼 창 닫기
};

#endif // !defined(AFX_XWINDOW_TUTORIALICON_H__5BCB6A0F_7DB0_474D_B3DB_E37AB10B7A4E__INCLUDED_)

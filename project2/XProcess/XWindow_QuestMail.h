// XWindow_QuestMail.h: interface for the _XWindow_QuestMail class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_QUESTMAIL_H__268291A2_5F36_4493_B362_F27C3E5BC5EB__INCLUDED_)
#define AFX_XWINDOW_QUESTMAIL_H__268291A2_5F36_4493_B362_F27C3E5BC5EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

typedef enum QuestMailButton
{
	_XQUESTMAILBUTTON_OK	= 0,		
		_XQUESTMAILBUTTON_YES,			
		_XQUESTMAILBUTTON_NO,		
		_XQUESTMAILBUTTON_LISTBOXMESSAGE
}; 

typedef enum _XQuestMailMode
{
	_XQUESTMAILMODE_OK = 0,
	_XQUESTMAILMODE_YESNO,
	_XQUESTMAILMODE_TUTORIALONLY //Author : 양희왕 //breif : 처음 강호 초출기 시 수락 거부를 바꾼다
		
}_XQUESTMAIL_MODE;

class _XWindow_QuestMail  : public _XWindow  
{
public:	
	int					m_SkinMode;				// 0:일반 풍운방 1:튜토리얼 풍운방
	int					m_QuestID;
	_XQUESTMAIL_MODE	m_QuestMailMode;
	BOOL				m_bStartQuest;

	_XListBox*			m_listboxQuestMessage;	// 풍운방 내용을 표시하는 리스트 박스 

	_XButton*			m_btnQuestMailOK;		// 풍운방 확인 버튼 
	_XButton*			m_btnQuestMailYES;		// 풍운방 수락 버튼 
	_XButton*			m_btnQuestMailNO;		// 풍운방 거부 버튼 
	
public:
	_XWindow_QuestMail();
	virtual ~_XWindow_QuestMail();
	
	static _XWindow_QuestMail*	CreateQuestMail( _XWindowManager* pWindowManager, int questid, int skinmode = 0 );
	
	void			Create( _XWindowManager* pWindowManager, DWORD windowid, int skinmode = 0 );	
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			ProcessWheelMessage( short zDelta );
	
	void			MoveWindow(int X, int Y);
	void			ShowWindow(BOOL show);
		
	void			SetMode( _XQUESTMAIL_MODE questmailmode );
	void			SetMessage( LPSTR messagestring );

};

#endif // !defined(AFX_XWINDOW_QUESTMAIL_H__268291A2_5F36_4493_B362_F27C3E5BC5EB__INCLUDED_)

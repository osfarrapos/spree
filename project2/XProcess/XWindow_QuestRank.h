// XWindow_QuestRank.h: interface for the _XWindow_QuestRank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_QUESTRANK_H__21884D9F_C927_4414_88E6_5B2B86252CEC__INCLUDED_)
#define AFX_XWINDOW_QUESTRANK_H__21884D9F_C927_4414_88E6_5B2B86252CEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

class _XWindow_QuestRank  : public _XWindow
{
private : 
	int				m_RankQuestID;
	BYTE			m_RankQuestType1;		//0:개인퀘스트 1:동행퀘스트 2:단퀘스트 
	BYTE			m_RankQuestType2;		//0:KillCount 1:Timer 2:ItemCount

	BOOL			m_bReceiveRankData;
	
	int				m_indexClan[10];
	TCHAR			m_SelectedQuestSubName[256];		// 퀘스트 부제목
	TCHAR			m_RankOrgName[10][128];
	TCHAR			m_RankName[10][128];
	TCHAR			m_RankLevel[10][128];
	TCHAR			m_RankQuestData[10][128];

	_XStatic		m_BorderTitle;

	_XImageStatic*	m_imageClanIcon;

	
public:
	_XWindow_QuestRank();
	virtual ~_XWindow_QuestRank();
	
	BOOL			Initialize(void);
	void			DestroyWindow(void);
	
	void			Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL			Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void			ShowWindow(BOOL show);
	void			MoveWindow(int X, int Y);
	
	void			SetListItem( _QUEST_RANK_DATA* questrankdata );
	BOOL			SetRankQuestID( int questid );
	int				GetRankQuestID(void) {return m_RankQuestID;};
	BOOL			GetRecieveRankData(void) {return m_bReceiveRankData;};

};

#endif // !defined(AFX_XWINDOW_QUESTRANK_H__21884D9F_C927_4414_88E6_5B2B86252CEC__INCLUDED_)

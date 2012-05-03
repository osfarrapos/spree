// XWindow_QuestWindow.h: interface for the _XWindow_QuestWindow class.
//
// Modified Date	: 2003.02.28
// Author			: Sohyun, Park
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_QUESTWINDOW_H__A5C9237C_4E1B_4210_8F97_F81E8D3C9148__INCLUDED_)
#define AFX_XWINDOW_QUESTWINDOW_H__A5C9237C_4E1B_4210_8F97_F81E8D3C9148__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"

typedef enum
{
	_XQUESTTAB_QUEST1		= 0,
		_XQUESTTAB_QUEST2,
		_XQUESTTAB_QUEST3,
		_XQUESTTAB_QUEST4,
		_XQUESTTAB_QUEST5,
		_XQUESTTAB_TOTALCOUNT
}_XQuestWindowTab;

class _XWindow_QuestWindow : public _XWindow  
{
protected :
	
	// 퀘스트 종류 탭버튼
	_XCheckButton*		m_QuestTab[ _XQUESTTAB_TOTALCOUNT ]; // 퀘스트 종류 : 제룡행, 무림행, 협객행, 기연행 , 무림소사 
	_XButton*			m_LeftButton;
	_XButton*			m_RightButton;
	
	// 퀘스트 이름 바탕 이미지 
	_XImageStatic*		m_QuestNameBorder1;
	_XImageStatic*		m_QuestNameBorder2;

	// 퀘스트 정보 바탕 이미지 
	_XImageStatic*		m_QuestInfomationBorder1;
	_XImageStatic*		m_QuestInfomationBorder2;
	_XImageStatic*		m_QuestInfomationBorder3;

	// 퀘스트 완료 도장 이미지 
	_XImageStatic*		m_QuestCompleteIconImage;
		
	//선택된 퀘스트 이름 
	TCHAR				m_SelectedQuestName[512];			// 선택된 퀘스트의 이름
	TCHAR				m_SelectedQuestSubName[512];		// 퀘스트 부제목
	TCHAR				m_TempSelectedQuestSubName[512];	// 퀘스트 부제목 임시 저장공간
	
	// 퀘스트 설명에서 진행정보와 개요
	LPTSTR				m_lpszQuestProcessInfomation;
	LPTSTR				m_lpszQuestOutline;
	
	int					m_indexProgressQuestGroup;
	int					m_indexCompleteQuestGroup;
	BOOL				m_bSelectedProgressQuestGroup;
	BOOL				m_bSelectedCompleteQuestGroup;
	BOOL				m_bOverFlowQuestSubName;
	
public :
	
	// 퀘스트 취소 버튼 
	_XButton*			m_QuestCancelButton;

	_XQuestWindowTab	m_SelectedTab;
	BOOL				m_ChangeTab;
	int					m_SelectedQuestID;
	int					m_SelectedItemIndex;
	int					m_PrevSelectedItemIndex;
	
	int					m_TotalCountQuestListIndex;
	int					m_GetTotalQuestCount; // 퀘스트 받은 개수 가 몇개 인가 (제룡행 제외) //Author : 양희왕
	
	_XListBox*			m_listboxQuestScriptItem;			// 퀘스트 이름을 표시하는 리스트 박스
	_XListBox*			m_listboxQuestDesc;					// 퀘스트에 관한 설명을 표시하는 리스트 박스 
	_XListBox*			m_listboxQuestReward;				// 퀘스트 보상물을 표시하는 리스트 박스
			
	_XList				m_QuestGroupList;
	int					m_QuestGroupNumber; // 제룡행 몇장인지 표시 

public:
	_XWindow_QuestWindow();
	virtual ~_XWindow_QuestWindow();
	
	BOOL		Initialize( void );
	void		DestroyWindow(void);
	
	void		Draw(_XGUIObject*& pforcusobject = g_CurrentFocusedObject);
	BOOL		Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL		ProcessWheelMessage( short zDelta );
	void		ProcessSelectedListBox();
	void		ProcessSelectedTabButton();
	
	void		MoveWindow(int X, int Y);

	BOOL		CheckMousePositionQuestDescriptionBox(void);
	void		SetListItem(void);
	void		ParseQuestDescString(void);
	
	void		ShowWindow(BOOL show);
	void		SetDefaultPosition( void );
	void		ChangeTab(_XQuestWindowTab selectedtab);
	void		SetParameter(void);
	
	void		SetSelectedQuestID(int id)	{	m_SelectedQuestID = id;		}
	int			GetSelectedQuestID(void)	{	return m_SelectedQuestID;	}
	
	void		SetSelectedItemIndex(int index)		{	m_SelectedItemIndex = index;	}
	int			GetSelectedItemIndex(void)			{	return m_SelectedItemIndex;		}
	
	void		SetQuestGroupListItem(int mainquestid);
	int			GetQuestGroupNumber( int mainquestid);
	int			GetQuestGroupNumber1(int mainquestid);
	int			GetQuestGroupNumber2(int mainquestid);
	int			GetQuestGroupNumber3(int mainquestid);
	int			GetQuestGroupNumber4(int mainquestid);
	int			GetQuestGroupNumber5(int mainquestid);
	int			GetQuestGroupNumber6(int mainquestid);		
	
	void		SetQuestTabButton(void);
	void		SetTempQuestSubName( TCHAR* comsubname, TCHAR* srcsubname );	///< 긴 퀘스트 부제목 축소
};

#endif // !defined(AFX_XWINDOW_QUESTWINDOW_H__A5C9237C_4E1B_4210_8F97_F81E8D3C9148__INCLUDED_)
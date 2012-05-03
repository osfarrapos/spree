// XWindow_NPCScript.h: interface for the XWindow_NPCScript class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XWINDOW_NPCSCRIPT_H_
#define _XWINDOW_NPCSCRIPT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <list>
#include "XKernel.h"
#include "XNPC.h"
#include "XWindow_WorldMinimap.h"
#include "XQuestScriptBaseType.h"

using namespace std;

typedef struct S_SUBSCRIPTITEM
{	
	int		index;
	BYTE	scriptclass;
	int		x;
	int		y;
	int		cx;
	int		cy;
}S_SUBSCRIPTITEM;

//2004.07.01->oneway48 insert
typedef enum _XNPC_Mode
{
	_XNPC_NONE = 0,
	_XNPC_ITEMTRADE,
	_XNPC_WAREHOUSE,
	_XNPC_CONTRACTWAREHOUSE,
	_XNPC_LEARNSKILL,
	_XNPC_REPAIRITEM,
	_XNPC_CONTRIBUTION,
	_XNPC_CREATEORGANIZATION,
	_XNPC_PVPBROKER,
	_XNPC_SOCKETPLUGINEX,
	_XNPC_COLLECTION,
	_XNPC_RESOURCE,
	_XNPC_INCHANT,
	_XNPC_BUFF,
	_XNPC_RANKQUEST,
	_XNPC_MOVEVILLAGE,
	_XNPC_CASTLEINFO,
	_XNPC_CHALLENGEINFO,
	_XNPC_MIXWEAPON,
	_XNPC_FREEMATCHINFO,
	_XNPC_MASTERINFO, //Author : 양희왕 //breif : NPC 사제 창
}_XNPC_MODE;

class _XWindow_NPCScript : public _XWindow  
{
public:	
	_XImageStatic*			m_NPCDialogBorder;
	_XImageStatic*			m_NPCDialogSubBorderA1;
	_XImageStatic*			m_NPCDialogSubBorderA2;
	_XImageStatic*			m_NPCDialogSubBorderB1;
	_XImageStatic*			m_NPCDialogSubBorderB2;
	_XImageStatic*			m_NPCDialogMainTitleBorder;
	_XImageStatic*			m_NPCDialogIMEBorder1;
	_XImageStatic*			m_NPCDialogIMEBorder2;
	_XImageStatic*			m_OrganizationNameIMEBorder1;
	_XImageStatic*			m_OrganizationNameIMEBorder2;
	_XImageStatic*			m_MiniStopEventIMEBorder1;
	_XImageStatic*			m_MiniStopEventIMEBorder2;
	_XImageStatic*			m_EventIMEBorder1;					
	_XImageStatic*			m_EventIMEBorder2;
	_XImageStatic*			m_imageDialogNextPageLeftBorder;
	_XImageStatic*			m_imageDialogNextPageRightBorder;
	_XImageStatic*			m_imageDialogNextPagePoint[2];
	_XImageStatic*			m_imageEventContent01;				// 이벤트 내용을 나타내는 이미지(행운의문자이벤트)
	_XImageStatic*			m_imageEventContent02;				// 이벤트 내용을 나타내는 이미지(훼밀리마트이벤트)
	_XImageStatic*			m_imageEventContent03;				// 이벤트 내용을 나타내는 이미지(예약가입이벤트)
	_XImageStatic*			m_imageEventContent04;				// 이벤트 내용을 나타내는 이미지(상용화이벤트)
	_XImageStatic*			m_imageQuesLevelDisplay;	// 퀘스트 난이도 표시 이미지
	_XImageStatic*			m_imageSavePoint[2];		// 현재 위치를 나타내는 이미지 
	
	// 퀘스트 관련 
	_XButton*				m_btnQuestOK;				// 퀘스트 관련 확인 버튼 
	_XButton*				m_btnBackToNPCMode;			// 퀘스트 대화 끝난 후에  NPC 기능으로 돌아가는 버튼  
	_XButton*				m_btnDialogNextPage;		// NPC대사가 많을때 다음 페이지로 넘기는 버튼 
	_XButton*				m_btnNPCDialogModExit;		// 대화 종료 버튼 - NPC 대화창을 나간다.
	_XButton*				m_ScriptButton[4];
	_XButton*				m_CompleteNameInput;
	_XButton*				m_NampInputCancle;
	_XButton*				m_btnMiniStopEventInput;
	
	_XListBox*				m_listboxNPCDialogMessage;	// NPC 대화를 표시하는 리스트 박스 
	_XListBox*				m_listboxQuestReward;		// 퀘스트에 관련되어서 진행할 때 보상물을 표시하는 리스트 박스
	_XListBox*				m_listboxSelectionItem;		// 퀘스트에서 선택지를 표시할 리스트 박스
	_XListBox*				m_listboxHaveQuestList;		// 가지고 있는 퀘스트 리스트를 표시할 리스트 박스
	
	_XWindow_WorldMinimap*	m_pMiniMapWindowPtr;		// 여장풀기에서 지도를 그리기 위해서 미니맵 윈도우의 포인트를 얻어온다.
	
	LPTSTR					m_pDialogStringTotalBuffer;
	LPSTR					m_lpszNPCDialogMessage;
	
	TCHAR					m_pTitleString[256];
	TCHAR					m_HaveQuestName[256];		// 퀘스트 이름
	TCHAR					m_HaveQuestType[128];		// 퀘스트 타입
	TCHAR					m_QuestString[128];			// 입력창에 입력된 텍스트를 저장
	TCHAR					m_EventCodeString01[6];		// 신규 이벤트 입력창 3개에 저장
	TCHAR					m_EventCodeString02[6];		// 신규 이벤트 입력창 3개에 저장
	TCHAR					m_EventCodeString03[6];		// 신규 이벤트 입력창 3개에 저장
			
	DWORD					m_ChatDisplayTimer;//2004.07.08->oneway48 insert
	DWORD					m_NPCDialogPositionTimeConter;
	DWORD					m_PrevDrawTime;
	
	FLOAT					m_SavePointPositionX;
	FLOAT					m_SavePointPositionY;
	FLOAT					m_EffectFrame;

	int						m_NPCDialogCount;// 2004.06.20->oneway48 close3-2
	int						m_DrawLineCount;//2004.07.08->oneway48 insert
	int						m_NPCDialogPositionCounter;
	int						m_NPCDialogLineCounter;
	int						m_HaveQuestID;				// 정보를 표시할 퀘스트의 아이디 
	int						m_pDialogStringTotalBufferLength;
	int						m_EventType;

	BOOL					m_bFirstDialog;// 2004.04.27->oneway48 insert
	BOOL					m_bNewChatDisplay;
	BOOL					m_bProgressNPCDialogMessage;
	BOOL					m_bPageClickQuestDialog;
	BOOL					m_bQuestStringInputComplete;// 입력창에서 Enter를 눌렀을때 
	
	_XNPC_MODE				m_NPCMode; 
	_XQUEST_TYPE			m_QuestType;// 퀘스트 타입 ( 제룡행, 무림행, 협객행, 기연행, 무림소사 )
	_XNPCMESSAGETYPE		m_MessageType;

	list <S_SUBSCRIPTITEM>	m_SubScriptList;
	

	_XStringSplitInfo		m_NPCDialogSplitinfo;

	_XIMEContainer			m_QuestIMEControl;			// 퀘스트에서 텍스트 비교시에 쓰이는 입력창
	_XIMEContainer			m_OrganizationNameIME;
	_XIMEContainer			m_MiniStopEventIMEControl;

public:
	_XWindow_NPCScript();
	virtual ~_XWindow_NPCScript();
	
	BOOL					Initialize( void );
	void					DestroyWindow( void );
	void					Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL					Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL					ProcessWheelMessage( short zDelta );
	void					ProcessNPCDialog(void);
	void					ProcessNPCDialogAtOnce(void);
	
	void					SetTitle( LPSTR titlestring );
	void					SetMessage( LPSTR messagestring );
	
	void					SetSubScriptList(int index, BYTE scriptclass);
	void					ClearSubScriptList(void);
	void					GenerateSubScriptList(void);
	
	void					ShowWindow(BOOL show);	
	void					SetDefaultPosition( void );
	
	BOOL					GetShowWindow(void);
	
	void					ChangeButton(void);
	
	void					InsertNPCChatString( LPSTR message, int messagemode,  BOOL mainchatwrite = TRUE );// 2004.04.28->oneway48 insert
	
	void					SetHaveQuestID(int questid);
	int						GetHaveQuestID(void)		{	return m_HaveQuestID;	};
	
	void					SetQuestListItem(void);
	void					SetMessageType(_XNPCMESSAGETYPE messagetype);

	BOOL					CheckQuestStringEditBoxArea(void);
	BOOL					CheckOrganizationEditBoxArea(void);
	BOOL					CheckEventEditBoxArea(void);			// 이벤트시 키 값을 입력하는 박스의 영역 체크 
	BOOL					CheckEvent01EditBoxArea(void);			// 예약가입 이벤트 시 키값을 입력하는 세개의 박스 중 하나
	BOOL					CheckEvent02EditBoxArea(void);			// 예약가입 이벤트 시 키값을 입력하는 세개의 박스 중 하나
	BOOL					CheckEvent03EditBoxArea(void);			// 예약가입 이벤트 시 키값을 입력하는 세개의 박스 중 하나
		
	void					SetMiniMapWindow(_XWindow_WorldMinimap* minimapwindow);	
	void					SetSavePointPosition( int npcid);
	
	// 분파 관련 함수들 ----------------------------------=	
	void					ClickOrganizationBtn(void);
	// ---------------------------------------------------=
	
};

#endif // !defined(AFX_XWINDOW_NPCSCRIPT_H__C651EDD5_1E98_478A_85B7_902BDB82B0F4__INCLUDED_)

// XWindow_ServerUnification.h: interface for the _XWindow_ServerUnification class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_SERVERUNIFICATION_H__BF83032F_B6C8_4662_AE35_FA64F1CEB4C0__INCLUDED_)
#define AFX_XWINDOW_SERVERUNIFICATION_H__BF83032F_B6C8_4662_AE35_FA64F1CEB4C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XNetwork.h"
#include "packet_for_unigrouplobby.h"

/*! 서버통합 단계 분류
	- 주의사항
	- 서버 선택
	- 창고 및 캐릭터 선택
	- 캐릭터명 중복 체크
	- 완료
*/

enum UNIFICATIONSTEP 
{ 
	SU_READRULE = 0,		
	SU_SELECTSERVER,		
	SU_SELECTCHARAC,
	SU_CHECKCHARAC,			
	SU_FINISH,				
};

//! 왼쪽 메뉴 텍스트 정보 
struct ServerUnificationInfo 
{
	TCHAR	Title[128];
	TCHAR	Infomation[10][256];
	char	InfomationLineCount;

	void reset()
	{		
		memset( Title, 0, sizeof(TCHAR) * 128 );
		memset( Infomation, 0, sizeof(TCHAR) * 10 * 256 );
		InfomationLineCount = 0;
	};
};

class _XWindow_ServerUnification : public _XWindow  
{
private:
	_XImageStatic				m_MenuEdge_Top;
	_XImageStatic				m_MenuEdge_Bottom;
	_XImageStatic				m_MenuEdge_Left;
	_XImageStatic				m_MenuEdge_Right;
	
	_XCheckButton*				m_checkbtnList[12];

	_XButton					m_NextButton;
	_XButton					m_PrevButton;
	_XButton					m_ExitButton;
	_XButton*					m_ChangeButton[3];
	//_XButton*					m_CompleteButton;

	_XImageStatic				m_TitleLeft;
	_XImageStatic				m_TitleRight;

	_XImageStatic				m_BoxLeft;
	_XImageStatic				m_BoxCenter;
	_XImageStatic				m_BoxRight;

	_XImageStatic				m_WidthLine;
	_XImageStatic				m_HeightLine;	

	_XImageStatic				m_ServerName;	

	UNIFICATIONSTEP				m_UnificationStep;

	ServerUnificationInfo*		m_TextInfo;
	SIZE						m_StepWindowSize;
	TCHAR						m_UnificationServerName[3][128];

	_XImageStatic				m_SelectBorderBox;
	_XImageStatic				m_SelectWarehouseBox;

	//! 모든 서버군 정보
	MSG_UG_SIMPLE_GROUP_INFO	m_Simple_Group_Info;		
	//! 선택한 서버
	int							m_SelectServerIndex;		
	//! 선택된 서버군 정보
	MSG_UG_DETAIL_GRP_INFO		m_Detail_Group_Info;		
	//! 선택된 창고 서버
	int							m_SelectWarehouseIndex;		
	//! 선택된 캐릭
	int							m_SelectCharacterIndex[3];	
	//! 최종 결과
	MSG_UG_MOVE_RESULT_COMPLETE m_MoveResult;					
	
public:
	_XWindow_ServerUnification();
	virtual ~_XWindow_ServerUnification();

	BOOL				Initialize(_XTextureManager& texturearchive1, _XTextureManager& texturearchive2);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	
	void				MoveWindow(int X, int Y);
	void				SetUnificationStep(UNIFICATIONSTEP step);
	UNIFICATIONSTEP		GetUnificationStep(void) { return m_UnificationStep; };

	void				DrawReadRule(void);
	void				DrawSelectServer(void);
	void				DrawSelectCharac(void);
	void				DrawCheckCharac(void);
	void				DrawFinish(void);

	//! 선택된 서버
	int					GetSelectServerIndex(void) { return m_SelectServerIndex; 	};
	//! 선택된 창고
	int					GetSelectWarehouseIndex(void) { return m_SelectWarehouseIndex; 	};
	int*				GetSelectCharacterIndex(void) { return m_SelectCharacterIndex; };
	//! 선택된 캐릭터 수
	BOOL				GetSelectCharacterCount(void);

	//! 서버 - 기본정보
	void				SetSimpleGroupInfo(MSG_UG_SIMPLE_GROUP_INFO* pInfo);
	//! 서버 - 상세정보
	void				SetDetailGroupInfo(MSG_UG_DETAIL_GRP_INFO* pInfo);
	//! 서버 - 캐릭이동 결과
	void				SetMoveResult(MSG_UG_MOVE_RESULT_COMPLETE* pInfo);
	//! 서버 - 캐릭이름 변경 결과
	void				SetChangeNameResult(MSG_UG_CHANGE_NAME_RESULT* pInfo);
	//! 체크버튼 클릭 처리
	void				ClickCheckButton(char index);
};

#endif // !defined(AFX_XWINDOW_SERVERUNIFICATION_H__BF83032F_B6C8_4662_AE35_FA64F1CEB4C0__INCLUDED_)

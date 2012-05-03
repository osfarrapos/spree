// XWindow_Group.h: interface for the _XWindow_Group class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_GROUP_H__452F2A05_6E59_47A7_94DD_C88636A496DA__INCLUDED_)
#define AFX_XWINDOW_GROUP_H__452F2A05_6E59_47A7_94DD_C88636A496DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "packet_for_organization.h"

/**
 * \brief 영웅단 창- 고정
 * 영웅단 관련 정보를 보여준다.
 * 영웅단 관련 기능 제공
*/

/// 영웅단 공지사항 모드
typedef enum
{
	_XNOTICEMODE_READ		= 0,
	_XNOTICEMODE_WRITE,
	_XNOTICEMODE_MODIFY
}_XNoticeMode;

/// 영웅단 공지 정보
struct XORGANIZATION_NOTICEINFO
{
	TCHAR		writer[13] ;
	TCHAR		writedate[15] ;
	TCHAR		notice[272] ;

	void reset()
	{
		memset( writer, 0, sizeof(writer) );
		memset( writedate, 0, sizeof(writedate) );
		memset( notice, 0, sizeof(notice) );
	} ;

} ;

typedef enum
{
	_XGROUPTAB_GROUPCLASS1 = 0,	// 영웅단
	_XGROUPTAB_GROUPCLASS2,	// 방회
	_XGROUPTAB_GROUPCLASS3,	// 장원
	_XGROUPTAB_GROUPCLASS4,	// 장원전황
#ifdef _XDEF_MASTER_AND_DISCIPLE_080721_MAGVIPER //Author : 양희왕 //breif : 사제시스템
	_XGROUPTAB_GROUPCLASS5,	// 사제
#endif
	_XGROUPTAB_TOTALCOUNT
}_XGroupWindowTab;



/// 공지사항 최대글자수
#define _XDEF_MAX_NOTICELENGTH	271

class _XWindow_MnDGroup; //Author : 양희왕 //breif: 사제관련 헤더 선언하면 컴파일 길어진다..

class _XWindow_Group : public _XWindow  
{
public:	
	// 공통
//#ifdef _XTS_ALIANCE_20061018	
	_XCheckButton*			m_BaseTabButton[_XGROUPTAB_TOTALCOUNT];
//#else	
//	_XCheckButton*			m_BaseTabButton;
//#endif
	
	// 영웅단 관련 탭 
	_XStickGauge*			m_pCotributionBorder;
	_XImageStatic*			m_pContributionGaugeLeftBorder;
	_XImageStatic*			m_pContributionGaugeRightBorder;	
	
	_XImageStatic*			m_pImageContributionRateLeftBorder;		// 정렬 방식 창 바탕 1
	_XImageStatic*			m_pImageContributionRateRightBorder;	// 정렬 방식 창 바탕 2
	_XImageStatic*			m_pImageLineUpLeftBorder;				// 정렬 방식 창 바탕 1
	_XImageStatic*			m_pImageLineUpRightBorder;				// 정렬 방식 창 바탕 2

	// 방회 관련 탭 	
#ifdef _XTS_ALIANCE_20061018	
	_XImageStatic*			m_pImageAlienceBorder;					// 영웅단 동맹 창 바탕
	_XImageStatic*			m_pImageAlienceName1;					// 영웅단 동맹 창 '방'자
	_XImageStatic*			m_pImageAlienceName2;					// 영웅단 동맹 창 '회'자
	_XImageStatic*			m_pImageAlienceMemberBorder;			// 영웅단 동맹 맴버 버튼 테두리
	_XImageStatic*			m_pImageAlienceMemberEmptyBorder;		// 영웅단 동맹 맴버공석 이미지 
#endif
#ifdef _XDEF_CASTLEBATTLE	
	_XImageStatic*			m_pImageCastleBattleBorder1;			// 장원전 텍스드 바탕 이미지 1
	_XImageStatic*			m_pImageCastleBattleBorder2;			// 장원전 텍스드 바탕 이미지 2
#endif
	
	// 영웅단 관련 탭 
	_XListBox*				m_OrganizationListBox;
	_XListBox*				m_SortTypeList;
	
	
	_XVScrollBar*			m_NoticeScrollBar;
	
	// 영웅단 관련 탭 
	_XButton*				m_pNoticeWrite;
	_XButton*				m_pNoticemodify;
	_XButton*				m_pBtnGroupLeave;		// 영웅단 탈퇴 버튼
	_XButton*				m_pBtnLineUpDropDown;	// 영웅단 정렬 버튼 

	_XButton*				m_PopUpButton[8];
	
	// 방회 관련 탭 
#ifdef _XTS_ALIANCE_20061018	
	_XButton*				m_pBtnAlienceName[5];	// 영웅단 동맹 각각을 표시하는 버튼 
	_XButton*				m_pBtnAlienceDestroy;	//영웅단 동맹 파기 버튼
#endif

#ifdef _XDEF_ALIANCE_WEBBOARD_20070130	
	_XButton*			m_pBtnGoToOrganization;		// 영웅단 게시판 바로가기 버튼
	_XButton*			m_pBtnGoToAliance;			// 영웅회 게시판 바로가기 버튼
#endif

#ifdef _XDEF_CASTLEBATTLE	
	_XButton*			m_pBtnCastleBattleTimeChange;		// 장원전 시간 변경 버튼
	_XButton*			m_pBtnCastleBattleGetMoney;			// 장원전 보유금 획득 버튼
		
	_XImageStatic*		m_pScheduleIcon[2];
	TCHAR				m_strDate[3][256];
	TCHAR				m_strPropertyDate[256];

	_XListBox*			m_lstboxChallengers;
	_XCastleInfo*		m_pMyCastleInfo;
	
	BOOL				m_bSetCastleTooltip;
#endif
	
	
	// 영웅단 관련 탭 
	list<_Unit*>			m_OrganizationMemberList;	///< 영웅단원들의 데이타 	
	list<_OutTime*>	m_OrganizationMemberOutTimeList;	///< 영웅단원들의 최종 접속 종료 시간	
	
	// 방회 관련 탭 
#ifdef _XTS_ALIANCE_20061018
	list<__ally_client_info*>	m_listAlienceMember;	// 영웅단 동맹 맴버들의 영웅단 이름, 인덱스
#endif

	_XImageStatic			m_CornerImage[4];
	_OrganizationBase		m_OrganizationBaseInfo;	
	
	XORGANIZATION_NOTICEINFO	m_NoticeText;			///< 현재 적용되어있는 영웅단 공지	
	XORGANIZATION_NOTICEINFO	m_SendNoticeTempText;	///< 변경할 영웅단 공지 
	_XIMEContainer			m_NoticeIMEControl;

	TCHAR					m_GroupInfoText[9][64]; 
	TCHAR					m_SendName[13];
	TCHAR					m_CurrentSortName[128];	///< 현재 적용되는 정렬타입명
	TCHAR					m_NoticeTempText[256][128];
	TCHAR					m_NoticeViewText[256][128];	
	
	// 방회 관련 탭 
#ifdef _XTS_ALIANCE_20061018
	TCHAR					m_AlienceMasterName[13];		// 회주 이름을 저장한다.
#endif
	_Unit					m_UpdateUnit;

	POINT					m_DrawPosPopUp;
	
	// 영웅단 관련 탭 
	int						m_PopUpCount;
	int						m_ManagerCount;

	int						m_NoticeTextScrollPos;
	int						m_NoticeTextLength;
	int						m_NoticeTextTotalCount;
	int						m_CurrentNoticeLinePos;	
	int						m_NoticeViewTextTotalCount;
	int						m_CurrentTargetedUserID;
	
	// 방회 관련 탭 
#ifdef _XTS_ALIANCE_20061018
	int						m_UniqueIndexAlience;		// 영웅단 동맹 고유 인덱스
#endif

	
	// 영웅단 관련 탭 
	BOOL					m_ShowPopUpLine;

	/// 오름차순, 내림차순 정렬 플래그
	BOOL					m_SortLevelFlag;	
	BOOL					m_SortZoneFlag;
	BOOL					m_SortOnlineFlag;
	
	// 방회 관련 탭 
		
	_XGroupWindowTab		m_SelectedTab;	//군웅창 관련 탭 : 영웅단, 방회
	_XNoticeMode			m_NoticeMode;
	
	// 장원 전황
	_XImageStatic*			m_TableBarLong;
	_XImageStatic*			m_TableBarShort;
	_XListBox*				m_lstboxOrgRecord;

	int						m_nEntryCount;	// 장원전 참가회수

	int						m_nWinCount;		// 승리 회수
	int						m_nLoseCount;		// 패전 회수
	int						m_nCombatCount;	// 총 회수
	int						m_nWinRate;		// 누적 승률

	int						m_nManorIndex;	// 마지막 장원전 진행한 곳

	// 마지막 장원진 진행 날짜
	int						m_nYear		: 8;
	int						m_nMonth		: 8;
	int						m_nDay		: 8;
	int						reserved	: 8;
	TCHAR					m_strLastWarDate[128];

	_XWindow_MnDGroup*		m_pMnDWindow;	//Author : 양희왕 //breif : 사제관련 탭

	
public:
	_XWindow_Group();
	virtual ~_XWindow_Group();

	BOOL 		Initialize( void );
	void		DestroyWindow( void );
	void		ResetData();
	
	void 		MoveWindow( int X, int Y );

	void 		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	void		DrawPopUp();
	BOOL		Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void		ShowWindow(BOOL show);
	void		ChangeTab(_XGroupWindowTab selectedtab);

	void		RefreshData(void);
	void		SetChallengerListItem(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											영웅단 관련 함수													//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void		CopyBaseInfo(_OrganizationBase* data);		///< 서버 -> 영웅단 기본정보	
	void		CopyMemberList(int count, _Unit* unitlist);	///< 서버 -> 영웅단원 정보 	
	void		CopyMemberOutTimeList( int count, _OutTime* unitouttimelist );	///< 서버 -> 영웅단원 최종접속시간 정보 	
	void		UpdateTextInfo();	///< 서버 -> 영웅단 공지 업데이트	
	void		UpdateOrganizationListBox();	///< 서버 -> 영웅단원 정보 업데이트
	
	
	void		JoinOrganizationMember(_Unit* unit, _OutTime* outtime);		///< 영웅단원 가입	
	void		DeleteOrganizationMember(_Unit* unit, _OutTime* outtime);	///< 영웅단원 자진탈퇴	
	void		DeleteCompulsionOrganizationMember(_Unit* unit, _OutTime* outtime);	///< 영웅단원 추방	
	void		UpdateOrganizationMember(_Unit* unit, _OutTime* outtime);	///< 영웅단원 업데이트	
	void		UpdateOrganizationEntrust(_Unit* unit);	///< 영웅단원 클래스 변경 	
	void		BreakUpOrganization(void);	///< 영웅단 해체
	// ------------------------------------------------=

	TCHAR*		GetSendTargetName() { return m_SendName;	};
	void		SetSendTargetName(TCHAR* targetname) { strcpy( m_SendName, targetname ); 	};
	void		ResetSendTargetName() { memset(m_SendName, 0, sizeof( TCHAR) * 13 );	};
	void		SetUpdeteUnit(_Unit* unit)  { m_UpdateUnit = *unit; };
	_Unit*		GetUpdateUnit() { return &m_UpdateUnit;	};
	int			GetManageCount() { return m_ManagerCount;	};
	int			GetTotalMemberCount() { return m_OrganizationBaseInfo.m_usCurMemberNum;	};

	void		ProcessPopUp(); ///< 영웅단 메뉴 오픈 - 단장 이상급만
	void		ClosePopUpMenu();

	// Network Send 관련 함수 ----------------------------=		
	void		GrantUnit();	///< 단장권한 부여 - 단주일 경우 	
	void		GrantCancleUnit();	///< 단장권한 박탈 - 단주일 경우 	
	void		TrustUnit();	///< 단주 위임 - 단주일 경우	
	void		PrugeUnit();	///< 추방 - 단주일 경우 	
	void		LeaveOrganization();	///< 탈퇴 - 단주일 경우 탈퇴 불가능(단원이 3이하면 영웅단 자동 해체)
	// ---------------------------------------------------=

	// 정렬 함수 -----------------------------------------=
	/// 레벨에 따른 정렬 ( m_SortLevelFlag 오름/내림 가능 )
	void		LineUpLevel();
	void		SortLessLevelUp( int unitindex, BOOL sortflag );
	/// 지역에 따른 정렬 ( m_SortZoneFlag 오름/내림 가능 )
	void		LineUpZone();
	void		SortLessZone( int unitindex, BOOL sortflag );
	/// 접속여부에 따른 정렬 ( m_SortOnlineFlag 오름/내림 가능 )
	void		LineUpOnline();
	void		SortLessOnline( int unitindex, BOOL sortflag );

	void		ClickSortButton( void );	///< 정렬리스트 View On/Off	
	void		SelectSortType( void );		///< 정렬 버튼 클릭시 타입에 따른 처리
	// ---------------------------------------------------=

	void		CalculateNoticeTextength( void );	///< 영웅단 공지 글자수 계산	
	void		SelectNoticeMode( _XNoticeMode mode);	///< 영웅단 공지모드 선택 - 읽기, 쓰기, 수정
	
	BOOL		ProcessWheelMessage( short zDelta );
	
	void		CopySendNoticeText( void );	///< 작성된 공지사항 서버로 Send	
	void		SetNoticeText(TCHAR* text, bool modifymode = false );	///< 공지사항 셋팅 (서버에서 받았을 시 , 수정 했을 시 )
	

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//											방회 관련 함수														//
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
#ifdef _XTS_ALIANCE_20061018	
	void		SetAlienceButton(void);			// 영웅단 동맹 멤버를 표시하는 버튼을 만든다.
	void		ResetAlienceInfo(void);
#endif
	
	void		SetCastleData(void);
	void		ShowCastleTooltip(void);
	void		HideCastleTooltip(void);
	void		ResetCastleData(void);
	
	// 장원전 관련 함수
	void		DrawOwnerInfo(void);
	void		DrawCBOrgTotalRecord(void);
	void		SetRecordListItem(int count, _PerOrgRecord* record);
	void		ResetRecordData(void);

#ifdef ALL_RB_RENEWALCHAT_BUGFIXED_070611_KERYGMA
	BOOL		IsMember( LPTSTR charactername );
#endif

};

#endif // !defined(AFX_XWINDOW_GROUP_H__452F2A05_6E59_47A7_94DD_C88636A496DA__INCLUDED_)

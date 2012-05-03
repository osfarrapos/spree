// XWindow_MiniTooltip.h: interface for the _XWindow_MiniTooltip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_MINITOOLTIP_H__442EF62D_C27A_4BEF_B7FE_E1A086466FEB__INCLUDED_)
#define AFX_XWINDOW_MINITOOLTIP_H__442EF62D_C27A_4BEF_B7FE_E1A086466FEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "XKernel.h"
#include "XWindow.h"
/**
 * \brief 툴팁창 
 * 마우스 오버시 뜨는 모든 툴팁
 *
 * 개선의 요지가 있음. 
 * 확장성이 너무 떨어짐(인터페이스 구성 관련 기획팀과 상의가 필요)
 *
 * 정보에 따른 사이즈 조절( 가로는 불변)
 * 주의 사항 
 * 영약 단계별 최대치 표시를 소스안에서 함 
 * 수치 변경시 MTMODE_CHARINFO_ABILITYSTEP로 검색해서 MaxValue값을 직접 변경해야 함
*/

/** 하나의 툴팁에 필요한 정보들
 *	하나의 툴팁에 필요한 정보가 늘어 날 경우 확장 해야 함 		
*/
typedef struct _tag_Mini_ItemInfo
{
	TCHAR NameBuf[128];	
	TCHAR LData_Buf[512];
	TCHAR Data_Buf[12][128];	
}_MINIITEMTOOLTIP_INFO;

/// 툴팁 타입
typedef enum 
{
	MTMODE_NONE=0,
	MTMODE_ITEM,					///< 퀵슬롯 아이템
	MTMODE_SKILL,					///< 퀵슬롯 스킬
	MTMODE_BUFF_EXTERNAL,			///< 버프창 외상
	MTMODE_BUFF_INTERNAL,			///< 버프창 내상
	MTMODE_BUFF_ITEM,				///< 버프창 아이템
	MTMODE_BUFF_SKILL,				///< 버프창 스킬
	MTMODE_BUFF_EFFECT,				///< 버프창 상태효과
	MTMODE_BUFF_MONSTATS_SPELL,		///< 몬스터창 스킬
	MTMODE_BUFF_MONSTATS_EFFECT,	///< 몬스터창 상태효과
	MTMODE_BUFF_MONSTATS_STATE,		///< 몬스터창 상태
	MTMODE_CHARINFO,				///< JinStatus창
	MTMODE_CHARINFO_ABILITY,		///< 인물정보창 능력 정보
	MTMODE_CHARINFO_ABILITYSTEP,	///< 인물정보창 영약 정보
	MTMODE_CHARINFO_INTOXICATION,	///< 영약 중독 정보
	MTMODE_CHARINFO_FAME,			///< 명성 정보
	MTMODE_PARTYINFO,				///< 파티 정보
	MTMODE_BUFF_INTOXICATION,		///< 영약 중독
	MTMODE_BUFF_NICKNAME,			///< 별호 버프
	MTMODE_BUFF_STUDYBOOK,			///< 무공서 버프
	MTMODE_PAYMENTINFO,				///< SDS 전용
	MTMODE_BUFF_PEMODE,				///< 공공의적 버프
	MTMODE_PK_RECORD,				///< 대전전황
	MTMODE_PK_TRACINGSERVICES,		///< PK 위치추적 정보
	MTMODE_ITEM_SHOUT,				///< 외치기
	MTMODE_ITEM_ACCOUNT,			///< 경험치/숙련도/구룡기행
	MTMODE_VIETNAMPAYMENTINFO,		///< 베트남 셧다운
	MTMODE_VIEWGROUP_CASTLEINFO,	///< 군웅창 장원정보
	MTMODE_PCRPREMIUM,				///< PC방 윈도우
	MTMODE_CASTLEINFO,				///< 장원 소유주 정보
	MTMODE_NOTORIOUS,				///< 평판 정보
	MTMODE_NPCBUFFINFO				///< NPC 버프 정보
}_MINITOOLTIP_MODE;

class _XWindow_MiniTooltip : public _XWindow  
{
private:
	_MINIITEMTOOLTIP_INFO	m_CurrentMiniTooltipInfo;
	_MINITOOLTIP_MODE		m_CurrentMiniTooltipMode;
	SIZE					m_MiniTooltipSize;

	int						m_CurrentDrawPosY;
	int						m_CurrentSlotNumber;
	int						m_ItemType;
	int						m_ItemId;
	DWORD					m_TimeCount;
	_XImageStatic*			m_RoundEdgeImage[4];

	_XImageStatic*			m_pGroupIcon;

	void	ResetMiniTooltipInfo();
	
public:
	_XWindow_MiniTooltip();
	virtual ~_XWindow_MiniTooltip();

	BOOL	Initialize(void);
	void	DestroyWindow(void);
	
	void	Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL	Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void	DrawMiniItem();
	void	DrawMiniSkill();
	void	DrawCharInfo();
	void	DrawCharInfoAbility();
	void	DrawCharInfoAbilityStep();
	void	DrawCharInfoIntoxication();	
	void	DrawCharInfoFame();
	void	DrawMiniBuffExternal();
	void	DrawMiniBuffInternal();			
	void	DrawMiniBuffItem();			
	void	DrawMiniBuffSkill();
	void	DrawMiniBuffEffect();
	void	DrawMonstatsSpell();
	void	DrawMonstatsEffect();
	void	DrawMonstateState();
	void	DrawPartyInfo();
	void	DrawMiniBuffIntoxication();
	void	DrawMiniBuffNickname();
	void	DrawMiniBuffStudyBook();
	void	DrawMiniBuffPeMode();
	//void	DrawPaymentMethod(); 양희왕 주석처리
	void	DrawPaymentMethod_NormalUser();
	void	DrawPaymentMethod_Premium1User();
	void	DrawPaymentMethod_Premium2User();
	void	DrawPaymentMethod_NormalPCRoom();
	void	DrawPaymentMethod_PremiumUser1NormalPCRoom();
	void	DrawPaymentMethod_PremiumUser2NormalPCRoom();
	void	DrawPaymentMethod_PremiumPCRoom();
	void	DrawPaymentMethod_PremiumUser1AndPCRoom();
	void	DrawPaymentMethod_PremiumUser2AndPCRoom();	
	void	DrawPKRecord();
	void	DrawVietnamPaymentMethod();

	void	DrawMiniItemShout();
	void	DrawAccountItemUse();

	void	DrawViewGroupCastleInfo();

	void	DrawCastleInfo();

	void	ShowWindow(BOOL show);

	BOOL	SetTooltipMode( _MINITOOLTIP_MODE mode, int slotnumber, int type, int id, DWORD Time = 0, int DrawX = 0, int DrawY = 0 );
	BOOL	SetTooltipInfo();
	_MINITOOLTIP_MODE GetMiniTooltipMode() { return m_CurrentMiniTooltipMode; }
	int     GetCurrentSlotNumber() { return m_CurrentSlotNumber; }

	int		GetItemId()		{	return m_ItemId;	}

private:
	void	DrawPaymentInfo();	//!< @author 양희왕
	void	DrawPkTracingservices(); //!< @author 양희왕
	void	DrawCharInfoNotorious();	//Author : 양희왕 //breif : 평판
	void	DrawNPCBuffWindowInfo(); //Author : 양희왕 //breif : 점혈술 설명창 (미국에서만 사용 )
	void	DrawPCRPremium();
};

#endif // !defined(AFX_XWINDOW_MINITOOLTIP_H__442EF62D_C27A_4BEF_B7FE_E1A086466FEB__INCLUDED_)

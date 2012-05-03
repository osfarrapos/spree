// XWindow_TargetInfo.h: interface for the _XWindow_TargetInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_TARGETINFO_H__E352666A_2C36_4C82_8BC0_703D946AF153__INCLUDED_)
#define AFX_XWINDOW_TARGETINFO_H__E352666A_2C36_4C82_8BC0_703D946AF153__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

/**
 * \brief 상대방 정보 보기 창
 * 간략보기는 상대방 동의 없이 가능 
 * 나머지는 동의 하에 볼 수 있음
*/

enum TargetInfoMode {
	tm_none = 0,
	tm_simple_info = 2,	///< 캐릭터 간략 정보
	tm_items_info,		///< 아이템 정보
	tm_ability_info,	///< 능력치 정보
	tm_all_info,		///< 전체 정보
};

/// 상대방 정보
struct _TARGETINFO
{
	char	cTargetName[128] ;			///< 대상 이름 
	
	short	sInnerLevel ;				///< 성취단계
	char	cMoonpa ;					///< 문파
	char	cClass ;					///< 직책	
	char	cClassGrade ;				///< 역할 단계

	short	sConstitution ;				///< 건강
	short	sZen ;						///< 진기
	short	sIntelligence ;				///< 지혜
	short	sDexterity ;				///< 민첩
	short	sStr ;						///< 근력

	/// 아이템 장착 위치
	enum _GET_ON_ITEMS_SLOT
	{
		pos_amor=0,         // 0
		pos_pants,			// 1
		pos_adae,			// 2
		pos_shoes,			// 3
		pos_cap,			// 4
		pos_gap_upper,		// 5
		pos_gap_lower,		// 6
		pos_backpack,		// 7
		pos_weapon_1,		// 8
		pos_weapon_2,		// 9
		get_on_max
	};
	_XUserItem getOnItems[get_on_max] ;
	
	void reset()
	{
		memset(cTargetName, 0, sizeof(TCHAR)*128);

		sInnerLevel		= 0;			
		cMoonpa			= 0;			
		cClass			= 0;			
		cClassGrade		= 0;			

		sConstitution	= 0;			
		sZen			= 0;			
		sIntelligence	= 0;			
		sDexterity		= 0;			
		sStr			= 0;			
		for( int i = 0; i < get_on_max; i++)
			getOnItems[i].Reset();
	} ;
};

class _XWindow_TargetInfo : public _XWindow  
{
private:
	_XStatic				m_BorderTile;

	int						m_SelectedSlotIndex;			
	_XImageStatic			m_EquipedItemEdgeIcon[5];	///< 장착 아이템의 상태 변화 이미지	

public :
	TargetInfoMode			m_TargetInfoMode;				///< 현재 정보보기 모드
	TargetInfoMode			m_TargetInfoModefromRequest;	///< 내가 요청한 정보보기 모드 
	TargetInfoMode			m_TargetInfoModeToRequest;		///< 상대방이 나한테 요청한 정보보기 모드
	_TARGETINFO				m_SelectedTargetInfo;
	TCHAR					m_SendName[128];					
	TCHAR					m_SendTempName[128];		///< m_SendName 이 있을 경우 저장하는 공간
	int						m_PtoPLevelDef;			///< 상대방과의 레벨 차이 단계
	
public:
	_XWindow_TargetInfo();
	virtual ~_XWindow_TargetInfo();

	BOOL		Initialize( void );
	void		DestroyWindow( void );

	void		ShowWindow(BOOL show);

	void		Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL		Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void		DrawEquippedItemList(void);
	int			SearchSlotRegion(  RECT* pRect  );

	void		MoveWindow(int X, int Y);
	void		DrawItemTooltip(int slotnumber, RECT iconrect);
};

#endif // !defined(AFX_XWINDOW_TARGETINFO_H__E352666A_2C36_4C82_8BC0_703D946AF153__INCLUDED_)

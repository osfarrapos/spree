// XWindow_LifeSkill.h: interface for the _XWindow_LifeSkill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_LIFESKILL_H__BE89E98B_FF4C_4D3C_AB54_9543CEFAA9E7__INCLUDED_)
#define AFX_XWINDOW_LIFESKILL_H__BE89E98B_FF4C_4D3C_AB54_9543CEFAA9E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XCollectionItem.h"

typedef enum
{
	_XLIFETAB_LIFECLASS1		= 0,
		_XLIFETAB_LIFECLASS2,
		_XLIFETAB_LIFECLASS3,
		_XLIFETAB_LIFECLASS4,
		_XLIFETAB_LIFECLASS5,
		_XLIFETAB_TOTALCOUNT
}_XLifeWindowTab;

typedef enum
{
	_XCOLLECTION_TYPE1		= 0,	// 전체 보기
		_XCOLLECTION_TYPE2,			// 하품
		_XCOLLECTION_TYPE3,			// 중품
		_XCOLLECTION_TYPE4,			// 상품
		_XCOLLECTION_TYPE5,			// 명품
		_XCOLLECTION_TYPE6,			// 보물
		_XCOLLECTION_TYPE7,			// 버튼 추가 2007.10.18
		_XCOLLECTIONTYPE_TOTALCOUNT
}_XCollectionType;

class _XWindow_LifeSkill  : public _XWindow
{
public:
	// 공통
	_XImageStatic*		m_imageSelectedButton;		// 정사각형 버튼이 선택되었을 때 테두리 이미지
	_XImageStatic*		m_imageTypeButtonBorder;	// 정사각형 버튼이 바탕 이미지
	_XImageStatic*		m_imageItemBorder;			// 아이템 테두리 
	_XImageStatic*		m_imageCompleteItemBorder;	// 전부 모은 필요아이템 바탕 이미지

	// 수집 관련 탭 
	_XImageStatic*		m_imageCollectionTypeBorder1;		// 수집 타입 바탕 생성 1 
	_XImageStatic*		m_imageCollectionTypeBorder2;		// 수집 타입 바탕 생성 2 

	// 자원 관련 탭 

	// 행동 관련 탭
	_XImageStatic*		m_imageStoreBorder1;		// 개인 상점 바탕 생성 1 ( 44 X 46 )
	_XImageStatic*		m_imageStoreBorder2;		// 개인 상점 바탕 생성 2 ( 100 X 46 )
	_XImageStatic*		m_imageStoreBorder3;		// 개인 상점 바탕 생성 3 ( 12 X 46 )
	_XImageStatic*		m_imageStoreBorderEdge1;	// 개인 상점 바탕 테두리 1 ( 44 X 46 )
	_XImageStatic*		m_imageStoreBorderEdge2;	// 개인 상점 바탕 테두리 2 ( 100 X 46 )
	_XImageStatic*		m_imageStoreBorderEdge3;	// 개인 상점 바탕 테두리 3 ( 12 X 46 )
	_XImageStatic*		m_imagePVPBorder1;			// 비무 홍보 바탕 생성 1 ( 44 X 46 )
	_XImageStatic*		m_imagePVPBorder2;			// 비무 홍보 바탕 생성 2 ( 100 X 46 )
	_XImageStatic*		m_imagePVPBorder3;			// 비무 홍보 바탕 생성 3 ( 12 X 46 )
	_XImageStatic*		m_imagePVPBorderEdge1;		// 비무 홍보 바탕 테두리 1 ( 44 X 46 )
	_XImageStatic*		m_imagePVPBorderEdge2;		// 비무 홍보 바탕 테두리 2 ( 100 X 46 )
	_XImageStatic*		m_imagePVPBorderEdge3;		// 비무 홍보 바탕 테두리 3 ( 12 X 46 )
	_XImageStatic*		m_imageEventBorder1;		// 이벤트 바탕 생성 1 ( 44 X 46 )
	_XImageStatic*		m_imageEventBorder2;		// 이벤트 바탕 생성 2 ( 100 X 46 )
	_XImageStatic*		m_imageEventBorder3;		// 이벤트 바탕 생성 3 ( 12 X 46 )
	_XImageStatic*		m_imageEventBorderEdge1;	// 이벤트 바탕 테두리 1 ( 44 X 46 )
	_XImageStatic*		m_imageEventBorderEdge2;	// 이벤트 바탕 테두리 2 ( 100 X 46 )
	_XImageStatic*		m_imageEventBorderEdge3;	// 이벤트 바탕 테두리 3 ( 12 X 46 )
	_XImageStatic*		m_imagePersonalStoreIcon;	// 개인상점 아이콘 (임시로 사용)
	_XImageStatic*		m_imagePVPMark;				// 비무 마크 아이콘 
	_XImageStatic*		m_imageEventIcon;			// 이벤트 아이콘 
	


	// 공통
	_XButton*			m_btnLifeSkillClassTab;		// 생활 기술이 확장 되면 삭제해야 함. 삭제 예정
	_XCheckButton*		m_btnLifeSkillTab[_XLIFETAB_TOTALCOUNT];				// 생활 스킬 종류 : 수집 자원 수배 행동 기술
	_XButton*			m_btnType[_XCOLLECTIONTYPE_TOTALCOUNT];					// 타입 버튼 : 하품,중품,상품,명품or 대동/석가장...

	// 수집 관련 탭 
	_XButton*			m_btnCollectionNeedItemInput;	// 수집에 필요한 아이템을 다 모으면 명품집에 넣기 위한 버튼
	
	// 자원 관련 탭 

	// 행동 관련 텝
	_XButton*			m_btnLifeSkillUse;			// 개인상점버튼
	_XButton*			m_btnPVPPR;					// 비무홍보버튼
	_XButton*			m_btnEvent;					// 이벤트버튼

	_XListBox*			m_listboxCollectionItem;	// 수집을 표시하는 리스트 박스
	_XListBox*			m_listboxResourceItem;		// 자원을 표시하는 리스트 박스
	_XListBox*			m_listboxResourceNeedItem;	// 자원을 표시하는 리스트 박스
	_XListBox*			m_lstboxCollectionExplain;	// 명품 수집 설명 글을 표시하는 리스트 박스

	_XCollectionItem*	m_pSelectedCollectionItem;	// 선택된 수집 아이템
		
	POINT				m_IconDragStartPos;
	POINT				m_CollectionNeedItemSlotPosition[16];	// 명품수집에서 필요한 아이템 위치 정보
	POINT				m_ResourcNeedItemSlotPosition[9];		// 자원수집에서 필요한 아이템 위치 정보

	int					m_indexSelectedResourceItem;	// 선택된 자원 아이템 인덱스
	int					m_indexCompleteNeedItem;		// 수집 목록에 넣을 필요 아이템 인덱스 
	
	short				m_MouseSlotLifeSkillID;
	
	bool				m_bHaveMouseSlot;
	bool				m_bDragState;
	
	_XLifeWindowTab		m_SelectedTab;
	_XCollectionType	m_SelectedTypeClass;

	int					m_nResourceTabTotalCount;	// 재료수집 탭의 총 갯수 (맵 오픈 여부에 따라 달라짐)
	

public:
	_XWindow_LifeSkill();
	virtual ~_XWindow_LifeSkill();
	
	BOOL				Initialize(void);
	void				DestroyWindow(void);
	
	void				Draw(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);
	BOOL				Process(_XGUIObject*& pfocusobject = g_CurrentFocusedObject);

	void				ShowWindow(BOOL show);
	void				MoveWindow( int X, int Y );
	BOOL				ProcessWheelMessage( short zDelta );

	// 명품 수집 
	BOOL				CheckMousePositionInCollectionItme(void);
	void				ProcessCollectionNeedItem(void);
	void				CheckCollectionCompleteNeedItem(void);
	int					CheckSelectedCollectionNeedItem(void);
	void				SetListItemCollectionExplain(void);				// 명품 설명글 리스트를 구성한다.
	
	// 자원 수집 
	int					CheckSelectedResourceNeedItem(void);

	// 행동 관련 
	void				ResetMouseSlot(void);
	int					CheckSelectedIconItem(void);
	void				DrawDragIcon(void);

	void				ChangeTab(_XLifeWindowTab selectedtab);
	void				ChangeType(_XCollectionType selectedtype);
	void				SetTypeButtonTexture(void);
	void				SetListCollectionItem(void);					// 수집 리스트를 구성한다.
	void				SetListResourceItem(void);						// 자원 리스트를 구성한다.
	void				SetListResourceNeedItem(BOOL bRenewScroll = TRUE );					// 자원을 수집하는데 필요한 리스트를 구성한다.
	void				SetSelectedCollectionItemIndex(int index);		// 선택된 수집 아이템을 복사한다.
	void				SetSelectedResourceItemOrGrupItem(DWORD commandid);				// 선택된 자원 아이템의 인덱스를 얻어낸다.
	
	int					SearchSlotRegion(RECT* pRect = NULL);
	
	inline short		GetMouseSlotLifeSkillID(void)	{	return m_MouseSlotLifeSkillID;	};	
	inline bool			GetHaveMouseSlot(void)	{	return m_bHaveMouseSlot;	};
};

#endif // !defined(AFX_XWINDOW_LIFESKILL_H__BE89E98B_FF4C_4D3C_AB54_9543CEFAA9E7__INCLUDED_)

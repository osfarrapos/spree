// XCollectionItem.h: interface for the _XCollectionItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XCOLLECTIONITEM_H__819D0878_D799_43D2_9057_72386E314D09__INCLUDED_)
#define AFX_XCOLLECTIONITEM_H__819D0878_D799_43D2_9057_72386E314D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItem.h"

#include <list>
using namespace std;

// 명품수집가의 보상을 나타내기 위한 클래스
class _XCollectionRewardSet
{
public:
	int		m_Index;											// 보상 세트 인덱스
	list <_XCollectionResourceItem*>		m_listRewardItem;	// 보상아이템
public:
	_XCollectionRewardSet();
	virtual	~_XCollectionRewardSet();
		
	void	ReleaseRewardItem(void);
	void	InsertRewardItem(  char ctype ,short id, unsigned char count, int index);
};

#if defined(_XENGLISH) || defined(_XRUSSIAN)
#define _XDEF_MAX_COLLECTION_LINE	10
#else
#define _XDEF_MAX_COLLECTION_LINE	3
#endif

// 명품수집가의 목록
class _XCollectionItem  
{
public:

	int		m_indexCollectionList;			// 몇번째 리스트인지 알 수 있는 인덱스 
	int		m_CollectionIndex;				// 명품수집 고유인덱스 
	int		m_NeedItemTotalCount;			// 수집해야 할 아이템 종류 총 갯수
	int		m_CompleteItemCurrentCount;		// 수집 완료한 아이템 종류 갯수
	int		m_CollectionMinimumLevel;		// 수집을 할 수 있는 최소 레벨
	int		m_PrecedeCollectionIndex[5];	// 선행으로 완료해야할 수집 인덱스
	short	m_CollectionVersion;			// 수집 버전
	short	m_CollectionType;				// 수집타입: 하품,중품,상품,명품
	bool	m_bEnableExchage;				// 수집 해야할 아이템을 다 모아서 교환가능한 상태
	bool	m_bComplete;					// 교환까지 완료해서 더이상 진행 할 수 없는 상태

#ifdef _XTS_NEWLIFEWINDOW
	LPTSTR	m_CollectionName;
#else
	TCHAR	m_CollectionName[128];			// 명품수집명 : 가죽수집..
#endif
	
#ifdef _XTS_NEWLIFEWINDOW
	LPTSTR	m_CollectionExplain;
	LPTSTR	m_TempString[_XDEF_MAX_COLLECTION_LINE];
#else
	TCHAR	m_CollectionExplain[_XDEF_MAX_COLLECTION_LINE][512];	// 명품수집에 대한 설명을 나타낸다.
#endif

	int		m_CollectionExplainLineCount;

	
	list <_XCollectionResourceItem*>	m_listNeedItem;		// 명품으로 바꾸기 위해서 필요한 아이템 리스트
	list <_XCollectionRewardSet*>		m_listRewardSet;	// 보상 세트 
	

public:
	_XCollectionItem();
	virtual ~_XCollectionItem();

	void	Release(void);
	void	InsertNeedItem(  char ctype ,short id, unsigned char count, int index );


};




#endif // !defined(AFX_XCOLLECTIONITEM_H__819D0878_D799_43D2_9057_72386E314D09__INCLUDED_)

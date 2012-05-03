// XResourceItem.h: interface for the _XResourceItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XRESOURCEITEM_H__D4E53F12_C036_4160_955C_BE13EA291762__INCLUDED_)
#define AFX_XRESOURCEITEM_H__D4E53F12_C036_4160_955C_BE13EA291762__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XGameItem.h"

#include <list>
using namespace std;
// 자원아이템을 재료로 바꾸어 주는 정보를 가지고 있는 클래스
class _XResourceItem  
{			
public:
	int		m_ResourceIndex;	
	int		m_currentcountNeeditem;			// 현재 완료된 필요한 아이템 갯수
	int		m_totalcountNeeditem;			// 필요한 아이템 총 갯수
	TCHAR	m_ResourceName[256];				// 재료 이름
		
	_XUserItem	m_RewardItem;				// 보상 아이템 
	list <_XCollectionResourceItem*>	m_listNeedItem;		// 재료로 바꾸기 위해서 필요한 아이템 리스트

public:
	_XResourceItem();
	virtual ~_XResourceItem();

	void	Release(void);

};

// 재료리스트 꾸러미
class _XResourcePackage  
{			
public:
	int			m_ResourcePackageIndex;				
	list <int>	m_listResourceItemIndex;	// 재료 인덱스를 가지고 있는 리스트
	
public:
	_XResourcePackage();
	virtual ~_XResourcePackage();
	
	void	Release(void);
	
};


class _XResourceGroupItem
{			
public:
	int			m_ResourceGroupItemIndex;			// 대분류인덱스		대동지역, 석가장 지역 ...
	int			m_ResourceGroupSubItemIndex;		// 소분류인덱스		가죽상인 양한길, 약재상인 박기순...
	int			m_ResourcePackageIndex;				// 재료 패키지 고유 인덱스
	bool		m_bSelectedGroupSubItem;			// 리스트에서 선택되었는지를 알 수 있다.
	TCHAR		m_ResourceGroupSubItemName[256];	// 재료 이름
		
public:
	_XResourceGroupItem();
	virtual ~_XResourceGroupItem();
	
	void	Release(void);
	
};

#endif // !defined(AFX_XRESOURCEITEM_H__D4E53F12_C036_4160_955C_BE13EA291762__INCLUDED_)

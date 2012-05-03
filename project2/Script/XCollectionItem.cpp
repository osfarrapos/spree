// XCollectionItem.cpp: implementation of the _XCollectionItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XCollectionItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// class _XCollectionRewardSet 
//////////////////////////////////////////////////////////////////////

_XCollectionRewardSet::_XCollectionRewardSet()
{
	m_Index = 0;
}

_XCollectionRewardSet::~_XCollectionRewardSet()
{
//	ReleaseRewardItem();
}


void _XCollectionRewardSet::ReleaseRewardItem()
{	
//	if( !m_listRewardItem.empty() )
//	{
//		_XCollectionResourceItem*	currentrewarditem = NULL;
//		list <_XCollectionResourceItem *>::iterator iter;
//		for(iter = m_listRewardItem.begin() ; iter != m_listRewardItem.end() ; iter++)
//		{
//			currentrewarditem  = *iter;
//			if(currentrewarditem)
//			{
//				SAFE_DELETE(currentrewarditem);
//				currentrewarditem = NULL;
//			}
//		}		
//		m_listRewardItem.clear();	
//	}

}

void _XCollectionRewardSet::InsertRewardItem( char ctype ,short id, unsigned char count, int index )
{	
	_XCollectionResourceItem* newrewarditem = new _XCollectionResourceItem;
	newrewarditem->Set_m_cType(ctype);
	newrewarditem->Set_m_sID(id);
	newrewarditem->Set_m_ucCount(count);
	newrewarditem->m_Index		= index;
		
	m_listRewardItem.push_back(newrewarditem);
}




//////////////////////////////////////////////////////////////////////
// class _XCollectionItem 
//////////////////////////////////////////////////////////////////////

_XCollectionItem::_XCollectionItem()
{
	m_indexCollectionList		= -1;
	m_CollectionMinimumLevel	= -1;
	m_CollectionVersion			= 0;
	m_CollectionType			= 0;
	m_NeedItemTotalCount		= 0;		// 수집해야 할 아이템 종류 총 갯수
	m_CompleteItemCurrentCount	= 0;		// 수집 완료한 아이템 종류 갯수
	for( int i = 0; i < 5; ++i)
	{
		m_PrecedeCollectionIndex[i] = -1;
	}
	m_bEnableExchage			= false;	// 수집 해야할 아이템을 다 모아서 교환가능한 상태
	m_bComplete					= false;	// 교환까지 완료해서 더이상 진행 할 수 없는 상태

#ifdef _XTS_NEWLIFEWINDOW
	m_CollectionName = NULL;
	m_CollectionExplain = NULL;
	for(i = 0 ; i < _XDEF_MAX_COLLECTION_LINE ; i++)
		m_TempString[i] = NULL;
#else
	memset( m_CollectionName, 0, sizeof(TCHAR)*128 ); 
	memset( m_CollectionExplain, 0, sizeof(TCHAR)*_XDEF_MAX_COLLECTION_LINE*512 );
#endif
}

_XCollectionItem::~_XCollectionItem()
{
//	Release();
}

void _XCollectionItem::Release()
{	
//	
//	if( !m_listNeedItem.empty() )
//	{
//		_XCollectionResourceItem*	currentneeditem = NULL;
//		list <_XCollectionResourceItem *>::iterator iter;
//		for(iter = m_listNeedItem.begin() ; iter != m_listNeedItem.end() ; iter++)
//		{
//			currentneeditem  = *iter;
//			if(currentneeditem)
//			{	
//				SAFE_DELETE(currentneeditem);
//				currentneeditem = NULL;
//			}
//		}	
//		m_listNeedItem.clear();
//	}
//	
//	if( !m_listRewardSet.empty() )
//	{
//		_XCollectionRewardSet*	currentrewardset = NULL;	
//		_XCollectionResourceItem*	currentrewarditem = NULL;	
//		list <_XCollectionRewardSet *>::iterator iter_rewardset;
//		for(iter_rewardset = m_listRewardSet.begin() ; iter_rewardset != m_listRewardSet.end() ; iter_rewardset++)
//		{
//			currentrewardset = *iter_rewardset;
//			if(currentrewardset)
//			{
//				if( !currentrewardset->m_listRewardItem.empty() )
//				{
//					list <_XCollectionResourceItem*>::iterator iter_rewarditem;
//					for(iter_rewarditem = currentrewardset->m_listRewardItem.begin() ; iter_rewarditem != currentrewardset->m_listRewardItem.end() ; iter_rewarditem++)
//					{
//						currentrewarditem = *iter_rewarditem;
//						if(currentrewarditem)
//						{										
//							SAFE_DELETE(currentrewarditem);
//							currentrewarditem = NULL;
//						}
//					}
//					currentrewardset->m_listRewardItem.clear();
//				}
//			}
//		}	
//	}
//	
//	if( !m_listRewardSet.empty() )
//	{
//		_XCollectionRewardSet*	currentrewardset = NULL;		
//		list <_XCollectionRewardSet *>::iterator iter_rewardset;
//		for(iter_rewardset = m_listRewardSet.begin() ; iter_rewardset != m_listRewardSet.end() ; iter_rewardset++)
//		{
//			currentrewardset = *iter_rewardset;
//			if(currentrewardset)
//			{
//				SAFE_DELETE(currentrewardset);
//				currentrewardset = NULL;
//			}
//		}	
//		m_listRewardSet.clear();
//	}

}


void _XCollectionItem::InsertNeedItem( char ctype ,short id, unsigned char count, int index )
{	
	_XCollectionResourceItem* newreneeditem = new _XCollectionResourceItem;
	newreneeditem->Set_m_cType(ctype);
	newreneeditem->Set_m_sID(id);
	newreneeditem->Set_m_ucCount(count);
	newreneeditem->m_Index		= index;
	
	m_listNeedItem.push_back(newreneeditem);
}








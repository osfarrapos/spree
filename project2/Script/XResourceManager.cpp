// XResourceManager.cpp: implementation of the _XResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XResourceManager.h"
#include "XResourceItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XResourceManager::_XResourceManager()
{
	m_arrayResourceItem = NULL;

}

_XResourceManager::~_XResourceManager()
{
	Release();

}

void _XResourceManager::Release()
{		
	_XResourceGroupItem* currentgroupitem		= NULL;
	_XResourcePackage* currentresourcepackage	= NULL;
	_XCollectionResourceItem* currentneeditem	= NULL;
	
	// 자원 리스트 속의 수집 아이템 삭제
	for( int i = 0; i < m_TotalCountResourceItem; ++i)
	{
		list <_XCollectionResourceItem*>::iterator iter_needitem;
		for(iter_needitem = m_arrayResourceItem[i].m_listNeedItem.begin() ; iter_needitem != m_arrayResourceItem[i].m_listNeedItem.end() ; ++iter_needitem)
		{
			currentneeditem = *iter_needitem;
			if(currentneeditem)
			{			
				SAFE_DELETE(currentneeditem);
				currentneeditem = NULL;
			}
		}
		m_arrayResourceItem[i].m_listNeedItem.clear();
	}
	//끝 - 자원 리스트 속의 수집 아이템 삭제

	// 자원 리스트 삭제
	SAFE_DELETE_ARRAY(m_arrayResourceItem);

	// 자원 패키지 속의 재료 인덱스 리스트 삭제	
	list <_XResourcePackage *>::iterator iter;
	for(iter = m_listResourcePackage.begin() ; iter != m_listResourcePackage.end() ; ++iter)
	{
		currentresourcepackage = *iter;
		if(currentresourcepackage)
		{
			currentresourcepackage->m_listResourceItemIndex.clear();
		}
	}	
	// 끝 - 자원 패키지 속의 재료 인덱스 리스트 삭제

	// 자원 패키지 삭제
	for(iter = m_listResourcePackage.begin() ; iter != m_listResourcePackage.end() ; ++iter)
	{
		currentresourcepackage = *iter;
		if(currentresourcepackage)
		{
			SAFE_DELETE(currentresourcepackage);
			currentresourcepackage = NULL;
		}
	}		
	m_listResourcePackage.clear();
	// 끝 - 자원 패키지 삭제
	
	// 그룹 삭제
	list <_XResourceGroupItem *>::iterator iter_gropitem;
	for(iter_gropitem = m_listResourceGroupItem.begin() ; iter_gropitem != m_listResourceGroupItem.end() ; ++iter_gropitem)
	{
		currentgroupitem = *iter_gropitem;
		if(currentgroupitem)
		{
			SAFE_DELETE(currentgroupitem);
			currentgroupitem = NULL;
		}
	}	
	m_listResourceGroupItem.clear();
	// 끝 - 그룹 삭제
}

BOOL _XResourceManager::LoadResourceItem(LPCTSTR filename)
{
	if(!filename || filename[0] == NULL)
		return FALSE;
	
	FILE* fileptr = NULL;
	fileptr = fopen(filename, "rb");
	if(!fileptr)
	{
		_XFatalError(_T("Quest script file open"));
		return FALSE;
	}
	
	if(!LoadResourceItem(fileptr))
	{
		_XFatalError("Quest script loading [%s]", filename);
		fclose(fileptr);
		return FALSE;
	}
	
	fclose(fileptr);
	return TRUE;
}

BOOL _XResourceManager::LoadResourceItem(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;
	
	
	Release();
	
	TCHAR buffer[512];
	int totalcount = 0;
	int index = 0;
	int collectionlistlinecount = 0;
	int needitemcount = 0;
	int currentneeditemcount = 0;
	int	packageresourceindex = 0;
	int temp = 0;
	BOOL bResourceTable = TRUE;
	BOOL bResourcePackage = FALSE;
	BOOL bResourceGroup = FALSE;
	_XResourcePackage* newpackage = NULL;
	_XResourceGroupItem* newgroup = NULL;
	

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(TCHAR)*512);
		if(!fgets(buffer, 511, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(bResourceTable)	// 자원 리스트 구성하기
			{		
				if( collectionlistlinecount == 0 )			// 자원 리스트 총 갯수
				{
					if( buffer[0] == _T('#') ) // skip unicode text file header
					{
						sscanf(buffer+1, "%d", &m_TotalCountResourceItem);
					}
					else
					{
						sscanf(buffer, "%d", &m_TotalCountResourceItem);
					}
					m_arrayResourceItem = new _XResourceItem[m_TotalCountResourceItem];
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 1 )		// 자원 리스트 고유 인덱스
				{
					sscanf(buffer, "%d", &m_arrayResourceItem[index].m_ResourceIndex );					
					collectionlistlinecount++;					
				}
				else if( collectionlistlinecount == 2 )		// 자원 수집 이름
				{
					memset( m_arrayResourceItem[index].m_ResourceName, 0, sizeof(TCHAR)*256 );
					strncpy( m_arrayResourceItem[index].m_ResourceName, buffer, sizeof(TCHAR)*256 );
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 3 )		// 수집해야 할 아이템 갯수
				{
					sscanf(buffer, "%d", &m_arrayResourceItem[index].m_totalcountNeeditem );
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 4 )		// 수집해야 할 아이템
				{
					_XCollectionResourceItem* newneeditem = new _XCollectionResourceItem;
					short itemid;	
					int cType = 0;
					unsigned int ucCount = 0;
					sscanf(buffer, "%d %d %d %d", &newneeditem->m_Index, &cType, &itemid, &ucCount);
					newneeditem->Set_m_sID(itemid);
					newneeditem->Set_m_cType(cType);
					newneeditem->Set_m_ucCount(ucCount); 
					newneeditem->Set_m_cSecond( _XGameItem::GetItemSecond( newneeditem->Get_m_cType(), newneeditem->Get_m_sID() ));
					m_arrayResourceItem[index].m_listNeedItem.push_back( newneeditem );
					currentneeditemcount++;
					
					if( currentneeditemcount == m_arrayResourceItem[index].m_totalcountNeeditem ) 
						collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 5 )		// 	보상 아이템
				{
					short itemid;
					int cType = 0;
					unsigned int ucCount = 0;
					sscanf(buffer, "%d %d %d", &cType, &itemid, &ucCount );
					m_arrayResourceItem[index].m_RewardItem.Set_m_sID(itemid);
					m_arrayResourceItem[index].m_RewardItem.Set_m_cType(cType);
					m_arrayResourceItem[index].m_RewardItem.Set_m_ucCount(ucCount); 

					m_arrayResourceItem[index].m_RewardItem.Set_m_cSecond(_XGameItem::GetItemSecond( m_arrayResourceItem[index].m_RewardItem.Get_m_cType(), m_arrayResourceItem[index].m_RewardItem.Get_m_sID() ));
					index++;
					if( index == m_TotalCountResourceItem ) // 보상세트의 갯수가 차면 하나의 수집 리스트 아이템이 끝난다.
					{						
						bResourceTable = FALSE;
						bResourcePackage = TRUE;
						bResourceGroup = FALSE;
						collectionlistlinecount = 0;
						totalcount = 0;
						index = 0;
						needitemcount = 0;
						currentneeditemcount = 0;
					}
					else // 자원 리스트의 총 갯수를 넘지 않으면 다음 자원 아이템의 정보를 받는다.
					{					
						collectionlistlinecount = 1;
						currentneeditemcount = 0;
						needitemcount = 0;
					}
				}
			}
			else if(bResourcePackage)	// 자원 패키지 구성하기
			{
				if( collectionlistlinecount == 0 )			// 자원 패키지 총 갯수
				{
					sscanf(buffer, "%d", &totalcount);
					
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 1 )		// 자원 패키지 고유 인덱스
				{					
					newpackage = new _XResourcePackage;
					sscanf(buffer, "%d", &newpackage->m_ResourcePackageIndex );
					
					collectionlistlinecount++;					
				}
				else if( collectionlistlinecount == 2 )		// 자원 패키지에 들어갈 자원 수집 갯수
				{
					sscanf(buffer, "%d", &needitemcount );

					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 3 )		// 자원 패키지에 들어갈 자원 수집 고유인덱스
				{
					sscanf(buffer, "%d %d", &temp, &packageresourceindex );
					newpackage->m_listResourceItemIndex.push_back(packageresourceindex);

					currentneeditemcount++;
					
					if( currentneeditemcount == needitemcount ) 
					{
						m_listResourcePackage.push_back(newpackage);
						index++;
						if( index == totalcount )
						{
							bResourceTable = FALSE;
							bResourcePackage = FALSE;
							bResourceGroup = TRUE;
							collectionlistlinecount = 0;
							totalcount = 0;
							index = 0;
							needitemcount = 0;
							currentneeditemcount = 0;
							packageresourceindex = 0;
						}
						else
						{
							collectionlistlinecount = 1;
							currentneeditemcount = 0;
							needitemcount = 0;
						}
					}
				}
			}
			else if(bResourceGroup)	// 자원 그룹 구성하기 ( 디스플레이용 ) 
			{
				if( collectionlistlinecount == 0 )			// 자원 패키지 총 갯수
				{
					sscanf(buffer, "%d", &totalcount);
					
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 1 )		// 자원 그룹 이름 
				{					
					newgroup = new _XResourceGroupItem;
					memset( newgroup->m_ResourceGroupSubItemName, 0, sizeof(TCHAR)*256 );
					strncpy( newgroup->m_ResourceGroupSubItemName, buffer, sizeof(TCHAR)*256 );
					collectionlistlinecount++;					
				}
				else if( collectionlistlinecount == 2 )		// 자원 소그룹 고유 인덱스 : 상인 이름
				{
					sscanf(buffer, "%d", &newgroup->m_ResourceGroupSubItemIndex );
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 3 )		// 자원 그룹 고유 인덱스 : 지역
				{
					sscanf(buffer, "%d", &newgroup->m_ResourceGroupItemIndex );
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 4 )		// 자원 소그룹이 가지고 있는 자원패키지 고유 인덱스
				{
					sscanf(buffer, "%d", &newgroup->m_ResourcePackageIndex );
					m_listResourceGroupItem.push_back(newgroup);
					index++;
					if( index == totalcount )
					{
						bResourceTable = FALSE;
						bResourcePackage = FALSE;
						bResourceGroup = FALSE;
						collectionlistlinecount = 0;
						totalcount = 0;
						index = 0;
						needitemcount = 0;
						currentneeditemcount = 0;
						break;
					}
					else
					{
						collectionlistlinecount = 1;
					}
					
				}
			}


		}
	} while(!feof(fileptr));


	return TRUE;
}


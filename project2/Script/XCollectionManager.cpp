// XCollectionManager.cpp: implementation of the _XCollectionManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XCollectionManager.h"
#include "XCollectionItem.h"

extern void ReplaceCharacter(LPSTR _string, TCHAR srcchar, TCHAR changechar);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XCollectionManager::_XCollectionManager()
{

}

_XCollectionManager::~_XCollectionManager()
{
	Release();

}

void _XCollectionManager::Release(void)
{		
	if( !m_listCollectionItem.empty() )
	{
		_XCollectionItem* currentcollection = NULL;
		_XCollectionRewardSet* currentrewardset = NULL;
		_XCollectionResourceItem* currentneeditem = NULL;
		_XCollectionResourceItem* currentrewarditem = NULL;
		
		// 필요 아이템 삭제
		list <_XCollectionItem*>::iterator iter_collection;
		for(iter_collection = m_listCollectionItem.begin() ; iter_collection != m_listCollectionItem.end() ; ++iter_collection)
		{
			currentcollection = *iter_collection;
			if(currentcollection)
			{			
				if( !currentcollection->m_listNeedItem.empty() )
				{
					list <_XCollectionResourceItem*>::iterator iter_needitem;
					for(iter_needitem = currentcollection->m_listNeedItem.begin() ; iter_needitem != currentcollection->m_listNeedItem.end() ; ++iter_needitem)
					{
						currentneeditem = *iter_needitem;
						if(currentneeditem)
						{
#ifdef _XTS_NEWLIFEWINDOW
							SAFE_DELETE_ARRAY(currentcollection->m_CollectionName);
							SAFE_DELETE_ARRAY(currentcollection->m_CollectionExplain);
#endif
							SAFE_DELETE(currentneeditem);
						}
					}
					currentcollection->m_listNeedItem.clear();
				}
				currentcollection = NULL;
			}
		}
		// 끝 - 필요 아이템 삭제
		
		// 보상 아이템 삭제
		for(iter_collection = m_listCollectionItem.begin() ; iter_collection != m_listCollectionItem.end() ; ++iter_collection)
		{
			currentcollection = *iter_collection;
			if(currentcollection)
			{		
				if( !currentcollection->m_listRewardSet.empty() )
				{
					list <_XCollectionRewardSet*>::iterator iter_rewardset;
					for(iter_rewardset = currentcollection->m_listRewardSet.begin() ; iter_rewardset != currentcollection->m_listRewardSet.end() ; ++iter_rewardset)
					{
						currentrewardset = *iter_rewardset;
						if(currentrewardset)
						{	
							if( !currentrewardset->m_listRewardItem.empty() )
							{
								list <_XCollectionResourceItem*>::iterator iter_rewarditem;
								for(iter_rewarditem = currentrewardset->m_listRewardItem.begin() ; iter_rewarditem != currentrewardset->m_listRewardItem.end() ; ++iter_rewarditem)
								{
									currentrewarditem = *iter_rewarditem;
									if(currentrewarditem)
									{										
										SAFE_DELETE(currentrewarditem);
									}
								}
								currentrewardset->m_listRewardItem.clear();
							}
							currentrewardset = NULL;						
						}
					}
				}	
				currentcollection = NULL;
			}
		}
		// 끝 - 보상 아이템 삭제
		
		// 보상 세트 삭제
		for(iter_collection = m_listCollectionItem.begin() ; iter_collection != m_listCollectionItem.end() ; ++iter_collection)
		{
			currentcollection = *iter_collection;
			if(currentcollection)
			{		
				if( !currentcollection->m_listRewardSet.empty() )
				{
					list <_XCollectionRewardSet*>::iterator iter_rewardset;
					for(iter_rewardset = currentcollection->m_listRewardSet.begin() ; iter_rewardset != currentcollection->m_listRewardSet.end() ; ++iter_rewardset)
					{
						currentrewardset = *iter_rewardset;
						if(currentrewardset)
						{	
							SAFE_DELETE(currentrewardset);
							currentrewardset = NULL;						
						}
					}
					currentcollection->m_listRewardSet.clear();
				}	
				currentcollection = NULL;
			}
		}
		// 끝 - 보상 세트 삭제

		// 보상 리스트 삭제
		for(iter_collection = m_listCollectionItem.begin() ; iter_collection != m_listCollectionItem.end() ; ++iter_collection)
		{
			currentcollection = *iter_collection;
			if(currentcollection)
			{		
				SAFE_DELETE(currentcollection);
				currentcollection = NULL;
			}
		}
		m_listCollectionItem.clear();
		// 끝 - 보상 리스트 삭제 	
	}

#ifdef _XTS_COLLECTION_PACKAGE_071023_KUKURI
	_map_CollectionPackage::iterator pack_iter;
	for(pack_iter = m_mapCollectionPackage.begin() ; pack_iter != m_mapCollectionPackage.end() ; ++pack_iter)
	{
		_s_Collection_Package* pPackage = pack_iter->second;
		SAFE_DELETE(pPackage);
	}
	m_mapCollectionPackage.clear();
#endif
}

BOOL _XCollectionManager::LoadCollectionItem(LPCTSTR filename)
{
	if(!filename || filename[0] == NULL)
		return FALSE;
	
	FILE* fileptr = NULL;
	fileptr = fopen(filename, "rb");
	if(!fileptr)
	{
		_XFatalError(_T("Collection script file open"));
		return FALSE;
	}
	
	if(!LoadCollectionItem(fileptr))
	{
		_XFatalError("Collection script loading [%s]", filename);
		fclose(fileptr);
		return FALSE;
	}
	
	fclose(fileptr);
	return TRUE;
}

BOOL _XCollectionManager::LoadCollectionItem(FILE* fileptr)
{
	if(!fileptr)
		return FALSE;
	
	Release();
	
	TCHAR buffer[512];
	int totalcount = 0;
	int index = 0;
	int collectionlistlinecount = 0;
//	int explainlinecount = 0;
	int	currentexplainlinecount = 0;
	int currentneeditemcount = 0;
	int rewardsetcount = 0;
	int currentrewardsetcount = 0;
	int rewarditemcount = 0;
	int currentrewarditemcount = 0;
	BOOL bReadCount = FALSE;
	BOOL bNewitem = FALSE;
	_XCollectionItem* newcollection = NULL;
	_XCollectionRewardSet* newrewardset = NULL;

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 511, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			if(!bReadCount)
			{
				sscanf(buffer, "%d", &totalcount);
				totalcount++;
				bReadCount = TRUE;
				collectionlistlinecount++;
			}
			else
			{
				if(!bNewitem)
				{
					newcollection = new _XCollectionItem;
					bNewitem = TRUE;
//					explainlinecount = 0;
					currentexplainlinecount = 0;
					currentneeditemcount = 0;
					rewardsetcount = 0;
					currentrewardsetcount = 0;
					rewarditemcount = 0;
					currentrewarditemcount = 0;
				}

				if( collectionlistlinecount == 1 )			// 수집품 고유 인덱스
				{
					sscanf(buffer, "%d", &newcollection->m_CollectionIndex );
					index++;
					collectionlistlinecount++;					
				}
				else if( collectionlistlinecount == 2 )		// 선행으로 완료해야 할 명품 수집품 고유 인덱스 5개
				{
					//sscanf(buffer, "%d", &newcollection->m_CollectionVersion );
					sscanf(buffer, "%d %d %d %d %d", &newcollection->m_PrecedeCollectionIndex[0], &newcollection->m_PrecedeCollectionIndex[1], 
						&newcollection->m_PrecedeCollectionIndex[2], &newcollection->m_PrecedeCollectionIndex[3], &newcollection->m_PrecedeCollectionIndex[4]);
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 3 )		// 수집품 타입 (하품, 중품, 상품, 명품 등을 구분한다.)
				{
					sscanf(buffer, "%d", &newcollection->m_CollectionType );
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 4 )		// 수집품 가능 최소레벨
				{
					sscanf(buffer, "%d", &newcollection->m_CollectionMinimumLevel );
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 5 )		// 수집품 이름 (가죽수집, 약초수집)
				{
#ifdef _XTS_NEWLIFEWINDOW
					int strlength = strlen(buffer);
					newcollection->m_CollectionName = new TCHAR[strlength+1];
					newcollection->m_CollectionName[strlength] = 0;
					strncpy(newcollection->m_CollectionName, buffer, strlength);
#else
					memset( newcollection->m_CollectionName, 0, sizeof(TCHAR)*128 );
					strncpy( newcollection->m_CollectionName, buffer, sizeof(TCHAR)*128 );
#endif
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 6 )		// 수집품 보상 내용 라인 수 (최대라인수 3줄)
				{
#ifdef _XTS_NEWLIFEWINDOW
#else
					memset( newcollection->m_CollectionExplain, 0, sizeof(TCHAR)*_XDEF_MAX_COLLECTION_LINE*512 );
#endif
					sscanf(buffer, "%d", &newcollection->m_CollectionExplainLineCount);
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 7 )		// 수집품 보상 내용
				{
#ifdef _XTS_NEWLIFEWINDOW
                    newcollection->m_TempString[currentexplainlinecount] = new TCHAR[512];
					memset(newcollection->m_TempString[currentexplainlinecount], 0, sizeof(TCHAR)*512);
					//strncpy( newcollection->m_TempString[currentexplainlinecount], buffer, sizeof(TCHAR)*512 );
					strcpy( newcollection->m_TempString[currentexplainlinecount], buffer );

					//OutputDebugString( newcollection->m_TempString[currentexplainlinecount] );					
#else
					strncpy( newcollection->m_CollectionExplain[currentexplainlinecount], buffer, sizeof(TCHAR)*512 );
#endif
					currentexplainlinecount++;
					
					if( currentexplainlinecount == newcollection->m_CollectionExplainLineCount ) 
						collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 8 )		// 수집해야 할 아이템 갯수
				{
#ifdef _XTS_NEWLIFEWINDOW
                    
                        int strlength = 0;
                        for(int i = 0 ; i < _XDEF_MAX_COLLECTION_LINE ; i++)
					    {
						    if(newcollection->m_TempString[i])
						    {
                                ReplaceCharacter(newcollection->m_TempString[i], _T('\r'), _T(' '));
                                ReplaceCharacter(newcollection->m_TempString[i], _T('\n'), _T(' '));
                                ReplaceCharacter(newcollection->m_TempString[i], _T('\t'), _T(' '));

                                strlength += strlen(newcollection->m_TempString[i]);
						    }
					    }

					    newcollection->m_CollectionExplain = new TCHAR[strlength+1];
					    memset(newcollection->m_CollectionExplain, 0, sizeof(TCHAR)*(strlength+1));
					    for(i = 0 ; i < _XDEF_MAX_COLLECTION_LINE ; i++)
					    {
						    if(newcollection->m_TempString[i])
						    {
							    strcat(newcollection->m_CollectionExplain, newcollection->m_TempString[i]);
						    }
					    }

    //					_XDWINPRINT("%s", newcollection->m_CollectionExplain);

					    for(i = 0 ; i < _XDEF_MAX_COLLECTION_LINE ; i++)
					    {
						    SAFE_DELETE_ARRAY(newcollection->m_TempString[i]);
					    }
                    
#endif
					sscanf(buffer, "%d", &newcollection->m_NeedItemTotalCount );
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 9 )		// 수집해야 할 아이템
				{
					_XCollectionResourceItem* newneeditem = new _XCollectionResourceItem;
					short itemid;
					int cType = 0;
					unsigned int ucCount = 0;
					sscanf(buffer, "%d %d %d %d", &newneeditem->m_Index, &cType, &itemid, &ucCount);					
					newneeditem->Set_m_sID(itemid);
					newneeditem->Set_m_cType(cType);
					newneeditem->Set_m_ucCount(ucCount);

					newneeditem->Set_m_cSecond( _XGameItem::GetItemSecond( newneeditem->Get_m_cType(), newneeditem->Get_m_sID()) );
					newcollection->m_listNeedItem.push_back( newneeditem );
					currentneeditemcount++;
					
					if( currentneeditemcount == newcollection->m_NeedItemTotalCount ) 
						collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 10 )		// 보상 세트 갯수
				{
					sscanf(buffer, "%d", &rewardsetcount);
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 11 )		// 보상 세트 인덱스
				{
					newrewardset = new _XCollectionRewardSet;
					sscanf(buffer, "%d", &newrewardset->m_Index);		
					
					collectionlistlinecount++;
				}
				else if( collectionlistlinecount == 12 )		// 보상 아이템 갯수
				{
					sscanf(buffer, "%d", &rewarditemcount);		
					
					collectionlistlinecount = 13;
				}
				else if( collectionlistlinecount == 13 )		// 보상 아이템
				{
					_XCollectionResourceItem* newrewarditem = new _XCollectionResourceItem;

					short itemid;
					int cType = 0;
					unsigned int ucCount = 0;
					sscanf(buffer, "%d %d %d %d", &newrewarditem->m_Index, &cType,  &itemid, &ucCount );
					newrewarditem->Set_m_sID( itemid );
					newrewarditem->Set_m_cType(cType);
					newrewarditem->Set_m_ucCount(ucCount); 

					newrewarditem->Set_m_cSecond( _XGameItem::GetItemSecond( newrewarditem->Get_m_cType(), newrewarditem->Get_m_sID()) );
					newrewardset->m_listRewardItem.push_back(newrewarditem);
					currentrewarditemcount++;
					
					if( currentrewarditemcount == rewarditemcount ) // 보상이이템의 갯수가 차면 하나의 보상 세트가 완성된다.
					{
						currentrewarditemcount = 0;
						rewarditemcount = 0;

						currentrewardsetcount++;
						if( currentrewardsetcount == rewardsetcount ) // 보상세트의 갯수가 차면 하나의 수집 리스트 아이템이 끝난다.
						{
							newcollection->m_listRewardSet.push_back(newrewardset);
							m_listCollectionItem.push_back( newcollection );
							collectionlistlinecount = 1;
							bNewitem = FALSE;
						}
						else // 보상 세트의 갯수가 남았으면 다시 보상 아이템의 갯수를 읽어야 한다.
						{
							newcollection->m_listRewardSet.push_back(newrewardset);
							collectionlistlinecount = 11;
						}
					}
				}
			}			

			if(index == totalcount)
			{
				break;
			}

		}
	} while(!feof(fileptr));

#ifdef _XTS_COLLECTION_PACKAGE_071023_KUKURI

	TCHAR tempstr[256];
	memset(tempstr, 0, sizeof(tempstr));
	int param1 = 0;
	int packagecount = 0;
	_s_Collection_Package* pPackage = NULL;

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 511, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			sscanf(buffer, "%s %d", tempstr, &param1);

			if(strncmp(tempstr, "_XNPC_INDEX", strlen("_XNPC_INDEX")) == 0)
			{
				pPackage = new _s_Collection_Package;
				pPackage->nNPCId = param1;
				packagecount = 0;
			}
			else if(strncmp(tempstr, "_XPACKAGE_COUNT", strlen("XPACKAGE_COUNT")) == 0)
			{
				if(pPackage)
				{
					pPackage->nCount = param1;
				}
			}
			else
			{
				sscanf(buffer, "%d", &param1);
				packagecount++;
				// package id
				if(pPackage)
				{
					pPackage->vCollectionId.push_back(param1);
				}
			}

			if(pPackage)
			{
				if(packagecount == pPackage->nCount && packagecount != 0)
				{
					m_mapCollectionPackage[pPackage->nNPCId] = pPackage;
				}
			}
		}
	}while(!feof(fileptr));

#endif

	return TRUE;
}

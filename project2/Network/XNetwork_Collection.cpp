// XNetwork_Collection.cpp : 수집 관련
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "Game_packets.h"
#include "XNetworkDefine.h"
#include "XNetwork.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"

#include "XWindow_NPCCollection.h"
#include "XWindow_NPCResource.h"
#include "XWindow_NPCResource_New.h"


BOOL _XNetwork::Send_MSGCollection(int type, int param1, int param2, int param3, int param4)
{
	switch(type)
	{
	case MSG_NO_COLLECT_REQ : // 명품 혹은 수집품 교환요청
		{
#ifdef _XDEF_NEW_COLLECTION_PACKET_080211_KUKURI

			VIEWPACKETLOG
				_XDWINPRINT("SEND MESSAGE : Collection Request Mode[%d]", param1);
	
			switch(param1)
			{
			case en_collect_mode_complete_resource :
				{
					int size = sizeof(MSG_COLLECT_COMPLETE_RESOURCE);
					MSG_COLLECT_COMPLETE_RESOURCE resourcereq;
					ZeroMemory(&resourcereq, size);
					resourcereq.header.sLength = size;
					resourcereq.ucMessage = MSG_NO_COLLECT_REQ;

					resourcereq.ucMode = (unsigned char)param1;
					resourcereq.usIndex = (unsigned short)param2;

					int itemslotindex[MAX_MATERIAL_RESOURCE_ITEM];
					memset(itemslotindex, 0, sizeof(itemslotindex));

					_XWindow_NPCResource_New* pNPCResource_New_Window = (_XWindow_NPCResource_New*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NEW_RESOURCEWINDOW);
					if(pNPCResource_New_Window)
					{
						pNPCResource_New_Window->GetItemSlotIndex(itemslotindex);
					}

					for(int i = 0 ; i < MAX_MATERIAL_RESOURCE_ITEM ; ++i)
					{
						resourcereq.item_slot_index[i] = itemslotindex[i];
					}

					return _XSendPacket_Encapsulate((char*)&resourcereq);
					
				}
				break;
			case en_collect_mode_save_collection : 
				{
					int size = sizeof(MSG_COLLECT_SAVE_COLLECTION);
					MSG_COLLECT_SAVE_COLLECTION collectionreq;
					ZeroMemory(&collectionreq, size);
					collectionreq.header.sLength = size;
					collectionreq.ucMessage = MSG_NO_COLLECT_REQ;

					collectionreq.ucMode = (unsigned char)param1;
					
					if(g_pLifeSkill_Window)
					{
						if(g_pInventory_Window)
						{
							collectionreq.ucInvenSlotIndex = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
							collectionreq.usIndex = g_pLifeSkill_Window->m_pSelectedCollectionItem->m_CollectionIndex;
							collectionreq.ucCollectionSlotIndex = g_pLifeSkill_Window->m_indexCompleteNeedItem;
						}
					}
					
					return _XSendPacket_Encapsulate( (char*)&collectionreq );
				}
				break;
			case en_collect_mode_complete_collection :
				{
					int size = sizeof(MSG_COLLECT_COMPLETE_COLLECTION);
					MSG_COLLECT_COMPLETE_COLLECTION collectionreq;
					ZeroMemory(&collectionreq, size);
					collectionreq.header.sLength = size;
					collectionreq.ucMessage = MSG_NO_COLLECT_REQ;

					collectionreq.ucMode = (unsigned char)param1;
					collectionreq.usIndex = (unsigned short)param2;
					collectionreq.ucSlotIndex = (unsigned char)param3;

					return _XSendPacket_Encapsulate( (char*)&collectionreq );
				}
				break;
			}
			
#else
//			//현재 패킷의 타입
//			u_char				ucPacketType;	//0: 자원 보상, 1: 명품 save, 2: 명품 보상
//			
//			u_short				usIndex;		//자원 혹은 명품 인덱스
//			
//			u_char				ucSlotIndex;	//자원 혹은 명품의 슬롯 인덱스(명품 save시에만 사용)
			
			VIEWPACKETLOG
				_XDWPACKETLOG( "SEND MESSAGE : Collection Request Socket Send ---- " );
			
			int size = sizeof(MSG_COLLECT_REQ);
			MSG_COLLECT_REQ collectionreq;
			ZeroMemory(&collectionreq, size);
			collectionreq.header.sLength = size;
			collectionreq.ucMessage = MSG_NO_COLLECT_REQ;	
			
			collectionreq.ucPacketType =  (unsigned char)param1;
			collectionreq.usIndex = (unsigned short)param2;
			collectionreq.ucSlotIndex =  (unsigned char)param3;

			
			return _XSendPacket_Encapsulate( (char*)&collectionreq );
#endif
		}
		break;
		
	}
	
	return TRUE;
}

void _XNetwork::MSGCollectionResult(MSG_COLLECT_RESULT* collection)
{
	_XWindow_NPCCollection* pNPCCollection_Window = (_XWindow_NPCCollection*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCCOLLECTION);
	_XWindow_NPCResource_New* pNPCResource_New_Window = (_XWindow_NPCResource_New*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NEW_RESOURCEWINDOW);
	if(!pNPCCollection_Window) return;
	if(!pNPCResource_New_Window) return;

	switch(collection->ucReqMode)
	{
	case en_collect_mode_complete_resource :
		{
			// 자원 수집

			//Author : 양희왕 //breif: 제련질의 추가
			pNPCResource_New_Window->SetStrength(false);

			if(collection->ucFail == 1)// 성공 
			{
				pNPCResource_New_Window->ChangeTab(_TAB_LIST);
				pNPCResource_New_Window->SetProgressExchange(FALSE);
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1623), _XDEF_CHATMESSAGECOLOR_SYSTEMGET, TRUE );

				
			}
			else// 실패
			{
				pNPCResource_New_Window->EnableExchangeButton(TRUE);
				pNPCResource_New_Window->SetProgressExchange(FALSE);
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}

		}
		break;
	case en_collect_mode_save_collection :
		{
			// 명품 저장
			if( collection->ucFail == 1 )// 성공 
			{
				if( !g_pLifeSkill_Window ) return;
				// 저장된 필요 아이템을 체크 한다. 
				g_pLifeSkill_Window->CheckCollectionCompleteNeedItem();
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1625), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
				
			}
			else// 실패
			{
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1626), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}

			if(g_pInventory_Window)
			{
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
			}
		}
		break;
	case en_collect_mode_complete_collection :
		{
			// 명품 수집
			if( collection->ucFail == 1 )// 성공 
			{
				pNPCCollection_Window->m_pSelectedCollectionItem->m_bComplete = true;
				pNPCCollection_Window->SetListItem();
				pNPCCollection_Window->SetProcessReward(false); 
				pNPCCollection_Window->m_pSelectedCollectionItem = NULL;
				for( int i = 0; i< 4; ++i)
				{
					pNPCCollection_Window->m_btnRewardSet[i]->ShowWindow(FALSE);
				}
				
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1623), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}
			else// 실패
			{
				pNPCCollection_Window->SetProcessReward(false); 				
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}
		}
		break;
	}
}

void _XNetwork::MSGCollectionResponse(MSG_COLLECT_RES* collectionresponse)
{
	_XWindow_NPCCollection* pNPCCollection_Window = (_XWindow_NPCCollection*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCCOLLECTION);
	_XWindow_NPCResource* pNPCResource_Window = (_XWindow_NPCResource*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCRESOURCE);
	if( !pNPCCollection_Window ) return;
	if( !pNPCResource_Window ) return;

	switch(collectionresponse->ucPacketType) {
	case 0:// 자원 보상
		{
			//Author : 양희왕 //breif: 제련질의 추가
			pNPCResource_Window->SetStrength(false);

			if( collectionresponse->ucFlag == 1 )// 성공 
			{
				pNPCResource_Window->SetSelectedResourceItem(pNPCResource_Window->m_indexSelectedResourceItem);
				pNPCResource_Window->m_btnExchange->EnableWindow(TRUE); 				
				pNPCResource_Window->SetProgressExchange(false);
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1623), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
				
			}
			else// 실패
			{
				pNPCResource_Window->m_btnExchange->EnableWindow(TRUE); 				
				pNPCResource_Window->SetProgressExchange(false);
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}

		}
		break;
	case 1:// 명품 save
		{
			if( collectionresponse->ucFlag == 1 )// 성공 
			{
				if( !g_pLifeSkill_Window ) return;
				// 저장된 필요 아이템을 체크 한다. 
				g_pLifeSkill_Window->CheckCollectionCompleteNeedItem();
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1625), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
				
			}
			else// 실패
			{
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1626), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}
		}
		break;
	case 2:// 명품 보상
		{
			if( collectionresponse->ucFlag == 1 )// 성공 
			{
				pNPCCollection_Window->m_pSelectedCollectionItem->m_bComplete = true;
				pNPCCollection_Window->SetListItem();
				pNPCCollection_Window->SetProcessReward(false); 
				pNPCCollection_Window->m_pSelectedCollectionItem = NULL;
				for( int i = 0; i< 4; ++i)
				{
					pNPCCollection_Window->m_btnRewardSet[i]->ShowWindow(FALSE);
				}
				
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1623), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}
			else// 실패
			{
				pNPCCollection_Window->SetProcessReward(false); 				
				InsertChatString( _XGETINTERFACETEXT(ID_STRING_NETWORKCOLLECTION_1624), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS, TRUE );
			}
		}
		break;
	}
}

#ifdef _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI
void _XNetwork::ParsingCollectionData(_CHARAC_COLLECTION_DATA collectiondata, _CHARAC_COLLECTION_DATA eventcollectiondata)
#else
void _XNetwork::ParsingCollectionData(_CHARAC_COLLECTION_DATA collectiondata)
#endif
{		
	_XCollectionItem* currentcollection = NULL;
	_XCollectionResourceItem* currentneeditem = NULL;
	
	if( !g_CollectionManager.m_listCollectionItem.empty() )
	{
		list <_XCollectionItem*>::iterator iter_collection;
		for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; ++iter_collection)
		{
			currentcollection = *iter_collection;
			if(currentcollection)
			{			
				currentcollection->m_CompleteItemCurrentCount = 0;
				currentcollection->m_bComplete = false;
				currentcollection->m_bEnableExchage = false;
				if( !currentcollection->m_listNeedItem.empty() )
				{
					list <_XCollectionResourceItem*>::iterator iter_needitem;
					for(iter_needitem = currentcollection->m_listNeedItem.begin() ; iter_needitem != currentcollection->m_listNeedItem.end() ; ++iter_needitem)
					{
						currentneeditem = *iter_needitem;
						if(currentneeditem)
						{			
							currentneeditem->m_bComplete = false;
						}
					}
				
				}
			}
		}

		for(int i = 0; i < MAX_COLLECTION_INDEX_COUNT; ++i)
		{
			if( collectiondata.m_aData[i].GetData() != 0 )
			{
				// 필요 아이템 삭제
				list <_XCollectionItem*>::iterator iter_collection;
				for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; ++iter_collection)
				{
					currentcollection = *iter_collection;
					if(currentcollection)
					{			
						if( currentcollection->m_CollectionIndex == i)
						{
							if( !collectiondata.m_aData[i].IsThisCompleted())
							{
								if( !currentcollection->m_listNeedItem.empty() )
								{
									currentcollection->m_CompleteItemCurrentCount = 0;
									for( int needitemindex = 0; needitemindex < COLLECTIONDATA_COMPLETEFLAG_INDEX; ++needitemindex )
									{
										if( collectiondata.m_aData[i].IsThisTrue(needitemindex) )
										{
											list <_XCollectionResourceItem*>::iterator iter_needitem;
											for(iter_needitem = currentcollection->m_listNeedItem.begin() ; iter_needitem != currentcollection->m_listNeedItem.end() ; ++iter_needitem)
											{
												currentneeditem = *iter_needitem;
												if(currentneeditem)
												{			
													if( currentneeditem->m_Index == needitemindex)
													{
														currentneeditem->m_bComplete = true;
														currentcollection->m_CompleteItemCurrentCount++;
													}
												}
											}
										}
									}
									if( currentcollection->m_CompleteItemCurrentCount == currentcollection->m_NeedItemTotalCount )
									{ 
										currentcollection->m_bEnableExchage = true;
									}
								}
							}
							else // 교환 완료
							{
								currentcollection->m_bComplete = true;
								list <_XCollectionResourceItem*>::iterator iter_needitem;
								for(iter_needitem = currentcollection->m_listNeedItem.begin() ; iter_needitem != currentcollection->m_listNeedItem.end() ; ++iter_needitem)
								{
									currentneeditem = *iter_needitem;
									if(currentneeditem)
									{			
										currentneeditem->m_bComplete = true;
									}
								}
							}
						}
						
					}
				}
			}
		}// for

#ifdef _XDEF_INCREASE_COLLECTION_INDEX_080328_KUKURI
		for(i = 0; i < MAX_COLLECTION_INDEX_COUNT; ++i)
		{
			if( eventcollectiondata.m_aData[i].GetData() != 0 )
			{
				// 필요 아이템 삭제
				list <_XCollectionItem*>::iterator iter_collection;
				for(iter_collection = g_CollectionManager.m_listCollectionItem.begin() ; iter_collection != g_CollectionManager.m_listCollectionItem.end() ; ++iter_collection)
				{
					currentcollection = *iter_collection;
					if(currentcollection)
					{			
						if( currentcollection->m_CollectionIndex == i+1000)	// index:1000~1511
						{
							if( !eventcollectiondata.m_aData[i].IsThisCompleted())
							{
								if( !currentcollection->m_listNeedItem.empty() )
								{
									currentcollection->m_CompleteItemCurrentCount = 0;
									for( int needitemindex = 0; needitemindex < COLLECTIONDATA_COMPLETEFLAG_INDEX; ++needitemindex )
									{
										if( eventcollectiondata.m_aData[i].IsThisTrue(needitemindex) )
										{
											list <_XCollectionResourceItem*>::iterator iter_needitem;
											for(iter_needitem = currentcollection->m_listNeedItem.begin() ; iter_needitem != currentcollection->m_listNeedItem.end() ; ++iter_needitem)
											{
												currentneeditem = *iter_needitem;
												if(currentneeditem)
												{			
													if( currentneeditem->m_Index == needitemindex)
													{
														currentneeditem->m_bComplete = true;
														currentcollection->m_CompleteItemCurrentCount++;
													}
												}
											}
										}
									}
									if( currentcollection->m_CompleteItemCurrentCount == currentcollection->m_NeedItemTotalCount )
									{ 
										currentcollection->m_bEnableExchage = true;
									}
								}
							}
							else // 교환 완료
							{
								currentcollection->m_bComplete = true;
								list <_XCollectionResourceItem*>::iterator iter_needitem;
								for(iter_needitem = currentcollection->m_listNeedItem.begin() ; iter_needitem != currentcollection->m_listNeedItem.end() ; ++iter_needitem)
								{
									currentneeditem = *iter_needitem;
									if(currentneeditem)
									{			
										currentneeditem->m_bComplete = true;
									}
								}
							}
						}
						
					}
				}
			}
		}// for
#endif
	}
}
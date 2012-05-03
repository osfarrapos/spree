// XNPC.cpp: implementation of the XNPC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMeshContainer.h"
#include "XNPC.h"
#include "XNPCScript.h"
#include "XUser.h"
#include "XWindow_NPCScript.h"
#include "XProc_ProcessDefine.h"
#include "XNPCScriptConditionItem.h"
#include "XSR_STRINGHEADER.H"

#define _XDEF_NPCSOUNDLIST		"NPCSoundList.ini"

_XNPCObject::_XNPCObject()
{
	m_pNPCScriptWindowPtr		= NULL;
	m_InsideFrustum				= TRUE;
	m_Distance					= 1000.0f;
	m_UniqueID					=	-1;
	m_NPCJobClass				=	_XNPC_JOBCLASS_FOOD;
	m_TradeItemClass			=	-1;
	m_NPCName					= NULL;
	m_NPCRoll					= NULL;

	m_NPCClass					=	_XNPC_CLASS_COMMON;
	m_NPCClanClass				=	_XGROUP_NANGIN;

	m_SelectedMessageIndex		= -1;
	m_SelectedSubScriptIndex	= -1;
	m_SelectedMainScriptIndex	= 0;
	m_PrevMainScriptIndex		= -1;

	m_MainScriptIndex			= 0;

	m_CurrentMotion				= _XNPCMOTION_IDLE;
	
	m_MessageType				= _XDEF_NPCMESSAGE_NORMAL;
	m_MessageString				= NULL;
	m_ChangeMessage				= FALSE;
	m_ChangeButton				= FALSE;
	m_ChangeNormalMessage		= FALSE;
	
	m_SpecialActionTimer		= 0;
	
	m_NPCRollType				= _XNPCROLL_NONE;

	m_bCheckClanNPC				= FALSE;		
	m_bCheckFriendship			= FALSE;		
	m_bCheckHostility			= FALSE;

	m_HaveQuestType				= _XNPC_HAVEQUEST_NONE;
	m_bNPCDialogMode			= FALSE;
	
	m_bDrawNPCChat				= FALSE;
	m_NPCChatBallonTimer		= 0;		// NPC 혼자말 유지 시간을 위한 변수 
	
	m_bStartQuestDialog			= FALSE;	// NPC가 퀘스트에 대한 대사를 가지고 있음.
	
	m_bShowNPCModel				= TRUE;		// NPC를 Rendering 할지 안 할지를 결정 
	m_EventType					= -1;		// 이벤트 종류 0:무영검-패밀리마트 1:봉인유저 2:퀴즈맞추기
	
	m_ModelDescriptor.ReserveModelStack( _XDEF_MAXMODEL_COMBINATIONCOUNT );
	m_ModelDescriptor.m_SwordEffectInitialized = FALSE;
	
}

_XNPCObject::~_XNPCObject()
{
	m_Triggerlist.disposeList();

#ifndef _XDEF_USENEWNPCTEXTSYSTEM
	SAFE_DELETE_ARRAY( m_NPCName );
	SAFE_DELETE_ARRAY(m_NPCRoll);
#endif
	
	if(m_MessageString)
	{
		delete m_MessageString;
		m_MessageString = NULL;
	}
}

void _XNPCObject::CreateEffectManager( void )
{
	int swordeffecttracecount = 0;
	if( m_ModelDescriptor.m_pBipedController->m_pSwordTraceInfoData )
	{
		swordeffecttracecount = m_ModelDescriptor.m_pBipedController->m_pSwordTraceInfoData->m_Pointcount;
	}
	
	m_ModelDescriptor.InitEffectManager(swordeffecttracecount);	
}

BOOL _XNPCObject::LoadScript( LPSTR Filename )
{
	if( !Filename ) return FALSE;

	FILE* FilePtr = NULL;

	FilePtr = fopen( Filename, "rb" );
	if( FilePtr )
	{
		LoadScript( FilePtr );
		fclose( FilePtr );
	}
	else
	{
		_XFatalError( "_XNPCObject::LoadScript(%s) : File open", Filename );
	}

	return TRUE;
}

BOOL _XNPCObject::LoadScript( FILE* FilePtr )
{
	if( !FilePtr ) return FALSE;	
	
	int count = this->m_Triggerlist.getitemcount();
	
	if( fread( &count, sizeof(int), 1, FilePtr ) < 1 )
	{
		_XFatalError( "_XNPCObject::SaveScript : save script count" );
		return FALSE;
	}
	
	this->m_Triggerlist.disposeList();
	
	for( int i = 0; i < count; ++i )
	{
		_XNPCMainScript* mainscript = new _XNPCMainScript;
		
		if( mainscript )
		{
			mainscript->m_UniqueID = this->m_UniqueID;

			if( !mainscript->LoadScript( FilePtr ) )
			{
				return FALSE;
			}

			// 미니스톱이벤트 관련 변수 
			m_EventType = mainscript->m_EventType;
			
			switch(m_NPCJobClass) {
			case _XNPC_JOBCLASS_FOOD:
			case _XNPC_JOBCLASS_CLOTHES:
			case _XNPC_JOBCLASS_REFINING:
			case _XNPC_JOBCLASS_HOTEL:
			case _XNPC_JOBCLASS_DRUGSTORE:
			case _XNPC_JOBCLASS_SHOP:
			case _XNPC_JOBCLASS_WEAPON:
			case _XNPC_JOBCLASS_MILITARYOFFICER:
			case _XNPC_JOBCLASS_HEANGSANGIN:
			case _XNPC_JOBCLASS_BUFF:
			case _XNPC_JOBCLASS_RESOURCE:
			case _XNPC_JOBCLASS_COLLECTION:
			case _XNPC_JOBCLASS_REPAIR:
			case _XNPC_JOBCLASS_CASTLECAPTAIN:
			case _XNPC_JOBCLASS_CASTLEATTACT :
			case _XNPC_JOBCLASS_TRACER :
				
				{
					m_NPCRollType = _XNPCROLL_TRADE;
				}
				break;
			case _XNPC_JOBCLASS_CASTLEDEFFENCE :
			case _XNPC_JOBCLASS_WAREHOUSE:
				{
					m_NPCRollType = _XNPCROLL_WAREHOUSE;
				}
				break;
			case _XNPC_JOBCLASS_GUARD:	// 경비대장 옮김 (trade->event)
			case _XNPC_JOBCLASS_COMMONPEOPLE:
			case _XNPC_JOBCLASS_GAEBANGPEOPLE:
			case _XNPC_JOBCLASS_SORIMPEOPLE:
			case _XNPC_JOBCLASS_NOCKRIMPEOPLE:
			case _XNPC_JOBCLASS_BEEGOONGPEOPLE:
			case _XNPC_JOBCLASS_MOODANGPEOPLE:
			case _XNPC_JOBCLASS_MAKYOPEOPLE:
				{
					m_NPCRollType = _XNPCROLL_EVENT;
				}
				break;
			case _XNPC_JOBCLASS_BLACKMARKET:
				{
					m_NPCRollType = _XNPCROLL_BLACKMARKET;
				}
				break;
			}					
			
			this->m_Triggerlist.insertItem( mainscript );
		}
		else
		{
			return FALSE;
		}		
	}
	
	return TRUE;
}

BOOL _XNPCObject::CheckIntersectRay( void )
{
	D3DXVECTOR3 pos;	
	pos.x = m_ModelDescriptor.m_Position._41;
	pos.y = m_ModelDescriptor.m_Position._42;
	pos.z = m_ModelDescriptor.m_Position._43;
	FLOAT RotateAngle = 0.0f;
	return m_ModelDescriptor.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, pos, RotateAngle );
}

void _XNPCSpeech( LPSTR szFilename, D3DXVECTOR3 position )
{
#ifdef _XUSEFMOD
	int soundindex = g_FMODWrapper.FL_InsertSound( szFilename, FSOUND_HW3D );
	g_FMODWrapper.FL_PlaySound3D( soundindex, position, FALSE );
#else
	Sound3DInit init3dsound;
	init3dsound.m_bLooping = false;
	init3dsound.m_bStreaming = false;
	init3dsound.m_bMusic = false;	
	init3dsound.m_pPackage = &g_NPCSoundArchive;
	init3dsound.m_sFileName = szFilename;
	
	if( g_NPCSoundObject )
	{
		g_NPCSoundObject->Destroy();
		g_NPCSoundObject = NULL;
	}						
	
	if(!AudioMgr()->CreateSound3D(g_NPCSoundObject)) return;
	if(!g_NPCSoundObject->Init(init3dsound))
	{
		_XFatalError("Error initializing npc audio file");
		return;
	}
		
	g_NPCSoundObject->SetPosition( position );
	g_NPCSoundObject->SetVolume( 1.0f );
	g_NPCSoundObject->Play();
#endif
}

void _XNPCObject::Process(void)
{
	if(m_pNPCScriptWindowPtr)
	{
		_XNPCMainScript* npcmainscriptitem = (_XNPCMainScript *)m_Triggerlist.getItem(m_SelectedMainScriptIndex);
		_XNPCMainScript* scriptitem = (_XNPCMainScript *)m_Triggerlist.getItem(m_MainScriptIndex);
		if(npcmainscriptitem && scriptitem ) // 2004.06.10->oneway48 insert
		{
			if(!g_ScriptStarted || m_ChangeNormalMessage)
			{
				m_pNPCScriptWindowPtr->ShowWindow(TRUE);

				_XSetDefaultPath();			

				if( !m_bStartQuestDialog )
				{
					if(m_MessageType == _XDEF_NPCMESSAGE_NORMAL)
					{
						m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_TALKING));
						if(npcmainscriptitem->m_NPCScriptConditionList.listEmpty())
						{
							//2004.07.05->oneway48 inert
							int count = 0;
							for(int i = 0 ; i < 5; ++i)
							{															
								if( npcmainscriptitem->strGreetingGeneralMessage[i] != NULL )
									count++;
							}						
							if( count != 0 ) 
							{
								m_SelectedMessageIndex = _XRandom() % count;
								m_SelectedMessageIndex++;
								if(m_SelectedMessageIndex >= count)
									m_SelectedMessageIndex = 0;	
							}
							else
								m_SelectedMessageIndex = 0;

							//조건 리스트가 없을 때 
							m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
							m_pNPCScriptWindowPtr->SetMessage(npcmainscriptitem->strGreetingGeneralMessage[m_SelectedMessageIndex]);
							SetMessageString(npcmainscriptitem->strGreetingGeneralMessage[m_SelectedMessageIndex]);

							if(g_SoundEffectFlag)
							{
								if(npcmainscriptitem->indexGreetingGeneralSound[m_SelectedMainScriptIndex] > 0)
								{										
									_XNPCSpeech( g_NPCSoundLUT[npcmainscriptitem->indexGreetingGeneralSound[m_SelectedMainScriptIndex]], 
										D3DXVECTOR3( m_ModelDescriptor.m_Position._41,
										m_ModelDescriptor.m_Position._42,
										m_ModelDescriptor.m_Position._43 ) );
									m_SpecialActionTimer = g_LocalSystemTime;
								}
							}

							if( g_NPCMotionExistTable[m_ObjectIndex].motion_conversation > -1 )
							{
								m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_conversation, true );
							}
							else
							{
								m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_idle, true );
							}
						}
						else
						{
							
							if(npcmainscriptitem->ProcessNPCCondition() == NPC_CONDITION_RESULT_SUCCESS)
							{
								if(g_LastNPCScriptOwner->m_bCheckHostility)
								{									
									//2004.07.05->oneway48 inert
									int count = 0;
									for(int i = 0 ; i < 5; ++i)
									{															
										if( npcmainscriptitem->strGreetingHostilityMessage[i] != NULL )
											count++;
									}						
									if( count != 0 ) 
									{
										m_SelectedMessageIndex = _XRandom() % count;
										m_SelectedMessageIndex++;
										if(m_SelectedMessageIndex >= count)
											m_SelectedMessageIndex = 0;	
									}
									else
										m_SelectedMessageIndex = 0;
									
									// 적대적인 문파일 때 
									m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
									m_pNPCScriptWindowPtr->SetMessage(npcmainscriptitem->strGreetingHostilityMessage[m_SelectedMessageIndex]);
									SetMessageString(npcmainscriptitem->strGreetingGeneralMessage[m_SelectedMessageIndex]);
									if(g_SoundEffectFlag)
									{
										if(npcmainscriptitem->indexGreetingHostilitySound[m_SelectedMainScriptIndex] > 0)
										{										
											_XNPCSpeech( g_NPCSoundLUT[npcmainscriptitem->indexGreetingHostilitySound[m_SelectedMainScriptIndex]], 
												D3DXVECTOR3( m_ModelDescriptor.m_Position._41,
												m_ModelDescriptor.m_Position._42,
												m_ModelDescriptor.m_Position._43 ) );
											m_SpecialActionTimer = g_LocalSystemTime;
										}
									}
								}
								else
								{
									//2004.07.05->oneway48 inert
									int count = 0;
									for(int i = 0 ; i < 5; ++i)
									{															
										if( npcmainscriptitem->strGreetingGeneralMessage[i] != NULL )
											count++;
									}						
									if( count != 0 ) 
									{
										m_SelectedMessageIndex = _XRandom() % count;
										m_SelectedMessageIndex++;
										if(m_SelectedMessageIndex >= count)
											m_SelectedMessageIndex = 0;	
									}
									else
										m_SelectedMessageIndex = 0;
									
									m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
									m_pNPCScriptWindowPtr->SetMessage(npcmainscriptitem->strGreetingGeneralMessage[m_SelectedMessageIndex]);
									SetMessageString(npcmainscriptitem->strGreetingGeneralMessage[m_SelectedMessageIndex]);
									if(g_SoundEffectFlag)
									{
										if(npcmainscriptitem->indexGreetingGeneralSound[m_SelectedMainScriptIndex] > 0)
										{										
											_XNPCSpeech( g_NPCSoundLUT[npcmainscriptitem->indexGreetingGeneralSound[m_SelectedMainScriptIndex]], 
												D3DXVECTOR3( m_ModelDescriptor.m_Position._41,
												m_ModelDescriptor.m_Position._42,
												m_ModelDescriptor.m_Position._43 ) );
											m_SpecialActionTimer = g_LocalSystemTime;
										}
									}
								}
							}
						}
					}		
					// subscript print
					int			subscriptcount = 0;
					if(!scriptitem->m_SubScriptList.listEmpty())
					{
						_XNPCScriptItem*		subscript;
						scriptitem->m_SubScriptList.resetList();
						m_pNPCScriptWindowPtr->ClearSubScriptList();
						do 
						{
							subscript = (_XNPCScriptItem *)scriptitem->m_SubScriptList.currentItem();
							m_pNPCScriptWindowPtr->SetSubScriptList(subscriptcount, subscript->scriptclass);
							
							scriptitem->m_SubScriptList.nextItem();
							subscriptcount++;
						} while(!scriptitem->m_SubScriptList.atHeadOfList());
						m_pNPCScriptWindowPtr->m_EventType = scriptitem->m_EventType;
						m_pNPCScriptWindowPtr->GenerateSubScriptList();
					}
					else
					{
						m_pNPCScriptWindowPtr->ClearSubScriptList();
					}		
				}
				else
				{
					if(m_ChangeMessage)
					{
						if( m_MessageType == _XDEF_NPCMESSAGE_NORMAL )
						{
							m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
							m_pNPCScriptWindowPtr->SetMessage(m_MessageString);
							m_ChangeMessage= FALSE;
						}
						else if( m_MessageType == _XDEF_NPCMESSAGE_QUEST )
						{
							_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CHECKING)     >>>>>>>>>>>>>>>>>>>>" );						
							_XLog("NPC : Quest NPCDialogMode Setting : QuestID[%d]" , m_HaveQuestID );
							
							m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
							m_pNPCScriptWindowPtr->SetHaveQuestID(m_HaveQuestID);
							m_pNPCScriptWindowPtr->SetMessage(m_MessageString);
							m_ChangeMessage= FALSE;
						}
#ifdef _XTS_HAVEQUESTLISTNPC
						else if( m_MessageType == _XDEF_NPCMESSAGE_HAVEQUESTLIST )
						{
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_TALKING));
							m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
							
							// subscript print
							int			subscriptcount = 0;
							if(!scriptitem->m_SubScriptList.listEmpty())
							{
								_XNPCScriptItem*		subscript;
								scriptitem->m_SubScriptList.resetList();
								m_pNPCScriptWindowPtr->ClearSubScriptList();
								do 
								{
									subscript = (_XNPCScriptItem *)scriptitem->m_SubScriptList.currentItem();
									m_pNPCScriptWindowPtr->SetSubScriptList(subscriptcount, subscript->scriptclass);
									
									scriptitem->m_SubScriptList.nextItem();
									subscriptcount++;
								} while(!scriptitem->m_SubScriptList.atHeadOfList());
								m_pNPCScriptWindowPtr->GenerateSubScriptList();
							}
						}
#endif
						else
						{
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_TALKING));
							m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);

							// subscript print
							int			subscriptcount = 0;
							if(!scriptitem->m_SubScriptList.listEmpty())
							{
								_XNPCScriptItem*		subscript;
								scriptitem->m_SubScriptList.resetList();
								m_pNPCScriptWindowPtr->ClearSubScriptList();
								do 
								{
									subscript = (_XNPCScriptItem *)scriptitem->m_SubScriptList.currentItem();
									m_pNPCScriptWindowPtr->SetSubScriptList(subscriptcount, subscript->scriptclass);
									
									scriptitem->m_SubScriptList.nextItem();
									subscriptcount++;
								} while(!scriptitem->m_SubScriptList.atHeadOfList());
								m_pNPCScriptWindowPtr->m_EventType = scriptitem->m_EventType;
								m_pNPCScriptWindowPtr->GenerateSubScriptList();
							}
							else
							{
								m_pNPCScriptWindowPtr->ClearSubScriptList();
							}		
						}
					}
					else
					{						
						// 퀘스트가 5개 이상 진행되고 있을 때 
						if(  g_QuestScriptManager.GetProgressQuestCount() >= ID_QUEST_SYNC-1 )
						{
							m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_TALKING));
							m_pNPCScriptWindowPtr->SetMessageType(_XDEF_NPCMESSAGE_NORMAL);
							
							m_pNPCScriptWindowPtr->SetMessage( _XGETINTERFACETEXT(ID_STRING_NEW_2622) );
							// subscript print
							int			subscriptcount = 0;
							if(!scriptitem->m_SubScriptList.listEmpty())
							{
								_XNPCScriptItem*		subscript;
								scriptitem->m_SubScriptList.resetList();
								m_pNPCScriptWindowPtr->ClearSubScriptList();
								do 
								{
									subscript = (_XNPCScriptItem *)scriptitem->m_SubScriptList.currentItem();
									m_pNPCScriptWindowPtr->SetSubScriptList(subscriptcount, subscript->scriptclass);
									
									scriptitem->m_SubScriptList.nextItem();
									subscriptcount++;
								} while(!scriptitem->m_SubScriptList.atHeadOfList());
								m_pNPCScriptWindowPtr->m_EventType = scriptitem->m_EventType;
								m_pNPCScriptWindowPtr->GenerateSubScriptList();
							}
							else
							{
								m_pNPCScriptWindowPtr->ClearSubScriptList();
							}		
						}
					}
				}

				if(m_ChangeNormalMessage)
					m_ChangeNormalMessage = FALSE;
			}
			else
			{
				if(m_ChangeMessage)
				{
					if( m_MessageType == _XDEF_NPCMESSAGE_NORMAL )
					{
						m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
						m_pNPCScriptWindowPtr->SetMessage(m_MessageString);
						m_ChangeMessage= FALSE;
					}
					else if( m_MessageType == _XDEF_NPCMESSAGE_QUEST )
					{
						_XLog("<<<<<<<<<<<<<<<<<<<<     QUEST MESSAGE (CLIENT CHECKING)     >>>>>>>>>>>>>>>>>>>>" );						
						_XLog("NPC : Quest NPCDialogMode Setting : QuestID[%d]" , m_HaveQuestID );

						m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
						m_pNPCScriptWindowPtr->SetHaveQuestID(m_HaveQuestID);
						m_pNPCScriptWindowPtr->SetMessage(m_MessageString);
						m_ChangeMessage= FALSE;
					}
#ifdef _XTS_HAVEQUESTLISTNPC
					else if( m_MessageType == _XDEF_NPCMESSAGE_HAVEQUESTLIST )
					{
						m_pNPCScriptWindowPtr->SetTitle(_XGETINTERFACETEXT(ID_STRING_NPCSCRIPT_TALKING));
						m_pNPCScriptWindowPtr->SetMessageType(m_MessageType);
						
						// subscript print
						int			subscriptcount = 0;
						if(!scriptitem->m_SubScriptList.listEmpty())
						{
							_XNPCScriptItem*		subscript;
							scriptitem->m_SubScriptList.resetList();
							m_pNPCScriptWindowPtr->ClearSubScriptList();
							do 
							{
								subscript = (_XNPCScriptItem *)scriptitem->m_SubScriptList.currentItem();
								m_pNPCScriptWindowPtr->SetSubScriptList(subscriptcount, subscript->scriptclass);
								
								scriptitem->m_SubScriptList.nextItem();
								subscriptcount++;
							} while(!scriptitem->m_SubScriptList.atHeadOfList());
							m_pNPCScriptWindowPtr->GenerateSubScriptList();
						}

						m_ChangeMessage= FALSE;
					}
#endif
				}
			}
		}
		m_PrevMainScriptIndex = m_SelectedMainScriptIndex;
	}
}

void _XNPCObject::SetConversationAction(void)
{
	if( g_NPCMotionExistTable[m_ObjectIndex].motion_conversation > -1 )
	{
		m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_conversation, true );
	}
	else
	{
		m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_idle, true );
	}
}

void _XNPCObject::SetIdleAction(void)
{
	if( g_NPCMotionExistTable[m_ObjectIndex].motion_idle > -1 )
		m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_idle, true );
}

void _XNPCObject::SetSurrenderAction(void)
{
	if( g_NPCMotionExistTable[m_ObjectIndex].motion_damage > -1 )
	{
		m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_damage, true );
	}
	else
	{
		m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_idle, true );
	}
}

void _XNPCObject::DrawNPCNameWindow(void)
{
	XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
	if( pProc_Maingame->m_bFunctionalObjectWideView ) return;

	D3DXVECTOR3 mpos, rvec;
	
	mpos.x = m_ModelDescriptor.m_Position._41;
	mpos.y = m_ModelDescriptor.m_Position._42 + (m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2);
	mpos.z = m_ModelDescriptor.m_Position._43;
	
	D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
		&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
	
	g_NPCNameWindow.m_EventType = m_EventType;
	g_NPCNameWindow.Draw( rvec.x, rvec.y-36.0f, m_NPCRoll, m_NPCName, FALSE, m_HaveQuestType, m_NPCJobClass );

		
	// 이름창 클릭시 NPC 클릭과 동일하게 처리 
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	if( ((scrnpos->x > rvec.x-(g_NPCNameWindow.m_WindowSize.cx>>1) ) && (scrnpos->x < rvec.x + (g_NPCNameWindow.m_WindowSize.cx>>1) ) &&
		(scrnpos->z > rvec.y-36.0f ) && (scrnpos->z < rvec.y-36.0f + g_NPCNameWindow.m_WindowSize.cy) ) )
	{			
		if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
		{			
			if( m_Distance < g_LastPickedObjectDistance )
			{
				g_LastPickedObjectDistance = m_Distance;
				g_LastPickedObjectType = _XPICKINGTARGET_NPC;
				g_LastPickedObject = (_XItem*)this;		
				
				if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView )
				{
					CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
					eodapp->ReadyCursor( _XMOUSETYPE_CONVERSATION );
				}
			}
		}			
	}
}
void _XNPCObject::DrawNPCChat(void)
{
	_XNPCMainScript* mainscript = (_XNPCMainScript*)m_Triggerlist.getItem(0);
	if(mainscript) 
	{			
		if(mainscript->strPrevGreetingMessage[m_SelectedMessageIndex] != NULL)
		{						
			D3DXVECTOR3 mpos, rvec;
			
			mpos.x = m_ModelDescriptor.m_Position._41;
			mpos.y = m_ModelDescriptor.m_Position._42 + (m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2);
			mpos.z = m_ModelDescriptor.m_Position._43;
			
			D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
				&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);

			m_NPCChatBalloonObject.SetMessage(mainscript->strPrevGreetingMessage[m_SelectedMessageIndex]);
			m_NPCChatBalloonObject.Draw(rvec.x , rvec.y-28.0f );
			
			// 이름창 클릭시 NPC 클릭과 동일하게 처리 
			ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
			if( (scrnpos->x > rvec.x-95.0f ) && (scrnpos->x < rvec.x + 95.0f ) )
			{
				if(	(scrnpos->z > rvec.y-36.0f ) && (scrnpos->z < rvec.y-36.0f + 28) ) 
				{	
					if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
					{			
						if( m_Distance < g_LastPickedObjectDistance )
						{
							g_LastPickedObjectDistance = m_Distance;
							g_LastPickedObjectType = _XPICKINGTARGET_NPC;
							g_LastPickedObject = (_XItem*)this;		
							
							XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
							if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
							{
								CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
								eodapp->ReadyCursor( _XMOUSETYPE_CONVERSATION );
							}
						}
					}			
				}		
			}
		}
		else	// 인사말전 대사의 내용이 없으면 이름창을 그린다.
		{
			DrawNPCNameWindow();
		}
	}
	else
	{
		DrawNPCNameWindow();
	}
}
void _XNPCObject::PlaySoundNPCPrevGreeting(void)
{
	_XNPCMainScript* mainscript = (_XNPCMainScript*)m_Triggerlist.getItem(0);
	
	if( mainscript ) 
	{
		int m_SelectedMessageIndex;
		// m_SelectedMessageIndex = _XRandom() % 5;
		// m_SelectedMessageIndex++;
		// if(m_SelectedMessageIndex >= 5)
		// m_SelectedMessageIndex = 0;
		
		m_SelectedMessageIndex = 0;
		
		if(mainscript->indexPrevGreetingSound[m_SelectedMessageIndex] > 0)
		{										
			D3DXVECTOR3 position;
			position = D3DXVECTOR3( m_ModelDescriptor.m_Position._41,
				m_ModelDescriptor.m_Position._42, m_ModelDescriptor.m_Position._43 );
			
			_XNPCSpeech( g_NPCSoundLUT[mainscript->indexPrevGreetingSound[m_SelectedMessageIndex]], position );
		}
	}
}

void _XNPCObject::ProcessNPCNameAndChat(void)
{	
	if( m_InsideFrustum )
	{
		bool hiddentype = false;		
		if( m_NPCJobClass != _XNPC_JOBCLASS_BLACKMARKET)
		{
			if( !g_pLocalUser->m_bIsBossBattle )
			{				
				hiddentype = false;
			}//if( !g_pLocalUser->m_bIsBossBattle )
			else //생사결 안에서는 다르게 처리 
			{
				hiddentype = true;
			}
		}
		else // 암상인일때 
		{
			hiddentype = true;			
		}//암상인일때

		if( !hiddentype )
		{
			if( m_DistanceFromLocalUserToNPC < 30.0f)
			{
				if( m_EventType != -1 ) // 이벤트가 있으면 혼잣말을 하지 않는다.
				{
					DrawNPCNameWindow();
				}
				else if( m_HaveQuestType == _XNPC_HAVEQUEST_NONE)
				{
					if( m_DistanceFromLocalUserToNPC > 15.0f)
					{	
						DrawNPCNameWindow();// 거리가 15-30 사이이면 무조건 이름창만 띄운다.
						m_bDrawNPCChat = TRUE; // 거리가 15이하일 때 먼저 말풍선부터 띄움.
						m_NPCChatBallonTimer = 0;
					}
					else//거리가 15보다 작을때 
					{				
						if(m_bDrawNPCChat)
						{	
							
							if(m_NPCChatBallonTimer == 0)
							{
								m_NPCChatBallonTimer = g_LocalSystemTime;
								
								_XNPCMainScript* mainscript = (_XNPCMainScript*)m_Triggerlist.getItem(0);
								if(mainscript) 
								{		
									int count = 0;
									for(int i = 0 ; i < 5; ++i)
									{															
										if( mainscript->strPrevGreetingMessage[i] != NULL )
											count++;
									}				
									if( count != 0 ) 
									{
										m_SelectedMessageIndex = _XRandom() % count;
										m_SelectedMessageIndex++;
										if(m_SelectedMessageIndex >= count)
											m_SelectedMessageIndex = 0;	
									}
									else
										m_SelectedMessageIndex = 0;
									
								}
							}
							
							DrawNPCChat();
							if( g_LocalSystemTime - m_NPCChatBallonTimer > _XDEF_NPCTIME_CHATBALLOON )// 5초동안 띄운다.
							{
								m_bDrawNPCChat = FALSE;
								m_NPCChatBallonTimer = g_LocalSystemTime;
							}
						}
						else
						{
							DrawNPCNameWindow();
							
							// 말풍선이 끝나고 대기하는 30초 사이에 음성 삽입
							if( g_LocalSystemTime - m_NPCChatBallonTimer > _XDEF_NPCTIME_SOUNDPREVGREETING 
								&&  g_LocalSystemTime - m_NPCChatBallonTimer < (_XDEF_NPCTIME_SOUNDPREVGREETING + 50) )
							{
								// NPC 혼자말 음성 
								if(!m_bNPCDialogMode)
								{
									PlaySoundNPCPrevGreeting();
								}
							}
							
							if( g_LocalSystemTime - m_NPCChatBallonTimer > _XDEF_NPCTIME_CHATBALLOONWAITING )//30초동안 대기
							{
								m_bDrawNPCChat = TRUE;
								m_NPCChatBallonTimer = g_LocalSystemTime;
							}
						}
					}						
				}
				else // 퀘스트를 가지고 있으면 이름창만 띄운다.
				{
					DrawNPCNameWindow();
				}
			} // if( m_DistanceFromLocalUserToNPC < 30.0f)
		}
		else
		{
			if( m_DistanceFromLocalUserToNPC < 2.0f)
			{
				if( m_HaveQuestType == _XNPC_HAVEQUEST_NONE)
				{
					if( m_DistanceFromLocalUserToNPC > 1.5f)
					{	
						DrawNPCNameWindow();// 거리가 1.5 - 2.0사이이면 무조건 이름창만 띄운다.
						m_bDrawNPCChat = TRUE; // 거리가 1.5이하일 때 먼저 말풍선부터 띄움.
						m_NPCChatBallonTimer = 0;
					}
					else//거리가 1.5보다 작을때 
					{				
						if(m_bDrawNPCChat)
						{	
							
							if(m_NPCChatBallonTimer == 0)
							{
								m_NPCChatBallonTimer = g_LocalSystemTime;
								
								_XNPCMainScript* mainscript = (_XNPCMainScript*)m_Triggerlist.getItem(0);
								if(mainscript) 
								{		
									int count = 0;
									for(int i = 0 ; i < 5; ++i)
									{															
										if( mainscript->strPrevGreetingMessage[i] != NULL )
											count++;
									}						
									if( count != 0 ) 
									{
										m_SelectedMessageIndex = _XRandom() % count;
										m_SelectedMessageIndex++;
										if(m_SelectedMessageIndex >= count)
											m_SelectedMessageIndex = 0;	
									}
									else
										m_SelectedMessageIndex = 0;
								}
							}
							
							DrawNPCChat();
							if( g_LocalSystemTime - m_NPCChatBallonTimer > _XDEF_NPCTIME_CHATBALLOON )// 5초동안 띄운다.
							{
								m_bDrawNPCChat = FALSE;
								m_NPCChatBallonTimer = g_LocalSystemTime;
							}
						}
						else
						{
							DrawNPCNameWindow();
							
							// 말풍선이 끝나고 대기하는 30초 사이에 음성 삽입
							if( g_LocalSystemTime - m_NPCChatBallonTimer > _XDEF_NPCTIME_SOUNDPREVGREETING 
								&&  g_LocalSystemTime - m_NPCChatBallonTimer < (_XDEF_NPCTIME_SOUNDPREVGREETING + 50) )
							{
								// NPC 혼자말 음성 
								if(!m_bNPCDialogMode)
								{
									PlaySoundNPCPrevGreeting();
								}
							}
							
							if( g_LocalSystemTime - m_NPCChatBallonTimer > _XDEF_NPCTIME_CHATBALLOONWAITING )//30초동안 대기
							{
								m_bDrawNPCChat = TRUE;
								m_NPCChatBallonTimer = g_LocalSystemTime;
							}
						}
					}						
				}
				else // 퀘스트를 가지고 있으면 이름창만 띄운다.
				{
					DrawNPCNameWindow();
				}
			} // if( m_DistanceFromLocalUserToNPC < 2.0f)
		}
	} // if( m_InsideFrustum )
}

void _XNPCObject::Render(void)
{
	bool soundplay = true;
	m_ModelDescriptor.Advance( soundplay, 0 );
	m_ModelDescriptor.Animation();

	if( !m_bNPCDialogMode )
	{
		if( m_ModelDescriptor.GetCurMotion() == g_NPCMotionExistTable[m_ObjectIndex].motion_idle )
		{
			if( m_SpecialActionTimer == 0 ) m_SpecialActionTimer = g_LocalSystemTime;
			
			// idle action time이 되었을때 action 으로 교체 
			if( g_LocalSystemTime - m_SpecialActionTimer > _XDEF_CHANGEIDLEMOTIONTIME )
			{				
				m_SpecialActionTimer = g_LocalSystemTime;
					
				// action 으로 교체 
				if( g_NPCMotionExistTable[m_ObjectIndex].motion_idleaction1 > -1 )
				{
					m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_idleaction1, true );
				}
				//else // action motion이 존재 하지 않는경우는 idle로 유지
			}
		}
			
		if( m_ModelDescriptor.GetCurMotion() == g_NPCMotionExistTable[m_ObjectIndex].motion_idleaction1 )
		{
			if( m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
			{
				m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_idle, true );
			}
		}
		if(m_ModelDescriptor.GetCurMotion() == g_NPCMotionExistTable[m_ObjectIndex].motion_damage)
		{
			if( m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
			{
				m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_idle, true );
			}
		}
	}
	else
	{		
		if( m_ModelDescriptor.GetCurMotion() == g_NPCMotionExistTable[m_ObjectIndex].motion_conversation )
		{
			if( m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
			{
				m_ModelDescriptor.SetCurMotion( g_NPCMotionExistTable[m_ObjectIndex].motion_idle, true );
			}
		}
	}
	

	m_ModelDescriptor.Render();	

	if( !m_bNPCDialogMode )
	{
		// NPC 혼잣말이나 이름창을 그리는 부분 
		ProcessNPCNameAndChat();
	}

}

void _XNPCObject::SetMessageString(LPCTSTR string)
{
	if(!string)
		return;
	
	int strlength = strlen(string);
	
	if(m_MessageString)
	{
		delete m_MessageString;
		m_MessageString = NULL;
	}
	
	if(strlength > 0)
	{
		m_MessageString = new TCHAR[strlength + 1];
		m_MessageString[strlength] = 0;
		strcpy(m_MessageString, string);
	}
}

void _XNPCObject::SetMessageType(_XNPCMESSAGETYPE messagetype)
{
	m_MessageType = messagetype;	
}

void _XNPCObject::SetChangeMessage(BOOL change)
{
	m_ChangeMessage = change;
}

void _XNPCObject::SetChangeButton(BOOL button)
{
	m_ChangeButton = button;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// NPC object manager
_XNPCObjectManager::_XNPCObjectManager()
{
	m_pNPCObjectArray = NULL;
	m_NPCObjectCount = 0;
	
	m_NPCObjectRenderCount = 0;
	m_pNPCObjectSortRenderArray = NULL;

	for(int i = 0 ; i < 150 ; ++i)
	{
		g_NPCSoundLUT[i] = NULL;
	}
}

_XNPCObjectManager::~_XNPCObjectManager()
{
	disposeList();
}

void _XNPCObjectManager::disposeList( void  )
{
	ssdef_INT::iterator it;
	for (it = m_ssNpcIndex.begin(); it != m_ssNpcIndex.end(); ++ it)
	{
		int index = (*it);
		g_NPCBIPEDMODEL[index].DestroyModel();
		g_NPCMODEL[index]->DestroyModel();
	}

	m_ssNpcIndex.clear();
	SAFE_DELETE_ARRAY( m_pNPCObjectArray );
	m_NPCObjectCount = 0;

	SAFE_DELETE_ARRAY( m_pNPCObjectSortRenderArray );
	m_NPCObjectRenderCount = 0;
}

BOOL _XNPCObjectManager::LoadNPCObjectList( FILE* Fileptr, LPSTR Filename )
{
	assert( Fileptr );
	if( !Fileptr ) 
	{
		_XFatalError( "LoadNPCObjectList(%s) : Invalid file pointer", Filename );
		return FALSE;
	}

	if( fread( g_LodTerrain.m_NPCListFilename, _MAX_PATH, 1, Fileptr ) < 1 )
	{
		_XFatalError( "LoadNPCObjectList(%s) Read npc list info", Filename );
		return FALSE;
	}
		
	int objectcount = 0;
	if( fread( &objectcount, sizeof(int), 1, Fileptr ) < 1 )
	{
		_XFatalError( "LoadNPCObjectList(%s) Read object count", Filename );		
		return FALSE;
	}
	
	disposeList();

	if( objectcount <= 0 ) return TRUE;

	// ==========================================================================
	// NPC Sound LUT
			
	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;
	
	FILE* pScripterFile = NULL;

	pScripterFile = g_ScriptArchive.GetPackedFile( _T("NPCSoundList.ini") );

	if(!pScripterFile)
	{
		_XFatalError("Can't Open NPC Sound list file");
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	TCHAR		szBuffer[256];
	int			soundfilecount = 0;
	while(1)
	{
		if(feof(pScripterFile)) break;
		
		memset(szBuffer, 0, sizeof(char)*256);
		if(!fgets(szBuffer, 255, pScripterFile))
		{
			break;
		}

		if( szBuffer[0] == ';' || szBuffer[0] == '\n' || !strlen(szBuffer) ) continue;

		if( strncmp( szBuffer, "_XM_EOS", 7 ) == 0 )
		{
			break;			
		}		

		g_NPCSoundAllTable[soundfilecount] = new TCHAR[256];
		
		szBuffer[ strlen(szBuffer)-2 ] = _T('\0');		

		strcpy(g_NPCSoundAllTable[soundfilecount], szBuffer);

		soundfilecount++;
	}
	
	g_ScriptArchive.ClosePackage();

	memset(g_NPCSoundUsedTable, 0, sizeof(int)*150);
	// =========================================================================

	m_NPCObjectCount = objectcount;

	m_pNPCObjectArray = new _XNPCObject[m_NPCObjectCount];
	m_pNPCObjectSortRenderArray = new _XNPCObject*[m_NPCObjectCount];
	
	if( !m_pNPCObjectArray )
	{
		_XFatalError( "LoadObjectList(%s) Allocation NPC object array", Filename );
		return FALSE;
	}

	_XObject savedobject;

	for( int _i = 0; _i < m_NPCObjectCount; ++_i )
	{
		if( fread( &m_pNPCObjectArray[_i].m_UniqueID, sizeof(int), 1, Fileptr ) < 1 )
		{
			_XFatalError( "LoadTerrain(%s) Read npc unique id", Filename );
			fclose( Fileptr );
			return FALSE;
		}	

		if( fread( &m_pNPCObjectArray[_i].m_NPCJobClass, sizeof(int), 1, Fileptr ) < 1 )
		{
			_XFatalError( "LoadTerrain(%s) Read npc zone class", Filename );
			fclose( Fileptr );
			return FALSE;
		}	

		if( fread( &m_pNPCObjectArray[_i].m_TradeItemClass, sizeof(int), 1, Fileptr ) < 1 )
		{
			_XFatalError( "LoadTerrain(%s) Read npc trade class", Filename );
			fclose( Fileptr );
			return FALSE;
		}

		if( fread( &m_pNPCObjectArray[_i].m_NPCClass, sizeof(int), 1, Fileptr ) < 1 )
		{
			_XFatalError( "LoadTerrain(%s) Read npc class", Filename );
			fclose( Fileptr );
			return FALSE;
		}	
		
		if( m_pNPCObjectArray[_i].m_NPCClass == _XNPC_CLASS_ONOFFTYPE )
		{
			m_pNPCObjectArray[_i].m_bShowNPCModel = FALSE;
		}
		
		if( fread( &m_pNPCObjectArray[_i].m_NPCClanClass, sizeof(int), 1, Fileptr ) < 1 )
		{
			_XFatalError( "LoadTerrain(%s) Read npc clan class", Filename );
			fclose( Fileptr );
			return FALSE;
		}

		int strlength = 0;		
		if( fread( &strlength, sizeof(int), 1, Fileptr ) < 1 )
		{
			_XFatalError( "LoadTerrain(%s) Read npc name size", Filename );
			fclose( Fileptr );
			return FALSE;
		}

#ifdef _XDEF_USENEWNPCTEXTSYSTEM
		int resourceid = (m_pNPCObjectArray[_i].m_UniqueID * 1000) + 1;
		m_pNPCObjectArray[_i].m_NPCName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid );
		fseek( Fileptr, strlength, SEEK_CUR );
#else
		if( m_pNPCObjectArray[_i].m_NPCName )
		{
			delete[] m_pNPCObjectArray[_i].m_NPCName;
			m_pNPCObjectArray[_i].m_NPCName = NULL;
		}
		
		if( strlength > 0 )
		{
			m_pNPCObjectArray[_i].m_NPCName = new TCHAR[ strlength+1 ];
			
			if( !m_pNPCObjectArray[_i].m_NPCName )
			{
				_XFatalError( "LoadTerrain(%s) Can't allocate npc name string buffer", Filename );
				return FALSE;
			}
			
			memset( m_pNPCObjectArray[_i].m_NPCName, 0, sizeof(TCHAR)*(strlength+1) );
			
			if( fread( m_pNPCObjectArray[_i].m_NPCName, strlength, 1, Fileptr ) < 1 )
			{
				_XFatalError( "LoadTerrain(%s) Read npc name", Filename );
				fclose( Fileptr );
				return FALSE;
			}		
		}
#endif		
		// ===========================================================================
		strlength = 0;
		if(fread(&strlength, sizeof(strlength), 1, Fileptr) < 1)
		{
			_XFatalError("LoadTerrain(%s) Read npc roll size", Filename);
			fclose(Fileptr);
			return FALSE;
		}

#ifdef _XDEF_USENEWNPCTEXTSYSTEM
		resourceid = (m_pNPCObjectArray[_i].m_UniqueID * 1000) + 2;
		m_pNPCObjectArray[_i].m_NPCRoll = g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid );
		fseek( Fileptr, strlength, SEEK_CUR );
#else
		if(m_pNPCObjectArray[_i].m_NPCRoll)
		{
			delete[] m_pNPCObjectArray[_i].m_NPCRoll;
			m_pNPCObjectArray[_i].m_NPCRoll = NULL;
		}

		if(strlength > 0)
		{
			m_pNPCObjectArray[_i].m_NPCRoll = new TCHAR[strlength + 1];
			if(!m_pNPCObjectArray[_i].m_NPCRoll)
			{
				_XFatalError("LoadTerrain(%S) Can't allocate npc roll string buffer", Filename);
				return FALSE;
			}

			memset(m_pNPCObjectArray[_i].m_NPCRoll, 0, sizeof(TCHAR)*(strlength+1));

			if(fread(m_pNPCObjectArray[_i].m_NPCRoll, strlength, 1, Fileptr) < 1)
			{
				_XFatalError("LoadTerrain(%s) Read npc roll", Filename);
				fclose(Fileptr);
				return FALSE;
			}
		}
#endif
		// ================================================================================

		if( fread( &savedobject, sizeof(_XObject), 1, Fileptr ) < 1 ) 
		{			
			_XFatalError( "LoadTerrain(%s) Read NPC object data", Filename );
			return FALSE;
		}
				
		m_pNPCObjectArray[_i].m_ObjectIndex = savedobject.m_ObjectIndex;
		m_pNPCObjectArray[_i].m_matWorldPosition = savedobject.m_matWorldPosition;
		m_pNPCObjectArray[_i].m_ModelDescriptor.m_Position = savedobject.m_matWorldPosition;
		m_pNPCObjectArray[_i].m_ModelDescriptor.ReserveModelStack(1);
		m_pNPCObjectArray[_i].m_ModelDescriptor.SetMeshModel( 0, g_NPCMODEL[savedobject.m_ObjectIndex] );
		m_pNPCObjectArray[_i].m_ModelDescriptor.LinkBipedObject( &g_NPCBIPEDMODEL[savedobject.m_ObjectIndex] );
		m_pNPCObjectArray[_i].m_ModelDescriptor.PlayAnimation( true );
		m_pNPCObjectArray[_i].CreateEffectManager();

		m_ssNpcIndex.insert(savedobject.m_ObjectIndex);

		/*FLOAT height = g_LodTerrain.GetTerrainHeight( D3DXVECTOR3( savedobject.m_matWorldPosition._41, 
			                                                       savedobject.m_matWorldPosition._42, 
																   savedobject.m_matWorldPosition._43  ) );

		if( fabs( height - savedobject.m_matWorldPosition._42 ) < 0.2f )
		{
			m_pNPCObjectArray[_i].m_ModelDescriptor.m_Position._42 = height;
			m_pNPCObjectArray[_i].m_matWorldPosition._42 = height;
		}*/

		if( !m_pNPCObjectArray[_i].LoadScript( Fileptr ) )
		{
			_XFatalError( "LoadTerrain(%s) read npc private script data", Filename );			
			return FALSE;
		}
	}

// ====================================================================================================
// ====================================================================================================	
	if( !g_ModelArchive.ReOpenPackage() ) 
		return FALSE;
	if( !g_AniArchive.ReOpenPackage() ) 
	{
		g_ModelArchive.ClosePackage();
		return FALSE;
	}


	char buff[256];
	_stprintf(buff, "Npc Count %d \n", m_ssNpcIndex.size());
	OutputDebugString(buff);

	ssdef_INT::iterator it;
	for (it = m_ssNpcIndex.begin(); it != m_ssNpcIndex.end(); ++ it)
	{
		int index = (*it);

		if (!g_NPCBIPEDMODEL[index].m_strBipedName.empty())
		{
			if( !g_NPCBIPEDMODEL[index].Load( g_ModelArchive.GetPackedFile( const_cast<char*>(g_NPCBIPEDMODEL[index].m_strBipedName.c_str()) ), const_cast<char*>(g_NPCBIPEDMODEL[index].m_strBipedName.c_str()) ) )
			{
				break;
			}
		}

		if (!g_NPCBIPEDMODEL[index].m_svTempMotion.empty())
		{
			if( !g_NPCBIPEDMODEL[index].ReserveAnimationStack( g_NPCBIPEDMODEL[index].m_svTempMotion.size() ) )
			{
				break;
			}
		}

		int i = 0;
		svdef_TEMP_MOTION::iterator it;
		for (it = g_NPCBIPEDMODEL[index].m_svTempMotion.begin(); it != g_NPCBIPEDMODEL[index].m_svTempMotion.end(); ++ i, ++ it)
		{
			TEMP_MOTION* pTempBipedNode = (*it);
			if (!pTempBipedNode->strKeyname.empty())
			{
				if( !g_NPCBIPEDMODEL[index].LoadKey( g_AniArchive.GetPackedFile( const_cast<char*>(pTempBipedNode->strKeyname.c_str()) ), const_cast<char*>(pTempBipedNode->strKeyname.c_str()) ) )
				{
					break;
				}
			}

			if (pTempBipedNode->bSoundEffectFrame == TRUE)
			{
				for( int j = 0; j < 9; ++j )
					g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetSoundEffectFrame(j, pTempBipedNode->SoundOutFrame[j], pTempBipedNode->SoundIndex[j] );
			}
/*
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 0, pTempBipedNode->AttackCmdFrame[0] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 1, pTempBipedNode->AttackCmdFrame[1] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 2, pTempBipedNode->AttackCmdFrame[2] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 3, pTempBipedNode->AttackCmdFrame[3] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 4, pTempBipedNode->AttackCmdFrame[4] );				
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 5, pTempBipedNode->AttackCmdFrame[5] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 6, pTempBipedNode->AttackCmdFrame[6] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 7, pTempBipedNode->AttackCmdFrame[7] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackFrame( 8, pTempBipedNode->AttackCmdFrame[8] );

			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 0, pTempBipedNode->AttackClass[0] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 1, pTempBipedNode->AttackClass[1] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 2, pTempBipedNode->AttackClass[2] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 3, pTempBipedNode->AttackClass[3] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 4, pTempBipedNode->AttackClass[4] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 5, pTempBipedNode->AttackClass[5] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 6, pTempBipedNode->AttackClass[6] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 7, pTempBipedNode->AttackClass[7] );
			g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetAttackClass( 8, pTempBipedNode->AttackClass[8] );

			for( j = 0; j < 9; j++ )
				g_NPCBIPEDMODEL[index].GetMotion( g_NPCBIPEDMODEL[index].GetMotionCnt()-1 )->SetEffectFrame(j, pTempBipedNode->EffectFrame[j], pTempBipedNode->EffectIndex[j] );
*/
		}

		if (!g_NPCMODEL[index]->m_strFileName.empty())
		{
			if( !g_NPCMODEL[index]->Load_Runtime( g_ModelArchive.GetPackedFile( const_cast<char*>(g_NPCMODEL[index]->m_strFileName.c_str()) ), 
				&g_MobTextureArchive, 
				!_XAnimMeshAbstractClass::m_UseVertexShaderAnimation, 
				const_cast<char*>(g_NPCMODEL[index]->m_strFileName.c_str()) ) )
			{
				break;
			}
		}
	}

	g_AniArchive.ClosePackage();
	g_ModelArchive.ClosePackage();

// ====================================================================================================
// ====================================================================================================
		
	// NPC Sound LUT Release
	for(int i = 0 ; i < soundfilecount ; ++i)
	{
		SAFE_DELETE(g_NPCSoundAllTable[i]);
	}
	
	_XDWINPRINT( "%d NPC Object loaded", m_NPCObjectCount );
				
	return TRUE;
}


int CompareNPCDistance(const void *pArg1, const void *pArg2)
{
	_XNPCObject** o1 = (_XNPCObject**)pArg1;
	_XNPCObject** o2 = (_XNPCObject**)pArg2;
	return ( (o1[0]->m_Distance < o2[0]->m_Distance ) ? 1 : -1);
}

void _XNPCObjectManager::CheckInsideFrustum( void )
{
	m_NPCObjectRenderCount = 0;

	for( int _i = 0; _i < m_NPCObjectCount; ++_i )
	{	
		FLOAT dx = (m_pNPCObjectArray[_i].m_matWorldPosition._41 - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
		FLOAT dz = (m_pNPCObjectArray[_i].m_matWorldPosition._43 - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
		m_pNPCObjectArray[_i].m_Distance = _XFastDistance( dx , dz );
		
		dx = (m_pNPCObjectArray[_i].m_matWorldPosition._41 - g_pLocalUser->m_Position.x );
		dz = (m_pNPCObjectArray[_i].m_matWorldPosition._43 - g_pLocalUser->m_Position.z);
		m_pNPCObjectArray[_i].m_DistanceFromLocalUserToNPC = _XFastDistance( dx , dz );
		
		FLOAT radius = 1.0f;
		if( m_pNPCObjectArray[_i].m_ModelDescriptor.m_ppModelList )
			radius = m_pNPCObjectArray[_i].m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius;
		
		m_pNPCObjectArray[_i].m_InsideFrustum = 
			( ( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( 
					D3DXVECTOR3( m_pNPCObjectArray[_i].m_ModelDescriptor.m_Position._41, 
					m_pNPCObjectArray[_i].m_ModelDescriptor.m_Position._42, 
					m_pNPCObjectArray[_i].m_ModelDescriptor.m_Position._43 ), 
				radius*2 ) != NOT_VISIBLE ) );

		if( m_pNPCObjectArray[_i].m_Distance < _XDEF_NPC_CULLINGRADIUS )
		{		
			m_pNPCObjectSortRenderArray[m_NPCObjectRenderCount] = &m_pNPCObjectArray[_i];
			m_NPCObjectRenderCount++;
		}
	}

	if( m_NPCObjectRenderCount > 1 )
	{
		qsort((void*)m_pNPCObjectSortRenderArray, m_NPCObjectRenderCount, sizeof(_XNPCObject*), CompareNPCDistance);
	}
}

void _XNPCObjectManager::CheckConditionOfNPCExistence( void )
{
	for( int _i = 0; _i < m_NPCObjectCount; ++_i )
	{	
		if( m_pNPCObjectArray[_i].m_NPCClass == _XNPC_CLASS_ONOFFTYPE )
		{
			_XNPCMainScript* mainscript = (_XNPCMainScript*)m_pNPCObjectArray[_i].m_Triggerlist.getItem(0);
			if(mainscript) 
			{
				if( !mainscript->m_NPCScriptConditionList.listEmpty() )
				{
					mainscript->m_NPCScriptConditionList.resetList();
					_XNPCScriptConditionItem* conditionitem = NULL;
					do 
					{
						conditionitem = (_XNPCScriptConditionItem *)mainscript->m_NPCScriptConditionList.currentItem();
						if( conditionitem )
						{							
							if( conditionitem->m_NPCSubscriptCondition.m_conditionclass == ID_NPCSUBSCRIPT_CONDITION_NPCEXISTENCE )
							{
								if( conditionitem->m_NPCSubscriptCondition.m_compareclass1 == ID_NPCSUBSCRIPT_NPCEXISTENCE_ON )
								{
									if( conditionitem->ProcessCheckConditionOfNPCExistence() == NPC_CONDITION_RESULT_SUCCESS )
									{
										m_pNPCObjectArray[_i].m_bShowNPCModel = TRUE;
									}
								}
								else 
								{
									if( conditionitem->ProcessCheckConditionOfNPCExistence() == NPC_CONDITION_RESULT_SUCCESS )
									{
										m_pNPCObjectArray[_i].m_bShowNPCModel = FALSE;
									}
								}
								
							}	
						}
						mainscript->m_NPCScriptConditionList.nextItem();
					} while(!mainscript->m_NPCScriptConditionList.atHeadOfList());
				}//	if( !mainscript->m_NPCScriptConditionList.listEmpty() )		
			}
		}	
	}
}

void _XNPCObjectManager::Render( void )
{	
	for( int _i = 0; _i < m_NPCObjectRenderCount; ++_i )
	{
		if( m_pNPCObjectSortRenderArray[_i]->m_bShowNPCModel )//생성 삭제용 NPC 는 조건에 따라 그릴수도 있고 안 그릴수도 있다.
		{
			if( m_pNPCObjectSortRenderArray[_i]->m_Distance < 16.0f )
			{
				m_pNPCObjectSortRenderArray[_i]->m_ModelDescriptor.SetShadowDecalInfo( 
						m_pNPCObjectSortRenderArray[_i]->m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y,
						m_pNPCObjectSortRenderArray[_i]->m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y * 2.0f,
						m_pNPCObjectSortRenderArray[_i]->m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius * 2.8f );
				
				m_pNPCObjectSortRenderArray[_i]->m_ModelDescriptor.RenderShadow();
			}
			
			if( m_pNPCObjectSortRenderArray[_i]->CheckIntersectRay() )
			{				
				if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
				{
					g_pCurrentSelectMaterial = &g_SelectedObjectMaterial;
					
					if( m_pNPCObjectSortRenderArray[_i]->m_Distance < g_LastPickedObjectDistance )
					{
						g_LastPickedObjectDistance = m_pNPCObjectSortRenderArray[_i]->m_Distance;
						g_LastPickedObjectType = _XPICKINGTARGET_NPC;
						g_LastPickedObject = (_XItem*)m_pNPCObjectSortRenderArray[_i];
						
						XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
						if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
						{
							CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
							eodapp->ReadyCursor( _XMOUSETYPE_CONVERSATION );
						}
					}
				}
				m_pNPCObjectSortRenderArray[_i]->Render();				
				g_pCurrentSelectMaterial = NULL;
			}
			else
			{				
				m_pNPCObjectSortRenderArray[_i]->Render();				
			}			
		}
	}
	

	/*
	for( int _i = 0; _i < m_NPCObjectCount; _i++ )
	{		
		if( m_pNPCObjectArray[_i].m_bShowNPCModel )//생성 삭제용 NPC 는 조건에 따라 그릴수도 있고 안 그릴수도 있다.
		{
			if( m_pNPCObjectArray[_i].m_InsideFrustum && m_pNPCObjectArray[_i].m_Distance < _XDEF_NPC_CULLINGRADIUS )
			{			
				if( m_pNPCObjectArray[_i].m_Distance < 16.0f )
				{
					m_pNPCObjectArray[_i].m_ModelDescriptor.SetShadowDecalInfo( m_pNPCObjectArray[_i].m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y,
						m_pNPCObjectArray[_i].m_ModelDescriptor.m_ppModelList[0]->OBB_Center.y * 2.0f,
						m_pNPCObjectArray[_i].m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius * 2.8f );
					m_pNPCObjectArray[_i].m_ModelDescriptor.RenderShadow();
				}
				
				if( m_pNPCObjectArray[_i].CheckIntersectRay() )
				{				
					if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
					{
						g_pCurrentSelectMaterial = &g_SelectedObjectMaterial;
						
						if( m_pNPCObjectArray[_i].m_Distance < g_LastPickedObjectDistance )
						{
							g_LastPickedObjectDistance = m_pNPCObjectArray[_i].m_Distance;
							g_LastPickedObjectType = _XPICKINGTARGET_NPC;
							g_LastPickedObject = (_XItem*)&m_pNPCObjectArray[_i];		
							
							XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
							if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
							{
								CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
								eodapp->ReadyCursor( _XMOUSETYPE_CONVERSATION );
							}
						}
					}
					m_pNPCObjectArray[_i].Render();				
					g_pCurrentSelectMaterial = NULL;
				}
				else
				{				
					m_pNPCObjectArray[_i].Render();				
				}			
			}
		}
	}
	*/
}


void _XNPCObjectManager::RenderNameToolTip( void )
{
//	D3DXVECTOR3 mpos;
//	D3DXVECTOR3 rvec;	
//	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );
//	for( int _i = 0; _i < m_NPCObjectCount; _i++ )
//	{	
//		if( m_pNPCObjectArray[_i].m_InsideFrustum ) //(_XDEF_NPC_CULLINGRADIUS*2.0f) )
//		{									
//			if( m_pNPCObjectArray[_i].m_Distance < 45.0f )
//			{
//				mpos.x = g_Npclist.m_pNPCObjectArray[_i].m_ModelDescriptor.m_Position._41;
//				mpos.y = g_Npclist.m_pNPCObjectArray[_i].m_ModelDescriptor.m_Position._42 
//					+ (g_Npclist.m_pNPCObjectArray[_i].m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2);
//				mpos.z = g_Npclist.m_pNPCObjectArray[_i].m_ModelDescriptor.m_Position._43;
//				
//				D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
//								 &g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
//				
//				g_NPCNameWindow.Draw( rvec.x, rvec.y-36.0f, g_Npclist.m_pNPCObjectArray[_i].m_NPCRoll, g_Npclist.m_pNPCObjectArray[_i].m_NPCName, 
//					FALSE, g_Npclist.m_pNPCObjectArray[_i].m_HaveQuestType, g_Npclist.m_pNPCObjectArray[_i].m_NPCJobClass );
//			}
//		}
//	}

}

void _XNPCObjectManager::RenderNPCChatBalloon( int index )
{
	
//	gpDev->SetRenderState(D3DRS_FOGENABLE, FALSE );	
//	
//	if( g_Npclist.m_pNPCObjectArray[index].m_InsideFrustum )
//	{
//		if( g_Npclist.m_pNPCObjectArray[index].m_Distance < 15.0f)
//		{
//			
//			_XNPCMainScript* mainscript = (_XNPCMainScript*)g_Npclist.m_pNPCObjectArray[index].m_Triggerlist.getItem(0);
//			if(mainscript) 
//			{			
//				if( g_Npclist.m_pNPCObjectArray[index].m_HaveQuestType == _XNPC_HAVEQUEST_NONE )
//				{
//					int m_SelectedMessageIndex;
//					int count = 0;
///*
//					for(int i = 0 ; i < 5; i++)
//					{															
//						if( mainscript->strPrevGreetingMessage[i] != NULL )
//							count++;
//					}						
//					m_SelectedMessageIndex = _XRandom() % count;
//					m_SelectedMessageIndex++;
//					if(m_SelectedMessageIndex >= count)
//*/
//						m_SelectedMessageIndex = 0;
//
//					if(mainscript->strPrevGreetingMessage[m_SelectedMessageIndex] != NULL)
//					{						
//						D3DXVECTOR3 mpos;
//						D3DXVECTOR3 rvec;
//						
//						mpos.x = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._41;
//						mpos.y = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._42 
//								 + (g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2);
//						mpos.z = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._43;
//						
//						D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
//							&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
//						
//
//						// 2004.06.20->oneway48 insert
//						//g_Npclist.m_pNPCObjectArray[index].m_NPCChatBalloonObject.SetNPCNameString( g_Npclist.m_pNPCObjectArray[index].m_NPCName, g_Npclist.m_pNPCObjectArray[index].m_NPCRoll );
//						g_Npclist.m_pNPCObjectArray[index].m_NPCChatBalloonObject.Draw(rvec.x , rvec.y-28.0f );
//						g_Npclist.m_pNPCObjectArray[index].m_NPCChatBalloonObject.SetMessage(mainscript->strPrevGreetingMessage[m_SelectedMessageIndex]);
//					}
//					else	// 인사말전 대사의 내용이 없으면 이름창을 그린다.
//					{
//						
//						D3DXVECTOR3 mpos;
//						D3DXVECTOR3 rvec;	
//						
//						mpos.x = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._41;
//						mpos.y = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._42 
//							+ (g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2) + 0.5f;
//						mpos.z = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._43;
//						
//						D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
//							&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
//						
//						g_NPCNameWindow.Draw( rvec.x, rvec.y, g_Npclist.m_pNPCObjectArray[index].m_NPCRoll,	
//							g_Npclist.m_pNPCObjectArray[index].m_NPCName, FALSE, g_Npclist.m_pNPCObjectArray[index].m_HaveQuestType,
//							g_Npclist.m_pNPCObjectArray[index].m_NPCJobClass );
//					}
//				}
//				else
//				{
//					
//					D3DXVECTOR3 mpos;
//					D3DXVECTOR3 rvec;	
//					
//					mpos.x = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._41;
//					mpos.y = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._42 
//						+ (g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2) + 0.5f;
//					mpos.z = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._43;
//					
//					D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
//						&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
//					
//
//					g_NPCNameWindow.Draw( rvec.x, rvec.y, g_Npclist.m_pNPCObjectArray[index].m_NPCRoll, 
//						g_Npclist.m_pNPCObjectArray[index].m_NPCName, FALSE, g_Npclist.m_pNPCObjectArray[index].m_HaveQuestType,
//						g_Npclist.m_pNPCObjectArray[index].m_NPCJobClass );
//				}
//			}
//		}
//		else if( g_Npclist.m_pNPCObjectArray[index].m_Distance < 45.0f)
//		{
//			D3DXVECTOR3 mpos;
//			D3DXVECTOR3 rvec;
//			
//			mpos.x = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._41;
//			mpos.y = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._42 
//				+ (g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1] * 2) + 0.5f;
//			mpos.z = g_Npclist.m_pNPCObjectArray[index].m_ModelDescriptor.m_Position._43;
//			
//			D3DXVec3Project( &rvec, &mpos, &g_MainViewPort, &g_LodTerrain.m_3PCamera.GetProjMatrix(), 
//				&g_LodTerrain.m_3PCamera.GetViewMatrix(), NULL);
//			
//			g_NPCNameWindow.Draw( rvec.x, rvec.y, g_Npclist.m_pNPCObjectArray[index].m_NPCRoll, 
//				g_Npclist.m_pNPCObjectArray[index].m_NPCName, FALSE,  g_Npclist.m_pNPCObjectArray[index].m_HaveQuestType,
//				g_Npclist.m_pNPCObjectArray[index].m_NPCJobClass );
//		}			
//	}		
//	
//	gpDev->SetRenderState(D3DRS_FOGENABLE, g_EnvironmentManager.m_Fog );

}

void _XNPCObjectManager::RemoveEffectInstance(int templateid)
{
	for(int i = 0 ; i < m_NPCObjectCount ; ++i)
	{
		if(g_Npclist.m_pNPCObjectArray[i].m_ModelDescriptor.m_EffectManager)
			g_Npclist.m_pNPCObjectArray[i].m_ModelDescriptor.m_EffectManager->DeleteInstance(templateid);
	}
}

int _XNPCObjectManager::GetNPCIndexByNPCJob(_XNPCJOBCLASS job)
{
	for(int i = 0 ; i < m_NPCObjectCount ; ++i)
	{
		// job이 unique 할때만 valid
		if(m_pNPCObjectArray[i].m_NPCJobClass == job)
		{
			return i;
		}
	}
	return 0;
}
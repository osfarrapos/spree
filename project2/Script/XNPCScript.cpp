// XNPCScript.cpp: implementation of the _XNPCScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XNPCScript.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XNPC.h"

#include "XNPCScriptConditionItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XNPCScriptItem::_XNPCScriptItem()
{
	enable			= TRUE;

	scriptclass		= -1;
	compareclass	= 1;
	
	condition1		= -1;
	condition2		= -1;
	condition3		= -1;	
	condition4		= -1;
	condition5		= -1;
	condition6		= -1;		 
	condition7		= -1;
	condition8		= -1;
	condition9		= -1;		 
	condition10		= -1;		 
	condition11		= -1;
	condition12		= -1;	//2004.11.17->oneway48 insert : 스킬 리스트가 12개로 늘어남
	condition13		= -1;	//2005.01.07->oneway48 insert : 스킬 리스트가 13개로 늘어남

	quantitynumber1	= -1;
	quantitynumber2	= -1;
	quantitynumber3	= -1;
	quantitynumber4	= -1;
	quantitynumber5	= -1;
	quantitystring  = NULL;

	messagestring	= NULL;
	
	for(int i = 0; i < 5; ++i)
	{
		strMessage1[i] = NULL;	
		strMessage2[i] = NULL;	
		strMessage3[i] = NULL;	
	}
}

_XNPCScriptItem::~_XNPCScriptItem()
{	
	if( quantitystring )
	{
		delete[] quantitystring;
		quantitystring = NULL;
	}
	if( messagestring )
	{
		delete[] messagestring;
		messagestring = NULL;
	}
	
	for(int i = 0; i < 5; ++i)
	{		
		if(strMessage1[i])
		{
			delete[] strMessage1[i];
			strMessage1[i] = NULL;
		}
		if(strMessage2[i])
		{
			delete[] strMessage2[i];
			strMessage2[i] = NULL;
		}
		if(strMessage3[i])
		{
			delete[] strMessage3[i];
			strMessage3[i] = NULL;
		}
	}
}

void _XNPCScriptItem::SetMessageString( LPSTR _message )
{
	if( messagestring )
	{
		delete[] messagestring;
		messagestring = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );

		messagestring = new TCHAR[ len+1 ];
		memset( messagestring, 0, len+1 );

		strcpy( messagestring, _message );
	}
}

void _XNPCScriptItem::SetQuantityString( LPSTR _quantitystring )
{
	if( quantitystring )
	{
		delete[] quantitystring;
		quantitystring = NULL;
	}
	
	if( _quantitystring )
	{
		int len = strlen( _quantitystring );

		quantitystring = new TCHAR[ len+1 ];
		memset( quantitystring, 0, len+1 );

		strcpy( quantitystring, _quantitystring );
	}
}

void _XNPCScriptItem::SetstrMessage1(int index, LPSTR _message)
{
	if( strMessage1[index] )
	{
		delete[] strMessage1[index];
		strMessage1[index] = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );
		
		strMessage1[index] = new char[ len+1 ];
		memset( strMessage1[index], 0, len+1 );
		
		strcpy( strMessage1[index], _message );
	}
}

void _XNPCScriptItem::SetstrMessage2(int index, LPSTR _message)
{
	if( strMessage2[index] )
	{
		delete[] strMessage2[index];
		strMessage2[index] = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );
		
		strMessage2[index] = new char[ len+1 ];
		memset( strMessage2[index], 0, len+1 );
		
		strcpy( strMessage2[index], _message );
	}
}

void _XNPCScriptItem::SetstrMessage3(int index, LPSTR _message)
{
	if( strMessage3[index] )
	{
		delete[] strMessage3[index];
		strMessage3[index] = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );
		
		strMessage3[index] = new char[ len+1 ];
		memset( strMessage3[index], 0, len+1 );
		
		strcpy( strMessage3[index], _message );
	}
}

_XNPCCONDITION_RESULT _XNPCScriptItem::ProcessCondition()
{
	
	int subscriptconditioncount = m_SubScriptConditionList.getitemcount();
	bool checkgeneral = FALSE;
	bool checkhostility = FALSE;
	

	switch(compareclass)
	{
	case ID_NPCSUBSCRIPT_CONDITIONFUNCTION_OR :
		{
			int failcount=0;
			if(subscriptconditioncount > 0)
			{
				m_SubScriptConditionList.resetList();
				_XNPCScriptConditionItem* conditionitem = NULL;
				do 
				{
					conditionitem = (_XNPCScriptConditionItem *)m_SubScriptConditionList.currentItem();

					if(conditionitem)
					{
						if(conditionitem->Process() == NPC_CONDITION_RESULT_FAIL) failcount++;
					}
					
					m_SubScriptConditionList.nextItem();
				} while( !m_SubScriptConditionList.atHeadOfList());
			}
			
			if(failcount == subscriptconditioncount)
				return NPC_CONDITION_RESULT_FAIL;
			else
				return NPC_CONDITION_RESULT_SUCCESS;

		}
		break;
	case ID_NPCSUBSCRIPT_CONDITIONFUNCTION_AND :
		{
			int failcount=0;
			if(subscriptconditioncount > 0)
			{
				m_SubScriptConditionList.resetList();
				_XNPCScriptConditionItem* conditionitem = NULL;
				do 
				{
					conditionitem = (_XNPCScriptConditionItem *)m_SubScriptConditionList.currentItem();
					
					if(conditionitem)
						if(conditionitem->Process() == NPC_CONDITION_RESULT_FAIL) failcount++;
						
					m_SubScriptConditionList.nextItem();
				} while( !m_SubScriptConditionList.atHeadOfList() );
			}
			
			if(failcount != 0 )
				return NPC_CONDITION_RESULT_FAIL;
			else
				return NPC_CONDITION_RESULT_SUCCESS;
		}
		break;
	}
				
	return NPC_CONDITION_RESULT_FAIL;

}

// _XNPCScriptItem 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// _XNPCMainScript

_XNPCMainScript::_XNPCMainScript()
{
	memset(scriptname, 0, 32);
	strcpy( scriptname, "Noname" );

	messagestring1 = NULL;
	messagestring2 = NULL;
	messagestring3 = NULL;
	messagestring4 = NULL;
	messagestring5 = NULL;

	soundindex1 = 0;
	soundindex2 = 0;
	soundindex3 = 0;
	soundindex4 = 0;
	soundindex5 = 0;
	
	for(int i = 0; i < 5; ++i)
	{
		strPrevGreetingMessage[i] = NULL;		
		strGreetingGeneralMessage[i] = NULL;	
		strGreetingHostilityMessage[i] = NULL;	
		strGreetingCustomerMessage[i] = NULL;	
		strGreetingHeroStayMessage[i] = NULL;	
		
		indexPrevGreetingSound[i] = 0;		
		indexGreetingGeneralSound[i] = 0;		
		indexGreetingHostilitySound[i] = 0;		
		indexGreetingCustomerSound[i] = 0;		
		indexGreetingHeroStaySound[i] = 0;		
	}

	//2004.07.05->oneway48 insert
	strGoodbyeGeneralMessage = NULL;	// 일반일 때 헤어질 때 대사 저장
	strGoodbyeCustomerMessage = NULL;	// 적대일 때 헤어질 때 대사 저장
	strGoodbyeHeroStayMessage = NULL;	// 단골일 때 헤어질 때 대사 저장
	strGoodbyeHostilityMessage = NULL;	// 영웅이 있을 때 헤어질 때 대사 저장
	
	for(int j = 0; j < 4; ++j)
	{
		indexGoodbyeSound[j] = -1;
	}

	m_EventType = -1;

	m_UniqueID  = -1;
}

_XNPCMainScript::~_XNPCMainScript()
{
	m_SubScriptList.disposeList();

	if( messagestring1 )
	{
		delete[] messagestring1;
		messagestring1 = NULL;
	}
	if( messagestring2 )
	{
		delete[] messagestring2;
		messagestring2 = NULL;
	}
	if( messagestring3 )
	{
		delete[] messagestring3;
		messagestring3 = NULL;
	}
	if( messagestring4 )
	{
		delete[] messagestring4;
		messagestring4 = NULL;
	}
	if( messagestring5 )
	{
		delete[] messagestring5;
		messagestring5 = NULL;
	}

	for(int i = 0 ; i < 5 ; ++i)
	{	
		if(strPrevGreetingMessage[i])
		{
			delete[] strPrevGreetingMessage[i];
			strPrevGreetingMessage[i] = NULL;
		}
		if(strGreetingGeneralMessage[i])
		{
			delete[] strGreetingGeneralMessage[i];
			strGreetingGeneralMessage[i] = NULL;
		}
		if(strGreetingHostilityMessage[i])
		{
			delete[] strGreetingHostilityMessage[i];
			strGreetingHostilityMessage[i] = NULL;
		}
		if(strGreetingCustomerMessage[i])
		{
			delete[] strGreetingCustomerMessage[i];
			strGreetingCustomerMessage[i] = NULL;
		}
		if(strGreetingHeroStayMessage[i])
		{
			delete[] strGreetingHeroStayMessage[i];
			strGreetingHeroStayMessage[i] = NULL;
		}
	}
	
	//2004.07.05->oneway48 insert
	if( strGoodbyeGeneralMessage )
	{
		delete[] strGoodbyeGeneralMessage;
		strGoodbyeGeneralMessage = NULL;
	}
	if( strGoodbyeCustomerMessage )
	{
		delete[] strGoodbyeCustomerMessage;
		strGoodbyeCustomerMessage = NULL;
	}
	if( strGoodbyeHeroStayMessage )
	{
		delete[] strGoodbyeHeroStayMessage;
		strGoodbyeHeroStayMessage = NULL;
	}
	if( strGoodbyeHostilityMessage )
	{
		delete[] strGoodbyeHostilityMessage;
		strGoodbyeHostilityMessage = NULL;
	}
	
	for(i = 0 ; i < 150 ; ++i)
	{
		if(g_NPCSoundLUT[i])
		{
			SAFE_DELETE(g_NPCSoundLUT[i]);
		}
	}
}

void _XNPCMainScript::SetScriptName( LPSTR _scriptname )
{
	memset(scriptname, 0, 32);
	if( strlen(_scriptname) > 31 )
		strncpy( scriptname, _scriptname, 31 );
	else 
		strcpy( scriptname, _scriptname );
}

void _XNPCMainScript::SetMessageString( LPSTR _message, int index )
{
	switch(index)
	{
	case 1 :
		{
			if( messagestring1 )
			{
				delete[] messagestring1;
				messagestring1 = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				messagestring1 = new TCHAR[ len+1 ];
				memset( messagestring1, 0, len+1 );
				
				strcpy( messagestring1, _message );
			}
		}
	case 2 :
		{
			if( messagestring2 )
			{
				delete[] messagestring2;
				messagestring2 = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				messagestring2 = new TCHAR[ len+1 ];
				memset( messagestring2, 0, len+1 );
				
				strcpy( messagestring2, _message );
			}
		}
	case 3 :
		{
			if( messagestring3 )
			{
				delete[] messagestring3;
				messagestring3 = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				messagestring3 = new TCHAR[ len+1 ];
				memset( messagestring3, 0, len+1 );
				
				strcpy( messagestring3, _message );
			}
		}
	case 4 :
		{
			if( messagestring4 )
			{
				delete[] messagestring4;
				messagestring4 = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				messagestring4 = new TCHAR[ len+1 ];
				memset( messagestring4, 0, len+1 );
				
				strcpy( messagestring4, _message );
			}
		}
		break;
	case 5 :
		{
			if( messagestring5 )
			{
				delete[] messagestring5;
				messagestring5 = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				messagestring5 = new TCHAR[ len+1 ];
				memset( messagestring5, 0, len+1 );
				
				strcpy( messagestring5, _message );
			}
		}
	default :
		break;
	}
}

void _XNPCMainScript::SetSoundIndex(int index, int soundfileindex)
{
	switch(index)
	{
	case 1 :
		soundindex1 = soundfileindex;
		break;
		
	case 2 :
		soundindex2 = soundfileindex;
		break;
		
	case 3 :
		soundindex3 = soundfileindex;
		break;
		
	case 4 :
		soundindex4 = soundfileindex;
		break;
		
	case 5 :
		soundindex5 = soundfileindex;
		break;
	}
}

void _XNPCMainScript::SetPrevGreetingMessage(int index, LPSTR _message)
{
	if( strPrevGreetingMessage[index] )
	{
		delete[] strPrevGreetingMessage[index];
		strPrevGreetingMessage[index] = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );
		
		strPrevGreetingMessage[index] = new char[ len+1 ];
		memset( strPrevGreetingMessage[index], 0, len+1 );
		
		strcpy( strPrevGreetingMessage[index], _message );
	}

}

void _XNPCMainScript::SetGreetingGeneralMessage(int index, LPSTR _message)
{
	if( strGreetingGeneralMessage[index] )
	{
		delete[] strGreetingGeneralMessage[index];
		strGreetingGeneralMessage[index] = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );
		
		strGreetingGeneralMessage[index] = new char[ len+1 ];
		memset( strGreetingGeneralMessage[index], 0, len+1 );
		
		strcpy( strGreetingGeneralMessage[index], _message );
	}
}

void _XNPCMainScript::SetGreetingHostilityMessage(int index, LPSTR _message)
{
	if( strGreetingHostilityMessage[index] )
	{
		delete[] strGreetingHostilityMessage[index];
		strGreetingHostilityMessage[index] = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );
		
		strGreetingHostilityMessage[index] = new char[ len+1 ];
		memset( strGreetingHostilityMessage[index], 0, len+1 );
		
		strcpy( strGreetingHostilityMessage[index], _message );
	}
}

void _XNPCMainScript::SetGreetingCustomerMessage(int index, LPSTR _message)
{
	if( strGreetingCustomerMessage[index] )
	{
		delete[] strGreetingCustomerMessage[index];
		strGreetingCustomerMessage[index] = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );
		
		strGreetingCustomerMessage[index] = new char[ len+1 ];
		memset( strGreetingCustomerMessage[index], 0, len+1 );
		
		strcpy( strGreetingCustomerMessage[index], _message );
	}
}

void _XNPCMainScript::SetGreetingHeroStayMessage(int index, LPSTR _message)
{
	if( strGreetingHeroStayMessage[index] )
	{
		delete[] strGreetingHeroStayMessage[index];
		strGreetingHeroStayMessage[index] = NULL;
	}
	
	if( _message )
	{
		int len = strlen( _message );
		
		strGreetingHeroStayMessage[index] = new char[ len+1 ];
		memset( strGreetingHeroStayMessage[index], 0, len+1 );
		
		strcpy( strGreetingHeroStayMessage[index], _message );
	}
}

void _XNPCMainScript::SetIndexPrevGreetingSound(int index, int soundfileindex)
{
	indexPrevGreetingSound[index] = soundfileindex;
}

void _XNPCMainScript::SetIndexGreetingGeneralSound(int index, int soundfileindex)
{
	indexGreetingGeneralSound[index] = soundfileindex;
}

void _XNPCMainScript::SetIndexGreetingHostilitySound(int index, int soundfileindex)
{
	indexGreetingHostilitySound[index] = soundfileindex;
}

void _XNPCMainScript::SetIndexGreetingCustomerSound(int index, int soundfileindex)
{
	indexGreetingCustomerSound[index] = soundfileindex;
}

void _XNPCMainScript::SetIndexGreetingHeroStaySound(int index, int soundfileindex)
{
	indexGreetingHeroStaySound[index] = soundfileindex;
}


void _XNPCMainScript::insertSubScriptItem( _XNPCScriptItem* scriptitem )
{
	m_SubScriptList.insertItem( (_XItem*)scriptitem );
}

_XNPCScriptItem* _XNPCMainScript::getSubScriptItem( int index )
{
	return (_XNPCScriptItem*)m_SubScriptList.getItem( index );
}

BOOL _XNPCMainScript::LoadScript( FILE* FilePtr )
{
	if( !FilePtr ) return FALSE;

	fread(scriptname, sizeof(TCHAR)*32, 1, FilePtr);	
	
	fread( &compareclass, sizeof(int), 1, FilePtr );
	
	m_NPCScriptConditionList.disposeList();
	
	int npcscriptconditioncount;
	
	if( fread( &npcscriptconditioncount, sizeof(int), 1, FilePtr ) < 1 ) return FALSE;
	
	if(npcscriptconditioncount > 0)
	{
		_XNPCScriptConditionItem* npcconditionitem = NULL;
		for( int i = 0 ; i < npcscriptconditioncount ; ++i)
		{
			npcconditionitem = new _XNPCScriptConditionItem;
			if(npcconditionitem)
			{
				if(!npcconditionitem->Load(FilePtr))
				{
					delete npcconditionitem;
					return FALSE;
				}
				m_NPCScriptConditionList.insertItem(npcconditionitem);
			}

		}
	}

	char tempmessagestring[1024];
	
	// 인사말 전 대사 & 인사말 로드 부분
	for( int i = 0; i < 5; ++i)
	{
		// 인삿말 전 대사
		int messagestringlength = 0;	
		fread( &messagestringlength, sizeof(int), 1, FilePtr );
		memset( tempmessagestring, 0, 1024 );
		if( messagestringlength > 0 )
		{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
			fseek( FilePtr, messagestringlength, SEEK_CUR );
			int resourceid = (m_UniqueID * 1000) + 10 + i + 1;			
			SetPrevGreetingMessage( i, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else
			fread( tempmessagestring, messagestringlength, 1, FilePtr );
			SetPrevGreetingMessage( i, tempmessagestring );
#endif
			
		}
		
		// 인삿말 일반
		messagestringlength = 0;	
		fread( &messagestringlength, sizeof(int), 1, FilePtr );
		memset( tempmessagestring, 0, 1024 );
		if( messagestringlength > 0 )
		{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
			fseek( FilePtr, messagestringlength, SEEK_CUR );
			int resourceid = (m_UniqueID * 1000) + 10 + i + 6;
			SetGreetingGeneralMessage( i, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else
			fread( tempmessagestring, messagestringlength, 1, FilePtr );
			SetGreetingGeneralMessage( i, tempmessagestring );
#endif
		}
		
		// 인삿말 적대
		messagestringlength = 0;	
		fread( &messagestringlength, sizeof(int), 1, FilePtr );
		memset( tempmessagestring, 0, 1024 );
		if( messagestringlength > 0 )
		{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
			fseek( FilePtr, messagestringlength, SEEK_CUR );
			int resourceid = (m_UniqueID * 1000) + 10 + i + 11;
			SetGreetingHostilityMessage( i, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else
			fread( tempmessagestring, messagestringlength, 1, FilePtr );
			SetGreetingHostilityMessage( i, tempmessagestring );
#endif
		}
		
		// 인삿말 단골
		messagestringlength = 0;	
		fread( &messagestringlength, sizeof(int), 1, FilePtr );
		memset( tempmessagestring, 0, 1024 );
		if( messagestringlength > 0 )
		{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
			fseek( FilePtr, messagestringlength, SEEK_CUR );
			int resourceid = (m_UniqueID * 1000) + 10 + i + 16;
			SetGreetingCustomerMessage( i, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else
			fread( tempmessagestring, messagestringlength, 1, FilePtr );
			SetGreetingCustomerMessage( i, tempmessagestring );
#endif
		}
		
		// 인삿말 영웅 있을 시
		messagestringlength = 0;	
		fread( &messagestringlength, sizeof(int), 1, FilePtr );
		memset( tempmessagestring, 0, 1024 );
		if( messagestringlength > 0 )
		{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
			fseek( FilePtr, messagestringlength, SEEK_CUR );
			int resourceid = (m_UniqueID * 1000) + 10 + i + 21;
			SetGreetingHeroStayMessage( i, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else
			fread( tempmessagestring, messagestringlength, 1, FilePtr );
			SetGreetingHeroStayMessage( i, tempmessagestring );
#endif
		}
		
		// 대사별 사운드 로드 
		fread(&indexPrevGreetingSound[i], sizeof(int), 1, FilePtr);			// 인사말 전 대사가 가지는 사운드 인덱스
		fread(&indexGreetingGeneralSound[i], sizeof(int), 1, FilePtr);		// 인사말 일반이 가지는 사운드 인덱스
		fread(&indexGreetingHostilitySound[i], sizeof(int), 1, FilePtr);	// 인사말 적대가 가지는 사운드 인덱스
		fread(&indexGreetingCustomerSound[i], sizeof(int), 1, FilePtr);		// 인사말 단골이 가지는 사운드 인덱스
		fread(&indexGreetingHeroStaySound[i], sizeof(int), 1, FilePtr);		// 인사말 영웅 있을 시 가지는 사운드 인덱스

		// 인사말 전 대사가 가지는 사운드
		if( indexPrevGreetingSound[i] > -1 )
		{
			if(g_NPCSoundUsedTable[indexPrevGreetingSound[i]] == 0 && indexPrevGreetingSound[i] > 0)
			{
				// 새로 들어갈 것
				if(g_NPCSoundLUT[indexPrevGreetingSound[i]])
				{
					SAFE_DELETE(g_NPCSoundLUT[indexPrevGreetingSound[i]]);
				}
				g_NPCSoundLUT[indexPrevGreetingSound[i]] = new TCHAR[256];
				memset( g_NPCSoundLUT[indexPrevGreetingSound[i]], 0, sizeof(TCHAR) * 256 );
				strcat(g_NPCSoundLUT[indexPrevGreetingSound[i]], g_NPCSoundAllTable[indexPrevGreetingSound[i]]);
				g_NPCSoundUsedTable[indexPrevGreetingSound[i]] = 1;
				g_NPCSoundCount++;
			}

			// 인사말 일반이 가지는 사운드
			if(g_NPCSoundUsedTable[indexGreetingGeneralSound[i]] == 0 && indexGreetingGeneralSound[i] > 0)
			{
				// 새로 들어갈 것
				if(g_NPCSoundLUT[indexGreetingGeneralSound[i]])
				{
					SAFE_DELETE(g_NPCSoundLUT[indexGreetingGeneralSound[i]]);
				}
				g_NPCSoundLUT[indexGreetingGeneralSound[i]] = new TCHAR[256];
				memset( g_NPCSoundLUT[indexGreetingGeneralSound[i]], 0, sizeof(TCHAR) * 256 );
				strcat(g_NPCSoundLUT[indexGreetingGeneralSound[i]], g_NPCSoundAllTable[indexGreetingGeneralSound[i]]);
				g_NPCSoundUsedTable[indexGreetingGeneralSound[i]] = 1;
				g_NPCSoundCount++;
			}

			// 인사말 적대가 가지는 사운드
			if(g_NPCSoundUsedTable[indexGreetingHostilitySound[i]] == 0 && indexGreetingHostilitySound[i] > 0)
			{
				// 새로 들어갈 것
				if(g_NPCSoundLUT[indexGreetingHostilitySound[i]])
				{
					SAFE_DELETE(g_NPCSoundLUT[indexGreetingHostilitySound[i]]);
				}
				g_NPCSoundLUT[indexGreetingHostilitySound[i]] = new TCHAR[256];
				memset( g_NPCSoundLUT[indexGreetingHostilitySound[i]], 0, sizeof(TCHAR) * 256 );
				strcat(g_NPCSoundLUT[indexGreetingHostilitySound[i]], g_NPCSoundAllTable[indexGreetingHostilitySound[i]]);
				g_NPCSoundUsedTable[indexGreetingHostilitySound[i]] = 1;
				g_NPCSoundCount++;
			}

			// 인사말 단골이 가지는 사운드
			if(g_NPCSoundUsedTable[indexGreetingCustomerSound[i]] == 0 && indexGreetingCustomerSound[i] > 0)
			{
				// 새로 들어갈 것
				if(g_NPCSoundLUT[indexGreetingCustomerSound[i]])
				{
					SAFE_DELETE(g_NPCSoundLUT[indexGreetingCustomerSound[i]]);
				}
				g_NPCSoundLUT[indexGreetingCustomerSound[i]] = new TCHAR[256];
				memset( g_NPCSoundLUT[indexGreetingCustomerSound[i]], 0, sizeof(TCHAR) * 256 );
				strcat(g_NPCSoundLUT[indexGreetingCustomerSound[i]], g_NPCSoundAllTable[indexGreetingCustomerSound[i]]);
				g_NPCSoundUsedTable[indexGreetingCustomerSound[i]] = 1;
				g_NPCSoundCount++;
			}

			// 인사말 영웅 있을 시 가지는 사운드
			if(g_NPCSoundUsedTable[indexGreetingHeroStaySound[i]] == 0 && indexGreetingHeroStaySound[i] > 0)
			{
				// 새로 들어갈 것
				if(g_NPCSoundLUT[indexGreetingHeroStaySound[i]])
				{
					SAFE_DELETE(g_NPCSoundLUT[indexGreetingHeroStaySound[i]]);
				}
				g_NPCSoundLUT[indexGreetingHeroStaySound[i]] = new TCHAR[256];
				memset( g_NPCSoundLUT[indexGreetingHeroStaySound[i]], 0, sizeof(TCHAR) * 256 );
				strcat(g_NPCSoundLUT[indexGreetingHeroStaySound[i]], g_NPCSoundAllTable[indexGreetingHeroStaySound[i]]);
				g_NPCSoundUsedTable[indexGreetingHeroStaySound[i]] = 1;
				g_NPCSoundCount++;
			}
		}
	}// for end

	//2004.07.05->oneway48 insert
	////////////////////////////// 헤어질 때 대사 로드 ///////////////////////////////////
	// 헤어질 때 0:일반 1:적대 2:단골 3:영웅 있을 때 
	for( i = 0; i < 4; ++i )
	{
		int messagestringlength = 0;	
		fread( &messagestringlength, sizeof(int), 1, FilePtr );
		memset( tempmessagestring, 0, 1024 );
		if( messagestringlength > 0 )
		{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
			fseek( FilePtr, messagestringlength, SEEK_CUR );
			int resourceid = (m_UniqueID * 1000) + 10 + 5 + i + 26;
			SetGoodByeMessage( i, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else
			fread( tempmessagestring, messagestringlength, 1, FilePtr );
			SetGoodByeMessage( i, tempmessagestring );
#endif
		}		
	}

	for( i = 0; i < 4; ++i )
	{
		fread(&indexGoodbyeSound[i], sizeof(int), 1, FilePtr);
		
		// 인사말 영웅 있을 시 가지는 사운드
		if( indexGoodbyeSound[i] > -1 )
		{
			if(g_NPCSoundUsedTable[indexGoodbyeSound[i]] == 0 && indexGoodbyeSound[i] > 0)
			{
				// 새로 들어갈 것
				if(g_NPCSoundLUT[indexGoodbyeSound[i]])
				{
					SAFE_DELETE(g_NPCSoundLUT[indexGoodbyeSound[i]]);
				}
				g_NPCSoundLUT[indexGoodbyeSound[i]] = new TCHAR[256];
				memset( g_NPCSoundLUT[indexGoodbyeSound[i]], 0, sizeof(TCHAR) * 256 );
				strcat(g_NPCSoundLUT[indexGoodbyeSound[i]], g_NPCSoundAllTable[indexGoodbyeSound[i]]);
				g_NPCSoundUsedTable[indexGoodbyeSound[i]] = 1;
				g_NPCSoundCount++;
			}
		}
	}
	// insert end


	m_SubScriptList.disposeList();

	int subscriptcount;
	if( fread( &subscriptcount, sizeof(int), 1, FilePtr ) < 1 )
	{
		_XFatalError( "_XNPCMainScript::LoadScript : load sub script count" );
		return FALSE;
	}
	
	for(  i = 0; i < subscriptcount; ++i )
	{
		_XNPCScriptItem* subitem = new _XNPCScriptItem;

		if( subitem )
		{
			fread( &subitem->enable, sizeof(BOOL), 1, FilePtr );
			fread( &subitem->scriptclass, sizeof(int), 1, FilePtr );
			fread( &subitem->compareclass, sizeof(int), 1, FilePtr );
			fread( &subitem->condition1, sizeof(int), 1, FilePtr );
			fread( &subitem->condition2, sizeof(int), 1, FilePtr );
			fread( &subitem->condition3, sizeof(int), 1, FilePtr );
			fread( &subitem->condition4, sizeof(int), 1, FilePtr );
			fread( &subitem->condition5, sizeof(int), 1, FilePtr );
			fread( &subitem->condition6, sizeof(int), 1, FilePtr );	
			fread( &subitem->condition7, sizeof(int), 1, FilePtr );
			fread( &subitem->condition8, sizeof(int), 1, FilePtr );
			fread( &subitem->condition9, sizeof(int), 1, FilePtr );	
			fread( &subitem->condition10, sizeof(int), 1, FilePtr );
			fread( &subitem->condition11, sizeof(int), 1, FilePtr );
			fread( &subitem->condition12, sizeof(int), 1, FilePtr );	//2004.11.17->oneway48 insert : 스킬 리스트가 12개로 늘어남 
			fread( &subitem->condition13, sizeof(int), 1, FilePtr );	//2004.11.17->oneway48 insert : 스킬 리스트가 12개로 늘어남 
			fread( &subitem->quantitynumber1, sizeof(int), 1, FilePtr );
			fread( &subitem->quantitynumber2, sizeof(int), 1, FilePtr );
			fread( &subitem->quantitynumber3, sizeof(int), 1, FilePtr );
			fread( &subitem->quantitynumber4, sizeof(int), 1, FilePtr );
			fread( &subitem->quantitynumber5, sizeof(int), 1, FilePtr );

			int quantitystringlength = 0;			
			fread( &quantitystringlength, sizeof(int), 1, FilePtr );

			if( quantitystringlength > 0 )
			{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
				fseek( FilePtr, quantitystringlength, SEEK_CUR );
				int resourceid = (m_UniqueID * 1000) + 52;
				subitem->SetQuantityString( g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else				
				memset( tempmessagestring, 0, 1024 );
				fread( tempmessagestring, quantitystringlength, 1, FilePtr );
				subitem->SetQuantityString( tempmessagestring );
#endif
			}

			int messagelength = 0;			
			fread( &messagelength, sizeof(int), 1, FilePtr );

			if( messagelength > 0 )
			{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
				fseek( FilePtr, messagelength, SEEK_CUR );
				int resourceid = (m_UniqueID * 1000) + 51;
				subitem->SetMessageString( g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else				
				memset( tempmessagestring, 0, 1024 );
				fread( tempmessagestring, messagelength, 1, FilePtr );
				subitem->SetMessageString( tempmessagestring );
#endif
			}

			// 대화-일반,단골,영웅 or 질문&답변-질문,답변 로드 
			for( int j = 0; j < 5; ++j)
			{
				int messagestringlength = 0;	
				fread( &messagestringlength, sizeof(int), 1, FilePtr );
				
				if( messagestringlength > 0 )
				{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
					fseek( FilePtr, messagestringlength, SEEK_CUR );
					int resourceid = (m_UniqueID * 1000) + 53 + j;
					subitem->SetstrMessage1( j, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else				
					memset( tempmessagestring, 0, 1024 ); // 2004.07.08->oneway48 insert
					fread( tempmessagestring, messagestringlength, 1, FilePtr );
					subitem->SetstrMessage1( j, tempmessagestring );
#endif
				}
				
				messagestringlength = 0;	
				fread( &messagestringlength, sizeof(int), 1, FilePtr );
				
				if( messagestringlength > 0 )
				{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
					fseek( FilePtr, messagestringlength, SEEK_CUR );
					int resourceid = (m_UniqueID * 1000) + 58 + j;
					subitem->SetstrMessage2( j, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else				
					memset( tempmessagestring, 0, 1024 );
					fread( tempmessagestring, messagestringlength, 1, FilePtr );					
					subitem->SetstrMessage2( j, tempmessagestring );
#endif
				}
				
				messagestringlength = 0;	
				fread( &messagestringlength, sizeof(int), 1, FilePtr );
				
				if( messagestringlength > 0 )
				{
#ifdef _XDEF_USENEWNPCTEXTSYSTEM
					fseek( FilePtr, messagestringlength, SEEK_CUR );
					int resourceid = (m_UniqueID * 1000) + 63 + j;
					subitem->SetstrMessage3( j, g_StringDataBase.GetTextString( _XSTRINGDB_SN_NPCDIALOG, resourceid ) );
#else				
					memset( tempmessagestring, 0, 1024 );
					fread( tempmessagestring, messagestringlength, 1, FilePtr );
					subitem->SetstrMessage3( j, tempmessagestring );
#endif
				}
			}
			
			//조건 리스트 로드부분 추가
			subitem->m_SubScriptConditionList.disposeList();
			
			int subscriptconditioncount;
			if( fread( &subscriptconditioncount, sizeof(int), 1, FilePtr ) < 1 )
			{
				return FALSE;
			}
			
			if(subscriptconditioncount > 0)
			{
				_XNPCScriptConditionItem* conditionitem = NULL;
				for( int k = 0 ; k < subscriptconditioncount ; ++k)
				{
					conditionitem = new _XNPCScriptConditionItem;
					if(conditionitem)
					{
						if(!conditionitem->Load(FilePtr))
						{
							delete conditionitem;
							return FALSE;
						}
						subitem->m_SubScriptConditionList.insertItem(conditionitem);
					}
				}
			}
			
			// 이벤트 종류를 저장한다.
			if( subitem->scriptclass == _XDEF_NPCSUBSCRIPTCLASS_MINISTOPEVENT )
			{
				m_EventType = subitem->condition1; //  0:무영검-패밀리마트 1:봉인유저 2:퀴즈맞추기
			}

			m_SubScriptList.insertItem( subitem );
		}
		else
		{
			return FALSE;
		}		
	}

	return TRUE;
}



_XNPCCONDITION_RESULT _XNPCMainScript::ProcessNPCCondition()
{
	
	int npcscriptconditioncount = m_NPCScriptConditionList.getitemcount();
	bool checkgeneral = FALSE;
	bool checkhostility = FALSE;
	

	switch(compareclass)
	{
	case ID_NPCSUBSCRIPT_CONDITIONFUNCTION_OR :
		{
			int failcount=0;
			if(npcscriptconditioncount > 0)
			{
				m_NPCScriptConditionList.resetList();
				_XNPCScriptConditionItem* conditionitem = NULL;
				do 
				{
					conditionitem = (_XNPCScriptConditionItem *)m_NPCScriptConditionList.currentItem();
					
					if(conditionitem)
						if(conditionitem->Process() == NPC_CONDITION_RESULT_FAIL) failcount++;

					m_NPCScriptConditionList.nextItem();
				} while( !m_NPCScriptConditionList.atHeadOfList());
			}
			
			if(failcount == npcscriptconditioncount)
				return NPC_CONDITION_RESULT_FAIL;
			else
				return NPC_CONDITION_RESULT_SUCCESS;

		}
		break;
	case ID_NPCSUBSCRIPT_CONDITIONFUNCTION_AND :
		{
			int failcount=0;
			if(npcscriptconditioncount > 0)
			{
				m_NPCScriptConditionList.resetList();
				_XNPCScriptConditionItem* conditionitem = NULL;
				do 
				{
					conditionitem = (_XNPCScriptConditionItem *)m_NPCScriptConditionList.currentItem();
					
					if(conditionitem)
						if(conditionitem->Process() == NPC_CONDITION_RESULT_FAIL) failcount++;
										
					m_NPCScriptConditionList.nextItem();
				} while(!m_NPCScriptConditionList.atHeadOfList());
			}
			
			if(failcount != 0 )
				return NPC_CONDITION_RESULT_FAIL;
			else
				return NPC_CONDITION_RESULT_SUCCESS;
			
		}
		break;
	}
				
	return NPC_CONDITION_RESULT_FAIL;
}



//2004.07.05->oneway48 insert
void _XNPCMainScript::SetGoodByeMessage(int index, LPSTR _message)
{
	switch(index) {
	case 0: // 일반
		{
			if( strGoodbyeGeneralMessage )
			{
				delete[] strGoodbyeGeneralMessage;
				strGoodbyeGeneralMessage = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				strGoodbyeGeneralMessage = new char[ len+1 ];
				memset( strGoodbyeGeneralMessage, 0, len+1 );
				
				strcpy( strGoodbyeGeneralMessage, _message );
			}
		}
		break;
	case 1: // 적대 
		{
			if( strGoodbyeHostilityMessage )
			{
				delete[] strGoodbyeHostilityMessage;
				strGoodbyeHostilityMessage = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				strGoodbyeHostilityMessage = new char[ len+1 ];
				memset( strGoodbyeHostilityMessage, 0, len+1 );
				
				strcpy( strGoodbyeHostilityMessage, _message );
			}
		}
		break;
	case 2: // 단골
		{
			if( strGoodbyeCustomerMessage )
			{
				delete[] strGoodbyeCustomerMessage;
				strGoodbyeCustomerMessage = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				strGoodbyeCustomerMessage = new char[ len+1 ];
				memset( strGoodbyeCustomerMessage, 0, len+1 );
				
				strcpy( strGoodbyeCustomerMessage, _message );
			}
		}
		break;
	case 3: // 영웅 있을 시
		{
			if( strGoodbyeHeroStayMessage )
			{
				delete[] strGoodbyeHeroStayMessage;
				strGoodbyeHeroStayMessage = NULL;
			}
			
			if( _message )
			{
				int len = strlen( _message );
				
				strGoodbyeHeroStayMessage = new char[ len+1 ];
				memset( strGoodbyeHeroStayMessage, 0, len+1 );
				
				strcpy( strGoodbyeHeroStayMessage, _message );
			}
		}
		break;
	}
	
}

void _XNPCMainScript::SetIndexGoodByeSound(int index, int soundfileindex)
{	
	indexGoodbyeSound[index] = soundfileindex;
}

// XFunctionalObject.cpp: implementation of the _XFunctionalObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XFunctionalObject.h"
#include "XFunctionalObjectTriggerItem.h"
#include "XParticleidDefine.h"
#include "XUser.h"

#include "XSR_STRINGHEADER.h"

extern _XMobMotionExistTable	g_MobMotionExistTable[_XDEF_MODELCLASSCOUNT_MOB];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFunctionalObject::_XFunctionalObject()
{
	m_ObjectName		= NULL;
	m_UniqueID			= -1;
	m_ObjectType		= -1;
	m_ObjectClass		= -1;
	m_ActionIndex		= _XMOBACTION_NONE;
	
	m_bWideViewMode		= FALSE;
	m_bReLocateMode		= FALSE;
		
	m_CameraDistance	= 0.0f;	//카메라 거리 
	m_CameraTargetHeight= 0.0f; //카메라 대상 높이
	m_CameraPitch		= 0.0f;	//카메라 상하 
	m_CameraYaw			= 0.0f;	//카메라 좌우 
	
	m_SystemMessage		= NULL;	//시스템 메세지
	m_Description		= NULL;	//간략한 설명 
	
	m_InsideFrustum		= FALSE;
	m_Position			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	m_ModelDescriptor.ReserveModelStack( _XDEF_MAXMODEL_COMBINATIONCOUNT );
	m_ModelDescriptor.m_SwordEffectInitialized = FALSE;
	
	m_AttackTargetObjectType		= 0;
	m_AttackTarget	= NULL;
	m_AttackHitValue= 0.0f;
	m_AttackInfo	= -1;

	m_bAttackable		= FALSE;
	m_HealthPercentage	= 100;
	m_AttackalbeTypeUniqueID  = -1;
	
	memset( m_matTemp, 0, sizeof(D3DXMATRIX) );
	m_matTemp._11 = 1.0f;
	m_matTemp._22 = 1.0f;
	m_matTemp._33 = 1.0f;
	m_matTemp._44 = 1.0f;	

	m_DieStartTime = 0;
	
	m_DefenderType =  0;
	m_DefenderID   =  0;
	
	m_DieStartTime	= 0;
	m_bDie			= FALSE;
}

_XFunctionalObject::~_XFunctionalObject()
{
	m_listFunctionalObjectTrigger.disposeList();
	
#ifndef _XDEF_USENEWAREATEXTSYSTEM
	SAFE_DELETE_ARRAY( m_ObjectName );
	SAFE_DELETE_ARRAY(m_SystemMessage);
	SAFE_DELETE_ARRAY( m_Description );
#endif

}


BOOL _XFunctionalObject::LoadScript( LPSTR Filename )
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
		return FALSE;
	}

	return TRUE;
}

BOOL _XFunctionalObject::LoadScript( FILE* FilePtr )
{
	if( !FilePtr ) return FALSE;	

	int count = this->m_listFunctionalObjectTrigger.getitemcount();

	if( fread( &count, sizeof(int), 1, FilePtr ) < 1 )
	{
		return FALSE;
	}

	this->m_listFunctionalObjectTrigger.disposeList();

	for( int i = 0; i < count; ++i )
	{
		_XFunctionalObjectTriggerItem* objecttrigger = new _XFunctionalObjectTriggerItem;

		if( objecttrigger )
		{
			if( !objecttrigger->Load( FilePtr ) )
			{
				return FALSE;
			}
			this->m_listFunctionalObjectTrigger.insertItem( objecttrigger );
		}
		else
		{
			return FALSE;
		}		
	}

	return TRUE;
}


BOOL _XFunctionalObject::Load(FILE* fileptr)
{
	
	int strlength = 0;		
	if( fread( &strlength, sizeof(int), 1, fileptr ) < 1 )
	{
		fclose( fileptr );
		return FALSE;
	}
	
	TCHAR	tempbuffer[256];
	int		resourceid  = -1;

#ifdef _XDEF_USENEWAREATEXTSYSTEM
	memset( tempbuffer, 0, sizeof(TCHAR) * 256 );

	if( strlength > 0 )
	{
		if( fread( tempbuffer, strlength, 1, fileptr ) < 1 )
		{
			fclose( fileptr );
			return FALSE;
		}		
	}
#else
	if( m_ObjectName )
	{
		delete[] m_ObjectName;
		m_ObjectName = NULL;
	}
	
	if( strlength > 0 )
	{
		m_ObjectName = new TCHAR[ strlength+1 ];
		
		if( !m_ObjectName )
		{
			return FALSE;
		}
		
		memset( m_ObjectName, 0, sizeof(TCHAR)*(strlength+1) );
		
		if( fread( m_ObjectName, strlength, 1, fileptr ) < 1 )
		{
			fclose( fileptr );
			return FALSE;
		}		
	}
#endif

	if(fread(&m_UniqueID, sizeof(int), 1, fileptr) < 1)
	{
		return FALSE;
	}

#ifdef _XDEF_USENEWAREATEXTSYSTEM
	resourceid = (((g_CurrentZoneInfoIndex) * 100 ) + m_UniqueID) * 10000;
	m_ObjectName = g_StringDataBase.GetTextString( _XSTRINGDB_SN_FUNCTIONALOBJECT, resourceid );
#endif

	if(fread(&m_ObjectType, sizeof(int), 1, fileptr) < 1)
	{
		return FALSE;
	}
//#ifdef _XTS_XTGVER_89
//	if(fread(&m_ObjectClass, sizeof(int), 1, fileptr) < 1)
//	{
//		return FALSE;
//	}
//#endif

	if(fread(&m_bWideViewMode, sizeof(BOOL), 1, fileptr) < 1)
	{
		return FALSE;
	}
	if(fread(&m_bReLocateMode, sizeof(BOOL), 1, fileptr) < 1)
	{
		return FALSE;
	}
	
	if( m_bWideViewMode )
	{
		if(fread(&m_CameraDistance, sizeof(float), 1, fileptr) < 1)
		{
			return FALSE;
		}
		if(fread(&m_CameraTargetHeight, sizeof(float), 1, fileptr) < 1)
		{
			return FALSE;
		}
		if(fread(&m_CameraPitch, sizeof(float), 1, fileptr) < 1)
		{
			return FALSE;
		}
		if(fread(&m_CameraYaw, sizeof(float), 1, fileptr) < 1)
		{
			return FALSE;
		}

		// 시스템 메세지 
		int strlength = 0;		
		if( fread( &strlength, sizeof(int), 1, fileptr ) < 1 )
		{
			fclose( fileptr );
			return FALSE;
		}

#ifdef _XDEF_USENEWAREATEXTSYSTEM
		memset( tempbuffer, 0, sizeof(TCHAR) * 256 );
		
		if( strlength > 0 )
		{
			if( fread( tempbuffer, strlength, 1, fileptr ) < 1 )
			{
				fclose( fileptr );
				return FALSE;
			}		
			
			resourceid = (((g_CurrentZoneInfoIndex) * 100 ) + m_UniqueID) * 10000 + 1;
			m_SystemMessage = g_StringDataBase.GetTextString( _XSTRINGDB_SN_FUNCTIONALOBJECT, resourceid );
		}
#else
		if( m_SystemMessage )
		{
			delete[] m_SystemMessage;
			m_SystemMessage = NULL;
		}
		
		if( strlength > 0 )
		{
			m_SystemMessage = new TCHAR[ strlength+1 ];
			
			if( !m_SystemMessage )
			{
				return FALSE;
			}
			
			memset( m_SystemMessage, 0, sizeof(TCHAR)*(strlength+1) );
			
			if( fread( m_SystemMessage, strlength, 1, fileptr ) < 1 )
			{
				fclose( fileptr );
				return FALSE;
			}		
		}
#endif

		// 간략한 설명 
		strlength = 0;		
		if( fread( &strlength, sizeof(int), 1, fileptr ) < 1 )
		{
			fclose( fileptr );
			return FALSE;
		}

#ifdef _XDEF_USENEWAREATEXTSYSTEM
		memset( tempbuffer, 0, sizeof(TCHAR) * 256 );
		
		if( strlength > 0 )
		{
			if( fread( tempbuffer, strlength, 1, fileptr ) < 1 )
			{
				fclose( fileptr );
				return FALSE;
			}		
			
			resourceid = (((g_CurrentZoneInfoIndex) * 100 ) + m_UniqueID) * 10000 + 2;
			m_Description = g_StringDataBase.GetTextString( _XSTRINGDB_SN_FUNCTIONALOBJECT, resourceid );
		}
#else		
		if( m_Description )
		{
			delete[] m_Description;
			m_Description = NULL;
		}
		
		if( strlength > 0 )
		{
			m_Description = new TCHAR[ strlength+1 ];
			
			if( !m_Description )
			{
				return FALSE;
			}
			
			memset( m_Description, 0, sizeof(TCHAR)*(strlength+1) );
			
			if( fread( m_Description, strlength, 1, fileptr ) < 1 )
			{
				fclose( fileptr );
				return FALSE;
			}
		}
#endif
	}

	_XObject objectsaveformat;
	if(fread(&objectsaveformat, sizeof(_XObject), 1, fileptr) < 1)
	{
		return FALSE;
	}
	m_ObjectIndex		= objectsaveformat.m_ObjectIndex;
	m_matWorldPosition	= objectsaveformat.m_matWorldPosition;

	m_Position			= D3DXVECTOR3(m_matWorldPosition._41 , m_matWorldPosition._42, m_matWorldPosition._43);
	if( m_ObjectType != -1 )
	{
		if( m_ObjectType >= 4000 )
		{
			m_ObjectType -= 4000;
			g_Moblist.AddReferenceModel( m_ObjectType );
			
			m_ModelDescriptor.ReserveModelStack(1);
			m_ModelDescriptor.SetMeshModel( 0, g_MOBMODEL[m_ObjectType] );
			m_ModelDescriptor.LinkBipedObject( &g_MOBBIPEDMODEL[m_ObjectType] );
			m_ModelDescriptor.PlayAnimation( true );
			m_ModelDescriptor.m_Position = m_matWorldPosition;
			CreateEffectManager();

			if(g_MobAttribTable[m_ObjectType].scripttype == 85)
			{
				// 석상은 packet을 받아야지만 rendering 되도록
				m_HealthPercentage = 0;
				m_bDie = TRUE;
			}
		}
		else
		{
			_XFatalError( "Functional object type error : %d", m_ObjectType );
			return FALSE;
		}

//		D3DXMATRIX rotmat;
//		D3DXMatrixIdentity( &rotmat );
//		D3DXMatrixRotationY( &rotmat, _X_RAD(270) );
//		D3DXMatrixMultiply( &m_ModelDescriptor.m_Position, &rotmat, &m_ModelDescriptor.m_Position );


	}
	


	if(!LoadScript(fileptr))
	{
		return FALSE;
	}

	return TRUE;
}


void _XFunctionalObject::CreateEffectManager( void )
{
	int swordeffecttracecount = 0;
	if( m_ModelDescriptor.m_pBipedController->m_pSwordTraceInfoData )
	{
		swordeffecttracecount = m_ModelDescriptor.m_pBipedController->m_pSwordTraceInfoData->m_Pointcount;
	}
	
	m_ModelDescriptor.InitEffectManager(swordeffecttracecount);	
}
void _XFunctionalObject::SetIdleAnimation( void )
{
	if( g_MobMotionExistTable[m_ObjectType].motion_idle > -1 )
	{
		m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, true );
	}
}
void _XFunctionalObject::SetBlowMotion( void )
{
	if( g_MobMotionExistTable[m_ObjectType].motion_blow > -1 )
	{
		m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_blow, true );
	}
	else
	{
		m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, true );
	}
}
void _XFunctionalObject::SetDieAnimation( void )
{
	if( m_HealthPercentage > 0 ) return;

	if( m_AttackalbeTypeUniqueID != -1)
	{
		if( g_MobMotionExistTable[m_ObjectType].motion_die2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_die2, true );
		}
		else
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, true );
		}
		m_AttackalbeTypeUniqueID = -1;
		g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[m_UniqueID].m_bCollidable = FALSE;
		m_bAttackable = FALSE;
		m_DieStartTime = g_LocalSystemTime;

		if( this == (_XFunctionalObject*)g_LastPickedObject ) 
			g_LastPickedObject = NULL;
		
		if( this == (_XFunctionalObject*)g_pLocalUser->m_AttackTarget )
		{
			g_pLocalUser->ResetAttackTarget();
			g_pLocalUser->ResetSpellAttackTarget();
		}
		if(this == (_XFunctionalObject*)g_pLocalUser->m_ViewTarget)
		{
			g_pLocalUser->ResetViewTarget();
		}
		
		smdef_XUser::iterator dt;
		for (dt = g_Userlist.m_smUserList.begin(); dt != g_Userlist.m_smUserList.end(); ++ dt)
		{
			if( (*dt).second->m_AttackTarget )
			{
				if( (*dt).second->m_AttackTarget == this )
				{
					(*dt).second->ResetAttackTarget();
				}
			}
		}
	}
	
}
void _XFunctionalObject::SetAnimation( void )
{
	if( m_HealthPercentage <= 100 &&  m_HealthPercentage > 70)
	{
		if( g_MobMotionExistTable[m_ObjectType].motion_idle > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, true );
		}
	}
	else if( m_HealthPercentage <= 70 && m_HealthPercentage > 50)
	{						
		if( g_MobMotionExistTable[m_ObjectType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack2, true );
		}
		else
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, true );
		}
	}
	else if( m_HealthPercentage <= 50 && m_HealthPercentage > 30)
	{
		if( g_MobMotionExistTable[m_ObjectType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack3, true );
		}
		else
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, true );
		}
	}
	else if( m_HealthPercentage <= 30 && m_HealthPercentage > 0)
	{
		if( g_MobMotionExistTable[m_ObjectType].motion_die1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_die1, true );
		}
		else
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, true );
		}
	}
	else if( m_HealthPercentage <= 0 )
	{
		if( m_AttackalbeTypeUniqueID != -1)
		{
			if( g_MobMotionExistTable[m_ObjectType].motion_die2 > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_die2, true );
			}
			else
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, true );
			}
			m_AttackalbeTypeUniqueID = -1;
			g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[m_UniqueID].m_bCollidable = FALSE;
			m_bAttackable = FALSE;
		}
	}

}

void _XFunctionalObject::SetAnimationIndex( int animationindex )
{
	m_ActionIndex = _XMOBACTION_NONE;
	switch(animationindex)
	{
	case _XMOBACTIONNUMBER_IDLE :
		m_ActionIndex = _XMOBACTION_IDLE;
		break;
	case _XMOBACTIONNUMBER_WALK :
	case _XMOBACTIONNUMBER_RUN :
		m_ActionIndex = _XMOBACTION_MOVE;
		break;
	case _XMOBACTIONNUMBER_ATTACK1 :
	case _XMOBACTIONNUMBER_ATTACK2 :
	case _XMOBACTIONNUMBER_ATTACK3 :
	case _XMOBACTIONNUMBER_RATTACK :
		m_ActionIndex = _XMOBACTION_ATTACK;
		break;
	case _XMOBACTIONNUMBER_BLOW :
		m_ActionIndex = _XMOBACTION_BLOW;
		break;
	case _XMOBACTIONNUMBER_DIE1 :
	case _XMOBACTIONNUMBER_DIE2 :
		{
			m_ActionIndex = _XMOBACTION_DIE;
		}
		break;
	case _XMOBACTIONNUMBER_SKILL1 :
	case _XMOBACTIONNUMBER_SKILL2 :
		m_ActionIndex = _XMOBACTION_SKILL;
		break;
	case _XMOBACTIONNUMBER_ENTERING :
		m_ActionIndex = _XMOBACTION_ENTERING;
		break;
	}

	switch( animationindex )
	{
	case _XMOBACTIONNUMBER_IDLE :
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		if( g_MobMotionExistTable[m_ObjectType].motion_idle > -1 )
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, TRUE );
		break;
	case _XMOBACTIONNUMBER_ATTACK1 :
		if( g_MobMotionExistTable[m_ObjectType].motion_attack1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack1, TRUE );	
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack2, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack3, TRUE );
		}			
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;
	case _XMOBACTIONNUMBER_ATTACK2 :
		if( g_MobMotionExistTable[m_ObjectType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack2, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack1, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack3, TRUE );
		}
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;
	case _XMOBACTIONNUMBER_ATTACK3 :
		if( g_MobMotionExistTable[m_ObjectType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack3, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack1, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack2, TRUE );
		}
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;
	case _XMOBACTIONNUMBER_RATTACK :
		if(g_MobMotionExistTable[m_ObjectType].motion_rattack > -1)
		{
			m_ModelDescriptor.SetCurMotion(g_MobMotionExistTable[m_ObjectType].motion_rattack, TRUE);
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack1, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack2, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_attack3 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_attack3, TRUE );
		}
		m_ModelDescriptor.SetDrawSwordEffect( TRUE );
		break;
	case _XMOBACTIONNUMBER_BLOW	:
		if( g_MobMotionExistTable[m_ObjectType].motion_blow > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_blow, TRUE );
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;
	case _XMOBACTIONNUMBER_DIE1	:
		if( m_ModelDescriptor.m_EffectManager )
		{
			m_ModelDescriptor.m_EffectManager->Dispose();
		}	

		if( g_MobMotionExistTable[m_ObjectType].motion_die1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_die1, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_die2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_die2, TRUE );
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;	
	case _XMOBACTIONNUMBER_DIE2	:
		if( m_ModelDescriptor.m_EffectManager )
		{
			m_ModelDescriptor.m_EffectManager->Dispose();
		}
		
		if( g_MobMotionExistTable[m_ObjectType].motion_die2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_die2, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_die1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_die1, TRUE );
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;	
	case _XMOBACTIONNUMBER_SKILL1	:
		if( m_ModelDescriptor.m_EffectManager )
		{
			m_ModelDescriptor.m_EffectManager->Dispose();
		}
		
		if( g_MobMotionExistTable[m_ObjectType].motion_skill1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_skill1, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_skill2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_skill2, TRUE );
		}		
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;		
	case _XMOBACTIONNUMBER_SKILL2	:
		if( m_ModelDescriptor.m_EffectManager )
		{
			m_ModelDescriptor.m_EffectManager->Dispose();
		}
		
		if( g_MobMotionExistTable[m_ObjectType].motion_skill2 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_skill2, TRUE );
		}
		else if( g_MobMotionExistTable[m_ObjectType].motion_skill1 > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_skill1, TRUE );
		}
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		break;		
		
	case _XMOBACTIONNUMBER_ENTERING :
		m_ModelDescriptor.SetDrawSwordEffect( FALSE );
		if( g_MobMotionExistTable[m_ObjectType].motion_entering > -1 )
		{
			m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_entering, TRUE );
		}
		else
		{
			if( g_MobMotionExistTable[m_ObjectType].motion_idle > -1 )
			{
				m_ModelDescriptor.SetCurMotion( g_MobMotionExistTable[m_ObjectType].motion_idle, TRUE );
			}
		}
		break;
	}
}
void _XFunctionalObject::ProcessAction(void)
{
	if( g_bFunctionalObjectScriptStarted )
	{		
		//g_CurrentFocusedObject = _XDEF_DUMMYID_FUNCTIONALOBJECT;
		g_bFunctionalObjectScriptStarted = FALSE;
		
		_XFunctionalObjectTriggerItem* functionalobjectitem = (_XFunctionalObjectTriggerItem *)m_listFunctionalObjectTrigger.getItem(0);
		if( functionalobjectitem )
		{
			functionalobjectitem->m_ProgressFunctionalObjectTrigger.condition = 0;
			map <int, _XFunctionalObjectConditionItem*>::iterator iter_condition;
			_XFunctionalObjectConditionItem* currentcondition = NULL;
			
			DWORD result;
			// Gate System 일 때 바깥으로 나갈때는 무조건 이동 시킨다.  문 바깥 쪽이 작은수 안쪽이 큰수
			// 현재 지역번호가 다음 번호보다 작으면 조건을 검사하지 안고 무조건 넘긴다.
			int currentareanumber = -1;
			int nextareanumber = -1;
			bool checkgatesystem = false;
			bool bpasscondition = false;
			if(!functionalobjectitem->m_mapNormalActionList.empty())
			{
				map <int, _XFunctionalObjectActionItem*>::iterator iter_normalaction;
				_XFunctionalObjectActionItem* currentaction= NULL;
				for(iter_normalaction = functionalobjectitem->m_mapNormalActionList.begin() ; iter_normalaction != functionalobjectitem->m_mapNormalActionList.end() ; ++iter_normalaction)
				{
					currentaction = iter_normalaction->second;
					if(currentaction)
					{
						if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_PLAYERMOVE )
						{
							// 현재 맵에서의 이동일때를 체크한다.
							if( currentaction->m_TDA.m_actiondata1 != _XDEF_AREACATEGORY_MAPWARP )
							{
								checkgatesystem = true;
								
								_XArea* pArea = g_AreaManager.GetArea( currentaction->m_TDA.m_actiondata1, currentaction->m_TDA.m_numericvalue1 );
								
								if( pArea )
								{
									D3DXVECTOR3 v0,v1,		//영역을 나타내는 v0,v1,v2,v3의 4점과 
										vNormal,			//영역의 안팎분별을 위한 NormalVector인 vNormal
										vPoint;				//대상의 위치를 나타내는 vPoint
									
									D3DXVECTOR3 vTemp;		//영역 한변의 중점에서 바라보는 대상의 위치 
									
									v0 = D3DXVECTOR3(m_Position.x,0,m_Position.z);
									v1 = D3DXVECTOR3( ((pArea->m_Point[0].x+pArea->m_Point[2].x)/2) ,0, ((pArea->m_Point[0].z+pArea->m_Point[2].z)/2) );
									
									vPoint = g_pLocalUser->m_Position;
									vPoint.y = 0.0f;
									
									vNormal = v0 - v1;
									vTemp = v0 - vPoint;
									
									if(D3DXVec3Dot(&vNormal,&vTemp)>0) // 문 중심과 이동할려는 위치 사이에 있다. 이동하지 않고 패스 
									{
										currentareanumber = currentaction->m_TDA.m_numericvalue1;
									}
									else
									{
										nextareanumber = currentaction->m_TDA.m_numericvalue1;
									}
								}
							}
						}
					}
				}
			}
			if( checkgatesystem )
			{
				if( currentareanumber != -1 &&  nextareanumber != -1 )
				{
					if( currentareanumber > nextareanumber )
					{
						bpasscondition = true;
					}
				}
			}

			if( !bpasscondition )
			{
				for(iter_condition = functionalobjectitem->m_mapConditionList.begin() ; iter_condition != functionalobjectitem->m_mapConditionList.end() ; ++iter_condition)
				{
					currentcondition = iter_condition->second;
					if(currentcondition)
					{					
						if( currentcondition->Process() == QUEST_CONDITION_RESULT_SUCCESS)
						{	
							functionalobjectitem->SetProgressCondition( currentcondition->m_ConditionID );						
						}
						else
						{ 
							if( functionalobjectitem->m_byCompareClass == ID_TRIGGER_CONDITIONFUNCTION_AND )
							{							
								switch(currentcondition->m_TDC.m_conditionclass)
								{
								case ID_OBJECT_CONDITION_CHECKCLAN :				// 플레이어_문파비교
									{							
										TCHAR messagestring[512];
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2909) , g_JoinGroupName[ currentcondition->m_TDC.m_compareclass1 ] );//"[%s]이(가) 아닙니다."
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
									}
									break;
									
								case ID_OBJECT_CONDITION_CHECKITEM :				// 플레이어_아이템검사
									{
										TCHAR messagestring[512];
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2910), _XGameItem::GetItemName(currentcondition->m_TDC.m_compareclass1, currentcondition->m_TDC.m_numericvalue1));//[%s]이(가) 없습니다.
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
									}
									break;
									
								case ID_OBJECT_CONDITION_CHECKGENDER :				// 플레이어_성별검사
									{
										if( currentcondition->m_TDC.m_compareclass1 == _XGENDER_MAN )
										{
											TCHAR messagestring[512];
											_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2911));//"남자가 아닙니다."
											g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
										}
										else
										{
											TCHAR messagestring[512];
											_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2912));//"여자가 아닙니다."
											g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
										}	
									}
									break;
									
								case ID_OBJECT_CONDITION_CHECKQUESTCOMPLETE :		// 퀘스트 완료 검사
									break;
									
								default:
									break;
								}
							}
						}
					}
				}
				
				int conditioncount = functionalobjectitem->m_mapConditionList.size();
				
				DWORD progressconditionlist = functionalobjectitem->m_ProgressFunctionalObjectTrigger.condition;			
				
				switch(functionalobjectitem->m_byCompareClass)
				{
				case ID_TRIGGER_CONDITIONFUNCTION_OR :
					{
						result = progressconditionlist ;
						result = !result;
					}
					break;
				case ID_TRIGGER_CONDITIONFUNCTION_AND :
					{
						result = progressconditionlist ^ functionalobjectitem->GetCompareValue(conditioncount);
					}
					break;
				}
				
			}
			else
			{
				result = 0;
			}


			if(result == 0) // 성공시 NormalAction 실행
			{
				if(!functionalobjectitem->m_mapNormalActionList.empty())
				{
					bool movesuccess = false;
					map <int, _XFunctionalObjectActionItem*>::iterator iter_normalaction;
					_XFunctionalObjectActionItem* currentaction= NULL;
					for(iter_normalaction = functionalobjectitem->m_mapNormalActionList.begin() ; iter_normalaction != functionalobjectitem->m_mapNormalActionList.end() ; ++iter_normalaction)
					{
						currentaction = iter_normalaction->second;
						if(currentaction)
						{
							// 문을 이동할 때 양방향 이동을 위한 처리 
							if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_PLAYERMOVE )
							{
								if( !movesuccess )
								{
									if(g_pLocalUser->m_bSendFunctionalMovePacket)
									{
										// send packet
										g_NetworkKernel.SendPacket(MSG_NO_BW_WAR, en_bwwar_enter_gate_req, this->m_UniqueID);
										g_pLocalUser->m_nFuctionalNormalAction = 1;	// normal action
										movesuccess = true;
									}
									else
									{
										if( currentaction->Process() == FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS )
										{
											movesuccess = true;
										}
									}
								}
							}
							else
							{
								currentaction->Process();
							}
						}
					}	
				}
			}
			else // 실패시 ExceptedAction 실행 
			{
				if( functionalobjectitem->m_byCompareClass == ID_TRIGGER_CONDITIONFUNCTION_OR )
				{
					map <int, _XFunctionalObjectConditionItem*>::iterator iter_condition;
					_XFunctionalObjectConditionItem* currentcondition = NULL;
					for(iter_condition = functionalobjectitem->m_mapConditionList.begin() ; iter_condition != functionalobjectitem->m_mapConditionList.end() ; ++iter_condition)
					{
						currentcondition = iter_condition->second;
						if(currentcondition)
						{
							switch(currentcondition->m_TDC.m_conditionclass)
							{
							case ID_OBJECT_CONDITION_CHECKCLAN :				// 플레이어_문파비교
								{							
									TCHAR messagestring[512];
									_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2909), g_JoinGroupName[ currentcondition->m_TDC.m_compareclass1 ] ); //"[%s]이(가) 아닙니다."
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
								}
								break;
								
							case ID_OBJECT_CONDITION_CHECKITEM :				// 플레이어_아이템검사
								{
									TCHAR messagestring[512];
									_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2910), _XGameItem::GetItemName(currentcondition->m_TDC.m_compareclass1, currentcondition->m_TDC.m_numericvalue1)); //"[%s]이(가) 없습니다."
									g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);	
								}
								break;
								
							case ID_OBJECT_CONDITION_CHECKGENDER :				// 플레이어_성별검사
								{
									if( currentcondition->m_TDC.m_compareclass1 == _XGENDER_MAN )
									{
										TCHAR messagestring[512];
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2911));//"남자가 아닙니다."
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
									}
									else
									{
										TCHAR messagestring[512];
										_snprintf(messagestring, sizeof(messagestring), _XGETINTERFACETEXT(ID_STRING_NEW_2912));//"여자가 아닙니다."
										g_NetworkKernel.InsertChatString(messagestring, _XDEF_CHATMESSAGEMODE_GLOBALNOTICE + _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);
									}	
								}
								break;
								
							case ID_OBJECT_CONDITION_CHECKQUESTCOMPLETE :		// 퀘스트 완료 검사
								break;
								
							default:
								break;
							}
						}
					}
				}
				
				if(!functionalobjectitem->m_mapExceptedActionList.empty())
				{					
					bool movesuccess = false;
					map <int, _XFunctionalObjectActionItem*>::iterator iter_exceptedaction;
					_XFunctionalObjectActionItem* currentaction= NULL;
					for(iter_exceptedaction = functionalobjectitem->m_mapExceptedActionList.begin() ; iter_exceptedaction != functionalobjectitem->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
					{
						currentaction = iter_exceptedaction->second;
						if(currentaction)
						{
							// 문을 이동할 때 양방향 이동을 위한 처리 
							if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_PLAYERMOVE )
							{
								if( !movesuccess )
								{
									if(g_pLocalUser->m_bSendFunctionalMovePacket)
									{
										// send packet
										g_NetworkKernel.SendPacket(MSG_NO_BW_WAR, en_bwwar_enter_gate_req, this->m_UniqueID);
										g_pLocalUser->m_nFuctionalNormalAction = 2;		// excepted action
										movesuccess = true;
									}
									if( currentaction->Process() == FUNCTIONALOBJECT_ACTION_RESULT_SUCCESS )
									{
										movesuccess = true;
									}
								}
							}
							else
							{
								currentaction->Process();
							}
						}
					}				
				}
			}
			
		}
	}

}
void _XFunctionalObject::ProcessAnimation(void)
{	
	//if( m_ObjectClass != 2 ) return; // TRAP LPS 일때만 적용한다.
			
	// 체력이 0보다 작을 때는 죽는 모션을 처리해야 한다.
	if( m_HealthPercentage <= 0 )
	{
		if(m_DieStartTime == 0 && !m_bDie)
		{
			SetDieAnimation();
		}		
	}
	
	if( m_ActionIndex == _XMOBACTION_ATTACK )
	{
		if(m_AttackTarget)
		{
			if(m_AttackTargetObjectType == 3)//_XPICKINGTARGET_MOB
			{
				_XMob* pTarget = (_XMob*)m_AttackTarget;

				if(pTarget->m_Action.action != _XMOBACTION_DIE)
				{
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME) 
					{
						D3DXMATRIX offsetmat = pTarget->m_ModelDescriptor.m_Position;
						BOOL bIntersect = FALSE;
						D3DXVECTOR3 pickedpoint, originpos;
						D3DXVECTOR3 direction = pTarget->m_Position - m_Position;
						originpos = m_Position;
						originpos.y = m_Position.y + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f);
						
						bIntersect = pTarget->m_ModelDescriptor.InterOBBvsRay(originpos, direction, pTarget->m_Position, pTarget->m_RotateAngle, &pickedpoint);
						if(!bIntersect)
							offsetmat._42 += 0.98f;
						else
						{
							offsetmat._41 = pickedpoint.x;
							offsetmat._42 = pickedpoint.y;
							offsetmat._43 = pickedpoint.z;
						}

						switch(m_AttackInfo)
						{
					case 0 :
					case 5 :
						{
							// Hit
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [HIT]", m_ObjectType, pTarget->m_MobType);
							
							pTarget->m_AttackTarget = this;
							pTarget->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
							
							D3DXVECTOR3 position;
							position = pTarget->m_Position;								
							
							int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
							if( weaponmaterial < g_MobWeaponSoundCount )
							{
								_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
							}
							
							pTarget->SetBlowMotion();
							
							if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								if(pTarget->m_ModelDescriptor.m_EffectManager)
									pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
									_XDEF_COMMONID_HIT2, &offsetmat );
							}
						}
						break;
					case 1 :
					case 6 :
						{
							// Critical
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [CRITICAL]", m_ObjectType, pTarget->m_MobType);
							
							pTarget->m_AttackTarget = this;
							pTarget->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
							
							D3DXVECTOR3 position;
							position = pTarget->m_Position;								
							
							int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
							if( weaponmaterial < g_MobWeaponSoundCount )
							{
								_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
							}

							pTarget->SetBlowMotion();
							
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								if(pTarget->m_ModelDescriptor.m_EffectManager)
									pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
									_XDEF_COMMONID_HIT2, &offsetmat );
							}
						}
						break;
					case 2 :
						{
							// Evation
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [EVATION]", m_ObjectType, pTarget->m_MobType);
						}
						break;
					case 3 :
						{
							// Block
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [BLOCK]", m_ObjectType, pTarget->m_MobType);
							
							D3DXVECTOR3 position;
							position = pTarget->m_Position;								
							
							int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
							if( weaponmaterial < g_MobWeaponSoundCount )
							{
								_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
							}

							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								if(pTarget->m_ModelDescriptor.m_EffectManager)
									pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
									_XDEF_COMMONID_HIT2, &offsetmat );
							}
						}
						break;
					case 4 :
						{
							// Shock
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To Monster[%d/%d] [SHOCK]", m_ObjectType, pTarget->m_MobType);
							
							pTarget->m_AttackTarget = this;
							pTarget->m_AttackTargetObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
							
							D3DXVECTOR3 position;
							position = pTarget->m_Position;								
							
							int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
							if( weaponmaterial < g_MobWeaponSoundCount )
							{
								_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
							}
							
							pTarget->SetBlowMotion();
							
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								if(pTarget->m_ModelDescriptor.m_EffectManager)
									pTarget->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager, 
									_XDEF_COMMONID_HIT2, &offsetmat );
							}
						}
						break;
					default :
						break;
					}
						if(m_AttackHitValue < 0.0f)
						{
							pTarget->SetAnimation(_XMOBACTION_DIE);
							pTarget->m_ModelDescriptor.SetLockEndFrame(TRUE);
							m_AttackHitValue = 0.0f;
						}
					} // if Command frame
				} // if(pTarget->m_Action != _XMOBACTION_DIE)
			} // if(m_AttackTargetObjectType == _XPICKINGTARGET_MOB)
			else if(m_AttackTargetObjectType == 2)//_XPICKINGTARGET_SELF
			{
				if(g_pLocalUser->GetMotionClass() != _XACTION_DIE)
				{
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME) 
					{
						D3DXMATRIX offsetmat = g_pLocalUser->m_ModelDescriptor.m_Position;
						
						BOOL bIntersect = FALSE;
						D3DXVECTOR3 pickedpoint, originpos;
						D3DXVECTOR3 direction = g_pLocalUser->m_Position - m_Position;
						originpos = m_Position;
						originpos.y = m_Position.y + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f);
						
						bIntersect = g_pLocalUser->m_ModelDescriptor.InterOBBvsRay(originpos, direction, g_pLocalUser->m_Position, g_pLocalUser->m_RotateAngle, &pickedpoint);
						if(!bIntersect)
						{
							offsetmat._42 += 0.98f;
						}
						else
						{
							offsetmat._41 = pickedpoint.x;
							offsetmat._42 = pickedpoint.y;
							offsetmat._43 = pickedpoint.z;
						}

						switch(m_AttackInfo)
						{
					case 0 :
					case 5 :
						{
							// Hit
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To LocalUser [HIT]");
															
							D3DXVECTOR3 position;
							position = g_pLocalUser->m_Position;								
							
							int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
							if( weaponmaterial < g_MobWeaponSoundCount )
							{
								_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
							}

							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									_XDEF_COMMONID_HIT2, &offsetmat);
							}
						}
						break;
					case 1 :
					case 6 :
						{
							// Critical
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To LocalUser [CRITICAL]");
							
							D3DXVECTOR3 position;
							position = g_pLocalUser->m_Position;								
							
							int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
							if( weaponmaterial < g_MobWeaponSoundCount )
							{
								_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
							}
							
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									_XDEF_COMMONID_HIT2, &offsetmat);
							}
						}
						break;
					case 2 :
						{
							// Evation
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To Monster [EVATION]");
						}
						break;
					case 3 :
						{
							// Block
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To Monster [BLOCK]");
							
							D3DXVECTOR3 position;
							position = g_pLocalUser->m_Position;								
							
							int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
							if( weaponmaterial < g_MobWeaponSoundCount )
							{
								_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
							}

							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									_XDEF_COMMONID_HIT2, &offsetmat);
							}
						}
						break;
					case 4 :
						{
							// Shock
							VIEWBATTLELOG
								_XDWINPRINT("=== monster attack(process) To Monster [SHOCK]");
							
							D3DXVECTOR3 position;
							position = g_pLocalUser->m_Position;								
							
							int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
							if( weaponmaterial < g_MobWeaponSoundCount )
							{
								_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
							}
							
							g_pLocalUser->SetBlowMotion();
							
							if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
							{
								g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
									_XDEF_COMMONID_HIT2, &offsetmat);
							}
						}
						break;
					default :
						break;
						}
					} // if command frame
				} // if(g_pLocalUser->GetMotionClass() != _XACTION_DIE)
			} // if(m_AttackTargetObjectType == _XPICKINGTARGET_SELF)
			else if(m_AttackTargetObjectType == 1)//_XPICKINGTARGET_PC
			{
				_XUser* pUser = (_XUser*)m_AttackTarget;

				if(pUser->GetMotionClass() != _XACTION_DIE)
				{
					if(m_ModelDescriptor.m_FrameResult & _XDEF_ACI_COMMANDFRAME) // 2005-4-26 kerygma 옮김.
					{
						D3DXMATRIX offsetmat = pUser->m_ModelDescriptor.m_Position;
						
						BOOL bIntersect = FALSE;
						D3DXVECTOR3 pickedpoint, originpos;
						D3DXVECTOR3 direction = pUser->m_Position - m_Position;
						originpos = m_Position;
						originpos.y = m_Position.y + 0.15f + 1.f * ((FLOAT)(m_ModelDescriptor.m_EffectPosition+1) / 3.0f);
						
						bIntersect = pUser->m_ModelDescriptor.InterOBBvsRay(originpos, direction, pUser->m_Position, pUser->m_RotateAngle, &pickedpoint);
						
						if(!bIntersect)
						{
							offsetmat._42 += 0.98f;
						}
						else
						{
							offsetmat._41 = pickedpoint.x;
							offsetmat._42 = pickedpoint.y;
							offsetmat._43 = pickedpoint.z;
						}
						

						switch(m_AttackInfo)
						{
						case 0 :
						case 5 :
							{
								// Hit
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other User[HIT]");
								
								D3DXVECTOR3 position;
								position = pUser->m_Position;								
								
								int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
								if( weaponmaterial < g_MobWeaponSoundCount )
								{
									_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
								}
								
								if(m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pUser->m_ModelDescriptor.m_EffectManager)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 1 :
						case 6 :
							{
								// Critical
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other user [CRITICAL]");
								
								D3DXVECTOR3 position;
								position = pUser->m_Position;								
								
								int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
								if( weaponmaterial < g_MobWeaponSoundCount )
								{
									_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
								}
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pUser->m_ModelDescriptor.m_EffectManager)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 2 :
							{
								// Evation
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other user [EVATION]");
							}
							break;
						case 3 :
							{
								// Block
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other user [BLOCK]");
								
								D3DXVECTOR3 position;
								position = pUser->m_Position;								
								
								int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
								if( weaponmaterial < g_MobWeaponSoundCount )
								{
									_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
								}
								if(m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS)
								{
									if(pUser->m_ModelDescriptor.m_EffectManager)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						case 4 :
							{
								// Shock
								VIEWBATTLELOG
									_XDWINPRINT("=== monster attack(process) To Other user [SHOCK]");
								
								D3DXVECTOR3 position;
								position = pUser->m_Position;								
								
								int weaponmaterial = g_MobAttribTable[m_ObjectType].weaponmaterial;
								if( weaponmaterial < g_MobWeaponSoundCount )
								{
									_XPlaySoundEffect( g_MobWeaponSoundIndexList[weaponmaterial], m_Position);
								}
								pUser->SetBlowMotion();
								
								if( m_ModelDescriptor.m_CameraDistance < _XDEF_EFFECT_CULLINGRADIUS )
								{
									if(pUser->m_ModelDescriptor.m_EffectManager)
										pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance(&g_CharacterEffectGroupManager, 
										_XDEF_COMMONID_HIT2, &offsetmat);
								}
							}
							break;
						default :
							break;
						}
					} // if command frame
				} // if(pTarget->GetMotionClass() != _XACTION_DIE)
			}// else if(m_AttackTargetObjectType == _XPICKINGTARGET_PC)
		} // has attack target


		if( m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
		{
			SetAnimationIndex(_XMOBACTIONNUMBER_IDLE);			
			m_DefenderID = 0;
		}
		

	}
	else if( m_ActionIndex == _XMOBACTION_BLOW)
	{
		if( m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
		{
			SetAnimationIndex(_XMOBACTIONNUMBER_IDLE);					
			m_DefenderID = 0;
		}
	}
	else if( m_ModelDescriptor.GetCurMotion() == g_MobMotionExistTable[m_ObjectType].motion_die2 )
	{
		if( m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
		{
			m_ModelDescriptor.SetLastFrame();
			m_ModelDescriptor.PlayAnimation(FALSE);								
			m_DefenderID = 0;
			m_bDie = TRUE;
		}
	}
//	else if( m_ActionIndex == _XMOBACTION_DIE)
//	{
//		if( m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
//		{
//			m_ModelDescriptor.SetLastFrame();
//			m_ModelDescriptor.PlayAnimation(FALSE);								
//			m_DefenderID = 0;
//			m_bDie = TRUE;
//		}						
//	}

	
}

BOOL _XFunctionalObject::SetAttackableFunctionalObject( void )
{
	m_bAttackable = FALSE;
	switch(m_ObjectClass) {
	case 0://문
		{			
			_XFunctionalObjectTriggerItem* functionalobjectitem = (_XFunctionalObjectTriggerItem *)m_listFunctionalObjectTrigger.getItem(0);
			if( functionalobjectitem )
			{
				if( functionalobjectitem->m_mapNormalActionList.empty() && functionalobjectitem->m_mapExceptedActionList.empty() )
				{
					// 행동식이 비어 있으면 공격가능을 세팅할 수 없으므로 FALSE이다.
					m_bAttackable = FALSE;
				}
				else
				{

					// 먼저 행동식에 공격가능하게 세팅하는 행동식이 있는지 검사한다.
					bool checkattactableaction = false;					
					bool bexistnormalaction = false;	// 만약 원하는 행동식이 존재한다면 Normal Action에 있는지 아닌지를 검사한다.
					
					// Normal Action 검사
					if( !functionalobjectitem->m_mapNormalActionList.empty())
					{
						map <int, _XFunctionalObjectActionItem*>::iterator iter_normalaction;
						_XFunctionalObjectActionItem* currentaction= NULL;
						for(iter_normalaction = functionalobjectitem->m_mapNormalActionList.begin() ; iter_normalaction != functionalobjectitem->m_mapNormalActionList.end() ; ++iter_normalaction)
						{
							currentaction = iter_normalaction->second;
							if(currentaction)
							{
								if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_SETATTACKOBJECTSTATE )
								{
									checkattactableaction = true;
									bexistnormalaction = true;
									break;
								}
								else if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_CONDITIONALATTACTSETTING )
								{
									if( currentaction->m_TDA.m_actiondata1 == 0 ) //장원전 시작일때 
									{
										//장원전이 시작했는지 검사한다.
										if(g_CBManager.m_enWarState >= _XEN_CB_PROGRESS && g_CBManager.m_enWarState < _XEN_CB_END)
										{
											checkattactableaction = true;
											bexistnormalaction = true;
											break;
										}
									}
								}
							}
						}
					}
					// Normal Action에 없으면 Exception Action 검사
					if( !checkattactableaction )
					{						
						if(!functionalobjectitem->m_mapExceptedActionList.empty())
						{
							map <int, _XFunctionalObjectActionItem*>::iterator iter_exceptedaction;
							_XFunctionalObjectActionItem* currentaction= NULL;
							for(iter_exceptedaction = functionalobjectitem->m_mapExceptedActionList.begin() ; iter_exceptedaction != functionalobjectitem->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
							{
								currentaction = iter_exceptedaction->second;
								if(currentaction)
								{
									if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_SETATTACKOBJECTSTATE )
									{
										checkattactableaction = true;
										bexistnormalaction = false;
										break;
									}
									else if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_CONDITIONALATTACTSETTING )
									{
										if( currentaction->m_TDA.m_actiondata1 == 0 ) //장원전 시작일때 
										{
											//장원전이 시작했는지 검사한다.
											if(g_CBManager.m_enWarState >= _XEN_CB_PROGRESS && g_CBManager.m_enWarState < _XEN_CB_END)
											{
												checkattactableaction = true;
												bexistnormalaction = false;
												break;
											}
										}
									}
								}
							}
						}
					}//if( !checkattactableaction )

					// 행동식을 검사해서 공격가능하게 세팅하는 행동식이 있으면 조건식을 검사하여 공격가능한 오브젝트인지를 결정한다.
					if( checkattactableaction )
					{
						DWORD result;
						functionalobjectitem->m_ProgressFunctionalObjectTrigger.condition = 0;
						map <int, _XFunctionalObjectConditionItem*>::iterator iter_condition;
						_XFunctionalObjectConditionItem* currentcondition = NULL;
						for(iter_condition = functionalobjectitem->m_mapConditionList.begin() ; iter_condition != functionalobjectitem->m_mapConditionList.end() ; ++iter_condition)
						{
							currentcondition = iter_condition->second;
							if(currentcondition)
							{					
								if( currentcondition->Process() == QUEST_CONDITION_RESULT_SUCCESS)
								{	
									functionalobjectitem->SetProgressCondition( currentcondition->m_ConditionID );						
								}
							}
						}
						
						int conditioncount = functionalobjectitem->m_mapConditionList.size();
						
						DWORD progressconditionlist = functionalobjectitem->m_ProgressFunctionalObjectTrigger.condition;			
						
						switch(functionalobjectitem->m_byCompareClass)
						{
						case ID_TRIGGER_CONDITIONFUNCTION_OR :
							{
								result = progressconditionlist ;
								result = !result;
							}
							break;
						case ID_TRIGGER_CONDITIONFUNCTION_AND :
							{
								result = progressconditionlist ^ functionalobjectitem->GetCompareValue(conditioncount);
							}
							break;
						}
						
						if(result == 0) // 성공시 NormalAction 실행
						{
							if( bexistnormalaction )
							{
								m_bAttackable = TRUE;
							}
							else
							{
								m_bAttackable = FALSE;
							}
						}
						else // 실패시 ExceptedAction 실행 
						{
							if( !bexistnormalaction )
							{
								m_bAttackable = TRUE;
							}
							else
							{
								m_bAttackable = FALSE;
							}
						}
					}//if( checkattactableaction )
					//End- 행동식을 검사해서 공격가능하게 세팅하는 행동식이 있으면 조건식을 검사하여 공격가능한 오브젝트인지를 결정한다.
				}
				
			}
		}
		break;
	case 1://보스전입구
		{
			m_bAttackable = FALSE;
		}
		break;
	case 2://TRAP LPS
		{
			m_bAttackable = TRUE;
		}
		break;
	default:
		{
			if( g_MobAttribTable[m_ObjectType].scripttype == 85 )
			{
				m_bAttackable = TRUE;
			}
			else
			{
				_XFunctionalObjectTriggerItem* functionalobjectitem = (_XFunctionalObjectTriggerItem *)m_listFunctionalObjectTrigger.getItem(0);
				if( functionalobjectitem )
				{
					if( functionalobjectitem->m_mapNormalActionList.empty() && functionalobjectitem->m_mapExceptedActionList.empty() )
					{
						// 행동식이 비어 있으면 공격가능을 세팅할 수 없으므로 FALSE이다.
						m_bAttackable = FALSE;
					}
					else
					{
						bool checkmatchinfo = false; // 비무 전광판 출력 오브젝트이면 다르게 처리해야한다.

						// 먼저 행동식에 공격가능하게 세팅하는 행동식이 있는지 검사한다.
						bool checkattactableaction = false;					
						bool bexistnormalaction = false;	// 만약 원하는 행동식이 존재한다면 Normal Action에 있는지 아닌지를 검사한다.
						bool bexistexcepttionaction = false;	// 만약 원하는 행동식이 존재한다면 Excepteion Action에 있는지 아닌지를 검사한다.
						
						// Normal Action 검사
						if( !functionalobjectitem->m_mapNormalActionList.empty())
						{
							map <int, _XFunctionalObjectActionItem*>::iterator iter_normalaction;
							_XFunctionalObjectActionItem* currentaction= NULL;
							for(iter_normalaction = functionalobjectitem->m_mapNormalActionList.begin() ; iter_normalaction != functionalobjectitem->m_mapNormalActionList.end() ; ++iter_normalaction)
							{
								currentaction = iter_normalaction->second;
								if(currentaction)
								{
									if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_SETATTACKOBJECTSTATE )
									{
										checkattactableaction = true;
										bexistnormalaction = true;
										break;
									}
									else if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_MATCHINFO )
									{
										checkmatchinfo = true;
										break;
									}
									else if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_CONDITIONALATTACTSETTING )
									{
										if( currentaction->m_TDA.m_actiondata1 == 0 ) //장원전 시작일때 
										{
											//장원전이 시작했는지 검사한다.
											if(g_CBManager.m_enWarState >= _XEN_CB_PROGRESS && g_CBManager.m_enWarState < _XEN_CB_END)
											{
												checkattactableaction = true;
												bexistnormalaction = true;
												break;
											}
										}
									}
								}
							}
						}
						// Normal Action에 없으면 Exception Action 검사
						if( !checkattactableaction )
						{						
							if(!functionalobjectitem->m_mapExceptedActionList.empty())
							{
								map <int, _XFunctionalObjectActionItem*>::iterator iter_exceptedaction;
								_XFunctionalObjectActionItem* currentaction= NULL;
								for(iter_exceptedaction = functionalobjectitem->m_mapExceptedActionList.begin() ; iter_exceptedaction != functionalobjectitem->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
								{
									currentaction = iter_exceptedaction->second;
									if(currentaction)
									{
										if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_SETATTACKOBJECTSTATE )
										{
											checkattactableaction = true;
											bexistexcepttionaction = true;
											break;
										}
										else if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_MATCHINFO )
										{
											checkmatchinfo = true;
											break;
										}
										else if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_CONDITIONALATTACTSETTING )
										{
											if( currentaction->m_TDA.m_actiondata1 == 0 ) //장원전 시작일때 
											{
												//장원전이 시작했는지 검사한다.
												if(g_CBManager.m_enWarState >= _XEN_CB_PROGRESS && g_CBManager.m_enWarState < _XEN_CB_END)
												{
													checkattactableaction = true;
													bexistexcepttionaction = true;
													break;
												}
											}
										}
									}
								}
							}
						}//if( !checkattactableaction )

						// 행동식을 검사해서 공격가능하게 세팅하는 행동식이 있으면 조건식을 검사하여 공격가능한 오브젝트인지를 결정한다.
						if( checkattactableaction )
						{
							DWORD result;
							functionalobjectitem->m_ProgressFunctionalObjectTrigger.condition = 0;
							map <int, _XFunctionalObjectConditionItem*>::iterator iter_condition;
							_XFunctionalObjectConditionItem* currentcondition = NULL;
							for(iter_condition = functionalobjectitem->m_mapConditionList.begin() ; iter_condition != functionalobjectitem->m_mapConditionList.end() ; ++iter_condition)
							{
								currentcondition = iter_condition->second;
								if(currentcondition)
								{					
									if( currentcondition->Process() == QUEST_CONDITION_RESULT_SUCCESS)
									{	
										functionalobjectitem->SetProgressCondition( currentcondition->m_ConditionID );						
									}
								}
							}
							
							int conditioncount = functionalobjectitem->m_mapConditionList.size();
							
							DWORD progressconditionlist = functionalobjectitem->m_ProgressFunctionalObjectTrigger.condition;			
							
							switch(functionalobjectitem->m_byCompareClass)
							{
							case ID_TRIGGER_CONDITIONFUNCTION_OR :
								{
									result = progressconditionlist ;
									result = !result;
								}
								break;
							case ID_TRIGGER_CONDITIONFUNCTION_AND :
								{
									result = progressconditionlist ^ functionalobjectitem->GetCompareValue(conditioncount);
								}
								break;
							}
							
							if(result == 0) // 성공시 NormalAction 실행
							{
								if( bexistnormalaction )
								{
									m_bAttackable = TRUE;
								}
								else
								{
									m_bAttackable = FALSE;
								}
							}
							else // 실패시 ExceptedAction 실행 
							{
								if( bexistexcepttionaction )
								{
									m_bAttackable = TRUE;
								}
								else
								{
									m_bAttackable = FALSE;
								}
							}
						}//if( checkattactableaction )
						else if( checkmatchinfo )
						{
							g_bFunctionalObjectScriptStarted = TRUE;
							ProcessAction();// 전광판을 바로 출력한다.							
						}
						//End- 행동식을 검사해서 공격가능하게 세팅하는 행동식이 있으면 조건식을 검사하여 공격가능한 오브젝트인지를 결정한다.
					}
					
				}
			}
				
		}
		break;
	}

	return m_bAttackable;
}

void _XFunctionalObject::ProcessMove()
{
	_XFunctionalObjectTriggerItem* functionalobjectitem = (_XFunctionalObjectTriggerItem *)m_listFunctionalObjectTrigger.getItem(0);
	if( functionalobjectitem )
	{
		if(g_pLocalUser->m_nFuctionalNormalAction == 1) // NormalAction 실행
		{
			if(!functionalobjectitem->m_mapNormalActionList.empty())
			{
				map <int, _XFunctionalObjectActionItem*>::iterator iter_normalaction;
				_XFunctionalObjectActionItem* currentaction= NULL;
				for(iter_normalaction = functionalobjectitem->m_mapNormalActionList.begin() ; iter_normalaction != functionalobjectitem->m_mapNormalActionList.end() ; ++iter_normalaction)
				{
					currentaction = iter_normalaction->second;
					if(currentaction)
					{
						// 문을 이동할 때 양방향 이동을 위한 처리 
						if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_PLAYERMOVE )
						{
							currentaction->Process();
						}
					}
				}	
			}
		}
		else if(g_pLocalUser->m_nFuctionalNormalAction == 2) // excepted action
		{
			if(!functionalobjectitem->m_mapExceptedActionList.empty())
			{
				map <int, _XFunctionalObjectActionItem*>::iterator iter_exceptedaction;
				_XFunctionalObjectActionItem* currentaction= NULL;
				for(iter_exceptedaction = functionalobjectitem->m_mapExceptedActionList.begin() ; iter_exceptedaction != functionalobjectitem->m_mapExceptedActionList.end() ; ++iter_exceptedaction)
				{
					currentaction = iter_exceptedaction->second;
					if(currentaction)
					{
						// 문을 이동할 때 양방향 이동을 위한 처리 
						if( currentaction->m_TDA.m_actionclass == ID_OBJECT_ACTION_PLAYERMOVE )
						{
							currentaction->Process();
						}
					}
				}	
			}
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Functional object manager
_XFunctionalObjectManager::_XFunctionalObjectManager()
{
	m_pFunctionalObjectArray = NULL;
	m_pFunctionalOBBDataArray = NULL;
	m_countFunctionalObject = 0;
}

_XFunctionalObjectManager::~_XFunctionalObjectManager()
{
	disposeList();
}

void _XFunctionalObjectManager::disposeList( void  )
{
	SAFE_DELETE_ARRAY( m_pFunctionalObjectArray );
	SAFE_DELETE_ARRAY( m_pFunctionalOBBDataArray );
	m_countFunctionalObject = 0;
}

BOOL _XFunctionalObjectManager::LoadFunctionalObjectList( FILE* Fileptr, LPSTR Filename )
{
	if( !Fileptr ) 
	{
		_XFatalError( "LoadFunctionalObjectList(%s) : Invalid file pointer", Filename );
		return FALSE;
	}
	
	disposeList();

	if( fread( &m_countFunctionalObject, sizeof(int), 1, Fileptr ) < 1 )
	{
		_XFatalError( "LoadFunctionalObjectList(%s) Read object count", Filename );		
		return FALSE;
	}	
	
	if( m_countFunctionalObject != 0 )
	{
		m_pFunctionalObjectArray = new _XFunctionalObject[ m_countFunctionalObject ];
		
		if( !m_pFunctionalObjectArray )
		{
			_XFatalError( "LoadFunctionalObjectList(%s) Allocation object temporary array", Filename );
			return FALSE;
		}
		for( int i = 0; i < m_countFunctionalObject; ++i )
		{
			if( !m_pFunctionalObjectArray[i].Load( Fileptr ) )
			{
				_XFatalError( "LoadFunctionalObjectList(%s) Read object data", Filename );
	//			delete[] functionalobject;
				return FALSE;
			}
			m_pFunctionalObjectArray[i].m_UniqueID = i;
		}
		SAFE_DELETE_ARRAY( m_pFunctionalOBBDataArray );
		m_pFunctionalOBBDataArray = new _XOBBData[ m_countFunctionalObject ];		
		if( !m_pFunctionalOBBDataArray )
		{
			_XFatalError( "LoadFunctionalObjectList(%s) Allocation object temporary array", Filename );
			return FALSE;
		}
		for( i = 0; i < m_countFunctionalObject; ++i )
		{
			int objectindex = m_pFunctionalObjectArray[i].m_ObjectIndex;
			
			float	OBB_Extent[3];
			OBB_Extent[0] = g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Extent[0] + 0.2f;
			OBB_Extent[1] = g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Extent[1] + 0.2f;
			OBB_Extent[2] = g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Extent[2] + 0.2f;
			
			D3DXVECTOR3 pAxis[3];
			pAxis[0] = g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Axis[0];
			pAxis[1] = g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Axis[1];
			pAxis[2] = g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Axis[2];
			
			D3DXMATRIX rotmat;
			
			D3DXMatrixIdentity( &rotmat );
			rotmat._11 = m_pFunctionalObjectArray[i].m_matWorldPosition._11;
			rotmat._21 = m_pFunctionalObjectArray[i].m_matWorldPosition._12;
			rotmat._31 = m_pFunctionalObjectArray[i].m_matWorldPosition._13;
			rotmat._12 = m_pFunctionalObjectArray[i].m_matWorldPosition._21;
			rotmat._22 = m_pFunctionalObjectArray[i].m_matWorldPosition._22;
			rotmat._32 = m_pFunctionalObjectArray[i].m_matWorldPosition._23;
			rotmat._13 = m_pFunctionalObjectArray[i].m_matWorldPosition._31;
			rotmat._23 = m_pFunctionalObjectArray[i].m_matWorldPosition._32;
			rotmat._33 = m_pFunctionalObjectArray[i].m_matWorldPosition._33;	
						
			D3DXMATRIX mat;
			
			D3DXMatrixIdentity( &mat );
			
			mat._11 = pAxis[0].x;
			mat._21 = pAxis[0].y;
			mat._31 = pAxis[0].z;
			mat._12 = pAxis[1].x;
			mat._22 = pAxis[1].y;
			mat._32 = pAxis[1].z;
			mat._13 = pAxis[2].x;
			mat._23 = pAxis[2].y;
			mat._33 = pAxis[2].z;	
			
			D3DXMatrixMultiply( &mat, &rotmat, &mat);
			
			pAxis[0].x = mat._11;
			pAxis[0].y = mat._21;
			pAxis[0].z = mat._31;
			pAxis[1].x = mat._12;
			pAxis[1].y = mat._22;
			pAxis[1].z = mat._32;
			pAxis[2].x = mat._13;
			pAxis[2].y = mat._23;
			pAxis[2].z = mat._33;

			
			D3DXVECTOR3 vCenter = D3DXVECTOR3(m_pFunctionalObjectArray[i].m_matWorldPosition._41 + g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Center.x,
				m_pFunctionalObjectArray[i].m_matWorldPosition._42 + g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Center.y,
				m_pFunctionalObjectArray[i].m_matWorldPosition._43 + g_LodTerrain.m_MeshObjectManager.m_ModelList[objectindex]->OBB_Center.z);
			
			m_pFunctionalOBBDataArray[i].Create(vCenter, pAxis, OBB_Extent);
			m_pFunctionalOBBDataArray[i].m_OBBLevel = 1;
			
			float fMagnitude  = _XFC_sqrt( (vCenter.x  * vCenter.x) + (vCenter.y  * vCenter.y) + (vCenter.z  * vCenter.z) );
			float fMagnitude2 = _XFC_sqrt( (OBB_Extent[0] * OBB_Extent[0]) + (OBB_Extent[1] * OBB_Extent[1]) + (OBB_Extent[2] * OBB_Extent[2]) );	
			m_pFunctionalOBBDataArray[i].m_Radius = fMagnitude + fMagnitude2;

			if(g_MobAttribTable[m_pFunctionalObjectArray[i].m_ObjectType].scripttype == 85)
			{
				// 석상은 packet을 받아야지만 충돌하도록
				m_pFunctionalOBBDataArray[i].m_bCollidable = FALSE;
			}
						
#ifdef _XDWDEBUG
			m_pFunctionalOBBDataArray[i].CreateOBBModel();
#endif
		}
		

		_XDWINPRINT( "%d Functional Object loaded", m_countFunctionalObject );
	}
	return TRUE;
}

void _XFunctionalObjectManager::CheckInsideFrustum( void )
{
	for( int i = 0; i < m_countFunctionalObject; ++i )
	{
		FLOAT dx = (m_pFunctionalObjectArray[i].m_matWorldPosition._41 - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
		FLOAT dz = (m_pFunctionalObjectArray[i].m_matWorldPosition._43 - g_LodTerrain.m_3PCamera.m_CameraPosition.z);

		D3DXVECTOR3 vecFunctionalObject = D3DXVECTOR3( m_pFunctionalObjectArray[i].m_matWorldPosition._41,
			m_pFunctionalObjectArray[i].m_matWorldPosition._42, m_pFunctionalObjectArray[i].m_matWorldPosition._43 );

		m_pFunctionalObjectArray[i].m_InsideFrustum =		
			( ( _XDistance( dx , dz ) < 150.0f ) &&
			( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( vecFunctionalObject, 10.0f ) != NOT_VISIBLE ) );

	}	
}

void _XFunctionalObjectManager::Render( void )
{
	gpDev->SetRenderState( D3DRS_AMBIENT, g_EnvironmentManager.m_ObjectAmbientColor );
	gpDev->SetRenderState( D3DRS_FOGENABLE, FALSE );

	for( int i = 0; i < m_countFunctionalObject; ++i )
	{
		if( m_pFunctionalObjectArray[i].m_InsideFrustum )
		{			
			if( g_LodTerrain.m_MeshObjectManager.m_ModelList[m_pFunctionalObjectArray[i].m_ObjectIndex] )
			{					
				if( m_pFunctionalObjectArray[i].m_HealthPercentage > 0 )
				{
					BOOL intersect = FALSE;
					
					D3DXVECTOR3 nRayDir;
					D3DXVECTOR3 mpos =  m_pFunctionalObjectArray[i].m_Position;
					mpos.y += g_LodTerrain.m_MeshObjectManager.m_ModelList[m_pFunctionalObjectArray[i].m_ObjectIndex]->OBB_Extent[1];
					D3DXVec3Normalize( &nRayDir, &g_vPickRayDir );
					
					
					intersect = RayAndSphereIntersect( g_vPickRayOrig, nRayDir, mpos, m_pFunctionalOBBDataArray[i].m_Radius );
					
					// Check intersect obb
					if( intersect )
					{
						D3DXVECTOR3 PickedPoint;
						intersect = m_pFunctionalOBBDataArray[i].InterOBBvsRay(g_vPickRayOrig, g_vPickInfinityRayDir, PickedPoint );
						
						if( intersect )
						{	
							FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - m_pFunctionalObjectArray[i].m_Position.x);
							FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - m_pFunctionalObjectArray[i].m_Position.z);
							FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
							
							if( _distance < 20.0f && _distance < g_LastPickedObjectDistance )
							{
								if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
								{
									g_LastPickedObjectDistance = _distance;
									g_LastPickedObjectType = _XPICKINGTARGET_FUNCTIONALOBJECT;
									g_LastPickedObject = (_XItem*)&m_pFunctionalObjectArray[i];	
									XProc_MainGame* pProc_Maingame = &((CEmperorOfDragonsApp*)gApp)->m_proc_maingame;
									if( !pProc_Maingame->m_bNPCDialogMode  && !pProc_Maingame->m_bNPCDialogWideView && !pProc_Maingame->m_bFunctionalObjectWideView)
									{
										if( g_MobAttribTable[m_pFunctionalObjectArray[i].m_ObjectType].scripttype != 85 &&
											g_MobAttribTable[m_pFunctionalObjectArray[i].m_ObjectType].scripttype != 90 &&
											!m_pFunctionalObjectArray[i].m_bAttackable ) 
										{
											CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
											eodapp->ReadyCursor( _XMOUSETYPE_GOTOGATE );
										}
										else
										{
											CEmperorOfDragonsApp* eodapp = (CEmperorOfDragonsApp*)gApp;
											eodapp->ReadyCursor( _XMOUSETYPE_MOBOVER );
										}
									}
								}
							}
						}	
					}
				}
				
				
				if( m_pFunctionalObjectArray[i].m_HealthPercentage == 100 && !m_pFunctionalObjectArray[i].m_bAttackable && g_MobAttribTable[m_pFunctionalObjectArray[i].m_ObjectType].scripttype != 85 )
				{
					g_LodTerrain.m_MeshObjectManager._RenderObject( m_pFunctionalObjectArray[i] );
				}
				else
				{
					bool soundplay = true;
					m_pFunctionalObjectArray[i].m_ModelDescriptor.Advance( soundplay, 0 );
					m_pFunctionalObjectArray[i].m_ModelDescriptor.Animation();
					m_pFunctionalObjectArray[i].m_ModelDescriptor.RenderFunctionalObject(&m_pFunctionalObjectArray[i]);
					
					if( m_pFunctionalObjectArray[i].m_ActionIndex == _XMOBACTION_ATTACK ) 
					{
						m_pFunctionalObjectArray[i].m_ModelDescriptor.RenderSwordEffect( _XDEF_DEFAULTRENDERSTYLE_MONSTERSWORDEFFECT );
					}					
				}						
				
				

//				if( m_pFunctionalObjectArray[i].m_HealthPercentage == 100 )
//				{
//					if( !m_pFunctionalObjectArray[i].m_bAttackable )
//					{
//						g_LodTerrain.m_MeshObjectManager._RenderObject( m_pFunctionalObjectArray[i] );
//					}
//					else
//					{
//						bool soundplay = true;
//						m_pFunctionalObjectArray[i].m_ModelDescriptor.Advance( soundplay, 0 );
//						
//						if( m_pFunctionalObjectArray[i].m_HealthPercentage > 0 )
//						{							
//							if( m_pFunctionalObjectArray[i].m_ModelDescriptor.GetCurMotion() == g_MobMotionExistTable[m_pFunctionalObjectArray[i].m_ObjectType].motion_blow )
//							{
//								if( m_pFunctionalObjectArray[i].m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
//								{
//									m_pFunctionalObjectArray[i].SetIdleAnimation();							
//									m_pFunctionalObjectArray[i].m_DefenderID = 0;
//								}
//							}
//
//							m_pFunctionalObjectArray[i].m_ModelDescriptor.Animation();
//							m_pFunctionalObjectArray[i].m_ModelDescriptor.RenderFunctionalObject(&m_pFunctionalObjectArray[i]);
//						}
//					}
//				}
//				else
//				{
//					bool soundplay = true;
//					m_pFunctionalObjectArray[i].m_ModelDescriptor.Advance( soundplay, 0 );
//
//					if( m_pFunctionalObjectArray[i].m_HealthPercentage > 0 )
//					{							
//						if( m_pFunctionalObjectArray[i].m_ModelDescriptor.GetCurMotion() == g_MobMotionExistTable[m_pFunctionalObjectArray[i].m_ObjectType].motion_blow )
//						{
//							if( m_pFunctionalObjectArray[i].m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
//							{
//								m_pFunctionalObjectArray[i].SetIdleAnimation();								
//								m_pFunctionalObjectArray[i].m_DefenderID = 0;
//							}
//						}
//
//						m_pFunctionalObjectArray[i].m_ModelDescriptor.Animation();
//						m_pFunctionalObjectArray[i].m_ModelDescriptor.RenderFunctionalObject(&m_pFunctionalObjectArray[i]);
//					}
//					else
//					{
//						if(m_pFunctionalObjectArray[i].m_DieStartTime == 0 && !m_pFunctionalObjectArray[i].m_bDie)
//						{
//							m_pFunctionalObjectArray[i].SetDieAnimation();
//						}
//						if( m_pFunctionalObjectArray[i].m_ModelDescriptor.GetCurMotion() == g_MobMotionExistTable[m_pFunctionalObjectArray[i].m_ObjectType].motion_die2 )
//						{
//							if( m_pFunctionalObjectArray[i].m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
//							{
//								m_pFunctionalObjectArray[i].m_ModelDescriptor.SetLastFrame();
//								m_pFunctionalObjectArray[i].m_ModelDescriptor.PlayAnimation(FALSE);								
//								m_pFunctionalObjectArray[i].m_DefenderID = 0;
//								m_pFunctionalObjectArray[i].m_bDie = TRUE;
//							}
//							if((g_LocalSystemTime - m_pFunctionalObjectArray[i].m_DieStartTime <= 3000) || m_pFunctionalObjectArray[i].m_DieStartTime == 0)
//							{
//								if(!m_pFunctionalObjectArray[i].m_bDie)
//								{
//									m_pFunctionalObjectArray[i].m_ModelDescriptor.Animation();
//									m_pFunctionalObjectArray[i].m_ModelDescriptor.RenderFunctionalObject(&m_pFunctionalObjectArray[i]);
//								}
//							}
//						}
//					}
//				}


				//// test
//				if( m_pFunctionalObjectArray[i].m_ModelDescriptor.GetCurMotion() == g_MobMotionExistTable[m_pFunctionalObjectArray[i].m_ObjectType].motion_die2 )
//				{
//					if( m_pFunctionalObjectArray[i].m_ModelDescriptor.m_FrameResult & _XDEF_ACI_LASTFRAME )
//					{
//						m_pFunctionalObjectArray[i].SetIdleAnimation();								
//					}
//				}			
			}
			
		}
	}

	gpDev->SetRenderState( D3DRS_FOGENABLE, TRUE );
}

void _XFunctionalObjectManager::Process( void )
{	
	for( int i = 0; i < m_countFunctionalObject; ++i )
	{
		//if( m_pFunctionalObjectArray[i].m_InsideFrustum )
		{			
			FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - m_pFunctionalObjectArray[i].m_Position.x);
			FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - m_pFunctionalObjectArray[i].m_Position.z);
			
			m_pFunctionalObjectArray[i].m_ModelDescriptor.m_CameraDistance = _XDistance(dx , dz);			
			if( m_pFunctionalObjectArray[i].m_ModelDescriptor.m_CameraDistance < 20.0f && m_pFunctionalObjectArray[i].m_ModelDescriptor.m_CameraDistance < g_LastPickedObjectDistance )
			{
				m_pFunctionalObjectArray[i].ProcessAnimation();
			}
		}
	}
}

void _XFunctionalObjectManager::SetAttackableFunctionalObjectID(int objectid, int objecttype, int healthpercentage, D3DXVECTOR3& vObjectPosition )
{
	for( int i = 0; i < m_countFunctionalObject; ++i )
	{
#ifdef _XDWDEBUG
		FLOAT dx = (m_pFunctionalObjectArray[i].m_matWorldPosition._41 - vObjectPosition.x);
		FLOAT dz = (m_pFunctionalObjectArray[i].m_matWorldPosition._43 - vObjectPosition.z);
		FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
		
		if( _distance < 0.5f )
		{
			if(m_pFunctionalObjectArray[i].m_ObjectType != objecttype)
			{
				_XDWINPRINT("Type 맞지 않음 (Client/Server) : %d/%d", m_pFunctionalObjectArray[i].m_ObjectType, objecttype);
			}
		}
#endif
		if(g_MobAttribTable[m_pFunctionalObjectArray[i].m_ObjectType].scripttype == 85)
		{
			// 석상
			if( m_pFunctionalObjectArray[i].m_ObjectType == objecttype )
			{
				//g_Moblist.AddReferenceModel(objecttype);
				
				FLOAT dx = (m_pFunctionalObjectArray[i].m_matWorldPosition._41 - vObjectPosition.x);
				FLOAT dz = (m_pFunctionalObjectArray[i].m_matWorldPosition._43 - vObjectPosition.z);
				FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
				
				if( _distance < 2.0f )
				{
					if( healthpercentage > 0 )
					{
						if( m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID == -1 )
						{
							m_pFunctionalObjectArray[i].m_ModelDescriptor.PlayAnimation(TRUE);
							m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID = objectid;
							m_pFunctionalOBBDataArray[i].m_bCollidable = TRUE;
//							m_pFunctionalObjectArray[i].m_bAttackable = FALSE;
							m_pFunctionalObjectArray[i].SetIdleAnimation();
							
							m_pFunctionalObjectArray[i].m_DieStartTime = 0;
							m_pFunctionalObjectArray[i].m_bDie = FALSE;
						}
						m_pFunctionalObjectArray[i].m_HealthPercentage = healthpercentage;
						
//						_XDWINPRINT("ObjectUID:%d  Name:%s  ObjectType:%d  health:%d", objectid, m_pFunctionalObjectArray[i].m_ObjectName, objecttype, healthpercentage);
					}
					else
					{
						m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID = objectid;
						m_pFunctionalOBBDataArray[i].m_bCollidable = FALSE;					
						m_pFunctionalObjectArray[i].m_HealthPercentage = healthpercentage;
						
						if(!m_pFunctionalObjectArray[i].m_bDie)
							m_pFunctionalObjectArray[i].m_bDie = TRUE;
						
//						_XDWINPRINT("ObjectUID:%d  Name:%s  ObjectType:%d  health:%d", objectid, m_pFunctionalObjectArray[i].m_ObjectName, objecttype, healthpercentage);
					}	
					break;
				}
			}
		}
		else if(g_MobAttribTable[objecttype].scripttype == 90)
		{
			FLOAT dx = (m_pFunctionalObjectArray[i].m_matWorldPosition._41 - vObjectPosition.x);
			FLOAT dz = (m_pFunctionalObjectArray[i].m_matWorldPosition._43 - vObjectPosition.z);
			FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );

//			_XDWINPRINT("object[%d] x:%f, z:%f", i, m_pFunctionalObjectArray[i].m_matWorldPosition._41, m_pFunctionalObjectArray[i].m_matWorldPosition._43);
			
			if( _distance < 2.0f )
			{
				if( healthpercentage > 0 )
				{
					if( m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID == -1 )
					{
						m_pFunctionalObjectArray[i].m_ModelDescriptor.PlayAnimation(TRUE);
						m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID = objectid;
						m_pFunctionalOBBDataArray[i].m_bCollidable = TRUE;
//						m_pFunctionalObjectArray[i].m_bAttackable = FALSE;
						m_pFunctionalObjectArray[i].SetIdleAnimation();
						
						m_pFunctionalObjectArray[i].m_DieStartTime = 0;
						m_pFunctionalObjectArray[i].m_bDie = FALSE;
					}
					else
					{
						// attackable id는 이미 있지만 collidable 및 나머지 다시 세팅
						if(!m_pFunctionalOBBDataArray[i].m_bCollidable)
						{
							m_pFunctionalObjectArray[i].m_ModelDescriptor.PlayAnimation(TRUE);
							m_pFunctionalOBBDataArray[i].m_bCollidable = TRUE;
							m_pFunctionalObjectArray[i].SetIdleAnimation();
							m_pFunctionalObjectArray[i].m_DieStartTime = 0;
							m_pFunctionalObjectArray[i].m_bDie = FALSE;
						}
					}

					m_pFunctionalObjectArray[i].m_HealthPercentage = healthpercentage;
					
//					_XDWINPRINT("ObjectUID:%d  Name:%s  ObjectType:%d  health:%d", objectid, m_pFunctionalObjectArray[i].m_ObjectName, objecttype, healthpercentage);
				}
				else
				{
					m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID = objectid;
					m_pFunctionalOBBDataArray[i].m_bCollidable = FALSE;					
					m_pFunctionalObjectArray[i].m_HealthPercentage = healthpercentage;
					
					if(!m_pFunctionalObjectArray[i].m_bDie)
					{
						if( m_pFunctionalObjectArray[i].m_ModelDescriptor.GetCurMotion() != g_MobMotionExistTable[m_pFunctionalObjectArray[i].m_ObjectType].motion_die2 )
						{
							m_pFunctionalObjectArray[i].SetDieAnimation();
							m_pFunctionalObjectArray[i].m_bDie = TRUE;
						}
					}
					
//					_XDWINPRINT("ObjectUID:%d  Name:%s  ObjectType:%d  health:%d", objectid, m_pFunctionalObjectArray[i].m_ObjectName, objecttype, healthpercentage);
				}	
				break;
			}
		}

		
/*		if( m_pFunctionalObjectArray[i].m_ObjectType == objecttype )
		{
			//g_Moblist.AddReferenceModel(objecttype);
	
			FLOAT dx = (m_pFunctionalObjectArray[i].m_matWorldPosition._41 - vObjectPosition.x);
			FLOAT dz = (m_pFunctionalObjectArray[i].m_matWorldPosition._43 - vObjectPosition.z);
			FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );

			if( _distance < 2.0f )
			{
				if( healthpercentage > 0 )
				{
					if( m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID == -1 )
					{
						m_pFunctionalObjectArray[i].m_ModelDescriptor.PlayAnimation(TRUE);
						m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID = objectid;
						m_pFunctionalOBBDataArray[i].m_bCollidable = TRUE;
//						m_pFunctionalObjectArray[i].m_bAttackable = FALSE;
						m_pFunctionalObjectArray[i].SetIdleAnimation();

						m_pFunctionalObjectArray[i].m_DieStartTime = 0;
						m_pFunctionalObjectArray[i].m_bDie = FALSE;
					}
					m_pFunctionalObjectArray[i].m_HealthPercentage = healthpercentage;

					_XDWINPRINT("ObjectUID:%d  Name:%s  ObjectType:%d  health:%d", objectid, m_pFunctionalObjectArray[i].m_ObjectName, objecttype, healthpercentage);
				}
				else
				{
					m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID = objectid;
					m_pFunctionalOBBDataArray[i].m_bCollidable = FALSE;					
					m_pFunctionalObjectArray[i].m_HealthPercentage = healthpercentage;

					if(!m_pFunctionalObjectArray[i].m_bDie)
						m_pFunctionalObjectArray[i].m_bDie = TRUE;

					_XDWINPRINT("ObjectUID:%d  Name:%s  ObjectType:%d  health:%d", objectid, m_pFunctionalObjectArray[i].m_ObjectName, objecttype, healthpercentage);
				}	
				break;
			}
		}*/
	}	
}

_XFunctionalObject* _XFunctionalObjectManager::FindAttackableFunctionalObject( unsigned short objectuniqueid )
{
	for( int i = 0; i < m_countFunctionalObject; ++i )
	{
		if( m_pFunctionalObjectArray[i].m_AttackalbeTypeUniqueID == objectuniqueid )
		{
			_XFunctionalObject* pFunctionalObject = &m_pFunctionalObjectArray[i];
			if(pFunctionalObject)
			{
				return pFunctionalObject;
			}
			else
			{
				return NULL;
			}
		}		
	}	
	return NULL;
}

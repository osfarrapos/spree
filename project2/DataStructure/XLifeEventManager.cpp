// XLifeEventManager.cpp: implementation of the _XLifeEventManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XLifeEventManager.h"

#include "XKernel.h"
#include "XAnimMeshContainer.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XLifeEventManager g_LifeEventManager;

_XLifeEventManager::_XLifeEventManager()
{
	// 남자 채집용
	m_nAniTable[0][0] = 469;
	m_nAniTable[0][1] = 470;
	m_nAniTable[0][2] = 471;
	m_nAniTable[0][3] = 472;

	// 남자 장작패기용
	m_nAniTable[0][4] = 473;
	m_nAniTable[0][5] = 474;
	m_nAniTable[0][6] = 475;
	
	// 남자 물긷기용
	m_nAniTable[0][7] = 476;
	m_nAniTable[0][8] = 477;
	m_nAniTable[0][9] = 478;
	m_nAniTable[0][10] = 479;
	m_nAniTable[0][11] = 480;
	m_nAniTable[0][12] = 481;

	m_nAniTable[1][0] = 438;
	m_nAniTable[1][1] = 439;
	m_nAniTable[1][2] = 440;
	m_nAniTable[1][3] = 441;

	m_nAniTable[1][4] = 442;
	m_nAniTable[1][5] = 443;
	m_nAniTable[1][6] = 444;

	m_nAniTable[1][7] = 445;
	m_nAniTable[1][8] = 446;
	m_nAniTable[1][9] = 447;
	m_nAniTable[1][10] = 448;
	m_nAniTable[1][11] = 449;
	m_nAniTable[1][12] = 450;

	for(int i = 0; i < LIFE_EVENT_ITEM_MAX_COUNT; ++ i)
	{
		for(int j = 0; j < 2; ++ j)
		{
			m_LifeEventItem[i][j] = NULL;
		}
	}

	m_nDirection = 1;
	m_fAxisTime = 0.f;
}

_XLifeEventManager::~_XLifeEventManager()
{
}

void _XLifeEventManager::DeleteData()
{
	for(int i = 0; i < LIFE_EVENT_ITEM_MAX_COUNT; ++ i)
	{
		for(int j = 0; j < 2; ++ j)
		{
			SAFE_DELETE( m_LifeEventItem[i][j] );
		}
	}
}

int _XLifeEventManager::GetaniIndex(int nGender, int nAniType)
{
	if (nGender >= 2) return -1;
	if (nAniType >= LIFE_EVENT_ANI_MAX_COUNT) return -1;
	return m_nAniTable[nGender][nAniType];
}

_XAnimMeshAbstractClass* _XLifeEventManager::GetEventItem(int nGender, int nIndex)
{
	if (nGender >= 2) return NULL;
	if (nIndex >= LIFE_EVENT_ITEM_MAX_COUNT) return NULL;

	return m_LifeEventItem[nGender][nIndex];
}

BOOL _XLifeEventManager::LoadLifeEventItem()
{
	if( !g_ModelArchive.ReOpenPackage() ) 
		return FALSE;

	if( !g_ScriptArchive.ReOpenPackage() )
		return FALSE;

	FILE* pScripterFile = NULL;
	
	pScripterFile = g_ScriptArchive.GetPackedFile( "CharactorModel.XMS" );

	LoadLifeEventItem(pScripterFile, &m_LifeEventItem[0][0], "m_item_tra_bc_handaxe.x3d");
	LoadLifeEventItem(pScripterFile, &m_LifeEventItem[1][0], "w_item_tra_bc_handaxe.x3d");

	LoadLifeEventItem(pScripterFile, &m_LifeEventItem[0][1], "M_item_tra_bc_hoe.x3d");
//	LoadLifeEventItem(pScripterFile, &m_LifeEventItem[1][1], "W_item_tra_bc_hoe.x3d");

	LoadLifeEventItem(pScripterFile, &m_LifeEventItem[0][2], "M_item_tra_bb_waterbowl.x3d");
	LoadLifeEventItem(pScripterFile, &m_LifeEventItem[1][2], "W_item_tra_bb_waterbowl.x3d");

	LoadLifeEventItem(pScripterFile, &m_LifeEventItem[0][3], "m_item_tra_bc_woodblock.x3d");
	LoadLifeEventItem(pScripterFile, &m_LifeEventItem[1][3], "w_item_tra_bc_woodblock.x3d");

	g_ScriptArchive.ClosePackage();
	
	g_ModelArchive.ClosePackage();

	return TRUE;
}

BOOL _XLifeEventManager::LoadLifeEventItem(FILE* pScripterFile, _XAnimMeshAbstractClass** ppMesh, LPSTR lpName)
{
	(*ppMesh) = _XAnimMeshAbstractClass::CreateAnimationMesh();

	if( !(*ppMesh)->Load(g_ModelArchive.GetPackedFile(lpName), 
							   &g_CharacterTextureArchive, TRUE, lpName ) )
	{
		return FALSE;
	}
	return TRUE;
}

void _XLifeEventManager::Process(float fTime)
{
	if (m_fAxisTime > 0.5f)
	{
		m_nDirection = -1;
	}
	else if (m_fAxisTime < -0.5f)
	{
		m_nDirection = 1;
	}
	m_fAxisTime += (fTime * m_nDirection);
}
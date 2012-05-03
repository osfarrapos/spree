// XLifeEventManager.h: interface for the _XLifeEventManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XLIFEEVENTMANAGER_H__4A07B13D_6E89_402D_A7C8_AAA19DE33D5E__INCLUDED_)
#define AFX_XLIFEEVENTMANAGER_H__4A07B13D_6E89_402D_A7C8_AAA19DE33D5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"

enum LIFE_EVENT_ANI
{
	LIFE_EVENT_ANI_COLLECT_COTTON = 0x00,
	LIFE_EVENT_ANI_COLLECT_TEA,
	LIFE_EVENT_ANI_COLLECT_MUSHROOM,
	LIFE_EVENT_ANI_COLLECT_TREE,

	LIFE_EVENT_ANI_FIREWOOD_READY,
	LIFE_EVENT_ANI_FIREWOOD_SUCCEED,
	LIFE_EVENT_ANI_FIREWOOD_FAIL,

	LIFE_EVENT_ANI_WATER_WAIT,
	LIFE_EVENT_ANI_WATER_IN,
	LIFE_EVENT_ANI_WATER_OUT,
	LIFE_EVENT_ANI_WATER_WALK_EMPTY,
	LIFE_EVENT_ANI_WATER_WALK_FULL,
	LIFE_EVENT_ANI_WATER_BLOW,

	LIFE_EVENT_ANI_MAX_COUNT
};

enum LIFE_EVENT_ITEM_TYPE
{
	LIFE_EVENT_ITEM_TYPE_AXE = 0,
	LIFE_EVENT_ITEM_TYPE_HOMI,
	LIFE_EVENT_ITEM_TYPE_BUCKER,
	LIFE_EVENT_ITEM_TYPE_FIREWOOD,
	
	LIFE_EVENT_ITEM_MAX_COUNT
};

class _XAnimMeshAbstractClass;

class _XLifeEventManager  
{
public:
	int				m_nAniTable[2][LIFE_EVENT_ANI_MAX_COUNT];


	_XAnimMeshAbstractClass*		m_LifeEventItem[2][LIFE_EVENT_ITEM_MAX_COUNT];

	D3DXMATRIX						m_matTubeWorld;
	D3DXMATRIX						m_matPointWorld;

	int								m_nDirection;
	float							m_fAxisTime;
public:
	_XLifeEventManager();
	virtual ~_XLifeEventManager();


	void	DeleteData();

	int		GetaniIndex(int nGender, int nAniType);

	_XAnimMeshAbstractClass* GetEventItem(int nGender, int nIndex);
	BOOL	LoadLifeEventItem();
	BOOL	LoadLifeEventItem(FILE* pScripterFile, _XAnimMeshAbstractClass** ppMesh, LPSTR lpName);

	void	Process(float fTime);
};

extern _XLifeEventManager g_LifeEventManager;

#endif // !defined(AFX_XLIFEEVENTMANAGER_H__4A07B13D_6E89_402D_A7C8_AAA19DE33D5E__INCLUDED_)

// xdummy.cpp: implementation of the _XDummy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xdummy.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XDummy::_XDummy()
{
	m_Position			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_GroupType			= _XGROUP_GAEBANG;
	m_RotateAngle		= 0.0f;

	m_bEastDir			= FALSE;
	m_bWestDir			= FALSE;
	m_bSouthDir			= FALSE;
	m_bNorthDir			= FALSE;

	m_bInsideFrustum	= TRUE;
	m_bImpact			= FALSE;	
}

_XDummy::~_XDummy()
{
}

void _XDummy::Create(unsigned short id, _XGROUPTYPE group, D3DXVECTOR3 position)
{
	m_DummyID		= id;
	m_Position		= position;

	m_ModelDescriptor.ReserveModelStack(1);
	m_ModelDescriptor.LinkBipedObject(&g_DUMMYBIPEDMODEL[group-1]);
	m_ModelDescriptor.SetMeshModel(0, g_DUMMYMODEL[group-1]);
	m_ModelDescriptor.SetCurMotion(0);
	m_ModelDescriptor.PlayAnimation();
	
	m_ModelDescriptor.InitEffectManager(1);

	UpdatePosition();
}

BOOL _XDummy::CheckInsideFrustum(void)
{
	FLOAT dx = (m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
	FLOAT dz = (m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
	m_ModelDescriptor.m_CameraDistance = _XFastDistance(dx, dz);

	m_bInsideFrustum = ( (m_ModelDescriptor.m_CameraDistance < _XDEF_MOB_CULLINGRADIUS) &&
		( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( m_Position, 1.0f ) != NOT_VISIBLE ) );

	return m_bInsideFrustum;
}

void _XDummy::Render(void)
{
	if(!m_bImpact)
		m_ModelDescriptor.Animation(0.0f);
	else
	{
		bool soundplay = true;
		m_ModelDescriptor.Advance( soundplay, 0 );
		m_ModelDescriptor.Animation();
	}

	BOOL intersect = FALSE;

	D3DXVECTOR3 nRayDir;
	D3DXVECTOR3 mpos = m_Position;
	mpos.y += m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1];
	D3DXVec3Normalize( &nRayDir, &g_vPickRayDir );
	
	// Check intersect sphere space...
	intersect = RayAndSphereIntersect( g_vPickRayOrig, nRayDir, mpos, m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius );
	
	// Check intersect obb
	if( intersect )
	{
		D3DXVECTOR3 PickedPoint;
		intersect = m_ModelDescriptor.InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, m_Position, m_RotateAngle, &PickedPoint );
		
		if( intersect )
		{	
			FLOAT dx = ( g_LodTerrain.m_3PCamera.m_CameraPosition.x - PickedPoint.x);
			FLOAT dz = ( g_LodTerrain.m_3PCamera.m_CameraPosition.z - PickedPoint.z);
			FLOAT _distance = _XFC_sqrt( dx*dx + dz*dz );
			
			if( _distance < g_LastPickedObjectDistance )
			{
				if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
				{
					g_LastPickedObjectDistance = _distance;
					g_LastPickedObjectType = _XPICKINGTARGET_DUMMY;
					g_LastPickedObject = this;
				}
			}
		}	
	}

	if(m_ModelDescriptor.m_CameraDistance < 12.0f)
	{
		m_ModelDescriptor.RenderShadow();
	}

	m_ModelDescriptor.Render();
	
	g_LastUsedRenderStyle = NULL;
}

void _XDummy::Process(void)
{
}

void _XDummy::SetAnimationIndex(int animationindex)
{
	m_ModelDescriptor.SetCurMotion(animationindex);
}

void _XDummy::UpdatePosition(void)
{
	D3DXMATRIX		rotmat;
	D3DXVECTOR3		terrainnormal;
	D3DXMatrixIdentity(&rotmat);
	D3DXMatrixRotationY(&rotmat, m_RotateAngle);
	m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(m_Position, &terrainnormal);
	D3DXMatrixTranslation(&m_ModelDescriptor.m_Position, m_Position.x, m_Position.y, m_Position.z);

	D3DXMatrixMultiply(&m_ModelDescriptor.m_Position, &rotmat, &m_ModelDescriptor.m_Position);
}



///////////////////////////////////////////////////////////////////////////////
// _XDummyList class
///////////////////////////////////////////////////////////////////////////////

_XDummyList::_XDummyList()
{
}

_XDummyList::~_XDummyList()
{
}

BOOL _XDummyList::Add(_XDummy* dummy)
{
	if(!insertItem(dummy))
		return FALSE;

	return TRUE;
}

_XDummy* _XDummyList::Find(unsigned short dummyid)
{
	if(listEmpty())
		return NULL;

	resetList();

	_XDummy* pDummy = NULL;

	do 
	{
		pDummy = (_XDummy*)currentItem();
		
		if(pDummy)
		{
			if(pDummy->m_DummyID == dummyid)
				return pDummy;
		}
		nextItem();
	} while(!atHeadOfList());

	return NULL;
}

void _XDummyList::Delete(unsigned short dummyid)
{
	if(listEmpty())
		return;

	resetList();

	_XDummy* pDummy = NULL;
	do 
	{
		pDummy = (_XDummy*)currentItem();
		
		if(pDummy)
		{
			if(pDummy->m_DummyID == dummyid)
			{
				Delete(pDummy);
				return;
			}
		}
		nextItem();
	} while(!atHeadOfList());
}

void _XDummyList::Delete(_XDummy* dummy)
{
	if(dummy)
	{
		if(dummy == (_XDummy*)g_LastPickedObject)
			g_LastPickedObject = NULL;

		if(dummy == (_XDummy*)g_pLocalUser->m_AttackTarget)
			g_pLocalUser->ResetAttackTarget();
		if(dummy == (_XDummy*)g_pLocalUser->m_ViewTarget)
			g_pLocalUser->ResetViewTarget();

		if (!g_Userlist.m_smUserList.empty())
		{
			_XUser* pUser;
			smdef_XUser::iterator it;
			for (it = g_Userlist.m_smUserList.begin(); it != g_Userlist.m_smUserList.end(); ++ it)
			{
				pUser = (*it).second;
				if(pUser->m_AttackTarget)
				{
					if(pUser->m_AttackTarget == dummy)
						pUser->ResetAttackTarget();
				}
			}
		}
		deleteItem(dummy);
	}
}

void _XDummyList::Process(void)
{
	// 필요하면 쓰기
}

void _XDummyList::Render(void)
{
	if(!listEmpty())
	{
		_XDummy* pDummy;
		resetList();
		do 
		{
			pDummy = (_XDummy*)currentItem();
			
			if(pDummy)
			{
				if(g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum(pDummy->m_Position, _XDEF_MOBCOLLIDEFRUSTUM_DEFAULT) != NOT_VISIBLE)
				{
					if(pDummy->m_ModelDescriptor.m_CameraDistance < _XDEF_USER_CULLINGRADIUS)
					{
						pDummy->Render();
					}
				}
			}
			else
				break;

			nextItem();
		} while(!atHeadOfList());
	}
}

void _XDummyList::CheckInsideFrustum(void)
{
	if(listEmpty()) 
		return;
	
	resetList();
	
	_XDummy* pDummy = NULL;
	do
	{
		pDummy = (_XDummy*)currentItem();
		
		if(pDummy)
		{		
			FLOAT dx = (pDummy->m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
			FLOAT dz = (pDummy->m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
			pDummy->m_ModelDescriptor.m_CameraDistance = _XDistance(dx , dz);
			
			pDummy->m_bInsideFrustum = 
				( (pDummy->m_ModelDescriptor.m_CameraDistance < _XDEF_USER_CULLINGRADIUS) &&
				( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pDummy->m_Position, 2.0f ) != NOT_VISIBLE ) );
		}
		
		nextItem();
	}while(!atHeadOfList());
}

void _XDummyList::RemoveEffectInstance(int templateid)
{
	if(listEmpty())
		return;

	resetList();

	_XDummy* pDummy = NULL;
	do 
	{
		pDummy = (_XDummy*)currentItem();
		
		if(pDummy)
		{
			if(pDummy->m_ModelDescriptor.m_EffectManager)
				pDummy->m_ModelDescriptor.m_EffectManager->DeleteInstance(templateid);
		}

		nextItem();
	} while(!atHeadOfList());
}
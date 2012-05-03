// xseat.cpp: implementation of the _XSeat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xseat.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XSeat::_XSeat()
{
	m_Position			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_GroupType			= _XGROUP_GAEBANG;
	m_bInsideFrustum	= FALSE;
	m_pMeshContainer	= NULL;
	m_CameraDistance	= 0.0f;
}

_XSeat::~_XSeat()
{

}

void _XSeat::Create(unsigned short id, _XGROUPTYPE group, D3DXVECTOR3 position, _XAnimMeshAbstractClass* pMeshContainer)
{
	m_pMeshContainer	= pMeshContainer;

	m_SeatID		= id;
	m_Position		= position;
}

void _XSeat::Render(void)
{
	BOOL intersect = FALSE;
	
	D3DXVECTOR3 nRayDir;
	D3DXVECTOR3 mpos = m_Position;
//	mpos.y += m_pMeshContainer->OBB_Extent[1];
	D3DXVec3Normalize( &nRayDir, &g_vPickRayDir );
	
	// Check intersect sphere space...
	intersect = RayAndSphereIntersect( g_vPickRayOrig, nRayDir, mpos, m_pMeshContainer->m_ObjectRadius );
	
	// Check intersect obb
	if( intersect )
	{
		D3DXVECTOR3 PickedPoint;
		intersect = m_pMeshContainer->InterOBBvsRay( g_vPickRayOrig, g_vPickInfinityRayDir, mpos, &PickedPoint );
		
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
					g_LastPickedObjectType = _XPICKINGTARGET_SEAT;
					g_LastPickedObject = this;
				}
			}
		}	
	}	
	m_pMeshContainer->Render_OriginalPos(mpos, 0, NULL);
}

///////////////////////////////////////////////////////////////////////////////
// _XSeatList class
///////////////////////////////////////////////////////////////////////////////

_XSeatList::_XSeatList()
{
}

_XSeatList::~_XSeatList()
{
}

BOOL _XSeatList::Add(_XSeat* seat)
{
	if(!insertItem(seat))
		return FALSE;

	return TRUE;
}

_XSeat* _XSeatList::Find(unsigned short id)
{
	if(listEmpty())
		return NULL;

	resetList();

	_XSeat* pSeat = NULL;

	do 
	{
		pSeat = (_XSeat*)currentItem();
		
		if(pSeat)
		{
			if(pSeat->m_SeatID == id)
				return pSeat;
		}
		nextItem();
	} while(!atHeadOfList());

	return NULL;
}

void _XSeatList::Delete(unsigned short id)
{
	if(listEmpty())
		return;

	resetList();

	_XSeat* pSeat = NULL;
	do 
	{
		pSeat = (_XSeat*)currentItem();

		if(pSeat)
		{
			if(pSeat->m_SeatID == id)
			{
				Delete(pSeat);
				return;
			}
		}
		nextItem();
	} while(!atHeadOfList());
}

void _XSeatList::Delete(_XSeat* seat)
{
	if(seat)
	{
		if(seat == (_XSeat*)g_LastPickedObject)
			g_LastPickedObject = NULL;

		if(seat == (_XSeat*)g_pLocalUser->m_AttackTarget)
			g_pLocalUser->ResetAttackTarget();
		if(seat == (_XSeat*)g_pLocalUser->m_ViewTarget)
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
					if(pUser->m_AttackTarget == seat)
						pUser->ResetAttackTarget();
				}
			}
		}

		deleteItem(seat);
	}
}

void _XSeatList::Render(void)
{
	if(!listEmpty())
	{
		_XSeat* pSeat;
		resetList();
		do 
		{
			pSeat = (_XSeat*)currentItem();

			if(pSeat)
			{
				if(g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum(pSeat->m_Position, _XDEF_MOBCOLLIDEFRUSTUM_DEFAULT) != NOT_VISIBLE)
				{
					if(pSeat->m_CameraDistance < _XDEF_USER_CULLINGRADIUS)
						pSeat->Render();
				}
			}
			else
				 break;
			nextItem();
		} while(!atHeadOfList());
	}
}

void _XSeatList::CheckInsideFrustum(void)
{
	if(listEmpty())
		return;

	resetList();

	_XSeat* pSeat = NULL;
	do 
	{
		pSeat = (_XSeat*)currentItem();
		
		if(pSeat)
		{
			FLOAT dx = (pSeat->m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
			FLOAT dz = (pSeat->m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
			pSeat->m_CameraDistance = _XDistance(dx , dz);
			
			pSeat->m_bInsideFrustum = 
				(g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pSeat->m_Position, 2.0f ) != NOT_VISIBLE );
		}
		nextItem();
	} while(!atHeadOfList());
}
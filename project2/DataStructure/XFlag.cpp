// XFlag.cpp: implementation of the _XFlag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XFlag.h"
#include "EODEXTDEF_GlobalObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFlag::_XFlag()
{
	m_Position			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_FlagType				= 0;
	m_FlagID			= 0;
	m_RotateAngle		= 0.0f;
	
	m_bInsideFrustum	= FALSE;

	m_MatchType			= 0;
	m_Applicant1		= 0;
	m_Applicant2		= 0;
	
	m_LastMessageReceiveTime = 0;
}

_XFlag::~_XFlag()
{

}

void _XFlag::Create(unsigned short id, char type, D3DXVECTOR3 position)
{
	m_FlagID		= id;
	m_FlagType			= type;
	m_Position		= position;

	m_ModelDescriptor.ReserveModelStack(1);
	m_ModelDescriptor.LinkBipedObject(&g_FLAGBIPEDMODEL);
	m_ModelDescriptor.SetMeshModel(0, g_FLAGMODEL);
	m_ModelDescriptor.SetCurMotion(0);
	m_ModelDescriptor.PlayAnimation();

	m_ModelDescriptor.InitEffectManager(1);
	
	UpdatePosition();
}

BOOL _XFlag::CheckInsideFrustum(void)
{
	FLOAT dx = (m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
	FLOAT dz = (m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
	m_ModelDescriptor.m_CameraDistance = _XFastDistance(dx, dz);

	m_bInsideFrustum = ((m_ModelDescriptor.m_CameraDistance < _XDEF_MOB_CULLINGRADIUS) &&
		(g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum(m_Position, 1.0f) != NOT_VISIBLE));

	return m_bInsideFrustum;
}

void _XFlag::Render(void)
{
	bool soundplay = true;
	m_ModelDescriptor.Advance(soundplay, 0);
	m_ModelDescriptor.Animation();

	BOOL intersect = FALSE;

	D3DXVECTOR3 nRayDir;
	D3DXVECTOR3 mpos = m_Position;
	mpos.y += m_ModelDescriptor.m_ppModelList[0]->OBB_Extent[1];
	D3DXVec3Normalize(&nRayDir, &g_vPickRayDir);

	// Check intersect sphere space...
	intersect = RayAndSphereIntersect( g_vPickRayOrig, nRayDir, mpos, m_ModelDescriptor.m_ppModelList[0]->m_ObjectRadius );
	
	// Check intersect obb
	if(intersect)
	{
		D3DXVECTOR3 PickedPoint;
		intersect = m_ModelDescriptor.InterOBBvsRay(g_vPickRayOrig, g_vPickInfinityRayDir, m_Position, m_RotateAngle, &PickedPoint);
		
		if(intersect)
		{	
			FLOAT dx = (g_LodTerrain.m_3PCamera.m_CameraPosition.x - PickedPoint.x);
			FLOAT dz = (g_LodTerrain.m_3PCamera.m_CameraPosition.z - PickedPoint.z);
			FLOAT _distance = _XFC_sqrt(dx*dx + dz*dz);
			
			if(_distance < g_LastPickedObjectDistance)
			{
				if(!g_CurrentFocusedObject || g_CurrentFocusedObject == _XDEF_DUMMYID_GAMEBACKGROUND)
				{
					g_LastPickedObjectDistance = _distance;
					g_LastPickedObjectType = _XPICKINGTARGET_FLAG;
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

void _XFlag::Process(void)
{

}

void _XFlag::UpdatePosition(void)
{
	D3DXMATRIX rotmat;
	D3DXVECTOR3 terrainnormal;

	D3DXMatrixIdentity(&rotmat);
	D3DXMatrixRotationY(&rotmat, m_RotateAngle);

	m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight(m_Position, &terrainnormal);
	D3DXMatrixTranslation(&m_ModelDescriptor.m_Position, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixMultiply(&m_ModelDescriptor.m_Position, &rotmat, &m_ModelDescriptor.m_Position);
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XFlagList::_XFlagList()
{

}

_XFlagList::~_XFlagList()
{

}

BOOL _XFlagList::Add(_XFlag* flag)
{
	if(!insertItem(flag))
		return FALSE;

	return TRUE;
}

_XFlag* _XFlagList::Find(unsigned short flagid)
{
	if(listEmpty())
		return NULL;

	resetList();

	_XFlag* pFlag = NULL;

	do 
	{
		pFlag = (_XFlag*)currentItem();
		
		if(pFlag)
		{
			if(pFlag->m_FlagID == flagid)
				return pFlag;
		}
		nextItem();
	} while(!atHeadOfList());

	return NULL;
}

void _XFlagList::Delete(unsigned short flagid)
{
	if(listEmpty())
		return;

	resetList();

	_XFlag* pFlag = NULL;
	do 
	{
		pFlag = (_XFlag*)currentItem();
		
		if(pFlag)
		{
			if(pFlag->m_FlagID == flagid)
			{
				Delete(pFlag);
				return;
			}
		}
		nextItem();
	} while(!atHeadOfList());
}

void _XFlagList::Delete(_XFlag* flag)
{
	if(flag)
	{
		if(flag == (_XFlag*)g_LastPickedObject)
			g_LastPickedObject = NULL;

		if(flag == (_XFlag*)g_pLocalUser->m_AttackTarget)
			g_pLocalUser->ResetAttackTarget();
		if(flag == (_XFlag*)g_pLocalUser->m_ViewTarget)
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
					if(pUser->m_AttackTarget == flag)
						pUser->ResetAttackTarget();
				}
			}
		}

		deleteItem(flag);
	}
}

void _XFlagList::Process(void)
{
	if(!listEmpty())
	{
		_XFlag* pFlag;
		resetList();

		do 
		{
			pFlag = (_XFlag*)currentItem();
			
			if(pFlag)
			{
				if((g_LocalSystemTime - pFlag->m_LastMessageReceiveTime) > _XDEF_FLAG_DELETETIME)
				{
					Delete(pFlag);
				}
				else
				{
					nextItem();
				}
			}
			else
				break;

			if(listEmpty())
				break;

		} while(!atHeadOfList());
	}
}

void _XFlagList::Render(void)
{
	if(!listEmpty())
	{
		_XFlag* pFlag;
		resetList();
		do 
		{
			pFlag = (_XFlag*)currentItem();
			
			if(pFlag)
			{
				if(g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum(pFlag->m_Position, _XDEF_MOBCOLLIDEFRUSTUM_DEFAULT) != NOT_VISIBLE)
				{
					if(pFlag->m_ModelDescriptor.m_CameraDistance < _XDEF_USER_CULLINGRADIUS)
					{
						pFlag->Render();
					}
				}
			}
			else
				break;

			nextItem();
		} while(!atHeadOfList());
	}
}

void _XFlagList::CheckInsideFrustum(void)
{
	if(listEmpty()) 
		return;
	
	resetList();
	
	_XFlag* pFlag = NULL;
	do
	{
		pFlag = (_XFlag*)currentItem();
		
		if(pFlag)
		{		
			FLOAT dx = (pFlag->m_Position.x - g_LodTerrain.m_3PCamera.m_CameraPosition.x);
			FLOAT dz = (pFlag->m_Position.z - g_LodTerrain.m_3PCamera.m_CameraPosition.z);
			pFlag->m_ModelDescriptor.m_CameraDistance = _XDistance(dx , dz);
			
			pFlag->m_bInsideFrustum = 
				( (pFlag->m_ModelDescriptor.m_CameraDistance < _XDEF_USER_CULLINGRADIUS) &&
				( g_LodTerrain.m_ObjectQuadTree.CheckObjectAgainstCurrrentFrustum( pFlag->m_Position, 2.0f ) != NOT_VISIBLE ) );
		}
		
		nextItem();
	}while(!atHeadOfList());
}

void _XFlagList::RemoveEffectInstance(int templateid)
{
	if(listEmpty())
		return;

	resetList();

	_XFlag* pFlag = NULL;
	do 
	{
		pFlag = (_XFlag*)currentItem();
		
		if(pFlag)
		{
			if(pFlag->m_ModelDescriptor.m_EffectManager)
				pFlag->m_ModelDescriptor.m_EffectManager->DeleteInstance(templateid);
		}

		nextItem();
	} while(!atHeadOfList());
}
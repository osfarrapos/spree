// X3PCamera.cpp: implementation of the _X3PCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "X3PCamera.h"
#include "XLodTerrain.h"
#include "EODEXTDEF_GlobalObject.h"
#include "SoundEffectList.h"


class _XLinearGraph
{
private:
	float a, b;
	
public:
	_XLinearGraph( float na, float nb ) :a( na ), b( nb ) {}	
	_XLinearGraph( float x0, float y0, float x1, float y1 )
	{
		// Calculate a and b from two points
		a = ( y1 - y0 ) / ( x1 - x0 );
		b = -a * x0 + y0;
	}
	
	virtual float GetValueAt( float x )
	{
		return a*x + b;
	}
};

/*
void MyCamera::Update(DWORD uiDelta)
{
	   m_vEyeTarget += (m_vEye - m_vEyeTarget) * min(uiDelta/250.0f, 1.0f);
	   m_vLookatTarget += (m_vLookat - m_vLookatTarget) * min(uiDelta/200.0f, 1.0f);
	   
	   D3DXMatrixLookAtLH( &m_matView, &m_vEyeTarget, &m_vLookatTarget, &m_vUp);
	   D3DXMatrixMultiply(&m_matViewProj, &m_matView, &m_matProj);
	   
	   D3DXMatrixInverse( &m_matInvView, NULL, &m_matView );
	   m_matBill = m_matInvView;
	   m_matBill._41 = 0.0f;
	   m_matBill._42 = 0.0f;
	   m_matBill._43 = 0.0f;
	   
	   UpdateFrustum();
}

*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_X3PCamera::_X3PCamera()
{
	Initialize();	
}

_X3PCamera::~_X3PCamera()
{

}


void _X3PCamera::Initialize( void )
{
	m_MinDistance					= 0.1f;
	m_MaxDistance					= 120.0f;
	m_TargetDistance				= 5.0f;
	m_CurrentTargetDistance			= 5.0f;
	m_AdditionalHeightMinDistance	= 0.0f;
	m_EnableFirstPersonView			= TRUE;
	
	m_MoveVector = m_TargetPosition = m_CameraPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_LookVector	= D3DXVECTOR3(0.0, 0.0, 1.0);
	m_UpVector		= D3DXVECTOR3(0.0, 1.0, 0.0);
	m_RightVector	= D3DXVECTOR3(1.0, 0.0, 0.0);
		
	mp_fYaw = mp_fPitch = mp_fRoll = 0.0f;

	m_CameraVelocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
	m_ZoominoutVelocity	=	 0.f;
	mp_fMinPitchLimit	=	-10.f;
	mp_fMaxPitchLimit	=	 89.99f;
	mp_fMinRollLimit	=	 0.f;
	mp_fMaxRollLimit	=	 0.f;

	D3DXMatrixIdentity(&mp_view_matrix);
	D3DXMatrixIdentity(&mp_proj_matrix);
	D3DXMatrixIdentity(&mp_viewproj_matrix);

	mp_fFov			= g_DefaultCameraFov;
	mp_fAspect		= _XDEF_CAMERA_DEFAULTASPECT;	
	mp_fNearPlane	= _XDEF_CAMERA_DEFAULTNEARPLANE;
	mp_fFarPlane	= _XDEF_CAMERA_DEFAULTFARPLANE;	

	m_ImpactValue   = 0.0f;
	
	m_AutoBackTrace = FALSE;
	m_CameraShakeMode = FALSE;
	m_CameraShakeDelayMode	= FALSE;
	m_ChangeCameraShakeAtDelayMode = FALSE;
	m_CameraShakeFactor = D3DXVECTOR3( 0.0f,0.0f,0.0f );
	m_fCameraShakeTimer = 0.0f;	
	
	m_QuaterViewMode = FALSE;
	m_QuaterViewChanging = FALSE;
		
	m_DefaultViewChanging = FALSE;
	
}


void _X3PCamera::Set(FLOAT fov, FLOAT aspect, FLOAT nearplane, FLOAT farplane)
{
	mp_fFov = fov;
	mp_fAspect = aspect;
	mp_fNearPlane = nearplane;
	mp_fFarPlane = farplane;
}

void _X3PCamera::Set(FLOAT pos_x, FLOAT pos_y, FLOAT pos_z)
{
	m_CameraPosition.x = pos_x;
	m_CameraPosition.y = pos_y;
	m_CameraPosition.z = pos_z;
}

void _X3PCamera::SetPitchLimits(FLOAT min, FLOAT max)
{
	mp_fMinPitchLimit = min;
	mp_fMaxPitchLimit = max;
}

void _X3PCamera::SetRollLimits(FLOAT min, FLOAT max)
{
	mp_fMinRollLimit = min;
	mp_fMaxRollLimit = max;
}

void _X3PCamera::SetDistance( FLOAT radius )
{ 
	FLOAT fprevDistance = m_TargetDistance;

	if( m_MinDistance > radius )
		m_TargetDistance = m_MinDistance; 
	else if( m_MaxDistance < radius )
		m_TargetDistance = m_MaxDistance; 
	else 
		m_TargetDistance = radius;

	m_ZoominoutVelocity = m_TargetDistance - fprevDistance;
}

void _X3PCamera::SetDistanceLimits( FLOAT min, FLOAT max )
{
	m_MinDistance = min( min, max );
	m_MaxDistance = max( min, max );
}

void _X3PCamera::AddYaw(FLOAT a)
{
	mp_fYaw += a;
	if( mp_fYaw > 360.0f )
		mp_fYaw = (FLOAT)fmod(mp_fYaw, 360.0);

	m_CameraVelocity.x = a;
}

void _X3PCamera::AddPitch(FLOAT a)
{
	if( m_QuaterViewMode ) return;
	
	mp_fPitch += a;
	
	if(mp_fPitch < mp_fMinPitchLimit)
		mp_fPitch = mp_fMinPitchLimit;
	else if(mp_fPitch > mp_fMaxPitchLimit)
		mp_fPitch = mp_fMaxPitchLimit;

	m_CameraVelocity.y = a;
}

void _X3PCamera::AddRoll(FLOAT a)
{
	if( m_QuaterViewMode ) return;

	mp_fRoll += a;

	if(mp_fRoll < mp_fMinRollLimit)
		mp_fRoll = mp_fMinRollLimit;
	else if(mp_fRoll > mp_fMaxRollLimit)
		mp_fRoll = mp_fMaxRollLimit;

	m_CameraVelocity.z = a;
}

void _X3PCamera::FindCameraPosition2D()
{
	FLOAT beforepitchx, beforepitchy, beforepitchz;
	FLOAT afterpitchx, afterpitchy, afterpitchz;
	FLOAT newcamerax, newcameray, newcameraz;
	
	// 시작 위치 -  Target position보다 z축쪽으로 m_TargetDistance만큼 떨어진 곳 (캐릭터의 뒤)
	// Target Position을 기준으로 회전해야 하므로, Target Position을 (0,0,0)으로 옮긴 것이다.  
	beforepitchx = 0;
	beforepitchy = 0;
	beforepitchz = m_TargetDistance;
	
	// 회전 순서는 Pitch -> Yaw이다. 카메라를 회전할 때에는 항상 이 순서를 지켜야 한다.
	
	// Y-Z plane에서 mp_fPitch만큼 회전 (x 값은 영향을 받지 않는다.)
	afterpitchx = beforepitchx;
	afterpitchy = cos(_X_RAD(mp_fPitch))*beforepitchy + sin(_X_RAD(mp_fPitch))*beforepitchz;
	afterpitchz = -sin(_X_RAD(mp_fPitch))*beforepitchy + cos(_X_RAD(mp_fPitch))*beforepitchz;
	
	// X-Z plane에서 mp_fYaw만큼 회전 (y값은 영향을 받지 않는다,)
	newcamerax = cos(_X_RAD(mp_fYaw))*afterpitchx + sin(_X_RAD(mp_fYaw))*afterpitchz;
	newcameray = afterpitchy;
	newcameraz = -sin(_X_RAD(mp_fYaw))*afterpitchx + cos(_X_RAD(mp_fYaw))*afterpitchz;
	
	// Target position을 (0,0,0)에서 다시 제자리로 옮긴것.
	m_CameraPosition.x = newcamerax + m_TargetPosition.x;
	m_CameraPosition.y = newcameray + m_TargetPosition.y;
	m_CameraPosition.z = newcameraz + m_TargetPosition.z;
	
//#define _XDPRINTCAMERAINFO
#ifdef _XDPRINTCAMERAINFO
	_XDWINPRINT("camera [after yaw] ===============================");
	_XDWINPRINT("x : %f     y : %f     z : %f", m_CameraPosition.x, m_CameraPosition.y, m_CameraPosition.z);
	_XDWINPRINT("mp_fPitch : %f    mp_fYaw : %f", mp_fPitch, mp_fYaw);
	_XDWINPRINT("distance : %f", m_TargetDistance);
	_XDWINPRINT("====================================================="); 
#endif	
}

void _X3PCamera::UpdateViewMatrix( _XLodTerrain* terrainobject, BOOL setexternproperties )
{
	if(!g_QuestScriptManager.GetQuestCameraMode())
	{
		FindCameraPosition2D();
		
		if( terrainobject )
		{			
			POINT		pt;
			S_WATER*	curwater;
			_XLODTILE*  curtile;
			FLOAT		height = terrainobject->GetTerrainHeight( m_CameraPosition );
			
			curtile		= terrainobject->GetTerrainTile( m_CameraPosition, &pt );
			curwater	= terrainobject->m_pWaterObjectManager->GetWaterObject( pt.y + (pt.x * g_LodTerrain.m_TileCount) );

			if( curwater && curwater->enabled )
			{
				if( height < curwater->height )
				{
					height = curwater->height;
				}				
			}			
			
			height += _XDEF_BASEHEIGHTOFTERRAIN / 1.5f;
			
			if( height > m_CameraPosition.y ) m_CameraPosition.y = height;
								
			// Check OBB
			D3DXVECTOR3 LastCollidePoint;
			FLOAT		collidedist = 9999.0f;
			D3DXVECTOR3 camerainvviewvector;
			BOOL		bCollide = FALSE;

			camerainvviewvector = m_CameraPosition - m_TargetPosition;
			D3DXVec3Normalize( &camerainvviewvector, &camerainvviewvector );
			camerainvviewvector *= (m_TargetDistance * 2.0f);

			if( !g_FreeMoveMode && curtile )
			{
				if( !curtile->svObbList.empty() )
				{
					_XOBBData* pOBBData;
					svdef_OBBData::iterator it;
					for (it = curtile->svObbList.begin(); it != curtile->svObbList.end(); ++ it)
					{
						pOBBData = (*it);
						D3DXVECTOR3 CollidePoint;
						if( pOBBData->InterOBBvsRay( m_TargetPosition, camerainvviewvector, CollidePoint ) )
						{
							D3DXVECTOR3 localcollidepoint;
							localcollidepoint = CollidePoint - m_TargetPosition;
							FLOAT dist = D3DXVec3LengthSq( &localcollidepoint );
							if( dist < collidedist )
							{
								if( dist < m_TargetDistance*m_TargetDistance )
								{
									collidedist = dist;
									LastCollidePoint = CollidePoint;									
									bCollide = TRUE;
								}
							}
						}
					}
				}
			}	
			
			if( !g_FreeMoveMode && !bCollide )
			{
				if( g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject != 0 )
				{
					for( int i = 0; i < g_LodTerrain.m_FunctionalObjectManager.m_countFunctionalObject; i++ )
					{
						if( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].m_bCollidable )// 충돌 가능할 때만 충돌 처리를 한다. ( 문이 부서지면 충돌처리를 하지 않는다.)
						{
							D3DXVECTOR3 CollidePoint;
							if( g_LodTerrain.m_FunctionalObjectManager.m_pFunctionalOBBDataArray[i].InterOBBvsRay( m_TargetPosition, camerainvviewvector, CollidePoint ) )
							{
								D3DXVECTOR3 localcollidepoint;
								localcollidepoint = CollidePoint - m_TargetPosition;
								FLOAT dist = D3DXVec3LengthSq( &localcollidepoint );
								if( dist < collidedist )
								{
									if( dist < m_TargetDistance*m_TargetDistance )
									{
										collidedist = dist;
										LastCollidePoint = CollidePoint;									
										bCollide = TRUE;
									}
								}
							}
						}
					}
				}
			}
			
			if( bCollide )
			{
				if( collidedist < m_MinDistance )
				{
					camerainvviewvector = LastCollidePoint - m_TargetPosition;
					D3DXVec3Normalize( &camerainvviewvector, &camerainvviewvector );
					m_CameraPosition = m_TargetPosition + (camerainvviewvector * m_MinDistance);
				}
				else
				{					
					m_CameraPosition = LastCollidePoint;
				}
			}
		}
	}

	D3DXVECTOR3 distvector = m_CameraPosition - m_TargetPosition;
	//distvector.y = 1.0f;
	m_CurrentTargetDistance = D3DXVec3Length( &distvector );

	// 카메라가 너무 가까이 붙었으면 타켓을 높이 설정해 하늘을 볼 수 있도록 설정.
	D3DXVECTOR3 targetposition = m_TargetPosition;

	if( !g_FreeMoveMode && m_EnableFirstPersonView )
	{
		if( m_CurrentTargetDistance < m_MinDistance+m_AdditionalHeightMinDistance )
		{
			targetposition.y += 0.1f;
		}
	}	
	
	m_LookVector = targetposition - m_CameraPosition;	
	D3DXVec3Normalize( &m_LookVector, &m_LookVector );	
	D3DXVec3Cross(&m_RightVector, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &m_LookVector);
	D3DXVec3Normalize(&m_RightVector, &m_RightVector);

	D3DXVECTOR3 newCamPos, newTargetPos;
	newCamPos	= m_CameraPosition;
	newTargetPos= targetposition;

	newCamPos.x += m_CameraShakeFactor.x;
	newCamPos.y += m_CameraShakeFactor.y;
	newCamPos.z += m_CameraShakeFactor.z;

	newTargetPos.x += m_CameraShakeFactor.x;
	newTargetPos.y += m_CameraShakeFactor.y;
	newTargetPos.z += m_CameraShakeFactor.z;

	D3DXMatrixLookAtLH( &mp_view_matrix, &newCamPos, &newTargetPos, &D3DXVECTOR3(0.0, 1.0, 0.0) );	

	// Copy core view matrix
	if( setexternproperties )
	{		
		g_ViewMatrix = mp_view_matrix;

#ifdef _XUSEFMOD
		
#ifdef _XDWDEBUG	
		extern BOOL	g_3DSoundTargettype;
		if( g_3DSoundTargettype )
		{
			g_FMODWrapper.SetListenerAttribute( m_CameraPosition, m_LookVector, m_UpVector );
		}
		else
		{
			g_FMODWrapper.SetListenerAttribute( m_TargetPosition, m_LookVector, m_UpVector );
		}
#else			
		g_FMODWrapper.SetListenerAttribute( m_CameraPosition, m_LookVector, m_UpVector );
		//g_FMODWrapper.SetListenerAttribute( m_TargetPosition, m_LookVector, m_UpVector );
#endif

#else
		AUDIOVECTOR newposition;
		newposition.x = m_CameraPosition.x;
		newposition.y = m_CameraPosition.y;
		newposition.z = m_CameraPosition.z;

		/*newposition.x = g_pLocalUser->m_Position.x;
		newposition.y = g_pLocalUser->m_Position.y;
		newposition.z = g_pLocalUser->m_Position.z;*/
	
		g_XAudioSystem.m_pListener->SetPosition( newposition );
		g_XAudioSystem.m_pListener->SetOrientation( m_LookVector, m_UpVector );
#endif
		if(g_pLocalUser)
			g_pLocalUser->m_ModelDescriptor.m_CameraDistance = m_CurrentTargetDistance;
	}
}

void _X3PCamera::UpdateProjMatrix()
{
	D3DXMatrixPerspectiveFovLH(&mp_proj_matrix, _X_RAD(mp_fFov), mp_fAspect, mp_fNearPlane, mp_fFarPlane);
	// Copy core view matrix
	g_ProjectionMatrix = mp_proj_matrix;
}

void _X3PCamera::UpdateViewprojMatrix()
{
	mp_viewproj_matrix = mp_view_matrix * mp_proj_matrix;
}

void _X3PCamera::CheckHeight( FLOAT y )
{
	if( y > m_CameraPosition.y ) m_CameraPosition.y = y;
}

FLOAT _X3PCamera::GetRotateRadian( POINT mousepos, D3DXVECTOR3 pos )
{
	FLOAT			x;
	FLOAT			z;	
	FLOAT			fRadian	= 0.0f;
	FLOAT			thetha;				//빗변
	
	
	D3DXVec3TransformCoord(&pos, &pos, &mp_view_matrix);
	FLOAT foriginx = pos.x / pos.z;
	FLOAT foriginy = pos.y / pos.z;
						  
	// Compute the vector of the pick ray in screen space
	// 마우스의 좌표를 -1 ~ 1사이로 수정 y는 윈도우좌표가 +- 가 바뀌어있기때문에 바꾼다.
	// 원점에서 깊이가 1인 곳까지의 방향 벡터..	
	x =  (( (FLOAT)(mousepos.x << 1) / (FLOAT)gnWidth  ) - 1.0f) / mp_proj_matrix._11;
	z = -(( (FLOAT)(mousepos.y << 1) / (FLOAT)gnHeight ) - 1.0f) / mp_proj_matrix._22;

	x -= foriginx;
	z -= foriginy;

	//_XDWINPRINT( "%f %f\n", x, z );
		
	//tan세타  = 높이  / 밑변 에서 
	//float theta = acosf( y / hypotenuse );
	// 1라디안은 0.01745도 
	if(x>0.0f && z>0.0f ) // 뷰좌표계의 임이의 1사 분면
	{
		fRadian = atanf( x / z );//*3.14f/180.0f);
	}
	else if( x < 0.0f && z > 0.0f) // 뷰좌표계의 임이의 2사 분면
	{
		fRadian = ( atanf( x / z ) );// * -1.0f;//*3.14f/180.0f);
	}
	else if( x < 0.0f && z < 0.0f) // 뷰좌표계의 임이의 3사 분면
	{
		thetha =  atanf( x / z ) ; // + 1.5705f ) * -1.0f;//*3.14f/180.0f);
		fRadian = thetha - _X_PI;
	}
	else if( x > 0.0f && z < 0.0f) // 뷰좌표계의 임이의 3사 분면
	{
		thetha	=  atanf( x / z ) ;
		fRadian =  _X_PI + thetha ;
	}	
	
	fRadian += _X_RAD( mp_fYaw );

	return fRadian;
}

#define _XDEF_CAMERADECREASERATE		0.76f //0.83f
#define _XDEF_CAMERAZOOMDECREASERATE	0.78f

void _X3PCamera::Advance( void )
{
	BOOL changedcamerastatus = FALSE;

	FLOAT abscamvel_x = fabs(m_CameraVelocity.x);
	FLOAT abscamvel_y = fabs(m_CameraVelocity.y);
	FLOAT abscamvel_z = fabs(m_CameraVelocity.z);
	FLOAT abscamvel_dist = fabs(m_ZoominoutVelocity);

	if( abscamvel_x > EPSILON3 || abscamvel_y > EPSILON3 || abscamvel_z > EPSILON3 || abscamvel_dist > EPSILON3 )
	{
		if( abscamvel_x > EPSILON3 )
		{
			m_CameraVelocity.x *= _XDEF_CAMERADECREASERATE;
			
			// add yaw
			mp_fYaw += m_CameraVelocity.x;
			if( mp_fYaw > 360.0f ) mp_fYaw = (FLOAT)fmod(mp_fYaw, 360.0);
		}

		if( abscamvel_y > EPSILON3 )
		{
			m_CameraVelocity.y *= _XDEF_CAMERADECREASERATE;
			
			//add pitch
			mp_fPitch += m_CameraVelocity.y;			
			if(mp_fPitch < mp_fMinPitchLimit) mp_fPitch = mp_fMinPitchLimit;
			else if(mp_fPitch > mp_fMaxPitchLimit) mp_fPitch = mp_fMaxPitchLimit;
		}

		if( abscamvel_z > EPSILON3 )
		{
			m_CameraVelocity.z *= _XDEF_CAMERADECREASERATE;
			
			//add roll
			mp_fRoll += m_CameraVelocity.z;			
			if(mp_fRoll < mp_fMinRollLimit) mp_fRoll = mp_fMinRollLimit;
			else if(mp_fRoll > mp_fMaxRollLimit) mp_fRoll = mp_fMaxRollLimit;
		}

		if( abscamvel_dist > EPSILON3 )
		{

#ifdef _XDWDEBUG
			extern BOOL g_MouseLockFlag;
			if( !g_MouseLockFlag )
			{
#endif
			if( m_MinDistance + m_AdditionalHeightMinDistance < m_TargetDistance )
			{
				m_ZoominoutVelocity *= _XDEF_CAMERAZOOMDECREASERATE;
			}
			else
			{
				m_ZoominoutVelocity *= 0.3f;
			}
			
			m_TargetDistance += m_ZoominoutVelocity;
			if( m_MinDistance > m_TargetDistance )
			{
				m_TargetDistance = m_MinDistance; 
			}
			else if( m_MaxDistance < m_TargetDistance )
			{
				m_TargetDistance = m_MaxDistance;
			}
#ifdef _XDWDEBUG
			}
#endif			
		}

		changedcamerastatus = TRUE;
	}
	
	if( m_QuaterViewChanging )
	{
		D3DXQUATERNION	nextrotquat;
		D3DXQUATERNION	orgrotquat;
		D3DXQUATERNION	targetrotquat;
		
		D3DXQuaternionRotationYawPitchRoll( &orgrotquat, _X_RAD(mp_fYaw), _X_RAD(mp_fPitch), _X_RAD(mp_fRoll) );

		if( m_DefaultViewChanging )
		{
			D3DXQuaternionRotationYawPitchRoll( &targetrotquat, _X_RAD(-45.0f), _X_RAD(30.0f), 0.0f );
		}
		else
		{
			D3DXQuaternionRotationYawPitchRoll( &targetrotquat, _X_RAD(180.0f), _X_RAD(30.0f), 0.0f );
		}
		
		FLOAT fElapsedTime = g_fElapsedFrameMilisecondTime*3.0f;
		if( fElapsedTime > 1.0f ) fElapsedTime = 1.0f;
		D3DXQuaternionSlerp( &nextrotquat, &orgrotquat, &targetrotquat, fElapsedTime );
		
		FLOAT fyaw, fpitch, froll;
		_XMeshMath_QuaternionToEulerAngle( nextrotquat, fyaw, fpitch, froll );
		
		fyaw = _X_DEG(fyaw);
		if( fyaw > 360.0f ) fyaw = (FLOAT)fmod(fyaw, 360.0);	
		
		fpitch	= _X_DEG(fpitch); 
		if(fpitch < mp_fMinPitchLimit) fpitch = mp_fMinPitchLimit; 
		else if(fpitch > mp_fMaxPitchLimit) fpitch = mp_fMaxPitchLimit;
		
		froll	= _X_DEG(froll);
		if(froll < mp_fMinRollLimit) froll = mp_fMinRollLimit;
		else if(froll > mp_fMaxRollLimit) froll = mp_fMaxRollLimit;
		
		if( fabs( mp_fYaw   - fyaw   ) > EPSILON1 ||
			fabs( mp_fPitch - fpitch ) > EPSILON1 ||
			fabs( mp_fRoll  - froll  ) > EPSILON1 )
		{
			mp_fYaw   = fyaw;
			mp_fPitch = fpitch;
			mp_fRoll  = froll;			
			changedcamerastatus = TRUE;
		}
		else
		{
			m_QuaterViewChanging = FALSE;
			m_QuaterViewMode = TRUE;

			m_DefaultViewChanging = FALSE;			
		}

		_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
		if( pminimapwindow )
		{
			// Set direction to minimap arrow 
			pminimapwindow->SetRotateFrustum( _X_RAD( 180 - mp_fYaw ) );
		}
	}
	else if( m_DefaultViewChanging )
	{
		D3DXQUATERNION	nextrotquat;
		D3DXQUATERNION	orgrotquat;
		D3DXQUATERNION	targetrotquat;
		
		D3DXQuaternionRotationYawPitchRoll( &orgrotquat, _X_RAD(mp_fYaw), _X_RAD(mp_fPitch), _X_RAD(mp_fRoll) );
		D3DXQuaternionRotationYawPitchRoll( &targetrotquat, _X_RAD(180.0f), _X_RAD(mp_fPitch), 0.0f );
		
		FLOAT fElapsedTime = g_fElapsedFrameMilisecondTime*3.0f;
		if( fElapsedTime > 1.0f ) fElapsedTime = 1.0f;
		D3DXQuaternionSlerp( &nextrotquat, &orgrotquat, &targetrotquat, fElapsedTime );
		
		FLOAT fyaw, fpitch, froll;
		_XMeshMath_QuaternionToEulerAngle( nextrotquat, fyaw, fpitch, froll );
		
		fyaw = _X_DEG(fyaw);
		if( fyaw > 360.0f ) fyaw = (FLOAT)fmod(fyaw, 360.0);	
		
		fpitch	= _X_DEG(fpitch); 
		if(fpitch < mp_fMinPitchLimit) fpitch = mp_fMinPitchLimit; 
		else if(fpitch > mp_fMaxPitchLimit) fpitch = mp_fMaxPitchLimit;
		
		froll	= _X_DEG(froll);
		if(froll < mp_fMinRollLimit) froll = mp_fMinRollLimit;
		else if(froll > mp_fMaxRollLimit) froll = mp_fMaxRollLimit;
		
		if( fabs( mp_fYaw   - fyaw   ) > EPSILON1 ||
			fabs( mp_fPitch - fpitch ) > EPSILON1 ||
			fabs( mp_fRoll  - froll  ) > EPSILON1 )
		{
			mp_fYaw   = fyaw;
			mp_fPitch = fpitch;
			mp_fRoll  = froll;			
			changedcamerastatus = TRUE;
		}
		else
		{
			m_DefaultViewChanging = FALSE;
		}

		_XWindow_WorldMinimap* pminimapwindow = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MINIMAPWINDOW );
		if( pminimapwindow )
		{
			// Set direction to minimap arrow 
			pminimapwindow->SetRotateFrustum( _X_RAD( 180 - mp_fYaw ) );
		}
	}
	else
	{
		if( !gpInput->GetMouseState()->bButton[1] && 
			( g_pLocalUser->GetMotionClass() == _XACTION_MOVE && 
			  ( g_pLocalUser->m_PathNodeCount >= 1 || g_pLocalUser->m_LeftFinalTargetLength > 64.0f ) ) &&
			  (fabs( g_pLocalUser->m_RotateAngle - g_pLocalUser->m_LastRotateAngle ) < EPSILON3) && m_AutoBackTrace )
		{
			D3DXQUATERNION	nextrotquat;	
			D3DXQUATERNION	orgrotquat;
			D3DXQUATERNION	targetrotquat;
					
			D3DXMATRIX  mtxRotate = g_pLocalUser->m_ModelDescriptor.m_Position;		
			mtxRotate._41 = mtxRotate._42 = mtxRotate._43 = 0.0f;
			
			D3DXQuaternionRotationYawPitchRoll( &orgrotquat, 0.0f, _X_RAD(mp_fPitch), 0.0f );

			D3DXQuaternionRotationMatrix( &targetrotquat, &mtxRotate );
			D3DXQuaternionMultiply( &targetrotquat, &orgrotquat, &targetrotquat );

			D3DXQuaternionRotationYawPitchRoll( &orgrotquat, _X_RAD(mp_fYaw), _X_RAD(mp_fPitch), _X_RAD(mp_fRoll) );

			FLOAT fElapsedTime = g_fElapsedFrameMilisecondTime;
			if( fElapsedTime > 1.0f ) fElapsedTime = 1.0f;
			D3DXQuaternionSlerp( &nextrotquat, &orgrotquat, &targetrotquat, fElapsedTime );

			FLOAT fyaw, fpitch, froll;
			_XMeshMath_QuaternionToEulerAngle( nextrotquat, fyaw, fpitch, froll );

			fyaw = _X_DEG(fyaw);
			if( fyaw > 360.0f ) fyaw = (FLOAT)fmod(fyaw, 360.0);	
			
			fpitch	= _X_DEG(fpitch); 
			if(fpitch < mp_fMinPitchLimit) fpitch = mp_fMinPitchLimit; 
			else if(fpitch > mp_fMaxPitchLimit) fpitch = mp_fMaxPitchLimit;
				
			froll	= _X_DEG(froll);
			if(froll < mp_fMinRollLimit) froll = mp_fMinRollLimit;
			else if(froll > mp_fMaxRollLimit) froll = mp_fMaxRollLimit;

			if( fabs( mp_fYaw   - fyaw   ) > EPSILON3 ||
				fabs( mp_fPitch - fpitch ) > EPSILON3 ||
				fabs( mp_fRoll  - froll  ) > EPSILON3 )
			{
				mp_fYaw   = fyaw;
				mp_fPitch = fpitch;
				mp_fRoll  = froll;			
				changedcamerastatus = TRUE;
			}
		}
	}

	if( m_CameraShakeMode )
	{
		if( !m_CameraShakeDelayMode )
		{
			const FLOAT shakeadditionalfactor = 10.0f;
			FLOAT fshakefactor = _XLinearGraph( 0.1f,0.05f,  0.5f,0.0f ).GetValueAt( g_fElapsedFrameMilisecondTime );
			m_CameraShakeFactor.x = fshakefactor * sinf(rand());
			m_CameraShakeFactor.y = fshakefactor * sinf(rand());
			m_CameraShakeFactor.z = fshakefactor * sinf(rand());
			
			D3DXMATRIX matOrientation;
			D3DXMatrixInverse( &matOrientation, NULL, &mp_view_matrix );
			D3DXVec3TransformNormal( &m_CameraShakeFactor, &m_CameraShakeFactor, &matOrientation );
			
			changedcamerastatus = TRUE;
			
			m_fCameraShakeTimer -= g_fElapsedFrameMilisecondTime;
			if( m_fCameraShakeTimer < 0.0f )
			{
				m_CameraShakeMode = FALSE; 
				m_CameraShakeFactor = D3DXVECTOR3( 0.0f,0.0f,0.0f );
			}
			
		}
		else
		{
			if( g_LocalSystemTime - m_fCameraShakeStartTimer < m_fCameraShakeTimer*1000 )
			{
				
				int temp = (int)(((g_LocalSystemTime - m_fCameraShakeStartTimer)/1000.0f)/5.0f)%2;
				if( temp == 0 )
				{					
					if( !m_ChangeCameraShakeAtDelayMode )
					{
						_XPlaySoundEffect(ID_SR_INTERFACE_EARTHQUAKE_WAV, g_pLocalUser->m_Position );
					}
					const FLOAT shakeadditionalfactor = 10.0f;
					FLOAT fshakefactor = _XLinearGraph( 0.05f,0.025f,  0.25f,0.0f ).GetValueAt( g_fElapsedFrameMilisecondTime );
					m_CameraShakeFactor.x = fshakefactor * sinf(rand());
					m_CameraShakeFactor.y = fshakefactor * sinf(rand());
					m_CameraShakeFactor.z = fshakefactor * sinf(rand());
					
					D3DXMATRIX matOrientation;
					D3DXMatrixInverse( &matOrientation, NULL, &mp_view_matrix );
					D3DXVec3TransformNormal( &m_CameraShakeFactor, &m_CameraShakeFactor, &matOrientation );
					
					changedcamerastatus = TRUE;
					m_ChangeCameraShakeAtDelayMode = TRUE;
				}
				else
				{
					if( m_ChangeCameraShakeAtDelayMode )
					{
						_XPlaySoundEffect(ID_SR_INTERFACE_EARTHQUAKE01_WAV, g_pLocalUser->m_Position );
					}
					m_ChangeCameraShakeAtDelayMode = FALSE;
				}
			}
			else
			{
				m_CameraShakeMode = FALSE; 
				m_CameraShakeFactor = D3DXVECTOR3( 0.0f,0.0f,0.0f );
			}
		}
		
	}

	if( changedcamerastatus )
	{
		UpdateViewMatrix( &g_LodTerrain, TRUE );
		g_LodTerrain.m_ObjectQuadTree.UpdateCamera(g_LodTerrain.m_3PCamera);
		g_LodTerrain.RebuildLevel();
	}
}


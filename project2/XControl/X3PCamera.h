// X3PCamera.h: interface for the _X3PCamera class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _X3PCAMERA_H_
#define _X3PCAMERA_H_

#include <XKernel.h>
#include <d3dx9.h>

#define _XDEF_BASEHEIGHTOFTERRAIN			0.85f
#define _XDEF_CHARACTERAVERAGEHEIGHT		1.18f
#define _XDEF_CHARACTERAVERAGEHEIGHT_CAMERA	1.18f

class _XLodTerrain;

extern BOOL g_FreeMoveMode;

class _X3PCamera  
{
public:
	_X3PCamera();
	virtual ~_X3PCamera();

	void	Initialize( void );

	void	Set(FLOAT fov, FLOAT aspect,
				FLOAT nearplane, FLOAT farplane);
	void	Set(FLOAT pos_x, FLOAT pos_y, FLOAT pos_z);

	void	SetTargetPosition( D3DXVECTOR3 pos )
	{ 
			if( g_FreeMoveMode )
			{
				m_TargetPosition.x = pos.x; 
				m_TargetPosition.z = pos.z; 

				if( m_TargetPosition.y < pos.y )
					m_TargetPosition.y = pos.y; 
			}
			else
			{
				m_TargetPosition = pos; 
			}
	};
	void	SetCameraPosition( D3DXVECTOR3 pos ){ m_CameraPosition = pos; };
	void	SetDistance( FLOAT radius );

	void	SetPitchLimits(float min, float max);
	void	SetRollLimits(float min, float max);
	void	SetDistanceLimits( FLOAT min, FLOAT max );
	void	SetAdditionalHeightMinDistance( FLOAT min ){ m_AdditionalHeightMinDistance = min; };
	void	AddYaw(float a);
	void	AddPitch(float a);
	void	AddRoll(float a);
	
	void	FindCameraPosition2D();

	inline	FLOAT GetYaw(){	return mp_fYaw; }
	inline	FLOAT GetPitch(){ return mp_fPitch; }
	inline	FLOAT GetRoll(){ return mp_fRoll; }	
	inline  FLOAT GetDistance( void ){ return m_TargetDistance; }
	inline	D3DXMATRIX& GetViewMatrix() 
	{
		return mp_view_matrix; 
	}
	inline	D3DXMATRIX& GetProjMatrix() 
	{ 
		return mp_proj_matrix; 
	}
	inline	D3DXMATRIX& GetViewprojMatrix() 
	{ 		
		return mp_viewproj_matrix; 
	}
	
	void	UpdateViewMatrix( _XLodTerrain* terrainobject = NULL, BOOL setexternproperties = TRUE );
	void	UpdateProjMatrix();
	void	UpdateViewprojMatrix();

	void    CheckHeight( FLOAT y );

	FLOAT	GetRotateRadian( POINT mousepos, D3DXVECTOR3 pos );

	void	Advance( void );

	BOOL	GetCameraShakeMode( void ){ return m_CameraShakeMode; }
	void	SetCameraShakeMode( BOOL enable, FLOAT ftimerpermsec = 99999.0f , BOOL delay = FALSE )
	{ 
			if( m_CameraShakeMode )
			{
				// 기존의 카메라 쉐이크를 유지하기 위해 새 타이머가 적을경우 무시
				if( m_fCameraShakeTimer > ftimerpermsec )
					return;
			}
			m_CameraShakeDelayMode = delay;

			m_CameraShakeMode = enable; 
			m_CameraShakeFactor = D3DXVECTOR3( 0.0f,0.0f,0.0f );
			m_fCameraShakeTimer = ftimerpermsec;
			m_fCameraShakeStartTimer = g_LocalSystemTime;
	}	

	BOOL    GetEnableFirstPersonView( void ){ return m_EnableFirstPersonView; }
	void    SetEnableFirstPersonView( BOOL enable )
	{
			m_EnableFirstPersonView = enable;
	}

	void	SetQuaterViewMode( BOOL enable )
	{
			m_QuaterViewChanging = enable;
			m_QuaterViewMode = FALSE;
	}
	void	SetDefaultViewMode( BOOL enable )
	{
			m_DefaultViewChanging = enable;
	}

public:
	FLOAT		m_MinDistance;
	FLOAT		m_MaxDistance;
	FLOAT		m_TargetDistance;
	FLOAT		m_AdditionalHeightMinDistance;
	FLOAT		m_CurrentTargetDistance;
	
	BOOL		m_EnableFirstPersonView;

	D3DXVECTOR3 m_TargetPosition;
	D3DXVECTOR3	m_CameraPosition;

	D3DXVECTOR3	m_MoveVector;
	D3DXVECTOR3	m_LookVector;
	D3DXVECTOR3	m_UpVector;
	D3DXVECTOR3	m_RightVector;
	
	FLOAT		mp_fYaw;
	FLOAT		mp_fPitch, mp_fMinPitchLimit, mp_fMaxPitchLimit;
	FLOAT		mp_fRoll,  mp_fMinRollLimit,  mp_fMaxRollLimit;

	D3DXVECTOR3	m_CameraVelocity;
	FLOAT		m_ZoominoutVelocity;

	D3DXMATRIX  mp_view_matrix;
	D3DXMATRIX	mp_proj_matrix;
	D3DXMATRIX	mp_viewproj_matrix;

	FLOAT		mp_fAspect;
	FLOAT		mp_fFov;
	FLOAT		mp_fNearPlane;
	FLOAT		mp_fFarPlane;

	FLOAT		m_ImpactValue;
	
	BOOL		m_AutoBackTrace;
	BOOL		m_CameraShakeMode;
	BOOL		m_CameraShakeDelayMode;
	BOOL		m_ChangeCameraShakeAtDelayMode;
	D3DXVECTOR3 m_CameraShakeFactor;
	FLOAT		m_fCameraShakeTimer;
	FLOAT		m_fCameraShakeStartTimer;
	
	BOOL		m_QuaterViewMode;
	BOOL		m_QuaterViewChanging;

	BOOL		m_DefaultViewChanging;
};

#endif // !defined(_X3PCAMERA_H_)

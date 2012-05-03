// XParticleActionBase.h: interface for the _XParticleActionBase class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPARTICLEACTION_H_
#define _XPARTICLEACTION_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XList.h"
#include "XParticleDefine.h"
#include "XParticleElement.h"
#include "XParticleEmitter.h"
#include "XParticleList.h"

class _XParticleList;

class _XTimeSlice : public _XItem
{
public :
	FLOAT starttime;
	FLOAT endtime;

public :
	_XTimeSlice()
	{
		//starttime = 0.0f;
		//endtime   = 30000.0f;		
	}
	_XTimeSlice( FLOAT _starttime, FLOAT _endtime )
	{		
		starttime = _starttime;
		endtime   = _endtime;
	}

	BOOL Load( FILE* fileptr )
	{
		if( !fileptr ) return FALSE;
		if( fread( &starttime, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
		if( fread( &endtime, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
		return TRUE;
	}
};

//////////////////////////////////////////////////////////////////////
// Particle action base class
class _XParticleActionBase : public _XItem
{
public:	
	static FLOAT		 dt;			// This is copied to here from global state.
	static FLOAT		 localtime;
	static FLOAT		 apptime;
	static BOOL			 bLocalUser;
	

	BOOL				 enable;
	_XParticleActionType actiontype;	// Type field
	_XList				 timetracklist;
		
public:
	_XParticleActionBase();
	virtual ~_XParticleActionBase();	
	virtual void Execute( _XParticleList* pParticleList ){};
	virtual BOOL CheckActionTime( void );
	virtual void CheckMaxTime( FLOAT _maxtime );
	virtual _XTimeSlice* InsertTimeSlice( _XTimeSlice* pTimeSlice );

	void	SortByTimeLine( void );
		
	static  BOOL  LoadActionType(  FILE* fileptr, _XParticleActionType& _actiontype );
	static  _XParticleActionBase* CreateAction( _XParticleActionType _actiontype );

	BOOL	Load( FILE* fileptr );
	
	virtual BOOL LoadSubClassData( FILE* fileptr ){ return TRUE; }
		
	_XParticleActionBase* CreateAndCopy( void );
	virtual void CopySubClassData( _XParticleActionBase* pNewObject ){}
};


//////////////////////////////////////////////////////////////////////
// Particle action expand classes

class _XParticleAction_Header : public _XParticleActionBase
{
public:	
	int		actions_allocated;
	int		count;			// Total actions in the list.
	FLOAT	padding[96];	// This must be the largest action.
	
public:	
	_XParticleAction_Header(){ actiontype = _XPA_HeaderID; }
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Header* pNewAction = (_XParticleAction_Header*)pNewObject;
		pNewAction->actions_allocated = this->actions_allocated;
		pNewAction->count = this->count;
		memcpy( this->padding, pNewAction->padding, sizeof(FLOAT)*96 );
	}
};


class _XParticleAction_Avoid : public _XParticleActionBase
{
public:	
	_XParticleEmitter	position;	// Avoid region
	FLOAT				look_ahead;	// how many time units ahead to look
	FLOAT				magnitude;	// what percent of the way to go each time
	FLOAT				epsilon;	// add to r^2 for softening
	
public:	
	_XParticleAction_Avoid(){ actiontype = _XPA_AvoidID; }
	void Initialize(float _magnitude, float _epsilon, float _look_ahead, 
					_XParticleEmitterType dtype,
					float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{				
		position = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
		magnitude = _magnitude;
		epsilon = _epsilon;
		look_ahead = _look_ahead;
	}

	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Avoid* pNewAction = (_XParticleAction_Avoid*)pNewObject;
		pNewAction->position = this->position;
		pNewAction->look_ahead = this->look_ahead;
		pNewAction->magnitude = this->magnitude;
		pNewAction->epsilon = this->epsilon;		
	}
};


class _XParticleAction_Bounce : public _XParticleActionBase
{
public:	
	_XParticleEmitter	position;			// Bounce region
	FLOAT				oneMinusFriction;	// Friction tangent to surface
	FLOAT				resilience;			// Resilence perpendicular to surface
	FLOAT				cutoffSqr;			// cutoff velocity; friction applies iff v > cutoff
		
public:	
	_XParticleAction_Bounce(){ actiontype = _XPA_BounceID; }
	void Initialize(float _friction, float _resilience, float _cutoff,
					_XParticleEmitterType dtype,
					float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{
		position = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
		oneMinusFriction = 1.0f - _friction;
		resilience = _resilience;
		cutoffSqr = fsqr(_cutoff);
	}

	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Bounce* pNewAction = (_XParticleAction_Bounce*)pNewObject;
		pNewAction->position = this->position;
		pNewAction->oneMinusFriction = this->oneMinusFriction;
		pNewAction->resilience = this->resilience;
		pNewAction->cutoffSqr = this->cutoffSqr;		
	}
};



class _XParticleAction_CallActionList : public _XParticleActionBase
{
public:	
	int					effecttemplateid;	// The action list number to call
	DWORD				usetargetposition;
	DWORD				insideparticles;
	BOOL				usestopvolumeeffect;
	BOOL				enablesink;
	_XParticleEmitter	position;			// Event region	
	
public:	
	_XParticleAction_CallActionList(){ actiontype = _XPA_CallActionListID;
									   effecttemplateid = 0;
									   usetargetposition = 0;
									   insideparticles = 1;
									   usestopvolumeeffect = FALSE;
									   enablesink = FALSE; }
	
	void Initialize( int _effecttemplateid, 
					 bool _stop_inside, _XParticleEmitterType dtype,
					 float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					 float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					 float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f )
	{
		effecttemplateid = _effecttemplateid;
		position = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}

	void Execute( _XParticleList* pParticleList );	

	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_CallActionList* pNewAction = (_XParticleAction_CallActionList*)pNewObject;
		pNewAction->effecttemplateid = this->effecttemplateid;				
		pNewAction->usetargetposition = this->usetargetposition;
		pNewAction->insideparticles = this->insideparticles;
		pNewAction->position = this->position;
		pNewAction->usestopvolumeeffect = this->usestopvolumeeffect;
		pNewAction->enablesink = this->enablesink;
	}
};



class _XParticleAction_CopyVertexB : public _XParticleActionBase
{
public:	
	bool copy_pos;		// True to copy pos to posB.
	bool copy_vel;		// True to copy vel to velB.
	
public:	
	_XParticleAction_CopyVertexB(){ actiontype = _XPA_CopyVertexBID; }
	void Initialize(bool _copy_pos = true, bool _copy_vel = false)
	{				
		copy_pos = _copy_pos;
		copy_vel = _copy_vel;
	}
	void Execute( _XParticleList* pParticleList );	

	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_CopyVertexB* pNewAction = (_XParticleAction_CopyVertexB*)pNewObject;
		pNewAction->copy_pos = this->copy_pos;
		pNewAction->copy_vel = this->copy_vel;		
	}
};


class _XParticleAction_Damping : public _XParticleActionBase
{
public:	
	D3DXVECTOR3		damping;	// Damping constant applied to velocity
	float			vlowSqr;	// Low and high cutoff velocities
	float			vhighSqr;
	
public:	
	_XParticleAction_Damping(){ actiontype = _XPA_DampingID; }
	void Initialize(float _damping_x, float _damping_y, float _damping_z, 
					float _vlow = 0.0f, float _vhigh = _XP_MAXFLOAT)
	{
		damping = D3DXVECTOR3(_damping_x, _damping_y, _damping_z);
		vlowSqr = fsqr(_vlow);
		vhighSqr = fsqr(_vhigh);
	}	
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Damping* pNewAction = (_XParticleAction_Damping*)pNewObject;
		pNewAction->damping = this->damping;
		pNewAction->vlowSqr = this->vlowSqr;
		pNewAction->vhighSqr = this->vhighSqr;		
	}
};


class _XParticleAction_Explosion : public _XParticleActionBase
{
public:	
	D3DXVECTOR3		center;		// The center of the explosion
	FLOAT			velocity;	// Of shock wave
	FLOAT			magnitude;	// At unit radius
	FLOAT			stdev;		// Sharpness or width of shock wave
	FLOAT			age;		// How long it's been going on
	FLOAT			epsilon;	// Softening parameter
	
public:	
	_XParticleAction_Explosion(){ actiontype = _XPA_ExplosionID; }
	void Initialize(float _center_x, float _center_y, float _center_z, float _velocity,
					float _magnitude, float _stdev, float _epsilon = _XP_EPS, float _age = 0.0f)
	{				
		center = D3DXVECTOR3(_center_x, _center_y, _center_z);
		velocity = _velocity;
		magnitude = _magnitude;
		stdev = _stdev;
		epsilon = _epsilon;
		age = _age;
		
		if(epsilon < 0.0f) epsilon = _XP_EPS;
	}
	
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Explosion* pNewAction = (_XParticleAction_Explosion*)pNewObject;
		pNewAction->center = this->center;
		pNewAction->velocity = this->velocity;
		pNewAction->magnitude = this->magnitude;		
		pNewAction->stdev = this->stdev;		
		pNewAction->age = this->age;		
		pNewAction->epsilon = this->epsilon;
	}
};


class _XParticleAction_Follow : public _XParticleActionBase
{
public:	
	FLOAT magnitude;	// The grav of each particle
	FLOAT epsilon;		// Softening parameter
	FLOAT max_radius;	// Only influence particles within max_radius	
	
public:	
	_XParticleAction_Follow(){ actiontype = _XPA_FollowID; }
	void Initialize(float _magnitude = 1.0f, float _epsilon = _XP_EPS, float _max_radius = _XP_MAXFLOAT)
	{
		magnitude  = _magnitude;
		epsilon    = _epsilon;
		max_radius = _max_radius;		
	}
	void Execute( _XParticleList* pParticleList );	

	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Follow* pNewAction = (_XParticleAction_Follow*)pNewObject;		
		pNewAction->magnitude = this->magnitude;				
		pNewAction->epsilon = this->epsilon;
		pNewAction->max_radius = this->max_radius;
	}
};


class _XParticleAction_Gravitate : public _XParticleActionBase
{
public:	
	FLOAT magnitude;	// The grav of each particle
	FLOAT epsilon;		// Softening parameter
	FLOAT max_radius;	// Only influence particles within max_radius	
	
public:	
	_XParticleAction_Gravitate(){ actiontype = _XPA_GravitateID; }
	void Initialize(float _magnitude = 1.0f, float _epsilon = _XP_EPS, 
		            float _max_radius = _XP_MAXFLOAT)
	{			
		magnitude	= _magnitude;
		epsilon		= _epsilon;
		max_radius	= _max_radius;
	}
	void Execute( _XParticleList* pParticleList );	

	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Gravitate* pNewAction = (_XParticleAction_Gravitate*)pNewObject;		
		pNewAction->magnitude = this->magnitude;				
		pNewAction->epsilon = this->epsilon;
		pNewAction->max_radius = this->max_radius;
	}
};


class _XParticleAction_Gravity : public _XParticleActionBase
{
public:	
	D3DXVECTOR3 direction;	// Amount to increment velocity
	
public:	
	_XParticleAction_Gravity(){ actiontype = _XPA_GravityID; }
	void Initialize(float dir_x, float dir_y, float dir_z)
	{
		direction = D3DXVECTOR3(dir_x, dir_y, dir_z);
	}
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Gravity* pNewAction = (_XParticleAction_Gravity*)pNewObject;		
		pNewAction->direction = this->direction;						
	}
};


class _XParticleAction_Jet : public _XParticleActionBase
{
public:	
	D3DXVECTOR3			center;		// Center of the fan
	_XParticleEmitter	acc;		// Acceleration vector domain
	FLOAT				magnitude;	// Scales acceleration
	FLOAT				epsilon;	// Softening parameter
	FLOAT				max_radius;	// Only influence particles within max_radius	
	
public:	
	_XParticleAction_Jet(){ actiontype = _XPA_JetID; }
	void Initialize(_XParticleList* pParticleList,
		            float _center_x, float _center_y, float _center_z,
					float _magnitude = 1.0f, float _epsilon = _XP_EPS, 
					float _max_radius = _XP_MAXFLOAT)
	{				
		center = D3DXVECTOR3(_center_x, _center_y, _center_z);
		acc			= pParticleList->m_pInstanceData->Velocity;
		magnitude	= _magnitude;
		epsilon		= _epsilon;
		max_radius	= _max_radius;
	}
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Jet* pNewAction = (_XParticleAction_Jet*)pNewObject;		
		pNewAction->center = this->center;						
		pNewAction->acc = this->acc;						
		pNewAction->magnitude = this->magnitude;						
		pNewAction->epsilon = this->epsilon;
		pNewAction->max_radius = this->max_radius;
	}
};


class _XParticleAction_KillOld : public _XParticleActionBase
{
public:	
	FLOAT	age_limit;		// Exact age at which to kill particles.
	bool	kill_less_than;	// True to kill particles less than limit.
	
public:	
	_XParticleAction_KillOld(){ actiontype = _XPA_KillOldID; }
	void Initialize(float _age_limit, bool _kill_less_than = false)
	{				
		age_limit		= _age_limit;
		kill_less_than	= _kill_less_than;
	}
	void Execute( _XParticleList* pParticleList );	

	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_KillOld* pNewAction = (_XParticleAction_KillOld*)pNewObject;		
		pNewAction->age_limit = this->age_limit;						
		pNewAction->kill_less_than = this->kill_less_than;
	}
};


class _XParticleAction_MatchVelocity : public _XParticleActionBase
{
public:	
	FLOAT magnitude;	// The grav of each particle
	FLOAT epsilon;		// Softening parameter
	FLOAT max_radius;	// Only influence particles within max_radius
	
public:	
	_XParticleAction_MatchVelocity(){ actiontype = _XPA_MatchVelocityID; }
	void Initialize(float _magnitude = 1.0f, float _epsilon = _XP_EPS, 
					float _max_radius = _XP_MAXFLOAT)
	{				
		magnitude	= _magnitude;
		epsilon		= _epsilon;
		max_radius	= _max_radius;
	}
	void Execute( _XParticleList* pParticleList );	

	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_MatchVelocity* pNewAction = (_XParticleAction_MatchVelocity*)pNewObject;		
		pNewAction->magnitude = this->magnitude;						
		pNewAction->epsilon = this->epsilon;
		pNewAction->max_radius = this->max_radius;
	}
};

class _XParticleAction_Move : public _XParticleActionBase
{
public:	
	
public:	
	_XParticleAction_Move(){ actiontype = _XPA_MoveID; }
	void Execute( _XParticleList* pParticleList );
};


class _XParticleAction_OrbitLine : public _XParticleActionBase
{
public:	
	D3DXVECTOR3		p;
	D3DXVECTOR3		axis;		// Endpoints of line to which particles are attracted
	FLOAT			magnitude;	// Scales acceleration
	FLOAT			epsilon;	// Softening parameter
	FLOAT			max_radius;	// Only influence particles within max_radius
	BOOL			usetargetposition;
	
public:	
	_XParticleAction_OrbitLine(){ actiontype = _XPA_OrbitLineID; }
	void Initialize(float _p_x, float _p_y, float _p_z,
					float _axis_x, float _axis_y, float _axis_z,
					float _magnitude = 1.0f, float _epsilon = _XP_EPS, 
					float _max_radius = _XP_MAXFLOAT)
	{	
		p		= D3DXVECTOR3(_p_x, _p_y, _p_z);
		axis	= D3DXVECTOR3(_axis_x, _axis_y, _axis_z);
		D3DXVec3Normalize( &axis, &axis );
		magnitude	= _magnitude;
		epsilon		= _epsilon;
		max_radius	= _max_radius;
		usetargetposition = FALSE;
	}
	
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_OrbitLine* pNewAction = (_XParticleAction_OrbitLine*)pNewObject;		
		pNewAction->p = this->p;
		pNewAction->axis = this->axis;
		pNewAction->magnitude = this->magnitude;
		pNewAction->epsilon = this->epsilon;
		pNewAction->max_radius = this->max_radius;
		pNewAction->usetargetposition = this->usetargetposition;
	}
};


class _XParticleAction_OrbitPoint : public _XParticleActionBase
{
public:	
	D3DXVECTOR3		center;		// Point to which particles are attracted
	FLOAT			magnitude;	// Scales acceleration
	FLOAT			epsilon;	// Softening parameter
	FLOAT			max_radius;	// Only influence particles within max_radius
	DWORD			usetargetposition;
	bool			directturnning;
	
public:	
	_XParticleAction_OrbitPoint(){ actiontype = _XPA_OrbitPointID; directturnning = true; }
	void Initialize(float _center_x, float _center_y, float _center_z,
					float _magnitude = 1.0f, float _epsilon = _XP_EPS, 
					float _max_radius = _XP_MAXFLOAT)
	{
		center		= D3DXVECTOR3(_center_x, _center_y, _center_z);
		magnitude	= _magnitude;
		epsilon		= _epsilon;
		max_radius	= _max_radius;
		usetargetposition = 0;
		directturnning = true;
	}
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_OrbitPoint* pNewAction = (_XParticleAction_OrbitPoint*)pNewObject;		
		pNewAction->center = this->center;		
		pNewAction->magnitude = this->magnitude;
		pNewAction->epsilon = this->epsilon;
		pNewAction->max_radius = this->max_radius;
		pNewAction->usetargetposition = this->usetargetposition;
		pNewAction->directturnning = this->directturnning;
	}
};


class _XParticleAction_RandomAccel : public _XParticleActionBase
{
public:	
	_XParticleEmitter	gen_acc;	// The domain of random accelerations.
	
public:	
	_XParticleAction_RandomAccel(){ actiontype = _XPA_RandomAccelID; }
	void Initialize(_XParticleEmitterType dtype,
					float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{				
		gen_acc = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);		
	}
	void Execute( _XParticleList* pParticleList );
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_RandomAccel* pNewAction = (_XParticleAction_RandomAccel*)pNewObject;		
		pNewAction->gen_acc = this->gen_acc;
	}
};


class _XParticleAction_RandomDisplace : public _XParticleActionBase
{
public:	
	_XParticleEmitter   gen_disp;	// The domain of random displacements.
	
public:	
	_XParticleAction_RandomDisplace(){ actiontype = _XPA_RandomDisplaceID; }
	void Initialize(_XParticleEmitterType dtype,
					float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{				
		gen_disp = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);		
	}
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_RandomDisplace* pNewAction = (_XParticleAction_RandomDisplace*)pNewObject;		
		pNewAction->gen_disp = this->gen_disp;
	}
};


class _XParticleAction_RandomVelocity : public _XParticleActionBase
{
public:	
	_XParticleEmitter	gen_vel;	// The domain of random velocities.
	
public:	
	_XParticleAction_RandomVelocity(){ actiontype = _XPA_RandomVelocityID; }
	void Initialize(_XParticleEmitterType dtype,
					float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{			
		gen_vel = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_RandomVelocity* pNewAction = (_XParticleAction_RandomVelocity*)pNewObject;		
		pNewAction->gen_vel = this->gen_vel;
	}
};


class _XParticleAction_Restore : public _XParticleActionBase
{
public:	
	FLOAT time_left;				// Time remaining until they should be in position.
	
public:	
	_XParticleAction_Restore(){ actiontype = _XPA_RestoreID; }
	void Initialize(float _time_left)
	{
		time_left = _time_left;
	}
	void Execute( _XParticleList* pParticleList );
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Restore* pNewAction = (_XParticleAction_Restore*)pNewObject;		
		pNewAction->time_left = this->time_left;
	}
};

class _XParticleAction_Sink : public _XParticleActionBase
{
public:	
	bool				kill_inside;	// True to dispose of particles *inside* domain
	_XParticleEmitter	position;		// Disposal region
	
public:	
	_XParticleAction_Sink(){ actiontype = _XPA_SinkID; }
	void Initialize(bool _kill_inside, _XParticleEmitterType dtype,
					float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{
		kill_inside = _kill_inside;
		position = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
	}
	void Execute( _XParticleList* pParticleList );	
		
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Sink* pNewAction = (_XParticleAction_Sink*)pNewObject;		
		pNewAction->kill_inside = this->kill_inside;
		pNewAction->position = this->position;
	}
};


class _XParticleAction_SinkVelocity : public _XParticleActionBase
{
public:	
	bool				kill_inside;	// True to dispose of particles with vel *inside* domain
	_XParticleEmitter	velocity;		// Disposal region
	
public:	
	_XParticleAction_SinkVelocity(){ actiontype = _XPA_SinkVelocityID; }
	void Initialize(bool _kill_inside, _XParticleEmitterType dtype,
					float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{
		kill_inside = _kill_inside;
		velocity = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);		
	}
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_SinkVelocity* pNewAction = (_XParticleAction_SinkVelocity*)pNewObject;		
		pNewAction->kill_inside = this->kill_inside;
		pNewAction->velocity = this->velocity;
	}
};

class _XParticleAction_SpeedLimit : public _XParticleActionBase
{
public:	
	FLOAT				min_speed;		// Clamp speed to this minimum.
	FLOAT				max_speed;		// Clamp speed to this maximum.
	
public:	
	_XParticleAction_SpeedLimit(){ actiontype = _XPA_SpeedLimitID; }
	void Initialize(float _min_speed, float _max_speed = _XP_MAXFLOAT)
	{	
		min_speed = _min_speed;
		max_speed = _max_speed;
	}
	void Execute( _XParticleList* pParticleList );	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_SpeedLimit* pNewAction = (_XParticleAction_SpeedLimit*)pNewObject;		
		pNewAction->min_speed = this->min_speed;
		pNewAction->max_speed = this->max_speed;
	}
};

class _XParticleAction_Source : public _XParticleActionBase
{
public:	
	_XParticleEmitter	position;		// Choose a position in this domain.
	_XParticleEmitter	positionB;		// Choose a positionB in this domain.
	_XParticleEmitter	size;			// Choose a size in this domain.
	_XParticleEmitter	velocity;		// Choose a velocity in this domain.
	_XParticleEmitter	color;			// Choose a color in this domain.
	FLOAT				alpha;			// Alpha of all generated particles
	FLOAT				particle_rate;	// Particles to generate per unit time
	FLOAT				age;			// Initial age of the particles
	FLOAT				age_sigma;		// St. dev. of initial age of the particles
	bool				vertexB_tracks;	// True to get positionB from position.

	// Link spline path...
	BOOL				userotation;
	int					emittersplinecontainerindex;
	FLOAT				emitterpathmovespeed;
	BOOL				rewind;

	BOOL				timebaseparticlerate; // TRUE : time base, FALSE : frame base
		
public:	
	_XParticleAction_Source()
	{ 
		actiontype = _XPA_SourceID;
		userotation = FALSE;
		emittersplinecontainerindex = -1;
		emitterpathmovespeed = 0.5f;				
		rewind = FALSE;
		timebaseparticlerate = TRUE;
	}
	void Initialize(_XParticleList* pParticleList,
		            float _particle_rate, _XParticleEmitterType dtype,
					float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
					float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
					float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{				
		particle_rate = _particle_rate;
		position	= _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);
		positionB	= pParticleList->m_pInstanceData->VertexB;
		size		= pParticleList->m_pInstanceData->Size;
		velocity	= pParticleList->m_pInstanceData->Velocity;
		color		= pParticleList->m_pInstanceData->Color;
		alpha		= pParticleList->m_pInstanceData->Alpha;
		age			= pParticleList->m_pInstanceData->Age;
		age_sigma	= pParticleList->m_pInstanceData->AgeSigma;
		vertexB_tracks = pParticleList->m_pInstanceData->vertexB_tracks;
		timebaseparticlerate = TRUE;
	}
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Source* pNewAction = (_XParticleAction_Source*)pNewObject;		
		pNewAction->position = this->position;
		pNewAction->positionB = this->positionB;
		pNewAction->size = this->size;
		pNewAction->velocity = this->velocity;
		pNewAction->color = this->color;
		pNewAction->alpha = this->alpha;
		pNewAction->particle_rate = this->particle_rate;
		pNewAction->age = this->age;
		pNewAction->age_sigma = this->age_sigma;
		pNewAction->vertexB_tracks = this->vertexB_tracks;

		pNewAction->userotation = this->userotation;
		pNewAction->emittersplinecontainerindex = this->emittersplinecontainerindex;
		pNewAction->emitterpathmovespeed = this->emitterpathmovespeed;
		pNewAction->rewind = this->rewind;
		pNewAction->timebaseparticlerate = this->timebaseparticlerate;
	}
};

class _XParticleAction_TargetColor : public _XParticleActionBase
{
public:	
	D3DXCOLOR			color;		// Color to shift towards		
	FLOAT				scale;		// Amount to shift by (1 == all the way)
	
public:	
	_XParticleAction_TargetColor(){ actiontype = _XPA_TargetColorID; }
	void Initialize(float _color_x, float _color_y, float _color_z,
					float _alpha, float _scale)
	{				
		color = D3DXCOLOR(_color_x, _color_y, _color_z, _alpha);
		scale = _scale;
	}
	void Execute( _XParticleList* pParticleList );	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_TargetColor* pNewAction = (_XParticleAction_TargetColor*)pNewObject;		
		pNewAction->color = this->color;
		pNewAction->scale = this->scale;		
	}
};

class _XParticleAction_TargetSize : public _XParticleActionBase
{
public:	
	D3DXVECTOR3			size;		// Size to shift towards
	D3DXVECTOR3			scale;		// Amount to shift by per frame (1 == all the way)
	
public:	
	_XParticleAction_TargetSize(){ actiontype = _XPA_TargetSizeID; }
	void Initialize(float _size_x,  float _size_y,  float _size_z,
					float _scale_x = 0.0f, float _scale_y = 0.0f, float _scale_z = 0.0f)
	{				
		size  = D3DXVECTOR3(_size_x,  _size_y,  _size_z);
		scale = D3DXVECTOR3(_scale_x, _scale_y, _scale_z);		
	}
	void Execute( _XParticleList* pParticleList );	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_TargetSize* pNewAction = (_XParticleAction_TargetSize*)pNewObject;		
		pNewAction->size = this->size;
		pNewAction->scale = this->scale;		
	}
};

class _XParticleAction_TargetVelocity : public _XParticleActionBase
{
public:	
	D3DXVECTOR3			velocity;	// Velocity to shift towards
	FLOAT				scale;		// Amount to shift by (1 == all the way)
	
public:	
	_XParticleAction_TargetVelocity(){ actiontype = _XPA_TargetVelocityID; }
	void Initialize(float vel_x, float vel_y, float vel_z, float _scale)
	{				
		velocity = D3DXVECTOR3(vel_x, vel_y, vel_z);
		scale = _scale;
	}
	void Execute( _XParticleList* pParticleList );	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_TargetVelocity* pNewAction = (_XParticleAction_TargetVelocity*)pNewObject;		
		pNewAction->velocity = this->velocity;
		pNewAction->scale = this->scale;		
	}
};

class _XParticleAction_Vertex : public _XParticleActionBase
{
public:		
	
public:	
	_XParticleAction_Vertex(){ actiontype = _XPA_VertexID; }
	void Initialize(_XParticleList* pParticleList,float x, float y, float z)
	{					
		D3DXVECTOR3 pos(x, y, z);
		D3DXVECTOR3 siz, vel, col, posB;
		if(pParticleList->m_pInstanceData->vertexB_tracks)
			posB = pos;
		else
			pParticleList->m_pInstanceData->VertexB.Generate(posB);
		pParticleList->m_pInstanceData->Size.Generate(siz);
		pParticleList->m_pInstanceData->Velocity.Generate(vel);
		pParticleList->m_pInstanceData->Color.Generate(col);
		
		_XParticleElement particledata;
		particledata.position  = pos;
		particledata.positionB = posB;
		particledata.size	   = siz;
		particledata.velocity  = vel;
		particledata.color.r   = col.x;
		particledata.color.g   = col.y;
		particledata.color.b   = col.z;
		particledata.age       = pParticleList->m_pInstanceData->Age;

		pParticleList->AddParticle( particledata );
	}	

	void Execute( _XParticleList* pParticleList );
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Vertex* pNewAction = (_XParticleAction_Vertex*)pNewObject;		
		// Nothing.... ^^;
	}
};

class _XParticleAction_Vortex : public _XParticleActionBase
{
public:	
	D3DXVECTOR3			center;		// Center of vortex
	D3DXVECTOR3			axis;		// Axis around which vortex is applied
	FLOAT				magnitude;	// Scale for rotation around axis
	FLOAT				epsilon;	// Softening parameter
	FLOAT				max_radius;	// Only influence particles within max_radius
	
public:	
	_XParticleAction_Vortex(){ actiontype = _XPA_VortexID; }	
	void Initialize(float _center_x, float _center_y, float _center_z,
					float _axis_x, float _axis_y, float _axis_z,
					float _magnitude = 1.0f, float _epsilon = _XP_EPS, 
					float _max_radius = _XP_MAXFLOAT)
	{				
		center = D3DXVECTOR3(_center_x, _center_y, _center_z);
		axis = D3DXVECTOR3(_axis_x, _axis_y, _axis_z);
		D3DXVec3Normalize( &axis, &axis );
		magnitude	= _magnitude;
		epsilon		= _epsilon;
		max_radius	= _max_radius;
	}
		
	void Execute( _XParticleList* pParticleList );	
	
	BOOL LoadSubClassData( FILE* fileptr );
	
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_Vortex* pNewAction = (_XParticleAction_Vortex*)pNewObject;		
		pNewAction->center = this->center;
		pNewAction->axis = this->axis;		
		pNewAction->magnitude = this->magnitude;		
		pNewAction->epsilon = this->epsilon;		
		pNewAction->max_radius = this->max_radius;		
	}
};

class _XParticleAction_StopVolume : public _XParticleActionBase
{
public:	
	_XParticleEmitter	position;			// Stop region	
		
public:	
	_XParticleAction_StopVolume(){ actiontype = _XPA_StopVolumeID; }
	void Initialize( bool _stop_inside, _XParticleEmitterType dtype,
		float a0 = 0.0f, float a1 = 0.0f, float a2 = 0.0f,
		float a3 = 0.0f, float a4 = 0.0f, float a5 = 0.0f,
		float a6 = 0.0f, float a7 = 0.0f, float a8 = 0.0f)
	{
		position = _XParticleEmitter(dtype, a0, a1, a2, a3, a4, a5, a6, a7, a8);		
	}
	
	void Execute( _XParticleList* pParticleList );	
			
	BOOL LoadSubClassData( FILE* fileptr );
		
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_StopVolume* pNewAction = (_XParticleAction_StopVolume*)pNewObject;
		pNewAction->position = this->position;				
	}
};

class _XParticleAction_EffectSound : public _XParticleActionBase
{
public:	
	int				m_nSoundIndex;
	
public:	
	_XParticleAction_EffectSound(){ actiontype = _XPA_EffectSoundID; m_nSoundIndex = -1; }
	void Initialize(int nIndex = -1)
	{
		m_nSoundIndex = nIndex;
	}
	void Execute( _XParticleList* pParticleList );	
	BOOL LoadSubClassData( FILE* fileptr );

	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_EffectSound* pNewAction = (_XParticleAction_EffectSound*)pNewObject;		
		pNewAction->m_nSoundIndex = this->m_nSoundIndex;
	}
};

class _XParticleAction_EffectLight : public _XParticleActionBase
{
public:	
	D3DLIGHT9	m_Light;

public:	
	_XParticleAction_EffectLight(){ actiontype = _XPA_EffectLightID; }
	void Initialize(D3DLIGHT9& Light)
	{				
		memcpy(&m_Light, &Light, sizeof(D3DLIGHT9));
	}
	void Execute( _XParticleList* pParticleList );	
	BOOL LoadSubClassData( FILE* fileptr );

	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_EffectLight* pNewAction = (_XParticleAction_EffectLight*)pNewObject;		
		memcpy(&m_Light, &pNewAction->m_Light, sizeof(D3DLIGHT9));
	}
};

class _XParticleAction_EffectScreen : public _XParticleActionBase
{
public:	
	D3DCOLOR	m_Color;
	FLOAT		m_fFadeTime;

public:	
	_XParticleAction_EffectScreen(){ actiontype = _XPA_EffectScreenID; }
	void Initialize(D3DCOLOR Color, FLOAT fFadeTime)
	{				
		m_Color			= Color;
		m_fFadeTime		= fFadeTime;
	}
	void Execute( _XParticleList* pParticleList );	
	BOOL LoadSubClassData( FILE* fileptr );

	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_EffectScreen* pNewAction = (_XParticleAction_EffectScreen*)pNewObject;		
		m_Color			= pNewAction->m_Color;
		m_fFadeTime		= pNewAction->m_fFadeTime;
	}
};

class _XParticleAction_CameraShake : public _XParticleActionBase
{
public:		
	FLOAT		m_fShakeTime;
	
public:	
	_XParticleAction_CameraShake(){ actiontype = _XPA_CameraShakeID; }
	void Initialize(FLOAT fShakeTime)
	{
		m_fShakeTime	= fShakeTime;
	}
	void Execute( _XParticleList* pParticleList );	
	BOOL LoadSubClassData( FILE* fileptr );
		
	void CopySubClassData( _XParticleActionBase* pNewObject )
	{
		if( !pNewObject ) return;
		_XParticleAction_CameraShake* pNewAction = (_XParticleAction_CameraShake*)pNewObject;
		m_fShakeTime		= pNewAction->m_fShakeTime;
	}
};

#endif // !defined(_XPARTICLEACTION_H_)


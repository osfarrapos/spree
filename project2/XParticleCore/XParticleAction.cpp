// XParticleAction.cpp: implementation of the _XParticleAction class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XMeshMath.h"
#include "XParticleAction.h"
#include "XSplineContainer.h"
#include "XBipedController.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XParticleEffectManager.h"

#include <assert.h>


//////////////////////////////////////////////////////////////////////
// Global def... and.. function....

FLOAT _XParticleActionBase::dt		  = 0.0f;
FLOAT _XParticleActionBase::localtime = 0.0f;
FLOAT _XParticleActionBase::apptime   = 0.0f;
BOOL _XParticleActionBase::bLocalUser = FALSE;

#define ONE_OVER_SIGMA_EXP (1.0f / 0.7975f)

#define _XDEF_MAXEVENTACTIONINTANCINGCOUNT	5
extern int					g_LastEventParticleIDArray[_XDEF_MAXEVENTACTIONINTANCINGCOUNT]; 
extern int					g_LastEventCount;
extern _XModelDescriptor*	g_pLastEventTargetModelDescriptor;

FLOAT	g_VisualEffectQuality = 1.0f;

// To offset [0 .. 1] vectors to [-.5 .. .5]
D3DXVECTOR3 vHalf(0.5, 0.5, 0.5);

D3DXVECTOR3 RandVec()
{
	return D3DXVECTOR3(drand48(), drand48(), drand48());
}

// Return a random number with a normal distribution.
float NRand(float sigma = 1.0f)
{	
	if(sigma == 0) return 0;
	
	float y;
	do
	{
		y = -logf(drand48());
	}
	while(drand48() > expf(-fsqr(y - 1.0f)*0.5f));
	
	if(rand() & 0x1)
		return y * sigma * ONE_OVER_SIGMA_EXP;
	else
		return -y * sigma * ONE_OVER_SIGMA_EXP;
}

//////////////////////////////////////////////////////////////////////
// Particle action base class

_XParticleActionBase::_XParticleActionBase()
{
	enable = TRUE;
}

_XParticleActionBase::~_XParticleActionBase()
{
	timetracklist.disposeList();
}

void _XParticleActionBase::SortByTimeLine( void )
{
	_XTimeSlice* pts = NULL;		
	
	int itemcount = timetracklist.getitemcount();
	
	for( int i = 0; i < itemcount; i++ )
	{
		timetracklist.resetList();
		
		for( int j = 0; j < itemcount-1; j++ )
		{
			pts = NULL;
			pts = (_XTimeSlice*)timetracklist.currentItem();
			
			if( !pts || !timetracklist.currentItem()->getright() )
			{				
				break;
			}
			
			if( pts->starttime > ((_XTimeSlice*)(timetracklist.currentItem()->getright()))->starttime )
			{				
				timetracklist.currentItemSwap();
			}
			else
			{
				timetracklist.nextItem();
			}
		}       		
	}
}

_XTimeSlice* _XParticleActionBase::InsertTimeSlice( _XTimeSlice* pTimeSlice )
{
	if( !pTimeSlice ) return NULL;

	timetracklist.resetList();
	_XTimeSlice* retTimeSlice = (_XTimeSlice*)timetracklist.insertItem( pTimeSlice );

	SortByTimeLine();

	return retTimeSlice;
}

BOOL _XParticleActionBase::CheckActionTime( void )
{
	if( !enable ) return FALSE;
	if( timetracklist.listEmpty() ) return FALSE;

	timetracklist.resetList();
	_XTimeSlice* pTimeSlice;
	do 
	{
		pTimeSlice = (_XTimeSlice*)timetracklist.currentItem();
		if( pTimeSlice )
		{
			if( pTimeSlice->starttime <= localtime && pTimeSlice->endtime >= localtime )
			{
				return TRUE;
			}
		}		
		timetracklist.nextItem();
	} while(!timetracklist.atHeadOfList());
	
	return FALSE;
}

void _XParticleActionBase::CheckMaxTime( FLOAT _maxtime )
{
	if( timetracklist.listEmpty() ) return;
	
	timetracklist.resetList();
	_XTimeSlice* pTimeSlice;
	do 
	{
		pTimeSlice = (_XTimeSlice*)timetracklist.currentItem();
		if( pTimeSlice )
		{
			if( pTimeSlice->starttime > _maxtime )
			{
				pTimeSlice->starttime = _maxtime - 0.01;
			}

			if( pTimeSlice->endtime > _maxtime )
			{
				pTimeSlice->endtime = _maxtime;
			}
		}		
		timetracklist.nextItem();
	} while(!timetracklist.atHeadOfList());
}

BOOL  _XParticleActionBase::LoadActionType( FILE* fileptr,  _XParticleActionType& _actiontype )
{
	if( !fileptr )
	{
		_actiontype = _XPA_Unknown;
		return FALSE;
	}

	if( fread( &_actiontype, sizeof(_XParticleActionType), 1, fileptr ) < 1 )
	{
		_actiontype = _XPA_Unknown;
		return FALSE;
	}
	
	return TRUE;
}

_XParticleActionBase* _XParticleActionBase::CreateAndCopy( void )
{
	_XParticleActionBase* pNewObject = CreateAction( this->actiontype );
	if( !pNewObject ) return NULL;

	pNewObject->enable = this->enable;
	CopySubClassData( pNewObject );

	if( this->timetracklist.listEmpty() ) return pNewObject;
	
	this->timetracklist.resetList();
	
	_XTimeSlice* pTimeSlice;

	do 
	{
		pTimeSlice = (_XTimeSlice*)this->timetracklist.currentItem();
		if( pTimeSlice )
		{
			_XTimeSlice* pNewTimeSlice = new _XTimeSlice;
			if( pNewTimeSlice )
			{
				pNewTimeSlice->starttime = pTimeSlice->starttime;
				pNewTimeSlice->endtime = pTimeSlice->endtime;
				pNewObject->timetracklist.resetList();
				pNewObject->timetracklist.insertItem( pNewTimeSlice );
			}
			else
			{
				break;
			}			
		}
		else
		{
			break;
		}
		
		this->timetracklist.nextItem();
	} while( !this->timetracklist.atHeadOfList() );

	return pNewObject;
}

_XParticleActionBase* _XParticleActionBase::CreateAction( _XParticleActionType _actiontype )
{
	_XParticleActionBase* pAction = NULL;

	switch( _actiontype ) 
	{
	case _XPA_HeaderID			: return (_XParticleActionBase*) new _XParticleAction_Header;
	case _XPA_AvoidID			: return (_XParticleActionBase*) new _XParticleAction_Avoid;
	case _XPA_BounceID			: return (_XParticleActionBase*) new _XParticleAction_Bounce;
	case _XPA_CallActionListID	: return (_XParticleActionBase*) new _XParticleAction_CallActionList;
	case _XPA_CopyVertexBID		: return (_XParticleActionBase*) new _XParticleAction_CopyVertexB;
	case _XPA_DampingID			: return (_XParticleActionBase*) new _XParticleAction_Damping;
	case _XPA_ExplosionID		: return (_XParticleActionBase*) new _XParticleAction_Explosion;
	case _XPA_FollowID			: return (_XParticleActionBase*) new _XParticleAction_Follow;
	case _XPA_GravitateID		: return (_XParticleActionBase*) new _XParticleAction_Gravitate;
	case _XPA_GravityID			: return (_XParticleActionBase*) new _XParticleAction_Gravity;
	case _XPA_JetID				: return (_XParticleActionBase*) new _XParticleAction_Jet;
	case _XPA_KillOldID			: return (_XParticleActionBase*) new _XParticleAction_KillOld;
	case _XPA_MatchVelocityID	: return (_XParticleActionBase*) new _XParticleAction_MatchVelocity;
	case _XPA_MoveID			: return (_XParticleActionBase*) new _XParticleAction_Move;
	case _XPA_OrbitLineID		: return (_XParticleActionBase*) new _XParticleAction_OrbitLine;
	case _XPA_OrbitPointID		: return (_XParticleActionBase*) new _XParticleAction_OrbitPoint;
	case _XPA_RandomAccelID		: return (_XParticleActionBase*) new _XParticleAction_RandomAccel;
	case _XPA_RandomDisplaceID	: return (_XParticleActionBase*) new _XParticleAction_RandomDisplace;
	case _XPA_RandomVelocityID	: return (_XParticleActionBase*) new _XParticleAction_RandomVelocity;
	case _XPA_RestoreID			: return (_XParticleActionBase*) new _XParticleAction_Restore;
	case _XPA_SinkID			: return (_XParticleActionBase*) new _XParticleAction_Sink;
	case _XPA_SinkVelocityID	: return (_XParticleActionBase*) new _XParticleAction_SinkVelocity;
	case _XPA_SourceID			: return (_XParticleActionBase*) new _XParticleAction_Source;
	case _XPA_SpeedLimitID		: return (_XParticleActionBase*) new _XParticleAction_SpeedLimit;
	case _XPA_TargetColorID		: return (_XParticleActionBase*) new _XParticleAction_TargetColor;
	case _XPA_TargetSizeID		: return (_XParticleActionBase*) new _XParticleAction_TargetSize;
	case _XPA_TargetVelocityID	: return (_XParticleActionBase*) new _XParticleAction_TargetVelocity;
	case _XPA_VortexID			: return (_XParticleActionBase*) new _XParticleAction_Vortex;
	case _XPA_VertexID			: return (_XParticleActionBase*) new _XParticleAction_Vertex;
	case _XPA_StopVolumeID		: return (_XParticleActionBase*) new _XParticleAction_StopVolume;
	case _XPA_EffectSoundID		: return (_XParticleActionBase*) new _XParticleAction_EffectSound;
	case _XPA_EffectLightID		: return (_XParticleActionBase*) new _XParticleAction_EffectLight;
	case _XPA_EffectScreenID	: return (_XParticleActionBase*) new _XParticleAction_EffectScreen;
	case _XPA_CameraShakeID		: return (_XParticleActionBase*) new _XParticleAction_CameraShake;
	}
	return NULL;
}

BOOL _XParticleActionBase::Load( FILE* fileptr )
{
	if( !fileptr ) return FALSE;

	//if( fread( &actiontype, sizeof(_XParticleActionType), 1, fileptr ) < 1 ) return FALSE; // use LoadActionType(..) function...
	
	if( fread( &enable, sizeof(BOOL), 1, fileptr ) < 1 ) return FALSE;
		
	int timeslicecount = 0;
	if( fread( &timeslicecount, sizeof(int), 1, fileptr ) < 1 ) return FALSE;

	timetracklist.disposeList();

	_XTimeSlice* pTimeSlice;

	for( int i = 0; i < timeslicecount; i++ )
	{
		pTimeSlice = new _XTimeSlice;
		if( !pTimeSlice->Load( fileptr ) )
		{
			delete pTimeSlice;
			return FALSE;
		}
		
		timetracklist.resetList();
		if( !timetracklist.insertItem( pTimeSlice ) ) return FALSE;
	}	
	
	return LoadSubClassData(fileptr);
}

//////////////////////////////////////////////////////////////////////
// Particle action 
BOOL _XParticleAction_Avoid::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( !position.Load( fileptr ) ) return FALSE;
	if( fread( &look_ahead, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &magnitude, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &epsilon, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;		
	return TRUE;
}

void _XParticleAction_Avoid::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float magdt = magnitude * dt;
	
	switch(position.type)
	{
	case _XPE_Plane:
		{
			if(look_ahead < _XP_MAXFLOAT)
			{
				for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
				{						
					_XParticleElement &m = (*(*it));
					
					// p2 stores the plane normal (the a,b,c of the plane eqn).
					// Old and new distances: dist(p,plane) = n * p + d
					// radius1 stores -n*p, which is d.
					float dist = D3DXVec3Dot( &m.position, &position.p2) + position.radius1;
					
					if(dist < look_ahead)
					{
						float vm = D3DXVec3Length( &m.velocity );
						D3DXVECTOR3 Vn = m.velocity / vm;
						// float dot = Vn * position.p2;
						
						D3DXVECTOR3 tmp = (position.p2 * (magdt / (dist*dist+epsilon))) + Vn;
						//m.velocity = tmp * (vm / tmp.length());
						m.velocity = tmp * (vm / D3DXVec3Length( &tmp ));
					}
				}
			}
			else
			{
				for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
				{						
					_XParticleElement &m = (*(*it));
					
					// p2 stores the plane normal (the a,b,c of the plane eqn).
					// Old and new distances: dist(p,plane) = n * p + d
					// radius1 stores -n*p, which is d.
					float dist = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
					
					float vm = D3DXVec3Length( &m.velocity );
					D3DXVECTOR3 Vn = m.velocity / vm;
					// float dot = Vn * position.p2;
					
					D3DXVECTOR3 tmp = (position.p2 * (magdt / (dist*dist+epsilon))) + Vn;
					m.velocity = tmp * ( vm / D3DXVec3Length( &tmp ) );
				}
			}
		}
		break;
	case _XPE_Rectangle:
		{
			// Compute the inverse matrix of the plane basis.
			D3DXVECTOR3 &u = position.u;
			D3DXVECTOR3 &v = position.v;
			
			// The normalized bases are needed inside the loop.
			D3DXVECTOR3 un = u / position.radius1Sqr;
			D3DXVECTOR3 vn = v / position.radius2Sqr;
			
			// w = u cross v
			float wx = u.y*v.z-u.z*v.y;
			float wy = u.z*v.x-u.x*v.z;
			float wz = u.x*v.y-u.y*v.x;
			
			float det = 1/(wz*u.x*v.y-wz*u.y*v.x-u.z*wx*v.y-u.x*v.z*wy+v.z*wx*u.y+u.z*v.x*wy);
			
			D3DXVECTOR3 s1((v.y*wz-v.z*wy), (v.z*wx-v.x*wz), (v.x*wy-v.y*wx));
			s1 *= det;
			D3DXVECTOR3 s2((u.y*wz-u.z*wy), (u.z*wx-u.x*wz), (u.x*wy-u.y*wx));
			s2 *= -det;
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext( m.position + m.velocity * dt * look_ahead );
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1;
				
				// Opposite signs if product < 0
				// There is no faster way to do this.
				if(distold * distnew >= 0)
					continue;
				
				float nv = D3DXVec3Dot( &position.p2, &m.velocity );
				float t = -distold / nv;
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, p - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				// Dot product with basis vectors of old frame
				// in terms of new frame gives position in uv frame.
				float upos = D3DXVec3Dot( &offset,&s1 );
				float vpos = D3DXVec3Dot( &offset,&s2 );
				
				// Did it cross plane outside triangle?
				if(upos < 0 || vpos < 0 || upos > 1 || vpos > 1) continue;
				
				// A hit! A most palpable hit!
				// Compute distance to the three edges.
				D3DXVECTOR3 uofs = ( un * (D3DXVec3Dot( &un,&offset)) ) - offset;
				float udistSqr = D3DXVec3LengthSq( &uofs );
				D3DXVECTOR3 vofs = ( vn * (D3DXVec3Dot( &vn,&offset)) ) - offset;
				float vdistSqr = D3DXVec3LengthSq( &vofs );
				
				D3DXVECTOR3 foffset((u + v) - offset);
				D3DXVECTOR3 fofs = (un * ( D3DXVec3Dot( &un, &foffset) )) - foffset;
				float fdistSqr = D3DXVec3LengthSq( &fofs );
				D3DXVECTOR3 gofs = (un * ( D3DXVec3Dot( &un, &foffset) )) - foffset;
				float gdistSqr = D3DXVec3LengthSq( &gofs );
				
				D3DXVECTOR3 S;
				if(udistSqr <= vdistSqr && udistSqr <= fdistSqr
					&& udistSqr <= gdistSqr) S = uofs;
				else if(vdistSqr <= fdistSqr && vdistSqr <= gdistSqr) S = vofs;
				else if(fdistSqr <= gdistSqr) S = fofs;
				else S = gofs;
				
				D3DXVec3Normalize( &S, &S );
				
				// We now have a vector to safety.
				float vm = D3DXVec3Length( &m.velocity );
				D3DXVECTOR3 Vn = m.velocity / vm;
				
				// Blend S into V.
				D3DXVECTOR3 tmp = (S * (magdt / (t*t+epsilon))) + Vn;
				m.velocity = tmp * (vm / D3DXVec3Length( &tmp ) );
			}
		}
		break;
	case _XPE_Triangle:
		{
			// Compute the inverse matrix of the plane basis.
			D3DXVECTOR3 &u = position.u;
			D3DXVECTOR3 &v = position.v;
			
			// The normalized bases are needed inside the loop.
			D3DXVECTOR3 un = u / position.radius1Sqr;
			D3DXVECTOR3 vn = v / position.radius2Sqr;
			
			// f is the third (non-basis) triangle edge.
			D3DXVECTOR3 f = v - u;
			D3DXVECTOR3 fn(f);
			D3DXVec3Normalize( &fn, &fn );
			
			// w = u cross v
			float wx = u.y*v.z-u.z*v.y;
			float wy = u.z*v.x-u.x*v.z;
			float wz = u.x*v.y-u.y*v.x;
			
			float det = 1/(wz*u.x*v.y-wz*u.y*v.x-u.z*wx*v.y-u.x*v.z*wy+v.z*wx*u.y+u.z*v.x*wy);
			
			D3DXVECTOR3 s1((v.y*wz-v.z*wy), (v.z*wx-v.x*wz), (v.x*wy-v.y*wx));
			s1 *= det;
			D3DXVECTOR3 s2((u.y*wz-u.z*wy), (u.z*wx-u.x*wz), (u.x*wy-u.y*wx));
			s2 *= -det;
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt * look_ahead);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1;
				
				// Opposite signs if product < 0
				// Is there a faster way to do this?
				if(distold * distnew >= 0)
					continue;
				
				float nv = D3DXVec3Dot( &position.p2, &m.velocity );
				float t = -distold / nv;
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, p - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				// Dot product with basis vectors of old frame
				// in terms of new frame gives position in uv frame.
				float upos = D3DXVec3Dot( &offset, &s1 );
				float vpos = D3DXVec3Dot( &offset, &s2 );
				
				// Did it cross plane outside triangle?
				if(upos < 0 || vpos < 0 || (upos + vpos) > 1) continue;
				
				// A hit! A most palpable hit!
				// Compute distance to the three edges.
				D3DXVECTOR3 uofs = (un * ( D3DXVec3Dot( &un, &offset) )) - offset;
				float udistSqr = D3DXVec3LengthSq( &uofs );
				D3DXVECTOR3 vofs = (vn * ( D3DXVec3Dot( &vn, &offset) )) - offset;
				float vdistSqr = D3DXVec3LengthSq( &vofs );
				D3DXVECTOR3 foffset(offset - u);
				D3DXVECTOR3 fofs = (fn * ( D3DXVec3Dot( &fn, &foffset) )) - foffset;
				float fdistSqr = D3DXVec3LengthSq( &fofs );
				D3DXVECTOR3 S;
				if(udistSqr <= vdistSqr && udistSqr <= fdistSqr) S = uofs;
				else if(vdistSqr <= fdistSqr) S = vofs;
				else S = fofs;
				
				D3DXVec3Normalize( &S, &S );
				
				// We now have a vector to safety.
				float vm = D3DXVec3Length( &m.velocity );
				D3DXVECTOR3 Vn = m.velocity / vm;
				
				// Blend S into V.
				D3DXVECTOR3 tmp = (S * (magdt / (t*t+epsilon))) + Vn;
				m.velocity = tmp * ( vm / D3DXVec3Length( &tmp ) );
			}
		}
		break;
	case _XPE_Disc:
		{
			float r1Sqr = fsqr(position.radius1);
			float r2Sqr = fsqr(position.radius2);
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt * look_ahead);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d. radius1Sqr stores d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1Sqr;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1Sqr;
				
				// Opposite signs if product < 0
				// Is there a faster way to do this?
				if(distold * distnew >= 0)
					continue;
				
				// Find position at the crossing point by parameterizing
				// p(t) = pos + vel * t
				// Solve dist(p(t),plane) = 0 e.g.
				// n * p(t) + D = 0 ->
				// n * p + t (n * v) + D = 0 ->
				// t = -(n * p + D) / (n * v)
				// Could factor n*v into distnew = distold + n*v and save a bit.
				// Safe since n*v != 0 assured by quick rejection test.
				// This calc is indep. of dt because we have established that it
				// will hit before dt. We just want to know when.
				float nv = D3DXVec3Dot( &position.p2, &m.velocity );
				float t = -distold / nv;
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, phit - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				float rad = D3DXVec3LengthSq( &offset );
				
				if(rad > r1Sqr || rad < r2Sqr)
					continue;
				
				// A hit! A most palpable hit!
				D3DXVECTOR3 S = offset;
				D3DXVec3Normalize( &S, &S );
				
				// We now have a vector to safety.
				float vm = D3DXVec3Length( &m.velocity );
				D3DXVECTOR3 Vn = m.velocity / vm;
				
				// Blend S into V.
				D3DXVECTOR3 tmp = ( S * (magdt / (t*t+epsilon)) ) + Vn;
				m.velocity = tmp * ( vm / D3DXVec3Length( &tmp ) );
			}
		}
		break;
	case _XPE_Sphere:
		{
			float rSqr = position.radius1 * position.radius1;
			
			// See which particles are aimed toward the sphere.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// First do a ray-sphere intersection test and
				// see if it's soon enough.
				// Can I do this faster without t?
				float vm = D3DXVec3Length( &m.velocity );
				D3DXVECTOR3 Vn = m.velocity / vm;
				
				D3DXVECTOR3 L = position.p1 - m.position;
				float v = D3DXVec3Dot( &L, &Vn );
				
				float disc = rSqr - D3DXVec3Dot(&L, &L) + v * v;
				if(disc < 0)
					continue; // I'm not heading toward it.
				
				// Compute length for second rejection test.
				float t = v - _XFC_sqrt(disc);
				if(t < 0 || t > (vm * look_ahead))
					continue;
				
				// Get a vector to safety.
				D3DXVECTOR3 C;
				D3DXVec3Cross( &C, &Vn, &L );
				D3DXVec3Normalize( &C, &C );
				D3DXVECTOR3 S;
				D3DXVec3Cross( &S, &Vn, &C );
				
				// Blend S into V.
				D3DXVECTOR3 tmp = (S * (magdt / (t*t+epsilon))) + Vn;
				m.velocity = tmp * (vm / D3DXVec3Length( &tmp ));
			}
		}
		break;
	}
}

BOOL _XParticleAction_Bounce::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( !position.Load( fileptr ) ) return FALSE;
	if( fread( &oneMinusFriction, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &resilience, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &cutoffSqr, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;			
	return TRUE;
}

void _XParticleAction_Bounce::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;
	
	switch(position.type)
	{
	case _XPE_Triangle:
		{
			// Compute the inverse matrix of the plane basis.
			D3DXVECTOR3 &u = position.u;
			D3DXVECTOR3 &v = position.v;
			
			// w = u cross v
			float wx = u.y*v.z-u.z*v.y;
			float wy = u.z*v.x-u.x*v.z;
			float wz = u.x*v.y-u.y*v.x;
			
			float det = 1/(wz*u.x*v.y-wz*u.y*v.x-u.z*wx*v.y-u.x*v.z*wy+v.z*wx*u.y+u.z*v.x*wy);
			
			D3DXVECTOR3 s1((v.y*wz-v.z*wy), (v.z*wx-v.x*wz), (v.x*wy-v.y*wx));
			s1 *= det;
			D3DXVECTOR3 s2((u.y*wz-u.z*wy), (u.z*wx-u.x*wz), (u.x*wy-u.y*wx));
			s2 *= -det;
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1;
				
				// Opposite signs if product < 0
				// Is there a faster way to do this?
				if(distold * distnew >= 0)
					continue;
				
				// Find position at the crossing point by parameterizing
				// p(t) = pos + vel * t
				// Solve dist(p(t),plane) = 0 e.g.
				// n * p(t) + D = 0 ->
				// n * p + t (n * v) + D = 0 ->
				// t = -(n * p + D) / (n * v)
				// Could factor n*v into distnew = distold + n*v and save a bit.
				// Safe since n*v != 0 assured by quick rejection test.
				// This calc is indep. of dt because we have established that it
				// will hit before dt. We just want to know when.
				float nv = D3DXVec3Dot( &position.p2, &m.velocity );
				float t = -distold / nv;
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, p - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				// Dot product with basis vectors of old frame
				// in terms of new frame gives position in uv frame.
				float upos = D3DXVec3Dot( &offset, &s1 );
				float vpos = D3DXVec3Dot( &offset, &s2 );
				
				// Did it cross plane outside triangle?
				if(upos < 0 || vpos < 0 || (upos + vpos) > 1)
					continue;
				
				// A hit! A most palpable hit!
				
				// Compute tangential and normal components of velocity
				D3DXVECTOR3 vn(position.p2 * nv); // Normal Vn = (V.N)N
				D3DXVECTOR3 vt(m.velocity - vn); // Tangent Vt = V - Vn
				
				// Compute new velocity heading out:
				// Don't apply friction if tangential velocity < cutoff
				if( D3DXVec3LengthSq( &vt ) <= cutoffSqr)
					m.velocity = vt - (vn * resilience);
				else
					m.velocity = (vt * oneMinusFriction) - (vn * resilience);
			}
		}
		break;
	case _XPE_Disc:
		{
			float r1Sqr = fsqr(position.radius1);
			float r2Sqr = fsqr(position.radius2);
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d. radius1Sqr stores d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1Sqr;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1Sqr;
				
				// Opposite signs if product < 0
				// Is there a faster way to do this?
				if(distold * distnew >= 0)
					continue;
				
				// Find position at the crossing point by parameterizing
				// p(t) = pos + vel * t
				// Solve dist(p(t),plane) = 0 e.g.
				// n * p(t) + D = 0 ->
				// n * p + t (n * v) + D = 0 ->
				// t = -(n * p + D) / (n * v)
				// Could factor n*v into distnew = distold + n*v and save a bit.
				// Safe since n*v != 0 assured by quick rejection test.
				// This calc is indep. of dt because we have established that it
				// will hit before dt. We just want to know when.
				float nv = D3DXVec3Dot( &position.p2, &m.velocity );
				float t = -distold / nv;
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, phit - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				float rad = D3DXVec3LengthSq( &offset );
				
				if(rad > r1Sqr || rad < r2Sqr)
					continue;
				
				// A hit! A most palpable hit!
				
				// Compute tangential and normal components of velocity
				D3DXVECTOR3 vn(position.p2 * nv); // Normal Vn = (V.N)N
				D3DXVECTOR3 vt(m.velocity - vn); // Tangent Vt = V - Vn
				
				// Compute new velocity heading out:
				// Don't apply friction if tangential velocity < cutoff
				if( D3DXVec3LengthSq( &vt ) <= cutoffSqr)
					m.velocity = vt - vn * resilience;
				else
					m.velocity = vt * oneMinusFriction - vn * resilience;
			}
		}
		break;
	case _XPE_Plane:
		{
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1;
				
				// Opposite signs if product < 0
				if(distold * distnew >= 0)
					continue;
				
				// Compute tangential and normal components of velocity
				float nmag = D3DXVec3Dot( &m.velocity, &position.p2 );
				D3DXVECTOR3 vn(position.p2 * nmag); // Normal Vn = (V.N)N
				D3DXVECTOR3 vt(m.velocity - vn); // Tangent Vt = V - Vn
				
				// Compute new velocity heading out:
				// Don't apply friction if tangential velocity < cutoff
				if( D3DXVec3LengthSq( &vt ) <= cutoffSqr)
					m.velocity = vt - vn * resilience;
				else
					m.velocity = vt * oneMinusFriction - vn * resilience;
			}
		}
		break;
	case _XPE_Rectangle:
		{
			// Compute the inverse matrix of the plane basis.
			D3DXVECTOR3 &u = position.u;
			D3DXVECTOR3 &v = position.v;
			
			// w = u cross v
			float wx = u.y*v.z-u.z*v.y;
			float wy = u.z*v.x-u.x*v.z;
			float wz = u.x*v.y-u.y*v.x;
			
			float det = 1/(wz*u.x*v.y-wz*u.y*v.x-u.z*wx*v.y-u.x*v.z*wy+v.z*wx*u.y+u.z*v.x*wy);
			
			D3DXVECTOR3 s1((v.y*wz-v.z*wy), (v.z*wx-v.x*wz), (v.x*wy-v.y*wx));
			s1 *= det;
			D3DXVECTOR3 s2((u.y*wz-u.z*wy), (u.z*wx-u.x*wz), (u.x*wy-u.y*wx));
			s2 *= -det;
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1;
				
				// Opposite signs if product < 0
				if(distold * distnew >= 0)
					continue;
				
				// Find position at the crossing point by parameterizing
				// p(t) = pos + vel * t
				// Solve dist(p(t),plane) = 0 e.g.
				// n * p(t) + D = 0 ->
				// n * p + t (n * v) + D = 0 ->
				// t = -(n * p + D) / (n * v)
				float t = -distold / ( D3DXVec3Dot( &position.p2, &m.velocity ) );
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, p - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				// Dot product with basis vectors of old frame
				// in terms of new frame gives position in uv frame.
				float upos = D3DXVec3Dot( &offset, &s1 );
				float vpos = D3DXVec3Dot( &offset, &s2 );
				
				// Crossed plane outside bounce region if !(0<=[uv]pos<=1)
				if(upos < 0 || upos > 1 || vpos < 0 || vpos > 1)
					continue;
				
				// A hit! A most palpable hit!
				
				// Compute tangential and normal components of velocity
				float nmag = D3DXVec3Dot( &m.velocity, &position.p2 );
				D3DXVECTOR3 vn(position.p2 * nmag); // Normal Vn = (V.N)N
				D3DXVECTOR3 vt(m.velocity - vn); // Tangent Vt = V - Vn
				
				// Compute new velocity heading out:
				// Don't apply friction if tangential velocity < cutoff
				if( D3DXVec3LengthSq( &vt ) <= cutoffSqr)
					m.velocity = vt - vn * resilience;
				else
					m.velocity = vt * oneMinusFriction - vn * resilience;
			}
		}
		break;
	case _XPE_Sphere:
		{
			// Sphere that particles bounce off
			// The particles are always forced out of the sphere.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's next position is inside domain.
				// If so, bounce it.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				if(position.Within(pnext))
				{
					// See if we were inside on previous timestep.
					bool pinside = position.Within(m.position);
					
					// Normal to surface. This works for a sphere. Isn't
					// computed quite right, should extrapolate particle
					// position to surface.
					D3DXVECTOR3 n(m.position - position.p1);
					D3DXVec3Normalize( &n, &n );
					
					// Compute tangential and normal components of velocity
					float nmag = D3DXVec3Dot( &m.velocity, &n );
					
					D3DXVECTOR3 vn(n * nmag); // Normal Vn = (V.N)N
					D3DXVECTOR3 vt = m.velocity - vn; // Tangent Vt = V - Vn
					
					if(pinside)
					{
						// Previous position was inside. If normal component of
						// velocity points in, reverse it. This effectively
						// repels particles which would otherwise be trapped
						// in the sphere.
						if(nmag < 0)
							m.velocity = vt - vn;
					}
					else
					{
						// Previous position was outside -> particle will cross
						// surface boundary. Reverse normal component of velocity,
						// and apply friction (if Vt >= cutoff) and resilience.
						
						// Compute new velocity heading out:
						// Don't apply friction if tangential velocity < cutoff
						if( D3DXVec3LengthSq( &vt ) <= cutoffSqr)
							m.velocity = vt - vn * resilience;
						else
							m.velocity = vt * oneMinusFriction - vn * resilience;
					}
				}
			}
		}
	}
}

BOOL _XParticleAction_CallActionList::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &effecttemplateid, sizeof(int), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &usetargetposition, sizeof(DWORD), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &insideparticles, sizeof(DWORD), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &usestopvolumeeffect, sizeof(BOOL), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &enablesink, sizeof(BOOL), 1, fileptr ) < 1 ) return FALSE;
	if( !position.Load( fileptr ) ) return FALSE;
	return TRUE;
}

// Set the secondary position of each particle to be its position.
void _XParticleAction_CallActionList::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	int currentinsidedparticles = 0;

	D3DXVECTOR3 prevCenter = position.p1;
	
	if( usetargetposition == 0 )		
	{
	}	
	else if( usetargetposition == 1 )
	{
		if( !g_pLastEventTargetModelDescriptor )
		{		
			if( pParticleList->m_UsedTargetMatrix )
			{
				D3DXMATRIX characterinvmat;
				characterinvmat = pParticleList->m_pParentModelDescriptor->m_Position;
				characterinvmat._41 = characterinvmat._42 = characterinvmat._43 = 0.0f;
				D3DXMatrixInverse( &characterinvmat, NULL, &characterinvmat );
				
				position.p1.x = pParticleList->m_LastUsedTargetPosition.x - pParticleList->m_pParentModelDescriptor->m_Position._41;
				position.p1.y = pParticleList->m_LastUsedTargetPosition.y - pParticleList->m_pParentModelDescriptor->m_Position._42;
				position.p1.z = pParticleList->m_LastUsedTargetPosition.z - pParticleList->m_pParentModelDescriptor->m_Position._43;
				D3DXVec3TransformCoord( &position.p1, &position.p1, &characterinvmat );				
			}
		}
		else
		{
			D3DXMATRIX characterinvmat;
			characterinvmat = pParticleList->m_pParentModelDescriptor->m_Position;
			characterinvmat._41 = characterinvmat._42 = characterinvmat._43 = 0.0f;
			D3DXMatrixInverse( &characterinvmat, NULL, &characterinvmat );

			position.p1.x = g_pLastEventTargetModelDescriptor->m_Position._41 - pParticleList->m_pParentModelDescriptor->m_Position._41;
			position.p1.y = g_pLastEventTargetModelDescriptor->m_Position._42 - pParticleList->m_pParentModelDescriptor->m_Position._42;
			position.p1.z = g_pLastEventTargetModelDescriptor->m_Position._43 - pParticleList->m_pParentModelDescriptor->m_Position._43;			
			D3DXVec3TransformCoord( &position.p1, &position.p1, &characterinvmat );

			// Backup
			pParticleList->m_UsedTargetMatrix = TRUE;
			pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41;
			pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42;
			pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43;
		}
	}
	else // link biped node...
	{
		if( !g_pLastEventTargetModelDescriptor )
		{		
			if( pParticleList->m_UsedTargetMatrix )
			{
				D3DXMATRIX characterinvmat;
				characterinvmat = pParticleList->m_pParentModelDescriptor->m_Position;
				characterinvmat._41 = characterinvmat._42 = characterinvmat._43 = 0.0f;
				D3DXMatrixInverse( &characterinvmat, NULL, &characterinvmat );
				
				position.p1.x = pParticleList->m_LastUsedTargetPosition.x - pParticleList->m_pParentModelDescriptor->m_Position._41;
				position.p1.y = pParticleList->m_LastUsedTargetPosition.y - pParticleList->m_pParentModelDescriptor->m_Position._42;
				position.p1.z = pParticleList->m_LastUsedTargetPosition.z - pParticleList->m_pParentModelDescriptor->m_Position._43;
				D3DXVec3TransformCoord( &position.p1, &position.p1, &characterinvmat );				
			}
		}
		else
		{
			D3DXMATRIX characterinvmat;
			characterinvmat = pParticleList->m_pParentModelDescriptor->m_Position;
			D3DXMatrixInverse( &characterinvmat, NULL, &characterinvmat );
			
			D3DXVECTOR3 targetworldposition = D3DXVECTOR3(0.0f,0.0f,0.0f);
			if( g_pLastEventTargetModelDescriptor->m_pBipedController )
			{
				_LPXM_BIPEDOBJECT lpNode = g_pLastEventTargetModelDescriptor->m_pBipedController->GetNode(
											g_pLastEventTargetModelDescriptor->m_pBipedController->m_CharacterPartIndex[usetargetposition - 2] );
				if (lpNode)
				{
					D3DXVec3TransformCoord( &targetworldposition, &targetworldposition, &lpNode->m_matTM );					
				}
			}
			
			D3DXVec3TransformCoord( &targetworldposition, &targetworldposition, &g_pLastEventTargetModelDescriptor->m_Position );
			
			D3DXVec3TransformCoord( &position.p1, &targetworldposition, &characterinvmat );

			// Backup
			pParticleList->m_UsedTargetMatrix = TRUE;
			pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41;
			pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42;
			pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43;
		}
	}

	switch(position.type)
	{
	case _XPE_Sphere:
		{
			// Sphere that particles bounce off
			// The particles are always forced out of the sphere.

			sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin();
			while(it != pParticleList->m_slParticleElement.end())
			{						
				BOOL particledeleted = FALSE;
				_XParticleElement &m = (*(*it));

				if( position.Within(m.position) )
				{
					currentinsidedparticles++;

					if( usestopvolumeeffect )
					{
						if( enablesink )
						{
							pParticleList->RemoveParticle( *it );
							it = pParticleList->m_slParticleElement.erase(it);
							particledeleted = TRUE;

							if( currentinsidedparticles >= insideparticles )
							{
								pParticleList->DeleteAllParticles();
								pParticleList->m_FadeMode = TRUE;

								if( g_LastEventCount < _XDEF_MAXEVENTACTIONINTANCINGCOUNT )
								{
									g_LastEventParticleIDArray[g_LastEventCount] = effecttemplateid;
									g_LastEventCount++;
								}
								
								position.p1 = prevCenter;
								return;
							}
						}
						else
						{
							m.velocity /= 10.0f;
						}
					}
					else
					{
						if( enablesink )
						{
							if( currentinsidedparticles >= insideparticles )
							{
								pParticleList->DeleteAllParticles();
								pParticleList->m_FadeMode = TRUE;

								if( g_LastEventCount < _XDEF_MAXEVENTACTIONINTANCINGCOUNT )
								{
									g_LastEventParticleIDArray[g_LastEventCount] = effecttemplateid;
									g_LastEventCount++;
								}
								
								position.p1 = prevCenter;
								return;
							}
							else
							{
								pParticleList->RemoveParticle( *it );
								it = pParticleList->m_slParticleElement.erase(it);
								particledeleted = TRUE;
							}
						}
					}
				}
				
				if( !particledeleted ) 
					++it;
/*
				// See if particle's next position is inside domain.
				// If so, bounce it.
				
				D3DXVECTOR3 pnext( m.position + m.velocity * dt);
				D3DXVECTOR3 pnext2( m.position + m.velocity * (position.radius1 > position.radius2 ? position.radius1 : position.radius2) );
												
				if( position.Within(pnext) || position.LineVsSphereIntersect( m.position, pnext2 ) )
				{
					// Normal to surface. This works for a sphere. Isn't
					// computed quite right, should extrapolate particle
					// position to surface.
					//D3DXVECTOR3 n(m.position - position.p1);
					//D3DXVec3Normalize( &n, &n );
					
					// Compute tangential and normal components of velocity
					//float nmag = D3DXVec3Dot( &m.velocity, &n );
					
					//D3DXVECTOR3 vn(n * nmag); // Normal Vn = (V.N)N
					//D3DXVECTOR3 vt = m.velocity - vn; // Tangent Vt = V - Vn
					
					//if(nmag <= 0)
					{
						//m.velocity = vt - vn;

						currentinsidedparticles++;

						if( usestopvolumeeffect )
						{
							if( enablesink )
							{
								pParticleList->RemoveParticle( *it );
								it = pParticleList->m_slParticleElement.erase(it);
								particledeleted = TRUE;
							}
							else
							{
								m.velocity /= 10.0f;
							}
						}
						//else
						{
							if( currentinsidedparticles >= insideparticles )
							{
								pParticleList->DeleteAllParticles();
								pParticleList->m_FadeMode = TRUE;

								if( g_LastEventCount < _XDEF_MAXEVENTACTIONINTANCINGCOUNT )
								{
									g_LastEventParticleIDArray[g_LastEventCount] = effecttemplateid;
									g_LastEventCount++;
								}
								
								position.p1 = prevCenter;
								return;
							}
						}
					}
				}
				if( !particledeleted ) 
					++it;
*/
			}
		}
	}

	if( usestopvolumeeffect )
	{
		if( currentinsidedparticles >= insideparticles )
		{
			if( effecttemplateid )
			{
				if( g_LastEventCount < _XDEF_MAXEVENTACTIONINTANCINGCOUNT )
				{
					g_LastEventParticleIDArray[g_LastEventCount] = effecttemplateid;
					g_LastEventCount++;
				}
			}
		}
	}

	position.p1 = prevCenter;
}

BOOL _XParticleAction_CopyVertexB::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;	
	if( fread( &copy_pos, sizeof(bool), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &copy_vel, sizeof(bool), 1, fileptr ) < 1 ) return FALSE;	
	return TRUE;
}

// Set the secondary position of each particle to be its position.
void _XParticleAction_CopyVertexB::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;


	if(copy_pos)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			m.positionB = m.position;
		}
	}
	
	if(copy_vel)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			m.velocityB = m.velocity;
		}
	}
}

BOOL _XParticleAction_Damping::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &damping, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &vlowSqr, sizeof(float), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &vhighSqr, sizeof(float), 1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

// Dampen velocities
void _XParticleAction_Damping::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	// This is important if dt is != 1.
	D3DXVECTOR3 one(1,1,1);
	D3DXVECTOR3 scale(one - ((one - damping) * dt));
	
	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));
		float vSqr = D3DXVec3LengthSq( &m.velocity );
		
		if(vSqr >= vlowSqr && vSqr <= vhighSqr)
		{
			m.velocity.x *= scale.x;
			m.velocity.y *= scale.y;
			m.velocity.z *= scale.z;
		}
	}
}

BOOL _XParticleAction_Explosion::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &center, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &velocity, sizeof(float), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &magnitude, sizeof(float), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &stdev, sizeof(float), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &age, sizeof(float), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &epsilon, sizeof(float), 1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}


// Exert force on each particle away from explosion center
void _XParticleAction_Explosion::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float radius = velocity * age;
	float magdt = magnitude * dt;
	float oneOverSigma = 1.0f / stdev;
	float inexp = -0.5f*fsqr(oneOverSigma);
	float outexp = ONEOVERSQRT2PI * oneOverSigma;
	
	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));
		
		// Figure direction to particle.
		D3DXVECTOR3 dir(m.position - center);
		float distSqr = D3DXVec3LengthSq( &dir );
		float dist = _XFC_sqrt(distSqr);
		float DistFromWaveSqr = fsqr(radius - dist);
		
		float Gd = exp(DistFromWaveSqr * inexp) * outexp;
		
		m.velocity += dir * (Gd * magdt / (dist * (distSqr + epsilon)));
	}
	
	age += dt;
}

BOOL _XParticleAction_Follow::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &magnitude, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &epsilon, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &max_radius, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;			
	return TRUE;
}

// Follow the next particle in the list
void _XParticleAction_Follow::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float magdt = magnitude * dt;
	float max_radiusSqr = max_radius * max_radius;
	
	if(max_radiusSqr < _XP_MAXFLOAT)
	{
		if (pParticleList->m_slParticleElement.size() > 1)
		{
			sldef_XParticleElement::iterator lt = pParticleList->m_slParticleElement.end();
			-- lt;
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != lt; ++ it)
			{						
				_XParticleElement &m = (*(*it));

				// Accelerate toward the particle after me in the list.
				sldef_XParticleElement::iterator nt = it;
				++ nt;
				_XParticleElement &nm = (*(*nt));

				D3DXVECTOR3 tohim( nm.position - m.position ); // tohim = p1 - p0
				float tohimlenSqr = D3DXVec3LengthSq( &tohim );
				
				if(tohimlenSqr < max_radiusSqr)
				{
					// Compute force exerted between the two bodies
					m.velocity += tohim * (magdt / (_XFC_sqrt(tohimlenSqr) * (tohimlenSqr + epsilon)));
				}
			}
		}
	}
	else
	{
		if (pParticleList->m_slParticleElement.size() > 1)
		{
			sldef_XParticleElement::iterator lt = pParticleList->m_slParticleElement.end();
			-- lt;
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != lt; ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// Accelerate toward the particle after me in the list.
				sldef_XParticleElement::iterator nt = it;
				++ nt;
				_XParticleElement &nm = (*(*nt));

				D3DXVECTOR3 tohim( nm.position - m.position ); // tohim = p1 - p0
				float tohimlenSqr = D3DXVec3LengthSq( &tohim );
				
				// Compute force exerted between the two bodies
				m.velocity += tohim * (magdt / (_XFC_sqrt(tohimlenSqr) * (tohimlenSqr + epsilon)));
			}
		}
	}
}


BOOL _XParticleAction_Gravitate::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &magnitude, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &epsilon, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &max_radius, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;		
	return TRUE;
}

// Inter-particle gravitation
void _XParticleAction_Gravitate::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float magdt = magnitude * dt;
	float max_radiusSqr = max_radius * max_radius;
	
	if(max_radiusSqr < _XP_MAXFLOAT)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Add interactions with other particles
			sldef_XParticleElement::iterator jt = it;
			++ jt;
			for (; jt != pParticleList->m_slParticleElement.end(); ++ jt)
			{
				_XParticleElement &mj = (*(*jt));
				
				D3DXVECTOR3 tohim(mj.position - m.position); // tohim = p1 - p0
				float tohimlenSqr = D3DXVec3LengthSq( &tohim );
				
				if(tohimlenSqr < max_radiusSqr)
				{
					// Compute force exerted between the two bodies
					D3DXVECTOR3 acc(tohim * (magdt / (_XFC_sqrt(tohimlenSqr) * (tohimlenSqr + epsilon))));
					
					m.velocity  += acc;
					mj.velocity -= acc;
				}
			}
		}
	}
	else
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Add interactions with other particles
			sldef_XParticleElement::iterator jt = it;
			++ jt;
			for (; jt != pParticleList->m_slParticleElement.end(); ++ jt)
			{
				_XParticleElement &mj = (*(*jt));
				
				D3DXVECTOR3 tohim(mj.position - m.position); // tohim = p1 - p0
				float tohimlenSqr = D3DXVec3LengthSq( &tohim );
				
				// Compute force exerted between the two bodies
				D3DXVECTOR3 acc(tohim * (magdt / (_XFC_sqrt(tohimlenSqr) * (tohimlenSqr + epsilon))));
				
				m.velocity  += acc;
				mj.velocity -= acc;
			}
		}
	}
}

BOOL _XParticleAction_Gravity::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &direction, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;	
	return TRUE;
}

// Acceleration in a constant direction
void _XParticleAction_Gravity::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	D3DXVECTOR3 ddir(direction * dt);
	
	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));

		// Step velocity with acceleration
		m.velocity += ddir;
	}
}

BOOL _XParticleAction_Jet::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &center, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;	
	if( !acc.Load( fileptr ) ) return FALSE;
	if( fread( &magnitude, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &epsilon, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &max_radius, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

// Accelerate particles along a line
void _XParticleAction_Jet::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float magdt = magnitude * dt;
	float max_radiusSqr = max_radius * max_radius;
	
	if(max_radiusSqr < _XP_MAXFLOAT)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Figure direction to particle.
			D3DXVECTOR3 dir(m.position - center);
			
			// Distance to jet (force drops as 1/r^2)
			// Soften by epsilon to avoid tight encounters to infinity
			float rSqr = D3DXVec3LengthSq( &dir );
			
			if(rSqr < max_radiusSqr)
			{
				D3DXVECTOR3 accel;
				acc.Generate(accel);
				
				// Step velocity with acceleration
				m.velocity += accel * (magdt / (rSqr + epsilon));
			}
		}
	}
	else
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Figure direction to particle.
			D3DXVECTOR3 dir(m.position - center);
			
			// Distance to jet (force drops as 1/r^2)
			// Soften by epsilon to avoid tight encounters to infinity
			float rSqr = D3DXVec3LengthSq( &dir );
			
			D3DXVECTOR3 accel;
			acc.Generate(accel);
			
			// Step velocity with acceleration
			m.velocity += accel * (magdt / (rSqr + epsilon));
		}
	}
}

BOOL _XParticleAction_KillOld::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &age_limit, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &kill_less_than, sizeof(bool), 1, fileptr ) < 1 ) return FALSE;	
	return TRUE;
}

// Get rid of older particles
void _XParticleAction_KillOld::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	// Must traverse list in reverse order so Remove will work
	sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin();
	while(it != pParticleList->m_slParticleElement.end())
	{
		_XParticleElement &m = (*(*it));
		if(!((m.age < age_limit) ^ kill_less_than))
		{
			pParticleList->RemoveParticle( *it );
			it = pParticleList->m_slParticleElement.erase(it);
		}
		else
		{
			++ it;
		}
	}
}

BOOL _XParticleAction_MatchVelocity::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &magnitude, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &epsilon, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &max_radius, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;			
	return TRUE;
}

// Match velocity to near neighbors
void _XParticleAction_MatchVelocity::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float magdt = magnitude * dt;
	float max_radiusSqr = max_radius * max_radius;
	
	if(max_radiusSqr < _XP_MAXFLOAT)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Add interactions with other particles
			sldef_XParticleElement::iterator jt = it;
			++ jt;
			for(; jt != pParticleList->m_slParticleElement.end(); ++ jt)
			{
				_XParticleElement &mj = (*(*jt));
				
				D3DXVECTOR3 tohim(mj.position - m.position); // tohim = p1 - p0
				float tohimlenSqr = D3DXVec3LengthSq( &tohim );
				
				if(tohimlenSqr < max_radiusSqr)
				{
					// Compute force exerted between the two bodies
					D3DXVECTOR3 acc(mj.velocity * (magdt / (tohimlenSqr + epsilon)));
					
					m.velocity  += acc;
					mj.velocity -= acc;
				}
			}
		}
	}
	else
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Add interactions with other particles
			sldef_XParticleElement::iterator jt = it;
			++ jt;
			for(; jt != pParticleList->m_slParticleElement.end(); ++ jt)
			{
				_XParticleElement &mj = (*(*jt));
				
				D3DXVECTOR3 tohim(mj.position - m.position); // tohim = p1 - p0
				float tohimlenSqr = D3DXVec3LengthSq( &tohim );
				
				// Compute force exerted between the two bodies
				D3DXVECTOR3 acc(mj.velocity * (magdt / (tohimlenSqr + epsilon)));
				
				m.velocity  += acc;
				mj.velocity -= acc;
			}
		}
	}
}

void _XParticleAction_Move::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	if( pParticleList->m_pInstanceData->RenderingType == _XParticleRT_LineEffect )
	{
		if( pParticleList->m_pInstanceData->EmitterBipedLinkType >= _XParticleBLT_CharacterRightHand && pParticleList->m_pParentModelDescriptor )
		{
			int bipedindex = pParticleList->m_pInstanceData->EmitterBipedLinkType-_XParticleBLT_CharacterRightHand;

			D3DXVECTOR3 toppos,bottompos;						
			D3DXVec3TransformCoord( &toppos, &pParticleList->m_pInstanceData->m_LastEmitPosition[0], &pParticleList->m_pParentModelDescriptor->m_PartMatrixContainer[bipedindex]);
			D3DXVec3TransformCoord( &bottompos, &pParticleList->m_pInstanceData->m_LastEmitPosition[1], &pParticleList->m_pParentModelDescriptor->m_PartMatrixContainer[bipedindex]);
			
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{			
				_XParticleElement &m = (*(*it));
				m.age += dt;					
				m.position += m.velocity * dt;

				m.pLineData->InsertLastPoint( toppos, bottompos );
			}
		}
		else
		{
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{			
				_XParticleElement &m = (*(*it));
				m.age += dt;					
				m.position += m.velocity * dt;
			
				D3DXVECTOR3 toppos = m.position;
				D3DXVECTOR3 bottompos = m.position;
				toppos -= (m.size/2.0f);
				bottompos += (m.size/2.0f);
				m.pLineData->InsertLastPoint( toppos, bottompos );
			}		
		}
	}
	else
	{
		// Step particle positions forward by dt, and age the particles.
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			m.age += dt;
			m.position += m.velocity * dt;
		}
	}
}

BOOL _XParticleAction_OrbitLine::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &p, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &axis, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &magnitude, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &epsilon, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &max_radius, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &usetargetposition, sizeof(BOOL), 1, fileptr ) < 1 ) return FALSE;	
	return TRUE;
}

// Accelerate particles towards a line
void _XParticleAction_OrbitLine::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float magdt = magnitude * dt;
	float max_radiusSqr = max_radius * max_radius;
	
	if(max_radiusSqr < _XP_MAXFLOAT)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Figure direction to particle from base of line.
			D3DXVECTOR3 f;
			if( usetargetposition == 0 || !g_pLastEventTargetModelDescriptor )
			{
				f = m.position - p;
			}
			else if( usetargetposition == 1 )
			{
				if( g_pLastEventTargetModelDescriptor )
				{
					f.x = m.position.x - g_pLastEventTargetModelDescriptor->m_Position._41;
					f.y = m.position.y - g_pLastEventTargetModelDescriptor->m_Position._42;
					f.z = m.position.z - g_pLastEventTargetModelDescriptor->m_Position._43;

					// Backup
					pParticleList->m_UsedTargetMatrix = TRUE;
					pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41;
					pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42;
					pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43;
				}
				else
				{
					f = m.position - p;
				}
			}
			else // link biped node...
			{
				if( g_pLastEventTargetModelDescriptor )
				{
					D3DXVECTOR3 biped_p;
					biped_p.x = g_pLastEventTargetModelDescriptor->m_Position._41 + g_pLastEventTargetModelDescriptor->m_PartMatrixContainer[usetargetposition - 2]._41;
					biped_p.y = g_pLastEventTargetModelDescriptor->m_Position._42 + g_pLastEventTargetModelDescriptor->m_PartMatrixContainer[usetargetposition - 2]._42;
					biped_p.z = g_pLastEventTargetModelDescriptor->m_Position._43 + g_pLastEventTargetModelDescriptor->m_PartMatrixContainer[usetargetposition - 2]._43;

					int nNode = g_pLastEventTargetModelDescriptor->m_pBipedController->m_CharacterPartIndex[usetargetposition - 2];
					_LPXM_BIPEDOBJECT lpNode = g_pLastEventTargetModelDescriptor->m_pBipedController->GetNode(nNode);
					if (lpNode)
					{
						biped_p.x += lpNode->m_matTM._41;
						biped_p.y += lpNode->m_matTM._42;
						biped_p.z += lpNode->m_matTM._43;
					}
					f = m.position - biped_p;

					// Backup
					pParticleList->m_UsedTargetMatrix = TRUE;
					pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41;
					pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42;
					pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43;
				}
				else
				{
					f = m.position - p;
				}
			}
			
/*			if( usetargetposition )
			{
				f.x = m.position.x - g_pLastEventTargetModelDescriptor->m_Position._41;
				f.y = m.position.y - g_pLastEventTargetModelDescriptor->m_Position._42;
				f.z = m.position.z - g_pLastEventTargetModelDescriptor->m_Position._43;
			}
			else
			{
				f = m.position - p;
			}
*/
			
			D3DXVECTOR3 w( axis * (D3DXVec3Dot( &f, &axis ) ));
			
			// Direction from particle to nearest point on line.
			D3DXVECTOR3 into = w - f;
			
			// Distance to line (force drops as 1/r^2, normalize by 1/r)
			// Soften by epsilon to avoid tight encounters to infinity
			float rSqr = D3DXVec3LengthSq( &into );
			
			if(rSqr < max_radiusSqr)
				// Step velocity with acceleration
				m.velocity += into * (magdt / (_XFC_sqrt(rSqr) + (rSqr + epsilon)));
		}
	}
	else
	{
		// Removed because it causes pipeline stalls.
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Figure direction to particle from base of line.
			D3DXVECTOR3 f;
			if( usetargetposition == 0 || !g_pLastEventTargetModelDescriptor )
			{
				f = m.position - p;
			}
			else if( usetargetposition == 1 )
			{
				if( g_pLastEventTargetModelDescriptor )
				{
					f.x = m.position.x - g_pLastEventTargetModelDescriptor->m_Position._41;
					f.y = m.position.y - g_pLastEventTargetModelDescriptor->m_Position._42;
					f.z = m.position.z - g_pLastEventTargetModelDescriptor->m_Position._43;

					// Backup
					pParticleList->m_UsedTargetMatrix = TRUE;
					pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41;
					pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42;
					pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43;
				}
				else
				{
					f = m.position - p;
				}
			}
			else // link biped node...
			{
				if( g_pLastEventTargetModelDescriptor )
				{
					D3DXVECTOR3 biped_p;
					biped_p.x = g_pLastEventTargetModelDescriptor->m_Position._41 + g_pLastEventTargetModelDescriptor->m_PartMatrixContainer[usetargetposition - 2]._41;
					biped_p.y = g_pLastEventTargetModelDescriptor->m_Position._42 + g_pLastEventTargetModelDescriptor->m_PartMatrixContainer[usetargetposition - 2]._42;
					biped_p.z = g_pLastEventTargetModelDescriptor->m_Position._43 + g_pLastEventTargetModelDescriptor->m_PartMatrixContainer[usetargetposition - 2]._43;

					int nNode = g_pLastEventTargetModelDescriptor->m_pBipedController->m_CharacterPartIndex[usetargetposition - 2];
					_LPXM_BIPEDOBJECT lpNode = g_pLastEventTargetModelDescriptor->m_pBipedController->GetNode(nNode);
					if (lpNode)
					{
						biped_p.x += lpNode->m_matTM._41;
						biped_p.y += lpNode->m_matTM._42;
						biped_p.z += lpNode->m_matTM._43;
					}
					f = m.position - biped_p;

					// Backup
					pParticleList->m_UsedTargetMatrix = TRUE;
					pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41;
					pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42;
					pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43;
				}
				else
				{
					f = m.position - p;
				}
			}

/*			if( usetargetposition )
			{
				f.x = m.position.x - g_pLastEventTargetModelDescriptor->m_Position._41;
				f.y = m.position.y - g_pLastEventTargetModelDescriptor->m_Position._42;
				f.z = m.position.z - g_pLastEventTargetModelDescriptor->m_Position._43;
			}
			else
			{
				f = m.position - p;
			}
*/			
			D3DXVECTOR3 w(axis * (D3DXVec3Dot( &f, &axis) ));
			
			// Direction from particle to nearest point on line.
			D3DXVECTOR3 into = w - f;
			
			// Distance to line (force drops as 1/r^2, normalize by 1/r)
			// Soften by epsilon to avoid tight encounters to infinity
			float rSqr = D3DXVec3LengthSq( &into );
			
			// Step velocity with acceleration
			m.velocity += into * (magdt / (_XFC_sqrt(rSqr) + (rSqr + epsilon)));
		}
	}
}

BOOL _XParticleAction_OrbitPoint::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &center, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &magnitude, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &epsilon, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &max_radius, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &usetargetposition, sizeof(DWORD), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &directturnning, sizeof(bool), 1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

// Accelerate particles towards a point
void _XParticleAction_OrbitPoint::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float magdt = magnitude * dt;
	float max_radiusSqr = max_radius * max_radius;

	D3DXMATRIX characterinvmat;
	if( pParticleList->m_pParentModelDescriptor )
	{
		characterinvmat = pParticleList->m_pParentModelDescriptor->m_Position;
		characterinvmat._41 = characterinvmat._42 = characterinvmat._43 = 0.0f;
		D3DXMatrixInverse( &characterinvmat, NULL, &characterinvmat );
	}
	
	if(max_radiusSqr < _XP_MAXFLOAT)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));

			// Figure direction to particle.
			D3DXVECTOR3 dir;
			if( usetargetposition == 0 || !pParticleList->m_pParentModelDescriptor ||
				!g_pLastEventTargetModelDescriptor || !g_pLastEventTargetModelDescriptor->m_pBipedController )
			{
				if( pParticleList->m_UsedTargetMatrix )
				{
					if( pParticleList->m_pParentModelDescriptor )
					{
						D3DXVECTOR3 particleworldposition;
						D3DXVec3TransformCoord( &particleworldposition, &m.position, &pParticleList->m_pParentModelDescriptor->m_Position );
						
						dir.x = pParticleList->m_LastUsedTargetPosition.x - particleworldposition.x;
						dir.y = pParticleList->m_LastUsedTargetPosition.y - particleworldposition.y;
						dir.z = pParticleList->m_LastUsedTargetPosition.z - particleworldposition.z;
						
						D3DXVec3TransformCoord( &dir, &dir, &characterinvmat );
					}
					else
					{
						dir = center - m.position;
					}
				}
				else
				{
					dir = center - m.position;
				}
			}
			else if( usetargetposition == 1 )
			{
				if(pParticleList->m_pParentModelDescriptor && g_pLastEventTargetModelDescriptor )
				{
					D3DXVECTOR3 particleworldposition;
					D3DXVec3TransformCoord( &particleworldposition, &m.position, &pParticleList->m_pParentModelDescriptor->m_Position );
					
					dir.x = g_pLastEventTargetModelDescriptor->m_Position._41 - particleworldposition.x;
					dir.y = g_pLastEventTargetModelDescriptor->m_Position._42 - particleworldposition.y;
					dir.z = g_pLastEventTargetModelDescriptor->m_Position._43 - particleworldposition.z;
					
					D3DXVec3TransformCoord( &dir, &dir, &characterinvmat );

					pParticleList->m_UsedTargetMatrix = TRUE;
					pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41;
					pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42;
					pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43;
				}
				else
				{
					dir = center - m.position;
				}
			}
			else // link biped node...
			{
				if(pParticleList->m_pParentModelDescriptor && g_pLastEventTargetModelDescriptor )
				{
					D3DXVECTOR3 particleworldposition;
					D3DXVec3TransformCoord( &particleworldposition, &m.position, &pParticleList->m_pParentModelDescriptor->m_Position );

					D3DXVECTOR3 targetworldposition = D3DXVECTOR3(0.0f,0.0f,0.0f);
					_LPXM_BIPEDOBJECT lpNode = g_pLastEventTargetModelDescriptor->m_pBipedController->GetNode(
												g_pLastEventTargetModelDescriptor->m_pBipedController->m_CharacterPartIndex[usetargetposition - 2] );
					if (lpNode)
					{
						D3DXVec3TransformCoord( &targetworldposition, &targetworldposition, &lpNode->m_matTM );					
					}

					D3DXVec3TransformCoord( &targetworldposition, &targetworldposition, &g_pLastEventTargetModelDescriptor->m_Position );				

					//TCHAR message[256];
					//sprintf( message, "%f %f %f", g_pLastEventTargetModelDescriptor->m_Position._41, g_pLastEventTargetModelDescriptor->m_Position._42, g_pLastEventTargetModelDescriptor->m_Position._43 );
					//_XDebugOutputString( message, 1, 2 );

					D3DXVECTOR3 biped_p = targetworldposition - particleworldposition;
					D3DXVec3TransformCoord( &dir, &biped_p, &characterinvmat );
					
					pParticleList->m_UsedTargetMatrix = TRUE;
					pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41 + targetworldposition.x;
					pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42 + targetworldposition.y;
					pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43 + targetworldposition.z;
				}
				else
				{
					dir = center - m.position;
				}
			}

			// Distance to gravity well (force drops as 1/r^2, normalize by 1/r)
			// Soften by epsilon to avoid tight encounters to infinity
			float rSqr = D3DXVec3LengthSq( &dir );
			
			if(rSqr < max_radiusSqr)
			{
				// Step velocity with acceleration
				if( directturnning )
				{
					D3DXVec3Normalize( &dir, &dir );
					m.velocity = dir * magdt;

					D3DXVECTOR3 nextpos = m.position + (m.velocity * dt);


					D3DXVECTOR3 p2 = -dir;
					D3DXVec3Normalize( &p2, &p2 ); // Must normalize it.
					
					// radius1 stores the d of the plane eqn.
					FLOAT dofplane = -D3DXVec3Dot( &center, &p2 );					

					if( D3DXVec3Dot( &nextpos, &p2) >= -dofplane )
					{
						m.position = center;
						m.velocity.x = 0.0f;
						m.velocity.y = 0.0f;
						m.velocity.z = 0.0f;
					}
				}
				else
				{
					m.velocity += dir * (magdt / (_XFC_sqrt(rSqr) + (rSqr + epsilon)));
				}
			}
		}
	}
	else
	{
		// Avoids pipeline stalls.
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Figure direction to particle.			
			D3DXVECTOR3 dir;
			if( usetargetposition == 0 || !pParticleList->m_pParentModelDescriptor ||
				!g_pLastEventTargetModelDescriptor || !g_pLastEventTargetModelDescriptor->m_pBipedController )
			{
				if( pParticleList->m_UsedTargetMatrix )
				{
					D3DXVECTOR3 particleworldposition;
					D3DXVec3TransformCoord( &particleworldposition, &m.position, &pParticleList->m_pParentModelDescriptor->m_Position );
					
					dir.x = pParticleList->m_LastUsedTargetPosition.x - particleworldposition.x;
					dir.y = pParticleList->m_LastUsedTargetPosition.y - particleworldposition.y;
					dir.z = pParticleList->m_LastUsedTargetPosition.z - particleworldposition.z;
					
					D3DXVec3TransformCoord( &dir, &dir, &characterinvmat );
				}
				else
				{
					dir = center - m.position;
				}
			}
			else if( usetargetposition == 1 )
			{
				D3DXVECTOR3 particleworldposition;
				D3DXVec3TransformCoord( &particleworldposition, &m.position, &pParticleList->m_pParentModelDescriptor->m_Position );
				
				dir.x = g_pLastEventTargetModelDescriptor->m_Position._41 - particleworldposition.x;
				dir.y = g_pLastEventTargetModelDescriptor->m_Position._42 - particleworldposition.y;
				dir.z = g_pLastEventTargetModelDescriptor->m_Position._43 - particleworldposition.z;
				
				D3DXVec3TransformCoord( &dir, &dir, &characterinvmat );

				pParticleList->m_UsedTargetMatrix = TRUE;
				pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41;
				pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42;
				pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43;
			}
			else // link biped node...
			{
				D3DXVECTOR3 particleworldposition;
				D3DXVec3TransformCoord( &particleworldposition, &m.position, &pParticleList->m_pParentModelDescriptor->m_Position );
				
				D3DXVECTOR3 targetworldposition = D3DXVECTOR3(0.0f,0.0f,0.0f);
				_LPXM_BIPEDOBJECT lpNode = g_pLastEventTargetModelDescriptor->m_pBipedController->GetNode(
					g_pLastEventTargetModelDescriptor->m_pBipedController->m_CharacterPartIndex[usetargetposition - 2] );
				if (lpNode)
				{
					D3DXVec3TransformCoord( &targetworldposition, &targetworldposition, &lpNode->m_matTM );					
				}
				
				D3DXVec3TransformCoord( &targetworldposition, &targetworldposition, &g_pLastEventTargetModelDescriptor->m_Position );
				
				D3DXVECTOR3 biped_p = targetworldposition - particleworldposition;
				D3DXVec3TransformCoord( &dir, &biped_p, &characterinvmat );

				pParticleList->m_UsedTargetMatrix = TRUE;
				pParticleList->m_LastUsedTargetPosition.x = g_pLastEventTargetModelDescriptor->m_Position._41 + targetworldposition.x;
				pParticleList->m_LastUsedTargetPosition.y = g_pLastEventTargetModelDescriptor->m_Position._42 + targetworldposition.y;
				pParticleList->m_LastUsedTargetPosition.z = g_pLastEventTargetModelDescriptor->m_Position._43 + targetworldposition.z;
			}

			if( directturnning )
			{
				D3DXVec3Normalize( &dir, &dir );
				m.velocity = dir * magdt;

				/*
				if( pParticleList->m_FadeMode )
				{
					m.velocity /= 5.0f;
				}
				*/
			}
			else
			{
				// Distance to gravity well (force drops as 1/r^2, normalize by 1/r)
				// Soften by epsilon to avoid tight encounters to infinity
				float rSqr = D3DXVec3LengthSq( &dir );
				
				// Step velocity with acceleration
				m.velocity += dir * (magdt / (_XFC_sqrt(rSqr) + (rSqr + epsilon)));

				/*
				if( pParticleList->m_FadeMode )
				{
					m.velocity /= 5.0f;
				}
				*/
			}
		}
	}
}

BOOL _XParticleAction_RandomAccel::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( !gen_acc.Load( fileptr ) ) return FALSE;	
	return TRUE;
}

// Accelerate in random direction each time step
void _XParticleAction_RandomAccel::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));
		
		D3DXVECTOR3 acceleration;
		gen_acc.Generate(acceleration);
		
		// dt will affect this by making a higher probability of
		// being near the original velocity after unit time. Smaller
		// dt approach a normal distribution instead of a square wave.
		m.velocity += acceleration * dt;
	}
}

BOOL _XParticleAction_RandomDisplace::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( !gen_disp.Load( fileptr ) ) return FALSE;	
	return TRUE;
}

// Immediately displace position randomly
void _XParticleAction_RandomDisplace::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));
		
		D3DXVECTOR3 displacement;
		gen_disp.Generate(displacement);
		
		// dt will affect this by making a higher probability of
		// being near the original position after unit time. Smaller
		// dt approach a normal distribution instead of a square wave.
		m.position += displacement * dt;
	}
}

BOOL _XParticleAction_RandomVelocity::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( !gen_vel.Load( fileptr ) ) return FALSE;	
	return TRUE;
}

// Immediately assign a random velocity
void _XParticleAction_RandomVelocity::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));
		
		D3DXVECTOR3 velocity;
		gen_vel.Generate(velocity);
		
		// Shouldn't multiply by dt because velocities are
		// invariant of dt. How should dt affect this?
		m.velocity = velocity;
	}
}

#if 0
// Produce coefficients of a velocity function v(t)=at^2 + bt + c
// satisfying initial x(0)=x0,v(0)=v0 and desired x(t)=xf,v(t)=vf,
// where x = x(0) + integrate(v(T),0,t)
static inline void _pconstrain(float x0, float v0, float xf, float vf,
							   float t, float *a, float *b, float *c)
{
	*c = v0;
	*b = 2 * (-t*vf - 2*t*v0 + 3*xf - 3*x0) / (t * t);
	*a = 3 * (t*vf + t*v0 - 2*xf + 2*x0) / (t * t * t);
}
#endif

BOOL _XParticleAction_Restore::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &time_left, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

// Over time, restore particles to initial positions
// Put all particles on the surface of a statue, explode the statue,
// and then suck the particles back to the original position. Cool!
void _XParticleAction_Restore::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	if(time_left <= 0)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Already constrained, keep it there.
			m.position = m.positionB;
			m.velocity = D3DXVECTOR3(0,0,0);
		}
	}
	else
	{
		float t = time_left;
		float dtSqr = dt * dt;
		float tSqrInv2dt = dt * 2.0f / (t * t);
		float tCubInv3dtSqr = dtSqr * 3.0f / (t * t * t);

		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
#if 1
			_XParticleElement &m = (*(*it));
			
			// Solve for a desired-behavior velocity function in each axis
			// _pconstrain(m.position.x, m.velocity.x, m.posB.x, 0., timeLeft, &a, &b, &c);
			
			// Figure new velocity at next timestep
			// m.velocity.x = a * dtSqr + b * dt + c;
			
			float b = (-2*t*m.velocity.x + 3*m.positionB.x - 3*m.position.x) * tSqrInv2dt;
			float a = (t*m.velocity.x - m.positionB.x - m.positionB.x + m.position.x + m.position.x) * tCubInv3dtSqr;
			
			// Figure new velocity at next timestep
			m.velocity.x += a + b;
			
			b = (-2*t*m.velocity.y + 3*m.positionB.y - 3*m.position.y) * tSqrInv2dt;
			a = (t*m.velocity.y - m.positionB.y - m.positionB.y + m.position.y + m.position.y) * tCubInv3dtSqr;
			
			// Figure new velocity at next timestep
			m.velocity.y += a + b;
			
			b = (-2*t*m.velocity.z + 3*m.positionB.z - 3*m.position.z) * tSqrInv2dt;
			a = (t*m.velocity.z - m.positionB.z - m.positionB.z + m.position.z + m.position.z) * tCubInv3dtSqr;
			
			// Figure new velocity at next timestep
			m.velocity.z += a + b;
#else
			_XParticleElement &m = (*(*it));
			
			// XXX Optimize this.
			// Solve for a desired-behavior velocity function in each axis
			float a, b, c; // Coefficients of velocity function needed
			
			_pconstrain(m.position.x, m.velocity.x, m.positionB.x, 0.,
				time_left, &a, &b, &c);
			
			// Figure new velocity at next timestep
			m.velocity.x = a * dtSqr + b * dt + c;
			
			_pconstrain(m.position.y, m.velocity.y, m.positionB.y, 0.,
				time_left, &a, &b, &c);
			
			// Figure new velocity at next timestep
			m.velocity.y = a * dtSqr + b * dt + c;
			
			_pconstrain(m.position.z, m.velocity.z, m.positionB.z, 0.,
				time_left, &a, &b, &c);
			
			// Figure new velocity at next timestep
			m.velocity.z = a * dtSqr + b * dt + c;
			
#endif
		}
	}
	
	time_left -= dt;
}

BOOL _XParticleAction_Sink::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &kill_inside, sizeof(bool), 1, fileptr ) < 1 ) return FALSE;
	if( !position.Load(fileptr) ) return FALSE;	
	return TRUE;
}

// Kill particles with positions on wrong side of the specified domain
void _XParticleAction_Sink::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	// Must traverse list in reverse order so Remove will work
	sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin();
	while(it != pParticleList->m_slParticleElement.end())
	{
		_XParticleElement &m = (*(*it));
		if(!(position.Within(m.position) ^ kill_inside))
		{
			pParticleList->RemoveParticle( *it );
			it = pParticleList->m_slParticleElement.erase(it);
		}
		else
		{
			++ it;
		}
	}
}

BOOL _XParticleAction_SinkVelocity::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &kill_inside, sizeof(bool), 1, fileptr ) < 1 ) return FALSE;
	if( !velocity.Load(fileptr) ) return FALSE;		
	return TRUE;
}

// Kill particles with velocities on wrong side of the specified domain
void _XParticleAction_SinkVelocity::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	// Must traverse list in reverse order so Remove will work
	sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin();
	while(it != pParticleList->m_slParticleElement.end())
	{
		_XParticleElement &m = (*(*it));
		if(!(velocity.Within(m.velocity) ^ kill_inside))
		{
			pParticleList->RemoveParticle( *it );
			it = pParticleList->m_slParticleElement.erase(it);
		}
		else
		{
			++ it;
		}
	}
}

BOOL _XParticleAction_Source::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( !position.Load(fileptr) ) return FALSE;
	if( !positionB.Load(fileptr) ) return FALSE;
	if( !size.Load(fileptr) ) return FALSE;
	if( !velocity.Load(fileptr) ) return FALSE;
	if( !color.Load(fileptr) ) return FALSE;
	if( fread( &alpha, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;

	if( alpha > 1.0f ) alpha = 1.0f;

	if( fread( &particle_rate, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &age, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &age_sigma, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &vertexB_tracks, sizeof(bool), 1, fileptr ) < 1 ) return FALSE;			
	
	// Temporary force setting....
	vertexB_tracks = FALSE;

	if( fread( &userotation, sizeof(BOOL), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &emittersplinecontainerindex, sizeof(int), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &emitterpathmovespeed, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &rewind, sizeof(BOOL), 1, fileptr ) < 1 ) return FALSE;

	if( fread( &timebaseparticlerate, sizeof(BOOL), 1, fileptr ) < 1 ) return FALSE;

	return TRUE;
}

// Randomly add particles to the system
void _XParticleAction_Source::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	if( pParticleList->m_pInstanceData->RenderingType == _XParticleRT_LineEffect )
	{
		if( pParticleList->m_pInstanceData->EmitterBipedLinkType >= _XParticleBLT_CharacterRightHand )
		{
			if( pParticleList->m_pInstanceData->LinkSubTarget & _XEMITTERSUBLINKTARGET_WEAPONTWOPOINT )
			{
				pParticleList->m_pInstanceData->m_LastEmitPosition[0] = pParticleList->m_LinkPosition[0];
				pParticleList->m_pInstanceData->m_LastEmitPosition[1] = pParticleList->m_LinkPosition[1];
			}
			else
			{
				pParticleList->m_pInstanceData->m_LastEmitPosition[0] = position.p1;
				pParticleList->m_pInstanceData->m_LastEmitPosition[1] = position.p2;
			}
		}
	}
	
	int rate = 0;
	if( timebaseparticlerate )			
	{
		FLOAT elapsedtime = dt;
		if( dt > 0.1f ) // 진행시간 제한 (30Fps기준 10 frame이상 진행되었으면 10 frame으로 제한)
		{
			elapsedtime = 0.1f;
		}
		rate = int(floor(particle_rate * elapsedtime));
		
		// Dither the fraction particle in time.
		if(drand48() < particle_rate * elapsedtime - float(rate)) rate++;

#if defined(_XDEF_MODELTHREADLOADING) || defined(_XDEF_VEQUALITYCONTROL) 

		if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
		{
			rate = (int)((FLOAT)rate * g_VisualEffectQuality);
		}
		else
		{
			if( rate >= 2 )
				rate = (int)((FLOAT)rate * g_VisualEffectQuality);
		}
#endif
	}
	else
	{
		//rate = (int)particle_rate;
		pParticleList->FrameEmitAccumulater += particle_rate;
		
		if( pParticleList->FrameEmitAccumulater > 1.0f )
		{
			rate = (int)pParticleList->FrameEmitAccumulater;
	
#if defined(_XDEF_MODELTHREADLOADING) || defined(_XDEF_VEQUALITYCONTROL) 
			if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
			{
				rate = (int)((FLOAT)rate * g_VisualEffectQuality);
			}
			else
			{
				if( rate >= 2 )
					rate = (int)((FLOAT)rate * g_VisualEffectQuality);
			}
#endif

			pParticleList->FrameEmitAccumulater -= rate;
		}
		else
		{
			if( emittersplinecontainerindex > -1 )
			{
				_XSplineContainer* pSplineContainer = &pParticleList->m_pInstanceData->m_pEmitterPathList[emittersplinecontainerindex]; // Warn... : check array boundary..
				
				if( pSplineContainer )
				{
					if( rewind )
					{
						if( pParticleList->EmitPathDirectionFrag )
						{
							/*
							if( timebaseparticlerate )
							{							
								pParticleList->Accumulatesplinepointindex += g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							}
							else
							{
								pParticleList->Accumulatesplinepointindex+=1.0f;
							}
							*/

							pParticleList->Accumulatesplinepointindex += g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							
							if( pParticleList->Accumulatesplinepointindex > pSplineContainer->m_TotalCurveCount )
							{
								FLOAT maxframe = (FLOAT)(pSplineContainer->m_TotalCurveCount-1);
								pParticleList->Accumulatesplinepointindex = maxframe - (pParticleList->Accumulatesplinepointindex - maxframe);
								pParticleList->EmitPathDirectionFrag = FALSE;
							}
						}
						else 
						{
							/*
							if( timebaseparticlerate )
							{							
								pParticleList->Accumulatesplinepointindex -= g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							}
							else
							{
								pParticleList->Accumulatesplinepointindex-=1.0f;
							}
							*/
							pParticleList->Accumulatesplinepointindex -= g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							
							if( pParticleList->Accumulatesplinepointindex < 0.0f )
							{
								pParticleList->Accumulatesplinepointindex = -pParticleList->Accumulatesplinepointindex;
								pParticleList->EmitPathDirectionFrag = TRUE;
							}
						}
					}
					else
					{
						if( timebaseparticlerate )
						{
							pParticleList->Accumulatesplinepointindex += g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							pParticleList->Accumulatesplinepointindex = fmod( pParticleList->Accumulatesplinepointindex, (FLOAT)pSplineContainer->m_TotalCurveCount );
						}
						else
						{
							pParticleList->Accumulatesplinepointindex += 1.0f;
						}
					}
					
					int splinepointindex = (UINT)pParticleList->Accumulatesplinepointindex;
					
					if( splinepointindex < 0 || splinepointindex >= pSplineContainer->m_TotalCurveCount )
					{
						splinepointindex = 0;
						pParticleList->Accumulatesplinepointindex = 0.0f;
					}
				}
			}

			return;
		}
	}		
	
	// Don't emit more than it can hold.
//	if( pParticleList->m_slParticleElement.size() + rate > pParticleList->particlelist->GetTotalElements() )
//		rate = pParticleList->particlelist->GetNumFreeElements();
	if( pParticleList->m_slParticleElement.size() + rate > pParticleList->m_pInstanceData->maxparticles )
		rate = pParticleList->m_pInstanceData->maxparticles - pParticleList->m_slParticleElement.size();

	D3DXVECTOR3 pos, posB, vel, col, siz;
	_XParticleElement particledata;
	D3DXVECTOR3 tempcolor;
	
	if(vertexB_tracks)
	{
		//BOOL applyfirstpos = FALSE;

		for(int i = 0; i < rate; i++)
		{
			position.Generate( particledata.position );
			size.Generate(particledata.size);
			velocity.Generate(particledata.velocity);

			/*
			if( !applyfirstpos )
			{
				applyfirstpos = TRUE;
				pParticleList->m_EmitPosition = particledata.position;
			}
			*/
						
			color.Generate(tempcolor);
			
			if( tempcolor.x > 1.0f ) tempcolor.x = 1.0f;
			if( tempcolor.y > 1.0f ) tempcolor.y = 1.0f;
			if( tempcolor.z > 1.0f ) tempcolor.z = 1.0f;
			
			particledata.color.r = tempcolor.x;
			particledata.color.g = tempcolor.y;
			particledata.color.b = tempcolor.z;
			particledata.color.a = alpha;

			particledata.age = age + NRand(age_sigma);

			if( pParticleList->m_pInstanceData->ViewtTargetType >= _XParticleVT_CameraAndRollAnimation  &&
				pParticleList->m_pInstanceData->ViewtTargetType <= _XParticleVT_FixedAndZRollAnimation )
			{
				particledata.randomdirection = NRand( 6.28318530717958647692528676624f ); // 360'
			}

			if( pParticleList->m_pInstanceData->LinkSubTarget & _XEMITTERSUBLINKTARGET_OBB )
			{
				particledata.position.y += pParticleList->m_AdditionalHeight;	
			}

			if( pParticleList->m_pInstanceData->EmitterBipedLinkType >= _XParticleBLT_CharacterRightHand && pParticleList->m_pParentModelDescriptor )
			{
				int bipedindex = pParticleList->m_pInstanceData->EmitterBipedLinkType-_XParticleBLT_CharacterRightHand;
				D3DXVec3TransformCoord( &particledata.position, &particledata.position, &pParticleList->m_pParentModelDescriptor->m_PartMatrixContainer[bipedindex]);

				if( pParticleList->m_pInstanceData->LinkSubTarget & _XEMITTERSUBLINKTARGET_VELOCITYVOLUME )
				{
					D3DXVec3TransformCoord( &particledata.velocity, &particledata.velocity, &pParticleList->m_pParentModelDescriptor->m_PartMatrixContainer[bipedindex]);
				}				
			}			
			
			if( pParticleList->m_LimitMaxParticleSize > 1.0f )
			{
				if( particledata.size.x > pParticleList->m_LimitMaxParticleSize )
				{
					particledata.size.x = pParticleList->m_LimitMaxParticleSize;
				}
				if( particledata.size.y > pParticleList->m_LimitMaxParticleSize )
				{
					particledata.size.y = pParticleList->m_LimitMaxParticleSize;
				}
				if( particledata.size.z > pParticleList->m_LimitMaxParticleSize )
				{
					particledata.size.z = pParticleList->m_LimitMaxParticleSize;
				}
			}

			pParticleList->AddParticle( particledata );
		}
	}
	else
	{
		for(int i = 0; i < rate; i++)
		{
			if( pParticleList->m_pInstanceData->LinkSubTarget & _XEMITTERSUBLINKTARGET_WEAPONTWOPOINT )
			{
				if( position.type == _XPE_Point )
				{
					particledata.position = pParticleList->m_LinkPosition[0];
				}
				else if( position.type == _XPE_Line )
				{
					D3DXVECTOR3 p2 = pParticleList->m_LinkPosition[1] - pParticleList->m_LinkPosition[0];
					particledata.position = pParticleList->m_LinkPosition[0] + p2 * drand48();
				}
				else
				{
					position.Generate(particledata.position);
				}
			}
			/*else if( pParticleList->LinkSubTarget & _XEMITTERSUBLINKTARGET_CLOTHONEPOINT )
			{
			
			  
			}*/
			else
			{
				position.Generate(particledata.position);
			}
			

			positionB.Generate(particledata.positionB);
			size.Generate(particledata.size);
			velocity.Generate(particledata.velocity);
			
			// Move emitting point from the path..
			if( emittersplinecontainerindex > -1 )
			{
				_XSplineContainer* pSplineContainer = &pParticleList->m_pInstanceData->m_pEmitterPathList[emittersplinecontainerindex]; // Warn... : check array boundary..
					
				if( pSplineContainer )
				{
					if( rewind )
					{
						if( pParticleList->EmitPathDirectionFrag )
						{
							/*
							if( timebaseparticlerate )
							{							
								pParticleList->Accumulatesplinepointindex += g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							}
							else
							{
								pParticleList->Accumulatesplinepointindex+=1.0f;
							}
							*/
							pParticleList->Accumulatesplinepointindex += g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							
							if( pParticleList->Accumulatesplinepointindex > pSplineContainer->m_TotalCurveCount )
							{
								FLOAT maxframe = (FLOAT)(pSplineContainer->m_TotalCurveCount-1);
								pParticleList->Accumulatesplinepointindex = maxframe - (pParticleList->Accumulatesplinepointindex - maxframe);
								pParticleList->EmitPathDirectionFrag = FALSE;
							}
						}
						else 
						{
							/*
							if( timebaseparticlerate )
							{							
								pParticleList->Accumulatesplinepointindex -= g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							}
							else
							{
								pParticleList->Accumulatesplinepointindex-=1.0f;
							}
							*/
							pParticleList->Accumulatesplinepointindex -= g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							
							if( pParticleList->Accumulatesplinepointindex < 0.0f )
							{
								pParticleList->Accumulatesplinepointindex = -pParticleList->Accumulatesplinepointindex;
								pParticleList->EmitPathDirectionFrag = TRUE;
							}
						}
					}
					else
					{
						/*
						if( timebaseparticlerate )
						{
							pParticleList->Accumulatesplinepointindex += g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
							pParticleList->Accumulatesplinepointindex = fmod( pParticleList->Accumulatesplinepointindex, (FLOAT)pSplineContainer->m_TotalCurveCount );
						}
						else
						{
							pParticleList->Accumulatesplinepointindex += 1.0f;
						}
						*/
						pParticleList->Accumulatesplinepointindex += g_fElapsedFrameMilisecondTime * emitterpathmovespeed;
						pParticleList->Accumulatesplinepointindex = fmod( pParticleList->Accumulatesplinepointindex, (FLOAT)pSplineContainer->m_TotalCurveCount );
					}

					int splinepointindex = (UINT)pParticleList->Accumulatesplinepointindex;
					
					if( splinepointindex < 0 || splinepointindex >= pSplineContainer->m_TotalCurveCount )
					{
						splinepointindex = 0;
						pParticleList->Accumulatesplinepointindex = 0.0f;
					}

					if( pSplineContainer->m_pCurveData )
					{
						if( userotation )
						{
							D3DXMATRIX  rotmatrix;						
							D3DXVECTOR3	upvector = D3DXVECTOR3(0.0f,1.0f,0.0f);
							D3DXQUATERNION normalrotquat = _XRotationArcNormalized( &upvector, &pSplineContainer->m_pCurveData[splinepointindex].nrm );
							D3DXMatrixRotationQuaternion( &rotmatrix, &normalrotquat );						
							D3DXVECTOR4 ret;
							upvector = D3DXVECTOR3(0.0f,0.0f,0.0f);
							D3DXVec3Transform( &ret, &particledata.position, &rotmatrix);
							particledata.position.x = ret.x + pSplineContainer->m_pCurveData[splinepointindex].pos.x;
							particledata.position.y = ret.y + pSplineContainer->m_pCurveData[splinepointindex].pos.y;
							particledata.position.z = ret.z + pSplineContainer->m_pCurveData[splinepointindex].pos.z;
						}
						else
						{
							particledata.position += pSplineContainer->m_pCurveData[splinepointindex].pos;
						}
					}
				}
			}

			color.Generate(tempcolor);

			if( tempcolor.x > 1.0f ) tempcolor.x = 1.0f;
			if( tempcolor.y > 1.0f ) tempcolor.y = 1.0f;
			if( tempcolor.z > 1.0f ) tempcolor.z = 1.0f;

			particledata.color.r = tempcolor.x;
			particledata.color.g = tempcolor.y;
			particledata.color.b = tempcolor.z;
			particledata.color.a = alpha;

			particledata.age = age + NRand(age_sigma);
			
			if( pParticleList->m_pInstanceData->ViewtTargetType >= _XParticleVT_CameraAndRollAnimation  &&
				pParticleList->m_pInstanceData->ViewtTargetType <= _XParticleVT_FixedAndZRollAnimation )
			{
				particledata.randomdirection = NRand( 6.28318530717958647692528676624f ); // 360'
			}

			if( pParticleList->m_pInstanceData->LinkSubTarget & _XEMITTERSUBLINKTARGET_OBB )
			{
				particledata.position.y += pParticleList->m_AdditionalHeight;	
			}

			if( pParticleList->m_pInstanceData->EmitterBipedLinkType >= _XParticleBLT_CharacterRightHand && pParticleList->m_pParentModelDescriptor )
			{
				int bipedindex = pParticleList->m_pInstanceData->EmitterBipedLinkType-_XParticleBLT_CharacterRightHand;
				D3DXVec3TransformCoord( &particledata.position, &particledata.position, &pParticleList->m_pParentModelDescriptor->m_PartMatrixContainer[bipedindex]);

				if( pParticleList->m_pInstanceData->LinkSubTarget & _XEMITTERSUBLINKTARGET_VELOCITYVOLUME )
				{
					D3DXVec3TransformCoord( &particledata.velocity, &particledata.velocity, &pParticleList->m_pParentModelDescriptor->m_PartMatrixContainer[bipedindex]);
				}
			}

			if( pParticleList->m_LimitMaxParticleSize > 1.0f )
			{
				if( particledata.size.x > pParticleList->m_LimitMaxParticleSize )
				{
					particledata.size.x = pParticleList->m_LimitMaxParticleSize;
				}
				if( particledata.size.y > pParticleList->m_LimitMaxParticleSize )
				{
					particledata.size.y = pParticleList->m_LimitMaxParticleSize;
				}
				if( particledata.size.z > pParticleList->m_LimitMaxParticleSize )
				{
					particledata.size.z = pParticleList->m_LimitMaxParticleSize;
				}
			}
			
			pParticleList->AddParticle( particledata );
		}
	}
}



BOOL _XParticleAction_SpeedLimit::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &min_speed, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	if( fread( &max_speed, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;	
	return TRUE;
}

void _XParticleAction_SpeedLimit::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float min_sqr = min_speed*min_speed;
	float max_sqr = max_speed*max_speed;
	
	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));

		float sSqr = D3DXVec3LengthSq( &m.velocity );
		if(sSqr<min_sqr && sSqr)
		{
			float s = _XFC_sqrt(sSqr);
			m.velocity *= (min_speed/s);
		}
		else if(sSqr>max_sqr)
		{
			float s = _XFC_sqrt(sSqr);
			m.velocity *= (max_speed/s);
		}
	}
}

BOOL _XParticleAction_TargetColor::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &color, sizeof(D3DXCOLOR), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &scale, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

// Change color of all particles toward the specified color
void _XParticleAction_TargetColor::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float scaleFac = scale * dt;
	
	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));

		m.color += (color - m.color) * scaleFac;

		if( m.color.r > 1.0f ) m.color.r = 1.0f;
		if( m.color.g > 1.0f ) m.color.g = 1.0f;
		if( m.color.b > 1.0f ) m.color.b = 1.0f;
		if( m.color.a > 1.0f ) m.color.a = 1.0f;
	}
}


BOOL _XParticleAction_TargetSize::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &size, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &scale, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;	
	return TRUE;
}

// Change sizes of all particles toward the specified size
void _XParticleAction_TargetSize::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float scaleFac_x = scale.x * dt;
	float scaleFac_y = scale.y * dt;
	float scaleFac_z = scale.z * dt;
	
	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));

		D3DXVECTOR3 dif(size - m.size);
		dif.x *= scaleFac_x;
		dif.y *= scaleFac_y;
		dif.z *= scaleFac_z;

		m.size += dif;

		//if( m.size.x > 2.0f ) m.size.x = 2.0f;
		//if( m.size.y > 2.0f ) m.size.y = 2.0f;
		//if( m.size.z > 2.0f ) m.size.z = 2.0f;
		if( pParticleList->m_LimitMaxParticleSize > 1.0f )
		{
			if( m.size.x > pParticleList->m_LimitMaxParticleSize )
			{
				m.size.x = pParticleList->m_LimitMaxParticleSize;
			}
			if( m.size.y > pParticleList->m_LimitMaxParticleSize )
			{
				m.size.y = pParticleList->m_LimitMaxParticleSize;
			}
			if( m.size.z > pParticleList->m_LimitMaxParticleSize )
			{
				m.size.z = pParticleList->m_LimitMaxParticleSize;
			}
		}
	}
}

BOOL _XParticleAction_TargetVelocity::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &velocity, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &scale, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;			
	return TRUE;
}

// Change velocity of all particles toward the specified velocity
void _XParticleAction_TargetVelocity::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float scaleFac = scale * dt;
	
	for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
	{						
		_XParticleElement &m = (*(*it));

		m.velocity += (velocity - m.velocity) * scaleFac;
	}
}


// If in immediate mode, quickly add a vertex.
// If building an action list, call pSource.
void _XParticleAction_Vertex::Execute( _XParticleList* pParticleList )
{
	if( !CheckActionTime() ) return;

	/*
	_ParticleState &_ps = _GetPState();
	
	if(_ps.in_new_list)
	{
		pSource(1, PDPoint, x, y, z);
		return;
	}
	
	// Immediate mode. Quickly add the vertex.
	if(_ps.pgrp == NULL)
		return;
	
	pVector pos(x, y, z);
	pVector siz, vel, col, posB;
	if(_ps.vertexB_tracks)
		posB = pos;
	else
		_ps.VertexB.Generate(posB);
	_ps.Size.Generate(siz);
	_ps.Vel.Generate(vel);
	_ps.Color.Generate(col);
	_ps.pgrp->Add(pos, posB, siz, vel, col, _ps.Alpha, _ps.Age);
	*/
}


BOOL _XParticleAction_Vortex::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &center, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &axis, sizeof(D3DXVECTOR3), 1, fileptr ) < 1 ) return FALSE;	
	if( fread( &magnitude, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;			
	if( fread( &epsilon, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;			
	if( fread( &max_radius, sizeof(FLOAT), 1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

// Immediately displace position using vortex
// Vortex tip at center, around axis, with magnitude
// and tightness exponent
void _XParticleAction_Vortex::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;

	float magdt = magnitude * dt;
	float max_radiusSqr = max_radius * max_radius;
	
	if(max_radiusSqr < _XP_MAXFLOAT)
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Vector from tip of vortex
			D3DXVECTOR3 offset(m.position - center);
			
			// Compute distance from particle to tip of vortex.
			float rSqr = D3DXVec3LengthSq( &offset );
			
			// Don't do anything to particle if too close or too far.
			if(rSqr > max_radiusSqr)
				continue;
			
			float r = _XFC_sqrt(rSqr);
			
			// Compute normalized offset vector.
			D3DXVECTOR3 offnorm(offset / r);
			
			// Construct orthogonal vector frame in which to rotate
			// transformed point around origin
			float axisProj = D3DXVec3Dot( &offnorm, &axis ); // offnorm . axis
			
			// Components of offset perpendicular and parallel to axis
			D3DXVECTOR3 w(axis * axisProj); // parallel component
			D3DXVECTOR3 u(offnorm - w); // perpendicular component
			
			// Perpendicular component completing frame:
			D3DXVECTOR3 v;
			D3DXVec3Cross( &v, &axis, &u );
			
			// Figure amount of rotation
			// Resultant is (cos theta) u + (sin theta) v
			float theta = magdt / (rSqr + epsilon);
			float s = sinf(theta);
			float c = cosf(theta);
			
			offset = (u * c + v * s + w) * r;
			
			// Translate back to object space
			m.position = offset + center;
		}
	}
	else
	{
		for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
		{						
			_XParticleElement &m = (*(*it));
			
			// Vector from tip of vortex
			D3DXVECTOR3 offset(m.position - center);
			
			// Compute distance from particle to tip of vortex.
			float rSqr = D3DXVec3LengthSq( &offset );
			
			float r = _XFC_sqrt(rSqr);
			
			// Compute normalized offset vector.
			D3DXVECTOR3 offnorm(offset / r);
			
			// Construct orthogonal vector frame in which to rotate
			// transformed point around origin
			float axisProj = D3DXVec3Dot( &offnorm, &axis ); // offnorm . axis
			
			// Components of offset perpendicular and parallel to axis
			D3DXVECTOR3 w(axis * axisProj); // parallel component
			D3DXVECTOR3 u(offnorm - w); // perpendicular component
			
			// Perpendicular component completing frame:
			D3DXVECTOR3 v;
			D3DXVec3Cross( &v, &axis, &u );
						
			// Figure amount of rotation
			// Resultant is (cos theta) u + (sin theta) v
			float theta = magdt / (rSqr + epsilon);
			float s = sinf(theta);
			float c = cosf(theta);
			
			offset = (u * c + v * s + w) * r;
			
			// Translate back to object space
			m.position = offset + center;
		}
	}
}

BOOL _XParticleAction_StopVolume::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( !position.Load( fileptr ) ) return FALSE;			
	return TRUE;
}

void _XParticleAction_StopVolume::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() ) return;
		
	switch(position.type)
	{
	case _XPE_Triangle:
		{
			// Compute the inverse matrix of the plane basis.
			D3DXVECTOR3 &u = position.u;
			D3DXVECTOR3 &v = position.v;
			
			// w = u cross v
			float wx = u.y*v.z-u.z*v.y;
			float wy = u.z*v.x-u.x*v.z;
			float wz = u.x*v.y-u.y*v.x;
			
			float det = 1/(wz*u.x*v.y-wz*u.y*v.x-u.z*wx*v.y-u.x*v.z*wy+v.z*wx*u.y+u.z*v.x*wy);
			
			D3DXVECTOR3 s1((v.y*wz-v.z*wy), (v.z*wx-v.x*wz), (v.x*wy-v.y*wx));
			s1 *= det;
			D3DXVECTOR3 s2((u.y*wz-u.z*wy), (u.z*wx-u.x*wz), (u.x*wy-u.y*wx));
			s2 *= -det;
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1;
				
				// Opposite signs if product < 0
				// Is there a faster way to do this?
				if(distold * distnew >= 0)
					continue;
				
				// Find position at the crossing point by parameterizing
				// p(t) = pos + vel * t
				// Solve dist(p(t),plane) = 0 e.g.
				// n * p(t) + D = 0 ->
				// n * p + t (n * v) + D = 0 ->
				// t = -(n * p + D) / (n * v)
				// Could factor n*v into distnew = distold + n*v and save a bit.
				// Safe since n*v != 0 assured by quick rejection test.
				// This calc is indep. of dt because we have established that it
				// will hit before dt. We just want to know when.
				float nv = D3DXVec3Dot( &position.p2, &m.velocity );
				float t = -distold / nv;
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, p - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				// Dot product with basis vectors of old frame
				// in terms of new frame gives position in uv frame.
				float upos = D3DXVec3Dot( &offset, &s1 );
				float vpos = D3DXVec3Dot( &offset, &s2 );
				
				// Did it cross plane outside triangle?
				if(upos < 0 || vpos < 0 || (upos + vpos) > 1)
					continue;

				m.velocity /= 1000.0f;
				//m.velocity.x = 0.0f;
				//m.velocity.y = 0.0f;
				//m.velocity.z = 0.0f;
			}
		}
		break;
	case _XPE_Disc:
		{
			float r1Sqr = fsqr(position.radius1);
			float r2Sqr = fsqr(position.radius2);
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d. radius1Sqr stores d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1Sqr;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1Sqr;
				
				// Opposite signs if product < 0
				// Is there a faster way to do this?
				if(distold * distnew >= 0)
					continue;
				
				// Find position at the crossing point by parameterizing
				// p(t) = pos + vel * t
				// Solve dist(p(t),plane) = 0 e.g.
				// n * p(t) + D = 0 ->
				// n * p + t (n * v) + D = 0 ->
				// t = -(n * p + D) / (n * v)
				// Could factor n*v into distnew = distold + n*v and save a bit.
				// Safe since n*v != 0 assured by quick rejection test.
				// This calc is indep. of dt because we have established that it
				// will hit before dt. We just want to know when.
				float nv = D3DXVec3Dot( &position.p2, &m.velocity );
				float t = -distold / nv;
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, phit - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				float rad = D3DXVec3LengthSq( &offset );
				
				if(rad > r1Sqr || rad < r2Sqr)
					continue;
				
				m.velocity /= 1000.0f;
				//m.velocity.x = 0.0f;
				//m.velocity.y = 0.0f;
				//m.velocity.z = 0.0f;
			}
		}
		break;
	case _XPE_Plane:
		{
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1;
				
				// Opposite signs if product < 0
				if(distold * distnew >= 0)
					continue;
				
				m.velocity /= 1000.0f;
				//m.velocity.x = 0.0f;
				//m.velocity.y = 0.0f;
				//m.velocity.z = 0.0f;
			}
		}
		break;
	case _XPE_Rectangle:
		{
			// Compute the inverse matrix of the plane basis.
			D3DXVECTOR3 &u = position.u;
			D3DXVECTOR3 &v = position.v;
			
			// w = u cross v
			float wx = u.y*v.z-u.z*v.y;
			float wy = u.z*v.x-u.x*v.z;
			float wz = u.x*v.y-u.y*v.x;
			
			float det = 1/(wz*u.x*v.y-wz*u.y*v.x-u.z*wx*v.y-u.x*v.z*wy+v.z*wx*u.y+u.z*v.x*wy);
			
			D3DXVECTOR3 s1((v.y*wz-v.z*wy), (v.z*wx-v.x*wz), (v.x*wy-v.y*wx));
			s1 *= det;
			D3DXVECTOR3 s2((u.y*wz-u.z*wy), (u.z*wx-u.x*wz), (u.x*wy-u.y*wx));
			s2 *= -det;
			
			// See which particles bounce.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's current and next positions cross plane.
				// If not, couldn't bounce, so keep going.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				// p2 stores the plane normal (the a,b,c of the plane eqn).
				// Old and new distances: dist(p,plane) = n * p + d
				// radius1 stores -n*p, which is d.
				float distold = D3DXVec3Dot( &m.position, &position.p2 ) + position.radius1;
				float distnew = D3DXVec3Dot( &pnext, &position.p2 ) + position.radius1;
				
				// Opposite signs if product < 0
				if(distold * distnew >= 0)
					continue;
				
				// Find position at the crossing point by parameterizing
				// p(t) = pos + vel * t
				// Solve dist(p(t),plane) = 0 e.g.
				// n * p(t) + D = 0 ->
				// n * p + t (n * v) + D = 0 ->
				// t = -(n * p + D) / (n * v)
				float t = -distold / ( D3DXVec3Dot( &position.p2, &m.velocity ) );
				
				// Actual intersection point p(t) = pos + vel t
				D3DXVECTOR3 phit(m.position + m.velocity * t);
				
				// Offset from origin in plane, p - origin
				D3DXVECTOR3 offset(phit - position.p1);
				
				// Dot product with basis vectors of old frame
				// in terms of new frame gives position in uv frame.
				float upos = D3DXVec3Dot( &offset, &s1 );
				float vpos = D3DXVec3Dot( &offset, &s2 );
				
				// Crossed plane outside bounce region if !(0<=[uv]pos<=1)
				if(upos < 0 || upos > 1 || vpos < 0 || vpos > 1)
					continue;
				
				m.velocity /= 1000.0f;
				//m.velocity.x = 0.0f;
				//m.velocity.y = 0.0f;
				//m.velocity.z = 0.0f;
			}
		}
		break;
	case _XPE_Sphere:
		{
			// Sphere that particles bounce off
			// The particles are always forced out of the sphere.
			for (sldef_XParticleElement::iterator it = pParticleList->m_slParticleElement.begin(); it != pParticleList->m_slParticleElement.end(); ++ it)
			{						
				_XParticleElement &m = (*(*it));
				
				// See if particle's next position is inside domain.
				// If so, bounce it.
				D3DXVECTOR3 pnext(m.position + m.velocity * dt);
				
				if(position.Within(pnext))
				{
					// See if we were inside on previous timestep.
					bool pinside = position.Within(m.position);
					
					// Normal to surface. This works for a sphere. Isn't
					// computed quite right, should extrapolate particle
					// position to surface.
					D3DXVECTOR3 n(m.position - position.p1);
					D3DXVec3Normalize( &n, &n );
					
					// Compute tangential and normal components of velocity
					float nmag = D3DXVec3Dot( &m.velocity, &n );
					
					D3DXVECTOR3 vn(n * nmag); // Normal Vn = (V.N)N
					D3DXVECTOR3 vt = m.velocity - vn; // Tangent Vt = V - Vn
					
					if(pinside)
					{						
						if(nmag < 0)
						{
							m.velocity /= 1000.0f;
							//m.velocity.x = 0.0f;
							//m.velocity.y = 0.0f;
							//m.velocity.z = 0.0f;
						}
					}
				}
			}
		}
	}
}

BOOL _XParticleAction_EffectSound::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;

	int nSoundIndex;
	if( fread( &nSoundIndex, sizeof(int), 1, fileptr ) < 1 ) return FALSE;

	if (nSoundIndex >= 0)
	{
		int length;
		char strName[MAX_PATH];
		memset(strName, 0, MAX_PATH);
		fread( &length, sizeof(int), 1, fileptr );
		fread( strName, sizeof(char), length, fileptr );
#ifdef _XUSEFMOD
		m_nSoundIndex = g_FMODWrapper.InsertSound( strName, FSOUND_HW3D );
#else	
		m_nSoundIndex = g_SoundEffectArchive.FindResource( strName );
#endif
	}

	return TRUE;
}

void _XParticleAction_EffectSound::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() )
	{
		if (pParticleList->m_bSoundReady == TRUE)
		{
			pParticleList->m_bSoundReady = FALSE;
		}
		return;
	}

	if (m_nSoundIndex >= 0 && pParticleList->m_bSoundReady == FALSE)
	{
#ifdef _XUSEFMOD
		D3DXVECTOR3 vPosition;
		vPosition.x = pParticleList->m_ParentMatrix._41;
		vPosition.y = pParticleList->m_ParentMatrix._42;
		vPosition.z = pParticleList->m_ParentMatrix._43;
		
		_XPlaySoundEffect(m_nSoundIndex, vPosition, false, false);		
#else	
		if (!g_pSoundEffect[m_nSoundIndex]->IsPlaying())
		{
			D3DXVECTOR3 vPosition;
			vPosition.x = pParticleList->m_ParentMatrix._41;
			vPosition.y = pParticleList->m_ParentMatrix._42;
			vPosition.z = pParticleList->m_ParentMatrix._43;

			_XPlaySoundEffect(m_nSoundIndex, vPosition, false, false);
		}
#endif
		pParticleList->m_bSoundReady = TRUE;
	}
}

BOOL _XParticleAction_EffectLight::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &m_Light, sizeof(D3DLIGHT9), 1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

void _XParticleAction_EffectLight::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() )
	{
		if (pParticleList->m_bLightReady == TRUE)
		{
			pParticleList->m_bLightReady = FALSE;
		}
		return;
	}

	if (pParticleList->m_bLightReady == FALSE)
	{
		D3DXVECTOR3 vPosition;
		vPosition.x = pParticleList->m_ParentMatrix._41;
		vPosition.y = pParticleList->m_ParentMatrix._42;
		vPosition.z = pParticleList->m_ParentMatrix._43;

		FLOAT fLifeTime = 0.f;

		timetracklist.resetList();
		_XTimeSlice* pTimeSlice;
		pTimeSlice = (_XTimeSlice*)timetracklist.currentItem();
		if( pTimeSlice )
		{
			fLifeTime = pTimeSlice->endtime - pTimeSlice->starttime;
		}		

		g_ParticleEffectManager.CreateLightEffect(vPosition, fLifeTime, m_Light);
		pParticleList->m_bLightReady = TRUE;
	}
}

BOOL _XParticleAction_EffectScreen::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &m_Color,		sizeof(D3DCOLOR),	1, fileptr ) < 1 ) return FALSE;
	if( fread( &m_fFadeTime,	sizeof(FLOAT),		1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

void _XParticleAction_EffectScreen::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() )
	{
		if (pParticleList->m_bScreenReady == TRUE)
		{
			pParticleList->m_bScreenReady = FALSE;
		}
		return;
	}
	if (_XParticleActionBase::bLocalUser == TRUE)
	{
		if (pParticleList->m_bScreenReady == FALSE)
		{
			g_ParticleEffectManager.StartScreenEffect(m_Color, m_fFadeTime);
			pParticleList->m_bScreenReady = TRUE;
		}
	}
}


BOOL _XParticleAction_CameraShake::LoadSubClassData( FILE* fileptr )
{
	if( !fileptr ) return FALSE;
	if( fread( &m_fShakeTime,	sizeof(FLOAT),		1, fileptr ) < 1 ) return FALSE;
	return TRUE;
}

void _XParticleAction_CameraShake::Execute(_XParticleList* pParticleList)
{
	if( !CheckActionTime() )
	{
		return;
	}
	
	g_LodTerrain.m_3PCamera.SetCameraShakeMode( TRUE, m_fShakeTime );
}


// XParticleDefine.h: interface for the _XParticleDefine class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPARTICLEDEFINE_H_
#define _XPARTICLEDEFINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <float.h>
#include "d3dx9.h"

#define _XP_MAXFLOAT	1.0e16f	// Actually this must be < sqrt(MAXFLOAT) since we store this value squared.
#define _XP_MAXINT		0x7fffffff
#define _XP_EPS			1e-3f

#define M_PI 3.1415926535897932384626433f
#define drand48() (((float) rand())/((float) RAND_MAX))
#define srand48(x) srand(x)
#define inline __forceinline
#define SQRT2PI 2.506628274631000502415765284811045253006
#define ONEOVERSQRT2PI (1. / SQRT2PI)
#define ONE_OVER_SIGMA_EXP (1.0f / 0.7975f)

static inline float fsqr(float f) { return f * f; }

#define BOIDLEN 0.3f

//////////////////////////////////////////////////////////////////////
// Type codes for all actions
typedef enum _XParticleActionType
{
	_XPA_Unknown = -1,
	_XPA_HeaderID = 0,			
	_XPA_AvoidID,			
	_XPA_BounceID,			
	_XPA_CallActionListID,	
	_XPA_CopyVertexBID,	
	_XPA_DampingID,		
	_XPA_ExplosionID,		
	_XPA_FollowID,			
	_XPA_GravitateID,		
	_XPA_GravityID,		
	_XPA_JetID,			
	_XPA_KillOldID,		
	_XPA_MatchVelocityID,	
	_XPA_MoveID,			
	_XPA_OrbitLineID,		
	_XPA_OrbitPointID,		
	_XPA_RandomAccelID,	
	_XPA_RandomDisplaceID,	
	_XPA_RandomVelocityID,	
	_XPA_RestoreID,		
	_XPA_SinkID,			
	_XPA_SinkVelocityID,	
	_XPA_SourceID,
	_XPA_SpeedLimitID,		
	_XPA_TargetColorID,	
	_XPA_TargetSizeID,		
	_XPA_TargetVelocityID,	
	_XPA_VortexID,
	_XPA_VertexID,
	_XPA_StopVolumeID,
	_XPA_EffectSoundID,
	_XPA_EffectLightID,
	_XPA_EffectScreenID,
	_XPA_CameraShakeID,

	_XPA_MaxCount
}_XParticleActionType;

enum _XParticleEmitterType
{
	_XPE_Point		= 0, // Single point
	_XPE_Line,			 // Line segment
	_XPE_Triangle,		 // Triangle
	_XPE_Plane,			 // Arbitrarily-oriented plane
	_XPE_Box,			 // Axis-aligned box
	_XPE_Sphere,		 // Sphere	
	_XPE_Cylinder,		 // Cylinder
	_XPE_Cone,			 // Cone
	_XPE_Blob,			 // Gaussian blob
	_XPE_Disc,			 // Arbitrarily-oriented disc
	_XPE_Rectangle		 // Rhombus-shaped planar region
};

enum _XParticleEmitterHelperType
{
	_XPEHC_None			= -1,
	_XPEHC_Point		= 0, // Single point
	_XPEHC_Line1,			 // Line segment
	_XPEHC_Line2,
	_XPEHC_Triangle1,		 // Triangle
	_XPEHC_Triangle2,
	_XPEHC_Triangle3,
	_XPEHC_Plane,		 // Arbitrarily-oriented plane
	_XPEHC_Box1,			 // Axis-aligned box
	_XPEHC_Box2,
	_XPEHC_Sphere,		 // Sphere
	_XPEHC_SphereScale1,
	_XPEHC_SphereScale2,	
	_XPEHC_Cylinder1,		 // Cylinder
	_XPEHC_Cylinder2,
	_XPEHC_CylinderRadius1,
	_XPEHC_CylinderRadius2,
	_XPEHC_Cone1,			 // Cone
	_XPEHC_Cone2,
	_XPEHC_ConeRadius1,
	_XPEHC_ConeRadius2,
	_XPEHC_Blob,			 // Gaussian blob
	_XPEHC_DiscCenter,			 // Arbitrarily-oriented disc
	_XPEHC_DiscRadius1,
	_XPEHC_DiscRadius2,	
	_XPEHC_Rectangle,		 // Rhombus-shaped planar region
	_XPEHC_RectangleUCoord,
	_XPEHC_RectangleVCoord,

	_XPEHC_Extra0,
	_XPEHC_Extra1,
	_XPEHC_Extra2,
	_XPEHC_Extra3,
	_XPEHC_Extra4,
};

enum _XParticleViewTargetType
{
	_XParticleVT_None = 0,
	_XParticleVT_Camera,
	_XParticleVT_LockX,
	_XParticleVT_LockY,
	_XParticleVT_LockZ,	
	_XParticleVT_Direction,
	_XParticleVT_ArbitrarilyPoint,	
	_XParticleVT_CameraAndRollAnimation,
	_XParticleVT_FixedAndXRollAnimation,
	_XParticleVT_FixedAndYRollAnimation,
	_XParticleVT_FixedAndZRollAnimation,
	_XParticleVT_MaxCount
};

enum _XParticleRenderingType
{
	_XParticleRT_Triangle = 0,
	_XParticleRT_NumericBillboard,
	_XParticleRT_Billboard,
	_XParticleRT_XZCrossBillboard,
	_XParticleRT_XYZCrossBillboard,
	_XParticleRT_Model,
	_XParticleRT_AnimationModel,
	_XParticleRT_OrgTextureModel,
	_XParticleRT_OrgTextureAnimationModel,
	_XParticleRT_TerrainDecal,
	_XParticleRT_LineEffect,
	_XParticleRT_MaxCount
};

enum _XParticleLifeTimeType
{
	_XParticleLT_Infinity = 0,
	_XParticleLT_LimitTime,
	_XParticleLT_OneTime
};

enum _XParticleBipedLinkType
{
	_XParticleBLT_World = 0,
	_XParticleBLT_Character,
	_XParticleBLT_CharacterRightHand,
	_XParticleBLT_CharacterLeftHand,
	_XParticleBLT_CharacterRightLeg,
	_XParticleBLT_CharacterLeftLeg,
	_XParticleBLT_CharacterShortWeapon,
	_XParticleBLT_CharacterLongWeapon,
	_XParticleBLT_CharacterPelvis,
	_XParticleBLT_CharacterHead,
	_XParticleBLT_CharacterRightCalf,
	_XParticleBLT_CharacterLeftCalf,
	_XParticleBLT_CharacterRightForearm,
	_XParticleBLT_CharacterLeftForearm,
	_XParticleBLT_CharacterLeftWeapon,
	_XParticleBLT_MaxCount
};

const int _XParticleLinkBone_MaxCount = _XParticleBLT_MaxCount - _XParticleBLT_CharacterRightHand;

#define _XEMITTERSUBLINKTARGET_NONE				0
#define _XEMITTERSUBLINKTARGET_VELOCITYVOLUME	1
#define _XEMITTERSUBLINKTARGET_WEAPONTWOPOINT	2
#define _XEMITTERSUBLINKTARGET_CLOTHONEPOINT	4
#define _XEMITTERSUBLINKTARGET_OBB				8

typedef struct 
{
    D3DXVECTOR3 position;
    float       pointsize;
    D3DCOLOR    color;
} VERTEX_PARTICLE;

#define D3DFVF_PARTICLE (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_PSIZE)



#endif // !defined(_XPARTICLEDEFINE_H_)
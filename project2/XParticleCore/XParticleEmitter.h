// XParticleEmitter.h: interface for the _XParticleEmitter class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XPARTICLEEMITTER_H_
#define _XPARTICLEEMITTER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XParticleDefine.h"

class _XParticleEmitter
{
public:
	_XParticleEmitterType type;	
	D3DXVECTOR3			  p1;
	D3DXVECTOR3			  p2;
	D3DXVECTOR3			  u;
	D3DXVECTOR3			  v;
	FLOAT				  radius1;
	FLOAT				  radius2;
	FLOAT				  radius1Sqr;
	FLOAT				  radius2Sqr;
	
	FLOAT				  snapsize;
	
	void Build( FLOAT _a0, FLOAT _a1, FLOAT _a2, FLOAT _a3, FLOAT _a4, 
				FLOAT _a5, FLOAT _a6, FLOAT _a7, FLOAT _a8 );
	bool Within(const D3DXVECTOR3 &) const;
	bool LineVsSphereIntersect(D3DXVECTOR3 &nextpos, D3DXVECTOR3 &curpos);
	void Generate(D3DXVECTOR3 &pos ) const;
			
public:
	_XParticleEmitter();
	_XParticleEmitter(_XParticleEmitterType emittertype,
					  FLOAT a0=0.0f, FLOAT a1=0.0f, FLOAT a2=0.0f,
					  FLOAT a3=0.0f, FLOAT a4=0.0f, FLOAT a5=0.0f,
					  FLOAT a6=0.0f, FLOAT a7=0.0f, FLOAT a8=0.0f);

	virtual ~_XParticleEmitter();
	
	BOOL			Load( FILE* fileptr );
};

#endif // !defined(_XPARTICLEEMITTER_H_)


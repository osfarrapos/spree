// XParticleEmitter.cpp: implementation of the _XParticleEmitter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XParticleEmitter.h"

extern D3DXVECTOR3 vHalf;
extern D3DXVECTOR3 RandVec();
extern float NRand(float sigma);
extern D3DXQUATERNION _XRotationArcNormalized( D3DXVECTOR3* vec1, D3DXVECTOR3* vec2 );
extern FLOAT _XMeshMath_DistanceBetweenTwoPoint(FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2);
extern FLOAT _XMeshMath_DistanceBetweenTwoPoint(D3DXVECTOR3 v1, D3DXVECTOR3 v2);

bool _XLineAndSphereIntersect( D3DXVECTOR3& v1, D3DXVECTOR3& v2, D3DXVECTOR3& vC, float fr2 )
{
	D3DXVECTOR3 N = v2 - v1;
	D3DXVec3Normalize( &N, &N );
	
	// p1 -> p2
	D3DXVECTOR3 vL = vC - v1;
	float fd = D3DXVec3Dot( &vL, &N ) ;
	float fl = D3DXVec3Dot( &vL, &vL );
	float flsquare = fl * fl;
	
	if(fd<0 && flsquare > fr2) return false;
	
	float fm = flsquare - fd*fd;
	
	if( fm * fm > fr2) return false; 
	
	// p2 -> p1
	vL = vC - v2;
	N = -N;
	fd = D3DXVec3Dot( &vL, &N ) ;
	fl = D3DXVec3Dot( &vL, &vL );	
	flsquare = fl * fl;
	
	if(fd<0 && flsquare > fr2) return false;
	
	fm = flsquare - fd*fd;
	
	if( fm * fm > fr2) return false; 
	
	return true;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XParticleEmitter::_XParticleEmitter()
{
	type = _XPE_Point;
	snapsize = 0.0f;
}


_XParticleEmitter::_XParticleEmitter(_XParticleEmitterType emittertype,
									 FLOAT a0, FLOAT a1, FLOAT a2, FLOAT a3, FLOAT a4, 
									 FLOAT a5, FLOAT a6, FLOAT a7, FLOAT a8 )
{
	// backup org data	
	snapsize = 0.0f;
	type = emittertype;
	Build(  a0,  a1,  a2,  a3,  a4, a5,  a6,  a7,  a8 );
}

void _XParticleEmitter::Build( FLOAT _a0, FLOAT _a1, FLOAT _a2, FLOAT _a3, FLOAT _a4, 
							   FLOAT _a5, FLOAT _a6, FLOAT _a7, FLOAT _a8 )
{
	switch(type)
	{
	case _XPE_Point:
		p1 = D3DXVECTOR3(_a0, _a1, _a2);
		break;

	case _XPE_Line:
		{
			p1 = D3DXVECTOR3(_a0, _a1, _a2);
			D3DXVECTOR3 tmp(_a3, _a4, _a5);
			// p2 is vector from p1 to other endpoint.
			p2 = tmp - p1;
		}
		break;

	case _XPE_Box:
		// p1 is the min corner. p2 is the max corner.
		if(_a0 < _a3)
		{
			p1.x = _a0; p2.x = _a3;
		}
		else
		{
			p1.x = _a3; p2.x = _a0;
		}
		if(_a1 < _a4)
		{
			p1.y = _a1; p2.y = _a4;
		}
		else
		{
			p1.y = _a4; p2.y = _a1;
		}
		if(_a2 < _a5)
		{
			p1.z = _a2; p2.z = _a5;
		}
		else
		{
			p1.z = _a5; p2.z = _a2;
		}
		break;

	case _XPE_Triangle:
		{
			p1 = D3DXVECTOR3(_a0, _a1, _a2);
			D3DXVECTOR3 tp2 = D3DXVECTOR3(_a3, _a4, _a5);
			D3DXVECTOR3 tp3 = D3DXVECTOR3(_a6, _a7, _a8);
			
			u = tp2 - p1;
			v = tp3 - p1;
			
			// The rest of this is needed for bouncing.
			radius1Sqr = D3DXVec3Length( &u );
			D3DXVECTOR3 tu = u / radius1Sqr;
			radius2Sqr = D3DXVec3Length( &v );
			D3DXVECTOR3 tv = v / radius2Sqr;
			
			D3DXVec3Cross( &p2, &tu, &tv );
			D3DXVec3Normalize( &p2, &p2 ); // Must normalize it.
			
			// radius1 stores the d of the plane eqn.			
			radius1 = -D3DXVec3Dot( &p1, &p2 );
		}
		break;
	case _XPE_Rectangle:
		{
			p1 = D3DXVECTOR3(_a0, _a1, _a2);
			u = D3DXVECTOR3(_a3, _a4, _a5);
			v = D3DXVECTOR3(_a6, _a7, _a8);
			
			// The rest of this is needed for bouncing.
			radius1Sqr = D3DXVec3Length( &u );
			D3DXVECTOR3 tu = u / radius1Sqr;
			radius2Sqr = D3DXVec3Length( &v );
			D3DXVECTOR3 tv = v / radius2Sqr;
						
			D3DXVec3Cross( &p2, &tu, &tv );
			D3DXVec3Normalize( &p2, &p2 ); // Must normalize it.
			
			// radius1 stores the d of the plane eqn.
			radius1 = -D3DXVec3Dot( &p1, &p2 );
		}
		break;
	case _XPE_Plane:
		{
			p1 = D3DXVECTOR3(_a0, _a1, _a2);
			p2 = D3DXVECTOR3(_a3, _a4, _a5);
			D3DXVec3Normalize( &p2, &p2 ); // Must normalize it.
			
			// radius1 stores the d of the plane eqn.
			radius1 = -D3DXVec3Dot( &p1, &p2 );
		}
		break;
	case _XPE_Sphere:
		p1 = D3DXVECTOR3(_a0, _a1, _a2);
		if(_a3 > _a4)
		{
			radius1 = _a3; radius2 = _a4;
		}
		else
		{
			radius1 = _a4; radius2 = _a3;
		}
		radius1Sqr = radius1 * radius1;
		radius2Sqr = radius2 * radius2;
		break;
	case _XPE_Cone:
	case _XPE_Cylinder:
		{
			// p2 is a vector from p1 to the other end of cylinder.
			// p1 is apex of cone.
			
			p1 = D3DXVECTOR3(_a0, _a1, _a2);
			D3DXVECTOR3 tmp(_a3, _a4, _a5);
			p2 = tmp - p1;
			
			if(_a6 > _a7)
			{
				radius1 = _a6; radius2 = _a7;
			}
			else
			{
				radius1 = _a7; radius2 = _a6;
			}
			radius1Sqr = fsqr(radius1);
			
			// Given an arbitrary nonzero vector n, make two orthonormal
			// vectors u and v forming a frame [u,v,n.normalize()].
			D3DXVECTOR3 n = p2;
			float p2l2 = D3DXVec3LengthSq( &n ); // Optimize this.
			D3DXVec3Normalize( &n, &n ); // Must normalize it.
			
			// radius2Sqr stores 1 / (p2.p2)
			// XXX Used to have an actual if.
			radius2Sqr = p2l2 ? 1.0f / p2l2 : 0.0f;
			
			// Find a vector orthogonal to n.
			D3DXVECTOR3 basis(1.0f, 0.0f, 0.0f);
			if(fabs( D3DXVec3Dot( &basis, &n ) ) > 0.999f)
				basis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			
			// Project away N component, normalize and cross to get
			// second orthonormal vector.
			//u = basis - n * (basis * n);
			FLOAT _dot = D3DXVec3Dot( &basis, &n );
			D3DXVECTOR3 _retv = n * _dot;
			u = basis - _retv;

			D3DXVec3Normalize( &u, &u ); // Must normalize it.			
			D3DXVec3Cross( &v, &n, &u );
		}
		break;
	case _XPE_Blob:
		{
			p1 = D3DXVECTOR3(_a0, _a1, _a2);
			radius1 = _a3;
			float tmp = 1.0f/radius1;
			radius2Sqr = -0.5f*fsqr(tmp);
			radius2 = (FLOAT)( ONEOVERSQRT2PI * tmp );
		}
		break;
	case _XPE_Disc:
		{
			p1 = D3DXVECTOR3(_a0, _a1, _a2); // Center point
			p2 = D3DXVECTOR3(_a3, _a4, _a5); // Normal (not used in Within and Generate)
			D3DXVec3Normalize( &p2, &p2 ); // Must normalize it.
			
			if(_a6 > _a7)
			{
				radius1 = _a6; radius2 = _a7;
			}
			else
			{
				radius1 = _a7; radius2 = _a6;
			}
			
			// Find a vector orthogonal to n.
			D3DXVECTOR3 basis(1.0f, 0.0f, 0.0f);
			if(fabs( D3DXVec3Dot( &basis, &p2) ) > 0.999f)
				basis = D3DXVECTOR3(0.0f,  1.0f, 0.0f);
			
			// Project away N component, normalize and cross to get
			// second orthonormal vector.
			//u = basis - p2 * (basis * p2);
			
			FLOAT _dot = D3DXVec3Dot( &basis, &p2 );
			D3DXVECTOR3 _retv = p2 * _dot;
			u = basis - _retv;

			D3DXVec3Normalize( &u, &u ); // Must normalize it.			
			D3DXVec3Cross( &v, &p2, &u );
			radius1Sqr = -D3DXVec3Dot( &p1, &p2 );
		}
		break;
	}
	
}


_XParticleEmitter::~_XParticleEmitter()
{

}


// Determines if pos is inside the domain
bool _XParticleEmitter::Within(const D3DXVECTOR3 &pos) const
{
	switch (type)
	{
	case _XPE_Box:
		return !((pos.x < p1.x) || (pos.x > p2.x) ||
			(pos.y < p1.y) || (pos.y > p2.y) ||
			(pos.z < p1.z) || (pos.z > p2.z));
	case _XPE_Plane:
		// Distance from plane = n * p + d
		// Inside is the positive half-space.
		return D3DXVec3Dot( &pos, &p2) >= -radius1;
	case _XPE_Sphere:
		{
			D3DXVECTOR3 rvec(pos - p1);
			float rSqr = D3DXVec3LengthSq( &rvec );
			return rSqr <= radius1Sqr && rSqr >= radius2Sqr;
		}		
	case _XPE_Cylinder:
	case _XPE_Cone:
		{
			// This is painful and slow. Might be better to do quick
			// accept/reject tests.
			// Let p2 = vector from base to tip of the cylinder
			// x = vector from base to test point
			// x . p2
			// dist = ------ = projected distance of x along the axis
			// p2. p2 ranging from 0 (base) to 1 (tip)
			//
			// rad = x - dist * p2 = projected vector of x along the base
			// p1 is the apex of the cone.
			
			D3DXVECTOR3 x(pos - p1);
			
			// Check axial distance
			// radius2Sqr stores 1 / (p2.p2)
			float dist = D3DXVec3Dot(&p2,&x) * radius2Sqr;
			if(dist < 0.0f || dist > 1.0f)
				return false;
			
			// Check radial distance; scale radius along axis for cones
			D3DXVECTOR3 xrad = x - p2 * dist; // Radial component of x
			float rSqr = D3DXVec3LengthSq( &xrad );
			
			if(type == _XPE_Cone)
				return (rSqr <= fsqr(dist * radius1) &&
				rSqr >= fsqr(dist * radius2));
			else
				return (rSqr <= radius1Sqr && rSqr >= fsqr(radius2));
		}
		break;
	case _XPE_Blob:
		{
			D3DXVECTOR3 x(pos - p1);
			// return exp(-0.5 * xSq * Sqr(oneOverSigma)) * ONEOVERSQRT2PI * oneOverSigma;
			float Gx = expf( D3DXVec3LengthSq( &x ) * radius2Sqr) * radius2;
			return (drand48() < Gx);
		}
	case _XPE_Point:
	case _XPE_Line:
	case _XPE_Rectangle:
	case _XPE_Triangle:
	case _XPE_Disc:
	default:
		return false; // XXX Is there something better?
	}
}

bool _XParticleEmitter::LineVsSphereIntersect(D3DXVECTOR3 &nextpos, D3DXVECTOR3 &curpos)
{
	switch (type)
	{	
	case _XPE_Sphere:
		{
			return _XLineAndSphereIntersect( curpos, nextpos, p1, radius1Sqr );
		}
	default:
		return false; // XXX Is there something better?
	}
}

// Generate a random point uniformly distrbuted within the domain
void _XParticleEmitter::Generate(D3DXVECTOR3 &pos) const
{
	switch (type)
	{
	case _XPE_Point:
		pos = p1;
		break;
	case _XPE_Line:
		pos = p1 + p2 * drand48();
		if( snapsize != 0.0f )
		{
			pos.x = pos.x - fmod( pos.x, snapsize );
			pos.y = pos.y - fmod( pos.y, snapsize );
			pos.z = pos.z - fmod( pos.z, snapsize );
		}
		break;
	case _XPE_Box:
		// Scale and translate [0,1] random to fit box
		pos.x = p1.x + (p2.x - p1.x) * drand48();
		pos.y = p1.y + (p2.y - p1.y) * drand48();
		pos.z = p1.z + (p2.z - p1.z) * drand48();
		if( snapsize != 0.0f )
		{
			pos.x = pos.x - fmod( pos.x, snapsize );
			pos.y = pos.y - fmod( pos.y, snapsize );
			pos.z = pos.z - fmod( pos.z, snapsize );
		}
		break;
	case _XPE_Triangle:
		{
			float r1 = drand48();
			float r2 = drand48();
			if(r1 + r2 < 1.0f)
				pos = p1 + u * r1 + v * r2;
			else
				pos = p1 + u * (1.0f-r1) + v * (1.0f-r2);
			if( snapsize != 0.0f )
			{
				pos.x = pos.x - fmod( pos.x, snapsize );
				pos.y = pos.y - fmod( pos.y, snapsize );
				pos.z = pos.z - fmod( pos.z, snapsize );
			}
		}
		break;
	case _XPE_Rectangle:
		pos = p1 + u * drand48() + v * drand48();
		if( snapsize != 0.0f )
		{
			pos.x = pos.x - fmod( pos.x, snapsize );
			pos.y = pos.y - fmod( pos.y, snapsize );
			pos.z = pos.z - fmod( pos.z, snapsize );
		}
		break;
	case _XPE_Plane: // How do I sensibly make a point on an infinite plane?
		pos = p1;
		if( snapsize != 0.0f )
		{
			pos.x = pos.x - fmod( pos.x, snapsize );
			pos.y = pos.y - fmod( pos.y, snapsize );
			pos.z = pos.z - fmod( pos.z, snapsize );
		}
		break;
	case _XPE_Sphere:
		// Place on [-1..1] sphere
		pos = RandVec() - vHalf;
		D3DXVec3Normalize( &pos, &pos );
		
		// Scale unit sphere pos by [0..r] and translate
		// (should distribute as r^2 law)
		if(radius1 == radius2)
			pos = p1 + pos * radius1;
		else
			pos = p1 + pos * (radius2 + drand48() * (radius1 - radius2));
		
		if( snapsize != 0.0f )
		{
			pos.x = pos.x - fmod( pos.x, snapsize );
			pos.y = pos.y - fmod( pos.y, snapsize );
			pos.z = pos.z - fmod( pos.z, snapsize );
		}
		break;
	case _XPE_Cylinder:
	case _XPE_Cone:
		{
			// For a cone, p2 is the apex of the cone.
			float dist = drand48(); // Distance between base and tip
			float theta = drand48() * 2.0f * float(M_PI); // Angle around axis
			// Distance from axis
			float r = radius2 + drand48() * (radius1 - radius2);
			
			float x = r * cosf(theta); // Weighting of each frame vector
			float y = r * sinf(theta);
			
			// Scale radius along axis for cones
			if(type == _XPE_Cone)
			{
				x *= dist;
				y *= dist;
			}
			
			pos = p1 + p2 * dist + u * x + v * y;

			if( snapsize != 0.0f )
			{
				pos.x = pos.x - fmod( pos.x, snapsize );
				pos.y = pos.y - fmod( pos.y, snapsize );
				pos.z = pos.z - fmod( pos.z, snapsize );
			}
		}
		break;
	case _XPE_Blob:
		pos.x = p1.x + NRand(radius1);
		pos.y = p1.y + NRand(radius1);
		pos.z = p1.z + NRand(radius1);

		if( snapsize != 0.0f )
		{
			pos.x = pos.x - fmod( pos.x, snapsize );
			pos.y = pos.y - fmod( pos.y, snapsize );
			pos.z = pos.z - fmod( pos.z, snapsize );
		}
		
		break;
	case _XPE_Disc:
		{
			float theta = drand48() * 2.0f * float(M_PI); // Angle around normal

			if( snapsize != 0.0f ) theta = theta - fmod( theta, _X_RAD(snapsize) );

			// Distance from center
			float r = radius2 + drand48() * (radius1 - radius2);
			
			float x = r * cosf(theta); // Weighting of each frame vector
			float y = r * sinf(theta);
			
			pos = p1 + u * x + v * y;
		}
		break;
	default:
		pos = D3DXVECTOR3(0,0,0);
	}
}


BOOL _XParticleEmitter::Load( FILE* fileptr )
{
	if( !fileptr ) return FALSE;

	//FLOAT _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8;
	FLOAT data[9];
	
	fread( &type, sizeof(_XParticleEmitterType), 1, fileptr );	
	fread( data, sizeof(FLOAT) * 9, 1, fileptr );
	fread( &snapsize, sizeof(FLOAT), 1, fileptr );
	Build( data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8] );

	/*
	fread( &_a0, sizeof(FLOAT), 1, fileptr );
	fread( &_a1, sizeof(FLOAT), 1, fileptr );
	fread( &_a2, sizeof(FLOAT), 1, fileptr );
	fread( &_a3, sizeof(FLOAT), 1, fileptr );
	fread( &_a4, sizeof(FLOAT), 1, fileptr );
	fread( &_a5, sizeof(FLOAT), 1, fileptr );
	fread( &_a6, sizeof(FLOAT), 1, fileptr );
	fread( &_a7, sizeof(FLOAT), 1, fileptr );
	fread( &_a8, sizeof(FLOAT), 1, fileptr );
	fread( &snapsize, sizeof(FLOAT), 1, fileptr );
	Build( _a0, _a1, _a2, _a3, _a4, _a5, _a6, _a7, _a8 );
	*/

	return TRUE;
}

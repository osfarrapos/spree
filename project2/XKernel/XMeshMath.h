//-----------------------------------------------------------------------------
// File: Math.h
//
//-----------------------------------------------------------------------------
#ifndef _XMESHMATH_H_
#define _XMESHMATH_H_

#include <d3d9.h>
#include <d3dx9.h>


#define D3DFVF_XMESH_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

typedef struct _XMESH_VERTEX
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
		
	FLOAT nx;
	FLOAT ny;
	FLOAT nz;

	FLOAT tu1;
	FLOAT tv1;
	static DWORD GetFVF( void ){ return D3DFVF_XMESH_VERTEX; }
} _XMESH_VERTEX, * _LPXMESH_VERTEX;

#define D3DFVF_XMESH_VERTEX_UV2 (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2 )
typedef struct _XMESH_VERTEX_UV2
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
		
	FLOAT nx;
	FLOAT ny;
	FLOAT nz;

	FLOAT tu1;
	FLOAT tv1;

	FLOAT tu2;
	FLOAT tv2;
	static DWORD GetFVF( void ){ return D3DFVF_XMESH_VERTEX_UV2; }
} _XMESH_VERTEX_UV2, * _LPXMESH_VERTEX_UV2;

#define D3DFVF_XMESH_PSVERTEX ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2  | D3DFVF_TEXCOORDSIZE2(0) | D3DFVF_TEXCOORDSIZE4(1) )

typedef struct _XMESH_PSVERTEX
{
	FLOAT x;
	FLOAT y;
	FLOAT z;
	
	FLOAT nx;
	FLOAT ny;
	FLOAT nz;
	
	FLOAT tu;
	FLOAT tv;
	
	FLOAT weight0, weight1;
	FLOAT index0,   index1;  //버텍스당 2개의 본과 그에 상당한 본의 인덱스
	/*	
	D3DXVECTOR3		S;
	D3DXVECTOR3		T;
    D3DXVECTOR3		SxT;
	*/
	
	static DWORD GetFVF( void ){ return D3DFVF_XMESH_PSVERTEX; }
} _XMESH_PSVERTEX, * _LPXMESH_PSVERTEX;

D3DXQUATERNION _XRotationArc( D3DXVECTOR3* vec1, D3DXVECTOR3* vec2 );
D3DXQUATERNION _XRotationArcNormalized( D3DXVECTOR3* vec1, D3DXVECTOR3* vec2 );
FLOAT _XFastDistance(FLOAT x, FLOAT z);
FLOAT _XDistance(FLOAT x, FLOAT z);
FLOAT _XGetAngleFromVectors( D3DXVECTOR3* v1, D3DXVECTOR3* v2 );
void  _XMeshMath_RotateCoord( FLOAT& x, FLOAT& z, FLOAT angle );
void  _XMeshMath_XYRotateCoord( D3DXVECTOR3& vec, FLOAT sinvalue, FLOAT cosvalue );
void  _XMeshMath_XYRotateCoord( D3DXVECTOR3& vec, FLOAT angle );
void  _XMeshMath_RotateAngleXYZ( D3DXMATRIX& m, FLOAT& x, FLOAT& y, FLOAT& z);

////////////////////////////////////////////////////////////////////////
// Quaternion 
////////////////////////////////////////////////////////////////////////

inline FLOAT _XMeshMath_SquareMagnitude (const D3DXVECTOR3& v);
inline FLOAT _XMeshMath_Magnitude (const D3DXVECTOR3& v);

//-----------------------------------------------------------------------------
// Name: Math_UnitMatrixFromMatrix()
// Desc: 메트릭스를 유닛메트릭스로 변환.. 스칼라 값이 빠지게 된다.. 4행렬은 무시
//-----------------------------------------------------------------------------
VOID _XMeshMath_UnitMatrixFromMatrix(D3DXMATRIX& m);

//-----------------------------------------------------------------------------
// Name: Math_QuaternionFromRotation()
// Desc: 회전값을 이용해 쿼터니언을 생성..
//-----------------------------------------------------------------------------
VOID _XMeshMath_QuaternionFromRotation( D3DXQUATERNION &q, D3DXVECTOR3& v, FLOAT fTheta );

//-----------------------------------------------------------------------------
// Name: Math_RotationFromQuaternion()
// Desc: 쿼터니언을 이용해 회전값을 생성한다..
//-----------------------------------------------------------------------------
VOID _XMeshMath_RotationFromQuaternion( D3DXVECTOR3& v, FLOAT& fTheta, D3DXQUATERNION &q);

//-----------------------------------------------------------------------------
// Name: Math_QuaternionFromAngles()
// Desc: Angles값(fYaw, fPitch, fRoll) 값을 이용해.. 쿼터니언 생성..
//-----------------------------------------------------------------------------
VOID _XMeshMath_QuaternionFromAngles( D3DXQUATERNION &q, FLOAT fYaw, FLOAT fPitch, FLOAT fRoll );

//-----------------------------------------------------------------------------
// Name: _XMeshMath_QuaternionToEulerAngle()
// Desc: Quaternion to euler angle
//-----------------------------------------------------------------------------
VOID _XMeshMath_QuaternionToEulerAngle(const D3DXQUATERNION& q, float& fYaw, float& fPitch, float& fRoll);

//-----------------------------------------------------------------------------
// Name: Math_MatrixFromQuaternion()
// Desc: 쿼터니언을 이용해 매트릭스 생성..
//-----------------------------------------------------------------------------
VOID _XMeshMath_MatrixFromQuaternion( D3DXMATRIX& mat, D3DXQUATERNION &trans);

//-----------------------------------------------------------------------------
// Name: Math_QuaternionFromMatrix()
// Desc: 매트릭스에서 쿼터니언 축출하기..
//-----------------------------------------------------------------------------
VOID _XMeshMath_QuaternionFromMatrix( D3DXQUATERNION &q, D3DXMATRIX& mat );

//-----------------------------------------------------------------------------
// Name: Math_QuaternionMultiply()
// Desc: 쿼터니언 곱하기.. q = a*b
//-----------------------------------------------------------------------------
VOID _XMeshMath_QuaternionMultiply( D3DXQUATERNION &q, D3DXQUATERNION &a, D3DXQUATERNION &b );


//-----------------------------------------------------------------------------
// Name: Math_QuaternionSlerp()
// Desc: q 는 a와 b의 진행률(fAlpha) 만큼 진행시킨 값이다. 
//-----------------------------------------------------------------------------
VOID _XMeshMath_QuaternionSlerp( D3DXQUATERNION &q, D3DXQUATERNION &a, D3DXQUATERNION &b, FLOAT fAlpha );


///////////////////////////////////////////////////////////////////////////////
// Name DistanceBetweenTwoPoint()
// Desc 점 a와 점 b 와의 거리를 구한다.. 
//////////////////////////////////////////////////////////////////////////////
FLOAT _XMeshMath_DistanceBetweenTwoPoint(FLOAT x1, FLOAT y1, FLOAT z1, FLOAT x2, FLOAT y2, FLOAT z2);
FLOAT _XMeshMath_DistanceBetweenTwoPoint(D3DXVECTOR3 v1, D3DXVECTOR3 v2);



// 이동 행렬을 채우고 난후, 이것을 이동시키기 위해 현재 월드 행렬과 곱할때 사용할 곱셈 함수.
VOID _XMeshMath_MatrixMultiply( D3DXMATRIX& q, D3DXMATRIX& a, D3DXMATRIX& b );
// 벡터와 매트릭스의 곱셈..
HRESULT _XMeshMath_VectorMatrixMultiply( D3DXVECTOR3& vDest, D3DXVECTOR3& vSrc, D3DXMATRIX& mat);
// 버텍스와 매트릭스의 곱셈..
HRESULT _XMeshMath_VertexMatrixMultiply( _XMESH_VERTEX& vDest, _XMESH_VERTEX& vSrc, D3DXMATRIX& mat);
// 역행렬을 구하는 함수..
HRESULT _XMeshMath_MatrixInvert( D3DXMATRIX& q, D3DXMATRIX& a );


// MATRIX 값을 단위행렬로 만드는 함수..
VOID _XMeshMath_SetIdentityMatrix( D3DXMATRIX& m );

// z,y,z 축을 따라 이동할 (tx,ty,tz)값을 받아들이고 이동행렬을 생성하는 함수..
VOID _XMeshMath_SetTranslateMatrix( D3DXMATRIX& m, FLOAT tx, FLOAT ty, FLOAT tz );
VOID _XMeshMath_SetTranslateMatrix( D3DXMATRIX& m, D3DXVECTOR3& v );

// 벡터를 가지고 회전 하기..
VOID _XMeshMath_RotateVector(D3DXMATRIX &m, FLOAT vAngle, D3DXVECTOR3 &vDir);

///////////////////////////////////////////////////////////////////////////////
// Name Situation_Eye()
// Desc D3DUtil_SetViewMatrix()와 비슷한 함수이나 eye_z 가 보는 점이아니라.
// 바라보는 방향이라는 것만 다르다.. 이것은 로컬 이동을 하기 위해서 
// 작성한 함수..
//////////////////////////////////////////////////////////////////////////////
VOID _XMeshMath_SituationEye(D3DXMATRIX& m,D3DXVECTOR3& eye_sit,D3DXVECTOR3& eye_Z,D3DXVECTOR3& eye_Y);

//-----------------------------------------------------------------------------
// Name: Math_SetViewMatrix()
// Desc: Given an eye point, a lookat point, and an up vector, this
//       function builds a 4x4 view matrix.
//-----------------------------------------------------------------------------
HRESULT _XMeshMath_SetViewMatrix( D3DXMATRIX& mat, D3DXVECTOR3& vFrom,
                                  D3DXVECTOR3& vAt, D3DXVECTOR3& vWorldUp );

FLOAT _XMeshMath_GetRotateRadian( D3DXVECTOR3& a, D3DXVECTOR3& b );


int  D3DXLineEquation(D3DXVECTOR3& vP0, D3DXVECTOR3& vP1, D3DXVECTOR3& vP2 );
VOID D3DXVec3Identity( D3DXVECTOR3& Vector );

int		v_minus_v3 (float r[3], float u[3], float v[3]);
int		c_mult_v3 (float r[3], float c, float u[3]);
int		ScalarMultD3DVector(D3DXVECTOR3* r, FLOAT c, D3DXVECTOR3* u);
int		D3DVectorMinusD3DVector(D3DXVECTOR3* r, D3DXVECTOR3* u, D3DXVECTOR3* v);
FLOAT	Magnitude(D3DXVECTOR3* a);
int		IsZero(float r);
BOOL	NormalizeD3DVectorWithLength(D3DXVECTOR3* a, FLOAT r);
HRESULT SetViewMatrix( LPD3DXMATRIX mat, D3DXVECTOR3* vFrom, D3DXVECTOR3* vAt, D3DXVECTOR3* vWorldUp );
VOID	D3DMatrixYRotation(D3DXMATRIX* mat, FLOAT angle);
VOID	FindPositionOnCircle(D3DXVECTOR3& vrOrig, D3DXVECTOR3& vCurrentPo, float fRadian, float fR, int nID);


BOOL	_XIntersectTriangle( D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2 );

template <class A> inline void _SWAP(A &a, A &b)
{   A   c;
    c = a;
    a = b;
    b = c;
}

unsigned __int64 snapshot_rdtsc();

#endif
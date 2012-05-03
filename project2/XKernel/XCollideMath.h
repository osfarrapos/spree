#ifndef SUMATH_H
#define SUMATH_H

#include <d3dx9.h>
#include <d3d9types.h>
#include <d3dx9math.h>

struct PLANEQUATION
{
 	D3DXVECTOR3 vNomal; //Top left
	float		fD;		//Top right
};

HRESULT SuMath_VectorMatrixMultiply( D3DXVECTOR3* vDest, D3DXVECTOR3* vSrc,
									D3DMATRIX* mat);
VOID	MakeIdentiVec( D3DXVECTOR3* psuVector );
VOID	FindThePointOnTheLine(D3DXVECTOR3* pOutput, D3DXVECTOR3* pvO, D3DXVECTOR3* pvD, float t);
VOID	Vectorsub(D3DXVECTOR3* pOutput, D3DXVECTOR3* pvO, D3DXVECTOR3* pv1);
int		LineEquation(D3DXVECTOR3* vP0, D3DXVECTOR3* vP1, D3DXVECTOR3* vP2 );
VOID	PlaneEquation( PLANEQUATION* pPlaneEq, D3DXVECTOR3* vP0, D3DXVECTOR3* vP1, D3DXVECTOR3* vP2);
float	GetRotateRadian( LPDIRECT3DDEVICE9 pd3dDevice, POINT ptCursor );
float	GetRotateRadianFromTwoPoint( LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3* pvbasis, D3DXVECTOR3* pvDir,
									D3DXMATRIX*	pViewMat )	;
float	GetRotateRadian( LPDIRECT3DDEVICE9 pd3dDevice, D3DXVECTOR3* pvbasis, POINT ptCursor, D3DXMATRIX*	pViewMat, D3DXMATRIX*	pProjMat, D3DVIEWPORT9*	pvp );
VOID	FindePoOnCircle(D3DXVECTOR3& vrOrig, D3DXVECTOR3& vCurrentPo, float fRadian, float fR, int nID);
float   GetPitchRadian( D3DXVECTOR3& vrOrig, D3DXVECTOR3& vCurrendtPo);
float	GetRadian( D3DXVECTOR3& vrOrig , D3DXVECTOR3& vCurrendtPo, int nID);

VOID	FindePoOnPitchRotatedCircle(D3DXVECTOR3& vrOrig, D3DXVECTOR3& vCurrendtPo, float fYawRaian, float fPich, float fR);
float	SuVec3Length( D3DXVECTOR3& v1, D3DXVECTOR3& v2 );
D3DXVECTOR3 GetAxisVector( D3DXVECTOR3& v1, D3DXVECTOR3& vTarget, D3DXVECTOR3& vAxis ); //축 벡터를 구한다.
float	GetAngleUseingDot( D3DXVECTOR3* pvA, D3DXVECTOR3* pvB); // 두 벡터의 내적을 이용해 두 벡터가 이루는 각을 구한다.
VOID	InitRotateMat( float fDegree, D3DXMATRIX&	matRotate);	// Y축을 고정시킨 회전된 메트릭스를 만든다.
VOID	GetRotateMat( D3DMATRIX& matRoate, D3DXVECTOR3& Axis, float fRadian); //축 Axis를 기준으로 fRadian 만큼 회전한 메트릭스를 구한다.
VOID	GetRightAxis( D3DXVECTOR3& vRight, const D3DXVECTOR3& vDirection); //단위 벡터인 vDirection으로 vRight벡터를 구한다.
VOID	GetRight_Up_Axis( D3DXVECTOR3& vRight, D3DXVECTOR3& vUp, const D3DXVECTOR3& vDirection );
//내용은 모른다 버텍스의 좌표들을 집어 넣으면.. obb 박스인 센터 값과. 3개의 축을 구할수 있다..
//어떻게 돌아 가는지 모름.. 3D 겜 엔진책 보고 배겼음..
void	SuGaussPointsFit ( int iQuantity, const D3DXVECTOR3* akPoint, D3DXVECTOR3& rkCenter, D3DXVECTOR3 akAxis[3], float afExtent[3] ); 

#endif
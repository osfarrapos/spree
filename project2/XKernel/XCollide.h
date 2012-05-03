#ifndef __SUCOLLIDE_H__
#define __SUCOLLIDE_H__

#include "d3d9types.h"
#include "d3dx9math.h"


struct SUVERTEX
{
    FLOAT	x, y, z, rhw;
    DWORD	color;
	FLOAT   tu, tv; // Vertex texture coordinates
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct PLANE2
{
 	float		Distance;
	D3DXVECTOR3	Normal;
};

struct SuVIEWVOLUME
{
	PLANE2 Plane[6];
};

struct SUPOLIGON
{
	D3DXVECTOR3 p[4];
};

struct SUSQUARE
{
	D3DXVECTOR3 p[4];
};

struct SUINTERSECTION
{
    int			dwFace;                 // mesh face that was intersected
    FLOAT		fBary1, fBary2;         // barycentric coords of intersection
    FLOAT		fDist;                  // distance from ray origin to intersection
    FLOAT		tu, tv;                 // texture coords of intersection
	SUPOLIGON	poly;
};

//===============================================================================================================
// 캐릭터를 마우스 피킹을 위해서 가상의 바운딩 십자가 판대기를 위한 구조체
// 주의 : 구조체 수정 필요하다. 넘 비효율적임 TT
//===============================================================================================================
struct BOUNDINGCROSS  
{
	SUPOLIGON	FrontPoly[2] ;			// 십자가 판대기중 XY축 면의 과 평행한 판대기의 폴리곤4개
	SUPOLIGON	SidePoly[2] ;			// 십자가 판대기중 ZY축 관 평해한 판대기의 폴리곤4개
	WORD		wIndex;					// 현재 자기캐릭터 인덱스
	float		fIntersectedLength;		// 뷰에서 픽킹된 거리
	bool		IsPicked;
};
//----------카메라에 관련된  디파인 문
#define CAMERSTOP					1		//카메라 스톱
#define CAMERAFORWARD				2		//카메라 전진
#define CAMERABACKWARD				3		//카메라 후진	

//------------------------------------

//----------------------------------------------------------------------------------------------------
//----------지형 ROAM에 관련 디파인 문

// Our custom FVF, which describes our custom vertex structure
// 버텍스 버퍼에서 FVF을 사용하기 위해서 아래의 플래그 조합을 사용한다.
// D3DFVF_XYZ : 정점은 변환(transformation)되지 않지만 
// Direct3D는 리이팅 계산을 수행하지는 않지만 설정된 월드, 뷰, 투사행렬을 사용 하여 변환 한다.

//#define MAXSIZE							1024		// 맵은 정사각형이므로 
#define FRAMEVARIANCE					50			// 프레임 변화량
#define MOVEOFFSET_PATCHCENTERPOINT		32			// 점이 이동할 offset
// Desired number of Binary Triangle tessellations per frame.
// This is not the desired number of triangles rendered!
// There are usually twice as many Binary Triangle structures as there are rendered triangles.
#define DESIREDTRIS		10000;

// Observer and Follower modes
#define FOLLOW_MODE		(0)
#define OBSERVE_MODE	(1)
#define DRIVE_MODE		(2)
#define FLY_MODE		(3)

//----------------------------------------------------------------------------------------------------

void ExtractFrustumPlanes(SuVIEWVOLUME* pViewVolume, LPDIRECT3DDEVICE9	pd3dDevice);//, PLANE2 *Planes )

bool IsPointInViewVolume( SuVIEWVOLUME* pViewVolume, D3DXVECTOR3* pvPoint);

BOOL IntersectRayTriangle( const D3DXVECTOR3& orig, const D3DXVECTOR3& dir,
							  D3DXVECTOR3& v0, D3DXVECTOR3& v1, D3DXVECTOR3& v2,
							  SUINTERSECTION* pintersectionInfo);

BOOL CharicPick( POINT ptCursor, BOUNDINGCROSS* pBoundingCross, LPDIRECT3DDEVICE9 pd3dDevice, 
				 D3DXMATRIX* p_Viewmat, D3DXMATRIX*	pProjMat, D3DVIEWPORT9*	pvp, SUINTERSECTION* pIntersectionInfo );
BOOL RayAndSphereIntersect( D3DXVECTOR3& vO, D3DXVECTOR3& vD, D3DXVECTOR3& vC, float fr); //구와 레이의 교차여부
BOOL RayAndSphereIntersectFindPoint( D3DXVECTOR3& vO, D3DXVECTOR3& vD, D3DXVECTOR3& vC, float fr, float& ft); //구와 레이의 교차첨을 구한다.
bool MgcTestIntersection (const D3DXVECTOR3& rkRay, const D3DXVECTOR3& vC, float fR);
FLOAT _XSphereIntersectRay( D3DXVECTOR3 &p_RayO, D3DXVECTOR3 &p_RayV, D3DXVECTOR3 &p_SO, FLOAT p_dSR );

int SercheClosestNo(SUINTERSECTION* interInfo, int n);
D3DXVECTOR3* FindYValueOfm_pVertex( D3DXVECTOR3* pv0 ,SUPOLIGON* pPlan);
int SercheClosestIntersectionInfo(SUINTERSECTION* interInfo, int n);
BOUNDINGCROSS* CreateAndUpdateBoundCross( BOUNDINGCROSS* pBoundingCrss, float fcenterXPo, float fcenterYPo, float fcenterZPo, 
					  int nXOffset, int nYOffset , int nZOffset );

#endif	// __SUCOLLIDE_H__

#ifndef CSuX3D_OBOX_H
#define CSuX3D_OBOX_H

#include <d3dx9.h>
#include "XList.h"

#define _XDEF_X3DOBBFILEINDICATOR		0xFFFFAF05

typedef struct 
{
	D3DXVECTOR3 kCenter;
    D3DXVECTOR3 akAxis[3];
	float		afExtent[3];
	WORD		xcoord;
	WORD		zcoord;
	int			OBBLevel;
} _XOBB_SaveFormat;



struct BOXVERTEX
{
    D3DXVECTOR3 p;      // Vertex position
	D3DCOLOR    color;  // Vertex color
};
#define D3DFVF_BOXVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)

class CSuModelBox
{
public:
    CSuModelBox();
	virtual ~CSuModelBox();
	void InitBox( D3DXVECTOR3* pVertexs, D3DXVECTOR3& vCenter );
	void Render( D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255 ) );
	void Render( D3DMATRIX& mat, D3DXVECTOR3* pAxis, D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 255, 255 ) );
	inline D3DXVECTOR3& Getm_vCenter(){ return m_vCenter; };
	BOXVERTEX* Getm_Vertices(){ return m_pVertex; }; 
	
protected:
    
	LPDIRECT3DDEVICE9		m_pd3dDevice;
	BOXVERTEX*				m_pVertex;
	D3DXVECTOR3				m_vCenter;
};


class CsuX3D_Box
{
public:
    CsuX3D_Box ();
	void Create(D3DXVECTOR3 &Center,D3DXVECTOR3 Axis[3],float Extent[3]);

    D3DXVECTOR3& Center ();
	void SetCenter (D3DXVECTOR3 &Center);

    D3DXVECTOR3* Axis();
	void SetAxis (D3DXVECTOR3 Axis[3]);
    float* Extents ();
	void SetExtents(D3DXVECTOR3 &Extent);

	D3DXVECTOR3* Vertices(); 
	void Update();

	D3DXVECTOR3  Min();
	D3DXVECTOR3  Max();

	void Setapex( D3DXVECTOR3* pv);
	inline D3DXVECTOR3* Getm_vapex(){ return m_vapex; }

	BOOL InterOBBvsOBB(CsuX3D_Box& rkBox0);	

	void ComputeVertices(D3DXVECTOR3 *Vertex);

public:
    D3DXVECTOR3 m_kCenter;
    D3DXVECTOR3 m_akAxis[3];
	float		m_afExtent[3];
	D3DXVECTOR3 akVertex[8];
	D3DXVECTOR3 m_vapex[8];
	D3DXVECTOR3 m_kMin;
	D3DXVECTOR3 m_kMax;
	int			m_OBBLevel;
};


typedef struct _XOBBCollideInfo
{
	D3DXVECTOR3		pos;
	D3DXVECTOR3		normal;
	float			depth;
	int				type;
}_XOBBCollideInfo;

static void SegmentSegmentCollision(float &t0,float &t1,D3DXVECTOR3 r0,D3DXVECTOR3  dir0,D3DXVECTOR3 r1,D3DXVECTOR3 dir1);

// by mahwang
// std::vector ·Î ¼öÁ¤
class _XOBBData// : public _XItem
{
public:
    _XOBBData ();
	void Create(D3DXVECTOR3 &Center,D3DXVECTOR3 Axis[3],float Extent[3]);

    D3DXVECTOR3& Center ();
	void SetCenter (D3DXVECTOR3 &Center);

    D3DXVECTOR3* Axis();
	void SetAxis (D3DXVECTOR3 Axis[3]);
    float* Extents ();
	void SetExtents(D3DXVECTOR3 &Extent);

	void RecalcRadius( void );
	
	BOOL InterOBBvsOBB(_XOBBData& rkBox0);
	int  InterOBBvsOBB(_XOBBData& rkBox0, D3DXVECTOR3& offset );
	BOOL InterOBBvsRay( D3DXVECTOR3 mouserayorigin, D3DXVECTOR3 mouseraydirection, D3DXVECTOR3 &PickedPoint );
	BOOL InterOBBvsRay( D3DXVECTOR3 mouserayorigin, D3DXVECTOR3 mouseraydirection, D3DXVECTOR3 &Offset, D3DXVECTOR3 &PickedPoint );
	bool InterMOBB(D3DXVECTOR3 &sVel, _XOBBData& rkBox, D3DXVECTOR3& offset, D3DXVECTOR3 &tVel,_XOBBCollideInfo *Contact);

	void GetSpan(const D3DXVECTOR3 & xAxis, float& min, float& max);
	bool SpanIntersect(D3DXVECTOR3 & xAxis, _XOBBData& xBox, float& depth);
	bool GetCollisionPlane(D3DXVECTOR3 * A, float* d, D3DXVECTOR3 & Ncoll, float &dcoll);
	bool IntersectOBBvsOBB(_XOBBData& rkBox,  D3DXVECTOR3& xP1 );

	BOOL Load( FILE* FilePtr );
	BOOL Save( FILE* FilePtr );

	void CreateModelVertex( void );

public:
    D3DXVECTOR3		m_kCenter;
    D3DXVECTOR3		m_akAxis[3];
	float			m_afExtent[3];	
	FLOAT			m_Radius;
	int				m_OBBLevel;
	D3DXVECTOR3		m_ModelVertex[8];
	BOOL			m_bCollidable;

#ifdef _XDWDEBUG
public:
	BOOL			m_Collide;
	CSuModelBox		m_OBBModel;
	D3DXVECTOR3		m_akVertex[8];
	
	D3DXVECTOR3*	Vertices(); 
	void			CreateOBBModel( void );
#endif

};

class _XOBBItem	:	public _XItem
{
public:
	_XOBBData* m_pOBBdata;	
};

extern void _XCreateOBBFromVertexList( D3DXVECTOR3* pVertex, int vertexcount, _XOBBData* pOBBData );
extern void _XOBBRotate( float a[3][3], float s, float tau, int i, int j, int k, int l );
extern void _XOBBMakeAlignedBox(CsuX3D_Box *Box,int Size, D3DXVECTOR3* Point); 
extern void _XOBBMakeOBBBox(CsuX3D_Box *Box,int Size, D3DXVECTOR3* Vertex);
extern CsuX3D_Box _XOBBMergeBoxes (CsuX3D_Box& Box0,CsuX3D_Box& Box1);
extern void _XOBBGetRotMatrix(D3DXMATRIX *m,D3DXVECTOR3 *Axis);
extern void _XOBBGetAxisFromMatrix(D3DXVECTOR3 *Axis,D3DXMATRIX *m);

extern WORD g_OBBModelVertIndex[36];

#endif

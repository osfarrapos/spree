#ifndef _XBILLBOARD_
#define _XBILLBOARD_

#include <d3dx9.h>


#define BBUpVector		0
#define BBRightVector	1

#define D3DFVF_XBILLBOARD_VERTEX  ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1 )


typedef struct _XBILLBOARD_VERTEX
{ 
    
	FLOAT x; 
    FLOAT y; 
    FLOAT z; 
    
	D3DCOLOR color; 
    D3DCOLOR specular; 

    FLOAT tu; 
    FLOAT tv; 

	_XBILLBOARD_VERTEX(){}
	_XBILLBOARD_VERTEX( FLOAT _x, FLOAT _y, FLOAT _z, D3DCOLOR _color, D3DCOLOR _specular, FLOAT _tu, FLOAT _tv )
	{
		x = _x; y = _y; z = _z;
		color = _color; specular = _specular;
		tu = _tu; tv = _tv;
	}	
	
    ~_XBILLBOARD_VERTEX(){ }

} _XBILLBOARD_VERTEX, *_LPXBILLBOARD_VERTEX; 

class _XBillboard
{
public:
	_XBILLBOARD_VERTEX	VertexList[4];
	D3DXMATRIX			Matrix;
	D3DXVECTOR3			Position;
	FLOAT				TextureX[50][4];
	FLOAT				TextureY[50][4];
	
	WORD				NumberOfFrames;
	WORD				CurrentFrame;
	FLOAT				AnimationCounter;
	FLOAT				AnimationSpeed;

public:
	_XBillboard();
	_XBillboard(FLOAT Width,FLOAT Height,D3DXVECTOR3 pos);
	_XBillboard(FLOAT Width,FLOAT Height,D3DXVECTOR3 pos,WORD NumOfFrames,WORD TextureSizeX,WORD TextureSizeY,WORD cols,WORD rows,WORD animspeed);

	void SetBillBoard( FLOAT Width,FLOAT Height,D3DXVECTOR3 pos );
	void SetBillBoardXZ( FLOAT Width,FLOAT Height,D3DXVECTOR3 pos );
	void SetPosition( D3DXVECTOR3 newpos ){ Position = newpos; } 

    void Render(D3DXVECTOR3  ViewPosition);
	void Render(D3DXVECTOR3  ViewPosition, FLOAT zangle);
	void Render(D3DXMATRIX   tm );
	void RenderAlpha(D3DXMATRIX   tm );
    void Render(D3DXVECTOR3  ViewPosition,double TimeElapsed);   
    void RenderCheap( void );
    void RenderCheap(double TimeElapsed);   
    void RenderRestrictAxis( D3DXVECTOR3 ViewPosition,D3DXVECTOR3 Axis,BYTE WhichVector) ;
    void RenderRestrictAxis( D3DXVECTOR3 ViewPosition,D3DXVECTOR3 Axis,BYTE WhichVector,double TimeElapsed)  ;
	   
    void SetTextureCoords(WORD vertex,FLOAT x,FLOAT y);
    void SetTextureCoords(WORD vertex,WORD x,WORD y,WORD TexWidth,WORD TexHeight);
    void SetTextureCoords(WORD frame,WORD vertex,WORD x,WORD y,WORD TexWidth,WORD TexHeight);
    void SetTextureCoords(WORD frame,WORD vertex,FLOAT x,FLOAT y);
};


#endif


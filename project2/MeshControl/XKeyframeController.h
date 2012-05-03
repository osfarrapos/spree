// XKeyframeController.h: interface for the _XKeyframeController class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XKEYFRAMECONTROLLER_H__CA1A3983_B64E_4671_895F_60A9C18CA6F1__INCLUDED_)
#define AFX_XKEYFRAMECONTROLLER_H__CA1A3983_B64E_4671_895F_60A9C18CA6F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "XMeshContainer.h"
#include <vector>

#include "assert.h"

typedef struct _XM_SCALEKEY
{
	FLOAT			nTime;
	D3DXVECTOR3		d3dVKeyVal;			 // Poistion, Scale
	_XM_SCALEKEY(){ nTime = 0;	}
}_XM_SCALEKEY, *_LPXM_SCALEKEY;

typedef struct _XM_SUBNODE
{
	int						nPosKey;
	_LPXM_POSITIONKEY		lpPosKey;

	int						nRotKey;
	_LPXM_ROTATIONKEY		lpRotKey;	

	int						nScaleKey;
	_LPXM_SCALEKEY			lpScaleKey;	

	D3DXMATRIX				matTM;

	FLOAT					fMaxTime;
}_XM_SUBNODE, *_LPXM_SUBNODE;

typedef struct _XM_KEYFRAME
{
	char					strName[MAX_PATH];
	FLOAT					fMaxTime;

	_LPXM_SUBNODE			lpSubNode;

}_XM_KEYFRAME, *_LPXM_KEYFRAME;

class _XKeyframeController
{
public:
	std::vector<_XM_KEYFRAME*>		m_svKeyframe;
	_LPXM_KEYFRAME					m_lpCurKeyframe;

	int								m_nNodeCount;

public:
	_XKeyframeController();
	virtual ~_XKeyframeController();

	bool	LoadKey( LPSTR szFilename );
	bool	LoadKey(FILE* pFile);

	BOOL	LoadNode( FILE* pFile, _XM_SUBNODE* pMeshKey );

	BOOL	LoadNode_TM( FILE* pFile, _XM_SUBNODE* pSubNode );
	BOOL	LoadNode_PosKey( FILE* pFile, _XM_SUBNODE* pMeshKey );
	BOOL	LoadNode_RotKey( FILE* pFile, _XM_SUBNODE* pMeshKey );
	BOOL	LoadNode_ScaleKey( FILE* pFile, _XM_SUBNODE* pMeshKey );


	void	FrameAnimation(FLOAT fAniFrame, D3DXMATRIX matWorld, D3DXMATRIX* pmatFrame);


	inline int				GetNodeCnt( void ){ return m_nNodeCount; };

	inline int				GetMotionCnt( void ){ return m_svKeyframe.size(); };
	inline _LPXM_KEYFRAME	GetKeyframe(int index){ assert(index < m_svKeyframe.size()); return m_svKeyframe[index]; };
};

#endif // !defined(AFX_XKEYFRAMECONTROLLER_H__CA1A3983_B64E_4671_895F_60A9C18CA6F1__INCLUDED_)

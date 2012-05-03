
// XKeyframeController.cpp: implementation of the _XKeyframeController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKeyframeController.h"

#include "XKeyframeDescriptor.h"

#include <vector>


bool _XKeyframeController::LoadKey( LPSTR szFilename )
{
	FILE* pFile;
	pFile = fopen(szFilename, "rb");

	if (pFile == NULL)
	{
		return false;
	}

	LoadKey(pFile);

	fclose(pFile);

	return true;
}
bool _XKeyframeController::LoadKey(FILE* pFile)
{
//	CHAR ShortFilename[512];
//	memset( ShortFilename, 0, sizeof(TCHAR)*512 );
//	GetFileNameFromPathString( ShortFilename, szFilename );

	_LPXM_KEYFRAME lpKeyframe = new _XM_KEYFRAME;
//	strcpy(lpKeyframe->strName, ShortFilename);
	lpKeyframe->fMaxTime = 0.f;

	DWORD dwVersion;
	char cHeader[MAX_PATH];
	memset(cHeader, 0, MAX_PATH);

	fread(cHeader,		3,		sizeof(char),			pFile);
	fread(&dwVersion,	1,		sizeof(DWORD),			pFile);

	fread(&m_nNodeCount,	1,	sizeof(int),			pFile);

	lpKeyframe->lpSubNode = new _XM_SUBNODE[m_nNodeCount];
	memset(lpKeyframe->lpSubNode, 0, sizeof(_XM_SUBNODE) * m_nNodeCount);

	for (int i = 0; i < m_nNodeCount; ++i)
	{
		D3DXMatrixIdentity(&lpKeyframe->lpSubNode[i].matTM);

		LoadNode(pFile, &lpKeyframe->lpSubNode[i]);
		fread(&lpKeyframe->lpSubNode[i].fMaxTime,	1,	sizeof(float),		pFile);
		if (lpKeyframe->fMaxTime < lpKeyframe->lpSubNode[i].fMaxTime)
		{
			lpKeyframe->fMaxTime = lpKeyframe->lpSubNode[i].fMaxTime;
		}
	}

	m_lpCurKeyframe = lpKeyframe;
	m_svKeyframe.push_back(lpKeyframe);

	return true;
}

BOOL _XKeyframeController::LoadNode( FILE* pFile, _XM_SUBNODE* pSubNode )
{
	LoadNode_TM(pFile, pSubNode);
	LoadNode_PosKey(pFile, pSubNode);
	LoadNode_RotKey(pFile, pSubNode);
	LoadNode_ScaleKey(pFile, pSubNode);
	return TRUE;
}

BOOL _XKeyframeController::LoadNode_TM( FILE* pFile, _XM_SUBNODE* pSubNode )
{
	D3DXMATRIX matScale, matWorld;
	D3DXMatrixIdentity(&pSubNode->matTM);

	D3DXVECTOR3	tmPos = D3DXVECTOR3(0, 0, 0), tmScale = D3DXVECTOR3(1, 1, 1);
	D3DXQUATERNION tmRot;

	D3DXQuaternionIdentity(&tmRot);

	fread(&tmPos,	1,	sizeof(D3DXVECTOR3),			pFile);
	fread(&tmRot,	1,	sizeof(D3DXQUATERNION),			pFile);
	fread(&tmScale,	1,	sizeof(D3DXVECTOR3),			pFile);

	tmRot.x = sinf(tmRot.w/2.0f)*tmRot.x;
	tmRot.y = sinf(tmRot.w/2.0f)*tmRot.y;
	tmRot.z = sinf(tmRot.w/2.0f)*tmRot.z;
	tmRot.w = cosf(tmRot.w/2.0f);
	D3DXMatrixRotationQuaternion(&matWorld, &tmRot);

	matWorld._41 = tmPos.x;
	matWorld._42 = tmPos.y;
	matWorld._43 = tmPos.z;

	D3DXMatrixScaling(&matScale, tmScale.x, tmScale.y, tmScale.z);
	D3DXMatrixMultiply(&pSubNode->matTM, &matScale, &matWorld);

	return TRUE;
}

BOOL _XKeyframeController::LoadNode_PosKey( FILE* pFile, _XM_SUBNODE* pSubNode )
{
	fread(&pSubNode->nPosKey,	1,	sizeof(int),			pFile);
	if (pSubNode->nPosKey > 0)
	{
		pSubNode->lpPosKey	= new _XM_POSITIONKEY[pSubNode->nPosKey];
		fread(pSubNode->lpPosKey,	pSubNode->nPosKey,	sizeof(_XM_POSITIONKEY),			pFile);
	}
	return TRUE;
}

BOOL _XKeyframeController::LoadNode_RotKey( FILE* pFile, _XM_SUBNODE* pSubNode )
{
	fread(&pSubNode->nRotKey,	1,	sizeof(int),			pFile);
	if (pSubNode->nRotKey > 0)
	{
		pSubNode->lpRotKey	= new _XM_ROTATIONKEY[pSubNode->nRotKey];
		fread(pSubNode->lpRotKey,	pSubNode->nRotKey,	sizeof(_XM_ROTATIONKEY),			pFile);
	}
	return TRUE;
}

BOOL _XKeyframeController::LoadNode_ScaleKey( FILE* pFile, _XM_SUBNODE* pSubNode )
{
	fread(&pSubNode->nScaleKey,	1,	sizeof(int),			pFile);
	if (pSubNode->nScaleKey > 0)
	{
		pSubNode->lpScaleKey	= new _XM_SCALEKEY[pSubNode->nScaleKey];
		fread(pSubNode->lpScaleKey,	pSubNode->nScaleKey,	sizeof(_XM_SCALEKEY),			pFile);
	}
	return TRUE;
}

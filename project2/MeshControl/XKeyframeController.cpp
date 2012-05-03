// XKeyframeController.cpp: implementation of the _XKeyframeController class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKeyframeController.h"

#include <vector>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XKeyframeController::_XKeyframeController()
{
	m_lpCurKeyframe = NULL;
	m_nNodeCount = 0;
}

_XKeyframeController::~_XKeyframeController()
{
	std::vector<_XM_KEYFRAME*>::iterator it;
	for (it = m_svKeyframe.begin(); it != m_svKeyframe.end(); ++ it)
	{
		_LPXM_KEYFRAME lpCurKeyframe =  (*it);
		for (int i = 0; i < m_nNodeCount; ++ i)
		{
			SAFE_DELETE_ARRAY( lpCurKeyframe->lpSubNode[i].lpPosKey );
			SAFE_DELETE_ARRAY( lpCurKeyframe->lpSubNode[i].lpScaleKey )
			SAFE_DELETE_ARRAY( lpCurKeyframe->lpSubNode[i].lpRotKey )
		}
		SAFE_DELETE_ARRAY( lpCurKeyframe->lpSubNode );
		SAFE_DELETE( lpCurKeyframe );
	}
}

void _XKeyframeController::FrameAnimation(FLOAT fAniFrame, D3DXMATRIX matWorld, D3DXMATRIX* pmatFrame)
{
	if (m_lpCurKeyframe == NULL) return;

	if (fAniFrame > m_lpCurKeyframe->fMaxTime)
	{
		fAniFrame = 0.f;
	}

	float fLerpValue;
	D3DXVECTOR3 v3Position, posWorld;
	D3DXMATRIX matRotate, matScale, matRender;
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixIdentity(&matRotate);
	D3DXQUATERNION quatRotate; 

	posWorld.x = matWorld._41;
	posWorld.y = matWorld._42;
	posWorld.z = matWorld._43;

	matWorld._41 = 0;
	matWorld._42 = 0;
	matWorld._43 = 0;


	for(int nodeindex = 0; nodeindex < m_nNodeCount; ++nodeindex)
	{
		_LPXM_SUBNODE lpSubNode = &m_lpCurKeyframe->lpSubNode[nodeindex];

		//check rot keys
		if (lpSubNode->lpRotKey != NULL)
		{
			// 최대 키값을 넘으면 마지막 값을 넣어준다.
			if(fAniFrame >= lpSubNode->lpRotKey[lpSubNode->nRotKey - 1].nTime)
			{
				D3DXMatrixRotationQuaternion(&matRotate, &lpSubNode->lpRotKey[lpSubNode->nRotKey - 1].Qt);
			}
			else
			{
				int i1 = 0, i2 = 0;

				for (int iKey = 0; iKey < lpSubNode->nRotKey; ++iKey)
				{
					if (lpSubNode->lpRotKey[iKey].nTime > fAniFrame)
					{
						i1 = (iKey > 0) ? iKey - 1 : 0;
						i2 = iKey;
						break;
					}
				}

				if (lpSubNode->lpRotKey[i1].nTime == lpSubNode->lpRotKey[i2].nTime)
				{
					fLerpValue = 0.f;
					quatRotate = lpSubNode->lpRotKey[i1].Qt;
				}
				else
				{
					fLerpValue = (float)(fAniFrame - lpSubNode->lpRotKey[i1].nTime) / (float)(lpSubNode->lpRotKey[i2].nTime - lpSubNode->lpRotKey[i1].nTime);
					D3DXQuaternionSlerp((D3DXQUATERNION*)&quatRotate, 
										(D3DXQUATERNION*)&lpSubNode->lpRotKey[i1].Qt, 
										(D3DXQUATERNION*)&lpSubNode->lpRotKey[i2].Qt, 
										fLerpValue);
				}
				D3DXMatrixRotationQuaternion(&matRotate, &quatRotate);
			}
		}
		else
		{
			D3DXMatrixIdentity(&matRotate);
		}


		if (lpSubNode->lpPosKey != NULL)
		{
			if(fAniFrame >= lpSubNode->lpPosKey[lpSubNode->nPosKey - 1].nTime)
			{
				v3Position = lpSubNode->lpPosKey[lpSubNode->nPosKey - 1].d3dVKeyVal;
			}
			else
			{
				int i1 = 0, i2 = 0;

				for (int iKey = 0; iKey < lpSubNode->nPosKey; ++iKey)
				{
					if (lpSubNode->lpPosKey[iKey].nTime > fAniFrame)
					{
						i1 = (iKey > 0) ? iKey - 1 : 0;
						i2 = iKey;
						break;
					}
				}

				if (lpSubNode->lpPosKey[i1].nTime == lpSubNode->lpPosKey[i2].nTime)
				{
					fLerpValue = 0.f;
					v3Position = lpSubNode->lpPosKey[i1].d3dVKeyVal;
				}
				else
				{
					fLerpValue = (float)(fAniFrame - lpSubNode->lpPosKey[i1].nTime)  / (float)(lpSubNode->lpPosKey[i2].nTime - lpSubNode->lpPosKey[i1].nTime);

					v3Position.x = lpSubNode->lpPosKey[i1].d3dVKeyVal.x + fLerpValue * (lpSubNode->lpPosKey[i2].d3dVKeyVal.x - lpSubNode->lpPosKey[i1].d3dVKeyVal.x);
					v3Position.y = lpSubNode->lpPosKey[i1].d3dVKeyVal.y + fLerpValue * (lpSubNode->lpPosKey[i2].d3dVKeyVal.y - lpSubNode->lpPosKey[i1].d3dVKeyVal.y);
					v3Position.z = lpSubNode->lpPosKey[i1].d3dVKeyVal.z + fLerpValue * (lpSubNode->lpPosKey[i2].d3dVKeyVal.z - lpSubNode->lpPosKey[i1].d3dVKeyVal.z);
				}
			}
		}
		else
		{
			v3Position.x = 0.f;
			v3Position.y = 0.f;
			v3Position.z = 0.f;
		}
		if (lpSubNode->lpScaleKey != NULL)
		{
			if(fAniFrame >= lpSubNode->lpScaleKey[lpSubNode->nScaleKey - 1].nTime)
			{
				matScale._11 = lpSubNode->lpScaleKey[lpSubNode->nScaleKey - 1].d3dVKeyVal.x;
				matScale._22 = lpSubNode->lpScaleKey[lpSubNode->nScaleKey - 1].d3dVKeyVal.y;
				matScale._33 = lpSubNode->lpScaleKey[lpSubNode->nScaleKey - 1].d3dVKeyVal.z;
			}
			else
			{
				int i1 = 0, i2 = 0;

				for (int iKey = 0; iKey < lpSubNode->nScaleKey; ++iKey)
				{
					if (lpSubNode->lpScaleKey[iKey].nTime > fAniFrame)
					{
						i1 = (iKey > 0) ? iKey - 1 : 0;
						i2 = iKey;
						break;
					}
				}

				if (lpSubNode->lpScaleKey[i1].nTime == lpSubNode->lpScaleKey[i2].nTime)
				{
					fLerpValue = 0.f;
					matScale._11 = lpSubNode->lpScaleKey[i1].d3dVKeyVal.x;
					matScale._22 = lpSubNode->lpScaleKey[i1].d3dVKeyVal.y;
					matScale._33 = lpSubNode->lpScaleKey[i1].d3dVKeyVal.z;
				}
				else
				{
					fLerpValue = (float)(fAniFrame - lpSubNode->lpScaleKey[i1].nTime)  / (float)(lpSubNode->lpScaleKey[i2].nTime - lpSubNode->lpScaleKey[i1].nTime);

					matScale._11 = lpSubNode->lpScaleKey[i1].d3dVKeyVal.x + fLerpValue * (lpSubNode->lpScaleKey[i2].d3dVKeyVal.x - lpSubNode->lpScaleKey[i1].d3dVKeyVal.x);
					matScale._22 = lpSubNode->lpScaleKey[i1].d3dVKeyVal.y + fLerpValue * (lpSubNode->lpScaleKey[i2].d3dVKeyVal.y - lpSubNode->lpScaleKey[i1].d3dVKeyVal.y);
					matScale._33 = lpSubNode->lpScaleKey[i1].d3dVKeyVal.z + fLerpValue * (lpSubNode->lpScaleKey[i2].d3dVKeyVal.z - lpSubNode->lpScaleKey[i1].d3dVKeyVal.z);
				}
			}
		}
		else
		{
			matScale._11 = 1.f;
			matScale._22 = 1.f;
			matScale._33 = 1.f;
		}

		D3DXMatrixMultiply(&matRender, &matRotate, &matWorld);
		D3DXMatrixMultiply(&pmatFrame[nodeindex], &matScale, &matRender);
		pmatFrame[nodeindex]._41 = v3Position.x + posWorld.x;
		pmatFrame[nodeindex]._42 = v3Position.y + posWorld.y;
		pmatFrame[nodeindex]._43 = v3Position.z + posWorld.z;
	}
}

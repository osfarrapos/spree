///////////////////////////////////////////////////////////////////////  
//	SpeedWindBlend.cpp
//
//	(c) 2004 IDV, Inc.
//
//	This class blends SpeedWind objects into a single set of wind outputs.
//
//
//	*** INTERACTIVE DATA VISUALIZATION (IDV) PROPRIETARY INFORMATION ***
//
//	This software is supplied under the terms of a license agreement or
//	nondisclosure agreement with Interactive Data Visualization and may
//	not be copied or disclosed except in accordance with the terms of
//	that agreement.
//
//      Copyright (c) 2001-2004 IDV, Inc.
//      All Rights Reserved.
//
//		IDV, Inc.
//		1233 Washington St. Suite 610
//		Columbia, SC 29201
//		Voice: (803) 799-1699
//		Fax:   (803) 931-0320
//		Web:   http://www.idvinc.com


/////////////////////////////////////////////////////////////////////////////
// Preprocessor

#include "stdafx.h"
#include "SpeedWindBlend.h"
using namespace std;

#define	INTERPOLATE(a, b, p)		(((b) - (a)) * (p) + (a))


/////////////////////////////////////////////////////////////////////////////
// Constants

static	const	float	c_fPi = 3.14159265358979323846f;
static	const	float	c_fHalfPi = c_fPi * 0.5f;
static	const	float	c_fQuarterPi = c_fPi * 0.25f;
static	const	float	c_fTwoPi = 2.0f * c_fPi;
static	const	float	c_fRad2Deg = 57.29578f;


/////////////////////////////////////////////////////////////////////////////
// _XSpeedWindBlend::_XSpeedWindBlend

_XSpeedWindBlend::_XSpeedWindBlend(void) :
	m_uiNumWindMatrices(0),
	m_pWindMatrices(NULL),
	m_uiNumLeafAngles(0),
	m_pLeafAngleMatrices(NULL)
{
	for (unsigned int i = 0; i < _XSpeedWind::NUM_LEAF_ANGLES; ++i)
		m_pLeafAngles[i] = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// _XSpeedWindBlend::~_XSpeedWindBlend

_XSpeedWindBlend::~_XSpeedWindBlend(void)
{
	delete[] m_pWindMatrices;
	for (unsigned int i = 0; i < _XSpeedWind::NUM_LEAF_ANGLES; ++i)
		delete[] m_pLeafAngles[i];

	delete[] m_pLeafAngleMatrices;
}


/////////////////////////////////////////////////////////////////////////////
// _XSpeedWindBlend::CreateMatricesAndRockAngles

void _XSpeedWindBlend::CreateMatricesAndRockAngles(void)
{
	// count matrices and rock angles
	if (m_vSpeedWinds.size( ) > 0)
	{
		m_uiNumWindMatrices = m_vSpeedWinds[0]->GetNumWindMatrices( );
		m_uiNumLeafAngles = m_vSpeedWinds[0]->GetNumLeafAngles( );
		for (unsigned int i = 1; i < m_vSpeedWinds.size( ); ++i)
		{
			m_uiNumWindMatrices = min(m_vSpeedWinds[i]->GetNumWindMatrices( ), m_uiNumWindMatrices);
			m_uiNumLeafAngles = min(m_vSpeedWinds[i]->GetNumLeafAngles( ), m_uiNumLeafAngles);
		}
	}
	else
	{
		m_uiNumWindMatrices = 0;
		m_uiNumLeafAngles = 0;
	}

	delete[] m_pWindMatrices;
	m_pWindMatrices = new _XSpeedWindMatrix[m_uiNumWindMatrices];

	for (unsigned int i = 0; i < _XSpeedWind::NUM_LEAF_ANGLES; ++i)
	{
		delete[] m_pLeafAngles[i];
		m_pLeafAngles[i] = new float[m_uiNumLeafAngles];
	}

	delete[] m_pLeafAngleMatrices;
	m_pLeafAngleMatrices = new _XSpeedWindMatrix[m_uiNumLeafAngles];
}


/////////////////////////////////////////////////////////////////////////////
// _XSpeedWindBlend::Advance

void _XSpeedWindBlend::Advance(void)
{
	// matrices
	for (unsigned int i = 0; i < m_uiNumWindMatrices; ++i)
	{
		// computed weighted average of all inputs
		float fStrength = 0.0f;
		float fXVibration = 0.0f;
		float fYVibration = 0.0f;
		float fWeightSum = 0.0f;
		float fAngleX = 0.0f;
		float fAngleY = 0.0f;

		for (unsigned int j = 0; j < m_vSpeedWinds.size( ); ++j)
		{
			_XSpeedWind* pWind = m_vSpeedWinds[j];
	
			fWeightSum += pWind->m_fBlendWeight;
			fStrength += pWind->m_vWindMatrices[i].m_fFinalStrength * pWind->m_fBlendWeight;
			float fTempAngle = pWind->m_vWindMatrices[i].m_fFinalAngle;
			fAngleX += cosf(fTempAngle) * pWind->m_fBlendWeight;
			fAngleY += sinf(fTempAngle) * pWind->m_fBlendWeight;
			fXVibration += static_cast<float>(pWind->m_vWindMatrices[i].m_cXVibration.GetValue( )) * pWind->m_fBlendWeight;
			fYVibration += static_cast<float>(pWind->m_vWindMatrices[i].m_cYVibration.GetValue( )) * pWind->m_fBlendWeight;
		}
		fStrength /= fWeightSum;
		fXVibration /= fWeightSum;
		fYVibration /= fWeightSum;
		fAngleX /= fWeightSum;
		fAngleY /= fWeightSum;

		// compute the new rotation matrix
		m_pWindMatrices[i].RotateAxis(fStrength, fAngleX, fAngleY, 0.0f);
		m_pWindMatrices[i].Rotate(fXVibration, 'x');
		m_pWindMatrices[i].Rotate(fYVibration, 'y');
	}

	// leaf angles
	for (i = 0; i < m_uiNumLeafAngles; ++i)
	{
		// computed weighted average of all inputs
		float fAngles[_XSpeedWind::NUM_LEAF_ANGLES];
		fAngles[0] = fAngles[1] = 0.0f;
		float fWeightSum = 0.0f;
		for (unsigned int j = 0; j < m_vSpeedWinds.size( ); ++j)
		{
			_XSpeedWind* pWind = m_vSpeedWinds[j];
			fWeightSum += pWind->m_fBlendWeight;

			for (unsigned int k = 0; k < _XSpeedWind::NUM_LEAF_ANGLES; ++k)
				fAngles[k] += pWind->m_pLeafAngles[k][i] * pWind->m_fBlendWeight;
		}

		for (unsigned int k = 0; k < _XSpeedWind::NUM_LEAF_ANGLES; ++k)
			m_pLeafAngles[k][i] = fAngles[k] / fWeightSum;
	}
}


/////////////////////////////////////////////////////////////////////////////
// _XSpeedWindBlend::BuildLeafAngleMatrices

void _XSpeedWindBlend::BuildLeafAngleMatrices(const float* pCameraDir)
{
    float fAzimuth = atan2f(pCameraDir[1], pCameraDir[0]) * c_fRad2Deg;
    float fPitch = -asinf(pCameraDir[2]) * c_fRad2Deg;

	for (unsigned int i = 0; i < m_uiNumLeafAngles; ++i)
	{
		_XSpeedWindMatrix& cMatrix = m_pLeafAngleMatrices[i];

		cMatrix.LoadIdentity( );
#ifdef SPEEDWIND_UPVECTOR_POS_Z
		cMatrix.Rotate(fAzimuth, 'z');
		cMatrix.Rotate(fPitch, 'y');
		cMatrix.Rotate(m_pLeafAngles[_XSpeedWind::RUSTLE][i], 'z');
		cMatrix.Rotate(m_pLeafAngles[_XSpeedWind::ROCK][i], 'x');
#endif
#ifdef SPEEDWIND_UPVECTOR_NEG_Z
		cMatrix.Rotate(-fAzimuth, 'z');
		cMatrix.Rotate(fPitch, 'y');
		cMatrix.Rotate(m_pLeafAngles[_XSpeedWind::RUSTLE][i], 'z');
		cMatrix.Rotate(m_pLeafAngles[_XSpeedWind::ROCK][i], 'x');
#endif
#ifdef SPEEDWIND_UPVECTOR_POS_Y
		cMatrix.Rotate(fAzimuth, 'y');
		cMatrix.Rotate(fPitch, 'z');
		cMatrix.Rotate(-m_pLeafAngles[_XSpeedWind::RUSTLE][i], 'y');
		cMatrix.Rotate(m_pLeafAngles[_XSpeedWind::ROCK][i], 'x');
#endif
#ifdef SPEEDWIND_UPVECTOR_DIRECTX_RIGHT_HANDED_COORDINATE_SYSTEM
		cMatrix.Rotate(-fAzimuth, 'z');
		cMatrix.Rotate(-fPitch, 'x');
		cMatrix.Rotate(m_pLeafAngles[_XSpeedWind::RUSTLE][i], 'z');
		cMatrix.Rotate(m_pLeafAngles[_XSpeedWind::ROCK][i], 'y');
#endif
	}
}


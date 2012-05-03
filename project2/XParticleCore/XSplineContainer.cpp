// XSplineContainer.cpp: implementation of the _XSplineContainer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XSplineContainer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XSplineContainer::_XSplineContainer()
{
	m_CurveType			= 0;
	m_ShortestDistance	= 0;
	
	m_pControlData		= NULL;	
	m_ControlCount		= 0;
	m_ControlIndx		= 0;

	m_DefualtNormal		= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
		
	m_pCurveData		= NULL;
	m_CurveSubDivision	= 0;
	m_TotalCurveCount	= 0;	
	m_pSplineName		= NULL;
	SetPathName( "Noname" );
}

_XSplineContainer::~_XSplineContainer()
{
	SAFE_DELETE_ARRAY( m_pControlData );	
	SAFE_DELETE_ARRAY( m_pCurveData );
	SAFE_DELETE_ARRAY( m_pSplineName );
}

void _XSplineContainer::SetPathName( LPCTSTR _splinename )
{
	if( !_splinename ) return;
	SAFE_DELETE_ARRAY(  m_pSplineName );
	int size = strlen( _splinename );
	
	m_pSplineName = new TCHAR[size+1];
	memset( m_pSplineName, 0, size+1 );
	memcpy( m_pSplineName, _splinename, size );
}

void _XSplineContainer::Build(D3DXVECTOR3 defualtnormal)
{
	m_DefualtNormal = defualtnormal;
	Build();
}

void _XSplineContainer::BuildControlPoint( int type, D3DXVECTOR3 forwardfactor, int rotatecount, FLOAT radius )
{
	if( !m_pControlData ) return;

	if( type == 0 ) // Line
	{
		m_pControlData[0] = D3DXVECTOR3( 0.0f, 0.000001f, 0.0f );

		for( int i = 1; i < m_ControlCount; i++ )
		{
			m_pControlData[i] = m_pControlData[i-1] + forwardfactor;
		}
	}
	else if( type == 1 ) // Circle
	{
		D3DXVECTOR3 vPoints;
		DWORD i;
		D3DXMATRIX mRot;
				
		vPoints.x = 0.0f;	
		vPoints.y = 0.00001f;	
		vPoints.z = radius;	
		SetControlData( 0, vPoints );
		
		D3DXMatrixRotationY( &mRot, 6.283185307178f / (FLOAT)(m_ControlCount-3) );
		
		for( i=1; i < m_ControlCount; i++ )
		{
			D3DXVECTOR4 result; 		
			D3DXVec3Transform( &result, &vPoints, &mRot );
			
			vPoints.x = result.x;
			vPoints.y = result.y;
			vPoints.z = result.z;
			
			SetControlData( i, vPoints );
		}
	}
	else if( type == 2 ) // Herix
	{
		D3DXVECTOR3 vPoints;
		DWORD i;
		D3DXMATRIX mRot;
		
		vPoints.x = 0.0f;	
		vPoints.y = 0.000001f;	
		vPoints.z = radius;	
		SetControlData( 0, vPoints );
		
		D3DXMatrixRotationY( &mRot, 6.283185307178f / ((FLOAT)(( m_ControlCount/rotatecount)-3)) );
		
		for( i=1; i < m_ControlCount; i++ )
		{
			D3DXVECTOR4 result;
			D3DXVec3Transform( &result, &vPoints, &mRot );
						
			vPoints.x = result.x + forwardfactor.x;
			vPoints.y = result.y + forwardfactor.y;
			vPoints.z = result.z + forwardfactor.z;
			
			SetControlData( i, vPoints );
		}
	}
	else if( type == 3 ) // Sine curve
	{
		m_pControlData[0] = D3DXVECTOR3( 0.0f, 0.000001f, 0.0f );
		
		BOOL signflag = TRUE;

		for( int i = 1; i < m_ControlCount; i++ )
		{
			m_pControlData[i] = m_pControlData[0] + (forwardfactor * i);
			
			if( signflag )
			{
				m_pControlData[i].x  += radius;
				signflag = FALSE;
			}
			else
			{
				m_pControlData[i].x  -= radius;
				signflag = TRUE;
			}
		}
	}
}


BOOL _XSplineContainer::ReserveControlData( int controlcount, int curvesubdivision )
{
	SAFE_DELETE_ARRAY( m_pControlData );	
	SAFE_DELETE_ARRAY( m_pCurveData );

	m_ControlCount = controlcount;
	m_CurveSubDivision = curvesubdivision;

	m_pControlData = new D3DXVECTOR3[ m_ControlCount ];
	if( !m_pControlData )
	{
		_XFatalError( "ERROR : Can't create the spline control buffer" );
		m_ControlCount = 0;
		m_CurveSubDivision = 0;
		return FALSE;
	}
	memset(m_pControlData, 0, sizeof(D3DXVECTOR3) * m_ControlCount );
			
	// Alloc Curve Points
	m_TotalCurveCount = m_ControlCount * m_CurveSubDivision;
	m_pCurveData = new _XSplineCurveData[ m_TotalCurveCount ];
	if( !m_pCurveData )
	{
		_XFatalError( "ERROR : Can't create the curve data buffer" );
		SAFE_DELETE_ARRAY( m_pControlData );		
		m_ControlCount		= 0;
		m_CurveSubDivision	= 0;
		m_TotalCurveCount	= 0;
		return FALSE;
	}

	memset(m_pCurveData, 0, sizeof(_XSplineCurveData) * m_TotalCurveCount );

	return TRUE;
}

void _XSplineContainer::SetControlData( int bufferindex, const D3DXVECTOR3& pos )
{
	if( !m_pControlData || bufferindex >= m_ControlCount ) return;	
	m_pControlData[bufferindex] = pos;
}

void _XSplineContainer::ReBuildCurve( void )
{
	SAFE_DELETE_ARRAY( m_pCurveData );
	// Alloc Curve Points
	m_TotalCurveCount = m_ControlCount * m_CurveSubDivision;
	m_pCurveData = new _XSplineCurveData[ m_TotalCurveCount ];
	if( !m_pCurveData )
	{
		_XFatalError( "ERROR : Can't create the curve data buffer" );
		SAFE_DELETE_ARRAY( m_pControlData );
		m_ControlCount		= 0;
		m_CurveSubDivision	= 0;
		m_TotalCurveCount	= 0;
		return;
	}
	
	memset(m_pCurveData, 0, sizeof(_XSplineCurveData) * m_TotalCurveCount );

	Build();
}

void _XSplineContainer::Build( void )
{
	if( !m_pControlData || !m_pCurveData ) return;

	FLOAT		u, u_2, u_3;
	int			i, j;
	int			index;
	D3DXVECTOR3 distVec;
	D3DXVECTOR3 origin(0.0, 0.0, 0.0);
	D3DXMATRIX	tempMtx;

	index = 0;
	// For each control Point (Minus the last three)
	for (i = 0; i < m_ControlCount - 3; i++) 
	{
		// For each subdivision
		for(j = 0; j < m_CurveSubDivision; j++) 
		{
			u = (FLOAT)j / m_CurveSubDivision;
			u_2 = u * u;
			u_3 = u_2 * u;

			if (m_CurveType == 0) 
			{
				// Position
				m_pCurveData[index].pos.x = _XSL_B_SPLINE(u, u_2, u_3,	m_pControlData[i  ].x,m_pControlData[i+1].x,
																		m_pControlData[i+2].x,m_pControlData[i+3].x);
				m_pCurveData[index].pos.y = _XSL_B_SPLINE(u, u_2, u_3,	m_pControlData[i  ].y,m_pControlData[i+1].y,
																		m_pControlData[i+2].y,m_pControlData[i+3].y);
				m_pCurveData[index].pos.z = _XSL_B_SPLINE(u, u_2, u_3,	m_pControlData[i  ].z,m_pControlData[i+1].z,
																		m_pControlData[i+2].z,m_pControlData[i+3].z);
				// Tangent
				m_pCurveData[index].tan.x = _XSL_B_TANGENT(u, u_2,		m_pControlData[i  ].x,m_pControlData[i+1].x,
																		m_pControlData[i+2].x,m_pControlData[i+3].x);
				m_pCurveData[index].tan.y = _XSL_B_TANGENT(u, u_2,		m_pControlData[i  ].y,m_pControlData[i+1].y,
																		m_pControlData[i+2].y,m_pControlData[i+3].y);
				m_pCurveData[index].tan.z = _XSL_B_TANGENT(u, u_2,		m_pControlData[i  ].z,m_pControlData[i+1].z,
																		m_pControlData[i+2].z,m_pControlData[i+3].z);
				D3DXVec3Normalize( &m_pCurveData[index].tan, &m_pCurveData[index].tan );
			}
			else 
			{
				m_pCurveData[index].pos.x = _XSL_CATMULL_ROM_SPLINE(u, u_2, u_3,m_pControlData[i  ].x,m_pControlData[i+1].x,
																				m_pControlData[i+2].x,m_pControlData[i+3].x);
				m_pCurveData[index].pos.y = _XSL_CATMULL_ROM_SPLINE(u, u_2, u_3,m_pControlData[i  ].y,m_pControlData[i+1].y,
																				m_pControlData[i+2].y,m_pControlData[i+3].y);
				m_pCurveData[index].pos.z = _XSL_CATMULL_ROM_SPLINE(u, u_2, u_3,m_pControlData[i  ].z,m_pControlData[i+1].z,
																				m_pControlData[i+2].z,m_pControlData[i+3].z);
				// Tangent
				m_pCurveData[index].tan.x = _XSL_CATMULL_ROM_TANGENT(u, u_2,	m_pControlData[i  ].x,m_pControlData[i+1].x,
																				m_pControlData[i+2].x,m_pControlData[i+3].x);
				m_pCurveData[index].tan.y = _XSL_CATMULL_ROM_TANGENT(u, u_2,	m_pControlData[i  ].y,m_pControlData[i+1].y,
																				m_pControlData[i+2].y,m_pControlData[i+3].y);
				m_pCurveData[index].tan.z = _XSL_CATMULL_ROM_TANGENT(u, u_2,	m_pControlData[i  ].z,m_pControlData[i+1].z,
																				m_pControlData[i+2].z,m_pControlData[i+3].z);
				D3DXVec3Normalize( &m_pCurveData[index].tan, &m_pCurveData[index].tan );
			}

			// Normal
			if (j == 0 && i == 0) 
			{
				m_pCurveData[0].nrm = m_DefualtNormal;
				D3DXVec3Cross( &m_pCurveData[0].sid, &m_pCurveData[0].nrm, &m_pCurveData[0].tan );
			}
			else 
			{
				// Cross two tangents To get a side vector
				D3DXVec3Cross( &m_pCurveData[index].sid, &m_pCurveData[index - 1].tan, &m_pCurveData[index].tan );
				D3DXVec3Normalize( &m_pCurveData[index].sid, &m_pCurveData[index].sid );
								
				// Make sure there are no Zero vectors
				if ( (m_pCurveData[index].sid.x == 0.0f) && (m_pCurveData[index].sid.y == 0.0f) && (m_pCurveData[index].sid.z == 0.0f) )
					  m_pCurveData[index - 1].sid = m_pCurveData[index].sid;
				// Check for Vector Flip				
				if ( D3DXVec3Dot( &m_pCurveData[index - 1].sid, &m_pCurveData[index].sid ) < 0.0f )
				{
					m_pCurveData[index].sid *= -1.0f;
				}
				// Cross side vector and tangent to get the normal
				D3DXVec3Cross( &m_pCurveData[index].nrm, &m_pCurveData[index].tan, &m_pCurveData[index].sid);
				// Normalize
				D3DXVec3Normalize( &m_pCurveData[index].nrm, &m_pCurveData[index].nrm );
			}

			// Distance
			if (j == 0 && i == 0) 
			{
				m_pCurveData[index].distance = 0;
				m_ShortestDistance = 65536.0f;
			}
			else
			{
				distVec = m_pCurveData[index].pos - m_pCurveData[index - 1].pos;
				m_pCurveData[index].distance = m_pCurveData[index - 1].distance + D3DXVec3Length( &distVec );
				if (m_pCurveData[index].distance < m_ShortestDistance) m_ShortestDistance = m_pCurveData[index].distance;
			}
								
			index++;
		}
	}
	
	m_TotalCurveCount = index;
}


BOOL _XSplineContainer::Load( FILE* pFilePtr )
{
	if( !pFilePtr ) return FALSE;

	// Save spline name
	int length = 0;
	if( fread( &length, sizeof(int), 1, pFilePtr ) < 1 )
	{
		_XFatalError( "Can't read the spline name length" );
	}
	
	if( length > 0 )
	{
		TCHAR namebuffer[256];
		memset( namebuffer, 0, sizeof(TCHAR)*256 );
		if( fread( namebuffer, sizeof(TCHAR)*length, 1, pFilePtr ) < 1 )
		{
			_XFatalError( "Can't read the spline name" );
			return FALSE;
		}

		SetPathName( namebuffer );
	}
	
	if( fread( &m_CurveType, sizeof(int), 1, pFilePtr ) < 1 )
	{
		_XFatalError( "Can't read the spline curve type" );
		return FALSE;
	}
	
	int controlcount = 0;
	if( fread( &controlcount, sizeof(int), 1, pFilePtr ) < 1 )
	{
		_XFatalError( "Can't read the spline control count" );
		return FALSE;
	}	

	int curvedivision = 0;
	if( fread( &curvedivision, sizeof(int), 1, pFilePtr ) < 1 )
	{
		_XFatalError( "Can't read the spline curve division count" );
		return FALSE;
	}	

	if( !ReserveControlData( controlcount, curvedivision ) )
	{		
		return FALSE;
	}
	
	D3DXVECTOR3 pos;
	for( int i = 0; i < controlcount; i++ )
	{
		if( fread( &m_pControlData[i], sizeof(D3DXVECTOR3), 1, pFilePtr ) < 1 )
		{
			_XFatalError( "Can't read the spline control point" );
			return FALSE;
		}
	}

	Build();	

	return TRUE;
}

BOOL _XSplineContainer::Save( FILE* pFilePtr )
{
	if( !pFilePtr ) return FALSE;

	// Save spline name
	int length = strlen( m_pSplineName );
	if( fwrite( &length, sizeof(int), 1, pFilePtr ) < 1 )
	{
		_XFatalError( "Can't write spline name length" );
	}
	
	if( length > 0 )
	{
		if( fwrite( m_pSplineName, sizeof(TCHAR)*length, 1, pFilePtr ) < 1 )
		{
			_XFatalError( "Can't write spline name" );
		}
	}
	
	if( fwrite( &m_CurveType, sizeof(int), 1, pFilePtr ) < 1 )
	{
		_XFatalError( "Can't write spline curve type" );
	}
	
	if( fwrite( &m_ControlCount, sizeof(int), 1, pFilePtr ) < 1 )
	{
		_XFatalError( "Can't write spline control count" );
	}	

	if( fwrite( &m_CurveSubDivision, sizeof(int), 1, pFilePtr ) < 1 )
	{
		_XFatalError( "Can't write spline curve division count" );
	}	

	if( m_pControlData )
	{
		for( int i = 0; i < m_ControlCount; i++ )
		{
			if( fwrite( &m_pControlData[i], sizeof(D3DXVECTOR3), 1, pFilePtr ) < 1 )
			{
				_XFatalError( "Can't write spline curve type" );
			}
		}
	}

	return TRUE;
}

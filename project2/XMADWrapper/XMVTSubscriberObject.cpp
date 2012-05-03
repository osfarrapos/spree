// XMVTSubscriberObject.cpp: implementation of the _XMVTSubscriberObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _XDEF_MADSYSTEM

#include "XMVTSubscriberObject.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Massive subscriber object

using namespace MassiveAdClient3;

_XMVTSubscriberObject::_XMVTSubscriberObject( LPTSTR nameToUse ) : CMassiveAdObjectSubscriber(nameToUse)
{
	m_iCrexID			= 0;
	m_iInventoryID		= 0;	
	m_bInView			= false;
	m_bHasTriedToCommit = false;
	m_bCached			= false;
	
	m_pTexture			= NULL;

	m_ImageWidth		= 0;
	m_ImageHeight		= 0;	

	m_fSize				= 0.0f;
	m_fAngle			= 1.0f;
}

_XMVTSubscriberObject::~_XMVTSubscriberObject()
{
	SAFE_RELEASE(m_pTexture);
}



UINT _XMVTSubscriberObject::Render( void )
{

	return 0;
}

UINT _XMVTSubscriberObject::Update( void )
{
	
	return 0;
}

BOOL _XMVTSubscriberObject::MediaDownload( UINT uCrexID )
{
	// Are we caching?
	if (m_bCached)
		return false;
	else
		return true;
	
	return 0;
}

BOOL _XMVTSubscriberObject::MediaDownloadComplete( LPCTSTR MediaData, const UINT uDataLength, const UINT uMediaType, const UINT	uCrexID )
{
	SAFE_RELEASE( m_pTexture );
	
	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, (LPCVOID)MediaData, uDataLength, 
													 D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
													 D3DX_FILTER_NONE,
													 D3DX_FILTER_NONE,
													 0, NULL, NULL, &m_pTexture ) ) )
	{
		// Clear crexID when the new image was not sucessfully built
		m_iCrexID = 0;			

		_XFatalError( "_XMVTSubscriberObject::Could not create texture" );
		return FALSE;
	}

	D3DSURFACE_DESC surfdesc;
	m_pTexture->GetLevelDesc( 0, &surfdesc );
	m_ImageWidth  = surfdesc.Width;
	m_ImageHeight = surfdesc.Height;	

	m_fSize				= _XFC_sqrt( m_ImageWidth*m_ImageWidth + m_ImageHeight*m_ImageHeight );

	m_iCrexID = uCrexID;	// Store crexID locally

	_XLog( "_XMVTSubscriberObject::Media download complete" );

	return TRUE;
}

void _XMVTSubscriberObject::Tick( void )
{
	// Massive Impression Structure
	MAD_Impression newImp;
	memset(&newImp,0,sizeof(MAD_Impression));
	
	// Determine View/Impression 
	//CalculateMetrics(); 
	
	newImp.bInView = m_bInView ? true : false;
	newImp.siSize  = m_fSize;
	newImp.fAngle  = m_fAngle;
	newImp.siScreenHeight = gnHeight;
	newImp.siScreenWidth  = gnWidth;
	
	// Update the MassiveAdObjectSubscriber with the Impression Data
	SetImpression(&newImp);	
}

BOOL _XMVTSubscriberObject::InitDeviceObject( void )
{

	return TRUE;
}

BOOL _XMVTSubscriberObject::RestoreDeviceObject( void )
{

	return TRUE;
}

void _XMVTSubscriberObject::ReleaseDeviceObject( void )
{
	//SAFE_RELEASE(m_pTexture);
}

#endif
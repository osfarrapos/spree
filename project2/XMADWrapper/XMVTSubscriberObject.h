// XMVTSubscriberObject.h: interface for the _XMVTSubscriberObject class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMVTSUBSCRIBEROBJECT_H_
#define _XMVTSUBSCRIBEROBJECT_H_

#ifdef _XDEF_MADSYSTEM

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XKernel.h"
#include "MassiveAdObjectSubscriber.h"

namespace MassiveAdClient3
{

class _XMVTSubscriberObject : public CMassiveAdObjectSubscriber
{

public:
	BOOL				m_bHasTriedToCommit;	
	BOOL				m_bInView;				// Is this object in view
	BOOL				m_bCached;				// Is this object being cached
	
	WCHAR				m_wszDisplayName[64];

	// Rotation, Translation, Scaling
	D3DXMATRIX			m_ModelMat;
	D3DXMATRIX			m_ViewMat;
	
	D3DXCOLOR			m_VertColor;
	D3DXVECTOR3			m_Normal;				// Normal of the ad object
	
	// Massive Info
	FLOAT				m_fSize;				// Size on screen
	FLOAT				m_fAngle;				// Dot product
		
	// Crex & Inventory ID
	UINT				m_iCrexID;
	UINT				m_iInventoryID;

	int					m_ImageWidth;
	int					m_ImageHeight;
	LPDIRECT3DTEXTURE9	m_pTexture;				// Texture data in graphic memory	
	
public:
	// Construction
	_XMVTSubscriberObject(LPTSTR nameToUse);
	~_XMVTSubscriberObject();
	
	// Loop
	UINT				Render( void );
	UINT				Update( void );
		
	// Accessors
	FLOAT				GetSize( void )  { return m_fSize; };
	FLOAT				GetAngle( void ) { return m_fAngle; };
	
	// Overloaded Massive Functions
	virtual massiveBool	MediaDownload(UINT	uCrexID);
	virtual massiveBool	MediaDownloadComplete(LPCTSTR MediaData, const UINT	uDataLength, const UINT	uMediaType, const UINT	uCrexID);
	virtual void		Tick();

	BOOL				InitDeviceObject( void );
	BOOL				RestoreDeviceObject( void );
	void				ReleaseDeviceObject( void );
};

} // name space

#endif

#endif // !defined(_XMVTSUBSCRIBEROBJECT_H_)

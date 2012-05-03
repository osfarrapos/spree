// XMADWrapper.h: interface for the _XMADWrapper class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XMADWRAPPER_H_
#define _XMADWRAPPER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "stdafx.h"

#ifdef _XDEF_MADSYSTEM

#include "MassiveMediaTypes.h"
#include "MassiveClientCore.h"
#include "MassiveAdObjectSubscriber.h"
#include "XMVTSubscriberObject.h"

#define MASSIVE_VERSION "3.1.0.35" 

namespace MassiveAdClient3
{
	
// Forward Declaration
class CMassiveClientCore;
class _XMVTSubscriberObject;

const USHORT	MAX_STATUS_TEXT = 512;
const USHORT	MAX_INVENTORY_ELEMENTS = 64;

typedef struct __XMADGameData
{
	LPTSTR	szSKU;
	LPTSTR	szSKUVersion;
	LPTSTR	szThirdPartyID;
	LPTSTR	szThirdPartyService;
	//LPTSTR	szZoneName;
	//LPTSTR	szNames[MAX_INVENTORY_ELEMENTS];
	UINT	uFlushTime;
	USHORT  uConfigFlags;
	USHORT  uLogLevel;
	//UINT	uNumElements;
} _XMADGameData;

typedef struct __XMADZONE_SETUPDATA
{
	TCHAR	locationname[64];
	TCHAR	zonename[64];
	int		horizalignment;
	int		vertalignment;
	POINT	windowposition;
	SIZE	imagesize;
	int		cyclinginterval;
	LPTSTR	szElementNames[MAX_INVENTORY_ELEMENTS];
	UINT	uNumElements;

	__XMADZONE_SETUPDATA()
	{
		for( int i = 0; i < MAX_INVENTORY_ELEMENTS; i++ )
		{
			szElementNames[i] = NULL;
		}
	}
	~__XMADZONE_SETUPDATA()
	{
		for( int i = 0; i < MAX_INVENTORY_ELEMENTS; i++ )
		{
			SAFE_DELETE_ARRAY(szElementNames[i]);
		}		
	}

}_XMADZONE_SETUPDATA;

class _XMADWrapper  
{

private:
	BOOL					m_bDataLoaded;
	BOOL					m_bMassiveInitialized;		// Massive AdClient initialized successfully?	
	CMassiveClientCore*		m_pCoreInstance;			// Massive AdClient Instance	
	_XMADGameData			m_GameData;

	TCHAR					m_szTextApp[MAX_STATUS_TEXT];
	TCHAR					m_szTextClient[MAX_STATUS_TEXT];

	POINT					m_WindowPosition;
	SIZE					m_WindowSize;

	_XImageStatic			m_TitleLeftBar;	
	_XImageStatic			m_TitleCenterBar;	
	_XImageStatic			m_TitleRightBar;	
	_XImageStatic			m_SubscriberImage;	

	int						m_CurrentLocationIndex;

	int						m_PrevSubscribers;
	int						m_CurrentSubscribers;
	int						m_LastCyclingTime;

	BOOL					m_NeedToFlush;


	// Massive Subscribers
	std::vector<_XMVTSubscriberObject*>				m_Subscribers;
	std::vector<_XMVTSubscriberObject*>::iterator	m_SubscriberCurrent;
	unsigned short									m_SubscriberCounter;

	std::vector<_XMADZONE_SETUPDATA*>				m_ZoneSetupData;
public:
	_XMADWrapper();
	virtual ~_XMADWrapper();


	BOOL	Initialize( void );
	void	Release( void );

	BOOL	IsMassiveInitialized() { return m_bMassiveInitialized; }

	BOOL	SetMADLocation( LPTSTR szLocationName );
	void	DisableMADLocation( LPTSTR szLocationName );

	BOOL	MassiveTick( void );
	BOOL	MassiveEnterZone( LPTSTR szName );
	BOOL	MassiveExitZone( LPTSTR szName	);
	BOOL	MassiveFlushImpressions( void );
	BOOL	Update( void );
	BOOL	UpdateTime( void );

	BOOL	LoadMADZoneSetupData( void );
	UINT	FileLoadGameDataFrom(LPTSTR szFileName);
	int		FileGetNextLine(LPTSTR pBufferIn, UINT inPos, UINT inLength, LPTSTR *ppBufferOut);

	void	SetClientDisplayState( void );

	BOOL	InitDeviceObject( void );
	BOOL	RestoreDeviceObject( void );
	void	ReleaseDeviceObject( void );

	void	CreateMADWindow( void );
	void	DrawMADWindow(void);

	BOOL	GetDownloadComplete( void );
};

	

}	// Namespace

using namespace MassiveAdClient3;

#endif 

#endif // !defined(_XMADWRAPPER_H_)

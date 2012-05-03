// XIGAADWrapperCore.h: interface for the _XIGAADWrapperCore class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _XIGAADWRAPPERCORE_H_
#define _XIGAADWRAPPERCORE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _ACCLAIM_IGAADSYSTEM

#include "radsdk_ll.h"
#include "radsdk.h"
#include "registry.h"

typedef enum _XIGAADZONE
{
	_XIGAADZONE_IDLE = 0,
	_XIGAADZONE_GAMELOADING,
	_XIGAADZONE_MAPLOADING,
	_XIGAADZONE_MAPLOADINGVIDEOAD,
	_XIGAADZONE_NPCDIALOG,
	_XIGAADZONE_AREACHANGE,
	_XIGAADZONE_MEDITATION,
	_XIGAADZONE_DEATH,
	_XIGAADZONE_DEATHVIDEOAD,
};

typedef struct
{
	unsigned __int64	appId;
	int					code;
	int					range;
    char				serverAddress[256];
    unsigned short		serverPort;

} _IGASetupInfo;



typedef struct __XIGAADZONE_SETUPDATA
{
	TCHAR	locationname[64];
	DWORD	zoneindex;
	DWORD	contentstype;
	DWORD	cellid;
	int		horizalignment;
	int		vertalignment;
	POINT	windowposition;
	SIZE	windowsize;
	int		cyclinginterval;
	int		displaytime;
	__XIGAADZONE_SETUPDATA()
	{
	}
	~__XIGAADZONE_SETUPDATA()
	{
	}

}_XIGAADZONE_SETUPDATA;


class _XWindow_BinkStatic;

class _XIGAADWrapperCore  
{
private:
	radSession				m_session;				// the radial network session handle
	BOOL					m_bUseCache;			// use caching library functions
	DWORD					m_dwBandwidth;			// bandwidth limit for non-cached mode
	unsigned __int64		m_appId;				// Global for appID
	BOOL					m_bInSession;			// flag to avoid session recursion

	_XIGAADZONE				m_ADZone;
	unsigned short			m_CurrentZone;
	unsigned int			m_CurrentContentstype;
	TCHAR					m_ContentsDescriptor[256];
	radContentTrack			m_impressiontrack;		// used to track impressions

	int						m_AlphaLevelDirection;	// 0 : stop, 1 : decrease, 2 : increase
	DWORD					m_FrameFadeTimer;
	FLOAT					m_FrameAlphaLevel;
	
	DWORD					m_DisplayTimeCounter;
	
	LPDIRECT3DTEXTURE9		m_pTexture;				// Texture data in graphic memory
	
	_XWindow_BinkStatic*	m_pBinkWindowObject;

	int						m_CurrentContentsFormat;
	DWORD					m_ProcessTimer;

	BOOL					m_ShowWindow;
	BOOL					m_ShowWindowFrame;

	SIZE					m_WindowSize;
	POINT					m_WindowPosition;
	SIZE					m_ImageSize;

	BOOL					m_UseSecondSize;
	SIZE					m_SecondWindowSize;

	_XImageStatic			m_TitleLeftBar;	
	_XImageStatic			m_TitleCenterBar;	
	_XImageStatic			m_TitleBar;	
	_XImageStatic			m_TitleRightBar;	
	_XImageStatic			m_SubscriberImage;	
	
	_XImageStatic			m_BottomCenterBar;

	_XImageStatic			m_LeftVertLine;
	_XImageStatic			m_RightVertLine;

	_XImageStatic			m_DownloadMessageWindow;
	_XImageStatic			m_DownloadMessageIndicator;
	int						m_IndicatorIndex;
	BOOL					m_IndicatorDirection;

	std::vector<_XIGAADZONE_SETUPDATA*>		m_ZoneSetupData;

	LPBYTE					m_ImageRecieveBuffer;
	DWORD					m_AllocatedImageRecieveBufferSize;

	LPBYTE					m_VideoRecieveBuffer;
	DWORD					m_AllocatedVideoRecieveBufferSize;	

	unsigned short			m_LastLoadedZone;
	unsigned int			m_LastLoadedContentstype;
	radInt64				m_LastLoadedContentsID;
	
	BOOL					m_NoMoreUpdate;
	BOOL					m_UseDefaultAD;
public:
	_XIGAADWrapperCore();
	virtual ~_XIGAADWrapperCore();

	BOOL	Initialize( void );
	void	Finalize( void );

	BOOL	LoadMADZoneSetupData( void );

	void	StartTrackImpression(radContentTrack* track, time_t min_time, time_t min_size, time_t max_size, long zoneId, long cellId, radInt64 contentId);
	void	EndTrackImpression(radContentTrack* track);

	void	Update( void );
	void	Update_Cache( void );
	//void	Update_NoCache( unsigned int appId, const char* userInfo, char* buf, int len );
	
	BOOL	InitDeviceObject( void );
	BOOL	RestoreDeviceObject( void );
	void	ReleaseDeviceObject( void );
	
	void	SetZoneInfo( _XIGAADZONE zone );	
	
	BOOL	CheckExistContent( _XIGAADZONE zone );

	void	CreateAdWindow( void );
	SIZE	GetWindowSize( void ){ return m_WindowSize; }
	FLOAT	GetFrameAlphaLevel( void ){ return m_FrameAlphaLevel; }
	DWORD	GetStaticImageAlphaLevel( void );
	void	StartDisplay( void ); // milliseconds
	void	SetFadeMode( void );
	void	ShowWindow( BOOL show ){ m_ShowWindow = show; }
	BOOL	GetShowStatus( void ){ return m_ShowWindow; }
	int		GetFadeMode( void ){ return m_AlphaLevelDirection; } // 0 : stop, 1 : decrease, 2 : increase
	void	SetUseSecondWindowSize( BOOL use, DWORD width = 0, DWORD height = 0 )
	{
			m_UseSecondSize = use;
			m_SecondWindowSize.cx = width;
			m_SecondWindowSize.cy = height;
	}
	void	DisableContentsUpdate( BOOL enable )
	{
			m_NoMoreUpdate = enable;
	}
	void	MoveWindowPosition( int x, int y )
	{
			m_WindowPosition.x = x;
			m_WindowPosition.y = y;
	}
	void	Render( void );
	void	Process( void );

protected:
	char*			code2string(char *buf, int code, int range);
	unsigned short	cellZone(UINT cellIdx);
	unsigned int	cellContent(UINT cellIdx);
	const char*		cellDescriptor(UINT cellIdx, char* desc, int len);
	int				compare_descriptor(const char* content_descriptor, const char* cell_descriptor);
	radInt64		chooseCellContent(radKeyValueList* contentList, int contentCount, unsigned int *ptype, unsigned short zoneType, unsigned int contentType, const char* cellDescriptor);

};

#endif //_ACCLAIM_IGAADSYSTEM

#endif // !defined(_XIGAADWRAPPERCORE_H_)

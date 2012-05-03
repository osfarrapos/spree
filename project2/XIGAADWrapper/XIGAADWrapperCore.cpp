// XIGAADWrapperCore.cpp: implementation of the _XIGAADWrapperCore class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "XIGAADWrapperCore.h"
#include "XProc_ProcessDefine.h"

#ifdef _ACCLAIM_IGAADSYSTEM

#include "XWindow_BinkStatic.h"

#include "XMessageBox.h"
#include "XFMODWrapper.h"
#include "XWindow_OptionSound.h"
#include "XWindow_Option.h"
extern _XMessageBox g_MessageBox;
extern _XFMODWrapper g_FMODWrapper;

//#define _BINKTEST

//////////////////////////////////////////////////////////////////////
// Previous define
//////////////////////////////////////////////////////////////////////

#define DEFAULT_MIN_TIME 1

extern "C" {
extern char _RAD_AUTH_SERVER[];
extern short _RAD_AUTH_PORT;
extern int  _RADSDK_VERSION;
}

char	_RAD_AUTH_SERVER[256];
short	_RAD_AUTH_PORT;
char	_CACHE_DIRECTORY[256];

LONG	g_IGAADDownloadCancelTimer = 0;

//BOOL	gbClose = FALSE;

extern _XTextureManager g_MainInterfaceTextureArchive;

// delay between content refreshing
#define REFRESH_TIMER	500

// global app name
const char APPNAME[] = "9Dragons online";
#define REGKEY HKEY_CURRENT_USER,"Software\\IGA\\9DragonsOnline"

unsigned int contentTypes[] = {2080100,12051000,5080100,5040100,12051000};

extern void _XInsertCenterSystemMessage( LPTSTR message , D3DCOLOR messageinfo );
extern void _XInsertSystemMessage( int messagemode, TCHAR* args, ... );

LPCTSTR	_XGetSecondWordPoint( LPCTSTR pszString )
{
	if( !pszString ) return NULL;

	TCHAR tempbuffer1[256];
	TCHAR tempbuffer2[256];

	sscanf( pszString, _T("%s %s"), tempbuffer1, tempbuffer2 );
	LPCTSTR pCpPoint = strstr( pszString, tempbuffer2 );
	return	pCpPoint;
}

int strnmv(char* dst, const char* src, size_t max)
{
	int c=0;
	for (; max>1; max--,c++)
		*dst++ = *src++;
	*dst = 0;
	return c;
}

char cbcontentlist(radKeyValue* kv, int progress, int total, void* userdata)
{
	/*
	// pump message loop until there are no more messages
	MSG msg;
	while(PeekMessage(&msg,0,0,0,PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gbClose)
		return -1;
	*/
	return 0;
}

char cbcontent(void* bytes, int progress, int total, void* user)
{
	/*
	// update progress bar
	SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETRANGE32, 0, (LPARAM)total);
	SendDlgItemMessage(hDlg, IDC_PROGRESS1, PBM_SETPOS, progress, 0);

	// pump message loop until there are no more messages
	MSG msg;
	while(PeekMessage(&msg,0,0,0,PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gbClose)
		return -1;

	// store bandwidth setting
	session.uiBandwidth = dwBandwidth;
	*/

	return 0;
}

#define GETFACTOR(x) x * (contentType / x)
unsigned int gettype(unsigned int contentType)
{
    contentType -= GETFACTOR(1000000);
    contentType -= GETFACTOR(100000);
    contentType -= GETFACTOR(10000);
    //contentType -= GETFACTOR(1000);  This was removed to cover content types of 1000, 1100, 1200
    return contentType;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XIGAADWrapperCore::_XIGAADWrapperCore()
{
	m_bUseCache						=	TRUE;
	m_dwBandwidth					=	150000;
	m_appId							=	103;
	m_bInSession					=	FALSE;
	m_pTexture						=	NULL;
	m_pBinkWindowObject				=	NULL;
	m_ProcessTimer					=	0;
	m_CurrentContentsFormat			=	0;
	memset( m_ContentsDescriptor, 0, sizeof(TCHAR) * 256 );
	
	m_ShowWindow					=	FALSE;

	m_AlphaLevelDirection			=	0;
	m_FrameFadeTimer				=	0;
	m_FrameAlphaLevel				=	0;	
	m_DisplayTimeCounter			=	0;	

	m_ShowWindowFrame				=	TRUE;
	
	m_ImageRecieveBuffer			=	NULL;
	m_AllocatedImageRecieveBufferSize	=	0;
	m_VideoRecieveBuffer			=	NULL;
	m_AllocatedVideoRecieveBufferSize	=	0;

	m_LastLoadedZone				= 0xFFFF;
	m_LastLoadedContentstype		= 0xFFFFFFFF;
	m_LastLoadedContentsID			= 0xFFFFFFFFFFFFFFFF;

	m_NoMoreUpdate					= FALSE;
	m_UseSecondSize					= FALSE;
	m_UseDefaultAD					= TRUE;
}

void _XIGAADWrapperCore::StartDisplay( void )
{
	m_AlphaLevelDirection	=	2;
	m_FrameFadeTimer		=	0;
	m_FrameAlphaLevel		=	0;	
	m_DisplayTimeCounter	=	0;
	m_ImageSize.cx			=	0;
	m_ImageSize.cy			=	0;
}

void _XIGAADWrapperCore::SetFadeMode( void )
{
	m_AlphaLevelDirection	=	1;
	m_FrameFadeTimer		=	0;
	m_FrameAlphaLevel		=	1.0f;
	m_DisplayTimeCounter	=	0;

	EndTrackImpression(&m_impressiontrack);
}

DWORD _XIGAADWrapperCore::GetStaticImageAlphaLevel( void )
{
	return ((m_SubscriberImage.m_FColor & 0xFF000000) >> 24);
}

_XIGAADWrapperCore::~_XIGAADWrapperCore()
{
	if( m_pTexture )
	{
		SAFE_RELEASE(m_pTexture);
	}

	if(m_pBinkWindowObject)
	{
		m_pBinkWindowObject->BinkFinalize();
		SAFE_DELETE( m_pBinkWindowObject );
	}

	SAFE_DELETE_ARRAY( m_ImageRecieveBuffer );
	//m_AllocatedImageRecieveBufferSize	=	0;
	SAFE_DELETE_ARRAY( m_VideoRecieveBuffer	);
	//m_AllocatedVideoRecieveBufferSize	=	0;
}

BOOL _XIGAADWrapperCore::Initialize( void )
{
	char		tbuf[32];
	Registry	r(REGKEY);

	if( !LoadMADZoneSetupData() )
		return FALSE;

	if( m_pBinkWindowObject )
	{
		m_pBinkWindowObject->BinkFinalize();
		SAFE_DELETE( m_pBinkWindowObject );
	}

	m_pBinkWindowObject = new _XWindow_BinkStatic;
	_XWINDOW_STRUCTURE binkstaticwindowstructure = 
	{
		TRUE, { (gnWidth>>1) - (852>>1), (gnHeight>>1) - (480>>1)}, {852, 480}, 0,
		-1,
		NULL,
		D3DCOLOR_ARGB(128, 0, 0, 0),
		_XDEF_WTITLE_DRAWKEYBOARDWINDOW	
	};
	m_pBinkWindowObject->Create(binkstaticwindowstructure);
	m_pBinkWindowObject->SetWindowMoveMode( _XWMOVE_FIXED );

	r.getStringDef("ServerAddress", _RAD_AUTH_SERVER, 256, "72.3.184.153"); 	
    _RAD_AUTH_PORT = r.getInt("ServerPort", 17475);
	
	//_XSetDefaultPath();

	memset( _CACHE_DIRECTORY, 0, sizeof(TCHAR) * 256 );
	strcpy( _CACHE_DIRECTORY, gModulePath );
	strcat( _CACHE_DIRECTORY, "\\ADCACHE" );

	if( _chdir( _CACHE_DIRECTORY ) == -1 )
	{
		if( _mkdir( _CACHE_DIRECTORY ) != -1 )
		{
			radSetting( "cache_dir", (void*)_CACHE_DIRECTORY, NULL );
		}
	}
	else
	{
		radSetting( "cache_dir", (void*)_CACHE_DIRECTORY, NULL );
	}
	
	char AppId[64];
	r.getStringDef("AppId", AppId, 64, "103");
	r.close();
	
	int retcode = 0;

	if( m_bUseCache )
	{
		// start caching thread
		retcode = radStart(atoi(AppId), 0xffff, contentTypes, sizeof(contentTypes)/sizeof(UINT), code2string(tbuf, r.getInt("Code", 0), r.getInt("Range", 0)));

		if( retcode != 0 )
		{
			_XFatalError( _T("Could not start rad system") );
			return FALSE;
		}
	} 
	else 
	{
		radInit();
	}

	int partyresourceindex = g_MainInterfaceTextureArchive.FindResource( "mi_party.tga" );
	
	m_DownloadMessageWindow.Create( 0, 0, 146, 36,
									&g_MainInterfaceTextureArchive, 
									partyresourceindex );
	m_DownloadMessageWindow.SetClipRect( 0, 144, 146, 179 );

	m_DownloadMessageIndicator.Create( 0, 0, 24, 17,
									   &g_MainInterfaceTextureArchive, 
									   partyresourceindex );
	m_DownloadMessageIndicator.SetClipRect( 168, 17, 192, 34 );


	m_IndicatorDirection = TRUE;
	m_IndicatorIndex = 0;	
	
	return TRUE;
}

void _XIGAADWrapperCore::Finalize( void )
{
	// uninitialize radial library
	if(m_bUseCache) 
	{
		radStop();
	} 
	else 
	{
		radUninit();
	}

	if( m_ZoneSetupData.size() > 0 )
	{
		std::for_each( m_ZoneSetupData.begin(), m_ZoneSetupData.end(), DELETEOBJECT() );
		m_ZoneSetupData.clear();
	}

	SAFE_RELEASE( m_pTexture );
	
	if(m_pBinkWindowObject)
	{
		m_pBinkWindowObject->BinkFinalize();
		SAFE_DELETE( m_pBinkWindowObject );
	}

	SAFE_DELETE_ARRAY( m_ImageRecieveBuffer );
	m_AllocatedImageRecieveBufferSize	=	0;
	SAFE_DELETE_ARRAY( m_VideoRecieveBuffer	);
	m_AllocatedVideoRecieveBufferSize	=	0;

	m_LastLoadedZone				= 0xFFFF;
	m_LastLoadedContentstype		= 0xFFFFFFFF;
	m_LastLoadedContentsID			= 0xFFFFFFFFFFFFFFFF;
}

void _XIGAADWrapperCore::StartTrackImpression(radContentTrack* track, time_t min_time, time_t min_size, time_t max_size, long zoneId, long cellId, radInt64 contentId)
{

	track->valid = true;
	track->startTime = ( time(0) );
	track->contentId = contentId;
	track->zoneId = zoneId;
	track->cellId = cellId;
	// Server now sends min time in mS, so divide by 1000
	track->min_time = min_time / 1000;
	track->min_size = min_size;
	track->max_size = max_size;
	track->pContent = 0;

	_XLog( "IGAAD : Start track impression [ZoneID:%d CellID:%d ContentID:%d Start:%d]", track->zoneId, track->cellId, track->contentId, track->startTime );
}

//
// stop tracking an impression
//
void _XIGAADWrapperCore::EndTrackImpression(radContentTrack* track)
{
	if(m_UseDefaultAD)
	{
		return;
	}

	// Eric M Solliday
	// Assuming that if you call EndTrackImpression you want to stop tracking
	// This is even if the track time is less than the minimum impression time
	track->valid = false;

	track->endTime = ( time(0) );
	if (track->endTime - track->startTime < track->min_time)
		return;

	if (m_bUseCache)
	{
		if( ( m_appId == 12 ) || ( m_appId == 23 ) || ( m_appId == 27 ) || ( m_appId == 29 ) )
		{
			radReportLocalImpression(track->zoneId, track->cellId, track->contentId, track->endTime-track->startTime, track->startTime, 25.0, 0.0, "");
		}
		else
		{
			radReportLocalImpression(track->zoneId, track->cellId, track->contentId, ( (track->endTime-track->startTime) * 1000 ), track->startTime, 25.0, 0.0, "");
		}
	}
	else
	{
		if( ( m_appId == 12 ) || ( m_appId == 23 ) || ( m_appId == 27 ) || ( m_appId == 29 ) )
		{
			radReportImpression(&m_session, track->zoneId, track->cellId, track->contentId, track->endTime-track->startTime, track->startTime, "");
		}
		else
		{
			radReportImpression(&m_session, track->zoneId, track->cellId, track->contentId, ( (track->endTime-track->startTime) * 1000 ), track->startTime, "");
		}
	}

	if (track->pContent)
		DeleteObject(track->pContent);
	track->pContent = 0;

	//_XInsertSystemMessage( 0, "IGAAD : End track impression [ZoneID:%d CellID:%d ContentID:%d Time:%d STime:%d]", track->zoneId, track->cellId, track->contentId, ( (track->endTime-track->startTime) * 1000 ), track->startTime );
	_XLog( "IGAAD : End track impression [ZoneID:%d CellID:%d ContentID:%d Time:%d STime:%d]", track->zoneId, track->cellId, track->contentId, ( (track->endTime-track->startTime) * 1000 ), track->startTime );

}

void _XIGAADWrapperCore::Update( void )
{
	char			tbuf[256];
	char*			userInfo;
	unsigned int	appId;

	if(	m_bInSession )
		return;

	m_bInSession = TRUE;

	Registry r(REGKEY);

	r.getStringDef("AppId", tbuf, 256, "103");

	appId = atoi(tbuf);

	userInfo = code2string(tbuf, r.getInt("Code", 0), r.getInt("Range", 0));

	//if (m_bUseCache)
	Update_Cache();
	//else
	//	Update_NoCache(appId, userInfo);

	// clear in-session flag
	m_bInSession = FALSE;
}

BOOL _XIGAADWrapperCore::CheckExistContent( _XIGAADZONE zone )
{
	_XIGAADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[ (int)zone-1 ];	

	BOOL exist = FALSE;

	if(pSetupData)
	{
		int				length = 0;
		int				nresult = -1;
		radInt64		contentId = 0;
		nresult = radGetLocalContent(pSetupData->zoneindex, pSetupData->contentstype, &contentId, &length, 0, 0);
		
		if( nresult == RAD_SUCCESS )
			exist = TRUE;
	}

	//_XInsertSystemMessage( 0, "Check AD content : %d %s", zone, exist ? "TRUE" : "FALSE" );
	//_XLog( "Check AD content : %d %d", zone, exist );

	return exist;
}

void _XIGAADWrapperCore::Update_Cache( void )
{
	char			props[2048];
	int				length;
	int				nresult;
	radInt64		contentId;

	// get a content item id
	contentId = 0;

	nresult = radGetLocalContent(m_CurrentZone, m_CurrentContentstype, &contentId, &length, 0, 0);

	//_XLog( "get local content info : zone:%d, type:%d, contentsid:%d, length:%d", m_CurrentZone, m_CurrentContentstype, contentId, length );

	// start tracking
	
	if (m_impressiontrack.valid) 
	{
		// skip this item if it's already in this cell
		if (contentId == m_impressiontrack.contentId && ( time(0) - m_impressiontrack.startTime) < 30)
		{
			return;
		}
		else
		{
			// stop tracking the item that's currently in the cell
			EndTrackImpression(&m_impressiontrack);
			//_XInsertSystemMessage( 0, "End track impression : Zone:%d, ContentType:%d", m_CurrentZone, m_CurrentContentstype );
		}
	}

	BOOL noneexistentcontent = FALSE;

	// make sure we got an id
	if (nresult != RAD_SUCCESS) 
	{
		noneexistentcontent = TRUE;

		TCHAR errormessage[256];
		errormessage[0] = NULL;

		switch( nresult )
		{
		case RAD_ERROR_SOCKETERROR		:		
			//g_MessageBox.SetMessage( "IGAAD Error : a socket error occured" );
			strcpy( errormessage, "IGAAD Error : a socket error occured" );
			break;     
		case RAD_ERROR_NOSERVER			:		
			//g_MessageBox.SetMessage( "IGAAD Error : error connecting to session server" );
			strcpy( errormessage, "IGAAD Error : error connecting to session server" );
			break;
		case RAD_ERROR_UNSUPPORTED		:		
			//g_MessageBox.SetMessage( "IGAAD Error : the server did not recognize a packet" );
			strcpy( errormessage, "IGAAD Error : the server did not recognize a packet" );
			break;		
		case RAD_ERROR_INVALIDPARAMETER	:		
			//g_MessageBox.SetMessage( "IGAAD Error : one or more parameters to a function were invalid" );
			strcpy( errormessage, "IGAAD Error : one or more parameters to a function were invalid" );
			break;
		case RAD_ERROR_INTERNALERROR	:		
			//g_MessageBox.SetMessage( "IGAAD Error : something bad happened internally" );
			strcpy( errormessage, "IGAAD Error : something bad happened internally" );
			break;
		case RAD_ERROR_COMMUNICATION	:		
			//g_MessageBox.SetMessage( "IGAAD Error : a communication error occured(connection dropped" );
			strcpy( errormessage, "IGAAD Error : a communication error occured(connection dropped" );
			break;
		case RAD_ERROR_BUFFERSIZE		:		
			//g_MessageBox.SetMessage( "IGAAD Error : the buffer passed to getcontent was too small to fit the content" );
			strcpy( errormessage, "IGAAD Error : the buffer passed to getcontent was too small to fit the content" );
			break;
		case RAD_ERROR_NOCONTENT		:
			{
				if( g_IGAADDownloadCancelTimer == 0 )
					g_IGAADDownloadCancelTimer = g_LocalSystemTime;

				if( g_LocalSystemTime - g_IGAADDownloadCancelTimer > 10000 )
				{
					unsigned int progress = 0;
					if (radSetting("progress", 0, &progress) == RAD_SUCCESS) 
					{
						if( progress > 0 )
						{
							//_XInsertSystemMessage( 0, "Download progress %d", progress );
							return;
						}
					}
				}
				else
				{
					return;
				}

				// 여기까지 오면 Default image 사용.
			}
			//g_MessageBox.SetMessage( "IGAAD Error : a request for a non-existent content id occured, or a search revealed no content" );			
			//strcpy( errormessage, "IGAAD Error : a request for a non-existent content id occured, or a search revealed no content" );
			//break;
		case RAD_ERROR_CACHE			:		
			//g_MessageBox.SetMessage( "IGAAD Error : there was a problem with the local cache(disk read-protected or file corrupted)" );
			strcpy( errormessage, "IGAAD Error : there was a problem with the local cache(disk read-protected or file corrupted)" );
			break;
		case RAD_ERROR_SYSTEM			:		
			//g_MessageBox.SetMessage( "IGAAD Error : couldn't create a thread for background data transfer" );
			strcpy( errormessage, "IGAAD Error : couldn't create a thread for background data transfer" );
			break;
		case RAD_ERROR_CANCELLED		:		
			//g_MessageBox.SetMessage( "IGAAD Error : a transfer operation was cancelled by a callback function" );
			strcpy( errormessage, "IGAAD Error : a transfer operation was cancelled by a callback function" );
			break;
		case RAD_ERROR_NOTINITIALIZED	:
			//g_MessageBox.SetMessage( "IGAAD Error : an api function was called before radInit() or radStart()" );
			strcpy( errormessage, "IGAAD Error : an api function was called before radInit() or radStart()" );
			break;
		case RAD_ERROR_DISABLED			:		
			//g_MessageBox.SetMessage( "IGAAD Error : the advertising subsystem has been globally disabled for this application" );
			strcpy( errormessage, "IGAAD Error : the advertising subsystem has been globally disabled for this application" );
			break;
		}		

		if( errormessage[0] )
		{
			_XLog( errormessage );
		}

		//_XInsertSystemMessage( 0, "%d:%s", nresult, errormessage );
	}
	else
	{
		//if(!m_impressiontrack.valid)
		//	return;
	}
	
	//if( _XCmdLineOption("testdefaultad",0) != 0 )
	//	noneexistentcontent = TRUE;

	if( m_LastLoadedZone == m_CurrentZone &&
		m_LastLoadedContentstype == m_CurrentContentstype &&
		m_LastLoadedContentsID == contentId )
	{
		return;
	}


	// put content id in frame title
	//sprintf(tbuf, "Cell%d (id %I64d)", curCell, contentId);
	//SetDlgItemText(hDlg, IDC_CELL1FRAME+curCell, tbuf);

	unsigned int dataformat = gettype(m_CurrentContentstype);

	if( noneexistentcontent )
	{
		_XLog( _T("IGANOTICE : Use default resource [Zone=%d,Content=%d,ID=%d,Size=%d]"), m_CurrentZone, m_CurrentContentstype, contentId, length );

		//_XInsertSystemMessage( 0, _T("IGANOTICE : Use default resource [Zone=%d,Content=%d,ID=%d,Size=%d]"), m_CurrentZone, m_CurrentContentstype, contentId, length  );

		if( dataformat == 1000 ) // Bink video
		{
			_XPackageArchive adresourcepackage;
			
			TCHAR packagefilename[_MAX_PATH];
			packagefilename[0] = NULL;
			strcpy( packagefilename, gModulePath );
			strcat( packagefilename, _T("\\Data\\Script\\ADRESOURCE.XP") );
			
			if( !adresourcepackage.OpenPackage( packagefilename ) )
			{
				return;
			}

			int resourceid = adresourcepackage.FindResource( _T("ADDefault.bik") );			
			if( resourceid >= 0 )
			{				
				length = adresourcepackage.GetPackedFileSize( resourceid );
				FILE* pFile = adresourcepackage.GetPackedFile( resourceid );

				if(!pFile)
				{
					_XFatalError( _T("IGAERROR : Could't access file [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
									  m_CurrentZone, m_CurrentContentstype, contentId, length );
					m_AllocatedVideoRecieveBufferSize = 0;
					adresourcepackage.FinalizePackage();
					return;
				}
				
				if( m_AllocatedVideoRecieveBufferSize < length )
				{
					SAFE_DELETE_ARRAY( m_VideoRecieveBuffer );

					m_VideoRecieveBuffer = new BYTE[ length ];

					if( !m_VideoRecieveBuffer )
					{
						_XFatalError( _T("IGAERROR : Could't allocate memory for default video [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
									  m_CurrentZone, m_CurrentContentstype, contentId, length );
						m_AllocatedVideoRecieveBufferSize = 0;
						adresourcepackage.FinalizePackage();
						return;
					}
					m_AllocatedVideoRecieveBufferSize = length;
				}
				else
				{
					if( m_VideoRecieveBuffer && (m_AllocatedVideoRecieveBufferSize > 0) )
					{
						memset( m_VideoRecieveBuffer, 0, sizeof(BYTE) * m_AllocatedVideoRecieveBufferSize );
					}
				}

				if( fread( m_VideoRecieveBuffer, length, 1, pFile ) < 1 )
				{
					_XFatalError( _T("IGAERROR : Could't read default video [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
									  m_CurrentZone, m_CurrentContentstype, contentId, m_AllocatedVideoRecieveBufferSize );
					
					adresourcepackage.FinalizePackage();
					return;
				}
			}

			adresourcepackage.FinalizePackage();

			// now get the content item
			//nresult = radGetLocalContent(0, 0, &contentId, &length, m_VideoRecieveBuffer, props);
		}
		else
		{
			_XPackageArchive adresourcepackage;
			
			TCHAR packagefilename[_MAX_PATH];
			packagefilename[0] = NULL;
			strcpy( packagefilename, gModulePath );
			strcat( packagefilename, _T("\\Data\\Script\\ADRESOURCE.XP") );

			if( !adresourcepackage.OpenPackage( packagefilename ) )
			{
				return;
			}

			int resourceid = adresourcepackage.FindResource( _T("ADDefault.jpg") );
			
			if( resourceid >= 0 )
			{				
				length = adresourcepackage.GetPackedFileSize( resourceid );
				FILE* pFile = adresourcepackage.GetPackedFile( resourceid );

				if(!pFile)
				{
					_XFatalError( _T("IGAERROR : Could't access file [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
									  m_CurrentZone, m_CurrentContentstype, contentId, length );
					m_AllocatedImageRecieveBufferSize = 0;
					adresourcepackage.FinalizePackage();
					return;
				}

				if( m_AllocatedImageRecieveBufferSize < length )
				{
					SAFE_DELETE_ARRAY( m_ImageRecieveBuffer );

					m_ImageRecieveBuffer = new BYTE[ length ];

					if( !m_ImageRecieveBuffer )
					{
						_XFatalError( _T("IGAERROR : Could't allocate memory for default image [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
								  m_CurrentZone, m_CurrentContentstype, contentId, length );				
						m_AllocatedImageRecieveBufferSize = 0;
						adresourcepackage.FinalizePackage();
						return;
					}
					m_AllocatedImageRecieveBufferSize = length;
				}
				else
				{
					if( m_ImageRecieveBuffer && (m_AllocatedImageRecieveBufferSize > 0) )
					{
						memset( m_ImageRecieveBuffer, 0, sizeof(BYTE) * m_AllocatedImageRecieveBufferSize );
					}
				}

				if( fread( m_ImageRecieveBuffer, length, 1, pFile ) < 1 )
				{
					_XFatalError( _T("IGAERROR : Could't read default image [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
									  m_CurrentZone, m_CurrentContentstype, contentId, length );
					adresourcepackage.FinalizePackage();
					return;
				}
			}

			adresourcepackage.FinalizePackage();
		}
	}
	else
	{
		if( dataformat == 1000 ) // Bink video
		{
			if( m_AllocatedVideoRecieveBufferSize < length )
			{
				SAFE_DELETE_ARRAY( m_VideoRecieveBuffer );

				m_VideoRecieveBuffer = new BYTE[ length ];

				if( !m_VideoRecieveBuffer )
				{
					_XFatalError( _T("IGAERROR : Could't allocate memory for IGAAD Video resource [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
								  m_CurrentZone, m_CurrentContentstype, contentId, length );
					m_AllocatedVideoRecieveBufferSize = 0;
					return;
				}
				m_AllocatedVideoRecieveBufferSize = length;
			}
			else
			{
				if( m_VideoRecieveBuffer && (m_AllocatedVideoRecieveBufferSize > 0) )
				{
					memset( m_VideoRecieveBuffer, 0, sizeof(BYTE) * m_AllocatedVideoRecieveBufferSize );
				}
			}

			// now get the content item
			nresult = radGetLocalContent(0, 0, &contentId, &length, m_VideoRecieveBuffer, props);
		}
		else
		{
			if( m_AllocatedImageRecieveBufferSize < length )
			{
				SAFE_DELETE_ARRAY( m_ImageRecieveBuffer );

				m_ImageRecieveBuffer = new BYTE[ length ];

				if( !m_ImageRecieveBuffer )
				{
					_XFatalError( _T("IGAERROR : Could't allocate memory for IGAAD Resource [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
								  m_CurrentZone, m_CurrentContentstype, contentId, length );				
					m_AllocatedImageRecieveBufferSize = 0;
					return;
				}
				m_AllocatedImageRecieveBufferSize = length;
			}
			else
			{
				if( m_ImageRecieveBuffer && (m_AllocatedImageRecieveBufferSize > 0) )
				{
					memset( m_ImageRecieveBuffer, 0, sizeof(BYTE) * m_AllocatedImageRecieveBufferSize );
					m_AllocatedImageRecieveBufferSize = length;
				}
			}

			// now get the content item
			nresult = radGetLocalContent(0, 0, &contentId, &length, m_ImageRecieveBuffer, props);
		}

		//_XInsertSystemMessage( 0, "get local content data : retcode : %d, zone:%d, type:%d, contentsid:%d, length:%d", nresult, m_CurrentZone, m_CurrentContentstype, contentId, length );
	}

	SAFE_RELEASE( m_pTexture );

	if (dataformat == 100  || // JPEG
		dataformat == 200  || // BMP
		dataformat == 1200 || // PNG
		dataformat == 1100 )  // DDS
	{
		/*
		TCHAR filename[64];
		sprintf( filename, "%d_%d.jpg", m_CurrentZone, m_CurrentContentstype );
		FILE* pFile = fopen(filename,"wb");
		fwrite( m_ImageRecieveBuffer, length, 1, pFile );
		fclose( pFile );		
		*/		

		HRESULT result;
		D3DXIMAGE_INFO SrcInfo;
		if( FAILED( result = D3DXGetImageInfoFromFileInMemory( m_ImageRecieveBuffer, length, &SrcInfo ) ) )
		{
			_XFatalError( "Can't read texture informations" );			
			return;
		}

		HRESULT ret;
		if( FAILED( ret = D3DXCreateTextureFromFileInMemoryEx( gpDev, (LPCVOID)m_ImageRecieveBuffer, length,
													 SrcInfo.Width, SrcInfo.Height, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
													 D3DX_FILTER_BOX,
													 D3DX_FILTER_BOX,
													 0, NULL, NULL, &m_pTexture ) ) )
		{
			_XFatalError( _T("IGAERROR : Could not create texture [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
							  m_CurrentZone, m_CurrentContentstype, contentId, length );
			return;
		}

		D3DSURFACE_DESC surfdesc;
		m_pTexture->GetLevelDesc( 0, &surfdesc );
		m_ImageSize.cx = surfdesc.Width;
		m_ImageSize.cy = surfdesc.Height;

		//_XLog( "AD Image size [%s] width = %d, height = %d", filename, m_ImageSize.cx, m_ImageSize.cy );

		CreateAdWindow();

		m_LastLoadedZone = m_CurrentZone;
		m_LastLoadedContentstype = m_CurrentContentstype;
		m_LastLoadedContentsID = contentId;
	}
	else if( dataformat == 500 || 
			 dataformat == 600   ) // WAV and MP3
	{
		
		m_LastLoadedZone = m_CurrentZone;
		m_LastLoadedContentstype = m_CurrentContentstype;
		m_LastLoadedContentsID = contentId;
	}
	else if( dataformat == 700 ) // Text
	{
		_XInsertCenterSystemMessage( (char*)m_ImageRecieveBuffer, D3DCOLOR_ARGB( 255, 255, 255, 0 ) );
		m_LastLoadedZone = m_CurrentZone;
		m_LastLoadedContentstype = m_CurrentContentstype;
		m_LastLoadedContentsID = contentId;
	}
	else if( dataformat == 1000 ) // Bink video
	{
#ifndef _BINKTEST
		
		//FILE* pFile = fopen( "bink.bik", "wb" );
		//fwrite( m_VideoRecieveBuffer, m_AllocatedVideoRecieveBufferSize, 1, pFile );
		//fclose( pFile );
		
		if(m_pBinkWindowObject)
		{
			m_pBinkWindowObject->Initailize();
			m_pBinkWindowObject->BinkInitialize( NULL, m_VideoRecieveBuffer );
		}
		CreateAdWindow();

		m_LastLoadedZone = m_CurrentZone;
		m_LastLoadedContentstype = m_CurrentContentstype;
		m_LastLoadedContentsID = contentId;
		m_NoMoreUpdate = TRUE;
		
#endif
	}
	else // Unknown..
	{
		_XFatalError( _T("IGAERROR : Unknown format [Zone=%d,Content=%d,ID=%d,Size=%d]"), 
					  m_CurrentZone, m_CurrentContentstype, contentId, length );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Start track impression.

	m_UseDefaultAD = noneexistentcontent;

	if( !m_UseDefaultAD )
	{
		radKeyValue* kv;
		radMakeKeyValues(props, &kv);

		// get default minimum threshold	
		Registry r(REGKEY);
		int min_time = atoi(radGetValue(kv, "minTime", "1"));
		if (min_time == 0)
			min_time = r.getInt("default_min_time", DEFAULT_MIN_TIME);

		long cellid = 0;
		_XIGAADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[ (int)m_ADZone-1 ];	
		if(pSetupData)
		{
			cellid = pSetupData->cellid;
		}

		// start tracking new impression
		StartTrackImpression(&m_impressiontrack, min_time,
							 atoi(radGetValue(kv, "minSize", "25")), 
							 atoi(radGetValue(kv, "maxSize", "100")),
							 m_CurrentZone, cellid , contentId);

		radFreeKeyValues(kv);

		//_XInsertSystemMessage( 0, "Start track impression : zone:%d, cellid:%d, contenttype:%d, contentsid:%d", m_CurrentZone, cellid, m_CurrentContentstype, contentId );
	}
}

/*
void _XIGAADWrapperCore::Update_NoCache( unsigned int appId, const char* userInfo, char* buf, int len )
{
	int nret;
	int buf_len = len;

	// start a session
	if (radStartSession(appId, &m_session) != RAD_SUCCESS) 
	{
		_XLog( "Error starting session, check server connection" );
		return;
	}

	// store bandwidth setting
	m_session.uiBandwidth = m_dwBandwidth;

	// get content list
	unsigned int count;
	radKeyValueList* propList = 0;
	if ((nret = radGetContentListCB(&m_session, m_CurrentZone, 0, 12, (char*)userInfo, &count, &propList, cbcontentlist, 0)) != RAD_SUCCESS) 
	{
		_XLog( "Error retrieving content list. Error code: %d", nret );
		return;
	}

	if (propList == NULL) 
	{
		_XLog( "Error: No content" );
		return;
	}

	// update result count
	//sprintf(tbuf, "Result Count: %d", count);
	//SetDlgItemText(hDlg, IDC_RESULTCOUNT, tbuf);
	unsigned int contentType;
		
	// get new content id
	__int64 contentId = chooseCellContent(propList, count, &contentType, m_CurrentZone, m_CurrentContentstype, m_ContentsDescriptor );

	// start tracking
	if (m_impressiontrack.valid) 
	{

		// skip this item if it's already in this cell
		if (contentId == m_impressiontrack.contentId)
			return;

		// stop tracking the item that's currently in the cell
		EndTrackImpression(&m_impressiontrack);
	}

	if (!contentId) 
	{
		
		//sprintf(tbuf, "Cell%d", curCell);
		//SetDlgItemText(hDlg, IDC_CELL1FRAME+curCell, tbuf);

		// set image in the picture window
		//SendMessage(hPict[curCell], WM_USER+100, 0,0);
		//lprintf("No content for cell %d, type %u", curCell, cellType);		
		return;
	}

	// put content id in frame title
	
	//sprintf(tbuf, "Cell%d (id %I64d)", curCell, contentId);
	//SetDlgItemText(hDlg, IDC_CELL1FRAME+curCell, tbuf);`
	
	//LARGE_INTEGER cnt_s, cnt_e, cnt_freq;
	//QueryPerformanceFrequency(&cnt_freq);
	//QueryPerformanceCounter(&cnt_s);	

	// get content item
	int nResult;
	char props[2048];
	len = buf_len;

	BOOL processnextaction = FALSE;

	nResult = radGetContentCB(&m_session, contentId, 0, &len, buf, props, cbcontent, 0);
	if (nResult != RAD_SUCCESS) {
		if (nResult == RAD_ERROR_CANCELLED)
			processnextaction = FALSE;		
		
		_XLog( "Download failed: Error %d", nResult );

		processnextaction = FALSE;
	}

	if( processnextaction )
	{
		m_CurrentContentsFormat = contentType;
		
		radKeyValue* kv;
		radMakeKeyValues(props, &kv);

		// get default minimum threshold
		Registry r(REGKEY);
		int min_time = atoi(radGetValue(kv, "minTime", "0"));
		if (min_time == 0)
			min_time = r.getInt("default_min_time", DEFAULT_MIN_TIME);

		// start tracking new impression
		StartTrackImpression(&m_impressiontrack, min_time,
			atoi(radGetValue(kv, "minSize", "25")), atoi(radGetValue(kv, "maxSize", "100")),
			m_CurrentZone, 0, contentId);

		radFreeKeyValues(kv);
	}
}
*/

char* _XIGAADWrapperCore::code2string(char *buf, int code, int range)
{
	char _code[][8] = {"m","f"};
	char _range[][8] = {"18","13","14","15","16","17","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35","36","37","38","39","40"};
	strcpy(buf, _code[code]);
	strcat(buf, _range[range]);
	return buf;
}

unsigned short _XIGAADWrapperCore::cellZone(UINT cellIdx)
{
	char key[256];
	Registry r(REGKEY);
	sprintf(key, "Cell%u_zone", cellIdx);
	return (unsigned short)pow(2.0,r.getInt(key,1));
}

unsigned int _XIGAADWrapperCore::cellContent(UINT cellIdx)
{
	char key[256];
	Registry r(REGKEY);
	sprintf(key, "Cell%u_content_value", cellIdx);
	return (unsigned int)r.getInt(key,contentTypes[0]);
}
const char* _XIGAADWrapperCore::cellDescriptor(UINT cellIdx, char* desc, int len)
{
	char key[256];
	Registry r(REGKEY);
	sprintf(key, "Cell%u_descriptor", cellIdx);
	r.getStringDef(key, desc, len, "0");
	return desc;
}

/* compare two descriptors, which are large hex numbers (256 bits) */
int _XIGAADWrapperCore::compare_descriptor(const char* content_descriptor, const char* cell_descriptor)
{
	char *s1, *s2;
	size_t l1, l2;
	unsigned int v1, v2;
	char av1[65];
	char av2[65];

	/* clear number buffers */
	memset(av1, 0x30, 64); memset(av2, 0x30, 64);
	av1[64]=0; av2[64]=0;
	l1 = strlen(content_descriptor); l2 = strlen(cell_descriptor);
	if (l1>64 || l2>64)
		return 0;

	if (l1>0) strnmv((av1+64)-l1, content_descriptor, l1+1);
	if (l2>0) strnmv((av2+64)-l2, cell_descriptor, l2+1);

	for (s1=av1+56,s2=av2+56; s1 >= av1; s1 -= 8, s2 -= 8)
	{
		sscanf(s1, "%x", &v1);
		sscanf(s2, "%x", &v2);
		if ((v1 & v2) != v2) {
			return 0;
		}
		*s1 = 0;
		*s2 = 0;
	}

	/* success */
	return 1;
}

radInt64 _XIGAADWrapperCore::chooseCellContent(radKeyValueList* contentList, int contentCount, unsigned int *ptype, unsigned short zoneType, unsigned int contentType, const char* cellDescriptor)
{
	int i=0;
	int start = rand() % contentCount;
	int end = start + contentCount;
   radKeyValueList* l;

	// move to first item
	for (l=contentList; l && i<start; l=l->pNext) {i++;}

	for (; i<end; i++, l=l->pNext)
	{
		// wrap l
		if (!l)
			l = contentList;

		// get the content type, zone, and descriptors
        const char* stype = radGetValue(l->pKeyValues, "contentType", "0");
        const char* szone = radGetValue(l->pKeyValues, "zoneId", "0");
        const char* sdesc = radGetValue(l->pKeyValues, "cellDescriptors", "0");
		if (!stype || !szone || !sdesc)
			return 0;

		unsigned long type = atol(stype);
		unsigned long zone = atol(szone);
		unsigned long desc = atol(sdesc);

		// check content type
		if (contentType != type)
			continue;

		// check zone
		if (!(zoneType & zone))
			continue;

		// check descriptors
		if (!compare_descriptor(sdesc, cellDescriptor))
			continue;

		// return content id
		*ptype = type;
		return _atoi64(radGetValue(l->pKeyValues, "contentId", "0"));
	}

	return 0;
}

void _XIGAADWrapperCore::SetZoneInfo( _XIGAADZONE zone )
{
	m_ADZone = zone;

	m_LastLoadedZone				= 0xFFFF;
	m_LastLoadedContentstype		= 0xFFFFFFFF;
	m_LastLoadedContentsID			= 0xFFFFFFFFFFFFFFFF;

	if( m_ADZone == _XIGAADZONE_IDLE )
	{
		m_CurrentZone			=	0;
		m_CurrentContentstype	=	0;
		return;
	}

	_XIGAADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[ (int)m_ADZone-1 ];	

	if(pSetupData)
	{
		m_CurrentZone = pSetupData->zoneindex;
		m_CurrentContentstype	=	pSetupData->contentstype;
	}

	if( m_ADZone == _XIGAADZONE_MEDITATION )
		m_ShowWindowFrame = FALSE;
	else
		m_ShowWindowFrame = TRUE;

	if(m_pBinkWindowObject)
	{
		m_pBinkWindowObject->BinkFinalize();
		m_pBinkWindowObject->Initailize();
	}

	CreateAdWindow();

	m_NoMoreUpdate = FALSE;

	m_UseSecondSize = FALSE;

	m_UseDefaultAD = TRUE;

	memset( &m_impressiontrack, 0, sizeof(radContentTrack) );

	m_ProcessTimer = 0;

	g_IGAADDownloadCancelTimer = 0;

	SAFE_RELEASE( m_pTexture );
}

BOOL _XIGAADWrapperCore::LoadMADZoneSetupData( void )
{
	std::for_each( m_ZoneSetupData.begin(), m_ZoneSetupData.end(), DELETEOBJECT() );
	m_ZoneSetupData.clear();
	
	_XPackageArchive adresourcepackage;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Script\\ADRESOURCE.XP") );
		
	if( !adresourcepackage.OpenPackage( packagefilename ) )
	{
		return FALSE;
	}
	
	FILE* pFile = NULL;
	pFile = adresourcepackage.GetPackedFile( _T("IGAADScript.ini") );

	if( !pFile )
	{
		adresourcepackage.FinalizePackage();
		return FALSE;
	}
	
	int index = 0;
	TCHAR buffer[512];
	
	_XIGAADZONE_SETUPDATA* pSetupData = NULL;
	
	while(1)
	{
		if( feof(pFile) ) 
			break;

		memset( buffer, 0, sizeof(TCHAR)*512 );
		if( !fgets( buffer, 511, pFile ) )
		{
			break;
		}
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) 
		{
			break;
		}
		
		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			if( strncmp( buffer, _T("LOCATIONAME"), strlen(_T("LOCATIONAME") ) ) == 0 )
			{
				pSetupData = new _XIGAADZONE_SETUPDATA;				
				memset( pSetupData, 0, sizeof(_XIGAADZONE_SETUPDATA) );

				strcpy( pSetupData->locationname, _XGetSecondWordPoint(buffer) );
				int len = strlen( pSetupData->locationname );
				if( pSetupData->locationname[len-1] == _T('\n') || pSetupData->locationname[len-1] == _T('\r') )
				{
					pSetupData->locationname[len-1] = NULL;
				}
				if( pSetupData->locationname[len-2] == _T('\n') || pSetupData->locationname[len-2] == _T('\r') )
				{
					pSetupData->locationname[len-2] = NULL;
				}

				pSetupData->horizalignment = -1;
				pSetupData->vertalignment = -1;

				m_ZoneSetupData.insert(m_ZoneSetupData.end(), pSetupData);
			}
			else if( strncmp( buffer, _T("ZONEINDEX"), strlen(_T("ZONEINDEX") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					DWORD zoneindex = 0;
					sscanf( buffer, _T("%s %d"), tempbuffer, &zoneindex );

					pSetupData->zoneindex = (unsigned short)pow(2.0,zoneindex);
				}
			}
			else if( strncmp( buffer, _T("CONTENTSTYPE"), strlen(_T("CONTENTSTYPE") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					sscanf( buffer, _T("%s %d"), tempbuffer, &(pSetupData->contentstype) );
				}
			}
			else if( strncmp( buffer, _T("CELLID"), strlen(_T("CELLID") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					sscanf( buffer, _T("%s %d"), tempbuffer, &(pSetupData->cellid) );
				}
			}
			else if( strncmp( buffer, _T("HORIZALIGN"), strlen(_T("HORIZALIGN") ) ) == 0 )
			{
				if( pSetupData )
				{
					LPCTSTR pStr = _XGetSecondWordPoint(buffer);					
					if( strncmp(pStr, _T("LEFT"), strlen(_T("LEFT")) ) == 0 )
					{
						pSetupData->horizalignment = 0;
					}
					else if( strncmp(pStr, _T("RIGHT"), strlen(_T("RIGHT")) ) == 0 )
					{
						pSetupData->horizalignment = 1;
					}
					else if( strncmp(pStr, _T("CENTER"), strlen(_T("CENTER")) ) == 0 )
					{
						pSetupData->horizalignment = 2;
					}
					else 
					{
						pSetupData->horizalignment = -1;
					}
				}
			}
			else if( strncmp( buffer, _T("VERTALIGN"), strlen(_T("VERTALIGN") ) ) == 0 )
			{
				if( pSetupData )
				{
					LPCTSTR pStr = _XGetSecondWordPoint(buffer);					

					if( strncmp(pStr, _T("TOP"), strlen(_T("TOP")) ) == 0 )
					{
						pSetupData->vertalignment = 0;
					}
					else if( strncmp(pStr, _T("BOTTOM"), strlen(_T("BOTTOM")) ) == 0 )
					{
						pSetupData->vertalignment = 1;
					}
					else if( strncmp(pStr, _T("CENTER"), strlen(_T("CENTER")) ) == 0 )
					{
						pSetupData->vertalignment = 2;
					}
					else 
					{
						pSetupData->vertalignment = -1;
					}
				}
			}			
			else if( strncmp( buffer, _T("POSITION"), strlen(_T("POSITION") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					sscanf( buffer, _T("%s %d %d"), tempbuffer, &(pSetupData->windowposition.x), &(pSetupData->windowposition.y) );
				}
			}
			else if( strncmp( buffer, _T("WINDOWSIZE"), strlen(_T("WINDOWSIZE") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					sscanf( buffer, _T("%s %d %d"), tempbuffer, &(pSetupData->windowsize.cx), &(pSetupData->windowsize.cy) );
				}
			}
			else if( strncmp( buffer, _T("INTERVAL"), strlen(_T("INTERVAL") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					sscanf( buffer, _T("%s %d"), tempbuffer, &(pSetupData->cyclinginterval) );

					unsigned int dataformat = gettype(pSetupData->contentstype);
					if(dataformat == 1000)
					{
						if( pSetupData->cyclinginterval > 1000 )
							pSetupData->cyclinginterval = 1000;
					}
				}
			}
			else if( strncmp( buffer, _T("DISPLAYTIME"), strlen(_T("DISPLAYTIME") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					sscanf( buffer, _T("%s %d"), tempbuffer, &(pSetupData->displaytime) );
				}
			}
		}
		
	}

	adresourcepackage.FinalizePackage();
	
	//_XLog( "Parsing complete : AD Script" );
	
	return TRUE;
}

BOOL _XIGAADWrapperCore::InitDeviceObject( void )
{

	return TRUE;
}

BOOL _XIGAADWrapperCore::RestoreDeviceObject( void )
{


	return TRUE;
}

void _XIGAADWrapperCore::ReleaseDeviceObject( void )
{
	SAFE_RELEASE( m_pTexture );

	if( m_pBinkWindowObject )
		m_pBinkWindowObject->BinkFinalize();
}

void _XIGAADWrapperCore::CreateAdWindow( void )
{
	if( m_ADZone == _XIGAADZONE_IDLE )
	{
		return;
	}

	_XIGAADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[ (int)m_ADZone-1 ];	

	if(!pSetupData)
	{
		return;
	}

	unsigned int dataformat = gettype(m_CurrentContentstype);

	m_WindowSize.cx = pSetupData->windowsize.cx/* + 8*/;
	m_WindowSize.cy = pSetupData->windowsize.cy/* + 20*/;

	if( dataformat == 1000 )
	{
		if(m_pBinkWindowObject && m_pBinkWindowObject->m_BinkObject)
		{
			//m_pBinkWindowObject->SetWindowSize( pSetupData->windowsize.cx, pSetupData->windowsize.cy );
		}
	}
	
	if( pSetupData->horizalignment == -1 && pSetupData->vertalignment == -1 )
	{
		m_WindowPosition.x = pSetupData->windowposition.x;
		m_WindowPosition.y = pSetupData->windowposition.y;
	}
	else 
	{
		if( pSetupData->horizalignment != -1 )
		{
			switch(pSetupData->horizalignment) 
			{
			case 0:		// left
				m_WindowPosition.x = 0;
				break;
			case 1:		// right
				m_WindowPosition.x = gnWidth - m_WindowSize.cx - 1;
				break;
			default:	// center				
			case 2:
				m_WindowPosition.x = (gnWidth>>1) - (m_WindowSize.cx>>1);
				break;			
			}
		}
		else
		{
			m_WindowPosition.x = pSetupData->windowposition.x;
		}

		if( pSetupData->vertalignment != -1 )
		{
			switch(pSetupData->vertalignment) 
			{
			case 0:		// top
				m_WindowPosition.y = 0;
				break;
			case 1:		// bottom
				m_WindowPosition.y = gnHeight - m_WindowSize.cy - 1;
				break;
			default:	// center				
			case 2:
				m_WindowPosition.y = (gnHeight>>1) - (m_WindowSize.cy>>1);
				break;			
			}			
		}		
		else
		{
			m_WindowPosition.y = pSetupData->windowposition.y;
		}
	}


	if( dataformat == 1000 )
	{
		if(m_pBinkWindowObject)
		{
			m_pBinkWindowObject->MoveWindow( m_WindowPosition.x + (m_WindowSize.cx>>1) - (pSetupData->windowsize.cx>>1), 
											 m_WindowPosition.y + ((m_WindowSize.cy)>>1) - (pSetupData->windowsize.cy>>1) );

			m_pBinkWindowObject->SetViewPortSize( pSetupData->windowsize.cx, pSetupData->windowsize.cy );
		}
	}

	int windowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Cash_Box2.tga" );
		
	m_TitleLeftBar.Create( m_WindowPosition.x-11, m_WindowPosition.y-14-21, m_WindowPosition.x-11+82, m_WindowPosition.y-14+44-21, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleLeftBar.SetClipRect( 48, 0, 130, 44 );

	m_TitleCenterBar.Create( m_WindowPosition.x, m_WindowPosition.y-21, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleCenterBar.SetClipRect( 224, 82, 256, 103 );

	FLOAT scalefactor = (FLOAT)m_WindowSize.cx / 32.0f;
	m_TitleCenterBar.SetScale( scalefactor, 1.0f );

	m_TitleRightBar.Create( m_WindowPosition.x+m_WindowSize.cx-71, m_WindowPosition.y-14-21, m_WindowPosition.x+m_WindowSize.cx-71+83, m_WindowPosition.y-14+44-21, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleRightBar.SetClipRect( 135, 0, 217, 44 );

	m_TitleBar.Create( m_WindowPosition.x+(m_WindowSize.cx>>1) - (164>>1), m_WindowPosition.y - 16-21, m_WindowPosition.x+(m_WindowSize.cx>>1) + (164>>1), m_WindowPosition.y+16+44-21, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleBar.SetClipRect( 51, 48, 215, 93 );

	m_BottomCenterBar.Create( m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy, m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy + 13, &g_MainInterfaceTextureArchive, windowimageindex );
	m_BottomCenterBar.SetClipRect( 156, 108, 256, 121 );
	scalefactor = (FLOAT)m_WindowSize.cx / 100.0f;
	m_BottomCenterBar.SetScale( scalefactor, 1.0f );
	
	m_LeftVertLine.Create( m_WindowPosition.x - 3, m_WindowPosition.y, m_WindowPosition.x, m_WindowPosition.y + m_WindowSize.cy, &g_MainInterfaceTextureArchive, windowimageindex );
	m_LeftVertLine.SetClipRect( 0, 0, 3, 100 );
	scalefactor = (FLOAT)(m_WindowSize.cy+13) / 100.0f;	
	m_LeftVertLine.SetScale( 1.0f, scalefactor );

	m_RightVertLine.Create( m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y, m_WindowPosition.x + m_WindowSize.cx + 3, m_WindowPosition.y + m_WindowSize.cy, &g_MainInterfaceTextureArchive, windowimageindex );
	m_RightVertLine.SetClipRect( 5, 0, 8, 100 );
	//scalefactor = ((FLOAT)m_WindowSize.cy / 100.0f;
	m_RightVertLine.SetScale( 1.0f, scalefactor );
	
	m_SubscriberImage.Create( 0, 0, 256, 256, &g_MainInterfaceTextureArchive, -1 );
}

void _XIGAADWrapperCore::Render( void )
{
	if( !m_ShowWindow ) return;

	if( m_ADZone == _XIGAADZONE_IDLE ) return;

	_XIGAADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[(int)m_ADZone-1];
	unsigned int dataformat = gettype(m_CurrentContentstype);

	/*
	if (dataformat == 100  || // JPEG
		dataformat == 200  || // BMP
		dataformat == 1200 || // PNG
		dataformat == 1100 )  // DDS
	{
		if( m_ImageSize.cx == 0 || m_ImageSize.cy == 0 )
		{
			return;
		}
	}
	*/


	// bink test
#ifdef _BINKTEST
	if( m_ADZone == _XIGAADZONE_MAPLOADINGVIDEOAD || m_ADZone == _XIGAADZONE_DEATHVIDEOAD )
		dataformat = 1000; 
#endif

	BOOL dataready = FALSE;
	if( dataformat == 1000 ) // Bink video
	{
		if( m_pBinkWindowObject && m_pBinkWindowObject->m_BinkObject )
		{
			dataready = TRUE;
		}
	}
	else if (dataformat == 100  || // JPEG
			 dataformat == 200  || // BMP
			 dataformat == 1200 || // PNG
			 dataformat == 1100 )  // DDS
	{
		if( m_pTexture && m_ImageSize.cx > 0 && m_ImageSize.cy > 0 )
			dataready = TRUE;
	}
	else
	{
		dataready = TRUE;
	}

	FLOAT uialphalevel = 0.0f;

	if( dataready )
	{
		int curtimer = g_LocalSystemTime;

		if( m_AlphaLevelDirection > 0 ) // Fade in/out
		{
			if( m_FrameFadeTimer == 0 )
			{ 
				m_FrameFadeTimer = curtimer;
			}
			
			if( curtimer - m_FrameFadeTimer > 20 )
			{
				m_FrameFadeTimer = curtimer;

				if( m_AlphaLevelDirection == 2 )
				{
					m_FrameAlphaLevel += 0.03f;
					if( m_FrameAlphaLevel > 1.0f )
					{
						m_FrameAlphaLevel = 1.0f;
						m_AlphaLevelDirection = 0; // stop

						m_DisplayTimeCounter = curtimer;

						if( dataformat == 1000 ) // Bink video
						{
							if( m_pBinkWindowObject && m_pBinkWindowObject->m_BinkObject )
							{
								m_pBinkWindowObject->m_PlayFlag = TRUE;
								BinkPause( m_pBinkWindowObject->m_BinkObject, FALSE );
							}
						}
					}			
				}
				else if( m_AlphaLevelDirection == 1 )
				{
					m_FrameAlphaLevel-= 0.03f;
					if( m_FrameAlphaLevel < 0.0f )
					{
						m_FrameAlphaLevel = 0.0f;
						m_AlphaLevelDirection = 0; // stop

						ShowWindow( FALSE );

						// 비디오 광고중 줄여놓은 볼륨을 원상복귀
						_XWindow_Option* pOption_Window = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
						if( pOption_Window )
						{
							if( pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND] )
							{
								if( ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pBGMVolumeSlider )
								{
									FLOAT volumegauge = ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pBGMVolumeSlider->m_GaugeValue / 100.0f;
									g_FMODWrapper.SetBGMMaxVolume( (BYTE)( 191.0f * volumegauge ) );
								}

								if( ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pEffectSoundVolumeSlider )
								{
									FLOAT volumegauge = ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pEffectSoundVolumeSlider->m_GaugeValue / 100.0f;
									g_FMODWrapper.SetVolume( (BYTE)( 255.0f * volumegauge ) );
								}

								if( ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pEnvSoundVolumeSlider )
								{
									FLOAT volumegauge = ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pEnvSoundVolumeSlider->m_GaugeValue / 100.0f;
									g_FMODWrapper.FL_SetVolume( (BYTE)( 255.0f * volumegauge ) );
								}
							}
						}

						return;
					}
				}		
			}
		}
		else
		{
			if( dataformat != 1000 ) // image fade
			{
				if( pSetupData->displaytime != -1 )
				{
					if( curtimer - m_DisplayTimeCounter >  pSetupData->displaytime )
					{
						SetFadeMode();				
					}
				}
			}

			m_FrameAlphaLevel = 1.0f;
		}

		uialphalevel = m_FrameAlphaLevel;
	}
	else
	{
		uialphalevel = 0.0f;
	}

	D3DCOLOR color;	
	int alphalevel = (int)( 255.0f * uialphalevel );
	if( alphalevel > 255 ) alphalevel = 255;
	if( alphalevel < 0   ) alphalevel = 0;
	color = D3DCOLOR_ARGB( alphalevel, 255, 255, 255 );

	BOOL shwowindowframe = m_ShowWindowFrame;

	if (dataformat == 100  || // JPEG
		dataformat == 200  || // BMP
		dataformat == 1200 || // PNG
		dataformat == 1100 )  // DDS
	{
		if( m_pTexture )
		{
			m_SubscriberImage.SetClipRect( 0, 0, m_ImageSize.cx, m_ImageSize.cy );

			if( pSetupData )
			{
				FLOAT fAspectRatio;

				FLOAT windowwidth  = 0.0f;
				FLOAT windowheight = 0.0f;
				if( m_UseSecondSize )
				{
					windowwidth	 = m_SecondWindowSize.cx;
					windowheight = m_SecondWindowSize.cy;
				}
				else
				{
					windowwidth	 = pSetupData->windowsize.cx;
					windowheight = pSetupData->windowsize.cy;
				}

				fAspectRatio = (FLOAT)windowheight/(FLOAT)m_ImageSize.cy; 
				if( (FLOAT)m_ImageSize.cx * fAspectRatio > (FLOAT)windowwidth )
				{
					fAspectRatio = (FLOAT)windowwidth/(FLOAT)m_ImageSize.cx;
				}

				int displaywidth  = (int)((FLOAT)m_ImageSize.cx*fAspectRatio );
				int displayheight = (int)((FLOAT)m_ImageSize.cy*fAspectRatio );

				m_SubscriberImage.SetScale( (FLOAT)displaywidth  / (FLOAT)m_ImageSize.cx,
											(FLOAT)displayheight / (FLOAT)m_ImageSize.cy );
				
				m_SubscriberImage.m_FColor = color;

				if( m_UseSecondSize )
				{
					_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x + m_SecondWindowSize.cx, m_WindowPosition.y + m_SecondWindowSize.cy, 
									(alphalevel<<24) /*==D3DCOLOR_ARGB( alphalevel, 0, 0, 0 )*/ );

					m_SubscriberImage.Draw( m_WindowPosition.x + (m_SecondWindowSize.cx>>1) - (displaywidth>>1), 
											m_WindowPosition.y + (m_SecondWindowSize.cy>>1) - (displayheight>>1), m_pTexture );
				}
				else
				{
					_XDrawSolidBar( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x + m_WindowSize.cx, m_WindowPosition.y + m_WindowSize.cy, 
									(alphalevel<<24) /*==D3DCOLOR_ARGB( alphalevel, 0, 0, 0 )*/ );

					m_SubscriberImage.Draw( m_WindowPosition.x + (m_WindowSize.cx>>1) - (displaywidth>>1), 
											m_WindowPosition.y + (m_WindowSize.cy>>1) - (displayheight>>1), m_pTexture );
				}
			}
		}
		else
		{
			shwowindowframe = FALSE;
			int centerx = m_WindowPosition.x + (m_WindowSize.cx>>1);
			int centery = m_WindowPosition.y + (m_WindowSize.cy>>1);

			int messageboxxpos = centerx - (146>>1);
			int messageboxypos = centery - (36>>1);
			
			m_DownloadMessageWindow.Draw( messageboxxpos, messageboxypos );

			if(m_IndicatorDirection) // forward
			{
				if( (gnFPS>>1) < gnFrameCounter )
				{
					if( m_IndicatorIndex == 2 )
					{
						m_IndicatorDirection = FALSE;
						m_IndicatorIndex=1;
					}
					else
					{
						m_IndicatorIndex++;
					}
				}
			}
			else // backward
			{
				if( (gnFPS>>1) )
				{
					if( m_IndicatorIndex == 0 )
					{
						m_IndicatorDirection = TRUE;
						m_IndicatorIndex=1;
					}
					else
					{
						m_IndicatorIndex--;
					}
				}
			}

			m_DownloadMessageIndicator.SetClipRect( 168 + (24*m_IndicatorIndex), 17, 192 + (24*m_IndicatorIndex), 34 );
			m_DownloadMessageIndicator.Draw( messageboxxpos+3, messageboxypos );
			_XDrawSolidBar( messageboxxpos + 8, messageboxypos+18, messageboxxpos+138, messageboxypos+32, 0xFF000000 );

			g_XBaseFont->SetColor( _XSC_DEFAULT_HIGHLIGHT );		
			g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + (m_WindowSize.cy>>1), _XFONT_ALIGNTYPE_CENTER, _T("Downloading...") );
			g_XBaseFont->Flush();
		}
	}	
	else if( dataformat == 1000 ) // Bink video
	{
		if( !m_pBinkWindowObject->m_BinkObject )
		{
			shwowindowframe = FALSE;
		}

		BOOL preveof = m_pBinkWindowObject->m_EndOfFrame;

		_XWindow_Option* pOption_Window = (_XWindow_Option*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_OPTION );
		if( pOption_Window )
		{
			if( pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND] )
			{
				if( ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pBGMVolumeSlider )
				{
					FLOAT volumegauge = ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pBGMVolumeSlider->m_GaugeValue / 100.0f;
					g_FMODWrapper.SetBGMMaxVolume( (BYTE)( (191.0f * volumegauge) * (1.0 - uialphalevel) ) );
				}
				
				if( ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pEffectSoundVolumeSlider )
				{
					FLOAT volumegauge = ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pEffectSoundVolumeSlider->m_GaugeValue / 100.0f;
					g_FMODWrapper.SetVolume( (BYTE)( (255.0f * volumegauge) * (1.0 - uialphalevel) ) );
				}

				if( ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pEnvSoundVolumeSlider )
				{
					FLOAT volumegauge = ((_XWindow_OptionSound*)pOption_Window->m_pSubWindow[_XOPTIONTAB_SOUND])->m_pEnvSoundVolumeSlider->m_GaugeValue / 100.0f;
					g_FMODWrapper.FL_SetVolume( (BYTE)( (255.0f * volumegauge) * (1.0 - uialphalevel) ) );
				}
			}
		}

		m_pBinkWindowObject->m_AlphaLevel = uialphalevel;
		m_pBinkWindowObject->Process();
		m_pBinkWindowObject->Draw();
		
		if( m_pBinkWindowObject->m_BinkObject )
		{
			if( m_pBinkWindowObject->m_BinkObject->FrameNum >= m_pBinkWindowObject->m_BinkObject->Frames )
			{
				if( m_AlphaLevelDirection != 1 )
				{
					BinkPause( m_pBinkWindowObject->m_BinkObject, TRUE );
					SetFadeMode();
				}
			}
		
			if( !preveof )
			{
				if( m_pBinkWindowObject->m_BinkObject && m_pBinkWindowObject->m_EndOfFrame )
				{
					if( m_AlphaLevelDirection != 1 )
					{
						BinkPause( m_pBinkWindowObject->m_BinkObject, TRUE );
						SetFadeMode();
					}
				}
			}
		}
	}
	
	if( shwowindowframe )
	{
		m_BottomCenterBar.m_FColor = m_LeftVertLine.m_FColor = m_RightVertLine.m_FColor = 
		m_TitleBar.m_FColor = m_TitleRightBar.m_FColor = m_TitleLeftBar.m_FColor = m_TitleCenterBar.m_FColor = color;
		
		m_BottomCenterBar.Draw();

		m_LeftVertLine.Draw();
		m_RightVertLine.Draw();

		m_TitleCenterBar.Draw();
		m_TitleLeftBar.Draw();
		m_TitleRightBar.Draw();
		m_TitleBar.Draw();
	}
}

void _XIGAADWrapperCore::Process( void )
{
	if( !m_ShowWindow ) return;

	if( m_ADZone == _XIGAADZONE_IDLE || m_NoMoreUpdate ) return;	

	DWORD curtime = timeGetTime();
	if( m_ProcessTimer == 0 )
	{
		m_ProcessTimer = curtime;
		this->Update();
		return;
	}

	_XIGAADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[(int)m_ADZone-1];
	
	if( curtime - m_ProcessTimer < pSetupData->cyclinginterval )
	{
		unsigned int dataformat = gettype(m_CurrentContentstype);

		if (dataformat == 100  || // JPEG
			dataformat == 200  || // BMP
			dataformat == 1200 || // PNG
			dataformat == 1100 )  // DDS
		{
			if( !( m_ImageSize.cx == 0 && m_ImageSize.cy == 0 ) ) // 이미지가 아직 다 안받아졌으면 계속 update
			{
				return;
			}
			
			m_ProcessTimer = curtime;
		}
		else
		{
			return;
		}
	}
	else
	{
		m_ProcessTimer = curtime;
	}

	this->Update();		
}

#endif //_ACCLAIM_IGAADSYSTEM


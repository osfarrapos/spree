// XFMODWrapper.cpp: implementation of the _XFMODWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XRegistryDef.h"
#include "XFMODWrapper.h"
#include "EmperorOfDragons.h"

#define _XFMOD_NUMCHANNELS		    32 //128
#define _XFSOUND_BUFFERSIZE			1000  /* millisecond value for FMOD buffersize. */
#define _XFSTREAM_BUFFERSIZE        1000

#define _XFSOUND_MAXINSTANCE		3
#define _XFFLXSOUND_MAXINSTANCE		1
#define _XFSTREAM_MAXINSTANCE		1

_XPackageArchive	g_MainSoundArchive;
_XFMODWrapper		g_FMODWrapper;

extern void _XInsertSystemMessage( int messagemode, TCHAR* args, ... );

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// user io callback functions

void *F_CALLBACKAPI _XFMOD_FileOpen(const char *name)
{
	int index = g_MainSoundArchive.FindResource( (LPTSTR)name );

	if( index == -1 )
	{
		return NULL;
	}

	_LPXPACKEDFILEHANDLE pHandle = new _XPACKEDFILEHANDLE;
	pHandle->fileid = index;
	pHandle->currentposition = 0;
	pHandle->fileoffset = 0;
	pHandle->filesize = 0;

	if( !g_MainSoundArchive.GetPackedFileInfo( index, pHandle->fileoffset, pHandle->filesize ) )
	{
		delete pHandle;
		return NULL;
	}

	return (void *)pHandle;
}

void F_CALLBACKAPI _XFMOD_FileClose(void *handle)
{
	delete handle;
}

int F_CALLBACKAPI _XFMOD_FileRead(void *buffer, int size, void *handle)
{
	_LPXPACKEDFILEHANDLE pHandle = (_LPXPACKEDFILEHANDLE)handle;
    FILE* pFilePtr = g_MainSoundArchive.GetPackedFile( pHandle->fileid, pHandle->currentposition );
	if( !pFilePtr ) 
	{
		return 0;
	}
		
	int readsize = fread(buffer, 1, size, pFilePtr);
	pHandle->currentposition = ftell( pFilePtr ) - pHandle->fileoffset;
	return readsize;
}

int F_CALLBACKAPI _XFMOD_FileSeek(void *handle, int pos, signed char mode)
{
	_LPXPACKEDFILEHANDLE pHandle = (_LPXPACKEDFILEHANDLE)handle;
	
	// rewind & get start file ptr
	FILE* pFilePtr = g_MainSoundArchive.GetPackedFile( pHandle->fileid );
	
	if( !pFilePtr ) 
	{
		return -1;
	}

	if( mode == SEEK_SET )
	{
		pHandle->currentposition = pos;
		if( pos > 0 )
		{
			return fseek( pFilePtr, pos, SEEK_CUR );
		}
		else 
		{
			return 0;
		}
	}
	else if( mode == SEEK_CUR )
	{
		pHandle->currentposition += pos;
		return fseek( pFilePtr, pHandle->currentposition, SEEK_CUR );
	}
	else  if( mode == SEEK_END )
	{
		pHandle->currentposition = pHandle->filesize;
		return fseek( pFilePtr, pHandle->filesize, SEEK_CUR );
	}

    return -1;
}

int F_CALLBACKAPI _XFMOD_FileTell(void *handle)
{
	_LPXPACKEDFILEHANDLE pHandle = (_LPXPACKEDFILEHANDLE)handle;
    return pHandle->currentposition;
}


signed char F_CALLBACKAPI _XFMOD_StreamEndCallBack(FSOUND_STREAM *stream, void *buff, int len, void *param)
{
    // end of stream callback doesnt have a 'buff' value, if it doesnt it could be a synch point.

	int iiii = 0;

    if (buff)
    {		
        //printf("\nSYNCHPOINT : \"%s\"\n", buff);
    }
    else
    {
		//g_FMODWrapper.m_StopedBGM = TRUE;        
    }
	
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FMOD Wrapper base methods

_XFMODWrapper::_XFMODWrapper()
{
	m_SetupParameter.SoundOutputMethod		=	FSOUND_OUTPUT_DSOUND;
	m_SetupParameter.SoundDriver			=	0;
	m_SetupParameter.SoundMixrate			=	44100;
	m_SetupParameter.MaxSoftWareChannels	=	_XFMOD_NUMCHANNELS;
	//m_SetupParameter.Flags					=	0;
	m_SetupParameter.Flags					=	/*FSOUND_INIT_STREAM_FROM_MAIN_THREAD |*/ FSOUND_INIT_GLOBALFOCUS | FSOUND_INIT_DSOUND_DEFERRED;
	m_SetupParameter.SoundBufferSize		=	_XFSOUND_BUFFERSIZE;
	m_SetupParameter.StreamingBufferSize	=	_XFSTREAM_BUFFERSIZE;
	m_SetupParameter.MixerTypes				=	FSOUND_MIXER_QUALITY_AUTODETECT;

	m_3DMinDistance							=	8.2f;
	m_3DMaxDistance							=	10000.0f;

	m_LastListenerPosition					=	D3DXVECTOR3( 0.0f,0.0f,0.0f );
	m_CurentListenerPosition				=	D3DXVECTOR3( 0.0f,0.0f,0.0f );

	m_ListnerForwardOrientVector			=	D3DXVECTOR3( 0.0f,0.0f,1.0f );
	m_ListnerTopOrientVector				=	D3DXVECTOR3( 0.0f,1.0f,0.0f );

	m_2DSoundMaxInstanceCount				=	1;
	m_SoundMaxInstanceCount					=	_XFSOUND_MAXINSTANCE;
	m_SoundFlxMaxInstanceCount				=	_XFFLXSOUND_MAXINSTANCE;
	m_StreanMaxInstanceCount				=	_XFSTREAM_MAXINSTANCE;
	
	m_pBGMStreamObject						=   NULL;
	m_ChangeBGM								=   FALSE;
	m_StopedBGM								=	TRUE;
	m_BGMChannel							=	-1;
	m_BGMVolume								=	0.0f;
	m_BGMMaxVolume							=	255.0f;

	m_LoadedSoundSampleCount				=	0;
	m_LoadedFLSoundSampleCount				=	0;
	
	memset( m_CurrentBGMName, 0, sizeof(TCHAR) * 64 );
	memset( m_NextBGMName, 0, sizeof(TCHAR) * 64 );	

	m_FLSoundProcessTimer					=   0;
}

_XFMODWrapper::~_XFMODWrapper()
{
	Release();
}

void _XFMODWrapper::LoadSettings( void )
{
	int SoundOutPutType			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDOUTPUTTYPE, 0, TRUE );
	m_SetupParameter.SoundDriver= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDDRIVERTYPE, 0, TRUE );
	int SoundMixRate			= g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDMIXRATE, 0, TRUE );

	switch( SoundOutPutType ) 
	{
	default:
	case 0 :	m_SetupParameter.SoundOutputMethod = FSOUND_OUTPUT_DSOUND;
		break;
	case 1 :	m_SetupParameter.SoundOutputMethod = FSOUND_OUTPUT_WINMM;
		break;	
	}

	switch(SoundMixRate) 
	{
	default:
	case 0 : m_SetupParameter.SoundMixrate = 44100;
		break;
	case 1 : m_SetupParameter.SoundMixrate = 22050;
		break;
	case 2 : m_SetupParameter.SoundMixrate = 11025;
		break;	
	}
}


BOOL _XFMODWrapper::Initialize( void )
{
	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Sound\\") );

#ifdef _XTESTSERVER	
	strcat( packagefilename, _T("tsSR_SOUND.XP") );
#else
	strcat( packagefilename, _T("SR_SOUND.XP") );
#endif

	if( !g_MainSoundArchive.OpenPackage( packagefilename ) )
	{
		return FALSE;
	}

	// Initialize ===================================================================================================
	_XLog("========================================================================" );		       
	_XLog( "FMOD : Initialize FMOD Sound system" );
	_XLog("========================================================================" );

	// Load registry setting ----------------------------------------------------------------------------------------
	LoadSettings();

	if(m_SetupParameter.SoundMixrate < 4000) 
    {
		m_SetupParameter.SoundMixrate = 44100;
    }

	// Check DLL version ---------------------------------------------------------------------------------------------
	if (FSOUND_GetVersion() < FMOD_VERSION)
    {
        _XFatalError( "FMOD : You are using the wrong DLL version!  You should be using FMOD %.02f", FMOD_VERSION );
        return FALSE;
    }
	else
	{
		_XLog( "FMOD : Use FMOD Sound system [%.02f]", FMOD_VERSION );
	}

	// Set sound output method	---------------------------------------------------------------------------------------
	FSOUND_SetOutput(m_SetupParameter.SoundOutputMethod);
	switch (FSOUND_GetOutput())
    {
	case FSOUND_OUTPUT_NOSOUND: _XLog("FMOD : Using [NoSound] output method"); break;
	case FSOUND_OUTPUT_WINMM:   _XLog("FMOD : Using [Windows Multimedia Waveout] output method"); break;
	case FSOUND_OUTPUT_DSOUND:  _XLog("FMOD : Using [Direct Sound] output method"); break;
	case FSOUND_OUTPUT_ASIO:    _XLog("FMOD : Using [ASIO] output method"); break;
    };

	// Set custom io callback  ----------------------------------------------------------------------------------------
	FSOUND_File_SetCallbacks( _XFMOD_FileOpen, _XFMOD_FileClose, _XFMOD_FileRead, _XFMOD_FileSeek, _XFMOD_FileTell );


	// Set sound driver -----------------------------------------------------------------------------------------------
	_XLog( "FMOD : Sound driver list" );
	for (int i=0; i < FSOUND_GetNumDrivers(); i++) 
    {
        _XLog ("	%d - %s", i+1, FSOUND_GetDriverName(i));    // print driver names
        {
            unsigned int caps = 0;			
            FSOUND_GetDriverCaps(i, &caps);            
            if (caps & FSOUND_CAPS_HARDWARE)
                _XLog ("	Driver supports hardware 3D sound !\n");
            if (caps & FSOUND_CAPS_EAX2)
                _XLog ("	Driver supports EAX 2 reverb !\n");
            if (caps & FSOUND_CAPS_EAX3)
                _XLog ("	Driver supports EAX 3 reverb !\n");
        }
    }

	if( m_SetupParameter.SoundDriver >= FSOUND_GetNumDrivers() )
	{
		m_SetupParameter.SoundDriver = 0;
	}

	// Set driver... 
	FSOUND_SetDriver(m_SetupParameter.SoundDriver); // Select sound card (0 = default)
			
	unsigned int caps = 0;	
	FSOUND_GetDriverCaps(FSOUND_GetDriver(), &caps);
	_XLog( "FMOD : Use Driver capabilities [%s]", FSOUND_GetDriverName(m_SetupParameter.SoundDriver) );
	if (!caps)
	{
		_XLog("	This driver will support software mode only." );
		_XLog("	It does not properly support 3D sound hardware.");
	}
	if (caps & FSOUND_CAPS_HARDWARE)
	{
		_XLog("		Driver supports hardware 3D sound !");
	}
	if (caps & FSOUND_CAPS_EAX2)
	{
		_XLog("		supports EAX 2 reverb !");
	}
	if (caps & FSOUND_CAPS_EAX3)
	{
		_XLog("		supports EAX 3 reverb !");
	}

	// Set mixer -----------------------------------------------------------------------------------------------------
	_XLog("FMOD : Setting mixer");
	FSOUND_SetMixer( m_SetupParameter.MixerTypes );
	switch (FSOUND_GetMixer())
    {
	case FSOUND_MIXER_BLENDMODE:     _XLog("FMOD : FSOUND_MIXER_BLENDMODE"); break;
	case FSOUND_MIXER_MMXP5:         _XLog("FMOD : FSOUND_MIXER_MMXP5"); break;
	case FSOUND_MIXER_MMXP6:         _XLog("FMOD : FSOUND_MIXER_MMXP6"); break;
	case FSOUND_MIXER_QUALITY_FPU:   _XLog("FMOD : FSOUND_MIXER_QUALITY_FPU"); break;
	case FSOUND_MIXER_QUALITY_MMXP5: _XLog("FMOD : FSOUND_MIXER_QUALITY_MMXP5"); break;
	case FSOUND_MIXER_QUALITY_MMXP6: _XLog("FMOD : FSOUND_MIXER_QUALITY_MMXP6"); break;
    };

	// Set sound & streamming buffer size -----------------------------------------------------------------------------	
	FSOUND_Stream_SetBufferSize(_XFSTREAM_BUFFERSIZE);
	FSOUND_SetBufferSize(m_SetupParameter.SoundBufferSize);
	_XLog( "FMOD : Set buffer size [Sound:%d][Stream:%d]", m_SetupParameter.SoundBufferSize, m_SetupParameter.StreamingBufferSize );

	int retrycount = 0;
	while (!FSOUND_Init(m_SetupParameter.SoundMixrate, m_SetupParameter.MaxSoftWareChannels, m_SetupParameter.Flags) && retrycount < 10) 
	{
		Sleep(100);
		retrycount++;	
		_XLog( "WARNING : Failed FMOD initialize [retry cnt : %d]", retrycount );
	}
	
	if (retrycount == 10)
	{
		_XFatalError("FMOD : Initialize error [%s]", FMOD_ErrorString(FSOUND_GetError()));
        return FALSE;
	}
	
	// Report hardware channels
	int num2d = 0, num3d = 0;	
    FSOUND_GetNumHWChannels(&num2d, &num3d, NULL);    
	_XLog("FMOD : Hardware channel report" );
    _XLog("		  2D channels : %d", num2d);
    _XLog("		  3D channels : %d", num3d);


	FSOUND_3D_SetDistanceFactor( 1.0f );

	m_SoundVolume = 0xFF;
	m_FLSoundVolume = 0xFF;
	m_StreamVolume = 0xFF;

	_XLog("========================================================================" );		       

	return TRUE;
}


void _XFMODWrapper::Release( void )
{
	svdef_XSOUNDMODULE::iterator it;
	for (it = m_svSoundModuleList.begin(); it != m_svSoundModuleList.end(); ++ it )
	{
		FSOUND_Sample_Free( (*it)->psoundobject );
	}	
	std::for_each(m_svSoundModuleList.begin(), m_svSoundModuleList.end(), DELETEOBJECT());
	m_svSoundModuleList.clear();

	for (it = m_svSoundModuleFlexibleList.begin(); it != m_svSoundModuleFlexibleList.end(); ++ it )
	{
		FSOUND_Sample_Free( (*it)->psoundobject );
	}
	std::for_each(m_svSoundModuleFlexibleList.begin(), m_svSoundModuleFlexibleList.end(), DELETEOBJECT());
	m_svSoundModuleFlexibleList.clear();	

	svdef_XSTREAMMODULE::iterator streamit;
	for (streamit = m_svStreamModuleList.begin(); streamit != m_svStreamModuleList.end(); ++ streamit )
	{
		if( (*streamit)->pstreamobject )
		{
			FSOUND_Stream_Stop( (*streamit)->pstreamobject );
			FSOUND_Stream_Close( (*streamit)->pstreamobject );
		}
	}

	std::for_each(m_svStreamModuleList.begin(), m_svStreamModuleList.end(), DELETEOBJECT());
	m_svStreamModuleList.clear();

	FSOUND_Stream_Stop( m_pBGMStreamObject );
	FSOUND_Stream_Close( m_pBGMStreamObject );
	m_pBGMStreamObject = NULL;
	m_StopedBGM = TRUE;
	m_ChangeBGM = FALSE;
	m_BGMVolume = 0.0f;

	m_LoadedSoundSampleCount = 0;
	m_LoadedFLSoundSampleCount = 0;
	
	FSOUND_Close();	

	g_MainSoundArchive.FinalizePackage();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 3D Sound methods

void _XFMODWrapper::SetListenerAttribute( D3DXVECTOR3& position, D3DXVECTOR3& forwardvector, D3DXVECTOR3& topvector )
{
	
	// vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s).
	D3DXVECTOR3 velocity;

	m_CurentListenerPosition = position;	
	velocity.x = ( position.x - m_LastListenerPosition.x ) * (1.0f / g_fElapsedFrameTime);
	velocity.y = ( position.y - m_LastListenerPosition.y ) * (1.0f / g_fElapsedFrameTime);
	velocity.z = ( position.z - m_LastListenerPosition.y ) * (1.0f / g_fElapsedFrameTime);
	// store pos for next time
	m_LastListenerPosition = m_CurentListenerPosition;
	
	FSOUND_3D_Listener_SetAttributes( position, velocity, 
									  forwardvector.x, forwardvector.y, forwardvector.z, 
									  topvector.x, topvector.y, topvector.z );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sound sample methods

int	_XFMODWrapper::InsertSound( LPCTSTR szfilename, DWORD mode )
{
	return InsertSound( szfilename, mode, m_3DMinDistance, m_3DMaxDistance );
}

int	_XFMODWrapper::InsertSound( LPCTSTR szfilename, DWORD mode, FLOAT mindist, FLOAT maxdist )
{

#ifdef _XDWDEBUG			
	if( g_MainSoundArchive.FindResource( szfilename ) < 0 )
	{
		_XLog( "ERROR : Sound file not found [%s]", szfilename );
		//_XInsertSystemMessage( 0, "ERROR : Sound file not found [%s]", szfilename );
	}
#endif

	svdef_XSOUNDMODULE::iterator it;
	
	int index = 0;
	for (it = m_svSoundModuleList.begin(); it != m_svSoundModuleList.end(); ++ it, index++ )
	{
		if (stricmp((*it)->filename, szfilename) == 0)
		{
			return index;
		}
	}
	
	_LPXSOUNDMODULE pNewSound = new _XSOUNDMODULE;
	
	memset( pNewSound->filename, 0, sizeof(TCHAR) * 32 );	
	strcpy( pNewSound->filename, szfilename );
	pNewSound->psoundobject = NULL;
	pNewSound->modeflag		= mode;
	pNewSound->mindistance	= mindist;
	pNewSound->maxdistance	= maxdist;
	pNewSound->playreferencecounter = 0;
	pNewSound->channel      = -1;
	pNewSound->lastusedtime	= 0;
	
	m_svSoundModuleList.push_back( pNewSound );

	return (m_svSoundModuleList.size() - 1);
}


int _XFMODWrapper::FindSound( LPCTSTR szfilename )
{
	svdef_XSOUNDMODULE::iterator it;
	
	int index = 0;
	for (it = m_svSoundModuleList.begin(); it != m_svSoundModuleList.end(); ++ it, index++ )
	{
		if (stricmp((*it)->filename, szfilename) == 0)
		{
			return index;
		}
	}

	return -1;
}

_LPXSOUNDMODULE	_XFMODWrapper::GetSoundObject( int index )
{
	if( index < 0 || index >= m_svSoundModuleList.size() ) return NULL;	
	return m_svSoundModuleList[index];
}

void _XFMODWrapper::UpdateReferenceCount( _LPXSOUNDMODULE	pSoundModule )
{
	pSoundModule->playreferencecounter = 0;
	for( int i = 0; i < FSOUND_GetMaxChannels(); i++ )
	{
		if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( i ) )
		{
			pSoundModule->playreferencecounter++;
		}
	}	
}

void _XFMODWrapper::PlaySound2D( int index )
{
	if( index < 0 || index >= m_svSoundModuleList.size() ) return;
	
	_LPXSOUNDMODULE pSoundModule = m_svSoundModuleList[index];
	
	// Load sound in runtime...
	if( !pSoundModule->psoundobject )
	{
		pSoundModule->psoundobject = FSOUND_Sample_Load( FSOUND_FREE, pSoundModule->filename, pSoundModule->modeflag, 0, 0);
		if( !pSoundModule->psoundobject )
		{
#ifdef _XDWDEBUG			
			_XInsertSystemMessage( 0, "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()) );
			//_XFatalError( "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()));
#endif
			return;
		}

		m_LoadedSoundSampleCount++;

		// Set max playbacks
		FSOUND_Sample_SetMaxPlaybacks( pSoundModule->psoundobject, m_2DSoundMaxInstanceCount );
	}
	
	pSoundModule->channel = FSOUND_PlaySound(FSOUND_FREE, pSoundModule->psoundobject);
	if( pSoundModule->channel > -1 )
	{
		FSOUND_SetVolume( pSoundModule->channel, m_SoundVolume );
	}

	pSoundModule->lastusedtime = g_LocalSystemTime;
}

void _XFMODWrapper::PlaySound3D( int index, D3DXVECTOR3& position, bool loop )
{
	if( index < 0 || index >= m_svSoundModuleList.size() ) return;

	_LPXSOUNDMODULE pSoundModule = m_svSoundModuleList[index];

	// Load sound in runtime...
	if( !pSoundModule->psoundobject )
	{
		pSoundModule->psoundobject = FSOUND_Sample_Load( FSOUND_FREE, pSoundModule->filename, pSoundModule->modeflag, 0, 0);
		if( !pSoundModule->psoundobject )
		{
#ifdef _XDWDEBUG
			_XInsertSystemMessage( 0, "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()) );
			//_XFatalError( "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()));			
#endif
			return;
		}
		
		m_LoadedSoundSampleCount++;

		if( pSoundModule->modeflag & FSOUND_HW3D )
		{
			// increasing mindistnace makes it louder in 3d space
			FSOUND_Sample_SetMinMaxDistance( pSoundModule->psoundobject, pSoundModule->mindistance, pSoundModule->maxdistance );			
		}		

		// Set max playbacks
		FSOUND_Sample_SetMaxPlaybacks( pSoundModule->psoundobject, m_SoundMaxInstanceCount );
	}
	
	FSOUND_Sample_SetMode( pSoundModule->psoundobject, (loop) ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF );
	
	pSoundModule->channel = FSOUND_PlaySoundEx(FSOUND_FREE, pSoundModule->psoundobject, NULL, TRUE);
	if( pSoundModule->channel > -1 )
	{
		float vel[3] = { 0,0,0 };
		FSOUND_3D_SetAttributes(pSoundModule->channel, position, vel);

		if (!FSOUND_SetPaused(pSoundModule->channel, FALSE))
		{
			//_XFatalError( "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()));
			_XLog( "FMOD WARNING %s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()));
			return;
		}

		FSOUND_SetVolume( pSoundModule->channel, m_SoundVolume );
	}	

	pSoundModule->lastusedtime = g_LocalSystemTime;
}

void _XFMODWrapper::StopSound( int index )
{
	if( index < 0 || index >= m_svSoundModuleList.size() ) return;
	
	_LPXSOUNDMODULE pSoundModule = m_svSoundModuleList[index];
	
	if( pSoundModule->psoundobject )
	{
		if( (pSoundModule->channel > -1) && FSOUND_IsPlaying( pSoundModule->channel ) )
		{
			if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
			{
				FSOUND_StopSound( pSoundModule->channel );				
				FSOUND_Sample_Free( pSoundModule->psoundobject );
			}
			
			pSoundModule->channel = -1;
		}
	}
}

void _XFMODWrapper::StopAllSound( void )
{
	svdef_XSOUNDMODULE::iterator it;
	
	for (it = m_svSoundModuleList.begin(); it != m_svSoundModuleList.end(); ++ it )
	{
		_LPXSOUNDMODULE pSoundModule = (*it);
		
		if( (pSoundModule->channel > -1) && FSOUND_IsPlaying( pSoundModule->channel ) )
		{
			if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
			{
				FSOUND_StopSound( pSoundModule->channel );
			}			
			pSoundModule->channel = -1;
		}
	}
}

void _XFMODWrapper::SetVolume( BYTE volume )
{
	m_SoundVolume = volume;

	svdef_XSOUNDMODULE::iterator it;
	
	int index = 0;
	for (it = m_svSoundModuleList.begin(); it != m_svSoundModuleList.end(); ++ it, index++ )
	{
		_LPXSOUNDMODULE pSoundModule = (*it);

		if( FSOUND_IsPlaying( pSoundModule->channel ) )
		{
			if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
			{
				FSOUND_SetVolume( pSoundModule->channel, m_SoundVolume );
			}			
		}
	}
}

BYTE _XFMODWrapper::GetVolume( void )
{
	return m_SoundVolume;
}

static const int _XDEF_SOUNDHITRATECHECKTIME = 30000;

void _XFMODWrapper::CheckHitrate( void )
{
	if( m_LastHitrateCheckTime != 0 )
	{
		if( g_LocalSystemTime - m_LastHitrateCheckTime < _XDEF_SOUNDHITRATECHECKTIME )
		{
			return;
		}
	}
	else
	{
		m_LastHitrateCheckTime = g_LocalSystemTime;
		return;
	}

	svdef_XSOUNDMODULE::iterator it;
	
	for (it = m_svSoundModuleList.begin(); it != m_svSoundModuleList.end(); ++ it )
	{
		_LPXSOUNDMODULE pSoundModule = (*it);
		
		if( pSoundModule->psoundobject )
		{
			if( !FSOUND_IsPlaying( pSoundModule->channel ) )
			{
				if( pSoundModule->lastusedtime > 0 )
				{
					if( g_LocalSystemTime - pSoundModule->lastusedtime > _XDEF_SOUNDHITRATECHECKTIME )
					{
						if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
						{
							if( FSOUND_StopSound( pSoundModule->channel ) )
							{
								FSOUND_Sample_Free( pSoundModule->psoundobject );

								pSoundModule->psoundobject = NULL;
								pSoundModule->channel = -1;
								m_LoadedSoundSampleCount--;
							}
						}						
					}
				}
			}
		}
	}

	for (it = m_svSoundModuleFlexibleList.begin(); it != m_svSoundModuleFlexibleList.end(); ++ it )
	{
		_LPXSOUNDMODULE pSoundModule = (*it);
		
		if( pSoundModule->psoundobject )
		{
			if( !FSOUND_IsPlaying( pSoundModule->channel ) )
			{
				if( pSoundModule->lastusedtime > 0 )
				{
					if( g_LocalSystemTime - pSoundModule->lastusedtime > _XDEF_SOUNDHITRATECHECKTIME )
					{
						if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
						{
							if( FSOUND_StopSound( pSoundModule->channel ) )
							{
								FSOUND_Sample_Free( pSoundModule->psoundobject );
								pSoundModule->psoundobject = NULL;
								pSoundModule->channel = -1;
								m_LoadedFLSoundSampleCount--;
							}
						}
					}
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Flexible list : Sound sample methods

void _XFMODWrapper::FL_ReleaseFlexibleList( void )
{
	svdef_XSOUNDMODULE::iterator it;
	for (it = m_svSoundModuleFlexibleList.begin(); it != m_svSoundModuleFlexibleList.end(); ++ it )
	{
		FSOUND_Sample_Free( (*it)->psoundobject );
	}

	std::for_each(m_svSoundModuleFlexibleList.begin(), m_svSoundModuleFlexibleList.end(), DELETEOBJECT());
	m_svSoundModuleFlexibleList.clear();

	m_FLSoundProcessTimer = 0;
}

int	_XFMODWrapper::FL_InsertSound( LPCTSTR szfilename, DWORD mode )
{
	return InsertSound( szfilename, mode, m_3DMinDistance, m_3DMaxDistance );
}

int	_XFMODWrapper::FL_InsertSound( LPCTSTR szfilename, DWORD mode, FLOAT mindist, FLOAT maxdist )
{
#ifdef _XDWDEBUG			
	if( g_MainSoundArchive.FindResource( szfilename ) < 0 )
	{
		_XLog( "ERROR : Sound file not found [%s]", szfilename );
		//_XInsertSystemMessage( 0, "ERROR : Sound file not found [%s]", szfilename );
	}
#endif

	svdef_XSOUNDMODULE::iterator it;
	
	int index = 0;
	for (it = m_svSoundModuleFlexibleList.begin(); it != m_svSoundModuleFlexibleList.end(); ++ it, index++ )
	{
		if (stricmp((*it)->filename, szfilename) == 0)
		{
			return index;
		}
	}
	
	_LPXSOUNDMODULE pNewSound = new _XSOUNDMODULE;
	
	memset( pNewSound->filename, 0, sizeof(TCHAR) * 32 );	
	strcpy( pNewSound->filename, szfilename );
	pNewSound->psoundobject = NULL;
	pNewSound->modeflag		= mode;
	pNewSound->mindistance	= mindist;
	pNewSound->maxdistance	= maxdist;
	pNewSound->playreferencecounter = 0;
	pNewSound->channel      = -1;
	pNewSound->fademode		= _XDEF_FLSOUNDFADEMODE_IDLE;
	
	m_svSoundModuleFlexibleList.push_back( pNewSound );

	return (m_svSoundModuleFlexibleList.size() - 1);
}


int _XFMODWrapper::FL_FindSound( LPCTSTR szfilename )
{
	svdef_XSOUNDMODULE::iterator it;
	
	int index = 0;
	for (it = m_svSoundModuleFlexibleList.begin(); it != m_svSoundModuleFlexibleList.end(); ++ it, index++ )
	{
		if (stricmp((*it)->filename, szfilename) == 0)
		{
			return index;
		}
	}

	return -1;
}

_LPXSOUNDMODULE	_XFMODWrapper::FL_GetSoundObject( int index )
{
	if( index < 0 || index >= m_svSoundModuleFlexibleList.size() ) return NULL;	
	return m_svSoundModuleFlexibleList[index];
}

void _XFMODWrapper::FL_PlaySound2D( int index )
{
	if( index < 0 || index >= m_svSoundModuleFlexibleList.size() ) return;
	
	_LPXSOUNDMODULE pSoundModule = m_svSoundModuleFlexibleList[index];
	
	// Load sound in runtime...
	if( !pSoundModule->psoundobject )
	{
		pSoundModule->psoundobject = FSOUND_Sample_Load( FSOUND_FREE, pSoundModule->filename, pSoundModule->modeflag, 0, 0);
		if( !pSoundModule->psoundobject )
		{
#ifdef _XDWDEBUG
			_XInsertSystemMessage( 0, "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()) );
			//_XFatalError( "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()));
#endif
			return;
		}

		m_LoadedFLSoundSampleCount++;
		
		// Set max playbacks
		FSOUND_Sample_SetMaxPlaybacks( pSoundModule->psoundobject, m_2DSoundMaxInstanceCount );		
	}
	
	pSoundModule->channel = FSOUND_PlaySound(FSOUND_FREE, pSoundModule->psoundobject);

	if( pSoundModule->channel > -1 )
	{
		//FSOUND_SetVolume( pSoundModule->channel, 0 );
		FSOUND_SetVolume( pSoundModule->channel, m_FLSoundVolume );
	}

	pSoundModule->lastusedtime = g_LocalSystemTime;
}

void _XFMODWrapper::FL_PlaySound3D( int index, D3DXVECTOR3& position, bool loop )
{
	if( index < 0 || index >= m_svSoundModuleFlexibleList.size() ) return;

	_LPXSOUNDMODULE pSoundModule = m_svSoundModuleFlexibleList[index];
	
	if( pSoundModule->channel > -1 && FSOUND_IsPlaying( pSoundModule->channel ) ) return;

	// Load sound in runtime...
	if( !pSoundModule->psoundobject )
	{
		pSoundModule->psoundobject = FSOUND_Sample_Load( FSOUND_FREE, pSoundModule->filename, pSoundModule->modeflag, 0, 0);
		if( !pSoundModule->psoundobject )
		{
#ifdef _XDWDEBUG
			_XInsertSystemMessage( 0, "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()) );
			//_XFatalError( "%s : %s\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()));			
#endif
			return;
		}
		
		m_LoadedFLSoundSampleCount++;

		if( pSoundModule->modeflag & FSOUND_HW3D )
		{
			// increasing mindistnace makes it louder in 3d space
			FSOUND_Sample_SetMinMaxDistance( pSoundModule->psoundobject, pSoundModule->mindistance, pSoundModule->maxdistance );			
		}		

		// Set max playbacks
		FSOUND_Sample_SetMaxPlaybacks( pSoundModule->psoundobject, m_SoundFlxMaxInstanceCount );
	}
	
	FSOUND_Sample_SetMode( pSoundModule->psoundobject, (loop) ? FSOUND_LOOP_NORMAL : FSOUND_LOOP_OFF );

	pSoundModule->channel = FSOUND_PlaySoundEx(FSOUND_FREE, pSoundModule->psoundobject, NULL, TRUE);

	if( pSoundModule->channel > -1 )
	{	
		float vel[3] = { 0,0,0 };
		FSOUND_3D_SetAttributes(pSoundModule->channel, position, vel);

		if (!FSOUND_SetPaused(pSoundModule->channel, FALSE))
		{
			//_XFatalError( "Play FL_Sound [ %s : %s ]\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()));
			_XLog( "Play FL_Sound [ %s : %s ]\n", pSoundModule->filename, FMOD_ErrorString(FSOUND_GetError()));
			//_XLog( "Channel : %d, Pos : %f %f %f, Velocity : %f %f %f", pSoundModule->channel, position.x, position.y, position.z, vel[0],vel[1],vel[2] );
		}
		
		FSOUND_SetVolume( pSoundModule->channel, 0 );
		pSoundModule->fademode		= _XDEF_FLSOUNDFADEMODE_FADEOUT;
		//FSOUND_SetVolume( pSoundModule->channel, m_FLSoundVolume );
	}

	pSoundModule->lastusedtime = g_LocalSystemTime;
}

void _XFMODWrapper::FL_FadeinSound( int index )
{
	if( index < 0 || index >= m_svSoundModuleFlexibleList.size() ) return;
	
	_LPXSOUNDMODULE pSoundModule = m_svSoundModuleFlexibleList[index];
	
	if( pSoundModule->psoundobject )
	{
		if( (pSoundModule->channel > -1) /*&& FSOUND_IsPlaying( pSoundModule->channel )*/ )
		{
			if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
			{
				pSoundModule->fademode = _XDEF_FLSOUNDFADEMODE_FADEIN;

				//FSOUND_StopSound( pSoundModule->channel );
			}
		}
	}
}

void _XFMODWrapper::FL_ProcessFLSound( void )
{
	if( m_FLSoundProcessTimer == 0 )
	{
		m_FLSoundProcessTimer = g_LocalSystemTime;
	}
	else
	{
		if( g_LocalSystemTime - m_FLSoundProcessTimer > 80 )
		{
			m_FLSoundProcessTimer = g_LocalSystemTime;
		}
		else
		{
			return;
		}
	}

	svdef_XSOUNDMODULE::iterator it;
	
	for (it = m_svSoundModuleFlexibleList.begin(); it != m_svSoundModuleFlexibleList.end(); ++ it )
	{
		_LPXSOUNDMODULE pSoundModule = (*it);
		
		if( pSoundModule->fademode != _XDEF_FLSOUNDFADEMODE_IDLE )
		{
			if( (pSoundModule->channel > -1) /*&& FSOUND_IsPlaying( pSoundModule->channel )*/ )
			{
				if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
				{
					int _volume = FSOUND_GetVolume( pSoundModule->channel );

					if( pSoundModule->fademode == _XDEF_FLSOUNDFADEMODE_FADEIN )
					{
						if( _volume > 0 )
						{
							_volume-=3;
							if(_volume < 0 ) 
							{
								_volume = 0;
								pSoundModule->fademode = _XDEF_FLSOUNDFADEMODE_IDLE;
								FSOUND_SetVolume(  pSoundModule->channel, _volume );
								FSOUND_StopSound( pSoundModule->channel );
							}
							else
							{
								FSOUND_SetVolume(  pSoundModule->channel, _volume );
							}							
						}
						else
						{
							pSoundModule->fademode = _XDEF_FLSOUNDFADEMODE_IDLE;
							FSOUND_SetVolume(  pSoundModule->channel, 0 );
							FSOUND_StopSound( pSoundModule->channel );
						}
					}
					else if( pSoundModule->fademode == _XDEF_FLSOUNDFADEMODE_FADEOUT )
					{
						if( _volume < m_FLSoundVolume )
						{
							_volume+=3;
							if(_volume > m_FLSoundVolume ) 
							{
								_volume = m_FLSoundVolume;
								pSoundModule->fademode = _XDEF_FLSOUNDFADEMODE_IDLE;
							}
							FSOUND_SetVolume(  pSoundModule->channel, _volume );
						}
						else
						{
							pSoundModule->fademode = _XDEF_FLSOUNDFADEMODE_IDLE;
							FSOUND_SetVolume(  pSoundModule->channel, m_FLSoundVolume );
						}
					}
				}
			}
		}
	}
}

void _XFMODWrapper::FL_StopSound( int index )
{
	if( index < 0 || index >= m_svSoundModuleFlexibleList.size() ) return;
	
	_LPXSOUNDMODULE pSoundModule = m_svSoundModuleFlexibleList[index];

	if( pSoundModule->psoundobject )
	{
		if( (pSoundModule->channel > -1) /*&& FSOUND_IsPlaying( pSoundModule->channel )*/ )
		{
			if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
			{
				FSOUND_StopSound( pSoundModule->channel );
			}
			
			pSoundModule->channel = -1;
		}
	}
}

void _XFMODWrapper::FL_StopAllSound( void )
{
	svdef_XSOUNDMODULE::iterator it;
	
	for (it = m_svSoundModuleFlexibleList.begin(); it != m_svSoundModuleFlexibleList.end(); ++ it )
	{
		_LPXSOUNDMODULE pSoundModule = (*it);
		
		if( (pSoundModule->channel > -1) /*&& FSOUND_IsPlaying( pSoundModule->channel )*/ )
		{
			if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
			{
				FSOUND_StopSound( pSoundModule->channel );
			}			
			pSoundModule->channel = -1;
		}
	}
}

void _XFMODWrapper::FL_SetVolume( BYTE volume )
{
	m_FLSoundVolume = volume;
	
	svdef_XSOUNDMODULE::iterator it;
	
	int index = 0;
	for (it = m_svSoundModuleFlexibleList.begin(); it != m_svSoundModuleFlexibleList.end(); ++ it, index++ )
	{
		_LPXSOUNDMODULE pSoundModule = (*it);
		
		if( FSOUND_IsPlaying( pSoundModule->channel ) )
		{
			if( pSoundModule->psoundobject == FSOUND_GetCurrentSample( pSoundModule->channel ) )
			{
				FSOUND_SetVolume( pSoundModule->channel, m_FLSoundVolume );
			}			
		}
	}
}

BYTE _XFMODWrapper::FL_GetVolume( void )
{
	return m_FLSoundVolume;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Streamming methods

int	 _XFMODWrapper::FindStream( LPCTSTR szfilename )
{
	svdef_XSTREAMMODULE::iterator it;
	
	int index = 0;
	for (it = m_svStreamModuleList.begin(); it != m_svStreamModuleList.end(); ++ it, index++ )
	{
		if (stricmp((*it)->filename, szfilename) == 0)
		{
			return index;
		}
	}
	return -1;	
}

int	 _XFMODWrapper::InsertStream( LPCTSTR szfilename, DWORD mode, FLOAT fadeoutspeed )
{

#ifdef _XDWDEBUG
	if( g_MainSoundArchive.FindResource( szfilename ) < 0 )
	{
		_XLog( "ERROR : Sound file not found [%s]", szfilename );
		//_XInsertSystemMessage( 0, "ERROR : Sound file not found [%s]", szfilename );
	}
#endif

	svdef_XSTREAMMODULE::iterator it;
	
	int index = -1;
	for (it = m_svStreamModuleList.begin(); it != m_svStreamModuleList.end(); ++ it, index++ )
	{
		if (stricmp((*it)->filename, szfilename) == 0)
		{
			return index;
		}
	}
	
	_LPXSTREAMMODULE pNewStream = new _XSTREAMMODULE;
	
	memset( pNewStream->filename, 0, sizeof(TCHAR) * 32 );	
	strcpy( pNewStream->filename, szfilename );
	pNewStream->pstreamobject	= NULL;
	pNewStream->modeflag		= mode;
	pNewStream->channel			= -1;
	pNewStream->play			= FALSE;
	pNewStream->fademode		= _XSOUNDFADEMODE_NONE;
	pNewStream->fadestarttimer	= 0;
	pNewStream->currentvolume	= -1.0f;
	pNewStream->speed			= fadeoutspeed;
	m_svStreamModuleList.push_back( pNewStream );
	
	return (m_svStreamModuleList.size() - 1);
}

/*
t_int32 nState = FSOUND_Stream_GetOpenState (m_Stream.m_pStream); 
if (nState != 0) 
return true; 
*/


void _XFMODWrapper::PlayStream( int index, bool loop )
{
	if( index < 0 || index >= m_svStreamModuleList.size() ) return;
	
	_LPXSTREAMMODULE pStreamModule = m_svStreamModuleList[index];
	
	// Load sound in runtime...
	if( !pStreamModule->pstreamobject )
	{
		pStreamModule->pstreamobject = FSOUND_Stream_Open( pStreamModule->filename, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_NONBLOCKING, 0, 0);
		if( !pStreamModule->pstreamobject )
		{
			_XFatalError( "%s : %s\n", pStreamModule->filename, FMOD_ErrorString(FSOUND_GetError()));
			return;
		}
		
		FSOUND_Stream_SetMode( pStreamModule->pstreamobject, FSOUND_2D | pStreamModule->modeflag );

		FSOUND_Stream_SetEndCallback(  pStreamModule->pstreamobject, _XFMOD_StreamEndCallBack, 0 );
		FSOUND_Stream_SetSyncCallback( pStreamModule->pstreamobject, _XFMOD_StreamEndCallBack, 0 );
	}

	if( pStreamModule->channel < 0)
	{
		pStreamModule->channel = FSOUND_Stream_Play(FSOUND_FREE, pStreamModule->pstreamobject);		
		if( pStreamModule->channel > -1 )
		{
			pStreamModule->fademode = _XSOUNDFADEMODE_FADEOUT;
			pStreamModule->fadestarttimer	= 0;
			pStreamModule->currentvolume	= -1.0f;
			
			FSOUND_SetVolume( pStreamModule->channel, 0 );
		}
	}
	else
	{
		if( FSOUND_IsPlaying(pStreamModule->channel) )
		{
			FSOUND_SAMPLE* pSoundSample = FSOUND_Stream_GetSample( pStreamModule->pstreamobject );
			
			if( pSoundSample )
			{
				if( pSoundSample != FSOUND_GetCurrentSample( pStreamModule->channel ) )
				{
					pStreamModule->channel = -1;

					pStreamModule->channel = FSOUND_Stream_Play(FSOUND_FREE, pStreamModule->pstreamobject);		
					if( pStreamModule->channel > -1 )
					{
						pStreamModule->fademode = _XSOUNDFADEMODE_FADEOUT;
						pStreamModule->fadestarttimer	= 0;
						pStreamModule->currentvolume	= -1.0f;
						
						FSOUND_SetVolume( pStreamModule->channel, 0 );
					}
				}
			}			
			else
			{
				pStreamModule->channel = -1;

				pStreamModule->channel = FSOUND_Stream_Play(FSOUND_FREE, pStreamModule->pstreamobject);		
				if( pStreamModule->channel > -1 )
				{
					pStreamModule->fademode = _XSOUNDFADEMODE_FADEOUT;
					pStreamModule->fadestarttimer	= 0;
					pStreamModule->currentvolume	= -1.0f;
					
					FSOUND_SetVolume( pStreamModule->channel, 0 );
				}
			}
		}		
		else
		{
			pStreamModule->channel = -1;

			FSOUND_Stream_SetPosition( pStreamModule->pstreamobject, 0 );

			pStreamModule->channel = FSOUND_Stream_Play(FSOUND_FREE, pStreamModule->pstreamobject);		
			if( pStreamModule->channel > -1 )
			{
				pStreamModule->fademode = _XSOUNDFADEMODE_FADEOUT;
				pStreamModule->fadestarttimer	= 0;
				pStreamModule->currentvolume	= -1.0f;
				
				FSOUND_SetVolume( pStreamModule->channel, 0 );
			}
		}
	}

	pStreamModule->play = TRUE;
}


void _XFMODWrapper::StopStream( int index )
{
	if( index < 0 || index >= m_svStreamModuleList.size() ) return;	
	
	_LPXSTREAMMODULE pStreamModule = m_svStreamModuleList[index];
	
	if(pStreamModule->pstreamobject)
	{
		FSOUND_Stream_Stop(pStreamModule->pstreamobject);	
		FSOUND_Stream_Close(pStreamModule->pstreamobject);
		pStreamModule->pstreamobject = NULL;
	}

	pStreamModule->channel = -1;
	pStreamModule->play = FALSE;
}

void _XFMODWrapper::StopAllStream( void )
{
	svdef_XSTREAMMODULE::iterator it;		
	for (it = m_svStreamModuleList.begin(); it != m_svStreamModuleList.end(); ++ it )
	{
		_LPXSTREAMMODULE pStreamModule = (*it);
		if(pStreamModule->pstreamobject)
		{
			FSOUND_Stream_Stop(pStreamModule->pstreamobject);
			FSOUND_Stream_Close(pStreamModule->pstreamobject);
			pStreamModule->pstreamobject = NULL;
		}
	}
}

_LPXSTREAMMODULE _XFMODWrapper::FL_GetStreamObject( int index )
{
	if( index < 0 || index >= m_svStreamModuleList.size() ) return NULL;
	return m_svStreamModuleList[index];
}

void _XFMODWrapper::ProcessStream( void )
{
	svdef_XSTREAMMODULE::iterator it;		
	for (it = m_svStreamModuleList.begin(); it != m_svStreamModuleList.end(); ++ it )
	{
		_LPXSTREAMMODULE pStreamModule = (*it);
		if(pStreamModule->pstreamobject)
		{
			if( pStreamModule->play )
			{
				if( pStreamModule->channel > -1 )
				{
					if( pStreamModule->fademode == _XSOUNDFADEMODE_FADEIN )
					{
						if( pStreamModule->fadestarttimer == 0 )
						{
							pStreamModule->fadestarttimer = g_LocalSystemTime;
						}

						int _volume = FSOUND_GetVolume( pStreamModule->channel );

						if( pStreamModule->currentvolume == -1.0f )
						{
							pStreamModule->currentvolume = _volume;
						}

						if( pStreamModule->currentvolume > 0 )
						{
							pStreamModule->currentvolume -=  ( ((FLOAT)(g_LocalSystemTime - pStreamModule->fadestarttimer) / pStreamModule->speed ) / 1000.0f );

							if( pStreamModule->currentvolume < 0.0f )
							{
								FSOUND_Stream_Stop(pStreamModule->pstreamobject);
								pStreamModule->fademode = _XSOUNDFADEMODE_NONE;
								pStreamModule->play = FALSE;
							}
							else
							{
								FSOUND_SetVolume( pStreamModule->channel, (int)pStreamModule->currentvolume );
							}
						}
						else
						{
							FSOUND_Stream_Stop(pStreamModule->pstreamobject);
							pStreamModule->fademode = _XSOUNDFADEMODE_NONE;
							pStreamModule->play = FALSE;
						}
					}
					else if( pStreamModule->fademode == _XSOUNDFADEMODE_FADEOUT )
					{
						if( pStreamModule->fadestarttimer == 0 )
						{
							pStreamModule->fadestarttimer = g_LocalSystemTime;
						}

						int _volume = FSOUND_GetVolume( pStreamModule->channel );
						
						if( pStreamModule->currentvolume == -1.0f )
						{
							pStreamModule->currentvolume = _volume;
						}
						
						if( pStreamModule->currentvolume < m_StreamVolume )
						{
							pStreamModule->currentvolume +=  ( ((FLOAT)(g_LocalSystemTime - pStreamModule->fadestarttimer) / pStreamModule->speed) / 1000.0f );
							if( pStreamModule->currentvolume > m_StreamVolume )
							{
								pStreamModule->currentvolume = m_StreamVolume;
								pStreamModule->fademode = _XSOUNDFADEMODE_NONE;								
							}

							FSOUND_SetVolume( pStreamModule->channel, (int)pStreamModule->currentvolume );
						}
					}
					else
					{
						if( pStreamModule->currentvolume < m_StreamVolume )
						{
							pStreamModule->currentvolume = m_StreamVolume;
							FSOUND_SetVolume( pStreamModule->channel, (int)pStreamModule->currentvolume );
						}
					}
				}
				else
				{
					pStreamModule->channel = FSOUND_Stream_Play(FSOUND_FREE, pStreamModule->pstreamobject);

					if( pStreamModule->channel > -1 )
					{
						pStreamModule->fademode = _XSOUNDFADEMODE_FADEOUT;
						FSOUND_SetVolume( pStreamModule->channel, 0 );
						pStreamModule->currentvolume = -1;
					}				
				}
			}
		}
	}	
}

void _XFMODWrapper::SetStreamVolume( BYTE volume )
{
	m_StreamVolume = volume;

	svdef_XSTREAMMODULE::iterator it;		
	for (it = m_svStreamModuleList.begin(); it != m_svStreamModuleList.end(); ++ it )
	{
		_LPXSTREAMMODULE pStreamModule = (*it);
		if(pStreamModule->pstreamobject)
		{
			if( pStreamModule->channel > -1 )
			{
				if( FSOUND_IsPlaying(pStreamModule->channel) )
				{
					FSOUND_SAMPLE* pSoundSample = FSOUND_Stream_GetSample( pStreamModule->pstreamobject );

					if( pSoundSample )
					{
						if( pSoundSample == FSOUND_GetCurrentSample( pStreamModule->channel ) )
						{
							FSOUND_SetVolume( pStreamModule->channel, m_StreamVolume );							
						}
					}
				}
			}
		}

		pStreamModule->currentvolume	= m_StreamVolume;
		pStreamModule->fademode			= _XSOUNDFADEMODE_NONE;
		pStreamModule->fadestarttimer	= 0;
	}
}

BYTE _XFMODWrapper::GetStreamVolume( void )
{
	return m_StreamVolume;
}

void _XFMODWrapper::SetFadeMode( LPCTSTR szfilename, _XSOUNDFADEMODE fademode, FLOAT speed  )
{
	int index = FindStream( szfilename );
	if( index > -1 )
	{
		SetFadeMode( index, fademode, speed );
	}
}

void _XFMODWrapper::SetFadeMode( int index, _XSOUNDFADEMODE fademode, FLOAT speed  )
{
	if( index < 0 || index >= m_svStreamModuleList.size() ) return;

	_LPXSTREAMMODULE pStreamModule = m_svStreamModuleList[index];
	if( pStreamModule )
	{
		pStreamModule->fademode = fademode;
		pStreamModule->fadestarttimer = g_LocalSystemTime;
		pStreamModule->currentvolume = -1.0f;
		pStreamModule->speed		 = speed;
	}
}

_XSOUNDFADEMODE	_XFMODWrapper::GetFadeMode( LPCTSTR szfilename )
{
	int index = FindStream( szfilename );
	if( index > -1 )
	{
		return GetFadeMode( index );
	}
	return _XSOUNDFADEMODE_NONE;
}

_XSOUNDFADEMODE	_XFMODWrapper::GetFadeMode( int index )
{
	if( index < 0 || index >= m_svStreamModuleList.size() ) return _XSOUNDFADEMODE_NONE;
	_LPXSTREAMMODULE pStreamModule = m_svStreamModuleList[index];
	if( pStreamModule )
	{
		return pStreamModule->fademode;
	}
	else
	{
		return _XSOUNDFADEMODE_NONE;
	}
}

void _XFMODWrapper::DeleteStream( LPCTSTR szfilename )
{
	int index = FindStream( szfilename );
	if( index > -1 )
	{
		DeleteStream( index );
	}
}

void _XFMODWrapper::DeleteStream( int index )
{
	if( index < 0 || index >= m_svStreamModuleList.size() ) return;
	
	_LPXSTREAMMODULE pStreamModule = m_svStreamModuleList[index];
	FSOUND_Stream_Close( pStreamModule->pstreamobject );
	
	svdef_XSTREAMMODULE::iterator it = m_svStreamModuleList.begin();
	it+=index;
	m_svStreamModuleList.erase( it );
}

BOOL _XFMODWrapper::IsBGMPlaying( void )
{
	if( m_BGMChannel > -1 )
	{
		if( FSOUND_IsPlaying(m_BGMChannel) )
		{
			FSOUND_SAMPLE* pSoundSample = FSOUND_Stream_GetSample( m_pBGMStreamObject );
			
			if( pSoundSample )
			{
				if( pSoundSample == FSOUND_GetCurrentSample( m_BGMChannel ) )
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void _XFMODWrapper::PlayBGM( LPCTSTR szfilename, BOOL loop )
{
	BOOL bgmplaying = FALSE;

	if( m_BGMChannel > -1 )
	{
		if( FSOUND_IsPlaying(m_BGMChannel) )
		{
			FSOUND_SAMPLE* pSoundSample = FSOUND_Stream_GetSample( m_pBGMStreamObject );
			
			if( pSoundSample )
			{
				if( pSoundSample == FSOUND_GetCurrentSample( m_BGMChannel ) )
				{
					bgmplaying = TRUE;
				}
			}
		}
	}
	
	if( !bgmplaying )
	{
		memset( m_CurrentBGMName, 0, sizeof(TCHAR) * 64 );
		strcpy( m_CurrentBGMName, szfilename );

		memset( m_NextBGMName, 0, sizeof(TCHAR) * 64 );

		if( m_pBGMStreamObject )
		{
			FSOUND_Stream_Stop(m_pBGMStreamObject);
			FSOUND_Stream_Close(m_pBGMStreamObject);
			m_pBGMStreamObject = NULL;
		}	

		if( loop )
			m_pBGMStreamObject = FSOUND_Stream_Open( m_CurrentBGMName, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_NONBLOCKING | FSOUND_LOOP_NORMAL, 0, 0);
		else
			m_pBGMStreamObject = FSOUND_Stream_Open( m_CurrentBGMName, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_NONBLOCKING, 0, 0);
			
		if( !m_pBGMStreamObject )
		{
			_XLog( "ERROR : BGM Play [ %s : %s ]\n", m_CurrentBGMName, FMOD_ErrorString(FSOUND_GetError()));
			m_ChangeBGM = TRUE;
			return;
		}
			
		if( loop )
			FSOUND_Stream_SetMode( m_pBGMStreamObject, FSOUND_2D | FSOUND_LOOP_NORMAL );
		else
			FSOUND_Stream_SetMode( m_pBGMStreamObject, FSOUND_2D | FSOUND_LOOP_OFF );
			

		FSOUND_Stream_SetEndCallback(  m_pBGMStreamObject, _XFMOD_StreamEndCallBack, 0 );
		FSOUND_Stream_SetSyncCallback( m_pBGMStreamObject, _XFMOD_StreamEndCallBack, 0 );		
		
		m_BGMVolume = 0.0f;

		m_BGMChannel = FSOUND_Stream_Play(FSOUND_FREE, m_pBGMStreamObject);		
		if( m_BGMChannel > -1 )
		{
			FSOUND_SetVolume( m_BGMChannel, 0 );
		}

		m_ChangeBGM = FALSE;
	}
	else
	{
		memset( m_NextBGMName, 0, sizeof(TCHAR) * 64 );
		if( strlen(szfilename) > 0 )
			strcpy( m_NextBGMName, szfilename );
		m_ChangeBGM = TRUE;
	}
}

void _XFMODWrapper::SetBGMLoopPoints( unsigned int loopstart, unsigned int loopend )
{
	if( !m_pBGMStreamObject ) return;
	FSOUND_Stream_SetLoopPoints( m_pBGMStreamObject, loopstart, loopend );
}

void _XFMODWrapper::ProcessBGM( void )
{
	if( m_ChangeBGM )
	{
		if( strlen(m_NextBGMName) <= 0 )
		{
			m_ChangeBGM = FALSE;
		}

		BOOL nextplay = FALSE;
		
		if( m_pBGMStreamObject )
		{
			if( m_BGMChannel < 0 )
			{
				m_BGMChannel = FSOUND_Stream_Play(FSOUND_FREE, m_pBGMStreamObject);
			}

			BOOL playing = FALSE;

			if( m_BGMChannel > -1 )
			{
				if( FSOUND_IsPlaying(m_BGMChannel) )
				{
					FSOUND_SAMPLE* pSoundSample = FSOUND_Stream_GetSample( m_pBGMStreamObject );
					
					if( pSoundSample )
					{
						if( pSoundSample == FSOUND_GetCurrentSample( m_BGMChannel ) )
						{
							playing = TRUE;
						}
					}
				}
			}

			if( playing )
			{
				if( m_BGMChannel > -1 )
				{
					if( m_BGMVolume > 0 )
					{
						m_BGMVolume -= 2.0f;//(0.0512f * g_fElapsedFrameTime); // Five seconds
						
						if( m_BGMVolume <= 0.0f ) // Stop prev bgm
						{
							m_BGMVolume = 0.0f;
							FSOUND_SetVolume( m_BGMChannel, (BYTE)m_BGMVolume );

							FSOUND_Stream_Stop(m_pBGMStreamObject);
							FSOUND_Stream_Close(m_pBGMStreamObject);
							m_pBGMStreamObject = NULL;
							m_BGMChannel = -1;

							m_pBGMStreamObject = FSOUND_Stream_Open( m_NextBGMName, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_NONBLOCKING, 0, 0);
							if( !m_pBGMStreamObject )
							{
								_XLog( "ERROR : BGM Play [ %s : %s ]\n", m_NextBGMName, FMOD_ErrorString(FSOUND_GetError()));
								return;
							}
							
							FSOUND_Stream_SetMode( m_pBGMStreamObject, FSOUND_2D | FSOUND_LOOP_OFF );
							FSOUND_Stream_SetEndCallback(  m_pBGMStreamObject, _XFMOD_StreamEndCallBack, 0 );
							FSOUND_Stream_SetSyncCallback( m_pBGMStreamObject, _XFMOD_StreamEndCallBack, 0 );		
							
							m_BGMChannel = FSOUND_Stream_Play(FSOUND_FREE, m_pBGMStreamObject);
							if( m_BGMChannel > -1 )
							{
								FSOUND_SetVolume( m_BGMChannel, (BYTE)m_BGMVolume );
							}
							
							m_ChangeBGM = FALSE;
							strcpy( m_CurrentBGMName, m_NextBGMName );
							memset( m_NextBGMName, 0, sizeof(TCHAR) * 64 );
						}
						else
						{
							FSOUND_SetVolume( m_BGMChannel, (BYTE)m_BGMVolume );
						}
					}
					else
					{
						nextplay = TRUE;
					}
				}
				else
				{
					nextplay = TRUE;
				}
			}
			else
			{
				nextplay = TRUE;
			}
		}
		else
		{
			nextplay = TRUE;
		}

		if( nextplay )
		{
			if( m_pBGMStreamObject )
			{
				FSOUND_Stream_Stop(m_pBGMStreamObject);
				FSOUND_Stream_Close(m_pBGMStreamObject);
				m_pBGMStreamObject = NULL;
			}

			m_BGMChannel = -1;
			m_BGMVolume = 0.0f;

			m_pBGMStreamObject = FSOUND_Stream_Open( m_NextBGMName, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_NONBLOCKING, 0, 0);
			if( !m_pBGMStreamObject )
			{
				_XLog( "ERROR : BGM Play [ %s : %s ]\n", m_NextBGMName, FMOD_ErrorString(FSOUND_GetError()));
				return;
			}
			
			FSOUND_Stream_SetMode( m_pBGMStreamObject, FSOUND_2D | FSOUND_LOOP_OFF );			
			FSOUND_Stream_SetEndCallback(  m_pBGMStreamObject, _XFMOD_StreamEndCallBack, 0 );
			FSOUND_Stream_SetSyncCallback( m_pBGMStreamObject, _XFMOD_StreamEndCallBack, 0 );		
			
			m_BGMChannel = FSOUND_Stream_Play(FSOUND_FREE, m_pBGMStreamObject);
			if( m_BGMChannel > -1 )
			{
				FSOUND_SetVolume( m_BGMChannel, (BYTE)m_BGMVolume );
			}
			
			m_ChangeBGM = FALSE;
			strcpy( m_CurrentBGMName, m_NextBGMName );
			memset( m_NextBGMName, 0, sizeof(TCHAR) * 64 );
		}		
	}
	else
	{
		if( m_pBGMStreamObject )
		{
			if( m_BGMChannel < 0 )
			{
				m_BGMChannel = FSOUND_Stream_Play(FSOUND_FREE, m_pBGMStreamObject);
			}

			if( m_BGMVolume < m_BGMMaxVolume )
			{
				BOOL playing = FALSE;

				if( m_BGMChannel > -1 )
				{
					if( FSOUND_IsPlaying(m_BGMChannel) )
					{
						FSOUND_SAMPLE* pSoundSample = FSOUND_Stream_GetSample( m_pBGMStreamObject );
						
						if( pSoundSample )
						{
							if( pSoundSample == FSOUND_GetCurrentSample( m_BGMChannel ) )
							{
								playing = TRUE;
							}
						}
					}
				}

				if( playing )
				{
					if( m_BGMChannel > -1 )
					{
						m_BGMVolume +=  2.0f;//(0.0512f * g_fElapsedFrameTime); // Five seconds
						if( m_BGMVolume > m_BGMMaxVolume ) m_BGMVolume = m_BGMMaxVolume;
						
						FSOUND_SetVolume( m_BGMChannel, (BYTE)m_BGMVolume );
					}
				}
			}
		}
	}
}


void _XFMODWrapper::StopBGM( void )
{
	if( m_pBGMStreamObject )
	{
		FSOUND_Stream_Stop(m_pBGMStreamObject);
		FSOUND_Stream_Close(m_pBGMStreamObject);
		m_pBGMStreamObject = NULL;
		m_StopedBGM = TRUE;
		m_ChangeBGM = FALSE;
		m_BGMVolume = 0.0f;

		memset( m_CurrentBGMName, 0, sizeof(TCHAR) * 64 );
		memset( m_NextBGMName, 0, sizeof(TCHAR) * 64 );
	}
}

void _XFMODWrapper::SetBGMMaxVolume( BYTE volume )
{ 
	m_BGMMaxVolume = volume;

	if( m_ChangeBGM ) 
	{
		if( m_BGMVolume > m_BGMMaxVolume )
		{
			m_BGMVolume = m_BGMMaxVolume;
		}
		//return;
	}

	if( !m_pBGMStreamObject ) return;

	if( m_BGMChannel > -1 )
	{
		if( FSOUND_IsPlaying(m_BGMChannel) )
		{
			FSOUND_SAMPLE* pSoundSample = FSOUND_Stream_GetSample( m_pBGMStreamObject );
			
			if( pSoundSample )
			{
				if( pSoundSample == FSOUND_GetCurrentSample( m_BGMChannel ) )
				{
					m_BGMVolume = m_BGMMaxVolume;
					FSOUND_SetVolume( m_BGMChannel, (BYTE)m_BGMVolume );
				}
			}
		}
	}
}



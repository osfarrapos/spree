// XMADWrapper.cpp: implementation of the _XMADWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _XDEF_MADSYSTEM

#include "XKernel.h"
#include "XMADWrapper.h"
#include "EODEXTDEF_GlobalObject.h"

#define _XDEF_MADSCRIPTFILE		_T("GameData.dat")

// Massive Namespace
using namespace MassiveAdClient3;

// test sample
const unsigned char MASSIVE_PublicKey[]=
{
	0x98,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0xd6,
	0x37,0xae,0xe5,0xcc,0x10,0xa5,0x8f,0xf0,0x2c,0x33,
	0xa9,0xb3,0xb6,0x13,0x17,0x9e,0xd0,0xf2,0x77,0x58,
	0xe2,0xf6,0x2f,0x43,0x72,0x35,0xc4,0xec,0x0c,0x3b,
	0xd3,0x2b,0x2d,0xd3,0x0c,0xa9,0xea,0xe8,0xa7,0x1b,
	0x1a,0x63,0x2b,0xbc,0xde,0xa8,0xd6,0xfa,0x8e,0x07,
	0x82,0xb5,0x0d,0x11,0x7d,0x61,0x96,0xd2,0xbf,0x5e,
	0xfe,0x3e,0x6f,0x94,0xd9,0x50,0xb3,0xf5,0xa0,0x77,
	0x6d,0xc1,0x64,0x51,0xe7,0x02,0x40,0xbc,0x55,0x9f,
	0xf6,0x2c,0x69,0x93,0x4f,0x7d,0x4f,0x37,0x91,0x48,
	0x8b,0xe1,0x30,0xc1,0xd3,0xc4,0x5d,0x06,0x93,0xcc,
	0x7f,0x2e,0x34,0x43,0xd8,0xbd,0x3b,0x09,0x60,0xe4,
	0x14,0x21,0x70,0xb2,0xd7,0x08,0x34,0x0a,0x7e,0xed,
	0xb9,0x69,0x5b,0x27,0xea,0x4a,0x71,0x03,0x00,0x00,
	0x00,0x01,0x00,0x01
};		

// acclaim
/*
const unsigned char MASSIVE_PublicKey[]=
{
	0x98,0x00,0x00,0x00,0x01,0x80,0x00,0x00,0x00,0x40,
	0x61,0x1f,0xb6,0x35,0x59,0x4a,0x5a,0x53,0xee,0xd2,
	0xde,0xaa,0xea,0xad,0xad,0x75,0x27,0xab,0x89,0x67,
	0x08,0x12,0xfd,0x70,0x6c,0x2a,0x02,0xcf,0x6e,0x35,
	0x42,0x74,0xd5,0xe1,0xf0,0xfd,0x43,0xf9,0x02,0x67,
	0xec,0xe5,0x03,0x52,0xbc,0x17,0x56,0x13,0x6d,0xae,
	0xb2,0xaf,0x0b,0xa3,0x8d,0xa6,0x03,0x42,0xf5,0x77,
	0x33,0x56,0x9b,0x6b,0xee,0xf3,0x78,0x92,0x55,0xb6,
	0x6b,0x62,0x12,0x88,0x8c,0x3b,0xc1,0x23,0x2c,0x7e,
	0xac,0xf7,0xa7,0x93,0xb4,0xdc,0x00,0x3a,0xa6,0xd7,
	0x88,0x55,0xfb,0x62,0x96,0x66,0xee,0x2c,0x76,0x32,
	0x1a,0xe9,0x08,0xd7,0x9c,0xdb,0x24,0xbd,0x4d,0x5c,
	0x59,0x65,0x91,0x71,0x02,0x24,0x1b,0x2e,0xc7,0xa2,
	0xb3,0xb1,0xc0,0xd6,0x26,0x95,0xb7,0x03,0x00,0x00,
	0x00,0x01,0x00,0x01
};
*/

const char *_XMAD_STRING_INVALID	= "Invalid";
const char *_XMAD_STRING_VIEW		= "View";
const char *_XMAD_STRING_IMPRESSION	= "Impression";
const char *_XMAD_STRING_OFFSCREEN	= "Offscreen";

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XMADWrapper::_XMADWrapper()
{
	m_pCoreInstance = NULL;	
	m_bMassiveInitialized = FALSE;
		
	// Clear data
	memset(m_szTextApp, 0, MAX_STATUS_TEXT);
	memset(m_szTextClient, 0, MAX_STATUS_TEXT);
	memset(&m_GameData, 0, sizeof(m_GameData));

	m_SubscriberCounter		= 0;
	m_PrevSubscribers		= -1;
	m_CurrentSubscribers	= -1;
	m_LastCyclingTime		= 0;
	m_CurrentLocationIndex	= -1;
	m_NeedToFlush			= FALSE;
}

_XMADWrapper::~_XMADWrapper()
{
	
}


BOOL _XMADWrapper::Initialize( void )
{
	// Load GameData from file

	_XSetDefaultPath();

	if (FileLoadGameDataFrom(_XDEF_MADSCRIPTFILE) == 0)
	{
		_XFatalError( _T("MADWrapper : Could not read file: %s"), _XDEF_MADSCRIPTFILE);
		return FALSE;
	}

	// Setup the Massive Initialization Structure
	MAD_MASSIVE_INIT_STRUCT InitStruct;
	memset(&InitStruct, 0, sizeof(MAD_MASSIVE_INIT_STRUCT));
	
	InitStruct.szPublicKey = (unsigned char*)MASSIVE_PublicKey;
	InitStruct.szSku = m_GameData.szSKU;
	InitStruct.szSkuVersion = m_GameData.szSKUVersion;
	InitStruct.szThirdPartyID = m_GameData.szThirdPartyID;
	InitStruct.szThirdPartyService = m_GameData.szThirdPartyService;
	InitStruct.u_libraryConfigFlags = m_GameData.uConfigFlags;
	InitStruct.eLogLevel = (MASSIVE_ENUM_LOG_LEVEL)m_GameData.uLogLevel;
	
	// Init the Massive client core system
	m_pCoreInstance = CMassiveClientCore::Initialize(&InitStruct);
	
	// Validity Check
	if (!m_pCoreInstance)
	{
		_XLog( _T("_XMADWrapper : Failed initialize client core") );
		return FALSE; 
	}
	
	// Log Application Messages to the Massive AdClient Log
	CMassiveClientCore::Instance()->Log(LOG_LEVEL_INFO, _T("NineDragons"), "Successfully Initialized");
	_XLog( _T("_XMADWrapper : successfully initialized") );

	if( !LoadMADZoneSetupData() )
	{
		return FALSE;
	}

	m_PrevSubscribers		= -1;
	m_CurrentSubscribers	= -1;
	m_bMassiveInitialized = TRUE;
	m_LastCyclingTime		= 0;
	m_NeedToFlush			= FALSE;

	return TRUE;
}

void _XMADWrapper::Release( void )
{
	if( !m_bMassiveInitialized )
		return;
	
	//////////////////////////////////////////////////////////////////////////
	// Massive shutdown
	_XLog( _T("_XMADWrapper : MAD Shutdown") );


	// Remove Subscribers
	_XLog( _T("_XMADWrapper : Remove MAD Subscribers") );
	if( m_Subscribers.size() > 0 )
	{
		/*
		std::vector<_XMVTSubscriberObject*>::iterator iter;
		for (iter = m_Subscribers.begin(); iter != m_Subscribers.end(); iter++) 
		{
			(*iter)->ReleaseDeviceObject();
			delete (_XMVTSubscriberObject*)(*iter);
		}		
		*/
		std::for_each( m_Subscribers.begin(), m_Subscribers.end(), DELETEOBJECT() );
		m_Subscribers.clear();
	}
	
	m_SubscriberCounter = 0;

	if( m_ZoneSetupData.size() > 0 )
	{
		/*
		std::vector<_XMADZONE_SETUPDATA*>::iterator maddataiter;
		for (maddataiter= m_ZoneSetupData.begin(); maddataiter != m_ZoneSetupData.end(); maddataiter++) 
		{
			delete (_XMADZONE_SETUPDATA*)(*maddataiter);
		}
		*/
		std::for_each( m_ZoneSetupData.begin(), m_ZoneSetupData.end(), DELETEOBJECT() );
		m_ZoneSetupData.clear();
	}

	// Validity Check
	if (IsMassiveInitialized())
	{	
		// Log Application Messages to the Massive AdClient Log
		_XLog( _T("_XMADWrapper : Shutdown - massive client core") );
		CMassiveClientCore::Instance()->Log(LOG_LEVEL_INFO, _T("NineDragons"), "Shutdown");	
		CMassiveClientCore::Shutdown(true);
	}
	else
	{
		_XLog( _T("_XMADWrapper : Skipped shutdown - massive client core") );
		CMassiveClientCore::Instance()->Log(LOG_LEVEL_INFO, _T("NineDragons"), "Skipped shutdown");	
	}

	//////////////////////////////////////////////////////////////////////////	
	// Remove object data

	if (m_GameData.szSKU)
		delete []m_GameData.szSKU;
	
	if (m_GameData.szSKUVersion)
		delete []m_GameData.szSKUVersion;
	
	if (m_GameData.szThirdPartyID)
		delete []m_GameData.szThirdPartyID;
	
	if (m_GameData.szThirdPartyService)
		delete []m_GameData.szThirdPartyService;

	/*
	if (m_GameData.szZoneName)
		delete []m_GameData.szZoneName;
	
	for (massiveU32 idx=0; idx < m_GameData.uNumElements; idx++)
	{
		if (m_GameData.szNames[idx])
		{
			delete m_GameData.szNames[idx];
			m_GameData.szNames[idx] = NULL;
		}
	}
	*/

	m_PrevSubscribers		= -1;
	m_CurrentSubscribers	= -1;

	m_bMassiveInitialized	= FALSE;
	
	m_NeedToFlush			= FALSE;

	m_LastCyclingTime		= 0;

	_XLog( _T("_XMADWrapper : Release complete") );
}

LPCTSTR	_XGetSecondWordPoint( LPCTSTR pszString )
{
	if( !pszString ) return NULL;

	TCHAR tempbuffer1[256];
	TCHAR tempbuffer2[256];

	sscanf( pszString, _T("%s %s"), tempbuffer1, tempbuffer2 );
	LPCTSTR pCpPoint = strstr( pszString, tempbuffer2 );
	return	pCpPoint;
}


BOOL _XMADWrapper::LoadMADZoneSetupData( void )
{
	std::for_each( m_ZoneSetupData.begin(), m_ZoneSetupData.end(), DELETEOBJECT() );
	m_ZoneSetupData.clear();
	/*
	std::vector<_XMADZONE_SETUPDATA*>::iterator maddataiter;
	for (maddataiter= m_ZoneSetupData.begin(); maddataiter != m_ZoneSetupData.end(); maddataiter++) 
	{
		delete (_XMADZONE_SETUPDATA*)(*maddataiter);
	}
	*/

	FILE* pFile = NULL;
	
	_XSetDefaultPath();

	pFile = fopen( _T("MADScript.ini"), _T("rt") );
	
	if( !pFile )
	{
		return FALSE;
	}
	
	int index = 0;
	TCHAR buffer[256];
	
	_XMADZONE_SETUPDATA* pSetupData = NULL;
	
	do 
	{
		if( feof(pFile) ) 
			break;
		
		memset( buffer, 0, sizeof(TCHAR)*256 );
		if( !fgets( buffer, 255, pFile ) )
		{
			break;
		}
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) 
			break;
		
		if( ( strlen(buffer) > 0 ) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r') )
		{
			if( strncmp( buffer, _T("LOCATIONAME"), strlen(_T("LOCATIONAME") ) ) == 0 )
			{
				pSetupData = new _XMADZONE_SETUPDATA;				
				memset( pSetupData, 0, sizeof(_XMADZONE_SETUPDATA) );

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

				m_ZoneSetupData.insert(m_ZoneSetupData.begin(), pSetupData);
			}
			else if( strncmp( buffer, _T("ZONENAME"), strlen(_T("ZONENAME") ) ) == 0 )
			{
				if( pSetupData )
				{
					strcpy( pSetupData->zonename, _XGetSecondWordPoint(buffer) );
					int len = strlen( pSetupData->zonename );
					if( pSetupData->zonename[len-1] == _T('\n') || pSetupData->zonename[len-1] == _T('\r') )
					{
						pSetupData->zonename[len-1] = NULL;
					}
					if( pSetupData->zonename[len-2] == _T('\n') || pSetupData->zonename[len-2] == _T('\r') )
					{
						pSetupData->zonename[len-2] = NULL;
					}
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

					pSetupData->horizalignment = -1;
					pSetupData->vertalignment = -1;
				}
			}
			else if( strncmp( buffer, _T("IMAGESIZE"), strlen(_T("IMAGESIZE") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					sscanf( buffer, _T("%s %d %d"), tempbuffer, &(pSetupData->imagesize.cx), &(pSetupData->imagesize.cy) );
				}
			}
			else if( strncmp( buffer, _T("INTERVAL"), strlen(_T("INTERVAL") ) ) == 0 )
			{
				if( pSetupData )
				{
					TCHAR tempbuffer[64];
					sscanf( buffer, _T("%s %d"), tempbuffer, &(pSetupData->cyclinginterval) );
				}
			}
			else if( strncmp( buffer, _T("ELEMENTS"), strlen(_T("ELEMENTS") ) ) == 0 )
			{
				if( pSetupData )
				{
					LPTSTR pStr = (LPTSTR)_XGetSecondWordPoint(buffer);

					USHORT		currentIE = 0;
					LPTSTR		tempElementName = NULL;
					
					tempElementName = strtok(pStr,_T(",\r\n"));

					while(currentIE < MAX_INVENTORY_ELEMENTS && NULL != tempElementName)
					{
						pSetupData->uNumElements = currentIE+1;
						pSetupData->szElementNames[currentIE] = new TCHAR[strlen(tempElementName)+1];
						strcpy(pSetupData->szElementNames[currentIE], tempElementName);

						tempElementName = strtok(NULL,_T(",\r\n"));
						
						currentIE++;
					}
				}
			}
		}
		
	} while( !feof(pFile) );

	fclose( pFile );

	//int size = m_ZoneSetupData.size();

	return TRUE;
}


UINT _XMADWrapper::FileLoadGameDataFrom(LPTSTR szFileName)
{
	// TODO - Platfrom independent
	HANDLE hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (INVALID_HANDLE_VALUE == hFile)
		return 0;
	
	// Create a Buffer to hold the data file, since its small lets load the whole file
	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == -1)
		return 0;
	
	DWORD	bytesRead = 0;
	DWORD	bytesToRead = 1024;
	UINT	currentBufferPosition = 0;
	
	// Read the File into a buffer
	LPTSTR	buffer = (LPTSTR)malloc(fileSize + 1);
	ReadFile(hFile, buffer, fileSize, &bytesRead, NULL);
	buffer[bytesRead] ='\0';
	
	// Get the SKU, Zone and IE's Key Value Pairs
	LPTSTR	tempBuffer = NULL;
	while (-1 != (currentBufferPosition = FileGetNextLine(buffer, currentBufferPosition, fileSize, &tempBuffer)))
	{
		LPTSTR	key = strtok(tempBuffer, _T(":\r\n") );
		LPTSTR  value = strtok(NULL, _T(":\r\n"));
		
		if (!strcmp(key, _T("sku")))
		{
			m_GameData.szSKU = (LPTSTR)malloc(strlen(value)+1);
			strcpy(m_GameData.szSKU, value);
			free(tempBuffer);
		}
		else if (!strcmp(key, _T("skuVersion")))
		{
			m_GameData.szSKUVersion = (LPTSTR)malloc(strlen(value)+1);
			strcpy(m_GameData.szSKUVersion, value);
			free(tempBuffer);
		}
		else if (!strcmp(key, _T("thirdPartyId")))
		{
			m_GameData.szThirdPartyID = (LPTSTR)malloc(strlen(value)+1);
			strcpy(m_GameData.szThirdPartyID, value);
			free(tempBuffer);
		}
		else if (!strcmp(key, _T("thirdPartyService")))
		{
			m_GameData.szThirdPartyService = (LPTSTR)malloc(strlen(value)+1);
			strcpy(m_GameData.szThirdPartyService, value);
			free(tempBuffer);
		}
		else if (!strcmp(key, _T("flushTime")))
		{
			m_GameData.uFlushTime = atoi(value);
			free(tempBuffer);
		}
		else if (!strcmp(key, _T("configFlag")))
		{
			m_GameData.uConfigFlags = atoi(value);
			free(tempBuffer);
		}
		else if (!strcmp(key, _T("logLevel")))
		{
			m_GameData.uLogLevel = atoi(value);
			free(tempBuffer);
		}
		/*
		else if (!strcmp(key,_T("zone")))
		{
			m_GameData.szZoneName = (LPTSTR)malloc(strlen(value)+1);
			strcpy(m_GameData.szZoneName, value);
			free(tempBuffer);
		}
		else if (!strcmp(key,_T("elements")))
		{
			USHORT		currentIE = 0;
			LPTSTR		tempElementName = NULL;
			
			tempElementName = strtok(value,_T(",\r\n"));
			while(currentIE < MAX_INVENTORY_ELEMENTS && NULL != tempElementName)
			{
				m_GameData.uNumElements = currentIE;
				m_GameData.szNames[currentIE] = (LPTSTR)malloc(strlen(tempElementName)+1);
				strcpy(m_GameData.szNames[currentIE], tempElementName);
				tempElementName = strtok(NULL,_T(",\r\n"));
				
				currentIE++;
			}
		}
		*/
	}
	
	m_bDataLoaded = true;
	return (UINT)bytesRead;
}

int	 _XMADWrapper::FileGetNextLine(LPTSTR pBufferIn, UINT inPos, UINT inLength, LPTSTR *ppBufferOut)
{
	// Valid Position
	if (inPos >= inLength)
		return -1;
	
	// Get the position in the buffer
	LPTSTR	currentPosition = pBufferIn + inPos;
	LPTSTR  newLinePosition = NULL;
	UINT	newBufferLen = 0;
	
	if ((newLinePosition = strchr(currentPosition, _T('\n') )))
	{
		// Copy where we started until the newline
		newBufferLen = (newLinePosition - currentPosition);
		*ppBufferOut = (LPTSTR)malloc(newBufferLen);
		memcpy(*ppBufferOut, currentPosition, newBufferLen);
		memcpy(*ppBufferOut+(newBufferLen - 1), _T("\0"), 1);
		
		return (newLinePosition - pBufferIn + 1);		// Current Position in the buffer is the one past the newline
	}
	else if ((newLinePosition = strchr(currentPosition,_T('\0'))))
	{
		// No new line, maybe there is an end of the string?
		newBufferLen = (newLinePosition - currentPosition);
		*ppBufferOut = (LPTSTR)malloc(newBufferLen);
		memcpy(*ppBufferOut, currentPosition, newBufferLen);
		memcpy(*ppBufferOut+(newBufferLen), _T("\0"), 1);
		
		return (newLinePosition - pBufferIn);			// Current Position in the buffer is the one past the newline
	}
	
	return -1; // No more lines to get
}

void _XMADWrapper::SetClientDisplayState( void )
{
	// Get the state of the Massive AdClient
	MAD_STATE currentState = CMassiveClientCore::Instance()->GetState();
	
	switch (currentState)
	{
		// Core Specific States
	case STATE_NOT_CONNECTED:
		sprintf(m_szTextClient, _T("%s"), _T("Not Connected"));
		break;
	case STATE_LOCATE_SERVICE:
		sprintf(m_szTextClient, _T("%s"), _T("Trying to Locate Services..."));
		break;
	case STATE_LOCATE_SERVICE_REQUEST:
		sprintf(m_szTextClient, _T("%s"), _T("Successfully Located Service"));
		break;
	case STATE_OPEN_SESSION:
		sprintf(m_szTextClient, _T("%s"), _T("Opening a Session..."));
		break;
	case STATE_OPEN_SESSION_REQUEST:
		sprintf(m_szTextClient, _T("%s"), _T("Successfully Opened Session."));
		break;
	case STATE_CONNECTED:
		sprintf(m_szTextClient, _T("%s"), _T("Connected Idle"));
		break;
	case STATE_ENTERZONE:
	case STATE_ENTERZONE_REQUEST:
		sprintf(m_szTextClient, _T("%s"), _T("Downloading zone..."));
		break;
	case STATE_EXITZONE_REQUEST:
	case STATE_EXITZONE:
		sprintf(m_szTextClient, _T("%s"), _T("Downloading zone..."));
		break;
	case STATE_MEDIA_REQUEST:
		sprintf(m_szTextClient, _T("%s"), _T("Requesting Media..."));
		break;
	case STATE_READY:
		sprintf(m_szTextClient, _T("%s"), _T("Ready"));
		break;
	case STATE_ERROR:
		sprintf(m_szTextClient, _T("%s"), _T("ERROR: Client is in Error State."));
		break;
	default:
		sprintf(m_szTextClient, _T("%s"), _T("UNKNOWN: Client State."));
		break;
	}
}

BOOL _XMADWrapper::SetMADLocation( LPTSTR szLocationName )
{
	std::vector<_XMADZONE_SETUPDATA*>::iterator maddataiter;
	int index = 0;
	for (maddataiter= m_ZoneSetupData.begin(); maddataiter != m_ZoneSetupData.end(); maddataiter++ ) 
	{
		_XMADZONE_SETUPDATA* pSetupData = (_XMADZONE_SETUPDATA*)(*maddataiter);
		
		if( pSetupData )
		{
			if( strncmp( pSetupData->locationname, szLocationName, strlen(szLocationName) ) == 0 )
			{
				if( !MassiveEnterZone(pSetupData->zonename) )
				{
					_XFatalError( _T("MADWrapper : Failed enter zone") );
					m_CurrentLocationIndex = -1;
					return FALSE;
				}

				m_CurrentLocationIndex = index;

				for( int i = 0; i < pSetupData->uNumElements; i++ )
				{
					m_Subscribers.insert(m_Subscribers.begin(), new _XMVTSubscriberObject( pSetupData->szElementNames[i]) );
				}

				if( pSetupData->uNumElements > 0 )
				{
					m_PrevSubscribers = m_CurrentSubscribers = 0;

					_XMVTSubscriberObject* pSubscriberobject = m_Subscribers[0];
					if( pSubscriberobject )
					{
						pSubscriberobject->m_bInView = TRUE;
					}
				}

				m_NeedToFlush = TRUE;

				CreateMADWindow();

				break;
			}
		}		

		index++;
	}

	return TRUE;
}

void _XMADWrapper::DisableMADLocation( LPTSTR szLocationName )
{
	std::vector<_XMADZONE_SETUPDATA*>::iterator maddataiter;
	for (maddataiter= m_ZoneSetupData.begin(); maddataiter != m_ZoneSetupData.end(); maddataiter++) 
	{
		_XMADZONE_SETUPDATA* pSetupData = (_XMADZONE_SETUPDATA*)(*maddataiter);
		
		if( pSetupData )
		{
			if( strncmp( pSetupData->locationname, szLocationName, strlen(szLocationName) ) == 0 )
			{
				if( !MassiveExitZone(pSetupData->zonename) )
				{
					//_XFatalError( _T("MADWrapper : Failed exit zone") );
					m_CurrentLocationIndex = -1;
					//return;
				}

				// Remove Subscribers
				_XLog( _T("_XMADWrapper : Remove MAD Subscribers") );

				/*
				std::vector<_XMVTSubscriberObject*>::iterator iter;
				for (iter = m_Subscribers.begin(); iter != m_Subscribers.end(); iter++) 
				{
					(*iter)->ReleaseDeviceObject();
					delete (_XMVTSubscriberObject*)(*iter);
				}
				*/

				std::for_each( m_Subscribers.begin(), m_Subscribers.end(), DELETEOBJECT() );
				m_Subscribers.clear();
				
				m_SubscriberCounter		= 0;
				m_CurrentSubscribers	= -1;
				m_PrevSubscribers		= -1;

				m_NeedToFlush = FALSE;

				break;
			}
		}		
	}

	m_CurrentLocationIndex = -1;
}

BOOL _XMADWrapper::MassiveEnterZone(LPTSTR szName)
{
	// Validity Checks
	if (!szName || !IsMassiveInitialized())
		return FALSE;
	
	// Log Application Messages to the Massive AdClient Log
	_XLog( _T("_XMADWrapper : Enter Zone: %s"), szName);
	CMassiveClientCore::Instance()->Log(LOG_LEVEL_INFO, _T("NineDragons"), _T("Enter Zone: %s"), szName);	
	
	return CMassiveClientCore::Instance()->EnterZone(szName);
}

BOOL _XMADWrapper::MassiveExitZone(LPTSTR szName)
{
	// Validity Check
	if (!szName || !IsMassiveInitialized())
		return FALSE;
	
	// Log Application Messages to the Massive AdClient Log
	_XLog( _T("_XMADWrapper : Exit Zone: %s"), szName);
	CMassiveClientCore::Instance()->Log(LOG_LEVEL_INFO, _T("NineDragons"), _T("Exit Zone: %s"), szName);	
	return CMassiveClientCore::Instance()->ExitZone(szName);
}

BOOL _XMADWrapper::MassiveTick( void )
{	
	// Tick the Massive AdClient
	if (IsMassiveInitialized())
		CMassiveClientCore::Instance()->Tick();
	
	return true;
}

BOOL _XMADWrapper::MassiveFlushImpressions( void )
{
	// Validity Check
	if (!IsMassiveInitialized())
		return false;
	
	// Log Application Messages to the Massive AdClient Log
	_XLog( _T("_XMADWrapper : FlushImpressions"));
	CMassiveClientCore::Instance()->Log(LOG_LEVEL_INFO, _T("NineDragons"), _T("FlushImpressions"));
	
	sprintf(m_szTextApp, _T("Flushing Impressions..."));
	_XLog( _T("_XMADWrapper : Flushing Impressions..."));

	massiveBool bResult = CMassiveClientCore::Instance()->FlushImpressions();
	
	sprintf(m_szTextApp, _T("Flushing Impressions Complete"));
	_XLog( _T("_XMADWrapper : Flushing Impressions Complete"));
	
	// Todo Check Status?
	return bResult;
}

BOOL _XMADWrapper::Update( void )
{
	if( m_bMassiveInitialized )
	{
		if( m_CurrentLocationIndex >= 0 && m_CurrentSubscribers != -1 )
		{
			if( m_LastCyclingTime != 0 )
			{
				_XMADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[m_CurrentLocationIndex];

				if( pSetupData->cyclinginterval > 0 )
				{
					if( pSetupData->uNumElements > 0 )
					{
						int _curtime = g_LocalSystemTime;
						if( (_curtime - m_LastCyclingTime) > pSetupData->cyclinginterval )
						{
							m_LastCyclingTime = _curtime;
							
							m_PrevSubscribers = m_CurrentSubscribers;

							m_CurrentSubscribers++;

							if( m_CurrentSubscribers > pSetupData->uNumElements-1 )
							{
								m_CurrentSubscribers = 0;
							}

							for( int i = 0; i < pSetupData->uNumElements; i++ )
							{
								m_Subscribers[i]->m_bInView = FALSE;
							}
							
							_XMVTSubscriberObject* pSubscriberobject = m_Subscribers[m_CurrentSubscribers];
							if( pSubscriberobject )
							{
								pSubscriberobject->m_bInView = TRUE;
							}

							m_NeedToFlush = TRUE;
						}
					}
				}
			}
			//else
			//			{
			//				m_LastCyclingTime = g_LocalSystemTime;
			//			}
			
		}
	}


	// Update subscriber...
	// Update all of our Subscribers
	std::vector<_XMVTSubscriberObject*>::iterator iter;
	for (iter = m_Subscribers.begin(); iter != m_Subscribers.end(); iter++) 
	{
		(_XMVTSubscriberObject*)(*iter)->Update();
	}

	// Tick/Update the Massive AdClient
	MassiveTick();

	return TRUE;
}


BOOL _XMADWrapper::InitDeviceObject( void )
{
	if( m_Subscribers.size() > 0 )
	{
		std::vector<_XMVTSubscriberObject*>::iterator iter;
		for (iter = m_Subscribers.begin(); iter != m_Subscribers.end(); iter++) 
		{
			if( !(_XMVTSubscriberObject*)(*iter)->InitDeviceObject() )
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL _XMADWrapper::RestoreDeviceObject( void )
{
	if( m_Subscribers.size() > 0 )
	{
		std::vector<_XMVTSubscriberObject*>::iterator iter;
		for (iter = m_Subscribers.begin(); iter != m_Subscribers.end(); iter++) 
		{
			if( !(_XMVTSubscriberObject*)(*iter)->RestoreDeviceObject() )
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

void _XMADWrapper::ReleaseDeviceObject( void )
{
	if( m_Subscribers.size() > 0 )
	{
		std::vector<_XMVTSubscriberObject*>::iterator iter;
		for (iter = m_Subscribers.begin(); iter != m_Subscribers.end(); iter++) 
		{
			//(_XMVTSubscriberObject*)
				(*iter)->ReleaseDeviceObject();
		}
	}
}

void _XMADWrapper::CreateMADWindow( void )
{
	if( m_CurrentLocationIndex < 0 ) return;
	_XMADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[m_CurrentLocationIndex];

	m_WindowSize.cx = pSetupData->imagesize.cx + 8;
	m_WindowSize.cy = pSetupData->imagesize.cy + 20;

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
			m_WindowPosition.x = (gnWidth>>1) - (m_WindowSize.cx>>1);
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
	}

	int windowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_main_back01.tga" );
		
	m_TitleLeftBar.Create( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+80, m_WindowPosition.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleLeftBar.SetClipRect( 1, 126, 81, 138 );

	m_TitleCenterBar.Create( m_WindowPosition.x+80, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-6, m_WindowPosition.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleCenterBar.SetClipRect( 84, 126, 94, 138 );

	FLOAT scalefactor = ((FLOAT)(m_WindowSize.cx-86)) / 10.0f;
	m_TitleCenterBar.SetScale( scalefactor, 1.0f );

	m_TitleRightBar.Create( m_WindowPosition.x+m_WindowSize.cx-6, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+12, &g_MainInterfaceTextureArchive, windowimageindex );
	m_TitleRightBar.SetClipRect( 177, 126, 183, 138 );
	
	m_SubscriberImage.Create( 0, 0, 256, 256, &g_MainInterfaceTextureArchive, -1 );
}

BOOL _XMADWrapper::GetDownloadComplete( void )
{
	if( m_CurrentSubscribers > -1 )
	{
		_XMVTSubscriberObject* pSubscriberobject = m_Subscribers[m_CurrentSubscribers];
		
		if( pSubscriberobject->m_pTexture )
		{
			return TRUE;
		}
	}

	return FALSE;
}

int  _xmad_indicatorcolorlevel  = 255;
BOOL _xmad_indicatordirection   = FALSE;
int	 _xmad_indicatortimer		= 0;

void _XMADWrapper::DrawMADWindow(void)
{
	int curtimer = g_LocalSystemTime;
	if( _xmad_indicatortimer == 0 )
	{
		_xmad_indicatortimer = curtimer;
	}
	if( curtimer - _xmad_indicatortimer > 30 )
	{
		_xmad_indicatortimer = curtimer;

		if( _xmad_indicatordirection )
		{
			_xmad_indicatorcolorlevel+=3;
			if( _xmad_indicatorcolorlevel > 255 )
			{
				_xmad_indicatorcolorlevel = 255;
				_xmad_indicatordirection = FALSE;
			}			
		}
		else
		{
			_xmad_indicatorcolorlevel-=3;
			if( _xmad_indicatorcolorlevel < 64 )
			{
				_xmad_indicatorcolorlevel = 64;
				_xmad_indicatordirection = TRUE;
			}
		}		
	}

	if( m_CurrentSubscribers > -1 )
	{
		_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y + 12, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, 0x7F000000 );
		_XDrawSolidBar( m_WindowPosition.x+1, m_WindowPosition.y + 13, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0x40000000 );
		
		m_TitleCenterBar.Draw();
		m_TitleLeftBar.Draw();
		m_TitleRightBar.Draw();
	
		_XMVTSubscriberObject* pSubscriberobject = m_Subscribers[m_CurrentSubscribers];

		if( pSubscriberobject->m_pTexture )
		{
			if( m_NeedToFlush ) // download finished, flush impression.
			{
				m_NeedToFlush = FALSE;
				MassiveFlushImpressions();
				m_LastCyclingTime = g_LocalSystemTime;
			}

			m_SubscriberImage.SetClipRect( 0, 0, pSubscriberobject->m_ImageWidth, pSubscriberobject->m_ImageHeight );

			_XMADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[m_CurrentLocationIndex];
			
			if( pSetupData )
			{
				m_SubscriberImage.SetScale( (FLOAT)pSetupData->imagesize.cx / (FLOAT)pSubscriberobject->m_ImageWidth,
					(FLOAT)pSetupData->imagesize.cy / (FLOAT)pSubscriberobject->m_ImageHeight );
				
				m_SubscriberImage.Draw( m_WindowPosition.x + (m_WindowSize.cx>>1) - (pSetupData->imagesize.cx>>1), 
					m_WindowPosition.y + ((m_WindowSize.cy+12)>>1) - (pSetupData->imagesize.cy>>1), pSubscriberobject->m_pTexture );
			}
		}
		else
		{
			if( m_PrevSubscribers > -1 )
			{
				_XMVTSubscriberObject* pSubscriberobject = m_Subscribers[m_PrevSubscribers];
				
				if( pSubscriberobject->m_pTexture )
				{
					m_SubscriberImage.SetClipRect( 0, 0, pSubscriberobject->m_ImageWidth, pSubscriberobject->m_ImageHeight );
					
					if( m_CurrentLocationIndex >= 0 )
					{
						_XMADZONE_SETUPDATA* pSetupData = m_ZoneSetupData[m_CurrentLocationIndex];

						if( pSetupData )
						{
							m_SubscriberImage.SetScale( (FLOAT)pSetupData->imagesize.cx / (FLOAT)pSubscriberobject->m_ImageWidth,
														(FLOAT)pSetupData->imagesize.cy / (FLOAT)pSubscriberobject->m_ImageHeight );

							m_SubscriberImage.Draw( m_WindowPosition.x + (m_WindowSize.cx>>1) - (pSetupData->imagesize.cx>>1), 
													m_WindowPosition.y + ((m_WindowSize.cy+12)>>1) - (pSetupData->imagesize.cy>>1), pSubscriberobject->m_pTexture );
						}
					}
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,_xmad_indicatorcolorlevel,_xmad_indicatorcolorlevel,_xmad_indicatorcolorlevel) );
					g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 12 + ((m_WindowSize.cy - 12)>>1), 
											_XFONT_ALIGNTYPE_CENTER, _T("Downloading...") );
				}
			}
			else
			{
				g_XBaseFont->SetColor( D3DCOLOR_ARGB(255,_xmad_indicatorcolorlevel,_xmad_indicatorcolorlevel,_xmad_indicatorcolorlevel) );
				g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 12 + ((m_WindowSize.cy - 12)>>1), 
										_XFONT_ALIGNTYPE_CENTER, _T("Downloading...") );
			}
		}

#ifdef _XDWDEBUG		
		if( m_GameData.uLogLevel == 5 )
		{
			switch (pSubscriberobject->GetViewState())
			{
			default:
			case 0:
				g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,255,0,0) );
				g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 15, _XMAD_STRING_INVALID );
				break;
			case 1:
				g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
				g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 15, _XMAD_STRING_VIEW );
				break;
			case 2:
				g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,0,255) );
				g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 15, _XMAD_STRING_IMPRESSION );
				break;
			case 3:
				g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,128,128,128) );
				g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 15, _XMAD_STRING_OFFSCREEN );
				break;
			}
		}
#endif
	
		g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,255,255,255) );
		g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 1, _T("Advertisement") );
		g_XBaseFont->Flush();
	}

#ifdef _XDWDEBUG 
	MAD_STATE currentState = CMassiveClientCore::Instance()->GetState();

	if( m_GameData.uLogLevel == 5 )
	{
		switch (currentState)
		{
			// Core Specific States
		case STATE_NOT_CONNECTED:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,255,0,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Not Connected" );		
			break;
		case STATE_LOCATE_SERVICE:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Trying to Locate Services..." );
			break;
		case STATE_LOCATE_SERVICE_REQUEST:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Successfully Located Service" );				
			break;
		case STATE_OPEN_SESSION:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Opening a Session...");
			break;
		case STATE_OPEN_SESSION_REQUEST:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Successfully Opened Session.");
			break;
		case STATE_CONNECTED:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Connected Idle");
			break;
		case STATE_ENTERZONE:
		case STATE_ENTERZONE_REQUEST:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Downloading zone...");
			break;
		case STATE_EXITZONE_REQUEST:
		case STATE_EXITZONE:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Downloading zone...");
			break;
		case STATE_MEDIA_REQUEST:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Requesting Media...");
			break;
		case STATE_READY:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,0,255,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "Ready");
			break;
		case STATE_ERROR:
			{
				g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,255,0,0) );
				MAD_INTERNAL_ERROR errorcode = CMassiveClientCore::Instance()->GetLastError();
				g_XBaseFont->Print( m_WindowPosition.x + 10, m_WindowPosition.y + 27, 1.0f, "ERROR: Client is in Error State. [%d]", errorcode );
			}
			break;
		default:
			g_XBaseFont->SetColor( D3DCOLOR_ARGB(160,255,0,0) );
			g_XBaseFont->Puts( m_WindowPosition.x + 10, m_WindowPosition.y + 27, "UNKNOWN: Client State.");
			break;
		}

		g_XBaseFont->Flush();
	}
#endif
	
}


#endif



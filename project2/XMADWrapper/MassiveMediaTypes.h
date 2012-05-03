//----------------------------------------------------------------------------
//
// © 2005 Massive Incorporated All Rights Reserved.
// This code is confidential and proprietary to Massive Incorporated, is ONLY
// for use by the direct recipient for the sole purpose of the integration into
// the represented software and must not be used for any other purpose, further
// distributed or reverse engineered.
//
//----------------------------------------------------------------------------
//
// Definitions for AdClient objects and types.
//

//----------------------------------------------------------------------------
#ifndef ___MASSIVEMEDIATYPES_H___
#define ___MASSIVEMEDIATYPES_H___
//----------------------------------------------------------------------------

#ifdef _XDEF_MADSYSTEM

namespace MassiveAdClient3
{

// Memory
typedef void *(*massiveMalloc) (size_t);
typedef void (*massiveFree) (void *);

typedef enum __MAD_INTERNAL_ERROR
{
	MAD_INTERNAL_SUCCESS = 0,					// Successfully handled request

	MAD_INTERNAL_GENERROR = -100,				// Unclassified error code
	MAD_INTERNAL_MEMORY_ERROR,					// Couldn't allocate any more memory
	MAD_INTERNAL_INVALID_SESSION,				// Bad Session ID
	MAD_INTERNAL_MULTIPLE_SESSIONS_ERROR,		// Cannot join multiple sessions, call MAD_INTERNAL_LeaveSession()

	// Client Core
	MAD_INTERNAL_CORE_NOT_INITIALIZED = -200,
	MAD_INTERNAL_CORE_ALREADY_INITIALIZED,
	MAD_INTERNAL_CORE_INVALID_INITSTRUCT,
	MAD_INTERNAL_CORE_INVALID_SKU,
	MAD_INTERNAL_CORE_INVALID_PUBLIC_KEY,
	MAD_INTERNAL_CORE_INVALID_GUID,
	MAD_INTERNAL_CORE_IMPRESSION_TIME_NOT_REACHED,
	MAD_INTERNAL_CORE_TICK_TIME_NOT_REACHED,
	MAD_INTERNAL_CORE_NO_CURRENT_ZONE,
	MAD_INTERNAL_CORE_FAILED_LOCATE_SERVICE,
	MAD_INTERNAL_CORE_FAILED_OPEN_SESSION,
	MAD_INTERNAL_CORE_FAILED_CLOSE_SESSION,

	// ZoneManagers,
	MAD_INTERNAL_ZONE_INVALID_NAME = -300,
	MAD_INTERNAL_ZONE_FAILED_IMPUPDATE_REQUEST_CREATION,
	MAD_INTERNAL_ZONE_FAILED_ENTERZONE,	
	MAD_INTERNAL_ZONE_FAILED_EXITZONE,

	// MAO
	MAD_INTERNAL_MAO_INVALID_NAME = -400,
	MAD_INTERNAL_MAO_FAILED_CREATION,
    MAD_INTERNAL_MAO_EXISTS,
	MAD_INTERNAL_INVALID_ASSET,
	MAD_INTERNAL_MAO_FAILED_ASSET_DOWNLOAD,
	MAD_INTERNAL_MAO_FAILED_ROTATE,
	MAD_INTERNAL_MAO_INVALID_DATA_BUFFER,
	MAD_INTERNAL_MAO_NO_CURRENT_VIEW_RECORD,
	MAD_INTERNAL_MAO_IS_PAUSED,
	MAD_INTERNAL_MAO_FAILED_IMPRESSION_REPORT,

	// Subscriber
	MAD_INTERNAL_MAOSUB_INVALID_NAME = -500,
	MAD_INTERNAL_MAOSUB_EXISTS,

	// Asset
	MAD_INTERNAL_ASSET_INVALID_ID = -600,
	MAD_INTERNAL_ASSET_EXISTS,
	MAD_INTERNAL_ASSET_FAILED_DOWNLOAD,
	MAD_INTERNAL_ASSET_INVALID_URL,
	MAD_INTERNAL_ASSET_INVALID_HASH,
	MAD_INTERNAL_ASSET_INVALID_SIZE,
	MAD_INTERNAL_ASSET_INVALID_DATE,
	MAD_INTERNAL_ASSET_INVALID_DATALENGTH,

    // Request Manager
    MAD_INTERNAL_REQ_MAN_BAD_REQ_STATUS = -700,
    MAD_INTERNAL_REQ_MAN_NOT_WRITABLE,
    MAD_INTERNAL_REQ_MAN_CANT_ADD_TO_QUEUE,
    MAD_INTERNAL_REQ_MAN_CANT_FIND_REQUEST,
    MAD_INTERNAL_REQ_MAN_CANT_ACCESS_QUEUE,

    // Network Manager
    MAD_INTERNAL_NETWORK_SOCKET_ERROR = -800,
    MAD_INTERNAL_NETWORK_ADDRESS_NOT_RESOLVED,
    MAD_INTERNAL_NETWORK_DNS_ERROR,
    MAD_INTERNAL_NETWORK_SEND_ERROR,
    MAD_INTERNAL_NETWORK_RECV_ERROR,
    MAD_INTERNAL_NETWORK_CONNECT_ERROR,
    MAD_INTERNAL_NETWORK_DISCONNECT_ERROR,
    MAD_INTERNAL_NETWORK_BAD_STATUS,
    
    // Transaction
    MAD_INTERNAL_TRANSACTION_NULL_REQUEST = -900,
    MAD_INTERNAL_TRANSACTION_REQ_BAD_STATE,
    
    // RequestBuilder
    MAD_INTERNAL_REQ_BUILDER_CANT_ADD = -1000,
    MAD_INTERNAL_REQ_BUILDER_NOT_FOUND,
    MAD_INTERNAL_REQ_BUILDER_EMPTY,
    
    // Request
    MAD_INTERNAL_REQ_INVALID_PARAMETER = -1100,
    MAD_INTERNAL_REQ_SERVER_INDEX_OUT_OF_BOUNDS,
    MAD_INTERNAL_REQ_INVALID_STATUS,
    MAD_INTERNAL_REQ_GENERAL_ERROR,
    

} MAD_INTERNAL_ERROR;


//===========================================================================
//
// Define types
//
//===========================================================================

#if !defined massiveBool
	typedef int massiveBool;
#endif

#if !defined massiveUByte
	#undef massiveByte
	typedef unsigned char massiveUByte; //The best variable type ever!!!
	typedef signed char   massiveByte;
#endif

#if !defined massiveU8						
	#undef massiveI8
	typedef unsigned char massiveU8;		
	typedef signed   char massiveI8;
#endif

#if !defined massiveU16						
	#undef massiveI16
	typedef unsigned short massiveU16;				
	typedef signed   short massiveI16;
#endif

#if !defined massiveU32
	#undef massiveI32
	typedef unsigned int massiveU32;
	typedef signed   int massiveI32;
#endif

#if !defined massiveFloat
	typedef float massiveFloat;
#endif

//===========================================================================
// The AdClient libray can be configured at startup with certain behaviors

typedef enum __MAD_CONFIG
{
	MAD_DEFAULT_CONFIG_FLAGS		= 0x00000000, // No specific configuration needed
	MAD_MANUAL_MEDIABUFFER_FLUSH	= 0x00000001, // The Asset Media buffer will not be Automatically  Deleted
	MAD_DOWNLOAD_MEDIA_IMMEDIATE	= 0x00000002, // Tells the client to download all media immediately and not wait for subsribers
	MAD_NETWORK_FAST				= 0x00000020, // Doubles the normal TCP buffersize as well as the AdClients internal network buffer
	MAD_NETWORK_SMALL				= 0x00000040, // Halves the normal TCP buffersize as well as the AdClients internal network buffer
} MAD_CONFIG;

//----------------------------------------------------------------------------
// The AdClient library can deliver a variety of advertisements efficiently and
// securely. Game developers that use a proprietary image format should contact
// Massive or provide a runtime conversion for one of the supported types.
typedef enum __MAD_MEDIATYPE
{
	MAD_MEDIA_INVALID = 0,
	MAD_MEDIA_IMAGE_JPG,					// Standard JPEG image files
	MAD_MEDIA_IMAGE_GIF,					// GIF89 image files (special use only)
	MAD_MEDIA_IMAGE_PNG,					// Standard PNG image files
	MAD_MEDIA_IMAGE_BMP_24,					// 24 Bit Bitmap image files
	MAD_MEDIA_IMAGE_TGA,					// Targa image files
	MAD_MEDIA_IMAGE_DXT1,					// DirectX compressed texture format (1)
	MAD_MEDIA_IMAGE_DXT5,					// DirectX compressed texture format (5)
	MAD_MEDIA_IMAGE_CUSTOM,					// Custom image file definition (special use only)
	MAD_MEDIA_IMAGE_LEGACY_JPG_SUB,			// Special JPEG image files for subway demo
	MAD_MEDIA_IMAGE_DXT1_M,					// DirectX compressed texture format with mipmaps(1)
	MAD_MEDIA_IMAGE_DXT3_M,					// DirectX compressed texture format with mipmaps(3)
	MAD_MEDIA_IMAGE_DXT5_M,					// DirectX compressed texture format with mipmaps(5)
	MAD_MEDIA_IMAGE_DXT1_A,					// DirectX compressed texture format with alpha(1)
	MAD_MEDIA_IMAGE_DXT5_A,					// DirectX compressed texture format with alpha(5)
	MAD_MEDIA_IMAGE_DXT1_AM,				// DirectX compressed texture format with alpha and mipmaps(1)
	MAD_MEDIA_IMAGE_LEGACY_WAV,				// WAV Audio [Obsolete]
	MAD_MEDIA_IMAGE_LEGACY_BIK,				// BIK Video [Obsolete]
	MAD_MEDIA_IMAGE_LEGACY_3D_MODEL,		// 3D Model Generic [Obsolete]
	MAD_MEDIA_IMAGE_MASSIVE_JPG,			// **Massive Internal Use Only**
	MAD_MEDIA_IMAGE_TX2,					// TX2 Image Files
	MAD_MEDIA_IMAGE_BMP_8,					// 8 Bit Bitmap image files
	MAD_MEDIA_IMAGE_BMP_16,					// 16 Bit Bitmap image files
	MAD_MEDIA_IMAGE_BMP_32,					// 32 Bit Bitmap image files
	MAD_MEDIA_IMAGE_TGA_VC_COMPRESSED,		// VC Compressed Tga
	MAD_MEDIA_IMAGE_TGA_32,					// 32 Bit Targa files
	MAD_MEDIA_IMAGE_LEGACY_TGA_COMPRESSED,	// Legacy Targa files
	MAD_MEDIA_IMAGE_INTERACTIVE_AD_FC,		// Custom FC Interactive Ad
	MAD_MEDIA_IMAGE_CUSTOM_SC4,				// Custom SC4 File
	MAD_MEDIA_IMAGE_CUSTOM_IFF,				// Custom IFF File
	MAD_MEDIA_IMAGE_CUSTOM_Z,				// Custom Z File

	MAD_MEDIA_VIDEO_START = 1024,
	MAD_MEDIA_VIDEO_BINK,					// Bink Video
	MAD_MEDIA_VIDEO_SWF,					// SWF (Flash)
	MAD_MEDIA_VIDEO_ID,						// ID Video File

	MAD_MEDIA_AUDIO_START = 2048,
	MAD_MEDIA_AUDIO_WAV,					// Standard .wav file
	MAD_MEDIA_AUDIO_MP3,					// For future use
	MAD_MEDIA_AUDIO_SND,					// For future use
	MAD_MEDIA_AUDIO_CUSTOM, 				// For future use

	MAD_MEDIA_3DOBJ_START = 4096,
	MAD_MEDIA_MODEL_GENERIC,				// 3d model of unspecified type
	MAD_MEDIA_MODEL_3DS,					// 3ds model

	MAD_MEDIA_OTHER_START = 8192,
	MAD_MEDIA_OTHER_TEXT_FILE,				// Text file for url, other string data
	MAD_MEDIA_OTHER_FLV,					// Flash Video

	MAD_MEDIA_TYPES_END

} MAD_MEDIATYPE ;

//----------------------------------------------------------------------------
// Level of detail for the client to log in Debug and Logging Builds

typedef enum __MASSIVE_ENUM_LOG_LEVEL
{
	LOG_LEVEL_NONE = 0,
	LOG_LEVEL_SEVERE,		// Malloc Fails, Internal Null Pointer, Network Error
	LOG_LEVEL_ERROR,		// Null Pointer passed to Lib, Parsing Error
	LOG_LEVEL_WARNING,		// Non critical problems/events
	LOG_LEVEL_INFO,			// Success/Fail of Transactions, Zone Names, IE Names
	LOG_LEVEL_DEBUG,		// Object Dumps, Full Request/Response Dumps, IE spreadsheet
	LOG_LEVEL_TRACE,		// Fuction Calls

	LOG_LEVEL_MAX			// Max Log Level

} MASSIVE_ENUM_LOG_LEVEL;

//----------------------------------------------------------------------------
// Types of Interactions with Objects

typedef enum __MASSIVE_INTERACTION
{
	INTERACTION_NONE = 0,	// No Interaction
	INTERACTION_USE,		// Use, Open, Activate
	INTERACTION_HOSTILE,	// Attack, shoot, melee etc
	INTERACTION_CONSUME,	// Eat, drink, etc
	INTERACTION_PICKUP,		// Pickup, move, carry etc
	INTERACTION_AREA,		// Enter, Leave area etc
	INTERACTION_BENEFIT,	// Beneficial to player
	INTERACTION_MAX

} MASSIVE_INTERACTION;

//----------------------------------------------------------------------------
// Subscriber View States

typedef enum __MAD_VIEW_STATE
{
	VIEW_STATE_INVALID = 0,
	VIEW_STATE_VIEW,
	VIEW_STATE_IMPRESSION,
	VIEW_STATE_OFFSCREEN,
	VIEW_STATE_MAX
} MAD_VIEW_STATE;


//----------------------------------------------------------------------------

#define MAD_RESTRICT_ROTATION_NONE				(0x00000000) // No specific configuration needed
#define MAD_RESTRICT_ZONE_ROTATION 				(0x00000001) // Notify game before uploading impression data
#define MAD_RESTRICT_LINE_OF_SIGHT_ROTATION     (0x00000002) // Have incoming binary data streamed via callback

//----------------------------------------------------------------------------
// The structure is passed in MassiveClientCore::Initialize() and MAD_Initialize()

typedef struct __MAD_MASSIVE_INIT_STRUCT
{
	char *szSku;								// Massive Assigned SKU
	char *szSkuVersion;							// Version of the SKU
	unsigned char *szPublicKey;					// Public Key
	unsigned short u_libraryConfigFlags;			// Massive Client Configuration Options
	MASSIVE_ENUM_LOG_LEVEL eLogLevel;			// Value from MASSIVE_ENUM_LOG_LEVEL
	unsigned long u_impressionFlushTime;		// Set to 0 to disable Auto Flush
	char *szThirdPartyID;						// Third Party Login ID
	char *szThirdPartyService;					// Third Party Service
} MAD_MASSIVE_INIT_STRUCT;

//----------------------------------------------------------------------------
// The structure is passed in MassiveClientCore::

typedef struct __MAD_MASSIVE_DEMOGRAPH_STRUCT
{
	char *szMassiveID;
} MAD_MASSIVE_DEMOGRAPH_STRUCT;


// TODO - how do we want to do Impressions
typedef struct _MAD_Impression
{
	// Texture, Video, Model Specific
	bool  bInView;
	short siSize;
	float fAngle;
	short siScreenWidth;
	short siScreenHeight;
	
	// Audio Specific
	bool  bPlayed;			
	float fFalloff;

}  MAD_Impression;



//===========================================================================
// Massive Object States

typedef enum __MAD_STATE
{
	// Standard States
	STATE_ERROR = 0,					// Error State
	STATE_INITIALIZING,					// Starting / Initializing State
	STATE_READY,						// Normal/Idle/Ready State
	STATE_PAUSED,						// Paused State
	STATE_SHUTDOWN,						// Object is shutting down

	// Core Specific States
	STATE_NOT_CONNECTED,				// Not Connected to the Massive
	STATE_LOCATE_SERVICE,				// Connecting
	STATE_LOCATE_SERVICE_REQUEST,		// Request Connection
	STATE_OPEN_SESSION,					// Opening Session
	STATE_OPEN_SESSION_REQUEST,			// Request Open Session
	STATE_CONNECTED,					// Connected (with Session)

	STATE_ENTERZONE,					// Receiving Zone information
	STATE_ENTERZONE_REQUEST,			// Request Zone information

	// ZoneManager Specific States
	STATE_EXITZONE,						// Exiting A Zone
	STATE_EXITZONE_REQUEST,				// Request to Exit a Zone

	// Mao Specific States
	STATE_MEDIA_REQUEST,
	STATE_MEDIA_DOWNLOAD,
	STATE_MEDIA_DOWNLOAD_COMPLETE,
	STATE_MEDIA_CLEAR_BUFFER,
	STATE_MEDIA_IDLE,
	STATE_MEDIA_ERROR,
	STATE_ROTATE_ASSET,

	STATE_MAX							// Max State for navigation

} MAD_STATE;

//----------------------------------------------------------------------------
//  HELPER DEFINITIONS
#define MAD_IS_MEDIA_IMAGE_TYPE(x)	((MAD_MEDIA_IMAGE_START < (x)) && (MAD_MEDIA_IMAGE_END > (x)))
#define MAD_IS_MEDIA_AUDIO_TYPE(x)	((MAD_MEDIA_AUDIO_START < (x)) && (MAD_MEDIA_AUDIO_END > (x)))
#define MAD_INVALID_HANDLE (0)
}

#endif

//----------------------------------------------------------------------------
#endif //__MASSIVEMEDIATYPES_H__
//----------------------------------------------------------------------------



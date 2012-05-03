#ifndef __BASEDEFS_H__
#define __BASEDEFS_H__
#ifdef __cplusplus

#include "xgamebase.h"	
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#define ZEROMEM(ptr,type,num) memset((ptr), 0, sizeof(type)*(num));

#define FILEID(A,B,C,D) ((A) | ((B)<<8) | ((C)<<16) | ((D)<<24))
#define ALPHAID		FILEID

// Safe memory freeing
#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=NULL; } }
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=NULL; } }
#define SAFE_LMDELETE(lm,p)		{ if(p) { lm->DeleteRef(p);	(p)=NULL; } }

// Helper for the printf-like function 
#define DOVSPRINTFEX(outbuf, str)	\
	va_list ap;					\
	va_start(ap, str);			\
	vsprintf(outbuf, str, ap);	\
	va_end(ap);

#define DOVSPRINTF(str)		\
	TCHAR strVBuffer[1024];	\
	DOVSPRINTFEX(strVBuffer, str)

#define DOUVSPRINTFEX(outbuf, str)	\
	va_list ap;					\
	va_start(ap, str);			\
	uvsprintf(outbuf, str, ap);	\
	va_end(ap);

#define DOUVSPRINTF(str)		\
	unchar ustrVBuffer[1024];	\
	DOUVSPRINTFEX(ustrVBuffer, str)

// If you're using MSVC6, you can comment out the above line and use the one
// below instead.
#define inline __forceinline

// classify return values
typedef enum _XClassify
{
	clFront,
	clBack,
	clCoplanar,
	clSpanned,
} _XClassify;

// Normally must be in XKernel.h, but it's better here
extern HWND			gHWnd;
extern HWND			gHWndFocus;
extern HINSTANCE	gHInst;
extern TCHAR		gModulePath[ _MAX_PATH ];
extern TCHAR		gCaptureFileSubFolder[_MAX_PATH];

void _XSetDefaultPath( LPSTR localpath  = NULL );
void _XRestorePrevPath( void );
void _XSetCaptureFileSubFolder( LPTSTR subpath );

#define _XDEF_DEFAULTPATH					"./Data"
#define _XDEF_DEFAULTPATH_ANIMATION			"./Data/Animation"
#define _XDEF_DEFAULTPATH_ENVIRONMENT		"./Data/Environment"
#define _XDEF_DEFAULTPATH_INTERFACE			"./Data/Interface"
#define _XDEF_DEFAULTPATH_MODEL				"./Data/Model"
#define _XDEF_DEFAULTPATH_TEXTURE			"./Data/Texture"
#define _XDEF_DEFAULTPATH_SCRIPTER			"./Data/Script"
#define _XDEF_DEFAULTPATH_EFFECTSCRIPT		"./Data/Script/EffectScripts"
#define _XDEF_DEFAULTPATH_CAPTURE			"./Capture"

typedef struct _XEnvironmentSet
{
	D3DCOLOR				ambientcolor;
	D3DCOLOR				ambientnextcolor;
	
	D3DXCOLOR				directionallight;
	D3DXCOLOR				directionallightnextcolor;	

	D3DCOLOR				fogcolor;
	D3DCOLOR				fognextcolor;
}_XEnvironmentSet;


typedef enum _XLANGUAGE_TYPE
{
	_XLANGUAGE_TYPE_KOREAN = 0,
	_XLANGUAGE_TYPE_VIETNAMESE,
	_XLANGUAGE_TYPE_TAIWANESE,
	_XLANGUAGE_TYPE_ENGLISH,
	_XLANGUAGE_TYPE_CHINESE,	
	_XLANGUAGE_TYPE_JAPANESE,
	_XLANGUAGE_TYPE_RUSSIAN,
	_XLANGUAGE_TYPE_THAI,
	_XLANGUAGE_TYPE_UNKNOWN
} _XLANGUAGE_TYPE;

typedef enum _XGraphicsQualityLevel
{
	_XGQLEVEL_LOW = 0,
		_XGQLEVEL_MIDDLE,	
		_XGQLEVEL_HIGH,
		_XGQLEVEL_MAX
};

typedef	struct _XGraphicsQualityProperty
{
	_XGraphicsQualityLevel	terrainqualitylevel;
	_XGraphicsQualityLevel	characterqualitylevel;
	_XGraphicsQualityLevel	npcqualitylevel;
	_XGraphicsQualityLevel	monsterqualitylevel;
	_XGraphicsQualityLevel	worldobjectqualitylevel;
	
	_XGraphicsQualityLevel	detailttilemiplevel;
	_XGraphicsQualityLevel	worldobjectmiplevel;
	_XGraphicsQualityLevel	charactermiplevel;
	
	_XGraphicsQualityLevel	sightrange;
	_XGraphicsQualityLevel	sightangle;
	
	_XGraphicsQualityProperty()
	{
		terrainqualitylevel		= _XGQLEVEL_MAX;
		characterqualitylevel	= _XGQLEVEL_MAX;
		npcqualitylevel			= _XGQLEVEL_MAX;
		monsterqualitylevel		= _XGQLEVEL_MAX;	
		worldobjectqualitylevel	= _XGQLEVEL_MAX;
		detailttilemiplevel		= _XGQLEVEL_MAX;
		worldobjectmiplevel		= _XGQLEVEL_MAX;
		charactermiplevel		= _XGQLEVEL_MAX;
		sightrange				= _XGQLEVEL_MIDDLE;
		sightangle				= _XGQLEVEL_MIDDLE;
	};
	
} _XGraphicsQualityProperty;

/////////////////////////////////////////////////////////////////////////////
// Network user message

#define WM_USER_NETWORKBASE					(WM_USER + 1930)
#define WM_USER_SOCKET						(WM_USER_NETWORKBASE + 1)
#define WM_USER_GETHOST						(WM_USER_NETWORKBASE + 2)
#define WM_USER_UDPSOCKET					(WM_USER_NETWORKBASE + 3)
#define WM_USER_MESSENGERSOCKET				(WM_USER_NETWORKBASE + 4)



/////////////////////////////////////////////////////////////////////////////
// Default camera info
#define _XDEF_CAMERA_DEFAULTFOV				40.0f
#define _XDEF_CAMERA_DEFAULTASPECT			1.0f
#define _XDEF_CAMERA_DEFAULTNEARPLANE		0.1f
#define _XDEF_CAMERA_DEFAULTFARPLANE		250.0f







#endif // __cplusplus
#endif // __BASEDEFS_H__

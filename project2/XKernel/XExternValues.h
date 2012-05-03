#ifndef __XEXTERNDEFS_H__
#define __XEXTERNDEFS_H__
#ifdef __cplusplus

#include "xgamebase.h"	
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "XBaseDefs.h"

#include "D3DFont.h"
#include "X3DUFont.h"
#include "XFont.h"
#include "XIME.h"
#include "XWindow.h"
#include "XWindow_IME.h"
#include "XTimeCounter.h"
#include "XRegistry.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// TripleX Core
////////////////////////////////////////////////////////////////////////////////////////////////
class X3DApp;
X3DApp*					 gApp										= NULL;
LPDIRECT3D9				 gpD3D										= NULL;	// The main D3D object
LPDIRECT3DDEVICE9		 gpDev										= NULL;	// The D3D rendering device
DWORD					 gnWidth									= 1024;
DWORD					 gnHeight									= 768;
bool					_XFullscreen								= true; 
D3DCAPS9				 g_d3dCaps;											// Caps for the device

BOOL					 g_reg_windowedmode							= 0;	// 0 == Fullscreen 1 == Windowed
int						 g_reg_screenwidth							= 1024;
int						 g_reg_screenheight							= 768;
int						 g_reg_screenrefreshrate					= 60;

// Frame counter
DWORD					gnFPS										= 0;
DWORD					gnFrameCounter								= 0;

// Windows stuffs
HWND					gHWnd;
HWND					gHWndFocus;
HINSTANCE				gHInst;

TCHAR					gModulePath[ _MAX_PATH ];
TCHAR					gPreviousPath[ _MAX_PATH ];
TCHAR					gCaptureFileSubFolder[_MAX_PATH];
TCHAR					gPackTemporaryFilePath[ _MAX_PATH ];

UINT					g_nPreviousState							= 0; 
HWND					g_DebugWindowHandle;
HWND					g_DebugWindowListHandle;							// Debug window list box handle



////////////////////////////////////////////////////////////////////////////////////////////////
// Render state machine controller
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _XADMINISTRATORMODE

bool					_XConsole									= false;
TCHAR					 g_LastCommandString[512];
HWND					 g_hRenderStateControllerWindow;					// Command window handle
HWND					 g_hRenderStateControllerEditWindow;				// Command window editbox handle
WNDPROC					 g_lpfnBaseEditWndProc;

#endif

bool					_XDebugWindow								= FALSE;
extern BOOL				 g_NormalGameExit;

////////////////////////////////////////////////////////////////////////////////////////////////
// Variable modified by the OSMSG callbacks
////////////////////////////////////////////////////////////////////////////////////////////////
bool					_XMsgClose									= false;
bool					_XAppActive									= false;

////////////////////////////////////////////////////////////////////////////////////////////////
// Language
////////////////////////////////////////////////////////////////////////////////////////////////
_XLANGUAGE_TYPE			g_LanguageType								= _XLANGUAGE_TYPE_KOREAN;
_XList					g_SubLanguageList;
int						g_SubLanguage								= 0;


////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Font
////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR						g_GUIFontName[64];
TCHAR						g_XFontName[64];
TCHAR						g_XLargeFontName[64];

_XFontManager_Baseclass*	g_XBaseFont		= NULL;
_XFontManager_Baseclass*	g_XSmallFont	= NULL;
_XFontManager_Baseclass*	g_XLargeFont	= NULL;

CD3DFont*					g_D3DNormalFont;

////////////////////////////////////////////////////////////////////////////////////////////////
// IME
////////////////////////////////////////////////////////////////////////////////////////////////
class _XInput;
_XInput*				gpInput = NULL;
_XWindow_IME			g_IMEWindow;
_XIMM					g_imm;
_XIMEKernel				g_IMEKernel;
int						g_LocalUserKeyboardRun						= 0;

////////////////////////////////////////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////////////////////////////////////////
LPD3DXSPRITE			g_pd3dxSprite;
D3DCOLOR				g_InterfaceColorFactor						= 0xFFFFFFFF;
BOOL					g_ViewMainInterface							= TRUE;
_XGUIObject*			g_CurrentFocusedObject;
_XWindowManager			g_MainWindowManager;
_XGUIControlCenter		g_XGUIControlCenter;
D3DCOLOR				g_SystemColorTable[16];

int						g_SelectCursorIndex							=	0;
int						g_SelectClickCursorIndex					=	0;
int						g_ClickSoundIndex							=	0;
int						g_WindowOpenSoundIndex						=	0;
int						g_WindowCloseSoundIndex						=	0;

////////////////////////////////////////////////////////////////////////////////////////////////
// Rendering state
_XRenderStyleManager	g_DefaultRenderStyleManager;
_XRenderStyleItem*		g_LastUsedRenderStyle						= NULL;

BOOL					g_RenderAlphaBlendFlag						= FALSE;
BOOL					g_RenderAlphaBlendUseAlphaChannelFlag		= FALSE;
D3DCOLOR				g_RenderAlphaBlendTextureFactor				= 0xFFFFFFFF;

D3DMATERIAL9			g_SelectedObjectMaterial;
D3DMATERIAL9			g_DamagedObjectMaterial;
BOOL					g_SelectedMaterialNoModulate2X				= FALSE;
D3DMATERIAL9*		    g_pCurrentSelectMaterial					= NULL;

////////////////////////////////////////////////////////////////////////////////////////////////
// Engine props
////////////////////////////////////////////////////////////////////////////////////////////////
DWORD					g_RS_UseZBuffer								= D3DZB_TRUE; //USEW;
DWORD					g_RS_ZBufferFunc							= D3DCMP_LESSEQUAL;
DWORD					g_AlphaRefLevel								= 64;
BOOL					g_TexturePreCacheing						= FALSE;

D3DPOOL					g_3DObjectMemoryPool						= D3DPOOL_MANAGED;
D3DPOOL					g_TextureMemoryPool							= D3DPOOL_MANAGED;
DWORD					g_3DObjectMemoryUsage						= D3DUSAGE_WRITEONLY;
DWORD					g_3DObjectLockMethods						= D3DLOCK_NOSYSLOCK;

DWORD					g_TextureQuality							= 1;
DWORD					g_MaxTextureWidth							= 256;
DWORD					g_MaxTextureHeight							= 256;
DWORD					g_LimitTextureWidth							= 256;
DWORD					g_LimitTextureHeight						= 256;


// Hardware confirm...
BOOL					g_UseHardwareCursor;
BOOL					g_bCanDoAlphaBlend;	
BOOL					g_bCanDoMultitexture;
BOOL					g_bCanDoAddSigned;
BOOL					g_bCanDoVertexFog;
BOOL					g_bCanDoRangeFog;
BOOL					g_bCanDoTableFog;

D3DXMATRIX				g_ViewMatrix;
D3DXMATRIX				g_ProjectionMatrix;
D3DXMATRIX				g_UserWorldMatrix;
D3DXVECTOR3				g_CameraPosition;
D3DXVECTOR3				g_CameraTargetPosition;
FLOAT					g_DefaultCameraFov							= _XDEF_CAMERA_DEFAULTFOV;
D3DVIEWPORT9			g_MainViewPort;

_XGraphicsQualityProperty	g_XGQProperty;

_XRegistryManager		g_RegManager;


////////////////////////////////////////////////////////////////////////////////////////////////
// Timer
////////////////////////////////////////////////////////////////////////////////////////////////
DWORD					g_LocalSystemTime							= 0;
FLOAT					g_fLocalSystemOrgTime						= 0.0f;
FLOAT					g_fLocalSystemTime							= 0.0f;
FLOAT					g_fPrevFrameTime							= 0.0f;
FLOAT					g_fElapsedFrameTime							= 0.0f;
FLOAT					g_fElapsedFrameMilisecondTime				= 0.0f;
_XTimeCounter			g_SystemTimeCounter;
_XTimer					g_GameTimer;


////////////////////////////////////////////////////////////////////////////////////////////////
// Application
////////////////////////////////////////////////////////////////////////////////////////////////
BOOL					g_NormalGameExit							= TRUE;


////////////////////////////////////////////////////////////////////////////////////////////////
// Package helper
////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR					g_LastAccessPackedFile[_MAX_PATH];


////////////////////////////////////////////////////////////////////////////////////////////////
// Debug helper
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _XDWDEBUG
BOOL					g_ViewNormalVector							= FALSE;
BOOL					g_UseTexture								= TRUE;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// gamma control
////////////////////////////////////////////////////////////////////////////////////////////////
FLOAT					g_fBrightness	=	0.0f;
FLOAT					g_fContrast		=	1.0f;
FLOAT					g_fGamma		=	1.0f;	
FLOAT					g_fBiasR		=	1.0f;
FLOAT					g_fBiasG		=	1.0f;
FLOAT					g_fBiasB		=	1.0f;
LONG					g_iLevels		=	256;
unsigned short			g_GammaTable[256*3];

////////////////////////////////////////////////////////////////////////////////////////////////
// Script
////////////////////////////////////////////////////////////////////////////////////////////////
TCHAR					g_XScriptDecryptFilename[64];

#endif // __cplusplus
#endif // __XEXTERNDEFS_H__
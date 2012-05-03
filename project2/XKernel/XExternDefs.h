#ifndef __XEXTERNDEFS_H__
#define __XEXTERNDEFS_H__
#ifdef __cplusplus

#include "xgamebase.h"	
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "D3DFont.h"
#include "X3DUFont.h"
#include "XFont.h"
#include "XRenderStyleManager.h"
#include "XIME.h"
#include "XWindow.h"
#include "XWindow_IME.h"
#include "XTimeCounter.h"
#include "XRegistry.h"

////////////////////////////////////////////////////////////////////////////////////////////////
// TripleX Core
////////////////////////////////////////////////////////////////////////////////////////////////
class X3DApp;
extern X3DApp*						gApp;
extern LPDIRECT3D9					gpD3D;							// The main D3D object
extern LPDIRECT3DDEVICE9			gpDev;							// The D3D rendering device
extern DWORD						gnWidth;
extern DWORD						gnHeight;
extern bool							_XFullscreen;
extern D3DCAPS9						g_d3dCaps;						// Caps for the device

extern BOOL							g_reg_windowedmode;
extern int							g_reg_screenwidth;
extern int							g_reg_screenheight;
extern int							g_reg_screenrefreshrate;

// Frame counter
extern DWORD						gnFPS;
extern DWORD						gnFrameCounter;

// Windows stuffs
extern HWND							gHWnd;
extern HWND							gHWndFocus;
extern HINSTANCE					gHInst;

extern TCHAR						gModulePath[ _MAX_PATH ];
extern TCHAR						gPreviousPath[ _MAX_PATH ];
extern TCHAR						gPackTemporaryFilePath[ _MAX_PATH ];

extern UINT							g_nPreviousState; 
extern HWND							g_DebugWindowHandle;
extern HWND							g_DebugWindowListHandle;		// Debug window list box handle



////////////////////////////////////////////////////////////////////////////////////////////////
// Render state machine controller
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _XADMINISTRATORMODE

extern bool							_XConsole;
extern TCHAR						g_LastCommandString[512];
extern HWND							g_hRenderStateControllerWindow;		// Command window handle
extern HWND							g_hRenderStateControllerEditWindow;	// Command window editbox handle
extern WNDPROC						g_lpfnBaseEditWndProc;

#endif

extern bool							_XDebugWindow;
extern BOOL							g_NormalGameExit;

////////////////////////////////////////////////////////////////////////////////////////////////
// Variable modified by the OSMSG callbacks
////////////////////////////////////////////////////////////////////////////////////////////////
extern bool							_XMsgClose;
extern bool							_XAppActive;

////////////////////////////////////////////////////////////////////////////////////////////////
// Language
////////////////////////////////////////////////////////////////////////////////////////////////
extern _XLANGUAGE_TYPE				g_LanguageType;
extern _XList						g_SubLanguageList;
extern int							g_SubLanguage;

////////////////////////////////////////////////////////////////////////////////////////////////
// GUI Font
////////////////////////////////////////////////////////////////////////////////////////////////
extern TCHAR						g_GUIFontName[64];
extern TCHAR						g_XFontName[64];
extern TCHAR						g_XLargeFontName[64];

extern CD3DFont*					g_D3DNormalFont;
extern _XFontManager_Baseclass*		g_XBaseFont;
extern _XFontManager_Baseclass*		g_XSmallFont;
extern _XFontManager_Baseclass*		g_XLargeFont;

////////////////////////////////////////////////////////////////////////////////////////////////
// IME
////////////////////////////////////////////////////////////////////////////////////////////////
class  _XInput;
extern _XInput*						gpInput;
extern _XIMEKernel					g_IMEKernel;
extern _XWindow_IME		 			g_IMEWindow;
extern int							g_LocalUserKeyboardRun;

////////////////////////////////////////////////////////////////////////////////////////////////
// Interface
////////////////////////////////////////////////////////////////////////////////////////////////
extern LPD3DXSPRITE					g_pd3dxSprite;
extern D3DCOLOR						g_InterfaceColorFactor;
extern BOOL							g_ViewMainInterface;
extern _XGUIObject*					g_CurrentFocusedObject;
extern _XWindowManager				g_MainWindowManager;
extern _XGUIControlCenter			g_XGUIControlCenter;
extern D3DCOLOR						g_SystemColorTable[16];

extern int							g_SelectCursorIndex;
extern int							g_SelectClickCursorIndex;
extern int							g_ClickSoundIndex;
extern int							g_WindowOpenSoundIndex;
extern int							g_WindowCloseSoundIndex;

////////////////////////////////////////////////////////////////////////////////////////////////
// Rendering state
////////////////////////////////////////////////////////////////////////////////////////////////
extern _XRenderStyleItem			g_DefaultRenderStyle;

extern _XRenderStyleManager			g_DefaultRenderStyleManager;
extern _XRenderStyleItem*			g_LastUsedRenderStyle;

extern BOOL							g_RenderAlphaBlendFlag;
extern BOOL							g_RenderAlphaBlendUseAlphaChannelFlag;
extern D3DCOLOR						g_RenderAlphaBlendTextureFactor;

extern D3DMATERIAL9					g_SelectedObjectMaterial;
extern D3DMATERIAL9					g_DamagedObjectMaterial;
extern BOOL							g_SelectedMaterialNoModulate2X;
extern D3DMATERIAL9*				g_pCurrentSelectMaterial;

////////////////////////////////////////////////////////////////////////////////////////////////
// Engine props
////////////////////////////////////////////////////////////////////////////////////////////////
extern DWORD						g_RS_UseZBuffer;
extern DWORD						g_RS_ZBufferFunc;
extern DWORD						g_AlphaRefLevel;
extern BOOL							g_TexturePreCacheing;

extern D3DPOOL						g_3DObjectMemoryPool;
extern D3DPOOL						g_TextureMemoryPool;
extern DWORD						g_TextureQuality;
extern DWORD						g_3DObjectMemoryUsage;
extern DWORD						g_3DObjectLockMethods;

extern DWORD						g_TextureQuality;
extern DWORD						g_MaxTextureWidth;
extern DWORD						g_MaxTextureHeight;
extern DWORD						g_LimitTextureWidth;
extern DWORD						g_LimitTextureHeight;

extern D3DXMATRIX					g_ViewMatrix;
extern D3DXMATRIX					g_ProjectionMatrix;
extern D3DXMATRIX					g_UserWorldMatrix;
extern D3DXVECTOR3					g_CameraPosition;
extern D3DXVECTOR3					g_CameraTargetPosition;
extern FLOAT						g_DefaultCameraFov;
extern D3DVIEWPORT9					g_MainViewPort;

extern _XGraphicsQualityProperty	g_XGQProperty;


// Hardware confirm...
extern BOOL							g_UseHardwareCursor;
extern BOOL							g_bCanDoAlphaBlend;	
extern BOOL							g_bCanDoMultitexture;
extern BOOL							g_bCanDoAddSigned;
extern BOOL							g_bCanDoVertexFog;
extern BOOL							g_bCanDoRangeFog;
extern BOOL							g_bCanDoTableFog;

extern _XRegistryManager			g_RegManager;

////////////////////////////////////////////////////////////////////////////////////////////////
// Timer
////////////////////////////////////////////////////////////////////////////////////////////////
extern DWORD						g_LocalSystemTime;
extern FLOAT						g_fLocalSystemTime;
extern FLOAT						g_fLocalSystemOrgTime;
extern FLOAT						g_fPrevFrameTime;
extern FLOAT						g_fElapsedFrameTime;
extern FLOAT						g_fElapsedFrameMilisecondTime;

extern _XTimeCounter				g_SystemTimeCounter;
extern _XTimer						g_GameTimer;

////////////////////////////////////////////////////////////////////////////////////////////////
// Application
////////////////////////////////////////////////////////////////////////////////////////////////
extern BOOL							g_NormalGameExit;

////////////////////////////////////////////////////////////////////////////////////////////////
// Package helper
////////////////////////////////////////////////////////////////////////////////////////////////
extern TCHAR						g_LastAccessPackedFile[_MAX_PATH];

////////////////////////////////////////////////////////////////////////////////////////////////
// Debug helper
////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _XDWDEBUG
extern BOOL					 		g_ViewNormalVector;
extern BOOL					 		g_UseTexture;
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// gamma control
////////////////////////////////////////////////////////////////////////////////////////////////
extern FLOAT					g_fBrightness;
extern FLOAT					g_fContrast;
extern FLOAT					g_fGamma;	
extern FLOAT					g_fBiasR;
extern FLOAT					g_fBiasG;
extern FLOAT					g_fBiasB;
extern LONG						g_iLevels;
extern unsigned short			g_GammaTable[256*3];


////////////////////////////////////////////////////////////////////////////////////////////////
// Script
////////////////////////////////////////////////////////////////////////////////////////////////
extern TCHAR					g_XScriptDecryptFilename[64];

#endif // __cplusplus
#endif // __XEXTERNDEFS_H__